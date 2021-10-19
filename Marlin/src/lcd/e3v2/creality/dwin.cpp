/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * DWIN by Creality3D
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD)

#include "dwin.h"

//#define USE_STRING_HEADINGS
//#define USE_STRING_TITLES

#if ENABLED(LCD_BED_LEVELING) && DISABLED(PROBE_MANUALLY) && ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT)
  #define HAS_ONESTEP_LEVELING 1
#endif

#if ANY(BABYSTEPPING, HAS_BED_PROBE, HAS_WORKSPACE_OFFSET)
  #define HAS_ZOFFSET_ITEM 1
#endif

#if !HAS_BED_PROBE && ENABLED(BABYSTEPPING)
  #define JUST_BABYSTEP 1
#endif

#include "../../fontutils.h"
#include "../../marlinui.h"

#include "../../../sd/cardreader.h"

#include "../../../MarlinCore.h"
#include "../../../core/serial.h"
#include "../../../core/macros.h"
#include "../../../gcode/queue.h"

#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../module/settings.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../../feature/host_actions.h"
#endif

#if HAS_ONESTEP_LEVELING
  #include "../../../feature/bedlevel/bedlevel.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

#if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
  #include "../../../feature/babystep.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#include <WString.h>
#include <stdio.h>
#include <string.h>

#ifndef MACHINE_SIZE
  #define MACHINE_SIZE STRINGIFY(X_BED_SIZE) "x" STRINGIFY(Y_BED_SIZE) "x" STRINGIFY(Z_MAX_POS)
#endif
#ifndef CORP_WEBSITE
  #define CORP_WEBSITE WEBSITE_URL
#endif

#define PAUSE_HEAT

#define MENU_CHAR_LIMIT  24
#define STATUS_Y        354

// Print speed limit
#define MIN_PRINT_SPEED  10
#define MAX_PRINT_SPEED 999

// Feedspeed limit (max feedspeed = DEFAULT_MAX_FEEDRATE * 2)
#define MIN_MAXFEEDSPEED      1
#define MIN_MAXACCELERATION   1
#define MIN_MAXJERK           0.1
#define MIN_STEP              1

#define FEEDRATE_E      (60)

// Minimum unit (0.1) : multiple (10)
#define UNITFDIGITS 1
#define MINUNITMULT pow(10, UNITFDIGITS)

#define ENCODER_WAIT_MS                  20
#define DWIN_VAR_UPDATE_INTERVAL         1024
#define DWIN_SCROLL_UPDATE_INTERVAL      SEC_TO_MS(2)
#define DWIN_REMAIN_TIME_UPDATE_INTERVAL SEC_TO_MS(20)

#define TROWS 6                         // Total rows
constexpr uint16_t MROWS = TROWS - 1,   // Last Row Index
                   TITLE_HEIGHT = 30,   // Title bar height
                   MLINE = 53,          // Menu line height
                   LBLX = 60,           // Menu item label X
                   MENU_CHR_W = 8, STAT_CHR_W = 10;

#define MBASE(L) (49 + MLINE * (L))
#define EBASE(L) (MBASE(L) - 2 * DISABLED(USE_STRING_TITLES))

#define BABY_Z_VAR TERN(HAS_BED_PROBE, probe.offset.z, dwin_zoffset)

#define DWIN_BOTTOM (DWIN_HEIGHT-1)
#define DWIN_RIGHT (DWIN_WIDTH-1)

// Value Init
HMI_value_t HMI_ValueStruct;
HMI_flag_t HMI_flag{0};

millis_t dwin_heat_time = 0;

uint8_t checkkey = 0;

enum SelectItem : uint8_t {
  PAGE_PRINT = 0,
  PAGE_PREPARE,
  PAGE_CONTROL,
  PAGE_INFO_LEVELING,

  PRINT_SETUP = 0,
  PRINT_PAUSE_RESUME,
  PRINT_STOP
};

typedef struct {
  uint8_t now, last;
  void set(uint8_t v) { now = last = v; }
  void reset() { set(0); }
  bool changed() { bool c = (now != last); if (c) last = now; return c; }
  bool dec() { if (now) now--; return changed(); }
  bool inc(uint8_t v) { if (now < (v - 1)) now++; else now = (v - 1); return changed(); }
} select_t;

select_t select_page{0}, select_file{0}, select_print{0}, select_prepare{0}
         , select_control{0}, select_axis{0}, select_temp{0}, select_motion{0}, select_tune{0}
         , select_advset{0}, select_PLA{0}, select_ABS{0}
         , select_speed{0}
         , select_acc{0}
         , select_jerk{0}
         , select_step{0}
         , select_item{0}
         ;

uint8_t index_file     = MROWS,
        index_prepare  = MROWS,
        index_control  = MROWS,
        index_leveling = MROWS,
        index_tune     = MROWS,
        index_advset   = MROWS;

bool dwin_abort_flag = false; // Flag to reset feedrate, return to Home

constexpr float default_max_feedrate[]        = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[]    = DEFAULT_MAX_ACCELERATION;

#if HAS_CLASSIC_JERK
  constexpr float default_max_jerk[]          = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
#endif

static uint8_t _card_percent = 0;
static uint16_t _remain_time = 0;

#if ENABLED(PAUSE_HEAT)
  #if HAS_HOTEND
    uint16_t resume_hotend_temp = 0;
  #endif
  #if HAS_HEATED_BED
    uint16_t resume_bed_temp = 0;
  #endif
#endif

#if HAS_ZOFFSET_ITEM
  float dwin_zoffset = 0, last_zoffset = 0;
#endif

#define DWIN_LANGUAGE_EEPROM_ADDRESS 0x01   // Between 0x01 and 0x63 (EEPROM_OFFSET-1)
                                            // BL24CXX::check() uses 0x00

inline bool HMI_IsChinese() { return HMI_flag.language == DWIN_CHINESE; }

void HMI_SetLanguageCache() {
  DWIN_JPG_CacheTo1(HMI_IsChinese() ? Language_Chinese : Language_English);
}

void HMI_SetLanguage() {
  #if BOTH(EEPROM_SETTINGS, IIC_BL24CXX_EEPROM)
    BL24CXX::read(DWIN_LANGUAGE_EEPROM_ADDRESS, (uint8_t*)&HMI_flag.language, sizeof(HMI_flag.language));
  #endif
  HMI_SetLanguageCache();
}

void HMI_ToggleLanguage() {
  HMI_flag.language = HMI_IsChinese() ? DWIN_ENGLISH : DWIN_CHINESE;
  HMI_SetLanguageCache();
  #if BOTH(EEPROM_SETTINGS, IIC_BL24CXX_EEPROM)
    BL24CXX::write(DWIN_LANGUAGE_EEPROM_ADDRESS, (uint8_t*)&HMI_flag.language, sizeof(HMI_flag.language));
  #endif
}

typedef struct { uint16_t x, y, w, h; } icon_info_t;
typedef struct { uint16_t x, y[2], w, h; } text_info_t;

void ICON_Button(const bool here, const int iconid, const icon_info_t &ico, const text_info_t (&txt)[2]) {
  const bool cn = HMI_IsChinese();
  DWIN_ICON_Show(ICON, iconid + here, ico.x, ico.y);
  if (here) DWIN_Draw_Rectangle(0, Color_White, ico.x, ico.y, ico.x + ico.w - 1, ico.y + ico.h - 1);
  DWIN_Frame_AreaCopy(1, txt[cn].x, txt[cn].y[here], txt[cn].x + txt[cn].w - 1, txt[cn].y[here] + txt[cn].h - 1, ico.x + (ico.w - txt[cn].w) / 2, (ico.y + ico.h - 28) - txt[cn].h/2);
}

//
// Main Menu: "Print"
//
void ICON_Print() {
  constexpr icon_info_t ico = { 17, 110, 110, 100 };
  constexpr text_info_t txt[2] = {
    { 1, { 417, 449 }, 30, 14 },
    { 1, { 405, 447 }, 27, 15 }
  };
  ICON_Button(select_page.now == PAGE_PRINT, ICON_Print_0, ico, txt);
}

//
// Main Menu: "Prepare"
//
void ICON_Prepare() {
  constexpr icon_info_t ico = { 145, 110, 110, 100 };
  constexpr text_info_t txt[2] = {
    { 33, { 417, 449 }, 51, 14 },
    { 31, { 405, 447 }, 27, 15 }
  };
  ICON_Button(select_page.now == PAGE_PREPARE, ICON_Prepare_0, ico, txt);
}

//
// Main Menu: "Control"
//
void ICON_Control() {
  constexpr icon_info_t ico = { 17, 226, 110, 100 };
  constexpr text_info_t txt[2] = {
    { 85, { 417, 449 }, 46, 14 },
    { 61, { 405, 447 }, 27, 15 }
  };
  ICON_Button(select_page.now == PAGE_CONTROL, ICON_Control_0, ico, txt);
}

//
// Main Menu: "Info"
//
void ICON_StartInfo() {
  constexpr icon_info_t ico = { 145, 226, 110, 100 };
  constexpr text_info_t txt[2] = {
    { 133, { 417, 449 }, 23, 14 },
    {  91, { 405, 447 }, 27, 15 }
  };
  ICON_Button(select_page.now == PAGE_INFO_LEVELING, ICON_Info_0, ico, txt);
}

//
// Main Menu: "Level"
//
void ICON_Leveling() {
  constexpr icon_info_t ico = { 145, 226, 110, 100 };
  constexpr text_info_t txt[2] = {
    {  88, { 433, 464 }, 36, 14 },
    { 211, { 405, 447 }, 27, 15 }
  };
  ICON_Button(select_page.now == PAGE_INFO_LEVELING, ICON_Leveling_0, ico, txt);
}

//
// Printing: "Tune"
//
void ICON_Tune() {
  constexpr icon_info_t ico = { 8, 232, 80, 100 };
  constexpr text_info_t txt[2] = {
    {   0, { 433, 464 }, 32, 14 },
    { 121, { 405, 447 }, 27, 15 }
  };
  ICON_Button(select_print.now == PRINT_SETUP, ICON_Setup_0, ico, txt);
}

//
// Printing: "Pause"
//
void ICON_Pause() {
  constexpr icon_info_t ico = { 96, 232, 80, 100 };
  constexpr text_info_t txt[2] = {
    { 157, { 417, 449 }, 39, 14 },
    { 181, { 405, 447 }, 27, 15 }
  };
  ICON_Button(select_print.now == PRINT_PAUSE_RESUME, ICON_Pause_0, ico, txt);
}

//
// Printing: "Resume"
//
void ICON_Resume() {
  constexpr icon_info_t ico = { 96, 232, 80, 100 };
  constexpr text_info_t txt[2] = {
    { 33, { 433, 464 }, 53, 14 },
    {  1, { 405, 447 }, 27, 15 }
  };
  ICON_Button(select_print.now == PRINT_PAUSE_RESUME, ICON_Continue_0, ico, txt);
}

void ICON_ResumeOrPause() {
  if (printingIsPaused() || HMI_flag.pause_flag || HMI_flag.pause_action)
    ICON_Resume();
  else
    ICON_Pause();
}

//
// Printing: "Stop"
//
void ICON_Stop() {
  constexpr icon_info_t ico = { 184, 232, 80, 100 };
  constexpr text_info_t txt[2] = {
    { 196, { 417, 449 }, 29, 14 },
    { 151, { 405, 447 }, 27, 12 }
  };
  ICON_Button(select_print.now == PRINT_STOP, ICON_Stop_0, ico, txt);
}

inline void Clear_Title_Bar() {
  DWIN_Draw_Box(1, Color_Bg_Blue, 0, 0, DWIN_WIDTH, TITLE_HEIGHT);
}

void Draw_Title(FSTR_P ftitle) {
  DWIN_Draw_String(false, DWIN_FONT_HEAD, Color_White, Color_Bg_Blue, 14, 4, ftitle);
}

inline void Clear_Menu_Area() {
  DWIN_Draw_Box(1, Color_Bg_Black, 0, TITLE_HEIGHT, DWIN_WIDTH, STATUS_Y - TITLE_HEIGHT);
}

void Clear_Main_Window() {
  Clear_Title_Bar();
  Clear_Menu_Area();
}

void Clear_Popup_Area() {
  Clear_Title_Bar();
  DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 31, DWIN_WIDTH, DWIN_HEIGHT);
}

void Draw_Popup_Bkgd_105() {
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 105, 258, 374);
}

void Draw_More_Icon(const uint8_t line) {
  DWIN_ICON_Show(ICON, ICON_More, 226, MBASE(line) - 3);
}

void Draw_Menu_Cursor(const uint8_t line) {
  //DWIN_ICON_Show(ICON, ICON_Rectangle, 0, MBASE(line) - 18);
  DWIN_Draw_Rectangle(1, Rectangle_Color, 0, MBASE(line) - 18, 14, MBASE(line + 1) - 20);
}

void Erase_Menu_Cursor(const uint8_t line) {
  DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, MBASE(line) - 18, 14, MBASE(line + 1) - 20);
}

void Move_Highlight(const int16_t from, const uint16_t newline) {
  Erase_Menu_Cursor(newline - from);
  Draw_Menu_Cursor(newline);
}

void Add_Menu_Line() {
  Move_Highlight(1, MROWS);
  DWIN_Draw_Line(Line_Color, 16, MBASE(MROWS + 1) - 20, 256, MBASE(MROWS + 1) - 19);
}

void Scroll_Menu(const uint8_t dir) {
  DWIN_Frame_AreaMove(1, dir, MLINE, Color_Bg_Black, 0, 31, DWIN_WIDTH, 349);
  switch (dir) {
    case DWIN_SCROLL_DOWN: Move_Highlight(-1, 0); break;
    case DWIN_SCROLL_UP:   Add_Menu_Line(); break;
  }
}

inline uint16_t nr_sd_menu_items() {
  return card.get_num_Files() + !card.flag.workDirIsRoot;
}

void Erase_Menu_Text(const uint8_t line) {
  DWIN_Draw_Rectangle(1, Color_Bg_Black, LBLX, MBASE(line) - 14, 271, MBASE(line) + 28);
}

void Draw_Menu_Icon(const uint8_t line, const uint8_t icon) {
  DWIN_ICON_Show(ICON, icon, 26, MBASE(line) - 3);
}

void _Decorate_Menu_Item(const uint8_t line, const uint8_t icon, bool more) {
  if (icon) Draw_Menu_Icon(line, icon);
  if (more) Draw_More_Icon(line);
}
void Draw_Menu_Item(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr, bool more=false) {
  if (label) DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Black, LBLX, MBASE(line) - 1, (char*)label);
  _Decorate_Menu_Item(line, icon, more);
}
void Draw_Menu_Item(const uint8_t line, const uint8_t icon=0, FSTR_P const flabel=nullptr, bool more=false) {
  if (flabel) DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Black, LBLX, MBASE(line) - 1, flabel);
  _Decorate_Menu_Item(line, icon, more);
}

void Draw_Menu_Line(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr, bool more=false) {
  Draw_Menu_Item(line, icon, label, more);
  DWIN_Draw_Line(Line_Color, 16, MBASE(line) + 33, 256, MBASE(line) + 34);
}

void Draw_Menu_Line(const uint8_t line, const uint8_t icon, FSTR_P const flabel, bool more=false) {
  Draw_Menu_Item(line, icon, flabel, more);
  DWIN_Draw_Line(Line_Color, 16, MBASE(line) + 33, 256, MBASE(line) + 34);
}

void Draw_Checkbox_Line(const uint8_t line, const bool ison) {
  const uint16_t x = 225, y = EBASE(line) - 2;
  DWIN_Draw_String(true, font8x16, Color_White, Color_Bg_Black, x + 5, y, ison ? F("X") : F(" "));
  DWIN_Draw_Rectangle(0, Color_White, x + 2, y + 2, x + 16, y + 16);
}

// AreaCopy for a Menu Item
void Item_AreaCopy(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2, const uint8_t row=0, const uint16_t inset=0, const uint16_t yadd=0) {
  DWIN_Frame_AreaCopy(1, x1, y1, x2, y2, LBLX + inset, MBASE(row) + yadd);
}

// AreaCopy for a Screen Title
void DWIN_Frame_TitleCopy(const uint16_t x1, const uint16_t y1, const uint16_t w, const uint16_t h) {
  DWIN_Frame_AreaCopy(1, x1, y1, x1 + w - 1, y1 + h - 1, (DWIN_WIDTH - w) / 2, (TITLE_HEIGHT - h) / 2);
}

// Draw "Back" line at the top
void Draw_Back_First(const bool is_sel=true) {
  Draw_Menu_Line(0, ICON_Back);
  if (HMI_IsChinese())
    Item_AreaCopy(129,  72, 156,  84);
  else
    Item_AreaCopy(223, 179, 254, 189);
  if (is_sel) Draw_Menu_Cursor(0);
}

template <typename T>
inline bool Apply_Encoder(const EncoderState &encoder_diffState, T &valref) {
  if (encoder_diffState == ENCODER_DIFF_CW)
    valref += EncoderRate.encoderMoveValue;
  else if (encoder_diffState == ENCODER_DIFF_CCW)
    valref -= EncoderRate.encoderMoveValue;
  return encoder_diffState == ENCODER_DIFF_ENTER;
}

//
// Draw Menus
//
#define CASE_BACK          0

#define MOTION_CASE_RATE   1
#define MOTION_CASE_ACCEL  2
#define MOTION_CASE_JERK   (MOTION_CASE_ACCEL + ENABLED(HAS_CLASSIC_JERK))
#define MOTION_CASE_STEPS  (MOTION_CASE_JERK + 1)
#define MOTION_CASE_TOTAL  MOTION_CASE_STEPS

#define PREPARE_CASE_MOVE  1
#define PREPARE_CASE_DISA  2
#define PREPARE_CASE_HOME  3
#define PREPARE_CASE_ZOFF (PREPARE_CASE_HOME + ENABLED(HAS_ZOFFSET_ITEM))
#define PREPARE_CASE_PLA  (PREPARE_CASE_ZOFF + ENABLED(HAS_HOTEND))
#define PREPARE_CASE_ABS  (PREPARE_CASE_PLA + ENABLED(HAS_HOTEND))
#define PREPARE_CASE_COOL (PREPARE_CASE_ABS + EITHER(HAS_HOTEND, HAS_HEATED_BED))
#define PREPARE_CASE_LANG (PREPARE_CASE_COOL + 1)
#define PREPARE_CASE_TOTAL PREPARE_CASE_LANG

#define CONTROL_CASE_TEMP 1
#define CONTROL_CASE_MOVE  (CONTROL_CASE_TEMP + 1)
#define CONTROL_CASE_SAVE  (CONTROL_CASE_MOVE + ENABLED(EEPROM_SETTINGS))
#define CONTROL_CASE_LOAD  (CONTROL_CASE_SAVE + ENABLED(EEPROM_SETTINGS))
#define CONTROL_CASE_RESET (CONTROL_CASE_LOAD + ENABLED(EEPROM_SETTINGS))
#define CONTROL_CASE_ADVSET (CONTROL_CASE_RESET + 1)
#define CONTROL_CASE_INFO  (CONTROL_CASE_ADVSET + 1)
#define CONTROL_CASE_TOTAL CONTROL_CASE_INFO

#define TUNE_CASE_SPEED 1
#define TUNE_CASE_TEMP (TUNE_CASE_SPEED + ENABLED(HAS_HOTEND))
#define TUNE_CASE_BED  (TUNE_CASE_TEMP + ENABLED(HAS_HEATED_BED))
#define TUNE_CASE_FAN  (TUNE_CASE_BED + ENABLED(HAS_FAN))
#define TUNE_CASE_ZOFF (TUNE_CASE_FAN + ENABLED(HAS_ZOFFSET_ITEM))
#define TUNE_CASE_TOTAL TUNE_CASE_ZOFF

#define TEMP_CASE_TEMP (0 + ENABLED(HAS_HOTEND))
#define TEMP_CASE_BED  (TEMP_CASE_TEMP + ENABLED(HAS_HEATED_BED))
#define TEMP_CASE_FAN  (TEMP_CASE_BED + ENABLED(HAS_FAN))
#define TEMP_CASE_PLA  (TEMP_CASE_FAN + ENABLED(HAS_HOTEND))
#define TEMP_CASE_ABS  (TEMP_CASE_PLA + ENABLED(HAS_HOTEND))
#define TEMP_CASE_TOTAL TEMP_CASE_ABS

#define PREHEAT_CASE_TEMP (0 + ENABLED(HAS_HOTEND))
#define PREHEAT_CASE_BED  (PREHEAT_CASE_TEMP + ENABLED(HAS_HEATED_BED))
#define PREHEAT_CASE_FAN  (PREHEAT_CASE_BED + ENABLED(HAS_FAN))
#define PREHEAT_CASE_SAVE (PREHEAT_CASE_FAN + ENABLED(EEPROM_SETTINGS))
#define PREHEAT_CASE_TOTAL PREHEAT_CASE_SAVE

