/**
  ******************************************************************************
  * @file    usbh_msc.h
  * @author  MCD Application Team
  * @brief   This file contains all the prototypes for the usbh_msc.c
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_MSC_H
#define __USBH_MSC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_msc_bot.h"
#include "usbh_msc_scsi.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */

/** @defgroup USBH_MSC_CORE
  * @brief This file is the Header file for usbh_msc.c
  * @{
  */


/** @defgroup USBH_MSC_CORE_Exported_Types
  * @{
  */

typedef enum
{
  MSC_INIT = 0,
  MSC_IDLE,
  MSC_TEST_UNIT_READY,
  MSC_READ_CAPACITY10,
  MSC_READ_INQUIRY,
  MSC_REQUEST_SENSE,
  MSC_READ,
  MSC_WRITE,
  MSC_UNRECOVERED_ERROR,
  MSC_PERIODIC_CHECK,
}
MSC_StateTypeDef;

typedef enum
{
  MSC_OK,
  MSC_NOT_READY,
  MSC_ERROR,

}
MSC_ErrorTypeDef;

typedef enum
{
  MSC_REQ_IDLE = 0,
  MSC_REQ_RESET,
  MSC_REQ_GET_MAX_LUN,
  MSC_REQ_ERROR,
}
MSC_ReqStateTypeDef;

#ifndef MAX_SUPPORTED_LUN
#define MAX_SUPPORTED_LUN       2U
#endif


/* Structure for LUN */
typedef struct
{
  MSC_StateTypeDef            state;
  MSC_ErrorTypeDef            error;
  USBH_StatusTypeDef          prev_ready_state;
  SCSI_CapacityTypeDef        capacity;
  SCSI_SenseTypeDef           sense;
  SCSI_StdInquiryDataTypeDef  inquiry;
  uint8_t                     state_changed;

}
MSC_LUNTypeDef;

/* Structure for MSC process */
typedef struct _MSC_Process
{
  uint8_t              max_lun;
  uint8_t              Reserved[3];
  uint8_t              InPipe;
  uint8_t              OutPipe;
  uint8_t              OutEp;
  uint8_t              InEp;
  uint16_t             OutEpSize;
  uint16_t             InEpSize;
  MSC_StateTypeDef     state;
  MSC_ErrorTypeDef     error;
  MSC_ReqStateTypeDef  req_state;
  MSC_ReqStateTypeDef  prev_req_state;
  BOT_HandleTypeDef    hbot;
  MSC_LUNTypeDef       unit[MAX_SUPPORTED_LUN];
  uint16_t             current_lun;
  uint16_t             rw_lun;
  uint32_t             timer;
}
MSC_HandleTypeDef;


/**
  * @}
  */



/** @defgroup USBH_MSC_CORE_Exported_Defines
  * @{
  */

#define USB_REQ_BOT_RESET                              0xFFU
#define USB_REQ_GET_MAX_LUN                            0xFEU


/* MSC Class Codes */
#define USB_MSC_CLASS                                  0x08U

/* Interface Descriptor field values for HID Boot Protocol */
#define MSC_BOT                                        0x50U
#define MSC_TRANSPARENT                                0x06U
/**
  * @}
  */

/** @defgroup USBH_MSC_CORE_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_MSC_CORE_Exported_Variables
  * @{
  */
extern USBH_ClassTypeDef  USBH_msc;
#define USBH_MSC_CLASS    &USBH_msc

/**
  * @}
  */

/** @defgroup USBH_MSC_CORE_Exported_FunctionsPrototype
  * @{
  */
uint8_t USBH_MSC_IsReady(USBH_HandleTypeDef *phost);
uint8_t USBH_MSC_GetMaxLUN(USBH_HandleTypeDef *phost);
uint8_t USBH_MSC_UnitIsReady(USBH_HandleTypeDef *phost, uint8_t lun);

USBH_StatusTypeDef USBH_MSC_GetLUNInfo(USBH_HandleTypeDef *phost, uint8_t lun,
                                       MSC_LUNTypeDef *info);

USBH_StatusTypeDef USBH_MSC_Read(USBH_HandleTypeDef *phost, uint8_t lun,
                                 uint32_t address, uint8_t *pbuf, uint32_t length);

USBH_StatusTypeDef USBH_MSC_Write(USBH_HandleTypeDef *phost, uint8_t lun,
                                  uint32_t address, uint8_t *pbuf, uint32_t length);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USBH_MSC_H */


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



