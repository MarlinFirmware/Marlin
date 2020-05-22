/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "../../../../../inc/MarlinConfigPre.h"
#if 1//ENABLED(SPI_FLASH)
#include <SPI.h>
#include "../../../../../inc/MarlinConfig.h"

#if ENABLED(TFT_LITTLE_VGL_UI)
#include "../inc/W25Qxx.h"

#ifndef SPI_FLASH_MISO_PIN
  #define SPI_FLASH_MISO_PIN W25QXX_MISO_PIN
#endif
#ifndef SPI_FLASH_MOSI_PIN
  #define SPI_FLASH_MOSI_PIN W25QXX_MOSI_PIN
#endif
#ifndef SPI_FLASH_SCK_PIN
  #define SPI_FLASH_SCK_PIN  W25QXX_SCK_PIN
#endif
#ifndef SPI_FLASH_CS_PIN
  #define SPI_FLASH_CS_PIN   W25QXX_CS_PIN
#endif

#define W25QXX_CS_H	OUT_WRITE(SPI_FLASH_CS_PIN, HIGH)
#define W25QXX_CS_L	OUT_WRITE(SPI_FLASH_CS_PIN, LOW)

ext_FLASH W25QXX;

void ext_FLASH::init(uint8_t spiRate){
	
	  OUT_WRITE(SPI_FLASH_CS_PIN, HIGH);

	  /**
	   * STM32F1 APB2 = 72MHz, APB1 = 36MHz, max SPI speed of this MCU if 18Mhz
	   * STM32F1 has 3 SPI ports, SPI1 in APB2, SPI2/SPI3 in APB1
	   * so the minimum prescale of SPI1 is DIV4, SPI2/SPI3 is DIV2
	   */
	  #if SPI_DEVICE == 1
	    #define SPI_CLOCK_MAX SPI_CLOCK_DIV4
	  #else
	    #define SPI_CLOCK_MAX SPI_CLOCK_DIV2
	  #endif
	  uint8_t  clock;
	  switch (spiRate) {
	    case SPI_FULL_SPEED:    clock = SPI_CLOCK_MAX ;  break;
	    case SPI_HALF_SPEED:    clock = SPI_CLOCK_DIV4 ; break;
	    case SPI_QUARTER_SPEED: clock = SPI_CLOCK_DIV8 ; break;
	    case SPI_EIGHTH_SPEED:  clock = SPI_CLOCK_DIV16; break;
	    case SPI_SPEED_5:       clock = SPI_CLOCK_DIV32; break;
	    case SPI_SPEED_6:       clock = SPI_CLOCK_DIV64; break;
	    default:                clock = SPI_CLOCK_DIV2;  // Default from the SPI library
	  }
	  SPI.setModule(SPI_DEVICE);
	  SPI.begin();
	  SPI.setClockDivider(clock);
	  SPI.setBitOrder(MSBFIRST);
	  SPI.setDataMode(SPI_MODE0);	
}

/**
 * @brief  Receive a single byte from the SPI port.
 *
 * @return Byte received
 *
 * @details
 */
uint8_t ext_FLASH::spi_flash_Rec() {
  uint8_t returnByte = SPI.transfer(ff);
  return returnByte;
}

uint8_t ext_FLASH::spi_flash_read_write_byte(uint8_t data) {
  uint8_t returnByte = SPI.transfer(data);
  return returnByte;
}

/**
 * @brief  Receive a number of bytes from the SPI port to a buffer
 *
 * @param  buf   Pointer to starting address of buffer to write to.
 * @param  nbyte Number of bytes to receive.
 * @return Nothing
 *
 * @details Uses DMA
 */
void ext_FLASH::spi_flash_Read(uint8_t* buf, uint16_t nbyte) {
  SPI.dmaTransfer(0, const_cast<uint8_t*>(buf), nbyte);
}

/**
 * @brief  Send a single byte on SPI port
 *
 * @param  b Byte to send
 *
 * @details
 */
