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

#ifdef TARGET_LPC1768

#include <lpc17xx_pinsel.h>
#include "HAL.h"
#include "../../core/macros.h"
#include "../../core/types.h"

// Interrupts
void cli(void) { __disable_irq(); } // Disable
void sei(void) { __enable_irq(); }  // Enable

// Time functions
void _delay_ms(const int delay_ms) {
  delay(delay_ms);
}

uint32_t millis() {
  return _millis;
}

void delayMicroseconds(uint32_t us) {
  static const int nop_factor = (SystemCoreClock / 11000000);
  static volatile int loops = 0;

  //previous ops already burned most of 1us, burn the rest
  loops = nop_factor / 4; //measured at 1us
  while (loops > 0) --loops;

  if (us < 2) return;
  us--;

  //redirect to delay for large values, then set new delay to remainder
  if (us > 1000) {
    delay(us / 1000);
    us = us % 1000;
  }

  if (us < 5) { // burn cycles, time in interrupts will not be taken into account
    loops = us * nop_factor;
    while (loops > 0) --loops;
  }
  else { // poll systick, more accurate through interrupts
    uint32_t start = SysTick->VAL;
    uint32_t load = SysTick->LOAD;
    uint32_t end = start - (load / 1000) * us;

    if (end >> 31)
      while (!(SysTick->VAL > start && SysTick->VAL < (load + end))) __NOP();
    else
      while (SysTick->VAL > end) __NOP();
  }
}

extern "C" void delay(const int msec) {
  volatile millis_t end = _millis + msec;
  SysTick->VAL = SysTick->LOAD; // reset systick counter so next systick is in exactly 1ms
                                // this could extend the time between systicks by upto 1ms
  while PENDING(_millis, end) __WFE();
}

// IO functions
// As defined by Arduino INPUT(0x0), OUPUT(0x1), INPUT_PULLUP(0x2)
void pinMode(uint8_t pin, uint8_t mode) {
  if (!WITHIN(pin, 0, NUM_DIGITAL_PINS - 1) || pin_map[pin].port == 0xFF)
    return;

  PINSEL_CFG_Type config = { pin_map[pin].port,
                             pin_map[pin].pin,
                             PINSEL_FUNC_0,
                             PINSEL_PINMODE_TRISTATE,
                             PINSEL_PINMODE_NORMAL };
  switch(mode) {
  case INPUT:
    LPC_GPIO(pin_map[pin].port)->FIODIR &= ~LPC_PIN(pin_map[pin].pin);
    PINSEL_ConfigPin(&config);
    break;
  case OUTPUT:
    LPC_GPIO(pin_map[pin].port)->FIODIR |=  LPC_PIN(pin_map[pin].pin);
    PINSEL_ConfigPin(&config);
    break;
  case INPUT_PULLUP:
    LPC_GPIO(pin_map[pin].port)->FIODIR &= ~LPC_PIN(pin_map[pin].pin);
    config.Pinmode = PINSEL_PINMODE_PULLUP;
    PINSEL_ConfigPin(&config);
    break;
  default:
    break;
  }
}

void digitalWrite(uint8_t pin, uint8_t pin_status) {
  if (!WITHIN(pin, 0, NUM_DIGITAL_PINS - 1) || pin_map[pin].port == 0xFF)
    return;

  if (pin_status)
    LPC_GPIO(pin_map[pin].port)->FIOSET = LPC_PIN(pin_map[pin].pin);
  else
    LPC_GPIO(pin_map[pin].port)->FIOCLR = LPC_PIN(pin_map[pin].pin);

  pinMode(pin, OUTPUT);  // Set pin mode on every write (Arduino version does this)

    /**
     * Must be done AFTER the output state is set. Doing this before will cause a
     * 2uS glitch if writing a "1".
     *
     * When the Port Direction bit is written to a "1" the output is immediately set
     * to the value of the FIOPIN bit which is "0" because of power up defaults.
     */
}

bool digitalRead(uint8_t pin) {
  if (!WITHIN(pin, 0, NUM_DIGITAL_PINS - 1) || pin_map[pin].port == 0xFF) {
    return false;
  }
  return LPC_GPIO(pin_map[pin].port)->FIOPIN & LPC_PIN(pin_map[pin].pin) ? 1 : 0;
}

void analogWrite(uint8_t pin, int pwm_value) {  // 1 - 254: pwm_value, 0: LOW, 255: HIGH

  extern bool LPC1768_PWM_attach_pin(uint8_t, uint32_t, uint32_t, uint8_t);
  extern bool LPC1768_PWM_write(uint8_t, uint32_t);
  extern bool LPC1768_PWM_detach_pin(uint8_t);
  #define MR0_MARGIN 200       // if channel value too close to MR0 the system locks up

  static bool out_of_PWM_slots = false;

  if (!WITHIN(pin, 0, NUM_DIGITAL_PINS - 1) || pin_map[pin].port == 0xFF)
    return;

  uint value = MAX(MIN(pwm_value, 255), 0);
  if (value == 0 || value == 255) {  // treat as digital pin
    LPC1768_PWM_detach_pin(pin);    // turn off PWM
    digitalWrite(pin, value);
  }
  else {
    if (LPC1768_PWM_attach_pin(pin, 1, (LPC_PWM1->MR0 - MR0_MARGIN),  0xff))   // locks up if get too close to MR0 value
      LPC1768_PWM_write(pin, map(value, 1, 254, 1, (LPC_PWM1->MR0 - MR0_MARGIN)));  // map 1-254 onto PWM range
    else {                                                                 // out of PWM channels
      if (!out_of_PWM_slots) usb_serial.printf(".\nWARNING - OUT OF PWM CHANNELS\n.\n");  //only warn once
      out_of_PWM_slots = true;
      digitalWrite(pin, value);  // treat as a digital pin if out of channels
    }
  }
}

extern bool HAL_adc_finished();

uint16_t analogRead(uint8_t adc_pin) {
  HAL_adc_start_conversion(adc_pin);
  while (!HAL_adc_finished());  // Wait for conversion to finish
  return HAL_adc_get_result();
}

// **************************
// Persistent Config Storage
// **************************

void eeprom_write_byte(unsigned char *pos, unsigned char value) {

}

unsigned char eeprom_read_byte(uint8_t * pos) { return '\0'; }

void eeprom_read_block (void *__dst, const void *__src, size_t __n) { }

void eeprom_update_block (const void *__src, void *__dst, size_t __n) { }

char *dtostrf (double __val, signed char __width, unsigned char __prec, char *__s) {
  char format_string[20];
  snprintf(format_string, 20, "%%%d.%df", __width, __prec);
  sprintf(__s, format_string, __val);
  return __s;
}

int32_t random(int32_t max) {
  return rand() % max;
}

int32_t random(int32_t min, int32_t max) {
  return min + rand() % (max - min);
}

void randomSeed(uint32_t value) {
  srand(value);
}

int map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif // TARGET_LPC1768
