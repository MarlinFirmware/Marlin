/*!
    \file    drv_usb_core.c
    \brief   USB core driver which can operate in host and device mode

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "drv_usb_core.h"
#include "drv_usb_hw.h"

/* local function prototypes ('static') */
static void usb_core_reset (usb_core_regs *usb_regs);

/*!
    \brief      configure USB core basic 
    \param[in]  usb_basic: pointer to USB capabilities
    \param[in]  usb_regs: USB core registers
    \param[in]  usb_core: USB core
    \param[out] none
    \retval     operation status
*/
usb_status usb_basic_init (usb_core_basic *usb_basic, 
                           usb_core_regs  *usb_regs, 
                           usb_core_enum   usb_core)
{
    /* configure USB default transfer mode as FIFO mode */
    usb_basic->transfer_mode = (uint8_t)USB_USE_FIFO;

    /* USB default speed is full-speed */
    usb_basic->core_speed = (uint8_t)USB_SPEED_FULL;

    usb_basic->core_enum = (uint8_t)usb_core;

    switch (usb_core) {
    case USB_CORE_ENUM_HS:
        usb_basic->base_reg = (uint32_t)USBHS_REG_BASE;

        /* set the host channel numbers */
        usb_basic->num_pipe = USBHS_MAX_CHANNEL_COUNT;

        /* set the device endpoint numbers */
        usb_basic->num_ep = USBHS_MAX_EP_COUNT;

#ifdef USB_ULPI_PHY_ENABLED
        usb_basic->phy_itf = USB_ULPI_PHY;
#else
        usb_basic->phy_itf = USB_EMBEDDED_PHY;
#endif /* USB_ULPI_PHY_ENABLED */

#ifdef USB_HS_INTERNAL_DMA_ENABLED
        usb_basic->transfer_mode = USB_USE_DMA;
#endif /* USB_HS_INTERNAL_DMA_ENABLED */
        break;
    
    case USB_CORE_ENUM_FS:
        usb_basic->base_reg = (uint32_t)USBFS_REG_BASE;

        /* set the host channel numbers */
        usb_basic->num_pipe = USBFS_MAX_CHANNEL_COUNT;

        /* set the device endpoint numbers */
        usb_basic->num_ep = USBFS_MAX_EP_COUNT;

        /* USBFS core use embedded physical layer */
        usb_basic->phy_itf = USB_EMBEDDED_PHY;
        break;

    default:
        return USB_FAIL;
    }

    usb_basic->sof_enable = USB_SOF_OUTPUT;
    usb_basic->low_power = USB_LOW_POWER;

    /* assign main registers address */
    *usb_regs = (usb_core_regs) {
        .gr          = (usb_gr*) (usb_basic->base_reg + USB_REG_OFFSET_CORE),
        .hr          = (usb_hr*) (usb_basic->base_reg + USB_REG_OFFSET_HOST),
        .dr          = (usb_dr*) (usb_basic->base_reg + USB_REG_OFFSET_DEV),

        .HPCS        = (uint32_t*) (usb_basic->base_reg + USB_REG_OFFSET_PORT),
        .PWRCLKCTL   = (uint32_t*) (usb_basic->base_reg + USB_REG_OFFSET_PWRCLKCTL)
    };

    /* assign device endpoint registers address */
    for (uint8_t i = 0U; i < usb_basic->num_ep; i++) {
        usb_regs->er_in[i] = (usb_erin *) \
            (usb_basic->base_reg + USB_REG_OFFSET_EP_IN + (i * USB_REG_OFFSET_EP));

        usb_regs->er_out[i] = (usb_erout *)\
            (usb_basic->base_reg + USB_REG_OFFSET_EP_OUT + (i * USB_REG_OFFSET_EP));
    }

    /* assign host pipe registers address */
    for (uint8_t i = 0U; i < usb_basic->num_pipe; i++) {
        usb_regs->pr[i] = (usb_pr *) \
            (usb_basic->base_reg + USB_REG_OFFSET_CH_INOUT + (i * USB_REG_OFFSET_CH));

        usb_regs->DFIFO[i] = (uint32_t *) \
            (usb_basic->base_reg + USB_DATA_FIFO_OFFSET + (i * USB_DATA_FIFO_SIZE));
    }

    return USB_OK;
}

