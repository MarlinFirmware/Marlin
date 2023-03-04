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
/** \file hc32f46x_ots.c
 **
 ** A detailed description is available at
 ** @link OtsGroup Ots description @endlink
 **
 **   - 2018-10-26  1.0 Wuze First version for Device Driver Library of Ots.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_ots.h"
#include "hc32f46x_utility.h"

#if (DDL_OTS_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup OtsGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! Parameter validity check for OTS auto off configuration value. */
#define IS_OTS_AUTO_OFF(EN)          \
    (((EN) == OtsAutoOff_Disable) || \
     ((EN) == OtsAutoOff_Enable))

/*! Parameter validity check for OTS interrupt enable/disable. */
#define IS_OTS_IE(IE)            \
    (((IE) == OtsInt_Disable) || \
     ((IE) == OtsInt_Enable))

/*! Parameter validity check for OTS clock selection configuration value. */
#define IS_OTS_CLK_SEL(CLK)       \
    (((CLK) == OtsClkSel_Xtal) || \
     ((CLK) == OtsClkSel_Hrc))

/*! Parameter validity check for OTS trigger source event . */
#define IS_OTS_TRIG_SRC_EVENT(x)                                   \
    ((((x) >= EVT_PORT_EIRQ0) && ((x) <= EVT_PORT_EIRQ15)) ||      \
     (((x) >= EVT_DMA1_TC0) && ((x) <= EVT_DMA2_BTC3)) ||          \
     (((x) >= EVT_EFM_OPTEND) && ((x) <= EVT_USBFS_SOF)) ||        \
     (((x) >= EVT_DCU1) && ((x) <= EVT_DCU4)) ||                   \
     (((x) >= EVT_TMR01_GCMA) && ((x) <= EVT_TMR02_GCMB)) ||       \
     (((x) >= EVT_RTC_ALM) && ((x) <= EVT_RTC_PRD)) ||             \
     (((x) >= EVT_TMR61_GCMA) && ((x) <= EVT_TMR61_GUDF)) ||       \
     (((x) >= EVT_TMR61_SCMA) && ((x) <= EVT_TMR61_SCMB)) ||       \
     (((x) >= EVT_TMR62_GCMA) && ((x) <= EVT_TMR62_GUDF)) ||       \
     (((x) >= EVT_TMR62_SCMA) && ((x) <= EVT_TMR62_SCMB)) ||       \
     (((x) >= EVT_TMR63_GCMA) && ((x) <= EVT_TMR63_GUDF)) ||       \
     (((x) >= EVT_TMR63_SCMA) && ((x) <= EVT_TMR63_SCMB)) ||       \
     (((x) >= EVT_TMRA1_OVF) && ((x) <= EVT_TMRA5_CMP)) ||         \
     (((x) >= EVT_TMRA6_OVF) && ((x) <= EVT_TMRA6_CMP)) ||         \
     (((x) >= EVT_USART1_EI) && ((x) <= EVT_USART4_RTO)) ||        \
     (((x) >= EVT_SPI1_SRRI) && ((x) <= EVT_AOS_STRG)) ||          \
     (((x) >= EVT_TMR41_SCMUH) && ((x) <= EVT_TMR42_SCMWL)) ||     \
     (((x) >= EVT_TMR43_SCMUH) && ((x) <= EVT_TMR43_SCMWL)) ||     \
     (((x) >= EVT_EVENT_PORT1) && ((x) <= EVT_EVENT_PORT4)) ||     \
     (((x) >= EVT_I2S1_TXIRQOUT) && ((x) <= EVT_I2S1_RXIRQOUT)) || \
     (((x) >= EVT_I2S2_TXIRQOUT) && ((x) <= EVT_I2S2_RXIRQOUT)) || \
     (((x) >= EVT_I2S3_TXIRQOUT) && ((x) <= EVT_I2S3_RXIRQOUT)) || \
     (((x) >= EVT_I2S4_TXIRQOUT) && ((x) <= EVT_I2S4_RXIRQOUT)) || \
     (((x) >= EVT_ACMP1) && ((x) <= EVT_ACMP3)) ||                 \
     (((x) >= EVT_I2C1_RXI) && ((x) <= EVT_I2C3_EE1)) ||           \
     (((x) >= EVT_PVD_PVD1) && ((x) <= EVT_OTS)) ||                \
     ((x) == EVT_WDT_REFUDF) ||                                    \
     (((x) >= EVT_ADC1_EOCA) && ((x) <= EVT_TRNG_END)) ||          \
     (((x) >= EVT_SDIOC1_DMAR) && ((x) <= EVT_SDIOC1_DMAW)) ||     \
     (((x) >= EVT_SDIOC2_DMAR) && ((x) <= EVT_SDIOC2_DMAW)) ||     \
     ((x) == EVT_MAX))

