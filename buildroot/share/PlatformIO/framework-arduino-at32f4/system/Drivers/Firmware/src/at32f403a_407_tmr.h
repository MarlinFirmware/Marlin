/**
  **************************************************************************
  * @file     at32f403a_407_tmr.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 tmr header file
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
#ifndef __AT32F403A_407_TMR_H
#define __AT32F403A_407_TMR_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup TMR
  * @{
  */

/** @defgroup TMR_flags_definition
  * @brief tmr flag
  * @{
  */

#define TMR_OVF_FLAG                     ((uint32_t)0x000001) /*!< tmr flag overflow */
#define TMR_C1_FLAG                      ((uint32_t)0x000002) /*!< tmr flag channel 1 */
#define TMR_C2_FLAG                      ((uint32_t)0x000004) /*!< tmr flag channel 2 */
#define TMR_C3_FLAG                      ((uint32_t)0x000008) /*!< tmr flag channel 3 */
#define TMR_C4_FLAG                      ((uint32_t)0x000010) /*!< tmr flag channel 4 */
#define TMR_HALL_FLAG                    ((uint32_t)0x000020) /*!< tmr flag hall */
#define TMR_TRIGGER_FLAG                 ((uint32_t)0x000040) /*!< tmr flag trigger */
#define TMR_BRK_FLAG                     ((uint32_t)0x000080) /*!< tmr flag brake */
#define TMR_C1_RECAPTURE_FLAG            ((uint32_t)0x000200) /*!< tmr flag channel 1 recapture */
#define TMR_C2_RECAPTURE_FLAG            ((uint32_t)0x000400) /*!< tmr flag channel 2 recapture */
#define TMR_C3_RECAPTURE_FLAG            ((uint32_t)0x000800) /*!< tmr flag channel 3 recapture */
#define TMR_C4_RECAPTURE_FLAG            ((uint32_t)0x001000) /*!< tmr flag channel 4 recapture */

/**
  * @}
  */

/** @defgroup TMR_interrupt_select_type_definition
  * @brief tmr interrupt select type
  * @{
  */

#define TMR_OVF_INT                      ((uint32_t)0x000001) /*!< tmr interrupt overflow */
#define TMR_C1_INT                       ((uint32_t)0x000002) /*!< tmr interrupt channel 1 */
#define TMR_C2_INT                       ((uint32_t)0x000004) /*!< tmr interrupt channel 2 */
#define TMR_C3_INT                       ((uint32_t)0x000008) /*!< tmr interrupt channel 3 */
#define TMR_C4_INT                       ((uint32_t)0x000010) /*!< tmr interrupt channel 4 */
#define TMR_HALL_INT                     ((uint32_t)0x000020) /*!< tmr interrupt hall */
#define TMR_TRIGGER_INT                  ((uint32_t)0x000040) /*!< tmr interrupt trigger */
#define TMR_BRK_INT                      ((uint32_t)0x000080) /*!< tmr interrupt brake */

/**
  * @}
  */

/** @defgroup TMR_exported_types
  * @{
  */

/**
  * @brief tmr clock division type
  */
typedef enum
{
  TMR_CLOCK_DIV1                         = 0x00, /*!< tmr clock division 1 */
  TMR_CLOCK_DIV2                         = 0x01, /*!< tmr clock division 2 */
  TMR_CLOCK_DIV4                         = 0x02  /*!< tmr clock division 4 */
} tmr_clock_division_type;

/**
  * @brief tmr counter mode type
  */
typedef enum
{
  TMR_COUNT_UP                           = 0x00, /*!< tmr counter mode up */
  TMR_COUNT_DOWN                         = 0x01, /*!< tmr counter mode down */
  TMR_COUNT_TWO_WAY_1                    = 0x02, /*!< tmr counter mode two way 1 */
  TMR_COUNT_TWO_WAY_2                    = 0x04, /*!< tmr counter mode two way 2 */
  TMR_COUNT_TWO_WAY_3                    = 0x06  /*!< tmr counter mode two way 3 */
} tmr_count_mode_type;

/**
  * @brief tmr primary mode select type
  */
typedef enum
{
  TMR_PRIMARY_SEL_RESET                  = 0x00, /*!< tmr primary mode select reset */
  TMR_PRIMARY_SEL_ENABLE                 = 0x01, /*!< tmr primary mode select enable */
  TMR_PRIMARY_SEL_OVERFLOW               = 0x02, /*!< tmr primary mode select overflow */
  TMR_PRIMARY_SEL_COMPARE                = 0x03, /*!< tmr primary mode select compare */
  TMR_PRIMARY_SEL_C1ORAW                 = 0x04, /*!< tmr primary mode select c1oraw */
  TMR_PRIMARY_SEL_C2ORAW                 = 0x05, /*!< tmr primary mode select c2oraw */
  TMR_PRIMARY_SEL_C3ORAW                 = 0x06, /*!< tmr primary mode select c3oraw */
  TMR_PRIMARY_SEL_C4ORAW                 = 0x07  /*!< tmr primary mode select c4oraw */
} tmr_primary_select_type;

/**
  * @brief tmr subordinate mode input select type
  */
