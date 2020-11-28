/**
  ******************************************************************************
  * @file    usbh_usr.cpp
  * @author  MCD Application Team
  * @brief   This file implements the board support package for the USB host library
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../../inc/MarlinConfig.h"

#if ENABLED(USB_HOST_MSC_FLASH_SUPPORT)

#include "usbh_usr.h"
#include "STM32_USB_Host_Library/Core/Inc/usbh_def.h"
#include "STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc.h"

USBH_HandleTypeDef MarlinUSB_Host;

uint8_t usbh_media_ready = 0;

/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_1 */
  switch(id)
  {
  case HOST_USER_CLASS_ACTIVE:
    usbh_media_ready = 1;
    break;

  case HOST_USER_SELECT_CONFIGURATION:
  case HOST_USER_DISCONNECTION:
  case HOST_USER_CONNECTION:
  default:
    usbh_media_ready = 0;
    break;
  }
  /* USER CODE END CALL_BACK_1 */
}

void USBH_HAL_Init(void)
{
  USBH_Init(&MarlinUSB_Host, USBH_UserProcess, 0);
  USBH_RegisterClass(&MarlinUSB_Host, USBH_MSC_CLASS);
  USBH_Start(&MarlinUSB_Host);
}

void USB_HAL_Loop(void)
{
  USBH_Process(&MarlinUSB_Host);
}

uint8_t USBH_IsConnected(void)
{
  return usbh_media_ready;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : lun id
  * @retval DSTATUS: Operation status, 0:ok 1:error
  */
uint8_t USBH_status(uint8_t lun)
{
  if(USBH_MSC_UnitIsReady(&MarlinUSB_Host, lun))
  {
    return 0; // Media ready
  }
  else
  {
    return 1; // Error
  }
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result, 0:ok 1:error
  */
uint8_t USBH_read(uint8_t lun, uint8_t *buff, uint32_t sector, uint32_t count)
{
  uint8_t res = 1;
  MSC_LUNTypeDef info;

  if(USBH_MSC_Read(&MarlinUSB_Host, lun, sector, buff, count) == USBH_OK)
  {
    res = 0; // Read successful
  }
  else
  {
    USBH_MSC_GetLUNInfo(&MarlinUSB_Host, lun, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog ("USB Disk is not ready!");
      res = 3; // Media not ready
      break;

    default:
      res = 1; // Error
      break;
    }
  }

  return res;
}

/* USER CODE BEGIN beforeWriteSection */
/* can be used to modify previous code / undefine following code / add new code */
/* USER CODE END beforeWriteSection */

/**
  * @brief  Writes Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result, 0:ok 1:error
  */
uint8_t USBH_write(uint8_t lun, const uint8_t *buff, uint32_t sector, uint32_t count)
{
  uint8_t res = 1;
  MSC_LUNTypeDef info;

  if(USBH_MSC_Write(&MarlinUSB_Host, lun, sector, (uint8_t *)buff, count) == USBH_OK)
  {
    res = 0; // Write successful
  }
  else
  {
    USBH_MSC_GetLUNInfo(&MarlinUSB_Host, lun, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_WRITE_PROTECTED:
      USBH_ErrLog("USB Disk is Write protected!");
      res = 2; // Write protected
      break;

    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog("USB Disk is not ready!");
      res = 3; // Media not ready
      break;

    default:
      res = 1; // Error
      break;
    }
  }

  return res;
}

#endif // USB_HOST_MSC_FLASH_SUPPORT
#endif // STM32
