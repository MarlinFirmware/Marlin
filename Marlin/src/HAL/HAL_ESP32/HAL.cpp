/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if EITHER(EEPROM_SETTINGS, WEBSUPPORT)
  #include "spiffs.h"
#endif

#if ENABLED(WIFISUPPORT)
  #include <ESPAsyncWebServer.h>
  #include "wifi.h"
  #if ENABLED(OTASUPPORT)
    #include "ota.h"
  #endif
  #if ENABLED(WEBSUPPORT)
    #include "web.h"
  #endif
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
  i2s_init();
}

void HAL_init_board(void) {
  #if EITHER(EEPROM_SETTINGS, WEBSUPPORT)
    spiffs_init();
  #endif

  #if ENABLED(WIFISUPPORT)
    wifi_init();
    #if ENABLED(OTASUPPORT)
      OTA_init();
    #endif
    #if ENABLED(WEBSUPPORT)
      web_init();
    #endif
    server.begin();
  #endif
}

void HAL_idletask(void) {
  #if ENABLED(OTASUPPORT)
    OTA_handle();
  #endif
}

void HAL_clear_reset_source(void) { }

uint8_t HAL_get_reset_source(void) { return rtc_get_reset_reason(1); }

void _delay_ms(int delay_ms) { delay(delay_ms); }

// return free memory between end of heap (or end bss) and whatever is current
int freeMemory() { return ESP.getFreeHeap(); }

// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------
#define ADC1_CHANNEL(pin) ADC1_GPIO ## pin ## _CHANNEL

adc1_channel_t get_channel(int pin) {
  switch (pin) {
    case 39: return ADC1_CHANNEL(39);
    case 36: return ADC1_CHANNEL(36);
    case 35: return ADC1_CHANNEL(35);
    case 34: return ADC1_CHANNEL(34);
    case 33: return ADC1_CHANNEL(33);
    case 32: return ADC1_CHANNEL(32);
  }
  return ADC1_CHANNEL_MAX;
}

void HAL_adc_init() {
  // Configure ADC
  adc1_config_width(ADC_WIDTH_12Bit);

  // Configure channels only if used as (re-)configuring a pin for ADC that is used elsewhere might have adverse effects
  #if HAS_TEMP_ADC_0
    adc1_config_channel_atten(get_channel(TEMP_0_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_1
    adc1_config_channel_atten(get_channel(TEMP_1_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_2
    adc1_config_channel_atten(get_channel(TEMP_2_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_3
    adc1_config_channel_atten(get_channel(TEMP_3_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_4
    adc1_config_channel_atten(get_channel(TEMP_4_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_5
    adc1_config_channel_atten(get_channel(TEMP_5_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_HEATED_BED
    adc1_config_channel_atten(get_channel(TEMP_BED_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_CHAMBER
    adc1_config_channel_atten(get_channel(TEMP_CHAMBER_PIN), ADC_ATTEN_11db);
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    adc1_config_channel_atten(get_channel(FILWIDTH_PIN), ADC_ATTEN_11db);
  #endif

  // Note that adc2 is shared with the WiFi module, which has higher priority, so the conversion may fail.
  // That's why we're not setting it up here.

  // Calculate ADC characteristics i.e. gain and offset factors
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, V_REF, &characteristics);
}

void HAL_adc_start_conversion(uint8_t adc_pin) {
  uint32_t mv;
  esp_adc_cal_get_voltage((adc_channel_t)get_channel(adc_pin), &characteristics, &mv);

  HAL_adc_result = mv*1023.0/3300.0;
}

void analogWrite(int pin, int value) {

  if (!PWM_PIN(pin)) return;

  static int cnt_channel = 1,
             pin_to_channel[40] = {};
  if (pin_to_channel[pin] == 0) {
    ledcAttachPin(pin, cnt_channel);
    ledcSetup(cnt_channel, 490, 8);
    ledcWrite(cnt_channel, value);

    pin_to_channel[pin] = cnt_channel++;
  }

  ledcWrite(pin_to_channel[pin], value);
}

#endif // ARDUINO_ARCH_ESP32
