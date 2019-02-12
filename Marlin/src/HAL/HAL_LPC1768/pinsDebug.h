/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#define digitalRead_mod(p)  digitalRead(p)
#define PRINT_PORT(p)
#define GET_ARRAY_PIN(p) pin_array[p].pin
#define NAME_FORMAT(p) PSTR("%-##p##s")
#define PRINT_ARRAY_NAME(x)  do {sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer);} while (0)
#define PRINT_PIN(p) do {sprintf_P(buffer, PSTR("%d.%02d"), LPC1768_PIN_PORT(p), LPC1768_PIN_PIN(p)); SERIAL_ECHO(buffer);} while (0)
#define MULTI_NAME_PAD 16 // space needed to be pretty if not first name assigned to a pin

// active ADC function/mode/code values for PINSEL registers
constexpr int8_t ADC_pin_mode(pin_t pin) {
  return (LPC1768_PIN_PORT(pin) == 0 && LPC1768_PIN_PIN(pin) == 2  ? 2 :
          LPC1768_PIN_PORT(pin) == 0 && LPC1768_PIN_PIN(pin) == 3  ? 2 :
          LPC1768_PIN_PORT(pin) == 0 && LPC1768_PIN_PIN(pin) == 23 ? 1 :
          LPC1768_PIN_PORT(pin) == 0 && LPC1768_PIN_PIN(pin) == 24 ? 1 :
          LPC1768_PIN_PORT(pin) == 0 && LPC1768_PIN_PIN(pin) == 25 ? 1 :
          LPC1768_PIN_PORT(pin) == 0 && LPC1768_PIN_PIN(pin) == 26 ? 1 :
          LPC1768_PIN_PORT(pin) == 1 && LPC1768_PIN_PIN(pin) == 30 ? 3 :
          LPC1768_PIN_PORT(pin) == 1 && LPC1768_PIN_PIN(pin) == 31 ? 3 : -1);
}

int8_t get_pin_mode(pin_t pin) {
  if (!VALID_PIN(pin)) return -1;
  uint8_t pin_port = LPC1768_PIN_PORT(pin);
  uint8_t pin_port_pin = LPC1768_PIN_PIN(pin);
  //get appropriate PINSEL register
  volatile uint32_t * pinsel_reg = (pin_port == 0 && pin_port_pin <= 15) ? &LPC_PINCON->PINSEL0 :
                                   (pin_port == 0)                       ? &LPC_PINCON->PINSEL1 :
                                   (pin_port == 1 && pin_port_pin <= 15) ? &LPC_PINCON->PINSEL2 :
                                    pin_port == 1                        ? &LPC_PINCON->PINSEL3 :
                                    pin_port == 2                        ? &LPC_PINCON->PINSEL4 :
                                    pin_port == 3                        ? &LPC_PINCON->PINSEL7 : &LPC_PINCON->PINSEL9;
  uint8_t pinsel_start_bit = pin_port_pin > 15 ? 2 * (pin_port_pin - 16) : 2 * pin_port_pin;
  int8_t pin_mode = (int8_t) ((*pinsel_reg >> pinsel_start_bit) & 0x3);
  return pin_mode;
}

bool GET_PINMODE(pin_t pin) {
  int8_t pin_mode = get_pin_mode(pin);
  if (pin_mode == -1 || pin_mode == ADC_pin_mode(pin)) // found an invalid pin or active analog pin
    return false;

  uint32_t * FIO_reg[5] PROGMEM = {(uint32_t*) 0x2009C000,(uint32_t*)  0x2009C020,(uint32_t*)  0x2009C040,(uint32_t*)  0x2009C060,(uint32_t*)  0x2009C080};
  return ((*FIO_reg[LPC1768_PIN_PORT(pin)] >> LPC1768_PIN_PIN(pin) & 1) != 0); //input/output state
}

bool GET_ARRAY_IS_DIGITAL(pin_t pin) {
  return (!IS_ANALOG(pin) || get_pin_mode(pin) != ADC_pin_mode(pin));
}
