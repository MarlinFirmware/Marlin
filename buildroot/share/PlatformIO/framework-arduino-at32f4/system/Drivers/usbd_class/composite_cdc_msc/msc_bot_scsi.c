/**
  **************************************************************************
  * @file     msc_bot_scsi.c
  * @brief    usb mass storage bulk-only transport and scsi command
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
// #include "msc_bot_scsi.h"
#include "cdc_msc_class.h"
#include "msc_diskio.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */

/** @defgroup USB_msc_bot_scsi
  * @brief usb device class mass storage demo
  * @{
  */  

/** @defgroup USB_msc_bot_functions
  * @{
  */


#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD uint8_t page00_inquiry_data[] ALIGNED_TAIL = {
  0x00,    
  0x00, 
  0x00, 
  0x00,
  0x00, 

};
#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD sense_type sense_data ALIGNED_TAIL =
{
  0x70,
  0x00,
  SENSE_KEY_ILLEGAL_REQUEST,
  0x00000000,
  0x0A,
  0x00000000,
  0x20,
  0x00,
  0x00000000
};

#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD uint8_t mode_sense6_data[8] ALIGNED_TAIL = 
{
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};

#if defined ( __ICCARM__ ) /* iar compiler */
  #pragma data_alignment=4
#endif
ALIGNED_HEAD uint8_t mode_sense10_data[8] ALIGNED_TAIL = 
{
  0x00,
  0x06,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};
/**
  * @brief  initialize bulk-only transport and scsi
  * @param  udev: to the structure of usbd_core_type
  * @retval none                            
  */
void bot_scsi_init(void *udev)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  pmsc->msc_state = MSC_STATE_MACHINE_IDLE;
  pmsc->bot_status = MSC_BOT_STATE_IDLE;
  pmsc->max_lun = MSC_SUPPORT_MAX_LUN - 1;
  
  pmsc->csw_struct.dCSWSignature = CSW_DCSWSIGNATURE;
  pmsc->csw_struct.dCSWDataResidue = 0;
  pmsc->csw_struct.dCSWSignature = 0;
  pmsc->csw_struct.dCSWTag = CSW_BCSWSTATUS_PASS;
  
  /* set out endpoint to receive status */
  usbd_ept_recv(pudev, USBD_MSC_BULK_OUT_EPT, (uint8_t *)&pmsc->cbw_struct, CBW_CMD_LENGTH);
}

/**
  * @brief  reset bulk-only transport and scsi
  * @param  udev: to the structure of usbd_core_type
  * @retval none                            
  */
void bot_scsi_reset(void *udev)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  pmsc->msc_state = MSC_STATE_MACHINE_IDLE;
  pmsc->bot_status = MSC_BOT_STATE_RECOVERY;
  pmsc->max_lun = MSC_SUPPORT_MAX_LUN - 1;
  
  /* set out endpoint to receive status */
  usbd_ept_recv(pudev, USBD_MSC_BULK_OUT_EPT, (uint8_t *)&pmsc->cbw_struct, CBW_CMD_LENGTH);
}

/**
  * @brief  bulk-only transport data in handler
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval none                            
  */
void bot_scsi_datain_handler(void *udev, uint8_t ept_num)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  switch(pmsc->msc_state)
  {
    case MSC_STATE_MACHINE_DATA_IN:
      if(bot_scsi_cmd_process(udev) != USB_OK)
      {
        bot_scsi_send_csw(udev, CSW_BCSWSTATUS_FAILED);
      }
      break;
    
    case MSC_STATE_MACHINE_LAST_DATA:
    case MSC_STATE_MACHINE_SEND_DATA:
      bot_scsi_send_csw(udev, CSW_BCSWSTATUS_PASS);
      break;
    
    default:
      break;
  }
}

/**
  * @brief  bulk-only transport data out handler
  * @param  udev: to the structure of usbd_core_type
  * @param  ept_num: endpoint number
  * @retval none                            
  */
