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

#define NAME_FORMAT "%-35s"   // one place to specify the format of all the sources of names
                               // "-" left justify, "28" minimum width of name, pad with blanks

#define IS_ANALOG(P) ((P) >= analogInputToDigitalPin(0) && ((P) <= analogInputToDigitalPin(15) || (P) <= analogInputToDigitalPin(7)))

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

#include "pinsDebug_list.h"
#line 51

// manually add pins that have names that are macros which don't play well with these macros
#if SERIAL_PORT == 0 && (AVR_ATmega2560_FAMILY || AVR_ATmega1284_FAMILY)
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
    #if AVR_ATmega2560_FAMILY
      { RXD_NAME, 0, true },
      { TXD_NAME, 1, true },
    #elif AVR_ATmega1284_FAMILY
      { RXD_NAME, 8, true },
      { TXD_NAME, 9, true },
    #endif
  #endif

  #include "pinsDebug_list.h"
  #line 102

};

#define AVR_ATmega2560_FAMILY_PLUS_70 (MOTHERBOARD == BOARD_BQ_ZUM_MEGA_3D \
|| MOTHERBOARD == BOARD_MIGHTYBOARD_REVE \
|| MOTHERBOARD == BOARD_MINIRAMBO \
|| MOTHERBOARD == BOARD_SCOOVO_X9H)

#if AVR_AT90USB1286_FAMILY
  // Working with Teensyduino extension so need to re-define some things
  #include "pinsDebug_Teensyduino.h"
  // Can't use the "digitalPinToPort" function from the Teensyduino type IDEs
  // portModeRegister takes a different argument
  #define digitalPinToTimer_DEBUG(p) digitalPinToTimer(p)
  #define digitalPinToBitMask_DEBUG(p) digitalPinToBitMask(p)
  #define digitalPinToPort_DEBUG(p) digitalPinToPort_Teensy(p)
  #define get_pinMode(pin) (*portModeRegister(pin) & digitalPinToBitMask_DEBUG(pin))
#elif AVR_ATmega2560_FAMILY_PLUS_70   // So we can access/display all the pins on boards using more than 70
  #include "pinsDebug_plus_70.h"
  #define digitalPinToTimer_DEBUG(p) digitalPinToTimer_plus_70(p)
  #define digitalPinToBitMask_DEBUG(p) digitalPinToBitMask_plus_70(p)
  #define digitalPinToPort_DEBUG(p) digitalPinToPort_plus_70(p)
  bool get_pinMode(int8_t pin) {return *portModeRegister(digitalPinToPort_DEBUG(pin)) & digitalPinToBitMask_DEBUG(pin); }
#else
  #define digitalPinToTimer_DEBUG(p) digitalPinToTimer(p)
  #define digitalPinToBitMask_DEBUG(p) digitalPinToBitMask(p)
  #define digitalPinToPort_DEBUG(p) digitalPinToPort(p)
  bool get_pinMode(int8_t pin) {return *portModeRegister(digitalPinToPort_DEBUG(pin)) & digitalPinToBitMask_DEBUG(pin); }
#endif

#if defined(__AVR_ATmega1284P__)  // 1284 IDE extensions set this to the number of
  #undef NUM_DIGITAL_PINS         // digital only pins while all other CPUs have it
  #define NUM_DIGITAL_PINS 32     // set to digital only + digital/analog
#endif

