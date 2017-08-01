/**********************************************************************
* $Id$		lpc17xx_qei.c				2010-05-21
*//**
* @file		lpc17xx_qei.c
* @brief	Contains all functions support for QEI firmware library on LPC17xx
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
/** @addtogroup QEI
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_qei.h"
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


#ifdef _QEI

/* Private Types -------------------------------------------------------------- */
/** @defgroup QEI_Private_Types QEI Private Types
 * @{
 */

/**
 * @brief QEI configuration union type definition
 */
typedef union {
	QEI_CFG_Type bmQEIConfig;
	uint32_t ulQEIConfig;
} QEI_CFGOPT_Type;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup QEI_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief		Resets value for each type of QEI value, such as velocity,
 * 				counter, position, etc..
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @param[in]	ulResetType		QEI Reset Type, should be one of the following:
 * 								- QEI_RESET_POS: Reset Position Counter
 * 								- QEI_RESET_POSOnIDX: Reset Position Counter on Index signal
 * 								- QEI_RESET_VEL: Reset Velocity
 * 								- QEI_RESET_IDX: Reset Index Counter
 * @return		None
 **********************************************************************/
void QEI_Reset(LPC_QEI_TypeDef *QEIx, uint32_t ulResetType)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	CHECK_PARAM(PARAM_QEI_RESET(ulResetType));

	QEIx->QEICON = ulResetType;
}

/*********************************************************************//**
 * @brief		Initializes the QEI peripheral according to the specified
*               parameters in the QEI_ConfigStruct.
 * @param[in]	QEIx				QEI peripheral, should be LPC_QEI
 * @param[in]	QEI_ConfigStruct	Pointer to a QEI_CFG_Type structure
*                    that contains the configuration information for the
*                    specified QEI peripheral
 * @return		None
 **********************************************************************/
void QEI_Init(LPC_QEI_TypeDef *QEIx, QEI_CFG_Type *QEI_ConfigStruct)
{

	CHECK_PARAM(PARAM_QEIx(QEIx));
	CHECK_PARAM(PARAM_QEI_DIRINV(QEI_ConfigStruct->DirectionInvert));
	CHECK_PARAM(PARAM_QEI_SIGNALMODE(QEI_ConfigStruct->SignalMode));
	CHECK_PARAM(PARAM_QEI_CAPMODE(QEI_ConfigStruct->CaptureMode));
	CHECK_PARAM(PARAM_QEI_INVINX(QEI_ConfigStruct->InvertIndex));

	/* Set up clock and power for QEI module */
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCQEI, ENABLE);

	/* As default, peripheral clock for QEI module
	 * is set to FCCLK / 2 */
	CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_QEI, CLKPWR_PCLKSEL_CCLK_DIV_1);

	// Reset all remaining value in QEI peripheral
	QEIx->QEICON = QEI_CON_RESP | QEI_CON_RESV | QEI_CON_RESI;
	QEIx->QEIMAXPOS = 0x00;
	QEIx->CMPOS0 = 0x00;
	QEIx->CMPOS1 = 0x00;
	QEIx->CMPOS2 = 0x00;
	QEIx->INXCMP = 0x00;
	QEIx->QEILOAD = 0x00;
	QEIx->VELCOMP = 0x00;
	QEIx->FILTER = 0x00;
	// Disable all Interrupt
	QEIx->QEIIEC = QEI_IECLR_BITMASK;
	// Clear all Interrupt pending
	QEIx->QEICLR = QEI_INTCLR_BITMASK;
	// Set QEI configuration value corresponding to its setting up value
	QEIx->QEICONF = ((QEI_CFGOPT_Type *)QEI_ConfigStruct)->ulQEIConfig;
}


/*********************************************************************//**
 * @brief		De-initializes the QEI peripheral registers to their
*                  default reset values.
 * @param[in]	QEIx	QEI peripheral, should be LPC_QEI
 * @return 		None
 **********************************************************************/
