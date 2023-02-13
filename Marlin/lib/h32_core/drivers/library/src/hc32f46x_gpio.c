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
/** \file hc32f46x_gpio.c
 **
 ** A detailed description is available at
 ** @link GpioGroup Gpio description @endlink
 **
 **   - 2018-10-12  1.0  Zhangxl First version for Device Driver Library of Gpio.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_gpio.h"
#include "hc32f46x_utility.h"

#if (DDL_GPIO_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup GpioGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define GPIO_BASE (0x40053800ul)
#define PODR_BASE (0x0004ul)
#define POER_BASE (0x0006ul)
#define POSR_BASE (0x0008ul)
#define PORR_BASE (0x000Aul)
#define PCR_BASE (0x0400ul)
#define PFSR_BASE (0x0402ul)

/*! Parameter validity check for port group. */
#define IS_VALID_PORT(x) \
    (((x) == PortA) ||   \
     ((x) == PortB) ||   \
     ((x) == PortC) ||   \
     ((x) == PortD) ||   \
     ((x) == PortE) ||   \
     ((x) == PortH))

/*! Parameter validity check for pin. */
#define IS_VALID_PIN(x) \
    (((x) == Pin00) ||  \
     ((x) == Pin01) ||  \
     ((x) == Pin02) ||  \
     ((x) == Pin03) ||  \
     ((x) == Pin04) ||  \
     ((x) == Pin05) ||  \
     ((x) == Pin06) ||  \
     ((x) == Pin07) ||  \
     ((x) == Pin08) ||  \
     ((x) == Pin09) ||  \
     ((x) == Pin10) ||  \
     ((x) == Pin11) ||  \
     ((x) == Pin12) ||  \
     ((x) == Pin13) ||  \
     ((x) == Pin14) ||  \
     ((x) == Pin15))

/*! Parameter validity check for debug pins. */
#define IS_VALID_DEBUGPIN(x) ((x) <= 0x1Fu)

/*! Parameter validity check for pin mode. */
#define IS_VALID_PINMODE(x)   \
    (((x) == Pin_Mode_In) ||  \
     ((x) == Pin_Mode_Out) || \
     ((x) == Pin_Mode_Ana))

/*! Parameter validity check for pin drive capacity. */
#define IS_VALID_PINDRV(x) \
    (((x) == Pin_Drv_L) || \
     ((x) == Pin_Drv_M) || \
     ((x) == Pin_Drv_H))

/*! Parameter validity check for pin output type. */
#define IS_VALID_PINTYPE(x)     \
    (((x) == Pin_OType_Cmos) || \
     ((x) == Pin_OType_Od))

/*! Parameter validity check for pin read wait cycle. */
#define IS_VALID_READWAIT(x) \
    (((x) == WaitCycle0) ||  \
     ((x) == WaitCycle1) ||  \
     ((x) == WaitCycle2) ||  \
     ((x) == WaitCycle3))

