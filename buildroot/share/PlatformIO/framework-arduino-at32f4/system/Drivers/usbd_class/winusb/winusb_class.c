/**
  **************************************************************************
  * @file     winusb_class.c
  * @brief    usb winusb class type
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
#include "winusb_class.h"
#include "winusb_desc.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @defgroup USB_winusb_class
  * @brief usb device class win usb
  * @{
  */

/** @defgroup USB_winusb_class_private_functions
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
#if (USBD_SUPPORT_WINUSB == 1)
static usb_sts_type usbd_get_winusb_descriptor(usbd_core_type *udev);
#endif
static usb_sts_type winusb_struct_init(winusb_struct_type *p_winusb);

/* winusb data struct */
winusb_struct_type winusb_struct;

/*winusb receive buffer define*/
static uint32_t g_winusb_rx_buffer[16];

/* usb device class handler */
usbd_class_handler winusb_class_handler =
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
  &winusb_struct
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
  winusb_struct_type *p_winusb = (winusb_struct_type *)pudev->class_handler->pdata;

  /* init winusb struct */
  winusb_struct_init(p_winusb);
 
  p_winusb->maxpacket = USBD_FS_WINUSB_MAXPACKET_SIZE;
  
#ifndef USB_EPT_AUTO_MALLOC_BUFFER
  /* use user define buffer address */
  usbd_ept_buf_custom_define(pudev, USBD_WINUSB_BULK_IN_EPT, EPT1_TX_ADDR);
  usbd_ept_buf_custom_define(pudev, USBD_WINUSB_BULK_OUT_EPT, EPT1_RX_ADDR);
#endif
   
  /* open out endpoint */
  usbd_ept_open(pudev, USBD_WINUSB_BULK_OUT_EPT, EPT_BULK_TYPE, USBD_WINUSB_OUT_MAXPACKET_SIZE);

  /* open in endpoint */
  usbd_ept_open(pudev, USBD_WINUSB_BULK_IN_EPT, EPT_BULK_TYPE, USBD_WINUSB_IN_MAXPACKET_SIZE);
  
  /* set out endpoint to receive status */
  usbd_ept_recv(pudev, USBD_WINUSB_BULK_OUT_EPT, p_winusb->g_rx_buff, USBD_WINUSB_OUT_MAXPACKET_SIZE);

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
  usbd_ept_close(pudev, USBD_WINUSB_BULK_IN_EPT);

  /* close out endpoint */
  usbd_ept_close(pudev, USBD_WINUSB_BULK_OUT_EPT);

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
  winusb_struct_type *p_winusb = (winusb_struct_type *)pudev->class_handler->pdata;

  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
  {
    /* class request */
    case USB_REQ_TYPE_CLASS:
      usbd_ctrl_unsupport(pudev);
      break;
    /* standard request */
    case USB_REQ_TYPE_STANDARD:
      switch(setup->bRequest)
      {
        case USB_STD_REQ_GET_DESCRIPTOR:
          usbd_ctrl_unsupport(pudev);
          break;
        case USB_STD_REQ_GET_INTERFACE:
          usbd_ctrl_send(pudev, (uint8_t *)&p_winusb->alt_setting, 1);
          break;
        case USB_STD_REQ_SET_INTERFACE:
          p_winusb->alt_setting = setup->wValue;
          break;
        case USB_STD_REQ_CLEAR_FEATURE:
          break;
        case USB_STD_REQ_SET_FEATURE:
          break;
        default:
          usbd_ctrl_unsupport(pudev);
          break;
      }
      break;
#if (USBD_SUPPORT_WINUSB == 1)      
    case USB_REQ_TYPE_VENDOR:
      if(setup->bRequest == WINUSB_BMS_VENDOR_CODE)
      {
        usbd_get_winusb_descriptor(pudev);
      }
      else
      {
        usbd_ctrl_unsupport(pudev);
      }
      break;
#endif
    default:
      usbd_ctrl_unsupport(pudev);
      break;
  }
  return status;
}

#if (USBD_SUPPORT_WINUSB == 1)
static usb_sts_type usbd_get_winusb_descriptor(usbd_core_type *udev)
{
	usb_sts_type ret = USB_OK;
  uint16_t len = 0;
  usbd_desc_t *desc = NULL;
  uint8_t desc_type = udev->setup.wIndex;
	
	switch(desc_type)
  {
		case USB_WINUSB_COMPAT_ID:
			desc = udev->desc_handler->get_device_winusb_os_feature();
		  break;
		case USB_WINUSB_PROPERTIES_ID:
			desc = udev->desc_handler->get_device_winusb_os_property();
		  break;
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
#endif

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
  usbd_core_type *pudev = (usbd_core_type *)udev;
  winusb_struct_type *p_winusb = (winusb_struct_type *)pudev->class_handler->pdata;
  usb_sts_type status = USB_OK;

  /* ...user code...
    trans next packet data
  */
  p_winusb->g_tx_completed = 1;

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
  usbd_core_type *pudev = (usbd_core_type *)udev;
  winusb_struct_type *p_winusb = (winusb_struct_type *)pudev->class_handler->pdata;

  /* get endpoint receive data length  */
  p_winusb->g_rxlen = usbd_get_recv_len(pudev, ept_num);

  /*set recv flag*/
  p_winusb->g_rx_completed = 1;

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
  * @brief  usb device winusb init
  * @param  p_winusb: to the structure of winusb_struct
  * @retval status of usb_sts_type
  */
static usb_sts_type winusb_struct_init(winusb_struct_type *p_winusb)
{
  p_winusb->g_tx_completed = 1;
  p_winusb->g_rx_completed = 0;
  p_winusb->alt_setting = 0;
  p_winusb->g_rx_buff = (uint8_t *)g_winusb_rx_buffer;
  return USB_OK;
}

/**
  * @brief  usb device class rx data process
  * @param  udev: to the structure of usbd_core_type
  * @param  recv_data: receive buffer
  * @retval receive data len
  */
uint16_t usb_winusb_get_rxdata(void *udev, uint8_t *recv_data)
{
  uint16_t i_index = 0;
  uint16_t tmp_len = 0;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  winusb_struct_type *p_winusb = (winusb_struct_type *)pudev->class_handler->pdata;

  if(p_winusb->g_rx_completed == 0)
  {
    return 0;
  }
  p_winusb->g_rx_completed = 0;
  tmp_len = p_winusb->g_rxlen;
  for(i_index = 0; i_index < p_winusb->g_rxlen; i_index ++)
  {
    recv_data[i_index] = p_winusb->g_rx_buff[i_index];
  }

  usbd_ept_recv(pudev, USBD_WINUSB_BULK_OUT_EPT, p_winusb->g_rx_buff, p_winusb->maxpacket);

  return tmp_len;
}

/**
  * @brief  usb device class send data
  * @param  udev: to the structure of usbd_core_type
  * @param  send_data: send data buffer
  * @param  len: send length
  * @retval error status
  */
error_status usb_winusb_send_data(void *udev, uint8_t *send_data, uint16_t len)
{
  error_status status = SUCCESS;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  winusb_struct_type *p_winusb = (winusb_struct_type *)pudev->class_handler->pdata;
  if(p_winusb->g_tx_completed)
  {
    p_winusb->g_tx_completed = 0;
    usbd_ept_send(pudev, USBD_WINUSB_BULK_IN_EPT, send_data, len);
  }
  else
  {
    status = ERROR;
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

