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
/** \file hc32f46x_rtc.c
 **
 ** A detailed description is available at
 ** @link RtcGroup Real-Time Clock description @endlink
 **
 **   - 2018-11-22  1.0  yangjp First version for Device Driver Library of RTC.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_rtc.h"
#include "hc32f46x_utility.h"

#if (DDL_RTC_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup RtcGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*!< Parameter valid check for clock source type */
#define IS_VALID_CLK_SOURCE_TYPE(x) \
    ((RtcClkXtal32 == (x)) ||       \
     (RtcClkLrc == (x)))

/*!< Parameter valid check for period interrupt condition */
#define IS_VALID_PERIOD_INT_CONDITION(x) \
    ((RtcPeriodIntInvalid == (x)) ||     \
     (RtcPeriodIntHalfSec == (x)) ||     \
     (RtcPeriodIntOneSec == (x)) ||      \
     (RtcPeriodIntOneMin == (x)) ||      \
     (RtcPeriodIntOneHour == (x)) ||     \
     (RtcPeriodIntOneDay == (x)) ||      \
     (RtcPeriodIntOneMon == (x)))

/*!< Parameter valid check for time format */
#define IS_VALID_TIME_FORMAT(x)      \
    ((RtcTimeFormat12Hour == (x)) || \
     (RtcTimeFormat24Hour == (x)))

/*!< Parameter valid check for compensation way */
#define IS_VALID_COMPEN_WAY(x)              \
    ((RtcOutputCompenDistributed == (x)) || \
     (RtcOutputCompenUniform == (x)))

/*!< Parameter valid check for compensation value range */
#define IS_VALID_COMPEN_VALUE_RANGE(x) ((x) <= 0x1FFu)

/*!< Parameter valid check for data format */
#define IS_VALID_DATA_FORMAT(x)   \
    ((RtcDataFormatDec == (x)) || \
     (RtcDataFormatBcd == (x)))

/*!< Parameter valid check for time second */
#define IS_VALID_TIME_SECOND(x) ((x) <= 59u)

/*!< Parameter valid check for time minute */
#define IS_VALID_TIME_MINUTE(x) ((x) <= 59u)

/*!< Parameter valid check for time hour */
#define IS_VALID_TIME_HOUR12(x) (((x) >= 1u) && ((x) <= 12u))
#define IS_VALID_TIME_HOUR24(x) ((x) <= 23u)

/*!< Parameter valid check for date weekday */
#define IS_VALID_DATE_WEEKDAY(x) ((x) <= 6u)

/*!< Parameter valid check for date day */
#define IS_VALID_DATE_DAY(x) (((x) >= 1u) && ((x) <= 31u))

/*!< Parameter valid check for date month */
#define IS_VALID_DATE_MONTH(x) (((x) >= 1u) && ((x) <= 12u))

/*!< Parameter valid check for date year */
#define IS_VALID_DATE_YEAR(x) ((x) <= 99u)

/*!< Parameter valid check for hour12 am/pm */
#define IS_VALID_HOUR12_AMPM(x) \
    ((RtcHour12Am == (x)) ||    \
     (RtcHour12Pm == (x)))

/*!< Parameter valid check for alarm weekday */
#define IS_VALID_ALARM_WEEKDAY(x) (((x) >= 1u) && ((x) <= 0x7Fu))

/*!< Parameter valid check for interrupt request type */
#define IS_VALID_IRQ_TYPE(x)  \
    ((RtcIrqPeriod == (x)) || \
     (RtcIrqAlarm == (x)))

/*!< 12 hour format am/pm status bit */
#define RTC_HOUR12_AMPM_MASK (0x20u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief De-Initialize RTC
 **
 ** \param [in] None
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorTimeout                De-Initialize timeout
 **
 ******************************************************************************/
