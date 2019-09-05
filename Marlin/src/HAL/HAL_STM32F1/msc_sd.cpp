#include "msc_sd.h"
#include "../../../src/sd/Sd2Card.h"
#include "spi.h"

Sd2Card sdCard;

USBMassStorage MarlinMSC;
USBCompositeSerial MarlinCompositeSerial;


bool MSC_Write(const uint8_t *writebuff, uint32_t startSector, uint16_t numSectors) {
  for(int i = 0; i < numSectors ; i++)
    sdCard.writeBlock(startSector, writebuff + i*512);
  return true;
}

bool MSC_Read(uint8_t *readbuff, uint32_t startSector, uint16_t numSectors) {
  for(int i = 0; i < numSectors ; i++)
    sdCard.readBlock(startSector, readbuff + i*512);
  return true;
}

void MSC_SD_init() {
  uint32 cardSize;
  sdCard.init(SPI_CLOCK_DIV4 ,SDSS);
  cardSize = sdCard.cardSize();
  #define PRODUCT_ID 0x29
  //CompositeSerial.begin();
  //USBComposite.end();
  USBComposite.setProductId(PRODUCT_ID);
  MarlinMSC.setDriveData(0, cardSize, MSC_Read, MSC_Write);
  MarlinMSC.registerComponent();
  MarlinCompositeSerial.registerComponent();
  USBComposite.begin();
  while(!USBComposite.isReady());
}