/*!
    \brief      initializes the USB controller registers and 
                prepares the core device mode or host mode operation
    \param[in]  usb_basic: pointer to USB capabilities
    \param[in]  usb_regs: pointer to USB core registers
    \param[out] none
    \retval     operation status
*/
usb_status usb_core_init (usb_core_basic usb_basic, usb_core_regs *usb_regs)
{
    if (USB_ULPI_PHY == usb_basic.phy_itf) {
        usb_regs->gr->GCCFG &= ~GCCFG_PWRON;

        if (usb_basic.sof_enable) {
            usb_regs->gr->GCCFG |= GCCFG_SOFOEN;
        }

        /* initialize the ULPI interface */
        usb_regs->gr->GUSBCS &= ~(GUSBCS_EMBPHY | GUSBCS_ULPIEOI);

#ifdef USBHS_EXTERNAL_VBUS_ENABLED
        /* use external VBUS driver */
        usb_regs->gr->GUSBCS |= GUSBCS_ULPIEVD;
#else
        /* use internal VBUS driver */
        usb_regs->gr->GUSBCS &= ~GUSBCS_ULPIEVD;
#endif /* USBHS_EXTERNAL_VBUS_ENABLED */

        /* soft reset the core */
        usb_core_reset (usb_regs);
    } else {
        usb_regs->gr->GUSBCS |= GUSBCS_EMBPHY;

        /* soft reset the core */
        usb_core_reset (usb_regs);

        /* active the transceiver and enable VBUS sensing */
        usb_regs->gr->GCCFG |= GCCFG_PWRON | GCCFG_VBUSACEN | GCCFG_VBUSBCEN;

#ifndef VBUS_SENSING_ENABLED
        usb_regs->gr->GCCFG |= GCCFG_VBUSIG;
#endif /* VBUS_SENSING_ENABLED */

        /* enable SOF output */
        if (usb_basic.sof_enable) {
            usb_regs->gr->GCCFG |= GCCFG_SOFOEN;
        }

        usb_mdelay(20U);
    }

    if ((uint8_t)USB_USE_DMA == usb_basic.transfer_mode) {
        usb_regs->gr->GAHBCS &= ~GAHBCS_BURST;
        usb_regs->gr->GAHBCS |= DMA_INCR8 | GAHBCS_DMAEN;
    }

#ifdef USE_OTG_MODE

    /* enable USB OTG features */
    usb_regs->gr->GUSBCS |= GUSBCS_HNPCEN | GUSBCS_SRPCEN;

    /* enable the USB wakeup and suspend interrupts */
    usb_regs->gr->GINTF = 0xBFFFFFFFU;

    usb_regs->gr->GINTEN = GINTEN_WKUPIE | GINTEN_SPIE | \
                                     GINTEN_OTGIE | GINTEN_SESIE | GINTEN_CIDPSCIE;

#endif /* USE_OTG_MODE */

    return USB_OK;
}

/*!
    \brief      write a packet into the TX FIFO associated with the endpoint
    \param[in]  usb_regs: pointer to USB core registers
    \param[in]  src_buf: pointer to source buffer
    \param[in]  fifo_num: FIFO number which is in (0..3)
    \param[in]  byte_count: packet byte count
    \param[out] none
    \retval     operation status
*/
usb_status usb_txfifo_write (usb_core_regs *usb_regs, 
                             uint8_t *src_buf, 
                             uint8_t  fifo_num, 
                             uint16_t byte_count)
{
    uint32_t word_count = (byte_count + 3U) / 4U;

    __IO uint32_t *fifo = usb_regs->DFIFO[fifo_num];

    while (word_count-- > 0U) {
        *fifo = *((__packed uint32_t *)src_buf);

        src_buf += 4U;
    }

    return USB_OK;
}

