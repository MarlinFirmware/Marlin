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
/** \file hc32f46x_timer6.c
 **
 ** A detailed description is available at
 ** @link Timer6Group Timer6 description @endlink
 **
 **   - 2018-11-23  1.0  Husj  First version for Device Driver Library of Timer6.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_timer6.h"
#include "hc32f46x_utility.h"

#if (DDL_TIMER6_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup Timer6Group
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*!< Parameter valid check for normal timer6 unit */
#define IS_VALID_NORMAL_TIMER6_UNIT(__TMR6x__) \
    ((M4_TMR61 == (__TMR6x__)) ||              \
     (M4_TMR62 == (__TMR6x__)) ||              \
     (M4_TMR63 == (__TMR6x__)))

/*!< Parameter valid check for period register*/
#define IS_VALID_PERIOD_TYPE(x) \
    ((Timer6PeriodA == (x)) ||  \
     (Timer6PeriodB == (x)) ||  \
     (Timer6PeriodC == (x)))
/*!< Parameter valid check for General compare register*/
#define IS_VALID_GEN_CMP_TYPE(x)   \
    ((Timer6GenCompareA == (x)) || \
     (Timer6GenCompareB == (x)) || \
     (Timer6GenCompareC == (x)) || \
     (Timer6GenCompareD == (x)) || \
     (Timer6GenCompareE == (x)) || \
     (Timer6GenCompareF == (x)))

/*!< Parameter valid check for Special compare register*/
#define IS_VALID_SPECIAL_CMP_TYPE(x) \
    ((Timer6SpclCompA == (x)) ||     \
     (Timer6SpclCompB == (x)) ||     \
     (Timer6SpclCompC == (x)) ||     \
     (Timer6SpclCompD == (x)) ||     \
     (Timer6SpclCompE == (x)) ||     \
     (Timer6SpclCompF == (x)))
/*!< Parameter valid check for Count clock division */
#define IS_VALID_COUNT_CLK_DIV(x) \
    ((Timer6PclkDiv1 == (x)) ||   \
     (Timer6PclkDiv2 == (x)) ||   \
     (Timer6PclkDiv4 == (x)) ||   \
     (Timer6PclkDiv8 == (x)) ||   \
     (Timer6PclkDiv16 == (x)) ||  \
     (Timer6PclkDiv64 == (x)) ||  \
     (Timer6PclkDiv256 == (x)) || \
     (Timer6PclkDiv1024 == (x)))

/*!< Parameter valid check for count mode */
#define IS_VALID_COUNT_MODE(x)            \
    ((Timer6CntSawtoothMode == (x)) ||    \
     (Timer6CntTriangularModeA == (x)) || \
     (Timer6CntTriangularModeB == (x)))

/*!< Parameter valid check for count direction */
#define IS_VALID_COUNT_DIR(x)     \
    ((Timer6CntDirDown == (x)) || \
     (Timer6CntDirUp == (x)))

/*!< Parameter valid check for timer6 output port */
#define IS_VALID_TIMER6_OUTPUT_PORT(x) \
    ((Timer6PWMA == (x)) ||            \
     (Timer6PWMB == (x)))

/*!< Parameter valid check for timer6 input port */
#define IS_VALID_TIMER6_INPUT_PORT(x) \
    ((Timer6PWMA == (x)) ||           \
     (Timer6PWMB == (x)) ||           \
     (Timer6TrigA == (x)) ||          \
     (Timer6TrigB == (x)))

/*!< Parameter valid check for start/stop count output status */
#define IS_VALID_STA_STP_OUTPUT_STATUS(x) \
    ((Timer6PWMxPortOutLow == (x)) ||     \
     (Timer6PWMxPortOutHigh == (x)))

/*!< Parameter valid check for match output status */
#define IS_VALID_MATCH_OUTPUT_STATUS(x) \
    ((Timer6PWMxCompareLow == (x)) ||   \
     (Timer6PWMxCompareHigh == (x)) ||  \
     (Timer6PWMxCompareKeep == (x)) ||  \
     (Timer6PWMxCompareInv == (x)))

/*!< Parameter valid check for match output status */
#define IS_VALID_MATCH_OUTPUT_STATUS(x) \
    ((Timer6PWMxCompareLow == (x)) ||   \
     (Timer6PWMxCompareHigh == (x)) ||  \
     (Timer6PWMxCompareKeep == (x)) ||  \
     (Timer6PWMxCompareInv == (x)))

/*!< Parameter valid check for port filter clock */
#define IS_VALID_PORT_FILTER_CLOCK(x)   \
    ((Timer6FltClkPclk0Div1 == (x)) ||  \
     (Timer6FltClkPclk0Div4 == (x)) ||  \
     (Timer6FltClkPclk0Div16 == (x)) || \
     (Timer6FltClkPclk0Div64 == (x)))

/*!< Parameter valid check for interrupt request source */
#define IS_VALID_VPERR_PCNT_NUM(x) \
    ((Timer6PeriodCnts0 == (x)) || \
     (Timer6PeriodCnts1 == (x)) || \
     (Timer6PeriodCnts2 == (x)) || \
     (Timer6PeriodCnts3 == (x)) || \
     (Timer6PeriodCnts4 == (x)) || \
     (Timer6PeriodCnts5 == (x)) || \
     (Timer6PeriodCnts6 == (x)) || \
     (Timer6PeriodCnts7 == (x)))
/*!< Parameter valid check for interrupt request source */
#define IS_VALID_VPERR_PCNT_EN_SOURCE(x) \
    ((Timer6PeriodCnteDisable == (x)) || \
     (Timer6PeriodCnteMin == (x)) ||     \
     (Timer6PeriodCnteMax == (x)) ||     \
     (Timer6PeriodCnteBoth == (x)))

/*!< Parameter valid check for interrupt request source */
#define IS_VALID_IRQ_SOURCE(x)  \
    ((Timer6INTENA == (x)) ||   \
     (Timer6INTENB == (x)) ||   \
     (Timer6INTENC == (x)) ||   \
     (Timer6INTEND == (x)) ||   \
     (Timer6INTENE == (x)) ||   \
     (Timer6INTENF == (x)) ||   \
     (Timer6INTENOVF == (x)) || \
     (Timer6INTENUDF == (x)) || \
     (Timer6INTENDTE == (x)) || \
     (Timer6INTENSAU == (x)) || \
     (Timer6INTENSAD == (x)) || \
     (Timer6INTENSBU == (x)) || \
     (Timer6INTENSBD == (x)))

