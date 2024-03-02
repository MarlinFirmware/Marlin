/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#if NOT_TARGET(STM32F746xx)
  #error "Oops! Select an STM32F746 environment"
#endif

#define BOARD_INFO_NAME      "NUCLEO-F746ZG"
#define DEFAULT_MACHINE_NAME "Prototype Board"

#if NO_EEPROM_SELECTED
  #define FLASH_EEPROM_EMULATION                  // Use Flash-based EEPROM emulation
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  // Not yet supported on F7 hardware
  //#define FLASH_EEPROM_LEVELING
#endif

/**
 * Timer assignments
 *
 * TIM1 -
 * TIM2 - Hardware PWM (Fan/Heater Pins)
 * TIM3 - Hardware PWM (Servo Pins)
 * TIM4 - STEP_TIMER (Marlin)
 * TIM5 -
 * TIM6 - TIMER_TONE (variant.h)
 * TIM7 - TIMER_SERVO (variant.h)
 * TIM9 - TIMER_SERIAL (platformio.ini)
 * TIM10 - For some reason trips Watchdog when used for SW Serial
 * TIM11 -
 * TIM12 -
 * TIM13 -
 * TIM14 - TEMP_TIMER (Marlin)
 *
 */
#define STEP_TIMER  4
#define TEMP_TIMER 14

// system/debug pins - do not use as GPIO
// PA13  SWDIO,
// PA14  SWCLK

/**
 * These pin assignments are arbitrary and intended for testing purposes.
 * Assignments may not be ideal, and not every assignment has been tested.
 * Proceed at your own risk.
 *                                                            _CN7_
 *                                              (X_STEP) PC6 | · · | PB8 (X_EN)
 *                                              (X_DIR) PB15 | · · | PB9 (X_CS)
 *                                             (LCD_D4) PB13 | · · | AVDD
 *                 _CN8_                                PB12 | · · | GND
 *             -- | · · | PC8  (SDIO_D0)     (HEATER_0) PA15 | · · | PA5  (SCLK)
 *          IOREF | · · | PC9  (SDIO_D1)        (BEEPER) PC7 | · · | PA6  (MISO)
 *          RESET | · · | PC10 (SDIO_D2)    (SERVO1_PIN) PB5 | · · | PA7  (MOSI)
 *          +3.3V | · · | PC11 (SDIO_D3)    (HEATER_BED) PB3 | · · | PD14 (SD_DETECT)
 *            +5V | · · | PC12 (SDIO_CLK)         (SDSS) PA4 | · · | PD15 (LCD_ENABLE)
 *            GND | · · | PD2  (SDIO_CMD)   (SERVO0_PIN) PB4 | · · | PF12 (LCD_RS)
 *            GND | ·  · | PG2  (SD_DETECT)                               ￣￣￣
 *            VIN | · · | PG3
 *                 ￣￣￣                                      _CN10
 *                                                      AVDD | · · | PF13 (BTN_EN1)
 *                 _CN9_                                AGND | · · | PE9  (BTN_EN2)
 *   (TEMP_0) PA3 | · · | PD7                            GND | · · | PE11 (BTN_ENC)
 * (TEMP_BED) PC0 | · · | PD6                   (E_STEP) PB1 | · · | PF14 (E_EN)
 *            PC3 | · · | PD5                            PC2 | · · | PE13
 *  (Y_STEP)  PF3 | · · | PD4 (Y_EN)                     PF4 | · · | PF15
 *   (Y_DIR)  PF5 | · · | PD3 (Y_CS)                     PB6 | · · | PG14
 *           PF10 | · · | GND                            PB2 | · · | PG9
 *             -- | · · | PE2                            GND | · · | PE8
 *            PA7 | · · | PE4                   (E_DIR) PD13 | · · | PE7  (E_CS)
 *            PF2 | · · | PE5                           PD12 | · · | GND
 *            PF1 | · · | PE6                  (Z_STEP) PD11 | · · | PE10 (Z_EN)
 *            PF0 | · · | PE3                    (Z_DIR) PE2 | · · | PE12 (Z_CS)
 *            GND | · · | PF8                            GND | · · | PE14
 *    (Z_MAX) PD0 | · · | PF7 (X_MIN)                    PA0 | · · | PE15
 *    (Z_MIN) PD1 | · · | PF9 (X_MAX)                    PB0 | · · | PB10 (FAN)
 *    (Y_MAX) PG0 | · · | PG1 (Y_MIN)                    PE0 | · · | PB11 (FAN1)
 *                 ￣￣￣                                     ￣￣￣￣
 */

#define X_MIN_PIN                           PF7
#define X_MAX_PIN                           PF9
#define Y_MIN_PIN                           PG1
#define Y_MAX_PIN                           PG0
#define Z_MIN_PIN                           PD1
#define Z_MAX_PIN                           PD0

//
// Steppers
//
#define X_STEP_PIN                          PC6
#define X_DIR_PIN                           PB15
#define X_ENABLE_PIN                        PB8
#define X_CS_PIN                            PB9

#define Y_STEP_PIN                          PF3
#define Y_DIR_PIN                           PF5
#define Y_ENABLE_PIN                        PD4
#define Y_CS_PIN                            PE3

#define Z_STEP_PIN                          PD11
#define Z_DIR_PIN                           PE2
#define Z_ENABLE_PIN                        PE10
#define Z_CS_PIN                            PE12

#define E0_STEP_PIN                         PB1
#define E0_DIR_PIN                          PD13
#define E0_ENABLE_PIN                       PF14
#define E0_CS_PIN                           PE7

#define E1_STEP_PIN                         PG1
#define E1_DIR_PIN                          PG13
#define E1_ENABLE_PIN                       PG14
#define E1_CS_PIN                           PG7

#if HAS_TMC_UART
  #define X_SERIAL_TX_PIN                   PB9
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                   PE3
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                   PE12
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN

  #define E0_SERIAL_TX_PIN                  PG9
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN
#endif

//
// Temperature Sensors
//

#define A0    0   // PA0  ADC1 IN0
#define A1    1   // PA1  ADC1 IN1
#define A2    2   // PA2  ADC1 IN2
#define A3    3   // PA3  ADC1 IN3
#define A4    4   // PA4  ADC1 IN4
#define A5    5   // PA5  ADC1 IN5
#define A6    6   // PA6  ADC1 IN6
#define A7    7   // PA7  ADC1 IN7
#define A8    8   // PB0  ADC1 IN8
#define A9    9   // PB1  ADC1 IN9
#define A10  10   // PC0  ADC1 IN10
#define A11  11   // PC1  ADC1 IN11
#define A12  12   // PC2  ADC1 IN12
#define A13  13   // PC3  ADC1 IN13
#define A14  14   // PC4  ADC1 IN14
#define A15  15   // PC5  ADC1 IN15
#if (STM32F746Zxx || STM32F746Ixx || STM32F746Bxx || STM32F746Nxx)
  #define A16  16   // PF3  ADC3 IN9
  #define A17  17   // PF4  ADC3 IN14
  #define A18  18   // PF5  ADC3 IN15
  #define A19  19   // PF6  ADC3 IN4
  #define A20  20   // PF7  ADC3 IN5
  #define A21  21   // PF8  ADC3 IN6
  #define A22  22   // PF9  ADC3 IN7
  #define A23  23   // PF10 ADC3 IN8
#endif

#define TEMP_0_PIN                     A3  // PA3
#define TEMP_1_PIN                     A10 // PC0
#define TEMP_BED_PIN                   A23 // PF10

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA15  // PWM Capable, TIM2_CH1
#define HEATER_1_PIN                        PB13
#define HEATER_BED_PIN                      PB3   // PWM Capable, TIM2_CH2

#ifndef FAN0_PIN
  #define FAN0_PIN                          PB10  // PWM Capable, TIM2_CH3
#endif
#define FAN1_PIN                            PB11  // PWM Capable, TIM2_CH4

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN               FAN1_PIN
#endif

//
// Servos
//
#define SERVO0_PIN                          PB4   // PWM Capable, TIM3_CH1
#define SERVO1_PIN                          PB5   // PWM Capable, TIM3_CH2

