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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

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
 */

// first pass - put the name strings into FLASH

#define _ADD_PIN_2(PIN_NAME, ENTRY_NAME) static const char ENTRY_NAME[] PROGMEM = { PIN_NAME };
#define _ADD_PIN(PIN_NAME, COUNTER) _ADD_PIN_2(PIN_NAME, entry_NAME_##COUNTER)
#define REPORT_NAME_DIGITAL(COUNTER, NAME) _ADD_PIN(#NAME, COUNTER)
#define REPORT_NAME_ANALOG(COUNTER, NAME) _ADD_PIN(#NAME, COUNTER)

#include "pinsDebug_list.h"
#line 49

// manually add pins that have names that are macros which don't play well with these macros
#if ANY(AVR_ATmega2560_FAMILY, AVR_ATmega1284_FAMILY, ARDUINO_ARCH_SAM, TARGET_LPC1768)
  #if SERIAL_IN_USE(0)
    static const char RXD_NAME_0[] PROGMEM = { "RXD0" };
    static const char TXD_NAME_0[] PROGMEM = { "TXD0" };
  #endif
  #if SERIAL_IN_USE(1)
    static const char RXD_NAME_1[] PROGMEM = { "RXD1" };
    static const char TXD_NAME_1[] PROGMEM = { "TXD1" };
  #endif
  #if SERIAL_IN_USE(2)
    static const char RXD_NAME_2[] PROGMEM = { "RXD2" };
    static const char TXD_NAME_2[] PROGMEM = { "TXD2" };
  #endif
  #if SERIAL_IN_USE(3)
    static const char RXD_NAME_3[] PROGMEM = { "RXD3" };
    static const char TXD_NAME_3[] PROGMEM = { "TXD3" };
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
   *         analog pin numbers were converted to digital when the array was created
   *     2 bytes containing the digital/analog bool flag
   */

  #if SERIAL_IN_USE(0)
    #if ANY(AVR_ATmega2560_FAMILY, ARDUINO_ARCH_SAM)
      { RXD_NAME_0, 0, true },
      { TXD_NAME_0, 1, true },
    #elif AVR_ATmega1284_FAMILY
      { RXD_NAME_0, 8, true },
      { TXD_NAME_0, 9, true },
    #elif defined(TARGET_LPC1768)           // TX P0_02  RX P0_03
      { RXD_NAME_0, 3, true },
      { TXD_NAME_0, 2, true },
    #endif
  #endif

  #if SERIAL_IN_USE(1)
    #if ANY(AVR_ATmega2560_FAMILY, ARDUINO_ARCH_SAM)
      { RXD_NAME_1, 19, true },
      { TXD_NAME_1, 18, true },
    #elif AVR_ATmega1284_FAMILY
      { RXD_NAME_1, 10, true },
      { TXD_NAME_1, 11, true },
    #elif defined(TARGET_LPC1768)
      #ifdef LPC_PINCFG_UART1_P2_00         // TX P2_00  RX P2_01
        { RXD_NAME_1, 0x41, true },
        { TXD_NAME_1, 0x40, true },
      #else                                 // TX P0_15  RX P0_16
        { RXD_NAME_1, 16, true },
        { TXD_NAME_1, 15, true },
      #endif
    #endif
  #endif

  #if SERIAL_IN_USE(2)
    #if ANY(AVR_ATmega2560_FAMILY, ARDUINO_ARCH_SAM)
      { RXD_NAME_2, 17, true },
      { TXD_NAME_2, 16, true },
    #elif defined(TARGET_LPC1768)
      #ifdef LPC_PINCFG_UART2_P2_08         // TX P2_08  RX P2_09
        { RXD_NAME_2, 0x49, true },
        { TXD_NAME_2, 0x48, true },
      #else                                 // TX P0_10  RX P0_11
        { RXD_NAME_2, 11, true },
        { TXD_NAME_2, 10, true },
      #endif
    #endif
  #endif

  #if SERIAL_IN_USE(3)
    #if ANY(AVR_ATmega2560_FAMILY, ARDUINO_ARCH_SAM)
      { RXD_NAME_3, 15, true },
      { TXD_NAME_3, 14, true },
    #elif defined(TARGET_LPC1768)
      #ifdef LPC_PINCFG_UART3_P0_25         // TX P0_25  RX P0_26
        { RXD_NAME_3, 0x1A, true },
        { TXD_NAME_3, 0x19, true },
      #elif defined(LPC_PINCFG_UART3_P4_28) // TX P4_28  RX P4_29
        { RXD_NAME_3, 0x9D, true },
        { TXD_NAME_3, 0x9C, true },
      #else                                 // TX P0_00  RX P0_01
        { RXD_NAME_3, 1, true },
        { TXD_NAME_3, 0, true },
      #endif
    #endif
  #endif

  #include "pinsDebug_list.h"
  #line 167

};

