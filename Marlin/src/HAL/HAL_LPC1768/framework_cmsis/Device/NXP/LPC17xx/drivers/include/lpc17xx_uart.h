/**********************************************************************
* $Id$		lpc17xx_uart.h				2010-06-18
*//**
* @file		lpc17xx_uart.h
* @brief	Contains all macro definitions and function prototypes
* 			support for UART firmware library on LPC17xx
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
/** @defgroup UART UART (Universal Asynchronous Receiver/Transmitter)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef __LPC17XX_UART_H
#define __LPC17XX_UART_H

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup UART_Public_Macros  UART Public Macros
 * @{
 */

/** UART time-out definitions in case of using Read() and Write function
 * with Blocking Flag mode
 */
#define UART_BLOCKING_TIMEOUT			(0xFFFFFFFFUL)

/**
 * @}
 */

/* Private Macros ------------------------------------------------------------- */
/** @defgroup UART_Private_Macros UART Private Macros
 * @{
 */

/* Accepted Error baud rate value (in percent unit) */
#define UART_ACCEPTED_BAUDRATE_ERROR	(3)			/*!< Acceptable UART baudrate error */


/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for Macro defines for UARTn Receiver Buffer Register
 **********************************************************************/
#define UART_RBR_MASKBIT   	((uint8_t)0xFF) 		/*!< UART Received Buffer mask bit (8 bits) */

/*********************************************************************//**
 * Macro defines for Macro defines for UARTn Transmit Holding Register
 **********************************************************************/
#define UART_THR_MASKBIT   	((uint8_t)0xFF) 		/*!< UART Transmit Holding mask bit (8 bits) */

/*********************************************************************//**
 * Macro defines for Macro defines for UARTn Divisor Latch LSB register
 **********************************************************************/
#define UART_LOAD_DLL(div)	((div) & 0xFF)	/**< Macro for loading least significant halfs of divisors */
#define UART_DLL_MASKBIT	((uint8_t)0xFF)	/*!< Divisor latch LSB bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UARTn Divisor Latch MSB register
 **********************************************************************/
#define UART_DLM_MASKBIT	((uint8_t)0xFF)			/*!< Divisor latch MSB bit mask */
#define UART_LOAD_DLM(div)  (((div) >> 8) & 0xFF)	/**< Macro for loading most significant halfs of divisors */

/*********************************************************************//**
 * Macro defines for Macro defines for UART interrupt enable register
 **********************************************************************/
#define UART_IER_RBRINT_EN		((uint32_t)(1<<0)) 	/*!< RBR Interrupt enable*/
#define UART_IER_THREINT_EN		((uint32_t)(1<<1)) 	/*!< THR Interrupt enable*/
#define UART_IER_RLSINT_EN		((uint32_t)(1<<2)) 	/*!< RX line status interrupt enable*/
#define UART1_IER_MSINT_EN		((uint32_t)(1<<3))	/*!< Modem status interrupt enable */
#define UART1_IER_CTSINT_EN		((uint32_t)(1<<7))	/*!< CTS1 signal transition interrupt enable */
#define UART_IER_ABEOINT_EN		((uint32_t)(1<<8)) 	/*!< Enables the end of auto-baud interrupt */
#define UART_IER_ABTOINT_EN		((uint32_t)(1<<9)) 	/*!< Enables the auto-baud time-out interrupt */
#define UART_IER_BITMASK		((uint32_t)(0x307)) /*!< UART interrupt enable register bit mask */
#define UART1_IER_BITMASK		((uint32_t)(0x38F)) /*!< UART1 interrupt enable register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART interrupt identification register
 **********************************************************************/
#define UART_IIR_INTSTAT_PEND	((uint32_t)(1<<0))	/*!<Interrupt Status - Active low */
#define UART_IIR_INTID_RLS		((uint32_t)(3<<1)) 	/*!<Interrupt identification: Receive line status*/
#define UART_IIR_INTID_RDA		((uint32_t)(2<<1)) 	/*!<Interrupt identification: Receive data available*/
#define UART_IIR_INTID_CTI		((uint32_t)(6<<1)) 	/*!<Interrupt identification: Character time-out indicator*/
#define UART_IIR_INTID_THRE		((uint32_t)(1<<1)) 	/*!<Interrupt identification: THRE interrupt*/
#define UART1_IIR_INTID_MODEM	((uint32_t)(0<<1)) 	/*!<Interrupt identification: Modem interrupt*/
#define UART_IIR_INTID_MASK		((uint32_t)(7<<1))	/*!<Interrupt identification: Interrupt ID mask */
#define UART_IIR_FIFO_EN		((uint32_t)(3<<6)) 	/*!<These bits are equivalent to UnFCR[0] */
#define UART_IIR_ABEO_INT		((uint32_t)(1<<8)) 	/*!< End of auto-baud interrupt */
#define UART_IIR_ABTO_INT		((uint32_t)(1<<9)) 	/*!< Auto-baud time-out interrupt */
#define UART_IIR_BITMASK		((uint32_t)(0x3CF))	/*!< UART interrupt identification register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART FIFO control register
 **********************************************************************/
