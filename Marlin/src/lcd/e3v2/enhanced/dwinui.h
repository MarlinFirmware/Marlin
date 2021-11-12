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
#pragma once

/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo
 * Version: 3.7.1
 * Date: 2021/11/09
 */

#include "dwin_lcd.h"
#include "../common/dwin_set.h"
#include "../common/dwin_font.h"
#include "../common/dwin_color.h"

// Extra Icons
#define ICON_Brightness           ICON_Motion
#define ICON_Cancel               ICON_StockConfiguration
#define ICON_CustomPreheat        ICON_SetEndTemp
#define ICON_Error                ICON_TempTooHigh
#define ICON_ExtrudeMinT          ICON_HotendTemp
#define ICON_FilLoad              ICON_WriteEEPROM
#define ICON_FilMan               ICON_ResumeEEPROM
#define ICON_FilSet               ICON_ResumeEEPROM
#define ICON_FilUnload            ICON_ReadEEPROM
#define ICON_Flow                 ICON_StepE
#define ICON_LevBed               ICON_SetEndTemp
#define ICON_Lock                 ICON_Cool
#define ICON_ManualMesh           ICON_HotendTemp
#define ICON_MeshNext             ICON_Axis
#define ICON_MeshSave             ICON_WriteEEPROM
#define ICON_MeshViewer           ICON_HotendTemp
#define ICON_MoveZ0               ICON_HotendTemp
#define ICON_Park                 ICON_Motion
#define ICON_PIDcycles            ICON_ResumeEEPROM
#define ICON_PIDValue             ICON_Contact
#define ICON_ProbeSet             ICON_SetEndTemp
#define ICON_ProbeTest            ICON_SetEndTemp
#define ICON_Pwrlossr             ICON_Motion
#define ICON_Reboot               ICON_ResumeEEPROM
#define ICON_Runout               ICON_MaxAccE
#define ICON_Scolor               ICON_MaxSpeed
#define ICON_SetCustomPreheat     ICON_SetEndTemp
#define ICON_Sound                ICON_Cool

// Default UI Colors
#define Def_Background_Color  Color_Bg_Black
#define Def_Cursor_color      Rectangle_Color
#define Def_TitleBg_color     Color_Bg_Blue
#define Def_TitleTxt_color    Color_White
#define Def_Text_Color        Color_White
#define Def_Selected_Color    Select_Color
#define Def_SplitLine_Color   Line_Color
#define Def_Highlight_Color   Color_White
#define Def_StatusBg_Color    RGB(0,20,20)
#define Def_StatusTxt_Color   Color_Yellow
#define Def_PopupBg_color     Color_Bg_Window
#define Def_PopupTxt_Color    Popup_Text_Color
#define Def_AlertBg_Color     Color_Bg_Red
#define Def_AlertTxt_Color    Color_Yellow
#define Def_PercentTxt_Color  Percent_Color
#define Def_Barfill_Color     BarFill_Color
#define Def_Indicator_Color   Color_White
#define Def_Coordinate_Color   Color_White

// UI element defines and constants
#define DWIN_FONT_MENU font8x16
#define DWIN_FONT_STAT font10x20
#define DWIN_FONT_HEAD font10x20
#define DWIN_FONT_ALERT font10x20
#define STATUS_Y 354
#define LCD_WIDTH (DWIN_WIDTH / 8)

constexpr uint16_t TITLE_HEIGHT = 30,                          // Title bar height
                   MLINE = 53,                                 // Menu line height
                   TROWS = (STATUS_Y - TITLE_HEIGHT) / MLINE,  // Total rows
                   MROWS = TROWS - 1,                          // Other-than-Back
                   ICOX = 26,                                  // Menu item icon X position
                   LBLX = 60,                                  // Menu item label X position
                   VALX = 210,                                 // Menu item value X position
                   MENU_CHR_W = 8, MENU_CHR_H = 16,            // Menu font 8x16
                   STAT_CHR_W = 10;

// Menuitem Y position
#define MYPOS(L) (TITLE_HEIGHT + MLINE * (L))

// Menuitem caption Offset
#define CAPOFF ((MLINE - MENU_CHR_H) / 2)

// Menuitem caption Y position
#define MBASE(L) (MYPOS(L) + CAPOFF)

// Create and add a MenuItem object to the menu array
#define ADDMENUITEM(V...) DWINUI::MenuItemsAdd(new MenuItemClass(V))
#define ADDMENUITEM_P(V...) DWINUI::MenuItemsAdd(new MenuItemPtrClass(V))

typedef struct { uint16_t left, top, right, bottom; } rect_t;
typedef struct { uint16_t x, y, w, h; } frame_rect_t;

