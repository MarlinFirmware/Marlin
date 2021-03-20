#pragma once
#include "../inc/MarlinConfig.h"
#ifdef LONGER3D
#define TARGET_LPC1768
#define FLASH_SIZE (uint32_t)8388608     //(8*1024*1024)
#define W25QXX_CS(n) digitalWrite(PC5,n)   

#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18
//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

class W25Qxx
{
private:
    /* data */
public:
    W25Qxx(/* args */);
    ~W25Qxx();
    void W25QXX_Init(void);
    uint16_t W25QXX_ReadID();
    void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);   //write data to flash
    void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);     //read data in flash
    void W25QXX_Erase_Sector(uint32_t Dst_Addr);	  //erase sector
    void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);  //write data to falsh is not check
    void W25QXX_Write_Enable(void);    //write enable
    void W25QXX_Write_Disable(void);   //write disable (protect)
    void W25QXX_Wait_Busy(void);     //wait for idle
    uint8_t	 W25QXX_ReadSR(void);    //read status register
    void W25QXX_Write_SR(uint8_t sr);  	//write status register
    void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);  
    void W25QXX_Erase_Chip(void);  	  //wipe the whole piece
    void W25QXX_WAKEUP(void);	      //wake up
    void W25QXX_PowerDown(void);    //go into power loss protection				
};

 typedef struct savepar
  {
    int16_t s_result[4];
    uint8_t checksflag;
  }SAVEDATA;  
#endif