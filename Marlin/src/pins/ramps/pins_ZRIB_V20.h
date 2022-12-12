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
 * ZONESTAR ZRIB V2.0 & V3.0 pin assignments
 * V2 and V3 Boards only differ in USB controller, nothing affecting the pins.
 * Schematic (2.0): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/RAMPS/ZONESTAR%20ZRIB%20V2.0/ZRIB_V2_Schematic.pdf
 * Origin (2.0): https://github.com/ZONESTAR3D/Control-Board/blob/main/8bit/ZRIB/ZRIB_V2/ZRIB_V2_Schematic.pdf
 * Schematic (3.0): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/RAMPS/ZONESTAR%20ZRIB%20V3.0/ZRIB_V3_Schematic.pdf
 * Origin (3.0): https://github.com/ZONESTAR3D/Control-Board/blob/main/8bit/ZRIB/ZRIB_V3/ZRIB_V3_Schematic.pdf
 * ATmega2560, ATmega1280
 */

#define AVR_CHIPOSCILLATOR_FREQ 16000000

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                        PinB5  // Analog Input
#endif

//
// Auto fans
//
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                     PinH3  // Fan
#endif
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN                     PinH3
#endif
#ifndef E2_AUTO_FAN_PIN
  #define E2_AUTO_FAN_PIN                     PinH3
#endif
#ifndef E3_AUTO_FAN_PIN
  #define E3_AUTO_FAN_PIN                     PinH3
#endif

#if ENABLED(ZONESTAR_LCD)
  #define LCD_PINS_RS                         PinH1
  #define LCD_PINS_ENABLE                     PinH0
  #define LCD_PINS_D4                         PinA1
  #define LCD_PINS_D5                         PinA3
  #define LCD_PINS_D6                         PinA5
  #define LCD_PINS_D7                         PinA7
  #define ADC_KEYPAD_PIN                      PinB4  // Analog Input
  #define BEEPER_PIN                          PinC0

  #define LCD_PINS_DEFINED
#endif

#include "pins_MKS_GEN_13.h" // ... RAMPS
