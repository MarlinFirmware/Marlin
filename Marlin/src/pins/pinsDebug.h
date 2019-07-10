/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#define MAX_NAME_LENGTH  39    // one place to specify the format of all the sources of names
                               // "-" left justify, "39" minimum width of name, pad with blanks

/**
 *  This routine minimizes RAM usage by creating a FLASH resident array to
 *  store the pin names, pin numbers and analog/digital flag.
 *
 *  Creating the array in FLASH is a two pass process.  The first pass puts the
 *  name strings into FLASH.  The second pass actually creates the array.
 *
 *  Both passes use the same pin list.  The list contains two macro names. The
 *  actual macro definitions are changed depending on which pass is being done.
 *
 */

// first pass - put the name strings into FLASH

#define _ADD_PIN_2(PIN_NAME, ENTRY_NAME) static const char ENTRY_NAME[] PROGMEM = { PIN_NAME };
#define _ADD_PIN(PIN_NAME, COUNTER) _ADD_PIN_2(PIN_NAME, entry_NAME_##COUNTER)
#define REPORT_NAME_DIGITAL(COUNTER, NAME) _ADD_PIN(#NAME, COUNTER)
#define REPORT_NAME_ANALOG(COUNTER, NAME) _ADD_PIN(#NAME, COUNTER)

#include "pinsDebug_list.h"
#line 46

// manually add pins that have names that are macros which don't play well with these macros
#if SERIAL_PORT == 0 && (AVR_ATmega2560_FAMILY || AVR_ATmega1284_FAMILY || defined(ARDUINO_ARCH_SAM))
  static const char RXD_NAME[] PROGMEM = { "RXD" };
  static const char TXD_NAME[] PROGMEM = { "TXD" };
#endif

/////////////////////////////////////////////////////////////////////////////

// second pass - create the array

#undef _ADD_PIN_2
#undef _ADD_PIN
#undef REPORT_NAME_DIGITAL
#undef REPORT_NAME_ANALOG

#define _ADD_PIN_2(ENTRY_NAME, NAME, IS_DIGITAL) { ENTRY_NAME, NAME, IS_DIGITAL },
#define _ADD_PIN(NAME, COUNTER, IS_DIGITAL) _ADD_PIN_2(entry_NAME_##COUNTER, NAME, IS_DIGITAL)
#define REPORT_NAME_DIGITAL(COUNTER, NAME) _ADD_PIN(NAME, COUNTER, true)
#define REPORT_NAME_ANALOG(COUNTER, NAME) _ADD_PIN(analogInputToDigitalPin(NAME), COUNTER, false)


typedef struct {
  PGM_P const name;
  pin_t pin;
  bool is_digital;
} PinInfo;

const PinInfo pin_array[] PROGMEM = {

  /**
   *  [pin name]  [pin number]  [is digital or analog]  1 = digital, 0 = analog
   *  Each entry takes up 6 bytes in FLASH:
   *     2 byte pointer to location of the name string
   *     2 bytes containing the pin number
   *         analog pin numbers were convereted to digital when the array was created
   *     2 bytes containing the digital/analog bool flag
   */

  // manually add pins ...
  #if SERIAL_PORT == 0
    #if (AVR_ATmega2560_FAMILY || defined(ARDUINO_ARCH_SAM))
      { RXD_NAME, 0, true },
      { TXD_NAME, 1, true },
    #elif AVR_ATmega1284_FAMILY
      { RXD_NAME, 8, true },
      { TXD_NAME, 9, true },
    #endif
  #endif

  #include "pinsDebug_list.h"
  #line 98

};

#include HAL_PATH(../HAL, pinsDebug.h)  // get the correct support file for this CPU

#ifndef M43_NEVER_TOUCH
  #define M43_NEVER_TOUCH(Q) false
#endif

static void print_input_or_output(const bool isout) {
  serialprintPGM(isout ? PSTR("Output = ") : PSTR("Input  = "));
}

