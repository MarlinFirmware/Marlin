/**
  **************************************************************************
  * @file     audio_desc.c
  * @brief    usb audio device descriptor
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
#include "audio_desc.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @defgroup USB_audio_desc
  * @brief usb device audio descriptor
  * @{
  */

/** @defgroup USB_audio_desc_private_functions
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
usbd_desc_handler audio_desc_handler =
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
  0x00,                                  /* bDeviceClass */
  0x00,                                  /* bDeviceSubClass */
  0x00,                                  /* bDeviceProtocol */
  USB_MAX_EP0_SIZE,                      /* bMaxPacketSize */
  LBYTE(USBD_AUDIO_VENDOR_ID),           /* idVendor */
  HBYTE(USBD_AUDIO_VENDOR_ID),           /* idVendor */
  LBYTE(USBD_AUDIO_PRODUCT_ID),          /* idProduct */
  HBYTE(USBD_AUDIO_PRODUCT_ID),          /* idProduct */
  0x00,                                  /* bcdDevice rel. 2.00 */
  0x02,
  USB_MFC_STRING,                        /* Index of manufacturer string */
  USB_PRODUCT_STRING,                    /* Index of product string */
  USB_SERIAL_STRING,                     /* Index of serial number string */
  0x01                                   /* bNumConfigurations */
};

