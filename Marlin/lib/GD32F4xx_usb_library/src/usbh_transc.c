/*!
    \file    usbh_transc.c
    \brief   USB host mode transactions driver

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

#include "drv_usb_hw.h"
#include "gdusbh_pipe.h"
#include "usbh_transc.h"

/* local function prototypes ('static') */
static usb_urb_state usbh_urb_wait (usbh_host *uhost, uint8_t pp_num, uint32_t wait_time);
static void usbh_setup_transc (usbh_host *uhost);
static void usbh_data_in_transc (usbh_host *uhost);
static void usbh_data_out_transc (usbh_host *uhost);
static void usbh_status_in_transc (usbh_host *uhost);
static void usbh_status_out_transc (usbh_host *uhost);
static uint32_t usbh_request_submit (usb_core_driver *udev, uint8_t pp_num);

/*!
    \brief      send the setup packet to the USB device
    \param[in]  udev: pointer to USB core instance
    \param[in]  buf: data buffer which will be sent to USB device
    \param[in]  pp_num: pipe number
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_ctlsetup_send (usb_core_driver *udev, uint8_t *buf, uint8_t pp_num)
{
    usb_pipe *pp = &udev->host.pipe[pp_num];

    pp->DPID = PIPE_DPID_SETUP;
    pp->xfer_buf = buf;
    pp->xfer_len = USB_SETUP_PACKET_LEN;

    return (usbh_status)usbh_request_submit (udev, pp_num);
}

/*!
    \brief      send a data packet to the USB device
    \param[in]  udev: pointer to USB core instance
    \param[in]  buf: data buffer which will be sent to USB device
    \param[in]  pp_num: pipe number
    \param[in]  len: length of the data to be sent
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_data_send (usb_core_driver *udev, uint8_t *buf, uint8_t pp_num, uint16_t len)
{
    usb_pipe *pp = &udev->host.pipe[pp_num];

    pp->xfer_buf = buf;
    pp->xfer_len = len;

    switch (pp->ep.type) {
    case USB_EPTYPE_CTRL:
        if (0U == len) {
            pp->data_toggle_out = 1U;
        }

        pp->DPID = PIPE_DPID[pp->data_toggle_out];
        break;

    case USB_EPTYPE_INTR:
        pp->DPID = PIPE_DPID[pp->data_toggle_out];

        pp->data_toggle_out ^= 1U;
        break;

    case USB_EPTYPE_BULK:
        pp->DPID = PIPE_DPID[pp->data_toggle_out];
        break;

    case USB_EPTYPE_ISOC:
        pp->DPID = PIPE_DPID[0];
        break;

    default:
        break;
    }

    usbh_request_submit (udev, pp_num);

    return USBH_OK;
}

/*!
    \brief      receive a data packet from the USB device
    \param[in]  udev: pointer to USB core instance
    \param[in]  buf: data buffer which will be received from USB device
    \param[in]  pp_num: pipe number
    \param[in]  len: length of the data to be received
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_data_recev (usb_core_driver *udev, uint8_t *buf, uint8_t pp_num, uint16_t len)
{
    usb_pipe *pp = &udev->host.pipe[pp_num];

    pp->xfer_buf = buf;
    pp->xfer_len = len;

    switch (pp->ep.type) {
    case USB_EPTYPE_CTRL:
        pp->DPID = PIPE_DPID[1];
        break;

    case USB_EPTYPE_INTR:
        pp->DPID = PIPE_DPID[pp->data_toggle_in];

        /* Toggle DATA PID */
        pp->data_toggle_in ^= 1U;
        break;

    case USB_EPTYPE_BULK:
        pp->DPID = PIPE_DPID[pp->data_toggle_in];
        break;

    case USB_EPTYPE_ISOC:
        pp->DPID = PIPE_DPID[0];
        break;

    default:
        break;
    }

    usbh_request_submit (udev, pp_num);

    return USBH_OK;
}

/*!
    \brief      USB control transfer handler
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     operation status
*/
usbh_status usbh_ctl_handler (usbh_host *uhost)
{
    usbh_status status = USBH_BUSY;

    switch (uhost->control.ctl_state) {
    case CTL_SETUP:
        usbh_setup_transc (uhost);
        break;

    case CTL_DATA_IN:
        usbh_data_in_transc (uhost);
        break;

    case CTL_DATA_OUT:
        usbh_data_out_transc (uhost);
        break;

    case CTL_STATUS_IN:
        usbh_status_in_transc (uhost);
        break;

    case CTL_STATUS_OUT:
        usbh_status_out_transc (uhost);
        break;

    case CTL_FINISH:
        uhost->control.ctl_state = CTL_IDLE;

        status = USBH_OK;
        break;

    case CTL_ERROR:
        if (++uhost->control.error_count <= USBH_MAX_ERROR_COUNT) {
            /* do the transmission again, starting from SETUP packet */
            uhost->control.ctl_state = CTL_SETUP;
        } else {
            status = USBH_FAIL;
        }
        break;

    default:
        break;
    }

    return status;
}

