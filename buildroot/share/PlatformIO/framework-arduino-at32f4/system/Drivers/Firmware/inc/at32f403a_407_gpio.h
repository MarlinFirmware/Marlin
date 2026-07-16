/**
  **************************************************************************
  * @file     at32f403a_407_gpio.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 gpio header file
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
#ifndef __AT32F403A_407_GPIO_H
#define __AT32F403A_407_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif


/* includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup GPIO
  * @{
  */

/** @defgroup GPIO_pins_number_definition
  * @{
  */

#define GPIO_PINS_0                      0x0001 /*!< gpio pins number 0 */
#define GPIO_PINS_1                      0x0002 /*!< gpio pins number 1 */
#define GPIO_PINS_2                      0x0004 /*!< gpio pins number 2 */
#define GPIO_PINS_3                      0x0008 /*!< gpio pins number 3 */
#define GPIO_PINS_4                      0x0010 /*!< gpio pins number 4 */
#define GPIO_PINS_5                      0x0020 /*!< gpio pins number 5 */
#define GPIO_PINS_6                      0x0040 /*!< gpio pins number 6 */
#define GPIO_PINS_7                      0x0080 /*!< gpio pins number 7 */
#define GPIO_PINS_8                      0x0100 /*!< gpio pins number 8 */
#define GPIO_PINS_9                      0x0200 /*!< gpio pins number 9 */
#define GPIO_PINS_10                     0x0400 /*!< gpio pins number 10 */
#define GPIO_PINS_11                     0x0800 /*!< gpio pins number 11 */
#define GPIO_PINS_12                     0x1000 /*!< gpio pins number 12 */
#define GPIO_PINS_13                     0x2000 /*!< gpio pins number 13 */
#define GPIO_PINS_14                     0x4000 /*!< gpio pins number 14 */
#define GPIO_PINS_15                     0x8000 /*!< gpio pins number 15 */
#define GPIO_PINS_ALL                    0xFFFF /*!< gpio all pins */

/**
  * @}
  */

#define IOMUX_MAKE_VALUE(reg_offset, bit_addr ,bit_num, bit_val) \
        (uint32_t)(((reg_offset) << 24) | ((bit_addr) << 16) | ((bit_num) << 8) | (bit_val))

/** @defgroup IOMUX_map_definition
  * @brief iomux map definitions
  * @{
  */

