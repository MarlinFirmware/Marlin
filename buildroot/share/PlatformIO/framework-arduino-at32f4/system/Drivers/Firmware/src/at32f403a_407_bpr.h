/**
  **************************************************************************
  * @file     at32f403a_407_bpr.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 bpr header file
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32F403A_407_BPR_H
#define __AT32F403A_407_BPR_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup BPR
  * @{
  */

/** @defgroup BPR_flags_definition
  * @brief bpr flag
  * @{
  */

#define BPR_TAMPER_INTERRUPT_FLAG        ((uint32_t)0x00000001) /*!< bpr tamper interrupt flag */
#define BPR_TAMPER_EVENT_FLAG            ((uint32_t)0x00000002) /*!< bpr tamper event flag */

/**
  * @}
  */

/** @defgroup BPR_exported_types
  * @{
  */

/**
  * @brief battery powered register data type
  */
typedef enum
{
  BPR_DATA1                              = 0x04, /*!< bpr data register 1 */
  BPR_DATA2                              = 0x08, /*!< bpr data register 2 */
  BPR_DATA3                              = 0x0C, /*!< bpr data register 3 */
  BPR_DATA4                              = 0x10, /*!< bpr data register 4 */
  BPR_DATA5                              = 0x14, /*!< bpr data register 5 */
  BPR_DATA6                              = 0x18, /*!< bpr data register 6 */
  BPR_DATA7                              = 0x1C, /*!< bpr data register 7 */
  BPR_DATA8                              = 0x20, /*!< bpr data register 8 */
  BPR_DATA9                              = 0x24, /*!< bpr data register 9 */
  BPR_DATA10                             = 0x28, /*!< bpr data register 10 */
  BPR_DATA11                             = 0x40, /*!< bpr data register 11 */
  BPR_DATA12                             = 0x44, /*!< bpr data register 12 */
  BPR_DATA13                             = 0x48, /*!< bpr data register 13 */
  BPR_DATA14                             = 0x4C, /*!< bpr data register 14 */
  BPR_DATA15                             = 0x50, /*!< bpr data register 15 */
  BPR_DATA16                             = 0x54, /*!< bpr data register 16 */
  BPR_DATA17                             = 0x58, /*!< bpr data register 17 */
  BPR_DATA18                             = 0x5C, /*!< bpr data register 18 */
  BPR_DATA19                             = 0x60, /*!< bpr data register 19 */
  BPR_DATA20                             = 0x64, /*!< bpr data register 20 */
  BPR_DATA21                             = 0x68, /*!< bpr data register 21 */
  BPR_DATA22                             = 0x6C, /*!< bpr data register 22 */
  BPR_DATA23                             = 0x70, /*!< bpr data register 23 */
  BPR_DATA24                             = 0x74, /*!< bpr data register 24 */
  BPR_DATA25                             = 0x78, /*!< bpr data register 25 */
  BPR_DATA26                             = 0x7C, /*!< bpr data register 26 */
  BPR_DATA27                             = 0x80, /*!< bpr data register 27 */
  BPR_DATA28                             = 0x84, /*!< bpr data register 28 */
  BPR_DATA29                             = 0x88, /*!< bpr data register 29 */
  BPR_DATA30                             = 0x8C, /*!< bpr data register 30 */
  BPR_DATA31                             = 0x90, /*!< bpr data register 31 */
  BPR_DATA32                             = 0x94, /*!< bpr data register 32 */
  BPR_DATA33                             = 0x98, /*!< bpr data register 33 */
  BPR_DATA34                             = 0x9C, /*!< bpr data register 34 */
  BPR_DATA35                             = 0xA0, /*!< bpr data register 35 */
  BPR_DATA36                             = 0xA4, /*!< bpr data register 36 */
  BPR_DATA37                             = 0xA8, /*!< bpr data register 37 */
  BPR_DATA38                             = 0xAC, /*!< bpr data register 38 */
  BPR_DATA39                             = 0xB0, /*!< bpr data register 39 */
  BPR_DATA40                             = 0xB4, /*!< bpr data register 40 */
  BPR_DATA41                             = 0xB8, /*!< bpr data register 41 */
  BPR_DATA42                             = 0xBC  /*!< bpr data register 42 */
} bpr_data_type;

/**
  * @brief bpr rtc output type
  */
typedef enum
{
  BPR_RTC_OUTPUT_NONE                    = 0x000, /*!< output disable */
  BPR_RTC_OUTPUT_CLOCK_CAL_BEFORE        = 0x080, /*!< output clock before calibration */
  BPR_RTC_OUTPUT_ALARM                   = 0x100, /*!< output alarm event with pluse mode */
  BPR_RTC_OUTPUT_SECOND                  = 0x300, /*!< output second event with pluse mode */
  BPR_RTC_OUTPUT_CLOCK_CAL_AFTER         = 0x480, /*!< output clock after calibration */
  BPR_RTC_OUTPUT_ALARM_TOGGLE            = 0x900, /*!< output alarm event with toggle mode */
  BPR_RTC_OUTPUT_SECOND_TOGGLE           = 0xB00  /*!< output second event with toggle mode */
} bpr_rtc_output_type;

