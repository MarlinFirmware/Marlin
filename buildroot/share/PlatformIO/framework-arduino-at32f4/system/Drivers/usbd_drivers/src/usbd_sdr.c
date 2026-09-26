/**
  **************************************************************************
  * @file     usbd_sdr.c
  * @brief    usb standard device request
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
#include "usbd_sdr.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_drivers
  * @{
  */

/** @defgroup USBD_drivers_standard_request
  * @brief usb device standard_request
  * @{
  */

/** @defgroup USBD_sdr_private_functions
  * @{
  */

static usb_sts_type usbd_get_descriptor(usbd_core_type *udev);
static usb_sts_type usbd_set_address(usbd_core_type *udev);
static usb_sts_type usbd_get_status(usbd_core_type *udev);
static usb_sts_type usbd_clear_feature(usbd_core_type *udev);
static usb_sts_type usbd_set_feature(usbd_core_type *udev);
static usb_sts_type usbd_get_configuration(usbd_core_type *udev);
static usb_sts_type usbd_set_configuration(usbd_core_type *udev);

/**
  * @brief  usb parse standard setup request
  * @param  setup: setup structure
  * @param  buf: setup buffer
  * @retval none
  */
void usbd_setup_request_parse(usb_setup_type *setup, uint8_t *buf)
{
  setup->bmRequestType         = *(uint8_t *) buf;
  setup->bRequest              = *(uint8_t *) (buf + 1);
  setup->wValue                = SWAPBYTE(buf + 2);
  setup->wIndex                = SWAPBYTE(buf + 4);
  setup->wLength               = SWAPBYTE(buf + 6);
}

/**
  * @brief  get usb standard device description request
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type usbd_get_descriptor(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  uint16_t len = 0;
  usbd_desc_t *desc = NULL;
  uint8_t desc_type = udev->setup.wValue >> 8;
  switch(desc_type)
  {
    case USB_DESCIPTOR_TYPE_DEVICE:
      desc = udev->desc_handler->get_device_descriptor();
      break;
    case USB_DESCIPTOR_TYPE_CONFIGURATION:
      desc = udev->desc_handler->get_device_configuration();
      break;
    case USB_DESCIPTOR_TYPE_STRING:
    {
      uint8_t str_desc = (uint8_t)udev->setup.wValue;
      switch(str_desc)
      {
        case USB_LANGID_STRING:
          desc = udev->desc_handler->get_device_lang_id();
          break;
        case USB_MFC_STRING:
          desc = udev->desc_handler->get_device_manufacturer_string();
          break;
        case USB_PRODUCT_STRING:
          desc = udev->desc_handler->get_device_product_string();
          break;
        case USB_SERIAL_STRING:
          desc = udev->desc_handler->get_device_serial_string();
          break;
        case USB_CONFIG_STRING:
          desc = udev->desc_handler->get_device_config_string();
          break;
        case USB_INTERFACE_STRING:
          desc = udev->desc_handler->get_device_interface_string();
          break;
        case USB_WINUSB_OS_STRING:
#if (USBD_SUPPORT_WINUSB == 1)
          if(udev->desc_handler->get_device_winusb_os_string != NULL)
          {
            desc = udev->desc_handler->get_device_winusb_os_string();
          }
          else
          {
            usbd_ctrl_unsupport(udev);
          }
#else
          usbd_ctrl_unsupport(udev);
#endif
          break;
        default:
          udev->class_handler->setup_handler(udev, &udev->setup);
          return ret;
      }
      break;
    }
    case USB_DESCIPTOR_TYPE_DEVICE_QUALIFIER:
      usbd_ctrl_unsupport(udev);
      break;
    case USB_DESCIPTOR_TYPE_OTHER_SPEED:
      usbd_ctrl_unsupport(udev);
      return ret;
    default:
      usbd_ctrl_unsupport(udev);
      return ret;
  }

  if(desc != NULL)
  {
    if((desc->length != 0) && (udev->setup.wLength != 0))
    {
      len = MIN(desc->length , udev->setup.wLength);
      usbd_ctrl_send(udev, desc->descriptor, len);
    }
  }
  return ret;
}

/**
  * @brief  this request sets the device address
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type usbd_set_address(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  usb_setup_type *setup = &udev->setup;
  uint8_t dev_addr;

  /* if wIndex or wLength are non-zero, then the behavior of
     the device is not specified
  */
  if(setup->wIndex == 0 && setup->wLength == 0)
  {
    dev_addr = (uint8_t)(setup->wValue) & 0x7f;

    /* device behavior when this request is received
       while the device is in the configured state is not specified.*/
    if(udev->conn_state == USB_CONN_STATE_CONFIGURED )
    {
      usbd_ctrl_unsupport(udev);
    }
    else
    {
      udev->device_addr = dev_addr;

      if(dev_addr != 0)
      {
        udev->conn_state = USB_CONN_STATE_ADDRESSED;
      }
      else
      {
        udev->conn_state = USB_CONN_STATE_DEFAULT;
      }
      usbd_ctrl_send_status(udev);
    }
  }
  else
  {
    usbd_ctrl_unsupport(udev);
  }
  return ret;
}

