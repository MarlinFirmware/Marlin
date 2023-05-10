/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.20.1
 * Date: 2022/10/25
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(DWIN_LCD_PROUI)

#include "dwin_defines.h"
#include "dwin_lcd.h"
#include "dwinui.h"

xy_int_t DWINUI::cursor = { 0 };
uint16_t DWINUI::pencolor = Color_White;
uint16_t DWINUI::textcolor = Def_Text_Color;
uint16_t DWINUI::backcolor = Def_Background_Color;
uint16_t DWINUI::buttoncolor = Def_Button_Color;
uint8_t  DWINUI::fontid = font8x16;
FSTR_P const DWINUI::Author = F(STRING_CONFIG_H_AUTHOR);

void (*DWINUI::onTitleDraw)(TitleClass* title) = nullptr;

void DWINUI::init() {
  cursor.reset();
  pencolor = Color_White;
  textcolor = Def_Text_Color;
  backcolor = Def_Background_Color;
  buttoncolor = Def_Button_Color;
  fontid = font8x16;
}

// Set text/number font
void DWINUI::setFont(fontid_t fid) { fontid = fid; }

// Get font character width
uint8_t DWINUI::fontWidth(fontid_t fid) {
  switch (fid) {
    case font6x12 : return 6;
    case font8x16 : return 8;
    case font10x20: return 10;
    case font12x24: return 12;
    case font14x28: return 14;
    case font16x32: return 16;
    case font20x40: return 20;
    case font24x48: return 24;
    case font28x56: return 28;
    case font32x64: return 32;
    default: return 0;
  }
}

// Get font character height
uint8_t DWINUI::fontHeight(fontid_t fid) {
  switch (fid) {
    case font6x12 : return 12;
    case font8x16 : return 16;
    case font10x20: return 20;
    case font12x24: return 24;
    case font14x28: return 28;
    case font16x32: return 32;
    case font20x40: return 40;
    case font24x48: return 48;
    case font28x56: return 56;
    case font32x64: return 64;
    default: return 0;
  }
}

// Get screen x coordinates from text column
uint16_t DWINUI::ColToX(uint8_t col) { return col * fontWidth(fontid); }

// Get screen y coordinates from text row
uint16_t DWINUI::RowToY(uint8_t row) { return row * fontHeight(fontid); }

// Set text/number color
void DWINUI::SetColors(uint16_t fgcolor, uint16_t bgcolor, uint16_t alcolor) {
  textcolor = fgcolor;
  backcolor = bgcolor;
  buttoncolor = alcolor;
}
void DWINUI::SetTextColor(uint16_t fgcolor) {
  textcolor = fgcolor;
}
void DWINUI::SetBackgroundColor(uint16_t bgcolor) {
  backcolor = bgcolor;
}

// Moves cursor to point
//  x: abscissa of the display
//  y: ordinate of the display
//  point: xy coordinate
void DWINUI::MoveTo(int16_t x, int16_t y) {
  cursor.x = x;
  cursor.y = y;
}
void DWINUI::MoveTo(xy_int_t point) {
  cursor = point;
}

// Moves cursor relative to the actual position
//  x: abscissa of the display
//  y: ordinate of the display
//  point: xy coordinate
void DWINUI::MoveBy(int16_t x, int16_t y) {
  cursor.x += x;
  cursor.y += y;
}
void DWINUI::MoveBy(xy_int_t point) {
  cursor += point;
}

// Draw a Centered string using arbitrary x1 and x2 margins
void DWINUI::Draw_CenteredString(bool bShow, fontid_t fid, uint16_t color, uint16_t bColor, uint16_t x1, uint16_t x2, uint16_t y, const char * const string) {
  const uint16_t x = _MAX(0U, x2 + x1 - strlen_P(string) * fontWidth(fid)) / 2 - 1;
  DWIN_Draw_String(bShow, fid, color, bColor, x, y, string);
}

// Draw a char
//  color: Character color
//  x: abscissa of the display
//  y: ordinate of the display
//  c: ASCII code of char
void DWINUI::Draw_Char(uint16_t color, uint16_t x, uint16_t y, const char c) {
  const char string[2] = { c, 0};
  DWIN_Draw_String(false, fontid, color, backcolor, x, y, string, 1);
}

// Draw a char at cursor position and increment cursor
void DWINUI::Draw_Char(uint16_t color, const char c) {
  Draw_Char(color, cursor.x, cursor.y, c);
  MoveBy(fontWidth(fontid), 0);
}

