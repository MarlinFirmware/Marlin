/**
  ******************************************************************************
  * @file    usbh_msc_scsi.c
  * @author  MCD Application Team
  * @brief   This file implements the SCSI commands
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
- "stm32xxxxx_{eval}{discovery}{adafruit}_lcd.c"
- "stm32xxxxx_{eval}{discovery}_sdram.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "../Inc/usbh_msc.h"
#include "../Inc/usbh_msc_scsi.h"
#include "../Inc/usbh_msc_bot.h"


/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */

/** @defgroup USBH_MSC_SCSI
  * @brief    This file includes the mass storage related functions
  * @{
  */


/** @defgroup USBH_MSC_SCSI_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup USBH_MSC_SCSI_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_MSC_SCSI_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_MSC_SCSI_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_MSC_SCSI_Exported_Variables
  * @{
  */

/**
  * @}
  */


/** @defgroup USBH_MSC_SCSI_Private_Functions
  * @{
  */


/**
  * @brief  USBH_MSC_SCSI_TestUnitReady
  *         Issue TestUnitReady command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_SCSI_TestUnitReady(USBH_HandleTypeDef *phost,
                                               uint8_t lun)
{
  USBH_StatusTypeDef    error = USBH_FAIL ;
  MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

  switch (MSC_Handle->hbot.cmd_state)
  {
    case BOT_CMD_SEND:

      /*Prepare the CBW and relevent field*/
      MSC_Handle->hbot.cbw.field.DataTransferLength = DATA_LEN_MODE_TEST_UNIT_READY;
      MSC_Handle->hbot.cbw.field.Flags = USB_EP_DIR_OUT;
      MSC_Handle->hbot.cbw.field.CBLength = CBW_LENGTH;

      USBH_memset(MSC_Handle->hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
      MSC_Handle->hbot.cbw.field.CB[0]  = OPCODE_TEST_UNIT_READY;

      MSC_Handle->hbot.state = BOT_SEND_CBW;
      MSC_Handle->hbot.cmd_state = BOT_CMD_WAIT;
      error = USBH_BUSY;
      break;

    case BOT_CMD_WAIT:
      error = USBH_MSC_BOT_Process(phost, lun);
      break;

    default:
      break;
  }

  return error;
}

/**
  * @brief  USBH_MSC_SCSI_ReadCapacity
  *         Issue Read Capacity command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  capacity: pointer to the capacity structure
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_SCSI_ReadCapacity(USBH_HandleTypeDef *phost,
                                              uint8_t lun,
                                              SCSI_CapacityTypeDef *capacity)
{
  USBH_StatusTypeDef    error = USBH_BUSY ;
  MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

  switch (MSC_Handle->hbot.cmd_state)
  {
    case BOT_CMD_SEND:

      /*Prepare the CBW and relevent field*/
      MSC_Handle->hbot.cbw.field.DataTransferLength = DATA_LEN_READ_CAPACITY10;
      MSC_Handle->hbot.cbw.field.Flags = USB_EP_DIR_IN;
      MSC_Handle->hbot.cbw.field.CBLength = CBW_LENGTH;

      USBH_memset(MSC_Handle->hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
      MSC_Handle->hbot.cbw.field.CB[0]  = OPCODE_READ_CAPACITY10;

      MSC_Handle->hbot.state = BOT_SEND_CBW;

      MSC_Handle->hbot.cmd_state = BOT_CMD_WAIT;
      MSC_Handle->hbot.pbuf = (uint8_t *)(void *)MSC_Handle->hbot.data;
      error = USBH_BUSY;
      break;

    case BOT_CMD_WAIT:

      error = USBH_MSC_BOT_Process(phost, lun);

      if (error == USBH_OK)
      {
        /*assign the capacity*/
        capacity->block_nbr = MSC_Handle->hbot.pbuf[3] | ((uint32_t)MSC_Handle->hbot.pbuf[2] << 8U) | \
                              ((uint32_t)MSC_Handle->hbot.pbuf[1] << 16U) | ((uint32_t)MSC_Handle->hbot.pbuf[0] << 24U);

        /*assign the page length*/
        capacity->block_size = (uint16_t)(MSC_Handle->hbot.pbuf[7] | ((uint32_t)MSC_Handle->hbot.pbuf[6] << 8U));
      }
      break;

    default:
      break;
  }

  return error;
}

