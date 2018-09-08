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

#include "../../inc/MarlinConfig.h"

#if ENABLED(PINS_DEBUGGING)

#include "../gcode.h"
#include "../../Marlin.h" // for pin_is_protected
#include "../../pins/pinsDebug.h"
#include "../../module/endstops.h"

#if HAS_Z_SERVO_PROBE
  #include "../../module/probe.h"
  #include "../../module/servo.h"
#endif

inline void toggle_pins() {
  const bool ignore_protection = parser.boolval('I');
  const int repeat = parser.intval('R', 1),
            start = PARSED_PIN_INDEX('S', 0),
            end = PARSED_PIN_INDEX('E', NUM_DIGITAL_PINS - 1),
            wait = parser.intval('W', 500);

  for (uint8_t i = start; i <= end; i++) {
    pin_t pin = GET_PIN_MAP_PIN(i);
    //report_pin_state_extended(pin, ignore_protection, false);
    if (!VALID_PIN(pin)) continue;
    if (!ignore_protection && pin_is_protected(pin)) {
      report_pin_state_extended(pin, ignore_protection, true, "Untouched ");
      SERIAL_EOL();
    }
    else {
      report_pin_state_extended(pin, ignore_protection, true, "Pulsing   ");
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
          digitalWrite(pin, 0); safe_delay(wait);
          digitalWrite(pin, 1); safe_delay(wait);
          digitalWrite(pin, 0); safe_delay(wait);
        }
      }

    }
    SERIAL_EOL();
  }
  SERIAL_ECHOLNPGM("Done.");

} // toggle_pins

