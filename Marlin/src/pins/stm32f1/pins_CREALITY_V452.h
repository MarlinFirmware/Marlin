/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Creality v4.5.2 (STM32F103RET6) board pin assignments
 */

#if NOT_TARGET(__STM32F1__)
  #error "Oops! Select an STM32F1 board in 'Tools > Board.'"
#elif HOTENDS > 1 || E_STEPPERS > 1
  #error "CREALITY_V452 supports up to 1 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "Creality v4.5.2"
#define DEFAULT_MACHINE_NAME "Creality3D"

//
// Release PB4 (Z_STEP_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

#define BOARD_NO_NATIVE_USB

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
  //#define SDCARD_EEPROM_EMULATION
#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PA11
  #define IIC_EEPROM_SCL                    PA12
  #define MARLIN_EEPROM_SIZE               0x800  // 2Kb (24C16)
#elif ENABLED(SDCARD_EEPROM_EMULATION)
  #define MARLIN_EEPROM_SIZE               0x800  // 2Kb
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PC4
#define Y_STOP_PIN                          PC5
#define Z_STOP_PIN                          PA4

#define FIL_RUNOUT_PIN                      PA7

//
// Probe
//
#define PROBE_TARE_PIN                      PA5
#define PROBE_ENABLE_PIN                    PC6   // Optoswitch to Enable Z Probe

//
// Steppers
//
#define X_ENABLE_PIN                        PC3
#define X_STEP_PIN                          PB8
#define X_DIR_PIN                           PB7

#define Y_ENABLE_PIN                        PC3
#define Y_STEP_PIN                          PB6
#define Y_DIR_PIN                           PB5

#define Z_ENABLE_PIN                        PC3
#define Z_STEP_PIN                          PB4
#define Z_DIR_PIN                           PB3

#define E0_ENABLE_PIN                       PC3
#define E0_STEP_PIN                         PC2
#define E0_DIR_PIN                          PB9

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PB1   // TH1
#define TEMP_BED_PIN                        PB0   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA1   // HEATER1
#define HEATER_BED_PIN                      PA2   // HOT BED

#define FAN_PIN                             PA0   // FAN
#define FAN_SOFT_PWM

//
// SD Card
//
#define SD_DETECT_PIN                       PC7
#define NO_SD_HOST_DRIVE                          // SD is only seen by the printer

#define SDIO_SUPPORT                              // Extra added by Creality
#define SDIO_CLOCK                       6000000  // In original source code overridden by Creality in sdio.h

//
// Misc. Functions
//
#define CASE_LIGHT_PIN                      PA6
