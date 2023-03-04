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
/** \file hc32f46x_dcu.c
 **
 ** A detailed description is available at
 ** @link DcuGroup DCU description @endlink
 **
 **   - 2018-10-15  1.0 Hongjh First version for Device Driver Library of DCU.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_dcu.h"
#include "hc32f46x_utility.h"

#if (DDL_DCU_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup DcuGroup
 ******************************************************************************/

//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Parameter valid check for DCU Instances. */
#define IS_VALID_DCU(__DCUx__)  \
    ((M4_DCU1 == (__DCUx__)) || \
     (M4_DCU2 == (__DCUx__)) || \
     (M4_DCU3 == (__DCUx__)) || \
     (M4_DCU4 == (__DCUx__)))

/*!< Parameter valid check for DCU DATA register. */
#define IS_VALID_DCU_DATA_REG(x)  \
    ((DcuRegisterData0 == (x)) || \
     (DcuRegisterData1 == (x)) || \
     (DcuRegisterData2 == (x)))

/*!< Parameter valid check for DCU operation mode. */
#define IS_VALID_DCU_OPERATION(x) \
    ((DcuOpAdd == (x)) ||         \
     (DcuOpSub == (x)) ||         \
     (DcuInvalid == (x)) ||       \
     (DcuOpCompare == (x)) ||     \
     (DcuHwTrigOpAdd == (x)) ||   \
     (DcuHwTrigOpSub == (x)))

/*!< Parameter valid check for DCU data size. */
#define IS_VALID_DCU_DATAZ_SIZE(x) \
    ((DcuDataBit8 == (x)) ||       \
     (DcuDataBit16 == (x)) ||      \
     (DcuDataBit32 == (x)))

/*!< Parameter valid check for DCU compare trigger mode type. */
#define IS_VALID_DCU_CMP_TRIG_MODE(x) \
    ((DcuCmpTrigbyData0 == (x)) ||    \
     (DcuCmpTrigbyData012 == (x)))

/*!< Parameter valid check for DCU interrupt. */
#define IS_VALID_DCU_INT(x) \
    ((DcuIntOp == (x)) ||   \
     (DcuIntLs2 == (x)) ||  \
     (DcuIntEq2 == (x)) ||  \
     (DcuIntGt2 == (x)) ||  \
     (DcuIntLs1 == (x)) ||  \
     (DcuIntEq1 == (x)) ||  \
     (DcuIntGt1 == (x)))

/*!< Parameter valid check for DCU interrupt mode. */
#define IS_VALID_DCU_INT_WIN_MODE(x) \
    ((DcuIntInvalid == (x)) ||       \
     (DcuWinIntInvalid == (x)) ||    \
     (DcuInsideWinCmpInt == (x)) ||  \
     (DcuOutsideWinCmpInt == (x)))

/*!< Parameter valid check for external trigger event. */
#define IS_VALID_TRG_SRC_EVENT(x)                                  \
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

