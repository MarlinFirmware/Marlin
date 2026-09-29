/**
  **************************************************************************
  * @file     at32f403a_407_dma.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 dma header file
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
#ifndef __AT32F403A_407_DMA_H
#define __AT32F403A_407_DMA_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup DMA
  * @{
  */

/** @defgroup DMA_interrupts_definition
  * @brief dma interrupt
  * @{
  */

#define DMA_FDT_INT                      ((uint32_t)0x00000002) /*!< dma full data transfer interrupt */
#define DMA_HDT_INT                      ((uint32_t)0x00000004) /*!< dma half data transfer interrupt */
#define DMA_DTERR_INT                    ((uint32_t)0x00000008) /*!< dma errorr interrupt */

/**
  * @}
  */

/** @defgroup DMA_flexible_channel
  * @{
  */

#define FLEX_CHANNEL1                    ((uint8_t)0x01) /*!< dma flexible channel1 */
#define FLEX_CHANNEL2                    ((uint8_t)0x02) /*!< dma flexible channel2 */
#define FLEX_CHANNEL3                    ((uint8_t)0x03) /*!< dma flexible channel3 */
#define FLEX_CHANNEL4                    ((uint8_t)0x04) /*!< dma flexible channel4 */
#define FLEX_CHANNEL5                    ((uint8_t)0x05) /*!< dma flexible channel5 */
#define FLEX_CHANNEL6                    ((uint8_t)0x06) /*!< dma flexible channel6 */
#define FLEX_CHANNEL7                    ((uint8_t)0x07) /*!< dma flexible channel7 */

/**
  * @}
  */

/** @defgroup DMA_flags_definition
  * @brief dma flag
  * @{
  */

#define DMA1_GL1_FLAG                    ((uint32_t)0x00000001) /*!< dma1 channel1 global flag */
#define DMA1_FDT1_FLAG                   ((uint32_t)0x00000002) /*!< dma1 channel1 full data transfer flag */
#define DMA1_HDT1_FLAG                   ((uint32_t)0x00000004) /*!< dma1 channel1 half data transfer flag */
#define DMA1_DTERR1_FLAG                 ((uint32_t)0x00000008) /*!< dma1 channel1 error flag */
#define DMA1_GL2_FLAG                    ((uint32_t)0x00000010) /*!< dma1 channel2 global flag */
#define DMA1_FDT2_FLAG                   ((uint32_t)0x00000020) /*!< dma1 channel2 full data transfer flag */
#define DMA1_HDT2_FLAG                   ((uint32_t)0x00000040) /*!< dma1 channel2 half data transfer flag */
#define DMA1_DTERR2_FLAG                 ((uint32_t)0x00000080) /*!< dma1 channel2 error flag */
#define DMA1_GL3_FLAG                    ((uint32_t)0x00000100) /*!< dma1 channel3 global flag */
#define DMA1_FDT3_FLAG                   ((uint32_t)0x00000200) /*!< dma1 channel3 full data transfer flag */
#define DMA1_HDT3_FLAG                   ((uint32_t)0x00000400) /*!< dma1 channel3 half data transfer flag */
#define DMA1_DTERR3_FLAG                 ((uint32_t)0x00000800) /*!< dma1 channel3 error flag */
#define DMA1_GL4_FLAG                    ((uint32_t)0x00001000) /*!< dma1 channel4 global flag */
#define DMA1_FDT4_FLAG                   ((uint32_t)0x00002000) /*!< dma1 channel4 full data transfer flag */
#define DMA1_HDT4_FLAG                   ((uint32_t)0x00004000) /*!< dma1 channel4 half data transfer flag */
#define DMA1_DTERR4_FLAG                 ((uint32_t)0x00008000) /*!< dma1 channel4 error flag */
#define DMA1_GL5_FLAG                    ((uint32_t)0x00010000) /*!< dma1 channel5 global flag */
#define DMA1_FDT5_FLAG                   ((uint32_t)0x00020000) /*!< dma1 channel5 full data transfer flag */
#define DMA1_HDT5_FLAG                   ((uint32_t)0x00040000) /*!< dma1 channel5 half data transfer flag */
#define DMA1_DTERR5_FLAG                 ((uint32_t)0x00080000) /*!< dma1 channel5 error flag */
#define DMA1_GL6_FLAG                    ((uint32_t)0x00100000) /*!< dma1 channel6 global flag */
#define DMA1_FDT6_FLAG                   ((uint32_t)0x00200000) /*!< dma1 channel6 full data transfer flag */
#define DMA1_HDT6_FLAG                   ((uint32_t)0x00400000) /*!< dma1 channel6 half data transfer flag */
#define DMA1_DTERR6_FLAG                 ((uint32_t)0x00800000) /*!< dma1 channel6 error flag */
#define DMA1_GL7_FLAG                    ((uint32_t)0x01000000) /*!< dma1 channel7 global flag */
#define DMA1_FDT7_FLAG                   ((uint32_t)0x02000000) /*!< dma1 channel7 full data transfer flag */
#define DMA1_HDT7_FLAG                   ((uint32_t)0x04000000) /*!< dma1 channel7 half data transfer flag */
#define DMA1_DTERR7_FLAG                 ((uint32_t)0x08000000) /*!< dma1 channel7 error flag */