typedef enum
{
  TMR_SUB_INPUT_SEL_IS0                  = 0x00, /*!< subordinate mode input select is0 */
  TMR_SUB_INPUT_SEL_IS1                  = 0x01, /*!< subordinate mode input select is1 */
  TMR_SUB_INPUT_SEL_IS2                  = 0x02, /*!< subordinate mode input select is2 */
  TMR_SUB_INPUT_SEL_IS3                  = 0x03, /*!< subordinate mode input select is3 */
  TMR_SUB_INPUT_SEL_C1INC                = 0x04, /*!< subordinate mode input select c1inc */
  TMR_SUB_INPUT_SEL_C1DF1                = 0x05, /*!< subordinate mode input select c1df1 */
  TMR_SUB_INPUT_SEL_C2DF2                = 0x06, /*!< subordinate mode input select c2df2 */
  TMR_SUB_INPUT_SEL_EXTIN                = 0x07  /*!< subordinate mode input select extin */
} sub_tmr_input_sel_type;

/**
  * @brief tmr subordinate mode select type
  */
typedef enum
{
  TMR_SUB_MODE_DIABLE                    = 0x00, /*!< subordinate mode disable */
  TMR_SUB_ENCODER_MODE_A                 = 0x01, /*!< subordinate mode select encoder mode a */
  TMR_SUB_ENCODER_MODE_B                 = 0x02, /*!< subordinate mode select encoder mode b */
  TMR_SUB_ENCODER_MODE_C                 = 0x03, /*!< subordinate mode select encoder mode c */
  TMR_SUB_RESET_MODE                     = 0x04, /*!< subordinate mode select reset */
  TMR_SUB_HANG_MODE                      = 0x05, /*!< subordinate mode select hang */
  TMR_SUB_TRIGGER_MODE                   = 0x06, /*!< subordinate mode select trigger */
  TMR_SUB_EXTERNAL_CLOCK_MODE_A          = 0x07  /*!< subordinate mode external clock mode a */
} tmr_sub_mode_select_type;

/**
  * @brief tmr encoder mode type
  */
typedef enum
{
  TMR_ENCODER_MODE_A                     = TMR_SUB_ENCODER_MODE_A, /*!< tmr encoder mode a */
  TMR_ENCODER_MODE_B                     = TMR_SUB_ENCODER_MODE_B, /*!< tmr encoder mode b */
  TMR_ENCODER_MODE_C                     = TMR_SUB_ENCODER_MODE_C  /*!< tmr encoder mode c */
} tmr_encoder_mode_type;

/**
  * @brief tmr output control mode type
  */
typedef enum
{
  TMR_OUTPUT_CONTROL_OFF                 = 0x00, /*!< tmr output control mode off */
  TMR_OUTPUT_CONTROL_HIGH                = 0x01, /*!< tmr output control mode high */
  TMR_OUTPUT_CONTROL_LOW                 = 0x02, /*!< tmr output control mode low */
  TMR_OUTPUT_CONTROL_SWITCH              = 0x03, /*!< tmr output control mode switch */
  TMR_OUTPUT_CONTROL_FORCE_LOW           = 0x04, /*!< tmr output control mode force low */
  TMR_OUTPUT_CONTROL_FORCE_HIGH          = 0x05, /*!< tmr output control mode force high */
  TMR_OUTPUT_CONTROL_PWM_MODE_A          = 0x06, /*!< tmr output control mode pwm a */
  TMR_OUTPUT_CONTROL_PWM_MODE_B          = 0x07  /*!< tmr output control mode pwm b */
} tmr_output_control_mode_type;

/**
  * @brief tmr force output type
  */
typedef enum
{
  TMR_FORCE_OUTPUT_HIGH                  = TMR_OUTPUT_CONTROL_FORCE_HIGH, /*!< tmr force output high */
  TMR_FORCE_OUTPUT_LOW                   = TMR_OUTPUT_CONTROL_FORCE_LOW   /*!< tmr force output low */
} tmr_force_output_type;

/**
  * @brief tmr output channel polarity type
  */
typedef enum
{
  TMR_OUTPUT_ACTIVE_HIGH                 = 0x00, /*!< tmr output channel polarity high */
  TMR_OUTPUT_ACTIVE_LOW                  = 0x01  /*!< tmr output channel polarity low */
} tmr_output_polarity_type;

/**
  * @brief tmr input channel polarity type
  */
typedef enum
{
  TMR_INPUT_RISING_EDGE                  = 0x00, /*!< tmr input channel polarity rising */
  TMR_INPUT_FALLING_EDGE                 = 0x01, /*!< tmr input channel polarity falling */
  TMR_INPUT_BOTH_EDGE                    = 0x03  /*!< tmr input channel polarity both edge */
} tmr_input_polarity_type;

/**
  * @brief tmr channel select type
  */
typedef enum
{
  TMR_SELECT_CHANNEL_1                   = 0x00, /*!< tmr channel select channel 1 */
  TMR_SELECT_CHANNEL_1C                  = 0x01, /*!< tmr channel select channel 1 complementary */
  TMR_SELECT_CHANNEL_2                   = 0x02, /*!< tmr channel select channel 2 */
  TMR_SELECT_CHANNEL_2C                  = 0x03, /*!< tmr channel select channel 2 complementary */
  TMR_SELECT_CHANNEL_3                   = 0x04, /*!< tmr channel select channel 3 */
  TMR_SELECT_CHANNEL_3C                  = 0x05, /*!< tmr channel select channel 3 complementary */
  TMR_SELECT_CHANNEL_4                   = 0x06  /*!< tmr channel select channel 4 */
} tmr_channel_select_type;

