/**
  **************************************************************************
  * @file     at32f403a_407_debug.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 debug header file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32F403A_407_DEBUG_H
#define __AT32F403A_407_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup DEBUG
  * @{
  */

/** @defgroup DEBUG_mode_definition
  * @{
  */

#define DEBUG_SLEEP                      0x00000001 /*!< debug sleep mode */
#define DEBUG_DEEPSLEEP                  0x00000002 /*!< debug deepsleep mode */
#define DEBUG_STANDBY                    0x00000004 /*!< debug standby mode */
#define DEBUG_WDT_PAUSE                  0x00000100 /*!< debug watchdog timer pause */
#define DEBUG_WWDT_PAUSE                 0x00000200 /*!< debug window watchdog timer pause */
#define DEBUG_TMR1_PAUSE                 0x00000400 /*!< debug timer1 pause */
#define DEBUG_TMR3_PAUSE                 0x00001000 /*!< debug timer3 pause */
#define DEBUG_I2C1_SMBUS_TIMEOUT         0x00008000 /*!< debug i2c1 smbus timeout */
#define DEBUG_I2C2_SMBUS_TIMEOUT         0x00010000 /*!< debug i2c2 smbus timeout */
#define DEBUG_I2C3_SMBUS_TIMEOUT         0x80000000 /*!< debug i2c3 smbus timeout */
#define DEBUG_TMR2_PAUSE                 0x00000800 /*!< debug timer2 pause */
#define DEBUG_TMR4_PAUSE                 0x00002000 /*!< debug timer4 pause */
#define DEBUG_CAN1_PAUSE                 0x00004000 /*!< debug can1 pause */
#define DEBUG_TMR8_PAUSE                 0x00020000 /*!< debug timer8 pause */
#define DEBUG_TMR5_PAUSE                 0x00040000 /*!< debug timer5 pause */
#define DEBUG_TMR6_PAUSE                 0x00080000 /*!< debug timer6 pause */
#define DEBUG_TMR7_PAUSE                 0x00100000 /*!< debug timer7 pause */
#define DEBUG_CAN2_PAUSE                 0x00200000 /*!< debug can2 pause */
#define DEBUG_TMR12_PAUSE                0x02000000 /*!< debug timer12 pause */
#define DEBUG_TMR13_PAUSE                0x04000000 /*!< debug timer13 pause */
#define DEBUG_TMR14_PAUSE                0x08000000 /*!< debug timer14 pause */
#define DEBUG_TMR9_PAUSE                 0x10000000 /*!< debug timer9 pause */
#define DEBUG_TMR10_PAUSE                0x20000000 /*!< debug timer10 pause */
#define DEBUG_TMR11_PAUSE                0x40000000 /*!< debug timer11 pause */

/**
  * @}
  */

/** @defgroup DEBUG_exported_types
  * @{
  */

/**
  * @brief type define debug register all
  */
typedef struct
{
  /**
    * @brief debug idcode register, offset:0x00
    */
  union
  {
    __IO uint32_t pid;
    struct
    {
      __IO uint32_t pid                  : 32;/* [31:0] */
    } idcode_bit;
  };

  /**
    * @brief debug ctrl register, offset:0x04
    */
  union
  {
    __IO uint32_t ctrl;
    struct
    {
      __IO uint32_t sleep_debug          : 1;/* [0] */
      __IO uint32_t deepsleep_debug      : 1;/* [1] */
      __IO uint32_t standby_debug        : 1;/* [2] */
      __IO uint32_t reserved1            : 2;/* [4:3] */
      __IO uint32_t trace_ioen           : 1;/* [5] */
      __IO uint32_t trace_mode           : 2;/* [7:6] */
      __IO uint32_t wdt_pause            : 1;/* [8] */
      __IO uint32_t wwdt_pause           : 1;/* [9] */
      __IO uint32_t tmr1_pause           : 1;/* [10] */
      __IO uint32_t tmr2_pause           : 1;/* [11] */
      __IO uint32_t tmr3_pause           : 1;/* [12] */
      __IO uint32_t tmr4_pause           : 1;/* [13] */
      __IO uint32_t can1_pause           : 1;/* [14] */
      __IO uint32_t i2c1_smbus_timeout   : 1;/* [15] */
      __IO uint32_t i2c2_smbus_timeout   : 1;/* [16] */
      __IO uint32_t tmr8_pause           : 1;/* [17] */
      __IO uint32_t tmr5_pause           : 1;/* [18] */
      __IO uint32_t tmr6_pause           : 1;/* [19] */
      __IO uint32_t tmr7_pause           : 1;/* [20] */
      __IO uint32_t can2_pause           : 1;/* [21] */
      __IO uint32_t reserved2            : 3;/* [24:22] */
      __IO uint32_t tmr12_pause          : 1;/* [25] */
      __IO uint32_t tmr13_pause          : 1;/* [26] */
      __IO uint32_t tmr14_pause          : 1;/* [27] */
      __IO uint32_t tmr9_pause           : 1;/* [28] */
      __IO uint32_t tmr10_pause          : 1;/* [29] */
      __IO uint32_t tmr11_pause          : 1;/* [30] */
      __IO uint32_t i2c3_smbus_timeout   : 1;/* [31] */
    } ctrl_bit;
  };

} debug_type;

/**
  * @}
  */

#define DEBUGMCU                         ((debug_type *) DEBUG_BASE)

/** @defgroup DEBUG_exported_functions
  * @{
  */

uint32_t debug_device_id_get(void);
void debug_periph_mode_set(uint32_t periph_debug_mode, confirm_state new_state);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
