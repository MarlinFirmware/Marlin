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
/** \file hc32f46x_cmp.c
 **
 ** A detailed description is available at
 ** @link CmpGroup CMP @endlink
 **
 **   - 2018-10-22  1.0  Pangw First version for Device Driver Library of CMP.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_cmp.h"
#include "hc32f46x_utility.h"

#if (DDL_CMP_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup CmpGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Parameter valid check for CMP Instances. */
#define IS_VALID_CMP(__CMPx__)  \
    ((M4_CMP1 == (__CMPx__)) || \
     (M4_CMP2 == (__CMPx__)) || \
     (M4_CMP3 == (__CMPx__)))

/*!< Parameter valid check for CMP function */
#define IS_VALID_CMP_FUNCTION(x) \
    ((CmpOutput == (x)) ||       \
     (CmpOutpuInv == (x)) ||     \
     (CmpVcoutOutput == (x)))

/*! Parameter validity check for edge sel. */
#define IS_VALID_EDGESEL(x)    \
    ((CmpNoneEdge == (x)) ||   \
     (CmpBothEdge == (x)) ||   \
     (CmpRisingEdge == (x)) || \
     (CmpFaillingEdge == (x)))

/*!< Parameter CMP FLT validity check for clock division. */
#define IS_VALID_FLTCLK_DIVISION(x) \
    ((CmpFltPclk3Div1 == (x)) ||    \
     (CmpFltPclk3Div2 == (x)) ||    \
     (CmpFltPclk3Div4 == (x)) ||    \
     (CmpFltPclk3Div8 == (x)) ||    \
     (CmpFltPclk3Div16 == (x)) ||   \
     (CmpFltPclk3Div32 == (x)) ||   \
     (CmpFltPclk3Div64 == (x)))

/*!< Parameter validity check for INP4 SEL. */
#define IS_VALID_INP4SEL(x)     \
    ((CmpInp4None == (x)) ||    \
     (CmpInp4PGAO == (x)) ||    \
     (CmpInp4PGAO_BP == (x)) || \
     (CmpInp4CMP1_INP4 == (x)))

/*!< Parameter validity check for INP INPUT SEL. */
#define IS_VALID_INPSEL(x)         \
    ((CmpInpNone == (x)) ||        \
     (CmpInp1 == (x)) ||           \
     (CmpInp2 == (x)) ||           \
     (CmpInp3 == (x)) ||           \
     (CmpInp4 == (x)) ||           \
     (CmpInp1_Inp2 == (x)) ||      \
     (CmpInp1_Inp3 == (x)) ||      \
     (CmpInp2_Inp3 == (x)) ||      \
     (CmpInp1_Inp4 == (x)) ||      \
     (CmpInp2_Inp4 == (x)) ||      \
     (CmpInp3_Inp4 == (x)) ||      \
     (CmpInp1_Inp2_Inp3 == (x)) || \
     (CmpInp1_Inp2_Inp4 == (x)) || \
     (CmpInp1_Inp3_Inp4 == (x)) || \
     (CmpInp2_Inp3_Inp4 == (x)) || \
     (CmpInp1_Inp2_Inp3_Inp4 == (x)))

/*!< Parameter validity check for INM INPUT SEL. */
#define IS_VALID_INMSEL(x) \
    ((CmpInm1 == (x)) ||   \
     (CmpInm2 == (x)) ||   \
     (CmpInm3 == (x)) ||   \
     (CmpInm4 == (x)) ||   \
     (CmpInmNone == (x)))

/*!< Parameter validity check for CMP_CR channel. */
#define IS_VALID_CMP_CR_CH(x) \
    ((CmpDac1 == (x)) ||      \
     (CmpDac2 == (x)))

/*!< Parameter validity check for ADC internal reference voltage path. */
#define IS_VALID_ADC_REF_VOLT_PATH(x)  \
    ((CmpAdcRefVoltPathDac1 == (x)) || \
     (CmpAdcRefVoltPathDac2 == (x)) || \
     (CmpAdcRefVoltPathVref == (x)))

/*!< RVADC Write Protection Key. */
#define RVADC_WRITE_PROT_KEY (0x5500u)

