#pragma once
#include <SPI.h>

namespace hal
{
    class SPIFlash
    {
        public:

            SPIFlash(SPIClass &spi_instance, uint8_t cs_pin, uint16_t chip_id = 0);

            bool begin();
            void end();

            uint16_t read_chip_id();
            uint8_t read_status();
            
            uint8_t read(uint32_t addr);
            void read_array(uint32_t addr, void* buf, uint16_t len);
            void read_array_dma(uint32_t addr, void* buf, uint16_t len);
            
            void write(uint32_t addr, uint8_t byt);
            void write_array(uint32_t addr, const void* buf, uint16_t len);
            void write_array_dma(uint32_t addr, const void* buf, uint16_t len);

            void erase_full_chip();
            void erase_4K(uint32_t address);
            void erase_32K(uint32_t address);
            void erase_64K(uint32_t addr);

            bool busy();
            bool write_enable_latch();
            void sleep();
            void wakeup(); 

        private:
        
            void command(uint8_t cmd, boolean write = false);

            void select();
            void unselect();

            uint8_t m_cs_pin;
            uint16_t m_chip_id;
            SPISettings m_settings;
            SPIClass& m_spi;
    };
}
