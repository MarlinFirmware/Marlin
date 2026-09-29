/**
  **************************************************************************
  * @file     usbd_core.c
  * @brief    usb driver
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
#include "usbd_sdr.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_drivers
  * @{
  */

/** @defgroup USBD_drivers_core
  * @brief usb device drivers core
  * @{
  */

/** @defgroup USBD_core_private_functions
  * @{
  */

/**
  * @brief  usb core in transfer complete handler
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @retval none
  */
void usbd_core_in_handler(usbd_core_type *udev, uint8_t ept_addr)
{
  /* get endpoint info*/
  usb_ept_info *ept_info = &udev->ept_in[ept_addr & 0x7F];

  if(ept_addr == 0)
  {
    if(udev->ept0_sts == USB_EPT0_DATA_IN)
    {
      if(ept_info->rem0_len > ept_info->maxpacket)
      {
        ept_info->rem0_len -= ept_info->maxpacket;
        usbd_ept_send(udev, 0, ept_info->trans_buf,
                      MIN(ept_info->rem0_len, ept_info->maxpacket));
      }
      /* endpoint 0 */
      else if(ept_info->last_len == ept_info->maxpacket
        && ept_info->ept0_slen >= ept_info->maxpacket
        && ept_info->ept0_slen < udev->ept0_wlength)
      {
        ept_info->last_len = 0;
        usbd_ept_send(udev, 0, 0, 0);
        usbd_ept_recv(udev, ept_addr, 0, 0);
      }
      else
      {

        if(udev->class_handler->ept0_tx_handler != 0 &&
            udev->conn_state == USB_CONN_STATE_CONFIGURED)
        {
          udev->class_handler->ept0_tx_handler(udev);
        }
        usbd_ctrl_recv_status(udev);

      }
    }
  }
  else if(udev->class_handler->in_handler != 0 &&
          udev->conn_state == USB_CONN_STATE_CONFIGURED)
  {
    /* other user define endpoint */
    udev->class_handler->in_handler(udev, ept_addr);
  }
}

/**
  * @brief  usb core out transfer complete handler
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @retval none
  */
void usbd_core_out_handler(usbd_core_type *udev, uint8_t ept_addr)
{
   /* get endpoint info*/
  usb_ept_info *ept_info = &udev->ept_out[ept_addr & 0x7F];

  if(ept_addr == 0)
  {
    /* endpoint 0 */
    if(udev->ept0_sts == USB_EPT0_DATA_OUT)
    {
      if(ept_info->rem0_len > ept_info->maxpacket)
      {
        ept_info->rem0_len -= ept_info->maxpacket;
        usbd_ept_recv(udev, ept_addr, ept_info->trans_buf,
                      MIN(ept_info->rem0_len, ept_info->maxpacket));
      }
      else
      {
          if(udev->class_handler->ept0_rx_handler != 0)
          {
            udev->class_handler->ept0_rx_handler(udev);
          }
          usbd_ctrl_send_status(udev);
      }
    }
  }
  else if(udev->class_handler->out_handler != 0 &&
          udev->conn_state == USB_CONN_STATE_CONFIGURED)
  {
    /* other user define endpoint */
    udev->class_handler->out_handler(udev, ept_addr);
  }
}

/**
  * @brief  usb core setup transfer complete handler
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @retval none
  */
void usbd_core_setup_handler(usbd_core_type *udev, uint8_t ept_num)
{
  /* setup parse */
  usbd_setup_request_parse(&udev->setup, udev->setup_buffer);

  /* set ept0 status */
  udev->ept0_sts = USB_EPT0_SETUP;
  udev->ept0_wlength = udev->setup.wLength;

  switch(udev->setup.bmRequestType & USB_REQ_RECIPIENT_MASK)
  {
    case USB_REQ_RECIPIENT_DEVICE:
      /* recipient device request */
      usbd_device_request(udev);
      break;
    case USB_REQ_RECIPIENT_INTERFACE:
      /* recipient interface request */
      usbd_interface_request(udev);
      break;
    case USB_REQ_RECIPIENT_ENDPOINT:
      /* recipient endpoint request */
      usbd_endpoint_request(udev);
      break;
    default:
      break;
  }
}

