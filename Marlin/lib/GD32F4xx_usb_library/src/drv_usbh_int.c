/*!
    \file    drv_usbh_int.c
    \brief   USB host mode interrupt handler file

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
#include "drv_usb_host.h"
#include "drv_usbh_int.h"
#include "gdusbh_core.h"

#if defined   (__CC_ARM)        /*!< ARM compiler */
    #pragma O0
#elif defined (__GNUC__)        /*!< GNU compiler */
    #pragma GCC optimize ("O0")
#elif defined  (__TASKING__)    /*!< TASKING compiler */ 
    #pragma optimize=0
#endif /* __CC_ARM */

/* local function prototypes ('static') */
static uint32_t usbh_int_port           (usb_core_driver *udev);
static uint32_t usbh_int_pipe           (usb_core_driver *udev);
static uint32_t usbh_int_pipe_in        (usb_core_driver *udev, uint32_t pp_num);
static uint32_t usbh_int_pipe_out       (usb_core_driver *udev, uint32_t pp_num);
static uint32_t usbh_int_rxfifonoempty  (usb_core_driver *udev);
static uint32_t usbh_int_txfifoempty    (usb_core_driver *udev, usb_pipe_mode pp_mode);

/*!
    \brief      handle global host interrupt
    \param[in]  udev: pointer to USB core instance
    \param[out] none
    \retval     operation status
*/
uint32_t usbh_isr (usb_core_driver *udev)
{
    uint32_t retval = 0U;

    __IO uint32_t intr = 0U;

    /* check if host mode */
    if (HOST_MODE == (udev->regs.gr->GINTF & GINTF_COPM)) {
        intr = usb_coreintr_get(&udev->regs);

        if (!intr) {
            return 0U;
        }

        if (intr & GINTF_SOF) {
            usbh_int_fop->SOF(udev->host.data);

            /* clear interrupt */
            udev->regs.gr->GINTF = GINTF_SOF;
        }

        if (intr & GINTF_RXFNEIF) {
            retval |= usbh_int_rxfifonoempty (udev);
        }

        if (intr & GINTF_NPTXFEIF) {
            retval |= usbh_int_txfifoempty (udev, PIPE_NON_PERIOD);
        }

        if (intr & GINTF_PTXFEIF) {
            retval |= usbh_int_txfifoempty (udev, PIPE_PERIOD);
        }

        if (intr & GINTF_HCIF) {
            retval |= usbh_int_pipe (udev);
        }

        if (intr & GINTF_HPIF) {
            retval |= usbh_int_port (udev);
        }

        if (intr & GINTF_DISCIF) {
            usbh_int_fop->disconnect(udev->host.data);

            /* clear interrupt */
            udev->regs.gr->GINTF = GINTF_DISCIF;
        }

        if (intr & GINTF_ISOONCIF) {
            udev->regs.pr[0]->HCHCTL |= HCHCTL_CEN | HCHCTL_CDIS;

            /* clear interrupt */
            udev->regs.gr->GINTF = GINTF_ISOONCIF;
        }

        if (intr & GINTF_SESIF) {
            usb_portvbus_switch (udev, 1U);

            udev->regs.gr->GINTF = GINTF_SESIF;
        }

        if (intr & GINTF_WKUPIF) {
            /* clear interrupt */
            udev->regs.gr->GINTF = GINTF_WKUPIF;
        }
    }

    return retval;
}

/*!
    \brief      handle USB pipe halt
    \param[in]  udev: pointer to USB core instance
    \param[in]  pp_num: pp_num: host channel number which is in (0..7)
    \param[in]  pp_int: pipe interrupt
    \param[in]  pp_status: pipe status
    \param[out] none
    \retval     none
*/
static inline void usb_pp_halt (usb_core_driver *udev, 
                                uint8_t pp_num, 
                                uint32_t pp_int,
                                usb_pipe_staus pp_status)
{
    udev->regs.pr[pp_num]->HCHINTEN |= HCHINTEN_CHIE;

    usb_pipe_halt(udev, pp_num);

    udev->regs.pr[pp_num]->HCHINTF = pp_int;

    udev->host.pipe[pp_num].pp_status = pp_status;
}

