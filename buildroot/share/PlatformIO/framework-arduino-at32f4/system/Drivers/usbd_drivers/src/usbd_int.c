/**
  **************************************************************************
  * @file     usbd_int.c
  * @brief    usb interrupt request
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
#include "usbd_int.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_drivers
  * @{
  */

/** @defgroup USBD_drivers_interrupt
  * @brief usb device interrupt
  * @{
  */

/** @defgroup USBD_int_private_functions
  * @{
  */

/**
  * @brief  usb device interrput request handler.
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_irq_handler(usbd_core_type *udev)
{
  usbd_type *usbx = udev->usb_reg;
  uint32_t sts_val = usbx->intsts;
  uint32_t sts_ien = usbx->ctrl;

  if(sts_val & USB_TC_FLAG)
  {
    /* endpoint tc interrupt handler */
    usbd_ept_loop_handler(udev);
  }

  if(sts_val & USB_RST_FLAG)
  {
    /* clear reset flag */
    usb_flag_clear(usbx, USB_RST_FLAG);

    /* reset interrupt handler */
    usbd_reset_handler(udev);
  }

  if((sts_val & USB_SOF_FLAG) &&
    (sts_ien & USB_SOF_INT))
  {
    /* clear sof flag */
    usb_flag_clear(usbx, USB_SOF_FLAG);

    /* sof interrupt handler */
    usbd_sof_handler(udev);
  }

  if((sts_val & USB_LSOF_FLAG) &&
    (sts_ien & USB_LSOF_INT))
  {
    /* clear lsof flag */
    usb_flag_clear(usbx, USB_LSOF_FLAG);
  }

  if((sts_val & USB_SP_FLAG) &&
    (sts_ien & USB_SP_INT))
  {
    /* clear suspend flag */
    usb_flag_clear(usbx, USB_SP_FLAG);

    /* usb suspend interrupt handler */
    usbd_suspend_handler(udev);
  }

  if((sts_val & USB_WK_FLAG) &&
    (sts_ien & USB_WK_INT))
  {
    /* usb wakeup interrupt handler */
    usbd_wakeup_handler(udev);

    /* clear wakeup flag */
    usb_flag_clear(usbx, USB_WK_FLAG);
  }
}

/**
  * @brief  usb device endpoint request handler.
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval none
  */
void usbd_eptn_handler(usbd_core_type *udev, usb_ept_number_type ept_num)
{
  usbd_type *usbx = udev->usb_reg;
  usb_ept_info *ept_info;
  uint32_t ept_val = usbx->ept[ept_num];
  uint16_t length;

  /* in interrupt request  */
  if(ept_val & USB_TXTC)
  {
    /* get endpoint register and in transfer info struct */
    ept_info = &udev->ept_in[ept_num];

    /* clear endpoint tc flag */
    USB_CLEAR_TXTC(ept_num);

    /* get endpoint tx length */
    ept_info->trans_len = USB_GET_TX_LEN(ept_num);

    /* offset the trans buffer */
    ept_info->trans_buf += ept_info->trans_len;

    if(ept_info->total_len == 0 || ept_num == USB_EPT0)
    {
      /* in transfer complete */
      usbd_core_in_handler(udev, ept_num);
    }
    else
    {
      /* endpoint continue send data */
      usbd_ept_send(udev, ept_num, ept_info->trans_buf, ept_info->total_len);
    }
    /* set the host assignment address */
    if(udev->conn_state == USB_CONN_STATE_ADDRESSED && udev->device_addr > 0)
    {
      usb_set_address(udev->usb_reg, udev->device_addr);
      udev->device_addr = 0;
    }
  }
  else
  {
    /* setup and out interrupt request */

    /* get endpoint register and out transfer info struct */
    ept_info = &udev->ept_out[ept_num];

    if((ept_val & USB_SETUPTC) != 0)
    {
      /* endpoint setup interrupt request */

      /* get endpoint received data length */
      ept_info->trans_len = USB_GET_RX_LEN(ept_num);

      /* read endpoint received data */
      usb_read_packet(udev->setup_buffer, ept_info->rx_addr, ept_info->trans_len);

      /* clear endpoint rx tc flag */
      USB_CLEAR_RXTC(USB_EPT0);

      /* endpoint setup complete handler */
      usbd_core_setup_handler(udev, ept_num);
    }
    else if(ept_val & USB_RXTC )
    {
      /* endpoint out interrupt request */
      USB_CLEAR_RXTC(ept_num);

      if(ept_info->is_double_buffer == 0)
      {
        /* get endpoint received data length */
        length = USB_GET_RX_LEN(ept_num);

        /* read endpoint received data */
        usb_read_packet(ept_info->trans_buf, ept_info->rx_addr, length);
      }
      else
      {
        if( ept_val & USB_RXDTS)
        {
          length = USB_DBUF0_GET_LEN(ept_num);
          usb_read_packet(ept_info->trans_buf, ept_info->tx_addr, length);
        }
        else
        {
          length = USB_DBUF1_GET_LEN(ept_num);
          usb_read_packet(ept_info->trans_buf, ept_info->rx_addr, length);
        }
        USB_FREE_DB_USER_BUFFER(ept_num, DATA_TRANS_OUT);
      }

      /* set received data length */
      ept_info->trans_len += length;
      ept_info->trans_buf += length;

      if(ept_info->total_len == 0 || length < ept_info->maxpacket || ept_num == USB_EPT0)
      {
        /* out transfer complete */
        usbd_core_out_handler(udev, ept_num);
      }
      else
      {
        /* endpoint continue receive data  */
        usbd_ept_recv(udev, ept_num, ept_info->trans_buf, ept_info->total_len);
      }
    }
  }
}