#if defined (AT32F403Axx)
#define SPI1_MUX_01                      SPI1_GMUX_0001    /*!< spi1_cs/i2s1_ws(pa15), spi1_sck/i2s1_ck(pb3), spi1_miso(pb4), spi1_mosi/i2s1_sd(pb5), i2s1_mck(pb0) */
#define SPI1_MUX_10                      SPI1_GMUX_0010    /*!< spi1_cs/i2s1_ws(pa4),  spi1_sck/i2s1_ck(pa5), spi1_miso(pa6), spi1_mosi/i2s1_sd(pa7), i2s1_mck(pb6) */
#define SPI1_MUX_11                      SPI1_GMUX_0011    /*!< spi1_cs/i2s1_ws(pa15), spi1_sck/i2s1_ck(pb3), spi1_miso(pb4), spi1_mosi/i2s1_sd(pb5), i2s1_mck(pb6) */
#define I2C1_MUX                         I2C1_GMUX_0001    /*!< i2c1_scl(pb8),      i2c1_sda(pb9) */
#define USART1_MUX                       USART1_GMUX_0001  /*!< usart1_tx(pb6),     usart1_rx(pb7) */
#define USART2_MUX                       USART2_GMUX_0001  /*!< usart2_tx(pd5),     usart2_rx(pd6),  usart2_ck(pd7),  usart2_cts(pd3),  usart2_rts(pd4) */
#define USART3_MUX_01                    USART3_GMUX_0001  /*!< usart3_tx(pc10),    usart3_rx(pc11), usart3_ck(pc12), usart3_cts(pb13), usart3_rts(pb14) */
#define USART3_MUX_11                    USART3_GMUX_0011  /*!< usart3_tx(pd8),     usart3_rx(pd9),  usart3_ck(pd10), usart3_cts(pd11), usart3_rts(pd12) */
#define TMR1_MUX_01                      TMR1_GMUX_0001    /*!< tmr1_ext(pa12),     tmr1_ch1(pa8),   tmr1_ch2(pa9),   tmr1_ch3(pa10),   tmr1_ch4(pa11), tmr1_brkin(pa6), tmr1_ch1c(pa7), tmr1_ch2c(pb0), tmr1_ch3c(pb1) */
#define TMR1_MUX_11                      TMR1_GMUX_0011    /*!< tmr1_ext(pe7),      tmr1_ch1(pe9),   tmr1_ch2(pe11),  tmr1_ch3(pe13),   tmr1_ch4(pe14), tmr1_brkin(pe15), tmr1_ch1c(pe8), tmr1_ch2c(pe10), tmr1_ch3c(pe12) */
#define TMR2_MUX_01                      TMR2_GMUX_01      /*!< tmr2_ch1_ext(pa15), tmr2_ch2(pb3),   tmr2_ch3(pa2),   tmr2_ch4(pa3) */
#define TMR2_MUX_10                      TMR2_GMUX_10      /*!< tmr2_ch1_ext(pa0),  tmr2_ch2(pa1),   tmr2_ch3(pb10),  tmr2_ch4(pb11) */
#define TMR2_MUX_11                      TMR2_GMUX_11      /*!< tmr2_ch1_ext(pa15), tmr2_ch2(pb3),   tmr2_ch3(pb10),  tmr2_ch4(pb11) */
#define TMR3_MUX_10                      TMR3_GMUX_0010    /*!< tmr3_ch1(pb4),      tmr3_ch2(pb5),   tmr3_ch3(pb0),   tmr3_ch4(pb1) */
#define TMR3_MUX_11                      TMR3_GMUX_0011    /*!< tmr3_ch1(pc6),      tmr3_ch2(pc7),   tmr3_ch3(pc8),   tmr3_ch4(pc9) */
#define TMR4_MUX                         TMR4_GMUX_0001    /*!< tmr4_ch1(pd12),     tmr4_ch2(pd13),  tmr4_ch3(pd14),  tmr4_ch4(pd15) */
#define CAN_MUX_10                       CAN1_GMUX_0010    /*!< can_rx(pb8),        can_tx(pb9) */
#define CAN_MUX_11                       CAN1_GMUX_0011    /*!< can_rx(pd0),        can_tx(pd1) */
#define PD01_MUX                         PD01_GMUX         /*!< pd0/pd1 mapping on osc_in/osc_out */
#define TMR5CH4_MUX                      TMR5CH4_GMUX      /*!< lick connected to tmr5_ch4 input capture for calibration */
#define ADC1_ETP_MUX                     ADC1_ETP_GMUX     /*!< adc1 external trigger preempted conversion muxing */
#define ADC1_ETO_MUX                     ADC1_ETO_GMUX     /*!< adc1 external trigger ordinary conversion muxing */
#define ADC2_ETP_MUX                     ADC2_ETP_GMUX     /*!< adc2 external trigger preempted conversion muxing */
#define ADC2_ETO_MUX                     ADC2_ETO_GMUX     /*!< adc2 external trigger ordinary conversion muxing */
#define SWJTAG_MUX_001                   SWJTAG_GMUX_001   /*!< full swj enabled  (jtag-dp  +  sw-dp)  but without jtrst */
#define SWJTAG_MUX_010                   SWJTAG_GMUX_010   /*!< jtag-dp disabled and sw-dp enabled */
#define SWJTAG_MUX_100                   SWJTAG_GMUX_100   /*!< full swj disabled  (jtag-dp  +  sw-dp) */
#endif
#if defined (AT32F407xx)
#define SPI1_MUX_01                      SPI1_GMUX_0001    /*!< spi1_cs/i2s1_ws(pa15), spi1_sck/i2s1_ck(pb3), spi1_miso(pb4), spi1_mosi/i2s1_sd(pb5), i2s1_mck(pb0) */
#define SPI1_MUX_10                      SPI1_GMUX_0010    /*!< spi1_cs/i2s1_ws(pa4),  spi1_sck/i2s1_ck(pa5), spi1_miso(pa6), spi1_mosi/i2s1_sd(pa7), i2s1_mck(pb6) */
#define SPI1_MUX_11                      SPI1_GMUX_0011    /*!< spi1_cs/i2s1_ws(pa15), spi1_sck/i2s1_ck(pb3), spi1_miso(pb4), spi1_mosi/i2s1_sd(pb5), i2s1_mck(pb6) */
#define I2C1_MUX                         I2C1_GMUX_0001    /*!< i2c1_scl(pb8),      i2c1_sda(pb9) */
#define USART1_MUX                       USART1_GMUX_0001  /*!< usart1_tx(pb6),     usart1_rx(pb7) */
#define USART2_MUX                       USART2_GMUX_0001  /*!< usart2_tx(pd5),     usart2_rx(pd6),  usart2_ck(pd7),  usart2_cts(pd3),  usart2_rts(pd4) */
#define USART3_MUX_01                    USART3_GMUX_0001  /*!< usart3_tx(pc10),    usart3_rx(pc11), usart3_ck(pc12), usart3_cts(pb13), usart3_rts(pb14) */
#define USART3_MUX_11                    USART3_GMUX_0011  /*!< usart3_tx(pd8),     usart3_rx(pd9),  usart3_ck(pd10), usart3_cts(pd11), usart3_rts(pd12) */
#define TMR1_MUX_01                      TMR1_GMUX_0001    /*!< tmr1_ext(pa12),     tmr1_ch1(pa8),   tmr1_ch2(pa9),   tmr1_ch3(pa10),   tmr1_ch4(pa11), tmr1_brkin(pa6), tmr1_ch1c(pa7), tmr1_ch2c(pb0), tmr1_ch3c(pb1) */
#define TMR1_MUX_11                      TMR1_GMUX_0011    /*!< tmr1_ext(pe7),      tmr1_ch1(pe9),   tmr1_ch2(pe11),  tmr1_ch3(pe13),   tmr1_ch4(pe14), tmr1_brkin(pe15), tmr1_ch1c(pe8), tmr1_ch2c(pe10), tmr1_ch3c(pe12) */
#define TMR2_MUX_01                      TMR2_GMUX_01      /*!< tmr2_ch1_ext(pa15), tmr2_ch2(pb3),   tmr2_ch3(pa2),   tmr2_ch4(pa3) */
#define TMR2_MUX_10                      TMR2_GMUX_10      /*!< tmr2_ch1_ext(pa0),  tmr2_ch2(pa1),   tmr2_ch3(pb10),  tmr2_ch4(pb11) */
#define TMR2_MUX_11                      TMR2_GMUX_11      /*!< tmr2_ch1_ext(pa15), tmr2_ch2(pb3),   tmr2_ch3(pb10),  tmr2_ch4(pb11) */
#define TMR3_MUX_10                      TMR3_GMUX_0010    /*!< tmr3_ch1(pb4),      tmr3_ch2(pb5),   tmr3_ch3(pb0),   tmr3_ch4(pb1) */
#define TMR3_MUX_11                      TMR3_GMUX_0011    /*!< tmr3_ch1(pc6),      tmr3_ch2(pc7),   tmr3_ch3(pc8),   tmr3_ch4(pc9) */
#define TMR4_MUX                         TMR4_GMUX_0001    /*!< tmr4_ch1(pd12),     tmr4_ch2(pd13),  tmr4_ch3(pd14),  tmr4_ch4(pd15) */
#define CAN_MUX_00                       CAN1_GMUX_0000    /*!< can_rx(pa11),       can_tx(pa12) */
#define CAN_MUX_10                       CAN1_GMUX_0010    /*!< can_rx(pb8),        can_tx(pb9) */
#define CAN_MUX_11                       CAN1_GMUX_0011    /*!< can_rx(pd0),        can_tx(pd1) */
#define PD01_MUX                         PD01_GMUX         /*!< pd0/pd1 mapping on osc_in/osc_out */
#define TMR5CH4_MUX                      TMR5CH4_GMUX      /*!< lick connected to tmr5_ch4 input capture for calibration */
#define ADC1_ETP_MUX                     ADC1_ETP_GMUX     /*!< adc1 external trigger preempted conversion muxing */
#define ADC1_ETO_MUX                     ADC1_ETO_GMUX     /*!< adc1 external trigger ordinary conversion muxing */
#define ADC2_ETP_MUX                     ADC2_ETP_GMUX     /*!< adc2 external trigger preempted conversion muxing */
#define ADC2_ETO_MUX                     ADC2_ETO_GMUX     /*!< adc2 external trigger ordinary conversion muxing */
#define EMAC_MUX                         EMAC_GMUX_01      /*!< rx_dv/crs_dv(pd8), rxd0(pd9), rxd1(pd10), rxd2(pd11), rxd3(pd12) */
#define CAN2_MUX                         CAN2_GMUX_0001    /*!< can2_rx(pb5), can2_tx(pb6) */
#define MII_RMII_SEL_MUX                 MII_RMII_SEL_GMUX /*!< mii or rmii selection */
#define SWJTAG_MUX_001                   SWJTAG_GMUX_001   /*!< full swj enabled  (jtag-dp  +  sw-dp)  but without jtrst */
#define SWJTAG_MUX_010                   SWJTAG_GMUX_010   /*!< jtag-dp disabled and sw-dp enabled */
#define SWJTAG_MUX_100                   SWJTAG_GMUX_100   /*!< full swj disabled  (jtag-dp  +  sw-dp) */
#define SPI3_MUX                         SPI3_GMUX_0001    /*!< spi3_cs/i2s3_ws(pa4), spi3_sck/i2s3_ck(pc10), spi3_miso(pc11), spi3_mosi/i2s3_sd(pc12), i2s3_mck(pc7) */
#define TMR2ITR1_MUX                     TMR2ITR1_GMAP_10  /*!< tmr2 internal trigger 1 mux remapping */
#define PTP_PPS_MUX                      PTP_PPS_GMUX      /*!< ethernet ptp pps mux function remapping */
#endif

/**
  * @}
  */

/** @defgroup IOMUX_map2_definition
  * @brief iomux map2 definitions
  * @{
  */

#define TMR9_MUX                         TMR9_GMUX          /*!< tmr9_ch1(pe5),   tmr9_ch2(pe6) */
#define XMC_NADV_MUX                     XMC_NADV_GMUX      /*!< xmc_nadv not used */
#define SPI4_MUX                         SPI4_GMUX_0001     /*!< spi4_cs/i2s4_ws(pe12), spi4_sck/i2s4_ck(pe11), spi4_miso(pe13), spi4_mosi/i2s4_sd(pe14), i2s4_mck(pc8) */
#define I2C3_MUX                         I2C3_GMUX_0001     /*!< i2c3_scl(pa8), i2c3_sda(pb4) */
#define SDIO2_MUX01                      SDIO2_GMUX_0001    /*!< sdio2_ck(pc4), sdio2_cmd(pc5), sdio2_d0(pa4), sdio2_d1(pa5), sdio2_d2(pa6), sdio2_d3(pa7) */
#define SDIO2_MUX10                      SDIO2_GMUX_0010    /*!< sdio2_ck(pa2), sdio2_cmd(pa3), sdio2_d0(pc0), sdio2_d1(pc1), sdio2_d2(pc2), sdio2_d3(pc3), sdio2_d4(pa4), sdio2_d5(pa5), sdio2_d6(pa6), sdio2_d7(pa7) */
#define SDIO2_MUX11                      SDIO2_GMUX_0011    /*!< sdio2_ck(pa2), sdio2_cmd(pa3), sdio2_d0(pa4), sdio2_d1(pa5), sdio2_d2(pa6), sdio2_d3(pa7) */
#define EXT_SPIM_EN_MUX                  EXT_SPIM_GMUX_1000 /*!< enable external spi-flash interface */

/**
  * @}
  */

/** @defgroup IOMUX_map3_definition
  * @brief iomux map3 definitions
  * @{
  */

#define TMR9_GMUX                        IOMUX_MAKE_VALUE(0x20, 0, 4, 0x01) /*!< tmr9_ch1(pe5),  tmr9_ch2(pe6) */

/**
  * @}
  */

/** @defgroup IOMUX_map4_definition
  * @brief iomux map4 definitions
  * @{
  */

#define TMR1_GMUX_0001                   IOMUX_MAKE_VALUE(0x24, 0, 4, 0x01)  /*!< tmr1_ext(pa12), tmr1_ch1(pa8), tmr1_ch2(pa9), tmr1_ch3(pa10), tmr1_ch4(pa11), tmr1_brkin(pa6), tmr1_ch1c(pa7), tmr1_ch2c(pb0), tmr1_ch3c(pb1) */
#define TMR1_GMUX_0011                   IOMUX_MAKE_VALUE(0x24, 0, 4, 0x03)  /*!< tmr1_ext(pe7),  tmr1_ch1(pe9), tmr1_ch2(pe11), tmr1_ch3(pe13), tmr1_ch4(pe14), tmr1_brkin(pe15), tmr1_ch1c(pe8), tmr1_ch2c(pe10), tmr1_ch3c(pe12) */
#define TMR2_GMUX_01                     IOMUX_MAKE_VALUE(0x24, 4, 2, 0x01)  /*!< tmr2_ch1_ext(pa15), tmr2_ch2(pb3), tmr2_ch3(pa2),  tmr2_ch4(pa3) */
#define TMR2_GMUX_10                     IOMUX_MAKE_VALUE(0x24, 4, 2, 0x02)  /*!< tmr2_ch1_ext(pa0),  tmr2_ch2(pa1), tmr2_ch3(pb10), tmr2_ch4(pb11) */
#define TMR2_GMUX_11                     IOMUX_MAKE_VALUE(0x24, 4, 2, 0x03)  /*!< tmr2_ch1_ext(pa15), tmr2_ch2(pb3), tmr2_ch3(pb10), tmr2_ch4(pb11) */
#define TMR2ITR1_GMUX_10                 IOMUX_MAKE_VALUE(0x24, 6, 2, 0x02)  /*!< ethernet ptp as input to tmr2_int.1 */
#define TMR2ITR1_GMUX_11                 IOMUX_MAKE_VALUE(0x24, 6, 2, 0x03)  /*!< usbdev sof as input to tmr2_int.1 */
#define TMR3_GMUX_0010                   IOMUX_MAKE_VALUE(0x24, 8, 4, 0x02)  /*!< tmr3_ch1(pb4),  tmr3_ch2(pb5),  tmr3_ch3(pb0),  tmr3_ch4(pb1) */
#define TMR3_GMUX_0011                   IOMUX_MAKE_VALUE(0x24, 8, 4, 0x03)  /*!< tmr3_ch1(pc6),  tmr3_ch2(pc7),  tmr3_ch3(pc8),  tmr3_ch4(pc9) */
#define TMR4_GMUX_0001                   IOMUX_MAKE_VALUE(0x24, 12, 4, 0x01) /*!< tmr4_ch1(pd12), tmr4_ch2(pd13), tmr4_ch3(pd14), tmr4_ch4(pd15) */
#define TMR5CH4_GMUX                     IOMUX_MAKE_VALUE(0x24, 19, 1, 0x01) /*!< lick connected to tmr5_ch4 input capture for calibration */

/**
  * @}
  */

/** @defgroup IOMUX_map5_definition
  * @brief iomux map5 definitions
  * @{
  */

#define UART5_GMUX_0001                  IOMUX_MAKE_VALUE(0x28, 0, 4, 0x01)  /*!< uart5_tx(pb9), uart5_rx(pb8) */
#define I2C1_GMUX_0001                   IOMUX_MAKE_VALUE(0x28, 4, 4, 0x01)  /*!< i2c1_scl(pb8), i2c1_sda(pb9) */
#define I2C3_GMUX_0001                   IOMUX_MAKE_VALUE(0x28, 12, 4, 0x01) /*!< i2c3_scl(pa8), i2c3_sda(pb4) */
#define SPI1_GMUX_0001                   IOMUX_MAKE_VALUE(0x28, 16, 4, 0x01) /*!< spi1_cs/i2s1_ws(pa15), spi1_sck/i2s1_ck(pb3), spi1_miso(pb4), spi1_mosi/i2s1_sd(pb5), i2s1_mck(pb0) */
#define SPI1_GMUX_0010                   IOMUX_MAKE_VALUE(0x28, 16, 4, 0x02) /*!< spi1_cs/i2s1_ws(pa4),  spi1_sck/i2s1_ck(pa5), spi1_miso(pa6), spi1_mosi/i2s1_sd(pa7), i2s1_mck(pb6) */
#define SPI1_GMUX_0011                   IOMUX_MAKE_VALUE(0x28, 16, 4, 0x03) /*!< spi1_cs/i2s1_ws(pa15), spi1_sck/i2s1_ck(pb3), spi1_miso(pb4), spi1_mosi/i2s1_sd(pb5), i2s1_mck(pb6) */
#define SPI2_GMUX_0001                   IOMUX_MAKE_VALUE(0x28, 20, 4, 0x01) /*!< i2s2_mck(pa3) */
#define SPI2_GMUX_0010                   IOMUX_MAKE_VALUE(0x28, 20, 4, 0x02) /*!< i2s2_mck(pa6) */
#define SPI3_GMUX_0001                   IOMUX_MAKE_VALUE(0x28, 24, 4, 0x01) /*!< spi3_cs/i2s3_ws(pa4),  spi3_sck/i2s3_ck(pc10), spi3_miso(pc11), spi3_mosi/i2s3_sd(pc12), i2s3_mck(pc7) */
#define SPI3_GMUX_0010                   IOMUX_MAKE_VALUE(0x28, 24, 4, 0x02) /*!< spi3_cs/i2s3_ws(pa15), spi3_sck/i2s3_ck(pb3),  spi3_miso(pb4),  spi3_mosi/i2s3_sd(pb5),  i2s3_mck(pb10) */
#define SPI3_GMUX_0011                   IOMUX_MAKE_VALUE(0x28, 24, 4, 0x03) /*!< spi3_cs/i2s3_ws(pa4),  spi3_sck/i2s3_ck(pc10), spi3_miso(pc11), spi3_mosi/i2s3_sd(pc12), i2s3_mck(pb10) */
#define SPI4_GMUX_0001                   IOMUX_MAKE_VALUE(0x28, 28, 4, 0x01) /*!< spi4_cs/i2s4_ws(pe12), spi4_sck/i2s4_ck(pe11), spi4_miso(pe13), spi4_mosi/i2s4_sd(pe14), i2s4_mck(pc8) */
#define SPI4_GMUX_0010                   IOMUX_MAKE_VALUE(0x28, 28, 4, 0x02) /*!< spi4_cs/i2s4_ws(pb6),  spi4_sck/i2s4_ck(pb7),  spi4_miso(pb8),  spi4_mosi/i2s4_sd(pb8),  i2s4_mck(pc8) */
#define SPI4_GMUX_0011                   IOMUX_MAKE_VALUE(0x28, 28, 4, 0x03) /*!< spi4_cs/i2s4_ws(pb6),  spi4_sck/i2s4_ck(pb7),  spi4_miso(pb8),  spi4_mosi/i2s4_sd(pb8),  i2s4_mck(pa10) */