/**
  * @brief  usb control endpoint send data
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @param  buffer: send data buffer
  * @param  len: send data length
  * @retval none
  */
void usbd_ctrl_send(usbd_core_type *udev, uint8_t *buffer, uint16_t len)
{
  usb_ept_info *ept_info = &udev->ept_in[0];

  ept_info->ept0_slen = len;
  ept_info->rem0_len = len;
  udev->ept0_sts = USB_EPT0_DATA_IN;

  usbd_ept_send(udev, 0, buffer, len);
}

/**
  * @brief  usb control endpoint receive data
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @param  buffer: recv data buffer
  * @param  len: recv data length
  * @retval none
  */
void usbd_ctrl_recv(usbd_core_type *udev, uint8_t *buffer, uint16_t len)
{
  usb_ept_info *ept_info = &udev->ept_out[0];

  ept_info->ept0_slen = len;
  ept_info->rem0_len = len;
  udev->ept0_sts = USB_EPT0_DATA_OUT;

  usbd_ept_recv(udev, 0, buffer, len);
}

/**
  * @brief  usb control endpoint send in status
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_ctrl_send_status(usbd_core_type *udev)
{
  udev->ept0_sts = USB_EPT0_STATUS_IN;

  usbd_ept_send(udev, 0, 0, 0);
}

/**
  * @brief  usb control endpoint send out status
  * @param  udev: usb device core handler type
  * @retval none
  */
void usbd_ctrl_recv_status(usbd_core_type *udev)
{
  udev->ept0_sts = USB_EPT0_STATUS_OUT;

  usbd_ept_recv(udev, 0, 0, 0);
}

/**
  * @brief  clear endpoint stall
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @retval none
  */
void usbd_clear_stall(usbd_core_type *udev, uint8_t ept_addr)
{
  usb_ept_info *ept_info;

  if(ept_addr & 0x80)
  {
    /* in endpoint */
    ept_info = &udev->ept_in[ept_addr & 0x7F];
    USB_CLEAR_TXDTS(ept_info->eptn);
    if(USB->ept_bit[ept_info->eptn].txsts == USB_TX_STALL)
      USB_SET_TXSTS(ept_info->eptn, USB_TX_NAK);
  }
  else
  {
    /* out endpoint */
    ept_info = &udev->ept_out[ept_addr & 0x7F];
    USB_CLEAR_RXDTS(ept_info->eptn);
    if(USB->ept_bit[ept_info->eptn].rxsts == USB_RX_STALL)
      USB_SET_RXSTS(ept_info->eptn, USB_RX_VALID);
  }
  ept_info->stall = 0;
}

/**
  * @brief  usb set endpoint to stall.
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @retval none
  */
void usbd_set_stall(usbd_core_type *udev, uint8_t ept_addr)
{
  usb_ept_info *ept_info;

  if(ept_addr & 0x80)
  {
    /* in endpoint */
    ept_info = &udev->ept_in[ept_addr & 0x7F];
    USB_SET_TXSTS(ept_info->eptn, USB_TX_STALL);
  }
  else
  {
    /* out endpoint */
    ept_info = &udev->ept_out[ept_addr & 0x7F];
    USB_SET_RXSTS(ept_info->eptn, USB_RX_STALL)
  }

  ept_info->stall = 1;
}

/**
  * @brief  un-support device request
  * @param  udev: usb device core handler type
  * @retval none
  */
void usbd_ctrl_unsupport(usbd_core_type *udev)
{
  /* return stall status */
  usbd_set_stall(udev, 0x00);
  usbd_set_stall(udev, 0x80);
}