#define PWM_PRINT(V) do{ sprintf_P(buffer, PSTR("PWM:  %4d"), V); SERIAL_ECHO(buffer); }while(0)
#define PWM_CASE(N,Z)                                           \
  case TIMER##N##Z:                                             \
    if (TCCR##N##A & (_BV(COM##N##Z##1) | _BV(COM##N##Z##0))) { \
      PWM_PRINT(OCR##N##Z);                                     \
      return true;                                              \
    } else return false

/**
 * Print a pin's PWM status.
 * Return true if it's currently a PWM pin.
 */
static bool pwm_status(uint8_t pin) {
  char buffer[20];   // for the sprintf statements

  switch (digitalPinToTimer_DEBUG(pin)) {

    #if defined(TCCR0A) && defined(COM0A1)
      #ifdef TIMER0A
        #if !AVR_AT90USB1286_FAMILY  // not available in Teensyduino type IDEs
          PWM_CASE(0, A);
        #endif
      #endif
      PWM_CASE(0, B);
    #endif

    #if defined(TCCR1A) && defined(COM1A1)
      PWM_CASE(1, A);
      PWM_CASE(1, B);
     #if defined(COM1C1) && defined(TIMER1C)
      PWM_CASE(1, C);
     #endif
    #endif

    #if defined(TCCR2A) && defined(COM2A1)
      PWM_CASE(2, A);
      PWM_CASE(2, B);
    #endif

    #if defined(TCCR3A) && defined(COM3A1)
      PWM_CASE(3, A);
      PWM_CASE(3, B);
      #ifdef COM3C1
        PWM_CASE(3, C);
      #endif
    #endif

    #ifdef TCCR4A
      PWM_CASE(4, A);
      PWM_CASE(4, B);
      PWM_CASE(4, C);
    #endif

    #if defined(TCCR5A) && defined(COM5A1)
      PWM_CASE(5, A);
      PWM_CASE(5, B);
      PWM_CASE(5, C);
    #endif

    case NOT_ON_TIMER:
    default:
      return false;
  }
  SERIAL_PROTOCOL_SP(2);
} // pwm_status


const volatile uint8_t* const PWM_other[][3] PROGMEM = {
    { &TCCR0A, &TCCR0B, &TIMSK0 },
    { &TCCR1A, &TCCR1B, &TIMSK1 },
  #if defined(TCCR2A) && defined(COM2A1)
    { &TCCR2A, &TCCR2B, &TIMSK2 },
  #endif
  #if defined(TCCR3A) && defined(COM3A1)
    { &TCCR3A, &TCCR3B, &TIMSK3 },
  #endif
  #ifdef TCCR4A
    { &TCCR4A, &TCCR4B, &TIMSK4 },
  #endif
  #if defined(TCCR5A) && defined(COM5A1)
    { &TCCR5A, &TCCR5B, &TIMSK5 },
  #endif
};


const volatile uint8_t* const PWM_OCR[][3] PROGMEM = {

  #ifdef TIMER0A
    { &OCR0A, &OCR0B, 0 },
  #else
    { 0, &OCR0B, 0 },
  #endif

  #if defined(COM1C1) && defined(TIMER1C)
   { (const uint8_t*)&OCR1A, (const uint8_t*)&OCR1B, (const uint8_t*)&OCR1C },
  #else
   { (const uint8_t*)&OCR1A, (const uint8_t*)&OCR1B, 0 },
  #endif

  #if defined(TCCR2A) && defined(COM2A1)
    { &OCR2A, &OCR2B, 0 },
  #endif

  #if defined(TCCR3A) && defined(COM3A1)
    #ifdef COM3C1
      { (const uint8_t*)&OCR3A, (const uint8_t*)&OCR3B, (const uint8_t*)&OCR3C },
    #else
      { (const uint8_t*)&OCR3A, (const uint8_t*)&OCR3B, 0 },
    #endif
  #endif

  #ifdef TCCR4A
    { (const uint8_t*)&OCR4A, (const uint8_t*)&OCR4B, (const uint8_t*)&OCR4C },
  #endif

  #if defined(TCCR5A) && defined(COM5A1)
    { (const uint8_t*)&OCR5A, (const uint8_t*)&OCR5B, (const uint8_t*)&OCR5C },
  #endif
};


#define TCCR_A(T)   pgm_read_word(&PWM_other[T][0])
#define TCCR_B(T)   pgm_read_word(&PWM_other[T][1])
#define TIMSK(T)    pgm_read_word(&PWM_other[T][2])
#define CS_0       0
#define CS_1       1
#define CS_2       2
#define WGM_0      0
#define WGM_1      1
#define WGM_2      3
#define WGM_3      4
#define TOIE       0

#define OCR_VAL(T, L)   pgm_read_word(&PWM_OCR[T][L])

static void err_is_counter()     { SERIAL_PROTOCOLPGM("   non-standard PWM mode"); }
static void err_is_interrupt()   { SERIAL_PROTOCOLPGM("   compare interrupt enabled"); }
static void err_prob_interrupt() { SERIAL_PROTOCOLPGM("   overflow interrupt enabled"); }

#if AVR_ATmega2560_FAMILY || AVR_AT90USB1286_FAMILY
  static void print_is_also_tied() { SERIAL_PROTOCOLPGM(" is also tied to this pin"); SERIAL_PROTOCOL_SP(14); }
#endif

void com_print(uint8_t N, uint8_t Z) {
  const uint8_t *TCCRA = (uint8_t*)TCCR_A(N);
  SERIAL_PROTOCOLPGM("    COM");
  SERIAL_PROTOCOLCHAR(N + '0');
  switch (Z) {
    case 'A':
      SERIAL_PROTOCOLPAIR("A: ", ((*TCCRA & (_BV(7) | _BV(6))) >> 6));
      break;
    case 'B':
      SERIAL_PROTOCOLPAIR("B: ", ((*TCCRA & (_BV(5) | _BV(4))) >> 4));
      break;
    case 'C':
      SERIAL_PROTOCOLPAIR("C: ", ((*TCCRA & (_BV(3) | _BV(2))) >> 2));
      break;
  }
}

void timer_prefix(uint8_t T, char L, uint8_t N) {  // T - timer    L - pwm  N - WGM bit layout
  char buffer[20];   // for the sprintf statements
  const uint8_t *TCCRB = (uint8_t*)TCCR_B(T),
                *TCCRA = (uint8_t*)TCCR_A(T);
  uint8_t WGM = (((*TCCRB & _BV(WGM_2)) >> 1) | (*TCCRA & (_BV(WGM_0) | _BV(WGM_1))));
  if (N == 4) WGM |= ((*TCCRB & _BV(WGM_3)) >> 1);

  SERIAL_PROTOCOLPGM("    TIMER");
  SERIAL_PROTOCOLCHAR(T + '0');
  SERIAL_PROTOCOLCHAR(L);
  SERIAL_PROTOCOL_SP(3);

  if (N == 3) {
    const uint8_t *OCRVAL8 = (uint8_t*)OCR_VAL(T, L - 'A');
    PWM_PRINT(*OCRVAL8);
  }
  else {
    const uint16_t *OCRVAL16 = (uint16_t*)OCR_VAL(T, L - 'A');
    PWM_PRINT(*OCRVAL16);
  }
  SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
  com_print(T,L);
  SERIAL_PROTOCOLPAIR("    CS: ", (*TCCRB & (_BV(CS_0) | _BV(CS_1) | _BV(CS_2)) ));

  SERIAL_PROTOCOLPGM("    TCCR");
  SERIAL_PROTOCOLCHAR(T + '0');
  SERIAL_PROTOCOLPAIR("A: ", *TCCRA);

  SERIAL_PROTOCOLPGM("    TCCR");
  SERIAL_PROTOCOLCHAR(T + '0');
  SERIAL_PROTOCOLPAIR("B: ", *TCCRB);

  const uint8_t *TMSK = (uint8_t*)TIMSK(T);
  SERIAL_PROTOCOLPGM("    TIMSK");
  SERIAL_PROTOCOLCHAR(T + '0');
  SERIAL_PROTOCOLPAIR(": ", *TMSK);

  const uint8_t OCIE = L - 'A' + 1;
  if (N == 3) { if (WGM == 0 || WGM == 2 || WGM ==  4 || WGM ==  6) err_is_counter(); }
  else        { if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13) err_is_counter(); }
  if (TEST(*TMSK, OCIE)) err_is_interrupt();
  if (TEST(*TMSK, TOIE)) err_prob_interrupt();
}

