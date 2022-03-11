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
 * Version: 3.15.1
 * Date: 2022/02/25
 *
 * Based on the original code provided by Creality under GPL
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_LCD_PROUI)

#include "../../../inc/MarlinConfig.h"
#include "dwin_lcd.h"
#include "dwinui.h"
#include "dwin_defines.h"

//#define DEBUG_OUT 1
#include "../../../core/debug_out.h"

int8_t MenuItemTotal = 0;
int8_t MenuItemCount = 0;
MenuItemClass** MenuItems = nullptr;
MenuClass *CurrentMenu = nullptr;
MenuClass *PreviousMenu = nullptr;

xy_int_t DWINUI::cursor = { 0 };
uint16_t DWINUI::pencolor = Color_White;
uint16_t DWINUI::textcolor = Def_Text_Color;
uint16_t DWINUI::backcolor = Def_Background_Color;
uint16_t DWINUI::buttoncolor = Def_Button_Color;
uint8_t  DWINUI::font = font8x16;
FSTR_P const DWINUI::Author = F(STRING_CONFIG_H_AUTHOR);

void (*DWINUI::onCursorErase)(const int8_t line)=nullptr;
void (*DWINUI::onCursorDraw)(const int8_t line)=nullptr;
void (*DWINUI::onTitleDraw)(TitleClass* title)=nullptr;
void (*DWINUI::onMenuDraw)(MenuClass* menu)=nullptr;

void DWINUI::init() {
  TERN_(DEBUG_DWIN, DEBUG_ECHOPGM("\r\nDWIN handshake ");)
  delay(750);   // Delay for wait to wakeup screen
  TERN(DEBUG_DWIN, if (DWIN_Handshake()) DEBUG_ECHOLNPGM("ok."); else DEBUG_ECHOLNPGM("error."), DWIN_Handshake());
  DWIN_Frame_SetDir(1);
  cursor.x = 0;
  cursor.y = 0;
  pencolor = Color_White;
  textcolor = Def_Text_Color;
  backcolor = Def_Background_Color;
  buttoncolor = Def_Button_Color;
  font = font8x16;
}

// Set text/number font
void DWINUI::setFont(uint8_t cfont) {
  font = cfont;
}

