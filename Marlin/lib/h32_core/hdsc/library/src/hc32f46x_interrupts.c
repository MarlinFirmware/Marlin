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
/*****************************************************************************/
/** \file hc32f46x_interrupts.c
 **
 ** A detailed description is available at
 ** @link InterruptGroup Interrupt description @endlink
 **
 **   - 2018-10-12  1.0  Zhangxl First version for Device Driver Library of
 **     interrupt.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_interrupts.h"
#include "hc32f46x_utility.h"

#if (DDL_INTERRUPTS_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup InterruptGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*! Parameter validity check for null pointer. */
#define IS_NULL_POINT(x) (NULL != (x))

/*! Parameter validity check for valid wakeup source from stop mode. */
#define IS_VALID_WKUP_SRC(x)     \
    (((x) == INT_USART1_WUPI) || \
     ((x) == INT_TMR01_GCMA) ||  \
     ((x) == INT_RTC_ALM) ||     \
     ((x) == INT_RTC_PRD) ||     \
     ((x) == INT_WKTM_PRD) ||    \
     ((x) == INT_ACMP1) ||       \
     ((x) == INT_PVD_PVD1) ||    \
     ((x) == INT_PVD_PVD2) ||    \
     ((x) == INT_SWDT_REFUDF) || \
     ((x) == INT_PORT_EIRQ0) ||  \
     ((x) == INT_PORT_EIRQ1) ||  \
     ((x) == INT_PORT_EIRQ2) ||  \
     ((x) == INT_PORT_EIRQ3) ||  \
     ((x) == INT_PORT_EIRQ4) ||  \
     ((x) == INT_PORT_EIRQ5) ||  \
     ((x) == INT_PORT_EIRQ6) ||  \
     ((x) == INT_PORT_EIRQ7) ||  \
     ((x) == INT_PORT_EIRQ8) ||  \
     ((x) == INT_PORT_EIRQ9) ||  \
     ((x) == INT_PORT_EIRQ10) || \
     ((x) == INT_PORT_EIRQ11) || \
     ((x) == INT_PORT_EIRQ12) || \
     ((x) == INT_PORT_EIRQ13) || \
     ((x) == INT_PORT_EIRQ14) || \
     ((x) == INT_PORT_EIRQ15))

