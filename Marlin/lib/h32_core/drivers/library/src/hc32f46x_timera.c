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
/** \file hc32f46x_timera.c
 **
 ** A detailed description is available at
 ** @link TimeraGroup Timer A description @endlink
 **
 **   - 2018-11-08  1.0  Yangjp  First version for Device Driver Library of
 **                      Timera.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_timera.h"
#include "hc32f46x_utility.h"

#if (DDL_TIMERA_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup TimeraGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*!< Parameter valid check for normal timera unit */
#define IS_VALID_NORMAL_TIMERA_UNIT(x) \
    ((M4_TMRA1 == (x)) ||              \
     (M4_TMRA2 == (x)) ||              \
     (M4_TMRA3 == (x)) ||              \
     (M4_TMRA4 == (x)) ||              \
     (M4_TMRA5 == (x)) ||              \
     (M4_TMRA6 == (x)))

/*!< Parameter valid check for sync startup timera unit */
#define IS_VALID_SYNC_STARTUP_TIMERA_UNIT(x) \
    ((M4_TMRA2 == (x)) ||                    \
     (M4_TMRA3 == (x)) ||                    \
     (M4_TMRA4 == (x)) ||                    \
     (M4_TMRA5 == (x)) ||                    \
     (M4_TMRA6 == (x)))

/*!< Parameter valid check for Count clock division */
#define IS_VALID_COUNT_CLK_DIV(x) \
    ((TimeraPclkDiv1 == (x)) ||   \
     (TimeraPclkDiv2 == (x)) ||   \
     (TimeraPclkDiv4 == (x)) ||   \
     (TimeraPclkDiv8 == (x)) ||   \
     (TimeraPclkDiv16 == (x)) ||  \
     (TimeraPclkDiv32 == (x)) ||  \
     (TimeraPclkDiv64 == (x)) ||  \
     (TimeraPclkDiv128 == (x)) || \
     (TimeraPclkDiv256 == (x)) || \
     (TimeraPclkDiv512 == (x)) || \
     (TimeraPclkDiv1024 == (x)))

/*!< Parameter valid check for count mode */
#define IS_VALID_COUNT_MODE(x)               \
    ((TimeraCountModeSawtoothWave == (x)) || \
     (TimeraCountModeTriangularWave == (x)))

/*!< Parameter valid check for count direction */
#define IS_VALID_COUNT_DIR(x)     \
    ((TimeraCountDirUp == (x)) || \
     (TimeraCountDirDown == (x)))

/*!< Parameter valid check for normal timera channel */
#define IS_VALID_NORMAL_TIMERA_CHANNEL(x) \
    ((TimeraCh1 == (x)) ||                \
     (TimeraCh2 == (x)) ||                \
     (TimeraCh3 == (x)) ||                \
     (TimeraCh4 == (x)) ||                \
     (TimeraCh5 == (x)) ||                \
     (TimeraCh6 == (x)) ||                \
     (TimeraCh7 == (x)) ||                \
     (TimeraCh8 == (x)))

/*!< Parameter valid check for set cache channel */
#define IS_VALID_SET_CACHE_CHANNEL(x) \
    ((TimeraCh1 == (x)) ||            \
     (TimeraCh3 == (x)) ||            \
     (TimeraCh5 == (x)) ||            \
     (TimeraCh7 == (x)))

/*!< Parameter valid check for enable cache channel */
#define IS_VALID_ENABLE_CACHE_CHANNEL(x) \
    ((TimeraCh1 == (x)) ||               \
     (TimeraCh3 == (x)) ||               \
     (TimeraCh5 == (x)) ||               \
     (TimeraCh7 == (x)))

/*!< Parameter valid check for timera count start output status */
#define IS_VALID_COUNT_START_OUTPUT(x)      \
    ((TimeraCountStartOutputLow == (x)) ||  \
     (TimeraCountStartOutputHigh == (x)) || \
     (TimeraCountStartOutputKeep == (x)))

/*!< Parameter valid check for timera count stop output status */
#define IS_VALID_COUNT_STOP_OUTPUT(x)      \
    ((TimeraCountStopOutputLow == (x)) ||  \
     (TimeraCountStopOutputHigh == (x)) || \
     (TimeraCountStopOutputKeep == (x)))

/*!< Parameter valid check for compare match output status */
#define IS_VALID_COMPARE_MATCH_OUTPUT(x)      \
    ((TimeraCompareMatchOutputLow == (x)) ||  \
     (TimeraCompareMatchOutputHigh == (x)) || \
     (TimeraCompareMatchOutputKeep == (x)) || \
     (TimeraCompareMatchOutputReverse == (x)))

/*!< Parameter valid check for period match output status */
#define IS_VALID_PERIOD_MATCH_OUTPUT(x)      \
    ((TimeraPeriodMatchOutputLow == (x)) ||  \
     (TimeraPeriodMatchOutputHigh == (x)) || \
     (TimeraPeriodMatchOutputKeep == (x)) || \
     (TimeraPeriodMatchOutputReverse == (x)))

/*!< Parameter valid check for specify output status */
#define IS_VALID_SPECIFY_OUTPUT_STATUS(x)   \
    ((TimeraSpecifyOutputInvalid == (x)) || \
     (TimeraSpecifyOutputLow == (x)) ||     \
     (TimeraSpecifyOutputHigh == (x)))

/*!< Parameter valid check for port filter clock */
#define IS_VALID_PORT_FILTER_CLOCK(x)  \
    ((TimeraFilterPclkDiv1 == (x)) ||  \
     (TimeraFilterPclkDiv4 == (x)) ||  \
     (TimeraFilterPclkDiv16 == (x)) || \
     (TimeraFilterPclkDiv64 == (x)))

/*!< Parameter valid check for capture filter port source */
#define IS_VALID_CAPTURE_FILTER_PORT_SOURCE(x) \
    ((TimeraFilterSourceCh1 == (x)) ||         \
     (TimeraFilterSourceCh2 == (x)) ||         \
     (TimeraFilterSourceCh3 == (x)) ||         \
     (TimeraFilterSourceCh4 == (x)) ||         \
     (TimeraFilterSourceCh5 == (x)) ||         \
     (TimeraFilterSourceCh6 == (x)) ||         \
     (TimeraFilterSourceCh7 == (x)) ||         \
     (TimeraFilterSourceCh8 == (x)) ||         \
     (TimeraFilterSourceTrig == (x)))

/*!< Parameter valid check for coding filter port source */
#define IS_VALID_CODING_FILTER_PORT_SOURCE(x) \
    ((TimeraFilterSourceClkA == (x)) ||       \
     (TimeraFilterSourceClkB == (x)) ||       \
     (TimeraFilterSourceTrig == (x)))

