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
/** \file hc32f46x_adc.c
 **
 ** A detailed description is available at
 ** @link AdcGroup Adc description @endlink
 **
 **   - 2018-11-30  1.0 First version for Device Driver Library of Adc.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_adc.h"
#include "hc32f46x_utility.h"

#if (DDL_ADC_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup AdcGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! Parameter validity check for ADC peripherals. */
#define IS_ADC_PERIPH(ADCx) \
    (((ADCx) == M4_ADC1) || \
     ((ADCx) == M4_ADC2))

/*! Parameter validity check for ADC1 channel index. */
#define IS_ADC1_CH_INDEX(x) \
    (((x) < ADC1_CH_COUNT))

/*! Parameter validity check for ADC2 channel index. */
#define IS_ADC2_CH_INDEX(x) \
    (((x) < ADC2_CH_COUNT))

/*! Parameter validity check for ADC average count. */
#define IS_ADC_AVCNT(AVCNT)     \
    (((AVCNT) == AdcAvcnt_2) || \
     (((AVCNT) >= AdcAvcnt_4) && ((AVCNT) <= AdcAvcnt_256)))

/*! Parameter validity check for ADC data alignment. */
#define IS_ADC_DATA_ALIGN(ALIGN)        \
    (((ALIGN) == AdcDataAlign_Right) || \
     ((ALIGN) == AdcDataAlign_Left))

/*! Parameter validity check for ADC auto clear DR. */
#define IS_ADC_CLREN(EN)          \
    (((EN) == AdcClren_Enable) || \
     ((EN) == AdcClren_Disable))

/*! Parameter validity check for ADC resolution. */
#define IS_ADC_RESOLUTION(RESOLUTION)         \
    (((RESOLUTION) == AdcResolution_8Bit) ||  \
     ((RESOLUTION) == AdcResolution_10Bit) || \
     ((RESOLUTION) == AdcResolution_12Bit))

/*! Parameter validity check for ADC scan convert mode. */
#define IS_ADC_SCAN_MODE(MODE)           \
    (((MODE) == AdcMode_SAOnce) ||       \
     ((MODE) == AdcMode_SAContinuous) || \
     ((MODE) == AdcMode_SAOnceSBOnce) || \
     ((MODE) == AdcMode_SAContinuousSBOnce))

/*! Parameter validity check for ADC RSCHSEL. */
#define IS_ADC_RSCHSEL(SEL)            \
    (((SEL) == AdcRschsel_Continue) || \
     ((SEL) == AdcRschsel_Restart))

/*! Parameter validity check for ADC SA trigger source. */
#define IS_ADC_TRGEN(EN)          \
    (((EN) == AdcTrgen_Enable) || \
     ((EN) == AdcTrgen_Disable))

/*! Parameter validity check for ADC SA trigger source. */
#define IS_ADC_TRGSEL(SEL)          \
    (((SEL) == AdcTrgsel_ADTRGX) || \
     ((SEL) == AdcTrgsel_TRGX0) ||  \
     ((SEL) == AdcTrgsel_TRGX1) ||  \
     ((SEL) == AdcTrgsel_TRGX0_TRGX1))

/*! Parameter validity check for ADC EOCAIEN/ENCBIEN. */
#define IS_ADC_EOCIEN(EN)           \
    (((EN) == AdcEocien_Disable) || \
     ((EN) == AdcEocien_Enable))

/*! Parameter validity check for ADC sampling time. */
#define IS_ADC_SAMPLE_TIME(TIME) \
    (((TIME) == 255u) ||         \
     (((TIME) >= 5u) && ((TIME) <= 254u)))

/*! Parameter validity check for ADC sync trigger mode. */
#define IS_ADC_SYNC_MODE(MODE)               \
    (((MODE) == AdcSync_SingleSerial) ||     \
     ((MODE) == AdcSync_SingleParallel) ||   \
     ((MODE) == AdcSync_ContinuousSerial) || \
     ((MODE) == AdcSync_ContinuousParallel))

/*! Parameter validity check for ADC sync able. */
#define IS_ADC_SYNC_ENABLE(EN)    \
    (((EN) == AdcSync_Disable) || \
     ((EN) == AdcSync_Enable))

/*! Parameter validity check for ADC ADWIEN  */
#define IS_ADC_AWDIEN(EN)           \
    (((EN) == AdcAwdInt_Disable) || \
     ((EN) == AdcAwdInt_Enable))

/*! Parameter validity check for ADC AWDSS */
#define IS_ADC_AWDSS(SS)          \
    (((SS) == AdcAwdSel_SA_SB) || \
     ((SS) == AdcAwdSel_SA) ||    \
     ((SS) == AdcAwdSel_SB) ||    \
     ((SS) == AdcAwdSel_SB_SA))

/*! Parameter validity check for ADC AWDMD */
#define IS_ADC_AWDMD(MD)          \
    (((MD) == AdcAwdCmpMode_0) || \
     ((MD) == AdcAwdCmpMode_1))

/*! Parameter validity check for ADC AWDEN */
#define IS_ADC_AWDEN(EN)         \
    (((EN) == AdcAwd_Disable) || \
     ((EN) == AdcAwd_Enable))

/*! Parameter validity check for ADC PGA control */
#define IS_ADC_PGA_CTL(CTL)          \
    (((CTL) == AdcPgaCtl_Invalid) || \
     ((CTL) == AdcPgaCtl_Amplify))

