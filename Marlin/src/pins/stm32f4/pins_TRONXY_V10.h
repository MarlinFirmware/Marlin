/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if HOTENDS > 3 || E_STEPPERS > 3
  #error "Tronxy V10 supports up to 3 hotends / E steppers."
#endif

#define BOARD_INFO_NAME      "Tronxy V10"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

#define STEP_TIMER                             6
#define TEMP_TIMER                            14

//
// Servos
//
//#define SERVO0_PIN                        PB10

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #undef NO_EEPROM_SELECTED
  #if TRONXY_UI > 0
    #define EEPROM_AT24CXX
  #else
    #define FLASH_EEPROM_EMULATION
  #endif
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // SoC Flash (framework-arduinoststm32-maple/STM32F1/libraries/EEPROM/EEPROM.h)
  #define EEPROM_START_ADDRESS (0x8000000UL + (512 * 1024) - 2 * EEPROM_PAGE_SIZE)
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2KB, but will use 2x more (4KB)
  #define MARLIN_EEPROM_SIZE    EEPROM_PAGE_SIZE
#else
  #if ENABLED(EEPROM_AT24CXX)
    #define AT24CXX_SCL                     PB8
    #define AT24CXX_SDA                     PB9
    #define AT24CXX_WP                      PB7
  #else
    #define I2C_EEPROM                            // AT24C32
  #endif
  #define MARLIN_EEPROM_SIZE              0x1000  // 4K
#endif

//
// SPI Flash
//
//#define SPI_FLASH
#if ENABLED(SPI_FLASH)
  #define SPI_FLASH_SIZE                0x200000  // 2MB
  #define SPI_FLASH_CS_PIN                  PG15  // SPI2
  #define SPI_FLASH_MOSI_PIN                PB5
  #define SPI_FLASH_MISO_PIN                PB4
  #define SPI_FLASH_SCK_PIN                 PB3
#endif

//
// Limit Switches
//
#define X_MIN_PIN                           PC15
#define X_MAX_PIN                           PB0
#define Y_STOP_PIN                          PC14

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PE3
#endif

#if ENABLED(DUAL_Z_ENDSTOP_PROBE)
  #if NUM_Z_STEPPERS > 1 && Z_HOME_TO_MAX         // Swap Z1/Z2 for dual Z with max homing
    #define Z_MIN_PIN                       PF11
    #define Z_MAX_PIN                       PC13
  #else
    #define Z_MIN_PIN                       PC13
    #define Z_MAX_PIN                       PF11
  #endif
#else
  #ifndef Z_STOP_PIN
    #define Z_STOP_PIN                      PC13
  #endif
#endif
//
// Filament Sensors
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PE6   // MT_DET
#endif
#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                   PF12
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PF0
#define X_STEP_PIN                          PE5
#define X_DIR_PIN                           PF1

#define Y_ENABLE_PIN                        PF5
#define Y_STEP_PIN                          PF9
#define Y_DIR_PIN                           PF3

#define Z_ENABLE_PIN                        PA5
#define Z_STEP_PIN                          PA6
#define Z_DIR_PIN                           PF15

#define E0_ENABLE_PIN                       PF14
#define E0_STEP_PIN                         PB1
#define E0_DIR_PIN                          PF13

#define E1_ENABLE_PIN                       PG5
#define E1_STEP_PIN                         PD12
#define E1_DIR_PIN                          PG4

#define E2_ENABLE_PIN                       PF7
#define E2_STEP_PIN                         PF6
#define E2_DIR_PIN                          PF4

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC3   // TH1
#define TEMP_BED_PIN                        PC2   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PG7   // HEATER1
#define HEATER_BED_PIN                      PE2   // HOT BED
//#define HEATER_BED_INVERTING              true

#define FAN0_PIN                            PG0   // FAN0
#define FAN1_PIN                            PB6   // FAN1
#define FAN2_PIN                            PG9   // FAN2
#define FAN3_PIN                            PF10  // FAN3
#define CONTROLLER_FAN_PIN                  PD7   // BOARD FAN
#define FAN_SOFT_PWM

