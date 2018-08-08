/**********************************************************************
* $Id$		lpc17xx_can.h			2010-06-18
*//**
* @file		lpc17xx_can.h
* @brief	Contains all macro definitions and function prototypes
* 			support for CAN firmware library on LPC17xx
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
/** @defgroup CAN CAN (Control Area Network)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_CAN_H_
#define LPC17XX_CAN_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Types --------------------------------------------------------------- */
/** @defgroup CAN_Public_Macros CAN Public Macros
 * @{
 */
#define MSG_ENABLE				((uint8_t)(0))
#define MSG_DISABLE				((uint8_t)(1))
#define CAN1_CTRL				((uint8_t)(0))
#define CAN2_CTRL				((uint8_t)(1))
#define PARAM_FULLCAN_IC(n)		((n==FULLCAN_IC0)||(n==FULLCAN_IC1))
#define ID_11					1
#define MAX_HW_FULLCAN_OBJ 		64
#define MAX_SW_FULLCAN_OBJ 		32

/**
 * @}
 */

/* Private Macros ------------------------------------------------------------- */
/** @defgroup CAN_Private_Macros CAN Private Macros
 * @{
 */

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for CAN Mode Register
 **********************************************************************/
/** CAN Reset mode */
#define CAN_MOD_RM			((uint32_t)(1))
/** CAN Listen Only Mode */
#define CAN_MOD_LOM			((uint32_t)(1<<1))
/** CAN Self Test mode */
#define CAN_MOD_STM			((uint32_t)(1<<2))
/** CAN Transmit Priority mode */
#define CAN_MOD_TPM			((uint32_t)(1<<3))
/** CAN Sleep mode */
#define CAN_MOD_SM			((uint32_t)(1<<4))
/** CAN Receive Polarity mode */
#define CAN_MOD_RPM			((uint32_t)(1<<5))
/** CAN Test mode */
#define CAN_MOD_TM			((uint32_t)(1<<7))

/*********************************************************************//**
 * Macro defines for CAN Command Register
 **********************************************************************/
/** CAN Transmission Request */
#define CAN_CMR_TR			((uint32_t)(1))
/** CAN Abort Transmission */
#define CAN_CMR_AT			((uint32_t)(1<<1))
/** CAN Release Receive Buffer */
#define CAN_CMR_RRB			((uint32_t)(1<<2))
/** CAN Clear Data Overrun */
#define CAN_CMR_CDO			((uint32_t)(1<<3))
/** CAN Self Reception Request */
#define CAN_CMR_SRR			((uint32_t)(1<<4))
/** CAN Select Tx Buffer 1 */
#define CAN_CMR_STB1		((uint32_t)(1<<5))
/** CAN Select Tx Buffer 2 */
#define CAN_CMR_STB2		((uint32_t)(1<<6))
/** CAN Select Tx Buffer 3 */
#define CAN_CMR_STB3		((uint32_t)(1<<7))

/*********************************************************************//**
 * Macro defines for CAN Global Status Register
 **********************************************************************/
/** CAN Receive Buffer Status */
#define CAN_GSR_RBS			((uint32_t)(1))
/** CAN Data Overrun Status */
#define CAN_GSR_DOS			((uint32_t)(1<<1))
/** CAN Transmit Buffer Status */
#define CAN_GSR_TBS			((uint32_t)(1<<2))
/** CAN Transmit Complete Status */
#define CAN_GSR_TCS			((uint32_t)(1<<3))
/** CAN Receive Status */
#define CAN_GSR_RS			((uint32_t)(1<<4))
/** CAN Transmit Status */
#define CAN_GSR_TS			((uint32_t)(1<<5))
/** CAN Error Status */
#define CAN_GSR_ES			((uint32_t)(1<<6))
/** CAN Bus Status */
#define CAN_GSR_BS			((uint32_t)(1<<7))
/** CAN Current value of the Rx Error Counter */
#define CAN_GSR_RXERR(n)	((uint32_t)((n&0xFF)<<16))
/** CAN Current value of the Tx Error Counter */
#define CAN_GSR_TXERR(n)	((uint32_t)(n&0xFF)<<24))

/*********************************************************************//**
 * Macro defines for CAN Interrupt and Capture Register
 **********************************************************************/
