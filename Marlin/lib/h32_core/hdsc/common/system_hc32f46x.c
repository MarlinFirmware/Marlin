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
/** \file system_hc32f46x.c
 **
 ** A detailed description is available at
 ** @link Hc32f46xSystemGroup Hc32f46xSystem description @endlink
 **
 **   - 2018-10-15  1.0  Zhangxl First version
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"

/**
 *******************************************************************************
 ** \addtogroup Hc32f46xSystemGroup
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('define')
 ******************************************************************************/

//@{

/**
 ******************************************************************************
 ** System Clock Frequency (Core Clock) Variable according CMSIS
 ******************************************************************************/
uint32_t SystemCoreClock = MRC_VALUE;

/**
 ******************************************************************************
 ** \brief  Setup the microcontroller system. Initialize the System and update
 ** the SystemCoreClock variable.
 **
 ** \param  None
 ** \return None
 ******************************************************************************/
void SystemInit(void)
{
    SystemCoreClockUpdate();
}

void SystemCoreClockUpdate(void) // Update SystemCoreClock variable
{
    uint8_t tmp = 0u;
    uint32_t plln = 19u, pllp = 1u, pllm = 0u, pllsource = 0u;

    tmp = M4_SYSREG->CMU_CKSWR_f.CKSW;
    switch (tmp)
    {
    case 0x00: /* use internal high speed RC */
        SystemCoreClock = HRC_VALUE;
        break;
    case 0x01: /* use internal middle speed RC */
        SystemCoreClock = MRC_VALUE;
        break;
    case 0x02: /* use internal low speed RC */
        SystemCoreClock = LRC_VALUE;
        break;
    case 0x03: /* use external high speed RC */
        SystemCoreClock = XTAL_VALUE;
        break;
    case 0x04: /* use external low speed RC */
        SystemCoreClock = XTAL32_VALUE;
        break;
    case 0x05: /* use MPLL */
        /* PLLCLK = ((pllsrc / pllm) * plln) / pllp */
        pllsource = M4_SYSREG->CMU_PLLCFGR_f.PLLSRC;
        plln = M4_SYSREG->CMU_PLLCFGR_f.MPLLN;
        pllp = M4_SYSREG->CMU_PLLCFGR_f.MPLLP;
        pllm = M4_SYSREG->CMU_PLLCFGR_f.MPLLM;
        /* use exteranl high speed OSC as PLL source */
        if (0ul == pllsource)
        {
            SystemCoreClock = (XTAL_VALUE) / (pllm + 1ul) * (plln + 1ul) / (pllp + 1ul);
        }
        /* use interanl high RC as PLL source */
        else if (1ul == pllsource)
        {
            SystemCoreClock = (HRC_VALUE) / (pllm + 1ul) * (plln + 1ul) / (pllp + 1ul);
        }
        else
        {
            /* Reserved */
        }
        break;
    }
}

//@} // UsartGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
