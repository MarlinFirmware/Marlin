/*!
    \file    drv_usbd_int.c
    \brief   USB device mode interrupt routines

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

#include "gdusbd_conf.h"
#include "drv_usbd_int.h"
#include "gdusbd_transc.h"

static uint32_t usbd_int_epout                 (usb_core_driver *udev);
static uint32_t usbd_int_epin                  (usb_core_driver *udev);
static uint32_t usbd_int_rxfifo                (usb_core_driver *udev);
static uint32_t usbd_int_reset                 (usb_core_driver *udev);
static uint32_t usbd_int_enumfinish            (usb_core_driver *udev);
static uint32_t usbd_int_suspend               (usb_core_driver *udev);

static uint32_t usbd_emptytxfifo_write         (usb_core_driver *udev, uint32_t ep_num);

static const uint8_t USB_SPEED[4] = {
    [DSTAT_EM_HS_PHY_30MHZ_60MHZ] = (uint8_t)USB_SPEED_HIGH,
    [DSTAT_EM_FS_PHY_30MHZ_60MHZ] = (uint8_t)USB_SPEED_FULL,
    [DSTAT_EM_FS_PHY_48MHZ] = (uint8_t)USB_SPEED_FULL,
    [DSTAT_EM_LS_PHY_6MHZ] = (uint8_t)USB_SPEED_LOW
};

/*!
    \brief      USB device-mode interrupts global service routine handler
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
void usbd_isr (usb_core_driver *udev)
{
    if (HOST_MODE != (udev->regs.gr->GINTF & GINTF_COPM)) {
        uint32_t intr = udev->regs.gr->GINTF & udev->regs.gr->GINTEN;

        /* there are no interrupts, avoid spurious interrupt */
        if (!intr) {
            return;
        }

        /* OUT endpoints interrupts */
        if (intr & GINTF_OEPIF) {
            (void)usbd_int_epout (udev);
        }

        /* IN endpoints interrupts */
        if (intr & GINTF_IEPIF) {
            (void)usbd_int_epin (udev);
        }

        /* suspend interrupt */
        if (intr & GINTF_SP) {
            (void)usbd_int_suspend (udev);
        }

        /* wakeup interrupt */
        if (intr & GINTF_WKUPIF) {
            /* inform upper layer by the resume event */
            udev->dev.cur_status = USBD_CONFIGURED;

            /* clear interrupt */
            udev->regs.gr->GINTF = GINTF_WKUPIF;
        }

        /* start of frame interrupt */
        if (intr & GINTF_SOF) {
            if (udev->dev.class_core->SOF) {
                (void)udev->dev.class_core->SOF(udev); 
            }

            /* clear interrupt */
            udev->regs.gr->GINTF = GINTF_SOF;
        }

        /* receive FIFO not empty interrupt */
        if (intr & GINTF_RXFNEIF) {
            (void)usbd_int_rxfifo (udev);
        }

        /* USB reset interrupt */
        if (intr & GINTF_RST) {
            (void)usbd_int_reset (udev);
        }

        /* enumeration has been done interrupt */
        if (intr & GINTF_ENUMFIF) {
            (void)usbd_int_enumfinish (udev);
        }

        /* incomplete synchronization IN transfer interrupt*/
        if (intr & GINTF_ISOINCIF) {
            if (NULL != udev->dev.class_core->incomplete_isoc_in) {
                (void)udev->dev.class_core->incomplete_isoc_in(udev);
            }

            /* Clear interrupt */
            udev->regs.gr->GINTF = GINTF_ISOINCIF;
        }

        /* incomplete synchronization OUT transfer interrupt*/
        if (intr & GINTF_ISOONCIF) {
            if (NULL != udev->dev.class_core->incomplete_isoc_out) {
                (void)udev->dev.class_core->incomplete_isoc_out(udev);
            }

            /* clear interrupt */
            udev->regs.gr->GINTF = GINTF_ISOONCIF;
        }

#ifdef VBUS_SENSING_ENABLED

        /* session request interrupt */
        if (intr & GINTF_SESIF) {
            udev->regs.gr->GINTF = GINTF_SESIF;
        }

        /* OTG mode interrupt */
        if (intr & GINTF_OTGIF) {
            if(udev->regs.gr->GOTGINTF & GOTGINTF_SESEND) {

            }

            /* clear OTG interrupt */
            udev->regs.gr->GINTF = GINTF_OTGIF;
        }
#endif /* VBUS_SENSING_ENABLED */
    }
}

#ifdef USB_HS_DEDICATED_EP1_ENABLED