static void pwm_details(uint8_t pin) {
  switch (digitalPinToTimer_DEBUG(pin)) {

    #if defined(TCCR0A) && defined(COM0A1)
      #ifdef TIMER0A
        #if !AVR_AT90USB1286_FAMILY  // not available in Teensyduino type IDEs
          case TIMER0A: timer_prefix(0, 'A', 3); break;
        #endif
      #endif
      case TIMER0B: timer_prefix(0, 'B', 3); break;
    #endif

    #if defined(TCCR1A) && defined(COM1A1)
      case TIMER1A: timer_prefix(1, 'A', 4); break;
      case TIMER1B: timer_prefix(1, 'B', 4); break;
      #if defined(COM1C1) && defined(TIMER1C)
        case TIMER1C: timer_prefix(1, 'C', 4); break;
      #endif
    #endif

    #if defined(TCCR2A) && defined(COM2A1)
      case TIMER2A: timer_prefix(2, 'A', 3); break;
      case TIMER2B: timer_prefix(2, 'B', 3); break;
    #endif

    #if defined(TCCR3A) && defined(COM3A1)
      case TIMER3A: timer_prefix(3, 'A', 4); break;
      case TIMER3B: timer_prefix(3, 'B', 4); break;
      #ifdef COM3C1
        case TIMER3C: timer_prefix(3, 'C', 4); break;
      #endif
    #endif

    #ifdef TCCR4A
      case TIMER4A: timer_prefix(4, 'A', 4); break;
      case TIMER4B: timer_prefix(4, 'B', 4); break;
      case TIMER4C: timer_prefix(4, 'C', 4); break;
    #endif

    #if defined(TCCR5A) && defined(COM5A1)
      case TIMER5A: timer_prefix(5, 'A', 4); break;
      case TIMER5B: timer_prefix(5, 'B', 4); break;
      case TIMER5C: timer_prefix(5, 'C', 4); break;
    #endif

    case NOT_ON_TIMER: break;

  }
  SERIAL_PROTOCOLPGM("  ");

  // on pins that have two PWMs, print info on second PWM
  #if AVR_ATmega2560_FAMILY || AVR_AT90USB1286_FAMILY
    // looking for port B7 - PWMs 0A and 1C
    if (digitalPinToPort_DEBUG(pin) == 'B' - 64 && 0x80 == digitalPinToBitMask_DEBUG(pin)) {
      #if !AVR_AT90USB1286_FAMILY
        SERIAL_PROTOCOLPGM("\n .");
        SERIAL_PROTOCOL_SP(18);
        SERIAL_PROTOCOLPGM("TIMER1C");
        print_is_also_tied();
        timer_prefix(1, 'C', 4);
      #else
        SERIAL_PROTOCOLPGM("\n .");
        SERIAL_PROTOCOL_SP(18);
        SERIAL_PROTOCOLPGM("TIMER0A");
        print_is_also_tied();
        timer_prefix(0, 'A', 3);
      #endif
    }
  #endif
} // pwm_details

