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
/** \file hc32f46x_timer4_oco.c
 **
 ** A detailed description is available at
 ** @link Timer4OcoGroup Timer4OCO description @endlink
 **
 **   - 2018-11-02  1.0 Hongjh First version for Device Driver Library of
 **                     Timer4OCO.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_timer4_oco.h"
#include "hc32f46x_utility.h"

#if (DDL_TIMER4_OCO_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup Timer4OcoGroup
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

/*!< Parameter validity check for oco channel  */
#define IS_VALID_OCO_CH(x)    \
    ((Timer4OcoOuh == (x)) || \
     (Timer4OcoOul == (x)) || \
     (Timer4OcoOvh == (x)) || \
     (Timer4OcoOvl == (x)) || \
     (Timer4OcoOwh == (x)) || \
     (Timer4OcoOwl == (x)))

/*!< Parameter validity check for oco low channel  */
#define IS_VALID_OCO_LOW_CH(x) \
    ((Timer4OcoOul == (x)) ||  \
     (Timer4OcoOvl == (x)) ||  \
     (Timer4OcoOwl == (x)))

/*!< Parameter validity check for even high channel  */
#define IS_VALID_OCO_HIGH_CH(x) \
    ((Timer4OcoOuh == (x)) ||   \
     (Timer4OcoOvh == (x)) ||   \
     (Timer4OcoOwh == (x)))

/*!< Parameter validity check for occr buffer mode  */
#define IS_VALID_OCCR_BUF_MODE(x)              \
    ((OccrBufDisable == (x)) ||                \
     (OccrBufTrsfByCntZero == (x)) ||          \
     (OccrBufTrsfByCntPeak == (x)) ||          \
     (OccrBufTrsfByCntZeroOrCntPeak == (x)) || \
     (OccrBufTrsfByCntZeroZicZero == (x)) ||   \
     (OccrBufTrsfByCntPeakPicZero == (x)) ||   \
     (OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero == (x)))

/*!< Parameter validity check for ocmr buffer mode  */
#define IS_VALID_OCMR_BUF_MODE(x)              \
    ((OcmrBufDisable == (x)) ||                \
     (OcmrBufTrsfByCntZero == (x)) ||          \
     (OcmrBufTrsfByCntPeak == (x)) ||          \
     (OcmrBufTrsfByCntZeroOrCntPeak == (x)) || \
     (OcmrBufTrsfByCntZeroZicZero == (x)) ||   \
     (OcmrBufTrsfByCntPeakPicZero == (x)) ||   \
     (OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero == (x)))

/*!< Parameter validity check for output level type  */
#define IS_VALID_OP_PORT_LEVEL(x) \
    ((OcPortLevelLow == (x)) ||   \
     (OcPortLevelHigh == (x)))

/*!< Parameter validity check for oco OP state  */
#define IS_VALID_OP_STATE(x)     \
    ((OcoOpOutputLow == (x)) ||  \
     (OcoOpOutputHigh == (x)) || \
     (OcoOpOutputHold == (x)) || \
     (OcoOpOutputReverse == (x)))

/*!< Parameter validity check for oco OCF state  */
#define IS_VALID_OCF_STATE(x) \
    ((OcoOcfSet == (x)) ||    \
     (OcoOcfHold == (x)))

