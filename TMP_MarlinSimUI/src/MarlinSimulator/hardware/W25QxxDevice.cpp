#include "W25QxxDevice.h"
#include "src/libs/W25Qxx.h"

void W25QxxDevice::onByteReceived(uint8_t _byte) {
  SPISlavePeripheral::onByteReceived(_byte);
  if (getCurrentToken() != 0xFF) return;

  switch (_byte) {
    case W25X_PageProgram:
    case W25X_ReadData:
    case W25X_SectorErase:
    case W25X_BlockErase:
      setRequestedDataSize(_byte, 3);
      break;
    case W25X_WriteEnable:
      break;
    case W25X_ReadStatusReg:
      fflush(fp);
      setResponse(0);
      break;
    default:
      break;
  }
};

void W25QxxDevice::onRequestedDataReceived(uint8_t token, uint8_t* _data, size_t count) {
  SPISlavePeripheral::onRequestedDataReceived(token, _data, count);
  switch (token) {
    case W25X_ReadData:
      currentAddress = (_data[0] << 16) | (_data[1] << 8) | _data[2];
      setResponse(data + currentAddress, flash_size - currentAddress);
      currentAddress = -1;
      break;
    case W25X_PageProgram:
      // receivind data to write!
      if (currentAddress > -1) {
        memcpy(data + currentAddress, _data, count);
        fwrite(_data, 1, count, fp);
        currentAddress = -1;
      }
      else {
        currentAddress = (_data[0] << 16) | (_data[1] << 8) | _data[2];
        fseek(fp, currentAddress, SEEK_SET);
        setRequestedDataSize(W25X_PageProgram, SPI_FLASH_PerWritePageSize);
      }
      break;
    case W25X_SectorErase:
      currentAddress = (_data[0] << 16) | (_data[1] << 8) | _data[2];
      memset(data + currentAddress, 0xFF, SPI_FLASH_SectorSize);
      fseek(fp, currentAddress, SEEK_SET);
      fwrite(data + currentAddress, 1, SPI_FLASH_SectorSize, fp);
      currentAddress = -1;
      break;
    case W25X_BlockErase:
      currentAddress = (_data[0] << 16) | (_data[1] << 8) | _data[2];
      memset(data + currentAddress, 0xFF, 65536); //64kb
      fseek(fp, currentAddress, SEEK_SET);
      fwrite(data + currentAddress, 1, 65536, fp);
      currentAddress = -1;
      break;
    default:
      break;
  }

}

void W25QxxDevice::onEndTransaction() {
  SPISlavePeripheral::onEndTransaction();
}
