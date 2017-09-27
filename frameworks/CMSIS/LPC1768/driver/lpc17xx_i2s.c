/**********************************************************************
* $Id$		lpc17xx_i2s.c				2010-09-23
*//**
* @file		lpc17xx_i2s.c
* @brief	Contains all functions support for I2S firmware
* 			library on LPC17xx
* @version	3.1
* @date		23. Sep. 2010
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
/** @addtogroup I2S
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_i2s.h"
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


#ifdef _I2S

/* Private Functions ---------------------------------------------------------- */

static uint8_t i2s_GetWordWidth(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode);
static uint8_t i2s_GetChannel(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode);

/********************************************************************//**
 * @brief		Get I2S wordwidth value
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is the I2S mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		The wordwidth value, should be: 8,16 or 32
 *********************************************************************/
static uint8_t i2s_GetWordWidth(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode) {
	uint8_t value;

	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) {
		value = (I2Sx->I2SDAO) & 0x03; /* get wordwidth bit */
	} else {
		value = (I2Sx->I2SDAI) & 0x03; /* get wordwidth bit */
	}
	switch (value) {
	case I2S_WORDWIDTH_8:
		return 8;
	case I2S_WORDWIDTH_16:
		return 16;
	default:
		return 32;
	}
}

/********************************************************************//**
 * @brief		Get I2S channel value
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is the I2S mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		The channel value, should be: 1(mono) or 2(stereo)
 *********************************************************************/
static uint8_t i2s_GetChannel(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode) {
	uint8_t value;

	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) {
		value = ((I2Sx->I2SDAO) & 0x04)>>2; /* get bit[2] */
	} else {
		value = ((I2Sx->I2SDAI) & 0x04)>>2; /* get bit[2] */
	}
        if(value == I2S_MONO) return 1;
          return 2;
}

/* End of Private Functions --------------------------------------------------- */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup I2S_Public_Functions
 * @{
 */

/********************************************************************//**
 * @brief		Initialize I2S
 * 					- Turn on power and clock
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @return 		none
 *********************************************************************/
void I2S_Init(LPC_I2S_TypeDef *I2Sx) {
	CHECK_PARAM(PARAM_I2Sx(I2Sx));

	// Turn on power and clock
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCI2S, ENABLE);
	LPC_I2S->I2SDAI = LPC_I2S->I2SDAO = 0x00;
}

/********************************************************************//**
 * @brief		Configuration I2S, setting:
 * 					- master/slave mode
 * 					- wordwidth value
 * 					- channel mode
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode transmit/receive mode, should be:
 * 					- I2S_TX_MODE = 0: transmit mode
 * 					- I2S_RX_MODE = 1: receive mode
 * @param[in]	ConfigStruct pointer to I2S_CFG_Type structure
 *              which will be initialized.
 * @return 		none
 *********************************************************************/
void I2S_Config(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode, I2S_CFG_Type* ConfigStruct)
{
	uint32_t bps, config;

	CHECK_PARAM(PARAM_I2Sx(I2Sx));

	CHECK_PARAM(PARAM_I2S_WORDWIDTH(ConfigStruct->wordwidth));
	CHECK_PARAM(PARAM_I2S_CHANNEL(ConfigStruct->mono));
	CHECK_PARAM(PARAM_I2S_STOP(ConfigStruct->stop));
	CHECK_PARAM(PARAM_I2S_RESET(ConfigStruct->reset));
	CHECK_PARAM(PARAM_I2S_WS_SEL(ConfigStruct->ws_sel));
	CHECK_PARAM(PARAM_I2S_MUTE(ConfigStruct->mute));

	/* Setup clock */
	bps = (ConfigStruct->wordwidth +1)*8;

	/* Calculate audio config */
	config = (bps - 1)<<6 | (ConfigStruct->ws_sel)<<5 | (ConfigStruct->reset)<<4 |
		(ConfigStruct->stop)<<3 | (ConfigStruct->mono)<<2 | (ConfigStruct->wordwidth);

	if(TRMode == I2S_RX_MODE){
		LPC_I2S->I2SDAI = config;
	}else{
		LPC_I2S->I2SDAO = config;
	}
}

/********************************************************************//**
 * @brief		DeInitial both I2S transmit or receive
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @return 		none
 *********************************************************************/
void I2S_DeInit(LPC_I2S_TypeDef *I2Sx) {
	CHECK_PARAM(PARAM_I2Sx(I2Sx));

	// Turn off power and clock
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCI2S, DISABLE);
}

/********************************************************************//**
 * @brief		Get I2S Buffer Level
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode Transmit/receive mode, should be:
 * 					- I2S_TX_MODE = 0: transmit mode
 * 					- I2S_RX_MODE = 1: receive mode
 * @return 		current level of Transmit/Receive Buffer
 *********************************************************************/
