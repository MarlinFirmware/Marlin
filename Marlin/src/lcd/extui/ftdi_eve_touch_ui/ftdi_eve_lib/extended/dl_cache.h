/**************
 * dl_cache.h *
 **************/

/****************************************************************************
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

/******************* DISPLAY LIST CACHE MANAGEMENT ************************/
/* The Display List Cache mechanism stores the display list corresponding
 * to a menu into RAM_G so that on subsequent calls drawing the menu does
 * not require as much SPI traffic. Dynamic content, such as indicators,
 * should not be cached.
 *
 * The DLCache can be used like so:
 *
 *   DLCache dlcache(UNIQUE_ID);
 *
 *   if (dlcache.hasData())
 *     dlcache.append();
 *   else
 *     dlcache.store(); // Add stuff to the DL
 */
class DLCache {
  private:
    typedef FTDI::ftdi_registers  REG;
    typedef FTDI::ftdi_memory_map MAP;

    uint8_t  dl_slot_indx;
    uint32_t dl_slot_addr;
    uint16_t dl_slot_size;
    uint16_t dl_slot_used;

    void load_slot() {load_slot(dl_slot_indx, dl_slot_addr, dl_slot_size, dl_slot_used);}
    void save_slot() {save_slot(dl_slot_indx, dl_slot_addr, dl_slot_size, dl_slot_used);}

    static void load_slot(uint8_t indx, uint32_t &addr, uint16_t &size, uint16_t &used);
    static void save_slot(uint8_t indx, uint32_t  addr, uint16_t  size, uint16_t  used);

    bool wait_until_idle();

  public:
    static void init();

    DLCache(uint8_t slot) {
      dl_slot_indx = slot;
      load_slot();
    }

    bool has_data();
    bool store(uint32_t min_bytes = 0);
    void append();
};

#define DL_CACHE_SLOTS   250