#define DMA2_GL1_FLAG                    ((uint32_t)0x10000001) /*!< dma2 channel1 global flag */
#define DMA2_FDT1_FLAG                   ((uint32_t)0x10000002) /*!< dma2 channel1 full data transfer flag */
#define DMA2_HDT1_FLAG                   ((uint32_t)0x10000004) /*!< dma2 channel1 half data transfer flag */
#define DMA2_DTERR1_FLAG                 ((uint32_t)0x10000008) /*!< dma2 channel1 error flag */
#define DMA2_GL2_FLAG                    ((uint32_t)0x10000010) /*!< dma2 channel2 global flag */
#define DMA2_FDT2_FLAG                   ((uint32_t)0x10000020) /*!< dma2 channel2 full data transfer flag */
#define DMA2_HDT2_FLAG                   ((uint32_t)0x10000040) /*!< dma2 channel2 half data transfer flag */
#define DMA2_DTERR2_FLAG                 ((uint32_t)0x10000080) /*!< dma2 channel2 error flag */
#define DMA2_GL3_FLAG                    ((uint32_t)0x10000100) /*!< dma2 channel3 global flag */
#define DMA2_FDT3_FLAG                   ((uint32_t)0x10000200) /*!< dma2 channel3 full data transfer flag */
#define DMA2_HDT3_FLAG                   ((uint32_t)0x10000400) /*!< dma2 channel3 half data transfer flag */
#define DMA2_DTERR3_FLAG                 ((uint32_t)0x10000800) /*!< dma2 channel3 error flag */
#define DMA2_GL4_FLAG                    ((uint32_t)0x10001000) /*!< dma2 channel4 global flag */
#define DMA2_FDT4_FLAG                   ((uint32_t)0x10002000) /*!< dma2 channel4 full data transfer flag */
#define DMA2_HDT4_FLAG                   ((uint32_t)0x10004000) /*!< dma2 channel4 half data transfer flag */
#define DMA2_DTERR4_FLAG                 ((uint32_t)0x10008000) /*!< dma2 channel4 error flag */
#define DMA2_GL5_FLAG                    ((uint32_t)0x10010000) /*!< dma2 channel5 global flag */
#define DMA2_FDT5_FLAG                   ((uint32_t)0x10020000) /*!< dma2 channel5 full data transfer flag */
#define DMA2_HDT5_FLAG                   ((uint32_t)0x10040000) /*!< dma2 channel5 half data transfer flag */
#define DMA2_DTERR5_FLAG                 ((uint32_t)0x10080000) /*!< dma2 channel5 error flag */
#define DMA2_GL6_FLAG                    ((uint32_t)0x10100000) /*!< dma2 channel6 global flag */
#define DMA2_FDT6_FLAG                   ((uint32_t)0x10200000) /*!< dma2 channel6 full data transfer flag */
#define DMA2_HDT6_FLAG                   ((uint32_t)0x10400000) /*!< dma2 channel6 half data transfer flag */
#define DMA2_DTERR6_FLAG                 ((uint32_t)0x10800000) /*!< dma2 channel6 error flag */
#define DMA2_GL7_FLAG                    ((uint32_t)0x11000000) /*!< dma2 channel7 global flag */
#define DMA2_FDT7_FLAG                   ((uint32_t)0x12000000) /*!< dma2 channel7 full data transfer flag */
#define DMA2_HDT7_FLAG                   ((uint32_t)0x14000000) /*!< dma2 channel7 half data transfer flag */
#define DMA2_DTERR7_FLAG                 ((uint32_t)0x18000000) /*!< dma2 channel7 error flag */

