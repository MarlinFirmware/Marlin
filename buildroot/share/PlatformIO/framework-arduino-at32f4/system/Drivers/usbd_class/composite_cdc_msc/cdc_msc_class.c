/**
  **************************************************************************
  * @file     cdc_msc_class.c
  * @brief    usb cdc class type
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
#include "cdc_msc_class.h"
#include "cdc_msc_desc.h"
// #include "msc_bot_scsi.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */
  
/** @defgroup USB_cdc_msc_class
  * @brief usb device class cdc msc demo
  * @{
  */  

/** @defgroup USB_cdc_msc_class_private_functions
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

static usb_sts_type cdc_struct_init(cdc_msc_struct_type *pcdc);
extern void usb_usart_config( linecoding_type linecoding);
static void usb_vcp_cmd_process(void *udev, uint8_t cmd, uint8_t *buff, uint16_t len);

static usb_sts_type cdc_class_setup_handler(void *udev, usb_setup_type *setup);
static usb_sts_type msc_class_setup_handler(void *udev, usb_setup_type *setup);

linecoding_type linecoding = 
{
  115200,
  0,
  0,
  8
};

/* cdc data struct */
cdc_msc_struct_type cdc_msc_struct;

/* usb device class handler */
usbd_class_handler cdc_msc_class_handler = 
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
  &cdc_msc_struct   //cdc_struct
};
/**
  * @brief  initialize usb custom hid endpoint
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type                            
  */
static usb_sts_type class_init_handler(void *udev)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pcdcmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  
  /* init cdc struct */
  cdc_struct_init(pcdcmsc);
  
  /* open in endpoint */
  usbd_ept_open(pudev, USBD_CDC_INT_EPT, EPT_INT_TYPE, USBD_CDC_CMD_MAXPACKET_SIZE);
  
  /* open in endpoint */
  usbd_ept_open(pudev, USBD_CDC_BULK_IN_EPT, EPT_BULK_TYPE, USBD_CDC_MSC_IN_MAXPACKET_SIZE);
  
  /* open out endpoint */
  usbd_ept_open(pudev, USBD_CDC_BULK_OUT_EPT, EPT_BULK_TYPE, USBD_CDC_MSC_OUT_MAXPACKET_SIZE);
  
  /* set out endpoint to receive status */
  usbd_ept_recv(pudev, USBD_CDC_BULK_OUT_EPT, pcdcmsc->g_rx_buff, USBD_CDC_MSC_OUT_MAXPACKET_SIZE);

  /* open in endpoint */
  usbd_ept_open(pudev, USBD_MSC_BULK_IN_EPT, EPT_BULK_TYPE, USBD_CDC_MSC_IN_MAXPACKET_SIZE);
  
  /* open out endpoint */
  usbd_ept_open(pudev, USBD_MSC_BULK_OUT_EPT, EPT_BULK_TYPE, USBD_CDC_MSC_OUT_MAXPACKET_SIZE);

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
  usbd_ept_close(pudev, USBD_CDC_INT_EPT);
  
  /* close in endpoint */
  usbd_ept_close(pudev, USBD_CDC_BULK_IN_EPT);
  
  /* close out endpoint */
  usbd_ept_close(pudev, USBD_CDC_BULK_OUT_EPT);

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
  switch(setup->bmRequestType & USB_REQ_RECIPIENT_MASK)
  {
    case USB_REQ_RECIPIENT_INTERFACE:
      if(setup->wIndex == VCPMSC_MSC_INTERFACE)
      {
        msc_class_setup_handler(udev, setup);
      }
      else
      {
        cdc_class_setup_handler(pudev, setup);
      }
      break;
    case USB_REQ_RECIPIENT_ENDPOINT:
      if(setup->wIndex == (USBD_MSC_BULK_IN_EPT | USBD_MSC_BULK_OUT_EPT))
      {
        msc_class_setup_handler(udev, setup);
      }
      else
      {
        cdc_class_setup_handler(pudev, setup);
      }
      break;
    
  }
  return status;
}

/**
  * @brief  usb device class setup request handler
  * @param  udev: to the structure of usbd_core_type
  * @param  setup: setup packet
  * @retval status of usb_sts_type                            
  */
static usb_sts_type cdc_class_setup_handler(void *udev, usb_setup_type *setup)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pcdc = (cdc_msc_struct_type *)pudev->class_handler->pdata;

  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
  {
    /* class request */
    case USB_REQ_TYPE_CLASS:
      if(setup->wLength)
      {
        if(setup->bmRequestType & USB_REQ_DIR_DTH)
        {
          usb_vcp_cmd_process(udev, setup->bRequest, pcdc->g_cmd, setup->wLength);
          usbd_ctrl_send(pudev, pcdc->g_cmd, setup->wLength);
        }
        else
        {
          pcdc->g_req = setup->bRequest;
          pcdc->g_len = setup->wLength;
          usbd_ctrl_recv(pudev, pcdc->g_cmd, pcdc->g_len);
          
        }
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
          usbd_ctrl_send(pudev, (uint8_t *)&pcdc->alt_setting, 1);
          break;
        case USB_STD_REQ_SET_INTERFACE:
          pcdc->alt_setting = setup->wValue;
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
    default:
      usbd_ctrl_unsupport(pudev);
      break;
  }
  return status;
}

/**
  * @brief  usb device class setup request handler
  * @param  udev: to the structure of usbd_core_type
  * @param  setup: setup packet
  * @retval status of usb_sts_type                            
  */
