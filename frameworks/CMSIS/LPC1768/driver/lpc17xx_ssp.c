/**********************************************************************
* $Id$		lpc17xx_ssp.c				2010-06-18
*//**
* @file		lpc17xx_ssp.c
* @brief	Contains all functions support for SSP firmware library on LPC17xx
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
/** @addtogroup SSP
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_ssp.h"
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


#ifdef _SSP

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup SSP_Public_Functions
 * @{
 */
static void setSSPclock (LPC_SSP_TypeDef *SSPx, uint32_t target_clock);

/*********************************************************************//**
 * @brief 		Setup clock rate for SSP device
 * @param[in] 	SSPx	SSP peripheral definition, should be:
 * 						- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	target_clock : clock of SSP (Hz)
 * @return 		None
 ***********************************************************************/
static void setSSPclock (LPC_SSP_TypeDef *SSPx, uint32_t target_clock)
{
    uint32_t prescale, cr0_div, cmp_clk, ssp_clk;

    CHECK_PARAM(PARAM_SSPx(SSPx));

    /* The SSP clock is derived from the (main system oscillator / 2),
       so compute the best divider from that clock */
    if (SSPx == LPC_SSP0){
    	ssp_clk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_SSP0);
    } else if (SSPx == LPC_SSP1) {
    	ssp_clk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_SSP1);
    } else {
    	return;
    }

	/* Find closest divider to get at or under the target frequency.
	   Use smallest prescale possible and rely on the divider to get
	   the closest target frequency */
	cr0_div = 0;
	cmp_clk = 0xFFFFFFFF;
	prescale = 2;
	while (cmp_clk > target_clock)
	{
		cmp_clk = ssp_clk / ((cr0_div + 1) * prescale);
		if (cmp_clk > target_clock)
		{
			cr0_div++;
			if (cr0_div > 0xFF)
			{
				cr0_div = 0;
				prescale += 2;
			}
		}
	}

    /* Write computed prescaler and divider back to register */
    SSPx->CR0 &= (~SSP_CR0_SCR(0xFF)) & SSP_CR0_BITMASK;
    SSPx->CR0 |= (SSP_CR0_SCR(cr0_div)) & SSP_CR0_BITMASK;
    SSPx->CPSR = prescale & SSP_CPSR_BITMASK;
}

/**
 * @}
 */

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup SSP_Public_Functions
 * @{
 */

/********************************************************************//**
 * @brief		Initializes the SSPx peripheral according to the specified
*               parameters in the SSP_ConfigStruct.
 * @param[in]	SSPx	SSP peripheral selected, should be:
 * 				 		- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	SSP_ConfigStruct Pointer to a SSP_CFG_Type structure
*                    that contains the configuration information for the
*                    specified SSP peripheral.
 * @return 		None
 *********************************************************************/
void SSP_Init(LPC_SSP_TypeDef *SSPx, SSP_CFG_Type *SSP_ConfigStruct)
{
	uint32_t tmp;

	CHECK_PARAM(PARAM_SSPx(SSPx));

	if(SSPx == LPC_SSP0) {
		/* Set up clock and power for SSP0 module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSSP0, ENABLE);
	} else if(SSPx == LPC_SSP1) {
		/* Set up clock and power for SSP1 module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSSP1, ENABLE);
	} else {
		return;
	}

	/* Configure SSP, interrupt is disable, LoopBack mode is disable,
	 * SSP is disable, Slave output is disable as default
	 */
	tmp = ((SSP_ConfigStruct->CPHA) | (SSP_ConfigStruct->CPOL) \
		| (SSP_ConfigStruct->FrameFormat) | (SSP_ConfigStruct->Databit))
		& SSP_CR0_BITMASK;
	// write back to SSP control register
	SSPx->CR0 = tmp;

	tmp = SSP_ConfigStruct->Mode & SSP_CR1_BITMASK;
	// Write back to CR1
	SSPx->CR1 = tmp;

	// Set clock rate for SSP peripheral
	setSSPclock(SSPx, SSP_ConfigStruct->ClockRate);
}

/*********************************************************************//**
 * @brief		De-initializes the SSPx peripheral registers to their
*                  default reset values.
 * @param[in]	SSPx	SSP peripheral selected, should be:
 * 				 		- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @return 		None
 **********************************************************************/
void SSP_DeInit(LPC_SSP_TypeDef* SSPx)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));

	if (SSPx == LPC_SSP0){
		/* Set up clock and power for SSP0 module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSSP0, DISABLE);
	} else if (SSPx == LPC_SSP1) {
		/* Set up clock and power for SSP1 module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSSP1, DISABLE);
	}
}

/*****************************************************************************//**
* @brief		Get data size bit selected
* @param[in]	SSPx pointer to LPC_SSP_TypeDef structure, should be:
* 				- LPC_SSP0: SSP0 peripheral
* 				- LPC_SSP1: SSP1 peripheral
* @return		Data size, could be:
*				- SSP_DATABIT_4: 4 bit transfer
*				- SSP_DATABIT_5: 5 bit transfer
*				...
*				- SSP_DATABIT_16: 16 bit transfer
*******************************************************************************/
uint8_t SSP_GetDataSize(LPC_SSP_TypeDef* SSPx)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	return (SSPx->CR0 & (0xF));
}