/**
  * @}
  */

/** @defgroup DMA_exported_types
  * @{
  */

/**
  * @brief dma flexible request type
  */
typedef enum
{
  DMA_FLEXIBLE_ADC1                      = 0x01, /*!< adc1 flexible request id */
  DMA_FLEXIBLE_ADC3                      = 0x03, /*!< adc3 flexible request id */
  DMA_FLEXIBLE_DAC1                      = 0x05, /*!< dac1 flexible request id */
  DMA_FLEXIBLE_DAC2                      = 0x06, /*!< dac2 flexible request id */
  DMA_FLEXIBLE_SPI1_RX                   = 0x09, /*!< spi1_rx flexible request id */
  DMA_FLEXIBLE_SPI1_TX                   = 0x0A, /*!< spi1_tx flexible request id */
  DMA_FLEXIBLE_SPI2_RX                   = 0x0B, /*!< spi2_rx flexible request id */
  DMA_FLEXIBLE_SPI2_TX                   = 0x0C, /*!< spi2_tx flexible request id */
  DMA_FLEXIBLE_SPI3_RX                   = 0x0D, /*!< spi3_rx flexible request id */
  DMA_FLEXIBLE_SPI3_TX                   = 0x0E, /*!< spi3_tx flexible request id */
  DMA_FLEXIBLE_SPI4_RX                   = 0x0F, /*!< spi4_rx flexible request id */
  DMA_FLEXIBLE_SPI4_TX                   = 0x10, /*!< spi4_tx flexible request id */
  DMA_FLEXIBLE_I2S2EXT_RX                = 0x11, /*!< i2s2ext_rx flexible request id */
  DMA_FLEXIBLE_I2S2EXT_TX                = 0x12, /*!< i2s2ext_tx flexible request id */
  DMA_FLEXIBLE_I2S3EXT_RX                = 0x13, /*!< i2s3ext_rx flexible request id */
  DMA_FLEXIBLE_I2S3EXT_TX                = 0x14, /*!< i2s3ext_tx flexible request id */
  DMA_FLEXIBLE_UART1_RX                  = 0x19, /*!< uart1_rx flexible request id */
  DMA_FLEXIBLE_UART1_TX                  = 0x1A, /*!< uart1_tx flexible request id */
  DMA_FLEXIBLE_UART2_RX                  = 0x1B, /*!< uart2_rx flexible request id */
  DMA_FLEXIBLE_UART2_TX                  = 0x1C, /*!< uart2_tx flexible request id */
  DMA_FLEXIBLE_UART3_RX                  = 0x1D, /*!< uart3_rx flexible request id */
  DMA_FLEXIBLE_UART3_TX                  = 0x1E, /*!< uart3_tx flexible request id */
  DMA_FLEXIBLE_UART4_RX                  = 0x1F, /*!< uart4_rx flexible request id */
  DMA_FLEXIBLE_UART4_TX                  = 0x20, /*!< uart4_tx flexible request id */
  DMA_FLEXIBLE_UART5_RX                  = 0x21, /*!< uart5_rx flexible request id */
  DMA_FLEXIBLE_UART5_TX                  = 0x22, /*!< uart5_tx flexible request id */
  DMA_FLEXIBLE_UART6_RX                  = 0x23, /*!< uart6_rx flexible request id */
  DMA_FLEXIBLE_UART6_TX                  = 0x24, /*!< uart6_tx flexible request id */
  DMA_FLEXIBLE_UART7_RX                  = 0x25, /*!< uart7_rx flexible request id */
  DMA_FLEXIBLE_UART7_TX                  = 0x26, /*!< uart7_tx flexible request id */
  DMA_FLEXIBLE_UART8_RX                  = 0x27, /*!< uart8_rx flexible request id */
  DMA_FLEXIBLE_UART8_TX                  = 0x28, /*!< uart8_tx flexible request id */
  DMA_FLEXIBLE_I2C1_RX                   = 0x29, /*!< i2c1_rx flexible request id */
  DMA_FLEXIBLE_I2C1_TX                   = 0x2A, /*!< i2c1_tx flexible request id */
  DMA_FLEXIBLE_I2C2_RX                   = 0x2B, /*!< i2c2_rx flexible request id */
  DMA_FLEXIBLE_I2C2_TX                   = 0x2C, /*!< i2c2_tx flexible request id */
  DMA_FLEXIBLE_I2C3_RX                   = 0x2D, /*!< i2c3_rx flexible request id */
  DMA_FLEXIBLE_I2C3_TX                   = 0x2E, /*!< i2c3_tx flexible request id */
  DMA_FLEXIBLE_SDIO1                     = 0x31, /*!< sdio1 flexible request id */
  DMA_FLEXIBLE_SDIO2                     = 0x32, /*!< sdio2 flexible request id */
  DMA_FLEXIBLE_TMR1_TRIG                 = 0x35, /*!< tmr1_trig flexible request id */
  DMA_FLEXIBLE_TMR1_HALL                 = 0x36, /*!< tmr1_hall flexible request id */
  DMA_FLEXIBLE_TMR1_OVERFLOW             = 0x37, /*!< tmr1_overflow flexible request id */
  DMA_FLEXIBLE_TMR1_CH1                  = 0x38, /*!< tmr1_ch1 flexible request id */
  DMA_FLEXIBLE_TMR1_CH2                  = 0x39, /*!< tmr1_ch2 flexible request id */
  DMA_FLEXIBLE_TMR1_CH3                  = 0x3A, /*!< tmr1_ch3 flexible request id */
  DMA_FLEXIBLE_TMR1_CH4                  = 0x3B, /*!< tmr1_ch4 flexible request id */
  DMA_FLEXIBLE_TMR2_TRIG                 = 0x3D, /*!< tmr2_trig flexible request id */
  DMA_FLEXIBLE_TMR2_OVERFLOW             = 0x3F, /*!< tmr2_overflow flexible request id */
  DMA_FLEXIBLE_TMR2_CH1                  = 0x40, /*!< tmr2_ch1 flexible request id */
  DMA_FLEXIBLE_TMR2_CH2                  = 0x41, /*!< tmr2_ch2 flexible request id */
  DMA_FLEXIBLE_TMR2_CH3                  = 0x42, /*!< tmr2_ch3 flexible request id */
  DMA_FLEXIBLE_TMR2_CH4                  = 0x43, /*!< tmr2_ch4 flexible request id */
  DMA_FLEXIBLE_TMR3_TRIG                 = 0x45, /*!< tmr3_trig flexible request id */
  DMA_FLEXIBLE_TMR3_OVERFLOW             = 0x47, /*!< tmr3_overflow flexible request id */
  DMA_FLEXIBLE_TMR3_CH1                  = 0x48, /*!< tmr3_ch1 flexible request id */
  DMA_FLEXIBLE_TMR3_CH2                  = 0x49, /*!< tmr3_ch2 flexible request id */
  DMA_FLEXIBLE_TMR3_CH3                  = 0x4A, /*!< tmr3_ch3 flexible request id */
  DMA_FLEXIBLE_TMR3_CH4                  = 0x4B, /*!< tmr3_ch4 flexible request id */
  DMA_FLEXIBLE_TMR4_TRIG                 = 0x4D, /*!< tmr4_trig flexible request id */
  DMA_FLEXIBLE_TMR4_OVERFLOW             = 0x4F, /*!< tmr4_overflow flexible request id */
  DMA_FLEXIBLE_TMR4_CH1                  = 0x50, /*!< tmr4_ch1 flexible request id */
  DMA_FLEXIBLE_TMR4_CH2                  = 0x51, /*!< tmr4_ch2 flexible request id */
  DMA_FLEXIBLE_TMR4_CH3                  = 0x52, /*!< tmr4_ch3 flexible request id */
  DMA_FLEXIBLE_TMR4_CH4                  = 0x53, /*!< tmr4_ch4 flexible request id */
  DMA_FLEXIBLE_TMR5_TRIG                 = 0x55, /*!< tmr5_trig flexible request id */
  DMA_FLEXIBLE_TMR5_OVERFLOW             = 0x57, /*!< tmr5_overflow flexible request id */
  DMA_FLEXIBLE_TMR5_CH1                  = 0x58, /*!< tmr5_ch1 flexible request id */
  DMA_FLEXIBLE_TMR5_CH2                  = 0x59, /*!< tmr5_ch2 flexible request id */
  DMA_FLEXIBLE_TMR5_CH3                  = 0x5A, /*!< tmr5_ch3 flexible request id */
  DMA_FLEXIBLE_TMR5_CH4                  = 0x5B, /*!< tmr5_ch4 flexible request id */
  DMA_FLEXIBLE_TMR6_OVERFLOW             = 0x5F, /*!< tmr6_overflow flexible request id */
  DMA_FLEXIBLE_TMR7_OVERFLOW             = 0x67, /*!< tmr7_overflow flexible request id */
  DMA_FLEXIBLE_TMR8_TRIG                 = 0x6D, /*!< tmr8_trig flexible request id */
  DMA_FLEXIBLE_TMR8_HALL                 = 0x6E, /*!< tmr8_hall flexible request id */
  DMA_FLEXIBLE_TMR8_OVERFLOW             = 0x6F, /*!< tmr8_overflow flexible request id */
  DMA_FLEXIBLE_TMR8_CH1                  = 0x70, /*!< tmr8_ch1 flexible request id */
  DMA_FLEXIBLE_TMR8_CH2                  = 0x71, /*!< tmr8_ch2 flexible request id */
  DMA_FLEXIBLE_TMR8_CH3                  = 0x72, /*!< tmr8_ch3 flexible request id */
  DMA_FLEXIBLE_TMR8_CH4                  = 0x73  /*!< tmr8_ch4 flexible request id */
} dma_flexible_request_type;

