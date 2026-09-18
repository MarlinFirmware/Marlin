/**
  **************************************************************************
  * @file     cdc_keyboard_desc.c
  * @brief    usb cdc and keyboard device descriptor
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
#include "usb_std.h"
#include "usbd_sdr.h"
#include "usbd_core.h"
#include "cdc_keyboard_desc.h"
/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @defgroup USB_cdc_keyboard_desc
  * @brief usb device cdc keyboard descriptor
  * @{
  */

/** @defgroup USB_cdc_keyboard_desc_private_functions
  * @{
  */

static usbd_desc_t *get_device_descriptor(void);
static usbd_desc_t *get_device_qualifier(void);
static usbd_desc_t *get_device_configuration(void);
static usbd_desc_t *get_device_other_speed(void);
static usbd_desc_t *get_device_lang_id(void);
static usbd_desc_t *get_device_manufacturer_string(void);
static usbd_desc_t *get_device_product_string(void);
static usbd_desc_t *get_device_serial_string(void);
static usbd_desc_t *get_device_interface_string(void);
static usbd_desc_t *get_device_config_string(void);

static uint16_t usbd_unicode_convert(uint8_t *string, uint8_t *unicode_buf);
static void usbd_int_to_unicode (uint32_t value , uint8_t *pbuf , uint8_t len);
static void get_serial_num(void);
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_usbd_desc_buffer[256] ALIGNED_TAIL;

/**
  * @brief device descriptor handler structure
  */
usbd_desc_handler cdc_keyboard_desc_handler =
{
  get_device_descriptor,
  get_device_qualifier,
  get_device_configuration,
  get_device_other_speed,
  get_device_lang_id,
  get_device_manufacturer_string,
  get_device_product_string,
  get_device_serial_string,
  get_device_interface_string,
  get_device_config_string,
};

/**
  * @brief usb device standard descriptor
  */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_usbd_descriptor[USB_DEVICE_DESC_LEN] ALIGNED_TAIL =
{
  USB_DEVICE_DESC_LEN,                   /* bLength */
  USB_DESCIPTOR_TYPE_DEVICE,             /* bDescriptorType */
  0x00,                                  /* bcdUSB */
  0x02,
  0xEF,                                  /* bDeviceClass */
  0x02,                                  /* bDeviceSubClass */
  0x01,                                  /* bDeviceProtocol */
  USB_MAX_EP0_SIZE,                      /* bMaxPacketSize */
  LBYTE(USBD_VCPKYBRD_VENDOR_ID),        /* idVendor */
  HBYTE(USBD_VCPKYBRD_VENDOR_ID),        /* idVendor */
  LBYTE(USBD_VCPKYBRD_PRODUCT_ID),       /* idProduct */
  HBYTE(USBD_VCPKYBRD_PRODUCT_ID),       /* idProduct */
  0x00,                                  /* bcdDevice rel. 2.00 */
  0x02,
  USB_MFC_STRING,                        /* Index of manufacturer string */
  USB_PRODUCT_STRING,                    /* Index of product string */
  USB_SERIAL_STRING,                     /* Index of serial number string */
  1                                      /* bNumConfigurations */
};

