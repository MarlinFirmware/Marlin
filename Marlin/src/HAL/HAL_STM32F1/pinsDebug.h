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
 * Support routines for STM32F1
 */
#ifdef __STM32F1__

/**
 * Translation of routines & variables used by pinsDebug.h
 */
#include "fastio_STM32F1.h"

extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS];

#define NUM_DIGITAL_PINS BOARD_NR_GPIO_PINS
#define NUMBER_PINS_TOTAL BOARD_NR_GPIO_PINS
#define VALID_PIN(pin) (pin >= 0 && pin < BOARD_NR_GPIO_PINS)
#define GET_ARRAY_PIN(p) pin_t(pin_array[p].pin)
#define pwm_status(pin) PWM_PIN(pin)
#define digitalRead_mod(p) extDigitalRead(p)
#define NAME_FORMAT(p) PSTR("%-##p##s")
#define PRINT_PIN(p) do{ sprintf_P(buffer, PSTR("%3hd "), int16_t(p)); SERIAL_ECHO(buffer); }while(0)
#define PRINT_PORT(p) print_port(p)
#define PRINT_ARRAY_NAME(x)  do {sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer);} while (0)
#define MULTI_NAME_PAD 20 // spaces needed to be pretty if not first name assigned to a pin

// pins that will cause hang/reset/disconnect in M43 Toggle and Watch utilities
#ifndef M43_NEVER_TOUCH
  #define M43_NEVER_TOUCH(Q) (Q >= 9 && Q <= 12) // SERIAL/USB pins PA9(TX) PA10(RX)
#endif

static inline int8_t get_pin_mode(pin_t pin) {
  return VALID_PIN(pin) ? _GET_MODE(pin) : -1;
}

static inline pin_t DIGITAL_PIN_TO_ANALOG_PIN(pin_t pin) {
  if (!VALID_PIN(pin)) return -1;
  int8_t adc_channel = int8_t(PIN_MAP[pin].adc_channel);
  #ifdef NUM_ANALOG_INPUTS
    if (adc_channel >= NUM_ANALOG_INPUTS) adc_channel = ADCx;
  #endif
  return pin_t(adc_channel);
}

static inline bool IS_ANALOG(pin_t pin) {
  if (!VALID_PIN(pin)) return false;
  if (PIN_MAP[pin].adc_channel != ADCx) {
    #ifdef NUM_ANALOG_INPUTS
      if (PIN_MAP[pin].adc_channel >= NUM_ANALOG_INPUTS) return false;
    #endif
    return _GET_MODE(pin) == GPIO_INPUT_ANALOG && !M43_NEVER_TOUCH(pin);
  }
  return false;
}

static inline bool GET_PINMODE(pin_t pin) {
  return VALID_PIN(pin) ? !IS_INPUT(pin) : false;
}

static inline bool GET_ARRAY_IS_DIGITAL(int16_t array_pin) {
  pin_t pin = GET_ARRAY_PIN(array_pin);
  bool isDigital = !IS_ANALOG(pin);
  #ifdef NUM_ANALOG_INPUTS
    if (!isDigital && PIN_MAP[pin].adc_channel >= NUM_ANALOG_INPUTS)
      isDigital = true;
  #endif
  return isDigital;
}

static inline void pwm_details(pin_t pin) {
  if (PWM_PIN(pin)) {
    char buffer[16], num = '?';
    timer_dev * const tdev = PIN_MAP[pin].timer_device;
    const uint8_t channel = PIN_MAP[pin].timer_channel;
    if (tdev == &timer1) num = '1';
    else if (tdev == &timer1) num = '1';
    else if (tdev == &timer2) num = '2';
    else if (tdev == &timer3) num = '3';
    else if (tdev == &timer4) num = '4';
    #ifdef STM32_HIGH_DENSITY
      else if (tdev == &timer5) num = '5';
      else if (tdev == &timer8) num = '8';
    #endif
    sprintf_P(buffer, PSTR(" TIM%c CH%c"), num, ('0' + channel));
    SERIAL_ECHO(buffer);
  }
}

static inline void print_port(pin_t pin) {
  char buffer[8];
  char port = 'A' + char(pin >> 4); // pin div 16
  /* seems not to be required for our devices
    gpio_dev* gp = PIN_MAP[pin].gpio_device;
    if (gp == &gpioa) port = 'A';
    else if (gp == &gpiob) port = 'B';
    else if (gp == &gpioc) port = 'C';
    else if (gp == &gpiod) port = 'D';
    #if STM32_NR_GPIO_PORTS > 4
      else if (gp == &gpioe) port = 'E';
      else if (gp == &gpiof) port = 'F';
      else if (gp == &gpiog) port = 'G';
    #endif
  */
  const int16_t gbit = PIN_MAP[pin].gpio_bit;
  sprintf_P(buffer, PSTR("P%c%hd "), port, gbit);
  if (gbit < 10) SERIAL_CHAR(' ');
  SERIAL_ECHO(buffer);
}

#endif // __STM32F1__
