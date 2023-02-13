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
/** \file hc32f46x_timer4_cnt.c
 **
 ** A detailed description is available at
 ** @link Timer4CntGroup Timer4CNT description @endlink
 **
 **   - 2018-11-02  1.0 Hongjh First version for Device Driver Library of
 **                     Timer4CNT.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_timer4_cnt.h"
#include "hc32f46x_utility.h"

#if (DDL_TIMER4_CNT_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup Timer4CntGroup
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

/*!< Parameter validity check for CNT pclk division  */
#define IS_VALID_CNT_CLK_DIV(x)      \
    ((Timer4CntPclkDiv1 == (x)) ||   \
     (Timer4CntPclkDiv2 == (x)) ||   \
     (Timer4CntPclkDiv4 == (x)) ||   \
     (Timer4CntPclkDiv8 == (x)) ||   \
     (Timer4CntPclkDiv16 == (x)) ||  \
     (Timer4CntPclkDiv32 == (x)) ||  \
     (Timer4CntPclkDiv64 == (x)) ||  \
     (Timer4CntPclkDiv128 == (x)) || \
     (Timer4CntPclkDiv256 == (x)) || \
     (Timer4CntPclkDiv512 == (x)) || \
     (Timer4CntPclkDiv1024 == (x)))

/*!< Parameter validity check for CNT mode  */
#define IS_VALID_CNT_MODE(x)           \
    ((Timer4CntSawtoothWave == (x)) || \
     (Timer4CntTriangularWave == (x)))

/*!< Parameter validity check for CNT interrupt mask  */
#define IS_VALID_CNT_INT_MSK(x)     \
    ((Timer4CntIntMask0 == (x)) ||  \
     (Timer4CntIntMask1 == (x)) ||  \
     (Timer4CntIntMask2 == (x)) ||  \
     (Timer4CntIntMask3 == (x)) ||  \
     (Timer4CntIntMask4 == (x)) ||  \
     (Timer4CntIntMask5 == (x)) ||  \
     (Timer4CntIntMask6 == (x)) ||  \
     (Timer4CntIntMask7 == (x)) ||  \
     (Timer4CntIntMask8 == (x)) ||  \
     (Timer4CntIntMask9 == (x)) ||  \
     (Timer4CntIntMask10 == (x)) || \
     (Timer4CntIntMask11 == (x)) || \
     (Timer4CntIntMask12 == (x)) || \
     (Timer4CntIntMask13 == (x)) || \
     (Timer4CntIntMask14 == (x)) || \
     (Timer4CntIntMask15 == (x)))

/*!< Parameter validity check for CNT match interrupt type  */
#define IS_VALID_CNT_INT_TYPE(x)       \
    ((Timer4CntZeroMatchInt == (x)) || \
     (Timer4CntPeakMatchInt == (x)))