void QEI_DeInit(LPC_QEI_TypeDef *QEIx)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));

	/* Turn off clock and power for QEI module */
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCQEI, DISABLE);
}


/*****************************************************************************//**
* @brief		Fills each QIE_InitStruct member with its default value:
* 				- DirectionInvert = QEI_DIRINV_NONE
* 				- SignalMode = QEI_SIGNALMODE_QUAD
* 				- CaptureMode = QEI_CAPMODE_4X
* 				- InvertIndex = QEI_INVINX_NONE
* @param[in]	QIE_InitStruct Pointer to a QEI_CFG_Type structure
*                    which will be initialized.
* @return		None
*******************************************************************************/
void QEI_ConfigStructInit(QEI_CFG_Type *QIE_InitStruct)
{
	QIE_InitStruct->CaptureMode = QEI_CAPMODE_4X;
	QIE_InitStruct->DirectionInvert = QEI_DIRINV_NONE;
	QIE_InitStruct->InvertIndex = QEI_INVINX_NONE;
	QIE_InitStruct->SignalMode = QEI_SIGNALMODE_QUAD;
}


/*********************************************************************//**
 * @brief		Check whether if specified flag status is set or not
 * @param[in]	QEIx		QEI peripheral, should be LPC_QEI
 * @param[in]	ulFlagType	Status Flag Type, should be one of the following:
 * 							- QEI_STATUS_DIR: Direction Status
 * @return		New Status of this status flag (SET or RESET)
 **********************************************************************/
FlagStatus QEI_GetStatus(LPC_QEI_TypeDef *QEIx, uint32_t ulFlagType)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	CHECK_PARAM(PARAM_QEI_STATUS(ulFlagType));
	return ((QEIx->QEISTAT & ulFlagType) ? SET : RESET);
}

/*********************************************************************//**
 * @brief		Get current position value in QEI peripheral
 * @param[in]	QEIx	QEI peripheral, should be LPC_QEI
 * @return		Current position value of QEI peripheral
 **********************************************************************/
uint32_t QEI_GetPosition(LPC_QEI_TypeDef *QEIx)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	return (QEIx->QEIPOS);
}

/*********************************************************************//**
 * @brief		Set max position value for QEI peripheral
 * @param[in]	QEIx		QEI peripheral, should be LPC_QEI
 * @param[in]	ulMaxPos	Max position value to set
 * @return		None
 **********************************************************************/
void QEI_SetMaxPosition(LPC_QEI_TypeDef *QEIx, uint32_t ulMaxPos)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	QEIx->QEIMAXPOS = ulMaxPos;
}

/*********************************************************************//**
 * @brief		Set position compare value for QEI peripheral
 * @param[in]	QEIx		QEI peripheral, should be LPC_QEI
 * @param[in]	bPosCompCh	Compare Position channel, should be:
 * 							- QEI_COMPPOS_CH_0: QEI compare position channel 0
 * 							- QEI_COMPPOS_CH_1: QEI compare position channel 1
 * 							- QEI_COMPPOS_CH_2: QEI compare position channel 2
 * @param[in]	ulPosComp	Compare Position value to set
 * @return		None
 **********************************************************************/
void QEI_SetPositionComp(LPC_QEI_TypeDef *QEIx, uint8_t bPosCompCh, uint32_t ulPosComp)
{
	uint32_t *tmp;

	CHECK_PARAM(PARAM_QEIx(QEIx));
	CHECK_PARAM(PARAM_QEI_COMPPOS_CH(bPosCompCh));
	tmp = (uint32_t *) (&(QEIx->CMPOS0) + bPosCompCh * 4);
	*tmp = ulPosComp;

}

/*********************************************************************//**
 * @brief		Get current index counter of QEI peripheral
 * @param[in]	QEIx		QEI peripheral, should be LPC_QEI
 * @return		Current value of QEI index counter
 **********************************************************************/
uint32_t QEI_GetIndex(LPC_QEI_TypeDef *QEIx)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	return (QEIx->INXCNT);
}

