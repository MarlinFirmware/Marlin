/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 */
#pragma once

/**
 * SoC (System on Chip) temperature sensor support for AT32F403
 *
 * The AT32F403 has an internal temperature sensor accessible via ADC.
 * This provides a basic implementation using the AT32 ADC peripheral.
 */

#ifdef ARDUINO_ARCH_AT32

// AT32 internal temperature sensor is connected to ADC channel
// The exact channel depends on the board design - typically an unused ADC pin
#ifndef TEMP_SOC_PIN
  #define TEMP_SOC_PIN -1  // Disabled by default
#endif

// Convert ADC reading to temperature in degrees C
// AT32 internal temp sensor: ~1.43V at 25C, -2.6mV/degree C
// ADC clock: SYSCLK/6 = 40 MHz (per CrealityOfficial/K2_Series_Klipper src/at32/adc.c)
// ADC sample time: ADC_SAMPLETIME_239_5 (slowest, for thermistors)
#define TEMP_SOC_TO_C(ADC_RAW) (25.0f - ((3.3f * (ADC_RAW) / 4095.0f - 1.43f) / 0.0026f))

#endif // ARDUINO_ARCH_AT32
