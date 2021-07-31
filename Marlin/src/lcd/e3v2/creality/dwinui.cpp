/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo
 * Version: 2.3
 * Date: 2021/07/13
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
 */

#include "../../../inc/MarlinConfigPre.h"

#if ANY(DWIN_CREALITY_LCD, IS_DWIN_MARLINUI)

#include "../../../inc/MarlinConfig.h"
#include "dwin_lcd.h"
#include "dwinui.h"

//#define DEBUG_OUT 1
#include "../../../core/debug_out.h"

xy_int_t DWINUI::cursor = { 0 };
uint16_t DWINUI::pencolor = Color_White;
uint16_t DWINUI::textcolor = Def_Text_Color;
uint16_t DWINUI::backcolor = Def_Background_Color;
uint8_t  DWINUI::font = font8x16;

void DWINUI::Init(void) {
  DEBUG_ECHOPGM("\r\nDWIN handshake ");
  delay(750);   // Delay here or init later in the boot process
  const bool success = DWIN_Handshake();
  if (success) DEBUG_ECHOLNPGM("ok."); else DEBUG_ECHOLNPGM("error.");
  DWIN_Frame_SetDir(DISABLED(DWIN_MARLINUI_LANDSCAPE) ? 1 : 0);
  TERN(SHOW_BOOTSCREEN,,DWIN_Frame_Clear(Color_Bg_Black));
  DWIN_UpdateLCD();
  cursor.x = 0;
  cursor.y = 0;
  pencolor = Color_White;
  textcolor = Def_Text_Color;
  backcolor = Def_Background_Color;
  font = font8x16;
}

// Set text/number font
void DWINUI::SetFont(uint8_t cfont) {
  font = cfont;
}

// Get font character width
uint8_t DWINUI::Get_font_width(uint8_t cfont) {
  switch (cfont) {
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

// Get font character heigh
uint8_t DWINUI::Get_font_height(uint8_t cfont) {
  switch (cfont) {
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

// Get screen x coodinates from text column
uint16_t DWINUI::ColToX(uint8_t col) {
  return col * Get_font_width(font);
}

// Get screen y coodinates from text row
uint16_t DWINUI::RowToY(uint8_t row) {
  return row * Get_font_height(font);
}

// Set text/number color
void DWINUI::SetColors(uint16_t fgcolor, uint16_t bgcolor) {
  textcolor = fgcolor;
  backcolor = bgcolor;
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

// Draw a Centered string using DWIN_WIDTH
void DWINUI::Draw_CenteredString(bool widthAdjust, bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, const char * const string) {
  const int8_t x = _MAX(0U, DWIN_WIDTH - strlen_P(string) * Get_font_width(size)) / 2 - 1;
  DWIN_Draw_String(widthAdjust, bShow, size, color, bColor, x, y, string);
}

// Draw a signed floating point number
//  bShow: true=display background color; false=don't display background color
//  zeroFill: true=zero fill; false=no zero fill
//  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
//  size: Font size
//  bColor: Background color
//  iNum: Number of whole digits
//  fNum: Number of decimal digits
//  x/y: Upper-left point
//  value: Float value
void DWINUI::Draw_Signed_Float(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value) {
  if (value < 0) {
    DWIN_Draw_FloatValue(bShow, zeroFill, zeroMode, size, color, bColor, iNum, fNum, x, y, -value);
    DWIN_Draw_String(false, bShow, size, color, bColor, x - 6, y, F("-"));
  }
  else {
    DWIN_Draw_String(false, bShow, size, color, bColor, x - 6, y, F(" "));
    DWIN_Draw_FloatValue(bShow, zeroFill, zeroMode, size, color, bColor, iNum, fNum, x, y, value);
  }
}

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

// Color Interpolator
//  val : Interpolator minv..maxv
//  minv : Minimum value
//  maxv : Maximun value
//  color1 : Start color
//  color2 : End color
uint16_t DWINUI::ColorInt(int16_t val, int16_t minv, int16_t maxv, uint16_t color1, uint16_t color2) {
  uint8_t B,G,R;
  float n;
  n = (float)(val-minv)/(maxv-minv);
  R = (1-n)*GetRColor(color1) + n*GetRColor(color2);
  G = (1-n)*GetGColor(color1) + n*GetGColor(color2);
  B = (1-n)*GetBColor(color1) + n*GetBColor(color2);
  return RGB(R,G,B);
}

// Draw a checkbox
//  Color: frame color
//  bColor: Background color
//  x/y: Upper-left point
//  mode : 0 : unchecked, 1 : checked
void DWINUI::Draw_Checkbox(uint16_t color, uint16_t bcolor, uint16_t x, uint16_t y, bool mode=false) {
  DWIN_Draw_String(false, true, font8x16, color, bcolor, x + 4, y, F(mode ? "x" : " "));
  DWIN_Draw_Rectangle(0, color, x + 2, y + 2, x + 17, y + 17);
}

TitleClass Title;

void TitleClass::Init(rect_t cframe, uint8_t cfont, int16_t color, int16_t bcolor, const char * const title) {
  font = cfont;
  backcolor = bcolor;
  textcolor = color;
  frame = cframe;
  SetCaption(title);
}

void TitleClass::Clear() {
  DWIN_Draw_Rectangle(1, backcolor, frame.left, frame.top, frame.right, frame.bottom);
}

void TitleClass::Draw() {
  Clear();
  DWIN_Draw_String(false, false, DWIN_FONT_HEAD, textcolor, backcolor, 14, (TitleHeight() - DWINUI::Get_font_height(font)) / 2 - 1, caption);
}

void TitleClass::SetFont(uint8_t cfont) {
  if ( font == cfont ) return;
  font = cfont;
  Draw();
}

void TitleClass::SetCaption(const char * const title) {
  if ( caption == title ) return;
  const uint8_t len = _MIN(sizeof(caption) - 1, strlen(title));
  memcpy(&caption[0], title, len);
  caption[len] = '\0';
  Draw();
}

void TitleClass::SetFrame(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  frame = {x1, y1, x2, y2};
  Draw();
}

void TitleClass::SetColors(int16_t color, int16_t bcolor) {
  if ( textcolor == color && backcolor == color ) return;
  textcolor = color;
  backcolor = bcolor;
  Draw();
}

void TitleClass::SetTextColor(int16_t color) {
  if ( textcolor == color ) return;
  textcolor = color;
  Draw();
}

void TitleClass::SetBackgroundColor(int16_t color) {
  if ( backcolor == color ) return;
  backcolor = color;
  Draw();
}

void TitleClass::FrameCopy(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  Clear();
  caption[0] = '\0';
  DWIN_Frame_AreaCopy(id, x1, y1, x2, y2, 14, (TitleHeight() - (y2 - y1)) / 2 - 1);
}

uint16_t TitleClass::TitleHeight() {
  return frame.bottom - frame.top;
};

#endif