/*!< Timer4x ECER register address. */
#define CMP_CR_DADRx(__DACx__) \
    ((CmpDac1 == (__DACx__)) ? &M4_CMP_CR->DADR1 : &M4_CMP_CR->DADR2)

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
 ** \brief Initializes the specified CMP.
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] pstcInitCfg             Pointer to CMP configure structure
 ** \arg This parameter detail refer @ref stc_cmp_init_t
 **
 ** \retval Ok                          CMP is initialized normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - CMPx is invalid
 **                                     - pstcInitCfg == NULL
 **
 ******************************************************************************/
en_result_t CMP_Init(M4_CMP_TypeDef *CMPx, const stc_cmp_init_t *pstcInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx && pstcInitCfg pointer */
    if ((IS_VALID_CMP(CMPx)) && (NULL != pstcInitCfg))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_EDGESEL(pstcInitCfg->enEdgeSel));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpIntEN));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpInvEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpOutputEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpVcoutOutputEn));
        DDL_ASSERT(IS_VALID_FLTCLK_DIVISION(pstcInitCfg->enFltClkDiv));

        /* De-Initialize CMP */
        CMPx->CTRL = (uint16_t)0x0000u;
        CMPx->VLTSEL = (uint16_t)0x0000u;
        CMPx->CVSSTB = (uint16_t)0x0005u;
        CMPx->CVSPRD = (uint16_t)0x000Fu;

        CMPx->CTRL_f.IEN = (uint16_t)pstcInitCfg->enCmpIntEN;
        CMPx->CTRL_f.INV = (uint16_t)pstcInitCfg->enCmpInvEn;
        CMPx->CTRL_f.EDGSL = (uint16_t)pstcInitCfg->enEdgeSel;
        CMPx->CTRL_f.FLTSL = (uint16_t)pstcInitCfg->enFltClkDiv;
        CMPx->CTRL_f.CMPOE = (uint16_t)pstcInitCfg->enCmpOutputEn;
        CMPx->CTRL_f.OUTEN = (uint16_t)pstcInitCfg->enCmpVcoutOutputEn;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Initialize CMP
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ******************************************************************************/
en_result_t CMP_DeInit(M4_CMP_TypeDef *CMPx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->CTRL = (uint16_t)0x0000u;
        CMPx->VLTSEL = (uint16_t)0x0000u;
        CMPx->CVSSTB = (uint16_t)0x0005u;
        CMPx->CVSPRD = (uint16_t)0x000Fu;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable CMP working
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] enCmd                   The CMP function state
 ** \arg Disable                        Disable CMP working
 ** \arg Enable                         Enable CMP working
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ******************************************************************************/
en_result_t CMP_Cmd(M4_CMP_TypeDef *CMPx, en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        CMPx->CTRL_f.CMPON = (uint16_t)(enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable CMP interrupt request
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] enCmd                   The CMP interrupt function state
 ** \arg Disable                        Disable interrupt request
 ** \arg Enable                         Enable interrupt request
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid.
 **
 ******************************************************************************/
en_result_t CMP_IrqCmd(M4_CMP_TypeDef *CMPx, en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->CTRL_f.IEN = (uint16_t)(enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set scan time(scan stable&&period)
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] u8ScanStable            CMP scan stable value
 ** \arg u8ScanStable < 16
 ** \param [in] u8ScanPeriod            CMP scan period value
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ** \note   u8ScanStable && u8ScanPeriod value must meet following condition:
 **             u8ScanPeriod > u8ScanStable + FLTSL_DIV*4 + 5
 **             FLTSL_DIV is filter sample period division(refer CMPx->CTRL_f.FLTSL)
 **
 ******************************************************************************/
en_result_t CMP_SetScanTime(M4_CMP_TypeDef *CMPx,
                            uint8_t u8ScanStable,
                            uint8_t u8ScanPeriod)
{
    uint16_t u16Flts;
    uint16_t u16FltslDiv;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if ((!IS_VALID_CMP(CMPx)) || (u8ScanStable & 0xF0u))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        u16Flts = CMPx->CTRL_f.FLTSL;
        u16FltslDiv = ((uint16_t)1u << (u16Flts - 1u));

        if ((0u != u16Flts) &&
            (u8ScanPeriod <= (u8ScanStable + u16FltslDiv * 4u + 5u)))
        {
            enRet = ErrorInvalidParameter;
        }
        else
        {
            CMPx->CVSSTB_f.STB = u8ScanStable;
            CMPx->CVSPRD_f.PRD = u8ScanPeriod;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable the specified CMP function.
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] enFunc                  CMP function selection
 ** \arg CmpVcoutOutput                 CMP Vcout output enable function
 ** \arg CmpOutpuInv                    CMP output invert enable function
 ** \arg CmpOutput                      CMP output enable function
 ** \param [in] enCmd                   CMP functional state
 ** \arg Enable                         Enable the specified CMP function
 ** \arg Disable                        Disable the specified CMP function
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ******************************************************************************/
en_result_t CMP_FuncCmd(M4_CMP_TypeDef *CMPx,
                        en_cmp_func_t enFunc,
                        en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));
        DDL_ASSERT(IS_VALID_CMP_FUNCTION(enFunc));

        if (Enable == enCmd)
        {
            CMPx->CTRL |= (uint16_t)enFunc;
        }
        else
        {
            CMPx->CTRL &= (uint16_t)(~((uint16_t)enFunc));
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Start CMP scan
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval Ok                          Start successfully
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ******************************************************************************/
en_result_t CMP_StartScan(M4_CMP_TypeDef *CMPx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->CTRL_f.CVSEN = (uint16_t)1u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Stop CMP scan
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval Ok                          Stop successfully
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ******************************************************************************/
en_result_t CMP_StopScan(M4_CMP_TypeDef *CMPx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->CTRL_f.CVSEN = (uint16_t)0u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set CMP filter sample clock division.
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] enFltClkDiv             The CMP filter sample clock division selection
 ** \arg CmpNoneFlt                     Unuse filter
 ** \arg CmpFltPclk3Div1                PCLK3/1
 ** \arg CmpFltPclk3Div2                PCLK3/2
 ** \arg CmpFltPclk3Div4                PCLK3/4
 ** \arg CmpFltPclk3Div8                PCLK3/8
 ** \arg CmpFltPclk3Div16               PCLK3/16
 ** \arg CmpFltPclk3Div32               PCLK3/32
 ** \arg CmpFltPclk3Div64               PCLK3/64
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ******************************************************************************/
en_result_t CMP_SetFilterClkDiv(M4_CMP_TypeDef *CMPx,
                                en_cmp_fltclk_div_t enFltClkDiv)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_FLTCLK_DIVISION(enFltClkDiv));
        CMPx->CTRL_f.FLTSL = (uint16_t)enFltClkDiv;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get CMP filter sample clock division.
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval CmpNoneFlt                  Unuse filter
 ** \retval CmpFltPclk3Div1             PCLK3/1
 ** \retval CmpFltPclk3Div2             PCLK3/2
 ** \retval CmpFltPclk3Div4             PCLK3/4
 ** \retval CmpFltPclk3Div8             PCLK3/8
 ** \retval CmpFltPclk3Div16            PCLK3/16
 ** \retval CmpFltPclk3Div32            PCLK3/32
 ** \retval CmpFltPclk3Div64            PCLK3/64
 **
 ******************************************************************************/
en_cmp_fltclk_div_t CMP_GetFilterClkDiv(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_fltclk_div_t)CMPx->CTRL_f.FLTSL;
}

/**
 *******************************************************************************
 ** \brief Set CMP detection edge selection.
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] enEdgeSel               The CMP detection edge selection
 ** \arg CmpNoneEdge                    None edge detection
 ** \arg CmpRisingEdge                  Rising edge detection
 ** \arg CmpFaillingEdge                Falling edge detection
 ** \arg CmpBothEdge                    Falling or Rising edge detection
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ******************************************************************************/
en_result_t CMP_SetEdgeSel(M4_CMP_TypeDef *CMPx,
                           en_cmp_edge_sel_t enEdgeSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_EDGESEL(enEdgeSel));
        CMPx->CTRL_f.EDGSL = (uint16_t)enEdgeSel;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get CMP detection edge selection.
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval CmpNoneEdge                 None edge detection
 ** \retval CmpRisingEdge               Rising edge detection
 ** \retval CmpFaillingEdge             Falling edge detection
 ** \retval CmpBothEdge                 Falling or Rising edge detection
 **
 ******************************************************************************/
en_cmp_edge_sel_t CMP_GetEdgeSel(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_edge_sel_t)CMPx->CTRL_f.EDGSL;
}

/**
 *******************************************************************************
 ** \brief Set CMP input sel
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] pstcInputSel            The CMP input selection structure
 ** \arg This parameter detail refer @ref stc_cmp_input_sel_t
 **
 ** \retval Ok                          Set successfully
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ******************************************************************************/
en_result_t CMP_InputSel(M4_CMP_TypeDef *CMPx,
                         const stc_cmp_input_sel_t *pstcInputSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx && pstcInputSel pointer */
    if ((IS_VALID_CMP(CMPx)) && (NULL != pstcInputSel))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_INMSEL(pstcInputSel->enInmSel));
        DDL_ASSERT(IS_VALID_INPSEL(pstcInputSel->enInpSel));
        DDL_ASSERT(IS_VALID_INP4SEL(pstcInputSel->enInp4Sel));

        if ((CmpInp4PGAO == pstcInputSel->enInp4Sel) ||
            (CmpInp4PGAO_BP == pstcInputSel->enInp4Sel))
        {
            if (M4_CMP3 != CMPx)
            {
                enRet = Ok;
            }
        }
        else if (CmpInp4CMP1_INP4 == pstcInputSel->enInp4Sel)
        {
            if (M4_CMP1 == CMPx)
            {
                enRet = Ok;
            }
        }
        else
        {
            enRet = Ok;
        }

        if (enRet == Ok)
        {
            CMPx->VLTSEL_f.CVSL = (uint16_t)pstcInputSel->enInpSel;
            CMPx->VLTSEL_f.RVSL = (uint16_t)pstcInputSel->enInmSel;
            CMPx->VLTSEL_f.C4SL = (uint16_t)pstcInputSel->enInp4Sel;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set CMP INP input selection
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] enInputSel              The INP input selection
 ** \arg CmpInpNone                     None input
 ** \arg CmpInp1                        INP1 input
 ** \arg CmpInp2                        INP2 input
 ** \arg CmpInp1_Inp2                   INP1 INP2 input
 ** \arg CmpInp3                        INP3 input
 ** \arg CmpInp1_Inp3                   INP1 INP3 input
 ** \arg CmpInp2_Inp3                   INP2 INP3 input
 ** \arg CmpInp1_Inp2_Inp3              INP1 INP2 INP3 input
 ** \arg CmpInp4                        INP4 input
 ** \arg CmpInp1_Inp4                   INP1 INP4 input
 ** \arg CmpInp2_Inp4                   INP2 INP4 input
 ** \arg CmpInp1_Inp2_Inp4              INP1 INP2 INP4 input
 ** \arg CmpInp3_Inp4                   INP3 INP4 input
 ** \arg CmpInp1_Inp3_Inp4              INP1 INP3 INP4 input
 ** \arg CmpInp2_Inp3_Inp4              INP2 INP3 INP4 input
 ** \arg CmpInp1_Inp2_Inp3_Inp4         INP1 INP2 INP3 INP4 input
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid
 **
 ******************************************************************************/
en_result_t CMP_SetInp(M4_CMP_TypeDef *CMPx, en_cmp_inp_sel_t enInputSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_INPSEL(enInputSel));
        CMPx->VLTSEL_f.CVSL = (uint16_t)enInputSel;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set CMP INP input selection
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval CmpInpNone                  None input
 ** \retval CmpInp1                     INP1 input
 ** \retval CmpInp2                     INP2 input
 ** \retval CmpInp1_Inp2                INP1 INP2 input
 ** \retval CmpInp3                     INP3 input
 ** \retval CmpInp1_Inp3                INP1 INP3 input
 ** \retval CmpInp2_Inp3                INP2 INP3 input
 ** \retval CmpInp1_Inp2_Inp3           INP1 INP2 INP3 input
 ** \retval CmpInp4                     INP4 input
 ** \retval CmpInp1_Inp4                INP1 INP4 input
 ** \retval CmpInp2_Inp4                INP2 INP4 input
 ** \retval CmpInp1_Inp2_Inp4           INP1 INP2 INP4 input
 ** \retval CmpInp3_Inp4                INP3 INP4 input
 ** \retval CmpInp1_Inp3_Inp4           INP1 INP3 INP4 input
 ** \retval CmpInp2_Inp3_Inp4           INP2 INP3 INP4 input
 ** \retval CmpInp1_Inp2_Inp3_Inp4      INP1 INP2 INP3 INP4 input
 **
 ******************************************************************************/
en_cmp_inp_sel_t CMP_GetInp(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_inp_sel_t)CMPx->VLTSEL_f.CVSL;
}