#define ADVSET_CASE_HOMEOFF   1
#define ADVSET_CASE_PROBEOFF  (ADVSET_CASE_HOMEOFF + ENABLED(HAS_ONESTEP_LEVELING))
#define ADVSET_CASE_HEPID     (ADVSET_CASE_PROBEOFF + ENABLED(HAS_HOTEND))
#define ADVSET_CASE_BEDPID    (ADVSET_CASE_HEPID + ENABLED(HAS_HEATED_BED))
#define ADVSET_CASE_PWRLOSSR  (ADVSET_CASE_BEDPID + ENABLED(POWER_LOSS_RECOVERY))
#define ADVSET_CASE_TOTAL     ADVSET_CASE_PWRLOSSR

//
// Draw Menus
//

void say_move_en(const uint8_t row) {
  Item_AreaCopy( 69,  61, 102,  71, row);         // "Move"
}
void say_max_en(const uint8_t row) {
  Item_AreaCopy( 75, 119, 100, 129, row);         // "Max"
}
void say_jerk_en(const uint8_t row) {
  Item_AreaCopy(104, 119, 128, 129, row, 30);     // "Jerk"
}
void say_speed_en(const uint16_t inset, const uint8_t row) {
  Item_AreaCopy(133, 119, 172, 132, row, inset);  // "Speed"
}
void say_max_accel_en(const uint8_t row) {
   say_max_en(row);                               // "Max"
   Item_AreaCopy(  0, 135,  79, 145, row, 30);    // "Acceleration"
}
void say_max_jerk_speed_en(const uint8_t row) {
  Item_AreaCopy( 75, 119, 172, 132, row);         // "Max Jerk Speed"
}
void say_x_en(const uint16_t inset, const uint8_t row) {
  Item_AreaCopy(175, 119, 184, 129, row, inset);  // "X"
}
void say_y_en(const uint16_t inset, const uint8_t row) {
  Item_AreaCopy(184, 119, 192, 129, row, inset);  // "Y"
}
void say_z_en(const uint16_t inset, const uint8_t row) {
  Item_AreaCopy(193, 119, 201, 129, row, inset);  // "Z"
}
void say_e_en(const uint16_t inset, const uint8_t row) {
  Item_AreaCopy(201, 119, 209, 129, row, inset);  // "E"
}
void say_pla_en(const uint16_t inset, const uint8_t row) {
  Item_AreaCopy(131, 164, 153, 174, row, inset);  // "PLA"
}
void say_abs_en(const uint16_t inset, const uint8_t row) {
  Item_AreaCopy(157,  76, 181,  86, row, inset);  // "ABS"
}
void say_home_offs_en(const uint8_t row) {
  Item_AreaCopy(153, 193, 225, 203, row);         // "Home Offset"
}
void say_probe_offs_en(const uint8_t row) {
  Item_AreaCopy(153, 205, 225, 215, row);         // "Probe Offset"
}
void say_steps_per_mm_en(const uint8_t row) {
  Item_AreaCopy(  1, 151,  91, 161, row);         // "Steps-per-mm"
}

void DWIN_Draw_Label(const uint8_t row, char *string) {
  DWIN_Draw_String(true, font8x16, Color_White, Color_Bg_Black, LBLX, MBASE(row), string);
}
void DWIN_Draw_Label(const uint8_t row, FSTR_P title) {
  DWIN_Draw_Label(row, (char*)title);
}

void DWIN_Draw_Signed_Float(uint8_t size, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, int32_t value) {
  DWIN_Draw_String(true, size, Color_White, bColor, x - 8, y, value < 0 ? F("-") : F(" "));
  DWIN_Draw_FloatValue(true, true, 0, size, Color_White, bColor, iNum, fNum, x, y, value < 0 ? -value : value);
}

void Draw_Edit_Integer3(const uint8_t row, const uint16_t value, const bool active=false) {
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, active ? Select_Color : Color_Bg_Black, 3, 220, EBASE(row), value);
}

void Draw_Edit_Integer4(const uint8_t row, const uint16_t value, const bool active=false) {
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, active ? Select_Color : Color_Bg_Black, 4, 220 - 1 * 8, EBASE(row), value);
}

void Draw_Edit_Float3(const uint8_t row, const uint16_t value, const bool active=false) {
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, active ? Select_Color : Color_Bg_Black, 3, UNITFDIGITS, 220 - UNITFDIGITS * 8, EBASE(row), (int32_t)value);
}

void Draw_Edit_Signed_Float2(const uint8_t row, const float value, const bool active=false) {
  DWIN_Draw_Signed_Float(font8x16, active ? Select_Color : Color_Bg_Black, 2, UNITFDIGITS, 220 + 8 - UNITFDIGITS * 8, EBASE(row), value);
}

void Draw_Edit_Signed_Float3(const uint8_t row, const float value, const bool active=false) {
  DWIN_Draw_Signed_Float(font8x16, active ? Select_Color : Color_Bg_Black, 3, UNITFDIGITS, 220 - UNITFDIGITS * 8, EBASE(row), value);
}

void Draw_Stat_Int(const uint16_t xpos, const uint16_t ypos, const uint16_t value) {
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, xpos, ypos, value);
}

void Draw_Stat_Float(const uint16_t xpos, const uint16_t ypos, const float value) {
  DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, xpos, ypos, value);
}

//
// Prepare Menu
//

void Item_Prepare_Move(const uint8_t row) {
  if (HMI_IsChinese())
    Item_AreaCopy(159, 70, 200, 84, row);
  else
    say_move_en(row);             // "Move"
  Draw_Menu_Line(row, ICON_Axis);
  Draw_More_Icon(row);
}

void Item_Prepare_Disable(const uint8_t row) {
  if (HMI_IsChinese())
    Item_AreaCopy(204, 70, 259, 82, row);
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, GET_TEXT_F(MSG_DISABLE_STEPPERS));
    #else
      Item_AreaCopy(104, 61, 191, 74, row); // "Disable Stepper"
    #endif
  }
  Draw_Menu_Line(row, ICON_CloseMotor);
}

void Item_Prepare_Home(const uint8_t row) {
  if (HMI_IsChinese())
    Item_AreaCopy(0, 89, 41, 101, row);
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, GET_TEXT_F(MSG_AUTO_HOME));
    #else
      Item_AreaCopy(202, 61, 271, 71, row); // "Auto Home"
    #endif
  }
  Draw_Menu_Line(row, ICON_Homing);
}

#if HAS_ZOFFSET_ITEM

  void Item_Prepare_Offset(const uint8_t row) {
    if (HMI_IsChinese()) {
      #if HAS_BED_PROBE
        Item_AreaCopy(174, 164, 223, 177, row);
      #else
        Item_AreaCopy(43, 89, 98, 101, row);
      #endif
    }
    else {
      #if HAS_BED_PROBE
        #ifdef USE_STRING_TITLES
          DWIN_Draw_Label(row, GET_TEXT_F(MSG_ZPROBE_ZOFFSET));
        #else
          Item_AreaCopy( 94, 179, 143, 190, row); // "Z-Offset"
        #endif
      #else
        #ifdef USE_STRING_TITLES
          DWIN_Draw_Label(row, GET_TEXT_F(MSG_SET_HOME_OFFSETS));
        #else
          Item_AreaCopy(  1,  76, 103,  87, row); // "Set home offsets"
        #endif
      #endif
    }
    Draw_Edit_Signed_Float2(row, BABY_Z_VAR * 100);
    Draw_Menu_Line(row, ICON_SetHome);
  }

#endif

#if HAS_HOTEND
  void Item_Prepare_PLA(const uint8_t row) {
    if (HMI_IsChinese())
      Item_AreaCopy(100, 89, 151, 101, row);
    else {
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(row, GET_TEXT_F(MSG_PREHEAT_1));
      #else
        Item_AreaCopy(108,  76, 155,  87, row); // "Preheat"
        say_pla_en(52, row);                    // "PLA"
      #endif
    }
    Draw_Menu_Line(row, ICON_PLAPreheat);
  }

  void Item_Prepare_ABS(const uint8_t row) {
    if (HMI_IsChinese())
      Item_AreaCopy(180,  89, 233, 100, row);
    else {
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(row, F("Preheat " PREHEAT_2_LABEL));
      #else
        Item_AreaCopy(108,  76, 155,  87, row); // "Preheat"
        say_abs_en(52, row);                    // "ABS"
      #endif
    }
    Draw_Menu_Line(row, ICON_ABSPreheat);
  }
#endif

#if HAS_PREHEAT
  void Item_Prepare_Cool(const uint8_t row) {
    if (HMI_IsChinese())
      Item_AreaCopy(1, 104,  56, 117, row);
    else {
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(row, GET_TEXT_F(MSG_COOLDOWN));
      #else
        Item_AreaCopy(200, 76, 264, 86, row); // "Cooldown"
      #endif
    }
    Draw_Menu_Line(row, ICON_Cool);
  }
#endif

void Item_Prepare_Lang(const uint8_t row) {
  if (HMI_IsChinese())
    Item_AreaCopy(239, 134, 266, 146, row);
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, F("UI Language"));
    #else
      Item_AreaCopy(1, 194, 96, 206, row);    // "LCD Language"
    #endif
  }
  DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Black, 226, EBASE(row), HMI_IsChinese() ? F("CN") : F("EN"));
  Draw_Menu_Icon(row, ICON_Language);
}

#define VISI(T,L,S) (WITHIN(L, T - MROWS, MROWS) || WITHIN(S, 0, MROWS))

void Draw_Prepare_Menu() {
  Clear_Main_Window();

  const int16_t scroll = MROWS - index_prepare; // Scrolled-up lines
  #define PSCROL(L) (scroll + (L))
  #define PVISI(L) VISI(PREPARE_CASE_TOTAL, L, PSCROL(L))

  if (HMI_IsChinese())
    DWIN_Frame_TitleCopy(133, 1, 28, 13);   // "Prepare"
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_PREPARE));
    #else
      DWIN_Frame_TitleCopy(179, 0, 48, 14); // "Prepare"
    #endif
  }

  if (PVISI(0)) Draw_Back_First(select_prepare.now == CASE_BACK);                 // < Back
  if (PVISI(PREPARE_CASE_MOVE)) Item_Prepare_Move(PSCROL(PREPARE_CASE_MOVE));     // Move >
  if (PVISI(PREPARE_CASE_DISA)) Item_Prepare_Disable(PSCROL(PREPARE_CASE_DISA));  // Disable Stepper
  if (PVISI(PREPARE_CASE_HOME)) Item_Prepare_Home(PSCROL(PREPARE_CASE_HOME));     // Auto Home
  #if HAS_ZOFFSET_ITEM
    if (PVISI(PREPARE_CASE_ZOFF)) Item_Prepare_Offset(PSCROL(PREPARE_CASE_ZOFF)); // Edit Z-Offset / Babystep / Set Home Offset
  #endif
  #if HAS_HOTEND
    if (PVISI(PREPARE_CASE_PLA)) Item_Prepare_PLA(PSCROL(PREPARE_CASE_PLA));      // Preheat PLA
    if (PVISI(PREPARE_CASE_ABS)) Item_Prepare_ABS(PSCROL(PREPARE_CASE_ABS));      // Preheat ABS
  #endif
  #if HAS_PREHEAT
    if (PVISI(PREPARE_CASE_COOL)) Item_Prepare_Cool(PSCROL(PREPARE_CASE_COOL));   // Cooldown
  #endif
  if (PVISI(PREPARE_CASE_LANG)) Item_Prepare_Lang(PSCROL(PREPARE_CASE_LANG));     // Language CN/EN

  if (select_prepare.now != CASE_BACK) Draw_Menu_Cursor(PSCROL(select_prepare.now));
}

//
// Control Menu
//

void Item_Control_Temp(const uint16_t row) {
  if (HMI_IsChinese())
    Item_AreaCopy(57, 104,  84, 116, row);
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, GET_TEXT_F(MSG_TEMPERATURE));
    #else
      Item_AreaCopy(1, 89,  83, 101, row);
    #endif
  }
  Draw_Menu_Line(row, ICON_Temperature);
  Draw_More_Icon(row);
}

void Item_Control_Motion(const uint16_t row) {
  if (HMI_IsChinese())
    Item_AreaCopy(87, 104, 114, 116, row);
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, GET_TEXT_F(MSG_MOTION));
    #else
      Item_AreaCopy(84, 89, 128,  99, row);
    #endif
  }
  Draw_Menu_Line(row, ICON_Motion);
  Draw_More_Icon(row);
}

void Item_Control_Advanced(const uint16_t row) {
  if (HMI_IsChinese())
    Item_AreaCopy(62, 180, 120, 192, row);
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, GET_TEXT_F(MSG_ADVANCED_SETTINGS));
    #else
      Item_AreaCopy(82, 135, 200, 149, row);
    #endif
  }
  Draw_Menu_Line(row, ICON_AdvSet);
  Draw_More_Icon(row);
}

void Item_Control_Info(const uint16_t row) {
  if (HMI_IsChinese())
    Item_AreaCopy(231, 104, 258, 116, row);
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, GET_TEXT_F(MSG_INFO_SCREEN));
    #else
      Item_AreaCopy(0, 104, 24, 114, row);
    #endif
  }
  Draw_Menu_Line(row, ICON_Info);
  Draw_More_Icon(row);
}

void Draw_Control_Menu() {
  Clear_Main_Window();

  #if CONTROL_CASE_TOTAL >= TROWS
    const int16_t scroll = MROWS - index_control; // Scrolled-up lines
  #else
    constexpr int16_t scroll = 0;
  #endif
  #define CSCROL(L) (scroll + (L))
  #define CLINE(L) MBASE(CSCROL(L))
  #define CVISI(L) VISI(CONTROL_CASE_TOTAL, L, CSCROL(L))

  if (HMI_IsChinese())
    DWIN_Frame_TitleCopy(103, 1, 28, 14);     // "Control"
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_CONTROL));
    #else
      DWIN_Frame_TitleCopy(128, 2, 49, 11);   // "Control"
    #endif
  }

  if (CVISI(0)) Draw_Back_First(select_control.now == CASE_BACK);               // < Back
  if (CVISI(CONTROL_CASE_TEMP)) Item_Control_Temp(CSCROL(CONTROL_CASE_TEMP));   // Temperature >
  if (CVISI(CONTROL_CASE_MOVE)) Item_Control_Motion(CSCROL(CONTROL_CASE_MOVE)); // Motion >

  if (HMI_IsChinese()) {
    #if ENABLED(EEPROM_SETTINGS)
      Item_AreaCopy(117, 104, 172, 116, CSCROL(CONTROL_CASE_SAVE));   // "Store Configuration"
      Item_AreaCopy(174, 103, 229, 116, CSCROL(CONTROL_CASE_LOAD));   // "Read Configuration"
      Item_AreaCopy(  1, 118,  56, 131, CSCROL(CONTROL_CASE_RESET));  // "Reset Configuration"
    #endif
  }
  else {
    #ifdef USE_STRING_TITLES
      #if ENABLED(EEPROM_SETTINGS)
        if (CVISI(CONTROL_CASE_SAVE)) DWIN_Draw_Label(CSCROL(CONTROL_CASE_SAVE), GET_TEXT_F(MSG_STORE_EEPROM));        // "Store Configuration"
        if (CVISI(CONTROL_CASE_LOAD)) DWIN_Draw_Label(CSCROL(CONTROL_CASE_LOAD), GET_TEXT_F(MSG_LOAD_EEPROM));         // "Read Configuration"
        if (CVISI(CONTROL_CASE_RESET)) DWIN_Draw_Label(CSCROL(CONTROL_CASE_RESET), GET_TEXT_F(MSG_RESTORE_DEFAULTS));  // "Reset Configuration"
      #endif
    #else
      #if ENABLED(EEPROM_SETTINGS)
        if (CVISI(CONTROL_CASE_SAVE))
          Item_AreaCopy(150,  89, 263, 102, CSCROL(CONTROL_CASE_SAVE));       // "Store Configuration"
        if (CVISI(CONTROL_CASE_LOAD)) {
          Item_AreaCopy( 26, 104,  57, 114, CSCROL(CONTROL_CASE_LOAD));       // "Read"
          Item_AreaCopy(182,  89, 263, 102, CSCROL(CONTROL_CASE_LOAD), 34);   // "Configuration"
        }
        if (CVISI(CONTROL_CASE_RESET)) {
          Item_AreaCopy( 59, 104,  93, 114, CSCROL(CONTROL_CASE_RESET));      // "Reset"
          Item_AreaCopy(182,  89, 263, 102, CSCROL(CONTROL_CASE_RESET), 37);  // "Configuration"
        }
      #endif
    #endif
  }

  if (CVISI(CONTROL_CASE_ADVSET)) Item_Control_Advanced(CSCROL(CONTROL_CASE_ADVSET));
  if (CVISI(CONTROL_CASE_INFO)) Item_Control_Info(CSCROL(CONTROL_CASE_INFO));

  if (select_control.now != CASE_BACK && CVISI(select_control.now))
    Draw_Menu_Cursor(CSCROL(select_control.now));

  // Draw icons and lines
  #define _TEMP_ICON(N, I, M) do { \
    if (CVISI(N)) { \
      Draw_Menu_Line(CSCROL(N), I); \
      if (M) { \
        Draw_More_Icon(CSCROL(N)); \
      } \
    } \
  } while(0)

  #if ENABLED(EEPROM_SETTINGS)
    _TEMP_ICON(CONTROL_CASE_SAVE, ICON_WriteEEPROM, false);
    _TEMP_ICON(CONTROL_CASE_LOAD, ICON_ReadEEPROM, false);
    _TEMP_ICON(CONTROL_CASE_RESET, ICON_ResumeEEPROM, false);
  #endif
}

//
// Tune Menu
//

void Draw_Tune_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(73, 2, 28, 12);       // "Tune"
    Item_AreaCopy(116, 164, 171, 176, TUNE_CASE_SPEED);
    #if HAS_HOTEND
      Item_AreaCopy(1, 134, 56, 146, TUNE_CASE_TEMP);
    #endif
    #if HAS_HEATED_BED
      Item_AreaCopy(58, 134, 113, 146, TUNE_CASE_BED);
    #endif
    #if HAS_FAN
      Item_AreaCopy(115, 134, 170, 146, TUNE_CASE_FAN);
    #endif
    #if HAS_ZOFFSET_ITEM
      Item_AreaCopy(174, 164, 223, 177, TUNE_CASE_ZOFF);
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_TUNE));
    #else
      DWIN_Frame_TitleCopy(94, 2, 33, 11);    // "Tune"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(TUNE_CASE_SPEED, GET_TEXT_F(MSG_SPEED));
      #if HAS_HOTEND
        DWIN_Draw_Label(TUNE_CASE_TEMP, GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND));
      #endif
      #if HAS_HEATED_BED
        DWIN_Draw_Label(TUNE_CASE_BED, GET_TEXT_F(MSG_UBL_SET_TEMP_BED));
      #endif
      #if HAS_FAN
        DWIN_Draw_Label(TUNE_CASE_FAN, GET_TEXT_F(MSG_FAN_SPEED));
      #endif
      DWIN_Draw_Label(TUNE_CASE_ZOFF, GET_TEXT_F(MSG_ZPROBE_ZOFFSET));
    #else
      Item_AreaCopy(1, 179, 92, 190, TUNE_CASE_SPEED);          // "Print speed"
      #if HAS_HOTEND
        Item_AreaCopy(197, 104, 238, 114, TUNE_CASE_TEMP);      // "Hotend"
        Item_AreaCopy(  1,  89,  83, 101, TUNE_CASE_TEMP, 44);  // "Temperature"
      #endif
      #if HAS_HEATED_BED
        Item_AreaCopy(240, 104, 264, 114, TUNE_CASE_BED);       // "Bed"
        Item_AreaCopy(  1,  89,  83, 101, TUNE_CASE_BED, 27);   // "Temperature"
      #endif
      #if HAS_FAN
        Item_AreaCopy(0, 119, 64, 132, TUNE_CASE_FAN);          // "Fan speed"
      #endif
      #if HAS_ZOFFSET_ITEM
        Item_AreaCopy(93, 179, 141, 189, TUNE_CASE_ZOFF);       // "Z-offset"
      #endif
    #endif
  }

  Draw_Back_First(select_tune.now == CASE_BACK);
  if (select_tune.now != CASE_BACK) Draw_Menu_Cursor(select_tune.now);

  Draw_Menu_Line(TUNE_CASE_SPEED, ICON_Speed);
  Draw_Edit_Integer3(TUNE_CASE_SPEED, feedrate_percentage);

  #if HAS_HOTEND
    Draw_Menu_Line(TUNE_CASE_TEMP, ICON_HotendTemp);
    Draw_Edit_Integer3(TUNE_CASE_TEMP, thermalManager.degTargetHotend(0));
  #endif
  #if HAS_HEATED_BED
    Draw_Menu_Line(TUNE_CASE_BED, ICON_BedTemp);
    Draw_Edit_Integer3(TUNE_CASE_BED, thermalManager.degTargetBed());
  #endif
  #if HAS_FAN
    Draw_Menu_Line(TUNE_CASE_FAN, ICON_FanSpeed);
    Draw_Edit_Integer3(TUNE_CASE_FAN, thermalManager.fan_speed[0]);
  #endif
  #if HAS_ZOFFSET_ITEM
    Draw_Menu_Line(TUNE_CASE_ZOFF, ICON_Zoffset);
    Draw_Edit_Signed_Float2(TUNE_CASE_ZOFF, BABY_Z_VAR * 100);
  #endif
}