/*!< Parameter valid check for status type */
#define IS_VALID_STATUS_TYPE(x) \
    ((Timer6CMAF == (x)) ||     \
     (Timer6CMBF == (x)) ||     \
     (Timer6CMCF == (x)) ||     \
     (Timer6CMDF == (x)) ||     \
     (Timer6CMEF == (x)) ||     \
     (Timer6CMFF == (x)) ||     \
     (Timer6OVFF == (x)) ||     \
     (Timer6UDFF == (x)) ||     \
     (Timer6DTEF == (x)) ||     \
     (Timer6CMSAUF == (x)) ||   \
     (Timer6CMSADF == (x)) ||   \
     (Timer6CMSBUF == (x)) ||   \
     (Timer6CMSBDF == (x)) ||   \
     (Timer6VPERNUM == (x)) ||  \
     (Timer6DIRF == (x)))

/*!< Parameter valid check for hardware up count/down count event type */
#define IS_VALID_HW_COUNT_TYPE(x)            \
    ((Timer6HwCntPWMALowPWMBRise == (x)) ||  \
     (Timer6HwCntPWMALowPWMBFall == (x)) ||  \
     (Timer6HwCntPWMAHighPWMBRise == (x)) || \
     (Timer6HwCntPWMAHighPWMBFall == (x)) || \
     (Timer6HwCntPWMBLowPWMARise == (x)) ||  \
     (Timer6HwCntPWMBLowPWMAFall == (x)) ||  \
     (Timer6HwCntPWMBHighPWMARise == (x)) || \
     (Timer6HwCntPWMBHighPWMAFall == (x)) || \
     (Timer6HwCntTRIGARise == (x)) ||        \
     (Timer6HwCntTRIGAFall == (x)) ||        \
     (Timer6HwCntTRIGBRise == (x)) ||        \
     (Timer6HwCntTRIGBFall == (x)) ||        \
     (Timer6HwCntAos0 == (x)) ||             \
     (Timer6HwCntAos1 == (x)))

/*!< Parameter valid check for hardware up start/stop/clear/capture event type */
#define IS_VALID_HW_STA_STP_CLR_CAP_TYPE(x) \
    ((Timer6HwTrigAos0 == (x)) ||           \
     (Timer6HwTrigAos1 == (x)) ||           \
     (Timer6HwTrigPWMARise == (x)) ||       \
     (Timer6HwTrigPWMAFall == (x)) ||       \
     (Timer6HwTrigPWMBRise == (x)) ||       \
     (Timer6HwTrigPWMBFall == (x)) ||       \
     (Timer6HwTrigTimTriARise == (x)) ||    \
     (Timer6HwTrigTimTriAFall == (x)) ||    \
     (Timer6HwTrigTimTriBRise == (x)) ||    \
     (Timer6HwTrigTimTriBFall == (x)) ||    \
     (Timer6HwTrigEnd == (x)))

/*!< Parameter valid check for timer6 input port type */
#define IS_VALID_INPUT_PORT_TYPE(x) \
    ((Timer6xCHA == (x)) ||         \
     (Timer6xCHB == (x)) ||         \
     (Timer6TrigA == (x)) ||        \
     (Timer6TrigB == (x)))

/*!< Parameter valid check for GenCMP and period register buffer transfer type*/
#define IS_VALID_GCMP_PRD_BUF_TYPE(x)   \
    ((Timer6GcmpPrdSingleBuf == (x)) || \
     (Timer6GcmpPrdDoubleBuf == (x)))

/*!< Parameter valid check for special compare register buffer transfer type */
#define IS_VALID_SPCL_BUF_TYPE(x)    \
    ((Timer6SpclSingleBuf == (x)) || \
     (Timer6SpclDoubleBuf == (x)))

/*!< Parameter valid check for spcl register transfer opportunity type */
#define IS_VALID_SPCL_TRANS_OPT_TYPE(x) \
    ((Timer6SplcOptNone == (x)) ||      \
     (Timer6SplcOptOverFlow == (x)) ||  \
     (Timer6SplcOptUnderFlow == (x)) || \
     (Timer6SplcOptBoth == (x)))

/*!< Parameter valid check for dead time register type */
#define IS_VALID_DEAD_TIME_TYPE(x)  \
    ((Timer6DeadTimUpAR == (x)) ||  \
     (Timer6DeadTimUpBR == (x)) ||  \
     (Timer6DeadTimDwnAR == (x)) || \
     (Timer6DeadTimDwnBR == (x)))

/*!< Parameter valid check for Z Phase input mask periods */
#define IS_VALID_ZPHASE_MASK_PRD(x) \
    ((Timer6ZMaskDis == (x)) ||     \
     (Timer6ZMask4Cyl == (x)) ||    \
     (Timer6ZMask8Cyl == (x)) ||    \
     (Tiemr6ZMask16Cyl == (x)))

/*!< Parameter valid check for event source */
#define IS_VALID_EVENT_SOURCE(x) ((x) <= 511)

/*! TimerA registers reset value */
#define TIMERA_REG_CNTER_RESET_VALUE (0x0000u)
#define TIMERA_REG_GCONR_RESET_VALUE (0x00000100ul)
#define TIMERA_REG_ICONR_RESET_VALUE (0x00000000ul)
#define TIMERA_REG_PCONR_RESET_VALUE (0x00000000ul)
#define TIMERA_REG_BCONR_RESET_VALUE (0x00000000ul)
#define TIMERA_REG_DCONR_RESET_VALUE (0x00000000ul)
#define TIMERA_REG_FCONR_RESET_VALUE (0x00000000ul)
#define TIMERA_REG_VPERR_RESET_VALUE (0x00000000ul)

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
 * \brief   Timer6 interrupt request enable or disable
 *
 * \param   [in]  TMR6x            Timer6 unit
 * \param   [in]  enTimer6Irq      Irq type
 * \param   [in]  bEn              true/false
 *
 * \retval  Ok:  config successfully
 *
 ******************************************************************************/