uint8_t I2S_GetLevel(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode)
{
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if(TRMode == I2S_TX_MODE)
	{
		return ((I2Sx->I2SSTATE >> 16) & 0xFF);
	}
	else
	{
		return ((I2Sx->I2SSTATE >> 8) & 0xFF);
	}
}

/********************************************************************//**
 * @brief		I2S Start: clear all STOP,RESET and MUTE bit, ready to operate
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @return 		none
 *********************************************************************/
void I2S_Start(LPC_I2S_TypeDef *I2Sx)
{
	//Clear STOP,RESET and MUTE bit
	I2Sx->I2SDAO &= ~I2S_DAI_RESET;
	I2Sx->I2SDAI &= ~I2S_DAI_RESET;
	I2Sx->I2SDAO &= ~I2S_DAI_STOP;
	I2Sx->I2SDAI &= ~I2S_DAI_STOP;
	I2Sx->I2SDAO &= ~I2S_DAI_MUTE;
}

/********************************************************************//**
 * @brief		I2S Send data
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	BufferData pointer to uint32_t is the data will be send
 * @return 		none
 *********************************************************************/
void I2S_Send(LPC_I2S_TypeDef *I2Sx, uint32_t BufferData) {
	CHECK_PARAM(PARAM_I2Sx(I2Sx));

	I2Sx->I2STXFIFO = BufferData;
}

/********************************************************************//**
 * @brief		I2S Receive Data
 * @param[in]	I2Sx pointer to LPC_I2S_TypeDef
 * @return 		received value
 *********************************************************************/
uint32_t I2S_Receive(LPC_I2S_TypeDef* I2Sx) {
	CHECK_PARAM(PARAM_I2Sx(I2Sx));

	return (I2Sx->I2SRXFIFO);

}

/********************************************************************//**
 * @brief		I2S Pause
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_Pause(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode) {
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) //Transmit mode
	{
		I2Sx->I2SDAO |= I2S_DAO_STOP;
	} else //Receive mode
	{
		I2Sx->I2SDAI |= I2S_DAI_STOP;
	}
}

/********************************************************************//**
 * @brief		I2S Mute
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_Mute(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode) {
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) //Transmit mode
	{
		I2Sx->I2SDAO |= I2S_DAO_MUTE;
	} else //Receive mode
	{
		I2Sx->I2SDAI |= I2S_DAI_MUTE;
	}
}

/********************************************************************//**
 * @brief		I2S Stop
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_Stop(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode) {
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) //Transmit mode
	{
		I2Sx->I2SDAO &= ~I2S_DAO_MUTE;
		I2Sx->I2SDAO |= I2S_DAO_STOP;
		I2Sx->I2SDAO |= I2S_DAO_RESET;
	} else //Receive mode
	{
		I2Sx->I2SDAI |= I2S_DAI_STOP;
		I2Sx->I2SDAI |= I2S_DAI_RESET;
	}
}

/********************************************************************//**
 * @brief		Set frequency for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	Freq is the frequency for I2S will be set. It can range
 * 				from 16-96 kHz(16, 22.05, 32, 44.1, 48, 96kHz)
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		Status: ERROR or SUCCESS
 *********************************************************************/
