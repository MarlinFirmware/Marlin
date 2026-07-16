/**
  **************************************************************************
  * @file     usb_int.h
  * @brief    usb header file
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
#ifndef __USB_INT_H
#define __USB_INT_H

#ifdef __cplusplus
extern "C" {
#endif


/* includes ------------------------------------------------------------------*/
#include "usbd_core.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_drivers
  * @{
  */

/** @addtogroup USBD_drivers_interrupt
  * @{
  */

/** @defgroup USBD_int_exported_functions
  * @{
  */
void usbd_irq_handler(usbd_core_type *udev);
void usbd_ept_loop_handler(usbd_core_type *udev);
void usbd_eptn_handler(usbd_core_type *udev, usb_ept_number_type ept_num);
void usbd_reset_handler(usbd_core_type *udev);
void usbd_sof_handler(usbd_core_type *udev);
void usbd_suspend_handler(usbd_core_type *udev);
void usbd_wakeup_handler(usbd_core_type *udev);

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