#ifndef digitalRead_mod                   // Use Teensyduino's version of digitalRead - it doesn't disable the PWMs
  int digitalRead_mod(const int8_t pin) { // same as digitalRead except the PWM stop section has been removed
    const uint8_t port = digitalPinToPort_DEBUG(pin);
    return (port != NOT_A_PIN) && (*portInputRegister(port) & digitalPinToBitMask_DEBUG(pin)) ? HIGH : LOW;
  }
#endif

void print_port(int8_t pin) {   // print port number
  #ifdef digitalPinToPort_DEBUG
    uint8_t x;
    SERIAL_PROTOCOLPGM("  Port: ");
    #if AVR_AT90USB1286_FAMILY
      x = (pin == 46 || pin == 47) ? 'E' : digitalPinToPort_DEBUG(pin) + 64;
    #else
      x = digitalPinToPort_DEBUG(pin) + 64;
    #endif
    SERIAL_CHAR(x);

    #if AVR_AT90USB1286_FAMILY
      if (pin == 46)
        x = '2';
      else if (pin == 47)
        x = '3';
      else {
        uint8_t temp = digitalPinToBitMask_DEBUG(pin);
        for (x = '0'; x < '9' && temp != 1; x++) temp >>= 1;
      }
    #else
      uint8_t temp = digitalPinToBitMask_DEBUG(pin);
      for (x = '0'; x < '9' && temp != 1; x++) temp >>= 1;
    #endif
    SERIAL_CHAR(x);
  #else
    SERIAL_PROTOCOL_SP(10);
  #endif
}

static void print_input_or_output(const bool isout) {
  serialprintPGM(isout ? PSTR("Output = ") : PSTR("Input  = "));
}