/*! Parameter validity check for ADC gain factor. */
#define IS_ADC_PGA_FACTOR(FACTOR)    \
    (((FACTOR) == AdcPgaFactor_2) || \
     (((FACTOR) >= AdcPgaFactor_2P133) && ((FACTOR) <= AdcPgaFactor_32)))

/*! Parameter validity check for ADC PGA negative. */
#define IS_ADC_PGA_NEGATIVE(N)       \
    (((N) == AdcPgaNegative_VSSA) || \
     ((N) == AdcPgaNegative_PGAVSS))

/*! Parameter validity check for ADC trigger source event . */
#define IS_ADC_TRIG_SRC_EVENT(x)                                   \
    (((x) == EVT_PORT_EIRQ0) ||                                    \
     (((x) > EVT_PORT_EIRQ0) && ((x) <= EVT_PORT_EIRQ15)) ||       \
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

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void ADC_ReadAllData(const M4_ADC_TypeDef *ADCx, uint16_t *pu16AdcData, uint8_t u8Length);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Initializes an ADC instance.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] pstcInit                Pointer to ADC initialization structure.
 **                                     See @ref stc_adc_init_t for details.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_Init(M4_ADC_TypeDef *ADCx, const stc_adc_init_t *pstcInit)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != ADCx) && (NULL != pstcInit))
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        DDL_ASSERT(IS_ADC_RESOLUTION(pstcInit->enResolution));
        DDL_ASSERT(IS_ADC_DATA_ALIGN(pstcInit->enDataAlign));
        DDL_ASSERT(IS_ADC_CLREN(pstcInit->enAutoClear));
        DDL_ASSERT(IS_ADC_SCAN_MODE(pstcInit->enScanMode));
        DDL_ASSERT(IS_ADC_RSCHSEL(pstcInit->enRschsel));

        /* Stop ADC conversion. */
        ADCx->STR = 0u;

        ADCx->CR0_f.ACCSEL = pstcInit->enResolution;
        ADCx->CR0_f.DFMT = pstcInit->enDataAlign;
        ADCx->CR0_f.CLREN = pstcInit->enAutoClear;
        ADCx->CR0_f.MS = pstcInit->enScanMode;
        ADCx->CR1_f.RSCHSEL = pstcInit->enRschsel;

        /* Disable EOC(End Of Conversion) interrupts default. */
        ADCx->ICR = (uint8_t)0x0;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Deinitializes an ADC instance.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_DeInit(M4_ADC_TypeDef *ADCx)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        /* Set the value of all registers to the reset value. */
        /* Stop ADC conversion. */
        ADCx->STR = 0u;
        ADCx->CR0 = 0u;
        ADCx->CR1 = 0u;
        ADCx->TRGSR = 0u;
        ADCx->CHSELRA0 = 0u;
        ADCx->CHSELRB0 = 0u;
        ADCx->AVCHSELR0 = 0u;
        ADCx->AWDCHSR0 = 0u;
        ADCx->SSTR0 = (uint8_t)0x0B;
        ADCx->SSTR1 = (uint8_t)0x0B;
        ADCx->SSTR2 = (uint8_t)0x0B;
        ADCx->SSTR3 = (uint8_t)0x0B;
        ADCx->SSTR4 = (uint8_t)0x0B;
        ADCx->SSTR5 = (uint8_t)0x0B;
        ADCx->SSTR6 = (uint8_t)0x0B;
        ADCx->SSTR7 = (uint8_t)0x0B;
        ADCx->SSTR8 = (uint8_t)0x0B;
        ADCx->CHMUXR0 = (uint16_t)0x3210;
        ADCx->CHMUXR1 = (uint16_t)0x7654;
        ADCx->ISR = 0u;
        ADCx->ICR = (uint8_t)0x03;
        ADCx->AWDCR = 0u;
        ADCx->AWDDR0 = 0u;
        ADCx->AWDDR1 = 0u;
        ADCx->AWDCHSR0 = 0u;
        ADCx->AWDSR0 = 0u;

        if (M4_ADC1 == ADCx)
        {
            ADCx->CHSELRA1 = 0u;
            ADCx->CHSELRB1 = 0u;
            ADCx->AVCHSELR1 = 0u;
            ADCx->CHMUXR2 = (uint16_t)0xBA98;
            ADCx->CHMUXR3 = (uint16_t)0xFEDC;
            ADCx->SYNCCR = (uint16_t)0x0C00;
            ADCx->AWDCHSR1 = 0u;
            ADCx->AWDSR1 = 0u;
            ADCx->PGACR = 0u;
            ADCx->PGAGSR = 0u;
            ADCx->PGAINSR0 = 0u;
            ADCx->PGAINSR1 = 0u;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set scan mode.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param[in] enMode                   See @ref en_adc_scan_mode_t for details.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_SetScanMode(M4_ADC_TypeDef *ADCx, en_adc_scan_mode_t enMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        DDL_ASSERT(IS_ADC_SCAN_MODE(enMode));

        ADCx->CR0_f.MS = enMode;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set trigger source.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] pstcTrgCfg              Pointer to ADC trigger source configuration structure.
 ** \arg u8Sequence                     The sequence which you want to set it's trigger source.
 ** \arg enTrgEnable                    Enable or disable trigger source.
 ** \arg enTrgSel                       The type of trigger source.
 ** \arg enInTrg0                       Event number @ref en_event_src_t.
 ** \arg enInTrg1                       Event number @ref en_event_src_t.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 ** \note Sequence A and Sequence B CAN NOT set the same trigger source.
 **
 ******************************************************************************/
en_result_t ADC_ConfigTriggerSrc(M4_ADC_TypeDef *ADCx,
                                 const stc_adc_trg_cfg_t *pstcTrgCfg)
{
    __IO uint32_t *io32AdcxTrgSelR0 = NULL;
    __IO uint32_t *io32AdcxTrgSelR1 = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != ADCx) &&
        (NULL != pstcTrgCfg) &&
        (pstcTrgCfg->u8Sequence <= ADC_SEQ_B))
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        DDL_ASSERT(IS_ADC_TRGSEL(pstcTrgCfg->enTrgSel));
        enRet = Ok;