/*!< Parameter valid check for interrupt request source */
#define IS_VALID_IRQ_SOURCE(x)                \
    ((TimeraIrqCaptureOrCompareCh1 == (x)) || \
     (TimeraIrqCaptureOrCompareCh2 == (x)) || \
     (TimeraIrqCaptureOrCompareCh3 == (x)) || \
     (TimeraIrqCaptureOrCompareCh4 == (x)) || \
     (TimeraIrqCaptureOrCompareCh5 == (x)) || \
     (TimeraIrqCaptureOrCompareCh6 == (x)) || \
     (TimeraIrqCaptureOrCompareCh7 == (x)) || \
     (TimeraIrqCaptureOrCompareCh8 == (x)) || \
     (TimeraIrqOverflow == (x)) ||            \
     (TimeraIrqUnderflow == (x)))

/*!< Parameter valid check for flag type */
#define IS_VALID_FLAG_TYPE(x)                  \
    ((TimeraFlagCaptureOrCompareCh1 == (x)) || \
     (TimeraFlagCaptureOrCompareCh2 == (x)) || \
     (TimeraFlagCaptureOrCompareCh3 == (x)) || \
     (TimeraFlagCaptureOrCompareCh4 == (x)) || \
     (TimeraFlagCaptureOrCompareCh5 == (x)) || \
     (TimeraFlagCaptureOrCompareCh6 == (x)) || \
     (TimeraFlagCaptureOrCompareCh7 == (x)) || \
     (TimeraFlagCaptureOrCompareCh8 == (x)) || \
     (TimeraFlagOverflow == (x)) ||            \
     (TimeraFlagUnderflow == (x)))

/*!< Parameter valid check for event source */
#define IS_VALID_EVENT_SOURCE(x) ((x) <= 511u)

/*!< Timera registers reset value */
#define TIMERA_REG_CNTER_RESET_VALUE (0x0000u)
#define TIMERA_REG_PERAR_RESET_VALUE (0xFFFFu)
#define TIMERA_REG_CMPAR_RESET_VALUE (0xFFFFu)
#define TIMERA_REG_BCSTR_RESET_VALUE (0x0002u)
#define TIMERA_REG_ICONR_RESET_VALUE (0x0000u)
#define TIMERA_REG_ECONR_RESET_VALUE (0x0000u)
#define TIMERA_REG_FCONR_RESET_VALUE (0x0000u)
#define TIMERA_REG_STFLR_RESET_VALUE (0x0000u)
#define TIMERA_REG_BCONR_RESET_VALUE (0x0000u)
#define TIMERA_REG_CCONR_RESET_VALUE (0x0000u)
#define TIMERA_REG_PCONR_RESET_VALUE (0x0000u)
#define TIMERA_REG_HCONR_RESET_VALUE (0x0000u)
#define TIMERA_REG_HCUPR_RESET_VALUE (0x0000u)
#define TIMERA_REG_HCDOR_RESET_VALUE (0x0000u)

#define TIMERA_REG_HTSSR0_RESET_VALUE (0x000001FFul)
#define TIMERA_REG_HTSSR1_RESET_VALUE (0x000001FFul)

/*!< Timera calculate register address of channel */
#define TIMERA_CALC_REG_ADDR(reg, chl) ((uint32_t)(&(reg)) + (chl)*0x4u)

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
 ** \brief De-Initialize Timera unit
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_DeInit(M4_TMRA_TypeDef *TIMERAx)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t u32Cnt = 0u;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        TIMERAx->CNTER = TIMERA_REG_CNTER_RESET_VALUE;
        TIMERAx->PERAR = TIMERA_REG_PERAR_RESET_VALUE;
        TIMERAx->BCSTR = TIMERA_REG_BCSTR_RESET_VALUE;
        TIMERAx->ICONR = TIMERA_REG_ICONR_RESET_VALUE;
        TIMERAx->ECONR = TIMERA_REG_ECONR_RESET_VALUE;
        TIMERAx->FCONR = TIMERA_REG_FCONR_RESET_VALUE;
        TIMERAx->STFLR = TIMERA_REG_STFLR_RESET_VALUE;
        TIMERAx->HCONR = TIMERA_REG_HCONR_RESET_VALUE;
        TIMERAx->HCUPR = TIMERA_REG_HCUPR_RESET_VALUE;
        TIMERAx->HCDOR = TIMERA_REG_HCDOR_RESET_VALUE;

        for (u32Cnt = 0u; u32Cnt < 8u; u32Cnt++)
        {
            *(__IO uint16_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CMPAR1, u32Cnt) = TIMERA_REG_CMPAR_RESET_VALUE;
        }
        for (u32Cnt = 0u; u32Cnt < 4u; u32Cnt++)
        {
            *(__IO uint16_t *)TIMERA_CALC_REG_ADDR(TIMERAx->BCONR1, u32Cnt * 2u) = TIMERA_REG_BCONR_RESET_VALUE;
        }
        for (u32Cnt = 0u; u32Cnt < 8u; u32Cnt++)
        {
            *(__IO uint16_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CCONR1, u32Cnt) = TIMERA_REG_CCONR_RESET_VALUE;
        }
        for (u32Cnt = 0u; u32Cnt < 8u; u32Cnt++)
        {
            *(__IO uint16_t *)TIMERA_CALC_REG_ADDR(TIMERAx->PCONR1, u32Cnt) = TIMERA_REG_PCONR_RESET_VALUE;
        }

        M4_AOS->TMRA_HTSSR0 = TIMERA_REG_HTSSR0_RESET_VALUE;
        M4_AOS->TMRA_HTSSR1 = TIMERA_REG_HTSSR1_RESET_VALUE;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize Timera unit base function
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] pstcBaseInit            Pointer to timera base init configuration
 ** \arg See the struct #stc_timera_base_init_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidMode            Unit 1 sync startup invalid
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **                                     - pstcBaseInit == NULL
 **
 ******************************************************************************/
en_result_t TIMERA_BaseInit(M4_TMRA_TypeDef *TIMERAx, const stc_timera_base_init_t *pstcBaseInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx) && (NULL != pstcBaseInit))
    {
        DDL_ASSERT(IS_VALID_COUNT_CLK_DIV(pstcBaseInit->enClkDiv));
        DDL_ASSERT(IS_VALID_COUNT_MODE(pstcBaseInit->enCntMode));
        DDL_ASSERT(IS_VALID_COUNT_DIR(pstcBaseInit->enCntDir));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcBaseInit->enSyncStartupEn));

        /* Configure control status register */
        TIMERAx->BCSTR_f.CKDIV = pstcBaseInit->enClkDiv;
        TIMERAx->BCSTR_f.MODE = pstcBaseInit->enCntMode;
        TIMERAx->BCSTR_f.DIR = pstcBaseInit->enCntDir;

        /* Unit 1 sync startup invalid */
        if ((M4_TMRA1 == TIMERAx) && (Enable == pstcBaseInit->enSyncStartupEn))
        {
            enRet = ErrorInvalidMode;
        }
        else
        {
            TIMERAx->BCSTR_f.SYNST = pstcBaseInit->enSyncStartupEn;
            enRet = Ok;
        }

        /* Configure period value register */
        TIMERAx->PERAR = pstcBaseInit->u16PeriodVal;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set Timera current count value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] u16Cnt                  Timera current count value
 ** \arg 0-0xFFFF
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_SetCurrCount(M4_TMRA_TypeDef *TIMERAx, uint16_t u16Cnt)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        TIMERAx->CNTER = u16Cnt;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timera current count value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \retval uint16_t                    Timera current count value
 **
 ******************************************************************************/