/*! Parameter validity check for pin function */
#define IS_VALID_FUNC(x)         \
    (((x) == Func_Gpio) ||       \
     ((x) == Func_Fcmref) ||     \
     ((x) == Func_Rtcout) ||     \
     ((x) == Func_Vcout) ||      \
     ((x) == Func_Adtrg) ||      \
     ((x) == Func_Mclkout) ||    \
     ((x) == Func_Tim4) ||       \
     ((x) == Func_Tim6) ||       \
     ((x) == Func_Tima0) ||      \
     ((x) == Func_Tima1) ||      \
     ((x) == Func_Tima2) ||      \
     ((x) == Func_Emb) ||        \
     ((x) == Func_Usart_Ck) ||   \
     ((x) == Func_Spi_Nss) ||    \
     ((x) == Func_Qspi) ||       \
     ((x) == Func_Key) ||        \
     ((x) == Func_Sdio) ||       \
     ((x) == Func_I2s) ||        \
     ((x) == Func_UsbF) ||       \
     ((x) == Func_Evnpt) ||      \
     ((x) == Func_Eventout) ||   \
     ((x) == Func_Usart1_Tx) ||  \
     ((x) == Func_Usart3_Tx) ||  \
     ((x) == Func_Usart1_Rx) ||  \
     ((x) == Func_Usart3_Rx) ||  \
     ((x) == Func_Usart1_Rts) || \
     ((x) == Func_Usart3_Rts) || \
     ((x) == Func_Usart1_Cts) || \
     ((x) == Func_Usart3_Cts) || \
     ((x) == Func_Usart2_Tx) ||  \
     ((x) == Func_Usart4_Tx) ||  \
     ((x) == Func_Usart2_Rx) ||  \
     ((x) == Func_Usart4_Rx) ||  \
     ((x) == Func_Usart2_Rts) || \
     ((x) == Func_Usart4_Rts) || \
     ((x) == Func_Usart2_Cts) || \
     ((x) == Func_Usart4_Cts) || \
     ((x) == Func_Spi1_Mosi) ||  \
     ((x) == Func_Spi3_Mosi) ||  \
     ((x) == Func_Spi1_Miso) ||  \
     ((x) == Func_Spi3_Miso) ||  \
     ((x) == Func_Spi1_Nss0) ||  \
     ((x) == Func_Spi3_Nss0) ||  \
     ((x) == Func_Spi1_Sck) ||   \
     ((x) == Func_Spi3_Sck) ||   \
     ((x) == Func_Spi2_Mosi) ||  \
     ((x) == Func_Spi4_Mosi) ||  \
     ((x) == Func_Spi2_Miso) ||  \
     ((x) == Func_Spi4_Miso) ||  \
     ((x) == Func_Spi2_Nss0) ||  \
     ((x) == Func_Spi4_Nss0) ||  \
     ((x) == Func_Spi2_Sck) ||   \
     ((x) == Func_Spi4_Sck) ||   \
     ((x) == Func_I2c1_Sda) ||   \
     ((x) == Func_I2c3_Sda) ||   \
     ((x) == Func_I2c1_Scl) ||   \
     ((x) == Func_I2c3_Scl) ||   \
     ((x) == Func_I2c2_Sda) ||   \
     ((x) == Func_Can1_Tx) ||    \
     ((x) == Func_I2c2_Scl) ||   \
     ((x) == Func_Can1_Rx) ||    \
     ((x) == Func_I2s1_Sd) ||    \
     ((x) == Func_I2s3_Sd) ||    \
     ((x) == Func_I2s1_Sdin) ||  \
     ((x) == Func_I2s3_Sdin) ||  \
     ((x) == Func_I2s1_Ws) ||    \
     ((x) == Func_I2s3_Ws) ||    \
     ((x) == Func_I2s1_Ck) ||    \
     ((x) == Func_I2s3_Ck) ||    \
     ((x) == Func_I2s2_Sd) ||    \
     ((x) == Func_I2s4_Sd) ||    \
     ((x) == Func_I2s2_Sdin) ||  \
     ((x) == Func_I2s4_Sdin) ||  \
     ((x) == Func_I2s2_Ws) ||    \
     ((x) == Func_I2s4_Ws) ||    \
     ((x) == Func_I2s2_Ck) ||    \
     ((x) == Func_I2s4_Ck))

/*! Parameter validity check for pin sub-function */
#define IS_VALID_SUBFUNC(x)    \
    (((x) == Func_Gpio) ||     \
     ((x) == Func_Fcmref) ||   \
     ((x) == Func_Rtcout) ||   \
     ((x) == Func_Vcout) ||    \
     ((x) == Func_Adtrg) ||    \
     ((x) == Func_Mclkout) ||  \
     ((x) == Func_Tim4) ||     \
     ((x) == Func_Tim6) ||     \
     ((x) == Func_Tima0) ||    \
     ((x) == Func_Tima1) ||    \
     ((x) == Func_Tima2) ||    \
     ((x) == Func_Emb) ||      \
     ((x) == Func_Usart_Ck) || \
     ((x) == Func_Spi_Nss) ||  \
     ((x) == Func_Qspi) ||     \
     ((x) == Func_Key) ||      \
     ((x) == Func_Sdio) ||     \
     ((x) == Func_I2s) ||      \
     ((x) == Func_UsbF) ||     \
     ((x) == Func_Evnpt) ||    \
     ((x) == Func_Eventout))

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
 ** \brief   Port init
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  u16Pin               GPIO pin index, This parameter can be
 **                                     any composed value of @ref en_pin_t
 ** \param   [in]  pstcPortInit         Structure pointer of port configuration
 **
 ** \retval  Ok                         Port initial successful
 **
 ******************************************************************************/
