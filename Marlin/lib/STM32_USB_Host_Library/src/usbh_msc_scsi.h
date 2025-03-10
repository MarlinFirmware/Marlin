/**
  ******************************************************************************
  * @file    usbh_msc_scsi.h
  * @author  MCD Application Team
  * @brief   Header file for usbh_msc_scsi.c
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
#ifndef __USBH_MSC_SCSI_H
#define __USBH_MSC_SCSI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"


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
  * @brief This file is the Header file for usbh_msc_scsi.c
  * @{
  */


/* Capacity data */
typedef struct
{
  uint32_t block_nbr;
  uint16_t block_size;
} SCSI_CapacityTypeDef;


/* Sense data */
typedef struct
{
  uint8_t key;
  uint8_t asc;
  uint8_t ascq;
} SCSI_SenseTypeDef;

/* INQUIRY data */
typedef struct
{
  uint8_t PeripheralQualifier;
  uint8_t DeviceType;
  uint8_t RemovableMedia;
  uint8_t vendor_id[9];
  uint8_t product_id[17];
  uint8_t revision_id[5];
} SCSI_StdInquiryDataTypeDef;

/** @defgroup USBH_MSC_SCSI_Exported_Defines
  * @{
  */
#define OPCODE_TEST_UNIT_READY            0x00U
#define OPCODE_READ_CAPACITY10            0x25U
#define OPCODE_READ10                     0x28U
#define OPCODE_WRITE10                    0x2AU
#define OPCODE_REQUEST_SENSE              0x03U
#define OPCODE_INQUIRY                    0x12U

#define DATA_LEN_MODE_TEST_UNIT_READY        0U
#define DATA_LEN_READ_CAPACITY10             8U
#define DATA_LEN_INQUIRY                    36U
#define DATA_LEN_REQUEST_SENSE              14U

#define CBW_CB_LENGTH                       16U
#define CBW_LENGTH                          10U

/** @defgroup USBH_MSC_SCSI_Exported_Defines
  * @{
  */
#define SCSI_SENSE_KEY_NO_SENSE                          0x00U
#define SCSI_SENSE_KEY_RECOVERED_ERROR                   0x01U
#define SCSI_SENSE_KEY_NOT_READY                         0x02U
#define SCSI_SENSE_KEY_MEDIUM_ERROR                      0x03U
#define SCSI_SENSE_KEY_HARDWARE_ERROR                    0x04U
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST                   0x05U
#define SCSI_SENSE_KEY_UNIT_ATTENTION                    0x06U
#define SCSI_SENSE_KEY_DATA_PROTECT                      0x07U
#define SCSI_SENSE_KEY_BLANK_CHECK                       0x08U
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC                   0x09U
#define SCSI_SENSE_KEY_COPY_ABORTED                      0x0AU
#define SCSI_SENSE_KEY_ABORTED_COMMAND                   0x0BU
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW                   0x0DU
#define SCSI_SENSE_KEY_MISCOMPARE                        0x0EU
/**
  * @}
  */


/** @defgroup USBH_MSC_SCSI_Exported_Defines
  * @{
  */
#define SCSI_ASC_NO_ADDITIONAL_SENSE_INFORMATION         0x00
#define SCSI_ASC_LOGICAL_UNIT_NOT_READY                  0x04
#define SCSI_ASC_INVALID_FIELD_IN_CDB                    0x24
#define SCSI_ASC_WRITE_PROTECTED                         0x27
#define SCSI_ASC_FORMAT_ERROR                            0x31
#define SCSI_ASC_INVALID_COMMAND_OPERATION_CODE          0x20
#define SCSI_ASC_NOT_READY_TO_READY_CHANGE               0x28
#define SCSI_ASC_MEDIUM_NOT_PRESENT                      0x3A
/**
  * @}
  */


/** @defgroup USBH_MSC_SCSI_Exported_Defines
  * @{
  */
#define SCSI_ASCQ_FORMAT_COMMAND_FAILED                  0x01
#define SCSI_ASCQ_INITIALIZING_COMMAND_REQUIRED          0x02
#define SCSI_ASCQ_OPERATION_IN_PROGRESS                  0x07

/**
  * @}
  */

/** @defgroup USBH_MSC_SCSI_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup _Exported_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup USBH_MSC_SCSI_Exported_FunctionsPrototype
  * @{
  */
USBH_StatusTypeDef USBH_MSC_SCSI_TestUnitReady(USBH_HandleTypeDef *phost,
                                               uint8_t lun);

USBH_StatusTypeDef USBH_MSC_SCSI_ReadCapacity(USBH_HandleTypeDef *phost,
                                              uint8_t lun,
                                              SCSI_CapacityTypeDef *capacity);

USBH_StatusTypeDef USBH_MSC_SCSI_Inquiry(USBH_HandleTypeDef *phost,
                                         uint8_t lun,
                                         SCSI_StdInquiryDataTypeDef *inquiry);

USBH_StatusTypeDef USBH_MSC_SCSI_RequestSense(USBH_HandleTypeDef *phost,
                                              uint8_t lun,
                                              SCSI_SenseTypeDef *sense_data);

USBH_StatusTypeDef USBH_MSC_SCSI_Write(USBH_HandleTypeDef *phost,
                                       uint8_t lun,
                                       uint32_t address,
                                       uint8_t *pbuf,
                                       uint32_t length);

USBH_StatusTypeDef USBH_MSC_SCSI_Read(USBH_HandleTypeDef *phost,
                                      uint8_t lun,
                                      uint32_t address,
                                      uint8_t *pbuf,
                                      uint32_t length);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USBH_MSC_SCSI_H */


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