/*****************************************************************************//**
* @brief		Fills each SSP_InitStruct member with its default value:
* 				- CPHA = SSP_CPHA_FIRST
* 				- CPOL = SSP_CPOL_HI
* 				- ClockRate = 1000000
* 				- Databit = SSP_DATABIT_8
* 				- Mode = SSP_MASTER_MODE
* 				- FrameFormat = SSP_FRAME_SSP
* @param[in]	SSP_InitStruct Pointer to a SSP_CFG_Type structure
*                    which will be initialized.
* @return		None
*******************************************************************************/
void SSP_ConfigStructInit(SSP_CFG_Type *SSP_InitStruct)
{
	SSP_InitStruct->CPHA = SSP_CPHA_FIRST;
	SSP_InitStruct->CPOL = SSP_CPOL_HI;
	SSP_InitStruct->ClockRate = 1000000;
	SSP_InitStruct->Databit = SSP_DATABIT_8;
	SSP_InitStruct->Mode = SSP_MASTER_MODE;
	SSP_InitStruct->FrameFormat = SSP_FRAME_SPI;
}


/*********************************************************************//**
 * @brief		Enable or disable SSP peripheral's operation
 * @param[in]	SSPx	SSP peripheral, should be:
 * 				- LPC_SSP0: SSP0 peripheral
 * 				- LPC_SSP1: SSP1 peripheral
 * @param[in]	NewState New State of SSPx peripheral's operation
 * @return 		none
 **********************************************************************/
void SSP_Cmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SSPx->CR1 |= SSP_CR1_SSP_EN;
	}
	else
	{
		SSPx->CR1 &= (~SSP_CR1_SSP_EN) & SSP_CR1_BITMASK;
	}
}

/*********************************************************************//**
 * @brief		Enable or disable Loop Back mode function in SSP peripheral
 * @param[in]	SSPx	SSP peripheral selected, should be:
 *  					- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	NewState	New State of Loop Back mode, should be:
 * 							- ENABLE: Enable this function
 * 							- DISABLE: Disable this function
 * @return 		None
 **********************************************************************/
void SSP_LoopBackCmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SSPx->CR1 |= SSP_CR1_LBM_EN;
	}
	else
	{
		SSPx->CR1 &= (~SSP_CR1_LBM_EN) & SSP_CR1_BITMASK;
	}
}

/*********************************************************************//**
 * @brief		Enable or disable Slave Output function in SSP peripheral
 * @param[in]	SSPx	SSP peripheral selected, should be:
 * 						- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	NewState	New State of Slave Output function, should be:
 * 							- ENABLE: Slave Output in normal operation
 * 							- DISABLE: Slave Output is disabled. This blocks
 * 							SSP controller from driving the transmit data
 * 							line (MISO)
 * Note: 		This function is available when SSP peripheral in Slave mode
 * @return 		None
 **********************************************************************/
void SSP_SlaveOutputCmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SSPx->CR1 &= (~SSP_CR1_SO_DISABLE) & SSP_CR1_BITMASK;
	}
	else
	{
		SSPx->CR1 |= SSP_CR1_SO_DISABLE;
	}
}



/*********************************************************************//**
 * @brief		Transmit a single data through SSPx peripheral
 * @param[in]	SSPx	SSP peripheral selected, should be:
 * 						- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	Data	Data to transmit (must be 16 or 8-bit long,
 * 						this depend on SSP data bit number configured)
 * @return 		none
 **********************************************************************/
void SSP_SendData(LPC_SSP_TypeDef* SSPx, uint16_t Data)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));

	SSPx->DR = SSP_DR_BITMASK(Data);
}