/**
 *******************************************************************************
 ** \brief Set CMP INM input selection
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] enInputSel              The INP input selection
 ** \arg CmpInmNone                     None input
 ** \arg CmpInm1                        INM1 input
 ** \arg CmpInm2                        INM2 input
 ** \arg CmpInm3                        INM3 input
 ** \arg CmpInm4                        INM4 input
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid.
 **
 ******************************************************************************/
en_result_t CMP_SetInm(M4_CMP_TypeDef *CMPx, en_cmp_inm_sel_t enInputSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_INMSEL(enInputSel));
        CMPx->VLTSEL_f.RVSL = (uint16_t)enInputSel;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get CMP INM input selection
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval CmpInmNone                  None input
 ** \retval CmpInm1                     INM1 input
 ** \retval CmpInm2                     INM2 input
 ** \retval CmpInm3                     INM3 input
 ** \retval CmpInm4                     INM4 input
 **
 ******************************************************************************/
en_cmp_inm_sel_t CMP_GetInm(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_inm_sel_t)CMPx->VLTSEL_f.RVSL;
}

/**
 *******************************************************************************
 ** \brief Set CMP INP4 input selection
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 ** \param [in] enInputSel              The INP input selection
 ** \arg CmpInp4None                    None input
 ** \arg CmpInp4PGAO                    PGAO output
 ** \arg CmpInp4PGAO_BP                 PGAO_BP output
 ** \arg CmpInp4CMP1_INP4               CMP1_INP4
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       CMPx is invalid.
 **
 ** \note                               Inp4 Selection is valid only for M4_CMP1
 **                                     and M4_CMP2.
 **
 ******************************************************************************/