/*!< Get the specified DATA register address of the specified DCU unit */
#define DCU_DATAx(__DCUx__, __DATAx__) ((uint32_t)(&(__DCUx__)->DATA0) + ((uint32_t)(__DATAx__)) * 4u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static __IO uint32_t *DCU_TRGSELx(const M4_DCU_TypeDef *DCUx);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Initializes a DCU.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] pstcInitCfg             Pointer to DCU configure structure
 ** \arg This parameter detail refer @ref stc_dcu_init_t
 **
 ** \retval Ok                          DCU is initialized normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - DCUx is invalid
 **                                     - pstcInitCfg == NULL
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t DCU_Init(M4_DCU_TypeDef *DCUx, const stc_dcu_init_t *pstcInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx && pstcInitCfg pointer */
    if ((IS_VALID_DCU(DCUx)) && (NULL != pstcInitCfg))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->enIntCmd));
        DDL_ASSERT(IS_VALID_DCU_OPERATION(pstcInitCfg->enOperation));
        DDL_ASSERT(IS_VALID_DCU_DATAZ_SIZE(pstcInitCfg->enDataSize));
        DDL_ASSERT(IS_VALID_DCU_INT_WIN_MODE(pstcInitCfg->enIntWinMode));
        DDL_ASSERT(IS_VALID_DCU_CMP_TRIG_MODE(pstcInitCfg->enCmpTriggerMode));

        /* De-initialize dcu register value */
        DCUx->CTL = 0ul;
        DCUx->INTSEL = 0ul;
        DCUx->FLAGCLR = 0x7Ful;

        /* Set dcu operation mode */
        DCUx->CTL_f.MODE = (uint32_t)pstcInitCfg->enOperation;

        /* Set dcu data sieze */
        DCUx->CTL_f.DATASIZE = (uint32_t)pstcInitCfg->enDataSize;

        /* Set dcu compare trigger mode */
        DCUx->CTL_f.COMP_TRG = (uint32_t)pstcInitCfg->enCmpTriggerMode;

        /* Set dcu interrupt window mode */
        DCUx->INTSEL_f.INT_WIN = (uint32_t)pstcInitCfg->enIntWinMode;

        DCUx->INTSEL = pstcInitCfg->u32IntSel;
        DCUx->CTL_f.INTEN = (uint32_t)(pstcInitCfg->enIntCmd);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Initializes a DCU.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 **
 ** \retval Ok                          De-Initialized successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_DeInit(M4_DCU_TypeDef *DCUx)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint32_t *TRGSELx = DCU_TRGSELx(DCUx);

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* De-initialize dcu register value */
        DCUx->CTL = 0u;
        DCUx->INTSEL = 0u;
        DCUx->FLAGCLR = 0x7Fu;
        *TRGSELx = EVT_MAX;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set DCU operation mode.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enMode                  DCU operation mode
 ** \arg DcuInvalid                     Invalid
 ** \arg DcuOpAdd                       Operation: Add
 ** \arg DcuOpSub                       Operation: Sub
 ** \arg DcuHwTrigOpAdd                 Operation: Hardware trigger Add
 ** \arg DcuHwTrigOpSub                 Operation: Hardware trigger Sub
 ** \arg DcuOpCompare                   Operation: Compare
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_SetOperationMode(M4_DCU_TypeDef *DCUx,
                                 en_dcu_operation_mode_t enMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_DCU_OPERATION(enMode));

        DCUx->CTL_f.MODE = (uint32_t)enMode;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get DCU operation mode.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 **
 ** \retval DcuInvalid                  Invalid
 ** \retval DcuOpAdd                    Operation: Add
 ** \retval DcuOpSub                    Operation: Sub
 ** \retval DcuHwTrigOpAdd              Operation: Hardware trigger Add
 ** \retval DcuHwTrigOpSub              Operation: Hardware trigger Sub
 ** \retval DcuOpCompare                Operation: Compare
 **
 ******************************************************************************/
en_dcu_operation_mode_t DCU_GetOperationMode(M4_DCU_TypeDef *DCUx)
{
    /* Check for DCUx pointer */
    DDL_ASSERT(IS_VALID_DCU(DCUx));

    return (en_dcu_operation_mode_t)DCUx->CTL_f.MODE;
}

/**
 *******************************************************************************
 ** \brief Set DCU data size.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enSize                  DCU data size
 ** \arg DcuDataBit8                    8 bit
 ** \arg DcuDataBit16                   16 bit
 ** \arg DcuDataBit32                   32 bit
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_SetDataSize(M4_DCU_TypeDef *DCUx, en_dcu_data_size_t enSize)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_DCU_DATAZ_SIZE(enSize));

        DCUx->CTL_f.DATASIZE = (uint32_t)enSize;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get DCU data size.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 **
 ** \retval DcuDataBit8                 8 bit
 ** \retval DcuDataBit16                16 bit
 ** \retval DcuDataBit32                32 bit
 **
 ******************************************************************************/
en_dcu_data_size_t DCU_GetDataSize(M4_DCU_TypeDef *DCUx)
{
    /* Check for DCUx pointer */
    DDL_ASSERT(IS_VALID_DCU(DCUx));

    return (en_dcu_data_size_t)(DCUx->CTL_f.DATASIZE);
}