/*********************************************************************//**
 * @brief		Set value for index compare in QEI peripheral
 * @param[in]	QEIx		QEI peripheral, should be LPC_QEI
 * @param[in]	ulIndexComp		Compare Index Value to set
 * @return		None
 **********************************************************************/
void QEI_SetIndexComp(LPC_QEI_TypeDef *QEIx, uint32_t ulIndexComp)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	QEIx->INXCMP = ulIndexComp;
}

/*********************************************************************//**
 * @brief		Set timer reload value for QEI peripheral. When the velocity timer is
 * 				over-flow, the value that set for Timer Reload register will be loaded
 * 				into the velocity timer for next period. The calculated velocity in RPM
 * 				therefore will be affect by this value.
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @param[in]	QEIReloadStruct	QEI reload structure
 * @return		None
 **********************************************************************/
void QEI_SetTimerReload(LPC_QEI_TypeDef *QEIx, QEI_RELOADCFG_Type *QEIReloadStruct)
{
	uint64_t pclk;

	CHECK_PARAM(PARAM_QEIx(QEIx));
	CHECK_PARAM(PARAM_QEI_TIMERRELOAD(QEIReloadStruct->ReloadOption));

	if (QEIReloadStruct->ReloadOption == QEI_TIMERRELOAD_TICKVAL) {
		QEIx->QEILOAD = QEIReloadStruct->ReloadValue - 1;
	} else {
		pclk = (uint64_t)CLKPWR_GetPCLK(CLKPWR_PCLKSEL_QEI);
		pclk = (pclk /(1000000/QEIReloadStruct->ReloadValue)) - 1;
		QEIx->QEILOAD = (uint32_t)pclk;
	}
}

/*********************************************************************//**
 * @brief		Get current timer counter in QEI peripheral
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @return		Current timer counter in QEI peripheral
 **********************************************************************/
uint32_t QEI_GetTimer(LPC_QEI_TypeDef *QEIx)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	return (QEIx->QEITIME);
}

/*********************************************************************//**
 * @brief		Get current velocity pulse counter in current time period
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @return		Current velocity pulse counter value
 **********************************************************************/
uint32_t QEI_GetVelocity(LPC_QEI_TypeDef *QEIx)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	return (QEIx->QEIVEL);
}

/*********************************************************************//**
 * @brief		Get the most recently measured velocity of the QEI. When
 * 				the Velocity timer in QEI is over-flow, the current velocity
 * 				value will be loaded into Velocity Capture register.
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @return		The most recently measured velocity value
 **********************************************************************/
uint32_t QEI_GetVelocityCap(LPC_QEI_TypeDef *QEIx)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	return (QEIx->QEICAP);
}

/*********************************************************************//**
 * @brief		Set Velocity Compare value for QEI peripheral
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @param[in]	ulVelComp		Compare Velocity value to set
 * @return		None
 **********************************************************************/
void QEI_SetVelocityComp(LPC_QEI_TypeDef *QEIx, uint32_t ulVelComp)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	QEIx->VELCOMP = ulVelComp;
}

/*********************************************************************//**
 * @brief		Set value of sampling count for the digital filter in
 * 				QEI peripheral
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @param[in]	ulSamplingPulse	Value of sampling count to set
 * @return		None
 **********************************************************************/
void QEI_SetDigiFilter(LPC_QEI_TypeDef *QEIx, uint32_t ulSamplingPulse)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	QEIx->FILTER = ulSamplingPulse;
}

