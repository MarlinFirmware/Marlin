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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(PINS_DEBUGGING)

#include "../gcode.h"
#include "../../MarlinCore.h" // for pin_is_protected
#include "../../pins/pinsDebug.h"
#include "../../module/endstops.h"

#if HAS_Z_SERVO_PROBE
  #include "../../module/probe.h"
  #include "../../module/servo.h"
#endif

#if ENABLED(BLTOUCH)
  #include "../../feature/bltouch.h"
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "../../feature/host_actions.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extui/ui_api.h"
#endif

#ifndef GET_PIN_MAP_PIN_M43
  #define GET_PIN_MAP_PIN_M43(Q) GET_PIN_MAP_PIN(Q)
#endif

inline void toggle_pins() {
  const bool ignore_protection = parser.boolval('I');
  const int repeat = parser.intval('R', 1),
            start = PARSED_PIN_INDEX('S', 0),
            end = PARSED_PIN_INDEX('L', NUM_DIGITAL_PINS - 1),
            wait = parser.intval('W', 500);

  LOOP_S_LE_N(i, start, end) {
    pin_t pin = GET_PIN_MAP_PIN_M43(i);
    if (!VALID_PIN(pin)) continue;
    if (M43_NEVER_TOUCH(i) || (!ignore_protection && pin_is_protected(pin))) {
      report_pin_state_extended(pin, ignore_protection, true, PSTR("Untouched "));
      SERIAL_EOL();
    }
    else {
      watchdog_refresh();
      report_pin_state_extended(pin, ignore_protection, true, PSTR("Pulsing   "));
      const bool prior_mode = GET_PINMODE(pin);
      #if AVR_AT90USB1286_FAMILY // Teensy IDEs don't know about these pins so must use FASTIO
        if (pin == TEENSY_E2) {
          SET_OUTPUT(TEENSY_E2);
          for (int16_t j = 0; j < repeat; j++) {
            WRITE(TEENSY_E2, LOW);  safe_delay(wait);
            WRITE(TEENSY_E2, HIGH); safe_delay(wait);
            WRITE(TEENSY_E2, LOW);  safe_delay(wait);
          }
        }
        else if (pin == TEENSY_E3) {
          SET_OUTPUT(TEENSY_E3);
          for (int16_t j = 0; j < repeat; j++) {
            WRITE(TEENSY_E3, LOW);  safe_delay(wait);
            WRITE(TEENSY_E3, HIGH); safe_delay(wait);
            WRITE(TEENSY_E3, LOW);  safe_delay(wait);
          }
        }
        else
      #endif
      {
        pinMode(pin, OUTPUT);
        for (int16_t j = 0; j < repeat; j++) {
          watchdog_refresh(); extDigitalWrite(pin, 0); safe_delay(wait);
          watchdog_refresh(); extDigitalWrite(pin, 1); safe_delay(wait);
          watchdog_refresh(); extDigitalWrite(pin, 0); safe_delay(wait);
          watchdog_refresh();
        }
      }
      pinMode(pin, prior_mode);
    }
    SERIAL_EOL();
  }
  SERIAL_ECHOLNPGM("Done.");

} // toggle_pins

