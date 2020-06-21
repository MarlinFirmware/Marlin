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

/********************************************************************************
 * @file     dwin_lcd.c
 * @author   LEO / Creality3D
 * @date     2019/07/18
 * @version  2.0.1
 * @brief    迪文屏控制操作函数
 ********************************************************************************/

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD)

#include "../../inc/MarlinConfig.h"

#include "dwin_lcd.h"
#include <string.h> // for memset

// Make sure DWIN_SendBuf is large enough to hold the largest
// printed string plus the draw command and tail.
uint8_t DWIN_SendBuf[11 + 24] = { 0xAA };
uint8_t DWIN_BufTail[4] = { 0xCC, 0x33, 0xC3, 0x3C };
uint8_t databuf[26] = { 0 };
uint8_t receivedType;

int recnum = 0;

inline void DWIN_Byte(size_t &i, const uint16_t bval) {
  DWIN_SendBuf[++i] = bval;
}

inline void DWIN_Word(size_t &i, const uint16_t wval) {
  DWIN_SendBuf[++i] = wval >> 8;
  DWIN_SendBuf[++i] = wval & 0xFF;
}

inline void DWIN_Long(size_t &i, const uint32_t lval) {
  DWIN_SendBuf[++i] = (lval >> 24) & 0xFF;
  DWIN_SendBuf[++i] = (lval >> 16) & 0xFF;
  DWIN_SendBuf[++i] = (lval >>  8) & 0xFF;
  DWIN_SendBuf[++i] = lval & 0xFF;
}

inline void DWIN_String(size_t &i, char * const string) {
  const size_t len = strlen(string);
  memcpy(&DWIN_SendBuf[i+1], string, len);
  i += len;
}

/*发送当前BUF中的数据以及包尾数据 len:整包数据长度*/
inline void DWIN_Send(size_t &i) {
  ++i;
  LOOP_L_N(n, i) {  MYSERIAL1.write(DWIN_SendBuf[n]);
                    delayMicroseconds(1); }
  LOOP_L_N(n, 4) {  MYSERIAL1.write(DWIN_BufTail[n]);
                    delayMicroseconds(1); }
}

/*----------------------------------------------系统变量函数----------------------------------------------*/
/*握手 1: 握手成功  2: 握手失败*/
bool DWIN_Handshake(void) {
  size_t i = 0;
  DWIN_Byte(i, 0x00);
  DWIN_Send(i);

  while (MYSERIAL1.available() > 0 && recnum < (signed)sizeof(databuf)) {
    databuf[recnum] = MYSERIAL1.read();
    // ignore the invalid data
    if (databuf[0] != FHONE) { // prevent the program from running.
      if (recnum > 0) {
        recnum = 0;
        ZERO(databuf);
      }
      continue;
    }
    delay(10);
    recnum++;
  }

  return ( recnum >= 3
        && databuf[0] == FHONE
        && databuf[1] == '\0'
        && databuf[2] == 'O'
        && databuf[3] == 'K' );
}

/*设定背光亮度 luminance:亮度(0x00~0xFF)*/
void DWIN_Backlight_SetLuminance(const uint8_t luminance) {
  size_t i = 0;
  DWIN_Byte(i, 0x30);
  DWIN_Byte(i, _MAX(luminance, 0x1F));
  DWIN_Send(i);
}

/*设定画面显示方向 dir:0,0°; 1,90°; 2,180°; 3,270°*/
void DWIN_Frame_SetDir(uint8_t dir) {
  size_t i = 0;
  DWIN_Byte(i, 0x34);
  DWIN_Byte(i, 0x5A);
  DWIN_Byte(i, 0xA5);
  DWIN_Byte(i, dir);
  DWIN_Send(i);
}

/*更新显示*/
void DWIN_UpdateLCD(void) {
  size_t i = 0;
  DWIN_Byte(i, 0x3D);
  DWIN_Send(i);
}

/*----------------------------------------------绘图相关函数----------------------------------------------*/
/*画面清屏 color:清屏颜色*/
void DWIN_Frame_Clear(const uint16_t color) {
  size_t i = 0;
  DWIN_Byte(i, 0x01);
  DWIN_Word(i, color);
  DWIN_Send(i);
}

