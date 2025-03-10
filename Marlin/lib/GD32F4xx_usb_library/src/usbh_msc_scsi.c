/*!
    \file    usbh_msc_scsi.c
    \brief   USB MSC SCSI commands implementing

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

#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bbb.h"

#include <string.h>

/*!
    \brief      send 'Inquiry' command to the device
    \param[in]  uhost: pointer to USB host handler
    \param[in]  lun: logic unit number
    \param[in]  inquiry: pointer to the inquiry structure
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_scsi_inquiry (usbh_host *uhost, uint8_t lun, scsi_std_inquiry_data *inquiry)
{
    usbh_status error = USBH_FAIL;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;

    switch (msc->bot.cmd_state) {
    case BOT_CMD_SEND:
        /* prepare the CBW and relevant field*/
        msc->bot.cbw.field.dCBWDataTransferLength = STANDARD_INQUIRY_DATA_LEN;
        msc->bot.cbw.field.bmCBWFlags = USB_TRX_IN;
        msc->bot.cbw.field.bCBWCBLength = CBW_LENGTH;

        memset(msc->bot.cbw.field.CBWCB, 0U, CBW_LENGTH);

        msc->bot.cbw.field.CBWCB[0] = SCSI_INQUIRY;
        msc->bot.cbw.field.CBWCB[1] = (lun << 5U);
        msc->bot.cbw.field.CBWCB[4] = 0x24U;

        msc->bot.state = BOT_SEND_CBW;
        msc->bot.cmd_state = BOT_CMD_WAIT;
        msc->bot.pbuf = (uint8_t *)(void *)msc->bot.data;
        error = USBH_BUSY;
        break;

    case BOT_CMD_WAIT:
        error = usbh_msc_bot_process(uhost, lun);

        if (USBH_OK == error) {
            memset(inquiry, 0U, sizeof(scsi_std_inquiry_data));

            /* assign inquiry data */
            inquiry->device_type = msc->bot.pbuf[0] & 0x1FU;
            inquiry->peripheral_qualifier = msc->bot.pbuf[0] >> 5U;

            if (0x80U == ((uint32_t)msc->bot.pbuf[1] & 0x80U)) {
                inquiry->removable_media = 1U;
            } else {
                inquiry->removable_media = 0U;
            }

            memcpy (inquiry->vendor_id, &msc->bot.pbuf[8], 8U);
            memcpy (inquiry->product_id, &msc->bot.pbuf[16], 16U);
            memcpy (inquiry->revision_id, &msc->bot.pbuf[32], 4U);
        }
        break;

    default:
        break;
    }

    return error;
}

/*!
    \brief      send 'Test unit ready' command to the device
    \param[in]  uhost: pointer to USB host handler
    \param[in]  lun: logic unit number
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_test_unitready (usbh_host *uhost, uint8_t lun)
{
    usbh_status status = USBH_FAIL;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;


    switch (msc->bot.cmd_state) {
    case BOT_CMD_SEND:  
        /* prepare the CBW and relevant field */
        msc->bot.cbw.field.dCBWDataTransferLength = CBW_LENGTH_TEST_UNIT_READY;
        msc->bot.cbw.field.bmCBWFlags = USB_TRX_OUT;
        msc->bot.cbw.field.bCBWCBLength = CBW_LENGTH;

        memset(msc->bot.cbw.field.CBWCB, 0U, CBW_CB_LENGTH);

        msc->bot.cbw.field.CBWCB[0] = SCSI_TEST_UNIT_READY; 
        msc->bot.state = BOT_SEND_CBW;
        msc->bot.cmd_state = BOT_CMD_WAIT;

        status = USBH_BUSY;
        break;

    case BOT_CMD_WAIT: 
        status = usbh_msc_bot_process(uhost, lun);
        break;

    default:
        break;
    }

    return status;
}

