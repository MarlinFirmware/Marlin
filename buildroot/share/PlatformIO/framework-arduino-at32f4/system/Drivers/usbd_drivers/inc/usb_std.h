/**
  **************************************************************************
  * @file     usb_std.h
  * @brief    usb standard header file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_STD_H
#define __USB_STD_H

#ifdef __cplusplus
extern "C" {
#endif


/* includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_drivers
  * @{
  */

/** @addtogroup USB_standard
  * @{
  */

/** @defgroup USB_standard_define
  * @{
  */

/**
  * @brief usb request recipient
  */
#define USB_REQ_RECIPIENT_DEVICE         0x00 /*!< usb request recipient device */
#define USB_REQ_RECIPIENT_INTERFACE      0x01 /*!< usb request recipient interface */
#define USB_REQ_RECIPIENT_ENDPOINT       0x02 /*!< usb request recipient endpoint */
#define USB_REQ_RECIPIENT_OTHER          0x03 /*!< usb request recipient other */
#define USB_REQ_RECIPIENT_MASK           0x1F /*!< usb request recipient mask */

/**
  * @brief usb request type
  */
#define USB_REQ_TYPE_STANDARD            0x00 /*!< usb request type standard */
#define USB_REQ_TYPE_CLASS               0x20 /*!< usb request type class */
#define USB_REQ_TYPE_VENDOR              0x40 /*!< usb request type vendor */
#define USB_REQ_TYPE_RESERVED            0x60 /*!< usb request type reserved */

/**
  * @brief usb request data transfer direction
  */
#define USB_REQ_DIR_HTD                  0x00 /*!< usb request data transfer direction host to device */
#define USB_REQ_DIR_DTH                  0x80 /*!< usb request data transfer direction device to host */

/**
  * @brief usb standard device requests codes
  */
#define USB_STD_REQ_GET_STATUS           0 /*!< usb request code status */
#define USB_STD_REQ_CLEAR_FEATURE        1 /*!< usb request code clear feature */
#define USB_STD_REQ_SET_FEATURE          3 /*!< usb request code feature */
#define USB_STD_REQ_SET_ADDRESS          5 /*!< usb request code address */
#define USB_STD_REQ_GET_DESCRIPTOR       6 /*!< usb request code get descriptor */
#define USB_STD_REQ_SET_DESCRIPTOR       7 /*!< usb request code set descriptor */
#define USB_STD_REQ_GET_CONFIGURATION    8 /*!< usb request code get configuration */
#define USB_STD_REQ_SET_CONFIGURATION    9 /*!< usb request code set configuration */
#define USB_STD_REQ_GET_INTERFACE        10 /*!< usb request code get interface */
#define USB_STD_REQ_SET_INTERFACE        11 /*!< usb request code set interface */
#define USB_STD_REQ_SYNCH_FRAME          12 /*!< usb request code synch frame */

/**
  * @brief usb standard device type
  */
#define USB_DESCIPTOR_TYPE_DEVICE        1 /*!< usb standard device type device */
#define USB_DESCIPTOR_TYPE_CONFIGURATION 2 /*!< usb standard device type configuration */
#define USB_DESCIPTOR_TYPE_STRING        3 /*!< usb standard device type string */
#define USB_DESCIPTOR_TYPE_INTERFACE     4 /*!< usb standard device type interface */
#define USB_DESCIPTOR_TYPE_ENDPOINT      5 /*!< usb standard device type endpoint */
#define USB_DESCIPTOR_TYPE_DEVICE_QUALIFIER     6 /*!< usb standard device type qualifier */
#define USB_DESCIPTOR_TYPE_OTHER_SPEED   7 /*!< usb standard device type other speed */
#define USB_DESCIPTOR_TYPE_INTERFACE_POWER       8 /*!< usb standard device type interface power */

/**
  * @brief usb standard string type
  */
#define  USB_LANGID_STRING               0 /*!< usb standard string type lang id */
#define  USB_MFC_STRING                  1 /*!< usb standard string type mfc */
#define  USB_PRODUCT_STRING              2 /*!< usb standard string type product */
#define  USB_SERIAL_STRING               3 /*!< usb standard string type serial */
#define  USB_CONFIG_STRING               4 /*!< usb standard string type config */
#define  USB_INTERFACE_STRING            5 /*!< usb standard string type interface */
#define  USB_WINUSB_OS_STRING            0xEE /*!< usb microsoft os string */

