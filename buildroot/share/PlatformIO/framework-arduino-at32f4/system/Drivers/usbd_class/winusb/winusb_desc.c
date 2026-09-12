/**
  **************************************************************************
  * @file     winusb_desc.c
  * @brief    usb winusb device descriptor
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
#include "stdio.h"
#include "usb_std.h"
#include "usbd_sdr.h"
#include "usbd_core.h"
#include "winusb_desc.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @defgroup USB_winusb_desc
  * @brief usb device winusb descriptor
  * @{
  */

/** @defgroup USB_winusb_desc_private_functions
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

#if (USBD_SUPPORT_WINUSB == 1)
static usbd_desc_t *get_device_winusb_os_string(void);
static usbd_desc_t *get_device_winusb_os_feature(void);
static usbd_desc_t *get_device_winusb_os_property(void);
#endif

#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_usbd_desc_buffer[256] ALIGNED_TAIL;

/**
  * @brief device descriptor handler structure
  */
usbd_desc_handler winusb_desc_handler =
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
#if (USBD_SUPPORT_WINUSB == 1)
  get_device_winusb_os_string,
  get_device_winusb_os_feature,
  get_device_winusb_os_property
#endif
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
  0x00,                                  /* bDeviceClass */
  0x00,                                  /* bDeviceSubClass */
  0x00,                                  /* bDeviceProtocol */
  USB_MAX_EP0_SIZE,                      /* bMaxPacketSize */
  LBYTE(USBD_WINUSB_VENDOR_ID),          /* idVendor */
  HBYTE(USBD_WINUSB_VENDOR_ID),          /* idVendor */
  LBYTE(USBD_WINUSB_PRODUCT_ID),         /* idProduct */
  HBYTE(USBD_WINUSB_PRODUCT_ID),         /* idProduct */
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
ALIGNED_HEAD static uint8_t g_usbd_configuration[USBD_WINUSB_CONFIG_DESC_SIZE] ALIGNED_TAIL =
{
  USB_DEVICE_CFG_DESC_LEN,               /* bLength: configuration descriptor size */
  USB_DESCIPTOR_TYPE_CONFIGURATION,      /* bDescriptorType: configuration */
  LBYTE(USBD_WINUSB_CONFIG_DESC_SIZE),   /* wTotalLength: bytes returned */
  HBYTE(USBD_WINUSB_CONFIG_DESC_SIZE),   /* wTotalLength: bytes returned */
  0x01,                                  /* bNumInterfaces: 1 interface */
  0x01,                                  /* bConfigurationValue: configuration value */
  0x00,                                  /* iConfiguration: index of string descriptor describing
                                            the configuration */
  0xC0,                                  /* bmAttributes: self powered */
  0x32,                                  /* MaxPower 100 mA: this current is used for detecting vbus */
  
  USB_DEVICE_IF_DESC_LEN,                /* bLength: interface descriptor size */
  USB_DESCIPTOR_TYPE_INTERFACE,          /* bDescriptorType: interface descriptor type */
  0x00,                                  /* bInterfaceNumber: number of interface */
  0x00,                                  /* bAlternateSetting: alternate set */
  0x02,                                  /* bNumEndpoints: number of endpoints */
  0xff,                                  /* bInterfaceClass: Vendor specific*/
  0x00,                                  /* bInterfaceSubClass: subclass code */
  0x00,                                  /* bInterfaceProtocol: protocol code */
  0x00,                                  /* iInterface: index of string descriptor */

  USB_DEVICE_EPT_LEN,                    /* bLength: size of endpoint descriptor in bytes */
  USB_DESCIPTOR_TYPE_ENDPOINT,           /* bDescriptorType: endpoint descriptor type */
  USBD_WINUSB_BULK_OUT_EPT,              /* bEndpointAddress: the address of endpoint on usb device described by this descriptor */
  USB_EPT_DESC_BULK,                     /* bmAttributes: endpoint attributes */
  LBYTE(USBD_WINUSB_OUT_MAXPACKET_SIZE),        
  HBYTE(USBD_WINUSB_OUT_MAXPACKET_SIZE),    /* wMaxPacketSize: maximum packe size this endpoint */	
  0x00,                                  /* bInterval: interval for polling endpoint for data transfers */    
  
  USB_DEVICE_EPT_LEN,                    /* bLength: size of endpoint descriptor in bytes */
  USB_DESCIPTOR_TYPE_ENDPOINT,           /* bDescriptorType: endpoint descriptor type */
  USBD_WINUSB_BULK_IN_EPT,                  /* bEndpointAddress: the address of endpoint on usb device described by this descriptor */
  USB_EPT_DESC_BULK,                     /* bmAttributes: endpoint attributes */	
  LBYTE(USBD_WINUSB_IN_MAXPACKET_SIZE),
  HBYTE(USBD_WINUSB_IN_MAXPACKET_SIZE),  /* wMaxPacketSize: maximum packe size this endpoint */	
  0x00,                                  /* bInterval: interval for polling endpoint for data transfers */
};

