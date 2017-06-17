/**********************************************************************
* $Id$		lpc17xx_pwm.c				2011-03-31
*//**
* @file		lpc17xx_pwm.c
* @brief	Contains all functions support for PWM firmware library on LPC17xx
* @version	2.1
* @date		31. Mar. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
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
/** @addtogroup PWM
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_pwm.h"
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


#ifdef _PWM


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup PWM_Public_Functions
 * @{
 */


/*********************************************************************//**
 * @brief 		Check whether specified interrupt flag in PWM is set or not
 * @param[in]	PWMx: PWM peripheral, should be LPC_PWM1
 * @param[in]	IntFlag: PWM interrupt flag, should be:
 * 				- PWM_INTSTAT_MR0: Interrupt flag for PWM match channel 0
 * 				- PWM_INTSTAT_MR1: Interrupt flag for PWM match channel 1
 * 				- PWM_INTSTAT_MR2: Interrupt flag for PWM match channel 2
 * 				- PWM_INTSTAT_MR3: Interrupt flag for PWM match channel 3
 * 				- PWM_INTSTAT_MR4: Interrupt flag for PWM match channel 4
 * 				- PWM_INTSTAT_MR5: Interrupt flag for PWM match channel 5
 * 				- PWM_INTSTAT_MR6: Interrupt flag for PWM match channel 6
 * 				- PWM_INTSTAT_CAP0: Interrupt flag for capture input 0
 * 				- PWM_INTSTAT_CAP1: Interrupt flag for capture input 1
 * @return 		New State of PWM interrupt flag (SET or RESET)
 **********************************************************************/
IntStatus PWM_GetIntStatus(LPC_PWM_TypeDef *PWMx, uint32_t IntFlag)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM_INTSTAT(IntFlag));

	return ((PWMx->IR & IntFlag) ? SET : RESET);
}



/*********************************************************************//**
 * @brief 		Clear specified PWM Interrupt pending
 * @param[in]	PWMx: PWM peripheral, should be LPC_PWM1
 * @param[in]	IntFlag: PWM interrupt flag, should be:
 * 				- PWM_INTSTAT_MR0: Interrupt flag for PWM match channel 0
 * 				- PWM_INTSTAT_MR1: Interrupt flag for PWM match channel 1
 * 				- PWM_INTSTAT_MR2: Interrupt flag for PWM match channel 2
 * 				- PWM_INTSTAT_MR3: Interrupt flag for PWM match channel 3
 * 				- PWM_INTSTAT_MR4: Interrupt flag for PWM match channel 4
 * 				- PWM_INTSTAT_MR5: Interrupt flag for PWM match channel 5
 * 				- PWM_INTSTAT_MR6: Interrupt flag for PWM match channel 6
 * 				- PWM_INTSTAT_CAP0: Interrupt flag for capture input 0
 * 				- PWM_INTSTAT_CAP1: Interrupt flag for capture input 1
 * @return 		None
 **********************************************************************/
void PWM_ClearIntPending(LPC_PWM_TypeDef *PWMx, uint32_t IntFlag)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM_INTSTAT(IntFlag));
	PWMx->IR = IntFlag;
}



/*****************************************************************************//**
* @brief		Fills each PWM_InitStruct member with its default value:
* 				- If PWMCounterMode = PWM_MODE_TIMER:
* 					+ PrescaleOption = PWM_TIMER_PRESCALE_USVAL
* 					+ PrescaleValue = 1
* 				- If PWMCounterMode = PWM_MODE_COUNTER:
* 					+ CountInputSelect = PWM_COUNTER_PCAP1_0
* 					+ CounterOption = PWM_COUNTER_RISING
* @param[in]	PWMTimerCounterMode Timer or Counter mode, should be:
* 				- PWM_MODE_TIMER: Counter of PWM peripheral is in Timer mode
* 				- PWM_MODE_COUNTER: Counter of PWM peripheral is in Counter mode
* @param[in]	PWM_InitStruct Pointer to structure (PWM_TIMERCFG_Type or
* 				 PWM_COUNTERCFG_Type) which will be initialized.
* @return		None
* Note: PWM_InitStruct pointer will be assigned to corresponding structure
* 		(PWM_TIMERCFG_Type or PWM_COUNTERCFG_Type) due to PWMTimerCounterMode.
*******************************************************************************/
void PWM_ConfigStructInit(uint8_t PWMTimerCounterMode, void *PWM_InitStruct)
{
	PWM_TIMERCFG_Type *pTimeCfg;
	PWM_COUNTERCFG_Type *pCounterCfg;
	CHECK_PARAM(PARAM_PWM_TC_MODE(PWMTimerCounterMode));

	pTimeCfg = (PWM_TIMERCFG_Type *) PWM_InitStruct;
	pCounterCfg = (PWM_COUNTERCFG_Type *) PWM_InitStruct;

	if (PWMTimerCounterMode == PWM_MODE_TIMER )
	{
		pTimeCfg->PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
		pTimeCfg->PrescaleValue = 1;
	}
	else if (PWMTimerCounterMode == PWM_MODE_COUNTER)
	{
		pCounterCfg->CountInputSelect = PWM_COUNTER_PCAP1_0;
		pCounterCfg->CounterOption = PWM_COUNTER_RISING;
	}
}


