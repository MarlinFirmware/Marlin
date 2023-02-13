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
/******************************************************************************/
/** \file hc32f46x_event_port.c
 **
 ** A detailed description is available at
 ** @link EventPortGroup EventPort description @endlink
 **
 **   - 2018-12-07  1.0  Zhangxl First version for Device Driver Library of
 **                      EventPort.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_event_port.h"
#include "hc32f46x_utility.h"

#if (DDL_EVENT_PORT_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup EventPortGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define EP1_BASE 0x40010800ul + 0x0100ul
#define EP2_BASE 0x40010800ul + 0x011Cul
#define EP3_BASE 0x40010800ul + 0x0138ul
#define EP4_BASE 0x40010800ul + 0x0154ul
#define EP1_DIR_BASE 0x00ul
#define EP1_IDR_BASE 0x04ul
#define EP1_ODR_BASE 0x08ul
#define EP1_ORR_BASE 0x0Cul
#define EP1_OSR_BASE 0x10ul
#define EP1_RISR_BASE 0x14ul
#define EP1_FAL_BASE 0x18ul
#define EP_NFCR_BASE 0x40010800ul + 0x0170ul

/*! Parameter validity check for port group. */
#define IS_VALID_EVENT_PORT(x) \
    (((x) == EventPort1) ||    \
     ((x) == EventPort2) ||    \
     ((x) == EventPort3) ||    \
     ((x) == EventPort4))

/*! Parameter validity check for pin. */
#define IS_VALID_EVENT_PIN(x) \
    (((x) == EventPin00) ||   \
     ((x) == EventPin01) ||   \
     ((x) == EventPin02) ||   \
     ((x) == EventPin03) ||   \
     ((x) == EventPin04) ||   \
     ((x) == EventPin05) ||   \
     ((x) == EventPin06) ||   \
     ((x) == EventPin07) ||   \
     ((x) == EventPin08) ||   \
     ((x) == EventPin09) ||   \
     ((x) == EventPin10) ||   \
     ((x) == EventPin11) ||   \
     ((x) == EventPin12) ||   \
     ((x) == EventPin13) ||   \
     ((x) == EventPin14) ||   \
     ((x) == EventPin15))

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
 ** \brief   Event Port init
 **
 ** \param   [in]  enEventPort          Event port index, This parameter can be
 **                                     any value of @ref en_event_port_t
 ** \param   [in]  u16EventPin          Event pin index, This parameter can be
 **                                     any composed value of @ref en_event_pin_t
 ** \param   [in]  pstcEventPortInit    Structure pointer of event port configuration
 **
 ** \retval  Ok                         Init successful
 **          ErrorInvalidParameter      Event port index invalid
 **
 ******************************************************************************/
