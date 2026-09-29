/**
  **************************************************************************
  * @file     audio_conf.h
  * @brief    usb audio config
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
#ifndef __AUDIO_CONF_H
#define __AUDIO_CONF_H

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_audio_hid_class
  * @{
  */

/** @defgroup USB_device_audio_hid_config_definition
  * @{
  */
#define AUDIO_SUPPORT_SPK                1
#define AUDIO_SUPPORT_MIC                1
#define AUDIO_SUPPORT_FEEDBACK           1

#define AUDIO_SUPPORT_FREQ_16K           0
#define AUDIO_SUPPORT_FREQ_48K           1


#define AUDIO_SUPPORT_FREQ               (AUDIO_SUPPORT_FREQ_16K + \
                                          AUDIO_SUPPORT_FREQ_48K \
                                         )

#define AUDIO_FREQ_16K                   16000
#define AUDIO_FREQ_48K                   48000
#define AUDIO_BITW_16                    16

#define AUDIO_MIC_CHANEL_NUM            2
#define AUDIO_MIC_DEFAULT_BITW          AUDIO_BITW_16

#define AUDIO_SPK_CHANEL_NUM            2
#define AUDIO_SPK_DEFAULT_BITW          AUDIO_BITW_16


#define AUDIO_SUPPORT_MAX_FREQ           48
#define AUDIO_DEFAULT_FREQ               AUDIO_FREQ_48K
#define AUDIO_DEFAULT_BITW               AUDIO_BITW_16

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



