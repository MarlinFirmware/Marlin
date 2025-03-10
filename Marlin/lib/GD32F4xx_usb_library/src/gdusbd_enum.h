/*!
    \file    usbd_enum.h
    \brief   USB enumeration definitions

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

#ifndef __USBD_ENUM_H
#define __USBD_ENUM_H

#include "gdusbd_core.h"
#include "gdusbd_conf.h"
#include <wchar.h>

#ifndef NULL
    #define NULL                0U
#endif

typedef enum _usb_reqsta {
    REQ_SUPP     = 0x0U,                   /* request support */
    REQ_NOTSUPP  = 0x1U,                   /* request not support */
} usb_reqsta;

/* string descriptor index */
enum _str_index
{
    STR_IDX_LANGID                = 0x0U,  /* language ID string index */
    STR_IDX_MFC                   = 0x1U,  /* manufacturer string index */
    STR_IDX_PRODUCT               = 0x2U,  /* product string index */
    STR_IDX_SERIAL                = 0x3U,  /* serial string index */
    STR_IDX_CONFIG                = 0x4U,  /* configuration string index */
    STR_IDX_ITF                   = 0x5U,  /* interface string index */
#ifndef WINUSB_EXEMPT_DRIVER
    STR_IDX_MAX                   = 0x6U,  /* string maximum index */
#else
    STR_IDX_MAX                   = 0xEFU, /* string maximum index */
#endif /* WINUSB_EXEMPT_DRIVER */
};

typedef enum _usb_pwrsta {
    USB_PWRSTA_SELF_POWERED       = 0x1U,  /* USB is in self powered status */
    USB_PWRSTA_REMOTE_WAKEUP      = 0x2U,  /* USB is in remote wakeup status */
} usb_pwrsta;

typedef enum _usb_feature
{
    USB_FEATURE_EP_HALT           = 0x0U,  /* USB has endpoint halt feature */
    USB_FEATURE_REMOTE_WAKEUP     = 0x1U,  /* USB has endpoint remote wakeup feature */
    USB_FEATURE_TEST_MODE         = 0x2U,  /* USB has endpoint test mode feature */
} usb_feature;

#define ENG_LANGID                0x0409U  /* english language ID */
#define CHN_LANGID                0x0804U  /* chinese language ID */

/* USB device exported macros */
#define CTL_EP(ep)                (((ep) == 0x00U) || ((ep) == 0x80U))

#define DEVICE_ID1                (0x1FFF7A10U)  /* device ID1 */
#define DEVICE_ID2                (0x1FFF7A14U)  /* device ID2 */
#define DEVICE_ID3                (0x1FFF7A18U)  /* device ID3 */

#define DEVICE_ID                 (0x40023D00U)

/* function declarations */
/* handle USB standard device request */
usb_reqsta usbd_standard_request (usb_core_driver *udev, usb_req *req);
/* handle USB device class request */
usb_reqsta usbd_class_request (usb_core_driver *udev, usb_req *req);
/* handle USB vendor request */
usb_reqsta usbd_vendor_request (usb_core_driver *udev, usb_req *req);
/* handle USB enumeration error */
void usbd_enum_error (usb_core_driver *udev, usb_req *req);
/* convert hex 32bits value into unicode char */
void int_to_unicode (uint32_t value, uint8_t *pbuf, uint8_t len);
/* get serial string */
void serial_string_get (uint16_t *unicode_str);

#endif /* __USBD_ENUM_H */
