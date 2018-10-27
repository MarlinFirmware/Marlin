/**********************************************************************
* $Id$		lpc17xx_rtc.h				2010-05-21
*//**
* @file		lpc17xx_rtc.h
* @brief	Contains all macro definitions and function prototypes
* 			support for RTC firmware library on LPC17xx
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
/** @defgroup RTC RTC (Real Time Clock)
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_RTC_H_
#define LPC17XX_RTC_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Private Macros ------------------------------------------------------------- */
/** @defgroup RTC_Private_Macros RTC Private Macros
 * @{
 */

/* ----------------------- BIT DEFINITIONS ----------------------------------- */
/* Miscellaneous register group --------------------------------------------- */
/**********************************************************************
* ILR register definitions
**********************************************************************/
/** ILR register mask */
#define RTC_ILR_BITMASK			((0x00000003))
/** Bit inform the source interrupt is counter increment*/
#define RTC_IRL_RTCCIF			((1<<0))
/** Bit inform the source interrupt is alarm match*/
#define RTC_IRL_RTCALF			((1<<1))

/**********************************************************************
* CCR register definitions
**********************************************************************/
/** CCR register mask */
#define RTC_CCR_BITMASK			((0x00000013))
/** Clock enable */
#define RTC_CCR_CLKEN			((1<<0))
/** Clock reset */
#define RTC_CCR_CTCRST			((1<<1))
/** Calibration counter enable */
#define RTC_CCR_CCALEN			((1<<4))

/**********************************************************************
* CIIR register definitions
**********************************************************************/
/** Counter Increment Interrupt bit for second */
#define RTC_CIIR_IMSEC			((1<<0))
/** Counter Increment Interrupt bit for minute */
#define RTC_CIIR_IMMIN			((1<<1))
/** Counter Increment Interrupt bit for hour */
#define RTC_CIIR_IMHOUR			((1<<2))
/** Counter Increment Interrupt bit for day of month */
#define RTC_CIIR_IMDOM			((1<<3))
/** Counter Increment Interrupt bit for day of week */
#define RTC_CIIR_IMDOW			((1<<4))
/** Counter Increment Interrupt bit for day of year */
#define RTC_CIIR_IMDOY			((1<<5))
/** Counter Increment Interrupt bit for month */
#define RTC_CIIR_IMMON			((1<<6))
/** Counter Increment Interrupt bit for year */
#define RTC_CIIR_IMYEAR			((1<<7))
/** CIIR bit mask */
#define RTC_CIIR_BITMASK		((0xFF))

/**********************************************************************
* AMR register definitions
**********************************************************************/
/** Counter Increment Select Mask bit for second */
#define RTC_AMR_AMRSEC			((1<<0))
/** Counter Increment Select Mask bit for minute */
#define RTC_AMR_AMRMIN			((1<<1))
/** Counter Increment Select Mask bit for hour */
#define RTC_AMR_AMRHOUR			((1<<2))
/** Counter Increment Select Mask bit for day of month */
#define RTC_AMR_AMRDOM			((1<<3))
/** Counter Increment Select Mask bit for day of week */
#define RTC_AMR_AMRDOW			((1<<4))
/** Counter Increment Select Mask bit for day of year */
#define RTC_AMR_AMRDOY			((1<<5))
/** Counter Increment Select Mask bit for month */
#define RTC_AMR_AMRMON			((1<<6))
/** Counter Increment Select Mask bit for year */
#define RTC_AMR_AMRYEAR			((1<<7))
/** AMR bit mask */
#define RTC_AMR_BITMASK			((0xFF))

/**********************************************************************
* RTC_AUX register definitions
**********************************************************************/
/** RTC Oscillator Fail detect flag */
#define RTC_AUX_RTC_OSCF		((1<<4))

/**********************************************************************
* RTC_AUXEN register definitions
**********************************************************************/
/** Oscillator Fail Detect interrupt enable*/
#define RTC_AUXEN_RTC_OSCFEN	((1<<4))

/* Consolidated time register group ----------------------------------- */
/**********************************************************************
* Consolidated Time Register 0 definitions
**********************************************************************/
#define RTC_CTIME0_SECONDS_MASK		((0x3F))
#define RTC_CTIME0_MINUTES_MASK		((0x3F00))
#define RTC_CTIME0_HOURS_MASK		((0x1F0000))
#define RTC_CTIME0_DOW_MASK			((0x7000000))

