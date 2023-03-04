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
/** \file hc32f46x_cmp.h
 **
 ** A detailed description is available at
 ** @link CmpGroup CMP @endlink
 **
 **   - 2018-10-22  1.0  Pangw First version for Device Driver Library of CMP.
 **
 ******************************************************************************/
#ifndef __HC32F46x_CMP_H__
#define __HC32F46x_CMP_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_CMP_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup CmpGroup Comparator(CMP)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief CMP function enumeration
     ******************************************************************************/
    typedef enum en_cmp_func
    {
        CmpVcoutOutput = (1u << 12), ///< CMP vcout output enable function
        CmpOutpuInv = (1u << 13),    ///< CMP output invert enable function
        CmpOutput = (1u << 14),      ///< CMP output enable function
    } en_cmp_func_t;

    /**
     *******************************************************************************
     ** \brief CMP edge selection enumeration
     ******************************************************************************/
    typedef enum en_cmp_edge_sel
    {
        CmpNoneEdge = 0u,     ///< None edge detection
        CmpRisingEdge = 1u,   ///< Rising edge detection
        CmpFaillingEdge = 2u, ///< Falling edge detection
        CmpBothEdge = 3u,     ///< Falling or Rising edge detection
    } en_cmp_edge_sel_t;

    /**
     *******************************************************************************
     ** \brief CMP filter sample clock division enumeration
     ******************************************************************************/
    typedef enum en_cmp_fltclk_div
    {
        CmpNoneFlt = 0u,       ///< Unuse filter
        CmpFltPclk3Div1 = 1u,  ///< PCLK3/1
        CmpFltPclk3Div2 = 2u,  ///< PCLK3/2
        CmpFltPclk3Div4 = 3u,  ///< PCLK3/4
        CmpFltPclk3Div8 = 4u,  ///< PCLK3/8
        CmpFltPclk3Div16 = 5u, ///< PCLK3/16
        CmpFltPclk3Div32 = 6u, ///< PCLK3/32
        CmpFltPclk3Div64 = 7u, ///< PCLK3/64
    } en_cmp_fltclk_div_t;

    /**
     *******************************************************************************
     ** \brief CMP INP4 input enumeration
     ******************************************************************************/
    typedef enum en_cmp_inp4_sel
    {
        CmpInp4None = 0u,      ///< None input
        CmpInp4PGAO = 1u,      ///< PGAO output
        CmpInp4PGAO_BP = 2u,   ///< PGAO_BP output
        CmpInp4CMP1_INP4 = 4u, ///< CMP1_INP4
    } en_cmp_inp4_sel_t;

    /**
     *******************************************************************************
     ** \brief CMP INP input enumeration
     ******************************************************************************/
    typedef enum en_cmp_inp_sel
    {
        CmpInpNone = 0u,              ///< None input
        CmpInp1 = 1u,                 ///< INP1 input
        CmpInp2 = 2u,                 ///< INP2 input
        CmpInp1_Inp2 = 3u,            ///< INP1 INP2 input
        CmpInp3 = 4u,                 ///< INP3 input
        CmpInp1_Inp3 = 5u,            ///< INP1 INP3 input
        CmpInp2_Inp3 = 6u,            ///< INP2 INP3 input
        CmpInp1_Inp2_Inp3 = 7u,       ///< INP1 INP2 INP3 input
        CmpInp4 = 8u,                 ///< INP4 input
        CmpInp1_Inp4 = 9u,            ///< INP1 INP4 input
        CmpInp2_Inp4 = 10u,           ///< INP2 INP4 input
        CmpInp1_Inp2_Inp4 = 11u,      ///< INP1 INP2 INP4 input
        CmpInp3_Inp4 = 12u,           ///< INP3 INP4 input
        CmpInp1_Inp3_Inp4 = 13u,      ///< INP1 INP3 INP4 input
        CmpInp2_Inp3_Inp4 = 14u,      ///< INP2 INP3 INP4 input
        CmpInp1_Inp2_Inp3_Inp4 = 15u, ///< INP1 INP2 INP3 INP4 input
    } en_cmp_inp_sel_t;

    /**
     *******************************************************************************
     ** \brief CMP INM input enumeration
     ******************************************************************************/
    typedef enum en_cmp_inm_sel
    {
        CmpInmNone = 0u, ///< None input
        CmpInm1 = 1u,    ///< INM1 input
        CmpInm2 = 2u,    ///< INM2 input
        CmpInm3 = 4u,    ///< INM3 input
        CmpInm4 = 8u,    ///< INM4 input
    } en_cmp_inm_sel_t;

    /**
     *******************************************************************************
     ** \brief CMP INP State enumeration (read only)
     ******************************************************************************/
    typedef enum en_cmp_inp_state
    {
        CmpInpNoneState = 0u, ///< none input state
        CmpInp1State = 1u,    ///< INP1 input state
        CmpInp2State = 2u,    ///< INP2 input state
        CmpInp3State = 4u,    ///< INP3 input state
        CmpInp4State = 8u,    ///< INP4 input state
    } en_cmp_inp_state_t;

    /**
     *******************************************************************************
     ** \brief CMP Output State enumeration (read only)
     ******************************************************************************/
    typedef enum en_cmp_output_state
    {
        CmpOutputLow = 0u,  ///< Compare output Low "0"
        CmpOutputHigh = 1u, ///< Compare output High "1"
    } en_cmp_output_state_t;

    /**
     *******************************************************************************
     ** \brief CMP input selection
     ******************************************************************************/
    typedef struct stc_cmp_input_sel
    {
        en_cmp_inm_sel_t enInmSel; ///< CMP INM sel

        en_cmp_inp_sel_t enInpSel; ///< CMP INP sel

        en_cmp_inp4_sel_t enInp4Sel; ///< CMP INP4 sel
    } stc_cmp_input_sel_t;

    /**
     ******************************************************************************
     ** \brief DAC channel
     ******************************************************************************/
    typedef enum en_cmp_dac_ch
    {
        CmpDac1 = 0u, ///< DAC1
        CmpDac2 = 1u, ///< DAC2
    } en_cmp_dac_ch_t;

    /**
     ******************************************************************************
     ** \brief ADC internal reference voltage path
     ******************************************************************************/
    typedef enum en_cmp_adc_int_ref_volt_path
    {
        CmpAdcRefVoltPathDac1 = (1u << 0u), ///< ADC internal reference voltage path: DAC1
        CmpAdcRefVoltPathDac2 = (1u << 1u), ///< ADC internal reference voltage path: DAC2
        CmpAdcRefVoltPathVref = (1u << 4u), ///< ADC internal reference voltage path: VREF
    } en_cmp_adc_int_ref_volt_path_t;

    /**
     *******************************************************************************
     ** \brief CMP initialization structure definition
     ******************************************************************************/
    typedef struct stc_cmp_init
    {
        en_cmp_edge_sel_t enEdgeSel; ///< CMP edge sel

        en_cmp_fltclk_div_t enFltClkDiv; ///< CMP FLTclock division

        en_functional_state_t enCmpOutputEn; ///< CMP Output enable

        en_functional_state_t enCmpVcoutOutputEn; ///< CMP output result enable

        en_functional_state_t enCmpInvEn; ///< CMP INV sel for output

        en_functional_state_t enCmpIntEN; ///< CMP interrupt enable
    } stc_cmp_init_t;

    /**
     *******************************************************************************
     ** \brief CMP DAC initialization structure definition
     ******************************************************************************/
    typedef struct stc_cmp_dac_init
    {
        uint8_t u8DacData; ///< CMP DAC Data register value

        en_functional_state_t enCmpDacEN; ///< CMP DAC enable
    } stc_cmp_dac_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t CMP_Init(M4_CMP_TypeDef *CMPx, const stc_cmp_init_t *pstcInitCfg);
    en_result_t CMP_DeInit(M4_CMP_TypeDef *CMPx);
    en_result_t CMP_Cmd(M4_CMP_TypeDef *CMPx, en_functional_state_t enCmd);
    en_result_t CMP_IrqCmd(M4_CMP_TypeDef *CMPx, en_functional_state_t enCmd);
    en_result_t CMP_SetScanTime(M4_CMP_TypeDef *CMPx,
                                uint8_t u8ScanStable,
                                uint8_t u8ScanPeriod);
    en_result_t CMP_FuncCmd(M4_CMP_TypeDef *CMPx,
                            en_cmp_func_t enFunc,
                            en_functional_state_t enCmd);
    en_result_t CMP_StartScan(M4_CMP_TypeDef *CMPx);
    en_result_t CMP_StopScan(M4_CMP_TypeDef *CMPx);
    en_result_t CMP_SetFilterClkDiv(M4_CMP_TypeDef *CMPx,
                                    en_cmp_fltclk_div_t enFltClkDiv);
    en_cmp_fltclk_div_t CMP_GetFilterClkDiv(M4_CMP_TypeDef *CMPx);
    en_result_t CMP_SetEdgeSel(M4_CMP_TypeDef *CMPx,
                               en_cmp_edge_sel_t enEdgeSel);
    en_cmp_edge_sel_t CMP_GetEdgeSel(M4_CMP_TypeDef *CMPx);
    en_result_t CMP_InputSel(M4_CMP_TypeDef *CMPx,
                             const stc_cmp_input_sel_t *pstcInputSel);
    en_result_t CMP_SetInp(M4_CMP_TypeDef *CMPx, en_cmp_inp_sel_t enInputSel);
    en_cmp_inp_sel_t CMP_GetInp(M4_CMP_TypeDef *CMPx);
    en_result_t CMP_SetInm(M4_CMP_TypeDef *CMPx, en_cmp_inm_sel_t enInputSel);
    en_cmp_inm_sel_t CMP_GetInm(M4_CMP_TypeDef *CMPx);
    en_result_t CMP_SetInp4(M4_CMP_TypeDef *CMPx, en_cmp_inp4_sel_t enInputSel);
    en_cmp_inp4_sel_t CMP_GetInp4(M4_CMP_TypeDef *CMPx);
    en_cmp_output_state_t CMP_GetOutputState(M4_CMP_TypeDef *CMPx);
    en_cmp_inp_state_t CMP_GetInpState(M4_CMP_TypeDef *CMPx);
    en_result_t CMP_DAC_Init(en_cmp_dac_ch_t enCh,
                             const stc_cmp_dac_init_t *pstcInitCfg);
    en_result_t CMP_DAC_DeInit(en_cmp_dac_ch_t enCh);
    en_result_t CMP_DAC_Cmd(en_cmp_dac_ch_t enCh, en_functional_state_t enCmd);
    en_result_t CMP_DAC_SetData(en_cmp_dac_ch_t enCh, uint8_t u8DacData);
    uint8_t CMP_DAC_GetData(en_cmp_dac_ch_t enCh);
    en_result_t CMP_ADC_SetRefVoltPath(en_cmp_adc_int_ref_volt_path_t enRefVoltPath);

    //@} // CmpGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_CMP_ENABLE */

#endif /* __HC32F46x_CMP_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
