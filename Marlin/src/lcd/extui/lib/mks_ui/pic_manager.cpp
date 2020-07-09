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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(TFT_LITTLE_VGL_UI)

#include "../../../../MarlinCore.h"

//#include "type_define.h"
#include "string.h"

#include "pic_manager.h"
#include "W25Qxx.h"
#include "../../../../sd/cardreader.h"
#include "draw_ready_print.h"
#include "mks_hardware_test.h"

//#include "gui.h"
//#include "spi_flash.h"

//uint8_t DMA_ERRO_FLAG;
extern uint16_t DeviceCode;
extern unsigned char bmp_public_buf[17 * 1024];

#if ENABLED(SDSUPPORT)
  extern char *createFilename(char * const buffer, const dir_t &p);
#endif

/*void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead) {}*/

uint32_t lv_get_pic_addr(uint8_t *Pname) {
  uint8_t Pic_cnt;
  uint8_t i, j;
  PIC_MSG PIC;
  uint32_t tmp_cnt = 0;
  uint32_t addr = 0;

  W25QXX.init(SPI_QUARTER_SPEED);

  W25QXX.SPI_FLASH_BufferRead(&Pic_cnt, PIC_COUNTER_ADDR, 1);
  if (Pic_cnt == 0xff)
    Pic_cnt = 0;
  for (i = 0; i < Pic_cnt; i++) {
    j = 0;
    do
    {
      W25QXX.SPI_FLASH_BufferRead(&PIC.name[j], PIC_NAME_ADDR + tmp_cnt, 1);
      tmp_cnt++;
    }while (PIC.name[j++] != '\0');

    if ((strcmp((char*)Pname, (char*)PIC.name)) == 0) {

      if ((DeviceCode == 0x9488) || (DeviceCode == 0x5761))
        addr = PIC_DATA_ADDR_TFT35 + i * PER_PIC_MAX_SPACE_TFT35;
      else
        addr = PIC_DATA_ADDR_TFT32 + i * PER_PIC_MAX_SPACE_TFT32;
      return (addr + 4);//The purpose of adding 4 is to remove 4-byte picture header information.
    }
  }

  return addr;
}

const char *picPath = "mks_pic";
const char *bakPath = "bak_pic";

const char *fontPath = "mks_font";
const char *bakFont = "bak_font";

#if 1

void spiFlashErase_PIC() {
  #if 1
    volatile uint32_t pic_sectorcnt = 0;

    //LCD_Clear(BACK_COLOR);
    //LCD_DisplayString(90,80,"SPI Flash");
    //LCD_DisplayString(120,90,"PIC Erasing...");
    if ((DeviceCode == 0x9488) || (DeviceCode == 0x5761)) {
      //LCD_ShowString(180,100,200,24,24,"SPI Flash");
      //LCD_ShowString(170,130,200,24,24,"PIC Erasing...");
    }
    else {
      //LCD_ShowString(100,90,200,24,24,"SPI Flash");
      //LCD_ShowString(100,120,200,24,24,"PIC Erasing...");
    }
    #if ENABLED(MKS_TEST)
      for (pic_sectorcnt = 0; pic_sectorcnt < 2; pic_sectorcnt++)
        W25QXX.SPI_FLASH_BlockErase(PICINFOADDR + pic_sectorcnt * 64 * 1024);

    #else
      for (pic_sectorcnt = 0; pic_sectorcnt < PIC_SIZE_xM * 1024 / 64; pic_sectorcnt++)
        W25QXX.SPI_FLASH_BlockErase(PICINFOADDR + pic_sectorcnt * 64 * 1024);

    #endif
    /*
    FLASH_Unlock();
    spiFlashEraseFlag = SPI_FLASH_ERASE_FLAG_DATA;
    FLASH_ProgramHalfWord(SPI_FLASH_ERASE_FLAG_ADDR,spiFlashEraseFlag);
    FLASH_Lock();
    */
    #if 0
      if (DeviceCode == 0x9488)
        LCD_ShowString(170, 130, 200, 24, 24, "PIC Erase Done");
      else
        LCD_ShowString(100, 120, 200, 24, 24, "PIC Erase Done");

    #endif
    //spiFlashEraseFlag = 1;
  #endif
}

