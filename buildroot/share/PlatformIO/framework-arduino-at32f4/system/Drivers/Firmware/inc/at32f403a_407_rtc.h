/**
  **************************************************************************
  * @file     at32f403a_407_rtc.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 rtc header file
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
#ifndef __AT32F403A_407_RTC_H
#define __AT32F403A_407_RTC_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup RTC
  * @{
  */

/** @defgroup RTC_interrupts_definition
  * @brief rtc interrupt
  * @{
  */

#define RTC_TS_INT                       ((uint16_t)0x0001) /*!< rtc time second interrupt */
#define RTC_TA_INT                       ((uint16_t)0x0002) /*!< rtc time alarm interrupt */
#define RTC_OVF_INT                      ((uint16_t)0x0004) /*!< rtc overflow interrupt */

/**
  * @}
  */

/** @defgroup RTC_flags_definition
  * @brief rtc flag
  * @{
  */

#define RTC_TS_FLAG                      ((uint16_t)0x0001) /*!< rtc time second flag */
#define RTC_TA_FLAG                      ((uint16_t)0x0002) /*!< rtc time alarm flag */
#define RTC_OVF_FLAG                     ((uint16_t)0x0004) /*!< rtc overflow flag */
#define RTC_UPDF_FLAG                    ((uint16_t)0x0008) /*!< rtc update finish flag */
#define RTC_CFGF_FLAG                    ((uint16_t)0x0020) /*!< rtc configuration finish flag */

/**
  * @}
  */

/** @defgroup RTC_exported_types
  * @{
  */

/**
  * @brief type define rtc register all
  */
typedef struct
{

  /**
    * @brief rtc ctrlh register, offset:0x00
    */
  union
  {
    __IO uint32_t ctrlh;
    struct
    {
      __IO uint32_t tsien                : 1; /* [0] */
      __IO uint32_t taien                : 1; /* [1] */
      __IO uint32_t ovfien               : 1; /* [2] */
      __IO uint32_t reserved1            : 29;/* [31:3] */
    } ctrlh_bit;
  };

  /**
    * @brief rtc ctrll register, offset:0x04
    */
  union
  {
    __IO uint32_t ctrll;
    struct
    {
      __IO uint32_t tsf                  : 1; /* [0] */
      __IO uint32_t taf                  : 1; /* [1] */
      __IO uint32_t ovff                 : 1; /* [2] */
      __IO uint32_t updf                 : 1; /* [3] */
      __IO uint32_t cfgen                : 1; /* [4] */
      __IO uint32_t cfgf                 : 1; /* [5] */
      __IO uint32_t reserved1            : 26;/* [31:6] */
    } ctrll_bit;
  };

  /**
    * @brief rtc divh register, offset:0x08
    */
  union
  {
    __IO uint32_t divh;
    struct
    {
      __IO uint32_t div                  : 4; /* [3:0] */
      __IO uint32_t reserved1            : 28;/* [31:4] */
    } divh_bit;
  };

  /**
    * @brief rtc divl register, offset:0x0C
    */
  union
  {
    __IO uint32_t divl;
    struct
    {
      __IO uint32_t div                  : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } divl_bit;
  };

  /**
    * @brief rtc divcnth register, offset:0x10
    */
  union
  {
    __IO uint32_t divcnth;
    struct
    {
      __IO uint32_t divcnt               : 4; /* [3:0] */
      __IO uint32_t reserved1            : 28;/* [31:15] */
    } divcnth_bit;
  };

  /**
    * @brief rtc divcntl register, offset:0x14
    */
  union
  {
    __IO uint32_t divcntl;
    struct
    {
      __IO uint32_t divcnt               : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } divcntl_bit;
  };

  /**
    * @brief rtc cnth register, offset:0x18
    */
  union
  {
    __IO uint32_t cnth;
    struct
    {
      __IO uint32_t cnt                  : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } cnth_bit;
  };

  /**
    * @brief rtc cntl register, offset:0x1C
    */
  union
  {
    __IO uint32_t cntl;
    struct
    {
      __IO uint32_t cnt                  : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } cntl_bit;
  };

  /**
    * @brief rtc tah register, offset:0x20
    */
  union
  {
    __IO uint32_t tah;
    struct
    {
      __IO uint32_t ta                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } tah_bit;
  };

  /**
    * @brief rtc tal register, offset:0x24
    */
  union
  {
    __IO uint32_t tal;
    struct
    {
      __IO uint32_t ta                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } tal_bit;
  };

} rtc_type;

/**
  * @}
  */

#define RTC                              ((rtc_type *) RTC_BASE)

/** @defgroup RTC_exported_functions
  * @{
  */

void rtc_counter_set(uint32_t counter_value);
uint32_t rtc_counter_get(void);
void rtc_divider_set(uint32_t div_value);
uint32_t rtc_divider_get(void);
void rtc_alarm_set(uint32_t alarm_value);
void rtc_interrupt_enable(uint16_t source, confirm_state new_state);
flag_status rtc_flag_get(uint16_t flag);
void rtc_flag_clear(uint16_t flag);
void rtc_wait_config_finish(void);
void rtc_wait_update_finish(void);

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
