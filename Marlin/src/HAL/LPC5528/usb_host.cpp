/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef TARGET_LPC5528

#include "../../inc/MarlinConfig.h"

#if ALL(USE_OTG_USB_HOST, USBHOST)

#include "usb_host.h"
#include "usb_msc.h"
#include "../shared/Marduino.h"
#include "masstorage1.h"
USBHost usb;
BulkStorage bulk(&usb);

bool USBHost::start() {
  udisk.Init(0, 0, false);
  return true;
}

void USBHost::Task() {
  hs_usb_loop();
}

uint8_t USBHost::getUsbTaskState() {
  if (udisk.LUNIsGood(0))
    usb.setUsbTaskState(USB_STATE_RUNNING);

  return u sb_task_state;
}

bool BulkStorage::LUNIsGood(uint8_t t) {
  return (udisk.LUNIsGood(0));
}

void USBHost::setUsbTaskState(uint8_t state) {
  usb_task_state = state;
  if (usb_task_state == USB_STATE_RUNNING) {
    capacity = capacity*block_size;
    block_size = udisk.GetSectorSize(0);
    block_count = udisk.GetCapacity(0);
  }
}

uint32_t BulkStorage::GetCapacity(uint8_t lun) {
  return usb->block_count;
}

uint16_t BulkStorage::GetSectorSize(uint8_t lun) {
  return 512;
}

uint8_t BulkStorage::Read(uint8_t lun, uint32_t addr, uint16_t bsize, uint8_t blocks, uint8_t *buf) {
  return udisk.Read(lun, addr, bsize, blocks, buf);
}

uint8_t BulkStorage::Write(uint8_t lun, uint32_t addr, uint16_t bsize, uint8_t blocks, const uint8_t * buf) {
  return udisk.Write(lun, addr, bsize, blocks, buf);
}

#endif // USE_OTG_USB_HOST && USBHOST
#endif // TARGET_LPC5528
