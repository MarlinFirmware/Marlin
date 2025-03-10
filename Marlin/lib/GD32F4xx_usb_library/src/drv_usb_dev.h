/*!
    \file    drv_usb_dev.h
    \brief   USB device low level driver header file

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

#ifndef __DRV_USB_DEV_H
#define __DRV_USB_DEV_H

#include "gdusbd_conf.h"
#include "drv_usb_core.h"

#define EP_IN(x)                            ((uint8_t)(0x80U | (x)))    /*!< device IN endpoint */
#define EP_OUT(x)                           ((uint8_t)(x))              /*!< device OUT endpoint */

enum usb_ctl_status {
    USB_CTL_IDLE = 0U,                                                  /*!< USB control transfer idle state */
    USB_CTL_DATA_IN,                                                    /*!< USB control transfer data in state */
    USB_CTL_LAST_DATA_IN,                                               /*!< USB control transfer last data in state */
    USB_CTL_DATA_OUT,                                                   /*!< USB control transfer data out state */
    USB_CTL_LAST_DATA_OUT,                                              /*!< USB control transfer last data out state */
    USB_CTL_STATUS_IN,                                                  /*!< USB control transfer status in state*/
    USB_CTL_STATUS_OUT                                                  /*!< USB control transfer status out state */
};

/* static inline function definitions */

/*!
    \brief      configure the USB device to be disconnected
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     operation status
*/
__STATIC_INLINE void usb_dev_disconnect (usb_core_driver *udev)
{
    udev->regs.dr->DCTL |= DCTL_SD;
}

/*!
    \brief      configure the USB device to be connected
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     operation status
*/
__STATIC_INLINE void usb_dev_connect (usb_core_driver *udev)
{
    udev->regs.dr->DCTL &= ~DCTL_SD;
}

/*!
    \brief      set the USB device address
    \param[in]  udev: pointer to USB device
    \param[in]  dev_addr: device address for setting
    \param[out] none
    \retval     operation status
*/
__STATIC_INLINE void usb_devaddr_set (usb_core_driver *udev, uint8_t dev_addr)
{
    udev->regs.dr->DCFG &= ~DCFG_DAR;
    udev->regs.dr->DCFG |= (uint32_t)dev_addr << 4U;
}

/*!
    \brief      read device all OUT endpoint interrupt register
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     interrupt status
*/
__STATIC_INLINE uint32_t usb_oepintnum_read (usb_core_driver *udev)
{
    uint32_t value = udev->regs.dr->DAEPINT;

    value &= udev->regs.dr->DAEPINTEN;

    return (value & DAEPINT_OEPITB) >> 16U;
}

/*!
    \brief      read device OUT endpoint interrupt flag register
    \param[in]  udev: pointer to USB device
    \param[in]  ep_num: endpoint number
    \param[out] none
    \retval     interrupt status
*/
__STATIC_INLINE uint32_t usb_oepintr_read (usb_core_driver *udev, uint8_t ep_num)
{
    uint32_t value = udev->regs.er_out[ep_num]->DOEPINTF;

    value &= udev->regs.dr->DOEPINTEN;

    return value;
}

/*!
    \brief      read device all IN endpoint interrupt register
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     interrupt status
*/
__STATIC_INLINE uint32_t usb_iepintnum_read (usb_core_driver *udev)
{
    uint32_t value = udev->regs.dr->DAEPINT;

    value &= udev->regs.dr->DAEPINTEN;
    
    return value & DAEPINT_IEPITB;
}

/*!
    \brief      set remote wakeup signaling
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     none
*/
__STATIC_INLINE void usb_rwkup_set (usb_core_driver *udev)
{
    if (udev->dev.pm.dev_remote_wakeup) {
        /* enable remote wakeup signaling */
        udev->regs.dr->DCTL |= DCTL_RWKUP;
    }
}

/*!
    \brief      reset remote wakeup signaling
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     none
*/
__STATIC_INLINE void usb_rwkup_reset (usb_core_driver *udev)
{
    if (udev->dev.pm.dev_remote_wakeup) {
        /* disable remote wakeup signaling */
        udev->regs.dr->DCTL &= ~DCTL_RWKUP;
    }
}

/* function declarations */
/* initialize USB core registers for device mode */
usb_status usb_devcore_init (usb_core_driver *udev);
/* enable the USB device mode interrupts */
usb_status usb_devint_enable (usb_core_driver *udev);
/* active the USB endpoint 0 transaction */
usb_status usb_transc0_active (usb_core_driver *udev, usb_transc *transc);
/* active the USB transaction */
usb_status usb_transc_active (usb_core_driver *udev, usb_transc *transc);
/* deactivate the USB transaction */
usb_status usb_transc_deactivate (usb_core_driver *udev, usb_transc *transc);
/* configure USB transaction to start IN transfer */
usb_status usb_transc_inxfer (usb_core_driver *udev, usb_transc *transc);
/* configure USB transaction to start OUT transfer */
usb_status usb_transc_outxfer (usb_core_driver *udev, usb_transc *transc);
/* set the USB transaction STALL status */
usb_status usb_transc_stall (usb_core_driver *udev, usb_transc *transc);
/* clear the USB transaction STALL status */
usb_status usb_transc_clrstall (usb_core_driver *udev, usb_transc *transc);
/* read device IN endpoint interrupt flag register */
uint32_t usb_iepintr_read (usb_core_driver *udev, uint8_t ep_num);
/* configures OUT endpoint 0 to receive SETUP packets */
void usb_ctlep_startout (usb_core_driver *udev);
/* active remote wakeup signaling */
void usb_rwkup_active (usb_core_driver *udev);
/* active USB core clock */
void usb_clock_active (usb_core_driver *udev);
/* USB device suspend */
void usb_dev_suspend (usb_core_driver *udev);
/* stop the device and clean up FIFOs */
void usb_dev_stop (usb_core_driver *udev);

#endif /* __DRV_USB_DEV_H */
