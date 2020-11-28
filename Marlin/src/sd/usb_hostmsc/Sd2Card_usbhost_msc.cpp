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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(USB_HOST_MSC_FLASH_SUPPORT)

#include "Sd2Card_usbhost_msc.h"
#include HAL_PATH(../../HAL, usb_hostlib/usbh_usr.h)

#include "../../MarlinCore.h"
#include "../../core/serial.h"
#include "../../module/temperature.h"

// idle() is always called in Marlin idle()
void Sd2Card::idle() {

  // Hareware IO init once time after startup
  static uint8_t firstCall = 1;
  if (firstCall) {
    USBH_HAL_Init();
    firstCall = 0;
  }

  USB_HAL_Loop();
}

// Marlin calls this function to check whether an USB drive is inserted.
// This is equivalent to polling the SD_DETECT when using SD cards.
bool Sd2Card::isInserted() {
  return USBH_IsConnected();
}

// Marlin calls this to initialize an SD card once it is inserted.
bool Sd2Card::init(const uint8_t, const pin_t) {
  return isInserted();
}

bool Sd2Card::readBlock(uint32_t block, uint8_t* dst) {
  if (!isInserted()) return false;

  return (USBH_read(0, dst, block, 1) == 0);
}

bool Sd2Card::writeBlock(uint32_t block, const uint8_t* src) {
  if (!isInserted()) return false;

  return (USBH_write(0, src, block, 1) == 0);
}

#endif // USB_HOST_MSC_FLASH_SUPPORT