/*!< Get the specified register address of the specified Timer4 unit */
#define TMR4_OCCRx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->OCCRUH + ((uint32_t)(__CH__)) * 4ul)
#define TMR4_OCMRx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->OCMRHUH + ((uint32_t)(__CH__)) * 4ul)
#define TMR4_OCERx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->OCERU + (((uint32_t)(__CH__)) / 2ul) * 4ul)
#define TMR4_OCSRx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->OCSRU + (((uint32_t)(__CH__)) / 2ul) * 4ul)

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
 ** \brief Initialize OCO module
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] pstcInitCfg             The pointer of OCO configure structure
 ** \arg This parameter detail refer @ref stc_timer4_oco_init_t
 **
 ** \retval Ok                          Initialize successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - pstcInitCfg == NULL
 **                                     - enCh is invalid
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_Init(M4_TMR4_TypeDef *TMR4x,
                            en_timer4_oco_ch_t enCh,
                            const stc_timer4_oco_init_t *pstcInitCfg)
{
    __IO stc_tmr4_ocsr_field_t *pstcOCSR = NULL;
    __IO stc_tmr4_ocer_field_t *pstcOCER = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x && pstcInitCfg pointer */
    if ((IS_VALID_TIMER4(TMR4x)) && (NULL != pstcInitCfg))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_CH(enCh));
        DDL_ASSERT(IS_VALID_TIMER4(TMR4x));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enOcoIntCmd));
        DDL_ASSERT(IS_VALID_OP_PORT_LEVEL(pstcInitCfg->enPortLevel));
        DDL_ASSERT(IS_VALID_OCMR_BUF_MODE(pstcInitCfg->enOcmrBufMode));
        DDL_ASSERT(IS_VALID_OCCR_BUF_MODE(pstcInitCfg->enOccrBufMode));

        enRet = Ok;
        /* Get pointer of current channel OCO register address */
        pstcOCER = (__IO stc_tmr4_ocer_field_t *)TMR4_OCERx(TMR4x, enCh);
        pstcOCSR = (__IO stc_tmr4_ocsr_field_t *)TMR4_OCSRx(TMR4x, enCh);

        /* Set OCMR and OCCR buffer mode */
        if (IS_VALID_OCO_HIGH_CH(enCh)) /* channel: Timer4OcoOuh, Timer4OcoOvh, Timer4OcoOwh */
        {
            pstcOCSR->OCEH = (uint16_t)0u;
            pstcOCSR->OCFH = (uint16_t)0u;

            /* OCMR buffer */
            switch (pstcInitCfg->enOcmrBufMode)
            {
            case OcmrBufDisable:
                pstcOCER->LMMH = (uint16_t)0u;
                pstcOCER->MHBUFEN = (uint16_t)0u;
                break;
            case OcmrBufTrsfByCntZero:
                pstcOCER->LMMH = (uint16_t)0u;
                pstcOCER->MHBUFEN = (uint16_t)1u;
                break;
            case OcmrBufTrsfByCntPeak:
                pstcOCER->LMMH = (uint16_t)0u;
                pstcOCER->MHBUFEN = (uint16_t)2u;
                break;
            case OcmrBufTrsfByCntZeroOrCntPeak:
                pstcOCER->LMMH = (uint16_t)0u;
                pstcOCER->MHBUFEN = (uint16_t)3u;
                break;
            case OcmrBufTrsfByCntZeroZicZero:
                pstcOCER->LMMH = (uint16_t)1u;
                pstcOCER->MHBUFEN = (uint16_t)1u;
                break;
            case OcmrBufTrsfByCntPeakPicZero:
                pstcOCER->LMMH = (uint16_t)1u;
                pstcOCER->MHBUFEN = (uint16_t)2u;
                break;
            case OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                pstcOCER->LMMH = (uint16_t)1u;
                pstcOCER->MHBUFEN = (uint16_t)3u;
                break;
            default:
                enRet = ErrorInvalidParameter;
                break;
            }

            if (enRet == Ok)
            {
                /* OCCR buffer */
                switch (pstcInitCfg->enOccrBufMode)
                {
                case OccrBufDisable:
                    pstcOCER->LMCH = (uint16_t)0u;
                    pstcOCER->CHBUFEN = (uint16_t)0u;
                    break;
                case OccrBufTrsfByCntZero:
                    pstcOCER->LMCH = (uint16_t)0u;
                    pstcOCER->CHBUFEN = (uint16_t)1u;
                    break;
                case OccrBufTrsfByCntPeak:
                    pstcOCER->LMCH = (uint16_t)0u;
                    pstcOCER->CHBUFEN = (uint16_t)2u;
                    break;
                case OccrBufTrsfByCntZeroOrCntPeak:
                    pstcOCER->LMCH = (uint16_t)0u;
                    pstcOCER->CHBUFEN = (uint16_t)3u;
                    break;
                case OccrBufTrsfByCntZeroZicZero:
                    pstcOCER->LMCH = (uint16_t)1u;
                    pstcOCER->CHBUFEN = (uint16_t)1u;
                    break;
                case OccrBufTrsfByCntPeakPicZero:
                    pstcOCER->LMCH = (uint16_t)1u;
                    pstcOCER->CHBUFEN = (uint16_t)2u;
                    break;
                case OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                    pstcOCER->LMCH = (uint16_t)1u;
                    pstcOCER->CHBUFEN = (uint16_t)3u;
                    break;
                default:
                    enRet = ErrorInvalidParameter;
                    break;
                }
            }

            if (enRet == Ok)
            {
                /* Set initial OP level */
                pstcOCSR->OCPH = (uint16_t)(pstcInitCfg->enPortLevel);
                /* set interrupt enable */
                pstcOCSR->OCIEH = (uint16_t)(pstcInitCfg->enOcoIntCmd);
            }
        }
        else if (IS_VALID_OCO_LOW_CH(enCh)) /* channel: Timer4OcoOul, Timer4OcoOvl, Timer4OcoOwl */
        {
            pstcOCSR->OCEL = (uint16_t)0u;
            pstcOCSR->OCFL = (uint16_t)0u;

            /* OCMR buffer */
            switch (pstcInitCfg->enOcmrBufMode)
            {
            case OcmrBufDisable:
                pstcOCER->LMML = (uint16_t)0u;
                pstcOCER->MLBUFEN = (uint16_t)0u;
                break;
            case OcmrBufTrsfByCntZero:
                pstcOCER->LMML = (uint16_t)0u;
                pstcOCER->MLBUFEN = (uint16_t)1u;
                break;
            case OcmrBufTrsfByCntPeak:
                pstcOCER->LMML = (uint16_t)0u;
                pstcOCER->MLBUFEN = (uint16_t)2u;
                break;
            case OcmrBufTrsfByCntZeroOrCntPeak:
                pstcOCER->LMML = (uint16_t)0u;
                pstcOCER->MLBUFEN = (uint16_t)3u;
                break;
            case OcmrBufTrsfByCntZeroZicZero:
                pstcOCER->LMML = (uint16_t)1u;
                pstcOCER->MLBUFEN = (uint16_t)1u;
                break;
            case OcmrBufTrsfByCntPeakPicZero:
                pstcOCER->LMML = (uint16_t)1u;
                pstcOCER->MLBUFEN = (uint16_t)2u;
                break;
            case OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                pstcOCER->LMML = (uint16_t)1u;
                pstcOCER->MLBUFEN = (uint16_t)3u;
                break;
            default:
                enRet = ErrorInvalidParameter;
                break;
            }

            if (enRet == Ok)
            {
                /* OCCR buffer */
                switch (pstcInitCfg->enOccrBufMode)
                {
                case OccrBufDisable:
                    pstcOCER->LMCL = (uint16_t)0u;
                    pstcOCER->CLBUFEN = (uint16_t)0u;
                    break;
                case OccrBufTrsfByCntZero:
                    pstcOCER->LMCL = (uint16_t)0u;
                    pstcOCER->CLBUFEN = (uint16_t)1u;
                    break;
                case OccrBufTrsfByCntPeak:
                    pstcOCER->LMCL = (uint16_t)0u;
                    pstcOCER->CLBUFEN = (uint16_t)2u;
                    break;
                case OccrBufTrsfByCntZeroOrCntPeak:
                    pstcOCER->LMCL = (uint16_t)0u;
                    pstcOCER->CLBUFEN = (uint16_t)3u;
                    break;
                case OccrBufTrsfByCntZeroZicZero:
                    pstcOCER->LMCL = (uint16_t)1u;
                    pstcOCER->CLBUFEN = (uint16_t)1u;
                    break;
                case OccrBufTrsfByCntPeakPicZero:
                    pstcOCER->LMCL = (uint16_t)1u;
                    pstcOCER->CLBUFEN = (uint16_t)2u;
                    break;
                case OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                    pstcOCER->LMCL = (uint16_t)1u;
                    pstcOCER->CLBUFEN = (uint16_t)3u;
                    break;
                default:
                    enRet = ErrorInvalidParameter;
                    break;
                }
            }

            if (enRet == Ok)
            {
                /* Set initial OP level */
                pstcOCSR->OCPL = (uint16_t)(pstcInitCfg->enPortLevel);
                /* set interrupt enable */
                pstcOCSR->OCIEL = (uint16_t)(pstcInitCfg->enOcoIntCmd);
            }
        }
        else
        {
            enRet = ErrorInvalidParameter;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Initialize OCO module
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 **
 ** \retval Ok                          De-Initialize successfully.
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - enCh is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_DeInit(M4_TMR4_TypeDef *TMR4x,
                              en_timer4_oco_ch_t enCh)
{
    __IO uint16_t *pu16OCCR = NULL;
    __IO uint32_t u32OCMR = 0ul;
    __IO stc_tmr4_ocsr_field_t *pstcOCSR = NULL;
    __IO stc_tmr4_ocer_field_t *pstcOCER = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_OCO_CH(enCh));

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        enRet = Ok;
        u32OCMR = TMR4_OCMRx(TMR4x, enCh);
        pu16OCCR = (__IO uint16_t *)TMR4_OCCRx(TMR4x, enCh);
        pstcOCSR = (__IO stc_tmr4_ocsr_field_t *)TMR4_OCSRx(TMR4x, enCh);
        pstcOCER = (__IO stc_tmr4_ocer_field_t *)TMR4_OCERx(TMR4x, enCh);

        /* Set default value */
        if (IS_VALID_OCO_HIGH_CH(enCh)) /* channel: Timer4OcoOuh, Timer4OcoOvh, Timer4OcoOwh */
        {
            pstcOCSR->OCEH = (uint16_t)0u;
            pstcOCSR->OCFH = (uint16_t)0u;
            pstcOCSR->OCIEH = (uint16_t)0u;
            pstcOCSR->OCPH = (uint16_t)0u;
            pstcOCER->LMMH = (uint16_t)0u;
            pstcOCER->MHBUFEN = (uint16_t)0u;
            pstcOCER->LMCH = (uint16_t)0u;
            pstcOCER->CHBUFEN = (uint16_t)0u;
            pstcOCER->MCECH = (uint16_t)0u;
            *pu16OCCR = (uint16_t)0u;
            *(__IO uint16_t *)u32OCMR = (uint16_t)0u;
        }
        else if (IS_VALID_OCO_LOW_CH(enCh)) /* channel: Timer4OcoOul, Timer4OcoOvl, Timer4OcoOwl */
        {
            pstcOCSR->OCEL = (uint16_t)0u;
            pstcOCSR->OCFL = (uint16_t)0u;
            pstcOCSR->OCIEL = (uint16_t)0u;
            pstcOCSR->OCPL = (uint16_t)0u;
            pstcOCER->LMML = (uint16_t)0u;
            pstcOCER->MLBUFEN = (uint16_t)0u;
            pstcOCER->LMCL = (uint16_t)0u;
            pstcOCER->CLBUFEN = (uint16_t)0u;
            pstcOCER->MCECL = (uint16_t)0u;
            *pu16OCCR = (uint16_t)0u;
            *(__IO uint32_t *)u32OCMR = (uint32_t)0ul;
        }
        else
        {
            enRet = ErrorInvalidParameter;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set occr buffer mode
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] enOccrBufMode           Occr buffer mode
 ** \arg OccrBufDisable                 Disable the register buffer function
 ** \arg OccrBufTrsfByCntZero           Register buffer transfer when counter value is 0x0000
 ** \arg OccrBufTrsfByCntPeak           Register buffer transfer when counter value is CPSR
 ** \arg OccrBufTrsfByCntZeroOrCntPeak  Register buffer transfer when the value is both 0 and CPSR
 ** \arg OccrBufTrsfByCntZeroZicZero    Register buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0
 ** \arg OccrBufTrsfByCntPeakPicZero    Register buffer transfer when counter value is CPSR and peak value detection mask counter value is 0 **
 ** \arg OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero      Register buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0 or
 **                                                       counter value is CPSR and peak value detection mask counter value is 0
 ** \retval Ok                          OCO occr buffer mode initialized
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - enCh is invalid
 **                                     - enOccrBufMode is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_SetOccrBufMode(M4_TMR4_TypeDef *TMR4x,
                                      en_timer4_oco_ch_t enCh,
                                      en_timer4_oco_occr_buf_t enOccrBufMode)
{
    __IO stc_tmr4_ocer_field_t *pstcOCER = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_CH(enCh));
        DDL_ASSERT(IS_VALID_OCCR_BUF_MODE(enOccrBufMode));

        enRet = Ok;
        /* Get pointer of current channel OCO register address */
        pstcOCER = (__IO stc_tmr4_ocer_field_t *)TMR4_OCERx(TMR4x, enCh);

        /* Set OCCR buffer mode */
        if (IS_VALID_OCO_HIGH_CH(enCh)) /* channel: Timer4OcoOuh, Timer4OcoOvh, Timer4OcoOwh */
        {
            /* OCCR buffer */
            switch (enOccrBufMode)
            {
            case OccrBufDisable:
                pstcOCER->LMCH = (uint16_t)0u;
                pstcOCER->CHBUFEN = (uint16_t)0u;
                break;
            case OccrBufTrsfByCntZero:
                pstcOCER->LMCH = (uint16_t)0u;
                pstcOCER->CHBUFEN = (uint16_t)1u;
                break;
            case OccrBufTrsfByCntPeak:
                pstcOCER->LMCH = (uint16_t)0u;
                pstcOCER->CHBUFEN = (uint16_t)2u;
                break;
            case OccrBufTrsfByCntZeroOrCntPeak:
                pstcOCER->LMCH = (uint16_t)0u;
                pstcOCER->CHBUFEN = (uint16_t)3u;
                break;
            case OccrBufTrsfByCntZeroZicZero:
                pstcOCER->LMCH = (uint16_t)1u;
                pstcOCER->CHBUFEN = (uint16_t)1u;
                break;
            case OccrBufTrsfByCntPeakPicZero:
                pstcOCER->LMCH = (uint16_t)1u;
                pstcOCER->CHBUFEN = (uint16_t)2u;
                break;
            case OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                pstcOCER->LMCH = (uint16_t)1u;
                pstcOCER->CHBUFEN = (uint16_t)3u;
                break;
            default:
                enRet = ErrorInvalidParameter;
                break;
            }
        }
        else if (IS_VALID_OCO_LOW_CH(enCh)) /* channel: Timer4OcoOul, Timer4OcoOvl, Timer4OcoOwl */
        {
            /* OCCR buffer */
            switch (enOccrBufMode)
            {
            case OccrBufDisable:
                pstcOCER->LMCL = (uint16_t)0u;
                pstcOCER->CLBUFEN = (uint16_t)0u;
                break;
            case OccrBufTrsfByCntZero:
                pstcOCER->LMCL = (uint16_t)0u;
                pstcOCER->CLBUFEN = (uint16_t)1u;
                break;
            case OccrBufTrsfByCntPeak:
                pstcOCER->LMCL = (uint16_t)0u;
                pstcOCER->CLBUFEN = (uint16_t)2u;
                break;
            case OccrBufTrsfByCntZeroOrCntPeak:
                pstcOCER->LMCL = (uint16_t)0u;
                pstcOCER->CLBUFEN = (uint16_t)3u;
                break;
            case OccrBufTrsfByCntZeroZicZero:
                pstcOCER->LMCL = (uint16_t)1u;
                pstcOCER->CLBUFEN = (uint16_t)1u;
                break;
            case OccrBufTrsfByCntPeakPicZero:
                pstcOCER->LMCL = (uint16_t)1u;
                pstcOCER->CLBUFEN = (uint16_t)2u;
                break;
            case OccrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                pstcOCER->LMCL = (uint16_t)1u;
                pstcOCER->CLBUFEN = (uint16_t)3u;
                break;
            default:
                enRet = ErrorInvalidParameter;
                break;
            }
        }
        else
        {
            enRet = ErrorInvalidParameter;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set occr buffer mode
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] enOcmrBufMode           Occr buffer mode
 ** \arg OcmrBufDisable                 Disable the register buffer function
 ** \arg OcmrBufTrsfByCntZero           Register buffer transfer when counter value is 0x0000
 ** \arg OcmrBufTrsfByCntPeak           Register buffer transfer when counter value is CPSR
 ** \arg OcmrBufTrsfByCntZeroOrCntPeak  Register buffer transfer when the value is both 0 and CPSR
 ** \arg OcmrBufTrsfByCntZeroZicZero    Register buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0
 ** \arg OcmrBufTrsfByCntPeakPicZero    Register buffer transfer when counter value is CPSR and peak value detection mask counter value is 0 **
 ** \arg OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero      Register buffer transfer when counter value is 0x0000 and zero value detection mask counter value is 0 or
 **                                                       counter value is CPSR and peak value detection mask counter value is 0
 **
 ** \retval Ok                          OCO ocmr buffer mode initialized
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - enCh is invalid
 **                                     - enOcmrBufMode is invalid.
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_SetOcmrBufMode(M4_TMR4_TypeDef *TMR4x,
                                      en_timer4_oco_ch_t enCh,
                                      en_timer4_oco_ocmr_buf_t enOcmrBufMode)
{
    __IO stc_tmr4_ocer_field_t *pstcOCER = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_CH(enCh));
        DDL_ASSERT(IS_VALID_OCMR_BUF_MODE(enOcmrBufMode));

        enRet = Ok;
        /* Get pointer of current channel OCO register address */
        pstcOCER = (__IO stc_tmr4_ocer_field_t *)TMR4_OCERx(TMR4x, enCh);

        /* Set OCMR buffer mode */
        if (IS_VALID_OCO_HIGH_CH(enCh)) /* channel: Timer4OcoOuh, Timer4OcoOvh, Timer4OcoOwh */
        {
            /* OCMR buffer */
            switch (enOcmrBufMode)
            {
            case OcmrBufDisable:
                pstcOCER->LMMH = (uint16_t)0u;
                pstcOCER->MHBUFEN = (uint16_t)0u;
                break;
            case OcmrBufTrsfByCntZero:
                pstcOCER->LMMH = (uint16_t)0u;
                pstcOCER->MHBUFEN = (uint16_t)1u;
                break;
            case OcmrBufTrsfByCntPeak:
                pstcOCER->LMMH = (uint16_t)0u;
                pstcOCER->MHBUFEN = (uint16_t)2u;
                break;
            case OcmrBufTrsfByCntZeroOrCntPeak:
                pstcOCER->LMMH = (uint16_t)0u;
                pstcOCER->MHBUFEN = (uint16_t)3u;
                break;
            case OcmrBufTrsfByCntZeroZicZero:
                pstcOCER->LMMH = (uint16_t)1u;
                pstcOCER->MHBUFEN = (uint16_t)1u;
                break;
            case OcmrBufTrsfByCntPeakPicZero:
                pstcOCER->LMMH = (uint16_t)1u;
                pstcOCER->MHBUFEN = (uint16_t)2u;
                break;
            case OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                pstcOCER->LMMH = (uint16_t)1u;
                pstcOCER->MHBUFEN = (uint16_t)3u;
                break;
            default:
                enRet = ErrorInvalidParameter;
                break;
            }
        }
        else if (IS_VALID_OCO_LOW_CH(enCh)) /* channel: Timer4OcoOul, Timer4OcoOvl, Timer4OcoOwl */
        {
            /* OCMR buffer */
            switch (enOcmrBufMode)
            {
            case OcmrBufDisable:
                pstcOCER->LMML = (uint16_t)0u;
                pstcOCER->MLBUFEN = (uint16_t)0u;
                break;
            case OcmrBufTrsfByCntZero:
                pstcOCER->LMML = (uint16_t)0u;
                pstcOCER->MLBUFEN = (uint16_t)1u;
                break;
            case OcmrBufTrsfByCntPeak:
                pstcOCER->LMML = (uint16_t)0u;
                pstcOCER->MLBUFEN = (uint16_t)2u;
                break;
            case OcmrBufTrsfByCntZeroOrCntPeak:
                pstcOCER->LMML = (uint16_t)0u;
                pstcOCER->MLBUFEN = (uint16_t)3u;
                break;
            case OcmrBufTrsfByCntZeroZicZero:
                pstcOCER->LMML = (uint16_t)1u;
                pstcOCER->MLBUFEN = (uint16_t)1u;
                break;
            case OcmrBufTrsfByCntPeakPicZero:
                pstcOCER->LMML = (uint16_t)1u;
                pstcOCER->MLBUFEN = (uint16_t)2u;
                break;
            case OcmrBufTrsfByCntZeroZicZeroOrCntPeakPicZero:
                pstcOCER->LMML = (uint16_t)1u;
                pstcOCER->MLBUFEN = (uint16_t)3u;
                break;
            default:
                enRet = ErrorInvalidParameter;
                break;
            }
        }
        else
        {
            enRet = ErrorInvalidParameter;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Extend the matching determination conditions of OCO channel
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] enCmd                   Extend the match conditions functional state
 ** \arg Enable                         Extend the match conditions function
 ** \arg Disable                        Don't extend the match conditions function
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_ExtMatchCondCmd(M4_TMR4_TypeDef *TMR4x,
                                       en_timer4_oco_ch_t enCh,
                                       en_functional_state_t enCmd)
{
    __IO stc_tmr4_ocer_field_t *pstcOCER = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_CH(enCh));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        /* Get pointer of current channel OCO register address  */
        pstcOCER = (__IO stc_tmr4_ocer_field_t *)TMR4_OCERx(TMR4x, enCh);
        IS_VALID_OCO_HIGH_CH(enCh) ? (pstcOCER->MCECH = (uint16_t)enCmd) : (pstcOCER->MCECL = (uint16_t)enCmd);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set compare mode of OCO high channel
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] pstcMode                pointer to structure of compare mode
 ** \arg This parameter detail refer @ref stc_oco_high_ch_compare_mode_t
 **
 ** \retval Ok                          OCO high channel compare mode is set successfully.
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - pstcMode pointer is NULL
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_SetHighChCompareMode(M4_TMR4_TypeDef *TMR4x,
                                            en_timer4_oco_ch_t enCh,
                                            const stc_oco_high_ch_compare_mode_t *pstcMode)
{
    uint16_t u16OCMR = 0u;
    __IO uint16_t *pu16OCMR = NULL;
    __IO stc_tmr4_ocer_field_t *pstcOCER = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x && pstcMode pointer */
    if ((IS_VALID_TIMER4(TMR4x)) && (NULL != pstcMode))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_HIGH_CH(enCh));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntZeroMatchOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntZeroNotMatchOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntUpCntMatchOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntPeakMatchOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntPeakNotMatchOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntDownCntMatchOpState));
        DDL_ASSERT(IS_VALID_OCF_STATE(pstcMode->enCntZeroMatchOcfState));
        DDL_ASSERT(IS_VALID_OCF_STATE(pstcMode->enCntUpCntMatchOcfState));
        DDL_ASSERT(IS_VALID_OCF_STATE(pstcMode->enCntPeakMatchOcfState));
        DDL_ASSERT(IS_VALID_OCF_STATE(pstcMode->enCntDownCntMatchOcfState));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcMode->enMatchConditionExtendCmd));

        /* Get pointer of current channel OCO register address */
        pu16OCMR = (__IO uint16_t *)TMR4_OCMRx(TMR4x, enCh);
        pstcOCER = (__IO stc_tmr4_ocer_field_t *)TMR4_OCERx(TMR4x, enCh);

        pstcOCER->MCECH = (uint16_t)(pstcMode->enMatchConditionExtendCmd);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntZeroMatchOpState << 10u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntZeroNotMatchOpState << 14u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntUpCntMatchOpState << 8u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntPeakMatchOpState << 6u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntPeakNotMatchOpState << 12u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntDownCntMatchOpState << 4u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntZeroMatchOcfState << 3u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntUpCntMatchOcfState << 2u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntPeakMatchOcfState << 1u);
        u16OCMR |= (uint16_t)((uint16_t)pstcMode->enCntDownCntMatchOcfState << 0u);

        *pu16OCMR = u16OCMR;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set compare mode of OCO low channel
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] pstcMode                pointer to structure of compare mode
 ** \arg This parameter detail refer @ref TIMER4_OCO_SetLowChCompareMode
 **
 ** \retval Ok                          OCO low channel compare mode is set successfully.
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - pstcMode pointer is NULL
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_SetLowChCompareMode(M4_TMR4_TypeDef *TMR4x,
                                           en_timer4_oco_ch_t enCh,
                                           const stc_oco_low_ch_compare_mode_t *pstcMode)
{
    uint32_t u32OCMR = 0ul;
    __IO uint32_t *pu32OCMR = NULL;
    __IO stc_tmr4_ocer_field_t *pstcOCER = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer and pstcMode pointer */
    if ((IS_VALID_TIMER4(TMR4x)) && (NULL != pstcMode))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_LOW_CH(enCh));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntZeroLowMatchHighMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntZeroLowMatchHighNotMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntZeroLowNotMatchHighMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntZeroLowNotMatchHighNotMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntUpCntLowMatchHighMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntUpCntLowMatchHighNotMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntUpCntLowNotMatchHighMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntPeakLowMatchHighMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntPeakLowMatchHighNotMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntPeakLowNotMatchHighMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntPeakLowNotMatchHighNotMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntDownLowMatchHighMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntDownLowMatchHighNotMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OP_STATE(pstcMode->enCntDownLowNotMatchHighMatchLowChOpState));
        DDL_ASSERT(IS_VALID_OCF_STATE(pstcMode->enCntZeroMatchOcfState));
        DDL_ASSERT(IS_VALID_OCF_STATE(pstcMode->enCntUpCntMatchOcfState));
        DDL_ASSERT(IS_VALID_OCF_STATE(pstcMode->enCntPeakMatchOcfState));
        DDL_ASSERT(IS_VALID_OCF_STATE(pstcMode->enCntDownCntMatchOcfState));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcMode->enMatchConditionExtendCmd));

        /* Get pointer of current channel OCO register address */
        pu32OCMR = (__IO uint32_t *)TMR4_OCMRx(TMR4x, enCh);
        pstcOCER = (__IO stc_tmr4_ocer_field_t *)TMR4_OCERx(TMR4x, enCh);
        ;

        pstcOCER->MCECL = (uint16_t)(pstcMode->enMatchConditionExtendCmd);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroLowMatchHighMatchLowChOpState << 26u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroLowMatchHighNotMatchLowChOpState << 10u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroLowNotMatchHighMatchLowChOpState << 30u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroLowNotMatchHighNotMatchLowChOpState << 14u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntUpCntLowMatchHighMatchLowChOpState << 24u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntUpCntLowMatchHighNotMatchLowChOpState << 8u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntUpCntLowNotMatchHighMatchLowChOpState << 18u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakLowMatchHighMatchLowChOpState << 22u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakLowMatchHighNotMatchLowChOpState << 6u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakLowNotMatchHighMatchLowChOpState << 28u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakLowNotMatchHighNotMatchLowChOpState << 12u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntDownLowMatchHighMatchLowChOpState << 20u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntDownLowMatchHighNotMatchLowChOpState << 4u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntDownLowNotMatchHighMatchLowChOpState << 16u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntZeroMatchOcfState << 3u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntUpCntMatchOcfState << 2u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntPeakMatchOcfState << 1u);
        u32OCMR |= (uint32_t)((uint32_t)pstcMode->enCntDownCntMatchOcfState << 0u);

        *pu32OCMR = u32OCMR;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set output function
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] enCmd                   The output functional state
 ** \arg Enable                         Enable output function
 ** \arg Disable                        Disable output function
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_OutputCompareCmd(M4_TMR4_TypeDef *TMR4x,
                                        en_timer4_oco_ch_t enCh,
                                        en_functional_state_t enCmd)
{
    __IO stc_tmr4_ocsr_field_t *pstcOCSR = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_CH(enCh));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        /* Get pointer of current channel OCO register address */
        pstcOCSR = (__IO stc_tmr4_ocsr_field_t *)TMR4_OCSRx(TMR4x, enCh);

        /* set register */
        IS_VALID_OCO_HIGH_CH(enCh) ? (pstcOCSR->OCEH = (uint16_t)enCmd) : (pstcOCSR->OCEL = (uint16_t)enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set OCO interrupt function
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] enCmd                   The interrupt functional state
 ** \arg Enable                         Enable interrupt function
 ** \arg Disable                        Disable interrupt function
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_IrqCmd(M4_TMR4_TypeDef *TMR4x,
                              en_timer4_oco_ch_t enCh,
                              en_functional_state_t enCmd)
{
    __IO stc_tmr4_ocsr_field_t *pstcOCSR = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_CH(enCh));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        /* Get pointer of current channel OCO register address */
        pstcOCSR = (__IO stc_tmr4_ocsr_field_t *)TMR4_OCSRx(TMR4x, enCh);
        /* set register */
        IS_VALID_OCO_HIGH_CH(enCh) ? (pstcOCSR->OCIEH = (uint16_t)enCmd) : (pstcOCSR->OCIEL = (uint16_t)enCmd);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get OCO interrupt flag
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 **
 ** \retval Reset                       None interrupt request on Timer4 OCO
 ** \retval Set                         Detection interrupt request on Timer4 OCO
 **
 ******************************************************************************/
