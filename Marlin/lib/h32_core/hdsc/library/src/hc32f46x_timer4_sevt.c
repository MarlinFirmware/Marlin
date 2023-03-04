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
/** \file hc32f46x_timer4_sevt.c
 **
 ** A detailed description is available at
 ** @link Timer4SevtGroup Timer4SEVT description @endlink
 **
 **   - 2018-11-02  1.0 Hongjh First version for Device Driver Library of
 **                     Timer4SEVT.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_timer4_sevt.h"
#include "hc32f46x_utility.h"

#if (DDL_TIMER4_SEVT_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup Timer4SevtGroup
 ******************************************************************************/

//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Parameter validity check for Timer4 unit  */
#define IS_VALID_TIMER4(__TMRx__) \
    ((M4_TMR41 == (__TMRx__)) ||  \
     (M4_TMR42 == (__TMRx__)) ||  \
     (M4_TMR43 == (__TMRx__)))

/*!< Parameter validity check for SEVT channel  */
#define IS_VALID_SEVT_CH(x)    \
    ((Timer4SevtCh0 == (x)) || \
     (Timer4SevtCh1 == (x)) || \
     (Timer4SevtCh2 == (x)) || \
     (Timer4SevtCh3 == (x)) || \
     (Timer4SevtCh4 == (x)) || \
     (Timer4SevtCh5 == (x)))

/*!< Parameter validity check for adct buffer mode  */
#define IS_VALID_SEVT_BUF_MODE(x)        \
    ((SevtBufDisable == (x)) ||          \
     (SevtBufCntZero == (x)) ||          \
     (SevtBufCntPeak == (x)) ||          \
     (SevtBufCntZeroOrCntPeak == (x)) || \
     (SevtBufCntZeroZicZero == (x)) ||   \
     (SevtBufCntPeakPicZero == (x)) ||   \
     (SevtBufCntZeroZicZeroOrCntPeakPicZero == (x)))

/*!< Parameter validity check for SEVT trigger event  */
#define IS_VALID_SEVT_TRG_EVT(x) \
    ((SevtTrgEvtSCMUH == (x)) || \
     (SevtTrgEvtSCMUL == (x)) || \
     (SevtTrgEvtSCMVH == (x)) || \
     (SevtTrgEvtSCMVL == (x)) || \
     (SevtTrgEvtSCMWH == (x)) || \
     (SevtTrgEvtSCMWL == (x)))

/*!< Parameter validity check for SEVT OCCR selection  */
#define IS_VALID_SEVT_OCCR_SEL(x) \
    ((SevtSelOCCRxh == (x)) ||    \
     (SevtSelOCCRxl == (x)))

/*!< Parameter validity check for SEVT running mode  */
#define IS_VALID_SEVT_MODE(x)      \
    ((SevtDelayTrigMode == (x)) || \
     (SevtCompareTrigMode == (x)))

/*!< Parameter validity check for SEVT mask time */
#define IS_VALID_SEVT_MSK(x)      \
    ((Timer4SevtMask0 == (x)) ||  \
     (Timer4SevtMask1 == (x)) ||  \
     (Timer4SevtMask2 == (x)) ||  \
     (Timer4SevtMask3 == (x)) ||  \
     (Timer4SevtMask4 == (x)) ||  \
     (Timer4SevtMask5 == (x)) ||  \
     (Timer4SevtMask6 == (x)) ||  \
     (Timer4SevtMask7 == (x)) ||  \
     (Timer4SevtMask8 == (x)) ||  \
     (Timer4SevtMask9 == (x)) ||  \
     (Timer4SevtMask10 == (x)) || \
     (Timer4SevtMask11 == (x)) || \
     (Timer4SevtMask12 == (x)) || \
     (Timer4SevtMask13 == (x)) || \
     (Timer4SevtMask14 == (x)) || \
     (Timer4SevtMask15 == (x)))