/**
  * @brief dma direction type
  */
typedef enum
{
  DMA_DIR_PERIPHERAL_TO_MEMORY           = 0x0000, /*!< dma data transfer direction:peripheral to memory */
  DMA_DIR_MEMORY_TO_PERIPHERAL           = 0x0010, /*!< dma data transfer direction:memory to peripheral */
  DMA_DIR_MEMORY_TO_MEMORY               = 0x4000  /*!< dma data transfer direction:memory to memory */
} dma_dir_type;

/**
  * @brief dma peripheral incremented type
  */
typedef enum
{
  DMA_PERIPHERAL_INC_DISABLE             = 0x00, /*!< dma peripheral increment mode disable */
  DMA_PERIPHERAL_INC_ENABLE              = 0x01  /*!< dma peripheral increment mode enable */
} dma_peripheral_inc_type;

/**
  * @brief dma memory incremented type
  */
typedef enum
{
  DMA_MEMORY_INC_DISABLE                 = 0x00, /*!< dma memory increment mode disable */
  DMA_MEMORY_INC_ENABLE                  = 0x01  /*!< dma memory increment mode enable */
} dma_memory_inc_type;

/**
  * @brief dma peripheral data size type
  */
typedef enum
{
  DMA_PERIPHERAL_DATA_WIDTH_BYTE         = 0x00, /*!< dma peripheral databus width 8bit */
  DMA_PERIPHERAL_DATA_WIDTH_HALFWORD     = 0x01, /*!< dma peripheral databus width 16bit */
  DMA_PERIPHERAL_DATA_WIDTH_WORD         = 0x02  /*!< dma peripheral databus width 32bit */
} dma_peripheral_data_size_type;