en_result_t Timer6_ConfigIrq(M4_TMR6_TypeDef *TMR6x, en_timer6_irq_type_t enTimer6Irq, bool bEn)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_IRQ_SOURCE(enTimer6Irq));

    switch (enTimer6Irq)
    {
    case Timer6INTENA:
        TMR6x->ICONR_f.INTENA = bEn;
        break;
    case Timer6INTENB:
        TMR6x->ICONR_f.INTENB = bEn;
        break;
    case Timer6INTENC:
        TMR6x->ICONR_f.INTENC = bEn;
        break;
    case Timer6INTEND:
        TMR6x->ICONR_f.INTEND = bEn;
        break;
    case Timer6INTENE:
        TMR6x->ICONR_f.INTENE = bEn;
        break;
    case Timer6INTENF:
        TMR6x->ICONR_f.INTENF = bEn;
        break;
    case Timer6INTENOVF:
        TMR6x->ICONR_f.INTENOVF = bEn;
        break;
    case Timer6INTENUDF:
        TMR6x->ICONR_f.INTENUDF = bEn;
        break;
    case Timer6INTENDTE:
        TMR6x->ICONR_f.INTENDTE = bEn;
        break;
    case Timer6INTENSAU:
        TMR6x->ICONR_f.INTENSAU = bEn;
        break;
    case Timer6INTENSAD:
        TMR6x->ICONR_f.INTENSAD = bEn;
        break;
    case Timer6INTENSBU:
        TMR6x->ICONR_f.INTENSBU = bEn;
        break;
    case Timer6INTENSBD:
        TMR6x->ICONR_f.INTENSBD = bEn;
        break;
    default:
        break;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get Timer6 status flag
 **
 ** \param [in] TMR6x            Timer6 unit
 **
 ** \param [in] enStatus         Timer6 status type
 **
 ** \retval Timer6 status
 **
 ******************************************************************************/
uint8_t Timer6_GetStatus(M4_TMR6_TypeDef *TMR6x, en_timer6_status_t enStatus)
{
    uint8_t status = 0u;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_STATUS_TYPE(enStatus));

    switch (enStatus)
    {
    case Timer6CMAF:
        status = (uint8_t)TMR6x->STFLR_f.CMAF;
        break;
    case Timer6CMBF:
        status = (uint8_t)TMR6x->STFLR_f.CMBF;
        break;
    case Timer6CMCF:
        status = (uint8_t)TMR6x->STFLR_f.CMCF;
        break;
    case Timer6CMDF:
        status = (uint8_t)TMR6x->STFLR_f.CMDF;
        break;
    case Timer6CMEF:
        status = (uint8_t)TMR6x->STFLR_f.CMEF;
        break;
    case Timer6CMFF:
        status = (uint8_t)TMR6x->STFLR_f.CMFF;
        break;
    case Timer6OVFF:
        status = (uint8_t)TMR6x->STFLR_f.OVFF;
        break;
    case Timer6UDFF:
        status = (uint8_t)TMR6x->STFLR_f.UDFF;
        break;
    case Timer6DTEF:
        status = (uint8_t)TMR6x->STFLR_f.DTEF;
        break;
    case Timer6CMSAUF:
        status = (uint8_t)TMR6x->STFLR_f.CMSAUF;
        break;
    case Timer6CMSADF:
        status = (uint8_t)TMR6x->STFLR_f.CMSADF;
        break;
    case Timer6CMSBUF:
        status = (uint8_t)TMR6x->STFLR_f.CMSBUF;
        break;
    case Timer6CMSBDF:
        status = (uint8_t)TMR6x->STFLR_f.CMSBDF;
        break;
    case Timer6VPERNUM:
        status = (uint8_t)TMR6x->STFLR_f.VPERNUM;
        break;
    case Timer6DIRF:
        status = (uint8_t)TMR6x->STFLR_f.DIRF;
        break;
    default:
        break;
    }

    return status;
}

/**
 *******************************************************************************
 ** \brief De-Initialize Timer6 unit
 **
 ** \param [in] TMR6x        Timer6 unit
 **
 ** \retval Ok               Process successfully done
 **
 ******************************************************************************/
en_result_t Timer6_DeInit(M4_TMR6_TypeDef *TMR6x)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->CNTER = TIMERA_REG_CNTER_RESET_VALUE;
    TMR6x->GCONR = TIMERA_REG_GCONR_RESET_VALUE;
    TMR6x->PCONR = TIMERA_REG_PCONR_RESET_VALUE;
    TMR6x->ICONR = TIMERA_REG_ICONR_RESET_VALUE;
    TMR6x->BCONR = TIMERA_REG_BCONR_RESET_VALUE;
    TMR6x->DCONR = TIMERA_REG_DCONR_RESET_VALUE;
    TMR6x->FCONR = TIMERA_REG_FCONR_RESET_VALUE;
    TMR6x->VPERR = TIMERA_REG_VPERR_RESET_VALUE;
    TMR6x->HSTAR = 0x00000000ul;
    TMR6x->HSTPR = 0x00000000ul;
    TMR6x->HCLRR = 0x00000000ul;
    TMR6x->HCPAR = 0x00000000ul;
    TMR6x->HCPBR = 0x00000000ul;
    TMR6x->HCUPR = 0x00000000ul;
    TMR6x->HCDOR = 0x00000000ul;

    return Ok;
}

/*******************************************************************************
 * \brief   Timer6 Base Config
 *
 *
 * \param   [in]  TMR6x                   Timer6 unit
 * \param   [in]  pstcTimer6BaseCntCfg    Bsee Config Pointer
 *
 * \retval  Ok:  Config Successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_Init(M4_TMR6_TypeDef *TMR6x, const stc_timer6_basecnt_cfg_t *pstcTimer6BaseCntCfg)
{
    en_result_t enRet = Ok;

    if (NULL == pstcTimer6BaseCntCfg)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
        DDL_ASSERT(IS_VALID_COUNT_MODE(pstcTimer6BaseCntCfg->enCntMode));
        DDL_ASSERT(IS_VALID_COUNT_DIR(pstcTimer6BaseCntCfg->enCntDir));
        DDL_ASSERT(IS_VALID_COUNT_CLK_DIV(pstcTimer6BaseCntCfg->enCntClkDiv));

        TMR6x->GCONR_f.MODE = pstcTimer6BaseCntCfg->enCntMode;
        TMR6x->GCONR_f.DIR = pstcTimer6BaseCntCfg->enCntDir;
        TMR6x->GCONR_f.CKDIV = pstcTimer6BaseCntCfg->enCntClkDiv;
    }
    return enRet;
}

/*******************************************************************************
 * \brief   Timer6 Unit Start Count
 *
 *
 * \param   [in]  TMR6x            Timer6 unit
 *
 * \retval  Ok:  Config Successfully
 *
 ******************************************************************************/