/**
 *******************************************************************************
 ** \brief Set DCU interrup window.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enIntWinMode            Interrupt window mode
 ** \arg DcuIntInvalid                  DCU don't occur interrupt
 ** \arg DcuWinIntInvalid               DCU window interrupt is invalid.
 ** \arg DcuInsideWinCmpInt             DCU occur interrupt when DATA2 �� DATA0 �� DATA2
 ** \arg DcuOutsideWinCmpInt            DCU occur interrupt when DATA0 > DATA1 or DATA0 < DATA2
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_SetIntWinMode(M4_DCU_TypeDef *DCUx,
                              en_dcu_int_win_mode_t enIntWinMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check the parameters */
    DDL_ASSERT(IS_VALID_DCU_INT_WIN_MODE(enIntWinMode));

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        DCUx->INTSEL_f.INT_WIN = (uint32_t)enIntWinMode;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get DCU interrup window.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 **
 ** \retval DcuIntInvalid               DCU don't occur interrupt
 ** \retval DcuWinIntInvalid            DCU window interrupt is invalid.
 ** \retval DcuInsideWinCmpInt          DCU occur interrupt when DATA2 �� DATA0 �� DATA2
 ** \retval DcuOutsideWinCmpInt         DCU occur interrupt when DATA0 > DATA1 or DATA0 < DATA2
 **
 ******************************************************************************/
en_dcu_int_win_mode_t DCU_GetIntWinMode(M4_DCU_TypeDef *DCUx)
{
    /* Check for DCUx pointer */
    DDL_ASSERT(IS_VALID_DCU(DCUx));

    return (en_dcu_int_win_mode_t)(DCUx->INTSEL_f.INT_WIN);
}

/**
 *******************************************************************************
 ** \brief Set DCU compare trigger mode.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enTriggerMode           DCU compare trigger mode
 ** \arg DcuCmpTrigbyData0              DCU compare triggered by DATA0
 ** \arg DcuCmpTrigbyData012            DCU compare triggered by DATA0 or DATA1 or DATA2
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_SetCmpTriggerMode(M4_DCU_TypeDef *DCUx,
                                  en_dcu_cmp_trigger_mode_t enTriggerMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_DCU_CMP_TRIG_MODE(enTriggerMode));

        DCUx->CTL_f.COMP_TRG = (uint32_t)enTriggerMode;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get DCU compare trigger mode.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 **
 ** \retval DcuCmpTrigbyData0           DCU compare triggered by DATA0
 ** \retval DcuCmpTrigbyData012         DCU compare triggered by DATA0 or DATA1 or DATA2
 **
 ******************************************************************************/
en_dcu_cmp_trigger_mode_t DCU_GetCmpTriggerMode(M4_DCU_TypeDef *DCUx)
{
    /* Check for DCUx pointer */
    DDL_ASSERT(IS_VALID_DCU(DCUx));

    return (en_dcu_cmp_trigger_mode_t)(DCUx->CTL_f.COMP_TRG);
}

/**
 *******************************************************************************
 ** \brief Enable DCU interrupt.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enCmd                   DCU interrupt state
 ** \arg Enable                         Enable the DCU interrupt function
 ** \arg Disable                        Disable the DCU interrupt function
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_IrqCmd(M4_DCU_TypeDef *DCUx, en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        DCUx->CTL_f.INTEN = (uint32_t)(enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the specified DCU flag
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enFlag                  The specified DCU flag
 ** \arg DcuIntOp                       DCU overflow or underflow
 ** \arg DcuIntLs2                      DCU DATA0 < DATA2
 ** \arg DcuIntEq2                      DCU DATA0 = DATA2
 ** \arg DcuIntGt2                      DCU DATA0 > DATA2
 ** \arg DcuIntLs1                      DCU DATA0 < DATA1
 ** \arg DcuIntEq1                      DCU DATA0 = DATA1
 ** \arg DcuIntGt1                      DCU DATA0 > DATA1
 **
 ** \retval Set                         Flag is set.
 ** \retval Reset                       Flag is reset or enStatus is invalid.
 **
 ******************************************************************************/
en_flag_status_t DCU_GetIrqFlag(M4_DCU_TypeDef *DCUx, en_dcu_flag_t enFlag)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_DCU(DCUx));
    DDL_ASSERT(IS_VALID_DCU_INT(enFlag));

    return ((DCUx->FLAG & enFlag) ? Set : Reset);
}

/**
 *******************************************************************************
 ** \brief Clear the specified DCU flag
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enFlag                  the specified DCU flag
 ** \arg DcuIntOp                       DCU overflow or underflow
 ** \arg DcuIntLs2                      DCU DATA0 < DATA2
 ** \arg DcuIntEq2                      DCU DATA0 = DATA2
 ** \arg DcuIntGt2                      DCU DATA0 > DATA2
 ** \arg DcuIntLs1                      DCU DATA0 < DATA1
 ** \arg DcuIntEq1                      DCU DATA0 = DATA1
 ** \arg DcuIntGt1                      DCU DATA0 > DATA1
 **
 ** \retval Ok                          Clear flag successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_ClearIrqFlag(M4_DCU_TypeDef *DCUx, en_dcu_flag_t enFlag)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_DCU_INT(enFlag));
        DCUx->FLAGCLR = (uint32_t)enFlag;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable/Disable DCU interrupt.
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enIntSel                DCU interrupt selection
 ** \arg DcuIntOp                       DCU overflow or underflow
 ** \arg DcuIntLs2                      DCU DATA0 < DATA2
 ** \arg DcuIntEq2                      DCU DATA0 = DATA2
 ** \arg DcuIntGt2                      DCU DATA0 > DATA2
 ** \arg DcuIntLs1                      DCU DATA0 < DATA1
 ** \arg DcuIntEq1                      DCU DATA0 = DATA1
 ** \arg DcuIntGt1                      DCU DATA0 > DATA1
 ** \param [in] enCmd                   DCU interrupt functional state
 ** \arg Enable                         Enable the specified DCU interrupt function
 ** \arg Disable                        Disable the specified DCU interrupt function
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - DCUx is invalid
 **                                     - enIntSel is invalid
 **
 ******************************************************************************/
en_result_t DCU_IrqSelCmd(M4_DCU_TypeDef *DCUx,
                          en_dcu_int_sel_t enIntSel,
                          en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_DCU_INT(enIntSel));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        enRet = Ok;
        switch (enIntSel)
        {
        case DcuIntOp:
            DCUx->INTSEL_f.INT_OP = (uint32_t)enCmd;
            break;
        case DcuIntLs2:
            DCUx->INTSEL_f.INT_LS2 = (uint32_t)enCmd;
            break;
        case DcuIntEq2:
            DCUx->INTSEL_f.INT_EQ2 = (uint32_t)enCmd;
            break;
        case DcuIntGt2:
            DCUx->INTSEL_f.INT_GT2 = (uint32_t)enCmd;
            break;
        case DcuIntLs1:
            DCUx->INTSEL_f.INT_LS1 = (uint32_t)enCmd;
            break;
        case DcuIntEq1:
            DCUx->INTSEL_f.INT_EQ1 = (uint32_t)enCmd;
            break;
        case DcuIntGt1:
            DCUx->INTSEL_f.INT_GT1 = (uint32_t)enCmd;
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
 ** \brief Read DCU register DATAx
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enDataReg               The specified DATA register.
 ** \arg DcuRegisterData0               DCU register DATA0
 ** \arg DcuRegisterData1               DCU register DATA1
 ** \arg DcuRegisterData2               DCU register DATA2
 **
 ** \retval DCU register DATAx value
 **
 ******************************************************************************/
uint8_t DCU_ReadDataByte(M4_DCU_TypeDef *DCUx,
                         en_dcu_data_register_t enDataReg)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_DCU(DCUx));
    DDL_ASSERT(IS_VALID_DCU_DATA_REG(enDataReg));

    return *(uint8_t *)DCU_DATAx(DCUx, enDataReg);
}

/**
 *******************************************************************************
 ** \brief Write DCU register DATAx
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enDataReg               The specified DATA register.
 ** \arg DcuRegisterData0               DCU register DATA0
 ** \arg DcuRegisterData1               DCU register DATA1
 ** \arg DcuRegisterData2               DCU register DATA2
 ** \param [in] u8Data                  The data will be written.
 **
 ** \retval Ok                          Write successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_WriteDataByte(M4_DCU_TypeDef *DCUx,
                              en_dcu_data_register_t enDataReg,
                              uint8_t u8Data)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_DCU_DATA_REG(enDataReg));

        *(uint8_t *)DCU_DATAx(DCUx, enDataReg) = u8Data;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Read DCU register DATAx
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enDataReg               The specified DATA register.
 ** \arg DcuRegisterData0               DCU register DATA0
 ** \arg DcuRegisterData1               DCU register DATA1
 ** \arg DcuRegisterData2               DCU register DATA2
 **
 ** \retval DCU register DATAx value
 **
 ******************************************************************************/
uint16_t DCU_ReadDataHalfWord(M4_DCU_TypeDef *DCUx,
                              en_dcu_data_register_t enDataReg)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_DCU(DCUx));
    DDL_ASSERT(IS_VALID_DCU_DATA_REG(enDataReg));

    return *(uint16_t *)DCU_DATAx(DCUx, enDataReg);
}

