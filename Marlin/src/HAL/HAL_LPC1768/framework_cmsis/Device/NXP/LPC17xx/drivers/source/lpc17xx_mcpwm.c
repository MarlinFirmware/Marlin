/**********************************************************************
* $Id$		lpc17xx_mcpwm.c				2010-05-21
*//**
* @file		lpc17xx_mcpwm.c
* @brief	Contains all functions support for Motor Control PWM firmware
* 			library on LPC17xx
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
/** @addtogroup MCPWM
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_mcpwm.h"
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


#ifdef _MCPWM

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup MCPWM_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief		Initializes the MCPWM peripheral
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected,
 * 				Should be: LPC_MCPWM
 * @return		None
 **********************************************************************/
void MCPWM_Init(LPC_MCPWM_TypeDef *MCPWMx)
{

	/* Turn On MCPWM PCLK */
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCMC, ENABLE);
	/* As default, peripheral clock for MCPWM module
	 * is set to FCCLK / 2 */
	// CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_MC, CLKPWR_PCLKSEL_CCLK_DIV_2);

	MCPWMx->MCCAP_CLR = MCPWM_CAPCLR_CAP(0) | MCPWM_CAPCLR_CAP(1) | MCPWM_CAPCLR_CAP(2);
	MCPWMx->MCINTFLAG_CLR = MCPWM_INT_ILIM(0) | MCPWM_INT_ILIM(1) | MCPWM_INT_ILIM(2) \
							| MCPWM_INT_IMAT(0) | MCPWM_INT_IMAT(1) | MCPWM_INT_IMAT(2) \
							| MCPWM_INT_ICAP(0) | MCPWM_INT_ICAP(1) | MCPWM_INT_ICAP(2);
	MCPWMx->MCINTEN_CLR = MCPWM_INT_ILIM(0) | MCPWM_INT_ILIM(1) | MCPWM_INT_ILIM(2) \
							| MCPWM_INT_IMAT(0) | MCPWM_INT_IMAT(1) | MCPWM_INT_IMAT(2) \
							| MCPWM_INT_ICAP(0) | MCPWM_INT_ICAP(1) | MCPWM_INT_ICAP(2);
}


/*********************************************************************//**
 * @brief		Configures each channel in MCPWM peripheral according to the
 * 				specified parameters in the MCPWM_CHANNEL_CFG_Type.
 * @param[in]	MCPWMx 			Motor Control PWM peripheral selected
 * 								should be: LPC_MCPWM
 * @param[in]	channelNum		Channel number, should be: 0..2.
 * @param[in]	channelSetup	Pointer to a MCPWM_CHANNEL_CFG_Type structure
*                    			that contains the configuration information for the
*                    			specified MCPWM channel.
 * @return		None
 **********************************************************************/
void MCPWM_ConfigChannel(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channelNum,
						MCPWM_CHANNEL_CFG_Type * channelSetup)
{
	if (channelNum <= 2) {
		if (channelNum == 0) {
			MCPWMx->MCTIM0 = channelSetup->channelTimercounterValue;
			MCPWMx->MCPER0 = channelSetup->channelPeriodValue;
			MCPWMx->MCPW0 = channelSetup->channelPulsewidthValue;
		} else if (channelNum == 1) {
			MCPWMx->MCTIM1 = channelSetup->channelTimercounterValue;
			MCPWMx->MCPER1 = channelSetup->channelPeriodValue;
			MCPWMx->MCPW1 = channelSetup->channelPulsewidthValue;
		} else if (channelNum == 2) {
			MCPWMx->MCTIM2 = channelSetup->channelTimercounterValue;
			MCPWMx->MCPER2 = channelSetup->channelPeriodValue;
			MCPWMx->MCPW2 = channelSetup->channelPulsewidthValue;
		} else {
			return;
		}

		if (channelSetup->channelType /* == MCPWM_CHANNEL_CENTER_MODE */){
			MCPWMx->MCCON_SET = MCPWM_CON_CENTER(channelNum);
		} else {
			MCPWMx->MCCON_CLR = MCPWM_CON_CENTER(channelNum);
		}

		if (channelSetup->channelPolarity /* == MCPWM_CHANNEL_PASSIVE_HI */){
			MCPWMx->MCCON_SET = MCPWM_CON_POLAR(channelNum);
		} else {
			MCPWMx->MCCON_CLR = MCPWM_CON_POLAR(channelNum);
		}

		if (channelSetup->channelDeadtimeEnable /* == ENABLE */){
			MCPWMx->MCCON_SET = MCPWM_CON_DTE(channelNum);
			MCPWMx->MCDEADTIME &= ~(MCPWM_DT(channelNum, 0x3FF));
			MCPWMx->MCDEADTIME |= MCPWM_DT(channelNum, channelSetup->channelDeadtimeValue);
		} else {
			MCPWMx->MCCON_CLR = MCPWM_CON_DTE(channelNum);
		}

		if (channelSetup->channelUpdateEnable /* == ENABLE */){
			MCPWMx->MCCON_CLR = MCPWM_CON_DISUP(channelNum);
		} else {
			MCPWMx->MCCON_SET = MCPWM_CON_DISUP(channelNum);
		}
	}
}


