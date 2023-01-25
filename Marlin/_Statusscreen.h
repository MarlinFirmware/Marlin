/**
 * Made with Marlin Bitmap Converter
 * https://marlinfw.org/tools/u8glib/converter.html
 *
 * This bitmap from the file '.unhappy.JPG'
 */
#pragma once

#define STATUS_LOGO_Y            6
#define STATUS_LOGO_WIDTH       40

const unsigned char status_logo_bmp[] PROGMEM = {
  B00000000,B00111111,B11111111,B11110000,B00000000, // �����?????????������
  B00000001,B11111111,B11111111,B11111110,B00000000, // ���?????????????����
  B00000111,B11111111,B11111111,B11111111,B10000000, // ��???????????????���
  B00001111,B11111111,B11111111,B11111111,B11100000, // ��????????????????��
  B00011111,B11111111,B11111111,B11111111,B11110000, // �?????????????????��
  B00111110,B00011111,B11111100,B00011111,B11111000, // �???�??????��??????�
  B01110001,B11111111,B11111111,B11100011,B11111000, // ??�???????????�????�
  B01100111,B11111111,B11111111,B11111000,B11111000, // ???????????????�???�
  B11111111,B00000011,B11110000,B00011111,B11111100, // ????���???���??????�
  B11111100,B11000011,B11000110,B00011111,B11111100, // ???�?��??�??�??????�
  B11111111,B11111111,B11111111,B11111111,B11111100, // ???????????????????�
  B01111111,B11111111,B11111111,B11111111,B11111100, // ???????????????????�
  B01111111,B11111111,B11111111,B11111111,B11111000, // ???????????????????�
  B00111111,B11111111,B11110001,B11111111,B11111000, // �?????????�????????�
  B00011111,B11110100,B00000111,B11111111,B11110000, // �??????���????????��
  B00001111,B11111111,B11111111,B11111111,B11100000, // ��????????????????��
  B00000111,B11111111,B11111111,B11111111,B10000000, // ��???????????????���
  B00000001,B11111111,B11111111,B11111110,B00000000, // ���?????????????����
  B00000000,B00111111,B11111111,B11110000,B00000000, // �����?????????������
  B00000000,B00000001,B11111110,B00000000,B00000000  // �������?????��������
};


#define STATUS_HOTEND_ANIM
#define STATUS_BED_ANIM
#define STATUS_HEATERS_X      48
#define STATUS_BED_X          74

///**
// * Marlin 3D Printer Firmware
// * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
// *
// * Based on Sprinter and grbl.
// * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program.  If not, see <http://www.gnu.org/licenses/>.
// *
// */
//#pragma once
//
// /**
//  * Custom Status Screen bitmap
//  *
//  * Place this file in the root with your configuration files
//  * and enable CUSTOM_STATUS_SCREEN_IMAGE in Configuration.h.
//  *
//  * Use the Marlin Bitmap Converter to make your own:
//  * http://marlinfw.org/tools/u8glib/converter.html
//  */
//
//  //
//  // Status Screen Logo bitmap
//  //
//#define STATUS_LOGO_Y            6
//#define STATUS_LOGO_WIDTH       40
//
//const unsigned char status_logo_bmp[] PROGMEM = {
//  B01110000,B00000000,B00001000,B00000011,B10001110,
//  B10001000,B00000000,B00001000,B00000100,B01010001,
//  B10001010,B11000111,B00111110,B00000100,B01010001,
//  B11111011,B00101000,B10001000,B00000111,B11001110,
//  B10001010,B00101111,B00001000,B00000100,B01010001,
//  B10001010,B00101000,B00001000,B00000100,B01010001,
//  B10001010,B00100111,B00000110,B00000100,B01001110,
//  B00000000,B00000000,B00000000,B00000000,B00000000,
//  B00000000,B00000000,B00000000,B00000000,B00000000,
//  B00000000,B00000000,B00000000,B00000000,B00000000,
//  B00000000,B00001110,B01000000,B00000000,B00000000,
//  B00000000,B00010001,B01000000,B00000000,B00000000,
//  B00000000,B00010001,B01010001,B00111000,B00000000,
//  B00000000,B00011110,B01010001,B01000000,B00000000,
//  B00000000,B00010000,B01010001,B00111000,B00000000,
//  B00000000,B00010000,B01010001,B00000100,B00000000,
//  B00000000,B00010000,B01001110,B00111000,B00000000
//};
//
////
//// Use default bitmaps
////
//#define STATUS_HOTEND_ANIM
//#define STATUS_BED_ANIM
//#define STATUS_HEATERS_X      48
//#define STATUS_BED_X          74
