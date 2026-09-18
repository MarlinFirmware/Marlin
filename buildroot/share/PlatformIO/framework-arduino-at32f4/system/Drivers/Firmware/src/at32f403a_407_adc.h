/**
  **************************************************************************
  * @file     at32f403a_407_adc.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 adc header file
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
#ifndef __AT32F403A_407_ADC_H
#define __AT32F403A_407_ADC_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */

/** @defgroup ADC_interrupts_definition
  * @brief adc interrupt
  * @{
  */

#define ADC_CCE_INT                      ((uint32_t)0x00000020) /*!< channels conversion end interrupt */
#define ADC_VMOR_INT                     ((uint32_t)0x00000040) /*!< voltage monitoring out of range interrupt */
#define ADC_PCCE_INT                     ((uint32_t)0x00000080) /*!< preempt channels conversion end interrupt */

/**
  * @}
  */

/** @defgroup ADC_flags_definition
  * @brief adc flag
  * @{
  */

#define ADC_VMOR_FLAG                    ((uint8_t)0x01) /*!< voltage monitoring out of range flag */
#define ADC_CCE_FLAG                     ((uint8_t)0x02) /*!< channels conversion end flag */
#define ADC_PCCE_FLAG                    ((uint8_t)0x04) /*!< preempt channels conversion end flag */
#define ADC_PCCS_FLAG                    ((uint8_t)0x08) /*!< preempt channel conversion start flag */
#define ADC_OCCS_FLAG                    ((uint8_t)0x10) /*!< ordinary channel conversion start flag */

/**
  * @}
  */

/** @defgroup ADC_exported_types
  * @{
  */

/**
  * @brief adc combine mode type(these options are reserved in adc2 and adc3)
  */
typedef enum
{
  ADC_INDEPENDENT_MODE                      = 0x00, /*!< independent mode */
  ADC_ORDINARY_SMLT_PREEMPT_SMLT_MODE       = 0x01, /*!< combined ordinary simultaneous + preempt simultaneous mode */
  ADC_ORDINARY_SMLT_PREEMPT_INTERLTRIG_MODE = 0x02, /*!< combined ordinary simultaneous + preempt interleaved trigger mode */
  ADC_ORDINARY_SHORTSHIFT_PREEMPT_SMLT_MODE = 0x03, /*!< combined ordinary short shifting + preempt simultaneous mode */
  ADC_ORDINARY_LONGSHIFT_PREEMPT_SMLT_MODE  = 0x04, /*!< combined ordinary long shifting + preempt simultaneous mode */
  ADC_PREEMPT_SMLT_ONLY_MODE                = 0x05, /*!< preempt simultaneous mode only */
  ADC_ORDINARY_SMLT_ONLY_MODE               = 0x06, /*!< ordinary simultaneous mode only */
  ADC_ORDINARY_SHORTSHIFT_ONLY_MODE         = 0x07, /*!< ordinary short shifting mode only */
  ADC_ORDINARY_LONGSHIFT_ONLY_MODE          = 0x08, /*!< slow interleaved mode only */
  ADC_PREEMPT_INTERLTRIG_ONLY_MODE          = 0x09  /*!< alternate trigger mode only */
} adc_combine_mode_type;

/**
  * @brief adc data align type
  */
typedef enum
{
  ADC_RIGHT_ALIGNMENT                    = 0x00, /*!< data right alignment */
  ADC_LEFT_ALIGNMENT                     = 0x01  /*!< data left alignment */
} adc_data_align_type;

/**
  * @brief adc channel select type
  */