/*********************************************************************//**
 * @brief		Receive a single data from SSPx peripheral
 * @param[in]	SSPx	SSP peripheral selected, should be
 * 						- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @return 		Data received (16-bit long)
 **********************************************************************/
uint16_t SSP_ReceiveData(LPC_SSP_TypeDef* SSPx)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));

	return ((uint16_t) (SSP_DR_BITMASK(SSPx->DR)));
}

/*********************************************************************//**
 * @brief 		SSP Read write data function
 * @param[in]	SSPx 	Pointer to SSP peripheral, should be
 * 						- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	dataCfg	Pointer to a SSP_DATA_SETUP_Type structure that
 * 						contains specified information about transmit
 * 						data configuration.
 * @param[in]	xfType	Transfer type, should be:
 * 						- SSP_TRANSFER_POLLING: Polling mode
 * 						- SSP_TRANSFER_INTERRUPT: Interrupt mode
 * @return 		Actual Data length has been transferred in polling mode.
 * 				In interrupt mode, always return (0)
 * 				Return (-1) if error.
 * Note: This function can be used in both master and slave mode.
 ***********************************************************************/
int32_t SSP_ReadWrite (LPC_SSP_TypeDef *SSPx, SSP_DATA_SETUP_Type *dataCfg, \
						SSP_TRANSFER_Type xfType)
{
	uint8_t *rdata8;
    uint8_t *wdata8;
	uint16_t *rdata16;
    uint16_t *wdata16;
    uint32_t stat;
    uint32_t tmp;
    int32_t dataword;

    dataCfg->rx_cnt = 0;
    dataCfg->tx_cnt = 0;
    dataCfg->status = 0;


	/* Clear all remaining data in RX FIFO */
	while (SSPx->SR & SSP_SR_RNE){
		tmp = (uint32_t) SSP_ReceiveData(SSPx);
	}

	// Clear status
	SSPx->ICR = SSP_ICR_BITMASK;
	if(SSP_GetDataSize(SSPx)>SSP_DATABIT_8)
		dataword = 1;
	else dataword = 0;

	// Polling mode ----------------------------------------------------------------------
	if (xfType == SSP_TRANSFER_POLLING){
		if (dataword == 0){
			rdata8 = (uint8_t *)dataCfg->rx_data;
			wdata8 = (uint8_t *)dataCfg->tx_data;
		} else {
			rdata16 = (uint16_t *)dataCfg->rx_data;
			wdata16 = (uint16_t *)dataCfg->tx_data;
		}
		while ((dataCfg->tx_cnt < dataCfg->length) || (dataCfg->rx_cnt < dataCfg->length)){
			if ((SSPx->SR & SSP_SR_TNF) && (dataCfg->tx_cnt < dataCfg->length)){
				// Write data to buffer
				if(dataCfg->tx_data == NULL){
					if (dataword == 0){
						SSP_SendData(SSPx, 0xFF);
						dataCfg->tx_cnt++;
					} else {
						SSP_SendData(SSPx, 0xFFFF);
						dataCfg->tx_cnt += 2;
					}
				} else {
					if (dataword == 0){
						SSP_SendData(SSPx, *wdata8);
						wdata8++;
						dataCfg->tx_cnt++;
					} else {
						SSP_SendData(SSPx, *wdata16);
						wdata16++;
						dataCfg->tx_cnt += 2;
					}
				}
			}

			// Check overrun error
			if ((stat = SSPx->RIS) & SSP_RIS_ROR){
				// save status and return
				dataCfg->status = stat | SSP_STAT_ERROR;
				return (-1);
			}

			// Check for any data available in RX FIFO
			while ((SSPx->SR & SSP_SR_RNE) && (dataCfg->rx_cnt < dataCfg->length)){
				// Read data from SSP data
				tmp = SSP_ReceiveData(SSPx);

				// Store data to destination
				if (dataCfg->rx_data != NULL)
				{
					if (dataword == 0){
						*(rdata8) = (uint8_t) tmp;
						rdata8++;
					} else {
						*(rdata16) = (uint16_t) tmp;
						rdata16++;
					}
				}
				// Increase counter
				if (dataword == 0){
					dataCfg->rx_cnt++;
				} else {
					dataCfg->rx_cnt += 2;
				}
			}
		}

		// save status
		dataCfg->status = SSP_STAT_DONE;

		if (dataCfg->tx_data != NULL){
			return dataCfg->tx_cnt;
		} else if (dataCfg->rx_data != NULL){
			return dataCfg->rx_cnt;
		} else {
			return (0);
		}
	}

	// Interrupt mode ----------------------------------------------------------------------
	else if (xfType == SSP_TRANSFER_INTERRUPT){

		while ((SSPx->SR & SSP_SR_TNF) && (dataCfg->tx_cnt < dataCfg->length)){
			// Write data to buffer
			if(dataCfg->tx_data == NULL){
				if (dataword == 0){
					SSP_SendData(SSPx, 0xFF);
					dataCfg->tx_cnt++;
				} else {
					SSP_SendData(SSPx, 0xFFFF);
					dataCfg->tx_cnt += 2;
				}
			} else {
				if (dataword == 0){
					SSP_SendData(SSPx, (*(uint8_t *)((uint32_t)dataCfg->tx_data + dataCfg->tx_cnt)));
					dataCfg->tx_cnt++;
				} else {
					SSP_SendData(SSPx, (*(uint16_t *)((uint32_t)dataCfg->tx_data + dataCfg->tx_cnt)));
					dataCfg->tx_cnt += 2;
				}
			}

			// Check error
			if ((stat = SSPx->RIS) & SSP_RIS_ROR){
				// save status and return
				dataCfg->status = stat | SSP_STAT_ERROR;
				return (-1);
			}

			// Check for any data available in RX FIFO
			while ((SSPx->SR & SSP_SR_RNE) && (dataCfg->rx_cnt < dataCfg->length)){
				// Read data from SSP data
				tmp = SSP_ReceiveData(SSPx);

				// Store data to destination
				if (dataCfg->rx_data != NULL)
				{
					if (dataword == 0){
						*(uint8_t *)((uint32_t)dataCfg->rx_data + dataCfg->rx_cnt) = (uint8_t) tmp;
					} else {
						*(uint16_t *)((uint32_t)dataCfg->rx_data + dataCfg->rx_cnt) = (uint16_t) tmp;
					}
				}
				// Increase counter
				if (dataword == 0){
					dataCfg->rx_cnt++;
				} else {
					dataCfg->rx_cnt += 2;
				}
			}
		}

		// If there more data to sent or receive
		if ((dataCfg->rx_cnt < dataCfg->length) || (dataCfg->tx_cnt < dataCfg->length)){
			// Enable all interrupt
			SSPx->IMSC = SSP_IMSC_BITMASK;
		} else {
			// Save status
			dataCfg->status = SSP_STAT_DONE;
		}
		return (0);
	}

	return (-1);
}

