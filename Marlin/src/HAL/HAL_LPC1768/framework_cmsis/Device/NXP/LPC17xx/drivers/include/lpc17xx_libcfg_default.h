/**********************************************************************
* $Id$		lpc17xx_libcfg_default.h				2010-05-21
*//**
* @file		lpc17xx_libcfg_default.h
* @brief	Default Library configuration header file
* @version	2.0
* @date		21. May. 2010
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2010, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

/* Library Configuration group ----------------------------------------------------------- */
/** @defgroup LIBCFG_DEFAULT LIBCFG_DEFAULT (Default Library Configuration)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_LIBCFG_DEFAULT_H_
#define LPC17XX_LIBCFG_DEFAULT_H_

/* Includes ------------------------------------------------------------------- */
#include "lpc_types.h"


/* Public Macros -------------------------------------------------------------- */
/** @defgroup LIBCFG_DEFAULT_Public_Macros LIBCFG_DEFAULT Public Macros
 * @{
 */

/************************** DEBUG MODE DEFINITIONS *********************************/
/* Un-comment the line below to compile the library in DEBUG mode, this will expanse
   the "CHECK_PARAM" macro in the FW library code */
//#define DEBUG


/******************* PERIPHERAL FW LIBRARY CONFIGURATION DEFINITIONS ***********************/
/* Comment the line below to disable the specific peripheral inclusion */

/* DEBUG_FRAMWORK ------------------------------ */
#define _DBGFWK

/* GPIO ------------------------------- */
#define _GPIO

/* EXTI ------------------------------- */
#define _EXTI

/* UART ------------------------------- */
#define _UART
#define _UART0
#define _UART1
#define _UART2
#define _UART3

/* SPI ------------------------------- */
#define _SPI

/* SYSTICK --------------------------- */
#define _SYSTICK

/* SSP ------------------------------- */
#define _SSP
#define _SSP0
#define _SSP1


/* I2C ------------------------------- */
#define _I2C
#define _I2C0
#define _I2C1
#define _I2C2

/* TIMER ------------------------------- */
#define _TIM

/* WDT ------------------------------- */
#define _WDT


/* GPDMA ------------------------------- */
#define _GPDMA


/* DAC ------------------------------- */
#define _DAC

/* DAC ------------------------------- */
#define _ADC


/* PWM ------------------------------- */
#define _PWM
#define _PWM1

/* RTC ------------------------------- */
#define _RTC

/* I2S ------------------------------- */
#define _I2S

/* USB device ------------------------------- */
#define _USBDEV
#define _USB_DMA

/* QEI ------------------------------- */
#define _QEI

/* MCPWM ------------------------------- */
#define _MCPWM

/* CAN--------------------------------*/
#define _CAN

/* RIT ------------------------------- */
#define _RIT

/* EMAC ------------------------------ */
#define _EMAC

/************************** GLOBAL/PUBLIC MACRO DEFINITIONS *********************************/

#ifdef  DEBUG
/*******************************************************************************
* @brief		The CHECK_PARAM macro is used for function's parameters check.
* 				It is used only if the library is compiled in DEBUG mode.
* @param[in]	expr - If expr is false, it calls check_failed() function
*                    	which reports the name of the source file and the source
*                    	line number of the call that failed.
*                    - If expr is true, it returns no value.
* @return		None
*******************************************************************************/
#define CHECK_PARAM(expr) ((expr) ? (void)0 : check_failed((uint8_t *)__FILE__, __LINE__))
#else
#define CHECK_PARAM(expr)
#endif /* DEBUG */

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup LIBCFG_DEFAULT_Public_Functions LIBCFG_DEFAULT Public Functions
 * @{
 */

#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line);
#endif

/**
 * @}
 */

#endif /* LPC17XX_LIBCFG_DEFAULT_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
