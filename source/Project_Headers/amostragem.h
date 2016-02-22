/**
 * \file amostragem.h
 * \brief Possui os prototipos das funcoes utilizadas em amostragem.c. 
 * 
 * Define os prototipos das funcoes utilizadas pelo arquivo amostragem.c
 * bem como as suas variaveis externas.
 * 
 */

#ifndef AMOSTRAGEM_H_
#define AMOSTRAGEM_H_

#include "Cpu.h"
#include "Events.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include <stdio.h>
#include "eeprom.h"
#include "serial.h"
#include "timer.h"
#include "relogio.h"

// Taxa de amostragem padrao de 10ms.
extern uint16_t TAXA_AMOSTRAGEM;

// Canal de conversao AD padrao 1.
extern uint8_t CANAL_AMOSTRAGEM;

// Flag que informa se a amostragem esta ocorrendo.
extern bool amostrando;


/**
 * @brief Configura o conversor A/D
 **/
void configuraAmostragem(void);


/**
 * @brief Inicia amostragem
 **/
void iniciaAmostragem(void);


/**
 * @brief Interrompe a realizacao da amostragem
 **/
void paraAmostragem(void);


/**
 * @brief Funcao que calibra o sensor.
 **/
void calibrarAmostragem(int16_t tempCalibra);


/**
 * @brief Realiza a amostragem.
 **/
uint16_t amostra(void);


/**
 * @brief Interrupcao que obtem a amostra.
 **/
void *getAmostra(void);

#endif
