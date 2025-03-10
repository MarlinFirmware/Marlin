/*!
    \file    usbd_transc.h
    \brief   USB transaction core functions prototype

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

#ifndef __USBD_TRANSC_H
#define __USBD_TRANSC_H

#include "gdusbd_core.h"

/* function declarations */
/* USB send data in the control transaction */
usbd_status usbd_ctl_send (usb_core_driver *udev);
/* USB receive data in control transaction */
usbd_status usbd_ctl_recev (usb_core_driver *udev);
/* USB send control transaction status */
usbd_status usbd_ctl_status_send (usb_core_driver *udev);
/* USB control receive status */
usbd_status usbd_ctl_status_recev (usb_core_driver *udev);
/* USB setup stage processing */
uint8_t usbd_setup_transc (usb_core_driver *udev);
/* data out stage processing */
uint8_t usbd_out_transc (usb_core_driver *udev, uint8_t ep_num);
/* data in stage processing */
uint8_t usbd_in_transc (usb_core_driver *udev, uint8_t ep_num);

#endif /* __USBD_TRANSC_H */