#ifdef __DEBUG
        switch (pstcTrgCfg->enTrgSel)
        {
        case AdcTrgsel_ADTRGX:
            break;

        case AdcTrgsel_TRGX0:
            DDL_ASSERT(IS_ADC_TRIG_SRC_EVENT(pstcTrgCfg->enInTrg0));
            break;

        case AdcTrgsel_TRGX1:
            DDL_ASSERT(IS_ADC_TRIG_SRC_EVENT(pstcTrgCfg->enInTrg1));
            break;

        case AdcTrgsel_TRGX0_TRGX1:
            DDL_ASSERT(IS_ADC_TRIG_SRC_EVENT(pstcTrgCfg->enInTrg0));
            DDL_ASSERT(IS_ADC_TRIG_SRC_EVENT(pstcTrgCfg->enInTrg1));
            break;

        default:
            enRet = ErrorInvalidParameter;
            break;
        }
        if (Ok == enRet)
#endif /* #ifdef __DEBUG */
        {
            if (ADC_SEQ_A == pstcTrgCfg->u8Sequence)
            {
                ADCx->TRGSR_f.TRGSELA = pstcTrgCfg->enTrgSel;
            }
            else
            {
                ADCx->TRGSR_f.TRGSELB = pstcTrgCfg->enTrgSel;
            }

            if (AdcTrgsel_ADTRGX != pstcTrgCfg->enTrgSel)
            {
                if (M4_ADC1 == ADCx)
                {
                    io32AdcxTrgSelR0 = &(M4_AOS->ADC1_ITRGSELR0);
                    io32AdcxTrgSelR1 = &(M4_AOS->ADC1_ITRGSELR1);
                }
                else
                {
                    io32AdcxTrgSelR0 = &(M4_AOS->ADC2_ITRGSELR0);
                    io32AdcxTrgSelR1 = &(M4_AOS->ADC2_ITRGSELR1);
                }

                switch (pstcTrgCfg->enTrgSel)
                {
                case AdcTrgsel_TRGX0:
                    *io32AdcxTrgSelR0 = pstcTrgCfg->enInTrg0;
                    break;

                case AdcTrgsel_TRGX1:
                    *io32AdcxTrgSelR1 = pstcTrgCfg->enInTrg1;
                    break;

                case AdcTrgsel_TRGX0_TRGX1:
                    *io32AdcxTrgSelR0 = pstcTrgCfg->enInTrg0;
                    *io32AdcxTrgSelR1 = pstcTrgCfg->enInTrg1;
                    break;

                default:
                    enRet = ErrorInvalidParameter;
                    break;
                }
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set trigger source.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u8Seq                   The sequence which you want to set it's trigger source.
 **
 ** \param [in] enState                 Enable or disable trigger source.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_TriggerSrcCmd(M4_ADC_TypeDef *ADCx,
                              uint8_t u8Seq,
                              en_functional_state_t enState)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != ADCx) && (u8Seq <= ADC_SEQ_B))
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));

        if (ADC_SEQ_A == u8Seq)
        {
            ADCx->TRGSR_f.TRGENA = enState;
        }
        else
        {
            ADCx->TRGSR_f.TRGENB = enState;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Add ADC channel.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] pstcChCfg               Pointer to ADC channel configuration structure.
 ** \arg u32Channel                     The channel(s) you want to configure.
 ** \arg u8Sequence                     The sequence which the channel(s) belong(s) to.
 ** \arg pu8SampTime                    Pointer to sampling time.
 **                                     eg. u32Channel = 1001b
 **                                     pu8SampTime[0] = channel 0's time
 **                                     pu8SampTime[1] = channel 3's time
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 ** \note Sequence A and Sequence B CAN NOT set the same channel!!!
 **
 ******************************************************************************/
