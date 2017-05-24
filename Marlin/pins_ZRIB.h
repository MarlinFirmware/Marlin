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
 * ZRIB V2.0/3.0/3.1 pin assignments
 * 
 * The default SERVO0_PIN is not wired on the ZRIB,
 * hence the re-define to another free pin.
 */

#include "pins_MKS_13.h"

#define ZRIB_D6_PIN 6
#define ZRIB_D9_PIN 9
#define ZRIB_D50_PIN 50
#undef RAMPS_D9_PIN
#define RAMPS_D9_PIN ZRIB_D9_PIN
#undef ORIG_E0_AUTO_FAN_PIN
#undef ORIG_E1_AUTO_FAN_PIN
#undef ORIG_E2_AUTO_FAN_PIN
#undef ORIG_E3_AUTO_FAN_PIN
#define ORIG_E0_AUTO_FAN_PIN ZRIB_D6_PIN
#define ORIG_E1_AUTO_FAN_PIN ZRIB_D6_PIN
#define ORIG_E2_AUTO_FAN_PIN ZRIB_D6_PIN
#define ORIG_E3_AUTO_FAN_PIN ZRIB_D6_PIN
#undef Z_MIN_PROBE_PIN
#define Z_MIN_PROBE_PIN Z_MAX_PIN
#undef SERVO0_PIN
#define SERVO0_PIN ZRIB_D50_PIN

