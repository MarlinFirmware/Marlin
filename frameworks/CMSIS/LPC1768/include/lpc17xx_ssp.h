/**********************************************************************
* $Id$		lpc17xx_ssp.h				2010-06-18
*//**
* @file		lpc17xx_ssp.h
* @brief	Contains all macro definitions and function prototypes
* 			support for SSP firmware library on LPC17xx
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
/** @defgroup SSP SSP (Synchronous Serial Port)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_SSP_H_
#define LPC17XX_SSP_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup SSP_Public_Macros SSP Public Macros
 * @{
 */

/*********************************************************************//**
 * SSP configuration parameter defines
 **********************************************************************/
/** Clock phase control bit */
#define SSP_CPHA_FIRST			((uint32_t)(0))
#define SSP_CPHA_SECOND			SSP_CR0_CPHA_SECOND


/** Clock polarity control bit */
/* There's no bug here!!!
 * - If bit[6] in SSPnCR0 is 0: SSP controller maintains the bus clock low between frames.
 * That means the active clock is in HI state.
 * - If bit[6] in SSPnCR0 is 1 (SSP_CR0_CPOL_HI): SSP controller maintains the bus clock
 * high between frames. That means the active clock is in LO state.
 */
#define SSP_CPOL_HI				((uint32_t)(0))
#define SSP_CPOL_LO				SSP_CR0_CPOL_HI

/** SSP master mode enable */
#define SSP_SLAVE_MODE			SSP_CR1_SLAVE_EN
#define SSP_MASTER_MODE			((uint32_t)(0))

/** SSP data bit number defines */
#define SSP_DATABIT_4		SSP_CR0_DSS(4) 			/*!< Databit number = 4 */
#define SSP_DATABIT_5		SSP_CR0_DSS(5) 			/*!< Databit number = 5 */
#define SSP_DATABIT_6		SSP_CR0_DSS(6) 			/*!< Databit number = 6 */
#define SSP_DATABIT_7		SSP_CR0_DSS(7) 			/*!< Databit number = 7 */
#define SSP_DATABIT_8		SSP_CR0_DSS(8) 			/*!< Databit number = 8 */
#define SSP_DATABIT_9		SSP_CR0_DSS(9) 			/*!< Databit number = 9 */
#define SSP_DATABIT_10		SSP_CR0_DSS(10) 		/*!< Databit number = 10 */
#define SSP_DATABIT_11		SSP_CR0_DSS(11) 		/*!< Databit number = 11 */
#define SSP_DATABIT_12		SSP_CR0_DSS(12) 		/*!< Databit number = 12 */
#define SSP_DATABIT_13		SSP_CR0_DSS(13) 		/*!< Databit number = 13 */
#define SSP_DATABIT_14		SSP_CR0_DSS(14) 		/*!< Databit number = 14 */
#define SSP_DATABIT_15		SSP_CR0_DSS(15) 		/*!< Databit number = 15 */
#define SSP_DATABIT_16		SSP_CR0_DSS(16) 		/*!< Databit number = 16 */

/** SSP Frame Format definition */
/** Motorola SPI mode */
#define SSP_FRAME_SPI		SSP_CR0_FRF_SPI
/** TI synchronous serial mode */
#define SSP_FRAME_TI		SSP_CR0_FRF_TI
/** National Micro-wire mode */
#define SSP_FRAME_MICROWIRE	SSP_CR0_FRF_MICROWIRE

/*********************************************************************//**
 * SSP Status defines
 **********************************************************************/
/** SSP status TX FIFO Empty bit */
#define SSP_STAT_TXFIFO_EMPTY		SSP_SR_TFE
/** SSP status TX FIFO not full bit */
#define SSP_STAT_TXFIFO_NOTFULL		SSP_SR_TNF
/** SSP status RX FIFO not empty bit */
#define SSP_STAT_RXFIFO_NOTEMPTY	SSP_SR_RNE
/** SSP status RX FIFO full bit */
#define SSP_STAT_RXFIFO_FULL		SSP_SR_RFF
/** SSP status SSP Busy bit */
#define SSP_STAT_BUSY				SSP_SR_BSY

