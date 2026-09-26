/**
  **************************************************************************
  * @file     at32f403a_407_usb.c
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    contains the functions for the usb firmware library
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

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @defgroup USB
  * @brief USB driver modules
  * @{
  */

#include "at32f403a_407_conf.h"


#ifdef USB_MODULE_ENABLED

/** @defgroup USB_private_functions
  * @{
  */

/**
  * @brief usb packet buffer start address
  */
#define USB_ENDP_DESC_TABLE_OFFSET       0x40
uint32_t g_usb_packet_address = USB_PACKET_BUFFER_ADDRESS;
static uint16_t g_usb_offset_addr = USB_ENDP_DESC_TABLE_OFFSET;

/**
  * @brief  initialize usb peripheral controller register
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @retval none
  */
void usb_dev_init(usbd_type *usbx)
{
  /* clear usb core reset */
  usbx->ctrl_bit.csrst = 0;

  /* clear usb interrupt status */
  usbx->intsts = 0;

  /* set usb packet buffer descirption table address */
  usbx->buftbl = USB_BUFFER_TABLE_ADDRESS;

  /* enable usb core and set device address to 0 */
  usbx->devaddr = 0x80;

  usb_interrupt_enable(usbx, USB_SOF_INT | USB_RST_INT | USB_SP_INT | USB_WK_INT | USB_TC_INT, TRUE);
}

/**
  * @brief  connect usb device
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @retval none
  */
void usb_connect(usbd_type *usbx)
{
  /* enable usb phy */
  usbx->ctrl_bit.disusb = 0;

  /* Dp 1.5k pull-up enable */
  usbx->cfg_bit.puo = 0;
}

/**
  * @brief  disconnect usb device
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @retval none
  */
void usb_disconnect(usbd_type *usbx)
{
  /* disable usb phy */
  usbx->ctrl_bit.disusb = TRUE;

  /* D+ 1.5k pull-up disable */
  usbx->cfg_bit.puo = TRUE;
}


/**
  * @brief  mapping usb packet buffer area
  *         two mapping intervals are available for packet buffer area,
  *         and are select by the usbbufs in the crm misc1 register.
  *         when usbbufs is 0,sram size is 512 bytes, packet buffer start
  *         address is 0x40006000.when usbbufs is 1, sram size is fixed to
  *         768~1280 bytes, and the  packet buffer start address is fixed to
  *         0x40007800,packet buffer size decided by whether can1 and can2 are
  *         enabled;when both can1 and can2 are disabled, usb packet buffer can be set to the
  *         maximum of 1280 bytes; when either can1 or can2 is enabled, usb packet buffer can be set to the
  *         maximum of 1024 bytes; when both CAN1 and CAN2 are enabled, usb packet buffer can be set to the
  *         maximum of 768 bytes.
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @retval none
  */
void usb_usbbufs_enable(usbd_type *usbx, confirm_state state)
{
  if(state == TRUE)
  {
    /* enable usbbufs */
    g_usb_packet_address = USB_PACKET_BUFFER_ADDRESS_EX;
    CRM->misc1_bit.usbbufs = TRUE;
  }
  else
  {
    /* disable usbbufs */
    g_usb_packet_address = USB_PACKET_BUFFER_ADDRESS;
    CRM->misc1_bit.usbbufs = FALSE;
  }
}

/**
  * @brief  open usb endpoint
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @param  ept_info: endpoint information structure
  * @retval none
  */
