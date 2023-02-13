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
/** \file hc32f46x_emb.c
 **
 ** A detailed description is available at
 ** @link EMBGroup EMB description @endlink
 **
 **   - 2018-11-24  1.0  Husj  First version for Device Driver Library of EMB.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_emb.h"
#include "hc32f46x_utility.h"

#if (DDL_EMB_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup EMBGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*!< Parameter valid check for emb unit */
#define IS_VALID_EMB_UNIT(__EMBx__) \
    ((M4_EMB1 == (__EMBx__)) ||     \
     (M4_EMB2 == (__EMBx__)) ||     \
     (M4_EMB3 == (__EMBx__)) ||     \
     (M4_EMB4 == (__EMBx__)))

/*!< Parameter valid check for emb status*/
#define IS_VALID_EMB_STATUS_TYPE(x) \
    ((EMBFlagPortIn == (x)) ||      \
     (EMBFlagPWMSame == (x)) ||     \
     (EMBFlagCmp == (x)) ||         \
     (EMBFlagOSCFail == (x)) ||     \
     (EMBPortInState == (x)) ||     \
     (EMBPWMState == (x)))

/*!< Parameter valid check for emb status clear*/
#define IS_VALID_EMB_STATUS_CLR(x) \
    ((EMBPortInFlagClr == (x)) ||  \
     (EMBPWMSameFlagCLr == (x)) || \
     (EMBCmpFlagClr == (x)) ||     \
     (EMBOSCFailFlagCLr == (x)))

/*!< Parameter valid check for emb irq enable*/
#define IS_VALID_EMB_IRQ(x)  \
    ((PORTBrkIrq == (x)) ||  \
     (PWMSmBrkIrq == (x)) || \
     (CMPBrkIrq == (x)) ||   \
     (OSCFailBrkIrq == (x)))

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

/*******************************************************************************
 * \brief   EMB interrupt request enable or disable
 *
 * \param   [in]  EMBx             EMB unit
 * \param   [in]  enEMBIrq         Irq type
 * \param   [in]  bEn              true/false
 *
 * \retval  en_result_t  Ok:  config success
 ******************************************************************************/
en_result_t EMB_ConfigIrq(M4_EMB_TypeDef *EMBx,
                          en_emb_irq_type_t enEMBIrq,
                          bool bEn)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (IS_VALID_EMB_UNIT(EMBx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_EMB_IRQ(enEMBIrq));

        enRet = Ok;
        switch (enEMBIrq)
        {
        case PORTBrkIrq:
            EMBx->INTEN_f.PORTINTEN = (uint32_t)bEn;
            break;
        case PWMSmBrkIrq:
            EMBx->INTEN_f.PWMINTEN = (uint32_t)bEn;
            break;
        case CMPBrkIrq:
            EMBx->INTEN_f.CMPINTEN = (uint32_t)bEn;
            break;
        case OSCFailBrkIrq:
            EMBx->INTEN_f.OSINTEN = (uint32_t)bEn;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get EMB status
 **
 ** \param [in] EMBx          EMB unit
 **
 ** \param [in] enStatus      EMB status type
 **
 ** \retval EMB status
 **
 ******************************************************************************/
bool EMB_GetStatus(M4_EMB_TypeDef *EMBx, en_emb_status_t enStatus)
{
    bool status = false;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_EMB_UNIT(EMBx));
    DDL_ASSERT(IS_VALID_EMB_STATUS_TYPE(enStatus));

    switch (enStatus)
    {
    case EMBFlagPortIn:
        status = EMBx->STAT_f.PORTINF;
        break;
    case EMBFlagPWMSame:
        status = EMBx->STAT_f.PWMSF;
        break;
    case EMBFlagCmp:
        status = EMBx->STAT_f.CMPF;
        break;
    case EMBFlagOSCFail:
        status = EMBx->STAT_f.OSF;
        break;
    case EMBPortInState:
        status = EMBx->STAT_f.PORTINST;
        break;
    case EMBPWMState:
        status = EMBx->STAT_f.PWMST;
        break;
    default:
        break;
    }

    return status;
}

/**
 *******************************************************************************
 ** \brief  EMB clear status(Recover from protection state)
 **
 ** \param  [in] EMBx                EMB unit
 **
 ** \param  [in] enStatusClr         EMB status clear type
 **
 ** \retval en_result_t  Ok:         Config Success
 **
 ******************************************************************************/
en_result_t EMB_ClrStatus(M4_EMB_TypeDef *EMBx,
                          en_emb_status_clr_t enStatusClr)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (IS_VALID_EMB_UNIT(EMBx))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_EMB_STATUS_CLR(enStatusClr));

        enRet = Ok;
        switch (enStatusClr)
        {
        case EMBPortInFlagClr:
            EMBx->STATCLR_f.PORTINFCLR = 1ul;
            break;
        case EMBPWMSameFlagCLr:
            EMBx->STATCLR_f.PWMSFCLR = 1ul;
            break;
        case EMBCmpFlagClr:
            EMBx->STATCLR_f.CMPFCLR = 1ul;
            break;
        case EMBOSCFailFlagCLr:
            EMBx->STATCLR_f.OSFCLR = 1ul;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }
    }

    return enRet;
}