/**
  * @brief  USBH_MSC_SCSI_Inquiry
  *         Issue Inquiry command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  capacity: pointer to the inquiry structure
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_SCSI_Inquiry(USBH_HandleTypeDef *phost, uint8_t lun,
                                         SCSI_StdInquiryDataTypeDef *inquiry)
{
  USBH_StatusTypeDef error = USBH_FAIL;
  MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

  switch (MSC_Handle->hbot.cmd_state)
  {
    case BOT_CMD_SEND:

      /*Prepare the CBW and relevent field*/
      MSC_Handle->hbot.cbw.field.DataTransferLength = DATA_LEN_INQUIRY;
      MSC_Handle->hbot.cbw.field.Flags = USB_EP_DIR_IN;
      MSC_Handle->hbot.cbw.field.CBLength = CBW_LENGTH;

      USBH_memset(MSC_Handle->hbot.cbw.field.CB, 0, CBW_LENGTH);
      MSC_Handle->hbot.cbw.field.CB[0]  = OPCODE_INQUIRY;
      MSC_Handle->hbot.cbw.field.CB[1]  = (lun << 5);
      MSC_Handle->hbot.cbw.field.CB[2]  = 0U;
      MSC_Handle->hbot.cbw.field.CB[3]  = 0U;
      MSC_Handle->hbot.cbw.field.CB[4]  = 0x24U;
      MSC_Handle->hbot.cbw.field.CB[5]  = 0U;

      MSC_Handle->hbot.state = BOT_SEND_CBW;

      MSC_Handle->hbot.cmd_state = BOT_CMD_WAIT;
      MSC_Handle->hbot.pbuf = (uint8_t *)(void *)MSC_Handle->hbot.data;
      error = USBH_BUSY;
      break;

    case BOT_CMD_WAIT:

      error = USBH_MSC_BOT_Process(phost, lun);

      if (error == USBH_OK)
      {
        USBH_memset(inquiry, 0, sizeof(SCSI_StdInquiryDataTypeDef));
        /*assign Inquiry Data */
        inquiry->DeviceType = MSC_Handle->hbot.pbuf[0] & 0x1FU;
        inquiry->PeripheralQualifier = MSC_Handle->hbot.pbuf[0] >> 5U;

        if (((uint32_t)MSC_Handle->hbot.pbuf[1] & 0x80U) == 0x80U)
        {
          inquiry->RemovableMedia = 1U;
        }
        else
        {
          inquiry->RemovableMedia = 0U;
        }

        USBH_memcpy(inquiry->vendor_id, &MSC_Handle->hbot.pbuf[8], 8U);
        USBH_memcpy(inquiry->product_id, &MSC_Handle->hbot.pbuf[16], 16U);
        USBH_memcpy(inquiry->revision_id, &MSC_Handle->hbot.pbuf[32], 4U);
      }
      break;

    default:
      break;
  }

  return error;
}

/**
  * @brief  USBH_MSC_SCSI_RequestSense
  *         Issue RequestSense command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  capacity: pointer to the sense data structure
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_SCSI_RequestSense(USBH_HandleTypeDef *phost,
                                              uint8_t lun,
                                              SCSI_SenseTypeDef *sense_data)
{
  USBH_StatusTypeDef    error = USBH_FAIL ;
  MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

  switch (MSC_Handle->hbot.cmd_state)
  {
    case BOT_CMD_SEND:

      /*Prepare the CBW and relevent field*/
      MSC_Handle->hbot.cbw.field.DataTransferLength = DATA_LEN_REQUEST_SENSE;
      MSC_Handle->hbot.cbw.field.Flags = USB_EP_DIR_IN;
      MSC_Handle->hbot.cbw.field.CBLength = CBW_LENGTH;

      USBH_memset(MSC_Handle->hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
      MSC_Handle->hbot.cbw.field.CB[0]  = OPCODE_REQUEST_SENSE;
      MSC_Handle->hbot.cbw.field.CB[1]  = (lun << 5);
      MSC_Handle->hbot.cbw.field.CB[2]  = 0U;
      MSC_Handle->hbot.cbw.field.CB[3]  = 0U;
      MSC_Handle->hbot.cbw.field.CB[4]  = DATA_LEN_REQUEST_SENSE;
      MSC_Handle->hbot.cbw.field.CB[5]  = 0U;

      MSC_Handle->hbot.state = BOT_SEND_CBW;
      MSC_Handle->hbot.cmd_state = BOT_CMD_WAIT;
      MSC_Handle->hbot.pbuf = (uint8_t *)(void *)MSC_Handle->hbot.data;
      error = USBH_BUSY;
      break;

    case BOT_CMD_WAIT:

      error = USBH_MSC_BOT_Process(phost, lun);

      if (error == USBH_OK)
      {
        sense_data->key  = MSC_Handle->hbot.pbuf[2] & 0x0FU;
        sense_data->asc  = MSC_Handle->hbot.pbuf[12];
        sense_data->ascq = MSC_Handle->hbot.pbuf[13];
      }
      break;

    default:
      break;
  }

  return error;
}