en_result_t CMP_SetInp4(M4_CMP_TypeDef *CMPx, en_cmp_inp4_sel_t enInputSel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    DDL_ASSERT(M4_CMP3 != CMPx);
    DDL_ASSERT(IS_VALID_INP4SEL(enInputSel));

    /* Check CMPx pointer */
    if (IS_VALID_CMP(CMPx))
    {
        CMPx->VLTSEL_f.C4SL = (uint16_t)enInputSel;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get CMP INP4 input selection
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval CmpInp4None                 None input
 ** \retval CmpInp4PGAO                 PGAO output
 ** \retval CmpInp4PGAO_BP              PGAO_BP output
 ** \retval CmpInp4CMP1_INP4            CMP1_INP4
 **
 ** \note                               Inp4 Selection is valid only for M4_CMP1
 **                                     and M4_CMP2.
 **
 ******************************************************************************/
en_cmp_inp4_sel_t CMP_GetInp4(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_inp4_sel_t)CMPx->VLTSEL_f.C4SL;
}

/**
 *******************************************************************************
 ** \brief Get CMP output state
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval CmpOutputLow                Compare output Low "0"
 ** \retval CmpOutputHigh               Compare output High "1"
 **
 ******************************************************************************/
en_cmp_output_state_t CMP_GetOutputState(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_output_state_t)(CMPx->MON_f.OMON);
}

