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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD)

#if ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT) && DISABLED(PROBE_MANUALLY)
  #define HAS_ONESTEP_LEVELING 1
#endif

#include "dwin.h"

#include <WString.h>
#include <stdio.h>
#include <string.h>

#include "../fontutils.h"
#include "../ultralcd.h"

#include "../../sd/cardreader.h"

#include "../../MarlinCore.h"
#include "../../core/serial.h"
#include "../../core/macros.h"
#include "../../gcode/queue.h"

#include "../../feature/powerloss.h"
#include "../../feature/babystep.h"

#include "../../module/settings.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../module/motion.h"
#include "../../module/planner.h"

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../feature/host_actions.h"
#endif

#if HAS_ONESTEP_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif

#ifndef MACHINE_SIZE
  #define MACHINE_SIZE "220x220x250"
#endif
#ifndef CORP_WEBSITE_C
  #define CORP_WEBSITE_C "www.cxsw3d.com"
#endif
#ifndef CORP_WEBSITE_E
  #define CORP_WEBSITE_E "www.creality.com"
#endif

#define PAUSE_HEAT true

#define USE_STRING_HEADINGS

#define MENU_FONT   font8x16
#define STAT_FONT   font10x20
#define HEADER_FONT font10x20

#define MENU_CHAR_LIMIT  24

// Fan speed limit
#define FANON           255
#define FANOFF          0

// Print speed limit
#define MAX_PRINT_SPEED   999
#define MIN_PRINT_SPEED   10

// Temp limits
#if HAS_HOTEND
  #define MAX_E_TEMP    (HEATER_0_MAXTEMP - (HOTEND_OVERSHOOT))
  #define MIN_E_TEMP    HEATER_0_MINTEMP
#endif

#if HAS_HEATED_BED
  #define MIN_BED_TEMP  BED_MINTEMP
#endif

// Feedspeed limit (max feedspeed = DEFAULT_MAX_FEEDRATE * 2)
#define MIN_MAXFEEDSPEED      1
#define MIN_MAXACCELERATION   1
#define MIN_MAXCORNER         0.1
#define MIN_STEP              1

#define FEEDRATE_E      (60)

// Mininum unit (0.1) : multiple (10)
#define MINUNITMULT     10

#define ENCODER_WAIT    20
#define DWIN_SCROLL_UPDATE_INTERVAL 2000
#define DWIN_REMAIN_TIME_UPDATE_INTERVAL 20000

constexpr uint16_t TROWS = 6, MROWS = TROWS - 1,        // Total rows, and other-than-Back
                   TITLE_HEIGHT = 30,                   // Title bar height
                   MLINE = 53,                          // Menu line height
                   LBLX = 60,                           // Menu item label X
                   MENU_CHR_W = 8, STAT_CHR_W = 10;

#define MBASE(L) (49 + (L)*MLINE)

#define BABY_Z_VAR TERN(HAS_BED_PROBE, probe.offset.z, zprobe_zoffset)

/* Value Init */
HMI_value_t HMI_ValueStruct;
HMI_Flag HMI_flag{0};

millis_t Encoder_ms     = 0;
millis_t Wait_ms        = 0;
millis_t dwin_heat_time = 0;

int checkkey = 0, last_checkkey = 0;

typedef struct {
  uint8_t now, last;
  void set(uint8_t v) { now = last = v; }
  void reset() { set(0); }
  bool changed() { bool c = (now != last); if (c) last = now; return c; }
  bool dec() { if (now) now--; return changed(); }
  bool inc(uint8_t v) { if (now < v) now++; else now = v; return changed(); }
} select_t;

select_t select_page{0}, select_file{0}, select_print{0}, select_prepare{0}
         , select_control{0}, select_axis{0}, select_temp{0}, select_motion{0}, select_tune{0}
         , select_PLA{0}, select_ABS{0}
         , select_speed{0}
         , select_acc{0}
         , select_corner{0}
         , select_step{0}
         // , select_leveling{0}
         ;

uint8_t index_file     = MROWS,
        index_prepare  = MROWS,
        index_control  = MROWS,
        index_leveling = MROWS,
        index_tune     = 5;

// char filebuf[50];

uint8_t countbuf = 0;

bool recovery_flag = false, abort_flag = false;

constexpr float default_max_feedrate[]        = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[]    = DEFAULT_MAX_ACCELERATION;
constexpr float default_max_jerk[]            = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
constexpr float default_axis_steps_per_unit[] = DEFAULT_AXIS_STEPS_PER_UNIT;

uint8_t Percentrecord = 0;
uint16_t last_Printtime = 0, remain_time = 0;
float last_temp_hotend_target = 0, last_temp_bed_target = 0;
float last_temp_hotend_current = 0, last_temp_bed_current = 0;
uint8_t last_fan_speed = 0;
uint16_t last_speed = 0;
float last_E_scale = 0;
bool DWIN_lcd_sd_status = 0;
bool pause_action_flag = 0;
int temphot = 0, tempbed = 0;
float zprobe_zoffset = 0;
float last_zoffset = 0, last_probe_zoffset = 0;

#define DWIN_LANGUAGE_EEPROM_ADDRESS 0x01   // Between 0x01 and 0x63 (EEPROM_OFFSET-1)
                                            // BL24CXX::check() uses 0x00

void HMI_SetLanguage(void) {
  BL24CXX::read(DWIN_LANGUAGE_EEPROM_ADDRESS, (uint8_t*)&HMI_flag.language_chinese, sizeof(HMI_flag.language_chinese));
  if (HMI_flag.language_chinese)
    DWIN_JPG_CacheTo1(Language_Chinese);
  else
    DWIN_JPG_CacheTo1(Language_English);
}

void HMI_SetAndSaveLanguageWestern(void) {
  HMI_flag.language_chinese = false;
  DWIN_JPG_CacheTo1(Language_English);
  BL24CXX::write(DWIN_LANGUAGE_EEPROM_ADDRESS, (uint8_t*)&HMI_flag.language_chinese, sizeof(HMI_flag.language_chinese));
}
void HMI_SetAndSaveLanguageChinese(void) {
  HMI_flag.language_chinese = true;
  DWIN_JPG_CacheTo1(Language_Chinese);
  BL24CXX::write(DWIN_LANGUAGE_EEPROM_ADDRESS, (uint8_t*)&HMI_flag.language_chinese, sizeof(HMI_flag.language_chinese));
}

void show_plus_or_minus(uint8_t size, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value) {
  if (value < 0) {
    DWIN_Draw_String(false, true, size, White, bColor, x - 6, y, (char*)"-");
    DWIN_Draw_FloatValue(true, true, 0, size, White, bColor, iNum, fNum, x, y, -value);
  }
  else {
    DWIN_Draw_String(false, true, size, White, bColor, x - 6, y, (char*)" ");
    DWIN_Draw_FloatValue(true, true, 0, size, White, bColor, iNum, fNum, x, y, value);
  }
}

void ICON_Print() {
  if (select_page.now == 0) {
    DWIN_ICON_Show(ICON, ICON_Print_1, 17, 130);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 1, 447, 271 - 243, 479 - 19, 58, 201);
    else
      DWIN_Frame_AreaCopy(1, 1, 451, 271 - 240, 479 - 16, 72 - 15, 201);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Print_0, 17, 130);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 1, 405, 271 - 243, 420, 58, 201);
    else
      DWIN_Frame_AreaCopy(1, 1, 423, 271 - 240, 423 + 12, 72 - 15, 201);
  }
}

void ICON_Prepare() {
  if (select_page.now == 1) {
    DWIN_ICON_Show(ICON, ICON_Prepare_1, 145, 130);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 31, 447, 271 - 213, 479 - 19, 186, 201);
    else
      DWIN_Frame_AreaCopy(1, 33, 451, 271 - 189, 479 - 13, 200 - 25, 201);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Prepare_0, 145, 130);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 31, 405, 271 - 213, 420, 186, 201);
    else
      DWIN_Frame_AreaCopy(1, 33, 423, 271 - 189, 423 + 15, 200 - 25, 201);
  }
}

void ICON_Control() {
  if (select_page.now == 2) {
    DWIN_ICON_Show(ICON, ICON_Control_1, 17, 246);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 61, 447, 271 - 183, 479 - 19, 58, 318);
    else
      DWIN_Frame_AreaCopy(1, 85, 451, 271 - 139, 479 - 16, 72 - 24, 318);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Control_0, 17, 246);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 61, 405, 271 - 183, 420, 58, 318);
    else
      DWIN_Frame_AreaCopy(1, 85, 423, 271 - 139, 479 - 45, 72 - 24, 318);
  }
}

void ICON_StartInfo(bool show) {
  if (show) {
    DWIN_ICON_Show(ICON, ICON_Info_1, 145, 246);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 91, 447, 271 - 153, 479 - 19, 186, 318);
    else
      DWIN_Frame_AreaCopy(1, 132, 451, 159, 479 - 13, 186, 318);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Info_0, 145, 246);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 91, 405, 271 - 153, 420, 186, 318);
    else
      DWIN_Frame_AreaCopy(1, 132, 423, 159, 423 + 12, 186, 318);
  }
}

void ICON_Leveling(bool show) {
  if (show) {
    DWIN_ICON_Show(ICON, ICON_Leveling_1, 145, 246);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 211, 447, 238, 479 - 19, 186, 318);
    else
      DWIN_Frame_AreaCopy(1, 84, 437, 120,  449, 200 - 18, 318);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Leveling_0, 145, 246);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 211, 405, 238, 420, 186, 318);
    else
      DWIN_Frame_AreaCopy(1, 84, 465, 120, 478, 200 - 18, 318);
  }
}

void ICON_Tune() {
  if (select_print.now == 0) {
    DWIN_ICON_Show(ICON, ICON_Setup_1, 8, 252);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 121, 447, 271 - 123, 479 - 21, 34, 325);
    else
      DWIN_Frame_AreaCopy(1, 1, 465, 271 - 237, 479 - 2, 48 - 17, 325);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Setup_0, 8, 252);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 121, 405, 271 - 123, 420, 34, 325);
    else
      DWIN_Frame_AreaCopy(1, 1, 438, 271 - 239, 479 - 31, 48 - 17, 325);
  }
}

void ICON_Pause() {
  if (select_print.now == 1) {
    DWIN_ICON_Show(ICON, ICON_Pause_1, 96, 252);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 181, 447, 271 - 63, 479 - 20, 124, 325);
    else
      DWIN_Frame_AreaCopy(1, 177, 451, 271 - 55, 479 - 17, 136 - 20, 325);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Pause_0, 96, 252);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 181, 405, 271 - 63, 420, 124, 325);
    else
      DWIN_Frame_AreaCopy(1, 177, 423, 271 - 56, 479 - 46, 136 - 20, 325);
  }
}

void ICON_Continue() {
  if (select_print.now == 1) {
    DWIN_ICON_Show(ICON, ICON_Continue_1, 96, 252);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 1, 447, 271 - 243, 479 - 19, 124, 325);
    else
      DWIN_Frame_AreaCopy(1, 1, 451, 271 - 239, 479 - 16, 136 - 15, 325);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Continue_0, 96, 252);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 1, 405, 271 - 243, 420, 124, 325);
    else
      DWIN_Frame_AreaCopy(1, 1, 424, 271 - 240, 479 - 45, 136 - 15, 325);
  }
}

void ICON_Stop() {
  if (select_print.now == 2) {
    DWIN_ICON_Show(ICON, ICON_Stop_1, 184, 252);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 151, 447, 271 - 93, 479 - 20, 210, 325);
    else
      DWIN_Frame_AreaCopy(1, 218, 451, 271 - 22, 479 - 14, 224 - 15, 325);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Stop_0, 184, 252);
    if (HMI_flag.language_chinese)
      DWIN_Frame_AreaCopy(1, 151, 405, 271 - 93, 420, 210, 325);
    else
      DWIN_Frame_AreaCopy(1, 218, 423, 271 - 24, 479 - 43, 224 - 15, 325);
  }
}

inline void Clear_Title_Bar(void) {
  DWIN_Draw_Rectangle(1, Background_blue,   0,  0,  DWIN_WIDTH,  30);
}

inline void Draw_Title(const char * const title) {
  DWIN_Draw_String(false, false, HEADER_FONT, White, Background_blue, 14, 4, (char*)title);
}

inline void Draw_Title(const __FlashStringHelper * title) {
  DWIN_Draw_String(false, false, HEADER_FONT, White, Background_blue, 14, 4, (char*)title);
}

inline void Clear_Menu_Area(void) {
  DWIN_Draw_Rectangle(1, Background_black,  0,  31, 272,  360);
}

inline void Clear_Main_Window(void) {
  Clear_Title_Bar();
  Clear_Menu_Area();
}

inline void Clear_Popup_Area(void) {
  Clear_Title_Bar();
  DWIN_Draw_Rectangle(1, Background_black,  0,  31, 272,  480);
}

void Draw_Popup_Bkgd_105(void) {
  DWIN_Draw_Rectangle(1, Background_window, 14, 105, 271 - 13, 479 - 105);
}

inline void Draw_More_Icon(const uint8_t line) {
  DWIN_ICON_Show(ICON, ICON_More, 226, 46 + line * MLINE);
}

inline void Draw_Menu_Cursor(const uint8_t line) {
  // DWIN_ICON_Show(ICON,ICON_Rectangle, 0, 31 + line * MLINE);
  DWIN_Draw_Rectangle(1, Rectangle_Color, 0, 31 + line * MLINE, 14, 31 + (line + 1) * MLINE - 2);
}

inline void Erase_Menu_Cursor(const uint8_t line) {
  DWIN_Draw_Rectangle(1, Background_black, 0, 31 + line * MLINE, 14, 31 + (line + 1) * MLINE - 2);
}

inline void Move_Highlight(const int16_t from, const uint16_t newline) {
  Erase_Menu_Cursor(newline - from);
  Draw_Menu_Cursor(newline);
}

inline void Add_Menu_Line() {
  Move_Highlight(1, MROWS);
  DWIN_Draw_Line(Line_Color, 16, 82 + MROWS * MLINE, 256, 83 + MROWS * MLINE);
}

inline void Scroll_Menu(const uint8_t dir) {
  DWIN_Frame_AreaMove(1, dir, MLINE, Background_black, 0, 31, 272, 349);
  switch (dir) {
    case DWIN_SCROLL_DOWN: Move_Highlight(-1, 0); break;
    case DWIN_SCROLL_UP:   Add_Menu_Line(); break;
  }
}

inline uint16_t nr_sd_menu_items() {
  return card.get_num_Files() + !card.flag.workDirIsRoot;
}

inline void Draw_Menu_Icon(const uint8_t line, const uint8_t icon) {
  DWIN_ICON_Show(ICON, icon, 26, 46 + line * MLINE);
}

inline void Erase_Menu_Text(const uint8_t line) {
  DWIN_Draw_Rectangle(1, Background_black, LBLX, 31 + line * MLINE + 4, 271, 28 + (line + 1) * MLINE - 4);
}

inline void Draw_Menu_Line(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr) {
  if (label) DWIN_Draw_String(false, false, font8x16, White, Background_black, LBLX, 48 + line * MLINE, (char*)label);
  if (icon) Draw_Menu_Icon(line, icon);
  DWIN_Draw_Line(Line_Color, 16, 29 + (line + 1) * MLINE, 256, 30 + (line + 1) * MLINE);
}

// The "Back" label is always on the first line
inline void Draw_Back_Label(void) {
  if (HMI_flag.language_chinese)
    DWIN_Frame_AreaCopy(1, 129, 72, 271 - 115, 479 - 395, LBLX, MBASE(0));
  else
    DWIN_Frame_AreaCopy(1, 226, 179, 271 - 15, 479 - 290, LBLX, MBASE(0));
}

// Draw "Back" line at the top
inline void Draw_Back_First(const bool is_sel=true) {
  Draw_Menu_Line(0, ICON_Back);
  Draw_Back_Label();
  if (is_sel) Draw_Menu_Cursor(0);
}

//
// Draw Menus
//

inline void draw_move_en(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 69, 61, 271 - 169, 479 - 408, LBLX, line); // "Move"
}

inline void Prepare_Item_Move(const uint8_t row) {
  if (HMI_flag.language_chinese)
    DWIN_Frame_AreaCopy(1, 159, 70, 271 - 71, 479 - 395, LBLX, MBASE(row));
  else
    draw_move_en(MBASE(row)); // "Move >"
  Draw_Menu_Line(row, ICON_Axis);
  Draw_More_Icon(row);
}

inline void Prepare_Item_Disable(const uint8_t row) {
  if (HMI_flag.language_chinese)
    DWIN_Frame_AreaCopy(1, 204, 70, 271 - 12, 479 - 397, LBLX, MBASE(row));
  else
    DWIN_Frame_AreaCopy(1, 103, 59, 271 - 71, 479 - 405, LBLX, MBASE(row)); // "Disable Stepper"
  Draw_Menu_Line(row, ICON_CloseMotor);
}

inline void Prepare_Item_Home(const uint8_t row) {
  if (HMI_flag.language_chinese)
    DWIN_Frame_AreaCopy(1, 0, 89, 271 - 230, 479 - 378, LBLX, MBASE(row));
  else
    DWIN_Frame_AreaCopy(1, 202, 61, 271 - 0, 479 - 408, LBLX, MBASE(row)); // "Auto Home"
  Draw_Menu_Line(row, ICON_Homing);
}