//
// SD card
//
#if SD_CONNECTION_IS(ONBOARD)
  #define SD_DETECT_PIN                     PG2

  // SDIO for SD Card
  //#define ONBOARD_SDIO
  #if ENABLED(ONBOARD_SDIO)
    #define SDIO_CLOCK                   4800000
    #define SDIO__MAX_CLOCK                  118
    #define SDIO_READ_RETRIES                 16
    #define SDIO_D0_PIN                     PC8
    //#define SDIO_D1_PIN                   PC9
    //#define SDIO_D2_PIN                   PC10
    //#define SDIO_D3_PIN                   PC11
    #define SDIO_CK_PIN                     PC12
    #define SDIO_CMD_PIN                    PD2
  #else                                           // SPI
    #define SD_MOSI_PIN                     PA7
    #define MOSI                     SD_MOSI_PIN
    #define SD_MISO_PIN                     PA6
    #define MISO                     SD_MISO_PIN
    #define SD_SCK_PIN                      PA5
    #define SCK                       SD_SCK_PIN
    #define SDSS                            PD14
  #endif
#endif

#define LD1_PIN                             PB0   // green
#define LD2_PIN                             PB7   // blue
#define LD3_PIN                             PB14  // red
#ifndef LED_BLUE
  #define LED_BLUE                       LD2_PIN  // already defined in JSON for nucleo_F746ZG
#endif
#define LED_PIN                         LED_BLUE

/**
 * Sections of CN9 & CN10 can be used as EXP1 & EXP2.
 *
 * Use a pair of 2x5 male-male headers to interface between
 * CN9 & CN10 and the standard 10 pin cables.
 *
 * Conductor 10 on the EXP1 cable will need to be modified to provide +5V.
 * Cut conductor 10 loose from the cable near the CN10 end and add an
 * adapter/jumper so you can plug into +5V.
 */

/**
 *          ----                              ----
 * PC2  9  |1  2| 10  PE13           PA7 15  |1  2| 16  PE4
 * PF4 11  |3  4| 12  PF15           PF2 17  |3  4| 18  PE5
 * PB6 13  |5  6| 14  PG14           PF1 19  |5  6| 20  PE6
 * PB2 15  |7  8| 16  PG9            PF0 21  |7  8| 22  PE3
 * GND 17  |9 10| 18  5V *           GND 23  |9 10| 24  PF8
 *          ----         |                    ----
 *    CN10  EXP1  CN10   |               CN9  EXP2  CN9
 *                       |
 *                       -- custom cable needed - 5V pin must be split out
 */

#define EXP1_01_PIN                         PC2
#define EXP1_02_PIN                         PE13
#define EXP1_03_PIN                         PF4
#define EXP1_04_PIN                         PF15
#define EXP1_05_PIN                         PB6
#define EXP1_06_PIN                         PG14
#define EXP1_07_PIN                         PB2
#define EXP1_08_PIN                         PG9

#define EXP2_01_PIN                         PA7
#define EXP2_02_PIN                         PE4
#define EXP2_03_PIN                         PF2
#define EXP2_04_PIN                         PE5
#define EXP2_05_PIN                         PF1
#define EXP2_06_PIN                         PE6
#define EXP2_07_PIN                         PF0
#define EXP2_08_PIN                         PE3

//
// SD Card
//

#if SD_CONNECTION_IS(LCD)
  #define SD_MOSI_PIN                EXP2_06_PIN
  #define SD_MISO_PIN                EXP2_01_PIN
  #define SD_SCK_PIN                 EXP2_02_PIN
  #define SDSS                       EXP2_04_PIN
  #define SD_DETECT_PIN              EXP2_07_PIN
  #define FORCE_SOFT_SPI
  //#define SOFTWARE_SPI                            // Use soft SPI for LCD's SD
#endif

//
// LCD / Controller
//

#if ANY(TFT_COLOR_UI, TFT_CLASSIC_UI, HAS_WIRED_LCD)
  #ifndef NO_CONTROLLER_CUSTOM_WIRING_WARNING
    #error "CAUTION! NUCLEO_F746ZG requires wiring modifications. See 'pins_NUCLEO_F746ZG.h' for details. (Define NO_CONTROLLER_CUSTOM_WIRING_WARNING to suppress this warning.)"
  #endif

  #define BEEPER_PIN                 EXP1_01_PIN
  #define BTN_EN1                    EXP2_03_PIN
  #define BTN_EN2                    EXP2_05_PIN
  #define BTN_ENC                    EXP1_02_PIN
#endif