void spiFlashErase_FONT() {
  volatile uint32_t Font_sectorcnt = 0;

  //LCD_Clear(BACK_COLOR);
  if ((DeviceCode == 0x9488) || (DeviceCode == 0x5761)) {
    //LCD_ShowString(180,100,200,24,24,"SPI Flash");
    //LCD_ShowString(170,130,200,24,24,"FONT Erasing...");
  }
  else {
    //LCD_ShowString(100,90,200,24,24,"SPI Flash");
    //LCD_ShowString(90,120,200,24,24,"FONT Erasing...");
  }

  for (Font_sectorcnt = 0; Font_sectorcnt < FONT_SIZE_xM * 1024 / 64; Font_sectorcnt++)
    W25QXX.SPI_FLASH_BlockErase(FONTINFOADDR + Font_sectorcnt * 64 * 1024);

  /*
  FLASH_Unlock();
  spiFlashEraseFlag = SPI_FLASH_ERASE_FLAG_DATA;
  FLASH_ProgramHalfWord(SPI_FLASH_ERASE_FLAG_ADDR,spiFlashEraseFlag);
  FLASH_Lock();
  */
  #if 0
    if (DeviceCode == 0x9488)
      LCD_ShowString(170, 130, 200, 24, 24, "FONT Erase Done");
    else
      LCD_ShowString(90, 120, 200, 24, 24, "FONT Erase Done");
    //LCD_DisplayString(120,90,"FONT Erase Done");
  #endif
  //spiFlashEraseFlag = 1;
}

uint32_t LogoWrite_Addroffset = 0;

uint8_t Pic_Logo_Write(uint8_t *LogoName, uint8_t *Logo_Wbuff, uint32_t LogoWriteSize) {
  //uint16_t n;
  uint32_t i;
  uint8_t temp1;
  static uint32_t logo_maxsize;

  if (LogoWriteSize <= 0) return 0;

  W25QXX.SPI_FLASH_BufferWrite(Logo_Wbuff, PIC_LOGO_ADDR + LogoWrite_Addroffset, LogoWriteSize);

  for (i = 0; i < LogoWriteSize; i++) {
    W25QXX.SPI_FLASH_BufferRead(&temp1, PIC_LOGO_ADDR + LogoWrite_Addroffset + i, 1);
    if (*(Logo_Wbuff + i) != temp1) return 0;
  }
  LogoWrite_Addroffset += LogoWriteSize;
  if ((DeviceCode == 0x9488) || (DeviceCode == 0x5761))
    logo_maxsize = LOGO_MAX_SIZE_TFT35;
  else
    logo_maxsize = LOGO_MAX_SIZE_TFT32;
  if (LogoWrite_Addroffset >= logo_maxsize)
    LogoWrite_Addroffset = 0;
  return 1;
}

uint32_t TitleLogoWrite_Addroffset = 0;
uint8_t Pic_TitleLogo_Write(uint8_t *TitleLogoName, uint8_t *TitleLogo_Wbuff, uint32_t TitleLogoWriteSize) {
  //uint32_t i;
  //uint8_t temp1;
  if (TitleLogoWriteSize <= 0)
    return 0;
  if ((DeviceCode == 0x9488) || (DeviceCode == 0x5761))
    W25QXX.SPI_FLASH_BufferWrite(TitleLogo_Wbuff, PIC_ICON_LOGO_ADDR_TFT35 + TitleLogoWrite_Addroffset, TitleLogoWriteSize);
  else
    W25QXX.SPI_FLASH_BufferWrite(TitleLogo_Wbuff, PIC_ICON_LOGO_ADDR_TFT32 + TitleLogoWrite_Addroffset, TitleLogoWriteSize);
  TitleLogoWrite_Addroffset += TitleLogoWriteSize;
  if (TitleLogoWrite_Addroffset >= TITLELOGO_MAX_SIZE)
    TitleLogoWrite_Addroffset = 0;
  return 1;
}