inline void Prepare_Item_Offset(const uint8_t row) {
  if (HMI_flag.language_chinese) {
    #if HAS_BED_PROBE
      DWIN_Frame_AreaCopy(1, 174, 164, 271 - 48, 479 - 302, LBLX, MBASE(row));
      show_plus_or_minus(font8x16, Background_black, 2, 2, 202, MBASE(row), probe.offset.z * 100);
    #else
      DWIN_Frame_AreaCopy(1, 43, 89, 271 - 173, 479 - 378, LBLX, MBASE(row));
    #endif
  }
  else {
    #if HAS_BED_PROBE
      DWIN_Frame_AreaCopy(1, 93, 179, 271 - 130, 479 - 290, LBLX, MBASE(row)); // "Z-Offset"
      show_plus_or_minus(font8x16, Background_black, 2, 2, 202, MBASE(row), probe.offset.z * 100);
    #else
      DWIN_Frame_AreaCopy(1, 1, 76, 271 - 165, 479 - 393, LBLX, MBASE(row)); // "..."
    #endif
  }
  Draw_Menu_Line(row, ICON_SetHome);
}

inline void Prepare_Item_PLA(const uint8_t row) {
  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 100, 89, 271 - 93 - 27, 479 - 378, LBLX, MBASE(row));
  }
  else {
    DWIN_Frame_AreaCopy(1, 107, 76, 271 - 115, 479 - 393, LBLX, MBASE(row)); // "Preheat"
    DWIN_Frame_AreaCopy(1, 157, 76, 181, 479 - 393, LBLX + 49 + 3, MBASE(row)); // "PLA"
  }
  Draw_Menu_Line(row, ICON_PLAPreheat);
}

inline void Prepare_Item_ABS(const uint8_t row) {
  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 180, 89, 271 - 11 - 27, 479 - 379, LBLX, MBASE(row));
  }
  else {
    DWIN_Frame_AreaCopy(1, 107, 76, 271 - 115, 479 - 393, LBLX, MBASE(row)); // "Preheat"
    DWIN_Frame_AreaCopy(1, 172, 76, 198, 479 - 393, LBLX + 49 + 3, MBASE(row)); // "ABS"
  }
  Draw_Menu_Line(row, ICON_ABSPreheat);
}

inline void Prepare_Item_Cool(const uint8_t row) {
  if (HMI_flag.language_chinese)
    DWIN_Frame_AreaCopy(1, 1, 104, 271 - 215, 479 - 362, LBLX, MBASE(row));
  else
    DWIN_Frame_AreaCopy(1, 200,  76, 271 - 7, 479 - 393, LBLX, MBASE(row));// "Cooldown"
  Draw_Menu_Line(row, ICON_Cool);
}

inline void Prepare_Item_Lang(const uint8_t row) {
  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 239, 134, 271 - 5, 479 - 333, LBLX, MBASE(row));
    DWIN_Draw_String(false, false, font8x16, White, Background_black, 226, MBASE(row), (char*)"CN");
  }
  else {
    DWIN_Frame_AreaCopy(1, 0, 194, 271 - 150, 479 - 272, LBLX, MBASE(row)); // "Language selection"
    DWIN_Draw_String(false, false, font8x16, White, Background_black, 226, MBASE(row), (char*)"EN");
  }
  Draw_Menu_Icon(row, ICON_Language);
}

inline void Draw_Prepare_Menu() {
  Clear_Main_Window();

  const int16_t scroll = MROWS - index_prepare; // Scrolled-up lines
  #define PSCROL(L) (scroll + (L))
  #define PVISI(L)  WITHIN(PSCROL(L), 0, MROWS)

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 133, 1, 271 - 111, 479 - 465 - 1, 14, 8); // "Prepare"
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_PREPARE));
    #else
      DWIN_Frame_AreaCopy(1, 178, 2, 271 - 42, 479 - 464 - 1, 14, 8); // "Prepare"
    #endif
  }

  if (PVISI(0)) Draw_Back_First(select_prepare.now == 0); // < Back
  if (PVISI(1)) Prepare_Item_Move(PSCROL(1));       // Move >
  if (PVISI(2)) Prepare_Item_Disable(PSCROL(2));    // Disable Stepper
  if (PVISI(3)) Prepare_Item_Home(PSCROL(3));       // Auto Home
  if (PVISI(4)) Prepare_Item_Offset(PSCROL(4));     // Z-Offset
  if (PVISI(5)) Prepare_Item_PLA(PSCROL(5));        // Preheat PLA
  if (PVISI(6)) Prepare_Item_ABS(PSCROL(6));        // Preheat ABS
  if (PVISI(7)) Prepare_Item_Cool(PSCROL(7));       // Cooldown
  if (PVISI(8)) Prepare_Item_Lang(PSCROL(8));       // Language CN/EN

  if (select_prepare.now) Draw_Menu_Cursor(PSCROL(select_prepare.now));
}

inline void Draw_Control_Menu() {
  Clear_Main_Window();

  const int16_t scroll = TERN(HAS_ONESTEP_LEVELING, MROWS - index_control, 0); // Scrolled-up lines

  #define CSCROL(L) (scroll + (L))
  #define CLINE(L)  MBASE(CSCROL(L))
  #define CVISI(L)  WITHIN(CSCROL(L), 0, MROWS)

  if (CVISI(0)) Draw_Back_First(select_control.now == 0);                        // < Back

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 103,   1, 271 - 141, 479 - 465, 14, 8);
    DWIN_Frame_AreaCopy(1,  57, 104, 271 - 187, 479 - 363, LBLX, CLINE(1));  // Temperature >
    DWIN_Frame_AreaCopy(1,  87, 104, 271 - 157, 479 - 363, LBLX, CLINE(2));  // Motion >
    DWIN_Frame_AreaCopy(1, 117, 104, 271 - 99, 479 - 363, LBLX, CLINE(3));   // Store Config
    DWIN_Frame_AreaCopy(1, 174, 103, 271 - 42, 479 - 363, LBLX, CLINE(4));   // Read Config
    DWIN_Frame_AreaCopy(1,   1, 118, 271 - 215, 479 - 348, LBLX, CLINE(5));  // Reset Config

    if (CVISI(6))
      DWIN_Frame_AreaCopy(1, 231, 104, 271 - 13, 479 - 363, LBLX, CLINE(6)); // Info >
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_CONTROL));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)GET_TEXT_F(MSG_TEMPERATURE));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)GET_TEXT_F(MSG_MOTION));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)GET_TEXT_F(MSG_STORE_EEPROM));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)GET_TEXT_F(MSG_LOAD_EEPROM));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 4), (char*)GET_TEXT_F(MSG_RESTORE_DEFAULTS));
      if (CVISI(6))
          DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 5), (char*)"Info");
    #else
      DWIN_Frame_AreaCopy(1, 128, 2, 271 - 95, 479 - 467, 14, 8);
      DWIN_Frame_AreaCopy(1,   1,    89, 271 - 188, 479 - 377 - 1, LBLX, CLINE(1));// Temperature >
      DWIN_Frame_AreaCopy(1,  84,    89, 271 - 143, 479 - 380,   LBLX, CLINE(2));// Motion >
      DWIN_Frame_AreaCopy(1, 131 + 17, 89, 271 -  3, 479 - 377 - 1, LBLX, CLINE(3));// "Store Configuration"
      DWIN_Frame_AreaCopy(1, 26, 104, 271 - 214, 479 - 365, LBLX, CLINE(4));   // "Read"
      DWIN_Frame_AreaCopy(1, 131 + 51, 89, 271 - 3, 479 - 377 - 1, LBLX + 31 + 3, CLINE(4)); // "Configuration"
      DWIN_Frame_AreaCopy(1, 59, 104, 271 - 178, 479 - 365, LBLX, CLINE(5));   // "Reset"
      DWIN_Frame_AreaCopy(1, 131 + 51, 89, 271 - 3, 479 - 377 - 1, LBLX + 34 + 3, CLINE(5)); // "Configuration"
      if (CVISI(6))
        DWIN_Frame_AreaCopy(1, 0, 104, 25, 115, LBLX, CLINE(6));               // Info >
    #endif
  }

  if (select_control.now && CVISI(select_control.now))
    Draw_Menu_Cursor(CSCROL(select_control.now));

  // Draw icons and lines
  LOOP_L_N(i, 6)
  if (CVISI(i + 1)) Draw_Menu_Line(CSCROL(i + 1), ICON_Temperature + i);

  Draw_More_Icon(CSCROL(1));
  Draw_More_Icon(CSCROL(2));
  if (CVISI(6)) Draw_More_Icon(CSCROL(6));
}

inline void Draw_Tune_Menu() {
  Clear_Main_Window();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 73, 2, 271 - 171, 479 - 466, 14, 9);
    DWIN_Frame_AreaCopy(1, 116, 164, 271 - 100, 479 - 303, LBLX, MBASE(1));
    DWIN_Frame_AreaCopy(1, 1, 134, 271 - 215, 479 - 333, LBLX, MBASE(2));
    DWIN_Frame_AreaCopy(1, 58, 134, 271 - 158, 479 - 333, LBLX, MBASE(3));
    DWIN_Frame_AreaCopy(1, 115, 134, 271 - 101, 479 - 333, LBLX, MBASE(4));
    DWIN_Frame_AreaCopy(1, 174, 164, 271 - 48, 479 - 302, LBLX, MBASE(5));
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_TUNE));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)GET_TEXT_F(MSG_SPEED));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)GET_TEXT_F(MSG_UBL_SET_TEMP_BED));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)GET_TEXT_F(MSG_FAN_SPEED));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 4), (char*)GET_TEXT_F(MSG_ZPROBE_ZOFFSET));
    #else
      DWIN_Frame_AreaCopy(1, 94, 2, 271 - 145, 479 - 467, 14, 9);
      DWIN_Frame_AreaCopy(1, 1, 179, 271 - 179, 479 - 287 - 2, LBLX, MBASE(1)); // print speed
      DWIN_Frame_AreaCopy(1, 197, 104, 271 - 33, 479 - 365, LBLX, MBASE(2)); // Hotend...
      DWIN_Frame_AreaCopy(1, 1, 89, 271 - 188, 479 - 377 - 1, LBLX + 41 + 3, MBASE(2)); // ...Temperature
      DWIN_Frame_AreaCopy(1, 240, 104, 271 - 7, 479 - 365, LBLX, MBASE(3)); // Bed...
      DWIN_Frame_AreaCopy(1, 1, 89, 271 - 188, 479 - 377 - 1, LBLX + 24 + 3, MBASE(3)); // ...Temperature
      DWIN_Frame_AreaCopy(1, 0, 119, 271 - 207, 479 - 347, LBLX, MBASE(4)); // fan speed
      DWIN_Frame_AreaCopy(1, 93, 179, 271 - 130, 479 - 290, LBLX, MBASE(5)); // Z-offset
    #endif
  }

  Draw_Back_First(select_tune.now == 0);
  if (select_tune.now) Draw_Menu_Cursor(select_tune.now);

  Draw_Menu_Line(1, ICON_Speed);
  Draw_Menu_Line(2, ICON_HotendTemp);
  Draw_Menu_Line(3, ICON_BedTemp);
  Draw_Menu_Line(4, ICON_FanSpeed);
  Draw_Menu_Line(5, ICON_Zoffset);

  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(1), feedrate_percentage);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2), thermalManager.temp_hotend[0].target);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3), thermalManager.temp_bed.target);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(4), thermalManager.fan_speed[0]);
  show_plus_or_minus(font8x16, Background_black, 2, 2, 202, MBASE(5), BABY_Z_VAR * 100);
}

inline void draw_max_en(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 245, 119, 271 - 2, 479 - 350, LBLX, line);   // "Max"
}
inline void draw_max_accel_en(const uint16_t line) {
  draw_max_en(line);
  DWIN_Frame_AreaCopy(1, 1, 135, 271 - 192, 479 - 334, LBLX + 24 + 3, line); // "Acceleration"
}
inline void draw_speed_en(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 184, 119, 224, 479 - 347, LBLX + inset, line); // "Speed"
}
inline void draw_corner_en(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 64, 119, 271 - 165, 479 - 350, LBLX + 24 + 3, line); // "Corner"
}
inline void draw_steps_per_mm(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 1, 151, 271 - 170, 479 - 318, LBLX, line);   // "Steps-per-mm"
}
inline void say_x(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 95, 104, 271 - 169, 479 - 365, LBLX + inset, line); // "X"
}
inline void say_y(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 104, 104, 271 - 161, 479 - 365, LBLX + inset, line); // "Y"
}
inline void say_z(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 112, 104, 271 - 151, 479 - 365, LBLX + inset, line); // "Z"
}
inline void say_e(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 237, 119, 271 - 27, 479 - 350, LBLX + inset, line); // "E"
}

inline void Draw_Motion_Menu() {
  Clear_Main_Window();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 1, 16, 271 - 243, 479 - 451, 14, 8);

    DWIN_Frame_AreaCopy(1, 173, 133, 228, 479 - 332, LBLX, MBASE(1));     // max speed
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(2));     // max...
    DWIN_Frame_AreaCopy(1, 28, 149, 271 - 202, 479 - 318, LBLX + 27, MBASE(2) + 1); // ...acceleration
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(3));     // max...
    DWIN_Frame_AreaCopy(1, 1, 180, 271 - 243, 479 - 287, LBLX + 27, MBASE(3) + 1); // ...
    DWIN_Frame_AreaCopy(1, 202, 133, 228, 479 - 332, LBLX + 54, MBASE(3)); // ...jerk
    DWIN_Frame_AreaCopy(1, 153, 148, 271 - 77, 479 - 318, LBLX, MBASE(4)); // flow ratio
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MOTION));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)"Feedrate");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)GET_TEXT_F(MSG_ACCELERATION));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)GET_TEXT_F(MSG_JERK));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)GET_TEXT_F(MSG_STEPS_PER_MM));
    #else
      DWIN_Frame_AreaCopy(1, 144, 16, 271 - 82, 479 - 453, 14, 8);
      draw_max_en(MBASE(1)); draw_speed_en(24 + 3, MBASE(1));               // "Max Speed"
      draw_max_accel_en(MBASE(2));                                          // "Max Acceleration"
      draw_max_en(MBASE(3)); draw_corner_en(MBASE(3));                      // "Max Corner"
      draw_steps_per_mm(MBASE(4));                                          // "Steps-per-mm"
    #endif
  }

  Draw_Back_First(select_motion.now == 0);
  if (select_motion.now) Draw_Menu_Cursor(select_motion.now);

  LOOP_L_N(i, 4) Draw_Menu_Line(i + 1, ICON_MaxSpeed + i);

  Draw_More_Icon(1);
  Draw_More_Icon(2);
  Draw_More_Icon(3);
  Draw_More_Icon(4);
}

//
// Draw Popup Windows
//

void Popup_Window_Temperature(const bool toohigh) {
  Clear_Popup_Area();
  Draw_Popup_Bkgd_105();
  if (toohigh) {
    DWIN_ICON_Show(ICON, ICON_TempTooHigh, 102, 165);
    if (HMI_flag.language_chinese) {
      DWIN_Frame_AreaCopy(1, 103, 371, 237, 479 - 93, 52, 285);
      DWIN_Frame_AreaCopy(1, 151, 389, 185, 402, 187, 285);
      DWIN_Frame_AreaCopy(1, 189, 389, 271 - 0, 402, 95, 310);
    }
    else {
      DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 36, 300, (char*)"Nozzle or Bed temperature");
      DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 92, 300, (char*)"is too high");
    }
  }
  else {
    DWIN_ICON_Show(ICON, ICON_TempTooLow, 102, 165);
    if (HMI_flag.language_chinese) {
      DWIN_Frame_AreaCopy(1, 103, 371, 271 - 1, 479 - 93, 52, 285);
      DWIN_Frame_AreaCopy(1, 189, 389, 271 - 0, 402, 95, 310);
    }
    else {
      DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 36, 300, (char*)"Nozzle or Bed temperature");
      DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 92, 300, (char*)"is too low");
    }
  }
}

inline void Draw_Popup_Bkgd_60() {
  DWIN_Draw_Rectangle(1, Background_window, 14, 60, 271 - 13, 330);
}

#if HAS_HOTEND

  void Popup_Window_ETempTooLow(void) {
    Clear_Main_Window();
    Draw_Popup_Bkgd_60();
    DWIN_ICON_Show(ICON, ICON_TempTooLow, 102, 105);
    if (HMI_flag.language_chinese) {
      DWIN_Frame_AreaCopy(1, 103, 371, 136, 479 - 93, 69, 240);
      DWIN_Frame_AreaCopy(1, 170, 371, 271 - 1, 479 - 93, 69 + 33, 240);
      DWIN_ICON_Show(ICON, ICON_Confirm_C, 86, 280);
    }
    else {
      DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 20, 235, (char*)"Nozzle is too cold");
      DWIN_ICON_Show(ICON, ICON_Confirm_E, 86, 280);
    }
  }

#endif

void Popup_Window_Resume(void) {
  Clear_Popup_Area();
  Draw_Popup_Bkgd_105();
  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 160, 338, 271 - 36, 479 - 125, 98, 135);
    DWIN_Frame_AreaCopy(1, 103, 321, 271 - 0, 479 - 144, 52, 192);
    DWIN_ICON_Show(ICON, ICON_Continue_C, 26, 307);
    DWIN_ICON_Show(ICON, ICON_Cancel_C, 146, 307);
  }
  else {
    DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 120, 115, (char*)"Tips");
    DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 24, 192, (char*)"I see the file stopped");
    DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 68, 212, (char*)"unexpectedly last time");
    DWIN_ICON_Show(ICON, ICON_Continue_E, 26, 307);
    DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 307);
  }
}