/*!< Parameter validity check for CNT clock source  */
#define IS_VALID_CNT_CLK(x)    \
    ((Timer4CntPclk == (x)) || \
     (Timer4CntExtclk == (x)))

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
 ** \brief Initialize Timer4 CNT
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] pstcInitCfg             Pointer to CNT initialization configuration structure
 ** \arg This parameter detail refer @ref stc_timer4_cnt_init_t
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TMR4x is invalid
 **                                     - pstcInitCfg == NULL
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_Init(M4_TMR4_TypeDef *TMR4x,
                            const stc_timer4_cnt_init_t *pstcInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_tmr4_ccsr_field_t CCSR_f = {0};
    stc_tmr4_cvpr_field_t CVPR_f = {0};

    /* Check for TMR4x && pstcInitCfg pointer */
    if ((IS_VALID_TIMER4(TMR4x)) && (NULL != pstcInitCfg))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_CNT_CLK(pstcInitCfg->enClk));
        DDL_ASSERT(IS_VALID_CNT_MODE(pstcInitCfg->enCntMode));
        DDL_ASSERT(IS_VALID_CNT_CLK_DIV(pstcInitCfg->enClkDiv));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enBufferCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enZeroIntCmd));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enPeakIntCmd));
        DDL_ASSERT(IS_VALID_CNT_INT_MSK(pstcInitCfg->enZeroIntMsk));
        DDL_ASSERT(IS_VALID_CNT_INT_MSK(pstcInitCfg->enPeakIntMsk));

        /* Set default value */
        TMR4x->CCSR = (uint16_t)0x0050u;
        TMR4x->CNTR = (uint16_t)0x0000u;
        TMR4x->CPSR = (uint16_t)0xFFFFu;
        TMR4x->CVPR = (uint16_t)0x0000u;

        /* stop count of CNT */
        CCSR_f.STOP = 1u;

        /* set count clock div of CNT */
        CCSR_f.CKDIV = pstcInitCfg->enClkDiv;

        /* set cnt mode */
        CCSR_f.MODE = pstcInitCfg->enCntMode;

        /* set buffer enable bit */
        CCSR_f.BUFEN = (uint16_t)(pstcInitCfg->enBufferCmd);

        /* set external clock enable bit */
        CCSR_f.ECKEN = (Timer4CntExtclk == pstcInitCfg->enClk) ? ((uint16_t)1u) : ((uint16_t)0u);

        /* Set interrupt enable */
        CCSR_f.IRQZEN = (uint16_t)(pstcInitCfg->enZeroIntCmd);
        CCSR_f.IRQPEN = (uint16_t)(pstcInitCfg->enPeakIntCmd);

        /* set intterrupt mask times */
        CVPR_f.ZIM = (uint16_t)(pstcInitCfg->enZeroIntMsk);
        CVPR_f.PIM = (uint16_t)(pstcInitCfg->enPeakIntMsk);

        /* Set Timer4 register */
        TMR4x->CVPR_f = CVPR_f;
        TMR4x->CCSR_f = CCSR_f;
        TMR4x->CPSR = pstcInitCfg->u16Cycle;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-initialize Timer4 CNT
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval Ok                          De-Initialize successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_DeInit(M4_TMR4_TypeDef *TMR4x)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Set default value */
        TMR4x->CCSR = (uint16_t)0x0050u;
        TMR4x->CNTR = (uint16_t)0x0000u;
        TMR4x->CPSR = (uint16_t)0xFFFFu;
        TMR4x->CVPR = (uint16_t)0x0000u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set Timer4 CNT clock source
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enCntClk                Timer4 CNT clock source
 ** \arg Timer4CntPclk                  Uses the internal clock (PCLK) as CNT's count clock.
 ** \arg Timer4CntExtclk                Uses an external input clock (EXCK) as CNT's count clock.
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_SetClock(M4_TMR4_TypeDef *TMR4x,
                                en_timer4_cnt_clk_t enCntClk)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_CNT_CLK(enCntClk));
        /* set external clock enable bit */
        TMR4x->CCSR_f.ECKEN = (uint16_t)(enCntClk);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timer4 CNT clock source
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval Timer4CntPclk               Uses the internal clock (PCLK) as CNT's count clock.
 ** \retval Timer4CntExtclk             Uses an external input clock (EXCK) as CNT's count clock.
 **
 ******************************************************************************/
en_timer4_cnt_clk_t TIMER4_CNT_GetClock(M4_TMR4_TypeDef *TMR4x)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    return (en_timer4_cnt_clk_t)(TMR4x->CCSR_f.ECKEN);
}