en_result_t PORT_Init(en_port_t enPort, uint16_t u16Pin, const stc_port_init_t *pstcPortInit)
{
    stc_port_pcr_field_t *PCRx;
    stc_port_pfsr_field_t *PFSRx;
    uint8_t u8PinPos = 0u;

    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));

    DDL_ASSERT(IS_VALID_PINMODE(pstcPortInit->enPinMode));
    DDL_ASSERT(IS_VALID_PINDRV(pstcPortInit->enPinDrv));
    DDL_ASSERT(IS_VALID_PINTYPE(pstcPortInit->enPinOType));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcPortInit->enLatch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcPortInit->enExInt));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcPortInit->enInvert));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcPortInit->enPullUp));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcPortInit->enPinSubFunc));

    PORT_Unlock();
    for (u8PinPos = 0u; u8PinPos < 16u; u8PinPos++)
    {
        if (u16Pin & (1ul << u8PinPos))
        {
            PCRx = (stc_port_pcr_field_t *)((uint32_t)(&M4_PORT->PCRA0) +
                                            enPort * 0x40ul + u8PinPos * 0x04ul);
            PFSRx = (stc_port_pfsr_field_t *)((uint32_t)(&M4_PORT->PFSRA0) +
                                              enPort * 0x40ul + u8PinPos * 0x04ul);

            /* Input latch function setting */
            PCRx->LTE = pstcPortInit->enLatch;

            /* External interrupt input enable setting */
            PCRx->INTE = pstcPortInit->enExInt;

            /* In_Out invert setting */
            PCRx->INVE = pstcPortInit->enInvert;

            /* Pin pull-up setting */
            PCRx->PUU = pstcPortInit->enPullUp;

            /* CMOS/OD output setting */
            PCRx->NOD = pstcPortInit->enPinOType;

            /* Pin drive mode setting */
            PCRx->DRV = pstcPortInit->enPinDrv;

            /* Pin mode setting */
            switch (pstcPortInit->enPinMode)
            {
            case Pin_Mode_In:
                PCRx->DDIS = 0u;
                PCRx->POUTE = 0u;
                break;
            case Pin_Mode_Out:
                PCRx->DDIS = 0u;
                PCRx->POUTE = 1u;
                break;
            case Pin_Mode_Ana:
                PCRx->DDIS = 1u;
                break;
            default:
                break;
            }
            /* Sub function enable setting */
            PFSRx->BFE = pstcPortInit->enPinSubFunc;
        }
    }
    PORT_Lock();
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Port de-init
 **
 ** \param   None
 **
 ** \retval  Ok                         GPIO de-initial successful
 **
 ******************************************************************************/
en_result_t PORT_DeInit(void)
{
    uint8_t u8PortIdx, u8PinIdx;
    PORT_Unlock();

    for (u8PortIdx = (uint8_t)PortA; u8PortIdx <= (uint8_t)PortH; u8PortIdx++)
    {
        *(uint16_t *)(GPIO_BASE + PODR_BASE + u8PortIdx * 0x10ul) = 0u;
        *(uint16_t *)(GPIO_BASE + POER_BASE + u8PortIdx * 0x10ul) = 0u;
        *(uint16_t *)(GPIO_BASE + POSR_BASE + u8PortIdx * 0x10ul) = 0u;
        *(uint16_t *)(GPIO_BASE + PORR_BASE + u8PortIdx * 0x10ul) = 0u;
        for (u8PinIdx = 0u; u8PinIdx < 16u; u8PinIdx++)
        {
            if (((uint8_t)PortH == u8PortIdx) && (3u == u8PinIdx))
            {
                break;
            }
            *(uint16_t *)(GPIO_BASE + PCR_BASE + u8PortIdx * 0x40ul + u8PinIdx * 0x4ul) = 0u;
            *(uint16_t *)(GPIO_BASE + PFSR_BASE + u8PortIdx * 0x40ul + u8PinIdx * 0x4ul) = 0u;
        }
    }
    M4_PORT->PCCR = 0u;
    M4_PORT->PINAER = 0u;
    M4_PORT->PSPCR = 0x1Fu;

    PORT_Lock();
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Special control register Setting
 **
 ** \param   [in]  u8DebugPort          Debug port setting register, This parameter
 **                                     can be any composed value of @ref en_debug_port_t
 **
 ** \param   [in] enFunc                The new state of the debug ports.
 ** \arg     Enable                     Enable.
 ** \arg     Disable                    Disable.
 **
 ** \retval  Ok                         Debug port set successful
 **
 ******************************************************************************/
en_result_t PORT_DebugPortSetting(uint8_t u8DebugPort, en_functional_state_t enFunc)
{
    /* parameter check */
    DDL_ASSERT(IS_VALID_DEBUGPIN(u8DebugPort));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enFunc));

    PORT_Unlock();

    if (Enable == enFunc)
    {
        M4_PORT->PSPCR |= (uint16_t)(u8DebugPort & 0x1Ful);
    }
    else
    {
        M4_PORT->PSPCR &= (uint16_t)(~(u8DebugPort & 0x1Ful));
    }

    PORT_Lock();
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Port Public Setting
 **
 ** \param   [in]  pstcPortPubSet       Structure pointer of public setting (PCCR)
 **
 ** \retval  Ok                         Port public register set successful
 **
 ******************************************************************************/
