/**
  **************************************************************************
  * @file     cdc_msc_desc.h
  * @brief    usb cdc descriptor header file
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
#ifndef __CDC_MSC_DESC_H
#define __CDC_MSC_DESC_H

#ifdef __cplusplus
extern "C" {
#endif
 
#include "cdc_msc_class.h"
#include "usbd_core.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */
  
/** @addtogroup USB_cdc_msc_desc
  * @{
  */

/** @defgroup USB_cdc_desc_definition 
  * @{
  */
/**
  * @brief usb bcd number define
  */
#define CDC_BCD_NUM                      0x0110

/**
  * @brief usb vendor id and product id define
  */
#define USBD_CDC_MSC_VENDOR_ID               0x2E3C
#define USBD_CDC_MSC_PRODUCT_ID              0x5760 

/**
  * @brief usb descriptor size define
  */

#define USBD_CDC_MSC_CONFIG_DESC_SIZE    106
#define USBD_CDC_SIZ_STRING_LANGID       4
#define USBD_CDC_SIZ_STRING_SERIAL       0x1A

/**
  * @brief usb string define(vendor, product configuration, interface)
  */
#define USBD_CDC_DESC_MANUFACTURER_STRING    "Artery"
#define USBD_CDC_DESC_PRODUCT_STRING         "AT32 Composite VCP and MSC  "
#define USBD_CDC_DESC_CONFIGURATION_STRING   "Composite VCP and MSC Config"
#define USBD_CDC_DESC_INTERFACE_STRING       "Composite VCP and MSC Interface"

/**
  * @brief usb endpoint interval define 
  */
#define CDC_HID_BINTERVAL_TIME                0xFF

/**
  * @brief usb interface define
  */
#define VCPMSC_MSC_INTERFACE                  0x00
#define VCPMSC_CDC_INTERFACE                  0x01
#define VCPMSC_CDC_DATA_INTERFACE             0x02


/**
  * @brief usb mcu id address deine
  */
#define         MCU_ID1                   (0x1FFFF7E8)
#define         MCU_ID2                   (0x1FFFF7EC)
#define         MCU_ID3                   (0x1FFFF7F0)
/**
  * @}
  */
  
extern usbd_desc_handler cdc_msc_desc_handler;


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