/*********************************************************************//**
 * @brief		Checks whether the specified SSP status flag is set or not
 * @param[in]	SSPx	SSP peripheral selected, should be:
 * 		 				- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	FlagType	Type of flag to check status, should be one
 * 							of following:
 *							- SSP_STAT_TXFIFO_EMPTY: TX FIFO is empty
 *							- SSP_STAT_TXFIFO_NOTFULL: TX FIFO is not full
 *							- SSP_STAT_RXFIFO_NOTEMPTY: RX FIFO is not empty
 *							- SSP_STAT_RXFIFO_FULL: RX FIFO is full
 *							- SSP_STAT_BUSY: SSP peripheral is busy
 * @return		New State of specified SSP status flag
 **********************************************************************/
FlagStatus SSP_GetStatus(LPC_SSP_TypeDef* SSPx, uint32_t FlagType)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_SSP_STAT(FlagType));

	return ((SSPx->SR & FlagType) ? SET : RESET);
}

/*********************************************************************//**
 * @brief		Enable or disable specified interrupt type in SSP peripheral
 * @param[in]	SSPx	SSP peripheral selected, should be:
 * 						- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	IntType	Interrupt type in SSP peripheral, should be:
 * 				- SSP_INTCFG_ROR: Receive Overrun interrupt
 * 				- SSP_INTCFG_RT: Receive Time out interrupt
 * 				- SSP_INTCFG_RX: RX FIFO is at least half full interrupt
 * 				- SSP_INTCFG_TX: TX FIFO is at least half empty interrupt
 * @param[in]	NewState New State of specified interrupt type, should be:
 * 				- ENABLE: Enable this interrupt type
 * 				- DISABLE: Disable this interrupt type
 * @return		None
 * Note: We can enable/disable multi-interrupt type by OR multi value
 **********************************************************************/
void SSP_IntConfig(LPC_SSP_TypeDef *SSPx, uint32_t IntType, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));

	if (NewState == ENABLE)
	{
		SSPx->IMSC |= IntType;
	}
	else
	{
		SSPx->IMSC &= (~IntType) & SSP_IMSC_BITMASK;
	}
}

