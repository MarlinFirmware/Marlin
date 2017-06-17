/**********************************************************************
* $Id$		lpc17xx_gpdma.c				2010-03-21
*//**
* @file		lpc17xx_gpdma.c
* @brief	Contains all functions support for GPDMA firmware
* 			library on LPC17xx
* @version	2.1
* @date		25. July. 2011
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
/** @addtogroup GPDMA
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_gpdma.h"
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

#ifdef _GPDMA


/* Private Variables ---------------------------------------------------------- */
/** @defgroup GPDMA_Private_Variables GPDMA Private Variables
 * @{
 */

/**
 * @brief Lookup Table of Connection Type matched with
 * Peripheral Data (FIFO) register base address
 */
//#ifdef __IAR_SYSTEMS_ICC__
volatile const void *GPDMA_LUTPerAddr[] = {
		(&LPC_SSP0->DR),				// SSP0 Tx
		(&LPC_SSP0->DR),				// SSP0 Rx
		(&LPC_SSP1->DR),				// SSP1 Tx
		(&LPC_SSP1->DR),				// SSP1 Rx
		(&LPC_ADC->ADGDR),			// ADC
		(&LPC_I2S->I2STXFIFO), 		// I2S Tx
		(&LPC_I2S->I2SRXFIFO), 		// I2S Rx
		(&LPC_DAC->DACR),				// DAC
		(&LPC_UART0->/*RBTHDLR.*/THR),	// UART0 Tx
		(&LPC_UART0->/*RBTHDLR.*/RBR),	// UART0 Rx
		(&LPC_UART1->/*RBTHDLR.*/THR),	// UART1 Tx
		(&LPC_UART1->/*RBTHDLR.*/RBR),	// UART1 Rx
		(&LPC_UART2->/*RBTHDLR.*/THR),	// UART2 Tx
		(&LPC_UART2->/*RBTHDLR.*/RBR),	// UART2 Rx
		(&LPC_UART3->/*RBTHDLR.*/THR),	// UART3 Tx
		(&LPC_UART3->/*RBTHDLR.*/RBR),	// UART3 Rx
		(&LPC_TIM0->MR0),				// MAT0.0
		(&LPC_TIM0->MR1),				// MAT0.1
		(&LPC_TIM1->MR0),				// MAT1.0
		(&LPC_TIM1->MR1),				// MAT1.1
		(&LPC_TIM2->MR0),				// MAT2.0
		(&LPC_TIM2->MR1),				// MAT2.1
		(&LPC_TIM3->MR0),				// MAT3.0
		(&LPC_TIM3->MR1)				// MAT3.1
};
//#else
//const uint32_t GPDMA_LUTPerAddr[] = {
//		((uint32_t)&LPC_SSP0->DR), 				// SSP0 Tx
//		((uint32_t)&LPC_SSP0->DR), 				// SSP0 Rx
//		((uint32_t)&LPC_SSP1->DR),				// SSP1 Tx
//		((uint32_t)&LPC_SSP1->DR),				// SSP1 Rx
//		((uint32_t)&LPC_ADC->ADGDR),			// ADC
//		((uint32_t)&LPC_I2S->I2STXFIFO), 		// I2S Tx
//		((uint32_t)&LPC_I2S->I2SRXFIFO), 		// I2S Rx
//		((uint32_t)&LPC_DAC->DACR),				// DAC
//		((uint32_t)&LPC_UART0->/*RBTHDLR.*/THR),	// UART0 Tx
//		((uint32_t)&LPC_UART0->/*RBTHDLR.*/RBR),	// UART0 Rx
//		((uint32_t)&LPC_UART1->/*RBTHDLR.*/THR),	// UART1 Tx
//		((uint32_t)&LPC_UART1->/*RBTHDLR.*/RBR),	// UART1 Rx
//		((uint32_t)&LPC_UART2->/*RBTHDLR.*/THR),	// UART2 Tx
//		((uint32_t)&LPC_UART2->/*RBTHDLR.*/RBR),	// UART2 Rx
//		((uint32_t)&LPC_UART3->/*RBTHDLR.*/THR),	// UART3 Tx
//		((uint32_t)&LPC_UART3->/*RBTHDLR.*/RBR),	// UART3 Rx
//		((uint32_t)&LPC_TIM0->MR0),				// MAT0.0
//		((uint32_t)&LPC_TIM0->MR1),				// MAT0.1
//		((uint32_t)&LPC_TIM1->MR0),				// MAT1.0
//		((uint32_t)&LPC_TIM1->MR1),				// MAT1.1
//		((uint32_t)&LPC_TIM2->MR0),				// MAT2.0
//		((uint32_t)&LPC_TIM2->MR1),				// MAT2.1
//		((uint32_t)&LPC_TIM3->MR0),				// MAT3.0
//		((uint32_t)&LPC_TIM3->MR1)				// MAT3.1
//};
//#endif
/**
 * @brief Lookup Table of GPDMA Channel Number matched with
 * GPDMA channel pointer
 */
