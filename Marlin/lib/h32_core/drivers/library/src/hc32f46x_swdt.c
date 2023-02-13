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
/** \file hc32f46x_swdt.c
 **
 ** A detailed description is available at
 ** @link SwdtGroup Special Watchdog Counter description @endlink
 **
 **   - 2018-10-16  1.0  yangjp First version for Device Driver Library of SWDT.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_swdt.h"
#include "hc32f46x_utility.h"

#if (DDL_SWDT_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup SwdtGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*!< Parameter valid check for flag type */
#define IS_VALID_FLAG_TYPE(x)           \
    ((SwdtFlagCountUnderflow == (x)) || \
     (SwdtFlagRefreshError == (x)))

/*!< SWDT_RR register refresh key */
#define SWDT_REFRESH_START_KEY ((uint16_t)0x0123)
#define SWDT_REFRESH_END_KEY_ ((uint16_t)0x3210)

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
 ** \brief SWDT refresh counter
 **
 ** \param [in] None
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t SWDT_RefreshCounter(void)
{
    en_result_t enRet = Ok;

    M4_SWDT->RR = SWDT_REFRESH_START_KEY;
    M4_SWDT->RR = SWDT_REFRESH_END_KEY_;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get SWDT counter current count value
 **
 ** \param [in] None
 **
 ** \retval uint16_t                    SWDT counter current count value
 **
 ******************************************************************************/
uint16_t SWDT_GetCountValue(void)
{
    return ((uint16_t)M4_SWDT->SR_f.CNT);
}

/**
 *******************************************************************************
 ** \brief Get SWDT flag status
 **
 ** \param [in] enFlag                  SWDT flag type
 ** \arg SwdtFlagCountUnderflow         Count underflow flag
 ** \arg SwdtFlagRefreshError           Refresh error flag
 **
 ** \retval Set                         Flag is set
 ** \retval Reset                       Flag is reset
 **
 ******************************************************************************/
en_flag_status_t SWDT_GetFlag(en_swdt_flag_type_t enFlag)
{
    en_flag_status_t enFlagSta = Reset;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_FLAG_TYPE(enFlag));

    switch (enFlag)
    {
    case SwdtFlagCountUnderflow:
        enFlagSta = (en_flag_status_t)M4_SWDT->SR_f.UDF;
        break;
    case SwdtFlagRefreshError:
        enFlagSta = (en_flag_status_t)M4_SWDT->SR_f.REF;
        break;
    default:
        break;
    }

    return enFlagSta;
}

/**
 *******************************************************************************
 ** \brief Clear SWDT flag status
 **
 ** \param [in] enFlag                  SWDT flag type
 ** \arg SwdtFlagCountUnderflow         Count underflow flag
 ** \arg SwdtFlagRefreshError           Refresh error flag
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t SWDT_ClearFlag(en_swdt_flag_type_t enFlag)
{
    en_result_t enRet = Ok;

    /* Check parameters */
    DDL_ASSERT(IS_VALID_FLAG_TYPE(enFlag));

    switch (enFlag)
    {
    case SwdtFlagCountUnderflow:
        M4_SWDT->SR_f.UDF = 0u;
        break;
    case SwdtFlagRefreshError:
        M4_SWDT->SR_f.REF = 0u;
        break;
    default:
        break;
    }

    return enRet;
}

//@} // SwdtGroup

#endif /* DDL_SWDT_ENABLE */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
