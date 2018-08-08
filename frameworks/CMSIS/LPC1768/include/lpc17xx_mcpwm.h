/**********************************************************************
* $Id$		lpc17xx_mcpwm.h				2010-05-21
*//**
* @file		lpc17xx_mcpwm.h
* @brief	Contains all macro definitions and function prototypes
* 			support for Motor Control PWM firmware library on LPC17xx
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
/** @defgroup MCPWM MCPWM (Motor Control PWM)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_MCPWM_H_
#define LPC17XX_MCPWM_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup MCPWM_Public_Macros MCPWM Public Macros
 * @{
 */


/** Edge aligned mode for channel in MCPWM */
#define MCPWM_CHANNEL_EDGE_MODE			((uint32_t)(0))
/** Center aligned mode for channel in MCPWM */
#define MCPWM_CHANNEL_CENTER_MODE		((uint32_t)(1))

/** Polarity of the MCOA and MCOB pins: Passive state is LOW, active state is HIGH */
#define MCPWM_CHANNEL_PASSIVE_LO		((uint32_t)(0))
/** Polarity of the MCOA and MCOB pins: Passive state is HIGH, active state is LOW */
#define MCPWM_CHANNEL_PASSIVE_HI		((uint32_t)(1))

/* Output Patent in 3-phase DC mode, the internal MCOA0 signal is routed to any or all of
 * the six output pins under the control of the bits in this register */
#define MCPWM_PATENT_A0		((uint32_t)(1<<0))	/**< MCOA0 tracks internal MCOA0 */
#define MCPWM_PATENT_B0		((uint32_t)(1<<1))	/**< MCOB0 tracks internal MCOA0 */
#define MCPWM_PATENT_A1		((uint32_t)(1<<2))	/**< MCOA1 tracks internal MCOA0 */
#define MCPWM_PATENT_B1		((uint32_t)(1<<3))	/**< MCOB1 tracks internal MCOA0 */
#define MCPWM_PATENT_A2		((uint32_t)(1<<4))	/**< MCOA2 tracks internal MCOA0 */
#define MCPWM_PATENT_B2		((uint32_t)(1<<5))	/**< MCOB2 tracks internal MCOA0 */

/* Interrupt type in MCPWM */
/** Limit interrupt for channel (0) */
#define MCPWM_INTFLAG_LIM0	MCPWM_INT_ILIM(0)
/** Match interrupt for channel (0) */
#define MCPWM_INTFLAG_MAT0	MCPWM_INT_IMAT(0)
/** Capture interrupt for channel (0) */
#define MCPWM_INTFLAG_CAP0	MCPWM_INT_ICAP(0)

/** Limit interrupt for channel (1) */
#define MCPWM_INTFLAG_LIM1	MCPWM_INT_ILIM(1)
/** Match interrupt for channel (1) */
#define MCPWM_INTFLAG_MAT1	MCPWM_INT_IMAT(1)
/** Capture interrupt for channel (1) */
#define MCPWM_INTFLAG_CAP1	MCPWM_INT_ICAP(1)

/** Limit interrupt for channel (2) */
#define MCPWM_INTFLAG_LIM2	MCPWM_INT_ILIM(2)
/** Match interrupt for channel (2) */
#define MCPWM_INTFLAG_MAT2	MCPWM_INT_IMAT(2)
/** Capture interrupt for channel (2) */
#define MCPWM_INTFLAG_CAP2	MCPWM_INT_ICAP(2)

/** Fast abort interrupt */
#define MCPWM_INTFLAG_ABORT	MCPWM_INT_ABORT

/**
 * @}
 */

/* Private Macros ------------------------------------------------------------- */
/** @defgroup MCPWM_Private_Macros MCPWM Private Macros
 * @{
 */

/*********************************************************************//**
 * Macro defines for MCPWM Control register
 **********************************************************************/
/* MCPWM Control register, these macro definitions below can be applied for these
 * register type:
 * - MCPWM Control read address
 * - MCPWM Control set address
 * - MCPWM Control clear address
 */
#define MCPWM_CON_RUN(n)		((n<=2) ? ((uint32_t)(1<<((n*8)+0))) : (0))		/**< Stops/starts timer channel n */
#define MCPWM_CON_CENTER(n)		((n<=2) ? ((uint32_t)(1<<((n*8)+1))) : (0))		/**< Edge/center aligned operation for channel n */
#define MCPWM_CON_POLAR(n)		((n<=2) ? ((uint32_t)(1<<((n*8)+2))) : (0))		/**< Select polarity of the MCOAn and MCOBn pin */
#define MCPWM_CON_DTE(n)		((n<=2) ? ((uint32_t)(1<<((n*8)+3))) : (0))		/**< Control the dead-time feature for channel n */
#define MCPWM_CON_DISUP(n)		((n<=2) ? ((uint32_t)(1<<((n*8)+4))) : (0))		/**< Enable/Disable update of functional register for channel n */
#define MCPWM_CON_INVBDC		((uint32_t)(1<<29))								/**< Control the polarity for all 3 channels */
#define MCPWM_CON_ACMODE		((uint32_t)(1<<30))								/**< 3-phase AC mode select */
#define MCPWM_CON_DCMODE		((uint32_t)(0x80000000))						/**< 3-phase DC mode select */

