/**********************************************************************
* $Id$		lpc17xx_i2s.h				2011-06-06
*//**
* @file		lpc17xx_i2s.h
* @brief	Contains all macro definitions and function prototypes
* 			support for I2S firmware library on LPC17xx
* @version	3.1
* @date		06. June. 2011
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
/** @defgroup I2S I2S (Inter-IC Sound bus)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_I2S_H_
#define LPC17XX_I2S_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup I2S_Public_Macros I2S Public Macros
 * @{
 */

/*********************************************************************//**
 * I2S configuration parameter defines
 **********************************************************************/
/** I2S Wordwidth bit */
#define I2S_WORDWIDTH_8		((uint32_t)(0))
#define I2S_WORDWIDTH_16	((uint32_t)(1))
#define I2S_WORDWIDTH_32	((uint32_t)(3))
/** I2S Channel bit */
#define I2S_STEREO			((uint32_t)(0))
#define I2S_MONO			((uint32_t)(1))
/** I2S Master/Slave mode bit */
#define I2S_MASTER_MODE		((uint8_t)(0))
#define I2S_SLAVE_MODE		((uint8_t)(1))
/** I2S Stop bit */
#define I2S_STOP_ENABLE		((uint8_t)(1))
#define I2S_STOP_DISABLE	((uint8_t)(0))
/** I2S Reset bit */
#define I2S_RESET_ENABLE	((uint8_t)(1))
#define I2S_RESET_DISABLE	((uint8_t)(0))
/** I2S Mute bit */
#define I2S_MUTE_ENABLE		((uint8_t)(1))
#define I2S_MUTE_DISABLE	((uint8_t)(0))
/** I2S Transmit/Receive bit */
#define I2S_TX_MODE			((uint8_t)(0))
#define I2S_RX_MODE			((uint8_t)(1))
/** I2S Clock Select bit */
#define I2S_CLKSEL_FRDCLK	((uint8_t)(0))
#define I2S_CLKSEL_MCLK		((uint8_t)(2))
/** I2S 4-pin Mode bit */
#define I2S_4PIN_ENABLE 	((uint8_t)(1))
#define I2S_4PIN_DISABLE 	((uint8_t)(0))
/** I2S MCLK Enable bit */
#define I2S_MCLK_ENABLE		((uint8_t)(1))
#define I2S_MCLK_DISABLE	((uint8_t)(0))
/** I2S select DMA bit */
#define I2S_DMA_1			((uint8_t)(0))
#define I2S_DMA_2			((uint8_t)(1))

/**
 * @}
 */

/* Private Macros ------------------------------------------------------------- */
/** @defgroup I2S_Private_Macros I2S Private Macros
 * @{
 */

/*********************************************************************//**
 * Macro defines for DAO-Digital Audio Output register
 **********************************************************************/
/** I2S wordwide - the number of bytes in data*/
#define I2S_DAO_WORDWIDTH_8		((uint32_t)(0))		/** 8 bit	*/
#define I2S_DAO_WORDWIDTH_16	((uint32_t)(1))		/** 16 bit	*/
#define I2S_DAO_WORDWIDTH_32	((uint32_t)(3))		/** 32 bit	*/
/** I2S control mono or stereo format */
#define I2S_DAO_MONO			((uint32_t)(1<<2))
/** I2S control stop mode */
#define I2S_DAO_STOP			((uint32_t)(1<<3))
/** I2S control reset mode */
#define I2S_DAO_RESET			((uint32_t)(1<<4))
/** I2S control master/slave mode */
#define I2S_DAO_SLAVE			((uint32_t)(1<<5))
/** I2S word select half period minus one */
#define I2S_DAO_WS_HALFPERIOD(n)	((uint32_t)(n<<6))
/** I2S control mute mode */
#define I2S_DAO_MUTE			((uint32_t)(1<<15))

/*********************************************************************//**
 * Macro defines for DAI-Digital Audio Input register
**********************************************************************/
/** I2S wordwide - the number of bytes in data*/
#define I2S_DAI_WORDWIDTH_8		((uint32_t)(0))		/** 8 bit	*/
#define I2S_DAI_WORDWIDTH_16	((uint32_t)(1))		/** 16 bit	*/
#define I2S_DAI_WORDWIDTH_32	((uint32_t)(3))		/** 32 bit	*/
/** I2S control mono or stereo format */
#define I2S_DAI_MONO			((uint32_t)(1<<2))
/** I2S control stop mode */
#define I2S_DAI_STOP			((uint32_t)(1<<3))
/** I2S control reset mode */
#define I2S_DAI_RESET			((uint32_t)(1<<4))
/** I2S control master/slave mode */
#define I2S_DAI_SLAVE			((uint32_t)(1<<5))
/** I2S word select half period minus one (9 bits)*/
#define I2S_DAI_WS_HALFPERIOD(n)	((uint32_t)((n&0x1FF)<<6))
/** I2S control mute mode */
#define I2S_DAI_MUTE			((uint32_t)(1<<15))