typedef enum
{
  ADC_CHANNEL_0                          = 0x00, /*!< adc channel 0 */
  ADC_CHANNEL_1                          = 0x01, /*!< adc channel 1 */
  ADC_CHANNEL_2                          = 0x02, /*!< adc channel 2 */
  ADC_CHANNEL_3                          = 0x03, /*!< adc channel 3 */
  ADC_CHANNEL_4                          = 0x04, /*!< adc channel 4 */
  ADC_CHANNEL_5                          = 0x05, /*!< adc channel 5 */
  ADC_CHANNEL_6                          = 0x06, /*!< adc channel 6 */
  ADC_CHANNEL_7                          = 0x07, /*!< adc channel 7 */
  ADC_CHANNEL_8                          = 0x08, /*!< adc channel 8 */
  ADC_CHANNEL_9                          = 0x09, /*!< adc channel 9 */
  ADC_CHANNEL_10                         = 0x0A, /*!< adc channel 10 */
  ADC_CHANNEL_11                         = 0x0B, /*!< adc channel 11 */
  ADC_CHANNEL_12                         = 0x0C, /*!< adc channel 12 */
  ADC_CHANNEL_13                         = 0x0D, /*!< adc channel 13 */
  ADC_CHANNEL_14                         = 0x0E, /*!< adc channel 14 */
  ADC_CHANNEL_15                         = 0x0F, /*!< adc channel 15 */
  ADC_CHANNEL_16                         = 0x10, /*!< adc channel 16 */
  ADC_CHANNEL_17                         = 0x11  /*!< adc channel 17 */
} adc_channel_select_type;

/**
  * @brief adc sampletime select type
  */
typedef enum
{
  ADC_SAMPLETIME_1_5                     = 0x00, /*!< adc sample time 1.5 cycle */
  ADC_SAMPLETIME_7_5                     = 0x01, /*!< adc sample time 7.5 cycle */
  ADC_SAMPLETIME_13_5                    = 0x02, /*!< adc sample time 13.5 cycle */
  ADC_SAMPLETIME_28_5                    = 0x03, /*!< adc sample time 28.5 cycle */
  ADC_SAMPLETIME_41_5                    = 0x04, /*!< adc sample time 41.5 cycle */
  ADC_SAMPLETIME_55_5                    = 0x05, /*!< adc sample time 55.5 cycle */
  ADC_SAMPLETIME_71_5                    = 0x06, /*!< adc sample time 71.5 cycle */
  ADC_SAMPLETIME_239_5                   = 0x07  /*!< adc sample time 239.5 cycle */
} adc_sampletime_select_type;

/**
  * @brief adc ordinary group trigger event select type
  */
typedef enum
{
  /*adc1 and adc2 ordinary trigger event*/
  ADC12_ORDINARY_TRIG_TMR1CH1            = 0x00, /*!< timer1 ch1 event as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_TMR1CH2            = 0x01, /*!< timer1 ch2 event as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_TMR1CH3            = 0x02, /*!< timer1 ch3 event as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_TMR2CH2            = 0x03, /*!< timer2 ch2 event as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_TMR3TRGOUT         = 0x04, /*!< timer3 trgout event as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_TMR4CH4            = 0x05, /*!< timer4 ch4 event as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_EXINT11_TMR8TRGOUT = 0x06, /*!< exint line11/timer8 trgout event as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_SOFTWARE           = 0x07, /*!< software(OCSWTRG) control bit as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_TMR1TRGOUT         = 0x0D, /*!< timer1 trgout event as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_TMR8CH1            = 0x0E, /*!< timer8 ch1 event as trigger source of adc1/adc2 ordinary sequence */
  ADC12_ORDINARY_TRIG_TMR8CH2            = 0x0F, /*!< timer8 ch2 event as trigger source of adc1/adc2 ordinary sequence */
  /*adc3 ordinary trigger event*/
  ADC3_ORDINARY_TRIG_TMR3CH1             = 0x00, /*!< timer3 ch1 event as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_TMR2CH3             = 0x01, /*!< timer2 ch3 event as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_TMR1CH3             = 0x02, /*!< timer1 ch3 event as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_TMR8CH1             = 0x03, /*!< timer8 ch1 event as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_TMR8TRGOUT          = 0x04, /*!< timer8 trgout event as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_TMR5CH1             = 0x05, /*!< timer5 ch1 event as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_TMR5CH3             = 0x06, /*!< timer5 ch3 event as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_SOFTWARE            = 0x07, /*!< software(OCSWTRG) control bit as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_TMR1TRGOUT          = 0x0D, /*!< timer1 trgout event as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_TMR1CH1             = 0x0E, /*!< timer1 ch1 event as trigger source of adc3 ordinary sequence */
  ADC3_ORDINARY_TRIG_TMR8CH3             = 0x0F  /*!< timer8 ch3 event as trigger source of adc3 ordinary sequence */
} adc_ordinary_trig_select_type;