/*********************************************************************//**
 * Macro defines for MCPWM Capture Control register
 **********************************************************************/
/* Capture Control register, these macro definitions below can be applied for these
 * register type:
 * - MCPWM Capture Control read address
 * - MCPWM Capture Control set address
 * - MCPWM Capture control clear address
 */
/** Enables/Disable channel (cap) capture event on a rising edge on MCI(mci) */
#define MCPWM_CAPCON_CAPMCI_RE(cap,mci)	(((cap<=2)&&(mci<=2)) ? ((uint32_t)(1<<((cap*6)+(mci*2)+0))) : (0))
/** Enables/Disable channel (cap) capture event on a falling edge on MCI(mci) */
#define MCPWM_CAPCON_CAPMCI_FE(cap,mci)	(((cap<=2)&&(mci<=2)) ? ((uint32_t)(1<<((cap*6)+(mci*2)+1))) : (0))
/** TC(n) is reset by channel (n) capture event */
#define MCPWM_CAPCON_RT(n)				((n<=2) ? ((uint32_t)(1<<(18+(n)))) : (0))
/** Hardware noise filter: channel (n) capture events are delayed */
#define MCPWM_CAPCON_HNFCAP(n)			((n<=2) ? ((uint32_t)(1<<(21+(n)))) : (0))

/*********************************************************************//**
 * Macro defines for MCPWM Interrupt register
 **********************************************************************/
/* Interrupt registers, these macro definitions below can be applied for these
 * register type:
 * - MCPWM Interrupt Enable read address
 * - MCPWM Interrupt Enable set address
 * - MCPWM Interrupt Enable clear address
 * - MCPWM Interrupt Flags read address
 * - MCPWM Interrupt Flags set address
 * - MCPWM Interrupt Flags clear address
 */
/** Limit interrupt for channel (n) */
#define MCPWM_INT_ILIM(n)	(((n>=0)&&(n<=2)) ? ((uint32_t)(1<<((n*4)+0))) : (0))
/** Match interrupt for channel (n) */
#define MCPWM_INT_IMAT(n)	(((n>=0)&&(n<=2)) ? ((uint32_t)(1<<((n*4)+1))) : (0))
/** Capture interrupt for channel (n) */
#define MCPWM_INT_ICAP(n)	(((n>=0)&&(n<=2)) ? ((uint32_t)(1<<((n*4)+2))) : (0))
/** Fast abort interrupt */
#define MCPWM_INT_ABORT		((uint32_t)(1<<15))

/*********************************************************************//**
 * Macro defines for MCPWM Count Control register
 **********************************************************************/
/* MCPWM Count Control register, these macro definitions below can be applied for these
 * register type:
 * - MCPWM Count Control read address
 * - MCPWM Count Control set address
 * - MCPWM Count Control clear address
 */
/** Counter(tc) advances on a rising edge on MCI(mci) pin */
#define MCPWM_CNTCON_TCMCI_RE(tc,mci)	(((tc<=2)&&(mci<=2)) ? ((uint32_t)(1<<((6*tc)+(2*mci)+0))) : (0))
/** Counter(cnt) advances on a falling edge on MCI(mci) pin */
#define MCPWM_CNTCON_TCMCI_FE(tc,mci)	(((tc<=2)&&(mci<=2)) ? ((uint32_t)(1<<((6*tc)+(2*mci)+1))) : (0))
/** Channel (n) is in counter mode */
#define MCPWM_CNTCON_CNTR(n)			((n<=2) ? ((uint32_t)(1<<(29+n))) : (0))

/*********************************************************************//**
 * Macro defines for MCPWM Dead-time register
 **********************************************************************/
/** Dead time value x for channel n */
#define MCPWM_DT(n,x)		((n<=2) ? ((uint32_t)((x&0x3FF)<<(n*10))) : (0))

/*********************************************************************//**
 * Macro defines for MCPWM Communication Pattern register
 **********************************************************************/
#define MCPWM_CP_A0		((uint32_t)(1<<0))	/**< MCOA0 tracks internal MCOA0 */
#define MCPWM_CP_B0		((uint32_t)(1<<1))	/**< MCOB0 tracks internal MCOA0 */
#define MCPWM_CP_A1		((uint32_t)(1<<2))	/**< MCOA1 tracks internal MCOA0 */
#define MCPWM_CP_B1		((uint32_t)(1<<3))	/**< MCOB1 tracks internal MCOA0 */
#define MCPWM_CP_A2		((uint32_t)(1<<4))	/**< MCOA2 tracks internal MCOA0 */
#define MCPWM_CP_B2		((uint32_t)(1<<5))	/**< MCOB2 tracks internal MCOA0 */

/*********************************************************************//**
 * Macro defines for MCPWM Capture clear address register
 **********************************************************************/
/** Clear the MCCAP (n) register */
#define MCPWM_CAPCLR_CAP(n)		((n<=2) ? ((uint32_t)(1<<n)) : (0))


