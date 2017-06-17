/**********************************************************************
* $Id$		lpc17xx_pwm.h				2011-03-31
*//**
* @file		lpc17xx_pwm.h
* @brief	Contains all macro definitions and function prototypes
* 			support for PWM firmware library on LPC17xx
* @version	2.1
* @date		31. Mar. 2011
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
/** @defgroup PWM PWM (Pulse Width Modulator)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_PWM_H_
#define LPC17XX_PWM_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Private Macros ------------------------------------------------------------- */
/** @defgroup PWM_Private_Macros PWM Private Macros
 * @{
 */

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/**********************************************************************
* IR register definitions
**********************************************************************/
/** Interrupt flag for PWM match channel for 6 channel */
#define PWM_IR_PWMMRn(n)    	((uint32_t)((n<4)?(1<<n):(1<<(n+4))))
/** Interrupt flag for capture input */
#define PWM_IR_PWMCAPn(n)		((uint32_t)(1<<(n+4)))
/**  IR register mask */
#define PWM_IR_BITMASK			((uint32_t)(0x0000073F))

/**********************************************************************
* TCR register definitions
**********************************************************************/
/** TCR register mask */
#define PWM_TCR_BITMASK				((uint32_t)(0x0000000B))
#define PWM_TCR_COUNTER_ENABLE      ((uint32_t)(1<<0)) /*!< PWM Counter Enable */
#define PWM_TCR_COUNTER_RESET       ((uint32_t)(1<<1)) /*!< PWM Counter Reset */
#define PWM_TCR_PWM_ENABLE          ((uint32_t)(1<<3)) /*!< PWM Enable */

/**********************************************************************
* CTCR register definitions
**********************************************************************/
/** CTCR register mask */
#define PWM_CTCR_BITMASK			((uint32_t)(0x0000000F))
/** PWM Counter-Timer Mode */
#define PWM_CTCR_MODE(n)        	((uint32_t)(n&0x03))
/** PWM Capture input select */
#define PWM_CTCR_SELECT_INPUT(n)	((uint32_t)((n&0x03)<<2))

/**********************************************************************
* MCR register definitions
**********************************************************************/
/** MCR register mask */
#define PWM_MCR_BITMASK				((uint32_t)(0x001FFFFF))
/** generate a PWM interrupt when a MATCHn occurs */
#define PWM_MCR_INT_ON_MATCH(n)     ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07))))
/** reset the PWM when a MATCHn occurs */
#define PWM_MCR_RESET_ON_MATCH(n)   ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07)+1)))
/** stop the PWM when a MATCHn occurs */
#define PWM_MCR_STOP_ON_MATCH(n)    ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07)+2)))

/**********************************************************************
* CCR register definitions
**********************************************************************/
/** CCR register mask */
#define PWM_CCR_BITMASK				((uint32_t)(0x0000003F))
/** PCAPn is rising edge sensitive */
#define PWM_CCR_CAP_RISING(n) 	 	((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1))))
/** PCAPn is falling edge sensitive */
#define PWM_CCR_CAP_FALLING(n) 		((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1)+1)))
/** PWM interrupt is generated on a PCAP event */
#define PWM_CCR_INT_ON_CAP(n)  		((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1)+2)))

/**********************************************************************
* PCR register definitions
**********************************************************************/
/** PCR register mask */
#define PWM_PCR_BITMASK			(uint32_t)0x00007E7C
/** PWM output n is a single edge controlled output */
#define PWM_PCR_PWMSELn(n)   	((uint32_t)(((n&0x7)<2) ? 0 : (1<<n)))
/** enable PWM output n */
#define PWM_PCR_PWMENAn(n)   	((uint32_t)(((n&0x7)<1) ? 0 : (1<<(n+8))))

/**********************************************************************
* LER register definitions
**********************************************************************/
/** LER register mask*/
#define PWM_LER_BITMASK				((uint32_t)(0x0000007F))
/** PWM MATCHn register update control */
#define PWM_LER_EN_MATCHn_LATCH(n)   ((uint32_t)((n<7) ? (1<<n) : 0))

/* ---------------- CHECK PARAMETER DEFINITIONS ---------------------------- */
/** Macro to determine if it is valid PWM peripheral or not */
#define PARAM_PWMx(n)	(((uint32_t *)n)==((uint32_t *)LPC_PWM1))

/** Macro check PWM1 match channel value */
#define PARAM_PWM1_MATCH_CHANNEL(n)		(n<=6)

/** Macro check PWM1 channel value */
#define PARAM_PWM1_CHANNEL(n)			((n>=1) && (n<=6))

/** Macro check PWM1 edge channel mode */
#define PARAM_PWM1_EDGE_MODE_CHANNEL(n)			((n>=2) && (n<=6))

/** Macro check PWM1 capture channel mode */
#define PARAM_PWM1_CAPTURE_CHANNEL(n)	((n==0) || (n==1))