uint32_t default_view_addroffset_r = 0;
void default_view_Write(uint8_t *default_view__Rbuff, uint32_t default_view_Writesize) {
  W25QXX.SPI_FLASH_BufferWrite(default_view__Rbuff, DEFAULT_VIEW_ADDR_TFT35 + default_view_addroffset_r, default_view_Writesize);
  default_view_addroffset_r += default_view_Writesize;
  if (default_view_addroffset_r >= DEFAULT_VIEW_MAX_SIZE)
    default_view_addroffset_r = 0;
}

uint32_t Pic_Info_Write(uint8_t *P_name, uint32_t P_size) {
  uint8_t pic_counter = 0;
  uint32_t Pic_SaveAddr;
  uint32_t Pic_SizeSaveAddr;
  uint32_t Pic_NameSaveAddr;
  //uint8_t temp;
  uint8_t Pname_temp;
  uint32_t i, j;
  uint32_t name_len = 0;
  uint32_t SaveName_len = 0;
  union union32 size_tmp;
  //union union32 size1;
  //uint8_t Pn[PIC_NAME_MAX_LEN];
  //uint8_t cnt_temp;
  //uint16_t n0;
  //uint32_t Name_saveAddr = 0;
  //uint8_t pic_position;

  W25QXX.SPI_FLASH_BufferRead(&pic_counter, PIC_COUNTER_ADDR, 1);

  if (pic_counter == 0xFF)
    pic_counter = 0;

  if ((DeviceCode == 0x9488) || (DeviceCode == 0x5761))
    Pic_SaveAddr = PIC_DATA_ADDR_TFT35 + pic_counter * PER_PIC_MAX_SPACE_TFT35;
  else
    Pic_SaveAddr = PIC_DATA_ADDR_TFT32 + pic_counter * PER_PIC_MAX_SPACE_TFT32;

  for (j = 0; j < pic_counter; j++) {
    do {
      W25QXX.SPI_FLASH_BufferRead(&Pname_temp, PIC_NAME_ADDR + SaveName_len, 1);
      SaveName_len++;
    } while (Pname_temp != '\0');
  }
  i = 0;
  while ((*(P_name + i) != '\0')) {
    i++;
    name_len++;
  }

  Pic_NameSaveAddr = PIC_NAME_ADDR + SaveName_len;
  W25QXX.SPI_FLASH_BufferWrite(P_name, Pic_NameSaveAddr, name_len + 1);
  Pic_SizeSaveAddr = PIC_SIZE_ADDR + 4 * pic_counter;
  size_tmp.dwords = P_size;
  W25QXX.SPI_FLASH_BufferWrite(size_tmp.bytes, Pic_SizeSaveAddr, 4);

  pic_counter++;
  W25QXX.SPI_FLASH_SectorErase(PIC_COUNTER_ADDR);
  W25QXX.SPI_FLASH_BufferWrite(&pic_counter, PIC_COUNTER_ADDR, 1);

  return Pic_SaveAddr;
}

uint8_t public_buf[512];

//uint8_t public_buf_test[512];
  #if ENABLED(SDSUPPORT)