// pretty report with PWM info
inline void report_pin_state_extended(int8_t pin, bool ignore, bool extended = false, const char *start_string = "") {
  uint8_t temp_char;
  char *name_mem_pointer, buffer[30];   // for the sprintf statements
  bool found = false, multi_name_pin = false;
  for (uint8_t x = 0; x < COUNT(pin_array); x++)  {    // scan entire array and report all instances of this pin
    if (pgm_read_byte(&pin_array[x].pin) == pin) {
      if (found) multi_name_pin = true;
      found = true;
      if (!multi_name_pin) {    // report digitial and analog pin number only on the first time through
        sprintf_P(buffer, PSTR("%sPIN: %3d "), start_string, pin);     // digital pin number
        SERIAL_ECHO(buffer);
        print_port(pin);
        if (IS_ANALOG(pin)) {
          sprintf_P(buffer, PSTR(" (A%2d)  "), int(pin - analogInputToDigitalPin(0)));    // analog pin number
          SERIAL_ECHO(buffer);
        }
        else SERIAL_ECHO_SP(8);   // add padding if not an analog pin
      }
      else {
        SERIAL_CHAR('.');
        SERIAL_ECHO_SP(26 + strlen(start_string));  // add padding if not the first instance found
      }
      name_mem_pointer = (char*)pgm_read_word(&pin_array[x].name);
      for (uint8_t y = 0; y < 28; y++) {                   // always print pin name
        temp_char = pgm_read_byte(name_mem_pointer + y);
        if (temp_char != 0)
          MYSERIAL.write(temp_char);
        else {
          for (uint8_t i = 0; i < 28 - y; i++) MYSERIAL.write(' ');
          break;
        }
      }
      if (extended) {
        if (pin_is_protected(pin) && !ignore)
          SERIAL_ECHOPGM("protected ");
        else {
          #if AVR_AT90USB1286_FAMILY //Teensy IDEs don't know about these pins so must use FASTIO
            if (pin == 46 || pin == 47) {
              if (pin == 46) {
                print_input_or_output(GET_OUTPUT(46));
                SERIAL_PROTOCOL(READ(46));
              }
              else if (pin == 47) {
                print_input_or_output(GET_OUTPUT(47));
                SERIAL_PROTOCOL(READ(47));
              }
            }
            else
          #endif
          {
            if (!(pgm_read_byte(&pin_array[x].is_digital))) {
              sprintf_P(buffer, PSTR("Analog in = %5d"), analogRead(pin - analogInputToDigitalPin(0)));
              SERIAL_ECHO(buffer);
            }
            else {

              if (!get_pinMode(pin)) {
                //pinMode(pin, INPUT_PULLUP);  // make sure input isn't floating - stopped doing this
                                               // because this could interfere with inductive/capacitive
                                               // sensors (high impedance voltage divider) and with PT100 amplifier
                print_input_or_output(false);
                SERIAL_PROTOCOL(digitalRead_mod(pin));
              }
              else if (pwm_status(pin)) {
                // do nothing
              }
              else {
                print_input_or_output(true);
                SERIAL_PROTOCOL(digitalRead_mod(pin));
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
    sprintf_P(buffer, PSTR("%sPIN: %3d "), start_string, pin);
    SERIAL_ECHO(buffer);
    print_port(pin);
    if (IS_ANALOG(pin)) {
      sprintf_P(buffer, PSTR(" (A%2d)  "), int(pin - analogInputToDigitalPin(0)));    // analog pin number
      SERIAL_ECHO(buffer);
    }
    else
      SERIAL_ECHO_SP(8);   // add padding if not an analog pin
    SERIAL_ECHOPGM("<unused/unknown>");
    if (extended) {
      #if AVR_AT90USB1286_FAMILY  //Teensy IDEs don't know about these pins so must use FASTIO
        if (pin == 46 || pin == 47) {
          SERIAL_PROTOCOL_SP(12);
          if (pin == 46) {
            print_input_or_output(GET_OUTPUT(46));
            SERIAL_PROTOCOL(READ(46));
          }
          else {
            print_input_or_output(GET_OUTPUT(47));
            SERIAL_PROTOCOL(READ(47));
          }
        }
        else
      #endif
      {
        if (get_pinMode(pin)) {
          SERIAL_PROTOCOL_SP(12);
          print_input_or_output(true);
          SERIAL_PROTOCOL(digitalRead_mod(pin));
        }
        else {
          if (IS_ANALOG(pin)) {
            sprintf_P(buffer, PSTR("   Analog in = %5d"), analogRead(pin - analogInputToDigitalPin(0)));
            SERIAL_ECHO(buffer);
            SERIAL_ECHOPGM("   ");
          }
          else
          SERIAL_ECHO_SP(12);   // add padding if not an analog pin

          print_input_or_output(false);
          SERIAL_PROTOCOL(digitalRead_mod(pin));
        }
        //if (!pwm_status(pin)) SERIAL_CHAR(' ');    // add padding if it's not a PWM pin
        if (extended) pwm_details(pin);  // report PWM capabilities only if doing an extended report
      }
    }
    SERIAL_EOL();
  }
}
