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

#include "../../core/serial.h"

#include "lib/Usb.h"
#include "lib/masstorage.h"

#include "Sd2Card_FlashDrive.h"

#if ENABLED(ULTRA_LCD) || ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/ultralcd.h"
#endif

USB usb;
BulkOnly bulk(&usb);

Sd2Card::state_t Sd2Card::state;

// The USB library needs to be called periodically to detect USB thumbdrive
// insertion and removals. Call this idle() function periodically to allow
// the USB library to monitor for such events. This function also takes care
// of initializing the USB library for the first time.

void Sd2Card::idle() {
  static uint32_t next_retry;

  switch (state) {
    case USB_HOST_DELAY_INIT:
      next_retry = millis() + 2000;
      state = USB_HOST_WAITING;
      break;
    case USB_HOST_WAITING:
      if (ELAPSED(millis(), next_retry)) {
        next_retry = millis() + 2000;
        state = USB_HOST_UNINITIALIZED;
      }
      break;
    case USB_HOST_UNINITIALIZED:
      SERIAL_ECHOPGM("Starting USB host...");
      if (!usb.start()) {
        SERIAL_ECHOPGM(" Failed. Retrying in 2s.");
        #if ENABLED(ULTRA_LCD) || ENABLED(EXTENSIBLE_UI)
          LCD_MESSAGEPGM("USB start failed");
        #endif
        state = USB_HOST_DELAY_INIT;
      }
      else
        state = USB_HOST_INITIALIZED;
      SERIAL_EOL();
      break;
    case USB_HOST_INITIALIZED:
      const uint8_t lastUsbTaskState = usb.getUsbTaskState();
      usb.Task();
      const uint8_t newUsbTaskState  = usb.getUsbTaskState();

      if (lastUsbTaskState == USB_STATE_RUNNING && newUsbTaskState != USB_STATE_RUNNING) {
        // the user pulled the flash drive. Make sure the bulk storage driver releases the address
        #ifdef USB_DEBUG
          SERIAL_ECHOLNPGM("USB drive removed");
        #endif
        //bulk.Release();
      }
      if (lastUsbTaskState != USB_STATE_RUNNING && newUsbTaskState == USB_STATE_RUNNING) {
        #ifdef USB_DEBUG
          SERIAL_ECHOLNPGM("USB drive inserted");
        #endif
      }
      break;
  }
}

// Marlin calls this function to check whether an USB drive is inserted.
// This is equivalent to polling the SD_DETECT when using SD cards.
bool Sd2Card::isInserted() {
  return usb.getUsbTaskState() == USB_STATE_RUNNING;
}

// Marlin calls this to initialize an SD card once it is inserted.
bool Sd2Card::init(const uint8_t sckRateID/*=0*/, const pin_t chipSelectPin/*=SD_CHIP_SELECT_PIN*/) {
  if (!ready()) return false;

  if (!bulk.LUNIsGood(0)) {
    SERIAL_ECHOLNPGM("LUN zero is not good");
    return false;
  }

  const uint32_t sectorSize = bulk.GetSectorSize(0);
  if (sectorSize != 512) {
    SERIAL_ECHOLNPAIR("Expecting sector size of 512. Got: ", sectorSize);
    return false;
  }

  #ifdef USB_DEBUG
    lun0_capacity = bulk.GetCapacity(0);
    SERIAL_ECHOLNPAIR("LUN Capacity (in blocks): ", lun0_capacity);
  #endif
  return true;
}

// Returns the capacity of the card in blocks.
uint32_t Sd2Card::cardSize() {
  if (!ready()) return 0;
  #ifndef USB_DEBUG
    const uint32_t
  #endif
      lun0_capacity = bulk.GetCapacity(0);
  return lun0_capacity;
}

bool Sd2Card::readBlock(uint32_t block, uint8_t* dst) {
  if (!ready()) return false;
  #ifdef USB_DEBUG
    if (block >= lun0_capacity) {
      SERIAL_ECHOLNPAIR("Attempt to read past end of LUN: ", block);
      return false;
    }
    #if USB_DEBUG > 1
      SERIAL_ECHOLNPAIR("Read block ", block);
    #endif
  #endif
  return bulk.Read(0, block, 512, 1, dst) == 0;
}

bool Sd2Card::writeBlock(uint32_t block, const uint8_t* src) {
  if (!ready()) return false;
  #ifdef USB_DEBUG
    if (block >= lun0_capacity) {
      SERIAL_ECHOLNPAIR("Attempt to write past end of LUN: ", block);
      return false;
    }
    #if USB_DEBUG > 1
      SERIAL_ECHOLNPAIR("Write block ", block);
    #endif
  #endif
  return bulk.Write(0, block, 512, 1, src) == 0;
}

#endif // USB_FLASH_DRIVE_SUPPORT
