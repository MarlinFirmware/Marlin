/**
MKS Robin Nano
U5 W25Q64BV, 16K SERIAL EEPROM: 


*/
#include "../../inc/MarlinConfig.h"

#if ENABLED(SPI_EEPROM_W25Q)
#include "../../libs/W25Qxx.h"

//W25QXXFlash W25QXX;
uint8_t spi_eeprom[MARLIN_EEPROM_SIZE];

void eeprom_test(void);

void eeprom_init(void){
    DEBUG("Start EEPROM");
    W25QXX.init(SPI_EIGHTH_SPEED);
    //eeprom_test();
    W25QXX.SPI_FLASH_BufferRead((uint8_t *)spi_eeprom,SPI_EEPROM_OFFSET,MARLIN_EEPROM_SIZE);
}

void eeprom_hw_deinit(void){
    DEBUG("Finish EEPROM");
    W25QXX.SPI_FLASH_WriteEnable();
    W25QXX.SPI_FLASH_SectorErase(SPI_EEPROM_OFFSET);
    //write
    W25QXX.SPI_FLASH_BufferWrite((uint8_t *)spi_eeprom,SPI_EEPROM_OFFSET,MARLIN_EEPROM_SIZE);
}

void eeprom_write_byte(uint8_t *pos, unsigned char value){
    uint16_t addr=(unsigned)pos;

    if(addr < MARLIN_EEPROM_SIZE){
        spi_eeprom[addr]=value;
    }else{
        ERROR("Write out of SPI size: %d %d",addr,MARLIN_EEPROM_SIZE);
    }
}

uint8_t eeprom_read_byte(uint8_t *pos) {
    uint16_t addr=(unsigned)pos;

    if(addr < MARLIN_EEPROM_SIZE){
        return spi_eeprom[addr];
    }else{
        ERROR("Read out of SPI size: %d %d",addr,MARLIN_EEPROM_SIZE);
        return 0;
    }
}

void eeprom_read_block(void *__dst, const void *__src, size_t __n){
  ERROR("Call to missing function");
};

void eeprom_update_block(const void *__src, void *__dst, size_t __n){
  ERROR("Call to missing function");
};

void eeprom_test(void){
    DEBUG("SPI Flash ID %0X",W25QXX.W25QXX_ReadID());
    DEBUG("Read FLASH:");
    for(uint32_t i=0; i < 50 ; ){
        memset(spi_eeprom,0,10);
        W25QXX.SPI_FLASH_BufferRead((uint8_t *)spi_eeprom,SPI_EEPROM_OFFSET+i,10);
        DEBUG("%d %0X %0X %0X %0X %0X %0X %0X %0X %0X %0X",i,spi_eeprom[0],spi_eeprom[1],spi_eeprom[2],spi_eeprom[3],spi_eeprom[4],spi_eeprom[5],spi_eeprom[6],spi_eeprom[7],spi_eeprom[8],spi_eeprom[9]);
        i=i+10;
    }

    DEBUG("Erase flash");
    W25QXX.SPI_FLASH_WriteEnable();
    W25QXX.SPI_FLASH_SectorErase(SPI_EEPROM_OFFSET);

    DEBUG("Read FLASH:");
    for(uint32_t i=0; i < 50 ; ){
        memset(spi_eeprom,0,10);
        W25QXX.SPI_FLASH_BufferRead((uint8_t *)spi_eeprom,SPI_EEPROM_OFFSET+i,10);
        DEBUG("%d %0X %0X %0X %0X %0X %0X %0X %0X %0X %0X",i,spi_eeprom[0],spi_eeprom[1],spi_eeprom[2],spi_eeprom[3],spi_eeprom[4],spi_eeprom[5],spi_eeprom[6],spi_eeprom[7],spi_eeprom[8],spi_eeprom[9]);
        i=i+10;
    }

    DEBUG("Read/write FLASH:");
    for(uint32_t i=0; i < 50 ; ){
        
        memset(spi_eeprom,0x0B,10);

        W25QXX.SPI_FLASH_WriteEnable();
        W25QXX.SPI_FLASH_BufferWrite((uint8_t *)spi_eeprom,SPI_EEPROM_OFFSET+i,10);

        W25QXX.SPI_FLASH_BufferRead((uint8_t *)spi_eeprom,SPI_EEPROM_OFFSET+i,10);
        DEBUG("%d %0X %0X %0X %0X %0X %0X %0X %0X %0X %0X",i,spi_eeprom[0],spi_eeprom[1],spi_eeprom[2],spi_eeprom[3],spi_eeprom[4],spi_eeprom[5],spi_eeprom[6],spi_eeprom[7],spi_eeprom[8],spi_eeprom[9]);
        i=i+10;
    }

}


#endif // SPI_EEPROM_W25Q