/**
  * @brief adc preempt group trigger event select type
  */
typedef enum
{
  /*adc1 and adc2 preempt trigger event*/
  ADC12_PREEMPT_TRIG_TMR1TRGOUT          = 0x00, /*!< timer1 trgout event as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_TMR1CH4             = 0x01, /*!< timer1 ch4 event as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_TMR2TRGOUT          = 0x02, /*!< timer2 trgout event as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_TMR2CH1             = 0x03, /*!< timer2 ch1 event as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_TMR3CH4             = 0x04, /*!< timer3 ch4 event as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_TMR4TRGOUT          = 0x05, /*!< timer4 trgout event as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_EXINT15_TMR8CH4     = 0x06, /*!< exint line15/timer8 ch4 event as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_SOFTWARE            = 0x07, /*!< software(PCSWTRG) control bit as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_TMR1CH1             = 0x0D, /*!< timer1 ch1 event as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_TMR8CH1             = 0x0E, /*!< timer8 ch1 event as trigger source of adc1/adc2 preempt sequence */
  ADC12_PREEMPT_TRIG_TMR8TRGOUT          = 0x0F, /*!< timer8 trgout event as trigger source of adc1/adc2 preempt sequence */
  /*adc3 preempt trigger event*/
  ADC3_PREEMPT_TRIG_TMR1TRGOUT           = 0x00, /*!< timer1 trgout event as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_TMR1CH4              = 0x01, /*!< timer1 ch4 event as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_TMR4CH3              = 0x02, /*!< timer4 ch3 event as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_TMR8CH2              = 0x03, /*!< timer8 ch2 event as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_TMR8CH4              = 0x04, /*!< timer8 ch4 event as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_TMR5TRGOUT           = 0x05, /*!< timer5 trgout event as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_TMR5CH4              = 0x06, /*!< timer5 ch4 event as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_SOFTWARE             = 0x07, /*!< software(PCSWTRG) control bit as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_TMR1CH1              = 0x0D, /*!< timer1 ch1 event as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_TMR1CH2              = 0x0E, /*!< timer1 ch2 event as trigger source of adc3 preempt sequence */
  ADC3_PREEMPT_TRIG_TMR8TRGOUT           = 0x0F  /*!< timer8 trgout event as trigger source of adc3 preempt sequence */
} adc_preempt_trig_select_type;

/**
  * @brief adc preempt channel type
  */
typedef enum
{
  ADC_PREEMPT_CHANNEL_1                  = 0x00, /*!< adc preempt channel 1 */
  ADC_PREEMPT_CHANNEL_2                  = 0x01, /*!< adc preempt channel 2 */
  ADC_PREEMPT_CHANNEL_3                  = 0x02, /*!< adc preempt channel 3 */
  ADC_PREEMPT_CHANNEL_4                  = 0x03  /*!< adc preempt channel 4 */
} adc_preempt_channel_type;

/**
  * @brief adc voltage_monitoring type
  */
