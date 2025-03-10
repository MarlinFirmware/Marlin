/**
  ******************************************************************************
  * @file    usbh_conf_template.h
  * @author  MCD Application Team
  * @brief   Header file for usbh_conf_template.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_USBH_CONF_H
#define __STM32_USBH_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"  /* replace 'stm32xxx' with your HAL driver header filename, ex: stm32f4xx.h */
#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @addtogroup STM32_USB_HOST_LIBRARY
  * @{
  */

/** @defgroup USBH_CONF
  * @brief usb host low level driver configuration file
  * @{
  */

/** @defgroup USBH_CONF_Exported_Defines
  * @{
  */

#define USBH_MAX_NUM_ENDPOINTS                2U
#define USBH_MAX_NUM_INTERFACES               2U
#define USBH_MAX_NUM_CONFIGURATION            1U
#define USBH_KEEP_CFG_DESCRIPTOR              1U
#define USBH_MAX_NUM_SUPPORTED_CLASS          1U
#define USBH_MAX_SIZE_CONFIGURATION           0x200U
#define USBH_MAX_DATA_BUFFER                  0x200U
#define USBH_DEBUG_LEVEL                      0U
#define USBH_USE_OS                           0U


/****************************************/
/* #define for FS and HS identification */
#define HOST_HS 		0
#define HOST_FS 		1

/** @defgroup USBH_Exported_Macros
  * @{
  */

/* Memory management macros */
#define USBH_malloc               malloc
#define USBH_free                 free
#define USBH_memset               memset
#define USBH_memcpy               memcpy

/* DEBUG macros */
#if (USBH_DEBUG_LEVEL > 0U)
#define  USBH_UsrLog(...)   do { \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBH_UsrLog(...) do {} while (0)
#endif

#if (USBH_DEBUG_LEVEL > 1U)

#define  USBH_ErrLog(...) do { \
                            printf("ERROR: ") ; \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBH_ErrLog(...) do {} while (0)
#endif

#if (USBH_DEBUG_LEVEL > 2U)
#define  USBH_DbgLog(...)   do { \
                            printf("DEBUG : ") ; \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBH_DbgLog(...) do {} while (0)
#endif

/**
  * @}
  */



/**
  * @}
  */


/** @defgroup USBH_CONF_Exported_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_CONF_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_CONF_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_CONF_Exported_FunctionsPrototype
  * @{
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_CONF_TEMPLATE_H */


/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
