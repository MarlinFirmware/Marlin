/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#if (AVR_ATmega2560_FAMILY || AVR_ATmega1284_FAMILY || defined(ARDUINO_ARCH_SAM) || defined(TARGET_LPC1768))
  #if SERIAL_PORT == 0
    static const char RXD_NAME_0[] PROGMEM = { "RXD0" };
    static const char TXD_NAME_0[] PROGMEM = { "TXD0" };
  #elif SERIAL_PORT == 1
    static const char RXD_NAME_1[] PROGMEM = { "RXD1" };
    static const char TXD_NAME_1[] PROGMEM = { "TXD1" };
  #elif SERIAL_PORT == 2
    static const char RXD_NAME_2[] PROGMEM = { "RXD2" };
    static const char TXD_NAME_2[] PROGMEM = { "TXD2" };
  #elif SERIAL_PORT == 3
    static const char RXD_NAME_3[] PROGMEM = { "RXD3" };
    static const char TXD_NAME_3[] PROGMEM = { "TXD3" };
  #endif
  #ifdef SERIAL_PORT_2
    #if SERIAL_PORT_2 == 0
      static const char RXD_NAME_0[] PROGMEM = { "RXD0" };
      static const char TXD_NAME_0[] PROGMEM = { "TXD0" };
    #elif SERIAL_PORT_2 == 1
      static const char RXD_NAME_1[] PROGMEM = { "RXD1" };
      static const char TXD_NAME_1[] PROGMEM = { "TXD1" };
    #elif SERIAL_PORT_2 == 2
      static const char RXD_NAME_2[] PROGMEM = { "RXD2" };
      static const char TXD_NAME_2[] PROGMEM = { "TXD2" };
    #elif SERIAL_PORT_2 == 3
      static const char RXD_NAME_3[] PROGMEM = { "RXD3" };
      static const char TXD_NAME_3[] PROGMEM = { "TXD3" };
    #endif
  #endif
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
      { RXD_NAME_0, 0, true },
      { TXD_NAME_0, 1, true },
    #elif AVR_ATmega1284_FAMILY
      { RXD_NAME_0, 8, true },
      { TXD_NAME_0, 9, true },
    #elif defined(TARGET_LPC1768)
      { RXD_NAME_0, 3, true },
      { TXD_NAME_0, 2, true },
    #endif
  #elif SERIAL_PORT == 1
    #if (AVR_ATmega2560_FAMILY || defined(ARDUINO_ARCH_SAM))
      { RXD_NAME_1, 19, true },
      { TXD_NAME_1, 18, true },
    #elif AVR_ATmega1284_FAMILY
      { RXD_NAME_1, 10, true },
      { TXD_NAME_1, 11, true },
    #elif defined(TARGET_LPC1768)
      { RXD_NAME_1, 16, true },
      { TXD_NAME_1, 15, true },
    #endif
  #elif SERIAL_PORT == 2
    #if (AVR_ATmega2560_FAMILY || defined(ARDUINO_ARCH_SAM))
      { RXD_NAME_2, 17, true },
      { TXD_NAME_2, 16, true },
    #elif defined(TARGET_LPC1768)
      { RXD_NAME_2, 11, true },
      { TXD_NAME_2, 10, true },
    #endif
  #elif SERIAL_PORT == 3
    #if (AVR_ATmega2560_FAMILY || defined(ARDUINO_ARCH_SAM))
      { RXD_NAME_3, 15, true },
      { TXD_NAME_3, 14, true },
    #elif defined(TARGET_LPC1768)
      { RXD_NAME_3, 1, true },
      { TXD_NAME_3, 0, true },
    #endif
  #endif

  #ifdef SERIAL_PORT_2
    #if SERIAL_PORT_2 == 0
      #if (AVR_ATmega2560_FAMILY || defined(ARDUINO_ARCH_SAM))
        { RXD_NAME_0, 0, true },
        { TXD_NAME_0, 1, true },
      #elif AVR_ATmega1284_FAMILY
        { RXD_NAME_0, 8, true },
        { TXD_NAME_0, 9, true },
      #elif defined(TARGET_LPC1768)
        { RXD_NAME_0, 3, true },
        { TXD_NAME_0, 2, true },
      #endif
    #elif SERIAL_PORT_2 == 1
      #if (AVR_ATmega2560_FAMILY || defined(ARDUINO_ARCH_SAM))
        { RXD_NAME_1, 19, true },
        { TXD_NAME_1, 18, true },
      #elif AVR_ATmega1284_FAMILY
        { RXD_NAME_1, 10, true },
        { TXD_NAME_1, 11, true },
      #elif defined(TARGET_LPC1768)
        { RXD_NAME_1, 16, true },
        { TXD_NAME_1, 15, true },
      #endif
    #elif SERIAL_PORT_2 == 2
      #if (AVR_ATmega2560_FAMILY || defined(ARDUINO_ARCH_SAM))
        { RXD_NAME_2, 17, true },
        { TXD_NAME_2, 16, true },
      #elif defined(TARGET_LPC1768)
        { RXD_NAME_2, 11, true },
        { TXD_NAME_2, 10, true },
      #endif
    #elif SERIAL_PORT_2 == 3
      #if (AVR_ATmega2560_FAMILY || defined(ARDUINO_ARCH_SAM))
        { RXD_NAME_3, 15, true },
        { TXD_NAME_3, 14, true },
      #elif defined(TARGET_LPC1768)
        { RXD_NAME_3, 1, true },
        { TXD_NAME_3, 0, true },
      #endif
    #endif
  #endif

  #include "pinsDebug_list.h"
  #line 172

};