/**
  * @brief usb configuration standard descriptor
  */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_usbd_configuration[USBD_VCPKYBRD_CONFIG_DESC_SIZE] ALIGNED_TAIL =
{
  USB_DEVICE_CFG_DESC_LEN,               /* bLength: configuration descriptor size */
  USB_DESCIPTOR_TYPE_CONFIGURATION,      /* bDescriptorType: configuration */
  LBYTE(USBD_VCPKYBRD_CONFIG_DESC_SIZE), /* wTotalLength: bytes returned */
  HBYTE(USBD_VCPKYBRD_CONFIG_DESC_SIZE), /* wTotalLength: bytes returned */
  0x03,                                  /* bNumInterfaces: 2 interface */
  0x01,                                  /* bConfigurationValue: configuration value */
  0x00,                                  /* iConfiguration: index of string descriptor describing
                                            the configuration */
  0xC0,                                  /* bmAttributes: self powered */
  0x32,                                  /* MaxPower 100 mA: this current is used for detecting vbus */

  0x08,                                  /* bLength */
  0x0B,                                  /* bDescriptorType */
  0x00,                                  /* bFirstInterface */
  0x02,                                  /* bInterfaceCount */
  0x02,                                  /* bFunctionClass */
  0x02,                                  /* bFunctionSubClass */
  0x01,                                  /* bFunctionProtocol */
  0x00,                                  /* iFunction (Index of string descriptor describing this function) */

  USB_DEVICE_IF_DESC_LEN,                /* bLength: interface descriptor size */
  USB_DESCIPTOR_TYPE_INTERFACE,          /* bDescriptorType: interface descriptor type */
  VCPKYBRD_CDC_INTERFACE,                         /* bInterfaceNumber: number of interface */
  0x00,                                  /* bAlternateSetting: alternate set */
  0x01,                                  /* bNumEndpoints: number of endpoints */
  USB_CLASS_CODE_CDC,                    /* bInterfaceClass: CDC class code */
  0x02,                                  /* bInterfaceSubClass: subclass code, Abstract Control Model*/
  0x01,                                  /* bInterfaceProtocol: protocol code, AT Command */
  0x00,                                  /* iInterface: index of string descriptor */

  0x05,                                  /* bFunctionLength: size of this descriptor in bytes */
  USBD_CDC_CS_INTERFACE,                 /* bDescriptorType: CDC interface descriptor type */
  USBD_CDC_SUBTYPE_HEADER,               /* bDescriptorSubtype: Header function Descriptor 0x00*/
  LBYTE(VCPKYBRD_BCD_NUM),
  HBYTE(VCPKYBRD_BCD_NUM),               /* bcdCDC: USB class definitions for communications */

  0x05,                                  /* bFunctionLength: size of this descriptor in bytes */
  USBD_CDC_CS_INTERFACE,                 /* bDescriptorType: CDC interface descriptor type */
  USBD_CDC_SUBTYPE_CMF,                  /* bDescriptorSubtype: Call Management function descriptor subtype 0x01 */
  0x00,                                  /* bmCapabilities: 0x00*/
  0x01,                                  /* bDataInterface: interface number of data class interface optionally used for call management */

  0x04,                                  /* bFunctionLength: size of this descriptor in bytes */
  USBD_CDC_CS_INTERFACE,                 /* bDescriptorType: CDC interface descriptor type */
  USBD_CDC_SUBTYPE_ACM,                  /* bDescriptorSubtype: Abstract Control Management functional descriptor subtype 0x02 */
  0x02,                                  /* bmCapabilities: Support Set_Line_Coding and Get_Line_Coding 0x02 */

  0x05,                                  /* bFunctionLength: size of this descriptor in bytes */
  USBD_CDC_CS_INTERFACE,                 /* bDescriptorType: CDC interface descriptor type */
  USBD_CDC_SUBTYPE_UFD,                  /* bDescriptorSubtype: Union Function Descriptor subtype 0x06 */
  0x00,                                  /* bControlInterface: The interface number of the communications or data class interface 0x00 */
  0x01,                                  /* bSubordinateInterface0: interface number of first subordinate interface in the union */

  USB_DEVICE_EPT_LEN,                    /* bLength: size of endpoint descriptor in bytes */
  USB_DESCIPTOR_TYPE_ENDPOINT,           /* bDescriptorType: endpoint descriptor type */
  USBD_VCPKYBRD_CDC_INT_EPT,             /* bEndpointAddress: the address of endpoint on usb device described by this descriptor */
  USB_EPT_DESC_INTERRUPT,                /* bmAttributes: endpoint attributes */
  LBYTE(USBD_VCPKYBRD_CMD_MAXPACKET_SIZE),
  HBYTE(USBD_VCPKYBRD_CMD_MAXPACKET_SIZE), /* wMaxPacketSize: maximum packe size this endpoint */
  VCPKYBRD_HID_BINTERVAL_TIME,            /* bInterval: interval for polling endpoint for data transfers */


  USB_DEVICE_IF_DESC_LEN,                /* bLength: interface descriptor size */
  USB_DESCIPTOR_TYPE_INTERFACE,          /* bDescriptorType: interface descriptor type */
  VCPKYBRD_CDC_DATA_INTERFACE,           /* bInterfaceNumber: number of interface */
  0x00,                                  /* bAlternateSetting: alternate set */
  0x02,                                  /* bNumEndpoints: number of endpoints */
  USB_CLASS_CODE_CDCDATA,                /* bInterfaceClass: CDC-data class code */
  0x00,                                  /* bInterfaceSubClass: Data interface subclass code 0x00*/
  0x00,                                  /* bInterfaceProtocol: data class protocol code 0x00 */
  0x00,                                  /* iInterface: index of string descriptor */

  USB_DEVICE_EPT_LEN,                    /* bLength: size of endpoint descriptor in bytes */
  USB_DESCIPTOR_TYPE_ENDPOINT,           /* bDescriptorType: endpoint descriptor type */
  USBD_VCPKYBRD_CDC_BULK_IN_EPT,         /* bEndpointAddress: the address of endpoint on usb device described by this descriptor */
  USB_EPT_DESC_BULK,                     /* bmAttributes: endpoint attributes */
  LBYTE(USBD_VCPKYBRD_IN_MAXPACKET_SIZE),
  HBYTE(USBD_VCPKYBRD_IN_MAXPACKET_SIZE), /* wMaxPacketSize: maximum packe size this endpoint */
  0x00,                                  /* bInterval: interval for polling endpoint for data transfers */

  USB_DEVICE_EPT_LEN,                    /* bLength: size of endpoint descriptor in bytes */
  USB_DESCIPTOR_TYPE_ENDPOINT,           /* bDescriptorType: endpoint descriptor type */
  USBD_VCPKYBRD_CDC_BULK_OUT_EPT,        /* bEndpointAddress: the address of endpoint on usb device described by this descriptor */
  USB_EPT_DESC_BULK,                     /* bmAttributes: endpoint attributes */
  LBYTE(USBD_VCPKYBRD_OUT_MAXPACKET_SIZE),
  HBYTE(USBD_VCPKYBRD_OUT_MAXPACKET_SIZE),/* wMaxPacketSize: maximum packe size this endpoint */
  0x00,                                  /* bInterval: interval for polling endpoint for data transfers */

  0x08, /* bLength */
  0x0B, /* bDescriptorType */
  0x02, /* bFirstInterface */
  0x01, /* bInterfaceCount */
  0x03, /* bFunctionClass */
  0x00, /* bFunctionSubClass */
  0x01, /* bFunctionProtocol */
  0x00, /* iFunction (Index of string descriptor describing this function) */

  USB_DEVICE_IF_DESC_LEN,                /* bLength: interface descriptor size */
  USB_DESCIPTOR_TYPE_INTERFACE,          /* bDescriptorType: interface descriptor type */
  VCPKYBRD_KEYBOARD_INTERFACE,          /* bInterfaceNumber: number of interface */
  0x00,                                  /* bAlternateSetting: alternate set */
  0x01,                                  /* bNumEndpoints: number of endpoints */
  USB_CLASS_CODE_HID,                    /* bInterfaceClass: class code hid */
  0x01,                                  /* bInterfaceSubClass: subclass code */
  0x01,                                  /* bInterfaceProtocol: protocol code */
  0x00,                                  /* iInterface: index of string descriptor */

  0x09,                                  /* bLength: size of HID descriptor in bytes */
  HID_CLASS_DESC_HID,                    /* bDescriptorType: HID descriptor type */
  LBYTE(VCPKYBRD_BCD_NUM),
  HBYTE(VCPKYBRD_BCD_NUM),                    /* bcdHID: HID class specification release number */
  0x00,                                  /* bCountryCode: hardware target conutry */
  0x01,                                  /* bNumDescriptors: number of HID class descriptor to follow */
  HID_CLASS_DESC_REPORT,                 /* bDescriptorType: report descriptor type */
  LBYTE(sizeof(g_usbd_vcpkybrd_hid_report)),
  HBYTE(sizeof(g_usbd_vcpkybrd_hid_report)), /* wDescriptorLength: total length of reprot descriptor */

  USB_DEVICE_EPT_LEN,                    /* bLength: size of endpoint descriptor in bytes */
  USB_DESCIPTOR_TYPE_ENDPOINT,           /* bDescriptorType: endpoint descriptor type */
  USBD_VCPKYBRD_HID_IN_EPT,              /* bEndpointAddress: the address of endpoint on usb device described by this descriptor */
  USB_EPT_DESC_INTERRUPT,                /* bmAttributes: endpoint attributes */
  LBYTE(USBD_VCPKYBRD_IN_MAXPACKET_SIZE),
  HBYTE(USBD_VCPKYBRD_IN_MAXPACKET_SIZE),/* wMaxPacketSize: maximum packe size this endpoint */
  VCPKYBRD_HID_BINTERVAL_TIME,           /* bInterval: interval for polling endpoint for data transfers */
};

