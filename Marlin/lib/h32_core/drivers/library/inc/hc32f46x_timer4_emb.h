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
/** \file hc32f46x_timer4_emb.h
 **
 ** A detailed description is available at
 ** @link Timer4EmbGroup Timer4EMB description @endlink
 **
 **   - 2018-11-02  1.0 Hongjh First version for Device Driver Library of
 **                     Timer4EMB.
 **
 ******************************************************************************/
#ifndef __HC32F46x_TIMER4_EMB_H__
#define __HC32F46x_TIMER4_EMB_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_TIMER4_EMB_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup Timer4EmbGroup Timer4 Emergency Brake(Timer4EMB)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/

    /**
     *******************************************************************************
     ** \brief Timer4 EMB hold function selection enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_emb_hold_mode
    {
        EmbChangePwm = 0u, ///< Don't hold PWM output when EMB signal occurs
        EmbHoldPwm = 1u,   ///< Hold PWM output when EMB signal occurs
    } en_timer4_emb_hold_mode_t;

    /**
     *******************************************************************************
     ** \brief Timer4 EMB state selection enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_emb_state
    {
        EmbTrigPwmOutputNormal = 0u,    ///< PWM output signal normally.
        EmbTrigPwmOutputHiz = 1u,       ///< PWM output Hiz signal.
        EmbTrigPwmOutputLowLevel = 2u,  ///< PWM output low level signal.
        EmbTrigPwmOutputHighLevel = 3u, ///< PWM output high level signal.
    } en_timer4_emb_state_t;

    /**
     *******************************************************************************
     ** \brief Timer4 EMB configure
     **
     ******************************************************************************/
    typedef struct stc_timer4_emb_init
    {
        en_timer4_emb_state_t enEmbState; ///< Timer4 EMB state selection and this parameter can be a value of @ref en_timer4_emb_state_t

        en_timer4_emb_hold_mode_t enPwmHold; ///< Timer4 EMB hold function selection and this parameter can be a value of @ref en_timer4_emb_hold_mode_t
    } stc_timer4_emb_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t TIMER4_EMB_Init(M4_TMR4_TypeDef *TMR4x,
                                const stc_timer4_emb_init_t *pstcInitCfg);
    en_result_t TIMER4_EMB_DeInit(M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_EMB_SetHoldMode(M4_TMR4_TypeDef *TMR4x,
                                       en_timer4_emb_hold_mode_t enHoldMode);
    en_timer4_emb_hold_mode_t TIMER4_EMB_GetHoldMode(M4_TMR4_TypeDef *TMR4x);
    en_result_t TIMER4_EMB_SetState(const M4_TMR4_TypeDef *TMR4x,
                                    en_timer4_emb_state_t enEmbState);
    en_timer4_emb_state_t TIMER4_EMB_GetState(const M4_TMR4_TypeDef *TMR4x);

    //@} // Timer4EmbGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_TIMER4_EMB_ENABLE */

#endif /* __HC32F46x_TIMER4_EMB_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