typedef enum
{
  ADC_VMONITOR_SINGLE_ORDINARY           = 0x00800200, /*!< voltage_monitoring on a single ordinary channel */
  ADC_VMONITOR_SINGLE_PREEMPT            = 0x00400200, /*!< voltage_monitoring on a single preempt channel */
  ADC_VMONITOR_SINGLE_ORDINARY_PREEMPT   = 0x00C00200, /*!< voltage_monitoring on a single ordinary or preempt channel */
  ADC_VMONITOR_ALL_ORDINARY              = 0x00800000, /*!< voltage_monitoring on all ordinary channel */
  ADC_VMONITOR_ALL_PREEMPT               = 0x00400000, /*!< voltage_monitoring on all preempt channel */
  ADC_VMONITOR_ALL_ORDINARY_PREEMPT      = 0x00C00000, /*!< voltage_monitoring on all ordinary and preempt channel */
  ADC_VMONITOR_NONE                      = 0x00000000  /*!< no channel guarded by the voltage_monitoring */
} adc_voltage_monitoring_type;

/**
  * @brief adc base config type
  */
typedef struct
{
  confirm_state                          sequence_mode;           /*!< adc sequence mode */
  confirm_state                          repeat_mode;             /*!< adc repeat mode */
  adc_data_align_type                    data_align;              /*!< adc data alignment */
  uint8_t                                ordinary_channel_length; /*!< adc ordinary channel sequence length*/
} adc_base_config_type;

/**
  * @brief type define adc register all
  */
