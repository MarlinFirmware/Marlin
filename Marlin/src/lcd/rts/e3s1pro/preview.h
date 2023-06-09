/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/*****************  gcode内嵌预览图读取相关定义(开始)  ******************/
// 图片文件标识位(长度4字节)
#define PIC_HEADER      "begin"
//#define PIC_PRESENT_LEN 4
// 图片长度位(长度4字节)
//#define PIC_DATA_LEN  4
// 图片类型(长度1字节)
//#define PIC_FORMAT_LEN  1   // 图片格式长度（字节）

enum{
  PIC_FORMAT_JPG    = 0x00, // jpg格式图片
  PIC_FORMAT_PNG    = 0x01, // png格式图片
  PIC_FORMAT_MAX    = 0x02  //
};

#define FORMAT_JPG "jpg"
#define FORMAT_PNG "png"
// 分辨率(长度1字节)
//#define PIC_RESOLUTION_LEN    1   // 图片分辨率长度（字节）

enum{
  PIC_RESOLUTION_36_36   = 0x00, // 分辨率 = 36*36
  PIC_RESOLUTION_48_48   = 0x01,  // 分辨率 = 48*48
  PIC_RESOLUTION_64_64   = 0x02,  // 分辨率 = 64*64
  PIC_RESOLUTION_96_96   = 0x03,  // 分辨率 = 96*96
  PIC_RESOLUTION_144_144 = 0x04,  // 分辨率 = 144*144
  PIC_RESOLUTION_200_200 = 0x05,  // 分辨率 = 200*200
  PIC_RESOLUTION_300_300 = 0x06,  // 分辨率 = 300*300
  PIC_RESOLUTION_600_600,
  PIC_RESOLUTION_MAX,   // gcode无图片
};

typedef struct {
  unsigned long addr;         // 变量地址
  unsigned long spAddr;       // 描述指针
  unsigned int  brightness;   // 亮度（0x0000 - 0x0100, 单位为 1/256）
  unsigned int  LeftUp_X;     // 显示区域的左上角X
  unsigned int  LeftUp_Y;     // 显示区域的左上角Y
  unsigned int  RightDown_X;  // 显示区域的右下角X
  unsigned int  RightDown_Y;  // 显示区域的右下角Y
} DwinBrightness_t;

#define RESOLUTION_36_36    "36*36"
#define RESOLUTION_48_48    "48*48"
#define RESOLUTION_64_64    "64*64"
#define RESOLUTION_96_96    "96*96"
#define RESOLUTION_144_144  "144*144"
#define RESOLUTION_200_200  "200*200"
#define RESOLUTION_300_300  "300*300"
#define RESOLUTION_600_600  "600*600"
#define VP_BRIGHTNESS_PRINT               0x8800

#define VP_OVERLAY_PIC_PTINT              0xB000  // 打印界面的预览图

// 打印界面的模型亮度，用于表示倒计时的一种方式(占40字节，0xA000 ~ 0xA01F)
#define SP_ADDR_BRIGHTNESS_PRINT          0x9000
// 模型行起始位(2字节)
//#define PIC_START_LINE_LEN  2   // 图片起始行长度（字节）
// 模型行结束位(2字节)
//#define PIC_END_LINE_LEN    2   // 图片结束行长度（字节）
// 模型高度(2字节)
//#define PIC_HIGH_LINE_LEN   2   // 图片高度长度（字节）
// 图片数据前引导位
//#define DATA_BEFOR_PIC_LENTH  (PIC_FORMAT_LEN + PIC_RESOLUTION_LEN + PIC_START_LINE_LEN + PIC_END_LINE_LEN + PIC_HIGH_LINE_LEN)

// 函数返回信息
enum {
  PIC_OK,             // 图片显示ok
  PIC_FORMAT_ERR,     // 图片格式错误
  PIC_RESOLUTION_ERR, // 图片分辨率错误
  PIC_MISS_ERR,       // gcode无图片
};

#define PREVIEW_PIC_FORMAT_NEED     PIC_FORMAT_JPG
#define PREVIEW_PIC_RESOLUTION_NEED PIC_RESOLUTION_96_96
#define PRINT_PIC_FORMAT_NEED       PIC_FORMAT_JPG
#define PRINT_PIC_RESOLUTION_NEED   PIC_RESOLUTION_300_300

#define DACAI_PREVIEW_PIC_ADDR1  "3:/20.jpg"  // 大彩文件选择预览图1
#define DACAI_PREVIEW_PIC_ADDR2  "3:/22.jpg"  // 大彩文件选择预览图2
#define DACAI_PREVIEW_PIC_ADDR3  "3:/23.jpg"  // 大彩文件选择预览图3
#define DACAI_PREVIEW_PIC_ADDR4  "3:/24.jpg"  // 大彩文件选择预览图4
#define DACAI_PRINT_PIC_ADDR     "3:/31.jpg"  // 大彩打印预览图1

/*****************  gcode内嵌预览图读取相关定义(结束)  ******************/
void RefreshBrightnessAtPrint(uint16_t persent);
bool gcodePicGetDataFormBase64(char * buf, unsigned long picLen, bool resetFlag);
bool gcodePicDataRead(unsigned long picLenth, char isDisplay, unsigned long jpgAddr);
char gcodePicExistjudge(char *fileName, unsigned int targetPicAddr, const char targetPicFormat, const char targetPicResolution);
char gcodePicDataSendToDwin(char *fileName, unsigned int jpgAddr, unsigned char jpgFormat, unsigned char jpgResolution);
void gcodePicDisplayOnOff(unsigned int jpgAddr, bool onoff);