/*********************************************************************//**
 * Macro defines for STAT register (Status Feedback register)
**********************************************************************/
/** I2S Status Receive or Transmit Interrupt */
#define I2S_STATE_IRQ		((uint32_t)(1))
/** I2S Status Receive or Transmit DMA1 */
#define I2S_STATE_DMA1		((uint32_t)(1<<1))
/** I2S Status Receive or Transmit DMA2 */
#define I2S_STATE_DMA2		((uint32_t)(1<<2))
/** I2S Status Current level of the Receive FIFO (5 bits)*/
#define I2S_STATE_RX_LEVEL(n)	((uint32_t)((n&1F)<<8))
/** I2S Status Current level of the Transmit FIFO (5 bits)*/
#define I2S_STATE_TX_LEVEL(n)	((uint32_t)((n&1F)<<16))

/*********************************************************************//**
 * Macro defines for DMA1 register (DMA1 Configuration register)
**********************************************************************/
/** I2S control DMA1 for I2S receive */
#define I2S_DMA1_RX_ENABLE		((uint32_t)(1))
/** I2S control DMA1 for I2S transmit */
#define I2S_DMA1_TX_ENABLE		((uint32_t)(1<<1))
/** I2S set FIFO level that trigger a receive DMA request on DMA1 */
#define I2S_DMA1_RX_DEPTH(n)	((uint32_t)((n&0x1F)<<8))
/** I2S set FIFO level that trigger a transmit DMA request on DMA1 */
#define I2S_DMA1_TX_DEPTH(n)	((uint32_t)((n&0x1F)<<16))

/*********************************************************************//**
 * Macro defines for DMA2 register (DMA2 Configuration register)
**********************************************************************/
/** I2S control DMA2 for I2S receive */
#define I2S_DMA2_RX_ENABLE		((uint32_t)(1))
/** I2S control DMA1 for I2S transmit */
#define I2S_DMA2_TX_ENABLE		((uint32_t)(1<<1))
/** I2S set FIFO level that trigger a receive DMA request on DMA1 */
#define I2S_DMA2_RX_DEPTH(n)	((uint32_t)((n&0x1F)<<8))
/** I2S set FIFO level that trigger a transmit DMA request on DMA1 */
#define I2S_DMA2_TX_DEPTH(n)	((uint32_t)((n&0x1F)<<16))

/*********************************************************************//**
* Macro defines for IRQ register (Interrupt Request Control register)
**********************************************************************/
/** I2S control I2S receive interrupt */
#define I2S_IRQ_RX_ENABLE		((uint32_t)(1))
/** I2S control I2S transmit interrupt */
#define I2S_IRQ_TX_ENABLE		((uint32_t)(1<<1))
/** I2S set the FIFO level on which to create an irq request */
#define I2S_IRQ_RX_DEPTH(n)		((uint32_t)((n&0x1F)<<8))
/** I2S set the FIFO level on which to create an irq request */
#define I2S_IRQ_TX_DEPTH(n)		((uint32_t)((n&0x1F)<<16))

/********************************************************************************//**
 * Macro defines for TXRATE/RXRATE register (Transmit/Receive Clock Rate register)
*********************************************************************************/
/** I2S Transmit MCLK rate denominator */
#define I2S_TXRATE_Y_DIVIDER(n)	((uint32_t)(n&0xFF))
/** I2S Transmit MCLK rate denominator */
#define I2S_TXRATE_X_DIVIDER(n)	((uint32_t)((n&0xFF)<<8))
/** I2S Receive MCLK rate denominator */
#define I2S_RXRATE_Y_DIVIDER(n)	((uint32_t)(n&0xFF))
/** I2S Receive MCLK rate denominator */
#define I2S_RXRATE_X_DIVIDER(n)	((uint32_t)((n&0xFF)<<8))