void ext_FLASH::spi_flash_Send(uint8_t b) {
  SPI.send(b);
}
/**
 * @brief  Write token and then write from 512 byte buffer to SPI (for SD card)
 *
 * @param  buf   Pointer with buffer start address
 * @return Nothing
 *
 * @details Use DMA
 */
void ext_FLASH::spi_flash_SendBlock(uint8_t token, const uint8_t* buf) {
  SPI.send(token);
  SPI.dmaSend(const_cast<uint8_t*>(buf), 512);
}

uint16_t ext_FLASH::W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	  
	W25QXX_CS_L;				    
	spi_flash_Send(0x90);//���Ͷ�ȡID����	    
	spi_flash_Send(0x00); 	    
	spi_flash_Send(0x00); 	    
	spi_flash_Send(0x00); 	 			   
	Temp|=spi_flash_Rec()<<8;  
	Temp|=spi_flash_Rec();	 
	W25QXX_CS_H;				    
	return Temp;
}  
void ext_FLASH::SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
	W25QXX_CS_L;
  /* Send "Write Enable" instruction */
  	spi_flash_Send(W25X_WriteEnable);
  /* Deselect the FLASH: Chip Select high */
	W25QXX_CS_H;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ext_FLASH::SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  W25QXX_CS_L;
  /* Send "Read Status Register" instruction */
  spi_flash_Send(W25X_ReadStatusReg);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = spi_flash_Rec();

  }
  while ((FLASH_Status & WIP_Flag) == 0x01); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  W25QXX_CS_H;
}

void ext_FLASH::SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
	W25QXX_CS_L;
  /* Send Sector Erase instruction */
  spi_flash_Send(W25X_SectorErase);
  /* Send SectorAddr high nibble address byte */
  spi_flash_Send((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  spi_flash_Send((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  spi_flash_Send(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */

  W25QXX_CS_H;
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

void ext_FLASH::SPI_FLASH_BlockErase(uint32_t BlockAddr)
{
    SPI_FLASH_WriteEnable();
    W25QXX_CS_L;
    /* Send Sector Erase instruction */
    spi_flash_Send(W25X_BlockErase);
    /* Send SectorAddr high nibble address byte */
    spi_flash_Send((BlockAddr & 0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    spi_flash_Send((BlockAddr & 0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    spi_flash_Send(BlockAddr & 0xFF);

    W25QXX_CS_H;

    SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ext_FLASH::SPI_FLASH_BulkErase(void)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  W25QXX_CS_L;
  
  /* Send Bulk Erase instruction  */
  spi_flash_Send(W25X_ChipErase);
  /* Deselect the FLASH: Chip Select high */
  W25QXX_CS_H;
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void ext_FLASH::SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */
  W25QXX_CS_L;
  /* Send "Write to Memory " instruction */
  spi_flash_Send(W25X_PageProgram);
  /* Send WriteAddr high nibble address byte to write to */
  spi_flash_Send((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  spi_flash_Send((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  spi_flash_Send(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
  }

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /* Send the current byte */
    spi_flash_Send(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  W25QXX_CS_H;

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void ext_FLASH::SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

  if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void ext_FLASH::SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  W25QXX_CS_L;

  /* Send "Read from Memory " instruction */
  spi_flash_Send(W25X_ReadData);

  /* Send ReadAddr high nibble address byte to read from */
  spi_flash_Send((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  spi_flash_Send((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  spi_flash_Send(ReadAddr & 0xFF);

  if(NumByteToRead<33)	
  {
    while (NumByteToRead--) /* while there is data to be read */
    {
      /* Read a byte from the FLASH */
      *pBuffer = spi_flash_Rec();
      /* Point to the next location where the byte read will be saved */
      pBuffer++;
    }
  }
  else
  {
      spi_flash_Read(pBuffer, NumByteToRead); 
   }
  W25QXX_CS_H;
}


void ext_FLASH::lv_pic_read(uint8_t *P_Rbuff,uint32_t addr,uint32_t size)
{
	SPI_FLASH_BufferRead((uint8_t *)P_Rbuff,addr,size);
}


#endif

#endif