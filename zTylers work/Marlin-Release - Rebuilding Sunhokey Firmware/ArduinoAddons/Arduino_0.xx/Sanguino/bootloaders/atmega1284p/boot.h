/* Modified to use out for SPM access
** Peter Knight, Optiboot project http://optiboot.googlecode.com
**
** Todo: Tidy up
**
** "_short" routines execute 1 cycle faster and use 1 less word of flash
** by using "out" instruction instead of "sts".
**
** Additional elpm variants that trust the value of RAMPZ
*/

/* Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007  Eric B. Weddington
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

/* $Id: boot.h,v 1.27.2.3 2008/09/30 13:58:48 arcanum Exp $ */

#ifndef _AVR_BOOT_H_
#define _AVR_BOOT_H_    1

/** \file */
/** \defgroup avr_boot <avr/boot.h>: Bootloader Support Utilities
    \code
    #include <avr/io.h>
    #include <avr/boot.h>
    \endcode

    The macros in this module provide a C language interface to the
    bootloader support functionality of certain AVR processors. These
    macros are designed to work with all sizes of flash memory.

    Global interrupts are not automatically disabled for these macros. It
    is left up to the programmer to do this. See the code example below. 
    Also see the processor datasheet for caveats on having global interrupts 
    enabled during writing of the Flash.

    \note Not all AVR processors provide bootloader support. See your
    processor datasheet to see if it provides bootloader support.

    \todo From email with Marek: On smaller devices (all except ATmega64/128),
    __SPM_REG is in the I/O space, accessible with the shorter "in" and "out"
    instructions - since the boot loader has a limited size, this could be an
    important optimization.

    \par API Usage Example
    The following code shows typical usage of the boot API.

    \code
    #include <inttypes.h>
    #include <avr/interrupt.h>
    #include <avr/pgmspace.h>
    
    void boot_program_page (uint32_t page, uint8_t *buf)
    {
        uint16_t i;
        uint8_t sreg;

        // Disable interrupts.

        sreg = SREG;
        cli();
    
        eeprom_busy_wait ();

        boot_page_erase (page);
        boot_spm_busy_wait ();      // Wait until the memory is erased.

        for (i=0; i<SPM_PAGESIZE; i+=2)
        {
            // Set up little-endian word.

            uint16_t w = *buf++;
            w += (*buf++) << 8;
        
            boot_page_fill (page + i, w);
        }

        boot_page_write (page);     // Store buffer in flash page.
        boot_spm_busy_wait();       // Wait until the memory is written.

        // Reenable RWW-section again. We need this if we want to jump back
        // to the application after bootloading.

        boot_rww_enable ();

        // Re-enable interrupts (if they were ever enabled).

        SREG = sreg;
    }\endcode */

#include <avr/eeprom.h>
#include <avr/io.h>
#include <inttypes.h>
#include <limits.h>

/* Check for SPM Control Register in processor. */
#if defined (SPMCSR)
#  define __SPM_REG    SPMCSR
#elif defined (SPMCR)
#  define __SPM_REG    SPMCR
#else
#  error AVR processor does not provide bootloader support!
#endif


/* Check for SPM Enable bit. */
#if defined(SPMEN)
#  define __SPM_ENABLE  SPMEN
#elif defined(SELFPRGEN)
#  define __SPM_ENABLE  SELFPRGEN
#else
#  error Cannot find SPM Enable bit definition!
#endif

/** \ingroup avr_boot
    \def BOOTLOADER_SECTION

    Used to declare a function or variable to be placed into a
    new section called .bootloader. This section and its contents
    can then be relocated to any address (such as the bootloader
    NRWW area) at link-time. */

#define BOOTLOADER_SECTION    __attribute__ ((section (".bootloader")))

/* Create common bit definitions. */
#ifdef ASB
#define __COMMON_ASB    ASB
#else
#define __COMMON_ASB    RWWSB
#endif

#ifdef ASRE
#define __COMMON_ASRE   ASRE
#else
#define __COMMON_ASRE   RWWSRE
#endif

/* Define the bit positions of the Boot Lock Bits. */

#define BLB12           5
#define BLB11           4
#define BLB02           3
#define BLB01           2

