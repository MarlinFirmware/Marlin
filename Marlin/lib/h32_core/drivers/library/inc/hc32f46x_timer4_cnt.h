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
/** \file hc32f46x_timer4_cnt.h
 **
 ** A detailed description is available at
 ** @link Timer4CntGroup Timer4CNT description @endlink
 **
 **   - 2018-11-02  1.0 Hongjh First version for Device Driver Library of
 **                     Timer4CNT.
 **
 ******************************************************************************/
#ifndef __HC32F46x_TIMER4_CNT_H__
#define __HC32F46x_TIMER4_CNT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_TIMER4_CNT_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup Timer4CntGroup Timer4 Counter(Timer4CNT)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief Timer4 count mode enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_cnt_mode
    {
        Timer4CntSawtoothWave = 0u,   ///< Timer4 count mode:sawtooth wave
        Timer4CntTriangularWave = 1u, ///< Timer4 count mode:triangular wave
    } en_timer4_cnt_mode_t;

    /**
     *******************************************************************************
     ** \brief CNT Clock Setting
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief Timer4 CNT clock division enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_cnt_clk_div
    {
        Timer4CntPclkDiv1 = 0u,     ///< Timer4 clock: PCLK
        Timer4CntPclkDiv2 = 1u,     ///< Timer4 clock: PCLK/2
        Timer4CntPclkDiv4 = 2u,     ///< Timer4 clock: PCLK/4
        Timer4CntPclkDiv8 = 3u,     ///< Timer4 clock: PCLK/8
        Timer4CntPclkDiv16 = 4u,    ///< Timer4 clock: PCLK/16
        Timer4CntPclkDiv32 = 5u,    ///< Timer4 clock: PCLK/32
        Timer4CntPclkDiv64 = 6u,    ///< Timer4 clock: PCLK/64
        Timer4CntPclkDiv128 = 7u,   ///< Timer4 clock: PCLK/128
        Timer4CntPclkDiv256 = 8u,   ///< Timer4 clock: PCLK/256
        Timer4CntPclkDiv512 = 9u,   ///< Timer4 clock: PCLK/512
        Timer4CntPclkDiv1024 = 10u, ///< Timer4 clock: PCLK/1024
    } en_timer4_cnt_clk_div_t;

    /**
     *******************************************************************************
     ** \brief Timer4 CNT clock soucre selection enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_cnt_clk
    {
        Timer4CntPclk = 0u,   ///<  Uses the internal clock (PCLK) as CNT's count clock.
        Timer4CntExtclk = 1u, ///<  Uses an external input clock (EXCK) as CNT's count clock.
    } en_timer4_cnt_clk_t;

    /**
     *******************************************************************************
     ** \brief Timer4 CNT interrupt selection enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_cnt_int
    {
        Timer4CntZeroMatchInt = (1ul << 8),  ///<  zero match interrupt
        Timer4CntPeakMatchInt = (1ul << 13), ///<  peak match interrupt
    } en_timer4_cnt_int_t;

    /**
     *******************************************************************************
     ** \brief Timer4 CNT interrupt mask times enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_cnt_int_mask
    {
        Timer4CntIntMask0 = 0u,   ///< CNT interrupt flag is always set(not masked) for every CNT count at "0x0000" or peak.
        Timer4CntIntMask1 = 1u,   ///< CNT interrupt flag is set once for 2 every CNT counts at "0x0000" or peak (skiping 1 count).
        Timer4CntIntMask2 = 2u,   ///< CNT interrupt flag is set once for 3 every CNT counts at "0x0000" or peak (skiping 2 count).
        Timer4CntIntMask3 = 3u,   ///< CNT interrupt flag is set once for 4 every CNT counts at "0x0000" or peak (skiping 3 count).
        Timer4CntIntMask4 = 4u,   ///< CNT interrupt flag is set once for 5 every CNT counts at "0x0000" or peak (skiping 4 count).
        Timer4CntIntMask5 = 5u,   ///< CNT interrupt flag is set once for 6 every CNT counts at "0x0000" or peak (skiping 5 count).
        Timer4CntIntMask6 = 6u,   ///< CNT interrupt flag is set once for 7 every CNT counts at "0x0000" or peak (skiping 6 count).
        Timer4CntIntMask7 = 7u,   ///< CNT interrupt flag is set once for 8 every CNT counts at "0x0000" or peak (skiping 7 count).
        Timer4CntIntMask8 = 8u,   ///< CNT interrupt flag is set once for 9 every CNT counts at "0x0000" or peak (skiping 8 count).
        Timer4CntIntMask9 = 9u,   ///< CNT interrupt flag is set once for 10 every CNT counts at "0x0000" or peak (skiping 9 count).
        Timer4CntIntMask10 = 10u, ///< CNT interrupt flag is set once for 11 every CNT counts at "0x0000" or peak (skiping 10 count).
        Timer4CntIntMask11 = 11u, ///< CNT interrupt flag is set once for 12 every CNT counts at "0x0000" or peak (skiping 11 count).
        Timer4CntIntMask12 = 12u, ///< CNT interrupt flag is set once for 13 every CNT counts at "0x0000" or peak (skiping 12 count).
        Timer4CntIntMask13 = 13u, ///< CNT interrupt flag is set once for 14 every CNT counts at "0x0000" or peak (skiping 13 count).
        Timer4CntIntMask14 = 14u, ///< CNT interrupt flag is set once for 15 every CNT counts at "0x0000" or peak (skiping 14 count).
        Timer4CntIntMask15 = 15u, ///< CNT interrupt flag is set once for 16 every CNT counts at "0x0000" or peak (skiping 15 count).
    } en_timer4_cnt_int_mask_t;

    /**
     *******************************************************************************
     ** \brief Timer4 CNT initialization configuration
     **
     ******************************************************************************/
    typedef struct stc_timer4_cnt_init
    {
        uint16_t u16Cycle; ///< CNT cycle

        en_timer4_cnt_mode_t enCntMode; ///< CNT count mode and this parameter can be a value of @ref en_timer4_cnt_mode_t

        en_timer4_cnt_clk_t enClk; ///< CNT Count clock and this parameter can be a value of @ref en_timer4_cnt_clk_t

        en_timer4_cnt_clk_div_t enClkDiv; ///< CNT clock divide and this parameter can be a value of @ref en_timer4_cnt_clk_div_t

        en_functional_state_t enBufferCmd; ///< Disable: Disable buffer function; Enable:Enable buffer function

        en_functional_state_t enZeroIntCmd; ///< Disable: Disable zero match interrupt; Enable:zero match interrupt

        en_functional_state_t enPeakIntCmd; ///< Disable: Disable peak match interrupt; Enable:peak match interrupt

        en_timer4_cnt_int_mask_t enZeroIntMsk; ///< CNT zero interrupt mask times and this parameter can be a value of @ref en_timer4_cnt_int_mask_t

        en_timer4_cnt_int_mask_t enPeakIntMsk; ///< CNT peak interrupt mask times and this parameter can be a value of @ref en_timer4_cnt_int_mask_t

    } stc_timer4_cnt_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t TIMER4_CNT_Init(M4_TMR4_TypeDef *TMR4x,
                                const stc_timer4_cnt_init_t *pstcInitCfg);
    en_result_t TIMER4_CNT_DeInit(M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_CNT_SetClock(M4_TMR4_TypeDef *TMR4x,
                                    en_timer4_cnt_clk_t enCntClk);
    en_timer4_cnt_clk_t TIMER4_CNT_GetClock(M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_CNT_SetClockDiv(M4_TMR4_TypeDef *TMR4x,
                                       en_timer4_cnt_clk_div_t enClkDiv);
    en_timer4_cnt_clk_div_t TIMER4_CNT_GetClockDiv(M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_CNT_SetMode(M4_TMR4_TypeDef *TMR4x,
                                   en_timer4_cnt_mode_t enMode);
    en_timer4_cnt_mode_t TIMER4_CNT_GetMode(M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_CNT_Start(M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_CNT_Stop(M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_CNT_IrqCmd(M4_TMR4_TypeDef *TMR4x,
                                  en_timer4_cnt_int_t enIntType,
                                  en_functional_state_t enCmd);
    en_flag_status_t TIMER4_CNT_GetIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                           en_timer4_cnt_int_t enIntType);
    en_result_t TIMER4_CNT_ClearIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                        en_timer4_cnt_int_t enIntType);
    en_result_t TIMER4_CNT_SetCycleVal(M4_TMR4_TypeDef *TMR4x, uint16_t u16Cycle);
    uint16_t TIMER4_CNT_GetCycleVal(const M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_CNT_ClearCountVal(M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_CNT_SetCountVal(M4_TMR4_TypeDef *TMR4x, uint16_t u16Count);
    uint16_t TIMER4_CNT_GetCountVal(const M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_CNT_SetIntMaskTimes(M4_TMR4_TypeDef *TMR4x,
                                           en_timer4_cnt_int_t enIntType,
                                           en_timer4_cnt_int_mask_t enMaskTimes);
    en_timer4_cnt_int_mask_t TIMER4_CNT_GetIntMaskTimes(M4_TMR4_TypeDef *TMR4x,
                                                        en_timer4_cnt_int_t enIntType);

    //@} // Timer4CntGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_TIMER4_CNT_ENABLE */

#endif /* __HC32F46x_TIMER4_CNT_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