#define UART_FCR_FIFO_EN		((uint8_t)(1<<0)) 	/*!< UART FIFO enable */
#define UART_FCR_RX_RS			((uint8_t)(1<<1)) 	/*!< UART FIFO RX reset */
#define UART_FCR_TX_RS			((uint8_t)(1<<2)) 	/*!< UART FIFO TX reset */
#define UART_FCR_DMAMODE_SEL 	((uint8_t)(1<<3)) 	/*!< UART DMA mode selection */
#define UART_FCR_TRG_LEV0		((uint8_t)(0)) 		/*!< UART FIFO trigger level 0: 1 character */
#define UART_FCR_TRG_LEV1		((uint8_t)(1<<6)) 	/*!< UART FIFO trigger level 1: 4 character */
#define UART_FCR_TRG_LEV2		((uint8_t)(2<<6)) 	/*!< UART FIFO trigger level 2: 8 character */
#define UART_FCR_TRG_LEV3		((uint8_t)(3<<6)) 	/*!< UART FIFO trigger level 3: 14 character */
#define UART_FCR_BITMASK		((uint8_t)(0xCF))	/*!< UART FIFO control bit mask */
#define UART_TX_FIFO_SIZE		(16)

/*********************************************************************//**
 * Macro defines for Macro defines for UART line control register
 **********************************************************************/
#define UART_LCR_WLEN5     		((uint8_t)(0))   		/*!< UART 5 bit data mode */
#define UART_LCR_WLEN6     		((uint8_t)(1<<0))   	/*!< UART 6 bit data mode */
#define UART_LCR_WLEN7     		((uint8_t)(2<<0))   	/*!< UART 7 bit data mode */
#define UART_LCR_WLEN8     		((uint8_t)(3<<0))   	/*!< UART 8 bit data mode */
#define UART_LCR_STOPBIT_SEL	((uint8_t)(1<<2))   	/*!< UART Two Stop Bits Select */
#define UART_LCR_PARITY_EN		((uint8_t)(1<<3))		/*!< UART Parity Enable */
#define UART_LCR_PARITY_ODD		((uint8_t)(0))         	/*!< UART Odd Parity Select */
#define UART_LCR_PARITY_EVEN	((uint8_t)(1<<4))		/*!< UART Even Parity Select */
#define UART_LCR_PARITY_F_1		((uint8_t)(2<<4))		/*!< UART force 1 stick parity */
#define UART_LCR_PARITY_F_0		((uint8_t)(3<<4))		/*!< UART force 0 stick parity */
#define UART_LCR_BREAK_EN		((uint8_t)(1<<6))		/*!< UART Transmission Break enable */
#define UART_LCR_DLAB_EN		((uint8_t)(1<<7))    	/*!< UART Divisor Latches Access bit enable */
#define UART_LCR_BITMASK		((uint8_t)(0xFF))		/*!< UART line control bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART1 Modem Control Register
 **********************************************************************/
#define UART1_MCR_DTR_CTRL		((uint8_t)(1<<0))		/*!< Source for modem output pin DTR */
#define UART1_MCR_RTS_CTRL		((uint8_t)(1<<1))		/*!< Source for modem output pin RTS */
#define UART1_MCR_LOOPB_EN		((uint8_t)(1<<4))		/*!< Loop back mode select */
#define UART1_MCR_AUTO_RTS_EN	((uint8_t)(1<<6))		/*!< Enable Auto RTS flow-control */
#define UART1_MCR_AUTO_CTS_EN	((uint8_t)(1<<7))		/*!< Enable Auto CTS flow-control */
#define UART1_MCR_BITMASK		((uint8_t)(0x0F3))		/*!< UART1 bit mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART line status register
 **********************************************************************/
