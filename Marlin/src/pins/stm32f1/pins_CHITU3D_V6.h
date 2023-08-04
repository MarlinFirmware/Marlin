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

#define BOARD_INFO_NAME "Chitu3D V6"

#define Z2_ENABLE_PIN                       PF3
#define Z2_STEP_PIN                         PF5
#define Z2_DIR_PIN                          PF1

#define Z_STOP_PIN                          PA14

#ifndef FIL_RUNOUT2_PIN
  #define FIL_RUNOUT2_PIN                   PF13
#endif

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PG9
#endif

#include "pins_CHITU3D_common.h"

/**
 * Circuit diagram https://github.com/MarlinFirmware/Marlin/files/3401484/x5sa-main_board-2.pdf
 *
 * Details on the 30 pin ribbon pins. From: https://3dtoday.ru/blogs/artem-sr/tronxy-x5sa-pro-ustanovka-bfp-touch-na-board-chitu3d-v6-cxy-v6-191017
 *
 * JP2 Ribbon 30 on CXY-V6-191017
 * ----------------------------------------------
 * | 2  4  6  8 10 12 14 16 18 20 22 24 26 28 30|
 * | 1  3  5  7  9 11 13 15 17 19 21 23 25 27 29|
 * ----------------------  ----------------------
 *
 *  --------------------------------------------------------------------------------------
 *  | Pin | Label     | Function & Notes                                                 |
 *  --------------------------------------------------------------------------------------
 *  |   1 | Hotend    | Hotend driver Q6 HY1403 via MCU Pin127 PG12                      |
 *  |   2 | Hotend    | Hotend driver Q6 HY1403 via MCU Pin127 PG12                      |
 *  |   3 | Hotend    | Hotend driver Q6 HY1403 via MCU Pin127 PG12                      |
 *  |   4 | Hotend    | Hotend driver Q6 HY1403 via MCU Pin127 PG12                      |
 *  |   5 | +24v      | Hotend +24v                                                      |
 *  |   6 | +24v      | Hotend +24v                                                      |
 *  |   7 | +24v      | Hotend +24v                                                      |
 *  |   8 | +24v      | Hotend +24v                                                      |
 *  |   9 | F_2       | Extruder Fan2 driver Q8 AO3400A X07S via MCU Pin129 PG14         |
 *  |  10 | +24v      | Extruder cooling Fan2 +24v                                       |
 *  |  11 | F_1       | Part Fan1 driver Q7 AO3400A X07S via MCU Pin128 PG13             |
 *  |  12 | +24v      | Part cooling Fanl +24v                                           |
 *  |  13 | 1B        | X-MOTOR Winding Drive                                            |
 *  |  14 | 1A        | X-MOTOR Winding Drive                                            |
 *  |  15 | 2B        | X-MOTOR Winding Drive                                            |
 *  |  16 | 2A        | X-MOTOR Winding Drive                                            |
 *  |  17 | lA        | El-Motor Winding Drive                                           |
 *  |  18 | 1B        | El-Motor Winding Drive                                           |
 *  |  19 | 2B        | El-Motor Winding Drive                                           |
 *  |  20 | 2A        | El-Motor Winding Drive                                           |
 *  |  21 | PROXIMITY | 10kΩ Pullup to +5V and 100nF to GND, then 20kΩ to MCU Pin124 PG9 |
 *  |  22 | +24v      | Proximity sensor +24v                                            |
 *  |  23 | +5V       | Filament sensor +5V XSTOP sensor +5V                             |
 *  |  24 | GND       | Proximity sensor GND                                             |
 *  |  25 | FILAMENT1 | 10kΩ Pullup to +5V and 100nF to GND, then 47kΩ to MCU Pin110 PA15|
 *  |  26 | GND       | Filament Sensor GND                                              |
 *  |  27 | XSTOP     | 10kΩ Pullup to +5V and 100nF to GND, then 47kΩ to MCU Pin125 PG10|
 *  |  28 | GND       | XSTOP sensor GND                                                 |
 *  |  29 | GND       | Extruder temperature NTC sensor return GND                       |
 *  |  30 | ETEMP     | 4k7Ω Pullup to +3V3 and 100nF to GND, then 4k7Ω to MCU Pin35 PA1 |
 *  --------------------------------------------------------------------------------------
 */
