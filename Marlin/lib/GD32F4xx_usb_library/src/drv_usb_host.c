/*!
    \file    drv_usb_host.c
    \brief   USB host mode low level driver

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

#include "drv_usb_hw.h"
#include "drv_usb_core.h"
#include "drv_usb_host.h"

const uint32_t PIPE_DPID[2] = {
    PIPE_DPID_DATA0,
    PIPE_DPID_DATA1
};

/*!
    \brief      initializes USB core for host mode
    \param[in]  udev: pointer to selected USB host
    \param[out] none
    \retval     operation status
*/
usb_status usb_host_init (usb_core_driver *udev)
{
    uint32_t i = 0U, inten = 0U;

    uint32_t nptxfifolen = 0U;
    uint32_t ptxfifolen = 0U;

    /* restart the PHY Clock */
    *udev->regs.PWRCLKCTL = 0U;

    /* initialize host configuration register */
    if (USB_ULPI_PHY == udev->bp.phy_itf) {
        usb_phyclock_config (udev, HCTL_30_60MHZ); 
    } else {
        usb_phyclock_config (udev, HCTL_48MHZ);
    }

    /* support FS/LS only */
    udev->regs.hr->HCTL &= ~HCTL_SPDFSLS;

    /* configure data FIFOs size */
#ifdef USB_FS_CORE
    if (USB_CORE_ENUM_FS == udev->bp.core_enum) {
        /* set Rx FIFO size */
        udev->regs.gr->GRFLEN = USB_RX_FIFO_FS_SIZE;

        /* set non-periodic TX FIFO size and address */
        nptxfifolen |= USB_RX_FIFO_FS_SIZE;
        nptxfifolen |= USB_HTX_NPFIFO_FS_SIZE << 16U;
        udev->regs.gr->DIEP0TFLEN_HNPTFLEN = nptxfifolen;

        /* set periodic TX FIFO size and address */
        ptxfifolen |= USB_RX_FIFO_FS_SIZE + USB_HTX_NPFIFO_FS_SIZE;
        ptxfifolen |= USB_HTX_PFIFO_FS_SIZE << 16U;
        udev->regs.gr->HPTFLEN = ptxfifolen;
    }
#endif /* USB_FS_CORE */

#ifdef USB_HS_CORE
    if (USB_CORE_ENUM_HS == udev->bp.core_enum) {
        /* set Rx FIFO size */
        udev->regs.gr->GRFLEN = USB_RX_FIFO_HS_SIZE;

        /* set non-periodic Tx FIFO size and address */
        nptxfifolen |= USB_RX_FIFO_HS_SIZE;
        nptxfifolen |= USB_HTX_NPFIFO_HS_SIZE << 16U;
        udev->regs.gr->DIEP0TFLEN_HNPTFLEN = nptxfifolen;

        /* set periodic Tx FIFO size and address */
        ptxfifolen |= USB_RX_FIFO_HS_SIZE + USB_HTX_NPFIFO_HS_SIZE;
        ptxfifolen |= USB_HTX_PFIFO_HS_SIZE << 16U;
        udev->regs.gr->HPTFLEN = ptxfifolen;
    }
#endif /* USB_HS_CORE */

#ifdef USE_OTG_MODE

    /* clear host set HNP enable in the usb_otg control register */
    udev->regs.gr->GOTGCS &= ~GOTGCS_HHNPEN;

#endif /* USE_OTG_MODE */

    /* make sure the FIFOs are flushed */

    /* flush all TX FIFOs in device or host mode */
    usb_txfifo_flush (&udev->regs, 0x10U);

    /* flush the entire Rx FIFO */
    usb_rxfifo_flush (&udev->regs);

    /* disable all interrupts */
    udev->regs.gr->GINTEN = 0U;

    /* clear any pending USB OTG interrupts */
    udev->regs.gr->GOTGINTF = 0xFFFFFFFFU;

    /* enable the USB wakeup and suspend interrupts */
    udev->regs.gr->GINTF = 0xBFFFFFFFU;

    /* clear all pending host channel interrupts */
    for (i = 0U; i < udev->bp.num_pipe; i++) {
        udev->regs.pr[i]->HCHINTF = 0xFFFFFFFFU;
        udev->regs.pr[i]->HCHINTEN = 0U;
    }

#ifndef USE_OTG_MODE
    usb_portvbus_switch (udev, 1U);
#endif /* USE_OTG_MODE */

    udev->regs.gr->GINTEN = GINTEN_WKUPIE | GINTEN_SPIE;

    /* enable host_mode-related interrupts */
    if (USB_USE_FIFO == udev->bp.transfer_mode) {
        inten = GINTEN_RXFNEIE;
    }

    inten |= GINTEN_SESIE | GINTEN_HPIE | GINTEN_HCIE | GINTEN_ISOINCIE;

    udev->regs.gr->GINTEN |= inten;

    inten = GINTEN_DISCIE | GINTEN_SOFIE;

    udev->regs.gr->GINTEN &= ~inten;

    return USB_OK;
}

