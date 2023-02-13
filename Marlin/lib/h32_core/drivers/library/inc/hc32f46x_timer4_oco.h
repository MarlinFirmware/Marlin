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
/** \file hc32f46x_timer4_oco.h
 **
 ** A detailed description is available at
 ** @link Timer4OcoGroup Timer4OCO description @endlink
 **
 **   - 2018-11-02  1.0 Hongjh First version for Device Driver Library of
 **                     Timer4OCO.
 **
 ******************************************************************************/
#ifndef __HC32F46x_TIMER4_OCO_H__
#define __HC32F46x_TIMER4_OCO_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_TIMER4_OCO_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup Timer4OcoGroup Timer4 Output Compare(Timer4OCO)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief Timer4 OCO channel enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_oco_ch
    {
        Timer4OcoOuh = 0u, ///< Timer4 OCO channel:OUH
        Timer4OcoOul = 1u, ///< Timer4 OCO channel:OUL
        Timer4OcoOvh = 2u, ///< Timer4 OCO channel:OVH
        Timer4OcoOvl = 3u, ///< Timer4 OCO channel:OVL
        Timer4OcoOwh = 4u, ///< Timer4 OCO channel:OWH
        Timer4OcoOwl = 5u, ///< Timer4 OCO channel:OWL
    } en_timer4_oco_ch_t;

    /**
     *******************************************************************************
     ** \brief output level of the OC port enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_oco_port_level
    {
        OcPortLevelLow = 0u,  ///< Output low level to OC port
        OcPortLevelHigh = 1u, ///< Output high level to OC port
    } en_timer4_oco_port_level_t;

    /**
     *******************************************************************************
     ** \brief buffer register function of OCCR
     **
     ******************************************************************************/
    typedef enum en_timer4_oco_occr_buf
    {
        OccrBufDisable = 0u,                              ///< Disable the register buffer function
        OccrBufTrsfByCntZero = 1u,                        ///< Register buffer transfer when counter value is 0x0000
        OccrBufTrsfByCntPeak = 2u,                        ///< Register buffer transfer when counter value is CPSR
        OccrBufTrsfByCntZeroOrCntPeak = 3u,               ///< Register buffer transfer when the value is both 0 and CPSR
        OccrBufTrsfByCntZeroZicZero = 4u,                 ///< Register buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0
        OccrBufTrsfByCntPeakPicZero = 5u,                 ///< Register buffer transfer when counter value is CPSR and peak value detection mask counter value is 0
        OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero = 6u, ///< Register buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0 or
                                                          ///< when counter value is CPSR and peak value detection mask counter value is 0
    } en_timer4_oco_occr_buf_t;

    /**
     *******************************************************************************
     ** \brief buffer register function of OCMR
     **
     ******************************************************************************/
    typedef enum en_timer4_oco_ocmr_buf
    {
        OcmrBufDisable = 0u,                              ///< Disable the register buffer function
        OcmrBufTrsfByCntZero = 1u,                        ///< Register buffer transfer when counter value is 0x0000
        OcmrBufTrsfByCntPeak = 2u,                        ///< Register buffer transfer when counter value is CPSR
        OcmrBufTrsfByCntZeroOrCntPeak = 3u,               ///< Register buffer transfer when the value is both 0 and CPSR
        OcmrBufTrsfByCntZeroZicZero = 4u,                 ///< Register buffer transfer when CNT counter value is 0x0000 and zero value detection mask counter value is 0
        OcmrBufTrsfByCntPeakPicZero = 5u,                 ///< Register buffer transfer when CNT counter value is CPSR and peak value detection mask counter value is 0
        OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero = 6u, ///< Register buffer transfer when CNT counter value is 0x0000 and zero value detection mask counter value is 0 or
                                                          ///< when CNT counter value is CPSR and peak value detection mask counter value is 0
    } en_timer4_oco_ocmr_buf_t;

    /**
     *******************************************************************************
     ** \brief OP output status enumeration
     **
     ******************************************************************************/
    typedef enum en_timer4_oco_op_state
    {
        OcoOpOutputHold = 0u,    ///< OP output hold
        OcoOpOutputHigh = 1u,    ///< OP output high
        OcoOpOutputLow = 2u,     ///< OP output low
        OcoOpOutputReverse = 3u, ///< OP output reverse
    } en_timer4_oco_op_state_t;

    /**
     *******************************************************************************
     ** \brief The condition for OCF set
     **
     ******************************************************************************/
    typedef enum en_timer4_oco_ocf_state
    {
        OcoOcfHold = 0u, ///< OCF hold
        OcoOcfSet = 1u,  ///< OCF set
    } en_timer4_oco_ocf_state_t;

    /**
     *******************************************************************************
     ** \brief The configuration of OCO high channel(OUH/OVH/OWH)
     **
     ******************************************************************************/
    typedef struct stc_oco_high_ch_compare_mode
    {
        en_timer4_oco_op_state_t enCntZeroMatchOpState;    ///< b11~b10 High channel's OP output status when high channel match occurs at the condition of CNT count=0x0000
        en_timer4_oco_op_state_t enCntZeroNotMatchOpState; ///< b15~b14 High channel's OP output status when high channel match doesn't occur at the condition of CNT count=0x0000

        en_timer4_oco_op_state_t enCntUpCntMatchOpState; ///< b9~b8 High channel's OP output status when high channel match occurs at the condition of CNT is counting up

        en_timer4_oco_op_state_t enCntPeakMatchOpState;    ///< b7~b6 High channel's OP output status when high channel match occurs at the condition of CNT count=Peak
        en_timer4_oco_op_state_t enCntPeakNotMatchOpState; ///< b13~b12 High channel's OP output status when high channel match doesn't occur at the condition of CNT count=Peak

        en_timer4_oco_op_state_t enCntDownCntMatchOpState; ///< b5~b4 High channel's OP output status when high channel match occurs at the condition of CNT is counting down

        en_timer4_oco_ocf_state_t enCntZeroMatchOcfState;    ///< b3 High channel's OCF status when high channel match occurs at the condition of CNT count=0x0000
        en_timer4_oco_ocf_state_t enCntUpCntMatchOcfState;   ///< b2 High channel's OCF status when high channel match occurs at the condition of CNT is counting up
        en_timer4_oco_ocf_state_t enCntPeakMatchOcfState;    ///< b1 High channel's OCF status when high channel match occurs at the condition of CNT count=Peak
        en_timer4_oco_ocf_state_t enCntDownCntMatchOcfState; ///< b0 High channel's OCF status when high channel match occurs at the condition of CNT is counting down

        en_functional_state_t enMatchConditionExtendCmd; ///< Enable:  Extend the match condition
                                                         ///< Disable: Don't extend the match conditio
    } stc_oco_high_ch_compare_mode_t;

    /**
     *******************************************************************************
     ** \brief The configuration of OCO low channel(OUL/OVL/OWL)
     **
     ******************************************************************************/
    typedef struct stc_oco_low_ch_compare_mode
    {
        en_timer4_oco_op_state_t enCntZeroLowMatchHighMatchLowChOpState;       ///< b27~b26 Low channel's OP output status when high channel and low channel match occurs at the condition of CNT count=0x0000
        en_timer4_oco_op_state_t enCntZeroLowMatchHighNotMatchLowChOpState;    ///< b11~b10 Low channel's OP output status when high channel not match and low channel match occurs at the condition of CNT count=0x0000
        en_timer4_oco_op_state_t enCntZeroLowNotMatchHighMatchLowChOpState;    ///< b31~b30 Low channel's OP output status when high channel match and low channel not match occurs at the condition of CNT count=0x0000
        en_timer4_oco_op_state_t enCntZeroLowNotMatchHighNotMatchLowChOpState; ///< b15~b14 Low channel's OP output status when high channel not match and low channel not match occurs at the condition of CNT count=0x0000

        en_timer4_oco_op_state_t enCntUpCntLowMatchHighMatchLowChOpState;    ///< b25~b24 Low channel's OP output status when high channel and low channel match occurs at the condition of CNT is counting up
        en_timer4_oco_op_state_t enCntUpCntLowMatchHighNotMatchLowChOpState; ///< b9~b8 Low channel's OP output status when high channel not match and low channel match occurs at the condition of CNT is counting up
        en_timer4_oco_op_state_t enCntUpCntLowNotMatchHighMatchLowChOpState; ///< b19~b18 Low channel's OP output status when high channel match and low channel not match occurs at the condition of CNT is counting up

        en_timer4_oco_op_state_t enCntPeakLowMatchHighMatchLowChOpState;       ///< b23~b22 Low channel's OP output status when high channel and low channel match occurs at the condition of CNT count=Peak
        en_timer4_oco_op_state_t enCntPeakLowMatchHighNotMatchLowChOpState;    ///< b7~b6 Low channel's OP output status when high channel not match and low channel match occurs at the condition of CNT count=Peak
        en_timer4_oco_op_state_t enCntPeakLowNotMatchHighMatchLowChOpState;    ///< b29~b28 Low channel's OP output status when high channel match and low channel not match occurs at the condition of CNT count=Peak
        en_timer4_oco_op_state_t enCntPeakLowNotMatchHighNotMatchLowChOpState; ///< b13~b12 Low channel's OP output status when high channel not match and low channel not match occurs at the condition of CNT count=Peak

        en_timer4_oco_op_state_t enCntDownLowMatchHighMatchLowChOpState;    ///< b21~b20 Low channel's OP output status when high channel and low channel match occurs at the condition of CNT is counting down
        en_timer4_oco_op_state_t enCntDownLowMatchHighNotMatchLowChOpState; ///< b5~b4 Low channel's OP output status when high channel not match and low channel match occurs at the condition of CNT is counting down
        en_timer4_oco_op_state_t enCntDownLowNotMatchHighMatchLowChOpState; ///< b17~b16 Low channel's OP output status when high channel match and low channel not match occurs at the condition of CNT is coutning down

        en_timer4_oco_ocf_state_t enCntZeroMatchOcfState;    ///< b3 Low channel's OCF status when low channel match occurs at the condition of CNT count=0x0000
        en_timer4_oco_ocf_state_t enCntUpCntMatchOcfState;   ///< b2 Low channel's OCF status when low channel match occurs at the condition of CNT is counting up
        en_timer4_oco_ocf_state_t enCntPeakMatchOcfState;    ///< b1 Low channel's OCF status when low channel match occurs at the condition of CNT count=Peak
        en_timer4_oco_ocf_state_t enCntDownCntMatchOcfState; ///< b0 Low channel's OCF status when low channel match occurs at the condition of CNT is counting down

        en_functional_state_t enMatchConditionExtendCmd; ///< Enable:  Extend the match condition
                                                         ///< Disable: Don't extend the match conditio
    } stc_oco_low_ch_compare_mode_t;

    /**
     *******************************************************************************
     ** \brief Timer4 OCO initialization configuration
     **
     ******************************************************************************/
    typedef struct stc_timer4_oco_init
    {
        en_timer4_oco_occr_buf_t enOccrBufMode; ///< buffer register function of OCCR

        en_timer4_oco_ocmr_buf_t enOcmrBufMode; ///< buffer register function of OCMR

        en_timer4_oco_port_level_t enPortLevel; ///< OP output level state

        en_functional_state_t enOcoIntCmd; ///< Disable: Disable interrupt; Enable:Enable interrupt
    } stc_timer4_oco_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t TIMER4_OCO_Init(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_oco_ch_t enCh,
                                const stc_timer4_oco_init_t *pstcInitCfg);
    en_result_t TIMER4_OCO_DeInit(M4_TMR4_TypeDef *TMR4x,
                                  en_timer4_oco_ch_t enCh);
    en_result_t TIMER4_OCO_SetOccrBufMode(M4_TMR4_TypeDef *TMR4x,
                                          en_timer4_oco_ch_t enCh,
                                          en_timer4_oco_occr_buf_t enOccrBufMode);
    en_result_t TIMER4_OCO_SetOcmrBufMode(M4_TMR4_TypeDef *TMR4x,
                                          en_timer4_oco_ch_t enCh,
                                          en_timer4_oco_ocmr_buf_t enOcmrBufMode);
    en_result_t TIMER4_OCO_ExtMatchCondCmd(M4_TMR4_TypeDef *TMR4x,
                                           en_timer4_oco_ch_t enCh,
                                           en_functional_state_t enCmd);
    en_result_t TIMER4_OCO_SetHighChCompareMode(M4_TMR4_TypeDef *TMR4x,
                                                en_timer4_oco_ch_t enCh,
                                                const stc_oco_high_ch_compare_mode_t *pstcMode);
    en_result_t TIMER4_OCO_SetLowChCompareMode(M4_TMR4_TypeDef *TMR4x,
                                               en_timer4_oco_ch_t enCh,
                                               const stc_oco_low_ch_compare_mode_t *pstcMode);
    en_result_t TIMER4_OCO_OutputCompareCmd(M4_TMR4_TypeDef *TMR4x,
                                            en_timer4_oco_ch_t enCh,
                                            en_functional_state_t enCmd);
    en_result_t TIMER4_OCO_IrqCmd(M4_TMR4_TypeDef *TMR4x,
                                  en_timer4_oco_ch_t enCh,
                                  en_functional_state_t enCmd);
    en_flag_status_t TIMER4_OCO_GetIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                           en_timer4_oco_ch_t enCh);
    en_result_t TIMER4_OCO_ClearIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                        en_timer4_oco_ch_t enCh);
    en_result_t TIMER4_OCO_SetOpPortLevel(M4_TMR4_TypeDef *TMR4x,
                                          en_timer4_oco_ch_t enCh,
                                          en_timer4_oco_port_level_t enLevel);
    en_timer4_oco_port_level_t TIMER4_OCO_GetOpPinLevel(M4_TMR4_TypeDef *TMR4x,
                                                        en_timer4_oco_ch_t enCh);
    en_result_t TIMER4_OCO_WriteOccr(M4_TMR4_TypeDef *TMR4x,
                                     en_timer4_oco_ch_t enCh,
                                     uint16_t u16Occr);
    uint16_t TIMER4_OCO_ReadOccr(const M4_TMR4_TypeDef *TMR4x,
                                 en_timer4_oco_ch_t enCh);

    //@} // Timer4OcoGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_TIMER4_OCO_ENABLE */

#endif /* __HC32F46x_TIMER4_OCO_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