uint16_t TIMERA_GetCurrCount(M4_TMRA_TypeDef *TIMERAx)
{
    uint16_t u16CurrCntVal = 0u;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        u16CurrCntVal = (uint16_t)TIMERAx->CNTER;
    }

    return u16CurrCntVal;
}

/**
 *******************************************************************************
 ** \brief Set Timera period value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] u16Period               Timera period value
 ** \arg 0-0xFFFF
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_SetPeriodValue(M4_TMRA_TypeDef *TIMERAx, uint16_t u16Period)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        TIMERAx->PERAR = u16Period;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timera period count value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \retval uint16_t                    Timera current period value
 **
 ******************************************************************************/
uint16_t TIMERA_GetPeriodValue(M4_TMRA_TypeDef *TIMERAx)
{
    uint16_t u16PeriodVal = 0u;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        u16PeriodVal = (uint16_t)TIMERAx->PERAR;
    }

    return u16PeriodVal;
}

/**
 *******************************************************************************
 ** \brief Enable or disable Timera software synchronous startup
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable synchronous startup
 ** \arg Enable                         Enable synchronous startup
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_SyncStartupCmd(M4_TMRA_TypeDef *TIMERAx, en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SYNC_STARTUP_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        TIMERAx->BCSTR_f.SYNST = enNewSta;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable Timera startup
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable timera startup
 ** \arg Enable                         Enable timera startup
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_Cmd(M4_TMRA_TypeDef *TIMERAx, en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        TIMERAx->BCSTR_f.START = enNewSta;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize Timera unit compare function
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera compare channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh2                      Timera channel 2
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh4                      Timera channel 4
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh6                      Timera channel 6
 ** \arg TimeraCh7                      Timera channel 7
 ** \arg TimeraCh8                      Timera channel 8
 **
 ** \param [in] pstcCompareInit         Pointer to timera compare init configuration
 ** \arg See the struct #stc_timera_compare_init_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **                                     - pstcCompareInit == NULL
 **
 ******************************************************************************/
en_result_t TIMERA_CompareInit(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                               const stc_timera_compare_init_t *pstcCompareInit)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmra_pconr_field_t *pstcTimeraPort;
    __IO stc_tmra_bconr_field_t *pstcTimeraCache;
    __IO stc_tmra_cmpar_field_t *pstcTimeraCompare;
    __IO stc_tmra_cconr_field_t *pstcTimeraCapture;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx) && (NULL != pstcCompareInit))
    {
        DDL_ASSERT(IS_VALID_NORMAL_TIMERA_CHANNEL(enChannel));
        DDL_ASSERT(IS_VALID_COUNT_START_OUTPUT(pstcCompareInit->enStartCountOutput));
        DDL_ASSERT(IS_VALID_COUNT_STOP_OUTPUT(pstcCompareInit->enStopCountOutput));
        DDL_ASSERT(IS_VALID_COMPARE_MATCH_OUTPUT(pstcCompareInit->enCompareMatchOutput));
        DDL_ASSERT(IS_VALID_PERIOD_MATCH_OUTPUT(pstcCompareInit->enPeriodMatchOutput));
        DDL_ASSERT(IS_VALID_SPECIFY_OUTPUT_STATUS(pstcCompareInit->enSpecifyOutput));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCompareInit->enCacheEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCompareInit->enTriangularCrestTransEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCompareInit->enTriangularTroughTransEn));

        /* Configure port control register */
        pstcTimeraPort = (stc_tmra_pconr_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->PCONR1, enChannel);
        pstcTimeraPort->STAC = pstcCompareInit->enStartCountOutput;
        pstcTimeraPort->STPC = pstcCompareInit->enStopCountOutput;
        pstcTimeraPort->CMPC = pstcCompareInit->enCompareMatchOutput;
        pstcTimeraPort->PERC = pstcCompareInit->enPeriodMatchOutput;
        pstcTimeraPort->FORC = pstcCompareInit->enSpecifyOutput;

        /* Configure cache control register */
        if ((TimeraCh1 == enChannel) || (TimeraCh3 == enChannel) ||
            (TimeraCh5 == enChannel) || (TimeraCh7 == enChannel))
        {
            pstcTimeraCache = (stc_tmra_bconr_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->BCONR1, enChannel);
            pstcTimeraCache->BSE0 = pstcCompareInit->enTriangularCrestTransEn;
            pstcTimeraCache->BSE1 = pstcCompareInit->enTriangularTroughTransEn;
            pstcTimeraCache->BEN = pstcCompareInit->enCacheEn;
            /* Configure compare cache value register */
            pstcTimeraCompare = (stc_tmra_cmpar_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CMPAR1, enChannel + 1);
            pstcTimeraCompare->CMP = pstcCompareInit->u16CompareCacheVal;
        }

        /* Configure compare value register */
        pstcTimeraCompare = (stc_tmra_cmpar_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CMPAR1, enChannel);
        pstcTimeraCompare->CMP = pstcCompareInit->u16CompareVal;

        /* Set compare output function */
        pstcTimeraCapture = (stc_tmra_cconr_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CCONR1, enChannel);
        pstcTimeraCapture->CAPMDA = 0u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set Timera compare value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera compare channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh2                      Timera channel 2
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh4                      Timera channel 4
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh6                      Timera channel 6
 ** \arg TimeraCh7                      Timera channel 7
 ** \arg TimeraCh8                      Timera channel 8
 **
 ** \param [in] u16CompareVal           Timera campare value
 ** \arg 0-0xFFFF
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_SetCompareValue(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                   uint16_t u16CompareVal)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmra_cmpar_field_t *pstcTimeraCompare;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_NORMAL_TIMERA_CHANNEL(enChannel));

        pstcTimeraCompare = (stc_tmra_cmpar_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CMPAR1, enChannel);
        pstcTimeraCompare->CMP = u16CompareVal;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timera compare value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera compare channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh2                      Timera channel 2
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh4                      Timera channel 4
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh6                      Timera channel 6
 ** \arg TimeraCh7                      Timera channel 7
 ** \arg TimeraCh8                      Timera channel 8
 **
 ** \retval uint16_t                    Timera compare value
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
uint16_t TIMERA_GetCompareValue(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel)
{
    uint16_t u16CompareVal = 0u;
    __IO stc_tmra_cmpar_field_t *pstcTimeraCompare;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_NORMAL_TIMERA_CHANNEL(enChannel));

        pstcTimeraCompare = (stc_tmra_cmpar_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CMPAR1, enChannel);
        u16CompareVal = (uint16_t)pstcTimeraCompare->CMP;
    }

    return u16CompareVal;
}