/**
 *******************************************************************************
 ** \brief Write DCU register DATAx
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enDataReg               The specified DATA register.
 ** \arg DcuRegisterData0               DCU register DATA0
 ** \arg DcuRegisterData1               DCU register DATA1
 ** \arg DcuRegisterData2               DCU register DATA2
 ** \param [in] u16Data                 The data will be written.
 **
 ** \retval Ok                          Write successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_WriteDataHalfWord(M4_DCU_TypeDef *DCUx,
                                  en_dcu_data_register_t enDataReg,
                                  uint16_t u16Data)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_DCU_DATA_REG(enDataReg));

        *(uint16_t *)DCU_DATAx(DCUx, enDataReg) = u16Data;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Read DCU register DATAx
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enDataReg               The specified DATA register.
 ** \arg DcuRegisterData0               DCU register DATA0
 ** \arg DcuRegisterData1               DCU register DATA1
 ** \arg DcuRegisterData2               DCU register DATA2
 **
 ** \retval DCU register DATAx value
 **
 ******************************************************************************/
uint32_t DCU_ReadDataWord(M4_DCU_TypeDef *DCUx,
                          en_dcu_data_register_t enDataReg)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_DCU(DCUx));
    DDL_ASSERT(IS_VALID_DCU_DATA_REG(enDataReg));

    return *(uint32_t *)DCU_DATAx(DCUx, enDataReg);
}

/**
 *******************************************************************************
 ** \brief Write DCU register DATAx
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enDataReg               The specified DATA register.
 ** \arg DcuRegisterData0               DCU register DATA0
 ** \arg DcuRegisterData1               DCU register DATA1
 ** \arg DcuRegisterData2               DCU register DATA2
 ** \param [in] u32Data                 The data will be written.
 **
 ** \retval Ok                          Write successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_WriteDataWord(M4_DCU_TypeDef *DCUx,
                              en_dcu_data_register_t enDataReg,
                              uint32_t u32Data)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for DCUx pointer */
    if (IS_VALID_DCU(DCUx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_DCU_DATA_REG(enDataReg));

        *(uint32_t *)DCU_DATAx(DCUx, enDataReg) = u32Data;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set DCU trigger source number
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 ** \param [in] enTriggerSrc            The trigger source.
 ** \arg This parameter can be any value of @ref en_event_src_t
 **
 ** \retval Ok                          Write successfully.
 ** \retval ErrorInvalidParameter       DCUx is invalid
 **
 ******************************************************************************/
en_result_t DCU_SetTriggerSrc(M4_DCU_TypeDef *DCUx,
                              en_event_src_t enTriggerSrc)
{
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint32_t *TRGSELx = DCU_TRGSELx(DCUx);

    if (NULL != TRGSELx)
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_TRG_SRC_EVENT(enTriggerSrc));

        *TRGSELx = (uint32_t)enTriggerSrc;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get DCU trigger source register address
 **
 ** \param [in] DCUx                    Pointer to DCU instance register base
 ** \arg M4_DCU1                        DCU unit 1 instance register base
 ** \arg M4_DCU2                        DCU unit 2 instance register base
 ** \arg M4_DCU3                        DCU unit 3 instance register base
 ** \arg M4_DCU4                        DCU unit 4 instance register base
 **
 ** \retval DCUx_TRGSEL address         DCUx is valid
 ** \retval NULL                        DCUx is invalid
 **
 ******************************************************************************/
static __IO uint32_t *DCU_TRGSELx(const M4_DCU_TypeDef *DCUx)
{
    __IO uint32_t *TRGSELx = NULL;

    if (M4_DCU1 == DCUx)
    {
        TRGSELx = &M4_AOS->DCU1_TRGSEL;
    }
    else if (M4_DCU2 == DCUx)
    {
        TRGSELx = &M4_AOS->DCU2_TRGSEL;
    }
    else if (M4_DCU3 == DCUx)
    {
        TRGSELx = &M4_AOS->DCU3_TRGSEL;
    }
    else if (M4_DCU4 == DCUx)
    {
        TRGSELx = &M4_AOS->DCU4_TRGSEL;
    }
    else
    {
        TRGSELx = NULL;
    }

    return TRGSELx;
}

//@} // DcuGroup

#endif /* DDL_DCU_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