void Popup_Window_Home(void) {
  Clear_Main_Window();
  Draw_Popup_Bkgd_60();
  DWIN_ICON_Show(ICON, ICON_BLTouch, 101, 105);
  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 0, 371, 33, 386, 85, 240);
    DWIN_Frame_AreaCopy(1, 203, 286, 271, 302, 118, 240);
    DWIN_Frame_AreaCopy(1, 0, 389, 150, 402, 61, 280);
  }
  else {
    DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 80, 230, (char*)GET_TEXT_F(MSG_LEVEL_BED_HOMING));
    DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 24, 260, (char*)"Please wait until completed");
  }
}

void Popup_Window_Leveling(void) {
  Clear_Main_Window();
  Draw_Popup_Bkgd_60();
  DWIN_ICON_Show(ICON, ICON_AutoLeveling, 101, 105);
  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 0, 371, 100, 386, 84, 240);
    DWIN_Frame_AreaCopy(1, 0, 389, 150, 402, 61, 280);
  }
  else {
    DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 76, 230, (char*)GET_TEXT_F(MSG_BED_LEVELING));
    DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 24, 260, (char*)"Please wait until completed");
  }
}

void Draw_Select_Highlight(const bool sel) {
  HMI_flag.select_flag = sel;
  const uint16_t c1 = sel ? Select_Color : Background_window,
                 c2 = sel ? Background_window : Select_Color;
  DWIN_Draw_Rectangle(0, c1, 25, 279, 126, 318);
  DWIN_Draw_Rectangle(0, c1, 24, 278, 127, 319);
  DWIN_Draw_Rectangle(0, c2, 145, 279, 246, 318);
  DWIN_Draw_Rectangle(0, c2, 144, 278, 247, 319);
}

void Popup_window_PauseOrStop(void) {
  Clear_Main_Window();
  Draw_Popup_Bkgd_60();
  if (HMI_flag.language_chinese) {
    if (select_print.now == 1) DWIN_Frame_AreaCopy(1, 237, 338, 269, 356, 98, 150);
    else if (select_print.now == 2) DWIN_Frame_AreaCopy(1, 221, 320, 253, 336, 98, 150);
    DWIN_Frame_AreaCopy(1, 220, 304, 264, 319, 130, 150);
    DWIN_ICON_Show(ICON, ICON_Confirm_C, 26, 280);
    DWIN_ICON_Show(ICON, ICON_Cancel_C, 146, 280);
  }
  else {
    if (select_print.now == 1) DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 88, 150, (char*)GET_TEXT_F(MSG_PAUSE_PRINT));
    else if (select_print.now == 2) DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, 92, 150, (char*)GET_TEXT_F(MSG_STOP_PRINT));
    DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
    DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
  }
  Draw_Select_Highlight(true);
}

void Draw_Printing_Screen(void) {
  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 30,  1, 271 - 200, 479 - 465,  14,   9);// Tune
    DWIN_Frame_AreaCopy(1,  0, 72, 271 - 208, 479 - 393,  41, 188);// Pause
    DWIN_Frame_AreaCopy(1, 65, 72, 271 - 143, 479 - 393, 176, 188); // Stop
  }
  else {
    DWIN_Frame_AreaCopy(1, 40,  2, 271 - 179, 479 - 464 - 1,  14,   9);// Tune
    DWIN_Frame_AreaCopy(1,  0, 44, 271 - 175, 479 - 420 - 1,  41, 188);// Pause
    DWIN_Frame_AreaCopy(1, 98, 44, 271 - 119, 479 - 420 - 1, 176, 188); // Stop
  }
}

void Draw_Print_ProgressBar() {
  DWIN_ICON_Show(ICON, ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, BarFill_Color, 16 + Percentrecord * 240 / 100, 93, 256, 113);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Percent_Color, Background_black, 2, 117, 133, Percentrecord);
  DWIN_Draw_String(false, false, font8x16, Percent_Color, Background_black, 117 + 16, 133, (char*)"%");
}

void Draw_Print_ProgressElapsed() {
  duration_t elapsed = print_job_timer.duration(); // print timer
  DWIN_Draw_IntValue(true, true, 1, font8x16, White, Background_black, 2, 42, 212, elapsed.value / 3600);
  DWIN_Draw_String(false, false, font8x16, White, Background_black, 42 + 16, 212, (char*)":");
  DWIN_Draw_IntValue(true, true, 1, font8x16, White, Background_black, 2, 42 + 24, 212, (elapsed.value % 3600) / 60);
}

void Draw_Print_ProgressRemain() {
  DWIN_Draw_IntValue(true, true, 1, font8x16, White, Background_black, 2, 176, 212, remain_time / 3600);
  DWIN_Draw_String(false, false, font8x16, White, Background_black, 176 + 16, 212, (char*)":");
  DWIN_Draw_IntValue(true, true, 1, font8x16, White, Background_black, 2, 176 + 24, 212, (remain_time % 3600) / 60);
}

void Goto_PrintProcess(void) {
  checkkey = PrintProcess;

  Clear_Main_Window();
  Draw_Printing_Screen();

  ICON_Tune();
  if (printingIsPaused()) ICON_Continue(); else ICON_Pause();
  ICON_Stop();

  // Copy into filebuf string before entry
  char * const name = card.longest_filename();
  const int8_t npos = _MAX(0, DWIN_WIDTH - strlen(name) * MENU_CHR_W) / 2;
  DWIN_Draw_String(false, false, font8x16, White, Background_black, npos, 60, name);

  DWIN_ICON_Show(ICON, ICON_PrintTime, 17, 193);
  DWIN_ICON_Show(ICON, ICON_RemainTime, 150, 191);

  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();
}

void Goto_MainMenu(void) {
  checkkey = MainMenu;

  Clear_Main_Window();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 2, 2, 271 - 244, 479 - 465, 14, 9); // "Home"
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MAIN));
    #else
      DWIN_Frame_AreaCopy(1, 0, 2, 271 - 232, 479 - 467, 14, 9);
    #endif
  }

  DWIN_ICON_Show(ICON, ICON_LOGO, 71, 52);

  ICON_Print();
  ICON_Prepare();
  ICON_Control();
  TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(select_page.now == 3);
}

inline ENCODER_DiffState get_encoder_state() {
  const millis_t ms = millis();
  if (PENDING(ms, Encoder_ms)) return ENCODER_DIFF_NO;
  const ENCODER_DiffState state = Encoder_ReceiveAnalyze();
  if (state != ENCODER_DIFF_NO) Encoder_ms = ms + ENCODER_WAIT;
  return state;
}

void HMI_Move_X(void) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (encoder_diffState == ENCODER_DIFF_CW) {
      HMI_ValueStruct.Move_X_scale += EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      HMI_ValueStruct.Move_X_scale -= EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      checkkey = AxisMove;
      EncoderRate.encoderRateEnabled = 0;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 216, MBASE(1), HMI_ValueStruct.Move_X_scale);
      if (!planner.is_full()) {
        // Wait for planner moves to finish!
        planner.synchronize();
        planner.buffer_line(current_position, MMM_TO_MMS(HOMING_FEEDRATE_XY), active_extruder);
      }
      DWIN_UpdateLCD();
      return;
    }
    NOLESS(HMI_ValueStruct.Move_X_scale, (X_MIN_POS) * MINUNITMULT);
    NOMORE(HMI_ValueStruct.Move_X_scale, (X_MAX_POS) * MINUNITMULT);
    current_position[X_AXIS] = HMI_ValueStruct.Move_X_scale / 10;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 216, MBASE(1), HMI_ValueStruct.Move_X_scale);
    DWIN_UpdateLCD();
  }
}

void HMI_Move_Y(void) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (encoder_diffState == ENCODER_DIFF_CW) {
      HMI_ValueStruct.Move_Y_scale += EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      HMI_ValueStruct.Move_Y_scale -= EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      checkkey = AxisMove;
      EncoderRate.encoderRateEnabled = 0;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 216, MBASE(2), HMI_ValueStruct.Move_Y_scale);
      if (!planner.is_full()) {
        // Wait for planner moves to finish!
        planner.synchronize();
        planner.buffer_line(current_position, MMM_TO_MMS(HOMING_FEEDRATE_XY), active_extruder);
      }
      DWIN_UpdateLCD();
      return;
    }
    NOLESS(HMI_ValueStruct.Move_Y_scale, (Y_MIN_POS) * MINUNITMULT);
    NOMORE(HMI_ValueStruct.Move_Y_scale, (Y_MAX_POS) * MINUNITMULT);
    current_position[Y_AXIS] = HMI_ValueStruct.Move_Y_scale / 10;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 216, MBASE(2), HMI_ValueStruct.Move_Y_scale);
    DWIN_UpdateLCD();
  }
}

void HMI_Move_Z(void) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (encoder_diffState == ENCODER_DIFF_CW) {
      HMI_ValueStruct.Move_Z_scale += EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      HMI_ValueStruct.Move_Z_scale -= EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      checkkey = AxisMove;
      EncoderRate.encoderRateEnabled = 0;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 216, MBASE(3), HMI_ValueStruct.Move_Z_scale);
      if (!planner.is_full()) {
        // Wait for planner moves to finish!
        planner.synchronize();
        planner.buffer_line(current_position, MMM_TO_MMS(HOMING_FEEDRATE_Z), active_extruder);
      }
      DWIN_UpdateLCD();
      return;
    }
    NOLESS(HMI_ValueStruct.Move_Z_scale, Z_MIN_POS * MINUNITMULT);
    NOMORE(HMI_ValueStruct.Move_Z_scale, Z_MAX_POS * MINUNITMULT);
    current_position[Z_AXIS] = HMI_ValueStruct.Move_Z_scale / 10;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 216, MBASE(3), HMI_ValueStruct.Move_Z_scale);
    DWIN_UpdateLCD();
  }
}

#if EXTRUDERS

  void HMI_Move_E(void) {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (encoder_diffState == ENCODER_DIFF_CW) {
        HMI_ValueStruct.Move_E_scale += EncoderRate.encoderMoveValue;
      }
      else if (encoder_diffState == ENCODER_DIFF_CCW) {
        HMI_ValueStruct.Move_E_scale -= EncoderRate.encoderMoveValue;
      }
      else if (encoder_diffState == ENCODER_DIFF_ENTER) {
        checkkey = AxisMove;
        EncoderRate.encoderRateEnabled = 0;
        last_E_scale = HMI_ValueStruct.Move_E_scale;
        show_plus_or_minus(font8x16, Background_black, 3, 1, 216, MBASE(4), HMI_ValueStruct.Move_E_scale);
        if (!planner.is_full()) {
          planner.synchronize(); // Wait for planner moves to finish!
          planner.buffer_line(current_position, MMM_TO_MMS(FEEDRATE_E), active_extruder);
        }
        DWIN_UpdateLCD();
        return;
      }
      if ((HMI_ValueStruct.Move_E_scale - last_E_scale) > (EXTRUDE_MAXLENGTH) * MINUNITMULT)
        HMI_ValueStruct.Move_E_scale = last_E_scale + (EXTRUDE_MAXLENGTH) * MINUNITMULT;
      else if ((last_E_scale - HMI_ValueStruct.Move_E_scale) > (EXTRUDE_MAXLENGTH) * MINUNITMULT)
        HMI_ValueStruct.Move_E_scale = last_E_scale - (EXTRUDE_MAXLENGTH) * MINUNITMULT;
      current_position.e = HMI_ValueStruct.Move_E_scale / 10;
      show_plus_or_minus(font8x16, Select_Color, 3, 1, 216, MBASE(4), HMI_ValueStruct.Move_E_scale);
      DWIN_UpdateLCD();
    }
  }

#endif

void HMI_Zoffset(void) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    last_zoffset = zprobe_zoffset;
    if (encoder_diffState == ENCODER_DIFF_CW) {
      HMI_ValueStruct.offset_value += EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      HMI_ValueStruct.offset_value -= EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      EncoderRate.encoderRateEnabled = 0;
      zprobe_zoffset                 = HMI_ValueStruct.offset_value / 100;
      #if HAS_BED_PROBE
        if (WITHIN(zprobe_zoffset - last_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
          probe.offset.z = zprobe_zoffset;
        settings.save();
      #elif ENABLED(BABYSTEPPING)
        babystep.add_mm(Z_AXIS, (zprobe_zoffset - last_zoffset));
      #else
        UNUSED(zprobe_zoffset - last_zoffset);
      #endif

      if (HMI_ValueStruct.show_mode == -4) {
        checkkey = Prepare;
        show_plus_or_minus(font8x16, Background_black, 2, 2, 202, MBASE(4 + MROWS - index_prepare), TERN(HAS_BED_PROBE, probe.offset.z * 100, HMI_ValueStruct.offset_value));
      }
      else {
        checkkey = Tune;
        show_plus_or_minus(font8x16, Background_black, 2, 2, 202, MBASE(5 + MROWS - index_tune), TERN(HAS_BED_PROBE, probe.offset.z * 100, HMI_ValueStruct.offset_value));
      }
      DWIN_UpdateLCD();
      return;
    }
    NOLESS(HMI_ValueStruct.offset_value, (Z_PROBE_OFFSET_RANGE_MIN) * 100);
    NOMORE(HMI_ValueStruct.offset_value, (Z_PROBE_OFFSET_RANGE_MAX) * 100);
    if (HMI_ValueStruct.show_mode == -4)
      show_plus_or_minus(font8x16, Select_Color, 2, 2, 202, MBASE(4 + MROWS - index_prepare), HMI_ValueStruct.offset_value);
    else
      show_plus_or_minus(font8x16, Select_Color, 2, 2, 202, MBASE(5 + MROWS - index_tune), HMI_ValueStruct.offset_value);
    DWIN_UpdateLCD();
  }
}

#if HAS_HOTEND

  void HMI_ETemp(void) {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (encoder_diffState == ENCODER_DIFF_CW) {
        HMI_ValueStruct.E_Temp += EncoderRate.encoderMoveValue;
      }
      else if (encoder_diffState == ENCODER_DIFF_CCW) {
        HMI_ValueStruct.E_Temp -= EncoderRate.encoderMoveValue;
      }
      else if (encoder_diffState == ENCODER_DIFF_ENTER) { // return
        EncoderRate.encoderRateEnabled = 0;
        if (HMI_ValueStruct.show_mode == -1) { // temperature
          checkkey = TemperatureID;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(1), HMI_ValueStruct.E_Temp);
        }
        else if (HMI_ValueStruct.show_mode == -2) {
          checkkey = PLAPreheat;
          ui.material_preset[0].hotend_temp = HMI_ValueStruct.E_Temp;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(1), ui.material_preset[0].hotend_temp);
          return;
        }
        else if (HMI_ValueStruct.show_mode == -3) {
          checkkey = ABSPreheat;
          ui.material_preset[1].hotend_temp = HMI_ValueStruct.E_Temp;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(1), ui.material_preset[1].hotend_temp);
          return;
        }
        else { // tune
          checkkey = Tune;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2 + MROWS - index_tune), HMI_ValueStruct.E_Temp);
        }
        thermalManager.setTargetHotend(HMI_ValueStruct.E_Temp, 0);
        return;
      }
      // E_Temp limit
      NOMORE(HMI_ValueStruct.E_Temp, MAX_E_TEMP);
      NOLESS(HMI_ValueStruct.E_Temp, MIN_E_TEMP);
      // E_Temp value
      if (HMI_ValueStruct.show_mode >= 0) // tune
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(2 + MROWS - index_tune), HMI_ValueStruct.E_Temp);
      else // other page
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(1), HMI_ValueStruct.E_Temp);
    }
  }

#endif // if HAS_HOTEND

#if HAS_HEATED_BED

  void HMI_BedTemp(void) {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (encoder_diffState == ENCODER_DIFF_CW) {
        HMI_ValueStruct.Bed_Temp += EncoderRate.encoderMoveValue;
      }
      else if (encoder_diffState == ENCODER_DIFF_CCW) {
        HMI_ValueStruct.Bed_Temp -= EncoderRate.encoderMoveValue;
      }
      else if (encoder_diffState == ENCODER_DIFF_ENTER) { // return
        EncoderRate.encoderRateEnabled = 0;
        if (HMI_ValueStruct.show_mode == -1) {
          checkkey = TemperatureID;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2), HMI_ValueStruct.Bed_Temp);
        }
        else if (HMI_ValueStruct.show_mode == -2) {
          checkkey = PLAPreheat;
          ui.material_preset[0].bed_temp = HMI_ValueStruct.Bed_Temp;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2), ui.material_preset[0].bed_temp);
          return;
        }
        else if (HMI_ValueStruct.show_mode == -3) {
          checkkey = ABSPreheat;
          ui.material_preset[1].bed_temp = HMI_ValueStruct.Bed_Temp;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2), ui.material_preset[1].bed_temp);
          return;
        }
        else {
          checkkey = Tune;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3 + MROWS - index_tune), HMI_ValueStruct.Bed_Temp);
        }
        thermalManager.setTargetBed(HMI_ValueStruct.Bed_Temp);
        return;
      }
      // Bed_Temp limit
      NOMORE(HMI_ValueStruct.Bed_Temp, BED_MAX_TARGET);
      NOLESS(HMI_ValueStruct.Bed_Temp, MIN_BED_TEMP);
      // Bed_Temp value
      if (HMI_ValueStruct.show_mode >= 0) // tune page
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(3 + MROWS - index_tune), HMI_ValueStruct.Bed_Temp);
      else // other page
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(2), HMI_ValueStruct.Bed_Temp);
    }
  }