/*! Max IRQ Handler. */
#define IRQ_NUM_MAX (128u)

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
func_ptr_t IrqHandler[IRQ_NUM_MAX] = {NULL};

    /**
     *******************************************************************************
     ** \brief IRQ Registration
     **
     ** param [in]                          pstcIrqRegiConf, IRQ registration
     **                                     configure structure
     **
     ** retval                              Ok, IRQ register successfully.
     **                                     ErrorInvalidParameter, IRQ No. and
     **                                     Vector No. are not match.
     **                                     ErrorUninitialized, Make sure the
     **                                     Interrupt select register (INTSEL) is
     **                                     default value (0x1FFu) before setting.
     **
     *****************************************************************************/
    en_result_t enIrqRegistration(const stc_irq_regi_conf_t *pstcIrqRegiConf)
    {
        // todo, assert ...
        stc_intc_sel_field_t *stcIntSel;
        en_result_t enRet = Ok;

        // DDL_ASSERT(NULL != pstcIrqRegiConf->pfnCallback);
        DDL_ASSERT(IS_NULL_POINT(pstcIrqRegiConf->pfnCallback));

        /* IRQ032~127 whether out of range */
        if (((((pstcIrqRegiConf->enIntSrc / 32) * 6 + 32) > pstcIrqRegiConf->enIRQn) ||
             (((pstcIrqRegiConf->enIntSrc / 32) * 6 + 37) < pstcIrqRegiConf->enIRQn)) &&
            (pstcIrqRegiConf->enIRQn >= 32))
        {
            enRet = ErrorInvalidParameter;
        }
        else
        {
            stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(&M4_INTC->SEL0) +
                                                 (4u * pstcIrqRegiConf->enIRQn));
            if (0x1FFu == stcIntSel->INTSEL)
            {
                stcIntSel->INTSEL = pstcIrqRegiConf->enIntSrc;
                IrqHandler[pstcIrqRegiConf->enIRQn] = pstcIrqRegiConf->pfnCallback;
            }
            else
            {
                enRet = ErrorUninitialized;
            }
        }
        return enRet;
    }

    /**
     *******************************************************************************
     ** \brief IRQ Resign
     **
     ** param [in]                          enIRQn, IRQ enumunation (Int000_IRQn ~
     **                                     Int127_IRQn
     **
     ** retval                              Ok, IRQ resign sucessfully.
     **                                     ErrorInvalidParameter, IRQ No. is out
     **                                     of range
     **
     *****************************************************************************/
    en_result_t enIrqResign(IRQn_Type enIRQn)
    {
        stc_intc_sel_field_t *stcIntSel;
        en_result_t enRet = Ok;

        if ((enIRQn < Int000_IRQn) || (enIRQn > Int127_IRQn))
        {
            enRet = ErrorInvalidParameter;
        }
        else
        {
            stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(&M4_INTC->SEL0) + (4ul * enIRQn));
            stcIntSel->INTSEL = 0x1FFu;
            IrqHandler[enIRQn] = NULL;
        }
        return enRet;
    }

    /**
     *******************************************************************************
     ** \brief Share IRQ handler enable
     **
     ** param [in]                          enIntSrc, interrupt souce, This parameter
     **                                     can be any value of @ref en_int_src_t
     **
     ** retval                              Ok
     **
     ******************************************************************************/
    en_result_t enShareIrqEnable(en_int_src_t enIntSrc)
    {
        uint32_t *VSSELx;

        // todo assert

        VSSELx = (uint32_t *)(((uint32_t)&M4_INTC->VSSEL128) + (4u * (enIntSrc / 32u)));
        *VSSELx |= (uint32_t)(1ul << (enIntSrc & 0x1Fu));

        return Ok;
    }

    /**
     *******************************************************************************
     ** \brief Share IRQ handler disable
     **
     ** param [in]                          enIntSrc, interrupt souce, This parameter
     **                                     can be any value of @ref en_int_src_t
     **
     ** retval                              Ok
     **
     ******************************************************************************/
    en_result_t enShareIrqDisable(en_int_src_t enIntSrc)
    {
        uint32_t *VSSELx;

        // todo assert

        VSSELx = (uint32_t *)(((uint32_t)&M4_INTC->VSSEL128) + (4u * (enIntSrc / 32u)));
        *VSSELx &= ~(uint32_t)(1ul << (enIntSrc & 0x1Fu));

        return Ok;
    }

    /**
     *******************************************************************************
     ** \brief Enable stop mode wakeup source
     **
     ** param [in]                          u32WakeupSrc, This parameter can be any
     **                                     composed value of @ref en_int_wkup_src_t
     **
     ** retval                              Ok, corresponding wakeup source be enabled
     **                                     ErrorInvalidParameter, parameter with
     **                                     non-definition bits
     **
     ******************************************************************************/
    en_result_t enIntWakeupEnable(uint32_t u32WakeupSrc)
    {
        en_result_t enRet = Ok;
        if (0ul != (u32WakeupSrc & 0xFD000000ul))
        {
            enRet = ErrorInvalidParameter;
        }
        else
        {
            M4_INTC->WUPEN |= u32WakeupSrc;
        }
        return enRet;
    }

    /**
     *******************************************************************************
     ** \brief Disable stop mode wakeup source
     **
     ** param [in]                          u32WakeupSrc, This parameter can be any
     **                                     composed value of @ref en_int_wkup_src_t
     **
     ** retval                              Ok, corresponding wakeup source be disabled
     **                                     ErrorInvalidParameter, parameter with
     **                                     non-definition bits
     **
     ******************************************************************************/
    en_result_t enIntWakeupDisable(uint32_t u32WakeupSrc)
    {
        en_result_t enRet = Ok;
        if (0ul != (u32WakeupSrc & 0xFD000000u))
        {
            enRet = ErrorInvalidParameter;
        }
        else
        {
            M4_INTC->WUPEN &= ~u32WakeupSrc;
        }
        return enRet;
    }

    /**
     *******************************************************************************
     ** \brief Event enable
     **
     ** param [in]                          u32Event, This parameter can be
     **                                     any composed value of @ref en_evt_t
     **
     ** retval                              Ok, corresponding event Ch. be enabled
     **
     ******************************************************************************/
    en_result_t enEventEnable(uint32_t u32Event)
    {
        M4_INTC->EVTER |= u32Event;
        return Ok;
    }

    /**
     *******************************************************************************
     ** \brief Event enable
     **
     ** param [in]                          u32Event, This parameter can be
     **                                     any composed value of @ref en_evt_t
     **
     ** retval                              Ok, corresponding event Ch. be disabled
     **
     ******************************************************************************/
    en_result_t enEventDisable(uint32_t u32Event)
    {
        M4_INTC->EVTER &= ~u32Event;
        return Ok;
    }

    /**
     *******************************************************************************
     ** \brief Interrupt enable
     **
     *  param [in]                          u32Int, This parameter can be any composed
     *                                      value of @ref en_int_t
     **
     ** retval                              Ok, corresponding interrupt vector be enabled
     **
     ******************************************************************************/
    en_result_t enIntEnable(uint32_t u32Int)
    {
        M4_INTC->IER |= u32Int;
        return Ok;
    }

    /**
     *******************************************************************************
     ** \brief Interrupt disable
     **
     *  param [in]                          u32Int, This parameter can be any composed
     *                                      value of @ref en_int_t
     **
     ** retval                              Ok, corresponding interrupt vector be disabled
     **
     ******************************************************************************/
    en_result_t enIntDisable(uint32_t u32Int)
    {
        M4_INTC->IER &= ~u32Int;
        return Ok;
    }

    /**
     *******************************************************************************
     ** \brief NMI IRQ handler
     **
     ******************************************************************************/
    void NMI_Handler(void)
    {
        NMI_IrqHandler();
    }

    /**
     *******************************************************************************
     ** \brief Hard Fault IRQ handler
     **
     ******************************************************************************/
    void HardFault_Handler(void)
    {
        HardFault_IrqHandler();
    }

    /**
     *******************************************************************************
     ** \brief MPU Fault IRQ handler
     **
     ******************************************************************************/
    void MemManage_Handler(void)
    {
        MemManage_IrqHandler();
    }

    /**
     *******************************************************************************
     ** \brief Bus Fault IRQ handler
     **
     ******************************************************************************/
    void BusFault_Handler(void)
    {
        BusFault_IrqHandler();
    }

    /**
     *******************************************************************************
     ** \brief Usage Fault IRQ handler
     **
     ******************************************************************************/
    void UsageFault_Handler(void)
    {
        UsageFault_IrqHandler();
    }

    /**
     *******************************************************************************
     ** \brief SVCall IRQ handler
     **
     ******************************************************************************/
    void SVC_Handler(void)
    {
        SVC_IrqHandler();
    }

    /**
     *******************************************************************************
     ** \brief DebugMon IRQ handler
     **
     ******************************************************************************/
    void DebugMon_Handler(void)
    {
        DebugMon_IrqHandler();
    }

    /**
     *******************************************************************************
     ** \brief PendSV IRQ handler
     **
     ******************************************************************************/
    void PendSV_Handler(void)
    {
        PendSV_IrqHandler();
    }

    /**
     *******************************************************************************
     ** \brief Systick IRQ handler
     **
     ******************************************************************************/
    void SysTick_Handler(void)
    {
        SysTick_IrqHandler();
    }

    /**
     *******************************************************************************
     ** \brief Int No.000 IRQ handler
     **
     ******************************************************************************/
    void IRQ000_Handler(void)
    {
        if (NULL != IrqHandler[Int000_IRQn])
        {
            IrqHandler[Int000_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.001 IRQ handler
     **
     ******************************************************************************/
    void IRQ001_Handler(void)
    {
        if (NULL != IrqHandler[Int001_IRQn])
        {
            IrqHandler[Int001_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.002 IRQ handler
     **
     ******************************************************************************/
    void IRQ002_Handler(void)
    {
        if (NULL != IrqHandler[Int002_IRQn])
        {
            IrqHandler[Int002_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.003 IRQ handler
     **
     ******************************************************************************/
    void IRQ003_Handler(void)
    {
        if (NULL != IrqHandler[Int003_IRQn])
        {
            IrqHandler[Int003_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.004 IRQ handler
     **
     ******************************************************************************/
    void IRQ004_Handler(void)
    {
        if (NULL != IrqHandler[Int004_IRQn])
        {
            IrqHandler[Int004_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.005 IRQ handler
     **
     ******************************************************************************/
    void IRQ005_Handler(void)
    {
        if (NULL != IrqHandler[Int005_IRQn])
        {
            IrqHandler[Int005_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.006 IRQ handler
     **
     ******************************************************************************/
    void IRQ006_Handler(void)
    {
        if (NULL != IrqHandler[Int006_IRQn])
        {
            IrqHandler[Int006_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.007 IRQ handler
     **
     ******************************************************************************/
    void IRQ007_Handler(void)
    {
        if (NULL != IrqHandler[Int007_IRQn])
        {
            IrqHandler[Int007_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.008 IRQ handler
     **
     ******************************************************************************/
    void IRQ008_Handler(void)
    {
        if (NULL != IrqHandler[Int008_IRQn])
        {
            IrqHandler[Int008_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.009 IRQ handler
     **
     ******************************************************************************/
    void IRQ009_Handler(void)
    {
        if (NULL != IrqHandler[Int009_IRQn])
        {
            IrqHandler[Int009_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.010 IRQ handler
     **
     ******************************************************************************/
    void IRQ010_Handler(void)
    {
        if (NULL != IrqHandler[Int010_IRQn])
        {
            IrqHandler[Int010_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.011 IRQ handler
     **
     ******************************************************************************/
    void IRQ011_Handler(void)
    {
        if (NULL != IrqHandler[Int011_IRQn])
        {
            IrqHandler[Int011_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.012 IRQ handler
     **
     ******************************************************************************/
    void IRQ012_Handler(void)
    {
        if (NULL != IrqHandler[Int012_IRQn])
        {
            IrqHandler[Int012_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.013 IRQ handler
     **
     ******************************************************************************/
    void IRQ013_Handler(void)
    {
        if (NULL != IrqHandler[Int013_IRQn])
        {
            IrqHandler[Int013_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.014 IRQ handler
     **
     ******************************************************************************/
    void IRQ014_Handler(void)
    {
        if (NULL != IrqHandler[Int014_IRQn])
        {
            IrqHandler[Int014_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.015 IRQ handler
     **
     ******************************************************************************/
    void IRQ015_Handler(void)
    {
        if (NULL != IrqHandler[Int015_IRQn])
        {
            IrqHandler[Int015_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.016 IRQ handler
     **
     ******************************************************************************/
    void IRQ016_Handler(void)
    {
        if (NULL != IrqHandler[Int016_IRQn])
        {
            IrqHandler[Int016_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.017 IRQ handler
     **
     ******************************************************************************/
    void IRQ017_Handler(void)
    {
        if (NULL != IrqHandler[Int017_IRQn])
        {
            IrqHandler[Int017_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.018 IRQ handler
     **
     ******************************************************************************/
    void IRQ018_Handler(void)
    {
        if (NULL != IrqHandler[Int018_IRQn])
        {
            IrqHandler[Int018_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.019 IRQ handler
     **
     ******************************************************************************/
    void IRQ019_Handler(void)
    {
        if (NULL != IrqHandler[Int019_IRQn])
        {
            IrqHandler[Int019_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.020 IRQ handler
     **
     ******************************************************************************/
    void IRQ020_Handler(void)
    {
        if (NULL != IrqHandler[Int020_IRQn])
        {
            IrqHandler[Int020_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.021 IRQ handler
     **
     ******************************************************************************/
    void IRQ021_Handler(void)
    {
        if (NULL != IrqHandler[Int021_IRQn])
        {
            IrqHandler[Int021_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.022 IRQ handler
     **
     ******************************************************************************/
    void IRQ022_Handler(void)
    {
        if (NULL != IrqHandler[Int022_IRQn])
        {
            IrqHandler[Int022_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.023 IRQ handler
     **
     ******************************************************************************/
    void IRQ023_Handler(void)
    {
        if (NULL != IrqHandler[Int023_IRQn])
        {
            IrqHandler[Int023_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.024 IRQ handler
     **
     ******************************************************************************/
    void IRQ024_Handler(void)
    {
        if (NULL != IrqHandler[Int024_IRQn])
        {
            IrqHandler[Int024_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.025 IRQ handler
     **
     ******************************************************************************/
    void IRQ025_Handler(void)
    {
        if (NULL != IrqHandler[Int025_IRQn])
        {
            IrqHandler[Int025_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.026 IRQ handler
     **
     ******************************************************************************/
    void IRQ026_Handler(void)
    {
        if (NULL != IrqHandler[Int026_IRQn])
        {
            IrqHandler[Int026_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.027 IRQ handler
     **
     ******************************************************************************/
    void IRQ027_Handler(void)
    {
        if (NULL != IrqHandler[Int027_IRQn])
        {
            IrqHandler[Int027_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.028 IRQ handler
     **
     ******************************************************************************/
    void IRQ028_Handler(void)
    {
        if (NULL != IrqHandler[Int028_IRQn])
        {
            IrqHandler[Int028_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.029 IRQ handler
     **
     ******************************************************************************/
    void IRQ029_Handler(void)
    {
        if (NULL != IrqHandler[Int029_IRQn])
        {
            IrqHandler[Int029_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.030 IRQ handler
     **
     ******************************************************************************/
    void IRQ030_Handler(void)
    {
        if (NULL != IrqHandler[Int030_IRQn])
        {
            IrqHandler[Int030_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.031 IRQ handler
     **
     ******************************************************************************/
    void IRQ031_Handler(void)
    {
        if (NULL != IrqHandler[Int031_IRQn])
        {
            IrqHandler[Int031_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.032 IRQ handler
     **
     ******************************************************************************/
    void IRQ032_Handler(void)
    {
        if (NULL != IrqHandler[Int032_IRQn])
        {
            IrqHandler[Int032_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.033 IRQ handler
     **
     ******************************************************************************/
    void IRQ033_Handler(void)
    {
        if (NULL != IrqHandler[Int033_IRQn])
        {
            IrqHandler[Int033_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.034 IRQ handler
     **
     ******************************************************************************/
    void IRQ034_Handler(void)
    {
        if (NULL != IrqHandler[Int034_IRQn])
        {
            IrqHandler[Int034_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.035 IRQ handler
     **
     ******************************************************************************/
    void IRQ035_Handler(void)
    {
        if (NULL != IrqHandler[Int035_IRQn])
        {
            IrqHandler[Int035_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.036 IRQ handler
     **
     ******************************************************************************/
    void IRQ036_Handler(void)
    {
        if (NULL != IrqHandler[Int036_IRQn])
        {
            IrqHandler[Int036_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.037 IRQ handler
     **
     ******************************************************************************/
    void IRQ037_Handler(void)
    {
        if (NULL != IrqHandler[Int037_IRQn])
        {
            IrqHandler[Int037_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.038 IRQ handler
     **
     ******************************************************************************/
    void IRQ038_Handler(void)
    {
        if (NULL != IrqHandler[Int038_IRQn])
        {
            IrqHandler[Int038_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.039 IRQ handler
     **
     ******************************************************************************/
    void IRQ039_Handler(void)
    {
        if (NULL != IrqHandler[Int039_IRQn])
        {
            IrqHandler[Int039_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.040 IRQ handler
     **
     ******************************************************************************/
    void IRQ040_Handler(void)
    {
        if (NULL != IrqHandler[Int040_IRQn])
        {
            IrqHandler[Int040_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.041 IRQ handler
     **
     ******************************************************************************/
    void IRQ041_Handler(void)
    {
        if (NULL != IrqHandler[Int041_IRQn])
        {
            IrqHandler[Int041_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.042 IRQ handler
     **
     ******************************************************************************/
    void IRQ042_Handler(void)
    {
        if (NULL != IrqHandler[Int042_IRQn])
        {
            IrqHandler[Int042_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.043 IRQ handler
     **
     ******************************************************************************/
    void IRQ043_Handler(void)
    {
        if (NULL != IrqHandler[Int043_IRQn])
        {
            IrqHandler[Int043_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.044 IRQ handler
     **
     ******************************************************************************/
    void IRQ044_Handler(void)
    {
        if (NULL != IrqHandler[Int044_IRQn])
        {
            IrqHandler[Int044_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.045 IRQ handler
     **
     ******************************************************************************/
    void IRQ045_Handler(void)
    {
        if (NULL != IrqHandler[Int045_IRQn])
        {
            IrqHandler[Int045_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.046 IRQ handler
     **
     ******************************************************************************/
    void IRQ046_Handler(void)
    {
        if (NULL != IrqHandler[Int046_IRQn])
        {
            IrqHandler[Int046_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.047 IRQ handler
     **
     ******************************************************************************/
    void IRQ047_Handler(void)
    {
        if (NULL != IrqHandler[Int047_IRQn])
        {
            IrqHandler[Int047_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.048 IRQ handler
     **
     ******************************************************************************/
    void IRQ048_Handler(void)
    {
        if (NULL != IrqHandler[Int048_IRQn])
        {
            IrqHandler[Int048_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.049 IRQ handler
     **
     ******************************************************************************/
    void IRQ049_Handler(void)
    {
        if (NULL != IrqHandler[Int049_IRQn])
        {
            IrqHandler[Int049_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.050 IRQ handler
     **
     ******************************************************************************/
    void IRQ050_Handler(void)
    {
        if (NULL != IrqHandler[Int050_IRQn])
        {
            IrqHandler[Int050_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.051 IRQ handler
     **
     ******************************************************************************/
    void IRQ051_Handler(void)
    {
        if (NULL != IrqHandler[Int051_IRQn])
        {
            IrqHandler[Int051_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.052 IRQ handler
     **
     ******************************************************************************/
    void IRQ052_Handler(void)
    {
        if (NULL != IrqHandler[Int052_IRQn])
        {
            IrqHandler[Int052_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.053 IRQ handler
     **
     ******************************************************************************/
    void IRQ053_Handler(void)
    {
        if (NULL != IrqHandler[Int053_IRQn])
        {
            IrqHandler[Int053_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.054 IRQ handler
     **
     ******************************************************************************/
    void IRQ054_Handler(void)
    {
        if (NULL != IrqHandler[Int054_IRQn])
        {
            IrqHandler[Int054_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.055 IRQ handler
     **
     ******************************************************************************/
    void IRQ055_Handler(void)
    {
        if (NULL != IrqHandler[Int055_IRQn])
        {
            IrqHandler[Int055_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.056 IRQ handler
     **
     ******************************************************************************/
    void IRQ056_Handler(void)
    {
        if (NULL != IrqHandler[Int056_IRQn])
        {
            IrqHandler[Int056_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.057 IRQ handler
     **
     ******************************************************************************/
    void IRQ057_Handler(void)
    {
        if (NULL != IrqHandler[Int057_IRQn])
        {
            IrqHandler[Int057_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.058 IRQ handler
     **
     ******************************************************************************/
    void IRQ058_Handler(void)
    {
        if (NULL != IrqHandler[Int058_IRQn])
        {
            IrqHandler[Int058_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.059 IRQ handler
     **
     ******************************************************************************/
    void IRQ059_Handler(void)
    {
        if (NULL != IrqHandler[Int059_IRQn])
        {
            IrqHandler[Int059_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.060 IRQ handler
     **
     ******************************************************************************/
    void IRQ060_Handler(void)
    {
        if (NULL != IrqHandler[Int060_IRQn])
        {
            IrqHandler[Int060_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.061 IRQ handler
     **
     ******************************************************************************/
    void IRQ061_Handler(void)
    {
        if (NULL != IrqHandler[Int061_IRQn])
        {
            IrqHandler[Int061_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.062 IRQ handler
     **
     ******************************************************************************/
    void IRQ062_Handler(void)
    {
        if (NULL != IrqHandler[Int062_IRQn])
        {
            IrqHandler[Int062_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.063 IRQ handler
     **
     ******************************************************************************/
    void IRQ063_Handler(void)
    {
        if (NULL != IrqHandler[Int063_IRQn])
        {
            IrqHandler[Int063_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.064 IRQ handler
     **
     ******************************************************************************/
    void IRQ064_Handler(void)
    {
        if (NULL != IrqHandler[Int064_IRQn])
        {
            IrqHandler[Int064_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.065 IRQ handler
     **
     ******************************************************************************/
    void IRQ065_Handler(void)
    {
        if (NULL != IrqHandler[Int065_IRQn])
        {
            IrqHandler[Int065_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.066 IRQ handler
     **
     ******************************************************************************/
    void IRQ066_Handler(void)
    {
        if (NULL != IrqHandler[Int066_IRQn])
        {
            IrqHandler[Int066_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.067 IRQ handler
     **
     ******************************************************************************/
    void IRQ067_Handler(void)
    {
        if (NULL != IrqHandler[Int067_IRQn])
        {
            IrqHandler[Int067_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.068 IRQ handler
     **
     ******************************************************************************/
    void IRQ068_Handler(void)
    {
        if (NULL != IrqHandler[Int068_IRQn])
        {
            IrqHandler[Int068_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.069 IRQ handler
     **
     ******************************************************************************/
    void IRQ069_Handler(void)
    {
        if (NULL != IrqHandler[Int069_IRQn])
        {
            IrqHandler[Int069_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.070 IRQ handler
     **
     ******************************************************************************/
    void IRQ070_Handler(void)
    {
        if (NULL != IrqHandler[Int070_IRQn])
        {
            IrqHandler[Int070_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.071 IRQ handler
     **
     ******************************************************************************/
    void IRQ071_Handler(void)
    {
        if (NULL != IrqHandler[Int071_IRQn])
        {
            IrqHandler[Int071_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.072 IRQ handler
     **
     ******************************************************************************/
    void IRQ072_Handler(void)
    {
        if (NULL != IrqHandler[Int072_IRQn])
        {
            IrqHandler[Int072_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.073 IRQ handler
     **
     ******************************************************************************/
    void IRQ073_Handler(void)
    {
        if (NULL != IrqHandler[Int073_IRQn])
        {
            IrqHandler[Int073_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.074 IRQ handler
     **
     ******************************************************************************/
    void IRQ074_Handler(void)
    {
        if (NULL != IrqHandler[Int074_IRQn])
        {
            IrqHandler[Int074_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.075 IRQ handler
     **
     ******************************************************************************/
    void IRQ075_Handler(void)
    {
        if (NULL != IrqHandler[Int075_IRQn])
        {
            IrqHandler[Int075_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.076 IRQ handler
     **
     ******************************************************************************/
    void IRQ076_Handler(void)
    {
        if (NULL != IrqHandler[Int076_IRQn])
        {
            IrqHandler[Int076_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.077 IRQ handler
     **
     ******************************************************************************/
    void IRQ077_Handler(void)
    {
        if (NULL != IrqHandler[Int077_IRQn])
        {
            IrqHandler[Int077_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.078 IRQ handler
     **
     ******************************************************************************/
    void IRQ078_Handler(void)
    {
        if (NULL != IrqHandler[Int078_IRQn])
        {
            IrqHandler[Int078_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.079 IRQ handler
     **
     ******************************************************************************/
    void IRQ079_Handler(void)
    {
        if (NULL != IrqHandler[Int079_IRQn])
        {
            IrqHandler[Int079_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.080 IRQ handler
     **
     ******************************************************************************/
    void IRQ080_Handler(void)
    {
        if (NULL != IrqHandler[Int080_IRQn])
        {
            IrqHandler[Int080_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.081 IRQ handler
     **
     ******************************************************************************/
    void IRQ081_Handler(void)
    {
        if (NULL != IrqHandler[Int081_IRQn])
        {
            IrqHandler[Int081_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.082 IRQ handler
     **
     ******************************************************************************/
    void IRQ082_Handler(void)
    {
        if (NULL != IrqHandler[Int082_IRQn])
        {
            IrqHandler[Int082_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.083 IRQ handler
     **
     ******************************************************************************/
    void IRQ083_Handler(void)
    {
        if (NULL != IrqHandler[Int083_IRQn])
        {
            IrqHandler[Int083_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.084 IRQ handler
     **
     ******************************************************************************/
    void IRQ084_Handler(void)
    {
        if (NULL != IrqHandler[Int084_IRQn])
        {
            IrqHandler[Int084_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.085 IRQ handler
     **
     ******************************************************************************/
    void IRQ085_Handler(void)
    {
        if (NULL != IrqHandler[Int085_IRQn])
        {
            IrqHandler[Int085_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.086 IRQ handler
     **
     ******************************************************************************/
    void IRQ086_Handler(void)
    {
        if (NULL != IrqHandler[Int086_IRQn])
        {
            IrqHandler[Int086_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.087 IRQ handler
     **
     ******************************************************************************/
    void IRQ087_Handler(void)
    {
        if (NULL != IrqHandler[Int087_IRQn])
        {
            IrqHandler[Int087_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.088 IRQ handler
     **
     ******************************************************************************/
    void IRQ088_Handler(void)
    {
        if (NULL != IrqHandler[Int088_IRQn])
        {
            IrqHandler[Int088_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.089 IRQ handler
     **
     ******************************************************************************/
    void IRQ089_Handler(void)
    {
        if (NULL != IrqHandler[Int089_IRQn])
        {
            IrqHandler[Int089_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.090 IRQ handler
     **
     ******************************************************************************/
    void IRQ090_Handler(void)
    {
        if (NULL != IrqHandler[Int090_IRQn])
        {
            IrqHandler[Int090_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.091 IRQ handler
     **
     ******************************************************************************/
    void IRQ091_Handler(void)
    {
        if (NULL != IrqHandler[Int091_IRQn])
        {
            IrqHandler[Int091_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.092 IRQ handler
     **
     ******************************************************************************/
    void IRQ092_Handler(void)
    {
        if (NULL != IrqHandler[Int092_IRQn])
        {
            IrqHandler[Int092_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.093 IRQ handler
     **
     ******************************************************************************/
    void IRQ093_Handler(void)
    {
        if (NULL != IrqHandler[Int093_IRQn])
        {
            IrqHandler[Int093_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.094 IRQ handler
     **
     ******************************************************************************/
    void IRQ094_Handler(void)
    {
        if (NULL != IrqHandler[Int094_IRQn])
        {
            IrqHandler[Int094_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.095 IRQ handler
     **
     ******************************************************************************/
    void IRQ095_Handler(void)
    {
        if (NULL != IrqHandler[Int095_IRQn])
        {
            IrqHandler[Int095_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.096 IRQ handler
     **
     ******************************************************************************/
    void IRQ096_Handler(void)
    {
        if (NULL != IrqHandler[Int096_IRQn])
        {
            IrqHandler[Int096_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.097 IRQ handler
     **
     ******************************************************************************/
    void IRQ097_Handler(void)
    {
        if (NULL != IrqHandler[Int097_IRQn])
        {
            IrqHandler[Int097_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.098 IRQ handler
     **
     ******************************************************************************/
    void IRQ098_Handler(void)
    {
        if (NULL != IrqHandler[Int098_IRQn])
        {
            IrqHandler[Int098_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.099 IRQ handler
     **
     ******************************************************************************/
    void IRQ099_Handler(void)
    {
        if (NULL != IrqHandler[Int099_IRQn])
        {
            IrqHandler[Int099_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.100 IRQ handler
     **
     ******************************************************************************/
    void IRQ100_Handler(void)
    {
        if (NULL != IrqHandler[Int100_IRQn])
        {
            IrqHandler[Int100_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.101 IRQ handler
     **
     ******************************************************************************/
    void IRQ101_Handler(void)
    {
        if (NULL != IrqHandler[Int101_IRQn])
        {
            IrqHandler[Int101_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.102 IRQ handler
     **
     ******************************************************************************/
    void IRQ102_Handler(void)
    {
        if (NULL != IrqHandler[Int102_IRQn])
        {
            IrqHandler[Int102_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.103 IRQ handler
     **
     ******************************************************************************/
    void IRQ103_Handler(void)
    {
        if (NULL != IrqHandler[Int103_IRQn])
        {
            IrqHandler[Int103_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.104 IRQ handler
     **
     ******************************************************************************/
    void IRQ104_Handler(void)
    {
        if (NULL != IrqHandler[Int104_IRQn])
        {
            IrqHandler[Int104_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.105 IRQ handler
     **
     ******************************************************************************/
    void IRQ105_Handler(void)
    {
        if (NULL != IrqHandler[Int105_IRQn])
        {
            IrqHandler[Int105_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.106 IRQ handler
     **
     ******************************************************************************/
    void IRQ106_Handler(void)
    {
        if (NULL != IrqHandler[Int106_IRQn])
        {
            IrqHandler[Int106_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.107 IRQ handler
     **
     ******************************************************************************/
    void IRQ107_Handler(void)
    {
        if (NULL != IrqHandler[Int107_IRQn])
        {
            IrqHandler[Int107_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.108 IRQ handler
     **
     ******************************************************************************/
    void IRQ108_Handler(void)
    {
        if (NULL != IrqHandler[Int108_IRQn])
        {
            IrqHandler[Int108_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.109 IRQ handler
     **
     ******************************************************************************/
    void IRQ109_Handler(void)
    {
        if (NULL != IrqHandler[Int109_IRQn])
        {
            IrqHandler[Int109_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.110 IRQ handler
     **
     ******************************************************************************/
    void IRQ110_Handler(void)
    {
        if (NULL != IrqHandler[Int110_IRQn])
        {
            IrqHandler[Int110_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.111 IRQ handler
     **
     ******************************************************************************/
    void IRQ111_Handler(void)
    {
        if (NULL != IrqHandler[Int111_IRQn])
        {
            IrqHandler[Int111_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.112 IRQ handler
     **
     ******************************************************************************/
    void IRQ112_Handler(void)
    {
        if (NULL != IrqHandler[Int112_IRQn])
        {
            IrqHandler[Int112_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.113 IRQ handler
     **
     ******************************************************************************/
    void IRQ113_Handler(void)
    {
        if (NULL != IrqHandler[Int113_IRQn])
        {
            IrqHandler[Int113_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.114 IRQ handler
     **
     ******************************************************************************/
    void IRQ114_Handler(void)
    {
        if (NULL != IrqHandler[Int114_IRQn])
        {
            IrqHandler[Int114_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.115 IRQ handler
     **
     ******************************************************************************/
    void IRQ115_Handler(void)
    {
        if (NULL != IrqHandler[Int115_IRQn])
        {
            IrqHandler[Int115_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.116 IRQ handler
     **
     ******************************************************************************/
    void IRQ116_Handler(void)
    {
        if (NULL != IrqHandler[Int116_IRQn])
        {
            IrqHandler[Int116_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.117 IRQ handler
     **
     ******************************************************************************/
    void IRQ117_Handler(void)
    {
        if (NULL != IrqHandler[Int117_IRQn])
        {
            IrqHandler[Int117_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.118 IRQ handler
     **
     ******************************************************************************/
    void IRQ118_Handler(void)
    {
        if (NULL != IrqHandler[Int118_IRQn])
        {
            IrqHandler[Int118_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.119 IRQ handler
     **
     ******************************************************************************/
    void IRQ119_Handler(void)
    {
        if (NULL != IrqHandler[Int119_IRQn])
        {
            IrqHandler[Int119_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.120 IRQ handler
     **
     ******************************************************************************/
    void IRQ120_Handler(void)
    {
        if (NULL != IrqHandler[Int120_IRQn])
        {
            IrqHandler[Int120_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.121 IRQ handler
     **
     ******************************************************************************/
    void IRQ121_Handler(void)
    {
        if (NULL != IrqHandler[Int121_IRQn])
        {
            IrqHandler[Int121_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.122 IRQ handler
     **
     ******************************************************************************/
    void IRQ122_Handler(void)
    {
        if (NULL != IrqHandler[Int122_IRQn])
        {
            IrqHandler[Int122_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.123 IRQ handler
     **
     ******************************************************************************/
    void IRQ123_Handler(void)
    {
        if (NULL != IrqHandler[Int123_IRQn])
        {
            IrqHandler[Int123_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.124 IRQ handler
     **
     ******************************************************************************/
    void IRQ124_Handler(void)
    {
        if (NULL != IrqHandler[Int124_IRQn])
        {
            IrqHandler[Int124_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.125 IRQ handler
     **
     ******************************************************************************/
    void IRQ125_Handler(void)
    {
        if (NULL != IrqHandler[Int125_IRQn])
        {
            IrqHandler[Int125_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.126 IRQ handler
     **
     ******************************************************************************/
    void IRQ126_Handler(void)
    {
        if (NULL != IrqHandler[Int126_IRQn])
        {
            IrqHandler[Int126_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.127 IRQ handler
     **
     ******************************************************************************/
    void IRQ127_Handler(void)
    {
        if (NULL != IrqHandler[Int127_IRQn])
        {
            IrqHandler[Int127_IRQn]();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.128 share IRQ handler
     **
     ******************************************************************************/
    void IRQ128_Handler(void)
    {
        uint32_t VSSEL128 = M4_INTC->VSSEL128;

        /* external interrupt 00 */
        if ((Set == bM4_INTC_EIFR_EIFR0) && (VSSEL128 & BIT_MASK_00))
        {
            Extint00_IrqHandler();
        }
        /* external interrupt 01 */
        if ((Set == bM4_INTC_EIFR_EIFR1) && (VSSEL128 & BIT_MASK_01))
        {
            Extint01_IrqHandler();
        }
        /* external interrupt 02 */
        if ((Set == bM4_INTC_EIFR_EIFR2) && (VSSEL128 & BIT_MASK_02))
        {
            Extint02_IrqHandler();
        }
        /* external interrupt 03 */
        if ((Set == bM4_INTC_EIFR_EIFR3) && (VSSEL128 & BIT_MASK_03))
        {
            Extint03_IrqHandler();
        }
        /* external interrupt 04 */
        if ((Set == bM4_INTC_EIFR_EIFR4) && (VSSEL128 & BIT_MASK_04))
        {
            Extint04_IrqHandler();
        }
        /* external interrupt 05 */
        if ((Set == bM4_INTC_EIFR_EIFR5) && (VSSEL128 & BIT_MASK_05))
        {
            Extint05_IrqHandler();
        }
        /* external interrupt 06 */
        if ((Set == bM4_INTC_EIFR_EIFR6) && (VSSEL128 & BIT_MASK_06))
        {
            Extint06_IrqHandler();
        }
        /* external interrupt 07 */
        if ((Set == bM4_INTC_EIFR_EIFR7) && (VSSEL128 & BIT_MASK_07))
        {
            Extint07_IrqHandler();
        }
        /* external interrupt 08 */
        if ((Set == bM4_INTC_EIFR_EIFR8) && (VSSEL128 & BIT_MASK_08))
        {
            Extint08_IrqHandler();
        }
        /* external interrupt 09 */
        if ((Set == bM4_INTC_EIFR_EIFR9) && (VSSEL128 & BIT_MASK_09))
        {
            Extint09_IrqHandler();
        }
        /* external interrupt 10 */
        if ((Set == bM4_INTC_EIFR_EIFR10) && (VSSEL128 & BIT_MASK_10))
        {
            Extint10_IrqHandler();
        }
        /* external interrupt 11 */
        if ((Set == bM4_INTC_EIFR_EIFR11) && (VSSEL128 & BIT_MASK_11))
        {
            Extint11_IrqHandler();
        }
        /* external interrupt 12 */
        if ((Set == bM4_INTC_EIFR_EIFR12) && (VSSEL128 & BIT_MASK_12))
        {
            Extint12_IrqHandler();
        }
        /* external interrupt 13 */
        if ((Set == bM4_INTC_EIFR_EIFR13) && (VSSEL128 & BIT_MASK_13))
        {
            Extint13_IrqHandler();
        }
        /* external interrupt 14 */
        if ((Set == bM4_INTC_EIFR_EIFR14) && (VSSEL128 & BIT_MASK_14))
        {
            Extint14_IrqHandler();
        }
        /* external interrupt 15 */
        if ((Set == bM4_INTC_EIFR_EIFR15) && (VSSEL128 & BIT_MASK_15))
        {
            Extint15_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.129 share IRQ handler
     **
     ******************************************************************************/
    void IRQ129_Handler(void)
    {
        uint32_t VSSEL129 = M4_INTC->VSSEL129;
        uint32_t u32Tmp1 = 0ul;
        uint32_t u32Tmp2 = 0ul;

        if (Set == bM4_DMA1_CH0CTL_IE)
        {
            /* DMA1 ch.0 Tx completed */
            if (Reset == bM4_DMA1_INTMASK1_MSKTC0)
            {
                if ((Set == bM4_DMA1_INTSTAT1_TC0) && (VSSEL129 & BIT_MASK_00))
                {
                    Dma1Tc0_IrqHandler();
                }
            }
            /* DMA1 ch.0 Block Tx completed */
            if (Reset == bM4_DMA1_INTMASK1_MSKBTC0)
            {
                if ((Set == bM4_DMA1_INTSTAT1_BTC0) && (VSSEL129 & BIT_MASK_08))
                {
                    Dma1Btc0_IrqHandler();
                }
            }
            /* DMA1 ch.0 Transfer/Request Error */
            u32Tmp1 = M4_DMA1->INTSTAT0 & 0x00010001ul;
            u32Tmp2 = (uint32_t)(~(M4_DMA1->INTMASK0) & 0x00010001ul);
            if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & BIT_MASK_16))
            {
                Dma1Err0_IrqHandler();
            }
        }
        if (Set == bM4_DMA1_CH1CTL_IE)
        {
            /* DMA1 ch.1 Tx completed */
            if (Reset == bM4_DMA1_INTMASK1_MSKTC1)
            {
                if ((Set == bM4_DMA1_INTSTAT1_TC1) && (VSSEL129 & BIT_MASK_01))
                {
                    Dma1Tc1_IrqHandler();
                }
            }
            /* DMA1 ch.1 Block Tx completed */
            if (Reset == bM4_DMA1_INTMASK1_MSKBTC1)
            {
                if ((Set == bM4_DMA1_INTSTAT1_BTC1) && (VSSEL129 & BIT_MASK_09))
                {
                    Dma1Btc1_IrqHandler();
                }
            }
            /* DMA1 ch.1 Transfer/Request Error */
            u32Tmp1 = M4_DMA1->INTSTAT0 & 0x00020002ul;
            u32Tmp2 = (uint32_t)(~(M4_DMA1->INTMASK0) & 0x00020002ul);
            if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & BIT_MASK_16))
            {
                Dma1Err1_IrqHandler();
            }
        }
        if (Set == bM4_DMA1_CH2CTL_IE)
        {
            /* DMA1 ch.2 Tx completed */
            if (Reset == bM4_DMA1_INTMASK1_MSKTC2)
            {
                if ((Set == bM4_DMA1_INTSTAT1_TC2) && (VSSEL129 & BIT_MASK_02))
                {
                    Dma1Tc2_IrqHandler();
                }
            }
            /* DMA1 ch.2 Block Tx completed */
            if (Reset == bM4_DMA1_INTMASK1_MSKBTC2)
            {
                if ((Set == bM4_DMA1_INTSTAT1_BTC2) && (VSSEL129 & BIT_MASK_10))
                {
                    Dma1Btc2_IrqHandler();
                }
            }
            /* DMA1 ch.2 Transfer/Request Error */
            u32Tmp1 = M4_DMA1->INTSTAT0 & 0x00040004ul;
            u32Tmp2 = (uint32_t)(~(M4_DMA1->INTMASK0) & 0x00040004ul);
            if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & BIT_MASK_16))
            {
                Dma1Err2_IrqHandler();
            }
        }
        if (Set == bM4_DMA1_CH3CTL_IE)
        {
            /* DMA1 ch.3 Tx completed */
            if (Reset == bM4_DMA1_INTMASK1_MSKTC3)
            {
                if ((Set == bM4_DMA1_INTSTAT1_TC3) && (VSSEL129 & BIT_MASK_03))
                {
                    Dma1Tc3_IrqHandler();
                }
            }
            /* DMA1 ch.3 Block Tx completed */
            if (Reset == bM4_DMA1_INTMASK1_MSKBTC3)
            {
                if ((Set == bM4_DMA1_INTSTAT1_BTC3) && (VSSEL129 & BIT_MASK_11))
                {
                    Dma1Btc3_IrqHandler();
                }
            }
            /* DMA1 ch.3 Transfer/Request Error */
            u32Tmp1 = M4_DMA1->INTSTAT0 & 0x00080008ul;
            u32Tmp2 = (uint32_t)(~(M4_DMA1->INTMASK0) & 0x00080008ul);
            if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & BIT_MASK_16))
            {
                Dma1Err3_IrqHandler();
            }
        }
        if (Set == bM4_DMA2_CH0CTL_IE)
        {
            /* DMA2 ch.0 Tx completed */
            if (Reset == bM4_DMA2_INTMASK1_MSKTC0)
            {
                if ((Set == bM4_DMA2_INTSTAT1_TC0) && (VSSEL129 & BIT_MASK_04))
                {
                    Dma2Tc0_IrqHandler();
                }
            }
            /* DMA2 ch.0 Block Tx completed */
            if (Reset == bM4_DMA2_INTMASK1_MSKBTC0)
            {
                if ((Set == bM4_DMA2_INTSTAT1_BTC0) && (VSSEL129 & BIT_MASK_12))
                {
                    Dma2Btc0_IrqHandler();
                }
            }
            /* DMA2 Ch.0 Transfer/Request Error */
            u32Tmp1 = M4_DMA2->INTSTAT0 & 0x00010001ul;
            u32Tmp2 = (uint32_t)(~(M4_DMA2->INTMASK0) & 0x00010001ul);
            if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & BIT_MASK_17))
            {
                Dma2Err0_IrqHandler();
            }
        }
        if (Set == bM4_DMA2_CH1CTL_IE)
        {
            /* DMA2 ch.1 Tx completed */
            if (Reset == bM4_DMA2_INTMASK1_MSKTC1)
            {
                if ((Set == bM4_DMA2_INTSTAT1_TC1) && (VSSEL129 & BIT_MASK_05))
                {
                    Dma2Tc1_IrqHandler();
                }
            }
            /* DMA2 ch.1 Block Tx completed */
            if (Reset == bM4_DMA2_INTMASK1_MSKBTC1)
            {
                if ((Set == bM4_DMA1_INTSTAT1_BTC1) && (VSSEL129 & BIT_MASK_13))
                {
                    Dma2Btc1_IrqHandler();
                }
            }
            /* DMA2 Ch.1 Transfer/Request Error */
            u32Tmp1 = M4_DMA2->INTSTAT0 & 0x00020002ul;
            u32Tmp2 = (uint32_t)(~(M4_DMA2->INTMASK0) & 0x00020002ul);
            if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & BIT_MASK_17))
            {
                Dma2Err1_IrqHandler();
            }
        }
        if (Set == bM4_DMA2_CH2CTL_IE)
        {
            /* DMA2 ch.2 Tx completed */
            if (Reset == bM4_DMA2_INTMASK1_MSKTC2)
            {
                if ((Set == bM4_DMA2_INTSTAT1_TC2) && (VSSEL129 & BIT_MASK_06))
                {
                    Dma2Tc2_IrqHandler();
                }
            }
            /* DMA2 ch.2 Block Tx completed */
            if (Reset == bM4_DMA2_INTMASK1_MSKBTC2)
            {
                if ((Set == bM4_DMA1_INTSTAT1_BTC2) && (VSSEL129 & BIT_MASK_14))
                {
                    Dma2Btc2_IrqHandler();
                }
            }
            /* DMA2 Ch.2 Transfer/Request Error */
            u32Tmp1 = M4_DMA2->INTSTAT0 & 0x00040004ul;
            u32Tmp2 = (uint32_t)(~(M4_DMA2->INTMASK0) & 0x00040004ul);
            if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & BIT_MASK_17))
            {
                Dma2Err2_IrqHandler();
            }
        }
        if (Set == bM4_DMA2_CH3CTL_IE)
        {
            /* DMA2 ch.3 Tx completed */
            if (Reset == bM4_DMA2_INTMASK1_MSKTC3)
            {
                if ((Set == bM4_DMA2_INTSTAT1_TC3) && (VSSEL129 & BIT_MASK_07))
                {
                    Dma2Tc3_IrqHandler();
                }
            }
            /* DMA2 ch.3 Block Tx completed */
            if (Reset == bM4_DMA2_INTMASK1_MSKBTC3)
            {
                if ((Set == bM4_DMA1_INTSTAT1_BTC3) && (VSSEL129 & BIT_MASK_15))
                {
                    Dma2Btc3_IrqHandler();
                }
            }
            /* DMA2 Ch.3 Transfer/Request Error */
            u32Tmp1 = M4_DMA2->INTSTAT0 & 0x00080008ul;
            u32Tmp2 = (uint32_t)(~(M4_DMA2->INTMASK0) & 0x00080008ul);
            if ((u32Tmp1 & u32Tmp2) && (VSSEL129 & BIT_MASK_17))
            {
                Dma2Err3_IrqHandler();
            }
        }
        /* EFM program/erase Error */
        if (Set == bM4_EFM_FITE_PEERRITE)
        {
            if ((M4_EFM->FSR & 0x0Fu) && (VSSEL129 & BIT_MASK_18))
            {
                EfmPgmEraseErr_IrqHandler();
            }
        }
        /* EFM collision Error */
        if (Set == bM4_EFM_FITE_RDCOLERRITE)
        {
            if ((Set == bM4_EFM_FSR_RDCOLERR) && (VSSEL129 & BIT_MASK_19))
            {
                EfmColErr_IrqHandler();
            }
        }
        /* EFM operate end */
        if (Set == bM4_EFM_FITE_OPTENDITE)
        {
            if ((Set == bM4_EFM_FSR_OPTEND) && (VSSEL129 & BIT_MASK_20))
            {
                EfmOpEnd_IrqHandler();
            }
        }
        /* QSPI interrupt */
        if ((Set == M4_QSPI->SR_f.RAER) && (VSSEL129 & BIT_MASK_22))
        {
            QspiInt_IrqHandler();
        }
        /* DCU ch.1 */
        u32Tmp1 = M4_DCU1->INTSEL;
        u32Tmp2 = M4_DCU1->FLAG;
        if ((u32Tmp1 & u32Tmp2 & 0x7Ful) && (VSSEL129 & BIT_MASK_23))
        {
            Dcu1_IrqHandler();
        }
        /* DCU ch.2 */
        u32Tmp1 = M4_DCU2->INTSEL;
        u32Tmp2 = M4_DCU2->FLAG;
        if ((u32Tmp1 & u32Tmp2 & 0x7Ful) && (VSSEL129 & BIT_MASK_24))
        {
            Dcu2_IrqHandler();
        }
        /* DCU ch.3 */
        u32Tmp1 = M4_DCU3->INTSEL;
        u32Tmp2 = M4_DCU3->FLAG;
        if ((u32Tmp1 & u32Tmp2 & 0x7Ful) && (VSSEL129 & BIT_MASK_25))
        {
            Dcu3_IrqHandler();
        }
        /* DCU ch.4 */
        u32Tmp1 = M4_DCU4->INTSEL;
        u32Tmp2 = M4_DCU4->FLAG;
        if ((u32Tmp1 & u32Tmp2 & 0x7Ful) && (VSSEL129 & BIT_MASK_26))
        {
            Dcu4_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.130 share IRQ handler
     **
     ******************************************************************************/
    void IRQ130_Handler(void)
    {
        uint32_t VSSEL130 = M4_INTC->VSSEL130;
        /* Timer0 Ch. 1 A compare match */
        if (Set == bM4_TMR01_BCONR_INTENA)
        {
            if ((Set == bM4_TMR01_STFLR_CMAF) && (VSSEL130 & BIT_MASK_00))
            {
                Timer01GCMA_IrqHandler();
            }
        }
        /* Timer0 Ch. 1 B compare match */
        if (Set == bM4_TMR01_BCONR_INTENB)
        {
            if ((Set == bM4_TMR01_STFLR_CMBF) && (VSSEL130 & BIT_MASK_01))
            {
                Timer01GCMB_IrqHandler();
            }
        }
        /* Timer0 Ch. 2 A compare match */
        if (Set == bM4_TMR02_BCONR_INTENA)
        {
            if ((Set == bM4_TMR02_STFLR_CMAF) && (VSSEL130 & BIT_MASK_02))
            {
                Timer02GCMA_IrqHandler();
            }
        }
        /* Timer0 Ch. 2 B compare match */
        if (Set == bM4_TMR02_BCONR_INTENB)
        {
            if ((Set == bM4_TMR02_STFLR_CMBF) && (VSSEL130 & BIT_MASK_03))
            {
                Timer02GCMB_IrqHandler();
            }
        }
        /* Main-OSC stop */
        if (Set == bM4_SYSREG_CMU_XTALSTDCR_XTALSTDIE)
        {
            if ((Set == bM4_SYSREG_CMU_XTALSTDSR_XTALSTDF) && (VSSEL130 & BIT_MASK_21))
            {
                MainOscStop_IrqHandler();
            }
        }
        /* Wakeup timer */
        if ((Set == bM4_WKTM_CR_WKOVF) && (VSSEL130 & BIT_MASK_22))
        {
            WakeupTimer_IrqHandler();
        }
        /* SWDT */
        if ((M4_SWDT->SR & (BIT_MASK_16 | BIT_MASK_17)) && (VSSEL130 & BIT_MASK_23))
        {
            Swdt_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.131 share IRQ handler
     **
     ******************************************************************************/
    void IRQ131_Handler(void)
    {
        uint32_t VSSEL131 = M4_INTC->VSSEL131;
        uint32_t u32Tmp1 = 0ul;
        uint32_t u32Tmp2 = 0ul;
        /* Timer6 Ch.1 A compare match */
        if (Set == bM4_TMR61_ICONR_INTENA)
        {
            if ((Set == bM4_TMR61_STFLR_CMAF) && (VSSEL131 & BIT_MASK_00))
            {
                Timer61GCMA_IrqHandler();
            }
        }
        /* Timer6 Ch.1 B compare match */
        if (Set == bM4_TMR61_ICONR_INTENB)
        {
            if ((Set == bM4_TMR61_STFLR_CMBF) && (VSSEL131 & BIT_MASK_01))
            {
                Timer61GCMB_IrqHandler();
            }
        }
        /* Timer6 Ch.1 C compare match */
        if (Set == bM4_TMR61_ICONR_INTENC)
        {
            if ((Set == bM4_TMR61_STFLR_CMCF) && (VSSEL131 & BIT_MASK_02))
            {
                Timer61GCMC_IrqHandler();
            }
        }
        /* Timer6 Ch.1 D compare match */
        if (Set == bM4_TMR61_ICONR_INTEND)
        {
            if ((Set == bM4_TMR61_STFLR_CMDF) && (VSSEL131 & BIT_MASK_03))
            {
                Timer61GCMD_IrqHandler();
            }
        }
        /* Timer6 Ch.1 E compare match */
        if (Set == bM4_TMR61_ICONR_INTENE)
        {
            if ((Set == bM4_TMR61_STFLR_CMEF) && (VSSEL131 & BIT_MASK_04))
            {
                Timer61GCME_IrqHandler();
            }
        }
        /* Timer6 Ch.1 F compare match */
        if (Set == bM4_TMR61_ICONR_INTENF)
        {
            if ((Set == bM4_TMR61_STFLR_CMFF) && (VSSEL131 & BIT_MASK_05))
            {
                Timer61GCMF_IrqHandler();
            }
        }
        /* Timer6 Ch.1 overflow */
        if (Set == bM4_TMR61_ICONR_INTENOVF)
        {
            if ((Set == bM4_TMR61_STFLR_OVFF) && (VSSEL131 & BIT_MASK_06))
            {
                Timer61GOV_IrqHandler();
            }
        }
        /* Timer6 Ch.1 underflow */
        if (Set == bM4_TMR61_ICONR_INTENUDF)
        {
            if ((Set == bM4_TMR61_STFLR_UDFF) && (VSSEL131 & BIT_MASK_07))
            {
                Timer61GUD_IrqHandler();
            }
        }
        /* Timer6 Ch.1 dead time */
        if (Set == bM4_TMR61_ICONR_INTENDTE)
        {
            if (((Set == bM4_TMR61_STFLR_DTEF)) && (VSSEL131 & BIT_MASK_08))
            {
                Timer61GDT_IrqHandler();
            }
        }
        /* Timer6 Ch.1 A up-down compare match */
        u32Tmp1 = (M4_TMR61->ICONR & (BIT_MASK_16 | BIT_MASK_17)) >> 7u;
        u32Tmp2 = M4_TMR61->STFLR & (BIT_MASK_09 | BIT_MASK_10);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_11))
        {
            Timer61SCMA_IrqHandler();
        }
        /* Timer6 Ch.1 B up-down compare match */
        u32Tmp1 = (M4_TMR61->ICONR & (BIT_MASK_18 | BIT_MASK_19)) >> 7u;
        u32Tmp2 = M4_TMR61->STFLR & (BIT_MASK_11 | BIT_MASK_12);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_12))
        {
            Timer61SCMB_IrqHandler();
        }
        /* Timer6 Ch.2 A compare match */
        if (Set == bM4_TMR62_ICONR_INTENA)
        {
            if ((Set == bM4_TMR62_STFLR_CMAF) && (VSSEL131 & BIT_MASK_16))
            {
                Timer62GCMA_IrqHandler();
            }
        }
        /* Timer6 Ch.2 B compare match */
        if (Set == bM4_TMR62_ICONR_INTENB)
        {
            if ((Set == bM4_TMR62_STFLR_CMBF) && (VSSEL131 & BIT_MASK_17))
            {
                Timer62GCMB_IrqHandler();
            }
        }
        /* Timer6 Ch.2 C compare match */
        if (Set == bM4_TMR62_ICONR_INTENC)
        {
            if ((Set == bM4_TMR62_STFLR_CMCF) && (VSSEL131 & BIT_MASK_18))
            {
                Timer62GCMC_IrqHandler();
            }
        }
        /* Timer6 Ch.2 D compare match */
        if (Set == bM4_TMR62_ICONR_INTEND)
        {
            if ((Set == bM4_TMR62_STFLR_CMDF) && (VSSEL131 & BIT_MASK_19))
            {
                Timer62GCMD_IrqHandler();
            }
        }
        /* Timer6 Ch.2 E compare match */
        if (Set == bM4_TMR62_ICONR_INTENE)
        {
            if ((Set == bM4_TMR62_STFLR_CMEF) && (VSSEL131 & BIT_MASK_20))
            {
                Timer62GCME_IrqHandler();
            }
        }
        /* Timer6 Ch.2 F compare match */
        if (Set == bM4_TMR62_ICONR_INTENF)
        {
            if ((Set == bM4_TMR62_STFLR_CMFF) && (VSSEL131 & BIT_MASK_21))
            {
                Timer62GCMF_IrqHandler();
            }
        }
        /* Timer6 Ch.2 overflow */
        if (Set == bM4_TMR62_ICONR_INTENOVF)
        {
            if ((Set == bM4_TMR62_STFLR_OVFF) && (VSSEL131 & BIT_MASK_22))
            {
                Timer62GOV_IrqHandler();
            }
        }
        /* Timer6 Ch.2 underflow */
        if (Set == bM4_TMR62_ICONR_INTENUDF)
        {
            if ((Set == bM4_TMR62_STFLR_UDFF) && (VSSEL131 & BIT_MASK_23))
            {
                Timer62GUD_IrqHandler();
            }
        }
        /* Timer6 Ch.2 dead time */
        if (Set == bM4_TMR62_ICONR_INTENDTE)
        {
            if (((Set == bM4_TMR62_STFLR_DTEF)) && (VSSEL131 & BIT_MASK_24))
            {
                Timer62GDT_IrqHandler();
            }
        }
        /* Timer6 Ch.2 A up-down compare match */
        u32Tmp1 = (M4_TMR62->ICONR & (BIT_MASK_16 | BIT_MASK_17)) >> 7u;
        u32Tmp2 = M4_TMR62->STFLR & (BIT_MASK_09 | BIT_MASK_10);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_27))
        {
            Timer62SCMA_IrqHandler();
        }
        /* Timer6 Ch.2 B up-down compare match */
        u32Tmp1 = (M4_TMR62->ICONR & (BIT_MASK_18 | BIT_MASK_19)) >> 7u;
        u32Tmp2 = M4_TMR62->STFLR & (BIT_MASK_11 | BIT_MASK_12);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL131 & BIT_MASK_28))
        {
            Timer62SCMB_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.132 share IRQ handler
     **
     ******************************************************************************/
    void IRQ132_Handler(void)
    {
        uint32_t VSSEL132 = M4_INTC->VSSEL132;
        uint32_t u32Tmp1 = 0ul;
        uint32_t u32Tmp2 = 0ul;
        /* Timer6 Ch.3 A compare match */
        if (Set == bM4_TMR63_ICONR_INTENA)
        {
            if ((Set == bM4_TMR63_STFLR_CMAF) && (VSSEL132 & BIT_MASK_00))
            {
                Timer63GCMA_IrqHandler();
            }
        }
        /* Timer6 Ch.3 B compare match */
        if (Set == bM4_TMR63_ICONR_INTENB)
        {
            if ((Set == bM4_TMR63_STFLR_CMBF) && (VSSEL132 & BIT_MASK_01))
            {
                Timer63GCMB_IrqHandler();
            }
        }
        /* Timer6 Ch.3 C compare match */
        if (Set == bM4_TMR63_ICONR_INTENC)
        {
            if ((Set == bM4_TMR63_STFLR_CMCF) && (VSSEL132 & BIT_MASK_02))
            {
                Timer63GCMC_IrqHandler();
            }
        }
        /* Timer6 Ch.3 D compare match */
        if (Set == bM4_TMR63_ICONR_INTEND)
        {
            if ((Set == bM4_TMR63_STFLR_CMDF) && (VSSEL132 & BIT_MASK_03))
            {
                Timer63GCMD_IrqHandler();
            }
        }
        /* Timer6 Ch.3 E compare match */
        if (Set == bM4_TMR63_ICONR_INTENE)
        {
            if ((Set == bM4_TMR63_STFLR_CMEF) && (VSSEL132 & BIT_MASK_04))
            {
                Timer63GCME_IrqHandler();
            }
        }
        /* Timer6 Ch.3 F compare match */
        if (Set == bM4_TMR63_ICONR_INTENF)
        {
            if ((Set == bM4_TMR63_STFLR_CMFF) && (VSSEL132 & BIT_MASK_05))
            {
                Timer63GCMF_IrqHandler();
            }
        }
        /* Timer6 Ch.3 overflow */
        if (Set == bM4_TMR63_ICONR_INTENOVF)
        {
            if ((Set == bM4_TMR63_STFLR_OVFF) && (VSSEL132 & BIT_MASK_06))
            {
                Timer63GOV_IrqHandler();
            }
        }
        /* Timer6 Ch.3 underflow */
        if (Set == bM4_TMR63_ICONR_INTENUDF)
        {
            if ((Set == bM4_TMR63_STFLR_UDFF) && (VSSEL132 & BIT_MASK_07))
            {
                Timer63GUD_IrqHandler();
            }
        }
        /* Timer6 Ch.3 dead time */
        if (Set == bM4_TMR63_ICONR_INTENDTE)
        {
            if (((Set == bM4_TMR63_STFLR_DTEF)) && (VSSEL132 & BIT_MASK_08))
            {
                Timer63GDT_IrqHandler();
            }
        }
        /* Timer6 Ch.3 A up-down compare match */
        u32Tmp1 = (M4_TMR63->ICONR & (BIT_MASK_16 | BIT_MASK_17)) >> 7u;
        u32Tmp2 = M4_TMR63->STFLR & (BIT_MASK_09 | BIT_MASK_10);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL132 & BIT_MASK_11))
        {
            Timer63SCMA_IrqHandler();
        }
        /* Timer6 Ch.3 B up-down compare match */
        u32Tmp1 = (M4_TMR63->ICONR & (BIT_MASK_18 | BIT_MASK_19)) >> 7u;
        u32Tmp2 = M4_TMR63->STFLR & (BIT_MASK_11 | BIT_MASK_12);
        if ((u32Tmp1 & u32Tmp2) && (VSSEL132 & BIT_MASK_12))
        {
            Timer63SCMB_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.136 share IRQ handler
     **
     ******************************************************************************/
    void IRQ136_Handler(void)
    {
        uint32_t u32Tmp1 = 0ul;
        uint32_t u32Tmp2 = 0ul;
        uint32_t VSSEL136 = M4_INTC->VSSEL136;

        u32Tmp1 = M4_TMRA1->BCSTR;
        /* TimerA Ch.1 overflow */
        if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (VSSEL136 & BIT_MASK_00))
        {
            TimerA1OV_IrqHandler();
        }
        /* TimerA Ch.1 underflow */
        if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (VSSEL136 & BIT_MASK_01))
        {
            TimerA1UD_IrqHandler();
        }
        u32Tmp1 = M4_TMRA1->ICONR;
        u32Tmp2 = M4_TMRA1->STFLR;
        /* TimerA Ch.1 compare match */
        if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (VSSEL136 & BIT_MASK_02))
        {
            TimerA1CMP_IrqHandler();
        }

        u32Tmp1 = M4_TMRA2->BCSTR;
        /* TimerA Ch.2 overflow */
        if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (VSSEL136 & BIT_MASK_03))
        {
            TimerA2OV_IrqHandler();
        }
        /* TimerA Ch.2 underflow */
        if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (VSSEL136 & BIT_MASK_04))
        {
            TimerA2UD_IrqHandler();
        }
        u32Tmp1 = M4_TMRA2->ICONR;
        u32Tmp2 = M4_TMRA2->STFLR;
        /* TimerA Ch.2 compare match */
        if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (VSSEL136 & BIT_MASK_05))
        {
            TimerA2CMP_IrqHandler();
        }

        u32Tmp1 = M4_TMRA3->BCSTR;
        /* TimerA Ch.3 overflow */
        if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (VSSEL136 & BIT_MASK_06))
        {
            TimerA3OV_IrqHandler();
        }
        /* TimerA Ch.3 underflow */
        if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (VSSEL136 & BIT_MASK_07))
        {
            TimerA3UD_IrqHandler();
        }
        u32Tmp1 = M4_TMRA3->ICONR;
        u32Tmp2 = M4_TMRA3->STFLR;
        /* TimerA Ch.3 compare match */
        if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (VSSEL136 & BIT_MASK_08))
        {
            TimerA3CMP_IrqHandler();
        }

        u32Tmp1 = M4_TMRA4->BCSTR;
        /* TimerA Ch.4 overflow */
        if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (VSSEL136 & BIT_MASK_09))
        {
            TimerA4OV_IrqHandler();
        }
        /* TimerA Ch.4 underflow */
        if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (VSSEL136 & BIT_MASK_10))
        {
            TimerA4UD_IrqHandler();
        }
        u32Tmp1 = M4_TMRA4->ICONR;
        u32Tmp2 = M4_TMRA4->STFLR;
        /* TimerA Ch.4 compare match */
        if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (VSSEL136 & BIT_MASK_11))
        {
            TimerA4CMP_IrqHandler();
        }

        u32Tmp1 = M4_TMRA5->BCSTR;
        /* TimerA Ch.5 overflow */
        if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (VSSEL136 & BIT_MASK_12))
        {
            TimerA5OV_IrqHandler();
        }
        /* TimerA Ch.5 underflow */
        if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (VSSEL136 & BIT_MASK_13))
        {
            TimerA5UD_IrqHandler();
        }
        u32Tmp1 = M4_TMRA5->ICONR;
        u32Tmp2 = M4_TMRA5->STFLR;
        /* TimerA Ch.5 compare match */
        if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (VSSEL136 & BIT_MASK_14))
        {
            TimerA5CMP_IrqHandler();
        }

        u32Tmp1 = M4_TMRA6->BCSTR;
        /* TimerA Ch.6 overflow */
        if ((u32Tmp1 & BIT_MASK_12) && (u32Tmp1 & BIT_MASK_14) && (VSSEL136 & BIT_MASK_16))
        {
            TimerA6OV_IrqHandler();
        }
        /* TimerA Ch.6 underflow */
        if ((u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_15) && (VSSEL136 & BIT_MASK_17))
        {
            TimerA6UD_IrqHandler();
        }
        u32Tmp1 = M4_TMRA6->ICONR;
        u32Tmp2 = M4_TMRA6->STFLR;
        /* TimerA Ch.6 compare match */
        if ((u32Tmp1 & u32Tmp2 & 0xFFul) && (VSSEL136 & BIT_MASK_18))
        {
            TimerA6CMP_IrqHandler();
        }
        /* USBFS global interrupt */
        if (Set == bM4_USBFS_GAHBCFG_GINTMSK)
        {
            u32Tmp1 = M4_USBFS->GINTMSK & 0xF77CFCFBul;
            u32Tmp2 = M4_USBFS->GINTSTS & 0xF77CFCFBul;
            if ((u32Tmp1 & u32Tmp2) && (VSSEL136 & BIT_MASK_19))
            {
                UsbGlobal_IrqHandler();
            }
        }

        u32Tmp1 = M4_USART1->SR;
        u32Tmp2 = M4_USART1->CR1;
        /* USART Ch.1 Receive error */
        if ((u32Tmp2 & BIT_MASK_05) && (u32Tmp1 & (BIT_MASK_00 | BIT_MASK_01 | BIT_MASK_03)) && (VSSEL136 & BIT_MASK_22))
        {
            Usart1RxErr_IrqHandler();
        }
        /* USART Ch.1 Receive completed */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_05) && (VSSEL136 & BIT_MASK_23))
        {
            Usart1RxEnd_IrqHandler();
        }
        /* USART Ch.1 Transmit data empty */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_07) && (VSSEL136 & BIT_MASK_24))
        {
            Usart1TxEmpty_IrqHandler();
        }
        /* USART Ch.1 Transmit completed */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_06) && (VSSEL136 & BIT_MASK_25))
        {
            Usart1TxEnd_IrqHandler();
        }
        /* USART Ch.1 Receive timeout */
        if ((u32Tmp2 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08) && (VSSEL136 & BIT_MASK_26))
        {
            Usart1RxTO_IrqHandler();
        }

        u32Tmp1 = M4_USART2->SR;
        u32Tmp2 = M4_USART2->CR1;
        /* USART Ch.2 Receive error */
        if ((u32Tmp2 & BIT_MASK_05) && (u32Tmp1 & (BIT_MASK_00 | BIT_MASK_01 | BIT_MASK_03)) && (VSSEL136 & BIT_MASK_27))
        {
            Usart2RxErr_IrqHandler();
        }
        /* USART Ch.2 Receive completed */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_05) && (VSSEL136 & BIT_MASK_28))
        {
            Usart2RxEnd_IrqHandler();
        }
        /* USART Ch.2 Transmit data empty */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_07) && (VSSEL136 & BIT_MASK_29))
        {
            Usart2TxEmpty_IrqHandler();
        }
        /* USART Ch.2 Transmit completed */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_06) && (VSSEL136 & BIT_MASK_30))
        {
            Usart2TxEnd_IrqHandler();
        }
        /* USART Ch.2 Receive timeout */
        if ((u32Tmp2 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08) && (VSSEL136 & BIT_MASK_31))
        {
            Usart2RxTO_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.137 share IRQ handler
     **
     ******************************************************************************/
    void IRQ137_Handler(void)
    {
        uint32_t u32Tmp1 = 0ul;
        uint32_t u32Tmp2 = 0ul;
        uint32_t VSSEL137 = M4_INTC->VSSEL137;

        u32Tmp1 = M4_USART3->SR;
        u32Tmp2 = M4_USART3->CR1;
        /* USART Ch.3 Receive error */
        if ((u32Tmp2 & BIT_MASK_05) && (u32Tmp1 & (BIT_MASK_00 | BIT_MASK_01 | BIT_MASK_03)) && (VSSEL137 & BIT_MASK_00))
        {
            Usart3RxErr_IrqHandler();
        }
        /* USART Ch.3 Receive completed */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_05) && (VSSEL137 & BIT_MASK_01))
        {
            Usart3RxEnd_IrqHandler();
        }
        /* USART Ch.3 Transmit data empty */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_07) && (VSSEL137 & BIT_MASK_02))
        {
            Usart3TxEmpty_IrqHandler();
        }
        /* USART Ch.3 Transmit completed */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_06) && (VSSEL137 & BIT_MASK_03))
        {
            Usart3TxEnd_IrqHandler();
        }
        /* USART Ch.3 Receive timeout */
        if ((u32Tmp2 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08) && (VSSEL137 & BIT_MASK_04))
        {
            Usart3RxTO_IrqHandler();
        }

        u32Tmp1 = M4_USART4->SR;
        u32Tmp2 = M4_USART4->CR1;
        /* USART Ch.4 Receive error */
        if ((u32Tmp2 & BIT_MASK_05) && (u32Tmp1 & (BIT_MASK_00 | BIT_MASK_01 | BIT_MASK_03)) && (VSSEL137 & BIT_MASK_05))
        {
            Usart4RxErr_IrqHandler();
        }
        /* USART Ch.4 Receive completed */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_05) && (VSSEL137 & BIT_MASK_06))
        {
            Usart4RxEnd_IrqHandler();
        }
        /* USART Ch.4 Transmit data empty */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_07) && (VSSEL137 & BIT_MASK_07))
        {
            Usart4TxEmpty_IrqHandler();
        }
        /* USART Ch.4 Transmit completed */
        if ((u32Tmp2 & u32Tmp1 & BIT_MASK_06) && (VSSEL137 & BIT_MASK_08))
        {
            Usart4TxEnd_IrqHandler();
        }
        /* USART Ch.4 Receive timeout */
        if ((u32Tmp2 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_08) && (VSSEL137 & BIT_MASK_09))
        {
            Usart4RxTO_IrqHandler();
        }

        u32Tmp1 = M4_SPI1->CR1;
        u32Tmp2 = M4_SPI1->SR;
        /* SPI Ch.1 Receive completed */
        if ((u32Tmp1 & BIT_MASK_10) && (u32Tmp2 & BIT_MASK_07) && (VSSEL137 & BIT_MASK_11))
        {
            Spi1RxEnd_IrqHandler();
        }
        /* SPI Ch.1 Transmit buf empty */
        if ((u32Tmp1 & BIT_MASK_09) && (u32Tmp2 & BIT_MASK_05) && (VSSEL137 & BIT_MASK_12))
        {
            Spi1TxEmpty_IrqHandler();
        }
        /* SPI Ch.1 bus idle */
        if ((u32Tmp1 & BIT_MASK_11) && (!(u32Tmp2 & BIT_MASK_00)) && (VSSEL137 & BIT_MASK_13))
        {
            Spi1Idle_IrqHandler();
        }
        /* SPI Ch.1 parity/overflow/underflow/mode error */
        if ((u32Tmp1 & BIT_MASK_08) &&
            ((u32Tmp2 & (BIT_MASK_00 | BIT_MASK_02 | BIT_MASK_03 | BIT_MASK_04))) &&
            (VSSEL137 & BIT_MASK_14))
        {
            Spi1Err_IrqHandler();
        }

        u32Tmp1 = M4_SPI2->CR1;
        u32Tmp2 = M4_SPI2->SR;
        /* SPI Ch.2 Receive completed */
        if ((u32Tmp1 & BIT_MASK_10) && (u32Tmp2 & BIT_MASK_07) && (VSSEL137 & BIT_MASK_16))
        {
            Spi2RxEnd_IrqHandler();
        }
        /* SPI Ch.2 Transmit buf empty */
        if ((u32Tmp1 & BIT_MASK_09) && (u32Tmp2 & BIT_MASK_05) && (VSSEL137 & BIT_MASK_17))
        {
            Spi2TxEmpty_IrqHandler();
        }
        /* SPI Ch.2 bus idle */
        if ((u32Tmp1 & BIT_MASK_11) && (!(u32Tmp2 & BIT_MASK_00)) && (VSSEL137 & BIT_MASK_18))
        {
            Spi2Idle_IrqHandler();
        }
        /* SPI Ch.2 parity/overflow/underflow/mode error */
        if ((u32Tmp1 & BIT_MASK_08) &&
            ((u32Tmp2 & (BIT_MASK_00 | BIT_MASK_02 | BIT_MASK_03 | BIT_MASK_04))) &&
            (VSSEL137 & BIT_MASK_19))
        {
            Spi2Err_IrqHandler();
        }

        u32Tmp1 = M4_SPI3->CR1;
        u32Tmp2 = M4_SPI3->SR;
        /* SPI Ch.3 Receive completed */
        if ((u32Tmp1 & BIT_MASK_10) && (u32Tmp2 & BIT_MASK_07) && (VSSEL137 & BIT_MASK_21))
        {
            Spi3RxEnd_IrqHandler();
        }
        /* SPI Ch.3 Transmit buf empty */
        if ((u32Tmp1 & BIT_MASK_09) && (u32Tmp2 & BIT_MASK_05) && (VSSEL137 & BIT_MASK_22))
        {
            Spi3TxEmpty_IrqHandler();
        }
        /* SPI Ch.3 bus idle */
        if ((u32Tmp1 & BIT_MASK_11) && (!(u32Tmp2 & BIT_MASK_00)) && (VSSEL137 & BIT_MASK_23))
        {
            Spi3Idle_IrqHandler();
        }
        /* SPI Ch.3 parity/overflow/underflow/mode error */
        if ((u32Tmp1 & BIT_MASK_08) &&
            ((u32Tmp2 & (BIT_MASK_00 | BIT_MASK_02 | BIT_MASK_03 | BIT_MASK_04))) &&
            (VSSEL137 & BIT_MASK_24))
        {
            Spi3Err_IrqHandler();
        }

        u32Tmp1 = M4_SPI4->CR1;
        u32Tmp2 = M4_SPI4->SR;
        /* SPI Ch.4 Receive completed */
        if ((u32Tmp1 & BIT_MASK_10) && (u32Tmp2 & BIT_MASK_07) && (VSSEL137 & BIT_MASK_26))
        {
            Spi4RxEnd_IrqHandler();
        }
        /* SPI Ch.4 Transmit buf empty */
        if ((u32Tmp1 & BIT_MASK_09) && (u32Tmp2 & BIT_MASK_05) && (VSSEL137 & BIT_MASK_27))
        {
            Spi4TxEmpty_IrqHandler();
        }
        /* SPI Ch.4 bus idle */
        if ((u32Tmp1 & BIT_MASK_11) && (!(u32Tmp2 & BIT_MASK_00)) && (VSSEL137 & BIT_MASK_28))
        {
            Spi4Idle_IrqHandler();
        }
        /* SPI Ch.4 parity/overflow/underflow/mode error */
        if ((u32Tmp1 & BIT_MASK_08) &&
            ((u32Tmp2 & (BIT_MASK_00 | BIT_MASK_02 | BIT_MASK_03 | BIT_MASK_04))) &&
            (VSSEL137 & BIT_MASK_29))
        {
            Spi4Err_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.138 share IRQ handler
     **
     ******************************************************************************/
    void IRQ138_Handler(void)
    {
        uint32_t u32Tmp1 = 0u;
        uint32_t VSSEL138 = M4_INTC->VSSEL138;

        u32Tmp1 = M4_TMR41->OCSRU;
        /* Timer4 Ch.1 U phase higher compare match */
        if ((VSSEL138 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
        {
            Timer41GCMUH_IrqHandler();
        }
        /* Timer4 Ch.1 U phase lower compare match */
        if ((VSSEL138 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
        {
            Timer41GCMUL_IrqHandler();
        }

        u32Tmp1 = M4_TMR41->OCSRV;
        /* Timer4 Ch.1 V phase higher compare match */
        if ((VSSEL138 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
        {
            Timer41GCMVH_IrqHandler();
        }
        /* Timer4 Ch.1 V phase lower compare match */
        if ((VSSEL138 & BIT_MASK_03) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
        {
            Timer41GCMVL_IrqHandler();
        }

        u32Tmp1 = M4_TMR41->OCSRW;
        /* Timer4 Ch.1 W phase higher compare match */
        if ((VSSEL138 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
        {
            Timer41GCMWH_IrqHandler();
        }
        /* Timer4 Ch.1 W phase lower compare match */
        if ((VSSEL138 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
        {
            Timer41GCMWL_IrqHandler();
        }

        u32Tmp1 = M4_TMR41->CCSR;
        /* Timer4 Ch.1 overflow */
        if ((VSSEL138 & BIT_MASK_06) && (u32Tmp1 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_09))
        {
            Timer41GOV_IrqHandler();
        }
        /* Timer4 Ch.1 underflow */
        if ((VSSEL138 & BIT_MASK_07) && (u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_14))
        {
            Timer41GUD_IrqHandler();
        }

        u32Tmp1 = M4_TMR41->RCSR;
        /* Timer4 Ch.1 U phase reload */
        if ((VSSEL138 & BIT_MASK_08) && (~(u32Tmp1 & BIT_MASK_00)) && (u32Tmp1 & BIT_MASK_04))
        {
            Timer41ReloadU_IrqHandler();
        }
        /* Timer4 Ch.1 V phase reload */
        if ((VSSEL138 & BIT_MASK_09) && (~(u32Tmp1 & BIT_MASK_01)) && (u32Tmp1 & BIT_MASK_08))
        {
            Timer41ReloadV_IrqHandler();
        }
        /* Timer4 Ch.1 W phase reload */
        if ((VSSEL138 & BIT_MASK_10) && (~(u32Tmp1 & BIT_MASK_02)) && (u32Tmp1 & BIT_MASK_12))
        {
            Timer41ReloadW_IrqHandler();
        }

        u32Tmp1 = M4_TMR42->OCSRU;
        /* Timer4 Ch.2 U phase higher compare match */
        if ((VSSEL138 & BIT_MASK_16) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
        {
            Timer41GCMUH_IrqHandler();
        }
        /* Timer4 Ch.2 U phase lower compare match */
        if ((VSSEL138 & BIT_MASK_17) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
        {
            Timer41GCMUL_IrqHandler();
        }

        u32Tmp1 = M4_TMR42->OCSRV;
        /* Timer4 Ch.2 V phase higher compare match */
        if ((VSSEL138 & BIT_MASK_18) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
        {
            Timer42GCMVH_IrqHandler();
        }
        /* Timer4 Ch.2 V phase lower compare match */
        if ((VSSEL138 & BIT_MASK_19) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
        {
            Timer42GCMVL_IrqHandler();
        }

        u32Tmp1 = M4_TMR42->OCSRW;
        /* Timer4 Ch.2 W phase higher compare match */
        if ((VSSEL138 & BIT_MASK_20) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
        {
            Timer42GCMWH_IrqHandler();
        }
        /* Timer4 Ch.2 W phase lower compare match */
        if ((VSSEL138 & BIT_MASK_21) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
        {
            Timer42GCMWL_IrqHandler();
        }

        u32Tmp1 = M4_TMR42->CCSR;
        /* Timer4 Ch.2 overflow */
        if ((VSSEL138 & BIT_MASK_22) && (u32Tmp1 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_09))
        {
            Timer42GOV_IrqHandler();
        }
        /* Timer4 Ch.2 underflow */
        if ((VSSEL138 & BIT_MASK_23) && (u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_14))
        {
            Timer42GUD_IrqHandler();
        }

        u32Tmp1 = M4_TMR42->RCSR;
        /* Timer4 Ch.2 U phase reload */
        if ((VSSEL138 & BIT_MASK_24) && (~(u32Tmp1 & BIT_MASK_00)) && (u32Tmp1 & BIT_MASK_04))
        {
            Timer42ReloadU_IrqHandler();
        }
        /* Timer4 Ch.2 V phase reload */
        if ((VSSEL138 & BIT_MASK_25) && (~(u32Tmp1 & BIT_MASK_01)) && (u32Tmp1 & BIT_MASK_08))
        {
            Timer42ReloadV_IrqHandler();
        }
        /* Timer4 Ch.2 W phase reload */
        if ((VSSEL138 & BIT_MASK_26) && (~(u32Tmp1 & BIT_MASK_02)) && (u32Tmp1 & BIT_MASK_12))
        {
            Timer42ReloadW_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.139 share IRQ handler
     **
     ******************************************************************************/
    void IRQ139_Handler(void)
    {
        uint32_t u32Tmp1 = 0u;
        uint32_t VSSEL139 = M4_INTC->VSSEL139;

        u32Tmp1 = M4_TMR43->OCSRU;
        /* Timer4 Ch.3 U phase higher compare match */
        if ((VSSEL139 & BIT_MASK_00) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
        {
            Timer43GCMUH_IrqHandler();
        }
        /* Timer4 Ch.3 U phase lower compare match */
        if ((VSSEL139 & BIT_MASK_01) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
        {
            Timer43GCMUL_IrqHandler();
        }

        u32Tmp1 = M4_TMR43->OCSRV;
        /* Timer4 Ch.3 V phase higher compare match */
        if ((VSSEL139 & BIT_MASK_02) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
        {
            Timer43GCMVH_IrqHandler();
        }
        /* Timer4 Ch.3 V phase lower compare match */
        if ((VSSEL139 & BIT_MASK_03) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
        {
            Timer43GCMVL_IrqHandler();
        }

        u32Tmp1 = M4_TMR43->OCSRW;
        /* Timer4 Ch.3 W phase higher compare match */
        if ((VSSEL139 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_04) && (u32Tmp1 & BIT_MASK_06))
        {
            Timer43GCMWH_IrqHandler();
        }
        /* Timer4 Ch.3 W phase lower compare match */
        if ((VSSEL139 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_05) && (u32Tmp1 & BIT_MASK_07))
        {
            Timer43GCMWL_IrqHandler();
        }

        u32Tmp1 = M4_TMR43->CCSR;
        /* Timer4 Ch.3 overflow */
        if ((VSSEL139 & BIT_MASK_06) && (u32Tmp1 & BIT_MASK_08) && (u32Tmp1 & BIT_MASK_09))
        {
            Timer43GOV_IrqHandler();
        }
        /* Timer4 Ch.3 underflow */
        if ((VSSEL139 & BIT_MASK_07) && (u32Tmp1 & BIT_MASK_13) && (u32Tmp1 & BIT_MASK_14))
        {
            Timer43GUD_IrqHandler();
        }

        u32Tmp1 = M4_TMR43->RCSR;
        /* Timer4 Ch.3 U phase reload */
        if ((VSSEL139 & BIT_MASK_08) && (~(u32Tmp1 & BIT_MASK_00)) && (u32Tmp1 & BIT_MASK_04))
        {
            Timer41ReloadU_IrqHandler();
        }
        /* Timer4 Ch.3 V phase reload */
        if ((VSSEL139 & BIT_MASK_09) && (~(u32Tmp1 & BIT_MASK_01)) && (u32Tmp1 & BIT_MASK_08))
        {
            Timer43ReloadV_IrqHandler();
        }
        /* Timer4 Ch.3 W phase reload */
        if ((VSSEL139 & BIT_MASK_10) && (~(u32Tmp1 & BIT_MASK_02)) && (u32Tmp1 & BIT_MASK_12))
        {
            Timer43ReloadW_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.140 share IRQ handler
     **
     ******************************************************************************/
    void IRQ140_Handler(void)
    {
        uint32_t VSSEL140 = M4_INTC->VSSEL140;
        uint32_t u32Tmp1 = 0u;
        uint32_t u32Tmp2 = 0u;
        /* EMB1 */
        u32Tmp1 = M4_EMB1->STAT & 0x0000000Ful;
        u32Tmp2 = M4_EMB1->INTEN & 0x0000000Ful;
        if ((u32Tmp1 & u32Tmp2) && (VSSEL140 & BIT_MASK_06))
        {
            Emb1_IrqHandler();
        }
        /* EMB2 */
        u32Tmp1 = M4_EMB2->STAT & 0x0000000Ful;
        u32Tmp2 = M4_EMB2->INTEN & 0x0000000Ful;
        if ((u32Tmp1 & u32Tmp2) && (VSSEL140 & BIT_MASK_07))
        {
            Emb2_IrqHandler();
        }
        /* EMB3 */
        u32Tmp1 = M4_EMB3->STAT & 0x0000000Ful;
        u32Tmp2 = M4_EMB3->INTEN & 0x0000000Ful;
        if ((u32Tmp1 & u32Tmp2) && (VSSEL140 & BIT_MASK_08))
        {
            Emb3_IrqHandler();
        }
        /* EMB4*/
        u32Tmp1 = M4_EMB4->STAT & 0x0000000Ful;
        u32Tmp2 = M4_EMB4->INTEN & 0x0000000Ful;
        if ((u32Tmp1 & u32Tmp2) && (VSSEL140 & BIT_MASK_09))
        {
            Emb4_IrqHandler();
        }

        /* I2S Ch.1 Transmit */
        if (Set == bM4_I2S1_CTRL_TXIE)
        {
            if ((Set == bM4_I2S1_SR_TXBA) && (VSSEL140 & BIT_MASK_16))
            {
                I2s1Tx_IrqHandler();
            }
        }
        /* I2S Ch.1 Receive */
        if (Set == bM4_I2S1_CTRL_RXIE)
        {
            if ((Set == bM4_I2S1_SR_RXBA) && (VSSEL140 & BIT_MASK_17))
            {
                I2s1Rx_IrqHandler();
            }
        }
        /* I2S Ch.1 Error */
        if (Set == bM4_I2S1_CTRL_EIE)
        {
            if ((M4_I2S1->ER & (BIT_MASK_00 | BIT_MASK_01)) && (VSSEL140 & BIT_MASK_18))
            {
                I2s1Err_IrqHandler();
            }
        }
        /* I2S Ch.2 Transmit */
        if (Set == bM4_I2S2_CTRL_TXIE)
        {
            if ((Set == bM4_I2S2_SR_TXBA) && (VSSEL140 & BIT_MASK_19))
            {
                I2s2Tx_IrqHandler();
            }
        }
        /* I2S Ch.2 Receive */
        if (Set == bM4_I2S2_CTRL_RXIE)
        {
            if ((Set == bM4_I2S2_SR_RXBA) && (VSSEL140 & BIT_MASK_20))
            {
                I2s2Rx_IrqHandler();
            }
        }
        /* I2S Ch.2 Error */
        if (Set == bM4_I2S2_CTRL_EIE)
        {
            if ((M4_I2S2->ER & (BIT_MASK_00 | BIT_MASK_01)) && (VSSEL140 & BIT_MASK_21))
            {
                I2s2Err_IrqHandler();
            }
        }
        /* I2S Ch.3 Transmit */
        if (Set == bM4_I2S3_CTRL_TXIE)
        {
            if ((Set == bM4_I2S3_SR_TXBA) && (VSSEL140 & BIT_MASK_22))
            {
                I2s3Tx_IrqHandler();
            }
        }
        /* I2S Ch.3 Receive */
        if (Set == bM4_I2S3_CTRL_RXIE)
        {
            if ((Set == bM4_I2S3_SR_RXBA) && (VSSEL140 & BIT_MASK_23))
            {
                I2s3Rx_IrqHandler();
            }
        }
        /* I2S Ch.3 Error */
        if (Set == bM4_I2S3_CTRL_EIE)
        {
            if ((M4_I2S3->ER & (BIT_MASK_00 | BIT_MASK_01)) && (VSSEL140 & BIT_MASK_24))
            {
                I2s3Err_IrqHandler();
            }
        }
        /* I2S Ch.4 Transmit */
        if (Set == bM4_I2S4_CTRL_TXIE)
        {
            if ((Set == bM4_I2S4_SR_TXBA) && (VSSEL140 & BIT_MASK_25))
            {
                I2s4Tx_IrqHandler();
            }
        }
        /* I2S Ch.4 Receive */
        if (Set == bM4_I2S4_CTRL_RXIE)
        {
            if ((Set == bM4_I2S4_SR_RXBA) && (VSSEL140 & BIT_MASK_26))
            {
                I2s4Rx_IrqHandler();
            }
        }
        /* I2S Ch.4 Error */
        if (Set == bM4_I2S4_CTRL_EIE)
        {
            if ((M4_I2S4->ER & (BIT_MASK_00 | BIT_MASK_01)) && (VSSEL140 & BIT_MASK_27))
            {
                I2s4Err_IrqHandler();
            }
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.141 share IRQ handler
     **
     ******************************************************************************/
    void IRQ141_Handler(void)
    {
        uint32_t VSSEL141 = M4_INTC->VSSEL141;
        uint32_t u32Tmp1 = 0ul;
        uint32_t u32Tmp2 = 0ul;
        /* I2C Ch.1 Receive completed */
        if (Set == bM4_I2C1_CR2_RFULLIE)
        {
            if ((Set == bM4_I2C1_SR_RFULLF) && (VSSEL141 & BIT_MASK_04))
            {
                I2c1RxEnd_IrqHandler();
            }
        }
        /* I2C Ch.1 Transmit completed */
        if (Set == bM4_I2C1_CR2_TENDIE)
        {
            if ((Set == bM4_I2C1_SR_TENDF) && (VSSEL141 & BIT_MASK_05))
            {
                I2c1TxEnd_IrqHandler();
            }
        }
        /* I2C Ch.1 Transmit data empty */
        if (Set == bM4_I2C1_CR2_TEMPTYIE)
        {
            if ((Set == bM4_I2C1_SR_TEMPTYF) && (VSSEL141 & BIT_MASK_06))
            {
                I2c1TxEmpty_IrqHandler();
            }
        }
        /* I2C Ch.1 Error */
        u32Tmp1 = M4_I2C1->CR2 & 0x00F05217ul;
        u32Tmp2 = M4_I2C1->SR & 0x00F05217ul;
        if ((u32Tmp1 & u32Tmp2) && (VSSEL141 & BIT_MASK_07))
        {
            I2c1Err_IrqHandler();
        }
        /* I2C Ch.2 Receive completed */
        if (Set == bM4_I2C2_CR2_RFULLIE)
        {
            if ((Set == bM4_I2C2_SR_RFULLF) && (VSSEL141 & BIT_MASK_08))
            {
                I2c2RxEnd_IrqHandler();
            }
        }
        /* I2C Ch.2 Transmit completed */
        if (Set == bM4_I2C2_CR2_TENDIE)
        {
            if ((Set == bM4_I2C2_SR_TENDF) && (VSSEL141 & BIT_MASK_09))
            {
                I2c2TxEnd_IrqHandler();
            }
        }
        /* I2C Ch.2 Transmit data empty */
        if (Set == bM4_I2C2_CR2_TEMPTYIE)
        {
            if ((Set == bM4_I2C2_SR_TEMPTYF) && (VSSEL141 & BIT_MASK_10))
            {
                I2c2TxEmpty_IrqHandler();
            }
        }
        /* I2C Ch.2 Error */
        u32Tmp1 = M4_I2C2->CR2 & 0x00F05217ul;
        u32Tmp2 = M4_I2C2->SR & 0x00F05217ul;
        if ((u32Tmp1 & u32Tmp2) && (VSSEL141 & BIT_MASK_11))
        {
            I2c2Err_IrqHandler();
        }
        /* I2C Ch.3 Receive completed */
        if (Set == bM4_I2C3_CR2_RFULLIE)
        {
            if ((Set == bM4_I2C3_SR_RFULLF) && (VSSEL141 & BIT_MASK_12))
            {
                I2c3RxEnd_IrqHandler();
            }
        }
        /* I2C Ch.3 Transmit completed */
        if (Set == bM4_I2C3_CR2_TENDIE)
        {
            if ((Set == bM4_I2C3_SR_TENDF) && (VSSEL141 & BIT_MASK_13))
            {
                I2c3TxEnd_IrqHandler();
            }
        }
        /* I2C Ch.3 Transmit data empty */
        if (Set == bM4_I2C3_CR2_TEMPTYIE)
        {
            if ((Set == bM4_I2C3_SR_TEMPTYF) && (VSSEL141 & BIT_MASK_14))
            {
                I2c3TxEmpty_IrqHandler();
            }
        }
        /* I2C Ch.3 Error */
        u32Tmp1 = M4_I2C3->CR2 & 0x00F05217ul;
        u32Tmp2 = M4_I2C3->SR & 0x00F05217ul;
        if ((u32Tmp1 & u32Tmp2) && (VSSEL141 & BIT_MASK_15))
        {
            I2c3Err_IrqHandler();
        }
        /* LVD Ch.1 detected */
        if (Set == bM4_SYSREG_PWR_PVDCR1_PVD1IRE)
        {
            if ((Set == bM4_SYSREG_PWR_PVDDSR_PVD1DETFLG) && (VSSEL141 & BIT_MASK_17))
            {
                Lvd1_IrqHandler();
            }
        }
        if (Set == bM4_SYSREG_PWR_PVDCR1_PVD2IRE)
        {
            /* LVD Ch.2 detected */
            if ((Set == bM4_SYSREG_PWR_PVDDSR_PVD2DETFLG) && (VSSEL141 & BIT_MASK_18))
            {
                Lvd2_IrqHandler();
            }
        }
        /* Freq. calculate error detected */
        if (Set == bM4_FCM_RIER_ERRIE)
        {
            if ((Set == bM4_FCM_SR_ERRF) && (VSSEL141 & BIT_MASK_20))
            {
                FcmErr_IrqHandler();
            }
        }
        /* Freq. calculate completed */
        if (Set == bM4_FCM_RIER_MENDIE)
        {
            if ((Set == bM4_FCM_SR_MENDF) && (VSSEL141 & BIT_MASK_21))
            {
                FcmEnd_IrqHandler();
            }
        }
        /* Freq. calculate overflow */
        if (Set == bM4_FCM_RIER_OVFIE)
        {
            if ((Set == bM4_FCM_SR_OVF) && (VSSEL141 & BIT_MASK_22))
            {
                FcmOV_IrqHandler();
            }
        }

        /* WDT */
        if ((M4_WDT->SR & (BIT_MASK_16 | BIT_MASK_17)) && (VSSEL141 & BIT_MASK_23))
        {
            Wdt_IrqHandler();
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.142 share IRQ handler
     **
     ******************************************************************************/
    void IRQ142_Handler(void)
    {
        uint32_t u32VSSEL142 = M4_INTC->VSSEL142;
        uint16_t u16Tmp = 0u;
        /* ADC unit.1 seq. A */
        if (Set == bM4_ADC1_ICR_EOCAIEN)
        {
            if ((Set == bM4_ADC1_ISR_EOCAF) && (u32VSSEL142 & BIT_MASK_00))
            {
                ADC1A_IrqHandler();
            }
        }
        /* ADC unit.1 seq. B */
        if (Set == bM4_ADC1_ICR_EOCBIEN)
        {
            if ((Set == bM4_ADC1_ISR_EOCBF) && (u32VSSEL142 & BIT_MASK_01))
            {
                ADC1B_IrqHandler();
            }
        }
        /* ADC unit.1 seq. A */
        u16Tmp = M4_ADC1->AWDSR0;
        if (Set == bM4_ADC1_AWDCR_AWDIEN)
        {
            if (((Set == bM4_ADC1_AWDSR1_AWDF16) || (u16Tmp)) && (u32VSSEL142 & BIT_MASK_02))
            {
                ADC1ChCmp_IrqHandler();
            }
        }
        /* ADC unit.1 seq. cmp */
        if (Set == bM4_ADC1_AWDCR_AWDIEN)
        {
            if (((Set == bM4_ADC1_AWDSR1_AWDF16) || (u16Tmp)) && (u32VSSEL142 & BIT_MASK_03))
            {
                ADC1SeqCmp_IrqHandler();
            }
        }

        /* ADC unit.2 seq. A */
        if (Set == bM4_ADC2_ICR_EOCAIEN)
        {
            if ((Set == bM4_ADC2_ISR_EOCAF) && (u32VSSEL142 & BIT_MASK_04))
            {
                ADC2A_IrqHandler();
            }
        }
        /* ADC unit.2 seq. B */
        if (Set == bM4_ADC2_ICR_EOCBIEN)
        {
            if ((Set == bM4_ADC2_ISR_EOCBF) && (u32VSSEL142 & BIT_MASK_05))
            {
                ADC2B_IrqHandler();
            }
        }
        /* ADC unit.2 seq. A */
        if (Set == bM4_ADC2_AWDCR_AWDIEN)
        {
            if ((M4_ADC2->AWDSR0 & 0x1FFu) && (u32VSSEL142 & BIT_MASK_06))
            {
                ADC2ChCmp_IrqHandler();
            }
        }
        /* ADC unit.2 seq. cmp */
        if (Set == bM4_ADC2_AWDCR_AWDIEN)
        {
            if ((M4_ADC2->AWDSR0 & 0x1FFu) && (u32VSSEL142 & BIT_MASK_07))
            {
                ADC2SeqCmp_IrqHandler();
            }
        }
    }

    /**
     *******************************************************************************
     ** \brief Int No.143 share IRQ handler
     **
     ******************************************************************************/
    void IRQ143_Handler(void)
    {
        uint8_t RTIF = 0u;
        uint8_t RTIE = 0u;
        uint8_t ERRINT = 0u;
        uint8_t TTCFG = 0u;
        uint16_t NORINTST = 0u;
        uint16_t NORINTSGEN = 0u;
        uint16_t ERRINTST = 0u;
        uint16_t ERRINTSGEN = 0u;

        /* SDIO Ch.1 */
        if (Set == bM4_INTC_VSSEL143_VSEL2)
        {
            NORINTST = M4_SDIOC1->NORINTST;
            NORINTSGEN = M4_SDIOC1->NORINTSGEN;
            ERRINTST = M4_SDIOC1->ERRINTST;
            ERRINTSGEN = M4_SDIOC1->ERRINTSGEN;

            if ((NORINTST & NORINTSGEN & 0x1F7u) || (ERRINTST & ERRINTSGEN & 0x017Fu))
            {
                Sdio1_IrqHandler();
            }
        }

        /* SDIO Ch.2 */
        if (Set == bM4_INTC_VSSEL143_VSEL5)
        {
            NORINTST = M4_SDIOC2->NORINTST;
            NORINTSGEN = M4_SDIOC2->NORINTSGEN;
            ERRINTST = M4_SDIOC2->ERRINTST;
            ERRINTSGEN = M4_SDIOC2->ERRINTSGEN;

            if ((NORINTST & NORINTSGEN & 0x1F7u) || (ERRINTST & ERRINTSGEN & 0x017Fu))
            {
                Sdio2_IrqHandler();
            }
        }

        /* CAN */
        if (Set == bM4_INTC_VSSEL143_VSEL6)
        {
            RTIF = M4_CAN->RTIF;
            RTIE = M4_CAN->RTIE;
            ERRINT = M4_CAN->ERRINT;
            TTCFG = M4_CAN->TTCFG;
            if ((TTCFG & BIT_MASK_05) ||
                (RTIF & BIT_MASK_00) ||
                (RTIF & RTIE & 0xFEu) ||
                ((ERRINT & BIT_MASK_00) && (ERRINT & BIT_MASK_01)) ||
                ((ERRINT & BIT_MASK_02) && (ERRINT & BIT_MASK_03)) ||
                ((ERRINT & BIT_MASK_04) && (ERRINT & BIT_MASK_05)) ||
                ((TTCFG & BIT_MASK_03) && (TTCFG & BIT_MASK_04)) ||
                ((TTCFG & BIT_MASK_06) && (TTCFG & BIT_MASK_07)))
            {
                Can_IrqHandler();
            }
        }
    }
//@} // InterruptGroup

#endif /* DDL_INTERRUPTS_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