void UpdatePic() {
  //int r;
  //unsigned char *p;
  //char rootPath[10]={0};
  char *fn;
  unsigned char logoFlag;
  uint16_t pbr;
  uint32_t pfileSize;
  uint32_t Pic_Write_Addr;
  /*----------------------------------*/

  // FILINFO fno;
  //DIR dir;
  //char tmp[30];
  #if 0//_USE_LFN
    static char lfn[_MAX_LFN + 1];
    finfo.lfname = lfn;
    finfo.lfsize = sizeof(lfn);
  #endif
  //SdFile curDir;
  //if (f_opendir(&dirs, picPath) == FR_OK)
  //card.cd(picPath);

  //const uint16_t fileCnt = card.get_num_Files();

  //SdFile *curDir;
  //SdFile dir;
  //dir.open(picPath, O_READ);
  //const char * const fname = card.diveToFile(true, curDir, picPath);
  //if (!fname) return;

  SdFile dir, root = card.getroot();
  if (dir.open(&root, picPath, O_RDONLY)) {

    disp_pic_update();
    spiFlashErase_PIC();

    dir_t d;
    while (dir.readDir(&d, card.longFilename) > 0) {
      #if 1
        /*
        if (power_det == 0) {
          PW_DET_ON;
          power_det=0;
        }
        SPI_FLASH_Init();
        */

        //for (uint16_t i = 0; i < fileCnt; i++) {
        //res = f_readdir(&dirs, &finfo);
        //card.getfilename_sorted(i);

        if (card.longFilename[0] == 0)
          break;
        /*if ( card.filename[0] == '.')
        continue;
      */
        if (card.longFilename[0] == '.')
          continue;

        fn = card.longFilename;

        /*if ((finfo.lfname[0] == 0) || (finfo.lfname == 0))
          fn = finfo.fname;
        else
          fn = finfo.lfname;*/

        /*   if (fno.fattrib & AM_DIR)
        {
          continue;
        }
        else */
        //{
        //if ((strstr(fn, ".gco")) || (strstr(fn, ".GCO")) || (fno.fattrib & AM_DIR))
        if (strstr(fn, ".bin")) {
          if (strstr(fn, "_logo"))
            logoFlag = 1;
          else if (strstr(fn, "_titlelogo"))
            logoFlag = 2;
          else if (strstr(fn, "_preview"))
            logoFlag = 3;
          else
            logoFlag = 0;

          //public_buf[0] = '\0';
          //strcat(public_buf, picPath);
          //strcat(public_buf, "/");
          char dosFilename[FILENAME_LENGTH];
          createFilename(dosFilename, d);
          //strcat(public_buf, dosFilename);

          SdFile file;
          if (file.open(&dir, dosFilename, O_READ)) {
            #if 1
            /*LCD_Clear(BACK_COLOR);

            if ((DeviceCode==0x9488)||(DeviceCode==0x5761)) {
              LCD_ShowString(170,100,200,24,24,(u8 *)fn);
              LCD_ShowString(180,130,200,24,24,"Updating...");
            }
            else {
              LCD_ShowString(90,90,200,24,24,(u8 *)fn);
              LCD_ShowString(90,120,200,24,24,"Updating...");
            }
            */
            if (logoFlag == 1) {
              while (1) {
                pbr = file.read(public_buf, BMP_WRITE_BUF_LEN);
                Pic_Logo_Write((uint8_t *)fn, public_buf, pbr); //
                if (pbr < BMP_WRITE_BUF_LEN) break;
              }
            }
            else if (logoFlag == 2) {
              while (1) {
                pbr = file.read(public_buf, BMP_WRITE_BUF_LEN);
                Pic_TitleLogo_Write((uint8_t *)fn, public_buf, pbr); //
                if (pbr < BMP_WRITE_BUF_LEN) break;
              }
            }
            else if (logoFlag == 3) {
              while (1)
              {
                pbr = file.read(public_buf, BMP_WRITE_BUF_LEN);
                default_view_Write(public_buf, pbr); //
                if (pbr < BMP_WRITE_BUF_LEN) break;
              }
            }
            else {
              pfileSize = file.fileSize();
              Pic_Write_Addr = Pic_Info_Write((uint8_t *)fn, pfileSize);
              //uint32_t addr_test = Pic_Write_Addr;
              //memset(bmp_public_buf, 0xff, BMP_WRITE_BUF_LEN);
              while (1)
              {
                pbr = file.read(public_buf, BMP_WRITE_BUF_LEN);
                W25QXX.SPI_FLASH_BufferWrite(public_buf, Pic_Write_Addr, pbr);
                Pic_Write_Addr += pbr;
                if (pbr < BMP_WRITE_BUF_LEN) break;
              }
              //W25QXX.SPI_FLASH_BufferRead(public_buf_test,addr_test,BMP_WRITE_BUF_LEN);
            }

            /*--------------read test--------------------*/
            /*
            BufferSet(picBuffer, 0xff, PICTURE_MAX_SIZE);
            if (logoFlag == 0)
            {
              PicMsg_Init();
              Pic_Read(fn,picBuffer);
            }
            else
              Pic_Logo_Read(fn,picBuffer,PICTURE_MAX_SIZE);
            */
            /*--------------read test--------------------*/

            file.close();
            #endif
          }

        }
        //}
        //}
        /*
            LCD_Clear(LCD_COLOR_BLACK);
            //LCD_ShowString(110,100,200,24,24,"Complete");
            LCD_DisplayString(110,80,"Complete");
            delay(0xfffff);
        */
        //r = f_chdir("/");
        #if 1
          //SdFile dir, root = card.getroot();
          /*if (dir.open(&root, bakPath, O_RDONLY))
      {
        dir.remove();
      }*/

          //r = f_rename(picPath, bakPath);


          //update_flag_ok = 1;
        #endif
      #endif
    }
    dir.rename(&root, bakPath);
  }
}