/*******************************************************************************
 * \brief   EMB Control Register(CR) for timer6
 *
 * \param  [in] EMBx               EMB unit
 * \param  [in] pstcEMBConfigCR    EMB Config CR pointer
 *
 * \retval  en_result_t    Ok:  Set successfully
 * \retval  en_result_t  ErrorInvalidParameter: Provided parameter is not valid
 ******************************************************************************/
en_result_t EMB_Config_CR_Timer6(const stc_emb_ctrl_timer6_t *pstcEMBConfigCR)
{
    uint32_t u32Val = 0ul;
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcEMBConfigCR)
    {
        if (pstcEMBConfigCR->bEnPortBrake)
        {
            u32Val |= 1ul;
        }
        if (pstcEMBConfigCR->bEnCmp1Brake)
        {
            u32Val |= 1ul << 1;
        }
        if (pstcEMBConfigCR->bEnCmp2Brake)
        {
            u32Val |= 1ul << 2;
        }
        if (pstcEMBConfigCR->bEnCmp3Brake)
        {
            u32Val |= 1ul << 3;
        }
        if (pstcEMBConfigCR->bEnOSCFailBrake)
        {
            u32Val |= 1ul << 5;
        }
        if (pstcEMBConfigCR->bEnTimer61PWMSBrake)
        {
            u32Val |= 1ul << 6;
        }
        if (pstcEMBConfigCR->bEnTimer62PWMSBrake)
        {
            u32Val |= 1ul << 7;
        }
        if (pstcEMBConfigCR->bEnTimer63PWMSBrake)
        {
            u32Val |= 1ul << 8;
        }
        if (EMBPortFltDiv0 == pstcEMBConfigCR->enPortInFltClkSel)
        {
        }
        if (EMBPortFltDiv8 == pstcEMBConfigCR->enPortInFltClkSel)
        {
            u32Val |= 1ul << 28;
        }
        if (EMBPortFltDiv32 == pstcEMBConfigCR->enPortInFltClkSel)
        {
            u32Val |= 2ul << 28;
        }
        if (EMBPortFltDiv128 == pstcEMBConfigCR->enPortInFltClkSel)
        {
            u32Val |= 3ul << 28;
        }
        if (pstcEMBConfigCR->bEnPorInFlt)
        {
            u32Val |= 1ul << 30;
        }
        if (pstcEMBConfigCR->bEnPortInLevelSel_Low)
        {
            u32Val |= 1ul << 31;
        }

        M4_EMB1->CTL = u32Val;
        enRet = Ok;
    }

    return enRet;
}

/*******************************************************************************
 * \brief   EMB Control Register(CR) for timer4
 *
 * \param  [in] EMBx               EMB unit
 * \param  [in] pstcEMBConfigCR    EMB Config CR pointer
 *
 * \retval  en_result_t    Ok:  Set successfully
 * \retval  en_result_t  ErrorInvalidParameter: Provided parameter is not valid
 ******************************************************************************/