/** CAN Receive Interrupt */
#define CAN_ICR_RI			((uint32_t)(1))
/** CAN Transmit Interrupt 1 */
#define CAN_ICR_TI1			((uint32_t)(1<<1))
/** CAN Error Warning Interrupt */
#define CAN_ICR_EI			((uint32_t)(1<<2))
/** CAN Data Overrun Interrupt */
#define CAN_ICR_DOI			((uint32_t)(1<<3))
/** CAN Wake-Up Interrupt */
#define CAN_ICR_WUI			((uint32_t)(1<<4))
/** CAN Error Passive Interrupt */
#define CAN_ICR_EPI			((uint32_t)(1<<5))
/** CAN Arbitration Lost Interrupt */
#define CAN_ICR_ALI			((uint32_t)(1<<6))
/** CAN Bus Error Interrupt */
#define CAN_ICR_BEI			((uint32_t)(1<<7))
/** CAN ID Ready Interrupt */
#define CAN_ICR_IDI			((uint32_t)(1<<8))
/** CAN Transmit Interrupt 2 */
#define CAN_ICR_TI2			((uint32_t)(1<<9))
/** CAN Transmit Interrupt 3 */
#define CAN_ICR_TI3			((uint32_t)(1<<10))
/** CAN Error Code Capture */
#define CAN_ICR_ERRBIT(n)	((uint32_t)((n&0x1F)<<16))
/** CAN Error Direction */
#define CAN_ICR_ERRDIR		((uint32_t)(1<<21))
/** CAN Error Capture */
#define CAN_ICR_ERRC(n)		((uint32_t)((n&0x3)<<22))
/** CAN Arbitration Lost Capture */
#define CAN_ICR_ALCBIT(n)		((uint32_t)((n&0xFF)<<24))

/*********************************************************************//**
 * Macro defines for CAN Interrupt Enable Register
 **********************************************************************/
/** CAN Receive Interrupt Enable */
#define CAN_IER_RIE			((uint32_t)(1))
/** CAN Transmit Interrupt Enable for buffer 1 */
#define CAN_IER_TIE1		((uint32_t)(1<<1))
/** CAN Error Warning Interrupt Enable */
#define CAN_IER_EIE			((uint32_t)(1<<2))
/** CAN Data Overrun Interrupt Enable */
#define CAN_IER_DOIE		((uint32_t)(1<<3))
/** CAN Wake-Up Interrupt Enable */
#define CAN_IER_WUIE		((uint32_t)(1<<4))
/** CAN Error Passive Interrupt Enable */
#define CAN_IER_EPIE		((uint32_t)(1<<5))
/** CAN Arbitration Lost Interrupt Enable */
#define CAN_IER_ALIE		((uint32_t)(1<<6))
/** CAN Bus Error Interrupt Enable */
#define CAN_IER_BEIE		((uint32_t)(1<<7))
/** CAN ID Ready Interrupt Enable */
#define CAN_IER_IDIE		((uint32_t)(1<<8))
/** CAN Transmit Enable Interrupt for Buffer 2 */
#define CAN_IER_TIE2		((uint32_t)(1<<9))
/** CAN Transmit Enable Interrupt for Buffer 3 */
#define CAN_IER_TIE3		((uint32_t)(1<<10))

/*********************************************************************//**
 * Macro defines for CAN Bus Timing Register
 **********************************************************************/
/** CAN Baudrate Prescaler */
#define CAN_BTR_BRP(n)		((uint32_t)(n&0x3FF))
/** CAN Synchronization Jump Width */
#define CAN_BTR_SJM(n)		((uint32_t)((n&0x3)<<14))
/** CAN Time Segment 1 */
#define CAN_BTR_TESG1(n)	((uint32_t)(n&0xF)<<16))
/** CAN Time Segment 2 */
#define CAN_BTR_TESG2(n)	((uint32_t)(n&0xF)<<20))
/** CAN Sampling */
#define CAN_BTR_SAM(n)		((uint32_t)(1<<23))

/*********************************************************************//**
 * Macro defines for CAN Error Warning Limit Register
 **********************************************************************/
/** CAN Error Warning Limit */
#define CAN_EWL_EWL(n)		((uint32_t)(n&0xFF))

/*********************************************************************//**
 * Macro defines for CAN Status Register
 **********************************************************************/
/** CAN Receive Buffer Status */
#define CAN_SR_RBS		((uint32_t)(1))
/** CAN Data Overrun Status */
#define CAN_SR_DOS		((uint32_t)(1<<1))
/** CAN Transmit Buffer Status 1 */
#define CAN_SR_TBS1		((uint32_t)(1<<2))
/** CAN Transmission Complete Status of Buffer 1 */
#define CAN_SR_TCS1		((uint32_t)(1<<3))
/** CAN Receive Status */
#define CAN_SR_RS		((uint32_t)(1<<4))
/** CAN Transmit Status 1 */
#define CAN_SR_TS1		((uint32_t)(1<<5))
/** CAN Error Status */
#define CAN_SR_ES		((uint32_t)(1<<6))
/** CAN Bus Status */
#define CAN_SR_BS		((uint32_t)(1<<7))
/** CAN Transmit Buffer Status 2 */
#define CAN_SR_TBS2		((uint32_t)(1<<10))
/** CAN Transmission Complete Status of Buffer 2 */
#define CAN_SR_TCS2		((uint32_t)(1<<11))
/** CAN Transmit Status 2 */
#define CAN_SR_TS2		((uint32_t)(1<<13))
/** CAN Transmit Buffer Status 2 */
#define CAN_SR_TBS3		((uint32_t)(1<<18))
/** CAN Transmission Complete Status of Buffer 2 */
#define CAN_SR_TCS3		((uint32_t)(1<<19))
/** CAN Transmit Status 2 */
#define CAN_SR_TS3		((uint32_t)(1<<21))