/** \ingroup avr_boot
    \def boot_spm_interrupt_enable()
    Enable the SPM interrupt. */

#define boot_spm_interrupt_enable()   (__SPM_REG |= (uint8_t)_BV(SPMIE))

/** \ingroup avr_boot
    \def boot_spm_interrupt_disable()
    Disable the SPM interrupt. */

#define boot_spm_interrupt_disable()  (__SPM_REG &= (uint8_t)~_BV(SPMIE))

/** \ingroup avr_boot
    \def boot_is_spm_interrupt()
    Check if the SPM interrupt is enabled. */

#define boot_is_spm_interrupt()       (__SPM_REG & (uint8_t)_BV(SPMIE))

/** \ingroup avr_boot
    \def boot_rww_busy()
    Check if the RWW section is busy. */

#define boot_rww_busy()          (__SPM_REG & (uint8_t)_BV(__COMMON_ASB))

/** \ingroup avr_boot
    \def boot_spm_busy()
    Check if the SPM instruction is busy. */

#define boot_spm_busy()               (__SPM_REG & (uint8_t)_BV(__SPM_ENABLE))

/** \ingroup avr_boot
    \def boot_spm_busy_wait()
    Wait while the SPM instruction is busy. */

#define boot_spm_busy_wait()          do{}while(boot_spm_busy())

#define __BOOT_PAGE_ERASE         (_BV(__SPM_ENABLE) | _BV(PGERS))
#define __BOOT_PAGE_WRITE         (_BV(__SPM_ENABLE) | _BV(PGWRT))
#define __BOOT_PAGE_FILL          _BV(__SPM_ENABLE)
#define __BOOT_RWW_ENABLE         (_BV(__SPM_ENABLE) | _BV(__COMMON_ASRE))
#define __BOOT_LOCK_BITS_SET      (_BV(__SPM_ENABLE) | _BV(BLBSET))

#define __boot_page_fill_short(address, data)   \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw  r0, %3\n\t"                       \
        "out %0, %1\n\t"                         \
        "spm\n\t"                                \
        "clr  r1\n\t"                            \
        :                                        \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_PAGE_FILL),       \
          "z" ((uint16_t)address),               \
          "r" ((uint16_t)data)                   \
        : "r0"                                   \
    );                                           \
}))

#define __boot_page_fill_normal(address, data)   \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw  r0, %3\n\t"                       \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
        "clr  r1\n\t"                            \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_PAGE_FILL),       \
          "z" ((uint16_t)address),               \
          "r" ((uint16_t)data)                   \
        : "r0"                                   \
    );                                           \
}))

#define __boot_page_fill_alternate(address, data)\
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw  r0, %3\n\t"                       \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
        ".word 0xffff\n\t"                       \
        "nop\n\t"                                \
        "clr  r1\n\t"                            \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_PAGE_FILL),       \
          "z" ((uint16_t)address),               \
          "r" ((uint16_t)data)                   \
        : "r0"                                   \
    );                                           \
}))

#define __boot_page_fill_extended(address, data) \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw  r0, %4\n\t"                       \
        "movw r30, %A3\n\t"                      \
        "sts %1, %C3\n\t"                        \
        "sts %0, %2\n\t"                         \
        "spm\n\t"                                \
        "clr  r1\n\t"                            \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "i" (_SFR_MEM_ADDR(RAMPZ)),            \
          "r" ((uint8_t)__BOOT_PAGE_FILL),       \
          "r" ((uint32_t)address),               \
          "r" ((uint16_t)data)                   \
        : "r0", "r30", "r31"                     \
    );                                           \
}))

#define __boot_page_fill_extended_short(address, data) \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw  r0, %4\n\t"                       \
        "movw r30, %A3\n\t"                      \
        "out %1, %C3\n\t"                        \
        "out %0, %2\n\t"                         \
        "spm\n\t"                                \
        "clr  r1\n\t"                            \
        :                                        \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),        \
          "i" (_SFR_IO_ADDR(RAMPZ)),            \
          "r" ((uint8_t)__BOOT_PAGE_FILL),       \
          "r" ((uint32_t)address),               \
          "r" ((uint16_t)data)                   \
        : "r0", "r30", "r31"                     \
    );                                           \
}))

#define __boot_page_erase_short(address)        \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "out %0, %1\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_PAGE_ERASE),      \
          "z" ((uint16_t)address)                \
    );                                           \
}))


#define __boot_page_erase_normal(address)        \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_PAGE_ERASE),      \
          "z" ((uint16_t)address)                \
    );                                           \
}))

#define __boot_page_erase_alternate(address)     \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
        ".word 0xffff\n\t"                       \
        "nop\n\t"                                \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_PAGE_ERASE),      \
          "z" ((uint16_t)address)                \
    );                                           \
}))

#define __boot_page_erase_extended(address)      \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw r30, %A3\n\t"                      \
        "sts  %1, %C3\n\t"                       \
        "sts %0, %2\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "i" (_SFR_MEM_ADDR(RAMPZ)),            \
          "r" ((uint8_t)__BOOT_PAGE_ERASE),      \
          "r" ((uint32_t)address)                \
        : "r30", "r31"                           \
    );                                           \
}))
#define __boot_page_erase_extended_short(address)      \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw r30, %A3\n\t"                      \
        "out  %1, %C3\n\t"                       \
        "out %0, %2\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),        \
          "i" (_SFR_IO_ADDR(RAMPZ)),            \
          "r" ((uint8_t)__BOOT_PAGE_ERASE),      \
          "r" ((uint32_t)address)                \
        : "r30", "r31"                           \
    );                                           \
}))

#define __boot_page_write_short(address)        \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "out %0, %1\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_PAGE_WRITE),      \
          "z" ((uint16_t)address)                \
    );                                           \
}))

#define __boot_page_write_normal(address)        \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_PAGE_WRITE),      \
          "z" ((uint16_t)address)                \
    );                                           \
}))

#define __boot_page_write_alternate(address)     \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
        ".word 0xffff\n\t"                       \
        "nop\n\t"                                \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_PAGE_WRITE),      \
          "z" ((uint16_t)address)                \
    );                                           \
}))

#define __boot_page_write_extended(address)      \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw r30, %A3\n\t"                      \
        "sts %1, %C3\n\t"                        \
        "sts %0, %2\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "i" (_SFR_MEM_ADDR(RAMPZ)),            \
          "r" ((uint8_t)__BOOT_PAGE_WRITE),      \
          "r" ((uint32_t)address)                \
        : "r30", "r31"                           \
    );                                           \
}))
#define __boot_page_write_extended_short(address)      \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "movw r30, %A3\n\t"                      \
        "out %1, %C3\n\t"                        \
        "out %0, %2\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),        \
          "i" (_SFR_IO_ADDR(RAMPZ)),            \
          "r" ((uint8_t)__BOOT_PAGE_WRITE),      \
          "r" ((uint32_t)address)                \
        : "r30", "r31"                           \
    );                                           \
}))

#define __boot_rww_enable_short()                      \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "out %0, %1\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_RWW_ENABLE)       \
    );                                           \
}))

#define __boot_rww_enable()                      \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_RWW_ENABLE)       \
    );                                           \
}))

#define __boot_rww_enable_alternate()            \
(__extension__({                                 \
    __asm__ __volatile__                         \
    (                                            \
        "sts %0, %1\n\t"                         \
        "spm\n\t"                                \
        ".word 0xffff\n\t"                       \
        "nop\n\t"                                \
        :                                        \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),        \
          "r" ((uint8_t)__BOOT_RWW_ENABLE)       \
    );                                           \
}))

/* From the mega16/mega128 data sheets (maybe others):

     Bits by SPM To set the Boot Loader Lock bits, write the desired data to
     R0, write "X0001001" to SPMCR and execute SPM within four clock cycles
     after writing SPMCR. The only accessible Lock bits are the Boot Lock bits
     that may prevent the Application and Boot Loader section from any
     software update by the MCU.

     If bits 5..2 in R0 are cleared (zero), the corresponding Boot Lock bit
     will be programmed if an SPM instruction is executed within four cycles
     after BLBSET and SPMEN (or SELFPRGEN) are set in SPMCR. The Z-pointer is 
     don't care during this operation, but for future compatibility it is 
     recommended to load the Z-pointer with $0001 (same as used for reading the 
     Lock bits). For future compatibility It is also recommended to set bits 7, 
     6, 1, and 0 in R0 to 1 when writing the Lock bits. When programming the 
     Lock bits the entire Flash can be read during the operation. */

