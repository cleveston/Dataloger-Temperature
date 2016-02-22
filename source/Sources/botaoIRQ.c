/**
 * \file botaoIRQ.c
 * \brief Contem as funcoes que operam com o botao
 * 
 * Contem as manipulacoes que foram necessarias para
 * usar o botaoIRQ contido na placa da Freescale.
 * 
 * \sa botaoIRQ.h 
 */

#include "botaoIRQ.h"

// Flag que informa se o botao continua pressionado
static bool contando = FALSE;

// Variavel que guarda quanto tempo o botao esta apertado
uint8_t segundos = 0;

// Variavel que guarda o tempo em milissegundos
uint8_t mSegundos = 0;

// Variavel que guarda quantas vezes o botao foi apertado
uint8_t botaoApertado = 0;

// Flag que informa se o botao esta bloqueado para debounce
static bool botaoBloqueado = FALSE;

// Flag que informa se o botao esta pressionado
bool apertado = FALSE;

/**
 * \fn void configuraIRQ(void)
 * \brief Configura IRQ
 * 
 * Funcao que inicializa e manipula registradores para utilizar
 * o botao corretamente.
 **/
void configuraIRQ(void) {

	/* IRQSC: IRQIE=0 */
	IRQSC &= (unsigned char) ~0x02; /* Disable IRQ Interrupt */
	IRQSC |= (unsigned char) 0x10;
	IRQSC |= (unsigned char) 0x04;
	/* IRQSC: IRQIE=1 */
	IRQSC |= (unsigned char) 0x02; /* Enable IRQ Interrupt */
}

/**
 * @fn static void *contaSegundo(void)
 * @brief Incrementa a variavel que marca o tempo que o botao ficou apertado 
 * 
 * Essa funcao serve para verificar quanto tempo o botao ficou pressionado,
 * foi implementada para medir se o botao ficou apertado por mais de tres 
 * segundos. E para realizar o debounce.
 **/
static void *contaSegundo(void) {

	//Habilita o botao novamente
	botaoBloqueado = FALSE;

	mSegundos++;

	//Se se passou 1s incrementa contador
	if (mSegundos >= 10) {
		
		mSegundos = 0;

		segundos++;

		//Se o botao continua apertado
		if (PTAD_PTAD5 == 0) {

			apertado = TRUE;

		} else {

			apertado = FALSE;
		}
	}

	return 0;

}

/**
 * @fn void resetIRQ(void)
 * @brief Reseta valores do IRQ para novo clique
 *
 * Zera a variavel que conta o numero de cliques no botao,
 * reseta a flag de pressionado, desabilita contador de cliques
 * e zera a variavel que conta o tempo de aperto.
 **/
void resetIRQ(void) {

	//Zera cliques no botao
	botaoApertado = 0;
	
	//Desbloqueia botao do debounce
	botaoBloqueado = 0;

	//Reseta flag que marca se o bota o esta pressionado
	apertado = FALSE;

	//Desabilita flag do contador de cliques
	contando = FALSE;

	//Zera variavel que marca o tempo que o botao ficou pressionado
	segundos = 0;
	
	//Zera variavel que marca o tempo que o botao ficou pressionado
	mSegundos = 0;

	//Desliga timer que conta o tempo que o botao ficou pressionado
	desligaTimer(2);

}

/**
 * @fn ISR(InterruptIRQ)
 * @brief Interrupcao gerada pelo botao
 **/
ISR(interruptIRQ) {

	//Reseta interrupcao
	IRQSC_IRQACK = 1;

	//Trava botao para debounce
	if (botaoBloqueado == FALSE) {

		botaoBloqueado = TRUE;

		//Clique no botao
		botaoApertado++;

		//Verifica se o timer já iniciou
		if (contando == FALSE) {

			//Bloqueia timer a partir do primeiro clique
			contando = TRUE;

			//Conta por 100ms com o timer 2 e incrementa a variavel de tempo
			conta(2, 100, contaSegundo);

		}
	}
}