/*!
    \brief      wait for USB URB(USB request block) state
    \param[in]  uhost: pointer to USB host
    \param[in]  pp_num: pipe number
    \param[in]  wait_time: wait time
    \param[out] none
    \retval     USB URB state
*/
static usb_urb_state usbh_urb_wait (usbh_host *uhost, uint8_t pp_num, uint32_t wait_time)
{
    usb_urb_state urb_status = GD_URB_IDLE;

    while (GD_URB_DONE != (urb_status = usbh_urbstate_get(uhost->data, pp_num))) {
        if (GD_URB_NOTREADY == urb_status) {
            break;
        } else if (GD_URB_STALL == urb_status) {
            uhost->control.ctl_state = CTL_SETUP;
            break;
        } else if (GD_URB_ERROR == urb_status) {
            uhost->control.ctl_state = CTL_ERROR;
            break;
        } else if ((wait_time > 0U) && ((usb_curframe_get(uhost->data)- uhost->control.timer) > wait_time)) {
            /* timeout for in transfer */
            uhost->control.ctl_state = CTL_ERROR;
            break;
        } else {
            /* no operation, just wait */
        }
    }

    return urb_status;
}

/*!
    \brief      USB setup transaction
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     none
*/
static void usbh_setup_transc (usbh_host *uhost)
{
    /* send a SETUP packet */
    usbh_ctlsetup_send (uhost->data, 
                        uhost->control.setup.data, 
                        uhost->control.pipe_out_num);

    if (GD_URB_DONE == usbh_urb_wait (uhost, uhost->control.pipe_out_num, 0U)) {
        uint8_t dir = (uhost->control.setup.req.bmRequestType & USB_TRX_MASK);

        if (uhost->control.setup.req.wLength) {
            if (USB_TRX_IN == dir) {
                uhost->control.ctl_state = CTL_DATA_IN;
            } else {
                uhost->control.ctl_state = CTL_DATA_OUT;
            }
        } else {
            if (USB_TRX_IN == dir) {
                uhost->control.ctl_state = CTL_STATUS_OUT;
            } else {
                uhost->control.ctl_state = CTL_STATUS_IN;
            }
        }

        /* set the delay timer to enable timeout for data stage completion */
        uhost->control.timer = (uint16_t)usb_curframe_get(uhost->data);
    }
}

/*!
    \brief      USB data IN transaction
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     none
*/
static void usbh_data_in_transc (usbh_host *uhost)
{
    usbh_data_recev (uhost->data,
                     uhost->control.buf,
                     uhost->control.pipe_in_num,
                     uhost->control.ctl_len);

    if (GD_URB_DONE == usbh_urb_wait (uhost, uhost->control.pipe_in_num, DATA_STAGE_TIMEOUT)) {
        uhost->control.ctl_state = CTL_STATUS_OUT;

        uhost->control.timer = (uint16_t)usb_curframe_get(uhost->data);
    }
}

/*!
    \brief      USB data OUT transaction
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     none
*/
static void usbh_data_out_transc (usbh_host *uhost)
{
    usbh_pipe_toggle_set(uhost->data, uhost->control.pipe_out_num, 1U);

    usbh_data_send (uhost->data,
                    uhost->control.buf,
                    uhost->control.pipe_out_num,
                    uhost->control.ctl_len);

    if (GD_URB_DONE == usbh_urb_wait (uhost, uhost->control.pipe_out_num, DATA_STAGE_TIMEOUT)) {
        uhost->control.ctl_state = CTL_STATUS_IN;

        uhost->control.timer = (uint16_t)usb_curframe_get(uhost->data);
    }
}

/*!
    \brief      USB status IN transaction
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     none
*/
static void usbh_status_in_transc (usbh_host *uhost)
{
    uint8_t pp_num = uhost->control.pipe_in_num;

    usbh_data_recev (uhost->data, NULL, pp_num, 0U);

    if (GD_URB_DONE == usbh_urb_wait (uhost, pp_num, NODATA_STAGE_TIMEOUT)) {
        uhost->control.ctl_state = CTL_FINISH;
    }
}

/*!
    \brief      USB status OUT transaction
    \param[in]  uhost: pointer to USB host
    \param[out] none
    \retval     none
*/
static void usbh_status_out_transc (usbh_host *uhost)
{
    uint8_t pp_num = uhost->control.pipe_out_num;

    usbh_data_send (uhost->data, NULL, pp_num, 0U);

    if (GD_URB_DONE == usbh_urb_wait (uhost, pp_num, NODATA_STAGE_TIMEOUT)) {
        uhost->control.ctl_state = CTL_FINISH;
    }
}

/*!
    \brief      prepare a pipe and start a transfer
    \param[in]  udev: pointer to USB core instance
    \param[in]  pp_num: pipe number
    \param[out] none
    \retval     operation status
*/
static uint32_t usbh_request_submit (usb_core_driver *udev, uint8_t pp_num) 
{
    udev->host.pipe[pp_num].urb_state = GD_URB_IDLE;
    udev->host.pipe[pp_num].xfer_count = 0U;

    return (uint32_t)usb_pipe_xfer (udev, pp_num);
}