#define UART_LSR_RDR		((uint8_t)(1<<0)) 	/*!<Line status register: Receive data ready*/
#define UART_LSR_OE			((uint8_t)(1<<1)) 	/*!<Line status register: Overrun error*/
#define UART_LSR_PE			((uint8_t)(1<<2)) 	/*!<Line status register: Parity error*/
#define UART_LSR_FE			((uint8_t)(1<<3)) 	/*!<Line status register: Framing error*/
#define UART_LSR_BI			((uint8_t)(1<<4)) 	/*!<Line status register: Break interrupt*/
#define UART_LSR_THRE		((uint8_t)(1<<5)) 	/*!<Line status register: Transmit holding register empty*/
#define UART_LSR_TEMT		((uint8_t)(1<<6)) 	/*!<Line status register: Transmitter empty*/
#define UART_LSR_RXFE		((uint8_t)(1<<7)) 	/*!<Error in RX FIFO*/
#define UART_LSR_BITMASK	((uint8_t)(0xFF)) 	/*!<UART Line status bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Modem (UART1 only) status register
 **********************************************************************/
#define UART1_MSR_DELTA_CTS		((uint8_t)(1<<0))	/*!< Set upon state change of input CTS */
#define UART1_MSR_DELTA_DSR		((uint8_t)(1<<1))	/*!< Set upon state change of input DSR */
#define UART1_MSR_LO2HI_RI		((uint8_t)(1<<2))	/*!< Set upon low to high transition of input RI */
#define UART1_MSR_DELTA_DCD		((uint8_t)(1<<3))	/*!< Set upon state change of input DCD */
#define UART1_MSR_CTS			((uint8_t)(1<<4))	/*!< Clear To Send State */
#define UART1_MSR_DSR			((uint8_t)(1<<5))	/*!< Data Set Ready State */
#define UART1_MSR_RI			((uint8_t)(1<<6))	/*!< Ring Indicator State */
#define UART1_MSR_DCD			((uint8_t)(1<<7))	/*!< Data Carrier Detect State */
#define UART1_MSR_BITMASK		((uint8_t)(0xFF))	/*!< MSR register bit-mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Scratch Pad Register
 **********************************************************************/
#define UART_SCR_BIMASK		((uint8_t)(0xFF))	/*!< UART Scratch Pad bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Auto baudrate control register
 **********************************************************************/
#define UART_ACR_START				((uint32_t)(1<<0))	/**< UART Auto-baud start */
#define UART_ACR_MODE				((uint32_t)(1<<1))	/**< UART Auto baudrate Mode 1 */
#define UART_ACR_AUTO_RESTART		((uint32_t)(1<<2))	/**< UART Auto baudrate restart */
#define UART_ACR_ABEOINT_CLR		((uint32_t)(1<<8))	/**< UART End of auto-baud interrupt clear */
#define UART_ACR_ABTOINT_CLR		((uint32_t)(1<<9))	/**< UART Auto-baud time-out interrupt clear */
#define UART_ACR_BITMASK			((uint32_t)(0x307))	/**< UART Auto Baudrate register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART IrDA control register
 **********************************************************************/
#define UART_ICR_IRDAEN			((uint32_t)(1<<0))			/**< IrDA mode enable */
#define UART_ICR_IRDAINV		((uint32_t)(1<<1))			/**< IrDA serial input inverted */
#define UART_ICR_FIXPULSE_EN	((uint32_t)(1<<2))			/**< IrDA fixed pulse width mode */
#define UART_ICR_PULSEDIV(n)	((uint32_t)((n&0x07)<<3))	/**< PulseDiv - Configures the pulse when FixPulseEn = 1 */
#define UART_ICR_BITMASK		((uint32_t)(0x3F))			/*!< UART IRDA bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Fractional divider register
 **********************************************************************/
#define UART_FDR_DIVADDVAL(n)	((uint32_t)(n&0x0F))		/**< Baud-rate generation pre-scaler divisor */
#define UART_FDR_MULVAL(n)		((uint32_t)((n<<4)&0xF0))	/**< Baud-rate pre-scaler multiplier value */
#define UART_FDR_BITMASK		((uint32_t)(0xFF))			/**< UART Fractional Divider register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART Tx Enable register
 **********************************************************************/
#define UART_TER_TXEN			((uint8_t)(1<<7)) 		/*!< Transmit enable bit */
#define UART_TER_BITMASK		((uint8_t)(0x80))		/**< UART Transmit Enable Register bit mask */

/*********************************************************************//**
 * Macro defines for Macro defines for UART1 RS485 Control register
 **********************************************************************/
#define UART1_RS485CTRL_NMM_EN		((uint32_t)(1<<0))	/*!< RS-485/EIA-485 Normal Multi-drop Mode (NMM)
														is disabled */
#define UART1_RS485CTRL_RX_DIS		((uint32_t)(1<<1))	/*!< The receiver is disabled */
#define UART1_RS485CTRL_AADEN		((uint32_t)(1<<2))	/*!< Auto Address Detect (AAD) is enabled */
#define UART1_RS485CTRL_SEL_DTR		((uint32_t)(1<<3))	/*!< If direction control is enabled
														(bit DCTRL = 1), pin DTR is used for direction control */