/**
 *******************************************************************************
 ** \brief  Set Timer4 CNT clock division
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enClkDiv                Timer4 CNT clock division
 ** \arg Timer4CntPclkDiv1              Timer4 CNT clock: PCLK
 ** \arg Timer4CntPclkDiv2              Timer4 CNT clock: PCLK/2
 ** \arg Timer4CntPclkDiv4              Timer4 CNT clock: PCLK/4
 ** \arg Timer4CntPclkDiv8              Timer4 CNT clock: PCLK/8
 ** \arg Timer4CntPclkDiv16             Timer4 CNT clock: PCLK/16
 ** \arg Timer4CntPclkDiv32             Timer4 CNT clock: PCLK/32
 ** \arg Timer4CntPclkDiv64             Timer4 CNT clock: PCLK/64
 ** \arg Timer4CntPclkDiv128            Timer4 CNT clock: PCLK/128
 ** \arg Timer4CntPclkDiv256            Timer4 CNT clock: PCLK/256
 ** \arg Timer4CntPclkDiv512            Timer4 CNT clock: PCLK/512
 ** \arg Timer4CntPclkDiv1024           Timer4 CNT clock: PCLK/1024
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_SetClockDiv(M4_TMR4_TypeDef *TMR4x,
                                   en_timer4_cnt_clk_div_t enClkDiv)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_CNT_CLK_DIV(enClkDiv));
        TMR4x->CCSR_f.CKDIV = (uint16_t)enClkDiv;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Get Timer4 CNT clock division
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval Timer4CntPclkDiv1           Timer4 CNT clock: PCLK
 ** \retval Timer4CntPclkDiv2           Timer4 CNT clock: PCLK/2
 ** \retval Timer4CntPclkDiv4           Timer4 CNT clock: PCLK/4
 ** \retval Timer4CntPclkDiv8           Timer4 CNT clock: PCLK/8
 ** \retval Timer4CntPclkDiv16          Timer4 CNT clock: PCLK/16
 ** \retval Timer4CntPclkDiv32          Timer4 CNT clock: PCLK/32
 ** \retval Timer4CntPclkDiv64          Timer4 CNT clock: PCLK/64
 ** \retval Timer4CntPclkDiv128         Timer4 CNT clock: PCLK/128
 ** \retval Timer4CntPclkDiv256         Timer4 CNT clock: PCLK/256
 ** \retval Timer4CntPclkDiv512         Timer4 CNT clock: PCLK/512
 ** \retval Timer4CntPclkDiv1024        Timer4 CNT clock: PCLK/1024
 **
 ******************************************************************************/
en_timer4_cnt_clk_div_t TIMER4_CNT_GetClockDiv(M4_TMR4_TypeDef *TMR4x)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    return (en_timer4_cnt_clk_div_t)(TMR4x->CCSR_f.CKDIV);
}

/**
 *******************************************************************************
 ** \brief Set Timer4 CNT mode
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enMode                  Timer4 CNT mode
 ** \arg Timer4CntSawtoothWave          Timer4 count mode:sawtooth wave
 ** \arg Timer4CntTriangularWave        Timer4 count mode:triangular wave
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_SetMode(M4_TMR4_TypeDef *TMR4x,
                               en_timer4_cnt_mode_t enMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_CNT_MODE(enMode));
        TMR4x->CCSR_f.MODE = (uint16_t)enMode;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timer4 CNT mode
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval Timer4CntSawtoothWave       Timer4 count mode:sawtooth wave
 ** \retval Timer4CntTriangularWave     Timer4 count mode:triangular wave
 **
 ******************************************************************************/
en_timer4_cnt_mode_t TIMER4_CNT_GetMode(M4_TMR4_TypeDef *TMR4x)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    return (en_timer4_cnt_mode_t)(TMR4x->CCSR_f.MODE);
}

