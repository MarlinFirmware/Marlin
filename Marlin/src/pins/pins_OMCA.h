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
 * Open Motion controller with enable based extruders (Final!)
 *
 *                        ATmega644
 *
 *                        +---\/---+
 *            (D 0) PB0  1|        |40  PA0 (AI 0 / D31)
 *            (D 1) PB1  2|        |39  PA1 (AI 1 / D30)
 *       INT2 (D 2) PB2  3|        |38  PA2 (AI 2 / D29)
 *        PWM (D 3) PB3  4|        |37  PA3 (AI 3 / D28)
 *        PWM (D 4) PB4  5|        |36  PA4 (AI 4 / D27)
 *       MOSI (D 5) PB5  6|        |35  PA5 (AI 5 / D26)
 *       MISO (D 6) PB6  7|        |34  PA6 (AI 6 / D25)
 *        SCK (D 7) PB7  8|        |33  PA7 (AI 7 / D24)
 *                  RST  9|        |32  AREF
 *                  VCC 10|        |31  GND
 *                  GND 11|        |30  AVCC
 *                XTAL2 12|        |29  PC7 (D 23)
 *                XTAL1 13|        |28  PC6 (D 22)
 *       RX0 (D 8)  PD0 14|        |27  PC5 (D 21) TDI
 *       TX0 (D 9)  PD1 15|        |26  PC4 (D 20) TDO
 *  INT0 RX1 (D 10) PD2 16|        |25  PC3 (D 19) TMS
 *  INT1 TX1 (D 11) PD3 17|        |24  PC2 (D 18) TCK
 *       PWM (D 12) PD4 18|        |23  PC1 (D 17) SDA
 *       PWM (D 13) PD5 19|        |22  PC0 (D 16) SCL
 *       PWM (D 14) PD6 20|        |21  PD7 (D 15) PWM
 *                        +--------+
 *
 * REF http://sanguino.cc/hardware
 */

/**
 * Rev B    26 DEC 2016
 *
 * added pointer to a current Arduino IDE extension
 *
 */

/**
 * A useable Arduino IDE extension (board manager) can be found at
 * https://github.com/Lauszus/Sanguino
 *
 * This extension has been tested on Arduino 1.6.12 & 1.8.0
 *
 * Here's the JSON path:
 * https://raw.githubusercontent.com/Lauszus/Sanguino/master/package_lauszus_sanguino_index.json
 *
 * When installing select 1.0.2
 *
 * Installation instructions can be found at https://learn.sparkfun.com/pages/CustomBoardsArduino
 * Just use the above JSON URL instead of Sparkfun's JSON.
 *
 * Once installed select the Sanguino board and then select the CPU.
 *
 */

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__)
  #error "Oops! Select 'Sanguino' in 'Tools > Board' and 'ATmega644' or 'ATmega644P' in 'Tools > Processor.'"
#endif

#define BOARD_NAME "Final OMCA"

//
// Limit Switches
//
#define X_STOP_PIN         0
#define Y_STOP_PIN         1
#define Z_STOP_PIN         2

//
// Steppers
//
#define X_STEP_PIN         26
#define X_DIR_PIN          25
#define X_ENABLE_PIN       10

#define Y_STEP_PIN         28
#define Y_DIR_PIN          27
#define Y_ENABLE_PIN       10

#define Z_STEP_PIN         23
#define Z_DIR_PIN          22
#define Z_ENABLE_PIN       10

#define E0_STEP_PIN        24
#define E0_DIR_PIN         21
#define E0_ENABLE_PIN      10

#define E1_STEP_PIN        -1   // 21
#define E1_DIR_PIN         -1   // 20
#define E1_ENABLE_PIN      -1   // 19

#define E2_STEP_PIN        -1   // 21
#define E2_DIR_PIN         -1   // 20
#define E2_ENABLE_PIN      -1   // 18

//
// Temperature Sensors
//
#define TEMP_0_PIN          0   // Analog Input
#define TEMP_1_PIN          1   // Analog Input
#define TEMP_BED_PIN        2   // Analog Input (1,2 or I2C)

//
// Heaters / Fans
//
#define HEATER_0_PIN        3   // DONE PWM on RIGHT connector
#define HEATER_BED_PIN      4

#ifndef FAN_PIN
  #define FAN_PIN          14   // PWM on MIDDLE connector
#endif

//
// Misc. Functions
//
#define SDSS               11

#define I2C_SCL_PIN        16
#define I2C_SDA_PIN        17

// future proofing
#define __FS  20
#define __FD  19
#define __GS  18
#define __GD  13

#define UNUSED_PWM         14   // PWM on LEFT connector
