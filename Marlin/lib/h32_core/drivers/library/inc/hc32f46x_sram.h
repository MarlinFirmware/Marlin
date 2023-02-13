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
/** \file hc32f46x_sram.h
 **
 ** A detailed description is available at
 ** @link SramGroup Internal SRAM description @endlink
 **
 **   - 2018-10-17  1.0  Zhangxl First version for Device Driver Library of
 **                      SRAM
 **
 ******************************************************************************/

#ifndef __HC32F46X_SRAM_H__
#define __HC32F46X_SRAM_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_SRAM_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 *  \defgroup SramGroup Internal SRAM
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
///< SRAM wait cycle register, parity/ECC check register protect code definition
#define SRAM_PROTECT_CODE (0x0000003Bu)

/*******************************************************************************
                Start addr.     End addr.       Size    Function
  SRAM1         0x20000000      0x2000FFFF      64KB    Even Parity Check
  SRAM2         0x20010000      0x2001FFFF      64KB    Even Parity Check
  SRAM3         0x20020000      0x20026FFF      28KB    ECC Check
  SRAM_Ret      0x200F0000      0x200F0FFF       4KB    Even Parity Check
  SRAM_HS       0x1FFF8000      0x1FFFFFFF      32KB    Even Parity Check
 ******************************************************************************/
///< SRAM1 base address definition
#define SRAM1_BASE_ADDR (*((volatile unsigned int *)(0x20000000UL)))

///< SRAM2 base address definition
#define SRAM2_BASE_ADDR (*((volatile unsigned int *)(0x20010000UL)))

///< SRAM3 base address definition
#define SRAM3_BASE_ADDR (*((volatile unsigned int *)(0x20020000UL)))

///< Retention SRAM base address definition
#define SRAMRET_BASE_ADDR (*((volatile unsigned int *)(0x200F0000UL)))

///< High speed SRAM base address definition
#define SRAMHS_BASE_ADDR (*((volatile unsigned int *)(0x1FFF8000UL)))

    typedef enum en_sram_index
    {
        Sram12Idx = 1u << 0,
        Sram3Idx = 1u << 1,
        SramHsIdx = 1u << 2,
        SramRetIdx = 1u << 3,
    } en_sram_index_t;
    /**
     *******************************************************************************
     ** \brief Enumeration to the write/read cycles of SRAM
     **
     ** \note
     ******************************************************************************/
    typedef enum en_sram_rw_cycle
    {
        SramCycle1 = 0u,
        SramCycle2 = 1u,
        SramCycle3 = 2u,
        SramCycle4 = 3u,
        SramCycle5 = 4u,
        SramCycle6 = 5u,
        SramCycle7 = 6u,
        SramCycle8 = 7u,
    } en_sram_rw_cycle_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to ECC check mode
     **
     ** \note
     ******************************************************************************/
    typedef enum en_ecc_mode
    {
        EccMode0 = 0u, ///< disable ECC check function
        EccMode1 = 1u, ///< no 1 bit ECC flag, interrupt/reset if 1 bit-ECC is detected
                       ///< generate 2 bit ECC flag, interrupt/reset if 2 bit-ECC is detected
        EccMode2 = 2u, ///< generate 1 bit ECC flag, but no interrupt/reset if 1 bit-ECC is detected
                       ///< generate 2 bit ECC flag, interrupt/reset if 2 bit-ECC is detected
        EccMode3 = 3u, ///< generate 1 bit ECC flag, interrupt/reset if 1 bit-ECC is detected
                       ///< generate 2 bit ECC flag, interrupt/reset if 2 bit-ECC is detected
    } en_ecc_mode_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to operation after ECC/Parity error
     **
     ** \note
     ******************************************************************************/
    typedef enum en_ecc_py_err_op
    {
        SramNmi = 0u,   ///< Generate NMI after ECC/Parity error detected
        SramReset = 1u, ///< Generate Reset after ECC/Parity error detected
    } en_ecc_py_err_op_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to the ECC/Parity error status of each SRAM
     **
     ** \note
     ******************************************************************************/
    typedef enum en_sram_err_status
    {
        Sram3EccErr1 = 1u << 0,     ///< SRAM3 1 bit ECC error
        Sram3EccErr2 = 1u << 1,     ///< SRAM3 2 bit ECC error
        Sram12ParityErr = 1u << 2,  ///< SRAM1/2 parity error
        SramHSParityErr = 1u << 3,  ///< High speed SRAM parity error
        SramRetParityErr = 1u << 4, ///< Retention SRAM parity error
    } en_sram_err_status_t;

    /**
     *******************************************************************************
     ** \brief SRAM configuration
     **
     ** \note The SRAM configuration structure
     ******************************************************************************/
    typedef struct stc_sram_config
    {
        uint8_t u8SramIdx;              ///< SRAM index, ref @ en_sram_index_t for details
        en_sram_rw_cycle_t enSramRC;    ///< SRAM read wait cycle setting
        en_sram_rw_cycle_t enSramWC;    ///< SRAM write wait cycle setting
        en_ecc_mode_t enSramEccMode;    ///< SRAM ECC mode setting
        en_ecc_py_err_op_t enSramEccOp; ///< SRAM3 ECC error handling setting
        en_ecc_py_err_op_t enSramPyOp;  ///< SRAM1/2/HS/Ret Parity error handling setting

    } stc_sram_config_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    extern en_result_t SRAM_Init(const stc_sram_config_t *pstcSramConfig);
    extern en_result_t SRAM_DeInit(void);
    extern en_result_t SRAM_WT_Disable(void);
    extern en_result_t SRAM_WT_Enable(void);
    extern en_result_t SRAM_CK_Disable(void);
    extern en_result_t SRAM_CK_Enable(void);
    extern en_flag_status_t SRAM_GetStatus(en_sram_err_status_t enSramErrStatus);
    extern en_result_t SRAM_ClrStatus(en_sram_err_status_t enSramErrStatus);

    //@} // SramGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_SRAM_ENABLE */

#endif /* __HC32F46X_SRAM_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