/*********************************************************************//**
 * SSP Interrupt Configuration defines
 **********************************************************************/
/** Receive Overrun */
#define SSP_INTCFG_ROR		SSP_IMSC_ROR
/** Receive TimeOut */
#define SSP_INTCFG_RT		SSP_IMSC_RT
/** Rx FIFO is at least half full */
#define SSP_INTCFG_RX		SSP_IMSC_RX
/** Tx FIFO is at least half empty */
#define SSP_INTCFG_TX		SSP_IMSC_TX

/*********************************************************************//**
 * SSP Configured Interrupt Status defines
 **********************************************************************/
/** Receive Overrun */
#define SSP_INTSTAT_ROR		SSP_MIS_ROR
/** Receive TimeOut */
#define SSP_INTSTAT_RT		SSP_MIS_RT
/** Rx FIFO is at least half full */
#define SSP_INTSTAT_RX		SSP_MIS_RX
/** Tx FIFO is at least half empty */
#define SSP_INTSTAT_TX		SSP_MIS_TX

/*********************************************************************//**
 * SSP Raw Interrupt Status defines
 **********************************************************************/
/** Receive Overrun */
#define SSP_INTSTAT_RAW_ROR		SSP_RIS_ROR
/** Receive TimeOut */
#define SSP_INTSTAT_RAW_RT		SSP_RIS_RT
/** Rx FIFO is at least half full */
#define SSP_INTSTAT_RAW_RX		SSP_RIS_RX
/** Tx FIFO is at least half empty */
#define SSP_INTSTAT_RAW_TX		SSP_RIS_TX

/*********************************************************************//**
 * SSP Interrupt Clear defines
 **********************************************************************/
/** Writing a 1 to this bit clears the "frame was received when
 * RxFIFO was full" interrupt */
#define SSP_INTCLR_ROR		SSP_ICR_ROR
/** Writing a 1 to this bit clears the "Rx FIFO was not empty and
 * has not been read for a timeout period" interrupt */
#define SSP_INTCLR_RT		SSP_ICR_RT

/*********************************************************************//**
 * SSP DMA defines
 **********************************************************************/
/** SSP bit for enabling RX DMA */
#define SSP_DMA_RX		SSP_DMA_RXDMA_EN
/** SSP bit for enabling TX DMA */
#define SSP_DMA_TX		SSP_DMA_TXDMA_EN

/* SSP Status Implementation definitions */
#define SSP_STAT_DONE		(1UL<<8)		/**< Done */
#define SSP_STAT_ERROR		(1UL<<9)		/**< Error */

/**
 * @}
 */

/* Private Macros ------------------------------------------------------------- */
/** @defgroup SSP_Private_Macros SSP Private Macros
 * @{
 */

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for CR0 register
 **********************************************************************/
/** SSP data size select, must be 4 bits to 16 bits */
#define SSP_CR0_DSS(n)   		((uint32_t)((n-1)&0xF))
/** SSP control 0 Motorola SPI mode */
#define SSP_CR0_FRF_SPI  		((uint32_t)(0<<4))
/** SSP control 0 TI synchronous serial mode */
#define SSP_CR0_FRF_TI   		((uint32_t)(1<<4))
/** SSP control 0 National Micro-wire mode */
#define SSP_CR0_FRF_MICROWIRE  	((uint32_t)(2<<4))
/** SPI clock polarity bit (used in SPI mode only), (1) = maintains the
   bus clock high between frames, (0) = low */
#define SSP_CR0_CPOL_HI		((uint32_t)(1<<6))
/** SPI clock out phase bit (used in SPI mode only), (1) = captures data
   on the second clock transition of the frame, (0) = first */
#define SSP_CR0_CPHA_SECOND	((uint32_t)(1<<7))
/** SSP serial clock rate value load macro, divider rate is
   PERIPH_CLK / (cpsr * (SCR + 1)) */
#define SSP_CR0_SCR(n)   	((uint32_t)((n&0xFF)<<8))
/** SSP CR0 bit mask */
#define SSP_CR0_BITMASK		((uint32_t)(0xFFFF))