void usb_ept_open(usbd_type *usbx, usb_ept_info *ept_info)
{
  uint16_t type = 0;

  /* set endpoint address */
  USB_SET_EPT_ADDRESS(ept_info->eptn, ept_info->ept_address);

  /* select endpoint transfer type */
  if(ept_info->trans_type == EPT_CONTROL_TYPE)
  {
    type = USB_EPT_CONTROL;
  }
  else if(ept_info->trans_type == EPT_BULK_TYPE)
  {
    type = USB_EPT_BULK;
  }
  else if(ept_info->trans_type == EPT_INT_TYPE)
  {
    type = USB_EPT_INT;
  }
  else if(ept_info->trans_type == EPT_ISO_TYPE)
  {
    type = USB_EPT_ISO;
    ept_info->is_double_buffer = TRUE;
  }

  /* configure endpoint transfer type (control, bulk, interrupt, isochronous) */
  USB_SET_TRANS_TYPE(ept_info->eptn, type);

  /* endpoint is in transfer */
  if(ept_info->inout == DATA_TRANS_IN)
  {
      if(ept_info->is_double_buffer == 0)
      {
        /* set in endpoint tx offset address */
        USB_SET_TX_ADDRESS(ept_info->eptn, ept_info->tx_addr);

        /* clear in endpoint data toggle */
        USB_CLEAR_TXDTS(ept_info->eptn);

        /* set endpoint transmission status: nak */
        USB_SET_TXSTS(ept_info->eptn, USB_TX_NAK);
      }
      else
      {
        /* set double buffer endpoint*/
        USB_SET_EPT_DOUBLE_BUFFER(ept_info->eptn);

        /* set in endpoint offset address0 and address1 */
        USB_SET_DOUBLE_BUFF0_ADDRESS(ept_info->eptn, ept_info->tx_addr);
        USB_SET_DOUBLE_BUFF1_ADDRESS(ept_info->eptn, ept_info->rx_addr);

        /* clear in and out data toggle */
        USB_CLEAR_TXDTS(ept_info->eptn);
        USB_CLEAR_RXDTS(ept_info->eptn);

        /* toggle rx data toggle flag */
        USB_TOGGLE_RXDTS(ept_info->eptn);

        /* set endpoint reception status: disable */
        USB_SET_RXSTS(ept_info->eptn, USB_RX_DISABLE);

        /* set endpoint transmision status: nak */
        USB_SET_TXSTS(ept_info->eptn, USB_TX_NAK);
      }
  }
  else
  {
    if(ept_info->is_double_buffer == 0)
    {
      /* set out endpoint rx offset address */
      USB_SET_RX_ADDRESS(ept_info->eptn, ept_info->rx_addr);

      /* clear out endpoint data toggle */
      USB_CLEAR_RXDTS(ept_info->eptn);

      /* set out endpoint max reception buffer size */
      USB_SET_RXLEN(ept_info->eptn, ept_info->maxpacket);

      /* set endpoint reception status: valid */
      USB_SET_RXSTS(ept_info->eptn, USB_RX_VALID);
    }
    else
    {
      /* set double buffer endpoint */
      USB_SET_EPT_DOUBLE_BUFFER(ept_info->eptn);

      /* set out endpoint offset address0 and address1 */
      USB_SET_DOUBLE_BUFF0_ADDRESS(ept_info->eptn, ept_info->tx_addr);
      USB_SET_DOUBLE_BUFF1_ADDRESS(ept_info->eptn, ept_info->rx_addr);

      /* set out endpoint max reception buffer size */
      USB_SET_EPT_DOUBLE_BUF0_LEN(ept_info->eptn, ept_info->maxpacket, DATA_TRANS_OUT);
      USB_SET_EPT_DOUBLE_BUF1_LEN(ept_info->eptn, ept_info->maxpacket, DATA_TRANS_OUT);

      /* clear in and out data toggle */
      USB_CLEAR_TXDTS(ept_info->eptn);
      USB_CLEAR_RXDTS(ept_info->eptn);

      /* toggle tx data toggle flag */
      USB_TOGGLE_TXDTS(ept_info->eptn);

      /* set endpoint reception status: valid */
      USB_SET_RXSTS(ept_info->eptn, USB_RX_VALID);

      /* set endpoint transmision status: disable */
      USB_SET_TXSTS(ept_info->eptn, USB_TX_DISABLE);
    }
  }
}


/**
  * @brief  close usb endpoint
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @param  ept_info: endpoint information structure
  * @retval none
  */
