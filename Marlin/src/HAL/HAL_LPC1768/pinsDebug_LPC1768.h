/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// active ADC function/mode/code values for PINSEL registers
int8_t ADC_pin_mode(HAL_PIN_TYPE pin) {
  uint8_t pin_port = LPC1768_PIN_PORT(pin);
  uint8_t pin_port_pin = LPC1768_PIN_PIN(pin);
  return (pin_port == 0 && pin_port_pin == 2  ? 2 :
          pin_port == 0 && pin_port_pin == 3  ? 2 :
          pin_port == 0 && pin_port_pin == 23 ? 1 :
          pin_port == 0 && pin_port_pin == 24 ? 1 :
          pin_port == 0 && pin_port_pin == 25 ? 1 :
          pin_port == 0 && pin_port_pin == 26 ? 1 :
          pin_port == 1 && pin_port_pin == 30 ? 3 :
          pin_port == 1 && pin_port_pin == 31 ? 3 : -1);
}

int8_t get_pin_mode(HAL_PIN_TYPE pin) {
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

bool GET_PINMODE(HAL_PIN_TYPE pin) {
  int8_t pin_mode = get_pin_mode(pin);
  if (pin_mode == -1 || (pin_mode && pin_mode == ADC_pin_mode(pin))) // found an invalid pin or active analog pin
    return false;

  uint32_t * FIO_reg[5] PROGMEM = {(uint32_t*) 0x2009C000,(uint32_t*)  0x2009C020,(uint32_t*)  0x2009C040,(uint32_t*)  0x2009C060,(uint32_t*)  0x2009C080};
  return ((*FIO_reg[LPC1768_PIN_PORT(pin)] >> LPC1768_PIN_PIN(pin) & 1) != 0); //input/output state
}

bool GET_ARRAY_IS_DIGITAL(HAL_PIN_TYPE pin) {
  int8_t pin_mode = get_pin_mode(pin);
  return (pin_mode != -1 && (!get_pin_mode(pin) || pin_mode != ADC_pin_mode(pin)));
}

/**
 * translation of routines & variables used by pinsDebug.h
 */

#define pwm_details(pin) pin = pin    // do nothing  // print PWM details
#define pwm_status(pin) false //Print a pin's PWM status. Return true if it's currently a PWM pin.
#define IS_ANALOG(P) (DIGITAL_PIN_TO_ANALOG_PIN(P) >= 0 ? 1 : 0)
#define digitalRead_mod(p)  digitalRead(p)
#define digitalPinToPort_DEBUG(p)  0
#define digitalPinToBitMask_DEBUG(pin) 0
#define PRINT_PORT(p) SERIAL_ECHO_SP(10);
#define GET_ARRAY_PIN(p) pin_array[p].pin
#define NAME_FORMAT(p) PSTR("%-##p##s")
//  #define PRINT_ARRAY_NAME(x)  do {sprintf_P(buffer, NAME_FORMAT(MAX_NAME_LENGTH) , pin_array[x].name); SERIAL_ECHO(buffer);} while (0)
#define PRINT_ARRAY_NAME(x)  do {sprintf_P(buffer, PSTR("%-35s") , pin_array[x].name); SERIAL_ECHO(buffer);} while (0)

// Pin map for M43
const HAL_PIN_TYPE pin_map[] = {
  P0_0,
  P0_1,
  P0_2,
  P0_3,
  P0_4,
  P0_5,
  P0_6,
  P0_7,
  P0_8,
  P0_9,
  P0_10,
  P0_11,
  P0_12,
  P0_13,
  P0_14,
  P0_15,
  P0_16,
  P0_17,
  P0_18,
  P0_19,
  P0_20,
  P0_21,
  P0_22,
  P0_23,
  P0_24,
  P0_25,
  P0_26,
  P0_27,
  P0_28,
  P0_29,
  P0_30,
  P1_0,
  P1_1,
  P1_2,
  P1_3,
  P1_4,
  P1_5,
  P1_6,
  P1_7,
  P1_8,
  P1_9,
  P1_10,
  P1_11,
  P1_12,
  P1_13,
  P1_14,
  P1_15,
  P1_16,
  P1_17,
  P1_18,
  P1_19,
  P1_20,
  P1_21,
  P1_22,
  P1_23,
  P1_24,
  P1_25,
  P1_26,
  P1_27,
  P1_28,
  P1_29,
  P1_30,
  P1_31,
  P2_0,
  P2_1,
  P2_2,
  P2_3,
  P2_4,
  P2_5,
  P2_6,
  P2_7,
  P2_8,
  P2_9,
  P2_10,
  P2_11,
  P2_12,
  P2_13,
  P3_25,
  P3_26,
  P4_28,
  P4_29
};

#define NUM_DIGITAL_PINS COUNT(pin_map)

#define GET_PIN_MAP_PIN(index) pin_map[index]