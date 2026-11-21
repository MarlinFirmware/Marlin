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

#include "env_validate.h"

//#define BOARD_CUSTOM_BUILD_FLAGS -DTONE_CHANNEL=4 -DTONE_TIMER=4 -DTIMER_TONE=4

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "BTT Manta M4P V2.1"
#endif

#define USES_DIAG_JUMPERS

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

//
// EEPROM
//
#if SHALL_USE_EEPROM(FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE                0x800U  // 2K
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
  #define MARLIN_EEPROM_SIZE     EEPROM_PAGE_SIZE
#endif

//
// Servos
//
#define SERVO0_PIN                          PA1   // SERVOS

//
// Probe enable
//
#if ENABLED(PROBE_ENABLE_DISABLE) && !defined(PROBE_ENABLE_PIN)
  #define PROBE_ENABLE_PIN            SERVO0_PIN
#endif

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
// Steppers
//
#define X_STEP_PIN                          PC6
#define X_DIR_PIN                           PA14
#define X_ENABLE_PIN                        PC7
#ifndef X_CS_PIN
  #define X_CS_PIN                          PB12
#endif

#define Y_STEP_PIN                          PB10
#define Y_DIR_PIN                           PB2
#define Y_ENABLE_PIN                        PB11
#ifndef Y_CS_PIN
  #define Y_CS_PIN                          PC10
#endif

#define Z_STEP_PIN                          PB0
#define Z_DIR_PIN                           PC5
#define Z_ENABLE_PIN                        PB1
#ifndef Z_CS_PIN
  #define Z_CS_PIN                          PC9
#endif

#define E0_STEP_PIN                         PB3
#define E0_DIR_PIN                          PB4
#define E0_ENABLE_PIN                       PD5
#ifndef E0_CS_PIN
  #define E0_CS_PIN                         PA13
#endif

//
// Default pins for TMC software SPI
// SPI1 level-shifted to the driver sockets and shared with the onboard SD card
//
#ifndef TMC_SPI_MOSI
  #define TMC_SPI_MOSI                      PA7
#endif
#ifndef TMC_SPI_MISO
  #define TMC_SPI_MISO                      PA6
#endif
#ifndef TMC_SPI_SCK
  #define TMC_SPI_SCK                       PA5
#endif

#if HAS_TMC_UART
  #define X_SERIAL_TX_PIN                   PB12
  #define Y_SERIAL_TX_PIN                   PC10
  #define Z_SERIAL_TX_PIN                   PC9
  #define E0_SERIAL_TX_PIN                  PA13

  // Reduce baud rate to improve software serial reliability
  #ifndef TMC_BAUD_RATE
    #define TMC_BAUD_RATE                  19200
  #endif

#endif // HAS_TMC_UART

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // Analog Input "TH0"
#define TEMP_BED_PIN                        PC4   // Analog Input "TB0"

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC8   // "HE"
#define HEATER_BED_PIN                      PD8   // "HB"

#define FAN0_PIN                            PD2   // "FAN0"
#define FAN1_PIN                            PD3   // "FAN1"
#define FAN2_PIN                            PD4   // "FAN2"

/**
 *                ------                                    ------
 * (BEEPER) PD6  | 1  2 | PB8  (BTN_ENC)  (MISO)      PB14 | 1  2 | PB13 (SCK)
 * (LCD_EN) PB9  | 3  4 | PC3  (LCD_RS)   (BTN_EN1)   PC11 | 3  4 | PA8  (SD_SS)
 * (LCD_D4) PA15 | 5  6   PA10 (LCD_D5)   (BTN_EN2)   PC12 | 5  6   PB15 (MOSI)
 * (LCD_D6)  PA9 | 7  8 | PB5  (LCD_D7)   (SD_DETECT) PC13 | 7  8 | RESET
 *           GND | 9 10 | 5V                           GND | 9 10 | --
 *                ------                                    ------
 *                 EXP1                                      EXP2
 */
#define EXP1_01_PIN                         PD6
#define EXP1_02_PIN                         PB8
#define EXP1_03_PIN                         PB9
#define EXP1_04_PIN                         PC3
#define EXP1_05_PIN                         PA15
#define EXP1_06_PIN                         PA10
#define EXP1_07_PIN                         PA9
#define EXP1_08_PIN                         PB5

#define EXP2_01_PIN                         PB14
#define EXP2_02_PIN                         PB13
#define EXP2_03_PIN                         PC11
#define EXP2_04_PIN                         PA8
#define EXP2_05_PIN                         PC12
#define EXP2_06_PIN                         PB15
#define EXP2_07_PIN                         PC13
#define EXP2_08_PIN                         -1

//
// SD Card
// Must use soft SPI because Marlin's default hardware SPI is tied to LCD's EXP2
//
#ifndef SDCARD_CONNECTION
  #if HAS_WIRED_LCD && DISABLED(NO_LCD_SDCARD)
    #define SDCARD_CONNECTION                LCD
  #else
    #define SDCARD_CONNECTION            ONBOARD
  #endif
#endif

#if SD_CONNECTION_IS(LCD)
  #define SD_SS_PIN                  EXP2_04_PIN
  #define SD_SCK_PIN                 EXP2_02_PIN
  #define SD_MISO_PIN                EXP2_01_PIN
  #define SD_MOSI_PIN                EXP2_06_PIN
  #define SD_DETECT_PIN              EXP2_07_PIN
#elif SD_CONNECTION_IS(ONBOARD)
  #define SD_DETECT_PIN                     -1    // J52 leaves DET unwired
  #define SD_SCK_PIN                        PA5
  #define SD_MISO_PIN                       PA6
  #define SD_MOSI_PIN                       PA7
  #define ONBOARD_SD_CS_PIN                 PA4   // Chip select for "System" SD card
  #define SD_SS_PIN            ONBOARD_SD_CS_PIN
#elif SD_CONNECTION_IS(CUSTOM_CABLE)
  #error "No custom SD drive cable defined for this board."
#endif

//
// LCD / Controller
//

#if ENABLED(CR10_STOCKDISPLAY)

  // Migrated to pins/lcd

#elif ENABLED(MKS_MINI_12864)

  // LCD pins defined in pins/lcd/MKS_MINI_12864.h

  #define LCD_FORCE_SOFT_SPI                      // Panel has its own pins on EXP2
  #if SD_CONNECTION_IS(LCD)
    #define FORCE_SOFT_SPI                        // SD shares EXP2 here
  #endif

#elif ENABLED(FYSETC_MINI_12864)

  // LCD pins defined in pins/lcd/FYSETC_MINI_12864.h

  #define LCD_FORCE_SOFT_SPI                      // Panel has its own pins on EXP2
  #if SD_CONNECTION_IS(LCD)
    #define FORCE_SOFT_SPI                        // SD shares EXP2 here
  #endif

#elif IS_TFTGLCD_PANEL

  // Migrated to pins/lcd

#elif HAS_WIRED_LCD

  // Migrated to pins/lcd — the standard EXP1_01..08 / EXP2_03/05 slot-order
  // wiring is supplied by lcd/WIRED.h / RRD_SC.h via the de-facto EXP ports.

  #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
    #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
  #endif

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1               120
  #define BOARD_ST7920_DELAY_2                80
  #define BOARD_ST7920_DELAY_3               580
#endif

//
// NeoPixel LED
//
#ifndef BOARD_NEOPIXEL_PIN
  #define BOARD_NEOPIXEL_PIN                PD0
#endif

#ifndef NEOPIXEL2_PIN
  #define NEOPIXEL2_PIN                     PD1
#endif