/**
  * @}
  */

/** @defgroup IOMUX_map6_definition
  * @brief iomux map6 definitions
  * @{
  */

#define CAN1_GMUX_0010                   IOMUX_MAKE_VALUE(0x2C, 0, 4, 0x02)  /*!< can_rx(pb8),   can_tx(pb9) */
#define CAN1_GMUX_0011                   IOMUX_MAKE_VALUE(0x2C, 0, 4, 0x03)  /*!< can_rx(pd0),   can_tx(pd1) */
#define CAN2_GMUX_0001                   IOMUX_MAKE_VALUE(0x2C, 4, 4, 0x01)  /*!< can2_rx(pb5),  can2_tx(pb6) */
#define SDIO2_GMUX_0001                  IOMUX_MAKE_VALUE(0x2C, 12, 4, 0x01) /*!< sdio2_ck(pc4), sdio2_cmd(pc5), sdio2_d0(pa4), sdio2_d1(pa5), sdio2_d2(pa6), sdio2_d3(pa7) */
#define SDIO2_GMUX_0010                  IOMUX_MAKE_VALUE(0x2C, 12, 4, 0x02) /*!< sdio2_ck(pa2), sdio2_cmd(pa3), sdio2_d0(pc0), sdio2_d1(pc1), sdio2_d2(pc2), sdio2_d3(pc3), sdio2_d4(pa4), sdio2_d5(pa5), sdio2_d6(pa6), sdio2_d7(pa7) */
#define SDIO2_GMUX_0011                  IOMUX_MAKE_VALUE(0x2C, 12, 4, 0x03) /*!< sdio2_ck(pa2), sdio2_cmd(pa3), sdio2_d0(pa4), sdio2_d1(pa5), sdio2_d2(pa6), sdio2_d3(pa7) */
#define USART1_GMUX_0001                 IOMUX_MAKE_VALUE(0x2C, 16, 4, 0x01) /*!< usart1_tx(pb6),  usart1_rx(pb7) */
#define USART2_GMUX_0001                 IOMUX_MAKE_VALUE(0x2C, 20, 4, 0x01) /*!< usart2_tx(pd5),  usart2_rx(pd6),  usart2_ck(pd7),  usart2_cts(pd3),  usart2_rts(pd4) */
#define USART3_GMUX_0001                 IOMUX_MAKE_VALUE(0x2C, 24, 4, 0x01) /*!< usart3_tx(pc10), usart3_rx(pc11), usart3_ck(pc12), usart3_cts(pb13), usart3_rts(pb14) */
#define USART3_GMUX_0011                 IOMUX_MAKE_VALUE(0x2C, 24, 4, 0x03) /*!< usart3_tx(pd8),  usart3_rx(pd9),  usart3_ck(pd10), usart3_cts(pd11), usart3_rts(pd12) */
#define UART4_GMUX_0010                  IOMUX_MAKE_VALUE(0x2C, 28, 4, 0x02) /*!< uart4_tx(pa0),   uart4_rx(pa1) */

/**
  * @}
  */

/** @defgroup IOMUX_map7_definition
  * @brief iomux map7 definitions
  * @{
  */

#define EXT_SPIM_GMUX_1000               IOMUX_MAKE_VALUE(0x30, 0, 4, 0x08)  /*!< spim_sck(pb1), spim_cs(pa8), spim_io0(pa11), spim_io1(pa12), spim_io2(pb7), spim_sio3(pb6) */
#define EXT_SPIM_GMUX_1001               IOMUX_MAKE_VALUE(0x30, 0, 4, 0x09)  /*!< spim_sck(pb1), spim_cs(pa8), spim_io0(pb10), spim_io1(pb11), spim_io2(pb7), spim_sio3(pb6) */
#define ADC1_ETP_GMUX                    IOMUX_MAKE_VALUE(0x30, 4, 1, 0x01)  /*!< adc1 external trigger preempted conversion muxing */
#define ADC1_ETO_GMUX                    IOMUX_MAKE_VALUE(0x30, 5, 1, 0x01)  /*!< adc1 external trigger ordinary conversion muxing */
#define ADC2_ETP_GMUX                    IOMUX_MAKE_VALUE(0x30, 8, 1, 0x01)  /*!< adc2 external trigger preempted conversion muxing */
#define ADC2_ETO_GMUX                    IOMUX_MAKE_VALUE(0x30, 9, 1, 0x01)  /*!< adc2 external trigger ordinary conversion muxing */
#define SWJTAG_GMUX_001                  IOMUX_MAKE_VALUE(0x30, 16, 3, 0x01) /*!< full swj enabled  (jtag-dp  +  sw-dp)  but without jtrst */
#define SWJTAG_GMUX_010                  IOMUX_MAKE_VALUE(0x30, 16, 3, 0x02) /*!< jtag-dp disabled and sw-dp enabled */
#define SWJTAG_GMUX_100                  IOMUX_MAKE_VALUE(0x30, 16, 3, 0x04) /*!< full swj disabled  (jtag-dp  +  sw-dp) */
#define PD01_GMUX                        IOMUX_MAKE_VALUE(0x30, 20, 1, 0x01) /*!< pd0/pd1 mapping on osc_in/osc_out */
#define XMC_GMUX_001                     IOMUX_MAKE_VALUE(0x30, 24, 3, 0x01) /*!< xmc_nwe(pd2), xmc_d0(pb14), xmc_d1(pc6), xmc_d2(pc11), xmc_d3(pc12), xmc_d4(pa2), xmc_d5(pa3), xmc_d6(pa4), xmc_d7(pa5), xmc_d13(pb12), xmc_noe(pc5) */
#define XMC_GMUX_010                     IOMUX_MAKE_VALUE(0x30, 24, 3, 0x02) /*!< xmc_nwe(pc2), xmc_d0(pb14), xmc_d1(pc6), xmc_d2(pc11), xmc_d3(pc12), xmc_d4(pa2), xmc_d5(pa3), xmc_d6(pa4), xmc_d7(pa5), xmc_d13(pb12), xmc_noe(pc5) */
#define XMC_NADV_GMUX                    IOMUX_MAKE_VALUE(0x30, 27, 1, 0x01) /*!< xmc_nadv not used */

