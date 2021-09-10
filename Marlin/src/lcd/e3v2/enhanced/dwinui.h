/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo
 * Version: 3.6.3
 * Date: 2021/08/09
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
#pragma once

#include "../../../core/types.h"
#include "dwin_lcd.h"

// ICON ID
#define ICON                7 // Icon set file 7.ICO

#define ICON_LOGO                  0
#define ICON_Print_0               1
#define ICON_Print_1               2
#define ICON_Prepare_0             3
#define ICON_Prepare_1             4
#define ICON_Control_0             5
#define ICON_Control_1             6
#define ICON_Leveling_0            7
#define ICON_Leveling_1            8
#define ICON_HotendTemp            9
#define ICON_BedTemp              10
#define ICON_Speed                11
#define ICON_Zoffset              12
#define ICON_Back                 13
#define ICON_File                 14
#define ICON_PrintTime            15
#define ICON_RemainTime           16
#define ICON_Setup_0              17
#define ICON_Setup_1              18
#define ICON_Pause_0              19
#define ICON_Pause_1              20
#define ICON_Continue_0           21
#define ICON_Continue_1           22
#define ICON_Stop_0               23
#define ICON_Stop_1               24
#define ICON_Bar                  25
#define ICON_More                 26

#define ICON_Axis                 27
#define ICON_CloseMotor           28
#define ICON_Homing               29
#define ICON_SetHome              30
#define ICON_PLAPreheat           31
#define ICON_ABSPreheat           32
#define ICON_Cool                 33
#define ICON_Language             34

#define ICON_MoveX                35
#define ICON_MoveY                36
#define ICON_MoveZ                37
#define ICON_Extruder             38

#define ICON_Temperature          40
#define ICON_Motion               41
#define ICON_WriteEEPROM          42
#define ICON_ReadEEPROM           43
#define ICON_ResumeEEPROM         44
#define ICON_Info                 45

#define ICON_SetEndTemp           46
#define ICON_SetBedTemp           47
#define ICON_FanSpeed             48
#define ICON_SetPLAPreheat        49
#define ICON_SetABSPreheat        50

#define ICON_MaxSpeed             51
#define ICON_MaxAccelerated       52
#define ICON_MaxJerk              53
#define ICON_Step                 54
#define ICON_PrintSize            55
#define ICON_Version              56
#define ICON_Contact              57
#define ICON_StockConfiguration   58
#define ICON_MaxSpeedX            59
#define ICON_MaxSpeedY            60
#define ICON_MaxSpeedZ            61
#define ICON_MaxSpeedE            62
#define ICON_MaxAccX              63
#define ICON_MaxAccY              64
#define ICON_MaxAccZ              65
#define ICON_MaxAccE              66
#define ICON_MaxSpeedJerkX        67
#define ICON_MaxSpeedJerkY        68
#define ICON_MaxSpeedJerkZ        69
#define ICON_MaxSpeedJerkE        70
#define ICON_StepX                71
#define ICON_StepY                72
#define ICON_StepZ                73
#define ICON_StepE                74
#define ICON_Setspeed             75
#define ICON_SetZOffset           76
#define ICON_Rectangle            77
#define ICON_BLTouch              78
#define ICON_TempTooLow           79
#define ICON_AutoLeveling         80
#define ICON_TempTooHigh          81
#define ICON_NoTips_C             82
#define ICON_NoTips_E             83
#define ICON_Continue_C           84
#define ICON_Continue_E           85
#define ICON_Cancel_C             86
#define ICON_Cancel_E             87
#define ICON_Confirm_C            88
#define ICON_Confirm_E            89
#define ICON_Info_0               90
#define ICON_Info_1               91

// Extra Icons
#define ICON_AdvSet               ICON_Language
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
#define ICON_HomeOffset           ICON_AdvSet
#define ICON_HomeOffsetX          ICON_StepX
#define ICON_HomeOffsetY          ICON_StepY
#define ICON_HomeOffsetZ          ICON_StepZ
#define ICON_LevBed               ICON_SetEndTemp
#define ICON_Lock                 ICON_Cool
#define ICON_ManualMesh           ICON_HotendTemp
#define ICON_MeshNext             ICON_Axis
#define ICON_MeshSave             ICON_WriteEEPROM
#define ICON_MoveZ0               ICON_HotendTemp
#define ICON_Park                 ICON_Motion
#define ICON_PIDbed               ICON_SetBedTemp
#define ICON_PIDcycles            ICON_ResumeEEPROM
#define ICON_PIDNozzle            ICON_SetEndTemp
#define ICON_PIDValue             ICON_Contact
#define ICON_ProbeOffsetX         ICON_StepX
#define ICON_ProbeOffsetY         ICON_StepY
#define ICON_ProbeOffsetZ         ICON_StepZ
#define ICON_ProbeSet             ICON_SetEndTemp
#define ICON_ProbeTest            ICON_SetEndTemp
#define ICON_Pwrlossr             ICON_Motion
#define ICON_Reboot               ICON_ResumeEEPROM
#define ICON_Runout               ICON_MaxAccE
#define ICON_Scolor               ICON_MaxSpeed
#define ICON_SetCustomPreheat     ICON_SetEndTemp
#define ICON_Sound                ICON_Cool