/**
  * @brief  usb endpoint send data
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @param  buffer: send data buffer
  * @param  len: send data length
  * @retval none
  */
void usbd_ept_send(usbd_core_type *udev, uint8_t ept_addr, uint8_t *buffer, uint16_t len)
{
  /* get endpoint info struct and register */
  usb_ept_info *ept_info = &udev->ept_in[ept_addr & 0x7F];
  uint16_t trs_len = 0;
  usbd_type *usbx = udev->usb_reg;

  /* set send data buffer and length */
  ept_info->trans_buf = buffer;
  ept_info->total_len = len;
  ept_info->trans_len = 0;

  if(ept_info->total_len > ept_info->maxpacket)
  {
    trs_len = ept_info->maxpacket;
    ept_info->total_len -= trs_len;
  }
  else
  {
    trs_len = len;
    ept_info->total_len = 0;
  }

  ept_info->last_len = trs_len;

  if(ept_info->is_double_buffer == 0)
  {
    /* write data to endpoint buffer */
    usb_write_packet(ept_info->trans_buf, ept_info->tx_addr, trs_len);

    /* set send data length */
    USB_SET_TXLEN((ept_addr & 0x7F), trs_len);
  }
  else
  {
    if(usbx->ept_bit[ept_addr & 0x7F].txdts)
    {
      USB_SET_EPT_DOUBLE_BUF1_LEN((ept_addr & 0x7F), trs_len, DATA_TRANS_IN);
      usb_write_packet(ept_info->trans_buf, ept_info->rx_addr, trs_len);
    }
    else
    {
      USB_SET_EPT_DOUBLE_BUF0_LEN((ept_addr & 0x7F), trs_len, DATA_TRANS_IN);
      usb_write_packet(ept_info->trans_buf, ept_info->tx_addr, trs_len);
    }
    USB_FREE_DB_USER_BUFFER((ept_addr & 0x7F), DATA_TRANS_IN);
  }

  /* set tx status valid */
  USB_SET_TXSTS((ept_addr & 0x7F), USB_TX_VALID);
}

/**
  * @brief  usb endpoint receive data
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @param  buffer: receive data buffer
  * @param  len: receive data length
  * @retval none
  */
void usbd_ept_recv(usbd_core_type *udev, uint8_t ept_addr, uint8_t *buffer, uint16_t len)
{
  /* get endpoint info struct and register */
  usb_ept_info *ept_info = &udev->ept_out[ept_addr & 0x7F];
  uint32_t trs_len = 0;

   /* set receive data buffer and length */
  ept_info->trans_buf = buffer;
  ept_info->total_len = len;
  ept_info->trans_len = 0;

  if(ept_info->total_len > ept_info->maxpacket)
  {
    trs_len = ept_info->maxpacket;
    ept_info->total_len -= trs_len;
  }
  else
  {
    trs_len = len;
    ept_info->total_len = 0;
  }

  /* set rx status valid */
  USB_SET_RXSTS((ept_addr & 0x7F), USB_RX_VALID);
}


/**
  * @brief  usb endpoint get receive data length
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @retval none
  */
uint32_t usbd_get_recv_len(usbd_core_type *udev, uint8_t ept_addr)
{
  usb_ept_info *ept = &udev->ept_out[ept_addr & 0x7F];
  return ept->trans_len;
}

/**
  * @brief  enable endpoint double buffer.
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @retval none
  */
void usbd_ept_dbuffer_enable(usbd_core_type *udev, uint8_t ept_addr)
{
  usb_ept_info *ept_info;
  if((ept_addr & 0x80) == 0)
  {
    /* out endpoint info */
    ept_info = &udev->ept_out[ept_addr & 0x7F];
  }
  else
  {
    /* in endpoint info */
    ept_info = &udev->ept_in[ept_addr & 0x7F];
  }
  ept_info->is_double_buffer = TRUE;
}

