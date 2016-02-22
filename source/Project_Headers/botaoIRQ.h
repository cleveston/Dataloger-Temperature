/**
 * \file botaoIRQ.h
 * \brief Possui os prototipos das funcoes utilizadas em botaoIRQ.c. 
 * 
 * Define os prototipos das funcoes utilizadas pelo arquivo botaoIRQ.c
 * bem como as suas variaveis externas.
 * 
 */

#ifndef BOTAOIRQ_H_

#define BOTAOIRQ_H_

#include "Cpu.h"
#include "Events.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "WDog.h"
#include "timer.h"
#include "relogio.h"

// Variavel que armazena quanto tempo o botao esta apertado
extern uint8_t segundos;

// Variavel que armazena quantas vezes o botao foi apertado
extern uint8_t botaoApertado;

// Flag que informa se o botao esta pressionado
extern bool apertado;


/**
 * @brief Configura IRQ
 **/
void configuraIRQ(void);


/**
 * @brief Reseta valores do IRQ para novo clique
 **/
void resetIRQ(void);

#endif 