en_result_t ADC_AddAdcChannel(M4_ADC_TypeDef *ADCx, const stc_adc_ch_cfg_t *pstcChCfg)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint8_t i;
    uint8_t j;
    uint32_t u32ChannelSel;
    __IO uint8_t *io8Sstr;

    if ((NULL != ADCx) &&
        (NULL != pstcChCfg) &&
        (pstcChCfg->u8Sequence <= ADC_SEQ_B))
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        if (M4_ADC1 == ADCx)
        {
            u32ChannelSel = pstcChCfg->u32Channel & ADC1_CH_ALL;
            if (ADC_SEQ_A == pstcChCfg->u8Sequence)
            {
                ADCx->CHSELRA0 |= (uint16_t)u32ChannelSel;
                ADCx->CHSELRA1 |= (uint16_t)(u32ChannelSel >> 16u);
            }
            else
            {
                ADCx->CHSELRB0 |= (uint16_t)u32ChannelSel;
                ADCx->CHSELRB1 |= (uint16_t)(u32ChannelSel >> 16u);
            }
        }
        else
        {
            u32ChannelSel = pstcChCfg->u32Channel & ADC2_CH_ALL;
            if (ADC_SEQ_A == pstcChCfg->u8Sequence)
            {
                ADCx->CHSELRA0 |= (uint16_t)u32ChannelSel;
            }
            else
            {
                ADCx->CHSELRB0 |= (uint16_t)u32ChannelSel;
            }
        }

        /* Set sampling time */
        i = 0u;
        j = 0u;
        io8Sstr = &(ADCx->SSTR0);
        while (0u != u32ChannelSel)
        {
            if (u32ChannelSel & 0x1ul)
            {
                io8Sstr[i] = pstcChCfg->pu8SampTime[j++];
            }
            u32ChannelSel >>= 1u;
            i++;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Delete ADC channel(s).
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] pstcChCfg               Pointer to ADC channel configuration structure.
 ** \arg u32Channel                     The channel(s) you want to delete.
 ** \arg u8Sequence                     (Ignore)
 ** \arg pu8SampTime                    (Ignore)
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ** \note                               You can use this function to delete ADC channel(s)
 **                                     and then set the corresponding pin(s) of the channel(s)
 **                                     to the other mode you need in your application.
 **
 ******************************************************************************/
en_result_t ADC_DelAdcChannel(M4_ADC_TypeDef *ADCx, const stc_adc_ch_cfg_t *pstcChCfg)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint16_t u16ChSelR0;
    uint16_t u16ChSelR1;

    if ((NULL != ADCx) &&
        (NULL != pstcChCfg) &&
        (pstcChCfg->u8Sequence <= ADC_SEQ_B))
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        u16ChSelR0 = (uint16_t)(pstcChCfg->u32Channel);
        u16ChSelR1 = (uint16_t)(pstcChCfg->u32Channel >> 16u);

        ADCx->CHSELRA0 &= (uint16_t)(~u16ChSelR0);
        ADCx->CHSELRB0 &= (uint16_t)(~u16ChSelR0);

        if (M4_ADC1 == ADCx)
        {
            ADCx->CHSELRA1 &= (uint16_t)(~u16ChSelR1);
            ADCx->CHSELRB1 &= (uint16_t)(~u16ChSelR1);
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief ADC interrupt configuration.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u8Seq                   The sequence to be configured.
 **
 ** \param [in] enState                 Enable or Disable sequence conversion done interrupt.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_SeqITCmd(M4_ADC_TypeDef *ADCx,
                         uint8_t u8Seq,
                         en_functional_state_t enState)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint8_t u8Msk = 0u;

    if ((NULL != ADCx) && (u8Seq <= ADC_SEQ_B))
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));

        u8Msk = (uint8_t)(0x1ul << u8Seq);
        ADCx->ISR &= (uint8_t)(~u8Msk);

        if (Enable == enState)
        {
            ADCx->ICR |= u8Msk;
        }
        else
        {
            ADCx->ICR &= (uint8_t)(~u8Msk);
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief ADC average conversion configuration.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] enAvgCnt                Average after enAvgCnt conversions.
 **                                     See @ref en_adc_avcnt_t for details.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_ConfigAvg(M4_ADC_TypeDef *ADCx, en_adc_avcnt_t enAvgCnt)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        DDL_ASSERT(IS_ADC_AVCNT(enAvgCnt));

        ADCx->CR0_f.AVCNT = enAvgCnt;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Add average channel(s).
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u32Channel              The channel(s), which will be set as average channel(s).
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ** \note                               The channel must first be configured as an analog channel
 **                                     by function ADC_AddAdcChannel.
 **
 ******************************************************************************/