en_result_t Timer6_StartCount(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->GCONR_f.START = 1ul;

    return Ok;
}

/*******************************************************************************
 * \brief   TImer6 Unit Stop Count
 *
 *
 * \param   [in]  TMR6x       Timer6 unit
 *
 * \retval  Ok:  Config Successfully
 *
 ******************************************************************************/
en_result_t Timer6_StopCount(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->GCONR_f.START = 0ul;

    return Ok;
}

/*******************************************************************************
 * \brief   Timer6 Unit Set Count Value
 *
 *
 * \param   [in]  TMR6x             Timer6 unit
 * \param   [in]  u16Value          Count Value
 *
 * \retval  Ok:  Config Successfully
 *
 ******************************************************************************/
en_result_t Timer6_SetCount(M4_TMR6_TypeDef *TMR6x, uint16_t u16Value)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->CNTER_f.CNT = u16Value;

    return Ok;
}

/*******************************************************************************
 * \brief   Timer6 Unit Get Count Value
 *
 *
 * \param   [in]  TMR6x           Timer6 unit
 * \param   [in]  u16Value        Count Value
 *
 * \retval  Ok:  Config Successfully
 *
 ******************************************************************************/
uint16_t Timer6_GetCount(M4_TMR6_TypeDef *TMR6x)
{
    uint16_t u16Value;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    u16Value = (uint16_t)TMR6x->CNTER_f.CNT;

    return u16Value;
}

/*******************************************************************************
 * \brief   Timer6 Unit Clear Count Value
 *
 *
 * \param   [in]  TMR6x      Timer6 unit
 *
 *
 * \retval  Ok:  Set Successfully
 *
 ******************************************************************************/
en_result_t Timer6_ClearCount(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->CNTER_f.CNT = 0ul;

    return Ok;
}

/*******************************************************************************
 * \brief   Timer6 unit set count period and buffer value
 *
 *
 * \param   [in]  TMR6x              Timer6 unit
 * \param   [in]  enTimer6Periodx    Period register name
 * \param   [in]  u16Period          Count period value
 *
 * \retval  Ok:  Set Successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_SetPeriod(M4_TMR6_TypeDef *TMR6x, en_timer6_period_t enTimer6Periodx, uint16_t u16Period)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_PERIOD_TYPE(enTimer6Periodx));

    switch (enTimer6Periodx)
    {
    case Timer6PeriodA:
        TMR6x->PERAR = u16Period;
        break;
    case Timer6PeriodB:
        TMR6x->PERBR = u16Period;
        break;
    case Timer6PeriodC:
        TMR6x->PERCR = u16Period;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/*******************************************************************************
 * \brief   Timer6 unit Set General Compare Register Value(for PWM output)
 *
 *
 * \param   [in]  TMR6x              Timer6 unit
 * \param   [in]  enTimer6Compare    General Compare Register name
 * \param   [in]  u16Compare         General Compare Register value
 *
 * \retval  Ok:  Set Successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_SetGeneralCmpValue(M4_TMR6_TypeDef *TMR6x, en_timer6_compare_t enTimer6Compare, uint16_t u16Compare)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_GEN_CMP_TYPE(enTimer6Compare));

    switch (enTimer6Compare)
    {
    case Timer6GenCompareA:
        TMR6x->GCMAR = u16Compare;
        break;
    case Timer6GenCompareB:
        TMR6x->GCMBR = u16Compare;
        break;
    case Timer6GenCompareC:
        TMR6x->GCMCR = u16Compare;
        break;
    case Timer6GenCompareD:
        TMR6x->GCMDR = u16Compare;
        break;
    case Timer6GenCompareE:
        TMR6x->GCMER = u16Compare;
        break;
    case Timer6GenCompareF:
        TMR6x->GCMFR = u16Compare;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/*******************************************************************************
 * \brief   Timer6 unit Set Special Compare Register Value
 *
 *
 * \param   [in]  TMR6x              Timer6 unit
 * \param   [in]  enTimer6SpclCmp    General Compare Register name
 * \param   [in]  u16SpclCmp         General Compare Register value
 *
 * \retval  Ok:  Set Successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_SetSpecialCmpValue(M4_TMR6_TypeDef *TMR6x, en_timer6_special_compare_t enTimer6SpclCmp, uint16_t u16SpclCmp)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_SPECIAL_CMP_TYPE(enTimer6SpclCmp));

    switch (enTimer6SpclCmp)
    {
    case Timer6SpclCompA:
        TMR6x->SCMAR = u16SpclCmp;
        break;
    case Timer6SpclCompB:
        TMR6x->SCMBR = u16SpclCmp;
        break;
    case Timer6SpclCompC:
        TMR6x->SCMCR = u16SpclCmp;
        break;
    case Timer6SpclCompD:
        TMR6x->SCMDR = u16SpclCmp;
        break;
    case Timer6SpclCompE:
        TMR6x->SCMER = u16SpclCmp;
        break;
    case Timer6SpclCompF:
        TMR6x->SCMFR = u16SpclCmp;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/*******************************************************************************
 * \brief   Timer6 config general compare buffer transfer function
 *
 *
 * \param   [in]  TMR6x                  Timer6 unit
 * \param   [in]  enTimer6PWMPort        PWM channel of timer6
 * \param   [in]  pstcTimer6GenBufCfg    General Compare Register Buffer Transfer Type Pointer
 *
 * \retval  Ok:  Set Successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_SetGeneralBuf(M4_TMR6_TypeDef *TMR6x, en_timer6_chx_port_t enTimer6PWMPort, const stc_timer6_gcmp_buf_cfg_t *pstcTimer6GenBufCfg)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_TIMER6_OUTPUT_PORT(enTimer6PWMPort));
    DDL_ASSERT(IS_VALID_GCMP_PRD_BUF_TYPE(pstcTimer6GenBufCfg->enGcmpBufTransType));

    switch (enTimer6PWMPort)
    {
    case Timer6PWMA:
        TMR6x->BCONR_f.BENA = pstcTimer6GenBufCfg->bEnGcmpTransBuf;
        TMR6x->BCONR_f.BSEA = pstcTimer6GenBufCfg->enGcmpBufTransType;
        break;
    case Timer6PWMB:
        TMR6x->BCONR_f.BENB = pstcTimer6GenBufCfg->bEnGcmpTransBuf;
        TMR6x->BCONR_f.BSEB = pstcTimer6GenBufCfg->enGcmpBufTransType;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/*******************************************************************************
 * \brief   Timer6 config special compare buffer transfer function
 *
 *
 * \param   [in]  TMR6x                   Timer6 unit
 * \param   [in]  enTimer6SpclCmp         Special Compare Register nameunit
 * \param   [in]  pstcTimer6SpclBufCfg    Special Compare Register Buffer Transfer Type Pointer
 *
 * \retval  Ok:  Set Successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_SetSpecialBuf(M4_TMR6_TypeDef *TMR6x, en_timer6_special_compare_t enTimer6SpclCmp, const stc_timer6_spcl_buf_cfg_t *pstcTimer6SpclBufCfg)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_SPECIAL_CMP_TYPE(enTimer6SpclCmp));
    DDL_ASSERT(IS_VALID_SPCL_BUF_TYPE(pstcTimer6SpclBufCfg->enSpclBufTransType));
    DDL_ASSERT(IS_VALID_SPCL_TRANS_OPT_TYPE(pstcTimer6SpclBufCfg->enSpclBufOptType));

    switch (enTimer6SpclCmp)
    {
    case Timer6SpclCompA:
        TMR6x->BCONR_f.BENSPA = pstcTimer6SpclBufCfg->bEnSpclTransBuf;
        TMR6x->BCONR_f.BSESPA = pstcTimer6SpclBufCfg->enSpclBufTransType;
        TMR6x->BCONR_f.BTRSPA = pstcTimer6SpclBufCfg->enSpclBufOptType;
        break;
    case Timer6SpclCompB:
        TMR6x->BCONR_f.BENSPB = pstcTimer6SpclBufCfg->bEnSpclTransBuf;
        TMR6x->BCONR_f.BSESPB = pstcTimer6SpclBufCfg->enSpclBufTransType;
        TMR6x->BCONR_f.BTRSPB = pstcTimer6SpclBufCfg->enSpclBufOptType;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/*******************************************************************************
 * \brief   Timer6 config period buffer transfer function
 *
 *
 * \param   [in]  TMR6x                  Timer6 unit
 * \param   [in]  pstcTimer6PrdBufCfg    Period Register Buffer Transfer Type Pointer
 *
 * \retval  Ok:  Set Successfully
 *
 ******************************************************************************/