/*!
    \brief      handle the host port interrupt
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     operation status
*/
#if defined (__ICCARM__)      /*!< IAR compiler */
    #pragma optimize = none
#endif /* __ICCARM */
static uint32_t usbh_int_port (usb_core_driver *udev)
{
    uint32_t retval = 0U;

    /* note: when the USB PHY use USB HS PHY, the flag is needed */
    uint8_t port_reset = 0U;

    __IO uint32_t port_state = *udev->regs.HPCS;

    /* clear the interrupt bits in GINTSTS */
    port_state &= ~(HPCS_PE | HPCS_PCD | HPCS_PEDC);

    /* port connect detected */
    if (*udev->regs.HPCS & HPCS_PCD) {
        port_state |= HPCS_PCD;

        usbh_int_fop->connect(udev->host.data);

        retval |= 1U;
    }

    /* port enable changed */
    if (*udev->regs.HPCS & HPCS_PEDC) {
        port_state |= HPCS_PEDC;

        if (*udev->regs.HPCS & HPCS_PE) {
            uint32_t port_speed = usb_curspeed_get(udev);
            uint32_t clock_type = udev->regs.hr->HCTL & HCTL_CLKSEL;

            udev->host.connect_status = 1U;

            if (PORT_SPEED_LOW == port_speed) {
                udev->regs.hr->HFT = 6000U;

                if (HCTL_6MHZ != clock_type) {
                    if (USB_EMBEDDED_PHY == udev->bp.phy_itf) {
                        usb_phyclock_config (udev, HCTL_6MHZ);
                    }

                    port_reset = 1U;
                }
            } else if (PORT_SPEED_FULL == port_speed) {
                udev->regs.hr->HFT = 48000U;

                if (HCTL_48MHZ != clock_type) {
                    usb_phyclock_config (udev, HCTL_48MHZ);
                }

                port_reset = 1U;
            } else {
                /* for high speed device and others */
                port_reset = 1U;
            }

            usbh_int_fop->port_enabled(udev->host.data);

            udev->regs.gr->GINTEN |= GINTEN_DISCIE | GINTEN_SOFIE;
        } else {
            usbh_int_fop->port_disabled(udev->host.data);
        }
    }

    if (port_reset) {
        usb_port_reset(udev);
    }

    /* clear port interrupts */
    *udev->regs.HPCS = port_state;

    return retval;
}

/*!
    \brief      handle all host channels interrupt
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     operation status
*/
static uint32_t usbh_int_pipe (usb_core_driver *udev)
{
    uint32_t pp_num = 0U;
    uint32_t retval = 0U;

    for (pp_num = 0U; pp_num < udev->bp.num_pipe; pp_num++) {
        if ((udev->regs.hr->HACHINT & HACHINT_HACHINT) & (1UL << pp_num)) {
            if (udev->regs.pr[pp_num]->HCHCTL & HCHCTL_EPDIR) {
                retval |= usbh_int_pipe_in (udev, pp_num);
            } else {
                retval |= usbh_int_pipe_out (udev, pp_num);
            }
        }
    }

    return retval;
}

/*!
    \brief      handle the IN channel interrupt
    \param[in]  udev: pointer to USB device instance
    \param[in]  pp_num: host channel number which is in (0..7)
    \param[out] none
    \retval     operation status
*/
#if defined (__ICCARM__)      /*!< IAR compiler */
    #pragma optimize = none