en_result_t EVENTPORT_Init(en_event_port_t enEventPort, uint16_t u16EventPin,
                           const stc_event_port_init_t *pstcEventPortInit)
{
    en_result_t enRet = Ok;

    uint32_t *EPDIRx;  ///< Direction register
    uint32_t *EPORRx;  ///< Reset after trigger enable register
    uint32_t *EPOSRx;  ///< Set after trigger enable register
    uint32_t *EPRISRx; ///< Rising edge detect enable register
    uint32_t *EPFALx;  ///< Falling edge detect enable register

    EPDIRx = (uint32_t *)(EP1_BASE + EP1_DIR_BASE + 0x1C * (enEventPort - 1));
    EPORRx = (uint32_t *)(EP1_BASE + EP1_ORR_BASE + 0x1C * (enEventPort - 1));
    EPOSRx = (uint32_t *)(EP1_BASE + EP1_OSR_BASE + 0x1C * (enEventPort - 1));
    EPRISRx = (uint32_t *)(EP1_BASE + EP1_RISR_BASE + 0x1C * (enEventPort - 1));
    EPFALx = (uint32_t *)(EP1_BASE + EP1_FAL_BASE + 0x1C * (enEventPort - 1));

    /* Direction configure */
    if (EventPortOut == pstcEventPortInit->enDirection)
    {
        *EPDIRx |= u16EventPin;
    }
    else
    {
        *EPDIRx &= (~(uint32_t)u16EventPin) & 0xFFFFul;
    }

    /* Reset if be triggered */
    if (Enable == pstcEventPortInit->enReset)
    {
        *EPORRx |= u16EventPin;
    }
    else
    {
        *EPORRx &= (~(uint32_t)u16EventPin) & 0xFFFFul;
    }

    /* Set if be triggered */
    if (Enable == pstcEventPortInit->enSet)
    {
        *EPOSRx |= u16EventPin;
    }
    else
    {
        *EPOSRx &= (~(uint32_t)u16EventPin) & 0xFFFFul;
    }

    /* Rising edge detect setting */
    if (Enable == pstcEventPortInit->enRisingDetect)
    {
        *EPRISRx |= u16EventPin;
    }
    else
    {
        *EPRISRx &= (~(uint32_t)u16EventPin) & 0xFFFFul;
    }

    /* Falling edge detect setting */
    if (Enable == pstcEventPortInit->enFallingDetect)
    {
        *EPFALx |= u16EventPin;
    }
    else
    {
        *EPFALx &= (~(uint32_t)u16EventPin) & 0xFFFFul;
    }

    /* Noise filter setting */
    switch (enEventPort)
    {
    case EventPort1:
        M4_AOS->PEVNTNFCR_f.NFEN1 = pstcEventPortInit->enFilter;
        M4_AOS->PEVNTNFCR_f.DIVS1 = pstcEventPortInit->enFilterClk;
        break;
    case EventPort2:
        M4_AOS->PEVNTNFCR_f.NFEN2 = pstcEventPortInit->enFilter;
        M4_AOS->PEVNTNFCR_f.DIVS2 = pstcEventPortInit->enFilterClk;
        break;
    case EventPort3:
        M4_AOS->PEVNTNFCR_f.NFEN3 = pstcEventPortInit->enFilter;
        M4_AOS->PEVNTNFCR_f.DIVS3 = pstcEventPortInit->enFilterClk;
        break;
    case EventPort4:
        M4_AOS->PEVNTNFCR_f.NFEN4 = pstcEventPortInit->enFilter;
        M4_AOS->PEVNTNFCR_f.DIVS4 = pstcEventPortInit->enFilterClk;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief   Event Port de-init, restore all registers to default value
 **
 ** \param   None
 **
 ** \retval  Ok                         De-init successful
 **
 ******************************************************************************/
en_result_t EVENTPORT_DeInit(void)
{
    uint32_t EPDIRx;
    uint32_t EPODRx;
    uint32_t EPORRx;
    uint32_t EPOSRx;
    uint32_t EPRISRx;
    uint32_t EPFALx;
    uint8_t u8EPCnt;

    EPDIRx = (uint32_t)(EP1_BASE + EP1_DIR_BASE);
    EPODRx = (uint32_t)(EP1_BASE + EP1_ODR_BASE);
    EPORRx = (uint32_t)(EP1_BASE + EP1_ORR_BASE);
    EPOSRx = (uint32_t)(EP1_BASE + EP1_OSR_BASE);
    EPRISRx = (uint32_t)(EP1_BASE + EP1_RISR_BASE);
    EPFALx = (uint32_t)(EP1_BASE + EP1_FAL_BASE);

    /* Restore all registers to default value */
    M4_AOS->PORT_PEVNTTRGSR12 = 0x1FFul;
    M4_AOS->PORT_PEVNTTRGSR34 = 0x1FFul;
    M4_AOS->PEVNTNFCR = 0ul;
    for (u8EPCnt = 0u; u8EPCnt < 4u; u8EPCnt++)
    {
        *(uint32_t *)(EPDIRx + 0x1Cul * u8EPCnt) = 0ul;
        *(uint32_t *)(EPODRx + 0x1Cul * u8EPCnt) = 0ul;
        *(uint32_t *)(EPORRx + 0x1Cul * u8EPCnt) = 0ul;
        *(uint32_t *)(EPOSRx + 0x1Cul * u8EPCnt) = 0ul;
        *(uint32_t *)(EPRISRx + 0x1Cul * u8EPCnt) = 0ul;
        *(uint32_t *)(EPFALx + 0x1Cul * u8EPCnt) = 0ul;
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Event Port trigger source select
 **
 ** \param   [in]  enEventPort          Event port index, This parameter can be
 **                                     any value of @ref en_event_port_t
 ** \param   [in]  enTriggerSrc         Event port trigger source. This parameter
 **                                     can be any value of @ref en_event_src_t
 ** \retval  Ok                         Trigger source is set
 **          ErrorInvalidParameter      Invalid event port enum
 **
 ******************************************************************************/
en_result_t EVENTPORT_SetTriggerSrc(en_event_port_t enEventPort,
                                    en_event_src_t enTriggerSrc)
{
    en_result_t enRet = Ok;
    DDL_ASSERT(IS_VALID_EVENT_PORT(enEventPort));

    if ((EventPort1 == enEventPort) || (EventPort2 == enEventPort))
    {
        M4_AOS->PORT_PEVNTTRGSR12 = enTriggerSrc;
    }
    else if ((EventPort3 == enEventPort) || (EventPort4 == enEventPort))
    {
        M4_AOS->PORT_PEVNTTRGSR34 = enTriggerSrc;
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief   Read Event Port value after be triggered
 **
 ** \param   [in]  enEventPort          Event port index, This parameter can be
 **                                     any value of @ref en_event_port_t
 **
 ** \retval  uint16_t                   The output port value
 **
 ******************************************************************************/
uint16_t EVENTPORT_GetData(en_event_port_t enEventPort)
{
    uint16_t u16Data = 0u;
    DDL_ASSERT(IS_VALID_EVENT_PORT(enEventPort));
    switch (enEventPort)
    {
    case EventPort1:
        u16Data = (uint16_t)(M4_AOS->PEVNTIDR1 & 0xFFFFul);
        break;
    case EventPort2:
        u16Data = (uint16_t)(M4_AOS->PEVNTIDR2 & 0xFFFFul);
        break;
    case EventPort3:
        u16Data = (uint16_t)(M4_AOS->PEVNTIDR3 & 0xFFFFul);
        break;
    case EventPort4:
        u16Data = (uint16_t)(M4_AOS->PEVNTIDR4 & 0xFFFFul);
        break;
    }
    return u16Data;
}

/**
 *******************************************************************************
 ** \brief   Read Event Pin value after triggered
 **
 ** \param   [in]  enEventPort          Event port index, This parameter can be
 **                                     any value of @ref en_event_port_t
 ** \param   [in]  enEventPin           GPIO pin index, This parameter can be
 **                                     any value of @ref en_event_pin_t
 ** \retval  en_flag_status_t           The output port pin value
 **
 ******************************************************************************/
en_flag_status_t EVENTPORT_GetBit(en_event_port_t enEventPort, en_event_pin_t enEventPin)
{
    bool bBitValue = false;

    switch (enEventPort)
    {
    case EventPort1:
        bBitValue = M4_AOS->PEVNTIDR1 & enEventPin;
        break;
    case EventPort2:
        bBitValue = M4_AOS->PEVNTIDR2 & enEventPin;
        break;
    case EventPort3:
        bBitValue = M4_AOS->PEVNTIDR3 & enEventPin;
        break;
    case EventPort4:
        bBitValue = M4_AOS->PEVNTIDR4 & enEventPin;
        break;
    }
    return (en_flag_status_t)(bool)((!!bBitValue));
}

/**
 *******************************************************************************
 ** \brief   Set Event Port Pin
 **
 ** \param   [in]  enEventPort          Event port index, This parameter can be
 **                                     any value of @ref en_event_port_t
 ** \param   [in]  u16EventPin          Event pin index, This parameter can be
 **                                     any composed value of @ref en_event_pin_t
 ** \retval  Ok                         Set successful
 **          ErrorInvalidParameter      Event port index invalid
 **
 ******************************************************************************/
en_result_t EVENTPORT_SetBits(en_event_port_t enEventPort, en_event_pin_t u16EventPin)
{
    en_result_t enRet = Ok;
    DDL_ASSERT(IS_VALID_EVENT_PORT(enEventPort));

    switch (enEventPort)
    {
    case EventPort1:
        M4_AOS->PEVNTODR1 |= u16EventPin;
        break;
    case EventPort2:
        M4_AOS->PEVNTODR2 |= u16EventPin;
        break;
    case EventPort3:
        M4_AOS->PEVNTODR3 |= u16EventPin;
        break;
    case EventPort4:
        M4_AOS->PEVNTODR4 |= u16EventPin;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief   Reset Event Port Pin
 **
 ** \param   [in]  enEventPort          Event port index, This parameter can be
 **                                     any value of @ref en_event_port_t
 ** \param   [in]  u16EventPin          Event pin index, This parameter can be
 **                                     any composed value of @ref en_event_pin_t
 ** \retval  Ok                         Reset successful
 **          ErrorInvalidParameter      Event port index invalid
 **
 ******************************************************************************/
en_result_t EVENTPORT_ResetBits(en_event_port_t enEventPort, en_event_pin_t u16EventPin)
{
    en_result_t enRet = Ok;
    DDL_ASSERT(IS_VALID_EVENT_PORT(enEventPort));

    switch (enEventPort)
    {
    case EventPort1:
        M4_AOS->PEVNTODR1 &= (~(uint32_t)u16EventPin) & 0xFFFFul;
        break;
    case EventPort2:
        M4_AOS->PEVNTODR2 &= (~(uint32_t)u16EventPin) & 0xFFFFul;
        break;
    case EventPort3:
        M4_AOS->PEVNTODR3 &= (~(uint32_t)u16EventPin) & 0xFFFFul;
        break;
    case EventPort4:
        M4_AOS->PEVNTODR4 &= (~(uint32_t)u16EventPin) & 0xFFFFul;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }
    return enRet;
}

//@} // EventPortGroup

#endif /* DDL_EVENT_PORT_ENABLE */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