#endif // if HAS_HEATED_BED

#if HAS_FAN

  void HMI_FanSpeed(void) {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (encoder_diffState == ENCODER_DIFF_CW) {
        HMI_ValueStruct.Fan_speed += EncoderRate.encoderMoveValue;
      }
      else if (encoder_diffState == ENCODER_DIFF_CCW) {
        HMI_ValueStruct.Fan_speed -= EncoderRate.encoderMoveValue;
      }
      else if (encoder_diffState == ENCODER_DIFF_ENTER) { // return
        EncoderRate.encoderRateEnabled = 0;
        if (HMI_ValueStruct.show_mode == -1) {
          checkkey = TemperatureID;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3), HMI_ValueStruct.Fan_speed);
        }
        else if (HMI_ValueStruct.show_mode == -2) {
          checkkey = PLAPreheat;
          ui.material_preset[0].fan_speed = HMI_ValueStruct.Fan_speed;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3), ui.material_preset[0].fan_speed);
          return;
        }
        else if (HMI_ValueStruct.show_mode == -3) {
          checkkey = ABSPreheat;
          ui.material_preset[1].fan_speed = HMI_ValueStruct.Fan_speed;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3), ui.material_preset[1].fan_speed);
          return;
        }
        else {
          checkkey = Tune;
          DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(4 + MROWS - index_tune), HMI_ValueStruct.Fan_speed);
        }
        thermalManager.set_fan_speed(0, HMI_ValueStruct.Fan_speed);
        return;
      }
      // Fan_speed limit
      NOMORE(HMI_ValueStruct.Fan_speed, FANON);
      NOLESS(HMI_ValueStruct.Fan_speed, FANOFF);
      // Fan_speed value
      if (HMI_ValueStruct.show_mode >= 0) // tune page
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(4 + MROWS - index_tune), HMI_ValueStruct.Fan_speed);
      else // other page
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(3), HMI_ValueStruct.Fan_speed);
    }
  }

#endif // if HAS_FAN

void HMI_PrintSpeed(void) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (encoder_diffState == ENCODER_DIFF_CW) {
      HMI_ValueStruct.print_speed += EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      HMI_ValueStruct.print_speed -= EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) { // return
      checkkey = Tune;
      EncoderRate.encoderRateEnabled = 0;
      feedrate_percentage            = HMI_ValueStruct.print_speed;
      DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(1 + MROWS - index_tune), HMI_ValueStruct.print_speed);
      return;
    }
    // print_speed limit
    NOMORE(HMI_ValueStruct.print_speed, MAX_PRINT_SPEED);
    NOLESS(HMI_ValueStruct.print_speed, MIN_PRINT_SPEED);
    // print_speed value
    DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(1 + MROWS - index_tune), HMI_ValueStruct.print_speed);
  }
}

void HMI_MaxFeedspeedXYZE(void) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (encoder_diffState == ENCODER_DIFF_CW) {
      HMI_ValueStruct.Max_Feedspeed += EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      HMI_ValueStruct.Max_Feedspeed -= EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) { // return
      checkkey = MaxSpeed;
      EncoderRate.encoderRateEnabled = 0;
      if (HMI_flag.feedspeed_flag == X_AXIS) planner.set_max_feedrate(X_AXIS, HMI_ValueStruct.Max_Feedspeed);
      else if (HMI_flag.feedspeed_flag == Y_AXIS) planner.set_max_feedrate(Y_AXIS, HMI_ValueStruct.Max_Feedspeed);
      else if (HMI_flag.feedspeed_flag == Z_AXIS) planner.set_max_feedrate(Z_AXIS, HMI_ValueStruct.Max_Feedspeed);
      #if HAS_HOTEND
        else if (HMI_flag.feedspeed_flag == E_AXIS) planner.set_max_feedrate(E_AXIS, HMI_ValueStruct.Max_Feedspeed);
      #endif
      DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
      return;
    }
    // MaxFeedspeed limit
    if (HMI_flag.feedspeed_flag == X_AXIS) {if (HMI_ValueStruct.Max_Feedspeed > default_max_feedrate[X_AXIS] * 2) HMI_ValueStruct.Max_Feedspeed = default_max_feedrate[X_AXIS] * 2; }
    else if (HMI_flag.feedspeed_flag == Y_AXIS) {if (HMI_ValueStruct.Max_Feedspeed > default_max_feedrate[Y_AXIS] * 2) HMI_ValueStruct.Max_Feedspeed = default_max_feedrate[Y_AXIS] * 2; }
    else if (HMI_flag.feedspeed_flag == Z_AXIS) {if (HMI_ValueStruct.Max_Feedspeed > default_max_feedrate[Z_AXIS] * 2) HMI_ValueStruct.Max_Feedspeed = default_max_feedrate[Z_AXIS] * 2; }
    #if HAS_HOTEND
      else if (HMI_flag.feedspeed_flag == E_AXIS) {if (HMI_ValueStruct.Max_Feedspeed > default_max_feedrate[E_AXIS] * 2) HMI_ValueStruct.Max_Feedspeed = default_max_feedrate[E_AXIS] * 2; }
    #endif
    if (HMI_ValueStruct.Max_Feedspeed < MIN_MAXFEEDSPEED) HMI_ValueStruct.Max_Feedspeed = MIN_MAXFEEDSPEED;
    // MaxFeedspeed value
    DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
  }
}

void HMI_MaxAccelerationXYZE(void) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (encoder_diffState == ENCODER_DIFF_CW) { HMI_ValueStruct.Max_Acceleration += EncoderRate.encoderMoveValue;}
    else if (encoder_diffState == ENCODER_DIFF_CCW) { HMI_ValueStruct.Max_Acceleration -= EncoderRate.encoderMoveValue;}
    else if (encoder_diffState == ENCODER_DIFF_ENTER) { // return
      checkkey = MaxAcceleration;
      EncoderRate.encoderRateEnabled = 0;
      if (HMI_flag.acc_flag == X_AXIS) planner.set_max_acceleration(X_AXIS, HMI_ValueStruct.Max_Acceleration);
      else if (HMI_flag.acc_flag == Y_AXIS) planner.set_max_acceleration(Y_AXIS, HMI_ValueStruct.Max_Acceleration);
      else if (HMI_flag.acc_flag == Z_AXIS) planner.set_max_acceleration(Z_AXIS, HMI_ValueStruct.Max_Acceleration);
      #if HAS_HOTEND
        else if (HMI_flag.acc_flag == E_AXIS) planner.set_max_acceleration(E_AXIS, HMI_ValueStruct.Max_Acceleration);
      #endif
      DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
      return;
    }
    // MaxAcceleration limit
    if (HMI_flag.acc_flag == X_AXIS) {if (HMI_ValueStruct.Max_Acceleration > default_max_acceleration[X_AXIS] * 2) HMI_ValueStruct.Max_Acceleration = default_max_acceleration[X_AXIS] * 2; }
    else if (HMI_flag.acc_flag == Y_AXIS) {if (HMI_ValueStruct.Max_Acceleration > default_max_acceleration[Y_AXIS] * 2) HMI_ValueStruct.Max_Acceleration = default_max_acceleration[Y_AXIS] * 2; }
    else if (HMI_flag.acc_flag == Z_AXIS) {if (HMI_ValueStruct.Max_Acceleration > default_max_acceleration[Z_AXIS] * 2) HMI_ValueStruct.Max_Acceleration = default_max_acceleration[Z_AXIS] * 2; }
    #if HAS_HOTEND
      else if (HMI_flag.acc_flag == E_AXIS) {if (HMI_ValueStruct.Max_Acceleration > default_max_acceleration[E_AXIS] * 2) HMI_ValueStruct.Max_Acceleration = default_max_acceleration[E_AXIS] * 2; }
    #endif
    if (HMI_ValueStruct.Max_Acceleration < MIN_MAXACCELERATION) HMI_ValueStruct.Max_Acceleration = MIN_MAXACCELERATION;
    // MaxAcceleration value
    DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
  }
}

void HMI_MaxCornerXYZE(void) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (encoder_diffState == ENCODER_DIFF_CW) {
      HMI_ValueStruct.Max_Corner += EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      HMI_ValueStruct.Max_Corner -= EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) { // return
      checkkey = MaxCorner;
      EncoderRate.encoderRateEnabled = 0;
      if (HMI_flag.corner_flag == X_AXIS) planner.set_max_jerk(X_AXIS, HMI_ValueStruct.Max_Corner / 10);
      else if (HMI_flag.corner_flag == Y_AXIS) planner.set_max_jerk(Y_AXIS, HMI_ValueStruct.Max_Corner / 10);
      else if (HMI_flag.corner_flag == Z_AXIS) planner.set_max_jerk(Z_AXIS, HMI_ValueStruct.Max_Corner / 10);
      else if (HMI_flag.corner_flag == E_AXIS) planner.set_max_jerk(E_AXIS, HMI_ValueStruct.Max_Corner / 10);
      DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(select_corner.now), HMI_ValueStruct.Max_Corner);
      return;
    }
    // MaxCorner limit
    if (HMI_flag.corner_flag == X_AXIS)
      NOMORE(HMI_ValueStruct.Max_Corner, default_max_jerk[X_AXIS] * 2 * MINUNITMULT);
    else if (HMI_flag.corner_flag == Y_AXIS)
      NOMORE(HMI_ValueStruct.Max_Corner, default_max_jerk[Y_AXIS] * 2 * MINUNITMULT);
    else if (HMI_flag.corner_flag == Z_AXIS)
      NOMORE(HMI_ValueStruct.Max_Corner, default_max_jerk[Z_AXIS] * 2 * MINUNITMULT);
    else if (HMI_flag.corner_flag == E_AXIS)
      NOMORE(HMI_ValueStruct.Max_Corner, default_max_jerk[E_AXIS] * 2 * MINUNITMULT);
    NOLESS(HMI_ValueStruct.Max_Corner, (MIN_MAXCORNER) * MINUNITMULT);
    // MaxCorner value
    DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_corner.now), HMI_ValueStruct.Max_Corner);
  }
}

void HMI_StepXYZE(void) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (encoder_diffState == ENCODER_DIFF_CW) {
      HMI_ValueStruct.Max_Step += EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      HMI_ValueStruct.Max_Step -= EncoderRate.encoderMoveValue;
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) { // return
      checkkey = Step;
      EncoderRate.encoderRateEnabled = 0;
      if (HMI_flag.step_flag == X_AXIS) planner.settings.axis_steps_per_mm[X_AXIS] = HMI_ValueStruct.Max_Step / 10;
      else if (HMI_flag.step_flag == Y_AXIS) planner.settings.axis_steps_per_mm[Y_AXIS] = HMI_ValueStruct.Max_Step / 10;
      else if (HMI_flag.step_flag == Z_AXIS) planner.settings.axis_steps_per_mm[Z_AXIS] = HMI_ValueStruct.Max_Step / 10;
      else if (HMI_flag.step_flag == E_AXIS) planner.settings.axis_steps_per_mm[E_AXIS] = HMI_ValueStruct.Max_Step / 10;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step);
      return;
    }
    // Step limit
    if (HMI_flag.step_flag == X_AXIS)
      NOMORE(HMI_ValueStruct.Max_Step, default_axis_steps_per_unit[X_AXIS] * 2 * MINUNITMULT);
    else if (HMI_flag.step_flag == Y_AXIS)
      NOMORE(HMI_ValueStruct.Max_Step, default_axis_steps_per_unit[Y_AXIS] * 2 * MINUNITMULT);
    else if (HMI_flag.step_flag == Z_AXIS)
      NOMORE(HMI_ValueStruct.Max_Step, default_axis_steps_per_unit[Z_AXIS] * 2 * MINUNITMULT);
    else if (HMI_flag.step_flag == E_AXIS)
      NOMORE(HMI_ValueStruct.Max_Step, default_axis_steps_per_unit[E_AXIS] * 2 * MINUNITMULT);
    NOLESS(HMI_ValueStruct.Max_Step, MIN_STEP);
    // Step value
    DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step);
  }
}

void update_variable(void) {
  /* Tune page temperature update */
  if (checkkey == Tune) {
    if (last_temp_hotend_target != thermalManager.temp_hotend[0].target)
      DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2 + MROWS - index_tune), thermalManager.temp_hotend[0].target);
    if (last_temp_bed_target != thermalManager.temp_bed.target)
      DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3 + MROWS - index_tune), thermalManager.temp_bed.target);
    if (last_fan_speed != thermalManager.fan_speed[0]) {
      DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(4 + MROWS - index_tune), thermalManager.fan_speed[0]);
      last_fan_speed = thermalManager.fan_speed[0];
    }
  }

  /* Temperature page temperature update */
  if (checkkey == TemperatureID) {
    if (last_temp_hotend_target != thermalManager.temp_hotend[0].target)
      DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(1), thermalManager.temp_hotend[0].target);
    if (last_temp_bed_target != thermalManager.temp_bed.target)
      DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2), thermalManager.temp_bed.target);
    if (last_fan_speed != thermalManager.fan_speed[0]) {
      DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3), thermalManager.fan_speed[0]);
      last_fan_speed = thermalManager.fan_speed[0];
    }
  }

  /* Bottom temperature update */
  if (last_temp_hotend_current != thermalManager.temp_hotend[0].celsius) {
    DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 33, 382, thermalManager.temp_hotend[0].celsius);
    last_temp_hotend_current = thermalManager.temp_hotend[0].celsius;
  }
  if (last_temp_hotend_target != thermalManager.temp_hotend[0].target) {
    DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 33 + 4 * STAT_CHR_W + 6, 382, thermalManager.temp_hotend[0].target);
    last_temp_hotend_target = thermalManager.temp_hotend[0].target;
  }
  if (last_temp_bed_current != thermalManager.temp_bed.celsius) {
    DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 178, 382, thermalManager.temp_bed.celsius);
    last_temp_bed_current = thermalManager.temp_bed.celsius;
  }
  if (last_temp_bed_target != thermalManager.temp_bed.target) {
    DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 178 + 4 * STAT_CHR_W + 6, 382, thermalManager.temp_bed.target);
    last_temp_bed_target = thermalManager.temp_bed.target;
  }
  if (last_speed != feedrate_percentage) {
    DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 33 + 2 * STAT_CHR_W, 429, feedrate_percentage);
    last_speed = feedrate_percentage;
  }
  #if HAS_BED_PROBE
    if (last_probe_zoffset != probe.offset.z) {
      show_plus_or_minus(STAT_FONT, Background_black, 2, 2, 178 + STAT_CHR_W, 429, probe.offset.z * 100);
      last_probe_zoffset = probe.offset.z;
    }
  #else
    if (last_zoffset != zprobe_zoffset) {
      show_plus_or_minus(STAT_FONT, Background_black, 2, 2, 178 + STAT_CHR_W, 429, zprobe_zoffset * 100);
      last_zoffset = zprobe_zoffset;
    }
  #endif
}

/**
* Read and cache the working directory.
*
* TODO: New code can follow the pattern of menu_media.cpp
* and rely on Marlin caching for performance. No need to
* cache files here.
*
*/

#ifndef strcasecmp_P
  #define strcasecmp_P(a, b) strcasecmp((a), (b))
#endif

