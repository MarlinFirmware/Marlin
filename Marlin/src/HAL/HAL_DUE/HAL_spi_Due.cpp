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
 * Software SPI functions originally from Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 *
 * Completely rewritten and tuned by Eduardo José Tagle in 2017/2018
 * in ARM thumb2 inline assembler and tuned for maximum speed and performance
 * allowing SPI clocks of up to 12 Mhz to increase SD card read/write performance
 */

/**
 * Description: HAL for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 */

#ifdef ARDUINO_ARCH_SAM

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../../inc/MarlinConfig.h"

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

#if ENABLED(DUE_SOFTWARE_SPI)

  // --------------------------------------------------------------------------
  // software SPI
  // --------------------------------------------------------------------------

  // set optimization so ARDUINO optimizes this file
  #pragma GCC optimize (3)

  /* ---------------- Delay Cycles routine -------------- */

  /* https://blueprints.launchpad.net/gcc-arm-embedded/+spec/delay-cycles */

  #define nop() __asm__ __volatile__("nop;\n\t":::)

  FORCE_INLINE static void __delay_4cycles(uint32_t cy) { // +1 cycle
    #if ARCH_PIPELINE_RELOAD_CYCLES<2
      #define EXTRA_NOP_CYCLES "nop"
    #else
      #define EXTRA_NOP_CYCLES ""
    #endif

    __asm__ __volatile__(
      ".syntax unified" "\n\t" // is to prevent CM0,CM1 non-unified syntax

      "loop%=:" "\n\t"
      " subs %[cnt],#1" "\n\t"
      EXTRA_NOP_CYCLES "\n\t"
      " bne loop%=" "\n\t"
      : [cnt]"+r"(cy) // output: +r means input+output
      : // input:
      : "cc" // clobbers:
    );
  }

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
    }
    else
      __delay_4cycles(x / 4);
  }

  /* ---------------- Delay in nanoseconds and in microseconds */

  #define DELAY_NS(x) DELAY_CYCLES( (x) * (F_CPU/1000000) / 1000)

  typedef uint8_t (*pfnSpiTransfer) (uint8_t b);
  typedef void    (*pfnSpiRxBlock)(uint8_t* buf, uint32_t nbyte);
  typedef void    (*pfnSpiTxBlock)(const uint8_t* buf, uint32_t nbyte);


  /* ---------------- Macros to be able to access definitions from asm */

  #define _PORT(IO) DIO ##  IO ## _WPORT
  #define _PIN_MASK(IO) MASK(DIO ## IO ## _PIN)
  #define _PIN_SHIFT(IO) DIO ## IO ## _PIN
  #define PORT(IO) _PORT(IO)
  #define PIN_MASK(IO) _PIN_MASK(IO)
  #define PIN_SHIFT(IO) _PIN_SHIFT(IO)

  // run at ~8 .. ~10Mhz - Tx version (Rx data discarded)
  static uint8_t spiTransferTx0(uint8_t bout) { // using Mode 0
    register uint32_t MOSI_PORT_PLUS30 = ((uint32_t) PORT(MOSI_PIN)) + 0x30;  /* SODR of port */
    register uint32_t MOSI_MASK = PIN_MASK(MOSI_PIN);
    register uint32_t SCK_PORT_PLUS30 = ((uint32_t) PORT(SCK_PIN)) + 0x30;    /* SODR of port */
    register uint32_t SCK_MASK = PIN_MASK(SCK_PIN);
    register uint32_t idx = 0;

    /* Negate bout, as the assembler requires a negated value */
    bout = ~bout;

    /* The software SPI routine */
    __asm__ __volatile__(
      ".syntax unified" "\n\t" // is to prevent CM0,CM1 non-unified syntax

      /* Bit 7 */
      " ubfx %[idx],%[txval],#7,#1" "\n\t"                      /* Place bit 7 in bit 0 of idx*/

      " str %[mosi_mask],[%[mosi_port], %[idx],LSL #2]" "\n\t"  /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[idx],%[txval],#6,#1" "\n\t"                      /* Place bit 6 in bit 0 of idx*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 6 */
      " str %[mosi_mask],[%[mosi_port], %[idx],LSL #2]" "\n\t"  /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[idx],%[txval],#5,#1" "\n\t"                      /* Place bit 5 in bit 0 of idx*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 5 */
      " str %[mosi_mask],[%[mosi_port], %[idx],LSL #2]" "\n\t"  /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[idx],%[txval],#4,#1" "\n\t"                      /* Place bit 4 in bit 0 of idx*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 4 */
      " str %[mosi_mask],[%[mosi_port], %[idx],LSL #2]" "\n\t"  /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[idx],%[txval],#3,#1" "\n\t"                      /* Place bit 3 in bit 0 of idx*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 3 */
      " str %[mosi_mask],[%[mosi_port], %[idx],LSL #2]" "\n\t"  /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[idx],%[txval],#2,#1" "\n\t"                      /* Place bit 2 in bit 0 of idx*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 2 */
      " str %[mosi_mask],[%[mosi_port], %[idx],LSL #2]" "\n\t"  /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[idx],%[txval],#1,#1" "\n\t"                      /* Place bit 1 in bit 0 of idx*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 1 */
      " str %[mosi_mask],[%[mosi_port], %[idx],LSL #2]" "\n\t"  /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[idx],%[txval],#0,#1" "\n\t"                      /* Place bit 0 in bit 0 of idx*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 0 */
      " str %[mosi_mask],[%[mosi_port], %[idx],LSL #2]" "\n\t"  /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " nop" "\n\t"                                             /* Result will be 0 */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      : [idx]"+r"( idx )
      : [txval]"r"( bout ) ,
        [mosi_mask]"r"( MOSI_MASK ),
        [mosi_port]"r"( MOSI_PORT_PLUS30 ),
        [sck_mask]"r"( SCK_MASK ),
        [sck_port]"r"( SCK_PORT_PLUS30 )
      : "cc"
    );

    return 0;
  }

 // Calculates the bit band alias address and returns a pointer address to word.
 // addr: The byte address of bitbanding bit.
 // bit:  The bit position of bitbanding bit.