#if ANY(TFT_COLOR_UI, TFT_CLASSIC_UI)
  #define TFT_CS_PIN                 EXP1_07_PIN
  #define TFT_SCK_PIN                EXP2_02_PIN
  #define TFT_MISO_PIN               EXP2_01_PIN
  #define TFT_MOSI_PIN               EXP2_06_PIN
  #define TFT_DC_PIN                 EXP1_08_PIN
  #define TFT_A0_PIN                  TFT_DC_PIN

  #define TFT_RESET_PIN              EXP1_04_PIN

  #define LCD_BACKLIGHT_PIN          EXP1_03_PIN
  #define TFT_BACKLIGHT_PIN    LCD_BACKLIGHT_PIN

  #define TOUCH_BUTTONS_HW_SPI
  #define TOUCH_BUTTONS_HW_SPI_DEVICE          1

  #ifndef TFT_WIDTH
    #define TFT_WIDTH                        480
  #endif
  #ifndef TFT_HEIGHT
    #define TFT_HEIGHT                       320
  #endif

  #define TOUCH_CS_PIN               EXP1_05_PIN  // SPI1_NSS
  #define TOUCH_SCK_PIN              EXP2_02_PIN  // SPI1_SCK
  #define TOUCH_MISO_PIN             EXP2_01_PIN  // SPI1_MISO
  #define TOUCH_MOSI_PIN             EXP2_06_PIN  // SPI1_MOSI

  #define LCD_READ_ID                       0xD3
  #define LCD_USE_DMA_SPI

  #define TFT_BUFFER_SIZE                  14400

  #ifndef TOUCH_CALIBRATION_X
    #define TOUCH_CALIBRATION_X           -17253
  #endif
  #ifndef TOUCH_CALIBRATION_Y
    #define TOUCH_CALIBRATION_Y            11579
  #endif
  #ifndef TOUCH_OFFSET_X
    #define TOUCH_OFFSET_X                   514
    #endif
  #ifndef TOUCH_OFFSET_Y
    #define TOUCH_OFFSET_Y                   -24
  #endif
  #ifndef TOUCH_ORIENTATION
    #define TOUCH_ORIENTATION    TOUCH_LANDSCAPE
  #endif

#elif HAS_WIRED_LCD

  #define LCD_PINS_EN                EXP1_03_PIN
  #define LCD_PINS_RS                EXP1_04_PIN
  #define LCD_BACKLIGHT_PIN                 -1

  // MKS MINI12864 and MKS LCD12864B; If using MKS LCD12864A (Need to remove RPK2 resistor)
  #if ENABLED(MKS_MINI_12864)

    #define ENABLE_SPI1
    #define FORCE_SOFT_SPI
    #define DOGLCD_A0                EXP1_07_PIN
    #define DOGLCD_CS                EXP1_06_PIN
    #define DOGLCD_SCK               EXP2_02_PIN
    #define DOGLCD_MOSI              EXP2_06_PIN
    //#define LCD_BACKLIGHT_PIN             -1
    //#define LCD_RESET_PIN                 -1

  #elif ENABLED(FYSETC_MINI_12864_2_1)

    #define LCD_PINS_DC              EXP1_04_PIN
    #define DOGLCD_CS                EXP1_03_PIN
    #define DOGLCD_A0                LCD_PINS_DC
    #define LCD_BACKLIGHT_PIN               -1
    #define LCD_RESET_PIN            EXP1_05_PIN
    #define NEOPIXEL_PIN             EXP1_06_PIN
    #define DOGLCD_MOSI              EXP2_06_PIN
    #define DOGLCD_SCK               EXP2_02_PIN
    //#define LCD_SCREEN_ROTATE              180  // 0, 90, 180, 270

  #else

    #define LCD_PINS_D4              EXP1_05_PIN
    #if IS_ULTIPANEL
      #define LCD_PINS_D5            EXP1_06_PIN
      #define LCD_PINS_D6            EXP1_07_PIN
      #define LCD_PINS_D7            EXP1_08_PIN
    #endif

  #endif // !MKS_MINI_12864

#endif // HAS_WIRED_LCD

#if HAS_MARLINUI_U8GLIB
  #define BOARD_ST7920_DELAY_1                96
  #define BOARD_ST7920_DELAY_2                48
  #define BOARD_ST7920_DELAY_3               600
#endif

// Init ITCM RAM or else callbacks don't register correctly
#define BOARD_PREINIT() do { \
  uint32_t *i = 0x00000000; \
  for (uint32_t count = 0; count < 0x0FFF; count++) i[count] = 0; \
}while(0)