/**
 *******************************************************************************
 ** \brief Set Timera compare cache value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera compare channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh7                      Timera channel 7
 **
 ** \param [in] u16CompareCache         Timera compare cache value
 ** \arg 0-0xFFFF
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_SetCacheValue(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                 uint16_t u16CompareCache)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmra_cmpar_field_t *pstcTimeraCompare;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_SET_CACHE_CHANNEL(enChannel));

        pstcTimeraCompare = (stc_tmra_cmpar_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CMPAR1, enChannel + 1);
        pstcTimeraCompare->CMP = u16CompareCache;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable Timera compare cache
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera compare channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh7                      Timera channel 7
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable timera compare cache
 ** \arg Enable                         Enable timera compare cache
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_CompareCacheCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                   en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmra_bconr_field_t *pstcTimeraCache;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_ENABLE_CACHE_CHANNEL(enChannel));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        /* Configure cache control register */
        pstcTimeraCache = (stc_tmra_bconr_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->BCONR1, enChannel);
        pstcTimeraCache->BEN = enNewSta;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Specify Timera port output status
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera compare channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh2                      Timera channel 2
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh4                      Timera channel 4
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh6                      Timera channel 6
 ** \arg TimeraCh7                      Timera channel 7
 ** \arg TimeraCh8                      Timera channel 8
 **
 ** \param [in] enOutputSta             Timera port output status
 ** \arg TimeraSpecifyOutputInvalid     Port output invalid
 ** \arg TimeraSpecifyOutputLow         Port output low level from next period
 ** \arg TimeraSpecifyOutputHigh        Port output high level from next period
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_SpecifyOutputSta(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                                    en_timera_specify_output_t enOutputSta)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmra_pconr_field_t *pstcTimeraPort;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_NORMAL_TIMERA_CHANNEL(enChannel));
        DDL_ASSERT(IS_VALID_SPECIFY_OUTPUT_STATUS(enOutputSta));

        pstcTimeraPort = (stc_tmra_pconr_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->PCONR1, enChannel);
        pstcTimeraPort->FORC = enOutputSta;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable Timera compare function
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera compare channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh2                      Timera channel 2
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh4                      Timera channel 4
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh6                      Timera channel 6
 ** \arg TimeraCh7                      Timera channel 7
 ** \arg TimeraCh8                      Timera channel 8
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable timera compare function
 ** \arg Enable                         Enable timera compare function
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_CompareCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                              en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmra_pconr_field_t *pstcTimeraPort;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_NORMAL_TIMERA_CHANNEL(enChannel));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        pstcTimeraPort = (stc_tmra_pconr_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->PCONR1, enChannel);
        pstcTimeraPort->OUTEN = enNewSta;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize Timera unit capture function
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera capture channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh2                      Timera channel 2
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh4                      Timera channel 4
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh6                      Timera channel 6
 ** \arg TimeraCh7                      Timera channel 7
 ** \arg TimeraCh8                      Timera channel 8
 **
 ** \param [in] pstcCapInit             Pointer to timera capture init configuration
 ** \arg See the struct #stc_timera_capture_init_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **                                     - pstcCapInit == NULL
 **
 ******************************************************************************/
en_result_t TIMERA_CaptureInit(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                               const stc_timera_capture_init_t *pstcCapInit)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmra_cconr_field_t *pstcTimeraCapture;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx) && (NULL != pstcCapInit))
    {
        DDL_ASSERT(IS_VALID_NORMAL_TIMERA_CHANNEL(enChannel));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCapInit->enCapturePwmRisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCapInit->enCapturePwmFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCapInit->enCaptureSpecifyEventEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCapInit->enCaptureTrigFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCapInit->enCaptureTrigRisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCapInit->enPwmFilterEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCapInit->enTrigFilterEn));
        DDL_ASSERT(IS_VALID_PORT_FILTER_CLOCK(pstcCapInit->enPwmClkDiv));
        DDL_ASSERT(IS_VALID_PORT_FILTER_CLOCK(pstcCapInit->enTrigClkDiv));

        /* Configure capture control register */
        pstcTimeraCapture = (stc_tmra_cconr_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CCONR1, enChannel);
        pstcTimeraCapture->HICP0 = pstcCapInit->enCapturePwmRisingEn;
        pstcTimeraCapture->HICP1 = pstcCapInit->enCapturePwmFallingEn;
        pstcTimeraCapture->HICP2 = pstcCapInit->enCaptureSpecifyEventEn;
        pstcTimeraCapture->NOFICKCP = pstcCapInit->enPwmClkDiv;
        pstcTimeraCapture->NOFIENCP = pstcCapInit->enPwmFilterEn;

        /* TIMA_<t>_TRIG port capture function only valid for TimeraCh4 */
        if (TimeraCh4 == enChannel)
        {
            pstcTimeraCapture->HICP3 = pstcCapInit->enCaptureTrigRisingEn;
            pstcTimeraCapture->HICP4 = pstcCapInit->enCaptureTrigFallingEn;
            /* Configure filter control register */
            TIMERAx->FCONR_f.NOFICKTG = pstcCapInit->enTrigClkDiv;
            TIMERAx->FCONR_f.NOFIENTG = pstcCapInit->enTrigFilterEn;
        }

        /* Set capture input function */
        pstcTimeraCapture->CAPMDA = 1u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable Timera capture filter
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enFilterPort            Timera capture filter input port
 ** \arg TimeraFilterSourceCh1          TIMA_<t>_PWM1 input port
 ** \arg TimeraFilterSourceCh2          TIMA_<t>_PWM2 input port
 ** \arg TimeraFilterSourceCh3          TIMA_<t>_PWM3 input port
 ** \arg TimeraFilterSourceCh4          TIMA_<t>_PWM4 input port
 ** \arg TimeraFilterSourceCh5          TIMA_<t>_PWM5 input port
 ** \arg TimeraFilterSourceCh6          TIMA_<t>_PWM6 input port
 ** \arg TimeraFilterSourceCh7          TIMA_<t>_PWM7 input port
 ** \arg TimeraFilterSourceCh8          TIMA_<t>_PWM8 input port
 ** \arg TimeraFilterSourceTrig         TIMA_<t>_TRIG input port
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable timera capture filter
 ** \arg Enable                         Enable timera capture filter
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_CaptureFilterCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_filter_source_t enFilterPort,
                                    en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO stc_tmra_cconr_field_t *pstcTimeraCapture;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_CAPTURE_FILTER_PORT_SOURCE(enFilterPort));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        if (TimeraFilterSourceTrig == enFilterPort)
        {
            TIMERAx->FCONR_f.NOFIENTG = enNewSta;
        }
        else
        {
            pstcTimeraCapture = (stc_tmra_cconr_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CCONR1, enFilterPort);
            pstcTimeraCapture->NOFIENCP = enNewSta;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timera capture value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera capture channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh2                      Timera channel 2
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh4                      Timera channel 4
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh6                      Timera channel 6
 ** \arg TimeraCh7                      Timera channel 7
 ** \arg TimeraCh8                      Timera channel 8
 **
 ** \retval uint16_t                    Timera capture value
 **
 ******************************************************************************/
uint16_t TIMERA_GetCaptureValue(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel)
{
    uint16_t u16CapVal = 0u;
    __IO stc_tmra_cmpar_field_t *pstcTimeraCompare;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_NORMAL_TIMERA_CHANNEL(enChannel));

        pstcTimeraCompare = (stc_tmra_cmpar_field_t *)TIMERA_CALC_REG_ADDR(TIMERAx->CMPAR1, enChannel);
        u16CapVal = (uint16_t)pstcTimeraCompare->CMP;
    }

    return u16CapVal;
}