#endif /* __ICCARM */
static uint32_t usbh_int_pipe_in (usb_core_driver *udev, uint32_t pp_num)
{
    usb_pr *pp_reg = udev->regs.pr[pp_num];

    usb_pipe *pp = &udev->host.pipe[pp_num];

    __IO uint32_t intr_pp = pp_reg->HCHINTF & pp_reg->HCHINTEN;

    uint8_t ep_type = (uint8_t)((pp_reg->HCHCTL & HCHCTL_EPTYPE) >> 18U);

    if (intr_pp & HCHINTF_ACK) {
        pp_reg->HCHINTF = HCHINTF_ACK;
    } else if (intr_pp & HCHINTF_STALL) {
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_STALL, PIPE_STALL);
        pp_reg->HCHINTF = HCHINTF_NAK;

        /* note: When there is a 'STALL', reset also NAK,
           else, the udev->host.pp_status = HC_STALL
           will be overwritten by 'NAK' in code below */
        intr_pp &= ~HCHINTF_NAK;
    } else if (intr_pp & HCHINTF_DTER) {
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_DTER, PIPE_DTGERR);
        pp_reg->HCHINTF = HCHINTF_NAK;
    } else {
        /* no operation */
    }

    if (intr_pp & HCHINTF_REQOVR) {
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_REQOVR, PIPE_REQOVR);
    } else if (intr_pp & HCHINTF_TF) {
        if ((uint8_t)USB_USE_DMA == udev->bp.transfer_mode) {
            udev->host.backup_xfercount[pp_num] = pp->xfer_len - (pp_reg->HCHLEN & HCHLEN_TLEN);
        }

        pp->pp_status = PIPE_XF;
        pp->err_count = 0U;

        pp_reg->HCHINTF = HCHINTF_TF;

        switch (ep_type) {
        case USB_EPTYPE_CTRL:
        case USB_EPTYPE_BULK:
            usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_NAK, PIPE_XF);

            pp->data_toggle_in ^= 1U;
            break;

        case USB_EPTYPE_INTR:
        case USB_EPTYPE_ISOC:
            pp_reg->HCHCTL |= HCHCTL_ODDFRM;
            pp->urb_state = GD_URB_DONE;
            break;

        default:
            break;
        }
    } else if (intr_pp & HCHINTF_CH) {
        pp_reg->HCHINTEN &= ~HCHINTEN_CHIE;

        switch (pp->pp_status) {
        case PIPE_XF:
            pp->urb_state = GD_URB_DONE;
            break;

        case PIPE_STALL:
            pp->urb_state = GD_URB_STALL;
            break;

        case PIPE_TRACERR:
        case PIPE_DTGERR:
            pp->err_count = 0U;
            pp->urb_state = GD_URB_ERROR;

            pp->data_toggle_in ^= 1U;
            break;

        case PIPE_IDLE:
        case PIPE_HALTED:
        case PIPE_NAK:
        case PIPE_NYET:
        case PIPE_BBERR:
        case PIPE_REQOVR:
        default:
            if((uint8_t)USB_EPTYPE_INTR == ep_type) {
                pp->data_toggle_in ^= 1U;
            }
            break;
        }

        pp_reg->HCHINTF = HCHINTF_CH;
    } else if (intr_pp & HCHINTF_USBER) {
        pp->err_count++;
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_USBER, PIPE_TRACERR);
    } else if (intr_pp & HCHINTF_NAK) {
        switch (ep_type) {
        case USB_EPTYPE_CTRL:
        case USB_EPTYPE_BULK:
            /* re-activate the channel */
            pp_reg->HCHCTL = (pp_reg->HCHCTL | HCHCTL_CEN) & ~HCHCTL_CDIS;
            break;

        case USB_EPTYPE_INTR:
            pp_reg->HCHINTEN |= HCHINTEN_CHIE;

            (void)usb_pipe_halt(udev, (uint8_t)pp_num);
            break;

        default:
            break;
        }

        pp->pp_status = PIPE_NAK;

        pp_reg->HCHINTF = HCHINTF_NAK;
    } else {
        /* no operation */
    }

    return 1U;
}

/*!
    \brief      handle the OUT channel interrupt
    \param[in]  udev: pointer to USB device instance
    \param[in]  pp_num: host channel number which is in (0..7)
    \param[out] none
    \retval     operation status
*/
#if defined (__ICCARM__)      /*!< IAR compiler */
    #pragma optimize = none
