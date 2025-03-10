/*!
    \file    usbh_msc_bbb.c
    \brief   USB MSC BBB protocol related functions

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

#include "gdusbh_pipe.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bbb.h"
#include "usbh_transc.h"
#include "drv_usbh_int.h"

/*!
    \brief      initialize the mass storage parameters
    \param[in]  uhost: pointer to USB host handler
    \param[out] none
    \retval     none
*/
void usbh_msc_bot_init (usbh_host *uhost)
{
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;

    msc->bot.cbw.field.dCBWSignature = BBB_CBW_SIGNATURE;
    msc->bot.cbw.field.dCBWTag = USBH_MSC_BOT_CBW_TAG;
    msc->bot.state = BOT_SEND_CBW;
    msc->bot.cmd_state = BOT_CMD_SEND;
}

/*!
    \brief      manage the different states of BOT transfer and updates the status to upper layer
    \param[in]  uhost: pointer to usb host handler
    \param[in]  lun: logic unit number
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_bot_process (usbh_host *uhost, uint8_t lun)
{
    bot_csw_status csw_status = BOT_CSW_CMD_FAILED;
    usbh_status status = USBH_BUSY;
    usbh_status error = USBH_BUSY;
    usb_urb_state urb_status = GD_URB_IDLE;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;

    switch (msc->bot.state) {
    case BOT_SEND_CBW:
        msc->bot.cbw.field.bCBWLUN = lun;
        msc->bot.state = BOT_SEND_CBW_WAIT;
        /* send CBW */
        usbh_data_send (uhost->data,
                        msc->bot.cbw.CBWArray, 
                        msc->pipe_out, 
                        BBB_CBW_LENGTH);
        break;

    case BOT_SEND_CBW_WAIT:
        urb_status = usbh_urbstate_get(uhost->data, msc->pipe_out);

        if (GD_URB_DONE == urb_status) {
            if (0U != msc->bot.cbw.field.dCBWDataTransferLength) {
                if (USB_TRX_IN == (msc->bot.cbw.field.bmCBWFlags & USB_TRX_MASK)) {
                    msc->bot.state = BOT_DATA_IN;
                } else {
                    msc->bot.state = BOT_DATA_OUT;
                }
            } else {
                msc->bot.state = BOT_RECEIVE_CSW;
            }

        } else if (GD_URB_NOTREADY == urb_status) {
            msc->bot.state = BOT_SEND_CBW;
        } else {
            if (GD_URB_STALL == urb_status) {
                msc->bot.state = BOT_ERROR_OUT;
            }
        }
        break;

    case BOT_DATA_IN:
        usbh_data_recev (uhost->data, 
                         msc->bot.pbuf, 
                         msc->pipe_in, 
                         msc->ep_size_in);

        msc->bot.state = BOT_DATA_IN_WAIT;
        break;

    case BOT_DATA_IN_WAIT:
        urb_status = usbh_urbstate_get(uhost->data, msc->pipe_in);

        /* BOT DATA IN stage */
        if (GD_URB_DONE == urb_status) {
            if (msc->bot.cbw.field.dCBWDataTransferLength > msc->ep_size_in) {
                msc->bot.pbuf += msc->ep_size_in;
                msc->bot.cbw.field.dCBWDataTransferLength -= msc->ep_size_in;
            } else {
                msc->bot.cbw.field.dCBWDataTransferLength = 0U;
            }

            if (msc->bot.cbw.field.dCBWDataTransferLength > 0U) {
                usbh_data_recev (uhost->data, 
                                 msc->bot.pbuf, 
                                 msc->pipe_in, 
                                 msc->ep_size_in);
            } else {
                msc->bot.state = BOT_RECEIVE_CSW;
            }
        } else if(GD_URB_STALL == urb_status) {
            /* this is data stage stall condition */
            msc->bot.state = BOT_ERROR_IN;
        } else {
            /* no operation */
        }
        break;

    case BOT_DATA_OUT:
        usbh_data_send (uhost->data,
                        msc->bot.pbuf, 
                        msc->pipe_out, 
                        msc->ep_size_out);

        msc->bot.state = BOT_DATA_OUT_WAIT;
        break;

    case BOT_DATA_OUT_WAIT:
        /* BOT DATA OUT stage */
        urb_status = usbh_urbstate_get(uhost->data, msc->pipe_out);
        if (GD_URB_DONE == urb_status) {
            if (msc->bot.cbw.field.dCBWDataTransferLength > msc->ep_size_out) {
                msc->bot.pbuf += msc->ep_size_out;
                msc->bot.cbw.field.dCBWDataTransferLength -= msc->ep_size_out;
            }  else {
                msc->bot.cbw.field.dCBWDataTransferLength = 0; /* reset this value and keep in same state */
            }

            if (msc->bot.cbw.field.dCBWDataTransferLength > 0) {
                usbh_data_send (uhost->data,
                                msc->bot.pbuf, 
                                msc->pipe_out, 
                                msc->ep_size_out);
            } else {
                msc->bot.state = BOT_RECEIVE_CSW;
            }
        } else if (GD_URB_NOTREADY == urb_status) {
            msc->bot.state = BOT_DATA_OUT;
        } else if (GD_URB_STALL == urb_status) {
            msc->bot.state = BOT_ERROR_OUT;
        } else {
            /* no operation */
        }
        break;

    case BOT_RECEIVE_CSW:
        /* BOT CSW stage */
        usbh_data_recev (uhost->data,
                         msc->bot.csw.CSWArray, 
                         msc->pipe_in, 
                         BBB_CSW_LENGTH);

        msc->bot.state = BOT_RECEIVE_CSW_WAIT;
        break;

    case BOT_RECEIVE_CSW_WAIT:
        urb_status = usbh_urbstate_get(uhost->data, msc->pipe_in);

        /* decode CSW */
        if (GD_URB_DONE == urb_status) {
            msc->bot.state = BOT_SEND_CBW;
            msc->bot.cmd_state = BOT_CMD_SEND;

            csw_status = usbh_msc_csw_decode(uhost);
            if (BOT_CSW_CMD_PASSED == csw_status) {
                status = USBH_OK;
            } else {
                status = USBH_FAIL;
            }
        } else if (GD_URB_STALL == urb_status) {
            msc->bot.state = BOT_ERROR_IN;
        } else {
            /* no operation */
        }
        break;

    case BOT_ERROR_IN: 
        error = usbh_msc_bot_abort(uhost, USBH_MSC_DIR_IN);

        if (USBH_OK == error) {
            msc->bot.state = BOT_RECEIVE_CSW;
        } else if (USBH_UNRECOVERED_ERROR == status) {
            /* this means that there is a stall error limit, do reset recovery */
            msc->bot.state = BOT_UNRECOVERED_ERROR;
        } else {
            /* no operation */
        }
        break;

    case BOT_ERROR_OUT: 
        status = usbh_msc_bot_abort (uhost, USBH_MSC_DIR_OUT);

        if (USBH_OK == status) {
            uint8_t toggle = usbh_pipe_toggle_get(uhost->data, msc->pipe_out);
            usbh_pipe_toggle_set(uhost->data, msc->pipe_out, 1U - toggle);
            usbh_pipe_toggle_set(uhost->data, msc->pipe_in, 0U);
            msc->bot.state = BOT_ERROR_IN;
        } else {
            if (USBH_UNRECOVERED_ERROR == status) {
                msc->bot.state = BOT_UNRECOVERED_ERROR;
            }
        }
        break;

    case BOT_UNRECOVERED_ERROR:
        status = usbh_msc_bot_reset(uhost);
        if (USBH_OK == status) {
            msc->bot.state = BOT_SEND_CBW;
        }
        break;

    default:
        break;
    }

    return status;
}

