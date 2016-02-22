/**
 * \file eeprom.c
 * \brief Contem as manipulacoes necessarias para usar a memoria
 * 
 * Possui as funcoes que controlam a memoria eeprom, fazendo sua
 * configuracao, escrita e leitura, alem de outras funcoes 
 * implementadas necessarias para seu correto funcionamento.
 * 
 * \sa eeprom.h
 **/
#include "eeprom.h"

// Union do endereco
unionAddress uAddr;

// Union para dados de 16 bits
unionAddress uDados;

uint16_t address = ENDERECO;

bool travado = TRUE;

// Flag que indica quando a memoria ficou cheia
bool memCheia = FALSE;

/**
 * @fn void configuraMem(void)
 * @brief Configura a memoria eeprom 
 * 
 * Faz as modificacoes no IIC para
 * fazer a configuracao necessaria da memoria
 * eeprom
 **/
void configuraMem() {

	int8_t dadoTemporario;

	HEADER dadoHeader;

	//Endereco base do inicio de gravacao dos dados na eeprom
	uint16_t end = (uint16_t) ENDERECO;

	//Endereco base do inicio de gravacao dos dados na eeprom
	uint16_t endHeader = (uint16_t) 0;

	//Habilita I2C
	IIC2C_IICEN = 1;

	//Modo slave
	IIC2C_MST = 0;

	//Ajuste da velocidade 99, Set speed to 50kHz for Bus = 18.8743MHz; 12.5k->0x39; 50k->0x99; 100k->0x59;
	IIC2F = 0x99;

	//Sem ack
	IIC2C_TXAK = 1;

	//R/W bit = 0;
	IIC2S_SRW = 0;

	//Inicia leitura das amostras
	while (1) {

		//Alimenta o DOG
		WDog_Clear();

		//Se o valor for igual ao inicio da formatacao
		if (leDado(end, &dadoTemporario)) {

			if (dadoTemporario == INIT_HEADER) {

				endHeader = end;

			} else if (dadoTemporario == INIT_FORMAT) {

				//Atualiza valor do address para inicio do bloco de dados
				address = end;
				break;

			}

		}

		//Verifica se no espaco disponivel cabe o evento de memoria cheia
		if (end >= (enderecoMax - sizeof(HEADER))) {

			memCheia = TRUE;
			break;

		}

		//Calcula endereco do proximo dado
		end++;
	}

	//Le ultimo header e atualiza sistema a partir do ultimo header
	if (endHeader > 0 && leHeader(endHeader, &dadoHeader)) {

		TAXA_AMOSTRAGEM = dadoHeader.dado;

		relogio_SetDate(dadoHeader.date.Year, dadoHeader.date.Month,
				dadoHeader.date.Day);

		relogio_SetTime(dadoHeader.time.Hour, dadoHeader.time.Min,
				dadoHeader.time.Sec, dadoHeader.time.Sec100);

	} else {

		//Se nada for encontrado, formata memoria
		limparMem(FALSE);

	}

}

/**
 * @fn void delay(void)
 * @brief Funcao de espera
 * 
 * Manipula os registradores TPM para fazer
 * uma espera de tempo.
 */
void delay() {
	
	int cont = 0;

//Para o timer
	TPM3SC = 0x00;

	TPM3CNT = 0;

//Clock = 4.194MHz
	TPM3MOD = 4;

//Desabilita interrupcao, fonte = clockbus, escala = 1
	TPM3SC = 0b00001000;

//Espera o tempo
	while (cont != MEM_DELAY) {

		while (TPM3SC_TOF == 0)
			;

		//Alimenta o DOG
		WDog_Clear();

		cont++;

		TPM3CNT = 0;
		TPM3SC_TOF = 0;
	}

	TPM3SC = 0;

}

/**
 * @fn void salvaAmostra(int dadoAmostra)
 * @brief Salva o dado amostrado na memoria eeprom
 * 
 * Verifica se ha espaco na memoria, se conseguir permissao 
 * utiliza a funcao escreveMem() para salvar a amostra na memoria.
 * 
 * @param Valor do dado amostrado
 */
void salvaAmostra(int8_t dadoAmostra) {

	//Verifica se tem espaco para o dado e para o evento de memoria cheia
	if (address < (enderecoMax - sizeof(HEADER) - sizeof(int8_t))) {

		escreveMem(address++, dadoAmostra);

	} else {

		memCheia = TRUE;

	}

}