inline void make_name_without_ext(char *dst, char *src, int maxlen=MENU_CHAR_LIMIT) {
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

inline void HMI_SDCardInit(void) { card.cdroot(); }

void MarlinUI::refresh() {
  // The card was mounted or unmounted
  // or some other status change occurred
  // DWIN_lcd_sd_status = false; // On next DWIN_Update
  // HMI_SDCardUpdate();
}

#define ICON_Folder ICON_More

char shift_name[LONG_FILENAME_LENGTH + 1];
int8_t shift_amt; // = 0
millis_t shift_ms; // = 0

// Init the shift name based on the highlighted item
inline void Init_Shift_Name() {
  const bool is_subdir = !card.flag.workDirIsRoot;
  const int8_t filenum = select_file.now - 1 - is_subdir; // Skip "Back" and ".."
  const uint16_t fileCnt = card.get_num_Files();
  if (WITHIN(filenum, 0, fileCnt - 1)) {
    card.getfilename_sorted(SD_ORDER(filenum, fileCnt));
    char * const name = card.longest_filename();
    make_name_without_ext(shift_name, name, 100);
  }
}

inline void Init_SDItem_Shift() {
  shift_amt = 0;
  shift_ms  = select_file.now > 0 && strlen(shift_name) > MENU_CHAR_LIMIT
         ? millis() + 750UL : 0;
}

/**
 * Display an SD item, adding a CDUP for subfolders.
 */
inline void Draw_SDItem(const uint16_t item, int16_t row=-1) {
  if (row < 0) row = item + 1 + MROWS - index_file;
  const bool is_subdir = !card.flag.workDirIsRoot;
  if (is_subdir && item == 0) {
    Draw_Menu_Line(row, ICON_Folder, (char*)"..");
    return;
  }

  card.getfilename_sorted(item - is_subdir);
  char * const name = card.longest_filename();

  // Init the current selected name
  // This is used during scroll drawing
  if (item == select_file.now - 1) {
    make_name_without_ext(shift_name, name, 100);
    Init_SDItem_Shift();
  }

  char str[strlen(name) + 1];

  make_name_without_ext(str, name);

  Draw_Menu_Line(row, card.flag.filenameIsDir ? ICON_Folder : ICON_File, str);
}

inline void Draw_SDItem_Shifted(int8_t &shift) {
  // Limit to the number of chars past the cutoff
  const size_t len = strlen(shift_name);
  NOMORE(shift, _MAX((signed)len - MENU_CHAR_LIMIT, 0));

  // Shorten to the available space
  const size_t lastchar = _MIN((signed)len, shift + MENU_CHAR_LIMIT);

  const char c = shift_name[lastchar];
  shift_name[lastchar] = '\0';

  const uint8_t row = select_file.now + MROWS - index_file; // skip "Back" and scroll
  Erase_Menu_Text(row);
  Draw_Menu_Line(row, 0, &shift_name[shift]);

  shift_name[lastchar] = c;
}

// Redraw the first set of SD Files
inline void Redraw_SD_List() {
  select_file.reset();
  index_file = MROWS;

  Clear_Menu_Area(); // Leave title bar unchanged

  Draw_Back_First();

  // As many files as will fit
  LOOP_L_N(i, _MIN(nr_sd_menu_items(), MROWS))
    Draw_SDItem(i, i + 1);

  Init_SDItem_Shift();
}

inline void SDCard_Up(void) {
  card.cdup();
  Redraw_SD_List();
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

inline void SDCard_Folder(char * const dirname) {
  card.cd(dirname);
  Redraw_SD_List();
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

//
// Watch for media mount / unmount
//
void HMI_SDCardUpdate(void) {
  if (HMI_flag.home_flag) return;
  if (DWIN_lcd_sd_status != card.isMounted()) {
    DWIN_lcd_sd_status = card.isMounted();
    // SERIAL_ECHOLNPAIR("HMI_SDCardUpdate: ", int(DWIN_lcd_sd_status));
    if (card.isMounted()) {
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
        card.flag.abort_sd_printing = true;
        wait_for_heatup             = false;
        abort_flag                  = true;
      }
    }
    DWIN_UpdateLCD();
  }
}

/* Start */
void HMI_StartFrame(const bool with_update) {
  Goto_MainMenu();

  DWIN_Draw_Rectangle(1, Background_black,  0,  360,  272, 479);

  DWIN_ICON_Show(ICON, ICON_HotendTemp, 13, 381);
  #if HOTENDS > 1
    // DWIN_ICON_Show(ICON,ICON_HotendTemp, 13, 381);
  #endif
  DWIN_ICON_Show(ICON, ICON_BedTemp,   158, 381);
  DWIN_ICON_Show(ICON, ICON_Speed,      13, 429);
  DWIN_ICON_Show(ICON, ICON_Zoffset,   158, 428);

  // Draw initial Status Area
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 33, 382, thermalManager.temp_hotend[0].celsius);
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 33 + 4 * STAT_CHR_W + 6, 382, thermalManager.temp_hotend[0].target);
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 178, 382, thermalManager.temp_bed.celsius);
  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 178 + 4 * STAT_CHR_W + 6, 382, thermalManager.temp_bed.target);

  DWIN_Draw_IntValue(true, true, 0, STAT_FONT, White, Background_black, 3, 33 + 2 * STAT_CHR_W, 429, feedrate_percentage);
  DWIN_Draw_String(false, false, STAT_FONT, White, Background_black, 33 + (2 + 3) * STAT_CHR_W + 2, 429, (char*)"%");

  show_plus_or_minus(STAT_FONT, Background_black, 2, 2, 178, 429, BABY_Z_VAR * 100);
  DWIN_Draw_String(false, false, STAT_FONT, White, Background_black, 33 + 3 * STAT_CHR_W + 5, 383, (char*)"/");
  DWIN_Draw_String(false, false, STAT_FONT, White, Background_black, 178 + 3 * STAT_CHR_W + 5, 383, (char*)"/");

  if (with_update) {
    DWIN_UpdateLCD();
    delay(5);
  }
}

inline void Draw_Info_Menu() {
  Clear_Main_Window();

  DWIN_Draw_String(false, false, font8x16, White, Background_black, (DWIN_WIDTH - strlen(MACHINE_SIZE) * MENU_CHR_W) / 2, 122, (char*)MACHINE_SIZE);
  DWIN_Draw_String(false, false, font8x16, White, Background_black, (DWIN_WIDTH - strlen(SHORT_BUILD_VERSION) * MENU_CHR_W) / 2, 195, (char*)SHORT_BUILD_VERSION);

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 30, 17, 271 - 214, 479 - 450, 14, 8);

    DWIN_Frame_AreaCopy(1, 197, 149, 271 - 19, 479 - 318, 108, 102);
    DWIN_Frame_AreaCopy(1, 1, 164, 271 - 215, 479 - 303, 108, 175);
    DWIN_Frame_AreaCopy(1, 58, 164, 271 - 158, 479 - 303, 105, 248);
    DWIN_Draw_String(false, false, font8x16, White, Background_black, (DWIN_WIDTH - strlen(CORP_WEBSITE_C) * MENU_CHR_W) / 2, 268, (char*)CORP_WEBSITE_C);
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_INFO_SCREEN));
    #else
      DWIN_Frame_AreaCopy(1, 190, 16, 271 - 56, 479 - 453, 14, 8);
    #endif

    DWIN_Frame_AreaCopy(1, 120, 150, 146, 479 - 318, 124, 102);
    DWIN_Frame_AreaCopy(1, 146, 151, 271 - 17, 479 - 318, 82, 175);
    DWIN_Frame_AreaCopy(1, 0, 165, 271 - 177, 479 - 304, 89, 248);
    DWIN_Draw_String(false, false, font8x16, White, Background_black, (DWIN_WIDTH - strlen(CORP_WEBSITE_E) * MENU_CHR_W) / 2, 268, (char*)CORP_WEBSITE_E);
  }

  Draw_Back_First();
  LOOP_L_N(i, 3) {
    DWIN_ICON_Show(ICON, ICON_PrintSize + i, 26, 99 + i * 73);
    DWIN_Draw_Line(Line_Color, 16, MBASE(2) + i * 73, 256, 156 + i * 73);
  }
}

inline void Draw_Print_File_Menu() {
  Clear_Title_Bar();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 0, 31, 271 - 216, 479 - 435, 14, 8);
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title("Print file"); // TODO: GET_TEXT_F
    #else
      DWIN_Frame_AreaCopy(1, 52, 31, 271 - 134, 479 - 438, 14, 8); // "Print file"
    #endif
  }

  Redraw_SD_List();
}

/* Main Process */
void HMI_MainMenu(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_page.inc(3)) {
      switch (select_page.now) {
        case 0: ICON_Print(); break;
        case 1: ICON_Print(); ICON_Prepare(); break;
        case 2: ICON_Prepare(); ICON_Control(); break;
        case 3: ICON_Control(); TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(1); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_page.dec()) {
      switch (select_page.now) {
        case 0: ICON_Print(); ICON_Prepare(); break;
        case 1: ICON_Prepare(); ICON_Control(); break;
        case 2: ICON_Control(); TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(0); break;
        case 3: TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(1); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_page.now) {
      /* Print File */
      case 0:
        checkkey = SelectFile;
        Draw_Print_File_Menu();
        break;

      /* Prepare */
      case 1:
        checkkey = Prepare;
        select_prepare.reset();
        index_prepare = MROWS;
        Draw_Prepare_Menu();
        break;

      /* Control */
      case 2:
        checkkey = Control;
        select_control.reset();
        index_control = MROWS;
        Draw_Control_Menu();
        break;

      /* Leveling */
      case 3:
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

/* Select (and Print) File */
void HMI_SelectFile(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();

  const uint16_t hasUpDir = !card.flag.workDirIsRoot;

  if (encoder_diffState == ENCODER_DIFF_NO) {
    if (shift_ms && select_file.now >= 1 + hasUpDir) {
      // Scroll selected filename every second
      const millis_t ms = millis();
      if (ELAPSED(ms, shift_ms)) {
        const bool was_reset = shift_amt < 0;
        shift_ms = ms + 375UL + was_reset * 250UL;  // ms per character
        int8_t shift_new = shift_amt + 1;           // Try to shift by...
        Draw_SDItem_Shifted(shift_new);             // Draw the item
        if (!was_reset && shift_new == 0)           // Was it limited to 0?
          shift_ms = 0;                             // No scrolling needed
        else if (shift_new == shift_amt)            // Scroll reached the end
          shift_new = -1;                           // Reset
        shift_amt = shift_new;                      // Set new scroll
      }
    }
    return;
  }

  // First pause is long. Easy.
  // On reset, long pause must be after 0.

  const uint16_t fullCnt = nr_sd_menu_items();

  if (encoder_diffState == ENCODER_DIFF_CW && fullCnt) {
    if (select_file.inc(fullCnt)) {
      const uint8_t itemnum = select_file.now - 1;              // -1 for "Back"
      if (shift_ms) {                                           // If line was shifted
        Erase_Menu_Text(select_file.now - 1 + MROWS - index_file); // Erase and
        Draw_SDItem(itemnum - 1);                               // redraw
      }
      if (select_file.now > MROWS && select_file.now > index_file) { // Cursor past the bottom
        index_file = select_file.now;                           // New bottom line
        Scroll_Menu(DWIN_SCROLL_UP);
        Draw_SDItem(itemnum, MROWS);                            // Draw and init the shift name
      }
      else {
        Move_Highlight(1, select_file.now + MROWS - index_file); // Just move highlight
        Init_Shift_Name();                                      // ...and init the shift name
      }
      Init_SDItem_Shift();
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && fullCnt) {
    if (select_file.dec()) {
      const uint8_t itemnum = select_file.now - 1;              // -1 for "Back"
      if (shift_ms) {                                           // If line was shifted
        Erase_Menu_Text(select_file.now + 1 + MROWS - index_file); // Erase and
        Draw_SDItem(itemnum + 1);                               // redraw
      }
      if (select_file.now < index_file - MROWS) {               // Cursor past the top
        index_file--;                                           // New bottom line
        Scroll_Menu(DWIN_SCROLL_DOWN);
        if (index_file == MROWS) {
          Draw_Back_First();
          shift_ms = 0;
        }
        else {
          Draw_SDItem(itemnum, 0);                              // Draw the item (and init shift name)
        }
      }
      else {
        Move_Highlight(-1, select_file.now + MROWS - index_file); // Just move highlight
        Init_Shift_Name();                                      // ...and init the shift name
      }
      Init_SDItem_Shift();                                      // Reset left. Init timer.
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (select_file.now == 0) {
      /* back */
      select_page.set(0);
      Goto_MainMenu();
    }
    else if (hasUpDir && select_file.now == 1) {
      /* CDUP */
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
      HMI_flag.heat_flag        = 1;
      HMI_flag.print_finish     = 0;
      HMI_ValueStruct.show_mode = 0;

      card.openAndPrintFile(card.filename);

      #if FAN_COUNT > 0
        // All fans on for Ender 3 v2 ?
        // The slicer should manage this for us.
        // for (uint8_t i = 0; i < FAN_COUNT; i++)
        //  thermalManager.fan_speed[i] = FANON;
      #endif

      Goto_PrintProcess();
    }
  }
HMI_SelectFileExit:
  DWIN_UpdateLCD();
}

/* Printing */
void HMI_Printing(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (HMI_flag.confirm_flag) {
    if (encoder_diffState == ENCODER_DIFF_ENTER) {
      HMI_flag.confirm_flag = 0;
      abort_flag            = 1;
    }
    return;
  }

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_print.inc(2)) {
      switch (select_print.now) {
        case 0: ICON_Tune(); break;
        case 1:
          ICON_Tune();
          if (printingIsPaused()) ICON_Continue(); else ICON_Pause();
          break;
        case 2:
          if (printingIsPaused()) ICON_Continue(); else ICON_Pause();
          ICON_Stop();
          break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_print.dec()) {
      switch (select_print.now) {
        case 0:
          ICON_Tune();
          if (printingIsPaused()) ICON_Continue(); else ICON_Pause();
          break;
        case 1:
          if (printingIsPaused()) ICON_Continue(); else ICON_Pause();
          ICON_Stop();
          break;
        case 2: ICON_Stop(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_print.now) {
      case 0: // setting
        checkkey = Tune;
        HMI_ValueStruct.show_mode = 0;
        select_tune.reset();
        index_tune = 5;
        Draw_Tune_Menu();
        break;
      case 1: // pause
        /* pause */
        if (HMI_flag.pause_flag) {
          ICON_Pause();

          char cmd[40];
          cmd[0] = '\0';

          #if ENABLED(PAUSE_HEAT)
            if (tempbed) sprintf_P(cmd, PSTR("M190 S%i\n"), tempbed);
            if (temphot) sprintf_P(&cmd[strlen(cmd)], PSTR("M109 S%i\n"), temphot);
          #endif

          strcat_P(cmd, PSTR("M24"));
          queue.inject(cmd);
        }
        else {
          HMI_flag.select_flag = 1;
          checkkey = Print_window;
          Popup_window_PauseOrStop();
        }
        break;

      case 2: // stop
        /* stop */
        HMI_flag.select_flag = 1;
        checkkey = Print_window;
        Popup_window_PauseOrStop();
        break;

      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}

/* pause and stop window */
void HMI_PauseOrStop(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_CW) {
    Draw_Select_Highlight(false);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    Draw_Select_Highlight(true);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (select_print.now == 1) { // pause window
      if (HMI_flag.select_flag) {
        pause_action_flag = 1;
        ICON_Continue();
        #if ENABLED(POWER_LOSS_RECOVERY)
          if (recovery.enabled) recovery.save(true);
        #endif
        queue.inject_P(PSTR("M25"));
      }
      else {
        // cancel pause
      }
      Goto_PrintProcess();
    }
    else if (select_print.now == 2) { // stop window
      if (HMI_flag.select_flag) {
        wait_for_heatup = false; // Stop waiting for heater

        #if 0
          // TODO: In ExtUI or MarlinUI add a common stop event
          // card.flag.abort_sd_printing = true;
        #else
          checkkey = Back_Main;
          // Wait for planner moves to finish!
          if (HMI_flag.home_flag) planner.synchronize();
          card.endFilePrint();
          #ifdef ACTION_ON_CANCEL
            host_action_cancel();
          #endif
          #ifdef EVENT_GCODE_SD_ABORT
            Popup_Window_Home();
            queue.inject_P(PSTR(EVENT_GCODE_SD_ABORT));
          #endif
          abort_flag = true;
        #endif
      }
      else {
        Goto_PrintProcess(); // cancel stop
      }
    }
  }
  DWIN_UpdateLCD();
}

inline void Draw_Move_Menu() {
  Clear_Main_Window();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 192, 1, 271 - 38, 479 - 465, 14, 8);
    DWIN_Frame_AreaCopy(1, 58, 118, 271 - 165, 479 - 347, LBLX, MBASE(1));
    DWIN_Frame_AreaCopy(1, 109, 118, 271 - 114, 479 - 347, LBLX, MBASE(2));
    DWIN_Frame_AreaCopy(1, 160, 118, 271 - 62, 479 - 347, LBLX, MBASE(3));
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 212, 118, 271 - 18, 479 - 348, LBLX, MBASE(4));
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MOVE_AXIS));
    #else
      DWIN_Frame_AreaCopy(1, 231, 2, 271 - 6, 479 - 467, 14, 8);
    #endif
    draw_move_en(MBASE(1)); say_x(33 + 3, MBASE(1));                    // "Move X"
    draw_move_en(MBASE(2)); say_y(33 + 3, MBASE(2));                    // "Move Y"
    draw_move_en(MBASE(3)); say_z(33 + 3, MBASE(3));                    // "Move Z"
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 123, 192, 271 - 95, 479 - 277, LBLX, MBASE(4)); // "Extruder"
    #endif
  }

  Draw_Back_First(select_axis.now == 0);
  if (select_axis.now) Draw_Menu_Cursor(select_axis.now);

  LOOP_L_N(i, MROWS) Draw_Menu_Line(i + 1, ICON_MoveX + i);
}

#include "../../libs/buzzer.h"

void HMI_AudioFeedback(const bool success=true) {
  if (success) {
    buzzer.tone(100, 659);
    buzzer.tone(10, 0);
    buzzer.tone(100, 698);
  }
  else
    buzzer.tone(40, 440);
}

/* Prepare */
void HMI_Prepare(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_prepare.inc(8)) {
      if (select_prepare.now > MROWS && select_prepare.now > index_prepare) {
        index_prepare = select_prepare.now;

        // Scroll up and draw a blank bottom line
        Scroll_Menu(DWIN_SCROLL_UP);
        Draw_Menu_Icon(MROWS, ICON_Axis + select_prepare.now - 1);

        // Draw "More" icon for sub-menus
        if (index_prepare < 7) Draw_More_Icon(MROWS - index_prepare + 1);

        if (index_prepare == 6) Prepare_Item_ABS(MROWS);
        else if (index_prepare == 7) Prepare_Item_Cool(MROWS);
        else if (index_prepare == 8) Prepare_Item_Lang(MROWS);
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

        if (index_prepare == 6) Prepare_Item_Move(0);
        else if (index_prepare == 7) Prepare_Item_Disable(0);
        else if (index_prepare == 8) Prepare_Item_Home(0);
      }
      else {
        Move_Highlight(-1, select_prepare.now + MROWS - index_prepare);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_prepare.now) {
      case 0: // back
        select_page.set(1);
        Goto_MainMenu();
        break;
      case 1: // axis move
        checkkey = AxisMove;
        select_axis.reset();
        Draw_Move_Menu();

        queue.inject_P(PSTR("G92 E0"));
        current_position.e = HMI_ValueStruct.Move_E_scale = 0;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 216, MBASE(1), current_position[X_AXIS] * MINUNITMULT);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 216, MBASE(2), current_position[Y_AXIS] * MINUNITMULT);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 216, MBASE(3), current_position[Z_AXIS] * MINUNITMULT);
        show_plus_or_minus(font8x16, Background_black, 3, 1, 216, MBASE(4), current_position.e * MINUNITMULT);
        break;
      case 2: // close motion
        queue.inject_P(PSTR("M84"));
        break;
      case 3: // homing
        checkkey = Last_Prepare;
        index_prepare = MROWS;
        queue.inject_P(PSTR("G28")); // G28 will set home_flag
        Popup_Window_Home();
        break;
      case 4: // Z-offset
        #if HAS_BED_PROBE
          checkkey = Homeoffset;
          HMI_ValueStruct.show_mode    = -4;
          HMI_ValueStruct.offset_value = probe.offset.z * 100;
          show_plus_or_minus(font8x16, Select_Color, 2, 2, 202, MBASE(4 + MROWS - index_prepare), HMI_ValueStruct.offset_value);
          EncoderRate.encoderRateEnabled = 1;
        #else
          // Apply workspace offset, making the current position 0,0,0
          queue.inject_P(PSTR("G92 X0 Y0 Z0"));
          HMI_AudioFeedback();
        #endif
        break;
      case 5: // PLA preheat
        thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0);
        thermalManager.setTargetBed(ui.material_preset[0].bed_temp);
        thermalManager.set_fan_speed(0, ui.material_preset[0].fan_speed);
        break;
      case 6: // ABS preheat
        thermalManager.setTargetHotend(ui.material_preset[1].hotend_temp, 0);
        thermalManager.setTargetBed(ui.material_preset[1].bed_temp);
        thermalManager.set_fan_speed(0, ui.material_preset[1].fan_speed);
        break;
      case 7: // cool
        thermalManager.zero_fan_speeds();
        thermalManager.disable_all_heaters();
        break;
      case 8: // language
        /* select language */
        HMI_flag.language_chinese ^= true;
        if (HMI_flag.language_chinese) {
          HMI_SetAndSaveLanguageChinese();
          DWIN_JPG_CacheTo1(Language_Chinese);
        }
        else {
          HMI_SetAndSaveLanguageWestern();
          DWIN_JPG_CacheTo1(Language_English);
        }
        Draw_Prepare_Menu();
        break;
      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}

