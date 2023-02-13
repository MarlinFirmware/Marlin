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
/** \file hc32f46x_timer4_pwm.c
 **
 ** A detailed description is available at
 ** @link Timer4PwmGroup Timer4PWM description @endlink
 **
 **   - 2018-11-02  1.0 Hongjh First version for Device Driver Library of
 **                     Timer4PWM.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_timer4_pwm.h"
#include "hc32f46x_utility.h"

#if (DDL_TIMER4_PWM_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup Timer4PwmGroup
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

/*!< Parameter validity check for PWM channel  */
#define IS_VALID_PWM_CH(x)  \
    ((Timer4PwmU == (x)) || \
     (Timer4PwmV == (x)) || \
     (Timer4PwmW == (x)))

/*!< Parameter validity check for PWM mode  */
#define IS_VALID_PWM_MODE(x)      \
    ((PwmThroughMode == (x)) ||   \
     (PwmDeadTimerMode == (x)) || \
     (PwmDeadTimerFilterMode == (x)))

/*!< Parameter valid check for PWM output state. */
#define IS_VALID_PWM_OUTPUT_STATE(x) \
    ((PwmHPwmLHold == (x)) ||        \
     (PwmHPwmLReverse == (x)) ||     \
     (PwmHReversePwmLHold == (x)) || \
     (PwmHHoldPwmLReverse == (x)))

/*!< Parameter valid check for PWM clock division. */
#define IS_VALID_PWM_CLK_DIV(x) \
    ((PwmPlckDiv1 == (x)) ||    \
     (PwmPlckDiv2 == (x)) ||    \
     (PwmPlckDiv4 == (x)) ||    \
     (PwmPlckDiv8 == (x)) ||    \
     (PwmPlckDiv16 == (x)) ||   \
     (PwmPlckDiv32 == (x)) ||   \
     (PwmPlckDiv64 == (x)) ||   \
     (PwmPlckDiv128 == (x)))

