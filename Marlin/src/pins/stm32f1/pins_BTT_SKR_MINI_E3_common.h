/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "env_validate.h"

// Release PB3/PB4 (E0 STP/DIR) from JTAG pins
#define DISABLE_JTAG

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

#if EITHER(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2KB
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE  // 2KB
#endif

//
// Servos
//
#define SERVO0_PIN                          PA1   // SERVOS

//
// Limit Switches
//
#define X_STOP_PIN                          PC0   // X-STOP
#define Y_STOP_PIN                          PC1   // Y-STOP
#define Z_STOP_PIN                          PC2   // Z-STOP

//
// Z Probe must be this pin
//
#define Z_MIN_PROBE_PIN                     PC14  // PROBE

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC15  // E0-STOP
#endif

//
// Power-loss Detection
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                    PC12  // Power Loss Detection: PWR-DET
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PB14
#define X_STEP_PIN                          PB13
#define X_DIR_PIN                           PB12

#define Y_ENABLE_PIN                        PB11
#define Y_STEP_PIN                          PB10
#define Y_DIR_PIN                           PB2

#define Z_ENABLE_PIN                        PB1
#define Z_STEP_PIN                          PB0
#define Z_DIR_PIN                           PC5

#define E0_ENABLE_PIN                       PD2
#define E0_STEP_PIN                         PB3
#define E0_DIR_PIN                          PB4

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // Analog Input "TH0"
#define TEMP_BED_PIN                        PC3   // Analog Input "TB0"

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC8   // "HE"
#define HEATER_BED_PIN                      PC9   // "HB"

#ifdef SKR_MINI_E3_V2
  #define FAN_PIN                           PC6
#else
  #define FAN_PIN                           PA8   // "FAN0"
#endif

//
// USB connect control
//
#ifdef SKR_MINI_E3_V2
  #define USB_CONNECT_PIN                   PA14
#else
  #define USB_CONNECT_PIN                   PC13
#endif

#define USB_CONNECT_INVERTING              false

/**
 *        SKR Mini E3 V1.0, V1.2                      SKR Mini E3 V2.0
 *                _____                                     _____
 *            5V | 1 2 | GND                            5V | 1 2 | GND
 *  (LCD_EN) PB7 | 3 4 | PB8  (LCD_RS)       (LCD_EN) PB15 | 3 4 | PB8  (LCD_RS)
 *  (LCD_D4) PB9 | 5 6   PA10 (BTN_EN2)      (LCD_D4) PB9  | 5 6   PA10 (BTN_EN2)
 *         RESET | 7 8 | PA9  (BTN_EN1)              RESET | 7 8 | PA9  (BTN_EN1)
 * (BTN_ENC) PB6 | 9 10| PB5  (BEEPER)      (BTN_ENC) PA15 | 9 10| PB5  (BEEPER)
 *                -----                                     -----
 *                EXP1                                      EXP1
 */
#ifdef SKR_MINI_E3_V2
  #define EXP1_9                            PA15
  #define EXP1_3                            PB15
#else
  #define EXP1_9                            PB6
  #define EXP1_3                            PB7
#endif