#define __boot_lock_bits_set_short(lock_bits)                    \
(__extension__({                                           \
    uint8_t value = (uint8_t)(~(lock_bits));               \
    __asm__ __volatile__                                   \
    (                                                      \
        "ldi r30, 1\n\t"                                   \
        "ldi r31, 0\n\t"                                   \
        "mov r0, %2\n\t"                                   \
        "out %0, %1\n\t"                                   \
        "spm\n\t"                                          \
        :                                                  \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),                  \
          "r" ((uint8_t)__BOOT_LOCK_BITS_SET),             \
          "r" (value)                                      \
        : "r0", "r30", "r31"                               \
    );                                                     \
}))

#define __boot_lock_bits_set(lock_bits)                    \
(__extension__({                                           \
    uint8_t value = (uint8_t)(~(lock_bits));               \
    __asm__ __volatile__                                   \
    (                                                      \
        "ldi r30, 1\n\t"                                   \
        "ldi r31, 0\n\t"                                   \
        "mov r0, %2\n\t"                                   \
        "sts %0, %1\n\t"                                   \
        "spm\n\t"                                          \
        :                                                  \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),                  \
          "r" ((uint8_t)__BOOT_LOCK_BITS_SET),             \
          "r" (value)                                      \
        : "r0", "r30", "r31"                               \
    );                                                     \
}))

#define __boot_lock_bits_set_alternate(lock_bits)          \
(__extension__({                                           \
    uint8_t value = (uint8_t)(~(lock_bits));               \
    __asm__ __volatile__                                   \
    (                                                      \
        "ldi r30, 1\n\t"                                   \
        "ldi r31, 0\n\t"                                   \
        "mov r0, %2\n\t"                                   \
        "sts %0, %1\n\t"                                   \
        "spm\n\t"                                          \
        ".word 0xffff\n\t"                                 \
        "nop\n\t"                                          \
        :                                                  \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),                  \
          "r" ((uint8_t)__BOOT_LOCK_BITS_SET),             \
          "r" (value)                                      \
        : "r0", "r30", "r31"                               \
    );                                                     \
}))

/*
   Reading lock and fuse bits:

     Similarly to writing the lock bits above, set BLBSET and SPMEN (or 
     SELFPRGEN) bits in __SPMREG, and then (within four clock cycles) issue an 
     LPM instruction.

     Z address:       contents:
     0x0000           low fuse bits
     0x0001           lock bits
     0x0002           extended fuse bits
     0x0003           high fuse bits

     Sounds confusing, doesn't it?

     Unlike the macros in pgmspace.h, no need to care for non-enhanced
     cores here as these old cores do not provide SPM support anyway.
 */

/** \ingroup avr_boot
    \def GET_LOW_FUSE_BITS
    address to read the low fuse bits, using boot_lock_fuse_bits_get
 */
#define GET_LOW_FUSE_BITS           (0x0000)
/** \ingroup avr_boot
    \def GET_LOCK_BITS
    address to read the lock bits, using boot_lock_fuse_bits_get
 */
#define GET_LOCK_BITS               (0x0001)
/** \ingroup avr_boot
    \def GET_EXTENDED_FUSE_BITS
    address to read the extended fuse bits, using boot_lock_fuse_bits_get
 */
#define GET_EXTENDED_FUSE_BITS      (0x0002)
/** \ingroup avr_boot
    \def GET_HIGH_FUSE_BITS
    address to read the high fuse bits, using boot_lock_fuse_bits_get
 */
#define GET_HIGH_FUSE_BITS          (0x0003)

