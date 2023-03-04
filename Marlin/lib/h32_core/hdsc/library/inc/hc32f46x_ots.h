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
/** \file hc32f46x_ots.h
 **
 ** A detailed description is available at
 ** @link OtsGroup Ots description @endlink
 **
 **   - 2018-10-26  1.0 Wuze First version for Device Driver Library of Ots.
 **
 ******************************************************************************/
#ifndef __HC32F46X_OTS_H__
#define __HC32F46X_OTS_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_OTS_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup OtsGroup On-chip Temperature Sensor(OTS)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /* Automatically turn off the analog temperature sensor after the temperature
       measurement is over. */
    typedef enum en_ots_auto_off
    {
        OtsAutoOff_Disable = 0x0, ///< Disable automatically turn off OTS.
        OtsAutoOff_Enable = 0x1,  ///< Enable automatically turn off OTS.
    } en_ots_auto_off_t;

    /* Temperature measurement end interrupt request. */
    typedef enum en_ots_ie
    {
        OtsInt_Disable = 0x0, ///< Disable OTS interrupt.
        OtsInt_Enable = 0x1,  ///< Enable OTS interrupt.
    } en_ots_ie_t;

    /* OTS clock selection. */
    typedef enum en_ots_clk_sel
    {
        OtsClkSel_Xtal = 0x0, ///< Select XTAL as OTS clock.
        OtsClkSel_Hrc = 0x1,  ///< Select HRC as OTS clock.
    } en_ots_clk_sel_t;

    /* OTS OTS initialization structure definition. */
    typedef struct stc_ots_init
    {
        en_ots_auto_off_t enAutoOff; ///< @ref en_ots_auto_off_t.
        en_ots_clk_sel_t enClkSel;   ///< @ref en_ots_clk_sel_t.
        uint8_t u8ClkFreq;           ///< OTS clock freq(MHz).
    } stc_ots_init_t;

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t OTS_Init(const stc_ots_init_t *pstcInit);
    void OTS_DeInit(void);
    en_result_t OTS_StartGetTemp(float32_t *pf32Temp, uint32_t u32Timeout);

    void OTS_ITCmd(en_functional_state_t enState);
    void OTS_StartIT(void);
    float32_t OTS_GetTempIT(void);

    void OTS_SetTriggerSrc(en_event_src_t enEvent);
    en_result_t OTS_CheckSample(float32_t *pf32Temp, uint32_t u32Timeout);

    void OTS_ScalingExperiment(uint16_t *pu16Dr1, uint16_t *pu16Dr2,
                               uint16_t *pu16Ecr, float32_t *pf32A);

    void OTS_SetKM(float32_t f32K, float32_t f32M);

    //@} // OtsGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_OTS_ENABLE */

#endif /* __HC32F46X_OTS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