/*********************************************************************//**
 * Macro defines for CAN Receive Frame Status Register
 **********************************************************************/
/** CAN ID Index */
#define CAN_RFS_ID_INDEX(n)	((uint32_t)(n&0x3FF))
/** CAN Bypass */
#define CAN_RFS_BP			((uint32_t)(1<<10))
/** CAN Data Length Code */
#define CAN_RFS_DLC(n)		((uint32_t)((n&0xF)<<16)
/** CAN Remote Transmission Request */
#define CAN_RFS_RTR			((uint32_t)(1<<30))
/** CAN control 11 bit or 29 bit Identifier */
#define CAN_RFS_FF			((uint32_t)(1<<31))

/*********************************************************************//**
 * Macro defines for CAN Receive Identifier Register
 **********************************************************************/
/** CAN 11 bit Identifier */
#define CAN_RID_ID_11(n)		((uint32_t)(n&0x7FF))
/** CAN 29 bit Identifier */
#define CAN_RID_ID_29(n)		((uint32_t)(n&0x1FFFFFFF))

/*********************************************************************//**
 * Macro defines for CAN Receive Data A Register
 **********************************************************************/
/** CAN Receive Data 1 */
#define CAN_RDA_DATA1(n)		((uint32_t)(n&0xFF))
/** CAN Receive Data 2 */
#define CAN_RDA_DATA2(n)		((uint32_t)((n&0xFF)<<8))
/** CAN Receive Data 3 */
#define CAN_RDA_DATA3(n)		((uint32_t)((n&0xFF)<<16))
/** CAN Receive Data 4 */
#define CAN_RDA_DATA4(n)		((uint32_t)((n&0xFF)<<24))

/*********************************************************************//**
 * Macro defines for CAN Receive Data B Register
 **********************************************************************/
/** CAN Receive Data 5 */
#define CAN_RDB_DATA5(n)		((uint32_t)(n&0xFF))
/** CAN Receive Data 6 */
#define CAN_RDB_DATA6(n)		((uint32_t)((n&0xFF)<<8))
/** CAN Receive Data 7 */
#define CAN_RDB_DATA7(n)		((uint32_t)((n&0xFF)<<16))
/** CAN Receive Data 8 */
#define CAN_RDB_DATA8(n)		((uint32_t)((n&0xFF)<<24))

/*********************************************************************//**
 * Macro defines for CAN Transmit Frame Information Register
 **********************************************************************/
/** CAN Priority */
#define CAN_TFI_PRIO(n)			((uint32_t)(n&0xFF))
/** CAN Data Length Code */
#define CAN_TFI_DLC(n)			((uint32_t)((n&0xF)<<16))
/** CAN Remote Frame Transmission */
#define CAN_TFI_RTR				((uint32_t)(1<<30))
/** CAN control 11-bit or 29-bit Identifier */
#define CAN_TFI_FF				((uint32_t)(1<<31))

/*********************************************************************//**
 * Macro defines for CAN Transmit Identifier Register
 **********************************************************************/
/** CAN 11-bit Identifier */
#define CAN_TID_ID11(n)			((uint32_t)(n&0x7FF))
/** CAN 11-bit Identifier */
#define CAN_TID_ID29(n)			((uint32_t)(n&0x1FFFFFFF))

/*********************************************************************//**
 * Macro defines for CAN Transmit Data A Register
 **********************************************************************/
/** CAN Transmit Data 1 */
#define CAN_TDA_DATA1(n)		((uint32_t)(n&0xFF))
/** CAN Transmit Data 2 */
#define CAN_TDA_DATA2(n)		((uint32_t)((n&0xFF)<<8))
/** CAN Transmit Data 3 */
#define CAN_TDA_DATA3(n)		((uint32_t)((n&0xFF)<<16))
/** CAN Transmit Data 4 */
#define CAN_TDA_DATA4(n)		((uint32_t)((n&0xFF)<<24))

/*********************************************************************//**
 * Macro defines for CAN Transmit Data B Register
 **********************************************************************/
