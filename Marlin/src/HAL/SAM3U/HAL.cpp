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

/**
 * HAL for Atmel SAM3U (AT91SAM3U4E) / 4pi
 */

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"

#if SAM3U_SERIAL_IN_USE(-1)
  #include "usb/usb_cdc.h"
#endif

// ------------------------
// Public functions
// ------------------------

void MarlinHAL::init() {
  #if HAS_MEDIA && PIN_EXISTS(SD_SS)
    OUT_WRITE(SD_SS_PIN, HIGH);   // Deselect the SD card before any other SPI user starts up
  #endif
  #if SAM3U_SERIAL_IN_USE(-1)
    usb_cdc_init();               // Enumerate as a CDC serial device
  #endif
}

/**
 * Push any buffered USB output onto the bus. Marlin writes a byte at a time,
 * so short packets are held back until here rather than sending a USB packet
 * per character.
 */
void MarlinHAL::idletask() {
  #if SAM3U_SERIAL_IN_USE(-1)
    usb_cdc_task();
  #endif
}

void MarlinHAL::init_board() {
  #ifdef BOARD_INIT
    BOARD_INIT();
  #endif
}

/**
 * RSTC_SR.RSTTYP tells us why the last reset happened. The SAM3U encodes it
 * in bits 10:8 (datasheet section 13.4.2).
 */
uint8_t MarlinHAL::get_reset_source() {
  switch ((RSTC->RSTC_SR & RSTC_SR_RSTTYP_Msk) >> RSTC_SR_RSTTYP_Pos) {
    case 0: return RST_POWER_ON;   // General reset (power-up)
    case 1: return RST_BACKUP;     // Wake-up from backup mode
    case 2: return RST_WATCHDOG;
    case 3: return RST_SOFTWARE;
    case 4: return RST_EXTERNAL;   // NRST pin
    default: return 0;
  }
}

// RSTC_SR.RSTTYP is latched by hardware and cleared by the next reset, so
// there is nothing to clear here.
void MarlinHAL::clear_reset_source() {}

void MarlinHAL::reboot() {
  RSTC->RSTC_CR = RSTC_CR_KEY(0xA5) | RSTC_CR_PROCRST | RSTC_CR_PERRST;
  for (;;) { /* wait for the reset to take effect */ }
}

// ------------------------
// Watchdog Timer
// ------------------------

#if ENABLED(USE_WATCHDOG)

  /**
   * The SAM3U watchdog can only be configured ONCE after reset - WDT_MR is
   * write-once. clock.cpp disables it early in boot, so re-enabling it here
   * would silently do nothing. Instead, clock.cpp leaves it alone when
   * USE_WATCHDOG is set and we program it for real here, at the first (and
   * only) opportunity.
   *
   * The watchdog runs from the slow clock divided by 128, i.e. 32768/128 =
   * 256Hz, so the 12-bit counter tops out at 16 seconds.
   */
  void MarlinHAL::watchdog_init() {
    constexpr uint32_t timeout_ms = TERN(WATCHDOG_DURATION_8S, 8000, 4000);
    constexpr uint32_t ticks = _MAX(_MIN((timeout_ms * 256UL) / 1000UL, 0xFFFUL), 1UL);

    WDT->WDT_MR = WDT_MR_WDV(ticks)
                | WDT_MR_WDD(ticks)     // No forbidden window
                | WDT_MR_WDRSTEN        // A timeout resets the chip
                | WDT_MR_WDDBGHLT       // Halt while halted in the debugger
                | WDT_MR_WDIDLEHLT;     // Halt in idle mode

    watchdog_refresh();
  }

  void MarlinHAL::watchdog_refresh() {
    WDT->WDT_CR = WDT_CR_KEY(0xA5) | WDT_CR_WDRSTT;
  }

#endif // USE_WATCHDOG

// ------------------------
// Free Memory Accessor
// ------------------------

/**
 * The linker script lays SRAM out as .data/.bss, then the heap growing up,
 * then a reserved stack region at the top. Free memory is what is left
 * between the top of the heap and the bottom of the stack.
 */
extern "C" {
  extern unsigned int _sheap;   // Bottom of the heap
  extern unsigned int _sstack;  // Bottom of the reserved stack region
  extern char* sbrk(int incr);
}

int freeMemory() {
  const char * const heap_top = sbrk(0);
  const char * const heap_end = heap_top ?: (const char *)&_sheap;
  return int((const char *)&_sstack - heap_end);
}

#endif // __SAM3U4E__
