/*!
    \file    printer_core.h
    \brief   the header file of USB printer device class core functions

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

#ifndef __PRINTER_CORE_H
#define __PRINTER_CORE_H

#include "usbd_enum.h"
#include "usb_ch9_std.h"

/* USB printing device class code */
#define USB_CLASS_PRINTER               0x07U

/* printing device subclass code */
#define USB_SUBCLASS_PRINTER            0x01U

/* printing device protocol code */
#define PROTOCOL_UNIDIRECTIONAL_ITF     0x01U
#define PROTOCOL_BI_DIRECTIONAL_ITF     0x02U
#define PROTOCOL_1284_4_ITF             0x03U
#define PROTOCOL_VENDOR                 0xFFU

#define DEVICE_ID_LEN                   103U

#define USB_PRINTER_CONFIG_DESC_LEN     32U

/* printing device specific-class request */
#define GET_DEVICE_ID                   0x00U
#define GET_PORT_STATUS                 0x01U
#define SOFT_RESET                      0x02U

#pragma pack(1)

/* USB configuration descriptor structure */
typedef struct
{
    usb_desc_config         config;
    usb_desc_itf            printer_itf;
    usb_desc_ep             printer_epin;
    usb_desc_ep             printer_epout;
} usb_printer_desc_config_set;

#pragma pack()

extern usb_desc printer_desc;
extern usb_class_core usbd_printer_cb;

#endif /* __PRINTER_CORE_H */
