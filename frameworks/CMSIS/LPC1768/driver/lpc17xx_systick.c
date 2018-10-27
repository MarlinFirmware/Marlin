/**********************************************************************
* $Id$		lpc17xx_systick.c				2010-05-21
*//**
* @file		lpc17xx_systick.c
* @brief	Contains all functions support for SYSTICK firmware library
* 			on LPC17xx
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
/** @addtogroup SYSTICK
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_systick.h"
#include "lpc17xx_clkpwr.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc17xx_libcfg.h"
#else
#include "lpc17xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */


#ifdef _SYSTICK

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup SYSTICK_Public_Functions
 * @{
 */
/*********************************************************************//**
 * @brief 		Initial System Tick with using internal CPU clock source
 * @param[in]	time	time interval(ms)
 * @return 		None
 **********************************************************************/
void SYSTICK_InternalInit(uint32_t time)
{
	uint32_t cclk;
	float maxtime;

	cclk = SystemCoreClock;
	/* With internal CPU clock frequency for LPC17xx is 'SystemCoreClock'
	 * And limit 24 bit for RELOAD value
	 * So the maximum time can be set:
	 * 1/SystemCoreClock * (2^24) * 1000 (ms)
	 */
	//check time value is available or not
	maxtime = (1<<24)/(SystemCoreClock / 1000);
	if(time > maxtime)
		//Error loop
		while(1);
	else
	{
		//Select CPU clock is System Tick clock source
		SysTick->CTRL |= ST_CTRL_CLKSOURCE;
		/* Set RELOAD value
		 * RELOAD = (SystemCoreClock/1000) * time - 1
		 * with time base is millisecond
		 */
		SysTick->LOAD = (cclk/1000)*time - 1;
	}
}

/*********************************************************************//**
 * @brief 		Initial System Tick with using external clock source
 * @param[in]	freq	external clock frequency(Hz)
 * @param[in]	time	time interval(ms)
 * @return 		None
 **********************************************************************/
void SYSTICK_ExternalInit(uint32_t freq, uint32_t time)
{
	float maxtime;

	/* With external clock frequency for LPC17xx is 'freq'
	 * And limit 24 bit for RELOAD value
	 * So the maximum time can be set:
	 * 1/freq * (2^24) * 1000 (ms)
	 */
	//check time value is available or not
	maxtime = (1<<24)/(freq / 1000);
	if (time>maxtime)
		//Error Loop
		while(1);
	else
	{
		//Select external clock is System Tick clock source
		SysTick->CTRL &= ~ ST_CTRL_CLKSOURCE;
		/* Set RELOAD value
		 * RELOAD = (freq/1000) * time - 1
		 * with time base is millisecond
		 */
		maxtime = (freq/1000)*time - 1;
		SysTick->LOAD = (freq/1000)*time - 1;
	}
}

/*********************************************************************//**
 * @brief 		Enable/disable System Tick counter
 * @param[in]	NewState	System Tick counter status, should be:
 * 					- ENABLE
 * 					- DISABLE
 * @return 		None
 **********************************************************************/
void SYSTICK_Cmd(FunctionalState NewState)
{
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if(NewState == ENABLE)
		//Enable System Tick counter
		SysTick->CTRL |= ST_CTRL_ENABLE;
	else
		//Disable System Tick counter
		SysTick->CTRL &= ~ST_CTRL_ENABLE;
}

/*********************************************************************//**
 * @brief 		Enable/disable System Tick interrupt
 * @param[in]	NewState	System Tick interrupt status, should be:
 * 					- ENABLE
 * 					- DISABLE
 * @return 		None
 **********************************************************************/
void SYSTICK_IntCmd(FunctionalState NewState)
{
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if(NewState == ENABLE)
		//Enable System Tick counter
		SysTick->CTRL |= ST_CTRL_TICKINT;
	else
		//Disable System Tick counter
		SysTick->CTRL &= ~ST_CTRL_TICKINT;
}

/*********************************************************************//**
 * @brief 		Get current value of System Tick counter
 * @param[in]	None
 * @return 		current value of System Tick counter
 **********************************************************************/
uint32_t SYSTICK_GetCurrentValue(void)
{
	return (SysTick->VAL);
}

/*********************************************************************//**
 * @brief 		Clear Counter flag
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SYSTICK_ClearCounterFlag(void)
{
	SysTick->CTRL &= ~ST_CTRL_COUNTFLAG;
}
/**
 * @}
 */

#endif /* _SYSTICK */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