/*********************************************************************//**
 * @brief		Check whether if specified interrupt flag status in QEI
 * 				peripheral is set or not
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @param[in]	ulIntType		Interrupt Flag Status type, should be:
								- QEI_INTFLAG_INX_Int: index pulse was detected interrupt
								- QEI_INTFLAG_TIM_Int: Velocity timer over flow interrupt
								- QEI_INTFLAG_VELC_Int: Capture velocity is less than compare interrupt
								- QEI_INTFLAG_DIR_Int: Change of direction interrupt
								- QEI_INTFLAG_ERR_Int: An encoder phase error interrupt
								- QEI_INTFLAG_ENCLK_Int: An encoder clock pulse was detected interrupt
								- QEI_INTFLAG_POS0_Int: position 0 compare value is equal to the
														current position interrupt
								- QEI_INTFLAG_POS1_Int: position 1 compare value is equal to the
														current position interrupt
								- QEI_INTFLAG_POS2_Int: position 2 compare value is equal to the
														current position interrupt
								- QEI_INTFLAG_REV_Int: Index compare value is equal to the current
														index count interrupt
								- QEI_INTFLAG_POS0REV_Int: Combined position 0 and revolution count interrupt
								- QEI_INTFLAG_POS1REV_Int: Combined position 1 and revolution count interrupt
								- QEI_INTFLAG_POS2REV_Int: Combined position 2 and revolution count interrupt
 * @return		New State of specified interrupt flag status (SET or RESET)
 **********************************************************************/
FlagStatus QEI_GetIntStatus(LPC_QEI_TypeDef *QEIx, uint32_t ulIntType)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	CHECK_PARAM(PARAM_QEI_INTFLAG(ulIntType));

	return((QEIx->QEIINTSTAT & ulIntType) ? SET : RESET);
}

/*********************************************************************//**
 * @brief		Enable/Disable specified interrupt in QEI peripheral
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @param[in]	ulIntType		Interrupt Flag Status type, should be:
 * 								- QEI_INTFLAG_INX_Int: index pulse was detected interrupt
 *								- QEI_INTFLAG_TIM_Int: Velocity timer over flow interrupt
 *								- QEI_INTFLAG_VELC_Int: Capture velocity is less than compare interrupt
 * 								- QEI_INTFLAG_DIR_Int: Change of direction interrupt
 *  							- QEI_INTFLAG_ERR_Int: An encoder phase error interrupt
 * 								- QEI_INTFLAG_ENCLK_Int: An encoder clock pulse was detected interrupt
 *								- QEI_INTFLAG_POS0_Int: position 0 compare value is equal to the
 *														current position interrupt
 *								- QEI_INTFLAG_POS1_Int: position 1 compare value is equal to the
 *														current position interrupt
 *								- QEI_INTFLAG_POS2_Int: position 2 compare value is equal to the
 *														current position interrupt
 *								- QEI_INTFLAG_REV_Int: Index compare value is equal to the current
 *														index count interrupt
 *								- QEI_INTFLAG_POS0REV_Int: Combined position 0 and revolution count interrupt
 *								- QEI_INTFLAG_POS1REV_Int: Combined position 1 and revolution count interrupt
 *								- QEI_INTFLAG_POS2REV_Int: Combined position 2 and revolution count interrupt
 * @param[in]	NewState		New function state, should be:
 *								- DISABLE
 *								- ENABLE
 * @return		None
 **********************************************************************/
void QEI_IntCmd(LPC_QEI_TypeDef *QEIx, uint32_t ulIntType, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	CHECK_PARAM(PARAM_QEI_INTFLAG(ulIntType));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE) {
		QEIx->QEIIES = ulIntType;
	} else {
		QEIx->QEIIEC = ulIntType;
	}
}


/*********************************************************************//**
 * @brief		Sets (forces) specified interrupt in QEI peripheral
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @param[in]	ulIntType		Interrupt Flag Status type, should be:
								- QEI_INTFLAG_INX_Int: index pulse was detected interrupt
								- QEI_INTFLAG_TIM_Int: Velocity timer over flow interrupt
								- QEI_INTFLAG_VELC_Int: Capture velocity is less than compare interrupt
								- QEI_INTFLAG_DIR_Int: Change of direction interrupt
								- QEI_INTFLAG_ERR_Int: An encoder phase error interrupt
								- QEI_INTFLAG_ENCLK_Int: An encoder clock pulse was detected interrupt
								- QEI_INTFLAG_POS0_Int: position 0 compare value is equal to the
														current position interrupt
								- QEI_INTFLAG_POS1_Int: position 1 compare value is equal to the
														current position interrupt
								- QEI_INTFLAG_POS2_Int: position 2 compare value is equal to the
														current position interrupt
								- QEI_INTFLAG_REV_Int: Index compare value is equal to the current
														index count interrupt
								- QEI_INTFLAG_POS0REV_Int: Combined position 0 and revolution count interrupt
								- QEI_INTFLAG_POS1REV_Int: Combined position 1 and revolution count interrupt
								- QEI_INTFLAG_POS2REV_Int: Combined position 2 and revolution count interrupt
 * @return		None
 **********************************************************************/
