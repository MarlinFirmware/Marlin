/******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
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
/** \file hc32f46x_exint_nmi_swi.h
 **
 ** A detailed description is available at
 ** @link ExintNmiSwiGroup Exint/Nmi/Swi description @endlink
 **
 **   - 2018-10-17  1.0  Zhangxl First version for Device Driver Library of
 **                      exint, Nmi, SW interrupt
 **
 ******************************************************************************/

#ifndef __HC32F46X_EXINT_NMI_SWI_H__
#define __HC32F46X_EXINT_NMI_SWI_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_EXINT_NMI_SWI_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup ExintNmiSwiGroup External Interrupts (External Interrupt),       \
     ** NMI (Non-Maskable Interrupt), SWI (Software Interrupt)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief Enumeration to filter clock setting for EXINT and NMI
     **
     ** \note
     ******************************************************************************/
    typedef enum en_ei_flt_clk
    {
        Pclk3Div1 = 0u,  ///< PCLK3 as EP filter clock source
        Pclk3Div8 = 1u,  ///< PCLK3 div8 as EP filter clock source
        Pclk3Div32 = 2u, ///< PCLK3 div32 as EP filter clock source
        Pclk3Div64 = 3u, ///< PCLK3 div64 as EP filter clock source
    } en_ei_flt_clk_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to NMI detection
     **
     ** \note
     ******************************************************************************/
    typedef enum en_nmi_lvl
    {
        NmiFallingEdge = 0u, ///< Falling edge detection
        NmiRisingEdge = 1u,  ///< Rising edge detection
    } en_nmi_lvl_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to EXTI detection
     **
     ** \note
     ******************************************************************************/
    typedef enum en_exti_lvl
    {
        ExIntFallingEdge = 0u, ///< Falling edge detection
        ExIntRisingEdge = 1u,  ///< Rising edge detection
        ExIntBothEdge = 2u,    ///< Falling or Rising edge detection
        ExIntLowLevel = 3u,    ///< "L" level detection
    } en_exti_lvl_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to define an index for EXINT
     ******************************************************************************/
    typedef enum en_exti_ch
    {
        ExtiCh00 = 0u,
        ExtiCh01 = 1u,
        ExtiCh02 = 2u,
        ExtiCh03 = 3u,
        ExtiCh04 = 4u,
        ExtiCh05 = 5u,
        ExtiCh06 = 6u,
        ExtiCh07 = 7u,
        ExtiCh08 = 8u,
        ExtiCh09 = 9u,
        ExtiCh10 = 10u,
        ExtiCh11 = 11u,
        ExtiCh12 = 12u,
        ExtiCh13 = 13u,
        ExtiCh14 = 14u,
        ExtiCh15 = 15u,
    } en_exti_ch_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to define the SWI channel
     ******************************************************************************/
    typedef enum en_swi_ch
    {
        SwiCh00 = 1u << 0,
        SwiCh01 = 1u << 1,
        SwiCh02 = 1u << 2,
        SwiCh03 = 1u << 3,
        SwiCh04 = 1u << 4,
        SwiCh05 = 1u << 5,
        SwiCh06 = 1u << 6,
        SwiCh07 = 1u << 7,
        SwiCh08 = 1u << 8,
        SwiCh09 = 1u << 9,
        SwiCh10 = 1u << 10,
        SwiCh11 = 1u << 11,
        SwiCh12 = 1u << 12,
        SwiCh13 = 1u << 13,
        SwiCh14 = 1u << 14,
        SwiCh15 = 1u << 15,
        SwiCh16 = 1u << 16,
        SwiCh17 = 1u << 17,
        SwiCh18 = 1u << 18,
        SwiCh19 = 1u << 19,
        SwiCh20 = 1u << 20,
        SwiCh21 = 1u << 21,
        SwiCh22 = 1u << 22,
        SwiCh23 = 1u << 23,
        SwiCh24 = 1u << 24,
        SwiCh25 = 1u << 25,
        SwiCh26 = 1u << 26,
        SwiCh27 = 1u << 27,
        SwiCh28 = 1u << 28,
        SwiCh29 = 1u << 29,
        SwiCh30 = 1u << 30,
        SwiCh31 = 1u << 31,
    } en_swi_ch_t;

    /**
     *******************************************************************************
     ** \brief External Interrupt configuration
     **
     ** \note The EXINT configuration
     ******************************************************************************/
    typedef struct stc_exint_config
    {
        en_exti_ch_t enExitCh;            ///< External Int CH.0~15 ref@ en_exti_ch_t
        en_functional_state_t enFilterEn; ///< TRUE: Enable filter function
        en_ei_flt_clk_t enFltClk;         ///< Filter clock, ref@ en_ei_flt_clk_t for details
        en_exti_lvl_t enExtiLvl;          ///< Detection level, ref@ en_exti_lvl_t for details
    } stc_exint_config_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to NMI Trigger source
     **
     ** \note
     ******************************************************************************/
    typedef enum en_nmi_src
    {
        NmiSrcNmi = 1u << 0,      ///< NMI pin
        NmiSrcSwdt = 1u << 1,     ///< Special watch dog timer
        NmiSrcVdu1 = 1u << 2,     ///< Voltage detect 1
        NmiSrcVdu2 = 1u << 3,     ///< Voltage detect 2
        NmiSrcXtalStop = 1u << 5, ///< Xtal stop
        NmiSrcSramPE = 1u << 8,   ///< SRAM1/2/HS/Ret parity error
        NmiSrcSramDE = 1u << 9,   ///< SRAM3 ECC error
        NmiSrcMpu = 1u << 10,     ///< MPU error
        NmiSrcWdt = 1u << 11,     ///< Watch dog timer
    } en_nmi_src_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to software interrupt or event
     **
     ** \note
     ******************************************************************************/
    typedef enum en_swi_type
    {
        SwEvent = 0u, ///< software event
        SwInt = 1u,   ///< software interrupt
    } en_swi_type_t;

    /**
     *******************************************************************************
     ** \brief NMI configuration
     **
     ** \note The NMI configuration
     ******************************************************************************/
    typedef struct stc_nmi_config
    {
        en_functional_state_t enFilterEn; ///< TRUE: Enable filter function
        en_ei_flt_clk_t enFilterClk;      ///< Filter clock, ref@ en_flt_clk_t for details
        en_nmi_lvl_t enNmiLvl;            ///< Detection level, ref@ en_nmi_lvl_t for details
        uint16_t u16NmiSrc;               ///< NMI trigger source, ref@ en_nmi_src_t for details
        func_ptr_t pfnNmiCallback;        ///< Callback pointers
    } stc_nmi_config_t;

    /**
     *******************************************************************************
     ** \brief SWI configuration
     **
     ** \note The SWI configuration
     ******************************************************************************/
    typedef struct stc_swi_config
    {
        en_swi_ch_t enSwiCh;       ///< SWI channel
        en_swi_type_t enSwiType;   ///< Select software interrupt or event
        func_ptr_t pfnSwiCallback; ///< Callback pointers
    } stc_swi_config_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    extern en_result_t EXINT_Init(const stc_exint_config_t *pstcExtiConfig);
    extern en_int_status_t EXINT_IrqFlgGet(en_exti_ch_t enExint);
    extern en_result_t EXINT_IrqFlgClr(en_exti_ch_t enExint);
    extern en_result_t NMI_Init(const stc_nmi_config_t *pstcNmiConfig);
    extern en_result_t NMI_DeInit(void);
    extern en_int_status_t NMI_IrqFlgGet(en_nmi_src_t enNmiSrc);
    extern en_result_t NMI_IrqFlgClr(uint16_t u16NmiSrc);
    extern en_result_t SWI_Enable(uint32_t u32SwiCh);
    extern en_result_t SWI_Disable(uint32_t u32SwiCh);

    //@} // ExintNmiSwiGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_EXINT_NMI_SWI_ENABLE */

#endif /* __HC32F46X_EXINT_NMI_SWI_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