/** Macro check PWM1 interrupt status type */
#define PARAM_PWM_INTSTAT(n)	((n==PWM_INTSTAT_MR0) || (n==PWM_INTSTAT_MR1) || (n==PWM_INTSTAT_MR2) \
|| (n==PWM_INTSTAT_MR3) || (n==PWM_INTSTAT_MR4) || (n==PWM_INTSTAT_MR5) \
|| (n==PWM_INTSTAT_MR6) || (n==PWM_INTSTAT_CAP0) || (n==PWM_INTSTAT_CAP1))
/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup PWM_Public_Types PWM Public Types
 * @{
 */

/** @brief Configuration structure in PWM TIMER mode */
typedef struct {

	uint8_t PrescaleOption;		/**< Prescale option, should be:
								- PWM_TIMER_PRESCALE_TICKVAL: Prescale in absolute value
								- PWM_TIMER_PRESCALE_USVAL: Prescale in microsecond value
								*/
	uint8_t Reserved[3];
	uint32_t PrescaleValue;		/**< Prescale value, 32-bit long, should be matched
								with PrescaleOption
								*/
} PWM_TIMERCFG_Type;

/** @brief Configuration structure in PWM COUNTER mode */
typedef struct {

	uint8_t CounterOption;		/**< Counter Option, should be:
								- PWM_COUNTER_RISING: Rising Edge
								- PWM_COUNTER_FALLING: Falling Edge
								- PWM_COUNTER_ANY: Both rising and falling mode
								*/
	uint8_t CountInputSelect;	/**< Counter input select, should be:
								- PWM_COUNTER_PCAP1_0: PWM Counter input selected is PCAP1.0 pin
								- PWM_COUNTER_PCAP1_1: PWM Counter input selected is PCAP1.1 pin
								*/
	uint8_t Reserved[2];
} PWM_COUNTERCFG_Type;

/** @brief PWM Match channel configuration structure */
typedef struct {
	uint8_t MatchChannel;	/**< Match channel, should be in range
							from 0..6 */
	uint8_t IntOnMatch;		/**< Interrupt On match, should be:
							- ENABLE: Enable this function.
							- DISABLE: Disable this function.
							*/
	uint8_t StopOnMatch;	/**< Stop On match, should be:
							- ENABLE: Enable this function.
							- DISABLE: Disable this function.
							*/
	uint8_t ResetOnMatch;	/**< Reset On match, should be:
							- ENABLE: Enable this function.
							- DISABLE: Disable this function.
							*/
} PWM_MATCHCFG_Type;


/** @brief PWM Capture Input configuration structure */
typedef struct {
	uint8_t CaptureChannel;	/**< Capture channel, should be in range
							from 0..1 */
	uint8_t RisingEdge;		/**< caption rising edge, should be:
							- ENABLE: Enable rising edge.
							- DISABLE: Disable this function.
							*/
	uint8_t FallingEdge;		/**< caption falling edge, should be:
							- ENABLE: Enable falling edge.
							- DISABLE: Disable this function.
								*/
	uint8_t IntOnCaption;	/**< Interrupt On caption, should be:
							- ENABLE: Enable interrupt function.
							- DISABLE: Disable this function.
							*/
} PWM_CAPTURECFG_Type;

/* Timer/Counter in PWM configuration type definition -----------------------------------*/

/** @brief PMW TC mode select option */
typedef enum {
	PWM_MODE_TIMER = 0,		/*!< PWM using Timer mode */
	PWM_MODE_COUNTER		/*!< PWM using Counter mode */
} PWM_TC_MODE_OPT;

#define PARAM_PWM_TC_MODE(n) ((n==PWM_MODE_TIMER) || (n==PWM_MODE_COUNTER))


/** @brief PWM Timer/Counter prescale option */
typedef enum
{
	PWM_TIMER_PRESCALE_TICKVAL = 0,			/*!< Prescale in absolute value */
	PWM_TIMER_PRESCALE_USVAL				/*!< Prescale in microsecond value */
} PWM_TIMER_PRESCALE_OPT;

#define PARAM_PWM_TIMER_PRESCALE(n) ((n==PWM_TIMER_PRESCALE_TICKVAL) || (n==PWM_TIMER_PRESCALE_USVAL))


/** @brief PWM Input Select in counter mode */
typedef enum {
	PWM_COUNTER_PCAP1_0 = 0,		/*!< PWM Counter input selected is PCAP1.0 pin */
	PWM_COUNTER_PCAP1_1			/*!< PWM counter input selected is CAP1.1 pin */
} PWM_COUNTER_INPUTSEL_OPT;

#define PARAM_PWM_COUNTER_INPUTSEL(n) ((n==PWM_COUNTER_PCAP1_0) || (n==PWM_COUNTER_PCAP1_1))

/** @brief PWM Input Edge Option in counter mode */
typedef enum {
    PWM_COUNTER_RISING = 1,		/*!< Rising edge mode */
    PWM_COUNTER_FALLING = 2,	/*!< Falling edge mode */
    PWM_COUNTER_ANY = 3			/*!< Both rising and falling mode */
} PWM_COUNTER_EDGE_OPT;

#define PARAM_PWM_COUNTER_EDGE(n)	((n==PWM_COUNTER_RISING) || (n==PWM_COUNTER_FALLING) \
|| (n==PWM_COUNTER_ANY))


