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

// Interrupts
void cli(void) { __disable_irq(); } // Disable
void sei(void) { __enable_irq(); }  // Enable

// Program Memory
void serialprintPGM(const char * str){
  usb_serial.print(str);
}

// Time functions
void _delay_ms(int delay_ms) {
  delay (delay_ms);
}

uint32_t millis() {
  return _millis;
}

//todo: recheck all of this
void delayMicroseconds(uint32_t us) {
  if (us < 2) return; // function jump, compare, return about 1us
  us--;
  static const int nop_factor = (SystemCoreClock / 10000000); // measured accurate at 10us
  static volatile int loops = 0;
  if (us < 20) { // burn cycles
    loops = us * nop_factor;
    while (loops > 0) --loops;
  }
  else { // poll systick
    int32_t start = SysTick->VAL;
    int32_t load = SysTick->LOAD;
    int32_t end = start - (load / 1000) * us;

    if (end >> 31)
      while (!(SysTick->VAL > start && SysTick->VAL < (load + end))) __NOP();
    else
      while (SysTick->VAL > end) __NOP();
  }
}

extern "C" void delay(int msec) {
   volatile int32_t end = _millis + msec;
   while (_millis < end) __WFE();
}

// IO functions
// As defined by Arduino INPUT(0x0), OUPUT(0x1), INPUT_PULLUP(0x2)
void pinMode(int pin, int mode) {
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

void digitalWrite(int pin, int pin_status) {
  if (!WITHIN(pin, 0, NUM_DIGITAL_PINS - 1) || pin_map[pin].port == 0xFF)
    return;

  if (pin_status)
    LPC_GPIO(pin_map[pin].port)->FIOSET = LPC_PIN(pin_map[pin].pin);
  else
    LPC_GPIO(pin_map[pin].port)->FIOCLR = LPC_PIN(pin_map[pin].pin);
}

bool digitalRead(int pin) {
  if (!WITHIN(pin, 0, NUM_DIGITAL_PINS - 1) || pin_map[pin].port == 0xFF) {
    return false;
  }
  return LPC_GPIO(pin_map[pin].port)->FIOPIN & LPC_PIN(pin_map[pin].pin) ? 1 : 0;
}

void analogWrite(int pin, int pin_status) { //todo: Hardware PWM
  /*
  if (pin == P2_4) {
    LPC_PWM1->MR5 = pin_status; // set value
    LPC_PWM1->LER = _BV(5); // set latch
  }
  else if (pin == P2_5) {
    LPC_PWM1->MR6 = pin_status;
    LPC_PWM1->LER = _BV(6);
  }
  */
}

extern bool HAL_adc_finished();

uint16_t analogRead(int adc_pin) {
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

/***/
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

#endif // TARGET_LPC1768