/** \ingroup avr_boot
    \def boot_lock_fuse_bits_get(address)

    Read the lock or fuse bits at \c address.

    Parameter \c address can be any of GET_LOW_FUSE_BITS,
    GET_LOCK_BITS, GET_EXTENDED_FUSE_BITS, or GET_HIGH_FUSE_BITS.

    \note The lock and fuse bits returned are the physical values,
    i.e. a bit returned as 0 means the corresponding fuse or lock bit
    is programmed.
 */
#define boot_lock_fuse_bits_get_short(address)                   \
(__extension__({                                           \
    uint8_t __result;                                      \
    __asm__ __volatile__                                   \
    (                                                      \
        "ldi r30, %3\n\t"                                  \
        "ldi r31, 0\n\t"                                   \
        "out %1, %2\n\t"                                   \
        "lpm %0, Z\n\t"                                    \
        : "=r" (__result)                                  \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),                  \
          "r" ((uint8_t)__BOOT_LOCK_BITS_SET),             \
          "M" (address)                                    \
        : "r0", "r30", "r31"                               \
    );                                                     \
    __result;                                              \
}))

#define boot_lock_fuse_bits_get(address)                   \
(__extension__({                                           \
    uint8_t __result;                                      \
    __asm__ __volatile__                                   \
    (                                                      \
        "ldi r30, %3\n\t"                                  \
        "ldi r31, 0\n\t"                                   \
        "sts %1, %2\n\t"                                   \
        "lpm %0, Z\n\t"                                    \
        : "=r" (__result)                                  \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),                  \
          "r" ((uint8_t)__BOOT_LOCK_BITS_SET),             \
          "M" (address)                                    \
        : "r0", "r30", "r31"                               \
    );                                                     \
    __result;                                              \
}))

/** \ingroup avr_boot
    \def boot_signature_byte_get(address)

    Read the Signature Row byte at \c address.  For some MCU types,
    this function can also retrieve the factory-stored oscillator
    calibration bytes.

    Parameter \c address can be 0-0x1f as documented by the datasheet.
    \note The values are MCU type dependent.
*/

#define __BOOT_SIGROW_READ (_BV(__SPM_ENABLE) | _BV(SIGRD))

#define boot_signature_byte_get_short(addr) \
(__extension__({                      \
      uint16_t __addr16 = (uint16_t)(addr);     \
      uint8_t __result;                         \
      __asm__ __volatile__                      \
      (                                         \
        "out %1, %2\n\t"                        \
        "lpm %0, Z" "\n\t"                      \
        : "=r" (__result)                       \
        : "i" (_SFR_IO_ADDR(__SPM_REG)),        \
          "r" ((uint8_t) __BOOT_SIGROW_READ),   \
          "z" (__addr16)                        \
      );                                        \
      __result;                                 \
}))

#define boot_signature_byte_get(addr) \
(__extension__({                      \
      uint16_t __addr16 = (uint16_t)(addr);     \
      uint8_t __result;                         \
      __asm__ __volatile__                      \
      (                                         \
        "sts %1, %2\n\t"                        \
        "lpm %0, Z" "\n\t"                      \
        : "=r" (__result)                       \
        : "i" (_SFR_MEM_ADDR(__SPM_REG)),       \
          "r" ((uint8_t) __BOOT_SIGROW_READ),   \
          "z" (__addr16)                        \
      );                                        \
      __result;                                 \
}))

/** \ingroup avr_boot
    \def boot_page_fill(address, data)

    Fill the bootloader temporary page buffer for flash 
    address with data word. 

    \note The address is a byte address. The data is a word. The AVR 
    writes data to the buffer a word at a time, but addresses the buffer
    per byte! So, increment your address by 2 between calls, and send 2
    data bytes in a word format! The LSB of the data is written to the lower 
    address; the MSB of the data is written to the higher address.*/

/** \ingroup avr_boot
    \def boot_page_erase(address)

    Erase the flash page that contains address.

    \note address is a byte address in flash, not a word address. */

/** \ingroup avr_boot
    \def boot_page_write(address)

    Write the bootloader temporary page buffer 
    to flash page that contains address.
    
    \note address is a byte address in flash, not a word address. */

/** \ingroup avr_boot
    \def boot_rww_enable()

    Enable the Read-While-Write memory section. */