/*!
    \brief      control the VBUS to power
    \param[in]  udev: pointer to selected usb host
    \param[in]  state: VBUS state
    \param[out] none
    \retval     none
*/
void usb_portvbus_switch (usb_core_driver *udev, uint8_t state)
{
    uint32_t port = 0U;

    /* enable or disable the external charge pump */
    // usb_vbus_drive (state);

    /* turn on the host port power. */
    port = usb_port_read (udev);

    if (!(port & HPCS_PP) && (1U == state)) {
        port |= HPCS_PP;
    }

    if ((port & HPCS_PP) && (0U == state)) {
        port &= ~HPCS_PP;
    }

    *udev->regs.HPCS = port;

    usb_mdelay (200U);
}

/*!
    \brief      reset host port
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     operation status
*/
uint32_t usb_port_reset (usb_core_driver *udev)
{
    __IO uint32_t port = usb_port_read (udev);

    *udev->regs.HPCS = port | HPCS_PRST;

    usb_mdelay(20U); /* see note */

    *udev->regs.HPCS = port & ~HPCS_PRST;

    usb_mdelay(20U);

    return 1U;
}

/*!
    \brief      initialize host pipe
    \param[in]  udev: pointer to USB device
    \param[in]  pipe_num: host pipe number which is in (0..7)
    \param[out] none
    \retval     operation status
*/
usb_status usb_pipe_init (usb_core_driver *udev, uint8_t pipe_num)
{
    usb_status status = USB_OK;

    __IO uint32_t pp_ctl = 0U;
    __IO uint32_t pp_inten = HCHINTEN_TFIE;

    usb_pipe *pp = &udev->host.pipe[pipe_num];

    /* clear old interrupt conditions for this host channel */
    udev->regs.pr[pipe_num]->HCHINTF = 0xFFFFFFFFU;

    if (USB_USE_DMA == udev->bp.transfer_mode) {
        pp_inten |= HCHINTEN_DMAERIE;
    }

    if (pp->ep.dir) {
        pp_inten |= HCHINTEN_BBERIE;
    }

    /* enable channel interrupts required for this transfer */
    switch (pp->ep.type) {
    case USB_EPTYPE_CTRL:
    case USB_EPTYPE_BULK:
        pp_inten |= HCHINTEN_STALLIE | HCHINTEN_USBERIE \
                    | HCHINTEN_DTERIE | HCHINTEN_NAKIE;

        if (!pp->ep.dir) {
            pp_inten |= HCHINTEN_NYETIE;

            if (pp->ping) {
                pp_inten |= HCHINTEN_ACKIE;
            }
        }
        break;

    case USB_EPTYPE_INTR:
        pp_inten |= HCHINTEN_STALLIE | HCHINTEN_USBERIE | HCHINTEN_DTERIE \
                     | HCHINTEN_NAKIE | HCHINTEN_REQOVRIE;
        break;

    case USB_EPTYPE_ISOC:
        pp_inten |= HCHINTEN_REQOVRIE | HCHINTEN_ACKIE;

        if (pp->ep.dir) {
            pp_inten |= HCHINTEN_USBERIE;
        }
        break;

    default:
        break;
    }

    udev->regs.pr[pipe_num]->HCHINTEN = pp_inten;

    /* enable the top level host channel interrupt */
    udev->regs.hr->HACHINTEN |= 1U << pipe_num;

    /* make sure host channel interrupts are enabled */
    udev->regs.gr->GINTEN |= GINTEN_HCIE;

    /* program the host channel control register */
    pp_ctl |= PIPE_CTL_DAR(pp->dev_addr);
    pp_ctl |= PIPE_CTL_EPNUM(pp->ep.num);
    pp_ctl |= PIPE_CTL_EPDIR(pp->ep.dir);
    pp_ctl |= PIPE_CTL_EPTYPE(pp->ep.type);
    pp_ctl |= PIPE_CTL_LSD(pp->dev_speed == PORT_SPEED_LOW);

    pp_ctl |= pp->ep.mps;
    pp_ctl |= ((uint32_t)(pp->ep.type == USB_EPTYPE_INTR) << 29U) & HCHCTL_ODDFRM;

    udev->regs.pr[pipe_num]->HCHCTL = pp_ctl;

    return status;
}

