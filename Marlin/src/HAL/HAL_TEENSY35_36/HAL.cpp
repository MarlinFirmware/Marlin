/* **************************************************************************

 Marlin 3D Printer Firmware
 Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/


/**
 * Description: HAL for Teensy35 (MK64FX512)
 */

#if defined(__MK64FX512__) || defined(__MK66FX1M0__)

#include "HAL.h"
#include "../shared/Delay.h"

#include <Wire.h>

uint16_t HAL_adc_result;

static const uint8_t pin2sc1a[] = {
  5, 14, 8, 9, 13, 12, 6, 7, 15, 4, 3, 19+128, 14+128, 15+128, // 0-13 -> A0-A13
  5, 14, 8, 9, 13, 12, 6, 7, 15, 4, // 14-23 are A0-A9
  255, 255, 255, 255, 255, 255, 255, // 24-30 are digital only
  14+128, 15+128, 17, 18, 4+128, 5+128, 6+128, 7+128, 17+128,  // 31-39 are A12-A20
  255, 255, 255, 255, 255, 255, 255, 255, 255,  // 40-48 are digital only
  10+128, 11+128, // 49-50 are A23-A24
  255, 255, 255, 255, 255, 255, 255, // 51-57 are digital only
  255, 255, 255, 255, 255, 255, // 58-63 (sd card pins) are digital only
  3, 19+128, // 64-65 are A10-A11
  23, 23+128,// 66-67 are A21-A22 (DAC pins)
  1, 1+128,  // 68-69 are A25-A26 (unused USB host port on Teensy 3.5)
  26,        // 70 is Temperature Sensor
  18+128     // 71 is Vref
};

/*
  // disable interrupts
  void cli(void) { noInterrupts(); }

  // enable interrupts
  void sei(void) { interrupts(); }
*/

void HAL_adc_init() {
  analog_init();
  while (ADC0_SC3 & ADC_SC3_CAL) {}; // Wait for calibration to finish
  NVIC_ENABLE_IRQ(IRQ_FTM1);
}

void HAL_clear_reset_source(void) { }

uint8_t HAL_get_reset_source(void) {
  switch (RCM_SRS0) {
    case 128: return RST_POWER_ON; break;
    case 64: return RST_EXTERNAL; break;
    case 32: return RST_WATCHDOG; break;
    // case 8: return RST_LOSS_OF_LOCK; break;
    // case 4: return RST_LOSS_OF_CLOCK; break;
    // case 2: return RST_LOW_VOLTAGE; break;
  }
  return 0;
}

extern "C" {
  extern char __bss_end;
  extern char __heap_start;
  extern void* __brkval;

  int freeMemory() {
    int free_memory;
    if ((int)__brkval == 0)
      free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
      free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
  }
}

void HAL_adc_start_conversion(const uint8_t adc_pin) { ADC0_SC1A = pin2sc1a[adc_pin]; }

uint16_t HAL_adc_get_result(void) { return ADC0_RA; }

#endif // __MK64FX512__ || __MK66FX1M0__
