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
 * MKS BASE 1.0 – Arduino Mega2560 with RAMPS v1.4 pin assignments
 *
 * Rev B - Override pin definitions for CASE_LIGHT and M3/M4/M5 spindle control
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "FRANCK BASE 1.0 supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "FRANCK BASE 1.0" // Based on MKS BASE 1.0
#define MKS_BASE_VERSION 10

#include "pins_MKS_BASE_common.h" // ... RAMPS

/**
 * Franck 
 * Si on défini LCD_BACKLIGHT_TIMEOUT X dans Configuration.h
 * On stoppe l'utilisation du BEEPER car la pin retroeclairage du LCD
 * est branché sur la pin du beeper.
 */
#if LCD_BACKLIGHT_TIMEOUT_MS > 0
#define USE_LCD_SCREENSAVER
#undef BEEPER_PIN
#define LCD_BACKLIGHT_PIN EXP1_10_PIN
#endif