/*!
    \brief      send the read capacity command to the device
    \param[in]  uhost: pointer to USB host handler
    \param[in]  lun: logic unit number
    \param[in]  capacity: pointer to SCSI capacity
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_read_capacity10 (usbh_host *uhost, uint8_t lun, scsi_capacity *capacity)
{
    usbh_status status = USBH_FAIL;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;

    switch (msc->bot.cmd_state) {
    case BOT_CMD_SEND:
        /* prepare the CBW and relevant field */
        msc->bot.cbw.field.dCBWDataTransferLength = READ_CAPACITY10_DATA_LEN;
        msc->bot.cbw.field.bmCBWFlags = USB_TRX_IN;
        msc->bot.cbw.field.bCBWCBLength = CBW_LENGTH;

        memset(msc->bot.cbw.field.CBWCB, 0U, CBW_CB_LENGTH);

        msc->bot.cbw.field.CBWCB[0] = SCSI_READ_CAPACITY10;
        msc->bot.state = BOT_SEND_CBW;
        msc->bot.cmd_state = BOT_CMD_WAIT;
        msc->bot.pbuf = (uint8_t *)(void *)msc->bot.data;

        status = USBH_BUSY;
        break;

    case BOT_CMD_WAIT:
        status = usbh_msc_bot_process(uhost, lun);

        if (USBH_OK == status) {
            capacity->block_nbr = msc->bot.pbuf[3] | \
                                  ((uint32_t)msc->bot.pbuf[2] << 8U) | \
                                  ((uint32_t)msc->bot.pbuf[1] << 16U) | \
                                  ((uint32_t)msc->bot.pbuf[0] << 24U);

            capacity->block_size = (uint16_t)(msc->bot.pbuf[7] | ((uint32_t)msc->bot.pbuf[6] << 8U));
        }
        break;

    default:
        break;
    }

    return status;
}

/*!
    \brief      send the mode sense6 command to the device
    \param[in]  uhost: pointer to USB host handler
    \param[in]  lun: logic unit number
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_mode_sense6 (usbh_host *uhost, uint8_t lun)
{
    usbh_status status = USBH_FAIL;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;


    switch (msc->bot.cmd_state) {
    case BOT_CMD_SEND:
        /* prepare the CBW and relevant field */
        msc->bot.cbw.field.dCBWDataTransferLength = XFER_LEN_MODE_SENSE6;
        msc->bot.cbw.field.bmCBWFlags = USB_TRX_IN;
        msc->bot.cbw.field.bCBWCBLength = CBW_LENGTH;

        memset(msc->bot.cbw.field.CBWCB, 0U, CBW_CB_LENGTH);

        msc->bot.cbw.field.CBWCB[0] = SCSI_MODE_SENSE6; 
        msc->bot.cbw.field.CBWCB[2] = MODE_SENSE_PAGE_CONTROL_FIELD | MODE_SENSE_PAGE_CODE;
        msc->bot.cbw.field.CBWCB[4] = XFER_LEN_MODE_SENSE6;
        msc->bot.state = BOT_SEND_CBW;
        msc->bot.cmd_state = BOT_CMD_WAIT;
        msc->bot.pbuf = (uint8_t *)(void *)msc->bot.data;

        status = USBH_BUSY;
        break;

    case BOT_CMD_WAIT:
        status = usbh_msc_bot_process(uhost, lun);

        if (USBH_OK == status) {
            if (msc->bot.data[2] & MASK_MODE_SENSE_WRITE_PROTECT) {

            } else {

            }
        }
        break;

    default:
        break;
    }


    return status;
}

/*!
    \brief      send the Request Sense command to the device
    \param[in]  uhost: pointer to USB host handler
    \param[in]  lun: logic unit number
    \param[in]  sense_data: pointer to sense data
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_request_sense (usbh_host *uhost, uint8_t lun, msc_scsi_sense *sense_data)
{
    usbh_status status = USBH_FAIL;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;

    switch (msc->bot.cmd_state) {
    case BOT_CMD_SEND:
        /* prepare the CBW and relevant field */
        msc->bot.cbw.field.dCBWDataTransferLength = ALLOCATION_LENGTH_REQUEST_SENSE;
        msc->bot.cbw.field.bmCBWFlags = USB_TRX_IN;
        msc->bot.cbw.field.bCBWCBLength = CBW_LENGTH;

        memset(msc->bot.cbw.field.CBWCB, 0U, CBW_CB_LENGTH);

        msc->bot.cbw.field.CBWCB[0] = SCSI_REQUEST_SENSE; 
        msc->bot.cbw.field.CBWCB[1] = (lun << 5U);
        msc->bot.cbw.field.CBWCB[4] = ALLOCATION_LENGTH_REQUEST_SENSE;

        msc->bot.state = BOT_SEND_CBW;
        msc->bot.cmd_state = BOT_CMD_WAIT;
        msc->bot.pbuf = (uint8_t *)(void *)msc->bot.data;

        status = USBH_BUSY;
        break;

    case BOT_CMD_WAIT:
        status = usbh_msc_bot_process(uhost, lun);

        if (status == USBH_OK) {
            /* get sense data */
            sense_data->SenseKey = msc->bot.pbuf[2] & 0x0FU;
            sense_data->ASC = msc->bot.pbuf[12];
            sense_data->ASCQ = msc->bot.pbuf[13];
        }
        break;

    default:
        break;
    }

    return status;
}

