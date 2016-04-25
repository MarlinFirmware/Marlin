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
 * SAV MkI pin assignments (AT90USB1286)
 * Requires the Teensyduino software with Teensy++ 2.0 selected in Arduino IDE!
 * http://www.pjrc.com/teensy/teensyduino.html
 * RepRap Clone Wars project board.
 */

#ifndef __AVR_AT90USB1286__
  #error Oops!  Make sure you have 'Teensy++ 2.0' selected from the 'Tools -> Boards' menu.
#endif

#define DEFAULT_MACHINE_NAME    "SAV MkI"
#define DEFAULT_SOURCE_CODE_URL "https://github.com/fmalpartida/Marlin/tree/SAV-MkI-config"

#define LARGE_FLASH        true

#define X_STEP_PIN         0
#define X_DIR_PIN          1
#define X_ENABLE_PIN       39

#define Y_STEP_PIN         2
#define Y_DIR_PIN          3
#define Y_ENABLE_PIN       38

#define Z_STEP_PIN         4
#define Z_DIR_PIN          5
#define Z_ENABLE_PIN       23

#define E0_STEP_PIN        6
#define E0_DIR_PIN         7
#define E0_ENABLE_PIN      19

#define HEATER_0_PIN       21  // Extruder
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define HEATER_BED_PIN     20  // Bed
#define FAN_PIN            16  // Fan   -- from Teensyduino environment.
                               // For the fan and Teensyduino uses a different pin mapping.

#define X_STOP_PIN         13
#define Y_STOP_PIN         14
//#define Z_STOP_PIN         15
#define Z_STOP_PIN         36  // For inductive sensor.

#define TEMP_0_PIN          7  // Extruder / Analog pin numbering
#define TEMP_BED_PIN        6  // Bed / Analog pin numbering

#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1

#if DISABLED(SDSUPPORT)
  // these pins are defined in the SD library if building with SD support
  #define SCK_PIN          9
  #define MISO_PIN         11
  #define MOSI_PIN         10
#endif
#define SDSS               20         // PB0 - 8 in marlin env.

// Extension header pin mapping
// ----------------------------
//  SCL (I2C)-D0    A0 (An), IO
//  SDA (I2C)-D1    A1 (An), IO
//  RX1-D2          A2 (An), IO
//  TX1-D3          A3 (An), IO
//  PWM-D24         A4 (An), IO
//  5V              GND
//  12V             GND
#define EXT_AUX_SCL_D0            0  // 0 (teensy), 24 (marlin)
#define EXT_AUX_SDA_D1            1  // 1 (teensy), 25 (marlin)
#define EXT_AUX_RX1_D2            26 // 2 (teensy), 26 (marlin)
#define EXT_AUX_TX1_D3            27 // 3 (teensy), 27 (marlin)
#define EXT_AUX_PWM_D24           12 // 24 (teensy), 12 (marlin)
#define EXT_AUX_A0                 0 // Analog
#define EXT_AUX_A0_IO             40 // Digital IO, 38 (teensy), 40 (marlin)
#define EXT_AUX_A1                 1 // Analog
#define EXT_AUX_A1_IO             41 // Digital IO, 39 (teensy), 41 (marlin)
#define EXT_AUX_A2                 2 // Analog
#define EXT_AUX_A2_IO             42 // Digital IO, 40 (teensy), 42 (marlin)
#define EXT_AUX_A3                 3 // Analog
#define EXT_AUX_A3_IO             43 // Digital IO, 41 (teensy), 43 (marlin)
#define EXT_AUX_A4                 4 // Analog
#define EXT_AUX_A4_IO             44 // Digital IO, 42 (teensy), 44 (marlin)


#define SDPOWER            -1
#define LED_PIN            -1
#define PS_ON_PIN          -1
#define SD_DETECT_PIN      -1

#define BEEPER_PIN         -1
#define LCD_PINS_RS        -1
#define LCD_PINS_ENABLE    -1
#define LCD_PINS_D4        -1
#define LCD_PINS_D5        -1
#define LCD_PINS_D6        -1
#define LCD_PINS_D7        -1

#if ENABLED(SAV_3DLCD)
  // For LCD SHIFT register LCD
  #define SR_DATA_PIN         EXT_AUX_SDA_D1
  #define SR_CLK_PIN          EXT_AUX_SCL_D0
#endif  // SAV_3DLCD

#if ENABLED(SAV_3DLCD) || ENABLED(SAV_3DGLCD)
  #define BTN_EN1            EXT_AUX_A1_IO
  #define BTN_EN2            EXT_AUX_A0_IO
  #define BTN_ENC            EXT_AUX_PWM_D24

  #define KILL_PIN           EXT_AUX_A2_IO
  #define HOME_PIN           EXT_AUX_A4_IO
#endif // SAV_3DLCD || SAV_3DGLCD

#define SERVO0_PIN         41 // In teensy's pin definition for pinMode (in servo.cpp)
