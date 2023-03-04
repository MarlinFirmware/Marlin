/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file hc32f46x_rtc.h
 **
 ** A detailed description is available at
 ** @link RtcGroup Real-Time Clock description @endlink
 **
 **   - 2018-11-22  1.0  yangjp First version for Device Driver Library of RTC.
 **
 ******************************************************************************/
#ifndef __HC32F46x_RTC_H__
#define __HC32F46x_RTC_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_RTC_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup RtcGroup Real-Time Clock(RTC)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief RTC period interrupt type enumeration
     ******************************************************************************/
    typedef enum en_rtc_period_int_type
    {
        RtcPeriodIntInvalid = 0u, ///< Period interrupt invalid
        RtcPeriodIntHalfSec = 1u, ///< 0.5 second period interrupt
        RtcPeriodIntOneSec = 2u,  ///< 1 second period interrupt
        RtcPeriodIntOneMin = 3u,  ///< 1 minute period interrupt
        RtcPeriodIntOneHour = 4u, ///< 1 hour period interrupt
        RtcPeriodIntOneDay = 5u,  ///< 1 day period interrupt
        RtcPeriodIntOneMon = 6u   ///< 1 month period interrupt
    } en_rtc_period_int_type_t;

    /**
     *******************************************************************************
     ** \brief RTC time format enumeration
     ******************************************************************************/
    typedef enum en_rtc_time_format
    {
        RtcTimeFormat12Hour = 0u, ///< 12 hours mode
        RtcTimeFormat24Hour = 1u, ///< 24 hours mode
    } en_rtc_time_format_t;

    /**
     *******************************************************************************
     ** \brief RTC 1Hz output compensation way enumeration
     ******************************************************************************/
    typedef enum en_rtc_output_compen
    {
        RtcOutputCompenDistributed = 0u, ///< Distributed compensation 1hz output
        RtcOutputCompenUniform = 1u,     ///< Uniform Compensation 1hz output
    } en_rtc_output_compen_t;

    /**
     *******************************************************************************
     ** \brief RTC work mode enumeration
     ******************************************************************************/
    typedef enum en_rtc_work_mode
    {
        RtcModeNormalCount = 0u, ///< Normal count mode
        RtcModeReadOrWrite = 1u, ///< Read or write mode
    } en_rtc_work_mode_t;

    /**
     *******************************************************************************
     ** \brief RTC count clock source enumeration
     ******************************************************************************/
    typedef enum en_rtc_clk_source
    {
        RtcClkXtal32 = 0u, ///< XTAL32 as clock source
        RtcClkLrc = 1u,    ///< LRC as clock source
    } en_rtc_clk_source_t;

    /**
     *******************************************************************************
     ** \brief RTC data format enumeration
     ******************************************************************************/
    typedef enum en_rtc_data_format
    {
        RtcDataFormatDec = 0u, ///< Decimal format
        RtcDataFormatBcd = 1u, ///< BCD format
    } en_rtc_data_format_t;

    /**
     *******************************************************************************
     ** \brief RTC 12 hour AM/PM enumeration
     ******************************************************************************/
    typedef enum en_rtc_hour12_ampm
    {
        RtcHour12Am = 0u, ///< Ante meridiem
        RtcHour12Pm = 1u, ///< Post meridiem
    } en_rtc_hour12_ampm_t;

    /**
     *******************************************************************************
     ** \brief RTC month enumeration
     ******************************************************************************/
    typedef enum en_rtc_month
    {
        RtcMonthJanuary = 1u,   ///< January
        RtcMonthFebruary = 2u,  ///< February
        RtcMonthMarch = 3u,     ///< March
        RtcMonthApril = 4u,     ///< April
        RtcMonthMay = 5u,       ///< May
        RtcMonthJune = 6u,      ///< June
        RtcMonthJuly = 7u,      ///< July
        RtcMonthAugust = 8u,    ///< August
        RtcMonthSeptember = 9u, ///< September
        RtcMonthOctober = 10u,  ///< October
        RtcMonthNovember = 11u, ///< November
        RtcMonthDecember = 12u, ///< December
    } en_rtc_month_t;

    /**
     *******************************************************************************
     ** \brief RTC weekday enumeration
     ******************************************************************************/
    typedef enum en_rtc_weekday
    {
        RtcWeekdaySunday = 0u,    ///< Sunday
        RtcWeekdayMonday = 1u,    ///< Monday
        RtcWeekdayTuesday = 2u,   ///< Tuesday
        RtcWeekdayWednesday = 3u, ///< Wednesday
        RtcWeekdayThursday = 4u,  ///< Thursday
        RtcWeekdayFriday = 5u,    ///< Friday
        RtcWeekdaySaturday = 6u   ///< Saturday
    } en_rtc_weekday_t;

    /**
     *******************************************************************************
     ** \brief RTC alarm weekday enumeration
     ******************************************************************************/
    typedef enum en_rtc_alarm_weekday
    {
        RtcAlarmWeekdaySunday = 0x01u,    ///< Sunday
        RtcAlarmWeekdayMonday = 0x02u,    ///< Monday
        RtcAlarmWeekdayTuesday = 0x04u,   ///< Tuesday
        RtcAlarmWeekdayWednesday = 0x08u, ///< Wednesday
        RtcAlarmWeekdayThursday = 0x10u,  ///< Thursday
        RtcAlarmWeekdayFriday = 0x20u,    ///< Friday
        RtcAlarmWeekdaySaturday = 0x40u,  ///< Saturday
    } en_rtc_alarm_weekday_t;

    /**
     *******************************************************************************
     ** \brief RTC interrupt request type enumeration
     ******************************************************************************/
    typedef enum en_rtc_irq_type_
    {
        RtcIrqPeriod = 0u, ///< Period count interrupt request
        RtcIrqAlarm = 1u,  ///< Alarm interrupt request
    } en_rtc_irq_type_t;

    /**
     *******************************************************************************
     ** \brief RTC date and time structure definition
     ******************************************************************************/
    typedef struct stc_rtc_date_time
    {
        uint8_t u8Year;              ///< Year (range 0-99)
        uint8_t u8Month;             ///< Month (range 1-12)
        uint8_t u8Day;               ///< Day (range 1-31)
        uint8_t u8Hour;              ///< Hours (range 1-12 when 12 hour format; range 0-23 when 24 hour format)
        uint8_t u8Minute;            ///< Minutes (range 0-59)
        uint8_t u8Second;            ///< Seconds (range 0-59)
        uint8_t u8Weekday;           ///< Weekday (range 0-6)
        en_rtc_hour12_ampm_t enAmPm; ///< The value is valid when 12-hour format
    } stc_rtc_date_time_t;

    /**
     *******************************************************************************
     ** \brief RTC alarm time structure definition
     ******************************************************************************/
    typedef struct stc_rtc_alarm_time
    {
        uint8_t u8Minute;            ///< Minutes (range 0-59)
        uint8_t u8Hour;              ///< Hours (range 1-12 when 12 hour format; range 0-23 when 24 hour format)
        uint8_t u8Weekday;           ///< Weekday (range RtcAlarmWeekdaySunday to RtcAlarmWeekdaySaturday)
        en_rtc_hour12_ampm_t enAmPm; ///< The value is valid when 12-hour format
    } stc_rtc_alarm_time_t;

    /**
     *******************************************************************************
     ** \brief RTC init structure definition
     ******************************************************************************/
    typedef struct stc_rtc_init
    {
        en_rtc_clk_source_t enClkSource;      ///< Clock source
        en_rtc_period_int_type_t enPeriodInt; ///< Period interrupt condition
        en_rtc_time_format_t enTimeFormat;    ///< RTC time format
        en_rtc_output_compen_t enCompenWay;   ///< 1HZ output compensation way
        uint16_t u16CompenVal;                ///< Clock error compensation value
        en_functional_state_t enCompenEn;     ///< Enable/Disable clock error compensation
    } stc_rtc_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    /* Base functions */
    en_result_t RTC_DeInit(void);
    en_result_t RTC_Init(const stc_rtc_init_t *pstcRtcInit);
    en_result_t RTC_Cmd(en_functional_state_t enNewSta);
    en_result_t RTC_EnterRwMode(void);
    en_result_t RTC_ExitRwMode(void);

    /* Extend functions */
    en_result_t RTC_PeriodIntConfig(en_rtc_period_int_type_t enIntType);
    en_result_t RTC_LowPowerSwitch(void);
    en_result_t RTC_SetClkCompenValue(uint16_t u16CompenVal);
    en_result_t RTC_ClkCompenCmd(en_functional_state_t enNewSta);
    en_result_t RTC_OneHzOutputCmd(en_functional_state_t enNewSta);

    /* Date and time functions */
    en_result_t RTC_SetDateTime(en_rtc_data_format_t enFormat, const stc_rtc_date_time_t *pstcRtcDateTime,
                                en_functional_state_t enUpdateDateEn, en_functional_state_t enUpdateTimeEn);
    en_result_t RTC_GetDateTime(en_rtc_data_format_t enFormat, stc_rtc_date_time_t *pstcRtcDateTime);

    /* Alarm functions */
    en_result_t RTC_SetAlarmTime(en_rtc_data_format_t enFormat, const stc_rtc_alarm_time_t *pstcRtcAlarmTime);
    en_result_t RTC_GetAlarmTime(en_rtc_data_format_t enFormat, stc_rtc_alarm_time_t *pstcRtcAlarmTime);
    en_result_t RTC_AlarmCmd(en_functional_state_t enNewSta);

    /* Interrupt and flags management functions ******************************************/
    en_result_t RTC_IrqCmd(en_rtc_irq_type_t enIrq, en_functional_state_t enNewSta);
    en_flag_status_t RTC_GetAlarmFlag(void);
    en_result_t RTC_ClearAlarmFlag(void);

    //@} // RtcGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_RTC_ENABLE */

#endif /* __HC32F46x_RTC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