/**
 *******************************************************************************
 ** \brief Start Timer4 CNT
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval Ok                          Start successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_Start(M4_TMR4_TypeDef *TMR4x)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        TMR4x->CCSR_f.STOP = (uint16_t)0u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Stop Timer4 CNT
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval Ok                          Stop successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_Stop(M4_TMR4_TypeDef *TMR4x)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        TMR4x->CCSR_f.STOP = (uint16_t)1u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set Timer4 CNT interrupt
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enIntType               The specified type of Timer4 CNT interrupt
 ** \arg Timer4CntZeroMatchIrq          Zero match interrupt of Timer4 CNT
 ** \arg Timer4CntPeakMatchIrq          Peak match interrupt of Timer4 CNT
 ** \param [in] enCmd                   DCU interrupt functional state
 ** \arg Enable                         Enable the specified Timer4 CNT interrupt function
 ** \arg Disable                        Disable the specified Timer4 CNT interrupt function
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TMR4x is invalid
 **                                     - enIntType is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_IrqCmd(M4_TMR4_TypeDef *TMR4x,
                              en_timer4_cnt_int_t enIntType,
                              en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));
        DDL_ASSERT(IS_VALID_CNT_INT_TYPE(enIntType));

        enRet = Ok;
        switch (enIntType)
        {
        case Timer4CntZeroMatchInt:
            TMR4x->CCSR_f.IRQZEN = (uint16_t)enCmd;
            break;
        case Timer4CntPeakMatchInt:
            TMR4x->CCSR_f.IRQPEN = (uint16_t)enCmd;
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
 ** \brief  Get Timer4 CNT interrupt flag
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enIntType               Timer4 CNT interrupt type
 ** \arg Timer4CntZeroMatchIrq          Zero match interrupt of Timer4 CNT
 ** \arg Timer4CntPeakMatchIrq          Peak match interrupt of Timer4 CNT
 **
 ** \retval Reset                       None interrupt request on Timer4 CNT
 ** \retval Set                         Detection interrupt request on Timer4 CNT
 **
 ******************************************************************************/
en_flag_status_t TIMER4_CNT_GetIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                       en_timer4_cnt_int_t enIntType)
{
    uint16_t u16Flag = 0u;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));
    DDL_ASSERT(IS_VALID_CNT_INT_TYPE(enIntType));

    switch (enIntType)
    {
    case Timer4CntZeroMatchInt:
        u16Flag = TMR4x->CCSR_f.IRQZF;
        break;
    case Timer4CntPeakMatchInt:
        u16Flag = TMR4x->CCSR_f.IRQPF;
        break;
    default:
        break;
    }

    return (en_flag_status_t)u16Flag;
}

/**
 *******************************************************************************
 ** \brief  Clear Timer4 CNT interrupt flag
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enIntType               Timer4 CNT interrupt type
 ** \arg Timer4CntZeroMatchIrq          Zero match interrupt of Timer4 CNT
 ** \arg Timer4CntPeakMatchIrq          Peak match interrupt of Timer4 CNT
 **
 ** \retval Ok                          Clear successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TMR4x is invalid
 **                                     - enIntType is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_ClearIrqFlag(M4_TMR4_TypeDef *TMR4x,
                                    en_timer4_cnt_int_t enIntType)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_CNT_INT_TYPE(enIntType));

        enRet = Ok;
        switch (enIntType)
        {
        case Timer4CntZeroMatchInt:
            TMR4x->CCSR_f.IRQZF = (uint16_t)0u;
            break;
        case Timer4CntPeakMatchInt:
            TMR4x->CCSR_f.IRQPF = (uint16_t)0u;
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
 ** \brief Set the cycle value of the specified Timer4 CNT.
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] u16Cycle                The Timer4 CNT cycle value
 ** \arg number of 16bit
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_SetCycleVal(M4_TMR4_TypeDef *TMR4x, uint16_t u16Cycle)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        TMR4x->CPSR = u16Cycle;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Get the cycle value of the specified Timer4 CNT.
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval The cycle value of the specified Timer4 CNT.
 **
 ******************************************************************************/
uint16_t TIMER4_CNT_GetCycleVal(const M4_TMR4_TypeDef *TMR4x)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    return TMR4x->CPSR;
}

/**
 *******************************************************************************
 ** \brief  Clear Timer4 CNT register CNTR
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval Ok                          Clear successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_ClearCountVal(M4_TMR4_TypeDef *TMR4x)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        TMR4x->CCSR_f.CLEAR = (uint16_t)1u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the current count value of the specified Timer4 CNT.
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] u16Count                The Timer4 CNT current count value
 ** \arg number of 16bit
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_SetCountVal(M4_TMR4_TypeDef *TMR4x, uint16_t u16Count)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        TMR4x->CNTR = u16Count;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timer4 CNT current count value
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 **
 ** \retval The current count value of the specified Timer4 CNT.
 **
 ******************************************************************************/