void Draw_Temperature_Menu() {
  Clear_Main_Window();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 236, 2, 271 - 8, 479 - 466, 14, 8);

    DWIN_Frame_AreaCopy(1, 1, 134, 271 - 215, 479 - 333, LBLX, MBASE(1));
    DWIN_Frame_AreaCopy(1, 58, 134, 271 - 158, 479 - 333, LBLX, MBASE(2));
    DWIN_Frame_AreaCopy(1, 115, 134, 271 - 101, 479 - 333, LBLX, MBASE(3));
    DWIN_Frame_AreaCopy(1, 100, 89, 271 - 93, 479 - 378, LBLX, MBASE(4));
    DWIN_Frame_AreaCopy(1, 180, 89, 271 - 11, 479 - 379, LBLX, MBASE(5));
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_TEMPERATURE));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)GET_TEXT_F(MSG_UBL_SET_TEMP_BED));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)GET_TEXT_F(MSG_FAN_SPEED));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)"PLA Preheat Settings");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 4), (char*)"ABS Preheat Settings");
    #else
      DWIN_Frame_AreaCopy(1, 56, 16, 271 - 130, 479 - 450 - 1, 14, 8);
      DWIN_Frame_AreaCopy(1, 197, 104, 271 - 33, 479 - 365, LBLX, MBASE(1));                    // Nozzle...
      DWIN_Frame_AreaCopy(1, 1, 89, 271 - 188, 479 - 377 - 1, LBLX + 41 + 3, MBASE(1));         // ...Temperature
      DWIN_Frame_AreaCopy(1, 240, 104, 271 - 7, 479 - 365, LBLX, MBASE(2));                     // Bed...
      DWIN_Frame_AreaCopy(1, 1, 89, 271 - 188, 479 - 377 - 1, LBLX + 24 + 3, MBASE(2));         // ...Temperature
      DWIN_Frame_AreaCopy(1, 0, 119, 271 - 207, 479 - 347, LBLX, MBASE(3));                     // Fan speed
      DWIN_Frame_AreaCopy(1, 107, 76, 271 - 115, 479 - 393, LBLX, MBASE(4));                    // Preheat...
      DWIN_Frame_AreaCopy(1, 157, 76, 181, 479 - 393, LBLX + 49 + 3, MBASE(4));                 // ...PLA
      DWIN_Frame_AreaCopy(1, 131, 119, 271 - 89, 479 - 347, LBLX + 49 + 24 + 6, MBASE(4));      // PLA setting
      DWIN_Frame_AreaCopy(1, 107, 76, 271 - 115, 479 - 393, LBLX, MBASE(5));                    // Preheat...
      DWIN_Frame_AreaCopy(1, 172, 76, 198, 479 - 393, LBLX + 49 + 3, MBASE(5));                 // ...ABS
      DWIN_Frame_AreaCopy(1, 131, 119, 271 - 89, 479 - 347, LBLX + 49 + 3 + 26 + 3, MBASE(5));  // ABS setting
    #endif
  }

  Draw_Back_First(select_temp.now == 0);
  if (select_temp.now) Draw_Menu_Cursor(select_temp.now);

  LOOP_L_N(i, 5) Draw_Menu_Line(i + 1, ICON_SetEndTemp + i);

  Draw_More_Icon(4);
  Draw_More_Icon(5);

  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(1), thermalManager.temp_hotend[0].target);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2), thermalManager.temp_bed.target);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3), thermalManager.fan_speed[0]);
}

/* Control */
void HMI_Control(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    #define CONTROL_ITEMS (5 + ENABLED(HAS_ONESTEP_LEVELING))
    if (select_control.inc(CONTROL_ITEMS)) {
      if (select_control.now > MROWS && select_control.now > index_control) {
        index_control = select_control.now;
        Scroll_Menu(DWIN_SCROLL_UP);
        Draw_Menu_Icon(MROWS, ICON_Temperature + select_control.now - 1);
        Draw_More_Icon(1 + MROWS - index_control); // Temperature >
        Draw_More_Icon(2 + MROWS - index_control); // Motion >
        if (index_control > MROWS) {
          Draw_More_Icon(6 + MROWS - index_control); // Info >
          if (HMI_flag.language_chinese)
            DWIN_Frame_AreaCopy(1, 231, 104, 271 - 13, 479 - 363, LBLX, MBASE(5));
          else
            DWIN_Frame_AreaCopy(1, 0, 104, 271 - 247, 479 - 365, LBLX, MBASE(5));
        }
      }
      else {
        Move_Highlight(1, select_control.now + MROWS - index_control);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_control.dec()) {
      if (select_control.now < index_control - MROWS) {
        index_control--;
        Scroll_Menu(DWIN_SCROLL_DOWN);
        if (index_control == MROWS)
          Draw_Back_First();
        else
          Draw_Menu_Line(0, ICON_Temperature + select_control.now - 1);
        Draw_More_Icon(0 + MROWS - index_control + 1); // Temperature >
        Draw_More_Icon(1 + MROWS - index_control + 1); // Motion >
      }
      else {
        Move_Highlight(-1, select_control.now + MROWS - index_control);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_control.now) {
      case 0: // back
        select_page.set(2);
        Goto_MainMenu();
        break;
      case 1: // temperature
        checkkey = TemperatureID;
        HMI_ValueStruct.show_mode = -1;
        select_temp.reset();
        Draw_Temperature_Menu();
        break;
      case 2: // motion
        checkkey = Motion;
        select_motion.reset();
        Draw_Motion_Menu();
        break;
      case 3: { // write EEPROM
        const bool success = settings.save();
        HMI_AudioFeedback(success);
      } break;
      case 4: { // read EEPROM
        const bool success = settings.load();
        HMI_AudioFeedback(success);
      } break;
      case 5: // resume EEPROM
        settings.reset();
        HMI_AudioFeedback();
        break;
      case 6: // info
        checkkey = Info;
        Draw_Info_Menu();
        break;
      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}


#if HAS_ONESTEP_LEVELING

  /* Leveling */
  void HMI_Leveling(void) {
    Popup_Window_Leveling();
    DWIN_UpdateLCD();
    queue.inject_P(PSTR("G28O\nG29"));
  }

#endif

/* Axis Move */
void HMI_AxisMove(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  #if HAS_HOTEND
    // popup window resume
    if (HMI_flag.ETempTooLow_flag) {
      if (encoder_diffState == ENCODER_DIFF_ENTER) {
        HMI_flag.ETempTooLow_flag = 0;
        Draw_Move_Menu();
        current_position.e = HMI_ValueStruct.Move_E_scale = 0;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 216, MBASE(1), HMI_ValueStruct.Move_X_scale);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 216, MBASE(2), HMI_ValueStruct.Move_Y_scale);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 216, MBASE(3), HMI_ValueStruct.Move_Z_scale);
        show_plus_or_minus(font8x16, Background_black, 3, 1, 216, MBASE(4), HMI_ValueStruct.Move_E_scale);
        DWIN_UpdateLCD();
      }
      return;
    }
  #endif

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_axis.inc(4)) Move_Highlight(1, select_axis.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_axis.dec()) Move_Highlight(-1, select_axis.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_axis.now) {
      case 0: // back
        checkkey = Prepare;
        select_prepare.set(1);
        index_prepare = MROWS;
        Draw_Prepare_Menu();
        break;
      case 1: // X axis move
        checkkey = Move_X;
        HMI_ValueStruct.Move_X_scale = current_position[X_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 216, MBASE(1), HMI_ValueStruct.Move_X_scale);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 2: // Y axis move
        checkkey = Move_Y;
        HMI_ValueStruct.Move_Y_scale = current_position[Y_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 216, MBASE(2), HMI_ValueStruct.Move_Y_scale);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 3: // Z axis move
        checkkey = Move_Z;
        HMI_ValueStruct.Move_Z_scale = current_position[Z_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 216, MBASE(3), HMI_ValueStruct.Move_Z_scale);
        EncoderRate.encoderRateEnabled = 1;
        break;
        #if HAS_HOTEND
          case 4: // Extruder
            // window tips
            #ifdef PREVENT_COLD_EXTRUSION
              if (thermalManager.temp_hotend[0].celsius < EXTRUDE_MINTEMP) {
                HMI_flag.ETempTooLow_flag = 1;
                Popup_Window_ETempTooLow();
                DWIN_UpdateLCD();
                return;
              }
            #endif
            checkkey = Extruder;
            HMI_ValueStruct.Move_E_scale = current_position.e * MINUNITMULT;
            show_plus_or_minus(font8x16, Select_Color, 3, 1, 216, MBASE(4), HMI_ValueStruct.Move_E_scale);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
    }
  }
  DWIN_UpdateLCD();
}

/* TemperatureID */
void HMI_Temperature(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_temp.inc(5)) Move_Highlight(1, select_temp.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_temp.dec()) Move_Highlight(-1, select_temp.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_temp.now) {
      case 0: // back
        checkkey = Control;
        select_control.set(1);
        index_control = MROWS;
        Draw_Control_Menu();
        break;
        #if HAS_HOTEND
          case 1: // nozzle temperature
            checkkey = ETemp;
            HMI_ValueStruct.E_Temp = thermalManager.temp_hotend[0].target;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(1), thermalManager.temp_hotend[0].target);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
        #if HAS_HEATED_BED
          case 2: // bed temperature
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = thermalManager.temp_bed.target;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(2), thermalManager.temp_bed.target);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
        #if HAS_FAN
          case 3: // fan speed
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = thermalManager.fan_speed[0];
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(3), thermalManager.fan_speed[0]);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
        #if HAS_HOTEND
          case 4: // PLA preheat setting
            checkkey = PLAPreheat;
            select_PLA.reset();
            HMI_ValueStruct.show_mode = -2;

            Clear_Main_Window();

            if (HMI_flag.language_chinese) {
              DWIN_Frame_AreaCopy(1, 59, 16, 271 - 132, 479 - 450, 14, 8);

              DWIN_Frame_AreaCopy(1, 100, 89, 124, 479 - 378, LBLX, MBASE(1));
              DWIN_Frame_AreaCopy(1, 1, 134, 271 - 215, 479 - 333, LBLX + 24, MBASE(1)); // PLA nozzle temp
              DWIN_Frame_AreaCopy(1, 100, 89, 124, 479 - 378, LBLX, MBASE(2));
              DWIN_Frame_AreaCopy(1, 58, 134, 271 - 158, 479 - 333, LBLX + 24, MBASE(2)); // PLA bed temp
              DWIN_Frame_AreaCopy(1, 100, 89, 124, 479 - 378, LBLX, MBASE(3));
              DWIN_Frame_AreaCopy(1, 115, 134, 271 - 101, 479 - 333, LBLX + 24, MBASE(3)); // PLA fan speed
              DWIN_Frame_AreaCopy(1, 72, 148, 271 - 120, 479 - 317, LBLX, MBASE(4)); // save PLA configuration
            }
            else {
              #ifdef USE_STRING_HEADINGS
                Draw_Title("PLA Settings"); // TODO: GET_TEXT_F
                DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)"Nozzle Temp");
                DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)"Bed Temp");
                DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)GET_TEXT_F(MSG_FAN_SPEED));
                DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)GET_TEXT_F(MSG_STORE_EEPROM));
              #else
                DWIN_Frame_AreaCopy(1, 56, 16, 271 - 130, 479 - 450 - 1, 14, 8);
                DWIN_Frame_AreaCopy(1, 157, 76, 181, 479 - 393, LBLX, MBASE(1));
                DWIN_Frame_AreaCopy(1, 197, 104, 271 - 33, 479 - 365, LBLX + 24 + 3, MBASE(1));
                DWIN_Frame_AreaCopy(1, 1, 89, 271 - 188, 479 - 377 - 1, LBLX + 24 + 41 + 6, MBASE(1)); // PLA nozzle temp
                DWIN_Frame_AreaCopy(1, 157, 76, 181, 479 - 393, LBLX, MBASE(2) + 3);
                DWIN_Frame_AreaCopy(1, 240, 104, 271 - 7, 479 - 365, LBLX + 24 + 3, MBASE(2) + 3);
                DWIN_Frame_AreaCopy(1, 1, 89, 271 - 188, 479 - 377 - 1, LBLX + 24 + 24 + 6, MBASE(2) + 3); // PLA bed temp
                DWIN_Frame_AreaCopy(1, 157, 76, 181, 479 - 393, LBLX, MBASE(3));
                DWIN_Frame_AreaCopy(1, 0, 119, 271 - 207, 479 - 347, LBLX + 24 + 3, MBASE(3)); // PLA fan speed
                DWIN_Frame_AreaCopy(1, 97, 165, 271 - 42, 479 - 301 - 1, LBLX, MBASE(4)); // save PLA configuration
              #endif
            }

            Draw_Back_First();

            Draw_Menu_Line(1, ICON_SetEndTemp);
            Draw_Menu_Line(2, ICON_SetBedTemp);
            Draw_Menu_Line(3, ICON_FanSpeed);
            Draw_Menu_Line(4, ICON_WriteEEPROM);

            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(1), ui.material_preset[0].hotend_temp);
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2), ui.material_preset[0].bed_temp);
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3), ui.material_preset[0].fan_speed);

            break;

          case 5: // ABS preheat setting
            checkkey = ABSPreheat;
            select_ABS.reset();
            HMI_ValueStruct.show_mode = -3;

            Clear_Main_Window();

            if (HMI_flag.language_chinese) {
              DWIN_Frame_AreaCopy(1, 142, 16, 271 - 48, 479 - 450, 14, 8);

              DWIN_Frame_AreaCopy(1, 180, 89, 204, 479 - 379, LBLX, MBASE(1));
              DWIN_Frame_AreaCopy(1, 1, 134, 271 - 215, 479 - 333, LBLX + 24, MBASE(1)); // ABS nozzle temp
              DWIN_Frame_AreaCopy(1, 180, 89, 204, 479 - 379, LBLX, MBASE(2));
              DWIN_Frame_AreaCopy(1, 58, 134, 271 - 158, 479 - 333, LBLX + 24, MBASE(2)); // ABS bed temp
              DWIN_Frame_AreaCopy(1, 180, 89, 204, 479 - 379, LBLX, MBASE(3));
              DWIN_Frame_AreaCopy(1, 115, 134, 271 - 101, 479 - 333, LBLX + 24, MBASE(3)); // ABS fan speed
              DWIN_Frame_AreaCopy(1, 72, 148, 271 - 120, 479 - 317, LBLX, MBASE(4));
              DWIN_Frame_AreaCopy(1, 180, 89, 204, 479 - 379, LBLX + 28, MBASE(4) + 2); // save ABS configuration
            }
            else {
              #ifdef USE_STRING_HEADINGS
                Draw_Title("ABS Settings"); // TODO: GET_TEXT_F
                DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)"Nozzle Temp");
                DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)"Bed Temp");
                DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)GET_TEXT_F(MSG_FAN_SPEED));
                DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)GET_TEXT_F(MSG_STORE_EEPROM));
              #else
                DWIN_Frame_AreaCopy(1, 56, 16, 271 - 130, 479 - 450 - 1, 14, 8);
                DWIN_Frame_AreaCopy(1, 172, 76, 198, 479 - 393, LBLX, MBASE(1));
                DWIN_Frame_AreaCopy(1, 197, 104, 271 - 33, 479 - 365, LBLX + 24 + 3, MBASE(1));
                DWIN_Frame_AreaCopy(1, 1, 89, 271 - 188, 479 - 377 - 1, LBLX + 24 + 41 + 6, MBASE(1)); // ABS nozzle temp
                DWIN_Frame_AreaCopy(1, 172, 76, 198, 479 - 393, LBLX, MBASE(2) + 3);
                DWIN_Frame_AreaCopy(1, 240, 104, 271 - 7, 479 - 365, LBLX + 24 + 3, MBASE(2) + 3);
                DWIN_Frame_AreaCopy(1, 1, 89, 271 - 188, 479 - 377 - 1, LBLX + 24 + 24 + 6, MBASE(2) + 3); // ABS bed temp
                DWIN_Frame_AreaCopy(1, 172, 76, 198, 479 - 393, LBLX, MBASE(3));
                DWIN_Frame_AreaCopy(1, 0, 119, 271 - 207, 479 - 347, LBLX + 24 + 3, MBASE(3)); // ABS fan speed
                DWIN_Frame_AreaCopy(1, 97, 165, 271 - 42, 479 - 301 - 1, LBLX, MBASE(4));
                DWIN_Frame_AreaCopy(1, 172, 76, 198, 479 - 393, LBLX + 33, MBASE(4));  // save ABS configuration
              #endif
            }

            Draw_Back_First();

            Draw_Menu_Line(1, ICON_SetEndTemp);
            Draw_Menu_Line(2, ICON_SetBedTemp);
            Draw_Menu_Line(3, ICON_FanSpeed);
            Draw_Menu_Line(4, ICON_WriteEEPROM);

            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(1), ui.material_preset[1].hotend_temp);
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(2), ui.material_preset[1].bed_temp);
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 3, 216, MBASE(3), ui.material_preset[1].fan_speed);

            break;
        #endif // if HAS_HOTEND
    }
  }
  DWIN_UpdateLCD();
}