const LPC_GPDMACH_TypeDef *pGPDMACh[8] = {
		LPC_GPDMACH0,	// GPDMA Channel 0
		LPC_GPDMACH1,	// GPDMA Channel 1
		LPC_GPDMACH2,	// GPDMA Channel 2
		LPC_GPDMACH3,	// GPDMA Channel 3
		LPC_GPDMACH4,	// GPDMA Channel 4
		LPC_GPDMACH5,	// GPDMA Channel 5
		LPC_GPDMACH6,	// GPDMA Channel 6
		LPC_GPDMACH7	// GPDMA Channel 7
};
/**
 * @brief Optimized Peripheral Source and Destination burst size
 */
const uint8_t GPDMA_LUTPerBurst[] = {
		GPDMA_BSIZE_4,				// SSP0 Tx
		GPDMA_BSIZE_4,				// SSP0 Rx
		GPDMA_BSIZE_4,				// SSP1 Tx
		GPDMA_BSIZE_4,				// SSP1 Rx
		GPDMA_BSIZE_1,				// ADC
		GPDMA_BSIZE_32, 			// I2S channel 0
		GPDMA_BSIZE_32, 			// I2S channel 1
		GPDMA_BSIZE_1,				// DAC
		GPDMA_BSIZE_1,				// UART0 Tx
		GPDMA_BSIZE_1,				// UART0 Rx
		GPDMA_BSIZE_1,				// UART1 Tx
		GPDMA_BSIZE_1,				// UART1 Rx
		GPDMA_BSIZE_1,				// UART2 Tx
		GPDMA_BSIZE_1,				// UART2 Rx
		GPDMA_BSIZE_1,				// UART3 Tx
		GPDMA_BSIZE_1,				// UART3 Rx
		GPDMA_BSIZE_1,				// MAT0.0
		GPDMA_BSIZE_1,				// MAT0.1
		GPDMA_BSIZE_1,				// MAT1.0
		GPDMA_BSIZE_1,				// MAT1.1
		GPDMA_BSIZE_1,				// MAT2.0
		GPDMA_BSIZE_1,				// MAT2.1
		GPDMA_BSIZE_1,				// MAT3.0
		GPDMA_BSIZE_1				// MAT3.1
};
/**
 * @brief Optimized Peripheral Source and Destination transfer width
 */
const uint8_t GPDMA_LUTPerWid[] = {
		GPDMA_WIDTH_BYTE,				// SSP0 Tx
		GPDMA_WIDTH_BYTE,				// SSP0 Rx
		GPDMA_WIDTH_BYTE,				// SSP1 Tx
		GPDMA_WIDTH_BYTE,				// SSP1 Rx
		GPDMA_WIDTH_WORD,				// ADC
		GPDMA_WIDTH_WORD, 				// I2S channel 0
		GPDMA_WIDTH_WORD, 				// I2S channel 1
		GPDMA_WIDTH_BYTE,				// DAC
		GPDMA_WIDTH_BYTE,				// UART0 Tx
		GPDMA_WIDTH_BYTE,				// UART0 Rx
		GPDMA_WIDTH_BYTE,				// UART1 Tx
		GPDMA_WIDTH_BYTE,				// UART1 Rx
		GPDMA_WIDTH_BYTE,				// UART2 Tx
		GPDMA_WIDTH_BYTE,				// UART2 Rx
		GPDMA_WIDTH_BYTE,				// UART3 Tx
		GPDMA_WIDTH_BYTE,				// UART3 Rx
		GPDMA_WIDTH_WORD,				// MAT0.0
		GPDMA_WIDTH_WORD,				// MAT0.1
		GPDMA_WIDTH_WORD,				// MAT1.0
		GPDMA_WIDTH_WORD,				// MAT1.1
		GPDMA_WIDTH_WORD,				// MAT2.0
		GPDMA_WIDTH_WORD,				// MAT2.1
		GPDMA_WIDTH_WORD,				// MAT3.0
		GPDMA_WIDTH_WORD				// MAT3.1
};