/** CAN Transmit Data 5 */
#define CAN_TDA_DATA5(n)		((uint32_t)(n&0xFF))
/** CAN Transmit Data 6 */
#define CAN_TDA_DATA6(n)		((uint32_t)((n&0xFF)<<8))
/** CAN Transmit Data 7 */
#define CAN_TDA_DATA7(n)		((uint32_t)((n&0xFF)<<16))
/** CAN Transmit Data 8 */
#define CAN_TDA_DATA8(n)		((uint32_t)((n&0xFF)<<24))

/*********************************************************************//**
 * Macro defines for CAN Sleep Clear Register
 **********************************************************************/
/** CAN1 Sleep mode */
#define CAN1SLEEPCLR			((uint32_t)(1<<1))
/** CAN2 Sleep Mode */
#define CAN2SLEEPCLR			((uint32_t)(1<<2))

/*********************************************************************//**
 * Macro defines for CAN Wake up Flags Register
 **********************************************************************/
/** CAN1 Sleep mode */
#define CAN_WAKEFLAGES_CAN1WAKE		((uint32_t)(1<<1))
/** CAN2 Sleep Mode */
#define CAN_WAKEFLAGES_CAN2WAKE		((uint32_t)(1<<2))

/*********************************************************************//**
 * Macro defines for Central transmit Status Register
 **********************************************************************/
/** CAN Transmit 1 */
#define CAN_TSR_TS1			((uint32_t)(1))
/** CAN Transmit 2 */
#define CAN_TSR_TS2			((uint32_t)(1<<1))
/** CAN Transmit Buffer Status 1 */
#define CAN_TSR_TBS1			((uint32_t)(1<<8))
/** CAN Transmit Buffer Status 2 */
#define CAN_TSR_TBS2			((uint32_t)(1<<9))
/** CAN Transmission Complete Status 1 */
#define CAN_TSR_TCS1			((uint32_t)(1<<16))
/** CAN Transmission Complete Status 2 */
#define CAN_TSR_TCS2			((uint32_t)(1<<17))

/*********************************************************************//**
 * Macro defines for Central Receive Status Register
 **********************************************************************/
/** CAN Receive Status 1 */
#define CAN_RSR_RS1				((uint32_t)(1))
/** CAN Receive Status 1 */
#define CAN_RSR_RS2				((uint32_t)(1<<1))
/** CAN Receive Buffer Status 1*/
#define CAN_RSR_RB1				((uint32_t)(1<<8))
/** CAN Receive Buffer Status 2*/
#define CAN_RSR_RB2				((uint32_t)(1<<9))
/** CAN Data Overrun Status 1 */
#define CAN_RSR_DOS1			((uint32_t)(1<<16))
/** CAN Data Overrun Status 1 */
#define CAN_RSR_DOS2			((uint32_t)(1<<17))

/*********************************************************************//**
 * Macro defines for Central Miscellaneous Status Register
 **********************************************************************/
/** Same CAN Error Status in CAN1GSR */
#define CAN_MSR_E1		((uint32_t)(1))
/** Same CAN Error Status in CAN2GSR */
#define CAN_MSR_E2		((uint32_t)(1<<1))
/** Same CAN Bus Status in CAN1GSR */
#define CAN_MSR_BS1		((uint32_t)(1<<8))
/** Same CAN Bus Status in CAN2GSR */
#define CAN_MSR_BS2		((uint32_t)(1<<9))

/*********************************************************************//**
 * Macro defines for Acceptance Filter Mode Register
 **********************************************************************/
/** CAN Acceptance Filter Off mode */
#define CAN_AFMR_AccOff		((uint32_t)(1))
/** CAN Acceptance File Bypass mode */
#define CAN_AFMR_AccBP		((uint32_t)(1<<1))
/** FullCAN Mode Enhancements */
#define CAN_AFMR_eFCAN		((uint32_t)(1<<2))

/*********************************************************************//**
 * Macro defines for Standard Frame Individual Start Address Register
 **********************************************************************/
/** The start address of the table of individual Standard Identifier */
#define CAN_STT_sa(n)		((uint32_t)((n&1FF)<<2))

/*********************************************************************//**
 * Macro defines for Standard Frame Group Start Address Register
 **********************************************************************/
/** The start address of the table of grouped Standard Identifier */
#define CAN_SFF_GRP_sa(n)		((uint32_t)((n&3FF)<<2))

/*********************************************************************//**
 * Macro defines for Extended Frame Start Address Register
 **********************************************************************/
/** The start address of the table of individual Extended Identifier */
#define CAN_EFF_sa(n)		((uint32_t)((n&1FF)<<2))

/*********************************************************************//**
 * Macro defines for Extended Frame Group Start Address Register
 **********************************************************************/
/** The start address of the table of grouped Extended Identifier */
#define CAN_Eff_GRP_sa(n)		((uint32_t)((n&3FF)<<2))

/*********************************************************************//**
 * Macro defines for End Of AF Table Register
 **********************************************************************/
