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
/** \file system_hc32f46x.h
 **
 ** A detailed description is available at
 ** @link Hc32f46xSystemGroup Hc32f46xSystem description @endlink
 **
 **   - 2018-10-15  1.0  First version.
 **
 ******************************************************************************/
#ifndef __SYSTEM_HC32F46x_H__
#define __SYSTEM_HC32F46x_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <stdint.h>

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup Hc32f46xSystemGroup HC32F46x System Configure
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global pre-processor symbols/macros ('define')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Clock Setup macro definition
 **
 ** - 0: CLOCK_SETTING_NONE  - User provides own clock setting in application
 ** - 1: CLOCK_SETTING_CMSIS -
 ******************************************************************************/
#define CLOCK_SETTING_NONE 0u
#define CLOCK_SETTING_CMSIS 1u

#if !defined(HRC_VALUE)
#define HRC_VALUE ((uint32_t)16000000) /*!< Internal high speed RC freq. */
#endif

#if !defined(MRC_VALUE)
#define MRC_VALUE ((uint32_t)8000000) /*!< Internal middle speed RC freq. */
#endif

#if !defined(LRC_VALUE)
#define LRC_VALUE ((uint32_t)32768) /*!< Internal low speed RC freq. */
#endif

#if !defined(XTAL_VALUE)
#define XTAL_VALUE ((uint32_t)8000000) /*!< External high speed OSC freq. */
#endif

#if !defined(XTAL32_VALUE)
#define XTAL32_VALUE ((uint32_t)32768) /*!< External low speed OSC freq. */
#endif

    /******************************************************************************/
    /*                                                                            */
    /*                      START OF USER SETTINGS HERE                           */
    /*                      ===========================                           */
    /*                                                                            */
    /*                 All lines with '<<<' can be set by user.                   */
    /*                                                                            */
    /******************************************************************************/

    /******************************************************************************/
    /* Global function prototypes ('extern', definition in C source)              */
    /******************************************************************************/
    extern uint32_t SystemCoreClock;         // System Clock Frequency (Core Clock)
    extern void SystemInit(void);            // Initialize the system
    extern void SystemCoreClockUpdate(void); // Update SystemCoreClock variable

    //@} // Hc32f46xSystemGroup

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_HC32F46x_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
