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

#ifndef HAL_PINSDEBUG_AVR_H

#define IS_ANALOG(P) ( WITHIN(P, analogInputToDigitalPin(0), analogInputToDigitalPin(15)) || (P) <= analogInputToDigitalPin(5) )

void HAL_print_analog_pin(char buffer[], int8_t pin) {
  sprintf(buffer, "(A%2d)  ", int(pin - analogInputToDigitalPin(0)));
}

void HAL_analog_pin_state(char buffer[], int8_t pin) {
  sprintf(buffer, "Analog in =% 5d", analogRead(pin - analogInputToDigitalPin(0)));
}

#define PWM_PRINT(V) do{ sprintf(buffer, "PWM:  %4d", V); SERIAL_ECHO(buffer); }while(0)
#define PWM_CASE(N,Z) \
  case TIMER##N##Z: \
    if (TCCR##N##A & (_BV(COM##N##Z##1) | _BV(COM##N##Z##0))) { \
      PWM_PRINT(OCR##N##Z); \
      return true; \
    } else return false

/**
 * Print a pin's PWM status.
 * Return true if it's currently a PWM pin.
 */
bool HAL_pwm_status(int8_t pin) {
  char buffer[20];   // for the sprintf statements
  switch(digitalPinToTimer(pin)) {

    #if defined(TCCR0A) && defined(COM0A1)
      PWM_CASE(0,A);
      PWM_CASE(0,B);
    #endif

    #if defined(TCCR1A) && defined(COM1A1)
      PWM_CASE(1,A);
      PWM_CASE(1,B);
      #if defined(COM1C1) && defined(TIMER1C)
        PWM_CASE(1,C);
      #endif
    #endif

    #if defined(TCCR2A) && defined(COM2A1)
      PWM_CASE(2,A);
      PWM_CASE(2,B);
    #endif

    #if defined(TCCR3A) && defined(COM3A1)
      PWM_CASE(3,A);
      PWM_CASE(3,B);
      #ifdef COM3C1
        PWM_CASE(3,C);
      #endif
    #endif

    #ifdef TCCR4A
      PWM_CASE(4,A);
      PWM_CASE(4,B);
      PWM_CASE(4,C);
    #endif

    #if defined(TCCR5A) && defined(COM5A1)
      PWM_CASE(5,A);
      PWM_CASE(5,B);
      PWM_CASE(5,C);
    #endif

    case NOT_ON_TIMER:
    default:
      return false;
  }
  SERIAL_PROTOCOLPGM("  ");
}


static void err_is_counter() {
  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
}
static void err_is_interrupt() {
  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because it's being used as an interrupt");
}
static void err_prob_interrupt() {
  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer is being used as an interrupt");
}
static void can_be_used() { SERIAL_PROTOCOLPGM("   can be used as PWM   "); }

static void HAL_pwm_details(uint8_t pin) {

  uint8_t WGM;

  switch(digitalPinToTimer(pin)) {

    #if defined(TCCR0A) && defined(COM0A1)
      case TIMER0A:
        TIMER_PREFIX(0,A,3);
        if (WGM_TEST1) err_is_counter();
        else if (TEST(TIMSK0, OCIE0A)) err_is_interrupt();
        else if (TEST(TIMSK0, TOIE0)) err_prob_interrupt();
        else can_be_used();
        break;
      case TIMER0B:
        TIMER_PREFIX(0,B,3);
        if (WGM_TEST1) err_is_counter();
        else if (TEST(TIMSK0, OCIE0B)) err_is_interrupt();
        else if (TEST(TIMSK0, TOIE0)) err_prob_interrupt();
        else can_be_used();
        break;
    #endif

    #if defined(TCCR1A) && defined(COM1A1)
      case TIMER1A:
        TIMER_PREFIX(1,A,4);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK1, OCIE1A)) err_is_interrupt();
        else if (TIMSK1 & (_BV(TOIE1) | _BV(ICIE1))) err_prob_interrupt();
        else can_be_used();
        break;
      case TIMER1B:
        TIMER_PREFIX(1,B,4);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK1, OCIE1B)) err_is_interrupt();
        else if (TIMSK1 & (_BV(TOIE1) | _BV(ICIE1))) err_prob_interrupt();
        else can_be_used();
        break;
      #if defined(COM1C1) && defined(TIMER1C)
        case TIMER1C:
          TIMER_PREFIX(1,C,4);
          if (WGM_TEST2) err_is_counter();
          else if (TEST(TIMSK1, OCIE1C)) err_is_interrupt();
          else if (TIMSK1 & (_BV(TOIE1) | _BV(ICIE1))) err_prob_interrupt();
          else can_be_used();
          break;
      #endif
    #endif

    #if defined(TCCR2A) && defined(COM2A1)
      case TIMER2A:
        TIMER_PREFIX(2,A,3);
        if (WGM_TEST1) err_is_counter();
        else if (TIMSK2 & (_BV(TOIE2) | _BV(OCIE2A))) err_is_interrupt();
        else if (TEST(TIMSK2, TOIE2)) err_prob_interrupt();
        else can_be_used();
        break;
      case TIMER2B:
        TIMER_PREFIX(2,B,3);
        if (WGM_TEST1) err_is_counter();
        else if (TEST(TIMSK2, OCIE2B)) err_is_interrupt();
        else if (TEST(TIMSK2, TOIE2)) err_prob_interrupt();
        else can_be_used();
        break;
    #endif

    #if defined(TCCR3A) && defined(COM3A1)
      case TIMER3A:
        TIMER_PREFIX(3,A,3);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK3, OCIE3A)) err_is_interrupt();
        else if (TIMSK3 & (_BV(TOIE3) | _BV(ICIE3))) err_prob_interrupt();
        else can_be_used();
        break;
      case TIMER3B:
        TIMER_PREFIX(3,B,3);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK3, OCIE3B)) err_is_interrupt();
        else if (TIMSK3 & (_BV(TOIE3) | _BV(ICIE3))) err_prob_interrupt();
        else can_be_used();
        break;
      #ifdef COM3C1
      case TIMER3C:
        TIMER_PREFIX(3,C,3);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK3, OCIE3C)) err_is_interrupt();
        else if (TIMSK3 & (_BV(TOIE3) | _BV(ICIE3))) err_prob_interrupt();
        else can_be_used();
        break;
      #endif
    #endif

    #ifdef TCCR4A
      case TIMER4A:
        TIMER_PREFIX(4,A,4);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK4, OCIE4A)) err_is_interrupt();
        else if (TIMSK4 & (_BV(TOIE4) | _BV(ICIE4))) err_prob_interrupt();
        else can_be_used();
        break;
      case TIMER4B:
        TIMER_PREFIX(4,B,4);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK4, OCIE4B)) err_is_interrupt();
        else if (TIMSK4 & (_BV(TOIE4) | _BV(ICIE4))) err_prob_interrupt();
        else can_be_used();
        break;
      case TIMER4C:
        TIMER_PREFIX(4,C,4);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK4, OCIE4C)) err_is_interrupt();
        else if (TIMSK4 & (_BV(TOIE4) | _BV(ICIE4))) err_prob_interrupt();
        else can_be_used();
        break;
    #endif

    #if defined(TCCR5A) && defined(COM5A1)
      case TIMER5A:
        TIMER_PREFIX(5,A,4);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK5, OCIE5A)) err_is_interrupt();
        else if (TIMSK5 & (_BV(TOIE5) | _BV(ICIE5))) err_prob_interrupt();
        else can_be_used();
        break;
      case TIMER5B:
        TIMER_PREFIX(5,B,4);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK5, OCIE5B)) err_is_interrupt();
        else if (TIMSK5 & (_BV(TOIE5) | _BV(ICIE5))) err_prob_interrupt();
        else can_be_used();
        break;
      case TIMER5C:
        TIMER_PREFIX(5,C,4);
        if (WGM_TEST2) err_is_counter();
        else if (TEST(TIMSK5, OCIE5C)) err_is_interrupt();
        else if (TIMSK5 & (_BV(TOIE5) | _BV(ICIE5))) err_prob_interrupt();
        else can_be_used();
        break;
    #endif

    case NOT_ON_TIMER: break;

  }
  SERIAL_PROTOCOLPGM("  ");
} // pwm_details


#endif
