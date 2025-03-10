/*!
    \file    usb_ch9_std.h
    \brief   USB 2.0 standard defines

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

#ifndef __USB_CH9_STD_H
#define __USB_CH9_STD_H

#include "gdusb_conf.h"

#define USB_DEV_QUALIFIER_DESC_LEN   0x0AU       /*!< USB device qualifier descriptor length */
#define USB_DEV_DESC_LEN             0x12U       /*!< USB device descriptor length */
#define USB_CFG_DESC_LEN             0x09U       /*!< USB configuration descriptor length */
#define USB_ITF_DESC_LEN             0x09U       /*!< USB interface descriptor length */
#define USB_EP_DESC_LEN              0x07U       /*!< USB endpoint descriptor length */
#define USB_IAD_DESC_LEN             0x08U       /*!< USB IAD descriptor length */
#define USB_OTG_DESC_LEN             0x03U       /*!< USB device OTG descriptor length */

#define USB_SETUP_PACKET_LEN         0x08U       /*!< USB setup packet length */

/* bit 7 of bmRequestType: data phase transfer direction */
#define USB_TRX_MASK                 0x80U       /*!< USB transfer direction mask */
#define USB_TRX_OUT                  0x00U       /*!< USB transfer OUT direction */
#define USB_TRX_IN                   0x80U       /*!< USB transfer IN direction */

/* bit 6..5 of bmRequestType: request type */
#define USB_REQTYPE_STRD             0x00U       /*!< USB standard request */
#define USB_REQTYPE_CLASS            0x20U       /*!< USB class request */
#define USB_REQTYPE_VENDOR           0x40U       /*!< USB vendor request */
#define USB_REQTYPE_MASK             0x60U       /*!< USB request mask */

#define USBD_BUS_POWERED             0x00U       /*!< USB bus power supply */
#define USBD_SELF_POWERED            0x01U       /*!< USB self power supply */

#define USB_STATUS_REMOTE_WAKEUP     2U          /*!< USB is in remote wakeup status */
#define USB_STATUS_SELF_POWERED      1U          /*!< USB is in self powered status */

/* bit 4..0 of bmRequestType: recipient type */
enum _usb_recp_type {
    USB_RECPTYPE_DEV  = 0x0U,                    /*!< USB device request type */
    USB_RECPTYPE_ITF  = 0x1U,                    /*!< USB interface request type */
    USB_RECPTYPE_EP   = 0x2U,                    /*!< USB endpoint request type */
    USB_RECPTYPE_MASK = 0x3U                     /*!< USB request type mask */
};

/* bRequest value */
enum _usb_request {
    USB_GET_STATUS        = 0x0U,                /*!< USB get status request */
    USB_CLEAR_FEATURE     = 0x1U,                /*!< USB clear feature request */
    USB_RESERVED2         = 0x2U,
    USB_SET_FEATURE       = 0x3U,                /*!< USB set feature request */
    USB_RESERVED4         = 0x4U,
    USB_SET_ADDRESS       = 0x5U,                /*!< USB set address request */
    USB_GET_DESCRIPTOR    = 0x6U,                /*!< USB get descriptor request */
    USB_SET_DESCRIPTOR    = 0x7U,                /*!< USB set descriptor request */
    USB_GET_CONFIGURATION = 0x8U,                /*!< USB get configuration request */
    USB_SET_CONFIGURATION = 0x9U,                /*!< USB set configuration request */
    USB_GET_INTERFACE     = 0xAU,                /*!< USB get interface request */
    USB_SET_INTERFACE     = 0xBU,                /*!< USB set interface request */
    USB_SYNCH_FRAME       = 0xCU                 /*!< USB synchronize frame request */
};

