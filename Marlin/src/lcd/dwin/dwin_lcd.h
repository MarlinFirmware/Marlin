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

/********************************************************************************
 * @file     dwin_lcd.h
 * @author   LEO / Creality3D
 * @date     2019/07/18
 * @version  2.0.1
 * @brief    迪文屏控制操作函数
 ********************************************************************************/

#include <stdint.h>

#define RECEIVED_NO_DATA         0x00
#define RECEIVED_SHAKE_HAND_ACK  0x01

#define FHONE                    0xAA

#define DWIN_SCROLL_UP   2
#define DWIN_SCROLL_DOWN 3

#define DWIN_WIDTH  272
#define DWIN_HEIGHT 480

/*接收数据解析 返回值:true,接收到数据;false,未接收到数据*/
bool DWIN_ReceiveAnalyze(void);

/*发送当前BUF中的数据以及包尾数据 len:整包数据长度*/
void DWIN_Send_BufTail(const uint8_t len);

/*----------------------------------------------系统变量函数----------------------------------------------*/
/*握手 1: 握手成功  2: 握手失败*/
bool DWIN_Handshake(void);

/*设定背光亮度 luminance:亮度(0x00~0xFF)*/
void DWIN_Backlight_SetLuminance(const uint8_t luminance);

/*设定画面显示方向 dir:0,0°; 1,90°; 2,180°; 3,270°*/
void DWIN_Frame_SetDir(uint8_t dir);

/*更新显示*/
void DWIN_UpdateLCD(void);

/*----------------------------------------------绘图相关函数----------------------------------------------*/
/*画面清屏 color:清屏颜色*/
void DWIN_Frame_Clear(const uint16_t color);

/*画面画线 color:线段颜色 xStart:X起始坐标 yStart:Y起始坐标 xEnd:X终止坐标 yEnd:Y终止坐标*/
void DWIN_Draw_Line(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

/*画面画矩形 mode:0,外框;1,填充;2,异或填充 color:颜色 xStart/yStart:矩形左上坐标 xEnd/yEnd:矩形右下坐标*/
void DWIN_Draw_Rectangle(uint8_t mode, uint16_t color,
                         uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

/*画面区域移动 mode:0,环移;1,平移 dir:0,向左移动;1,向右移动;2,向上移动;3,向下移动 dis:移动距离
                color:填充颜色 xStart/yStart:选定区域左上坐标 xEnd/yEnd:选定区域右下坐标*/
void DWIN_Frame_AreaMove(uint8_t mode, uint8_t dir, uint16_t dis,
                         uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);

/*----------------------------------------------文本相关函数----------------------------------------------*/
/*画面显示字符串 widthAdjust:true,自调整字符宽度;false,不调整字符宽度 bShow:true,显示背景色;false,不显示背景色 size:字号大小
                  color:字符颜色 bColor:背景颜色 x/y:字符串左上坐标 *string:字符串*/
void DWIN_Draw_String(bool widthAdjust, bool bShow, uint8_t size,
                      uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, char *string);

/*画面显示正整数 bShow:true,显示背景色;false,不显示背景色 zeroFill:true,补零;false,不补零 zeroMode:1,无效0显示为0; 0,无效0显示为空格 size:字号大小
                  color:字符颜色 bColor:背景颜色 iNum:位数 x/y:变量左上坐标 value:整型变量*/
void DWIN_Draw_IntValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                          uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, uint16_t value);

/*画面显示浮点数 bShow:true,显示背景色;false,不显示背景色 zeroFill:true,补零;false,不补零 zeroMode:1,无效0显示为0; 0,无效0显示为空格 size:字号大小
                  color:字符颜色 bColor:背景颜色 iNum:整数位数 fNum:小数位数 x/y:变量左上坐标 value:浮点数变量*/
void DWIN_Draw_FloatValue(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color,
                            uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value);

/*----------------------------------------------图片相关函数----------------------------------------------*/
/*jpg图片显示并缓存在#0虚拟显示区 id:图片ID*/
void DWIN_JPG_ShowAndCache(const uint8_t id);

/*图标显示 libID:图标库ID picID:图标ID x/y:图标左上坐标*/
void DWIN_ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y);

/*jpg图片解压到#1虚拟显示区 id:图片ID*/
void DWIN_JPG_CacheToN(uint8_t n, uint8_t id);

/*jpg图片解压到#1虚拟显示区 id:图片ID*/
inline void DWIN_JPG_CacheTo1(uint8_t id) { DWIN_JPG_CacheToN(1, id); }

/*从虚拟显示区复制区域至当前画面 cacheID:虚拟区号 xStart/yStart:虚拟区左上坐标 xEnd/yEnd:虚拟区右下坐标 x/y:当前画面粘贴坐标*/
void DWIN_Frame_AreaCopy(uint8_t cacheID, uint16_t xStart, uint16_t yStart,
                         uint16_t xEnd, uint16_t yEnd, uint16_t x, uint16_t y);
