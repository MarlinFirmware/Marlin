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
 * HAL for Teensy 3.5 (MK64FX512) and Teensy 3.6 (MK66FX1M0)
 */

#if defined(__MK64FX512__) || defined(__MK66FX1M0__)

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
#endif

USBSerialType USBSerial(false, SerialUSB);

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

// ------------------------
// Watchdog Timer
// ------------------------

#if ENABLED(USE_WATCHDOG)

  #define WDT_TIMEOUT_MS TERN(WATCHDOG_DURATION_8S, 8000, 4000) // 4 or 8 second timeout

  void MarlinHAL::watchdog_init() {
    WDOG_TOVALH = 0;
    WDOG_TOVALL = WDT_TIMEOUT_MS;
    WDOG_STCTRLH = WDOG_STCTRLH_WDOGEN;
  }

  void MarlinHAL::watchdog_refresh() {
    // Watchdog refresh sequence
    WDOG_REFRESH = 0xA602;
    WDOG_REFRESH = 0xB480;
  }

#endif

// ------------------------
// ADC
// ------------------------

int8_t MarlinHAL::adc_select;

void MarlinHAL::adc_init() {
  analog_init();
  while (ADC0_SC3 & ADC_SC3_CAL) { /* Wait for calibration to finish */ }
  while (ADC1_SC3 & ADC_SC3_CAL) { /* Wait for calibration to finish */ }
  NVIC_ENABLE_IRQ(IRQ_FTM1);
}

void MarlinHAL::adc_start(const pin_t adc_pin) {
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
  const uint16_t pin = pin2sc1a[adc_pin];
  if (pin == 0xFF) {
    adc_select = -1;    // Digital only
  }
  else if (pin & 0x80) {
    adc_select = 1;
    ADC1_SC1A = pin & 0x7F;
  }
  else {
    adc_select = 0;
    ADC0_SC1A = pin;
  }
}

uint16_t MarlinHAL::adc_value() {
  switch (adc_select) {
    case 0: return ADC0_RA;
    case 1: return ADC1_RA;
  }
  return 0;
}

// ------------------------
// Free Memory Accessor
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

#endif // __MK64FX512__ || __MK66FX1M0__