/*********************************************************************//**
 * @brief		Write to MCPWM shadow registers - Update the value for period
 * 				and pulse width in MCPWM peripheral.
 * @param[in]	MCPWMx 			Motor Control PWM peripheral selected
 * 								Should be: LPC_MCPWM
 * @param[in]	channelNum		Channel Number, should be: 0..2.
 * @param[in]	channelSetup	Pointer to a MCPWM_CHANNEL_CFG_Type structure
*                    			that contains the configuration information for the
*                    			specified MCPWM channel.
 * @return		None
 **********************************************************************/
void MCPWM_WriteToShadow(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channelNum,
								MCPWM_CHANNEL_CFG_Type *channelSetup)
{
	if (channelNum == 0){
		MCPWMx->MCPER0 = channelSetup->channelPeriodValue;
		MCPWMx->MCPW0 = channelSetup->channelPulsewidthValue;
	} else if (channelNum == 1) {
		MCPWMx->MCPER1 = channelSetup->channelPeriodValue;
		MCPWMx->MCPW1 = channelSetup->channelPulsewidthValue;
	} else if (channelNum == 2) {
		MCPWMx->MCPER2 = channelSetup->channelPeriodValue;
		MCPWMx->MCPW2 = channelSetup->channelPulsewidthValue;
	}
}



/*********************************************************************//**
 * @brief		Configures capture function in MCPWM peripheral
 * @param[in]	MCPWMx 			Motor Control PWM peripheral selected
 * 								Should be: LPC_MCPWM
 * @param[in]	channelNum		MCI (Motor Control Input pin) number
 * 								Should be: 0..2
 * @param[in]	captureConfig	Pointer to a MCPWM_CAPTURE_CFG_Type structure
*                    			that contains the configuration information for the
*                    			specified MCPWM capture.
 * @return
 **********************************************************************/
void MCPWM_ConfigCapture(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channelNum,
						MCPWM_CAPTURE_CFG_Type *captureConfig)
{
	if (channelNum <= 2) {

		if (captureConfig->captureFalling /* == ENABLE */) {
			MCPWMx->MCCAPCON_SET = MCPWM_CAPCON_CAPMCI_FE(captureConfig->captureChannel, channelNum);
		} else {
			MCPWMx->MCCAPCON_CLR = MCPWM_CAPCON_CAPMCI_FE(captureConfig->captureChannel, channelNum);
		}

		if (captureConfig->captureRising /* == ENABLE */) {
			MCPWMx->MCCAPCON_SET = MCPWM_CAPCON_CAPMCI_RE(captureConfig->captureChannel, channelNum);
		} else {
			MCPWMx->MCCAPCON_CLR = MCPWM_CAPCON_CAPMCI_RE(captureConfig->captureChannel, channelNum);
		}

		if (captureConfig->timerReset /* == ENABLE */){
			MCPWMx->MCCAPCON_SET = MCPWM_CAPCON_RT(captureConfig->captureChannel);
		} else {
			MCPWMx->MCCAPCON_CLR = MCPWM_CAPCON_RT(captureConfig->captureChannel);
		}

		if (captureConfig->hnfEnable /* == ENABLE */){
			MCPWMx->MCCAPCON_SET = MCPWM_CAPCON_HNFCAP(channelNum);
		} else {
			MCPWMx->MCCAPCON_CLR = MCPWM_CAPCON_HNFCAP(channelNum);
		}
	}
}


/*********************************************************************//**
 * @brief		Clears current captured value in specified capture channel
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected
 * 							Should be: LPC_MCPWM
 * @param[in]	captureChannel	Capture channel number, should be: 0..2
 * @return		None
 **********************************************************************/
void MCPWM_ClearCapture(LPC_MCPWM_TypeDef *MCPWMx, uint32_t captureChannel)
{
	MCPWMx->MCCAP_CLR = MCPWM_CAPCLR_CAP(captureChannel);
}