/* descriptor types of USB specifications */
enum _usb_desctype {
    USB_DESCTYPE_DEV              = 0x1U,        /*!< USB device descriptor type */
    USB_DESCTYPE_CONFIG           = 0x2U,        /*!< USB configuration descriptor type */
    USB_DESCTYPE_STR              = 0x3U,        /*!< USB string descriptor type */
    USB_DESCTYPE_ITF              = 0x4U,        /*!< USB interface descriptor type */
    USB_DESCTYPE_EP               = 0x5U,        /*!< USB endpoint descriptor type */
    USB_DESCTYPE_DEV_QUALIFIER    = 0x6U,        /*!< USB device qualifier descriptor type */
    USB_DESCTYPE_OTHER_SPD_CONFIG = 0x7U,        /*!< USB other speed configuration descriptor type */
    USB_DESCTYPE_ITF_POWER        = 0x8U,        /*!< USB interface power descriptor type */
    USB_DESCTYPE_IAD              = 0xBU,        /*!< USB interface association descriptor type */
    USB_DESCTYPE_BOS              = 0xFU         /*!< USB BOS descriptor type  */
};

/* USB Endpoint Descriptor bmAttributes bit definitions */
/* bits 1..0 : transfer type */
enum _usbx_type {
    USB_EP_ATTR_CTL  = 0x0U,                     /*!< USB control transfer type */
    USB_EP_ATTR_ISO  = 0x1U,                     /*!< USB Isochronous transfer type */
    USB_EP_ATTR_BULK = 0x2U,                     /*!< USB Bulk transfer type */
    USB_EP_ATTR_INT  = 0x3U                      /*!< USB Interrupt transfer type */
};

/* bits 3..2 : Sync type (only if ISOCHRONOUS) */
#define USB_EP_ATTR_NOSYNC                  0x00U     /*!< No Synchronization */
#define USB_EP_ATTR_ASYNC                   0x04U     /*!< Asynchronous */
#define USB_EP_ATTR_ADAPTIVE                0x08U     /*!< Adaptive */
#define USB_EP_ATTR_SYNC                    0x0CU     /*!< Synchronous */
#define USB_EP_ATTR_SYNCTYPE                0x0CU     /*!< Synchronous type */

/* bits 5..4 : usage type (only if ISOCHRONOUS) */
#define USB_EP_ATTR_DATA                    0x00U     /*!< Data endpoint */
#define USB_EP_ATTR_FEEDBACK                0x10U     /*!< Feedback endpoint */
#define USB_EP_ATTR_IMPLICIT_FEEDBACK_DATA  0x20U     /*!< Implicit feedback Data endpoint */
#define USB_EP_ATTR_USAGETYPE               0x30U     /*!< Usage type */

/* endpoint max packet size bits12..11 */
#define USB_EP_MPS_ADD_0                    (0x00 << 11) /*!< None(1 transaction per microframe */
#define USB_EP_MPS_ADD_1                    (0x01 << 11) /*!< 1 additional(2 transaction per microframe */
#define USB_EP_MPS_ADD_2                    (0x02 << 11) /*!< 2 additional(3 transaction per microframe */

#define FEATURE_SELECTOR_EP                 0x00U        /*!< USB endpoint feature selector */
#define FEATURE_SELECTOR_DEV                0x01U        /*!< USB device feature selector */
#define FEATURE_SELECTOR_REMOTEWAKEUP       0x01U        /*!< USB feature selector remote wakeup */

#define BYTE_SWAP(addr)      (((uint16_t)(*((uint8_t *)(addr)))) + \
                             (uint16_t)(((uint16_t)(*(((uint8_t *)(addr)) + 1U))) << 8U))

#define BYTE_LOW(x)          ((uint8_t)((x) & 0x00FFU))
#define BYTE_HIGH(x)         ((uint8_t)(((x) & 0xFF00U) >> 8U))

#define USB_MIN(a, b)        (((a) < (b)) ? (a) : (b))

#define USB_DEFAULT_CONFIG                  0U

/* USB classes */
#define USB_CLASS_HID                       0x03U       /*!< USB HID class */
#define USB_CLASS_MSC                       0x08U       /*!< USB MSC class */

/* use the following values when USB host need to get descriptor  */
#define USBH_DESC(x)                        (((x)<< 8U) & 0xFF00U)

/* as per USB specs 9.2.6.4 :standard request with data request timeout: 5sec
   standard request with no data stage timeout : 50ms */
#define DATA_STAGE_TIMEOUT                  5000U       /*!< USB data stage timeout*/
#define NODATA_STAGE_TIMEOUT                50U         /*!< USB no data stage timeout*/

#pragma pack(1)