/**
  * @brief dma memory data size type
  */
typedef enum
{
  DMA_MEMORY_DATA_WIDTH_BYTE             = 0x00, /*!< dma memory databus width 8bit */
  DMA_MEMORY_DATA_WIDTH_HALFWORD         = 0x01, /*!< dma memory databus width 16bit */
  DMA_MEMORY_DATA_WIDTH_WORD             = 0x02  /*!< dma memory databus width 32bit */
} dma_memory_data_size_type;

/**
  * @brief dma priority level type
  */
typedef enum
{
  DMA_PRIORITY_LOW                       = 0x00, /*!< dma channel priority: low */
  DMA_PRIORITY_MEDIUM                    = 0x01, /*!< dma channel priority: mediue */
  DMA_PRIORITY_HIGH                      = 0x02, /*!< dma channel priority: high */
  DMA_PRIORITY_VERY_HIGH                 = 0x03  /*!< dma channel priority: very high */
} dma_priority_level_type;

/**
  * @brief dma init type
  */
typedef struct
{
  uint32_t                               peripheral_base_addr;    /*!< base addrress for peripheral */
  uint32_t                               memory_base_addr;        /*!< base addrress for memory */
  dma_dir_type                           direction;               /*!< dma transmit direction, peripheral as source or as destnation  */
  uint16_t                               buffer_size;             /*!< counter to transfer */
  confirm_state                          peripheral_inc_enable;   /*!< periphera address increment after one transmit */
  confirm_state                          memory_inc_enable;       /*!< memory address increment after one transmit */
  dma_peripheral_data_size_type          peripheral_data_width;   /*!< peripheral data width for transmit */
  dma_memory_data_size_type              memory_data_width;       /*!< memory data width for transmit */
  confirm_state                          loop_mode_enable;        /*!< when circular mode enable, buffer size will reload if count to 0 */
  dma_priority_level_type                priority;                /*!< dma priority can choose from very high, high, dedium or low */
} dma_init_type;