en_result_t PORT_PubSetting(const stc_port_pub_set_t *pstcPortPubSet)
{
    DDL_ASSERT(IS_VALID_FUNC(pstcPortPubSet->enSubFuncSel));
    DDL_ASSERT(IS_VALID_READWAIT(pstcPortPubSet->enReadWait));
    PORT_Unlock();

    /* PCCR setting */
    /* Sub function setting */
    M4_PORT->PCCR_f.BFSEL = pstcPortPubSet->enSubFuncSel;

    /* PIDRx, PCRxy read wait cycle setting */
    M4_PORT->PCCR_f.RDWT = pstcPortPubSet->enReadWait;

    PORT_Lock();
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   PSPCR, PCCR, PINAER, PCRxy, PFSRxy write enable
 **
 ** \param   None
 **
 ** \retval  None
 **
 ******************************************************************************/
void PORT_Unlock(void)
{
    M4_PORT->PWPR = 0xA501u;
}

/**
 *******************************************************************************
 ** \brief  SPCR, PCCR, PINAER, PCRxy, PFSRxy write disable
 **
 ** \param  None
 **
 ** \retval None
 **
 ******************************************************************************/
void PORT_Lock(void)
{
    M4_PORT->PWPR = 0xA500u;
}

/**
 *******************************************************************************
 ** \brief   Read Port value
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 **
 ** \retval  uint16_t                   The output port value
 **
 ******************************************************************************/
uint16_t PORT_GetData(en_port_t enPort)
{
    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));

    uint32_t *PIDRx;
    PIDRx = (uint32_t *)((uint32_t)(&M4_PORT->PIDRA) + 0x10u * enPort);
    return (uint16_t)(*PIDRx);
}

/**
 *******************************************************************************
 ** \brief   Read Pin value
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  enPin                GPIO pin index, This parameter can be
 **                                     any value of @ref en_pin_t
 ** \retval  en_flag_status_t           The output port pin value
 **
 ******************************************************************************/
en_flag_status_t PORT_GetBit(en_port_t enPort, uint16_t enPin)
{
    uint32_t *PIDRx;

    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));
    DDL_ASSERT(IS_VALID_PIN(enPin));

    PIDRx = (uint32_t *)((uint32_t)(&M4_PORT->PIDRA) + 0x10u * enPort);
    return (en_flag_status_t)((bool)(!!(*PIDRx & (enPin))));
}

/**
 *******************************************************************************
 ** \brief   Set Port value
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  u16Pin               GPIO pin index, This parameter can be
 **                                     any composed value of @ref en_pin_t
 **
 ** \retval  Ok                         Data be set to corresponding port
 **
 ******************************************************************************/
en_result_t PORT_SetPortData(en_port_t enPort, uint16_t u16Pin)
{
    uint16_t *PODRx;

    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));

    PODRx = (uint16_t *)((uint32_t)(&M4_PORT->PODRA) + 0x10u * enPort);
    *PODRx |= u16Pin;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Set Port value
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  u16Pin               GPIO pin index, This parameter can be
 **                                     any composed value of @ref en_pin_t
 **
 ** \retval  Ok                         Data be reset to corresponding port
 **
 ******************************************************************************/
en_result_t PORT_ResetPortData(en_port_t enPort, uint16_t u16Pin)
{
    uint16_t *PODRx;

    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));

    PODRx = (uint16_t *)((uint32_t)(&M4_PORT->PODRA) + 0x10u * enPort);
    *PODRx &= (uint16_t)(~u16Pin);
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Port Pin Output enable
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  u16Pin               GPIO pin index, This parameter can be
 **                                     any composed value of @ref en_pin_t
 ** \param   [in]  enNewState           The new state of pin direction setting
 ** \retval  Ok                         Set successful to corresponding port/pin
 **
 ******************************************************************************/
en_result_t PORT_OE(en_port_t enPort, uint16_t u16Pin, en_functional_state_t enNewState)
{
    uint16_t *POERx;

    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));

    POERx = (uint16_t *)((uint32_t)(&M4_PORT->POERA) + 0x10ul * enPort);
    if (Enable == enNewState)
    {
        *POERx |= u16Pin;
    }
    else
    {
        *POERx &= (uint16_t)(~u16Pin);
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Set Port Pin
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  u16Pin               GPIO pin index, This parameter can be
 **                                     any composed value of @ref en_pin_t
 ** \retval  Ok                         Set successful to corresponding pins
 **
 ******************************************************************************/
en_result_t PORT_SetBits(en_port_t enPort, uint16_t u16Pin)
{
    uint16_t *POSRx;

    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));

    POSRx = (uint16_t *)((uint32_t)(&M4_PORT->POSRA) + 0x10u * enPort);
    *POSRx |= u16Pin;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Reset Port Pin
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  u16Pin               GPIO pin index, This parameter can be
 **                                     any composed value of @ref en_pin_t
 ** \retval  Ok                         Set successful to corresponding pins
 **
 ******************************************************************************/
