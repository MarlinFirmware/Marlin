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
/** \file hc32f46x_adc.h
 **
 ** A detailed description is available at
 ** @link AdcGroup Adc description @endlink
 **
 **   - 2018-11-30  1.0 First version for Device Driver Library of Adc.
 **
 ******************************************************************************/
#ifndef __HC32F46X_ADC_H__
#define __HC32F46X_ADC_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_ADC_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup AdcGroup Analog-to-Digital Converter(ADC)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief  ADC average count.
     **
     ******************************************************************************/
    typedef enum en_adc_avcnt
    {
        AdcAvcnt_2 = 0x0,   ///< Average after 2 conversions.
        AdcAvcnt_4 = 0x1,   ///< Average after 4 conversions.
        AdcAvcnt_8 = 0x2,   ///< Average after 8 conversions.
        AdcAvcnt_16 = 0x3,  ///< Average after 16 conversions.
        AdcAvcnt_32 = 0x4,  ///< Average after 32 conversions.
        AdcAvcnt_64 = 0x5,  ///< Average after 64 conversions.
        AdcAvcnt_128 = 0x6, ///< Average after 128 conversions.
        AdcAvcnt_256 = 0x7, ///< Average after 256 conversions.
    } en_adc_avcnt_t;

    /**
     *******************************************************************************
     ** \brief  ADC data alignment
     **
     ******************************************************************************/
    typedef enum en_adc_data_align
    {
        AdcDataAlign_Right = 0x0, ///< Data right alignment.
        AdcDataAlign_Left = 0x1,  ///< Data left alignment.
    } en_adc_data_align_t;

    /**
     *******************************************************************************
     ** \brief  Automatically clear data registers after reading data.
     **         The auto clear function is mainly used to detect whether the data register
     **         is updated.
     **
     ******************************************************************************/
    typedef enum en_adc_clren
    {
        AdcClren_Disable = 0x0, ///< Automatic clear function disable.
        AdcClren_Enable = 0x1,  ///< Automatic clear function enable.
    } en_adc_clren_t;

    /**
     *******************************************************************************
     ** \brief  ADC resolution.
     **
     ******************************************************************************/
    typedef enum en_adc_resolution
    {
        AdcResolution_12Bit = 0x0, ///< Resolution is 12 bit.
        AdcResolution_10Bit = 0x1, ///< Resolution is 10 bit.
        AdcResolution_8Bit = 0x2,  ///< Resolution is 8 bit.
    } en_adc_resolution_t;

    /**
     *******************************************************************************
     ** \brief  ADC scan mode.
     **
     ******************************************************************************/
    typedef enum en_adc_scan_mode
    {
        AdcMode_SAOnce = 0x0,             ///< Sequence A works once.
        AdcMode_SAContinuous = 0x1,       ///< Sequence A works always.
        AdcMode_SAOnceSBOnce = 0x2,       ///< Sequence A and sequence B work once.
        AdcMode_SAContinuousSBOnce = 0x3, ///< Sequence A works always, sequence works once.
    } en_adc_scan_mode_t;

    /**
     *******************************************************************************
     ** \brief  ADC sequence A restart position.
     **
     ******************************************************************************/
    typedef enum en_adc_rschsel
    {
        AdcRschsel_Continue = 0x0, ///< After sequence A is interrupted by sequence B,
                                   ///< sequence A continues to scan from the interrupt
                                   ///< when it restarts.

        AdcRschsel_Restart = 0x1, ///< After sequence A is interrupted by sequence B,
                                  ///< sequence A restarts scanning from the first channel
                                  ///< when it restarts.
    } en_adc_rschsel_t;

    /**
     *******************************************************************************
     ** \brief  ADC external or internal trigger source enable/disable .
     **
     ******************************************************************************/
    typedef enum en_adc_trgen
    {
        AdcTrgen_Disable = 0x0, ///< External or internal trigger source disable.
        AdcTrgen_Enable = 0x1,  ///< External or internal trigger source enable.
    } en_adc_trgen_t;

    /**
     *******************************************************************************
     ** \brief  ADC sequence trigger source selection.
     **
     ******************************************************************************/
    typedef enum en_adc_trgsel
    {
        AdcTrgsel_ADTRGX = 0x0,      ///< X = 1(use ADC1) / 2(use ADC2), same as below.
        AdcTrgsel_TRGX0 = 0x1,       ///< Pin IN_TRG10 / IN_TRG20.
        AdcTrgsel_TRGX1 = 0x2,       ///< Pin IN_TRG11 / IN_TRG21.
        AdcTrgsel_TRGX0_TRGX1 = 0x3, ///< Pin IN_TRG10 + IN_TRG11 / IN_TRG20 + IN_TRG21.
    } en_adc_trgsel_t;

    /**
     *******************************************************************************
     ** \brief  Sequence A/B conversion completion interrupt enable/disable.
     **
     ******************************************************************************/
    typedef enum en_adc_eocien
    {
        AdcEocien_Disable = 0x0, ///< Conversion completion interrupt disable.
        AdcEocien_Enable = 0x1,  ///< Conversion completion interrupt enable.
    } en_adc_eocien_t;

    /**
     *******************************************************************************
     ** \brief ADC sync mode.
     **
     ******************************************************************************/
    typedef enum en_adc_sync_mode
    {
        AdcSync_SingleSerial = 0x0u,     ///< Single: ADC1 and ADC2 only sample and convert once after triggering.
                                         ///< Serial: ADC2 start after ADC1 N PCLK4 cycles.
        AdcSync_SingleParallel = 0x2u,   ///< Parallel: ADC1 and ADC2 start at the same time.
        AdcSync_ContinuousSerial = 0x4u, ///< Continuous: ADC1 and ADC2 continuously sample and convert after triggering.
        AdcSync_ContinuousParallel = 0x6u,
    } en_adc_sync_mode_t;

    /**
     *******************************************************************************
     ** \brief  ADC sync enable/disable.
     **
     ******************************************************************************/
    typedef enum en_adc_syncen
    {
        AdcSync_Disable = 0x0, ///< Disable sync mode.
        AdcSync_Enable = 0x1,  ///< Enable sync mode.
    } en_adc_syncen_t;

    /**
     *******************************************************************************
     ** \brief  Analog watchdog interrupt enable/disable.
     **
     ******************************************************************************/
    typedef enum en_adc_awdien
    {
        AdcAwdInt_Disable = 0x0, ///< Disable AWD interrupt.
        AdcAwdInt_Enable = 0x1,  ///< Enable AWD interrupt.
    } en_adc_awdien_t;

    /**
     *******************************************************************************
     ** \brief  Analog watchdog interrupt event sequence selection.
     **
     ******************************************************************************/
    typedef enum en_adc_awdss
    {
        AdcAwdSel_SA_SB = 0x0, ///< Sequence A and B output interrupt event -- ADC_SEQCMP.
        AdcAwdSel_SA = 0x1,    ///< Sequence A output interrupt event -- ADC_SEQCMP.
        AdcAwdSel_SB = 0x2,    ///< Sequence B output interrupt event -- ADC_SEQCMP.
        AdcAwdSel_SB_SA = 0x3, ///< Same as AdcAwdSel_SA_SB.
    } en_adc_awdss_t;

    /**
     *******************************************************************************
     ** \brief  Analog watchdog comparison mode selection.
     **
     ******************************************************************************/
    typedef enum en_adc_awdmd
    {
        AdcAwdCmpMode_0 = 0x0, ///< Upper limit is AWDDR0, lower limit is AWDDR1.
                               ///< If AWDDR0 > result or result > AWDDR1,
                               ///< the interrupt will be occur.

        AdcAwdCmpMode_1 = 0x1, ///< The range is [AWDDR0, AWDDR1].
                               ///< If AWDDR0 <= result <= AWDDR1, the interrupt will be occur.
    } en_adc_awdmd_t;

    /**
     *******************************************************************************
     ** \brief  Analog watchdog enable/disable.
     **
     ******************************************************************************/
    typedef enum en_adc_awden
    {
        AdcAwd_Disable = 0x0, ///< Disable AWD.
        AdcAwd_Enable = 0x1,  ///< Enable AWD.
    } en_adc_awden_t;

    /**
     *******************************************************************************
     ** \brief  PGA control.
     **
     ******************************************************************************/
    typedef enum en_adc_pga_ctl
    {
        AdcPgaCtl_Invalid = 0x0, ///< Amplifier is invalid.
        AdcPgaCtl_Amplify = 0xE, ///< Amplifier effective.
    } en_adc_pga_ctl_t;

    /**
     *******************************************************************************
     ** \brief  The amplification factor of the amplifier is as follows.
     **
     ******************************************************************************/
    typedef enum en_adc_pga_factor
    {
        AdcPgaFactor_2 = 0x0,      ///< PGA magnification 2.
        AdcPgaFactor_2P133 = 0x1,  ///< PGA magnification 2.133.
        AdcPgaFactor_2P286 = 0x2,  ///< PGA magnification 2.286.
        AdcPgaFactor_2P667 = 0x3,  ///< PGA magnification 2.667.
        AdcPgaFactor_2P909 = 0x4,  ///< PGA magnification 2.909.
        AdcPgaFactor_3P2 = 0x5,    ///< PGA magnification 3.2.
        AdcPgaFactor_3P556 = 0x6,  ///< PGA magnification 3.556.
        AdcPgaFactor_4 = 0x7,      ///< PGA magnification 4.
        AdcPgaFactor_4P571 = 0x8,  ///< PGA magnification 4.571.
        AdcPgaFactor_5P333 = 0x9,  ///< PGA magnification 5.333.
        AdcPgaFactor_6P4 = 0xA,    ///< PGA magnification 6.4.
        AdcPgaFactor_8 = 0xB,      ///< PGA magnification 8.
        AdcPgaFactor_10P667 = 0xC, ///< PGA magnification 10.667.
        AdcPgaFactor_16 = 0xD,     ///< PGA magnification 16.
        AdcPgaFactor_32 = 0xE,     ///< PGA magnification 32.
    } en_adc_pga_factor_t;

    /**
     *******************************************************************************
     ** \brief  Negative phase input selection
     **
     ******************************************************************************/
    typedef enum en_adc_pga_negative
    {
        AdcPgaNegative_PGAVSS = 0x0, ///< Use external port PGAVSS as PGA negative input.
        AdcPgaNegative_VSSA = 0x1,   ///< Use internal analog ground VSSA as PGA negative input.
    } en_adc_pga_negative_t;

    /**
     *******************************************************************************
     ** \brief Structure definition of ADC
     **
     ******************************************************************************/
    typedef struct stc_adc_ch_cfg
    {
        uint32_t u32Channel;  ///< ADC channels mask.
        uint8_t u8Sequence;   ///< The sequence which the channel(s) belong to.
        uint8_t *pu8SampTime; ///< Pointer to sampling time.
    } stc_adc_ch_cfg_t;

    typedef struct stc_adc_awd_cfg
    {
        en_adc_awdmd_t enAwdmd; ///< Comparison mode of the values.
        en_adc_awdss_t enAwdss; ///< Interrupt output select.
        uint16_t u16AwdDr0;     ///< Your range DR0.
        uint16_t u16AwdDr1;     ///< Your range DR1.
    } stc_adc_awd_cfg_t;

    typedef struct stc_adc_trg_cfg
    {
        uint8_t u8Sequence;       ///< The sequence will be configured trigger source.
        en_adc_trgsel_t enTrgSel; ///< Trigger source type.
        en_event_src_t enInTrg0;  ///< Internal trigger 0 source number
                                  ///< (event number @ref en_event_src_t).
        en_event_src_t enInTrg1;  ///< Internal trigger 1 source number
                                  ///< (event number @ref en_event_src_t).
    } stc_adc_trg_cfg_t;

    typedef struct stc_adc_init
    {
        en_adc_resolution_t enResolution; ///< ADC resolution 12bit/10bit/8bit.
        en_adc_data_align_t enDataAlign;  ///< ADC data alignment.
        en_adc_clren_t enAutoClear;       ///< Automatically clear data register.
                                          ///< after reading data register(enable/disable).
        en_adc_scan_mode_t enScanMode;    ///< ADC scan mode.
        en_adc_rschsel_t enRschsel;       ///< Restart or continue.
    } stc_adc_init_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief ADC sequence definition.
 **
 ******************************************************************************/
