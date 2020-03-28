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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Support routines for LPC1768
 */

/**
 * Translation of routines & variables used by pinsDebug.h
 */

#define NUMBER_PINS_TOTAL NUM_DIGITAL_PINS
#define pwm_details(pin) pin = pin    // do nothing  // print PWM details
#define pwm_status(pin) false //Print a pin's PWM status. Return true if it's currently a PWM pin.
#define IS_ANALOG(P) (DIGITAL_PIN_TO_ANALOG_PIN(P) >= 0 ? 1 : 0)
#define digitalRead_mod(p) extDigitalRead(p)
#define PRINT_PORT(p)
#define GET_ARRAY_PIN(p) pin_array[p].pin
#define PRINT_ARRAY_NAME(x) do{ sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); }while(0)
#define PRINT_PIN(p) do{ sprintf_P(buffer, PSTR("%d.%02d"), LPC176x::pin_port(p), LPC176x::pin_bit(p)); SERIAL_ECHO(buffer); }while(0)
#define MULTI_NAME_PAD 16 // space needed to be pretty if not first name assigned to a pin

// pins that will cause hang/reset/disconnect in M43 Toggle and Watch utilities
#ifndef M43_NEVER_TOUCH
  #define M43_NEVER_TOUCH(Q) ((Q) == P0_29 || (Q) == P0_30 || (Q) == P2_09)  // USB pins
#endif

bool GET_PINMODE(const pin_t pin) {
  if (!LPC176x::pin_is_valid(pin) || LPC176x::pin_adc_enabled(pin)) // found an invalid pin or active analog pin
    return false;

  return LPC176x::gpio_direction(pin);
}

bool GET_ARRAY_IS_DIGITAL(const pin_t pin) {
  return (!LPC176x::pin_has_adc(pin) || !LPC176x::pin_adc_enabled(pin));
}