/* USB standard device request structure */
typedef struct _usb_req {
    uint8_t           bmRequestType;  /*!< type of request */
    uint8_t           bRequest;       /*!< request of setup packet */
    uint16_t          wValue;         /*!< value of setup packet */
    uint16_t          wIndex;         /*!< index of setup packet */
    uint16_t          wLength;        /*!< length of setup packet */
} usb_req;

/* USB setup packet define */
typedef union _usb_setup {
    uint8_t data[8];

    usb_req req;
} usb_setup;

/* USB descriptor defines */

typedef struct _usb_desc_header {
    uint8_t bLength;                      /*!< size of the descriptor */
    uint8_t bDescriptorType;              /*!< type of the descriptor */
} usb_desc_header;

typedef struct _usb_desc_dev {
    usb_desc_header header;               /*!< descriptor header, including type and size */

    uint16_t bcdUSB;                      /*!< BCD of the supported USB specification */
    uint8_t  bDeviceClass;                /*!< USB device class */
    uint8_t  bDeviceSubClass;             /*!< USB device subclass */
    uint8_t  bDeviceProtocol;             /*!< USB device protocol */
    uint8_t  bMaxPacketSize0;             /*!< size of the control (address 0) endpoint's bank in bytes */
    uint16_t idVendor;                    /*!< vendor ID for the USB product */
    uint16_t idProduct;                   /*!< unique product ID for the USB product */
    uint16_t bcdDevice;                   /*!< product release (version) number */
    uint8_t  iManufacturer;               /*!< string index for the manufacturer's name */
    uint8_t  iProduct;                    /*!< string index for the product name/details */
    uint8_t  iSerialNumber;               /*!< string index for the product's globally unique hexadecimal serial number */
    uint8_t  bNumberConfigurations;       /*!< total number of configurations supported by the device */
} usb_desc_dev;

typedef struct _usb_desc_config {
    usb_desc_header header;               /*!< descriptor header, including type and size */

    uint16_t wTotalLength;                /*!< size of the configuration descriptor header,and all sub descriptors inside the configuration */
    uint8_t  bNumInterfaces;              /*!< total number of interfaces in the configuration */
    uint8_t  bConfigurationValue;         /*!< configuration index of the current configuration */
    uint8_t  iConfiguration;              /*!< index of a string descriptor describing the configuration */
    uint8_t  bmAttributes;                /*!< configuration attributes */
    uint8_t  bMaxPower;                   /*!< maximum power consumption of the device while in the current configuration */
} usb_desc_config;

typedef struct _usb_desc_itf {
    usb_desc_header header;               /*!< descriptor header, including type and size */

    uint8_t bInterfaceNumber;             /*!< index of the interface in the current configuration */
    uint8_t bAlternateSetting;            /*!< alternate setting for the interface number */
    uint8_t bNumEndpoints;                /*!< total number of endpoints in the interface */
    uint8_t bInterfaceClass;              /*!< interface class ID */
    uint8_t bInterfaceSubClass;           /*!< interface subclass ID */
    uint8_t bInterfaceProtocol;           /*!< interface protocol ID */
    uint8_t iInterface;                   /*!< index of the string descriptor describing the interface */
} usb_desc_itf;

typedef struct _usb_desc_ep {
    usb_desc_header header;               /*!< descriptor header, including type and size. */

    uint8_t  bEndpointAddress;            /*!< logical address of the endpoint */
    uint8_t  bmAttributes;                /*!< endpoint attributes */
    uint16_t wMaxPacketSize;              /*!< size of the endpoint bank, in bytes */
    uint8_t  bInterval;                   /*!< polling interval in milliseconds for the endpoint if it is an INTERRUPT or ISOCHRONOUS type */
} usb_desc_ep;

typedef struct _usb_desc_LANGID {
    usb_desc_header header;               /*!< descriptor header, including type and size. */
    uint16_t wLANGID;                     /*!< LANGID code */
} usb_desc_LANGID;

typedef struct _usb_desc_str {
    usb_desc_header header;               /*!< descriptor header, including type and size. */
    uint16_t unicode_string[64];          /*!< unicode string data */
} usb_desc_str;

#pragma pack()

/* compute string descriptor length */
#define USB_STRING_LEN(unicode_chars) (sizeof(usb_desc_header) + ((unicode_chars) << 1U))

#endif /* __USB_CH9_STD_H */
