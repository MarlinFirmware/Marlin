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
#ifdef __AVR__

#include "../../inc/MarlinConfig.h"
#include "HAL.h"
#include <avr/wdt.h>

#ifdef USBCON
  DefaultSerial1 MSerial0(false, Serial);
  #ifdef BLUETOOTH
    BTSerial btSerial(false, bluetoothSerial);
  #endif
#endif

// ------------------------
// Public Variables
// ------------------------

// Don't initialize/override variable (which would happen in .init4)
uint8_t MarlinHAL::reset_reason __attribute__((section(".noinit")));

// ------------------------
// Public functions
// ------------------------

__attribute__((naked))             // Don't output function pro- and epilogue
__attribute__((used))              // Output the function, even if "not used"
__attribute__((section(".init3"))) // Put in an early user definable section
void save_reset_reason() {
  #if ENABLED(OPTIBOOT_RESET_REASON)
    __asm__ __volatile__(
      A("STS %0, r2")
      : "=m"(hal.reset_reason)
    );
  #else
    hal.reset_reason = MCUSR;
  #endif

  // Clear within 16ms since WDRF bit enables a 16ms watchdog timer -> Boot loop
  hal.clear_reset_source();
  wdt_disable();
}

void MarlinHAL::init() {
  // Init Servo Pins
  #define INIT_SERVO(N) OUT_WRITE(SERVO##N##_PIN, LOW)
  #if HAS_SERVO_0
    INIT_SERVO(0);
  #endif
  #if HAS_SERVO_1
    INIT_SERVO(1);
  #endif
  #if HAS_SERVO_2
    INIT_SERVO(2);
  #endif
  #if HAS_SERVO_3
    INIT_SERVO(3);
  #endif

  init_pwm_timers();   // Init user timers to default frequency - 1000HZ
}

void MarlinHAL::reboot() {
  #if ENABLED(USE_WATCHDOG)
    while (1) { /* run out the watchdog */ }
  #else
    void (*resetFunc)() = 0;  // Declare resetFunc() at address 0
    resetFunc();              // Jump to address 0
  #endif
}

// ------------------------
// Watchdog Timer
// ------------------------

#if ENABLED(USE_WATCHDOG)

  #include <avr/wdt.h>
  #include "../../MarlinCore.h"

  // Initialize watchdog with 8s timeout, if possible. Otherwise, make it 4s.
  void MarlinHAL::watchdog_init() {
    #if ENABLED(WATCHDOG_DURATION_8S) && defined(WDTO_8S)
      #define WDTO_NS WDTO_8S
    #else
      #define WDTO_NS WDTO_4S
    #endif
    #if ENABLED(WATCHDOG_RESET_MANUAL)
      // Enable the watchdog timer, but only for the interrupt.
      // Take care, as this requires the correct order of operation, with interrupts disabled.
      // See the datasheet of any AVR chip for details.
      wdt_reset();
      cli();
      _WD_CONTROL_REG = _BV(_WD_CHANGE_BIT) | _BV(WDE);
      _WD_CONTROL_REG = _BV(WDIE) | (WDTO_NS & 0x07) | ((WDTO_NS & 0x08) << 2); // WDTO_NS directly does not work. bit 0-2 are consecutive in the register but the highest value bit is at bit 5
                                                                                // So worked for up to WDTO_2S
      sei();
      wdt_reset();
    #else
      wdt_enable(WDTO_NS); // The function handles the upper bit correct.
    #endif
    //delay(10000); // test it!
  }

  //===========================================================================
  //=================================== ISR ===================================
  //===========================================================================

  // Watchdog timer interrupt, called if main program blocks >4sec and manual reset is enabled.
  #if ENABLED(WATCHDOG_RESET_MANUAL)
    ISR(WDT_vect) {
      sei();  // With the interrupt driven serial we need to allow interrupts.
      SERIAL_ERROR_MSG(STR_WATCHDOG_FIRED);
      minkill();  // interrupt-safe final kill and infinite loop
    }
  #endif

  // Reset watchdog. MUST be called at least every 4 seconds after the
  // first watchdog_init or AVR will go into emergency procedures.
  void MarlinHAL::watchdog_refresh() { wdt_reset(); }

#endif // USE_WATCHDOG

// ------------------------
// Free Memory Accessor
// ------------------------

#if ENABLED(SDSUPPORT)

  #include "../../sd/SdFatUtil.h"
  int freeMemory() { return SdFatUtil::FreeRam(); }

#else // !SDSUPPORT

  extern "C" {
    extern char __bss_end;
    extern char __heap_start;
    extern void* __brkval;

    int freeMemory() {
      int free_memory;
      if ((int)__brkval == 0)
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
      else
        free_memory = ((int)&free_memory) - ((int)__brkval);
      return free_memory;
    }
  }

#endif // !SDSUPPORT

#endif // __AVR__
