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
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.15.1
 * Date: 2022/02/25
 */

#include "dwin_lcd.h"
#include "../common/dwin_set.h"
#include "../common/dwin_font.h"
#include "../common/dwin_color.h"

// Extra Icons
#define ICON_AdvSet               ICON_Language
#define ICON_BedSizeX             ICON_PrintSize
#define ICON_BedSizeY             ICON_PrintSize
#define ICON_Binary               ICON_Contact
#define ICON_Brightness           ICON_Motion
#define ICON_Cancel               ICON_StockConfiguration
#define ICON_CustomPreheat        ICON_SetEndTemp
#define ICON_Error                ICON_TempTooHigh
#define ICON_ESDiag               ICON_Info
#define ICON_ExtrudeMinT          ICON_HotendTemp
#define ICON_FilLoad              ICON_WriteEEPROM
#define ICON_FilMan               ICON_ResumeEEPROM
#define ICON_FilSet               ICON_ResumeEEPROM
#define ICON_FilUnload            ICON_ReadEEPROM
#define ICON_Flow                 ICON_StepE
#define ICON_Folder               ICON_More
#define ICON_FWRetLength          ICON_StepE
#define ICON_FWRetSpeed           ICON_Setspeed
#define ICON_FWRetZRaise          ICON_MoveZ
#define ICON_FWRecSpeed           ICON_Setspeed
#define ICON_HomeX                ICON_MoveX
#define ICON_HomeY                ICON_MoveY
#define ICON_HomeZ                ICON_MoveZ
#define ICON_HomeOffset           ICON_AdvSet
#define ICON_HomeOffsetX          ICON_StepX
#define ICON_HomeOffsetY          ICON_StepY
#define ICON_HomeOffsetZ          ICON_StepZ
#define ICON_HSMode               ICON_StockConfiguration
#define ICON_InvertE0             ICON_StepE
#define ICON_Tram                 ICON_SetEndTemp
#define ICON_Level                ICON_HotendTemp
#define ICON_Lock                 ICON_Cool
#define ICON_ManualMesh           ICON_HotendTemp
#define ICON_MaxPosX              ICON_MoveX
#define ICON_MaxPosY              ICON_MoveY
#define ICON_MaxPosZ              ICON_MoveZ
#define ICON_MeshNext             ICON_Axis
#define ICON_MeshPoints           ICON_SetEndTemp
#define ICON_MeshSave             ICON_WriteEEPROM
#define ICON_MeshViewer           ICON_HotendTemp
#define ICON_MoveZ0               ICON_HotendTemp
#define ICON_Park                 ICON_Motion
#define ICON_ParkPos              ICON_AdvSet
#define ICON_ParkPosX             ICON_StepX
#define ICON_ParkPosY             ICON_StepY
#define ICON_ParkPosZ             ICON_StepZ
#define ICON_PhySet               ICON_PrintSize
#define ICON_PIDbed               ICON_SetBedTemp
#define ICON_PIDcycles            ICON_ResumeEEPROM
#define ICON_PIDValue             ICON_Contact
#define ICON_PrintStats           ICON_PrintTime
#define ICON_PrintStatsReset      ICON_RemainTime
#define ICON_ProbeDeploy          ICON_SetEndTemp
#define ICON_ProbeMargin          ICON_PrintSize
#define ICON_ProbeOffsetX         ICON_StepX
#define ICON_ProbeOffsetY         ICON_StepY
#define ICON_ProbeOffsetZ         ICON_StepZ
#define ICON_ProbeSet             ICON_SetEndTemp
#define ICON_ProbeStow            ICON_SetEndTemp
#define ICON_ProbeTest            ICON_SetEndTemp
#define ICON_ProbeZSpeed          ICON_MaxSpeedZ
#define ICON_Pwrlossr             ICON_Motion
#define ICON_Reboot               ICON_ResumeEEPROM
#define ICON_Runout               ICON_MaxAccE
#define ICON_Scolor               ICON_MaxSpeed
#define ICON_SetBaudRate          ICON_Setspeed
#define ICON_SetCustomPreheat     ICON_SetEndTemp
#define ICON_Sound                ICON_Cool
#define ICON_CaseLight            ICON_Motion
#define ICON_LedControl           ICON_Motion

