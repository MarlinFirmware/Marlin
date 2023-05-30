/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 * Copyright (c) 2017 Victor Perez
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
#include "../platforms.h"

#ifdef HAS_LPC55XX

#include "HAL.h"
// #include "usb_serial.h"

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"
#include "fastio.h"

#ifdef USBCON
  DefaultSerial1 MSerial0(false, UsbSerial);
#endif

// ------------------------
// Public Variables
// ------------------------

uint16_t HAL_adc_result;

// ------------------------
// Public functions
// ------------------------

TERN_(POSTMORTEM_DEBUGGING, extern void install_min_serial());

void HAL_init(void) {
  constexpr int cpuFreq = F_CPU;
  UNUSED(cpuFreq);
  // SetTimerInterruptPriorities();
  // i2s_init(12);
  HAL_timer_init();
  // HAL_timer_start(STEP_TIMER_NUM,2000000);
}

// ------------------------
// ADC
// ------------------------

static void set_adc_init(uint8_t adc_pin) {
  pinMode(adc_pin, INPUT);
  adc_init(adc_pin);
}

void HAL_adc_init() { 
  TERN_(HAS_TEMP_ADC_0, set_adc_init(TEMP_0_PIN));
  TERN_(HAS_TEMP_ADC_1, set_adc_init(TEMP_1_PIN));
  TERN_(HAS_HEATED_BED, set_adc_init(TEMP_BED_PIN));
}

void HAL_adc_start_conversion(const uint8_t adc_pin) { 
  HAL_adc_result = analogRead(adc_pin); 
}


uint16_t HAL_adc_get_result() { 
  return HAL_adc_result; 
}

// HAL idle task
void HAL_idletask() { }

void HAL_clear_reset_source(void) {
  TERN_(USE_WATCHDOG, watchdog_clear_timeout_flag());
}

uint8_t HAL_get_reset_source(void) {
  #if ENABLED(USE_WATCHDOG)
    if (watchdog_timed_out()) return RST_WATCHDOG;
  #endif
  return RST_POWER_ON;
}

void HAL_reboot() { NVIC_SystemReset(); }

void flashFirmware(const int16_t) {
  
}


// Maple Compatibility
volatile uint32_t systick_uptime_millis = 0;
systickCallback_t systick_user_callback;
void systick_attach_callback(systickCallback_t cb) { systick_user_callback = cb; }
void HAL_SYSTICK_Callback() {
  systick_uptime_millis++;
  if (systick_user_callback) systick_user_callback();
}


#endif