/**
  * @brief tamper pin active level type
  */
typedef enum
{
  BPR_TAMPER_PIN_ACTIVE_HIGH             = 0x00, /*!< tamper pin input active level is high */
  BPR_TAMPER_PIN_ACTIVE_LOW              = 0x01  /*!< tamper pin input active level is low */
} bpr_tamper_pin_active_level_type;

/**
  * @brief type define bpr register all
  */
typedef struct
{
  /**
    * @brief reserved, offset:0x00
    */
  __IO uint32_t reserved1;

  /**
    * @brief bpr dt1 register, offset:0x04
    */
  union
  {
    __IO uint32_t dt1;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt1_bit;
  };

  /**
    * @brief bpr dt2 register, offset:0x08
    */
  union
  {
    __IO uint32_t dt2;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt2_bit;
  };

  /**
    * @brief bpr dt3 register, offset:0x0C
    */
  union
  {
    __IO uint32_t dt3;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt3_bit;
  };

  /**
    * @brief bpr dt4 register, offset:0x10
    */
  union
  {
    __IO uint32_t dt4;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt4_bit;
  };

  /**
    * @brief bpr dt5 register, offset:0x14
    */
  union
  {
    __IO uint32_t dt5;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt5_bit;
  };

  /**
    * @brief bpr dt6 register, offset:0x18
    */
  union
  {
    __IO uint32_t dt6;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt6_bit;
  };

  /**
    * @brief bpr dt7 register, offset:0x1C
    */
  union
  {
    __IO uint32_t dt7;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt7_bit;
  };

  /**
    * @brief bpr dt8 register, offset:0x20
    */
  union
  {
    __IO uint32_t dt8;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt8_bit;
  };

  /**
    * @brief bpr dt9 register, offset:0x24
    */
  union
  {
    __IO uint32_t dt9;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt9_bit;
  };

  /**
    * @brief bpr dt10 register, offset:0x28
    */
  union
  {
    __IO uint32_t dt10;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt10_bit;
  };

  /**
    * @brief bpr rtccal register, offset:0x2C
    */
  union
  {
    __IO uint32_t rtccal;
    struct
    {
      __IO uint32_t calval               : 7; /* [6:0] */
      __IO uint32_t calout               : 1; /* [7] */
      __IO uint32_t outen                : 1; /* [8] */
      __IO uint32_t outsel               : 1; /* [9] */
      __IO uint32_t ccos                 : 1; /* [10] */
      __IO uint32_t outm                 : 1; /* [11] */
      __IO uint32_t reserved1            : 20;/* [31:12] */
    } rtccal_bit;
  };

  /**
    * @brief bpr ctrl register, offset:0x30
    */
  union
  {
    __IO uint32_t ctrl;
    struct
    {
      __IO uint32_t tpen                 : 1; /* [0] */
      __IO uint32_t tpp                  : 1; /* [1] */
      __IO uint32_t reserved1            : 30;/* [31:2] */
    } ctrl_bit;
  };

  /**
    * @brief bpr ctrlsts register, offset:0x34
    */
  union
  {
    __IO uint32_t ctrlsts;
    struct
    {
      __IO uint32_t tpefclr              : 1;/* [0] */
      __IO uint32_t tpifclr              : 1;/* [1] */
      __IO uint32_t tpien                : 1;/* [2] */
      __IO uint32_t reserved1            : 5;/* [7:3] */
      __IO uint32_t tpef                 : 1;/* [8] */
      __IO uint32_t tpif                 : 1;/* [9] */
      __IO uint32_t reserved2            : 22;/* [31:10] */
    } ctrlsts_bit;
  };

  /**
    * @brief reserved, offset:0x38
    */
  __IO uint32_t reserved2;

  /**
    * @brief reserved, offset:0x3C
    */
  __IO uint32_t reserved3;

  /**
    * @brief bpr dt11 register, offset:0x40
    */
  union
  {
    __IO uint32_t dt11;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt11_bit;
  };

  /**
    * @brief bpr dt12 register, offset:0x44
    */
  union
  {
    __IO uint32_t dt12;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt12_bit;
  };

  /**
    * @brief bpr dt13 register, offset:0x48
    */
  union
  {
    __IO uint32_t dt13;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt13_bit;
  };

  /**
    * @brief bpr dt14 register, offset:0x4C
    */
  union
  {
    __IO uint32_t dt14;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt14_bit;
  };

  /**
    * @brief bpr dt15 register, offset:0x50
    */
  union
  {
    __IO uint32_t dt15;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt15_bit;
  };

  /**
    * @brief bpr dt16 register, offset:0x54
    */
  union
  {
    __IO uint32_t dt16;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt16_bit;
  };

  /**
    * @brief bpr dt17 register, offset:0x58
    */
  union
  {
    __IO uint32_t dt17;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt17_bit;
  };

  /**
    * @brief bpr dt18 register, offset:0x5C
    */
  union
  {
    __IO uint32_t dt18;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt18_bit;
  };

  /**
    * @brief bpr dt19 register, offset:0x60
    */
  union
  {
    __IO uint32_t dt19;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt19_bit;
  };

  /**
    * @brief bpr dt20 register, offset:0x64
    */
  union
  {
    __IO uint32_t dt20;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt20_bit;
  };

  /**
    * @brief bpr dt21 register, offset:0x68
    */
  union
  {
    __IO uint32_t dt21;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt21_bit;
  };

  /**
    * @brief bpr dt22 register, offset:6C
    */
  union
  {
    __IO uint32_t dt22;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt22_bit;
  };

  /**
    * @brief bpr dt23 register, offset:0x70
    */
  union
  {
    __IO uint32_t dt23;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt23_bit;
  };

  /**
    * @brief bpr dt24 register, offset:0x74
    */
  union
  {
    __IO uint32_t dt24;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt24_bit;
  };

  /**
    * @brief bpr dt25 register, offset:0x78
    */
  union
  {
    __IO uint32_t dt25;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt25_bit;
  };

  /**
    * @brief bpr dt26 register, offset:0x7C
    */
  union
  {
    __IO uint32_t dt26;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt26_bit;
  };

  /**
    * @brief bpr dt27 register, offset:0x80
    */
  union
  {
    __IO uint32_t dt27;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt27_bit;
  };

  /**
    * @brief bpr dt28 register, offset:0x84
    */
  union
  {
    __IO uint32_t dt28;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt28_bit;
  };

  /**
    * @brief bpr dt29 register, offset:0x88
    */
  union
  {
    __IO uint32_t dt29;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt29_bit;
  };

  /**
    * @brief bpr dt30 register, offset:0x8C
    */
  union
  {
    __IO uint32_t dt30;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt30_bit;
  };

  /**
    * @brief bpr dt31 register, offset:0x90
    */
  union
  {
    __IO uint32_t dt31;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt31_bit;
  };

  /**
    * @brief bpr dt32 register, offset:0x94
    */
  union
  {
    __IO uint32_t dt32;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt32_bit;
  };

  /**
    * @brief bpr dt33 register, offset:0x98
    */
  union
  {
    __IO uint32_t dt33;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt33_bit;
  };

  /**
    * @brief bpr dt34 register, offset:0x9C
    */
  union
  {
    __IO uint32_t dt34;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt34_bit;
  };

  /**
    * @brief bpr dt35 register, offset:0xA0
    */
  union
  {
    __IO uint32_t dt35;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt35_bit;
  };

  /**
    * @brief bpr dt36 register, offset:0xA4
    */
  union
  {
    __IO uint32_t dt36;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt36_bit;
  };

  /**
    * @brief bpr dt37 register, offset:0xA8
    */
  union
  {
    __IO uint32_t dt37;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt37_bit;
  };

  /**
    * @brief bpr dt38 register, offset:0xAC
    */
  union
  {
    __IO uint32_t dt38;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt38_bit;
  };

  /**
    * @brief bpr dt39 register, offset:0xB0
    */
  union
  {
    __IO uint32_t dt39;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt39_bit;
  };

  /**
    * @brief bpr dt40 register, offset:0xB4
    */
  union
  {
    __IO uint32_t dt40;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt40_bit;
  };

  /**
    * @brief bpr dt41 register, offset:0xB8
    */
  union
  {
    __IO uint32_t dt41;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt41_bit;
  };

  /**
    * @brief bpr dt42 register, offset:0xBC
    */
  union
  {
    __IO uint32_t dt42;
    struct
    {
      __IO uint32_t dt                   : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:15] */
    } dt42_bit;
  };
} bpr_type;

/**
  * @}
  */

#define BPR                              ((bpr_type *) BPR_BASE)

/** @defgroup BPR_exported_functions
  * @{
  */

void bpr_reset(void);
flag_status bpr_flag_get(uint32_t flag);
void bpr_flag_clear(uint32_t flag);
void bpr_interrupt_enable(confirm_state new_state);
uint16_t bpr_data_read(bpr_data_type bpr_data);
void bpr_data_write(bpr_data_type bpr_data, uint16_t data_value);
void bpr_rtc_output_select(bpr_rtc_output_type output_source);
void bpr_rtc_clock_calibration_value_set(uint8_t calibration_value);
void bpr_tamper_pin_enable(confirm_state new_state);
void bpr_tamper_pin_active_level_set(bpr_tamper_pin_active_level_type active_level);

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
