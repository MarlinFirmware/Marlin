/*!
    \file    printer_core.c
    \brief   USB printer device class core functions

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

#include "printer_core.h"

#define USBD_VID                     0x28E9U
#define USBD_PID                     0x028DU

/* printer port status: paper not empty/selected/no error */
static uint8_t g_port_status = 0x18U;

uint8_t g_printer_data_buf[PRINTER_OUT_PACKET];

__ALIGN_BEGIN uint8_t PRINTER_DEVICE_ID[DEVICE_ID_LEN] __ALIGN_END =
{
    0x00, 0x67,
    'M', 'A', 'N', 'U', 'F', 'A', 'C', 'T', 'U', 'R', 'E', 'R', ':',
    'G', 'I', 'G', 'A', ' ', 'D', 'E', 'V', 'I', 'C', 'E', '-', ';',
    'C', 'O', 'M', 'M', 'A', 'N', 'D', ' ', 'S', 'E', 'T', ':',
    'P', 'C', 'L', ',', 'M', 'P', 'L', ';',
    'M', 'O', 'D', 'E', 'L', ':',
    'L', 'a', 's', 'e', 'r', 'B', 'e', 'a', 'm', '?', ';',
    'C', 'O', 'M', 'M', 'E', 'N', 'T', ':',
    'G', 'o', 'o', 'd', ' ', '!', ';',
    'A', 'C', 'T', 'I', 'V', 'E', ' ', 'C', 'O', 'M', 'M', 'A', 'N', 'D', ' ', 'S', 'E', 'T', ':',
    'P', 'C', 'L', ';'
};

/* USB standard device descriptor */
__ALIGN_BEGIN const usb_desc_dev printer_dev_desc __ALIGN_END =
{
    .header = 
     {
         .bLength          = USB_DEV_DESC_LEN, 
         .bDescriptorType  = USB_DESCTYPE_DEV,
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
    .bNumberConfigurations = USBD_CFG_MAX_NUM,
};

/* USB device configuration descriptor */
__ALIGN_BEGIN const usb_printer_desc_config_set printer_config_desc __ALIGN_END = 
{
    .config = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_config), 
             .bDescriptorType = USB_DESCTYPE_CONFIG
         },
        .wTotalLength         = USB_PRINTER_CONFIG_DESC_LEN,
        .bNumInterfaces       = 0x01U,
        .bConfigurationValue  = 0x01U,
        .iConfiguration       = 0x00U,
        .bmAttributes         = 0xA0U,
        .bMaxPower            = 0x32U
    },

    .printer_itf = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_itf), 
             .bDescriptorType =  USB_DESCTYPE_ITF 
         },
        .bInterfaceNumber     = 0x00U,
        .bAlternateSetting    = 0x00U,
        .bNumEndpoints        = 0x02U,
        .bInterfaceClass      = USB_CLASS_PRINTER,
        .bInterfaceSubClass   = USB_SUBCLASS_PRINTER,
        .bInterfaceProtocol   = PROTOCOL_BI_DIRECTIONAL_ITF,
        .iInterface           = 0x00U
    },

    .printer_epin = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_ep), 
             .bDescriptorType = USB_DESCTYPE_EP 
         },
        .bEndpointAddress     = PRINTER_IN_EP,
        .bmAttributes         = USB_EP_ATTR_BULK,
        .wMaxPacketSize       = PRINTER_IN_PACKET,
        .bInterval            = 0x00U
    },

    .printer_epout = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_ep), 
             .bDescriptorType = USB_DESCTYPE_EP 
         },
        .bEndpointAddress     = PRINTER_OUT_EP,
        .bmAttributes         = USB_EP_ATTR_BULK,
        .wMaxPacketSize       = PRINTER_OUT_PACKET,
        .bInterval            = 0x00U
    },
};

/* USB language ID Descriptor */
static __ALIGN_BEGIN const usb_desc_LANGID usbd_language_id_desc __ALIGN_END = 
{
    .header = 
     {
         .bLength         = sizeof(usb_desc_LANGID), 
         .bDescriptorType = USB_DESCTYPE_STR,
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
         .bLength         = USB_STRING_LEN(16U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', '-', 'U', 'S', 'B', '_', 'P', 'r', 'i', 'n', 't', 'e', 'r'}
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

/* USB string descriptor */
static void *const usbd_msc_strings[] = 
{
    [STR_IDX_LANGID]  = (uint8_t *)&usbd_language_id_desc,
    [STR_IDX_MFC]     = (uint8_t *)&manufacturer_string,
    [STR_IDX_PRODUCT] = (uint8_t *)&product_string,
    [STR_IDX_SERIAL]  = (uint8_t *)&serial_string
};

usb_desc printer_desc = {
    .dev_desc    = (uint8_t *)&printer_dev_desc,
    .config_desc = (uint8_t *)&printer_config_desc,
    .strings     = usbd_msc_strings
};

/* local function prototypes ('static') */
static uint8_t printer_init         (usb_dev *udev, uint8_t config_index);
static uint8_t printer_deinit       (usb_dev *udev, uint8_t config_index);
static uint8_t printer_req          (usb_dev *udev, usb_req *req);
static uint8_t printer_in           (usb_dev *udev, uint8_t ep_num);
static uint8_t printer_out          (usb_dev *udev, uint8_t ep_num);

usb_class_core usbd_printer_cb = {
    .init          = printer_init,
    .deinit        = printer_deinit,

    .req_proc      = printer_req,

    .data_in       = printer_in,
    .data_out      = printer_out
};

/*!
    \brief      initialize the printer device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t printer_init (usb_dev *udev, uint8_t config_index)
{
    /* initialize the data TX endpoint */
    usbd_ep_setup (udev, &(printer_config_desc.printer_epin));

    /* initialize the data RX endpoint */
    usbd_ep_setup (udev, &(printer_config_desc.printer_epout));

    /* prepare to receive data */
    usbd_ep_recev(udev, PRINTER_OUT_EP, g_printer_data_buf, PRINTER_OUT_PACKET);

    return USBD_OK;
}

/*!
    \brief      deinitialize the printer device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t printer_deinit (usb_dev *udev, uint8_t config_index)
{
    /* deinitialize the data TX/RX endpoint */
    usbd_ep_clear (udev, PRINTER_IN_EP);
    usbd_ep_clear (udev, PRINTER_OUT_EP);

    return USBD_OK;
}

/*!
    \brief      handle the printer class-specific requests
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: device class-specific request
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t printer_req(usb_dev *udev, usb_req *req)
{
    usb_transc *transc = &udev->dev.transc_in[0];

    switch (req->bRequest) {
    case GET_DEVICE_ID:
        transc->xfer_buf = (uint8_t *)PRINTER_DEVICE_ID;
        transc->remain_len = DEVICE_ID_LEN;
        break;

    case GET_PORT_STATUS:
        transc->xfer_buf = (uint8_t *)&g_port_status;
        transc->remain_len = 1U;
        break;

    case SOFT_RESET:
        usbd_ep_recev(udev, PRINTER_OUT_EP, g_printer_data_buf, PRINTER_OUT_PACKET);
        break;

    default:
        return USBD_FAIL; 
    }

    return USBD_OK;
}

/*!
    \brief      handle printer data
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint number
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t  printer_in (usb_dev *udev, uint8_t ep_num)
{
     return USBD_OK;
}

/*!
    \brief      handle printer data
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint number
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t  printer_out (usb_dev *udev, uint8_t ep_num)
{
     return USBD_OK;
}