/*********************************************************************//**
 * @brief		Get current captured value in specified capture channel
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected,
 * 							Should be: LPC_MCPWM
 * @param[in]	captureChannel	Capture channel number, should be: 0..2
 * @return		None
 **********************************************************************/
uint32_t MCPWM_GetCapture(LPC_MCPWM_TypeDef *MCPWMx, uint32_t captureChannel)
{
	if (captureChannel == 0){
		return (MCPWMx->MCCR0);
	} else if (captureChannel == 1) {
		return (MCPWMx->MCCR1);
	} else if (captureChannel == 2) {
		return (MCPWMx->MCCR2);
	}
	return (0);
}


/*********************************************************************//**
 * @brief		Configures Count control in MCPWM peripheral
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected
 * 							Should be: LPC_MCPWM
 * @param[in]	channelNum	Channel number, should be: 0..2
 * @param[in]	countMode	Count mode, should be:
 * 							- ENABLE: Enables count mode.
 * 							- DISABLE: Disable count mode, the channel is in timer mode.
 * @param[in]	countConfig	Pointer to a MCPWM_COUNT_CFG_Type structure
*                    		that contains the configuration information for the
*                    		specified MCPWM count control.
 * @return		None
 **********************************************************************/
void MCPWM_CountConfig(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channelNum,
					uint32_t countMode, MCPWM_COUNT_CFG_Type *countConfig)
{
	if (channelNum <= 2) {
		if (countMode /* == ENABLE */){
			MCPWMx->MCCNTCON_SET = MCPWM_CNTCON_CNTR(channelNum);
			if (countConfig->countFalling /* == ENABLE */) {
				MCPWMx->MCCNTCON_SET = MCPWM_CNTCON_TCMCI_FE(countConfig->counterChannel,channelNum);
			} else {
				MCPWMx->MCCNTCON_CLR = MCPWM_CNTCON_TCMCI_FE(countConfig->counterChannel,channelNum);
			}
			if (countConfig->countRising /* == ENABLE */) {
				MCPWMx->MCCNTCON_SET = MCPWM_CNTCON_TCMCI_RE(countConfig->counterChannel,channelNum);
			} else {
				MCPWMx->MCCNTCON_CLR = MCPWM_CNTCON_TCMCI_RE(countConfig->counterChannel,channelNum);
			}
		} else {
			MCPWMx->MCCNTCON_CLR = MCPWM_CNTCON_CNTR(channelNum);
		}
	}
}


/*********************************************************************//**
 * @brief		Start MCPWM activity for each MCPWM channel
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected
 * 							Should be: LPC_MCPWM
 * @param[in]	channel0	State of this command on channel 0:
 * 							- ENABLE: 'Start' command will effect on channel 0
 * 							- DISABLE: 'Start' command will not effect on channel 0
 * @param[in]	channel1	State of this command on channel 1:
 * 							- ENABLE: 'Start' command will effect on channel 1
 * 							- DISABLE: 'Start' command will not effect on channel 1
 * @param[in]	channel2	State of this command on channel 2:
 * 							- ENABLE: 'Start' command will effect on channel 2
 * 							- DISABLE: 'Start' command will not effect on channel 2
 * @return		None
 **********************************************************************/
void MCPWM_Start(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channel0,
					uint32_t channel1, uint32_t channel2)
{
	uint32_t regVal = 0;
	regVal = (channel0 ? MCPWM_CON_RUN(0) : 0) | (channel1 ? MCPWM_CON_RUN(1) : 0) \
				| (channel2 ? MCPWM_CON_RUN(2) : 0);
	MCPWMx->MCCON_SET = regVal;
}


/*********************************************************************//**
 * @brief		Stop MCPWM activity for each MCPWM channel
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected
 * 							Should be: LPC_MCPWM
 * @param[in]	channel0	State of this command on channel 0:
 * 							- ENABLE: 'Stop' command will effect on channel 0
 * 							- DISABLE: 'Stop' command will not effect on channel 0
 * @param[in]	channel1	State of this command on channel 1:
 * 							- ENABLE: 'Stop' command will effect on channel 1
 * 							- DISABLE: 'Stop' command will not effect on channel 1
 * @param[in]	channel2	State of this command on channel 2:
 * 							- ENABLE: 'Stop' command will effect on channel 2
 * 							- DISABLE: 'Stop' command will not effect on channel 2
 * @return		None
 **********************************************************************/
