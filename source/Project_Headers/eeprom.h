/**
 * \file eeprom.h
 * \brief Possui os prototipos das funcoes utilizadas em eeprom.c
 * 
 * Define os prototipos das funcoes utilizadas pelo arquivo eeprom.c
 * bem como as suas macros e variaveis externas. 
 * 
 **/

#ifndef EEPROM_H_
#define EEPROM_H_

#include "Cpu.h"
#include "Events.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "WDog.h"
#include "relogio.h"
#include "timer.h"
#include "serial.h"
#include "amostragem.h"

/**
 * @def Define como 1 se o dado for uma amostra
 **/
#define AMOSTRA 1

/**
 * @def Define como 2 se o dado for um evento
 **/
#define EVENTO 2

/**
 * @def Define como 1 o evento de baixa tensao
 **/
#define FALTA_ENERGIA 1

/**
 * @def Define como 2 o evento de memoria cheia
 **/
#define MEMORIA_CHEIA 2

/**
 * @def Define o endereco maximo da EEPROM = 2047
 **/
#define enderecoMax 0x07FF 

/**
 * @def Define o inicio do endereço da memoria para dados 
 **/
#define ENDERECO 6 

/**
 * @def Endereco da memoria EEPROM (10100000) 
 **/
#define escravoIIC 0xA0 

/**
 *  @def Define o valor padrao do delay da memoria
 **/
#define MEM_DELAY 800

/**
 * @def Valor que inicia um novo header
 **/
#define INIT_HEADER 127

/**
 * @def Valor padrao para a formatacao da memoria
 **/
#define INIT_FORMAT 126

/**
 * @union unionAddress
 * @brief Separa um endereco de 2 bytes em partes H/L de 1 byte
 **/
typedef union {
	uint16_t EE;
	struct {
		uint8_t H;
		uint8_t L;
	} Bytes;
} unionAddress;

/**
 * @struct HEADER
 * @brief Estrutura que sera salva na memoria
 **/
typedef struct header {
	uint8_t header;		//Inicio do header
	char_t tipo; 		//AMOSTRA ou EVENTO
	int16_t dado;		//TAXA AMOSTRAGEM ou TIPO DE EVENTO
	DATEREC date;
	TIMEREC time;
} HEADER;

extern uint16_t address;

// Flag que indica quando a memoria ficou cheia
extern bool memCheia;

/**
 * @brief Configura a memoria eeprom 
 **/
void configuraMem(void);

/**
 * @brief Funcao de espera
 **/
void delay(void);

/**
 * @brief Limpa os dados da memoria eeprom
 **/
void limparMem(bool mensagemSerial);

/**
 * @brief Le dado do endereco informado
 **/
//Le dado do endereco informado, popula struct e retorna se conseguiu ler ou nao
bool leDado(uint16_t endereco, int8_t *dadoTemporario);

/**
 * @brief Le header do inicio do bloco dos dados 
 **/
bool leHeader(uint16_t endereco, HEADER* dadoTemporario);

/**
 * @brief Escreve um dado desejado na memoria
 **/
void escreveMem(uint16_t endereco, byte dado);

/**
 * @brief Salva struct na memoria
 **/
void salvaHeader(HEADER *cabecalho);

/**
 * @brief Salva o dado amostrado na memoria eeprom
 **/
void salvaAmostra(int8_t dadoAmostra);

#endif /* EEPROM_H_ */
