/**********************************************************************
* $Id$		lpc17xx_pinsel.h				2010-05-21
*//**
* @file		lpc17xx_pinsel.h
* @brief	Contains all macro definitions and function prototypes
* 			support for Pin connect block firmware library on LPC17xx
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
/** @defgroup PINSEL PINSEL (Pin Selection)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_PINSEL_H_
#define LPC17XX_PINSEL_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup PINSEL_Public_Macros PINSEL Public Macros
 * @{
 */

/*********************************************************************//**
 *!< Macros define for PORT Selection
 ***********************************************************************/
#define PINSEL_PORT_0 	((0))	/**< PORT 0*/
#define PINSEL_PORT_1 	((1))	/**< PORT 1*/
#define PINSEL_PORT_2 	((2))	/**< PORT 2*/
#define PINSEL_PORT_3 	((3))	/**< PORT 3*/
#define PINSEL_PORT_4 	((4))	/**< PORT 4*/

/***********************************************************************
 * Macros define for Pin Function selection
 **********************************************************************/
#define PINSEL_FUNC_0	((0))	/**< default function*/
#define PINSEL_FUNC_1	((1))	/**< first alternate function*/
#define PINSEL_FUNC_2	((2))	/**< second alternate function*/
#define PINSEL_FUNC_3	((3))	/**< third or reserved alternate function*/

/***********************************************************************
 * Macros define for Pin Number of Port
 **********************************************************************/
#define PINSEL_PIN_0 	((0)) 	/**< Pin 0 */
#define PINSEL_PIN_1 	((1)) 	/**< Pin 1 */
#define PINSEL_PIN_2 	((2)) 	/**< Pin 2 */
#define PINSEL_PIN_3 	((3)) 	/**< Pin 3 */
#define PINSEL_PIN_4 	((4)) 	/**< Pin 4 */
#define PINSEL_PIN_5 	((5)) 	/**< Pin 5 */
#define PINSEL_PIN_6 	((6)) 	/**< Pin 6 */
#define PINSEL_PIN_7 	((7)) 	/**< Pin 7 */
#define PINSEL_PIN_8 	((8)) 	/**< Pin 8 */
#define PINSEL_PIN_9 	((9)) 	/**< Pin 9 */
#define PINSEL_PIN_10 	((10)) 	/**< Pin 10 */
#define PINSEL_PIN_11 	((11)) 	/**< Pin 11 */
#define PINSEL_PIN_12 	((12)) 	/**< Pin 12 */
#define PINSEL_PIN_13 	((13)) 	/**< Pin 13 */
#define PINSEL_PIN_14 	((14)) 	/**< Pin 14 */
#define PINSEL_PIN_15 	((15)) 	/**< Pin 15 */
#define PINSEL_PIN_16 	((16)) 	/**< Pin 16 */
#define PINSEL_PIN_17 	((17)) 	/**< Pin 17 */
#define PINSEL_PIN_18 	((18)) 	/**< Pin 18 */
#define PINSEL_PIN_19 	((19)) 	/**< Pin 19 */
#define PINSEL_PIN_20 	((20)) 	/**< Pin 20 */
#define PINSEL_PIN_21 	((21)) 	/**< Pin 21 */
#define PINSEL_PIN_22 	((22)) 	/**< Pin 22 */
#define PINSEL_PIN_23 	((23)) 	/**< Pin 23 */
#define PINSEL_PIN_24 	((24)) 	/**< Pin 24 */
#define PINSEL_PIN_25 	((25)) 	/**< Pin 25 */
#define PINSEL_PIN_26 	((26)) 	/**< Pin 26 */
#define PINSEL_PIN_27 	((27)) 	/**< Pin 27 */
#define PINSEL_PIN_28 	((28)) 	/**< Pin 28 */
#define PINSEL_PIN_29 	((29)) 	/**< Pin 29 */
#define PINSEL_PIN_30 	((30)) 	/**< Pin 30 */
#define PINSEL_PIN_31 	((31)) 	/**< Pin 31 */

/***********************************************************************
 * Macros define for Pin mode
 **********************************************************************/
#define PINSEL_PINMODE_PULLUP		((0))	/**< Internal pull-up resistor*/
#define PINSEL_PINMODE_TRISTATE 	((2))	/**< Tri-state */
#define PINSEL_PINMODE_PULLDOWN 	((3)) 	/**< Internal pull-down resistor */

/***********************************************************************
 * Macros define for Pin mode (normal/open drain)
 **********************************************************************/
#define	PINSEL_PINMODE_NORMAL		((0))	/**< Pin is in the normal (not open drain) mode.*/
#define	PINSEL_PINMODE_OPENDRAIN	((1)) 	/**< Pin is in the open drain mode */

/***********************************************************************
 * Macros define for I2C mode
 ***********************************************************************/
#define	PINSEL_I2C_Normal_Mode		((0))	/**< The standard drive mode */
#define	PINSEL_I2C_Fast_Mode		((1)) 	/**<  Fast Mode Plus drive mode */

/**
 * @}
 */

/* Private Macros ------------------------------------------------------------- */
/** @defgroup PINSEL_Private_Macros PINSEL Private Macros
 * @{
 */

/* Pin selection define */
/* I2C Pin Configuration register bit description */
#define PINSEL_I2CPADCFG_SDADRV0 	_BIT(0) /**< Drive mode control for the SDA0 pin, P0.27 */
#define PINSEL_I2CPADCFG_SDAI2C0	_BIT(1) /**< I2C mode control for the SDA0 pin, P0.27 */
#define PINSEL_I2CPADCFG_SCLDRV0	_BIT(2) /**< Drive mode control for the SCL0 pin, P0.28 */
#define PINSEL_I2CPADCFG_SCLI2C0	_BIT(3) /**< I2C mode control for the SCL0 pin, P0.28 */

/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup PINSEL_Public_Types PINSEL Public Types
 * @{
 */

/** @brief Pin configuration structure */
typedef struct
{
	uint8_t Portnum;	/**< Port Number, should be PINSEL_PORT_x,
						where x should be in range from 0 to 4 */
	uint8_t Pinnum;		/**< Pin Number, should be PINSEL_PIN_x,
						where x should be in range from 0 to 31 */
	uint8_t Funcnum;	/**< Function Number, should be PINSEL_FUNC_x,
						where x should be in range from 0 to 3 */
	uint8_t Pinmode;	/**< Pin Mode, should be:
						- PINSEL_PINMODE_PULLUP: Internal pull-up resistor
						- PINSEL_PINMODE_TRISTATE: Tri-state
						- PINSEL_PINMODE_PULLDOWN: Internal pull-down resistor */
	uint8_t OpenDrain;	/**< OpenDrain mode, should be:
						- PINSEL_PINMODE_NORMAL: Pin is in the normal (not open drain) mode
						- PINSEL_PINMODE_OPENDRAIN: Pin is in the open drain mode */
} PINSEL_CFG_Type;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup PINSEL_Public_Functions PINSEL Public Functions
 * @{
 */

void PINSEL_ConfigPin(PINSEL_CFG_Type *PinCfg);
void PINSEL_ConfigTraceFunc (FunctionalState NewState);
void PINSEL_SetI2C0Pins(uint8_t i2cPinMode, FunctionalState filterSlewRateEnable);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_PINSEL_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