/**
 *******************************************************************************
 ** \brief Initialize Timera unit orthogonal coding function
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] pstcCodingInit          Pointer to timera orthogonal coding configuration
 ** \arg See the struct #stc_timera_orthogonal_coding_init_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **                                     - pstcCodingInit == NULL
 **
 ******************************************************************************/
en_result_t TIMERA_OrthogonalCodingInit(M4_TMRA_TypeDef *TIMERAx, const stc_timera_orthogonal_coding_init_t *pstcCodingInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx) && (NULL != pstcCodingInit))
    {
        DDL_ASSERT(IS_VALID_PORT_FILTER_CLOCK(pstcCodingInit->enTrigClkDiv));
        DDL_ASSERT(IS_VALID_PORT_FILTER_CLOCK(pstcCodingInit->enClkBClkDiv));
        DDL_ASSERT(IS_VALID_PORT_FILTER_CLOCK(pstcCodingInit->enClkAClkDiv));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enTrigFilterEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enClkBFilterEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enClkAFilterEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncClkALowAndClkBRisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncClkALowAndClkBFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncClkAHighAndClkBRisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncClkAHighAndClkBFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncClkBLowAndClkARisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncClkBLowAndClkAFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncClkBHighAndClkARisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncClkBHighAndClkAFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncTrigRisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncTrigFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncSpecifyEventTriggerEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncAnotherUnitOverflowEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enIncAnotherUnitUnderflowEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecClkALowAndClkBRisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecClkALowAndClkBFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecClkAHighAndClkBRisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecClkAHighAndClkBFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecClkBLowAndClkARisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecClkBLowAndClkAFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecClkBHighAndClkARisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecClkBHighAndClkAFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecTrigRisingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecTrigFallingEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecSpecifyEventTriggerEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecAnotherUnitOverflowEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCodingInit->enDecAnotherUnitUnderflowEn));

        /* Configure hardware increase event register */
        TIMERAx->HCUPR_f.HCUP0 = pstcCodingInit->enIncClkALowAndClkBRisingEn;
        TIMERAx->HCUPR_f.HCUP1 = pstcCodingInit->enIncClkALowAndClkBFallingEn;
        TIMERAx->HCUPR_f.HCUP2 = pstcCodingInit->enIncClkAHighAndClkBRisingEn;
        TIMERAx->HCUPR_f.HCUP3 = pstcCodingInit->enIncClkAHighAndClkBFallingEn;
        TIMERAx->HCUPR_f.HCUP4 = pstcCodingInit->enIncClkBLowAndClkARisingEn;
        TIMERAx->HCUPR_f.HCUP5 = pstcCodingInit->enIncClkBLowAndClkAFallingEn;
        TIMERAx->HCUPR_f.HCUP6 = pstcCodingInit->enIncClkBHighAndClkARisingEn;
        TIMERAx->HCUPR_f.HCUP7 = pstcCodingInit->enIncClkBHighAndClkAFallingEn;
        TIMERAx->HCUPR_f.HCUP8 = pstcCodingInit->enIncTrigRisingEn;
        TIMERAx->HCUPR_f.HCUP9 = pstcCodingInit->enIncTrigFallingEn;
        TIMERAx->HCUPR_f.HCUP10 = pstcCodingInit->enIncSpecifyEventTriggerEn;
        TIMERAx->HCUPR_f.HCUP11 = pstcCodingInit->enIncAnotherUnitOverflowEn;
        TIMERAx->HCUPR_f.HCUP12 = pstcCodingInit->enIncAnotherUnitUnderflowEn;

        /* Configure hardware decrease event register */
        TIMERAx->HCDOR_f.HCDO0 = pstcCodingInit->enDecClkALowAndClkBRisingEn;
        TIMERAx->HCDOR_f.HCDO1 = pstcCodingInit->enDecClkALowAndClkBFallingEn;
        TIMERAx->HCDOR_f.HCDO2 = pstcCodingInit->enDecClkAHighAndClkBRisingEn;
        TIMERAx->HCDOR_f.HCDO3 = pstcCodingInit->enDecClkAHighAndClkBFallingEn;
        TIMERAx->HCDOR_f.HCDO4 = pstcCodingInit->enDecClkBLowAndClkARisingEn;
        TIMERAx->HCDOR_f.HCDO5 = pstcCodingInit->enDecClkBLowAndClkAFallingEn;
        TIMERAx->HCDOR_f.HCDO6 = pstcCodingInit->enDecClkBHighAndClkARisingEn;
        TIMERAx->HCDOR_f.HCDO7 = pstcCodingInit->enDecClkBHighAndClkAFallingEn;
        TIMERAx->HCDOR_f.HCDO8 = pstcCodingInit->enDecTrigRisingEn;
        TIMERAx->HCDOR_f.HCDO9 = pstcCodingInit->enDecTrigFallingEn;
        TIMERAx->HCDOR_f.HCDO10 = pstcCodingInit->enDecSpecifyEventTriggerEn;
        TIMERAx->HCDOR_f.HCDO11 = pstcCodingInit->enDecAnotherUnitOverflowEn;
        TIMERAx->HCDOR_f.HCDO12 = pstcCodingInit->enDecAnotherUnitUnderflowEn;

        /* Configure filter control register */
        TIMERAx->FCONR_f.NOFICKTG = pstcCodingInit->enTrigClkDiv;
        TIMERAx->FCONR_f.NOFIENTG = pstcCodingInit->enTrigFilterEn;
        TIMERAx->FCONR_f.NOFICKCB = pstcCodingInit->enClkBClkDiv;
        TIMERAx->FCONR_f.NOFIENCB = pstcCodingInit->enClkBFilterEn;
        TIMERAx->FCONR_f.NOFICKCA = pstcCodingInit->enClkAClkDiv;
        TIMERAx->FCONR_f.NOFIENCA = pstcCodingInit->enClkAFilterEn;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set Timera orthogonal coding value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] u16CodingCnt            Timera orthogonal coding value
 ** \arg 0-0xFFFF
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_SetOrthogonalCodingCount(M4_TMRA_TypeDef *TIMERAx, uint16_t u16CodingCnt)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        TIMERAx->CNTER = u16CodingCnt;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timera orthogonal coding value
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \retval uint16_t                    Timera orthogonal coding value
 **
 ******************************************************************************/
