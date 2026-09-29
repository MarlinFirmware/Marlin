/**
  **************************************************************************
  * @file     audio_class.h
  * @brief    usb audio class file
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
#ifndef __AUDIO_HID_CLASS_H
#define __AUDIO_HID_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_std.h"
#include "usbd_core.h"
#include "audio_conf.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_audio_hid_class
  * @{
  */

/** @defgroup USB_audio_hid_class_definition
  * @{
  */

/**
  * @brief endpoint define
  */
#define USBD_AUHID_AUDIO_MIC_IN_EPT            0x81
#define USBD_AUHID_AUDIO_SPK_OUT_EPT           0x02
#define USBD_AUHID_AUDIO_FEEDBACK_EPT          0x83

/**
  * @brief usb custom hid use endpoint define
  */
#define USBD_AUHID_HID_IN_EPT                  0x84
#define USBD_AUHID_HID_OUT_EPT                 0x05

/**
  * @brief usb custom hid in and out max packet size define
  */
#define USBD_AUHID_IN_MAXPACKET_SIZE           0x40
#define USBD_AUHID_OUT_MAXPACKET_SIZE          0x40

/**
  * @brief endpoint support max size
  */
#define AUDIO_REMAIN_SIZE                40
#define AUDIO_MIC_IN_MAXPACKET_SIZE      (AUDIO_SUPPORT_MAX_FREQ * AUDIO_MIC_CHANEL_NUM * (AUDIO_MIC_DEFAULT_BITW / 8) + AUDIO_REMAIN_SIZE)
#define AUDIO_SPK_OUT_MAXPACKET_SIZE     (AUDIO_SUPPORT_MAX_FREQ * AUDIO_SPK_CHANEL_NUM * (AUDIO_SPK_DEFAULT_BITW / 8) + AUDIO_REMAIN_SIZE)
#define AUDIO_FEEDBACK_MAXPACKET_SIZE    0x3
#define FEEDBACK_REFRESH_TIME            0x8
/**
  * @brief request type define
  */
#define AUDIO_REQ_CONTROL_INTERFACE      0x01
#define AUDIO_REQ_CONTROL_ENDPOINT       0x02
#define AUDIO_REQ_CONTROL_MASK           0x03

/**
  * @brief audio set cur type define
  */
#define AUDIO_MUTE_CONTROL               0x01
#define AUDIO_VOLUME_CONTROL             0x02
#define AUDIO_FREQ_SET_CONTROL           0x03

/**
  * @brief audio descriptor type
  */

/**
  * @brief audio set cur type define
  */
#define AUDIO_MUTE_CONTROL               0x01
#define AUDIO_VOLUME_CONTROL             0x02
#define AUDIO_FREQ_SET_CONTROL           0x03

/**
  * @brief audio descriptor type
  */
#define AUDIO_DESCRIPTOR_TYPE             0x21
#define AUDIO_DESCRIPTOR_SIZE             0x09

/**
  * @brief usb audio control struct
  */
typedef struct
{
  uint8_t enpd;
  uint8_t interface;
  uint8_t request_no;
  uint8_t spk_mute;
  uint8_t mic_mute;
  uint16_t spk_volume;
  uint16_t mic_volume;
  uint32_t spk_freq;
  uint32_t mic_freq;
  uint16_t spk_volume_limits[3]; /*[0] is mininum value, [1] is maxnum value, [2] is volume resolution */
  uint16_t mic_volume_limits[3]; /*[0] is mininum value, [1] is maxnum value, [2] is volume resolution */

  uint8_t audio_cmd;
  uint32_t audio_cmd_len;
  uint32_t spk_alt_setting;
  uint32_t mic_alt_setting;
  uint8_t g_audio_cur[64];
  uint8_t audio_spk_data[AUDIO_SPK_OUT_MAXPACKET_SIZE];
  uint8_t audio_mic_data[AUDIO_MIC_IN_MAXPACKET_SIZE];
  uint8_t audio_feed_back[AUDIO_FEEDBACK_MAXPACKET_SIZE+1];


  /* hid */
  uint32_t hid_protocol;
  uint32_t hid_set_idle;
  uint32_t alt_setting;
  uint8_t hid_set_report[64];
  uint8_t g_rxhid_buff[USBD_AUHID_OUT_MAXPACKET_SIZE];
  uint8_t g_txhid_buff[USBD_AUHID_IN_MAXPACKET_SIZE];
  uint8_t hid_state;
}usb_audio_hid_type;

extern usbd_class_handler audio_hid_class_handler;

usb_sts_type audio_hid_class_send_report(void *udev, uint8_t *report, uint16_t len);

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