//
// Motion Menu
//
void Draw_Motion_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 16, 28, 13);                        // "Motion"
    Item_AreaCopy(173, 133, 228, 147, MOTION_CASE_RATE);        // Max speed
    Item_AreaCopy(173, 133, 200, 147, MOTION_CASE_ACCEL);       // Max...
    Item_AreaCopy(28, 149, 69, 161, MOTION_CASE_ACCEL, 30, 1);  // ...Acceleration
    #if HAS_CLASSIC_JERK
      Item_AreaCopy(173, 133, 200, 147, MOTION_CASE_JERK);      // Max...
      Item_AreaCopy(1, 180, 28, 192, MOTION_CASE_JERK, 30, 1);  // ...
      Item_AreaCopy(202, 133, 228, 147, MOTION_CASE_JERK, 57);  // ...Jerk
    #endif
    Item_AreaCopy(153, 148, 194, 161, MOTION_CASE_STEPS);       // Flow ratio
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MOTION));
    #else
      DWIN_Frame_TitleCopy(144, 16, 46, 11);                            // "Motion"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MOTION_CASE_RATE, F("Feedrate"));                 // "Feedrate"
      DWIN_Draw_Label(MOTION_CASE_ACCEL, GET_TEXT_F(MSG_ACCELERATION)); // "Acceleration"
      #if HAS_CLASSIC_JERK
        DWIN_Draw_Label(MOTION_CASE_JERK, GET_TEXT_F(MSG_JERK));        // "Jerk"
      #endif
      DWIN_Draw_Label(MOTION_CASE_STEPS, GET_TEXT_F(MSG_STEPS_PER_MM)); // "Steps/mm"
    #else
      say_max_en(MOTION_CASE_RATE); say_speed_en(30, MOTION_CASE_RATE); // "Max Speed"
      say_max_accel_en(MOTION_CASE_ACCEL);                              // "Max Acceleration"
      #if HAS_CLASSIC_JERK
        say_max_en(MOTION_CASE_JERK); say_jerk_en(MOTION_CASE_JERK);    // "Max Jerk"
      #endif
      say_steps_per_mm_en(MOTION_CASE_STEPS);                           // "Steps-per-mm"
    #endif
  }

  Draw_Back_First(select_motion.now == CASE_BACK);
  if (select_motion.now != CASE_BACK) Draw_Menu_Cursor(select_motion.now);

  uint8_t i = 0;
  #define _MOTION_ICON(N) Draw_Menu_Line(++i, ICON_MaxSpeed + (N) - 1)
  _MOTION_ICON(MOTION_CASE_RATE); Draw_More_Icon(i);
  _MOTION_ICON(MOTION_CASE_ACCEL); Draw_More_Icon(i);
  #if HAS_CLASSIC_JERK
    _MOTION_ICON(MOTION_CASE_JERK); Draw_More_Icon(i);
  #endif
  _MOTION_ICON(MOTION_CASE_STEPS); Draw_More_Icon(i);
}

//
// Draw Popup Windows
//

#if HAS_HOTEND || HAS_HEATED_BED

  void DWIN_Popup_Temperature(const bool toohigh) {
    Clear_Popup_Area();
    Draw_Popup_Bkgd_105();
    if (toohigh) {
      DWIN_ICON_Show(ICON, ICON_TempTooHigh, 102, 165);
      if (HMI_IsChinese()) {
        DWIN_Frame_AreaCopy(1, 103, 371, 237, 386,  52, 285); // Temp Too High
        DWIN_Frame_AreaCopy(1, 151, 389, 185, 402, 187, 285);
        DWIN_Frame_AreaCopy(1, 189, 389, 271, 402,  95, 310);
      }
      else {
        DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, 36, 300, F("Nozzle or Bed temperature"));
        DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, 92, 300, F("is too high"));
      }
    }
    else {
      DWIN_ICON_Show(ICON, ICON_TempTooLow, 102, 165);
      if (HMI_IsChinese()) {
        DWIN_Frame_AreaCopy(1, 103, 371, 270, 386, 52, 285); // Tenp Too Low
        DWIN_Frame_AreaCopy(1, 189, 389, 271, 402, 95, 310);
      }
      else {
        DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, 36, 300, F("Nozzle or Bed temperature"));
        DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, 92, 300, F("is too low"));
      }
    }
  }

#endif

void Draw_Popup_Bkgd_60() {
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 330);
}

#if HAS_HOTEND

  void Popup_Window_ETempTooLow() {
    Clear_Main_Window();
    Draw_Popup_Bkgd_60();
    DWIN_ICON_Show(ICON, ICON_TempTooLow, 102, 105);
    if (HMI_IsChinese()) {
      DWIN_Frame_AreaCopy(1, 103, 371, 136, 386, 69, 240);      // Nozzle Too Cold
      DWIN_Frame_AreaCopy(1, 170, 371, 270, 386, 69 + 33, 240);
      DWIN_ICON_Show(ICON, ICON_Confirm_C, 86, 280);
    }
    else {
      DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, 20, 235, F("Nozzle is too cold"));
      DWIN_ICON_Show(ICON, ICON_Confirm_E, 86, 280);
    }
  }

#endif

void Popup_Window_Resume() {
  Clear_Popup_Area();
  Draw_Popup_Bkgd_105();
  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, 160, 338, 235, 354, 98, 135);    // Resume Interrupted Print
    DWIN_Frame_AreaCopy(1, 103, 321, 271, 335, 52, 192);
    DWIN_ICON_Show(ICON, ICON_Cancel_C,    26, 307);
    DWIN_ICON_Show(ICON, ICON_Continue_C, 146, 307);
  }
  else {
    DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 14) / 2, 115, F("Continue Print"));
    DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 22) / 2, 192, F("It looks like the last"));
    DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 22) / 2, 212, F("file was interrupted."));
    DWIN_ICON_Show(ICON, ICON_Cancel_E,    26, 307);
    DWIN_ICON_Show(ICON, ICON_Continue_E, 146, 307);
  }
}

void Popup_Window_Home(const bool parking/*=false*/) {
  Clear_Main_Window();
  Draw_Popup_Bkgd_60();
  DWIN_ICON_Show(ICON, ICON_BLTouch, 101, 105);
  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, 0, 371, 33, 386, 85, 240);       // Wait for Move to Complete
    DWIN_Frame_AreaCopy(1, 203, 286, 271, 302, 118, 240);
    DWIN_Frame_AreaCopy(1, 0, 389, 150, 402, 61, 280);
  }
  else {
    DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * (parking ? 7 : 10)) / 2, 230, parking ? F("Parking") : F("Homing XYZ"));
    DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 23) / 2, 260, F("Please wait until done."));
  }
}

#if HAS_ONESTEP_LEVELING

  void Popup_Window_Leveling() {
    Clear_Main_Window();
    Draw_Popup_Bkgd_60();
    DWIN_ICON_Show(ICON, ICON_AutoLeveling, 101, 105);
    if (HMI_IsChinese()) {
      DWIN_Frame_AreaCopy(1, 0, 371, 100, 386, 84, 240);    // Wait for Leveling
      DWIN_Frame_AreaCopy(1, 0, 389, 150, 402, 61, 280);
    }
    else {
      DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 13) / 2, 230, GET_TEXT_F(MSG_BED_LEVELING));
      DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 23) / 2, 260, F("Please wait until done."));
    }
  }

#endif

void Draw_Select_Highlight(const bool sel) {
  HMI_flag.select_flag = sel;
  const uint16_t c1 = sel ? Select_Color : Color_Bg_Window,
                 c2 = sel ? Color_Bg_Window : Select_Color;
  DWIN_Draw_Rectangle(0, c1, 25, 279, 126, 318);
  DWIN_Draw_Rectangle(0, c1, 24, 278, 127, 319);
  DWIN_Draw_Rectangle(0, c2, 145, 279, 246, 318);
  DWIN_Draw_Rectangle(0, c2, 144, 278, 247, 319);
}

void Popup_window_PauseOrStop() {
  Clear_Main_Window();
  Draw_Popup_Bkgd_60();
  if (HMI_IsChinese()) {
         if (select_print.now == PRINT_PAUSE_RESUME) DWIN_Frame_AreaCopy(1, 237, 338, 269, 356, 98, 150);    // Pause
    else if (select_print.now == PRINT_STOP) DWIN_Frame_AreaCopy(1, 221, 320, 253, 336, 98, 150);    // Stop
    DWIN_Frame_AreaCopy(1, 220, 304, 264, 319, 130, 150); // Print
    DWIN_ICON_Show(ICON, ICON_Confirm_C, 26, 280);
    DWIN_ICON_Show(ICON, ICON_Cancel_C, 146, 280);
  }
  else {
         if (select_print.now == PRINT_PAUSE_RESUME) DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 11) / 2, 150, GET_TEXT_F(MSG_PAUSE_PRINT));
    else if (select_print.now == PRINT_STOP) DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 10) / 2, 150, GET_TEXT_F(MSG_STOP_PRINT));
    DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
    DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
  }
  Draw_Select_Highlight(true);
}

void Draw_Printing_Screen() {
  const uint16_t y = 168;
  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(30, 1, 42, 14);              // "Printing"
    DWIN_Frame_AreaCopy(1,  0, 72,  63, 86,  43, y);  // "Printing Time"
    DWIN_Frame_AreaCopy(1, 65, 72, 128, 86, 178, y);  // "Remain"
  }
  else {
    DWIN_Frame_TitleCopy(42, 0, 47, 14);              // "Printing"
    DWIN_Frame_AreaCopy(1,   1, 43,  97, 59,  43, y); // "Printing Time"
    DWIN_Frame_AreaCopy(1, 100, 43, 152, 56, 178, y); // "Remain"
  }
}

void Draw_Print_ProgressBar() {
  constexpr uint16_t y = 93, h = 21;
  DWIN_ICON_Show(ICON, ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, BarFill_Color, 16 + _card_percent * 240 / 100, y, 256, y + h - 1);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Percent_Color, Color_Bg_Black, 2, 117, y + 40, _card_percent);
  DWIN_Draw_String(false, font8x16, Percent_Color, Color_Bg_Black, 133, y + 40, F("%"));
}

void Draw_Print_ProgressElapsed() {
  constexpr uint16_t x = 45, y = 192;
  duration_t elapsed = print_job_timer.duration(); // print timer
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, x, y, elapsed.value / 3600);
  DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Black, x + 8 * 2, y, F(":"));
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, x + 8 * 3, y, (elapsed.value % 3600) / 60);
}

void Draw_Print_ProgressRemain() {
  constexpr uint16_t x = 179, y = 192;
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, x, y, _remain_time / 3600);
  DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Black, x + 8 * 2, y, F(":"));
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, x + 8 * 3, y, (_remain_time % 3600) / 60);
}

void Goto_PrintProcess() {
  checkkey = PrintProcess;

  Clear_Main_Window();
  Draw_Printing_Screen();

  ICON_Tune();
  ICON_ResumeOrPause();
  ICON_Stop();

  // Copy into filebuf string before entry
  char * const name = card.longest_filename();
  const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(name) * MENU_CHR_W) / 2;
  DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Black, npos, 60, name);

  DWIN_ICON_Show(ICON, ICON_PrintTime,   17, 163);
  DWIN_ICON_Show(ICON, ICON_RemainTime, 150, 161);

  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();
}

void Goto_MainMenu() {
  checkkey = MainMenu;

  Clear_Main_Window();

  if (HMI_IsChinese())
    DWIN_Frame_TitleCopy(2, 2, 26, 13);   // "Home" etc
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MAIN));
    #else
      DWIN_Frame_TitleCopy(0, 2, 40, 11); // "Home"
    #endif
  }

  DWIN_ICON_Show(ICON, ICON_LOGO, 71, 52);

  ICON_Print();
  ICON_Prepare();
  ICON_Control();
  TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)();
}

inline EncoderState get_encoder_state() {
  static millis_t Encoder_ms = 0;
  const millis_t ms = millis();
  if (PENDING(ms, Encoder_ms)) return ENCODER_DIFF_NO;
  const EncoderState state = Encoder_ReceiveAnalyze();
  if (state != ENCODER_DIFF_NO) Encoder_ms = ms + ENCODER_WAIT_MS;
  return state;
}

void HMI_Plan_Move(const feedRate_t fr_mm_s) {
  if (!planner.is_full()) {
    planner.synchronize();
    planner.buffer_line(current_position, fr_mm_s);
    DWIN_UpdateLCD();
  }
}

void HMI_Move_Done(const AxisEnum axis) {
  EncoderRate.enabled = false;
  planner.synchronize();
  checkkey = AxisMove;
  DWIN_UpdateLCD();
}

void HMI_Move_X() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_X_scaled)) {
    Draw_Edit_Float3(1, HMI_ValueStruct.Move_X_scaled);
    return HMI_Move_Done(X_AXIS);
  }
  LIMIT(HMI_ValueStruct.Move_X_scaled, (X_MIN_POS) * MINUNITMULT, (X_MAX_POS) * MINUNITMULT);
  current_position.x = HMI_ValueStruct.Move_X_scaled / MINUNITMULT;
  Draw_Edit_Float3(1, HMI_ValueStruct.Move_X_scaled, true);
  DWIN_UpdateLCD();
  HMI_Plan_Move(homing_feedrate(X_AXIS));
}

void HMI_Move_Y() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_Y_scaled)) {
    Draw_Edit_Float3(2, HMI_ValueStruct.Move_Y_scaled);
    return HMI_Move_Done(Y_AXIS);
  }
  LIMIT(HMI_ValueStruct.Move_Y_scaled, (Y_MIN_POS) * MINUNITMULT, (Y_MAX_POS) * MINUNITMULT);
  current_position.y = HMI_ValueStruct.Move_Y_scaled / MINUNITMULT;
  Draw_Edit_Float3(2, HMI_ValueStruct.Move_Y_scaled, true);
  DWIN_UpdateLCD();
  HMI_Plan_Move(homing_feedrate(Y_AXIS));
}

void HMI_Move_Z() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_Z_scaled)) {
    Draw_Edit_Float3(3, HMI_ValueStruct.Move_Z_scaled);
    return HMI_Move_Done(Z_AXIS);
  }
  LIMIT(HMI_ValueStruct.Move_Z_scaled, (Z_MIN_POS) * MINUNITMULT, (Z_MAX_POS) * MINUNITMULT);
  current_position.z = HMI_ValueStruct.Move_Z_scaled / MINUNITMULT;
  Draw_Edit_Float3(3, HMI_ValueStruct.Move_Z_scaled, true);
  DWIN_UpdateLCD();
  HMI_Plan_Move(homing_feedrate(Z_AXIS));
}

#if HAS_HOTEND

  void HMI_Move_E() {
    static float last_E_scaled = 0;
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_E_scaled)) {
      last_E_scaled = HMI_ValueStruct.Move_E_scaled;
      Draw_Edit_Signed_Float3(4, last_E_scaled);
      return HMI_Move_Done(E_AXIS);
    }
    LIMIT(HMI_ValueStruct.Move_E_scaled, last_E_scaled - (EXTRUDE_MAXLENGTH) * MINUNITMULT, last_E_scaled + (EXTRUDE_MAXLENGTH) * MINUNITMULT);
    current_position.e = HMI_ValueStruct.Move_E_scaled / MINUNITMULT;
    Draw_Edit_Signed_Float3(4, HMI_ValueStruct.Move_E_scaled, true);
    DWIN_UpdateLCD();
    HMI_Plan_Move(MMM_TO_MMS(FEEDRATE_E));
  }

#endif

#if HAS_ZOFFSET_ITEM

  bool printer_busy() { return planner.movesplanned() || printingIsActive(); }

  void HMI_Zoffset() {
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    uint8_t zoff_line;
    switch (HMI_ValueStruct.show_mode) {
      case -4: zoff_line = PREPARE_CASE_ZOFF + MROWS - index_prepare; break;
      default: zoff_line = TUNE_CASE_ZOFF + MROWS - index_tune;
    }
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.offset_value)) {
      EncoderRate.enabled = false;
      #if HAS_BED_PROBE
        probe.offset.z = dwin_zoffset;
        TERN_(EEPROM_SETTINGS, settings.save());
      #endif
      checkkey = HMI_ValueStruct.show_mode == -4 ? Prepare : Tune;
      Draw_Edit_Signed_Float2(zoff_line, TERN(HAS_BED_PROBE, BABY_Z_VAR * 100, HMI_ValueStruct.offset_value));
      DWIN_UpdateLCD();
      return;
    }
    LIMIT(HMI_ValueStruct.offset_value, (Z_PROBE_OFFSET_RANGE_MIN) * 100, (Z_PROBE_OFFSET_RANGE_MAX) * 100);
    last_zoffset = dwin_zoffset;
    dwin_zoffset = HMI_ValueStruct.offset_value / 100.0f;
    #if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      if (BABYSTEP_ALLOWED()) babystep.add_mm(Z_AXIS, dwin_zoffset - last_zoffset);
    #endif
    Draw_Edit_Signed_Float2(zoff_line, HMI_ValueStruct.offset_value, true);
    DWIN_UpdateLCD();
  }

#endif // HAS_ZOFFSET_ITEM

#if HAS_HOTEND

  void HMI_ETemp() {
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    uint8_t temp_line;
    switch (HMI_ValueStruct.show_mode) {
      case -1: temp_line = TEMP_CASE_TEMP; break;
      case -2: temp_line = PREHEAT_CASE_TEMP; break;
      case -3: temp_line = PREHEAT_CASE_TEMP; break;
      default: temp_line = TUNE_CASE_TEMP + MROWS - index_tune;
    }
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.E_Temp)) {
      EncoderRate.enabled = false;
      if (HMI_ValueStruct.show_mode == -2) {
        checkkey = PLAPreheat;
        ui.material_preset[0].hotend_temp = HMI_ValueStruct.E_Temp;
        Draw_Edit_Integer3(temp_line, ui.material_preset[0].hotend_temp);
        return;
      }
      else if (HMI_ValueStruct.show_mode == -3) {
        checkkey = ABSPreheat;
        ui.material_preset[1].hotend_temp = HMI_ValueStruct.E_Temp;
        Draw_Edit_Integer3(temp_line, ui.material_preset[1].hotend_temp);
        return;
      }
      else if (HMI_ValueStruct.show_mode == -1) // Temperature
        checkkey = TemperatureID;
      else
        checkkey = Tune;
      Draw_Edit_Integer3(temp_line, HMI_ValueStruct.E_Temp);
      thermalManager.setTargetHotend(HMI_ValueStruct.E_Temp, 0);
      return;
    }
    // E_Temp limit
    LIMIT(HMI_ValueStruct.E_Temp, HEATER_0_MINTEMP, thermalManager.hotend_max_target(0));
    // E_Temp value
    Draw_Edit_Integer3(temp_line, HMI_ValueStruct.E_Temp, true);
  }

#endif // HAS_HOTEND

#if HAS_HEATED_BED

  void HMI_BedTemp() {
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    uint8_t bed_line;
    switch (HMI_ValueStruct.show_mode) {
      case -1: bed_line = TEMP_CASE_BED; break;
      case -2: bed_line = PREHEAT_CASE_BED; break;
      case -3: bed_line = PREHEAT_CASE_BED; break;
      default: bed_line = TUNE_CASE_BED + MROWS - index_tune;
    }
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Bed_Temp)) {
      EncoderRate.enabled = false;
      if (HMI_ValueStruct.show_mode == -2) {
        checkkey = PLAPreheat;
        ui.material_preset[0].bed_temp = HMI_ValueStruct.Bed_Temp;
        Draw_Edit_Integer3(bed_line, ui.material_preset[0].bed_temp);
        return;
      }
      else if (HMI_ValueStruct.show_mode == -3) {
        checkkey = ABSPreheat;
        ui.material_preset[1].bed_temp = HMI_ValueStruct.Bed_Temp;
        Draw_Edit_Integer3(bed_line, ui.material_preset[1].bed_temp);
        return;
      }
      else if (HMI_ValueStruct.show_mode == -1)
        checkkey = TemperatureID;
      else
        checkkey = Tune;
      Draw_Edit_Integer3(bed_line, HMI_ValueStruct.Bed_Temp);
      thermalManager.setTargetBed(HMI_ValueStruct.Bed_Temp);
      return;
    }
    // Bed_Temp limit
    LIMIT(HMI_ValueStruct.Bed_Temp, BED_MINTEMP, BED_MAX_TARGET);
    // Bed_Temp value
    Draw_Edit_Integer3(bed_line, HMI_ValueStruct.Bed_Temp, true);
  }