/** The End of Table of AF LookUp Table */
#define CAN_EndofTable(n)		((uint32_t)((n&3FF)<<2))

/*********************************************************************//**
 * Macro defines for LUT Error Address Register
 **********************************************************************/
/** CAN Look-Up Table Error Address */
#define CAN_LUTerrAd(n)		((uint32_t)((n&1FF)<<2))

/*********************************************************************//**
 * Macro defines for LUT Error Register
 **********************************************************************/
/** CAN Look-Up Table Error */
#define CAN_LUTerr		((uint32_t)(1))

/*********************************************************************//**
 * Macro defines for Global FullCANInterrupt Enable Register
 **********************************************************************/
/** Global FullCANInterrupt Enable */
#define CAN_FCANIE		((uint32_t)(1))

/*********************************************************************//**
 * Macro defines for FullCAN Interrupt and Capture Register 0
 **********************************************************************/
/** FullCAN Interrupt and Capture (0-31)*/
#define CAN_FCANIC0_IntPnd(n)	((uint32_t)(1<<n))

/*********************************************************************//**
 * Macro defines for FullCAN Interrupt and Capture Register 1
 **********************************************************************/
/** FullCAN Interrupt and Capture (0-31)*/
#define CAN_FCANIC1_IntPnd(n)	((uint32_t)(1<<(n-32)))


/* ---------------- CHECK PARAMETER DEFINITIONS ---------------------------- */
/** Macro to determine if it is valid CAN peripheral or not */
#define PARAM_CANx(x)			((((uint32_t*)x)==((uint32_t *)LPC_CAN1)) \
||(((uint32_t*)x)==((uint32_t *)LPC_CAN2)))

/*	Macro to determine if it is valid CANAF or not*/
#define PARAM_CANAFx(x)			(((uint32_t*)x)== ((uint32_t*)LPC_CANAF))

/*	Macro to determine if it is valid CANAF RAM or not*/
#define PARAM_CANAFRAMx(x)		(((uint32_t*)x)== (uint32_t*)LPC_CANAF_RAM)

/*	Macro to determine if it is valid CANCR or not*/
#define PARAM_CANCRx(x)			(((uint32_t*)x)==((uint32_t*)LPC_CANCR))

/** Macro to check Data to send valid */
#define PARAM_I2S_DATA(data) 	((data>=0)&&(data <= 0xFFFFFFFF))

/** Macro to check frequency value */
#define PRAM_I2S_FREQ(freq)		((freq>=16000)&&(freq <= 96000))

/** Macro to check Frame Identifier */
#define PARAM_ID_11(n)			((n>>11)==0) /*-- 11 bit --*/
#define PARAM_ID_29(n)			((n>>29)==0) /*-- 29 bit --*/

/** Macro to check DLC value */
#define PARAM_DLC(n)			((n>>4)==0)  /*-- 4 bit --*/
/** Macro to check ID format type */
#define PARAM_ID_FORMAT(n)		((n==STD_ID_FORMAT)||(n==EXT_ID_FORMAT))

/** Macro to check Group identifier */
#define PARAM_GRP_ID(x, y)		((x<=y))

/** Macro to check Frame type */
#define PARAM_FRAME_TYPE(n)		((n==DATA_FRAME)||(n==REMOTE_FRAME))

/** Macro to check Control/Central Status type parameter */
#define PARAM_CTRL_STS_TYPE(n)	((n==CANCTRL_GLOBAL_STS)||(n==CANCTRL_INT_CAP) \
||(n==CANCTRL_ERR_WRN)||(n==CANCTRL_STS))

/** Macro to check CR status type */
#define PARAM_CR_STS_TYPE(n)	((n==CANCR_TX_STS)||(n==CANCR_RX_STS) \
||(n==CANCR_MS))
/** Macro to check AF Mode type parameter */
#define PARAM_AFMODE_TYPE(n)	((n==CAN_Normal)||(n==CAN_AccOff) \
||(n==CAN_AccBP)||(n==CAN_eFCAN))

/** Macro to check Operation Mode */
#define PARAM_MODE_TYPE(n)		((n==CAN_OPERATING_MODE)||(n==CAN_RESET_MODE) \
||(n==CAN_LISTENONLY_MODE)||(n==CAN_SELFTEST_MODE) \
||(n==CAN_TXPRIORITY_MODE)||(n==CAN_SLEEP_MODE) \
||(n==CAN_RXPOLARITY_MODE)||(n==CAN_TEST_MODE))

/** Macro define for struct AF_Section parameter */
#define PARAM_CTRL(n)	((n==CAN1_CTRL)|(n==CAN2_CTRL))

/** Macro define for struct AF_Section parameter */
#define PARAM_MSG_DISABLE(n)	((n==MSG_ENABLE)|(n==MSG_DISABLE))

