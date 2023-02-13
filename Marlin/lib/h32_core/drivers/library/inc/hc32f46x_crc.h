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
/** \file hc32f46x_crc.h
 **
 ** A detailed description is available at
 ** @link CrcGroup Crc description @endlink
 **
 **   - 2019-03-07  1.0 Wuze First version for Device Driver Library of Crc.
 **
 ******************************************************************************/
#ifndef __HC32F46X_CRC_H__
#define __HC32F46X_CRC_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_CRC_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup CrcGroup Cyclic Redundancy Check(CRC)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/* Bits definitions of CRC control register(CRC_CR). */
/*
 * Definitions of CRC protocol.
 * NOTE: CRC16 polynomial is X16 + X12 + X5 + 1
 *       CRC32 polynomial is X32 + X26 + X23 + X22 + X16 + X12 + X11 + X10 + \
 *                           X8 + X7 + X5 + X4 + X2 + X + 1
 */
#define CRC_SEL_16B ((uint32_t)0x0)
#define CRC_SEL_32B ((uint32_t)(0x1ul << 1u))

/*
 * Identifies the transpose configuration of the source data.
 * If this function is enabled, the source data's bits in bytes are transposed.
 * e.g. There's a source data 0x1234 which will be calculated checksum and this
 * function is enabled, the final data be calculated is 0x482C.
 * 0x12: bit0->bit7, bit1->bit6, ..., bit7->bit0, the data byte changed to 0x48.
 * 0x48: bit0->bit7, bit1->bit6, ..., bit7->bit0, the data byte changed to 0x2C.
 * The same to 32 bit data while using CRC32.
 */
#define CRC_REFIN_DISABLE ((uint32_t)0x0)
#define CRC_REFIN_ENABLE ((uint32_t)(0x1ul << 2u))

/*
 * Identifies the transpose configuration of the checksum.
 * If this function is enabled, bits of the checksum will be transposed.
 * e.g. There is a CRC16 checksum is 0x5678 before this function enabled, then
 * this function is enabled, the checksum will be 0x1E6A.
 * 0x5678: bit0->bit15, bit1->bit14, ..., bit15->bit0, the final data is 0x1E6A.
 * The same to CRC32 checksum while using CRC32.
 */
#define CRC_REFOUT_DISABLE ((uint32_t)0x0)
#define CRC_REFOUT_ENABLE ((uint32_t)(0x1ul << 3u))

/*
 * XORs the CRC checksum with 0xFFFF(CRC16) or 0xFFFFFFFF(CRC32).
 * e.g. There is a CRC16 checksum is 0x5678 before this function enabled.
 * If this function enabled, the checksum will be 0xA987.
 * The same to CRC32 checksum while using CRC32.
 */
#define CRC_XOROUT_DISABLE ((uint32_t)0x0)
#define CRC_XOROUT_ENABLE ((uint32_t)(0x1ul << 4u))

#define CRC_CONFIG_MASK ((uint32_t)(0x1Eu))

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    void CRC_Init(uint32_t u32Config);
    uint16_t CRC_Calculate16B(uint16_t u16InitVal, const uint16_t *pu16Data, uint32_t u32Length);
    uint32_t CRC_Calculate32B(uint32_t u32InitVal, const uint32_t *pu32Data, uint32_t u32Length);
    bool CRC_Check16B(uint16_t u16InitVal, uint16_t u16CheckSum, const uint16_t *pu16Data, uint32_t u32Length);
    bool CRC_Check32B(uint32_t u32InitVal, uint32_t u32CheckSum, const uint32_t *pu32Data, uint32_t u32Length);

    //@} // CrcGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_CRC_ENABLE */

#endif /* __HC32F46X_CRC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