inline void servo_probe_test() {
  #if !(NUM_SERVOS > 0 && HAS_SERVO_0)

    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM("SERVO not setup");

  #elif !HAS_Z_SERVO_PROBE

    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM("Z_PROBE_SERVO_NR not setup");

  #else // HAS_Z_SERVO_PROBE

    const uint8_t probe_index = parser.byteval('P', Z_PROBE_SERVO_NR);

    SERIAL_PROTOCOLLNPGM("Servo probe test");
    SERIAL_PROTOCOLLNPAIR(".  using index:  ", probe_index);
    SERIAL_PROTOCOLLNPAIR(".  deploy angle: ", servo_angles[probe_index][0]);
    SERIAL_PROTOCOLLNPAIR(".  stow angle:   ", servo_angles[probe_index][1]);

    bool probe_inverting;

    #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)

      #define PROBE_TEST_PIN Z_MIN_PIN

      SERIAL_PROTOCOLLNPAIR(". probe uses Z_MIN pin: ", PROBE_TEST_PIN);
      SERIAL_PROTOCOLLNPGM(". uses Z_MIN_ENDSTOP_INVERTING (ignores Z_MIN_PROBE_ENDSTOP_INVERTING)");
      SERIAL_PROTOCOLPGM(". Z_MIN_ENDSTOP_INVERTING: ");

      #if Z_MIN_ENDSTOP_INVERTING
        SERIAL_PROTOCOLLNPGM("true");
      #else
        SERIAL_PROTOCOLLNPGM("false");
      #endif

      probe_inverting = Z_MIN_ENDSTOP_INVERTING;

    #elif ENABLED(Z_MIN_PROBE_ENDSTOP)

      #define PROBE_TEST_PIN Z_MIN_PROBE_PIN
      SERIAL_PROTOCOLLNPAIR(". probe uses Z_MIN_PROBE_PIN: ", PROBE_TEST_PIN);
      SERIAL_PROTOCOLLNPGM(". uses Z_MIN_PROBE_ENDSTOP_INVERTING (ignores Z_MIN_ENDSTOP_INVERTING)");
      SERIAL_PROTOCOLPGM(". Z_MIN_PROBE_ENDSTOP_INVERTING: ");

      #if Z_MIN_PROBE_ENDSTOP_INVERTING
        SERIAL_PROTOCOLLNPGM("true");
      #else
        SERIAL_PROTOCOLLNPGM("false");
      #endif

      probe_inverting = Z_MIN_PROBE_ENDSTOP_INVERTING;

    #endif

    SERIAL_PROTOCOLLNPGM(". deploy & stow 4 times");
    SET_INPUT_PULLUP(PROBE_TEST_PIN);
    uint8_t i = 0;
    bool deploy_state, stow_state;
    do {
      MOVE_SERVO(probe_index, servo_angles[Z_PROBE_SERVO_NR][0]); // Deploy
      safe_delay(500);
      deploy_state = READ(PROBE_TEST_PIN);
      MOVE_SERVO(probe_index, servo_angles[Z_PROBE_SERVO_NR][1]); // Stow
      safe_delay(500);
      stow_state = READ(PROBE_TEST_PIN);
    } while (++i < 4);
    if (probe_inverting != deploy_state) SERIAL_PROTOCOLLNPGM("WARNING - INVERTING setting probably backwards");

    if (deploy_state != stow_state) {
      SERIAL_PROTOCOLLNPGM("BLTouch clone detected");
      if (deploy_state) {
        SERIAL_PROTOCOLLNPGM(".  DEPLOYED state: HIGH (logic 1)");
        SERIAL_PROTOCOLLNPGM(".  STOWED (triggered) state: LOW (logic 0)");
      }
      else {
        SERIAL_PROTOCOLLNPGM(".  DEPLOYED state: LOW (logic 0)");
        SERIAL_PROTOCOLLNPGM(".  STOWED (triggered) state: HIGH (logic 1)");
      }
      #if ENABLED(BLTOUCH)
        SERIAL_PROTOCOLLNPGM("ERROR: BLTOUCH enabled - set this device up as a Z Servo Probe with inverting as true.");
      #endif
    }
    else {                                           // measure active signal length
      MOVE_SERVO(probe_index, servo_angles[Z_PROBE_SERVO_NR][0]); // Deploy
      safe_delay(500);
      SERIAL_PROTOCOLLNPGM("please trigger probe");
      uint16_t probe_counter = 0;

      // Allow 30 seconds max for operator to trigger probe
      for (uint16_t j = 0; j < 500 * 30 && probe_counter == 0 ; j++) {

        safe_delay(2);

        if (0 == j % (500 * 1)) gcode.reset_stepper_timeout(); // Keep steppers powered

        if (deploy_state != READ(PROBE_TEST_PIN)) { // probe triggered

          for (probe_counter = 1; probe_counter < 50 && deploy_state != READ(PROBE_TEST_PIN); ++probe_counter)
            safe_delay(2);

          if (probe_counter == 50)
            SERIAL_PROTOCOLLNPGM("Z Servo Probe detected"); // >= 100mS active time
          else if (probe_counter >= 2)
            SERIAL_PROTOCOLLNPAIR("BLTouch compatible probe detected - pulse width (+/- 4mS): ", probe_counter * 2); // allow 4 - 100mS pulse
          else
            SERIAL_PROTOCOLLNPGM("noise detected - please re-run test"); // less than 2mS pulse

          MOVE_SERVO(probe_index, servo_angles[Z_PROBE_SERVO_NR][1]); // Stow

        }  // pulse detected

      } // for loop waiting for trigger

      if (probe_counter == 0) SERIAL_PROTOCOLLNPGM("trigger not detected");

    } // measure active signal length

  #endif

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

  if (parser.seen('T')) {   // must be first or else its "S" and "E" parameters will execute endstop or servo test
    toggle_pins();
    return;
  }

  // Enable or disable endstop monitoring
  if (parser.seen('E')) {
    endstops.monitor_flag = parser.value_bool();
    SERIAL_PROTOCOLPGM("endstop monitor ");
    serialprintPGM(endstops.monitor_flag ? PSTR("en") : PSTR("dis"));
    SERIAL_PROTOCOLLNPGM("abled");
    return;
  }

  if (parser.seen('S')) {
    servo_probe_test();
    return;
  }

  // Get the range of pins to test or watch
  uint8_t first_pin = PARSED_PIN_INDEX('P', 0),
          last_pin = parser.seenval('P') ? first_pin : NUMBER_PINS_TOTAL - 1;

  if (first_pin > last_pin) return;

  const bool ignore_protection = parser.boolval('I');

  // Watch until click, M108, or reset
  if (parser.boolval('W')) {
    SERIAL_PROTOCOLLNPGM("Watching pins");

    #ifdef ARDUINO_ARCH_SAM
      NOLESS(first_pin, 2);  // don't hijack the UART pins
    #endif
    uint8_t pin_state[last_pin - first_pin + 1];
    for (uint8_t i = first_pin; i <= last_pin; i++) {
      pin_t pin = GET_PIN_MAP_PIN(i);
      if (!VALID_PIN(pin)) continue;
      if (!ignore_protection && pin_is_protected(pin)) continue;
      pinMode(pin, INPUT_PULLUP);
      delay(1);
      /*
        if (IS_ANALOG(pin))
          pin_state[pin - first_pin] = analogRead(DIGITAL_PIN_TO_ANALOG_PIN(pin)); // int16_t pin_state[...]
        else
      //*/
          pin_state[i - first_pin] = digitalRead(pin);
    }

    #if HAS_RESUME_CONTINUE
      wait_for_user = true;
      KEEPALIVE_STATE(PAUSED_FOR_USER);
    #endif

    for (;;) {
      for (uint8_t i = first_pin; i <= last_pin; i++) {
        pin_t pin = GET_PIN_MAP_PIN(i);
        if (!VALID_PIN(pin)) continue;
        if (!ignore_protection && pin_is_protected(pin)) continue;
        const byte val =
          /*
            IS_ANALOG(pin)
              ? analogRead(DIGITAL_PIN_TO_ANALOG_PIN(pin)) : // int16_t val
              :
          //*/
            digitalRead(pin);
        if (val != pin_state[i - first_pin]) {
          report_pin_state_extended(pin, ignore_protection, false);
          pin_state[i - first_pin] = val;
        }
      }

      #if HAS_RESUME_CONTINUE
        if (!wait_for_user) {
          KEEPALIVE_STATE(IN_HANDLER);
          break;
        }
      #endif

      safe_delay(200);
    }
    return;
  }

  // Report current state of selected pin(s)
  for (uint8_t i = first_pin; i <= last_pin; i++) {
    pin_t pin = GET_PIN_MAP_PIN(i);
    if (VALID_PIN(pin)) report_pin_state_extended(pin, ignore_protection, true);
  }
}

#endif // PINS_DEBUGGING
