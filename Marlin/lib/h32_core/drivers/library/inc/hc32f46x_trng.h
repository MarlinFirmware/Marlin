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
/** \file hc32f46x_trng.h
 **
 ** A detailed description is available at
 ** @link TrngGroup Trng description @endlink
 **
 **   - 2018-10-20  1.0 Wuze First version for Device Driver Library of Trng.
 **
 ******************************************************************************/
#ifndef __HC32F46X_TRNG_H__
#define __HC32F46X_TRNG_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_TRNG_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup TrngGroup True Random Number Generator(TRNG)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/
    /* The data register loads the new initial value from the random number
       generator before the random number is generated. */
    typedef enum en_trng_load_ctrl
    {
        TrngLoadNewInitValue_Disable = 0x0, ///< Disable load new initial values.
        TrngLoadNewInitValue_Enable = 0x1,  ///< Enable load new initial values.
    } en_trng_load_ctrl_t;

    /* Shift n times when capturing random noise. */
    typedef enum en_trng_shift_cnt
    {
        TrngShiftCount_32 = 0x3,  ///< Shift 32 times when capturing random noise.
        TrngShiftCount_64 = 0x4,  ///< Shift 64 times when capturing random noise.
        TrngShiftCount_128 = 0x5, ///< Shift 128 times when capturing random noise.
        TrngShiftCount_256 = 0x6, ///< Shift 256 times when capturing random noise.
    } en_trng_shift_cnt_t;

    /* TRNG initialization structure definition. */
    typedef struct stc_trng_init
    {
        en_trng_load_ctrl_t enLoadCtrl;   ///< @ref en_trng_load_ctrl_t.
        en_trng_shift_cnt_t enShiftCount; ///< @ref en_trng_shift_cnt_t.
    } stc_trng_init_t;

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/

    en_result_t TRNG_Init(const stc_trng_init_t *pstcInit);
    void TRNG_DeInit(void);
    en_result_t TRNG_Generate(uint32_t *pu32Random, uint8_t u8Length, uint32_t u32Timeout);

    void TRNG_StartIT(void);
    void TRNG_GetRandomNum(uint32_t *pu32Random, uint8_t u8Length);

    //@} // TrngGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_TRNG_ENABLE */

#endif /* __HC32F46X_TRNG_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