/*!
    \brief      prepare host channel for transferring packets
    \param[in]  udev: pointer to USB device
    \param[in]  pipe_num: host pipe number which is in (0..7)
    \param[out] none
    \retval     operation status
*/
usb_status usb_pipe_xfer (usb_core_driver *udev, uint8_t pipe_num)
{
    usb_status status = USB_OK;

    uint16_t dword_len = 0U;
    uint16_t packet_count = 0U;

    __IO uint32_t pp_ctl = 0U;

    usb_pipe *pp = &udev->host.pipe[pipe_num];

    uint16_t max_packet_len = pp->ep.mps;

    /* compute the expected number of packets associated to the transfer */
    if (pp->xfer_len > 0U) {
        packet_count = (uint16_t)((pp->xfer_len + max_packet_len - 1U) / max_packet_len);

        if (packet_count > HC_MAX_PACKET_COUNT) {
            packet_count = HC_MAX_PACKET_COUNT;
            pp->xfer_len = (uint16_t)(packet_count * max_packet_len);
        }
    } else {
        packet_count = 1U;
    }

    if (pp->ep.dir) {
        pp->xfer_len = (uint16_t)(packet_count * max_packet_len);
    }

    /* initialize the host channel transfer information */
    udev->regs.pr[pipe_num]->HCHLEN = pp->xfer_len | pp->DPID | PIPE_XFER_PCNT(packet_count);

    if (USB_USE_DMA == udev->bp.transfer_mode) {
        udev->regs.pr[pipe_num]->HCHDMAADDR = (unsigned int)pp->xfer_buf;
    }

    pp_ctl = udev->regs.pr[pipe_num]->HCHCTL;

    if (usb_frame_even(udev)) {
        pp_ctl |= HCHCTL_ODDFRM;
    } else {
        pp_ctl &= ~HCHCTL_ODDFRM;
    }

    /* set host channel enabled */
    pp_ctl |= HCHCTL_CEN;
    pp_ctl &= ~HCHCTL_CDIS;

    udev->regs.pr[pipe_num]->HCHCTL = pp_ctl;

    if (USB_USE_FIFO == udev->bp.transfer_mode) {
        if ((0U == pp->ep.dir) && (pp->xfer_len > 0U)) {
            switch (pp->ep.type) {
            /* non-periodic transfer */
            case USB_EPTYPE_CTRL:
            case USB_EPTYPE_BULK:
                dword_len = (uint16_t)((pp->xfer_len + 3U) / 4U);

                /* check if there is enough space in FIFO space */
                if (dword_len > (udev->regs.gr->HNPTFQSTAT & HNPTFQSTAT_NPTXFS)) {
                    /* need to process data in nptxfempty interrupt */
                    udev->regs.gr->GINTEN |= GINTEN_NPTXFEIE;
                }
                break;

            /* periodic transfer */
            case USB_EPTYPE_INTR:
            case USB_EPTYPE_ISOC:
                dword_len = (uint16_t)((pp->xfer_len + 3U) / 4U);

                /* check if there is enough space in fifo space */
                if (dword_len > (udev->regs.hr->HPTFQSTAT & HPTFQSTAT_PTXFS)) {
                    /* need to process data in ptxfempty interrupt */
                    udev->regs.gr->GINTEN |= GINTEN_PTXFEIE;
                }
                break;

            default:
                break;
            }

            /* write packet into the TX fifo. */
            usb_txfifo_write (&udev->regs, pp->xfer_buf, pipe_num, (uint16_t)pp->xfer_len);
        }
    }

    return status;
}

