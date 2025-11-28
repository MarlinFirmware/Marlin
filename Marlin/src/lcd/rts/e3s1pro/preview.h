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

/*****************  Gcode embedded preview reading related definition (start)  ******************/
// Image file identification bit (4 bytes in length)
#define PIC_HEADER      "begin"
//#define PIC_PRESENT_LEN 4
// Picture length bit (length 4 bytes)
//#define PIC_DATA_LEN  4
// Image type (length 1 byte)
//#define PIC_FORMAT_LEN  1   // Photo format length (byte)

enum{
  PIC_FORMAT_JPG    = 0x00, // JPG format picture
  PIC_FORMAT_PNG    = 0x01, // PNG format picture
  PIC_FORMAT_MAX    = 0x02  //
};

#define FORMAT_JPG "jpg"
#define FORMAT_PNG "png"
// Resolution (length 1 byte)
//#define PIC_RESOLUTION_LEN    1   // Picture resolution length (byte)

enum{
  PIC_RESOLUTION_36_36   = 0x00, // Resolution = 36*36
  PIC_RESOLUTION_48_48   = 0x01,  // Resolution = 48*48
  PIC_RESOLUTION_64_64   = 0x02,  // Resolution = 64*64
  PIC_RESOLUTION_96_96   = 0x03,  // Resolution = 96*96
  PIC_RESOLUTION_144_144 = 0x04,  // Resolution = 144*144
  PIC_RESOLUTION_200_200 = 0x05,  // Resolution = 200*200
  PIC_RESOLUTION_300_300 = 0x06,  // Resolution = 300*300
  PIC_RESOLUTION_600_600,
  PIC_RESOLUTION_MAX,   // gcode no picture
};

typedef struct {
  unsigned long addr;         // Variable address
  unsigned long spAddr;       // Description pointer
  unsigned int  brightness;   // Brightness (0x0000-0x0100, unit is 1/256)
  unsigned int  LeftUp_X;     // Show the upper left corner of the area X
  unsigned int  LeftUp_Y;     // The upper left corner of the display area y
  unsigned int  RightDown_X;  // Show the lower right corner of the area x
  unsigned int  RightDown_Y;  // The lower right corner of the display area y
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

#define VP_OVERLAY_PIC_PTINT              0xB000  // Preview of the print interface

// The model brightness of the print interface is used to indicate a way of countdown (40 bytes, 0xa000 ~ 0xa01F)
#define SP_ADDR_BRIGHTNESS_PRINT          0x9000
// The model starts at the beginning (2 bytes)
//#define PIC_START_LINE_LEN  2   // Picture starting length (byte)
// The end of the model line (2 bytes)
//#define PIC_END_LINE_LEN    2   // Image ending the governor (byte)
// Model height (2 bytes)
//#define PIC_HIGH_LINE_LEN   2   // Picture height length (byte)
// Pre -guidance position of picture data
//#define DATA_BEFOR_PIC_LENTH  (PIC_FORMAT_LEN + PIC_RESOLUTION_LEN + PIC_START_LINE_LEN + PIC_END_LINE_LEN + PIC_HIGH_LINE_LEN)

// 函数返回信息
enum {
  PIC_OK,             // Picture show OK
  PIC_FORMAT_ERR,     // Image format error
  PIC_RESOLUTION_ERR, // Picture resolution error
  PIC_MISS_ERR,       // gcode no picture
};

#define PREVIEW_PIC_FORMAT_NEED     PIC_FORMAT_JPG
#define PREVIEW_PIC_RESOLUTION_NEED PIC_RESOLUTION_96_96
#define PRINT_PIC_FORMAT_NEED       PIC_FORMAT_JPG
#define PRINT_PIC_RESOLUTION_NEED   PIC_RESOLUTION_300_300

#define DACAI_PREVIEW_PIC_ADDR1  "3:/20.jpg"  // Preview Figure 1 of Da Cai File 1
#define DACAI_PREVIEW_PIC_ADDR2  "3:/22.jpg"  // Preview Preview of Da Cai File 2
#define DACAI_PREVIEW_PIC_ADDR3  "3:/23.jpg"  // Preview Figure 3 of Da Cai File 3
#define DACAI_PREVIEW_PIC_ADDR4  "3:/24.jpg"  // Preview Figure 4 of Da Cai File 4
#define DACAI_PRINT_PIC_ADDR     "3:/31.jpg"  // Da Cai Print Preview Figure 1

/*****************  Gcode embedded preview reading related definition (end)  ******************/

void refreshBrightnessAtPrint(const uint16_t percent);
bool gcodePicGetDataFormBase64(char * buf, unsigned long picLen, bool resetFlag);
bool gcodePicDataRead(unsigned long picLenth, char isDisplay, unsigned long jpgAddr);
char gcodePicExistjudge(char *fileName, unsigned int targetPicAddr, const char targetPicFormat, const char targetPicResolution);
char gcodePicDataSendToDwin(char *fileName, unsigned int jpgAddr, unsigned char jpgFormat, unsigned char jpgResolution);
void gcodePicDisplayOnOff(unsigned int jpgAddr, bool onoff);