/**
  * @brief usb hid descriptor
  */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD uint8_t g_vcpkybrd_hid_usb_desc[9] ALIGNED_TAIL =
{
  0x09,                                  /* bLength: size of HID descriptor in bytes */
  HID_CLASS_DESC_HID,                    /* bDescriptorType: HID descriptor type */
  LBYTE(VCPKYBRD_BCD_NUM),
  HBYTE(VCPKYBRD_BCD_NUM),               /* bcdHID: HID class specification release number */
  0x00,                                  /* bCountryCode: hardware target conutry */
  0x01,                                  /* bNumDescriptors: number of HID class descriptor to follow */
  HID_CLASS_DESC_REPORT,                 /* bDescriptorType: report descriptor type */
  LBYTE(sizeof(g_usbd_vcpkybrd_hid_report)),
  HBYTE(sizeof(g_usbd_vcpkybrd_hid_report)),  /* wDescriptorLength: total length of reprot descriptor */
};

/**
  * @brief usb hid keyboard report descriptor
  */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD uint8_t g_usbd_vcpkybrd_hid_report[USBD_VCPKYBRD_HID_SIZ_REPORT_DESC] ALIGNED_TAIL =
{
  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
  0x09, 0x06, // USAGE (Keyboard)
  0xa1, 0x01, // COLLECTION (Application)
  0x05, 0x07, // USAGE_PAGE (Keyboard)
  0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00, // LOGICAL_MINIMUM (0)
  0x25, 0x01, // LOGICAL_MAXIMUM (1)
  0x75, 0x01, // REPORT_SIZE (1)
  0x95, 0x08, // REPORT_COUNT (8)
  0x81, 0x02, // INPUT (Data,Var,Abs)
  0x95, 0x01, // REPORT_COUNT (1)
  0x75, 0x08, // REPORT_SIZE (8)
  0x81, 0x03, // INPUT (Cnst,Var,Abs)
  0x95, 0x05, // REPORT_COUNT (5)
  0x75, 0x01, // REPORT_SIZE (1)
  0x05, 0x08, // USAGE_PAGE (LEDs)
  0x19, 0x01, // USAGE_MINIMUM (Num Lock)
  0x29, 0x05, // USAGE_MAXIMUM (Kana)
  0x91, 0x02, // OUTPUT (Data,Var,Abs)
  0x95, 0x01, // REPORT_COUNT (1)
  0x75, 0x03, // REPORT_SIZE (3)
  0x91, 0x03, // OUTPUT (Cnst,Var,Abs)
  0x95, 0x06, // REPORT_COUNT (6)
  0x75, 0x08, // REPORT_SIZE (8)
  0x15, 0x00, // LOGICAL_MINIMUM (0)
  0x25, 0xFF, // LOGICAL_MAXIMUM (255)
  0x05, 0x07, // USAGE_PAGE (Keyboard)
  0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00, // INPUT (Data,Ary,Abs)
  0xc0        // END_COLLECTION
};
/**
  * @brief usb string lang id
  */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_string_lang_id[USBD_VCPKYBRD_SIZ_STRING_LANGID] ALIGNED_TAIL =
{
  USBD_VCPKYBRD_SIZ_STRING_LANGID,
  USB_DESCIPTOR_TYPE_STRING,
  0x09,
  0x04,
};