uint16_t TIMERA_GetOrthogonalCodingCount(M4_TMRA_TypeDef *TIMERAx)
{
    uint16_t u16CodingCnt = 0u;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        u16CodingCnt = (uint16_t)TIMERAx->CNTER;
    }

    return u16CodingCnt;
}

/**
 *******************************************************************************
 ** \brief Enable or disable Timera orthogonal coding filter
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enFilterPort            Timera orthogonal coding filter input port
 ** \arg TimeraFilterSourceClkA         TIMA_<t>_CLKA input port
 ** \arg TimeraFilterSourceClkB         TIMA_<t>_CLKB input port
 ** \arg TimeraFilterSourceTrig         TIMA_<t>_TRIG input port
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable timera orthogonal coding filter
 ** \arg Enable                         Enable timera orthogonal coding filter
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_OrthogonalCodingFilterCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_filter_source_t enFilterPort,
                                             en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_CODING_FILTER_PORT_SOURCE(enFilterPort));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        switch (enFilterPort)
        {
        case TimeraFilterSourceClkA:
            TIMERAx->FCONR_f.NOFIENCA = enNewSta;
            break;
        case TimeraFilterSourceClkB:
            TIMERAx->FCONR_f.NOFIENCB = enNewSta;
            break;
        case TimeraFilterSourceTrig:
            TIMERAx->FCONR_f.NOFIENTG = enNewSta;
            break;
        default:
            break;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize Timera unit hardware trigger event function
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] pstcHwTriggerInit       Pointer to timera hardware trigger event configuration
 ** \arg See the struct #stc_timera_hw_trigger_init_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **                                     - pstcHwTriggerInit == NULL
 **
 ** \note If sync startup(BCSTR.SYNST) bit set 1 trigger hardware sync startup when HCONR.HSTA1~0 bit set
 **
 ******************************************************************************/
en_result_t TIMERA_HwTriggerInit(M4_TMRA_TypeDef *TIMERAx, const stc_timera_hw_trigger_init_t *pstcHwTriggerInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx) && (NULL != pstcHwTriggerInit))
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwStartup.enTrigRisingStartupEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwStartup.enTrigFallingStartupEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwStartup.enSpecifyEventStartupEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwStop.enTrigRisingStopEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwStop.enTrigFallingStopEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwStop.enSpecifyEventStopEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwClear.enTrigRisingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwClear.enTrigFallingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwClear.enSpecifyEventClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwClear.enAnotherUnitTrigRisingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwClear.enAnotherUnitTrigFallingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwClear.enChannel3RisingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwTriggerInit->stcHwClear.enChannel3FallingClearEn));

        /* Configure hardware startup conditions */
        TIMERAx->HCONR_f.HSTA0 = pstcHwTriggerInit->stcHwStartup.enTrigRisingStartupEn;
        TIMERAx->HCONR_f.HSTA1 = pstcHwTriggerInit->stcHwStartup.enTrigFallingStartupEn;
        TIMERAx->HCONR_f.HSTA2 = pstcHwTriggerInit->stcHwStartup.enSpecifyEventStartupEn;

        /* Configure hardware stop conditions */
        TIMERAx->HCONR_f.HSTP0 = pstcHwTriggerInit->stcHwStop.enTrigRisingStopEn;
        TIMERAx->HCONR_f.HSTP1 = pstcHwTriggerInit->stcHwStop.enTrigFallingStopEn;
        TIMERAx->HCONR_f.HSTP2 = pstcHwTriggerInit->stcHwStop.enSpecifyEventStopEn;

        /* Configure hardware clear conditions */
        TIMERAx->HCONR_f.HCLE0 = pstcHwTriggerInit->stcHwClear.enTrigRisingClearEn;
        TIMERAx->HCONR_f.HCLE1 = pstcHwTriggerInit->stcHwClear.enTrigFallingClearEn;
        TIMERAx->HCONR_f.HCLE2 = pstcHwTriggerInit->stcHwClear.enSpecifyEventClearEn;
        TIMERAx->HCONR_f.HCLE3 = pstcHwTriggerInit->stcHwClear.enAnotherUnitTrigRisingClearEn;
        TIMERAx->HCONR_f.HCLE4 = pstcHwTriggerInit->stcHwClear.enAnotherUnitTrigFallingClearEn;
        TIMERAx->HCONR_f.HCLE5 = pstcHwTriggerInit->stcHwClear.enChannel3RisingClearEn;
        TIMERAx->HCONR_f.HCLE6 = pstcHwTriggerInit->stcHwClear.enChannel3FallingClearEn;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Timera hardware startup Config
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] pstcHwStartup           Pointer to timera hardware startup configuration
 ** \arg See the struct #stc_timera_hw_startup_cofig_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **                                     - pstcHwStartup == NULL
 **
 ******************************************************************************/
en_result_t TIMERA_HwStartupConfig(M4_TMRA_TypeDef *TIMERAx, const stc_timera_hw_startup_cofig_t *pstcHwStartup)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx) && (NULL != pstcHwStartup))
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwStartup->enTrigRisingStartupEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwStartup->enTrigFallingStartupEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwStartup->enSpecifyEventStartupEn));

        TIMERAx->HCONR_f.HSTA0 = pstcHwStartup->enTrigRisingStartupEn;
        TIMERAx->HCONR_f.HSTA1 = pstcHwStartup->enTrigFallingStartupEn;
        TIMERAx->HCONR_f.HSTA2 = pstcHwStartup->enSpecifyEventStartupEn;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Timera hardware stop Config
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] pstcHwStop              Pointer to timera hardware stop configuration
 ** \arg See the struct #stc_timera_hw_stop_cofig_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **                                     - pstcHwStop == NULL
 **
 ******************************************************************************/
