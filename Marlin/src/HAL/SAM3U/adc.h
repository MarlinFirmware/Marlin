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
#pragma once

/**
 * 12-bit ADC (ADC12B) for Atmel SAM3U (AT91SAM3U4E)
 *
 * The SAM3U carries two independent converters: an 8-channel 12-bit ADC12B and
 * an 8-channel 10-bit ADC. Only ADC12B is used here - it is the one the 4pi
 * routes its thermistors and analog endstops to, and it is the more accurate
 * of the two.
 *
 * Channel-to-pin assignment is fixed in silicon:
 *
 *   AD12B0 = PA22   AD12B2 = PB3    AD12B4 = PC15   AD12B6 = PC17
 *   AD12B1 = PA30   AD12B3 = PB4    AD12B5 = PC16   AD12B7 = PC18
 *
 * Marlin passes pin numbers (not channel numbers) to the HAL ADC entry points,
 * so the mapping is resolved here.
 */

#include "include/pinmapping.h"

// Configure ADC12B. Called once from MarlinHAL::adc_init().
void sam3u_adc_init();

// Route a pin's ADC channel to the converter and take the pin away from the PIO
void sam3u_adc_enable_pin(const pin_t pin);

// Begin a conversion. Safe to call from an ISR.
void sam3u_adc_start_pin(const pin_t pin);

// True once the conversion started by sam3u_adc_start_pin() has a result
bool sam3u_adc_ready();

// The result of the last completed conversion (0..4095)
uint16_t sam3u_adc_value();

// Blocking convenience read, used by analogRead()
uint16_t sam3u_adc_read_pin(const pin_t pin);

// Silicon channel for a pin, or -1 if the pin has no ADC12B input
int8_t sam3u_adc_channel(const pin_t pin);