// Buttons
#define BTN_Continue          85
#define BTN_Cancel            87
#define BTN_Confirm           89
#define BTN_Print             90
#define BTN_Save              91

// Extended and default UI Colors
#define Color_Black           0
#define Color_Green           RGB(0,63,0)
#define Color_Aqua            RGB(0,63,31)
#define Color_Blue            RGB(0,0,31)

// UI element defines and constants
#define DWIN_FONT_MENU font8x16
#define DWIN_FONT_STAT font10x20
#define DWIN_FONT_HEAD font10x20
#define DWIN_FONT_ALERT font10x20
#define STATUS_Y 354
#define LCD_WIDTH (DWIN_WIDTH / 8)  // only if the default font is font8x16

// Minimum unit (0.1) : multiple (10)
#define UNITFDIGITS 1
#define MINUNITMULT POW(10, UNITFDIGITS)

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
#define MENU_ITEM(V...) DWINUI::MenuItemsAdd(new MenuItemClass(V))
#define EDIT_ITEM(V...) DWINUI::MenuItemsAdd(new MenuItemPtrClass(V))

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
  extern uint16_t buttoncolor;
  extern uint8_t  font;
  extern FSTR_P const Author;

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
  inline uint8_t fontWidth() { return fontWidth(font); };

  // Get font character height
  uint8_t fontHeight(uint8_t cfont);
  inline uint8_t fontHeight() { return fontHeight(font); };

  // Get screen x coordinates from text column
  uint16_t ColToX(uint8_t col);

  // Get screen y coordinates from text row
  uint16_t RowToY(uint8_t row);

  // Set text/number color
  void SetColors(uint16_t fgcolor, uint16_t bgcolor, uint16_t alcolor);
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

  // Extend a frame box
  //  v: value to extend
  inline frame_rect_t ExtendFrame(frame_rect_t frame, uint8_t v) {
    frame_rect_t t;
    t.x = frame.x - v;
    t.y = frame.y - v;
    t.w = frame.w + 2 * v;
    t.h = frame.h + 2 * v;
    return t;
  }

  // Draw an Icon with transparent background from the library ICON
  //  icon: Icon ID
  //  x/y: Upper-left point
  inline void Draw_Icon(uint8_t icon, uint16_t x, uint16_t y) {
    DWIN_ICON_Show(ICON, icon, x, y);
  }

  // Draw an Icon from the library ICON with its background
  //  icon: Icon ID
  //  x/y: Upper-left point
  inline void Draw_IconWB(uint8_t icon, uint16_t x, uint16_t y) {
    DWIN_ICON_Show(true, false, false, ICON, icon, x, y);
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
    DWIN_Draw_Value(bShow, 0, zeroFill, zeroMode, size, color, bColor, iNum, 0, x, y, value);
  }
  inline void Draw_Int(uint8_t iNum, long value) {
    DWIN_Draw_Value(false, 0, true, 0, font, textcolor, backcolor, iNum, 0, cursor.x, cursor.y, value);
    MoveBy(iNum * fontWidth(font), 0);
  }
  inline void Draw_Int(uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_Value(false, 0, true, 0, font, textcolor, backcolor, iNum, 0, x, y, value);
  }
  inline void Draw_Int(uint16_t color, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_Value(false, 0, true, 0, font, color, backcolor, iNum, 0, x, y, value);
  }
  inline void Draw_Int(uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_Value(true, 0, true, 0, font, color, bColor, iNum, 0, x, y, value);
  }
  inline void Draw_Int(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_Value(true, 0, true, 0, size, color, bColor, iNum, 0, x, y, value);
  }

  // Draw a signed integer
  //  bShow: true=display background color; false=don't display background color
  //  zeroFill: true=zero fill; false=no zero fill
  //  zeroMode: 1=leading 0 displayed as 0; 0=leading 0 displayed as a space
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  iNum: Number of digits
  //  x/y: Upper-left coordinate
  //  value: Integer value
  inline void Draw_Signed_Int(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_Value(bShow, 1, zeroFill, zeroMode, size, color, bColor, iNum, 0, x, y, value);
  }
  inline void Draw_Signed_Int(uint8_t iNum, long value) {
    DWIN_Draw_Value(false, 1, true, 0, font, textcolor, backcolor, iNum, 0, cursor.x, cursor.y, value);
    MoveBy(iNum * fontWidth(font), 0);
  }
  inline void Draw_Signed_Int(uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_Value(false, 1, true, 0, font, textcolor, backcolor, iNum, 0, x, y, value);
  }
  inline void Draw_Signed_Int(uint16_t color, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_Value(false, 1, true, 0, font, color, backcolor, iNum, 0, x, y, value);
  }
  inline void Draw_Signed_Int(uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_Value(true, 1, true, 0, font, color, bColor, iNum, 0, x, y, value);
  }
  inline void Draw_Signed_Int(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    DWIN_Draw_Value(true, 1, true, 0, size, color, bColor, iNum, 0, x, y, value);
  }

  // Draw a positive floating point number
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
    DWIN_Draw_Value(bShow, 0, zeroFill, zeroMode, size, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint8_t iNum, uint8_t fNum, float value) {
    DWIN_Draw_Value(false, 0, true, 0, font, textcolor, backcolor, iNum, fNum, cursor.x, cursor.y, value);
    MoveBy((iNum + fNum + 1) * fontWidth(font), 0);
  }
  inline void Draw_Float(uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_Value(false, 0, true, 0, font, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint16_t color, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_Value(false, 0, true, 0, font, color, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_Value(true, 0, true, 0, font, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_Value(true, 0, true, 0, size, color, bColor, iNum, fNum, x, y, value);
  }

  // Draw a signed floating point number
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
  inline void Draw_Signed_Float(uint8_t bShow, bool zeroFill, uint8_t zeroMode, uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_Value(bShow, 1, zeroFill, zeroMode, size, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint8_t iNum, uint8_t fNum, float value) {
    DWIN_Draw_Value(false, 1, true, 0, font, textcolor, backcolor, iNum, fNum, cursor.x, cursor.y, value);
    MoveBy((iNum + fNum + 1) * fontWidth(font), 0);
  }
  inline void Draw_Signed_Float(uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_Value(false, 1, true, 0, font, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint8_t size, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_Value(false, 1, true, 0, size, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_Value(true, 1, true, 0, font, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint8_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    DWIN_Draw_Value(true, 1, true, 0, size, color, bColor, iNum, fNum, x, y, value);
  }

  // Draw a char at cursor position
  void Draw_Char(uint16_t color, const char c);
  inline void Draw_Char(const char c) { Draw_Char(textcolor, c); }

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
  void Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x1, uint16_t x2, uint16_t y, const char * const string);
  inline void Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, const char * const string) {
    Draw_CenteredString(bShow, size, color, bColor, 0, DWIN_WIDTH, y, string);
  }
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

  // Draw a box
  //  mode: 0=frame, 1=fill, 2=XOR fill
  //  color: Rectangle color
  //  frame: Box coordinates and size
  inline void Draw_Box(uint8_t mode, uint16_t color, frame_rect_t frame) {
    DWIN_Draw_Box(mode, color, frame.x, frame.y, frame.w, frame.h);
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

  // ------------------------- Buttons ------------------------------//

  void Draw_Button(uint16_t color, uint16_t bcolor, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const char * const caption);
  inline void Draw_Button(uint16_t color, uint16_t bcolor, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, FSTR_P caption) {
    Draw_Button(color, bcolor, x1, y1, x2, y2, FTOP(caption));
  }
  inline void Draw_Button(FSTR_P caption, uint16_t x, uint16_t y) {
    Draw_Button(textcolor, buttoncolor, x, y, x + 99, y + 37, caption);
  }
  void Draw_Button(uint8_t id, uint16_t x, uint16_t y);

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
  void ClearMainArea();

  // Clear MenuItems array and free MenuItems elements
  void MenuItemsClear();

  // Prepare MenuItems array
  void MenuItemsPrepare(int8_t totalitems);

  // Add elements to the MenuItems array
  MenuItemClass* MenuItemsAdd(MenuItemClass* menuitem);

};
