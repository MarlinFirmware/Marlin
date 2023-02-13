/*****************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd. ("HDSC").
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
/** \file hc32f46x_clk.h
 **
 ** A detailed description is available at
 ** @link CmuGroup Clock description @endlink
 **
 **   - 2018-10-13  1.0  Chengy First version for Device Driver Library of CMU.
 **
 ******************************************************************************/
#ifndef __HC32F46X_CLK_H__
#define __HC32F46X_CLK_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_CLK_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup CmuGroup Clock Manage Unit(CMU)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/

    /**
     *******************************************************************************
     ** \brief  The system clock source.
     **
     ******************************************************************************/
    typedef enum en_clk_sys_source
    {
        ClkSysSrcHRC = 0u,    ///< The system clock source is HRC.
        ClkSysSrcMRC = 1u,    ///< The system clock source is MRC.
        ClkSysSrcLRC = 2u,    ///< The system clock source is LRC.
        ClkSysSrcXTAL = 3u,   ///< The system clock source is XTAL.
        ClkSysSrcXTAL32 = 4u, ///< The system clock source is XTAL32.
        CLKSysSrcMPLL = 5u,   ///< The system clock source is MPLL.
    } en_clk_sys_source_t;

    /**
     *******************************************************************************
     ** \brief  The pll clock source.
     **
     ******************************************************************************/
    typedef enum en_clk_pll_source
    {
        ClkPllSrcXTAL = 0u, ///< The pll clock source is XTAL.
        ClkPllSrcHRC = 1u,  ///< The pll clock source is HRC.
    } en_clk_pll_source_t;

    /**
     *******************************************************************************
     ** \brief  The usb clock source.
     **
     ******************************************************************************/
    typedef enum en_clk_usb_source
    {
        ClkUsbSrcSysDiv2 = 2u, ///< The usb clock source is 1/2 system clock.
        ClkUsbSrcSysDiv3 = 3u, ///< The usb clock source is 1/3 system clock.
        ClkUsbSrcSysDiv4 = 4u, ///< The usb clock source is 1/4 system clock.
        ClkUsbSrcMpllp = 8u,   ///< The usb clock source is MPLLP.
        ClkUsbSrcMpllq = 9u,   ///< The usb clock source is MPLLQ.
        ClkUsbSrcMpllr = 10u,  ///< The usb clock source is MPLLR.
        ClkUsbSrcUpllp = 11u,  ///< The usb clock source is UPLLP.
        ClkUsbSrcUpllq = 12u,  ///< The usb clock source is UPLLQ.
        ClkUsbSrcUpllr = 13u,  ///< The usb clock source is UPLLR.
    } en_clk_usb_source_t;

    /**
     *******************************************************************************
     ** \brief  The peripheral(adc/trng/I2S) clock source.
     **
     ******************************************************************************/
    typedef enum en_clk_peri_source
    {
        ClkPeriSrcPclk = 0u,   ///< The peripheral(adc/trng/I2S) clock source is division from system clock.
        ClkPeriSrcMpllp = 8u,  ///< The peripheral(adc/trng/I2S) clock source is MPLLP.
        ClkPeriSrcMpllq = 9u,  ///< The peripheral(adc/trng/I2S) clock source is MPLLQ.
        ClkPeriSrcMpllr = 10u, ///< The peripheral(adc/trng/I2S) clock source is MPLLR.
        ClkPeriSrcUpllp = 11u, ///< The peripheral(adc/trng/I2S) clock source is UPLLP.
        ClkPeriSrcUpllq = 12u, ///< The peripheral(adc/trng/I2S) clock source is UPLLQ.
        ClkPeriSrcUpllr = 13u, ///< The peripheral(adc/trng/I2S) clock source is UPLLR.
    } en_clk_peri_source_t;

    /**
     *******************************************************************************
     ** \brief  The clock output source.
     **
     ******************************************************************************/
    typedef enum en_clk_output_source
    {
        ClkOutputSrcHrc = 0u,     ///< The clock output source is HRC
        ClkOutputSrcMrc = 1u,     ///< The clock output source is MRC.
        ClkOutputSrcLrc = 2u,     ///< The clock output source is LRC.
        ClkOutputSrcXtal = 3u,    ///< The clock output source is XTAL.
        ClkOutputSrcXtal32 = 4u,  ///< The clock output source is XTAL32
        ClkOutputSrcMpllp = 6u,   ///< The clock output source is MPLLP.
        ClkOutputSrcUpllp = 7u,   ///< The clock output source is UPLLP.
        ClkOutputSrcMpllq = 8u,   ///< The clock output source is MPLLQ.
        ClkOutputSrcUpllq = 9u,   ///< The clock output source is UPLLQ.
        ClkOutputSrcSysclk = 11u, ///< The clock output source is system clock.
    } en_clk_output_source_t;

    /**
     *******************************************************************************
     ** \brief  The clock frequency source for measure or reference.
     **
     ******************************************************************************/
    typedef enum en_clk_fcm_source
    {
        ClkFcmSrcXtal = 0u,   ///< The clock frequency measure or reference source is XTAL
        ClkFcmSrcXtal32 = 1u, ///< The clock frequency measure or reference source is XTAL32.
        ClkFcmSrcHrc = 2u,    ///< The clock frequency measure or reference source is HRC.
        ClkFcmSrcLrc = 3u,    ///< The clock frequency measure or reference source is LRC.
        ClkFcmSrcSwdtrc = 4u, ///< The clock frequency measure or reference source is SWDTRC
        ClkFcmSrcPclk1 = 5u,  ///< The clock frequency measure or reference source is PCLK1.
        ClkFcmSrcUpllp = 6u,  ///< The clock frequency measure or reference source is UPLLP.
        ClkFcmSrcMrc = 7u,    ///< The clock frequency measure or reference source is MRC.
        ClkFcmSrcMpllp = 8u,  ///< The clock frequency measure or reference source is MPLLP.
        ClkFcmSrcRtcLrc = 9u, ///< The clock frequency measure or reference source is RTCLRC.
    } en_clk_fcm_intref_source_t,
        en_clk_fcm_measure_source_t;

    /**
     *******************************************************************************
     ** \brief  The clock flag status.
     **
     ******************************************************************************/
    typedef enum en_clk_flag
    {
        ClkFlagHRCRdy = 0u,       ///< The clock flag is HRC ready.
        ClkFlagXTALRdy = 1u,      ///< The clock flag is XTAL ready.
        ClkFlagMPLLRdy = 2u,      ///< The clock flag is MPLL ready.
        ClkFlagUPLLRdy = 3u,      ///< The clock flag is UPLL ready.
        ClkFlagXTALStoppage = 4u, ///< The clock flag is XTAL stoppage.
    } en_clk_flag_t;

    /**
     *******************************************************************************
     ** \brief  The clock frequency measure flag status.
     **
     ******************************************************************************/
    typedef enum en_clk_fcm_flag
    {
        ClkFcmFlagErrf = 0u,  ///< The clock frequency flag is frequency abnormal.
        ClkFcmFlagMendf = 1u, ///< The clock frequency flag is end of measurement.
        ClkFcmFlagOvf = 2u,   ///< The clock frequency flag is counter overflow.
    } en_clk_fcm_flag_t;

    /**
     *******************************************************************************
     ** \brief  The source of xtal.
     **
     ******************************************************************************/
    typedef enum en_clk_xtal_mode
    {
        ClkXtalModeOsc = 0u,    ///< Use external high speed osc as source.
        ClkXtalModeExtClk = 1u, ///< Use external clk as source.
    } en_clk_xtal_mode_t;

    /**
     *******************************************************************************
     ** \brief  The drive capability of xtal.
     **
     ******************************************************************************/
    typedef enum en_clk_xtal_drv
    {
        ClkXtalHighDrv = 0u, ///< High drive capability.20MHz~24MHz.
        ClkXtalMidDrv = 1u,  ///< Middle drive capability.16MHz~20MHz.
        ClkXtalLowDrv = 2u,  ///< Low drive capability.8MHz~16MHz.
        ClkXtalTinyDrv = 3u, ///< Tiny drive capability.8MHz.
    } en_clk_xtal_drv_t;

    /**
     *******************************************************************************
     ** \brief  The stable time of XTAL.
     **
     ** \note   It depends on SUPDRV bit.
     ******************************************************************************/
    typedef enum en_clk_xtal_stb_cycle
    {
        ClkXtalStbCycle35 = 1u,   ///< stable time is 35(36) cycle.
        ClkXtalStbCycle67 = 2u,   ///< stable time is 67(68) cycle.
        ClkXtalStbCycle131 = 3u,  ///< stable time is 131(132) cycle.
        ClkXtalStbCycle259 = 4u,  ///< stable time is 259(260) cycle.
        ClkXtalStbCycle547 = 5u,  ///< stable time is 547(548) cycle.
        ClkXtalStbCycle1059 = 6u, ///< stable time is 1059(1060) cycle.
        ClkXtalStbCycle2147 = 7u, ///< stable time is 2147(2148) cycle.
        ClkXtalStbCycle4291 = 8u, ///< stable time is 4291(4292) cycle.
        ClkXtalStbCycle8163 = 9u, ///< stable time is 8163(8164) cycle.
    } en_clk_xtal_stb_cycle_t;

    /**
     *******************************************************************************
     ** \brief  The handle of xtal stoppage.
     **
     ******************************************************************************/
    typedef enum en_clk_xtal_stp_mode
    {
        ClkXtalStpModeInt = 0u,   ///< The handle of stoppage is interrupt.
        ClkXtalStpModeReset = 1u, ///< The handle of stoppage is reset.
    } en_clk_xtal_stp_mode_t;

    /**
     *******************************************************************************
     ** \brief  The drive capability of xtal32.
     **
     ******************************************************************************/
    typedef enum en_clk_xtal32_drv
    {
        ClkXtal32MidDrv = 0u,  ///< Middle drive capability.32.768KHz.
        ClkXtal32HighDrv = 1u, ///< High drive capability.32.768KHz.
        ClkXtal32LowDrv = 2u,  ///< Low drive capability.32.768KHz.
        ClkXtal32TinyDrv = 3u, ///< Tiny drive capability.32.768KHz.
    } en_clk_xtal32_drv_t;

    /**
     *******************************************************************************
     ** \brief  The filter mode of xtal32.
     **
     ******************************************************************************/
    typedef enum en_clk_xtal32_filter_mode
    {
        ClkXtal32FilterModeFull = 0u, ///< Valid in run,stop,power down mode.
        ClkXtal32FilterModePart = 2u, ///< Valid in run mode.
        ClkXtal32FilterModeNone = 3u, ///< Invalid in run,stop,power down mode.
    } en_clk_xtal32_filter_mode_t;

    /**
     *******************************************************************************
     ** \brief  The division factor of system clock.
     **
     ******************************************************************************/
    typedef enum en_clk_sysclk_div_factor
    {
        ClkSysclkDiv1 = 0u,  ///< 1 division.
        ClkSysclkDiv2 = 1u,  ///< 2 division.
        ClkSysclkDiv4 = 2u,  ///< 4 division.
        ClkSysclkDiv8 = 3u,  ///< 8 division.
        ClkSysclkDiv16 = 4u, ///< 16 division.
        ClkSysclkDiv32 = 5u, ///< 32 division.
        ClkSysclkDiv64 = 6u, ///< 64 division.
    } en_clk_sysclk_div_factor_t;

    /**
     *******************************************************************************
     ** \brief  The division factor of system clock.It will be used for debug clock.
     **
     ******************************************************************************/
    typedef enum en_clk_tpiuclk_div_factor
    {
        ClkTpiuclkDiv1 = 0u, ///< 1 division.
        ClkTpiuclkDiv2 = 1u, ///< 2 division.
        ClkTpiuclkDiv4 = 2u, ///< 4 division.
    } en_clk_tpiuclk_div_factor_t;

    /**
     *******************************************************************************
     ** \brief  The division factor of clock output.
     **
     ******************************************************************************/
    typedef enum en_clk_output_div_factor
    {
        ClkOutputDiv1 = 0u,   ///< 1 division.
        ClkOutputDiv2 = 1u,   ///< 2 division.
        ClkOutputDiv4 = 2u,   ///< 4 division.
        ClkOutputDiv8 = 3u,   ///< 8 division.
        ClkOutputDiv16 = 4u,  ///< 16 division.
        ClkOutputDiv32 = 5u,  ///< 32 division.
        ClkOutputDiv64 = 6u,  ///< 64 division.
        ClkOutputDiv128 = 7u, ///< 128 division.
    } en_clk_output_div_factor_t;

    /**
     *******************************************************************************
     ** \brief  The division factor of fcm measure source.
     **
     ******************************************************************************/
    typedef enum en_clk_fcm_measure_div_factor
    {
        ClkFcmMeaDiv1 = 0u,  ///< 1 division.
        ClkFcmMeaDiv4 = 1u,  ///< 4 division.
        ClkFcmMeaDiv8 = 2u,  ///< 8 division.
        ClkFcmMeaDiv32 = 3u, ///< 32 division.
    } en_clk_fcm_measure_div_factor_t;

    /**
     *******************************************************************************
     ** \brief  The division factor of fcm reference source.
     **
     ******************************************************************************/
    typedef enum en_clk_fcm_intref_div_factor
    {
        ClkFcmIntrefDiv32 = 0u,   ///< 32 division.
        ClkFcmIntrefDiv128 = 1u,  ///< 128 division.
        ClkFcmIntrefDiv1024 = 2u, ///< 1024 division.
        ClkFcmIntrefDiv8192 = 3u, ///< 8192 division.
    } en_clk_fcm_intref_div_factor_t;

    /**
     *******************************************************************************
     ** \brief  The edge of the fcm reference source.
     **
     ******************************************************************************/
    typedef enum en_clk_fcm_edge
    {
        ClkFcmEdgeRising = 0u,  ///< Rising edge.
        ClkFcmEdgeFalling = 1u, ///< Falling edge.
        ClkFcmEdgeBoth = 2u,    ///< Both edge.
    } en_clk_fcm_edge_t;

    /**
     *******************************************************************************
     ** \brief  The filter clock of the fcm reference source.
     **
     ******************************************************************************/
    typedef enum en_clk_fcm_filter_clk
    {
        ClkFcmFilterClkNone = 0u,        ///< None filter.
        ClkFcmFilterClkFcmSrc = 1u,      ///< Use fcm measurement source as filter clock.
        ClkFcmFilterClkFcmSrcDiv4 = 2u,  ///< Use 1/4 fcm measurement source as filter clock.
        ClkFcmFilterClkFcmSrcDiv16 = 3u, ///< Use 1/16 fcm measurement source as filter clock.
    } en_clk_fcm_filter_clk_t;

    /**
     *******************************************************************************
     ** \brief  The fcm reference source.
     **
     ******************************************************************************/
    typedef enum en_clk_fcm_refer
    {
        ClkFcmExtRef = 0u,   ///< Use external reference.
        ClkFcmInterRef = 1u, ///< Use internal reference.
    } en_clk_fcm_refer_t;

    /**
     *******************************************************************************
     ** \brief  The handle of fcm abnormal.
     **
     ******************************************************************************/
    typedef enum en_clk_fcm_abnormal_handle
    {
        ClkFcmHandleInterrupt = 0u, ///< The handle of fcm abnormal is interrupt.
        ClkFcmHandleReset = 1u,     ///< The handle of fcm abnormal is reset.
    } en_clk_fcm_abnormal_handle_t;

    /**
     *******************************************************************************
     ** \brief  The channel of clock output.
     **
     ******************************************************************************/
    typedef enum en_clk_output_ch
    {
        ClkOutputCh1 = 1u, ///< The output of clk is MCO_1.
        ClkOutputCh2 = 2u, ///< The output of clk is MCO_2.
    } en_clk_output_ch_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of XTAL.
     **
     ** \note   Configures the XTAL if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_xtal_cfg
    {
        en_functional_state_t enFastStartup; ///< Enable fast start up or not.
        en_clk_xtal_mode_t enMode;           ///< Select xtal mode.
        en_clk_xtal_drv_t enDrv;             ///< Select xtal drive capability.
    } stc_clk_xtal_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of XTAL stoppage.
     **
     ** \note   Configures the XTAL stoppage if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_xtal_stp_cfg
    {
        en_functional_state_t enDetect;    ///< Enable detect stoppage or not.
        en_clk_xtal_stp_mode_t enMode;     ///< Select the handle of xtal stoppage.
        en_functional_state_t enModeReset; ///< Enable reset for handle the xtal stoppage.
        en_functional_state_t enModeInt;   ///< Enable interrupt for handle the xtal stoppage.
    } stc_clk_xtal_stp_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of XTAL32.
     **
     ** \note   Configures the XTAL32 if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_xtal32_cfg
    {
        en_functional_state_t enFastStartup;      ///< Enable fast start up or not.
        en_clk_xtal32_drv_t enDrv;                ///< Select xtal32 drive capability.
        en_clk_xtal32_filter_mode_t enFilterMode; ///< The filter mode of xtal32.
    } stc_clk_xtal32_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of PLL.
     **
     ** \note   Configures the PLL if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_pll_cfg
    {
        uint32_t PllpDiv; ///< Pllp clk, division factor of VCO out.
        uint32_t PllqDiv; ///< Pllq clk, division factor of VCO out.
        uint32_t PllrDiv; ///< Pllr clk, division factor of VCO out.
        uint32_t plln;    ///< Multiplication factor of vco out, ensure between 240M~480M
        uint32_t pllmDiv; ///< Division factor of VCO in, ensure between 1M~12M.
    } stc_clk_mpll_cfg_t, stc_clk_upll_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of system clock.
     **
     ** \note   Configures the system clock if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_sysclk_cfg
    {
        en_clk_sysclk_div_factor_t enHclkDiv;  ///< Division for hclk.
        en_clk_sysclk_div_factor_t enExclkDiv; ///< Division for exclk.
        en_clk_sysclk_div_factor_t enPclk0Div; ///< Division for pclk0.
        en_clk_sysclk_div_factor_t enPclk1Div; ///< Division for pclk1.
        en_clk_sysclk_div_factor_t enPclk2Div; ///< Division for pclk2.
        en_clk_sysclk_div_factor_t enPclk3Div; ///< Division for pclk3.
        en_clk_sysclk_div_factor_t enPclk4Div; ///< Division for pclk4.
    } stc_clk_sysclk_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of clock output.
     **
     ** \note   Configures the clock output if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_output_cfg
    {
        en_clk_output_source_t enOutputSrc;     ///< The clock output source.
        en_clk_output_div_factor_t enOutputDiv; ///< The division factor of clock output source.
    } stc_clk_output_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of fcm window.
     **
     ** \note   Configures the fcm window if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_fcm_window_cfg
    {
        uint16_t windowLower; ///< The lower value of the window.
        uint16_t windowUpper; ///< The upper value of the window.
    } stc_clk_fcm_window_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of fcm measurement.
     **
     ** \note   Configures the fcm measurement if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_fcm_measure_cfg
    {
        en_clk_fcm_measure_source_t enSrc;        ///< The measurement source.
        en_clk_fcm_measure_div_factor_t enSrcDiv; ///< The division factor of measurement source.
    } stc_clk_fcm_measure_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of fcm reference.
     **
     ** \note   Configures the fcm reference if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_fcm_reference_cfg
    {
        en_functional_state_t enExtRef;             ///< Enable external reference or not.
        en_clk_fcm_edge_t enEdge;                   ///< The edge of internal reference.
        en_clk_fcm_filter_clk_t enFilterClk;        ///< The filter clock of internal reference.
        en_clk_fcm_refer_t enRefSel;                ///< Select reference.
        en_clk_fcm_intref_source_t enIntRefSrc;     ///< Select internal reference.
        en_clk_fcm_intref_div_factor_t enIntRefDiv; ///< The division factor of internal reference.
    } stc_clk_fcm_reference_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of fcm interrupt.
     **
     ** \note   Configures the fcm interrupt if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_fcm_interrupt_cfg
    {
        en_clk_fcm_abnormal_handle_t enHandleSel; ///< Use interrupt or reset.
        en_functional_state_t enHandleReset;      ///< Enable reset or not.
        en_functional_state_t enHandleInterrupt;  ///< Enable interrupt or not.
        en_functional_state_t enOvfInterrupt;     ///< Enable overflow interrupt or not.
        en_functional_state_t enEndInterrupt;     ///< Enable measurement end interrupt or not.
    } stc_clk_fcm_interrupt_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of fcm.
     **
     ** \note   Configures the fcm if needed.
     **
     ******************************************************************************/
    typedef struct stc_clk_fcm_cfg
    {
        stc_clk_fcm_window_cfg_t *pstcFcmWindowCfg; ///< Window configuration struct.
        stc_clk_fcm_measure_cfg_t *pstcFcmMeaCfg;   ///< Measurement configuration struct.
        stc_clk_fcm_reference_cfg_t *pstcFcmRefCfg; ///< Reference configuration struct.
        stc_clk_fcm_interrupt_cfg_t *pstcFcmIntCfg; ///< Interrupt configuration struct.
    } stc_clk_fcm_cfg_t;

    /**
     *******************************************************************************
     ** \brief  Clock frequency structure.
     **
     ******************************************************************************/
    typedef struct stc_clk_freq
    {
        uint32_t sysclkFreq; ///< System clock frequency.
        uint32_t hclkFreq;   ///< Hclk frequency.
        uint32_t exckFreq;   ///< Exclk frequency.
        uint32_t pclk0Freq;  ///< Pclk0 frequency.
        uint32_t pclk1Freq;  ///< Pclk1 frequency.
        uint32_t pclk2Freq;  ///< Pclk2 frequency.
        uint32_t pclk3Freq;  ///< Pclk3 frequency.
        uint32_t pclk4Freq;  ///< Pclk4 frequency.
    } stc_clk_freq_t;

    /**
     *******************************************************************************
     ** \brief  PLL Clock frequency structure.
     **
     ******************************************************************************/
    typedef struct stc_pll_clk_freq
    {
        uint32_t mpllp; ///< mpllp clock frequency.
        uint32_t mpllq; ///< mpllq clock frequency.
        uint32_t mpllr; ///< mpllr clock frequency.
        uint32_t upllp; ///< upllp clock frequency.
        uint32_t upllq; ///< upllq clock frequency.
        uint32_t upllr; ///< upllr clock frequency.
    } stc_pll_clk_freq_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    void CLK_XtalConfig(const stc_clk_xtal_cfg_t *pstcXtalCfg);
    void CLK_XtalStbConfig(const en_clk_xtal_stb_cycle_t enXtalStb);
    void CLK_XtalStpConfig(const stc_clk_xtal_stp_cfg_t *pstcXtalStpCfg);
    en_result_t CLK_XtalCmd(en_functional_state_t enNewState);

    void CLK_Xtal32Config(const stc_clk_xtal32_cfg_t *pstcXtal32Cfg);
    en_result_t CLK_Xtal32Cmd(en_functional_state_t enNewState);

    void CLK_HrcTrim(int8_t trimValue);
    en_result_t CLK_HrcCmd(en_functional_state_t enNewState);

    void CLK_MrcTrim(int8_t trimValue);
    en_result_t CLK_MrcCmd(en_functional_state_t enNewState);

    void CLK_LrcTrim(int8_t trimValue);
    en_result_t CLK_LrcCmd(en_functional_state_t enNewState);

    void CLK_SetPllSource(en_clk_pll_source_t enPllSrc);
    void CLK_MpllConfig(const stc_clk_mpll_cfg_t *pstcMpllCfg);
    en_result_t CLK_MpllCmd(en_functional_state_t enNewState);

    void CLK_UpllConfig(const stc_clk_upll_cfg_t *pstcUpllCfg);
    en_result_t CLK_UpllCmd(en_functional_state_t enNewState);

    void CLK_SetSysClkSource(en_clk_sys_source_t enTargetSysSrc);
    en_clk_sys_source_t CLK_GetSysClkSource(void);

    void CLK_SysClkConfig(const stc_clk_sysclk_cfg_t *pstcSysclkCfg);
    void CLK_GetClockFreq(stc_clk_freq_t *pstcClkFreq);
    void CLK_GetPllClockFreq(stc_pll_clk_freq_t *pstcPllClkFreq);

    void CLK_SetUsbClkSource(en_clk_usb_source_t enTargetUsbSrc);
    void CLK_SetPeriClkSource(en_clk_peri_source_t enTargetPeriSrc);
    void CLK_SetI2sClkSource(const M4_I2S_TypeDef *pstcI2sReg, en_clk_peri_source_t enTargetPeriSrc);
    en_clk_peri_source_t CLK_GetI2sClkSource(const M4_I2S_TypeDef *pstcI2sReg);

    void CLK_TpiuClkConfig(const en_clk_tpiuclk_div_factor_t enTpiuDiv);
    void CLK_TpiuClkCmd(en_functional_state_t enNewState);

    void CLK_OutputClkConfig(en_clk_output_ch_t enCh, const stc_clk_output_cfg_t *pstcOutputCfg);
    void CLK_OutputClkCmd(en_clk_output_ch_t enCh, en_functional_state_t enNewState);
    en_flag_status_t CLK_GetFlagStatus(en_clk_flag_t enClkFlag);

    void CLK_FcmConfig(const stc_clk_fcm_cfg_t *pstcClkFcmCfg);
    void CLK_FcmCmd(en_functional_state_t enNewState);

    uint16_t CLK_GetFcmCounter(void);
    en_flag_status_t CLK_GetFcmFlag(en_clk_fcm_flag_t enFcmFlag);
    void CLK_ClearFcmFlag(en_clk_fcm_flag_t enFcmFlag);

    void CLK_ClearXtalStdFlag(void);

    //@} // CmuGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_CLK_ENABLE */

#endif /* __HC32F46X_CLK_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