en_result_t TIMERA_HwStopConfig(M4_TMRA_TypeDef *TIMERAx, const stc_timera_hw_stop_cofig_t *pstcHwStop)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx) && (NULL != pstcHwStop))
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwStop->enTrigRisingStopEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwStop->enTrigFallingStopEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwStop->enSpecifyEventStopEn));

        TIMERAx->HCONR_f.HSTP0 = pstcHwStop->enTrigRisingStopEn;
        TIMERAx->HCONR_f.HSTP1 = pstcHwStop->enTrigFallingStopEn;
        TIMERAx->HCONR_f.HSTP2 = pstcHwStop->enSpecifyEventStopEn;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Timera hardware clear Config
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] pstcHwClear             Pointer to timera hardware clear configuration
 ** \arg See the struct #stc_timera_hw_clear_config_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **                                     - pstcHwClear == NULL
 **
 ******************************************************************************/
en_result_t TIMERA_HwClearConfig(M4_TMRA_TypeDef *TIMERAx, const stc_timera_hw_clear_config_t *pstcHwClear)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx) && (NULL != pstcHwClear))
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwClear->enTrigRisingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwClear->enTrigFallingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwClear->enSpecifyEventClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwClear->enAnotherUnitTrigRisingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwClear->enAnotherUnitTrigFallingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwClear->enChannel3RisingClearEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcHwClear->enChannel3FallingClearEn));

        TIMERAx->HCONR_f.HCLE0 = pstcHwClear->enTrigRisingClearEn;
        TIMERAx->HCONR_f.HCLE1 = pstcHwClear->enTrigFallingClearEn;
        TIMERAx->HCONR_f.HCLE2 = pstcHwClear->enSpecifyEventClearEn;
        TIMERAx->HCONR_f.HCLE3 = pstcHwClear->enAnotherUnitTrigRisingClearEn;
        TIMERAx->HCONR_f.HCLE4 = pstcHwClear->enAnotherUnitTrigFallingClearEn;
        TIMERAx->HCONR_f.HCLE5 = pstcHwClear->enChannel3RisingClearEn;
        TIMERAx->HCONR_f.HCLE6 = pstcHwClear->enChannel3FallingClearEn;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable Timera interrupt request
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enIrq                   Timera interrupt request
 ** \arg TimeraIrqCaptureOrCompareCh1   Channel 1 interrupt request
 ** \arg TimeraIrqCaptureOrCompareCh2   Channel 2 interrupt request
 ** \arg TimeraIrqCaptureOrCompareCh3   Channel 3 interrupt request
 ** \arg TimeraIrqCaptureOrCompareCh4   Channel 4 interrupt request
 ** \arg TimeraIrqCaptureOrCompareCh5   Channel 5 interrupt request
 ** \arg TimeraIrqCaptureOrCompareCh6   Channel 6 interrupt request
 ** \arg TimeraIrqCaptureOrCompareCh7   Channel 7 interrupt request
 ** \arg TimeraIrqCaptureOrCompareCh8   Channel 8 interrupt request
 ** \arg TimeraIrqOverflow              Count overflow interrupt request
 ** \arg TimeraIrqUnderflow             Count underflow interrupt request
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable timera interrupt request
 ** \arg Enable                         Enable timera interrupt request
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_IrqCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_irq_type_t enIrq,
                          en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_IRQ_SOURCE(enIrq));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        switch (enIrq)
        {
        case TimeraIrqCaptureOrCompareCh1:
            TIMERAx->ICONR_f.ITEN1 = enNewSta;
            break;
        case TimeraIrqCaptureOrCompareCh2:
            TIMERAx->ICONR_f.ITEN2 = enNewSta;
            break;
        case TimeraIrqCaptureOrCompareCh3:
            TIMERAx->ICONR_f.ITEN3 = enNewSta;
            break;
        case TimeraIrqCaptureOrCompareCh4:
            TIMERAx->ICONR_f.ITEN4 = enNewSta;
            break;
        case TimeraIrqCaptureOrCompareCh5:
            TIMERAx->ICONR_f.ITEN5 = enNewSta;
            break;
        case TimeraIrqCaptureOrCompareCh6:
            TIMERAx->ICONR_f.ITEN6 = enNewSta;
            break;
        case TimeraIrqCaptureOrCompareCh7:
            TIMERAx->ICONR_f.ITEN7 = enNewSta;
            break;
        case TimeraIrqCaptureOrCompareCh8:
            TIMERAx->ICONR_f.ITEN8 = enNewSta;
            break;
        case TimeraIrqOverflow:
            TIMERAx->BCSTR_f.ITENOVF = enNewSta;
            break;
        case TimeraIrqUnderflow:
            TIMERAx->BCSTR_f.ITENUDF = enNewSta;
            break;
        default:
            break;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable Timera event request
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enChannel               Timera event request channel
 ** \arg TimeraCh1                      Timera channel 1
 ** \arg TimeraCh2                      Timera channel 2
 ** \arg TimeraCh3                      Timera channel 3
 ** \arg TimeraCh4                      Timera channel 4
 ** \arg TimeraCh5                      Timera channel 5
 ** \arg TimeraCh6                      Timera channel 6
 ** \arg TimeraCh7                      Timera channel 7
 ** \arg TimeraCh8                      Timera channel 8
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable timera event request
 ** \arg Enable                         Enable timera event request
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_EventCmd(M4_TMRA_TypeDef *TIMERAx, en_timera_channel_t enChannel,
                            en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_NORMAL_TIMERA_CHANNEL(enChannel));

        switch (enChannel)
        {
        case TimeraCh1:
            TIMERAx->ECONR_f.ETEN1 = enNewSta;
            break;
        case TimeraCh2:
            TIMERAx->ECONR_f.ETEN2 = enNewSta;
            break;
        case TimeraCh3:
            TIMERAx->ECONR_f.ETEN3 = enNewSta;
            break;
        case TimeraCh4:
            TIMERAx->ECONR_f.ETEN4 = enNewSta;
            break;
        case TimeraCh5:
            TIMERAx->ECONR_f.ETEN5 = enNewSta;
            break;
        case TimeraCh6:
            TIMERAx->ECONR_f.ETEN6 = enNewSta;
            break;
        case TimeraCh7:
            TIMERAx->ECONR_f.ETEN7 = enNewSta;
            break;
        case TimeraCh8:
            TIMERAx->ECONR_f.ETEN8 = enNewSta;
            break;
        default:
            break;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Timera flag status
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enFlag                  Timera flag type
 ** \arg TimeraFlagCaptureOrCompareCh1  Channel 1 match flag
 ** \arg TimeraFlagCaptureOrCompareCh2  Channel 2 match flag
 ** \arg TimeraFlagCaptureOrCompareCh3  Channel 3 match flag
 ** \arg TimeraFlagCaptureOrCompareCh4  Channel 4 match flag
 ** \arg TimeraFlagCaptureOrCompareCh5  Channel 5 match flag
 ** \arg TimeraFlagCaptureOrCompareCh6  Channel 6 match flag
 ** \arg TimeraFlagCaptureOrCompareCh7  Channel 7 match flag
 ** \arg TimeraFlagCaptureOrCompareCh8  Channel 8 match flag
 ** \arg TimeraFlagOverflow             Count overflow flag
 ** \arg TimeraFlagUnderflow            Count underflow flag
 **
 ** \retval Set                         Flag is set
 ** \retval Reset                       Flag is reset
 **
 ******************************************************************************/
en_flag_status_t TIMERA_GetFlag(M4_TMRA_TypeDef *TIMERAx, en_timera_flag_type_t enFlag)
{
    en_flag_status_t enFlagSta = Reset;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_FLAG_TYPE(enFlag));

        switch (enFlag)
        {
        case TimeraFlagCaptureOrCompareCh1:
            enFlagSta = (en_flag_status_t)TIMERAx->STFLR_f.CMPF1;
            break;
        case TimeraFlagCaptureOrCompareCh2:
            enFlagSta = (en_flag_status_t)TIMERAx->STFLR_f.CMPF2;
            break;
        case TimeraFlagCaptureOrCompareCh3:
            enFlagSta = (en_flag_status_t)TIMERAx->STFLR_f.CMPF3;
            break;
        case TimeraFlagCaptureOrCompareCh4:
            enFlagSta = (en_flag_status_t)TIMERAx->STFLR_f.CMPF4;
            break;
        case TimeraFlagCaptureOrCompareCh5:
            enFlagSta = (en_flag_status_t)TIMERAx->STFLR_f.CMPF5;
            break;
        case TimeraFlagCaptureOrCompareCh6:
            enFlagSta = (en_flag_status_t)TIMERAx->STFLR_f.CMPF6;
            break;
        case TimeraFlagCaptureOrCompareCh7:
            enFlagSta = (en_flag_status_t)TIMERAx->STFLR_f.CMPF7;
            break;
        case TimeraFlagCaptureOrCompareCh8:
            enFlagSta = (en_flag_status_t)TIMERAx->STFLR_f.CMPF8;
            break;
        case TimeraFlagOverflow:
            enFlagSta = (en_flag_status_t)TIMERAx->BCSTR_f.OVFF;
            break;
        case TimeraFlagUnderflow:
            enFlagSta = (en_flag_status_t)TIMERAx->BCSTR_f.UDFF;
            break;
        default:
            break;
        }
    }

    return enFlagSta;
}