en_result_t Timer6_SetPeriodBuf(M4_TMR6_TypeDef *TMR6x, const stc_timer6_period_buf_cfg_t *pstcTimer6PrdBufCfg)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_GCMP_PRD_BUF_TYPE(pstcTimer6PrdBufCfg->enPeriodBufTransType));

    TMR6x->BCONR_f.BENP = pstcTimer6PrdBufCfg->bEnPeriodTransBuf;
    TMR6x->BCONR_f.BSEP = pstcTimer6PrdBufCfg->enPeriodBufTransType;

    return Ok;
}

/*******************************************************************************
 * \brief   Timer6 unit get General Compare Register Value(for PWM output)
 *
 *
 * \param   [in]  TMR6x              Timer6 unit
 * \param   [in]  enTimer6Compare    General Compare Register name
 *
 *
 * \retval  u16TempValue:  General Compare Register value
 *
 ******************************************************************************/
uint16_t Timer6_GetGeneralCmpValue(M4_TMR6_TypeDef *TMR6x, en_timer6_compare_t enTimer6Compare)
{
    uint16_t u16TempValue = 0u;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_GEN_CMP_TYPE(enTimer6Compare));

    switch (enTimer6Compare)
    {
    case Timer6GenCompareA:
        u16TempValue = (uint16_t)TMR6x->GCMAR;
        break;
    case Timer6GenCompareB:
        u16TempValue = (uint16_t)TMR6x->GCMBR;
        break;
    case Timer6GenCompareC:
        u16TempValue = (uint16_t)TMR6x->GCMCR;
        break;
    case Timer6GenCompareD:
        u16TempValue = (uint16_t)TMR6x->GCMDR;
        break;
    case Timer6GenCompareE:
        u16TempValue = (uint16_t)TMR6x->GCMER;
        break;
    case Timer6GenCompareF:
        u16TempValue = (uint16_t)TMR6x->GCMFR;
        break;
    default:
        break;
    }

    return u16TempValue;
}

/***********************************************************************
 * \brief   Timer6 Config valid count period
 *
 *
 * \param   [in]  TMR6x             Timer6 unit
 * \param   [in]  pstcTimer6ValidPerCfg    Valid Count Period Pointer
 *
 * \retval  Ok:  Config successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ***********************************************************************/
en_result_t Timer6_SetValidPeriod(M4_TMR6_TypeDef *TMR6x, const stc_timer6_validper_cfg_t *pstcTimer6ValidPerCfg)
{
    en_result_t enRet = Ok;

    if (NULL == pstcTimer6ValidPerCfg)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
        DDL_ASSERT(IS_VALID_VPERR_PCNT_EN_SOURCE(pstcTimer6ValidPerCfg->enValidCdtEn));
        DDL_ASSERT(IS_VALID_VPERR_PCNT_NUM(pstcTimer6ValidPerCfg->enValidCntNum));

        TMR6x->VPERR_f.PCNTS = pstcTimer6ValidPerCfg->enValidCntNum;
        TMR6x->VPERR_f.PCNTE = pstcTimer6ValidPerCfg->enValidCdtEn;
        TMR6x->VPERR_f.SPPERIA = pstcTimer6ValidPerCfg->bPeriodSCMA;
        TMR6x->VPERR_f.SPPERIB = pstcTimer6ValidPerCfg->bPeriodSCMB;
    }
    return enRet;
}