/**Macro to check Interrupt Type parameter */
#define PARAM_INT_EN_TYPE(n)	((n==CANINT_RIE)||(n==CANINT_TIE1) \
||(n==CANINT_EIE)||(n==CANINT_DOIE) \
||(n==CANINT_WUIE)||(n==CANINT_EPIE) \
||(n==CANINT_ALIE)||(n==CANINT_BEIE) \
||(n==CANINT_IDIE)||(n==CANINT_TIE2) \
||(n==CANINT_TIE3)||(n==CANINT_FCE))

/** Macro to check AFLUT Entry type */
#define PARAM_AFLUT_ENTRY_TYPE(n)	((n==FULLCAN_ENTRY)||(n==EXPLICIT_STANDARD_ENTRY)\
||(n==GROUP_STANDARD_ENTRY)||(n==EXPLICIT_EXTEND_ENTRY)	\
||(n==GROUP_EXTEND_ENTRY))

/** Macro to check position */
#define PARAM_POSITION(n)	(n<512)

/**
 * @}
 */

/* Public Types --------------------------------------------------------------- */
/** @defgroup CAN_Public_Types CAN Public Types
 * @{
 */

/** CAN configuration structure */
/***********************************************************************
 * CAN device configuration commands (IOCTL commands and arguments)
 **********************************************************************/
/**
 * @brief CAN ID format definition
 */
typedef enum {
	STD_ID_FORMAT = 0, 	/**< Use standard ID format (11 bit ID) */
	EXT_ID_FORMAT = 1	/**< Use extended ID format (29 bit ID) */
} CAN_ID_FORMAT_Type;

/**
 * @brief AFLUT Entry type definition
 */
typedef enum {
	FULLCAN_ENTRY = 0,
	EXPLICIT_STANDARD_ENTRY,
	GROUP_STANDARD_ENTRY,
	EXPLICIT_EXTEND_ENTRY,
	GROUP_EXTEND_ENTRY
} AFLUT_ENTRY_Type;

/**
 * @brief Symbolic names for type of CAN message
 */
typedef enum {
	DATA_FRAME = 0, 	/**< Data frame */
	REMOTE_FRAME = 1	/**< Remote frame */
} CAN_FRAME_Type;

/**
 * @brief CAN Control status definition
 */
typedef enum {
	CANCTRL_GLOBAL_STS = 0, /**< CAN Global Status */
	CANCTRL_INT_CAP, 		/**< CAN Interrupt and Capture */
	CANCTRL_ERR_WRN, 		/**< CAN Error Warning Limit */
	CANCTRL_STS				/**< CAN Control Status */
} CAN_CTRL_STS_Type;

/**
 * @brief Central CAN status type definition
 */
typedef enum {
	CANCR_TX_STS = 0, 	/**< Central CAN Tx Status */
	CANCR_RX_STS, 		/**< Central CAN Rx Status */
	CANCR_MS			/**< Central CAN Miscellaneous Status */
} CAN_CR_STS_Type;

/**
 * @brief FullCAN Interrupt Capture type definition
 */
typedef enum{
	FULLCAN_IC0,	/**< FullCAN Interrupt and Capture 0 */
	FULLCAN_IC1	/**< FullCAN Interrupt and Capture 1 */
}FullCAN_IC_Type;

/**
 * @brief CAN interrupt enable type definition
 */
typedef enum {
	CANINT_RIE = 0, 	/**< CAN Receiver Interrupt Enable */
	CANINT_TIE1, 		/**< CAN Transmit Interrupt Enable */
	CANINT_EIE, 		/**< CAN Error Warning Interrupt Enable */
	CANINT_DOIE, 		/**< CAN Data Overrun Interrupt Enable */
	CANINT_WUIE, 		/**< CAN Wake-Up Interrupt Enable */
	CANINT_EPIE, 		/**< CAN Error Passive Interrupt Enable */
	CANINT_ALIE, 		/**< CAN Arbitration Lost Interrupt Enable */
	CANINT_BEIE, 		/**< CAN Bus Error Inter rupt Enable */
	CANINT_IDIE, 		/**< CAN ID Ready Interrupt Enable */
	CANINT_TIE2, 		/**< CAN Transmit Interrupt Enable for Buffer2 */
	CANINT_TIE3, 		/**< CAN Transmit Interrupt Enable for Buffer3 */
	CANINT_FCE			/**< FullCAN Interrupt Enable */
} CAN_INT_EN_Type;

/**
 * @brief Acceptance Filter Mode type definition
 */
typedef enum {
	CAN_Normal = 0, 	/**< Normal Mode */
	CAN_AccOff, 		/**< Acceptance Filter Off Mode */
	CAN_AccBP, 			/**< Acceptance Fileter Bypass Mode */
	CAN_eFCAN			/**< FullCAN Mode Enhancement */
} CAN_AFMODE_Type;