#define BITBAND_ADDRESS(addr, bit) \
    (((uint32_t)(addr) & 0xF0000000) + 0x02000000 + ((uint32_t)(addr)&0xFFFFF)*32 + (bit)*4)

  // run at ~8 .. ~10Mhz - Rx version (Tx line not altered)
  static uint8_t spiTransferRx0(uint8_t bout) { // using Mode 0
    register uint32_t bin = 0;
    register uint32_t work = 0;
    register uint32_t BITBAND_MISO_PORT = BITBAND_ADDRESS( ((uint32_t)PORT(MISO_PIN))+0x3C, PIN_SHIFT(MISO_PIN));  /* PDSR of port in bitband area */
    register uint32_t SCK_PORT_PLUS30 = ((uint32_t) PORT(SCK_PIN)) + 0x30;    /* SODR of port */
    register uint32_t SCK_MASK = PIN_MASK(SCK_PIN);
    UNUSED(bout);

    /* The software SPI routine */
    __asm__ __volatile__(
      ".syntax unified" "\n\t" // is to prevent CM0,CM1 non-unified syntax

      /* bit 7 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#7,#1" "\n\t"                /* Store read bit as the bit 7 */

      /* bit 6 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#6,#1" "\n\t"                /* Store read bit as the bit 6 */

      /* bit 5 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#5,#1" "\n\t"                /* Store read bit as the bit 5 */

      /* bit 4 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#4,#1" "\n\t"                /* Store read bit as the bit 4 */

      /* bit 3 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#3,#1" "\n\t"                /* Store read bit as the bit 3 */

      /* bit 2 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#2,#1" "\n\t"                /* Store read bit as the bit 2 */

      /* bit 1 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#1,#1" "\n\t"                /* Store read bit as the bit 1 */

      /* bit 0 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#0,#1" "\n\t"                /* Store read bit as the bit 0 */

      : [bin]"+r"(bin),
        [work]"+r"(work)
      : [bitband_miso_port]"r"( BITBAND_MISO_PORT ),
        [sck_mask]"r"( SCK_MASK ),
        [sck_port]"r"( SCK_PORT_PLUS30 )
      : "cc"
    );

    return bin;
  }

  // run at ~4Mhz
  static uint8_t spiTransfer1(uint8_t b) { // using Mode 0
    int bits = 8;
    do {
      WRITE(MOSI_PIN, b & 0x80);
      b <<= 1;        // little setup time

      WRITE(SCK_PIN, HIGH);
      DELAY_NS(125);  // 10 cycles @ 84mhz

      b |= (READ(MISO_PIN) != 0);

      WRITE(SCK_PIN, LOW);
      DELAY_NS(125);  // 10 cycles @ 84mhz
    } while (--bits);
    return b;
  }

  // all the others
  static uint32_t spiDelayCyclesX4 = (F_CPU/1000000); // 4uS => 125khz

  static uint8_t spiTransferX(uint8_t b) { // using Mode 0
    int bits = 8;
    do {
      WRITE(MOSI_PIN, b & 0x80);
      b <<= 1; // little setup time

      WRITE(SCK_PIN, HIGH);
      __delay_4cycles(spiDelayCyclesX4);

      b |= (READ(MISO_PIN) != 0);

      WRITE(SCK_PIN, LOW);
      __delay_4cycles(spiDelayCyclesX4);
    } while (--bits);
    return b;
  }

  // Pointers to generic functions for byte transfers
  static pfnSpiTransfer spiTransferTx = spiTransferX;
  static pfnSpiTransfer spiTransferRx = spiTransferX;

  // Block transfers run at ~8 .. ~10Mhz - Tx version (Rx data discarded)
  static void spiTxBlock0(const uint8_t* ptr, uint32_t todo) {
    register uint32_t MOSI_PORT_PLUS30 = ((uint32_t) PORT(MOSI_PIN)) + 0x30;  /* SODR of port */
    register uint32_t MOSI_MASK = PIN_MASK(MOSI_PIN);
    register uint32_t SCK_PORT_PLUS30 = ((uint32_t) PORT(SCK_PIN)) + 0x30;    /* SODR of port */
    register uint32_t SCK_MASK = PIN_MASK(SCK_PIN);
    register uint32_t work = 0;
    register uint32_t txval = 0;

    /* The software SPI routine */
    __asm__ __volatile__(
      ".syntax unified" "\n\t" // is to prevent CM0,CM1 non-unified syntax

      " loop%=:" "\n\t"
      " ldrb.w %[txval], [%[ptr]], #1" "\n\t"                   /* Load value to send, increment buffer */
      " mvn %[txval],%[txval]" "\n\t"                           /* Negate value */

      /* Bit 7 */
      " ubfx %[work],%[txval],#7,#1" "\n\t"                     /* Place bit 7 in bit 0 of work*/

      " str %[mosi_mask],[%[mosi_port], %[work],LSL #2]" "\n\t" /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[work],%[txval],#6,#1" "\n\t"                     /* Place bit 6 in bit 0 of work*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 6 */
      " str %[mosi_mask],[%[mosi_port], %[work],LSL #2]" "\n\t" /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[work],%[txval],#5,#1" "\n\t"                     /* Place bit 5 in bit 0 of work*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 5 */
      " str %[mosi_mask],[%[mosi_port], %[work],LSL #2]" "\n\t" /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[work],%[txval],#4,#1" "\n\t"                     /* Place bit 4 in bit 0 of work*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 4 */
      " str %[mosi_mask],[%[mosi_port], %[work],LSL #2]" "\n\t" /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[work],%[txval],#3,#1" "\n\t"                     /* Place bit 3 in bit 0 of work*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 3 */
      " str %[mosi_mask],[%[mosi_port], %[work],LSL #2]" "\n\t" /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[work],%[txval],#2,#1" "\n\t"                     /* Place bit 2 in bit 0 of work*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 2 */
      " str %[mosi_mask],[%[mosi_port], %[work],LSL #2]" "\n\t" /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[work],%[txval],#1,#1" "\n\t"                     /* Place bit 1 in bit 0 of work*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 1 */
      " str %[mosi_mask],[%[mosi_port], %[work],LSL #2]" "\n\t" /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " ubfx %[work],%[txval],#0,#1" "\n\t"                     /* Place bit 0 in bit 0 of work*/
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */

      /* Bit 0 */
      " str %[mosi_mask],[%[mosi_port], %[work],LSL #2]" "\n\t"  /* Access the proper SODR or CODR registers based on that bit */
      " str %[sck_mask],[%[sck_port]]" "\n\t"                   /* SODR */
      " subs %[todo],#1" "\n\t"                                 /* Decrement count of pending words to send, update status */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"              /* CODR */
      " bne.n loop%=" "\n\t"                                    /* Repeat until done */

      : [ptr]"+r" ( ptr ) ,
        [todo]"+r" ( todo ) ,
        [work]"+r"( work ) ,
        [txval]"+r"( txval )
      : [mosi_mask]"r"( MOSI_MASK ),
        [mosi_port]"r"( MOSI_PORT_PLUS30 ),
        [sck_mask]"r"( SCK_MASK ),
        [sck_port]"r"( SCK_PORT_PLUS30 )
      : "cc"
    );
  }

  static void spiRxBlock0(uint8_t* ptr, uint32_t todo) {
    register uint32_t bin = 0;
    register uint32_t work = 0;
    register uint32_t BITBAND_MISO_PORT = BITBAND_ADDRESS( ((uint32_t)PORT(MISO_PIN))+0x3C, PIN_SHIFT(MISO_PIN));  /* PDSR of port in bitband area */
    register uint32_t SCK_PORT_PLUS30 = ((uint32_t) PORT(SCK_PIN)) + 0x30;    /* SODR of port */
    register uint32_t SCK_MASK = PIN_MASK(SCK_PIN);

    /* The software SPI routine */
    __asm__ __volatile__(
      ".syntax unified" "\n\t" // is to prevent CM0,CM1 non-unified syntax

      " loop%=:" "\n\t"

      /* bit 7 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#7,#1" "\n\t"                /* Store read bit as the bit 7 */

      /* bit 6 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#6,#1" "\n\t"                /* Store read bit as the bit 6 */

      /* bit 5 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#5,#1" "\n\t"                /* Store read bit as the bit 5 */

      /* bit 4 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#4,#1" "\n\t"                /* Store read bit as the bit 4 */

      /* bit 3 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#3,#1" "\n\t"                /* Store read bit as the bit 3 */

      /* bit 2 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#2,#1" "\n\t"                /* Store read bit as the bit 2 */

      /* bit 1 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#1,#1" "\n\t"                /* Store read bit as the bit 1 */

      /* bit 0 */
      " str %[sck_mask],[%[sck_port]]" "\n\t"           /* SODR */
      " ldr %[work],[%[bitband_miso_port]]" "\n\t"      /* PDSR on bitband area for required bit: work will be 1 or 0 based on port */
      " str %[sck_mask],[%[sck_port],#0x4]" "\n\t"      /* CODR */
      " bfi %[bin],%[work],#0,#1" "\n\t"                /* Store read bit as the bit 0 */

      " subs %[todo],#1" "\n\t"                         /* Decrement count of pending words to send, update status */
      " strb.w %[bin], [%[ptr]], #1" "\n\t"             /* Store read value into buffer, increment buffer pointer */
      " bne.n loop%=" "\n\t"                            /* Repeat until done */

      : [ptr]"+r"(ptr),
        [todo]"+r"(todo),
        [bin]"+r"(bin),
        [work]"+r"(work)
      : [bitband_miso_port]"r"( BITBAND_MISO_PORT ),
        [sck_mask]"r"( SCK_MASK ),
        [sck_port]"r"( SCK_PORT_PLUS30 )
      : "cc"
    );
  }

  static void spiTxBlockX(const uint8_t* buf, uint32_t todo) {
    do {
      (void) spiTransferTx(*buf++);
    } while (--todo);
  }

  static void spiRxBlockX(uint8_t* buf, uint32_t todo) {
    do {
      *buf++ = spiTransferRx(0xff);
    } while (--todo);
  }

  // Pointers to generic functions for block tranfers
  static pfnSpiTxBlock spiTxBlock = spiTxBlockX;
  static pfnSpiRxBlock spiRxBlock = spiRxBlockX;

  #if MB(ALLIGATOR)  // control SDSS pin
    void spiBegin() {
      SET_OUTPUT(SS_PIN);
      WRITE(SS_PIN, HIGH);
      SET_OUTPUT(SCK_PIN);
      SET_INPUT(MISO_PIN);
      SET_OUTPUT(MOSI_PIN);
    }

    uint8_t spiRec() {
      WRITE(SS_PIN, LOW);
      WRITE(MOSI_PIN, 1); /* Output 1s 1*/
      uint8_t b = spiTransferRx(0xFF);
      WRITE(SS_PIN, HIGH);
      return b;
    }

    void spiRead(uint8_t* buf, uint16_t nbyte) {
      uint32_t todo = nbyte;
      if (todo == 0) return;

      WRITE(SS_PIN, LOW);
      WRITE(MOSI_PIN, 1); /* Output 1s 1*/
      spiRxBlock(buf,nbyte);
      WRITE(SS_PIN, HIGH);
    }

    void spiSend(uint8_t b) {
      WRITE(SS_PIN, LOW);
      (void) spiTransferTx(b);
      WRITE(SS_PIN, HIGH);
    }

    void spiSendBlock(uint8_t token, const uint8_t* buf) {
      WRITE(SS_PIN, LOW);
      (void) spiTransferTx(token);
      spiTxBlock(buf,512);
      WRITE(SS_PIN, HIGH);

  #else   // let calling routine control SDSS
    void spiBegin() {
      SET_OUTPUT(SS_PIN);
      SET_OUTPUT(SCK_PIN);
      SET_INPUT(MISO_PIN);
      SET_OUTPUT(MOSI_PIN);
    }

    uint8_t spiRec() {
      WRITE(MOSI_PIN, 1); /* Output 1s 1*/
      uint8_t b = spiTransferRx(0xFF);
      return b;
    }

    void spiRead(uint8_t* buf, uint16_t nbyte) {
      uint32_t todo = nbyte;
      if (todo == 0) return;

      WRITE(MOSI_PIN, 1); /* Output 1s 1*/
      spiRxBlock(buf,nbyte);
    }

    void spiSend(uint8_t b) {
      (void) spiTransferTx(b);
    }

    void spiSendBlock(uint8_t token, const uint8_t* buf) {
      (void) spiTransferTx(token);
      spiTxBlock(buf,512);

    #endif

  }

  /**
   * spiRate should be
   *  0 :  8 - 10 MHz
   *  1 :  4 - 5 MHz
   *  2 :  2 - 2.5 MHz
   *  3 :  1 - 1.25 MHz
   *  4 :  500 - 625 kHz
   *  5 :  250 - 312 kHz
   *  6 :  125 - 156 kHz
   */
  void spiInit(uint8_t spiRate) {
    switch (spiRate) {
      case 0:
        spiTransferTx = spiTransferTx0;
        spiTransferRx = spiTransferRx0;
        spiTxBlock = spiTxBlock0;
        spiRxBlock = spiRxBlock0;
        break;
      case 1:
        spiTransferTx = spiTransfer1;
        spiTransferRx = spiTransfer1;
        spiTxBlock = spiTxBlockX;
        spiRxBlock = spiRxBlockX;
        break;
      default:
        spiDelayCyclesX4 = (F_CPU/1000000) >> (6 - spiRate);
        spiTransferTx = spiTransferX;
        spiTransferRx = spiTransferX;
        spiTxBlock = spiTxBlockX;
        spiRxBlock = spiRxBlockX;
        break;
    }

    #if MB(ALLIGATOR)
      WRITE(SS_PIN, HIGH);
    #endif
    WRITE(MOSI_PIN, HIGH);
    WRITE(SCK_PIN, LOW);
  }

  /** Begin SPI transaction, set clock, bit order, data mode */
  void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
    // TODO: to be implemented
  }

  #pragma GCC reset_options