en_result_t RTC_DeInit(void)
{
    uint8_t u8RegSta;
    uint32_t u32Timeout, u32TimeCnt = 0u;
    en_result_t enRet = Ok;

    M4_RTC->CR0_f.RESET = 0u;
    /* Waiting for normal count status or end of RTC software reset */
    u32Timeout = SystemCoreClock >> 8u;
    do
    {
        u8RegSta = (uint8_t)M4_RTC->CR0_f.RESET;
        u32TimeCnt++;
    } while ((u32TimeCnt < u32Timeout) && (u8RegSta == 1u));

    if (1u == u8RegSta)
    {
        enRet = ErrorTimeout;
    }
    else
    {
        /* Initialize all RTC registers */
        M4_RTC->CR0_f.RESET = 1u;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize RTC
 **
 ** \param [in] pstcRtcInit             Pointer to RTC init configuration
 ** \arg See the struct #stc_rtc_init_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval Error                       Parameter error
 **
 ******************************************************************************/
en_result_t RTC_Init(const stc_rtc_init_t *pstcRtcInit)
{
    en_result_t enRet = Ok;

    if (NULL == pstcRtcInit)
    {
        enRet = Error;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_CLK_SOURCE_TYPE(pstcRtcInit->enClkSource));
        DDL_ASSERT(IS_VALID_PERIOD_INT_CONDITION(pstcRtcInit->enPeriodInt));
        DDL_ASSERT(IS_VALID_TIME_FORMAT(pstcRtcInit->enTimeFormat));
        DDL_ASSERT(IS_VALID_COMPEN_WAY(pstcRtcInit->enCompenWay));
        DDL_ASSERT(IS_VALID_COMPEN_VALUE_RANGE(pstcRtcInit->u16CompenVal));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcRtcInit->enCompenEn));

        /* Configure clock */
        if (RtcClkLrc == pstcRtcInit->enClkSource)
        {
            M4_RTC->CR3_f.LRCEN = 1u;
        }
        M4_RTC->CR3_f.RCKSEL = pstcRtcInit->enClkSource;

        /* Configure control register */
        M4_RTC->CR1_f.PRDS = pstcRtcInit->enPeriodInt;
        M4_RTC->CR1_f.AMPM = pstcRtcInit->enTimeFormat;
        M4_RTC->CR1_f.ONEHZSEL = pstcRtcInit->enCompenWay;

        /* Configure clock error compensation register */
        M4_RTC->ERRCRH_f.COMP8 = ((uint32_t)pstcRtcInit->u16CompenVal >> 8u) & 0x01u;
        M4_RTC->ERRCRL = (uint32_t)pstcRtcInit->u16CompenVal & 0x00FFu;
        M4_RTC->ERRCRH_f.COMPEN = pstcRtcInit->enCompenEn;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enter RTC read/write mode
 **
 ** \param [in] None
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorTimeout                Enter mode timeout
 **
 ******************************************************************************/
en_result_t RTC_EnterRwMode(void)
{
    uint8_t u8RegSta;
    uint32_t u32Timeout, u32TimeCnt = 0u;
    en_result_t enRet = Ok;

    /* Mode switch when RTC is running */
    if (0u != M4_RTC->CR1_f.START)
    {
        M4_RTC->CR2_f.WAIT = 1u;
        /* Waiting for WAITF bit set */
        u32Timeout = SystemCoreClock >> 8u;
        do
        {
            u8RegSta = (uint8_t)M4_RTC->CR2_f.WAITF;
            u32TimeCnt++;
        } while ((u32TimeCnt < u32Timeout) && (u8RegSta == 0u));

        if (0u == u8RegSta)
        {
            enRet = ErrorTimeout;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Exit RTC read/write mode
 **
 ** \param [in] None
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorTimeout                Exit mode timeout
 **
 ******************************************************************************/
en_result_t RTC_ExitRwMode(void)
{
    uint8_t u8RegSta;
    uint32_t u32Timeout, u32TimeCnt = 0u;
    en_result_t enRet = Ok;

    /* Mode switch when RTC is running */
    if (0u != M4_RTC->CR1_f.START)
    {
        M4_RTC->CR2_f.WAIT = 0u;
        /* Waiting for WAITF bit reset */
        u32Timeout = SystemCoreClock >> 8u;
        do
        {
            u8RegSta = (uint8_t)M4_RTC->CR2_f.WAITF;
            u32TimeCnt++;
        } while ((u32TimeCnt < u32Timeout) && (u8RegSta == 1u));

        if (1u == u8RegSta)
        {
            enRet = ErrorTimeout;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable RTC count
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable RTC count
 ** \arg Enable                         Enable RTC count
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t RTC_Cmd(en_functional_state_t enNewSta)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    M4_RTC->CR1_f.START = enNewSta;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief RTC period interrupt config
 **
 ** \param [in] enIntType               Period interrupt request type
 ** \arg RtcPeriodIntInvalid            Period interrupt invalid
 ** \arg RtcPeriodIntHalfSec            0.5 second period interrupt
 ** \arg RtcPeriodIntOneSec             1 second period interrupt
 ** \arg RtcPeriodIntOneMin             1 minute period interrupt
 ** \arg RtcPeriodIntOneHour            1 hour period interrupt
 ** \arg RtcPeriodIntOneDay             1 day period interrupt
 ** \arg RtcPeriodIntOneMon             1 month period interrupt
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t RTC_PeriodIntConfig(en_rtc_period_int_type_t enIntType)
{
    uint8_t u8RtcSta;
    uint8_t u8IntSta;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_PERIOD_INT_CONDITION(enIntType));

    u8RtcSta = (uint8_t)M4_RTC->CR1_f.START;
    u8IntSta = (uint8_t)M4_RTC->CR2_f.PRDIE;
    /* Disable period interrupt when START=1 and PRDIE=1 */
    if ((1u == u8IntSta) && (1u == u8RtcSta))
    {
        M4_RTC->CR2_f.PRDIE = 0u;
    }
    M4_RTC->CR1_f.PRDS = enIntType;

    if ((1u == u8IntSta) && (1u == u8RtcSta))
    {
        M4_RTC->CR2_f.PRDIE = 1u;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief RTC switch to low power mode
 **
 ** \param [in] None
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidMode            RTC count not start
 ** \retval ErrorTimeout                Switch timeout
 **
 ******************************************************************************/
en_result_t RTC_LowPowerSwitch(void)
{
    uint8_t u8RegSta;
    uint32_t u32Timeout, u32TimeCnt = 0u;
    en_result_t enRet = ErrorInvalidMode;

    /* Check RTC work status */
    if (0u != M4_RTC->CR1_f.START)
    {
        M4_RTC->CR2_f.WAIT = 1u;
        /* Waiting for RTC WAITF bit set */
        u32Timeout = SystemCoreClock / 100u;
        do
        {
            u8RegSta = (uint8_t)M4_RTC->CR2_f.WAITF;
            u32TimeCnt++;
        } while ((u32TimeCnt < u32Timeout) && (u8RegSta == 0u));

        if (0u == u8RegSta)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            M4_RTC->CR2_f.WAIT = 0u;
            /* Waiting for RTC WAITF bit reset */
            u32TimeCnt = 0u;
            do
            {
                u8RegSta = (uint8_t)M4_RTC->CR2_f.WAITF;
                u32TimeCnt++;
            } while ((u32TimeCnt < u32Timeout) && (u8RegSta == 1u));

            if (1u == u8RegSta)
            {
                enRet = ErrorTimeout;
            }
            else
            {
                enRet = Ok;
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set RTC 1hz output compensation value
 **
 ** \param [in] u16CompenVal            Clock compensation value
 ** \arg 0~0x1FF
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t RTC_SetClkCompenValue(uint16_t u16CompenVal)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_COMPEN_VALUE_RANGE(u16CompenVal));

    M4_RTC->ERRCRH_f.COMP8 = ((uint32_t)u16CompenVal >> 8u) & 0x01u;
    M4_RTC->ERRCRL = (uint32_t)u16CompenVal & 0x00FFu;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable clock compensation
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable RTC clock compensation
 ** \arg Enable                         Enable RTC clock compensation
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t RTC_ClkCompenCmd(en_functional_state_t enNewSta)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    M4_RTC->ERRCRH_f.COMPEN = enNewSta;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable RTC 1hz output
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable RTC 1hz output
 ** \arg Enable                         Enable RTC 1hz output
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t RTC_OneHzOutputCmd(en_functional_state_t enNewSta)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    M4_RTC->CR1_f.ONEHZOE = enNewSta;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set RTC current date and time
 **
 ** \param [in] enFormat                Date and time data format
 ** \arg RtcDataFormatDec               Decimal format
 ** \arg RtcDataFormatBcd               BCD format
 **
 ** \param [in] pstcRtcDateTime         Pointer to RTC date and time configuration
 ** \arg See the struct #stc_rtc_date_time_t
 **
 ** \param [in] enUpdateDateEn          The function new state(Contain year/month/day/weekday)
 ** \arg Disable                        Disable update RTC date
 ** \arg Enable                         Enable update RTC date
 **
 ** \param [in] enUpdateTimeEn          The function new state(Contain hour/minute/second)
 ** \arg Disable                        Disable update RTC time
 ** \arg Enable                         Enable update RTC time
 **
 ** \retval Ok                          Process successfully done
 ** \retval Error                       Enter or exit read/write mode failed
 ** \retval ErrorInvalidParameter       Parameter enUpdateDateEn or enUpdateTimeEn invalid
 **
 ******************************************************************************/
en_result_t RTC_SetDateTime(en_rtc_data_format_t enFormat, const stc_rtc_date_time_t *pstcRtcDateTime,
                            en_functional_state_t enUpdateDateEn, en_functional_state_t enUpdateTimeEn)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_DATA_FORMAT(enFormat));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enUpdateDateEn));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enUpdateTimeEn));

    /* Check update status */
    if (((Disable == enUpdateDateEn) && (Disable == enUpdateTimeEn)) || (NULL == pstcRtcDateTime))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check the date parameters */
        if (Enable == enUpdateDateEn)
        {
            if (RtcDataFormatDec == enFormat)
            {
                DDL_ASSERT(IS_VALID_DATE_YEAR(pstcRtcDateTime->u8Year));
                DDL_ASSERT(IS_VALID_DATE_MONTH(pstcRtcDateTime->u8Month));
                DDL_ASSERT(IS_VALID_DATE_DAY(pstcRtcDateTime->u8Day));
            }
            else
            {
                DDL_ASSERT(IS_VALID_DATE_YEAR(BCD2DEC(pstcRtcDateTime->u8Year)));
                DDL_ASSERT(IS_VALID_DATE_MONTH(BCD2DEC(pstcRtcDateTime->u8Month)));
                DDL_ASSERT(IS_VALID_DATE_DAY(BCD2DEC(pstcRtcDateTime->u8Day)));
            }
            DDL_ASSERT(IS_VALID_DATE_WEEKDAY(pstcRtcDateTime->u8Weekday));
        }
        /* Check the time parameters */
        if (Enable == enUpdateTimeEn)
        {
            if (RtcDataFormatDec == enFormat)
            {
                if (RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM)
                {
                    DDL_ASSERT(IS_VALID_TIME_HOUR12(pstcRtcDateTime->u8Hour));
                    DDL_ASSERT(IS_VALID_HOUR12_AMPM(pstcRtcDateTime->enAmPm));
                }
                else
                {
                    DDL_ASSERT(IS_VALID_TIME_HOUR24(pstcRtcDateTime->u8Hour));
                }
                DDL_ASSERT(IS_VALID_TIME_MINUTE(pstcRtcDateTime->u8Minute));
                DDL_ASSERT(IS_VALID_TIME_SECOND(pstcRtcDateTime->u8Second));
            }
            else
            {
                if (RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM)
                {
                    DDL_ASSERT(IS_VALID_TIME_HOUR12(BCD2DEC(pstcRtcDateTime->u8Hour)));
                    DDL_ASSERT(IS_VALID_HOUR12_AMPM(pstcRtcDateTime->enAmPm));
                }
                else
                {
                    DDL_ASSERT(IS_VALID_TIME_HOUR24(BCD2DEC(pstcRtcDateTime->u8Hour)));
                }
                DDL_ASSERT(IS_VALID_TIME_MINUTE(BCD2DEC(pstcRtcDateTime->u8Minute)));
                DDL_ASSERT(IS_VALID_TIME_SECOND(BCD2DEC(pstcRtcDateTime->u8Second)));
            }
        }

        /* Enter read/write mode */
        if (RTC_EnterRwMode() == ErrorTimeout)
        {
            enRet = Error;
        }
        else
        {
            /* Update date */
            if (Enable == enUpdateDateEn)
            {
                if (RtcDataFormatDec == enFormat)
                {
                    M4_RTC->YEAR = DEC2BCD((uint32_t)pstcRtcDateTime->u8Year);
                    M4_RTC->MON = DEC2BCD((uint32_t)pstcRtcDateTime->u8Month);
                    M4_RTC->DAY = DEC2BCD((uint32_t)pstcRtcDateTime->u8Day);
                }
                else
                {
                    M4_RTC->YEAR = pstcRtcDateTime->u8Year;
                    M4_RTC->MON = pstcRtcDateTime->u8Month;
                    M4_RTC->DAY = pstcRtcDateTime->u8Day;
                }
                M4_RTC->WEEK = pstcRtcDateTime->u8Weekday;
            }
            /* Update time */
            if (Enable == enUpdateTimeEn)
            {
                if (RtcDataFormatDec == enFormat)
                {
                    if ((RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM) &&
                        (RtcHour12Pm == pstcRtcDateTime->enAmPm))
                    {
                        M4_RTC->HOUR = DEC2BCD((uint32_t)pstcRtcDateTime->u8Hour) | RTC_HOUR12_AMPM_MASK;
                    }
                    else
                    {
                        M4_RTC->HOUR = DEC2BCD((uint32_t)pstcRtcDateTime->u8Hour);
                    }
                    M4_RTC->MIN = DEC2BCD((uint32_t)pstcRtcDateTime->u8Minute);
                    M4_RTC->SEC = DEC2BCD((uint32_t)pstcRtcDateTime->u8Second);
                }
                else
                {
                    if ((RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM) &&
                        (RtcHour12Pm == pstcRtcDateTime->enAmPm))
                    {
                        M4_RTC->HOUR = (uint32_t)pstcRtcDateTime->u8Hour | RTC_HOUR12_AMPM_MASK;
                    }
                    else
                    {
                        M4_RTC->HOUR = (uint32_t)pstcRtcDateTime->u8Hour;
                    }
                    M4_RTC->MIN = pstcRtcDateTime->u8Minute;
                    M4_RTC->SEC = pstcRtcDateTime->u8Second;
                }
            }
            /* Exit read/write mode */
            if (RTC_ExitRwMode() == ErrorTimeout)
            {
                enRet = Error;
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get RTC current date and time
 **
 ** \param [in] enFormat                Date and time data format
 ** \arg RtcDataFormatDec               Decimal format
 ** \arg RtcDataFormatBcd               BCD format
 **
 ** \param [out] pstcRtcDateTime        Pointer to RTC date and time configuration
 ** \arg See the struct #stc_rtc_date_time_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval Error                       Enter or exit read/write mode failed
 **
 ******************************************************************************/
en_result_t RTC_GetDateTime(en_rtc_data_format_t enFormat, stc_rtc_date_time_t *pstcRtcDateTime)
{
    en_result_t enRet = Ok;

    if (NULL == pstcRtcDateTime)
    {
        enRet = Error;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_DATA_FORMAT(enFormat));

        /* Enter read/write mode */
        if (RTC_EnterRwMode() == ErrorTimeout)
        {
            enRet = Error;
        }
        else
        {
            /* Get RTC date and time registers */
            pstcRtcDateTime->u8Year = (uint8_t)(M4_RTC->YEAR);
            pstcRtcDateTime->u8Month = (uint8_t)(M4_RTC->MON);
            pstcRtcDateTime->u8Day = (uint8_t)(M4_RTC->DAY);
            pstcRtcDateTime->u8Weekday = (uint8_t)(M4_RTC->WEEK);
            pstcRtcDateTime->u8Hour = (uint8_t)(M4_RTC->HOUR);
            pstcRtcDateTime->u8Minute = (uint8_t)(M4_RTC->MIN);
            pstcRtcDateTime->u8Second = (uint8_t)(M4_RTC->SEC);
            if (RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM)
            {
                if (RTC_HOUR12_AMPM_MASK == (pstcRtcDateTime->u8Hour & RTC_HOUR12_AMPM_MASK))
                {
                    pstcRtcDateTime->u8Hour &= (uint8_t)(~RTC_HOUR12_AMPM_MASK);
                    pstcRtcDateTime->enAmPm = RtcHour12Pm;
                }
                else
                {
                    pstcRtcDateTime->enAmPm = RtcHour12Am;
                }
            }

            /* Check decimal format*/
            if (RtcDataFormatDec == enFormat)
            {
                pstcRtcDateTime->u8Year = BCD2DEC(pstcRtcDateTime->u8Year);
                pstcRtcDateTime->u8Month = BCD2DEC(pstcRtcDateTime->u8Month);
                pstcRtcDateTime->u8Day = BCD2DEC(pstcRtcDateTime->u8Day);
                pstcRtcDateTime->u8Hour = BCD2DEC(pstcRtcDateTime->u8Hour);
                pstcRtcDateTime->u8Minute = BCD2DEC(pstcRtcDateTime->u8Minute);
                pstcRtcDateTime->u8Second = BCD2DEC(pstcRtcDateTime->u8Second);
            }

            /* exit read/write mode */
            if (RTC_ExitRwMode() == ErrorTimeout)
            {
                enRet = Error;
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set RTC alarm time
 **
 ** \param [in] enFormat                Date and time data format
 ** \arg RtcDataFormatDec               Decimal format
 ** \arg RtcDataFormatBcd               BCD format
 **
 ** \param [in] pstcRtcAlarmTime        Pointer to RTC alarm time configuration
 ** \arg See the struct #stc_rtc_alarm_time_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval Error                       Parameter error
 **
 ******************************************************************************/
en_result_t RTC_SetAlarmTime(en_rtc_data_format_t enFormat, const stc_rtc_alarm_time_t *pstcRtcAlarmTime)
{
    en_result_t enRet = Ok;

    if (NULL == pstcRtcAlarmTime)
    {
        enRet = Error;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_DATA_FORMAT(enFormat));

        if (RtcDataFormatDec == enFormat)
        {
            if (RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM)
            {
                DDL_ASSERT(IS_VALID_TIME_HOUR12(pstcRtcAlarmTime->u8Hour));
                DDL_ASSERT(IS_VALID_HOUR12_AMPM(pstcRtcAlarmTime->enAmPm));
            }
            else
            {
                DDL_ASSERT(IS_VALID_TIME_HOUR24(pstcRtcAlarmTime->u8Hour));
            }
            DDL_ASSERT(IS_VALID_TIME_MINUTE(pstcRtcAlarmTime->u8Minute));
        }
        else
        {
            if (RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM)
            {
                DDL_ASSERT(IS_VALID_TIME_HOUR12(BCD2DEC(pstcRtcAlarmTime->u8Hour)));
                DDL_ASSERT(IS_VALID_HOUR12_AMPM(pstcRtcAlarmTime->enAmPm));
            }
            else
            {
                DDL_ASSERT(IS_VALID_TIME_HOUR24(BCD2DEC(pstcRtcAlarmTime->u8Hour)));
            }
            DDL_ASSERT(IS_VALID_TIME_MINUTE(BCD2DEC(pstcRtcAlarmTime->u8Minute)));
        }
        DDL_ASSERT(IS_VALID_ALARM_WEEKDAY(pstcRtcAlarmTime->u8Weekday));

        /* Configure alarm registers */
        if (RtcDataFormatDec == enFormat)
        {
            if ((RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM) &&
                (RtcHour12Pm == pstcRtcAlarmTime->enAmPm))
            {
                M4_RTC->ALMHOUR = DEC2BCD((uint32_t)pstcRtcAlarmTime->u8Hour) | RTC_HOUR12_AMPM_MASK;
            }
            else
            {
                M4_RTC->ALMHOUR = DEC2BCD((uint32_t)pstcRtcAlarmTime->u8Hour);
            }
            M4_RTC->ALMMIN = DEC2BCD((uint32_t)pstcRtcAlarmTime->u8Minute);
        }
        else
        {
            if ((RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM) &&
                (RtcHour12Pm == pstcRtcAlarmTime->enAmPm))
            {
                M4_RTC->ALMHOUR = (uint32_t)pstcRtcAlarmTime->u8Hour | RTC_HOUR12_AMPM_MASK;
            }
            else
            {
                M4_RTC->ALMHOUR = (uint32_t)pstcRtcAlarmTime->u8Hour;
            }
            M4_RTC->ALMMIN = pstcRtcAlarmTime->u8Minute;
        }
        M4_RTC->ALMWEEK = pstcRtcAlarmTime->u8Weekday;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get RTC alarm time
 **
 ** \param [in] enFormat                Date and time data format
 ** \arg RtcDataFormatDec               Decimal format
 ** \arg RtcDataFormatBcd               BCD format
 **
 ** \param [out] pstcRtcAlarmTime       Pointer to RTC alarm time configuration
 ** \arg See the struct #stc_rtc_alarm_time_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval Error                       Parameter error
 **
 ******************************************************************************/
en_result_t RTC_GetAlarmTime(en_rtc_data_format_t enFormat, stc_rtc_alarm_time_t *pstcRtcAlarmTime)
{
    en_result_t enRet = Ok;

    if (NULL == pstcRtcAlarmTime)
    {
        enRet = Error;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_DATA_FORMAT(enFormat));

        /* Get RTC date and time register */
        pstcRtcAlarmTime->u8Weekday = (uint8_t)M4_RTC->ALMWEEK;
        pstcRtcAlarmTime->u8Minute = (uint8_t)M4_RTC->ALMMIN;
        pstcRtcAlarmTime->u8Hour = (uint8_t)M4_RTC->ALMHOUR;
        if (RtcTimeFormat12Hour == M4_RTC->CR1_f.AMPM)
        {
            if ((pstcRtcAlarmTime->u8Hour & RTC_HOUR12_AMPM_MASK) == RTC_HOUR12_AMPM_MASK)
            {
                pstcRtcAlarmTime->u8Hour &= (uint8_t)(~RTC_HOUR12_AMPM_MASK);
                pstcRtcAlarmTime->enAmPm = RtcHour12Pm;
            }
            else
            {
                pstcRtcAlarmTime->enAmPm = RtcHour12Am;
            }
        }

        /* Check decimal format*/
        if (RtcDataFormatDec == enFormat)
        {
            pstcRtcAlarmTime->u8Hour = BCD2DEC(pstcRtcAlarmTime->u8Hour);
            pstcRtcAlarmTime->u8Minute = BCD2DEC(pstcRtcAlarmTime->u8Minute);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable RTC alarm function
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable RTC alarm function
 ** \arg Enable                         Enable RTC alarm function
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t RTC_AlarmCmd(en_functional_state_t enNewSta)
{
    uint8_t u8RtcSta;
    uint8_t u8IntSta;
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    u8RtcSta = (uint8_t)M4_RTC->CR1_f.START;
    u8IntSta = (uint8_t)M4_RTC->CR2_f.ALMIE;
    /* Disable alarm interrupt and clear alarm flag when START=1 and ALMIE=1 */
    if ((1u == u8IntSta) && (1u == u8RtcSta))
    {
        M4_RTC->CR2_f.ALMIE = 0u;
    }
    M4_RTC->CR2_f.ALME = enNewSta;

    if ((1u == u8IntSta) && (1u == u8RtcSta))
    {
        M4_RTC->CR1_f.ALMFCLR = 0u;
        M4_RTC->CR2_f.ALMIE = u8IntSta;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable RTC interrupt request
 **
 ** \param [in] enIrq                   RTC interrupt request type
 ** \arg RtcIrqPeriod                   Period count interrupt request
 ** \arg RtcIrqAlarm                    Alarm interrupt request
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable interrupt request
 ** \arg Enable                         Enable interrupt request
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t RTC_IrqCmd(en_rtc_irq_type_t enIrq, en_functional_state_t enNewSta)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_IRQ_TYPE(enIrq));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    /* enable/disable interrupt */
    switch (enIrq)
    {
    case RtcIrqPeriod:
        M4_RTC->CR2_f.PRDIE = enNewSta;
        break;
    case RtcIrqAlarm:
        M4_RTC->CR2_f.ALMIE = enNewSta;
        break;
    default:
        break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get RTC Alarm flag status
 **
 ** \param [in] None
 **
 ** \retval Set                         Flag is set
 ** \retval Reset                       Flag is reset
 **
 ******************************************************************************/
en_flag_status_t RTC_GetAlarmFlag(void)
{
    return (en_flag_status_t)(M4_RTC->CR2_f.ALMF);
}

/**
 *******************************************************************************
 ** \brief Clear RTC Alarm flag status
 **
 ** \param [in] None
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t RTC_ClearAlarmFlag(void)
{
    en_result_t enRet = Ok;

    M4_RTC->CR1_f.ALMFCLR = 0u;

    return enRet;
}

//@} // RtcGroup

#endif /* DDL_RTC_ENABLE */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