/*!
    \brief      read a packet from the Rx FIFO associated with the endpoint
    \param[in]  usb_regs: pointer to USB core registers
    \param[in]  dest_buf: pointer to destination buffer
    \param[in]  byte_count: packet byte count
    \param[out] none
    \retval     void type pointer
*/
void *usb_rxfifo_read (usb_core_regs *usb_regs, uint8_t *dest_buf, uint16_t byte_count)
{
    uint32_t word_count = (byte_count + 3U) / 4U;

    __IO uint32_t *fifo = usb_regs->DFIFO[0];

    while (word_count-- > 0U) {
        *(__packed uint32_t *)dest_buf = *fifo;

        dest_buf += 4U;
    }

    return ((void *)dest_buf);
}

/*!
    \brief      flush a TX FIFO or all TX FIFOs
    \param[in]  usb_regs: pointer to USB core registers
    \param[in]  fifo_num: FIFO number which is in (0..3)
    \param[out] none
    \retval     operation status
*/
usb_status usb_txfifo_flush (usb_core_regs *usb_regs, uint8_t fifo_num)
{
    usb_regs->gr->GRSTCTL = ((uint32_t)fifo_num << 6U) | GRSTCTL_TXFF;

    /* wait for TX FIFO flush bit is set */
    while (usb_regs->gr->GRSTCTL & GRSTCTL_TXFF) {
        /* no operation */
    }

    /* wait for 3 PHY clocks*/
    usb_udelay(3U);

    return USB_OK;
}

/*!
    \brief      flush the entire Rx FIFO
    \param[in]  usb_regs: pointer to USB core registers
    \param[out] none
    \retval     operation status
*/
usb_status usb_rxfifo_flush (usb_core_regs *usb_regs)
{
    usb_regs->gr->GRSTCTL = GRSTCTL_RXFF;

    /* wait for RX FIFO flush bit is set */
    while (usb_regs->gr->GRSTCTL & GRSTCTL_RXFF) {
        /* no operation */
    }

    /* wait for 3 PHY clocks */
    usb_udelay(3U);

    return USB_OK;
}

/*!
    \brief      set endpoint or channel TX FIFO size
    \param[in]  usb_regs: pointer to USB core registers
    \param[in]  fifo: TX FIFO number
    \param[in]  size: assigned TX FIFO size
    \param[out] none
    \retval     none
*/
void usb_set_txfifo(usb_core_regs *usb_regs, uint8_t fifo, uint16_t size)
{
    uint32_t tx_offset;

    tx_offset = usb_regs->gr->GRFLEN;

    if (fifo == 0U) {
        usb_regs->gr->DIEP0TFLEN_HNPTFLEN = ((uint32_t)size << 16) | tx_offset;
    } else {
        tx_offset += (usb_regs->gr->DIEP0TFLEN_HNPTFLEN) >> 16;

        for (uint8_t i = 0U; i < (fifo - 1U); i++) {
            tx_offset += (usb_regs->gr->DIEPTFLEN[i] >> 16);
        }

        /* multiply Tx_Size by 2 to get higher performance */
        usb_regs->gr->DIEPTFLEN[fifo - 1U] = ((uint32_t)size << 16) | tx_offset;
    }
}

/*!
    \brief      set USB current mode
    \param[in]  usb_regs: pointer to USB core registers
    \param[out] none
    \retval     none
*/
void usb_curmode_set(usb_core_regs *usb_regs, uint8_t mode)
{
    usb_regs->gr->GUSBCS &= ~(GUSBCS_FDM | GUSBCS_FHM);

    if (DEVICE_MODE == mode) {
        usb_regs->gr->GUSBCS |= GUSBCS_FDM;
    } else if (HOST_MODE == mode) {
        usb_regs->gr->GUSBCS |= GUSBCS_FHM;
    } else {
        /* OTG mode and other mode can not be here! */
    }
}

/*!
    \brief      configure USB core to soft reset
    \param[in]  usb_regs: pointer to USB core registers
    \param[out] none
    \retval     none
*/
static void usb_core_reset (usb_core_regs *usb_regs)
{
    /* enable core soft reset */
    usb_regs->gr->GRSTCTL |= GRSTCTL_CSRST;

    /* wait for the core to be soft reset */
    while (usb_regs->gr->GRSTCTL & GRSTCTL_CSRST) {
        /* no operation */
    }

    /* wait for additional 3 PHY clocks */
    usb_udelay(3U);
}