/**********************************************************************
* Consolidated Time Register 1 definitions
**********************************************************************/
#define RTC_CTIME1_DOM_MASK			((0x1F))
#define RTC_CTIME1_MONTH_MASK		((0xF00))
#define RTC_CTIME1_YEAR_MASK		((0xFFF0000))

/**********************************************************************
* Consolidated Time Register 2 definitions
**********************************************************************/
#define RTC_CTIME2_DOY_MASK			((0xFFF))

/**********************************************************************
* Time Counter Group and Alarm register group
**********************************************************************/
/** SEC register mask */
#define RTC_SEC_MASK			(0x0000003F)
/** MIN register mask */
#define RTC_MIN_MASK			(0x0000003F)
/** HOUR register mask */
#define RTC_HOUR_MASK			(0x0000001F)
/** DOM register mask */
#define RTC_DOM_MASK			(0x0000001F)
/** DOW register mask */
#define RTC_DOW_MASK			(0x00000007)
/** DOY register mask */
#define RTC_DOY_MASK			(0x000001FF)
/** MONTH register mask */
#define RTC_MONTH_MASK			(0x0000000F)
/** YEAR register mask */
#define RTC_YEAR_MASK			(0x00000FFF)

#define RTC_SECOND_MAX		59 /*!< Maximum value of second */
#define RTC_MINUTE_MAX		59 /*!< Maximum value of minute*/
#define RTC_HOUR_MAX		23 /*!< Maximum value of hour*/
#define RTC_MONTH_MIN		1 /*!< Minimum value of month*/
#define RTC_MONTH_MAX		12 /*!< Maximum value of month*/
#define RTC_DAYOFMONTH_MIN 	1 /*!< Minimum value of day of month*/
#define RTC_DAYOFMONTH_MAX 	31 /*!< Maximum value of day of month*/
#define RTC_DAYOFWEEK_MAX	6 /*!< Maximum value of day of week*/
#define RTC_DAYOFYEAR_MIN	1 /*!< Minimum value of day of year*/
#define RTC_DAYOFYEAR_MAX	366 /*!< Maximum value of day of year*/
#define RTC_YEAR_MAX		4095 /*!< Maximum value of year*/

/**********************************************************************
* Calibration register
**********************************************************************/
/* Calibration register */
/** Calibration value */
#define RTC_CALIBRATION_CALVAL_MASK		((0x1FFFF))
/** Calibration direction */
#define RTC_CALIBRATION_LIBDIR			((1<<17))
/** Calibration max value */
#define RTC_CALIBRATION_MAX				((0x20000))
/** Calibration definitions */
#define RTC_CALIB_DIR_FORWARD			((uint8_t)(0))
#define RTC_CALIB_DIR_BACKWARD			((uint8_t)(1))


/* ---------------- CHECK PARAMETER DEFINITIONS ---------------------------- */
/** Macro to determine if it is valid RTC peripheral */
#define PARAM_RTCx(x)	(((uint32_t *)x)==((uint32_t *)LPC_RTC))

/* Macro check RTC interrupt type */
#define PARAM_RTC_INT(n)	((n==RTC_INT_COUNTER_INCREASE) || (n==RTC_INT_ALARM))

/* Macro check RTC time type */
#define PARAM_RTC_TIMETYPE(n)	((n==RTC_TIMETYPE_SECOND) || (n==RTC_TIMETYPE_MINUTE) \
|| (n==RTC_TIMETYPE_HOUR) || (n==RTC_TIMETYPE_DAYOFWEEK) \
|| (n==RTC_TIMETYPE_DAYOFMONTH) || (n==RTC_TIMETYPE_DAYOFYEAR) \
|| (n==RTC_TIMETYPE_MONTH) || (n==RTC_TIMETYPE_YEAR))

/* Macro check RTC calibration type */
#define PARAM_RTC_CALIB_DIR(n)	((n==RTC_CALIB_DIR_FORWARD) || (n==RTC_CALIB_DIR_BACKWARD))

/* Macro check RTC GPREG type */
#define PARAM_RTC_GPREG_CH(n)	(n<=4)

