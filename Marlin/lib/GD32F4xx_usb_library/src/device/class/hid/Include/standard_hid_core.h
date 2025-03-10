/*!
    \file    standard_hid_core.h
    \brief   definitions for HID core

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

#ifndef __STANDARD_HID_CORE_H
#define __STANDARD_HID_CORE_H

#include "usbd_enum.h"
#include "usb_hid.h"

#define USB_HID_CONFIG_DESC_LEN          0x22U
#define USB_HID_REPORT_DESC_LEN          0x29U

#define NO_CMD                           0xFFU

typedef struct {
    uint32_t protocol;
    uint32_t idle_state;

    uint8_t data[HID_IN_PACKET];
    __IO uint8_t prev_transfer_complete;
} standard_hid_handler;

typedef struct {
    void (*hid_itf_config) (void);
    void (*hid_itf_data_process) (usb_dev *udev);
} hid_fop_handler;

extern usb_desc hid_desc;
extern usb_class_core usbd_hid_cb;

/* function declarations */
/* register HID interface operation functions */
uint8_t hid_itfop_register (usb_dev *udev, hid_fop_handler *hid_fop);
/* send keyboard report */
uint8_t hid_report_send (usb_dev *udev, uint8_t *report, uint32_t len);

#endif /* __STANDARD_HID_CORE_H */