/**
 * 3-.0：The font size, 0x00-0x09, corresponds to the font size below:
 * 0x00=6*12   0x01=8*16   0x02=10*20  0x03=12*24  0x04=14*28
 * 0x05=16*32  0x06=20*40  0x07=24*48  0x08=28*56  0x09=32*64
 */
#define font6x12  0x00
#define font8x16  0x01
#define font10x20 0x02
#define font12x24 0x03
#define font14x28 0x04
#define font16x32 0x05
#define font20x40 0x06
#define font24x48 0x07
#define font28x56 0x08
#define font32x64 0x09

// Extended and default UI Colors
#define RGB(R,G,B)  (R << 11) | (G << 5) | (B) // R,B: 0..31; G: 0..63
#define GetRColor(color) ((color >> 11) & 0x1F)
#define GetGColor(color) ((color >>  5) & 0x3F)
#define GetBColor(color) ((color >>  0) & 0x1F)

#define Color_White       0xFFFF
#define Color_Bg_Window   0x31E8  // Popup background color
#define Color_Bg_Blue     0x1125  // Dark blue background color
#define Color_Bg_Black    0x0841  // Black background color
#define Color_Bg_Red      0xF00F  // Red background color
#define Popup_Text_Color  0xD6BA  // Popup font background color
#define Line_Color        0x3A6A  // Split line color
#define Rectangle_Color   0xEE2F  // Blue square cursor color
#define Percent_Color     0xFE29  // Percentage color
#define BarFill_Color     0x10E4  // Fill color of progress bar
#define Select_Color      0x33BB  // Selected color

#define Color_Black           0
#define Color_Red             RGB(31,0,0)
#define Color_Yellow          RGB(31,63,0)
#define Color_Green           RGB(0,63,0)
#define Color_Aqua            RGB(0,63,31)
#define Color_Blue            RGB(0,0,31)

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