void usb_ept_close(usbd_type *usbx, usb_ept_info *ept_info)
{
  if(ept_info->is_double_buffer == 0)
  {
    if(ept_info->inout == DATA_TRANS_IN)
    {
      /*clear tx data toggle */
      USB_CLEAR_TXDTS(ept_info->eptn);

      /* set tx status: disable */
      USB_SET_TXSTS(ept_info->eptn, USB_TX_DISABLE);
    }
    else
    {
      /*clear rx data toggle */
      USB_CLEAR_RXDTS(ept_info->eptn);

      /* set rx status: disable */
      USB_SET_RXSTS(ept_info->eptn, USB_RX_DISABLE);

    }
  }
  else
  {
    /* double buffer */

    /*clear rx and tx data toggle */
    USB_CLEAR_TXDTS(ept_info->eptn);
    USB_CLEAR_RXDTS(ept_info->eptn);

    if(ept_info->inout == DATA_TRANS_IN)
    {
      /* toggle tx */
      USB_TOGGLE_TXDTS(ept_info->eptn);

      /* set tx and rx status: disable */
      USB_SET_TXSTS(ept_info->eptn, USB_TX_DISABLE);
      USB_SET_RXSTS(ept_info->eptn, USB_RX_DISABLE);
    }
    else
    {
      /* toggle rx */
      USB_TOGGLE_RXDTS(ept_info->eptn);

      /* set tx and rx status: disable */
      USB_SET_TXSTS(ept_info->eptn, USB_TX_DISABLE);
      USB_SET_RXSTS(ept_info->eptn, USB_RX_DISABLE);
    }
  }
}

/**
  * @brief  write data from user memory to usb buffer
  * @param  pusr_buf: point to user buffer
  * @param  offset_addr: endpoint tx offset address
  * @param  nbytes: number of bytes data write to usb buffer
  * @retval none
  */
void usb_write_packet(uint8_t *pusr_buf, uint16_t offset_addr, uint16_t nbytes)
{
  /* endpoint tx buffer address */
  __IO uint16_t *d_addr = (__IO uint16_t *)(offset_addr * 2 + g_usb_packet_address);

  uint32_t nhbytes = (nbytes + 1) >> 1;
  uint32_t n_index;
  uint16_t *pbuf = (uint16_t *)pusr_buf;
  for(n_index = 0; n_index < nhbytes; n_index ++)
  {
#if defined (__ICCARM__) && (__VER__ < 7000000)
    *d_addr++  = *(__packed uint16_t *)pbuf;
#else
    *d_addr++ = __UNALIGNED_UINT16_READ(pbuf);
#endif
    d_addr ++;
    pbuf ++;
  }
}

/**
  * @brief  read data from usb buffer to user buffer
  * @param  pusr_buf: point to user buffer
  * @param  offset_addr: endpoint rx offset address
  * @param  nbytes: number of bytes data write to usb buffer
  * @retval none
  */
void usb_read_packet(uint8_t *pusr_buf, uint16_t offset_addr, uint16_t nbytes)
{
  __IO uint16_t *s_addr = (__IO uint16_t *)(offset_addr * 2 + g_usb_packet_address);
  uint32_t nhbytes = (nbytes + 1) >> 1;
  uint32_t n_index;
  uint16_t *pbuf = (uint16_t *)pusr_buf;
  for(n_index = 0; n_index < nhbytes; n_index ++)
  {
#if defined (__ICCARM__) && (__VER__ < 7000000)
    *(__packed uint16_t *)pbuf = *(__IO uint16_t *)s_addr ++;
#else
    __UNALIGNED_UINT16_WRITE(pbuf, *(__IO uint16_t *)s_addr ++);
#endif
    s_addr ++;
    pbuf ++;
  }
}


/**
  * @brief  usb interrupt enable
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @param  interrupt:
  *         this parameter can be any combination of the following values:
  *         - USB_LSOF_INT
  *         - USB_SOF_INT
  *         - USB_RST_INT
  *         - USB_SP_INT
  *         - USB_WK_INT
  *         - USB_BE_INT
  *         - USB_UCFOR_INT
  *         - USB_TC_INT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void usb_interrupt_enable(usbd_type *usbx, uint16_t interrupt, confirm_state new_state)
{
  if(new_state == TRUE)
  {
    usbx->ctrl |= interrupt;
  }
  else
  {
    usbx->ctrl &= ~interrupt;
  }
}

/**
  * @brief  set the host assignment address
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @param  address: host assignment address
  * @retval none
  */
