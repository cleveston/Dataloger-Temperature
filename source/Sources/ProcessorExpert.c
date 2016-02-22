//-------------------------------------------------------------------------//
//                  Universidade Federal de Santa Maria                    //
//                   Curso de Engenharia de Computacao                     //
//                     Projeto de Sistemas Embarcados                      //
//								 	   //
//   Autor: Iury Cleveston, Lucas Cielo, Miguel Pfitscher                  //
//   		                                                           //
//   Data: 29/06/2015                                                      //
//=========================================================================//
//                         Descricao do Programa                           //
//=========================================================================//
//   Datalogger de Temperatura com Comunicacao Serial                      //
//                                                                         //
//-------------------------------------------------------------------------//

#include "Cpu.h"
#include "Events.h"
#include "Vtpm1ovf.h"
#include "Vtpm2ovf.h"
#include "Vtpm3ovf.h"
#include "relogio.h"
#include "ASerial.h"
#include "Vlvd.h"
#include "Virq.h"
#include "WDog.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "stdlib.h"
#include "stdio.h"
#include "timer.h"
#include "eeprom.h"
#include "amostragem.h"
#include "botaoIRQ.h"
#include "serial.h"

/**
 * @enum Estados da FSM
 **/
enum {
	eMEDINDO = 0, ePARADO
} estado;

//Prototipos
void calibrar(void);
void sair(void);
void setDate(void);
void setTime(void);
void setSenha(void);
void setAmostragem(void);
void mostrar(void);
void status(void);
void ajuda(void);

/**
 * @fn void configuraLVD(void)
 * @brief Configura a funcao de tensao baixa
 **/
void configuraLVD(void) {

	SPMSC1 = 0x24;
	/* SPMSC2: LVWF=0,LVWACK=0,LVDV=1,LVWV=1,PPDF=0,PPDACK=0,PDC=0,PPDC=0 */

	SPMSC2 = 0x30;

}

/**
 * @fn ISR(tensaoBaixa)
 * @brief Interrupcao gerada ao detectar queda de tensao
 **/ISR(tensaoBaixa) {

	HEADER cabecalho;

	SPMSC2 |= 0x40;

	//Para Amostragem
	paraAmostragem();

	//Obtem data/hora e salva na struct
	(void) relogio_GetTime(&(cabecalho.time));
	(void) relogio_GetDate(&(cabecalho.date));

	//Popula struct que sera salva na memoria
	cabecalho.tipo = (bool) EVENTO;
	cabecalho.dado = (int16_t) FALTA_ENERGIA;

	//Salva header na memoria
	salvaHeader(&cabecalho);

}

/**
 * @fn void sair(void)
 * @brief Funcao que desloga o usuario 
 **/
void sair(void) {

	//Finaliza sessao do usuario
	autenticado = FALSE;

	escreveSerial("\nSessao Finalizada!");

}

/**
 * @fn void calibrar(void)
 * @brief Funcao para calibrar o sensor
 **/
void calibrar(void) {

	escreveSerial("\nInforme a temperatura: ");

	//Le nova data na serial
	if (leSerial() == TRUE) {

		//Converte para inteiro
		int16_t valor = atoi(buffer);

		//Testa se temperatura informada esta dentro do intervalo
		if (valor > -127 && valor < 126) {

			//Calibra o sensor
			calibrarAmostragem(valor);

			escreveSerial("\nSensor Calibrado com Sucesso!");

		} else {

			escreveSerial("\nTemperatura fora do intervalo!");

		}

	} else {

		escreveSerial("\nOperacao Cancelada!");

	}

}

/**
 * @fn void ajuda(void)
 * @brief Funcao que mostra menu de ajuda
 **/
void ajuda(void) {

	escreveSerial("\n-------------AJUDA--------------");
	escreveSerial("\n1) AJUDA");
	escreveSerial("\n2) AMOSTRAGEM");
	escreveSerial("\n3) APAGAR");
	escreveSerial("\n4) CALIBRAR");
	escreveSerial("\n5) DATA");
	escreveSerial("\n6) IR");
	escreveSerial("\n7) HORA");
	escreveSerial("\n8) MOSTRAR");
	escreveSerial("\n9) PARAR");
	escreveSerial("\n10) SENHA");
	escreveSerial("\n11) STATUS");
	escreveSerial("\n12) SAIR");
	escreveSerial("\n---------------------------------");

}

