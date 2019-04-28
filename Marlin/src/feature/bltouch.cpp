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

void BLTouch::reset() {
  _reset();                                          // RESET into normal mode
  #if BLTOUCH_DELAY < BLTOUCH_RESET_DELAY
    safe_delay(BLTOUCH_RESET_DELAY - BLTOUCH_DELAY); // Might need to add some waiting time 
  #endif
}

void BLTouch::clear() {
  reset();     // RESET or RESET_SW will clear an alarm condition but...
               // ...it will not clear a triggered condition in SW mode when the pin is currently up
               // ANTClabs <-- CODE ERROR
  _stow();     // STOW will pull up the pin and clear any triggered condition unless it fails
  _deploy();   // DEPLOY to test the probe. Could fail
  _stow();     // STOW to be ready for meaningful work. Could fail
}

void BLTouch::init() {
  // This is called by marlin.cpp on initialization
  // SET_5V_MODE (if enabled). OD_MODE is the default on power on.
  // This mode will stay active until manual SET_OD_MODE or power cycle
  #if ENABLED(BLTOUCH_FORCE_5V_MODE)         
    _set_5V_mode();                          // Set 5V mode if explicitely demanded (V3 upwards)
    #if BLTOUCH_DELAY < BLTOUCH_SET5V_DELAY
      safe_delay(BLTOUCH_SET5V_DELAY - BLTOUCH_DELAY); // Might need to add some waiting time 
    #endif
  #endif
  clear();
  // There really should be no alarm outstanding now, and no triggered condition. But if there is,
  // there is no need to worry people here on init right at the start of the printer.
}

bool BLTouch::triggered() {
  // Used in the main loop to check the pin state when probing and internally here
  return (
    #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
      READ(Z_MIN_PIN) != Z_MIN_ENDSTOP_INVERTING
    #else
      READ(Z_MIN_PROBE_PIN) != Z_MIN_PROBE_ENDSTOP_INVERTING
    #endif
  );
}

bool BLTouch::deploy_stow_wrapper(const bool deploy) {
  // Do a DEPLOY
  if (deploy) {
    #if ENABLED(BLTOUCH_DEBUG_MSGS)
      SERIAL_ECHOLN("BLTouch DEPLOY requested");
    #endif

    _deploy();                               // Attempt to deploy the probe
    #if BLTOUCH_DELAY < BLTOUCH_DEPLOY_DELAY
      safe_delay(BLTOUCH_DEPLOY_DELAY - BLTOUCH_DELAY); // Might need to add some waiting time 
    #endif

    // The deploy might have failed or the probe is actually triggered (nozzle too low?)
    if (triggered()) {                      
      #if ENABLED(BLTOUCH_DEBUG_MSGS)
        SERIAL_ECHOLN("BLTouch ALARM or TRIGGER after DEPLOY, recovering");
      #endif

      clear();                               // Get the probe into start condition

      _deploy();                             // Last attempt
      #if BLTOUCH_DELAY < BLTOUCH_DEPLOY_DELAY
        safe_delay(BLTOUCH_DEPLOY_DELAY - BLTOUCH_DELAY); // Might need to add some waiting time 
      #endif
      
      // The deploy might have failed or the probe is actually triggered (nozzle too low?) again
      if (triggered()) {                     // but somehow, it didn't work, still triggered
        #if ENABLED(BLTOUCH_DEBUG_MSGS)
          SERIAL_ECHOLN("BLTouch ALARM or TRIGGER after DEPLOY, recovery failed");
        #endif
        SERIAL_ERROR_MSG(MSG_STOP_BLTOUCH);  // Tell the user something is wrong, needs action
        stop();                              // but i'ts not too bad, no need to kill, allow restart
        return true;                         // Tell our caller we goofed in case he cares to know
      }                    
    }

    // The BLTouch V3 (and V2) and some clones can be used in "SWITCH" mode. When triggered, 
    // instead of a 10ms pulse, there is an indefinetly long trigger condition, until 
    // reset by the STOW as soon as this "very very long" pulse is registered. Might help in cases
    // where the trigger pulse is difficult to register (noise, caps on the signal)
    // Entering this mode manually also enables M119 display of the probe pin state.
    #if ENABLED(BLTOUCH_FORCE_SW_MODE) 
      _set_SW_mode();                        // If explicitely demanded, RESET into SW mode
    #endif

    // Now the probe is ready to issue a 10ms pulse (or longer, if in SW mode) when the pin goes up.
    // The trigger STOW (see motion.cpp for example) will pull up the probes pin as soon as the pulse
    // is registered.
  }

  // Do a STOW
  else {
    #if ENABLED(BLTOUCH_DEBUG_MSGS)
      SERIAL_ECHOLN("BLTouch STOW requested");
    #endif

    // A STOW will clear a triggered condition in the probe (10ms pulse or the "very very long" pulse
    // in SW mode).
    // At the moment that we come in here, we might (pulse) or will (SW mode) see the trigger on the pin.
    // So even though we know a STOW will be ignored if an ALARM condition is active, we will STOW.
    // Note: If the probe is deployed AND in an ALARM condition, this STOW will not pull up the pin 
    // and the ALARM condition will still be there. --> ANTClabs should change this behaviour maybe

    _stow();
    #if BLTOUCH_DELAY < BLTOUCH_STOW_DELAY
      safe_delay(BLTOUCH_STOW_DELAY - BLTOUCH_DELAY); // Might need to add some waiting time 
    #endif

    if (triggered()) {                       // "triggered" means "ALARM" in this szenario - STOW failed
      #if ENABLED(BLTOUCH_DEBUG_MSGS)
        SERIAL_ECHOLN("BLTouch ALARM or TRIGGER after STOW, recovering");
      #endif

      reset();                               // This RESET will then also pull up the pin. If it doesn't
                                             // work and the pin is still down, there will no longer be
                                             // an ALARM condition though.

      _stow();                               // But one more STOW will catch that
      #if BLTOUCH_DELAY < BLTOUCH_STOW_DELAY
        safe_delay(BLTOUCH_STOW_DELAY - BLTOUCH_DELAY); // Might need to add some waiting time 
       #endif
                                             // so if there is now STILL an ALARM condition:
      if (triggered()) {
        #if ENABLED(BLTOUCH_DEBUG_MSGS)
          SERIAL_ECHOLN("BLTouch ALARM or TRIGGER after STOW, recovery failed");
        #endif
        SERIAL_ERROR_MSG(MSG_STOP_BLTOUCH);  // Tell the user something is wrong, needs action
        stop();                              // but it's not too bad, no need to kill, allow restart
        return true;                         // Tell our caller we goofed in case he cares to know
      }
    }
  }

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("bltouch.set_deployed(", in_deploy, ")");

  return false; // report success to caller
}

#endif // BLTOUCH