/**
  * @brief usb configuration attributes
  */
#define USB_CONF_REMOTE_WAKEUP           2 /*!< usb configuration attributes remote wakeup */
#define USB_CONF_SELF_POWERED            1 /*!< usb configuration attributes self powered */

/**
  * @brief usb standard feature selectors
  */
#define USB_FEATURE_EPT_HALT             0 /*!< usb standard feature selectors endpoint halt */
#define USB_FEATURE_REMOTE_WAKEUP        1 /*!< usb standard feature selectors remote wakeup */
#define USB_FEATURE_TEST_MODE            2 /*!< usb standard feature selectors test mode */

/**
  * @brief usb winusb feature id
  */
#define USB_WINUSB_COMPAT_ID            0x04 /*!< usb winusb compat id os descriptor */
#define USB_WINUSB_PROPERTIES_ID         0x05 /*!< usb winusb properties id os descriptor */

/**
  * @brief usb device connect state
  */
typedef enum
{
  USB_CONN_STATE_DEFAULT                =1, /*!< usb device connect state default */
  USB_CONN_STATE_ADDRESSED,                 /*!< usb device connect state address */
  USB_CONN_STATE_CONFIGURED,                /*!< usb device connect state configured */
  USB_CONN_STATE_SUSPENDED                  /*!< usb device connect state suspend */
}usbd_conn_state;

/**
  * @brief endpoint 0 state
  */
#define USB_EPT0_IDLE                    0 /*!< usb endpoint state idle */
#define USB_EPT0_SETUP                   1 /*!< usb endpoint state setup */
#define USB_EPT0_DATA_IN                 2 /*!< usb endpoint state data in */
#define USB_EPT0_DATA_OUT                3 /*!< usb endpoint state data out */
#define USB_EPT0_STATUS_IN               4 /*!< usb endpoint state status in */
#define USB_EPT0_STATUS_OUT              5 /*!< usb endpoint state status out */
#define USB_EPT0_STALL                   6 /*!< usb endpoint state stall */

/**
  * @brief usb descriptor length
  */
#define USB_DEVICE_QUALIFIER_DESC_LEN    0x0A /*!< usb qualifier descriptor length */
#define USB_DEVICE_DESC_LEN              0x12 /*!< usb device descriptor length */
#define USB_DEVICE_CFG_DESC_LEN          0x09 /*!< usb configuration descriptor length */
#define USB_DEVICE_IF_DESC_LEN           0x09 /*!< usb interface descriptor length */
#define USB_DEVICE_EPT_LEN               0x07 /*!< usb endpoint descriptor length */
#define USB_DEVICE_OTG_DESC_LEN          0x03 /*!< usb otg descriptor length */
#define USB_DEVICE_LANGID_STR_DESC_LEN   0x04 /*!< usb lang id string descriptor length */
#define USB_DEVICE_OTHER_SPEED_DESC_SIZ_LEN 0x09 /*!< usb other speed descriptor length */

/**
  * @brief usb class code
  */
#define USB_CLASS_CODE_AUDIO             0x01 /*!< usb class code audio */
#define USB_CLASS_CODE_CDC               0x02 /*!< usb class code cdc */
#define USB_CLASS_CODE_HID               0x03 /*!< usb class code hid */
#define USB_CLASS_CODE_PRINTER           0x07 /*!< usb class code printer */
#define USB_CLASS_CODE_MSC               0x08 /*!< usb class code msc */
#define USB_CLASS_CODE_HUB               0x09 /*!< usb class code hub */
#define USB_CLASS_CODE_CDCDATA           0x0A /*!< usb class code cdc data */
#define USB_CLASS_CODE_CCID              0x0B /*!< usb class code ccid */
#define USB_CLASS_CODE_VIDEO             0x0E /*!< usb class code video */
#define USB_CLASS_CODE_VENDOR            0xFF /*!< usb class code vendor */

/**
  * @brief usb endpoint type
  */
#define USB_EPT_DESC_CONTROL             0x00 /*!< usb endpoint description type control */
#define USB_EPT_DESC_ISO                 0x01 /*!< usb endpoint description type iso */
#define USB_EPT_DESC_BULK                0x02 /*!< usb endpoint description type bulk */
#define USB_EPT_DESC_INTERRUPT           0x03 /*!< usb endpoint description type interrupt */