/*********************************************************************//**
 * @brief 		Initializes the PWMx peripheral corresponding to the specified
 *               parameters in the PWM_ConfigStruct.
 * @param[in]	PWMx PWM peripheral, should be LPC_PWM1
 * @param[in]	PWMTimerCounterMode Timer or Counter mode, should be:
 * 				- PWM_MODE_TIMER: Counter of PWM peripheral is in Timer mode
 * 				- PWM_MODE_COUNTER: Counter of PWM peripheral is in Counter mode
 * @param[in]	PWM_ConfigStruct Pointer to structure (PWM_TIMERCFG_Type or
 * 				 PWM_COUNTERCFG_Type) which will be initialized.
 * @return 		None
 * Note: PWM_ConfigStruct pointer will be assigned to corresponding structure
 * 		(PWM_TIMERCFG_Type or PWM_COUNTERCFG_Type) due to PWMTimerCounterMode.
 **********************************************************************/
void PWM_Init(LPC_PWM_TypeDef *PWMx, uint32_t PWMTimerCounterMode, void *PWM_ConfigStruct)
{
	PWM_TIMERCFG_Type *pTimeCfg;
	PWM_COUNTERCFG_Type *pCounterCfg;
	uint64_t clkdlycnt;

	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM_TC_MODE(PWMTimerCounterMode));

	pTimeCfg = (PWM_TIMERCFG_Type *)PWM_ConfigStruct;
	pCounterCfg = (PWM_COUNTERCFG_Type *)PWM_ConfigStruct;


	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCPWM1, ENABLE);
	CLKPWR_SetPCLKDiv (CLKPWR_PCLKSEL_PWM1, CLKPWR_PCLKSEL_CCLK_DIV_4);
	// Get peripheral clock of PWM1
	clkdlycnt = (uint64_t) CLKPWR_GetPCLK (CLKPWR_PCLKSEL_PWM1);


	// Clear all interrupts pending
	PWMx->IR = 0xFF & PWM_IR_BITMASK;
	PWMx->TCR = 0x00;
	PWMx->CTCR = 0x00;
	PWMx->MCR = 0x00;
	PWMx->CCR = 0x00;
	PWMx->PCR = 0x00;
	PWMx->LER = 0x00;

	if (PWMTimerCounterMode == PWM_MODE_TIMER)
	{
		CHECK_PARAM(PARAM_PWM_TIMER_PRESCALE(pTimeCfg->PrescaleOption));

		/* Absolute prescale value */
		if (pTimeCfg->PrescaleOption == PWM_TIMER_PRESCALE_TICKVAL)
		{
			PWMx->PR   = pTimeCfg->PrescaleValue - 1;
		}
		/* uSecond prescale value */
		else
		{
			clkdlycnt = (clkdlycnt * pTimeCfg->PrescaleValue) / 1000000;
			PWMx->PR = ((uint32_t) clkdlycnt) - 1;
		}

	}
	else if (PWMTimerCounterMode == PWM_MODE_COUNTER)
	{
		CHECK_PARAM(PARAM_PWM_COUNTER_INPUTSEL(pCounterCfg->CountInputSelect));
		CHECK_PARAM(PARAM_PWM_COUNTER_EDGE(pCounterCfg->CounterOption));

		PWMx->CTCR |= (PWM_CTCR_MODE((uint32_t)pCounterCfg->CounterOption)) \
						| (PWM_CTCR_SELECT_INPUT((uint32_t)pCounterCfg->CountInputSelect));
	}
}