/**
  * @brief  get usb status request
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type usbd_get_status(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  switch(udev->conn_state)
  {
    case USB_CONN_STATE_ADDRESSED:
    case USB_CONN_STATE_CONFIGURED:
      if(udev->remote_wakup)
      {
        udev->config_status |= USB_CONF_REMOTE_WAKEUP;
      }
      usbd_ctrl_send(udev, (uint8_t *)(&udev->config_status), 2);
      break;
    default:
      usbd_ctrl_unsupport(udev);
      break;
  }
  return ret;
}

/**
  * @brief  clear usb feature request
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type usbd_clear_feature(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  usb_setup_type *setup = &udev->setup;
  switch(udev->conn_state)
  {
    case USB_CONN_STATE_ADDRESSED:
    case USB_CONN_STATE_CONFIGURED:
      if(setup->wValue == USB_FEATURE_REMOTE_WAKEUP)
      {
        udev->remote_wakup = 0;
        udev->config_status &= ~USB_CONF_REMOTE_WAKEUP;
        udev->class_handler->setup_handler(udev, &udev->setup);
        usbd_ctrl_send_status(udev);
      }
      break;
    default:
      usbd_ctrl_unsupport(udev);
      break;
  }
  return ret;
}

/**
  * @brief  set usb feature request
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type usbd_set_feature(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  usb_setup_type *setup = &udev->setup;
  if(setup->wValue == USB_FEATURE_REMOTE_WAKEUP)
  {
    udev->remote_wakup = 1;
    udev->class_handler->setup_handler(udev, &udev->setup);
    usbd_ctrl_send_status(udev);
  }
  else
  {
    usbd_ctrl_unsupport(udev);
  }
  return ret;
}

/**
  * @brief  get usb configuration request
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type usbd_get_configuration(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  usb_setup_type *setup = &udev->setup;
  if(setup->wLength != 1)
  {
    usbd_ctrl_unsupport(udev);
  }
  else
  {
    switch(udev->conn_state)
    {
      case USB_CONN_STATE_ADDRESSED:
        udev->default_config = 0;
        usbd_ctrl_send(udev, (uint8_t *)(&udev->default_config), 1);
        break;
      case USB_CONN_STATE_CONFIGURED:
        usbd_ctrl_send(udev, (uint8_t *)(&udev->dev_config), 1);
        break;
      default:
        usbd_ctrl_unsupport(udev);
        break;
    }
  }
  return ret;
}

/**
  * @brief  sets the usb device configuration request
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
static usb_sts_type usbd_set_configuration(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  static uint8_t config_value;
  usb_setup_type *setup = &udev->setup;
  config_value = (uint8_t)setup->wValue;

  if(setup->wIndex == 0 && setup->wLength == 0)
  {
    switch(udev->conn_state)
    {
      case USB_CONN_STATE_ADDRESSED:
        if(config_value)
        {
          udev->dev_config = config_value;
          udev->conn_state = USB_CONN_STATE_CONFIGURED;
          udev->class_handler->init_handler(udev);
          usbd_ctrl_send_status(udev);
        }
        else
        {
          usbd_ctrl_send_status(udev);
        }

        break;
      case USB_CONN_STATE_CONFIGURED:
        if(config_value == 0)
        {
          udev->conn_state = USB_CONN_STATE_ADDRESSED;
          udev->dev_config = config_value;
          udev->class_handler->clear_handler(udev);
          usbd_ctrl_send_status(udev);
        }
        else if(config_value != udev->dev_config)
        {
          udev->class_handler->clear_handler(udev);
          udev->dev_config = config_value;
          udev->class_handler->init_handler(udev);
          usbd_ctrl_send_status(udev);
        }
        else
        {
          usbd_ctrl_send_status(udev);
        }
        break;
      default:
        usbd_ctrl_unsupport(udev);
        break;
    }
  }
  else
  {
    usbd_ctrl_unsupport(udev);
  }
  return ret;
}

/**
  * @brief  standard usb device requests
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
usb_sts_type usbd_device_request(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  usb_setup_type *setup = &udev->setup;
  if((setup->bmRequestType & USB_REQ_TYPE_RESERVED) != USB_REQ_TYPE_STANDARD)
  {
    udev->class_handler->setup_handler(udev, &udev->setup);
    return ret;
  }
  switch(udev->setup.bRequest)
  {
    case USB_STD_REQ_GET_STATUS:
      usbd_get_status(udev);
      break;
    case USB_STD_REQ_CLEAR_FEATURE:
      usbd_clear_feature(udev);
      break;
    case USB_STD_REQ_SET_FEATURE:
      usbd_set_feature(udev);
      break;
    case USB_STD_REQ_SET_ADDRESS:
      usbd_set_address(udev);
      break;
    case USB_STD_REQ_GET_DESCRIPTOR:
      usbd_get_descriptor(udev);
      break;
    case USB_STD_REQ_GET_CONFIGURATION:
      usbd_get_configuration(udev);
      break;
    case USB_STD_REQ_SET_CONFIGURATION:
      usbd_set_configuration(udev);
      break;
    default:
      usbd_ctrl_unsupport(udev);
      break;
  }
  return ret;
}

/**
  * @brief  standard usb interface requests
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
usb_sts_type usbd_interface_request(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  usb_setup_type *setup = &udev->setup;
  switch(udev->conn_state)
  {
    case USB_CONN_STATE_DEFAULT:
    case USB_CONN_STATE_ADDRESSED:
    case USB_CONN_STATE_CONFIGURED:
      ret = udev->class_handler->setup_handler(udev, &udev->setup);
      if(setup->wLength == 0 && ret == USB_OK)
      {
        usbd_ctrl_send_status(udev);
      }
      break;
    default:
      usbd_ctrl_unsupport(udev);
      break;
  }
  return ret;
}

/**
  * @brief  standard usb endpoint requests
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type
  */
