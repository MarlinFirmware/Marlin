/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * DWIN G-code thumbnail preview
 * Author: Miguel A. Risco-Castillo
 * version: 3.1.2
 * Date: 2022/09/03
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * For commercial applications additional licenses can be requested
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(DWIN_LCD_PROUI, HAS_GCODE_PREVIEW)

#include "../../../core/types.h"
#include "../../marlinui.h"
#include "../../../sd/cardreader.h"
#include "../../../MarlinCore.h" // for wait_for_user
#include "dwin_lcd.h"
#include "dwinui.h"
#include "dwin.h"
#include "dwin_popup.h"
#include "base64.hpp"
#include "gcode_preview.h"

typedef struct {
  char name[13] = "";   //8.3 + null
  uint32_t thumbstart = 0;
  int thumbsize = 0;
  int thumbheight = 0;
  int thumbwidth = 0;
  uint8_t *thumbdata = nullptr;
  float time = 0;
  float filament = 0;
  float layer = 0;
  float width = 0;
  float height = 0;
  float length = 0;
  void setname(const char * const fn);
  void clear();
} fileprop_t;
fileprop_t fileprop;

void fileprop_t::setname(const char * const fn) {
  const uint8_t len = _MIN(sizeof(name) - 1, strlen(fn));
  memcpy(&name[0], fn, len);
  name[len] = '\0';
}

void fileprop_t::clear() {
  fileprop.name[0] = '\0';
  fileprop.thumbstart = 0;
  fileprop.thumbsize = 0;
  fileprop.thumbheight = 0;
  fileprop.thumbwidth = 0;
  fileprop.thumbdata = nullptr;
  fileprop.time = 0;
  fileprop.filament = 0;
  fileprop.layer = 0;
  fileprop.height = 0;
  fileprop.width = 0;
  fileprop.length = 0;
}

void Get_Value(char *buf, const char * const key, float &value) {
  char num[10] = "";
  char * posptr = 0;
  uint8_t i = 0;
  if (!!value) return;
  posptr = strstr(buf, key);
  if (posptr != nullptr) {
    while (i < sizeof(num)) {
      char c = posptr[0];
      if (!ISEOL(c) && (c != 0)) {
        if ((c > 47 && c < 58) || (c == '.')) num[i++] = c;
        posptr++;
      }
      else {
        num[i] = '\0';
        value = atof(num);
        return;
      }
    }
  }
}

bool Has_Preview() {
  const char * tbstart = "; thumbnail begin 230x180";
  char * posptr = 0;
  uint8_t nbyte = 1;
  uint32_t indx = 0;
  char buf[256];
  float tmp = 0;

  fileprop.clear();
  fileprop.setname(card.filename);

  card.openFileRead(fileprop.name);

  while ((nbyte > 0) && (indx < 4 * sizeof(buf)) && !fileprop.thumbstart) {
    nbyte = card.read(buf, sizeof(buf) - 1);
    if (nbyte > 0) {
      buf[nbyte] = '\0';
      Get_Value(buf, ";TIME:", fileprop.time);
      Get_Value(buf, ";Filament used:", fileprop.filament);
      Get_Value(buf, ";Layer height:", fileprop.layer);
      Get_Value(buf, ";MINX:", tmp);
      Get_Value(buf, ";MAXX:", fileprop.width);
      fileprop.width -= tmp;
      tmp = 0;
      Get_Value(buf, ";MINY:", tmp);
      Get_Value(buf, ";MAXY:", fileprop.length);
      fileprop.length -= tmp;
      tmp = 0;
      Get_Value(buf, ";MINZ:", tmp);
      Get_Value(buf, ";MAXZ:", fileprop.height);
      fileprop.height -= tmp;
      posptr = strstr(buf, tbstart);
      if (posptr != NULL) {
        fileprop.thumbstart = indx + (posptr - &buf[0]);
      }
      else {
        indx += _MAX(10, nbyte - (signed)strlen(tbstart));
        card.setIndex(indx);
      }
    }
  }

  if (!fileprop.thumbstart) {
    card.closefile();
    LCD_MESSAGE_F("Thumbnail not found");
    return 0;
  }

  // Get the size of the thumbnail
  card.setIndex(fileprop.thumbstart + strlen(tbstart));
  for (uint8_t i = 0; i < 16; i++) {
    char c = card.get();
    if (!ISEOL(c)) {
      buf[i] = c;
    }
    else {
      buf[i] = 0;
      break;
    }
  }
  fileprop.thumbsize = atoi(buf);

  // Exit if there isn't a thumbnail
  if (!fileprop.thumbsize) {
    card.closefile();
    LCD_MESSAGE_F("Invalid Thumbnail Size");
    return 0;
  }

  uint16_t readed = 0;
  uint8_t buf64[fileprop.thumbsize];

  fileprop.thumbdata = new uint8_t[3 + 3 * (fileprop.thumbsize / 4)];  // Reserve space for the JPEG thumbnail

  while (readed < fileprop.thumbsize) {
    uint8_t c = card.get();
    if (!ISEOL(c) && (c != ';') && (c != ' ')) {
      buf64[readed] = c;
      readed++;
    }
  }
  card.closefile();
  buf64[readed] = 0;

  fileprop.thumbsize = decode_base64(buf64, fileprop.thumbdata);  card.closefile();
  DWINUI::WriteToSRAM(0x00, fileprop.thumbsize, fileprop.thumbdata);
  delete[] fileprop.thumbdata;
  return true;
}

void Preview_DrawFromSD() {
  if (Has_Preview()) {
    char buf[46];
    char str_1[6] = "";
    char str_2[6] = "";
    char str_3[6] = "";
    DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, 0, DWIN_WIDTH, STATUS_Y - 1);
    if (fileprop.time) {
      sprintf_P(buf, PSTR("Estimated time: %i:%02i"), (uint16_t)fileprop.time / 3600, ((uint16_t)fileprop.time % 3600) / 60);
      DWINUI::Draw_String(20, 10, buf);
    }
    if (fileprop.filament) {
      sprintf_P(buf, PSTR("Filament used: %s m"), dtostrf(fileprop.filament, 1, 2, str_1));
      DWINUI::Draw_String(20, 30, buf);
    }
    if (fileprop.layer) {
      sprintf_P(buf, PSTR("Layer height: %s mm"), dtostrf(fileprop.layer, 1, 2, str_1));
      DWINUI::Draw_String(20, 50, buf);
    }
    if (fileprop.width) {
      sprintf_P(buf, PSTR("Volume: %sx%sx%s mm"), dtostrf(fileprop.width, 1, 1, str_1), dtostrf(fileprop.length, 1, 1, str_2), dtostrf(fileprop.height, 1, 1, str_3));
      DWINUI::Draw_String(20, 70, buf);
    }
    DWINUI::Draw_Button(BTN_Print, 26, 290);
    DWINUI::Draw_Button(BTN_Cancel, 146, 290);
    DWIN_ICON_Show(0, 0, 1, 21, 90, 0x00);
    Draw_Select_Highlight(true, 290);
    DWIN_UpdateLCD();
  }
  else {
    HMI_flag.select_flag = 1;
    wait_for_user = false;
  }
}

void Preview_Invalidate() {
  fileprop.thumbstart = 0;
}

bool Preview_Valid() {
  return !!fileprop.thumbstart;
}

void Preview_Reset() {
  fileprop.thumbsize = 0;
}

#endif // HAS_GCODE_PREVIEW && DWIN_LCD_PROUI