/**
  * @brief usb string serial
  */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_string_serial[USBD_VCPKYBRD_SIZ_STRING_SERIAL] ALIGNED_TAIL =
{
  USBD_VCPKYBRD_SIZ_STRING_SERIAL,
  USB_DESCIPTOR_TYPE_STRING,
};


/* device descriptor */
static usbd_desc_t device_descriptor =
{
  USB_DEVICE_DESC_LEN,
  g_usbd_descriptor
};

/* config descriptor */
static usbd_desc_t config_descriptor =
{
  USBD_VCPKYBRD_CONFIG_DESC_SIZE,
  g_usbd_configuration
};

/* langid descriptor */
static usbd_desc_t langid_descriptor =
{
  USBD_VCPKYBRD_SIZ_STRING_LANGID,
  g_string_lang_id
};

/* serial descriptor */
static usbd_desc_t serial_descriptor =
{
  USBD_VCPKYBRD_SIZ_STRING_SERIAL,
  g_string_serial
};

static usbd_desc_t vp_desc;

/**
  * @brief  standard usb unicode convert
  * @param  string: source string
  * @param  unicode_buf: unicode buffer
  * @retval length
  */
static uint16_t usbd_unicode_convert(uint8_t *string, uint8_t *unicode_buf)
{
  uint16_t str_len = 0, id_pos = 2;
  uint8_t *tmp_str = string;

  while(*tmp_str != '\0')
  {
    str_len ++;
    unicode_buf[id_pos ++] = *tmp_str ++;
    unicode_buf[id_pos ++] = 0x00;
  }

  str_len = str_len * 2 + 2;
  unicode_buf[0] = str_len;
  unicode_buf[1] = USB_DESCIPTOR_TYPE_STRING;

  return str_len;
}