/**
  * @brief  usb auto define endpoint buffer
  * @param  usb_ept_info: endpoint information
  * @retval none
  */
void usbd_ept_buf_auto_define(usb_ept_info *ept_info)
{
  if(ept_info->is_double_buffer == 0)
  {
    if( ept_info->inout == DATA_TRANS_IN )
    {
      if(ept_info->tx_addr == 0)
        ept_info->tx_addr = usb_buffer_malloc(ept_info->maxpacket);
    }
    else
    {
      if(ept_info->rx_addr == 0)
        ept_info->rx_addr = usb_buffer_malloc(ept_info->maxpacket);
    }
  }
  else
  {
    /* double buffer auto define */
    if(ept_info->tx_addr == 0)
      ept_info->tx_addr = usb_buffer_malloc(ept_info->maxpacket);
    if(ept_info->rx_addr == 0)
      ept_info->rx_addr = usb_buffer_malloc(ept_info->maxpacket);
  }
}



/**
  * @brief  usb custom define endpoint buffer
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @param  addr: usb fifo offset address
  * @retval none
  */
void usbd_ept_buf_custom_define(usbd_core_type *udev, uint8_t ept_addr,
                                uint32_t addr)
{
  usb_ept_info *ept_info;
  if((ept_addr & 0x80) == 0)
  {
    /* out endpoint info */
    ept_info = &udev->ept_out[ept_addr & 0x7F];
  }
  else
  {
    /* in endpoint info */
    ept_info = &udev->ept_in[ept_addr & 0x7F];
  }

  if(ept_info->is_double_buffer == 0)
  {
    if( ept_info->inout == DATA_TRANS_IN )
    {
      ept_info->tx_addr = addr;
    }
    else
    {
      ept_info->rx_addr = addr;
    }
  }
  else
  {
    /* double buffer malloc */
    ept_info->tx_addr = addr & 0xFFFF;
    ept_info->rx_addr = (addr >> 16) & 0xFFFF;
  }
}

/**
  * @brief  usb open endpoint
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @param  ept_type: endpoint type
  * @param  maxpacket: endpoint support max buffer size
  * @retval none
  */
void usbd_ept_open(usbd_core_type *udev, uint8_t ept_addr, uint8_t ept_type, uint16_t maxpacket)
{
  usbd_type *usbx = udev->usb_reg;
  usb_ept_info *ept_info;

  if((ept_addr & 0x80) == 0)
  {
    /* out endpoint info */
    ept_info = &udev->ept_out[ept_addr & 0x7F];
    ept_info->inout = DATA_TRANS_OUT;
  }
  else
  {
    /* in endpoint info */
    ept_info = &udev->ept_in[ept_addr & 0x7F];
    ept_info->inout = DATA_TRANS_IN;
  }

  /* set endpoint maxpacket and type */
  ept_info->maxpacket = (maxpacket + 1) & 0xFFFE;
  ept_info->trans_type = ept_type;

#ifdef USB_EPT_AUTO_MALLOC_BUFFER
  usbd_ept_buf_auto_define(ept_info);
#endif
  /* open endpoint */
  usb_ept_open(usbx, ept_info);
}

/**
  * @brief  usb close endpoint
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_addr: endpoint number
  * @retval none
  */
void usbd_ept_close(usbd_core_type *udev, uint8_t ept_addr)
{
  usb_ept_info *ept_info;
  if(ept_addr & 0x80)
  {
    /* in endpoint */
    ept_info = &udev->ept_in[ept_addr & 0x7F];
  }
  else
  {
    /* out endpoint */
    ept_info = &udev->ept_out[ept_addr & 0x7F];
  }

  /* close endpoint */
  usb_ept_close(udev->usb_reg, ept_info);
}

