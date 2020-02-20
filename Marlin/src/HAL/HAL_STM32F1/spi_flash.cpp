#include "spi_flash.h"

namespace hal
{
    constexpr size_t SPIFLASH_PAGESIZE = 256;

    enum SPICommand
    {
        WRITEENABLE      = 0x06,
        WRITEDISABLE     = 0x04,
        BLOCKERASE_4K    = 0x20,
        BLOCKERASE_32K   = 0x52,
        BLOCKERASE_64K   = 0xD8,
        CHIPERASE        = 0x60,
        STATUSREAD       = 0x05,
        STATUSWRITE      = 0x01,
        ARRAYREAD        = 0x0B,
        ARRAYREADLOWFREQ = 0x03,
        SLEEP            = 0xB9,
        WAKE             = 0xAB,
        BYTEPAGEPROGRAM  = 0x02,
        IDREAD           = 0x9F,
        MACREAD          = 0x4B
    };

    SPIFlash::SPIFlash(SPIClass &spi_instance, uint8_t cs_pin, uint16_t chip_id)
    : m_spi(spi_instance)
    , m_cs_pin(cs_pin)
    , m_chip_id(chip_id)
    {
    }

    void SPIFlash::select()
    {
        m_spi.beginTransaction(m_settings);
        digitalWrite(m_cs_pin, LOW);
    }

    void SPIFlash::unselect()
    {
        digitalWrite(m_cs_pin, HIGH);
        m_spi.endTransaction();
    }

    boolean SPIFlash::begin()
    {
        pinMode(m_cs_pin, OUTPUT);
        m_spi.begin();
        m_settings = SPISettings(25000000, MSBFIRST, SPI_MODE0);

        unselect();
        wakeup();
    
        if (m_chip_id == 0 || read_chip_id() == m_chip_id)
        {
            command(SPICommand::STATUSWRITE, true);
            m_spi.transfer(0);
            unselect();
            return true;
        }

        return false;
    }

    uint16_t SPIFlash::read_chip_id()
    {
        command(SPICommand::IDREAD);
        uint16_t chip_id = m_spi.transfer(0) << 8;
        chip_id |= m_spi.transfer(0);
        unselect();
        return chip_id;
    }

    uint8_t SPIFlash::read(uint32_t addr)
    {
        command(SPICommand::ARRAYREADLOWFREQ);
        m_spi.transfer(addr >> 16);
        m_spi.transfer(addr >> 8);
        m_spi.transfer(addr);
        uint8_t result = m_spi.transfer(0);
        unselect();
        return result;
    }

    void SPIFlash::read_array(uint32_t addr, void* buf, uint16_t len)
    {
        command(SPICommand::ARRAYREAD);
        m_spi.transfer(addr >> 16);
        m_spi.transfer(addr >> 8);
        m_spi.transfer(addr);
        m_spi.transfer(0); // dummy 8 clocks

        for (uint16_t i = 0; i < len; ++i)
        {
            ((uint8_t*)buf)[i] = m_spi.transfer(0);
        }
        
        unselect();
    }

    void SPIFlash::read_array_dma(uint32_t addr, void* buf, uint16_t len)
    {
        command(SPICommand::ARRAYREAD);
        m_spi.transfer(addr >> 16);
        m_spi.transfer(addr >> 8);
        m_spi.transfer(addr);
        m_spi.transfer(0); // dummy 8 clocks

        m_spi.dmaTransfer(nullptr, buf, len);

        for (uint16_t i = 0; i < len; ++i)
        {
            ((uint8_t*)buf)[i] = m_spi.transfer(0);
        }
        
        unselect();
    }

    void SPIFlash::command(uint8_t cmd, bool write)
    {
        if (cmd != SPICommand::WAKE)
        {
            while(busy());
        }
        
        if (write)
        {
            select();
            m_spi.transfer(SPICommand::WRITEENABLE);
            unselect();
            while(busy());
        }

        select();
        m_spi.transfer(cmd);
    }

