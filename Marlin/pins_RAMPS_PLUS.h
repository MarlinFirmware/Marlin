/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * 3DYMY Arduino Mega with RAMPS v1.4 pin assignments
 * https://github.com/MarlinFirmware/Marlin/issues/5332
 *
 * http://www.yoycart.com/Product/521797326074/
 * http://www.iographix.com/products/3d-printer-main-control-board-automatic-leveling-double-shaft-reprap-ramps1-4-plus-bt7272a/
 */

#define BOARD_NAME "RAMPS 1.4 Plus"

#include "pins_RAMPS.h"

//
// Steppers
//
// Plus swaps E0 / E1 plugs vs RAMPS 1.4
#undef E0_STEP_PIN
#undef E0_DIR_PIN
#undef E0_ENABLE_PIN
#define E0_STEP_PIN        36
#define E0_DIR_PIN         34
#define E0_ENABLE_PIN      30

#undef E1_STEP_PIN
#undef E1_DIR_PIN
#undef E1_ENABLE_PIN
#define E1_STEP_PIN        26
#define E1_DIR_PIN         28
#define E1_ENABLE_PIN      24

//
// Heaters / Fans
//
#undef  FAN_PIN
#undef  HEATER_0_PIN
#undef  HEATER_BED_PIN

#define HEATER_0_PIN     RAMPS_D8_PIN

#if ENABLED(IS_RAMPS_EFB)                      // Hotend, Fan, Bed
  #define FAN_PIN        RAMPS_D9_PIN
  #define HEATER_BED_PIN RAMPS_D10_PIN
#elif ENABLED(IS_RAMPS_EEF)                    // Hotend, Hotend, Fan
  #define HEATER_1_PIN   RAMPS_D9_PIN
  #define FAN_PIN        RAMPS_D10_PIN
#elif ENABLED(IS_RAMPS_EEB)                    // Hotend, Hotend, Bed
  #define HEATER_1_PIN   RAMPS_D9_PIN
  #define HEATER_BED_PIN RAMPS_D10_PIN
#elif ENABLED(IS_RAMPS_EFF)                    // Hotend, Fan, Fan
  #define FAN_PIN        RAMPS_D9_PIN
  #define FAN1_PIN       RAMPS_D10_PIN
#elif ENABLED(IS_RAMPS_SF)                     // Spindle, Fan
  #define FAN_PIN        RAMPS_D10_PIN
#else                                          // Non-specific are "EFB" (i.e., "EFBF" or "EFBE")
  #define FAN_PIN        RAMPS_D9_PIN
  #define HEATER_BED_PIN RAMPS_D10_PIN
  #if HOTENDS == 1
    #define FAN1_PIN     MOSFET_D_PIN
  #else
    #define HEATER_1_PIN MOSFET_D_PIN
  #endif
#endif
