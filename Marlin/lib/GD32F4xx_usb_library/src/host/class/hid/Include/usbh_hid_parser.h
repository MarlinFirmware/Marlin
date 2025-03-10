/*!
    \file    usbh_hid_core.h
    \brief   header file for the usbh_hid_core.c

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

#ifndef __USBH_HID_PARSER_H
#define __USBH_HID_PARSER_H

#include "usbh_hid_core.h"
#include "usbh_hid_usage.h"

typedef struct
{
    uint8_t     *data;
    uint32_t    size;
    uint8_t     shift;
    uint8_t     count;
    uint8_t     sign;
    uint32_t    logical_min;  /*min value device can return*/
    uint32_t    logical_max;  /*max value device can return*/
    uint32_t    physical_min; /*min vale read can report*/
    uint32_t    physical_max; /*max value read can report*/
    uint32_t    resolution;
} hid_report_item;

/* function declarations */
/* read a hid report item */
uint32_t hid_item_read (hid_report_item *ri, uint8_t ndx);
/* write a hid report item */
uint32_t hid_item_write (hid_report_item *ri, uint32_t value, uint8_t ndx);

#endif /* __USBH_HID_PARSER_H */
