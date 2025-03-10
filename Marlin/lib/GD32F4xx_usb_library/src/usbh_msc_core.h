/*!
    \file    usbh_core.h
    \brief   header file for the usbh_core.c

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

#ifndef __USBH_MSC_CORE_H
#define __USBH_MSC_CORE_H

#include "gdusb_msc.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bbb.h"

#define MSC_MAX_SUPPORTED_LUN                   2U

typedef enum
{
    MSC_INIT = 0U,
    MSC_IDLE,
    MSC_TEST_UNIT_READY,
    MSC_READ_CAPACITY10,
    MSC_READ_INQUIRY,
    MSC_REQUEST_SENSE,
    MSC_READ,
    MSC_WRITE,
    MSC_UNRECOVERED_ERROR,
    MSC_PERIODIC_CHECK,
} msc_state;

typedef enum
{
    MSC_OK,
    MSC_NOT_READY,
    MSC_ERROR,
} msc_error;

typedef enum
{
    MSC_REQ_IDLE = 0U,
    MSC_REQ_RESET,
    MSC_REQ_GET_MAX_LUN,
    MSC_REQ_ERROR,
} msc_req_state;

/* Structure for LUN */
typedef struct
{
    msc_state               state;
    msc_error               error;
    msc_scsi_sense          sense;
    scsi_capacity           capacity;
    scsi_std_inquiry_data   inquiry;
    usbh_status             prev_ready_state;
    uint8_t                 state_changed;
} msc_lun;

/* structure for MSC process */
typedef struct _msc_process
{
    uint8_t         pipe_in;
    uint8_t         pipe_out;
    uint8_t         ep_in;
    uint8_t         ep_out;
    uint16_t        ep_size_in;
    uint16_t        ep_size_out;
    uint8_t         cur_lun;
    uint16_t        rw_lun;
    uint32_t        max_lun;
    msc_state       state;
    msc_error       error;
    msc_req_state   req_state;
    msc_req_state   prev_req_state;
    bot_handle      bot;
    msc_lun         unit[MSC_MAX_SUPPORTED_LUN];
    uint32_t        timer;
} usbh_msc_handler;

extern usbh_class usbh_msc;

/* function declarations */
/* get msc logic unit information */
usbh_status usbh_msc_lun_info_get (usbh_host *uhost, uint8_t lun, msc_lun *info);
/* msc read interface */
usbh_status usbh_msc_read (usbh_host *uhost,
                           uint8_t lun,
                           uint32_t address,
                           uint8_t *pbuf,
                           uint32_t length);
/* msc write interface */
usbh_status usbh_msc_write (usbh_host *uhost,
                            uint8_t lun,
                            uint32_t address,
                            uint8_t *pbuf,
                            uint32_t length);

#endif  /* __USBH_MSC_CORE_H */
