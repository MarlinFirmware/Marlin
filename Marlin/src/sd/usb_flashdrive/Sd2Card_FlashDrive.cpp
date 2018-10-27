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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(USB_FLASH_DRIVE_SUPPORT)

#include "lib/Usb.h"
#include "lib/masstorage.h"

#include "Sd2Card_FlashDrive.h"

#include <SPI.h>

#include "../../core/serial.h"

USB usb;
BulkOnly bulk(&usb);

Sd2Card::state_t Sd2Card::state;
uint32_t         Sd2Card::block;

bool Sd2Card::usbHostReady() {
  return state == USB_HOST_INITIALIZED;
}

bool Sd2Card::isInserted() {
  return usb.getUsbTaskState() == USB_STATE_RUNNING;
}

// Marlin calls this whenever an SD card is detected, so this method
// should not be used to initialize the USB host library
bool Sd2Card::init(uint8_t sckRateID, uint8_t chipSelectPin) {
  if (!usbHostReady()) return false;

  if (!bulk.LUNIsGood(0)) {
    SERIAL_ECHOLNPGM("LUN zero is not good\n");
    return false;
  }

  SERIAL_ECHOLNPAIR("LUN Capacity: ",bulk.GetCapacity(0));

  const uint32_t sectorSize = bulk.GetSectorSize(0);
  if (sectorSize != 512) {
    SERIAL_ECHOLNPAIR("Expecting sector size of 512, got: ",sectorSize);
    return false;
  }

  return true;
}

void Sd2Card::idle() {
  static uint32_t next_retry;

  switch (state) {
    case USB_HOST_DELAY_INIT:
      next_retry = millis() + 10000;
      state = USB_HOST_WAITING;
      break;
    case USB_HOST_WAITING:
      if (millis() > next_retry) {
        next_retry = millis() + 10000;
        state = USB_HOST_UNINITIALIZED;
      }
      break;
    case USB_HOST_UNINITIALIZED:
      SERIAL_ECHOLNPGM("Starting USB host");
      if (!usb.start()) {
        SERIAL_ECHOLNPGM("USB host failed to start. Will retry in 10 seconds.");
        state = USB_HOST_DELAY_INIT;
      }
      else {
        SERIAL_ECHOLNPGM("USB host initialized");
        state = USB_HOST_INITIALIZED;
      }
      break;
    case USB_HOST_INITIALIZED:
      const uint8_t lastUsbTaskState = usb.getUsbTaskState();
      usb.Task();
      const uint8_t newUsbTaskState  = usb.getUsbTaskState();

      if (lastUsbTaskState == USB_STATE_RUNNING && newUsbTaskState != USB_STATE_RUNNING) {
        // the user pulled the flash drive. Make sure the bulk storage driver releases the address
        SERIAL_ECHOLNPGM("Drive removed\n");
        //bulk.Release();
      }
      if (lastUsbTaskState != USB_STATE_RUNNING && newUsbTaskState == USB_STATE_RUNNING)
        SERIAL_ECHOLNPGM("Drive inserted\n");
      break;
  }
}

uint32_t Sd2Card::cardSize() {
  if (!usbHostReady()) return 0;
  return bulk.GetCapacity(0);
}

bool Sd2Card::readData(uint8_t* dst) {
  return readBlock(block++, dst);
}

bool Sd2Card::readStart(uint32_t blockNumber) {
  block = blockNumber;
  return true;
}

bool Sd2Card::readStop() {
  return usbHostReady();
}

bool Sd2Card::readBlock(uint32_t block, uint8_t* dst) {
  if (!usbHostReady()) {
    SERIAL_ECHOLNPGM("Read from uninitalized USB host");
    return false;
  }
  return bulk.Read(0, block, 512, 1, dst) == 0;
}

bool Sd2Card::writeData(const uint8_t* src) {
  return writeBlock(block++, src);
}

bool Sd2Card::writeStart(uint32_t blockNumber, uint32_t eraseCount) {
  block = blockNumber;
  return true;
}

bool Sd2Card::writeStop() {
  return usbHostReady();
}

bool Sd2Card::writeBlock(uint32_t blockNumber, const uint8_t* src) {
  if (!usbHostReady()) {
    SERIAL_ECHOLNPGM("Write to uninitalized USB host");
    return false;
  }
  return bulk.Write(0, blockNumber, 512, 1, src) == 0;
}

#endif // USB_FLASH_DRIVE_SUPPORT
