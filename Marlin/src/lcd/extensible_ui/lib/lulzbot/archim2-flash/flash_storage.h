/*******************
 * flash_storage.h *
 *******************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

class SPIFlash {
  public:
    static constexpr uint32_t erase_unit_size = 4 * 1024; // Minimum erase unit
    static constexpr uint32_t write_page_size = 256;      // Minimum page write unit

    enum {
      READ_STATUS_1 = 0x05,
      READ_STATUS_2 = 0x35,
      READ_STATUS_3 = 0x33,
      WRITE_ENABLE  = 0x06,
      WRITE_DISABLE = 0x04,
      READ_ID       = 0x90,
      READ_JEDEC_ID = 0x9F,
      READ_DATA     = 0x03,
      PAGE_PROGRAM  = 0x02,
      ERASE_4K      = 0x20,
      ERASE_64K     = 0xD8,
      ERASE_CHIP    = 0xC7
    };

    static void wait_while_busy();
    static void erase_sector_4k(uint32_t addr);
    static void erase_sector_64k(uint32_t addr);
    static void erase_chip  ();

    static void read_jedec_id(uint8_t &manufacturer_id, uint8_t &device_type, uint8_t &capacity);

    static void spi_read_begin(uint32_t addr);
    static void spi_read_end();

    static void spi_write_begin(uint32_t addr);
    static void spi_write_end();

    static uint32_t write(uint32_t addr, const void *data, size_t size);
    static uint32_t read(uint32_t addr, void *data, size_t size);
};

class UIFlashStorage : private SPIFlash {
  private:

    static bool is_present;
    static int32_t  get_config_read_offset(uint32_t block_size);
    static int32_t  get_config_write_offset(uint32_t block_size);

    static uint32_t get_media_file_start(uint8_t slot);
    static void     set_media_file_size(uint8_t slot, uint32_t size);
    static uint32_t get_media_file_size(uint8_t slot);

    static constexpr uint32_t delimiter = 0x4C554C5A; // 'LULZ'
  public:
    enum error_t {
      SUCCESS,
      FILE_NOT_FOUND,
      READ_ERROR,
      VERIFY_ERROR,
      WOULD_OVERWRITE
    };

    static void    initialize  ();
    static void    format_flash ();
    static bool    check_known_device();

    static bool    is_valid ();
    static void    write_version_info();

    static void    write_config_data  (const void *data, size_t size);
    static bool    verify_config_data (const void *data, size_t size);
    static bool    read_config_data   (void *data, size_t size);
    static error_t write_media_file   (progmem_str filename, uint8_t slot = 0);

    class BootMediaReader;
};

class UIFlashStorage::BootMediaReader {
  private:
    uint32_t addr;
    uint32_t bytes_remaining;

  public:
    bool isAvailable(uint32_t slot = 0);
    int16_t read(void *buffer, size_t const size);

    static int16_t read(void *obj, void *buffer, const size_t size);
};
