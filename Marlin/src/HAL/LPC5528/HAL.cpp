/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../platforms.h"

#ifdef TARGET_LPC5528

//#include "HAL.h"
//#include "usb_serial.h"

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"
#include "fastio.h"

#ifdef USBCON
  DefaultSerial1 MSerial0(false, UsbSerial);
#endif

int freeMemory() {
  volatile char top;
  return &top - reinterpret_cast<char*>(_sbrk(0));
}

// ------------------------
// Public Variables
// ------------------------

uint16_t MarlinHAL::adc_result;
pin_t MarlinHAL::adc_pin = 0;

// ------------------------
// Public functions
// ------------------------

TERN_(POSTMORTEM_DEBUGGING, extern void install_min_serial());

void MarlinHAL::init() {
  constexpr int cpuFreq = F_CPU;
  UNUSED(cpuFreq);
  //SetTimerInterruptPriorities();
  //i2s_init(12);
  HAL_timer_init();
  //timer_start(STEP_TIMER_NUM,2000000);
}

// ------------------------
// ADC
// ------------------------

static void set_adc_init(uint8_t adc_pin) {
  pinMode(adc_pin, INPUT);
  adc_init(adc_pin);
}

void MarlinHAL::adc_init() {
  TERN_(HAS_TEMP_ADC_0, set_adc_init(TEMP_0_PIN));
  TERN_(HAS_TEMP_ADC_1, set_adc_init(TEMP_1_PIN));
  TERN_(HAS_HEATED_BED, set_adc_init(TEMP_BED_PIN));
}

void MarlinHAL::adc_start(const pin_t adc_pin) {
  adc_result = analogRead(adc_pin);
}

// HAL idle task
void MarlinHAL::idletask() { }

void MarlinHAL::clear_reset_source() {
  TERN_(USE_WATCHDOG, watchdog_clear_timeout_flag());
}

uint8_t MarlinHAL::get_reset_source() {
  #if ENABLED(USE_WATCHDOG)
    if (watchdog_timed_out()) return RST_WATCHDOG;
  #endif
  return RST_POWER_ON;
}

#if ENABLED(USE_WATCHDOG)

  // TODO: Add watchdog support.

  //#include <lpc55xx_wdt.h>
  //#define WDT_TIMEOUT_US TERN(WATCHDOG_DURATION_8S, 8000000, 4000000) // 4 or 8 second timeout

  void MarlinHAL::watchdog_refresh() {
    WWDT_Refresh(WWDT);
    #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
      TOGGLE(LED_PIN);  // heartbeat indicator
    #endif
  }

#endif // USE_WATCHDOG

void MarlinHAL::reboot() { NVIC_SystemReset(); }

void flashFirmware(const int16_t) {}

// Maple Compatibility
volatile uint32_t systick_uptime_millis = 0;
systickCallback_t systick_user_callback;
void systick_attach_callback(systickCallback_t cb) { systick_user_callback = cb; }
void SYSTICK_Callback() {
  systick_uptime_millis++;
  if (systick_user_callback) systick_user_callback();
}

#endif // TARGET_LPC5528
