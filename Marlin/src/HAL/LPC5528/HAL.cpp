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
#include <fsl_power.h>
#include "fastio.h"

extern "C" {
  void u8g_Delay(uint16_t val) { delay(val); }
  void U8g_delay(int msec) { u8g_Delay(uint16_t(msec)); }
  void u8g_MicroDelay() { DELAY_US(1); }
  void u8g_10MicroDelay() { DELAY_US(10); }
}

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
  HAL_timer_init();
  TERN_(POSTMORTEM_DEBUGGING, install_min_serial());  // Set up the serial output for the crash dump
}

// ------------------------
// ADC
// ------------------------

// The framework's adc_init(pin) does the one-time LPADC setup (clock, power,
// calibration) on its first call, then configures the command and trigger for
// the given pin. Temperature::init calls adc_enable() for every ADC sensor it
// knows about, so hook there rather than hand-listing a few pins.
void MarlinHAL::adc_init() {}

void MarlinHAL::adc_enable(const pin_t pin) {
  if (pin < 0) return;
  pinMode(pin, INPUT);
  ::adc_init(uint8_t(pin));
}

void MarlinHAL::adc_start(const pin_t adc_pin) {
  adc_result = analogRead(adc_pin);
}

// HAL idle task
void MarlinHAL::idletask() { }

/**
 * The ROM boot code records the cause of the last reset in the always-on
 * domain register PMC->AOREG1 (UM11126 13.4.6). It survives the reset itself
 * and is only cleared by a power-on or brown-out reset.
 */
void MarlinHAL::clear_reset_source() {
  PMC->AOREG1 = PMC_AOREG1_POR_MASK | PMC_AOREG1_PADRESET_MASK | PMC_AOREG1_BODRESET_MASK
              | PMC_AOREG1_SYSTEMRESET_MASK | PMC_AOREG1_WDTRESET_MASK | PMC_AOREG1_SWRRESET_MASK;
  TERN_(USE_WATCHDOG, watchdog_clear_timeout_flag());
}

uint8_t MarlinHAL::get_reset_source() {
  const uint32_t r = PMC->AOREG1;
  uint8_t flags = 0;
  if (r & PMC_AOREG1_POR_MASK)        flags |= RST_POWER_ON;
  if (r & PMC_AOREG1_PADRESET_MASK)   flags |= RST_EXTERNAL;
  if (r & PMC_AOREG1_BODRESET_MASK)   flags |= RST_BROWN_OUT;
  if (r & PMC_AOREG1_WDTRESET_MASK)   flags |= RST_WATCHDOG;
  // A CPU-requested system reset and an explicit software reset both come from
  // the firmware itself (NVIC_SystemReset, i.e. MarlinHAL::reboot).
  if (r & (PMC_AOREG1_SYSTEMRESET_MASK | PMC_AOREG1_SWRRESET_MASK)) flags |= RST_SOFTWARE;

  #if ENABLED(USE_WATCHDOG)
    if (watchdog_timed_out()) flags |= RST_WATCHDOG;
  #endif

  return flags ?: RST_POWER_ON;
}

#if ENABLED(USE_WATCHDOG)

  #include <fsl_wwdt.h>
  #include <fsl_clock.h>
  #include <fsl_power.h>

  /**
   * WDCLK is FRO 1 MHz divided by SYSCON->WDTCLKDIV, feeding a 24-bit
   * down-counter behind a fixed divide-by-4 prescaler (UM11126 30.2, 30.5):
   *
   *   timeout = timeoutValue * 4 / WDCLK
   *
   * With WDTCLKDIV set to divide-by-1 that gives 1,000,000 counts for 4s and
   * 2,000,000 for 8s. Both are within the 24-bit range and above the 0xFF
   * minimum reload value.
   *
   * Note WDCLK accuracy is only +/-40% over voltage, temperature and process
   * (UM11126 30.2), so a nominal 4s timeout is somewhere in 2.9s..6.7s.
   */
  #define WDT_TIMEOUT_COUNT TERN(WATCHDOG_DURATION_8S, 2000000UL, 1000000UL)

  void MarlinHAL::watchdog_init() {
    // WDTCLKDIV resets with HALT set (reset value 0x4000000), so the divider
    // is stopped and the counter never decrements. Reset the divider and set
    // divide-by-1, which also clears HALT. (UM11126 4.5.53 Table 105, 30.3)
    POWER_DisablePD(kPDRUNCFG_PD_FRO1M);          // WDCLK source must be running
    CLOCK_SetClkDiv(kCLOCK_DivWdtClk, 1U, true);  // reset divider, divide-by-1, clears HALT

    CLOCK_EnableClock(kCLOCK_Wwdt);               // AHB clock for the register interface
    wwdt_config_t config;
    WWDT_GetDefaultConfig(&config);
    config.enableWwdt            = true;       // Start the watchdog
    config.enableWatchdogReset   = true;       // Reset on timeout
    config.enableWatchdogProtect = false;      // Allow timeout updates
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

void flashFirmware(const int16_t) { MarlinHAL::reboot(); }

// Maple Compatibility
volatile uint32_t systick_uptime_millis = 0;
systickCallback_t systick_user_callback;
void systick_attach_callback(systickCallback_t cb) { systick_user_callback = cb; }
void SYSTICK_Callback() {
  systick_uptime_millis++;
  if (systick_user_callback) systick_user_callback();
}

#endif // TARGET_LPC5528