/**
 *******************************************************************************
 ** \brief Clear Timera flag status
 **
 ** \param [in] TIMERAx                 Pointer to timera unit configuration address
 ** \arg M4_TMRA1                       Timera unit 1 configuration Address
 ** \arg M4_TMRA2                       Timera unit 2 configuration Address
 ** \arg M4_TMRA3                       Timera unit 3 configuration Address
 ** \arg M4_TMRA4                       Timera unit 4 configuration Address
 ** \arg M4_TMRA5                       Timera unit 5 configuration Address
 ** \arg M4_TMRA6                       Timera unit 6 configuration Address
 **
 ** \param [in] enFlag                  Timera flag type
 ** \arg TimeraFlagCaptureOrCompareCh1  Channel 1 match flag
 ** \arg TimeraFlagCaptureOrCompareCh2  Channel 2 match flag
 ** \arg TimeraFlagCaptureOrCompareCh3  Channel 3 match flag
 ** \arg TimeraFlagCaptureOrCompareCh4  Channel 4 match flag
 ** \arg TimeraFlagCaptureOrCompareCh5  Channel 5 match flag
 ** \arg TimeraFlagCaptureOrCompareCh6  Channel 6 match flag
 ** \arg TimeraFlagCaptureOrCompareCh7  Channel 7 match flag
 ** \arg TimeraFlagCaptureOrCompareCh8  Channel 8 match flag
 ** \arg TimeraFlagOverflow             Count overflow flag
 ** \arg TimeraFlagUnderflow            Count underflow flag
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - TIMERAx is invalid
 **
 ******************************************************************************/
en_result_t TIMERA_ClearFlag(M4_TMRA_TypeDef *TIMERAx, en_timera_flag_type_t enFlag)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_NORMAL_TIMERA_UNIT(TIMERAx))
    {
        DDL_ASSERT(IS_VALID_FLAG_TYPE(enFlag));

        switch (enFlag)
        {
        case TimeraFlagCaptureOrCompareCh1:
            TIMERAx->STFLR_f.CMPF1 = 0u;
            break;
        case TimeraFlagCaptureOrCompareCh2:
            TIMERAx->STFLR_f.CMPF2 = 0u;
            break;
        case TimeraFlagCaptureOrCompareCh3:
            TIMERAx->STFLR_f.CMPF3 = 0u;
            break;
        case TimeraFlagCaptureOrCompareCh4:
            TIMERAx->STFLR_f.CMPF4 = 0u;
            break;
        case TimeraFlagCaptureOrCompareCh5:
            TIMERAx->STFLR_f.CMPF5 = 0u;
            break;
        case TimeraFlagCaptureOrCompareCh6:
            TIMERAx->STFLR_f.CMPF6 = 0u;
            break;
        case TimeraFlagCaptureOrCompareCh7:
            TIMERAx->STFLR_f.CMPF7 = 0u;
            break;
        case TimeraFlagCaptureOrCompareCh8:
            TIMERAx->STFLR_f.CMPF8 = 0u;
            break;
        case TimeraFlagOverflow:
            TIMERAx->BCSTR_f.OVFF = 0u;
            break;
        case TimeraFlagUnderflow:
            TIMERAx->BCSTR_f.UDFF = 0u;
            break;
        default:
            break;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set counter event trigger source
 **
 ** \param [in] enTriggerSrc            Counter event trigger source
 ** \arg 0-511                          Used to trigger counter start/stop/clear/increment/decrement
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t TIMERA_SetCountTriggerSrc(en_event_src_t enTriggerSrc)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_EVENT_SOURCE(enTriggerSrc));

    M4_AOS->TMRA_HTSSR0 = enTriggerSrc;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set capture event trigger source
 **
 ** \param [in] enTriggerSrc            Capture event trigger source
 ** \arg 0-511                          Used to trigger the capture function
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t TIMERA_SetCaptureTriggerSrc(en_event_src_t enTriggerSrc)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_EVENT_SOURCE(enTriggerSrc));

    M4_AOS->TMRA_HTSSR1 = enTriggerSrc;

    return enRet;
}

//@} // TimeraGroup

#endif /* DDL_TIMERA_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
