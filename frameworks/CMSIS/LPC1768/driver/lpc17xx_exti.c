/**********************************************************************
* $Id$		lpc17xx_exti.c				2010-06-18
*//**
* @file		lpc17xx_exti.c
* @brief	Contains all functions support for External interrupt firmware
* 			library on LPC17xx
* @version	3.0
* @date		18. June. 2010
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
/** @addtogroup EXTI
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_exti.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc17xx_libcfg.h"
#else
#include "lpc17xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */


#ifdef _EXTI

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup EXTI_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Initial for EXT
 * 				- Set EXTINT, EXTMODE, EXTPOLAR registers to default value
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EXTI_Init(void)
{
	LPC_SC->EXTINT = 0xF;
	LPC_SC->EXTMODE = 0x0;
	LPC_SC->EXTPOLAR = 0x0;
}


/*********************************************************************//**
* @brief 		Close EXT
* @param[in]	None
* @return 		None
**********************************************************************/
void	EXTI_DeInit(void)
{
	;
}

/*********************************************************************//**
 * @brief 		Configuration for EXT
 * 				- Set EXTINT, EXTMODE, EXTPOLAR register
 * @param[in]	EXTICfg	Pointer to a EXTI_InitTypeDef structure
 *              that contains the configuration information for the
 *              specified external interrupt
 * @return 		None
 **********************************************************************/
void EXTI_Config(EXTI_InitTypeDef *EXTICfg)
{
	LPC_SC->EXTINT = 0x0;
	EXTI_SetMode(EXTICfg->EXTI_Line, EXTICfg->EXTI_Mode);
	EXTI_SetPolarity(EXTICfg->EXTI_Line, EXTICfg->EXTI_polarity);
}

/*********************************************************************//**
* @brief 		Set mode for EXTI pin
* @param[in]	EXTILine	 external interrupt line, should be:
* 				- EXTI_EINT0: external interrupt line 0
* 				- EXTI_EINT1: external interrupt line 1
* 				- EXTI_EINT2: external interrupt line 2
* 				- EXTI_EINT3: external interrupt line 3
* @param[in]	mode 	external mode, should be:
* 				- EXTI_MODE_LEVEL_SENSITIVE
* 				- EXTI_MODE_EDGE_SENSITIVE
* @return 		None
*********************************************************************/
void EXTI_SetMode(EXTI_LINE_ENUM EXTILine, EXTI_MODE_ENUM mode)
{
	if(mode == EXTI_MODE_EDGE_SENSITIVE)
	{
		LPC_SC->EXTMODE |= (1 << EXTILine);
	}
	else if(mode == EXTI_MODE_LEVEL_SENSITIVE)
	{
		LPC_SC->EXTMODE &= ~(1 << EXTILine);
	}
}

/*********************************************************************//**
* @brief 		Set polarity for EXTI pin
* @param[in]	EXTILine	 external interrupt line, should be:
* 				- EXTI_EINT0: external interrupt line 0
* 				- EXTI_EINT1: external interrupt line 1
* 				- EXTI_EINT2: external interrupt line 2
* 				- EXTI_EINT3: external interrupt line 3
* @param[in]	polarity	 external polarity value, should be:
* 				- EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE
* 				- EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE
* @return 		None
*********************************************************************/
void EXTI_SetPolarity(EXTI_LINE_ENUM EXTILine, EXTI_POLARITY_ENUM polarity)
{
	if(polarity == EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE)
	{
		LPC_SC->EXTPOLAR |= (1 << EXTILine);
	}
	else if(polarity == EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE)
	{
		LPC_SC->EXTPOLAR &= ~(1 << EXTILine);
	}
}

/*********************************************************************//**
* @brief 		Clear External interrupt flag
* @param[in]	EXTILine	 external interrupt line, should be:
* 				- EXTI_EINT0: external interrupt line 0
* 				- EXTI_EINT1: external interrupt line 1
* 				- EXTI_EINT2: external interrupt line 2
* 				- EXTI_EINT3: external interrupt line 3
* @return 		None
*********************************************************************/
void EXTI_ClearEXTIFlag(EXTI_LINE_ENUM EXTILine)
{
		LPC_SC->EXTINT = (1 << EXTILine);
}

/**
 * @}
 */

#endif /* _EXTI */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

