/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#define BOARD_INFO_NAME "Melzi Ender 2 (Creality)"

/**
 * Connectors.
 *                                              FAN1     LASER     K-FAN   B-MOT(A6) E-TEMP(A7)  CHECK         AVR ISP
 *                                           --------- --------- --------- --------- --------- ---------      ----------
 *  J11 (Power Switch)                       |12V GND| |12V  D4| |12V  D4| |GND D25| |GND D24| |D29 GND|      |MISO  5V|
 *  ------                                   --------- --------- --------- --------- --------- ---------      |SCK MOSI|
 *  |V-IN|(Regulator)       BED     HOT-END     FAN2      OFF     X-STOP    Y-STOP    Z-STOP                  |RST  GND|
 *  |GND |               --------- --------- --------- --------- --------- --------- ---------                ----------
 *  |12V |               |12V D12| |12V D13| |12V GND| |D27 D17| |GND D18| |GND D19| |GND D20|
 *  ------               --------- --------- --------- --------- --------- --------- ---------
 *
 *                EXP1                  EXP1 as ENDER2 STOCKDISPLAY           EXP1 as CR10 STOCKDISPLAY
 *               ------                            ------                               ------
 * (AVR_SCK) D7 | 1  2 | D16                  SCK | 1  2 | BTN_E            BEEPER_PIN | 1  2 | BTN_ENC
 *          D11 | 3  4 | RESET            BTN_EN1 | 3  4 | RESET               BTN_EN1 | 3  4 | RESET
 *          D10   5  6 | D30              BTN_EN2   5  6 | LCD_A0              BTN_EN2   5  6 | LCD_D4 (ST9720 CLK)
 *          D28 | 7  8 | D5 (AVR_MOSI)     LCD_CS | 7  8 | MOSI     (ST9720 CS) LCD_RS | 7  8 | LCD_EN (ST9720 DAT)
 *          GND | 9 10 | 5V                   GND | 9 10 | 5V                      GND | 9 10 | 5V
 *               ------                            ------                               ------
 */

#define EXP1_01_PIN                            7
#define EXP1_02_PIN                           16
#define EXP1_03_PIN                           11
#define EXP1_04_PIN                           -1  // hardware reset line
#define EXP1_05_PIN                           10
#define EXP1_06_PIN                           30
#define EXP1_07_PIN                           28
#define EXP1_08_PIN                            5

//
// LCD / Controller
//

#if ENABLED(CR10_STOCKDISPLAY)

  // Migrated to pins/lcd
  #if HAS_MEDIA
    #error "Cannot have SDSUPPORT with CR10_STOCKDISPLAY on this motherboard." // Hardware SDCARD SCK and MOSI pins are reallocated.
  #endif

#elif ENABLED(ENDER2_STOCKDISPLAY)

  // Migrated to pins/lcd
  #define ADAPTER_MELZI_CREALITY
  #define LCD_PINS_DEFINED

#endif

#define LCD_SDSS_PIN                          31  // Controller's SD card

#include "pins_MELZI.h" // ... SANGUINOLOLU_12 ... SANGUINOLOLU_11

#if ENABLED(BLTOUCH)
  #ifndef SERVO0_PIN
    #define SERVO0_PIN                        27
  #endif
  #if SERVO0_PIN == BEEPER_PIN
    #undef BEEPER_PIN
  #endif
#elif HAS_FILAMENT_SENSOR
  #ifndef FIL_RUNOUT_PIN
    #define FIL_RUNOUT_PIN                    27
  #endif
  #if FIL_RUNOUT_PIN == BEEPER_PIN
    #undef BEEPER_PIN
  #endif
#endif
