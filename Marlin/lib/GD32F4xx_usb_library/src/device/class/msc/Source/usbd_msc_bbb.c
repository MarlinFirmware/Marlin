/*!
    \file    usbd_msc_bbb.c
    \brief   USB BBB(Bulk/Bulk/Bulk) protocol core functions
    \note    BBB means Bulk-only transport protocol for USB MSC

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

/* local function prototypes ('static') */
static void msc_bbb_cbw_decode (usb_core_driver *udev);
static void msc_bbb_data_send (usb_core_driver *udev, uint8_t *pbuf, uint32_t Len);
static void msc_bbb_abort (usb_core_driver *udev);

/*!
    \brief      initialize the bbb process
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
void msc_bbb_init (usb_core_driver *udev)
{
    uint8_t lun_num;

    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->bbb_state = BBB_IDLE;
    msc->bbb_status = BBB_STATUS_NORMAL;

    /* initializes the storage logic unit */
    for(lun_num = 0U; lun_num < MEM_LUN_NUM; lun_num++) {
        usbd_mem_fops->mem_init(lun_num);
    }

    /* flush the RX FIFO */
    usbd_fifo_flush (udev, MSC_OUT_EP);

    /* flush the TX FIFO */
    usbd_fifo_flush (udev, MSC_IN_EP);

    /* prepare endpoint to receive the first BBB CBW */
    usbd_ep_recev (udev, MSC_OUT_EP, (uint8_t *)&msc->bbb_cbw, BBB_CBW_LENGTH);
}

/*!
    \brief      reset the BBB machine
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
void msc_bbb_reset (usb_core_driver *udev)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->bbb_state = BBB_IDLE;
    msc->bbb_status = BBB_STATUS_RECOVERY;

    /* prepare endpoint to receive the first BBB command */
    usbd_ep_recev (udev, MSC_OUT_EP, (uint8_t *)&msc->bbb_cbw, BBB_CBW_LENGTH);
}

/*!
    \brief      de-initialize the BBB machine
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
void msc_bbb_deinit (usb_core_driver *udev)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->bbb_state = BBB_IDLE;
}

/*!
    \brief      handle BBB data IN stage
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint number
    \param[out] none
    \retval     none
*/
void msc_bbb_data_in (usb_core_driver *udev, uint8_t ep_num)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    switch (msc->bbb_state) {
    case BBB_DATA_IN:
        if (scsi_process_cmd (udev, msc->bbb_cbw.bCBWLUN, &msc->bbb_cbw.CBWCB[0]) < 0) {
            msc_bbb_csw_send (udev, CSW_CMD_FAILED);
        }
        break;

    case BBB_SEND_DATA:
    case BBB_LAST_DATA_IN:
        msc_bbb_csw_send (udev, CSW_CMD_PASSED);
        break;

    default:
        break;
    }
}

/*!
    \brief      handle BBB data OUT stage
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint number
    \param[out] none
    \retval     none
*/
void msc_bbb_data_out (usb_core_driver *udev, uint8_t ep_num)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    switch (msc->bbb_state) {
    case BBB_IDLE:
        msc_bbb_cbw_decode (udev);
        break;

    case BBB_DATA_OUT:
        if (scsi_process_cmd (udev, msc->bbb_cbw.bCBWLUN, &msc->bbb_cbw.CBWCB[0]) < 0) {
            msc_bbb_csw_send (udev, CSW_CMD_FAILED);
        }
        break;

    default:
        break;
    }
}

/*!
    \brief      send the CSW(command status wrapper)
    \param[in]  udev: pointer to USB device instance
    \param[in]  csw_status: CSW status
    \param[out] none
    \retval     none
*/
void msc_bbb_csw_send (usb_core_driver *udev, uint8_t csw_status)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->bbb_csw.dCSWSignature = BBB_CSW_SIGNATURE;
    msc->bbb_csw.bCSWStatus = csw_status;
    msc->bbb_state = BBB_IDLE;

    usbd_ep_send (udev, MSC_IN_EP, (uint8_t *)&msc->bbb_csw, BBB_CSW_LENGTH);

    /* prepare endpoint to receive next command */
    usbd_ep_recev (udev, MSC_OUT_EP, (uint8_t *)&msc->bbb_cbw, BBB_CBW_LENGTH);
}

