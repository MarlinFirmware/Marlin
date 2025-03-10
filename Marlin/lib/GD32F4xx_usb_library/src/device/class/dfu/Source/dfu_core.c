/*!
    \file    dfu_core.c
    \brief   USB DFU device class core functions

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

#include "dfu_core.h"
#include "drv_usb_hw.h"
#include "dfu_mal.h"
#include "flash_if.h"
#include <string.h>

#define USBD_VID                     0x28E9U
#define USBD_PID                     0x0189U

/* local function prototypes ('static') */
static uint8_t dfu_init(usb_dev *udev, uint8_t config_index);
static uint8_t dfu_deinit(usb_dev *udev, uint8_t config_index);
static uint8_t dfu_req_handler(usb_dev *udev, usb_req *req);
static uint8_t dfu_data_in(usb_dev *udev, uint8_t ep_num);
static void dfu_detach(usb_dev *udev, usb_req *req);
static void dfu_dnload(usb_dev *udev, usb_req *req);
static void dfu_upload(usb_dev *udev, usb_req *req);
static void dfu_getstatus(usb_dev *udev, usb_req *req);
static void dfu_clrstatus(usb_dev *udev, usb_req *req);
static void dfu_getstate(usb_dev *udev, usb_req *req);
static void dfu_abort(usb_dev *udev, usb_req *req);
static void dfu_mode_leave(usb_dev *udev);
static uint8_t dfu_getstatus_complete (usb_dev *udev);

static void (*dfu_request_process[])(usb_dev *udev, usb_req *req) = 
{
    [DFU_DETACH]    = dfu_detach,
    [DFU_DNLOAD]    = dfu_dnload,
    [DFU_UPLOAD]    = dfu_upload,
    [DFU_GETSTATUS] = dfu_getstatus,
    [DFU_CLRSTATUS] = dfu_clrstatus,
    [DFU_GETSTATE]  = dfu_getstate,
    [DFU_ABORT]     = dfu_abort
};

/* note:it should use the c99 standard when compiling the below codes */
/* USB standard device descriptor */
__ALIGN_BEGIN const usb_desc_dev dfu_dev_desc __ALIGN_END =
{
    .header = 
     {
         .bLength          = USB_DEV_DESC_LEN, 
         .bDescriptorType  = USB_DESCTYPE_DEV
     },
    .bcdUSB                = 0x0200U,
    .bDeviceClass          = 0x00U,
    .bDeviceSubClass       = 0x00U,
    .bDeviceProtocol       = 0x00U,
    .bMaxPacketSize0       = USB_FS_EP0_MAX_LEN,
    .idVendor              = USBD_VID,
    .idProduct             = USBD_PID,
    .bcdDevice             = 0x0100U,
    .iManufacturer         = STR_IDX_MFC,
    .iProduct              = STR_IDX_PRODUCT,
    .iSerialNumber         = STR_IDX_SERIAL,
    .bNumberConfigurations = USBD_CFG_MAX_NUM
};

/* USB device configuration descriptor */
__ALIGN_BEGIN const usb_dfu_desc_config_set dfu_config_desc __ALIGN_END = 
{
    .config = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_config), 
             .bDescriptorType = USB_DESCTYPE_CONFIG 
         },
        .wTotalLength         = USB_DFU_CONFIG_DESC_SIZE,
        .bNumInterfaces       = 0x01U,
        .bConfigurationValue  = 0x01U,
        .iConfiguration       = 0x00U,
        .bmAttributes         = 0x80U,
        .bMaxPower            = 0x32U
    },

    .dfu_itf = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_itf), 
             .bDescriptorType = USB_DESCTYPE_ITF 
         },
        .bInterfaceNumber     = 0x00U,
        .bAlternateSetting    = 0x00U,
        .bNumEndpoints        = 0x00U,
        .bInterfaceClass      = USB_DFU_CLASS,
        .bInterfaceSubClass   = USB_DFU_SUBCLASS_UPGRADE,
        .bInterfaceProtocol   = USB_DFU_PROTOCL_DFU,
        .iInterface           = 0x05U
    },

    .dfu_func = 
    {
        .header = 
         {
            .bLength          = sizeof(usb_desc_dfu_func), 
            .bDescriptorType  = DFU_DESC_TYPE 
         },
        .bmAttributes         = USB_DFU_CAN_DOWNLOAD | USB_DFU_CAN_UPLOAD | USB_DFU_WILL_DETACH,
        .wDetachTimeOut       = 0x00FFU,
        .wTransferSize        = TRANSFER_SIZE,
        .bcdDFUVersion        = 0x011AU,
    },
};

