/**
  **************************************************************************
  * @file     audio_desc.h
  * @brief    usb audio descriptor header file
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
#ifndef __AUDIO_HID_DESC_H
#define __AUDIO_HID_DESC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "audio_hid_class.h"
#include "usbd_core.h"
#include "audio_conf.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_audio_hid_desc
  * @{
  */

/** @defgroup USB_audio_hid_desc_definition
  * @{
  */

#define AUHID_BCD_NUM                    0x0100

#define USBD_AUHID_VENDOR_ID             0x2E3C
#define USBD_AUHID_PRODUCT_ID            0x5555

#define USBD_AUHID_SIZ_STRING_LANGID     4
#define USBD_AUHID_SIZ_STRING_SERIAL     0x1A

#define USBD_AUHID_DESC_MANUFACTURER_STRING    "Artery"
#define USBD_AUHID_DESC_PRODUCT_STRING         "AT32 Audio"
#define USBD_AUHID_DESC_CONFIGURATION_STRING   "Audio Config"
#define USBD_AUHID_DESC_INTERFACE_STRING       "Audio Interface"
/**
  * @brief audio interface subclass codes
  */
#define AUDIO_SUBCLASS_UNDEFINED         0x00
#define AUDIO_SUBCLASS_AUDIOCONTROL      0x01
#define AUDIO_SUBCLASS_AUDIOSTREAMING    0x02
#define AUDIO_SUBCLASS_MIDISTREMING      0x03

/**
  * @brief audio class-specific descriptor types
  */
#define AUDIO_CS_INTERFACE               0x24
#define AUDIO_CS_ENDPOINT                0x25
#define AUDIO_CS_STRING                  0x23
#define AUDIO_CS_CONFIGURATION           0x22
#define AUDIO_CS_DEVICE                  0x21
#define AUDIO_CS_UNDEFINED               0x20

/**
  * @brief audio interface protocol codes
  */
#define AUDIO_PROTOCOL_UNDEFINED         0x00

/**
  * @brief audio class-specific ac interface descriptor subtypes
  */
#define AUDIO_AC_DESCRIPTOR_UNDEFINED    0x00
#define AUDIO_AC_HEADER                  0x01
#define AUDIO_AC_INPUT_TERMINAL          0x02
#define AUDIO_AC_OUTPUT_TERMINAL         0x03
#define AUDIO_AC_MIXER_UNIT              0x04
#define AUDIO_AC_SELECTOR_UNIT           0x05
#define AUDIO_AC_FEATURE_UNIT            0x06
#define AUDIO_AC_PROCESSING_UNIT         0x07
#define AUDIO_AC_EXTENSION_UNIT          0x08

/**
  * @brief audio class-specific as interface descriptor subtypes
  */
#define AUDIO_AS_DESCRIPTOR_UNDEFINED    0x00
#define AUDIO_AS_GENERAL                 0x01
#define AUDIO_AS_FORMAT_TYPE             0x02
#define AUDIO_AS_FORMAT_SPECIFIC         0x03

/**
  * @brief audio class-specific request codes
  */
#define AUDIO_REQUEST_CODE_UNDEFINED     0x00
#define AUDIO_REQ_SET_CUR                0x01
#define AUDIO_REQ_GET_CUR                0x81
#define AUDIO_REQ_SET_MIN                0x02
#define AUDIO_REQ_GET_MIN                0x82
#define AUDIO_REQ_SET_MAX                0x03
#define AUDIO_REQ_GET_MAX                0x83
#define AUDIO_REQ_SET_RES                0x04
#define AUDIO_REQ_GET_RES                0x84
#define AUDIO_REQ_SET_MEM                0x05
#define AUDIO_REQ_GET_MEM                0x85
#define AUDIO_REQ_GET_STAT               0xFF

/**
  * @brief audio feature unit control selectors
  */
#define AUDIO_FU_CONTROL_UNDEFINED       0x00
#define AUDIO_FU_MUTE_CONTROL            0x01
#define AUDIO_FU_VOLUME_CONTROL          0x02
#define AUDIO_FU_BASS_CONTROL            0x03
#define AUDIO_FU_MID_CONTROL             0x04
#define AUDIO_FU_TREBLE_CONTROL          0x05

/**
  * @brief audio terminal type
  */
#define AUDIO_TERMINAL_TYPE_UNDEFINED    0x0100
#define AUDIO_TERMINAL_TYPE_STREAMING    0x0101
#define AUDIO_TERMINAL_TYPE_VENDOR       0x01FF
#define AUDIO_INPUT_TERMINAL_UNDEFINED   0x0200
#define AUDIO_INPUT_TERMINAL_MICROPHONE  0x0201
#define AUDIO_OUTPUT_TERMINAL_UNDEFINED  0x0300
#define AUDIO_OUTPUT_TERMINAL_SPEAKER    0x0301

