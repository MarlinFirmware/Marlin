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
/** \file hc32f46x_dcu.h
 **
 ** A detailed description is available at
 ** @link DcuGroup DCU description @endlink
 **
 **   - 2018-10-15  1.0 Hongjh First version for Device Driver Library of DCU.
 **
 ******************************************************************************/
#ifndef __HC32F46x_DCU_H__
#define __HC32F46x_DCU_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_DCU_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup DcuGroup Data Computing Unit(DCU)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief DCU register data enumeration
 **
 ******************************************************************************/
typedef enum en_dcu_data_register
{
    DcuRegisterData0 = 0u,                  ///< DCU DATA0
    DcuRegisterData1 = 1u,                  ///< DCU DATA1
    DcuRegisterData2 = 2u,                  ///< DCU DATA2
} en_dcu_data_register_t;

/**
 *******************************************************************************
 ** \brief DCU operation enumeration
 **
 ******************************************************************************/
typedef enum en_dcu_operation_mode
{
    DcuInvalid     = 0u,                    ///< DCU Invalid
    DcuOpAdd       = 1u,                    ///< DCU operation: Add
    DcuOpSub       = 2u,                    ///< DCU operation: Sub
    DcuHwTrigOpAdd = 3u,                    ///< DCU operation: Hardware trigger Add
    DcuHwTrigOpSub = 4u,                    ///< DCU operation: Hardware trigger Sub
    DcuOpCompare   = 5u,                    ///< DCU operation: Compare
} en_dcu_operation_mode_t;

/**
 *******************************************************************************
 ** \brief DCU data size enumeration
 **
 ******************************************************************************/
typedef enum en_dcu_data_size
{
    DcuDataBit8  = 0u,                      ///< DCU data size: 8 bit
    DcuDataBit16 = 1u,                      ///< DCU data size: 16 bit
    DcuDataBit32 = 2u,                      ///< DCU data size: 32 bit
} en_dcu_data_size_t;

/**
 *******************************************************************************
 ** \brief DCU compare operation trigger mode enumeration
 **
 ******************************************************************************/
typedef enum en_dcu_cmp_trigger_mode
{
    DcuCmpTrigbyData0   = 0u,               ///< DCU compare triggered by DATA0
    DcuCmpTrigbyData012 = 1u,               ///< DCU compare triggered by DATA0 or DATA1 or DATA2
} en_dcu_cmp_trigger_mode_t;

/**
 *******************************************************************************
 ** \brief DCU interrupt selection enumeration
 **
 ******************************************************************************/
typedef enum en_dcu_int_sel
{
    DcuIntOp  = (1ul << 0),                 ///< DCU overflow or underflow interrupt
    DcuIntLs2 = (1ul << 1),                 ///< DCU DATA0 < DATA2 interrupt
    DcuIntEq2 = (1ul << 2),                 ///< DCU DATA0 = DATA2 interrupt
    DcuIntGt2 = (1ul << 3),                 ///< DCU DATA0 > DATA2 interrupt
    DcuIntLs1 = (1ul << 4),                 ///< DCU DATA0 < DATA1 interrupt
    DcuIntEq1 = (1ul << 5),                 ///< DCU DATA0 = DATA1 interrupt
    DcuIntGt1 = (1ul << 6),                 ///< DCU DATA0 > DATA1 interrupt
} en_dcu_int_sel_t, en_dcu_flag_t;

/**
 *******************************************************************************
 ** \brief DCU window interrupt mode enumeration
 **
 ******************************************************************************/
typedef enum en_dcu_int_win_mode
{
    DcuIntInvalid       = 0u,               ///< DCU don't occur interrupt
    DcuWinIntInvalid    = 1u,               ///< DCU window interrupt is invalid.
    DcuInsideWinCmpInt  = 2u,               ///< DCU occur interrupt when DATA2 ¡Ü DATA0 ¡Ü DATA2
    DcuOutsideWinCmpInt = 3u,               ///< DCU occur interrupt when DATA0 > DATA1 or DATA0 < DATA2
} en_dcu_int_win_mode_t;

/**
 *******************************************************************************
 ** \brief DCU initialization configuration
 **
 ******************************************************************************/
