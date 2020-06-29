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
#pragma once

#ifdef __cplusplus
extern "C" { /* C-declarations for C++ */
#endif

#include <stdint.h>
#include <string.h>
#include "lvgl.h"

#include "../../inc/MarlinConfigPre.h"

#ifndef HAS_SPI_FLASH_FONT
  #define HAS_SPI_FLASH_FONT              0 //disabled until fix the font load code
#endif
#ifndef HAS_GCODE_PREVIEW
  #define HAS_GCODE_PREVIEW               1
#endif
#ifndef HAS_LANG_SELECT_SCREEN
  #define HAS_LANG_SELECT_SCREEN               0
#endif
#ifndef HAS_BAK_VIEW_IN_FLASH
  #define HAS_BAK_VIEW_IN_FLASH               1
#endif
#ifndef HAS_GCODE_DEFAULT_VIEW_IN_FLASH
  #define HAS_GCODE_DEFAULT_VIEW_IN_FLASH               1
#endif
#ifndef SPI_FLASH_SIZE
  #define SPI_FLASH_SIZE                  0x1000000 // 16MB
#endif

#define PIC_MAX_CN           100    // Maximum number of pictures
#define PIC_NAME_MAX_LEN      50    // Picture name maximum length

#define LOGO_MAX_SIZE_TFT35             (300*1024)
#define LOGO_MAX_SIZE_TFT32             (150*1024)
#define TITLELOGO_MAX_SIZE              (150*1024) // Little logo maximum
#define DEFAULT_VIEW_MAX_SIZE           (200*200*2)
#define FLASH_VIEW_MAX_SIZE             (200*200*2)

#define PER_PIC_MAX_SPACE_TFT35         (32*1024)
#define PER_PIC_MAX_SPACE_TFT32         (16*1024)
#define PER_FONT_MAX_SPACE              (16*1024)

#if SPI_FLASH_SIZE == 0x200000
  //pic
  //Robin_pro pic addr
  #define PIC_NAME_ADDR                   0x001000      // Pic information addr
  #define PIC_SIZE_ADDR                   0x001800      // Pic size information addr
  #define PIC_COUNTER_ADDR                0x002000      // Pic total number
  #define PER_PIC_SAVE_ADDR               0x000000      // Storage address of each picture
  #define PIC_LOGO_ADDR                   0x000000      // Logo addr
  #define PIC_DATA_ADDR                   0x003000    //

  // TFT35
  #define DEFAULT_VIEW_ADDR_TFT35         0x1ea070
  #define BAK_VIEW_ADDR_TFT35             (DEFAULT_VIEW_ADDR_TFT35+90*1024)
  #define PIC_ICON_LOGO_ADDR_TFT35        (BAK_VIEW_ADDR_TFT35+80*1024)
  #define PIC_DATA_ADDR_TFT35             0x003000 // (PIC_ICON_LOGO_ADDR_TFT35+350*1024) //0xC5800

  #define PIC_DATA_ADDR_TFT32             0x00F000
  #define PIC_ICON_LOGO_ADDR_TFT32        0x5D8000
  #define PIC_OTHER_SIZE_ADDR_TFT32       0x5EE000

#else
  //pic
  //Robin_pro pic addr
  #define PIC_NAME_ADDR                   0x003000      // Pic information addr
  #define PIC_SIZE_ADDR                   0x007000      // Pic size information addr
  #define PIC_COUNTER_ADDR                0x008000      // Pic total number
  #define PER_PIC_SAVE_ADDR               0x009000      // Storage address of each picture
  #define PIC_LOGO_ADDR                   0x00A000      // Logo addr
  //#define PIC_DATA_ADDR                 0x02F000      //

  // TFT35
  #define DEFAULT_VIEW_ADDR_TFT35         0xC5800
  #define BAK_VIEW_ADDR_TFT35             (DEFAULT_VIEW_ADDR_TFT35+90*1024)
  #define PIC_ICON_LOGO_ADDR_TFT35        (BAK_VIEW_ADDR_TFT35+80*1024)
  #define PIC_DATA_ADDR_TFT35             (PIC_ICON_LOGO_ADDR_TFT35+350*1024) //0xC5800

  // TFT32
  #define PIC_DATA_ADDR_TFT32             0x02F000
  #define PIC_ICON_LOGO_ADDR_TFT32        0x5D8000
  #define PIC_OTHER_SIZE_ADDR_TFT32       0x5EE000

  // font
  #define FONTINFOADDR                    0x600000 // 6M -- font addr
  #define UNIGBK_FLASH_ADDR              (FONTINFOADDR+4096) // 4*1024
  #define GBK_FLASH_ADDR                 (UNIGBK_FLASH_ADDR+180224) // 176*1024

#endif

// Flash flag
#define FLASH_INF_VALID_FLAG            0xAA558761
// SD card information first addr
#define VAR_INF_ADDR                    0x000000

union union32 {
  uint8_t bytes[4];
  uint32_t dwords;
};

// pic information
struct pic_msg {
  uint8_t name[PIC_NAME_MAX_LEN];
  union union32 size;
};

typedef struct pic_msg PIC_MSG;

#define BMP_WRITE_BUF_LEN 512

#define PICINFOADDR   0

#define PIC_SIZE_xM   6
#define FONT_SIZE_xM  2

extern void Pic_Read(uint8_t *Pname, uint8_t *P_Rbuff);
extern void lv_pic_test(uint8_t *P_Rbuff, uint32_t addr, uint32_t size);
extern uint32_t lv_get_pic_addr(uint8_t *Pname);
extern void get_spi_flash_data(const char *rec_buf, int offset, int size);
extern void spi_flash_read_test();
extern void default_view_Read(uint8_t *default_view_Rbuff, uint32_t default_view_Readsize);
extern void flash_view_Read(uint8_t *flash_view_Rbuff, uint32_t flash_view_Readsize);

#ifdef __cplusplus
} /* C-declarations for C++ */
#endif