/*!
    \brief      send the write10 command to the device
    \param[in]  uhost: pointer to USB host handler
    \param[in]  lun: logic unit number
    \param[in]  data_buf: data buffer contains the data to write
    \param[in]  addr: address to which the data will be written
    \param[in]  sector_num: number of sector to be written
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_write10 (usbh_host *uhost, uint8_t lun, uint8_t *data_buf, uint32_t addr, uint32_t sector_num)
{
    usbh_status status = USBH_FAIL;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;

    switch (msc->bot.cmd_state) {
    case BOT_CMD_SEND:
        msc->bot.cbw.field.dCBWDataTransferLength = sector_num * msc->unit[lun].capacity.block_size;
        msc->bot.cbw.field.bmCBWFlags = USB_TRX_OUT;
        msc->bot.cbw.field.bCBWCBLength = CBW_LENGTH;

        memset(msc->bot.cbw.field.CBWCB, 0U, CBW_CB_LENGTH);

        msc->bot.cbw.field.CBWCB[0] = SCSI_WRITE10; 

        /* logical block address */
        msc->bot.cbw.field.CBWCB[2] = (((uint8_t*)&addr)[3]);
        msc->bot.cbw.field.CBWCB[3] = (((uint8_t*)&addr)[2]);
        msc->bot.cbw.field.CBWCB[4] = (((uint8_t*)&addr)[1]);
        msc->bot.cbw.field.CBWCB[5] = (((uint8_t*)&addr)[0]);

        /* transfer length */
        msc->bot.cbw.field.CBWCB[7] = (((uint8_t *)&sector_num)[1]);
        msc->bot.cbw.field.CBWCB[8] = (((uint8_t *)&sector_num)[0]);

        msc->bot.state = BOT_SEND_CBW;
        msc->bot.cmd_state = BOT_CMD_WAIT;
        msc->bot.pbuf = data_buf;

        status = USBH_BUSY;
        break;

    case BOT_CMD_WAIT:
        status = usbh_msc_bot_process(uhost, lun);
        break;

    default:
        break;
    }

    return status;
}

/*!
    \brief      send the read10 command to the device
    \param[in]  uhost: pointer to USB host handler
    \param[in]  lun: logic unit number
    \param[in]  data_buf: data buffer contains the data to write
    \param[in]  addr: address to which the data will be read
    \param[in]  sector_num: number of sector to be read
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_read10 (usbh_host *uhost, uint8_t lun, uint8_t *data_buf, uint32_t addr, uint32_t sector_num)
{
    usbh_status status = USBH_FAIL;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;

    switch (msc->bot.cmd_state) {
    case BOT_CMD_SEND:
        /* prepare the CBW and relevant field */
        msc->bot.cbw.field.dCBWDataTransferLength = sector_num * msc->unit[lun].capacity.block_size;
        msc->bot.cbw.field.bmCBWFlags = USB_TRX_IN;
        msc->bot.cbw.field.bCBWCBLength = CBW_LENGTH;

        memset(msc->bot.cbw.field.CBWCB, 0U, CBW_CB_LENGTH);

        msc->bot.cbw.field.CBWCB[0] = SCSI_READ10; 

        /* logical block address */
        msc->bot.cbw.field.CBWCB[2] = (((uint8_t*)&addr)[3]);
        msc->bot.cbw.field.CBWCB[3] = (((uint8_t*)&addr)[2]);
        msc->bot.cbw.field.CBWCB[4] = (((uint8_t*)&addr)[1]);
        msc->bot.cbw.field.CBWCB[5] = (((uint8_t*)&addr)[0]);

        /* transfer length */
        msc->bot.cbw.field.CBWCB[7] = (((uint8_t *)&sector_num)[1]);
        msc->bot.cbw.field.CBWCB[8] = (((uint8_t *)&sector_num)[0]);

        msc->bot.state = BOT_SEND_CBW;
        msc->bot.cmd_state = BOT_CMD_WAIT;
        msc->bot.pbuf = data_buf;

        status = USBH_BUSY;
        break;

    case BOT_CMD_WAIT:
        status = usbh_msc_bot_process(uhost, lun);
        break;

    default:
        break;
    }

    return status;
}
