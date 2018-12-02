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

#ifndef HAL_PINSDEBUG_TEENSY_H

#define NUMBER_PINS_TOTAL NUM_DIGITAL_PINS
#define MULTI_NAME_PAD 16 // space needed to be pretty if not first name assigned to a pin

#define FTM0_CH0_PIN 22
#define FTM0_CH1_PIN 23
#define FTM0_CH2_PIN  9
#define FTM0_CH3_PIN 10
#define FTM0_CH4_PIN  6
#define FTM0_CH5_PIN 20
#define FTM0_CH6_PIN 21
#define FTM0_CH7_PIN  5
#define FTM1_CH0_PIN  3
#define FTM1_CH1_PIN  4
#define FTM2_CH0_PIN 29
#define FTM2_CH1_PIN 30
#define FTM3_CH0_PIN  2
#define FTM3_CH1_PIN 14
#define FTM3_CH2_PIN  7
#define FTM3_CH3_PIN  8
#define FTM3_CH4_PIN 35
#define FTM3_CH5_PIN 36
#define FTM3_CH6_PIN 37
#define FTM3_CH7_PIN 38
#ifdef __MK66FX1M0__ // Teensy3.6
  #define TPM1_CH0_PIN 16
  #define TPM1_CH1_PIN 17
#endif

#define IS_ANALOG(P) ((P) >= analogInputToDigitalPin(0) && (P) <= analogInputToDigitalPin(9)) || ((P) >= analogInputToDigitalPin(12) && (P) <= analogInputToDigitalPin(20))

void HAL_print_analog_pin(char buffer[], int8_t pin) {
  if (pin <= 23)      sprintf(buffer, "(A%2d)  ", int(pin - 14));
  else if (pin <= 39) sprintf(buffer, "(A%2d)  ", int(pin - 19));
}

void HAL_analog_pin_state(char buffer[], int8_t pin) {
  if (pin <= 23)      sprintf(buffer, "Analog in =% 5d", analogRead(pin - 14));
  else if (pin <= 39) sprintf(buffer, "Analog in =% 5d", analogRead(pin - 19));
}

#define PWM_PRINT(V) do{ sprintf(buffer, "PWM:  %4d", 22); SERIAL_ECHO(buffer); }while(0)
#define FTM_CASE(N,Z) \
  case FTM##N##_CH##Z##_PIN: \
    if (FTM##N##_C##Z##V) { \
      PWM_PRINT(FTM##N##_C##Z##V); \
      return true; \
    } else return false

/**
 * Print a pin's PWM status.
 * Return true if it's currently a PWM pin.
 */
bool HAL_pwm_status(int8_t pin) {
  char buffer[20];   // for the sprintf statements
  switch(pin) {
    FTM_CASE(0,0);
    FTM_CASE(0,1);
    FTM_CASE(0,2);
    FTM_CASE(0,3);
    FTM_CASE(0,4);
    FTM_CASE(0,5);
    FTM_CASE(0,6);
    FTM_CASE(0,7);
    FTM_CASE(1,0);
    FTM_CASE(1,1);
    FTM_CASE(2,0);
    FTM_CASE(2,1);
    FTM_CASE(3,0);
    FTM_CASE(3,1);
    FTM_CASE(3,2);
    FTM_CASE(3,3);
    FTM_CASE(3,4);
    FTM_CASE(3,5);
    FTM_CASE(3,6);
    FTM_CASE(3,7);

    case NOT_ON_TIMER:
    default:
      return false;
  }
  SERIAL_ECHOPGM("  ");
}

static void HAL_pwm_details(uint8_t pin) { /* TODO */ }

#endif
