#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "W25Qxx.h"

#define HAS_SPI_FLASH_COMPRESSION 1

/**
 * This class manage and optimize SPI Flash data storage,
 * keeping a internal buffer to write and save full
 * SPI flash pages as needed.
 *
 * Its supports Compression too, compacting and uncompacting
 * data on the fly, in real time, using a simple but fast
 * RLE implementation.
 *
 * Using it in the current LVGL_UI, it compacts 2.9MB of icons
 * down to 370kb!!!
 * The LVGL UI refresh rate became faster, when now we can fit
 * all LVGL UI in a tiny 2MB SPI Flash, like the Chitu Board.
 *
 * == Usage ==
 *
 * Writing:
 *
 * It have a internal buffer that keeps data until it
 * fits in a full SPI Flash full page.
 * When the buffer is full, it saves the data to spi flash.
 *
 *    SPIFlashStorage.beginWrite(myStartAddress);
 *    while (you have data to save) {
 *      SPIFlashStorage.addData(myBuffer, bufferSize);
 *    }
 *    SPIFlashStorage.endWrite(); //need to flush remaining buffer data
 *
 * Reading:
 *
 * When reading, it loads a full page from SPI Flash at once
 * and keep it in a internal buffer. As the user are consuming the
 * buffer, it will loading the data as needed.
 *
 *    SPIFlashStorage.beginRead(myStartAddress);
 *    while (you have data to read) {
 *      SPIFlashStorage.readData(myBuffer, bufferSize);
 *    }
 *
 * Compression:
 *
 * Its the big gain of using this class is Compression.
 * When compression is enabled, it keep another buffer, with the
 * compressed data. And only when this buffer became full, it flush
 * the page.
 *
 * The same is for reading: it read a compressed page, and
 * uncompress as the user ask for data.
 */
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