void bot_scsi_dataout_handler(void *udev, uint8_t ept_num)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  switch(pmsc->msc_state)
  {
    case MSC_STATE_MACHINE_IDLE:
      bot_cbw_decode(udev);
      break;
    
    case MSC_STATE_MACHINE_DATA_OUT:
      if(bot_scsi_cmd_process(udev) != USB_OK)
      {
        bot_scsi_send_csw(udev, CSW_BCSWSTATUS_FAILED);
      }
      break;
  }
}

/**
  * @brief  bulk-only cbw decode
  * @param  udev: to the structure of usbd_core_type
  * @retval none                            
  */
void bot_cbw_decode(void *udev)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  
  pmsc->csw_struct.dCSWTag = pmsc->cbw_struct.dCBWTage;
  pmsc->csw_struct.dCSWDataResidue = pmsc->cbw_struct.dCBWDataTransferLength;
  
  /* check param */
  if((pmsc->cbw_struct.dCBWSignature != CBW_DCBWSIGNATURE) ||
    (usbd_get_recv_len(pudev, USBD_MSC_BULK_OUT_EPT) != CBW_CMD_LENGTH)
    || (pmsc->cbw_struct.bCBWLUN > MSC_SUPPORT_MAX_LUN) ||
      (pmsc->cbw_struct.bCBWCBLength < 1) || (pmsc->cbw_struct.bCBWCBLength > 16))
  {
    bot_scsi_sense_code(udev, SENSE_KEY_ILLEGAL_REQUEST, INVALID_COMMAND);
    pmsc->bot_status = MSC_BOT_STATE_ERROR;
    bot_scsi_stall(udev);
  }
  else
  {
    if(bot_scsi_cmd_process(udev) != USB_OK)
    {
      bot_scsi_stall(udev);
    }
    else if((pmsc->msc_state != MSC_STATE_MACHINE_DATA_IN) &&
            (pmsc->msc_state != MSC_STATE_MACHINE_DATA_OUT) &&
            (pmsc->msc_state != MSC_STATE_MACHINE_LAST_DATA))
    {
      if(pmsc->data_len == 0)
      {
        bot_scsi_send_csw(udev, CSW_BCSWSTATUS_PASS);
      }
      else if(pmsc->data_len > 0)
      {
        bot_scsi_send_data(udev, pmsc->data, pmsc->data_len);
      }
    }
  }
}

/**
  * @brief  send bot data
  * @param  udev: to the structure of usbd_core_type
  * @param  buffer: data buffer
  * @param  len: data len
  * @retval none                            
  */
void bot_scsi_send_data(void *udev, uint8_t *buffer, uint32_t len)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  uint32_t data_len = MIN(len, pmsc->cbw_struct.dCBWDataTransferLength);
  
  pmsc->csw_struct.dCSWDataResidue -= data_len;
  pmsc->csw_struct.bCSWStatus = CSW_BCSWSTATUS_PASS;
  
  pmsc->msc_state = MSC_STATE_MACHINE_SEND_DATA;
  
  usbd_ept_send(pudev, USBD_MSC_BULK_IN_EPT, 
                buffer, data_len);
}

/**
  * @brief  send command status
  * @param  udev: to the structure of usbd_core_type
  * @param  status: csw status
  * @retval none                            
  */
void bot_scsi_send_csw(void *udev, uint8_t status)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  
  pmsc->csw_struct.bCSWStatus = status;
  pmsc->csw_struct.dCSWSignature = CSW_DCSWSIGNATURE;
  pmsc->msc_state = MSC_STATE_MACHINE_IDLE;
  
  usbd_ept_send(pudev, USBD_MSC_BULK_IN_EPT, 
                (uint8_t *)&pmsc->csw_struct, CSW_CMD_LENGTH);
  
  usbd_ept_recv(pudev, USBD_MSC_BULK_OUT_EPT,
               (uint8_t *)&pmsc->cbw_struct, CBW_CMD_LENGTH);
}