/* USB language ID Descriptor */
static __ALIGN_BEGIN const usb_desc_LANGID usbd_language_id_desc __ALIGN_END = 
{
    .header = {
         .bLength         = sizeof(usb_desc_LANGID), 
         .bDescriptorType = USB_DESCTYPE_STR
     },
    .wLANGID              = ENG_LANGID
};

/* USB manufacture string */
static __ALIGN_BEGIN const usb_desc_str manufacturer_string __ALIGN_END = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(10U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'i', 'g', 'a', 'D', 'e', 'v', 'i', 'c', 'e'}
};

/* USB product string */
static __ALIGN_BEGIN const usb_desc_str product_string __ALIGN_END = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(12U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', '-', 'U', 'S', 'B', '_', 'D', 'F', 'U'}
};

/* USBD serial string */
static __ALIGN_BEGIN usb_desc_str serial_string __ALIGN_END = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(2U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     }
};

/* USB configure string */
static __ALIGN_BEGIN const usb_desc_str config_string __ALIGN_END = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(15U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', ' ', 'U', 'S', 'B', ' ', 'C', 'O', 'N', 'F', 'I', 'G'}
};

static __ALIGN_BEGIN const usb_desc_str interface_string __ALIGN_END = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(44U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'@', 'I', 'n', 't', 'e', 'r', 'n', 'a', 'l', 'F', 'l', 'a', 's', 'h', ' ', '/', '0', 'x', '0', '8', '0', '0', 
                       '0', '0', '0', '0', '/', '1', '6', '*', '0', '0', '1', 'K', 'a', ',', '4', '8', '*', '0', '0', '1', 'K', 'g'}
};

void *const usbd_dfu_strings[] = 
{
    [STR_IDX_LANGID]  = (uint8_t *)&usbd_language_id_desc,
    [STR_IDX_MFC]     = (uint8_t *)&manufacturer_string,
    [STR_IDX_PRODUCT] = (uint8_t *)&product_string,
    [STR_IDX_SERIAL]  = (uint8_t *)&serial_string,
    [STR_IDX_CONFIG]  = (uint8_t *)&config_string,
    [STR_IDX_ITF]     = (uint8_t *)&interface_string
};

usb_desc dfu_desc = {
    .dev_desc    = (uint8_t *)&dfu_dev_desc,
    .config_desc = (uint8_t *)&dfu_config_desc,
    .strings     = usbd_dfu_strings
};

usb_class_core dfu_class = {
    .init            = dfu_init,
    .deinit          = dfu_deinit,
    .req_proc        = dfu_req_handler,
    .data_in         = dfu_data_in
};

/*!
    \brief      initialize the DFU device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t dfu_init (usb_dev *udev, uint8_t config_index)
{
    static __ALIGN_BEGIN usbd_dfu_handler dfu_handler __ALIGN_END;

    /* unlock the internal flash */
    dfu_mal_init();

    memset((void *)&dfu_handler, 0, sizeof(usbd_dfu_handler));

    dfu_handler.base_addr = APP_LOADED_ADDR;
    dfu_handler.manifest_state = MANIFEST_COMPLETE;
    dfu_handler.bState = STATE_DFU_IDLE;
    dfu_handler.bStatus = STATUS_OK;

    udev->dev.class_data[USBD_DFU_INTERFACE] = (void *)&dfu_handler;

    return USBD_OK;
}

/*!
    \brief      de-initialize the DFU device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t dfu_deinit (usb_dev *udev, uint8_t config_index)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    /* restore device default state */
    memset(udev->dev.class_data[USBD_DFU_INTERFACE], 0, sizeof(usbd_dfu_handler));

    dfu->bState = STATE_DFU_IDLE;
    dfu->bStatus = STATUS_OK;

    /* lock the internal flash */
    dfu_mal_deinit();

    return USBD_OK;
}

/*!
    \brief      handle the DFU class-specific requests
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: device class-specific request
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t dfu_req_handler (usb_dev *udev, usb_req *req)
{
    if (req->bRequest < DFU_REQ_MAX) {
        dfu_request_process[req->bRequest](udev, req);
    } else {
        return USBD_FAIL;
    }

    return USBD_OK;
}

/*!
    \brief      handle data Stage
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: the endpoint number
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t dfu_data_in (usb_dev *udev, uint8_t ep_num)
{
    if (0U == ep_num) {
        dfu_getstatus_complete(udev);
    }

    return USBD_OK;
}

/*!
    \brief      leave DFU mode and reset device to jump to user loaded code
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void dfu_mode_leave (usb_dev *udev)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    dfu->manifest_state = MANIFEST_COMPLETE;

    if (dfu_config_desc.dfu_func.bmAttributes & 0x04U) {
        dfu->bState = STATE_DFU_MANIFEST_SYNC;
    } else {
        dfu->bState = STATE_DFU_MANIFEST_WAIT_RESET;

        /* lock the internal flash */
        dfu_mal_deinit();

        /* generate system reset to allow jumping to the user code */
        NVIC_SystemReset();
    }
}