/*!
    \brief      complete the clear feature request
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint number
    \param[out] none
    \retval     none
*/
void msc_bbb_clrfeature (usb_core_driver *udev, uint8_t ep_num)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    if (msc->bbb_status == BBB_STATUS_ERROR)/* bad CBW signature */ {
        usbd_ep_stall(udev, MSC_IN_EP);

        msc->bbb_status = BBB_STATUS_NORMAL;
    } else if(((ep_num & 0x80U) == 0x80U) && (msc->bbb_status != BBB_STATUS_RECOVERY)) {
        msc_bbb_csw_send (udev, CSW_CMD_FAILED);
    } else {
    
    }
}

/*!
    \brief      decode the CBW command and set the BBB state machine accordingly
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void msc_bbb_cbw_decode (usb_core_driver *udev)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    msc->bbb_csw.dCSWTag = msc->bbb_cbw.dCBWTag;
    msc->bbb_csw.dCSWDataResidue = msc->bbb_cbw.dCBWDataTransferLength;

    if ((BBB_CBW_LENGTH != usbd_rxcount_get (udev, MSC_OUT_EP)) ||
            (BBB_CBW_SIGNATURE != msc->bbb_cbw.dCBWSignature)||
                (msc->bbb_cbw.bCBWLUN > 1U) || 
                    (msc->bbb_cbw.bCBWCBLength < 1U) || 
                        (msc->bbb_cbw.bCBWCBLength > 16U)) {
        /* illegal command handler */
        scsi_sense_code (udev, msc->bbb_cbw.bCBWLUN, ILLEGAL_REQUEST, INVALID_CDB);

        msc->bbb_status = BBB_STATUS_ERROR;

        msc_bbb_abort (udev);
    } else {
        if (scsi_process_cmd (udev, msc->bbb_cbw.bCBWLUN, &msc->bbb_cbw.CBWCB[0]) < 0) {
            msc_bbb_abort (udev);
        } else if ((BBB_DATA_IN != msc->bbb_state) && 
                    (BBB_DATA_OUT != msc->bbb_state) &&
                      (BBB_LAST_DATA_IN != msc->bbb_state)) { /* burst xfer handled internally */
            if (msc->bbb_datalen > 0U) {
                msc_bbb_data_send (udev, msc->bbb_data, msc->bbb_datalen);
            } else if (0U == msc->bbb_datalen) {
                msc_bbb_csw_send (udev, CSW_CMD_PASSED);
            } else {
            
            }
        } else {
        
        }
    }
}

/*!
    \brief      send the requested data
    \param[in]  udev: pointer to USB device instance
    \param[in]  buf: pointer to data buffer
    \param[in]  len: data length
    \param[out] none
    \retval     none
*/
static void msc_bbb_data_send (usb_core_driver *udev, uint8_t *buf, uint32_t len)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    len = USB_MIN (msc->bbb_cbw.dCBWDataTransferLength, len);

    msc->bbb_csw.dCSWDataResidue -= len;
    msc->bbb_csw.bCSWStatus = CSW_CMD_PASSED;
    msc->bbb_state = BBB_SEND_DATA;

    usbd_ep_send (udev, MSC_IN_EP, buf, len);
}

/*!
    \brief      abort the current transfer
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void msc_bbb_abort (usb_core_driver *udev)
{
    usbd_msc_handler *msc = (usbd_msc_handler *)udev->dev.class_data[USBD_MSC_INTERFACE];

    if ((0U == msc->bbb_cbw.bmCBWFlags) && 
         (0U != msc->bbb_cbw.dCBWDataTransferLength) &&
          (BBB_STATUS_NORMAL == msc->bbb_status)) {
        usbd_ep_stall(udev, MSC_OUT_EP);
    }

    usbd_ep_stall(udev, MSC_IN_EP);

    if (msc->bbb_status == BBB_STATUS_ERROR) {
        usbd_ep_recev (udev, MSC_OUT_EP, (uint8_t *)&msc->bbb_cbw, BBB_CBW_LENGTH);
    }
}
