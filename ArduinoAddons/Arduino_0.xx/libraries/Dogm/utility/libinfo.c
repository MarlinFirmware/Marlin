/*

  libinfo.c

  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 library.

  The dogm128 library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.

  Controller SPI Interface Selection 
    DOG_SPI_USI		Universal Serial Interface of ATTINY controller
    DOG_SPI_ATMEGA	ATMEGA SPI Interface
    DOG_SPI_ARDUINO	SPI Interface for ATMEGA within Arduino Environment
    nothing defined		defaults to DOG_SPI_ARDUINO

  Display Selection
  one of the following constants could be defined:
    DOGM128_HW		DOGM128 Display
    DOGS102_HW		DOGS102 Display
    DOGM132_HW		DOGM132 Display
    DOGXL60_HW_BW	DOGXL160 Display
    DOGXL60_HW_GR	DOGXL160 Display
    nothing defined		defaults to DOGM128 Display
    
  Normal or Revers Display Mode
    DOG_REVERSE		Display output rotated by 180 degree
    nothing defined		No rotation of the output
    
*/

#include "dogm128.h"

#define LH 8

void libinfo_draw(void)
{
  uint8_t x;
  uint8_t y = DOG_HEIGHT - LH;
#ifdef ADA_ST7565P_HW
  dog_DrawStrP(0, y, font_4x6, DOG_PSTR("ST7565P"));
  y -= LH;
#endif
#ifdef DOGM128_HW
  dog_DrawStrP(0, y, font_4x6, DOG_PSTR("M128"));
  y -= LH;
#endif
#ifdef DOGS102_HW
  dog_DrawStrP(0, y, font_4x6, DOG_PSTR("S102"));
  y -= LH;
#endif
#ifdef DOGM132_HW
  dog_DrawStrP(0, y, font_4x6, DOG_PSTR("M132"));
  y -= LH;
#endif
#ifdef DOGXL160_HW_BW
  dog_DrawStrP(0, y, font_4x6, DOG_PSTR("XL160 BW"));
  y -= LH;
#endif
#ifdef DOGXL160_HW_GR
  dog_DrawStrP(0, y, font_4x6, DOG_PSTR("XL160 GR"));
  y -= LH;
#endif

#ifdef DOG_REVERSE
  dog_DrawStrP(0, y, font_4x6, DOG_PSTR("REVERSE"));
  y -= LH;
#endif

#ifdef DOG_DOUBLE_MEMORY
  dog_DrawStrP(0, y, font_4x6, DOG_PSTR("DOUBLE_MEM"));
  y -= LH;
#endif

#ifdef DOG_SPI_SW_ARDUINO
  dog_DrawStrP(0, y, font_4x6, DOG_PSTR("SW SPI"));
  y -= LH;
#endif

  x = dog_DrawStrP(0, y, font_4x6, DOG_PSTR("Pages "));
  dog_DrawStr(x, y, font_4x6, dog_itoa(DOG_PAGE_CNT));
  y -= LH;

}