/**
 * @fn void status(void)
 * @brief Funcao que mostra o status do sistema 
 **/
void status(void) {

	DATEREC date;
	TIMEREC time;

	escreveSerial("\n-------------STATUS--------------");

	//Estatisticas da amostragem
	if (amostrando == TRUE) {

		(void) sprintf(buffer, "\nAmostragem: EXECUTANDO! (%d ms)",
				TAXA_AMOSTRAGEM);

		escreveSerial(buffer);

	} else {

		escreveSerial("\nAmostragem: PARADA!");
	}

	//Obtem data/hora e salva na struct
	(void) relogio_GetTime(&time);
	(void) relogio_GetDate(&date);

	(void) sprintf(buffer, "\nHorario %d/%d/%d %d:%d:%d:%d", date.Day,
			date.Month, date.Year, time.Hour, time.Min, time.Sec, time.Sec100);

	escreveSerial(buffer);

	//Estatisticas da memoria
	if (memCheia == TRUE) {

		escreveSerial("\nMemoria: CHEIA!");

	} else {

		//Espaco total para salvamento
		uint32_t espacoTotal = enderecoMax - ENDERECO;

		//Calculo da porcentagem de uso da memoria
		uint32_t porcentagemUso = (100 * (espacoTotal - address)) / espacoTotal;

		(void) sprintf(buffer, "\nMemoria Disponivel: %d por cento",
				(uint16_t) porcentagemUso);

		escreveSerial(buffer);
	}

	escreveSerial("\n----------------------------------");

}

/**
 * @fn void setDate(void)
 * @brief Funcao que configura a data
 **/
void setDate(void) {

	uint16_t ano = 0;
	uint16_t mes = 0;
	uint16_t dia = 0;

	escreveSerial("\nInforme nova data (yyyy/mm/dd): ");

	//Le nova data na serial
	if (leSerial() == TRUE && strlen(buffer) == 10) {

		//Quebra data no caracter '/' e converte para inteiro
		(void) sscanf(buffer, "%d/%d/%d", &ano, &mes, &dia);

		//Verifica se o formato esta correto e se esta do tamanho esperado
		if (ano != 0 && mes != 0 && dia != 0) {

			//Seta data no relogio
			(void) relogio_SetDate((word) ano, (byte) mes, (byte) dia);

			//Define mensagem do buffer
			(void) sprintf(buffer, "\nData definida com sucesso: %d/%d/%d", ano,
					mes, dia);

			escreveSerial(buffer);

		} else {

			escreveSerial("\nFormato Invalido!");
		}

	} else {

		escreveSerial("\nOperacao Cancelada!");

	}

}

/**
 * @fn void setTime(void)
 * @brief Funcao que configura o horario
 **/
void setTime(void) {

	uint16_t hora = 0;
	uint16_t minuto = 0;
	uint16_t segundo = 0;
	uint16_t mSegundo = 0;

	escreveSerial("\nInforme novo horario (hh:mm:ss:ms): ");

	//Le novo horario na serial
	if (leSerial() == TRUE) {

		//Verifica se esta do tamanho esperado
		if (strlen(buffer) == 11) {

			//Quebra horario no caracter ':' e converte para inteiro
			(void) sscanf(buffer, "%d:%d:%d:%d", &hora, &minuto, &segundo,
					&mSegundo);

			//Seta horario no relogio
			(void) relogio_SetTime((byte) hora, (byte) minuto, (byte) segundo,
					(byte) mSegundo);

			//Define mensagem do buffer
			(void) sprintf(buffer,
					"\nHorario definido com sucesso: %d:%d:%d:%d", hora, minuto,
					segundo, mSegundo);

			escreveSerial(buffer);

		} else {

			escreveSerial("\nFormato Invalido!");

		}
	} else {

		escreveSerial("\nOperacao Cancelada!");

	}
}