typedef struct
{

  /**
    * @brief adc sts register, offset:0x00
    */
  union
  {
    __IO uint32_t sts;
    struct
    {
      __IO uint32_t vmor                 : 1; /* [0] */
      __IO uint32_t cce                  : 1; /* [1] */
      __IO uint32_t pcce                 : 1; /* [2] */
      __IO uint32_t pccs                 : 1; /* [3] */
      __IO uint32_t occs                 : 1; /* [4] */
      __IO uint32_t reserved1            : 27;/* [31:5] */
    } sts_bit;
  };

  /**
    * @brief adc ctrl1 register, offset:0x04
    */
  union
  {
    __IO uint32_t ctrl1;
    struct
    {
      __IO uint32_t vmcsel               : 5; /* [4:0] */
      __IO uint32_t cceien               : 1; /* [5] */
      __IO uint32_t vmorien              : 1; /* [6] */
      __IO uint32_t pcceien              : 1; /* [7] */
      __IO uint32_t sqen                 : 1; /* [8] */
      __IO uint32_t vmsgen               : 1; /* [9] */
      __IO uint32_t pcautoen             : 1; /* [10] */
      __IO uint32_t ocpen                : 1; /* [11] */
      __IO uint32_t pcpen                : 1; /* [12] */
      __IO uint32_t ocpcnt               : 3; /* [15:13] */
      __IO uint32_t mssel                : 4; /* [19:16] */
      __IO uint32_t reserved1            : 2; /* [21:20] */
      __IO uint32_t pcvmen               : 1; /* [22] */
      __IO uint32_t ocvmen               : 1; /* [23] */
      __IO uint32_t reserved2            : 8; /* [31:24] */
    } ctrl1_bit;
  };

   /**
    * @brief adc ctrl2 register, offset:0x08
    */
  union
  {
    __IO uint32_t ctrl2;
    struct
    {
      __IO uint32_t adcen                : 1; /* [0] */
      __IO uint32_t rpen                 : 1; /* [1] */
      __IO uint32_t adcal                : 1; /* [2] */
      __IO uint32_t adcalinit            : 1; /* [3] */
      __IO uint32_t reserved1            : 4; /* [7:4] */
      __IO uint32_t ocdmaen              : 1; /* [8] */
      __IO uint32_t reserved2            : 2; /* [10:9] */
      __IO uint32_t dtalign              : 1; /* [11] */
      __IO uint32_t pctesel_l            : 3; /* [14:12] */
      __IO uint32_t pcten                : 1; /* [15] */
      __IO uint32_t reserved3            : 1; /* [16] */
      __IO uint32_t octesel_l            : 3; /* [19:17] */
      __IO uint32_t octen                : 1; /* [20] */
      __IO uint32_t pcswtrg              : 1; /* [21] */
      __IO uint32_t ocswtrg              : 1; /* [22] */
      __IO uint32_t itsrven              : 1; /* [23] */
      __IO uint32_t pctesel_h            : 1; /* [24] */
      __IO uint32_t octesel_h            : 1; /* [25] */
      __IO uint32_t reserved4            : 6; /* [31:26] */
    } ctrl2_bit;
  };

  /**
  * @brief adc spt1 register, offset:0x0C
  */
  union
  {
    __IO uint32_t spt1;
    struct
    {
      __IO uint32_t cspt10               : 3; /* [2:0] */
      __IO uint32_t cspt11               : 3; /* [5:3] */
      __IO uint32_t cspt12               : 3; /* [8:6] */
      __IO uint32_t cspt13               : 3; /* [11:9] */
      __IO uint32_t cspt14               : 3; /* [14:12] */
      __IO uint32_t cspt15               : 3; /* [17:15] */
      __IO uint32_t cspt16               : 3; /* [20:18] */
      __IO uint32_t cspt17               : 3; /* [23:21] */
      __IO uint32_t reserved1            : 8;/* [31:24] */
    } spt1_bit;
  };

  /**
  * @brief adc spt2 register, offset:0x10
  */
  union
  {
    __IO uint32_t spt2;
    struct
    {
      __IO uint32_t cspt0                : 3;/* [2:0] */
      __IO uint32_t cspt1                : 3;/* [5:3] */
      __IO uint32_t cspt2                : 3;/* [8:6] */
      __IO uint32_t cspt3                : 3;/* [11:9] */
      __IO uint32_t cspt4                : 3;/* [14:12] */
      __IO uint32_t cspt5                : 3;/* [17:15] */
      __IO uint32_t cspt6                : 3;/* [20:18] */
      __IO uint32_t cspt7                : 3;/* [23:21] */
      __IO uint32_t cspt8                : 3;/* [26:24] */
      __IO uint32_t cspt9                : 3;/* [29:27] */
      __IO uint32_t reserved1            : 2;/* [31:30] */
    } spt2_bit;
  };

  /**
  * @brief adc pcdto1 register, offset:0x14
  */
  union
  {
    __IO uint32_t pcdto1;
    struct
    {
      __IO uint32_t pcdto1               : 12; /* [11:0] */
      __IO uint32_t reserved1            : 20; /* [31:12] */
    } pcdto1_bit;
  };

  /**
  * @brief adc pcdto2 register, offset:0x18
  */
  union
  {
    __IO uint32_t pcdto2;
    struct
    {
      __IO uint32_t pcdto2               : 12; /* [11:0] */
      __IO uint32_t reserved1            : 20; /* [31:12] */
    } pcdto2_bit;
  };

  /**
  * @brief adc pcdto3 register, offset:0x1C
  */
  union
  {
    __IO uint32_t pcdto3;
    struct
    {
      __IO uint32_t pcdto3               : 12; /* [11:0] */
      __IO uint32_t reserved1            : 20; /* [31:12] */
    } pcdto3_bit;
  };

  /**
  * @brief adc pcdto4 register, offset:0x20
  */
  union
  {
    __IO uint32_t pcdto4;
    struct
    {
      __IO uint32_t pcdto4               : 12; /* [11:0] */
      __IO uint32_t reserved1            : 20; /* [31:12] */
    } pcdto4_bit;
  };

  /**
  * @brief adc vmhb register, offset:0x24
  */
  union
  {
    __IO uint32_t vmhb;
    struct
    {
      __IO uint32_t vmhb                 : 12; /* [11:0] */
      __IO uint32_t reserved1            : 20; /* [31:12] */
    } vmhb_bit;
  };

  /**
  * @brief adc vmlb register, offset:0x28
  */
  union
  {
    __IO uint32_t vmlb;
    struct
    {
      __IO uint32_t vmlb                 : 12; /* [11:0] */
      __IO uint32_t reserved1            : 20; /* [31:12] */
    } vmlb_bit;
  };

  /**
  * @brief adc osq1 register, offset:0x2C
  */
  union
  {
    __IO uint32_t osq1;
    struct
    {
      __IO uint32_t osn13                : 5; /* [4:0] */
      __IO uint32_t osn14                : 5; /* [9:5] */
      __IO uint32_t osn15                : 5; /* [14:10] */
      __IO uint32_t osn16                : 5; /* [19:15] */
      __IO uint32_t oclen                : 4; /* [23:20] */
      __IO uint32_t reserved1            : 8; /* [31:24] */
    } osq1_bit;
  };

  /**
  * @brief adc osq2 register, offset:0x30
  */
  union
  {
    __IO uint32_t osq2;
    struct
    {
      __IO uint32_t osn7                 : 5; /* [4:0] */
      __IO uint32_t osn8                 : 5; /* [9:5] */
      __IO uint32_t osn9                 : 5; /* [14:10] */
      __IO uint32_t osn10                : 5; /* [19:15] */
      __IO uint32_t osn11                : 5; /* [24:20] */
      __IO uint32_t osn12                : 5; /* [29:25] */
      __IO uint32_t reserved1            : 2; /* [31:30] */
    } osq2_bit;
  };

  /**
  * @brief adc osq3 register, offset:0x34
  */
  union
  {
    __IO uint32_t osq3;
    struct
    {
      __IO uint32_t osn1                 : 5; /* [4:0] */
      __IO uint32_t osn2                 : 5; /* [9:5] */
      __IO uint32_t osn3                 : 5; /* [14:10] */
      __IO uint32_t osn4                 : 5; /* [19:15] */
      __IO uint32_t osn5                 : 5; /* [24:20] */
      __IO uint32_t osn6                 : 5; /* [29:25] */
      __IO uint32_t reserved1            : 2; /* [31:30] */
    } osq3_bit;
  };

  /**
  * @brief adc psq register, offset:0x38
  */
  union
  {
    __IO uint32_t psq;
    struct
    {
      __IO uint32_t psn1                 : 5; /* [4:0] */
      __IO uint32_t psn2                 : 5; /* [9:5] */
      __IO uint32_t psn3                 : 5; /* [14:10] */
      __IO uint32_t psn4                 : 5; /* [19:15] */
      __IO uint32_t pclen                : 2; /* [21:20] */
      __IO uint32_t reserved1            : 10;/* [31:22] */
    } psq_bit;
  };

  /**
  * @brief adc pdt1 register, offset:0x3C
  */
  union
  {
    __IO uint32_t pdt1;
    struct
    {
      __IO uint32_t pdt1                 : 16; /* [15:0] */
      __IO uint32_t reserved1            : 16; /* [31:16] */
    } pdt1_bit;
  };

  /**
  * @brief adc pdt2 register, offset:0x40
  */
  union
  {
    __IO uint32_t pdt2;
    struct
    {
      __IO uint32_t pdt2                 : 16; /* [15:0] */
      __IO uint32_t reserved1            : 16; /* [31:16] */
    } pdt2_bit;
  };

  /**
  * @brief adc pdt3 register, offset:0x44
  */
  union
  {
    __IO uint32_t pdt3;
    struct
    {
      __IO uint32_t pdt3                 : 16; /* [15:0] */
      __IO uint32_t reserved1            : 16; /* [31:16] */
    } pdt3_bit;
  };

  /**
  * @brief adc pdt4 register, offset:0x48
  */
  union
  {
    __IO uint32_t pdt4;
    struct
    {
      __IO uint32_t pdt4                 : 16; /* [15:0] */
      __IO uint32_t reserved1            : 16; /* [31:16] */
    } pdt4_bit;
  };

  /**
  * @brief adc odt register, offset:0x4C
  */
  union
  {
    __IO uint32_t odt;
    struct
    {
      __IO uint32_t odt                  : 16; /* [15:0] */
      __IO uint32_t adc2odt              : 16; /* [31:16] */
    } odt_bit;
  };

} adc_type;