/**
 * @}
 */

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup GPDMA_Public_Functions
 * @{
 */

/********************************************************************//**
 * @brief 		Initialize GPDMA controller
 * @param 		None
 * @return 		None
 *********************************************************************/
void GPDMA_Init(void)
{
	/* Enable GPDMA clock */
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCGPDMA, ENABLE);

	// Reset all channel configuration register
	LPC_GPDMACH0->DMACCConfig = 0;
	LPC_GPDMACH1->DMACCConfig = 0;
	LPC_GPDMACH2->DMACCConfig = 0;
	LPC_GPDMACH3->DMACCConfig = 0;
	LPC_GPDMACH4->DMACCConfig = 0;
	LPC_GPDMACH5->DMACCConfig = 0;
	LPC_GPDMACH6->DMACCConfig = 0;
	LPC_GPDMACH7->DMACCConfig = 0;

	/* Clear all DMA interrupt and error flag */
	LPC_GPDMA->DMACIntTCClear = 0xFF;
	LPC_GPDMA->DMACIntErrClr = 0xFF;
}

/********************************************************************//**
 * @brief 		Setup GPDMA channel peripheral according to the specified
 *               parameters in the GPDMAChannelConfig.
 * @param[in]	GPDMAChannelConfig Pointer to a GPDMA_CH_CFG_Type
 * 									structure that contains the configuration
 * 									information for the specified GPDMA channel peripheral.
 * @return		ERROR if selected channel is enabled before
 * 				or SUCCESS if channel is configured successfully
 *********************************************************************/