//UI elements defines and constants
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
  void Draw();
  void SetCaption(const char * const title);
  inline void SetCaption(const __FlashStringHelper * title) { SetCaption((char *)title); }
  void ShowCaption(const char * const title);
  inline void ShowCaption(const __FlashStringHelper * title) { ShowCaption((char *)title); }
  void SetFrame(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void SetFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void FrameCopy(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  void FrameCopy(uint16_t x, uint16_t y, uint16_t h, uint16_t v);
};
extern TitleClass Title;

class MenuItemClass {
protected:
public:
  uint8_t pos = 0;
  uint8_t icon = 0;
  char caption[32] = "";
  uint8_t frameid = 0;
  rect_t frame = {0};
  void (*onDraw)(MenuItemClass* menuitem, int8_t line) = nullptr;
  void (*onClick)() = nullptr;
  MenuItemClass() {};
  MenuItemClass(uint8_t cicon, const char * const text=nullptr, void (*ondraw)(MenuItemClass* menuitem, int8_t line)=nullptr, void (*onclick)()=nullptr);
  MenuItemClass(uint8_t cicon, const __FlashStringHelper * text = nullptr, void (*ondraw)(MenuItemClass* menuitem, int8_t line)=nullptr, void (*onclick)()=nullptr) : MenuItemClass(cicon, (char*)text, ondraw, onclick){}
  MenuItemClass(uint8_t cicon, uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void (*ondraw)(MenuItemClass* menuitem, int8_t line)=nullptr, void (*onclick)()=nullptr);
  void SetFrame(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  virtual ~MenuItemClass(){};
  virtual void Draw(int8_t line);
};

class MenuItemPtrClass: public MenuItemClass {
public:
  void *value = nullptr;
  using MenuItemClass::MenuItemClass;
  MenuItemPtrClass(uint8_t cicon, const char * const text, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)(), void* val);
  MenuItemPtrClass(uint8_t cicon, const __FlashStringHelper * text, void (*ondraw)(MenuItemClass* menuitem, int8_t line), void (*onclick)(), void* val) : MenuItemPtrClass(cicon, (char*)text, ondraw, onclick, val){}
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
  void Draw();
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

  extern void (*onCursorErase)(uint8_t line);
  extern void (*onCursorDraw)(uint8_t line);
  extern void (*onTitleDraw)(TitleClass* title);
  extern void (*onMenuDraw)(MenuClass* menu);

  // DWIN LCD Initialization
  void Init(void);

  // Set text/number font
  void SetFont(uint8_t cfont);

  // Get font character width
  uint8_t Get_font_width(uint8_t cfont);

  // Get font character heigh
  uint8_t Get_font_height(uint8_t cfont);

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
    MoveBy(iNum * Get_font_width(font), 0);
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
    MoveBy((iNum + fNum + 1) * Get_font_width(font), 0);
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
    MoveBy((iNum + fNum + 1) * Get_font_width(font), 0);
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

  // Draw a string
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  x/y: Upper-left coordinate of the string
  //  *string: The string
  inline void Draw_String(uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(false, font, textcolor, backcolor, x, y, string);
  }
  inline void Draw_String(uint16_t x, uint16_t y, const __FlashStringHelper *title) {
    DWIN_Draw_String(false, font, textcolor, backcolor, x, y, (char *)title);
  }
  inline void Draw_String(uint16_t color, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(false, font, color, backcolor, x, y, string);
  }
  inline void Draw_String(uint16_t color, uint16_t x, uint16_t y, const __FlashStringHelper *title) {
    DWIN_Draw_String(false, font, color, backcolor, x, y, (char *)title);
  }
  inline void Draw_String(uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(true, font, color, bgcolor, x, y, string);
  }
  inline void Draw_String(uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, const __FlashStringHelper *title) {
    DWIN_Draw_String(true, font, color, bgcolor, x, y, (char *)title);
  }
  inline void Draw_String(uint8_t size, uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(true, size, color, bgcolor, x, y, string);
  }
  inline void Draw_String(uint8_t size, uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, const __FlashStringHelper *title) {
    DWIN_Draw_String(true, size, color, bgcolor, x, y, (char *)title);
  }

  // Draw a centered string using DWIN_WIDTH
  //  bShow: true=display background color; false=don't display background color
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  y: Upper coordinate of the string
  //  *string: The string
  void Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, const char * const string);
  inline void Draw_CenteredString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t y, const __FlashStringHelper *title) {
    Draw_CenteredString(bShow, size, color, bColor, y, (char *)title);
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t bcolor, uint16_t y, const char * const string) {
    Draw_CenteredString(true, font, color, bcolor, y, string);
  }
  inline void Draw_CenteredString(uint8_t size, uint16_t color, uint16_t y, const char * const string) {
    Draw_CenteredString(false, size, color, backcolor, y, string);
  }
  inline void Draw_CenteredString(uint8_t size, uint16_t color, uint16_t y, const __FlashStringHelper *title) {
    Draw_CenteredString(false, size, color, backcolor, y, (char *)title);
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t y, const char * const string) {
    Draw_CenteredString(false, font, color, backcolor, y, string);
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t y, const __FlashStringHelper *title) {
    Draw_CenteredString(false, font, color, backcolor, y, (char *)title);
  }
  inline void Draw_CenteredString(uint16_t y, const char * const string) {
    Draw_CenteredString(false, font, textcolor, backcolor, y, string);
  }
  inline void Draw_CenteredString(uint16_t y, const __FlashStringHelper *title) {
    Draw_CenteredString(false, font, textcolor, backcolor, y, (char *)title);
  }

  // Draw a circle
  //  Color: circle color
  //  x: the abscissa of the center of the circle
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
  //  maxv : Maximun value
  //  color1 : Start color
  //  color2 : End color
  uint16_t ColorInt(int16_t val, int16_t minv, int16_t maxv, uint16_t color1, uint16_t color2);

  // -------------------------- Extra -------------------------------//

  // Draw a circle filled with color
  //  bcolor: fill color
  //  x: the abscissa of the center of the circle
  //  y: ordinate of the center of the circle
  //  r: circle radius
  void Draw_FillCircle(uint16_t bcolor, uint16_t x,uint16_t y,uint8_t r);
  inline void Draw_FillCircle(uint16_t bcolor, uint8_t r) {
    Draw_FillCircle(bcolor, cursor.x, cursor.y, r);
  }

  // Color Interpolator through Red->Yellow->Green->Blue
  //  val : Interpolator minv..maxv
  //  minv : Minimum value
  //  maxv : Maximun value
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
  void MenuItemsPrepare(uint8_t totalitems);

  // Add elements to the MenuItems array
  MenuItemClass* MenuItemsAdd(MenuItemClass* menuitem);

};