#endif /* __ICCARM */
static uint32_t usbh_int_pipe_out (usb_core_driver *udev, uint32_t pp_num)
{
    usb_pr *pp_reg = udev->regs.pr[pp_num];

    usb_pipe *pp = &udev->host.pipe[pp_num];

    uint32_t intr_pp = pp_reg->HCHINTF & pp_reg->HCHINTEN;

    if (intr_pp & HCHINTF_ACK) {
        if (GD_URB_PING == pp->urb_state) {
            pp->err_count = 0U;
            usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_TF, PIPE_XF);
        }

        pp_reg->HCHINTF = HCHINTF_ACK;
    } else if (intr_pp & HCHINTF_STALL) {
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_STALL, PIPE_STALL);
    } else if (intr_pp & HCHINTF_DTER) {
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_DTER, PIPE_DTGERR);
        pp_reg->HCHINTF = HCHINTF_NAK;
    } else if (intr_pp & HCHINTF_REQOVR) {
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_REQOVR, PIPE_REQOVR);
    } else if (intr_pp & HCHINTF_TF) {
        pp->err_count = 0U;
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_TF, PIPE_XF);
    } else if (intr_pp & HCHINTF_NAK) {
        pp->err_count = 0U;
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_NAK, PIPE_NAK);
    } else if (intr_pp & HCHINTF_USBER) {
        pp->err_count++;
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_USBER, PIPE_TRACERR);
    } else if (intr_pp & HCHINTF_NYET) {
        pp->err_count = 0U;
        usb_pp_halt (udev, (uint8_t)pp_num, HCHINTF_NYET, PIPE_NYET);
    } else if (intr_pp & HCHINTF_CH) {
        udev->regs.pr[pp_num]->HCHINTEN &= ~HCHINTEN_CHIE;

        switch (pp->pp_status) {
        case PIPE_XF:
            pp->urb_state = GD_URB_DONE;

            if ((uint8_t)USB_EPTYPE_BULK == ((pp_reg->HCHCTL & HCHCTL_EPTYPE) >> 18U)) {
                pp->data_toggle_out ^= 1U; 
            }
            break;

        case PIPE_NAK:

            if (GD_URB_PING == pp->urb_state) {
                (void)usb_pipe_ping (udev, (uint8_t)pp_num);
            } else {
                pp->urb_state = GD_URB_NOTREADY;
            }
            break;

        case PIPE_NYET:
            if (1U == udev->host.pipe[pp_num].ping) {
                (void)usb_pipe_ping (udev, (uint8_t)pp_num);
                pp->urb_state = GD_URB_PING;
            } 
            else {
                pp->urb_state = GD_URB_NOTREADY;
            }
            break;

        case PIPE_STALL:
            pp->urb_state = GD_URB_STALL;
            break;

        case PIPE_TRACERR:
            if (3U == pp->err_count) {
                pp->urb_state = GD_URB_ERROR;
                pp->err_count = 0U;
            }
            break;

        case PIPE_IDLE:
        case PIPE_HALTED:
        case PIPE_BBERR:
        case PIPE_REQOVR:
        case PIPE_DTGERR:
        default:
            break;
        }

        pp_reg->HCHINTF = HCHINTF_CH;
    } else {
        /* no operation */
    }

    return 1U;
}

/*!
    \brief      handle the RX FIFO non-empty interrupt
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     operation status
*/
#if defined (__ICCARM__)      /*!< IAR compiler */
    #pragma optimize = none