/**
 * @fn void setSenha(void)
 * @brief Funcao que altera a senha
 **/
void setSenha(void) {

	escreveSerial("\nInforme sua nova Senha: ");

//Le nova senha na serial
	if (leSerial() == TRUE) {

		//Verifica se a senha esta do tamanho adequado
		if (strlen(buffer) >= 5) {

			//Copia nova senha do buffer
			(void) strcpy(SENHA, buffer);

			escreveSerial("\nSenha definida com sucesso!");

		} else {

			escreveSerial("\nSenha muito pequena. Minimo 5 caracteres!");

		}

	} else {

		escreveSerial("\nOperacao Cancelada!");

	}

}

/**
 * @fn void setAmostragem(void)
 * @brief Funcao que altera a frequencia da taxa de amostragem
 **/
void setAmostragem(void) {

	escreveSerial("\nInforme a nova Taxa de Amostragem (10ms - 10000ms): ");

	//Le nova amostragem na serial
	if (leSerial() == TRUE) {

		uint16_t valor = atoi(buffer);

		//Verifica se a taxa de amostragem esta dentro do intervalo
		if (valor >= 10 && valor <= 10000) {

			//Define nova taxa de amostragem
			TAXA_AMOSTRAGEM = valor;

			escreveSerial("\nTaxa de Amostragem definida com sucesso!");

		} else {

			escreveSerial("\nTaxa de Amostragem fora do intervalo!");

		}

	} else {

		escreveSerial("\nOperacao Cancelada!");

	}

}

/**
 * @fn void mostrar(void)
 * @brief Funcao que mostra os dados na tela
 **/
void mostrar(void) {

	//Struct que armazenara o cabecalho dos dados
	HEADER headerTemporario;

	int8_t dadoTemporario;

	//Endereco da struct
	uint16_t end = (uint16_t) ENDERECO; //Endereco base do inicio de gravacao dos dados na eeprom

	//Desabilita interrupcoes durante a mostra dos dados
	DisableInterrupts

	escreveSerial(
			"\n------------------------------DADOS--------------------------------");

	//Repete enquanto tiver dados na memoria
	while (leHeader(end, &headerTemporario)
			&& headerTemporario.header == INIT_HEADER) {

		if (headerTemporario.tipo == AMOSTRA) {

			uint16_t mSegundos = headerTemporario.time.Sec100;
			uint16_t segundo = headerTemporario.time.Sec;
			uint16_t minutos = headerTemporario.time.Min;
			uint16_t horas = headerTemporario.time.Hour;

			//Desloca endereco para inicio dos dados
			end = end + sizeof(HEADER);

			//Inicia leitura das amostras
			while (leDado(end, &dadoTemporario) && dadoTemporario != INIT_FORMAT
					&& dadoTemporario != INIT_HEADER) {

				//Soma-se tempo de amostragem em mSegundos
				mSegundos += headerTemporario.dado;

				//Faz formatacao do tempo
				if (mSegundos >= 1000) {

					segundo += mSegundos / 1000;
					mSegundos = mSegundos % 1000;

					if (segundo >= 60) {

						minutos += segundo / 60;
						segundo = segundo % 60;

						if (minutos >= 60) {

							horas += minutos / 60;
							minutos = minutos % 60;

						}
					}

				}

				(void) sprintf(buffer,
						"\nAmostra: %d graus - Amostragem: %d ms - Horario %d/%d/%d %d:%d:%d:%d",
						dadoTemporario, headerTemporario.dado,
						headerTemporario.date.Day, headerTemporario.date.Month,
						headerTemporario.date.Year, horas, minutos, segundo,
						mSegundos);

				escreveSerial(buffer);

				//Calcula endereco do proximo dado
				end = end + sizeof(int8_t);

			}

		} else if (headerTemporario.tipo == EVENTO) {

			(void) sprintf(buffer,
					"\nEvento: %s - Horario %d/%d/%d %d:%d:%d:%d\n",
					(headerTemporario.dado == FALTA_ENERGIA) ?
							"FALTA ENERGIA" : "MEMORIA CHEIA",
					headerTemporario.date.Day, headerTemporario.date.Month,
					headerTemporario.date.Year, headerTemporario.time.Hour,
					headerTemporario.time.Min, headerTemporario.time.Sec,
					headerTemporario.time.Sec100);

			escreveSerial(buffer);

			//Calcula endereco do proximo dado
			end = end + sizeof(int8_t);

		} else {

			break;
		}

	}

	escreveSerial(
			"\n-------------------------------------------------------------------------");

	EnableInterrupts

}

