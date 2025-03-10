/*!
    \file    usbh_hid_mouse.h
    \brief   header file for the usbh_hid_mouse.c

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

#ifndef __USBH_HID_MOUSE_H
#define __USBH_HID_MOUSE_H

#include "usbh_hid_core.h"

typedef struct _hid_mouse_info
{
    uint8_t x;
    uint8_t y;
    uint8_t buttons[3];
} hid_mouse_info;

/* function declarations */
/* initialize mouse */
void usr_mouse_init (void);
/* process mouse data */
void usr_mouse_process_data (hid_mouse_info *data);
/* initialize mouse function */
usbh_status usbh_hid_mouse_init (usb_core_driver *udev, usbh_host *uhost);
/* get mouse information */
hid_mouse_info *usbh_hid_mouse_info_get (usb_core_driver *udev, usbh_host *uhost);
/* mouse machine */
void usbh_hid_mouse_machine (usb_core_driver *udev, usbh_host *uhost);

#endif /* __USBH_HID_MOUSE_H */
