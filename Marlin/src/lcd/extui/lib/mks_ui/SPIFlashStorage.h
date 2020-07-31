#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "W25Qxx.h"

#define HAS_SPI_FLASH_COMPRESSION 1

class SPIFlashStorage
{
public:
    // Write operation
    void beginWrite(uint32_t startAddress);
    void endWrite();
    void writeData(uint8_t* data, uint16_t size);

    // Read operation
    void beginRead(uint32_t startAddress);
    void readData(uint8_t* data, uint16_t size);

    uint32_t getCurrentPage() { return m_currentPage; }

private:
    void flushPage();
    void savePage(uint8_t* buffer);
    void loadPage(uint8_t* buffer);
    void readPage();
    uint16_t inData(uint8_t* data, uint16_t size);
    uint16_t outData(uint8_t* data, uint16_t size);

    uint8_t m_pageData[SPI_FLASH_PageSize];
    uint32_t m_currentPage;
    uint16_t m_pageDataUsed;
    uint16_t m_pageDataFree;
    uint32_t m_startAddress;
#if HAS_SPI_FLASH_COMPRESSION
    uint8_t m_compressedData[SPI_FLASH_PageSize];
    uint16_t m_compressedDataUsed;
    uint16_t m_compressedDataFree;
#endif
};

extern SPIFlashStorage SPIFlash;

#endif
