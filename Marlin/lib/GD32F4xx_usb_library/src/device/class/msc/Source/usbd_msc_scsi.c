/*!
    \file    usbd_msc_scsi.c
    \brief   USB SCSI layer functions

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
#include "usbd_enum.h"
#include "usbd_msc_bbb.h"
#include "usbd_msc_scsi.h"
#include "usbd_msc_data.h"

/* local function prototypes ('static') */
static int8_t scsi_test_unit_ready      (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_inquiry              (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_read_format_capacity (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_read_capacity10      (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_request_sense        (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_mode_sense6          (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_toc_cmd_read         (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_mode_sense10         (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_write10              (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_read10               (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static int8_t scsi_verify10             (usb_core_driver *udev, uint8_t lun, uint8_t *params);

static int8_t scsi_process_read         (usb_core_driver *udev, uint8_t lun);
static int8_t scsi_process_write        (usb_core_driver *udev, uint8_t lun);

static inline int8_t scsi_check_address_range  (usb_core_driver *udev, uint8_t lun, uint32_t blk_offset, uint16_t blk_nbr);
static inline int8_t scsi_format_cmd           (usb_core_driver *udev, uint8_t lun);
static inline int8_t scsi_start_stop_unit      (usb_core_driver *udev, uint8_t lun, uint8_t *params);
static inline int8_t scsi_allow_medium_removal (usb_core_driver *udev, uint8_t lun, uint8_t *params);

/*!
    \brief      process SCSI commands
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
int8_t scsi_process_cmd(usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    switch (params[0]) {
    case SCSI_TEST_UNIT_READY:
        return scsi_test_unit_ready (udev, lun, params);

    case SCSI_REQUEST_SENSE:
        return scsi_request_sense (udev, lun, params);

    case SCSI_INQUIRY:
        return scsi_inquiry (udev, lun, params);

    case SCSI_START_STOP_UNIT:
        return scsi_start_stop_unit (udev, lun, params);

    case SCSI_ALLOW_MEDIUM_REMOVAL:
        return scsi_allow_medium_removal (udev, lun, params);

    case SCSI_MODE_SENSE6:
        return scsi_mode_sense6 (udev, lun, params);

    case SCSI_MODE_SENSE10:
        return scsi_mode_sense10 (udev, lun, params);

    case SCSI_READ_FORMAT_CAPACITIES:
        return scsi_read_format_capacity (udev, lun, params);

    case SCSI_READ_CAPACITY10:
        return scsi_read_capacity10 (udev, lun, params);

    case SCSI_READ10:
        return scsi_read10 (udev, lun, params); 

    case SCSI_WRITE10:
        return scsi_write10 (udev, lun, params);

    case SCSI_VERIFY10:
        return scsi_verify10 (udev, lun, params);

    case SCSI_FORMAT_UNIT:
        return scsi_format_cmd (udev, lun);

    case SCSI_READ_TOC_DATA:
        return scsi_toc_cmd_read (udev, lun, params);

    default:
        scsi_sense_code (udev, lun, ILLEGAL_REQUEST, INVALID_CDB);
        return -1;
    }
}

/*!
    \brief      load the last error code in the error list
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  skey: sense key
    \param[in]  asc: additional sense key
    \param[out] none
    \retval     none
*/
void scsi_sense_code (usb_core_driver *udev, uint8_t lun, uint8_t skey, uint8_t asc)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->scsi_sense[msc->scsi_sense_tail].SenseKey = skey;
    msc->scsi_sense[msc->scsi_sense_tail].ASC = asc << 8U;
    msc->scsi_sense_tail++;

    if (SENSE_LIST_DEEPTH == msc->scsi_sense_tail) {
        msc->scsi_sense_tail = 0U;
    }
}

/*!
    \brief      process SCSI Test Unit Ready command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_test_unit_ready (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    /* case 9 : Hi > D0 */
    if (0U != msc->bbb_cbw.dCBWDataTransferLength) {
        scsi_sense_code (udev, msc->bbb_cbw.bCBWLUN, ILLEGAL_REQUEST, INVALID_CDB);

        return -1;
    }

    if (0 != usbd_mem_fops->mem_ready(lun)) {
        scsi_sense_code(udev, lun, NOT_READY, MEDIUM_NOT_PRESENT);

        return -1;
    }
    
    if (1U == msc->scsi_disk_pop) {
        usbd_disconnect (udev);
    }

    msc->bbb_datalen = 0U;

    return 0;
}

/*!
    \brief      process Inquiry command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_inquiry (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    uint8_t *page = NULL;
    uint16_t len = 0U;

    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    if (params[1] & 0x01U) {
        page = (uint8_t *)msc_page00_inquiry_data;

        len = INQUIRY_PAGE00_LENGTH;
    } else {
        page = (uint8_t *)usbd_mem_fops->mem_inquiry_data[lun];

        len = (uint16_t)(page[4] + 5U);

        if (params[4] <= len) {
            len = params[4];
        }
    }

    msc->bbb_datalen = len;

    while (len) {
        len--;
        msc->bbb_data[len] = page[len];
    }

    return 0;
}

/*!
    \brief      process Read Capacity 10 command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_read_capacity10 (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    uint32_t blk_num = usbd_mem_fops->mem_block_len[lun] - 1U;
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->scsi_blk_nbr[lun] = usbd_mem_fops->mem_block_len[lun];
    msc->scsi_blk_size[lun] = usbd_mem_fops->mem_block_size[lun];

    msc->bbb_data[0] = (uint8_t)(blk_num >> 24U);
    msc->bbb_data[1] = (uint8_t)(blk_num >> 16U);
    msc->bbb_data[2] = (uint8_t)(blk_num >> 8U);
    msc->bbb_data[3] = (uint8_t)(blk_num);

    msc->bbb_data[4] = (uint8_t)(msc->scsi_blk_size[lun] >> 24U);
    msc->bbb_data[5] = (uint8_t)(msc->scsi_blk_size[lun] >> 16U);
    msc->bbb_data[6] = (uint8_t)(msc->scsi_blk_size[lun] >> 8U);
    msc->bbb_data[7] = (uint8_t)(msc->scsi_blk_size[lun]);

    msc->bbb_datalen = 8U;

    return 0;
}

/*!
    \brief      process Read Format Capacity command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_read_format_capacity (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    uint16_t i = 0U;
    uint32_t blk_size = usbd_mem_fops->mem_block_size[lun];
    uint32_t blk_num = usbd_mem_fops->mem_block_len[lun];
    uint32_t blk_nbr = blk_num - 1U;

    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    for (i = 0U; i < 12U; i++) {
        msc->bbb_data[i] = 0U;
    }

    msc->bbb_data[3] = 0x08U;
    msc->bbb_data[4] = (uint8_t)(blk_nbr >> 24U);
    msc->bbb_data[5] = (uint8_t)(blk_nbr >> 16U);
    msc->bbb_data[6] = (uint8_t)(blk_nbr >>  8U);
    msc->bbb_data[7] = (uint8_t)(blk_nbr);

    msc->bbb_data[8] = 0x02U;
    msc->bbb_data[9] = (uint8_t)(blk_size >> 16U);
    msc->bbb_data[10] = (uint8_t)(blk_size >> 8U);
    msc->bbb_data[11] = (uint8_t)(blk_size);

    msc->bbb_datalen = 12U;

    return 0;
}

/*!
    \brief      process Mode Sense6 command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_mode_sense6 (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    uint16_t len = 8U;
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->bbb_datalen = len;

    while (len) {
        len--;
        msc->bbb_data[len] = msc_mode_sense6_data[len];
    }

    return 0;
}

/*!
    \brief      process Mode Sense10 command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_mode_sense10 (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    uint16_t len = 8U;
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->bbb_datalen = len;

    while (len) {
        len--;
        msc->bbb_data[len] = msc_mode_sense10_data[len];
    }

    return 0;
}

/*!
    \brief      process Request Sense command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_request_sense (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    uint8_t i = 0U;
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    for (i = 0U; i < REQUEST_SENSE_DATA_LEN; i++) {
        msc->bbb_data[i] = 0U;
    }

    msc->bbb_data[0] = 0x70U;
    msc->bbb_data[7] = REQUEST_SENSE_DATA_LEN - 6U;

    if ((msc->scsi_sense_head != msc->scsi_sense_tail)) {
        msc->bbb_data[2] = msc->scsi_sense[msc->scsi_sense_head].SenseKey;
        msc->bbb_data[12] = msc->scsi_sense[msc->scsi_sense_head].ASCQ;
        msc->bbb_data[13] = msc->scsi_sense[msc->scsi_sense_head].ASC;
        msc->scsi_sense_head++;

        if (msc->scsi_sense_head == SENSE_LIST_DEEPTH) {
            msc->scsi_sense_head = 0U;
        }
    }

    msc->bbb_datalen = USB_MIN(REQUEST_SENSE_DATA_LEN, params[4]);

    return 0;
}

/*!
    \brief      process Start Stop Unit command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static inline int8_t scsi_start_stop_unit (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->bbb_datalen = 0U;
    msc->scsi_disk_pop = 1U;

    return 0;
}

/*!
    \brief      process Allow Medium Removal command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static inline int8_t scsi_allow_medium_removal (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->bbb_datalen = 0U;

    return 0;
}

/*!
    \brief      process Read10 command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_read10 (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    if (msc->bbb_state == BBB_IDLE) {
        /* direction is from device to host */
        if (0x80U != (msc->bbb_cbw.bmCBWFlags & 0x80U)) {
            scsi_sense_code (udev, msc->bbb_cbw.bCBWLUN, ILLEGAL_REQUEST, INVALID_CDB);

            return -1;
        }

        if (0 != usbd_mem_fops->mem_ready(lun)) {
            scsi_sense_code (udev, lun, NOT_READY, MEDIUM_NOT_PRESENT);

            return -1;
        }

        msc->scsi_blk_addr = (params[2] << 24U) | (params[3] << 16U) | \
                             (params[4] << 8U) |  params[5];

        msc->scsi_blk_len = (params[7] << 8U) | params[8];

        if (scsi_check_address_range (udev, lun, msc->scsi_blk_addr, (uint16_t)msc->scsi_blk_len) < 0) {
            return -1; /* error */
        }

        msc->bbb_state = BBB_DATA_IN;

        msc->scsi_blk_addr *= msc->scsi_blk_size[lun];
        msc->scsi_blk_len  *= msc->scsi_blk_size[lun];

        /* cases 4,5 : Hi <> Dn */
        if (msc->bbb_cbw.dCBWDataTransferLength != msc->scsi_blk_len) {
            scsi_sense_code (udev, msc->bbb_cbw.bCBWLUN, ILLEGAL_REQUEST, INVALID_CDB);

            return -1;
        }
    }

    msc->bbb_datalen = MSC_MEDIA_PACKET_SIZE;

    return scsi_process_read (udev, lun);
}

/*!
    \brief      process Write10 command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_write10 (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    if (BBB_IDLE == msc->bbb_state) {
        /* case 8 : Hi <> Do */
        if (0x80U == (msc->bbb_cbw.bmCBWFlags & 0x80U)) {
            scsi_sense_code (udev, msc->bbb_cbw.bCBWLUN, ILLEGAL_REQUEST, INVALID_CDB);

            return -1;
        }

        /* check whether media is ready */
        if (0 != usbd_mem_fops->mem_ready(lun)) {
            scsi_sense_code (udev, lun, NOT_READY, MEDIUM_NOT_PRESENT);

            return -1;
        }

        /* check if media is write-protected */
        if (0 != usbd_mem_fops->mem_protected(lun)) {
            scsi_sense_code (udev, lun, NOT_READY, WRITE_PROTECTED);

            return -1;
        }

        msc->scsi_blk_addr = (params[2] << 24U) | (params[3] << 16U) | \
                             (params[4] << 8U) |  params[5];

        msc->scsi_blk_len = (params[7] << 8U) | params[8];

        /* check if LBA address is in the right range */
        if (scsi_check_address_range (udev, lun, msc->scsi_blk_addr, (uint16_t)msc->scsi_blk_len) < 0) {
            return -1; /* error */
        }

        msc->scsi_blk_addr *= msc->scsi_blk_size[lun];
        msc->scsi_blk_len  *= msc->scsi_blk_size[lun];

        /* cases 3,11,13 : Hn,Ho <> D0 */
        if (msc->bbb_cbw.dCBWDataTransferLength != msc->scsi_blk_len) {
            scsi_sense_code (udev, msc->bbb_cbw.bCBWLUN, ILLEGAL_REQUEST, INVALID_CDB);

            return -1;
        }

        /* prepare endpoint to receive first data packet */
        msc->bbb_state = BBB_DATA_OUT;

        usbd_ep_recev (udev, 
                       MSC_OUT_EP, 
                       msc->bbb_data, 
                       USB_MIN (msc->scsi_blk_len, MSC_MEDIA_PACKET_SIZE));
    } else { /* write process ongoing */
        return scsi_process_write (udev, lun);
    }

    return 0;
}

/*!
    \brief      process Verify10 command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_verify10 (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    if (0x02U == (params[1] & 0x02U)) {
        scsi_sense_code (udev, lun, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);

        return -1; /* error, verify mode not supported*/
    }

    if (scsi_check_address_range (udev, lun, msc->scsi_blk_addr, (uint16_t)msc->scsi_blk_len) < 0) {
        return -1; /* error */
    }

    msc->bbb_datalen = 0U;

    return 0;
}

/*!
    \brief      check address range
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  blk_offset: block offset
    \param[in]  blk_nbr: number of block to be processed
    \param[out] none
    \retval     status
*/
static inline int8_t scsi_check_address_range (usb_core_driver *udev, uint8_t lun, uint32_t blk_offset, uint16_t blk_nbr)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    if ((blk_offset + blk_nbr) > msc->scsi_blk_nbr[lun]) {
        scsi_sense_code (udev, lun, ILLEGAL_REQUEST, ADDRESS_OUT_OF_RANGE);

        return -1;
    }

    return 0;
}

/*!
    \brief      handle read process
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[out] none
    \retval     status
*/
static int8_t scsi_process_read (usb_core_driver *udev, uint8_t lun)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    uint32_t len = USB_MIN(msc->scsi_blk_len, MSC_MEDIA_PACKET_SIZE);

    if (usbd_mem_fops->mem_read(lun,
                                msc->bbb_data, 
                                msc->scsi_blk_addr, 
                                (uint16_t)(len / msc->scsi_blk_size[lun])) < 0) {
        scsi_sense_code(udev, lun, HARDWARE_ERROR, UNRECOVERED_READ_ERROR);

        return -1; 
    }

    usbd_ep_send (udev, MSC_IN_EP, msc->bbb_data, len);

    msc->scsi_blk_addr += len;
    msc->scsi_blk_len  -= len;

    /* case 6 : Hi = Di */
    msc->bbb_csw.dCSWDataResidue -= len;

    if (0U == msc->scsi_blk_len) {
        msc->bbb_state = BBB_LAST_DATA_IN;
    }

    return 0;
}