class TitleClass {
public:
  char caption[32] = "";
  uint8_t frameid = 0;
  rect_t frame = {0};
  void draw();
  void SetCaption(const char * const title);
  inline void SetCaption(FSTR_P title) { SetCaption((char *)title); }
  void ShowCaption(const char * const title);
  inline void ShowCaption(FSTR_P title) { ShowCaption((char *)title); }
  void SetFrame(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void SetFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void FrameCopy(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void FrameCopy(uint16_t x, uint16_t y, uint16_t h, uint16_t v);
};
extern TitleClass Title;

class MenuItemClass {
protected:
public:
  int8_t pos = 0;
  uint8_t icon = 0;
  char caption[32] = "";
  uint8_t frameid = 0;
  rect_t frame = {0};
  void (*onDraw)(MenuItemClass* menuitem, int8_t line) = nullptr;
  void (*onClick)() = nullptr;
  MenuItemClass() {};
  MenuItemClass(uint8_t cicon, const char * const text=nullptr, void (*ondraw)(MenuItemClass* menuitem, int8_t line)=nullptr, void (*onclick)()=nullptr);
  MenuItemClass(uint8_t cicon, FSTR_P text = nullptr, void (*ondraw)(MenuItemClass* menuitem, int8_t line)=nullptr, void (*onclick)()=nullptr) : MenuItemClass(cicon, FTOP(text), ondraw, onclick){}
  MenuItemClass(uint8_t cicon, uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void (*ondraw)(MenuItemClass* menuitem, int8_t line)=nullptr, void (*onclick)()=nullptr);
  void SetFrame(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  virtual ~MenuItemClass(){};
  virtual void draw(int8_t line);
};

class MenuItemPtrClass: public MenuItemClass {
public:
  void *value = nullptr;
  using MenuItemClass::MenuItemClass;
  MenuItemPtrClass(uint8_t cicon, const char * const text, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)(), void* val);
  MenuItemPtrClass(uint8_t cicon, FSTR_P text, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)(), void* val) : MenuItemPtrClass(cicon, FTOP(text), ondraw, onclick, val){}
};

class MenuClass {
public:
  int8_t topline = 0;
  int8_t selected = 0;
  TitleClass MenuTitle;
  MenuClass();
  virtual ~MenuClass(){};
  inline int8_t line() { return selected - topline; };
  inline int8_t line(uint8_t pos) {return pos - topline; };
  void draw();
  void onScroll(bool dir);
  void onClick();
  MenuItemClass* SelectedItem();
};
extern MenuClass *CurrentMenu;

namespace DWINUI {
  extern xy_int_t cursor;
  extern uint16_t pencolor;
  extern uint16_t textcolor;
  extern uint16_t backcolor;
  extern uint8_t  font;

  extern void (*onCursorErase)(const int8_t line);
  extern void (*onCursorDraw)(const int8_t line);
  extern void (*onTitleDraw)(TitleClass* title);
  extern void (*onMenuDraw)(MenuClass* menu);

  // DWIN LCD Initialization
  void init();

  // Set text/number font
  void setFont(uint8_t cfont);

  // Get font character width
  uint8_t fontWidth(uint8_t cfont);

  // Get font character heigh
  uint8_t fontHeight(uint8_t cfont);

  // Get screen x coodinates from text column
  uint16_t ColToX(uint8_t col);

  // Get screen y coodinates from text row
  uint16_t RowToY(uint8_t row);

  // Set text/number color
  void SetColors(uint16_t fgcolor, uint16_t bgcolor);
  void SetTextColor(uint16_t fgcolor);
  void SetBackgroundColor(uint16_t bgcolor);

  // Moves cursor to point
  //  x: abscissa of the display
  //  y: ordinate of the display
  //  point: xy coordinate
  void MoveTo(int16_t x, int16_t y);
  void MoveTo(xy_int_t point);

  // Moves cursor relative to the actual position
  //  x: abscissa of the display
  //  y: ordinate of the display
  //  point: xy coordinate
  void MoveBy(int16_t x, int16_t y);
  void MoveBy(xy_int_t point);

  // Draw a line from the cursor to xy position
  //  color: Line segment color
  //  x/y: End point
  inline void LineTo(uint16_t color, uint16_t x, uint16_t y) {
    DWIN_Draw_Line(color, cursor.x, cursor.y, x, y);
  }
  inline void LineTo(uint16_t x, uint16_t y) {
    DWIN_Draw_Line(pencolor, cursor.x, cursor.y, x, y);
  }

