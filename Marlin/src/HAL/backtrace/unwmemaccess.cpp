/***************************************************************************
 * ARM Stack Unwinder, Michael.McTernan.2001@cs.bris.ac.uk
 * Updated, adapted and several bug fixes on 2018 by Eduardo José Tagle
 *
 * This program is PUBLIC DOMAIN.
 * This means that there is no copyright and anyone is able to take a copy
 * for free and use it as they wish, with or without modifications, and in
 * any context, commercially or otherwise. The only limitation is that I
 * don't guarantee that the software is fit for any purpose or accept any
 * liability for it's use or misuse - this software is without warranty.
 ***************************************************************************
 * File Description: Utility functions to access memory
 **************************************************************************/

#if defined(__arm__) || defined(__thumb__)

#include "unwmemaccess.h"

/* Validate address */

#ifdef ARDUINO_ARCH_SAM
// For DUE, valid address ranges are
//  SRAM  (0x20070000 - 0x20088000) (96kb)
//  FLASH (0x00080000 - 0x00100000) (512kb)
//
#define START_SRAM_ADDR   0x20070000
#define END_SRAM_ADDR     0x20088000
#define START_FLASH_ADDR  0x00080000
#define END_FLASH_ADDR    0x00100000
#endif

#ifdef TARGET_LPC1768
// For LPC1769:
//  SRAM  (0x10000000 - 0x10008000) (32kb)
//  FLASH (0x00000000 - 0x00080000) (512kb)
//
#define START_SRAM_ADDR   0x10000000
#define END_SRAM_ADDR     0x10008000
#define START_FLASH_ADDR  0x00000000
#define END_FLASH_ADDR    0x00080000
#endif

#if 0
// For STM32F103CBT6
//  SRAM  (0x20000000 - 0x20005000) (20kb)
//  FLASH (0x00000000 - 0x00020000) (128kb)
//
#define START_SRAM_ADDR   0x20000000
#define END_SRAM_ADDR     0x20005000
#define START_FLASH_ADDR  0x00000000
#define END_FLASH_ADDR    0x00020000
#endif

#ifdef __STM32F1__
// For STM32F103ZET6/STM32F103VET6
//  SRAM  (0x20000000 - 0x20010000) (64kb)
//  FLASH (0x00000000 - 0x00080000) (512kb)
//
#define START_SRAM_ADDR   0x20000000
#define END_SRAM_ADDR     0x20010000
#define START_FLASH_ADDR  0x00000000
#define END_FLASH_ADDR    0x00080000
#endif

#ifdef STM32F4
// For STM32F407VET
//  SRAM  (0x20000000 - 0x20030000) (192kb)
//  FLASH (0x08000000 - 0x08080000) (512kb)
//
#define START_SRAM_ADDR   0x20000000
#define END_SRAM_ADDR     0x20030000
#define START_FLASH_ADDR  0x08000000
#define END_FLASH_ADDR    0x08080000
#endif

#ifdef STM32F7
// For STM32F765 in BORG
//  SRAM  (0x20000000 - 0x20080000) (512kb)
//  FLASH (0x08000000 - 0x08100000) (1024kb)
//
#define START_SRAM_ADDR   0x20000000
#define END_SRAM_ADDR     0x20080000
#define START_FLASH_ADDR  0x08000000
#define END_FLASH_ADDR    0x08100000
#endif

#ifdef __MK64FX512__
// For MK64FX512 in TEENSY 3.5
//  SRAM  (0x1FFF0000 - 0x20020000) (192kb)
//  FLASH (0x00000000 - 0x00080000) (512kb)
//
#define START_SRAM_ADDR   0x1FFF0000
#define END_SRAM_ADDR     0x20020000
#define START_FLASH_ADDR  0x00000000
#define END_FLASH_ADDR    0x00080000
#endif

#ifdef __MK66FX1M0__
// For MK66FX1M0 in TEENSY 3.6
//  SRAM  (0x1FFF0000 - 0x20030000) (256kb)
//  FLASH (0x00000000 - 0x00140000) (1.25Mb)
//
#define START_SRAM_ADDR   0x1FFF0000
#define END_SRAM_ADDR     0x20030000
#define START_FLASH_ADDR  0x00000000
#define END_FLASH_ADDR    0x00140000
#endif

static bool validate_addr(uint32_t addr) {

  // Address must be in SRAM range
  if (addr >= START_SRAM_ADDR && addr < END_SRAM_ADDR)
    return true;

  // Or in FLASH range
  if (addr >= START_FLASH_ADDR && addr < END_FLASH_ADDR)
    return true;

  return false;
}

bool UnwReadW(const uint32_t a, uint32_t *v) {
  if (!validate_addr(a))
    return false;

  *v = *(uint32_t *)a;
  return true;
}

bool UnwReadH(const uint32_t a, uint16_t *v) {
  if (!validate_addr(a))
    return false;

  *v = *(uint16_t *)a;
  return true;
}

bool UnwReadB(const uint32_t a, uint8_t *v) {
  if (!validate_addr(a))
    return false;

  *v = *(uint8_t *)a;
  return true;
}

#endif