/*!
    \brief      USB dedicated OUT endpoint 1 interrupt service routine handler
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     operation status
*/
uint32_t usbd_int_dedicated_ep1out (usb_core_driver *udev)
{
    uint32_t oepintr = 0U;
    uint32_t oeplen = 0U;

    oepintr = udev->regs.er_out[1]->DOEPINTF;
    oepintr &= udev->regs.dr->DOEP1INTEN;

    /* transfer complete */
    if(oepintr & DOEPINTF_TF){
        /* clear the bit in DOEPINTn for this interrupt */
        udev->regs.er_out[1]->DOEPINTF = DOEPINTF_TF;

        if(USB_USE_DMA == udev->bp.transfer_mode){
            oeplen = udev->regs.er_out[1]->DOEPLEN;

            /* to do : handle more than one single max packet size packet */
            udev->dev.transc_out[1].xfer_count = udev->dev.transc_out[1].max_len - \
                                    (oeplen & DEPLEN_TLEN);
        }

        /* rx complete */
        usbd_out_transc (udev, 1U);
    }

    return 1U;
}

/*!
    \brief      USB dedicated IN endpoint 1 interrupt service routine handler
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     operation status
*/
uint32_t usbd_int_dedicated_ep1in (usb_core_driver *udev)
{
    uint32_t inten, intr, emptyen;

    inten = udev->regs.dr->DIEP1INTEN;
    emptyen = udev->regs.dr->DIEPFEINTEN;

    inten |= ((emptyen >> 1 ) & 0x1) << 7;

    intr = udev->regs.er_in[1]->DIEPINTF & inten;

    if(intr & DIEPINTF_TF){
        udev->regs.dr->DIEPFEINTEN &= ~(0x1 << 1);

        udev->regs.er_in[1]->DIEPINTF = DIEPINTF_TF;

        /* TX complete */
        usbd_in_transc (udev, 1);
    }

    if(intr & DIEPINTF_TXFE){
        usbd_emptytxfifo_write(udev, 1);

        udev->regs.er_in[1]->DIEPINTF = DIEPINTF_TXFE;
    }

    return 1;
}

#endif

/*!
    \brief      indicates that an OUT endpoint has a pending interrupt
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     operation status
*/
static uint32_t usbd_int_epout (usb_core_driver *udev)
{
    uint32_t epintnum = 0U;
    uint8_t ep_num = 0U;

    for (epintnum = usb_oepintnum_read (udev); epintnum; epintnum >>= 1, ep_num++) {
        if (epintnum & 0x01U) {
            __IO uint32_t oepintr = usb_oepintr_read (udev, ep_num);

            /* transfer complete interrupt */
            if (oepintr & DOEPINTF_TF) {
                /* clear the bit in DOEPINTF for this interrupt */
                udev->regs.er_out[ep_num]->DOEPINTF = DOEPINTF_TF;

                if ((uint8_t)USB_USE_DMA == udev->bp.transfer_mode) {
                    __IO uint32_t eplen = udev->regs.er_out[ep_num]->DOEPLEN;

                    udev->dev.transc_out[ep_num].xfer_count = udev->dev.transc_out[ep_num].max_len - \
                                                                (eplen & DEPLEN_TLEN);
                }

                /* inform upper layer: data ready */
                (void)usbd_out_transc (udev, ep_num);

                if ((uint8_t)USB_USE_DMA == udev->bp.transfer_mode) {
                    if ((0U == ep_num) && ((uint8_t)USB_CTL_STATUS_OUT == udev->dev.control.ctl_state)) {
                        usb_ctlep_startout (udev);
                    }
                }
            }

            /* setup phase finished interrupt (control endpoints) */
            if (oepintr & DOEPINTF_STPF) {
                /* inform the upper layer that a setup packet is available */
                (void)usbd_setup_transc (udev);

                udev->regs.er_out[ep_num]->DOEPINTF = DOEPINTF_STPF;
            }
        }
    }

    return 1U;
}

/*!
    \brief      indicates that an IN endpoint has a pending interrupt
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     operation status
*/
static uint32_t usbd_int_epin (usb_core_driver *udev)
{
    uint32_t epintnum = 0U;
    uint8_t ep_num = 0U;

    for (epintnum = usb_iepintnum_read (udev); epintnum; epintnum >>= 1, ep_num++) {
        if (epintnum & 0x1U) {
            __IO uint32_t iepintr = usb_iepintr_read (udev, ep_num);

            if (iepintr & DIEPINTF_TF) {
                udev->regs.er_in[ep_num]->DIEPINTF = DIEPINTF_TF;

                /* data transmission is completed */
                (void)usbd_in_transc (udev, ep_num);

                if ((uint8_t)USB_USE_DMA == udev->bp.transfer_mode) {
                    if ((0U == ep_num) && ((uint8_t)USB_CTL_STATUS_IN == udev->dev.control.ctl_state)) {
                        usb_ctlep_startout (udev);
                    }
                }
            }

            if (iepintr & DIEPINTF_TXFE) {
                usbd_emptytxfifo_write (udev, (uint32_t)ep_num);

                udev->regs.er_in[ep_num]->DIEPINTF = DIEPINTF_TXFE;
            }
        }
    }

    return 1U;
}