/*********************************************************************//**
 * @brief		De-initializes the PWM peripheral registers to their
*                  default reset values.
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @return 		None
 **********************************************************************/
void PWM_DeInit (LPC_PWM_TypeDef *PWMx)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));

	// Disable PWM control (timer, counter and PWM)
	PWMx->TCR = 0x00;
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCPWM1, DISABLE);

}


/*********************************************************************//**
 * @brief	 	Enable/Disable PWM peripheral
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]	NewState	New State of this function, should be:
 * 							- ENABLE: Enable PWM peripheral
 * 							- DISABLE: Disable PWM peripheral
 * @return 		None
 **********************************************************************/
void PWM_Cmd(LPC_PWM_TypeDef *PWMx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		PWMx->TCR	|=  PWM_TCR_PWM_ENABLE;
	}
	else
	{
		PWMx->TCR &= (~PWM_TCR_PWM_ENABLE) & PWM_TCR_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Enable/Disable Counter in PWM peripheral
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]	NewState New State of this function, should be:
 * 							- ENABLE: Enable Counter in PWM peripheral
 * 							- DISABLE: Disable Counter in PWM peripheral
 * @return 		None
 **********************************************************************/
void PWM_CounterCmd(LPC_PWM_TypeDef *PWMx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));
	if (NewState == ENABLE)
	{
		PWMx->TCR	|=  PWM_TCR_COUNTER_ENABLE;
	}
	else
	{
		PWMx->TCR &= (~PWM_TCR_COUNTER_ENABLE) & PWM_TCR_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Reset Counter in PWM peripheral
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @return 		None
 **********************************************************************/
void PWM_ResetCounter(LPC_PWM_TypeDef *PWMx)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	PWMx->TCR |= PWM_TCR_COUNTER_RESET;
	PWMx->TCR &= (~PWM_TCR_COUNTER_RESET) & PWM_TCR_BITMASK;
}


/*********************************************************************//**
 * @brief 		Configures match for PWM peripheral
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]   PWM_MatchConfigStruct	Pointer to a PWM_MATCHCFG_Type structure
*                    that contains the configuration information for the
*                    specified PWM match function.
 * @return 		None
 **********************************************************************/
void PWM_ConfigMatch(LPC_PWM_TypeDef *PWMx, PWM_MATCHCFG_Type *PWM_MatchConfigStruct)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM1_MATCH_CHANNEL(PWM_MatchConfigStruct->MatchChannel));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(PWM_MatchConfigStruct->IntOnMatch));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(PWM_MatchConfigStruct->ResetOnMatch));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(PWM_MatchConfigStruct->StopOnMatch));

	//interrupt on MRn
	if (PWM_MatchConfigStruct->IntOnMatch == ENABLE)
	{
		PWMx->MCR |= PWM_MCR_INT_ON_MATCH(PWM_MatchConfigStruct->MatchChannel);
	}
	else
	{
		PWMx->MCR &= (~PWM_MCR_INT_ON_MATCH(PWM_MatchConfigStruct->MatchChannel)) \
					& PWM_MCR_BITMASK;
	}

	//reset on MRn
	if (PWM_MatchConfigStruct->ResetOnMatch == ENABLE)
	{
		PWMx->MCR |= PWM_MCR_RESET_ON_MATCH(PWM_MatchConfigStruct->MatchChannel);
	}
	else
	{
		PWMx->MCR &= (~PWM_MCR_RESET_ON_MATCH(PWM_MatchConfigStruct->MatchChannel)) \
					& PWM_MCR_BITMASK;
	}

	//stop on MRn
	if (PWM_MatchConfigStruct->StopOnMatch == ENABLE)
	{
		PWMx->MCR |= PWM_MCR_STOP_ON_MATCH(PWM_MatchConfigStruct->MatchChannel);
	}
	else
	{
		PWMx->MCR &= (~PWM_MCR_STOP_ON_MATCH(PWM_MatchConfigStruct->MatchChannel)) \
					& PWM_MCR_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Configures capture input for PWM peripheral
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]   PWM_CaptureConfigStruct	Pointer to a PWM_CAPTURECFG_Type structure
*                    that contains the configuration information for the
*                    specified PWM capture input function.
 * @return 		None
 **********************************************************************/