/**
 * void salvaHeader(HEADER *c)
 * @brief Salva struct na memoria
 * 
 * Salva a struct na memoria contendo o cabecalho do dado, 
 * junto com seu tipo, data e hora usando a funcao escreveMem().
 * 
 * @param Struct com valores a ser salvos
 */
void salvaHeader(HEADER *c) {

	//Verifica se a memoria tem espaco para esta struct e para o de memoria cheia
	if (address < (enderecoMax - 2 * sizeof(HEADER))
			|| (c->tipo == EVENTO && c->dado == MEMORIA_CHEIA)) {

		//Padrao do inicio do bloco
		c->header = (uint8_t) INIT_HEADER;

		escreveMem(address++, (byte) c->header);
		escreveMem(address++, (byte) c->tipo);

		uDados.EE = c->dado;
		escreveMem(address++, (byte) uDados.Bytes.H);
		escreveMem(address++, (byte) uDados.Bytes.L);

		uDados.EE = c->date.Year;
		escreveMem(address++, (byte) uDados.Bytes.H);
		escreveMem(address++, (byte) uDados.Bytes.L);

		escreveMem(address++, (byte) c->date.Month);
		escreveMem(address++, (byte) c->date.Day);
		escreveMem(address++, (byte) c->time.Hour);
		escreveMem(address++, (byte) c->time.Min);
		escreveMem(address++, (byte) c->time.Sec);
		escreveMem(address++, (byte) c->time.Sec100);

	} else {

		memCheia = TRUE;

	}
}

/**
 * @fn void limparMem(bool mensagemSerial)
 * @brief Limpa os dados da memoria eeprom
 * 
 * Limpa os dados da memoria eeprom, substituindo
 * os dados pelo valor padrao 127.
 * 
 * @param TRUE/FALSE para mostrar a limpeza 
 **/
void limparMem(bool mensagemSerial) {

	uint16_t end;

// address=dataAddress : primeiro endereco para dados
	for (end = ENDERECO; end <= enderecoMax; end++) {

//Escreve valor padrao para formatacao em cada endereco da memoria 
		escreveMem(end, INIT_FORMAT);

//Mostra andamento na serial
		if (mensagemSerial == TRUE) {

			(void) sprintf(buffer, "\nApagando Bloco: %d", end);

			escreveSerial(buffer);
		}

	}

//Reseta flag
	memCheia = FALSE;

//Inicia endereco na primeira posicao
	address = ENDERECO;
}

/**
 * @fn byte auxMem(bool read, byte dado)
 * @brief funcao auxiliar usada para ler ou escrever na memoria
 * 
 * Funcao que auxilia as funcoes de leitura e escrita na memoria,
 * sendo chamada quando conveniente. 
 * 
 * @param read indica se necessita-se de leitura ou escrita
 * @param dado e o valor do dado ou endereco que ira ser lido ou escrito
 **/
byte auxMem(bool read, byte dado) {
//read = TRUE, write = FALSE

	delay();

	while (!IIC2S_IICIF)
		; //Espera IBIF

	IIC2S_IICIF = 1;

	while (IIC2S_RXAK)
		; //Espera RXAK

	if (read == TRUE) {

		uint8_t lixo;

//Seta o recebimento
		IIC2C_TX = 0;

//Retira ACK
		IIC2C_TXAK = 1;

//(Retirado do manual da freescale)
		lixo = IIC2D;

		while (!IIC2S_IICIF)
			; //Espera IBIF  

		IIC2S_IICIF = 1;

//Stop bit
		IIC2C_MST = 0;

//Realiza a leitura do dado, duas vezes
		return IIC2D;

	} else {

//Envia dado ou endereço
		IIC2D = dado;

		return 0;
	}
}

/**
 * @fn void escreveMem(unsigned int endereco, byte dado)
 * @brief Escreve um dado desejado na memoria
 * 
 * Usa as manipulacoes da memoria eeprom contido no
 * manual IIC para escrever um certo dado em um
 * endereco na memoria
 * 
 * @param endereco e o valor onde o dado sera gravado
 * @param dado e o valor do dado a ser gravado
 **/
