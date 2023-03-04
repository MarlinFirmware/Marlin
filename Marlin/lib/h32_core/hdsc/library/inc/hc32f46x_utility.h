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
/** \file hc32f46x_utility.h
 **
 ** A detailed description is available at
 ** @link DdlUtilityGroup Ddl Utility description @endlink
 **
 **   - 2018-11-02  1.0  Zhangxl First version for Device Driver Library Utility.
 **
 ******************************************************************************/
#ifndef __HC32F46x_UTILITY_H__
#define __HC32F46x_UTILITY_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_UTILITY_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup DdlUtilityGroup Device Driver Library Utility(DDLUTILITY)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    /* Initialize uart for printf */
    en_result_t Ddl_UartInit(void);

    /* A approximate delay */
    void Ddl_Delay1ms(uint32_t u32Cnt);
    void Ddl_Delay1us(uint32_t u32Cnt);

/*! Ddl assert, you can add your own assert functions by implement the function
Ddl_AssertHook definition follow the function Ddl_AssertHook declaration */
#ifdef __DEBUG
#define DDL_ASSERT(x)                                                       \
    do                                                                      \
    {                                                                       \
        ((x) ? (void)0 : Ddl_AssertHandler((uint8_t *)__FILE__, __LINE__)); \
    } while (0)
    /* Exported function */
    void Ddl_AssertHandler(uint8_t *file, int16_t line);
#else
#define DDL_ASSERT(x) (void)(0)
#endif /* __DEBUG */

    //@} // DdlUtilityGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_UTILITY_ENABLE */

#endif /* __HC32F46x_UTILITY_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