/* ADC sequence definition */
#define ADC_SEQ_A ((uint8_t)0)
#define ADC_SEQ_B ((uint8_t)1)

/* ADC pin definition */
#define ADC1_IN0 ((uint8_t)0)
#define ADC1_IN1 ((uint8_t)1)
#define ADC1_IN2 ((uint8_t)2)
#define ADC1_IN3 ((uint8_t)3)
#define ADC12_IN4 ((uint8_t)4)
#define ADC12_IN5 ((uint8_t)5)
#define ADC12_IN6 ((uint8_t)6)
#define ADC12_IN7 ((uint8_t)7)
#define ADC12_IN8 ((uint8_t)8)
#define ADC12_IN9 ((uint8_t)9)
#define ADC12_IN10 ((uint8_t)10)
#define ADC12_IN11 ((uint8_t)11)
#define ADC1_IN12 ((uint8_t)12)
#define ADC1_IN13 ((uint8_t)13)
#define ADC1_IN14 ((uint8_t)14)
#define ADC1_IN15 ((uint8_t)15)
#define ADC_PIN_INVALID ((uint8_t)0xFF)

/* ADC channel index definition */
#define ADC_CH_IDX0 (0u)
#define ADC_CH_IDX1 (1u)
#define ADC_CH_IDX2 (2u)
#define ADC_CH_IDX3 (3u)
#define ADC_CH_IDX4 (4u)
#define ADC_CH_IDX5 (5u)
#define ADC_CH_IDX6 (6u)
#define ADC_CH_IDX7 (7u)
#define ADC_CH_IDX8 (8u)
#define ADC_CH_IDX9 (9u)
#define ADC_CH_IDX10 (10u)
#define ADC_CH_IDX11 (11u)
#define ADC_CH_IDX12 (12u)
#define ADC_CH_IDX13 (13u)
#define ADC_CH_IDX14 (14u)
#define ADC_CH_IDX15 (15u)
#define ADC_CH_IDX16 (16u)

