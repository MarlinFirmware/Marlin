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

/*****************************************************************************
 * 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *    pInput      指向输入缓冲区, 以UTF-8编码
 *    Unic        指向输出缓冲区, 其保存的数据即是Unicode编码值,
 *                类型为unsigned long .
 *
 * 返回值:
 *    成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/

#include "../../../inc/MarlinConfig.h"

#include "utf8_unicode.h"
#include "stdio.h"

int enc_utf8_to_unicode(const char* pInput, unsigned short int *Unic) {
  unsigned short int OneUnic = 0x0;
  if (pInput == nullptr || Unic == nullptr) {
    //SERIAL_ECHOLNPGM("\r\n enc_utf8_to_unicode inout err, pInput = ", pInput,
    //                    "Unic = ", Unic);
    return false;
  }
  int len = strlen(pInput);
  //SERIAL_ECHOLNPGM("\r\n strlen(pInput) = ", strlen(pInput),
  //                  "\r\n len = ", len,
  //                  "\r\n sizeof(pInput) = ", sizeof(pInput),
  //                  "\r\n sizeof(Unic) = ", sizeof(Unic),);
  for (int i = 0; i < len; i++) {
    enc_utf8_to_unicode_one((unsigned char*) &pInput[i], &OneUnic);
    Unic[i] = OneUnic;
    hal.watchdog_refresh();
    // char buf[20] = {0};
    // char buf1[10] = {0};
    // memcpy(buf1, Unic, 10);

    // sprintf(buf, "%x %x %x %x %x %x%x %x", buf1[0], buf1[1], buf1[2], buf1[3], buf1[4], buf1[5], buf1[6], buf1[7]);
    // SERIAL_ECHOLNPGM("\r\n i = ", i,
    //                   "\r\n len = ", len,
    //                   "\r\n buf1 = ", buf);
  }
  return true;
}


int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned short int *Unic) {
  //assert(pInput != NULL && Unic != NULL);
  if (pInput == nullptr || Unic == nullptr) return false;

  // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
  char b1, b2, b3, b4, b5, b6;

  *Unic = 0x0; // 把 *Unic 初始化为全零
  int utfbytes = enc_get_utf8_size(*pInput);
  unsigned char *pOutput = (unsigned char *) Unic;

  switch (utfbytes) {
    case 0:
      *pOutput     = *pInput;
      utfbytes    += 1;
      break;
    case 2:
      b1 = *pInput;
      b2 = *(pInput + 1);
      if ( (b2 & 0xE0) != 0x80 )
          return 0;
      *pOutput     = (b1 << 6) + (b2 & 0x3F);
      *(pOutput+1) = (b1 >> 2) & 0x07;
      break;
    case 3:
      b1 = *pInput;
      b2 = *(pInput + 1);
      b3 = *(pInput + 2);
      if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
          return 0;
      *pOutput     = (b2 << 6) + (b3 & 0x3F);
      *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
      break;
    case 4:
      b1 = *pInput;
      b2 = *(pInput + 1);
      b3 = *(pInput + 2);
      b4 = *(pInput + 3);
      if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
              || ((b4 & 0xC0) != 0x80) )
          return 0;
      *pOutput     = (b3 << 6) + (b4 & 0x3F);
      *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
      *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
      break;
    case 5:
      b1 = *pInput;
      b2 = *(pInput + 1);
      b3 = *(pInput + 2);
      b4 = *(pInput + 3);
      b5 = *(pInput + 4);
      if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
              || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
          return 0;
      *pOutput     = (b4 << 6) + (b5 & 0x3F);
      *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
      *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
      *(pOutput+3) = (b1 << 6);
      break;
    case 6:
      b1 = *pInput;
      b2 = *(pInput + 1);
      b3 = *(pInput + 2);
      b4 = *(pInput + 3);
      b5 = *(pInput + 4);
      b6 = *(pInput + 5);
      if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
              || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
              || ((b6 & 0xC0) != 0x80) )
          return 0;
      *pOutput     = (b5 << 6) + (b6 & 0x3F);
      *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
      *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
      *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
      break;

    default: return 0;
  }

  return utfbytes;
}

int enc_get_utf8_size(const unsigned char pInput) {
  unsigned char c = pInput;
  // 0xxxxxxx 返回0
  // 10xxxxxx 不存在
  // 110xxxxx 返回2
  // 1110xxxx 返回3
  // 11110xxx 返回4
  // 111110xx 返回5
  // 1111110x 返回6
  if (c <  0x80) return 0;
  if (c >= 0x80 && c < 0xC0) return -1;
  if (c >= 0xC0 && c < 0xE0) return 2;
  if (c >= 0xE0 && c < 0xF0) return 3;
  if (c >= 0xF0 && c < 0xF8) return 4;
  if (c >= 0xF8 && c < 0xFC) return 5;
  if (c >= 0xFC) return 6;

  return -1;
}

// #c---end
