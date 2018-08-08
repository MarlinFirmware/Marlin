/**********************************************************************
* $Id$		lpc17xx_spi.h				2010-05-21
*//**
* @file		lpc17xx_spi.h
* @brief	Contains all macro definitions and function prototypes
* 			support for SPI firmware library on LPC17xx
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
/** @defgroup SPI SPI (Serial Peripheral Interface)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_SPI_H_
#define LPC17XX_SPI_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup SPI_Public_Macros SPI Public Macros
 * @{
 */

/*********************************************************************//**
 * SPI configuration parameter defines
 **********************************************************************/
/** Clock phase control bit */
#define SPI_CPHA_FIRST			((uint32_t)(0))
#define SPI_CPHA_SECOND			((uint32_t)(1<<3))

/** Clock polarity control bit */
#define SPI_CPOL_HI				((uint32_t)(0))
#define SPI_CPOL_LO				((uint32_t)(1<<4))

/** SPI master mode enable */
#define SPI_SLAVE_MODE			((uint32_t)(0))
#define SPI_MASTER_MODE			((uint32_t)(1<<5))

/** LSB enable bit */
#define SPI_DATA_MSB_FIRST		((uint32_t)(0))
#define SPI_DATA_LSB_FIRST		((uint32_t)(1<<6))

/** SPI data bit number defines */
#define SPI_DATABIT_16		SPI_SPCR_BITS(0)		/*!< Databit number = 16 */
#define SPI_DATABIT_8		SPI_SPCR_BITS(0x08) 	/*!< Databit number = 8 */
#define SPI_DATABIT_9		SPI_SPCR_BITS(0x09) 	/*!< Databit number = 9 */
#define SPI_DATABIT_10		SPI_SPCR_BITS(0x0A) 	/*!< Databit number = 10 */
#define SPI_DATABIT_11		SPI_SPCR_BITS(0x0B) 	/*!< Databit number = 11 */
#define SPI_DATABIT_12		SPI_SPCR_BITS(0x0C) 	/*!< Databit number = 12 */
#define SPI_DATABIT_13		SPI_SPCR_BITS(0x0D) 	/*!< Databit number = 13 */
#define SPI_DATABIT_14		SPI_SPCR_BITS(0x0E) 	/*!< Databit number = 14 */
#define SPI_DATABIT_15		SPI_SPCR_BITS(0x0F) 	/*!< Databit number = 15 */

/*********************************************************************//**
 * SPI Status Flag defines
 **********************************************************************/
/** Slave abort */
#define SPI_STAT_ABRT		SPI_SPSR_ABRT
/** Mode fault */
#define SPI_STAT_MODF		SPI_SPSR_MODF
/** Read overrun */
#define SPI_STAT_ROVR		SPI_SPSR_ROVR
/** Write collision */
#define SPI_STAT_WCOL		SPI_SPSR_WCOL
/** SPI transfer complete flag */
#define SPI_STAT_SPIF		SPI_SPSR_SPIF

/* SPI Status Implementation definitions */
#define SPI_STAT_DONE		(1UL<<8)		/**< Done */
#define SPI_STAT_ERROR		(1UL<<9)		/**< Error */

/**
 * @}
 */


/* Private Macros ------------------------------------------------------------- */
/** @defgroup SPI_Private_Macros SPI Private Macros
 * @{
 */

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for SPI Control Register
 **********************************************************************/
/** Bit enable, the SPI controller sends and receives the number
 * of bits selected by bits 11:8 */
#define SPI_SPCR_BIT_EN			((uint32_t)(1<<2))
/** Clock phase control bit */
#define SPI_SPCR_CPHA_SECOND	((uint32_t)(1<<3))
/** Clock polarity control bit */
#define SPI_SPCR_CPOL_LOW 		((uint32_t)(1<<4))
/** SPI master mode enable */
#define SPI_SPCR_MSTR		 	((uint32_t)(1<<5))
/** LSB enable bit */
#define SPI_SPCR_LSBF			((uint32_t)(1<<6))
/** SPI interrupt enable bit */
#define SPI_SPCR_SPIE			((uint32_t)(1<<7))
/**  When bit 2 of this register is 1, this field controls the
number of bits per transfer */
#define SPI_SPCR_BITS(n)		((n==0) ? ((uint32_t)0) : ((uint32_t)((n&0x0F)<<8)))
/** SPI Control bit mask */
#define SPI_SPCR_BITMASK		((uint32_t)(0xFFC))

