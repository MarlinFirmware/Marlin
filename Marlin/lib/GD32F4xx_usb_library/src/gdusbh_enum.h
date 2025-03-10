/*!
    \file    usbh_enum.h
    \brief   USB host mode USB enumeration header file

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

#ifndef __USBH_ENUM_H
#define __USBH_ENUM_H

#include "gdusb_conf.h"
#include "gdusbh_core.h"

#if 0 //defined(USE_GD32)
/* function declarations */
/* configure USB control status parameters */
void usbh_ctlstate_config (usbh_host *uhost, uint8_t *buf, uint16_t len);
/* get device descriptor from the USB device */
usbh_status usbh_devdesc_get (usbh_host *uhost, uint8_t len);
/* get configuration descriptor from the USB device */
usbh_status usbh_cfgdesc_get (usbh_host *uhost, uint16_t len);
/* get string descriptor from the USB device */
usbh_status usbh_strdesc_get (usbh_host *uhost,uint8_t str_index, uint8_t *buf, uint16_t len);
/* set the address to the connected device */
usbh_status usbh_setaddress (usbh_host *uhost, uint8_t dev_addr);
/* set the configuration value to the connected device */
usbh_status usbh_setcfg (usbh_host *uhost, uint16_t config);
/* set the interface value to the connected device */
usbh_status usbh_setinterface (usbh_host *uhost, uint8_t itf_num, uint8_t alter_setting);
/* set or enable a specific device feature */
usbh_status usbh_setdevfeature (usbh_host *uhost, uint8_t feature_selector, uint16_t windex);
/* clear or disable a specific device feature */
usbh_status usbh_clrdevfeature (usbh_host *uhost, uint8_t feature_selector, uint16_t windex);
/* clear or disable a specific feature */
usbh_status usbh_clrfeature (usbh_host *uhost, uint8_t ep_addr, uint8_t pp_num); 
/* get the next descriptor header */
usb_desc_header *usbh_nextdesc_get (uint8_t *pbuf, uint16_t *ptr);
/* select an interface */
usbh_status usbh_interface_select (usb_dev_prop *udev, uint8_t interface);
/* find the interface index for a specific class */
uint8_t usbh_interface_find (usb_dev_prop *udev, uint8_t main_class, uint8_t sub_class, uint8_t protocol);
/* find the interface index for a specific class interface and alternate setting number */
uint8_t usbh_interfaceindex_find (usb_dev_prop *udev, uint8_t interface_number, uint8_t alt_settings);

#endif

#endif /* __USBH_ENUM_H */