/**
  * @brief audio format type 1
  */
#define AUDIO_FORMAT_TYPE_I              0x01

/**
  * @brief audio interface config
  */
#define AUDIO_INTERFACE_NUM              (AUDIO_SUPPORT_SPK + AUDIO_SUPPORT_MIC)
#define AUDIO_INTERFACE_LEN              ((0x08 + AUDIO_INTERFACE_NUM) + AUDIO_INTERFACE_NUM * 0x22)
#define AUDIO_MIC_INTERFACE              0x01
#define AUDIO_SPK_INTERFACE              0x02

/**
  * @brief audio interface descriptor size define
  */
#define AUDIO_INPUT_TERMINAL_SIZE        0x0C
#define AUDIO_OUTPUT_TERMINAL_SIZE       0x09
#define AUDIO_FEATURE_UNIT_SIZE          0x0D

/**
  * @brief audio terminal id define
  */
#define AUDIO_MIC_INPUT_TERMINAL_ID      0x01
#define AUDIO_MIC_FEATURE_UNIT_ID        0x02
#define AUDIO_MIC_OUTPUT_TERMINAL_ID     0x03
#define AUDIO_SPK_INPUT_TERMINAL_ID      0x04
#define AUDIO_SPK_FEATURE_UNIT_ID        0x05
#define AUDIO_SPK_OUTPUT_TERMINAL_ID     0x06

/**
  * @brief audio interface number
  */
#define AUDIO_MIC_INTERFACE_NUMBER       0x01
#if (AUDIO_SUPPORT_MIC == 1)
#define AUDIO_SPK_INTERFACE_NUMBER       0x02
#else
#define AUDIO_SPK_INTERFACE_NUMBER       0x01
#endif
#define HID_INTERFACE_NUMBER             0x03

/**
  * @brief audio support freq
  */
#define AT32_AUDIO_FREQ_16K              16000
#define AT32_AUDIO_FREQ_48K              48000

/**
  * @brief audio microphone freq and channel config
  */
#define AUDIO_MIC_FREQ_SIZE              (AUDIO_SUPPORT_FREQ)
#define AUDIO_MIC_CHR                    AUDIO_MIC_CHANEL_NUM
#define AUDIO_MIC_BITW                   (AUDIO_MIC_DEFAULT_BITW)

/**
  * @brief audio speaker freq and channel config
  */
#define AUDIO_SPK_FREQ_SIZE              (AUDIO_SUPPORT_FREQ)
#define AUDIO_SPK_CHR                    AUDIO_SPK_CHANEL_NUM
#define AUDIO_SPK_BITW                   (AUDIO_SPK_DEFAULT_BITW)

#define AUHID_HID_BINTERVAL_TIME         0x01

/**
  * @brief usb bcd number define
  */
#define AUHID_HID_BCD_NUM                      0x0110

/**
  * @brief usb hid class descriptor define
  */
#define HID_CLASS_DESC_HID               0x21
#define HID_CLASS_DESC_REPORT            0x22
#define HID_CLASS_DESC_PHYSICAL          0x23

/**
  * @brief usb hid report id define
  */
#define HID_REPORT_ID_1                   0x01
#define HID_REPORT_ID_2                   0x02
#define HID_REPORT_ID_3                   0x03
#define HID_REPORT_ID_4                   0x04
#define HID_REPORT_ID_5                   0x05
#define HID_REPORT_ID_6                   0xF0

#define MCU_ID1                           (0x1FFFF7E8)
#define MCU_ID2                           (0x1FFFF7EC)
#define MCU_ID3                           (0x1FFFF7F0)


#define SAMPLE_FREQ(frq)                 (uint8_t)(frq), (uint8_t)((frq >> 8)), (uint8_t)((frq >> 16))

#define USBD_AUHID_HID_SIZ_REPORT_DESC         126
#define USBD_AUHID_HID_DESC_SIZE                32
#define USBD_AUHID_CONFIG_DESC_SIZE             ( 0x12 + AUDIO_INTERFACE_LEN + \
                                          + (0x31 + AUDIO_SPK_FREQ_SIZE * 3) \
                                          + (0x31 + AUDIO_MIC_FREQ_SIZE * 3) \
                                          + (9 * AUDIO_SUPPORT_FEEDBACK) \
                                          + USBD_AUHID_HID_DESC_SIZE)

extern uint8_t g_usbd_audio_hid_report[USBD_AUHID_HID_SIZ_REPORT_DESC];
extern uint8_t g_audio_hid_usb_desc[9];

extern usbd_desc_handler audio_hid_desc_handler;

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

