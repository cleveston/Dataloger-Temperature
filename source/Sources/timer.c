/**
 * \file timer.c
 * \brief Este arquivo contem as configuracoes dos tres timers disponiveis ao programa. 
 *
 * Aqui esta contido como e implementado os tres timers
 * que sao utilizados durante a execucao do programa. 
 * 
 * \sa timer.h
 **/

#include "timer.h"

//Tempo de execucao para cada timer
static uint16_t tempo1 = 0;
static uint16_t tempo2 = 0;
static uint16_t tempo3 = 0;

//Contadores para os timers
static volatile uint16_t contador1 = 0;
static volatile uint16_t contador2 = 0;
static volatile uint16_t contador3 = 0;

//Funcao que ira executar quando o timer terminar a contagem
static void (*funcao1)(void);
static void (*funcao2)(void);
static void (*funcao3)(void);


/**
 * @fn void conta(unsigned short int timer, unsigned int time, void* funcao)
 * @brief Funcao que inicia contagem do timer escolhido. Ao final da contagem, executa a funcao passada como parametro
 * 
 * Faz a configuracao de algum dos tres timers a partir dos parametros
 * gerados por alguma outra funcao ou inseridos pelo usuario.
 * 
 * @param timer e o numero do contador desejado
 * @param time e o tempo que este contador deve contar
 * @param funcao deve ser executado apos a contagem
 **/
void conta(uint8_t timer, uint16_t time, void* funcao) {

	switch (timer) {
	case 1:
		tempo1 = time;
		contador1 = time;

		funcao1 = (void (*)()) funcao;

		TPM1SC = 0x00;
		TPM1CNT = 0x00;
		TPM1MOD = (CPU_BUS_CLK_HZ / 1000);
		TPM1SC = TPM1SC_CLKSA_MASK | TPM1SC_TOIE_MASK;

		break;

	case 2:
		tempo2 = time;
		contador2 = time;

		funcao2 = (void (*)()) funcao;

		TPM2SC = 0x00;
		TPM2CNT = 0x00;
		TPM2MOD = (CPU_BUS_CLK_HZ / 1000);
		TPM2SC = TPM2SC_CLKSA_MASK | TPM2SC_TOIE_MASK;

		break;
	case 3:
		tempo3 = time;
		contador3 = time;

		funcao3 = (void (*)()) funcao;

		TPM3SC = 0x00;
		TPM3CNT = 0x00;
		TPM3MOD = (CPU_BUS_CLK_HZ / 1000);
		TPM3SC = TPM3SC_CLKSA_MASK | TPM3SC_TOIE_MASK;

		break;
	}

}

/**
 * @fn void desligaTimer(int timer)
 * @brief Desliga o timer desejado.
 * 
 * Zera todos os bits de TPMxSC, desligando algum dos tres timers disponiveis.
 * 
 * @param timer que deseja ser encerrado.
 **/
void desligaTimer(uint8_t timer) {

	switch (timer) {
	case 1:
		TPM1SC = 0x00;
		break;

	case 2:
		TPM2SC = 0x00;
		break;

	case 3:
		TPM3SC = 0x00;
		break;
	}

}

/**
 * @fn ISR(timer1)
 * @brief Interrupcao do Timer 1
 **/
ISR(timer1) {

	TPM1SC_TOF = 0;

	//Conta ate terminar o periodo
	if (contador1 > 0) {

		contador1--;

	} else {

		//Inicia novo periodo
		contador1 = tempo1;

		//Executa funcao determinada
		funcao1();

	}

}

/**
 * @fn ISR(timer2)
 * @brief Interrupcao do Timer 2
 **/
ISR(timer2) {

	TPM2SC_TOF = 0;

	//Conta ate terminar o periodo
	if (contador2 > 0) {

		contador2--;

	} else {

		//Inicia novo periodo
		contador2 = tempo2;

		//Executa funcao determinada
		funcao2();

	}

}

/**
 * @fn ISR(timer3)
 * @brief Interrupcao do Timer 3
 **/
ISR(timer3) {

	TPM3SC_TOF = 0;

	//Conta ate terminar o periodo
	if (contador3 > 0) {

		contador3--;

	} else {

		//Inicia novo periodo
		contador3 = tempo3;

		//Executa funcao determinada
		funcao3();

	}

}