#else

  #if MB(ALLIGATOR)

    // slave selects controlled by SPI controller
    // doesn't support changing SPI speeds for SD card

    // --------------------------------------------------------------------------
    // hardware SPI
    // --------------------------------------------------------------------------
    // 8.4 MHz, 4 MHz, 2 MHz, 1 MHz, 0.5 MHz, 0.329 MHz, 0.329 MHz
    int spiDueDividors[] = { 10, 21, 42, 84, 168, 255, 255 };
    bool spiInitMaded = false;

    void spiBegin() {
      if(spiInitMaded == false) {
        // Configure SPI pins
        PIO_Configure(
           g_APinDescription[SCK_PIN].pPort,
           g_APinDescription[SCK_PIN].ulPinType,
           g_APinDescription[SCK_PIN].ulPin,
           g_APinDescription[SCK_PIN].ulPinConfiguration);
        PIO_Configure(
           g_APinDescription[MOSI_PIN].pPort,
           g_APinDescription[MOSI_PIN].ulPinType,
           g_APinDescription[MOSI_PIN].ulPin,
           g_APinDescription[MOSI_PIN].ulPinConfiguration);
        PIO_Configure(
           g_APinDescription[MISO_PIN].pPort,
           g_APinDescription[MISO_PIN].ulPinType,
           g_APinDescription[MISO_PIN].ulPin,
           g_APinDescription[MISO_PIN].ulPinConfiguration);

        // set master mode, peripheral select, fault detection
        SPI_Configure(SPI0, ID_SPI0, SPI_MR_MSTR | SPI_MR_MODFDIS | SPI_MR_PS);
        SPI_Enable(SPI0);

        #if MB(ALLIGATOR)
          SET_OUTPUT(DAC0_SYNC);
          #if EXTRUDERS > 1
            SET_OUTPUT(DAC1_SYNC);
            WRITE(DAC1_SYNC, HIGH);
          #endif
          SET_OUTPUT(SPI_EEPROM1_CS);
          SET_OUTPUT(SPI_EEPROM2_CS);
          SET_OUTPUT(SPI_FLASH_CS);
          WRITE(DAC0_SYNC, HIGH);
          WRITE(SPI_EEPROM1_CS, HIGH );
          WRITE(SPI_EEPROM2_CS, HIGH );
          WRITE(SPI_FLASH_CS, HIGH );
          WRITE(SS_PIN, HIGH );
        #endif // MB(ALLIGATOR)

        OUT_WRITE(SDSS,0);

        PIO_Configure(
          g_APinDescription[SPI_PIN].pPort,
          g_APinDescription[SPI_PIN].ulPinType,
          g_APinDescription[SPI_PIN].ulPin,
          g_APinDescription[SPI_PIN].ulPinConfiguration);

        spiInit(1);
        spiInitMaded = true;
      }
    }

    void spiInit(uint8_t spiRate) {
      if(spiInitMaded == false) {
        if(spiRate > 6) spiRate = 1;

        #if MB(ALLIGATOR)
          // Set SPI mode 1, clock, select not active after transfer, with delay between transfers
          SPI_ConfigureNPCS(SPI0, SPI_CHAN_DAC,
                            SPI_CSR_CSAAT | SPI_CSR_SCBR(spiDueDividors[spiRate]) |
                            SPI_CSR_DLYBCT(1));
          // Set SPI mode 0, clock, select not active after transfer, with delay between transfers
          SPI_ConfigureNPCS(SPI0, SPI_CHAN_EEPROM1, SPI_CSR_NCPHA |
                            SPI_CSR_CSAAT | SPI_CSR_SCBR(spiDueDividors[spiRate]) |
                            SPI_CSR_DLYBCT(1));
        #endif//MB(ALLIGATOR)

        // Set SPI mode 0, clock, select not active after transfer, with delay between transfers
        SPI_ConfigureNPCS(SPI0, SPI_CHAN, SPI_CSR_NCPHA |
                          SPI_CSR_CSAAT | SPI_CSR_SCBR(spiDueDividors[spiRate]) |
                          SPI_CSR_DLYBCT(1));
        SPI_Enable(SPI0);
        spiInitMaded = true;
      }
    }

    // Write single byte to SPI
    void spiSend(byte b) {
      // write byte with address and end transmission flag
      SPI0->SPI_TDR = (uint32_t)b | SPI_PCS(SPI_CHAN) | SPI_TDR_LASTXFER;
      // wait for transmit register empty
      while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
      // wait for receive register
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      // clear status
      SPI0->SPI_RDR;
      //delayMicroseconds(1U);
    }

    void spiSend(const uint8_t* buf, size_t n) {
      if (n == 0) return;
      for (size_t i = 0; i < n - 1; i++) {
        SPI0->SPI_TDR = (uint32_t)buf[i] | SPI_PCS(SPI_CHAN);
        while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
        while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
        SPI0->SPI_RDR;
        //delayMicroseconds(1U);
      }
      spiSend(buf[n - 1]);
    }

    void spiSend(uint32_t chan, byte b) {
      uint8_t dummy_read = 0;
      // wait for transmit register empty
      while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
      // write byte with address and end transmission flag
      SPI0->SPI_TDR = (uint32_t)b | SPI_PCS(chan) | SPI_TDR_LASTXFER;
      // wait for receive register
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      // clear status
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 1)
        dummy_read = SPI0->SPI_RDR;
      UNUSED(dummy_read);
    }

    void spiSend(uint32_t chan, const uint8_t* buf, size_t n) {
      uint8_t dummy_read = 0;
      if (n == 0) return;
      for (int i = 0; i < (int)n - 1; i++) {
        while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
        SPI0->SPI_TDR = (uint32_t)buf[i] | SPI_PCS(chan);
        while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
        while ((SPI0->SPI_SR & SPI_SR_RDRF) == 1)
          dummy_read = SPI0->SPI_RDR;
        UNUSED(dummy_read);
      }
      spiSend(chan, buf[n - 1]);
    }

    // Read single byte from SPI
    uint8_t spiRec() {
      // write dummy byte with address and end transmission flag
      SPI0->SPI_TDR = 0x000000FF | SPI_PCS(SPI_CHAN) | SPI_TDR_LASTXFER;
      // wait for transmit register empty
      while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);

      // wait for receive register
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      // get byte from receive register
      //delayMicroseconds(1U);
      return SPI0->SPI_RDR;
    }

    uint8_t spiRec(uint32_t chan) {
      uint8_t spirec_tmp;
      // wait for transmit register empty
      while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 1)
        spirec_tmp =  SPI0->SPI_RDR;
        UNUSED(spirec_tmp);

      // write dummy byte with address and end transmission flag
      SPI0->SPI_TDR = 0x000000FF | SPI_PCS(chan) | SPI_TDR_LASTXFER;

      // wait for receive register
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      // get byte from receive register
      return SPI0->SPI_RDR;
    }

    // Read from SPI into buffer
    void spiRead(uint8_t*buf, uint16_t nbyte) {
      if (nbyte-- == 0) return;

      for (int i = 0; i < nbyte; i++) {
        //while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
        SPI0->SPI_TDR = 0x000000FF | SPI_PCS(SPI_CHAN);
        while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
        buf[i] = SPI0->SPI_RDR;
        //delayMicroseconds(1U);
      }
      buf[nbyte] = spiRec();
    }

    // Write from buffer to SPI
    void spiSendBlock(uint8_t token, const uint8_t* buf) {
      SPI0->SPI_TDR = (uint32_t)token | SPI_PCS(SPI_CHAN);
      while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
      //while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      //SPI0->SPI_RDR;
      for (int i = 0; i < 511; i++) {
        SPI0->SPI_TDR = (uint32_t)buf[i] | SPI_PCS(SPI_CHAN);
        while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
        while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
        SPI0->SPI_RDR;
        //delayMicroseconds(1U);
      }
      spiSend(buf[511]);
    }

    /** Begin SPI transaction, set clock, bit order, data mode */
    void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
      // TODO: to be implemented
    }

  #else  // U8G compatible hardware SPI

    void spiInit(uint8_t spiRate = 6 ) {  // default to slowest rate if not specified)
      // 8.4 MHz, 4 MHz, 2 MHz, 1 MHz, 0.5 MHz, 0.329 MHz, 0.329 MHz
      int spiDueDividors[] = { 10, 21, 42, 84, 168, 255, 255 };
      if(spiRate > 6) spiRate = 1;

          /* enable PIOA and SPI0 */
      REG_PMC_PCER0 = (1UL << ID_PIOA) | (1UL << ID_SPI0);

      /* disable PIO on A26 and A27 */
      REG_PIOA_PDR = 0x0c000000;
      OUT_WRITE(SDSS, 1);

      /* reset SPI0 (from sam lib) */
      SPI0->SPI_CR = SPI_CR_SPIDIS;
      SPI0->SPI_CR = SPI_CR_SWRST;
      SPI0->SPI_CR = SPI_CR_SWRST;
      SPI0->SPI_CR = SPI_CR_SPIEN;


      /* master mode, no fault detection, chip select 0 */
      SPI0->SPI_MR = SPI_MR_MSTR | SPI_MR_PCSDEC | SPI_MR_MODFDIS;

      /* SPI mode 0, 8 Bit data transfer, baud rate */
      SPI0->SPI_CSR[0] = SPI_CSR_SCBR(spiDueDividors[spiRate]) | 1;
    }

    static uint8_t spiTransfer(uint8_t data) {

      /* wait until tx register is empty */
      while( (SPI0->SPI_SR & SPI_SR_TDRE) == 0 );
      /* send data */
      SPI0->SPI_TDR = (uint32_t)data; // | SPI_PCS(0xF);

      // wait for transmit register empty
      while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);

      // wait for receive register
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      // get byte from receive register
      return SPI0->SPI_RDR;
    }

    void spiBegin() {
      spiInit();
    }

    uint8_t spiRec() {
      uint8_t data = spiTransfer(0xff);
      return data;
    }

    void spiRead(uint8_t*buf, uint16_t nbyte) {
      if (nbyte == 0) return;
      for (int i = 0; i < nbyte; i++) {
        buf[i] = spiTransfer(0xff);
      }
    }

    void spiSend(uint8_t data) {
      spiTransfer(data);
    }

    void spiSend(const uint8_t* buf, size_t n) {
      if (n == 0) return;
      for (uint16_t i = 0; i < n; i++)
        spiTransfer(buf[i]);
    }

    void spiSendBlock(uint8_t token, const uint8_t* buf) {
      spiTransfer(token);
      for (uint16_t i = 0; i < 512; i++)
        spiTransfer(buf[i]);
    }

  #endif  //MB(ALLIGATOR)
#endif // ENABLED(SOFTWARE_SPI)

#endif // ARDUINO_ARCH_SAM