#define USB_EPT_DESC_NSYNC               0x00 /*!< usb endpoint description nsync */
#define USB_ETP_DESC_ASYNC               0x04 /*!< usb endpoint description async */
#define USB_ETP_DESC_ADAPTIVE            0x08 /*!< usb endpoint description adaptive */
#define USB_ETP_DESC_SYNC                0x0C /*!< usb endpoint description sync */

#define USB_EPT_DESC_DATA_EPT            0x00 /*!< usb endpoint description data */
#define USB_EPT_DESC_FD_EPT              0x10 /*!< usb endpoint description fd */
#define USB_EPT_DESC_FDDATA_EPT          0x20 /*!< usb endpoint description fddata */

/**
  * @brief usb cdc class descriptor define
  */
#define USBD_CDC_CS_INTERFACE             0x24
#define USBD_CDC_CS_ENDPOINT              0x25

/**
  * @brief usb cdc class sub-type define
  */
#define USBD_CDC_SUBTYPE_HEADER           0x00
#define USBD_CDC_SUBTYPE_CMF              0x01
#define USBD_CDC_SUBTYPE_ACM              0x02
#define USBD_CDC_SUBTYPE_UFD              0x06

/**
  * @brief usb cdc class request code define
  */
#define SET_LINE_CODING                   0x20
#define GET_LINE_CODING                   0x21

/**
  * @brief usb cdc class set line coding struct
  */
typedef struct
{
  uint32_t bitrate;                      /* line coding baud rate */
  uint8_t format;                        /* line coding foramt */
  uint8_t parity;                        /* line coding parity */
  uint8_t data;                          /* line coding data bit */
}linecoding_type;

/**
  * @brief usb hid class descriptor define
  */
#define HID_CLASS_DESC_HID               0x21
#define HID_CLASS_DESC_REPORT            0x22
#define HID_CLASS_DESC_PHYSICAL          0x23

/**
  * @brief usb hid class request code define
  */
#define HID_REQ_SET_PROTOCOL             0x0B
#define HID_REQ_GET_PROTOCOL             0x03
#define HID_REQ_SET_IDLE                 0x0A
#define HID_REQ_GET_IDLE                 0x02
#define HID_REQ_SET_REPORT               0x09
#define HID_REQ_GET_REPORT               0x01
#define HID_DESCRIPTOR_TYPE              0x21
#define HID_REPORT_DESC                  0x22

/**
  * @brief endpoint 0 max size
  */
#define USB_MAX_EP0_SIZE                 64 /*!< usb endpoint 0 max size */

/**
  * @brief usb swap address
  */
#define SWAPBYTE(addr)        (uint16_t)(((uint16_t)(*((uint8_t *)(addr)))) + \
                               (((uint16_t)(*(((uint8_t *)(addr)) + 1))) << 8)) /*!< swap address */

/**
  * @brief min and max define
  */
#ifndef MIN
#define MIN(a, b)  (uint16_t)(((a) < (b)) ? (a) : (b)) /*!< min define*/
#endif

#ifndef MAX
#define MAX(a, b)  (uint16_t)(((a) > (b)) ? (a) : (b)) /*!< max define*/
#endif

/**
  * @brief low byte and high byte define
  */
#define LBYTE(x)  ((uint8_t)(x & 0x00FF))        /*!< low byte define */
#define HBYTE(x)  ((uint8_t)((x & 0xFF00) >>8))  /*!< high byte define*/

/**
  * @}
  */

/** @defgroup USB_standard_exported_types
  * @{
  */

/**
  * @brief usb return status
  */
typedef enum
{
  USB_OK,              /*!< usb status ok */
  USB_FAIL,            /*!< usb status fail */
  USB_WAIT,            /*!< usb status wait */
  USB_NOT_SUPPORT,     /*!< usb status not support */
  USB_ERROR,           /*!< usb status error */
}usb_sts_type;

/**
  * @brief format of usb setup data
  */
typedef struct
{
  uint8_t                                bmRequestType;                 /*!< characteristics of request */
  uint8_t                                bRequest;                      /*!< specific request */
  uint16_t                               wValue;                        /*!< word-sized field that varies according to request */
  uint16_t                               wIndex;                        /*!< word-sized field that varies according to request
                                                                           typically used to pass an index or offset */
  uint16_t                               wLength;                       /*!< number of bytes to transfer if there is a data stage */
}usb_setup_type;