/**
  * @brief  send scsi sense code
  * @param  udev: to the structure of usbd_core_type
  * @param  sense_key: sense key
  * @param  asc: asc
  * @retval none                            
  */
void bot_scsi_sense_code(void *udev, uint8_t sense_key, uint8_t asc)
{
  sense_data.sense_key = sense_key;
  sense_data.asc = asc;
}


/**
  * @brief  check address
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @param  blk_offset: blk offset address
  * @param  blk_count: blk number
  * @retval usb_sts_type                            
  */
usb_sts_type bot_scsi_check_address(void *udev, uint8_t lun, uint32_t blk_offset, uint32_t blk_count)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  if((blk_offset + blk_count) > pmsc->blk_nbr[lun])
  {
    bot_scsi_sense_code(udev, SENSE_KEY_ILLEGAL_REQUEST, ADDRESS_OUT_OF_RANGE);
    return USB_FAIL;
  }
  return USB_OK;
}

/**
  * @brief  bot endpoint stall
  * @param  udev: to the structure of usbd_core_type
  * @retval none                            
  */
void bot_scsi_stall(void *udev)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  
  if((pmsc->cbw_struct.dCBWDataTransferLength != 0) &&
    (pmsc->cbw_struct.bmCBWFlags == 0) &&
    pmsc->bot_status == MSC_BOT_STATE_IDLE)
  {
    usbd_set_stall(pudev, USBD_MSC_BULK_OUT_EPT);
  }
  usbd_set_stall(pudev, USBD_MSC_BULK_IN_EPT);
  
  if(pmsc->bot_status == MSC_BOT_STATE_ERROR)
  {
    usbd_ept_recv(pudev, USBD_MSC_BULK_OUT_EPT, 
                 (uint8_t *)&pmsc->cbw_struct, CBW_CMD_LENGTH);
  }
}