/**
  * @brief  usb int convert to unicode
  * @param  value: int value
  * @param  pbus: unicode buffer
  * @param  len: length
  * @retval none
  */
static void usbd_int_to_unicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;

  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2 * idx] = (value >> 28) + '0';
  }
  else
  {
      pbuf[2 * idx] = (value >> 28) + 'A' - 10;
    }

    value = value << 4;

    pbuf[2 * idx + 1] = 0;
  }
}

/**
  * @brief  usb get serial number
  * @param  none
  * @retval none
  */
static void get_serial_num(void)
{
  uint32_t serial0, serial1, serial2;

  serial0 = *(uint32_t*)MCU_ID1;
  serial1 = *(uint32_t*)MCU_ID2;
  serial2 = *(uint32_t*)MCU_ID3;

  serial0 += serial2;

  if (serial0 != 0)
  {
    usbd_int_to_unicode (serial0, &g_string_serial[2] ,8);
    usbd_int_to_unicode (serial1, &g_string_serial[18] ,4);
  }
}

/**
  * @brief  get device descriptor
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_descriptor(void)
{
  return &device_descriptor;
}

/**
  * @brief  get device qualifier
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t * get_device_qualifier(void)
{
  return NULL;
}

/**
  * @brief  get config descriptor
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_configuration(void)
{
  return &config_descriptor;
}

/**
  * @brief  get other speed descriptor
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_other_speed(void)
{
  return NULL;
}

/**
  * @brief  get lang id descriptor
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_lang_id(void)
{
  return &langid_descriptor;
}


/**
  * @brief  get manufacturer descriptor
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_manufacturer_string(void)
{
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_VCPKYBRD_DESC_MANUFACTURER_STRING, g_usbd_desc_buffer);
  vp_desc.descriptor = g_usbd_desc_buffer;
  return &vp_desc;
}

/**
  * @brief  get product descriptor
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_product_string(void)
{
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_VCPKYBRD_DESC_PRODUCT_STRING, g_usbd_desc_buffer);
  vp_desc.descriptor = g_usbd_desc_buffer;
  return &vp_desc;
}

/**
  * @brief  get serial descriptor
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_serial_string(void)
{
  get_serial_num();
  return &serial_descriptor;
}

/**
  * @brief  get interface descriptor
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_interface_string(void)
{
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_VCPKYBRD_DESC_INTERFACE_STRING, g_usbd_desc_buffer);
  vp_desc.descriptor = g_usbd_desc_buffer;
  return &vp_desc;
}

/**
  * @brief  get device config descriptor
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_config_string(void)
{
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_VCPKYBRD_DESC_CONFIGURATION_STRING, g_usbd_desc_buffer);
  vp_desc.descriptor = g_usbd_desc_buffer;
  return &vp_desc;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
