/**
  ******************************************************************************
  * @file    usbh_ioreq.c
  * @author  MCD Application Team
  * @brief   This file handles the issuing of the USB transactions
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
#include "../Inc/usbh_ioreq.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/

/** @defgroup USBH_IOREQ
  * @brief This file handles the standard protocol processing (USB v2.0)
  * @{
  */


/** @defgroup USBH_IOREQ_Private_Defines
  * @{
  */

/**
  * @}
  */


/** @defgroup USBH_IOREQ_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */



/** @defgroup USBH_IOREQ_Private_Macros
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_IOREQ_Private_Variables
  * @{
  */
/**
  * @}
  */
/** @defgroup USBH_IOREQ_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */


/** @defgroup USBH_IOREQ_Private_Functions
  * @{
  */



/**
  * @brief  USBH_CtlSendSetup
  *         Sends the Setup Packet to the Device
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer from which the Data will be send to Device
  * @param  pipe_num: Pipe Number
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_CtlSendSetup(USBH_HandleTypeDef *phost,
                                     uint8_t *buff,
                                     uint8_t pipe_num)
{

  USBH_LL_SubmitURB(phost,                      /* Driver handle    */
                    pipe_num,             /* Pipe index       */
                    0U,                    /* Direction : OUT  */
                    USBH_EP_CONTROL,      /* EP type          */
                    USBH_PID_SETUP,       /* Type setup       */
                    buff,                 /* data buffer      */
                    USBH_SETUP_PKT_SIZE,  /* data length      */
                    0U);
  return USBH_OK;
}


/**
  * @brief  USBH_CtlSendData
  *         Sends a data Packet to the Device
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer from which the Data will be sent to Device
  * @param  length: Length of the data to be sent
  * @param  pipe_num: Pipe Number
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_CtlSendData(USBH_HandleTypeDef *phost,
                                    uint8_t *buff,
                                    uint16_t length,
                                    uint8_t pipe_num,
                                    uint8_t do_ping)
{
  if (phost->device.speed != USBH_SPEED_HIGH)
  {
    do_ping = 0U;
  }

  USBH_LL_SubmitURB(phost,                      /* Driver handle    */
                    pipe_num,             /* Pipe index       */
                    0U,                   /* Direction : OUT  */
                    USBH_EP_CONTROL,      /* EP type          */
                    USBH_PID_DATA,        /* Type Data        */
                    buff,                 /* data buffer      */
                    length,               /* data length      */
                    do_ping);             /* do ping (HS Only)*/

  return USBH_OK;
}


/**
  * @brief  USBH_CtlReceiveData
  *         Receives the Device Response to the Setup Packet
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer in which the response needs to be copied
  * @param  length: Length of the data to be received
  * @param  pipe_num: Pipe Number
  * @retval USBH Status.
  */
USBH_StatusTypeDef USBH_CtlReceiveData(USBH_HandleTypeDef *phost,
                                       uint8_t *buff,
                                       uint16_t length,
                                       uint8_t pipe_num)
{
  USBH_LL_SubmitURB(phost,                      /* Driver handle    */
                    pipe_num,             /* Pipe index       */
                    1U,                    /* Direction : IN   */
                    USBH_EP_CONTROL,      /* EP type          */
                    USBH_PID_DATA,        /* Type Data        */
                    buff,                 /* data buffer      */
                    length,               /* data length      */
                    0U);
  return USBH_OK;

}


/**
  * @brief  USBH_BulkSendData
  *         Sends the Bulk Packet to the device
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer from which the Data will be sent to Device
  * @param  length: Length of the data to be sent
  * @param  pipe_num: Pipe Number
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_BulkSendData(USBH_HandleTypeDef *phost,
                                     uint8_t *buff,
                                     uint16_t length,
                                     uint8_t pipe_num,
                                     uint8_t do_ping)
{
  if (phost->device.speed != USBH_SPEED_HIGH)
  {
    do_ping = 0U;
  }

  USBH_LL_SubmitURB(phost,                      /* Driver handle    */
                    pipe_num,             /* Pipe index       */
                    0U,                    /* Direction : IN   */
                    USBH_EP_BULK,         /* EP type          */
                    USBH_PID_DATA,        /* Type Data        */
                    buff,                 /* data buffer      */
                    length,               /* data length      */
                    do_ping);             /* do ping (HS Only)*/
  return USBH_OK;
}