/**
  * @}
  */

/** @defgroup IOMUX_map8_definition
  * @brief iomux map8 definitions
  * @{
  */

#if defined (AT32F407xx)
#define EMAC_GMUX_01                     IOMUX_MAKE_VALUE(0x34, 16, 2, 0x01) /*!< rx_dv/crs_dv(pd8), rxd0(pd9), rxd1(pd10), rxd2(pd11), rxd3(pd12) */
#define MII_RMII_SEL_GMUX                IOMUX_MAKE_VALUE(0x34, 18, 1, 0x01) /*!< mii or rmii selection */
#define PTP_PPS_GMUX                     IOMUX_MAKE_VALUE(0x34, 19, 1, 0x01) /*!< ethernet ptp pps mux function remapping */
#endif
#define USART6_GMUX                      IOMUX_MAKE_VALUE(0x34, 20, 4, 0x01) /*!< usart6_tx(pa4), usart6_rx(pa5) */
#define UART7_GMUX                       IOMUX_MAKE_VALUE(0x34, 24, 4, 0x01) /*!< uart7_tx(pb4),  uart7_rx(pb3) */
#define UART8_GMUX                       IOMUX_MAKE_VALUE(0x34, 28, 4, 0x01) /*!< uart8_tx(pc2),  uart8_rx(pc3) */

/**
  * @}
  */

/** @defgroup GPIO_exported_types
  * @{
  */

/**
  * @brief gpio mode select
  */
typedef enum
{
  GPIO_MODE_INPUT                        = 0x00, /*!< gpio input mode */
  GPIO_MODE_OUTPUT                       = 0x10, /*!< gpio output mode */
  GPIO_MODE_MUX                          = 0x08, /*!< gpio mux function mode */
  GPIO_MODE_ANALOG                       = 0x03  /*!< gpio analog in/out mode */
} gpio_mode_type;

/**
  * @brief gpio output drive strength select
  */
typedef enum
{
  GPIO_DRIVE_STRENGTH_STRONGER           = 0x01, /*!< stronger sourcing/sinking strength */
  GPIO_DRIVE_STRENGTH_MODERATE           = 0x02  /*!< moderate sourcing/sinking strength */
} gpio_drive_type;

/**
  * @brief gpio output type
  */
typedef enum
{
  GPIO_OUTPUT_PUSH_PULL                  = 0x00, /*!< output push-pull */
  GPIO_OUTPUT_OPEN_DRAIN                 = 0x04  /*!< output open-drain */
} gpio_output_type;

/**
  * @brief gpio pull type
  */
typedef enum
{
  GPIO_PULL_NONE                         = 0x0004, /*!< floating for input, no pull for output */
  GPIO_PULL_UP                           = 0x0018, /*!< pull-up */
  GPIO_PULL_DOWN                         = 0x0028  /*!< pull-down */
} gpio_pull_type;

/**
  * @brief gpio pins source type
  */
typedef enum
{
  GPIO_PINS_SOURCE0                      = 0x00, /*!< gpio pins source number 0 */
  GPIO_PINS_SOURCE1                      = 0x01, /*!< gpio pins source number 1 */
  GPIO_PINS_SOURCE2                      = 0x02, /*!< gpio pins source number 2 */
  GPIO_PINS_SOURCE3                      = 0x03, /*!< gpio pins source number 3 */
  GPIO_PINS_SOURCE4                      = 0x04, /*!< gpio pins source number 4 */
  GPIO_PINS_SOURCE5                      = 0x05, /*!< gpio pins source number 5 */
  GPIO_PINS_SOURCE6                      = 0x06, /*!< gpio pins source number 6 */
  GPIO_PINS_SOURCE7                      = 0x07, /*!< gpio pins source number 7 */
  GPIO_PINS_SOURCE8                      = 0x08, /*!< gpio pins source number 8 */
  GPIO_PINS_SOURCE9                      = 0x09, /*!< gpio pins source number 9 */
  GPIO_PINS_SOURCE10                     = 0x0A, /*!< gpio pins source number 10 */
  GPIO_PINS_SOURCE11                     = 0x0B, /*!< gpio pins source number 11 */
  GPIO_PINS_SOURCE12                     = 0x0C, /*!< gpio pins source number 12 */
  GPIO_PINS_SOURCE13                     = 0x0D, /*!< gpio pins source number 13 */
  GPIO_PINS_SOURCE14                     = 0x0E, /*!< gpio pins source number 14 */
  GPIO_PINS_SOURCE15                     = 0x0F  /*!< gpio pins source number 15 */
} gpio_pins_source_type;

/**
  * @brief gpio port source type
  */
typedef enum
{
  GPIO_PORT_SOURCE_GPIOA                 = 0x00, /*!< gpio port source gpioa */
  GPIO_PORT_SOURCE_GPIOB                 = 0x01, /*!< gpio port source gpiob */
  GPIO_PORT_SOURCE_GPIOC                 = 0x02, /*!< gpio port source gpioc */
  GPIO_PORT_SOURCE_GPIOD                 = 0x03, /*!< gpio port source gpiod */
  GPIO_PORT_SOURCE_GPIOE                 = 0x04, /*!< gpio port source gpioe */
} gpio_port_source_type;

/**
  * @brief gpio init type
  */
typedef struct
{
  uint32_t                               gpio_pins;           /*!< pins number selection */
  gpio_output_type                       gpio_out_type;       /*!< output type selection */
  gpio_pull_type                         gpio_pull;           /*!< pull type selection */
  gpio_mode_type                         gpio_mode;           /*!< mode selection */
  gpio_drive_type                        gpio_drive_strength; /*!< drive strength selection */
} gpio_init_type;

/**
  * @brief type define gpio register all
  */