#define EXPERIMENT_COUNT ((uint8_t)10)

// #define OTS_USE_DEFAULT_PARA
#define OTS_TRIMMING

#ifndef OTS_USE_DEFAULT_PARA
#define OTS_XTAL_K (737272.73f)
#define OTS_XTAL_M (27.55f)
#define OTS_HRC_K (3002.59f)
#define OTS_HRC_M (27.92f)
#endif

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
#ifdef OTS_TRIMMING
static void OTS_Trimming(void);
#endif
static void OTS_ReadData(float32_t *pf32Dr1, float32_t *pf32Dr2, float32_t *pf32Ecr);
static float OTS_ComputeA(float32_t f32Dr1, float32_t f32Dr2, float32_t f32Ecr);
static float OTS_ComputeTemp(float32_t f32Dr1, float32_t f32Dr2, float32_t f32Ecr);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t m_u8OtsClkSel;
static float32_t m_f32OtsSlopeK = 0.0f;
static float32_t m_f32OtsOffsetM = 0.0f;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
*******************************************************************************
** \brief Initializes the OTS.
**
** \param [in] pstcInit                See @ref stc_ots_init_t for details.
**
** \retval Ok                          No error occurred.
** \retval ErrorInvalidParameter       Parameter error.
**
******************************************************************************/
en_result_t OTS_Init(const stc_ots_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

#ifdef OTS_USE_DEFAULT_PARA
    struct
    {
        int32_t s32K : 24;
        int32_t s32M : 8;
    } stcPara;
#endif

    if (NULL != pstcInit)
    {
        DDL_ASSERT(IS_OTS_AUTO_OFF(pstcInit->enAutoOff));
        DDL_ASSERT(IS_OTS_CLK_SEL(pstcInit->enClkSel));
#ifdef OTS_TRIMMING
        OTS_Trimming();
#endif
        /* Stop ots sampling. */
        bM4_OTS_CTL_OTSST = 0u;

        M4_OTS->CTL_f.TSSTP = pstcInit->enAutoOff;
        /* Disable OTS interrupt default. */
        M4_OTS->CTL_f.OTSIE = OtsInt_Disable;
        M4_OTS->CTL_f.OTSCK = pstcInit->enClkSel;
        m_u8OtsClkSel = pstcInit->enClkSel;

#ifdef OTS_USE_DEFAULT_PARA
        stcPara.s32K = M4_OTS->LPR_f.TSSLP;
        stcPara.s32M = M4_OTS->LPR_f.TSOFS;

        m_f32OtsSlopeK = ((float)(stcPara.s32K * pstcInit->u8ClkFreq)) / 8.0f;
        m_f32OtsOffsetM = (float32_t)stcPara.s32M;
#else
        if (OtsClkSel_Xtal == pstcInit->enClkSel)
        {
            m_f32OtsSlopeK = OTS_XTAL_K;
            m_f32OtsOffsetM = OTS_XTAL_M;
        }
        else
        {
            m_f32OtsSlopeK = OTS_HRC_K;
            m_f32OtsOffsetM = OTS_HRC_M;
        }
#endif

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Deinitializes the TRNG.
 **
 ** \param  None.
 **
 ** \retval None.
 **
 ******************************************************************************/
void OTS_DeInit(void)
{
    /* Stop ots sampling. */
    bM4_OTS_CTL_OTSST = 0u;

    /* Set the value of all registers to the reset value. */
    M4_OTS->CTL = 0u;
    M4_OTS->DR1 = 0u;
    M4_OTS->DR2 = 0u;
    M4_OTS->ECR = 0u;
}

/**
 *******************************************************************************
 ** \brief  Start OTS and get the temperature value.
 **
 ** \param  [out] pf32Temp              The address to store the temperature value.
 **
 ** \param  [in] u32Timeout             Timeout value(millisecond).
 **
 ** \retval Ok                          No error occurred.
 ** \retval ErrorTimeout                OTS works timeout.
 ** \retval ErrorInvalidParameter       Parameter error.
 **
 ******************************************************************************/
en_result_t OTS_StartGetTemp(float32_t *pf32Temp, uint32_t u32Timeout)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != pf32Temp) && (0u != u32Timeout))
    {
        /* Start ots sampling. */
        bM4_OTS_CTL_OTSST = 1u;

        enRet = OTS_CheckSample(pf32Temp, u32Timeout);
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Enable or disable OTS interrupt.
 **
 ** \param  [in] enState                Enable or disable OTS interrupt.
 **
 ** \retval None.
 **
 ******************************************************************************/
void OTS_ITCmd(en_functional_state_t enState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));

    bM4_OTS_CTL_OTSIE = (uint32_t)enState;
}