/*!
    \brief      handle write process
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[out] none
    \retval     status
*/
static int8_t scsi_process_write (usb_core_driver *udev, uint8_t lun)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    uint32_t len = USB_MIN(msc->scsi_blk_len, MSC_MEDIA_PACKET_SIZE);

    if (usbd_mem_fops->mem_write (lun,
                                  msc->bbb_data, 
                                  msc->scsi_blk_addr, 
                                  (uint16_t)(len / msc->scsi_blk_size[lun])) < 0) {
        scsi_sense_code(udev, lun, HARDWARE_ERROR, WRITE_FAULT);

        return -1;
    }

    msc->scsi_blk_addr += len;
    msc->scsi_blk_len  -= len;

    /* case 12 : Ho = Do */
    msc->bbb_csw.dCSWDataResidue -= len;

    if (0U == msc->scsi_blk_len) {
        msc_bbb_csw_send (udev, CSW_CMD_PASSED);
    } else {
        /* prepare endpoint to receive next packet */
        usbd_ep_recev (udev, 
                       MSC_OUT_EP, 
                       msc->bbb_data, 
                       USB_MIN (msc->scsi_blk_len, MSC_MEDIA_PACKET_SIZE));
    }

    return 0;
}

/*!
    \brief      process Format Unit command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[out] none
    \retval     status
*/
static inline int8_t scsi_format_cmd (usb_core_driver *udev, uint8_t lun)
{
    return 0;
}

/*!
    \brief      process Read_Toc command
    \param[in]  udev: pointer to USB device instance
    \param[in]  lun: logical unit number
    \param[in]  params: command parameters
    \param[out] none
    \retval     status
*/
static int8_t scsi_toc_cmd_read (usb_core_driver *udev, uint8_t lun, uint8_t *params)
{
    uint8_t* pPage;
    uint16_t len;

    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    pPage = (uint8_t *)&usbd_mem_fops->mem_toc_data[lun * READ_TOC_CMD_LEN];
    len = (uint16_t)pPage[1] + 2U;

    msc->bbb_datalen = len;

    while (len) {
        len--;
        msc->bbb_data[len] = pPage[len];
    }

    return 0;
}