//
// Laser / Spindle
//
#if HAS_CUTTER
  #define SPINDLE_LASER_ENA_PIN             PB11  // wifi:TX
  #if ENABLED(SPINDLE_LASER_USE_PWM)
    #define SPINDLE_LASER_PWM_PIN           PB10  // wifi:RX-TIM2_CH3
    // The PWM pin definition const PinMap PinMap_PWM[] in PeripheralPins.c must be compounded here
    // See PWM_PIN(x) definition for details
  #endif
#endif

//
// Misc
//
#define BEEPER_PIN                          PA8

//#define LED_PIN                           PG10
#define PS_ON_PIN                           PG10  // Temporarily switch the machine with LED simulation

#if ENABLED(TRONXY_BACKUP_POWER)
  #define POWER_LOSS_PIN                    PF11  // Configure as drop-down input
#else
  #define POWER_LOSS_PIN                    PE1   // Output of LM393 comparator, configured as pullup
#endif
//#define POWER_LM393_PIN                   PE0   // +V for the LM393 comparator, configured as output high

#if ENABLED(TFT_TRONXY_X5SA)
  #error "TFT_TRONXY_X5SA is not yet supported."
#endif

#if 0

//
// TFT with FSMC interface
//
#if HAS_FSMC_TFT
  #define TFT_RESET_PIN                     PB12
  #define TFT_BACKLIGHT_PIN                 PG8

  #define LCD_USE_DMA_FSMC                        // Use DMA transfers to send data to the TFT

  #define TFT_CS_PIN                        PG12
  #define TFT_RS_PIN                        PG2

  //#define TFT_WIDTH                        480
  //#define TFT_HEIGHT                       320
  //#define TFT_PIXEL_OFFSET_X                48
  //#define TFT_PIXEL_OFFSET_Y                32
  //#define TFT_DRIVER                   ILI9488
  //#define TFT_BUFFER_SIZE                14400

  #if NEED_TOUCH_PINS
    #define TOUCH_CS_PIN                    PD11  // SPI1_NSS
    #define TOUCH_SCK_PIN                   PB13  // SPI1_SCK
    #define TOUCH_MISO_PIN                  PB14  // SPI1_MISO
    #define TOUCH_MOSI_PIN                  PB15  // SPI1_MOSI
  #endif

  #if (LCD_CHIP_INDEX == 1 && (TRONXY_UI == 1 || TRONXY_UI == 2)) || LCD_CHIP_INDEX == 3
    #define TOUCH_CALIBRATION_X           -17181
    #define TOUCH_CALIBRATION_Y            11434
    #define TOUCH_OFFSET_X                   501
    #define TOUCH_OFFSET_Y                    -9
  #elif LCD_CHIP_INDEX == 1 && TRONXY_UI == 4
    #define TOUCH_CALIBRATION_X            11166
    #define TOUCH_CALIBRATION_Y            17162
    #define TOUCH_OFFSET_X                   -10
    #define TOUCH_OFFSET_Y                   -16
  #elif LCD_CHIP_INDEX == 4 && TRONXY_UI == 3
    //#define TOUCH_CALIBRATION_X           8781
    //#define TOUCH_CALIBRATION_Y          11773
    //#define TOUCH_OFFSET_X                 -17
    //#define TOUCH_OFFSET_Y                 -16
    // Upside-down
    #define TOUCH_CALIBRATION_X            -8553
    #define TOUCH_CALIBRATION_Y           -11667
    #define TOUCH_OFFSET_X                   253
    #define TOUCH_OFFSET_Y                   331
  #elif LCD_CHIP_INDEX == 2
    #define TOUCH_CALIBRATION_X            17184
    #define TOUCH_CALIBRATION_Y            10604
    #define TOUCH_OFFSET_X                   -31
    #define TOUCH_OFFSET_Y                   -29
  #endif
#endif

#endif

//
// SD Card
//
#define ONBOARD_SDIO
#define SD_DETECT_PIN                       -1    // PF0, but not connected
#define SDIO_CLOCK                       4500000
#define SDIO_READ_RETRIES                     16
