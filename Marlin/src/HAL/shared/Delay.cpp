/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if defined(__arm__) || defined(__thumb__)

  #ifndef DELAY_CYCLES_ITERATION_COST
    #define DELAY_CYCLES_ITERATION_COST 4
  #endif

  // https://blueprints.launchpad.net/gcc-arm-embedded/+spec/delay-cycles

  #define nop() __asm__ __volatile__("nop;\n\t":::)
  FORCE_INLINE static void __delay_4cycles(uint32_t cy) { // +1 cycle
    #if ARCH_PIPELINE_RELOAD_CYCLES < 2
      #define EXTRA_NOP_CYCLES A("nop")
    #else
      #define EXTRA_NOP_CYCLES ""
    #endif

    __asm__ __volatile__(
      A(".syntax unified") // is to prevent CM0,CM1 non-unified syntax
      L("1")
      A("subs %[cnt],#1")
      EXTRA_NOP_CYCLES
      A("bne 1b")
      : [cnt]"+r"(cy)   // output: +r means input+output
      :                 // input:
      : "cc"            // clobbers:
    );
  }


  void delay_asm(uint32_t cy) { 
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

    
  // Delay in cycles
  void delay_nop(uint32_t x) {
    if (__builtin_constant_p(x)) {
      if (x <= (DELAY_CYCLES_ITERATION_COST)) {
        switch (x) { case 6: nop(); case 5: nop(); case 4: nop(); case 3: nop(); case 2: nop(); case 1: nop(); }
      }
      else { // because of +1 cycle inside delay_4cycles
        const uint32_t rem = (x - 1) % (DELAY_CYCLES_ITERATION_COST);
        switch (rem) { case 5: nop(); case 4: nop(); case 3: nop(); case 2: nop(); case 1: nop(); }
        if ((x = (x - 1) / (DELAY_CYCLES_ITERATION_COST)))
          __delay_4cycles(x); // if need more then 4 nop loop is more optimal
      }
    }
    else if ((x /= DELAY_CYCLES_ITERATION_COST))
      __delay_4cycles(x);
  }

  // We can't use CMSIS since it's not available on all platform, so fallback to hardcoded register values
  #define HW_REG(X)   *(volatile uint32_t *)(X)
  #define _DWT_CTRL   0xE0001000
  #define _DWT_CYCCNT 0xE0001004
  #define _DEM_CR     0xE000EDFC
  #define _LAR        0xE0001FB0

  void delay_dwt(uint32_t count) {
    uint32_t start = HW_REG(_DWT_CYCCNT), elapsed;  // CYCCNT is 32bits, takes 37s or so to wrap.
    do {
      elapsed = HW_REG(_DWT_CYCCNT) - start;
    } while (elapsed < count);
  }

  void initial_delay_cycle(uint32_t v) {
    // Check if we have a working DWT implementation in the CPU (see https://developer.arm.com/documentation/ddi0439/b/Data-Watchpoint-and-Trace-Unit/DWT-Programmers-Model)
    if (!HW_REG(_DWT_CTRL)) {
      // No DWT present, likely a dumb Cortex M0 so it's safe to use NOP counting here
      DELAY_CYCLES = delay_nop;
    }
    else {
      // From https://stackoverflow.com/a/41188674/1469714
      HW_REG(_DEM_CR) = HW_REG(_DEM_CR) | 0x01000000;   // Enable trace
      #if __CORTEX_M == 7
        HW_REG(_LAR) = 0xC5ACCE55;                      // Unlock access to DWT registers, see https://developer.arm.com/documentation/ihi0029/e/ section B2.3.10 
      #endif
      HW_REG(_DWT_CYCCNT) = 0;                          // Clear DWT cycle counter
      HW_REG(_DWT_CTRL) = HW_REG(_DWT_CTRL) | 1;        // Enable DWT cycle counter

      DELAY_CYCLES = delay_dwt;
      v -= 9; // Need to adjust for the initial enabling above
    }
    // Call the newly set function now
    DELAY_CYCLES(v - 3); // Account for this patch's overhead here
  }
  
  DelayImpl DELAY_CYCLES = initial_delay_cycle;

  #if ENABLED(MARLIN_DEV_MODE) && !defined(__CORTEX_M)
    FORCE_INLINE static void validate_DELAY_CYCLES_ITERATION_COST() {
      DISABLE_ISRS();
        uint32 end = systick_get_count();
        __delay_4cycles(100);
        uint32 start = systick_get_count();
      ENABLE_ISRS();
      uint32 cycles = (end - start) / 100.0;
      if (DELAY_CYCLES_ITERATION_COST > cycles * 1.05 || DELAY_CYCLES_ITERATION_COST < cycles * 0.95) {
        SERIAL_ECHOLNPAIR("DELAY_CYCLES_ITERATION_COST is: ", DELAY_CYCLES_ITERATION_COST, " but SHOULD be: ", (uint32)cycles);
      }
      else {
        SERIAL_ECHOLNPAIR("DELAY_CYCLES_ITERATION_COST is OK");
      }
    }

    void compute_delay_constants() {
      // Start the DWT counter (we don't care about the cycles here)
      initial_delay_cycle(20); 
      // Ok, let's measure basic stuff to ensure everything is working as expected
      // Step 1: Cost of reading the counter
      uint32_t s = HW_REG(_DWT_CYCCNT);
      uint32_t c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Reading cycle counter takes: ", (uint32_t)c-s);

      // Step 2: Cost of calling a function via the function pointer
      s = HW_REG(_DWT_CYCCNT);
      DELAY_CYCLES(0);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the DWT based delay : ", (uint32_t)c-s);

      // Step 3: Cost of calling the nop_counter
      DelayImpl f = delay_nop;
      s = HW_REG(_DWT_CYCCNT);
      f(0);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the nop based delay : ", (uint32_t)c-s);

      // Step 4: Cost of calling the assembler's version
      f = delay_asm;
      s = HW_REG(_DWT_CYCCNT);
      f(0);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the asm based delay : ", (uint32_t)c-s);


      // Check consistency
      s = HW_REG(_DWT_CYCCNT);
      DELAY_CYCLES(100);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the DWT based delay for 100 cycles : ", (uint32_t)c-s);      
      // Check consistency
      s = HW_REG(_DWT_CYCCNT);
      f(100);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the asm based delay for 100 cycles : ", (uint32_t)c-s);      
    }
  #endif

#endif