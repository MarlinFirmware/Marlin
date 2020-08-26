/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if !defined(STM32F4) && !defined(STM32F4xx)
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "LERDGE S supports up to 2 hotends / E-steppers."
#endif

#define BOARD_INFO_NAME      "Lerdge S"
#define DEFAULT_MACHINE_NAME "LERDGE"

#define STEP_TIMER                             4
#define TEMP_TIMER                             2

//#define I2C_EEPROM

//
// Servos
//
#define SERVO0_PIN                          PD12  //confirmed
//#define SERVO1_PIN                        -1

//
// Limit Switches
//
#define X_MIN_PIN                           PG9   //confirmed
#define Y_MIN_PIN                           PG10  //confirmed
#define Z_MIN_PIN                           PG11  //confirmed

#define X_MAX_PIN                           PG12  //confirmed
#define Y_MAX_PIN                           PG13  //confirmed
#define Z_MAX_PIN                           PG14  //confirmed

//
// Filament runout
//
#define FIL_RUNOUT_PIN                      PC5   //confirmed

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PG8   //confirmed
#endif

//
// Steppers
//
#define X_STEP_PIN                          PF7   //confirmed
#define X_DIR_PIN                           PF8   //confirmed
#define X_ENABLE_PIN                        PF6   //confirmed

#define Y_STEP_PIN                          PF10  //confirmed
#define Y_DIR_PIN                           PF11  //confirmed
#define Y_ENABLE_PIN                        PF9   //confirmed

#define Z_STEP_PIN                          PF13  //confirmed
#define Z_DIR_PIN                           PF14  //confirmed
#define Z_ENABLE_PIN                        PF12  //confirmed

#define E0_STEP_PIN                         PG0   //confirmed
#define E0_DIR_PIN                          PG1   //confirmed
#define E0_ENABLE_PIN                       PF15  //confirmed

#define E1_STEP_PIN                         PG3   //confirmed
#define E1_DIR_PIN                          PG4   //confirmed
#define E1_ENABLE_PIN                       PG2   //confirmed

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC0   // See below for activation of thermistor readings
#define TEMP_1_PIN                          PC1   // See below for activation of thermistor readings
#define TEMP_BED_PIN                        PC3   //confirmed

// Lergde-S can choose thermocouple/thermistor mode in software.
// For use with thermistors, these pins must be OUT/LOW.
// This is done automatically.
#define TEMP_0_TR_ENABLE_PIN                PF3
#define TEMP_1_TR_ENABLE_PIN                PF4

// MAX6675 Cold-Junction-Compensated K-Thermocouple to Digital Converter (0°C to +1024°C)
// https://datasheets.maximintegrated.com/en/ds/MAX6675.pdf

#define MAX6675_SCK_PIN                     PB3   // max6675 datasheet: SCK pin, found with multimeter, not tested
#define MAX6675_DO_PIN                      PB4   // max6675 datasheet: SO pin, found with multimeter, not tested
#define MAX6675_SS_PIN                      PC4   // max6675 datasheet: /CS pin, found with multimeter, not tested and likely wrong

// Expansion board with second max6675
// Warning: Some boards leave the slot unpopulated.

//#define MAX6675_SCK2_PIN                  PB3   // max6675 datasheet: SCK pin, found with multimeter, not tested
//#define MAX6675_DO2_PIN                   PB4   // max6675 datasheet: SO pin, found with multimeter, not tested
//#define MAX6675_SS2_PIN                   PF1   // max6675 datasheet: /CS pin, found with multimeter, not tested

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA0   //confirmed
#define HEATER_1_PIN                        PA1   //confirmed
#define HEATER_BED_PIN                      PA3   //confirmed

#define FAN_PIN                             PA15  // heater 0 fan 1 //confirmed
#define FAN1_PIN                            PB10  // heater 1 fan 2 //confirmed
#define FAN2_PIN                            PF5   // heater 0 fan 2 and heater 1 fan 1 (two sockets, switched together) //confirmed

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                   PF5
#endif

//
// Průša i3 MK2 Multi Material Multiplexer Support
//
//#define E_MUX0_PIN                        -1
//#define E_MUX1_PIN                        -1

//
// LED / Lighting
//
//Lerdge-S board has two LED connectors (this is the one on the mainboard)
#define CASE_LIGHT_PIN                      PC7   //confirmed

//on the dual extrusion addon board is a RGB connector
#define RGB_LED_R_PIN                       PC7   // Shared with the mainboard LED light connector (CASE_LIGHT_PIN), confirmed
#define RGB_LED_G_PIN                       PB0   //confirmed
#define RGB_LED_B_PIN                       PB1   //confirmed

//
// Misc. Functions
//
#define SDSS                                PC11  // SD is working using SDIO, not sure if this definition is needed?
#define LED_PIN                             PC6   // Mainboard soldered green LED, confirmed
#define PS_ON_PIN                           PB2   // Board has a power module connector, confirmed
#define KILL_PIN                            -1    // There is no reset button on the LCD
#define POWER_LOSS_PIN                      -1    // PB2 could be used for this as well

//
// SD support
//
#define SDIO_SUPPORT

#define SCK_PIN                             PC12  //confirmed working
#define MISO_PIN                            PC8   //confirmed working
#define MOSI_PIN                            PD2   //confirmed working
#define SS_PIN                              PC11  //confirmed working

#define SD_DETECT_PIN                       PG15  //confirmed

//
// Persistent Storage
// If no option is selected below the SD Card will be used
// (this section modelled after pins_LONGER3D_LK.h)
// Warning: Not tested yet! Pins traced with multimeter, mistakes are possible
//#define SPI_EEPROM

#if ENABLED(SPI_EEPROM)
  // Lerdge has an SPI EEPROM Winbond W25Q128 (128Mbits) https://www.pjrc.com/teensy/W25Q128FV.pdf
  #define SPI_CHAN_EEPROM1                     1
  #define SPI_EEPROM1_CS                    PB12  // datasheet: /CS pin, found with multimeter, not tested
  #define EEPROM_SCK                        PB13  // datasheet: CLK pin, found with multimeter, not tested
  #define EEPROM_MISO                       PB14  // datasheet: DO pin, found with multimeter, not tested
  #define EEPROM_MOSI                       PB15  // datasheet: DI pin, found with multimeter, not tested
  #define EEPROM_PAGE_SIZE               0x1000U  // 4KB (from datasheet)
  #define MARLIN_EEPROM_SIZE 16UL * (EEPROM_PAGE_SIZE)   // Limit to 64KB for now...
#else
  #define MARLIN_EEPROM_SIZE              0x800U  // On SD, Limit to 2KB, require this amount of RAM
#endif

//
// LCD / Controller
//

// The LCD is initialized in FSMC mode
#define BEEPER_PIN                          PD13  //confirmed

#define BTN_EN1                             PC14  //confirmed
#define BTN_EN2                             PC15  //confirmed
#define BTN_ENC                             PC13  //confirmed

#define TFT_RESET_PIN                       PD6   //confirmed
#define TFT_BACKLIGHT_PIN                   PD3   //confirmed

#define TFT_CS_PIN                          PD7   // TFT works
#define TFT_RS_PIN                          PD11  // TFT works

// There is touch, but calibration is off
#define TOUCH_CS_PIN                        PB6
#define TOUCH_SCK_PIN                       PB3
#define TOUCH_MOSI_PIN                      PB5
#define TOUCH_MISO_PIN                      PB4
