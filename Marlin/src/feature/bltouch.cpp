/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfig.h"

#if ENABLED(BLTOUCH)

#include "bltouch.h"

BLTouch bltouch;

#include "../module/servo.h"

void stop();

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../core/debug_out.h"

void BLTouch::command(const BLTCommand cmd) {
  MOVE_SERVO(Z_PROBE_SERVO_NR, cmd);
  safe_delay(BLTOUCH_DELAY);
}

void BLTouch::init() {
  reset();                            // Clear all BLTouch error conditions
  stow();
}

bool BLTouch::triggered() {
  return (
    #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
      READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING
    #else
      READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING
    #endif
  );
}

bool BLTouch::set_deployed(const bool in_deploy) {
  if (in_deploy && triggered()) {     // If BLTouch says it's triggered
    reset();                          //  try to reset it.
    _deploy(); _stow();               // Deploy and stow to clear the "triggered" condition.
    safe_delay(1500);                 // Wait for internal self-test to complete.
                                      //  (Measured completion time was 0.65 seconds
                                      //   after reset, deploy, and stow sequence)
    if (triggered()) {                // If it still claims to be triggered...
      SERIAL_ERROR_MSG(MSG_STOP_BLTOUCH);
      stop();                         // punt!
      return true;
    }
  }

  #if ENABLED(BLTOUCH_V3)
    #if ENABLED(BLTOUCH_FORCE_5V_MODE)
      set_5V_mode();                  // Assume 5V DC logic level if endstop pullup resistors are enabled
    #else
      set_OD_mode();
    #endif
  #endif

  if (in_deploy) {
    _deploy();
    #if ENABLED(BLTOUCH_V3)
      set_SW_mode();                  // Ensure Switch mode is activated for BLTouch V3. Ignored on V2.
    #endif
  }
  else _stow();

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("bltouch.set_deployed(", in_deploy, ")");

  return false;
}

#endif // BLTOUCH