void MCPWM_Stop(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channel0,
		uint32_t channel1, uint32_t channel2)
{
	uint32_t regVal = 0;
	regVal = (channel0 ? MCPWM_CON_RUN(0) : 0) | (channel1 ? MCPWM_CON_RUN(1) : 0) \
				| (channel2 ? MCPWM_CON_RUN(2) : 0);
	MCPWMx->MCCON_CLR = regVal;
}


/*********************************************************************//**
 * @brief		Enables/Disables 3-phase AC motor mode on MCPWM peripheral
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected
 * 							Should be: LPC_MCPWM
 * @param[in]	acMode		State of this command, should be:
 * 							- ENABLE.
 * 							- DISABLE.
 * @return		None
 **********************************************************************/
void MCPWM_ACMode(LPC_MCPWM_TypeDef *MCPWMx, uint32_t acMode)
{
	if (acMode){
		MCPWMx->MCCON_SET = MCPWM_CON_ACMODE;
	} else {
		MCPWMx->MCCON_CLR = MCPWM_CON_ACMODE;
	}
}


/*********************************************************************//**
 * @brief		Enables/Disables 3-phase DC motor mode on MCPWM peripheral
 * @param[in]	MCPWMx 			Motor Control PWM peripheral selected
 * 								Should be: LPC_MCPWM
 * @param[in]	dcMode			State of this command, should be:
 * 								- ENABLE.
 * 								- DISABLE.
 * @param[in]	outputInvered	Polarity of the MCOB outputs for all 3 channels,
 * 								should be:
 * 								- ENABLE: The MCOB outputs have opposite polarity
 * 									from the MCOA outputs.
 * 								- DISABLE: The MCOB outputs have the same basic
 * 									polarity as the MCOA outputs.
 * @param[in]	outputPattern	A value contains bits that enables/disables the specified
 * 								output pins route to the internal MCOA0 signal, should be:
								- MCPWM_PATENT_A0: 	 MCOA0 tracks internal MCOA0
								- MCPWM_PATENT_B0: 	 MCOB0 tracks internal MCOA0
								- MCPWM_PATENT_A1: 	 MCOA1 tracks internal MCOA0
								- MCPWM_PATENT_B1: 	 MCOB1 tracks internal MCOA0
								- MCPWM_PATENT_A2: 	 MCOA2 tracks internal MCOA0
								- MCPWM_PATENT_B2: 	 MCOB2 tracks internal MCOA0
 * @return		None
 *
 * Note: all these outputPatent values above can be ORed together for using as input parameter.
 **********************************************************************/
void MCPWM_DCMode(LPC_MCPWM_TypeDef *MCPWMx, uint32_t dcMode,
					uint32_t outputInvered, uint32_t outputPattern)
{
	if (dcMode){
		MCPWMx->MCCON_SET = MCPWM_CON_DCMODE;
	} else {
		MCPWMx->MCCON_CLR = MCPWM_CON_DCMODE;
	}

	if (outputInvered) {
		MCPWMx->MCCON_SET = MCPWM_CON_INVBDC;
	} else {
		MCPWMx->MCCON_CLR = MCPWM_CON_INVBDC;
	}

	MCPWMx->MCCCP = outputPattern;
}


/*********************************************************************//**
 * @brief		Configures the specified interrupt in MCPWM peripheral
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected
 * 							Should be: LPC_MCPWM
 * @param[in]	ulIntType	Interrupt type, should be:
 * 							- MCPWM_INTFLAG_LIM0: Limit interrupt for channel (0)
 * 							- MCPWM_INTFLAG_MAT0: Match interrupt for channel (0)
 * 							- MCPWM_INTFLAG_CAP0: Capture interrupt for channel (0)
 * 							- MCPWM_INTFLAG_LIM1: Limit interrupt for channel (1)
 * 							- MCPWM_INTFLAG_MAT1: Match interrupt for channel (1)
 * 							- MCPWM_INTFLAG_CAP1: Capture interrupt for channel (1)
 * 							- MCPWM_INTFLAG_LIM2: Limit interrupt for channel (2)
 * 							- MCPWM_INTFLAG_MAT2: Match interrupt for channel (2)
 * 							- MCPWM_INTFLAG_CAP2: Capture interrupt for channel (2)
 * 							- MCPWM_INTFLAG_ABORT: Fast abort interrupt
 * @param[in]	NewState	New State of this command, should be:
 * 							- ENABLE.
 * 							- DISABLE.
 * @return		None
 *
 * Note: all these ulIntType values above can be ORed together for using as input parameter.
 **********************************************************************/
void MCPWM_IntConfig(LPC_MCPWM_TypeDef *MCPWMx, uint32_t ulIntType, FunctionalState NewState)
{
	if (NewState) {
		MCPWMx->MCINTEN_SET = ulIntType;
	} else {
		MCPWMx->MCINTEN_CLR = ulIntType;
	}
}