/**
 *******************************************************************************
 ** \brief  Start OTS only.
 **
 ** \param  None.
 **
 ** \retval None.
 **
 ******************************************************************************/
void OTS_StartIT(void)
{
    /* Start ots sampling. */
    bM4_OTS_CTL_OTSST = 1u;
}

/**
 *******************************************************************************
 ** \brief  Get temperature value.
 **
 ** \param  None.
 **
 ** \retval                             Temperature value.
 **
 ******************************************************************************/
float32_t OTS_GetTempIT(void)
{
    float32_t f32Dr1;
    float32_t f32Dr2;
    float32_t f32Ecr;

    OTS_ReadData(&f32Dr1, &f32Dr2, &f32Ecr);

    return OTS_ComputeTemp(f32Dr1, f32Dr2, f32Ecr);
}

/**
 *******************************************************************************
 ** \brief  Set OTS AOS trigger source.
 **
 ** \param  [in] enEvent                See @ref en_event_src_t for details.
 **
 ** \retval None.
 **
 ******************************************************************************/
void OTS_SetTriggerSrc(en_event_src_t enEvent)
{
    DDL_ASSERT(IS_OTS_TRIG_SRC_EVENT(enEvent) && (EVT_OTS != enEvent));

    M4_AOS->OTS_TRG = enEvent;
}

/**
*******************************************************************************
** \brief  Check and get temperature value.
**
** \param  [out] pf32Temp              The address where the temperature value stored.
**
** \param  [in] u32Timeout             Timeout value(millisecond).
**
** \retval Ok                          No error occurred.
** \retval ErrorTimeout                OTS works timeout.
** \retval ErrorInvalidParameter       Parameter error.
**
******************************************************************************/
en_result_t OTS_CheckSample(float32_t *pf32Temp, uint32_t u32Timeout)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t u32OtsTimeout;
    __IO uint32_t u32TimeCount;
    float32_t f32Dr1 = 0.0f;
    float32_t f32Dr2 = 0.0f;
    float32_t f32Ecr = 0.0f;

    if ((NULL != pf32Temp) && (0u != u32Timeout))
    {
        /* 10 is the number of required instructions cycles for the below loop statement. */
        u32OtsTimeout = u32Timeout * (SystemCoreClock / 10u / 1000u);
        u32TimeCount = 0u;
        enRet = ErrorTimeout;
        while (u32TimeCount < u32OtsTimeout)
        {
            if (bM4_OTS_CTL_OTSST == 0u)
            {
                enRet = Ok;
                OTS_ReadData(&f32Dr1, &f32Dr2, &f32Ecr);
                break;
            }

            u32TimeCount++;
        }

        /* Stop ots sampling. */
        bM4_OTS_CTL_OTSST = 0u;

        if (Ok == enRet)
        {
            *pf32Temp = OTS_ComputeTemp(f32Dr1, f32Dr2, f32Ecr);
        }
    }

    return enRet;
}

