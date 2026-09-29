/**
  **************************************************************************
  * @file     at32f403a_407_crm.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 crm header file
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
#ifndef __AT32F403A_407_CRM_H
#define __AT32F403A_407_CRM_H

#ifdef __cplusplus
extern "C" {
#endif


/* includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup CRM
  * @{
  */

#define CRM_REG(value)                   PERIPH_REG(CRM_BASE, value)
#define CRM_REG_BIT(value)               PERIPH_REG_BIT(value)

/** @defgroup CRM_flags_definition
  * @brief crm flag
  * @{
  */

#define CRM_HICK_STABLE_FLAG             MAKE_VALUE(0x00, 1)  /*!< high speed internal clock stable flag */
#define CRM_HEXT_STABLE_FLAG             MAKE_VALUE(0x00, 17) /*!< high speed external crystal stable flag */
#define CRM_PLL_STABLE_FLAG              MAKE_VALUE(0x00, 25) /*!< phase locking loop stable flag */
#define CRM_LEXT_STABLE_FLAG             MAKE_VALUE(0x20, 1)  /*!< low speed external crystal stable flag */
#define CRM_LICK_STABLE_FLAG             MAKE_VALUE(0x24, 1)  /*!< low speed internal clock stable flag */
#define CRM_ALL_RESET_FLAG               MAKE_VALUE(0x24, 24) /*!< all reset flag */
#define CRM_NRST_RESET_FLAG              MAKE_VALUE(0x24, 26) /*!< nrst pin reset flag */
#define CRM_POR_RESET_FLAG               MAKE_VALUE(0x24, 27) /*!< power on reset flag */
#define CRM_SW_RESET_FLAG                MAKE_VALUE(0x24, 28) /*!< software reset flag */
#define CRM_WDT_RESET_FLAG               MAKE_VALUE(0x24, 29) /*!< watchdog timer reset flag */
#define CRM_WWDT_RESET_FLAG              MAKE_VALUE(0x24, 30) /*!< window watchdog timer reset flag */
#define CRM_LOWPOWER_RESET_FLAG          MAKE_VALUE(0x24, 31) /*!< low-power reset flag */
#define CRM_LICK_READY_INT_FLAG          MAKE_VALUE(0x08, 0)  /*!< low speed internal clock stable interrupt ready flag */
#define CRM_LEXT_READY_INT_FLAG          MAKE_VALUE(0x08, 1)  /*!< low speed external crystal stable interrupt ready flag */
#define CRM_HICK_READY_INT_FLAG          MAKE_VALUE(0x08, 2)  /*!< high speed internal clock stable interrupt ready flag */
#define CRM_HEXT_READY_INT_FLAG          MAKE_VALUE(0x08, 3)  /*!< high speed external crystal stable interrupt ready flag */
#define CRM_PLL_READY_INT_FLAG           MAKE_VALUE(0x08, 4)  /*!< phase locking loop stable interrupt ready flag */
#define CRM_CLOCK_FAILURE_INT_FLAG       MAKE_VALUE(0x08, 7)  /*!< clock failure interrupt ready flag */

/**
  * @}
  */

/** @defgroup CRM_interrupts_definition
  * @brief crm interrupt
  * @{
  */

#define CRM_LICK_STABLE_INT              ((uint32_t)0x00000100) /*!< low speed internal clock stable interrupt */
#define CRM_LEXT_STABLE_INT              ((uint32_t)0x00000200) /*!< low speed external crystal stable interrupt */
#define CRM_HICK_STABLE_INT              ((uint32_t)0x00000400) /*!< high speed internal clock stable interrupt */
#define CRM_HEXT_STABLE_INT              ((uint32_t)0x00000800) /*!< high speed external crystal stable interrupt */
#define CRM_PLL_STABLE_INT               ((uint32_t)0x00001000) /*!< phase locking loop stable interrupt */
#define CRM_CLOCK_FAILURE_INT            ((uint32_t)0x00800000) /*!< clock failure interrupt */

/**
  * @}
  */

/** @defgroup CRM_exported_types
  * @{
  */

/**
  * @brief crm periph clock
  */
typedef enum
{
#if defined (AT32F403Axx)
  /* ahb periph */
  CRM_DMA1_PERIPH_CLOCK                  = MAKE_VALUE(0x14, 0),  /*!< dma1 periph clock */
  CRM_DMA2_PERIPH_CLOCK                  = MAKE_VALUE(0x14, 1),  /*!< dma2 periph clock */
  CRM_CRC_PERIPH_CLOCK                   = MAKE_VALUE(0x14, 6),  /*!< crc periph clock */
  CRM_XMC_PERIPH_CLOCK                   = MAKE_VALUE(0x14, 8),  /*!< xmc periph clock */
  CRM_SDIO1_PERIPH_CLOCK                 = MAKE_VALUE(0x14, 10), /*!< sdio1 periph clock */
  CRM_SDIO2_PERIPH_CLOCK                 = MAKE_VALUE(0x14, 11), /*!< sdio2 periph clock */
  /* apb2 periph */
  CRM_IOMUX_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 0),  /*!< iomux periph clock */
  CRM_GPIOA_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 2),  /*!< gpioa periph clock */
  CRM_GPIOB_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 3),  /*!< gpiob periph clock */
  CRM_GPIOC_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 4),  /*!< gpioc periph clock */
  CRM_GPIOD_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 5),  /*!< gpiod periph clock */
  CRM_GPIOE_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 6),  /*!< gpioe periph clock */
  CRM_ADC1_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 9),  /*!< adc1 periph clock */
  CRM_ADC2_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 10), /*!< adc2 periph clock */
  CRM_TMR1_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 11), /*!< tmr1 periph clock */
  CRM_SPI1_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 12), /*!< spi1 periph clock */
  CRM_TMR8_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 13), /*!< tmr8 periph clock */
  CRM_USART1_PERIPH_CLOCK                = MAKE_VALUE(0x18, 14), /*!< usart1 periph clock */
  CRM_ADC3_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 15), /*!< adc3 periph clock */
  CRM_TMR9_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 19), /*!< tmr9 periph clock */
  CRM_TMR10_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 20), /*!< tmr10 periph clock */
  CRM_TMR11_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 21), /*!< tmr11 periph clock */
  CRM_ACC_PERIPH_CLOCK                   = MAKE_VALUE(0x18, 22), /*!< acc periph clock */
  CRM_I2C3_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 23), /*!< i2c3 periph clock */
  CRM_USART6_PERIPH_CLOCK                = MAKE_VALUE(0x18, 24), /*!< usart6 periph clock */
  CRM_UART7_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 25), /*!< uart7 periph clock */
  CRM_UART8_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 26), /*!< uart8 periph clock */
  /* apb1 periph */
  CRM_TMR2_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 0),  /*!< tmr2 periph clock */
  CRM_TMR3_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 1),  /*!< tmr3 periph clock */
  CRM_TMR4_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 2),  /*!< tmr4 periph clock */
  CRM_TMR5_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 3),  /*!< tmr5 periph clock */
  CRM_TMR6_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 4),  /*!< tmr6 periph clock */
  CRM_TMR7_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 5),  /*!< tmr7 periph clock */
  CRM_TMR12_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 6),  /*!< tmr12 periph clock */
  CRM_TMR13_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 7),  /*!< tmr13 periph clock */
  CRM_TMR14_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 8),  /*!< tmr14 periph clock */
  CRM_WWDT_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 11), /*!< wwdt periph clock */
  CRM_SPI2_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 14), /*!< spi2 periph clock */
  CRM_SPI3_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 15), /*!< spi3 periph clock */
  CRM_SPI4_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 16), /*!< spi4 periph clock */
  CRM_USART2_PERIPH_CLOCK                = MAKE_VALUE(0x1C, 17), /*!< usart2 periph clock */
  CRM_USART3_PERIPH_CLOCK                = MAKE_VALUE(0x1C, 18), /*!< usart3 periph clock */
  CRM_UART4_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 19), /*!< uart4 periph clock */
  CRM_UART5_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 20), /*!< uart5 periph clock */
  CRM_I2C1_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 21), /*!< i2c1 periph clock */
  CRM_I2C2_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 22), /*!< i2c2 periph clock */
  CRM_USB_PERIPH_CLOCK                   = MAKE_VALUE(0x1C, 23), /*!< usb periph clock */
  CRM_CAN1_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 25), /*!< can1 periph clock */
  CRM_CAN2_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 26), /*!< can2 periph clock */
  CRM_BPR_PERIPH_CLOCK                   = MAKE_VALUE(0x1C, 27), /*!< bpr periph clock */
  CRM_PWC_PERIPH_CLOCK                   = MAKE_VALUE(0x1C, 28), /*!< pwc periph clock */
  CRM_DAC_PERIPH_CLOCK                   = MAKE_VALUE(0x1C, 29)  /*!< dac periph clock */
#endif

#if defined (AT32F407xx)
  /* ahb periph */
  CRM_DMA1_PERIPH_CLOCK                  = MAKE_VALUE(0x14, 0),  /*!< dma1 periph clock */
  CRM_DMA2_PERIPH_CLOCK                  = MAKE_VALUE(0x14, 1),  /*!< dma2 periph clock */
  CRM_CRC_PERIPH_CLOCK                   = MAKE_VALUE(0x14, 6),  /*!< crc periph clock */
  CRM_XMC_PERIPH_CLOCK                   = MAKE_VALUE(0x14, 8),  /*!< xmc periph clock */
  CRM_SDIO1_PERIPH_CLOCK                 = MAKE_VALUE(0x14, 10), /*!< sdio1 periph clock */
  CRM_SDIO2_PERIPH_CLOCK                 = MAKE_VALUE(0x14, 11), /*!< sdio2 periph clock */
  CRM_EMAC_PERIPH_CLOCK                  = MAKE_VALUE(0x14, 14), /*!< emac periph clock */
  CRM_EMACTX_PERIPH_CLOCK                = MAKE_VALUE(0x14, 15), /*!< emac tx periph clock */
  CRM_EMACRX_PERIPH_CLOCK                = MAKE_VALUE(0x14, 16), /*!< emac rx periph clock */
  CRM_EMACPTP_PERIPH_CLOCK               = MAKE_VALUE(0x14, 28), /*!< emac ptp periph clock */
  /* apb2 periph */
  CRM_IOMUX_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 0),  /*!< iomux periph clock */
  CRM_GPIOA_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 2),  /*!< gpioa periph clock */
  CRM_GPIOB_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 3),  /*!< gpiob periph clock */
  CRM_GPIOC_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 4),  /*!< gpioc periph clock */
  CRM_GPIOD_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 5),  /*!< gpiod periph clock */
  CRM_GPIOE_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 6),  /*!< gpioe periph clock */
  CRM_ADC1_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 9),  /*!< adc1 periph clock */
  CRM_ADC2_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 10), /*!< adc2 periph clock */
  CRM_TMR1_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 11), /*!< tmr1 periph clock */
  CRM_SPI1_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 12), /*!< spi1 periph clock */
  CRM_TMR8_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 13), /*!< tmr8 periph clock */
  CRM_USART1_PERIPH_CLOCK                = MAKE_VALUE(0x18, 14), /*!< usart1 periph clock */
  CRM_ADC3_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 15), /*!< adc3 periph clock */
  CRM_TMR9_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 19), /*!< tmr9 periph clock */
  CRM_TMR10_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 20), /*!< tmr10 periph clock */
  CRM_TMR11_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 21), /*!< tmr11 periph clock */
  CRM_ACC_PERIPH_CLOCK                   = MAKE_VALUE(0x18, 22), /*!< acc periph clock */
  CRM_I2C3_PERIPH_CLOCK                  = MAKE_VALUE(0x18, 23), /*!< i2c3 periph clock */
  CRM_USART6_PERIPH_CLOCK                = MAKE_VALUE(0x18, 24), /*!< usart6 periph clock */
  CRM_UART7_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 25), /*!< uart7 periph clock */
  CRM_UART8_PERIPH_CLOCK                 = MAKE_VALUE(0x18, 26), /*!< uart8 periph clock */
  /* apb1 periph */
  CRM_TMR2_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 0),  /*!< tmr2 periph clock */
  CRM_TMR3_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 1),  /*!< tmr3 periph clock */
  CRM_TMR4_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 2),  /*!< tmr4 periph clock */
  CRM_TMR5_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 3),  /*!< tmr5 periph clock */
  CRM_TMR6_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 4),  /*!< tmr6 periph clock */
  CRM_TMR7_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 5),  /*!< tmr7 periph clock */
  CRM_TMR12_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 6),  /*!< tmr12 periph clock */
  CRM_TMR13_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 7),  /*!< tmr13 periph clock */
  CRM_TMR14_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 8),  /*!< tmr14 periph clock */
  CRM_WWDT_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 11), /*!< wwdt periph clock */
  CRM_SPI2_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 14), /*!< spi2 periph clock */
  CRM_SPI3_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 15), /*!< spi3 periph clock */
  CRM_SPI4_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 16), /*!< spi4 periph clock */
  CRM_USART2_PERIPH_CLOCK                = MAKE_VALUE(0x1C, 17), /*!< usart2 periph clock */
  CRM_USART3_PERIPH_CLOCK                = MAKE_VALUE(0x1C, 18), /*!< usart3 periph clock */
  CRM_UART4_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 19), /*!< uart4 periph clock */
  CRM_UART5_PERIPH_CLOCK                 = MAKE_VALUE(0x1C, 20), /*!< uart5 periph clock */
  CRM_I2C1_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 21), /*!< i2c1 periph clock */
  CRM_I2C2_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 22), /*!< i2c2 periph clock */
  CRM_USB_PERIPH_CLOCK                   = MAKE_VALUE(0x1C, 23), /*!< usb periph clock */
  CRM_CAN1_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 25), /*!< can1 periph clock */
  CRM_CAN2_PERIPH_CLOCK                  = MAKE_VALUE(0x1C, 26), /*!< can2 periph clock */
  CRM_BPR_PERIPH_CLOCK                   = MAKE_VALUE(0x1C, 27), /*!< bpr periph clock */
  CRM_PWC_PERIPH_CLOCK                   = MAKE_VALUE(0x1C, 28), /*!< pwc periph clock */
  CRM_DAC_PERIPH_CLOCK                   = MAKE_VALUE(0x1C, 29)  /*!< dac periph clock */
#endif

} crm_periph_clock_type;

/**
  * @brief crm periph reset
  */
typedef enum
{
#if defined (AT32F403Axx)
  /* apb2 periph */
  CRM_IOMUX_PERIPH_RESET                 = MAKE_VALUE(0x0C, 0),  /*!< iomux periph reset */
  CRM_EXINT_PERIPH_RESET                 = MAKE_VALUE(0x0C, 1),  /*!< exint periph reset */
  CRM_GPIOA_PERIPH_RESET                 = MAKE_VALUE(0x0C, 2),  /*!< gpioa periph reset */
  CRM_GPIOB_PERIPH_RESET                 = MAKE_VALUE(0x0C, 3),  /*!< gpiob periph reset */
  CRM_GPIOC_PERIPH_RESET                 = MAKE_VALUE(0x0C, 4),  /*!< gpioc periph reset */
  CRM_GPIOD_PERIPH_RESET                 = MAKE_VALUE(0x0C, 5),  /*!< gpiod periph reset */
  CRM_GPIOE_PERIPH_RESET                 = MAKE_VALUE(0x0C, 6),  /*!< gpioe periph reset */
  CRM_ADC1_PERIPH_RESET                  = MAKE_VALUE(0x0C, 9),  /*!< adc1 periph reset */
  CRM_ADC2_PERIPH_RESET                  = MAKE_VALUE(0x0C, 10), /*!< adc2 periph reset */
  CRM_TMR1_PERIPH_RESET                  = MAKE_VALUE(0x0C, 11), /*!< tmr1 periph reset */
  CRM_SPI1_PERIPH_RESET                  = MAKE_VALUE(0x0C, 12), /*!< spi2 periph reset */
  CRM_TMR8_PERIPH_RESET                  = MAKE_VALUE(0x0C, 13), /*!< tmr8 periph reset */
  CRM_USART1_PERIPH_RESET                = MAKE_VALUE(0x0C, 14), /*!< usart1 periph reset */
  CRM_ADC3_PERIPH_RESET                  = MAKE_VALUE(0x0C, 15), /*!< adc3 periph reset */
  CRM_TMR9_PERIPH_RESET                  = MAKE_VALUE(0x0C, 19), /*!< tmr9 periph reset */
  CRM_TMR10_PERIPH_RESET                 = MAKE_VALUE(0x0C, 20), /*!< tmr10 periph reset */
  CRM_TMR11_PERIPH_RESET                 = MAKE_VALUE(0x0C, 21), /*!< tmr11 periph reset */
  CRM_ACC_PERIPH_RESET                   = MAKE_VALUE(0x0C, 22), /*!< acc periph reset */
  CRM_I2C3_PERIPH_RESET                  = MAKE_VALUE(0x0C, 23), /*!< i2c3 periph reset */
  CRM_USART6_PERIPH_RESET                = MAKE_VALUE(0x0C, 24), /*!< usart6 periph reset */
  CRM_UART7_PERIPH_RESET                 = MAKE_VALUE(0x0C, 25), /*!< uart7 periph reset */
  CRM_UART8_PERIPH_RESET                 = MAKE_VALUE(0x0C, 26), /*!< uart8 periph reset */
  /* apb1 periph */
  CRM_TMR2_PERIPH_RESET                  = MAKE_VALUE(0x10, 0),  /*!< tmr2 periph reset */
  CRM_TMR3_PERIPH_RESET                  = MAKE_VALUE(0x10, 1),  /*!< tmr3 periph reset */
  CRM_TMR4_PERIPH_RESET                  = MAKE_VALUE(0x10, 2),  /*!< tmr4 periph reset */
  CRM_TMR5_PERIPH_RESET                  = MAKE_VALUE(0x10, 3),  /*!< tmr5 periph reset */
  CRM_TMR6_PERIPH_RESET                  = MAKE_VALUE(0x10, 4),  /*!< tmr6 periph reset */
  CRM_TMR7_PERIPH_RESET                  = MAKE_VALUE(0x10, 5),  /*!< tmr7 periph reset */
  CRM_TMR12_PERIPH_RESET                 = MAKE_VALUE(0x10, 6),  /*!< tmr12 periph reset */
  CRM_TMR13_PERIPH_RESET                 = MAKE_VALUE(0x10, 7),  /*!< tmr13 periph reset */
  CRM_TMR14_PERIPH_RESET                 = MAKE_VALUE(0x10, 8),  /*!< tmr14 periph reset */
  CRM_WWDT_PERIPH_RESET                  = MAKE_VALUE(0x10, 11), /*!< wwdt periph reset */
  CRM_SPI2_PERIPH_RESET                  = MAKE_VALUE(0x10, 14), /*!< spi2 periph reset */
  CRM_SPI3_PERIPH_RESET                  = MAKE_VALUE(0x10, 15), /*!< spi3 periph reset */
  CRM_SPI4_PERIPH_RESET                  = MAKE_VALUE(0x10, 16), /*!< spi4 periph reset */
  CRM_USART2_PERIPH_RESET                = MAKE_VALUE(0x10, 17), /*!< usart2 periph reset */
  CRM_USART3_PERIPH_RESET                = MAKE_VALUE(0x10, 18), /*!< usart3 periph reset */
  CRM_UART4_PERIPH_RESET                 = MAKE_VALUE(0x10, 19), /*!< uart4 periph reset */
  CRM_UART5_PERIPH_RESET                 = MAKE_VALUE(0x10, 20), /*!< uart5 periph reset */
  CRM_I2C1_PERIPH_RESET                  = MAKE_VALUE(0x10, 21), /*!< i2c1 periph reset */
  CRM_I2C2_PERIPH_RESET                  = MAKE_VALUE(0x10, 22), /*!< i2c2 periph reset */
  CRM_USB_PERIPH_RESET                   = MAKE_VALUE(0x10, 23), /*!< usb periph reset */
  CRM_CAN1_PERIPH_RESET                  = MAKE_VALUE(0x10, 25), /*!< can1 periph reset */
  CRM_CAN2_PERIPH_RESET                  = MAKE_VALUE(0x10, 26), /*!< can2 periph reset */
  CRM_BPR_PERIPH_RESET                   = MAKE_VALUE(0x10, 27), /*!< bpr periph reset */
  CRM_PWC_PERIPH_RESET                   = MAKE_VALUE(0x10, 28), /*!< pwc periph reset */
  CRM_DAC_PERIPH_RESET                   = MAKE_VALUE(0x10, 29)  /*!< dac periph reset */
#endif

#if defined (AT32F407xx)
  /* ahb periph */
  CRM_EMAC_PERIPH_RESET                  = MAKE_VALUE(0x28, 14), /*!< emac periph reset */
  /* apb2 periph */
  CRM_IOMUX_PERIPH_RESET                 = MAKE_VALUE(0x0C, 0),  /*!< iomux periph reset */
  CRM_EXINT_PERIPH_RESET                 = MAKE_VALUE(0x0C, 1),  /*!< exint periph reset */
  CRM_GPIOA_PERIPH_RESET                 = MAKE_VALUE(0x0C, 2),  /*!< gpioa periph reset */
  CRM_GPIOB_PERIPH_RESET                 = MAKE_VALUE(0x0C, 3),  /*!< gpiob periph reset */
  CRM_GPIOC_PERIPH_RESET                 = MAKE_VALUE(0x0C, 4),  /*!< gpioc periph reset */
  CRM_GPIOD_PERIPH_RESET                 = MAKE_VALUE(0x0C, 5),  /*!< gpiod periph reset */
  CRM_GPIOE_PERIPH_RESET                 = MAKE_VALUE(0x0C, 6),  /*!< gpioe periph reset */
  CRM_ADC1_PERIPH_RESET                  = MAKE_VALUE(0x0C, 9),  /*!< adc1 periph reset */
  CRM_ADC2_PERIPH_RESET                  = MAKE_VALUE(0x0C, 10), /*!< adc2 periph reset */
  CRM_TMR1_PERIPH_RESET                  = MAKE_VALUE(0x0C, 11), /*!< tmr1 periph reset */
  CRM_SPI1_PERIPH_RESET                  = MAKE_VALUE(0x0C, 12), /*!< spi2 periph reset */
  CRM_TMR8_PERIPH_RESET                  = MAKE_VALUE(0x0C, 13), /*!< tmr8 periph reset */
  CRM_USART1_PERIPH_RESET                = MAKE_VALUE(0x0C, 14), /*!< usart1 periph reset */
  CRM_ADC3_PERIPH_RESET                  = MAKE_VALUE(0x0C, 15), /*!< adc3 periph reset */
  CRM_TMR9_PERIPH_RESET                  = MAKE_VALUE(0x0C, 19), /*!< tmr9 periph reset */
  CRM_TMR10_PERIPH_RESET                 = MAKE_VALUE(0x0C, 20), /*!< tmr10 periph reset */
  CRM_TMR11_PERIPH_RESET                 = MAKE_VALUE(0x0C, 21), /*!< tmr11 periph reset */
  CRM_ACC_PERIPH_RESET                   = MAKE_VALUE(0x0C, 22), /*!< acc periph reset */
  CRM_I2C3_PERIPH_RESET                  = MAKE_VALUE(0x0C, 23), /*!< i2c3 periph reset */
  CRM_USART6_PERIPH_RESET                = MAKE_VALUE(0x0C, 24), /*!< usart6 periph reset */
  CRM_UART7_PERIPH_RESET                 = MAKE_VALUE(0x0C, 25), /*!< uart7 periph reset */
  CRM_UART8_PERIPH_RESET                 = MAKE_VALUE(0x0C, 26), /*!< uart8 periph reset */
  /* apb1 periph */
  CRM_TMR2_PERIPH_RESET                  = MAKE_VALUE(0x10, 0),  /*!< tmr2 periph reset */
  CRM_TMR3_PERIPH_RESET                  = MAKE_VALUE(0x10, 1),  /*!< tmr3 periph reset */
  CRM_TMR4_PERIPH_RESET                  = MAKE_VALUE(0x10, 2),  /*!< tmr4 periph reset */
  CRM_TMR5_PERIPH_RESET                  = MAKE_VALUE(0x10, 3),  /*!< tmr5 periph reset */
  CRM_TMR6_PERIPH_RESET                  = MAKE_VALUE(0x10, 4),  /*!< tmr6 periph reset */
  CRM_TMR7_PERIPH_RESET                  = MAKE_VALUE(0x10, 5),  /*!< tmr7 periph reset */
  CRM_TMR12_PERIPH_RESET                 = MAKE_VALUE(0x10, 6),  /*!< tmr12 periph reset */
  CRM_TMR13_PERIPH_RESET                 = MAKE_VALUE(0x10, 7),  /*!< tmr13 periph reset */
  CRM_TMR14_PERIPH_RESET                 = MAKE_VALUE(0x10, 8),  /*!< tmr14 periph reset */
  CRM_WWDT_PERIPH_RESET                  = MAKE_VALUE(0x10, 11), /*!< wwdt periph reset */
  CRM_SPI2_PERIPH_RESET                  = MAKE_VALUE(0x10, 14), /*!< spi2 periph reset */
  CRM_SPI3_PERIPH_RESET                  = MAKE_VALUE(0x10, 15), /*!< spi3 periph reset */
  CRM_SPI4_PERIPH_RESET                  = MAKE_VALUE(0x10, 16), /*!< spi4 periph reset */
  CRM_USART2_PERIPH_RESET                = MAKE_VALUE(0x10, 17), /*!< usart2 periph reset */
  CRM_USART3_PERIPH_RESET                = MAKE_VALUE(0x10, 18), /*!< usart3 periph reset */
  CRM_UART4_PERIPH_RESET                 = MAKE_VALUE(0x10, 19), /*!< uart4 periph reset */
  CRM_UART5_PERIPH_RESET                 = MAKE_VALUE(0x10, 20), /*!< uart5 periph reset */
  CRM_I2C1_PERIPH_RESET                  = MAKE_VALUE(0x10, 21), /*!< i2c1 periph reset */
  CRM_I2C2_PERIPH_RESET                  = MAKE_VALUE(0x10, 22), /*!< i2c2 periph reset */
  CRM_USB_PERIPH_RESET                   = MAKE_VALUE(0x10, 23), /*!< usb periph reset */
  CRM_CAN1_PERIPH_RESET                  = MAKE_VALUE(0x10, 25), /*!< can1 periph reset */
  CRM_CAN2_PERIPH_RESET                  = MAKE_VALUE(0x10, 26), /*!< can2 periph reset */
  CRM_BPR_PERIPH_RESET                   = MAKE_VALUE(0x10, 27), /*!< bpr periph reset */
  CRM_PWC_PERIPH_RESET                   = MAKE_VALUE(0x10, 28), /*!< pwc periph reset */
  CRM_DAC_PERIPH_RESET                   = MAKE_VALUE(0x10, 29)  /*!< dac periph reset */
#endif

} crm_periph_reset_type;

/**
  * @brief crm periph clock in sleep mode
  */
typedef enum
{
  /* ahb periph */
  CRM_SRAM_PERIPH_CLOCK_SLEEP_MODE       = MAKE_VALUE(0x14, 2), /*!< sram sleep mode periph clock */
  CRM_FLASH_PERIPH_CLOCK_SLEEP_MODE      = MAKE_VALUE(0x14, 4)  /*!< flash sleep mode periph clock */
} crm_periph_clock_sleepmd_type;

/**
  * @brief crm pll mult_x
  */
typedef enum
{
  CRM_PLL_MULT_2                         = 0,  /*!< pll multiplication factor 2 */
  CRM_PLL_MULT_3                         = 1,  /*!< pll multiplication factor 3 */
  CRM_PLL_MULT_4                         = 2,  /*!< pll multiplication factor 4 */
  CRM_PLL_MULT_5                         = 3,  /*!< pll multiplication factor 5 */
  CRM_PLL_MULT_6                         = 4,  /*!< pll multiplication factor 6 */
  CRM_PLL_MULT_7                         = 5,  /*!< pll multiplication factor 7 */
  CRM_PLL_MULT_8                         = 6,  /*!< pll multiplication factor 8 */
  CRM_PLL_MULT_9                         = 7,  /*!< pll multiplication factor 9 */
  CRM_PLL_MULT_10                        = 8,  /*!< pll multiplication factor 10 */
  CRM_PLL_MULT_11                        = 9,  /*!< pll multiplication factor 11 */
  CRM_PLL_MULT_12                        = 10, /*!< pll multiplication factor 12 */
  CRM_PLL_MULT_13                        = 11, /*!< pll multiplication factor 13 */
  CRM_PLL_MULT_14                        = 12, /*!< pll multiplication factor 14 */
  CRM_PLL_MULT_15                        = 13, /*!< pll multiplication factor 15 */
  CRM_PLL_MULT_16                        = 15, /*!< pll multiplication factor 16 */
  CRM_PLL_MULT_17                        = 16, /*!< pll multiplication factor 17 */
  CRM_PLL_MULT_18                        = 17, /*!< pll multiplication factor 18 */
  CRM_PLL_MULT_19                        = 18, /*!< pll multiplication factor 19 */
  CRM_PLL_MULT_20                        = 19, /*!< pll multiplication factor 20 */
  CRM_PLL_MULT_21                        = 20, /*!< pll multiplication factor 21 */
  CRM_PLL_MULT_22                        = 21, /*!< pll multiplication factor 22 */
  CRM_PLL_MULT_23                        = 22, /*!< pll multiplication factor 23 */
  CRM_PLL_MULT_24                        = 23, /*!< pll multiplication factor 24 */
  CRM_PLL_MULT_25                        = 24, /*!< pll multiplication factor 25 */
  CRM_PLL_MULT_26                        = 25, /*!< pll multiplication factor 26 */
  CRM_PLL_MULT_27                        = 26, /*!< pll multiplication factor 27 */
  CRM_PLL_MULT_28                        = 27, /*!< pll multiplication factor 28 */
  CRM_PLL_MULT_29                        = 28, /*!< pll multiplication factor 29 */
  CRM_PLL_MULT_30                        = 29, /*!< pll multiplication factor 30 */
  CRM_PLL_MULT_31                        = 30, /*!< pll multiplication factor 31 */
  CRM_PLL_MULT_32                        = 31, /*!< pll multiplication factor 32 */
  CRM_PLL_MULT_33                        = 32, /*!< pll multiplication factor 33 */
  CRM_PLL_MULT_34                        = 33, /*!< pll multiplication factor 34 */
  CRM_PLL_MULT_35                        = 34, /*!< pll multiplication factor 35 */
  CRM_PLL_MULT_36                        = 35, /*!< pll multiplication factor 36 */
  CRM_PLL_MULT_37                        = 36, /*!< pll multiplication factor 37 */
  CRM_PLL_MULT_38                        = 37, /*!< pll multiplication factor 38 */
  CRM_PLL_MULT_39                        = 38, /*!< pll multiplication factor 39 */
  CRM_PLL_MULT_40                        = 39, /*!< pll multiplication factor 40 */
  CRM_PLL_MULT_41                        = 40, /*!< pll multiplication factor 41 */
  CRM_PLL_MULT_42                        = 41, /*!< pll multiplication factor 42 */
  CRM_PLL_MULT_43                        = 42, /*!< pll multiplication factor 43 */
  CRM_PLL_MULT_44                        = 43, /*!< pll multiplication factor 44 */
  CRM_PLL_MULT_45                        = 44, /*!< pll multiplication factor 45 */
  CRM_PLL_MULT_46                        = 45, /*!< pll multiplication factor 46 */
  CRM_PLL_MULT_47                        = 46, /*!< pll multiplication factor 47 */
  CRM_PLL_MULT_48                        = 47, /*!< pll multiplication factor 48 */
  CRM_PLL_MULT_49                        = 48, /*!< pll multiplication factor 49 */
  CRM_PLL_MULT_50                        = 49, /*!< pll multiplication factor 50 */
  CRM_PLL_MULT_51                        = 50, /*!< pll multiplication factor 51 */
  CRM_PLL_MULT_52                        = 51, /*!< pll multiplication factor 52 */
  CRM_PLL_MULT_53                        = 52, /*!< pll multiplication factor 53 */
  CRM_PLL_MULT_54                        = 53, /*!< pll multiplication factor 54 */
  CRM_PLL_MULT_55                        = 54, /*!< pll multiplication factor 55 */
  CRM_PLL_MULT_56                        = 55, /*!< pll multiplication factor 56 */
  CRM_PLL_MULT_57                        = 56, /*!< pll multiplication factor 57 */
  CRM_PLL_MULT_58                        = 57, /*!< pll multiplication factor 58 */
  CRM_PLL_MULT_59                        = 58, /*!< pll multiplication factor 59 */
  CRM_PLL_MULT_60                        = 59, /*!< pll multiplication factor 60 */
  CRM_PLL_MULT_61                        = 60, /*!< pll multiplication factor 61 */
  CRM_PLL_MULT_62                        = 61, /*!< pll multiplication factor 62 */
  CRM_PLL_MULT_63                        = 62, /*!< pll multiplication factor 63 */
  CRM_PLL_MULT_64                        = 63  /*!< pll multiplication factor 64 */
} crm_pll_mult_type;

/**
  * @brief crm pll clock source
  */
typedef enum
{
  CRM_PLL_SOURCE_HICK                    = 0x00, /*!< high speed internal clock as pll reference clock source */
  CRM_PLL_SOURCE_HEXT                    = 0x01, /*!< high speed external crystal as pll reference clock source */
  CRM_PLL_SOURCE_HEXT_DIV                = 0x02  /*!< high speed external crystal div as pll reference clock source */
} crm_pll_clock_source_type;

/**
  * @brief crm pll clock output range
  */
typedef enum
{
  CRM_PLL_OUTPUT_RANGE_LE72MHZ           = 0x00, /*!< pll clock output range less than or equal to 72mhz */
  CRM_PLL_OUTPUT_RANGE_GT72MHZ           = 0x01  /*!< pll clock output range greater than 72mhz */
} crm_pll_output_range_type;

/**
  * @brief crm clock source
  */
typedef enum
{
  CRM_CLOCK_SOURCE_HICK                  = 0x00, /*!< high speed internal clock */
  CRM_CLOCK_SOURCE_HEXT                  = 0x01, /*!< high speed external crystal */
  CRM_CLOCK_SOURCE_PLL                   = 0x02, /*!< phase locking loop */
  CRM_CLOCK_SOURCE_LEXT                  = 0x03, /*!< low speed external crystal */
  CRM_CLOCK_SOURCE_LICK                  = 0x04  /*!< low speed internal clock */
} crm_clock_source_type;

/**
  * @brief crm ahb division
  */
typedef enum
{
  CRM_AHB_DIV_1                          = 0x00, /*!< sclk div1 to ahbclk */
  CRM_AHB_DIV_2                          = 0x08, /*!< sclk div2 to ahbclk */
  CRM_AHB_DIV_4                          = 0x09, /*!< sclk div4 to ahbclk */
  CRM_AHB_DIV_8                          = 0x0A, /*!< sclk div8 to ahbclk */
  CRM_AHB_DIV_16                         = 0x0B, /*!< sclk div16 to ahbclk */
  CRM_AHB_DIV_64                         = 0x0C, /*!< sclk div64 to ahbclk */
  CRM_AHB_DIV_128                        = 0x0D, /*!< sclk div128 to ahbclk */
  CRM_AHB_DIV_256                        = 0x0E, /*!< sclk div256 to ahbclk */
  CRM_AHB_DIV_512                        = 0x0F  /*!< sclk div512 to ahbclk */
} crm_ahb_div_type;

/**
  * @brief crm apb1 division
  */
typedef enum
{
  CRM_APB1_DIV_1                         = 0x00, /*!< ahbclk div1 to apb1clk */
  CRM_APB1_DIV_2                         = 0x04, /*!< ahbclk div2 to apb1clk */
  CRM_APB1_DIV_4                         = 0x05, /*!< ahbclk div4 to apb1clk */
  CRM_APB1_DIV_8                         = 0x06, /*!< ahbclk div8 to apb1clk */
  CRM_APB1_DIV_16                        = 0x07  /*!< ahbclk div16 to apb1clk */
} crm_apb1_div_type;

/**
  * @brief crm apb2 division
  */
typedef enum
{
  CRM_APB2_DIV_1                         = 0x00, /*!< ahbclk div1 to apb2clk */
  CRM_APB2_DIV_2                         = 0x04, /*!< ahbclk div2 to apb2clk */
  CRM_APB2_DIV_4                         = 0x05, /*!< ahbclk div4 to apb2clk */
  CRM_APB2_DIV_8                         = 0x06, /*!< ahbclk div8 to apb2clk */
  CRM_APB2_DIV_16                        = 0x07  /*!< ahbclk div16 to apb2clk */
} crm_apb2_div_type;

/**
  * @brief crm adc division
  */
typedef enum
{
  CRM_ADC_DIV_2                          = 0x00, /*!< apb2clk div2 to adcclk */
  CRM_ADC_DIV_4                          = 0x01, /*!< apb2clk div4 to adcclk */
  CRM_ADC_DIV_6                          = 0x02, /*!< apb2clk div6 to adcclk */
  CRM_ADC_DIV_8                          = 0x03, /*!< apb2clk div8 to adcclk */
  CRM_ADC_DIV_12                         = 0x05, /*!< apb2clk div12 to adcclk */
  CRM_ADC_DIV_16                         = 0x07  /*!< apb2clk div16 to adcclk */
} crm_adc_div_type;

/**
  * @brief crm usb division
  */
typedef enum
{
  CRM_USB_DIV_1_5                        = 0x00, /*!< pllclk div1.5 to usbclk */
  CRM_USB_DIV_1                          = 0x01, /*!< pllclk div1 to usbclk */
  CRM_USB_DIV_2_5                        = 0x02, /*!< pllclk div2.5 to usbclk */
  CRM_USB_DIV_2                          = 0x03, /*!< pllclk div2 to usbclk */
  CRM_USB_DIV_3_5                        = 0x04, /*!< pllclk div3.5 to usbclk */
  CRM_USB_DIV_3                          = 0x05, /*!< pllclk div3 to usbclk */
  CRM_USB_DIV_4                          = 0x06  /*!< pllclk div4 to usbclk */
} crm_usb_div_type;

/**
  * @brief crm rtc clock
  */
typedef enum
{
  CRM_RTC_CLOCK_NOCLK                    = 0x00, /*!< no clock as rtc clock source */
  CRM_RTC_CLOCK_LEXT                     = 0x01, /*!< low speed external crystal as rtc clock source */
  CRM_RTC_CLOCK_LICK                     = 0x02, /*!< low speed internal clock as rtc clock source */
  CRM_RTC_CLOCK_HEXT_DIV                 = 0x03  /*!< high speed external crystal div as rtc clock source */
} crm_rtc_clock_type;

/**
  * @brief crm hick 48mhz division
  */
typedef enum
{
  CRM_HICK48_DIV6                        = 0x00, /*!< high speed internal clock (48 mhz) div6 */
  CRM_HICK48_NODIV                       = 0x01  /*!< high speed internal clock (48 mhz) no div */
} crm_hick_div_6_type;

/**
  * @brief crm hext division
  */
typedef enum
{
  CRM_HEXT_DIV_2                         = 0x00, /*!< high speed external crystal div2 */
  CRM_HEXT_DIV_3                         = 0x01, /*!< high speed external crystal div3 */
  CRM_HEXT_DIV_4                         = 0x02, /*!< high speed external crystal div4 */
  CRM_HEXT_DIV_5                         = 0x03  /*!< high speed external crystal div5 */
} crm_hext_div_type;

/**
  * @brief crm sclk select
  */
typedef enum
{
  CRM_SCLK_HICK                          = 0x00, /*!< select high speed internal clock as sclk */
  CRM_SCLK_HEXT                          = 0x01, /*!< select high speed external crystal as sclk */
  CRM_SCLK_PLL                           = 0x02  /*!< select phase locking loop clock as sclk */
} crm_sclk_type;

/**
  * @brief crm clkout select
  */
