/**
 * \file timer.h
 * \brief Possui os prototipos das funcoes utilizadas em timer.c. 
 * 
 * Define os prototipos das funcoes utilizadas pelo arquivo timer.c
 * 
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "Cpu.h"
#include "Events.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"


/**
 * @brief Funcao que inicia contagem do timer escolhido. Ao final da contagem, executa a funcao passada como parametro. 
 */
void conta(uint8_t timer, uint16_t time, void* funcao);


/**
 * @brief Funcao que desligar o timer. 
 */
void desligaTimer(uint8_t timer);

#endif