/**
*******************************************************************************
** \brief  OTS scaling experiment. If you want to get a more accurate temperature value,
**         you need to do a calibration experiment.
**
** \param  [out] pu16Dr1                Address to store OTS data register 1.
**
** \param  [out] pu16Dr2                Address to store OTS data register 2.
**
** \param  [out] pu16Ecr                Address to store OTS error compensation register.
**
** \param  [out] pf32A                  Address to store parameter A(for calibration experiments).
**
******************************************************************************/
void OTS_ScalingExperiment(uint16_t *pu16Dr1, uint16_t *pu16Dr2,
                           uint16_t *pu16Ecr, float32_t *pf32A)
{
    float32_t f32Dr1;
    float32_t f32Dr2;
    float32_t f32Ecr;

    if ((NULL != pu16Dr1) && (NULL != pu16Dr2) && (NULL != pu16Ecr) && (NULL != pf32A))
    {
        /* Start ots sampling. */
        bM4_OTS_CTL_OTSST = 1u;

        while ((bM4_OTS_CTL_OTSST & 0x1u) != 0u)
        {
            ;
        }

        *pu16Dr1 = M4_OTS->DR1;
        *pu16Dr2 = M4_OTS->DR2;
        *pu16Ecr = M4_OTS->ECR;

        f32Dr1 = (float32_t)(*pu16Dr1);
        f32Dr2 = (float32_t)(*pu16Dr2);
        if (OtsClkSel_Hrc == m_u8OtsClkSel)
        {
            f32Ecr = (float32_t)(*pu16Ecr);
        }
        else
        {
            f32Ecr = 1.0f;
        }

        *pf32A = OTS_ComputeA(f32Dr1, f32Dr2, f32Ecr);
    }
}

/**
*******************************************************************************
** \brief
**
******************************************************************************/
void OTS_SetKM(float32_t f32K, float32_t f32M)
{
    m_f32OtsSlopeK = f32K;
    m_f32OtsOffsetM = f32M;
}

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/**
*******************************************************************************
** \brief  OTS Trimming.
**
******************************************************************************/
#ifdef OTS_TRIMMING
#define bM4_OTSCR2 (*((__IO uint32_t *)(0x4004A410)))
#define bM4_OTSTMR (*((__IO uint32_t *)(0x4004A412)))
static void OTS_Trimming(void)
{
    uint16_t m_u16OtsTrm;

    m_u16OtsTrm = (uint16_t)bM4_OTSTMR;
    bM4_OTSCR2 = (uint16_t)0x7841;
    bM4_OTSCR2 = (uint16_t)0x7841;
    bM4_OTSTMR = m_u16OtsTrm;
}
#endif
/**
*******************************************************************************
** \brief  Read OTS data registers.
**
******************************************************************************/
static void OTS_ReadData(float32_t *pf32Dr1, float32_t *pf32Dr2, float32_t *pf32Ecr)
{
    uint16_t u16Dr1;
    uint16_t u16Dr2;
    uint16_t u16Ecr;

    u16Dr1 = M4_OTS->DR1;
    u16Dr2 = M4_OTS->DR2;
    u16Ecr = M4_OTS->ECR;

    *pf32Dr1 = (float32_t)u16Dr1;
    *pf32Dr2 = (float32_t)u16Dr2;
    if (OtsClkSel_Hrc == m_u8OtsClkSel)
    {
        *pf32Ecr = (float32_t)u16Ecr;
    }
    else
    {
        *pf32Ecr = 1.0f;
    }
}

/**
*******************************************************************************
** \brief  Calculate A.
**
******************************************************************************/
static float OTS_ComputeA(float32_t f32Dr1, float32_t f32Dr2, float32_t f32Ecr)
{
    float32_t f32Ret = 0.f;

    if ((0.0f != f32Dr1) && (0.0f != f32Dr2) && (0.0f != f32Ecr))
    {
        f32Ret = (((1.0f / f32Dr1) - (1.0f / f32Dr2)) * f32Ecr);
    }

    return f32Ret;
}

/**
*******************************************************************************
** \brief  Calculate temperature value.
**
******************************************************************************/
static float OTS_ComputeTemp(float32_t f32Dr1, float32_t f32Dr2, float32_t f32Ecr)
{
    float32_t f32Ret = 0.f;

    if ((0.0f != f32Dr1) && (0.0f != f32Dr2) && (0.0f != f32Ecr))
    {
        f32Ret = m_f32OtsSlopeK * ((1.0f / f32Dr1) - (1.0f / f32Dr2)) * f32Ecr + m_f32OtsOffsetM;
    }

    return f32Ret;
}

//@} // OtsGroup

#endif /* DDL_OTS_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