en_result_t EMB_Config_CR_Timer4(M4_EMB_TypeDef *EMBx,
                                 const stc_emb_ctrl_timer4_t *pstcEMBConfigCR)
{
    uint32_t u32Val = 0ul;
    en_result_t enRet = ErrorInvalidParameter;

    if ((M4_EMB1 != EMBx) &&
        (IS_VALID_EMB_UNIT(EMBx)) &&
        (NULL != pstcEMBConfigCR))
    {
        if (pstcEMBConfigCR->bEnPortBrake)
        {
            u32Val |= 1ul;
        }
        if (pstcEMBConfigCR->bEnCmp1Brake)
        {
            u32Val |= 1ul << 1;
        }
        if (pstcEMBConfigCR->bEnCmp2Brake)
        {
            u32Val |= 1ul << 2;
        }
        if (pstcEMBConfigCR->bEnCmp3Brake)
        {
            u32Val |= 1ul << 3;
        }
        if (pstcEMBConfigCR->bEnOSCFailBrake)
        {
            u32Val |= 1ul << 5;
        }
        if (pstcEMBConfigCR->bEnTimer4xWHLSammeBrake)
        {
            u32Val |= 1ul << 6;
        }
        if (pstcEMBConfigCR->bEnTimer4xVHLSammeBrake)
        {
            u32Val |= 1ul << 7;
        }
        if (pstcEMBConfigCR->bEnTimer4xUHLSammeBrake)
        {
            u32Val |= 1ul << 8;
        }
        if (EMBPortFltDiv0 == pstcEMBConfigCR->enPortInFltClkSel)
        {
        }
        if (EMBPortFltDiv8 == pstcEMBConfigCR->enPortInFltClkSel)
        {
            u32Val |= 1ul << 28;
        }
        if (EMBPortFltDiv32 == pstcEMBConfigCR->enPortInFltClkSel)
        {
            u32Val |= 2ul << 28;
        }
        if (EMBPortFltDiv128 == pstcEMBConfigCR->enPortInFltClkSel)
        {
            u32Val |= 3ul << 28;
        }
        if (pstcEMBConfigCR->bEnPorInFlt)
        {
            u32Val |= 1ul << 30;
        }
        if (pstcEMBConfigCR->bEnPortInLevelSel_Low)
        {
            u32Val |= 1ul << 31;
        }

        EMBx->CTL = u32Val;
        enRet = Ok;
    }

    return enRet;
}

/*******************************************************************************
 * \brief   EMB detect PWM atcive level (short detection) selection for timer6
 *
 * \param  [in] EMBx            EMB unit
 * \param  [in] pstcEMBPWMlv    EMB en detect active level pointer
 *
 * \retval  en_result_t    Ok:  Set successfully
 * \retval  en_result_t  ErrorInvalidParameter: Provided parameter is not valid
 ******************************************************************************/
en_result_t EMB_PWMLv_Timer6(const stc_emb_pwm_level_timer6_t *pstcEMBPWMlv)
{
    uint32_t u32Val = 0ul;
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != pstcEMBPWMlv)
    {
        if (pstcEMBPWMlv->bEnTimer61HighLevelDect)
        {
            u32Val |= 0x1ul;
        }
        if (pstcEMBPWMlv->bEnTimer62HighLevelDect)
        {
            u32Val |= 0x2ul;
        }
        if (pstcEMBPWMlv->bEnTimer63HighLevelDect)
        {
            u32Val |= 0x4ul;
        }

        M4_EMB1->PWMLV = u32Val;
        enRet = Ok;
    }

    return enRet;
}

/*******************************************************************************
 * \brief   EMB detect PWM atcive level (short detection) selection for timer4
 *
 * \param  [in] EMBx            EMB unit
 * \param  [in] pstcEMBPWMlv    EMB en detect active level pointer
 *
 * \retval  en_result_t    Ok:  Set successfully
 * \retval  en_result_t  ErrorInvalidParameter: Provided parameter is not valid
 ******************************************************************************/
en_result_t EMB_PWMLv_Timer4(M4_EMB_TypeDef *EMBx,
                             const stc_emb_pwm_level_timer4_t *pstcEMBPWMlv)
{
    uint32_t u32Val = 0ul;
    en_result_t enRet = ErrorInvalidParameter;

    if ((IS_VALID_EMB_UNIT(EMBx)) &&
        (M4_EMB1 != EMBx) &&
        (NULL != pstcEMBPWMlv))
    {
        if (pstcEMBPWMlv->bEnWHLphaseHighLevelDect)
        {
            u32Val |= 0x1ul;
        }
        if (pstcEMBPWMlv->bEnVHLPhaseHighLevelDect)
        {
            u32Val |= 0x2ul;
        }
        if (pstcEMBPWMlv->bEnUHLPhaseHighLevelDect)
        {
            u32Val |= 0x4ul;
        }

        EMBx->PWMLV = u32Val;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  EMB Software brake
 **
 ** \param  [in] EMBx        EMB unit
 **
 ** \param  [in] bEn          true: Software Brake Enable / false: Software Brake Disable
 **
 ** \retval en_result_t  Ok:         Config Success
 **
 ******************************************************************************/
en_result_t EMB_SwBrake(M4_EMB_TypeDef *EMBx, bool bEn)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_EMB_UNIT(EMBx));

    EMBx->SOE_f.SOE = (uint32_t)bEn;

    return Ok;
}

//@} // EMBGroup

#endif /* DDL_EMB_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