en_flag_status_t TIMER4_OCO_GetIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                       en_timer4_oco_ch_t enCh)
{
    en_flag_status_t enFlag = Reset;
    __IO stc_tmr4_ocsr_field_t *pstcOCSR = NULL;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_OCO_CH(enCh));
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (__IO stc_tmr4_ocsr_field_t *)TMR4_OCSRx(TMR4x, enCh);

    /* set return value */
    if (IS_VALID_OCO_HIGH_CH(enCh)) /* channel: Timer4OcoOuh, Timer4OcoOvh, Timer4OcoOwh */
    {
        enFlag = (en_flag_status_t)(pstcOCSR->OCFH);
    }
    else if (IS_VALID_OCO_LOW_CH(enCh)) /* channel: Timer4OcoOul, Timer4OcoOvl, Timer4OcoOwl */
    {
        enFlag = (en_flag_status_t)(pstcOCSR->OCFL);
    }
    else
    {
        /* Do nothing: only avoid MISRA warning */
    }

    return enFlag;
}

/**
 *******************************************************************************
 ** \brief Clear OCO interrupt flag
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 **
 ** \retval Ok                          OCO interrupt flag is clear
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_ClearIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                    en_timer4_oco_ch_t enCh)
{
    __IO stc_tmr4_ocsr_field_t *pstcOCSR = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_CH(enCh));

        /* Get pointer of current channel OCO register address */
        pstcOCSR = (__IO stc_tmr4_ocsr_field_t *)TMR4_OCSRx(TMR4x, enCh);
        /* set return value */
        IS_VALID_OCO_HIGH_CH(enCh) ? (pstcOCSR->OCFH = 0u) : (pstcOCSR->OCFL = 0u);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set OP pin level of OCO
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] enLevel                 OP port level of OCO
 ** \arg OcPortLevelLow                 Output low level to OC port
 ** \arg OcPortLevelHigh                Output high level to OC port
 **
 ** \retval Ok                          OCO interrupt flag is clear
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_SetOpPortLevel(M4_TMR4_TypeDef *TMR4x,
                                      en_timer4_oco_ch_t enCh,
                                      en_timer4_oco_port_level_t enLevel)
{
    __IO stc_tmr4_ocsr_field_t *pstcOCSR = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_CH(enCh));
        DDL_ASSERT(IS_VALID_OP_PORT_LEVEL(enLevel));

        /* Get pointer of current channel OCO register address */
        pstcOCSR = (__IO stc_tmr4_ocsr_field_t *)TMR4_OCSRx(TMR4x, enCh);
        IS_VALID_OCO_HIGH_CH(enCh) ? (pstcOCSR->OCFH = (uint16_t)enLevel) : (pstcOCSR->OCFL = (uint16_t)enLevel);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get OP pin level of OCO
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 **
 ** \retval OcPortLevelLow              Output low level to OC port
 ** \retval OcPortLevelHigh             Output high level to OC port
 **
 ******************************************************************************/
