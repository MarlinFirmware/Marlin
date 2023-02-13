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
/** \file hc32f46x_sram.c
 **
 ** A detailed description is available at
 ** @link SramGroup Internal SRAM module description @endlink
 **
 **   - 2018-10-17  1.0  Zhangxl First version for Device Driver Library of
 **                      SRAM module
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_sram.h"
#include "hc32f46x_utility.h"

#if (DDL_SRAM_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup SramGroup
 ******************************************************************************/
//@{
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! Parameter validity check for ECC/Parity error handling. */
#define IS_VALID_ERR_OP(x) \
    (((x) == SramNmi) ||   \
     ((x) == SramReset))

/*! Parameter validity check for SRAM ECC mode */
#define IS_VALID_ECC_MD(x) \
    (((x) == EccMode0) ||  \
     ((x) == EccMode1) ||  \
     ((x) == EccMode2) ||  \
     ((x) == EccMode3))

/*! Parameter validity check for SRAM Index */
#define IS_VALID_INDEX(x)  \
    (((x) == Sram12Idx) || \
     ((x) == Sram3Idx) ||  \
     ((x) == SramHsIdx) || \
     ((x) == SramRetIdx))

/*! Parameter validity check for SRAM R/W wait cycle */
#define IS_VALID_WAIT_CYCLE(x) \
    (((x) == SramCycle1) ||    \
     ((x) == SramCycle2) ||    \
     ((x) == SramCycle3) ||    \
     ((x) == SramCycle4) ||    \
     ((x) == SramCycle5) ||    \
     ((x) == SramCycle6) ||    \
     ((x) == SramCycle7) ||    \
     ((x) == SramCycle8))

/*! Parameter validity check for SRAM error status */
#define IS_VALID_ERR(x)          \
    (((x) == Sram3EccErr1) ||    \
     ((x) == Sram3EccErr2) ||    \
     ((x) == Sram12ParityErr) || \
     ((x) == SramHSParityErr) || \
     ((x) == SramRetParityErr))

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
 ** \brief SRAM read, write wait cycle register disable function
 **
 ** \param  None
 **
 ** \retval Ok                          SRAM R/W wait cycle register disabled
 **
 ******************************************************************************/
en_result_t SRAM_WT_Disable(void)
{
    M4_SRAMC->WTPR = 0x76u;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief SRAM read, write wait cycle register enable function
 **
 ** \param  None
 **
 ** \retval Ok                          SRAM R/W wait cycle register enabled
 **
 ******************************************************************************/
en_result_t SRAM_WT_Enable(void)
{
    M4_SRAMC->WTPR = 0x77u;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief SRAM ECC/Parity check register disable function
 **
 ** \param  None
 **
 ** \retval Ok                          SRAM ECC/Parity check register disabled
 **
 ******************************************************************************/
en_result_t SRAM_CK_Disable(void)
{
    M4_SRAMC->CKPR = 0x76u;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief SRAM ECC/Parity check register enable function
 **
 ** \param  None
 **
 ** \retval Ok                          SRAM ECC/Parity check register enabled
 **
 ******************************************************************************/
en_result_t SRAM_CK_Enable(void)
{
    M4_SRAMC->CKPR = 0x77u;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get SRAM ECC/Parity error status flag
 **
 ** \param  [in] enSramErrStatus        SRAM error status, This parameter can be
 **                                     some values of @ref en_sram_err_status_t
 **
 ** \retval Set                         Corresponding error occurs
 **         Reset                       Corresponding error not occurs
 **
 ******************************************************************************/
en_flag_status_t SRAM_GetStatus(en_sram_err_status_t enSramErrStatus)
{
    DDL_ASSERT(IS_VALID_ERR(enSramErrStatus));
    if (true == !!(enSramErrStatus & M4_SRAMC->CKSR))
    {
        return Set;
    }
    else
    {
        return Reset;
    }
}

/**
 *******************************************************************************
 ** \brief Clear SRAM ECC/Parity error status flag
 **
 ** \param  [in] enSramErrStatus        SRAM error status, This parameter can be
 **                                     some values of @ref en_sram_err_status_t
 **
 ** \retval Ok                          Corresponding error flag be cleared
 **         ErrorInvalidParameter       Invalid parameter
 **
 ******************************************************************************/
en_result_t SRAM_ClrStatus(en_sram_err_status_t enSramErrStatus)
{
    DDL_ASSERT(IS_VALID_ERR(enSramErrStatus));
    M4_SRAMC->CKSR |= enSramErrStatus;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief SRAM initialization
 **
 ** \param [in]  pstcSramConfig         SRAM configure structure
 **
 ** \retval Ok                          SRAM initialized
 **         ErrorInvalidParameter       Invalid parameter
 **
 ******************************************************************************/
en_result_t SRAM_Init(const stc_sram_config_t *pstcSramConfig)
{
    uint8_t i = 0u;
    uint8_t u8TmpIdx;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_WAIT_CYCLE(pstcSramConfig->enSramRC));
    DDL_ASSERT(IS_VALID_WAIT_CYCLE(pstcSramConfig->enSramWC));
    DDL_ASSERT(IS_VALID_ECC_MD(pstcSramConfig->enSramEccMode));
    DDL_ASSERT(IS_VALID_ERR_OP(pstcSramConfig->enSramEccOp));
    DDL_ASSERT(IS_VALID_ERR_OP(pstcSramConfig->enSramPyOp));

    u8TmpIdx = pstcSramConfig->u8SramIdx;

    if (0u == u8TmpIdx)
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        SRAM_WT_Enable();
        SRAM_CK_Enable();
        for (i = 0u; i < 4u; i++)
        {
            if (true == (u8TmpIdx & 0x01u))
            {
                M4_SRAMC->WTCR |= (pstcSramConfig->enSramRC |
                                   (pstcSramConfig->enSramWC << 4ul))
                                  << (i * 8ul);
            }
            u8TmpIdx >>= 1u;
        }
        /* SRAM3 ECC config */
        if (pstcSramConfig->u8SramIdx & Sram3Idx)
        {
            M4_SRAMC->CKCR_f.ECCMOD = pstcSramConfig->enSramEccMode;
            M4_SRAMC->CKCR_f.ECCOAD = pstcSramConfig->enSramEccOp;
        }
        /* SRAM1/2/HS/Ret parity config */
        else
        {
            M4_SRAMC->CKCR_f.PYOAD = pstcSramConfig->enSramPyOp;
        }

        SRAM_WT_Disable();
        SRAM_CK_Disable();
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief SRAM de-initialization
 **
 ** \param  None
 **
 ** \retval Ok                          SRAM de-initialized
 **
 ******************************************************************************/
en_result_t SRAM_DeInit(void)
{
    /* SRAM R/W wait register */
    M4_SRAMC->WTPR = 0x77ul;
    M4_SRAMC->WTCR = 0ul;
    M4_SRAMC->WTPR = 0x76ul;

    /* SRAM check register */
    M4_SRAMC->CKPR = 0x77ul;
    M4_SRAMC->CKCR = 0ul;
    M4_SRAMC->CKPR = 0x76ul;

    /* SRAM status register */
    M4_SRAMC->CKSR = 0x1Ful;

    return Ok;
}

//@} // SramGroup

#endif /* DDL_SRAM_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
