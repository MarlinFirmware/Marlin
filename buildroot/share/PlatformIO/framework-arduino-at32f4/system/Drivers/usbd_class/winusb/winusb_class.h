/**
  **************************************************************************
  * @file     winusb_class.h
  * @brief    usb winusb class file
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
#ifndef __WINUSB_CLASS_H
#define __WINUSB_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_std.h"
#include "usbd_core.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_winusb_class
  * @{
  */

/** @defgroup USB_winusb_class_definition
  * @{
  */

/**
  * @brief usb use endpoint define
  */
#define USBD_WINUSB_BULK_IN_EPT             0x81
#define USBD_WINUSB_BULK_OUT_EPT            0x01

/**
  * @brief usb in and out max packet size define
  */

#define USBD_FS_WINUSB_MAXPACKET_SIZE        0x40
#define USBD_WINUSB_IN_MAXPACKET_SIZE        USBD_FS_WINUSB_MAXPACKET_SIZE
#define USBD_WINUSB_OUT_MAXPACKET_SIZE       USBD_FS_WINUSB_MAXPACKET_SIZE

#define WINUSB_BMS_VENDOR_CODE            0xA0

/**
  * @}
  */

/** @defgroup USB_winusb_class_exported_types
  * @{
  */

/**
  * @brief usb winusb class struct
  */
typedef struct
{
  uint32_t alt_setting;
  uint8_t *g_rx_buff;
  uint16_t g_len, g_rxlen;
  __IO uint8_t g_tx_completed, g_rx_completed;
  uint32_t maxpacket;
}winusb_struct_type;


/**
  * @}
  */

/** @defgroup USB_cdc_class_exported_functions
  * @{
  */
extern usbd_class_handler winusb_class_handler;
uint16_t usb_winusb_get_rxdata(void *udev, uint8_t *recv_data);
error_status usb_winusb_send_data(void *udev, uint8_t *send_data, uint16_t len);

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