Status I2S_FreqConfig(LPC_I2S_TypeDef *I2Sx, uint32_t Freq, uint8_t TRMode) {

	uint32_t i2s_clk;
	uint8_t channel, wordwidth;
	uint32_t x, y;
	uint64_t divider;
	uint16_t dif;
	uint16_t x_divide, y_divide;
	uint16_t err, ErrorOptimal = 0xFFFF;

	uint32_t N;

	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PRAM_I2S_FREQ(Freq));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	//Get the frequency of PCLK_I2S
	i2s_clk = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_I2S);

	if(TRMode == I2S_TX_MODE)
	{
		channel = i2s_GetChannel(I2Sx,I2S_TX_MODE);
		wordwidth = i2s_GetWordWidth(I2Sx,I2S_TX_MODE);
	}
	else
	{
		channel = i2s_GetChannel(I2Sx,I2S_RX_MODE);
		wordwidth = i2s_GetWordWidth(I2Sx,I2S_RX_MODE);
	}

	/* Calculate X and Y divider
	 * The MCLK rate for the I2S transmitter is determined by the value
	 * in the I2STXRATE/I2SRXRATE register. The required I2STXRATE/I2SRXRATE
	 * setting depends on the desired audio sample rate desired, the format
	 * (stereo/mono) used, and the data size.
	 * The formula is:
	 * 		I2S_MCLK = PCLK_I2S * (X/Y) / 2
     * In that, Y must be greater than or equal to X. X should divides evenly
     * into Y.
	 * We have:
	 * 		I2S_MCLK = Freq * channel*wordwidth * (I2STXBITRATE+1);
	 * So: (X/Y) = (Freq * channel*wordwidth * (I2STXBITRATE+1))*2/PCLK_I2S
	 * We use a loop function to chose the most suitable X,Y value
	 */

	/* divider is a fixed point number with 16 fractional bits */
    divider = (((uint64_t)Freq *channel*wordwidth * 2)<<16) / i2s_clk;

	/* find N that make x/y <= 1 -> divider <= 2^16 */
	for(N=64;N>0;N--){
		if((divider*N) < (1<<16)) break;
	}

	if(N == 0) return ERROR;

	divider *= N;

	for (y = 255; y > 0; y--) {
		x = y * divider;
		if(x & (0xFF000000)) continue;
		dif = x & 0xFFFF;
		if(dif>0x8000) err = 0x10000-dif;
		else err = dif;
		if (err == 0)
		{
			y_divide = y;
			break;
		}
		else if (err < ErrorOptimal)
		{
			ErrorOptimal = err;
			y_divide = y;
		}
	}
	x_divide = ((uint64_t)y_divide * Freq *(channel*wordwidth)* N * 2)/i2s_clk;
	if(x_divide >= 256) x_divide = 0xFF;
	if(x_divide == 0) x_divide = 1;

	if (TRMode == I2S_TX_MODE)// Transmitter
	{
		I2Sx->I2STXBITRATE = N-1;
		I2Sx->I2STXRATE = y_divide | (x_divide << 8);
	} else //Receiver
	{
		I2Sx->I2SRXBITRATE = N-1;
		I2Sx->I2STXRATE = y_divide | (x_divide << 8);
	}
	return SUCCESS;
}

/********************************************************************//**
 * @brief		I2S set bitrate
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	bitrate value will be set
 * 				bitrate value should be in range: 0 .. 63
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_SetBitRate(LPC_I2S_TypeDef *I2Sx, uint8_t bitrate, uint8_t TRMode)
{
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_BITRATE(bitrate));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if(TRMode == I2S_TX_MODE)
	{
		I2Sx->I2STXBITRATE = bitrate;
	}
	else
	{
		I2Sx->I2SRXBITRATE = bitrate;
	}
}

/********************************************************************//**
 * @brief		Configuration operating mode for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	ModeConfig pointer to I2S_MODEConf_Type will be used to
 * 				configure
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_ModeConfig(LPC_I2S_TypeDef *I2Sx, I2S_MODEConf_Type* ModeConfig,
		uint8_t TRMode)
{
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_CLKSEL(ModeConfig->clksel));
	CHECK_PARAM(PARAM_I2S_4PIN(ModeConfig->fpin));
	CHECK_PARAM(PARAM_I2S_MCLK(ModeConfig->mcena));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_TX_MODE) {
		I2Sx->I2STXMODE &= ~0x0F; //clear bit 3:0 in I2STXMODE register
		if (ModeConfig->clksel == I2S_CLKSEL_MCLK) {
			I2Sx->I2STXMODE |= 0x02;
		}
		if (ModeConfig->fpin == I2S_4PIN_ENABLE) {
			I2Sx->I2STXMODE |= (1 << 2);
		}
		if (ModeConfig->mcena == I2S_MCLK_ENABLE) {
			I2Sx->I2STXMODE |= (1 << 3);
		}
	} else {
		I2Sx->I2SRXMODE &= ~0x0F; //clear bit 3:0 in I2STXMODE register
		if (ModeConfig->clksel == I2S_CLKSEL_MCLK) {
			I2Sx->I2SRXMODE |= 0x02;
		}
		if (ModeConfig->fpin == I2S_4PIN_ENABLE) {
			I2Sx->I2SRXMODE |= (1 << 2);
		}
		if (ModeConfig->mcena == I2S_MCLK_ENABLE) {
			I2Sx->I2SRXMODE |= (1 << 3);
		}
	}
}

/********************************************************************//**
 * @brief		Configure DMA operation for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	DMAConfig pointer to I2S_DMAConf_Type will be used to configure
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		none
 *********************************************************************/
void I2S_DMAConfig(LPC_I2S_TypeDef *I2Sx, I2S_DMAConf_Type* DMAConfig,
		uint8_t TRMode)
{
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_DMA(DMAConfig->DMAIndex));
	CHECK_PARAM(PARAM_I2S_DMA_DEPTH(DMAConfig->depth));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_RX_MODE) {
		if (DMAConfig->DMAIndex == I2S_DMA_1) {
			LPC_I2S->I2SDMA1 = (DMAConfig->depth) << 8;
		} else {
			LPC_I2S->I2SDMA2 = (DMAConfig->depth) << 8;
		}
	} else {
		if (DMAConfig->DMAIndex == I2S_DMA_1) {
			LPC_I2S->I2SDMA1 = (DMAConfig->depth) << 16;
		} else {
			LPC_I2S->I2SDMA2 = (DMAConfig->depth) << 16;
		}
	}
}