static usb_sts_type msc_class_setup_handler(void *udev, usb_setup_type *setup)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  switch(setup->bmRequestType & USB_REQ_TYPE_RESERVED)
  {
    /* class request */
    case USB_REQ_TYPE_CLASS:
      
      switch(setup->bRequest)
      {
        case MSC_REQ_GET_MAX_LUN:
          usbd_ctrl_send(pudev, (uint8_t *)&cdc_msc_struct.max_lun, 1);
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
            usbd_ept_open(pudev, (uint8_t)setup->wIndex, EPT_BULK_TYPE, USBD_CDC_MSC_IN_MAXPACKET_SIZE);
          }
          else
          {
            usbd_ept_open(pudev, (uint8_t)setup->wIndex, EPT_BULK_TYPE, USBD_CDC_MSC_OUT_MAXPACKET_SIZE);
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
  cdc_msc_struct_type *pcdc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  uint32_t recv_len = usbd_get_recv_len(pudev, 0);
  /* ...user code... */
  if( pcdc->g_req == SET_LINE_CODING)
  {
    /* class process */
    usb_vcp_cmd_process(udev, pcdc->g_req, pcdc->g_cmd, recv_len);
  }
  
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
  cdc_msc_struct_type *pcdcmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  usb_sts_type status = USB_OK;
  
  /* ...user code...
    trans next packet data
  */

  if((ept_num & 0x7F) == (USBD_CDC_BULK_IN_EPT & 0x7F))
  {
    pcdcmsc->g_tx_completed = 1;
  }
  if((ept_num & 0x7F) == (USBD_MSC_BULK_IN_EPT & 0x7F))
  {
    bot_scsi_datain_handler(udev, ept_num);
  }
  
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
  cdc_msc_struct_type *pcdcmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  
  
  
  if((ept_num & 0x7F) == (USBD_CDC_BULK_OUT_EPT & 0x7F))
  {
    /* get endpoint receive data length  */
    pcdcmsc->g_rxlen = usbd_get_recv_len(pudev, ept_num);
  
    /*set recv flag*/
    pcdcmsc->g_rx_completed = 1;
  }
  if((ept_num & 0x7F) == (USBD_MSC_BULK_OUT_EPT & 0x7F))
  {
    bot_scsi_dataout_handler(udev, ept_num);
  }

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
  * @brief  usb device cdc init
  * @param  pcdc: to the structure of cdc_struct
  * @retval status of usb_sts_type                             
  */
static usb_sts_type cdc_struct_init(cdc_msc_struct_type *pcdc)
{
  pcdc->g_tx_completed = 1;
  pcdc->g_rx_completed = 0;
  pcdc->alt_setting = 0;
  pcdc->linecoding.bitrate = linecoding.bitrate;
  pcdc->linecoding.data = linecoding.data;
  pcdc->linecoding.format = linecoding.format;
  pcdc->linecoding.parity = linecoding.parity;
  return USB_OK;
}

/**
  * @brief  usb device class rx data process
  * @param  udev: to the structure of usbd_core_type
  * @param  recv_data: receive buffer
  * @retval receive data len                            
  */
uint16_t usb_vcp_get_rxdata(void *udev, uint8_t *recv_data)
{
  uint16_t i_index = 0;
  uint16_t tmp_len = 0;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pcdc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  
 if(pcdc->g_rx_completed == 0)
  {
    return 0;
  }
  pcdc->g_rx_completed = 0;
  tmp_len = pcdc->g_rxlen;
  for(i_index = 0; i_index < pcdc->g_rxlen; i_index ++)
  {
    recv_data[i_index] = pcdc->g_rx_buff[i_index];
  }
  
  usbd_ept_recv(pudev, USBD_CDC_BULK_OUT_EPT, pcdc->g_rx_buff, USBD_CDC_MSC_OUT_MAXPACKET_SIZE);
  
  return tmp_len;
}

/**
  * @brief  usb device class send data
  * @param  udev: to the structure of usbd_core_type
  * @param  send_data: send data buffer
  * @param  len: send length
  * @retval error status                            
  */
error_status usb_vcp_send_data(void *udev, uint8_t *send_data, uint16_t len)
{
  error_status status = SUCCESS;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pcdc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  if(pcdc->g_tx_completed)
  {
    pcdc->g_tx_completed = 0;
    usbd_ept_send(pudev, USBD_CDC_BULK_IN_EPT, send_data, len);
  }
  else
  {
    status = ERROR;
  }
  return status;
}

/**
  * @brief  usb device function
  * @param  udev: to the structure of usbd_core_type
  * @param  cmd: request number
  * @param  buff: request buffer
  * @param  len: buffer length
  * @retval none                            
  */
static void usb_vcp_cmd_process(void *udev, uint8_t cmd, uint8_t *buff, uint16_t len)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pcdc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  switch(cmd)
  {
    case SET_LINE_CODING:
      pcdc->linecoding.bitrate = (uint32_t)(buff[0] | (buff[1] << 8) | (buff[2] << 16) | (buff[3] <<24));
      pcdc->linecoding.format = buff[4];
      pcdc->linecoding.parity = buff[5];
      pcdc->linecoding.data = buff[6];
#ifdef USB_VIRTUAL_COMPORT
      /* set hardware usart */
      usb_usart_config(pcdc->linecoding);
#endif
      break;
    
    case GET_LINE_CODING:
      buff[0] = (uint8_t)pcdc->linecoding.bitrate;
      buff[1] = (uint8_t)(pcdc->linecoding.bitrate >> 8);
      buff[2] = (uint8_t)(pcdc->linecoding.bitrate >> 16);
      buff[3] = (uint8_t)(pcdc->linecoding.bitrate >> 24);
      buff[4] = (uint8_t)(pcdc->linecoding.format);
      buff[5] = (uint8_t)(pcdc->linecoding.parity);
      buff[6] = (uint8_t)(pcdc->linecoding.data);
      break;
    
    default:
      break;
  }
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