/** \ingroup avr_boot
    \def boot_lock_bits_set(lock_bits)

    Set the bootloader lock bits.

    \param lock_bits A mask of which Boot Loader Lock Bits to set.

    \note In this context, a 'set bit' will be written to a zero value.
    Note also that only BLBxx bits can be programmed by this command.

    For example, to disallow the SPM instruction from writing to the Boot
    Loader memory section of flash, you would use this macro as such:

    \code
    boot_lock_bits_set (_BV (BLB11));
    \endcode

    \note Like any lock bits, the Boot Loader Lock Bits, once set,
    cannot be cleared again except by a chip erase which will in turn
    also erase the boot loader itself. */

/* Normal versions of the macros use 16-bit addresses.
   Extended versions of the macros use 32-bit addresses.
   Alternate versions of the macros use 16-bit addresses and require special
   instruction sequences after LPM.

   FLASHEND is defined in the ioXXXX.h file.
   USHRT_MAX is defined in <limits.h>. */ 

#if defined(__AVR_ATmega161__) || defined(__AVR_ATmega163__) \
    || defined(__AVR_ATmega323__)

/* Alternate: ATmega161/163/323 and 16 bit address */
#define boot_page_fill(address, data) __boot_page_fill_alternate(address, data)
#define boot_page_erase(address)      __boot_page_erase_alternate(address)
#define boot_page_write(address)      __boot_page_write_alternate(address)
#define boot_rww_enable()             __boot_rww_enable_alternate()
#define boot_lock_bits_set(lock_bits) __boot_lock_bits_set_alternate(lock_bits)

#elif (FLASHEND > USHRT_MAX)

/* Extended: >16 bit address */
#define boot_page_fill(address, data) __boot_page_fill_extended_short(address, data)
#define boot_page_erase(address)      __boot_page_erase_extended_short(address)
#define boot_page_write(address)      __boot_page_write_extended_short(address)
#define boot_rww_enable()             __boot_rww_enable_short()
#define boot_lock_bits_set(lock_bits) __boot_lock_bits_set_short(lock_bits)

#else

/* Normal: 16 bit address */
#define boot_page_fill(address, data) __boot_page_fill_short(address, data)
#define boot_page_erase(address)      __boot_page_erase_short(address)
#define boot_page_write(address)      __boot_page_write_short(address)
#define boot_rww_enable()             __boot_rww_enable_short()
#define boot_lock_bits_set(lock_bits) __boot_lock_bits_set_short(lock_bits)

#endif

/** \ingroup avr_boot

    Same as boot_page_fill() except it waits for eeprom and spm operations to
    complete before filling the page. */

#define boot_page_fill_safe(address, data) \
do { \
    boot_spm_busy_wait();                       \
    eeprom_busy_wait();                         \
    boot_page_fill(address, data);              \
} while (0)

/** \ingroup avr_boot

    Same as boot_page_erase() except it waits for eeprom and spm operations to
    complete before erasing the page. */

#define boot_page_erase_safe(address) \
do { \
    boot_spm_busy_wait();                       \
    eeprom_busy_wait();                         \
    boot_page_erase (address);                  \
} while (0)

/** \ingroup avr_boot

    Same as boot_page_write() except it waits for eeprom and spm operations to
    complete before writing the page. */

#define boot_page_write_safe(address) \
do { \
    boot_spm_busy_wait();                       \
    eeprom_busy_wait();                         \
    boot_page_write (address);                  \
} while (0)

/** \ingroup avr_boot

    Same as boot_rww_enable() except waits for eeprom and spm operations to
    complete before enabling the RWW mameory. */

#define boot_rww_enable_safe() \
do { \
    boot_spm_busy_wait();                       \
    eeprom_busy_wait();                         \
    boot_rww_enable();                          \
} while (0)

/** \ingroup avr_boot

    Same as boot_lock_bits_set() except waits for eeprom and spm operations to
    complete before setting the lock bits. */

#define boot_lock_bits_set_safe(lock_bits) \
do { \
    boot_spm_busy_wait();                       \
    eeprom_busy_wait();                         \
    boot_lock_bits_set (lock_bits);             \
} while (0)

#endif /* _AVR_BOOT_H_ */