/*********************************************************************//**
 * Macro defines for CR1 register
 **********************************************************************/
/** SSP control 1 loopback mode enable bit */
#define SSP_CR1_LBM_EN		((uint32_t)(1<<0))
/** SSP control 1 enable bit */
#define SSP_CR1_SSP_EN		((uint32_t)(1<<1))
/** SSP control 1 slave enable */
#define SSP_CR1_SLAVE_EN	((uint32_t)(1<<2))
/** SSP control 1 slave out disable bit, disables transmit line in slave
   mode */
#define SSP_CR1_SO_DISABLE	((uint32_t)(1<<3))
/** SSP CR1 bit mask */
#define SSP_CR1_BITMASK		((uint32_t)(0x0F))

/*********************************************************************//**
 * Macro defines for DR register
 **********************************************************************/
/** SSP data bit mask */
#define SSP_DR_BITMASK(n)   ((n)&0xFFFF)

/*********************************************************************//**
 * Macro defines for SR register
 **********************************************************************/
/** SSP status TX FIFO Empty bit */
#define SSP_SR_TFE      ((uint32_t)(1<<0))
/** SSP status TX FIFO not full bit */
#define SSP_SR_TNF      ((uint32_t)(1<<1))
/** SSP status RX FIFO not empty bit */
#define SSP_SR_RNE      ((uint32_t)(1<<2))
/** SSP status RX FIFO full bit */
#define SSP_SR_RFF      ((uint32_t)(1<<3))
/** SSP status SSP Busy bit */
#define SSP_SR_BSY      ((uint32_t)(1<<4))
/** SSP SR bit mask */
#define SSP_SR_BITMASK	((uint32_t)(0x1F))

/*********************************************************************//**
 * Macro defines for CPSR register
 **********************************************************************/
/** SSP clock prescaler */
#define SSP_CPSR_CPDVSR(n) 	((uint32_t)(n&0xFF))
/** SSP CPSR bit mask */
#define SSP_CPSR_BITMASK	((uint32_t)(0xFF))

/*********************************************************************//**
 * Macro define for (IMSC) Interrupt Mask Set/Clear registers
 **********************************************************************/
/** Receive Overrun */
#define SSP_IMSC_ROR	((uint32_t)(1<<0))
/** Receive TimeOut */
#define SSP_IMSC_RT		((uint32_t)(1<<1))
/** Rx FIFO is at least half full */
#define SSP_IMSC_RX		((uint32_t)(1<<2))
/** Tx FIFO is at least half empty */
#define SSP_IMSC_TX		((uint32_t)(1<<3))
/** IMSC bit mask */
#define SSP_IMSC_BITMASK	((uint32_t)(0x0F))

/*********************************************************************//**
 * Macro define for (RIS) Raw Interrupt Status registers
 **********************************************************************/
/** Receive Overrun */
#define SSP_RIS_ROR		((uint32_t)(1<<0))
/** Receive TimeOut */
#define SSP_RIS_RT		((uint32_t)(1<<1))
/** Rx FIFO is at least half full */
#define SSP_RIS_RX		((uint32_t)(1<<2))
/** Tx FIFO is at least half empty */
#define SSP_RIS_TX		((uint32_t)(1<<3))
/** RIS bit mask */
#define SSP_RIS_BITMASK	((uint32_t)(0x0F))

/*********************************************************************//**
 * Macro define for (MIS) Masked Interrupt Status registers
 **********************************************************************/
/** Receive Overrun */
#define SSP_MIS_ROR		((uint32_t)(1<<0))
/** Receive TimeOut */
#define SSP_MIS_RT		((uint32_t)(1<<1))
/** Rx FIFO is at least half full */
#define SSP_MIS_RX		((uint32_t)(1<<2))
/** Tx FIFO is at least half empty */
#define SSP_MIS_TX		((uint32_t)(1<<3))
/** MIS bit mask */
#define SSP_MIS_BITMASK	((uint32_t)(0x0F))

/*********************************************************************//**
 * Macro define for (ICR) Interrupt Clear registers
 **********************************************************************/
/** Writing a 1 to this bit clears the "frame was received when
 * RxFIFO was full" interrupt */