#endif // HAS_HEATED_BED

#if HAS_PREHEAT && HAS_FAN

  void HMI_FanSpeed() {
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    uint8_t fan_line;
    switch (HMI_ValueStruct.show_mode) {
      case -1: fan_line = TEMP_CASE_FAN; break;
      case -2: fan_line = PREHEAT_CASE_FAN; break;
      case -3: fan_line = PREHEAT_CASE_FAN; break;
      default: fan_line = TUNE_CASE_FAN + MROWS - index_tune;
    }

    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Fan_speed)) {
      EncoderRate.enabled = false;
      if (HMI_ValueStruct.show_mode == -2) {
        checkkey = PLAPreheat;
        ui.material_preset[0].fan_speed = HMI_ValueStruct.Fan_speed;
        Draw_Edit_Integer3(fan_line, ui.material_preset[0].fan_speed);
        return;
      }
      else if (HMI_ValueStruct.show_mode == -3) {
        checkkey = ABSPreheat;
        ui.material_preset[1].fan_speed = HMI_ValueStruct.Fan_speed;
        Draw_Edit_Integer3(fan_line, ui.material_preset[1].fan_speed);
        return;
      }
      else if (HMI_ValueStruct.show_mode == -1)
        checkkey = TemperatureID;
      else
        checkkey = Tune;
      Draw_Edit_Integer3(fan_line, HMI_ValueStruct.Fan_speed);
      thermalManager.set_fan_speed(0, HMI_ValueStruct.Fan_speed);
      return;
    }
    // Fan_speed limit
    LIMIT(HMI_ValueStruct.Fan_speed, 0, 255);
    // Fan_speed value
    Draw_Edit_Integer3(fan_line, HMI_ValueStruct.Fan_speed, true);
  }

#endif // HAS_PREHEAT && HAS_FAN

void HMI_PrintSpeed() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.print_speed)) {
    checkkey = Tune;
    EncoderRate.enabled = false;
    feedrate_percentage = HMI_ValueStruct.print_speed;
    Draw_Edit_Integer3(select_tune.now + MROWS - index_tune, HMI_ValueStruct.print_speed);
    return;
  }
  // print_speed limit
  LIMIT(HMI_ValueStruct.print_speed, MIN_PRINT_SPEED, MAX_PRINT_SPEED);
  // print_speed value
  Draw_Edit_Integer3(select_tune.now + MROWS - index_tune, HMI_ValueStruct.print_speed, true);
}

#define LAST_AXIS TERN(HAS_HOTEND, E_AXIS, Z_AXIS)

void HMI_MaxFeedspeedXYZE() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Feedspeed)) {
    checkkey = MaxSpeed;
    EncoderRate.enabled = false;
    if (WITHIN(HMI_flag.feedspeed_axis, X_AXIS, LAST_AXIS))
      planner.set_max_feedrate(HMI_flag.feedspeed_axis, HMI_ValueStruct.Max_Feedspeed);
    Draw_Edit_Integer4(select_speed.now, HMI_ValueStruct.Max_Feedspeed);
    return;
  }
  // MaxFeedspeed limit
  if (WITHIN(HMI_flag.feedspeed_axis, X_AXIS, LAST_AXIS))
    NOMORE(HMI_ValueStruct.Max_Feedspeed, default_max_feedrate[HMI_flag.feedspeed_axis] * 2);
  if (HMI_ValueStruct.Max_Feedspeed < MIN_MAXFEEDSPEED) HMI_ValueStruct.Max_Feedspeed = MIN_MAXFEEDSPEED;
  // MaxFeedspeed value
  Draw_Edit_Integer4(select_speed.now, HMI_ValueStruct.Max_Feedspeed, true);
}

void HMI_MaxAccelerationXYZE() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Acceleration)) {
    checkkey = MaxAcceleration;
    EncoderRate.enabled = false;
    if (WITHIN(HMI_flag.acc_axis, X_AXIS, LAST_AXIS))
      planner.set_max_acceleration(HMI_flag.acc_axis, HMI_ValueStruct.Max_Acceleration);
    Draw_Edit_Integer4(select_acc.now, HMI_ValueStruct.Max_Acceleration);
    return;
  }
  // MaxAcceleration limit
  if (WITHIN(HMI_flag.acc_axis, X_AXIS, LAST_AXIS))
    NOMORE(HMI_ValueStruct.Max_Acceleration, default_max_acceleration[HMI_flag.acc_axis] * 2);
  if (HMI_ValueStruct.Max_Acceleration < MIN_MAXACCELERATION) HMI_ValueStruct.Max_Acceleration = MIN_MAXACCELERATION;
  // MaxAcceleration value
  Draw_Edit_Integer4(select_acc.now, HMI_ValueStruct.Max_Acceleration, true);
}

#if HAS_CLASSIC_JERK

  void HMI_MaxJerkXYZE() {
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Jerk_scaled)) {
      checkkey = MaxJerk;
      EncoderRate.enabled = false;
      if (WITHIN(HMI_flag.jerk_axis, X_AXIS, LAST_AXIS))
        planner.set_max_jerk(HMI_flag.jerk_axis, HMI_ValueStruct.Max_Jerk_scaled / 10);
      Draw_Edit_Float3(select_jerk.now, HMI_ValueStruct.Max_Jerk_scaled);
      return;
    }
    // MaxJerk limit
    if (WITHIN(HMI_flag.jerk_axis, X_AXIS, LAST_AXIS))
      NOMORE(HMI_ValueStruct.Max_Jerk_scaled, default_max_jerk[HMI_flag.jerk_axis] * 2 * MINUNITMULT);
    NOLESS(HMI_ValueStruct.Max_Jerk_scaled, (MIN_MAXJERK) * MINUNITMULT);
    // MaxJerk value
    Draw_Edit_Float3(select_jerk.now, HMI_ValueStruct.Max_Jerk_scaled, true);
  }

#endif // HAS_CLASSIC_JERK

void HMI_StepXYZE() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Step_scaled)) {
    checkkey = Step;
    EncoderRate.enabled = false;
    if (WITHIN(HMI_flag.step_axis, X_AXIS, LAST_AXIS))
      planner.settings.axis_steps_per_mm[HMI_flag.step_axis] = HMI_ValueStruct.Max_Step_scaled / 10;
    Draw_Edit_Float3(select_step.now, HMI_ValueStruct.Max_Step_scaled);
    return;
  }
  // Step limit
  if (WITHIN(HMI_flag.step_axis, X_AXIS, LAST_AXIS))
    NOMORE(HMI_ValueStruct.Max_Step_scaled, 999.9 * MINUNITMULT);
  NOLESS(HMI_ValueStruct.Max_Step_scaled, MIN_STEP);
  // Step value
  Draw_Edit_Float3(select_step.now, HMI_ValueStruct.Max_Step_scaled, true);
}

// Draw X, Y, Z and blink if in an un-homed or un-trusted state
void _update_axis_value(const AxisEnum axis, const uint16_t x, const uint16_t y, const bool blink, const bool force) {
  const bool draw_qmark = axis_should_home(axis),
             draw_empty = NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING) && !draw_qmark && !axis_is_trusted(axis);

  // Check for a position change
  static xyz_pos_t oldpos = { -1, -1, -1 };
  const float p = current_position[axis];
  const bool changed = oldpos[axis] != p;
  if (changed) oldpos[axis] = p;

  if (force || changed || draw_qmark || draw_empty) {
    if (blink && draw_qmark)
      DWIN_Draw_String(true, font8x16, Color_White, Color_Bg_Black, x, y, F("???.?"));
    else if (blink && draw_empty)
      DWIN_Draw_String(true, font8x16, Color_White, Color_Bg_Black, x, y, F("     "));
    else
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, x, y, p * 10);
  }
}

void _draw_xyz_position(const bool force) {
  //SERIAL_ECHOPGM("Draw XYZ:");
  static bool _blink = false;
  const bool blink = !!(millis() & 0x400UL);
  if (force || blink != _blink) {
    _blink = blink;
    //SERIAL_ECHOPGM(" (blink)");
    _update_axis_value(X_AXIS,  35, 459, blink, true);
    _update_axis_value(Y_AXIS, 120, 459, blink, true);
    _update_axis_value(Z_AXIS, 205, 459, blink, true);
  }
  //SERIAL_EOL();
}

void update_variable() {
  #if HAS_HOTEND
    static celsius_t _hotendtemp = 0, _hotendtarget = 0;
    const celsius_t hc = thermalManager.wholeDegHotend(0),
                    ht = thermalManager.degTargetHotend(0);
    const bool _new_hotend_temp = _hotendtemp != hc,
               _new_hotend_target = _hotendtarget != ht;
    if (_new_hotend_temp) _hotendtemp = hc;
    if (_new_hotend_target) _hotendtarget = ht;
  #endif
  #if HAS_HEATED_BED
    static celsius_t _bedtemp = 0, _bedtarget = 0;
    const celsius_t bc = thermalManager.wholeDegBed(),
                    bt = thermalManager.degTargetBed();
    const bool _new_bed_temp = _bedtemp != bc,
               _new_bed_target = _bedtarget != bt;
    if (_new_bed_temp) _bedtemp = bc;
    if (_new_bed_target) _bedtarget = bt;
  #endif
  #if HAS_FAN
    static uint8_t _fanspeed = 0;
    const bool _new_fanspeed = _fanspeed != thermalManager.fan_speed[0];
    if (_new_fanspeed) _fanspeed = thermalManager.fan_speed[0];
  #endif

  if (checkkey == Tune) {
    // Tune page temperature update
    #if HAS_HOTEND
      if (_new_hotend_target)
        Draw_Edit_Integer3(TUNE_CASE_TEMP + MROWS - index_tune, _hotendtarget);
    #endif
    #if HAS_HEATED_BED
      if (_new_bed_target)
        Draw_Edit_Integer3(TUNE_CASE_BED + MROWS - index_tune, _bedtarget);
    #endif
    #if HAS_FAN
      if (_new_fanspeed)
        Draw_Edit_Integer3(TUNE_CASE_FAN + MROWS - index_tune, _fanspeed);
    #endif
  }
  else if (checkkey == TemperatureID) {
    // Temperature page temperature update
    #if HAS_HOTEND
      if (_new_hotend_target) Draw_Edit_Integer3(TEMP_CASE_TEMP, _hotendtarget);
    #endif
    #if HAS_HEATED_BED
      if (_new_bed_target) Draw_Edit_Integer3(TEMP_CASE_BED, _bedtarget);
    #endif
    #if HAS_FAN
      if (_new_fanspeed) Draw_Edit_Integer3(TEMP_CASE_FAN, _fanspeed);
    #endif
  }

  // Bottom temperature update

  #if HAS_HOTEND
    if (_new_hotend_temp)
      Draw_Stat_Int(28, 384, _hotendtemp);
    if (_new_hotend_target)
      Draw_Stat_Int(25 + 4 * STAT_CHR_W + 6, 384, _hotendtarget);

    static int16_t _flow = planner.flow_percentage[0];
    if (_flow != planner.flow_percentage[0]) {
      _flow = planner.flow_percentage[0];
      Draw_Stat_Int(116 + 2 * STAT_CHR_W, 417, _flow);
    }
  #endif

  #if HAS_HEATED_BED
    if (_new_bed_temp)
      Draw_Stat_Int(28, 417, _bedtemp);
    if (_new_bed_target)
      Draw_Stat_Int(25 + 4 * STAT_CHR_W + 6, 417, _bedtarget);
  #endif

  static int16_t _feedrate = 100;
  if (_feedrate != feedrate_percentage) {
    _feedrate = feedrate_percentage;
    Draw_Stat_Int(116 + 2 * STAT_CHR_W, 384, _feedrate);
  }

  #if HAS_FAN
    if (_new_fanspeed) {
      _fanspeed = thermalManager.fan_speed[0];
      Draw_Stat_Int(195 + 2 * STAT_CHR_W, 384, _fanspeed);
    }
  #endif

  static float _offset = 0;
  if (BABY_Z_VAR != _offset) {
    _offset = BABY_Z_VAR;
    if (BABY_Z_VAR < 0) {
      Draw_Stat_Float(207, 417, -_offset * 100);
      DWIN_Draw_String(true, font8x16, Color_White, Color_Bg_Black, 205, 419, F("-"));
    }
    else {
      Draw_Stat_Float(207, 417, _offset * 100);
      DWIN_Draw_String(true, font8x16, Color_White, Color_Bg_Black, 205, 419, F(" "));
    }
  }

  _draw_xyz_position(false);
}

/**
 * Read and cache the working directory.
 *
 * TODO: New code can follow the pattern of menu_media.cpp
 * and rely on Marlin caching for performance. No need to
 * cache files here.
 */

#ifndef strcasecmp_P
  #define strcasecmp_P(a, b) strcasecmp((a), (b))
#endif

void make_name_without_ext(char *dst, char *src, size_t maxlen=MENU_CHAR_LIMIT) {
  char * const name = card.longest_filename();
  size_t pos        = strlen(name); // index of ending nul

  // For files, remove the extension
  // which may be .gcode, .gco, or .g
  if (!card.flag.filenameIsDir)
    while (pos && src[pos] != '.') pos--; // find last '.' (stop at 0)

  size_t len = pos;   // nul or '.'
  if (len > maxlen) { // Keep the name short
    pos        = len = maxlen; // move nul down
    dst[--pos] = '.'; // insert dots
    dst[--pos] = '.';
    dst[--pos] = '.';
  }

  dst[len] = '\0';    // end it

  // Copy down to 0
  while (pos--) dst[pos] = src[pos];
}

void HMI_SDCardInit() { card.cdroot(); }

void MarlinUI::refresh() { /* Nothing to see here */ }

#if HAS_LCD_BRIGHTNESS
  void MarlinUI::_set_brightness() { DWIN_LCD_Brightness(backlight ? brightness : 0); }
#endif

#if ENABLED(SCROLL_LONG_FILENAMES)

  char shift_name[LONG_FILENAME_LENGTH + 1];
  int8_t shift_amt; // = 0
  millis_t shift_ms; // = 0

  // Init the shift name based on the highlighted item
  void Init_Shift_Name() {
    const bool is_subdir = !card.flag.workDirIsRoot;
    const int8_t filenum = select_file.now - 1 - is_subdir; // Skip "Back" and ".."
    const uint16_t fileCnt = card.get_num_Files();
    if (WITHIN(filenum, 0, fileCnt - 1)) {
      card.getfilename_sorted(SD_ORDER(filenum, fileCnt));
      char * const name = card.longest_filename();
      make_name_without_ext(shift_name, name, 100);
    }
  }

  void Init_SDItem_Shift() {
    shift_amt = 0;
    shift_ms  = select_file.now != CASE_BACK && strlen(shift_name) > MENU_CHAR_LIMIT
           ? millis() + 750UL : 0;
  }

#endif

/**
 * Display an SD item, adding a CDUP for subfolders.
 */
void Draw_SDItem(const uint16_t item, int16_t row=-1) {
  if (row < 0) row = item + 1 + MROWS - index_file;
  const bool is_subdir = !card.flag.workDirIsRoot;
  if (is_subdir && item == 0) {
    Draw_Menu_Line(row, ICON_Folder, F(".."));
    return;
  }

  card.getfilename_sorted(SD_ORDER(item - is_subdir, card.get_num_Files()));
  char * const name = card.longest_filename();

  #if ENABLED(SCROLL_LONG_FILENAMES)
    // Init the current selected name
    // This is used during scroll drawing
    if (item == select_file.now - 1) {
      make_name_without_ext(shift_name, name, 100);
      Init_SDItem_Shift();
    }
  #endif

  // Draw the file/folder with name aligned left
  char str[strlen(name) + 1];
  make_name_without_ext(str, name);
  Draw_Menu_Line(row, card.flag.filenameIsDir ? ICON_Folder : ICON_File, str);
}

#if ENABLED(SCROLL_LONG_FILENAMES)

  void Draw_SDItem_Shifted(uint8_t &shift) {
    // Limit to the number of chars past the cutoff
    const size_t len = strlen(shift_name);
    NOMORE(shift, _MAX(len - MENU_CHAR_LIMIT, 0U));

    // Shorten to the available space
    const size_t lastchar = _MIN((signed)len, shift + MENU_CHAR_LIMIT);

    const char c = shift_name[lastchar];
    shift_name[lastchar] = '\0';

    const uint8_t row = select_file.now + MROWS - index_file; // skip "Back" and scroll
    Erase_Menu_Text(row);
    Draw_Menu_Line(row, 0, &shift_name[shift]);

    shift_name[lastchar] = c;
  }

#endif

// Redraw the first set of SD Files
void Redraw_SD_List() {
  select_file.reset();
  index_file = MROWS;

  Clear_Menu_Area(); // Leave title bar unchanged

  Draw_Back_First();

  if (card.isMounted()) {
    // As many files as will fit
    LOOP_L_N(i, _MIN(nr_sd_menu_items(), MROWS))
      Draw_SDItem(i, i + 1);

    TERN_(SCROLL_LONG_FILENAMES, Init_SDItem_Shift());
  }
  else {
    DWIN_Draw_Rectangle(1, Color_Bg_Red, 10, MBASE(3) - 10, DWIN_WIDTH - 10, MBASE(4));
    DWIN_Draw_String(false, font16x32, Color_Yellow, Color_Bg_Red, ((DWIN_WIDTH) - 8 * 16) / 2, MBASE(3), F("No Media"));
  }
}

bool DWIN_lcd_sd_status = false;

void SDCard_Up() {
  card.cdup();
  Redraw_SD_List();
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

void SDCard_Folder(char * const dirname) {
  card.cd(dirname);
  Redraw_SD_List();
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

//
// Watch for media mount / unmount
//
void HMI_SDCardUpdate() {
  if (HMI_flag.home_flag) return;
  if (DWIN_lcd_sd_status != card.isMounted()) {
    DWIN_lcd_sd_status = card.isMounted();
    //SERIAL_ECHOLNPGM("HMI_SDCardUpdate: ", DWIN_lcd_sd_status);
    if (DWIN_lcd_sd_status) {
      if (checkkey == SelectFile)
        Redraw_SD_List();
    }
    else {
      // clean file icon
      if (checkkey == SelectFile) {
        Redraw_SD_List();
      }
      else if (checkkey == PrintProcess || checkkey == Tune || printingIsActive()) {
        // TODO: Move card removed abort handling
        //       to CardReader::manage_media.
        card.abortFilePrintSoon();
        wait_for_heatup = wait_for_user = false;
        dwin_abort_flag = true; // Reset feedrate, return to Home
      }
    }
    DWIN_UpdateLCD();
  }
}

//
// The status area is always on-screen, except during
// full-screen modal dialogs. (TODO: Keep alive during dialogs)
//
void Draw_Status_Area(const bool with_update) {

  DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, STATUS_Y, DWIN_WIDTH, DWIN_HEIGHT - 1);

  #if HAS_HOTEND
    DWIN_ICON_Show(ICON, ICON_HotendTemp, 10, 383);
    Draw_Stat_Int(28, 384, thermalManager.wholeDegHotend(0));
    DWIN_Draw_String(false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    Draw_Stat_Int(25 + 4 * STAT_CHR_W + 6, 384, thermalManager.degTargetHotend(0));

    DWIN_ICON_Show(ICON, ICON_StepE, 112, 417);
    Draw_Stat_Int(116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    DWIN_Draw_String(false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWIN_ICON_Show(ICON, ICON_BedTemp, 10, 416);
    Draw_Stat_Int(28, 417, thermalManager.wholeDegBed());
    DWIN_Draw_String(false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    Draw_Stat_Int(25 + 4 * STAT_CHR_W + 6, 417, thermalManager.degTargetBed());
  #endif

  DWIN_ICON_Show(ICON, ICON_Speed, 113, 383);
  Draw_Stat_Int(116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  DWIN_Draw_String(false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 384, F("%"));

  #if HAS_FAN
    DWIN_ICON_Show(ICON, ICON_FanSpeed, 187, 383);
    Draw_Stat_Int(195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
  #endif

  #if HAS_ZOFFSET_ITEM
    DWIN_ICON_Show(ICON, ICON_Zoffset, 187, 416);
  #endif

  if (BABY_Z_VAR < 0) {
    Draw_Stat_Float(207, 417, -BABY_Z_VAR * 100);
    DWIN_Draw_String(true, font8x16, Color_White, Color_Bg_Black, 205, 419, F("-"));
  }
  else {
    Draw_Stat_Float(207, 417, BABY_Z_VAR * 100);
    DWIN_Draw_String(true, font8x16, Color_White, Color_Bg_Black, 205, 419, F(" "));
  }

  DWIN_Draw_Rectangle(1, Line_Color, 0, 449, DWIN_WIDTH, 451);

  DWIN_ICON_Show(ICON, ICON_MaxSpeedX,  10, 456);
  DWIN_ICON_Show(ICON, ICON_MaxSpeedY,  95, 456);
  DWIN_ICON_Show(ICON, ICON_MaxSpeedZ, 180, 456);
  _draw_xyz_position(true);

  if (with_update) {
    DWIN_UpdateLCD();
    delay(5);
  }
}

void HMI_StartFrame(const bool with_update) {
  Goto_MainMenu();
  Draw_Status_Area(with_update);
}

void Draw_Info_Menu() {
  Clear_Main_Window();

  DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(MACHINE_SIZE) * MENU_CHR_W) / 2, 122, F(MACHINE_SIZE));
  DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(SHORT_BUILD_VERSION) * MENU_CHR_W) / 2, 195, F(SHORT_BUILD_VERSION));

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(30, 17, 28, 13);                   // "Info"

    DWIN_Frame_AreaCopy(1, 197, 149, 252, 161, 108, 102);   // "Size"
    DWIN_Frame_AreaCopy(1,   1, 164,  56, 176, 108, 175);   // "Firmware Version"
    DWIN_Frame_AreaCopy(1,  58, 164, 113, 176, 105, 248);   // "Contact Details"
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_INFO_SCREEN));
    #else
      DWIN_Frame_TitleCopy(192, 15, 23, 12);                // "Info"
    #endif

    DWIN_Frame_AreaCopy(1, 120, 150, 146, 161, 124, 102);   // "Size"
    DWIN_Frame_AreaCopy(1, 146, 151, 254, 161,  82, 175);   // "Firmware Version"
    DWIN_Frame_AreaCopy(1,   1, 164,  96, 175,  89, 248);   // "Contact details"
  }
  DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(CORP_WEBSITE) * MENU_CHR_W) / 2, 268, F(CORP_WEBSITE));

  Draw_Back_First();
  LOOP_L_N(i, 3) {
    DWIN_ICON_Show(ICON, ICON_PrintSize + i, 26, 99 + i * 73);
    DWIN_Draw_Line(Line_Color, 16, MBASE(2) + i * 73, 256, 156 + i * 73);
  }
}