typedef struct
{
  /**
    * @brief gpio cfglr register, offset:0x00
    */
  union
  {
    __IO uint32_t cfglr;
    struct
    {
      __IO uint32_t iomc0                : 2; /* [1:0] */
      __IO uint32_t iofc0                : 2; /* [3:2] */
      __IO uint32_t iomc1                : 2; /* [5:4] */
      __IO uint32_t iofc1                : 2; /* [7:6] */
      __IO uint32_t iomc2                : 2; /* [9:8] */
      __IO uint32_t iofc2                : 2; /* [11:10] */
      __IO uint32_t iomc3                : 2; /* [13:12] */
      __IO uint32_t iofc3                : 2; /* [15:14] */
      __IO uint32_t iomc4                : 2; /* [17:16] */
      __IO uint32_t iofc4                : 2; /* [19:18] */
      __IO uint32_t iomc5                : 2; /* [21:20] */
      __IO uint32_t iofc5                : 2; /* [23:22] */
      __IO uint32_t iomc6                : 2; /* [25:24] */
      __IO uint32_t iofc6                : 2; /* [27:26] */
      __IO uint32_t iomc7                : 2; /* [29:28] */
      __IO uint32_t iofc7                : 2; /* [31:30] */
    } cfglr_bit;
  };

  /**
    * @brief gpio cfghr register, offset:0x04
    */
  union
  {
    __IO uint32_t cfghr;
    struct
    {
      __IO uint32_t iomc8                : 2; /* [1:0] */
      __IO uint32_t iofc8                : 2; /* [3:2] */
      __IO uint32_t iomc9                : 2; /* [5:4] */
      __IO uint32_t iofc9                : 2; /* [7:6] */
      __IO uint32_t iomc10               : 2; /* [9:8] */
      __IO uint32_t iofc10               : 2; /* [11:10] */
      __IO uint32_t iomc11               : 2; /* [13:12] */
      __IO uint32_t iofc11               : 2; /* [15:14] */
      __IO uint32_t iomc12               : 2; /* [17:16] */
      __IO uint32_t iofc12               : 2; /* [19:18] */
      __IO uint32_t iomc13               : 2; /* [21:20] */
      __IO uint32_t iofc13               : 2; /* [23:22] */
      __IO uint32_t iomc14               : 2; /* [25:24] */
      __IO uint32_t iofc14               : 2; /* [27:26] */
      __IO uint32_t iomc15               : 2; /* [29:28] */
      __IO uint32_t iofc15               : 2; /* [31:30] */
    } cfghr_bit;
  };

  /**
    * @brief gpio idt register, offset:0x08
    */
  union
  {
    __IO uint32_t idt;
    struct
    {
      __IO uint32_t idt0                 : 1; /* [0] */
      __IO uint32_t idt1                 : 1; /* [1] */
      __IO uint32_t idt2                 : 1; /* [2] */
      __IO uint32_t idt3                 : 1; /* [3] */
      __IO uint32_t idt4                 : 1; /* [4] */
      __IO uint32_t idt5                 : 1; /* [5] */
      __IO uint32_t idt6                 : 1; /* [6] */
      __IO uint32_t idt7                 : 1; /* [7] */
      __IO uint32_t idt8                 : 1; /* [8] */
      __IO uint32_t idt9                 : 1; /* [9] */
      __IO uint32_t idt10                : 1; /* [10] */
      __IO uint32_t idt11                : 1; /* [11] */
      __IO uint32_t idt12                : 1; /* [12] */
      __IO uint32_t idt13                : 1; /* [13] */
      __IO uint32_t idt14                : 1; /* [14] */
      __IO uint32_t idt15                : 1; /* [15] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } idt_bit;
  };

  /**
    * @brief gpio odt register, offset:0x0C
    */
  union
  {
    __IO uint32_t odt;
    struct
    {
      __IO uint32_t odt0                 : 1; /* [0] */
      __IO uint32_t odt1                 : 1; /* [1] */
      __IO uint32_t odt2                 : 1; /* [2] */
      __IO uint32_t odt3                 : 1; /* [3] */
      __IO uint32_t odt4                 : 1; /* [4] */
      __IO uint32_t odt5                 : 1; /* [5] */
      __IO uint32_t odt6                 : 1; /* [6] */
      __IO uint32_t odt7                 : 1; /* [7] */
      __IO uint32_t odt8                 : 1; /* [8] */
      __IO uint32_t odt9                 : 1; /* [9] */
      __IO uint32_t odt10                : 1; /* [10] */
      __IO uint32_t odt11                : 1; /* [11] */
      __IO uint32_t odt12                : 1; /* [12] */
      __IO uint32_t odt13                : 1; /* [13] */
      __IO uint32_t odt14                : 1; /* [14] */
      __IO uint32_t odt15                : 1; /* [15] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } odt_bit;
  };

  /**
    * @brief gpio scr register, offset:0x10
    */
  union
  {
    __IO uint32_t scr;
    struct
    {
      __IO uint32_t iosb0                : 1; /* [0] */
      __IO uint32_t iosb1                : 1; /* [1] */
      __IO uint32_t iosb2                : 1; /* [2] */
      __IO uint32_t iosb3                : 1; /* [3] */
      __IO uint32_t iosb4                : 1; /* [4] */
      __IO uint32_t iosb5                : 1; /* [5] */
      __IO uint32_t iosb6                : 1; /* [6] */
      __IO uint32_t iosb7                : 1; /* [7] */
      __IO uint32_t iosb8                : 1; /* [8] */
      __IO uint32_t iosb9                : 1; /* [9] */
      __IO uint32_t iosb10               : 1; /* [10] */
      __IO uint32_t iosb11               : 1; /* [11] */
      __IO uint32_t iosb12               : 1; /* [12] */
      __IO uint32_t iosb13               : 1; /* [13] */
      __IO uint32_t iosb14               : 1; /* [14] */
      __IO uint32_t iosb15               : 1; /* [15] */
      __IO uint32_t iocb0                : 1; /* [16] */
      __IO uint32_t iocb1                : 1; /* [17] */
      __IO uint32_t iocb2                : 1; /* [18] */
      __IO uint32_t iocb3                : 1; /* [19] */
      __IO uint32_t iocb4                : 1; /* [20] */
      __IO uint32_t iocb5                : 1; /* [21] */
      __IO uint32_t iocb6                : 1; /* [22] */
      __IO uint32_t iocb7                : 1; /* [23] */
      __IO uint32_t iocb8                : 1; /* [24] */
      __IO uint32_t iocb9                : 1; /* [25] */
      __IO uint32_t iocb10               : 1; /* [26] */
      __IO uint32_t iocb11               : 1; /* [27] */
      __IO uint32_t iocb12               : 1; /* [28] */
      __IO uint32_t iocb13               : 1; /* [29] */
      __IO uint32_t iocb14               : 1; /* [30] */
      __IO uint32_t iocb15               : 1; /* [31] */
    } scr_bit;
  };

  /**
    * @brief gpio clr register, offset:0x14
    */
  union
  {
    __IO uint32_t clr;
    struct
    {
      __IO uint32_t iocb0                : 1; /* [0] */
      __IO uint32_t iocb1                : 1; /* [1] */
      __IO uint32_t iocb2                : 1; /* [2] */
      __IO uint32_t iocb3                : 1; /* [3] */
      __IO uint32_t iocb4                : 1; /* [4] */
      __IO uint32_t iocb5                : 1; /* [5] */
      __IO uint32_t iocb6                : 1; /* [6] */
      __IO uint32_t iocb7                : 1; /* [7] */
      __IO uint32_t iocb8                : 1; /* [8] */
      __IO uint32_t iocb9                : 1; /* [9] */
      __IO uint32_t iocb10               : 1; /* [10] */
      __IO uint32_t iocb11               : 1; /* [11] */
      __IO uint32_t iocb12               : 1; /* [12] */
      __IO uint32_t iocb13               : 1; /* [13] */
      __IO uint32_t iocb14               : 1; /* [14] */
      __IO uint32_t iocb15               : 1; /* [15] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } clr_bit;
  };

  /**
    * @brief gpio wpr register, offset:0x18
    */
  union
  {
    __IO uint32_t wpr;
    struct
    {
      __IO uint32_t wpen0                : 1; /* [0] */
      __IO uint32_t wpen1                : 1; /* [1] */
      __IO uint32_t wpen2                : 1; /* [2] */
      __IO uint32_t wpen3                : 1; /* [3] */
      __IO uint32_t wpen4                : 1; /* [4] */
      __IO uint32_t wpen5                : 1; /* [5] */
      __IO uint32_t wpen6                : 1; /* [6] */
      __IO uint32_t wpen7                : 1; /* [7] */
      __IO uint32_t wpen8                : 1; /* [8] */
      __IO uint32_t wpen9                : 1; /* [9] */
      __IO uint32_t wpen10               : 1; /* [10] */
      __IO uint32_t wpen11               : 1; /* [11] */
      __IO uint32_t wpen12               : 1; /* [12] */
      __IO uint32_t wpen13               : 1; /* [13] */
      __IO uint32_t wpen14               : 1; /* [14] */
      __IO uint32_t wpen15               : 1; /* [15] */
      __IO uint32_t wpseq                : 1; /* [16] */
      __IO uint32_t reserved1            : 15;/* [31:17] */
    } wpr_bit;
  };

  /**
    * @brief gpio reserved1 register, offset:0x1C~0x38
    */
    __IO uint32_t reserved1[8];

  /**
    * @brief gpio hdrv register, offset:0x3C
    */
  union
  {
    __IO uint32_t hdrv;
    struct
    {
      __IO uint32_t hdrv0                : 1; /* [0] */
      __IO uint32_t hdrv1                : 1; /* [1] */
      __IO uint32_t hdrv2                : 1; /* [2] */
      __IO uint32_t hdrv3                : 1; /* [3] */
      __IO uint32_t hdrv4                : 1; /* [4] */
      __IO uint32_t hdrv5                : 1; /* [5] */
      __IO uint32_t hdrv6                : 1; /* [6] */
      __IO uint32_t hdrv7                : 1; /* [7] */
      __IO uint32_t hdrv8                : 1; /* [8] */
      __IO uint32_t hdrv9                : 1; /* [9] */
      __IO uint32_t hdrv10               : 1; /* [10] */
      __IO uint32_t hdrv11               : 1; /* [11] */
      __IO uint32_t hdrv12               : 1; /* [12] */
      __IO uint32_t hdrv13               : 1; /* [13] */
      __IO uint32_t hdrv14               : 1; /* [14] */
      __IO uint32_t hdrv15               : 1; /* [15] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } hdrv_bit;
  };
} gpio_type;

/**
  * @brief type define iomux register all
  */
typedef struct
{
  /**
    * @brief mux event control register, offset:0x00
    */
  union
  {
    __IO uint32_t evtout;
    struct
    {
      __IO uint32_t selpin               : 4; /* [3:0] */
      __IO uint32_t selport              : 3; /* [6:4] */
      __IO uint32_t evoen                : 1; /* [7]   */
      __IO uint32_t reserved1            : 24;/* [31:8] */
    } evtout_bit;
  };

  /**
    * @brief iomux remap register, offset:0x04
    */
  union
  {
    __IO uint32_t remap;
    struct
    {
      __IO uint32_t spi1_mux_l           : 1; /* [0] */
      __IO uint32_t i2c1_mux             : 1; /* [1] */
      __IO uint32_t usart1_mux           : 1; /* [2] */
      __IO uint32_t usart2_mux           : 1; /* [3] */
      __IO uint32_t usart3_mux           : 2; /* [5:4] */
      __IO uint32_t tmr1_mux             : 2; /* [7:6] */
      __IO uint32_t tmr2_mux             : 2; /* [9:8] */
      __IO uint32_t tmr3_mux             : 2; /* [11:10] */
      __IO uint32_t tmr4_mux             : 1; /* [12] */
      __IO uint32_t can_mux              : 2; /* [14:13] */
      __IO uint32_t pd01_mux             : 1; /* [15] */
      __IO uint32_t tmr5ch4_mux          : 1; /* [16] */
      __IO uint32_t adc1_extrgpre_mux    : 1; /* [17] */
      __IO uint32_t adc1_extrgord_mux    : 1; /* [18] */
      __IO uint32_t adc2_extrgpre_mux    : 1; /* [19] */
      __IO uint32_t adc2_extrgord_mux    : 1; /* [20] */
      __IO uint32_t emac_mux             : 1; /* [21] */
      __IO uint32_t can2_mux             : 1; /* [22] */
      __IO uint32_t mii_rmii_sel_mux     : 1; /* [23] */
      __IO uint32_t swjtag_mux           : 3; /* [26:24] */
      __IO uint32_t reserved1            : 1; /* [27] */
      __IO uint32_t spi3_mux             : 1; /* [28] */
      __IO uint32_t tim2itr1_mux         : 1; /* [29] */
      __IO uint32_t ptp_pps_mux          : 1; /* [30] */
      __IO uint32_t spi1_mux_h           : 1; /* [31] */
    } remap_bit;
  };

  /**
    * @brief mux external interrupt configuration register 1, offset:0x08
    */
  union
  {
    __IO uint32_t exintc1;
    struct
    {
      __IO uint32_t exint0              : 4; /* [3:0] */
      __IO uint32_t exint1              : 4; /* [7:4] */
      __IO uint32_t exint2              : 4; /* [11:8] */
      __IO uint32_t exint3              : 4; /* [15:12] */
      __IO uint32_t reserved1           : 16;/* [31:16] */
    } exintc1_bit;
  };

  /**
    * @brief mux external interrupt configuration register 2, offset:0x0c
    */
  union
  {
    __IO uint32_t exintc2;
    struct
    {
      __IO uint32_t exint4              : 4; /* [3:0] */
      __IO uint32_t exint5              : 4; /* [7:4] */
      __IO uint32_t exint6              : 4; /* [11:8] */
      __IO uint32_t exint7              : 4; /* [15:12] */
      __IO uint32_t reserved1           : 16;/* [31:16] */
    } exintc2_bit;
  };

  /**
    * @brief mux external interrupt configuration register 3, offset:0x10
    */
  union
  {
    __IO uint32_t exintc3;
    struct
    {
      __IO uint32_t exint8              : 4; /* [3:0] */
      __IO uint32_t exint9              : 4; /* [7:4] */
      __IO uint32_t exint10             : 4; /* [11:8] */
      __IO uint32_t exint11             : 4; /* [15:12] */
      __IO uint32_t reserved1           : 16;/* [31:16] */
    } exintc3_bit;
  };

  /**
    * @brief mux external interrupt configuration register 4, offset:0x14
    */
  union
  {
    __IO uint32_t exintc4;
    struct
    {
      __IO uint32_t exint12             : 4; /* [3:0] */
      __IO uint32_t exint13             : 4; /* [7:4] */
      __IO uint32_t exint14             : 4; /* [11:8] */
      __IO uint32_t exint15             : 4; /* [15:12] */
      __IO uint32_t reserved1           : 16;/* [31:16] */
    } exintc4_bit;
  };

  /**
    * @brief iomux reserved1 register, offset:0x18
    */
  __IO uint32_t reserved1;

  /**
    * @brief iomux remap register 2, offset:0x1C
    */
  union
  {
    __IO uint32_t remap2;
    struct
    {
      __IO uint32_t tmr15_mux            : 1; /* [0] */
      __IO uint32_t reserved1            : 4; /* [4:1] */
      __IO uint32_t tmr9_mux             : 1; /* [5] */
      __IO uint32_t tmr10_mux            : 1; /* [6] */
      __IO uint32_t tmr11_mux            : 1; /* [7] */
      __IO uint32_t tmr13_mux            : 1; /* [8] */
      __IO uint32_t tmr14_mux            : 1; /* [9] */
      __IO uint32_t xmc_nadv_mux         : 1; /* [10] */
      __IO uint32_t reserved2            : 6; /* [16:11] */
      __IO uint32_t spi4_mux             : 1; /* [17] */
      __IO uint32_t i2c3_mux             : 1; /* [18] */
      __IO uint32_t sdio2_mux            : 2; /* [20:19] */
      __IO uint32_t ext_spim_en_mux      : 1; /* [21] */
      __IO uint32_t reserved3            : 10;/* [31:22] */
    } remap2_bit;
  };

  /**
    * @brief iomux remap register 3, offset:0x20
    */
  union
  {
    __IO uint32_t remap3;
    struct
    {
      __IO uint32_t tmr9_gmux            : 4; /* [3:0] */
      __IO uint32_t reserved1            : 28;/* [31:4] */
    } remap3_bit;
  };

  /**
    * @brief iomux remap register 4, offset:0x24
    */
  union
  {
    __IO uint32_t remap4;
    struct
    {
      __IO uint32_t tmr1_gmux            : 4; /* [3:0] */
      __IO uint32_t tmr2_gmux            : 2; /* [5:4] */
      __IO uint32_t tmr2itr1_gmux        : 2; /* [7:6] */
      __IO uint32_t tmr3_gmux            : 4; /* [11:8] */
      __IO uint32_t tmr4_gmux            : 4; /* [15:12] */
      __IO uint32_t reserved1            : 3; /* [18:16] */
      __IO uint32_t tmr5ch4_gmux         : 1; /* [19] */
      __IO uint32_t reserved2            : 12; /* [31:20] */
    } remap4_bit;
  };

  /**
    * @brief iomux remap register 5, offset:0x28
    */
  union
  {
    __IO uint32_t remap5;
    struct
    {
      __IO uint32_t usart5_gmux          : 4; /* [3:0] */
      __IO uint32_t i2c1_gmux            : 4; /* [7:4] */
      __IO uint32_t reserved1            : 4; /* [11:8] */
      __IO uint32_t i2c3_gmux            : 4; /* [15:12] */
      __IO uint32_t spi1_gmux            : 4; /* [19:16] */
      __IO uint32_t spi2_gmux            : 4; /* [23:20] */
      __IO uint32_t spi3_gmux            : 4; /* [27:24] */
      __IO uint32_t spi4_gmux            : 4; /* [31:28] */
    } remap5_bit;
  };

  /**
    * @brief iomux remap register 6, offset:0x2C
    */
  union
  {
    __IO uint32_t remap6;
    struct
    {
      __IO uint32_t can1_gmux            : 4; /* [3:0] */
      __IO uint32_t can2_gmux            : 4; /* [7:4] */
      __IO uint32_t reserved1            : 4; /* [11:8] */
      __IO uint32_t sdio2_gmux           : 4; /* [15:12] */
      __IO uint32_t usart1_gmux          : 4; /* [19:16] */
      __IO uint32_t usart2_gmux          : 4; /* [23:20] */
      __IO uint32_t usart3_gmux          : 4; /* [27:24] */
      __IO uint32_t uart4_gmux           : 4; /* [31:28] */
    } remap6_bit;
  };

  /**
    * @brief iomux remap register 7, offset:0x30
    */
  union
  {
    __IO uint32_t remap7;
    struct
    {
      __IO uint32_t ext_spim_gmux        : 3; /* [2:0] */
      __IO uint32_t ext_spim_gen         : 1; /* [3] */
      __IO uint32_t adc1_etp_gmux        : 1; /* [4] */
      __IO uint32_t adc1_eto_gmux        : 1; /* [5] */
      __IO uint32_t reserved1            : 2; /* [7:6] */
      __IO uint32_t adc2_etp_gmux        : 1; /* [8] */
      __IO uint32_t adc2_eto_gmux        : 1; /* [9] */
      __IO uint32_t reserved2            : 6; /* [15:10] */
      __IO uint32_t swjtag_gmux          : 3; /* [18:16] */
      __IO uint32_t reserved3            : 1; /* [19] */
      __IO uint32_t pd01_gmux            : 1; /* [20] */
      __IO uint32_t reserved4            : 3; /* [23:21] */
      __IO uint32_t xmc_gmux             : 3; /* [26:24] */
      __IO uint32_t xmc_nadv_gmux        : 1; /* [27] */
      __IO uint32_t reserved5            : 4; /* [31:28] */
    } remap7_bit;
  };

  /**
    * @brief iomux remap register 8, offset:0x34
    */
  union
  {
    __IO uint32_t remap8;
    struct
    {
      __IO uint32_t reserved1            : 16; /* [15:0] */
      __IO uint32_t emac_gmux            : 2; /* [17:16] */
      __IO uint32_t mii_rmii_sel_gmux    : 1; /* [18] */
      __IO uint32_t ptp_pps_gmux         : 1; /* [19] */
      __IO uint32_t usart6_gmux          : 4; /* [23:20] */
      __IO uint32_t uart7_gmux           : 4; /* [27:24] */
      __IO uint32_t uart8_gmux           : 4; /* [31:28] */
    } remap8_bit;
  };
} iomux_type;

/**
  * @}
  */

#define GPIOA                            ((gpio_type *) GPIOA_BASE)
#define GPIOB                            ((gpio_type *) GPIOB_BASE)
#define GPIOC                            ((gpio_type *) GPIOC_BASE)
#define GPIOD                            ((gpio_type *) GPIOD_BASE)
#define GPIOE                            ((gpio_type *) GPIOE_BASE)
#define IOMUX                            ((iomux_type *) IOMUX_BASE)

/** @defgroup GPIO_exported_functions
  * @{
  */

void gpio_reset(gpio_type *gpio_x);
void gpio_iomux_reset(void);
void gpio_init(gpio_type *gpio_x, gpio_init_type *gpio_init_struct);
void gpio_default_para_init(gpio_init_type *gpio_init_struct);
flag_status gpio_input_data_bit_read(gpio_type *gpio_x, uint16_t pins);
uint16_t gpio_input_data_read(gpio_type *gpio_x);
flag_status gpio_output_data_bit_read(gpio_type *gpio_x, uint16_t pins);
uint16_t gpio_output_data_read(gpio_type *gpio_x);
void gpio_bits_set(gpio_type *gpio_x, uint16_t pins);
void gpio_bits_reset(gpio_type *gpio_x, uint16_t pins);
void gpio_bits_write(gpio_type *gpio_x, uint16_t pins, confirm_state bit_state);
void gpio_port_wirte(gpio_type *gpio_x, uint16_t port_value);
void gpio_pin_wp_config(gpio_type *gpio_x, uint16_t pins);
void gpio_pins_huge_driven_config(gpio_type *gpio_x, uint16_t pins, confirm_state new_state);
void gpio_event_output_config(gpio_port_source_type gpio_port_source, gpio_pins_source_type gpio_pin_source);
void gpio_event_output_enable(confirm_state new_state);
void gpio_pin_remap_config(uint32_t gpio_remap, confirm_state new_state);
void gpio_exint_line_config(gpio_port_source_type gpio_port_source, gpio_pins_source_type gpio_pin_source);

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