/*!< Get the specified register address of the specified Timer4 unit */
#define TMR4_RCSRx(__TMR4x__) ((uint32_t) & (__TMR4x__)->RCSR)
#define TMR4_POCRx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->POCRU + ((uint32_t)(__CH__)) * 4ul)
#define TMR4_PDARx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->PDARU + ((uint32_t)(__CH__)) * 8ul)
#define TMR4_PDBRx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->PDBRU + ((uint32_t)(__CH__)) * 8ul)
#define TMR4_PFSRx(__TMR4x__, __CH__) ((uint32_t) & (__TMR4x__)->PFSRU + ((uint32_t)(__CH__)) * 8ul)

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
 ** \brief Initialize a couple PWM channels
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of PWM
 ** \arg Timer4PwmOuhl                  Timer4 PWM couple channel OUH&OUL
 ** \arg Timer4PwmOvhl                  Timer4 PWM couple channel OVH&OVL
 ** \arg Timer4PwmOwhl                  Timer4 PWM couple channel OWH&OWL
 ** \param [in] pstcInitCfg             The pointer of PWM configure structure
 ** \arg This parameter detail refer @ref stc_timer4_pwm_init_t
 **
 ** \retval Ok                          Initialize successfully
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - pstcInitCfg == NULL
 **                                     - enCh is invalid
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t TIMER4_PWM_Init(M4_TMR4_TypeDef *TMR4x,
                            en_timer4_pwm_ch_t enCh,
                            const stc_timer4_pwm_init_t *pstcInitCfg)
{
    __IO stc_tmr4_pocr_field_t *pstcPOCR_f = NULL;
    __IO stc_tmr4_rcsr_field_t *pstcRCSR_f = NULL;
    en_result_t enRet = Ok;

    /* Check TMR4x && pstcInitCfg pointer */
    if ((IS_VALID_TIMER4(TMR4x)) && (NULL != pstcInitCfg))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_PWM_MODE(pstcInitCfg->enMode));
        DDL_ASSERT(IS_VALID_PWM_CLK_DIV(pstcInitCfg->enClkDiv));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enRtIntMaskCmd));
        DDL_ASSERT(IS_VALID_PWM_OUTPUT_STATE(pstcInitCfg->enOutputState));

        /* Get pointer of current channel PWM register address */
        pstcRCSR_f = (__IO stc_tmr4_rcsr_field_t *)TMR4_RCSRx(TMR4x);
        pstcPOCR_f = (__IO stc_tmr4_pocr_field_t *)TMR4_POCRx(TMR4x, enCh);

        /* Configure PWM mode */
        pstcPOCR_f->PWMMD = (uint16_t)(pstcInitCfg->enMode);

        /* Configure PWM mode */
        pstcPOCR_f->LVLS = (uint16_t)(pstcInitCfg->enOutputState);

        /* Set timer clock division */
        pstcPOCR_f->DIVCK = (uint16_t)(pstcInitCfg->enClkDiv);

        /* Set interrupt mask */
        switch (enCh)
        {
        case Timer4PwmU:
            pstcRCSR_f->RTIDU = (uint16_t)(pstcInitCfg->enRtIntMaskCmd);
            break;
        case Timer4PwmV:
            pstcRCSR_f->RTIDV = (uint16_t)(pstcInitCfg->enRtIntMaskCmd);
            break;
        case Timer4PwmW:
            pstcRCSR_f->RTIDW = (uint16_t)(pstcInitCfg->enRtIntMaskCmd);
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

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Initialize a couple PWM channels
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of PWM
 ** \arg Timer4PwmOuhl                  Timer4 PWM couple channel OUH&OUL
 ** \arg Timer4PwmOvhl                  Timer4 PWM couple channel OVH&OVL
 ** \arg Timer4PwmOwhl                  Timer4 PWM couple channel OWH&OWL
 **
 ** \retval Ok                          De-Initialize successfully.
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - enCh out of range
 **
 ******************************************************************************/
en_result_t TIMER4_PWM_DeInit(M4_TMR4_TypeDef *TMR4x,
                              en_timer4_pwm_ch_t enCh)
{
    en_result_t enRet = Ok;
    __IO uint16_t *pu16PDAR = NULL;
    __IO uint16_t *pu16PDBR = NULL;
    __IO uint16_t *pu16PFSR = NULL;
    __IO stc_tmr4_pocr_field_t *pstcPOCR_f = NULL;
    __IO stc_tmr4_rcsr_field_t *pstcRCSR_f = NULL;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Get pointer of current channel PWM register address */
        pu16PDAR = (__IO uint16_t *)TMR4_PDARx(TMR4x, enCh);
        pu16PDBR = (__IO uint16_t *)TMR4_PDBRx(TMR4x, enCh);
        pu16PFSR = (__IO uint16_t *)TMR4_PFSRx(TMR4x, enCh);
        pstcRCSR_f = (__IO stc_tmr4_rcsr_field_t *)TMR4_RCSRx(TMR4x);
        pstcPOCR_f = (__IO stc_tmr4_pocr_field_t *)TMR4_POCRx(TMR4x, enCh);

        *pu16PDAR = (uint16_t)0u;
        *pu16PDBR = (uint16_t)0u;
        *pu16PFSR = (uint16_t)0u;
        pstcPOCR_f->DIVCK = (uint16_t)0u;
        pstcPOCR_f->LVLS = (uint16_t)0u;
        pstcPOCR_f->PWMMD = (uint16_t)0u;

        switch (enCh)
        {
        case Timer4PwmU:
            pstcRCSR_f->RTIDU = (uint16_t)0u;
            break;
        case Timer4PwmV:
            pstcRCSR_f->RTIDV = (uint16_t)0u;
            break;
        case Timer4PwmW:
            pstcRCSR_f->RTIDW = (uint16_t)0u;
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

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Start PWM timer
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of PWM
 ** \arg Timer4PwmOuhl                  Timer4 PWM couple channel OUH&OUL
 ** \arg Timer4PwmOvhl                  Timer4 PWM couple channel OVH&OVL
 ** \arg Timer4PwmOwhl                  Timer4 PWM couple channel OWH&OWL
 **
 ** \retval Ok                          Start timer successfully
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - enCh out of range
 **
 ******************************************************************************/
en_result_t TIMER4_PWM_StartTimer(M4_TMR4_TypeDef *TMR4x,
                                  en_timer4_pwm_ch_t enCh)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmr4_rcsr_field_t *pstcRCSR_f = NULL;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        enRet = Ok;
        /* Get pointer of current channel PWM register address */
        pstcRCSR_f = (__IO stc_tmr4_rcsr_field_t *)TMR4_RCSRx(TMR4x);

        switch (enCh)
        {
        case Timer4PwmU:
            pstcRCSR_f->RTEU = (uint16_t)1u;
            break;
        case Timer4PwmV:
            pstcRCSR_f->RTEV = (uint16_t)1u;
            break;
        case Timer4PwmW:
            pstcRCSR_f->RTEW = (uint16_t)1u;
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
 ** \brief Stop PWM timer
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of PWM
 ** \arg Timer4PwmOuhl                  Timer4 PWM couple channel OUH&OUL
 ** \arg Timer4PwmOvhl                  Timer4 PWM couple channel OVH&OVL
 ** \arg Timer4PwmOwhl                  Timer4 PWM couple channel OWH&OWL
 **
 ** \retval Ok                          Stop timer successfully
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - enCh out of range
 **
 ******************************************************************************/
en_result_t TIMER4_PWM_StopTimer(M4_TMR4_TypeDef *TMR4x,
                                 en_timer4_pwm_ch_t enCh)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmr4_rcsr_field_t *pstcRCSR_f = NULL;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_PWM_CH(enCh));

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        enRet = Ok;

        /* Get pointer of current channel PWM register address */
        pstcRCSR_f = (__IO stc_tmr4_rcsr_field_t *)TMR4_RCSRx(TMR4x);
        switch (enCh)
        {
        case Timer4PwmU:
            pstcRCSR_f->RTSU = (uint16_t)1u;
            break;
        case Timer4PwmV:
            pstcRCSR_f->RTSV = (uint16_t)1u;
            break;
        case Timer4PwmW:
            pstcRCSR_f->RTSW = (uint16_t)1u;
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
 ** \brief Get PWM reload-timer interrupt flag
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of PWM
 ** \arg Timer4PwmOuhl                  Timer4 PWM couple channel OUH&OUL
 ** \arg Timer4PwmOvhl                  Timer4 PWM couple channel OVH&OVL
 ** \arg Timer4PwmOwhl                  Timer4 PWM couple channel OWH&OWL
 **
 ** \retval Reset                       None interrupt request on PWM reload-timer
 ** \retval Set                         Detection interrupt request on PWM reload-timer
 **
 ******************************************************************************/
en_flag_status_t TIMER4_PWM_GetIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                       en_timer4_pwm_ch_t enCh)
{
    uint16_t u16Flag = 0u;
    __IO stc_tmr4_rcsr_field_t *pstcRCSR_f = NULL;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_PWM_CH(enCh));
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    /* Get pointer of current channel PWM register address */
    pstcRCSR_f = (__IO stc_tmr4_rcsr_field_t *)TMR4_RCSRx(TMR4x);

    switch (enCh)
    {
    case Timer4PwmU:
        u16Flag = pstcRCSR_f->RTIFU;
        break;
    case Timer4PwmV:
        u16Flag = pstcRCSR_f->RTIFV;
        break;
    case Timer4PwmW:
        u16Flag = pstcRCSR_f->RTIFW;
        break;
    default:
        break;
    }

    return (en_flag_status_t)u16Flag;
}

/**
 *******************************************************************************
 ** \brief Clear PWM reload-timer interrupt flag
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of PWM
 ** \arg Timer4PwmOuhl                  Timer4 PWM couple channel OUH&OUL
 ** \arg Timer4PwmOvhl                  Timer4 PWM couple channel OVH&OVL
 ** \arg Timer4PwmOwhl                  Timer4 PWM couple channel OWH&OWL
 **
 ** \retval Ok                          PWM reload-timer interrupt flag is clear
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - TMR4x is invalid
 **                                     - enCh out of range
 **
 ******************************************************************************/
en_result_t TIMER4_PWM_ClearIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                    en_timer4_pwm_ch_t enCh)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmr4_rcsr_field_t *pstcRCSR_f = NULL;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_PWM_CH(enCh));

        enRet = Ok;
        /* Get pointer of current channel PWM register address */
        pstcRCSR_f = (__IO stc_tmr4_rcsr_field_t *)TMR4_RCSRx(TMR4x);
        switch (enCh)
        {
        case Timer4PwmU:
            pstcRCSR_f->RTICU = (uint16_t)1u;
            break;
        case Timer4PwmV:
            pstcRCSR_f->RTICV = (uint16_t)1u;
            break;
        case Timer4PwmW:
            pstcRCSR_f->RTICW = (uint16_t)1u;
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
 ** \brief Write timer count cycle
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of PWM
 ** \arg Timer4PwmOuhl                  Timer4 PWM couple channel OUH&OUL
 ** \arg Timer4PwmOvhl                  Timer4 PWM couple channel OVH&OVL
 ** \arg Timer4PwmOwhl                  Timer4 PWM couple channel OWH&OWL
 ** \param [in] u16PDAR                 PDAR value
 ** \arg  0~65535
 ** \param [in] u16PDBR                 PDBR value
 ** \arg  0~65535
 **
 ** \retval Ok                          Timer count cycle is written
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_PWM_WriteDeadRegionValue(M4_TMR4_TypeDef *TMR4x,
                                            en_timer4_pwm_ch_t enCh,
                                            uint16_t u16PDAR,
                                            uint16_t u16PDBR)
{
    __IO uint16_t *pu16PDAR = NULL;
    __IO uint16_t *pu16PDBR = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_PWM_CH(enCh));

        /* Get pointer of current channel PWM register address */
        pu16PDAR = (__IO uint16_t *)TMR4_PDARx(TMR4x, enCh);
        pu16PDBR = (__IO uint16_t *)TMR4_PDBRx(TMR4x, enCh);

        /* set the register */
        *pu16PDAR = u16PDAR;
        *pu16PDBR = u16PDBR;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Read dead region count value
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of PWM
 ** \arg Timer4PwmOuhl                  Timer4 PWM couple channel OUH&OUL
 ** \arg Timer4PwmOvhl                  Timer4 PWM couple channel OVH&OVL
 ** \arg Timer4PwmOwhl                  Timer4 PWM couple channel OWH&OWL
 ** \param [out] u16PDAR                Pointer of uint16_t type
 ** \arg  0~65535
 ** \param [out] u16PDBR                Pointer of uint16_t type
 ** \arg  0~65535
 **
 ** \retval Ok                          Read successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_PWM_ReadDeadRegionValue(M4_TMR4_TypeDef *TMR4x,
                                           en_timer4_pwm_ch_t enCh,
                                           uint16_t *u16PDAR,
                                           uint16_t *u16PDBR)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_PWM_CH(enCh));

        /* Get pointer of current channel PWM register address */
        *u16PDAR = *(__IO uint16_t *)TMR4_PDARx(TMR4x, enCh);
        *u16PDBR = *(__IO uint16_t *)TMR4_PDBRx(TMR4x, enCh);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set cycle of PWM timer
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCh                    Channel of PWM
 ** \arg Timer4PwmOuhl                  Timer4 PWM couple channel OUH&OUL
 ** \arg Timer4PwmOvhl                  Timer4 PWM couple channel OVH&OVL
 ** \arg Timer4PwmOwhl                  Timer4 PWM couple channel OWH&OWL
 ** \param [in]  u16Count               PWM pulse counter value
 ** \arg   0~65535
 **
 ** \retval Ok                          Cycle of PWM timer is set
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_PWM_SetFilterCountValue(M4_TMR4_TypeDef *TMR4x,
                                           en_timer4_pwm_ch_t enCh,
                                           uint16_t u16Count)
{
    __IO uint16_t *pu16PFSR = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_PWM_CH(enCh));

        /* Get pointer of current channel PWM register address */
        pu16PFSR = (__IO uint16_t *)TMR4_PFSRx(TMR4x, enCh);
        *pu16PFSR = u16Count;

        enRet = Ok;
    }

    return enRet;
}

//@} // Timer4PwmGroup

#endif /* DDL_TIMER4_PWM_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
