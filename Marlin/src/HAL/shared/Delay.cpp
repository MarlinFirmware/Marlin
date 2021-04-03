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
#include "Delay.h"

#include "../../inc/MarlinConfig.h"

#if defined(__arm__) || defined(__thumb__)

  static uint32_t ASM_CYCLES_PER_ITERATION = 4;   // Initial bet which will be adjusted in calibrate_delay_loop

  // Simple assembler loop counting down
  void delay_asm(uint32_t cy) {
    cy = _MAX(cy / ASM_CYCLES_PER_ITERATION, 1U); // Zero is forbidden here
    __asm__ __volatile__(
      A(".syntax unified") // is to prevent CM0,CM1 non-unified syntax
      L("1")
      A("subs %[cnt],#1")
      A("bne 1b")
      : [cnt]"+r"(cy)   // output: +r means input+output
      :                 // input:
      : "cc"            // clobbers:
    );
  }

  // We can't use CMSIS since it's not available on all platform, so fallback to hardcoded register values
  #define HW_REG(X)   *(volatile uint32_t *)(X)
  #define _DWT_CTRL   0xE0001000
  #define _DWT_CYCCNT 0xE0001004      // CYCCNT is 32bits, takes 37s or so to wrap.
  #define _DEM_CR     0xE000EDFC
  #define _LAR        0xE0001FB0

  // Use hardware cycle counter instead, it's much safer
  void delay_dwt(uint32_t count) {
    // Reuse the ASM_CYCLES_PER_ITERATION variable to avoid wasting another useless variable
    register uint32_t start = HW_REG(_DWT_CYCCNT) - ASM_CYCLES_PER_ITERATION, elapsed;
    do {
      elapsed = HW_REG(_DWT_CYCCNT) - start;
    } while (elapsed < count);
  }

  // Pointer to asm function, calling the functions has a ~20 cycles overhead
  DelayImpl DelayCycleFnc = delay_asm;

  void calibrate_delay_loop() {
    // Check if we have a working DWT implementation in the CPU (see https://developer.arm.com/documentation/ddi0439/b/Data-Watchpoint-and-Trace-Unit/DWT-Programmers-Model)
    if (!HW_REG(_DWT_CTRL)) {
      // No DWT present, so fallback to plain old ASM nop counting
      // Unfortunately, we don't exactly know how many iteration it'll take to decrement a counter in a loop
      // It depends on the CPU architecture, the code current position (flash vs SRAM)
      // So, instead of wild guessing and making mistake, instead
      // compute it once for all
      ASM_CYCLES_PER_ITERATION = 1;
      // We need to fetch some reference clock before waiting
      cli();
        uint32_t start = micros();
        delay_asm(1000); // On a typical CPU running in MHz, waiting 1000 "unknown cycles" means it'll take between 1ms to 6ms, that's perfectly acceptable
        uint32_t end = micros();
      sei();
      uint32_t expectedCycles = (end - start) * ((F_CPU) / 1000000UL); // Convert microseconds to cycles
      // Finally compute the right scale
      ASM_CYCLES_PER_ITERATION = (uint32_t)(expectedCycles / 1000);

      // No DWT present, likely a Cortex M0 so NOP counting is our best bet here
      DelayCycleFnc = delay_asm;
    }
    else {
      // Enable DWT counter
      // From https://stackoverflow.com/a/41188674/1469714
      HW_REG(_DEM_CR) = HW_REG(_DEM_CR) | 0x01000000;   // Enable trace
      #if __CORTEX_M == 7
        HW_REG(_LAR) = 0xC5ACCE55;                      // Unlock access to DWT registers, see https://developer.arm.com/documentation/ihi0029/e/ section B2.3.10
      #endif
      HW_REG(_DWT_CYCCNT) = 0;                          // Clear DWT cycle counter
      HW_REG(_DWT_CTRL) = HW_REG(_DWT_CTRL) | 1;        // Enable DWT cycle counter

      // Then calibrate the constant offset from the counter
      ASM_CYCLES_PER_ITERATION = 0;
      uint32_t s = HW_REG(_DWT_CYCCNT);
      uint32_t e = HW_REG(_DWT_CYCCNT);  // (e - s) contains the number of cycle required to read the cycle counter
      delay_dwt(0);
      uint32_t f = HW_REG(_DWT_CYCCNT);  // (f - e) contains the delay to call the delay function + the time to read the cycle counter
      ASM_CYCLES_PER_ITERATION = (f - e) - (e - s);

      // Use safer DWT function
      DelayCycleFnc = delay_dwt;
    }
  }

  #if ENABLED(MARLIN_DEV_MODE)
    void dump_delay_accuracy_check() {
      auto report_call_time = [](PGM_P const name, PGM_P const unit, const uint32_t cycles, const uint32_t total, const bool do_flush=true) {
        SERIAL_ECHOPGM("Calling ");
        serialprintPGM(name);
        SERIAL_ECHOLNPAIR(" for ", cycles);
        serialprintPGM(unit);
        SERIAL_ECHOLNPAIR(" took: ", total);
        serialprintPGM(unit);
        if (do_flush) SERIAL_FLUSH();
      };

      uint32_t s, e;

      SERIAL_ECHOLNPAIR("Computed delay calibration value: ", ASM_CYCLES_PER_ITERATION);
      SERIAL_FLUSH();
      // Display the results of the calibration above
      constexpr uint32_t testValues[] = { 1, 5, 10, 20, 50, 100, 150, 200, 350, 500, 750, 1000 };
      for (auto i : testValues) {
        s = micros(); DELAY_US(i); e = micros();
        report_call_time(PSTR("delay"), PSTR("us"), i, e - s);
      }

      if (HW_REG(_DWT_CTRL)) {
        for (auto i : testValues) {
          s = HW_REG(_DWT_CYCCNT); DELAY_CYCLES(i); e = HW_REG(_DWT_CYCCNT);
          report_call_time(PSTR("runtime delay"), PSTR("cycles"), i, e - s);
        }

        // Measure the delay to call a real function compared to a function pointer
        s = HW_REG(_DWT_CYCCNT); delay_dwt(1); e = HW_REG(_DWT_CYCCNT);
        report_call_time(PSTR("delay_dwt"), PSTR("cycles"), 1, e - s);

        static PGMSTR(dcd, "DELAY_CYCLES directly ");

        s = HW_REG(_DWT_CYCCNT); DELAY_CYCLES( 1); e = HW_REG(_DWT_CYCCNT);
        report_call_time(dcd, PSTR("cycles"),  1, e - s, false);

        s = HW_REG(_DWT_CYCCNT); DELAY_CYCLES( 5); e = HW_REG(_DWT_CYCCNT);
        report_call_time(dcd, PSTR("cycles"),  5, e - s, false);

        s = HW_REG(_DWT_CYCCNT); DELAY_CYCLES(10); e = HW_REG(_DWT_CYCCNT);
        report_call_time(dcd, PSTR("cycles"), 10, e - s, false);

        s = HW_REG(_DWT_CYCCNT); DELAY_CYCLES(20); e = HW_REG(_DWT_CYCCNT);
        report_call_time(dcd, PSTR("cycles"), 20, e - s, false);

        s = HW_REG(_DWT_CYCCNT); DELAY_CYCLES(50); e = HW_REG(_DWT_CYCCNT);
        report_call_time(dcd, PSTR("cycles"), 50, e - s, false);

        s = HW_REG(_DWT_CYCCNT); DELAY_CYCLES(100); e = HW_REG(_DWT_CYCCNT);
        report_call_time(dcd, PSTR("cycles"), 100, e - s, false);

        s = HW_REG(_DWT_CYCCNT); DELAY_CYCLES(200); e = HW_REG(_DWT_CYCCNT);
        report_call_time(dcd, PSTR("cycles"), 200, e - s, false);
      }
    }
  #endif // MARLIN_DEV_MODE


#else

  void calibrate_delay_loop() {}
  #if ENABLED(MARLIN_DEV_MODE)
    void dump_delay_accuracy_check() {
      static PGMSTR(none, "N/A on this platform");
      serialprintPGM(none);
    }
  #endif

#endif