typedef enum
{
  CRM_CLKOUT_NOCLK                       = 0x00, /*!< output no clock to clkout pin */
  CRM_CLKOUT_LICK                        = 0x02, /*!< output low speed internal clock to clkout pin */
  CRM_CLKOUT_LEXT                        = 0x03, /*!< output low speed external crystal to clkout pin */
  CRM_CLKOUT_SCLK                        = 0x04, /*!< output system clock to clkout pin */
  CRM_CLKOUT_HICK                        = 0x05, /*!< output high speed internal clock to clkout pin */
  CRM_CLKOUT_HEXT                        = 0x06, /*!< output high speed external crystal to clkout pin */
  CRM_CLKOUT_PLL_DIV_2                   = 0x07, /*!< output phase locking loop clock div2 to clkout pin */
  CRM_CLKOUT_PLL_DIV_4                   = 0x0C, /*!< output phase locking loop clock div4 to clkout pin */
  CRM_CLKOUT_USB                         = 0x0D, /*!< output usbclk to clkout pin */
  CRM_CLKOUT_ADC                         = 0x0E  /*!< output adcclk to clkout pin */
} crm_clkout_select_type;

/**
  * @brief crm clkout division
  */
typedef enum
{
  CRM_CLKOUT_DIV_1                       = 0x00, /*!< clkout div1 */
  CRM_CLKOUT_DIV_2                       = 0x08, /*!< clkout div2 */
  CRM_CLKOUT_DIV_4                       = 0x09, /*!< clkout div4 */
  CRM_CLKOUT_DIV_8                       = 0x0A, /*!< clkout div8 */
  CRM_CLKOUT_DIV_16                      = 0x0B, /*!< clkout div16 */
  CRM_CLKOUT_DIV_64                      = 0x0C, /*!< clkout div64 */
  CRM_CLKOUT_DIV_128                     = 0x0D, /*!< clkout div128 */
  CRM_CLKOUT_DIV_256                     = 0x0E, /*!< clkout div256 */
  CRM_CLKOUT_DIV_512                     = 0x0F  /*!< clkout div512 */
} crm_clkout_div_type;

/**
  * @brief crm auto step mode
  */
typedef enum
{
  CRM_AUTO_STEP_MODE_DISABLE             = 0x00, /*!< disable auto step mode */
  CRM_AUTO_STEP_MODE_ENABLE              = 0x03  /*!< enable auto step mode */
} crm_auto_step_mode_type;

/**
  * @brief crm usbdev interrupt remap
  */
typedef enum
{
  CRM_USB_INT19_INT20                    = 0x00, /*!< usb high and low priority irq numer use 19 and 20 */
  CRM_USB_INT73_INT74                    = 0x01  /*!< usb high and low priority irq numer use 73 and 74 */
} crm_usb_int_map_type;

/**
  * @brief crm usb 48 mhz clock source select
  */
typedef enum
{
  CRM_USB_CLOCK_SOURCE_PLL               = 0x00, /*!< select phase locking loop clock as usb clock source */
  CRM_USB_CLOCK_SOURCE_HICK              = 0x01  /*!< select high speed internal clock as usb clock source */
} crm_usb_clock_source_type;

/**
  * @brief crm hick as system clock frequency select
  */
typedef enum
{
  CRM_HICK_SCLK_8MHZ                     = 0x00, /*!< fixed 8 mhz when hick is selected as sclk */
  CRM_HICK_SCLK_48MHZ                    = 0x01  /*!< 8 mhz or 48 mhz depend on hickdiv when hick is selected as sclk */
} crm_hick_sclk_frequency_type;

/**
  * @brief crm emac output pulse width
  */
typedef enum
{
  CRM_EMAC_PULSE_125MS                   = 0x00, /*!< emac output pulse width 125ms */
  CRM_EMAC_PULSE_1SCLK                   = 0x01  /*!< emac output pulse width 1 system clock */
} crm_emac_output_pulse_type;

/**
  * @brief crm clocks freqency structure
  */
typedef struct
{
  uint32_t sclk_freq; /*!< system clock frequency */
  uint32_t ahb_freq;  /*!< ahb bus clock frequency */
  uint32_t apb2_freq; /*!< apb2 bus clock frequency */
  uint32_t apb1_freq; /*!< apb1 bus clock frequency */
  uint32_t adc_freq;  /*!< adc clock frequency */
} crm_clocks_freq_type;

/**
  * @brief type define crm register all
  */
