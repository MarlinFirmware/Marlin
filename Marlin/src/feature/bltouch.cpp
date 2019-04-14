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
  #if ENABLED(BLTOUCH_DEBUG_MSGS)
    SERIAL_ECHOLNPAIR("BLTouch Command :", cmd);
  #endif
  MOVE_SERVO(Z_PROBE_SERVO_NR, cmd);
  safe_delay(BLTOUCH_DELAY);
}

void BLTouch::init() {
  reset();                            // Clear all BLTouch error conditions
  _stow();
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
  // Do a DEPLOY
  if (in_deploy) {

    // The BLTouch probe may be in ALARM condition (blinking RED) or currently triggered 
    // (either a 10ms pulse or "very very long" pulse in SW mode)
    // RESET to clear the ALARM (An unneeded RESET won't hurt and a STOW will 
    // clear the triggered condition
    if (triggered()) {                      
      #if ENABLED(BLTOUCH_DEBUG_MSGS)
        SERIAL_ECHOLN("BLTouch ALARM or TRIGGER, will RESET, STOW");
      #endif
      reset();                              // Reset, but now we do not know, is it deployed or stowed?
      _stow();                              // Force a stow
      safe_delay(1000);                     // Give it some time to be safe
      if (triggered()) {                    // If the ALARM condition persists. This is really bad.
        SERIAL_ERROR_MSG(MSG_STOP_BLTOUCH);
        stop();                             // punt!
      return true;
      }
    }

    #if ENABLED(BLTOUCH_V3)
      #if ENABLED(BLTOUCH_FORCE_OPEN_DRAIN_MODE)  // Set OPEN DRAIN mode if explicitely demanded
        set_OD_mode();
      #elif ENABLED(BLTOUCH_FORCE_5V_MODE)        // Set 5V mode if explicitely demanded
        set_5V_mode();
      #else
        set_5V_mode();                            // If no explicit setting requested, choose 5V mode 
                                                  // to be compatible with pre-V3 probes
      #endif
    #endif

    _deploy();

    // BLTouch pre V3 and clones will "drop" down again after being triggered and having
    // issued a 10ms pulse. The trigger STOW (see motion.cpp for example) will pull up 
    // the probe as soon as the pulse is registered.

    // The BLTouch V3 (and V2) can be set to "SWITCH" mode. When triggered, instead of a 10ms
    // pulse, there is an indefinetly long trigger condition, until reset by the STOW as
    // soon as this "very very long" pulse is registered.Also, the probe stays pulled up. 
    // It doesn't "drop" down again after being triggered. The STOW is still needed to reset
    // the triggered condition.
    #if ENABLED(BLTOUCH_V3) \
        || ENABLED(BLTOUCH_FORCE_SW_MODE)         // V3 should always use SW mode
                                                  // or, if explicitely requested (maybe a V2 probe?)
      set_SW_mode();
    #endif
  }

  // Do a STOW
  else 
    // The STOW will clear a triggered condition in the probe (10ms pulse or "very
    // very long" pulse in SW mode)
      _stow();

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("bltouch.set_deployed(", in_deploy, ")");

  return false;
}

#endif // BLTOUCH
