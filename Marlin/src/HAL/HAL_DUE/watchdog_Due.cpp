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

#ifdef ARDUINO_ARCH_SAM

#include "../../inc/MarlinConfig.h"
#include "../../Marlin.h"
#include "watchdog_Due.h"

// Override Arduino runtime to either config or disable the watchdog
//
// We need to configure the watchdog as soon as possible in the boot
// process, because watchdog initialization at hardware reset on SAM3X8E
// is unreliable, and there is risk of unintended resets if we delay
// that initialization to a later time.
void watchdogSetup(void) {

  #if ENABLED(USE_WATCHDOG)

    // 4 seconds timeout
    uint32_t timeout = 4000;

    // Calculate timeout value in WDT counter ticks: This assumes
    // the slow clock is running at 32.768 kHz watchdog
    // frequency is therefore 32768 / 128 = 256 Hz
    timeout = (timeout << 8) / 1000;
    if (timeout == 0)
      timeout = 1;
    else if (timeout > 0xFFF)
      timeout = 0xFFF;

    // We want to enable the watchdog with the specified timeout
    uint32_t value =
      WDT_MR_WDV(timeout) |               // With the specified timeout
      WDT_MR_WDD(timeout) |               // and no invalid write window
    #if !(SAMV70 || SAMV71 || SAME70 || SAMS70)
      WDT_MR_WDRPROC   |                  // WDT fault resets processor only - We want
                                          // to keep PIO controller state
    #endif
      WDT_MR_WDDBGHLT  |                  // WDT stops in debug state.
      WDT_MR_WDIDLEHLT;                   // WDT stops in idle state.

    #if ENABLED(WATCHDOG_RESET_MANUAL)
      // We enable the watchdog timer, but only for the interrupt.

      // Configure WDT to only trigger an interrupt
      value |= WDT_MR_WDFIEN;             // Enable WDT fault interrupt.

      // Disable WDT interrupt (just in case, to avoid triggering it!)
      NVIC_DisableIRQ(WDT_IRQn);

      // Initialize WDT with the given parameters
      WDT_Enable(WDT, value);

      // Configure and enable WDT interrupt.
      NVIC_ClearPendingIRQ(WDT_IRQn);
      NVIC_SetPriority(WDT_IRQn, 0); // Use highest priority, so we detect all kinds of lockups
      NVIC_EnableIRQ(WDT_IRQn);

    #else

      // a WDT fault triggers a reset
      value |= WDT_MR_WDRSTEN;

      // Initialize WDT with the given parameters
      WDT_Enable(WDT, value);

    #endif

    // Reset the watchdog
    WDT_Restart(WDT);

  #else

    // Make sure to completely disable the Watchdog
    WDT_Disable(WDT);

  #endif
}

#if ENABLED(USE_WATCHDOG)
  // Initialize watchdog - On SAM3X, Watchdog was already configured
  //  and enabled or disabled at startup, so no need to reconfigure it
  //  here.
  void watchdog_init(void) {
    // Reset watchdog to start clean
    WDT_Restart(WDT);
  }
#endif // USE_WATCHDOG

#endif
