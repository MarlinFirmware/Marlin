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
 * HAL for Teensy 3.2 (MK20DX256)
 */

#ifdef __MK20DX256__

#include "HAL.h"
#include "../shared/Delay.h"

#include <Wire.h>

// ------------------------
// Serial ports
// ------------------------

#define _IMPLEMENT_SERIAL(X) DefaultSerial##X MSerial##X(false, Serial##X)
#define IMPLEMENT_SERIAL(X)  _IMPLEMENT_SERIAL(X)
#if WITHIN(SERIAL_PORT, 0, 3)
  IMPLEMENT_SERIAL(SERIAL_PORT);
#else
  #error "SERIAL_PORT must be from 0 to 3."
#endif
USBSerialType USBSerial(false, SerialUSB);

// ------------------------
// Class Utilities
// ------------------------

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

// ------------------------
// MarlinHAL Class
// ------------------------

void MarlinHAL::reboot() { _reboot_Teensyduino_(); }

uint8_t MarlinHAL::get_reset_source() {
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

// ADC

void MarlinHAL::adc_init() {
  analog_init();
  while (ADC0_SC3 & ADC_SC3_CAL) {}; // Wait for calibration to finish
  NVIC_ENABLE_IRQ(IRQ_FTM1);
}

void MarlinHAL::adc_start(const pin_t pin) {
  static const uint8_t pin2sc1a[] = {
      5, 14, 8, 9, 13, 12, 6, 7, 15, 4, 0, 19, 3, 31, // 0-13, we treat them as A0-A13
      5, 14, 8, 9, 13, 12, 6, 7, 15, 4, // 14-23 (A0-A9)
      31, 31, 31, 31, 31, 31, 31, 31, 31, 31, // 24-33
      0+64, 19+64, 3+64, 31+64, // 34-37 (A10-A13)
      26, 22, 23, 27, 29, 30 // 38-43: temp. sensor, VREF_OUT, A14, bandgap, VREFH, VREFL. A14 isn't connected to anything in Teensy 3.0.
  };
  ADC0_SC1A = pin2sc1a[pin];
}

uint16_t MarlinHAL::adc_value() { return ADC0_RA; }

#endif // __MK20DX256__
