/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
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
/** \file hc32f46x_timer0.h
 **
 ** A detailed description is available at
 ** @link Timer0Group description @endlink
 **
 **   - 2018-10-11  1.0  Wangmin  First version for Device Driver Library of TIMER0.
 **
 ******************************************************************************/

#ifndef __HC32F46x_TIMER0_H__
#define __HC32F46x_TIMER0_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/

#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_TIMER0_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup Timer0Group Timer0
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief Timer0 channel enumeration
     **
     ******************************************************************************/
    typedef enum en_tim0_channel
    {
        Tim0_ChannelA = 0x00u,
        Tim0_ChannelB = 0x01u
    } en_tim0_channel_t;

    /**
     *******************************************************************************
     ** \brief Timer0 Async Mode clock enumeration
     **
     ******************************************************************************/
    typedef enum en_tim0_async_clock_src
    {
        Tim0_LRC = 0x00u,
        Tim0_XTAL32 = 0x01u
    } en_tim0_async_clock_src_t;

    /**
     *******************************************************************************
     ** \brief Timer0 Sync Mode clock enumeration
     **
     ******************************************************************************/
    typedef enum en_tim0_sync_clock_src
    {
        Tim0_Pclk1 = 0x00u,
        Tim0_InsideHardTrig = 0x01u
    } en_tim0_sync_clock_src_t;

    /**
     *******************************************************************************
     ** \brief Timer0 counter mode enumeration
     **
     ******************************************************************************/
    typedef enum en_tim0_counter_mode
    {
        Tim0_Sync = 0x00u,
        Tim0_Async = 0x01u
    } en_tim0_counter_mode_t;

    /**
     *******************************************************************************
     ** \brief Timer0 trigger event mode enumeration
     **
     ******************************************************************************/
    typedef enum en_tim0_function
    {
        Tim0_OutputCapare = 0x00u,
        Tim0_InputCaptrue = 0x01u
    } en_tim0_function_t;

    /**
     *******************************************************************************
     ** \brief Timer0 clock division enumeration
     **
     ******************************************************************************/
    typedef enum en_tim0_clock_div
    {
        Tim0_ClkDiv0 = 0u,
        Tim0_ClkDiv2,
        Tim0_ClkDiv4,
        Tim0_ClkDiv8,
        Tim0_ClkDiv16,
        Tim0_ClkDiv32,
        Tim0_ClkDiv64,
        Tim0_ClkDiv128,
        Tim0_ClkDiv256,
        Tim0_ClkDiv512,
        Tim0_ClkDiv1024
    } en_tim0_clock_div_t;

    /**
     *******************************************************************************
     ** \brief Timer0 trigger function init structrue definition
     **
     ******************************************************************************/
    typedef struct stc_tim0_trigger_init
    {
        en_tim0_function_t Tim0_OCMode; ///< specifies the TIM mode value of @ref en_tim0_function_t
        en_event_src_t Tim0_SelTrigSrc; ///< specifies the TIM Clk Src, and this parameter can be a value of @ref en_event_src_t
        bool Tim0_InTrigEnable;         ///< specifies the TIM enable trigger
        bool Tim0_InTrigClear;          ///< specifies the TIM enable trigger clear
        bool Tim0_InTrigStop;           ///< specifies the TIM enable trigger stop
        bool Tim0_InTrigStart;          ///< specifies the TIM enable trigger start
    } stc_tim0_trigger_init_t;

    /**
     *******************************************************************************
     ** \brief Timer0 base counter function init structrue definition
     **
     ******************************************************************************/
    typedef struct stc_tim0_base_init
    {
        en_tim0_clock_div_t Tim0_ClockDivision;          ///< specifies the TIM clock division, and this parameter can be a value of @ref en_tim0_clock_div_t*/
        en_tim0_sync_clock_src_t Tim0_SyncClockSource;   ///< specifies the TIM sync clock source, and this parameter can be a value of @ref en_tim0_sync_clock_src_t*/
        en_tim0_async_clock_src_t Tim0_AsyncClockSource; ///< specifies the TIM async clock source, and this parameter can be a value of @ref en_tim0_async_clock_src_t*/
        en_tim0_counter_mode_t Tim0_CounterMode;         ///< specifies the TIM counter mode, and this parameter can be a value of @ref en_tim0_counter_mode_t*/
        uint16_t Tim0_CmpValue;                          ///< specifies the TIM counter value This value can be set 0-0xFFFF
    } stc_tim0_base_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    en_flag_status_t TIMER0_GetFlag(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh);
    en_result_t TIMER0_ClearFlag(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh);
    en_result_t TIMER0_Cmd(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh,
                           en_functional_state_t enCmd);
    en_result_t TIMER0_SetFunc(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh,
                               en_tim0_function_t enFunc);
    en_result_t TIMER0_IntCmd(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh,
                              en_functional_state_t enCmd);
    uint16_t TIMER0_GetCntReg(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh);
    en_result_t TIMER0_WriteCntReg(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh, uint16_t u16Cnt);
    uint16_t TIMER0_GetCmpReg(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh);
    en_result_t TIMER0_WriteCmpReg(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh, uint16_t u16Cnt);
    en_result_t TIMER0_BaseInit(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh,
                                const stc_tim0_base_init_t *pstcBaseInit);
    en_result_t TIMER0_DeInit(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh);
    en_result_t TIMER0_HardTriggerInit(M4_TMR0_TypeDef *pstcTim0Reg, en_tim0_channel_t enCh,
                                       const stc_tim0_trigger_init_t *pStcInit);
    void TIMER0_SetTriggerSrc(en_event_src_t enEvent);

    //@} // Timer0Group

#ifdef __cplusplus
}
#endif

#endif /* DDL_TIMER0_ENABLE */

#endif /* __HC32F46x_TIMER0_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