/*************************************************************************************//**
 * Macro defines for TXBITRATE & RXBITRATE register (Transmit/Receive Bit Rate register)
**************************************************************************************/
#define I2S_TXBITRATE(n)	((uint32_t)(n&0x3F))
#define I2S_RXBITRATE(n)	((uint32_t)(n&0x3F))

/**********************************************************************************//**
 * Macro defines for TXMODE/RXMODE register (Transmit/Receive Mode Control register)
************************************************************************************/
/** I2S Transmit select clock source (2 bits)*/
#define I2S_TXMODE_CLKSEL(n)	((uint32_t)(n&0x03))
/** I2S Transmit control 4-pin mode */
#define I2S_TXMODE_4PIN_ENABLE	((uint32_t)(1<<2))
/** I2S Transmit control the TX_MCLK output */
#define I2S_TXMODE_MCENA		((uint32_t)(1<<3))
/** I2S Receive select clock source */
#define I2S_RXMODE_CLKSEL(n)	((uint32_t)(n&0x03))
/** I2S Receive control 4-pin mode */
#define I2S_RXMODE_4PIN_ENABLE	((uint32_t)(1<<2))
/** I2S Receive control the TX_MCLK output */
#define I2S_RXMODE_MCENA		((uint32_t)(1<<3))


/* ---------------- CHECK PARAMETER DEFINITIONS ---------------------------- */
/** Macro to determine if it is valid I2S peripheral */
#define PARAM_I2Sx(n)	(((uint32_t *)n)==((uint32_t *)LPC_I2S))
/** Macro to check Data to send valid */
#define PRAM_I2S_FREQ(freq)		((freq>=16000)&&(freq <= 96000))
/* Macro check I2S word width type */
#define PARAM_I2S_WORDWIDTH(n)	((n==I2S_WORDWIDTH_8)||(n==I2S_WORDWIDTH_16)\
||(n==I2S_WORDWIDTH_32))
/* Macro check I2S channel type */
#define PARAM_I2S_CHANNEL(n)	((n==I2S_STEREO)||(n==I2S_MONO))
/* Macro check I2S master/slave mode */
#define PARAM_I2S_WS_SEL(n)		((n==I2S_MASTER_MODE)||(n==I2S_SLAVE_MODE))
/* Macro check I2S stop mode */
#define PARAM_I2S_STOP(n)	((n==I2S_STOP_ENABLE)||(n==I2S_STOP_DISABLE))
/* Macro check I2S reset mode */
#define PARAM_I2S_RESET(n)	((n==I2S_RESET_ENABLE)||(n==I2S_RESET_DISABLE))
/* Macro check I2S reset mode */
#define PARAM_I2S_MUTE(n)	((n==I2S_MUTE_ENABLE)||(n==I2S_MUTE_DISABLE))
/* Macro check I2S transmit/receive mode */
#define PARAM_I2S_TRX(n) 		((n==I2S_TX_MODE)||(n==I2S_RX_MODE))
/* Macro check I2S clock select mode */
#define PARAM_I2S_CLKSEL(n)		((n==I2S_CLKSEL_FRDCLK)||(n==I2S_CLKSEL_MCLK))
/* Macro check I2S 4-pin mode */
#define PARAM_I2S_4PIN(n)	((n==I2S_4PIN_ENABLE)||(n==I2S_4PIN_DISABLE))
/* Macro check I2S MCLK mode */
#define PARAM_I2S_MCLK(n)	((n==I2S_MCLK_ENABLE)||(n==I2S_MCLK_DISABLE))
/* Macro check I2S DMA mode */
#define PARAM_I2S_DMA(n)		((n==I2S_DMA_1)||(n==I2S_DMA_2))
/* Macro check I2S DMA depth value */
#define PARAM_I2S_DMA_DEPTH(n)	(n<=31)
/* Macro check I2S irq level value */
#define PARAM_I2S_IRQ_LEVEL(n)	(n<=31)
/* Macro check I2S half-period value */
#define PARAM_I2S_HALFPERIOD(n)	(n<512)
/* Macro check I2S bit-rate value */
#define PARAM_I2S_BITRATE(n)	(n<=63)
/**
 * @}
 */



/* Public Types --------------------------------------------------------------- */
/** @defgroup I2S_Public_Types I2S Public Types
 * @{
 */

/**
 * @brief I2S configuration structure definition
 */