/********************************************************************//**
 * @brief		Enable/Disable DMA operation for I2S
 * @param[in]	I2Sx: I2S peripheral selected, should be: LPC_I2S
 * @param[in]	DMAIndex chose what DMA is used, should be:
 * 				- I2S_DMA_1 = 0: DMA1
 * 				- I2S_DMA_2 = 1: DMA2
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @param[in]	NewState is new state of DMA operation, should be:
 * 				- ENABLE
 * 				- DISABLE
 * @return 		none
 *********************************************************************/
void I2S_DMACmd(LPC_I2S_TypeDef *I2Sx, uint8_t DMAIndex, uint8_t TRMode,
		FunctionalState NewState)
{
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));
	CHECK_PARAM(PARAM_I2S_DMA(DMAIndex));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));

	if (TRMode == I2S_RX_MODE) {
		if (DMAIndex == I2S_DMA_1) {
			if (NewState == ENABLE)
				I2Sx->I2SDMA1 |= 0x01;
			else
				I2Sx->I2SDMA1 &= ~0x01;
		} else {
			if (NewState == ENABLE)
				I2Sx->I2SDMA2 |= 0x01;
			else
				I2Sx->I2SDMA2 &= ~0x01;
		}
	} else {
		if (DMAIndex == I2S_DMA_1) {
			if (NewState == ENABLE)
				I2Sx->I2SDMA1 |= 0x02;
			else
				I2Sx->I2SDMA1 &= ~0x02;
		} else {
			if (NewState == ENABLE)
				I2Sx->I2SDMA2 |= 0x02;
			else
				I2Sx->I2SDMA2 &= ~0x02;
		}
	}
}

/********************************************************************//**
 * @brief		Configure IRQ for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @param[in]	level is the FIFO level that triggers IRQ request
 * @return 		none
 *********************************************************************/
void I2S_IRQConfig(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode, uint8_t level) {
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_I2S_TRX(TRMode));
	CHECK_PARAM(PARAM_I2S_IRQ_LEVEL(level));

	if (TRMode == I2S_RX_MODE) {
		I2Sx->I2SIRQ |= (level << 8);
	} else {
		I2Sx->I2SIRQ |= (level << 16);
	}
}

/********************************************************************//**
 * @brief		Enable/Disable IRQ for I2S
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @param[in]	NewState is new state of DMA operation, should be:
 * 				- ENABLE
 * 				- DISABLE
 * @return 		none
 *********************************************************************/
void I2S_IRQCmd(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode, FunctionalState NewState) {
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (TRMode == I2S_RX_MODE) {
		if (NewState == ENABLE)
			I2Sx->I2SIRQ |= 0x01;
		else
			I2Sx->I2SIRQ &= ~0x01;
		//Enable DMA

	} else {
		if (NewState == ENABLE)
			I2Sx->I2SIRQ |= 0x02;
		else
			I2Sx->I2SIRQ &= ~0x02;
	}
}

/********************************************************************//**
 * @brief		Get I2S interrupt status
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		FunctionState	should be:
 * 				- ENABLE: interrupt is enable
 * 				- DISABLE: interrupt is disable
 *********************************************************************/
FunctionalState I2S_GetIRQStatus(LPC_I2S_TypeDef *I2Sx,uint8_t TRMode)
{
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	if(TRMode == I2S_TX_MODE)
		return ((FunctionalState)((I2Sx->I2SIRQ >> 1)&0x01));
	else
		return ((FunctionalState)((I2Sx->I2SIRQ)&0x01));
}

/********************************************************************//**
 * @brief		Get I2S interrupt depth
 * @param[in]	I2Sx I2S peripheral selected, should be: LPC_I2S
 * @param[in]	TRMode is transmit/receive mode, should be:
 * 				- I2S_TX_MODE = 0: transmit mode
 * 				- I2S_RX_MODE = 1: receive mode
 * @return 		depth of FIFO level on which to create an irq request
 *********************************************************************/
uint8_t I2S_GetIRQDepth(LPC_I2S_TypeDef *I2Sx,uint8_t TRMode)
{
	CHECK_PARAM(PARAM_I2Sx(I2Sx));
	if(TRMode == I2S_TX_MODE)
		return (((I2Sx->I2SIRQ)>>16)&0xFF);
	else
		return (((I2Sx->I2SIRQ)>>8)&0xFF);
}
/**
 * @}
 */

#endif /* _I2S */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