void usb_set_address(usbd_type *usbx, uint8_t address)
{
  usbx->devaddr_bit.addr = address;
  usbx->devaddr_bit.cen = TRUE;
}

/**
  * @brief  set endpoint tx or rx status to stall
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @param  ept_info: endpoint information structure
  * @retval none
  */
void usb_ept_stall(usbd_type *usbx, usb_ept_info *ept_info)
{
  if(ept_info->inout == DATA_TRANS_IN)
  {
    USB_SET_TXSTS(ept_info->eptn, USB_TX_STALL)
  }
  else
  {
    USB_SET_RXSTS(ept_info->eptn, USB_RX_STALL)
  }
}

/**
  * @brief  usb device enter suspend mode
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @retval none
  */
void usb_enter_suspend(usbd_type *usbx)
{
  usbx->ctrl_bit.ssp = TRUE;
  usbx->ctrl_bit.lpm = TRUE;
}

/**
  * @brief  usb device exit suspend mode
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @retval none
  */
void usb_exit_suspend(usbd_type *usbx)
{
  usbx->ctrl_bit.ssp = FALSE;
  usbx->ctrl_bit.lpm = FALSE;
}

/**
  * @brief  usb remote wakeup set
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @retval none
  */
void usb_remote_wkup_set(usbd_type *usbx)
{
   usbx->ctrl_bit.gresume = TRUE;
}

/**
  * @brief  usb remote wakeup clear
  * @param  usbx: to select the usb peripheral.
  *         parameter as following values: USB
  * @retval none
  */
void usb_remote_wkup_clear(usbd_type *usbx)
{
  usbx->ctrl_bit.gresume = FALSE;
}

/**
  * @brief  usb auto malloc endpoint buffer
  * @param  mapacket: endpoint support max packet size
  * @retval none
  */
uint16_t usb_buffer_malloc(uint16_t maxpacket)
{
  uint16_t offset = g_usb_offset_addr;
  g_usb_offset_addr += maxpacket;
  return offset;
}

/**
  * @brief  free usb endpoint buffer
  * @param  none
  * @retval none
  */
void usb_buffer_free(void)
{
  g_usb_offset_addr = USB_ENDP_DESC_TABLE_OFFSET;
}

/**
  * @brief  get flag of usb.
  * @param  usbx: select the usb peripheral
  * @param  flag: select the usb flag
  *         this parameter can be one of the following values:
  *         - USB_INOUT_FLAG
  *         - USB_LSOF_FLAG
  *         - USB_SOF_FLAG
  *         - USB_RST_FLAG
  *         - USB_SP_FLAG
  *         - USB_WK_FLAG
  *         - USB_BE_FLAG
  *         - USB_UCFOR_FLAG
  *         - USB_TC_FLAG
  * @retval none
  */
flag_status usb_flag_get(usbd_type *usbx, uint16_t flag)
{
  flag_status status = RESET;

  if((usbx->intsts & flag) == RESET)
  {
    status = RESET;
  }
  else
  {
    status = SET;
  }
  return status;
}

/**
  * @brief  clear flag of usb.
  * @param  usbx: select the usb peripheral
  * @param  flag: select the usb flag
  *         this parameter can be one of the following values:
  *         - USB_INOUT_FLAG
  *         - USB_LSOF_FLAG
  *         - USB_SOF_FLAG
  *         - USB_RST_FLAG
  *         - USB_SP_FLAG
  *         - USB_WK_FLAG
  *         - USB_BE_FLAG
  *         - USB_UCFOR_FLAG
  *         - USB_TC_FLAG
  * @retval none
  */
void usb_flag_clear(usbd_type *usbx, uint16_t flag)
{
  usbx->intsts = ~flag;
}

/**
  * @}
  */

#endif

/**
  * @}
  */

/**
  * @}
  */