/**
  * @brief usb configuration standard descriptor
  */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_usbd_configuration[USBD_AUDIO_CONFIG_DESC_SIZE] ALIGNED_TAIL =
{
  USB_DEVICE_CFG_DESC_LEN,               /* bLength: configuration descriptor size */
  USB_DESCIPTOR_TYPE_CONFIGURATION,      /* bDescriptorType: configuration */
  LBYTE(USBD_AUDIO_CONFIG_DESC_SIZE),    /* wTotalLength: bytes returned */
  HBYTE(USBD_AUDIO_CONFIG_DESC_SIZE),    /* wTotalLength: bytes returned */
  0x1 + AUDIO_INTERFACE_NUM,             /* bNumInterfaces: n interface */
  0x01,                                  /* bConfigurationValue: configuration value */
  0x00,                                  /* iConfiguration: index of string descriptor describing
                                            the configuration */
  0xC0,                                  /* bmAttributes: self powered */
  0x32,                                  /* MaxPower 100 mA: this current is used for detecting vbus */

  USB_DEVICE_IF_DESC_LEN,                /* bLength: interface descriptor size */
  USB_DESCIPTOR_TYPE_INTERFACE,          /* bDescriptorType: interface descriptor type */
  0x00,                                  /* bInterfaceNumber: number of interface */
  0x00,                                  /* bAlternateSetting: alternate set */
  0x00,                                  /* bNumEndpoints: number of endpoints */
  USB_CLASS_CODE_AUDIO,                  /* bInterfaceClass: audio class code */
  AUDIO_SUBCLASS_AUDIOCONTROL,           /* bInterfaceSubClass: audio control */
  AUDIO_PROTOCOL_UNDEFINED,              /* bInterfaceProtocol: undefined */
  0x00,                                  /* iInterface: index of string descriptor */

  0x08+AUDIO_INTERFACE_NUM,              /* bLength: size of this descriptor, in bytes 8+n */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: cs interface descriptor type */
  AUDIO_AC_HEADER,                       /* bDescriptorSubtype: Header function Descriptor*/
  LBYTE(AUDIO_BCD_NUM),
  HBYTE(AUDIO_BCD_NUM),                  /* bcdCDC: audio device class specification release number */
  LBYTE(AUDIO_INTERFACE_LEN),
  HBYTE(AUDIO_INTERFACE_LEN),            /* wTotalLength: total number of bytes returned for the class-specific audio control interface */
  AUDIO_INTERFACE_NUM,                   /* bInClollection: the number of audio streaming */
#if (AUDIO_INTERFACE_NUM == 2)           /* two interface */
  0x02,
  0x01,
#else
  0x01,
#endif

  /* usb microphone config */
#if (AUDIO_SUPPORT_MIC == 1)
  AUDIO_INPUT_TERMINAL_SIZE,             /* bLength: descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: configuration */
  AUDIO_AC_INPUT_TERMINAL,               /* bDescriptorSubtype: input_terminal type*/
  AUDIO_MIC_INPUT_TERMINAL_ID,           /* bTerminalID: id of this input terminal*/
  LBYTE(AUDIO_INPUT_TERMINAL_MICROPHONE),
  HBYTE(AUDIO_INPUT_TERMINAL_MICROPHONE),/* wTerminalType: terminal is microphone */
  0x00,                                  /* bAssocTerminal: no association */
  AUDIO_MIC_CHR,                         /* bNrChannels: two channel */
#if (AUDIO_MIC_CHR == 2)
  0x03,                                  /* wChannelConfig: left front and right front */
#endif
#if (AUDIO_MIC_CHR == 1)
  0x00,                                  /* wChannelConfig */
#endif
  0x00,                                  /* wChannelConfig */
  0x00,                                  /* iChannelNames: unused */
  0x00,                                  /* iTerminal: unused */

  AUDIO_FEATURE_UNIT_SIZE,               /* bLength: descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: configuration */
  AUDIO_AC_FEATURE_UNIT,                 /* bDescriptorSubtype: feature unit type*/
  AUDIO_MIC_FEATURE_UNIT_ID,             /* bUnitID: id of this feature unit */
  AUDIO_MIC_INPUT_TERMINAL_ID,           /* bSourceID: from input terminal */
  0x02,                                  /* bControlSize: 2 byte */
  0x03,                                  /* bmaControls0: mute & volume*/
  0x00,
  0x00,                                  /* bmaControls1 */
  0x00,
  0x00,                                  /* bmaControls2 */
  0x00,
  0x00,                                  /* iFeature: unused */

  AUDIO_OUTPUT_TERMINAL_SIZE,            /* bLength: descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: configuration */
  AUDIO_AC_OUTPUT_TERMINAL,              /* bDescriptorSubtype: output_terminal type*/
  AUDIO_MIC_OUTPUT_TERMINAL_ID,          /* bTerminalID: id of this output terminal*/
  LBYTE(AUDIO_TERMINAL_TYPE_STREAMING),
  HBYTE(AUDIO_TERMINAL_TYPE_STREAMING),  /* wTerminalType: usb streaming */
  0x00,                                  /* bAssocTerminal: unused */
  AUDIO_MIC_FEATURE_UNIT_ID,             /* bSourceID: from feature unit terminal */
  0x00,                                  /* iTerminal: unused */
#endif

#if (AUDIO_SUPPORT_SPK == 1)
  /* speaker config */
  AUDIO_INPUT_TERMINAL_SIZE,             /* bLength: descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: configuration */
  AUDIO_AC_INPUT_TERMINAL,               /* bDescriptorSubtype: input_terminal type*/
  AUDIO_SPK_INPUT_TERMINAL_ID,           /* bTerminalID: id of this input terminal*/
  LBYTE(AUDIO_TERMINAL_TYPE_STREAMING),  /* wTerminalType: usb streaming */
  HBYTE(AUDIO_TERMINAL_TYPE_STREAMING),  /* wTerminalType: usb streaming */
  0x00,                                  /* bAssocTerminal: no association */
  AUDIO_SPK_CHR,                         /* bNrChannels: two channel */
#if (AUDIO_SPK_CHR == 2)
  0x03,                                  /* wChannelConfig: left front and right front */
#endif
#if (AUDIO_SPK_CHR == 1)
  0x00,                                  /* wChannelConfig */
#endif
  0x00,                                  /* wChannelConfig */
  0x00,                                  /* iChannelNames: unused */
  0x00,                                  /* iTerminal: unused */

  AUDIO_FEATURE_UNIT_SIZE,               /* bLength: descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: configuration */
  AUDIO_AC_FEATURE_UNIT,                 /* bDescriptorSubtype: feature unit type*/
  AUDIO_SPK_FEATURE_UNIT_ID,             /* bUnitID: id of this feature unit */
  AUDIO_SPK_INPUT_TERMINAL_ID,           /* bSourceID: from input terminal */
  0x02,                                  /* bControlSize: 2 byte */
  0x03,                                  /* bmaControls0: mute & volume*/
  0x00,
  0x00,                                  /* bmaControls1: 0x0000 */
  0x00,
  0x00,                                  /* bmaControls2: 0x0000 */
  0x00,
  0x00,                                  /* iFeature: unused */

  AUDIO_OUTPUT_TERMINAL_SIZE,            /* bLength: descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: configuration */
  AUDIO_AC_OUTPUT_TERMINAL,              /* bDescriptorSubtype: output_terminal type*/
  AUDIO_SPK_OUTPUT_TERMINAL_ID,          /* bTerminalID: id of this output terminal*/
  LBYTE(AUDIO_OUTPUT_TERMINAL_SPEAKER),  /* wTerminalType: usb speaker */
  HBYTE(AUDIO_OUTPUT_TERMINAL_SPEAKER),  /* wTerminalType: usb speaker */
  0x00,                                  /* bAssocTerminal: unused */
  AUDIO_SPK_FEATURE_UNIT_ID,             /* bSourceID: from feature unit terminal */
  0x00,                                  /* iTerminal: unused */
#endif

#if (AUDIO_SUPPORT_MIC == 1)
  /* microphone interface */
  0x09,                                  /* bLength: descriptor size */
  USB_DESCIPTOR_TYPE_INTERFACE,          /* bDescriptorType: interface descriptor type */
  AUDIO_MIC_INTERFACE_NUMBER,            /* bInterfaceNumber: index of this interface */
  0x00,                                  /* bAlternateSetting: index of this setting */
  0x00,                                  /* bNumEndpoints: 0 endpoints */
  USB_CLASS_CODE_AUDIO,                  /* bInterfaceClass: audio */
  AUDIO_SUBCLASS_AUDIOSTREAMING,         /* bInterfaceSubclass: audio streaming */
  0x00,                                  /* bInterfaceProtocol: unused */
  0x00,                                  /* iInterface: unused */

  0x09,                                  /* bLength: descriptor size */
  USB_DESCIPTOR_TYPE_INTERFACE,          /* bDescriptorType: interface descriptor type */
  AUDIO_MIC_INTERFACE_NUMBER,            /* bInterfaceNumber: index of this interface */
  0x01,                                  /* bAlternateSetting: index of this setting */
  0x01,                                  /* bNumEndpoints: 1 endpoints */
  USB_CLASS_CODE_AUDIO,                  /* bInterfaceClass: audio */
  AUDIO_SUBCLASS_AUDIOSTREAMING,         /* bInterfaceSubclass: audio streaming */
  0x00,                                  /* bInterfaceProtocol: unused */
  0x00,                                  /* iInterface: unused */

  0x07,                                  /* bLength: configuration descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: interface descriptor type */
  AUDIO_AS_GENERAL,                      /* bDescriptorSubtype: general sub type*/
  AUDIO_MIC_OUTPUT_TERMINAL_ID,          /* bTerminalLink: unit id of the output terminal */
  0x01,                                  /* bDelay: interface delay */
  0x01,                                  /* wFormatTag: pcm format*/
  0x00,                                  /* wFormatTag: pcm format*/

  0x08 + AUDIO_MIC_FREQ_SIZE * 3,        /* bLength: descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: interface descriptor type */
  AUDIO_AS_FORMAT_TYPE,                  /* bDescriptorSubtype: format subtype */
  AUDIO_FORMAT_TYPE_I,                   /* bFormatType: format type 1 */
  AUDIO_MIC_CHR,                         /* bNrChannels: channel number */
  AUDIO_MIC_BITW / 8,                    /* bSubFrameSize: per audio subframe */
  AUDIO_MIC_BITW,                        /* bBitResolution: n bits per sample */
  AUDIO_MIC_FREQ_SIZE,                   /* bSamFreqType: n frequency supported */
#if (AUDIO_SUPPORT_FREQ_16K == 1)
  SAMPLE_FREQ(AT32_AUDIO_FREQ_16K),      /* tSamFreq: 16000hz */
#endif
#if (AUDIO_SUPPORT_FREQ_48K == 1)
  SAMPLE_FREQ(AT32_AUDIO_FREQ_48K),      /* tSamFreq: 48000hz */
#endif

  0x09,                                  /* bLength: size of endpoint descriptor in bytes */
  USB_DESCIPTOR_TYPE_ENDPOINT,           /* bDescriptorType: endpoint descriptor type */
  USBD_AUDIO_MIC_IN_EPT,                 /* bEndpointAddress: the address of endpoint on usb device described by this descriptor */
  USB_EPT_DESC_ISO | USB_ETP_DESC_ASYNC, /* bmAttributes: endpoint attributes */
  LBYTE(AUDIO_MIC_IN_MAXPACKET_SIZE),
  HBYTE(AUDIO_MIC_IN_MAXPACKET_SIZE),    /* wMaxPacketSize: maximum packe size this endpoint */
  AUDIO_BINTERVAL_TIME,                    /* bInterval: interval for polling endpoint for data transfers */
  0x00,                                  /* bRefresh: unused */
  0x00,                                  /* bSynchAddress: unused */

  0x07,                                  /* bLength: size of endpoint descriptor in bytes */
  AUDIO_CS_ENDPOINT,                     /* bDescriptorType: cs endpoint descriptor type */
  0x01,                                  /* bDescriptorSubtype: general subtype */
  0x01,                                  /* bmAttributes */
  0x00,                                  /* bLockDelayUnits: unused */
  0x00,                                  /* wLockDelay: unused */
  0x00,                                  /* wLockDelay: unused */
#endif

#if (AUDIO_SUPPORT_SPK == 1)
  /* speaker interface */
  0x09,                                  /* bLength: descriptor size */
  USB_DESCIPTOR_TYPE_INTERFACE,          /* bDescriptorType: interface descriptor type */
  AUDIO_SPK_INTERFACE_NUMBER,            /* bInterfaceNumber: index of this interface */
  0x00,                                  /* bAlternateSetting: index of this setting */
  0x00,                                  /* bNumEndpoints: 0 endpoints */
  USB_CLASS_CODE_AUDIO,                  /* bInterfaceClass: audio */
  AUDIO_SUBCLASS_AUDIOSTREAMING,         /* bInterfaceSubclass: audio streaming */
  0x00,                                  /* bInterfaceProtocol: unused */
  0x00,                                  /* iInterface: unused */

  0x09,                                  /* bLength: descriptor size */
  USB_DESCIPTOR_TYPE_INTERFACE,          /* bDescriptorType: interface descriptor type */
  AUDIO_SPK_INTERFACE_NUMBER,            /* bInterfaceNumber: index of this interface */
  0x01,                                  /* bAlternateSetting: index of this setting */
  0x01 + AUDIO_SUPPORT_FEEDBACK,         /* bNumEndpoints: endpoints */
  USB_CLASS_CODE_AUDIO,                  /* bInterfaceClass: audio */
  AUDIO_SUBCLASS_AUDIOSTREAMING,         /* bInterfaceSubclass: audio streaming */
  0x00,                                  /* bInterfaceProtocol: unused */
  0x00,                                  /* iInterface: unused */

  0x07,                                  /* bLength: configuration descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: interface descriptor type */
  AUDIO_AS_GENERAL,                      /* bDescriptorSubtype: general sub type*/
  AUDIO_SPK_INPUT_TERMINAL_ID,           /* bTerminalLink: unit id of the input terminal */
  0x01,                                  /* bDelay: interface delay */
  0x01,                                  /* wFormatTag: pcm format*/
  0x00,                                  /* wFormatTag: pcm format*/

  0x08 + AUDIO_SPK_FREQ_SIZE * 3,        /* bLength: descriptor size */
  AUDIO_CS_INTERFACE,                    /* bDescriptorType: interface descriptor type */
  AUDIO_AS_FORMAT_TYPE,                  /* bDescriptorSubtype: format subtype */
  AUDIO_FORMAT_TYPE_I,                   /* bFormatType: format type 1 */
  AUDIO_SPK_CHR,                         /* bNrChannels: channel number */
  AUDIO_SPK_BITW / 8,                    /* bSubFrameSize: per audio subframe */
  AUDIO_SPK_BITW,                        /* bBitResolution: n bits per sample */
  AUDIO_SPK_FREQ_SIZE,                   /* bSamFreqType: n frequency supported */
#if (AUDIO_SUPPORT_FREQ_16K == 1)
  SAMPLE_FREQ(AT32_AUDIO_FREQ_16K),      /* tSamFreq: 16000hz */
#endif
#if (AUDIO_SUPPORT_FREQ_48K == 1)
  SAMPLE_FREQ(AT32_AUDIO_FREQ_48K),      /* tSamFreq: 48000hz */
#endif

  0x09,                                  /* bLength: size of endpoint descriptor in bytes */
  USB_DESCIPTOR_TYPE_ENDPOINT,           /* bDescriptorType: endpoint descriptor type */
  USBD_AUDIO_SPK_OUT_EPT,                /* bEndpointAddress: the address of endpoint on usb device described by this descriptor */
  USB_EPT_DESC_ISO | USB_ETP_DESC_ASYNC, /* bmAttributes: endpoint attributes */
  LBYTE(AUDIO_SPK_OUT_MAXPACKET_SIZE),
  HBYTE(AUDIO_SPK_OUT_MAXPACKET_SIZE),   /* wMaxPacketSize: maximum packe size this endpoint */
  AUDIO_BINTERVAL_TIME,                  /* bInterval: interval for polling endpoint for data transfers */
  0x00,                                  /* bRefresh: unused */
#if (AUDIO_SUPPORT_FEEDBACK == 1)
  USBD_AUDIO_FEEDBACK_EPT,               /* bSynchAddress: feedback endpoint */
#else
  0x00,                                  /* bSynchAddress: unused */
#endif

  0x07,                                  /* bLength: size of endpoint descriptor in bytes */
  AUDIO_CS_ENDPOINT,                     /* bDescriptorType: cs endpoint descriptor type */
  0x01,                                  /* bDescriptorSubtype: general subtype */
  0x01,                                  /* bmAttributes */
  0x00,                                  /* bLockDelayUnits: unused */
  0x00,                                  /* wLockDelay: unused */
  0x00,                                  /* wLockDelay: unused */

#if (AUDIO_SUPPORT_FEEDBACK == 1)
  0x09,                                  /* bLength: size of endpoint descriptor in bytes */
  USB_DESCIPTOR_TYPE_ENDPOINT,           /* bDescriptorType: endpoint descriptor type */
  USBD_AUDIO_FEEDBACK_EPT,               /* bEndpointAddress: the address of endpoint on usb device described by this descriptor */
  0x11,                                  /* bmAttributes: endpoint attributes */
  LBYTE(AUDIO_FEEDBACK_MAXPACKET_SIZE),  /* wMaxPacketSize: maximum packe size this endpoint */
  HBYTE(AUDIO_FEEDBACK_MAXPACKET_SIZE),  /* wMaxPacketSize: maximum packe size this endpoint */
  1,                                     /* bInterval: interval for polling endpoint for data transfers */
  FEEDBACK_REFRESH_TIME,                 /* bRefresh: this field indicates the rate at which an iso syncronization
                                                      pipe provides new syncronization feedback data. this rate must be a power of
                                                      2, therefore only the power is reported back and the range of this field is from
                                                      1(2ms) to 9(512ms) */
  0x00                                   /* bSynchAddress: 0x00*/
#endif

#endif
};