/**
  * @brief  bulk-only transport scsi command test unit
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_test_unit(void *udev, uint8_t lun)
{
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  
  if(pmsc->cbw_struct.dCBWDataTransferLength != 0)
  {
    bot_scsi_sense_code(udev, SENSE_KEY_ILLEGAL_REQUEST, INVALID_COMMAND);
    return USB_FAIL;
  }
  
  pmsc->data_len = 0;
  return status;
}

/**
  * @brief  bulk-only transport scsi command inquiry
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_inquiry(void *udev, uint8_t lun)
{
  uint8_t *pdata;
  uint32_t trans_len = 0;
  usb_sts_type status = USB_OK;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  
  if(pmsc->cbw_struct.CBWCB[1] & 0x01)
  {
    pdata = page00_inquiry_data;
    trans_len = 5;
  }
  else
  {
    pdata = get_inquiry(lun);
    if(pmsc->cbw_struct.dCBWDataTransferLength < SCSI_INQUIRY_DATA_LENGTH)
    {
      trans_len = pmsc->cbw_struct.dCBWDataTransferLength;
    }
    else
    {
      trans_len = SCSI_INQUIRY_DATA_LENGTH;
    }
  }
  
  pmsc->data_len = trans_len;
  while(trans_len)
  {
    trans_len --;
    pmsc->data[trans_len] = pdata[trans_len];
  }
  return status;
}

/**
  * @brief  bulk-only transport scsi command start stop
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_start_stop(void *udev, uint8_t lun)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  pmsc->data_len = 0;
  return USB_OK;
}

/**
  * @brief  bulk-only transport scsi command meidum removal
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_allow_medium_removal(void *udev, uint8_t lun)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  pmsc->data_len = 0;
  return USB_OK;
}

/**
  * @brief  bulk-only transport scsi command mode sense6
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_mode_sense6(void *udev, uint8_t lun)
{
  uint8_t data_len = 8;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  pmsc->data_len = 8;
  while(data_len)
  {
    data_len --;
    pmsc->data[data_len] = mode_sense6_data[data_len];
  };
  return USB_OK;
}

/**
  * @brief  bulk-only transport scsi command mode sense10
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_mode_sense10(void *udev, uint8_t lun)
{
  uint8_t data_len = 8;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  pmsc->data_len = 8;
  while(data_len)
  {
    data_len --;
    pmsc->data[data_len] = mode_sense10_data[data_len];
  };
  return USB_OK;
}

/**
  * @brief  bulk-only transport scsi command capacity
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_capacity(void *udev, uint8_t lun)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  uint8_t *pdata = pmsc->data;
  msc_disk_capacity(lun, &pmsc->blk_nbr[lun], &pmsc->blk_size[lun]);
    
  pdata[0] = (uint8_t)((pmsc->blk_nbr[lun] - 1) >> 24);
  pdata[1] = (uint8_t)((pmsc->blk_nbr[lun] - 1) >> 16);
  pdata[2] = (uint8_t)((pmsc->blk_nbr[lun] - 1) >> 8);
  pdata[3] = (uint8_t)((pmsc->blk_nbr[lun] - 1));
  
  pdata[4] = (uint8_t)((pmsc->blk_size[lun]) >> 24);
  pdata[5] = (uint8_t)((pmsc->blk_size[lun]) >> 16);
  pdata[6] = (uint8_t)((pmsc->blk_size[lun]) >> 8);
  pdata[7] = (uint8_t)((pmsc->blk_size[lun]));
  
  pmsc->data_len = 8;
  return USB_OK;
}

/**
  * @brief  bulk-only transport scsi command format capacity
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_format_capacity(void *udev, uint8_t lun)
{  
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  uint8_t *pdata = pmsc->data;
  
  pdata[0] = 0;
  pdata[1] = 0;
  pdata[2] = 0;
  pdata[3] = 0x08;
  
  msc_disk_capacity(lun, &pmsc->blk_nbr[lun], &pmsc->blk_size[lun]);
  
  pdata[4] = (uint8_t)((pmsc->blk_nbr[lun] - 1) >> 24);
  pdata[5] = (uint8_t)((pmsc->blk_nbr[lun] - 1) >> 16);
  pdata[6] = (uint8_t)((pmsc->blk_nbr[lun] - 1) >> 8);
  pdata[7] = (uint8_t)((pmsc->blk_nbr[lun] - 1));
  
  pdata[8] = 0x02;
  
  pdata[9] = (uint8_t)((pmsc->blk_size[lun]) >> 16);
  pdata[10] = (uint8_t)((pmsc->blk_size[lun]) >> 8);
  pdata[11] = (uint8_t)((pmsc->blk_size[lun]));
  
  pmsc->data_len = 12;
  
  return USB_OK;
}

/**
  * @brief  bulk-only transport scsi command request sense
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_request_sense(void *udev, uint8_t lun)
{
  uint32_t trans_len = 0x12;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  uint8_t *pdata = pmsc->data;
  uint8_t *sdata = (uint8_t *)&sense_data;
 
  while(trans_len)
  {
    trans_len --;
    pdata[trans_len] = sdata[trans_len];
  }
  
  if(pmsc->cbw_struct.dCBWDataTransferLength < REQ_SENSE_STANDARD_DATA_LEN)
  {
    pmsc->data_len = pmsc->cbw_struct.dCBWDataTransferLength;
  }
  else
  {
    pmsc->data_len = REQ_SENSE_STANDARD_DATA_LEN;
  }
  return USB_OK;
}

/**
  * @brief  bulk-only transport scsi command verify
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_verify(void *udev, uint8_t lun)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  uint8_t *cmd = pmsc->cbw_struct.CBWCB;
  if((pmsc->cbw_struct.CBWCB[1] & 0x02) == 0x02)
  {
    bot_scsi_sense_code(udev, SENSE_KEY_ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
    return USB_FAIL;
  }
  
  pmsc->blk_addr = cmd[2] << 24 | cmd[3] << 16 | cmd[4] << 8 | cmd[5];
  pmsc->blk_len = cmd[7] << 8 | cmd[8];
  
  if(bot_scsi_check_address(udev, lun, pmsc->blk_addr, pmsc->blk_len) != USB_OK)
  {
    return USB_FAIL;
  }
  pmsc->data_len = 0;
  return USB_OK;
}

/**
  * @brief  bulk-only transport scsi command read10
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_read10(void *udev, uint8_t lun)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  uint8_t *cmd = pmsc->cbw_struct.CBWCB;
  uint32_t len;
  
  if(pmsc->msc_state == MSC_STATE_MACHINE_IDLE)
  {
    if((pmsc->cbw_struct.bmCBWFlags & 0x80) != 0x80)
    {
      bot_scsi_sense_code(udev, SENSE_KEY_ILLEGAL_REQUEST, INVALID_COMMAND);
      return USB_FAIL;
    }
    
    pmsc->blk_addr = cmd[2] << 24 | cmd[3] << 16 | cmd[4] << 8 | cmd[5];
    pmsc->blk_len = cmd[7] << 8 | cmd[8];
    
    if(bot_scsi_check_address(udev, lun, pmsc->blk_addr, pmsc->blk_len) != USB_OK)
    {
      return USB_FAIL;
    }
    
    pmsc->blk_addr *= pmsc->blk_size[lun];
    pmsc->blk_len *= pmsc->blk_size[lun];
    
    if(pmsc->cbw_struct.dCBWDataTransferLength != pmsc->blk_len)
    {
      bot_scsi_sense_code(udev, SENSE_KEY_ILLEGAL_REQUEST, INVALID_COMMAND);
      return USB_FAIL;
    }
    pmsc->msc_state  = MSC_STATE_MACHINE_DATA_IN;
  }
  pmsc->data_len = MSC_MAX_DATA_BUF_LEN;
  
  len = MIN(pmsc->blk_len, MSC_MAX_DATA_BUF_LEN);
  if( msc_disk_read(lun, pmsc->blk_addr, pmsc->data, len) != USB_OK)
  {
    bot_scsi_sense_code(udev, SENSE_KEY_HARDWARE_ERROR, MEDIUM_NOT_PRESENT);
    return USB_FAIL;
  }
  usbd_ept_send(pudev, USBD_MSC_BULK_IN_EPT, pmsc->data, len);
  pmsc->blk_addr += len;
  pmsc->blk_len -= len;
  
  pmsc->csw_struct.dCSWDataResidue -= len;
  if(pmsc->blk_len == 0)
  {
    pmsc->msc_state = MSC_STATE_MACHINE_LAST_DATA;
  }
  
  return USB_OK;
}


/**
  * @brief  bulk-only transport scsi command write10
  * @param  udev: to the structure of usbd_core_type
  * @param  lun: logical units number
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_write10(void *udev, uint8_t lun)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  uint8_t *cmd = pmsc->cbw_struct.CBWCB;
  uint32_t len;
  
  if(pmsc->msc_state == MSC_STATE_MACHINE_IDLE)
  {
    if((pmsc->cbw_struct.bmCBWFlags & 0x80) == 0x80)
    {
      bot_scsi_sense_code(udev, SENSE_KEY_ILLEGAL_REQUEST, INVALID_COMMAND);
      return USB_FAIL;
    }
    
    pmsc->blk_addr = cmd[2] << 24 | cmd[3] << 16 | cmd[4] << 8 | cmd[5];
    pmsc->blk_len = cmd[7] << 8 | cmd[8];
    
    if(bot_scsi_check_address(udev, lun, pmsc->blk_addr, pmsc->blk_len) != USB_OK)
    {
      return USB_FAIL;
    }
    
    pmsc->blk_addr *= pmsc->blk_size[lun];
    pmsc->blk_len *= pmsc->blk_size[lun];
    
    if(pmsc->cbw_struct.dCBWDataTransferLength != pmsc->blk_len)
    {
      bot_scsi_sense_code(udev, SENSE_KEY_ILLEGAL_REQUEST, INVALID_COMMAND);
      return USB_FAIL;
    }
    
    pmsc->msc_state  = MSC_STATE_MACHINE_DATA_OUT;
    len = MIN(pmsc->blk_len, MSC_MAX_DATA_BUF_LEN);
    usbd_ept_recv(pudev, USBD_MSC_BULK_OUT_EPT, (uint8_t *)pmsc->data, len);
    
  }
  else
  {
    len = MIN(pmsc->blk_len, MSC_MAX_DATA_BUF_LEN);
    if(msc_disk_write(lun, pmsc->blk_addr, pmsc->data, len) != USB_OK)
    {
      bot_scsi_sense_code(udev, SENSE_KEY_HARDWARE_ERROR, MEDIUM_NOT_PRESENT);
      return USB_FAIL;
    }
    
    pmsc->blk_addr += len;
    pmsc->blk_len -= len;
  
    pmsc->csw_struct.dCSWDataResidue -= len;
    
    if(pmsc->blk_len == 0)
    {
      bot_scsi_send_csw(udev, CSW_BCSWSTATUS_PASS);
    }
    else
    {
      len = MIN(pmsc->blk_len, MSC_MAX_DATA_BUF_LEN);
      usbd_ept_recv(pudev, USBD_MSC_BULK_OUT_EPT, (uint8_t *)pmsc->data, len);
    }
  }
  return USB_OK;
}

/**
  * @brief  clear feature
  * @param  udev: to the structure of usbd_core_type
  * @param  etp_num: endpoint number
  * @retval status of usb_sts_type                            
  */
