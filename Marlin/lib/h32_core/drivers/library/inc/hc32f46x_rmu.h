/*****************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd. ("HDSC").
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
/** \file hc32f46x_rmu.h
 **
 ** A detailed description is available at
 ** @link RmuGroup RMU description @endlink
 **
 **   - 2018-10-28  1.0  Chengy First version for Device Driver Library of RMU
 **
 ******************************************************************************/
#ifndef __HC32F46X_RMU_H__
#define __HC32F46X_RMU_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_RMU_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup RmuGroup Reset Management Unit(RMU)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief system reset cause flag
     **
     ******************************************************************************/
    typedef struct stc_rmu_rstcause
    {
        en_flag_status_t enMultiRst;     ///< Multiply reset cause
        en_flag_status_t enXtalErr;      ///< Xtal error reset
        en_flag_status_t enClkFreqErr;   ///< Clk freqence error reset
        en_flag_status_t enRamEcc;       ///< Ram ECC reset
        en_flag_status_t enRamParityErr; ///< Ram parity error reset
        en_flag_status_t enMpuErr;       ///< Mpu error reset
        en_flag_status_t enSoftware;     ///< Software reset
        en_flag_status_t enPowerDown;    ///< Power down reset
        en_flag_status_t enSwdt;         ///< Special watchdog timer reset
        en_flag_status_t enWdt;          ///< Watchdog timer reset
        en_flag_status_t enPvd2;         ///< Program voltage Dectection 2 reset
        en_flag_status_t enPvd1;         ///< Program voltage Dectection 1 reset
        en_flag_status_t enBrownOut;     ///< Brown out reset
        en_flag_status_t enRstPin;       ///< Reset pin reset
        en_flag_status_t enPowerOn;      ///< Power on reset
    } stc_rmu_rstcause_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t RMU_GetResetCause(stc_rmu_rstcause_t *pstcData);
    en_result_t RMU_ClrResetFlag(void);

    //@} // RmuGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_RMU_ENABLE */

#endif /* __HC32F46X_RMU_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
