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

#include "../platforms.h"

#ifdef ARDUINO_ARCH_MFL

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"

uint16_t MarlinHAL::adc_result;

#if ENABLED(POSTMORTEM_DEBUGGING)
  extern void install_min_serial();
#endif

#if ENABLED(MARLIN_DEV_MODE)
  // Dump the clock frequencies of the system, AHB, APB1, APB2, and F_CPU.
  static inline void HAL_clock_frequencies_dump() {
    auto& rcuInstance = rcu::RCU::get_instance();
    uint32_t freq = rcuInstance.get_clock_frequency(rcu::Clock_Frequency::CK_SYS);
    SERIAL_ECHOPGM("\nSYSTEM_CLOCK=", freq);
    freq = rcuInstance.get_clock_frequency(rcu::Clock_Frequency::CK_AHB);
    SERIAL_ECHOPGM("\nABH_CLOCK=", freq);
    freq = rcuInstance.get_clock_frequency(rcu::Clock_Frequency::CK_APB1);
    SERIAL_ECHOPGM("\nAPB1_CLOCK=", freq);
    freq = rcuInstance.get_clock_frequency(rcu::Clock_Frequency::CK_APB2);
    SERIAL_ECHOPGM("\nAPB2_CLOCK=", freq,
    "\nF_CPU=", F_CPU);
    // Done
    SERIAL_ECHOPGM("\n--\n");
  }
#endif  // MARLIN_DEV_MODE

// Initializes the Marlin HAL
void MarlinHAL::init() {
  constexpr unsigned int cpuFreq = F_CPU;
  UNUSED(cpuFreq);

#if PIN_EXISTS(LED)
  OUT_WRITE(LED_PIN, LOW);
#endif

  SetTimerInterruptPriorities();

  // Print clock frequencies to host serial
  TERN_(MARLIN_DEV_MODE, HAL_clock_frequencies_dump());

  // Register min serial
  TERN_(POSTMORTEM_DEBUGGING, install_min_serial());
}

// Returns the reset source based on the flags set in the RCU module
uint8_t MarlinHAL::get_reset_source() {
  return
    (RCU_I.get_flag(rcu::Status_Flags::FLAG_FWDGTRST)) ? RST_WATCHDOG :
    (RCU_I.get_flag(rcu::Status_Flags::FLAG_SWRST)) ? RST_SOFTWARE :
    (RCU_I.get_flag(rcu::Status_Flags::FLAG_EPRST)) ? RST_EXTERNAL :
    (RCU_I.get_flag(rcu::Status_Flags::FLAG_PORRST)) ? RST_POWER_ON :
    (RCU_I.get_flag(rcu::Status_Flags::FLAG_LPRST)) ? RST_BROWN_OUT :
    0;
}

// Returns the amount of free memory available in bytes
int MarlinHAL::freeMemory() {
  volatile char top;
  return &top - reinterpret_cast<char*>(_sbrk(0));
}

// Watchdog Timer
#if ENABLED(USE_WATCHDOG)
  #define WDT_TIMEOUT_US  TERN(WATCHDOG_DURATION_8S, 8000000, 4000000)  // 4 or 8 second timeout

  #include <FWatchdogTimer.h>

  FWatchdogTimer& watchdogTimer = FWatchdogTimer::get_instance();

  // Initializes the watchdog timer
  void MarlinHAL::watchdog_init() {
    IF_DISABLED(DISABLE_WATCHDOG_INIT, watchdogTimer.begin(WDT_TIMEOUT_US));
  }

  // Refreshes the watchdog timer to prevent system reset
  void MarlinHAL::watchdog_refresh() {
    watchdogTimer.reload();
    #if DISABLED(PINS_DEBUGGING) && PIN_EXISTS(LED)
      TOGGLE(LED_PIN);  // Heartbeat indicator
    #endif
  }
#endif

extern "C" {
  extern unsigned int _ebss;  // End of bss section
}

// Resets the system to initiate a firmware flash.
WEAK void flashFirmware(const int16_t) {
  hal.reboot();
}

#endif // ARDUINO_ARCH_MFL