/*!
    \brief      handle data IN stage in control endpoint 0
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     USB device operation status
  */
static uint8_t dfu_getstatus_complete (usb_dev *udev)
{
    uint32_t addr;

    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    if (STATE_DFU_DNBUSY == dfu->bState) {
        /* decode the special command */
        if (0U == dfu->block_num) {
            if (1U == dfu->data_len){
                if (GET_COMMANDS == dfu->buf[0]) {
                    /* no operation */
                }
            } else if (5U == dfu->data_len) {
                if (SET_ADDRESS_POINTER == dfu->buf[0]) {
                    /* set flash operation address */
                    dfu->base_addr = *(uint32_t *)(dfu->buf + 1U);
                } else if (ERASE == dfu->buf[0]) {
                    dfu->base_addr = *(uint32_t *)(dfu->buf + 1U);

                    dfu_mal_erase(dfu->base_addr);
                } else {
                    /* no operation */
                }
            } else {
                /* no operation */
            }
        } else if (dfu->block_num > 1U) {  /* regular download command */
            /* decode the required address */
            addr = (dfu->block_num - 2U) * TRANSFER_SIZE + dfu->base_addr;

            dfu_mal_write (dfu->buf, addr, dfu->data_len);

            dfu->block_num = 0U;
        } else {
            /* no operation */
        }

        dfu->data_len = 0U;

        /* update the device state and poll timeout */
        dfu->bState = STATE_DFU_DNLOAD_SYNC;

        return USBD_OK;
    } else if (dfu->bState == STATE_DFU_MANIFEST) { /* manifestation in progress */
        /* start leaving DFU mode */
        dfu_mode_leave(udev);
    } else {
        /* no operation */
    }

    return USBD_OK;
}

/*!
    \brief      handle the DFU_DETACH request
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none.
*/
static void dfu_detach(usb_dev *udev, usb_req *req)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    switch (dfu->bState) {
    case STATE_DFU_IDLE:
    case STATE_DFU_DNLOAD_SYNC:
    case STATE_DFU_DNLOAD_IDLE:
    case STATE_DFU_MANIFEST_SYNC:
    case STATE_DFU_UPLOAD_IDLE:
        dfu->bStatus = STATUS_OK;
        dfu->bState = STATE_DFU_IDLE;
        dfu->iString = 0U; /* iString */

        dfu->block_num = 0U;
        dfu->data_len = 0U;
        break;
 
    default:
        break;
    }

    /* check the detach capability in the DFU functional descriptor */
    if (dfu_config_desc.dfu_func.wDetachTimeOut & DFU_DETACH_MASK) {
        usbd_disconnect (udev);

        usbd_connect (udev);
    } else {
        /* wait for the period of time specified in detach request */
        usb_mdelay (4U);
    }
}

/*!
    \brief      handle the DFU_DNLOAD request
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none
*/
static void dfu_dnload(usb_dev *udev, usb_req *req)
{
    usb_transc *transc = &udev->dev.transc_out[0];
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    switch (dfu->bState) {
    case STATE_DFU_IDLE:
    case STATE_DFU_DNLOAD_IDLE:
        if (req->wLength > 0U) {
            /* update the global length and block number */
            dfu->block_num = req->wValue;
            dfu->data_len = req->wLength;

            dfu->bState = STATE_DFU_DNLOAD_SYNC;

            transc->remain_len = dfu->data_len;
            transc->xfer_buf = dfu->buf;
        } else {
            dfu->manifest_state = MANIFEST_IN_PROGRESS;
            dfu->bState = STATE_DFU_MANIFEST_SYNC;
        }
        break;

    default:
        break;
    }
}

