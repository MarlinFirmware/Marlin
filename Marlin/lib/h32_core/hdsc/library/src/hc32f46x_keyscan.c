/*******************************************************************************
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
/** \file hc32f46x_keyscan.c
 **
 ** A detailed description is available at
 ** @link KeyscanGroup Keyscan module description @endlink
 **
 **   - 2018-10-17  1.0  Zhangxl First version for Device Driver Library of
 **                      key scan module
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_keyscan.h"
#include "hc32f46x_utility.h"

#if (DDL_KEYSCAN_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup KeyscanGroup
 ******************************************************************************/
//@{
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! Parameter validity check for Hiz cycle */
#define IS_VALID_HIZ_CLCYE(x) \
    (((x) == Hiz4) ||         \
     ((x) == Hiz8) ||         \
     ((x) == Hiz16) ||        \
     ((x) == Hiz32) ||        \
     ((x) == Hiz64) ||        \
     ((x) == Hiz256) ||       \
     ((x) == Hiz512) ||       \
     ((x) == Hiz1K))

/*! Parameter validity check for Low cycle */
#define IS_VALID_LOW_CLCYE(x) \
    (((x) == Low8) ||         \
     ((x) == Low16) ||        \
     ((x) == Low32) ||        \
     ((x) == Low64) ||        \
     ((x) == Low128) ||       \
     ((x) == Low256) ||       \
     ((x) == Low512) ||       \
     ((x) == Low1K) ||        \
     ((x) == Low2K) ||        \
     ((x) == Low4K) ||        \
     ((x) == Low8K) ||        \
     ((x) == Low16K) ||       \
     ((x) == Low32K) ||       \
     ((x) == Low64K) ||       \
     ((x) == Low128K) ||      \
     ((x) == Low256K) ||      \
     ((x) == Low512K) ||      \
     ((x) == Low1M) ||        \
     ((x) == Low2M) ||        \
     ((x) == Low4M) ||        \
     ((x) == Low8M) ||        \
     ((x) == Low16M))

/*! Parameter validity check for scan clock */
#define IS_VALID_SCAN_CLK(x) \
    (((x) == KeyscanHclk) || \
     ((x) == KeyscanLrc) ||  \
     ((x) == KeyscanXtal32))

/*! Parameter validity check for keyout selection */
#define IS_VALID_KEY_OUT(x) \
    (((x) == Keyout0To1) || \
     ((x) == Keyout0To2) || \
     ((x) == Keyout0To3) || \
     ((x) == Keyout0To4) || \
     ((x) == Keyout0To5) || \
     ((x) == Keyout0To6) || \
     ((x) == Keyout0To7))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief KEYSCAN initialization
 **
 ** \param [in]  pstcKeyscanConfig      KEYSCAN configure structure
 **
 ** \retval Ok                          KEYSCAN initialized
 **         ErrorInvalidMode            Uninitialized, cannot configure it properly
 **
 ******************************************************************************/
en_result_t KEYSCAN_Init(const stc_keyscan_config_t *pstcKeyscanConfig)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_HIZ_CLCYE(pstcKeyscanConfig->enHizCycle));
    DDL_ASSERT(IS_VALID_LOW_CLCYE(pstcKeyscanConfig->enLowCycle));
    DDL_ASSERT(IS_VALID_SCAN_CLK(pstcKeyscanConfig->enKeyscanClk));
    DDL_ASSERT(IS_VALID_KEY_OUT(pstcKeyscanConfig->enKeyoutSel));

    /* cannot configure keyscan control register when running */
    if (Set == M4_KEYSCAN->SER_f.SEN)
    {
        enRet = ErrorInvalidMode;
    }
    else
    {
        M4_KEYSCAN->SCR_f.T_HIZ = pstcKeyscanConfig->enHizCycle;
        M4_KEYSCAN->SCR_f.T_LLEVEL = pstcKeyscanConfig->enLowCycle;
        M4_KEYSCAN->SCR_f.CKSEL = pstcKeyscanConfig->enKeyscanClk;
        M4_KEYSCAN->SCR_f.KEYOUTSEL = pstcKeyscanConfig->enKeyoutSel;
        M4_KEYSCAN->SCR_f.KEYINSEL = pstcKeyscanConfig->u16KeyinSel;
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief KEYSCAN de-initialization
 **
 ** \param  None
 **
 ** \retval Ok                          KEYSCAN de-initialized
 **
 ******************************************************************************/
en_result_t KEYSCAN_DeInit(void)
{
    M4_KEYSCAN->SER = 0ul;
    M4_KEYSCAN->SCR = 0ul;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Start keyscan function
 **
 ** \param  None
 **
 ** \retval Ok                          Keyscan function started
 **
 ******************************************************************************/
en_result_t KEYSCAN_Start(void)
{
    M4_KEYSCAN->SER_f.SEN = Set;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Stop keyscan function
 **
 ** \param  None
 **
 ** \retval Ok                          Keyscan function stopped
 **
 ******************************************************************************/
en_result_t KEYSCAN_Stop(void)
{
    M4_KEYSCAN->SER_f.SEN = Reset;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get Key column index
 **
 ** \param  None
 **
 ** \retval uint8_t                     Index of KEYOUT
 **
 ******************************************************************************/
uint8_t KEYSCAN_GetColIdx(void)
{
    return (uint8_t)(M4_KEYSCAN->SSR_f.INDEX);
}

//@} // KeyscanGroup

#endif /* DDL_KEYSCAN_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
