/**********************************************************************
* $Id$		lpc17xx_nvic.c				2010-05-21
*//**
* @file		lpc17xx_nvic.c
* @brief	Contains all expansion functions support for
* 			NVIC firmware library on LPC17xx. The main
* 			NVIC functions are defined in core_cm3.h
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

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup NVIC
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_nvic.h"


/* Private Macros ------------------------------------------------------------- */
/** @addtogroup NVIC_Private_Macros
 * @{
 */

/* Vector table offset bit mask */
#define NVIC_VTOR_MASK              0x3FFFFF80

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup NVIC_Public_Functions
 * @{
 */


/*****************************************************************************//**
 * @brief		De-initializes the NVIC peripheral registers to their default
 * 				reset values.
 * @param		None
 * @return      None
 *
 * These following NVIC peripheral registers will be de-initialized:
 * - Disable Interrupt (32 IRQ interrupt sources that matched with LPC17xx)
 * - Clear all Pending Interrupts (32 IRQ interrupt source that matched with LPC17xx)
 * - Clear all Interrupt Priorities (32 IRQ interrupt source that matched with LPC17xx)
 *******************************************************************************/
void NVIC_DeInit(void)
{
	uint8_t tmp;

	/* Disable all interrupts */
	NVIC->ICER[0] = 0xFFFFFFFF;
	NVIC->ICER[1] = 0x00000001;
	/* Clear all pending interrupts */
	NVIC->ICPR[0] = 0xFFFFFFFF;
	NVIC->ICPR[1] = 0x00000001;

	/* Clear all interrupt priority */
	for (tmp = 0; tmp < 32; tmp++) {
		NVIC->IP[tmp] = 0x00;
	}
}

/*****************************************************************************//**
 * @brief			De-initializes the SCB peripheral registers to their default
 *                  reset values.
 * @param			none
 * @return 			none
 *
 * These following SCB NVIC peripheral registers will be de-initialized:
 * - Interrupt Control State register
 * - Interrupt Vector Table Offset register
 * - Application Interrupt/Reset Control register
 * - System Control register
 * - Configuration Control register
 * - System Handlers Priority Registers
 * - System Handler Control and State Register
 * - Configurable Fault Status Register
 * - Hard Fault Status Register
 * - Debug Fault Status Register
 *******************************************************************************/
void NVIC_SCBDeInit(void)
{
	uint8_t tmp;

	SCB->ICSR = 0x0A000000;
	SCB->VTOR = 0x00000000;
	SCB->AIRCR = 0x05FA0000;
	SCB->SCR = 0x00000000;
	SCB->CCR = 0x00000000;

	for (tmp = 0; tmp < (sizeof(SCB->SHP) / sizeof(SCB->SHP[0])); tmp++) {
		SCB->SHP[tmp] = 0x00;
	}

	SCB->SHCSR = 0x00000000;
	SCB->CFSR = 0xFFFFFFFF;
	SCB->HFSR = 0xFFFFFFFF;
	SCB->DFSR = 0xFFFFFFFF;
}


/*****************************************************************************//**
 * @brief		Set Vector Table Offset value
 * @param		offset Offset value
 * @return      None
 *******************************************************************************/
void NVIC_SetVTOR(uint32_t offset)
{
//	SCB->VTOR  = (offset & NVIC_VTOR_MASK);
	SCB->VTOR  = offset;
}

/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
