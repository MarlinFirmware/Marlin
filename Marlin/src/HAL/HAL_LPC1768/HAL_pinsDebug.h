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

bool endstop_monitor_flag = false;

#define MAX_NAME_LENGTH  35    // one place to specify the format of all the sources of names
                               // "-" left justify, "35" minimum width of name, pad with blanks

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
#define REPORT_NAME_DIGITAL(NAME, COUNTER) _ADD_PIN(#NAME, COUNTER)
#define REPORT_NAME_ANALOG(NAME, COUNTER) _ADD_PIN(#NAME, COUNTER)

#include "../../../pinsDebug_list.h"
#line 51

/////////////////////////////////////////////////////////////////////////////

// second pass - create the array

#undef _ADD_PIN_2
#undef _ADD_PIN
#undef REPORT_NAME_DIGITAL
#undef REPORT_NAME_ANALOG

#define _ADD_PIN_2(ENTRY_NAME, NAME, IS_DIGITAL) { ENTRY_NAME, NAME, IS_DIGITAL },
#define _ADD_PIN(NAME, COUNTER, IS_DIGITAL) _ADD_PIN_2(entry_NAME_##COUNTER, NAME, IS_DIGITAL)
#define REPORT_NAME_DIGITAL(NAME, COUNTER) _ADD_PIN(NAME, COUNTER, true)
#define REPORT_NAME_ANALOG(NAME, COUNTER) _ADD_PIN(analogInputToDigitalPin(NAME), COUNTER, false)


typedef struct {
  const char * const name;
  uint8_t pin;
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

  #endif

  #include "../../../pinsDebug_list.h"
  #line 102

};

#define AVR_ATmega2560_FAMILY_PLUS_70 (MOTHERBOARD == BOARD_BQ_ZUM_MEGA_3D \
|| MOTHERBOARD == BOARD_MIGHTYBOARD_REVE \
|| MOTHERBOARD == BOARD_MINIRAMBO \
|| MOTHERBOARD == BOARD_SCOOVO_X9H)


#include "pinsDebug_Re_ARM.h"

#define PWM_PRINT(V) do{ sprintf_P(buffer, PSTR("PWM:  %4d"), V); SERIAL_ECHO(buffer); }while(0)
#define PWM_CASE(N,Z)                                           \
  case TIMER##N##Z:                                             \
    if (TCCR##N##A & (_BV(COM##N##Z##1) | _BV(COM##N##Z##0))) { \
      PWM_PRINT(OCR##N##Z);                                     \
      return true;                                              \
    } else return false

bool PWM_ok = true;

static void print_input_or_output(const bool isout) {
  serialprintPGM(isout ? PSTR("Output = ") : PSTR("Input  = "));
}

// pretty report with PWM info
inline void report_pin_state_extended(int8_t pin, bool ignore, bool extended = false, const char *start_string = "") {
  uint8_t temp_char;
  char *name_mem_pointer, buffer[30];   // for the sprintf statements
  bool found = false, multi_name_pin = false;
  for (uint8_t x = 0; x < COUNT(pin_array); x++)  {    // scan entire array and report all instances of this pin
    if (GET_ARRAY_PIN(x) == pin) {
      GET_PIN_INFO(pin);
      if (found) multi_name_pin = true;
      found = true;
      if (!multi_name_pin) {    // report digitial and analog pin number only on the first time through
        sprintf_P(buffer, PSTR("%sPIN: %3d "), start_string, pin);     // digital pin number
        SERIAL_ECHO(buffer);
        PRINT_PORT(pin);
        if (IS_ANALOG(pin)) {
          sprintf_P(buffer, PSTR(" (A%2d)  "), DIGITAL_PIN_TO_ANALOG_PIN(pin));    // analog pin number
          SERIAL_ECHO(buffer);
        }
        else SERIAL_ECHO_SP(8);   // add padding if not an analog pin
      }
      else {
        SERIAL_CHAR('.');
        SERIAL_ECHO_SP(26 + strlen(start_string));  // add padding if not the first instance found
      }
      PRINT_ARRAY_NAME(x);
      if (extended) {
        if (pin_is_protected(pin) && !ignore)
          SERIAL_ECHOPGM("protected ");
        else {
//SERIAL_PROTOCOLPAIR(" GET_ARRAY_IS_DIGITAL(x) 0 = analog : ", GET_ARRAY_IS_DIGITAL(x));           

          if (!GET_ARRAY_IS_DIGITAL(x)) {
            sprintf_P(buffer, PSTR("Analog in = %5d"), analogRead(DIGITAL_PIN_TO_ANALOG_PIN(pin)));
            SERIAL_ECHO(buffer);
          }
          else {

//MYSERIAL.printf("  GET_PINMODE(pin) 1 = output : %d   ", GET_PINMODE(pin));           
            if (!GET_PINMODE(pin)) {
              //pinMode(pin, INPUT_PULLUP);  // make sure input isn't floating - stopped doing this
                                             // because this could interfere with inductive/capacitive
                                             // sensors (high impedance voltage divider) and with PT100 amplifier
              print_input_or_output(false);
              SERIAL_PROTOCOL(digitalRead_mod(pin));
            }
            #if PWM_ok
              else if (pwm_status(pin)) {
                // do nothing
              }
              #endif
            else {
              print_input_or_output(true);
              SERIAL_PROTOCOL(digitalRead_mod(pin));
            }
          }
          #if PWM_ok
            if (!multi_name_pin && extended) pwm_details(pin);  // report PWM capabilities only on the first pass & only if doing an extended report
          #endif
        }
      }
      SERIAL_EOL();
    }  // end of IF
  } // end of for loop

  if (!found) {
    GET_PIN_INFO(pin);
    sprintf_P(buffer, PSTR("%sPIN: %3d "), start_string, pin);
    SERIAL_ECHO(buffer);
    PRINT_PORT(pin);
    if (IS_ANALOG(pin)) {
      sprintf_P(buffer, PSTR(" (A%2d)  "), DIGITAL_PIN_TO_ANALOG_PIN(pin));    // analog pin number
      SERIAL_ECHO(buffer);
    }
    else
      SERIAL_ECHO_SP(8);   // add padding if not an analog pin
    SERIAL_ECHOPGM("<unused/unknown>");
    if (extended) {
      if (GET_PINMODE(pin)) {
        SERIAL_PROTOCOL_SP(MAX_NAME_LENGTH - 16);
        print_input_or_output(true);
        SERIAL_PROTOCOL(digitalRead_mod(pin));
      }
      else {
        if (IS_ANALOG(pin)) {
          sprintf_P(buffer, PSTR("   Analog in = %5d"), analogRead(DIGITAL_PIN_TO_ANALOG_PIN(pin)));
          SERIAL_ECHO(buffer);
          SERIAL_ECHOPGM("   ");
        }
        else
        SERIAL_ECHO_SP(MAX_NAME_LENGTH - 16);   // add padding if not an analog pin

        print_input_or_output(false);
        SERIAL_PROTOCOL(digitalRead_mod(pin));
      }
      //if (!pwm_status(pin)) SERIAL_CHAR(' ');    // add padding if it's not a PWM pin
      #if PWM_ok
        if (extended) pwm_details(pin);  // report PWM capabilities only if doing an extended report
      #endif
    }
    SERIAL_EOL();
  }
}