inline void Draw_Max_Speed_Menu() {
  Clear_Main_Window();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 1, 16, 271 - 243, 479 - 451, 14, 8);

    auto say_max_speed = [](const uint16_t row) {
      DWIN_Frame_AreaCopy(1, 173, 133, 228, 479 - 332, LBLX, row);        // "Max speed"
    };

    say_max_speed(MBASE(1));                                                // "Max speed"
    DWIN_Frame_AreaCopy(1, 229, 133, 236, 479 - 332, LBLX + 55 + 3, MBASE(1)); // X
    say_max_speed(MBASE(2));                                                // "Max speed"
    DWIN_Frame_AreaCopy(1, 1, 150, 271 - 264, 479 - 319, LBLX + 55 + 3, MBASE(2) + 3); // Y
    say_max_speed(MBASE(3));                                                // "Max speed"
    DWIN_Frame_AreaCopy(1, 9, 150, 271 - 255, 479 - 319, LBLX + 55 + 3, MBASE(3) + 3); // Z
    say_max_speed(MBASE(4));                                                // "Max speed"
    DWIN_Frame_AreaCopy(1, 18, 150, 271 - 246, 479 - 319, LBLX + 55 + 3, MBASE(4) + 3); // E
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title("Max Speed (mm/s)"); // TODO: GET_TEXT_F
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)"Max Feedrate X");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)"Max Feedrate Y");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)"Max Feedrate Z");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)"Max Feedrate E");
    #else
      DWIN_Frame_AreaCopy(1, 144, 16, 271 - 82, 479 - 453, 14, 8);

      draw_max_en(MBASE(1));            // "Max"
      DWIN_Frame_AreaCopy(1, 184, 119, 271 - 37, 479 - 347, LBLX + 24 + 3, MBASE(1)); // "Speed X"

      draw_max_en(MBASE(2));            // "Max"
      draw_speed_en(24 + 3, MBASE(2));  // "Speed"
      say_y(24 + 40 + 6, MBASE(2));     // "Y"

      draw_max_en(MBASE(3));            // "Max"
      draw_speed_en(24 + 3, MBASE(3));  // "Speed"
      say_z(24 + 40 + 6, MBASE(3));     // "Z"

      draw_max_en(MBASE(4));            // "Max"
      draw_speed_en(24 + 3, MBASE(4));  // "Speed"
      say_e(24 + 40 + 6, MBASE(4));     // "E"
    #endif
  }

  Draw_Back_First();
  LOOP_L_N(i, 4) Draw_Menu_Line(i + 1, ICON_MaxSpeedX + i);

  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(1), planner.settings.max_feedrate_mm_s[X_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(2), planner.settings.max_feedrate_mm_s[Y_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(3), planner.settings.max_feedrate_mm_s[Z_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(4), planner.settings.max_feedrate_mm_s[E_AXIS]);
}

inline void Draw_Max_Accel_Menu() {
  Clear_Main_Window();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 1, 16, 271 - 243, 479 - 451, 14, 8);

    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(1));
    DWIN_Frame_AreaCopy(1, 28, 149, 271 - 202, 479 - 318, LBLX + 27, MBASE(1) + 1);
    DWIN_Frame_AreaCopy(1, 229, 133, 236, 479 - 332, LBLX + 27 + 41 + 3, MBASE(1)); // max acceleration X
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(2));
    DWIN_Frame_AreaCopy(1, 28, 149, 271 - 202, 479 - 318, LBLX + 27, MBASE(2) + 1);
    DWIN_Frame_AreaCopy(1, 1, 150, 271 - 264, 479 - 319, LBLX + 27 + 41 + 3, MBASE(2) + 2); // max acceleration Y
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(3));
    DWIN_Frame_AreaCopy(1, 28, 149, 271 - 202, 479 - 318, LBLX + 27, MBASE(3) + 1);
    DWIN_Frame_AreaCopy(1, 9, 150, 271 - 255, 479 - 319, LBLX + 27 + 41 + 3, MBASE(3) + 2); // max acceleration Z
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(4));
    DWIN_Frame_AreaCopy(1, 28, 149, 271 - 202, 479 - 318, LBLX + 27, MBASE(4) + 1);
    DWIN_Frame_AreaCopy(1, 18, 150, 271 - 246, 479 - 319, LBLX + 27 + 41 + 3, MBASE(4) + 2); // max acceleration E
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_ACCELERATION));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)"Max Accel X");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)"Max Accel Y");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)"Max Accel Z");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)"Max Accel E");
    #else
      DWIN_Frame_AreaCopy(1, 144, 16, 271 - 82, 479 - 453, 14, 8);
      draw_max_accel_en(MBASE(1)); say_x(24 + 78 + 6, MBASE(1)); // "Max Acceleration X"
      draw_max_accel_en(MBASE(2)); say_y(24 + 78 + 6, MBASE(2)); // "Max Acceleration Y"
      draw_max_accel_en(MBASE(3)); say_z(24 + 78 + 6, MBASE(3)); // "Max Acceleration Z"
      draw_max_accel_en(MBASE(4)); say_e(24 + 78 + 6, MBASE(4)); // "Max Acceleration E"
    #endif
  }

  Draw_Back_First();
  LOOP_L_N(i, 4) Draw_Menu_Line(i + 1, ICON_MaxAccX + i);

  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(1), planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(2), planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(3), planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, White, Background_black, 4, 210, MBASE(4), planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
}

inline void Draw_Max_Jerk_Menu() {
  Clear_Main_Window();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 1, 16, 271 - 243, 479 - 451, 14, 8);

    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(1));
    DWIN_Frame_AreaCopy(1, 1, 180, 271 - 243, 479 - 287, LBLX + 27, MBASE(1) + 1);
    DWIN_Frame_AreaCopy(1, 202, 133, 228, 479 - 332, LBLX + 53, MBASE(1));
    DWIN_Frame_AreaCopy(1, 229, 133, 236, 479 - 332, LBLX + 80 + 3, MBASE(1)); // max corner speed X
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(2));
    DWIN_Frame_AreaCopy(1, 1, 180, 271 - 243, 479 - 287, LBLX + 27, MBASE(2) + 1);
    DWIN_Frame_AreaCopy(1, 202, 133, 228, 479 - 332, LBLX + 53, MBASE(2));
    DWIN_Frame_AreaCopy(1, 1, 150, 271 - 264, 479 - 319, LBLX + 80 + 3, MBASE(2) + 3); // max corner speed Y
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(3));
    DWIN_Frame_AreaCopy(1, 1, 180, 271 - 243, 479 - 287, LBLX + 27, MBASE(3) + 1);
    DWIN_Frame_AreaCopy(1, 202, 133, 228, 479 - 332, LBLX + 53, MBASE(3));
    DWIN_Frame_AreaCopy(1, 9, 150, 271 - 255, 479 - 319, LBLX + 80 + 3, MBASE(3) + 3); // max corner speed Z
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 479 - 332, LBLX, MBASE(4));
    DWIN_Frame_AreaCopy(1, 1, 180, 271 - 243, 479 - 287, LBLX + 27, MBASE(4) + 1);
    DWIN_Frame_AreaCopy(1, 202, 133, 228, 479 - 332, LBLX + 53, MBASE(4));
    DWIN_Frame_AreaCopy(1, 18, 150, 271 - 246, 479 - 319, LBLX + 80 + 3, MBASE(4) + 3); // max corner speed E
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_JERK));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)"Max Jerk X");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)"Max Jerk Y");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)"Max Jerk Z");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)"Max Jerk E");
    #else
      DWIN_Frame_AreaCopy(1, 144, 16, 271 - 82, 479 - 453, 14, 8);
      draw_max_en(MBASE(1));            // "Max"
      draw_corner_en(MBASE(1));         // "Corner"
      draw_speed_en(66 + 6, MBASE(1));  // "Speed"
      say_x(106 + 9, MBASE(1));         // "X"

      draw_max_en(MBASE(2));            // "Max"
      draw_corner_en(MBASE(2));         // "Corner"
      draw_speed_en(66 + 6, MBASE(2));  // "Speed"
      say_y(106 + 9, MBASE(2));         // "Y"

      draw_max_en(MBASE(3));            // "Max"
      draw_corner_en(MBASE(3));         // "Corner"
      draw_speed_en(66 + 6, MBASE(3));  // "Speed"
      say_z(106 + 9, MBASE(3));         // "Z"

      draw_max_en(MBASE(4));            // "Max"
      draw_corner_en(MBASE(4));         // "Corner"
      draw_speed_en(66 + 6, MBASE(4));  // "Speed"
      say_e(106 + 9, MBASE(4));         // "E"
    #endif
  }

  Draw_Back_First();
  LOOP_L_N(i, 4) Draw_Menu_Line(i + 1, ICON_MaxSpeedCornerX + i);

  DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(1), planner.max_jerk[X_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(2), planner.max_jerk[Y_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(3), planner.max_jerk[Z_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(4), planner.max_jerk[E_AXIS] * MINUNITMULT);
}

inline void Draw_Steps_Menu() {
  Clear_Main_Window();

  if (HMI_flag.language_chinese) {
    DWIN_Frame_AreaCopy(1, 1, 16, 271 - 243, 479 - 451, 14, 8);

    DWIN_Frame_AreaCopy(1, 153, 148, 271 - 77, 479 - 318, LBLX, MBASE(1));
    DWIN_Frame_AreaCopy(1, 229, 133, 236, 479 - 332, LBLX + 41 + 3, MBASE(1)); // Transmission Ratio X
    DWIN_Frame_AreaCopy(1, 153, 148, 271 - 77, 479 - 318, LBLX, MBASE(2));
    DWIN_Frame_AreaCopy(1, 1, 150, 271 - 264, 479 - 319, LBLX + 41 + 3, MBASE(2) + 3); // Transmission Ratio Y
    DWIN_Frame_AreaCopy(1, 153, 148, 271 - 77, 479 - 318, LBLX, MBASE(3));
    DWIN_Frame_AreaCopy(1, 9, 150, 271 - 255, 479 - 319, LBLX + 41 + 3, MBASE(3) + 3); // Transmission Ratio Z
    DWIN_Frame_AreaCopy(1, 153, 148, 271 - 77, 479 - 318, LBLX, MBASE(4));
    DWIN_Frame_AreaCopy(1, 18, 150, 271 - 246, 479 - 319, LBLX + 41 + 3, MBASE(4) + 3); // Transmission Ratio E
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_STEPS_PER_MM));
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 0), (char*)"Steps/mm X");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 1), (char*)"Steps/mm Y");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 2), (char*)"Steps/mm Z");
      DWIN_Draw_String(false, true, font8x16, White, Background_black, 60, 102 + (MLINE * 3), (char*)"Steps/mm E");
    #else
      DWIN_Frame_AreaCopy(1, 144, 16, 271 - 82, 479 - 453, 14, 8);
      draw_steps_per_mm(MBASE(1)); say_x(100 + 3, MBASE(1)); // "Steps-per-mm X"
      draw_steps_per_mm(MBASE(2)); say_y(100 + 3, MBASE(2)); // "Y"
      draw_steps_per_mm(MBASE(3)); say_z(100 + 3, MBASE(3)); // "Z"
      draw_steps_per_mm(MBASE(4)); say_e(100 + 3, MBASE(4)); // "E"
    #endif
  }

  Draw_Back_First();
  LOOP_L_N(i, 4) Draw_Menu_Line(i + 1, ICON_StepX + i);

  DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(1), planner.settings.axis_steps_per_mm[X_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(2), planner.settings.axis_steps_per_mm[Y_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(3), planner.settings.axis_steps_per_mm[Z_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Background_black, 3, 1, 210, MBASE(4), planner.settings.axis_steps_per_mm[E_AXIS] * MINUNITMULT);
}

/* Motion */
void HMI_Motion(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_motion.inc(4)) Move_Highlight(1, select_motion.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_motion.dec()) Move_Highlight(-1, select_motion.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_motion.now) {
      case 0: // back
        checkkey = Control;
        select_control.set(2);
        index_control = MROWS;
        Draw_Control_Menu();
        break;
      case 1: // max speed
        checkkey = MaxSpeed;
        select_speed.reset();
        Draw_Max_Speed_Menu();
        break;
      case 2: // max acceleration
        checkkey = MaxAcceleration;
        select_acc.reset();
        Draw_Max_Accel_Menu();
        break;
      case 3: // max corner speed
        checkkey = MaxCorner;
        select_corner.reset();
        Draw_Max_Jerk_Menu();
        break;
      case 4: // transmission ratio
        checkkey = Step;
        select_step.reset();
        Draw_Steps_Menu();
        break;
      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}

/* Info */
void HMI_Info(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    #if HAS_ONESTEP_LEVELING
      checkkey = Control;
      select_control.set(CONTROL_ITEMS);
      Draw_Control_Menu();
    #else
      select_page.set(3);
      Goto_MainMenu();
    #endif
  }
  DWIN_UpdateLCD();
}

/* Tune */
void HMI_Tune(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_tune.inc(6)) {
      if (select_tune.now > MROWS && select_tune.now > index_tune) {
        index_tune = select_tune.now;
        Scroll_Menu(DWIN_SCROLL_UP);
        Prepare_Item_Lang(5);
      }
      else {
        Move_Highlight(1, select_tune.now + MROWS - index_tune);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_tune.dec()) {
      if (select_tune.now < index_tune - MROWS) {
        index_tune--;
        Scroll_Menu(DWIN_SCROLL_DOWN);
        if (index_tune == MROWS) Draw_Back_First();
      }
      else {
        Move_Highlight(-1, select_tune.now + MROWS - index_tune);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_tune.now) {
      case 0: { // Back
        select_print.set(0);
        Goto_PrintProcess();
      }
      break;
      case 1: // Print speed
        checkkey = PrintSpeed;
        HMI_ValueStruct.print_speed = feedrate_percentage;
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(1 + MROWS - index_tune), feedrate_percentage);
        EncoderRate.encoderRateEnabled = 1;
        break;
        #if HAS_HOTEND
          case 2: // Nozzle temp
            checkkey = ETemp;
            HMI_ValueStruct.E_Temp = thermalManager.temp_hotend[0].target;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(2 + MROWS - index_tune), thermalManager.temp_hotend[0].target);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
        #if HAS_HEATED_BED
          case 3: // Bed temp
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = thermalManager.temp_bed.target;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(3 + MROWS - index_tune), thermalManager.temp_bed.target);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
        #if HAS_FAN
          case 4: // Fan speed
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = thermalManager.fan_speed[0];
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(4 + MROWS - index_tune), thermalManager.fan_speed[0]);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
      case 5: // Z-offset
        checkkey = Homeoffset;
        HMI_ValueStruct.offset_value = BABY_Z_VAR * 100;
        show_plus_or_minus(font8x16, Select_Color, 2, 2, 202, MBASE(5 + MROWS - index_tune), HMI_ValueStruct.offset_value);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 6: // Language
        // Select language
        HMI_flag.language_chinese ^= true;

        Clear_Main_Window();

        if (HMI_flag.language_chinese) {
          HMI_SetAndSaveLanguageChinese();
          DWIN_JPG_CacheTo1(Language_Chinese);
        }
        else {
          HMI_SetAndSaveLanguageWestern();
          DWIN_JPG_CacheTo1(Language_English);
        }

        Draw_Tune_Menu();
        break;

      default: break;
    }
  }
  DWIN_UpdateLCD();
}