inline void servo_probe_test() {

  #if !(NUM_SERVOS > 0 && HAS_SERVO_0)

    SERIAL_ERROR_MSG("SERVO not set up.");

  #elif !HAS_Z_SERVO_PROBE

    SERIAL_ERROR_MSG("Z_PROBE_SERVO_NR not set up.");

  #else // HAS_Z_SERVO_PROBE

    const uint8_t probe_index = parser.byteval('P', Z_PROBE_SERVO_NR);

    SERIAL_ECHOLNPAIR("Servo probe test\n"
                      ". using index:  ", int(probe_index),
                      ", deploy angle: ", servo_angles[probe_index][0],
                      ", stow angle:   ", servo_angles[probe_index][1]
    );

    bool deploy_state = false, stow_state;

    #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)

      #define PROBE_TEST_PIN Z_MIN_PIN
      constexpr bool probe_inverting = Z_MIN_ENDSTOP_INVERTING;

      SERIAL_ECHOLNPAIR(". Probe Z_MIN_PIN: ", int(PROBE_TEST_PIN));
      SERIAL_ECHOPGM(". Z_MIN_ENDSTOP_INVERTING: ");

    #else

      #define PROBE_TEST_PIN Z_MIN_PROBE_PIN
      constexpr bool probe_inverting = Z_MIN_PROBE_ENDSTOP_INVERTING;

      SERIAL_ECHOLNPAIR(". Probe Z_MIN_PROBE_PIN: ", int(PROBE_TEST_PIN));
      SERIAL_ECHOPGM(   ". Z_MIN_PROBE_ENDSTOP_INVERTING: ");

    #endif

    serialprint_truefalse(probe_inverting);
    SERIAL_EOL();

    SET_INPUT_PULLUP(PROBE_TEST_PIN);

    // First, check for a probe that recognizes an advanced BLTouch sequence.
    // In addition to STOW and DEPLOY, it uses SW MODE (and RESET in the beginning)
    // to see if this is one of the following: BLTOUCH Classic 1.2, 1.3,  or
    // BLTouch Smart 1.0, 2.0, 2.2, 3.0, 3.1. But only if the user has actually
    // configured a BLTouch as being present. If the user has not configured this,
    // the BLTouch will be detected in the last phase of these tests (see further on).
    bool blt = false;
    // This code will try to detect a BLTouch probe or clone
    #if ENABLED(BLTOUCH)
      SERIAL_ECHOLNPGM(". Check for BLTOUCH");
      bltouch._reset();
      bltouch._stow();
      if (probe_inverting == READ(PROBE_TEST_PIN)) {
        bltouch._set_SW_mode();
        if (probe_inverting != READ(PROBE_TEST_PIN)) {
          bltouch._deploy();
          if (probe_inverting == READ(PROBE_TEST_PIN)) {
            bltouch._stow();
            SERIAL_ECHOLNPGM("= BLTouch Classic 1.2, 1.3, Smart 1.0, 2.0, 2.2, 3.0, 3.1 detected.");
            // Check for a 3.1 by letting the user trigger it, later
            blt = true;
        }
      }
    }
    #endif

    // The following code is common to all kinds of servo probes.
    // Since it could be a real servo or a BLTouch (any kind) or a clone,
    // use only "common" functions - i.e. SERVO_MOVE. No bltouch.xxxx stuff.

    // If it is already recognised as a being a BLTouch, no need for this test
    if (!blt) {
      // DEPLOY and STOW 4 times and see if the signal follows
      // Then it is a mechanical switch
      uint8_t i = 0;
      SERIAL_ECHOLNPGM(". Deploy & stow 4 times");
      do {
        MOVE_SERVO(probe_index, servo_angles[Z_PROBE_SERVO_NR][0]); // Deploy
        safe_delay(500);
        deploy_state = READ(PROBE_TEST_PIN);
        MOVE_SERVO(probe_index, servo_angles[Z_PROBE_SERVO_NR][1]); // Stow
        safe_delay(500);
        stow_state = READ(PROBE_TEST_PIN);
      } while (++i < 4);

      if (probe_inverting != deploy_state) SERIAL_ECHOLNPGM("WARNING: INVERTING setting probably backwards.");

      if (deploy_state != stow_state) {
        SERIAL_ECHOLNPGM("= Mechanical Switch detected");
        if (deploy_state) {
          SERIAL_ECHOLNPAIR("  DEPLOYED state: HIGH (logic 1)",
                            "  STOWED (triggered) state: LOW (logic 0)");
        }
        else {
          SERIAL_ECHOLNPAIR("  DEPLOYED state: LOW (logic 0)",
                            "  STOWED (triggered) state: HIGH (logic 1)");
        }
        #if ENABLED(BLTOUCH)
          SERIAL_ECHOLNPGM("FAIL: BLTOUCH enabled - Set up this device as a Servo Probe with INVERTING set to 'true'.");
        #endif
        return;
      }
    }

    // Ask the user for a trigger event and measure the pulse width.
    MOVE_SERVO(probe_index, servo_angles[Z_PROBE_SERVO_NR][0]); // Deploy
    safe_delay(500);
    SERIAL_ECHOLNPGM("** Please trigger probe within 30 sec **");
    uint16_t probe_counter = 0;

    // Wait 30 seconds for user to trigger probe
    for (uint16_t j = 0; j < 500 * 30 && probe_counter == 0 ; j++) {
      safe_delay(2);

      if (0 == j % (500 * 1)) gcode.reset_stepper_timeout();    // Keep steppers powered

      if (deploy_state != READ(PROBE_TEST_PIN)) {               // probe triggered
        for (probe_counter = 0; probe_counter < 15 && deploy_state != READ(PROBE_TEST_PIN); ++probe_counter) safe_delay(2);

        SERIAL_ECHOPGM(". Pulse width");
        if (probe_counter == 15)
          SERIAL_ECHOLNPGM(": 30ms or more");
        else
          SERIAL_ECHOLNPAIR(" (+/- 4ms): ", probe_counter * 2);

        if (probe_counter >= 4) {
          if (probe_counter == 15) {
            if (blt) SERIAL_ECHOPGM("= BLTouch V3.1");
            else     SERIAL_ECHOPGM("= Z Servo Probe");
          }
          else SERIAL_ECHOPGM("= BLTouch pre V3.1 (or compatible)");
          SERIAL_ECHOLNPGM(" detected.");
        }
        else SERIAL_ECHOLNPGM("FAIL: Noise detected - please re-run test");

        MOVE_SERVO(probe_index, servo_angles[Z_PROBE_SERVO_NR][1]); // Stow
        return;
      }
    }

    if (!probe_counter) SERIAL_ECHOLNPGM("FAIL: No trigger detected");

  #endif // HAS_Z_SERVO_PROBE

} // servo_probe_test