// pretty report with PWM info
inline void report_pin_state_extended(pin_t pin, bool ignore, bool extended = false, const char *start_string = "") {
  char buffer[MAX_NAME_LENGTH + 1];   // for the sprintf statements
  bool found = false, multi_name_pin = false;

  for (uint8_t x = 0; x < COUNT(pin_array); x++)  {    // scan entire array and report all instances of this pin
    if (GET_ARRAY_PIN(x) == pin) {
      if (found) multi_name_pin = true;
      found = true;
      if (!multi_name_pin) {    // report digitial and analog pin number only on the first time through
        sprintf_P(buffer, PSTR("%sPIN: "), start_string);     // digital pin number
        SERIAL_ECHO(buffer);
        PRINT_PIN(pin);
        PRINT_PORT(pin);
        if (int8_t(DIGITAL_PIN_TO_ANALOG_PIN(pin)) >= 0) {
          sprintf_P(buffer, PSTR(" (A%2d)  "), DIGITAL_PIN_TO_ANALOG_PIN(pin));    // analog pin number
          SERIAL_ECHO(buffer);
        }
        else SERIAL_ECHO_SP(8);   // add padding if not an analog pin
      }
      else {
        SERIAL_CHAR('.');
        SERIAL_ECHO_SP(MULTI_NAME_PAD + strlen(start_string));  // add padding if not the first instance found
      }
      PRINT_ARRAY_NAME(x);
      if (extended) {
        if (pin_is_protected(pin) && !ignore)
          SERIAL_ECHOPGM("protected ");
        else {
          #if AVR_AT90USB1286_FAMILY //Teensy IDEs don't know about these pins so must use FASTIO
            if (pin == 46 || pin == 47) {
              if (pin == 46) {
                print_input_or_output(IS_OUTPUT(46));
                SERIAL_CHAR('0' + READ(46));
              }
              else if (pin == 47) {
                print_input_or_output(IS_OUTPUT(47));
                SERIAL_CHAR('0' + READ(47));
              }
            }
            else
          #endif
          {
            if (!GET_ARRAY_IS_DIGITAL(x)) {
              sprintf_P(buffer, PSTR("Analog in = %5ld"), (long)analogRead(DIGITAL_PIN_TO_ANALOG_PIN(pin)));
              SERIAL_ECHO(buffer);
            }
            else {
              if (!GET_PINMODE(pin)) {
                //pinMode(pin, INPUT_PULLUP);  // make sure input isn't floating - stopped doing this
                                               // because this could interfere with inductive/capacitive
                                               // sensors (high impedance voltage divider) and with PT100 amplifier
                print_input_or_output(false);
                SERIAL_ECHO(digitalRead_mod(pin));
              }
              else if (pwm_status(pin)) {
                // do nothing
              }
              else {
                print_input_or_output(true);
                SERIAL_ECHO(digitalRead_mod(pin));
              }
            }
            if (!multi_name_pin && extended) pwm_details(pin);  // report PWM capabilities only on the first pass & only if doing an extended report
          }
        }
      }
      SERIAL_EOL();
    }  // end of IF
  } // end of for loop

  if (!found) {
    sprintf_P(buffer, PSTR("%sPIN: "), start_string);
    SERIAL_ECHO(buffer);
    PRINT_PIN(pin);
    PRINT_PORT(pin);
    if (int8_t(DIGITAL_PIN_TO_ANALOG_PIN(pin)) >= 0) {
      sprintf_P(buffer, PSTR(" (A%2d)  "), DIGITAL_PIN_TO_ANALOG_PIN(pin));    // analog pin number
      SERIAL_ECHO(buffer);
    }
    else
      SERIAL_ECHO_SP(8);   // add padding if not an analog pin
    SERIAL_ECHOPGM("<unused/unknown>");
    if (extended) {
      #if AVR_AT90USB1286_FAMILY  //Teensy IDEs don't know about these pins so must use FASTIO
        if (pin == 46 || pin == 47) {
          SERIAL_ECHO_SP(12);
          if (pin == 46) {
            print_input_or_output(IS_OUTPUT(46));
            SERIAL_CHAR('0' + READ(46));
          }
          else {
            print_input_or_output(IS_OUTPUT(47));
            SERIAL_CHAR('0' + READ(47));
          }
        }
        else
      #endif
      {
        if (pwm_status(pin)) {
          // do nothing
        }
        else if (GET_PINMODE(pin)) {
          SERIAL_ECHO_SP(MAX_NAME_LENGTH - 16);
          print_input_or_output(true);
          SERIAL_ECHO(digitalRead_mod(pin));
        }
        else {
          if (IS_ANALOG(pin)) {
            sprintf_P(buffer, PSTR("   Analog in = %5ld"), (long)analogRead(DIGITAL_PIN_TO_ANALOG_PIN(pin)));
            SERIAL_ECHO(buffer);
            SERIAL_ECHOPGM("   ");
          }
          else
          SERIAL_ECHO_SP(MAX_NAME_LENGTH - 16);   // add padding if not an analog pin

          print_input_or_output(false);
          SERIAL_ECHO(digitalRead_mod(pin));
        }
        //if (!pwm_status(pin)) SERIAL_CHAR(' ');    // add padding if it's not a PWM pin
        if (extended) {
          SERIAL_ECHO_SP(MAX_NAME_LENGTH - 16);
          pwm_details(pin);  // report PWM capabilities only if doing an extended report
        }
      }
    }
    SERIAL_EOL();
  }
}
