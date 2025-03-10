/*!
    \file    usbd_msc_bbb.h
    \brief   the header file of the usbd_msc_bot.c file

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

#ifndef __USBD_MSC_BBB_H
#define __USBD_MSC_BBB_H

#include "gd32_usbd_core.h"
#include "msc_bbb.h"
#include "usbd_msc_mem.h"
#include "usbd_msc_scsi.h"

/* MSC BBB state */
enum msc_bbb_state {
    BBB_IDLE = 0U,          /*!< idle state  */
    BBB_DATA_OUT,           /*!< data OUT state */
    BBB_DATA_IN,            /*!< data IN state */
    BBB_LAST_DATA_IN,       /*!< last data IN state */
    BBB_SEND_DATA           /*!< send immediate data state */
};

/* MSC BBB status */
enum msc_bbb_status {
    BBB_STATUS_NORMAL = 0U, /*!< normal status */
    BBB_STATUS_RECOVERY,    /*!< recovery status*/
    BBB_STATUS_ERROR        /*!< error status */
};

typedef struct
{
    uint8_t bbb_data[MSC_MEDIA_PACKET_SIZE];

    uint8_t max_lun;
    uint8_t bbb_state;
    uint8_t bbb_status;

    uint32_t bbb_datalen;

    msc_bbb_cbw bbb_cbw;
    msc_bbb_csw bbb_csw;

    uint8_t scsi_sense_head;
    uint8_t scsi_sense_tail;

    uint32_t scsi_blk_size[MEM_LUN_NUM];
    uint32_t scsi_blk_nbr[MEM_LUN_NUM];

    uint32_t scsi_blk_addr;
    uint32_t scsi_blk_len;
    uint32_t scsi_disk_pop;

    msc_scsi_sense scsi_sense[SENSE_LIST_DEEPTH];
} usbd_msc_handler;

/* function declarations */
/* initialize the bbb process */
void msc_bbb_init (usb_core_driver *udev);
/* reset the BBB machine */
void msc_bbb_reset (usb_core_driver *udev);
/* deinitialize the BBB machine */
void msc_bbb_deinit (usb_core_driver *udev);
/* handle BBB data IN stage */
void msc_bbb_data_in (usb_core_driver *udev, uint8_t ep_num);
/* handle BBB data OUT stage */
void msc_bbb_data_out (usb_core_driver *udev, uint8_t ep_num);
/* send the CSW(command status wrapper) */
void msc_bbb_csw_send (usb_core_driver *udev, uint8_t csw_status);
/* complete the clear feature request */
void msc_bbb_clrfeature (usb_core_driver *udev, uint8_t ep_num);

#endif /* __USBD_MSC_BBB_H */