/**
  * @brief usb string lang id
  */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_string_lang_id[USBD_WINUSB_SIZ_STRING_LANGID] ALIGNED_TAIL =
{
  USBD_WINUSB_SIZ_STRING_LANGID,
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
ALIGNED_HEAD static uint8_t g_string_serial[USBD_WINUSB_SIZ_STRING_SERIAL] ALIGNED_TAIL =
{
  USBD_WINUSB_SIZ_STRING_SERIAL,
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
  USBD_WINUSB_CONFIG_DESC_SIZE,
  g_usbd_configuration
};

/* langid descriptor */
static usbd_desc_t langid_descriptor =
{
  USBD_WINUSB_SIZ_STRING_LANGID,
  g_string_lang_id
};

/* serial descriptor */
static usbd_desc_t serial_descriptor =
{
  USBD_WINUSB_SIZ_STRING_SERIAL,
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
  unicode_buf[0] = (uint8_t)str_len;
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
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_WINUSB_DESC_MANUFACTURER_STRING, g_usbd_desc_buffer);
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
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_WINUSB_DESC_PRODUCT_STRING, g_usbd_desc_buffer);
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
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_WINUSB_DESC_INTERFACE_STRING, g_usbd_desc_buffer);
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
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_WINUSB_DESC_CONFIGURATION_STRING, g_usbd_desc_buffer);
  vp_desc.descriptor = g_usbd_desc_buffer;
  return &vp_desc;
}

#if (USBD_SUPPORT_WINUSB == 1)

#define USB_LEN_OS_FEATURE_DESC   0x28
#define USB_LEN_OS_PROPERTY_DESC  0x8E

/* os string descriptor fields */
const uint8_t winusb_os_string[8] = { 
   'M','S','F','T','1','0','0',
   WINUSB_BMS_VENDOR_CODE, 
};

#define U32TO8C(v) ((v & 0xFF), ((v >> 8) & 0xFF), ((v >> 16) & 0xFF), ((v >> 24) & 0xFF))

#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_usbd_winusb_os_feature[USB_LEN_OS_FEATURE_DESC] ALIGNED_TAIL =
{
  USB_LEN_OS_FEATURE_DESC, 0x00, 0x00, 0x00,     /*dwlength: 4 byte*/
  0x00, 0x01,                                    /* bcd version 1.0 */
  0x04, 0x00,                                    /* windex: extended compat ID descritor*/
  0x01,                                          /* bcount the number of custom property sections */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      /* reserved */
  /* function section */
  0x00,                                          /* first interface number */
  0x00,                                          /* reserved */
  'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,      /* 8 byte: the function compatible id */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,/* 8 byte: the function subcompatible id */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00             /* reserved 6 bytes */   
};

#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_usbd_winusb_os_property[USB_LEN_OS_PROPERTY_DESC] ALIGNED_TAIL =
{
	USB_LEN_OS_PROPERTY_DESC, 0x00, 0x00, 0x00,/* dwlength: 4 byte*/
  0x00, 0x01,                                /* bcd version 1.0 */
  0x05, 0x00,                                /* property descriptor index(5) */
  0x01, 0x00,                                /* wcount: number of section (1) */
  
  /* custom property section */
  0x84, 0x00, 0x00, 0x00,                   /* dwsize: size of property section */
  0x01, 0x00, 0x00, 0x00,                   /* property data type */
  0x28, 0x00,                               /* property name length */
  /* property name: DeviceinterfaceGUID */
  'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00, 'c', 0x00, 'e', 0x00,
  'I', 0x00, 'n', 0x00, 't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00,
  'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00, 'U', 0x00, 'I', 0x00, 
  'D', 0x00, 0x00, 0x00,
  
  0x4E, 0x00, 0x00, 0x00, /* property data length */
  /*property data: {13eb360b-bc1e-46cb-ac8b-ef3da47b4062} */
  '{', 0x00, '1', 0x00, '3', 0x00, 'E', 0x00, 'B', 0x00, '3', 0x00,
  '6', 0x00, '0', 0x00, 'B', 0x00, '-', 0x00, 'B', 0x00, 'C', 0x00,
  '1', 0x00, 'E', 0x00, '-', 0x00, '4', 0x00, '6', 0x00, 'C', 0x00,
  'B', 0x00, '-', 0x00, 'A', 0x00, 'C', 0x00, '8', 0x00, 'B', 0x00,
  '-', 0x00, 'E', 0x00, 'F', 0x00, '3', 0x00, 'D', 0x00, 'A', 0x00,
  '4', 0x00, '7', 0x00, 'B', 0x00, '4', 0x00, '0', 0x00, '6', 0x00,
  '2', 0x00, '}', 0x00, 0x00, 0x00
};

static usbd_desc_t winusb_os_feature_descriptor =
{
  USB_LEN_OS_FEATURE_DESC,
  g_usbd_winusb_os_feature
};

static usbd_desc_t winusb_os_property_descriptor =
{
  USB_LEN_OS_PROPERTY_DESC,
  g_usbd_winusb_os_property
};

/**
  * @brief  get winusb os sring
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_winusb_os_string(void)
{
	vp_desc.length = usbd_unicode_convert((uint8_t *)winusb_os_string, g_usbd_desc_buffer);
  vp_desc.descriptor = g_usbd_desc_buffer;
  return &vp_desc;
}

/**
  * @brief  get winusb os feature
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_winusb_os_feature(void)
{
	return &winusb_os_feature_descriptor;
}

/**
  * @brief  get winusb os property
  * @param  none
  * @retval usbd_desc
  */
static usbd_desc_t *get_device_winusb_os_property(void)
{
	return &winusb_os_property_descriptor;
}


#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