/* ADC1 channel mask definition */
#define ADC1_CH0 (0x1ul << 0u)   ///< Default mapping pin ADC1_IN0
#define ADC1_CH1 (0x1ul << 1u)   ///< Default mapping pin ADC1_IN1
#define ADC1_CH2 (0x1ul << 2u)   ///< Default mapping pin ADC1_IN2
#define ADC1_CH3 (0x1ul << 3u)   ///< Default mapping pin ADC1_IN3
#define ADC1_CH4 (0x1ul << 4u)   ///< Default mapping pin ADC12_IN4
#define ADC1_CH5 (0x1ul << 5u)   ///< Default mapping pin ADC12_IN5
#define ADC1_CH6 (0x1ul << 6u)   ///< Default mapping pin ADC12_IN6
#define ADC1_CH7 (0x1ul << 7u)   ///< Default mapping pin ADC12_IN7
#define ADC1_CH8 (0x1ul << 8u)   ///< Default mapping pin ADC12_IN8
#define ADC1_CH9 (0x1ul << 9u)   ///< Default mapping pin ADC12_IN9
#define ADC1_CH10 (0x1ul << 10u) ///< Default mapping pin ADC12_IN10
#define ADC1_CH11 (0x1ul << 11u) ///< Default mapping pin ADC12_IN11
#define ADC1_CH12 (0x1ul << 12u) ///< Default mapping pin ADC12_IN12
#define ADC1_CH13 (0x1ul << 13u) ///< Default mapping pin ADC12_IN13
#define ADC1_CH14 (0x1ul << 14u) ///< Default mapping pin ADC12_IN14
#define ADC1_CH15 (0x1ul << 15u) ///< Default mapping pin ADC12_IN15
#define ADC1_CH16 (0x1ul << 16u)
#define ADC1_CH_INTERNAL (ADC1_CH16) ///< 8bit DAC_1/DAC_2 or internal VERF, dependent on CMP_RVADC
#define ADC1_CH_ALL (0x0001FFFFul)
#define ADC1_PIN_MASK_ALL (ADC1_CH_ALL & ~ADC1_CH_INTERNAL)

