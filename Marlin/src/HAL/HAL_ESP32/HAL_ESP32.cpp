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

#ifdef ARDUINO_ARCH_ESP32

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../HAL.h"
#include <rom/rtc.h>

// --------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

int HAL_adc_result;

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_clear_reset_source(void) { }

uint8_t HAL_get_reset_source (void) {
  return rtc_get_reset_reason(1);
}

void _delay_ms(int delay_ms) {
	delay(delay_ms);
}

// return free memory between end of heap (or end bss) and whatever is current
int freeMemory() {
  return ESP.getFreeHeap();
}

// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------

void HAL_adc_init() {
  analogReadResolution(10); // ESP32 has 12bits ADC but current tables are designed for 10 bits.
}

void HAL_adc_start_conversion (uint8_t adc_pin) {
  int r = analogRead(adc_pin);
  Serial.printf("adc_pint %d adc results %d\n", adc_pin, r);
  HAL_adc_result = r >> 2;
}

#endif // ARDUINO_ARCH_ESP32