/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup MCPWM_Public_Types MCPWM Public Types
 * @{
 */

/**
 * @brief Motor Control PWM Channel Configuration structure type definition
 */
typedef struct {
	uint32_t channelType;					/**< Edge/center aligned mode for this channel,
												should be:
												- MCPWM_CHANNEL_EDGE_MODE: Channel is in Edge mode
												- MCPWM_CHANNEL_CENTER_MODE: Channel is in Center mode
												*/
	uint32_t channelPolarity;				/**< Polarity of the MCOA and MCOB pins, should be:
												- MCPWM_CHANNEL_PASSIVE_LO: Passive state is LOW, active state is HIGH
												- MCPWM_CHANNEL_PASSIVE_HI: Passive state is HIGH, active state is LOW
												*/
	uint32_t channelDeadtimeEnable;			/**< Enable/Disable DeadTime function for channel, should be:
												- ENABLE.
												- DISABLE.
												*/
	uint32_t channelDeadtimeValue;			/**< DeadTime value, should be less than 0x3FF */
	uint32_t channelUpdateEnable;			/**< Enable/Disable updates of functional registers,
												 should be:
												- ENABLE.
												- DISABLE.
												*/
	uint32_t channelTimercounterValue;		/**< MCPWM Timer Counter value */
	uint32_t channelPeriodValue;			/**< MCPWM Period value */
	uint32_t channelPulsewidthValue;		/**< MCPWM Pulse Width value */
} MCPWM_CHANNEL_CFG_Type;

/**
 * @brief MCPWM Capture Configuration type definition
 */
typedef struct {
	uint32_t captureChannel;		/**< Capture Channel Number, should be in range from 0 to 2 */
	uint32_t captureRising;			/**< Enable/Disable Capture on Rising Edge event, should be:
										- ENABLE.
										- DISABLE.
										*/
	uint32_t captureFalling;		/**< Enable/Disable Capture on Falling Edge event, should be:
										- ENABLE.
										- DISABLE.
										*/
	uint32_t timerReset;			/**< Enable/Disable Timer reset function an capture, should be:
										- ENABLE.
										- DISABLE.
										*/
	uint32_t hnfEnable;				/**< Enable/Disable Hardware noise filter function, should be:
										- ENABLE.
										- DISABLE.
										*/
} MCPWM_CAPTURE_CFG_Type;


/**
 * @brief MCPWM Count Control Configuration type definition
 */
typedef struct {
	uint32_t counterChannel;		/**< Counter Channel Number, should be in range from 0 to 2 */
	uint32_t countRising;			/**< Enable/Disable Capture on Rising Edge event, should be:
										- ENABLE.
										- DISABLE.
										*/
	uint32_t countFalling;		/**< Enable/Disable Capture on Falling Edge event, should be:
										- ENABLE.
										- DISABLE.
										*/
} MCPWM_COUNT_CFG_Type;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup MCPWM_Public_Functions MCPWM Public Functions
 * @{
 */

void MCPWM_Init(LPC_MCPWM_TypeDef *MCPWMx);
void MCPWM_ConfigChannel(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channelNum,
						MCPWM_CHANNEL_CFG_Type * channelSetup);
void MCPWM_WriteToShadow(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channelNum,
						MCPWM_CHANNEL_CFG_Type *channelSetup);
void MCPWM_ConfigCapture(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channelNum,
						MCPWM_CAPTURE_CFG_Type *captureConfig);
void MCPWM_ClearCapture(LPC_MCPWM_TypeDef *MCPWMx, uint32_t captureChannel);
uint32_t MCPWM_GetCapture(LPC_MCPWM_TypeDef *MCPWMx, uint32_t captureChannel);
void MCPWM_CountConfig(LPC_MCPWM_TypeDef *MCPWMx, uint32_t channelNum,
					uint32_t countMode, MCPWM_COUNT_CFG_Type *countConfig);
void MCPWM_Start(LPC_MCPWM_TypeDef *MCPWMx,uint32_t channel0, uint32_t channel1, uint32_t channel2);
void MCPWM_Stop(LPC_MCPWM_TypeDef *MCPWMx,uint32_t channel0, uint32_t channel1, uint32_t channel2);
void MCPWM_ACMode(LPC_MCPWM_TypeDef *MCPWMx,uint32_t acMode);
void MCPWM_DCMode(LPC_MCPWM_TypeDef *MCPWMx, uint32_t dcMode,
					uint32_t outputInvered, uint32_t outputPattern);
void MCPWM_IntConfig(LPC_MCPWM_TypeDef *MCPWMx, uint32_t ulIntType, FunctionalState NewState);
void MCPWM_IntSet(LPC_MCPWM_TypeDef *MCPWMx, uint32_t ulIntType);
void MCPWM_IntClear(LPC_MCPWM_TypeDef *MCPWMx, uint32_t ulIntType);
FlagStatus MCPWM_GetIntStatus(LPC_MCPWM_TypeDef *MCPWMx, uint32_t ulIntType);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_MCPWM_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