/*!< Get the specified register address of the specified Timer4 unit */
#define TMR4_SCCRx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->SCCRUH + ((uint32_t)(__CH__)) * 4ul)
#define TMR4_SCSRx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->SCSRUH + ((uint32_t)(__CH__)) * 4ul)
#define TMR4_SCMRx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->SCMRUH + ((uint32_t)(__CH__)) * 4ul)

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
 ** \brief Initialize a Special-Event channel
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Timer4 SEVT channel
 ** \arg Timer4SevtCh0                  Timer4 SEVT channel:0
 ** \arg Timer4SevtCh1                  Timer4 SEVT channel:1
 ** \arg Timer4SevtCh2                  Timer4 SEVT channel:2
 ** \arg Timer4SevtCh3                  Timer4 SEVT channel:3
 ** \arg Timer4SevtCh4                  Timer4 SEVT channel:4
 ** \arg Timer4SevtCh5                  Timer4 SEVT channel:5
 ** \param [in] pstcInitCfg             The pointer of SEVT configure structure
 ** \arg This parameter detail refer @ref stc_timer4_sevt_init_t
 **
 ** \retval Ok                          Initialize successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - pstcInitCfg == NULL
 **                                     - enCh is invalid
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t TIMER4_SEVT_Init(M4_TMR4_TypeDef *TMR4x,
                             en_timer4_sevt_ch_t enCh,
                             const stc_timer4_sevt_init_t *pstcInitCfg)
{
    __IO uint16_t *pu16SCCR = NULL;
    __IO stc_tmr4_scsr_field_t stcSCSR_f;
    __IO stc_tmr4_scmr_field_t stcSCMR_f;
    __IO stc_tmr4_scsr_field_t *pstcSCSR_f = NULL;
    __IO stc_tmr4_scmr_field_t *pstcSCMR_f = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x && pstcInitCfg pointer */
    if ((IS_VALID_TIMER4(TMR4x)) && (NULL != pstcInitCfg))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_SEVT_CH(enCh));
        DDL_ASSERT(IS_VALID_SEVT_MODE(pstcInitCfg->enMode));
        DDL_ASSERT(IS_VALID_SEVT_BUF_MODE(pstcInitCfg->enBuf));
        DDL_ASSERT(IS_VALID_SEVT_MSK(pstcInitCfg->enMaskTimes));
        DDL_ASSERT(IS_VALID_SEVT_TRG_EVT(pstcInitCfg->enTrigEvt));
        DDL_ASSERT(IS_VALID_SEVT_OCCR_SEL(pstcInitCfg->enOccrSel));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpAmcZicCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpAmcPicCmd));

        enRet = Ok;
        /* Get actual address of register list of current channel */
        pu16SCCR = (__IO uint16_t *)TMR4_SCCRx(TMR4x, enCh);
        pstcSCSR_f = (__IO stc_tmr4_scsr_field_t *)TMR4_SCSRx(TMR4x, enCh);
        pstcSCMR_f = (__IO stc_tmr4_scmr_field_t *)TMR4_SCMRx(TMR4x, enCh);

        /* Configure default parameter */
        *pu16SCCR = (uint16_t)0u;
        *(__IO uint16_t *)pstcSCSR_f = (uint16_t)0x0000u;
        *(__IO uint16_t *)pstcSCMR_f = (uint16_t)0xFF00u;

        switch (pstcInitCfg->enBuf)
        {
        case SevtBufDisable:
            stcSCSR_f.BUFEN = (uint16_t)0u;
            stcSCSR_f.LMC = (uint16_t)0u;
            break;
        case SevtBufCntZero:
            stcSCSR_f.BUFEN = (uint16_t)1u;
            stcSCSR_f.LMC = (uint16_t)0u;
            break;
        case SevtBufCntPeak:
            stcSCSR_f.BUFEN = (uint16_t)2u;
            stcSCSR_f.LMC = (uint16_t)0u;
            break;
        case SevtBufCntZeroOrCntPeak:
            stcSCSR_f.BUFEN = (uint16_t)3u;
            stcSCSR_f.LMC = (uint16_t)0u;
            break;
        case SevtBufCntZeroZicZero:
            stcSCSR_f.BUFEN = (uint16_t)1u;
            stcSCSR_f.LMC = (uint16_t)1u;
            break;
        case SevtBufCntPeakPicZero:
            stcSCSR_f.BUFEN = (uint16_t)2u;
            stcSCSR_f.LMC = (uint16_t)1u;
            break;
        case SevtBufCntZeroZicZeroOrCntPeakPicZero:
            stcSCSR_f.BUFEN = (uint16_t)3u;
            stcSCSR_f.LMC = (uint16_t)1u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }

        if (Ok == enRet)
        {
            /* Configure start trigger output channel number */
            stcSCSR_f.EVTOS = (uint16_t)(pstcInitCfg->enTrigEvt);

            /* Select SEVT running mode */
            stcSCSR_f.EVTMS = (uint16_t)(pstcInitCfg->enMode);

            /* select OCO OCCR register: OCCR(x) */
            stcSCSR_f.EVTDS = (uint16_t)(pstcInitCfg->enOccrSel);

            /* Set the comparison with CNT interrupt mask counter */
            stcSCMR_f.AMC = (uint16_t)(pstcInitCfg->enMaskTimes);
            stcSCMR_f.MZCE = (uint16_t)(pstcInitCfg->enCmpAmcZicCmd);
            stcSCMR_f.MPCE = (uint16_t)(pstcInitCfg->enCmpAmcPicCmd);

            *pstcSCSR_f = stcSCSR_f;
            *pstcSCMR_f = stcSCMR_f;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Initialize a SEVT channel
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Timer4 SEVT channel
 ** \arg Timer4SevtCh0                  Timer4 SEVT channel:0
 ** \arg Timer4SevtCh1                  Timer4 SEVT channel:1
 ** \arg Timer4SevtCh2                  Timer4 SEVT channel:2
 ** \arg Timer4SevtCh3                  Timer4 SEVT channel:3
 ** \arg Timer4SevtCh4                  Timer4 SEVT channel:4
 ** \arg Timer4SevtCh5                  Timer4 SEVT channel:5
 **
 ** \retval Ok                          De-Initialize successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_SEVT_DeInit(M4_TMR4_TypeDef *TMR4x,
                               en_timer4_sevt_ch_t enCh)
{
    __IO uint16_t *pu16SCCR = NULL;
    __IO stc_tmr4_scsr_field_t *pstcSCSR_f = NULL;
    __IO stc_tmr4_scmr_field_t *pstcSCMR_f = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_SEVT_CH(enCh));

        /* Get actual address of register list of current channel */
        pu16SCCR = (__IO uint16_t *)TMR4_SCCRx(TMR4x, enCh);
        pstcSCSR_f = (__IO stc_tmr4_scsr_field_t *)TMR4_SCSRx(TMR4x, enCh);
        pstcSCMR_f = (__IO stc_tmr4_scmr_field_t *)TMR4_SCMRx(TMR4x, enCh);

        /* Configure default parameter */
        *pu16SCCR = 0u;
        *(__IO uint16_t *)pstcSCSR_f = (uint16_t)0x0000u;
        *(__IO uint16_t *)pstcSCMR_f = (uint16_t)0xFF00u;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set Timer4 SEVT trigger event.
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Timer4 SEVT channel
 ** \arg Timer4SevtCh0                  Timer4 SEVT channel:0
 ** \arg Timer4SevtCh1                  Timer4 SEVT channel:1
 ** \arg Timer4SevtCh2                  Timer4 SEVT channel:2
 ** \arg Timer4SevtCh3                  Timer4 SEVT channel:3
 ** \arg Timer4SevtCh4                  Timer4 SEVT channel:4
 ** \arg Timer4SevtCh5                  Timer4 SEVT channel:5
 ** \param [in] enTrgEvt                Timer4 Special-EVT Event
 ** \arg SevtTrgEvtSCMUH                Timer4 Special-EVT Event: TMR4_Ux_SCMUH
 ** \arg SevtTrgEvtSCMUL                Timer4 Special-EVT Event: TMR4_Ux_SCMUL
 ** \arg SevtTrgEvtSCMVH                Timer4 Special-EVT Event: TMR4_Ux_SCMVH
 ** \arg SevtTrgEvtSCMVL                Timer4 Special-EVT Event: TMR4_Ux_SCMVL
 ** \arg SevtTrgEvtSCMWH                Timer4 Special-EVT Event: TMR4_Ux_SCMWH
 ** \arg SevtTrgEvtSCMWL                Timer4 Special-EVT Event: TMR4_Ux_SCMWL
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_SEVT_SetTriggerEvent(M4_TMR4_TypeDef *TMR4x,
                                        en_timer4_sevt_ch_t enCh,
                                        en_timer4_sevt_trigger_evt_t enTrgEvt)
{
    __IO stc_tmr4_scsr_field_t *pstcSCSR_f = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_SEVT_CH(enCh));
        DDL_ASSERT(IS_VALID_SEVT_TRG_EVT(enTrgEvt));

        /* Get actual address of register list of current channel */
        pstcSCSR_f = (__IO stc_tmr4_scsr_field_t *)TMR4_SCSRx(TMR4x, enCh);
        pstcSCSR_f->EVTOS = (uint16_t)(enTrgEvt);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set Timer4 SEVT trigger condition.
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Timer4 SEVT channel
 ** \arg Timer4SevtCh0                  Timer4 SEVT channel:0
 ** \arg Timer4SevtCh1                  Timer4 SEVT channel:1
 ** \arg Timer4SevtCh2                  Timer4 SEVT channel:2
 ** \arg Timer4SevtCh3                  Timer4 SEVT channel:3
 ** \arg Timer4SevtCh4                  Timer4 SEVT channel:4
 ** \arg Timer4SevtCh5                  Timer4 SEVT channel:5
 ** \param [in] pstcTrigCond            The pointer of SEVT trigger condition structure
 ** \arg This parameter detail refer @ref stc_timer4_sevt_trigger_cond_t
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_SEVT_SetTriggerCond(M4_TMR4_TypeDef *TMR4x,
                                       en_timer4_sevt_ch_t enCh,
                                       const stc_timer4_sevt_trigger_cond_t *pstcTrigCond)
{
    __IO stc_tmr4_scsr_field_t *pstcSCSR_f = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_SEVT_CH(enCh));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTrigCond->enUpMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTrigCond->enZeroMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTrigCond->enDownMatchCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcTrigCond->enPeakMatchCmd));

        /* Get actual address of register list of current channel */
        pstcSCSR_f = (__IO stc_tmr4_scsr_field_t *)TMR4_SCSRx(TMR4x, enCh);
        pstcSCSR_f->PEN = (uint16_t)(pstcTrigCond->enPeakMatchCmd);
        pstcSCSR_f->ZEN = (uint16_t)(pstcTrigCond->enZeroMatchCmd);
        pstcSCSR_f->UEN = (uint16_t)(pstcTrigCond->enUpMatchCmd);
        pstcSCSR_f->DEN = (uint16_t)(pstcTrigCond->enDownMatchCmd);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Write compare or delay value to Timer4 SEVT
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Timer4 SEVT channel
 ** \arg Timer4SevtCh0                  Timer4 SEVT channel:0
 ** \arg Timer4SevtCh1                  Timer4 SEVT channel:1
 ** \arg Timer4SevtCh2                  Timer4 SEVT channel:2
 ** \arg Timer4SevtCh3                  Timer4 SEVT channel:3
 ** \arg Timer4SevtCh4                  Timer4 SEVT channel:4
 ** \arg Timer4SevtCh5                  Timer4 SEVT channel:5
 ** \param [in] u16SccrVal              Timer4 SEVT compare value
 **
 ** \retval Ok                          Compare or delay value to Timer4 SEVT is set
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_SEVT_WriteSCCR(M4_TMR4_TypeDef *TMR4x,
                                  en_timer4_sevt_ch_t enCh,
                                  uint16_t u16SccrVal)
{
    __IO uint16_t *pu16SCCR = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* check parameters */
        DDL_ASSERT(IS_VALID_SEVT_CH(enCh));

        /* Get actual address of register list of current channel */
        pu16SCCR = (__IO uint16_t *)TMR4_SCCRx(TMR4x, enCh);
        *pu16SCCR = u16SccrVal;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Read compare value or delay value of ATVR
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Timer4 SEVT channel
 ** \arg Timer4SevtCh0                  Timer4 SEVT channel:0
 ** \arg Timer4SevtCh1                  Timer4 SEVT channel:1
 ** \arg Timer4SevtCh2                  Timer4 SEVT channel:2
 ** \arg Timer4SevtCh3                  Timer4 SEVT channel:3
 ** \arg Timer4SevtCh4                  Timer4 SEVT channel:4
 ** \arg Timer4SevtCh5                  Timer4 SEVT channel:5
 **
 ** \retval Value of register SCCR
 **
 ******************************************************************************/
uint16_t TIMER4_SEVT_ReadSCCR(M4_TMR4_TypeDef *TMR4x,
                              en_timer4_sevt_ch_t enCh)
{
    __IO uint16_t *pu16SCCR = NULL;

    /* check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));
    DDL_ASSERT(IS_VALID_SEVT_CH(enCh));

    /* Get actual address of register list of current channel */
    pu16SCCR = (__IO uint16_t *)TMR4_SCCRx(TMR4x, enCh);

    return *pu16SCCR;
}

/**
 *******************************************************************************
 ** \brief Set Timer4 SEVT trigger event.
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Timer4 SEVT channel
 ** \arg Timer4SevtCh0                  Timer4 SEVT channel:0
 ** \arg Timer4SevtCh1                  Timer4 SEVT channel:1
 ** \arg Timer4SevtCh2                  Timer4 SEVT channel:2
 ** \arg Timer4SevtCh3                  Timer4 SEVT channel:3
 ** \arg Timer4SevtCh4                  Timer4 SEVT channel:4
 ** \arg Timer4SevtCh5                  Timer4 SEVT channel:5
 ** \param [in] enMaskTimes             Timer4 Special-EVT event mask times
 ** \arg Timer4SevtMask0                Mask 0 time.
 ** \arg Timer4SevtMask1                Mask 1 times.
 ** \arg Timer4SevtMask2                Mask 2 times.
 ** \arg Timer4SevtMask3                Mask 3 times.
 ** \arg Timer4SevtMask4                Mask 4 times.
 ** \arg Timer4SevtMask5                Mask 5 times.
 ** \arg Timer4SevtMask6                Mask 6 times.
 ** \arg Timer4SevtMask7                Mask 7 times.
 ** \arg Timer4SevtMask8                Mask 8 times.
 ** \arg Timer4SevtMask9                Mask 9 times.
 ** \arg Timer4SevtMask10               Mask 10 times
 ** \arg Timer4SevtMask11               Mask 11 times
 ** \arg Timer4SevtMask12               Mask 12 times
 ** \arg Timer4SevtMask13               Mask 13 times
 ** \arg Timer4SevtMask14               Mask 14 times
 ** \arg Timer4SevtMask15               Mask 15 times
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_SEVT_SetMaskTimes(M4_TMR4_TypeDef *TMR4x,
                                     en_timer4_sevt_ch_t enCh,
                                     en_timer4_sevt_mask_t enMaskTimes)
{
    __IO stc_tmr4_scmr_field_t *pstcSCMR_f = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_SEVT_CH(enCh));
        DDL_ASSERT(IS_VALID_SEVT_MSK(enMaskTimes));

        /* Get actual address of register list of current channel */
        pstcSCMR_f = (__IO stc_tmr4_scmr_field_t *)TMR4_SCMRx(TMR4x, enCh);
        pstcSCMR_f->AMC = (uint16_t)(enMaskTimes);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timer4 SEVT mask count.
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Timer4 SEVT channel
 ** \arg Timer4SevtCh0                  Timer4 SEVT channel:0
 ** \arg Timer4SevtCh1                  Timer4 SEVT channel:1
 ** \arg Timer4SevtCh2                  Timer4 SEVT channel:2
 ** \arg Timer4SevtCh3                  Timer4 SEVT channel:3
 ** \arg Timer4SevtCh4                  Timer4 SEVT channel:4
 ** \arg Timer4SevtCh5                  Timer4 SEVT channel:5
 **
 ** \retval Timer4SevtMask0             Mask 0 time.
 ** \retval Timer4SevtMask1             Mask 1 times.
 ** \retval Timer4SevtMask2             Mask 2 times.
 ** \retval Timer4SevtMask3             Mask 3 times.
 ** \retval Timer4SevtMask4             Mask 4 times.
 ** \retval Timer4SevtMask5             Mask 5 times.
 ** \retval Timer4SevtMask6             Mask 6 times.
 ** \retval Timer4SevtMask7             Mask 7 times.
 ** \retval Timer4SevtMask8             Mask 8 times.
 ** \retval Timer4SevtMask9             Mask 9 times.
 ** \retval Timer4SevtMask10            Mask 10 times
 ** \retval Timer4SevtMask11            Mask 11 times
 ** \retval Timer4SevtMask12            Mask 12 times
 ** \retval Timer4SevtMask13            Mask 13 times
 ** \retval Timer4SevtMask14            Mask 14 times
 ** \retval Timer4SevtMask15            Mask 15 times
 **
 ******************************************************************************/
en_timer4_sevt_mask_t TIMER4_SEVT_GetMaskTimes(M4_TMR4_TypeDef *TMR4x,
                                               en_timer4_sevt_ch_t enCh)
{
    __IO stc_tmr4_scmr_field_t *pstcSCMR_f = NULL;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));
    DDL_ASSERT(IS_VALID_SEVT_CH(enCh));

    /* Get actual address of register list of current channel */
    pstcSCMR_f = (__IO stc_tmr4_scmr_field_t *)TMR4_SCMRx(TMR4x, enCh);

    return (en_timer4_sevt_mask_t)pstcSCMR_f->AMC;
}

//@} // Timer4SevtGroup

#endif /* DDL_TIMER4_SEVT_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