void spi_flash_read_test() {W25QXX.SPI_FLASH_BufferRead(public_buf, UNIGBK_FLASH_ADDR, BMP_WRITE_BUF_LEN);}

void UpdateFont() {
  //int r;
  //unsigned char *p;
  //char rootPath[10]={0};
  char *fn;
  //unsigned char logoFlag;
  uint16_t pbr;
  uint32_t flashaddr = 0;
  //uint32_t pfileSize;
  //uint32_t Pic_Write_Addr;
  /*----------------------------------*/

  // FILINFO fno;
  //DIR dir;
  //char tmp[30];
  #if 0//_USE_LFN
    static char lfn[_MAX_LFN + 1];
    finfo.lfname = lfn;
    finfo.lfsize = sizeof(lfn);
  #endif
  //SdFile curDir;
  //if (f_opendir(&dirs, picPath) == FR_OK)
  //card.cd(picPath);

  //const uint16_t fileCnt = card.get_num_Files();

  //SdFile *curDir;
  //SdFile dir;
  //dir.open(picPath, O_READ);
  //const char * const fname = card.diveToFile(true, curDir, picPath);
  //if (!fname) return;

  SdFile dir, root = card.getroot();
  if (dir.open(&root, fontPath, O_RDONLY)) {

    disp_font_update();
    spiFlashErase_FONT();

    dir_t d;
    while (dir.readDir(&d, card.longFilename) > 0)
    {
      #if 1
        /*if (power_det == 0)
        {
          PW_DET_ON;
          power_det=0;
        }
        SPI_FLASH_Init();*/


        //for (uint16_t i = 0; i < fileCnt; i++)
        //{
        //res = f_readdir(&dirs, &finfo);
        //card.getfilename_sorted(i);

        if (card.longFilename[0] == 0)
          break;
        /*if ( card.filename[0] == '.')
        continue;
      */
        if (card.longFilename[0] == '.')
          continue;

        fn = card.longFilename;

        /*if ((finfo.lfname[0] == 0) || (finfo.lfname == 0))
          fn = finfo.fname;
        else
          fn = finfo.lfname;*/

        /*   if (fno.fattrib & AM_DIR)
        {
        continue;
        }
        else */
        //{
        //if ((strstr(fn, ".gco")) || (strstr(fn, ".GCO")) || (fno.fattrib & AM_DIR))
        if (strstr(fn, ".bin")) {
          char dosFilename[FILENAME_LENGTH];
          createFilename(dosFilename, d);
          //strcat(public_buf, dosFilename);

          SdFile file;
          if (file.open(&dir, dosFilename, O_READ)) {

            #if 1
              /*LCD_Clear(BACK_COLOR);

              if ((DeviceCode==0x9488)||(DeviceCode==0x5761))
              {
                LCD_ShowString(170,100,200,24,24,(u8 *)fn);
                LCD_ShowString(180,130,200,24,24,"Updating...");
              }
              else
              {
                LCD_ShowString(90,90,200,24,24,(u8 *)fn);
                LCD_ShowString(90,120,200,24,24,"Updating...");
              }
              */
              flashaddr = UNIGBK_FLASH_ADDR;
              pbr = 0;
              while (1)
              {
                flashaddr += pbr;
                pbr = file.read(public_buf, BMP_WRITE_BUF_LEN);
                W25QXX.SPI_FLASH_BufferWrite(public_buf, flashaddr, pbr);
                //W25QXX.SPI_FLASH_BufferRead(public_buf_test,flashaddr,pbr);
                /*if (UNIGBKFlag == 1)
                {
                  fontrate = (uint16_t)(((float)(flashaddr - UNIGBK_FLASH_ADDR)/(float)(psrc.fsize))*100);
                }
                else
                {
                  fontrate = (uint16_t)(((float)(flashaddr - GBK_FLASH_ADDR)/(float)(psrc.fsize))*100);
                }

                if (fontrate > 99) fontrate=99;

                if (fontrate < 10)
                {
                  fontString[0] = fontrate%10 + 0x30;
                  fontString[1] = '%';
                  fontString[2] = '\0';
                }
                else
                {
                  fontString[0] = fontrate/10 + 0x30;
                  fontString[1] = fontrate%10 + 0x30;
                  fontString[2] = '%';
                  fontString[3] = '\0';
                }*/
                //LCD_DisplayString(140,130,fontString);
                if ((DeviceCode == 0x9488) || (DeviceCode == 0x5761)) {
                  //LCD_ShowString(200,160,200,24,24,fontString);
                }
                else {
                  //LCD_ShowString(140,150,200,24,24,fontString);
                }
                if (pbr < BMP_WRITE_BUF_LEN) break;
              }
              /*--------------read test--------------------*/
              /*
              BufferSet(picBuffer, 0xff, PICTURE_MAX_SIZE);
              if (logoFlag == 0)
              {
                PicMsg_Init();
                Pic_Read(fn,picBuffer);
              }
              else
                Pic_Logo_Read(fn,picBuffer,PICTURE_MAX_SIZE);
              */
              /*--------------read test--------------------*/

              file.close();
            #endif
          }

        }
        //}
        //}
        /*
            LCD_Clear(LCD_COLOR_BLACK);
            //LCD_ShowString(110,100,200,24,24,"Complete");
            LCD_DisplayString(110,80,"Complete");
            delay(0xfffff);
        */
        //r = f_chdir("/");
        #if 1
          //SdFile dir, root = card.getroot();
          /*if (dir.open(&root, bakPath, O_RDONLY))
      {
        dir.remove();
      }*/
          dir.rename(&root, bakFont);
          //r = f_rename(picPath, bakPath);


          //update_flag_ok = 1;
        #endif
      #endif
    }
  }
}
  #endif // SDSUPPORT

