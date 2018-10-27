/**********************************************************************
* $Id$		lpc17xx_exti.h				2010-05-21
*//**
* @file		lpc17xx_exti.h
* @brief	Contains all macro definitions and function prototypes
* 			support for External interrupt firmware library on LPC17xx
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
/** @defgroup EXTI EXTI (External Interrupt)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_EXTI_H_
#define LPC17XX_EXTI_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Private Macros ------------------------------------------------------------- */
/** @defgroup EXTI_Private_Macros EXTI Private Macros
 * @{
 */
/*********************************************************************//**
 * Macro defines for EXTI  control register
 **********************************************************************/
#define EXTI_EINT0_BIT_MARK 	0x01
#define EXTI_EINT1_BIT_MARK 	0x02
#define EXTI_EINT2_BIT_MARK 	0x04
#define EXTI_EINT3_BIT_MARK 	0x08

/**
 * @}
 */

/* Private Macros ------------------------------------------------------------- */
/** @defgroup EXTI_Public_Types EXTI Public Types
 * @{
 */

/**
 * @brief EXTI external interrupt line option
 */
typedef enum
{
	EXTI_EINT0, /*!<  External interrupt 0, P2.10 */
	EXTI_EINT1, /*!<  External interrupt 0, P2.11 */
	EXTI_EINT2, /*!<  External interrupt 0, P2.12 */
	EXTI_EINT3 	/*!<  External interrupt 0, P2.13 */
} EXTI_LINE_ENUM;

/**
 * @brief EXTI mode option
 */
typedef enum
{
	EXTI_MODE_LEVEL_SENSITIVE, 	/*!< Level sensitivity is selected */
	EXTI_MODE_EDGE_SENSITIVE  	/*!< Edge sensitivity is selected */
} EXTI_MODE_ENUM;

/**
 * @brief EXTI polarity option
 */
typedef enum
{
	EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE,	/*!< Low active or falling edge sensitive
												depending on pin mode */
	EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE	/*!< High active or rising edge sensitive
												depending on pin mode */
} EXTI_POLARITY_ENUM;

/**
 * @brief EXTI Initialize structure
 */
typedef struct
{
	EXTI_LINE_ENUM EXTI_Line; /*!<Select external interrupt pin (EINT0, EINT1, EINT 2, EINT3) */

	EXTI_MODE_ENUM EXTI_Mode; /*!< Choose between Level-sensitivity or Edge sensitivity */

	EXTI_POLARITY_ENUM EXTI_polarity; /*!< 	If EXTI mode is level-sensitive: this element use to select low or high active level
											if EXTI mode is polarity-sensitive: this element use to select falling or rising edge */

}EXTI_InitTypeDef;


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup EXTI_Public_Functions EXTI Public Functions
 * @{
 */

void EXTI_Init(void);
void EXTI_DeInit(void);

void EXTI_Config(EXTI_InitTypeDef *EXTICfg);
void EXTI_SetMode(EXTI_LINE_ENUM EXTILine, EXTI_MODE_ENUM mode);
void EXTI_SetPolarity(EXTI_LINE_ENUM EXTILine, EXTI_POLARITY_ENUM polarity);
void EXTI_ClearEXTIFlag(EXTI_LINE_ENUM EXTILine);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* LPC17XX_EXTI_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