/**
  * @brief format of standard device descriptor
  */
typedef struct
{
  uint8_t                                bLength;                       /*!< size of this descriptor in bytes */
  uint8_t                                bDescriptorType;               /*!< device descriptor type */
  uint16_t                               bcdUSB;                        /*!< usb specification release number */
  uint8_t                                bDeviceClass;                  /*!< class code (assigned by the usb-if) */
  uint8_t                                bDeviceSubClass;               /*!< subclass code (assigned by the usb-if) */
  uint8_t                                bDeviceProtocol;               /*!< protocol code ((assigned by the usb-if)) */
  uint8_t                                bMaxPacketSize0;               /*!< maximum packet size for endpoint zero */
  uint16_t                               idVendor;                      /*!< verndor id ((assigned by the usb-if)) */
  uint16_t                               idProduct;                     /*!< product id ((assigned by the usb-if)) */
  uint16_t                               bcdDevice;                     /*!< device release number in binary-coded decimal */
  uint8_t                                iManufacturer;                 /*!< index of string descriptor describing manufacturer */
  uint8_t                                iProduct;                      /*!< index of string descriptor describing product */
  uint8_t                                iSerialNumber;                 /*!< index of string descriptor describing serial number */
  uint8_t                                bNumConfigurations;            /*!< number of possible configurations */
}usb_device_desc_type;

/**
  * @brief format of standard configuration descriptor
  */
typedef struct
{
  uint8_t                                bLength;                        /*!< size of this descriptor in bytes */
  uint8_t                                bDescriptorType;                /*!< configuration descriptor type */
  uint16_t                               wTotalLength;                   /*!< total length of data returned for this configuration */
  uint8_t                                bNumInterfaces;                 /*!< number of interfaces supported by this configuration */
  uint8_t                                bConfigurationValue;            /*!< value to use as an argument to the SetConfiguration() request */
  uint8_t                                iConfiguration;                 /*!< index of string descriptor describing this configuration */
  uint8_t                                bmAttributes;                   /*!< configuration characteristics
                                                                            D7 reserved
                                                                            D6 self-powered
                                                                            D5 remote wakeup
                                                                            D4~D0 reserved */
  uint8_t                                bMaxPower;                      /*!< maximum power consumption of the usb device from the bus */


}usb_configuration_desc_type;

/**
  * @brief format of standard interface descriptor
  */
typedef struct
{
  uint8_t                                bLength;                        /*!< size of this descriptor in bytes */
  uint8_t                                bDescriptorType;                /*!< interface descriptor type */
  uint8_t                                bInterfaceNumber;               /*!< number of this interface */
  uint8_t                                bAlternateSetting;              /*!< value used to select this alternate setting for the interface */
  uint8_t                                bNumEndpoints;                  /*!< number of endpoints used by this interface */
  uint8_t                                bInterfaceClass;                /*!< class code (assigned by the usb-if) */
  uint8_t                                bInterfaceSubClass;             /*!< subclass code (assigned by the usb-if) */
  uint8_t                                bInterfaceProtocol;             /*!< protocol code (assigned by the usb-if) */
  uint8_t                                iInterface;                     /*!< index of string descriptor describing this interface */
}usb_interface_desc_type;

/**
  * @brief format of standard endpoint descriptor
  */
typedef struct
{
  uint8_t                                bLength;                        /*!< size of this descriptor in bytes */
  uint8_t                                bDescriptorType;                /*!< endpoint descriptor type */
  uint8_t                                bEndpointAddress;               /*!< the address of the endpoint on the usb device described by this descriptor */
  uint8_t                                bmAttributes;                   /*!< describes the endpoints attributes when it is configured using bConfiguration value */
  uint16_t                               wMaxPacketSize;                 /*!< maximum packet size this endpoint */
  uint8_t                                bInterval;                      /*!< interval for polling endpoint for data transfers */
}usb_endpoint_desc_type;

/**
  * @brief format of header
  */
typedef struct
{
  uint8_t                                bLength;                        /*!< size of this descriptor in bytes */
  uint8_t                                bDescriptorType;                /*!< descriptor type */
}usb_header_desc_type;


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif

