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

bool BLTouch::last_written_mode; // Initialized by settings.load, 0 = Open Drain; 1 = 5V Drain

#include "../module/servo.h"

void stop();

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../core/debug_out.h"

bool BLTouch::command(const BLTCommand cmd, const millis_t &ms) {
  if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR("BLTouch Command :", cmd);
  MOVE_SERVO(Z_PROBE_SERVO_NR, cmd);
  safe_delay(MAX(ms, (uint32_t)BLTOUCH_DELAY)); // BLTOUCH_DELAY is also the *minimum* delay
  return triggered();
}

// Init the class and device. Call from setup().
void BLTouch::init(const bool set_voltage/*=false*/) {
  // Voltage Setting (if enabled). At every Marlin initialization:
  // BLTOUCH < V3.0 and clones: This will be ignored by the probe
  // BLTOUCH V3.0: SET_5V_MODE or SET_OD_MODE (if enabled).
  //               OD_MODE is the default on power on, but setting it does not hurt
  //               This mode will stay active until manual SET_OD_MODE or power cycle
  // BLTOUCH V3.1: SET_5V_MODE or SET_OD_MODE (if enabled).
  //               At power on, the probe will default to the eeprom settings configured by the user
  #if ENABLED(BLTOUCH_FORCE_MODE_SET)

    constexpr bool should_set = true;

  #else

    if (DEBUGGING(LEVELING)) {
      DEBUG_ECHOLNPAIR("last_written_mode - ", (int)last_written_mode);
      DEBUG_ECHOLNPGM("config mode - "
        #if ENABLED(BLTOUCH_SET_5V_MODE)
          "BLTOUCH_SET_5V_MODE"
        #else
          "OD"
        #endif
      );
    }

    const bool should_set = last_written_mode != (false
      #if ENABLED(BLTOUCH_SET_5V_MODE)
        || true
      #endif
    );

  #endif

  if (should_set && set_voltage)
    mode_conv_proc((false
      #if ENABLED(BLTOUCH_SET_5V_MODE)
        || true
      #endif
    ));

  _reset();
  _stow();
}

void BLTouch::clear() {
  _reset();    // RESET or RESET_SW will clear an alarm condition but...
               // ...it will not clear a triggered condition in SW mode when the pin is currently up
               // ANTClabs <-- CODE ERROR
  _stow();     // STOW will pull up the pin and clear any triggered condition unless it fails, don't care
  _deploy();   // DEPLOY to test the probe. Could fail, don't care
  _stow();     // STOW to be ready for meaningful work. Could fail, don't care
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

bool BLTouch::deploy_proc() {
  // Do a DEPLOY
  if (DEBUGGING(LEVELING)) DEBUG_ECHOLN("BLTouch DEPLOY requested");

  // Attempt to DEPLOY, wait for DEPLOY_DELAY or ALARM
  if (_deploy_query_alarm()) {
    // The deploy might have failed or the probe is already triggered (nozzle too low?)
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLN("BLTouch ALARM or TRIGGER after DEPLOY, recovering");

    clear();                               // Get the probe into start condition

    // Last attempt to DEPLOY
    if (_deploy_query_alarm()) {
      // The deploy might have failed or the probe is actually triggered (nozzle too low?) again
      if (DEBUGGING(LEVELING)) DEBUG_ECHOLN("BLTouch Recovery Failed");

      SERIAL_ERROR_MSG(MSG_STOP_BLTOUCH);  // Tell the user something is wrong, needs action
      stop();                              // but it's not too bad, no need to kill, allow restart

      return true;                         // Tell our caller we goofed in case he cares to know
    }
  }

  // One of the recommended ANTClabs ways to probe, using SW MODE
  #if ENABLED(BLTOUCH_FORCE_SW_MODE)
   _set_SW_mode();
  #endif

  // Now the probe is ready to issue a 10ms pulse when the pin goes up.
  // The trigger STOW (see motion.cpp for example) will pull up the probes pin as soon as the pulse
  // is registered.

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLN("bltouch.deploy_proc() end");

  return false; // report success to caller
}

bool BLTouch::stow_proc() {
  // Do a STOW
  if (DEBUGGING(LEVELING)) DEBUG_ECHOLN("BLTouch STOW requested");

  // A STOW will clear a triggered condition in the probe (10ms pulse).
  // At the moment that we come in here, we might (pulse) or will (SW mode) see the trigger on the pin.
  // So even though we know a STOW will be ignored if an ALARM condition is active, we will STOW.
  // Note: If the probe is deployed AND in an ALARM condition, this STOW will not pull up the pin
  // and the ALARM condition will still be there. --> ANTClabs should change this behavior maybe

  // Attempt to STOW, wait for STOW_DELAY or ALARM
  if (_stow_query_alarm()) {
    // The stow might have failed
    if (DEBUGGING(LEVELING)) DEBUG_ECHOLN("BLTouch ALARM or TRIGGER after STOW, recovering");

    _reset();                              // This RESET will then also pull up the pin. If it doesn't
                                           // work and the pin is still down, there will no longer be
                                           // an ALARM condition though.
                                           // But one more STOW will catch that
    // Last attempt to STOW
    if (_stow_query_alarm()) {             // so if there is now STILL an ALARM condition:

      if (DEBUGGING(LEVELING)) DEBUG_ECHOLN("BLTouch Recovery Failed");

      SERIAL_ERROR_MSG(MSG_STOP_BLTOUCH);  // Tell the user something is wrong, needs action
      stop();                              // but it's not too bad, no need to kill, allow restart

      return true;                         // Tell our caller we goofed in case he cares to know
    }
  }

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLN("bltouch.stow_proc() end");

  return false; // report success to caller
}

bool BLTouch::status_proc() {
  /**
   * Return a TRUE for "YES, it is DEPLOYED"
   * This function will ensure switch state is reset after execution
   */

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLN("BLTouch STATUS requested");

  _set_SW_mode();              // Incidentally, _set_SW_mode() will also RESET any active alarm
  const bool tr = triggered(); // If triggered in SW mode, the pin is up, it is STOWED

  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("BLTouch is ", (int)tr);

  if (tr) _stow(); else _deploy();  // Turn off SW mode, reset any trigger, honor pin state
  return !tr;
}

void BLTouch::mode_conv_proc(const bool M5V) {
  /**
   * BLTOUCH pre V3.0 and clones: No reaction at all to this sequence apart from a DEPLOY -> STOW
   * BLTOUCH V3.0: This will set the mode (twice) and sadly, a STOW is needed at the end, because of the deploy
   * BLTOUCH V3.1: This will set the mode and store it in the eeprom. The STOW is not needed but does not hurt
   */
  if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("BLTouch Set Mode - ", (int)M5V);
  _deploy();
  if (M5V) _set_5V_mode(); else _set_OD_mode();
  _mode_store();
  if (M5V) _set_5V_mode(); else _set_OD_mode();
  _stow();
  last_written_mode = M5V;
}

#endif // BLTOUCH
