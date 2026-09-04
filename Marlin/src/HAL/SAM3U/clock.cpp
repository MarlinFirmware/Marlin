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
#include "clock.h"

/**
 * CMSIS declares SystemCoreClock in system_sam3u.h and defines it in
 * system_sam3u.c, which this HAL does not build (it also supplies a
 * SystemInit() that our startup code deliberately never calls - the clock is
 * brought up from main() instead). Define the variable here so anything that
 * reads it sees the real Master Clock frequency.
 */
uint32_t SystemCoreClock = F_CPU;

// Guard against a wedged oscillator/PLL rather than hanging forever
#define CLOCK_TIMEOUT 0xFFFFFUL

extern "C" void sam3u_clock_init() {

  // Allow the NRST pin to reset the chip (needed for SAM-BA / BOSSA uploads)
  RSTC->RSTC_MR = RSTC_MR_KEY(0xA5) | RSTC_MR_URSTEN;

  // Flash wait states for 96MHz. Both banks: the SAM3U4E is dual-plane.
  EFC0->EEFC_FMR = EEFC_FMR_FWS(3);
  EFC1->EEFC_FMR = EEFC_FMR_FWS(3);

  /**
   * WDT_MR is write-once after reset. If we disabled the watchdog here it
   * could never be turned back on, so only do that when Marlin does not want
   * it; otherwise leave the default 16s timeout running and let
   * MarlinHAL::watchdog_init() program it for real - that is the one write we
   * get. 16s is comfortably longer than Marlin's boot.
   */
  #if DISABLED(USE_WATCHDOG)
    WDT->WDT_MR = WDT_MR_WDDIS;
  #endif

  uint32_t timeout;

  // Start the 12MHz crystal oscillator, keeping the RC running as a fallback
  PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(0x8)
                | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
  timeout = 0;
  while (!(PMC->PMC_SR & PMC_SR_MOSCXTS) && ++timeout < CLOCK_TIMEOUT) { }

  // Switch the main clock over to the crystal
  PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(0x8)
                | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;
  timeout = 0;
  while (!(PMC->PMC_SR & PMC_SR_MOSCSELS) && ++timeout < CLOCK_TIMEOUT) { }

  // Run MCK from the main clock while PLLA settles
  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~uint32_t(PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_MAIN_CLK;
  timeout = 0;
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && ++timeout < CLOCK_TIMEOUT) { }

  // PLLA: 12MHz / DIVA * (MULA + 1) = 96MHz. Bit 29 must be set on write.
  PMC->CKGR_PLLAR = (1UL << 29)
                  | CKGR_PLLAR_MULA(SAM3U_PLLA_MULA)
                  | CKGR_PLLAR_DIVA(SAM3U_PLLA_DIVA)
                  | CKGR_PLLAR_PLLACOUNT(0x3F);
  timeout = 0;
  while (!(PMC->PMC_SR & PMC_SR_LOCKA) && ++timeout < CLOCK_TIMEOUT) { }

  // Select the prescaler first, then the source - the datasheet requires the
  // two PMC_MCKR fields to be programmed in separate writes, each followed by
  // a wait on MCKRDY.
  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~uint32_t(PMC_MCKR_PRES_Msk)) | PMC_MCKR_PRES_CLK_1;
  timeout = 0;
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && ++timeout < CLOCK_TIMEOUT) { }

  PMC->PMC_MCKR = (PMC->PMC_MCKR & ~uint32_t(PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_PLLA_CLK;
  timeout = 0;
  while (!(PMC->PMC_SR & PMC_SR_MCKRDY) && ++timeout < CLOCK_TIMEOUT) { }

  SystemCoreClock = F_CPU;
}

#endif // __SAM3U4E__