#include HAL_PATH(../HAL, pinsDebug.h)  // get the correct support file for this CPU

#ifndef M43_NEVER_TOUCH
  #define M43_NEVER_TOUCH(Q) false
#endif

static void print_input_or_output(const bool isout) {
  serialprintPGM(isout ? PSTR("Output = ") : PSTR("Input  = "));
}

// pretty report with PWM info
inline void report_pin_state_extended(pin_t pin, const bool ignore, const bool extended=false, PGM_P const start_string=nullptr) {
  char buffer[MAX_NAME_LENGTH + 1];   // for the sprintf statements
  bool found = false, multi_name_pin = false;

  auto alt_pin_echo = [](const pin_t &pin) {
    #if AVR_AT90USB1286_FAMILY
      // Use FastIO for pins Teensy doesn't expose
      if (pin == 46) {
        print_input_or_output(IS_OUTPUT(46));
        SERIAL_CHAR('0' + READ(46));
        return false;
      }
      else if (pin == 47) {
        print_input_or_output(IS_OUTPUT(47));
        SERIAL_CHAR('0' + READ(47));
        return false;
      }
    #endif
    return true;
  };

  LOOP_L_N(x, COUNT(pin_array))  {    // scan entire array and report all instances of this pin
    if (GET_ARRAY_PIN(x) == pin) {
      if (!found) {    // report digital and analog pin number only on the first time through
        if (start_string) serialprintPGM(start_string);
        serialprintPGM(PSTR("PIN: "));
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
        SERIAL_ECHO_SP(MULTI_NAME_PAD + (start_string ? strlen_P(start_string) : 0));  // add padding if not the first instance found
      }
      PRINT_ARRAY_NAME(x);
      if (extended) {
        if (pin_is_protected(pin) && !ignore)
          SERIAL_ECHOPGM("protected ");
        else {
          if (alt_pin_echo(pin)) {
            if (!GET_ARRAY_IS_DIGITAL(x)) {
              sprintf_P(buffer, PSTR("Analog in = %5ld"), (long)analogRead(DIGITAL_PIN_TO_ANALOG_PIN(pin)));
              SERIAL_ECHO(buffer);
            }
            else {
              if (!GET_PINMODE(pin)) {
                //pinMode(pin, INPUT_PULLUP);  // make sure input isn't floating - stopped doing this
                                               // because this could interfere with inductive/capacitive
                                               // sensors (high impedance voltage divider) and with Pt100 amplifier
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
      multi_name_pin = found;
      found = true;
    }  // end of IF
  } // end of for loop

  if (!found) {
    if (start_string) serialprintPGM(start_string);
    serialprintPGM(PSTR("PIN: "));
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

      if (alt_pin_echo(pin)) {
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