Status GPDMA_Setup(GPDMA_Channel_CFG_Type *GPDMAChannelConfig)
{
	LPC_GPDMACH_TypeDef *pDMAch;
	uint32_t tmp1, tmp2;

	if (LPC_GPDMA->DMACEnbldChns & (GPDMA_DMACEnbldChns_Ch(GPDMAChannelConfig->ChannelNum))) {
		// This channel is enabled, return ERROR, need to release this channel first
		return ERROR;
	}

	// Get Channel pointer
	pDMAch = (LPC_GPDMACH_TypeDef *) pGPDMACh[GPDMAChannelConfig->ChannelNum];

	// Reset the Interrupt status
	LPC_GPDMA->DMACIntTCClear = GPDMA_DMACIntTCClear_Ch(GPDMAChannelConfig->ChannelNum);
	LPC_GPDMA->DMACIntErrClr = GPDMA_DMACIntErrClr_Ch(GPDMAChannelConfig->ChannelNum);

	// Clear DMA configure
	pDMAch->DMACCControl = 0x00;
	pDMAch->DMACCConfig = 0x00;

	/* Assign Linker List Item value */
	pDMAch->DMACCLLI = GPDMAChannelConfig->DMALLI;

	/* Set value to Channel Control Registers */
	switch (GPDMAChannelConfig->TransferType)
	{
	// Memory to memory
	case GPDMA_TRANSFERTYPE_M2M:
		// Assign physical source and destination address
		pDMAch->DMACCSrcAddr = GPDMAChannelConfig->SrcMemAddr;
		pDMAch->DMACCDestAddr = GPDMAChannelConfig->DstMemAddr;
		pDMAch->DMACCControl
				= GPDMA_DMACCxControl_TransferSize(GPDMAChannelConfig->TransferSize) \
						| GPDMA_DMACCxControl_SBSize(GPDMA_BSIZE_32) \
						| GPDMA_DMACCxControl_DBSize(GPDMA_BSIZE_32) \
						| GPDMA_DMACCxControl_SWidth(GPDMAChannelConfig->TransferWidth) \
						| GPDMA_DMACCxControl_DWidth(GPDMAChannelConfig->TransferWidth) \
						| GPDMA_DMACCxControl_SI \
						| GPDMA_DMACCxControl_DI \
						| GPDMA_DMACCxControl_I;
		break;
	// Memory to peripheral
	case GPDMA_TRANSFERTYPE_M2P:
		// Assign physical source
		pDMAch->DMACCSrcAddr = GPDMAChannelConfig->SrcMemAddr;
		// Assign peripheral destination address
		pDMAch->DMACCDestAddr = (uint32_t)GPDMA_LUTPerAddr[GPDMAChannelConfig->DstConn];
		pDMAch->DMACCControl
				= GPDMA_DMACCxControl_TransferSize((uint32_t)GPDMAChannelConfig->TransferSize) \
						| GPDMA_DMACCxControl_SBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_DBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_SWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_DWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_SI \
						| GPDMA_DMACCxControl_I;
		break;
	// Peripheral to memory
	case GPDMA_TRANSFERTYPE_P2M:
		// Assign peripheral source address
		pDMAch->DMACCSrcAddr = (uint32_t)GPDMA_LUTPerAddr[GPDMAChannelConfig->SrcConn];
		// Assign memory destination address
		pDMAch->DMACCDestAddr = GPDMAChannelConfig->DstMemAddr;
		pDMAch->DMACCControl
				= GPDMA_DMACCxControl_TransferSize((uint32_t)GPDMAChannelConfig->TransferSize) \
						| GPDMA_DMACCxControl_SBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_SWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DI \
						| GPDMA_DMACCxControl_I;
		break;
	// Peripheral to peripheral
	case GPDMA_TRANSFERTYPE_P2P:
		// Assign peripheral source address
		pDMAch->DMACCSrcAddr = (uint32_t)GPDMA_LUTPerAddr[GPDMAChannelConfig->SrcConn];
		// Assign peripheral destination address
		pDMAch->DMACCDestAddr = (uint32_t)GPDMA_LUTPerAddr[GPDMAChannelConfig->DstConn];
		pDMAch->DMACCControl
				= GPDMA_DMACCxControl_TransferSize((uint32_t)GPDMAChannelConfig->TransferSize) \
						| GPDMA_DMACCxControl_SBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DBSize((uint32_t)GPDMA_LUTPerBurst[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_SWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->SrcConn]) \
						| GPDMA_DMACCxControl_DWidth((uint32_t)GPDMA_LUTPerWid[GPDMAChannelConfig->DstConn]) \
						| GPDMA_DMACCxControl_I;
		break;
	// Do not support any more transfer type, return ERROR
	default:
		return ERROR;
	}

	/* Re-Configure DMA Request Select for source peripheral */
	if (GPDMAChannelConfig->SrcConn > 15)
	{
		LPC_SC->DMAREQSEL |= (1<<(GPDMAChannelConfig->SrcConn - 16));
	} else {
		LPC_SC->DMAREQSEL &= ~(1<<(GPDMAChannelConfig->SrcConn - 8));
	}

	/* Re-Configure DMA Request Select for Destination peripheral */
	if (GPDMAChannelConfig->DstConn > 15)
	{
		LPC_SC->DMAREQSEL |= (1<<(GPDMAChannelConfig->DstConn - 16));
	} else {
		LPC_SC->DMAREQSEL &= ~(1<<(GPDMAChannelConfig->DstConn - 8));
	}

	/* Enable DMA channels, little endian */
	LPC_GPDMA->DMACConfig = GPDMA_DMACConfig_E;
	while (!(LPC_GPDMA->DMACConfig & GPDMA_DMACConfig_E));

	// Calculate absolute value for Connection number
	tmp1 = GPDMAChannelConfig->SrcConn;
	tmp1 = ((tmp1 > 15) ? (tmp1 - 8) : tmp1);
	tmp2 = GPDMAChannelConfig->DstConn;
	tmp2 = ((tmp2 > 15) ? (tmp2 - 8) : tmp2);

	// Configure DMA Channel, enable Error Counter and Terminate counter
	pDMAch->DMACCConfig = GPDMA_DMACCxConfig_IE | GPDMA_DMACCxConfig_ITC /*| GPDMA_DMACCxConfig_E*/ \
		| GPDMA_DMACCxConfig_TransferType((uint32_t)GPDMAChannelConfig->TransferType) \
		| GPDMA_DMACCxConfig_SrcPeripheral(tmp1) \
		| GPDMA_DMACCxConfig_DestPeripheral(tmp2);

	return SUCCESS;
}


/*********************************************************************//**
 * @brief		Enable/Disable DMA channel
 * @param[in]	channelNum	GPDMA channel, should be in range from 0 to 7
 * @param[in]	NewState	New State of this command, should be:
 * 					- ENABLE.
 * 					- DISABLE.
 * @return		None
 **********************************************************************/