/*!
    \brief      handle the RX status queue level interrupt
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     operation status
*/
static uint32_t usbd_int_rxfifo (usb_core_driver *udev)
{
    usb_transc *transc = NULL;

    uint8_t data_PID = 0U;
    uint32_t bcount = 0U;

    __IO uint32_t devrxstat = 0U;

    /* disable the Rx status queue non-empty interrupt */
    udev->regs.gr->GINTEN &= ~GINTEN_RXFNEIE;

    /* get the status from the top of the FIFO */
    devrxstat = udev->regs.gr->GRSTATP;

    uint8_t ep_num = (uint8_t)(devrxstat & GRSTATRP_EPNUM);

    transc = &udev->dev.transc_out[ep_num];

    bcount = (devrxstat & GRSTATRP_BCOUNT) >> 4U;
    data_PID = (uint8_t)((devrxstat & GRSTATRP_DPID) >> 15U);

#ifdef USE_USB_HS
    #ifndef USE_450Z_EVAL
    /* ensure no-DMA mode can work */
    if ((1U == ep_num) && (0U == (udev->regs.er_out[ep_num]->DOEPLEN & DEPLEN_PCNT))) {
        uint32_t devepctl = udev->regs.er_out[ep_num]->DOEPCTL;

        devepctl |= DEPCTL_SNAK;
        devepctl &= ~DEPCTL_EPEN;
        devepctl &= ~DEPCTL_EPD;

        udev->regs.er_out[ep_num]->DOEPCTL = devepctl;
    }
    #endif /* USE_450Z_EVAL */
#endif /* USE_USB_HS */

    switch ((devrxstat & GRSTATRP_RPCKST) >> 17U) {
    case RSTAT_GOUT_NAK:
        break;

    case RSTAT_DATA_UPDT:
        if (bcount > 0U) {
            (void)usb_rxfifo_read (&udev->regs, transc->xfer_buf, (uint16_t)bcount);

            transc->xfer_buf += bcount;
            transc->xfer_count += bcount;
        }
        break;

    case RSTAT_XFER_COMP:
        /* trigger the OUT endpoint interrupt */
        break;

    case RSTAT_SETUP_COMP:
        /* trigger the OUT endpoint interrupt */
        break;

    case RSTAT_SETUP_UPDT:
        if ((0U == transc->ep_addr.num) && (8U == bcount) && (DPID_DATA0 == data_PID)) {
            /* copy the setup packet received in FIFO into the setup buffer in RAM */
            (void)usb_rxfifo_read (&udev->regs, (uint8_t *)&udev->dev.control.req, (uint16_t)bcount);

            transc->xfer_count += bcount;
        }
        break;

    default:
        break;
    }

    /* enable the Rx status queue level interrupt */
    udev->regs.gr->GINTEN |= GINTEN_RXFNEIE;

    return 1U;
}

/*!
    \brief      handle USB reset interrupt
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status
*/
static uint32_t usbd_int_reset (usb_core_driver *udev)
{
    uint32_t i;

    /* clear the remote wakeup signaling */
    udev->regs.dr->DCTL &= ~DCTL_RWKUP;

    /* flush the TX FIFO */
    (void)usb_txfifo_flush (&udev->regs, 0U);

    for (i = 0U; i < udev->bp.num_ep; i++) {
        udev->regs.er_in[i]->DIEPINTF = 0xFFU;
        udev->regs.er_out[i]->DOEPINTF = 0xFFU;
    }

    /* clear all pending device endpoint interrupts */
    udev->regs.dr->DAEPINT = 0xFFFFFFFFU;

    /* enable endpoint 0 interrupts */
    udev->regs.dr->DAEPINTEN = 1U | (1U << 16U);

    /* enable OUT endpoint interrupts */
    udev->regs.dr->DOEPINTEN = DOEPINTEN_STPFEN | DOEPINTEN_TFEN;

#ifdef USB_HS_DEDICATED_EP1_ENABLED
    udev->regs.dr->DOEP1INTEN = DOEPINTEN_STPFEN | DOEPINTEN_TFEN;
#endif

    /* enable IN endpoint interrupts */
    udev->regs.dr->DIEPINTEN = DIEPINTEN_TFEN;

#ifdef USB_HS_DEDICATED_EP1_ENABLED
    udev->regs.dr->DIEP1INTEN = DIEPINTEN_TFEN;
#endif

    /* reset device address */
    udev->regs.dr->DCFG &= ~DCFG_DAR;

    /* configure endpoint 0 to receive SETUP packets */
    usb_ctlep_startout (udev);

    /* clear USB reset interrupt */
    udev->regs.gr->GINTF = GINTF_RST;

    udev->dev.transc_out[0] = (usb_transc) {
        .ep_type = USB_EPTYPE_CTRL,
        .max_len = USB_FS_EP0_MAX_LEN
    };

    (void)usb_transc_active (udev, &udev->dev.transc_out[0]);

    udev->dev.transc_in[0] = (usb_transc) {
        .ep_addr = {
            .dir = 1U
        },

        .ep_type = USB_EPTYPE_CTRL,
        .max_len = USB_FS_EP0_MAX_LEN
    };

    (void)usb_transc_active (udev, &udev->dev.transc_in[0]);

    /* upon reset call user call back */
    udev->dev.cur_status = (uint8_t)USBD_DEFAULT;

    return 1U;
}