// Draw a string at cursor position
//  color: Character color
//  *string: The string
//  rlimit: For draw less chars than string length use rlimit
void DWINUI::Draw_String(const char * const string, uint16_t rlimit) {
  DWIN_Draw_String(false, fontid, textcolor, backcolor, cursor.x, cursor.y, string, rlimit);
  MoveBy(strlen(string) * fontWidth(fontid), 0);
}
void DWINUI::Draw_String(uint16_t color, const char * const string, uint16_t rlimit) {
  DWIN_Draw_String(false, fontid, color, backcolor, cursor.x, cursor.y, string, rlimit);
  MoveBy(strlen(string) * fontWidth(fontid), 0);
}

// Draw a numeric integer value
//  bShow: true=display background color; false=don't display background color
//  signedMode: 1=signed; 0=unsigned
//  fid: Font ID
//  color: Character color
//  bColor: Background color
//  iNum: Number of digits
//  x/y: Upper-left coordinate
//  value: Integer value
void DWINUI::Draw_Int(uint8_t bShow, bool signedMode, fontid_t fid, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, int32_t value) {
  char nstr[10];
  sprintf_P(nstr, PSTR("%*li"), (signedMode ? iNum + 1 : iNum), value);
  DWIN_Draw_String(bShow, fid, color, bColor, x, y, nstr);
}

// Draw a numeric float value
//  bShow: true=display background color; false=don't display background color
//  signedMode: 1=signed; 0=unsigned
//  fid: Font ID
//  color: Character color
//  bColor: Background color
//  iNum: Number of digits
//  fNum: Number of decimal digits
//  x/y: Upper-left coordinate
//  value: float value
void DWINUI::Draw_Float(uint8_t bShow, bool signedMode, fontid_t fid, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
  char nstr[10];
  DWIN_Draw_String(bShow, fid, color, bColor, x, y, dtostrf(value, iNum + (signedMode ? 2:1) + fNum, fNum, nstr));
}

// ------------------------- Icons -------------------------------//

// Draw an Icon and select library automatically
//  BG: The icon background display: false=Background filtering is not displayed, true=Background display
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
void DWINUI::ICON_Show(bool BG, uint8_t icon, uint16_t x, uint16_t y) {
  const uint8_t libID = ICON TERN_(HAS_CUSTOMICONS, + (icon / 100));
  const uint8_t picID = icon TERN_(HAS_CUSTOMICONS, % 100);
  DWIN_ICON_Show(BG, false, !BG, libID, picID, x, y);
}

// ------------------------- Buttons ------------------------------//

void DWINUI::Draw_Button(uint16_t color, uint16_t bcolor, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const char * const caption) {
  DWIN_Draw_Rectangle(1, bcolor, x1, y1, x2, y2);
  Draw_CenteredString(0, fontid, color, bcolor, x1, x2, (y2 + y1 - fontHeight())/2, caption);
}

void DWINUI::Draw_Button(uint8_t id, uint16_t x, uint16_t y) {
  switch (id) {
    case BTN_Cancel  : Draw_Button(GET_TEXT_F(MSG_BUTTON_CANCEL), x, y); break;
    case BTN_Confirm : Draw_Button(GET_TEXT_F(MSG_BUTTON_CONFIRM), x, y); break;
    case BTN_Continue: Draw_Button(GET_TEXT_F(MSG_BUTTON_CONTINUE), x, y); break;
    case BTN_Print   : Draw_Button(GET_TEXT_F(MSG_BUTTON_PRINT), x, y); break;
    case BTN_Save    : Draw_Button(GET_TEXT_F(MSG_BUTTON_SAVE), x, y); break;
    case BTN_Purge   : Draw_Button(GET_TEXT_F(MSG_BUTTON_PURGE), x, y); break;
    default: break;
  }
}

// -------------------------- Extra -------------------------------//

// Draw a circle
//  color: circle color
//  x: the abscissa of the center of the circle
//  y: ordinate of the center of the circle
//  r: circle radius
void DWINUI::Draw_Circle(uint16_t color, uint16_t x, uint16_t y, uint8_t r) {
  int a = 0, b = 0;
  while (a <= b) {
    b = SQRT(sq(r) - sq(a));
    if (a == 0) b--;
    DWIN_Draw_Point(color, 1, 1, x + a, y + b);   // Draw some sector 1
    DWIN_Draw_Point(color, 1, 1, x + b, y + a);   // Draw some sector 2
    DWIN_Draw_Point(color, 1, 1, x + b, y - a);   // Draw some sector 3
    DWIN_Draw_Point(color, 1, 1, x + a, y - b);   // Draw some sector 4
    DWIN_Draw_Point(color, 1, 1, x - a, y - b);   // Draw some sector 5
    DWIN_Draw_Point(color, 1, 1, x - b, y - a);   // Draw some sector 6
    DWIN_Draw_Point(color, 1, 1, x - b, y + a);   // Draw some sector 7
    DWIN_Draw_Point(color, 1, 1, x - a, y + b);   // Draw some sector 8
    a++;
  }
}