/**
  * @brief  usb device connect to host
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_connect(usbd_core_type *udev)
{
  usb_connect(udev->usb_reg);
}

/**
  * @brief  usb device disconnect to host
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_disconnect(usbd_core_type *udev)
{
  usb_disconnect(udev->usb_reg);
}

/**
  * @brief  usb device set device address.
  * @param  udev: to the structure of usbd_core_type
  * @param  address: host assignment address
  * @retval none
  */
void usbd_set_device_addr(usbd_core_type *udev, uint8_t address)
{
  usb_set_address(udev->usb_reg, address);
}

/**
  * @brief  get usb connect state
  * @param  udev: to the structure of usbd_core_type
  * @retval usb connect state
  */
usbd_conn_state usbd_connect_state_get(usbd_core_type *udev)
{
  return udev->conn_state;
}

/**
  * @brief  usb device remote wakeup
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_remote_wakeup(usbd_core_type *udev)
{
  /* set connect state */
  udev->conn_state = udev->old_conn_state;

  usb_exit_suspend(udev->usb_reg);

  usb_remote_wkup_set(udev->usb_reg);

  usb_delay_ms(10);

  usb_remote_wkup_clear(udev->usb_reg);

}

/**
  * @brief  usb device enter suspend mode
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_enter_suspend(usbd_core_type *udev)
{
  usb_enter_suspend(udev->usb_reg);
}

/**
  * @brief  usb endpoint structure initialization
  * @param  udev: to the structure of usbd_core_type
  * @retval none
  */
void usbd_ept_defaut_init(usbd_core_type *udev)
{
  uint8_t i_index = 0;
  /* init in endpoint info structure */
  for(i_index = 0; i_index < USB_EPT_MAX_NUM; i_index ++)
  {
    udev->ept_in[i_index].eptn        = i_index;
    udev->ept_in[i_index].ept_address = i_index;
    udev->ept_in[i_index].inout         = DATA_TRANS_IN;
    udev->ept_in[i_index].maxpacket   = 0;
    udev->ept_in[i_index].trans_buf   = 0;
    udev->ept_in[i_index].total_len   = 0;
    udev->ept_in[i_index].tx_addr     = 0;
    udev->ept_in[i_index].rx_addr     = 0;
  }

  /* init out endpoint info structure */
  for(i_index = 0; i_index < USB_EPT_MAX_NUM; i_index ++)
  {
    udev->ept_out[i_index].eptn        = i_index;
    udev->ept_out[i_index].ept_address = i_index;
    udev->ept_out[i_index].inout         = DATA_TRANS_OUT;
    udev->ept_out[i_index].maxpacket   = 0;
    udev->ept_out[i_index].trans_buf   = 0;
    udev->ept_out[i_index].total_len   = 0;
    udev->ept_out[i_index].rx_addr     = 0;
    udev->ept_out[i_index].tx_addr     = 0;
  }
  return;
}

/**
  * @brief  initializes the usb core
  * @param  udev: to the structure of usbd_core_type
  * @param  usb_reg: usb register pointer (USB)
  * @param  class_handler: usb class handler
  * @param  desc_handler: device config handler
  * @param  core_id: usb core id number
  * @retval none
  */
void usbd_core_init(usbd_core_type *udev,
                    usb_reg_type *usb_reg,
                    usbd_class_handler *class_handler,
                    usbd_desc_handler *desc_handler,
                    uint8_t core_id)
{
  /* usb class handler */
  udev->class_handler = class_handler;

  /* usb description handler */
  udev->desc_handler = desc_handler;

  /* set usb register type */
  udev->usb_reg = usb_reg;

  /* set usb connect state to default */
  udev->conn_state = USB_CONN_STATE_DEFAULT;

  /* init in endpoint info structure */
  usbd_ept_defaut_init(udev);

#ifdef USB_BUFFER_SIZE_EX
  /* usb buffer size extend 768-1280 byte */
  usb_usbbufs_enable(usb_reg, TRUE);
#endif

  /*usb register config */
  usb_dev_init(udev->usb_reg);
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