/**
 * @fn void main(void)
 * @brief Funcao principal do programa
 * 
 * Aqui esta contido a FSM que foi especificada para o
 * trabalho, ela possui dois estados, PARADO e MEDINDO.
 * Estas ficam em um laco infinito esperando que o usuario
 * envie mensagens que ativem as demais funcoes descritas
 * nessa documentacao.
 **/
void main(void) {

	PE_low_level_init();

	//Configura a memoria EEPROM
	configuraMem();

	//Configura amostragem
	configuraAmostragem();

	//Configura botao IRQ
	configuraIRQ();

	//Configura tensao baixa
	configuraLVD();

	//Estado inicial depende se a memoria esta ou nao cheia
	if (memCheia == TRUE) {

		estado = ePARADO;

	} else {

		estado = eMEDINDO;

	}

	escreveSerial("\n**********************************************");
	escreveSerial("\n    	    DATALOGGER DE TEMPERATURA	       ");
	escreveSerial("\n**********************************************");
	escreveSerial("\n Lucas Cielo, Iury Cleveston, Miguel Pfitcher ");
	escreveSerial("\n**********************************************");

	for (;;) {

		//Alimenta o DOG
		WDog_Clear();

		//Verifica se chegou algum caracter na serial
		verificaSerial();

		switch (estado) {

		case eMEDINDO:

			//Verifica se a memoria esta cheia
			if (memCheia == TRUE) {

				HEADER cabecalho;

				//Para amostragem
				paraAmostragem();

				//Obtem data/hora e salva na struct
				(void) relogio_GetTime(&(cabecalho.time));
				(void) relogio_GetDate(&(cabecalho.date));

				//Popula struct que sera salva na memoria
				cabecalho.tipo = (bool) EVENTO;
				cabecalho.dado = (int16_t) MEMORIA_CHEIA;

				//Salva header na memoria
				salvaHeader(&cabecalho);

				estado = ePARADO;

			} else if (botaoApertado == 1 && segundos >= 1 && apertado == FALSE) { //Se o botao foi clicado uma vez

				//Reset IRQ para novo clique
				resetIRQ();

				estado = ePARADO;

			} else if (botaoApertado == 2) { //Se o botao foi clicado duas vezes em menos de 1s

				//Reset IRQ para novo clique
				resetIRQ();

				//Faz upload dos dados
				mostrar();

			} else if (botaoApertado == 1 && segundos >= 3) { //Se o botao foi clicado e mantido pressionado por 3s

			//Reset IRQ para novo clique
				resetIRQ();

				//Para a amostragem
				paraAmostragem();

				escreveSerial("\nApagando Memoria...");

				//Limpa memoria
				limparMem(FALSE);

				escreveSerial("\nMemoria Apagada!");

			} else if (autenticado == TRUE && (strcmp(buffer, "status") == 0)) {

				//Mostra status do sistema
				status();

			} else if (autenticado == TRUE && (strcmp(buffer, "ajuda") == 0)) {

				//Mostra ajuda do sistema
				ajuda();

			} else if (autenticado == TRUE && (strcmp(buffer, "senha") == 0)) {

				//Altera senha de configuracao
				setSenha();

			} else if (autenticado == TRUE
					&& (strcmp(buffer, "amostragem") == 0)) {

				//Para amostragem
				paraAmostragem();

				//Altera taxa de amostragem
				setAmostragem();

			} else if (autenticado == TRUE && (strcmp(buffer, "parar") == 0)) {

				estado = ePARADO;

			} else if (autenticado == TRUE
					&& (strcmp(buffer, "mostrar") == 0)) {

				//Mostra dados na tela
				mostrar();

			} else if (autenticado == TRUE && (strcmp(buffer, "apagar") == 0)) {

				//Para amostragem
				paraAmostragem();

				escreveSerial("\nApagando Memoria...");

				//Limpa dados da memoria
				limparMem(FALSE);

				escreveSerial("\nMemoria Apagada!");

			} else if (autenticado == TRUE && (strcmp(buffer, "data") == 0)) {

				//Para amostragem
				paraAmostragem();

				//Configura nova data
				setDate();

			} else if (autenticado == TRUE && (strcmp(buffer, "hora") == 0)) {

				//Para amostragem
				paraAmostragem();

				//Configura novo horario
				setTime();

			} else if (autenticado == TRUE
					&& (strcmp(buffer, "calibrar") == 0)) {

				//Para amostragem
				paraAmostragem();

				//Funcao que calibra o sensor
				calibrar();

			} else if (autenticado == TRUE && (strcmp(buffer, "sair") == 0)) {

				//Funcao que desloga o usuario
				sair();

			} else if (amostrando == FALSE) {

				//Inicia amostragem
				iniciaAmostragem();

			}

			break;

		case ePARADO:

			if (botaoApertado == 1 && segundos >= 1 && apertado == FALSE) {

				//Reset IRQ para novo clique
				resetIRQ();

				//Verifica se a memoria esta cheia
				if (memCheia == FALSE) {

					estado = eMEDINDO;

				}

			} else if (botaoApertado == 2) {

				//Reset IRQ para novo clique
				resetIRQ();

				mostrar();

			} else if (botaoApertado == 1 && segundos >= 3) {

				escreveSerial("\nApagando Memoria...");

				//Limpa memoria
				limparMem(FALSE);

				escreveSerial("\nMemoria Apagada!");

				//Reset IRQ para novo clique
				resetIRQ();

				estado = eMEDINDO;

			} else if (autenticado == TRUE && (strcmp(buffer, "ajuda") == 0)) {

				//Mostra ajuda do sistema
				ajuda();

			} else if (autenticado == TRUE && (strcmp(buffer, "status") == 0)) {

				//Mostra status do sistema
				status();

			} else if (autenticado == TRUE && (strcmp(buffer, "senha") == 0)) {

				//Altera senha de configuracao
				setSenha();

			} else if (autenticado == TRUE
					&& (strcmp(buffer, "amostragem") == 0)) {

				//Altera taxa de amostragem
				setAmostragem();

			} else if (autenticado == TRUE && (strcmp(buffer, "ir") == 0)) {

				//Verifica se a memoria esta cheia
				if (memCheia == FALSE) {

					estado = eMEDINDO;

				} else {

					escreveSerial("\nMemoria Cheia!");
				}

			} else if (autenticado == TRUE
					&& (strcmp(buffer, "mostrar") == 0)) {

				//Mostrar dados na tela
				mostrar();

			} else if (autenticado == TRUE && (strcmp(buffer, "apagar") == 0)) {

				escreveSerial("\nApagando Memoria...");

				//Limpa memoria
				limparMem(FALSE);

				escreveSerial("\nMemoria Apagada!");

				estado = eMEDINDO;

			} else if (autenticado == TRUE && (strcmp(buffer, "data") == 0)) {

				//Configura nova data
				setDate();

			} else if (autenticado == TRUE && (strcmp(buffer, "hora") == 0)) {

				//Configura novo horario
				setTime();

			} else if (autenticado == TRUE
					&& (strcmp(buffer, "calibrar") == 0)) {

				//Funcao que calibra o sensor
				calibrar();

			} else if (autenticado == TRUE && (strcmp(buffer, "sair") == 0)) {

				//Funcao que desloga o usuario
				sair();

			} else if (amostrando == TRUE) {

				//Para amostragem
				paraAmostragem();

			}

			break;

		default:

			estado = eMEDINDO;

			break;
		}

	}

	/*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
	for (;;) {
	}
	/*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/