/**
  * @brief type define dma register
  */
typedef struct
{
  /**
    * @brief dma sts register, offset:0x00
    */
  union
  {
    __IO uint32_t sts;
    struct
    {
      __IO uint32_t gf1                  : 1; /* [0] */
      __IO uint32_t fdtf1                : 1; /* [1] */
      __IO uint32_t hdtf1                : 1; /* [2] */
      __IO uint32_t dterrf1              : 1; /* [3] */
      __IO uint32_t gf2                  : 1; /* [4] */
      __IO uint32_t fdtf2                : 1; /* [5] */
      __IO uint32_t hdtf2                : 1; /* [6] */
      __IO uint32_t dterrf2              : 1; /* [7] */
      __IO uint32_t gf3                  : 1; /* [8] */
      __IO uint32_t fdtf3                : 1; /* [9] */
      __IO uint32_t hdtf3                : 1; /* [10] */
      __IO uint32_t dterrf3              : 1; /* [11] */
      __IO uint32_t gf4                  : 1; /* [12] */
      __IO uint32_t fdtf4                : 1; /* [13] */
      __IO uint32_t hdtf4                : 1; /* [14] */
      __IO uint32_t dterrf4              : 1; /* [15] */
      __IO uint32_t gf5                  : 1; /* [16] */
      __IO uint32_t fdtf5                : 1; /* [17] */
      __IO uint32_t hdtf5                : 1; /* [18] */
      __IO uint32_t dterrf5              : 1; /* [19] */
      __IO uint32_t gf6                  : 1; /* [20] */
      __IO uint32_t fdtf6                : 1; /* [21] */
      __IO uint32_t hdtf6                : 1; /* [22] */
      __IO uint32_t dterrf6              : 1; /* [23] */
      __IO uint32_t gf7                  : 1; /* [24] */
      __IO uint32_t fdtf7                : 1; /* [25] */
      __IO uint32_t hdtf7                : 1; /* [26] */
      __IO uint32_t dterrf7              : 1; /* [27] */
      __IO uint32_t reserved1            : 4; /* [31:28] */
    } sts_bit;
  };

  /**
    * @brief dma clr register, offset:0x04
    */
  union
  {
    __IO uint32_t clr;
    struct
    {
      __IO uint32_t gfc1                 : 1; /* [0] */
      __IO uint32_t fdtfc1               : 1; /* [1] */
      __IO uint32_t hdtfc1               : 1; /* [2] */
      __IO uint32_t dterrfc1             : 1; /* [3] */
      __IO uint32_t gfc2                 : 1; /* [4] */
      __IO uint32_t fdtfc2               : 1; /* [5] */
      __IO uint32_t hdtfc2               : 1; /* [6] */
      __IO uint32_t dterrfc2             : 1; /* [7] */
      __IO uint32_t gfc3                 : 1; /* [8] */
      __IO uint32_t fdtfc3               : 1; /* [9] */
      __IO uint32_t hdtfc3               : 1; /* [10] */
      __IO uint32_t dterrfc3             : 1; /* [11] */
      __IO uint32_t gfc4                 : 1; /* [12] */
      __IO uint32_t fdtfc4               : 1; /* [13] */
      __IO uint32_t hdtfc4               : 1; /* [14] */
      __IO uint32_t dterrfc4             : 1; /* [15] */
      __IO uint32_t gfc5                 : 1; /* [16] */
      __IO uint32_t fdtfc5               : 1; /* [17] */
      __IO uint32_t hdtfc5               : 1; /* [18] */
      __IO uint32_t dterrfc5             : 1; /* [19] */
      __IO uint32_t gfc6                 : 1; /* [20] */
      __IO uint32_t fdtfc6               : 1; /* [21] */
      __IO uint32_t hdtfc6               : 1; /* [22] */
      __IO uint32_t dterrfc6             : 1; /* [23] */
      __IO uint32_t gfc7                 : 1; /* [24] */
      __IO uint32_t fdtfc7               : 1; /* [25] */
      __IO uint32_t hdtfc7               : 1; /* [26] */
      __IO uint32_t dterrfc7             : 1; /* [27] */
      __IO uint32_t reserved1            : 4; /* [31:28] */
    } clr_bit;
  };
  /**
    * @brief reserved, offset:0x08~0x9C
    */
  __IO uint32_t reserved1[38];
  /**
    * @brief dma src_sel0 register, offset:0xA0
    */
  union
  {
    __IO uint32_t src_sel0;
    struct
    {
      __IO uint32_t ch1_src              : 8; /* [7:0] */
      __IO uint32_t ch2_src              : 8; /* [15:8] */
      __IO uint32_t ch3_src              : 8; /* [23:16] */
      __IO uint32_t ch4_src              : 8; /* [31:24] */
    } src_sel0_bit;
  };

  /**
    * @brief dma src_sel1 register, offset:0xA4
    */
  union
  {
    __IO uint32_t src_sel1;
    struct
    {
      __IO uint32_t ch5_src              : 8; /* [7:0] */
      __IO uint32_t ch6_src              : 8; /* [15:8] */
      __IO uint32_t ch7_src              : 8; /* [23:16] */
      __IO uint32_t dma_flex_en          : 1; /* [24] */
      __IO uint32_t reserved1            : 7; /* [31:25] */
    } src_sel1_bit;
  };
} dma_type;