/**
 * @brief CAN Mode Type definition
 */
typedef enum {
	CAN_OPERATING_MODE = 0, 	/**< Operating Mode */
	CAN_RESET_MODE, 			/**< Reset Mode */
	CAN_LISTENONLY_MODE, 		/**< Listen Only Mode */
	CAN_SELFTEST_MODE, 			/**< Seft Test Mode */
	CAN_TXPRIORITY_MODE, 		/**< Transmit Priority Mode */
	CAN_SLEEP_MODE, 			/**< Sleep Mode */
	CAN_RXPOLARITY_MODE, 		/**< Receive Polarity Mode */
	CAN_TEST_MODE				/**< Test Mode */
} CAN_MODE_Type;

/**
 * @brief Error values that functions can return
 */
typedef enum {
	CAN_OK = 1, 				/**< No error */
	CAN_OBJECTS_FULL_ERROR, 	/**< No more rx or tx objects available */
	CAN_FULL_OBJ_NOT_RCV, 		/**< Full CAN object not received */
	CAN_NO_RECEIVE_DATA, 		/**< No have receive data available */
	CAN_AF_ENTRY_ERROR, 		/**< Entry load in AFLUT is unvalid */
	CAN_CONFLICT_ID_ERROR, 		/**< Conflict ID occur */
	CAN_ENTRY_NOT_EXIT_ERROR	/**< Entry remove outo AFLUT is not exit */
} CAN_ERROR;

/**
 * @brief Pin Configuration structure
 */
typedef struct {
	uint8_t RD; 			/**< Serial Inputs, from CAN transceivers, should be:
							 ** For CAN1:
							 - CAN_RD1_P0_0: RD pin is on P0.0
							 - CAN_RD1_P0_21 : RD pin is on P0.21
							 ** For CAN2:
							 - CAN_RD2_P0_4: RD pin is on P0.4
							 - CAN_RD2_P2_7: RD pin is on P2.7
							 */
	uint8_t TD;				/**< Serial Outputs, To CAN transceivers, should be:
							 ** For CAN1:
							 - CAN_TD1_P0_1: TD pin is on P0.1
							 - CAN_TD1_P0_22: TD pin is on P0.22
							 ** For CAN2:
							 - CAN_TD2_P0_5: TD pin is on P0.5
							 - CAN_TD2_P2_8: TD pin is on P2.8
							 */
} CAN_PinCFG_Type;

/**
 * @brief CAN message object structure
 */
typedef struct {
	uint32_t id; 			/**< 29 bit identifier, it depend on "format" value
								 - if format = STD_ID_FORMAT, id should be 11 bit identifier
								 - if format = EXT_ID_FORMAT, id should be 29 bit identifier
							 */
	uint8_t dataA[4]; 		/**< Data field A */
	uint8_t dataB[4]; 		/**< Data field B */
	uint8_t len; 			/**< Length of data field in bytes, should be:
								 - 0000b-0111b: 0-7 bytes
								 - 1xxxb: 8 bytes
							*/
	uint8_t format; 		/**< Identifier Format, should be:
								 - STD_ID_FORMAT: Standard ID - 11 bit format
								 - EXT_ID_FORMAT: Extended ID - 29 bit format
							*/
	uint8_t type; 			/**< Remote Frame transmission, should be:
								 - DATA_FRAME: the number of data bytes called out by the DLC
								 field are send from the CANxTDA and CANxTDB registers
								 - REMOTE_FRAME: Remote Frame is sent
							*/
} CAN_MSG_Type;

/**
 * @brief FullCAN Entry structure
 */
typedef struct {
	uint8_t controller;		/**< CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	uint8_t disable;		/**< Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0
								 - MSG_DISABLE: disable bit = 1
							*/
	uint16_t id_11;			/**< Standard ID, should be 11-bit value */
} FullCAN_Entry;

/**
 * @brief Standard ID Frame Format Entry structure
 */
typedef struct {
	uint8_t controller; 	/**< CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	uint8_t disable; 		/**< Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0
								 - MSG_DISABLE: disable bit = 1
							*/
	uint16_t id_11; 		/**< Standard ID, should be 11-bit value */
} SFF_Entry;

/**
 * @brief Group of Standard ID Frame Format Entry structure
 */
typedef struct {
	uint8_t controller1; 	/**< First CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	uint8_t disable1; 		/**< First Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0)
								 - MSG_DISABLE: disable bit = 1
							*/
	uint16_t lowerID; 		/**< ID lower bound, should be 11-bit value */
	uint8_t controller2; 	/**< Second CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	uint8_t disable2; 		/**< Second Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0
								 - MSG_DISABLE: disable bit = 1
							*/
	uint16_t upperID; 		/**< ID upper bound, should be 11-bit value and
								 equal or greater than lowerID
							*/
} SFF_GPR_Entry;

