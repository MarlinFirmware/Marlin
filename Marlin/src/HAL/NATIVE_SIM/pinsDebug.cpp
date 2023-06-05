/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef __PLAT_NATIVE_SIM__

#include "../../inc/MarlinConfig.h"
#include "pinsDebug.h"

int8_t ADC_pin_mode(pin_t pin) { return -1; }

int8_t get_pin_mode(const pin_t pin) { return VALID_PIN(pin) ? 0 : -1; }

bool GET_PINMODE(const pin_t pin) {
  const int8_t pin_mode = get_pin_mode(pin);
  if (pin_mode == -1 || pin_mode == ADC_pin_mode(pin)) // Invalid pin or active analog pin
    return false;

  return (Gpio::getMode(pin) != 0); // Input/output state
}

bool GET_ARRAY_IS_DIGITAL(const pin_t pin) {
  return !IS_ANALOG(pin) || get_pin_mode(pin) != ADC_pin_mode(pin);
}

void print_port(const pin_t) {}
void pwm_details(const pin_t) {}
bool pwm_status(const pin_t) { return false; }

#endif
