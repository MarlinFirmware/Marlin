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

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"
#include "adc.h"
#include "clock.h"

/**
 * ADC clock. The datasheet caps ADC12B at 5MHz in 12-bit mode:
 *   ADCClock = MCK / ((PRESCAL + 1) * 2)
 *   96MHz / ((9 + 1) * 2) = 4.8MHz
 */
#define ADC_PRESCAL  9
#define ADC_CLOCK    ((F_CPU) / ((ADC_PRESCAL + 1) * 2))

// Startup time >= 20us: STARTUP = (t * ADCClock / 8) - 1
#define ADC_STARTUP  ((20UL * (ADC_CLOCK / 1000000UL) / 8UL) - 1UL)

// Sample & hold >= 600ns: SHTIM = (t * ADCClock) - 1
#define ADC_SHTIM    ((600UL * (ADC_CLOCK / 1000000UL) / 1000UL) - 1UL)

// The channel whose conversion is currently in flight
static int8_t adc_active_channel = -1;

int8_t sam3u_adc_channel(const pin_t pin) {
  switch (pin) {
    case PA22: return 0;
    case PA30: return 1;
    case PB3:  return 2;
    case PB4:  return 3;
    case PC15: return 4;
    case PC16: return 5;
    case PC17: return 6;
    case PC18: return 7;
    default:   return -1;
  }
}

void sam3u_adc_init() {
  pmc_enable_periph_clk(ID_ADC12B);

  ADC12B->ADC12B_CR = ADC12B_CR_SWRST;

  ADC12B->ADC12B_MR = ADC12B_MR_PRESCAL(ADC_PRESCAL)
                    | ADC12B_MR_STARTUP(ADC_STARTUP)
                    | ADC12B_MR_SHTIM(ADC_SHTIM);   // Software trigger, 12-bit, single-ended

  // Unity gain, no offset, single-ended. IBCTL 1 is the datasheet's
  // recommendation for ADC clocks in the 1-5MHz range.
  ADC12B->ADC12B_ACR = ADC12B_ACR_GAIN(0) | ADC12B_ACR_IBCTL(1);
}

void sam3u_adc_enable_pin(const pin_t pin) {
  const int8_t ch = sam3u_adc_channel(pin);
  if (ch < 0) return;

  // Hand the pad to the analog front end: disable the PIO and its pull-up so
  // the input floats to the sensor rather than being driven or biased.
  Pio * const port = SAM3U_PIO(pin);
  const uint32_t mask = SAM3U_MASK(pin);
  pmc_enable_periph_clk(SAM3U_PIO_ID(pin));
  port->PIO_PUDR = mask;
  port->PIO_ODR  = mask;
  port->PIO_PDR  = mask;
}

void sam3u_adc_start_pin(const pin_t pin) {
  const int8_t ch = sam3u_adc_channel(pin);
  if (ch < 0) { adc_active_channel = -1; return; }

  // Only one channel is enabled at a time so ADC12B_LCDR is unambiguous
  ADC12B->ADC12B_CHDR = 0xFF;
  ADC12B->ADC12B_CHER = 1UL << ch;
  adc_active_channel = ch;
  ADC12B->ADC12B_CR = ADC12B_CR_START;
}

bool sam3u_adc_ready() {
  return adc_active_channel < 0 || bool(ADC12B->ADC12B_SR & ADC12B_SR_DRDY);
}

uint16_t sam3u_adc_value() {
  if (adc_active_channel < 0) return 0;
  return uint16_t(ADC12B->ADC12B_LCDR & 0xFFF);
}

uint16_t sam3u_adc_read_pin(const pin_t pin) {
  if (sam3u_adc_channel(pin) < 0) return 0;
  sam3u_adc_start_pin(pin);
  // A conversion takes well under 10us; bail out rather than hang if the
  // converter is not running for some reason.
  uint32_t timeout = 100000;
  while (!(ADC12B->ADC12B_SR & ADC12B_SR_DRDY) && --timeout) { }
  return timeout ? sam3u_adc_value() : 0;
}

#endif // __SAM3U4E__