void Draw_Print_File_Menu() {
  Clear_Title_Bar();

  if (HMI_IsChinese())
    DWIN_Frame_TitleCopy(0, 31, 56, 14);  // "Print file"
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MEDIA_MENU));
    #else
      DWIN_Frame_TitleCopy(52, 31, 86, 11); // "Print file"
    #endif
  }

  Redraw_SD_List();
}

// Main Process
void HMI_MainMenu() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_page.inc(4)) {
      switch (select_page.now) {
        case PAGE_PRINT: ICON_Print(); break;
        case PAGE_PREPARE: ICON_Print(); ICON_Prepare(); break;
        case PAGE_CONTROL: ICON_Prepare(); ICON_Control(); break;
        case PAGE_INFO_LEVELING: ICON_Control(); TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_page.dec()) {
      switch (select_page.now) {
        case PAGE_PRINT: ICON_Print(); ICON_Prepare(); break;
        case PAGE_PREPARE: ICON_Prepare(); ICON_Control(); break;
        case PAGE_CONTROL: ICON_Control(); TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(); break;
        case PAGE_INFO_LEVELING: TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_page.now) {
      case PAGE_PRINT:
        checkkey = SelectFile;
        Draw_Print_File_Menu();
        break;

      case PAGE_PREPARE:
        checkkey = Prepare;
        select_prepare.reset();
        index_prepare = MROWS;
        Draw_Prepare_Menu();
        break;

      case PAGE_CONTROL:
        checkkey = Control;
        select_control.reset();
        index_control = MROWS;
        Draw_Control_Menu();
        break;

      case PAGE_INFO_LEVELING:
        #if HAS_ONESTEP_LEVELING
          checkkey = Leveling;
          HMI_Leveling();
        #else
          checkkey = Info;
          Draw_Info_Menu();
        #endif
        break;
    }
  }
  DWIN_UpdateLCD();
}

// Select (and Print) File
void HMI_SelectFile() {
  EncoderState encoder_diffState = get_encoder_state();

  const uint16_t hasUpDir = !card.flag.workDirIsRoot;

  if (encoder_diffState == ENCODER_DIFF_NO) {
    #if ENABLED(SCROLL_LONG_FILENAMES)
      if (shift_ms && select_file.now >= 1 + hasUpDir) {
        // Scroll selected filename every second
        const millis_t ms = millis();
        if (ELAPSED(ms, shift_ms)) {
          const bool was_reset = shift_amt < 0;
          shift_ms = ms + 375UL + was_reset * 250UL;  // ms per character
          uint8_t shift_new = shift_amt + 1;           // Try to shift by...
          Draw_SDItem_Shifted(shift_new);             // Draw the item
          if (!was_reset && shift_new == 0)           // Was it limited to 0?
            shift_ms = 0;                             // No scrolling needed
          else if (shift_new == shift_amt)            // Scroll reached the end
            shift_new = -1;                           // Reset
          shift_amt = shift_new;                      // Set new scroll
        }
      }
    #endif
    return;
  }

  // First pause is long. Easy.
  // On reset, long pause must be after 0.

  const uint16_t fullCnt = nr_sd_menu_items();

  if (encoder_diffState == ENCODER_DIFF_CW && fullCnt) {
    if (select_file.inc(1 + fullCnt)) {
      const uint8_t itemnum = select_file.now - 1;              // -1 for "Back"
      if (TERN0(SCROLL_LONG_FILENAMES, shift_ms)) {             // If line was shifted
        Erase_Menu_Text(itemnum + MROWS - index_file);          // Erase and
        Draw_SDItem(itemnum - 1);                               // redraw
      }
      if (select_file.now > MROWS && select_file.now > index_file) { // Cursor past the bottom
        index_file = select_file.now;                           // New bottom line
        Scroll_Menu(DWIN_SCROLL_UP);
        Draw_SDItem(itemnum, MROWS);                            // Draw and init the shift name
      }
      else {
        Move_Highlight(1, select_file.now + MROWS - index_file); // Just move highlight
        TERN_(SCROLL_LONG_FILENAMES, Init_Shift_Name());         // ...and init the shift name
      }
      TERN_(SCROLL_LONG_FILENAMES, Init_SDItem_Shift());
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && fullCnt) {
    if (select_file.dec()) {
      const uint8_t itemnum = select_file.now - 1;              // -1 for "Back"
      if (TERN0(SCROLL_LONG_FILENAMES, shift_ms)) {             // If line was shifted
        Erase_Menu_Text(select_file.now + 1 + MROWS - index_file); // Erase and
        Draw_SDItem(itemnum + 1);                               // redraw
      }
      if (select_file.now < index_file - MROWS) {               // Cursor past the top
        index_file--;                                           // New bottom line
        Scroll_Menu(DWIN_SCROLL_DOWN);
        if (index_file == MROWS) {
          Draw_Back_First();
          TERN_(SCROLL_LONG_FILENAMES, shift_ms = 0);
        }
        else
          Draw_SDItem(itemnum, 0);                              // Draw the item (and init shift name)
      }
      else {
        Move_Highlight(-1, select_file.now + MROWS - index_file); // Just move highlight
        TERN_(SCROLL_LONG_FILENAMES, Init_Shift_Name());        // ...and init the shift name
      }
      TERN_(SCROLL_LONG_FILENAMES, Init_SDItem_Shift());        // Reset left. Init timer.
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (select_file.now == CASE_BACK) { // Back
      select_page.set(0);
      Goto_MainMenu();
    }
    else if (hasUpDir && select_file.now == 1) { // CD-Up
      SDCard_Up();
      goto HMI_SelectFileExit;
    }
    else {
      const uint16_t filenum = select_file.now - 1 - hasUpDir;
      card.getfilename_sorted(SD_ORDER(filenum, card.get_num_Files()));

      // Enter that folder!
      if (card.flag.filenameIsDir) {
        SDCard_Folder(card.filename);
        goto HMI_SelectFileExit;
      }

      // Reset highlight for next entry
      select_print.reset();
      select_file.reset();

      // Start choice and print SD file
      HMI_flag.heat_flag = true;
      HMI_flag.print_finish = false;
      HMI_ValueStruct.show_mode = 0;

      card.openAndPrintFile(card.filename);

      #if HAS_FAN
        // All fans on for Ender 3 v2 ?
        // The slicer should manage this for us.
        //for (uint8_t i = 0; i < FAN_COUNT; i++)
        //  thermalManager.fan_speed[i] = 255;
      #endif

      Goto_PrintProcess();
    }
  }

  HMI_SelectFileExit:
    DWIN_UpdateLCD();
}

// Printing
void HMI_Printing() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (HMI_flag.done_confirm_flag) {
    if (encoder_diffState == ENCODER_DIFF_ENTER) {
      HMI_flag.done_confirm_flag = false;
      dwin_abort_flag = true; // Reset feedrate, return to Home
    }
    return;
  }

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_print.inc(3)) {
      switch (select_print.now) {
        case PRINT_SETUP: ICON_Tune(); break;
        case PRINT_PAUSE_RESUME: ICON_Tune(); ICON_ResumeOrPause(); break;
        case PRINT_STOP: ICON_ResumeOrPause(); ICON_Stop(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_print.dec()) {
      switch (select_print.now) {
        case PRINT_SETUP: ICON_Tune(); ICON_ResumeOrPause(); break;
        case PRINT_PAUSE_RESUME: ICON_ResumeOrPause(); ICON_Stop(); break;
        case PRINT_STOP: ICON_Stop(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_print.now) {
      case PRINT_SETUP:
        checkkey = Tune;
        HMI_ValueStruct.show_mode = 0;
        select_tune.reset();
        index_tune = MROWS;
        Draw_Tune_Menu();
        break;

      case PRINT_PAUSE_RESUME:
        if (HMI_flag.pause_flag) {
          ICON_Pause();

          char cmd[40];
          cmd[0] = '\0';

          #if BOTH(HAS_HEATED_BED, PAUSE_HEAT)
            if (resume_bed_temp) sprintf_P(cmd, PSTR("M190 S%i\n"), resume_bed_temp);
          #endif
          #if BOTH(HAS_HOTEND, PAUSE_HEAT)
            if (resume_hotend_temp) sprintf_P(&cmd[strlen(cmd)], PSTR("M109 S%i\n"), resume_hotend_temp);
          #endif

          strcat_P(cmd, M24_STR);
          queue.inject(cmd);
        }
        else {
          HMI_flag.select_flag = true;
          checkkey = Print_window;
          Popup_window_PauseOrStop();
        }
        break;

      case PRINT_STOP:
        HMI_flag.select_flag = true;
        checkkey = Print_window;
        Popup_window_PauseOrStop();
        break;

      default: break;
    }
  }
  DWIN_UpdateLCD();
}

// Pause and Stop window
void HMI_PauseOrStop() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_CW)
    Draw_Select_Highlight(false);
  else if (encoder_diffState == ENCODER_DIFF_CCW)
    Draw_Select_Highlight(true);
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (select_print.now == PRINT_PAUSE_RESUME) {
      if (HMI_flag.select_flag) {
        HMI_flag.pause_action = true;
        queue.inject(F("M25"));
      }
      Goto_PrintProcess();
    }
    else if (select_print.now == PRINT_STOP) {
      if (HMI_flag.select_flag) {
        checkkey = Back_Main;
        wait_for_heatup = wait_for_user = false;       // Stop waiting for heating/user
        card.abortFilePrintSoon();                     // Let the main loop handle SD abort
        dwin_abort_flag = true;                        // Reset feedrate, return to Home
        #ifdef ACTION_ON_CANCEL
          hostui.cancel();
        #endif
        Popup_Window_Home(true);
        if (HMI_flag.home_flag) planner.synchronize(); // Wait for planner moves to finish!
      }
      else
        Goto_PrintProcess(); // cancel stop
    }
  }
  DWIN_UpdateLCD();
}

void Draw_Move_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(192, 1, 42, 14); // "Move"
    Item_AreaCopy(58, 118, 106, 132, 1);
    Item_AreaCopy(109, 118, 157, 132, 2);
    Item_AreaCopy(160, 118, 209, 132, 3);
    TERN_(HAS_HOTEND, Item_AreaCopy(212, 118, 253, 131, 4));
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MOVE_AXIS));
    #else
      DWIN_Frame_TitleCopy(231, 2, 35, 11);   // "Move"
    #endif

    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(1, GET_TEXT_F(MSG_MOVE_X));
      DWIN_Draw_Label(2, GET_TEXT_F(MSG_MOVE_Y));
      DWIN_Draw_Label(3, GET_TEXT_F(MSG_MOVE_Z));
      TERN_(HAS_HOTEND, DWIN_Draw_Label(4, GET_TEXT_F(MSG_MOVE_E)));
    #else
      say_move_en(1); say_x_en(38, 1); // "Move X"
      say_move_en(2); say_y_en(38, 2); // "Move Y"
      say_move_en(3); say_z_en(38, 3); // "Move Z"
      TERN_(HAS_HOTEND, (say_move_en(4), Item_AreaCopy(99, 194, 151, 204, 4, 38))); // "Move Extruder"
    #endif
  }

  Draw_Back_First(select_axis.now == CASE_BACK);
  if (select_axis.now != CASE_BACK) Draw_Menu_Cursor(select_axis.now);

  // Draw separators and icons
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_MoveX + i);
}

void Item_Adv_HomeOffsets(const uint8_t row) {
  if (false && HMI_IsChinese()) {
    // TODO: Chinese "Set Home Offsets"
  }
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, GET_TEXT_F(MSG_SET_HOME_OFFSETS));
    #else
      Item_AreaCopy(1,  76, 102,  87, row); // "Set Home Offsets"
    #endif
  }
  Draw_Menu_Line(row, ICON_HomeOffset);
  Draw_More_Icon(row);
}

#if HAS_ONESTEP_LEVELING

  void Item_Adv_ProbeOffsets(const uint8_t row) {
    if (false && HMI_IsChinese()) {
      // TODO: Chinese "Probe Offsets"
    }
    else {
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(row, GET_TEXT_F(MSG_ZPROBE_OFFSETS));
      #else
        say_probe_offs_en(row);
      #endif
    }
    Draw_Menu_Line(row, ICON_ProbeOffset);
    Draw_More_Icon(row);
  }

#endif

void Item_Adv_HotendPID(const uint8_t row) {
  if (false && HMI_IsChinese()) {
    // TODO: Chinese "Hotend PID"
  }
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, F("Hotend PID"));
    #else
      Item_AreaCopy(96, 104, 167, 114, row); // "Hotend PID"
    #endif
  }
  Draw_Menu_Line(row, ICON_PIDNozzle);
}

void Item_Adv_BedPID(const uint8_t row) {
  if (false && HMI_IsChinese()) {
    // TODO: Chinese "Bed PID"
  }
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(row, F("Bed PID"));
    #else
      Item_AreaCopy(241, 104, 263, 115, row);     // "Bed"
      Item_AreaCopy(145, 104, 167, 114, row, 27); // "PID"
    #endif
  }
  Draw_Menu_Line(row, ICON_PIDbed);
}

#if ENABLED(POWER_LOSS_RECOVERY)

  void Item_Adv_PLR(const uint8_t row) {
    if (false && HMI_IsChinese()) {
      // TODO: Chinese "Power-loss Recovery"
    }
    else {
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(row, GET_TEXT_F(MSG_ZPROBE_OFFSETS));
      #else
        Item_AreaCopy(1, 208, 137, 221, row);  // "Power-loss Recovery"
      #endif
    }
    Draw_Menu_Line(row, ICON_Motion);
    Draw_Checkbox_Line(row, recovery.enabled);
  }

#endif

void Draw_AdvancedSettings_Menu() {
  Clear_Main_Window();

  #if ADVSET_CASE_TOTAL >= TROWS
    const int16_t scroll = MROWS - index_advset; // Scrolled-up lines
  #else
    constexpr int16_t scroll = 0;
  #endif
  #define ASCROL(L) (scroll + (L))
  #define AVISI(L) VISI(ADVSET_CASE_TOTAL, L, ASCROL(L))

  if (false && HMI_IsChinese()) {
    // TODO: Chinese "Advanced Settings"
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_ADVANCED_SETTINGS));
    #else
      DWIN_Frame_TitleCopy(93, 401, 126, 15); // "Advanced Settings"
    #endif
  }

  if (AVISI(0)) Draw_Back_First(select_advset.now == CASE_BACK);
  if (AVISI(ADVSET_CASE_HOMEOFF)) Item_Adv_HomeOffsets(ASCROL(ADVSET_CASE_HOMEOFF));      // Set Home Offsets >
  #if HAS_ONESTEP_LEVELING
    if (AVISI(ADVSET_CASE_PROBEOFF)) Item_Adv_ProbeOffsets(ASCROL(ADVSET_CASE_PROBEOFF)); // Probe Offsets >
  #endif
  if (AVISI(ADVSET_CASE_HEPID)) Item_Adv_HotendPID(ASCROL(ADVSET_CASE_HEPID));            // Nozzle PID
  if (AVISI(ADVSET_CASE_BEDPID)) Item_Adv_BedPID(ASCROL(ADVSET_CASE_BEDPID));             // Bed PID
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (AVISI(ADVSET_CASE_PWRLOSSR)) Item_Adv_PLR(ASCROL(ADVSET_CASE_PWRLOSSR));          // Power-loss recovery
  #endif
  if (select_advset.now != CASE_BACK) Draw_Menu_Cursor(ASCROL(select_advset.now));
}

void Item_HomeOffs_X(const uint8_t row) {
  if (false && HMI_IsChinese()) {
    // TODO: Chinese "Home Offset X"
  }
  else {
    #ifdef USE_STRING_TITLES
      Draw_Menu_Line(row, ICON_HomeOffsetX, GET_TEXT_F(MSG_HOME_OFFSET_X));
    #else
      say_home_offs_en(row); say_x_en(75, row);   // "Home Offset X"
    #endif
  }
  Draw_Menu_Line(row, ICON_HomeOffset);
  Draw_Edit_Signed_Float3(row, HMI_ValueStruct.Home_OffX_scaled);
}

void Item_HomeOffs_Y(const uint8_t row) {
  if (false && HMI_IsChinese()) {
    // TODO: Chinese "Home Offset Y"
  }
  else {
    #ifdef USE_STRING_TITLES
      Draw_Menu_Line(row, ICON_HomeOffsetY, GET_TEXT_F(MSG_HOME_OFFSET_Y));
    #else
      say_home_offs_en(row); say_y_en(75, row);   // "Home Offset X"
    #endif
  }
  Draw_Menu_Line(row, ICON_HomeOffset);
  Draw_Edit_Signed_Float3(row, HMI_ValueStruct.Home_OffY_scaled);
}

void Item_HomeOffs_Z(const uint8_t row) {
  if (false && HMI_IsChinese()) {
    // TODO: Chinese "Home Offset Z"
  }
  else {
    #ifdef USE_STRING_TITLES
      Draw_Menu_Line(row, ICON_HomeOffsetZ, GET_TEXT_F(MSG_HOME_OFFSET_Z));
    #else
      say_home_offs_en(row); say_z_en(75, row);   // "Home Offset Z"
    #endif
  }
  Draw_Menu_Line(row, ICON_HomeOffset);
  Draw_Edit_Signed_Float3(row, HMI_ValueStruct.Home_OffZ_scaled);
}

void Draw_HomeOff_Menu() {
  Clear_Main_Window();
  if (false && HMI_IsChinese()) {
    // TODO: Chinese "Home Offsets"
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_SET_HOME_OFFSETS));
    #else
      DWIN_Frame_TitleCopy(1, 401, 91, 12);         // "Home Offsets"
    #endif
  }
  Draw_Back_First(select_item.now == CASE_BACK);
  Item_HomeOffs_X(1);                               // "Home Offset X"
  Item_HomeOffs_Y(2);                               // "Home Offset Y"
  Item_HomeOffs_Z(3);                               // "Home Offset Z"
  if (select_item.now != CASE_BACK) Draw_Menu_Cursor(select_item.now);
}