/**
  * @brief tmr channel1 input connected type
  */
typedef enum
{
  TMR_CHANEL1_CONNECTED_C1IRAW           = 0x00, /*!< channel1 pins is only connected to C1IRAW input */
  TMR_CHANEL1_2_3_CONNECTED_C1IRAW_XOR   = 0x01  /*!< channel1/2/3 pins are connected to C1IRAW input after xored */
} tmr_channel1_input_connected_type;

/**
  * @brief tmr input channel mapped type channel direction
  */
typedef enum
{
  TMR_CC_CHANNEL_MAPPED_DIRECT           = 0x01, /*!< channel is configured as input, mapped direct */
  TMR_CC_CHANNEL_MAPPED_INDIRECT         = 0x02, /*!< channel is configured as input, mapped indirect */
  TMR_CC_CHANNEL_MAPPED_STI              = 0x03  /*!< channel is configured as input, mapped trc */
} tmr_input_direction_mapped_type;

/**
  * @brief tmr input divider type
  */
typedef enum
{
  TMR_CHANNEL_INPUT_DIV_1                = 0x00, /*!< tmr channel input divider 1 */
  TMR_CHANNEL_INPUT_DIV_2                = 0x01, /*!< tmr channel input divider 2 */
  TMR_CHANNEL_INPUT_DIV_4                = 0x02, /*!< tmr channel input divider 4 */
  TMR_CHANNEL_INPUT_DIV_8                = 0x03  /*!< tmr channel input divider 8 */
} tmr_channel_input_divider_type;

/**
  * @brief tmr dma request source select type
  */
typedef enum
{
  TMR_DMA_REQUEST_BY_CHANNEL             = 0x00, /*!< tmr dma request source select channel */
  TMR_DMA_REQUEST_BY_OVERFLOW            = 0x01  /*!< tmr dma request source select overflow */
} tmr_dma_request_source_type;

/**
  * @brief tmr dma request type
  */
typedef enum
{
  TMR_OVERFLOW_DMA_REQUEST               = 0x00000100, /*!< tmr dma request select overflow */
  TMR_C1_DMA_REQUEST                     = 0x00000200, /*!< tmr dma request select channel 1 */
  TMR_C2_DMA_REQUEST                     = 0x00000400, /*!< tmr dma request select channel 2 */
  TMR_C3_DMA_REQUEST                     = 0x00000800, /*!< tmr dma request select channel 3 */
  TMR_C4_DMA_REQUEST                     = 0x00001000, /*!< tmr dma request select channel 4 */
  TMR_HALL_DMA_REQUEST                   = 0x00002000, /*!< tmr dma request select hall */
  TMR_TRIGGER_DMA_REQUEST                = 0x00004000  /*!< tmr dma request select trigger */
} tmr_dma_request_type;

/**
  * @brief tmr event triggered by software type
  */
typedef enum
{
  TMR_OVERFLOW_SWTRIG                    = 0x00000001, /*!< tmr event triggered by software of overflow */
  TMR_C1_SWTRIG                          = 0x00000002, /*!< tmr event triggered by software of channel 1 */
  TMR_C2_SWTRIG                          = 0x00000004, /*!< tmr event triggered by software of channel 2 */
  TMR_C3_SWTRIG                          = 0x00000008, /*!< tmr event triggered by software of channel 3 */
  TMR_C4_SWTRIG                          = 0x00000010, /*!< tmr event triggered by software of channel 4 */
  TMR_HALL_SWTRIG                        = 0x00000020, /*!< tmr event triggered by software of hall */
  TMR_TRIGGER_SWTRIG                     = 0x00000040, /*!< tmr event triggered by software of trigger */
  TMR_BRK_SWTRIG                         = 0x00000080  /*!< tmr event triggered by software of brake */
}tmr_event_trigger_type;

/**
  * @brief tmr polarity active type
  */
typedef enum
{
  TMR_POLARITY_ACTIVE_HIGH               = 0x00, /*!< tmr polarity active high */
  TMR_POLARITY_ACTIVE_LOW                = 0x01, /*!< tmr polarity active low */
  TMR_POLARITY_ACTIVE_BOTH               = 0x02  /*!< tmr polarity active both high ande low */
}tmr_polarity_active_type;

/**
  * @brief tmr external signal divider type
  */
typedef enum
{
  TMR_ES_FREQUENCY_DIV_1                 = 0x00, /*!< tmr external signal frequency divider 1 */
  TMR_ES_FREQUENCY_DIV_2                 = 0x01, /*!< tmr external signal frequency divider 2 */
  TMR_ES_FREQUENCY_DIV_4                 = 0x02, /*!< tmr external signal frequency divider 4 */
  TMR_ES_FREQUENCY_DIV_8                 = 0x03  /*!< tmr external signal frequency divider 8 */
}tmr_external_signal_divider_type;

/**
  * @brief tmr external signal polarity type
  */
typedef enum
{
  TMR_ES_POLARITY_NON_INVERTED           = 0x00, /*!< tmr external signal polarity non-inerted */
  TMR_ES_POLARITY_INVERTED               = 0x01  /*!< tmr external signal polarity inerted */
}tmr_external_signal_polarity_type;

/**
  * @brief tmr dma transfer length type
  */
