/**
  **************************************************************************
  * @file     custom_hid_desc.h
  * @brief    usb custom hid descriptor header file
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
#ifndef __CUSTOM_HID_DESC_H
#define __CUSTOM_HID_DESC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "custom_hid_class.h"
#include "usbd_core.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_custom_hid_desc
  * @{
  */

/** @defgroup USB_custom_hid_desc_definition
  * @{
  */


/**
  * @brief usb bcd number define
  */
#define CUSHID_BCD_NUM                   0x0110

/**
  * @brief usb vendor id and product id define
  */
#define USBD_CUSHID_VENDOR_ID            0x2E3C
#define USBD_CUSHID_PRODUCT_ID           0x5745

/**
  * @brief usb descriptor size define
  */
#define USBD_CUSHID_CONFIG_DESC_SIZE     41
#define USBD_CUSHID_SIZ_REPORT_DESC      126
#define USBD_CUSHID_SIZ_STRING_LANGID    4
#define USBD_CUSHID_SIZ_STRING_SERIAL    0x1A

/**
  * @brief usb string define(vendor, product configuration, interface)
  */
#define USBD_CUSHID_DESC_MANUFACTURER_STRING    "Artery"
#define USBD_CUSHID_DESC_PRODUCT_STRING         "Custom HID"
#define USBD_CUSHID_DESC_CONFIGURATION_STRING   "Custom HID Config"
#define USBD_CUSHID_DESC_INTERFACE_STRING       "Custom HID Interface"

#define CUSHID_BINTERVAL_TIME            0x0A

/**
  * @brief usb hid report id define
  */
/**
  * @brief usb hid report id define
  */
#define HID_REPORT_ID_1                   0x01
#define HID_REPORT_ID_2                   0x02
#define HID_REPORT_ID_3                   0x03
#define HID_REPORT_ID_4                   0x04
#define HID_REPORT_ID_5                   0x05
#define HID_REPORT_ID_6                   0xF0

/**
  * @brief usb mcu id address deine
  */
#define MCU_ID1                          (0x1FFFF7E8)
#define MCU_ID2                          (0x1FFFF7EC)
#define MCU_ID3                          (0x1FFFF7F0)

/**
  * @}
  */
extern uint8_t g_usbd_custom_hid_report[USBD_CUSHID_SIZ_REPORT_DESC];
extern uint8_t g_custom_hid_usb_desc[9];
extern usbd_desc_handler custom_hid_desc_handler;

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