/**
  * @brief  USBH_MSC_SCSI_Write
  *         Issue write10 command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  address: sector address
  * @param  pbuf: pointer to data
  * @param  length: number of sector to write
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_SCSI_Write(USBH_HandleTypeDef *phost,
                                       uint8_t lun,
                                       uint32_t address,
                                       uint8_t *pbuf,
                                       uint32_t length)
{
  USBH_StatusTypeDef    error = USBH_FAIL ;

  MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

  switch (MSC_Handle->hbot.cmd_state)
  {
    case BOT_CMD_SEND:

      /*Prepare the CBW and relevent field*/
      MSC_Handle->hbot.cbw.field.DataTransferLength = length * MSC_Handle->unit[0].capacity.block_size;
      MSC_Handle->hbot.cbw.field.Flags = USB_EP_DIR_OUT;
      MSC_Handle->hbot.cbw.field.CBLength = CBW_LENGTH;

      USBH_memset(MSC_Handle->hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
      MSC_Handle->hbot.cbw.field.CB[0]  = OPCODE_WRITE10;

      /*logical block address*/
      MSC_Handle->hbot.cbw.field.CB[2]  = (((uint8_t *)(void *)&address)[3]);
      MSC_Handle->hbot.cbw.field.CB[3]  = (((uint8_t *)(void *)&address)[2]);
      MSC_Handle->hbot.cbw.field.CB[4]  = (((uint8_t *)(void *)&address)[1]);
      MSC_Handle->hbot.cbw.field.CB[5]  = (((uint8_t *)(void *)&address)[0]);


      /*Transfer length */
      MSC_Handle->hbot.cbw.field.CB[7]  = (((uint8_t *)(void *)&length)[1]) ;
      MSC_Handle->hbot.cbw.field.CB[8]  = (((uint8_t *)(void *)&length)[0]) ;


      MSC_Handle->hbot.state = BOT_SEND_CBW;
      MSC_Handle->hbot.cmd_state = BOT_CMD_WAIT;
      MSC_Handle->hbot.pbuf = pbuf;
      error = USBH_BUSY;
      break;

    case BOT_CMD_WAIT:
      error = USBH_MSC_BOT_Process(phost, lun);
      break;

    default:
      break;
  }

  return error;
}

/**
  * @brief  USBH_MSC_SCSI_Read
  *         Issue Read10 command.
  * @param  phost: Host handle
  * @param  lun: Logical Unit Number
  * @param  address: sector address
  * @param  pbuf: pointer to data
  * @param  length: number of sector to read
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_MSC_SCSI_Read(USBH_HandleTypeDef *phost,
                                      uint8_t lun,
                                      uint32_t address,
                                      uint8_t *pbuf,
                                      uint32_t length)
{
  USBH_StatusTypeDef    error = USBH_FAIL ;
  MSC_HandleTypeDef *MSC_Handle = (MSC_HandleTypeDef *) phost->pActiveClass->pData;

  switch (MSC_Handle->hbot.cmd_state)
  {
    case BOT_CMD_SEND:

      /*Prepare the CBW and relevent field*/
      MSC_Handle->hbot.cbw.field.DataTransferLength = length * MSC_Handle->unit[0].capacity.block_size;
      MSC_Handle->hbot.cbw.field.Flags = USB_EP_DIR_IN;
      MSC_Handle->hbot.cbw.field.CBLength = CBW_LENGTH;

      USBH_memset(MSC_Handle->hbot.cbw.field.CB, 0, CBW_CB_LENGTH);
      MSC_Handle->hbot.cbw.field.CB[0]  = OPCODE_READ10;

      /*logical block address*/
      MSC_Handle->hbot.cbw.field.CB[2]  = (((uint8_t *)(void *)&address)[3]);
      MSC_Handle->hbot.cbw.field.CB[3]  = (((uint8_t *)(void *)&address)[2]);
      MSC_Handle->hbot.cbw.field.CB[4]  = (((uint8_t *)(void *)&address)[1]);
      MSC_Handle->hbot.cbw.field.CB[5]  = (((uint8_t *)(void *)&address)[0]);


      /*Transfer length */
      MSC_Handle->hbot.cbw.field.CB[7]  = (((uint8_t *)(void *)&length)[1]) ;
      MSC_Handle->hbot.cbw.field.CB[8]  = (((uint8_t *)(void *)&length)[0]) ;


      MSC_Handle->hbot.state = BOT_SEND_CBW;
      MSC_Handle->hbot.cmd_state = BOT_CMD_WAIT;
      MSC_Handle->hbot.pbuf = pbuf;
      error = USBH_BUSY;
      break;

    case BOT_CMD_WAIT:
      error = USBH_MSC_BOT_Process(phost, lun);
      break;

    default:
      break;
  }

  return error;
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

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