/*********************************************************************//**
 * @brief	Check whether the specified Raw interrupt status flag is
 * 			set or not
 * @param[in]	SSPx	SSP peripheral selected, should be:
 * 						- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	RawIntType	Raw Interrupt Type, should be:
 * 				- SSP_INTSTAT_RAW_ROR: Receive Overrun interrupt
 * 				- SSP_INTSTAT_RAW_RT: Receive Time out interrupt
 * 				- SSP_INTSTAT_RAW_RX: RX FIFO is at least half full interrupt
 * 				- SSP_INTSTAT_RAW_TX: TX FIFO is at least half empty interrupt
 * @return	New State of specified Raw interrupt status flag in SSP peripheral
 * Note: Enabling/Disabling specified interrupt in SSP peripheral does not
 * 		effect to Raw Interrupt Status flag.
 **********************************************************************/
IntStatus SSP_GetRawIntStatus(LPC_SSP_TypeDef *SSPx, uint32_t RawIntType)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_SSP_INTSTAT_RAW(RawIntType));

	return ((SSPx->RIS & RawIntType) ? SET : RESET);
}

/*********************************************************************//**
 * @brief		Get Raw Interrupt Status register
 * @param[in]	SSPx	SSP peripheral selected, should be:
 * 						- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @return		Raw Interrupt Status (RIS) register value
 **********************************************************************/
uint32_t SSP_GetRawIntStatusReg(LPC_SSP_TypeDef *SSPx)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	return (SSPx->RIS);
}

/*********************************************************************//**
 * @brief	Check whether the specified interrupt status flag is
 * 			set or not
 * @param[in]	SSPx	SSP peripheral selected, should be:
 * 						- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	IntType	Raw Interrupt Type, should be:
 * 				- SSP_INTSTAT_ROR: Receive Overrun interrupt
 * 				- SSP_INTSTAT_RT: Receive Time out interrupt
 * 				- SSP_INTSTAT_RX: RX FIFO is at least half full interrupt
 * 				- SSP_INTSTAT_TX: TX FIFO is at least half empty interrupt
 * @return	New State of specified interrupt status flag in SSP peripheral
 * Note: Enabling/Disabling specified interrupt in SSP peripheral effects
 * 			to Interrupt Status flag.
 **********************************************************************/
IntStatus SSP_GetIntStatus (LPC_SSP_TypeDef *SSPx, uint32_t IntType)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_SSP_INTSTAT(IntType));

	return ((SSPx->MIS & IntType) ? SET :RESET);
}

/*********************************************************************//**
 * @brief				Clear specified interrupt pending in SSP peripheral
 * @param[in]	SSPx	SSP peripheral selected, should be:
 *  					- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	IntType	Interrupt pending to clear, should be:
 * 						- SSP_INTCLR_ROR: clears the "frame was received when
 * 						RxFIFO was full" interrupt.
 * 						- SSP_INTCLR_RT: clears the "Rx FIFO was not empty and
 * 						has not been read for a timeout period" interrupt.
 * @return		None
 **********************************************************************/
void SSP_ClearIntPending(LPC_SSP_TypeDef *SSPx, uint32_t IntType)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_SSP_INTCLR(IntType));

	SSPx->ICR = IntType;
}

/*********************************************************************//**
 * @brief				Enable/Disable DMA function for SSP peripheral
 * @param[in]	SSPx	SSP peripheral selected, should be:
 *  					- LPC_SSP0: SSP0 peripheral
 * 						- LPC_SSP1: SSP1 peripheral
 * @param[in]	DMAMode	Type of DMA, should be:
 * 						- SSP_DMA_TX: DMA for the transmit FIFO
 * 						- SSP_DMA_RX: DMA for the Receive FIFO
 * @param[in]	NewState	New State of DMA function on SSP peripheral,
 * 						should be:
 * 						- ENALBE: Enable this function
 * 						- DISABLE: Disable this function
 * @return		None
 **********************************************************************/
void SSP_DMACmd(LPC_SSP_TypeDef *SSPx, uint32_t DMAMode, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SSPx(SSPx));
	CHECK_PARAM(PARAM_SSP_DMA(DMAMode));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SSPx->DMACR |= DMAMode;
	}
	else
	{
		SSPx->DMACR &= (~DMAMode) & SSP_DMA_BITMASK;
	}
}

/**
 * @}
 */

#endif /* _SSP */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