#define UART1_RS485CTRL_DCTRL_EN	((uint32_t)(1<<4))	/*!< Enable Auto Direction Control */
#define UART1_RS485CTRL_OINV_1		((uint32_t)(1<<5))	/*!< This bit reverses the polarity of the direction
														control signal on the RTS (or DTR) pin. The direction control pin
														will be driven to logic "1" when the transmitter has data to be sent */
#define UART1_RS485CTRL_BITMASK		((uint32_t)(0x3F))	/**< RS485 control bit-mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART1 RS-485 Address Match register
 **********************************************************************/
#define UART1_RS485ADRMATCH_BITMASK ((uint8_t)(0xFF)) 	/**< Bit mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART1 RS-485 Delay value register
 **********************************************************************/
/* Macro defines for UART1 RS-485 Delay value register */
#define UART1_RS485DLY_BITMASK		((uint8_t)(0xFF)) 	/** Bit mask value */

/*********************************************************************//**
 * Macro defines for Macro defines for UART FIFO Level register
 **********************************************************************/
#define UART_FIFOLVL_RXFIFOLVL(n)	((uint32_t)(n&0x0F))		/**< Reflects the current level of the UART receiver FIFO */
#define UART_FIFOLVL_TXFIFOLVL(n)	((uint32_t)((n>>8)&0x0F))	/**< Reflects the current level of the UART transmitter FIFO */
#define UART_FIFOLVL_BITMASK		((uint32_t)(0x0F0F))		/**< UART FIFO Level Register bit mask */


/* ---------------- CHECK PARAMETER DEFINITIONS ---------------------------- */

/** Macro to check the input UART_DATABIT parameters */
#define PARAM_UART_DATABIT(databit)	((databit==UART_DATABIT_5) || (databit==UART_DATABIT_6)\
|| (databit==UART_DATABIT_7) || (databit==UART_DATABIT_8))

/** Macro to check the input UART_STOPBIT parameters */
#define PARAM_UART_STOPBIT(stopbit)	((stopbit==UART_STOPBIT_1) || (stopbit==UART_STOPBIT_2))

/** Macro to check the input UART_PARITY parameters */
#define PARAM_UART_PARITY(parity)	((parity==UART_PARITY_NONE) || (parity==UART_PARITY_ODD) \
|| (parity==UART_PARITY_EVEN) || (parity==UART_PARITY_SP_1) \
|| (parity==UART_PARITY_SP_0))

/** Macro to check the input UART_FIFO parameters */
#define PARAM_UART_FIFO_LEVEL(fifo)	((fifo==UART_FIFO_TRGLEV0) \
|| (fifo==UART_FIFO_TRGLEV1) || (fifo==UART_FIFO_TRGLEV2) \
|| (fifo==UART_FIFO_TRGLEV3))

/** Macro to check the input UART_INTCFG parameters */
#define PARAM_UART_INTCFG(IntCfg)	((IntCfg==UART_INTCFG_RBR) || (IntCfg==UART_INTCFG_THRE) \
|| (IntCfg==UART_INTCFG_RLS) || (IntCfg==UART_INTCFG_ABEO) \
|| (IntCfg==UART_INTCFG_ABTO))

/** Macro to check the input UART1_INTCFG parameters - expansion input parameter for UART1 */
#define PARAM_UART1_INTCFG(IntCfg)	((IntCfg==UART1_INTCFG_MS) || (IntCfg==UART1_INTCFG_CTS))

/** Macro to check the input UART_AUTOBAUD_MODE parameters */
#define PARAM_UART_AUTOBAUD_MODE(ABmode)	((ABmode==UART_AUTOBAUD_MODE0) || (ABmode==UART_AUTOBAUD_MODE1))

/** Macro to check the input UART_AUTOBAUD_INTSTAT parameters */
#define PARAM_UART_AUTOBAUD_INTSTAT(ABIntStat)	((ABIntStat==UART_AUTOBAUD_INTSTAT_ABEO) || \
		(ABIntStat==UART_AUTOBAUD_INTSTAT_ABTO))

/** Macro to check the input UART_IrDA_PULSEDIV parameters */
#define PARAM_UART_IrDA_PULSEDIV(PulseDiv)	((PulseDiv==UART_IrDA_PULSEDIV2) || (PulseDiv==UART_IrDA_PULSEDIV4) \
|| (PulseDiv==UART_IrDA_PULSEDIV8) || (PulseDiv==UART_IrDA_PULSEDIV16) \
|| (PulseDiv==UART_IrDA_PULSEDIV32) || (PulseDiv==UART_IrDA_PULSEDIV64) \
|| (PulseDiv==UART_IrDA_PULSEDIV128) || (PulseDiv==UART_IrDA_PULSEDIV256))

