/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2019 BigTreeTech [https://github.com/bigtreetech]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#if defined(__STM32F1__) && HAS_SD_HOST_DRIVE

#include "msc_sd.h"
#include "SPI.h"

#define PRODUCT_ID 0x29

USBMassStorage MarlinMSC;
MarlinUSBCompositeSerial MarlinCompositeSerial;

#include "../../inc/MarlinConfig.h"

#if SD_CONNECTION_IS(ONBOARD)

  #include "onboard_sd.h"

  static bool MSC_Write(const uint8_t *writebuff, uint32_t startSector, uint16_t numSectors) {
    return (disk_write(0, writebuff, startSector, numSectors) == RES_OK);
  }
  static bool MSC_Read(uint8_t *readbuff, uint32_t startSector, uint16_t numSectors) {
    return (disk_read(0, readbuff, startSector, numSectors) == RES_OK);
  }

#endif

#if ENABLED(EMERGENCY_PARSER)
  void (*real_rx_callback)(void);

  void my_rx_callback(void) {
    real_rx_callback();
    int len = MarlinCompositeSerial.available();
    while (len-- > 0) // >0 because available() may return a negative value
      emergency_parser.update(MarlinCompositeSerial.emergency_state, MarlinCompositeSerial.peek());
  }
#endif

void MSC_SD_init() {
  USBComposite.setProductId(PRODUCT_ID);
  // Just set MarlinCompositeSerial enabled to true
  // because when MarlinCompositeSerial.begin() is used in setup()
  // it clears all USBComposite devices.
  MarlinCompositeSerial.begin();
  USBComposite.end();
  USBComposite.clear();
  // Set api and register mass storage
  #if SD_CONNECTION_IS(ONBOARD)
    uint32_t cardSize;
    if (disk_initialize(0) == RES_OK) {
      if (disk_ioctl(0, GET_SECTOR_COUNT, (void *)(&cardSize)) == RES_OK) {
        MarlinMSC.setDriveData(0, cardSize, MSC_Read, MSC_Write);
        MarlinMSC.registerComponent();
      }
    }
  #endif
  // Register composite Serial
  MarlinCompositeSerial.registerComponent();
  USBComposite.begin();
  #if ENABLED(EMERGENCY_PARSER)
    //rx is usbSerialPart.endpoints[2]
    real_rx_callback = usbSerialPart.endpoints[2].callback;
    usbSerialPart.endpoints[2].callback = my_rx_callback;
  #endif
}

#endif // __STM32F1__ && HAS_SD_HOST_DRIVE
