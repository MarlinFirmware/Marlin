/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// MKS E3D V2 (ZNP Robin Nano_DW V2.2) as found in the Elegoo Neptune 3 Pro / Plus / Max
//

#define ALLOW_STM32F4
#include "env_validate.h"

#if HOTENDS > 1 || E_STEPPERS > 1
  #error "MKS E3D V2 only supports 1 hotend / E stepper."
#endif

#define BOARD_INFO_NAME "ZNP Robin Nano_DW V2.2"

#define BOARD_LCD_SERIAL_PORT 6

//
// Limit Switches
//
#define X_DIAG_PIN                          PA13  // X-

//
// Z Probe
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PA8   // PROBE
#endif

//
// Fans
//
#define FAN0_PIN                            PA7   // FAN1
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                   PB0   // FAN2
#endif

//
// Misc. Functions
//
#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                    PB9   // FAN3 / Top light
#endif

#include "pins_MKS_NEPTUNE_3_common.h"