/* Macro to check the input UART1_SignalState parameters */
#define PARAM_UART1_SIGNALSTATE(x) ((x==INACTIVE) || (x==ACTIVE))

/** Macro to check the input PARAM_UART1_MODEM_PIN parameters */
#define PARAM_UART1_MODEM_PIN(x) ((x==UART1_MODEM_PIN_DTR) || (x==UART1_MODEM_PIN_RTS))

/** Macro to check the input PARAM_UART1_MODEM_MODE parameters */
#define PARAM_UART1_MODEM_MODE(x) ((x==UART1_MODEM_MODE_LOOPBACK) || (x==UART1_MODEM_MODE_AUTO_RTS) \
|| (x==UART1_MODEM_MODE_AUTO_CTS))

/** Macro to check the direction control pin type */
#define PARAM_UART_RS485_DIRCTRL_PIN(x)	((x==UART1_RS485_DIRCTRL_RTS) || (x==UART1_RS485_DIRCTRL_DTR))

/* Macro to determine if it is valid UART port number */
#define PARAM_UARTx(x)	((((uint32_t *)x)==((uint32_t *)LPC_UART0)) \
|| (((uint32_t *)x)==((uint32_t *)LPC_UART1)) \
|| (((uint32_t *)x)==((uint32_t *)LPC_UART2)) \
|| (((uint32_t *)x)==((uint32_t *)LPC_UART3)))
#define PARAM_UART_IrDA(x) (((uint32_t *)x)==((uint32_t *)LPC_UART3))
#define PARAM_UART1_MODEM(x) (((uint32_t *)x)==((uint32_t *)LPC_UART1))

/** Macro to check the input value for UART1_RS485_CFG_MATCHADDRVALUE parameter */
#define PARAM_UART1_RS485_CFG_MATCHADDRVALUE(x) ((x<0xFF))

/** Macro to check the input value for UART1_RS485_CFG_DELAYVALUE parameter */
#define PARAM_UART1_RS485_CFG_DELAYVALUE(x) ((x<0xFF))

/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup UART_Public_Types UART Public Types
 * @{
 */

/**
 * @brief UART Databit type definitions
 */
typedef enum {
	UART_DATABIT_5		= 0,     		/*!< UART 5 bit data mode */
	UART_DATABIT_6,		     			/*!< UART 6 bit data mode */
	UART_DATABIT_7,		     			/*!< UART 7 bit data mode */
	UART_DATABIT_8		     			/*!< UART 8 bit data mode */
} UART_DATABIT_Type;

/**
 * @brief UART Stop bit type definitions
 */
typedef enum {
	UART_STOPBIT_1		= (0),   					/*!< UART 1 Stop Bits Select */
	UART_STOPBIT_2		 							/*!< UART Two Stop Bits Select */
} UART_STOPBIT_Type;

/**
 * @brief UART Parity type definitions
 */
typedef enum {
	UART_PARITY_NONE 	= 0,					/*!< No parity */
	UART_PARITY_ODD,	 						/*!< Odd parity */
	UART_PARITY_EVEN, 							/*!< Even parity */
	UART_PARITY_SP_1, 							/*!< Forced "1" stick parity */
	UART_PARITY_SP_0 							/*!< Forced "0" stick parity */
} UART_PARITY_Type;

/**
 * @brief FIFO Level type definitions
 */
typedef enum {
	UART_FIFO_TRGLEV0 = 0,	/*!< UART FIFO trigger level 0: 1 character */
	UART_FIFO_TRGLEV1, 		/*!< UART FIFO trigger level 1: 4 character */
	UART_FIFO_TRGLEV2,		/*!< UART FIFO trigger level 2: 8 character */
	UART_FIFO_TRGLEV3		/*!< UART FIFO trigger level 3: 14 character */
} UART_FITO_LEVEL_Type;

/********************************************************************//**
* @brief UART Interrupt Type definitions
**********************************************************************/
typedef enum {
	UART_INTCFG_RBR = 0,	/*!< RBR Interrupt enable*/
	UART_INTCFG_THRE,		/*!< THR Interrupt enable*/
	UART_INTCFG_RLS,		/*!< RX line status interrupt enable*/
	UART1_INTCFG_MS,		/*!< Modem status interrupt enable (UART1 only) */
	UART1_INTCFG_CTS,		/*!< CTS1 signal transition interrupt enable (UART1 only) */
	UART_INTCFG_ABEO,		/*!< Enables the end of auto-baud interrupt */
	UART_INTCFG_ABTO		/*!< Enables the auto-baud time-out interrupt */
} UART_INT_Type;

/**
 * @brief UART Line Status Type definition
 */
typedef enum {
	UART_LINESTAT_RDR	= UART_LSR_RDR,			/*!<Line status register: Receive data ready*/
	UART_LINESTAT_OE	= UART_LSR_OE,			/*!<Line status register: Overrun error*/
	UART_LINESTAT_PE	= UART_LSR_PE,			/*!<Line status register: Parity error*/
	UART_LINESTAT_FE	= UART_LSR_FE,			/*!<Line status register: Framing error*/
	UART_LINESTAT_BI	= UART_LSR_BI,			/*!<Line status register: Break interrupt*/
	UART_LINESTAT_THRE	= UART_LSR_THRE,		/*!<Line status register: Transmit holding register empty*/
	UART_LINESTAT_TEMT	= UART_LSR_TEMT,		/*!<Line status register: Transmitter empty*/
	UART_LINESTAT_RXFE	= UART_LSR_RXFE			/*!<Error in RX FIFO*/
} UART_LS_Type;

/**
 * @brief UART Auto-baudrate mode type definition
 */
typedef enum {
	UART_AUTOBAUD_MODE0				= 0,			/**< UART Auto baudrate Mode 0 */
	UART_AUTOBAUD_MODE1							/**< UART Auto baudrate Mode 1 */
} UART_AB_MODE_Type;

/**
 * @brief Auto Baudrate mode configuration type definition
 */
typedef struct {
	UART_AB_MODE_Type	ABMode;			/**< Autobaudrate mode */
	FunctionalState		AutoRestart;	/**< Auto Restart state */
} UART_AB_CFG_Type;

/**
 * @brief UART End of Auto-baudrate type definition
 */
typedef enum {
	UART_AUTOBAUD_INTSTAT_ABEO		= UART_IIR_ABEO_INT,		/**< UART End of auto-baud interrupt  */
	UART_AUTOBAUD_INTSTAT_ABTO		= UART_IIR_ABTO_INT			/**< UART Auto-baud time-out interrupt  */
}UART_ABEO_Type;

/**
 * UART IrDA Control type Definition
 */
typedef enum {
	UART_IrDA_PULSEDIV2		= 0,		/**< Pulse width = 2 * Tpclk
										- Configures the pulse when FixPulseEn = 1 */
	UART_IrDA_PULSEDIV4,				/**< Pulse width = 4 * Tpclk
										- Configures the pulse when FixPulseEn = 1 */
	UART_IrDA_PULSEDIV8,				/**< Pulse width = 8 * Tpclk
										- Configures the pulse when FixPulseEn = 1 */
	UART_IrDA_PULSEDIV16,				/**< Pulse width = 16 * Tpclk
										- Configures the pulse when FixPulseEn = 1 */
	UART_IrDA_PULSEDIV32,				/**< Pulse width = 32 * Tpclk
										- Configures the pulse when FixPulseEn = 1 */
	UART_IrDA_PULSEDIV64,				/**< Pulse width = 64 * Tpclk
										- Configures the pulse when FixPulseEn = 1 */
	UART_IrDA_PULSEDIV128,				/**< Pulse width = 128 * Tpclk
										- Configures the pulse when FixPulseEn = 1 */
	UART_IrDA_PULSEDIV256				/**< Pulse width = 256 * Tpclk
										- Configures the pulse when FixPulseEn = 1 */
} UART_IrDA_PULSE_Type;

/********************************************************************//**
* @brief UART1 Full modem -  Signal states definition
**********************************************************************/
typedef enum {
	INACTIVE = 0,			/* In-active state */
	ACTIVE = !INACTIVE 		/* Active state */
}UART1_SignalState;

/**
 * @brief UART modem status type definition
 */
typedef enum {
	UART1_MODEM_STAT_DELTA_CTS	= UART1_MSR_DELTA_CTS,		/*!< Set upon state change of input CTS */
	UART1_MODEM_STAT_DELTA_DSR	= UART1_MSR_DELTA_DSR,		/*!< Set upon state change of input DSR */
	UART1_MODEM_STAT_LO2HI_RI	= UART1_MSR_LO2HI_RI,		/*!< Set upon low to high transition of input RI */
	UART1_MODEM_STAT_DELTA_DCD	= UART1_MSR_DELTA_DCD,		/*!< Set upon state change of input DCD */
	UART1_MODEM_STAT_CTS		= UART1_MSR_CTS,			/*!< Clear To Send State */
	UART1_MODEM_STAT_DSR		= UART1_MSR_DSR,			/*!< Data Set Ready State */
	UART1_MODEM_STAT_RI			= UART1_MSR_RI,				/*!< Ring Indicator State */
	UART1_MODEM_STAT_DCD		= UART1_MSR_DCD				/*!< Data Carrier Detect State */
} UART_MODEM_STAT_type;

/**
 * @brief Modem output pin type definition
 */
typedef enum {
	UART1_MODEM_PIN_DTR			= 0,		/*!< Source for modem output pin DTR */
	UART1_MODEM_PIN_RTS						/*!< Source for modem output pin RTS */
} UART_MODEM_PIN_Type;

/**
 * @brief UART Modem mode type definition
 */
typedef enum {
	UART1_MODEM_MODE_LOOPBACK	= 0,		/*!< Loop back mode select */
	UART1_MODEM_MODE_AUTO_RTS,				/*!< Enable Auto RTS flow-control */
	UART1_MODEM_MODE_AUTO_CTS 				/*!< Enable Auto CTS flow-control */
} UART_MODEM_MODE_Type;

/**
 * @brief UART Direction Control Pin type definition
 */
typedef enum {
	UART1_RS485_DIRCTRL_RTS = 0,	/**< Pin RTS is used for direction control */
	UART1_RS485_DIRCTRL_DTR			/**< Pin DTR is used for direction control */
} UART_RS485_DIRCTRL_PIN_Type;

/********************************************************************//**
* @brief UART Configuration Structure definition
**********************************************************************/
typedef struct {
  uint32_t Baud_rate;   		/**< UART baud rate */
  UART_PARITY_Type Parity;    	/**< Parity selection, should be:
							   - UART_PARITY_NONE: No parity
							   - UART_PARITY_ODD: Odd parity
							   - UART_PARITY_EVEN: Even parity
							   - UART_PARITY_SP_1: Forced "1" stick parity
							   - UART_PARITY_SP_0: Forced "0" stick parity
							   */
  UART_DATABIT_Type Databits;   /**< Number of data bits, should be:
							   - UART_DATABIT_5: UART 5 bit data mode
							   - UART_DATABIT_6: UART 6 bit data mode
							   - UART_DATABIT_7: UART 7 bit data mode
							   - UART_DATABIT_8: UART 8 bit data mode
							   */
  UART_STOPBIT_Type Stopbits;   /**< Number of stop bits, should be:
							   - UART_STOPBIT_1: UART 1 Stop Bits Select
							   - UART_STOPBIT_2: UART 2 Stop Bits Select
							   */
} UART_CFG_Type;

/********************************************************************//**
* @brief UART FIFO Configuration Structure definition
**********************************************************************/

typedef struct {
	FunctionalState FIFO_ResetRxBuf;	/**< Reset Rx FIFO command state , should be:
										 - ENABLE: Reset Rx FIFO in UART
										 - DISABLE: Do not reset Rx FIFO  in UART
										 */
	FunctionalState FIFO_ResetTxBuf;	/**< Reset Tx FIFO command state , should be:
										 - ENABLE: Reset Tx FIFO in UART
										 - DISABLE: Do not reset Tx FIFO  in UART
										 */
	FunctionalState FIFO_DMAMode;		/**< DMA mode, should be:
										 - ENABLE: Enable DMA mode in UART
										 - DISABLE: Disable DMA mode in UART
										 */
	UART_FITO_LEVEL_Type FIFO_Level;	/**< Rx FIFO trigger level, should be:
										- UART_FIFO_TRGLEV0: UART FIFO trigger level 0: 1 character
										- UART_FIFO_TRGLEV1: UART FIFO trigger level 1: 4 character
										- UART_FIFO_TRGLEV2: UART FIFO trigger level 2: 8 character
										- UART_FIFO_TRGLEV3: UART FIFO trigger level 3: 14 character
										*/
} UART_FIFO_CFG_Type;

/********************************************************************//**
* @brief UART1 Full modem -  RS485 Control configuration type
**********************************************************************/
typedef struct {
	FunctionalState NormalMultiDropMode_State; /*!< Normal MultiDrop mode State:
													- ENABLE: Enable this function.
													- DISABLE: Disable this function. */
	FunctionalState Rx_State;					/*!< Receiver State:
													- ENABLE: Enable Receiver.
													- DISABLE: Disable Receiver. */
	FunctionalState AutoAddrDetect_State;		/*!< Auto Address Detect mode state:
												- ENABLE: ENABLE this function.
												- DISABLE: Disable this function. */
	FunctionalState AutoDirCtrl_State;			/*!< Auto Direction Control State:
												- ENABLE: Enable this function.
												- DISABLE: Disable this function. */
	UART_RS485_DIRCTRL_PIN_Type DirCtrlPin;		/*!< If direction control is enabled, state:
												- UART1_RS485_DIRCTRL_RTS:
												pin RTS is used for direction control.
												- UART1_RS485_DIRCTRL_DTR:
												pin DTR is used for direction control. */
	 SetState DirCtrlPol_Level;					/*!< Polarity of the direction control signal on
												the RTS (or DTR) pin:
												- RESET: The direction control pin will be driven
												to logic "0" when the transmitter has data to be sent.
												- SET: The direction control pin will be driven
												to logic "1" when the transmitter has data to be sent. */
	uint8_t MatchAddrValue;					/*!< address match value for RS-485/EIA-485 mode, 8-bit long */
	uint8_t DelayValue;						/*!< delay time is in periods of the baud clock, 8-bit long */
} UART1_RS485_CTRLCFG_Type;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup UART_Public_Functions UART Public Functions
 * @{
 */
/* UART Init/DeInit functions --------------------------------------------------*/
void UART_Init(LPC_UART_TypeDef *UARTx, UART_CFG_Type *UART_ConfigStruct);
void UART_DeInit(LPC_UART_TypeDef* UARTx);
void UART_ConfigStructInit(UART_CFG_Type *UART_InitStruct);

/* UART Send/Receive functions -------------------------------------------------*/
void UART_SendByte(LPC_UART_TypeDef* UARTx, uint8_t Data);
uint8_t UART_ReceiveByte(LPC_UART_TypeDef* UARTx);
uint32_t UART_Send(LPC_UART_TypeDef *UARTx, uint8_t *txbuf,
		uint32_t buflen, TRANSFER_BLOCK_Type flag);
uint32_t UART_Receive(LPC_UART_TypeDef *UARTx, uint8_t *rxbuf, \
		uint32_t buflen, TRANSFER_BLOCK_Type flag);

/* UART FIFO functions ----------------------------------------------------------*/
void UART_FIFOConfig(LPC_UART_TypeDef *UARTx, UART_FIFO_CFG_Type *FIFOCfg);
void UART_FIFOConfigStructInit(UART_FIFO_CFG_Type *UART_FIFOInitStruct);

/* UART get information functions -----------------------------------------------*/
uint32_t UART_GetIntId(LPC_UART_TypeDef* UARTx);
uint8_t UART_GetLineStatus(LPC_UART_TypeDef* UARTx);

/* UART operate functions -------------------------------------------------------*/
void UART_IntConfig(LPC_UART_TypeDef *UARTx, UART_INT_Type UARTIntCfg, \
				FunctionalState NewState);
void UART_TxCmd(LPC_UART_TypeDef *UARTx, FunctionalState NewState);
FlagStatus UART_CheckBusy(LPC_UART_TypeDef *UARTx);
void UART_ForceBreak(LPC_UART_TypeDef* UARTx);

/* UART Auto-baud functions -----------------------------------------------------*/
void UART_ABClearIntPending(LPC_UART_TypeDef *UARTx, UART_ABEO_Type ABIntType);
void UART_ABCmd(LPC_UART_TypeDef *UARTx, UART_AB_CFG_Type *ABConfigStruct, \
				FunctionalState NewState);

/* UART1 FullModem functions ----------------------------------------------------*/
void UART_FullModemForcePinState(LPC_UART1_TypeDef *UARTx, UART_MODEM_PIN_Type Pin, \
							UART1_SignalState NewState);
void UART_FullModemConfigMode(LPC_UART1_TypeDef *UARTx, UART_MODEM_MODE_Type Mode, \
							FunctionalState NewState);
uint8_t UART_FullModemGetStatus(LPC_UART1_TypeDef *UARTx);

/* UART RS485 functions ----------------------------------------------------------*/
void UART_RS485Config(LPC_UART1_TypeDef *UARTx, \
		UART1_RS485_CTRLCFG_Type *RS485ConfigStruct);
void UART_RS485ReceiverCmd(LPC_UART1_TypeDef *UARTx, FunctionalState NewState);
void UART_RS485SendSlvAddr(LPC_UART1_TypeDef *UARTx, uint8_t SlvAddr);
uint32_t UART_RS485SendData(LPC_UART1_TypeDef *UARTx, uint8_t *pData, uint32_t size);

/* UART IrDA functions-------------------------------------------------------------*/
void UART_IrDAInvtInputCmd(LPC_UART_TypeDef* UARTx, FunctionalState NewState);
void UART_IrDACmd(LPC_UART_TypeDef* UARTx, FunctionalState NewState);
void UART_IrDAPulseDivConfig(LPC_UART_TypeDef *UARTx, UART_IrDA_PULSE_Type PulseDiv);
/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __LPC17XX_UART_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
