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

/**
 * \file
 * \brief Sd2Card class for USB Flash Drive
 */
#include "../SdFatConfig.h"
#include "../SdInfo.h"
#include "../disk_io_driver.h"

#if DISABLED(USE_OTG_USB_HOST)
  /**
   * Define SOFTWARE_SPI to use bit-bang SPI
   */
  #if EITHER(MEGA_SOFT_SPI, USE_SOFTWARE_SPI)
    #define SOFTWARE_SPI
  #endif

  // SPI pin definitions - do not edit here - change in SdFatConfig.h
  #if ENABLED(SOFTWARE_SPI)
    #warning "Auto-assigning '10' as the SD_CHIP_SELECT_PIN."
    #define SD_CHIP_SELECT_PIN  10                // Software SPI chip select pin for the SD
  #else
    // hardware pin defs
    #define SD_CHIP_SELECT_PIN  SD_SS_PIN         // The default chip select pin for the SD card is SS.
  #endif
#endif

class DiskIODriver_USBFlash : public DiskIODriver {
  private:
    uint32_t pos;

    static void usbStateDebug();

  public:
    static bool usbStartup();
    static bool isInserted();

    bool init(const uint8_t sckRateID=0, const pin_t chipSelectPin=TERN(USE_OTG_USB_HOST, 0, SD_CHIP_SELECT_PIN)) override;

    inline bool readCSD(csd_t*)                                  override { return true; }

    inline bool readStart(const uint32_t block)                  override { pos = block; return isReady(); }
    inline bool readData(uint8_t *dst)                           override { return readBlock(pos++, dst); }
    inline bool readStop()                                       override { return true; }

    inline bool writeStart(const uint32_t block, const uint32_t) override { pos = block; return isReady(); }
    inline bool writeData(const uint8_t *src)                    override { return writeBlock(pos++, src); }
    inline bool writeStop()                                      override { return true; }

    bool readBlock(uint32_t block, uint8_t *dst) override;
    bool writeBlock(uint32_t blockNumber, const uint8_t *src) override;

    uint32_t cardSize() override;

    bool isReady() override;

    void idle() override;
};