/**
  * @brief  USBH_BulkReceiveData
  *         Receives IN bulk packet from device
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer in which the received data packet to be copied
  * @param  length: Length of the data to be received
  * @param  pipe_num: Pipe Number
  * @retval USBH Status.
  */
USBH_StatusTypeDef USBH_BulkReceiveData(USBH_HandleTypeDef *phost,
                                        uint8_t *buff,
                                        uint16_t length,
                                        uint8_t pipe_num)
{
  USBH_LL_SubmitURB(phost,                      /* Driver handle    */
                    pipe_num,             /* Pipe index       */
                    1U,                    /* Direction : IN   */
                    USBH_EP_BULK,         /* EP type          */
                    USBH_PID_DATA,        /* Type Data        */
                    buff,                 /* data buffer      */
                    length,               /* data length      */
                    0U);
  return USBH_OK;
}


/**
  * @brief  USBH_InterruptReceiveData
  *         Receives the Device Response to the Interrupt IN token
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer in which the response needs to be copied
  * @param  length: Length of the data to be received
  * @param  pipe_num: Pipe Number
  * @retval USBH Status.
  */
USBH_StatusTypeDef USBH_InterruptReceiveData(USBH_HandleTypeDef *phost,
                                             uint8_t *buff,
                                             uint8_t length,
                                             uint8_t pipe_num)
{
  USBH_LL_SubmitURB(phost,                      /* Driver handle    */
                    pipe_num,             /* Pipe index       */
                    1U,                   /* Direction : IN   */
                    USBH_EP_INTERRUPT,    /* EP type          */
                    USBH_PID_DATA,        /* Type Data        */
                    buff,                 /* data buffer      */
                    (uint16_t)length,     /* data length      */
                    0U);

  return USBH_OK;
}

/**
  * @brief  USBH_InterruptSendData
  *         Sends the data on Interrupt OUT Endpoint
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer from where the data needs to be copied
  * @param  length: Length of the data to be sent
  * @param  pipe_num: Pipe Number
  * @retval USBH Status.
  */
USBH_StatusTypeDef USBH_InterruptSendData(USBH_HandleTypeDef *phost,
                                          uint8_t *buff,
                                          uint8_t length,
                                          uint8_t pipe_num)
{
  USBH_LL_SubmitURB(phost,                      /* Driver handle    */
                    pipe_num,             /* Pipe index       */
                    0U,                   /* Direction : OUT   */
                    USBH_EP_INTERRUPT,    /* EP type          */
                    USBH_PID_DATA,        /* Type Data        */
                    buff,                 /* data buffer      */
                    (uint16_t)length,     /* data length      */
                    0U);

  return USBH_OK;
}

/**
  * @brief  USBH_IsocReceiveData
  *         Receives the Device Response to the Isochronous IN token
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer in which the response needs to be copied
  * @param  length: Length of the data to be received
  * @param  pipe_num: Pipe Number
  * @retval USBH Status.
  */
USBH_StatusTypeDef USBH_IsocReceiveData(USBH_HandleTypeDef *phost,
                                        uint8_t *buff,
                                        uint32_t length,
                                        uint8_t pipe_num)
{
  USBH_LL_SubmitURB(phost,                      /* Driver handle    */
                    pipe_num,             /* Pipe index       */
                    1U,                   /* Direction : IN   */
                    USBH_EP_ISO,          /* EP type          */
                    USBH_PID_DATA,        /* Type Data        */
                    buff,                 /* data buffer      */
                    (uint16_t)length,     /* data length      */
                    0U);


  return USBH_OK;
}

/**
  * @brief  USBH_IsocSendData
  *         Sends the data on Isochronous OUT Endpoint
  * @param  phost: Host Handle
  * @param  buff: Buffer pointer from where the data needs to be copied
  * @param  length: Length of the data to be sent
  * @param  pipe_num: Pipe Number
  * @retval USBH Status.
  */
USBH_StatusTypeDef USBH_IsocSendData(USBH_HandleTypeDef *phost,
                                     uint8_t *buff,
                                     uint32_t length,
                                     uint8_t pipe_num)
{
  USBH_LL_SubmitURB(phost,                      /* Driver handle    */
                    pipe_num,             /* Pipe index       */
                    0U,                   /* Direction : OUT   */
                    USBH_EP_ISO,          /* EP type          */
                    USBH_PID_DATA,        /* Type Data        */
                    buff,                 /* data buffer      */
                    (uint16_t)length,     /* data length      */
                    0U);

  return USBH_OK;
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



