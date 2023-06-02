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

/**
 * Support routines for X86_64
 */

/**
 * Translation of routines & variables used by pinsDebug.h
 */

#define NUMBER_PINS_TOTAL   NUM_DIGITAL_PINS
#define IS_ANALOG(P)        (DIGITAL_PIN_TO_ANALOG_PIN(P) >= 0 ? 1 : 0)
#define digitalRead_mod(p)  digitalRead(p)
#define GET_ARRAY_PIN(p)    pin_array[p].pin
#define PRINT_ARRAY_NAME(x) do{ sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); }while(0)
#define PRINT_PIN(p)        do{ sprintf_P(buffer, PSTR("%3d "), p); SERIAL_ECHO(buffer); }while(0)
#define PRINT_PIN_ANALOG(p) do{ sprintf_P(buffer, PSTR(" (A%2d)  "), DIGITAL_PIN_TO_ANALOG_PIN(pin)); SERIAL_ECHO(buffer); }while(0)
#define MULTI_NAME_PAD      16 // space needed to be pretty if not first name assigned to a pin

// active ADC function/mode/code values for PINSEL registers
constexpr int8_t ADC_pin_mode(pin_t pin) { return -1; }

int8_t get_pin_mode(const pin_t pin) { return VALID_PIN(pin) ? 0 : -1; }

bool GET_PINMODE(const pin_t pin) {
  const int8_t pin_mode = get_pin_mode(pin);
  if (pin_mode == -1 || pin_mode == ADC_pin_mode(pin)) // Invalid pin or active analog pin
    return false;

  return (Gpio::getMode(pin) != 0); // Input/output state
}

bool GET_ARRAY_IS_DIGITAL(const pin_t pin) {
  return (!IS_ANALOG(pin) || get_pin_mode(pin) != ADC_pin_mode(pin));
}

void pwm_details(const pin_t pin) {}
bool pwm_status(const pin_t) { return false; }

void print_port(const pin_t) {}
