/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : WDog.c
**     Project     : ProcessorExpert
**     Processor   : MC9S08QE128CLK
**     Component   : WatchDog
**     Version     : Component 02.141, Driver 01.21, CPU db: 3.00.067
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2015-06-29, 17:00, # CodeGen: 29
**     Abstract    :
**         This device "WatchDog" implements a watchdog.
**         When enabled, the watchdog timer counts until it reaches
**         a critical value corresponding to the period specified
**         in 'Properties'. Then, the watchdog causes a CPU reset.
**         Applications may clear the timer before it reaches the critical
**         value. The timer then restarts the watchdog counter.
**         Watchdog is used for increasing the safety of the system
**         (unforeseeable situations can cause system crash or an
**         infinite loop - watchdog can restart the system or handle
**         the situation).
**
**         Note: Watchdog can be enabled or disabled in the initialization
**               code only. If the watchdog is once enabled user have
**               to call Clear method in defined time intervals.
**     Settings    :
**         Watchdog causes             : Reset CPU
**
**         Clock Source                : Internal clock 1-kHz
**
**         Initial Watchdog state      : Enabled
**
**         Mode of operation           : Normal
**
**         High speed mode
**           Watchdog period/frequency
**             microseconds            : 256000
**             milliseconds            : 256
**             Hz                      : 4
**
**         Run register                : SOPT1     [$1802]
**         Mode register               : SRS       [$1800]
**         Prescaler register          : SOPT1     [$1802]
**     Contents    :
**         Clear - byte WDog_Clear(void);
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file WDog.c
** @version 01.21
** @brief
**         This device "WatchDog" implements a watchdog.
**         When enabled, the watchdog timer counts until it reaches
**         a critical value corresponding to the period specified
**         in 'Properties'. Then, the watchdog causes a CPU reset.
**         Applications may clear the timer before it reaches the critical
**         value. The timer then restarts the watchdog counter.
**         Watchdog is used for increasing the safety of the system
**         (unforeseeable situations can cause system crash or an
**         infinite loop - watchdog can restart the system or handle
**         the situation).
**
**         Note: Watchdog can be enabled or disabled in the initialization
**               code only. If the watchdog is once enabled user have
**               to call Clear method in defined time intervals.
*/         
/*!
**  @addtogroup WDog_module WDog module documentation
**  @{
*/         


/* MODULE WDog. */

#include "WDog.h"
/*
** ===================================================================
**     Method      :  WDog_Clear (component WatchDog)
**     Description :
**         Clears the watchdog timer (it makes the timer restart from
**         zero).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - The component is disabled
** ===================================================================
*/
/*
void WDog_Clear(void)

**      This method is implemented as macro. See WDog.h file.      **
*/

/* END WDog. */

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
