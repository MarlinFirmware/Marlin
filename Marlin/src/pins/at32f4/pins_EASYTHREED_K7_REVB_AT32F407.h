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
 */

#pragma once

/**
 * EasyThreed K7 Rev. B ET4000+ pin assignments
 *
 * Board marking: AT32F407ARCT7
 * Build profile: AT32F407RCT7
 *
 * Derived from the MKS Robin Lite mapping used by the known-working
 * Vexisu K7 Rev. B firmware. That firmware has run and printed on the
 * exact target machine. Individual functions still require staged
 * native-AT32 hardware validation.
 *
 * The K7 has no heated bed, so no bed heater or bed sensor is exposed.
 */

#ifndef ARDUINO_ARCH_AT32
  #error "AT32 HAL required for this board"
#endif

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "EasyThreed K7 Rev. B only supports 1 hotend / E stepper."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "EasyThreed K7 Rev. B ET4000+"
#endif

#define BOARD_NO_NATIVE_USB
#define DISABLE_JTAG

//
// External 24C16 EEPROM
//
// The known-working Maple firmware uses I2C1 on PB6 / PB7.
// Software I2C avoids relying on the incomplete AT32 Wire singleton.
//
#if ANY(NO_EEPROM_SELECTED, I2C_EEPROM)
  #define I2C_EEPROM
  #define SOFT_I2C_EEPROM
  #define I2C_SCL_PIN                       PB6
  #define I2C_SDA_PIN                       PB7
  #define MARLIN_EEPROM_SIZE             0x800U  // 2K (24C16)
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PC13
#define Y_STOP_PIN                          PC0
#define Z_MIN_PIN                           PC12
#define Z_MAX_PIN                           PB9

//
// Steppers
//
#define X_STEP_PIN                          PC6
#define X_DIR_PIN                           PB12
#define X_ENABLE_PIN                        PB10

#define Y_STEP_PIN                          PB11
#define Y_DIR_PIN                           PB2
#define Y_ENABLE_PIN                        PB10

#define Z_STEP_PIN                          PB1
#define Z_DIR_PIN                           PC5
#define Z_ENABLE_PIN                        PB10

#define E0_STEP_PIN                         PC4
#define E0_DIR_PIN                          PA5
#define E0_ENABLE_PIN                       PA4

//
// Hotend / Fan
//
#define HEATER_0_PIN                        PC9
#define FAN0_PIN                            PA8

//
// Temperature Sensor
//
#define TEMP_0_PIN                          PA0

//
// Filament runout input
//
#define FIL_RUNOUT_PIN                      PB8

/**
 *                 ------
 *              PD2 | 1  2 | PB3
 *              PB5 | 3  4 | PA11
 *              PB4 | 5  6 | PC1
 *              PC3 | 7  8 | PC2
 *              GND | 9 10 | 5V
 *                 ------
 *                   EXP3
 */
#define EXP3_01_PIN                         PD2
#define EXP3_02_PIN                         PB3
#define EXP3_03_PIN                         PB5
#define EXP3_04_PIN                         PA11
#define EXP3_05_PIN                         PB4
#define EXP3_06_PIN                         PC1
#define EXP3_07_PIN                         PC3
#define EXP3_08_PIN                         PC2
#define EXP3_09_PIN                         -1    // GND
#define EXP3_10_PIN                         -1    // 5V

//
// Four-button K7 Rev. B user interface
//
#if ENABLED(EASYTHREED_UI)
  #define BTN_HOME                   EXP3_07_PIN  // INPUT_PULLUP (unused)
  #define BTN_FEED                   EXP3_02_PIN  // Run E forward
  #define BTN_RETRACT                EXP3_03_PIN  // Run E backward
  #define BTN_PRINT                  EXP3_04_PIN  // Start file print
  #define BTN_HOME_GND               EXP3_08_PIN  // OUTPUT LOW
  #define BTN_FEED_GND               EXP3_05_PIN  // OUTPUT LOW
  #define BTN_RETRACT_GND            EXP3_06_PIN  // OUTPUT LOW
  #define EASYTHREED_LED_PIN         EXP3_01_PIN
#endif

//
// Motor current PWM
//
#define MOTOR_CURRENT_PWM_XY_PIN            PB0
#define MOTOR_CURRENT_PWM_Z_PIN             PA7
#define MOTOR_CURRENT_PWM_E_PIN             PA6
#define MOTOR_CURRENT_PWM_RANGE (65535/10/3.3)
#define DEFAULT_PWM_MOTOR_CURRENT { 1000, 1000, 1000 }

//
// Early safe output states
//
// Keep the heater, fan, and motor-current outputs inactive and disable
// all stepper drivers before serial, media, EEPROM, and subsystem init.
//
#define BOARD_PREINIT() do { \
  OUT_WRITE(HEATER_0_PIN, LOW); \
  OUT_WRITE(FAN0_PIN, LOW); \
  OUT_WRITE(X_ENABLE_PIN, HIGH); \
  OUT_WRITE(E0_ENABLE_PIN, HIGH); \
  OUT_WRITE(MOTOR_CURRENT_PWM_XY_PIN, LOW); \
  OUT_WRITE(MOTOR_CURRENT_PWM_Z_PIN, LOW); \
  OUT_WRITE(MOTOR_CURRENT_PWM_E_PIN, LOW); \
} while (0)

//
// SD Card
//
#define SD_DETECT_PIN                       PC10
#define SPI_DEVICE                             2
#define SD_SCK_PIN                          PB13
#define SD_MISO_PIN                         PB14
#define SD_MOSI_PIN                         PB15
#define SD_SS_PIN                           PA15