en_result_t PORT_ResetBits(en_port_t enPort, uint16_t u16Pin)
{
    uint16_t *PORRx;

    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));

    PORRx = (uint16_t *)((uint32_t)(&M4_PORT->PORRA) + 0x10u * enPort);
    *PORRx |= u16Pin;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Toggle Port Pin
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  u16Pin               GPIO pin index, This parameter can be
 **                                     any composed value of @ref en_pin_t
 ** \retval  Ok                         Set successful to corresponding pins
 **
 ******************************************************************************/
en_result_t PORT_Toggle(en_port_t enPort, uint16_t u16Pin)
{
    uint16_t *POTRx;

    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));

    POTRx = (uint16_t *)((uint32_t)(&M4_PORT->POTRA) + 0x10u * enPort);
    *POTRx |= u16Pin;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Set port always ON
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in] enNewState            The new state of the port always ON function.
 ** \arg     Enable                     Enable.
 ** \arg     Disable                    Disable.
 **
 ** \retval  Ok                         Set successful to corresponding pins
 **
 ******************************************************************************/
en_result_t PORT_AlwaysOn(en_port_t enPort, en_functional_state_t enNewState)
{
    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    PORT_Unlock();

    if (Enable == enNewState)
    {
        M4_PORT->PINAER |= Enable << (uint8_t)enPort;
    }
    else
    {
        M4_PORT->PINAER &= (uint16_t)(~(((1ul << (uint8_t)enPort)) & 0x1Ful));
    }

    PORT_Lock();
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Set Port Pin function
 **
 ** \param   [in]  enPort               GPIO port index, This parameter can be
 **                                     any value of @ref en_port_t
 ** \param   [in]  u16Pin               GPIO pin index, This parameter can be
 **                                     any value of @ref en_pin_t
 ** \param   [in]  enFuncSel            Function selection, This parameter can be
 **                                     any value of @ref en_port_func_t
 **
 ** \param   [in] enSubFunc             The new state of the gpio sub-function.
 ** \arg     Enable                     Enable.
 ** \arg     Disable                    Disable.
 **
 ** \retval  Ok                         Set successful to corresponding pins
 **
 ******************************************************************************/
en_result_t PORT_SetFunc(en_port_t enPort, uint16_t u16Pin, en_port_func_t enFuncSel,
                         en_functional_state_t enSubFunc)
{
    stc_port_pfsr_field_t *PFSRx;
    uint8_t u8PinPos = 0u;

    /* parameter check */
    DDL_ASSERT(IS_VALID_PORT(enPort));
    DDL_ASSERT(IS_VALID_FUNC(enFuncSel));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enSubFunc));

    PORT_Unlock();

    for (u8PinPos = 0u; u8PinPos < 16u; u8PinPos++)
    {
        if (u16Pin & (uint16_t)(1ul << u8PinPos))
        {
            PFSRx = (stc_port_pfsr_field_t *)((uint32_t)(&M4_PORT->PFSRA0) + 0x40ul * enPort + 0x4ul * u8PinPos);

            /* main function setting */
            PFSRx->FSEL = enFuncSel;

            /* sub function enable setting */
            PFSRx->BFE = (Enable == enSubFunc ? Enable : Disable);
        }
    }

    PORT_Lock();
    return Ok;
}

/**
 *******************************************************************************
 ** \brief   Set global sub-function
 **
 ** \param   [in]  enFuncSel            Function selection, This parameter can be
 **                                     some values of @ref en_port_func_t, cannot
 **                                     large than 15u
 **
 ** \retval  Ok                         Set successful to corresponding pins
 **
 ******************************************************************************/
en_result_t PORT_SetSubFunc(en_port_func_t enFuncSel)
{
    /* parameter check */
    DDL_ASSERT(IS_VALID_SUBFUNC(enFuncSel));

    PORT_Unlock();

    M4_PORT->PCCR_f.BFSEL = enFuncSel;

    PORT_Lock();
    return Ok;
}

//@} // GpioGroup

#endif /* DDL_GPIO_ENABLE */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