/**
  * @}
  */

#define ADC1                             ((adc_type *) ADC1_BASE)
#define ADC2                             ((adc_type *) ADC2_BASE)
#define ADC3                             ((adc_type *) ADC3_BASE)

/** @defgroup ADC_exported_functions
  * @{
  */

void adc_reset(adc_type *adc_x);
void adc_enable(adc_type *adc_x, confirm_state new_state);
void adc_combine_mode_select(adc_combine_mode_type combine_mode);
void adc_base_default_para_init(adc_base_config_type *adc_base_struct);
void adc_base_config(adc_type *adc_x, adc_base_config_type *adc_base_struct);
void adc_dma_mode_enable(adc_type *adc_x, confirm_state new_state);
void adc_interrupt_enable(adc_type *adc_x, uint32_t adc_int, confirm_state new_state);
void adc_calibration_init(adc_type *adc_x);
flag_status adc_calibration_init_status_get(adc_type *adc_x);
void adc_calibration_start(adc_type *adc_x);
flag_status adc_calibration_status_get(adc_type *adc_x);
void adc_voltage_monitor_enable(adc_type *adc_x, adc_voltage_monitoring_type adc_voltage_monitoring);
void adc_voltage_monitor_threshold_value_set(adc_type *adc_x, uint16_t adc_high_threshold, uint16_t adc_low_threshold);
void adc_voltage_monitor_single_channel_select(adc_type *adc_x, adc_channel_select_type adc_channel);
void adc_ordinary_channel_set(adc_type *adc_x, adc_channel_select_type adc_channel, uint8_t adc_sequence, adc_sampletime_select_type adc_sampletime);
void adc_preempt_channel_length_set(adc_type *adc_x, uint8_t adc_channel_lenght);
void adc_preempt_channel_set(adc_type *adc_x, adc_channel_select_type adc_channel, uint8_t adc_sequence, adc_sampletime_select_type adc_sampletime);
void adc_ordinary_conversion_trigger_set(adc_type *adc_x, adc_ordinary_trig_select_type adc_ordinary_trig, confirm_state new_state);
void adc_preempt_conversion_trigger_set(adc_type *adc_x, adc_preempt_trig_select_type adc_preempt_trig, confirm_state new_state);
void adc_preempt_offset_value_set(adc_type *adc_x, adc_preempt_channel_type adc_preempt_channel, uint16_t adc_offset_value);
void adc_ordinary_part_count_set(adc_type *adc_x, uint8_t adc_channel_count);
void adc_ordinary_part_mode_enable(adc_type *adc_x, confirm_state new_state);
void adc_preempt_part_mode_enable(adc_type *adc_x, confirm_state new_state);
void adc_preempt_auto_mode_enable(adc_type *adc_x, confirm_state new_state);
void adc_tempersensor_vintrv_enable(confirm_state new_state);
void adc_ordinary_software_trigger_enable(adc_type *adc_x, confirm_state new_state);
flag_status adc_ordinary_software_trigger_status_get(adc_type *adc_x);
void adc_preempt_software_trigger_enable(adc_type *adc_x, confirm_state new_state);
flag_status adc_preempt_software_trigger_status_get(adc_type *adc_x);
uint16_t adc_ordinary_conversion_data_get(adc_type *adc_x);
uint32_t adc_combine_ordinary_conversion_data_get(void);
uint16_t adc_preempt_conversion_data_get(adc_type *adc_x, adc_preempt_channel_type adc_preempt_channel);
flag_status adc_flag_get(adc_type *adc_x, uint8_t adc_flag);
void adc_flag_clear(adc_type *adc_x, uint32_t adc_flag);

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