/*******************************************************************************
 * \brief   Port input config(Trig)
 *
 *
 * \param   [in]  TMR6x             Timer6 unit
 * \param   [in]  pstcTimer6PortInputCfg   Point Input Config Pointer
 *
 * \retval  Ok:  Set successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_PortInputConfig(M4_TMR6_TypeDef *TMR6x, const stc_timer6_port_input_cfg_t *pstcTimer6PortInputCfg)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    if (NULL == pstcTimer6PortInputCfg)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        switch (pstcTimer6PortInputCfg->enPortSel)
        {
        case Timer6xCHA:
            TMR6x->PCONR_f.CAPMDA = pstcTimer6PortInputCfg->enPortMode;
            TMR6x->FCONR_f.NOFIENGA = pstcTimer6PortInputCfg->bFltEn;
            TMR6x->FCONR_f.NOFICKGA = pstcTimer6PortInputCfg->enFltClk;
            break;

        case Timer6xCHB:
            TMR6x->PCONR_f.CAPMDB = pstcTimer6PortInputCfg->enPortMode;
            TMR6x->FCONR_f.NOFIENGB = pstcTimer6PortInputCfg->bFltEn;
            TMR6x->FCONR_f.NOFICKGB = pstcTimer6PortInputCfg->enFltClk;
            break;

        case Timer6TrigA:
            TMR6x->FCONR_f.NOFIENTA = pstcTimer6PortInputCfg->bFltEn;
            TMR6x->FCONR_f.NOFICKTA = pstcTimer6PortInputCfg->enFltClk;
            break;

        case Timer6TrigB:
            TMR6x->FCONR_f.NOFIENTB = pstcTimer6PortInputCfg->bFltEn;
            TMR6x->FCONR_f.NOFICKTB = pstcTimer6PortInputCfg->enFltClk;
            break;

        default:
            enRet = ErrorInvalidParameter;
            break;
        }
    }
    return enRet;
}

/*******************************************************************************
 * \brief   Timer6 Output Port config
 *
 *
 * \param   [in]  TMR6x                    Timer6 unit
 * \param   [in]  enTimer6PWMPort          Timer6 Port(PWMA/PWMB)
 * \param   [in]  pstcTimer6PortOutCfg     timer6 Port Config Pointer
 *
 * \retval  Ok:  Set successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_PortOutputConfig(M4_TMR6_TypeDef *TMR6x,
                                    en_timer6_chx_port_t enTimer6PWMPort,
                                    const stc_timer6_port_output_cfg_t *pstcTimer6PortOutCfg)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_TIMER6_OUTPUT_PORT(enTimer6PWMPort));

    if (NULL == pstcTimer6PortOutCfg)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        switch (enTimer6PWMPort)
        {
        case Timer6PWMA:
            TMR6x->PCONR_f.CAPMDA = pstcTimer6PortOutCfg->enPortMode;
            TMR6x->PCONR_f.STACA = pstcTimer6PortOutCfg->enStaOut;
            TMR6x->PCONR_f.STPCA = pstcTimer6PortOutCfg->enStpOut;
            TMR6x->PCONR_f.STASTPSA = pstcTimer6PortOutCfg->enStaStp;
            TMR6x->PCONR_f.CMPCA = pstcTimer6PortOutCfg->enCmpc;
            TMR6x->PCONR_f.PERCA = pstcTimer6PortOutCfg->enPerc;
            TMR6x->PCONR_f.OUTENA = pstcTimer6PortOutCfg->bOutEn;
            TMR6x->PCONR_f.EMBVALA = pstcTimer6PortOutCfg->enDisVal;
            break;

        case Timer6PWMB:
            TMR6x->PCONR_f.CAPMDB = pstcTimer6PortOutCfg->enPortMode;
            TMR6x->PCONR_f.STACB = pstcTimer6PortOutCfg->enStaOut;
            TMR6x->PCONR_f.STPCB = pstcTimer6PortOutCfg->enStpOut;
            TMR6x->PCONR_f.STASTPSB = pstcTimer6PortOutCfg->enStaStp;
            TMR6x->PCONR_f.CMPCB = pstcTimer6PortOutCfg->enCmpc;
            TMR6x->PCONR_f.PERCB = pstcTimer6PortOutCfg->enPerc;
            TMR6x->PCONR_f.OUTENB = pstcTimer6PortOutCfg->bOutEn;
            TMR6x->PCONR_f.EMBVALB = pstcTimer6PortOutCfg->enDisVal;
            break;

        default:
            enRet = ErrorInvalidParameter;
            break;
        }
    }
    return enRet;
}

/*******************************************************************************
 * \brief   Timer6 unit Set DeadTime Register Value(for PWM output)
 *
 *
 * \param   [in]  TMR6x              Timer6 unit
 * \param   [in]  enTimer6DTReg      DeadTime Register name
 * \param   [in]  u16DTValue         DeadTime Register value
 *
 * \retval  Ok:  Set Successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_SetDeadTimeValue(M4_TMR6_TypeDef *TMR6x, en_timer6_dead_time_reg_t enTimer6DTReg, uint16_t u16DTValue)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_DEAD_TIME_TYPE(enTimer6DTReg));

    switch (enTimer6DTReg)
    {
    case Timer6DeadTimUpAR:
        TMR6x->DTUAR = u16DTValue;
        break;
    case Timer6DeadTimUpBR:
        TMR6x->DTUBR = u16DTValue;
        break;
    case Timer6DeadTimDwnAR:
        TMR6x->DTDAR = u16DTValue;
        break;
    case Timer6DeadTimDwnBR:
        TMR6x->DTDBR = u16DTValue;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/*******************************************************************************
 * \brief   Config DeadTime function
 *
 *
 * \param   [in]  TMR6x               Timer6 unit
 * \param   [in]  pstcTimer6DTCfg     Timer6 dead time config pointer
 *
 * \retval  Ok:  Set Successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_ConfigDeadTime(M4_TMR6_TypeDef *TMR6x, const stc_timer6_deadtime_cfg_t *pstcTimer6DTCfg)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    if (NULL == pstcTimer6DTCfg)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        TMR6x->DCONR_f.SEPA = pstcTimer6DTCfg->bEnDtEqualUpDwn;
        TMR6x->DCONR_f.DTBENU = pstcTimer6DTCfg->bEnDtBufUp;
        TMR6x->DCONR_f.DTBEND = pstcTimer6DTCfg->bEnDtBufDwn;
        TMR6x->DCONR_f.DTCEN = pstcTimer6DTCfg->bEnDeadtime;
    }
    return enRet;
}

/*******************************************************************************
 * \brief   Config Software Synchrony Start
 *
 *
 * \param   [in]  pstcTimer6SwSyncStart    Software Synchrony Start Pointer
 *
 * \retval  Ok:  Set successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_SwSyncStart(const stc_timer6_sw_sync_t *pstcTimer6SwSyncStart)
{
    en_result_t enRet = Ok;
    uint32_t u32Val = 0ul;

    if (NULL == pstcTimer6SwSyncStart)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        if (pstcTimer6SwSyncStart->bTimer61)
        {
            u32Val |= 0x1ul;
        }
        if (pstcTimer6SwSyncStart->bTimer62)
        {
            u32Val |= 0x2ul;
        }
        if (pstcTimer6SwSyncStart->bTimer63)
        {
            u32Val |= 0x4ul;
        }

        M4_TMR6_CR->SSTAR = u32Val;
    }
    return enRet;
}

/*******************************************************************************
 * \brief   Config Software Synchrony Stop
 *
 *
 * \param   [in]  pstcTimer6SwSyncStop    Software Synchrony Stop Pointer
 *
 * \retval  Ok:  Set successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_SwSyncStop(const stc_timer6_sw_sync_t *pstcTimer6SwSyncStop)
{
    en_result_t enRet = Ok;
    uint32_t u32Val = 0ul;

    if (NULL == pstcTimer6SwSyncStop)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        if (pstcTimer6SwSyncStop->bTimer61)
        {
            u32Val |= 0x1ul;
        }
        if (pstcTimer6SwSyncStop->bTimer62)
        {
            u32Val |= 0x2ul;
        }
        if (pstcTimer6SwSyncStop->bTimer63)
        {
            u32Val |= 0x4ul;
        }

        M4_TMR6_CR->SSTPR = u32Val;
    }
    return enRet;
}

/*******************************************************************************
 * \brief   Config Software Synchrony Clear
 *
 *
 * \param   [in]  pstcTimer6SwSyncClear    Software Synchrony Clear Pointer
 *
 * \retval  Ok:  Set successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_SwSyncClear(const stc_timer6_sw_sync_t *pstcTimer6SwSyncClear)
{
    en_result_t enRet = Ok;
    uint32_t u32Val = 0ul;

    if (NULL == pstcTimer6SwSyncClear)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        if (pstcTimer6SwSyncClear->bTimer61)
        {
            u32Val |= 0x1ul;
        }
        if (pstcTimer6SwSyncClear->bTimer62)
        {
            u32Val |= 0x2ul;
        }
        if (pstcTimer6SwSyncClear->bTimer63)
        {
            u32Val |= 0x4ul;
        }

        M4_TMR6_CR->SCLRR = u32Val;
    }
    return enRet;
}

/*******************************************************************************
 * \brief   Get Software Synchrony status
 *
 *
 * \param   [in]  pstcTimer6SwSyncState    Software Synchrony State Pointer
 *
 * \retval  Ok:  Set successfully
 * \retval  ErrorInvalidParameter: Provided parameter is not valid
 *
 ******************************************************************************/