// Draw a circle filled with color
//  bcolor: fill color
//  x: the abscissa of the center of the circle
//  y: ordinate of the center of the circle
//  r: circle radius
void DWINUI::Draw_FillCircle(uint16_t bcolor, uint16_t x,uint16_t y,uint8_t r) {
  int a = 0, b = 0;
  while (a <= b) {
    b = SQRT(sq(r) - sq(a)); // b=sqrt(r*r-a*a);
    if (a == 0) b--;
    DWIN_Draw_Line(bcolor, x-b,y-a,x+b,y-a);
    DWIN_Draw_Line(bcolor, x-a,y-b,x+a,y-b);
    DWIN_Draw_Line(bcolor, x-b,y+a,x+b,y+a);
    DWIN_Draw_Line(bcolor, x-a,y+b,x+a,y+b);
    a++;
  }
}

// Color Interpolator
//  val : Interpolator minv..maxv
//  minv : Minimum value
//  maxv : Maximum value
//  color1 : Start color
//  color2 : End color
uint16_t DWINUI::ColorInt(int16_t val, int16_t minv, int16_t maxv, uint16_t color1, uint16_t color2) {
  uint8_t B, G, R;
  const float n = float(val - minv) / (maxv - minv);
  R = (1 - n) * GetRColor(color1) + n * GetRColor(color2);
  G = (1 - n) * GetGColor(color1) + n * GetGColor(color2);
  B = (1 - n) * GetBColor(color1) + n * GetBColor(color2);
  return RGB(R, G, B);
}

// Color Interpolator through Red->Yellow->Green->Blue (Pro UI)
//  val : Interpolator minv..maxv
//  minv : Minimum value
//  maxv : Maximum value
uint16_t DWINUI::RainbowInt(int16_t val, int16_t minv, int16_t maxv) {
  uint8_t B, G, R;
  const uint8_t maxB = 28, maxR = 28, maxG = 38;
  const int16_t limv = _MAX(abs(minv), abs(maxv));
  float n = minv >= 0 ? float(val - minv) / (maxv - minv) : (float)val / limv;
  LIMIT(n, -1, 1);
  if (n < 0) {
    R = 0;
    G = (1 + n) * maxG;
    B = (-n) * maxB;
  }
  else if (n < 0.5) {
    R = maxR * n * 2;
    G = maxG;
    B = 0;
  }
  else {
    R = maxR;
    G = maxG * (1 - n);
    B = 0;
  }
  return RGB(R, G, B);
}

// Draw a checkbox
//  Color: frame color
//  bColor: Background color
//  x/y: Upper-left point
//  mode : 0 : unchecked, 1 : checked
void DWINUI::Draw_Checkbox(uint16_t color, uint16_t bcolor, uint16_t x, uint16_t y, bool checked=false) {
  DWIN_Draw_Rectangle(0, color, x + 2, y + 2, x + 17, y + 17);
  DWIN_Draw_Rectangle(1, checked ? color : bcolor, x + 6, y + 6, x + 13, y + 13);
}

// Clear Menu by filling the menu area with background color
void DWINUI::ClearMainArea() {
  DWIN_Draw_Rectangle(1, backcolor, 0, TITLE_HEIGHT, DWIN_WIDTH - 1, STATUS_Y - 1);
}

/* Title Class ==============================================================*/

TitleClass Title;

void TitleClass::draw() {
  if (DWINUI::onTitleDraw != nullptr) (*DWINUI::onTitleDraw)(this);
}

void TitleClass::SetCaption(const char * const title) {
  frameid = 0;
  if ( caption == title ) return;
  const uint8_t len = _MIN(sizeof(caption) - 1, strlen(title));
  memcpy(&caption[0], title, len);
  caption[len] = '\0';
}

void TitleClass::ShowCaption(const char * const title) {
  SetCaption(title);
  draw();
}

void TitleClass::SetFrame(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  caption[0] = '\0';
  frameid = id;
  frame = { x1, y1, x2, y2 };
}

void TitleClass::SetFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  SetFrame(1, x, y, x + w - 1, y + h - 1);
}

void TitleClass::FrameCopy(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  SetFrame(id, x1, y1, x2, y2);
  draw();
}

void TitleClass::FrameCopy(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  FrameCopy(1, x, y, x + w - 1, y + h - 1);
}

#endif // DWIN_LCD_PROUI
