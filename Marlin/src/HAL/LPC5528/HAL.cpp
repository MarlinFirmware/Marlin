/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#ifdef TARGET_LPC5528

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"
#include "fastio.h"

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

  #include <fsl_wwdt.h>
  #include <fsl_clock.h>

  // WWDT clock source is FRO1M (1 MHz).
  // Timeout period = timeoutValue * 4 / WWDT_clock_Hz
  // For 4s timeout at 1 MHz: 4 * 1,000,000 / 4 = 1,000,000 counts
  // For 8s timeout at 1 MHz: 8 * 1,000,000 / 4 = 2,000,000 counts
  #define WDT_TIMEOUT_COUNT TERN(WATCHDOG_DURATION_8S, 2000000UL, 1000000UL)

  void MarlinHAL::watchdog_init() {
    CLOCK_EnableClock(kCLOCK_Wwdt);
    wwdt_config_t config;
    WWDT_GetDefaultConfig(&config);
    config.enableWatchdogReset   = true;      // Reset on timeout
    config.enableWatchdogProtect = false;     // Allow timeout updates
    config.windowValue           = 0xFFFFFFUL; // Windowing disabled
    config.timeoutValue          = WDT_TIMEOUT_COUNT;
    config.warningValue          = 0;
    config.clockFreq_Hz          = CLOCK_GetWdtClkFreq();
    WWDT_Init(WWDT, &config);
  }

  void MarlinHAL::watchdog_refresh() {
    WWDT_Refresh(WWDT);
    #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
      TOGGLE(LED_PIN);  // heartbeat indicator
    #endif
  }

  bool MarlinHAL::watchdog_timed_out() {
    return !!(WWDT_GetStatusFlags(WWDT) & kWWDT_TimeoutFlag);
  }

  void MarlinHAL::watchdog_clear_timeout_flag() {
    WWDT_ClearStatusFlags(WWDT, kWWDT_TimeoutFlag);
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
