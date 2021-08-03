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
#pragma once

#include <stdint.h>

typedef enum {
  USB_STATE_INIT,
  USB_STATE_ERROR,
  USB_STATE_RUNNING,
} usb_state_t;

class USBHost {
public:
  bool start();
  void Task();
  uint8_t getUsbTaskState();
  void setUsbTaskState(uint8_t state);
  uint8_t regRd(uint8_t reg) { return 0x0; };
  uint8_t usb_task_state = USB_STATE_INIT;
  uint8_t lun = 0;
  uint32_t capacity = 0;
  uint16_t block_size = 0;
  uint32_t block_count = 0;
};

class BulkStorage {
public:
  BulkStorage(USBHost *usb) : usb(usb) {};

  bool LUNIsGood(uint8_t t);
  uint32_t GetCapacity(uint8_t lun);
  uint16_t GetSectorSize(uint8_t lun);
  uint8_t Read(uint8_t lun, uint32_t addr, uint16_t bsize, uint8_t blocks, uint8_t *buf);
  uint8_t Write(uint8_t lun, uint32_t addr, uint16_t bsize, uint8_t blocks, const uint8_t * buf);

  USBHost *usb;
};

extern USBHost usb;
extern BulkStorage bulk;