#define SSP_ICR_ROR		((uint32_t)(1<<0))
/** Writing a 1 to this bit clears the "Rx FIFO was not empty and
 * has not been read for a timeout period" interrupt */
#define SSP_ICR_RT		((uint32_t)(1<<1))
/** ICR bit mask */
#define SSP_ICR_BITMASK	((uint32_t)(0x03))

/*********************************************************************//**
 * Macro defines for DMACR register
 **********************************************************************/
/** SSP bit for enabling RX DMA */
#define SSP_DMA_RXDMA_EN  	((uint32_t)(1<<0))
/** SSP bit for enabling TX DMA */
#define SSP_DMA_TXDMA_EN  	((uint32_t)(1<<1))
/** DMACR	bit mask */
#define SSP_DMA_BITMASK		((uint32_t)(0x03))


/* ---------------- CHECK PARAMETER DEFINITIONS ---------------------------- */
/** Macro to determine if it is valid SSP port number */
#define PARAM_SSPx(n)	((((uint32_t *)n)==((uint32_t *)LPC_SSP0)) \
|| (((uint32_t *)n)==((uint32_t *)LPC_SSP1)))

/** Macro check clock phase control mode */
#define PARAM_SSP_CPHA(n) 		((n==SSP_CPHA_FIRST) || (n==SSP_CPHA_SECOND))

/** Macro check clock polarity mode */
#define PARAM_SSP_CPOL(n)		((n==SSP_CPOL_HI) || (n==SSP_CPOL_LO))

/* Macro check master/slave mode */
#define PARAM_SSP_MODE(n)		((n==SSP_SLAVE_MODE) || (n==SSP_MASTER_MODE))

/* Macro check databit value */
#define PARAM_SSP_DATABIT(n) 	((n==SSP_DATABIT_4) || (n==SSP_DATABIT_5) \
|| (n==SSP_DATABIT_6) || (n==SSP_DATABIT_16) \
|| (n==SSP_DATABIT_7) || (n==SSP_DATABIT_8) \
|| (n==SSP_DATABIT_9) || (n==SSP_DATABIT_10) \
|| (n==SSP_DATABIT_11) || (n==SSP_DATABIT_12) \
|| (n==SSP_DATABIT_13) || (n==SSP_DATABIT_14) \
|| (n==SSP_DATABIT_15))

/* Macro check frame type */
#define PARAM_SSP_FRAME(n) ((n==SSP_FRAME_SPI) || (n==SSP_FRAME_TI)\
|| (n==SSP_FRAME_MICROWIRE))

/* Macro check SSP status */
#define PARAM_SSP_STAT(n) ((n==SSP_STAT_TXFIFO_EMPTY) || (n==SSP_STAT_TXFIFO_NOTFULL) \
|| (n==SSP_STAT_RXFIFO_NOTEMPTY) || (n==SSP_STAT_RXFIFO_FULL) \
|| (n==SSP_STAT_BUSY))

/* Macro check interrupt configuration */
#define PARAM_SSP_INTCFG(n)	((n==SSP_INTCFG_ROR) || (n==SSP_INTCFG_RT) \
|| (n==SSP_INTCFG_RX) || (n==SSP_INTCFG_TX))

/* Macro check interrupt status value */
#define PARAM_SSP_INTSTAT(n) ((n==SSP_INTSTAT_ROR) || (n==SSP_INTSTAT_RT) \
|| (n==SSP_INTSTAT_RX) || (n==SSP_INTSTAT_TX))

/* Macro check interrupt status raw value */
#define PARAM_SSP_INTSTAT_RAW(n)	((n==SSP_INTSTAT_RAW_ROR) || (n==SSP_INTSTAT_RAW_RT) \
|| (n==SSP_INTSTAT_RAW_RX) || (n==SSP_INTSTAT_RAW_TX))

/* Macro check interrupt clear mode */
#define PARAM_SSP_INTCLR(n)	((n==SSP_INTCLR_ROR) || (n==SSP_INTCLR_RT))

