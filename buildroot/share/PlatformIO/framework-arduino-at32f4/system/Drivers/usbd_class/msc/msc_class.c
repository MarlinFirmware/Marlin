/**
  **************************************************************************
  * @file     msc_class.c
  * @brief    usb msc class type
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
#include "usbd_core.h"
#include "msc_class.h"
#include "msc_desc.h"
#include "msc_bot_scsi.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @defgroup USB_msc_class
  * @brief usb device class msc demo
  * @{
  */

/** @defgroup USB_msc_class_private_functions
  * @{
  */

static usb_sts_type class_init_handler(void *udev);
static usb_sts_type class_clear_handler(void *udev);
static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup);
static usb_sts_type class_ept0_tx_handler(void *udev);
static usb_sts_type class_ept0_rx_handler(void *udev);
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num);
static usb_sts_type class_sof_handler(void *udev);
static usb_sts_type class_event_handler(void *udev, usbd_event_type event);

msc_type msc_struct;

/* usb device class handler */
usbd_class_handler msc_class_handler =
{
  class_init_handler,
  class_clear_handler,
  class_setup_handler,
  class_ept0_tx_handler,
  class_ept0_rx_handler,
  class_in_handler,
  class_out_handler,
  class_sof_handler,
  class_event_handler,
  &msc_struct
};

/**
  * @brief  initialize usb endpoint
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_init_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;

  /* open in endpoint */
  usbd_ept_open(pudev, USBD_MSC_BULK_IN_EPT, EPT_BULK_TYPE, USBD_OUT_MAXPACKET_SIZE);

  /* open out endpoint */
  usbd_ept_open(pudev, USBD_MSC_BULK_OUT_EPT, EPT_BULK_TYPE, USBD_OUT_MAXPACKET_SIZE);

  bot_scsi_init(udev);

  return status;
}

/**
  * @brief  clear endpoint or other state
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_clear_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;

  /* close in endpoint */
  usbd_ept_close(pudev, USBD_MSC_BULK_IN_EPT);

  /* close out endpoint */
  usbd_ept_close(pudev, USBD_MSC_BULK_OUT_EPT);

  return status;
}

/**
  * @brief  usb device class setup request handler
  * @param  udev: to the structure of usbd_core_type
  * @param  setup: setup packet
  * @retval status of usb_sts_type
  */
static usb_sts_type class_setup_handler(void *udev, usb_setup_type *setup)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  msc_type *pmsc = (msc_type *)pudev->class_handler->pdata;
  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
  {
    /* class request */
    case USB_REQ_TYPE_CLASS:

      switch(setup->bRequest)
      {
        case MSC_REQ_GET_MAX_LUN:
          usbd_ctrl_send(pudev, (uint8_t *)&pmsc->max_lun, 1);
          break;
        case MSC_REQ_BO_RESET:
          bot_scsi_reset(udev);
          usbd_ctrl_send_status(pudev);
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;

      }
      break;
    /* standard request */
    case USB_REQ_TYPE_STANDARD:

      switch(setup->bRequest)
      {
        case USB_STD_REQ_GET_DESCRIPTOR:
          usbd_ctrl_unsupport(pudev);
          break;
        case USB_STD_REQ_GET_INTERFACE:
          usbd_ctrl_send(pudev, (uint8_t *)&pmsc->alt_setting, 1);
          break;
        case USB_STD_REQ_SET_INTERFACE:
          pmsc->alt_setting = setup->wValue;
          break;
        case USB_STD_REQ_CLEAR_FEATURE:
          usbd_ept_close(pudev, (uint8_t)setup->wIndex);

          if((setup->wIndex & 0x80) == 0x80)
          {
            usbd_ept_open(pudev, (uint8_t)setup->wIndex, EPT_BULK_TYPE, USBD_IN_MAXPACKET_SIZE);
          }
          else
          {
            usbd_ept_open(pudev, (uint8_t)setup->wIndex, EPT_BULK_TYPE, USBD_OUT_MAXPACKET_SIZE);
          }
          bot_scsi_clear_feature(udev, setup->wIndex);
          break;
        case USB_STD_REQ_SET_FEATURE:
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;
    default:
      usbd_ctrl_unsupport(pudev);
      break;
  }
  return status;
}

/**
  * @brief  usb device endpoint 0 in status stage complete
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_tx_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */

  return status;
}

/**
  * @brief  usb device endpoint 0 out status stage complete
  * @param  udev: usb device core handler type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_ept0_rx_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  uint32_t recv_len = usbd_get_recv_len(pudev, 0);
  /* ...user code... */
  return status;
}

/**
  * @brief  usb device transmision complete handler
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_in_handler(void *udev, uint8_t ept_num)
{
  usb_sts_type status = USB_OK;

  bot_scsi_datain_handler(udev, ept_num);
  return status;
}

/**
  * @brief  usb device endpoint receive data
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval status of usb_sts_type
  */
static usb_sts_type class_out_handler(void *udev, uint8_t ept_num)
{
  usb_sts_type status = USB_OK;
  bot_scsi_dataout_handler(udev, ept_num);
  return status;
}

/**
  * @brief  usb device sof handler
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type class_sof_handler(void *udev)
{
  usb_sts_type status = USB_OK;

  /* ...user code... */

  return status;
}

/**
  * @brief  usb device event handler
  * @param  udev: to the structure of usbd_core_type
  * @param  event: usb device event
  * @retval status of usb_sts_type
  */
static usb_sts_type class_event_handler(void *udev, usbd_event_type event)
{
  usb_sts_type status = USB_OK;
  switch(event)
  {
    case USBD_RESET_EVENT:

      /* ...user code... */

      break;
    case USBD_SUSPEND_EVENT:

      /* ...user code... */

      break;
    case USBD_WAKEUP_EVENT:
      /* ...user code... */

      break;

    default:
      break;
  }
  return status;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

