/**
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.19.1
 * Date: 2022/08/18
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

#include "../../../inc/MarlinConfigPre.h"

#include "../common/dwin_set.h"
#include "../common/dwin_font.h"
#include "../common/dwin_color.h"
#include "dwin_lcd.h"

// Extra Icons
// CI(0,I) uses custom library  9.ICO
// CI(1,I) uses custom library 10.ICO
#define CI(L,I) (I TERN_(HAS_CUSTOMICONS, + L*100))

#undef ICON_Folder
#undef ICON_AdvSet
#undef ICON_HomeOffset
#undef ICON_HomeOffsetX
#undef ICON_HomeOffsetY
#undef ICON_HomeOffsetZ
#undef ICON_ProbeOffset
#undef ICON_ProbeOffsetX
#undef ICON_ProbeOffsetY
#undef ICON_ProbeOffsetZ
#undef ICON_PIDNozzle
#undef ICON_PIDbed

#define ICON_AdvSet            CI(1,ICON_Language)
#define ICON_BedSizeX          CI(1,ICON_PrintSize)
#define ICON_BedSizeY          CI(1,ICON_PrintSize)
#define ICON_BedTramming       CI(1,ICON_SetHome)
#define ICON_Binary            CI(1,ICON_Contact)
#define ICON_BltouchReset      CI(1,ICON_StockConfiguration)
//#define ICON_Brightness        CI(1,ICON_Motion)
#define ICON_Cancel            CI(1,ICON_StockConfiguration)
#define ICON_CustomPreheat     CI(1,ICON_SetBedTemp)
#define ICON_Error             CI(1,ICON_TempTooHigh)
#define ICON_ESDiag            CI(1,ICON_Info)
#define ICON_ExtrudeMinT       CI(1,ICON_HotendTemp)
#define ICON_FilLoad           CI(1,ICON_WriteEEPROM)
#define ICON_FilMan            CI(1,ICON_ResumeEEPROM)
#define ICON_FilSet            CI(1,ICON_ResumeEEPROM)
#define ICON_FilUnload         CI(1,ICON_ReadEEPROM)
#define ICON_Flow              CI(1,ICON_StepE)
#define ICON_Folder            CI(1,ICON_More)
#define ICON_FWRetract         CI(1,ICON_StepE)
#define ICON_FWRetLength       CI(1,ICON_StepE)
#define ICON_FWRetSpeed        CI(1,ICON_Setspeed)
#define ICON_FWRetZRaise       CI(1,ICON_MoveZ)
#define ICON_FWRecSpeed        CI(1,ICON_Setspeed)
#define ICON_FWRecExtra        CI(1,ICON_StepE)
#define ICON_HomeX             CI(1,ICON_MoveX)
#define ICON_HomeY             CI(1,ICON_MoveY)
#define ICON_HomeZ             CI(1,ICON_MoveZ)
#define ICON_HomeOffset        CI(1,ICON_AdvSet)
#define ICON_HomeOffsetX       CI(1,ICON_StepX)
#define ICON_HomeOffsetY       CI(1,ICON_StepY)
#define ICON_HomeOffsetZ       CI(1,ICON_StepZ)
#define ICON_Host              CI(1,ICON_Contact)
#define ICON_HSMode            CI(1,ICON_StockConfiguration)
#define ICON_InvertE0          CI(1,ICON_StepE)
#define ICON_JDmm              CI(1,ICON_MaxJerk)
#define ICON_Tram              CI(1,ICON_SetEndTemp)
#define ICON_Level             CI(1,ICON_Mesh)
#define ICON_Lock              CI(1,ICON_Cool)
#define ICON_ManualMesh        CI(1,ICON_Mesh)
#define ICON_MaxPosX           CI(1,ICON_MoveX)
#define ICON_MaxPosY           CI(1,ICON_MoveY)
#define ICON_MaxPosZ           CI(1,ICON_MoveZ)
#define ICON_MeshNext          CI(1,ICON_Axis)
#define ICON_MeshPoints        CI(1,ICON_SetEndTemp)
#define ICON_MeshSave          CI(1,ICON_WriteEEPROM)
#define ICON_MeshViewer        CI(1,ICON_HotendTemp)
#define ICON_MoveZ0            CI(1,ICON_SetEndTemp)
#define ICON_Park              CI(1,ICON_Motion)
#define ICON_ParkPos           CI(1,ICON_AdvSet)
#define ICON_ParkPosX          CI(1,ICON_StepX)
#define ICON_ParkPosY          CI(1,ICON_StepY)
#define ICON_ParkPosZ          CI(1,ICON_StepZ)
#define ICON_PhySet            CI(1,ICON_PrintSize)
#define ICON_PIDNozzle         CI(1,ICON_SetEndTemp)
#define ICON_PIDBed            CI(1,ICON_SetBedTemp)
#define ICON_PIDCycles         CI(1,ICON_ResumeEEPROM)
#define ICON_PIDValue          CI(1,ICON_Contact)
#define ICON_PrintStats        CI(1,ICON_PrintTime)
#define ICON_Preheat1          CI(1,ICON_PLAPreheat)
#define ICON_Preheat2          CI(1,ICON_ABSPreheat)
#define ICON_Preheat3          CI(1,ICON_CustomPreheat)
#define ICON_Preheat4          CI(1,ICON_CustomPreheat)
#define ICON_Preheat5          CI(1,ICON_CustomPreheat)
#define ICON_Preheat6          CI(1,ICON_CustomPreheat)
#define ICON_PrintStatsReset   CI(1,ICON_RemainTime)
#define ICON_ProbeDeploy       CI(1,ICON_SetEndTemp)
#define ICON_ProbeMargin       CI(1,ICON_PrintSize)
#define ICON_ProbeMultiple     CI(1,ICON_SetHome)
#define ICON_ProbeOffsetX      CI(1,ICON_StepX)
#define ICON_ProbeOffsetY      CI(1,ICON_StepY)
#define ICON_ProbeOffsetZ      CI(1,ICON_StepZ)
#define ICON_ProbeSet          CI(1,ICON_SetEndTemp)
#define ICON_ProbeStow         CI(1,ICON_SetEndTemp)
#define ICON_ProbeTest         CI(1,ICON_SetEndTemp)
#define ICON_ProbeZSpeed       CI(1,ICON_MaxSpeedZ)
#define ICON_Pwrlossr          CI(1,ICON_Motion)
#define ICON_Reboot            CI(1,ICON_ResumeEEPROM)
#define ICON_Runout            CI(1,ICON_MaxAccE)
#define ICON_Scolor            CI(1,ICON_MaxSpeed)
#define ICON_SetBaudRate       CI(1,ICON_Setspeed)
#define ICON_SetCustomPreheat  CI(1,ICON_SetBedTemp)
#define ICON_SetPreheat1       CI(1,ICON_SetPLAPreheat)
#define ICON_SetPreheat2       CI(1,ICON_SetABSPreheat)
#define ICON_SetPreheat3       CI(1,ICON_SetCustomPreheat)
#define ICON_SetPreheat4       CI(1,ICON_SetCustomPreheat)
#define ICON_SetPreheat5       CI(1,ICON_SetCustomPreheat)
#define ICON_SetPreheat6       CI(1,ICON_SetCustomPreheat)
#define ICON_Sound             CI(1,ICON_Cool)
#define ICON_TBSetup           CI(1,ICON_Contact)
#define ICON_UBLActive         CI(1,ICON_Mesh)
#define ICON_ZAfterHome        CI(1,ICON_SetEndTemp)

//LED Lights
#define ICON_CaseLight         CI(1,ICON_Motion)
#define ICON_LedControl        CI(1,ICON_Motion)

//MPC
#define ICON_MPCNozzle         CI(1,ICON_SetEndTemp)
#define ICON_MPCValue          CI(1,ICON_Contact)
#define ICON_MPCHeater         CI(1,ICON_Temperature)
#define ICON_MPCHeatCap        CI(1,ICON_SetBedTemp)
#define ICON_MPCFan            CI(1,ICON_FanSpeed)

// Buttons
#define BTN_Continue          85
#define BTN_Cancel            87
#define BTN_Confirm           89
#define BTN_Print             90
#define BTN_Save              91
#define BTN_Purge             92

// Extended and default UI Colors
#define Color_Black           0x0000 //changed 0
#define Color_Green           0x07E0 //RGB(0,63,0)
#define Color_Aqua            RGB(0,63,31)
#define Color_Blue            0x015F //RGB(0,0,31)
#define Color_Light_White     0xBDD7
#define Color_Light_Green     0x3460
#define Color_Cyan            0x07FF
#define Color_Light_Cyan      0x04F3
#define Color_Light_Blue      0x3A6A
#define Color_Magenta         0xF81F
#define Color_Light_Magenta   0x9813
#define Color_Light_Red       0x8800
#define Color_Orange          0xFB00 //0xFA20
#define Color_Light_Orange    0xFC80 //0xFBC0
#define Color_Light_Yellow    0x8BE0
#define Color_Brown           0xCC27
#define Color_Light_Brown     0x6204
#define Color_Grey            0x18E3

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

constexpr uint8_t  TITLE_HEIGHT = 30,                          // Title bar height
                   MLINE = 53,                                 // Menu line height
                   TROWS = (STATUS_Y - TITLE_HEIGHT) / MLINE,  // Total rows
                   MROWS = TROWS - 1,                          // Other-than-Back
                   ICOX = 26,                                  // Menu item icon X position
                   LBLX = 55,                                  // Menu item label X position
                   VALX = 210,                                 // Menu item value X position
                   MENU_CHR_W = 8, MENU_CHR_H = 16,            // Menu font 8x16
                   STAT_CHR_W = 10;

// Menuitem Y position
#define MYPOS(L) (TITLE_HEIGHT + MLINE * (L))

// Menuitem caption Offset
#define CAPOFF ((MLINE - MENU_CHR_H) / 2)

// Menuitem caption Y position
#define MBASE(L) (MYPOS(L) + CAPOFF)

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

namespace DWINUI {
  extern xy_int_t cursor;
  extern uint16_t pencolor;
  extern uint16_t textcolor;
  extern uint16_t backcolor;
  extern uint16_t buttoncolor;
  extern font_t   font;
  extern FSTR_P const Author;

  extern void (*onTitleDraw)(TitleClass* title);

  // DWIN LCD Initialization
  void init();

  // Set text/number font
  void setFont(font_t cfont);

  // Get font character width
  font_t fontWidth(font_t cfont);
  inline font_t fontWidth() { return fontWidth(font); };

  // Get font character height
  font_t fontHeight(font_t cfont);
  inline font_t fontHeight() { return fontHeight(font); };

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

  // Draw an Icon and select library automatically
  //  BG: The icon background display: false=Background filtering is not displayed, true=Background display
  //  libID: Icon library ID
  //  picID: Icon ID
  //  x/y: Upper-left point
  void ICON_Show(bool BG, uint8_t icon, uint16_t x, uint16_t y);

  // Draw an Icon with transparent background from the library ICON
  //  icon: Icon ID
  //  x/y: Upper-left point
  inline void Draw_Icon(uint8_t icon, uint16_t x, uint16_t y) {
    ICON_Show(false, icon, x, y);
  }

  // Draw an Icon from the library ICON with its background
  //  icon: Icon ID
  //  x/y: Upper-left point
  inline void Draw_IconWB(uint8_t icon, uint16_t x, uint16_t y) {
    ICON_Show(true, icon, x, y);
  }

  // Draw a numeric integer value
  //  bShow: true=display background color; false=don't display background color
  //  signedMode: 1=signed; 0=unsigned
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  iNum: Number of digits
  //  x/y: Upper-left coordinate
  //  value: Integer value
  void Draw_Int(uint8_t bShow, bool signedMode, font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, int32_t value);

  // Draw a positive integer
  inline void Draw_Int(uint8_t bShow, font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(bShow, 0, size, color, bColor, iNum, x, y, value);
  }
  inline void Draw_Int(uint8_t iNum, long value) {
    Draw_Int(false, 0, font, textcolor, backcolor, iNum, cursor.x, cursor.y, value);
    MoveBy(iNum * fontWidth(font), 0);
  }
  inline void Draw_Int(uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(false, 0, font, textcolor, backcolor, iNum, x, y, value);
  }
  inline void Draw_Int(uint16_t color, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(false, 0, font, color, backcolor, iNum, x, y, value);
  }
  inline void Draw_Int(uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(true, 0, font, color, bColor, iNum, x, y, value);
  }
  inline void Draw_Int(font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(true, 0, size, color, bColor, iNum, x, y, value);
  }

  // Draw a signed integer
  inline void Draw_Signed_Int(uint8_t bShow, font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(bShow, 1, size, color, bColor, iNum, x, y, value);
  }
  inline void Draw_Signed_Int(uint8_t iNum, long value) {
    Draw_Int(false, 1, font, textcolor, backcolor, iNum, cursor.x, cursor.y, value);
    MoveBy(iNum * fontWidth(font), 0);
  }
  inline void Draw_Signed_Int(uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(false, 1, font, textcolor, backcolor, iNum, x, y, value);
  }
  inline void Draw_Signed_Int(uint16_t color, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(false, 1, font, color, backcolor, iNum, x, y, value);
  }
  inline void Draw_Signed_Int(uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(true, 1, font, color, bColor, iNum, x, y, value);
  }
  inline void Draw_Signed_Int(font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint16_t x, uint16_t y, long value) {
    Draw_Int(true, 1, size, color, bColor, iNum, x, y, value);
  }

  // Draw a numeric float value
  //  bShow: true=display background color; false=don't display background color
  //  signedMode: 1=signed; 0=unsigned
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  iNum: Number of digits
  //  fNum: Number of decimal digits
  //  x/y: Upper-left coordinate
  //  value: float value
  void Draw_Float(uint8_t bShow, bool signedMode, font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value);

  // Draw a positive floating point number
  inline void Draw_Float(uint8_t bShow, font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(bShow, 0, size, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint8_t iNum, uint8_t fNum, float value) {
    Draw_Float(false, 0, font, textcolor, backcolor, iNum, fNum, cursor.x, cursor.y, value);
    MoveBy((iNum + fNum + 1) * fontWidth(font), 0);
  }
  inline void Draw_Float(uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(false, 0, font, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(font_t size, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(false, 0, size, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(true, 0, font, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Float(font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(true, 0, size, color, bColor, iNum, fNum, x, y, value);
  }

  // Draw a signed floating point number
  inline void Draw_Signed_Float(uint8_t bShow, font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(bShow, 1, size, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint8_t iNum, uint8_t fNum, float value) {
    Draw_Float(false, 1, font, textcolor, backcolor, iNum, fNum, cursor.x, cursor.y, value);
    MoveBy((iNum + fNum + 1) * fontWidth(font), 0);
  }
  inline void Draw_Signed_Float(uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(false, 1, font, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(font_t size, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(false, 1, size, textcolor, backcolor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(true, 1, font, color, bColor, iNum, fNum, x, y, value);
  }
  inline void Draw_Signed_Float(font_t size, uint16_t color, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, float value) {
    Draw_Float(true, 1, size, color, bColor, iNum, fNum, x, y, value);
  }

  // Draw a char
  //  color: Character color
  //  x: abscissa of the display
  //  y: ordinate of the display
  //  c: ASCII code of char
  void Draw_Char(uint16_t color, uint16_t x, uint16_t y, const char c);
  inline void Draw_Char(uint16_t x, uint16_t y, const char c) { Draw_Char(textcolor, x, y, c); };
  // Draw a char at cursor position and increment cursor
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
  inline void Draw_String(font_t size, uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, const char * const string) {
    DWIN_Draw_String(true, size, color, bgcolor, x, y, string);
  }
  inline void Draw_String(font_t size, uint16_t color, uint16_t bgcolor, uint16_t x, uint16_t y, FSTR_P title) {
    DWIN_Draw_String(true, size, color, bgcolor, x, y, title);
  }

  // Draw a centered string using DWIN_WIDTH
  //  bShow: true=display background color; false=don't display background color
  //  size: Font size
  //  color: Character color
  //  bColor: Background color
  //  y: Upper coordinate of the string
  //  *string: The string
  void Draw_CenteredString(bool bShow, font_t size, uint16_t color, uint16_t bColor, uint16_t x1, uint16_t x2, uint16_t y, const char * const string);
  inline void Draw_CenteredString(bool bShow, font_t size, uint16_t color, uint16_t bColor, uint16_t y, const char * const string) {
    Draw_CenteredString(bShow, size, color, bColor, 0, DWIN_WIDTH, y, string);
  }
  inline void Draw_CenteredString(bool bShow, font_t size, uint16_t color, uint16_t bColor, uint16_t y, FSTR_P string) {
    Draw_CenteredString(bShow, size, color, bColor, y, FTOP(string));
  }
  inline void Draw_CenteredString(uint16_t color, uint16_t bcolor, uint16_t y, const char * const string) {
    Draw_CenteredString(true, font, color, bcolor, y, string);
  }
  inline void Draw_CenteredString(font_t size, uint16_t color, uint16_t y, const char * const string) {
    Draw_CenteredString(false, size, color, backcolor, y, string);
  }
  inline void Draw_CenteredString(font_t size, uint16_t color, uint16_t y, FSTR_P title) {
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

  // Clear by filling the area with background color
  // Area (0, TITLE_HEIGHT, DWIN_WIDTH, STATUS_Y - 1)
  void ClearMainArea();

};
