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
#pragma once

//#if defined(__cplusplus)
//extern "C" {     /* Make sure we have C-declarations in C++ programs */
//#endif

#include <stdint.h>

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

#define WIP_Flag                0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte              0xA5

#define SPI_FLASH_SectorSize         4096
#define SPI_FLASH_PageSize           256
#define SPI_FLASH_PerWritePageSize   256

#if 0

#define PIC_NAME_MAX_LEN		50

#define LOGO_MAX_SIZE			(300*1024) //logo���ֵ
#define TITLELOGO_MAX_SIZE		(150*1024) //logo���ֵ
#define DEFAULT_VIEW_MAX_SIZE 	(200*200*2)
#define FLASH_VIEW_MAX_SIZE 	(200*200*2)

//ͼƬ
//Robin2�洢��ַ
#define PIC_NAME_ADDR			0x003000	//ͼƬ��Ϣ�洢��ַ��ͼƬ����
#define PIC_SIZE_ADDR			0x007000	//ͼƬ��Ϣ�洢��ַ��ͼƬ��Сֵ
#define PIC_COUNTER_ADDR		0x008000	//ͼƬ������ֵ�洢��ַ
#define PIC_LOGO_ADDR			0x009000	//ͼƬlogo�洢��ַ
//#define PIC_DATA_ADDR			0x02f000	//ͼƬ���ݴ洢��ַ

#define DEFAULT_VIEW_ADDR		0XC5800
#define BAK_VIEW_ADDR			(DEFAULT_VIEW_ADDR+90*1024)
#define PIC_ICON_LOGO_ADDR 		(BAK_VIEW_ADDR+80*1024)

#define PIC_DATA_ADDR			(PIC_ICON_LOGO_ADDR+350*1024)	//ͼƬ���ݴ洢��ַ//(800*240)	

//�ֿ�
#define FONTINFOADDR			0x600000//6M�Ժ��ַΪ�ֿ�
#define UNIGBK_FLASH_ADDR		(FONTINFOADDR+4096)//4*1024
#define GBK_FLASH_ADDR			(UNIGBK_FLASH_ADDR+180224)//176*1024

#define PER_PIC_MAX_SPACE		(32*1024)	//Ϊ�˷�ֹ����Խ������⣬ÿ��СͼƬ�����仮��Ӧ��ȡ�ܹ�����4K��ֵ

//
union union32
{
	uint8_t bytes[4];
	uint32_t dwords;
};
//ͼƬ��Ϣ�ṹ��
struct pic_msg
{
	uint8_t name[PIC_NAME_MAX_LEN];
	union union32 size;
};

typedef struct pic_msg PIC_MSG;

#endif

class ext_FLASH{
public:
	void init(uint8_t spiRate);
	static uint8_t spi_flash_Rec();
	static uint8_t spi_flash_read_write_byte(uint8_t data);
	static void spi_flash_Read(uint8_t* buf, uint16_t nbyte);
	static void spi_flash_Send(uint8_t b);
	static void spi_flash_SendBlock(uint8_t token, const uint8_t* buf);
	static uint16_t W25QXX_ReadID(void);
	static void SPI_FLASH_WriteEnable(void);
	static void SPI_FLASH_WaitForWriteEnd(void);
	static void SPI_FLASH_SectorErase(uint32_t SectorAddr);
	static void SPI_FLASH_BlockErase(uint32_t BlockAddr);
	static void SPI_FLASH_BulkErase(void);
	static void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
	static void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
	static void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
	//uint32_t lv_get_pic_addr(uint8_t *Pname);
	void lv_pic_read(uint8_t *P_Rbuff,uint32_t addr,uint32_t size);

};
extern ext_FLASH W25QXX;

//extern uint32_t lv_get_pic_addr(uint8_t *Pname);

//#if defined(__cplusplus)
//}    /* Make sure we have C-declarations in C++ programs */
//#endif