/*!
    \brief      handles the DFU UPLOAD request.
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none
*/
static void  dfu_upload (usb_dev *udev, usb_req *req)
{
    uint8_t *phy_addr = NULL;
    uint32_t addr = 0U;
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    usb_transc *transc = &udev->dev.transc_in[0];

    if(req->wLength <= 0U) {
        dfu->bState = STATE_DFU_IDLE;
        return;
    }

    switch (dfu->bState) {
    case STATE_DFU_IDLE:
    case STATE_DFU_UPLOAD_IDLE:
        /* update the global length and block number */
        dfu->block_num = req->wValue;
        dfu->data_len = req->wLength;

        /* DFU get command */
        if (0U == dfu->block_num) {
            /* update the state machine */
            dfu->bState = (dfu->data_len > 3U) ? STATE_DFU_IDLE : STATE_DFU_UPLOAD_IDLE;

            /* store the values of all supported commands */
            dfu->buf[0] = GET_COMMANDS;
            dfu->buf[1] = SET_ADDRESS_POINTER;
            dfu->buf[2] = ERASE;

            /* send the status data over EP0 */
            transc->xfer_buf = &(dfu->buf[0]);
            transc->remain_len = 3U;
        } else if (dfu->block_num > 1U) {
            dfu->bState = STATE_DFU_UPLOAD_IDLE;

            /* change is accelerated */
            addr = (dfu->block_num - 2U) * TRANSFER_SIZE + dfu->base_addr;

            /* return the physical address where data are stored */
            phy_addr = dfu_mal_read (dfu->buf, addr, dfu->data_len);

            /* send the status data over EP0 */
            transc->xfer_buf = phy_addr;
            transc->remain_len = dfu->data_len;
        } else {
            dfu->bState = STATUS_ERR_STALLEDPKT;
        }
        break;

    default:
        dfu->data_len = 0U;
        dfu->block_num = 0U;
        break;
    }
}

/*!
    \brief      handle the DFU_GETSTATUS request
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none
*/
static void  dfu_getstatus (usb_dev *udev, usb_req *req)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    usb_transc *transc = &udev->dev.transc_in[0];

    switch (dfu->bState) {
    case STATE_DFU_DNLOAD_SYNC:
        if (0U != dfu->data_len) {
            dfu->bState = STATE_DFU_DNBUSY;

            if (0U == dfu->block_num) {
                if (ERASE == dfu->buf[0]) {
                    dfu_mal_getstatus (dfu->base_addr, CMD_ERASE, (uint8_t *)&dfu->bwPollTimeout0);
                } else {
                    dfu_mal_getstatus (dfu->base_addr, CMD_WRITE, (uint8_t *)&dfu->bwPollTimeout0);
                }
            }
        } else {
            dfu->bState = STATE_DFU_DNLOAD_IDLE;
        }
        break;

    case STATE_DFU_MANIFEST_SYNC:
        if (MANIFEST_IN_PROGRESS == dfu->manifest_state) {
            dfu->bState = STATE_DFU_MANIFEST;
            dfu->bwPollTimeout0 = 1U;
        } else if ((MANIFEST_COMPLETE == dfu->manifest_state) && \
            (dfu_config_desc.dfu_func.bmAttributes & 0x04U)){
            dfu->bState = STATE_DFU_IDLE;
            dfu->bwPollTimeout0 = 0U;
        } else {
            /* no operation */
        }
        break;

    default:
        break;
    }

    /* send the status data of DFU interface to host over EP0 */
    transc->xfer_buf = (uint8_t *)&(dfu->bStatus);
    transc->remain_len = 6U;
}

/*!
    \brief      handle the DFU_CLRSTATUS request
    \param      udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void  dfu_clrstatus (usb_dev *udev, usb_req *req)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    if (STATE_DFU_ERROR == dfu->bState) {
        dfu->bStatus = STATUS_OK;
        dfu->bState = STATE_DFU_IDLE;
    } else {
        /* state error */
        dfu->bStatus = STATUS_ERR_UNKNOWN;
        dfu->bState = STATE_DFU_ERROR;
    }

    dfu->iString = 0U; /* iString: index = 0 */
}

/*!
    \brief      handle the DFU_GETSTATE request
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void  dfu_getstate (usb_dev *udev, usb_req *req)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    usb_transc *transc = &udev->dev.transc_in[0];

    /* send the current state of the DFU interface to host */
    transc->xfer_buf = &(dfu->bState);
    transc->remain_len = 1U;
}

/*!
    \brief      handle the DFU_ABORT request
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void dfu_abort (usb_dev *udev, usb_req *req)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->dev.class_data[USBD_DFU_INTERFACE];

    switch (dfu->bState){
    case STATE_DFU_IDLE:
    case STATE_DFU_DNLOAD_SYNC:
    case STATE_DFU_DNLOAD_IDLE:
    case STATE_DFU_MANIFEST_SYNC:
    case STATE_DFU_UPLOAD_IDLE:
        dfu->bStatus = STATUS_OK;
        dfu->bState = STATE_DFU_IDLE;
        dfu->iString = 0U; /* iString: index = 0 */

        dfu->block_num = 0U;
        dfu->data_len = 0U;
        break;

    default:
        break;
    }
}
