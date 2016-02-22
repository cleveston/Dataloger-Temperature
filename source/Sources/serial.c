/**
 * \file serial.c
 * \brief Contem as manipulacoes necessarias para usar corretamente a porta serial
 *
 * Aqui esta contido como e feita a leitura e escrita no buffer de dados atraves da porta serial.
 * Tambem possui as funcoes que verificam a autenticidade do usuario para obter acesso ao programa
 * e a de verificacao do buffer no qual o programa interpreta os comandos enviados pelo usuario.
 *
 * \sa serial.h
 **/

#include "serial.h"

/// Buffer que guardara os caracteres.
char_t buffer[TAMANHO_BUFFER];

/// Senha para configuracao.
char_t SENHA[TAMANHO_BUFFER] = "barrichelo";

/// Flag que indica que o usuario esta no modo de configuracao.
bool autenticado = FALSE;

/// Variavel que armazena tempo de espera da serial.
uint8_t segundoSerial = 0;

/// Variavel que armazena tempo para entrar no modo de configuracao. 
uint8_t segundoConf = 0;

/**
 * @fn static void autentica(void)
 * @brief Funcao que autentica o usuario
 * 
 * Verifica se a senha que o usuario inseriu no buffer condiz
 * com a senha do sistema. Estando correta, permite acesso a 
 * central de configuracoes.
 *
 **/
static void autentica() {

	escreveSerial("\nInforme sua senha: ");

	//Espera o usuario digitar a senha
	if (leSerial() == TRUE) {

		//Compara se a senha informada e correta
		if (strcmp(buffer, SENHA) == 0) {

			//Autentica o usuario
			autenticado = TRUE;

			escreveSerial(
					"\n\n-------- Bem Vindo a Central de Configurações --------");

			escreveSerial("\nInforme o Comando: ");

		} else {

			escreveSerial("\nSenha Errada!");

		}

	} else {

		escreveSerial("\nOperacao Cancelada!");

	}

}

/**
 * @fn void *incSerial(void)
 * @brief Marca o tempo de espera para a serial
 **/
void *incSerial() {
	segundoSerial++;

	return 0;
}

/**
 * @fn void *incConf(void)
 * @brief Marca o tempo que o usuario possui para entrar no modo de configuracao
 **/
void *incConf() {
	segundoConf++;

	return 0;

}

/**
 * @fn void verificaSerial(void)
 * @brief Funcao que verifica se o buffer recebeu algum caracter.
 * 
 * Faz a verificacao do buffer a procura de algum caracter
 * esperado, por exemplo, o '+' quando repetido tres vezes 
 * dentro de uma faixa de tempo da acesso ao usuario poder
 * fazer sua autenticacao. 
 **/
void verificaSerial() {

	//Buffer de chegada da interface serial
	uint8_t bufferSerial = 0;

	//Verifica se chegou algum caracter
	bufferSerial = (uint8_t) ASerial_GetCharsInRxBuf();

	if (bufferSerial == 1) {

		//Le serial e armazena no buffer
		(void) leSerial();

		//Se for o caracter esperado para configuracao e o usuario estiver deslogado
		if (strcmp(buffer, "+") == 0 && autenticado == FALSE) {

			//Variavel que armazena o numero de vezes que o caracter foi digitado
			uint8_t vezesCaracter = 0;

			//Zera contadores
			segundoConf = 0;

			//Incrementa contador
			vezesCaracter++;

			//Inicia contador do tempo para entrar no modo de configuracao
			(void) conta(3, 1000, incConf);

			while (1) {

				WDog_Clear();

				bufferSerial = (uint8_t) ASerial_GetCharsInRxBuf();

				if (bufferSerial == 1) {

					//Le serial novamente
					(void) leSerial();

					//Se for o caracter esperado para configuracao
					if (strcmp(buffer, "+") == 0) {

						vezesCaracter++;

					}

					//Se passou 3s sem 3x o caractere '+', termina funcao. Ou se o usuario digitou 3x o caracter '+'
					if (segundoConf > 3 && vezesCaracter < 3) {

						//Desligar timer do tempo para entrar com o caracter
						(void) desligaTimer(2);

						escreveSerial("\nTempo Esgotado!");

						break;
					} else if (vezesCaracter == 3) {

						//Desligar timer do tempo para entrar com o caracter
						(void) desligaTimer(2);

						//Chama funcao para autenticar
						autentica();

						break;
					}

				}
			}

		} else if (strcmp(buffer, "+++") == 0 && autenticado == FALSE) {

			//Chama funcao para autenticar
			autentica();

		} else if (autenticado == FALSE) {

			escreveSerial("\nVoce precisa estar no Modo de Configuracao!");

		}

	}

}