/*画面画线 color:线段颜色 xStart:X起始坐标 yStart:Y起始坐标 xEnd:X终止坐标 yEnd:Y终止坐标*/
void DWIN_Draw_Line(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  DWIN_Byte(i, 0x03);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

/*画面画矩形 mode:0,外框;1,填充;2,异或填充 color:颜色 xStart/yStart:矩形左上坐标 xEnd/yEnd:矩形右下坐标*/
void DWIN_Draw_Rectangle(uint8_t mode, uint16_t color,
                         uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  DWIN_Byte(i, 0x05);
  DWIN_Byte(i, mode);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

/*画面区域移动 mode:0,环移;1,平移 dir:0,向左移动;1,向右移动;2,向上移动;3,向下移动 dis:移动距离
                color:填充颜色 xStart/yStart:选定区域左上坐标 xEnd/yEnd:选定区域右下坐标*/
void DWIN_Frame_AreaMove(uint8_t mode, uint8_t dir, uint16_t dis,
                         uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  size_t i = 0;
  DWIN_Byte(i, 0x09);
  DWIN_Byte(i, (mode << 7) | dir);
  DWIN_Word(i, dis);
  DWIN_Word(i, color);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Send(i);
}

/*----------------------------------------------文本相关函数----------------------------------------------*/
/*画面显示字符串 widthAdjust:true,自调整字符宽度;false,不调整字符宽度 bShow:true,显示背景色;false,不显示背景色 size:字号大小
                  color:字符颜色 bColor:背景颜色 x/y:字符串左上坐标 *string:字符串*/
void DWIN_Draw_String(bool widthAdjust, bool bShow, uint8_t size,
                      uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, char *string) {
  size_t i = 0;
  DWIN_Byte(i, 0x11);
  DWIN_Byte(i, (widthAdjust? 0x80:0x00) | (bShow? 0x40:0x00) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_String(i, string);
  DWIN_Send(i);
}

/*画面显示正整数 bShow:true,显示背景色;false,不显示背景色 zeroFill:true,补零;false,不补零 zeroMode:1,无效0显示为0; 0,无效0显示为空格 size:字号大小
                  color:字符颜色 bColor:背景颜色 iNum:位数 x/y:变量左上坐标 value:整型变量*/
void DWIN_Draw_IntValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                          uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, uint16_t value) {
  size_t i = 0;
  DWIN_Byte(i, 0x14);
  DWIN_Byte(i, (bShow? 0x80:0x00) | (zeroFill? 0x20:0x00) | (zeroMode? 0x10:0x00) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, iNum);
  DWIN_Byte(i, 0); // fNum
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  #if 0
    for (char count = 0; count < 8; count++) {
      DWIN_Byte(i, value);
      value >>= 8;
      if ((value&0xFF) == 0x00) break;
    }
  #else
    // Write a big-endian 64 bit integer
    const size_t p = i + 1;
    for (char count = 8; count--;) { // 7..0
      ++i;
      DWIN_SendBuf[p + count] = value;
      value >>= 8;
    }
  #endif

  DWIN_Send(i);
}

/*画面显示浮点数 bShow:true,显示背景色;false,不显示背景色 zeroFill:true,补零;false,不补零 zeroMode:1,无效0显示为0; 0,无效0显示为空格 size:字号大小
                  color:字符颜色 bColor:背景颜色 iNum:整数位数 fNum:小数位数 x/y:变量左上坐标 value:浮点数变量*/
void DWIN_Draw_FloatValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value) {
  //uint8_t *fvalue = (uint8_t*)&value;
  size_t i = 0;
  DWIN_Byte(i, 0x14);
  DWIN_Byte(i, (bShow? 0x80:0x00) | (zeroFill? 0x20:0x00) | (zeroMode? 0x10:0x00) | size);
  DWIN_Word(i, color);
  DWIN_Word(i, bColor);
  DWIN_Byte(i, iNum);
  DWIN_Byte(i, fNum);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Long(i, value);
  /*
  DWIN_Byte(i, fvalue[3]);
  DWIN_Byte(i, fvalue[2]);
  DWIN_Byte(i, fvalue[1]);
  DWIN_Byte(i, fvalue[0]);
  */
  DWIN_Send(i);
}

/*----------------------------------------------图片相关函数----------------------------------------------*/
/*jpg图片显示并缓存在#0虚拟显示区 id:图片ID*/
void DWIN_JPG_ShowAndCache(const uint8_t id) {
  size_t i = 0;
  DWIN_Word(i, 0x2200);
  DWIN_Byte(i, id);
  DWIN_Send(i);     //AA 23 00 00 00 00 08 00 01 02 03 CC 33 C3 3C
}

/*图标显示 libID:图标库ID picID:图标ID x/y:图标左上坐标*/
void DWIN_ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y) {
  NOMORE(x, DWIN_WIDTH - 1);
  NOMORE(y, DWIN_HEIGHT - 1); // -- ozy -- srl
  size_t i = 0;
  DWIN_Byte(i, 0x23);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Byte(i, 0x80 | libID);
  DWIN_Byte(i, picID);
  DWIN_Send(i);
}

/*jpg图片解压到#1虚拟显示区 id:图片ID*/
void DWIN_JPG_CacheToN(uint8_t n, uint8_t id) {
  size_t i = 0;
  DWIN_Byte(i, 0x25);
  DWIN_Byte(i, n);
  DWIN_Byte(i, id);
  DWIN_Send(i);
}

/*从虚拟显示区复制区域至当前画面 cacheID:虚拟区号 xStart/yStart:虚拟区左上坐标 xEnd/yEnd:虚拟区右下坐标 x/y:当前画面粘贴坐标*/
void DWIN_Frame_AreaCopy(uint8_t cacheID, uint16_t xStart, uint16_t yStart,
                         uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y) {
  size_t i = 0;
  DWIN_Byte(i, 0x27);
  DWIN_Byte(i, 0x80 | cacheID);
  DWIN_Word(i, xStart);
  DWIN_Word(i, yStart);
  DWIN_Word(i, xEnd);
  DWIN_Word(i, yEnd);
  DWIN_Word(i, x);
  DWIN_Word(i, y);
  DWIN_Send(i);
}

#endif // DWIN_CREALITY_LCD