usb_sts_type usbd_endpoint_request(usbd_core_type *udev)
{
  usb_sts_type ret = USB_OK;
  usb_setup_type *setup = &udev->setup;
  uint8_t ept_addr = LBYTE(setup->wIndex);
  usb_ept_info *ept_info;

  if((setup->bmRequestType & USB_REQ_TYPE_RESERVED) != USB_REQ_TYPE_STANDARD)
  {
    udev->class_handler->setup_handler(udev, &udev->setup);
    return ret;
  }
  switch(setup->bRequest)
  {
    case USB_STD_REQ_GET_STATUS:
      switch(udev->conn_state)
      {
        case USB_CONN_STATE_ADDRESSED:
          if((ept_addr & 0x7F) != 0)
          {
            usbd_set_stall(udev, ept_addr);
          }
          if((ept_addr & 0x80) != 0)
          {
           ept_info = &udev->ept_in[ept_addr & 0x7F];
          }
          else
          {
            ept_info = &udev->ept_out[ept_addr & 0x7F];
          }
          ept_info->status = 0x0000;
          usbd_ctrl_send(udev, (uint8_t *)(&ept_info->status), 2);
          
          break;
        case USB_CONN_STATE_CONFIGURED:
        {
          if((ept_addr & 0x80) != 0)
          {
           ept_info = &udev->ept_in[ept_addr & 0x7F];
          }
          else
          {
            ept_info = &udev->ept_out[ept_addr & 0x7F];
          }
          if(ept_info->stall == 1)
          {
            ept_info->status = 0x0001;
          }
          else
          {
            ept_info->status = 0x0000;
          }
          usbd_ctrl_send(udev, (uint8_t *)(&ept_info->status), 2);
        }
          break;
        default:
          usbd_ctrl_unsupport(udev);
          break;
      }
      break;
    case USB_STD_REQ_CLEAR_FEATURE:
      switch(udev->conn_state)
      {
        case USB_CONN_STATE_ADDRESSED:
          if((ept_addr != 0x00) && (ept_addr != 0x80))
          {
            usbd_set_stall(udev, ept_addr);
          }
          break;
        case USB_CONN_STATE_CONFIGURED:
          if(setup->wValue == USB_FEATURE_EPT_HALT)
          {
            if((ept_addr & 0x7F) != 0x00 )
            {
              usbd_clear_stall(udev, ept_addr);
              udev->class_handler->setup_handler(udev, &udev->setup);
            }
            usbd_ctrl_send_status(udev);
          }
          break;
        default:
          usbd_ctrl_unsupport(udev);
          break;
      }
      break;
    case USB_STD_REQ_SET_FEATURE:
      switch(udev->conn_state)
      {
        case USB_CONN_STATE_ADDRESSED:
          if((ept_addr != 0x00) && (ept_addr != 0x80))
          {
            usbd_set_stall(udev, ept_addr);
          }
          break;
        case USB_CONN_STATE_CONFIGURED:
          if(setup->wValue == USB_FEATURE_EPT_HALT)
          {
            if((ept_addr != 0x00) && (ept_addr != 0x80))
            {
              usbd_set_stall(udev, ept_addr);
            }
          }
          udev->class_handler->setup_handler(udev, &udev->setup);
          usbd_ctrl_send_status(udev);
          break;
        default:
          usbd_ctrl_unsupport(udev);
          break;
      }
      break;
    default:
      usbd_ctrl_unsupport(udev);
      break;
  }
  return ret;
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

