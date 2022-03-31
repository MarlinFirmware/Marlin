/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <stdint.h>

#include HAL_PATH(../HAL, MarlinSPI.h)

#define W25X_WriteEnable        0x06
#define W25X_WriteDisable       0x04
#define W25X_ReadStatusReg      0x05
#define W25X_WriteStatusReg     0x01
#define W25X_ReadData           0x03
#define W25X_FastReadData       0x0B
#define W25X_FastReadDual       0x3B
#define W25X_PageProgram        0x02
#define W25X_BlockErase         0xD8
#define W25X_SectorErase        0x20
#define W25X_ChipErase          0xC7
#define W25X_PowerDown          0xB9
#define W25X_ReleasePowerDown   0xAB
#define W25X_DeviceID           0xAB
#define W25X_ManufactDeviceID   0x90
#define W25X_JedecDeviceID      0x9F

#define WIP_Flag                0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte              0xA5

#define SPI_FLASH_SectorSize         4096
#define SPI_FLASH_PageSize           256
#define SPI_FLASH_PerWritePageSize   256

class W25QXXFlash {
private:
  static MarlinSPI mySPI;
public:
  void init(uint8_t spiRate);
  static uint8_t spi_flash_Rec();
  static uint8_t spi_flash_read_write_byte(uint8_t data);
  static void spi_flash_Read(uint8_t *buf, uint16_t nbyte);
  static void spi_flash_Send(uint8_t b);
  static void spi_flash_SendBlock(uint8_t token, const uint8_t *buf);
  static uint16_t W25QXX_ReadID(void);
  static void SPI_FLASH_WriteEnable(void);
  static void SPI_FLASH_WaitForWriteEnd(void);
  static void SPI_FLASH_SectorErase(uint32_t SectorAddr);
  static void SPI_FLASH_BlockErase(uint32_t BlockAddr);
  static void SPI_FLASH_BulkErase(void);
  static void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
  static void SPI_FLASH_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
  static void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
};

extern W25QXXFlash W25QXX;