#endif

#if 1

void Pic_Read(uint8_t *Pname, uint8_t *P_Rbuff) {
  uint8_t i, j;
  uint8_t Pic_cnt;
  uint32_t tmp_cnt = 0;
  PIC_MSG PIC;

  //void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);

  W25QXX.SPI_FLASH_BufferRead(&Pic_cnt, PIC_COUNTER_ADDR, 1);
  if (Pic_cnt == 0xff)
    Pic_cnt = 0;

  for (i = 0; i < Pic_cnt; i++) {
    //pic name
    j = 0;
    do
    {
      W25QXX.SPI_FLASH_BufferRead(&PIC.name[j], PIC_NAME_ADDR + tmp_cnt, 1);
      tmp_cnt++;
    }while (PIC.name[j++] != '\0');
    //pic size
    W25QXX.SPI_FLASH_BufferRead(PIC.size.bytes, PIC_SIZE_ADDR + i * 4, 4);

    if ((strcmp((char*)Pname, (char*)PIC.name)) == 0) {
      W25QXX.SPI_FLASH_BufferRead((uint8_t *)P_Rbuff, PIC_DATA_ADDR_TFT35 + i * PER_PIC_MAX_SPACE_TFT35, PIC.size.dwords);
      /*if (DMA_ERRO_FLAG)
      {
        DMA_ERRO_FLAG = 0;
        SPI_FLASH_BufferRead((uint8_t *)P_Rbuff,PIC_DATA_ADDR+i*PER_PIC_MAX_SPACE,PIC.size.dwords);
      }*/
      break;
    }
  }
}