void QEI_IntSet(LPC_QEI_TypeDef *QEIx, uint32_t ulIntType)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	CHECK_PARAM(PARAM_QEI_INTFLAG(ulIntType));

	QEIx->QEISET = ulIntType;
}

/*********************************************************************//**
 * @brief		Clear (force) specified interrupt (pending) in QEI peripheral
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @param[in]	ulIntType		Interrupt Flag Status type, should be:
								- QEI_INTFLAG_INX_Int: index pulse was detected interrupt
								- QEI_INTFLAG_TIM_Int: Velocity timer over flow interrupt
								- QEI_INTFLAG_VELC_Int: Capture velocity is less than compare interrupt
								- QEI_INTFLAG_DIR_Int: Change of direction interrupt
								- QEI_INTFLAG_ERR_Int: An encoder phase error interrupt
								- QEI_INTFLAG_ENCLK_Int: An encoder clock pulse was detected interrupt
								- QEI_INTFLAG_POS0_Int: position 0 compare value is equal to the
														current position interrupt
								- QEI_INTFLAG_POS1_Int: position 1 compare value is equal to the
														current position interrupt
								- QEI_INTFLAG_POS2_Int: position 2 compare value is equal to the
														current position interrupt
								- QEI_INTFLAG_REV_Int: Index compare value is equal to the current
														index count interrupt
								- QEI_INTFLAG_POS0REV_Int: Combined position 0 and revolution count interrupt
								- QEI_INTFLAG_POS1REV_Int: Combined position 1 and revolution count interrupt
								- QEI_INTFLAG_POS2REV_Int: Combined position 2 and revolution count interrupt
 * @return		None
 **********************************************************************/
void QEI_IntClear(LPC_QEI_TypeDef *QEIx, uint32_t ulIntType)
{
	CHECK_PARAM(PARAM_QEIx(QEIx));
	CHECK_PARAM(PARAM_QEI_INTFLAG(ulIntType));

	QEIx->QEICLR = ulIntType;
}


/*********************************************************************//**
 * @brief		Calculates the actual velocity in RPM passed via velocity
 * 				capture value and Pulse Per Round (of the encoder) value
 * 				parameter input.
 * @param[in]	QEIx			QEI peripheral, should be LPC_QEI
 * @param[in]	ulVelCapValue	Velocity capture input value that can
 * 								be got from QEI_GetVelocityCap() function
 * @param[in]	ulPPR			Pulse per round of encoder
 * @return		The actual value of velocity in RPM (Round per minute)
 **********************************************************************/
uint32_t QEI_CalculateRPM(LPC_QEI_TypeDef *QEIx, uint32_t ulVelCapValue, uint32_t ulPPR)
{
	uint64_t rpm, clock, Load, edges;

	// Get current Clock rate for timer input
	clock = (uint64_t)CLKPWR_GetPCLK(CLKPWR_PCLKSEL_QEI);
	// Get Timer load value (velocity capture period)
	Load  = (uint64_t)(QEIx->QEILOAD + 1);
	// Get Edge
	edges = (uint64_t)((QEIx->QEICONF & QEI_CONF_CAPMODE) ? 4 : 2);
	// Calculate RPM
	rpm = ((clock * ulVelCapValue * 60) / (Load * ulPPR * edges));

	return (uint32_t)(rpm);
}


/**
 * @}
 */

#endif /* _QEI */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

