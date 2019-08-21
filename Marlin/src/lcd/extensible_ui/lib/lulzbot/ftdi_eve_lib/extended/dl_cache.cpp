/****************
 * dl_cache.cpp *
 ****************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "ftdi_extended.h"

#ifdef FTDI_EXTENDED

/* The Display List Cache mechanism stores the display list corresponding
 * to a menu into RAM_G so that on subsequent calls drawing the menu does
 * not require as much SPI traffic.
 *
 * Layout of Cache memory:
 *
 * The cache memory begins with a table at
 * DL_CACHE_START: each table entry contains
 * an address and size for a cached DL slot.
 *
 * Immediately following the table is the
 * DL_FREE_ADDR, which points to free cache
 * space; following this is occupied DL space,
 * and after that free space that is yet to
 * be used.
 *
 *  location        data        sizeof
 *
 *  DL_CACHE_START  slot0_addr     4
 *                  slot0_size     4
 *                  slot1_addr     4
 *                  slot1_size     4
 *                      ...
 *                  slotN_addr     4
 *                  slotN_size     4
 *  DL_FREE_ADDR    dl_free_ptr    4
 *                  cached data
 *                      ...
 *  dl_free_ptr     empty space
 *                      ...
 */

#define DL_CACHE_START   MAP::RAM_G_SIZE - 0xFFFF
#define DL_FREE_ADDR     DL_CACHE_START + DL_CACHE_SLOTS * 8

using namespace FTDI;

// The init function ensures all cache locations are marked as empty

void DLCache::init() {
  CLCD::mem_write_32(DL_FREE_ADDR, DL_FREE_ADDR + 4);
  for(uint8_t slot = 0; slot < DL_CACHE_SLOTS; slot++) {
    save_slot(slot, 0, 0);
  }
}

bool DLCache::has_data() {
  return dl_size != 0;
}

bool DLCache::wait_until_idle() {
  const unsigned long startTime = millis();
  do {
    if ((millis() - startTime) > 250) {
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM("Timeout on DL_Cache::Wait_Until_Idle()");
      CLCD::CommandFifo::reset();
      return false;
    }
    #ifdef __MARLIN_FIRMWARE__
      ExtUI::yield();
    #endif
  } while (CLCD::CommandFifo::is_processing());
  return true;
}

/* This caches the current display list in RAMG so
 * that it can be appended later. The memory is
 * dynamically allocated following DL_FREE_ADDR.
 *
 * If num_bytes is provided, then that many bytes
 * will be reserved so that the cache may be re-written
 * later with potentially a bigger DL.
 */

bool DLCache::store(uint32_t num_bytes /* = 0*/) {
  CLCD::CommandFifo cmd;

  // Execute any commands already in the FIFO
  cmd.execute();
  if (!wait_until_idle())
    return false;

  // Figure out how long the display list is
  uint32_t new_dl_size = CLCD::mem_read_32(REG::CMD_DL) & 0x1FFF;
  uint32_t free_space  = 0;
  uint32_t dl_alloc    = 0;

  if (dl_addr == 0) {
    // If we are allocating new space...
    dl_addr     = CLCD::mem_read_32(DL_FREE_ADDR);
    free_space  = MAP::RAM_G_SIZE - dl_addr;
    dl_alloc    = num_bytes ? num_bytes : new_dl_size;
    dl_size     = new_dl_size;
  } else {
    // Otherwise, we can only store as much space
    // as was previously allocated.
    free_space  = num_bytes ? num_bytes : dl_size;
    dl_alloc    = 0;
    dl_size     = new_dl_size;
  }

  if (dl_size > free_space) {
    // Not enough memory to cache the display list.
    #ifdef UI_FRAMEWORK_DEBUG
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR("Not enough space in GRAM to cache display list, free space: ", free_space);
      SERIAL_ECHOLNPAIR(" Required: ", dl_size);
    #endif
    return false;
  } else {
    #ifdef UI_FRAMEWORK_DEBUG
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR("Saving DL to RAMG cache, bytes: ", dl_size);
      SERIAL_ECHOLNPAIR(" Free space: ", free_space);
    #endif
    cmd.memcpy(dl_addr, MAP::RAM_DL, dl_size);
    cmd.execute();
    save_slot(dl_slot, dl_addr, dl_size);
    if (dl_alloc > 0) {
      // If we allocated space dynamically, then adjust dl_free_addr.
      CLCD::mem_write_32(DL_FREE_ADDR, dl_addr + dl_alloc);
    }
    return true;
  }
}

void DLCache::save_slot(uint8_t dl_slot, uint32_t dl_addr, uint32_t dl_size) {
  CLCD::mem_write_32(DL_CACHE_START + dl_slot * 8 + 0, dl_addr);
  CLCD::mem_write_32(DL_CACHE_START + dl_slot * 8 + 4, dl_size);
}

void DLCache::load_slot() {
  dl_addr  = CLCD::mem_read_32(DL_CACHE_START + dl_slot * 8 + 0);
  dl_size  = CLCD::mem_read_32(DL_CACHE_START + dl_slot * 8 + 4);
}

void DLCache::append() {
  CLCD::CommandFifo cmd;
  cmd.append(dl_addr, dl_size);
  #ifdef UI_FRAMEWORK_DEBUG
    cmd.execute();
    wait_until_idle();
    SERIAL_ECHO_START();
    SERIAL_ECHOPAIR("Appending to DL from RAMG cache, bytes: ", dl_size);
    SERIAL_ECHOLNPAIR(" REG_CMD_DL: ", CLCD::mem_read_32(REG::CMD_DL));
  #endif
}

#endif // FTDI_EXTENDED