/* ADC2 channel definition */
#define ADC2_CH0 (0x1ul << 0u) ///< Default mapping pin ADC12_IN4
#define ADC2_CH1 (0x1ul << 1u) ///< Default mapping pin ADC12_IN5
#define ADC2_CH2 (0x1ul << 2u) ///< Default mapping pin ADC12_IN6
#define ADC2_CH3 (0x1ul << 3u) ///< Default mapping pin ADC12_IN7
#define ADC2_CH4 (0x1ul << 4u) ///< Default mapping pin ADC12_IN8
#define ADC2_CH5 (0x1ul << 5u) ///< Default mapping pin ADC12_IN9
#define ADC2_CH6 (0x1ul << 6u) ///< Default mapping pin ADC12_IN10
#define ADC2_CH7 (0x1ul << 7u) ///< Default mapping pin ADC12_IN11
#define ADC2_CH8 (0x1ul << 8u)
#define ADC2_CH_INTERNAL (ADC2_CH8) ///< 8bit DAC_1/DAC_2 or internal VERF, dependent on CMP_RVADC
#define ADC2_CH_ALL (0x000001FFul)
#define ADC2_PIN_MASK_ALL (ADC2_CH_ALL & ~ADC2_CH_INTERNAL)

/*
 * PGA channel definition.
 * NOTE: The PGA channel directly maps external pins and does not correspond to the ADC channel.
 */
