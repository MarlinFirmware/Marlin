/**
  **************************************************************************
  * @file     usbd_core.h
  * @brief    usb device core header file
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
#ifndef __USBD_CORE_H
#define __USBD_CORE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "usb_conf.h"
#include "usb_std.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_drivers
  * @{
  */

/** @addtogroup USBD_drivers_core
  * @{
  */

/** @defgroup USBD_core_exported_types
  * @{
  */

/**
  * @brief usb device event
  */
typedef enum
{
  USBD_NOP_EVENT,      /*!< usb device event nop */
  USBD_RESET_EVENT,    /*!< usb device event reset */
  USBD_SUSPEND_EVENT,  /*!< usb device event suspend */
  USBD_WAKEUP_EVENT,   /*!< usb device event wakeup */
  USBD_ERR_EVENT       /*!< usb device event error */
}usbd_event_type;

/**
  * @brief usb device descriptor struct
  */
typedef struct
{
  uint16_t length;                       /*!< descriptor length */
  uint8_t *descriptor;                   /*!< descriptor string */
}usbd_desc_t;

/**
  * @brief usb device descriptor handler
  */
typedef struct
{
  usbd_desc_t *(*get_device_descriptor)(void);                       /*!< get device descriptor callback */
  usbd_desc_t *(*get_device_qualifier)(void);                        /*!< get device qualifier callback */
  usbd_desc_t *(*get_device_configuration)(void);                    /*!< get device configuration callback */
  usbd_desc_t *(*get_device_other_speed)(void);                      /*!< get device other speed callback */
  usbd_desc_t *(*get_device_lang_id)(void);                          /*!< get device lang id callback */
  usbd_desc_t *(*get_device_manufacturer_string)(void);              /*!< get device manufacturer callback */
  usbd_desc_t *(*get_device_product_string)(void);                   /*!< get device product callback */
  usbd_desc_t *(*get_device_serial_string)(void);                    /*!< get device serial callback */
  usbd_desc_t *(*get_device_interface_string)(void);                 /*!< get device interface string callback */
  usbd_desc_t *(*get_device_config_string)(void);                    /*!< get device device config callback */
#if (USBD_SUPPORT_WINUSB == 1)
  usbd_desc_t *(*get_device_winusb_os_string)(void);                 /*!< get winusb os string */
  usbd_desc_t *(*get_device_winusb_os_feature)(void);                /*!< get winusb os feature */
  usbd_desc_t *(*get_device_winusb_os_property)(void);               /*!< get winusb os property */
#endif
}usbd_desc_handler;

/**
  * @brief usb device class handler
  */
typedef struct
{
  usb_sts_type (*init_handler)(void *udev);                          /*!< usb class init handler */
  usb_sts_type (*clear_handler)(void *udev);                         /*!< usb class clear handler */
  usb_sts_type (*setup_handler)(void *udev, usb_setup_type *setup);  /*!< usb class setup handler */
  usb_sts_type (*ept0_tx_handler)(void *udev);                       /*!< usb class endpoint 0 tx complete handler */
  usb_sts_type (*ept0_rx_handler)(void *udev);                       /*!< usb class endpoint 0 rx complete handler */
  usb_sts_type (*in_handler)(void *udev, uint8_t ept_num);           /*!< usb class in transfer complete handler */
  usb_sts_type (*out_handler)(void *udev, uint8_t ept_num);          /*!< usb class out transfer complete handler */
  usb_sts_type (*sof_handler)(void *udev);                           /*!< usb class sof handler */
  usb_sts_type (*event_handler)(void *udev, usbd_event_type event);  /*!< usb class event handler */
  void         *pdata;                                               /*!< usb class data pointer */
}usbd_class_handler;

/**
  * @brief usb device core struct type
  */
typedef struct
{
  usb_reg_type *usb_reg;                 /*!< usb register pointer */

  usbd_class_handler *class_handler;     /*!< usb device class handler pointer */
  usbd_desc_handler *desc_handler;       /*!< usb device descriptor handler pointer */

  usb_ept_info ept_in[USB_EPT_MAX_NUM];  /*!< usb in endpoint infomation struct */
  usb_ept_info ept_out[USB_EPT_MAX_NUM]; /*!< usb out endpoint infomation struct */

  usb_setup_type setup;                  /*!< usb setup type struct */
  uint8_t setup_buffer[12];              /*!< usb setup request buffer */

  uint8_t ept0_sts;                      /*!< usb control endpoint 0 state */
  uint8_t speed;                         /*!< usb speed */
  uint16_t ept0_wlength;                 /*!< usb endpoint 0 transfer length */

  usbd_conn_state conn_state;            /*!< usb current connect state */
  usbd_conn_state old_conn_state;        /*!< usb save the previous connect state */

  uint8_t device_addr;                   /*!< device address */
  uint8_t remote_wakup;                  /*!< remote wakeup state */
  uint8_t default_config;                /*!< usb default config state */
  uint8_t dev_config;                    /*!< usb device config state */
  uint16_t config_status;                /*!< usb configure status */
}usbd_core_type;

/**
  * @}
  */

/** @defgroup USBD_core_exported_functions
  * @{
  */
void usbd_core_in_handler(usbd_core_type *udev, uint8_t ept_num);
void usbd_core_out_handler(usbd_core_type *udev, uint8_t ept_num);
void usbd_core_setup_handler(usbd_core_type *udev, uint8_t ept_num);
void usbd_ctrl_unsupport(usbd_core_type *udev);
void usbd_ctrl_send(usbd_core_type *udev, uint8_t *buffer, uint16_t len);
void usbd_ctrl_recv(usbd_core_type *udev, uint8_t *buffer, uint16_t len);
void usbd_ctrl_send_status(usbd_core_type *udev);
void usbd_ctrl_recv_status(usbd_core_type *udev);
void usbd_set_stall(usbd_core_type *udev, uint8_t ept_addr);
void usbd_clear_stall(usbd_core_type *udev, uint8_t ept_addr);
void usbd_ept_open(usbd_core_type *udev, uint8_t ept_addr, uint8_t ept_type, uint16_t maxpacket);
void usbd_ept_close(usbd_core_type *udev, uint8_t ept_addr);
void usbd_ept_send(usbd_core_type *udev, uint8_t ept_num, uint8_t *buffer, uint16_t len);
void usbd_ept_recv(usbd_core_type *udev, uint8_t ept_num, uint8_t *buffer, uint16_t len);
void usbd_connect(usbd_core_type *udev);
void usbd_disconnect(usbd_core_type *udev);
void usbd_set_device_addr(usbd_core_type *udev, uint8_t address);
uint32_t usbd_get_recv_len(usbd_core_type *udev, uint8_t ept_addr);
usbd_conn_state usbd_connect_state_get(usbd_core_type *udev);
void usbd_ept_dbuffer_enable(usbd_core_type *udev, uint8_t ept_addr);
void usbd_ept_buf_auto_define(usb_ept_info *ept_info);
void usbd_ept_buf_custom_define( usbd_core_type *udev, uint8_t ept_addr,
                                 uint32_t addr);
void usbd_ept_defaut_init(usbd_core_type *udev);
void usbd_remote_wakeup(usbd_core_type *udev);
void usbd_enter_suspend(usbd_core_type *udev);
void usbd_core_init(usbd_core_type *udev,
                    usb_reg_type *usb_reg,
                    usbd_class_handler *dev_handler,
                    usbd_desc_handler *desc_handler,
                    uint8_t core_id);


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

