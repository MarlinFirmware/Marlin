/*!
    \file    drv_usb_host.h
    \brief   USB host mode low level driver header file

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

#ifndef __DRV_USB_HOST_H
#define __DRV_USB_HOST_H

#include "drv_usb_regs.h"
#include "usb_ch9_std.h"
#include "drv_usb_core.h"

typedef enum _usb_pipe_mode
{
    PIPE_PERIOD     = 0U,
    PIPE_NON_PERIOD = 1U
} usb_pipe_mode;

/*!
    \brief      get USB even frame
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     none
*/
__STATIC_INLINE uint8_t usb_frame_even (usb_core_driver *udev)
{
    return (uint8_t)!(udev->regs.hr->HFINFR & 0x01U);
}

/*!
    \brief      configure USB clock of PHY
    \param[in]  udev: pointer to USB device
    \param[in]  clock: PHY clock
    \param[out] none
    \retval     none
*/
__STATIC_INLINE void usb_phyclock_config (usb_core_driver *udev, uint8_t clock)
{
    udev->regs.hr->HCTL &= ~HCTL_CLKSEL;
    udev->regs.hr->HCTL |= clock;
}

/*!
    \brief      read USB port
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     port status
*/
__STATIC_INLINE uint32_t usb_port_read (usb_core_driver *udev)
{
    return *udev->regs.HPCS & ~(HPCS_PE | HPCS_PCD | HPCS_PEDC);
}

/*!
    \brief      get USB current speed
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     USB current speed
*/
__STATIC_INLINE uint32_t usb_curspeed_get (usb_core_driver *udev)
{
    return *udev->regs.HPCS & HPCS_PS;
}

/*!
    \brief      get USB current frame
    \param[in]  udev: pointer to USB device
    \param[out] none
    \retval     USB current frame
*/
__STATIC_INLINE uint32_t usb_curframe_get (usb_core_driver *udev)
{
    return (udev->regs.hr->HFINFR & 0xFFFFU);
}

/* function declarations */
/* initializes USB core for host mode */
usb_status usb_host_init (usb_core_driver *udev);
/* control the VBUS to power */
void usb_portvbus_switch (usb_core_driver *udev, uint8_t state);
/* reset host port */
uint32_t usb_port_reset (usb_core_driver *udev);
/* initialize host pipe */
usb_status usb_pipe_init (usb_core_driver *udev, uint8_t pipe_num);
/* prepare host pipe for transferring packets */
usb_status usb_pipe_xfer (usb_core_driver *udev, uint8_t pipe_num);
/* halt host pipe */
usb_status usb_pipe_halt (usb_core_driver *udev, uint8_t pipe_num);
/* configure host pipe to do ping operation */
usb_status usb_pipe_ping (usb_core_driver *udev, uint8_t pipe_num);
/* stop the USB host and clean up FIFO */
void usb_host_stop (usb_core_driver *udev);

#endif  /* __DRV_USB_HOST_H */