void bot_scsi_clear_feature(void *udev, uint8_t ept_num)
{
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  if(pmsc->bot_status == MSC_BOT_STATE_ERROR)
  {
    usbd_set_stall(pudev, USBD_MSC_BULK_IN_EPT);
    pmsc->bot_status = MSC_BOT_STATE_IDLE;
  }
  else if(((ept_num & 0x80) == 0x80) && (pmsc->bot_status != MSC_BOT_STATE_RECOVERY))
  {
    bot_scsi_send_csw(udev, CSW_BCSWSTATUS_FAILED);
  }
}

/**
  * @brief  bulk-only transport scsi command process
  * @param  udev: to the structure of usbd_core_type
  * @retval status of usb_sts_type                            
  */
usb_sts_type bot_scsi_cmd_process(void *udev)
{
  usb_sts_type status = USB_FAIL;
  usbd_core_type *pudev = (usbd_core_type *)udev;
  cdc_msc_struct_type *pmsc = (cdc_msc_struct_type *)pudev->class_handler->pdata;
  switch(pmsc->cbw_struct.CBWCB[0])
  {
    case MSC_CMD_INQUIRY:
      status = bot_scsi_inquiry(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_START_STOP:
      status = bot_scsi_start_stop(udev, pmsc->cbw_struct.bCBWLUN);
      break;  
    
    case MSC_CMD_MODE_SENSE6:
      status = bot_scsi_mode_sense6(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_MODE_SENSE10:
      status = bot_scsi_mode_sense10(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_ALLOW_MEDIUM_REMOVAL:
      status = bot_scsi_allow_medium_removal(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_READ_10:
      status = bot_scsi_read10(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_READ_CAPACITY:
      status = bot_scsi_capacity(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_REQUEST_SENSE:
      status = bot_scsi_request_sense(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_TEST_UNIT:
      status = bot_scsi_test_unit(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_VERIFY:
      status = bot_scsi_verify(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_WRITE_10:
      status = bot_scsi_write10(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    case MSC_CMD_READ_FORMAT_CAPACITY:
      status = bot_scsi_format_capacity(udev, pmsc->cbw_struct.bCBWLUN);
      break;
    
    default:
      bot_scsi_sense_code(udev, SENSE_KEY_ILLEGAL_REQUEST, INVALID_COMMAND);
      status = USB_FAIL;
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