/*!
    \brief      manages the different error handling for stall
    \param[in]  uhost: pointer to USB host handler
    \param[in]  direction: data IN or OUT
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_bot_abort (usbh_host *uhost, uint8_t direction)
{
    usbh_status status = USBH_BUSY;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;

    switch (direction) {
    case USBH_MSC_DIR_IN :
        /* send clear feature command on bulk IN endpoint */
        status = usbh_clrfeature(uhost,
                                 msc->ep_in,
                                 msc->pipe_in);
        break;

    case USBH_MSC_DIR_OUT :
        /*send clear feature command on bulk OUT endpoint */
        status = usbh_clrfeature(uhost,
                                 msc->ep_out,
                                 msc->pipe_out);
        break;

    default:
        break;
    }

    return status;
}

/*!
    \brief      reset MSC bot transfer
    \param[in]  uhost: pointer to USB host handler
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_msc_bot_reset (usbh_host *uhost)
{
    usbh_status status = USBH_BUSY;

    if (CTL_IDLE == uhost->control.ctl_state) {
        uhost->control.setup.req = (usb_req) {
            .bmRequestType = USB_TRX_OUT | USB_REQTYPE_CLASS | USB_RECPTYPE_ITF,
            .bRequest      = BBB_RESET,
            .wValue        = 0U,
            .wIndex        = 0U,
            .wLength       = 0U
        };

        usbh_ctlstate_config (uhost, NULL, 0U);
    } 

    status = usbh_ctl_handler (uhost);

    return status;
}

/*!
    \brief      decode the CSW received by the device and updates the same to upper layer
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     on success USBH_MSC_OK, on failure USBH_MSC_FAIL
    \notes
          Refer to USB Mass-Storage Class: BOT (www.usb.org)
          6.3.1 Valid CSW Conditions :
          The host shall consider the CSW valid when:
          1. dCSWSignature is equal to 53425355h
          2. the CSW is 13 (Dh) bytes in length,
          3. dCSWTag matches the dCBWTag from the corresponding CBW.
*/
bot_csw_status usbh_msc_csw_decode (usbh_host *uhost)
{
    bot_csw_status status = BOT_CSW_CMD_FAILED;
    usbh_msc_handler *msc = (usbh_msc_handler *)uhost->active_class->class_data;

    /* checking if the transfer length is different than 13 */
    if (BBB_CSW_LENGTH != usbh_xfercount_get (uhost->data, msc->pipe_in)) {
        status = BOT_CSW_PHASE_ERROR;
    } else {
        /* CSW length is correct */

        /* check validity of the CSW Signature and CSWStatus */
        if (BBB_CSW_SIGNATURE == msc->bot.csw.field.dCSWSignature) {
            /* check condition 1. dCSWSignature is equal to 53425355h */
            if (msc->bot.csw.field.dCSWTag == msc->bot.cbw.field.dCBWTag) {
                /* check condition 3. dCSWTag matches the dCBWTag from the corresponding CBW */
                if (0U == msc->bot.csw.field.bCSWStatus) {
                    status = BOT_CSW_CMD_PASSED;
                } else if (1U == msc->bot.csw.field.bCSWStatus) {
                    status = BOT_CSW_CMD_FAILED;
                } else if (2U == msc->bot.csw.field.bCSWStatus) {
                    status = BOT_CSW_PHASE_ERROR;
                } else {
                    /* no operation */
                }
            }
        } else {
            /* If the CSW signature is not valid, we shall return the phase error to
               upper layers for reset recovery */
            status = BOT_CSW_PHASE_ERROR;
        }
    }

    return status;
}
