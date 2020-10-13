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
 * HAL for Teensy 4.0 / 4.1 (IMXRT1062)
 */

#ifdef __IMXRT1062__

#include "HAL.h"
#include "../shared/Delay.h"
#include "timers.h"

#include <Wire.h>

uint16_t HAL_adc_result, HAL_adc_select;

static const uint8_t pin2sc1a[] = {
  0x07,  // 0/A0  AD_B1_02
  0x08,  // 1/A1  AD_B1_03
  0x0C,  // 2/A2  AD_B1_07
  0x0B,  // 3/A3  AD_B1_06
  0x06,  // 4/A4  AD_B1_01
  0x05,  // 5/A5  AD_B1_00
  0x0F,  // 6/A6  AD_B1_10
  0x00,  // 7/A7  AD_B1_11
  0x0D,  // 8/A8  AD_B1_08
  0x0E,  // 9/A9  AD_B1_09
  0x01,  // 24/A10 AD_B0_12
  0x02,  // 25/A11 AD_B0_13
  0x83,  // 26/A12 AD_B1_14 - only on ADC2, 3
  0x84,  // 27/A13 AD_B1_15 - only on ADC2, 4
  0x07,  // 14/A0  AD_B1_02
  0x08,  // 15/A1  AD_B1_03
  0x0C,  // 16/A2  AD_B1_07
  0x0B,  // 17/A3  AD_B1_06
  0x06,  // 18/A4  AD_B1_01
  0x05,  // 19/A5  AD_B1_00
  0x0F,  // 20/A6  AD_B1_10
  0x00,  // 21/A7  AD_B1_11
  0x0D,  // 22/A8  AD_B1_08
  0x0E,  // 23/A9  AD_B1_09
  0x01,  // 24/A10 AD_B0_12
  0x02,  // 25/A11 AD_B0_13
  0x83,  // 26/A12 AD_B1_14 - only on ADC2, 3
  0x84,  // 27/A13 AD_B1_15 - only on ADC2, 4
  #ifdef ARDUINO_TEENSY41
    0xFF,  // 28
    0xFF,  // 29
    0xFF,  // 30
    0xFF,  // 31
    0xFF,  // 32
    0xFF,  // 33
    0xFF,  // 34
    0xFF,  // 35
    0xFF,  // 36
    0xFF,  // 37
    0x81,  // 38/A14 AD_B1_12 - only on ADC2, 1
    0x82,  // 39/A15 AD_B1_13 - only on ADC2, 2
    0x09,  // 40/A16 AD_B1_04
    0x0A,  // 41/A17 AD_B1_05
  #endif
};

/*
// disable interrupts
void cli() { noInterrupts(); }

// enable interrupts
void sei() { interrupts(); }
*/

void HAL_adc_init() {
  analog_init();
  while (ADC1_GC & ADC_GC_CAL) ;
  while (ADC2_GC & ADC_GC_CAL) ;
}

void HAL_clear_reset_source() {
  uint32_t reset_source = SRC_SRSR;
  SRC_SRSR = reset_source;
 }

uint8_t HAL_get_reset_source() {
  switch (SRC_SRSR & 0xFF) {
    case 1: return RST_POWER_ON; break;
    case 2: return RST_SOFTWARE; break;
    case 4: return RST_EXTERNAL; break;
    // case 8: return RST_BROWN_OUT; break;
    case 16: return RST_WATCHDOG; break;
     case 64: return RST_JTAG; break;
    // case 128: return RST_OVERTEMP; break;
  }
  return 0;
}

#define __bss_end _ebss

extern "C" {
  extern char __bss_end;
  extern char __heap_start;
  extern void* __brkval;

  // Doesn't work on Teensy 4.x
  uint32_t freeMemory() {
    uint32_t free_memory;
    if ((uint32_t)__brkval == 0)
      free_memory = ((uint32_t)&free_memory) - ((uint32_t)&__bss_end);
    else
      free_memory = ((uint32_t)&free_memory) - ((uint32_t)__brkval);
    return free_memory;
  }
}

void HAL_adc_start_conversion(const uint8_t adc_pin) {
  const uint16_t pin = pin2sc1a[adc_pin];
  if (pin == 0xFF) {
    HAL_adc_select = -1; // Digital only
  }
  else if (pin & 0x80) {
    HAL_adc_select = 1;
    ADC2_HC0 = pin & 0x7F;
  }
  else {
    HAL_adc_select = 0;
    ADC1_HC0 = pin;
  }
}

uint16_t HAL_adc_get_result() {
  switch (HAL_adc_select) {
    case 0:
      while (!(ADC1_HS & ADC_HS_COCO0)) ; // wait
      return ADC1_R0;
    case 1:
      while (!(ADC2_HS & ADC_HS_COCO0)) ; // wait
      return ADC2_R0;
  }
  return 0;
}

bool is_output(uint8_t pin) {
  const struct digital_pin_bitband_and_config_table_struct *p;
  p = digital_pin_to_info_PGM + pin;
  return (*(p->reg + 1) & p->mask);
}

#endif // __IMXRT1062__