/*!
    \brief      handle USB speed enumeration finish interrupt
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     status
*/
static uint32_t usbd_int_enumfinish (usb_core_driver *udev)
{
    uint8_t enum_speed = (uint8_t)((udev->regs.dr->DSTAT & DSTAT_ES) >> 1U);

    udev->regs.dr->DCTL &= ~DCTL_CGINAK;
    udev->regs.dr->DCTL |= DCTL_CGINAK;

    udev->regs.gr->GUSBCS &= ~GUSBCS_UTT;

    /* set USB turn-around time based on device speed and PHY interface */
    if (USB_SPEED[enum_speed] == (uint8_t)USB_SPEED_HIGH) {
        udev->bp.core_speed = (uint8_t)USB_SPEED_HIGH;

        udev->regs.gr->GUSBCS |= 0x09U << 10U;
    } else {
        udev->bp.core_speed = (uint8_t)USB_SPEED_FULL;

        udev->regs.gr->GUSBCS |= 0x05U << 10U;
    }

    /* clear interrupt */
    udev->regs.gr->GINTF = GINTF_ENUMFIF;

    return 1U;
}

/*!
    \brief      USB suspend interrupt handler
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     operation status
*/
static uint32_t usbd_int_suspend (usb_core_driver *udev)
{
    __IO uint8_t low_power = udev->bp.low_power;
    __IO uint8_t suspend = (uint8_t)(udev->regs.dr->DSTAT & DSTAT_SPST);
    __IO uint8_t is_configured = (udev->dev.cur_status == (uint8_t)USBD_CONFIGURED)? 1U : 0U;

    udev->dev.backup_status = udev->dev.cur_status;
    udev->dev.cur_status = (uint8_t)USBD_SUSPENDED;

    if (low_power && suspend && is_configured) {
        /* switch-off the OTG clocks */
        *udev->regs.PWRCLKCTL |= PWRCLKCTL_SUCLK | PWRCLKCTL_SHCLK;

        /* enter DEEP_SLEEP mode with LDO in low power mode */
        // pmu_to_deepsleepmode(PMU_LDO_LOWPOWER, WFI_CMD);
    }

    /* clear interrupt */
    udev->regs.gr->GINTF = GINTF_SP;

    return 1U;
}

/*!
    \brief      check FIFO for the next packet to be loaded
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint identifier which is in (0..3)
    \param[out] none
    \retval     status
*/
static uint32_t usbd_emptytxfifo_write (usb_core_driver *udev, uint32_t ep_num)
{
    uint32_t len;
    uint32_t word_count;

    usb_transc *transc = &udev->dev.transc_in[ep_num];

    len = transc->xfer_len - transc->xfer_count;

    /* get the data length to write */
    if (len > transc->max_len) {
        len = transc->max_len;
    }

    word_count = (len + 3U) / 4U;

    while (((udev->regs.er_in[ep_num]->DIEPTFSTAT & DIEPTFSTAT_IEPTFS) >= word_count) && \
              (transc->xfer_count < transc->xfer_len)) {
        len = transc->xfer_len - transc->xfer_count;

        if (len > transc->max_len) {
            len = transc->max_len;
        }

        /* write FIFO in word(4bytes) */
        word_count = (len + 3U) / 4U;

        /* write the FIFO */
        (void)usb_txfifo_write (&udev->regs, transc->xfer_buf, (uint8_t)ep_num, (uint16_t)len);

        transc->xfer_buf += len;
        transc->xfer_count += len;

        if (transc->xfer_count == transc->xfer_len) {
            /* disable the device endpoint FIFO empty interrupt */
            udev->regs.dr->DIEPFEINTEN &= ~(0x01U << ep_num);
        }
    }

    return 1U;
}