uint16_t TIMER4_CNT_GetCountVal(const M4_TMR4_TypeDef *TMR4x)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));

    return TMR4x->CNTR;
}

/**
 *******************************************************************************
 ** \brief Set Timer4 CNT interrupt mask times
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enIntType               Timer4 CNT interrupt type
 ** \arg Timer4CntZeroMatchIrq          Zero match interrupt of Timer4 CNT
 ** \arg Timer4CntPeakMatchIrq          Peak match interrupt of Timer4 CNT
 ** \param [in] enMaskTimes             Timer4 CNT interrupt mask times
 ** \arg Timer4CntIntMask0              CNT interrupt flag is always set(not masked) for every CNT count at "0x0000" or peak.
 ** \arg Timer4CntIntMask1              CNT interrupt flag is set once for 2 every CNT counts at "0x0000" or peak (skiping 1 count).
 ** \arg Timer4CntIntMask2              CNT interrupt flag is set once for 3 every CNT counts at "0x0000" or peak (skiping 2 count).
 ** \arg Timer4CntIntMask3              CNT interrupt flag is set once for 4 every CNT counts at "0x0000" or peak (skiping 3 count).
 ** \arg Timer4CntIntMask4              CNT interrupt flag is set once for 5 every CNT counts at "0x0000" or peak (skiping 4 count).
 ** \arg Timer4CntIntMask5              CNT interrupt flag is set once for 6 every CNT counts at "0x0000" or peak (skiping 5 count).
 ** \arg Timer4CntIntMask6              CNT interrupt flag is set once for 7 every CNT counts at "0x0000" or peak (skiping 6 count).
 ** \arg Timer4CntIntMask7              CNT interrupt flag is set once for 8 every CNT counts at "0x0000" or peak (skiping 7 count).
 ** \arg Timer4CntIntMask8              CNT interrupt flag is set once for 9 every CNT counts at "0x0000" or peak (skiping 8 count).
 ** \arg Timer4CntIntMask9              CNT interrupt flag is set once for 10 every CNT counts at "0x0000" or peak (skiping 9 count).
 ** \arg Timer4CntIntMask10             CNT interrupt flag is set once for 11 every CNT counts at "0x0000" or peak (skiping 10 count).
 ** \arg Timer4CntIntMask11             CNT interrupt flag is set once for 12 every CNT counts at "0x0000" or peak (skiping 11 count).
 ** \arg Timer4CntIntMask12             CNT interrupt flag is set once for 13 every CNT counts at "0x0000" or peak (skiping 12 count).
 ** \arg Timer4CntIntMask13             CNT interrupt flag is set once for 14 every CNT counts at "0x0000" or peak (skiping 13 count).
 ** \arg Timer4CntIntMask14             CNT interrupt flag is set once for 15 every CNT counts at "0x0000" or peak (skiping 14 count).
 ** \arg Timer4CntIntMask15             CNT interrupt flag is set once for 16 every CNT counts at "0x0000" or peak (skiping 15 count).
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       TMR4x is invalid
 **
 ******************************************************************************/