/* PLA Preheat */
void HMI_PLAPreheatSetting(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_PLA.inc(4)) Move_Highlight(1, select_PLA.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_PLA.dec()) Move_Highlight(-1, select_PLA.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_PLA.now) {
      case 0: // back
        checkkey = TemperatureID;
        select_temp.now = 4;
        HMI_ValueStruct.show_mode = -1;
        Draw_Temperature_Menu();
        break;
        #if HAS_HOTEND
          case 1: // set nozzle temperature
            checkkey = ETemp;
            HMI_ValueStruct.E_Temp = ui.material_preset[0].hotend_temp;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(1), ui.material_preset[0].hotend_temp);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
        #if HAS_HEATED_BED
          case 2: // set bed temperature
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = ui.material_preset[0].bed_temp;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(2), ui.material_preset[0].bed_temp);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
        #if HAS_FAN
          case 3: // set fan speed
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = ui.material_preset[0].fan_speed;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(3), ui.material_preset[0].fan_speed);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
      case 4: { // save PLA configuration
        const bool success = settings.save();
        HMI_AudioFeedback(success);
      } break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

/* ABS Preheat */
void HMI_ABSPreheatSetting(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_ABS.inc(4)) Move_Highlight(1, select_ABS.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_ABS.dec()) Move_Highlight(-1, select_ABS.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_ABS.now) {
      case 0: // back
        checkkey = TemperatureID;
        select_temp.now = 5;
        HMI_ValueStruct.show_mode = -1;
        Draw_Temperature_Menu();
        break;
        #if HAS_HOTEND
          case 1: // set nozzle temperature
            checkkey = ETemp;
            HMI_ValueStruct.E_Temp = ui.material_preset[1].hotend_temp;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(1), ui.material_preset[1].hotend_temp);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
        #if HAS_HEATED_BED
          case 2: // set bed temperature
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = ui.material_preset[1].bed_temp;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(2), ui.material_preset[1].bed_temp);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
        #if HAS_FAN
          case 3: // set fan speed
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = ui.material_preset[1].fan_speed;
            DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 3, 216, MBASE(3), ui.material_preset[1].fan_speed);
            EncoderRate.encoderRateEnabled = 1;
            break;
        #endif
      case 4: { // save ABS configuration
        const bool success = settings.save();
        HMI_AudioFeedback(success);
      } break;
      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}

/* Max Speed */
void HMI_MaxSpeed(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_speed.inc(4)) Move_Highlight(1, select_speed.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_speed.dec()) Move_Highlight(-1, select_speed.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_speed.now) {
      case 0: // back
        checkkey = Motion;
        select_motion.now = 1;
        Draw_Motion_Menu();
        break;
      case 1: // max Speed X
        checkkey                      = MaxSpeed_value;
        HMI_flag.feedspeed_flag       = X_AXIS;
        HMI_ValueStruct.Max_Feedspeed = planner.settings.max_feedrate_mm_s[X_AXIS];
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 2: // max Speed Y
        checkkey                      = MaxSpeed_value;
        HMI_flag.feedspeed_flag       = Y_AXIS;
        HMI_ValueStruct.Max_Feedspeed = planner.settings.max_feedrate_mm_s[Y_AXIS];
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 3: // max Speed Z
        checkkey                      = MaxSpeed_value;
        HMI_flag.feedspeed_flag       = Z_AXIS;
        HMI_ValueStruct.Max_Feedspeed = planner.settings.max_feedrate_mm_s[Z_AXIS];
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 4: // max Speed E
        checkkey                      = MaxSpeed_value;
        HMI_flag.feedspeed_flag       = E_AXIS;
        HMI_ValueStruct.Max_Feedspeed = planner.settings.max_feedrate_mm_s[E_AXIS];
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
        EncoderRate.encoderRateEnabled = 1;
        break;
      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}

/* Max Acceleration */
void HMI_MaxAcceleration(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_acc.inc(4)) Move_Highlight(1, select_acc.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_acc.dec()) Move_Highlight(-1, select_acc.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_acc.now) {
      case 0: // back
        checkkey          = Motion;
        select_motion.now = 2;
        Draw_Motion_Menu();
        break;
      case 1: // max acceleration X
        checkkey                         = MaxAcceleration_value;
        HMI_flag.acc_flag                = X_AXIS;
        HMI_ValueStruct.Max_Acceleration = planner.settings.max_acceleration_mm_per_s2[X_AXIS];
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 2: // max acceleration Y
        checkkey                         = MaxAcceleration_value;
        HMI_flag.acc_flag                = Y_AXIS;
        HMI_ValueStruct.Max_Acceleration = planner.settings.max_acceleration_mm_per_s2[Y_AXIS];
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 3: // max acceleration Z
        checkkey                         = MaxAcceleration_value;
        HMI_flag.acc_flag                = Z_AXIS;
        HMI_ValueStruct.Max_Acceleration = planner.settings.max_acceleration_mm_per_s2[Z_AXIS];
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 4: // max acceleration E
        checkkey                         = MaxAcceleration_value;
        HMI_flag.acc_flag                = E_AXIS;
        HMI_ValueStruct.Max_Acceleration = planner.settings.max_acceleration_mm_per_s2[E_AXIS];
        DWIN_Draw_IntValue(true, true, 0, font8x16, White, Select_Color, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
        EncoderRate.encoderRateEnabled = 1;
        break;
      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}

/* Max Corner */
void HMI_MaxCorner(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_corner.inc(4)) Move_Highlight(1, select_corner.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_corner.dec()) Move_Highlight(-1, select_corner.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_corner.now) {
      case 0: // back
        checkkey          = Motion;
        select_motion.now = 3;
        Draw_Motion_Menu();
        break;
      case 1: // max corner X
        checkkey                   = MaxCorner_value;
        HMI_flag.corner_flag       = X_AXIS;
        HMI_ValueStruct.Max_Corner = planner.max_jerk[X_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_corner.now), HMI_ValueStruct.Max_Corner);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 2: // max corner Y
        checkkey                   = MaxCorner_value;
        HMI_flag.corner_flag       = Y_AXIS;
        HMI_ValueStruct.Max_Corner = planner.max_jerk[Y_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_corner.now), HMI_ValueStruct.Max_Corner);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 3: // max corner Z
        checkkey                   = MaxCorner_value;
        HMI_flag.corner_flag       = Z_AXIS;
        HMI_ValueStruct.Max_Corner = planner.max_jerk[Z_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_corner.now), HMI_ValueStruct.Max_Corner);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 4: // max corner E
        checkkey                   = MaxCorner_value;
        HMI_flag.corner_flag       = E_AXIS;
        HMI_ValueStruct.Max_Corner = planner.max_jerk[E_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_corner.now), HMI_ValueStruct.Max_Corner);
        EncoderRate.encoderRateEnabled = 1;
        break;
      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}

/* Step */
void HMI_Step(void) {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_step.inc(4)) Move_Highlight(1, select_step.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_step.dec()) Move_Highlight(-1, select_step.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_step.now) {
      case 0: // back
        checkkey = Motion;
        select_motion.now = 4;
        Draw_Motion_Menu();
        break;
      case 1: // max step X
        checkkey                 = Step_value;
        HMI_flag.step_flag       = X_AXIS;
        HMI_ValueStruct.Max_Step = planner.settings.axis_steps_per_mm[X_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 2: // max step Y
        checkkey                 = Step_value;
        HMI_flag.step_flag       = Y_AXIS;
        HMI_ValueStruct.Max_Step = planner.settings.axis_steps_per_mm[Y_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 3: // max step Z
        checkkey                 = Step_value;
        HMI_flag.step_flag       = Z_AXIS;
        HMI_ValueStruct.Max_Step = planner.settings.axis_steps_per_mm[Z_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step);
        EncoderRate.encoderRateEnabled = 1;
        break;
      case 4: // max step E
        checkkey                 = Step_value;
        HMI_flag.step_flag       = E_AXIS;
        HMI_ValueStruct.Max_Step = planner.settings.axis_steps_per_mm[E_AXIS] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, White, Select_Color, 3, 1, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step);
        EncoderRate.encoderRateEnabled = 1;
        break;
      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}

void HMI_Init(void) {
  HMI_SDCardInit();

  for (uint16_t t = 0; t <= 100; t += 2) {
    DWIN_ICON_Show(ICON, ICON_Bar, 15, 260);
    DWIN_Draw_Rectangle(1, Background_black, 15 + t * 242 / 100, 260, 257, 280);
    DWIN_UpdateLCD();
    delay(20);
  }

  HMI_SetLanguage();
}

void DWIN_Update(void) {
  EachMomentUpdate();   // Status update
  HMI_SDCardUpdate();   // SD card update
  DWIN_HandleScreen();  // Rotary encoder update
}

void EachMomentUpdate(void) {
  static millis_t next_rts_update_ms = 0;
  const millis_t ms = millis();
  if (PENDING(ms, next_rts_update_ms)) return;
  next_rts_update_ms = ms + DWIN_SCROLL_UPDATE_INTERVAL;

  // variable update
  update_variable();

  if (checkkey == PrintProcess) {
    // if print done
    if (HMI_flag.print_finish && !HMI_flag.confirm_flag) {
      HMI_flag.print_finish = 0;
      HMI_flag.confirm_flag = 1;

      TERN_(POWER_LOSS_RECOVERY, recovery.cancel());

      planner.finish_and_disable();

      // show percent bar and value
      Percentrecord = 0;
      Draw_Print_ProgressBar();

      // show print done confirm
      DWIN_Draw_Rectangle(1, Background_black, 0, 250,  271, 360);
      DWIN_ICON_Show(ICON, HMI_flag.language_chinese ? ICON_Confirm_C : ICON_Confirm_E, 86, 302 - 19);
    }
    else if (HMI_flag.pause_flag != printingIsPaused()) {
      // print status update
      HMI_flag.pause_flag = printingIsPaused();
      if (HMI_flag.pause_flag) ICON_Continue(); else ICON_Pause();
    }
  }

  // pause after homing
  if (pause_action_flag && printingIsPaused() && !planner.has_blocks_queued()) {
    pause_action_flag = 0;
    #if ENABLED(PAUSE_HEAT)
      tempbed = thermalManager.temp_bed.target;
      temphot = thermalManager.temp_hotend[0].target;
      thermalManager.disable_all_heaters();
    #endif
    queue.inject_P(PSTR("G1 F1200 X0 Y0"));
  }

  if (card.isPrinting() && checkkey == PrintProcess) { // print process
    const uint8_t card_pct = card.percentDone();
    static uint8_t last_cardpercentValue = 101;
    if (last_cardpercentValue != card_pct) { // print percent
      last_cardpercentValue = card_pct;
      if (card_pct) {
        Percentrecord = card_pct;
        Draw_Print_ProgressBar();
      }
    }

    duration_t elapsed = print_job_timer.duration(); // print timer
    /* already print time */
    const uint16_t min = (elapsed.value % 3600) / 60;
    if (last_Printtime != min) { // 1 minute update
      last_Printtime = min;
      Draw_Print_ProgressElapsed();
    }
    /* remain print time */
    static millis_t next_remain_time_update = 0;
    if (elapsed.minute() > 5 && ELAPSED(ms, next_remain_time_update) && HMI_flag.heat_flag == 0) { // show after 5 min and 20s update
      remain_time = ((elapsed.value - dwin_heat_time) * ((float)card.getFileSize() / (float)card.getIndex())) - (elapsed.value - dwin_heat_time);
      next_remain_time_update += 20 * 1000UL;
      Draw_Print_ProgressRemain();
    }
  }
  else if (abort_flag && !HMI_flag.home_flag) { // Print Stop
    abort_flag = 0;
    HMI_ValueStruct.print_speed = feedrate_percentage = 100;
    zprobe_zoffset = TERN(HAS_BED_PROBE, probe.offset.z, 0);

    planner.finish_and_disable();

    #if DISABLED(SD_ABORT_NO_COOLDOWN)
      thermalManager.disable_all_heaters();
    #endif

    select_page.set(0);
    Goto_MainMenu();
  }
  else if (DWIN_lcd_sd_status && recovery.dwin_flag) { // resume print before power off
    recovery.dwin_flag = false;

    recovery.load();
    if (!recovery.valid()) return recovery.purge();

    auto draw_first_option = [](const bool sel) {
      const uint16_t c1 = sel ? Background_window : Select_Color;
      DWIN_Draw_Rectangle(0, c1, 25, 306, 126, 345);
      DWIN_Draw_Rectangle(0, c1, 24, 305, 127, 346);
    };

    auto update_selection = [&](const bool sel) {
      HMI_flag.select_flag = sel;
      draw_first_option(sel);
      const uint16_t c2 = sel ? Select_Color : Background_window;
      DWIN_Draw_Rectangle(0, c2, 145, 306, 246, 345);
      DWIN_Draw_Rectangle(0, c2, 144, 305, 247, 346);
    };

    const uint16_t fileCnt = card.get_num_Files();
    for (uint16_t i = 0; i < fileCnt; i++) {
      // TODO: Resume print via M1000 then update the UI
      // with the active filename which can come from CardReader.
      card.getfilename_sorted(SD_ORDER(i, fileCnt));
      if (!strcmp(card.filename, &recovery.info.sd_filename[1])) { // Resume print before power failure while have the same file
        recovery_flag = 1;
        HMI_flag.select_flag = 1;
        Popup_Window_Resume();
        draw_first_option(false);
        char * const name = card.longest_filename();
        const int8_t npos = _MAX(0, DWIN_WIDTH - strlen(name) * (MENU_CHR_W)) / 2;
        DWIN_Draw_String(false, true, font8x16, Font_window, Background_window, npos, 252, name);
        DWIN_UpdateLCD();
        break;
      }
    }

    // if hasn't resumable G-code file
    if (!recovery_flag) return;

    while (recovery_flag) {
      ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
      if (encoder_diffState != ENCODER_DIFF_NO) {
        if (encoder_diffState == ENCODER_DIFF_ENTER) {
          recovery_flag = 0;
          if (HMI_flag.select_flag) break;
          TERN_(POWER_LOSS_RECOVERY, recovery.cancel());
          HMI_StartFrame(true);
          return;
        }
        else
          update_selection(encoder_diffState == ENCODER_DIFF_CCW);

        DWIN_UpdateLCD();
      }
    }

    select_print.set(0);
    HMI_ValueStruct.show_mode = 0;
    HMI_StartFrame(false);
    recovery.resume();
    return;
  }
  DWIN_UpdateLCD();
}

void DWIN_HandleScreen(void) {
  switch (checkkey) {
    case MainMenu:              HMI_MainMenu(); break;
    case SelectFile:            HMI_SelectFile(); break;
    case Prepare:               HMI_Prepare(); break;
    case Control:               HMI_Control(); break;
    case Leveling:              break;
    case PrintProcess:          HMI_Printing(); break;
    case Print_window:          HMI_PauseOrStop(); break;
    case AxisMove:              HMI_AxisMove(); break;
    case TemperatureID:         HMI_Temperature(); break;
    case Motion:                HMI_Motion(); break;
    case Info:                  HMI_Info(); break;
    case Tune:                  HMI_Tune(); break;
    case PLAPreheat:            HMI_PLAPreheatSetting(); break;
    case ABSPreheat:            HMI_ABSPreheatSetting(); break;
    case MaxSpeed:              HMI_MaxSpeed(); break;
    case MaxAcceleration:       HMI_MaxAcceleration(); break;
    case MaxCorner:             HMI_MaxCorner(); break;
    case Step:                  HMI_Step(); break;
    case Move_X:                HMI_Move_X(); break;
    case Move_Y:                HMI_Move_Y(); break;
    case Move_Z:                HMI_Move_Z(); break;
    #if HAS_HOTEND
      case Extruder:            HMI_Move_E(); break;
      case ETemp:               HMI_ETemp(); break;
    #endif
    case Homeoffset:            HMI_Zoffset(); break;
    #if HAS_HEATED_BED
      case BedTemp:             HMI_BedTemp(); break;
    #endif
    #if HAS_FAN
      case FanSpeed:            HMI_FanSpeed(); break;
    #endif
    case PrintSpeed:            HMI_PrintSpeed(); break;
    case MaxSpeed_value:        HMI_MaxFeedspeedXYZE(); break;
    case MaxAcceleration_value: HMI_MaxAccelerationXYZE(); break;
    case MaxCorner_value:       HMI_MaxCornerXYZE(); break;
    case Step_value:            HMI_StepXYZE(); break;
    default: break;
  }
}

void DWIN_CompletedHoming(void) {
  HMI_flag.home_flag = false;
  if (checkkey == Last_Prepare) {
    checkkey = Prepare;
    select_prepare.now = 3;
    index_prepare = MROWS;
    Draw_Prepare_Menu();
  }
  else if (checkkey == Back_Main) {
    HMI_ValueStruct.print_speed = feedrate_percentage = 100;
    zprobe_zoffset = TERN0(BLTOUCH, probe.offset.z);
    planner.finish_and_disable();
    Goto_MainMenu();
  }
}

void DWIN_CompletedLeveling(void) {
  if (checkkey == Leveling) Goto_MainMenu();
}

#endif // DWIN_CREALITY_LCD
