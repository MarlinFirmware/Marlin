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
/** \file hc32f46x_keyscan.h
 **
 ** A detailed description is available at
 ** @link KeyscanGroup Keyscan description @endlink
 **
 **   - 2018-10-17  1.0  Zhangxl First version for Device Driver Library of
 **                      keyscan
 **
 ******************************************************************************/

#ifndef __HC32F46X_KEYSCAN_H__
#define __HC32F46X_KEYSCAN_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_KEYSCAN_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     *  \defgroup KeyscanGroup Matrix Key Scan Module (KeyScan)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief Enumeration to hi-z state cycles of each keyout
     **
     ** \note
     ******************************************************************************/
    typedef enum en_hiz_cycle
    {
        Hiz4 = 0u,
        Hiz8 = 1u,
        Hiz16 = 2u,
        Hiz32 = 3u,
        Hiz64 = 4u,
        Hiz256 = 5u,
        Hiz512 = 6u,
        Hiz1K = 7u,
    } en_hiz_cycle_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to low state cycles of each keyout
     **
     ** \note
     ******************************************************************************/
    typedef enum en_low_cycle
    {
        Low8 = 3u,
        Low16 = 4u,
        Low32 = 5u,
        Low64 = 6u,
        Low128 = 7u,
        Low256 = 8u,
        Low512 = 9u,
        Low1K = 10u,
        Low2K = 11u,
        Low4K = 12u,
        Low8K = 13u,
        Low16K = 14u,
        Low32K = 15u,
        Low64K = 16u,
        Low128K = 17u,
        Low256K = 18u,
        Low512K = 19u,
        Low1M = 20u,
        Low2M = 21u,
        Low4M = 22u,
        Low8M = 23u,
        Low16M = 24u,
    } en_low_cycle_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to key scan clock
     **
     ** \note
     ******************************************************************************/
    typedef enum en_keyscan_clk
    {
        KeyscanHclk = 0u,   ///< use HCLK as scan clock
        KeyscanLrc = 1u,    ///< use internal Low RC as scan clock
        KeyscanXtal32 = 2u, ///< use external XTAL32 as scan clock
    } en_keyscan_clk_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to KEYOUT combination
     ******************************************************************************/
    typedef enum en_keyout_sel
    {
        Keyout0To1 = 1u, ///< KEYOUT 0 to 1 are selected
        Keyout0To2 = 2u, ///< KEYOUT 0 to 2 are selected
        Keyout0To3 = 3u, ///< KEYOUT 0 to 3 are selected
        Keyout0To4 = 4u, ///< KEYOUT 0 to 4 are selected
        Keyout0To5 = 5u, ///< KEYOUT 0 to 5 are selected
        Keyout0To6 = 6u, ///< KEYOUT 0 to 6 are selected
        Keyout0To7 = 7u, ///< KEYOUT 0 to 7 are selected
    } en_keyout_sel_t;

    /**
     *******************************************************************************
     ** \brief Enumeration to KEYIN combination
     ******************************************************************************/
    typedef enum en_keyin_sel
    {
        Keyin00 = 1u << 0,  ///< KEYIN 0 is selected
        Keyin01 = 1u << 1,  ///< KEYIN 1 is selected
        Keyin02 = 1u << 2,  ///< KEYIN 2 is selected
        Keyin03 = 1u << 3,  ///< KEYIN 3 is selected
        Keyin04 = 1u << 4,  ///< KEYIN 4 is selected
        Keyin05 = 1u << 5,  ///< KEYIN 5 is selected
        Keyin06 = 1u << 6,  ///< KEYIN 6 is selected
        Keyin07 = 1u << 7,  ///< KEYIN 7 is selected
        Keyin08 = 1u << 8,  ///< KEYIN 8 is selected
        Keyin09 = 1u << 9,  ///< KEYIN 9 is selected
        Keyin10 = 1u << 10, ///< KEYIN 10 is selected
        Keyin11 = 1u << 11, ///< KEYIN 11 is selected
        Keyin12 = 1u << 12, ///< KEYIN 12 is selected
        Keyin13 = 1u << 13, ///< KEYIN 13 is selected
        Keyin14 = 1u << 14, ///< KEYIN 14 is selected
        Keyin15 = 1u << 15, ///< KEYIN 15 is selected
    } en_keyin_sel_t;

    /**
     *******************************************************************************
     ** \brief Keyscan configuration
     **
     ** \note The Keyscan configuration structure
     ******************************************************************************/
    typedef struct stc_keyscan_config
    {
        en_hiz_cycle_t enHizCycle;     ///< KEYOUT Hiz state cycles, ref @ en_hiz_cycle_t for details
        en_low_cycle_t enLowCycle;     ///< KEYOUT Low state cycles, ref @ en_low_cycle_t for details
        en_keyscan_clk_t enKeyscanClk; ///< Key scan clock, ref @ en_keyscan_clk_t for details
        en_keyout_sel_t enKeyoutSel;   ///< KEYOUT selection, ref @ en_keyout_sel_t for details
        uint16_t u16KeyinSel;          ///< KEYIN selection, ref @ en_keyin_sel_t for details
    } stc_keyscan_config_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    extern en_result_t KEYSCAN_Init(const stc_keyscan_config_t *pstcKeyscanConfig);
    extern en_result_t KEYSCAN_DeInit(void);
    extern en_result_t KEYSCAN_Start(void);
    extern en_result_t KEYSCAN_Stop(void);
    extern uint8_t KEYSCAN_GetColIdx(void);

    //@} // KeyscanGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_KEYSCAN_ENABLE */

#endif /* __HC32F46X_KEYSCAN_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