/*!
    \brief      halt pipe
    \param[in]  udev: pointer to USB device
    \param[in]  pipe_num: host pipe number which is in (0..7)
    \param[out] none
    \retval     operation status
*/
usb_status usb_pipe_halt (usb_core_driver *udev, uint8_t pipe_num)
{
    __IO uint32_t pp_ctl = udev->regs.pr[pipe_num]->HCHCTL;

    uint8_t ep_type = (uint8_t)((pp_ctl & HCHCTL_EPTYPE) >> 18U);

    pp_ctl |= HCHCTL_CEN | HCHCTL_CDIS;

    switch (ep_type) {
    case USB_EPTYPE_CTRL:
    case USB_EPTYPE_BULK:
        if (0U == (udev->regs.gr->HNPTFQSTAT & HNPTFQSTAT_NPTXFS)) {
            pp_ctl &= ~HCHCTL_CEN;
        }
        break;

    case USB_EPTYPE_INTR:
    case USB_EPTYPE_ISOC:
        if (0U == (udev->regs.hr->HPTFQSTAT & HPTFQSTAT_PTXFS)) {
            pp_ctl &= ~HCHCTL_CEN;
        }
        break;

    default:
        break;
    }

    udev->regs.pr[pipe_num]->HCHCTL = pp_ctl;

    return USB_OK;
}

/*!
    \brief      configure host pipe to do ping operation
    \param[in]  udev: pointer to USB device
    \param[in]  pipe_num: host pipe number which is in (0..7)
    \param[out] none
    \retval     operation status
*/
usb_status usb_pipe_ping (usb_core_driver *udev, uint8_t pipe_num)
{
    uint32_t pp_ctl = 0U;

    udev->regs.pr[pipe_num]->HCHLEN = HCHLEN_PING | (HCHLEN_PCNT & (1U << 19U));

    pp_ctl = udev->regs.pr[pipe_num]->HCHCTL;
 
    pp_ctl |= HCHCTL_CEN;
    pp_ctl &= ~HCHCTL_CDIS;

    udev->regs.pr[pipe_num]->HCHCTL = pp_ctl;

    return USB_OK;
}

/*!
    \brief      stop the USB host and clean up FIFO
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     none
*/
void usb_host_stop (usb_core_driver *udev)
{
    uint32_t i;
    __IO uint32_t pp_ctl = 0U;

    udev->regs.hr->HACHINTEN = 0x0U;
    udev->regs.hr->HACHINT = 0xFFFFFFFFU;

    /* flush out any leftover queued requests. */
    for (i = 0U; i < udev->bp.num_pipe; i++) {
        pp_ctl = udev->regs.pr[i]->HCHCTL;

        pp_ctl &= ~(HCHCTL_CEN | HCHCTL_EPDIR);
        pp_ctl |= HCHCTL_CDIS;

        udev->regs.pr[i]->HCHCTL = pp_ctl;
    }

    /* flush the FIFO */
    usb_rxfifo_flush (&udev->regs);
    usb_txfifo_flush (&udev->regs, 0x10U);
}