typedef struct
{
  /**
    * @brief crm ctrl register, offset:0x00
    */
  union
  {
    __IO uint32_t ctrl;
    struct
    {
      __IO uint32_t hicken               : 1; /* [0] */
      __IO uint32_t hickstbl             : 1; /* [1] */
      __IO uint32_t hicktrim             : 6; /* [7:2] */
      __IO uint32_t hickcal              : 8; /* [15:8] */
      __IO uint32_t hexten               : 1; /* [16] */
      __IO uint32_t hextstbl             : 1; /* [17] */
      __IO uint32_t hextbyps             : 1; /* [18] */
      __IO uint32_t cfden                : 1; /* [19] */
      __IO uint32_t reserved1            : 4; /* [23:20] */
      __IO uint32_t pllen                : 1; /* [24] */
      __IO uint32_t pllstbl              : 1; /* [25] */
      __IO uint32_t reserved2            : 6; /* [31:26] */
    } ctrl_bit;
  };

  /**
    * @brief crm cfg register, offset:0x04
    */
  union
  {
    __IO uint32_t cfg;
    struct
    {
      __IO uint32_t sclksel              : 2; /* [1:0] */
      __IO uint32_t sclksts              : 2; /* [3:2] */
      __IO uint32_t ahbdiv               : 4; /* [7:4] */
      __IO uint32_t apb1div              : 3; /* [10:8] */
      __IO uint32_t apb2div              : 3; /* [13:11] */
      __IO uint32_t adcdiv_l             : 2; /* [15:14] */
      __IO uint32_t pllrcs               : 1; /* [16] */
      __IO uint32_t pllhextdiv           : 1; /* [17] */
      __IO uint32_t pllmult_l            : 4; /* [21:18] */
      __IO uint32_t usbdiv_l             : 2; /* [23:22] */
      __IO uint32_t clkout_sel           : 3; /* [26:24] */
      __IO uint32_t usbdiv_h             : 1; /* [27] */
      __IO uint32_t adcdiv_h             : 1; /* [28] */
      __IO uint32_t pllmult_h            : 2; /* [30:29] */
      __IO uint32_t pllrange             : 1; /* [31] */
    } cfg_bit;
  };

  /**
    * @brief crm clkint register, offset:0x08
    */
  union
  {
    __IO uint32_t clkint;
    struct
    {
      __IO uint32_t lickstblf            : 1; /* [0] */
      __IO uint32_t lextstblf            : 1; /* [1] */
      __IO uint32_t hickstblf            : 1; /* [2] */
      __IO uint32_t hextstblf            : 1; /* [3] */
      __IO uint32_t pllstblf             : 1; /* [4] */
      __IO uint32_t reserved1            : 2; /* [6:5] */
      __IO uint32_t cfdf                 : 1; /* [7] */
      __IO uint32_t lickstblien          : 1; /* [8] */
      __IO uint32_t lextstblien          : 1; /* [9] */
      __IO uint32_t hickstblien          : 1; /* [10] */
      __IO uint32_t hextstblien          : 1; /* [11] */
      __IO uint32_t pllstblien           : 1; /* [12] */
      __IO uint32_t reserved2            : 3; /* [15:13] */
      __IO uint32_t lickstblfc           : 1; /* [16] */
      __IO uint32_t lextstblfc           : 1; /* [17] */
      __IO uint32_t hickstblfc           : 1; /* [18] */
      __IO uint32_t hextstblfc           : 1; /* [19] */
      __IO uint32_t pllstblfc            : 1; /* [20] */
      __IO uint32_t reserved3            : 2; /* [22:21] */
      __IO uint32_t cfdfc                : 1; /* [23] */
      __IO uint32_t reserved4            : 8; /* [31:24] */
    } clkint_bit;
  };

  /**
    * @brief crm apb2rst register, offset:0x0C
    */
  union
  {
    __IO uint32_t apb2rst;
    struct
    {
      __IO uint32_t iomuxrst             : 1; /* [0] */
      __IO uint32_t exintrst             : 1; /* [1] */
      __IO uint32_t gpioarst             : 1; /* [2] */
      __IO uint32_t gpiobrst             : 1; /* [3] */
      __IO uint32_t gpiocrst             : 1; /* [4] */
      __IO uint32_t gpiodrst             : 1; /* [5] */
      __IO uint32_t gpioerst             : 1; /* [6] */
      __IO uint32_t reserved1            : 2; /* [8:7] */
      __IO uint32_t adc1rst              : 1; /* [9] */
      __IO uint32_t adc2rst              : 1; /* [10] */
      __IO uint32_t tmr1rst              : 1; /* [11] */
      __IO uint32_t spi1rst              : 1; /* [12] */
      __IO uint32_t tmr8rst              : 1; /* [13] */
      __IO uint32_t usart1rst            : 1; /* [14] */
      __IO uint32_t adc3rst              : 1; /* [15] */
      __IO uint32_t reserved2            : 3; /* [18:16] */
      __IO uint32_t tmr9rst              : 1; /* [19] */
      __IO uint32_t tmr10rst             : 1; /* [20] */
      __IO uint32_t tmr11rst             : 1; /* [21] */
      __IO uint32_t accrst               : 1; /* [22] */
      __IO uint32_t i2c3rst              : 1; /* [23] */
      __IO uint32_t usart6rst            : 1; /* [24] */
      __IO uint32_t uart7rst             : 1; /* [25] */
      __IO uint32_t uart8rst             : 1; /* [26] */
      __IO uint32_t reserved3            : 5; /* [31:27] */
    } apb2rst_bit;
  };

  /**
    * @brief crm apb1rst register, offset:0x10
    */
  union
  {
    __IO uint32_t apb1rst;
    struct
    {
      __IO uint32_t tmr2rst              : 1; /* [0] */
      __IO uint32_t tmr3rst              : 1; /* [1] */
      __IO uint32_t tmr4rst              : 1; /* [2] */
      __IO uint32_t tmr5rst              : 1; /* [3] */
      __IO uint32_t tmr6rst              : 1; /* [4] */
      __IO uint32_t tmr7rst              : 1; /* [5] */
      __IO uint32_t tmr12rst             : 1; /* [6] */
      __IO uint32_t tmr13rst             : 1; /* [7] */
      __IO uint32_t tmr14rst             : 1; /* [8] */
      __IO uint32_t reserved1            : 2; /* [10:9] */
      __IO uint32_t wwdtrst              : 1; /* [11] */
      __IO uint32_t reserved2            : 2; /* [13:12] */
      __IO uint32_t spi2rst              : 1; /* [14] */
      __IO uint32_t spi3rst              : 1; /* [15] */
      __IO uint32_t spi4rst              : 1; /* [16] */
      __IO uint32_t usart2rst            : 1; /* [17] */
      __IO uint32_t usart3rst            : 1; /* [18] */
      __IO uint32_t uart4rst             : 1; /* [19] */
      __IO uint32_t uart5rst             : 1; /* [20] */
      __IO uint32_t i2c1rst              : 1; /* [21] */
      __IO uint32_t i2c2rst              : 1; /* [22] */
      __IO uint32_t usbrst               : 1; /* [23] */
      __IO uint32_t reserved3            : 1; /* [24] */
      __IO uint32_t can1rst              : 1; /* [25] */
      __IO uint32_t can2rst              : 1; /* [26] */
      __IO uint32_t bprrst               : 1; /* [27] */
      __IO uint32_t pwcrst               : 1; /* [28] */
      __IO uint32_t dacrst               : 1; /* [29] */
      __IO uint32_t reserved4            : 2; /* [31:30] */
    } apb1rst_bit;
  };

  /**
    * @brief crm ahben register, offset:0x14
    */
  union
  {
    __IO uint32_t ahben;
#if defined (AT32F403Axx)
    struct
    {
      __IO uint32_t dma1en               : 1; /* [0] */
      __IO uint32_t dma2en               : 1; /* [1] */
      __IO uint32_t sramen               : 1; /* [2] */
      __IO uint32_t reserved1            : 1; /* [3] */
      __IO uint32_t flashen              : 1; /* [4] */
      __IO uint32_t reserved2            : 1; /* [5] */
      __IO uint32_t crcen                : 1; /* [6] */
      __IO uint32_t reserved3            : 1; /* [7] */
      __IO uint32_t xmcen                : 1; /* [8] */
      __IO uint32_t reserved4            : 1; /* [9] */
      __IO uint32_t sdio1en              : 1; /* [10] */
      __IO uint32_t sdio2en              : 1; /* [11] */
      __IO uint32_t reserved5            : 20;/* [31:12] */
    } ahben_bit;
#endif
#if defined (AT32F407xx)
    struct
    {
      __IO uint32_t dma1en               : 1; /* [0] */
      __IO uint32_t dma2en               : 1; /* [1] */
      __IO uint32_t sramen               : 1; /* [2] */
      __IO uint32_t reserved1            : 1; /* [3] */
      __IO uint32_t flashen              : 1; /* [4] */
      __IO uint32_t reserved2            : 1; /* [5] */
      __IO uint32_t crcen                : 1; /* [6] */
      __IO uint32_t reserved3            : 1; /* [7] */
      __IO uint32_t xmcen                : 1; /* [8] */
      __IO uint32_t reserved4            : 1; /* [9] */
      __IO uint32_t sdio1en              : 1; /* [10] */
      __IO uint32_t sdio2en              : 1; /* [11] */
      __IO uint32_t reserved5            : 2; /* [13:12] */
      __IO uint32_t emacen               : 1; /* [14] */
      __IO uint32_t emactxen             : 1; /* [15] */
      __IO uint32_t emacrxen             : 1; /* [16] */
      __IO uint32_t reserved6            : 11;/* [27:17] */
      __IO uint32_t emacptpen            : 1; /* [28] */
      __IO uint32_t reserved7            : 3; /* [31:29] */
    } ahben_bit;
#endif
  };

  /**
    * @brief crm apb2en register, offset:0x18
    */
  union
  {
    __IO uint32_t apb2en;
    struct
    {
      __IO uint32_t iomuxen              : 1; /* [0] */
      __IO uint32_t reserved1            : 1; /* [1] */
      __IO uint32_t gpioaen              : 1; /* [2] */
      __IO uint32_t gpioben              : 1; /* [3] */
      __IO uint32_t gpiocen              : 1; /* [4] */
      __IO uint32_t gpioden              : 1; /* [5] */
      __IO uint32_t gpioeen              : 1; /* [6] */
      __IO uint32_t reserved2            : 2; /* [8:7] */
      __IO uint32_t adc1en               : 1; /* [9] */
      __IO uint32_t adc2en               : 1; /* [10] */
      __IO uint32_t tmr1en               : 1; /* [11] */
      __IO uint32_t spi1en               : 1; /* [12] */
      __IO uint32_t tmr8en               : 1; /* [13] */
      __IO uint32_t usart1en             : 1; /* [14] */
      __IO uint32_t adc3en               : 1; /* [15] */
      __IO uint32_t reserved3            : 3; /* [18:16] */
      __IO uint32_t tmr9en               : 1; /* [19] */
      __IO uint32_t tmr10en              : 1; /* [20] */
      __IO uint32_t tmr11en              : 1; /* [21] */
      __IO uint32_t accen                : 1; /* [22] */
      __IO uint32_t i2c3en               : 1; /* [23] */
      __IO uint32_t usart6en             : 1; /* [24] */
      __IO uint32_t uart7en              : 1; /* [25] */
      __IO uint32_t uart8en              : 1; /* [26] */
      __IO uint32_t reserved4            : 5; /* [31:27] */
    } apb2en_bit;
  };

  /**
    * @brief crm apb1en register, offset:0x1C
    */
  union
  {
    __IO uint32_t apb1en;
    struct
    {
      __IO uint32_t tmr2en               : 1; /* [0] */
      __IO uint32_t tmr3en               : 1; /* [1] */
      __IO uint32_t tmr4en               : 1; /* [2] */
      __IO uint32_t tmr5en               : 1; /* [3] */
      __IO uint32_t tmr6en               : 1; /* [4] */
      __IO uint32_t tmr7en               : 1; /* [5] */
      __IO uint32_t tmr12en              : 1; /* [6] */
      __IO uint32_t tmr13en              : 1; /* [7] */
      __IO uint32_t tmr14en              : 1; /* [8] */
      __IO uint32_t reserved1            : 2; /* [10:9] */
      __IO uint32_t wwdten               : 1; /* [11] */
      __IO uint32_t reserved2            : 2; /* [13:12] */
      __IO uint32_t spi2en               : 1; /* [14] */
      __IO uint32_t spi3en               : 1; /* [15] */
      __IO uint32_t spi4en               : 1; /* [16] */
      __IO uint32_t usart2en             : 1; /* [17] */
      __IO uint32_t usart3en             : 1; /* [18] */
      __IO uint32_t uart4en              : 1; /* [19] */
      __IO uint32_t uart5en              : 1; /* [20] */
      __IO uint32_t i2c1en               : 1; /* [21] */
      __IO uint32_t i2c2en               : 1; /* [22] */
      __IO uint32_t usben                : 1; /* [23] */
      __IO uint32_t reserved3            : 1; /* [24] */
      __IO uint32_t can1en               : 1; /* [25] */
      __IO uint32_t can2en               : 1; /* [26] */
      __IO uint32_t bpren                : 1; /* [27] */
      __IO uint32_t pwcen                : 1; /* [28] */
      __IO uint32_t dacen                : 1; /* [29] */
      __IO uint32_t reserved4            : 2; /* [31:30] */
    } apb1en_bit;
  };

  /**
    * @brief crm bpdc register, offset:0x20
    */
  union
  {
    __IO uint32_t bpdc;
    struct
    {
      __IO uint32_t lexten               : 1; /* [0] */
      __IO uint32_t lextstbl             : 1; /* [1] */
      __IO uint32_t lextbyps             : 1; /* [2] */
      __IO uint32_t reserved1            : 5; /* [7:3] */
      __IO uint32_t rtcsel               : 2; /* [9:8] */
      __IO uint32_t reserved2            : 5; /* [14:10] */
      __IO uint32_t rtcen                : 1; /* [15] */
      __IO uint32_t bpdrst               : 1; /* [16] */
      __IO uint32_t reserved3            : 15;/* [31:17] */
    } bpdc_bit;
  };

  /**
    * @brief crm ctrlsts register, offset:0x24
    */
  union
  {
    __IO uint32_t ctrlsts;
    struct
    {
      __IO uint32_t licken               : 1; /* [0] */
      __IO uint32_t lickstbl             : 1; /* [1] */
      __IO uint32_t reserved1            : 22;/* [23:2] */
      __IO uint32_t rstfc                : 1; /* [24] */
      __IO uint32_t reserved2            : 1; /* [25] */
      __IO uint32_t nrstf                : 1; /* [26] */
      __IO uint32_t porrstf              : 1; /* [27] */
      __IO uint32_t swrstf               : 1; /* [28] */
      __IO uint32_t wdtrstf              : 1; /* [29] */
      __IO uint32_t wwdtrstf             : 1; /* [30] */
      __IO uint32_t lprstf               : 1; /* [31] */
    } ctrlsts_bit;
  };

  /**
    * @brief crm ahbrst register, offset:0x28
    */
  union
  {
    __IO uint32_t ahbrst;
#if defined (AT32F407xx)
    struct
    {
      __IO uint32_t reserved1            : 14;/* [13:0] */
      __IO uint32_t emacrst              : 1; /* [14] */
      __IO uint32_t reserved2            : 17;/* [31:15] */
    } ahbrst_bit;
#endif
  };

  /**
    * @brief crm reserved1 register, offset:0x2C
    */
  __IO uint32_t reserved1;

  /**
    * @brief crm misc1 register, offset:0x30
    */
  union
  {
    __IO uint32_t misc1;
    struct
    {
      __IO uint32_t hickcal_key          : 8; /* [7:0] */
      __IO uint32_t reserved1            : 8; /* [15:8] */
      __IO uint32_t clkout_sel           : 1; /* [16] */
      __IO uint32_t reserved2            : 7; /* [23:17] */
      __IO uint32_t usbbufs              : 1; /* [24] */
      __IO uint32_t hickdiv              : 1; /* [25] */
      __IO uint32_t reserved3            : 2; /* [27:26] */
      __IO uint32_t clkoutdiv            : 4; /* [31:28] */
    } misc1_bit;
  };

  /**
    * @brief crm reserved2 register, offset:0x4C~0x34
    */
  __IO uint32_t reserved2[7];

  /**
    * @brief crm misc2 register, offset:0x50
    */
  union
  {
    __IO uint32_t misc2;
    struct
    {
      __IO uint32_t reserved1            : 16;/* [15:0] */
      __IO uint32_t clk_to_tmr           : 1; /* [16] */
      __IO uint32_t reserved2            : 15;/* [31:17] */
    } misc2_bit;
  };

  /**
    * @brief crm misc3 register, offset:0x54
    */
  union
  {
    __IO uint32_t misc3;
    struct
    {
      __IO uint32_t reserved1            : 4; /* [3:0] */
      __IO uint32_t auto_step_en         : 2; /* [5:4] */
      __IO uint32_t reserved2            : 2; /* [7:6] */
      __IO uint32_t hick_to_usb          : 1; /* [8] */
      __IO uint32_t hick_to_sclk         : 1; /* [9] */
      __IO uint32_t reserved3            : 2; /* [11:10] */
      __IO uint32_t hextdiv              : 2; /* [13:12] */
      __IO uint32_t reserved4            : 1; /* [14] */
      __IO uint32_t emac_pps_sel         : 1; /* [15] */
      __IO uint32_t reserved5            : 16;/* [31:16] */
    } misc3_bit;
  };

  /**
    * @brief crm reserved3 register, offset:0x58
    */
  __IO uint32_t reserved3;

  /**
    * @brief crm intmap register, offset:0x5C
    */
  union
  {
    __IO uint32_t intmap;
    struct
    {
      __IO uint32_t usbintmap            : 1; /* [0] */
      __IO uint32_t reserved1            : 31;/* [31:1] */
    } intmap_bit;
  };

} crm_type;

