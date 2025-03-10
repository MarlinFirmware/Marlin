/**
  ******************************************************************************
  * @file    usbh_ioreq.h
  * @author  MCD Application Team
  * @brief   Header file for usbh_ioreq.c
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
#ifndef __USBH_IOREQ_H
#define __USBH_IOREQ_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_usbh_conf.h"
#include "usbh_core.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/

/** @defgroup USBH_IOREQ
  * @brief This file is the header file for usbh_ioreq.c
  * @{
  */


/** @defgroup USBH_IOREQ_Exported_Defines
  * @{
  */

#define USBH_PID_SETUP                            0U
#define USBH_PID_DATA                             1U

#define USBH_EP_CONTROL                           0U
#define USBH_EP_ISO                               1U
#define USBH_EP_BULK                              2U
#define USBH_EP_INTERRUPT                         3U

#define USBH_SETUP_PKT_SIZE                       8U
/**
  * @}
  */


/** @defgroup USBH_IOREQ_Exported_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_IOREQ_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_IOREQ_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_IOREQ_Exported_FunctionsPrototype
  * @{
  */
USBH_StatusTypeDef USBH_CtlSendSetup(USBH_HandleTypeDef *phost,
                                     uint8_t *buff,
                                     uint8_t pipe_num);

USBH_StatusTypeDef USBH_CtlSendData(USBH_HandleTypeDef *phost,
                                    uint8_t *buff,
                                    uint16_t length,
                                    uint8_t pipe_num,
                                    uint8_t do_ping);

USBH_StatusTypeDef USBH_CtlReceiveData(USBH_HandleTypeDef *phost,
                                       uint8_t *buff,
                                       uint16_t length,
                                       uint8_t pipe_num);

USBH_StatusTypeDef USBH_BulkReceiveData(USBH_HandleTypeDef *phost,
                                        uint8_t *buff,
                                        uint16_t length,
                                        uint8_t pipe_num);

USBH_StatusTypeDef USBH_BulkSendData(USBH_HandleTypeDef *phost,
                                     uint8_t *buff,
                                     uint16_t length,
                                     uint8_t pipe_num,
                                     uint8_t do_ping);

USBH_StatusTypeDef USBH_InterruptReceiveData(USBH_HandleTypeDef *phost,
                                             uint8_t             *buff,
                                             uint8_t             length,
                                             uint8_t             pipe_num);

USBH_StatusTypeDef USBH_InterruptSendData(USBH_HandleTypeDef *phost,
                                          uint8_t *buff,
                                          uint8_t length,
                                          uint8_t pipe_num);


USBH_StatusTypeDef USBH_IsocReceiveData(USBH_HandleTypeDef *phost,
                                        uint8_t *buff,
                                        uint32_t length,
                                        uint8_t pipe_num);


USBH_StatusTypeDef USBH_IsocSendData(USBH_HandleTypeDef *phost,
                                     uint8_t *buff,
                                     uint32_t length,
                                     uint8_t pipe_num);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_IOREQ_H */

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