  // Draw an Icon with transparent background from the library ICON
  //  icon: Icon ID
  //  x/y: Upper-left point
  inline void Draw_Icon(uint8_t icon, uint16_t x, uint16_t y) {
    DWIN_ICON_Show(ICON, icon, x, y);
  }

  // Draw a positive integer
  //  bShow: true=display background color; false=don't display background color
  //  zeroFill: true=zero fill; false=no zero fill
  //  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  iNum: Number of digits
  //  x/y: Upper-left coordinate
  //  value: Integer value
  inline void Draw_Int(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(bShow, zeroFill, zeroMode, size, color, bColor, iNum, x, y, value);
  }
  inline void Draw_Int(uint8_t iNum, long value) {
    DWIN_Draw_IntValue(false, true, 0, font, textcolor, backcolor, iNum, cursor.x, cursor.y, value);
    MoveBy(iNum * fontWidth(font), 0);
  }
  inline void Draw_Int(uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(false, true, 0, font, textcolor, backcolor, iNum, x, y, value);
  }
  inline void Draw_Int(uint16_t color, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(false, true, 0, font, color, backcolor, iNum, x, y, value);
  }
  inline void Draw_Int(uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(true, true, 0, font, color, bColor, iNum, x, y, value);
  }
  inline void Draw_Int(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_IntValue(true, true, 0, size, color, bColor, iNum, x, y, value);
  }

