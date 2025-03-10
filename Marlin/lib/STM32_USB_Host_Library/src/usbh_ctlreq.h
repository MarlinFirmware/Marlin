/**
  ******************************************************************************
  * @file    usbh_ctlreq.h
  * @author  MCD Application Team
  * @brief   Header file for usbh_ctlreq.c
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
#ifndef __USBH_CTLREQ_H
#define __USBH_CTLREQ_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/

/** @defgroup USBH_CTLREQ
  * @brief This file is the
  * @{
  */


/** @defgroup USBH_CTLREQ_Exported_Defines
  * @{
  */
/*Standard Feature Selector for clear feature command*/
#define FEATURE_SELECTOR_ENDPOINT         0x00U
#define FEATURE_SELECTOR_DEVICE           0x01U
#define FEATURE_SELECTOR_REMOTEWAKEUP     0X01U


#define INTERFACE_DESC_TYPE               0x04U
#define ENDPOINT_DESC_TYPE                0x05U
#define INTERFACE_DESC_SIZE               0x09U

/**
  * @}
  */


/** @defgroup USBH_CTLREQ_Exported_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_CTLREQ_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_CTLREQ_Exported_Variables
  * @{
  */
extern uint8_t USBH_CfgDesc[512];
/**
  * @}
  */

/** @defgroup USBH_CTLREQ_Exported_FunctionsPrototype
  * @{
  */
USBH_StatusTypeDef USBH_CtlReq(USBH_HandleTypeDef *phost, uint8_t *buff,
                               uint16_t length);

USBH_StatusTypeDef USBH_GetDescriptor(USBH_HandleTypeDef *phost,
                                      uint8_t  req_type, uint16_t value_idx,
                                      uint8_t *buff, uint16_t length);

USBH_StatusTypeDef USBH_Get_DevDesc(USBH_HandleTypeDef *phost, uint8_t length);

USBH_StatusTypeDef USBH_Get_StringDesc(USBH_HandleTypeDef *phost,
                                       uint8_t string_index, uint8_t *buff,
                                       uint16_t length);

USBH_StatusTypeDef USBH_SetCfg(USBH_HandleTypeDef *phost, uint16_t cfg_idx);

USBH_StatusTypeDef USBH_Get_CfgDesc(USBH_HandleTypeDef *phost, uint16_t length);

USBH_StatusTypeDef USBH_SetAddress(USBH_HandleTypeDef *phost,
                                   uint8_t DeviceAddress);

USBH_StatusTypeDef USBH_SetInterface(USBH_HandleTypeDef *phost, uint8_t ep_num,
                                     uint8_t altSetting);

USBH_StatusTypeDef USBH_SetFeature(USBH_HandleTypeDef *phost, uint8_t wValue);

USBH_StatusTypeDef USBH_ClrFeature(USBH_HandleTypeDef *phost, uint8_t ep_num);

USBH_DescHeader_t *USBH_GetNextDesc(uint8_t *pbuf, uint16_t *ptr);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_CTLREQ_H */

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