/**
  * @brief  usb device endpoint loop handler.
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_ept_loop_handler(usbd_core_type *udev)
{
  usbd_type *usbx = udev->usb_reg;
  usb_ept_number_type ept_num = USB_EPT0;
  uint32_t sts_val;

  while((sts_val = usbx->intsts) & USB_TC_FLAG)
  {
    /* get the interrupt endpoint number */
    ept_num = (usb_ept_number_type)(sts_val & USB_EPT_NUM_FLAG);

    usbd_eptn_handler(udev, ept_num);
  }
}

/**
  * @brief  usb device reset interrupt request handler.
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_reset_handler(usbd_core_type *udev)
{
  /* free usb buffer */
  usb_buffer_free();

  usbd_ept_defaut_init(udev);
#ifndef USB_EPT_AUTO_MALLOC_BUFFER
  usbd_ept_buf_custom_define(udev, 0x80, EPT0_TX_ADDR);
  usbd_ept_buf_custom_define(udev, 0x00, EPT0_RX_ADDR);
#endif

  /* open endpoint 0 out */
  usbd_ept_open(udev, 0x00, EPT_CONTROL_TYPE, 0x40);

  /* open endpoint 0 in */
  usbd_ept_open(udev, 0x80, EPT_CONTROL_TYPE, 0x40);

  /* set device address to 0 */
  usb_set_address(udev->usb_reg, 0);

  /* usb connect state set to default */
  udev->conn_state = USB_CONN_STATE_DEFAULT;

  /* user define reset event */
  if(udev->class_handler->event_handler)
    udev->class_handler->event_handler(udev, USBD_RESET_EVENT);
}

/**
  * @brief  usb device sof interrupt request handler.
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_sof_handler(usbd_core_type *udev)
{
  /* user sof handler in class define*/
  if(udev->class_handler->sof_handler)
    udev->class_handler->sof_handler(udev);
}

/**
  * @brief  usb device suspend interrupt request handler.
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_suspend_handler(usbd_core_type *udev)
{
  /* save connect state */
  udev->old_conn_state = udev->conn_state;

  /* set current state to suspend */
  udev->conn_state = USB_CONN_STATE_SUSPENDED;

  /* enter suspend mode */
  usbd_enter_suspend(udev);

  /* user define suspend event */
  if(udev->class_handler->event_handler)
    udev->class_handler->event_handler(udev, USBD_SUSPEND_EVENT);
}

/**
  * @brief  usb device wakup interrupt request handler.
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_wakeup_handler(usbd_core_type *udev)
{
  /* exit suspend mode */
  usb_exit_suspend(udev->usb_reg);

  /* restore connect state */
  udev->conn_state = udev->old_conn_state;

  /* user define wakeup event */
  if(udev->class_handler->event_handler)
    udev->class_handler->event_handler(udev, USBD_WAKEUP_EVENT);
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