  // Draw a floating point number
  //  bShow: true=display background color; false=don't display background color
  //  zeroFill: true=zero fill; false=no zero fill
  //  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  iNum: Number of whole digits
  //  fNum: Number of decimal digits
  //  x/y: Upper-left point
  //  value: Float value
  inline void Draw_Float(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(bShow, zeroFill, zeroMode, size, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint8_t iNum, uint8_t fNum, float value) {
    DWIN_Draw_FloatValue(false, true, 0, font, textcolor, backcolor, iNum, fNum,  cursor.x, cursor.y, value);
    MoveBy((iNum + fNum + 1) * fontWidth(font), 0);
  }
  inline void Draw_Float(uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(false, true, 0, font, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint16_t color, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(false, true, 0, font, color, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(true, true, 0, font, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_FloatValue(true, true, 0, size, color, bColor, iNum, fNum, x, y, value);
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
  void Draw_Signed_Float(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value);
  inline void Draw_Signed_Float(uint8_t iNum, uint8_t fNum, float value) {
    Draw_Signed_Float(false, true, 0, font, textcolor, backcolor, iNum, fNum, cursor.x, cursor.y, value);
    MoveBy((iNum + fNum + 1) * fontWidth(font), 0);
  }
  inline void Draw_Signed_Float(uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Signed_Float(false, true, 0, font, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint8_t size, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Signed_Float(false, true, 0, size, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Signed_Float(true, true, 0, font, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Signed_Float(true, true, 0, size, color, bColor, iNum, fNum, x, y, value);
  }

  // Draw a char at cursor position
  void Draw_Char(const char c);

  // Draw a string at cursor position
  //  color: Character color
  //  *string: The string
  //  rlimit: For draw less chars than string length use rlimit
  void Draw_String(const char * const string, uint16_t rlimit = 0xFFFF);
  void Draw_String(uint16_t color, const char * const string, uint16_t rlimit = 0xFFFF);
  inline void Draw_String(FSTR_P  string, uint16_t rlimit = 0xFFFF) {
    Draw_String(FTOP(string), rlimit);
  }
  inline void Draw_String(uint16_t color, FSTR_P string, uint16_t rlimit = 0xFFFF) {
    Draw_String(color, FTOP(string), rlimit);
  }

  // Draw a string
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  x/y: Upper-left coordinate of the string
  //  *string: The string
  inline void Draw_String(uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(false, font, textcolor, backcolor, x, y, string);
  }
  inline void Draw_String(uint16_t x, uint16_t y, FSTR_P title) {
    DWIN_Draw_String(false, font, textcolor, backcolor, x, y, FTOP(title));
  }
  inline void Draw_String(uint16_t color, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(false, font, color, backcolor, x, y, string);
  }
  inline void Draw_String(uint16_t color, uint16_t x, uint16_t y, FSTR_P title) {
    DWIN_Draw_String(false, font, color, backcolor, x, y, title);
  }
  inline void Draw_String(uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(true, font, color, bgcolor, x, y, string);
  }
  inline void Draw_String(uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, FSTR_P title) {
    DWIN_Draw_String(true, font, color, bgcolor, x, y, title);
  }
  inline void Draw_String(uint8_t size, uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(true, size, color, bgcolor, x, y, string);
  }
  inline void Draw_String(uint8_t size, uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, FSTR_P title) {
    DWIN_Draw_String(true, size, color, bgcolor, x, y, title);
  }

  // Draw a centered string using DWIN_WIDTH
  //  bShow: true=display background color; false=don't display background color
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  y: Upper coordinate of the string
  //  *string: The string
  void Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, const char * const string);
  inline void Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, FSTR_P string) {
    Draw_CenteredString(bShow, size, color, bColor, y, FTOP(string));
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t bcolor, uint16_t y, const char * const string) {
    Draw_CenteredString(true, font, color, bcolor, y, string);
  }
  inline void Draw_CenteredString(uint8_t size, uint16_t color, uint16_t y, const char * const string) {
    Draw_CenteredString(false, size, color, backcolor, y, string);
  }
  inline void Draw_CenteredString(uint8_t size, uint16_t color, uint16_t y, FSTR_P title) {
    Draw_CenteredString(false, size, color, backcolor, y, title);
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t y, const char * const string) {
    Draw_CenteredString(false, font, color, backcolor, y, string);
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t y, FSTR_P title) {
    Draw_CenteredString(false, font, color, backcolor, y, title);
  }
  inline void Draw_CenteredString(uint16_t y, const char * const string) {
    Draw_CenteredString(false, font, textcolor, backcolor, y, string);
  }
  inline void Draw_CenteredString(uint16_t y, FSTR_P title) {
    Draw_CenteredString(false, font, textcolor, backcolor, y, title);
  }

  // Draw a circle
  //  Color: circle color
  //  x: abscissa of the center of the circle
  //  y: ordinate of the center of the circle
  //  r: circle radius
  void Draw_Circle(uint16_t color, uint16_t x,uint16_t y,uint8_t r);
  inline void Draw_Circle(uint16_t color, uint8_t r) {
    Draw_Circle(color, cursor.x, cursor.y, r);
  }

  // Draw a checkbox
  //  Color: frame color
  //  bColor: Background color
  //  x/y: Upper-left point
  //  checked : 0 : unchecked, 1 : checked
  void Draw_Checkbox(uint16_t color, uint16_t bcolor, uint16_t x, uint16_t y, bool checked);
  inline void Draw_Checkbox(uint16_t x, uint16_t y, bool checked=false) {
    Draw_Checkbox(textcolor, backcolor, x, y, checked);
  }

  // Color Interpolator
  //  val : Interpolator minv..maxv
  //  minv : Minimum value
  //  maxv : Maximum value
  //  color1 : Start color
  //  color2 : End color
  uint16_t ColorInt(int16_t val, int16_t minv, int16_t maxv, uint16_t color1, uint16_t color2);

  // -------------------------- Extra -------------------------------//

  // Draw a circle filled with color
  //  bcolor: fill color
  //  x: abscissa of the center of the circle
  //  y: ordinate of the center of the circle
  //  r: circle radius
  void Draw_FillCircle(uint16_t bcolor, uint16_t x,uint16_t y,uint8_t r);
  inline void Draw_FillCircle(uint16_t bcolor, uint8_t r) {
    Draw_FillCircle(bcolor, cursor.x, cursor.y, r);
  }

  // Color Interpolator through Red->Yellow->Green->Blue
  //  val : Interpolator minv..maxv
  //  minv : Minimum value
  //  maxv : Maximum value
  uint16_t RainbowInt(int16_t val, int16_t minv, int16_t maxv);

  // Write buffer data to the SRAM
  //  addr: SRAM start address 0x0000-0x7FFF
  //  length: Bytes to write
  //  data: address of the buffer with data
  inline void WriteToSRAM(uint16_t addr, uint16_t length, uint8_t *data) {
    DWIN_WriteToMem(0x5A, addr, length, data);
  }

  // Write buffer data to the Flash
  //  addr: Flash start address 0x0000-0x3FFF
  //  length: Bytes to write
  //  data: address of the buffer with data
  inline void WriteToFlash(uint16_t addr, uint16_t length, uint8_t *data) {
    DWIN_WriteToMem(0xA5, addr, length, data);
  }

  // Clear Menu by filling the area with background color
  // Area (0, TITLE_HEIGHT, DWIN_WIDTH, STATUS_Y - 1)
  void ClearMenuArea();

  // Clear MenuItems array and free MenuItems elements
  void MenuItemsClear();

  // Prepare MenuItems array
  void MenuItemsPrepare(int8_t totalitems);

  // Add elements to the MenuItems array
  MenuItemClass* MenuItemsAdd(MenuItemClass* menuitem);

};
