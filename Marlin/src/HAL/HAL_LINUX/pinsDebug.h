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

/**
 * Support routines for X86_64
 */

/**
 * Translation of routines & variables used by pinsDebug.h
 */

#define NUMBER_PINS_TOTAL NUM_DIGITAL_PINS
#define pwm_details(pin) pin = pin    // do nothing  // print PWM details
#define pwm_status(pin) false //Print a pin's PWM status. Return true if it's currently a PWM pin.
#define IS_ANALOG(P) (DIGITAL_PIN_TO_ANALOG_PIN(P) >= 0 ? 1 : 0)
#define digitalRead_mod(p)  digitalRead(p)
#define PRINT_PORT(p)
#define GET_ARRAY_PIN(p) pin_array[p].pin
#define NAME_FORMAT(p) PSTR("%-##p##s")
#define PRINT_ARRAY_NAME(x) do{ sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); }while(0)
#define PRINT_PIN(p) do{ sprintf_P(buffer, PSTR("%3d "), p); SERIAL_ECHO(buffer); }while(0)
#define MULTI_NAME_PAD 16 // space needed to be pretty if not first name assigned to a pin

// active ADC function/mode/code values for PINSEL registers
constexpr int8_t ADC_pin_mode(pin_t pin) {
  return (-1);
}

int8_t get_pin_mode(pin_t pin) {
  if (!VALID_PIN(pin)) return -1;
  return 0;
}

bool GET_PINMODE(pin_t pin) {
  int8_t pin_mode = get_pin_mode(pin);
  if (pin_mode == -1 || pin_mode == ADC_pin_mode(pin)) // found an invalid pin or active analog pin
    return false;

  return (Gpio::getMode(pin) != 0); //input/output state
}

bool GET_ARRAY_IS_DIGITAL(pin_t pin) {
  return (!IS_ANALOG(pin) || get_pin_mode(pin) != ADC_pin_mode(pin));
}