#define PGA_CH0 (0x1ul << ADC1_IN0)  ///< Mapping pin ADC1_IN0
#define PGA_CH1 (0x1ul << ADC1_IN1)  ///< Mapping pin ADC1_IN1
#define PGA_CH2 (0x1ul << ADC1_IN2)  ///< Mapping pin ADC1_IN2
#define PGA_CH3 (0x1ul << ADC1_IN3)  ///< Mapping pin ADC1_IN3
#define PGA_CH4 (0x1ul << ADC12_IN4) ///< Mapping pin ADC12_IN4
#define PGA_CH5 (0x1ul << ADC12_IN5) ///< Mapping pin ADC12_IN5
#define PGA_CH6 (0x1ul << ADC12_IN6) ///< Mapping pin ADC12_IN6
#define PGA_CH7 (0x1ul << ADC12_IN7) ///< Mapping pin ADC12_IN7
#define PGA_CH8 (0x1ul << ADC12_IN8) ///< Mapping internal 8bit DAC1 output
#define PGA_CH_ALL (0x000001FFul)

/* ADC1 has up to 17 channels */
#define ADC1_CH_COUNT (17u)

/* ADC2 has up to 9 channels */
#define ADC2_CH_COUNT (9u)

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t ADC_Init(M4_ADC_TypeDef *ADCx, const stc_adc_init_t *pstcInit);
    en_result_t ADC_DeInit(M4_ADC_TypeDef *ADCx);

    en_result_t ADC_SetScanMode(M4_ADC_TypeDef *ADCx, en_adc_scan_mode_t enMode);
    en_result_t ADC_ConfigTriggerSrc(M4_ADC_TypeDef *ADCx, const stc_adc_trg_cfg_t *pstcTrgCfg);
    en_result_t ADC_TriggerSrcCmd(M4_ADC_TypeDef *ADCx, uint8_t u8Seq, en_functional_state_t enState);

    en_result_t ADC_AddAdcChannel(M4_ADC_TypeDef *ADCx, const stc_adc_ch_cfg_t *pstcChCfg);
    en_result_t ADC_DelAdcChannel(M4_ADC_TypeDef *ADCx, const stc_adc_ch_cfg_t *pstcChCfg);
    en_result_t ADC_SeqITCmd(M4_ADC_TypeDef *ADCx, uint8_t u8Seq, en_functional_state_t enState);

    en_result_t ADC_ConfigAvg(M4_ADC_TypeDef *ADCx, en_adc_avcnt_t enAvgCnt);
    en_result_t ADC_AddAvgChannel(M4_ADC_TypeDef *ADCx, uint32_t u32Channel);
    en_result_t ADC_DelAvgChannel(M4_ADC_TypeDef *ADCx, uint32_t u32Channel);

    en_result_t ADC_ConfigAwd(M4_ADC_TypeDef *ADCx, const stc_adc_awd_cfg_t *pstcAwdCfg);
    en_result_t ADC_AwdCmd(M4_ADC_TypeDef *ADCx, en_functional_state_t enState);
    en_result_t ADC_AwdITCmd(M4_ADC_TypeDef *ADCx, en_functional_state_t enState);
    en_result_t ADC_AddAwdChannel(M4_ADC_TypeDef *ADCx, uint32_t u32Channel);
    en_result_t ADC_DelAwdChannel(M4_ADC_TypeDef *ADCx, uint32_t u32Channel);

    void ADC_ConfigPga(en_adc_pga_factor_t enFactor, en_adc_pga_negative_t enNegativeIn);
    void ADC_PgaCmd(en_functional_state_t enState);
    void ADC_AddPgaChannel(uint32_t u32Channel);
    void ADC_DelPgaChannel(uint32_t u32Channel);

    void ADC_ConfigSync(en_adc_sync_mode_t enMode, uint8_t u8TrgDelay);
    void ADC_SyncCmd(en_functional_state_t enState);

    en_result_t ADC_StartConvert(M4_ADC_TypeDef *ADCx);
    en_result_t ADC_StopConvert(M4_ADC_TypeDef *ADCx);
    en_flag_status_t ADC_GetEocFlag(const M4_ADC_TypeDef *ADCx, uint8_t u8Seq);
    void ADC_ClrEocFlag(M4_ADC_TypeDef *ADCx, uint8_t u8Seq);
    en_result_t ADC_PollingSa(M4_ADC_TypeDef *ADCx, uint16_t *pu16AdcData, uint8_t u8Length, uint32_t u32Timeout);

    en_result_t ADC_GetAllData(const M4_ADC_TypeDef *ADCx, uint16_t *pu16AdcData, uint8_t u8Length);
    en_result_t ADC_GetChData(const M4_ADC_TypeDef *ADCx, uint32_t u32TargetCh, uint16_t *pu16AdcData, uint8_t u8Length);
    uint16_t ADC_GetValue(const M4_ADC_TypeDef *ADCx, uint8_t u8ChIndex);

    uint32_t ADC_GetAwdFlag(const M4_ADC_TypeDef *ADCx);
    void ADC_ClrAwdFlag(M4_ADC_TypeDef *ADCx);
    void ADC_ClrAwdChFlag(M4_ADC_TypeDef *ADCx, uint32_t u32AwdCh);

    en_result_t ADC_ChannleRemap(M4_ADC_TypeDef *ADCx, uint32_t u32DestChannel, uint8_t u8AdcPin);
    uint8_t ADC_GetChannelPinNum(const M4_ADC_TypeDef *ADCx, uint8_t u8ChIndex);

    //@} // AdcGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_ADC_ENABLE */

#endif /* __HC32F46X_ADC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
