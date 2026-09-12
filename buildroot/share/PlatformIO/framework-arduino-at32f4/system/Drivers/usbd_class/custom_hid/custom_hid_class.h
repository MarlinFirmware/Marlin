/**
  **************************************************************************
  * @file     custom_hid_class.h
  * @brief    usb custom hid header file
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
#ifndef __CUSTOM_HID_CLASS_H
#define __CUSTOM_HID_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_std.h"
#include "usbd_core.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_custom_hid_class
  * @{
  */

/** @defgroup USB_custom_hid_class_endpoint_definition
  * @{
  */

/**
  * @brief usb custom hid use endpoint define
  */
#define USBD_CUSTOM_HID_IN_EPT                  0x81
#define USBD_CUSTOM_HID_OUT_EPT                 0x01

/**
  * @brief usb custom hid in and out max packet size define
  */
#define USBD_CUSTOM_IN_MAXPACKET_SIZE           0x40
#define USBD_CUSTOM_OUT_MAXPACKET_SIZE          0x40

/**
  * @}
  */

/** @defgroup USB_custom_hid_class_request_code_definition
  * @{
  */

typedef struct
{
  uint8_t g_rxhid_buff[USBD_CUSTOM_OUT_MAXPACKET_SIZE];
  uint8_t g_txhid_buff[USBD_CUSTOM_IN_MAXPACKET_SIZE];

  uint32_t hid_protocol;
  uint32_t hid_set_idle;
  uint32_t alt_setting;
  
  uint8_t hid_set_report[64];
  uint8_t hid_get_report[64];
  uint8_t hid_state;
}custom_hid_type;

/**
  * @}
  */

/** @defgroup USB_custom_hid_class_exported_functions
  * @{
  */
extern usbd_class_handler custom_hid_class_handler;
usb_sts_type custom_hid_class_send_report(void *udev, uint8_t *report, uint16_t len);
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