en_result_t ADC_AddAvgChannel(M4_ADC_TypeDef *ADCx, uint32_t u32Channel)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint16_t u16AvgChR0;
    uint16_t u16AvgChR1;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        if (M4_ADC1 == ADCx)
        {
            u32Channel &= ADC1_CH_ALL;
        }
        else
        {
            u32Channel &= ADC2_CH_ALL;
        }

        u16AvgChR0 = (uint16_t)u32Channel;
        u16AvgChR1 = (uint16_t)(u32Channel >> 16u);

        ADCx->AVCHSELR0 |= u16AvgChR0;
        if (M4_ADC1 == ADCx)
        {
            ADCx->AVCHSELR1 |= u16AvgChR1;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Delete average channel(s).
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u32Channel              The average channel(s) which you want to delete.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_DelAvgChannel(M4_ADC_TypeDef *ADCx, uint32_t u32Channel)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint16_t u16AvgChR0;
    uint16_t u16AvgChR1;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        u16AvgChR0 = (uint16_t)u32Channel;
        u16AvgChR1 = (uint16_t)(u32Channel >> 16u);

        ADCx->AVCHSELR0 &= (uint16_t)(~u16AvgChR0);
        if (M4_ADC1 == ADCx)
        {
            ADCx->AVCHSELR1 &= (uint16_t)(~u16AvgChR1);
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief ADC AWD(analog watch dog) configuration.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] pstcAwdCfg              Pointer to the configuration structure.
 **                                     See @ref stc_adc_awd_cfg_t for details.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_ConfigAwd(M4_ADC_TypeDef *ADCx, const stc_adc_awd_cfg_t *pstcAwdCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != ADCx) && (NULL != pstcAwdCfg))
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        DDL_ASSERT(IS_ADC_AWDMD(pstcAwdCfg->enAwdmd));
        DDL_ASSERT(IS_ADC_AWDSS(pstcAwdCfg->enAwdss));

        ADCx->AWDCR_f.AWDEN = AdcAwd_Disable;
        ADCx->AWDCR_f.AWDIEN = AdcAwdInt_Disable;
        ADCx->AWDCR_f.AWDMD = pstcAwdCfg->enAwdmd;
        ADCx->AWDCR_f.AWDSS = pstcAwdCfg->enAwdss;

        ADCx->AWDDR0 = pstcAwdCfg->u16AwdDr0;
        ADCx->AWDDR1 = pstcAwdCfg->u16AwdDr1;

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable ADC AWD(analog watch dog).
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] enState                 Enable or disable AWD.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_AwdCmd(M4_ADC_TypeDef *ADCx, en_functional_state_t enState)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));

        ADCx->AWDCR_f.AWDEN = enState;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable ADC AWD(analog watch dog) interrupt.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] enState                 Enable or disable AWD interrupt.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_AwdITCmd(M4_ADC_TypeDef *ADCx, en_functional_state_t enState)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));

        ADCx->AWDCR_f.AWDIEN = enState;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Add AWD channel(s).
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u32Channel              The channel(s), which will be set as AWD channel(s).
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ** \note                               The channel must first be configured as an analog channel
 **                                     by function ADC_AddAdcChannel.
 **
 ******************************************************************************/
en_result_t ADC_AddAwdChannel(M4_ADC_TypeDef *ADCx, uint32_t u32Channel)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint16_t u16ChSelR0;
    uint16_t u16ChSelR1;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        if (M4_ADC1 == ADCx)
        {
            u32Channel &= ADC1_CH_ALL;
        }
        else
        {
            u32Channel &= ADC2_CH_ALL;
        }

        u16ChSelR0 = (uint16_t)u32Channel;
        u16ChSelR1 = (uint16_t)(u32Channel >> 16u);

        ADCx->AWDCHSR0 |= u16ChSelR0;
        if (M4_ADC1 == ADCx)
        {
            ADCx->AWDCHSR1 |= u16ChSelR1;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Delete AWD channel(s).
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u32Channel              The AWD channel(s) which you are going to delete.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_DelAwdChannel(M4_ADC_TypeDef *ADCx, uint32_t u32Channel)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint16_t u16ChSelR0;
    uint16_t u16ChSelR1;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        u16ChSelR0 = (uint16_t)u32Channel;
        u16ChSelR1 = (uint16_t)(u32Channel >> 16u);

        ADCx->AWDCHSR0 &= (uint16_t)(~u16ChSelR0);
        if (M4_ADC1 == ADCx)
        {
            ADCx->AWDCHSR1 &= (uint16_t)(~u16ChSelR1);
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief ADC programmable gain amplifier(PGA) configuration.
 **
 ** \param [in] enFactor                PGA gain factor.
 ** \param [in] enNegativeIn            PGA negative input select.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ** \note                               Only ADC1 has PGA.
 **
 ******************************************************************************/
void ADC_ConfigPga(en_adc_pga_factor_t enFactor, en_adc_pga_negative_t enNegativeIn)
{
    DDL_ASSERT(IS_ADC_PGA_FACTOR(enFactor));
    DDL_ASSERT(IS_ADC_PGA_NEGATIVE(enNegativeIn));

    M4_ADC1->PGAGSR_f.GAIN = enFactor;
    M4_ADC1->PGAINSR1_f.PGAVSSEN = enNegativeIn;
}

/**
 *******************************************************************************
 ** \brief Enable or disable PGA.
 **
 ** \param [in] enState                 Enable or disable PGA.
 **
 ** \retval None.
 **
 ******************************************************************************/
void ADC_PgaCmd(en_functional_state_t enState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));

    if (Enable == enState)
    {
        M4_ADC1->PGACR_f.PGACTL = AdcPgaCtl_Amplify;
    }
    else
    {
        M4_ADC1->PGACR_f.PGACTL = AdcPgaCtl_Invalid;
    }
}

/**
 *******************************************************************************
 ** \brief Add PGA channel(s).
 **
 ** \param[in] u32Channel               The channel(s), which you want to gain.
 **
 ** \retval None.
 **
 ** \note                               Only ADC1 has PGA. The channel must first
 **                                     be configured as an analog channel
 **                                     by function ADC_AddAdcChannel
 **
 ******************************************************************************/
void ADC_AddPgaChannel(uint32_t u32Channel)
{
    M4_ADC1->PGAINSR0 |= ((uint16_t)(u32Channel & PGA_CH_ALL));
}

/**
 *******************************************************************************
 ** \brief Delete PGA channel(s).
 **
 ** \param[in] u32Channel               The PGA channel(s) which will be deleted.
 **
 ** \retval None.
 **
 ******************************************************************************/
void ADC_DelPgaChannel(uint32_t u32Channel)
{
    M4_ADC1->PGAINSR0 &= (uint16_t)(~u32Channel);
}