#if HAS_ONESTEP_LEVELING

  void Draw_ProbeOff_Menu() {
    Clear_Main_Window();
    Draw_Back_First(select_item.now == CASE_BACK);
    if (false && HMI_IsChinese()) {
      // TODO: Chinese "Probe Offsets"
    }
    else {
      #ifdef USE_STRING_HEADINGS
        Draw_Title(GET_TEXT_F(MSG_ZPROBE_OFFSETS));
      #else
        DWIN_Frame_TitleCopy(124, 431, 91, 12);                             // "Probe Offsets"
      #endif
      #ifdef USE_STRING_TITLES
        Draw_Menu_Line(1, ICON_ProbeOffsetX, GET_TEXT_F(MSG_ZPROBE_XOFFSET));  // Probe X Offset
        Draw_Menu_Line(2, ICON_ProbeOffsetY, GET_TEXT_F(MSG_ZPROBE_YOFFSET));  // Probe Y Offset
      #else
        say_probe_offs_en(1); say_x_en(75, 1);  // "Probe Offset X"
        say_probe_offs_en(2); say_y_en(75, 2);  // "Probe Offset Y"
      #endif
    }

    Draw_Edit_Signed_Float3(1, HMI_ValueStruct.Probe_OffX_scaled);
    Draw_Edit_Signed_Float3(2, HMI_ValueStruct.Probe_OffY_scaled);

    if (select_item.now != CASE_BACK) Draw_Menu_Cursor(select_item.now);
  }

#endif

#include "../../../libs/buzzer.h"

void HMI_AudioFeedback(const bool success=true) {
  #if HAS_BUZZER
    if (success) {
      buzzer.tone(100, 659);
      buzzer.tone(10, 0);
      buzzer.tone(100, 698);
    }
    else
      buzzer.tone(40, 440);
  #endif
}

// Prepare
void HMI_Prepare() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_prepare.inc(1 + PREPARE_CASE_TOTAL)) {
      if (select_prepare.now > MROWS && select_prepare.now > index_prepare) {
        index_prepare = select_prepare.now;

        // Scroll up and draw a blank bottom line
        Scroll_Menu(DWIN_SCROLL_UP);
        Draw_Menu_Icon(MROWS, ICON_Axis + select_prepare.now - 1);

        // Draw "More" icon for sub-menus
        if (index_prepare < 7) Draw_More_Icon(MROWS - index_prepare + 1);

        #if HAS_HOTEND
          if (index_prepare == PREPARE_CASE_ABS) Item_Prepare_ABS(MROWS);
        #endif
        #if HAS_PREHEAT
          if (index_prepare == PREPARE_CASE_COOL) Item_Prepare_Cool(MROWS);
        #endif
        if (index_prepare == PREPARE_CASE_LANG) Item_Prepare_Lang(MROWS);
      }
      else {
        Move_Highlight(1, select_prepare.now + MROWS - index_prepare);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_prepare.dec()) {
      if (select_prepare.now < index_prepare - MROWS) {
        index_prepare--;
        Scroll_Menu(DWIN_SCROLL_DOWN);

        if (index_prepare == MROWS)
          Draw_Back_First();
        else
          Draw_Menu_Line(0, ICON_Axis + select_prepare.now - 1);

        if (index_prepare < 7) Draw_More_Icon(MROWS - index_prepare + 1);

             if (index_prepare == 6) Item_Prepare_Move(0);
        else if (index_prepare == 7) Item_Prepare_Disable(0);
        else if (index_prepare == 8) Item_Prepare_Home(0);
      }
      else {
        Move_Highlight(-1, select_prepare.now + MROWS - index_prepare);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_prepare.now) {
      case CASE_BACK:
        select_page.set(1);
        Goto_MainMenu();
        break;
      case PREPARE_CASE_MOVE:
        checkkey = AxisMove;
        select_axis.reset();
        Draw_Move_Menu();

        Draw_Edit_Float3(1, current_position.x * MINUNITMULT);
        Draw_Edit_Float3(2, current_position.y * MINUNITMULT);
        Draw_Edit_Float3(3, current_position.z * MINUNITMULT);
        #if HAS_HOTEND
          HMI_ValueStruct.Move_E_scaled = current_position.e * MINUNITMULT;
          Draw_Edit_Signed_Float3(4, HMI_ValueStruct.Move_E_scaled);
        #endif
        break;

      case PREPARE_CASE_DISA: queue.inject(F("M84")); break;

      case PREPARE_CASE_HOME: // Homing
        checkkey = Last_Prepare;
        index_prepare = MROWS;
        queue.inject_P(G28_STR); // G28 will set home_flag
        Popup_Window_Home();
        break;
      #if HAS_ZOFFSET_ITEM
        case PREPARE_CASE_ZOFF:
          #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
            checkkey = Homeoffset;
            HMI_ValueStruct.show_mode = -4;
            HMI_ValueStruct.offset_value = BABY_Z_VAR * 100;
            Draw_Edit_Signed_Float2(PREPARE_CASE_ZOFF + MROWS - index_prepare, HMI_ValueStruct.offset_value, true);
            EncoderRate.enabled = true;
          #else
            // Apply workspace offset, making the current position 0,0,0
            queue.inject(F("G92 X0 Y0 Z0"));
            HMI_AudioFeedback();
          #endif
          break;
      #endif
      #if HAS_PREHEAT
        case PREPARE_CASE_PLA:
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0));
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(ui.material_preset[0].bed_temp));
          TERN_(HAS_FAN, thermalManager.set_fan_speed(0, ui.material_preset[0].fan_speed));
          break;
        case PREPARE_CASE_ABS:
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(ui.material_preset[1].hotend_temp, 0));
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(ui.material_preset[1].bed_temp));
          TERN_(HAS_FAN, thermalManager.set_fan_speed(0, ui.material_preset[1].fan_speed));
          break;
        case PREPARE_CASE_COOL:
          TERN_(HAS_FAN, thermalManager.zero_fan_speeds());
          #if HAS_HOTEND || HAS_HEATED_BED
            thermalManager.disable_all_heaters();
          #endif
          break;
      #endif
      case PREPARE_CASE_LANG:
        HMI_ToggleLanguage();
        Draw_Prepare_Menu();
        break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

void Draw_Temperature_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(236, 2, 28, 12); // "Temperature"
    #if HAS_HOTEND
      Item_AreaCopy(1, 134, 56, 146, TEMP_CASE_TEMP);
    #endif
    #if HAS_HEATED_BED
      Item_AreaCopy(58, 134, 113, 146, TEMP_CASE_BED);
    #endif
    #if HAS_FAN
      Item_AreaCopy(115, 134, 170, 146, TEMP_CASE_FAN);
    #endif
    #if HAS_HOTEND
      Item_AreaCopy(100, 89, 178, 101, TEMP_CASE_PLA);
      Item_AreaCopy(180, 89, 260, 100, TEMP_CASE_ABS);
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_TEMPERATURE));
    #else
      DWIN_Frame_TitleCopy(56, 15, 85, 14);   // "Temperature"
    #endif
    #ifdef USE_STRING_TITLES
      #if HAS_HOTEND
        DWIN_Draw_Label(TEMP_CASE_TEMP, GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND));
      #endif
      #if HAS_HEATED_BED
        DWIN_Draw_Label(TEMP_CASE_BED, GET_TEXT_F(MSG_UBL_SET_TEMP_BED));
      #endif
      #if HAS_FAN
        DWIN_Draw_Label(TEMP_CASE_FAN, GET_TEXT_F(MSG_FAN_SPEED));
      #endif
      #if HAS_HOTEND
        DWIN_Draw_Label(TEMP_CASE_PLA, F(PREHEAT_1_LABEL " Preheat Settings"));
        DWIN_Draw_Label(TEMP_CASE_ABS, F(PREHEAT_2_LABEL " Preheat Settings"));
      #endif
    #else
      #if HAS_HOTEND
        Item_AreaCopy(197, 104, 238, 114, TEMP_CASE_TEMP);    // "Nozzle"
        Item_AreaCopy(1,  89,  83, 101, TEMP_CASE_TEMP, 44);  // "Temperature"
      #endif
      #if HAS_HEATED_BED
        Item_AreaCopy(240, 104, 264, 114, TEMP_CASE_BED);     // "Bed"
        Item_AreaCopy(1,  89,  83, 101, TEMP_CASE_BED, 27);   // "Temperature"
      #endif
      #if HAS_FAN
        Item_AreaCopy(  1, 119,  61, 132, TEMP_CASE_FAN);     // "Fan speed"
      #endif
      #if HAS_HOTEND
        Item_AreaCopy(107,  76, 156,  86, TEMP_CASE_PLA);     // "Preheat"
        say_pla_en(52, TEMP_CASE_PLA);                        // "PLA"
        Item_AreaCopy(150, 135, 202, 148, TEMP_CASE_PLA, 79); // "Settings"

        Item_AreaCopy(107,  76, 156,  86, TEMP_CASE_ABS);     // "Preheat"
        say_abs_en(52, TEMP_CASE_ABS);                        // "ABS"
        Item_AreaCopy(150, 135, 202, 148, TEMP_CASE_ABS, 81); // "Settings"
      #endif
    #endif
  }

  Draw_Back_First(select_temp.now == CASE_BACK);
  if (select_temp.now != CASE_BACK) Draw_Menu_Cursor(select_temp.now);

  // Draw icons and lines
  uint8_t i = 0;
  #define _TMENU_ICON(N) Draw_Menu_Line(++i, ICON_SetEndTemp + (N) - 1)
  #if HAS_HOTEND
    _TMENU_ICON(TEMP_CASE_TEMP);
    Draw_Edit_Integer3(i, thermalManager.degTargetHotend(0));
  #endif
  #if HAS_HEATED_BED
    _TMENU_ICON(TEMP_CASE_BED);
    Draw_Edit_Integer3(i, thermalManager.degTargetBed());
  #endif
  #if HAS_FAN
    _TMENU_ICON(TEMP_CASE_FAN);
    Draw_Edit_Integer3(i, thermalManager.fan_speed[0]);
  #endif
  #if HAS_HOTEND
    // PLA/ABS items have submenus
    _TMENU_ICON(TEMP_CASE_PLA);
    Draw_More_Icon(i);
    _TMENU_ICON(TEMP_CASE_ABS);
    Draw_More_Icon(i);
  #endif
}

// Control
void HMI_Control() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_control.inc(1 + CONTROL_CASE_TOTAL)) {
      if (select_control.now > MROWS && select_control.now > index_control) {
        index_control = select_control.now;

        // Scroll up and draw a blank bottom line
        Scroll_Menu(DWIN_SCROLL_UP);

        switch (index_control) {  // Last menu items
          case CONTROL_CASE_ADVSET: Item_Control_Advanced(MROWS); break;
          case CONTROL_CASE_INFO:   Item_Control_Info(MROWS);     break;
          default: break;
        }

      }
      else
        Move_Highlight(1, select_control.now + MROWS - index_control);
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_control.dec()) {
      if (select_control.now < index_control - MROWS) {
        index_control--;
        Scroll_Menu(DWIN_SCROLL_DOWN);
        switch (index_control) {  // First menu items
          case MROWS:     Draw_Back_First();      break;
          case MROWS + 1: Item_Control_Temp(0);   break;
          case MROWS + 2: Item_Control_Motion(0); break;
          default: break;
        }
      }
      else
        Move_Highlight(-1, select_control.now + MROWS - index_control);
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_control.now) {
      case CASE_BACK:
        select_page.set(2);
        Goto_MainMenu();
        break;
      case CONTROL_CASE_TEMP:
        checkkey = TemperatureID;
        HMI_ValueStruct.show_mode = -1;
        select_temp.reset();
        Draw_Temperature_Menu();
        break;
      case CONTROL_CASE_MOVE:
        checkkey = Motion;
        select_motion.reset();
        Draw_Motion_Menu();
        break;
      #if ENABLED(EEPROM_SETTINGS)
        case CONTROL_CASE_SAVE: {
          const bool success = settings.save();
          HMI_AudioFeedback(success);
        } break;
        case CONTROL_CASE_LOAD: {
          const bool success = settings.load();
          HMI_AudioFeedback(success);
        } break;
        case CONTROL_CASE_RESET:
          settings.reset();
          HMI_AudioFeedback();
          break;
      #endif
      case CONTROL_CASE_ADVSET:
        checkkey = AdvSet;
        select_advset.reset();
        Draw_AdvancedSettings_Menu();
        break;
      case CONTROL_CASE_INFO:
        checkkey = Info;
        Draw_Info_Menu();
        break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

#if HAS_ONESTEP_LEVELING
  // Leveling
  void HMI_Leveling() {
    Popup_Window_Leveling();
    DWIN_UpdateLCD();
    queue.inject(F("G28O\nG29"));
  }
#endif

// Axis Move
void HMI_AxisMove() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  #if ENABLED(PREVENT_COLD_EXTRUSION)
    // popup window resume
    if (HMI_flag.cold_flag) {
      if (encoder_diffState == ENCODER_DIFF_ENTER) {
        HMI_flag.cold_flag = false;
        HMI_ValueStruct.Move_E_scaled = current_position.e * MINUNITMULT;
        Draw_Move_Menu();
        Draw_Edit_Float3(1, HMI_ValueStruct.Move_X_scaled);
        Draw_Edit_Float3(2, HMI_ValueStruct.Move_Y_scaled);
        Draw_Edit_Float3(3, HMI_ValueStruct.Move_Z_scaled);
        Draw_Edit_Signed_Float3(4, 0);
        DWIN_UpdateLCD();
      }
      return;
    }
  #endif

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_axis.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_axis.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_axis.dec()) Move_Highlight(-1, select_axis.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_axis.now) {
      case CASE_BACK:
        checkkey = Prepare;
        select_prepare.set(1);
        index_prepare = MROWS;
        Draw_Prepare_Menu();
        break;
      case 1: // X axis move
        checkkey = Move_X;
        HMI_ValueStruct.Move_X_scaled = current_position.x * MINUNITMULT;
        Draw_Edit_Float3(1, HMI_ValueStruct.Move_X_scaled, true);
        EncoderRate.enabled = true;
        break;
      case 2: // Y axis move
        checkkey = Move_Y;
        HMI_ValueStruct.Move_Y_scaled = current_position.y * MINUNITMULT;
        Draw_Edit_Float3(2, HMI_ValueStruct.Move_Y_scaled, true);
        EncoderRate.enabled = true;
        break;
      case 3: // Z axis move
        checkkey = Move_Z;
        HMI_ValueStruct.Move_Z_scaled = current_position.z * MINUNITMULT;
        Draw_Edit_Float3(3, HMI_ValueStruct.Move_Z_scaled, true);
        EncoderRate.enabled = true;
        break;
        #if HAS_HOTEND
          case 4: // Extruder
            #if ENABLED(PREVENT_COLD_EXTRUSION)
              if (thermalManager.tooColdToExtrude(0)) {
                HMI_flag.cold_flag = true;
                Popup_Window_ETempTooLow();
                DWIN_UpdateLCD();
                return;
              }
            #endif
            checkkey = Extruder;
            HMI_ValueStruct.Move_E_scaled = current_position.e * MINUNITMULT;
            Draw_Edit_Signed_Float3(4, HMI_ValueStruct.Move_E_scaled, true);
            EncoderRate.enabled = true;
            break;
        #endif
    }
  }
  DWIN_UpdateLCD();
}

// TemperatureID
void HMI_Temperature() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_temp.inc(1 + TEMP_CASE_TOTAL)) Move_Highlight(1, select_temp.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_temp.dec()) Move_Highlight(-1, select_temp.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_temp.now) {
      case CASE_BACK:
        checkkey = Control;
        select_control.set(1);
        index_control = MROWS;
        Draw_Control_Menu();
        break;
      #if HAS_HOTEND
        case TEMP_CASE_TEMP:
          checkkey = ETemp;
          HMI_ValueStruct.E_Temp = thermalManager.degTargetHotend(0);
          Draw_Edit_Integer3(1, HMI_ValueStruct.E_Temp, true);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_HEATED_BED
        case TEMP_CASE_BED:
          checkkey = BedTemp;
          HMI_ValueStruct.Bed_Temp = thermalManager.degTargetBed();
          Draw_Edit_Integer3(2, HMI_ValueStruct.Bed_Temp, true);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_FAN
        case TEMP_CASE_FAN:
          checkkey = FanSpeed;
          HMI_ValueStruct.Fan_speed = thermalManager.fan_speed[0];
          Draw_Edit_Integer3(3, HMI_ValueStruct.Fan_speed, true);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_HOTEND
        case TEMP_CASE_PLA: {
          checkkey = PLAPreheat;
          select_PLA.reset();
          HMI_ValueStruct.show_mode = -2;

          Clear_Main_Window();

          if (HMI_IsChinese()) {
            DWIN_Frame_TitleCopy(59, 16, 81, 14);                       // "PLA Settings"
            Item_AreaCopy(100, 89, 124, 101, PREHEAT_CASE_TEMP);
            Item_AreaCopy(1, 134, 56, 146, PREHEAT_CASE_TEMP, 24);      // PLA nozzle temp
            #if HAS_HEATED_BED
              Item_AreaCopy(100, 89, 124, 101, PREHEAT_CASE_BED);
              Item_AreaCopy(58, 134, 113, 146, PREHEAT_CASE_BED, 24);   // PLA bed temp
            #endif
            #if HAS_FAN
              Item_AreaCopy(100, 89, 124, 101, PREHEAT_CASE_FAN);
              Item_AreaCopy(115, 134, 170, 146, PREHEAT_CASE_FAN, 24);  // PLA fan speed
            #endif
            #if ENABLED(EEPROM_SETTINGS)
              Item_AreaCopy(72, 148, 151, 162, PREHEAT_CASE_SAVE);      // Save PLA configuration
            #endif
          }
          else {
            #ifdef USE_STRING_HEADINGS
              Draw_Title(F(PREHEAT_1_LABEL " Settings")); // TODO: GET_TEXT_F
            #else
              DWIN_Frame_TitleCopy(56, 15, 85, 14);                       // "Temperature"  TODO: "PLA Settings"
            #endif
            #ifdef USE_STRING_TITLES
              DWIN_Draw_Label(PREHEAT_CASE_TEMP, F("Nozzle Temp"));
              #if HAS_HEATED_BED
                DWIN_Draw_Label(PREHEAT_CASE_BED, F("Bed Temp"));
              #endif
              #if HAS_FAN
                DWIN_Draw_Label(PREHEAT_CASE_FAN, GET_TEXT_F(MSG_FAN_SPEED));
              #endif
              #if ENABLED(EEPROM_SETTINGS)
                DWIN_Draw_Label(PREHEAT_CASE_SAVE, GET_TEXT_F(MSG_STORE_EEPROM));
              #endif
            #else
              say_pla_en(0, PREHEAT_CASE_TEMP);                           // "PLA"
              Item_AreaCopy(198, 104, 237, 114, PREHEAT_CASE_TEMP, 27);   // "Nozzle"
              Item_AreaCopy(1,  89,  81, 102, PREHEAT_CASE_TEMP, 71);     // "Temperature"
              #if HAS_HEATED_BED
                say_pla_en(0, PREHEAT_CASE_BED);                          // "PLA"
                Item_AreaCopy(240, 104, 264, 114, PREHEAT_CASE_BED, 27);  // "Bed"
                Item_AreaCopy(1, 89, 83, 101, PREHEAT_CASE_BED, 54);      // "Temperature"
              #endif
              #if HAS_FAN
                say_pla_en(0, PREHEAT_CASE_FAN);                          // "PLA"
                Item_AreaCopy(0, 119, 64, 132, PREHEAT_CASE_FAN, 27);     // "Fan speed"
              #endif
              #if ENABLED(EEPROM_SETTINGS)
                Item_AreaCopy(98, 164, 233, 177, PREHEAT_CASE_SAVE);      // "Save PLA parameters"
              #endif
            #endif
          }

          Draw_Back_First();

          uint8_t i = 0;
          Draw_Menu_Line(++i, ICON_SetEndTemp);
          Draw_Edit_Integer3(i, ui.material_preset[0].hotend_temp);
          #if HAS_HEATED_BED
            Draw_Menu_Line(++i, ICON_SetBedTemp);
            Draw_Edit_Integer3(i, ui.material_preset[0].bed_temp);
          #endif
          #if HAS_FAN
            Draw_Menu_Line(++i, ICON_FanSpeed);
            Draw_Edit_Integer3(i, ui.material_preset[0].fan_speed);
          #endif
          #if ENABLED(EEPROM_SETTINGS)
            Draw_Menu_Line(++i, ICON_WriteEEPROM);
          #endif
        } break;

        case TEMP_CASE_ABS: { // ABS preheat setting
          checkkey = ABSPreheat;
          select_ABS.reset();
          HMI_ValueStruct.show_mode = -3;

          Clear_Main_Window();

          if (HMI_IsChinese()) {
            DWIN_Frame_TitleCopy(142, 16, 82, 14);                        // "ABS Settings"

            Item_AreaCopy(180, 89, 204, 100, PREHEAT_CASE_TEMP);
            Item_AreaCopy(1, 134, 56, 146, PREHEAT_CASE_TEMP, 24);        // ABS nozzle temp
            #if HAS_HEATED_BED
              Item_AreaCopy(180, 89, 204, 100, PREHEAT_CASE_BED);
              Item_AreaCopy(58, 134, 113, 146, PREHEAT_CASE_BED, 24);     // ABS bed temp
            #endif
            #if HAS_FAN
              Item_AreaCopy(180, 89, 204, 100, PREHEAT_CASE_FAN);
              Item_AreaCopy(115, 134, 170, 146, PREHEAT_CASE_FAN, 24);    // ABS fan speed
            #endif
            #if ENABLED(EEPROM_SETTINGS)
              Item_AreaCopy(72, 148, 151, 162, PREHEAT_CASE_SAVE);
              Item_AreaCopy(180, 89, 204, 100, PREHEAT_CASE_SAVE, 28, 2); // Save ABS configuration
            #endif
          }
          else {
            #ifdef USE_STRING_HEADINGS
              Draw_Title(F("ABS Settings")); // TODO: GET_TEXT_F
            #else
              DWIN_Frame_TitleCopy(56, 15, 85, 14);                       // "Temperature"  TODO: "ABS Settings"
            #endif
            #ifdef USE_STRING_TITLES
              DWIN_Draw_Label(PREHEAT_CASE_TEMP, F("Nozzle Temp"));
              #if HAS_HEATED_BED
                DWIN_Draw_Label(PREHEAT_CASE_BED, F("Bed Temp"));
              #endif
              #if HAS_FAN
                DWIN_Draw_Label(PREHEAT_CASE_FAN, GET_TEXT_F(MSG_FAN_SPEED));
              #endif
              #if ENABLED(EEPROM_SETTINGS)
                DWIN_Draw_Label(PREHEAT_CASE_SAVE, GET_TEXT_F(MSG_STORE_EEPROM));
              #endif
            #else
              say_abs_en(0, PREHEAT_CASE_TEMP);                           // "ABS"
              Item_AreaCopy(197, 104, 238, 114, PREHEAT_CASE_TEMP, 29);   // "Nozzle"
              Item_AreaCopy(1,  89,  34, 102, PREHEAT_CASE_TEMP, 73);     // "Temp"
              #if HAS_HEATED_BED
                say_abs_en(0, PREHEAT_CASE_BED);                          // "ABS"
                Item_AreaCopy(240, 104, 264, 114, PREHEAT_CASE_BED, 29);  // "Bed"
                Item_AreaCopy(1,  89,  83, 102, PREHEAT_CASE_BED, 56);    // "Temperature"
              #endif
              #if HAS_FAN
                say_abs_en(0, PREHEAT_CASE_FAN);                          // "ABS"
                Item_AreaCopy(0, 119,  64, 132, PREHEAT_CASE_FAN, 29);    // "Fan speed"
              #endif
              #if ENABLED(EEPROM_SETTINGS)
                Item_AreaCopy(98, 165, 233, 177, PREHEAT_CASE_SAVE);      // "Save PLA parameters"
                say_abs_en(33, PREHEAT_CASE_SAVE);                        // "ABS"
              #endif
            #endif
          }

          Draw_Back_First();

          uint8_t i = 0;
          Draw_Menu_Line(++i, ICON_SetEndTemp);
          Draw_Edit_Integer3(i, ui.material_preset[1].hotend_temp);
          #if HAS_HEATED_BED
            Draw_Menu_Line(++i, ICON_SetBedTemp);
            Draw_Edit_Integer3(i, ui.material_preset[1].bed_temp);
          #endif
          #if HAS_FAN
            Draw_Menu_Line(++i, ICON_FanSpeed);
            Draw_Edit_Integer3(i, ui.material_preset[1].fan_speed);
          #endif
          #if ENABLED(EEPROM_SETTINGS)
            Draw_Menu_Line(++i, ICON_WriteEEPROM);
          #endif

        } break;

      #endif // HAS_HOTEND
    }
  }
  DWIN_UpdateLCD();
}

