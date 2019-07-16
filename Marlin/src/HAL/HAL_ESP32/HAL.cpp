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

#include "HAL.h"
#include "HAL_timers_ESP32.h"
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

// ------------------------
// Externals
// ------------------------

portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

// ------------------------
// Local defines
// ------------------------

#define V_REF 1100

// ------------------------
// Public Variables
// ------------------------

uint16_t HAL_adc_result;

// ------------------------
// Private Variables
// ------------------------

esp_adc_cal_characteristics_t characteristics[ADC_ATTEN_MAX];
adc_atten_t attenuations[ADC1_CHANNEL_MAX] = {};
uint32_t thresholds[ADC_ATTEN_MAX];
volatile int numPWMUsed = 0,
             pwmPins[MAX_PWM_PINS],
             pwmValues[MAX_PWM_PINS];

// ------------------------
// Public functions
// ------------------------

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

// ------------------------
// ADC
// ------------------------
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

void adc1_set_attenuation(adc1_channel_t chan, adc_atten_t atten) {
  if (attenuations[chan] != atten) {
    adc1_config_channel_atten(chan, atten);
    attenuations[chan] = atten;
  }
}

void HAL_adc_init() {
  // Configure ADC
  adc1_config_width(ADC_WIDTH_12Bit);

  // Configure channels only if used as (re-)configuring a pin for ADC that is used elsewhere might have adverse effects
  #if HAS_TEMP_ADC_0
    adc1_set_attenuation(get_channel(TEMP_0_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_1
    adc1_set_attenuation(get_channel(TEMP_1_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_2
    adc1_set_attenuation(get_channel(TEMP_2_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_3
    adc1_set_attenuation(get_channel(TEMP_3_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_4
    adc1_set_attenuation(get_channel(TEMP_4_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_ADC_5
    adc1_set_attenuation(get_channel(TEMP_5_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_HEATED_BED
    adc1_set_attenuation(get_channel(TEMP_BED_PIN), ADC_ATTEN_11db);
  #endif
  #if HAS_TEMP_CHAMBER
    adc1_set_attenuation(get_channel(TEMP_CHAMBER_PIN), ADC_ATTEN_11db);
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    adc1_set_attenuation(get_channel(FILWIDTH_PIN), ADC_ATTEN_11db);
  #endif

  // Note that adc2 is shared with the WiFi module, which has higher priority, so the conversion may fail.
  // That's why we're not setting it up here.

  // Calculate ADC characteristics (i.e., gain and offset factors for each attenuation level)
  for (int i = 0; i < ADC_ATTEN_MAX; i++) {
    esp_adc_cal_characterize(ADC_UNIT_1, (adc_atten_t)i, ADC_WIDTH_BIT_12, V_REF, &characteristics[i]);
    
    // Change attenuation 100mV below the calibrated threshold
    thresholds[i] = esp_adc_cal_raw_to_voltage(4095, &characteristics[i]);
  }
}

void HAL_adc_start_conversion(uint8_t adc_pin) {
  const adc1_channel_t chan = get_channel(adc_pin);
  uint32_t mv;
  esp_adc_cal_get_voltage((adc_channel_t)chan, &characteristics[attenuations[chan]], &mv);

  // Change the attenuation level based on the new reading
  adc_atten_t atten;
  if (mv < thresholds[ADC_ATTEN_DB_0] - 100)
    adc1_set_attenuation(chan, ADC_ATTEN_DB_0);
  else if (mv > thresholds[ADC_ATTEN_DB_0] - 50 && mv < thresholds[ADC_ATTEN_DB_2_5] - 100)
    adc1_set_attenuation(chan, ADC_ATTEN_DB_2_5);
  else if (mv > thresholds[ADC_ATTEN_DB_2_5] - 50 && mv < thresholds[ADC_ATTEN_DB_6] - 100)
    adc1_set_attenuation(chan, ADC_ATTEN_DB_6);
  else if (mv > thresholds[ADC_ATTEN_DB_6] - 50)
    adc1_set_attenuation(chan, ADC_ATTEN_DB_11);

  HAL_adc_result = mv * 1023.0 / 3300.0;
}

void analogWrite(pin_t pin, int value) {
  // Use ledc hardware for internal pins
  if (pin < 34) {
    static int cnt_channel = 1, pin_to_channel[40] = { 0 };
    if (pin_to_channel[pin] == 0) {
      ledcAttachPin(pin, cnt_channel);
      ledcSetup(cnt_channel, 490, 8);
      ledcWrite(cnt_channel, value);
      pin_to_channel[pin] = cnt_channel++;
    }
    ledcWrite(pin_to_channel[pin], value);
    return;
  }

  int idx = -1;

  // Search Pin
  for (int i = 0; i < numPWMUsed; ++i)
    if (pwmPins[i] == pin) { idx = i; break; }

  // not found ?
  if (idx < 0) {
    // No slots remaining
    if (numPWMUsed >= MAX_PWM_PINS) return;

    // Take new slot for pin
    idx = numPWMUsed;
    pwmPins[idx] = pin;
    // Start timer on first use
    if (idx == 0) HAL_timer_start(PWM_TIMER_NUM, PWM_TIMER_FREQUENCY);

    ++numPWMUsed;
  }

  // Use 7bit internal value - add 1 to have 100% high at 255
  pwmValues[idx] = (value + 1) / 2;
}

// Handle PWM timer interrupt
HAL_PWM_TIMER_ISR() {
  HAL_timer_isr_prologue(PWM_TIMER_NUM);

  static uint8_t count = 0;

  for (int i = 0; i < numPWMUsed; ++i) {
    if (count == 0)                   // Start of interval
      WRITE(pwmPins[i], pwmValues[i] ? HIGH : LOW);
    else if (pwmValues[i] == count)   // End of duration
      WRITE(pwmPins[i], LOW);
  }

  // 128 for 7 Bit resolution
  count = (count + 1) & 0x7F;

  HAL_timer_isr_epilogue(PWM_TIMER_NUM);
}

#endif // ARDUINO_ARCH_ESP32
