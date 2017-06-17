/**********************************************************************
* $Id$		lpc17xx_rit.h				2010-05-21
*//**
* @file		lpc17xx_rit.h
* @brief	Contains all macro definitions and function prototypes
* 			support for RIT firmware library on LPC17xx
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
/** @defgroup RIT RIT (Repetitive Interrupt Timer)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_RIT_H_
#define LPC17XX_RIT_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Private Macros ------------------------------------------------------------- */
/** @defgroup RIT_Private_Macros RIT Private Macros
 * @{
 */

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for RIT control register
 **********************************************************************/
/**	Set interrupt flag when the counter value equals the masked compare value */
#define RIT_CTRL_INTEN	((uint32_t) (1))
/** Set timer enable clear to 0 when the counter value equals the masked compare value  */
#define RIT_CTRL_ENCLR 	((uint32_t) _BIT(1))
/** Set timer enable on debug */
#define RIT_CTRL_ENBR	((uint32_t) _BIT(2))
/** Set timer enable */
#define RIT_CTRL_TEN	((uint32_t) _BIT(3))

/** Macro to determine if it is valid RIT peripheral */
#define PARAM_RITx(n)	(((uint32_t *)n)==((uint32_t *)LPC_RIT))
/**
 * @}
 */



/* Public Functions ----------------------------------------------------------- */
/** @defgroup RIT_Public_Functions RIT Public Functions
 * @{
 */
/* RIT Init/DeInit functions */
void RIT_Init(LPC_RIT_TypeDef *RITx);
void RIT_DeInit(LPC_RIT_TypeDef *RITx);

/* RIT config timer functions */
void RIT_TimerConfig(LPC_RIT_TypeDef *RITx, uint32_t time_interval);

/* Enable/Disable RIT functions */
void RIT_TimerClearCmd(LPC_RIT_TypeDef *RITx, FunctionalState NewState);
void RIT_Cmd(LPC_RIT_TypeDef *RITx, FunctionalState NewState);
void RIT_TimerDebugCmd(LPC_RIT_TypeDef *RITx, FunctionalState NewState);

/* RIT Interrupt functions */
IntStatus RIT_GetIntStatus(LPC_RIT_TypeDef *RITx);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_RIT_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
