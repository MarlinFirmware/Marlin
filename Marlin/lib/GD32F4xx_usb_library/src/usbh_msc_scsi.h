/*!
    \file    usbh_msc_scsi.h
    \brief   header file for usbh_msc_scsi.c

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

#ifndef __USBH_MSC_SCSI_H
#define __USBH_MSC_SCSI_H

#include "msc_scsi.h"
#include "gdusbh_enum.h"

/* capacity data */
typedef struct
{
    uint32_t block_nbr;
    uint16_t block_size;
} scsi_capacity;

/* inquiry data */
typedef struct
{
    uint8_t peripheral_qualifier;
    uint8_t device_type;
    uint8_t removable_media;
    uint8_t vendor_id[9];
    uint8_t product_id[17];
    uint8_t revision_id[5];
} scsi_std_inquiry_data;

typedef struct
{
    uint32_t msc_capacity;
    uint32_t msc_sense_key;
    uint16_t msc_page_len;
    uint8_t msc_write_protect;
}usbh_msc_parameter;

#define DESC_REQUEST_SENSE                   0x00U
#define ALLOCATION_LENGTH_REQUEST_SENSE      63U
#define XFER_LEN_MODE_SENSE6                 63U

#define MASK_MODE_SENSE_WRITE_PROTECT        0x80U
#define MODE_SENSE_PAGE_CONTROL_FIELD        0x00U
#define MODE_SENSE_PAGE_CODE                 0x3FU
#define DISK_WRITE_PROTECTED                 0x01U

/* function declarations */
/* send 'Inquiry' command to the device */
usbh_status usbh_msc_scsi_inquiry (usbh_host *uhost, uint8_t lun, scsi_std_inquiry_data *inquiry);
/* send 'Test unit ready' command to the device */
usbh_status usbh_msc_test_unitready (usbh_host *uhost, uint8_t lun);
/* send the read capacity command to the device */
usbh_status usbh_msc_read_capacity10 (usbh_host *uhost, uint8_t lun, scsi_capacity *capacity);
/* send the mode sense6 command to the device */
usbh_status usbh_msc_mode_sense6 (usbh_host *uhost, uint8_t lun);
/* send the Request Sense command to the device */
usbh_status usbh_msc_request_sense (usbh_host *uhost, uint8_t lun, msc_scsi_sense *sense_data);
/* send the write10 command to the device */
usbh_status usbh_msc_write10 (usbh_host *uhost,
                              uint8_t lun,
                              uint8_t *data_buf,
                              uint32_t addr,
                              uint32_t byte_num);
/* send the read10 command to the device */
usbh_status usbh_msc_read10 (usbh_host *uhost,
                             uint8_t lun,
                             uint8_t *data_buf,
                             uint32_t addr,
                             uint32_t byte_num);

#endif /* __USBH_MSC_SCSI_H */