/**
  * @}
  */

#define CRM                              ((crm_type *) CRM_BASE)

/** @defgroup CRM_exported_functions
  * @{
  */

void crm_reset(void);
void crm_lext_bypass(confirm_state new_state);
void crm_hext_bypass(confirm_state new_state);
flag_status crm_flag_get(uint32_t flag);
error_status crm_hext_stable_wait(void);
void crm_hick_clock_trimming_set(uint8_t trim_value);
void crm_hick_clock_calibration_set(uint8_t cali_value);
void crm_periph_clock_enable(crm_periph_clock_type value, confirm_state new_state);
void crm_periph_reset(crm_periph_reset_type value, confirm_state new_state);
void crm_periph_sleep_mode_clock_enable(crm_periph_clock_sleepmd_type value, confirm_state new_state);
void crm_clock_source_enable(crm_clock_source_type source, confirm_state new_state);
void crm_flag_clear(uint32_t flag);
void crm_rtc_clock_select(crm_rtc_clock_type value);
void crm_rtc_clock_enable(confirm_state new_state);
void crm_ahb_div_set(crm_ahb_div_type value);
void crm_apb1_div_set(crm_apb1_div_type value);
void crm_apb2_div_set(crm_apb2_div_type value);
void crm_adc_clock_div_set(crm_adc_div_type div_value);
void crm_usb_clock_div_set(crm_usb_div_type div_value);
void crm_clock_failure_detection_enable(confirm_state new_state);
void crm_battery_powered_domain_reset(confirm_state new_state);
void crm_pll_config(crm_pll_clock_source_type clock_source, crm_pll_mult_type mult_value, crm_pll_output_range_type pll_range);
void crm_sysclk_switch(crm_sclk_type value);
crm_sclk_type crm_sysclk_switch_status_get(void);
void crm_clocks_freq_get(crm_clocks_freq_type *clocks_struct);
void crm_clock_out_set(crm_clkout_select_type clkout);
void crm_interrupt_enable(uint32_t crm_int, confirm_state new_state);
void crm_auto_step_mode_enable(confirm_state new_state);
void crm_usb_interrupt_remapping_set(crm_usb_int_map_type int_remap);
void crm_hick_divider_select(crm_hick_div_6_type value);
void crm_hick_sclk_frequency_select(crm_hick_sclk_frequency_type value);
void crm_usb_clock_source_select(crm_usb_clock_source_type value);
void crm_clkout_to_tmr10_enable(confirm_state new_state);
void crm_hext_clock_div_set(crm_hext_div_type value);
void crm_clkout_div_set(crm_clkout_div_type clkout_div);
#if defined (AT32F407xx)
void crm_emac_output_pulse_set(crm_emac_output_pulse_type width);
#endif

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
