/**
  **************************************************************************
  * @file     printer_class.h
  * @brief    usb printer class file
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
#ifndef __PRINTER_CLASS_H
#define __PRINTER_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_std.h"
#include "usbd_core.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @addtogroup USB_printer_class
  * @{
  */

/** @defgroup USB_printer_class_definition
  * @{
  */

#define USBD_PRINTER_BULK_IN_EPT         0x81
#define USBD_PRINTER_BULK_OUT_EPT        0x01


#define USBD_PRINTER_IN_MAXPACKET_SIZE   0x40
#define USBD_PRINTER_OUT_MAXPACKET_SIZE  0x40

#define PRINTER_DEVICE_ID_LEN            24

typedef enum
{
  PRINTER_REQ_GET_DEVICE_ID               = 0x00,
  PRINTER_REQ_GET_PORT_STATUS             = 0x01,
  PRINTER_REQ_GET_SOFT_RESET              = 0x02
}printer_req_type;


typedef struct
{
  uint32_t alt_setting;
  uint32_t g_printer_port_status;
  uint8_t g_rx_buff[USBD_PRINTER_OUT_MAXPACKET_SIZE];
  uint8_t g_printer_data[USBD_PRINTER_OUT_MAXPACKET_SIZE];
  __IO uint8_t g_tx_completed;
  __IO uint8_t g_rx_completed;
  uint32_t g_rxlen;
}printer_type;

extern usbd_class_handler printer_class_handler;
uint16_t usb_printer_get_rxdata(void *udev, uint8_t *recv_data);
error_status usb_printer_send_data(void *udev, uint8_t *send_data, uint16_t len);
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

