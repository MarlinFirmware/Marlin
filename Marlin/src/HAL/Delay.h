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

/**
 * Busy wait delay cycles routines:
 *
 *  DELAY_CYCLES(count): Delay execution in cycles
 *  DELAY_NS(count): Delay execution in nanoseconds
 *  DELAY_US(count): Delay execution in microseconds
 */

#ifndef MARLIN_DELAY_H
#define MARLIN_DELAY_H

#include "../core/macros.h"

#if defined(__arm__) || defined(__thumb__)

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

  // Delay in cycles
  FORCE_INLINE static void DELAY_CYCLES(uint32_t x) {

    if (__builtin_constant_p(x)) {
      #define MAXNOPS 4

      if (x <= (MAXNOPS)) {
        switch (x) { case 4: nop(); case 3: nop(); case 2: nop(); case 1: nop(); }
      }
      else { // because of +1 cycle inside delay_4cycles
        const uint32_t rem = (x - 1) % (MAXNOPS);
        switch (rem) { case 3: nop(); case 2: nop(); case 1: nop(); }
        if ((x = (x - 1) / (MAXNOPS)))
          __delay_4cycles(x); // if need more then 4 nop loop is more optimal
      }
      #undef MAXNOPS
    }
    else
      __delay_4cycles(x / 4);
  }
  #undef nop

#elif defined(__AVR__)

  #define nop() __asm__ __volatile__("nop;\n\t":::)

  FORCE_INLINE static void __delay_4cycles(uint8_t cy) {
    __asm__ __volatile__(
      L("1")
      A("dec %[cnt]")
      A("nop")
      A("brne 1b")
      : [cnt] "+r"(cy)  // output: +r means input+output
      :                 // input:
      : "cc"            // clobbers:
    );
  }

  // Delay in cycles
  FORCE_INLINE static void DELAY_CYCLES(uint16_t x) {

    if (__builtin_constant_p(x)) {
      #define MAXNOPS 4

      if (x <= (MAXNOPS)) {
        switch (x) { case 4: nop(); case 3: nop(); case 2: nop(); case 1: nop(); }
      }
      else {
        const uint32_t rem = (x) % (MAXNOPS);
        switch (rem) { case 3: nop(); case 2: nop(); case 1: nop(); }
        if ((x = (x) / (MAXNOPS)))
          __delay_4cycles(x); // if need more then 4 nop loop is more optimal
      }

      #undef MAXNOPS
    }
    else
      __delay_4cycles(x / 4);
  }
  #undef nop

#elif defined(ESP32)

  FORCE_INLINE static void DELAY_CYCLES(uint32_t x) {
    unsigned long ccount, stop;

    __asm__ __volatile__ ( "rsr     %0, ccount" : "=a" (ccount) );

    stop = ccount + x; // This can overflow

    while (ccount < stop) { // This doesn't deal with overflows
      __asm__ __volatile__ ( "rsr     %0, ccount" : "=a" (ccount) );
    }
  }

#else

  #error "Unsupported MCU architecture"

#endif

// Delay in nanoseconds
#define DELAY_NS(x) DELAY_CYCLES( (x) * (F_CPU/1000000L) / 1000L )

// Delay in microseconds
#define DELAY_US(x) DELAY_CYCLES( (x) * (F_CPU/1000000L) )

#endif // MARLIN_DELAY_H