void GPDMA_ChannelCmd(uint8_t channelNum, FunctionalState NewState)
{
	LPC_GPDMACH_TypeDef *pDMAch;

	// Get Channel pointer
	pDMAch = (LPC_GPDMACH_TypeDef *) pGPDMACh[channelNum];

	if (NewState == ENABLE) {
		pDMAch->DMACCConfig |= GPDMA_DMACCxConfig_E;
	} else {
		pDMAch->DMACCConfig &= ~GPDMA_DMACCxConfig_E;
	}
}
/*********************************************************************//**
 * @brief		Check if corresponding channel does have an active interrupt
 * 				request or not
 * @param[in]	type		type of status, should be:
 * 					- GPDMA_STAT_INT: 		GPDMA Interrupt Status
 * 					- GPDMA_STAT_INTTC: 	GPDMA Interrupt Terminal Count Request Status
 * 					- GPDMA_STAT_INTERR:	GPDMA Interrupt Error Status
 * 					- GPDMA_STAT_RAWINTTC:	GPDMA Raw Interrupt Terminal Count Status
 * 					- GPDMA_STAT_RAWINTERR:	GPDMA Raw Error Interrupt Status
 * 					- GPDMA_STAT_ENABLED_CH:GPDMA Enabled Channel Status
 * @param[in]	channel		GPDMA channel, should be in range from 0 to 7
 * @return		IntStatus	status of DMA channel interrupt after masking
 * 				Should be:
 * 					- SET: the corresponding channel has no active interrupt request
 * 					- RESET: the corresponding channel does have an active interrupt request
 **********************************************************************/
IntStatus GPDMA_IntGetStatus(GPDMA_Status_Type type, uint8_t channel)
{
	CHECK_PARAM(PARAM_GPDMA_STAT(type));
	CHECK_PARAM(PARAM_GPDMA_CHANNEL(channel));

	switch (type)
	{
	case GPDMA_STAT_INT: //check status of DMA channel interrupts
		if (LPC_GPDMA->DMACIntStat & (GPDMA_DMACIntStat_Ch(channel)))
			return SET;
		return RESET;
	case GPDMA_STAT_INTTC: // check terminal count interrupt request status for DMA
		if (LPC_GPDMA->DMACIntTCStat & GPDMA_DMACIntTCStat_Ch(channel))
			return SET;
		return RESET;
	case GPDMA_STAT_INTERR: //check interrupt status for DMA channels
		if (LPC_GPDMA->DMACIntErrStat & GPDMA_DMACIntTCClear_Ch(channel))
			return SET;
		return RESET;
	case GPDMA_STAT_RAWINTTC: //check status of the terminal count interrupt for DMA channels
		if (LPC_GPDMA->DMACRawIntErrStat & GPDMA_DMACRawIntTCStat_Ch(channel))
			return SET;
		return RESET;
	case GPDMA_STAT_RAWINTERR: //check status of the error interrupt for DMA channels
		if (LPC_GPDMA->DMACRawIntTCStat & GPDMA_DMACRawIntErrStat_Ch(channel))
			return SET;
		return RESET;
	default: //check enable status for DMA channels
		if (LPC_GPDMA->DMACEnbldChns & GPDMA_DMACEnbldChns_Ch(channel))
			return SET;
		return RESET;
	}
}

/*********************************************************************//**
 * @brief		Clear one or more interrupt requests on DMA channels
 * @param[in]	type		type of interrupt request, should be:
 * 					- GPDMA_STATCLR_INTTC: 	GPDMA Interrupt Terminal Count Request Clear
 * 					- GPDMA_STATCLR_INTERR: GPDMA Interrupt Error Clear
 * @param[in]	channel		GPDMA channel, should be in range from 0 to 7
 * @return		None
 **********************************************************************/
void GPDMA_ClearIntPending(GPDMA_StateClear_Type type, uint8_t channel)
{
	CHECK_PARAM(PARAM_GPDMA_STATCLR(type));
	CHECK_PARAM(PARAM_GPDMA_CHANNEL(channel));

	if (type == GPDMA_STATCLR_INTTC) // clears the terminal count interrupt request on DMA channel
		LPC_GPDMA->DMACIntTCClear = GPDMA_DMACIntTCClear_Ch(channel);
	else // clear the error interrupt request
		LPC_GPDMA->DMACIntErrClr = GPDMA_DMACIntErrClr_Ch(channel);
}

/**
 * @}
 */

#endif /* _GPDMA */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