void PWM_ConfigCapture(LPC_PWM_TypeDef *PWMx, PWM_CAPTURECFG_Type *PWM_CaptureConfigStruct)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM1_CAPTURE_CHANNEL(PWM_CaptureConfigStruct->CaptureChannel));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(PWM_CaptureConfigStruct->FallingEdge));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(PWM_CaptureConfigStruct->IntOnCaption));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(PWM_CaptureConfigStruct->RisingEdge));

	if (PWM_CaptureConfigStruct->RisingEdge == ENABLE)
	{
		PWMx->CCR |= PWM_CCR_CAP_RISING(PWM_CaptureConfigStruct->CaptureChannel);
	}
	else
	{
		PWMx->CCR &= (~PWM_CCR_CAP_RISING(PWM_CaptureConfigStruct->CaptureChannel)) \
					& PWM_CCR_BITMASK;
	}

	if (PWM_CaptureConfigStruct->FallingEdge == ENABLE)
	{
		PWMx->CCR |= PWM_CCR_CAP_FALLING(PWM_CaptureConfigStruct->CaptureChannel);
	}
	else
	{
		PWMx->CCR &= (~PWM_CCR_CAP_FALLING(PWM_CaptureConfigStruct->CaptureChannel)) \
					& PWM_CCR_BITMASK;
	}

	if (PWM_CaptureConfigStruct->IntOnCaption == ENABLE)
	{
		PWMx->CCR |= PWM_CCR_INT_ON_CAP(PWM_CaptureConfigStruct->CaptureChannel);
	}
	else
	{
		PWMx->CCR &= (~PWM_CCR_INT_ON_CAP(PWM_CaptureConfigStruct->CaptureChannel)) \
					& PWM_CCR_BITMASK;
	}
}


/*********************************************************************//**
 * @brief 		Read value of capture register PWM peripheral
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]	CaptureChannel: capture channel number, should be in
 * 				range 0 to 1
 * @return 		Value of capture register
 **********************************************************************/
uint32_t PWM_GetCaptureValue(LPC_PWM_TypeDef *PWMx, uint8_t CaptureChannel)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM1_CAPTURE_CHANNEL(CaptureChannel));

	switch (CaptureChannel)
	{
	case 0:
		return PWMx->CR0;

	case 1:
		return PWMx->CR1;

	default:
		return (0);
	}
}


/********************************************************************//**
 * @brief 		Update value for each PWM channel with update type option
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]	MatchChannel Match channel
 * @param[in]	MatchValue Match value
 * @param[in]	UpdateType Type of Update, should be:
 * 				- PWM_MATCH_UPDATE_NOW: The update value will be updated for
 * 					this channel immediately
 * 				- PWM_MATCH_UPDATE_NEXT_RST: The update value will be updated for
 * 					this channel on next reset by a PWM Match event.
 * @return		None
 *********************************************************************/
void PWM_MatchUpdate(LPC_PWM_TypeDef *PWMx, uint8_t MatchChannel, \
					uint32_t MatchValue, uint8_t UpdateType)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM1_MATCH_CHANNEL(MatchChannel));
	CHECK_PARAM(PARAM_PWM_MATCH_UPDATE(UpdateType));

	switch (MatchChannel)
	{
	case 0:
		PWMx->MR0 = MatchValue;
		break;

	case 1:
		PWMx->MR1 = MatchValue;
		break;

	case 2:
		PWMx->MR2 = MatchValue;
		break;

	case 3:
		PWMx->MR3 = MatchValue;
		break;

	case 4:
		PWMx->MR4 = MatchValue;
		break;

	case 5:
		PWMx->MR5 = MatchValue;
		break;

	case 6:
		PWMx->MR6 = MatchValue;
		break;
	}

	// Write Latch register
	PWMx->LER |= PWM_LER_EN_MATCHn_LATCH(MatchChannel);

	// In case of update now
	if (UpdateType == PWM_MATCH_UPDATE_NOW)
	{
		PWMx->TCR |= PWM_TCR_COUNTER_RESET;
		PWMx->TCR &= (~PWM_TCR_COUNTER_RESET) & PWM_TCR_BITMASK;
	}
}