void Draw_Max_Speed_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 16, 28, 13);          // "Max Speed (mm/s)"

    auto say_max_speed_cn = [](const uint8_t line) {
      Item_AreaCopy(173, 133, 228, 147, line);    // "Max speed"
    };

    say_max_speed_cn(1);                          // "Max speed"
    Item_AreaCopy(229, 133, 236, 147, 1, 58);     // "X"
    say_max_speed_cn(2);                          // "Max speed"
    Item_AreaCopy(1, 150, 7, 160, 2, 58, 3);      // "Y"
    say_max_speed_cn(3);                          // "Max speed"
    Item_AreaCopy(9, 150, 16, 160, 3, 58, 3);     // "Z"
    #if HAS_HOTEND
      say_max_speed_cn(4);                        // "Max speed"
      Item_AreaCopy(18, 150, 25, 160, 4, 58, 3);  // "E"
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(F("Max Speed (mm/s)")); // TODO: GET_TEXT_F
    #else
      DWIN_Frame_TitleCopy(144, 16, 46, 11);                  // "Max Speed (mm/s)"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(1, F("Max Feedrate X"));
      DWIN_Draw_Label(2, F("Max Feedrate Y"));
      DWIN_Draw_Label(3, F("Max Feedrate Z"));
      #if HAS_HOTEND
        DWIN_Draw_Label(4, F("Max Feedrate E"));
      #endif
    #else
      say_max_en(1); say_speed_en(30, 1); say_x_en(73, 1);    // "Max Speed X"
      say_max_en(2); say_speed_en(30, 2); say_y_en(73, 2);    // "Max Speed Y"
      say_max_en(3); say_speed_en(30, 3); say_z_en(73, 3);    // "Max Speed Z"
      #if HAS_HOTEND
        say_max_en(4); say_speed_en(30, 4); say_e_en(73, 4);  // "Max Speed E"
      #endif
    #endif
  }

  Draw_Back_First();
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_MaxSpeedX + i);
  Draw_Edit_Integer4(1, planner.settings.max_feedrate_mm_s[X_AXIS]);
  Draw_Edit_Integer4(2, planner.settings.max_feedrate_mm_s[Y_AXIS]);
  Draw_Edit_Integer4(3, planner.settings.max_feedrate_mm_s[Z_AXIS]);
  #if HAS_HOTEND
    Draw_Edit_Integer4(4, planner.settings.max_feedrate_mm_s[E_AXIS]);
  #endif
}

void Draw_Max_Accel_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 16, 28, 13);            // "Acceleration"

    Item_AreaCopy(173, 133, 200, 147, 1);
    Item_AreaCopy( 28, 149,  69, 161, 1, 30, 1);
    Item_AreaCopy(229, 133, 236, 147, 1, 74);       // Max acceleration X
    Item_AreaCopy(173, 133, 200, 147, 2);
    Item_AreaCopy( 28, 149,  69, 161, 2, 30, 1);
    Item_AreaCopy(  1, 150,   7, 160, 2, 74, 2);    // Max acceleration Y
    Item_AreaCopy(173, 133, 200, 147, 3);
    Item_AreaCopy( 28, 149,  69, 161, 3, 30, 1);
    Item_AreaCopy(  9, 150,  16, 160, 3, 74, 2);    // Max acceleration Z
    #if HAS_HOTEND
      Item_AreaCopy(173, 133, 200, 147, 4);
      Item_AreaCopy( 28, 149,  69, 161, 4, 30, 1);
      Item_AreaCopy( 18, 150,  25, 160, 4, 74, 2);  // Max acceleration E
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_ACCELERATION));
    #else
      DWIN_Frame_TitleCopy(144, 16, 46, 11);    // "Acceleration"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(1, F("Max Accel X"));
      DWIN_Draw_Label(2, F("Max Accel Y"));
      DWIN_Draw_Label(3, F("Max Accel Z"));
      #if HAS_HOTEND
        DWIN_Draw_Label(4, F("Max Accel E"));
      #endif
    #else
      say_max_accel_en(1); say_x_en(112, 1);    // "Max Acceleration X"
      say_max_accel_en(2); say_y_en(112, 2);    // "Max Acceleration Y"
      say_max_accel_en(3); say_z_en(112, 3);    // "Max Acceleration Z"
      #if HAS_HOTEND
        say_max_accel_en(4); say_e_en(112, 4);  // "Max Acceleration E"
      #endif
    #endif
  }

  Draw_Back_First();
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_MaxAccX + i);
  Draw_Edit_Integer4(1, planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
  Draw_Edit_Integer4(2, planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
  Draw_Edit_Integer4(3, planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
  #if HAS_HOTEND
    Draw_Edit_Integer4(4, planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
  #endif
}

#if HAS_CLASSIC_JERK
  void Draw_Max_Jerk_Menu() {
    Clear_Main_Window();

    if (HMI_IsChinese()) {
      DWIN_Frame_TitleCopy(1, 16, 28, 13);            // "Jerk"

      Item_AreaCopy(173, 133, 200, 147, 1);
      Item_AreaCopy(  1, 180,  28, 192, 1, 30, 1);
      Item_AreaCopy(202, 133, 228, 147, 1, 56);
      Item_AreaCopy(229, 133, 236, 147, 1, 86);       // Max Jerk speed X
      Item_AreaCopy(173, 133, 200, 147, 2);
      Item_AreaCopy(  1, 180,  28, 192, 2, 30, 1);
      Item_AreaCopy(202, 133, 228, 147, 2, 56);
      Item_AreaCopy(  1, 150,   7, 160, 2, 86, 3);    // Max Jerk speed Y
      Item_AreaCopy(173, 133, 200, 147, 3);
      Item_AreaCopy(  1, 180,  28, 192, 3, 30, 1);
      Item_AreaCopy(202, 133, 228, 147, 3, 56);
      Item_AreaCopy(  9, 150,  16, 160, 3, 86, 3);    // Max Jerk speed Z
      #if HAS_HOTEND
        Item_AreaCopy(173, 133, 200, 147, 4);
        Item_AreaCopy(  1, 180,  28, 192, 4, 30, 1);
        Item_AreaCopy(202, 133, 228, 147, 4, 56);
        Item_AreaCopy( 18, 150,  25, 160, 4, 86, 3);  // Max Jerk speed E
      #endif
    }
    else {
      #ifdef USE_STRING_HEADINGS
        Draw_Title(GET_TEXT_F(MSG_JERK));
      #else
        DWIN_Frame_TitleCopy(144, 16, 46, 11);        // "Jerk"
      #endif
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(1, GET_TEXT_F(MSG_VA_JERK));
        DWIN_Draw_Label(2, GET_TEXT_F(MSG_VB_JERK));
        DWIN_Draw_Label(3, GET_TEXT_F(MSG_VC_JERK));
        #if HAS_HOTEND
          DWIN_Draw_Label(4, GET_TEXT_F(MSG_VE_JERK));
        #endif
      #else
        say_max_jerk_speed_en(1); say_x_en(102, 1);   // Max Jerk speed X
        say_max_jerk_speed_en(2); say_y_en(102, 2);   // Max Jerk speed Y
        say_max_jerk_speed_en(3); say_z_en(102, 3);   // Max Jerk speed Z
        #if HAS_HOTEND
          say_max_jerk_speed_en(4); say_e_en(102, 4); // Max Jerk speed E
        #endif
      #endif
    }

    Draw_Back_First();
    LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_MaxSpeedJerkX + i);
    Draw_Edit_Float3(1, planner.max_jerk[X_AXIS] * MINUNITMULT);
    Draw_Edit_Float3(2, planner.max_jerk[Y_AXIS] * MINUNITMULT);
    Draw_Edit_Float3(3, planner.max_jerk[Z_AXIS] * MINUNITMULT);
    #if HAS_HOTEND
      Draw_Edit_Float3(4, planner.max_jerk[E_AXIS] * MINUNITMULT);
    #endif
  }
#endif

void Draw_Steps_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 16, 28, 13);            // "Steps per mm"

    Item_AreaCopy(153, 148, 194, 161, 1);
    Item_AreaCopy(229, 133, 236, 147, 1, 44);       // Transmission Ratio X
    Item_AreaCopy(153, 148, 194, 161, 2);
    Item_AreaCopy(  1, 150,   7, 160, 2, 44, 3);    // Transmission Ratio Y
    Item_AreaCopy(153, 148, 194, 161, 3);
    Item_AreaCopy(  9, 150,  16, 160, 3, 44, 3);    // Transmission Ratio Z
    #if HAS_HOTEND
      Item_AreaCopy(153, 148, 194, 161, 4);
      Item_AreaCopy( 18, 150,  25, 160, 4, 44, 3);  // Transmission Ratio E
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_STEPS_PER_MM));
    #else
      DWIN_Frame_TitleCopy(144, 16, 46, 11);        // "Steps per mm"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(1, GET_TEXT_F(MSG_A_STEPS));
      DWIN_Draw_Label(2, GET_TEXT_F(MSG_B_STEPS));
      DWIN_Draw_Label(3, GET_TEXT_F(MSG_C_STEPS));
      #if HAS_HOTEND
        DWIN_Draw_Label(4, GET_TEXT_F(MSG_E_STEPS));
      #endif
    #else
      say_steps_per_mm_en(1); say_x_en(101, 1);     // "Steps-per-mm X"
      say_steps_per_mm_en(2); say_y_en(101, 2);     // "Y"
      say_steps_per_mm_en(3); say_z_en(101, 3);     // "Z"
      #if HAS_HOTEND
        say_steps_per_mm_en(4); say_e_en(101, 4);   // "E"
      #endif
    #endif
  }

  Draw_Back_First();
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_StepX + i);
  Draw_Edit_Float3(1, planner.settings.axis_steps_per_mm[X_AXIS] * MINUNITMULT);
  Draw_Edit_Float3(2, planner.settings.axis_steps_per_mm[Y_AXIS] * MINUNITMULT);
  Draw_Edit_Float3(3, planner.settings.axis_steps_per_mm[Z_AXIS] * MINUNITMULT);
  #if HAS_HOTEND
    Draw_Edit_Float3(4, planner.settings.axis_steps_per_mm[E_AXIS] * MINUNITMULT);
  #endif
}