en_result_t TIMER4_CNT_SetIntMaskTimes(M4_TMR4_TypeDef *TMR4x,
                                       en_timer4_cnt_int_t enIntType,
                                       en_timer4_cnt_int_mask_t enMaskTimes)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check TMR4x pointer */
    if (IS_VALID_TIMER4(TMR4x))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_CNT_INT_TYPE(enIntType));
        DDL_ASSERT(IS_VALID_CNT_INT_MSK(enMaskTimes));

        enRet = Ok;
        switch (enIntType)
        {
        case Timer4CntZeroMatchInt:
            TMR4x->CVPR_f.ZIM = (uint16_t)enMaskTimes;
            break;
        case Timer4CntPeakMatchInt:
            TMR4x->CVPR_f.PIM = (uint16_t)enMaskTimes;
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
 ** \brief Get Timer4 CNT interrupt mask times
 **
 ** \param [in] TMR4x                   Pointer to Timer4 instance register base
 ** \arg M4_TMR41                       Timer4 unit 1 instance register base
 ** \arg M4_TMR42                       Timer4 unit 2 instance register base
 ** \arg M4_TMR43                       Timer4 unit 3 instance register base
 ** \param [in] enIntType               Timer4 CNT interrupt type
 ** \arg Timer4CntZeroMatchIrq          Zero match interrupt of Timer4 CNT
 ** \arg Timer4CntPeakMatchIrq          Peak match interrupt of Timer4 CNT
 **
 ** \retval Timer4CntIntMask0           CNT interrupt flag is always set(not masked) for every CNT count at "0x0000" or peak.
 ** \retval Timer4CntIntMask1           CNT interrupt flag is set once for 2 every CNT counts at "0x0000" or peak (skiping 1 count).
 ** \retval Timer4CntIntMask2           CNT interrupt flag is set once for 3 every CNT counts at "0x0000" or peak (skiping 2 count).
 ** \retval Timer4CntIntMask3           CNT interrupt flag is set once for 4 every CNT counts at "0x0000" or peak (skiping 3 count).
 ** \retval Timer4CntIntMask4           CNT interrupt flag is set once for 5 every CNT counts at "0x0000" or peak (skiping 4 count).
 ** \retval Timer4CntIntMask5           CNT interrupt flag is set once for 6 every CNT counts at "0x0000" or peak (skiping 5 count).
 ** \retval Timer4CntIntMask6           CNT interrupt flag is set once for 7 every CNT counts at "0x0000" or peak (skiping 6 count).
 ** \retval Timer4CntIntMask7           CNT interrupt flag is set once for 8 every CNT counts at "0x0000" or peak (skiping 7 count).
 ** \retval Timer4CntIntMask8           CNT interrupt flag is set once for 9 every CNT counts at "0x0000" or peak (skiping 8 count).
 ** \retval Timer4CntIntMask9           CNT interrupt flag is set once for 10 every CNT counts at "0x0000" or peak (skiping 9 count).
 ** \retval Timer4CntIntMask10          CNT interrupt flag is set once for 11 every CNT counts at "0x0000" or peak (skiping 10 count).
 ** \retval Timer4CntIntMask11          CNT interrupt flag is set once for 12 every CNT counts at "0x0000" or peak (skiping 11 count).
 ** \retval Timer4CntIntMask12          CNT interrupt flag is set once for 13 every CNT counts at "0x0000" or peak (skiping 12 count).
 ** \retval Timer4CntIntMask13          CNT interrupt flag is set once for 14 every CNT counts at "0x0000" or peak (skiping 13 count).
 ** \retval Timer4CntIntMask14          CNT interrupt flag is set once for 15 every CNT counts at "0x0000" or peak (skiping 14 count).
 ** \retval Timer4CntIntMask15          CNT interrupt flag is set once for 16 every CNT counts at "0x0000" or peak (skiping 15 count).
 **
 ******************************************************************************/
en_timer4_cnt_int_mask_t TIMER4_CNT_GetIntMaskTimes(M4_TMR4_TypeDef *TMR4x,
                                                    en_timer4_cnt_int_t enIntType)
{
    uint16_t u16MaskTimes = 0u;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_TIMER4(TMR4x));
    DDL_ASSERT(IS_VALID_CNT_INT_TYPE(enIntType));

    switch (enIntType)
    {
    case Timer4CntZeroMatchInt:
        u16MaskTimes = TMR4x->CVPR_f.ZIM;
        break;
    case Timer4CntPeakMatchInt:
        u16MaskTimes = TMR4x->CVPR_f.PIM;
        break;
    default:
        break;
    }

    return (en_timer4_cnt_int_mask_t)u16MaskTimes;
}

//@} // Timer4CntGroup

#endif /* DDL_TIMER4_CNT_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