/**
 * Access to the pin table, available to the HAL's support file.
 * On AVR the table lives in PROGMEM and must be read with pgm_read_*.
 */
inline pin_t getPinByIndex(const uint16_t index) {
  #ifdef __AVR__
    return pin_t(pgm_read_byte(&pin_array[index].pin));
  #else
    return pin_t(pin_array[index].pin);
  #endif
}

inline bool pinTableIsDigital(const uint16_t index) {
  #ifdef __AVR__
    return pgm_read_byte(&pin_array[index].is_digital);
  #else
    return pin_array[index].is_digital;
  #endif
}

// Print a pin's name padded to MAX_NAME_LENGTH. Names are in PROGMEM.
inline void printPinNameByIndex(const uint16_t index) {
  PGM_P const name = (PGM_P)pgm_read_ptr(&pin_array[index].name);
  SERIAL_ECHOPGM_P(name);
  const size_t len = strlen_P(name);
  if (len < MAX_NAME_LENGTH) SERIAL_ECHO_SP(MAX_NAME_LENGTH - len);
}

/**
 * PinsDebugBase<Impl> - Shared pins debugging code
 *
 * Each HAL provides `struct PinsDebug : PinsDebugBase<PinsDebug>` and defines
 * only what is special to the platform. Anything the HAL leaves out falls back
 * to the default here. The base calls hooks as Impl::hook(), which resolves at
 * compile time to the HAL's version if there is one, so there is no vtable.
 *
 * Required of the HAL:
 *   static constexpr uint16_t count     Number of pins (M43 index range)
 *   static constexpr uint8_t name_pad   Space to pad a pin's additional names
 *   static bool is_analog(pin_t)        The pin is an analog input
 *   static int16_t analog_index(pin_t)  Analog input number, or -1
 *   static bool is_output(pin_t)        The pin is currently an output
 *
 * Optional (see the defaults below):
 *   pin_from_index, never_touch, is_valid, is_digital_at, read_digital,
 *   pwm_active, print_pwm, print_port, print_number, print_analog
 */
template<class Impl>
struct PinsDebugBase {

  // Map an M43 index to a pin number, and exclude indexes M43 must never touch
  static pin_t pin_from_index(const uint16_t index) { return pin_t(index); }
  static bool never_touch(const uint16_t) { return false; }

  static bool is_valid(const pin_t pin) { return WITHIN(pin, 0, pin_t(Impl::count - 1)); }

  // Is the pin at this table index a digital pin (versus analog)?
  static bool is_digital_at(const uint16_t index) { return pinTableIsDigital(index); }

  // Read a pin without changing its mode (some digitalRead implementations stop PWM)
  static int read_digital(const pin_t pin) { return extDigitalRead(pin); }

  // Print PWM status. pwm_active returns true if the pin is a PWM output
  // (and may print its duty). The default reports no PWM.
  static bool pwm_active(const pin_t) { return false; }
  static void print_pwm(const pin_t) {}

  static void print_port(const pin_t) {}

  static void print_number(const pin_t pin) {
    char buffer[16];
    sprintf_P(buffer, PSTR("%3d "), int(pin));
    SERIAL_ECHO(buffer);
  }

  static void print_analog(const pin_t pin) {
    char buffer[20];
    sprintf_P(buffer, PSTR(" (A%2d)  "), int(Impl::analog_index(pin)));
    SERIAL_ECHO(buffer);
  }

  // An M43 command must not touch this pin
  static bool is_untouchable(const uint16_t index, const pin_t pin, const bool ignore_protection) {
    return Impl::never_touch(index) || (!ignore_protection && marlin.pin_is_protected(pin));
  }

  // Get the pin for an M43 index. False if the index maps to no valid pin.
  static FORCE_INLINE bool get_pin(const uint16_t index, pin_t &pin) {
    pin = Impl::pin_from_index(index);
    return Impl::is_valid(pin);
  }

  // Same, but also false for a pin M43 mustn't touch
  static FORCE_INLINE bool get_touchable_pin(const uint16_t index, const bool ignore_protection, pin_t &pin) {
    return get_pin(index, pin) && !is_untouchable(index, pin, ignore_protection);
  }

