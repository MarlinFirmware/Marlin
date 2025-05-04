/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../platforms.h"

#ifdef __PLAT_RP2040__

#include "../../inc/MarlinConfigPre.h"

#if HAS_SD_HOST_DRIVE

#include "../../sd/cardreader.h"

#include <tusb.h>  // TinyUSB device stack

#define BLOCK_SIZE 512
#define SD_MULTIBLOCK_RETRY_CNT 1

DiskIODriver* diskIODriver() {
  #if HAS_MULTI_VOLUME
    #if SHARED_VOLUME_IS(SD_ONBOARD)
      return &card.media_driver_sdcard;
    #elif SHARED_VOLUME_IS(USB_FLASH_DRIVE)
      return &card.media_driver_usbFlash;
    #endif
  #else
    return card.diskIODriver();
  #endif
}

/** Callbacks used by TinyUSB MSC **/

extern "C" {

bool tud_msc_ready_cb(uint8_t lun) {
  return diskIODriver()->isReady();
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, void* buffer, uint32_t bufsize) {
  const uint32_t blocks = bufsize / BLOCK_SIZE;
  for (uint16_t rcount = SD_MULTIBLOCK_RETRY_CNT; rcount--; ) {
    if (diskIODriver()->readBlocks(lba, (uint8_t*)buffer, blocks))
      return bufsize; // Success
  }
  return -1; // Failure after retries
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint8_t const* buffer, uint32_t bufsize) {
  const uint32_t blocks = bufsize / BLOCK_SIZE;
  for (uint16_t rcount = SD_MULTIBLOCK_RETRY_CNT; rcount--; ) {
    if (diskIODriver()->writeBlocks(lba, buffer, blocks))
      return bufsize; // Success
  }
  return -1; // Failure after retries
}

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
  memcpy(vendor_id,  "MARLIN  ", 8);
  memcpy(product_id, "Product       ", 16);
  memcpy(product_rev, "0.01", 4);
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size) {
  *block_count = diskIODriver()->cardSize();
  *block_size = BLOCK_SIZE;
}

void tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
  if (load_eject) {
    if (start) {
      // Handle media load
    } else {
      // Handle media eject
    }
  }
}

} // extern "C"

void MSC_SD_init() {
  tusb_init();
  // Add USB reinitialization logic if needed
}

#endif // HAS_SD_HOST_DRIVE
#endif // __PLAT_RP2040__