/********************************************************************//**
 * @brief 		Update value for multi PWM channel with update type option
 * 				at the same time
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]	MatchStruct Structure that contents match value of 7 pwm channels
 * @param[in]	UpdateType Type of Update, should be:
 * 				- PWM_MATCH_UPDATE_NOW: The update value will be updated for
 * 					this channel immediately
 * 				- PWM_MATCH_UPDATE_NEXT_RST: The update value will be updated for
 * 					this channel on next reset by a PWM Match event.
 * @return		None
 *********************************************************************/
void PWM_MultiMatchUpdate(LPC_PWM_TypeDef *PWMx, PWM_Match_T *MatchStruct , uint8_t UpdateType)
{
	uint8_t LatchValue = 0;
	uint8_t i;

	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM_MATCH_UPDATE(UpdateType));

	//Update match value
	for(i=0;i<7;i++)
	{
		if(MatchStruct[i].Status == SET)
		{
			if(i<4)
				*((volatile unsigned int *)(&(PWMx->MR0) + i)) = MatchStruct[i].Matchvalue;
			else
			{
				*((volatile unsigned int *)(&(PWMx->MR4) + (i-4))) = MatchStruct[i].Matchvalue;
			}
			LatchValue |=(1<<i);
		}
	}
	//set update for multi-channel at the same time
	PWMx->LER = LatchValue;

	// In case of update now
	if (UpdateType == PWM_MATCH_UPDATE_NOW)
	{
		PWMx->TCR |= PWM_TCR_COUNTER_RESET;
		PWMx->TCR &= (~PWM_TCR_COUNTER_RESET) & PWM_TCR_BITMASK;
	}
}
/********************************************************************//**
 * @brief 		Configure Edge mode for each PWM channel
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]	PWMChannel PWM channel, should be in range from 2 to 6
 * @param[in]	ModeOption PWM mode option, should be:
 * 				- PWM_CHANNEL_SINGLE_EDGE: Single Edge mode
 * 				- PWM_CHANNEL_DUAL_EDGE: Dual Edge mode
 * @return 		None
 * Note: PWM Channel 1 can not be selected for mode option
 *********************************************************************/
void PWM_ChannelConfig(LPC_PWM_TypeDef *PWMx, uint8_t PWMChannel, uint8_t ModeOption)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM1_EDGE_MODE_CHANNEL(PWMChannel));
	CHECK_PARAM(PARAM_PWM_CHANNEL_EDGE(ModeOption));

	// Single edge mode
	if (ModeOption == PWM_CHANNEL_SINGLE_EDGE)
	{
		PWMx->PCR &= (~PWM_PCR_PWMSELn(PWMChannel)) & PWM_PCR_BITMASK;
	}
	// Double edge mode
	else if (PWM_CHANNEL_DUAL_EDGE)
	{
		PWMx->PCR |= PWM_PCR_PWMSELn(PWMChannel);
	}
}



/********************************************************************//**
 * @brief 		Enable/Disable PWM channel output
 * @param[in]	PWMx	PWM peripheral selected, should be LPC_PWM1
 * @param[in]	PWMChannel PWM channel, should be in range from 1 to 6
 * @param[in]	NewState New State of this function, should be:
 * 				- ENABLE: Enable this PWM channel output
 * 				- DISABLE: Disable this PWM channel output
 * @return		None
 *********************************************************************/
void PWM_ChannelCmd(LPC_PWM_TypeDef *PWMx, uint8_t PWMChannel, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_PWMx(PWMx));
	CHECK_PARAM(PARAM_PWM1_CHANNEL(PWMChannel));

	if (NewState == ENABLE)
	{
		PWMx->PCR |= PWM_PCR_PWMENAn(PWMChannel);
	}
	else
	{
		PWMx->PCR &= (~PWM_PCR_PWMENAn(PWMChannel)) & PWM_PCR_BITMASK;
	}
}

/**
 * @}
 */

#endif /* _PWM */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
