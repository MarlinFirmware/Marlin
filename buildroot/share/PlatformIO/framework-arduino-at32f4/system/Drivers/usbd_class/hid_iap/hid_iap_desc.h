/**
  **************************************************************************
  * @file     hid_iap_desc.h
  * @brief    usb hid descriptor header file
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
#ifndef __HID_IAP_DESC_H
#define __HID_IAP_DESC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hid_iap_class.h"
#include "usbd_core.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_hid_iap_desc
  * @{
  */

/** @defgroup USB_hid_iap_desc_definition
  * @{
  */


#define HIDIAP_BCD_NUM                   0x0110

#define USBD_HIDIAP_VENDOR_ID            0x2E3C
#define USBD_HIDIAP_PRODUCT_ID           0xAF01

#define USBD_HIDIAP_CONFIG_DESC_SIZE     41
#define USBD_HIDIAP_SIZ_REPORT_DESC      32
#define USBD_HIDIAP_SIZ_STRING_LANGID    4
#define USBD_HIDIAP_SIZ_STRING_SERIAL    0x1A

#define USBD_HIDIAP_DESC_MANUFACTURER_STRING    "Artery"
#define USBD_HIDIAP_DESC_PRODUCT_STRING         "HID IAP"
#define USBD_HIDIAP_DESC_CONFIGURATION_STRING   "HID IAP Config"
#define USBD_HIDIAP_DESC_INTERFACE_STRING       "HID IAP Interface"

#define HIDIAP_BINTERVAL_TIME                0x01

#define         MCU_ID1                   (0x1FFFF7E8)
#define         MCU_ID2                   (0x1FFFF7EC)
#define         MCU_ID3                   (0x1FFFF7F0)
extern uint8_t g_usbd_hidiap_report[USBD_HIDIAP_SIZ_REPORT_DESC];
extern uint8_t g_hidiap_usb_desc[9];

extern usbd_desc_handler hid_iap_desc_handler;


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

