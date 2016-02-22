/**
 * \file amostragem.c
 * \brief Contem as funcoes que manipulam como e realizada a amostragem
 * 
 * Possui as funcoes de como e configurada e realizada a amostragem e 
 * como a mesma e salva na memoria. Tambem possui as funcoes de 
 * calibragem para realizar a amostragem.
 * 
 * \sa amostragem.h 
 */

#include "amostragem.h"

// Taxa de amostragem padrao de 5000ms
uint16_t TAXA_AMOSTRAGEM = 5000;

// Canal de conversao AD padrao 1
uint8_t CANAL_AMOSTRAGEM = 1;

// Flag que informa se a amostragem esta ocorrendo
bool amostrando = FALSE;

// Valores para calibragem
static int16_t mCalibraF = 0;
static uint16_t mCalibraQ = 0;

/**
 * @fn void configuraAmostragem(void)
 * @brief Configura o conversor A/D
 * 
 * Faz a configuracao do conversor A/D usando 10 bits, clock
 * assincrono de 1MHz e habilitando o bandgap
 * 
 **/
void configuraAmostragem() {

	//Reset ADCSC1 e ADCSC2
	ADCSC1 = 0b00011111;
	ADCSC2 = 0;

	//Low power, long sample, 10 bits, Asynchronous clock/2 = 1MHz
	ADCCFG = 0b01000100;

	//Habilita bandgap buffer
	SPMSC1_BGBE = 1;

	ADCSC1 = CANAL_AMOSTRAGEM;
}

/**
 * @fn void calibrarAmostragem(int tempCalibra)
 * @brief Funcao que calibra o sensor
 * 
 * Essa funcao precisa ser chamada para calibrar o sensor com a temperatura ambiente
 * 
 * @param tempCalibra e o valor para comparacao com a temperatura de 25 graus
 **/
void calibrarAmostragem(int16_t tempCalibra) {

	int16_t bg, vTemp, temp25;

	uint32_t Vdd;

	//Canal do bandgap, tensao de referencia
	ADCSC1 = 27;

	while (!ADCSC1_COCO)
		; //Espera conversao acabar

	//Valor do bandcap
	bg = ADCR;

	//Desliga o conversor A/D
	ADCSC1 = 0x1F;

	//Canal do sensor de temperatura
	ADCSC1 = 26;

	while (!ADCSC1_COCO)
		; //Espera conversao acabar

	//Valor da temperatura
	vTemp = ADCR;

	//Desliga o conversor A/D
	ADCSC1 = 0x1F;

	Vdd = 49140 / bg;

	//Calcula temp25 perde dados. mas não é um problema
	temp25 = 28714 / Vdd;

	if (tempCalibra <= 25) {

		mCalibraF = 1000 * (temp25 - vTemp) / (tempCalibra - 25);

	} else if (tempCalibra > 25) {

		mCalibraQ = 1000 * (vTemp - temp25) / (tempCalibra - 25);

	}

}

/**
 * @fn void paraAmostragem(void)
 * @brief Interrompe a realizacao da amostragem
 * 
 * Para a realizacao da amostragem, mudando seu status para
 * FALSE e desligando o timer de amostragem.
 * 
 **/
void paraAmostragem() {

	escreveSerial("\nParando Amostragem...");

	amostrando = FALSE;

	//Desliga timer da amostragem
	desligaTimer(1);

	escreveSerial("\nAmostragem Parada!");

}

/**
 * @fn void iniciaAmostragem(void)
 * @brief Inicia amostragem
 * 
 * Inicia a amostragem obtendo a data e hora em que e
 * iniciada e depois de um periodo de tempo, 
 * comeca a realizar a amostragem. 
 * 
 **/
void iniciaAmostragem() {

	HEADER cabecalho;

	escreveSerial("\nIniciando Amostragem...");

	amostrando = TRUE;

	//Obtem data/hora e salva na struct
	(void) relogio_GetTime(&(cabecalho.time));
	(void) relogio_GetDate(&(cabecalho.date));

	//Popula struct que sera salva na memoria
	cabecalho.tipo = (bool) AMOSTRA;
	cabecalho.dado = (int16_t) TAXA_AMOSTRAGEM;

	//Salva header na memoria
	salvaHeader(&cabecalho);

	//Conta ate TAXA_AMOSTRAGEM com o timer 1 e executa a funcao getAmostra
	conta(1, TAXA_AMOSTRAGEM, getAmostra);

	escreveSerial("\nAmostragem Iniciada!");
}

/**
 * @fn unsigned int amostra(void)
 * @brief Realiza a amostragem
 * 
 * Configura o conversor A/D para encontrar uma tensao de referencia
 * e uma temperatura amostrada a fim de calcular o valor de temperatura 
 * real. Esse calculo varia se a temperatura amostrada for maior ou 
 * menor do que 25 graus celsius.
 * 
 * @return Valor real de temperatura obtido
 **/
uint16_t amostra() {

	int16_t valorAmostrado, temp25, mQuente, mFrio, bg, temp;

	uint32_t Vdd;

	//Canal do bandgap, tensao de referencia
	ADCSC1 = 27;

	while (!ADCSC1_COCO)
		; //Espera conversao acabar

	//Valor do bandcap
	bg = ADCR;

	//Desliga o conversor
	ADCSC1 = 0x1F;

	//Canal do sensor de temperatura
	ADCSC1 = 26;

	while (!ADCSC1_COCO)
		; //Espera conversao acabar

	//Valor da temperatura
	valorAmostrado = ADCR;

	//Desliga o conversor
	ADCSC1 = 0x1F;

	//Calculo padrao
	Vdd = 49140 / bg;

	temp25 = 28714 / Vdd;

	if (mCalibraF == 0) {

		mFrio = 6740 / Vdd;

	} else {

		mFrio = mCalibraF;

	}

	if (mCalibraQ == 0) {

		mQuente = 7244 / Vdd;

	} else {

		mQuente = mCalibraQ;
	}

	if (valorAmostrado >= temp25) {

		temp = 25 - (((valorAmostrado - temp25) * 100) / mFrio);

	} else {

		temp = 25 - (((valorAmostrado - temp25) * 100) / mQuente);

	}

	return temp;
}

/**
 * @fn void *getAmostra(void)
 * @brief Interrupcao que obtem a amostra
 * 
 * Interrupcao que chama a funcao de realizacao de amostragem
 * e em seguida a funcao de salvar a amostra na memoria.
 * Para esse trecho desabilitamos as interrupcoes.
 * 
 */
void *getAmostra(void) {

	int8_t dadoAmostra;

	DisableInterrupts

	//Realiza a amostragem
	dadoAmostra = amostra();

	//Salva amostra na memoria
	salvaAmostra(dadoAmostra);

	EnableInterrupts

}
