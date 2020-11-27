/**
  ******************************************************************************
  * @file    usbh_pipes.h
  * @author  MCD Application Team
  * @brief   Header file for usbh_pipes.c
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
#ifndef __USBH_PIPES_H
#define __USBH_PIPES_H

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

/** @defgroup USBH_PIPES
  * @brief This file is the header file for usbh_pipes.c
  * @{
  */

/** @defgroup USBH_PIPES_Exported_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_PIPES_Exported_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_PIPES_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_PIPES_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_PIPES_Exported_FunctionsPrototype
  * @{
  */

USBH_StatusTypeDef USBH_OpenPipe(USBH_HandleTypeDef *phost,
                                 uint8_t pipe_num,
                                 uint8_t epnum,
                                 uint8_t dev_address,
                                 uint8_t speed,
                                 uint8_t ep_type,
                                 uint16_t mps);

USBH_StatusTypeDef USBH_ClosePipe(USBH_HandleTypeDef *phost,
                                  uint8_t pipe_num);

uint8_t USBH_AllocPipe(USBH_HandleTypeDef *phost,
                       uint8_t ep_addr);

USBH_StatusTypeDef USBH_FreePipe(USBH_HandleTypeDef *phost,
                                 uint8_t idx);




/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* __USBH_PIPES_H */


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


