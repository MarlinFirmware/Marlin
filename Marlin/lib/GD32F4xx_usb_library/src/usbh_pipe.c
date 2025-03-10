/*!
    \file    usbh_pipe.c
    \brief   USB host mode pipe operation driver

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

#include "gdusbh_pipe.h"

/* local function prototypes ('static') */
static uint16_t usbh_freepipe_get (usb_core_driver *udev);

/*!
    \brief      create a pipe
    \param[in]  udev: pointer to USB core instance
    \param[in]  dev: USB device
    \param[in]  pp_num: pipe number
    \param[in]  ep_type: endpoint type
    \param[in]  ep_mpl: endpoint max packet length
    \param[out] none
    \retval     operation status
*/
uint8_t usbh_pipe_create (usb_core_driver *udev,
                          usb_dev_prop *dev,
                          uint8_t  pp_num,
                          uint8_t  ep_type,
                          uint16_t ep_mpl)
{
    usb_pipe *pp = &udev->host.pipe[pp_num];

    pp->dev_addr = dev->addr;
    pp->dev_speed = dev->speed;
    pp->ep.type = ep_type;
    pp->ep.mps = ep_mpl;
    pp->ping = (uint8_t)(dev->speed == PORT_SPEED_HIGH);

    usb_pipe_init (udev, pp_num);

    return HC_OK;
}

/*!
    \brief      modify a pipe
    \param[in]  udev: pointer to USB core instance
    \param[in]  pp_num: pipe number
    \param[in]  dev_addr: device address
    \param[in]  dev_speed: device speed
    \param[in]  ep_mpl: endpoint max packet length
    \param[out] none
    \retval     operation status
*/
uint8_t usbh_pipe_update (usb_core_driver *udev,
                          uint8_t  pp_num,
                          uint8_t  dev_addr,
                          uint32_t dev_speed,
                          uint16_t ep_mpl)
{
    usb_pipe *pp = &udev->host.pipe[pp_num];

    if ((pp->dev_addr != dev_addr) && (dev_addr)) {
        pp->dev_addr = dev_addr;
    }

    if ((pp->dev_speed != dev_speed) && (dev_speed)) {
        pp->dev_speed = dev_speed;
    }

    if ((pp->ep.mps != ep_mpl) && (ep_mpl)) {
        pp->ep.mps = ep_mpl; 
    }

    usb_pipe_init (udev, pp_num);

    return HC_OK;
}

/*!
    \brief      allocate a new pipe
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
    \param[out] none
    \retval     operation status
*/
uint8_t usbh_pipe_allocate (usb_core_driver *udev, uint8_t ep_addr)
{
    uint16_t pp_num = usbh_freepipe_get (udev);

    if (HC_ERROR != pp_num) {
        udev->host.pipe[pp_num].in_used = 1U;
        udev->host.pipe[pp_num].ep.dir = EP_DIR(ep_addr);
        udev->host.pipe[pp_num].ep.num = EP_ID(ep_addr);
    }

    return (uint8_t)pp_num;
}

/*!
    \brief      free a pipe
    \param[in]  udev: pointer to USB core instance
    \param[in]  pp_num: pipe number
    \param[out] none
    \retval     operation status
*/
uint8_t usbh_pipe_free (usb_core_driver *udev, uint8_t pp_num)
{
    if (pp_num < HC_MAX) {
        udev->host.pipe[pp_num].in_used = 0U;
    }

    return USBH_OK;
}

/*!
    \brief      delete all USB host pipe
    \param[in]  udev: pointer to USB core instance
    \param[out] none
    \retval     operation status
*/
uint8_t usbh_pipe_delete (usb_core_driver *udev)
{
    uint8_t pp_num = 0U;

    for (pp_num = 2U; pp_num < HC_MAX; pp_num++) {
        udev->host.pipe[pp_num] = (usb_pipe) {0};
    }

    return USBH_OK;
}

/*!
    \brief      get a free pipe number for allocation
    \param[in]  udev: pointer to USB core instance
    \param[out] none
    \retval     operation status
*/
static uint16_t usbh_freepipe_get (usb_core_driver *udev)
{
    uint8_t pp_num = 0U;

    for (pp_num = 0U; pp_num < HC_MAX; pp_num++) {
        if (0U == udev->host.pipe[pp_num].in_used) {
            return (uint16_t)pp_num;
        }
    }

    return HC_ERROR;
}