/**
 * @}
 */


/* Public Types --------------------------------------------------------------- */
/** @defgroup RTC_Public_Types RTC Public Types
 * @{
 */

/** @brief Time structure definitions for easy manipulate the data */
typedef struct {
	uint32_t SEC; 		/*!< Seconds Register */
	uint32_t MIN; 		/*!< Minutes Register */
	uint32_t HOUR; 		/*!< Hours Register */
	uint32_t DOM;		/*!< Day of Month Register */
	uint32_t DOW; 		/*!< Day of Week Register */
	uint32_t DOY; 		/*!< Day of Year Register */
	uint32_t MONTH; 	/*!< Months Register */
	uint32_t YEAR; 		/*!< Years Register */
} RTC_TIME_Type;

/** @brief RTC interrupt source */
typedef enum {
	RTC_INT_COUNTER_INCREASE = RTC_IRL_RTCCIF, 	/*!<  Counter Increment Interrupt */
	RTC_INT_ALARM = RTC_IRL_RTCALF 				/*!< The alarm interrupt */
} RTC_INT_OPT;


/** @brief RTC time type option */
typedef enum {
	RTC_TIMETYPE_SECOND = 0, 		/*!< Second */
	RTC_TIMETYPE_MINUTE = 1, 		/*!< Month */
	RTC_TIMETYPE_HOUR = 2, 			/*!< Hour */
	RTC_TIMETYPE_DAYOFWEEK = 3, 	/*!< Day of week */
	RTC_TIMETYPE_DAYOFMONTH = 4, 	/*!< Day of month */
	RTC_TIMETYPE_DAYOFYEAR = 5, 	/*!< Day of year */
	RTC_TIMETYPE_MONTH = 6, 		/*!< Month */
	RTC_TIMETYPE_YEAR = 7 			/*!< Year */
} RTC_TIMETYPE_Num;

/**
 * @}
 */



/* Public Functions ----------------------------------------------------------- */
/** @defgroup RTC_Public_Functions RTC Public Functions
 * @{
 */

void RTC_Init (LPC_RTC_TypeDef *RTCx);
void RTC_DeInit(LPC_RTC_TypeDef *RTCx);
void RTC_ResetClockTickCounter(LPC_RTC_TypeDef *RTCx);
void RTC_Cmd (LPC_RTC_TypeDef *RTCx, FunctionalState NewState);
void RTC_CntIncrIntConfig (LPC_RTC_TypeDef *RTCx, uint32_t CntIncrIntType, \
								FunctionalState NewState);
void RTC_AlarmIntConfig (LPC_RTC_TypeDef *RTCx, uint32_t AlarmTimeType, \
								FunctionalState NewState);
void RTC_SetTime (LPC_RTC_TypeDef *RTCx, uint32_t Timetype, uint32_t TimeValue);
uint32_t RTC_GetTime(LPC_RTC_TypeDef *RTCx, uint32_t Timetype);
void RTC_SetFullTime (LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime);
void RTC_GetFullTime (LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime);
void RTC_SetAlarmTime (LPC_RTC_TypeDef *RTCx, uint32_t Timetype, uint32_t ALValue);
uint32_t RTC_GetAlarmTime (LPC_RTC_TypeDef *RTCx, uint32_t Timetype);
void RTC_SetFullAlarmTime (LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime);
void RTC_GetFullAlarmTime (LPC_RTC_TypeDef *RTCx, RTC_TIME_Type *pFullTime);
IntStatus RTC_GetIntPending (LPC_RTC_TypeDef *RTCx, uint32_t IntType);
void RTC_ClearIntPending (LPC_RTC_TypeDef *RTCx, uint32_t IntType);
void RTC_CalibCounterCmd(LPC_RTC_TypeDef *RTCx, FunctionalState NewState);
void RTC_CalibConfig(LPC_RTC_TypeDef *RTCx, uint32_t CalibValue, uint8_t CalibDir);
void RTC_WriteGPREG (LPC_RTC_TypeDef *RTCx, uint8_t Channel, uint32_t Value);
uint32_t RTC_ReadGPREG (LPC_RTC_TypeDef *RTCx, uint8_t Channel);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_RTC_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