typedef enum
{
  TMR_DMA_TRANSFER_1BYTE                 = 0x00, /*!< tmr dma transfer length 1 byte */
  TMR_DMA_TRANSFER_2BYTES                = 0x01, /*!< tmr dma transfer length 2 bytes */
  TMR_DMA_TRANSFER_3BYTES                = 0x02, /*!< tmr dma transfer length 3 bytes */
  TMR_DMA_TRANSFER_4BYTES                = 0x03, /*!< tmr dma transfer length 4 bytes */
  TMR_DMA_TRANSFER_5BYTES                = 0x04, /*!< tmr dma transfer length 5 bytes */
  TMR_DMA_TRANSFER_6BYTES                = 0x05, /*!< tmr dma transfer length 6 bytes */
  TMR_DMA_TRANSFER_7BYTES                = 0x06, /*!< tmr dma transfer length 7 bytes */
  TMR_DMA_TRANSFER_8BYTES                = 0x07, /*!< tmr dma transfer length 8 bytes */
  TMR_DMA_TRANSFER_9BYTES                = 0x08, /*!< tmr dma transfer length 9 bytes */
  TMR_DMA_TRANSFER_10BYTES               = 0x09, /*!< tmr dma transfer length 10 bytes */
  TMR_DMA_TRANSFER_11BYTES               = 0x0A, /*!< tmr dma transfer length 11 bytes */
  TMR_DMA_TRANSFER_12BYTES               = 0x0B, /*!< tmr dma transfer length 12 bytes */
  TMR_DMA_TRANSFER_13BYTES               = 0x0C, /*!< tmr dma transfer length 13 bytes */
  TMR_DMA_TRANSFER_14BYTES               = 0x0D, /*!< tmr dma transfer length 14 bytes */
  TMR_DMA_TRANSFER_15BYTES               = 0x0E, /*!< tmr dma transfer length 15 bytes */
  TMR_DMA_TRANSFER_16BYTES               = 0x0F, /*!< tmr dma transfer length 16 bytes */
  TMR_DMA_TRANSFER_17BYTES               = 0x10, /*!< tmr dma transfer length 17 bytes */
  TMR_DMA_TRANSFER_18BYTES               = 0x11  /*!< tmr dma transfer length 18 bytes */
}tmr_dma_transfer_length_type;

/**
  * @brief tmr dma base address type
  */
typedef enum
{
  TMR_CTRL1_ADDRESS                      = 0x0000, /*!< tmr dma base address ctrl1 */
  TMR_CTRL2_ADDRESS                      = 0x0001, /*!< tmr dma base address ctrl2 */
  TMR_STCTRL_ADDRESS                     = 0x0002, /*!< tmr dma base address stctrl */
  TMR_IDEN_ADDRESS                       = 0x0003, /*!< tmr dma base address iden */
  TMR_ISTS_ADDRESS                       = 0x0004, /*!< tmr dma base address ists */
  TMR_SWEVT_ADDRESS                      = 0x0005, /*!< tmr dma base address swevt */
  TMR_CM1_ADDRESS                        = 0x0006, /*!< tmr dma base address cm1 */
  TMR_CM2_ADDRESS                        = 0x0007, /*!< tmr dma base address cm2 */
  TMR_CCTRL_ADDRESS                      = 0x0008, /*!< tmr dma base address cctrl */
  TMR_CVAL_ADDRESS                       = 0x0009, /*!< tmr dma base address cval */
  TMR_DIV_ADDRESS                        = 0x000A, /*!< tmr dma base address div */
  TMR_PR_ADDRESS                         = 0x000B, /*!< tmr dma base address pr */
  TMR_RPR_ADDRESS                        = 0x000C, /*!< tmr dma base address rpr */
  TMR_C1DT_ADDRESS                       = 0x000D, /*!< tmr dma base address c1dt */
  TMR_C2DT_ADDRESS                       = 0x000E, /*!< tmr dma base address c2dt */
  TMR_C3DT_ADDRESS                       = 0x000F, /*!< tmr dma base address c3dt */
  TMR_C4DT_ADDRESS                       = 0x0010, /*!< tmr dma base address c4dt */
  TMR_BRK_ADDRESS                        = 0x0011, /*!< tmr dma base address brake */
  TMR_DMACTRL_ADDRESS                    = 0x0012  /*!< tmr dma base address dmactrl */
}tmr_dma_address_type;

/**
  * @brief tmr brk polarity type
  */
typedef enum
{
  TMR_BRK_INPUT_ACTIVE_LOW               = 0x00, /*!< tmr brk input channel active low */
  TMR_BRK_INPUT_ACTIVE_HIGH              = 0x01  /*!< tmr brk input channel active high */
}tmr_brk_polarity_type;

/**
  * @brief tmr write protect level type
  */
typedef enum
{
  TMR_WP_OFF                             = 0x00, /*!< tmr write protect off */
  TMR_WP_LEVEL_3                         = 0x01, /*!< tmr write protect level 3 */
  TMR_WP_LEVEL_2                         = 0x02, /*!< tmr write protect level 2 */
  TMR_WP_LEVEL_1                         = 0x03  /*!< tmr write protect level 1 */
}tmr_wp_level_type;

/**
  * @brief tmr output config type
  */