/**
  * @brief type define dma channel register all
  */
typedef struct
{
  /**
    * @brief dma ctrl register, offset:0x08+20*(x-1) x=1...7
    */
  union
  {
    __IO uint32_t ctrl;
    struct
    {
      __IO uint32_t chen                 : 1; /* [0] */
      __IO uint32_t fdtien               : 1; /* [1] */
      __IO uint32_t hdtien               : 1; /* [2] */
      __IO uint32_t dterrien             : 1; /* [3] */
      __IO uint32_t dtd                  : 1; /* [4] */
      __IO uint32_t lm                   : 1; /* [5] */
      __IO uint32_t pincm                : 1; /* [6] */
      __IO uint32_t mincm                : 1; /* [7] */
      __IO uint32_t pwidth               : 2; /* [9:8] */
      __IO uint32_t mwidth               : 2; /* [11:10] */
      __IO uint32_t chpl                 : 2; /* [13:12] */
      __IO uint32_t m2m                  : 1; /* [14] */
      __IO uint32_t reserved1            : 17;/* [31:15] */
    } ctrl_bit;
  };

  /**
    * @brief dma dtcnt register, offset:0x0C+20*(x-1) x=1...7
    */
  union
  {
    __IO uint32_t dtcnt;
    struct
    {
      __IO uint32_t cnt                  : 16;/* [15:0] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } dtcnt_bit;
  };

  /**
    * @brief dma cpba register, offset:0x10+20*(x-1) x=1...7
    */
  union
  {
    __IO uint32_t paddr;
    struct
    {
      __IO uint32_t paddr                : 32;/* [31:0] */
    } paddr_bit;
  };

  /**
    * @brief dma cmba register, offset:0x14+20*(x-1) x=1...7
    */
  union
  {
    __IO uint32_t maddr;
    struct
    {
      __IO uint32_t maddr                : 32;/* [31:0] */
    } maddr_bit;
  };
} dma_channel_type;

/**
  * @}
  */

#define DMA1                             ((dma_type *) DMA1_BASE)
#define DMA1_CHANNEL1                    ((dma_channel_type *) DMA1_CHANNEL1_BASE)
#define DMA1_CHANNEL2                    ((dma_channel_type *) DMA1_CHANNEL2_BASE)
#define DMA1_CHANNEL3                    ((dma_channel_type *) DMA1_CHANNEL3_BASE)
#define DMA1_CHANNEL4                    ((dma_channel_type *) DMA1_CHANNEL4_BASE)
#define DMA1_CHANNEL5                    ((dma_channel_type *) DMA1_CHANNEL5_BASE)
#define DMA1_CHANNEL6                    ((dma_channel_type *) DMA1_CHANNEL6_BASE)
#define DMA1_CHANNEL7                    ((dma_channel_type *) DMA1_CHANNEL7_BASE)

#define DMA2                             ((dma_type *) DMA2_BASE)
#define DMA2_CHANNEL1                    ((dma_channel_type *) DMA2_CHANNEL1_BASE)
#define DMA2_CHANNEL2                    ((dma_channel_type *) DMA2_CHANNEL2_BASE)
#define DMA2_CHANNEL3                    ((dma_channel_type *) DMA2_CHANNEL3_BASE)
#define DMA2_CHANNEL4                    ((dma_channel_type *) DMA2_CHANNEL4_BASE)
#define DMA2_CHANNEL5                    ((dma_channel_type *) DMA2_CHANNEL5_BASE)
#define DMA2_CHANNEL6                    ((dma_channel_type *) DMA2_CHANNEL6_BASE)
#define DMA2_CHANNEL7                    ((dma_channel_type *) DMA2_CHANNEL7_BASE)

/** @defgroup DMA_exported_functions
  * @{
  */

void dma_reset(dma_channel_type* dmax_channely);
void dma_data_number_set(dma_channel_type* dmax_channely, uint16_t data_number);
uint16_t dma_data_number_get(dma_channel_type* dmax_channely);
void dma_interrupt_enable(dma_channel_type* dmax_channely, uint32_t dma_int, confirm_state new_state);
void dma_channel_enable(dma_channel_type* dmax_channely, confirm_state new_state);
void dma_flexible_config(dma_type* dma_x, uint8_t flex_channelx, dma_flexible_request_type flexible_request);
flag_status dma_flag_get(uint32_t dmax_flag);
void dma_flag_clear(uint32_t dmax_flag);
void dma_default_para_init(dma_init_type* dma_init_struct);
void dma_init(dma_channel_type* dmax_channely, dma_init_type* dma_init_struct);

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
