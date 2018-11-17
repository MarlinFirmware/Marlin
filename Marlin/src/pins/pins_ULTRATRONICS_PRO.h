/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * ReprapWorld ULTRATRONICS v1.0
 */

#ifndef ARDUINO_ARCH_SAM
  #error "Oops! Select 'Arduino Due' in 'Tools > Board.'"
#endif

#define BOARD_NAME "Ultratronics v1.0"

//
// Servos
//
#if NUM_SERVOS > 0
  #define SERVO0_PIN       11
  #if NUM_SERVOS > 1
    #define SERVO1_PIN     12
  #endif
#endif

//
// Limit Switches
//
#define X_MIN_PIN          31
#define X_MAX_PIN          30
#define Y_MIN_PIN          12
#define Y_MAX_PIN          11
#define Z_MIN_PIN          29
#define Z_MAX_PIN          28

//
// Steppers
//
#define X_STEP_PIN         35
#define X_DIR_PIN          34
#define X_ENABLE_PIN       37

#define Y_STEP_PIN         22
#define Y_DIR_PIN          23
#define Y_ENABLE_PIN       33

#define Z_STEP_PIN         25
#define Z_DIR_PIN          26
#define Z_ENABLE_PIN       24

#define E0_STEP_PIN        47
#define E0_DIR_PIN         46
#define E0_ENABLE_PIN      48

#define E1_STEP_PIN        44
#define E1_DIR_PIN         36
#define E1_ENABLE_PIN      45

#define E2_STEP_PIN        42
#define E2_DIR_PIN         41
#define E2_ENABLE_PIN      43

#define E3_STEP_PIN        39
#define E3_DIR_PIN         38
#define E3_ENABLE_PIN      40

//
// Temperature Sensors
//
#define TEMP_0_PIN          0   // Analog Input
#define TEMP_1_PIN          2   // Analog Input
#define TEMP_2_PIN          3   // Analog Input
#define TEMP_3_PIN          4   // Analog Input
#define TEMP_BED_PIN        1   // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN        3
#define HEATER_1_PIN        8
#define HEATER_2_PIN        7
#define HEATER_3_PIN        9
#define HEATER_BED_PIN      2

#ifndef FAN_PIN
  #define FAN_PIN           6
#endif
#define FAN2_PIN            5

//
// Misc. Functions
//
#define SDSS               59
#define SD_DETECT_PIN      60
#define LED_PIN            13
#define PS_ON_PIN          32

//
// SPI Buses
//

#define DAC0_SYNC          53   // PB14
#define SPI_CHAN_DAC        1

#define SPI_CHAN_EEPROM1   -1
#define SPI_EEPROM1_CS     -1
#define SPI_EEPROM2_CS     -1
#define SPI_FLASH_CS       -1

// SPI for Max6675 or Max31855 Thermocouple
#define MAX6675_SS_PIN     65
#define MAX31855_SS0       65
#define MAX31855_SS1       52
#define MAX31855_SS2       50
#define MAX31855_SS3       51

#define ENC424_SS          61

//
// LCD / Controller
//

#define BEEPER_PIN         27

#if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)

  #define LCD_PINS_RS      A8     // CS chip select / SS chip slave select
  #define LCD_PINS_ENABLE  MOSI   // SID (MOSI)
  #define LCD_PINS_D4      SCK    // SCK (CLK) clock

  #define BTN_EN1          20
  #define BTN_EN2          21
  #define BTN_ENC          64

#endif // REPRAPWORLD_GRAPHICAL_LCD