/* PWM configuration type definition ----------------------------------------------------- */
/** @brief PWM operating mode options */
typedef enum {
    PWM_CHANNEL_SINGLE_EDGE,	/*!< PWM Channel Single edge mode */
    PWM_CHANNEL_DUAL_EDGE		/*!< PWM Channel Dual edge mode */
} PWM_CHANNEL_EDGE_OPT;

#define PARAM_PWM_CHANNEL_EDGE(n)	((n==PWM_CHANNEL_SINGLE_EDGE) || (n==PWM_CHANNEL_DUAL_EDGE))


/** @brief PWM update type */
typedef enum {
	PWM_MATCH_UPDATE_NOW = 0,			/**< PWM Match Channel Update Now */
	PWM_MATCH_UPDATE_NEXT_RST			/**< PWM Match Channel Update on next
											PWM Counter resetting */
} PWM_MATCH_UPDATE_OPT;

#define PARAM_PWM_MATCH_UPDATE(n)	((n==PWM_MATCH_UPDATE_NOW) || (n==PWM_MATCH_UPDATE_NEXT_RST))


/** @brief PWM interrupt status type definition ----------------------------------------------------- */
/** @brief PWM Interrupt status type */
typedef enum
{
	PWM_INTSTAT_MR0 = PWM_IR_PWMMRn(0), 	/**< Interrupt flag for PWM match channel 0 */
	PWM_INTSTAT_MR1 = PWM_IR_PWMMRn(1),		/**< Interrupt flag for PWM match channel 1 */
	PWM_INTSTAT_MR2 = PWM_IR_PWMMRn(2),		/**< Interrupt flag for PWM match channel 2 */
	PWM_INTSTAT_MR3 = PWM_IR_PWMMRn(3),		/**< Interrupt flag for PWM match channel 3 */
	PWM_INTSTAT_CAP0 = PWM_IR_PWMCAPn(0),	/**< Interrupt flag for capture input 0 */
	PWM_INTSTAT_CAP1 = PWM_IR_PWMCAPn(1),	/**< Interrupt flag for capture input 1 */
	PWM_INTSTAT_MR4 = PWM_IR_PWMMRn(4),		/**< Interrupt flag for PWM match channel 4 */
	PWM_INTSTAT_MR6 = PWM_IR_PWMMRn(5),		/**< Interrupt flag for PWM match channel 5 */
	PWM_INTSTAT_MR5 = PWM_IR_PWMMRn(6)		/**< Interrupt flag for PWM match channel 6 */
}PWM_INTSTAT_TYPE;

/** @brief Match update structure */
typedef struct
{
	uint32_t Matchvalue;
	FlagStatus Status;
}PWM_Match_T;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup PWM_Public_Functions PWM Public Functions
 * @{
 */

void PWM_PinConfig(LPC_PWM_TypeDef *PWMx, uint8_t PWM_Channel, uint8_t PinselOption);
IntStatus PWM_GetIntStatus(LPC_PWM_TypeDef *PWMx, uint32_t IntFlag);
void PWM_ClearIntPending(LPC_PWM_TypeDef *PWMx, uint32_t IntFlag);
void PWM_ConfigStructInit(uint8_t PWMTimerCounterMode, void *PWM_InitStruct);
void PWM_Init(LPC_PWM_TypeDef *PWMx, uint32_t PWMTimerCounterMode, void *PWM_ConfigStruct);
void PWM_DeInit (LPC_PWM_TypeDef *PWMx);
void PWM_Cmd(LPC_PWM_TypeDef *PWMx, FunctionalState NewState);
void PWM_CounterCmd(LPC_PWM_TypeDef *PWMx, FunctionalState NewState);
void PWM_ResetCounter(LPC_PWM_TypeDef *PWMx);
void PWM_ConfigMatch(LPC_PWM_TypeDef *PWMx, PWM_MATCHCFG_Type *PWM_MatchConfigStruct);
void PWM_ConfigCapture(LPC_PWM_TypeDef *PWMx, PWM_CAPTURECFG_Type *PWM_CaptureConfigStruct);
uint32_t PWM_GetCaptureValue(LPC_PWM_TypeDef *PWMx, uint8_t CaptureChannel);
void PWM_MatchUpdate(LPC_PWM_TypeDef *PWMx, uint8_t MatchChannel, \
					uint32_t MatchValue, uint8_t UpdateType);
void PWM_ChannelConfig(LPC_PWM_TypeDef *PWMx, uint8_t PWMChannel, uint8_t ModeOption);
void PWM_ChannelCmd(LPC_PWM_TypeDef *PWMx, uint8_t PWMChannel, FunctionalState NewState);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_PWM_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
