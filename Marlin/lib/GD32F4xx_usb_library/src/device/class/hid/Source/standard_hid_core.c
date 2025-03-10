/*!
    \file    standard_hid_core.c
    \brief   HID class driver

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

#include "standard_hid_core.h"
#include <string.h>

#define USBD_VID                     0x28e9U
#define USBD_PID                     0x0380U

/* Note:it should use the C99 standard when compiling the below codes */
/* USB standard device descriptor */
__ALIGN_BEGIN const usb_desc_dev hid_dev_desc __ALIGN_END =
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

__ALIGN_BEGIN const usb_hid_desc_config_set hid_config_desc __ALIGN_END = 
{
    .config = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_config), 
             .bDescriptorType = USB_DESCTYPE_CONFIG 
         },
        .wTotalLength         = USB_HID_CONFIG_DESC_LEN,
        .bNumInterfaces       = 0x01U,
        .bConfigurationValue  = 0x01U,
        .iConfiguration       = 0x00U,
        .bmAttributes         = 0xA0U,
        .bMaxPower            = 0x32U
    },

    .hid_itf = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_itf), 
             .bDescriptorType = USB_DESCTYPE_ITF
         },
        .bInterfaceNumber     = 0x00U,
        .bAlternateSetting    = 0x00U,
        .bNumEndpoints        = 0x01U,
        .bInterfaceClass      = USB_HID_CLASS,
        .bInterfaceSubClass   = USB_HID_SUBCLASS_BOOT_ITF,
        .bInterfaceProtocol   = USB_HID_PROTOCOL_KEYBOARD,
        .iInterface           = 0x00U
    },

    .hid_vendor = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_hid), 
             .bDescriptorType = USB_DESCTYPE_HID 
         },
        .bcdHID               = 0x0111U,
        .bCountryCode         = 0x00U,
        .bNumDescriptors      = 0x01U,
        .bDescriptorType      = USB_DESCTYPE_REPORT,
        .wDescriptorLength    = USB_HID_REPORT_DESC_LEN,
    },

    .hid_epin = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_ep), 
             .bDescriptorType = USB_DESCTYPE_EP
         },
        .bEndpointAddress     = HID_IN_EP,
        .bmAttributes         = USB_EP_ATTR_INT,
        .wMaxPacketSize       = HID_IN_PACKET,
        .bInterval            = 0x40U
    }
};