/*********************************************************************//**
 * Macro defines for  SPI Status Register
 **********************************************************************/
/** Slave abort */
#define SPI_SPSR_ABRT		((uint32_t)(1<<3))
/** Mode fault */
#define SPI_SPSR_MODF		((uint32_t)(1<<4))
/** Read overrun */
#define SPI_SPSR_ROVR		((uint32_t)(1<<5))
/** Write collision */
#define SPI_SPSR_WCOL		((uint32_t)(1<<6))
/** SPI transfer complete flag */
#define SPI_SPSR_SPIF 		((uint32_t)(1<<7))
/** SPI Status bit mask */
#define SPI_SPSR_BITMASK	((uint32_t)(0xF8))

/*********************************************************************//**
 * Macro defines for SPI Data Register
 **********************************************************************/
/** SPI Data low bit-mask */
#define SPI_SPDR_LO_MASK	((uint32_t)(0xFF))
/** SPI Data high bit-mask */
#define SPI_SPDR_HI_MASK	((uint32_t)(0xFF00))
/** SPI Data bit-mask */
#define SPI_SPDR_BITMASK	((uint32_t)(0xFFFF))

/*********************************************************************//**
 * Macro defines for SPI Clock Counter Register
 **********************************************************************/
/** SPI clock counter setting */
#define SPI_SPCCR_COUNTER(n) 	((uint32_t)(n&0xFF))
/** SPI clock counter bit-mask */
#define SPI_SPCCR_BITMASK		((uint32_t)(0xFF))

/***********************************************************************
 * Macro defines for SPI Test Control Register
 **********************************************************************/
/** SPI Test bit */
#define SPI_SPTCR_TEST_MASK	((uint32_t)(0xFE))
/** SPI Test register bit mask */
#define SPI_SPTCR_BITMASK	((uint32_t)(0xFE))

/*********************************************************************//**
 * Macro defines for SPI Test Status Register
 **********************************************************************/
/** Slave abort */
#define SPI_SPTSR_ABRT		((uint32_t)(1<<3))
/** Mode fault */
#define SPI_SPTSR_MODF		((uint32_t)(1<<4))
/** Read overrun */
#define SPI_SPTSR_ROVR		((uint32_t)(1<<5))
/** Write collision */
#define SPI_SPTSR_WCOL		((uint32_t)(1<<6))
/** SPI transfer complete flag */
#define SPI_SPTSR_SPIF 		((uint32_t)(1<<7))
/** SPI Status bit mask */
#define SPI_SPTSR_MASKBIT	((uint32_t)(0xF8))

/*********************************************************************//**
 * Macro defines for SPI Interrupt Register
 **********************************************************************/
/** SPI interrupt flag */
#define SPI_SPINT_INTFLAG 	((uint32_t)(1<<0))
/** SPI interrupt register bit mask */
#define SPI_SPINT_BITMASK 	((uint32_t)(0x01))


/* ---------------- CHECK PARAMETER DEFINITIONS ---------------------------- */
/** Macro to determine if it is valid SPI port number */
#define PARAM_SPIx(n)	(((uint32_t *)n)==((uint32_t *)LPC_SPI))

/** Macro check Clock phase control mode */
#define PARAM_SPI_CPHA(n) 	((n==SPI_CPHA_FIRST) || (n==SPI_CPHA_SECOND))

/** Macro check Clock polarity control mode */
#define PARAM_SPI_CPOL(n)	((n==SPI_CPOL_HI) || (n==SPI_CPOL_LO))

/** Macro check master/slave mode */
#define PARAM_SPI_MODE(n)	((n==SPI_SLAVE_MODE) || (n==SPI_MASTER_MODE))

/** Macro check LSB/MSB mode */
#define PARAM_SPI_DATA_ORDER(n) ((n==SPI_DATA_MSB_FIRST) || (n==SPI_DATA_LSB_FIRST))