  // Pretty report with PWM info
  static void print_state(const pin_t pin, const bool ignore, const bool extended=false, FSTR_P const start_string=nullptr) {
    char buffer[MAX_NAME_LENGTH + 1];   // for the sprintf statements
    bool found = false, multi_name_pin = false;

    auto alt_pin_echo = [](const pin_t &pin) {
      #if AVR_AT90USB1286_FAMILY
        // Use FastIO for pins Teensy doesn't expose
        if (pin == PIN_E2) {
          print_io_state(IS_OUTPUT(PIN_E2));
          print_state_name(READ(PIN_E2));
          return false;
        }
        else if (pin == PIN_E3) {
          print_io_state(IS_OUTPUT(PIN_E3));
          print_state_name(READ(PIN_E3));
          return false;
        }
      #endif
      return true;
    };

    for (uint8_t x = 0; x < COUNT(pin_array); ++x)  {    // scan entire array and report all instances of this pin
      if (getPinByIndex(x) == pin) {
        if (!found) {    // report digital and analog pin number only on the first time through
          if (start_string) SERIAL_ECHO(start_string);
          SERIAL_ECHOPGM("PIN: ");
          Impl::print_number(pin);
          Impl::print_port(pin);
          if (int8_t(Impl::analog_index(pin)) >= 0) Impl::print_analog(pin); // analog pin number
          else SERIAL_ECHO_SP(8);                                            // add padding if not an analog pin
        }
        else {
          SERIAL_CHAR('.');
          SERIAL_ECHO_SP(Impl::name_pad + (start_string ? strlen_P(FTOP(start_string)) : 0));  // add padding if not the first instance found
        }
        printPinNameByIndex(x);
        if (extended) {
          if (marlin.pin_is_protected(pin) && !ignore)
            SERIAL_ECHOPGM("protected ");
          else {
            if (alt_pin_echo(pin)) {
              if (!Impl::is_digital_at(x)) {
                sprintf_P(buffer, PSTR("Analog in = %5ld"), (long)analogRead(int8_t(Impl::analog_index(pin))));
                SERIAL_ECHO(buffer);
              }
              else {
                if (!Impl::is_output(pin)) {
                  //pinMode(pin, INPUT_PULLUP);  // make sure input isn't floating - stopped doing this
                                                 // because this could interfere with inductive/capacitive
                                                 // sensors (high impedance voltage divider) and with Pt100 amplifier
                  print_io_state(false);
                  print_state_name(Impl::read_digital(pin));
                }
                else if (Impl::pwm_active(pin)) {
                  // do nothing
                }
                else {
                  print_io_state(true);
                  print_state_name(Impl::read_digital(pin));
                }
              }
              if (!multi_name_pin && extended) Impl::print_pwm(pin);  // report PWM capabilities only on the first pass & only if doing an extended report
            }
          }
        }
        SERIAL_EOL();
        multi_name_pin = found;
        found = true;
      }  // end of IF
    } // end of for loop

    if (!found) {
      if (start_string) SERIAL_ECHO(start_string);
      SERIAL_ECHOPGM("PIN: ");
      Impl::print_number(pin);
      Impl::print_port(pin);
      if (int8_t(Impl::analog_index(pin)) >= 0) Impl::print_analog(pin); // analog pin number
      else SERIAL_ECHO_SP(8);                                            // add padding if not an analog pin
      SERIAL_ECHOPGM("<unused/unknown>");
      if (extended) {

        if (alt_pin_echo(pin)) {
          if (Impl::pwm_active(pin)) {
            // do nothing
          }
          else if (Impl::is_output(pin)) {
            SERIAL_ECHO_SP(MAX_NAME_LENGTH - 16);
            print_io_state(true);
            print_state_name(Impl::read_digital(pin));
          }
          else {
            if (Impl::is_analog(pin)) {
              sprintf_P(buffer, PSTR("   Analog in = %5ld"), (long)analogRead(int8_t(Impl::analog_index(pin))));
              SERIAL_ECHO(buffer);
              SERIAL_ECHOPGM("   ");
            }
            else
            SERIAL_ECHO_SP(MAX_NAME_LENGTH - 16);   // add padding if not an analog pin

            print_io_state(false);
            print_state_name(Impl::read_digital(pin));
          }
          //if (!Impl::pwm_active(pin)) SERIAL_CHAR(' ');    // add padding if it's not a PWM pin
          if (extended) {
            SERIAL_ECHO_SP(MAX_NAME_LENGTH - 16);
            Impl::print_pwm(pin);  // report PWM capabilities only if doing an extended report
          }
        }
      }
      SERIAL_EOL();
    }
  }

private:
  static void print_io_state(const bool isout) { SERIAL_ECHO(isout ? F("Output ") : F("Input  ")); }
  static void print_state_name(const bool state) { SERIAL_ECHO(state ? F("HIGH") : F("LOW")); }
};

#include HAL_PATH(.., pinsDebug.h)  // get the correct support file for this CPU (declares PinsDebug)
