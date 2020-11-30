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

/**
 * Arduino Mega for Tronxy X5S-2E, etc.
 */

#if NOT_TARGET(__AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#elif HOTENDS > 2 || E_STEPPERS > 2
  #error "TRONXY-V3-1.0 supports only 2 hotends/E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "TRONXY-V3-1.0"

//
// Servos
//
#define SERVO1_PIN         12   // 2560 PIN 25/PB6

//
// Import RAMPS 1.4 pins
//
#include "pins_RAMPS.h"

/**
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *  ===== AUX-1 connector =====
 *
 *           2  4  6  8
 *       #----------------#
 *    #2 |   °  °  °  °   |
 *    #1 |   °  °  °  °   |
 * NOTCH #------    ------#
 *           1  3  5  7
 *
 * ###################################
 * # Pin | ATMEGA2560 Pin  | Arduino #
 * ###################################
 * #  1  |       VCC       |     +   #
 * #  2  |       VCC       |     +   #
 * #  3  |       GND       |     -   #
 * #  4  |       GND       |     -   #
 * #  5  |       N/C       |         #
 * #  6  |  3 / PE1 (TXD0) |    D1   #
 * #  7  |       N/C       |         #
 * #  8  |  2 / PE0 (RXD0) |    D0   #
 * ###################################
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *  ===== Limit Switch connectors =====
 *
 *               ############## X+ #################
 *        X+     # Pin | ATMEGA2560 Pin  | Arduino #
 *      1 2 3    ###################################
 *    #- --- -#  #  1  |  6 / PE4 (INT4) |    D2   #
 *    | ° ° ° |  #  2  |       GND       |     -   #
 *    #-------#  #  3  |       VCC       |     +   #
 *               ###################################
 *
 *               ############## X- #################
 *        X-     # Pin | ATMEGA2560 Pin  | Arduino #
 *      1 2 3    ###################################
 *    #- --- -#  #  1  |  7 / PE5 (INT5) |    D3   #
 *    | ° ° ° |  #  2  |       GND       |     -   #
 *    #-------#  #  3  |       VCC       |     +   #
 *               ###################################
 *
 *               ############## Y+ #################
 *        Y+     # Pin | ATMEGA2560 Pin  | Arduino #
 *      1 2 3    ###################################
 *    #- --- -#  #  1  | 63/PJ0 (PCINT9) |   D15   #
 *    | ° ° ° |  #  2  |       GND       |     -   #
 *    #-------#  #  3  |       VCC       |     +   #
 *               ###################################
 *
 *               ############## Y- #################
 *        Y-     # Pin | ATMEGA2560 Pin  | Arduino #
 *      1 2 3    ###################################
 *    #- --- -#  #  1  | 64/PJ1 (PCINT10)|   D14   #
 *    | ° ° ° |  #  2  |       GND       |     -   #
 *    #-------#  #  3  |       VCC       |     +   #
 *               ###################################
 *
 *               ############## Z+ #################
 *        Z+     # Pin | ATMEGA2560 Pin  | Arduino #
 *      1 2 3    ###################################
 *    #- --- -#  #  1  | 45 / PD2 (INT2) |   D19   #
 *    | ° ° ° |  #  2  |       GND       |     -   #
 *    #-------#  #  3  |       VCC       |     +   #
 *               ###################################
 *
 *               ############## Z- #################
 *        Z-     # Pin | ATMEGA2560 Pin  | Arduino #
 *      1 2 3    ###################################
 *    #- --- -#  #  1  | 46 / PD3 (INT3) |   D18   #
 *    | ° ° ° |  #  2  |       GND       |     -   #
 *    #-------#  #  3  |       VCC       |     +   #
 *               ###################################
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *  ===== EXP1/EXP2 connectors =====
 *
 * (NOTE ORDER)   EXP2                        EXP1
 *           2  4  6  8 10               2  4  6  8 10
 *       #-------------------#       #-------------------#
 *    #2 |   °  °  °  °  °   |    #2 |   °  °  °  °  °   |
 *    #1 |   °  °  °  °  °   |    #1 |   °  °  °  °  °   |
 * NOTCH #--------    -------# NOTCH #--------    -------#
 *           1  3  5  7  9               1  3  5  7  9
 *
 * ############# EXP1 ################
 * # Pin | ATMEGA2560 Pin  | Arduino #
 * ###################################
 * #  1  |    53 / PC0     |   D37   #
 * #  2  |    55 / PC2     |   D35   #
 * #  3  | 12 / PH0 (RXD2) |   D17   #
 * #  4  | 13 / PH1 (TXD2) |   D16   #
 * #  5  |    77 / PA1     |   D23   #
 * #  6  |    75 / PA3     |   D25   #
 * #  7  |    73 / PA5     |   D27   #
 * #  8  |    71 / PA7     |   D29   #
 * #  9  |       GND       |     -   #
 * # 10  |       VCC       |     +   #
 * ###################################
 *
 * ############# EXP2 ################
 * # Pin | ATMEGA2560 Pin  | Arduino #
 * ###################################
 * #  1  | 22 / PB3 (MISO) |   D50   #
 * #  2  | 20 / PB1 (SCK)  |   D52   #
 * #  3  |    59 / PC6     |   D31   #
 * #  4  |       N/C       |         #
 * #  5  |    57 / PC4     |   D33   #
 * #  6  | 21 / PB2 (MOSI) |   D51   #
 * #  7  |       N/C       |         #
 * #  8  |   30 / !RESET   |  RESET  #
 * #  9  |       GND       |     -   #
 * # 10  |    51 / PG0     |   D41   #
 * ###################################
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *  ===== ICSP connector =====
 *
 *        2  4  6
 *      #---------#
 *      | °  °  ° |
 *      | °  °  ° |
 *      #---------#
 *        1  3  5
 *
 * ##################################
 * # Pin | ATMEGA2560 Pin | Arduino #
 * ##################################
 * #  1  | 22 / PB3 (MISO)|    50   #
 * #  2  |       VCC      |     +   #
 * #  3  | 20 / PB1 (SCK) |    52   #
 * #  4  | 21 / PB2 (MOSI)|    51   #
 * #  5  | 30 / !RESET    |  RESET  #
 * #  6  |       GND      |     -   #
 * ##################################
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *  ===== SERVOS connector =====
 *
 *        2  4  6
 *      #---------#
 *      | °  °  ° |
 *      | °  °  ° |
 *      #---------#
 *        1  3  5
 *
 * ##################################
 * # Pin | ATMEGA2560 Pin | Arduino #
 * ##################################
 * #  1  | 25 / PB6 (OC1B)|   D12   #
 * #  2  | 24 / PB5 (OC1A)|   D11   #
 * #  3  |       GND      |     -   #
 * #  4  |       GND      |     -   #
 * #  5  |       VCC      |     +   #
 * #  6  |       VCC      |     +   #
 * ##################################
 *
 * NOTE: Pins 1 and 2 are the ones closest to the "L" and "PWR" LEDs.
 *
 * NOTE: Check servo wiring before connecting, for example:
 * + Airtronics (non-Z) use: Red = +; Black = -; Black & White, White or Orange = signal.
 * + Airtronics-Z use: Red = +; Black = -; Blue = signal.
 * + Futaba use: Red = +; Black = -; White = signal.
 * + Hitec use: Red = +; Black = -; Yellow = signal.
 * + JR use: Red = +; Brown = -; Orange = signal.
 *
 * NOTE: Test your servo limits:
 * Due to effects of component tolerances and/or age, the usable range of S-values for individual servos may be less than the settable 0-255 range. For example:
 * 1. One servo may have a fully usable range of M280 P0 S0 through M280 P0 S255.
 * 2. A different servo (of the same brand and model) may have a usable range of only M280 P0 S0 through M280 P0 S165 after which you experience binding.
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *  ===== Thermistor connectors =====
 *
 * (NOTE ORDER) TH2      TH1      TB
 *              1 2      1 2      1 2
 *            #-   -#  #-   -#  #-   -#
 *            | ° ° |  | ° ° |  | ° ° |
 *            #-----#  #-----#  #-----#
 *
 * ############## TB #################
 * # Pin | ATMEGA2560 Pin  | Arduino #
 * ###################################
 * #  1  |       GND       |     -   #
 * #  2  | 83 / PK6 (ADC14)|   A14   #
 * ###################################
 *
 * ############## TH1 ################
 * # Pin | ATMEGA2560 Pin  | Arduino #
 * ###################################
 * #  1  |       GND       |     -   #
 * #  2  | 84 / PK5 (ADC13)|   A13   #
 * ###################################
 *
 * ############## TH2 ################
 * # Pin | ATMEGA2560 Pin  | Arduino #
 * ###################################
 * #  1  |       GND       |     -   #
 * #  2  | 82 / PK7 (ADC15)|   A15   #
 * ###################################
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *  ===== XS3 connector =====
 *
 *        2  4  6
 *      #---------#
 *      | °  °  ° |
 *      | °  °  ° |
 *      #---------#
 *        1  3  5
 *
 * ##################################
 * # Pin | ATMEGA2560 Pin | Arduino #
 * ##################################
 * #  1  |   85 / ADC12   |   A12   #
 * #  2  |   86 / ADC11   |   A11   #
 * #  3  |       GND      |     -   #
 * #  4  |       GND      |     -   #
 * #  5  |       VCC      |     +   #
 * #  6  |       VCC      |     +   #
 * ##################################
 *
 * NOTE: Pins 1 and 2 are the ones closest to the "L" and "PWR" LEDs.
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */
