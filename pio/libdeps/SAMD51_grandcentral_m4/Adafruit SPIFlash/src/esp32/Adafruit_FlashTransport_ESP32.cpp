/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 hathach for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Adafruit_FlashTransport.h"

#if CONFIG_IDF_TARGET_ESP32S2

Adafruit_FlashTransport_ESP32::Adafruit_FlashTransport_ESP32(void) {
  _cmd_read = SFLASH_CMD_READ;
  _addr_len = 3; // work with most device if not set

  _partition = NULL;
  memset(&_flash_device, 0, sizeof(_flash_device));
}

void Adafruit_FlashTransport_ESP32::begin(void) {
  _partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA,
                                        ESP_PARTITION_SUBTYPE_DATA_FAT, NULL);

  if (!_partition) {
    log_e("No FAT partition found");
  }
}

SPIFlash_Device_t *Adafruit_FlashTransport_ESP32::getFlashDevice(void) {
  if (!_partition)
    return NULL;

  esp_flash_t const *flash = _partition->flash_chip;

  _flash_device.total_size = flash->size;

  _flash_device.manufacturer_id = (flash->chip_id >> 16);
  _flash_device.memory_type = (flash->chip_id >> 8) & 0xff;
  _flash_device.capacity = flash->chip_id & 0xff;

  return &_flash_device;
}

void Adafruit_FlashTransport_ESP32::setClockSpeed(uint32_t write_hz,
                                                  uint32_t read_hz) {
  // do nothing, just use current configured clock
}

bool Adafruit_FlashTransport_ESP32::runCommand(uint8_t command) {
  // TODO maybe SFLASH_CMD_ERASE_CHIP should erase whole partition
  // do nothing, mostly write enable
  return true;
}

bool Adafruit_FlashTransport_ESP32::readCommand(uint8_t command,
                                                uint8_t *response,
                                                uint32_t len) {
  // mostly is Read STATUS, just fill with 0x0
  memset(response, 0, len);

  return true;
}

bool Adafruit_FlashTransport_ESP32::writeCommand(uint8_t command,
                                                 uint8_t const *data,
                                                 uint32_t len) {
  //  do nothing, mostly is Write Status
  return true;
}

bool Adafruit_FlashTransport_ESP32::eraseCommand(uint8_t command,
                                                 uint32_t addr) {
  uint32_t erase_sz = (command == SFLASH_CMD_ERASE_BLOCK) ? SFLASH_BLOCK_SIZE
                                                          : SFLASH_SECTOR_SIZE;
  return ESP_OK == esp_partition_erase_range(_partition, addr, erase_sz);
}

bool Adafruit_FlashTransport_ESP32::readMemory(uint32_t addr, uint8_t *data,
                                               uint32_t len) {
  return ESP_OK == esp_partition_read(_partition, addr, data, len);
}

bool Adafruit_FlashTransport_ESP32::writeMemory(uint32_t addr,
                                                uint8_t const *data,
                                                uint32_t len) {
  return ESP_OK == esp_partition_write(_partition, addr, data, len);
}

#endif