/* USB language ID Descriptor */
static __ALIGN_BEGIN const usb_desc_LANGID usbd_language_id_desc __ALIGN_END = 
{
    .header = 
     {
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
         .bLength         = USB_STRING_LEN(17U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', '-','U', 'S', 'B', '_', 'K', 'e', 'y', 'b', 'o', 'a', 'r', 'd'}
};

/* USBD serial string */
static __ALIGN_BEGIN usb_desc_str serial_string __ALIGN_END = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(12U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     }
};

void *const usbd_hid_strings[] = 
{
    [STR_IDX_LANGID]  = (uint8_t *)&usbd_language_id_desc,
    [STR_IDX_MFC]     = (uint8_t *)&manufacturer_string,
    [STR_IDX_PRODUCT] = (uint8_t *)&product_string,
    [STR_IDX_SERIAL]  = (uint8_t *)&serial_string
};

usb_desc hid_desc = {
    .dev_desc    = (uint8_t *)&hid_dev_desc,
    .config_desc = (uint8_t *)&hid_config_desc,
    .strings     = usbd_hid_strings
};

__ALIGN_BEGIN const uint8_t hid_report_desc[USB_HID_REPORT_DESC_LEN] __ALIGN_END =
{
    0x05, 0x01,  /* USAGE_PAGE (Generic Desktop) */
    0x09, 0x06,  /* USAGE (Keyboard) */
    0xa1, 0x01,  /* COLLECTION (Application) */

    0x05, 0x07,  /* USAGE_PAGE (Keyboard/Keypad) */
    0x19, 0xe0,  /* USAGE_MINIMUM (Keyboard LeftControl) */
    0x29, 0xe7,  /* USAGE_MAXIMUM (Keyboard Right GUI) */
    0x15, 0x00,  /* LOGICAL_MINIMUM (0) */
    0x25, 0x01,  /* LOGICAL_MAXIMUM (1) */
    0x95, 0x08,  /* REPORT_COUNT (8) */
    0x75, 0x01,  /* REPORT_SIZE (1) */
    0x81, 0x02,  /* INPUT (Data,Var,Abs) */

    0x95, 0x01,  /* REPORT_COUNT (1) */
    0x75, 0x08,  /* REPORT_SIZE (8) */
    0x81, 0x03,  /* INPUT (Cnst,Var,Abs) */

    0x95, 0x06,  /* REPORT_COUNT (6) */
    0x75, 0x08,  /* REPORT_SIZE (8) */
    0x25, 0xFF,  /* LOGICAL_MAXIMUM (255) */
    0x19, 0x00,  /* USAGE_MINIMUM (Reserved (no event indicated)) */
    0x29, 0x65,  /* USAGE_MAXIMUM (Keyboard Application) */
    0x81, 0x00,  /* INPUT (Data,Ary,Abs) */

    0xc0         /* END_COLLECTION */
};

/* local function prototypes ('static') */
static uint8_t hid_init    (usb_dev *udev, uint8_t config_index);
static uint8_t hid_deinit  (usb_dev *udev, uint8_t config_index);
static uint8_t hid_req     (usb_dev *udev, usb_req *req);
static uint8_t hid_data_in (usb_dev *udev, uint8_t ep_num);

usb_class_core usbd_hid_cb = {
    .command         = NO_CMD,
    .alter_set       = 0U,

    .init            = hid_init,
    .deinit          = hid_deinit,
    .req_proc        = hid_req,
    .data_in         = hid_data_in
};

/*!
    \brief      register HID interface operation functions
    \param[in]  udev: pointer to USB device instance
    \param[in]  hid_fop: HID operation function structure
    \param[out] none
    \retval     USB device operation status
*/
uint8_t hid_itfop_register (usb_dev *udev, hid_fop_handler *hid_fop)
{
    if (NULL != hid_fop) {
        udev->dev.user_data = (void *)hid_fop;

        return USBD_OK;
    }

    return USBD_FAIL;
}

/*!
    \brief      send keyboard report
    \param[in]  udev: pointer to USB device instance
    \param[in]  report: pointer to HID report
    \param[in]  len: data length
    \param[out] none
    \retval     USB device operation status
*/
uint8_t hid_report_send (usb_dev *udev, uint8_t *report, uint32_t len)
{
    standard_hid_handler *hid = (standard_hid_handler *)udev->dev.class_data[USBD_HID_INTERFACE];

    hid->prev_transfer_complete = 0U;

    usbd_ep_send(udev, HID_IN_EP, report, len);

    return USBD_OK;
}

/*!
    \brief      initialize the HID device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_init (usb_dev *udev, uint8_t config_index)
{
    static __ALIGN_BEGIN standard_hid_handler hid_handler __ALIGN_END;

    memset((void *)&hid_handler, 0U, sizeof(standard_hid_handler));

    /* initialize the data TX endpoint */
    usbd_ep_setup (udev, &(hid_config_desc.hid_epin));

    hid_handler.prev_transfer_complete = 1U;

    udev->dev.class_data[USBD_HID_INTERFACE] = (void *)&hid_handler;

    if (NULL != udev->dev.user_data) {
        ((hid_fop_handler *)udev->dev.user_data)->hid_itf_config();
    }

    return USBD_OK;
}

/*!
    \brief      de-initialize the HID device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_deinit (usb_dev *udev, uint8_t config_index)
{
    /* deinitialize HID endpoints */
    usbd_ep_clear(udev, HID_IN_EP);

    return USBD_OK;
}

/*!
    \brief      handle the HID class-specific requests
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: device class-specific request
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_req (usb_dev *udev, usb_req *req)
{
    usb_transc *transc = &udev->dev.transc_in[0];

    standard_hid_handler *hid = (standard_hid_handler *)udev->dev.class_data[USBD_HID_INTERFACE];

    switch (req->bRequest) {
    case GET_REPORT:
        /* no use for this driver */
        break;

    case GET_IDLE:
        transc->xfer_buf = (uint8_t *)&hid->idle_state;

        transc->remain_len = 1U;
        break;

    case GET_PROTOCOL:
        transc->xfer_buf = (uint8_t *)&hid->protocol;

        transc->remain_len = 1U;
        break;

    case SET_REPORT:
        /* no use for this driver */
        break;

    case SET_IDLE:
        hid->idle_state = (uint8_t)(req->wValue >> 8U);
        break;

    case SET_PROTOCOL:
        hid->protocol = (uint8_t)(req->wValue);
        break;

    case USB_GET_DESCRIPTOR:
        if (USB_DESCTYPE_REPORT == (req->wValue >> 8U)) {
            transc->remain_len = USB_MIN(USB_HID_REPORT_DESC_LEN, req->wLength);
            transc->xfer_buf = (uint8_t *)hid_report_desc;

            return REQ_SUPP;
        }
        break;

    default:
        break;
    }

    return USBD_OK;
}

/*!
    \brief      handle data stage
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint identifier
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t hid_data_in (usb_dev *udev, uint8_t ep_num)
{
    standard_hid_handler *hid = (standard_hid_handler *)udev->dev.class_data[USBD_HID_INTERFACE];

    if (0U != hid->data[2]) {
        hid->data[2] = 0x00U;

        usbd_ep_send(udev, HID_IN_EP, hid->data, HID_IN_PACKET);
    } else {
        hid->prev_transfer_complete = 1U;
    }

    return USBD_OK;
}
