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
/** \file hc32f46x_wdt.h
 **
 ** A detailed description is available at
 ** @link WdtGroup Watchdog Counter description @endlink
 **
 **   - 2018-10-18  1.0  yangjp First version for Device Driver Library of WDT.
 **
 ******************************************************************************/
#ifndef __HC32F46x_WDT_H__
#define __HC32F46x_WDT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_WDT_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup WdtGroup WatchDog Counter(WDT)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief WDT count cycle enumeration
     ******************************************************************************/
    typedef enum en_wdt_count_cycle
    {
        WdtCountCycle256 = 0u,   ///< 256 cycle
        WdtCountCycle4096 = 1u,  ///< 4096 cycle
        WdtCountCycle16384 = 2u, ///< 16384 cycle
        WdtCountCycle65536 = 3u, ///< 65536 cycle
    } en_wdt_count_cycle_t;

    /**
     *******************************************************************************
     ** \brief WDT count clock division enumeration
     ******************************************************************************/
    typedef enum en_wdt_clk_div
    {
        WdtPclk3Div4 = 2u,     ///< PCLK3/4
        WdtPclk3Div64 = 6u,    ///< PCLK3/64
        WdtPclk3Div128 = 7u,   ///< PCLK3/128
        WdtPclk3Div256 = 8u,   ///< PCLK3/256
        WdtPclk3Div512 = 9u,   ///< PCLK3/512
        WdtPclk3Div1024 = 10u, ///< PCLK3/1024
        WdtPclk3Div2048 = 11u, ///< PCLK3/2048
        WdtPclk3Div8192 = 13u, ///< PCLK3/8192
    } en_wdt_clk_div_t;

    /**
     *******************************************************************************
     ** \brief WDT allow refresh percent range enumeration
     ******************************************************************************/
    typedef enum en_wdt_refresh_range
    {
        WdtRefresh100Pct = 0u,                  ///< 100%
        WdtRefresh0To25Pct = 1u,                ///< 0%~25%
        WdtRefresh25To50Pct = 2u,               ///< 25%~50%
        WdtRefresh0To50Pct = 3u,                ///< 0%~50%
        WdtRefresh50To75Pct = 4u,               ///< 50%~75%
        WdtRefresh0To25PctAnd50To75Pct = 5u,    ///< 0%~25% & 50%~75%
        WdtRefresh25To75Pct = 6u,               ///< 25%~75%
        WdtRefresh0To75Pct = 7u,                ///< 0%~75%
        WdtRefresh75To100Pct = 8u,              ///< 75%~100%
        WdtRefresh0To25PctAnd75To100Pct = 9u,   ///< 0%~25% & 75%~100%
        WdtRefresh25To50PctAnd75To100Pct = 10u, ///< 25%~50% & 75%~100%
        WdtRefresh0To50PctAnd75To100Pct = 11u,  ///< 0%~50% & 75%~100%
        WdtRefresh50To100Pct = 12u,             ///< 50%~100%
        WdtRefresh0To25PctAnd50To100Pct = 13u,  ///< 0%~25% & 50%~100%
        WdtRefresh25To100Pct = 14u,             ///< 25%~100%
        WdtRefresh0To100Pct = 15u,              ///< 0%~100%
    } en_wdt_refresh_range_t;

    /**
     *******************************************************************************
     ** \brief WDT refresh error or count underflow trigger event type enumeration
     ******************************************************************************/
    typedef enum en_wdt_event_request_type
    {
        WdtTriggerInterruptRequest = 0u, ///< Interrupt request
        WdtTriggerResetRequest = 1u,     ///< Reset request
    } en_wdt_event_request_type_t;

    /**
     *******************************************************************************
     ** \brief WDT flag type enumeration
     ******************************************************************************/
    typedef enum en_wdt_flag_type
    {
        WdtFlagCountUnderflow = 0u, ///< Count underflow flag
        WdtFlagRefreshError = 1u,   ///< Refresh error flag
    } en_wdt_flag_type_t;

    /**
     *******************************************************************************
     ** \brief WDT init structure definition
     ******************************************************************************/
    typedef struct stc_wdt_init
    {
        en_wdt_count_cycle_t enCountCycle;         ///< Count cycle
        en_wdt_clk_div_t enClkDiv;                 ///< Count clock division
        en_wdt_refresh_range_t enRefreshRange;     ///< Allow refresh percent range
        en_functional_state_t enSleepModeCountEn;  ///< Enable/disable count in the sleep mode
        en_wdt_event_request_type_t enRequestType;  ///< Refresh error or count underflow trigger event type
    } stc_wdt_init_t;

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
    en_result_t WDT_Init(const stc_wdt_init_t *pstcWdtInit);
    en_result_t WDT_RefreshCounter(void);
    uint16_t WDT_GetCountValue(void);

    /* Flags functions */
    en_flag_status_t WDT_GetFlag(en_wdt_flag_type_t enFlag);
    en_result_t WDT_ClearFlag(en_wdt_flag_type_t enFlag);

    //@} // WdtGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_WDT_ENABLE */

#endif /* __HC32F46x_WDT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