/**
 *******************************************************************************
 ** \brief ADC sync mode configuration.
 **
 ** \param [in] enMode                  Synchronous mode types.
 ** \param [in] u8TrgDelay              ADC2 trigger delay time.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
void ADC_ConfigSync(en_adc_sync_mode_t enMode, uint8_t u8TrgDelay)
{
    DDL_ASSERT(IS_ADC_SYNC_MODE(enMode));

    /* Disable synchronous mode first. */
    M4_ADC1->SYNCCR_f.SYNCEN = AdcSync_Disable;

    M4_ADC1->SYNCCR_f.SYNCMD = enMode;
    M4_ADC1->SYNCCR_f.SYNCDLY = u8TrgDelay;
}

/**
 *******************************************************************************
 ** \brief Enable or disable sync mode.
 **
 ** \param [in] enState                 Enable or disable sync mode.
 **
 ** \retval None.
 **
 ******************************************************************************/
void ADC_SyncCmd(en_functional_state_t enState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));

    M4_ADC1->SYNCCR_f.SYNCEN = enState;
}

/**
 *******************************************************************************
 ** \brief Start an ADC conversion.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ** \note                               Software startup only support sequence A.
 **
 ******************************************************************************/
en_result_t ADC_StartConvert(M4_ADC_TypeDef *ADCx)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        ADCx->STR = 1u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Stop an ADC conversion and clear flags.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval ErrorTimeout                Timeout.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_StopConvert(M4_ADC_TypeDef *ADCx)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint32_t u32TimeCount = 0u;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        enRet = Ok;
        /* Make sure the ADC is really stopped. */
        while (ADCx->STR == 1u)
        {
            /* Stop ADC conversion. */
            ADCx->STR = 0u;
            if (++u32TimeCount > 10000u)
            {
                enRet = ErrorTimeout;
                break;
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the conversion status flag.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u8Seq                   The sequence which you want to get.
 **                                     it's conversion status flag.
 **
 ** \retval Set                         ADC converted done.
 ** \retval Reset                       ADC is converting or parameter error.
 **
 ******************************************************************************/
en_flag_status_t ADC_GetEocFlag(const M4_ADC_TypeDef *ADCx, uint8_t u8Seq)
{
    en_flag_status_t enFlag = Reset;

    DDL_ASSERT(IS_ADC_PERIPH(ADCx));

    if (ADCx->ISR & ((uint8_t)(0x1ul << u8Seq)))
    {
        enFlag = Set;
    }

    return enFlag;
}

/**
 *******************************************************************************
 ** \brief Clear conversion status flag of sequence A or sequence B.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u8Seq                   The sequence which you want to clear
 **                                     it's conversion status flag.
 **
 ** \retval None.
 **
 ******************************************************************************/
void ADC_ClrEocFlag(M4_ADC_TypeDef *ADCx, uint8_t u8Seq)
{
    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        ADCx->ISR &= (uint8_t)(~(0x1ul << u8Seq));
    }
}

/**
 *******************************************************************************
 ** \brief ADC start sequence A, check it's EOC status and get the data.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [out] pu16AdcData            The address to store ADC value.
 **                                     The location of the data store depends on
 **                                     the parameter u8Length.
 **                                     u8Length >= ADCx_CH_COUNT(ADC1_CH_COUNT or ADC2_CH_COUNT),
 **                                     all of the ADC data regs will be read:
 **                                     pu16AdcData[0] = data of Channel 0,
 **                                     pu16AdcData[1] = data of Channel 1,
 **                                     pu16AdcData[2] = data of Channel 2,
 **                                                 ...
 **                                     u8Length < ADCx_CH_COUNT(ADC1_CH_COUNT or ADC2_CH_COUNT),
 **                                     only the data of the enabled channles will be read:
 **                                     pu16AdcData[0] = data of the 1st enabled channel,
 **                                     pu16AdcData[1] = data of the 2nd enabled channel,
 **                                     pu16AdcData[2] = data of the 3rd enabled channel,
 **                                                 ...
 **
 ** \param [in] u8Length                The length of the ADC data to be read.
 **
 ** \param [in] u32Timeout              Timeout(millisecond).
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval ErrorTimeout                Timeout.
 ** \retval OperationInProgress         ADC is converting.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_PollingSa(M4_ADC_TypeDef *ADCx,
                          uint16_t *pu16AdcData,
                          uint8_t u8Length,
                          uint32_t u32Timeout)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint8_t u8AllDataLength = 0u;
    uint32_t u32Channel = 0u;
    uint32_t u32AdcTimeout = 0u;
    __IO uint32_t u32TimeCount = 0u;

    if ((NULL != ADCx) && (NULL != pu16AdcData) && (0u != u8Length))
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        if (M4_ADC1 == ADCx)
        {
            if (u8Length >= ADC1_CH_COUNT)
            {
                u8AllDataLength = ADC1_CH_COUNT;
            }
            else
            {
                u32Channel = (uint32_t)ADCx->CHSELRA1;
                u32Channel <<= 16u;
                u32Channel |= (uint32_t)ADCx->CHSELRA0;
            }
        }
        else
        {
            if (u8Length >= ADC2_CH_COUNT)
            {
                u8AllDataLength = ADC2_CH_COUNT;
            }
            else
            {
                u32Channel = (uint32_t)M4_ADC1->CHSELRA0;
            }
        }

        /* Start ADC conversion. */
        ADCx->STR = (uint8_t)0x01;

        /* 10 is the number of required instructions cycles for the below loop statement. */
        u32AdcTimeout = u32Timeout * (SystemCoreClock / 10u / 1000u);
        u32TimeCount = 0u;
        enRet = ErrorTimeout;
        while (u32TimeCount < u32AdcTimeout)
        {
            if (bM4_ADC1_ISR_EOCAF)
            {
                /* Get ADC data. */
                if (u8AllDataLength)
                {
                    ADC_ReadAllData(ADCx, pu16AdcData, u8AllDataLength);
                }
                else
                {
                    ADC_GetChData(ADCx, u32Channel, pu16AdcData, u8Length);
                }

                /* Clear sequence A flag. */
                bM4_ADC1_ISR_EOCAF = 0u;
                enRet = Ok;
                break;
            }
            u32TimeCount++;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Reading all data regs of an ADC.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [out] pu16AdcData            The address where the data will be stored.
 **                                     pu16AdcData[0] = data of Channel 0,
 **                                     pu16AdcData[1] = data of Channel 1,
 **                                     pu16AdcData[2] = data of Channel 2,
 **                                                 ...
 **
 ** \param [in] u8Length                The length of the ADC data to be read.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_GetAllData(const M4_ADC_TypeDef *ADCx, uint16_t *pu16AdcData, uint8_t u8Length)
{
    en_result_t enRet = ErrorInvalidParameter;

    DDL_ASSERT(IS_ADC_PERIPH(ADCx));
    if (((M4_ADC1 == ADCx) && (u8Length >= ADC1_CH_COUNT)) ||
        ((M4_ADC2 == ADCx) && (u8Length >= ADC2_CH_COUNT)))
    {
        ADC_ReadAllData(ADCx, pu16AdcData, u8Length);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Reading the data of the specified channel(s).
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base
 ** \arg M4_ADC1                        ADC unit 1 instance register base
 ** \arg M4_ADC2                        ADC unit 2 instance register base
 **
 ** \param [in] u32TargetCh             The specified channel(s)
 **
 ** \param [out] pu16AdcData            The address where the data will be stored.
 **                                     pu16AdcData[0] = data of the 1st enabled channel,
 **                                     pu16AdcData[1] = data of the 2nd enabled channel,
 **                                     pu16AdcData[2] = data of the 3rd enabled channel,
 **                                     eg. u32TargetCh = 1001b
 **                                     pu16AdcData[0] = Channel 0's data,
 **                                     pu16AdcData[1] = Channel 3's data,
 **
 ** \param [in] u8Length                The length of the ADC data to be read.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_GetChData(const M4_ADC_TypeDef *ADCx,
                          uint32_t u32TargetCh,
                          uint16_t *pu16AdcData,
                          uint8_t u8Length)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint8_t i;
    uint8_t j;
    uint32_t u32Channel;
    __IO const uint16_t *pu16DataReg = &(ADCx->DR0);

    if ((NULL != ADCx) && (NULL != pu16AdcData) && (0u != u8Length))
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        if (M4_ADC1 == ADCx)
        {
            u32Channel = u32TargetCh & ADC1_CH_ALL;
        }
        else
        {
            u32Channel = u32TargetCh & ADC2_CH_ALL;
        }

        i = 0u;
        j = 0u;
        while ((0u != u32Channel) && (0u != u8Length))
        {
            if (0u != (u32Channel & 0x1ul))
            {
                pu16AdcData[j] = pu16DataReg[i];
                j++;
                u8Length--;
            }

            u32Channel >>= 1u;
            i++;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the value of a specified channel via channel index.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base
 ** \arg M4_ADC1                        ADC unit 1 instance register base
 ** \arg M4_ADC2                        ADC unit 2 instance register base
 **
 ** \param [in] u8ChIndex               The index of the specified channel.
 **                                     u8ChIndex < ADC1_CH_COUNT while ADCx == M4_ADC1;
 **                                     u8ChIndex < ADC2_CH_COUNT while ADCx == M4_ADC2.
 **
 ** \retval An uint16_t value -- the ADC value of the specified channel.
 **
 ******************************************************************************/
uint16_t ADC_GetValue(const M4_ADC_TypeDef *ADCx, uint8_t u8ChIndex)
{
    __IO const uint16_t *pu16DataReg = &(ADCx->DR0);

    DDL_ASSERT(IS_ADC_PERIPH(ADCx));

    return pu16DataReg[u8ChIndex];
}

/**
 *******************************************************************************
 ** \brief Get all AWD channels status flags.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \retval u32AwdFlag                  0  -- No ADC channel meets AWD comparison conditions.
 **                                     !0 -- The bit value of the channel that satisfies the
 **                                           AWD condition is 1.
 **
 ******************************************************************************/
uint32_t ADC_GetAwdFlag(const M4_ADC_TypeDef *ADCx)
{
    uint32_t u32AwdFlag;

    DDL_ASSERT(IS_ADC_PERIPH(ADCx));

    if (M4_ADC1 == ADCx)
    {
        u32AwdFlag = ADCx->AWDSR1;
        u32AwdFlag <<= 16u;
        u32AwdFlag |= ADCx->AWDSR0;
    }
    else
    {
        u32AwdFlag = ADCx->AWDSR0;
    }

    return u32AwdFlag;
}

/**
 *******************************************************************************
 ** \brief Clear all AWD channels status flags
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ******************************************************************************/
void ADC_ClrAwdFlag(M4_ADC_TypeDef *ADCx)
{
    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        ADCx->AWDSR0 = 0u;
        if (M4_ADC1 == ADCx)
        {
            ADCx->AWDSR0 = 0u;
            ADCx->AWDSR1 = 0u;
        }
    }
}

/**
 *******************************************************************************
 ** \brief Clear AWD specified channels status flags.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u32AwdCh                The channel(s) which you want to clear it's flag(s).
 **
 **
 ******************************************************************************/
void ADC_ClrAwdChFlag(M4_ADC_TypeDef *ADCx, uint32_t u32AwdCh)
{
    uint16_t u16ChR0;
    uint16_t u16ChR1;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));

        u16ChR0 = (uint16_t)u32AwdCh;
        u16ChR1 = (uint16_t)(u32AwdCh >> 16u);

        ADCx->AWDSR0 &= (uint16_t)(~u16ChR0);
        if (M4_ADC1 == ADCx)
        {
            ADCx->AWDSR1 &= (uint16_t)(~u16ChR1);
        }
    }
}

/**
 *******************************************************************************
 ** \brief Remap an ADC pin to channel(s).
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u32DestChannel          Destination channel(s).
 **
 ** \param [in] u8AdcPin                ADC pin number.
 **
 ** \retval ErrorInvalidParameter       Parameter error.
 ** \retval Ok                          No error occurred.
 **
 ******************************************************************************/
en_result_t ADC_ChannleRemap(M4_ADC_TypeDef *ADCx,
                             uint32_t u32DestChannel,
                             uint8_t u8AdcPin)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint8_t i;
    uint8_t u8OffsetReg;
    uint8_t u8ChPos;
    uint16_t u16AdcPin = u8AdcPin;
    __IO uint16_t *io16Chmuxr = NULL;

    if (NULL != ADCx)
    {
        DDL_ASSERT(IS_ADC_PERIPH(ADCx));
        enRet = Ok;
        if (M4_ADC1 == ADCx)
        {
            if (u16AdcPin <= ADC1_IN15)
            {
                u32DestChannel &= ADC1_PIN_MASK_ALL;
            }
            else
            {
                enRet = ErrorInvalidParameter;
            }
        }
        else
        {
            if ((u16AdcPin > ADC12_IN4) && (u16AdcPin < ADC12_IN11))
            {
                u16AdcPin -= 4u;
                u32DestChannel &= ADC2_PIN_MASK_ALL;
            }
            else
            {
                enRet = ErrorInvalidParameter;
            }
        }

        if (Ok == enRet)
        {
            i = 0u;
            while (0u != u32DestChannel)
            {
                if (u32DestChannel & 0x1ul)
                {
                    u8OffsetReg = i / 4u;
                    u8ChPos = (i % 4u) * 4u;
                    io16Chmuxr = &(ADCx->CHMUXR0) + u8OffsetReg;
                    *io16Chmuxr &= (uint16_t)(~(0xFul << u8ChPos));
                    *io16Chmuxr |= (uint16_t)(u16AdcPin << u8ChPos);
                }

                u32DestChannel >>= 1u;
                i++;
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the number of the pin corresponding to the specified channel.
 **
 ** \param [in] ADCx                    Pointer to ADC instance register base.
 ** \arg M4_ADC1                        ADC unit 1 instance register base.
 ** \arg M4_ADC2                        ADC unit 2 instance register base.
 **
 ** \param [in] u8ChIndex               This channel that you want to get its pin number.
 **
 ** \retval [0, 15]                     The correct ADC pin number.
 ** \retval [0xFF]                      The invalid ADC pin number.
 **
 ******************************************************************************/
uint8_t ADC_GetChannelPinNum(const M4_ADC_TypeDef *ADCx, uint8_t u8ChIndex)
{
    uint8_t u8OffsetPin;
    uint8_t u8OffsetReg;
    uint8_t u8ChPos;
    uint8_t u8AdcPin = ADC_PIN_INVALID;
    __IO const uint16_t *io16Chmuxr = NULL;

    DDL_ASSERT(IS_ADC_PERIPH(ADCx));

    if (M4_ADC1 == ADCx)
    {
        DDL_ASSERT(IS_ADC1_CH_INDEX(u8ChIndex));
        u8OffsetPin = 0u;
    }
    else
    {
        DDL_ASSERT(IS_ADC2_CH_INDEX(u8ChIndex));
        u8OffsetPin = 4u;
    }

    u8OffsetReg = u8ChIndex / 4u;
    u8ChPos = (u8ChIndex % 4u) * 4u;
    io16Chmuxr = &(ADCx->CHMUXR0) + u8OffsetReg;
    u8AdcPin = (uint8_t)((*io16Chmuxr >> u8ChPos) & ((uint16_t)0xF));
    u8AdcPin += u8OffsetPin;

    return u8AdcPin;
}

/*******************************************************************************
 * Function implementation - local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Read all data of an ADC. pu16AdcData[0] = DR0, pu16AdcData[1] = DR1, ...
 **
 ******************************************************************************/
static void ADC_ReadAllData(const M4_ADC_TypeDef *ADCx, uint16_t *pu16AdcData, uint8_t u8Length)
{
    uint8_t i;
    __IO const uint16_t *pu16DataReg = &(ADCx->DR0);

    for (i = 0u; i < u8Length; i++)
    {
        pu16AdcData[i] = pu16DataReg[i];
    }
}

//@} // AdcGroup

#endif /* DDL_ADC_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