typedef struct
{
  tmr_output_control_mode_type           oc_mode;             /*!< output channel mode */
  confirm_state                          oc_idle_state;       /*!< output channel idle state */
  confirm_state                          occ_idle_state;      /*!< output channel complementary idle state */
  tmr_output_polarity_type               oc_polarity;         /*!< output channel polarity */
  tmr_output_polarity_type               occ_polarity;        /*!< output channel complementary polarity */
  confirm_state                          oc_output_state;     /*!< output channel enable */
  confirm_state                          occ_output_state;    /*!< output channel complementary enable */
} tmr_output_config_type;

/**
  * @brief tmr input capture config type
  */
typedef struct
{
  tmr_channel_select_type                input_channel_select;   /*!< tmr input channel select */
  tmr_input_polarity_type                input_polarity_select;  /*!< tmr input polarity select */
  tmr_input_direction_mapped_type        input_mapped_select;    /*!< tmr channel mapped direct or indirect */
  uint8_t                                input_filter_value;     /*!< tmr channel filter value */
} tmr_input_config_type;

/**
  * @brief tmr brkdt config type
  */
typedef struct
{
  uint8_t                                deadtime;            /*!< dead-time generator setup */
  tmr_brk_polarity_type                  brk_polarity;        /*!< tmr brake polarity */
  tmr_wp_level_type                      wp_level;            /*!< write protect configuration */
  confirm_state                          auto_output_enable;  /*!< automatic output enable */
  confirm_state                          fcsoen_state;        /*!< frozen channel status when output enable */
  confirm_state                          fcsodis_state;       /*!< frozen channel status when output disable */
  confirm_state                          brk_enable;          /*!< tmr brk enale */
} tmr_brkdt_config_type;

/**
  * @brief type define tmr register all
  */