// Motion
void HMI_Motion() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_motion.inc(1 + MOTION_CASE_TOTAL)) Move_Highlight(1, select_motion.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_motion.dec()) Move_Highlight(-1, select_motion.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_motion.now) {
      case CASE_BACK:
        checkkey = Control;
        select_control.set(CONTROL_CASE_MOVE);
        index_control = MROWS;
        Draw_Control_Menu();
        break;
      case MOTION_CASE_RATE:
        checkkey = MaxSpeed;
        select_speed.reset();
        Draw_Max_Speed_Menu();
        break;
      case MOTION_CASE_ACCEL:
        checkkey = MaxAcceleration;
        select_acc.reset();
        Draw_Max_Accel_Menu();
        break;
      #if HAS_CLASSIC_JERK
        case MOTION_CASE_JERK:
          checkkey = MaxJerk;
          select_jerk.reset();
          Draw_Max_Jerk_Menu();
         break;
      #endif
      case MOTION_CASE_STEPS:
        checkkey = Step;
        select_step.reset();
        Draw_Steps_Menu();
        break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

// Advanced Settings
void HMI_AdvSet() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_advset.inc(1 + ADVSET_CASE_TOTAL)) {
      if (select_advset.now > MROWS && select_advset.now > index_advset) {
        index_advset = select_advset.now;

        // Scroll up and draw a blank bottom line
        Scroll_Menu(DWIN_SCROLL_UP);

        //switch (index_advset) {  // Redraw last menu items
        //  default: break;
        //}

      }
      else {
        Move_Highlight(1, select_advset.now + MROWS - index_advset);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_advset.dec()) {
      if (select_advset.now < index_advset - MROWS) {
        index_advset--;
        Scroll_Menu(DWIN_SCROLL_DOWN);

        //switch (index_advset) {  // Redraw first menu items
        //  default: break;
        //}
      }
      else {
        Move_Highlight(-1, select_advset.now + MROWS - index_advset);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_advset.now) {
      case CASE_BACK:
        checkkey = Control;
        select_control.set(CONTROL_CASE_ADVSET);
        index_control = CONTROL_CASE_ADVSET;
        Draw_Control_Menu();
        break;

      #if HAS_HOME_OFFSET
        case ADVSET_CASE_HOMEOFF:
          checkkey = HomeOff;
          select_item.reset();
          HMI_ValueStruct.Home_OffX_scaled = home_offset[X_AXIS] * 10;
          HMI_ValueStruct.Home_OffY_scaled = home_offset[Y_AXIS] * 10;
          HMI_ValueStruct.Home_OffZ_scaled = home_offset[Z_AXIS] * 10;
          Draw_HomeOff_Menu();
          break;
      #endif

      #if HAS_ONESTEP_LEVELING
        case ADVSET_CASE_PROBEOFF:
          checkkey = ProbeOff;
          select_item.reset();
          HMI_ValueStruct.Probe_OffX_scaled = probe.offset.x * 10;
          HMI_ValueStruct.Probe_OffY_scaled = probe.offset.y * 10;
          Draw_ProbeOff_Menu();
          break;
      #endif

      #if HAS_HOTEND
        case ADVSET_CASE_HEPID:
          thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0);
          thermalManager.PID_autotune(ui.material_preset[0].hotend_temp, H_E0, 10, true);
          break;
      #endif

      #if HAS_HEATED_BED
        case ADVSET_CASE_BEDPID:
          thermalManager.setTargetBed(ui.material_preset[0].bed_temp);
          thermalManager.PID_autotune(ui.material_preset[0].bed_temp, H_BED, 10, true);
          break;
      #endif

      #if ENABLED(POWER_LOSS_RECOVERY)
        case ADVSET_CASE_PWRLOSSR:
          recovery.enable(!recovery.enabled);
          Draw_Checkbox_Line(ADVSET_CASE_PWRLOSSR + MROWS - index_advset, recovery.enabled);
          break;
      #endif
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

#if HAS_HOME_OFFSET

  // Home Offset
  void HMI_HomeOff() {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    // Avoid flicker by updating only the previous menu
    if (encoder_diffState == ENCODER_DIFF_CW) {
      if (select_item.inc(1 + 3)) Move_Highlight(1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      if (select_item.dec()) Move_Highlight(-1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      switch (select_item.now) {
        case CASE_BACK:
          checkkey = AdvSet;
          select_advset.set(ADVSET_CASE_HOMEOFF);
          Draw_AdvancedSettings_Menu();
          break;
        case 1: // Home Offset X
          checkkey = HomeOffX;
          Draw_Edit_Signed_Float3(1, HMI_ValueStruct.Home_OffX_scaled, true);
          EncoderRate.enabled = true;
          break;
        case 2: // Home Offset Y
          checkkey = HomeOffY;
          Draw_Edit_Signed_Float3(2, HMI_ValueStruct.Home_OffY_scaled, true);
          EncoderRate.enabled = true;
          break;
        case 3: // Home Offset Z
          checkkey = HomeOffZ;
          Draw_Edit_Signed_Float3(3, HMI_ValueStruct.Home_OffZ_scaled, true);
          EncoderRate.enabled = true;
          break;
        default: break;
      }
    }
    DWIN_UpdateLCD();
  }

  void HMI_HomeOffN(const AxisEnum axis, float &posScaled, const_float_t lo, const_float_t hi) {
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    if (Apply_Encoder(encoder_diffState, posScaled)) {
      checkkey = HomeOff;
      EncoderRate.enabled = false;
      set_home_offset(axis, posScaled / 10);
      Draw_Edit_Signed_Float3(select_item.now, posScaled);
      return;
    }
    LIMIT(posScaled, lo, hi);
    Draw_Edit_Signed_Float3(select_item.now, posScaled, true);
  }

  void HMI_HomeOffX() { HMI_HomeOffN(X_AXIS, HMI_ValueStruct.Home_OffX_scaled, -500, 500); }
  void HMI_HomeOffY() { HMI_HomeOffN(Y_AXIS, HMI_ValueStruct.Home_OffY_scaled, -500, 500); }
  void HMI_HomeOffZ() { HMI_HomeOffN(Z_AXIS, HMI_ValueStruct.Home_OffZ_scaled,  -20,  20); }

#endif // HAS_HOME_OFFSET

#if HAS_ONESTEP_LEVELING

  // Probe Offset
  void HMI_ProbeOff() {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    // Avoid flicker by updating only the previous menu
    if (encoder_diffState == ENCODER_DIFF_CW) {
      if (select_item.inc(1 + 2)) Move_Highlight(1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      if (select_item.dec()) Move_Highlight(-1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      switch (select_item.now) {
        case CASE_BACK:
          checkkey = AdvSet;
          select_advset.set(ADVSET_CASE_PROBEOFF);
          Draw_AdvancedSettings_Menu();
          break;
        case 1: // Probe Offset X
          checkkey = ProbeOffX;
          Draw_Edit_Signed_Float3(1, HMI_ValueStruct.Probe_OffX_scaled, true);
          EncoderRate.enabled = true;
          break;
        case 2: // Probe Offset Y
          checkkey = ProbeOffY;
          Draw_Edit_Signed_Float3(2, HMI_ValueStruct.Probe_OffY_scaled, true);
          EncoderRate.enabled = true;
          break;
      }
    }
    DWIN_UpdateLCD();
  }

  void HMI_ProbeOffN(float &posScaled, float &offset_ref) {
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    if (Apply_Encoder(encoder_diffState, posScaled)) {
      checkkey = ProbeOff;
      EncoderRate.enabled = false;
      offset_ref = posScaled / 10;
      Draw_Edit_Signed_Float3(select_item.now, posScaled);
      return;
    }
    LIMIT(posScaled, -500, 500);
    Draw_Edit_Signed_Float3(select_item.now, posScaled, true);
  }

  void HMI_ProbeOffX() { HMI_ProbeOffN(HMI_ValueStruct.Probe_OffX_scaled, probe.offset.x); }
  void HMI_ProbeOffY() { HMI_ProbeOffN(HMI_ValueStruct.Probe_OffY_scaled, probe.offset.y); }

#endif // HAS_ONESTEP_LEVELING

// Info
void HMI_Info() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    #if HAS_ONESTEP_LEVELING
      checkkey = Control;
      select_control.set(CONTROL_CASE_INFO);
      Draw_Control_Menu();
    #else
      select_page.set(3);
      Goto_MainMenu();
    #endif
  }
  DWIN_UpdateLCD();
}

// Tune
void HMI_Tune() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_tune.inc(1 + TUNE_CASE_TOTAL)) {
      if (select_tune.now > MROWS && select_tune.now > index_tune) {
        index_tune = select_tune.now;
        Scroll_Menu(DWIN_SCROLL_UP);
      }
      else
        Move_Highlight(1, select_tune.now + MROWS - index_tune);
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_tune.dec()) {
      if (select_tune.now < index_tune - MROWS) {
        index_tune--;
        Scroll_Menu(DWIN_SCROLL_DOWN);
        if (index_tune == MROWS) Draw_Back_First();
      }
      else
        Move_Highlight(-1, select_tune.now + MROWS - index_tune);
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_tune.now) {
      case 0: { // Back
        select_print.set(0);
        Goto_PrintProcess();
      }
      break;
      case TUNE_CASE_SPEED: // Print speed
        checkkey = PrintSpeed;
        HMI_ValueStruct.print_speed = feedrate_percentage;
        Draw_Edit_Integer3(TUNE_CASE_SPEED + MROWS - index_tune, HMI_ValueStruct.print_speed, true);
        EncoderRate.enabled = true;
        break;
      #if HAS_HOTEND
        case TUNE_CASE_TEMP: // Nozzle temp
          checkkey = ETemp;
          HMI_ValueStruct.E_Temp = thermalManager.degTargetHotend(0);
          Draw_Edit_Integer3(TUNE_CASE_TEMP + MROWS - index_tune, HMI_ValueStruct.E_Temp, true);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_HEATED_BED
        case TUNE_CASE_BED: // Bed temp
          checkkey = BedTemp;
          HMI_ValueStruct.Bed_Temp = thermalManager.degTargetBed();
          Draw_Edit_Integer3(TUNE_CASE_BED + MROWS - index_tune, HMI_ValueStruct.Bed_Temp, true);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_FAN
        case TUNE_CASE_FAN: // Fan speed
          checkkey = FanSpeed;
          HMI_ValueStruct.Fan_speed = thermalManager.fan_speed[0];
          Draw_Edit_Integer3(TUNE_CASE_FAN + MROWS - index_tune, HMI_ValueStruct.Fan_speed, true);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_ZOFFSET_ITEM
        case TUNE_CASE_ZOFF: // Z-offset
          #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
            checkkey = Homeoffset;
            HMI_ValueStruct.offset_value = BABY_Z_VAR * 100;
            Draw_Edit_Signed_Float2(TUNE_CASE_ZOFF + MROWS - index_tune, HMI_ValueStruct.offset_value, true);
            EncoderRate.enabled = true;
          #else
            // Apply workspace offset, making the current position 0,0,0
            queue.inject(F("G92 X0 Y0 Z0"));
            HMI_AudioFeedback();
          #endif
        break;
      #endif
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

#if HAS_PREHEAT

  // PLA Preheat
  void HMI_PLAPreheatSetting() {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    // Avoid flicker by updating only the previous menu
    if (encoder_diffState == ENCODER_DIFF_CW) {
      if (select_PLA.inc(1 + PREHEAT_CASE_TOTAL)) Move_Highlight(1, select_PLA.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      if (select_PLA.dec()) Move_Highlight(-1, select_PLA.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      switch (select_PLA.now) {
        case CASE_BACK:
          checkkey = TemperatureID;
          select_temp.now = TEMP_CASE_PLA;
          HMI_ValueStruct.show_mode = -1;
          Draw_Temperature_Menu();
          break;
        #if HAS_HOTEND
          case PREHEAT_CASE_TEMP:
            checkkey = ETemp;
            HMI_ValueStruct.E_Temp = ui.material_preset[0].hotend_temp;
            Draw_Edit_Integer3(PREHEAT_CASE_TEMP, ui.material_preset[0].hotend_temp, true);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_HEATED_BED
          case PREHEAT_CASE_BED:
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = ui.material_preset[0].bed_temp;
            Draw_Edit_Integer3(PREHEAT_CASE_BED, ui.material_preset[0].bed_temp, true);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_FAN
          case PREHEAT_CASE_FAN:
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = ui.material_preset[0].fan_speed;
            Draw_Edit_Integer3(PREHEAT_CASE_FAN, ui.material_preset[0].fan_speed, true);
            EncoderRate.enabled = true;
            break;
        #endif
        #if ENABLED(EEPROM_SETTINGS)
          case PREHEAT_CASE_SAVE: {
            const bool success = settings.save();
            HMI_AudioFeedback(success);
          } break;
        #endif
        default: break;
      }
    }
    DWIN_UpdateLCD();
  }

  // ABS Preheat
  void HMI_ABSPreheatSetting() {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    // Avoid flicker by updating only the previous menu
    if (encoder_diffState == ENCODER_DIFF_CW) {
      if (select_ABS.inc(1 + PREHEAT_CASE_TOTAL)) Move_Highlight(1, select_ABS.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      if (select_ABS.dec()) Move_Highlight(-1, select_ABS.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      switch (select_ABS.now) {
        case CASE_BACK:
          checkkey = TemperatureID;
          select_temp.now = TEMP_CASE_ABS;
          HMI_ValueStruct.show_mode = -1;
          Draw_Temperature_Menu();
          break;
        #if HAS_HOTEND
          case PREHEAT_CASE_TEMP:
            checkkey = ETemp;
            HMI_ValueStruct.E_Temp = ui.material_preset[1].hotend_temp;
            Draw_Edit_Integer3(PREHEAT_CASE_TEMP, ui.material_preset[1].hotend_temp, true);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_HEATED_BED
          case PREHEAT_CASE_BED:
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = ui.material_preset[1].bed_temp;
            Draw_Edit_Integer3(PREHEAT_CASE_BED, ui.material_preset[1].bed_temp, true);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_FAN
          case PREHEAT_CASE_FAN:
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = ui.material_preset[1].fan_speed;
            Draw_Edit_Integer3(PREHEAT_CASE_FAN, ui.material_preset[1].fan_speed, true);
            EncoderRate.enabled = true;
            break;
        #endif
        #if ENABLED(EEPROM_SETTINGS)
          case PREHEAT_CASE_SAVE: {
            const bool success = settings.save();
            HMI_AudioFeedback(success);
          } break;
        #endif
        default: break;
      }
    }
    DWIN_UpdateLCD();
  }

#endif

// Max Speed
void HMI_MaxSpeed() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_speed.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_speed.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_speed.dec()) Move_Highlight(-1, select_speed.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (WITHIN(select_speed.now, 1, 4)) {
      checkkey = MaxSpeed_value;
      HMI_flag.feedspeed_axis = AxisEnum(select_speed.now - 1);
      HMI_ValueStruct.Max_Feedspeed = planner.settings.max_feedrate_mm_s[HMI_flag.feedspeed_axis];
      Draw_Edit_Integer4(select_speed.now, HMI_ValueStruct.Max_Feedspeed, true);
      EncoderRate.enabled = true;
    }
    else { // Back
      checkkey = Motion;
      select_motion.now = MOTION_CASE_RATE;
      Draw_Motion_Menu();
    }
  }
  DWIN_UpdateLCD();
}

// Max Acceleration
void HMI_MaxAcceleration() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_acc.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_acc.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_acc.dec()) Move_Highlight(-1, select_acc.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (WITHIN(select_acc.now, 1, 4)) {
      checkkey = MaxAcceleration_value;
      HMI_flag.acc_axis = AxisEnum(select_acc.now - 1);
      HMI_ValueStruct.Max_Acceleration = planner.settings.max_acceleration_mm_per_s2[HMI_flag.acc_axis];
      Draw_Edit_Integer4(select_acc.now, HMI_ValueStruct.Max_Acceleration, true);
      EncoderRate.enabled = true;
    }
    else { // Back
      checkkey = Motion;
      select_motion.now = MOTION_CASE_ACCEL;
      Draw_Motion_Menu();
    }
  }
  DWIN_UpdateLCD();
}

#if HAS_CLASSIC_JERK
  // Max Jerk
  void HMI_MaxJerk() {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    // Avoid flicker by updating only the previous menu
    if (encoder_diffState == ENCODER_DIFF_CW) {
      if (select_jerk.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_jerk.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      if (select_jerk.dec()) Move_Highlight(-1, select_jerk.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      if (WITHIN(select_jerk.now, 1, 4)) {
        checkkey = MaxJerk_value;
        HMI_flag.jerk_axis = AxisEnum(select_jerk.now - 1);
        HMI_ValueStruct.Max_Jerk_scaled = planner.max_jerk[HMI_flag.jerk_axis] * MINUNITMULT;
        Draw_Edit_Float3(select_jerk.now, HMI_ValueStruct.Max_Jerk_scaled, true);
        EncoderRate.enabled = true;
      }
      else { // Back
        checkkey = Motion;
        select_motion.now = MOTION_CASE_JERK;
        Draw_Motion_Menu();
      }
    }
    DWIN_UpdateLCD();
  }
#endif // HAS_CLASSIC_JERK

// Step
void HMI_Step() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_step.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_step.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_step.dec()) Move_Highlight(-1, select_step.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (WITHIN(select_step.now, 1, 4)) {
      checkkey = Step_value;
      HMI_flag.step_axis = AxisEnum(select_step.now - 1);
      HMI_ValueStruct.Max_Step_scaled = planner.settings.axis_steps_per_mm[HMI_flag.step_axis] * MINUNITMULT;
      Draw_Edit_Float3(select_step.now, HMI_ValueStruct.Max_Step_scaled, true);
      EncoderRate.enabled = true;
    }
    else { // Back
      checkkey = Motion;
      select_motion.now = MOTION_CASE_STEPS;
      Draw_Motion_Menu();
    }
  }
  DWIN_UpdateLCD();
}

void HMI_Init() {
  HMI_SDCardInit();

  for (uint16_t t = 0; t <= 100; t += 2) {
    DWIN_ICON_Show(ICON, ICON_Bar, 15, 260);
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 15 + t * 242 / 100, 260, 257, 280);
    DWIN_UpdateLCD();
    delay(20);
  }

  HMI_SetLanguage();
}

void DWIN_Update() {
  EachMomentUpdate();   // Status update
  HMI_SDCardUpdate();   // SD card update
  DWIN_HandleScreen();  // Rotary encoder update
}

void EachMomentUpdate() {
  static millis_t next_var_update_ms = 0, next_rts_update_ms = 0;

  const millis_t ms = millis();
  if (ELAPSED(ms, next_var_update_ms)) {
    next_var_update_ms = ms + DWIN_VAR_UPDATE_INTERVAL;
    update_variable();
  }

  if (PENDING(ms, next_rts_update_ms)) return;
  next_rts_update_ms = ms + DWIN_SCROLL_UPDATE_INTERVAL;

  if (checkkey == PrintProcess) {
    // if print done
    if (HMI_flag.print_finish && !HMI_flag.done_confirm_flag) {
      HMI_flag.print_finish = false;
      HMI_flag.done_confirm_flag = true;

      TERN_(POWER_LOSS_RECOVERY, recovery.cancel());

      planner.finish_and_disable();

      // show percent bar and value
      _card_percent = 0;
      Draw_Print_ProgressBar();

      // show print done confirm
      DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 250, DWIN_WIDTH - 1, STATUS_Y);
      DWIN_ICON_Show(ICON, HMI_IsChinese() ? ICON_Confirm_C : ICON_Confirm_E, 86, 283);
    }
    else if (HMI_flag.pause_flag != printingIsPaused()) {
      // print status update
      HMI_flag.pause_flag = printingIsPaused();
      ICON_ResumeOrPause();
    }
  }

  // pause after homing
  if (HMI_flag.pause_action && printingIsPaused() && !planner.has_blocks_queued()) {
    HMI_flag.pause_action = false;
    #if ENABLED(PAUSE_HEAT)
      TERN_(HAS_HOTEND, resume_hotend_temp = thermalManager.degTargetHotend(0));
      TERN_(HAS_HEATED_BED, resume_bed_temp = thermalManager.degTargetBed());
      thermalManager.disable_all_heaters();
    #endif
    queue.inject(F("G1 F1200 X0 Y0"));
  }

  if (card.isPrinting() && checkkey == PrintProcess) { // print process
    const uint8_t card_pct = card.percentDone();
    static uint8_t last_cardpercentValue = 101;
    if (last_cardpercentValue != card_pct) { // print percent
      last_cardpercentValue = card_pct;
      if (card_pct) {
        _card_percent = card_pct;
        Draw_Print_ProgressBar();
      }
    }

    duration_t elapsed = print_job_timer.duration(); // print timer

    // Print time so far
    static uint16_t last_Printtime = 0;
    const uint16_t min = (elapsed.value % 3600) / 60;
    if (last_Printtime != min) { // 1 minute update
      last_Printtime = min;
      Draw_Print_ProgressElapsed();
    }

    // Estimate remaining time every 20 seconds
    static millis_t next_remain_time_update = 0;
    if (_card_percent > 1 && ELAPSED(ms, next_remain_time_update) && !HMI_flag.heat_flag) {
      _remain_time = (elapsed.value - dwin_heat_time) / (_card_percent * 0.01f) - (elapsed.value - dwin_heat_time);
      next_remain_time_update += DWIN_REMAIN_TIME_UPDATE_INTERVAL;
      Draw_Print_ProgressRemain();
    }
  }
  else if (dwin_abort_flag && !HMI_flag.home_flag) { // Print Stop
    dwin_abort_flag = false;
    HMI_ValueStruct.print_speed = feedrate_percentage = 100;
    dwin_zoffset = BABY_Z_VAR;
    select_page.set(0);
    Goto_MainMenu();
  }
  #if ENABLED(POWER_LOSS_RECOVERY)
    else if (DWIN_lcd_sd_status && recovery.dwin_flag) { // resume print before power off
      static bool recovery_flag = false;

      recovery.dwin_flag = false;
      recovery_flag = true;

      auto update_selection = [&](const bool sel) {
        HMI_flag.select_flag = sel;
        const uint16_t c1 = sel ? Color_Bg_Window : Select_Color;
        DWIN_Draw_Rectangle(0, c1, 25, 306, 126, 345);
        DWIN_Draw_Rectangle(0, c1, 24, 305, 127, 346);
        const uint16_t c2 = sel ? Select_Color : Color_Bg_Window;
        DWIN_Draw_Rectangle(0, c2, 145, 306, 246, 345);
        DWIN_Draw_Rectangle(0, c2, 144, 305, 247, 346);
      };

      Popup_Window_Resume();
      update_selection(true);

      char * const name = card.longest_filename();
      const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(name) * (MENU_CHR_W)) / 2;
      DWIN_Draw_String(true, font8x16, Popup_Text_Color, Color_Bg_Window, npos, 252, name);
      DWIN_UpdateLCD();

      while (recovery_flag) {
        EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
        if (encoder_diffState != ENCODER_DIFF_NO) {
          if (encoder_diffState == ENCODER_DIFF_ENTER) {
            recovery_flag = false;
            if (HMI_flag.select_flag) break;
            TERN_(POWER_LOSS_RECOVERY, queue.inject(F("M1000C")));
            HMI_StartFrame(true);
            return;
          }
          else
            update_selection(encoder_diffState == ENCODER_DIFF_CW);

          DWIN_UpdateLCD();
        }
      }

      select_print.set(0);
      HMI_ValueStruct.show_mode = 0;
      queue.inject(F("M1000"));
      Goto_PrintProcess();
      Draw_Status_Area(true);
    }
  #endif // POWER_LOSS_RECOVERY

  DWIN_UpdateLCD();
}

void DWIN_HandleScreen() {
  switch (checkkey) {
    case MainMenu:        HMI_MainMenu(); break;
    case SelectFile:      HMI_SelectFile(); break;
    case Prepare:         HMI_Prepare(); break;
    case Control:         HMI_Control(); break;
    case Leveling:        break;
    case PrintProcess:    HMI_Printing(); break;
    case Print_window:    HMI_PauseOrStop(); break;
    case AxisMove:        HMI_AxisMove(); break;
    case TemperatureID:   HMI_Temperature(); break;
    case Motion:          HMI_Motion(); break;
    case AdvSet:          HMI_AdvSet(); break;
    #if HAS_HOME_OFFSET
      case HomeOff:       HMI_HomeOff(); break;
      case HomeOffX:      HMI_HomeOffX(); break;
      case HomeOffY:      HMI_HomeOffY(); break;
      case HomeOffZ:      HMI_HomeOffZ(); break;
    #endif
    #if HAS_ONESTEP_LEVELING
      case ProbeOff:      HMI_ProbeOff(); break;
      case ProbeOffX:     HMI_ProbeOffX(); break;
      case ProbeOffY:     HMI_ProbeOffY(); break;
    #endif
    case Info:            HMI_Info(); break;
    case Tune:            HMI_Tune(); break;
    #if HAS_PREHEAT
      case PLAPreheat:    HMI_PLAPreheatSetting(); break;
      case ABSPreheat:    HMI_ABSPreheatSetting(); break;
    #endif
    case MaxSpeed:        HMI_MaxSpeed(); break;
    case MaxAcceleration: HMI_MaxAcceleration(); break;
    #if HAS_CLASSIC_JERK
      case MaxJerk:       HMI_MaxJerk(); break;
    #endif
    case Step:            HMI_Step(); break;
    case Move_X:          HMI_Move_X(); break;
    case Move_Y:          HMI_Move_Y(); break;
    case Move_Z:          HMI_Move_Z(); break;
    #if HAS_HOTEND
      case Extruder:      HMI_Move_E(); break;
      case ETemp:         HMI_ETemp(); break;
    #endif
    #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
      case Homeoffset:    HMI_Zoffset(); break;
    #endif
    #if HAS_HEATED_BED
      case BedTemp:       HMI_BedTemp(); break;
    #endif
    #if HAS_PREHEAT && HAS_FAN
      case FanSpeed:      HMI_FanSpeed(); break;
    #endif
    case PrintSpeed:      HMI_PrintSpeed(); break;
    case MaxSpeed_value:  HMI_MaxFeedspeedXYZE(); break;
    case MaxAcceleration_value: HMI_MaxAccelerationXYZE(); break;
    #if HAS_CLASSIC_JERK
      case MaxJerk_value: HMI_MaxJerkXYZE(); break;
    #endif
    case Step_value:      HMI_StepXYZE(); break;
    default: break;
  }
}

void DWIN_CompletedHoming() {
  HMI_flag.home_flag = false;
  dwin_zoffset = TERN0(HAS_BED_PROBE, probe.offset.z);
  if (checkkey == Last_Prepare) {
    checkkey = Prepare;
    select_prepare.now = PREPARE_CASE_HOME;
    index_prepare = MROWS;
    Draw_Prepare_Menu();
  }
  else if (checkkey == Back_Main) {
    HMI_ValueStruct.print_speed = feedrate_percentage = 100;
    planner.finish_and_disable();
    Goto_MainMenu();
  }
}

void DWIN_CompletedLeveling() {
  if (checkkey == Leveling) Goto_MainMenu();
}

void DWIN_StatusChanged(const char * const cstr/*=nullptr*/) {
  DWIN_Draw_Rectangle(1, Color_Bg_Blue, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 24);
  const int8_t x = _MAX(0U, DWIN_WIDTH - strlen(cstr) * MENU_CHR_W) / 2;
  DWIN_Draw_String(false, font8x16, Color_White, Color_Bg_Blue, x, STATUS_Y + 3, cstr);
  DWIN_UpdateLCD();
}

void DWIN_StatusChanged(FSTR_P const fstr) {
  char str[strlen_P(FTOP(fstr)) + 1];
  strcpy_P(str, FTOP(fstr));
  DWIN_StatusChanged(str);
}

#endif // DWIN_CREALITY_LCD