/*********************************************************************//**
 * @brief		Sets/Forces the specified interrupt for MCPWM peripheral
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected
 * 							Should be LPC_MCPWM
 * @param[in]	ulIntType	Interrupt type, should be:
 * 							- MCPWM_INTFLAG_LIM0: Limit interrupt for channel (0)
 * 							- MCPWM_INTFLAG_MAT0: Match interrupt for channel (0)
 * 							- MCPWM_INTFLAG_CAP0: Capture interrupt for channel (0)
 * 							- MCPWM_INTFLAG_LIM1: Limit interrupt for channel (1)
 * 							- MCPWM_INTFLAG_MAT1: Match interrupt for channel (1)
 * 							- MCPWM_INTFLAG_CAP1: Capture interrupt for channel (1)
 * 							- MCPWM_INTFLAG_LIM2: Limit interrupt for channel (2)
 * 							- MCPWM_INTFLAG_MAT2: Match interrupt for channel (2)
 * 							- MCPWM_INTFLAG_CAP2: Capture interrupt for channel (2)
 * 							- MCPWM_INTFLAG_ABORT: Fast abort interrupt
 * @return		None
 * Note: all these ulIntType values above can be ORed together for using as input parameter.
 **********************************************************************/
void MCPWM_IntSet(LPC_MCPWM_TypeDef *MCPWMx, uint32_t ulIntType)
{
	MCPWMx->MCINTFLAG_SET = ulIntType;
}


/*********************************************************************//**
 * @brief		Clear the specified interrupt pending for MCPWM peripheral
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected,
 * 							should be: LPC_MCPWM
 * @param[in]	ulIntType	Interrupt type, should be:
 * 							- MCPWM_INTFLAG_LIM0: Limit interrupt for channel (0)
 * 							- MCPWM_INTFLAG_MAT0: Match interrupt for channel (0)
 * 							- MCPWM_INTFLAG_CAP0: Capture interrupt for channel (0)
 * 							- MCPWM_INTFLAG_LIM1: Limit interrupt for channel (1)
 * 							- MCPWM_INTFLAG_MAT1: Match interrupt for channel (1)
 * 							- MCPWM_INTFLAG_CAP1: Capture interrupt for channel (1)
 * 							- MCPWM_INTFLAG_LIM2: Limit interrupt for channel (2)
 * 							- MCPWM_INTFLAG_MAT2: Match interrupt for channel (2)
 * 							- MCPWM_INTFLAG_CAP2: Capture interrupt for channel (2)
 * 							- MCPWM_INTFLAG_ABORT: Fast abort interrupt
 * @return		None
 * Note: all these ulIntType values above can be ORed together for using as input parameter.
 **********************************************************************/
void MCPWM_IntClear(LPC_MCPWM_TypeDef *MCPWMx, uint32_t ulIntType)
{
	MCPWMx->MCINTFLAG_CLR = ulIntType;
}


/*********************************************************************//**
 * @brief		Check whether if the specified interrupt in MCPWM is set or not
 * @param[in]	MCPWMx 		Motor Control PWM peripheral selected,
 * 							should be: LPC_MCPWM
 * @param[in]	ulIntType	Interrupt type, should be:
 * 							- MCPWM_INTFLAG_LIM0: Limit interrupt for channel (0)
 * 							- MCPWM_INTFLAG_MAT0: Match interrupt for channel (0)
 * 							- MCPWM_INTFLAG_CAP0: Capture interrupt for channel (0)
 * 							- MCPWM_INTFLAG_LIM1: Limit interrupt for channel (1)
 * 							- MCPWM_INTFLAG_MAT1: Match interrupt for channel (1)
 * 							- MCPWM_INTFLAG_CAP1: Capture interrupt for channel (1)
 * 							- MCPWM_INTFLAG_LIM2: Limit interrupt for channel (2)
 * 							- MCPWM_INTFLAG_MAT2: Match interrupt for channel (2)
 * 							- MCPWM_INTFLAG_CAP2: Capture interrupt for channel (2)
 * 							- MCPWM_INTFLAG_ABORT: Fast abort interrupt
 * @return		None
 **********************************************************************/
FlagStatus MCPWM_GetIntStatus(LPC_MCPWM_TypeDef *MCPWMx, uint32_t ulIntType)
{
	return ((MCPWMx->MCINTFLAG & ulIntType) ? SET : RESET);
}

/**
 * @}
 */

#endif /* _MCPWM */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