void escreveMem(uint16_t endereco, byte dado) {

	uint8_t temp = 0;

	delay();

//Recebe o endereço
	uAddr.EE = endereco;

//Deixa só os 3 bits menos significativos da parte alta, ou seja, so ate 2047 
//Bit menos significativo indica write ou read
	temp = (uAddr.Bytes.H & 0x07) << 1;

// RX/TX = 1; MS/SL = 1; TXAK = 0;
	IIC2C_TXAK = 0;

//Start bit
	IIC2C |= 0x30;

	IIC2C_TX = 1;

//Transmite 3 bits de endereco e os bits endereca escravo, com modo write
	IIC2D = escravoIIC | temp;

//Escreve a parte baixa do endereco
	(void) auxMem(FALSE, uAddr.Bytes.L);

//Escreve o dado
	(void) auxMem(FALSE, dado);

	while (!IIC2S_IICIF)
		; // espera IBIF

	IIC2S_IICIF = 1;

	while (IIC2S_RXAK)
		; // aguarda RXAK;

//Limpa a flag
	IIC2S_IICIF = 1;

//Stop bit
	IIC2C_MST = 0;
}

/**
 * @fn byte leMem(unsigned int endereco)
 * @brief Le um byte da memoria
 *
 * Recebe um endereco de 2 bytes, quebra-o e encontra o valor 
 * do dado contido neste endereco na memoria.
 * 
 * @param endereco para leitura
 * @return valor do dado contido no endereco de parametro
 **/
byte leMem(uint16_t endereco) {
	byte temp = 0, dadoLido = 0;

	delay();

//Recebe o endereco
	uAddr.EE = endereco;

	temp = (uAddr.Bytes.H & 0x07) << 1;

// RX/TX = 1; MS/SL = 1; TXAK = 0;
	IIC2C_TXAK = 0;

// Start bit
	IIC2C |= 0x30;

//Transmite 3 bits de endereco e os bits endereca escravo, com modo write
	IIC2D = escravoIIC | temp;

//Transmite parte baixa do endereco
	(void) auxMem(FALSE, uAddr.Bytes.L);

	while (!IIC2S_IICIF)
		; //Espera IBIF

	IIC2S_IICIF = 1;

	while (IIC2S_RXAK)
		; //Aguarda RXAK

//Reseta para colocar no modo read
	IIC2C_RSTA = 1;

//Transmite 3 bits de endereco e os bits endereca escravo, com modo read
	IIC2D = escravoIIC | temp | 1;

//Dado recebe o valor lido
	dadoLido = auxMem(TRUE, 0);

	return dadoLido;
}

/**
 * @fn bool leHeader(unsigned int endereco, HEADER* dadoTemporario)
 * @brief Le o header do inicio do bloco de dados
 * @param endereco para leitura
 * @param dadoTemporario e o campo da struct
 * @return TRUE se houve exito ao ler
 **/
bool leHeader(uint16_t endereco, HEADER* dadoTemporario) {

//Verifica se o endereco esta dentro do intervalo correto
	if (endereco >= ENDERECO && endereco <= enderecoMax) {

		dadoTemporario->header = leMem(endereco);
		dadoTemporario->tipo = leMem(++endereco);

		dadoTemporario->dado = leMem(++endereco);
		dadoTemporario->dado = dadoTemporario->dado << 8; //shifta		
		dadoTemporario->dado |= leMem(++endereco);

		dadoTemporario->date.Year = leMem(++endereco);
		dadoTemporario->date.Year = dadoTemporario->date.Year << 8; //shifta		
		dadoTemporario->date.Year |= leMem(++endereco);

		dadoTemporario->date.Month = leMem(++endereco);
		dadoTemporario->date.Day = leMem(++endereco);
		dadoTemporario->time.Hour = leMem(++endereco);
		dadoTemporario->time.Min = leMem(++endereco);
		dadoTemporario->time.Sec = leMem(++endereco);
		dadoTemporario->time.Sec100 = leMem(++endereco);

		return TRUE;

	} else {

		return FALSE;

	}
}

/**
 * @fn bool leDado(unsigned int endereco, short int *dadoTemporario)
 * @brief Le dado do endereco informado
 * 
 * Tentar ler um dado de um certo endereco, se conseguiu,
 * popula struct com a funcao leMem().
 * 
 * @param endereco para leitura
 * @param dadoTemporario e o valor a ser salvo na variavel
 * @return TRUE se houve exito na leitura
 * 
 **/
bool leDado(uint16_t endereco, int8_t *dadoTemporario) {

//Verifica se o endereco esta dentro do intervalo correto
	if (endereco >= ENDERECO && endereco <= enderecoMax) {

//Salva valor na variavel
		*(dadoTemporario) = (int8_t) leMem(endereco);

		return TRUE;

	} else {

		return FALSE;

	}
}