/* Macro check DMA mode */
#define PARAM_SSP_DMA(n)	((n==SSP_DMA_TX) || (n==SSP_DMA_RX))
/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup SSP_Public_Types SSP Public Types
 * @{
 */

/** @brief SSP configuration structure */
typedef struct {
	uint32_t Databit; 		/** Databit number, should be SSP_DATABIT_x,
							where x is in range from 4 - 16 */
	uint32_t CPHA;			/** Clock phase, should be:
							- SSP_CPHA_FIRST: first clock edge
							- SSP_CPHA_SECOND: second clock edge */
	uint32_t CPOL;			/** Clock polarity, should be:
							- SSP_CPOL_HI: high level
							- SSP_CPOL_LO: low level */
	uint32_t Mode;			/** SSP mode, should be:
							- SSP_MASTER_MODE: Master mode
							- SSP_SLAVE_MODE: Slave mode */
	uint32_t FrameFormat;	/** Frame Format:
							- SSP_FRAME_SPI: Motorola SPI frame format
							- SSP_FRAME_TI: TI frame format
							- SSP_FRAME_MICROWIRE: National Microwire frame format */
	uint32_t ClockRate;		/** Clock rate,in Hz */
} SSP_CFG_Type;

/**
 * @brief SSP Transfer Type definitions
 */
typedef enum {
	SSP_TRANSFER_POLLING = 0,	/**< Polling transfer */
	SSP_TRANSFER_INTERRUPT		/**< Interrupt transfer */
} SSP_TRANSFER_Type;

/**
 * @brief SPI Data configuration structure definitions
 */
typedef struct {
	void *tx_data;				/**< Pointer to transmit data */
	uint32_t tx_cnt;			/**< Transmit counter */
	void *rx_data;				/**< Pointer to transmit data */
	uint32_t rx_cnt;			/**< Receive counter */
	uint32_t length;			/**< Length of transfer data */
	uint32_t status;			/**< Current status of SSP activity */
} SSP_DATA_SETUP_Type;


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup SSP_Public_Functions SSP Public Functions
 * @{
 */

/* SSP Init/DeInit functions --------------------------------------------------*/
void SSP_Init(LPC_SSP_TypeDef *SSPx, SSP_CFG_Type *SSP_ConfigStruct);
void SSP_DeInit(LPC_SSP_TypeDef* SSPx);

/* SSP configure functions ----------------------------------------------------*/
void SSP_ConfigStructInit(SSP_CFG_Type *SSP_InitStruct);

/* SSP enable/disable functions -----------------------------------------------*/
void SSP_Cmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState);
void SSP_LoopBackCmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState);
void SSP_SlaveOutputCmd(LPC_SSP_TypeDef* SSPx, FunctionalState NewState);
void SSP_DMACmd(LPC_SSP_TypeDef *SSPx, uint32_t DMAMode, FunctionalState NewState);

/* SSP get information functions ----------------------------------------------*/
FlagStatus SSP_GetStatus(LPC_SSP_TypeDef* SSPx, uint32_t FlagType);
uint8_t SSP_GetDataSize(LPC_SSP_TypeDef* SSPx);
IntStatus SSP_GetRawIntStatus(LPC_SSP_TypeDef *SSPx, uint32_t RawIntType);
uint32_t SSP_GetRawIntStatusReg(LPC_SSP_TypeDef *SSPx);
IntStatus SSP_GetIntStatus (LPC_SSP_TypeDef *SSPx, uint32_t IntType);

/* SSP transfer data functions ------------------------------------------------*/
void SSP_SendData(LPC_SSP_TypeDef* SSPx, uint16_t Data);
uint16_t SSP_ReceiveData(LPC_SSP_TypeDef* SSPx);
int32_t SSP_ReadWrite (LPC_SSP_TypeDef *SSPx, SSP_DATA_SETUP_Type *dataCfg, \
						SSP_TRANSFER_Type xfType);

/* SSP IRQ function ------------------------------------------------------------*/
void SSP_IntConfig(LPC_SSP_TypeDef *SSPx, uint32_t IntType, FunctionalState NewState);
void SSP_ClearIntPending(LPC_SSP_TypeDef *SSPx, uint32_t IntType);


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
