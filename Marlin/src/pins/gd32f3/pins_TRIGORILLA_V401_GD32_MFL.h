/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Anycubic TriGorilla Gen V4.0.1 (GD32F303RGT6) - Kobra 2 Neo pin assignments
 *
 * Pin assignments are taken from the Anycubic Kobra 2 Neo firmware sources,
 * which build this board as an STM32F103RET6: https://github.com/ANYCUBIC-3D/Kobra2_Neo
 */

#include "env_validate.h"

#define BOARD_INFO_NAME      "Trigorilla Gen V4.0.1"
#define DEFAULT_MACHINE_NAME "Anycubic Kobra 2 Neo"

#define BOARD_NO_NATIVE_USB

/**
 * Free PA15, PB3 and PB4 from the JTAG pins (SWD is kept). PA15 is the shared
 * stepper enable, so with JTAG left enabled the drivers can never be turned on
 * and no motor moves; PB3/PB4 are the rotary encoder.
 */
#define DISABLE_JTAG

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define FLASH_EEPROM_EMULATION                  // Last 4K of flash bank 0
  //#define SDCARD_EEPROM_EMULATION
#endif

//
// Limit Switches
//
#define X_MIN_PIN                           PB11
#define X_MAX_PIN                           -1
#define Y_MIN_PIN                           PC13
#define Y_MAX_PIN                           -1
#define Z_MIN_PIN                           PA1
#define Z_MAX_PIN                           PA0

//
// Steppers
//
#define X_ENABLE_PIN                        PA15
#define X_STEP_PIN                          PA12
#define X_DIR_PIN                           PA11

#define Y_ENABLE_PIN                X_ENABLE_PIN
#define Y_STEP_PIN                          PA9
#define Y_DIR_PIN                           PA8

#define Z_ENABLE_PIN                X_ENABLE_PIN
#define Z_STEP_PIN                          PB0
#define Z_DIR_PIN                           PB1

#define E0_ENABLE_PIN               X_ENABLE_PIN
#define E0_STEP_PIN                         PB15
#define E0_DIR_PIN                          PB14

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC3   // TH1
#define TEMP_1_PIN                          -1    // TH2
#define TEMP_BED_PIN                        PC1   // TB1

//
// Heaters
//
#define HEATER_0_PIN                        PB8   // HE1
#define HEATER_1_PIN                        -1
#define HEATER_BED_PIN                      PB9   // HB1

//
// Fans
//
#define FAN0_PIN                            PB5   // FAN0
#define FAN1_PIN                            PB13  // FAN1
#define FAN2_PIN                            PB12  // FAN2, auto fan for E0
#define CONTROLLER_FAN_PIN              FAN2_PIN

//
// Misc
//
#define BEEPER_PIN                          PB7
#define LED_PIN                             -1
#define CASE_LIGHT_PIN                      -1
#define FIL_RUNOUT_PIN                      PC15
#define POWER_LOSS_PIN                      PC2
#define POWER_MONITOR_VOLTAGE_PIN           PC2

//
// Host serial port. USART1 in GD32 numbering (PA2/PA3).
//
#define UART1_TX_PIN                        PA2
#define UART1_RX_PIN                        PA3

//
// TFT (ST7789 over SPI)
//
#define TFT_POWER_PIN                       PB6   // Panel supply enable (vendor POWER_CTRL)
#define TFT_BACKLIGHT_PIN                   PC0
#define TFT_CS_PIN                          PA4
#define TFT_DC_PIN                          PA6
#define TFT_SCK_PIN                         PA5
#define TFT_MOSI_PIN                        PA7

//
// Rotary encoder
//
#define BTN_ENC                             PB4
#define BTN_EN1                             PB10
#define BTN_EN2                             PB3

//
// SD Card
//
#define ONBOARD_SDIO

#if ENABLED(ONBOARD_SDIO)
  #define SDIO_D0_PIN                       PC8
  #define SDIO_D1_PIN                       PC9
  #define SDIO_D2_PIN                       PC10
  #define SDIO_D3_PIN                       PC11
  #define SDIO_CK_PIN                       PC12
  #define SDIO_CMD_PIN                      PD2
#else
  #define SD_SS_PIN                         PC11  // SDIO_D3_PIN
  #define SD_SCK_PIN                        PC12  // SDIO_CK_PIN
  #define SD_MISO_PIN                       PC8   // SDIO_D0_PIN
  #define SD_MOSI_PIN                       PD2   // SDIO_CMD_PIN
  #define SOFTWARE_SPI
#endif

#define SD_DETECT_PIN                       PA10
