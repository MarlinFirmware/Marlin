#pragma once

#include "../user_interface.h"

#include "SPISlavePeripheral.h"

/**
 * SPI Flash W25Qxx device
 **/
 #define SPI_FLASH_IMAGE "./spi_flash.bin"
 #ifndef SPI_FLASH_IMAGE
   #error "You need set SPI_FLASH_IMAGE to keep SPI FLASH data."
 #endif

class W25QxxDevice: public SPISlavePeripheral {
public:
  W25QxxDevice(SpiBus& spi_bus, pin_type cs, size_t flash_size) : SPISlavePeripheral(spi_bus, cs), flash_size(flash_size) {
    // read current data
    data = new uint8_t[flash_size];
    memset(data, 0xFF, flash_size);

    fp = fopen(SPI_FLASH_IMAGE, "rb+");
    if (fp == nullptr) {
      fp = fopen(SPI_FLASH_IMAGE, "wb+");
      assert(fp);
      fwrite(data, 1, flash_size, fp);
    } else {
      [[maybe_unused]] auto value = fread(data, 1, flash_size, fp);
    }
  }
  virtual ~W25QxxDevice() {
    fclose(fp);
    delete[] data;
  };

  size_t flash_size;

  void onByteReceived(uint8_t _byte) override;
  void onEndTransaction() override;
  void onRequestedDataReceived(uint8_t token, uint8_t* _data, size_t count) override;

  FILE *fp = nullptr;
  uint8_t *data;
  int32_t currentAddress = -1;
};
