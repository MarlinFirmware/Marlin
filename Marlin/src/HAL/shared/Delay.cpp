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
    uint32_t start = HW_REG(_DWT_CYCCNT) - ASM_CYCLES_PER_ITERATION, elapsed;  
    do {
      elapsed = HW_REG(_DWT_CYCCNT) - start;
    } while (elapsed < count);
  }

  // Pointer to asm function, calling the functions has a ~20 cycles overhead
  DelayImpl DELAY_CYCLES = delay_asm;

  #if ENABLED(MARLIN_DEV_MODE) && !defined(__CORTEX_M)
 /*    FORCE_INLINE static void validate_DELAY_CYCLES_ITERATION_COST() {
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
    } */

    void compute_delay_constants() {
      // Start the DWT counter (we don't care about the cycles here)
      // Ok, let's measure basic stuff to ensure everything is working as expected
      // Step 1: Cost of reading the counter
      uint32_t s = HW_REG(_DWT_CYCCNT);
      uint32_t c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Reading cycle counter takes: ", (uint32_t)c-s);
      SERIAL_FLUSH();

      // Step 2: Cost of calling a function via the function pointer
      s = HW_REG(_DWT_CYCCNT);
      DELAY_CYCLES(0);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the DWT based delay : ", (uint32_t)c-s);
      SERIAL_FLUSH();

      // Step 3: Cost of calling the nop_counter
      DelayImpl f = delay_asm;
      s = HW_REG(_DWT_CYCCNT);
      f(0);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the asm based delay : ", (uint32_t)c-s);
      SERIAL_FLUSH();


      // Check consistency
      s = HW_REG(_DWT_CYCCNT);
      DELAY_CYCLES(100);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the DWT based delay for 100 cycles : ", (uint32_t)c-s);      
      SERIAL_FLUSH();
      s = HW_REG(_DWT_CYCCNT);
      DELAY_CYCLES(200);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the DWT based delay for 200 cycles : ", (uint32_t)c-s);      
      SERIAL_FLUSH();
      s = HW_REG(_DWT_CYCCNT);
      DELAY_CYCLES(300);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the DWT based delay for 300 cycles : ", (uint32_t)c-s);      
      SERIAL_FLUSH();
      // Check consistency
      s = HW_REG(_DWT_CYCCNT);
      f(100);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the asm based delay for 100 cycles : ", (uint32_t)c-s);      
      SERIAL_FLUSH();
      s = HW_REG(_DWT_CYCCNT);
      f(200);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the asm based delay for 200 cycles : ", (uint32_t)c-s);      
      SERIAL_FLUSH();
      s = HW_REG(_DWT_CYCCNT);
      f(300);
      c = HW_REG(_DWT_CYCCNT);
      SERIAL_ECHOLNPAIR("Overhead of calling the asm based delay for 300 cycles : ", (uint32_t)c-s);      
      SERIAL_FLUSH();
    }
  #endif

  void calibrate_delay_loop()
  {
    // Check if we have a working DWT implementation in the CPU (see https://developer.arm.com/documentation/ddi0439/b/Data-Watchpoint-and-Trace-Unit/DWT-Programmers-Model)
    if (true || !HW_REG(_DWT_CTRL)) {
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
      DELAY_CYCLES = delay_asm;
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
      DELAY_CYCLES = delay_dwt;
    }

    #if ENABLED(MARLIN_DEV_MODE)
      SERIAL_ECHOLNPAIR("Computed delay calibration value: ", ASM_CYCLES_PER_ITERATION);
      SERIAL_FLUSH();
      // Display the results of the calibration above
      uint32_t testValues[] = { 1, 5, 10, 20, 50, 100, 150, 200, 350, 500, 750, 1000 };
      for (auto i : testValues) {
        uint32_t s = micros();
        DELAY_US(i);
        uint32_t e = micros();
        SERIAL_ECHOLNPAIR("Calling the delay function for ", i, "us took: ", e - s, "us");      
        SERIAL_FLUSH();
      }
    #endif
  }
#else
  void calibrate_delay_loop() {}
#endif