void lv_pic_test(uint8_t *P_Rbuff, uint32_t addr, uint32_t size) {
  W25QXX.init(SPI_QUARTER_SPEED);
  W25QXX.SPI_FLASH_BufferRead((uint8_t *)P_Rbuff, addr, size);
  /*if (DMA_ERRO_FLAG) {
    DMA_ERRO_FLAG = 0;
    SPI_FLASH_BufferRead((uint8_t *)P_Rbuff,addr,size);
  }*/

}

void get_spi_flash_data(const char *rec_buf, int addr, int size) {
  W25QXX.init(SPI_QUARTER_SPEED);
  W25QXX.SPI_FLASH_BufferRead((uint8_t *)rec_buf, UNIGBK_FLASH_ADDR + addr, size);
}

#endif

#if 1

  uint32_t logo_addroffset = 0;
  void Pic_Logo_Read(uint8_t *LogoName, uint8_t *Logo_Rbuff, uint32_t LogoReadsize) {
    W25QXX.SPI_FLASH_BufferRead(Logo_Rbuff, PIC_LOGO_ADDR + logo_addroffset, LogoReadsize);
    logo_addroffset += LogoReadsize;
    if (logo_addroffset >= LOGO_MAX_SIZE_TFT35)
      logo_addroffset = 0;
  }

  uint32_t default_view_addroffset = 0;
  void default_view_Read(uint8_t *default_view_Rbuff, uint32_t default_view_Readsize) {
    W25QXX.init(SPI_QUARTER_SPEED);

    W25QXX.SPI_FLASH_BufferRead(default_view_Rbuff, DEFAULT_VIEW_ADDR_TFT35 + default_view_addroffset + 4, default_view_Readsize);
    default_view_addroffset += default_view_Readsize;
    if (default_view_addroffset >= DEFAULT_VIEW_MAX_SIZE)
      default_view_addroffset = 0;

  }

  uint32_t flash_view_addroffset = 0;
  void flash_view_Read(uint8_t *flash_view_Rbuff, uint32_t flash_view_Readsize) {
    W25QXX.init(SPI_QUARTER_SPEED);

    W25QXX.SPI_FLASH_BufferRead(flash_view_Rbuff, BAK_VIEW_ADDR_TFT35 + flash_view_addroffset, flash_view_Readsize);
    flash_view_addroffset += flash_view_Readsize;
    if (flash_view_addroffset >= FLASH_VIEW_MAX_SIZE)
      flash_view_addroffset = 0;

  }

#endif

#endif // TFT_LITTLE_VGL_UI