// Get font character width
uint8_t DWINUI::fontWidth(uint8_t cfont) {
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

// Get font character height
uint8_t DWINUI::fontHeight(uint8_t cfont) {
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

// Get screen x coordinates from text column
uint16_t DWINUI::ColToX(uint8_t col) {
  return col * fontWidth(font);
}

// Get screen y coordinates from text row
uint16_t DWINUI::RowToY(uint8_t row) {
  return row * fontHeight(font);
}

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
void DWINUI::Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x1, uint16_t x2, uint16_t y, const char * const string) {
  const uint16_t x = _MAX(0U, x2 + x1 - strlen_P(string) * fontWidth(size)) / 2 - 1;
  DWIN_Draw_String(bShow, size, color, bColor, x, y, string);
}

// // Draw a Centered string using DWIN_WIDTH
// void DWINUI::Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, const char * const string) {
//   const int8_t x = _MAX(0U, DWIN_WIDTH - strlen_P(string) * fontWidth(size)) / 2 - 1;
//   DWIN_Draw_String(bShow, size, color, bColor, x, y, string);
// }

// Draw a char at cursor position
void DWINUI::Draw_Char(uint16_t color, const char c) {
  const char string[2] = { c, 0};
  DWIN_Draw_String(false, font, color, backcolor, cursor.x, cursor.y, string, 1);
  MoveBy(fontWidth(font), 0);
}

// Draw a string at cursor position
//  color: Character color
//  *string: The string
//  rlimit: For draw less chars than string length use rlimit
void DWINUI::Draw_String(const char * const string, uint16_t rlimit) {
  DWIN_Draw_String(false, font, textcolor, backcolor, cursor.x, cursor.y, string, rlimit);
  MoveBy(strlen(string) * fontWidth(font), 0);
}
void DWINUI::Draw_String(uint16_t color, const char * const string, uint16_t rlimit) {
  DWIN_Draw_String(false, font, color, backcolor, cursor.x, cursor.y, string, rlimit);
  MoveBy(strlen(string) * fontWidth(font), 0);
}

// ------------------------- Buttons ------------------------------//

void DWINUI::Draw_Button(uint16_t color, uint16_t bcolor, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const char * const caption) {
  DWIN_Draw_Rectangle(1, bcolor, x1, y1, x2, y2);
  Draw_CenteredString(0, font, color, bcolor, x1, x2, (y2 + y1 - fontHeight())/2, caption);
}

void DWINUI::Draw_Button(uint8_t id, uint16_t x, uint16_t y) {
  switch (id) {
    case BTN_Cancel  : Draw_Button(GET_TEXT_F(MSG_BUTTON_CANCEL), x, y); break;
    case BTN_Confirm : Draw_Button(GET_TEXT_F(MSG_BUTTON_CONFIRM), x, y); break;
    case BTN_Continue: Draw_Button(GET_TEXT_F(MSG_BUTTON_CONTINUE), x, y); break;
    case BTN_Print   : Draw_Button(GET_TEXT_F(MSG_BUTTON_PRINT), x, y); break;
    case BTN_Save    : Draw_Button(GET_TEXT_F(MSG_BUTTON_SAVE), x, y); break;
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
  const float n = (float)(val - minv) / (maxv - minv);
  R = (1-n) * GetRColor(color1) + n * GetRColor(color2);
  G = (1-n) * GetGColor(color1) + n * GetGColor(color2);
  B = (1-n) * GetBColor(color1) + n * GetBColor(color2);
  return RGB(R, G, B);
}

// Color Interpolator through Red->Yellow->Green->Blue
//  val : Interpolator minv..maxv
//  minv : Minimum value
//  maxv : Maximum value
uint16_t DWINUI::RainbowInt(int16_t val, int16_t minv, int16_t maxv) {
  uint8_t B, G, R;
  const uint8_t maxB = 28, maxR = 28, maxG = 38;
  const int16_t limv = _MAX(abs(minv), abs(maxv));
  float n = minv >= 0 ? (float)(val - minv) / (maxv - minv) : (float)val / limv;
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
  DWIN_Draw_String(true, font8x16, color, bcolor, x + 4, y, checked ? F("x") : F(" "));
  DWIN_Draw_Rectangle(0, color, x + 2, y + 2, x + 17, y + 17);
}

// Clear Menu by filling the menu area with background color
void DWINUI::ClearMenuArea() {
  DWIN_Draw_Rectangle(1, backcolor, 0, TITLE_HEIGHT, DWIN_WIDTH - 1, STATUS_Y - 1);
}

void DWINUI::MenuItemsClear() {
  if (MenuItems == nullptr) return;
  for (int8_t i = 0; i < MenuItemCount; i++) delete MenuItems[i];
  delete[] MenuItems;
  MenuItems = nullptr;
  MenuItemCount = 0;
  MenuItemTotal = 0;
}

void DWINUI::MenuItemsPrepare(int8_t totalitems) {
  MenuItemsClear();
  MenuItemTotal = totalitems;
  MenuItems = new MenuItemClass*[totalitems];
}

MenuItemClass* DWINUI::MenuItemsAdd(MenuItemClass* menuitem) {
  if (MenuItemCount < MenuItemTotal) {
    MenuItems[MenuItemCount] = menuitem;
    menuitem->pos = MenuItemCount++;
    return menuitem;
  }
  else {
    delete menuitem;
    return nullptr;
  }
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

/* Menu Class ===============================================================*/

MenuClass::MenuClass() {
  selected = 0;
  topline = 0;
}

void MenuClass::draw() {
  MenuTitle.draw();
  if (DWINUI::onMenuDraw != nullptr) (*DWINUI::onMenuDraw)(this);
  for (int8_t i = 0; i < MenuItemCount; i++)
    MenuItems[i]->draw(i - topline);
  if (DWINUI::onCursorDraw != nullptr) DWINUI::onCursorDraw(line());
  DWIN_UpdateLCD();
}

void MenuClass::onScroll(bool dir) {
  int8_t sel = selected;
  if (dir) sel++; else sel--;
  LIMIT(sel, 0, MenuItemCount - 1);
  if (sel != selected) {
    if (DWINUI::onCursorErase != nullptr) DWINUI::onCursorErase(line());
    if ((sel - topline) == TROWS) {
      DWIN_Frame_AreaMove(1, DWIN_SCROLL_UP, MLINE, DWINUI::backcolor, 0, TITLE_HEIGHT + 1, DWIN_WIDTH, STATUS_Y - 1);
      topline++;
      MenuItems[sel]->draw(TROWS - 1);
    }
    if ((sel < topline)) {
      DWIN_Frame_AreaMove(1, DWIN_SCROLL_DOWN, MLINE, DWINUI::backcolor, 0, TITLE_HEIGHT + 1, DWIN_WIDTH, STATUS_Y - 1);
      topline--;
      MenuItems[sel]->draw(0);
    }
    selected = sel;
    if (DWINUI::onCursorDraw != nullptr) DWINUI::onCursorDraw(line());
    DWIN_UpdateLCD();
  }
}

void MenuClass::onClick() {
  if (MenuItems[selected]->onClick != nullptr) (*MenuItems[selected]->onClick)();
}

MenuItemClass *MenuClass::SelectedItem() {
  return MenuItems[selected];
}

/* MenuItem Class ===========================================================*/

MenuItemClass::MenuItemClass(uint8_t cicon, const char * const text, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)()) {
  icon = cicon;
  onClick = onclick;
  onDraw = ondraw;
  const uint8_t len = _MIN(sizeof(caption) - 1, strlen(text));
  memcpy(&caption[0], text, len);
  caption[len] = '\0';
}

MenuItemClass::MenuItemClass(uint8_t cicon, uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)()) {
  icon = cicon;
  onClick = onclick;
  onDraw = ondraw;
  caption[0] = '\0';
  frameid = id;
  frame = { x1, y1, x2, y2 };
}

void MenuItemClass::SetFrame(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  caption[0] = '\0';
  frameid = id;
  frame = { x1, y1, x2, y2 };
}

void MenuItemClass::draw(int8_t line) {
  if (line < 0 || line >= TROWS) return;
  if (onDraw != nullptr) (*onDraw)(this, line);
};

MenuItemPtrClass::MenuItemPtrClass(uint8_t cicon, const char * const text, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)(), void* val) : MenuItemClass(cicon, text, ondraw, onclick) {
  value = val;
};

#endif // DWIN_LCD_PROUI