typedef struct stc_dcu_init
{
    uint32_t                  u32IntSel;        ///< Specifies interrupt selection and This parameter can be a value of @ref en_dcu_int_sel_t

    en_functional_state_t     enIntCmd;         ///< Select DCU interrupt function. Enable:Enable DCU interrupt function; Disable:Disable DCU interrupt function

    en_dcu_int_win_mode_t     enIntWinMode;     ///< Specifies interrupt window mode and This parameter can be a value of @ref en_dcu_int_win_mode_t

    en_dcu_data_size_t        enDataSize;       ///< Specifies DCU data size and This parameter can be a value of @ref en_dcu_data_size_t

    en_dcu_operation_mode_t   enOperation;      ///< Specifies DCU operation and This parameter can be a value of @ref en_dcu_operation_mode_t

    en_dcu_cmp_trigger_mode_t enCmpTriggerMode; ///< Specifies DCU compare operation trigger mode size and This parameter can be a value of @ref en_dcu_cmp_trigger_mode_t

} stc_dcu_init_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
en_result_t DCU_Init(M4_DCU_TypeDef *DCUx, const stc_dcu_init_t *pstcInitCfg);
en_result_t DCU_DeInit(M4_DCU_TypeDef *DCUx);
en_result_t DCU_SetOperationMode(M4_DCU_TypeDef *DCUx,
                                en_dcu_operation_mode_t enMode);
en_dcu_operation_mode_t DCU_GetOperationMode(M4_DCU_TypeDef *DCUx);
en_result_t DCU_SetDataSize(M4_DCU_TypeDef *DCUx, en_dcu_data_size_t enSize);
en_dcu_data_size_t DCU_GetDataSize(M4_DCU_TypeDef *DCUx);
en_result_t DCU_SetIntWinMode(M4_DCU_TypeDef *DCUx,
                                en_dcu_int_win_mode_t enIntWinMode);
en_dcu_int_win_mode_t DCU_GetIntWinMode(M4_DCU_TypeDef *DCUx);
en_result_t DCU_SetCmpTriggerMode(M4_DCU_TypeDef *DCUx,
                                en_dcu_cmp_trigger_mode_t enTriggerMode);
en_dcu_cmp_trigger_mode_t DCU_GetCmpTriggerMode(M4_DCU_TypeDef *DCUx);
en_result_t DCU_EnableInterrupt(M4_DCU_TypeDef *DCUx);
en_result_t DCU_DisableInterrupt(M4_DCU_TypeDef *DCUx);
en_flag_status_t DCU_GetIrqFlag(M4_DCU_TypeDef *DCUx, en_dcu_flag_t enFlag);
en_result_t DCU_ClearIrqFlag(M4_DCU_TypeDef *DCUx, en_dcu_flag_t enFlag);
en_result_t DCU_IrqSelCmd(M4_DCU_TypeDef *DCUx,
                                en_dcu_int_sel_t enIntSel,
                                en_functional_state_t enCmd);
uint8_t DCU_ReadDataByte(M4_DCU_TypeDef *DCUx,
                                en_dcu_data_register_t enDataReg);
en_result_t DCU_WriteDataByte(M4_DCU_TypeDef *DCUx,
                                en_dcu_data_register_t enDataReg, uint8_t u8Data);
uint16_t DCU_ReadDataHalfWord(M4_DCU_TypeDef *DCUx,
                                en_dcu_data_register_t enDataReg);
en_result_t DCU_WriteDataHalfWord(M4_DCU_TypeDef *DCUx,
                                en_dcu_data_register_t enDataReg,
                                uint16_t u16Data);
uint32_t DCU_ReadDataWord(M4_DCU_TypeDef *DCUx,
                                en_dcu_data_register_t enDataReg);
en_result_t DCU_WriteDataWord(M4_DCU_TypeDef *DCUx,
                                en_dcu_data_register_t enDataReg,
                                uint32_t u32Data);
en_result_t DCU_SetTriggerSrc(M4_DCU_TypeDef *DCUx,
                                en_event_src_t enTriggerSrc);

//@} // DcuGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_DCU_ENABLE */

#endif /* __HC32F46x_DCU_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
