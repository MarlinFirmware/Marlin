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
/** \file hc32f46x_timer4_pwm.h
 **
 ** A detailed description is available at
 ** @link Timer4PwmGroup Timer4PWM description @endlink
 **
 **   - 2018-11-02  1.0 Hongjh First version for Device Driver Library of
 **                     Timer4PWM.
 **
 ******************************************************************************/
#ifndef __HC32F46x_TIMER4_PWM_H__
#define __HC32F46x_TIMER4_PWM_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_TIMER4_PWM_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup Timer4PwmGroup Timer4 Pulse-Width Modulation(Timer4PWM)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /*******************************************************************************
     ** \brief Timer4 PWM channel enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_pwm_ch
    {
        Timer4PwmU = 0u, ///< Timer4 PWM couple channel: U
        Timer4PwmV = 1u, ///< Timer4 PWM couple channel: V
        Timer4PwmW = 2u, ///< Timer4 PWM couple channel: W
    } en_timer4_pwm_ch_t;

    /*******************************************************************************
     ** \brief Timer4 PWM operation mode enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_pwm_mode
    {
        PwmThroughMode = 0u,         ///< through mode
        PwmDeadTimerMode = 1u,       ///< Dead timer mode
        PwmDeadTimerFilterMode = 2u, ///< Dead timer filter mode
    } en_timer4_pwm_mode_t;

    /*******************************************************************************
     ** \brief Timer4 PWM DMOD bit setting enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_pwm_output_state
    {
        PwmHPwmLHold = 0u,        ///< Output PWML and PWMH signals without changing the level
        PwmHPwmLReverse = 1u,     ///< Output both PWML and PWMH signals reversed
        PwmHReversePwmLHold = 2u, ///< Output the PWMH signal reversed, outputs the PWML signal without changing the level.
        PwmHHoldPwmLReverse = 3u, ///< Output the PWMH signal without changing the level, Outputs the PWML signal reversed.
    } en_timer4_pwm_output_state_t;

    /*******************************************************************************
     ** \brief Timer4 PWM count clock prescaler enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_pwm_timer_clk_div
    {
        PwmPlckDiv1 = 0u,   ///< PWM timer clock prescaler: None
        PwmPlckDiv2 = 1u,   ///< PWM timer clock prescaler: 1/2
        PwmPlckDiv4 = 2u,   ///< PWM timer clock prescaler: 1/4
        PwmPlckDiv8 = 3u,   ///< PWM timer clock prescaler: 1/8
        PwmPlckDiv16 = 4u,  ///< PWM timer clock prescaler: 1/16
        PwmPlckDiv32 = 5u,  ///< PWM timer clock prescaler: 1/32
        PwmPlckDiv64 = 6u,  ///< PWM timer clock prescaler: 1/64
        PwmPlckDiv128 = 7u, ///< PWM timer clock prescaler: 1/128
    } en_timer4_pwm_timer_clk_div_t;

    /*******************************************************************************
     ** \brief Timer4 PWM initialization configuration
     **
     ******************************************************************************/
    typedef struct stc_timer4_pwm_init
    {
        en_timer4_pwm_mode_t enMode; ///< Select PWM mode and this parameter can be a value of @ref en_timer4_pwm_mode_t

        en_timer4_pwm_timer_clk_div_t enClkDiv; ///< Clock division of PWM timer and this parameter can be a value of @ref en_timer4_pwm_timer_clk_div_t

        en_timer4_pwm_output_state_t enOutputState; ///< Polarity for PWMH and PWML signal output and this parameter can be a value of @ref en_timer4_pwm_output_state_t

        en_functional_state_t enRtIntMaskCmd; ///< Enable: Enable mask reload-timer interrupt, FALSE: don't mask reload-timer interrupt
    } stc_timer4_pwm_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t TIMER4_PWM_Init(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_pwm_ch_t enCh,
                                const stc_timer4_pwm_init_t *pstcInitCfg);
    en_result_t TIMER4_PWM_DeInit(M4_TMR4_TypeDef *TMR4x,
                                  en_timer4_pwm_ch_t enCh);
    en_result_t TIMER4_PWM_StartTimer(M4_TMR4_TypeDef *TMR4x,
                                      en_timer4_pwm_ch_t enCh);
    en_result_t TIMER4_PWM_StopTimer(M4_TMR4_TypeDef *TMR4x,
                                     en_timer4_pwm_ch_t enCh);
    en_flag_status_t TIMER4_PWM_GetIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                           en_timer4_pwm_ch_t enCh);
    en_result_t TIMER4_PWM_ClearIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                        en_timer4_pwm_ch_t enCh);
    en_result_t TIMER4_PWM_WriteDeadRegionValue(M4_TMR4_TypeDef *TMR4x,
                                                en_timer4_pwm_ch_t enCh,
                                                uint16_t u16PDAR,
                                                uint16_t u16PDBR);
    en_result_t TIMER4_PWM_ReadDeadRegionValue(M4_TMR4_TypeDef *TMR4x,
                                               en_timer4_pwm_ch_t enCh,
                                               uint16_t *u16PDAR,
                                               uint16_t *u16PDBR);
    en_result_t TIMER4_PWM_SetFilterCountValue(M4_TMR4_TypeDef *TMR4x,
                                               en_timer4_pwm_ch_t enCh,
                                               uint16_t u16Count);

    //@} // Timer4PwmGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_TIMER4_PWM_ENABLE */

#endif /* __HC32F46x_TIMER4_PWM_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