en_timer4_oco_port_level_t TIMER4_OCO_GetOpPinLevel(M4_TMR4_TypeDef *TMR4x,
                                                    en_timer4_oco_ch_t enCh)
{
    __IO stc_tmr4_ocsr_field_t *pstcOCSR = NULL;
    en_timer4_oco_port_level_t enLevel = OcPortLevelLow;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_OCO_CH(enCh));
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    /* Get pointer of current channel OCO register address */
    pstcOCSR = (__IO stc_tmr4_ocsr_field_t *)TMR4_OCSRx(TMR4x, enCh);

    if (IS_VALID_OCO_HIGH_CH(enCh)) /* channel: Timer4OcoOuh, Timer4OcoOvh, Timer4OcoOwh */
    {
        enLevel = (en_timer4_oco_port_level_t)(pstcOCSR->OCPH);
    }
    else if (IS_VALID_OCO_LOW_CH(enCh)) /* channel: Timer4OcoOul, Timer4OcoOvl, Timer4OcoOwl */
    {
        enLevel = (en_timer4_oco_port_level_t)(pstcOCSR->OCPL);
    }
    else
    {
        /* Do nothing: only avoid MISRA warning */
    }

    return enLevel;
}

/**
 *******************************************************************************
 ** \brief Write OCCR register
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 ** \param [in] u16Occr                 The value of occr
 ** \arg 16bit value
 **
 ** \retval Ok                          OCCR written
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_OCO_WriteOccr(M4_TMR4_TypeDef *TMR4x,
                                 en_timer4_oco_ch_t enCh,
                                 uint16_t u16Occr)
{
    __IO uint16_t *pu16OCCR = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_OCO_CH(enCh));

        /* Get pointer of current channel OCO register address */
        pu16OCCR = (__IO uint16_t *)TMR4_OCCRx(TMR4x, enCh);
        /* set register */
        *pu16OCCR = u16Occr;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get OCCR register value
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of OCO
 ** \arg Timer4OcoOuh                   Timer oco channel:OUH
 ** \arg Timer4OcoOul                   Timer oco channel:OUL
 ** \arg Timer4OcoOvh                   Timer oco channel:OVH
 ** \arg Timer4OcoOvl                   Timer oco channel:OVL
 ** \arg Timer4OcoOwh                   Timer oco channel:OWH
 ** \arg Timer4OcoOwl                   Timer oco channel:OWL
 **
 ** \retval OCCR register value
 **
 ******************************************************************************/
uint16_t TIMER4_OCO_ReadOccr(const M4_TMR4_TypeDef *TMR4x,
                             en_timer4_oco_ch_t enCh)
{
    __IO uint16_t *pu16OCCR = NULL;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));
    DDL_ASSERT(IS_VALID_OCO_CH(enCh));

    /* Get pointer of current channel OCO register address */
    pu16OCCR = (__IO uint16_t *)TMR4_OCCRx(TMR4x, enCh);

    return (*pu16OCCR);
}

//@} // Timer4OcoGroup

#endif /* DDL_TIMER4_OCO_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