#if HAS_WIRED_LCD

  #if ENABLED(CR10_STOCKDISPLAY)

    #define BEEPER_PIN                      PB5
    #define BTN_ENC                       EXP1_9

    #define BTN_EN1                         PA9
    #define BTN_EN2                         PA10

    #define LCD_PINS_RS                     PB8
    #define LCD_PINS_ENABLE               EXP1_3
    #define LCD_PINS_D4                     PB9

  #elif ENABLED(ZONESTAR_LCD)                     // ANET A8 LCD Controller - Must convert to 3.3V - CONNECTING TO 5V WILL DAMAGE THE BOARD!

    #error "CAUTION! ZONESTAR_LCD requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. Comment out this line to continue."

    #define LCD_PINS_RS                     PB9
    #define LCD_PINS_ENABLE               EXP1_9
    #define LCD_PINS_D4                     PB8
    #define LCD_PINS_D5                     PA10
    #define LCD_PINS_D6                     PA9
    #define LCD_PINS_D7                     PB5
    #define ADC_KEYPAD_PIN                  PA1   // Repurpose servo pin for ADC - CONNECTING TO 5V WILL DAMAGE THE BOARD!

  #elif EITHER(MKS_MINI_12864, ENDER2_STOCKDISPLAY)

    #define BTN_ENC                       EXP1_9
    #define BTN_EN1                         PA9
    #define BTN_EN2                         PA10

    #define DOGLCD_CS                       PB8
    #define DOGLCD_A0                       PB9
    #define DOGLCD_SCK                      PB5
    #define DOGLCD_MOSI                   EXP1_3

    #define FORCE_SOFT_SPI
    #define LCD_BACKLIGHT_PIN               -1

  #elif IS_TFTGLCD_PANEL

    #if ENABLED(TFTGLCD_PANEL_SPI)

      #error "CAUTION! TFTGLCD_PANEL_SPI requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. Comment out this line to continue."

      /**
       * TFTGLCD_PANEL_SPI display pinout
       *
       *               Board                                      Display
       *               _____                                       _____
       *           5V | 1 2 | GND                (SPI1-MISO) MISO | 1 2 | SCK   (SPI1-SCK)
       * (FREE)   PB7 | 3 4 | PB8  (LCD_CS)      (PA9)     LCD_CS | 3 4 | SD_CS (PA10)
       * (FREE)   PB9 | 5 6 | PA10 (SD_CS)                 (FREE) | 5 6 | MOSI  (SPI1-MOSI)
       *        RESET | 7 8 | PA9  (MOD_RESET)   (PB5)     SD_DET | 7 8 | (FREE)
       * (BEEPER) PB6 | 9 10| PB5  (SD_DET)                   GND | 9 10| 5V
       *               -----                                       -----
       *                EXP1                                        EXP1
       *
       * Needs custom cable:
       *
       *    Board   Adapter   Display
       *           _________
       *   EXP1-1 ----------- EXP1-10
       *   EXP1-2 ----------- EXP1-9
       *   SPI1-4 ----------- EXP1-6
       *   EXP1-4 ----------- FREE
       *   SPI1-3 ----------- EXP1-2
       *   EXP1-6 ----------- EXP1-4
       *   EXP1-7 ----------- FREE
       *   EXP1-8 ----------- EXP1-3
       *   SPI1-1 ----------- EXP1-1
       *  EXP1-10 ----------- EXP1-7
       */

      #define TFTGLCD_CS                    PA9

    #endif

  #else
    #error "Only CR10_STOCKDISPLAY, ZONESTAR_LCD, ENDER2_STOCKDISPLAY, MKS_MINI_12864, and TFTGLCD_PANEL_(SPI|I2C) are currently supported on the BIGTREE_SKR_MINI_E3."
  #endif

#endif // HAS_WIRED_LCD

#if BOTH(TOUCH_UI_FTDI_EVE, LCD_FYSETC_TFT81050)

  #error "CAUTION! LCD_FYSETC_TFT81050 requires wiring modifications. See 'pins_BTT_SKR_MINI_E3_common.h' for details. Comment out this line to continue."

  /** FYSETC TFT TFT81050 display pinout
   *
   *               Board                                      Display
   *               _____                                       _____
   *           5V | 1 2 | GND                (SPI1-MISO) MISO | 1 2 | SCK   (SPI1-SCK)
   * (FREE)   PB7 | 3 4 | PB8  (LCD_CS)      (PA9)  MOD_RESET | 3 4 | SD_CS (PA10)
   * (FREE)   PB9 | 5 6 | PA10 (SD_CS)       (PB8)     LCD_CS | 5 6 | MOSI  (SPI1-MOSI)
   *        RESET | 7 8 | PA9  (MOD_RESET)   (PB5)     SD_DET | 7 8 | RESET
   * (BEEPER) PB6 | 9 10| PB5  (SD_DET)                   GND | 9 10| 5V
   *               -----                                       -----
   *                EXP1                                        EXP1
   *
   * Needs custom cable:
   *
   *    Board   Adapter   Display
   *           _________
   *   EXP1-1 ----------- EXP1-10
   *   EXP1-2 ----------- EXP1-9
   *   SPI1-4 ----------- EXP1-6
   *   EXP1-4 ----------- EXP1-5
   *   SPI1-3 ----------- EXP1-2
   *   EXP1-6 ----------- EXP1-4
   *   EXP1-7 ----------- EXP1-8
   *   EXP1-8 ----------- EXP1-3
   *   SPI1-1 ----------- EXP1-1
   *  EXP1-10 ----------- EXP1-7
   */

  #define CLCD_SPI_BUS                         1  // SPI1 connector

  #define BEEPER_PIN                      EXP1_9

  #define CLCD_MOD_RESET                    PA9
  #define CLCD_SPI_CS                       PB8

#endif // TOUCH_UI_FTDI_EVE && LCD_FYSETC_TFT81050

//
// SD Support
//

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define SD_DETECT_PIN                     PC4
#elif SD_CONNECTION_IS(LCD) && (BOTH(TOUCH_UI_FTDI_EVE, LCD_FYSETC_TFT81050) || IS_TFTGLCD_PANEL)
  #define SD_DETECT_PIN                     PB5
  #define SD_SS_PIN                         PA10
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "SD CUSTOM_CABLE is not compatible with SKR Mini E3."
#endif

#define ONBOARD_SPI_DEVICE                     1  // SPI1 -> used only by HAL/STM32F1...
#define ONBOARD_SD_CS_PIN                   PA4   // Chip select for "System" SD card

#define ENABLE_SPI1
#define SDSS                   ONBOARD_SD_CS_PIN
#define SD_SCK_PIN                          PA5
#define SD_MISO_PIN                         PA6
#define SD_MOSI_PIN                         PA7
