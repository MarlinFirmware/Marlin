#ifndef _PIC_MANAGER_H_
#define _PIC_MANAGER_H_

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#include <stdint.h>
#include <string.h>
#include "lvgl.h"

#if 1

#define PIC_MAX_CN		100				//Maximum number of pictures
#define PIC_NAME_MAX_LEN		50	//Picture name maximum length

#define LOGO_MAX_SIZE_TFT35						(300*1024) 
#define LOGO_MAX_SIZE_TFT32						(150*1024) 
#define TITLELOGO_MAX_SIZE		(150*1024) //little logo maximum
#define DEFAULT_VIEW_MAX_SIZE 	(200*200*2)
#define FLASH_VIEW_MAX_SIZE 			(200*200*2)

#define PER_PIC_MAX_SPACE_TFT35		(32*1024)	
#define PER_PIC_MAX_SPACE_TFT32		(16*1024)
#define PER_FONT_MAX_SPACE		(16*1024)

//pic
//Robin_pro pic addr
#define PIC_NAME_ADDR			0x003000	//pic information addr
#define PIC_SIZE_ADDR			0x007000	//pic size information addr
#define PIC_COUNTER_ADDR		0x008000	//pic total number
#define PER_PIC_SAVE_ADDR		0x009000	//Storage address of each picture
#define PIC_LOGO_ADDR			0x00A000	//logo addr
//#define PIC_DATA_ADDR			0x02f000	//
//TFT35
#define DEFAULT_VIEW_ADDR_TFT35				0XC5800
#define BAK_VIEW_ADDR_TFT35						(DEFAULT_VIEW_ADDR_TFT35+90*1024)
#define PIC_ICON_LOGO_ADDR_TFT35 	    (BAK_VIEW_ADDR_TFT35+80*1024)
#define PIC_DATA_ADDR_TFT35						(PIC_ICON_LOGO_ADDR_TFT35+350*1024)//0XC5800	    
//TFT32
#define PIC_DATA_ADDR_TFT32 		0x02f000	
#define PIC_ICON_LOGO_ADDR_TFT32 0x5d8000	
#define PIC_OTHER_SIZE_ADDR_TFT32  0X5ee000 

//font
#define FONTINFOADDR					0x600000//6M -- font addr
#define UNIGBK_FLASH_ADDR		(FONTINFOADDR+4096)//4*1024
#define GBK_FLASH_ADDR				(UNIGBK_FLASH_ADDR+180224)//176*1024

//flash flag
#define FLASH_INF_VALID_FLAG			0xaa558761
//SD card information first addr
#define	VAR_INF_ADDR		0x000000

//
union union32
{
	uint8_t bytes[4];
	uint32_t dwords;
};
//pic information
struct pic_msg
{
	uint8_t name[PIC_NAME_MAX_LEN];
	union union32 size;
};

typedef struct pic_msg PIC_MSG;

#define BMP_WRITE_BUF_LEN 512

#define PICINFOADDR		0

#define PIC_SIZE_xM				6
#define FONT_SIZE_xM			2 	


//extern void PicMsg_Init(void);
extern void Pic_Read(uint8_t *Pname,uint8_t *P_Rbuff);
//extern void bindBmpFileData(const uint8_t **pBuf, uint8_t *pName);
//extern void Pic_Logo_Read(uint8_t *LogoName,uint8_t *Logo_Rbuff,uint32_t LogoReadsize);
//extern void default_view_Read(uint8_t *default_view_Rbuff,uint32_t default_view_Readsize);
//extern void flash_view_Read(uint8_t *flash_view_Rbuff,uint32_t flash_view_Readsize);

//extern void lv_Pic_Read(uint8_t *Pname,uint8_t *P_Rbuff,uint32_t addr,uint32_t size);
extern void lv_pic_test(uint8_t *P_Rbuff,uint32_t addr,uint32_t size);
#endif
extern uint32_t lv_get_pic_addr(uint8_t *Pname);

extern void get_spi_flash_data(const char *rec_buf,int offset, int size);
//extern void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

extern void spi_flash_read_test();
extern void default_view_Read(uint8_t *default_view_Rbuff,uint32_t default_view_Readsize);
extern void flash_view_Read(uint8_t *flash_view_Rbuff,uint32_t flash_view_Readsize);

#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif

#endif