/**
 * @fn bool leSerial(void)
 * @brief Funcao que le string da serial e retorna se houve exito
 *
 * Essa funcao le os caracteres mandados pelo usuario na porta serial,
 * a leitura acaba quando o caracter de enter for lido e a funcao retorna TRUE.
 * Retorna FALSE caso um ctrl-c seja detectado ou se houve estouro do timer 
 * por falta de comandos do usuario. 
 *
 * @return TRUE se durante o tempo do timer receber o caracter de ENTER, senao FALSE
 **/
bool leSerial() {

	//Indice do vetor do buffer
	uint16_t index = 0;

	//Buffer de chegada da interface serial
	uint8_t bufferSerial = 0;

	//Caracter que chegou no buffer
	char_t caracter;

	//Limpa buffer para receber nova palavra
	(void) strcpy(buffer, "");

	//Zera contador do tempo de espera
	segundoSerial = 0;

	//Conta 1s e chama funcao incSerial
	(void) conta(2, 1000, incSerial);

	//Repete ate o ultimo caracter for enter ou estourar o tempo
	while (1) {

		//Alimenta o Cao de Guarda
		WDog_Clear();

//Verifica se chegou algum caracter na serial
		bufferSerial = (uint8_t) ASerial_GetCharsInRxBuf();

		if (bufferSerial == 1) {

			//Espera ate receber caracter
			while (ASerial_RecvChar(&caracter)) {

				//Alimenta o DOG
				WDog_Clear();

			}

			//Se caracter for enter, termina aplicacao
			if (caracter == '\n') {

				//Adiciona caracter terminador de string
				buffer[index] = '\0';

				//Desliga timer que conta o tempo
				(void) desligaTimer(2);

				return TRUE;

			} else if (caracter == 3) {	//Ctrl-C

				//Limpa buffer
				(void) strcpy(buffer, "");

				return FALSE;

			} else {

				//Adiciona caracter no buffer e converte para minusculo
				buffer[index] = tolower(caracter);

				index++;

			}

		} else if (segundoSerial > TEMPO_ESPERA) { //Se passou o tempo sem ninguem entrar com nada, termina aplicacao

			//Desliga timer que conta o tempo
			(void) desligaTimer(2);

			escreveSerial("\nTempo esgotado!");

			return FALSE;

		}

	}

}

/**
 * @fn void escreveSerial ( char * mensagem )
 * @brief Funcao que escreve mensagem da serial.
 *
 * Escreve os caracteres da mensagem na serial ate identificar 
 * um caracter de fim de string, entao encerra a escrita e faz
 * a limpeza do buffer.
 * 
 * @param caracteres da mensagem a ser escrita
 **/
void escreveSerial(char *mensagem) {

	//Indice do vetor de caracteres
	uint16_t index = 0;

	//Repete ate que o ultimo caracter for o terminador de string '\0'
	while (mensagem[index] != '\0') {

		//Escreve cada caracter na serial
		while (ASerial_SendChar(mensagem[index])) {

			//Alimenta o DOG
			WDog_Clear();

		}

		index++;

	}

	//Limpa buffer
	(void) strcpy(buffer, "");

}