#endif /* __ICCARM */
static uint32_t usbh_int_rxfifonoempty (usb_core_driver *udev)
{
    uint32_t count = 0U;

    __IO uint8_t pp_num = 0U;
    __IO uint32_t rx_stat = 0U;

    /* disable the RX status queue level interrupt */
    udev->regs.gr->GINTEN &= ~GINTEN_RXFNEIE;

    rx_stat = udev->regs.gr->GRSTATP;
    pp_num = (uint8_t)(rx_stat & GRSTATRP_CNUM);

    switch ((rx_stat & GRSTATRP_RPCKST) >> 17U) {
    case GRXSTS_PKTSTS_IN:
        count = (rx_stat & GRSTATRP_BCOUNT) >> 4U;

        /* read the data into the host buffer. */
        if ((count > 0U) && (NULL != udev->host.pipe[pp_num].xfer_buf)) {
            (void)usb_rxfifo_read (&udev->regs, udev->host.pipe[pp_num].xfer_buf, (uint16_t)count);

            /* manage multiple transfer packet */
            udev->host.pipe[pp_num].xfer_buf += count;
            udev->host.pipe[pp_num].xfer_count += count;

            udev->host.backup_xfercount[pp_num] = udev->host.pipe[pp_num].xfer_count;

            if (udev->regs.pr[pp_num]->HCHLEN & HCHLEN_PCNT) {
                /* re-activate the channel when more packets are expected */
                __IO uint32_t pp_ctl = udev->regs.pr[pp_num]->HCHCTL;

                pp_ctl |= HCHCTL_CEN;
                pp_ctl &= ~HCHCTL_CDIS;

                udev->regs.pr[pp_num]->HCHCTL = pp_ctl;
            }
        }
        break;

    case GRXSTS_PKTSTS_IN_XFER_COMP:
        break;

    case GRXSTS_PKTSTS_DATA_TOGGLE_ERR:
        count = (rx_stat & GRSTATRP_BCOUNT) >> 4U;

        while (count > 0U) {
            rx_stat = udev->regs.gr->GRSTATP;
            count--;
        }
        break;

    case GRXSTS_PKTSTS_CH_HALTED:
        break;

    default:
        break;
    }

    /* enable the RX status queue level interrupt */
    udev->regs.gr->GINTEN |= GINTEN_RXFNEIE;

    return 1U;
}

/*!
    \brief      handle the TX FIFO empty interrupt
    \param[in]  udev: pointer to USB device instance
    \param[in]  pp_mode: pipe mode
    \param[out] none
    \retval     operation status
*/
#if defined (__ICCARM__)      /*!< IAR compiler */
    #pragma optimize = none
#endif /* __ICCARM */
static uint32_t usbh_int_txfifoempty (usb_core_driver *udev, usb_pipe_mode pp_mode)
{
    uint8_t pp_num = 0U;
    uint16_t word_count = 0U, len = 0U;
    __IO uint32_t *txfiforeg = 0U, txfifostate = 0U;

    if (PIPE_NON_PERIOD == pp_mode) {
        txfiforeg = &udev->regs.gr->HNPTFQSTAT;
    } else if (PIPE_PERIOD == pp_mode) {
        txfiforeg = &udev->regs.hr->HPTFQSTAT;
    } else {
        return 0U;
    }

    txfifostate = *txfiforeg;

    pp_num = (uint8_t)((txfifostate & TFQSTAT_CNUM) >> 27U);

    word_count = (uint16_t)(udev->host.pipe[pp_num].xfer_len + 3U) / 4U;

    while (((txfifostate & TFQSTAT_TXFS) >= word_count) && (0U != udev->host.pipe[pp_num].xfer_len)) {
        len = (uint16_t)(txfifostate & TFQSTAT_TXFS) * 4U;

        if (len > udev->host.pipe[pp_num].xfer_len) {
            /* last packet */
            len = (uint16_t)udev->host.pipe[pp_num].xfer_len;

            if (PIPE_NON_PERIOD == pp_mode) {
                udev->regs.gr->GINTEN &= ~GINTEN_NPTXFEIE;
            } else {
                udev->regs.gr->GINTEN &= ~GINTEN_PTXFEIE;
            }
        }

        word_count = (uint16_t)((udev->host.pipe[pp_num].xfer_len + 3U) / 4U);
        usb_txfifo_write (&udev->regs, udev->host.pipe[pp_num].xfer_buf, pp_num, len);

        udev->host.pipe[pp_num].xfer_buf += len;
        udev->host.pipe[pp_num].xfer_len -= len;
        udev->host.pipe[pp_num].xfer_count += len;

        txfifostate = *txfiforeg;
    }

    return 1U;
}
