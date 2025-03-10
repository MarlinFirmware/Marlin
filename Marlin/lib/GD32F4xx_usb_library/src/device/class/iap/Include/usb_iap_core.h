/*!
    \file    usb_iap_core.h
    \brief   the header file of IAP driver

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

#ifndef __USB_IAP_CORE_H
#define __USB_IAP_CORE_H

#include "usbd_enum.h"
#include "usb_hid.h"

#define USB_SERIAL_STRING_SIZE              0x06U

#ifdef USE_USB_FS
    #define USB_DESC_LEN_IAP_REPORT         35U
#elif defined(USE_USB_HS)
    #define USB_DESC_LEN_IAP_REPORT         36U
#else
    #error "please select 'USE_USB_FS' or 'USE_USB_HS'"
#endif

#define USB_DESC_LEN_IAP_CONFIG_SET         41U

/* special commands with download request */
#define IAP_OPTION_BYTE1                    0x01U
#define IAP_ERASE                           0x02U
#define IAP_DNLOAD                          0x03U
#define IAP_LEAVE                           0x04U
#define IAP_GETBIN_ADDRESS                  0x05U
#define IAP_OPTION_BYTE2                    0x06U

typedef struct
{
    uint8_t option_byte[IAP_IN_PACKET];

    /* state machine variables */
    uint8_t dev_status[IAP_IN_PACKET];
    uint8_t bin_addr[IAP_IN_PACKET];

    uint8_t report_buf[IAP_OUT_PACKET + 1U];

    uint8_t reportID;
    uint8_t flag;

    uint32_t protocol;
    uint32_t idlestate;

    uint16_t transfer_times;
    uint16_t page_count;
    uint16_t lps;           /* last packet size */
    uint32_t file_length;
    uint32_t base_address;
} usbd_iap_handler;

typedef void (*app_func) (void);

extern usb_desc iap_desc;
extern usb_class_core iap_class;

/* function declarations */
/* send IAP report */
uint8_t iap_report_send (usb_dev *udev, uint8_t *report, uint32_t len);

#endif /* __USB_IAP_CORE_H */