en_result_t Timer6_GetSwSyncState(stc_timer6_sw_sync_t *pstcTimer6SwSyncState)
{
    en_result_t enRet = Ok;

    if (NULL == pstcTimer6SwSyncState)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        if (M4_TMR6_CR->SSTAR & 0x1ul)
        {
            pstcTimer6SwSyncState->bTimer61 = true;
        }
        else
        {
            pstcTimer6SwSyncState->bTimer61 = false;
        }
        if (M4_TMR6_CR->SSTAR & 0x2ul)
        {
            pstcTimer6SwSyncState->bTimer62 = true;
        }
        else
        {
            pstcTimer6SwSyncState->bTimer62 = false;
        }
        if (M4_TMR6_CR->SSTAR & 0x4ul)
        {
            pstcTimer6SwSyncState->bTimer63 = true;
        }
        else
        {
            pstcTimer6SwSyncState->bTimer63 = false;
        }
    }

    return enRet;
}

/*******************************************************************************
 * \brief   Timer6 Hardware UpCount Event config
 *
 * \param   [in]  TMR6x            Timer6 unit
 * \param   [in]  enTimer6HwCntUp  Hardware UpCount Event
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ConfigHwCntUp(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_cnt_t enTimer6HwCntUp)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_HW_COUNT_TYPE(enTimer6HwCntUp));

    u32Val = TMR6x->HCUPR;
    TMR6x->HCUPR = u32Val | (1ul << enTimer6HwCntUp);

    return Ok;
}

/**************************************************************
 * \brief   Clear Timer6 Hardware UpCount Event
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ************************************************************/
en_result_t Timer6_ClearHwCntUp(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->HCUPR = 0ul;

    return Ok;
}

