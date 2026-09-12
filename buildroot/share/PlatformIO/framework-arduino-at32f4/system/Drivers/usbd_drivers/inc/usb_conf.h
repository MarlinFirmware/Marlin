/**
  **************************************************************************
  * @file     usb_conf.h
  * @brief    usb config header file
  **************************************************************************
  *
  * Copyright (c) 2025, Artery Technology, All rights reserved.
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
#ifndef __USB_CONF_H
#define __USB_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f403a_407.h"
#include "stdio.h"

/** @addtogroup AT32F407_periph_examples
  * @{
  */

/** @addtogroup 407_USB_device_vcp_loopback
  * @{
  */

/**
  * @brief usb endpoint max num define
  */
#ifndef USB_EPT_MAX_NUM
#define USB_EPT_MAX_NUM                   8  /*!< usb device support endpoint number */
#endif

/**
  * @brief usb buffer extend to 768-1280 bytes
  */
//#define USB_BUFFER_SIZE_EX  /*!< usb enable extend buffer */


/**
  * @brief auto malloc usb endpoint buffer
  */
//#define USB_EPT_AUTO_MALLOC_BUFFER  /*!< usb auto malloc endpoint tx and rx buffer */

/**
  * @brief usb virtual comport
  */
#define USB_VIRTUAL_COMPORT


#ifndef USB_EPT_AUTO_MALLOC_BUFFER
/**
  * @brief user custom endpoint buffer
  *        EPTn_TX_ADDR, EPTn_RX_ADDR must less than usb buffer size
  */

/* ept0 tx start address 0x40, size 0x40,
   so rx start address is 0x40 + 0x40 = 0x80 */
#define EPT0_TX_ADDR                     0x40    /*!< usb endpoint 0 tx buffer address offset */
#define EPT0_RX_ADDR                     0x80    /*!< usb endpoint 0 rx buffer address offset */

#define EPT1_TX_ADDR                     0xC0    /*!< usb endpoint 1 tx buffer address offset */
#define EPT1_RX_ADDR                     0x100   /*!< usb endpoint 1 rx buffer address offset */

#define EPT2_TX_ADDR                     0x140   /*!< usb endpoint 2 tx buffer address offset */
#define EPT2_RX_ADDR                     0x180   /*!< usb endpoint 2 rx buffer address offset */

#define EPT3_TX_ADDR                     0x00    /*!< usb endpoint 3 tx buffer address offset */
#define EPT3_RX_ADDR                     0x00    /*!< usb endpoint 3 rx buffer address offset */

#define EPT4_TX_ADDR                     0x00    /*!< usb endpoint 4 tx buffer address offset */
#define EPT4_RX_ADDR                     0x00    /*!< usb endpoint 4 rx buffer address offset */

#define EPT5_TX_ADDR                     0x00    /*!< usb endpoint 5 tx buffer address offset */
#define EPT5_RX_ADDR                     0x00    /*!< usb endpoint 5 rx buffer address offset */

#define EPT6_TX_ADDR                     0x00    /*!< usb endpoint 6 tx buffer address offset */
#define EPT6_RX_ADDR                     0x00    /*!< usb endpoint 6 rx buffer address offset */

#define EPT7_TX_ADDR                     0x00    /*!< usb endpoint 7 tx buffer address offset */
#define EPT7_RX_ADDR                     0x00    /*!< usb endpoint 7 rx buffer address offset */

#endif

void usb_delay_ms(uint32_t ms);
void usb_delay_us(uint32_t us);

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
