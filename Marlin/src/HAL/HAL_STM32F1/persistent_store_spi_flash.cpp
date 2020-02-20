#ifdef __STM32F1__
#include "../../inc/MarlinConfig.h"

#if BOTH(EEPROM_SETTINGS, SPI_FLASH_EEPROM_EMULATION)

    #include "../shared/persistent_store_api.h"
    #include <SPI.h>
    #include "spi_flash.h"

    #define SPI_FLASH_EEPROM_SIZE 0x1000 // 4K - do not change!

    #ifndef E2END
    #define E2END SPI_FLASH_EEPROM_SIZE - 1
    #endif

    static char __attribute__ ((aligned(4))) _eeprom_data[SPI_FLASH_EEPROM_SIZE];

    SPIClass _spi(SPI_FLASH_DEVICE);
    hal::SPIFlash _flash(_spi, SPI_FLASH_CS_PIN);

    bool PersistentStore::access_start()
    {
        _flash.begin();
        _flash.read_array(SPI_FLASH_EEPROM_OFFSET,  _eeprom_data, SPI_FLASH_EEPROM_SIZE);
        _flash.end();
    }

    bool PersistentStore::access_finish()
    {
        _flash.begin();
        _flash.erase_4K(SPI_FLASH_EEPROM_OFFSET);
        _flash.write_array(SPI_FLASH_EEPROM_OFFSET,  _eeprom_data, SPI_FLASH_EEPROM_SIZE);
        _flash.end();
    }
  
    bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc)
    {
        for (size_t i = 0; i < size; i++)
        {
            _eeprom_data[pos + i] = value[i];
        }
        crc16(crc, value, size);
        pos += size;
        return false;
    }

    bool PersistentStore::read_data(int &pos, uint8_t* value, const size_t size, uint16_t *crc, const bool writing/*=true*/)
    {
        for (size_t i = 0; i < size; i++)
        {
            uint8_t c = _eeprom_data[pos + i];
            if (writing) value[i] = c;
            crc16(crc, &c, 1);
        }
        pos += size;
        return false;
    }

    size_t PersistentStore::capacity()
    {
        return SPI_FLASH_EEPROM_SIZE; 
    }

#endif
#endif