/*******************************************************************************
 * \brief   Set Timer6 Hardware DownCount Event
 *
 * \param   [in]  TMR6x              Timer6 unit
 * \param   [in]  enTimer6HwCntDwn   Hardware DownCount Event
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ConfigHwCntDwn(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_cnt_t enTimer6HwCntDwn)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_HW_COUNT_TYPE(enTimer6HwCntDwn));

    u32Val = TMR6x->HCDOR;
    TMR6x->HCDOR = u32Val | (1ul << enTimer6HwCntDwn);

    return Ok;
}

/*******************************************************************************
 * \brief   Clear Timer6 Hardware DownCount Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ClearHwCntDwn(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->HCDOR = 0ul;

    return Ok;
}

/*******************************************************************************
 * \brief   Config Hardware Start Event
 *
 *
 * \param   [in]  TMR6x              Timer6 unit
 * \param   [in]  enTimer6HwStart    Hardware Start Event
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ConfigHwStart(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwStart)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_HW_STA_STP_CLR_CAP_TYPE(enTimer6HwStart));

    u32Val = TMR6x->HSTAR;
    TMR6x->HSTAR = u32Val | (1ul << enTimer6HwStart);

    return Ok;
}

/*******************************************************************************
 * \brief   Clear Hardware Start Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ClearHwStart(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->HSTAR = 0ul;

    return Ok;
}

/*******************************************************************************
 * \brief   Enable Hardware Start Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_EnableHwStart(M4_TMR6_TypeDef *TMR6x)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    u32Val = TMR6x->HSTAR;
    TMR6x->HSTAR = u32Val | (1ul << 31u);

    return Ok;
}

/*******************************************************************************
 * \brief   Disable Hardware Start Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_DisableHwStart(M4_TMR6_TypeDef *TMR6x)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    u32Val = TMR6x->HSTAR;
    TMR6x->HSTAR = u32Val & 0x7FFFFFFFul;

    return Ok;
}

/*******************************************************************************
 * \brief   Config Hardware Stop Event
 *
 *
 * \param   [in]  TMR6x              Timer6 unit
 * \param   [in]  enTimer6HwStop     Hardware Stop Event
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ConfigHwStop(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwStop)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_HW_STA_STP_CLR_CAP_TYPE(enTimer6HwStop));

    u32Val = TMR6x->HSTPR;
    TMR6x->HSTPR = u32Val | (1ul << enTimer6HwStop);

    return Ok;
}

/*******************************************************************************
 * \brief   Clear Hardware Stop Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ClearHwStop(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->HSTPR = 0ul;
    return Ok;
}

/*******************************************************************************
 * \brief   Enable Hardware Stop Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_EnableHwStop(M4_TMR6_TypeDef *TMR6x)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    u32Val = TMR6x->HSTPR;
    TMR6x->HSTPR = u32Val | (1ul << 31u);
    return Ok;
}

/*******************************************************************************
 * \brief   Disable Hardware Stop Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_DisableHwStop(M4_TMR6_TypeDef *TMR6x)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    u32Val = TMR6x->HSTPR;
    TMR6x->HSTPR = u32Val & 0x7FFFFFFFul;

    return Ok;
}

/*******************************************************************************
 * \brief   Config Hardware Clear Event
 *
 *
 * \param   [in]  TMR6x              Timer6 unit
 * \param   [in]  enTimer6HwClear    Hardware Clear Event
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ConfigHwClear(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwClear)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_HW_STA_STP_CLR_CAP_TYPE(enTimer6HwClear));

    u32Val = TMR6x->HCLRR;
    TMR6x->HCLRR = u32Val | (1ul << enTimer6HwClear);

    return Ok;
}

/*******************************************************************************
 * \brief   Clear Hardware Clear Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ClearHwClear(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->HCLRR = 0ul;

    return Ok;
}

/*******************************************************************************
 * \brief   Enable Hardware Clear Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_EnableHwClear(M4_TMR6_TypeDef *TMR6x)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    u32Val = TMR6x->HCLRR;
    TMR6x->HCLRR = u32Val | (1ul << 31u);

    return Ok;
}

/*******************************************************************************
 * \brief   Disable Hardware Clear Event
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_DisableHwClear(M4_TMR6_TypeDef *TMR6x)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    u32Val = TMR6x->HCLRR;
    TMR6x->HCLRR = u32Val & 0x7FFFFFFFul;

    return Ok;
}

/*******************************************************************************
 * \brief   Config Hardware Capture Event A
 *
 *
 * \param   [in]  TMR6x                Timer6 unit
 * \param   [in]  enTimer6HwCaptureA   Hardware capture event A selection
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ConfigHwCaptureA(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwCaptureA)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_HW_STA_STP_CLR_CAP_TYPE(enTimer6HwCaptureA));

    u32Val = TMR6x->HCPAR;
    TMR6x->HCPAR = u32Val | (1ul << enTimer6HwCaptureA);
    // TMR6x->PCONR_f.CAPMDA = 1;

    return Ok;
}

/*******************************************************************************
 * \brief   Clear Hardware Capture Event A
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ClearHwCaptureA(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->HCPAR = 0ul;

    return Ok;
}

/*******************************************************************************
 * \brief   Config Hardware Capture Event B
 *
 *
 * \param   [in]  TMR6x                Timer6 unit
 * \param   [in]  enTimer6HwCaptureB   Hardware capture event B selection
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ConfigHwCaptureB(M4_TMR6_TypeDef *TMR6x, en_timer6_hw_trig_t enTimer6HwCaptureB)
{
    uint32_t u32Val;

    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_HW_STA_STP_CLR_CAP_TYPE(enTimer6HwCaptureB));

    u32Val = TMR6x->HCPBR;
    TMR6x->HCPBR = u32Val | (1ul << enTimer6HwCaptureB);
    // TMR6x->PCONR_f.CAPMDB = 1;

    return Ok;
}

/*******************************************************************************
 * \brief   Clear Hardware Capture Event B
 *
 *
 * \param   [in]  TMR6x    Timer6 unit
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ClearHwCaptureB(M4_TMR6_TypeDef *TMR6x)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));

    TMR6x->HCPBR = 0ul;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Set trigger source 0 of hardware event
 **
 ** \param [in] enTriggerSrc            Counter event trigger source
 ** \arg 0-511                          Used to trigger counter start/stop/clear/increment/decrement/capture
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Timer6_SetTriggerSrc0(en_event_src_t enTriggerSrc)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_EVENT_SOURCE(enTriggerSrc));

    M4_AOS->TMR6_HTSSR1 = enTriggerSrc;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set trigger source 1 of hardware event
 **
 ** \param [in] enTriggerSrc            Counter event trigger source
 ** \arg 0-511                          Used to trigger counter start/stop/clear/increment/decrement/capture
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t Timer6_SetTriggerSrc1(en_event_src_t enTriggerSrc)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_EVENT_SOURCE(enTriggerSrc));

    M4_AOS->TMR6_HTSSR2 = enTriggerSrc;

    return enRet;
}

/*******************************************************************************
 * \brief   Z phase input mask config
 *
 *
 * \param   [in]  TMR6x                   Timer6 unit
 * \param   [in]  pstcTimer6ZMaskCfg      Z phase input mask config pointer
 *
 * \retval  Ok:  Set successfully
 *
 ******************************************************************************/
en_result_t Timer6_ConfigZMask(M4_TMR6_TypeDef *TMR6x, const stc_timer6_zmask_cfg_t *pstcTimer6ZMaskCfg)
{
    DDL_ASSERT(IS_VALID_NORMAL_TIMER6_UNIT(TMR6x));
    DDL_ASSERT(IS_VALID_ZPHASE_MASK_PRD(pstcTimer6ZMaskCfg->enZMaskCycle));

    TMR6x->GCONR_f.ZMSKVAL = pstcTimer6ZMaskCfg->enZMaskCycle;
    TMR6x->GCONR_f.ZMSKPOS = pstcTimer6ZMaskCfg->bFltPosCntMaksEn;
    TMR6x->GCONR_f.ZMSKREV = pstcTimer6ZMaskCfg->bFltRevCntMaksEn;

    return Ok;
}

//@} // Timer6Group

#endif /* DDL_TIMER6_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
