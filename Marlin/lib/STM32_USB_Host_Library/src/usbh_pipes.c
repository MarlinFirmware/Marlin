/**
  ******************************************************************************
  * @file    usbh_pipes.c
  * @author  MCD Application Team
  * @brief   This file implements functions for opening and closing Pipes
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

/* Includes ------------------------------------------------------------------*/
#include "usbh_pipes.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/

/** @defgroup USBH_PIPES
  * @brief This file includes opening and closing Pipes
  * @{
  */

/** @defgroup USBH_PIPES_Private_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_PIPES_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_PIPES_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_PIPES_Private_Variables
  * @{
  */

/**
  * @}
  */


/** @defgroup USBH_PIPES_Private_Functions
  * @{
  */
static uint16_t USBH_GetFreePipe(USBH_HandleTypeDef *phost);


/**
  * @brief  USBH_Open_Pipe
  *         Open a  pipe
  * @param  phost: Host Handle
  * @param  pipe_num: Pipe Number
  * @param  dev_address: USB Device address allocated to attached device
  * @param  speed : USB device speed (Full/Low)
  * @param  ep_type: end point type (Bulk/int/ctl)
  * @param  mps: max pkt size
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_OpenPipe(USBH_HandleTypeDef *phost, uint8_t pipe_num,
                                 uint8_t epnum, uint8_t dev_address,
                                 uint8_t speed, uint8_t ep_type, uint16_t mps)
{
  USBH_LL_OpenPipe(phost, pipe_num, epnum, dev_address, speed, ep_type, mps);

  return USBH_OK;
}


/**
  * @brief  USBH_ClosePipe
  *         Close a  pipe
  * @param  phost: Host Handle
  * @param  pipe_num: Pipe Number
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe_num)
{
  USBH_LL_ClosePipe(phost, pipe_num);

  return USBH_OK;
}


/**
  * @brief  USBH_Alloc_Pipe
  *         Allocate a new Pipe
  * @param  phost: Host Handle
  * @param  ep_addr: End point for which the Pipe to be allocated
  * @retval Pipe number
  */
uint8_t USBH_AllocPipe(USBH_HandleTypeDef *phost, uint8_t ep_addr)
{
  uint16_t pipe;

  pipe =  USBH_GetFreePipe(phost);

  if (pipe != 0xFFFFU)
  {
    phost->Pipes[pipe & 0xFU] = 0x8000U | ep_addr;
  }

  return (uint8_t)pipe;
}


/**
  * @brief  USBH_Free_Pipe
  *         Free the USB Pipe
  * @param  phost: Host Handle
  * @param  idx: Pipe number to be freed
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_FreePipe(USBH_HandleTypeDef *phost, uint8_t idx)
{
  if (idx < 11U)
  {
    phost->Pipes[idx] &= 0x7FFFU;
  }

  return USBH_OK;
}


/**
  * @brief  USBH_GetFreePipe
  * @param  phost: Host Handle
  *         Get a free Pipe number for allocation to a device endpoint
  * @retval idx: Free Pipe number
  */
static uint16_t USBH_GetFreePipe(USBH_HandleTypeDef *phost)
{
  uint8_t idx = 0U;

  for (idx = 0U ; idx < 11U ; idx++)
  {
    if ((phost->Pipes[idx] & 0x8000U) == 0U)
    {
      return (uint16_t)idx;
    }
  }

  return 0xFFFFU;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