/**
 * @brief Extended ID Frame Format Entry structure
 */
typedef struct {
	uint8_t controller; 	/**< CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	uint32_t ID_29; 		/**< Extend ID, shoud be 29-bit value */
} EFF_Entry;


/**
 * @brief Group of Extended ID Frame Format Entry structure
 */
typedef struct {
	uint8_t controller1; 	/**< First CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	uint8_t controller2; 	/**< Second Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0(default)
								 - MSG_DISABLE: disable bit = 1
							*/
	uint32_t lowerEID; 		/**< Extended ID lower bound, should be 29-bit value */
	uint32_t upperEID; 		/**< Extended ID upper bound, should be 29-bit value */
} EFF_GPR_Entry;


/**
 * @brief Acceptance Filter Section Table structure
 */
typedef struct {
	FullCAN_Entry* FullCAN_Sec; 	/**< The pointer point to FullCAN_Entry */
	uint8_t FC_NumEntry;			/**< FullCAN Entry Number */
	SFF_Entry* SFF_Sec; 			/**< The pointer point to SFF_Entry */
	uint8_t SFF_NumEntry;			/**< Standard ID Entry Number */
	SFF_GPR_Entry* SFF_GPR_Sec; 	/**< The pointer point to SFF_GPR_Entry */
	uint8_t SFF_GPR_NumEntry;		/**< Group Standard ID Entry Number */
	EFF_Entry* EFF_Sec; 			/**< The pointer point to EFF_Entry */
	uint8_t EFF_NumEntry;			/**< Extended ID Entry Number */
	EFF_GPR_Entry* EFF_GPR_Sec; 	/**< The pointer point to EFF_GPR_Entry */
	uint8_t EFF_GPR_NumEntry;		/**< Group Extended ID Entry Number */
} AF_SectionDef;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup CAN_Public_Functions CAN Public Functions
 * @{
 */

/* Init/DeInit CAN peripheral -----------*/
void CAN_Init(LPC_CAN_TypeDef *CANx, uint32_t baudrate);
void CAN_DeInit(LPC_CAN_TypeDef *CANx);

/* CAN messages functions ---------------*/
Status CAN_SendMsg(LPC_CAN_TypeDef *CANx, CAN_MSG_Type *CAN_Msg);
Status CAN_ReceiveMsg(LPC_CAN_TypeDef *CANx, CAN_MSG_Type *CAN_Msg);
CAN_ERROR FCAN_ReadObj(LPC_CANAF_TypeDef* CANAFx, CAN_MSG_Type *CAN_Msg);

/* CAN configure functions ---------------*/
void CAN_ModeConfig(LPC_CAN_TypeDef* CANx, CAN_MODE_Type mode,
		FunctionalState NewState);
void CAN_SetAFMode(LPC_CANAF_TypeDef* CANAFx, CAN_AFMODE_Type AFmode);
void CAN_SetCommand(LPC_CAN_TypeDef* CANx, uint32_t CMRType);

/* AFLUT functions ---------------------- */
CAN_ERROR CAN_SetupAFLUT(LPC_CANAF_TypeDef* CANAFx, AF_SectionDef* AFSection);
CAN_ERROR CAN_LoadFullCANEntry(LPC_CAN_TypeDef* CANx, uint16_t ID);
CAN_ERROR CAN_LoadExplicitEntry(LPC_CAN_TypeDef* CANx, uint32_t ID,
		CAN_ID_FORMAT_Type format);
CAN_ERROR CAN_LoadGroupEntry(LPC_CAN_TypeDef* CANx, uint32_t lowerID,
		uint32_t upperID, CAN_ID_FORMAT_Type format);
CAN_ERROR CAN_RemoveEntry(AFLUT_ENTRY_Type EntryType, uint16_t position);

/* CAN interrupt functions -----------------*/
void CAN_IRQCmd(LPC_CAN_TypeDef* CANx, CAN_INT_EN_Type arg, FunctionalState NewState);
uint32_t CAN_IntGetStatus(LPC_CAN_TypeDef* CANx);

/* CAN get status functions ----------------*/
IntStatus CAN_FullCANIntGetStatus (LPC_CANAF_TypeDef* CANAFx);
uint32_t CAN_FullCANPendGetStatus (LPC_CANAF_TypeDef* CANAFx, FullCAN_IC_Type type);
uint32_t CAN_GetCTRLStatus(LPC_CAN_TypeDef* CANx, CAN_CTRL_STS_Type arg);
uint32_t CAN_GetCRStatus(LPC_CANCR_TypeDef* CANCRx, CAN_CR_STS_Type arg);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_CAN_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