typedef struct
{
  /**
    * @brief tmr ctrl1 register, offset:0x00
    */
  union
  {
    __IO uint32_t ctrl1;
    struct
    {
      __IO uint32_t tmren                : 1; /* [0] */
      __IO uint32_t ovfen                : 1; /* [1] */
      __IO uint32_t ovfs                 : 1; /* [2] */
      __IO uint32_t ocmen                : 1; /* [3] */
      __IO uint32_t cnt_dir              : 3; /* [6:4] */
      __IO uint32_t prben                : 1; /* [7] */
      __IO uint32_t clkdiv               : 2; /* [9:8] */
      __IO uint32_t pmen                 : 1; /* [10] */
      __IO uint32_t reserved1            : 21;/* [31:11] */
    } ctrl1_bit;
  };

  /**
    * @brief tmr ctrl2 register, offset:0x04
    */
  union
  {
    __IO uint32_t ctrl2;
    struct
    {
      __IO uint32_t cbctrl               : 1; /* [0] */
      __IO uint32_t reserved1            : 1; /* [1] */
      __IO uint32_t ccfs                 : 1; /* [2] */
      __IO uint32_t drs                  : 1; /* [3] */
      __IO uint32_t ptos                 : 3; /* [6:4] */
      __IO uint32_t c1insel              : 1; /* [7] */
      __IO uint32_t c1ios                : 1; /* [8] */
      __IO uint32_t c1cios               : 1; /* [9] */
      __IO uint32_t c2ios                : 1; /* [10] */
      __IO uint32_t c2cios               : 1; /* [11] */
      __IO uint32_t c3ios                : 1; /* [12] */
      __IO uint32_t c3cios               : 1; /* [13] */
      __IO uint32_t c4ios                : 1; /* [14] */
      __IO uint32_t reserved2            : 17;/* [31:15] */
    } ctrl2_bit;
  };

  /**
    * @brief tmr smc register, offset:0x08
    */
  union
  {
    __IO uint32_t stctrl;
    struct
    {
      __IO uint32_t smsel                : 3; /* [2:0] */
      __IO uint32_t reserved1            : 1; /* [3] */
      __IO uint32_t stis                 : 3; /* [6:4] */
      __IO uint32_t sts                  : 1; /* [7] */
      __IO uint32_t esf                  : 4; /* [11:8] */
      __IO uint32_t esdiv                : 2; /* [13:12] */
      __IO uint32_t ecmben               : 1; /* [14] */
      __IO uint32_t esp                  : 1; /* [15] */
      __IO uint32_t reserved2            : 16;/* [31:16] */
    } stctrl_bit;
  };

  /**
    * @brief tmr die register, offset:0x0C
    */
  union
  {
    __IO uint32_t iden;
    struct
    {
      __IO uint32_t ovfien               : 1; /* [0] */
      __IO uint32_t c1ien                : 1; /* [1] */
      __IO uint32_t c2ien                : 1; /* [2] */
      __IO uint32_t c3ien                : 1; /* [3] */
      __IO uint32_t c4ien                : 1; /* [4] */
      __IO uint32_t hallien              : 1; /* [5] */
      __IO uint32_t tien                 : 1; /* [6] */
      __IO uint32_t brkie                : 1; /* [7] */
      __IO uint32_t ovfden               : 1; /* [8] */
      __IO uint32_t c1den                : 1; /* [9] */
      __IO uint32_t c2den                : 1; /* [10] */
      __IO uint32_t c3den                : 1; /* [11] */
      __IO uint32_t c4den                : 1; /* [12] */
      __IO uint32_t hallde               : 1; /* [13] */
      __IO uint32_t tden                 : 1; /* [14] */
      __IO uint32_t reserved1            : 17;/* [31:15] */
    } iden_bit;
  };

  /**
    * @brief tmr ists register, offset:0x10
    */
  union
  {
    __IO uint32_t ists;
    struct
    {
      __IO uint32_t ovfif                : 1; /* [0] */
      __IO uint32_t c1if                 : 1; /* [1] */
      __IO uint32_t c2if                 : 1; /* [2] */
      __IO uint32_t c3if                 : 1; /* [3] */
      __IO uint32_t c4if                 : 1; /* [4] */
      __IO uint32_t hallif               : 1; /* [5] */
      __IO uint32_t trgif                : 1; /* [6] */
      __IO uint32_t brkif                : 1; /* [7] */
      __IO uint32_t reserved1            : 1; /* [8] */
      __IO uint32_t c1rf                 : 1; /* [9] */
      __IO uint32_t c2rf                 : 1; /* [10] */
      __IO uint32_t c3rf                 : 1; /* [11] */
      __IO uint32_t c4rf                 : 1; /* [12] */
      __IO uint32_t reserved2            : 19;/* [31:13] */
    } ists_bit;
  };

  /**
    * @brief tmr eveg register, offset:0x14
    */
  union
  {
    __IO uint32_t swevt;
    struct
    {
      __IO uint32_t  ovfswtr             : 1; /* [0] */
      __IO uint32_t  c1swtr              : 1; /* [1] */
      __IO uint32_t  c2swtr              : 1; /* [2] */
      __IO uint32_t  c3swtr              : 1; /* [3] */
      __IO uint32_t  c4swtr              : 1; /* [4] */
      __IO uint32_t  hallswtr            : 1; /* [5] */
      __IO uint32_t  trgswtr             : 1; /* [6] */
      __IO uint32_t  brkswtr             : 1; /* [7] */
      __IO uint32_t  reserved            : 24;/* [31:8] */
    } swevt_bit;
  };

  /**
    * @brief tmr ccm1 register, offset:0x18
    */
  union
  {
    __IO uint32_t cm1;

    /**
     * @brief channel mode
     */
    struct
    {
      __IO uint32_t c1c                  : 2; /* [1:0] */
      __IO uint32_t c1oien               : 1; /* [2] */
      __IO uint32_t c1oben               : 1; /* [3] */
      __IO uint32_t c1octrl              : 3; /* [6:4] */
      __IO uint32_t c1osen               : 1; /* [7] */
      __IO uint32_t c2c                  : 2; /* [9:8] */
      __IO uint32_t c2oien               : 1; /* [10] */
      __IO uint32_t c2oben               : 1; /* [11] */
      __IO uint32_t c2octrl              : 3; /* [14:12] */
      __IO uint32_t c2osen               : 1; /* [15] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } cm1_output_bit;

    /**
      * @brief input capture mode
      */
    struct
    {
      __IO uint32_t c1c                  : 2; /* [1:0] */
      __IO uint32_t c1idiv               : 2; /* [3:2] */
      __IO uint32_t c1df                 : 4; /* [7:4] */
      __IO uint32_t c2c                  : 2; /* [9:8] */
      __IO uint32_t c2idiv               : 2; /* [11:10] */
      __IO uint32_t c2df                 : 4; /* [15:12] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } cm1_input_bit;
  };

  /**
    * @brief tmr ccm2 register, offset:0x1C
    */
  union
  {
    __IO uint32_t cm2;

    /**
      * @brief channel mode
      */
    struct
    {
      __IO uint32_t c3c                  : 2; /* [1:0] */
      __IO uint32_t c3oien               : 1; /* [2] */
      __IO uint32_t c3oben               : 1; /* [3] */
      __IO uint32_t c3octrl              : 3; /* [6:4] */
      __IO uint32_t c3osen               : 1; /* [7] */
      __IO uint32_t c4c                  : 2; /* [9:8] */
      __IO uint32_t c4oien               : 1; /* [10] */
      __IO uint32_t c4oben               : 1; /* [11] */
      __IO uint32_t c4octrl              : 3; /* [14:12] */
      __IO uint32_t c4osen               : 1; /* [15] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } cm2_output_bit;

    /**
      * @brief input capture mode
      */
    struct
    {
      __IO uint32_t c3c                  : 2; /* [1:0] */
      __IO uint32_t c3idiv               : 2; /* [3:2] */
      __IO uint32_t c3df                 : 4; /* [7:4] */
      __IO uint32_t c4c                  : 2; /* [9:8] */
      __IO uint32_t c4idiv               : 2; /* [11:10] */
      __IO uint32_t c4df                 : 4; /* [15:12] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } cm2_input_bit;
  };

  /**
    * @brief tmr cce register, offset:0x20
    */
  union
  {
    uint32_t cctrl;
    struct
    {
      __IO uint32_t c1en                 : 1; /* [0] */
      __IO uint32_t c1p                  : 1; /* [1] */
      __IO uint32_t c1cen                : 1; /* [2] */
      __IO uint32_t c1cp                 : 1; /* [3] */
      __IO uint32_t c2en                 : 1; /* [4] */
      __IO uint32_t c2p                  : 1; /* [5] */
      __IO uint32_t c2cen                : 1; /* [6] */
      __IO uint32_t c2cp                 : 1; /* [7] */
      __IO uint32_t c3en                 : 1; /* [8] */
      __IO uint32_t c3p                  : 1; /* [9] */
      __IO uint32_t c3cen                : 1; /* [10] */
      __IO uint32_t c3cp                 : 1; /* [11] */
      __IO uint32_t c4en                 : 1; /* [12] */
      __IO uint32_t c4p                  : 1; /* [13] */
      __IO uint32_t reserved1            : 18;/* [31:14] */
    } cctrl_bit;
  };

  /**
    * @brief tmr cnt register, offset:0x24
    */
  union
  {
    __IO uint32_t cval;
    struct
    {
      __IO uint32_t cval                 : 32;/* [31:0] */
    } cval_bit;
  };

  /**
    * @brief tmr div, offset:0x28
    */
  union
  {
    __IO uint32_t div;
    struct
    {
      __IO uint32_t div                  : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } div_bit;
  };

  /**
    * @brief tmr pr register, offset:0x2C
    */
  union
  {
    __IO  uint32_t pr;
    struct
    {
      __IO uint32_t pr                   : 32;/* [31:0] */
    } pr_bit;
  };

  /**
    * @brief tmr rpr register, offset:0x30
    */
  union
  {
    __IO uint32_t rpr;
    struct
    {
      __IO uint32_t rpr                  : 8; /* [7:0] */
      __IO uint32_t reserved1            : 24;/* [31:8] */
    } rpr_bit;
  };

  /**
    * @brief tmr c1dt register, offset:0x34
    */
  union
  {
    uint32_t c1dt;
    struct
    {
      __IO uint32_t c1dt                 : 32;/* [31:0] */
    } c1dt_bit;
  };

  /**
    * @brief tmr c2dt register, offset:0x38
    */
  union
  {
    uint32_t c2dt;
    struct
    {
      __IO uint32_t c2dt                 : 32;/* [31:0] */
    } c2dt_bit;
  };

  /**
    * @brief tmr c3dt register, offset:0x3C
    */
  union
  {
    __IO uint32_t c3dt;
    struct
    {
      __IO uint32_t c3dt                 : 32;/* [31:0] */
    } c3dt_bit;
  };

  /**
    * @brief tmr c4dt register, offset:0x40
    */
  union
  {
    __IO uint32_t c4dt;
    struct
    {
      __IO uint32_t c4dt                 : 32;/* [31:0] */
    } c4dt_bit;
  };

  /**
    * @brief tmr brk register, offset:0x44
    */
  union
  {
    __IO uint32_t brk;
    struct
    {
      __IO uint32_t dtc                  : 8; /* [7:0] */
      __IO uint32_t wpc                  : 2; /* [9:8] */
      __IO uint32_t fcsodis              : 1; /* [10] */
      __IO uint32_t fcsoen               : 1; /* [11] */
      __IO uint32_t brken                : 1; /* [12] */
      __IO uint32_t brkv                 : 1; /* [13] */
      __IO uint32_t aoen                 : 1; /* [14] */
      __IO uint32_t oen                  : 1; /* [15] */
      __IO uint32_t reserved1            : 16; /* [31:16] */
    } brk_bit;
  };
  /**
    * @brief tmr dmactrl register, offset:0x48
    */
  union
  {
    __IO uint32_t dmactrl;
    struct
    {
      __IO uint32_t addr                 : 5; /* [4:0] */
      __IO uint32_t reserved1            : 3; /* [7:5] */
      __IO uint32_t dtb                  : 5; /* [12:8] */
      __IO uint32_t reserved2            : 19;/* [31:13] */
    } dmactrl_bit;
  };

  /**
    * @brief tmr dmadt register, offset:0x4C
    */
  union
  {
    __IO uint32_t dmadt;
    struct
    {
      __IO uint32_t dmadt                : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } dmadt_bit;
  };

} tmr_type;

/**
  * @}
  */

#define TMR1                             ((tmr_type *) TMR1_BASE)
#define TMR2                             ((tmr_type *) TMR2_BASE)
#define TMR3                             ((tmr_type *) TMR3_BASE)
#define TMR4                             ((tmr_type *) TMR4_BASE)
#define TMR5                             ((tmr_type *) TMR5_BASE)
#define TMR6                             ((tmr_type *) TMR6_BASE)
#define TMR7                             ((tmr_type *) TMR7_BASE)
#define TMR8                             ((tmr_type *) TMR8_BASE)
#define TMR9                             ((tmr_type *) TMR9_BASE)
#define TMR10                            ((tmr_type *) TMR10_BASE)
#define TMR11                            ((tmr_type *) TMR11_BASE)
#define TMR12                            ((tmr_type *) TMR12_BASE)
#define TMR13                            ((tmr_type *) TMR13_BASE)
#define TMR14                            ((tmr_type *) TMR14_BASE)

/** @defgroup TMR_exported_functions
  * @{
  */

void tmr_reset(tmr_type *tmr_x);
void tmr_counter_enable(tmr_type *tmr_x, confirm_state new_state);
void tmr_output_default_para_init(tmr_output_config_type *tmr_output_struct);
void tmr_input_default_para_init(tmr_input_config_type *tmr_input_struct);
void tmr_brkdt_default_para_init(tmr_brkdt_config_type *tmr_brkdt_struct);
void tmr_base_init(tmr_type* tmr_x, uint32_t tmr_pr, uint32_t tmr_div);
void tmr_clock_source_div_set(tmr_type *tmr_x, tmr_clock_division_type tmr_clock_div);
void tmr_cnt_dir_set(tmr_type *tmr_x, tmr_count_mode_type tmr_cnt_dir);
void tmr_repetition_counter_set(tmr_type *tmr_x, uint8_t tmr_rpr_value);
void tmr_counter_value_set(tmr_type *tmr_x, uint32_t tmr_cnt_value);
uint32_t tmr_counter_value_get(tmr_type *tmr_x);
void tmr_div_value_set(tmr_type *tmr_x, uint32_t tmr_div_value);
uint32_t tmr_div_value_get(tmr_type *tmr_x);
void tmr_output_channel_config(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, \
                               tmr_output_config_type *tmr_output_struct);
void tmr_output_channel_mode_select(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, \
                                    tmr_output_control_mode_type oc_mode);
void tmr_period_value_set(tmr_type *tmr_x, uint32_t tmr_pr_value);
uint32_t tmr_period_value_get(tmr_type *tmr_x);
void tmr_channel_value_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, \
                           uint32_t tmr_channel_value);
uint32_t tmr_channel_value_get(tmr_type *tmr_x, tmr_channel_select_type tmr_channel);
void tmr_period_buffer_enable(tmr_type *tmr_x, confirm_state new_state);
void tmr_output_channel_buffer_enable(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, \
                                   confirm_state new_state);
void tmr_output_channel_immediately_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, \
                                        confirm_state new_state);
void tmr_output_channel_switch_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, \
                                   confirm_state new_state);
void tmr_one_cycle_mode_enable(tmr_type *tmr_x, confirm_state new_state);
void tmr_32_bit_function_enable (tmr_type *tmr_x, confirm_state new_state);
void tmr_overflow_request_source_set(tmr_type *tmr_x, confirm_state new_state);
void tmr_overflow_event_disable(tmr_type *tmr_x, confirm_state new_state);
void tmr_input_channel_init(tmr_type *tmr_x, tmr_input_config_type *input_struct, \
                            tmr_channel_input_divider_type divider_factor);
void tmr_channel_enable(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, confirm_state new_state);
void tmr_input_channel_filter_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, \
                                  uint16_t filter_value);
void tmr_pwm_input_config(tmr_type *tmr_x, tmr_input_config_type *input_struct, \
                          tmr_channel_input_divider_type divider_factor);
void tmr_channel1_input_select(tmr_type *tmr_x, tmr_channel1_input_connected_type ti1_connect);
void tmr_input_channel_divider_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, \
                                   tmr_channel_input_divider_type divider_factor);
void tmr_primary_mode_select(tmr_type *tmr_x, tmr_primary_select_type primary_mode);
void tmr_sub_mode_select(tmr_type *tmr_x, tmr_sub_mode_select_type sub_mode);
void tmr_channel_dma_select(tmr_type *tmr_x, tmr_dma_request_source_type cc_dma_select);
void tmr_hall_select(tmr_type *tmr_x,  confirm_state new_state);
void tmr_channel_buffer_enable(tmr_type *tmr_x, confirm_state new_state);
void tmr_trigger_input_select(tmr_type *tmr_x, sub_tmr_input_sel_type trigger_select);
void tmr_sub_sync_mode_set(tmr_type *tmr_x, confirm_state new_state);
void tmr_dma_request_enable(tmr_type *tmr_x, tmr_dma_request_type dma_request, confirm_state new_state);
void tmr_interrupt_enable(tmr_type *tmr_x, uint32_t tmr_interrupt, confirm_state new_state);
flag_status tmr_flag_get(tmr_type *tmr_x, uint32_t tmr_flag);
void tmr_flag_clear(tmr_type *tmr_x, uint32_t tmr_flag);
void tmr_event_sw_trigger(tmr_type *tmr_x, tmr_event_trigger_type tmr_event);
void tmr_output_enable(tmr_type *tmr_x, confirm_state new_state);
void tmr_internal_clock_set(tmr_type *tmr_x);

void tmr_output_channel_polarity_set(tmr_type *tmr_x, tmr_channel_select_type tmr_channel, \
                                     tmr_polarity_active_type oc_polarity);
void tmr_external_clock_config(tmr_type *tmr_x, tmr_external_signal_divider_type es_divide, \
                               tmr_external_signal_polarity_type  es_polarity, uint16_t es_filter);
void tmr_external_clock_mode1_config(tmr_type *tmr_x, tmr_external_signal_divider_type es_divide, \
                                     tmr_external_signal_polarity_type  es_polarity, uint16_t es_filter);
void tmr_external_clock_mode2_config(tmr_type *tmr_x, tmr_external_signal_divider_type es_divide, \
                                     tmr_external_signal_polarity_type  es_polarity, uint16_t es_filter);
void tmr_encoder_mode_config(tmr_type *tmr_x, tmr_encoder_mode_type encoder_mode, tmr_input_polarity_type \
                             ic1_polarity, tmr_input_polarity_type ic2_polarity);
void tmr_force_output_set(tmr_type *tmr_x,  tmr_channel_select_type tmr_channel, \
                          tmr_force_output_type force_output);
void tmr_dma_control_config(tmr_type *tmr_x, tmr_dma_transfer_length_type dma_length, \
                            tmr_dma_address_type dma_base_address);
void tmr_brkdt_config(tmr_type *tmr_x, tmr_brkdt_config_type *brkdt_struct);

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
