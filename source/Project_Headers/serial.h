/**
 * \file serial.h
 * \brief Possui os prototipos das funcoes utilizadas em serial.c. 
 * 
 * Define os prototipos das funcoes utilizadas pelo arquivo serial.c
 * bem como as suas macros e variaveis externas.
 * 
 */

#ifndef SERIAL_H_

#define SERIAL_H_

#include "Cpu.h"
#include "Events.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "WDog.h"
#include "ASerial.h"
#include "timer.h"
#include "ctype.h"

/**
 * @def TAMANHO_BUFFER
 * @brief Define tamanho do buffer de informacoes.
 */
#define TAMANHO_BUFFER 512

/**
 * @def TEMPO_ESPERA
 * @brief Tempo de espera da serial
 */
#define TEMPO_ESPERA 10

// Senha para configuracao.
extern char_t SENHA[TAMANHO_BUFFER];

// Flag que indica que o usuario esta no modo de configuracao.
extern bool autenticado;

// Buffer que guardara os caracteres.
extern char_t buffer[TAMANHO_BUFFER];

/**
 * @brief Funcao que verifica se o buffer recebeu algum caracter.
 */
void verificaSerial(void);

/**
 * @brief Funcao que le string da serial e retorna se houve exito.
 */
bool leSerial(void);

/**
 * @brief Funcao que escreve mensagem da serial
 */
void escreveSerial(char *mensagem);

#endif
