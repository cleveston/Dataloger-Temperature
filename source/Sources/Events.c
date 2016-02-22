/* ###################################################################
 **     Filename    : Events.c
 **     Project     : ProcessorExpert
 **     Processor   : MC9S08QE128CLK
 **     Component   : Events
 **     Version     : Driver 01.02
 **     Compiler    : CodeWarrior HCS08 C Compiler
 **     Date/Time   : 2015-05-31, 11:25, # CodeGen: 0
 **     Abstract    :
 **         This is user's event module.
 **         Put your event handler code here.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file Events.c
 ** @version 01.02
 ** @brief
 **         This is user's event module.
 **         Put your event handler code here.
 */
/*!
 **  @addtogroup Events_module Events module documentation
 **  @{
 */
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

//char_t buffer_2[256];

//uint16_t index = 0;

/*
 ** ===================================================================
 **     Event       :  ASerial_OnError (module Events)
 **
 **     Component   :  ASerial [AsynchroSerial]
 **     Description :
 **         This event is called when a channel error (not the error
 **         returned by a given method) occurs. The errors can be read
 **         using <GetError> method.
 **         The event is available only when the <Interrupt
 **         service/event> property is enabled.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void ASerial_OnError(void) {
	/* Write your code here ... */
}

/*
 ** ===================================================================
 **     Event       :  ASerial_OnRxChar (module Events)
 **
 **     Component   :  ASerial [AsynchroSerial]
 **     Description :
 **         This event is called after a correct character is received.
 **         The event is available only when the <Interrupt
 **         service/event> property is enabled and either the <Receiver>
 **         property is enabled or the <SCI output mode> property (if
 **         supported) is set to Single-wire mode.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void ASerial_OnRxChar(void) {

	//Se caracter for enter, termina aplicação
//	if (BufferRead == '\n') {
//
//		//Adiciona caracter terminador de string
//		buffer_2[index] = '\0';
//
//	} else {
//
//		//Adiciona caracter no buffer
//		buffer_2[index] = BufferRead;
//
//		index++;
//
//	}
}

/*
 ** ===================================================================
 **     Event       :  ASerial_OnTxChar (module Events)
 **
 **     Component   :  ASerial [AsynchroSerial]
 **     Description :
 **         This event is called after a character is transmitted.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void ASerial_OnTxChar(void) {
	/* Write your code here ... */
}

/* END Events */

/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.3 [05.08]
 **     for the Freescale HCS08 series of microcontrollers.
 **
 ** ###################################################################
 */