    boolean SPIFlash::busy()
    {
        return read_status() & 1;
    }

    boolean SPIFlash::write_enable_latch()
    {
        return read_status() & 2;
    }

    uint8_t SPIFlash::read_status()
    {
        select();
        m_spi.transfer(SPICommand::STATUSREAD);
        uint8_t status = m_spi.transfer(0);
        unselect();
        return status;
    }

    void SPIFlash::write(uint32_t addr, uint8_t bt)
    {
        command(SPICommand::BYTEPAGEPROGRAM, true);
        m_spi.transfer(addr >> 16);
        m_spi.transfer(addr >> 8);
        m_spi.transfer(addr);
        m_spi.transfer(bt);
        unselect();
    }

    void SPIFlash::write_array(uint32_t addr, const void* buf, uint16_t len)
    {
        uint16_t bytes_in_page = SPIFLASH_PAGESIZE - (addr % SPIFLASH_PAGESIZE);
        uint16_t offset = 0;

        while (len > 0)
        {
            uint16_t batch_size = (len <= bytes_in_page) ? len : bytes_in_page;

            command(SPICommand::BYTEPAGEPROGRAM, true);
            m_spi.transfer(addr >> 16);
            m_spi.transfer(addr >> 8);
            m_spi.transfer(addr);

            for (uint16_t i = 0; i < batch_size; i++)
            {
                m_spi.transfer(((uint8_t*)buf)[offset + i]);
            }
            
            unselect();

            //wait till it's programmed
            while(write_enable_latch());

            addr += batch_size;
            offset += batch_size;
            len -= batch_size;
            bytes_in_page = SPIFLASH_PAGESIZE;
        }
    }

    void SPIFlash::write_array_dma(uint32_t addr, const void* buf, uint16_t len)
    {
        const uint8_t * byte_buf = (const uint8_t*)buf;
        uint16_t bytes_in_page = SPIFLASH_PAGESIZE - (addr % SPIFLASH_PAGESIZE);

        while (len > 0)
        {
            uint16_t batch_size = (len <= bytes_in_page) ? len : bytes_in_page;

            command(SPICommand::BYTEPAGEPROGRAM, true);
            m_spi.transfer(addr >> 16);
            m_spi.transfer(addr >> 8);
            m_spi.transfer(addr);

            m_spi.dmaSend(byte_buf, batch_size);
            
            unselect();

            //wait till it's programmed
            while(write_enable_latch());

            addr += batch_size;
            byte_buf += batch_size;
            len -= batch_size;
            bytes_in_page = SPIFLASH_PAGESIZE;
        }
    }

    void SPIFlash::erase_full_chip()
    {
        command(SPICommand::CHIPERASE, true);
        unselect();
        while(write_enable_latch());
    }

    void SPIFlash::erase_4K(uint32_t addr)
    {
        command(SPICommand::BLOCKERASE_4K, true);
        m_spi.transfer(addr >> 16);
        m_spi.transfer(addr >> 8);
        m_spi.transfer(addr);
        unselect();
        while(write_enable_latch());
    }

    void SPIFlash::erase_32K(uint32_t addr)
    {
        command(SPICommand::BLOCKERASE_32K, true);
        m_spi.transfer(addr >> 16);
        m_spi.transfer(addr >> 8);
        m_spi.transfer(addr);
        unselect();
        while(write_enable_latch());
    }

    void SPIFlash::erase_64K(uint32_t addr)
    {
        command(SPICommand::BLOCKERASE_64K, true);
        m_spi.transfer(addr >> 16);
        m_spi.transfer(addr >> 8);
        m_spi.transfer(addr);
        unselect();
        while(write_enable_latch());
    }

    void SPIFlash::sleep()
    {
        command(SPICommand::SLEEP);
        unselect();
    }

    void SPIFlash::wakeup()
    {
        command(SPICommand::WAKE);
        unselect();
    }

    void SPIFlash::end()
    {
        m_spi.end();
    }
}