/**
 * M43: Pin debug - report pin state, watch pins, toggle pins and servo probe test/report
 *
 *  M43         - report name and state of pin(s)
 *                  P<pin>  Pin to read or watch. If omitted, reads all pins.
 *                  I       Flag to ignore Marlin's pin protection.
 *
 *  M43 W       - Watch pins -reporting changes- until reset, click, or M108.
 *                  P<pin>  Pin to read or watch. If omitted, read/watch all pins.
 *                  I       Flag to ignore Marlin's pin protection.
 *
 *  M43 E<bool> - Enable / disable background endstop monitoring
 *                  - Machine continues to operate
 *                  - Reports changes to endstops
 *                  - Toggles LED_PIN when an endstop changes
 *                  - Cannot reliably catch the 5mS pulse from BLTouch type probes
 *
 *  M43 T       - Toggle pin(s) and report which pin is being toggled
 *                  S<pin>  - Start Pin number.   If not given, will default to 0
 *                  L<pin>  - End Pin number.   If not given, will default to last pin defined for this board
 *                  I<bool> - Flag to ignore Marlin's pin protection.   Use with caution!!!!
 *                  R       - Repeat pulses on each pin this number of times before continueing to next pin
 *                  W       - Wait time (in miliseconds) between pulses.  If not given will default to 500
 *
 *  M43 S       - Servo probe test
 *                  P<index> - Probe index (optional - defaults to 0
 */
void GcodeSuite::M43() {

  // 'T' must be first. It uses 'S' and 'E' differently.
  if (parser.seen('T')) return toggle_pins();

  // 'E' Enable or disable endstop monitoring and return
  if (parser.seen('E')) {
    endstops.monitor_flag = parser.value_bool();
    SERIAL_ECHOPGM("endstop monitor ");
    serialprintPGM(endstops.monitor_flag ? PSTR("en") : PSTR("dis"));
    SERIAL_ECHOLNPGM("abled");
    return;
  }

  // 'S' Run servo probe test and return
  if (parser.seen('S')) return servo_probe_test();

  // 'P' Get the range of pins to test or watch
  uint8_t first_pin = PARSED_PIN_INDEX('P', 0),
          last_pin = parser.seenval('P') ? first_pin : NUMBER_PINS_TOTAL - 1;

  if (first_pin > last_pin) return;

  // 'I' to ignore protected pins
  const bool ignore_protection = parser.boolval('I');

  // 'W' Watch until click, M108, or reset
  if (parser.boolval('W')) {
    SERIAL_ECHOLNPGM("Watching pins");
    #ifdef ARDUINO_ARCH_SAM
      NOLESS(first_pin, 2); // Don't hijack the UART pins
    #endif
    uint8_t pin_state[last_pin - first_pin + 1];
    LOOP_S_LE_N(i, first_pin, last_pin) {
      pin_t pin = GET_PIN_MAP_PIN_M43(i);
      if (!VALID_PIN(pin)) continue;
      if (M43_NEVER_TOUCH(i) || (!ignore_protection && pin_is_protected(pin))) continue;
      pinMode(pin, INPUT_PULLUP);
      delay(1);
        /*
        if (IS_ANALOG(pin))
          pin_state[pin - first_pin] = analogRead(DIGITAL_PIN_TO_ANALOG_PIN(pin)); // int16_t pin_state[...]
        else
        //*/
          pin_state[i - first_pin] = extDigitalRead(pin);
    }

    #if HAS_RESUME_CONTINUE
      KEEPALIVE_STATE(PAUSED_FOR_USER);
      wait_for_user = true;
      #if ENABLED(HOST_PROMPT_SUPPORT)
        host_prompt_do(PROMPT_USER_CONTINUE, PSTR("M43 Wait Called"), CONTINUE_STR);
      #endif
      #if ENABLED(EXTENSIBLE_UI)
        ExtUI::onUserConfirmRequired_P(PSTR("M43 Wait Called"));
      #endif
    #endif

    for (;;) {
      LOOP_S_LE_N(i, first_pin, last_pin) {
        pin_t pin = GET_PIN_MAP_PIN_M43(i);
        if (!VALID_PIN(pin)) continue;
        if (M43_NEVER_TOUCH(i) || (!ignore_protection && pin_is_protected(pin))) continue;
        const byte val =
          /*
          IS_ANALOG(pin)
            ? analogRead(DIGITAL_PIN_TO_ANALOG_PIN(pin)) : // int16_t val
            :
          //*/
            extDigitalRead(pin);
        if (val != pin_state[i - first_pin]) {
          report_pin_state_extended(pin, ignore_protection, false);
          pin_state[i - first_pin] = val;
        }
      }

      #if HAS_RESUME_CONTINUE
        if (!wait_for_user) break;
      #endif

      safe_delay(200);
    }
  }
  else {
    // Report current state of selected pin(s)
    LOOP_S_LE_N(i, first_pin, last_pin) {
      pin_t pin = GET_PIN_MAP_PIN_M43(i);
      if (VALID_PIN(pin)) report_pin_state_extended(pin, ignore_protection, true);
    }
  }
}

#endif // PINS_DEBUGGING
