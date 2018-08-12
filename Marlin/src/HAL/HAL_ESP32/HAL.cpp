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

#include "HAL.h"
#include <rom/rtc.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(WIFISUPPORT)
  #include "ota.h"
#endif

// --------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------

portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

#define V_REF 1100

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

uint16_t HAL_adc_result;

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

esp_adc_cal_characteristics_t characteristics;

// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_init(void) {
  #if ENABLED(WIFISUPPORT)
    OTA_init();
  #endif
}

void HAL_idletask(void) {
  #if ENABLED(WIFISUPPORT)
    OTA_handle();
  #endif
}

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
#define ADC1_CHANNEL(pin) ADC1_GPIO##pin_CHANNEL

adc1_channel_t get_channel(int pin) {
  switch (pin) {
    case 36: return ADC1_GPIO36_CHANNEL;
    case 39: return ADC1_GPIO39_CHANNEL;
  }

  return ADC1_CHANNEL_MAX;
}

void HAL_adc_init() {
  // Configure ADC
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(get_channel(36), ADC_ATTEN_11db);
  adc1_config_channel_atten(get_channel(39), ADC_ATTEN_11db);

  // Calculate ADC characteristics i.e. gain and offset factors
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, V_REF, &characteristics);
}

void HAL_adc_start_conversion (uint8_t adc_pin) {
  uint32_t mv;
  esp_adc_cal_get_voltage((adc_channel_t)get_channel(adc_pin), &characteristics, &mv);

  HAL_adc_result = mv*1023.0/3300.0;
}

int pin_to_channel[40] = {};
int cnt_channel = 1;
void analogWrite(int pin, int value) {
  if (pin_to_channel[pin] == 0) {
    ledcAttachPin(pin, cnt_channel);
    ledcSetup(cnt_channel, 490, 8);
    ledcWrite(cnt_channel, value);

    pin_to_channel[pin] = cnt_channel++;
  }

  ledcWrite(pin_to_channel[pin], value);
}
#endif // ARDUINO_ARCH_ESP32
