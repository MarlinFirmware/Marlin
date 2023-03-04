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
/** \file hc32f46x_emb.h
 **
 ** A detailed description is available at
 ** @link EMBGroup EMB description @endlink
 **
 **   - 2018-11-24  1.0  Husj  First version for Device Driver Library of EMB.
 **
 ******************************************************************************/
#ifndef __HC32F46x_EMB_H__
#define __HC32F46x_EMB_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_EMB_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup EMBGroup Emergency Brake(EMB)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/

    /**
     *******************************************************************************
     ** \brief EMB status enumeration
     ******************************************************************************/
    typedef enum en_emb_status
    {
        EMBFlagPortIn = 0u,  ///< EMB port in brake flag
        EMBFlagPWMSame = 1u, ///< EMB PWM same brake flag
        EMBFlagCmp = 2u,     ///< EMB CMP brake flag
        EMBFlagOSCFail = 3u, ///< EMB oscillator fail brake flag
        EMBPortInState = 4u, ///< EMB port in state
        EMBPWMState = 5u,    ///< EMB PWM same state
    } en_emb_status_t;

    /**
     *******************************************************************************
     ** \brief EMB status clear(recover) enumeration
     ******************************************************************************/
    typedef enum en_emb_status_clr
    {
        EMBPortInFlagClr = 0u,  ///< EMB port in brake flag clear
        EMBPWMSameFlagCLr = 1u, ///< EMB PWM same brake flag clear
        EMBCmpFlagClr = 2u,     ///< EMB CMP brake flag clear
        EMBOSCFailFlagCLr = 3u, ///< EMB oscillator fail brake flag clear
    } en_emb_status_clr_t;

    /**
     *******************************************************************************
     ** \brief EMB irq enumeration
     ******************************************************************************/
    typedef enum en_emb_irq_type
    {
        PORTBrkIrq = 0u,    ///< EMB port brake interrupt
        PWMSmBrkIrq = 1u,   ///< EMB PWM same brake interrupt
        CMPBrkIrq = 2u,     ///< EMB CMP brake interrupt
        OSCFailBrkIrq = 3u, ///< EMB oscillator fail brake interrupt
    } en_emb_irq_type_t;

    /**
     *******************************************************************************
     ** \brief EMB port in filter enumeration
     ******************************************************************************/
    typedef enum en_emb_port_filter
    {
        EMBPortFltDiv0 = 0u,   ///< EMB port in filter with PCLK clock
        EMBPortFltDiv8 = 1u,   ///< EMB port in filter with PCLK/8 clock
        EMBPortFltDiv32 = 2u,  ///< EMB port in filter with PCLK/32 clock
        EMBPortFltDiv128 = 3u, ///< EMB port in filter with PCLK/128 clock
    } en_emb_port_filter_t;

    /**
     *******************************************************************************
     ** \brief EMB CR0 for timer6 config
     ** \note
     ******************************************************************************/
    typedef struct stc_emb_ctrl_timer6
    {
        bool bEnPortBrake;                      ///< Enable port brake
        bool bEnCmp1Brake;                      ///< Enable CMP1 brake
        bool bEnCmp2Brake;                      ///< Enable CMP2 brake
        bool bEnCmp3Brake;                      ///< Enable CMP3 brake
        bool bEnOSCFailBrake;                   ///< Enable OSC fail brake
        bool bEnTimer61PWMSBrake;               ///< Enable tiemr61 PWM same brake
        bool bEnTimer62PWMSBrake;               ///< Enable tiemr62 PWM same brake
        bool bEnTimer63PWMSBrake;               ///< Enable tiemr63 PWM same brake
        en_emb_port_filter_t enPortInFltClkSel; ///< Port in filter clock selection
        bool bEnPorInFlt;                       ///< Enable port in filter
        bool bEnPortInLevelSel_Low;             ///< Poit input active level 1: LowLevel 0:HighLevel
    } stc_emb_ctrl_timer6_t;

    /**
     *******************************************************************************
     ** \brief EMB CR1~3 for timer4x config
     ** \note
     ******************************************************************************/
    typedef struct stc_emb_ctrl_timer4
    {
        bool bEnPortBrake;                      ///< Enable port brake
        bool bEnCmp1Brake;                      ///< Enable CMP1 brake
        bool bEnCmp2Brake;                      ///< Enable CMP2 brake
        bool bEnCmp3Brake;                      ///< Enable CMP3 brake
        bool bEnOSCFailBrake;                   ///< Enable OS fail brake
        bool bEnTimer4xWHLSammeBrake;           ///< Enable tiemr4x PWM WH WL same brake
        bool bEnTimer4xVHLSammeBrake;           ///< Enable tiemr4x PWM VH VL same brake
        bool bEnTimer4xUHLSammeBrake;           ///< Enable tiemr4x PWM UH UL same brake
        en_emb_port_filter_t enPortInFltClkSel; ///< Port in filter clock selection
        bool bEnPorInFlt;                       ///< Enable port in filter
        bool bEnPortInLevelSel_Low;             ///< Poit input active level 1: LowLevel 0:HighLevel
    } stc_emb_ctrl_timer4_t;

    /**
     *******************************************************************************
     ** \brief EMB PWM level detect timer6 config
     ** \note
     ******************************************************************************/
    typedef struct stc_emb_pwm_level_timer6
    {
        bool bEnTimer61HighLevelDect; ///< Enable tiemr61 active detected level 1:HighLevel 0:LowLevel
        bool bEnTimer62HighLevelDect; ///< Enable tiemr62 active detected level 1:HighLevel 0:LowLevel
        bool bEnTimer63HighLevelDect; ///< Enable tiemr63 active detected level 1:HighLevel 0:LowLevel
    } stc_emb_pwm_level_timer6_t;

    /**
     *******************************************************************************
     ** \brief EMB PWM level detect timer4x config
     ** \note
     ******************************************************************************/
    typedef struct stc_emb_pwm_level_timer4
    {
        bool bEnUHLPhaseHighLevelDect; ///< Enable tiemr4x UH UL active detected level 1:HighLevel 0:LowLevel
        bool bEnVHLPhaseHighLevelDect; ///< Enable tiemr4x VH VL active detected level 1:HighLevel 0:LowLevel
        bool bEnWHLphaseHighLevelDect; ///< Enable tiemr4x WH WL active detected level 1:HighLevel 0:LowLevel
    } stc_emb_pwm_level_timer4_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    /* IRQ config */
    en_result_t EMB_ConfigIrq(M4_EMB_TypeDef *EMBx,
                              en_emb_irq_type_t enEMBIrq,
                              bool bEn);
    /* Get status(flag) */
    bool EMB_GetStatus(M4_EMB_TypeDef *EMBx, en_emb_status_t enStatus);
    /*  Status(flag) clear (recover) */
    en_result_t EMB_ClrStatus(M4_EMB_TypeDef *EMBx,
                              en_emb_status_clr_t enStatusClr);
    /*  Control Register(CTL) config for timer6 */
    en_result_t EMB_Config_CR_Timer6(const stc_emb_ctrl_timer6_t *pstcEMBConfigCR);
    /*  Control Register(CTL) config for timer4 */
    en_result_t EMB_Config_CR_Timer4(M4_EMB_TypeDef *EMBx,
                                     const stc_emb_ctrl_timer4_t *pstcEMBConfigCR);
    /*  PWM level detect (short detection) selection config for timer6 */
    en_result_t EMB_PWMLv_Timer6(const stc_emb_pwm_level_timer6_t *pstcEMBPWMlv);
    /*  PWM level detect (short detection) selection config for timer4 */
    en_result_t EMB_PWMLv_Timer4(M4_EMB_TypeDef *EMBx,
                                 const stc_emb_pwm_level_timer4_t *pstcEMBPWMlv);
    /*  Software brake */
    en_result_t EMB_SwBrake(M4_EMB_TypeDef *EMBx, bool bEn);

    //@} // EMBGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_EMB_ENABLE */

#endif /* __HC32F46x_EMB_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