typedef struct {
	uint8_t wordwidth;		/** the number of bytes in data as follow:
							-I2S_WORDWIDTH_8: 8 bit data
							-I2S_WORDWIDTH_16: 16 bit data
							-I2S_WORDWIDTH_32: 32 bit data */
	uint8_t	mono; 			/** Set mono/stereo mode, should be:
							- I2S_STEREO: stereo mode
							- I2S_MONO: mono mode */
	uint8_t stop;			/** Disables accesses on FIFOs, should be:
							- I2S_STOP_ENABLE: enable stop mode
							- I2S_STOP_DISABLE: disable stop mode */
	uint8_t reset;			/** Asynchronously reset tje transmit channel and FIFO, should be:
							- I2S_RESET_ENABLE: enable reset mode
							- I2S_RESET_DISABLE: disable reset mode */
	uint8_t ws_sel;			/** Set Master/Slave mode, should be:
							- I2S_MASTER_MODE: I2S master mode
							- I2S_SLAVE_MODE: I2S slave mode */
	uint8_t mute;			/** MUTE mode: when true, the transmit channel sends only zeroes, shoule be:
							- I2S_MUTE_ENABLE: enable mute mode
							- I2S_MUTE_DISABLE: disable mute mode */
	uint8_t Reserved0[2];
} I2S_CFG_Type;

/**
 * @brief I2S DMA configuration structure definition
 */
typedef struct {
	uint8_t DMAIndex;		/** Select DMA1 or DMA2, should be:
							- I2S_DMA_1: DMA1
							- I2S_DMA_2: DMA2 */
	uint8_t depth;			/** FIFO level that triggers a DMA request */
	uint8_t Reserved0[2];
}I2S_DMAConf_Type;

/**
 * @brief I2S mode configuration structure definition
 */
typedef struct{
	uint8_t clksel;			/** Clock source selection, should be:
							- I2S_CLKSEL_FRDCLK: Select the fractional rate divider clock output
							- I2S_CLKSEL_MCLK: Select the MCLK signal as the clock source */
	uint8_t fpin;			/** Select four pin mode, should be:
							- I2S_4PIN_ENABLE: 4-pin enable
							- I2S_4PIN_DISABLE: 4-pin disable */
	uint8_t mcena;			/** Select MCLK mode, should be:
							- I2S_MCLK_ENABLE: MCLK enable for output
							- I2S_MCLK_DISABLE: MCLK disable for output */
	uint8_t Reserved;
}I2S_MODEConf_Type;


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup I2S_Public_Functions I2S Public Functions
 * @{
 */
/* I2S Init/DeInit functions ---------*/
void I2S_Init(LPC_I2S_TypeDef *I2Sx);
void I2S_DeInit(LPC_I2S_TypeDef *I2Sx);

/* I2S configuration functions --------*/
void I2S_Config(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode, I2S_CFG_Type* ConfigStruct);
Status I2S_FreqConfig(LPC_I2S_TypeDef *I2Sx, uint32_t Freq, uint8_t TRMode);
void I2S_SetBitRate(LPC_I2S_TypeDef *I2Sx, uint8_t bitrate, uint8_t TRMode);
void I2S_ModeConfig(LPC_I2S_TypeDef *I2Sx, I2S_MODEConf_Type* ModeConfig, uint8_t TRMode);
uint8_t I2S_GetLevel(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode);

/* I2S operate functions -------------*/
void I2S_Send(LPC_I2S_TypeDef *I2Sx, uint32_t BufferData);
uint32_t I2S_Receive(LPC_I2S_TypeDef* I2Sx);
void I2S_Start(LPC_I2S_TypeDef *I2Sx);
void I2S_Pause(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode);
void I2S_Mute(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode);
void I2S_Stop(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode);

/* I2S DMA functions ----------------*/
void I2S_DMAConfig(LPC_I2S_TypeDef *I2Sx, I2S_DMAConf_Type* DMAConfig, uint8_t TRMode);
void I2S_DMACmd(LPC_I2S_TypeDef *I2Sx, uint8_t DMAIndex,uint8_t TRMode, FunctionalState NewState);

/* I2S IRQ functions ----------------*/
void I2S_IRQCmd(LPC_I2S_TypeDef *I2Sx,uint8_t TRMode, FunctionalState NewState);
void I2S_IRQConfig(LPC_I2S_TypeDef *I2Sx, uint8_t TRMode, uint8_t level);
FunctionalState I2S_GetIRQStatus(LPC_I2S_TypeDef *I2Sx,uint8_t TRMode);
uint8_t I2S_GetIRQDepth(LPC_I2S_TypeDef *I2Sx,uint8_t TRMode);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* LPC17XX_SSP_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