/**
  * @brief usb string lang id
  */
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD static uint8_t g_string_lang_id[USBD_AUDIO_SIZ_STRING_LANGID] ALIGNED_TAIL =
{
  USBD_AUDIO_SIZ_STRING_LANGID,
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
ALIGNED_HEAD static uint8_t g_string_serial[USBD_AUDIO_SIZ_STRING_SERIAL] ALIGNED_TAIL =
{
  USBD_AUDIO_SIZ_STRING_SERIAL,
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
  USBD_AUDIO_CONFIG_DESC_SIZE,
  g_usbd_configuration
};

/* langid descriptor */
static usbd_desc_t langid_descriptor =
{
  USBD_AUDIO_SIZ_STRING_LANGID,
  g_string_lang_id
};

/* serial descriptor */
static usbd_desc_t serial_descriptor =
{
  USBD_AUDIO_SIZ_STRING_SERIAL,
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
      pbuf[2 * idx] = (value >> 28) + '0';
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
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_AUDIO_DESC_MANUFACTURER_STRING, g_usbd_desc_buffer);
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
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_AUDIO_DESC_PRODUCT_STRING, g_usbd_desc_buffer);
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
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_AUDIO_DESC_INTERFACE_STRING, g_usbd_desc_buffer);
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
  vp_desc.length = usbd_unicode_convert((uint8_t *)USBD_AUDIO_DESC_CONFIGURATION_STRING, g_usbd_desc_buffer);
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