/**
 *******************************************************************************
 ** \brief Get CMP INP state
 **
 ** \param [in] CMPx                    Pointer to CMP instance register base
 ** \arg M4_CMP1                        CMP unit 1 instance register base
 ** \arg M4_CMP2                        CMP unit 2 instance register base
 ** \arg M4_CMP3                        CMP unit 3 instance register base
 **
 ** \retval CmpOutputLow                Compare output Low "0"
 ** \retval CmpOutputHigh               Compare output High "1"
 **
 ******************************************************************************/
en_cmp_inp_state_t CMP_GetInpState(M4_CMP_TypeDef *CMPx)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP(CMPx));

    return (en_cmp_inp_state_t)(CMPx->MON_f.CVST);
}

/**
 *******************************************************************************
 ** \brief Initialize CMP DAC
 **
 ** \param [in] enCh                    CMP DAC channel
 ** \arg CmpDac1                        CMP CR DAC channel: DAC1
 ** \arg CmpDac2                        CMP CR DAC channel: DAC2
 ** \param [in] pstcInitCfg             Pointer to CMP DAC configure structure
 ** \arg This parameter detail refer @ref stc_cmp_dac_init_t
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       enCh is invalid.
 **
 ******************************************************************************/
en_result_t CMP_DAC_Init(en_cmp_dac_ch_t enCh,
                         const stc_cmp_dac_init_t *pstcInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((IS_VALID_CMP_CR_CH(enCh)) && (pstcInitCfg != NULL))
    {
        /* Check parameter */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enCmpDacEN));

        M4_CMP_CR->DACR &= (uint16_t)(~(1ul << enCh)); /* Disable DAC */

        *(__IO uint8_t *)CMP_CR_DADRx(enCh) = pstcInitCfg->u8DacData; /* Set DAC data */

        if (Enable == pstcInitCfg->enCmpDacEN)
        {
            M4_CMP_CR->DACR |= (uint16_t)(1ul << enCh); /* Enable DAC */
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Initialize CMP DAC
 **
 ** \param [in] enCh                    CMP DAC channel
 ** \arg CmpDac1                        CMP CR DAC channel: DAC1
 ** \arg CmpDac2                        CMP CR DAC channel: DAC2
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       enCh is invalid.
 **
 ******************************************************************************/
en_result_t CMP_DAC_DeInit(en_cmp_dac_ch_t enCh)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    if (IS_VALID_CMP_CR_CH(enCh))
    {
        M4_CMP_CR->DACR &= (uint16_t)(~(1ul << enCh));
        *(__IO uint8_t *)CMP_CR_DADRx(enCh) = 0u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable CMP DAC working
 **
 ** \param [in] enCh                    CMP DAC channel
 ** \arg CmpDac1                        CMP DAC channel: DAC1
 ** \arg CmpDac2                        CMP DAC channel: DAC2
 ** \param [in] enCmd                   The CMP DAC function state
 ** \arg Disable                        Disable CMP DAC working
 ** \arg Enable                         Enable CMP DAC working
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       enCh is invalid.
 **
 ******************************************************************************/
en_result_t CMP_DAC_Cmd(en_cmp_dac_ch_t enCh, en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    if (IS_VALID_CMP_CR_CH(enCh))
    {
        if (Enable == enCmd)
        {
            M4_CMP_CR->DACR |= (uint16_t)(1ul << enCh);
        }
        else
        {
            M4_CMP_CR->DACR &= (uint16_t)(~(1ul << enCh));
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set DAC data register value
 **
 ** \param [in] enCh                    CMP DAC channel
 ** \arg CmpDac1                        CMP CR DAC channel: DAC1
 ** \arg CmpDac2                        CMP CR DAC channel: DAC2
 ** \param [in]  u8DacData              DAC data register value
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       enCh is invalid.
 **
 ******************************************************************************/
en_result_t CMP_DAC_SetData(en_cmp_dac_ch_t enCh, uint8_t u8DacData)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    if (IS_VALID_CMP_CR_CH(enCh))
    {
        *(__IO uint8_t *)CMP_CR_DADRx(enCh) = u8DacData;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get DAC data register value
 **
 ** \param [in] enCh                    CMP DAC channel
 ** \arg CmpDac1                        CMP CR DAC channel: DAC1
 ** \arg CmpDac2                        CMP CR DAC channel: DAC2
 **
 ** \retval                             DAC data register value
 **
 ******************************************************************************/
uint8_t CMP_DAC_GetData(en_cmp_dac_ch_t enCh)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_CMP_CR_CH(enCh));

    return *(__IO uint8_t *)CMP_CR_DADRx(enCh);
}

/**
 *******************************************************************************
 ** \brief Set ADC internal reference voltage path
 **
 ** \param [in] enRefVoltPath           ADC internal reference voltage path
 ** \arg CmpAdcRefVoltPathDac1          ADC internal reference voltage path: DAC1
 ** \arg CmpAdcRefVoltPathDac2          ADC internal reference voltage path: DAC2
 ** \arg CmpAdcRefVoltPathVref          ADC internal reference voltage path: VREF
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       enRefVoltPath is invalid.
 **
 ******************************************************************************/
en_result_t CMP_ADC_SetRefVoltPath(en_cmp_adc_int_ref_volt_path_t enRefVoltPath)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameter */
    if (IS_VALID_ADC_REF_VOLT_PATH(enRefVoltPath))
    {
        M4_CMP_CR->RVADC = RVADC_WRITE_PROT_KEY; /* Release write protection */
        M4_CMP_CR->RVADC = enRefVoltPath;        /* Set reference voltage path */
        enRet = Ok;
    }

    return enRet;
}

//@} // CmpGroup

#endif /* DDL_CMP_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