/** Macro check databit value */
#define PARAM_SPI_DATABIT(n)	((n==SPI_DATABIT_16) || (n==SPI_DATABIT_8) \
|| (n==SPI_DATABIT_9) || (n==SPI_DATABIT_10) \
|| (n==SPI_DATABIT_11) || (n==SPI_DATABIT_12) \
|| (n==SPI_DATABIT_13) || (n==SPI_DATABIT_14) \
|| (n==SPI_DATABIT_15))

/** Macro check status flag */
#define PARAM_SPI_STAT(n)	((n==SPI_STAT_ABRT) || (n==SPI_STAT_MODF) \
|| (n==SPI_STAT_ROVR) || (n==SPI_STAT_WCOL) \
|| (n==SPI_STAT_SPIF))

/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup SPI_Public_Types SPI Public Types
 * @{
 */

/** @brief SPI configuration structure */
typedef struct {
	uint32_t Databit; 		/** Databit number, should be SPI_DATABIT_x,
							where x is in range from 8 - 16 */
	uint32_t CPHA;			/** Clock phase, should be:
							- SPI_CPHA_FIRST: first clock edge
							- SPI_CPHA_SECOND: second clock edge */
	uint32_t CPOL;			/** Clock polarity, should be:
							- SPI_CPOL_HI: high level
							- SPI_CPOL_LO: low level */
	uint32_t Mode;			/** SPI mode, should be:
							- SPI_MASTER_MODE: Master mode
							- SPI_SLAVE_MODE: Slave mode */
	uint32_t DataOrder;		/** Data order, should be:
							- SPI_DATA_MSB_FIRST: MSB first
							- SPI_DATA_LSB_FIRST: LSB first */
	uint32_t ClockRate;		/** Clock rate,in Hz, should not exceed
							(SPI peripheral clock)/8 */
} SPI_CFG_Type;


/**
 * @brief SPI Transfer Type definitions
 */
typedef enum {
	SPI_TRANSFER_POLLING = 0,	/**< Polling transfer */
	SPI_TRANSFER_INTERRUPT		/**< Interrupt transfer */
} SPI_TRANSFER_Type;

/**
 * @brief SPI Data configuration structure definitions
 */
typedef struct {
	void *tx_data;				/**< Pointer to transmit data */
	void *rx_data;				/**< Pointer to transmit data */
	uint32_t length;			/**< Length of transfer data */
	uint32_t counter;			/**< Data counter index */
	uint32_t status;			/**< Current status of SPI activity */
} SPI_DATA_SETUP_Type;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup SPI_Public_Functions SPI Public Functions
 * @{
 */

/* SPI Init/DeInit functions ---------*/
void SPI_Init(LPC_SPI_TypeDef *SPIx, SPI_CFG_Type *SPI_ConfigStruct);
void SPI_DeInit(LPC_SPI_TypeDef *SPIx);
void SPI_SetClock (LPC_SPI_TypeDef *SPIx, uint32_t target_clock);
void SPI_ConfigStructInit(SPI_CFG_Type *SPI_InitStruct);

/* SPI transfer functions ------------*/
void SPI_SendData(LPC_SPI_TypeDef *SPIx, uint16_t Data);
uint16_t SPI_ReceiveData(LPC_SPI_TypeDef *SPIx);
int32_t SPI_ReadWrite (LPC_SPI_TypeDef *SPIx, SPI_DATA_SETUP_Type *dataCfg, SPI_TRANSFER_Type xfType);

/* SPI Interrupt functions ---------*/
void SPI_IntCmd(LPC_SPI_TypeDef *SPIx, FunctionalState NewState);
IntStatus SPI_GetIntStatus (LPC_SPI_TypeDef *SPIx);
void SPI_ClearIntPending(LPC_SPI_TypeDef *SPIx);

/* SPI get information functions-----*/
uint8_t SPI_GetDataSize (LPC_SPI_TypeDef *SPIx);
uint32_t SPI_GetStatus(LPC_SPI_TypeDef *SPIx);
FlagStatus SPI_CheckStatus (uint32_t inputSPIStatus,  uint8_t SPIStatus);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_SPI_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
