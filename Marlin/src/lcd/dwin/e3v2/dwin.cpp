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
 * Rewrite by Jacob Myers
 */

#include "../../../inc/MarlinConfigPre.h"

#include "dwin.h"

#include <WString.h>
#include <stdio.h>
#include <string.h>

#include "../../fontutils.h"
#include "../../marlinui.h"

#include "../../../sd/cardreader.h"

#include "../../../MarlinCore.h"
#include "../../../core/serial.h"
#include "../../../core/macros.h"
#include "../../../gcode/queue.h"
#include "../../../gcode/gcode.h"

#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../module/settings.h"

#include "../../../libs/buzzer.h"

#if ENABLED(DWIN_CREALITY_LCD)

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../../feature/host_actions.h"
#endif

#if ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT) && DISABLED(PROBE_MANUALLY)
  #define HAS_ONESTEP_LEVELING 1
#endif

#if ANY(BABYSTEPPING, HAS_BED_PROBE, HAS_WORKSPACE_OFFSET)
  #define HAS_ZOFFSET_ITEM 1
#endif

#ifndef strcasecmp_P
  #define strcasecmp_P(a, b) strcasecmp((a), (b))
#endif

#if HAS_ONESTEP_LEVELING
  #include "../../../feature/bedlevel/bedlevel.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

#if ANY(HAS_HOTEND, HAS_HEATED_BED, HAS_FAN) && PREHEAT_COUNT
  #define HAS_PREHEAT 1
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#define MACHINE_SIZE STRINGIFY(X_BED_SIZE) "x" STRINGIFY(Y_BED_SIZE) "x" STRINGIFY(Z_MAX_POS)

<<<<<<< HEAD
#define CORP_WEBSITE_E "www.creality.com"
=======
#define USE_STRING_HEADINGS
//#define USE_STRING_TITLES
>>>>>>> origin/bugfix-2.0.x

#define DWIN_FONT_MENU font8x16
#define DWIN_FONT_STAT font10x20
#define DWIN_FONT_HEAD font10x20

#define MENU_CHAR_LIMIT  24
#define STATUS_Y 352

#define MAX_PRINT_SPEED   500
#define MIN_PRINT_SPEED   10

#if HAS_FAN
  #define MAX_FAN_SPEED     255
  #define MIN_FAN_SPEED     0
#endif

#if HAS_ZOFFSET_ITEM
  #define MAX_Z_OFFSET 9.99
  #if HAS_BED_PROBE
    #define MIN_Z_OFFSET -9.99
  #else
    #define MIN_Z_OFFSET -1
  #endif
#endif

#if HAS_HOTEND
  #define MAX_FLOW_RATE   200
  #define MIN_FLOW_RATE   10

  #define MAX_E_TEMP    (HEATER_0_MAXTEMP - (HOTEND_OVERSHOOT))
  #define MIN_E_TEMP    HEATER_0_MINTEMP
#endif

#if HAS_HEATED_BED
  #define MAX_BED_TEMP  BED_MAXTEMP
  #define MIN_BED_TEMP  BED_MINTEMP
#endif

<<<<<<< HEAD
constexpr uint16_t TROWS = 6, MROWS = TROWS - 1,
                   TITLE_HEIGHT = 30,
                   MLINE = 53,
                   LBLX = 60,
=======
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

constexpr uint16_t TROWS = 6, MROWS = TROWS - 1,        // Total rows, and other-than-Back
                   TITLE_HEIGHT = 30,                   // Title bar height
                   MLINE = 53,                          // Menu line height
                   LBLX = 60,                           // Menu item label X
>>>>>>> origin/bugfix-2.0.x
                   MENU_CHR_W = 8, STAT_CHR_W = 10;

#define MBASE(L) (49 + MLINE * (L))

constexpr float default_max_feedrate[]        = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[]    = DEFAULT_MAX_ACCELERATION;
constexpr float default_steps[]               = DEFAULT_AXIS_STEPS_PER_UNIT;
#if HAS_CLASSIC_JERK
  constexpr float default_max_jerk[]            = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
#endif

<<<<<<< HEAD
uint8_t active_menu = MainMenu;
uint8_t selection = 0;
uint8_t scrollpos = 0;
uint8_t filescrl = 0;
uint8_t process = Main;
uint8_t popup;

void *valuepointer;
float tempvalue;
float valuemin;
float valuemax;
uint8_t valueunit;
uint8_t valuetype;

uint8_t printpercent = 0;
uint16_t remainingtime = 0;
uint16_t elapsedtime = 0;
millis_t dwin_heat_time = 0;
char lastmsg[31];
bool printing = false;
bool paused = false;
bool sdprint = false;

int16_t pausetemp, pausebed;
=======
static uint8_t _card_percent = 0;
static uint16_t _remain_time = 0;

#if ENABLED(PAUSE_HEAT)
  TERN_(HAS_HOTEND, uint16_t resume_hotend_temp = 0);
  TERN_(HAS_HEATED_BED, uint16_t resume_bed_temp = 0);
#endif
>>>>>>> origin/bugfix-2.0.x

bool liveadjust = false;
bool bedonly = false;
float zoffsetvalue = 0;
uint8_t gridpoint;

/* General Display Functions */

inline void Clear_Screen(uint8_t e/*=3*/) {
  if (e==1||e==3||e==4) DWIN_Draw_Rectangle(1, Color_Bg_Blue, 0, 0, DWIN_WIDTH, TITLE_HEIGHT); // Clear Title Bar
  if (e==2||e==3) DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 31, DWIN_WIDTH, STATUS_Y); // Clear Menu Area
  if (e==4) DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 31, DWIN_WIDTH, DWIN_HEIGHT); // Clear Popup Area
}

inline void Draw_Float(float value, uint8_t row, bool selected/*=false*/, uint8_t minunit/*=10*/) {
  uint16_t bColor;
  if (selected) bColor = Select_Color;
  else bColor = Color_Bg_Black;
  if (value < 0) {
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, bColor, 4-log10(minunit), log10(minunit), 202, MBASE(row), -value * minunit);
    DWIN_Draw_String(false, true, font8x16, Color_White, bColor, 196, MBASE(row), F("-"));
  }
  else {
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, bColor, 4-log10(minunit), log10(minunit), 202, MBASE(row), value * minunit);
    DWIN_Draw_String(false, true, font8x16, Color_White, bColor, 196, MBASE(row), F(" "));
  }
}

inline void Draw_Title(char *title) {
  DWIN_Draw_String(false, false, DWIN_FONT_HEAD, Color_White, Color_Bg_Blue, 14, 4, title);
}

inline void Draw_Menu_Item(uint8_t row, uint8_t icon/*=0*/, char *label, bool more/*=false*/) {
  if (label) DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, LBLX, MBASE(row) - 1, label); // Draw Label
  if (icon) DWIN_ICON_Show(ICON, icon, 26, MBASE(row) - 3); //Draw Menu Icon
  if (more) DWIN_ICON_Show(ICON, ICON_More, 226, MBASE(row) - 3); // Draw More Arrow
  DWIN_Draw_Line(Line_Color, 16, MBASE(row) + 33, 256, MBASE(row) + 34); // Draw Menu Line
}

inline void Draw_Menu(uint8_t menu, uint8_t select/*=0*/, uint8_t scroll/*=0*/) {
  selection = select;
  scrollpos = scroll;
  if (selection-scrollpos > MROWS)
    scrollpos = selection - MROWS;
  process = Menu;
  active_menu = menu;
  Clear_Screen();
  Draw_Title(Get_Menu_Title(menu));
  LOOP_L_N(i, TROWS) Menu_Item_Handler(menu, i + scrollpos);
  DWIN_Draw_Rectangle(1, Rectangle_Color, 0, MBASE(selection-scrollpos) - 18, 14, MBASE(selection-scrollpos) + 33);
}

/* Primary Menus and Screen Elements */

void Main_Menu_Icons() {
  if (selection == 0) {
    DWIN_ICON_Show(ICON, ICON_Print_1, 17, 130);
    DWIN_Draw_Rectangle(0, Color_White, 17, 130, 126, 229);
    DWIN_Frame_AreaCopy(1, 1, 451, 31, 463, 57, 201);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Print_0, 17, 130);
    DWIN_Frame_AreaCopy(1, 1, 423, 31, 435, 57, 201);
  }
  if (selection == 1) {
    DWIN_ICON_Show(ICON, ICON_Prepare_1, 145, 130);
    DWIN_Draw_Rectangle(0, Color_White, 145, 130, 254, 229);
    DWIN_Frame_AreaCopy(1, 33, 451, 82, 466, 175, 201);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Prepare_0, 145, 130);
    DWIN_Frame_AreaCopy(1, 33, 423, 82, 438, 175, 201);
  }
  if (selection == 2) {
    DWIN_ICON_Show(ICON, ICON_Control_1, 17, 246);
    DWIN_Draw_Rectangle(0, Color_White, 17, 246, 126, 345);
    DWIN_Frame_AreaCopy(1, 85, 451, 132, 463, 48, 318);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Control_0, 17, 246);
    DWIN_Frame_AreaCopy(1, 85, 423, 132, 434, 48, 318);
  }
  #if ANY(HAS_ONESTEP_LEVELING, PROBE_MANUALLY) 
    if (selection == 3) {
      DWIN_ICON_Show(ICON, ICON_Leveling_1, 145, 246);
      DWIN_Draw_Rectangle(0, Color_White, 145, 246, 254, 345);
      DWIN_Frame_AreaCopy(1, 84, 437, 120,  449, 182, 318);
    }
    else {
      DWIN_ICON_Show(ICON, ICON_Leveling_0, 145, 246);
      DWIN_Frame_AreaCopy(1, 84, 465, 120, 478, 182, 318);
    }
  #else
    if (selection == 3) {
      DWIN_ICON_Show(ICON, ICON_Info_1, 145, 246);
      DWIN_Draw_Rectangle(0, Color_White, 145, 246, 254, 345);
      DWIN_Frame_AreaCopy(1, 132, 451, 159, 466, 186, 318);
    }
    else {
      DWIN_ICON_Show(ICON, ICON_Info_0, 145, 246);
      DWIN_Frame_AreaCopy(1, 132, 423, 159, 435, 186, 318);
    }
  #endif
}

void Draw_Main_Menu(uint8_t select/*=0*/) {
  process = Main;
  active_menu = MainMenu;
  selection = select;
  Clear_Screen();
  Draw_Title(Get_Menu_Title(MainMenu));
  DWIN_ICON_Show(ICON, ICON_LOGO, 71, 52);
  Main_Menu_Icons();
}

void Print_Screen_Icons() {
  //DWIN_Frame_AreaCopy(1, 40,  2,  92, 14,  14,   9);  // Tune
  //DWIN_Frame_AreaCopy(1,  0, 44,  96, 58,  41, 168);  // Pause
  //DWIN_Frame_AreaCopy(1, 98, 44, 152, 58, 176, 168);  // Stop
  if (selection == 0) {
    DWIN_ICON_Show(ICON, ICON_Setup_1, 8, 252);
    DWIN_Draw_Rectangle(0, Color_White, 8, 252, 87, 351);
    DWIN_Frame_AreaCopy(1,   0, 466,  34, 476, 31, 325);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Setup_0, 8, 252);
    DWIN_Frame_AreaCopy(1,   0, 438,  32, 448, 31, 325);
  }
  if (selection == 2) {
    DWIN_ICON_Show(ICON, ICON_Stop_1, 184, 252);
    DWIN_Draw_Rectangle(0, Color_White, 184, 252, 263, 351);
    DWIN_Frame_AreaCopy(1, 218, 451, 249, 466, 209, 325);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Stop_0, 184, 252);
    DWIN_Frame_AreaCopy(1, 218, 423, 247, 436, 209, 325);
  }
  if (paused) {
    if (selection == 1) {
      DWIN_ICON_Show(ICON, ICON_Continue_1, 96, 252);
      DWIN_Draw_Rectangle(0, Color_White, 96, 252, 175, 351);
      DWIN_Frame_AreaCopy(1, 1, 452, 32, 464, 121, 325);
    }
    else {
      DWIN_ICON_Show(ICON, ICON_Continue_0, 96, 252);
      DWIN_Frame_AreaCopy(1, 1, 424, 31, 434, 121, 325);
    }
  }
  else {
    if (selection == 1) {
      DWIN_ICON_Show(ICON, ICON_Pause_1, 96, 252);
      DWIN_Draw_Rectangle(0, Color_White, 96, 252, 175, 351);
      DWIN_Frame_AreaCopy(1, 177, 451, 216, 462, 116, 325);
    }
    else {
      DWIN_ICON_Show(ICON, ICON_Pause_0, 96, 252);
      DWIN_Frame_AreaCopy(1, 177, 423, 215, 433, 116, 325);
    }
  }
}

void Draw_Print_Screen() {
  process = Print;
  selection = 0;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 352, DWIN_WIDTH-8, 376);
  Print_Screen_Icons();
  DWIN_ICON_Show(ICON, ICON_PrintTime, 14, 171);
  DWIN_ICON_Show(ICON, ICON_RemainTime, 147, 169);
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, 41, 163, (char*)"Elapsed");
  DWIN_Draw_String(false, false, font8x16,  Color_White, Color_Bg_Black, 176, 163, (char*)"Remaining");
  DWIN_Draw_Rectangle(1, Color_Grey, 8, 214, DWIN_WIDTH-8, 238);
  DWIN_Draw_Rectangle(1, Color_Grey, 8, 214, DWIN_WIDTH-8, 238);
  const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(lastmsg) * MENU_CHR_W) / 2;
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, npos, 219, lastmsg);
  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();
  if (sdprint) {
    char * const name = card.longest_filename();
    const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(name) * MENU_CHR_W) / 2;
    DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, npos, 60, name);
  }
  else {
    char * const name = (char*)"Host Print";
    const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(name) * MENU_CHR_W) / 2;
    DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, npos, 60, name);
  }
}

<<<<<<< HEAD
void Draw_Print_ProgressBar() {
  DWIN_ICON_Show(ICON, ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, BarFill_Color, 16 + printpercent * 240 / 100, 93, 256, 113);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Percent_Color, Color_Bg_Black, 3, 109, 133, printpercent);
  DWIN_Draw_String(false, false, font8x16, Percent_Color, Color_Bg_Black, 133, 133, (char*)"%");
}

void Draw_Print_ProgressRemain() {
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 176, 187, remainingtime / 3600);
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, 192, 187, (char*)":");
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 200, 187, (remainingtime % 3600) / 60);
}

void Draw_Print_ProgressElapsed() {
  duration_t elapsed = print_job_timer.duration(); // print timer
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 42, 187, elapsed.value / 3600);
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, 58, 187, (char*)":");
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 66, 187, (elapsed.value % 3600) / 60);
}

void Draw_Print_confirm() {
  process = Confirm;
  popup = Complete;
  DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 252, 263, 351);
  DWIN_ICON_Show(ICON, ICON_Confirm_E, 87, 283);
  DWIN_Draw_Rectangle(0, Select_Color, 86, 282, 187, 321);
  DWIN_Draw_Rectangle(0, Select_Color, 85, 281, 188, 322);
}

void Draw_SD_Item(uint8_t item, uint8_t row) {
  if (item == 0) {
    if (card.flag.workDirIsRoot)
      Draw_Menu_Item(0, ICON_Back, (char*)"Back");
    else
      Draw_Menu_Item(0, ICON_Back, (char*)"..");
=======
void Clear_Title_Bar() {
  DWIN_Draw_Rectangle(1, Color_Bg_Blue, 0, 0, DWIN_WIDTH, 30);
}

void Draw_Title(const char * const title) {
  DWIN_Draw_String(false, false, DWIN_FONT_HEAD, Color_White, Color_Bg_Blue, 14, 4, (char*)title);
}

void Draw_Title(const __FlashStringHelper * title) {
  DWIN_Draw_String(false, false, DWIN_FONT_HEAD, Color_White, Color_Bg_Blue, 14, 4, (char*)title);
}

void Clear_Menu_Area() {
  DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 31, DWIN_WIDTH, STATUS_Y);
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
  // DWIN_ICON_Show(ICON,ICON_Rectangle, 0, MBASE(line) - 18);
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
>>>>>>> origin/bugfix-2.0.x
  }
  else {
    card.getfilename_sorted(SD_ORDER(item-1, card.get_num_Files()));
    char * const filename = card.longest_filename();
    size_t max = MENU_CHAR_LIMIT;
    size_t pos = strlen(filename), len = pos;
    if (!card.flag.filenameIsDir)
      while (pos && filename[pos] != '.') pos--;
    len = pos;
    if (len > max) len = max;
    char name[len+1];
    LOOP_L_N(i, len) name[i] = filename[i];
    if (pos > max)
      LOOP_S_L_N(i, len-3, len) name[i] = '.';
    name[len] = '\0';
    Draw_Menu_Item(row, card.flag.filenameIsDir ? ICON_More : ICON_File, name);
  }
}

void Draw_SD_List(bool removed/*=false*/) {
  Clear_Screen();
  Draw_Title((char*)"Select File");
  selection = 0;
  scrollpos = 0;
  process = File;
  if (card.isMounted() && !removed) {
    LOOP_L_N(i, _MIN(card.get_num_Files()+1, TROWS))
      Draw_SD_Item(i, i);
  }
  else {
    Draw_Menu_Item(0, ICON_Back, (char*)"Back");
    DWIN_Draw_Rectangle(1, Color_Bg_Red, 10, MBASE(3) - 10, DWIN_WIDTH - 10, MBASE(4));
    DWIN_Draw_String(false, false, font16x32, Color_Yellow, Color_Bg_Red, ((DWIN_WIDTH) - 8 * 16) / 2, MBASE(3), (char*)"No Media");
  }
  DWIN_Draw_Rectangle(1, Rectangle_Color, 0, MBASE(0) - 18, 14, MBASE(0) + 33);
}

<<<<<<< HEAD
void Draw_Status_Area(const bool with_update) {

  DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, STATUS_Y, DWIN_WIDTH, DWIN_HEIGHT - 1);

  #if HAS_HOTEND
    DWIN_ICON_Show(ICON, ICON_HotendTemp, 10, 383);
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 384, thermalManager.temp_hotend[0].celsius);
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.temp_hotend[0].target);

    DWIN_ICON_Show(ICON, ICON_StepE, 112, 417);
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWIN_ICON_Show(ICON, ICON_BedTemp, 10, 416);
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 417, thermalManager.temp_bed.celsius);
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.temp_bed.target);
  #endif
=======
inline uint16_t nr_sd_menu_items() {
  return card.get_num_Files() + !card.flag.workDirIsRoot;
}

void Draw_Menu_Icon(const uint8_t line, const uint8_t icon) {
  DWIN_ICON_Show(ICON, icon, 26, MBASE(line) - 3);
}

void Erase_Menu_Text(const uint8_t line) {
  DWIN_Draw_Rectangle(1, Color_Bg_Black, LBLX, MBASE(line) - 14, 271, MBASE(line) + 28);
}

void Draw_Menu_Line(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr) {
  if (label) DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, LBLX, MBASE(line) - 1, (char*)label);
  if (icon) Draw_Menu_Icon(line, icon);
  DWIN_Draw_Line(Line_Color, 16, MBASE(line) + 33, 256, MBASE(line) + 34);
}

// The "Back" label is always on the first line
void Draw_Back_Label() {
  if (HMI_IsChinese())
    DWIN_Frame_AreaCopy(1, 129, 72, 156, 84, LBLX, MBASE(0));
  else
    DWIN_Frame_AreaCopy(1, 226, 179, 256, 189, LBLX, MBASE(0));
}

// Draw "Back" line at the top
void Draw_Back_First(const bool is_sel=true) {
  Draw_Menu_Line(0, ICON_Back);
  Draw_Back_Label();
  if (is_sel) Draw_Menu_Cursor(0);
}
>>>>>>> origin/bugfix-2.0.x

  #if HAS_FAN
    DWIN_ICON_Show(ICON, ICON_FanSpeed, 187, 383);
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
  #endif

<<<<<<< HEAD
  #if HAS_ZOFFSET_ITEM
    DWIN_ICON_Show(ICON, ICON_Zoffset, 187, 416);
    if (zoffsetvalue < 0) {
      DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, -zoffsetvalue * 100);
      DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, F("-"));
    }
    else {
      DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, zoffsetvalue * 100);
      DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, F(" "));
    }
  #endif

  DWIN_ICON_Show(ICON, ICON_Speed, 113, 383);
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 384, F("%"));

  DWIN_Draw_Rectangle(1, Line_Color, 0, 449, DWIN_WIDTH, 451);

  DWIN_ICON_Show(ICON, ICON_MaxSpeedX,   10, 456);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 35, 459, current_position.x * 10);
  DWIN_ICON_Show(ICON, ICON_MaxSpeedY,   95, 456);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 120, 459, current_position.y * 10);
  DWIN_ICON_Show(ICON, ICON_MaxSpeedZ,   180, 456);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 205, 459, current_position.z * 10);

  if (with_update) {
    DWIN_UpdateLCD();
  }
=======
//
// Draw Menus
//

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
#define CONTROL_CASE_INFO  (CONTROL_CASE_RESET + 1)
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

//
// Draw Menus
//

void DWIN_Draw_Label(const uint16_t y, char *string) {
  DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, LBLX, y, string);
}
void DWIN_Draw_Label(const uint16_t y, const __FlashStringHelper *title) {
  DWIN_Draw_Label(y, (char*)title);
}

void draw_move_en(const uint16_t line) {
  #ifdef USE_STRING_TITLES
    DWIN_Draw_Label(line, F("Move"));
  #else
    DWIN_Frame_AreaCopy(1, 69, 61, 102, 71, LBLX, line); // "Move"
  #endif
}

void DWIN_Frame_TitleCopy(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) { DWIN_Frame_AreaCopy(id, x1, y1, x2, y2, 14, 8); }

void Item_Prepare_Move(const uint8_t row) {
  if (HMI_IsChinese())
    DWIN_Frame_AreaCopy(1, 159, 70, 200, 84, LBLX, MBASE(row));
  else
    draw_move_en(MBASE(row)); // "Move"
  Draw_Menu_Line(row, ICON_Axis);
  Draw_More_Icon(row);
}

void Item_Prepare_Disable(const uint8_t row) {
  if (HMI_IsChinese())
    DWIN_Frame_AreaCopy(1, 204, 70, 259, 82, LBLX, MBASE(row));
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_DISABLE_STEPPERS));
    #else
      DWIN_Frame_AreaCopy(1, 103, 59, 200, 74, LBLX, MBASE(row)); // "Disable Stepper"
    #endif
  }
  Draw_Menu_Line(row, ICON_CloseMotor);
}

void Item_Prepare_Home(const uint8_t row) {
  if (HMI_IsChinese())
    DWIN_Frame_AreaCopy(1, 0, 89, 41, 101, LBLX, MBASE(row));
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_AUTO_HOME));
    #else
      DWIN_Frame_AreaCopy(1, 202, 61, 271, 71, LBLX, MBASE(row)); // "Auto Home"
    #endif
  }
  Draw_Menu_Line(row, ICON_Homing);
>>>>>>> origin/bugfix-2.0.x
}

void Update_Status_Area() {
  
  static int16_t feedrate = 100;
  static float x = current_position.x;
  static float y = current_position.y;
  static float z = current_position.z;

<<<<<<< HEAD
  #if HAS_HOTEND
    static float hotend = 0;
    static int16_t hotendtarget = 0;
    static int16_t flow = planner.flow_percentage[0];
    if (thermalManager.temp_hotend[0].celsius != hotend) {
      hotend = thermalManager.temp_hotend[0].celsius;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 384, thermalManager.temp_hotend[0].celsius);
    }
    if (thermalManager.temp_hotend[0].target != hotendtarget) {
      hotendtarget = thermalManager.temp_hotend[0].target;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.temp_hotend[0].target);
=======
  void Item_Prepare_Offset(const uint8_t row) {
    if (HMI_IsChinese()) {
      #if HAS_BED_PROBE
        DWIN_Frame_AreaCopy(1, 174, 164, 223, 177, LBLX, MBASE(row));
        DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 2, 2, 202, MBASE(row), probe.offset.z * 100);
      #else
        DWIN_Frame_AreaCopy(1, 43, 89, 98, 101, LBLX, MBASE(row));
      #endif
    }
    else {
      #if HAS_BED_PROBE
        #ifdef USE_STRING_TITLES
          DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_ZPROBE_ZOFFSET));
        #else
          DWIN_Frame_AreaCopy(1, 93, 179, 141, 189, LBLX, MBASE(row));    // "Z-Offset"
        #endif
        DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 2, 2, 202, MBASE(row), probe.offset.z * 100);
      #else
        #ifdef USE_STRING_TITLES
          DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_SET_HOME_OFFSETS));
        #else
          DWIN_Frame_AreaCopy(1, 1, 76, 106, 86, LBLX, MBASE(row));       // "Set home offsets"
        #endif
      #endif
>>>>>>> origin/bugfix-2.0.x
    }
    if (planner.flow_percentage[0] != flow) {
      flow = planner.flow_percentage[0];
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    }
  #endif

<<<<<<< HEAD
  #if HAS_HEATED_BED
    static float bed = 0;
    static int16_t bedtarget = 0;
    if (thermalManager.temp_bed.celsius != bed) {
      bed = thermalManager.temp_bed.celsius;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 417, thermalManager.temp_bed.celsius);
    }
    if (thermalManager.temp_bed.target != bedtarget) {
      bedtarget = thermalManager.temp_bed.target;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.temp_bed.target);
=======
#if HAS_HOTEND
  void Item_Prepare_PLA(const uint8_t row) {
    if (HMI_IsChinese()) {
      DWIN_Frame_AreaCopy(1, 100, 89, 151, 101, LBLX, MBASE(row));
    }
    else {
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(MBASE(row), F("Preheat " PREHEAT_1_LABEL));
      #else
        DWIN_Frame_AreaCopy(1, 107, 76, 156, 86, LBLX, MBASE(row));       // "Preheat"
        DWIN_Frame_AreaCopy(1, 157, 76, 181, 86, LBLX + 52, MBASE(row));  // "PLA"
      #endif
>>>>>>> origin/bugfix-2.0.x
    }
  #endif

<<<<<<< HEAD
  #if HAS_FAN
    static uint8_t fan = 0;
    if (thermalManager.fan_speed[0] != fan) {
      fan = thermalManager.fan_speed[0];
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
    }
  #endif

  #if HAS_ZOFFSET_ITEM
    static float offset = zoffsetvalue;
    if (zoffsetvalue != offset) {
      offset = zoffsetvalue;
      if (zoffsetvalue < 0) {
        DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, -zoffsetvalue * 100);
        DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, (char*)"-");
      }
      else {
        DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, zoffsetvalue* 100);
        DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, (char*)" ");
      }
=======
  void Item_Prepare_ABS(const uint8_t row) {
    if (HMI_IsChinese()) {
      DWIN_Frame_AreaCopy(1, 180, 89, 233, 100, LBLX, MBASE(row));
    }
    else {
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(MBASE(row), F("Preheat " PREHEAT_2_LABEL));
      #else
        DWIN_Frame_AreaCopy(1, 107, 76, 156, 86, LBLX, MBASE(row));       // "Preheat"
        DWIN_Frame_AreaCopy(1, 172, 76, 198, 86, LBLX + 52, MBASE(row));  // "ABS"
      #endif
>>>>>>> origin/bugfix-2.0.x
    }
  #endif

<<<<<<< HEAD
  if (feedrate_percentage != feedrate) {
    feedrate = feedrate_percentage;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
=======
#if HAS_PREHEAT
  void Item_Prepare_Cool(const uint8_t row) {
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1,   1, 104,  56, 117, LBLX, MBASE(row));
    else {
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_COOLDOWN));
      #else
        DWIN_Frame_AreaCopy(1, 200,  76, 264,  86, LBLX, MBASE(row));      // "Cooldown"
      #endif
    }
    Draw_Menu_Line(row, ICON_Cool);
>>>>>>> origin/bugfix-2.0.x
  }

<<<<<<< HEAD
  if (current_position.x != x) {
    x = current_position.x;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 35, 459, current_position.x * 10);
  }
  if (current_position.y != y) {
    y = current_position.y;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 120, 459, current_position.y * 10);
  }
  if (current_position.z != z) {
    z = current_position.z;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 205, 459, current_position.z * 10);
  }
  DWIN_UpdateLCD();
}

void Draw_Popup(char *line1, char *line2, char *line3, uint8_t mode, uint8_t icon/*=0*/) {
  selection = 0;
  process = mode;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
  uint8_t ypos;
  if (mode == Popup || mode == Confirm)
    ypos = 150;
  else
    ypos = 230;
  if (icon > 0) 
    DWIN_ICON_Show(ICON, icon, 101, 105);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * strlen(line1)) / 2, ypos, line1);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * strlen(line2)) / 2, ypos+30, line2);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * strlen(line3)) / 2, ypos+60, line3);
  if (mode == Popup) {
    DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
    DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
    Popup_Select();
=======
void Item_Prepare_Lang(const uint8_t row) {
  if (HMI_IsChinese())
    DWIN_Frame_AreaCopy(1, 239, 134, 266, 146, LBLX, MBASE(row));
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(row), F("UI Language"));
    #else
      DWIN_Frame_AreaCopy(1, 0, 194, 121, 207, LBLX, MBASE(row)); // "Language selection"
    #endif
  }
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, 226, MBASE(row), HMI_IsChinese() ? F("CN") : F("EN"));
  Draw_Menu_Icon(row, ICON_Language);
}

void Draw_Prepare_Menu() {
  Clear_Main_Window();

  const int16_t scroll = MROWS - index_prepare; // Scrolled-up lines
  #define PSCROL(L) (scroll + (L))
  #define PVISI(L)  WITHIN(PSCROL(L), 0, MROWS)

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 133, 1, 160, 13);   // "Prepare"
>>>>>>> origin/bugfix-2.0.x
  }
  else if (mode == Confirm) {
    DWIN_ICON_Show(ICON, ICON_Continue_E, 87, 283);
  }
}

<<<<<<< HEAD
/* Menu Item Config */
=======
void Item_Control_Info(const uint16_t line) {
  if (HMI_IsChinese())
    DWIN_Frame_AreaCopy(1, 231, 104, 258, 116, LBLX, line);
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(line, F("Info"));
    #else
      DWIN_Frame_AreaCopy(1, 0, 104, 24, 114, LBLX, line);
    #endif
  }
}

void Draw_Control_Menu() {
  Clear_Main_Window();
>>>>>>> origin/bugfix-2.0.x

void Menu_Item_Handler(uint8_t menu, uint8_t item, bool draw/*=true*/) {
  uint8_t row = item - scrollpos;
  switch (menu) {
    case Prepare:

      #define PREPARE_BACK 0
      #define PREPARE_MOVE (PREPARE_BACK + 1)
      #define PREPARE_DISABLE (PREPARE_MOVE + 1)
      #define PREPARE_HOME (PREPARE_DISABLE + 1)
      #define PREPARE_MANUALLEVEL (PREPARE_HOME + 1)
      #define PREPARE_ZOFFSET (PREPARE_MANUALLEVEL + ENABLED(HAS_ZOFFSET_ITEM))
      #define PREPARE_PREHEAT (PREPARE_ZOFFSET + ENABLED(HAS_PREHEAT))
      #define PREPARE_COOLDOWN (PREPARE_PREHEAT + ENABLED(HAS_PREHEAT))
      #define PREPARE_CHANGEFIL (PREPARE_COOLDOWN + ENABLED(ADVANCED_PAUSE_FEATURE))
      #define PREPARE_TOTAL PREPARE_CHANGEFIL

<<<<<<< HEAD
      switch (item) {
        case PREPARE_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Main_Menu(1);
          }
          break;
        case PREPARE_MOVE:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Move", true);
          }
          else {
            Draw_Menu(Move);
          }
          break;
        case PREPARE_DISABLE:
          if (draw) {
            Draw_Menu_Item(row, ICON_CloseMotor, (char*)"Disable Stepper");
          }
          else {
            queue.inject_P(PSTR("M84"));
          }
          break;
        case PREPARE_HOME:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetHome, (char*)"Auto Home");
          }
          else {
            Popup_Handler(Home);
            gcode.process_subcommands_now_P( PSTR("G28"));
            planner.synchronize();
            Draw_Menu(Prepare);
          }
          break;
        case PREPARE_MANUALLEVEL:
          if (draw) {
            Draw_Menu_Item(row, ICON_PrintSize, (char*)"Manual Leveling", true);
          }
          else {
            Popup_Handler(Home);
            gcode.process_subcommands_now_P( PSTR("G28"));
            planner.synchronize();
            Draw_Menu(ManualLevel);
          }
          break;
        #if HAS_ZOFFSET_ITEM
        case PREPARE_ZOFFSET:
          if (draw) {
            Draw_Menu_Item(row, ICON_Zoffset, (char*)"Z-Offset", true);
          }
          else {
            Draw_Menu(ZOffset);
          }
          break;
        #endif
        #if HAS_PREHEAT
        case PREPARE_PREHEAT:
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)"Preheat", true);
          }
          else {
            Draw_Menu(Preheat);
          }
          break;
        case PREPARE_COOLDOWN:
          if (draw) {
            Draw_Menu_Item(row, ICON_Cool, (char*)"Cooldown", true);
          } 
          else {
            thermalManager.zero_fan_speeds();
            thermalManager.disable_all_heaters();
          }
          break;
        #endif
        #if ENABLED(ADVANCED_PAUSE_FEATURE)
        case PREPARE_CHANGEFIL:
          if (draw) {
            #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
              Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament", true);
            #else
              Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament");
            #endif
          }
          else {
            #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
              Draw_Menu(ChangeFilament);
            #else
              Popup_Handler(FilChange);
              gcode.process_subcommands_now_P(PSTR("M600 B1"));
              planner.synchronize();
              Draw_Menu(Prepare, 8);
            #endif
          }
          break;
        #endif
      }
      break;
    case Move:

      #define MOVE_BACK 0
      #define MOVE_X (MOVE_BACK + 1)
      #define MOVE_Y (MOVE_X + 1)
      #define MOVE_Z (MOVE_Y + 1)
      #define MOVE_E (MOVE_Z + ENABLED(HAS_HOTEND))
      #define MOVE_TOTAL MOVE_E

      switch (item) {
        case MOVE_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Prepare, 1);
          }
          break;
        case MOVE_X:
          if (draw) {
            Draw_Menu_Item(row, ICON_MoveX, (char*)"Move X");
            Draw_Float(current_position.x, row, false);
          }
          else {
            Modify_Value(current_position.x, X_MIN_POS, X_MAX_POS, 10);
          }
          break;
        case MOVE_Y:
          if (draw) {
            Draw_Menu_Item(row, ICON_MoveY, (char*)"Move Y");
            Draw_Float(current_position.y, row);
          }
          else {
            Modify_Value(current_position.y, Y_MIN_POS, Y_MAX_POS, 10);
          }
          break;
        case MOVE_Z:
          if (draw) {
            Draw_Menu_Item(row, ICON_MoveZ, (char*)"Move Z");
            Draw_Float(current_position.z, row);
          }
          else {
            Modify_Value(current_position.z, Z_MIN_POS, Z_MAX_POS, 10);
          }
          break;
        #if HAS_HOTEND
        case MOVE_E:
          if (draw) {
            Draw_Menu_Item(row, ICON_Extruder, (char*)"Extruder");
            Draw_Float(current_position.e, item);
          }
          else {
            if (thermalManager.temp_hotend[0].celsius < EXTRUDE_MINTEMP) {
              Popup_Handler(ETemp);
            }
            else {
              Modify_Value(current_position.e, -500, 500, 10);
            }
          }
          break;
        #endif
=======
    #if ENABLED(EEPROM_SETTINGS)
      DWIN_Frame_AreaCopy(1, 117, 104, 172, 116, LBLX, CLINE(CONTROL_CASE_SAVE));   // Store Configuration
      DWIN_Frame_AreaCopy(1, 174, 103, 229, 116, LBLX, CLINE(CONTROL_CASE_LOAD));   // Read Configuration
      DWIN_Frame_AreaCopy(1,   1, 118,  56, 131, LBLX, CLINE(CONTROL_CASE_RESET));  // Reset Configuration
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_CONTROL));
    #else
      DWIN_Frame_TitleCopy(1, 128, 2, 176, 12);                                         // "Control"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(CLINE(CONTROL_CASE_TEMP), GET_TEXT_F(MSG_TEMPERATURE));
      DWIN_Draw_Label(CLINE(CONTROL_CASE_MOVE), GET_TEXT_F(MSG_MOTION));
      #if ENABLED(EEPROM_SETTINGS)
        DWIN_Draw_Label(CLINE(CONTROL_CASE_SAVE), GET_TEXT_F(MSG_STORE_EEPROM));
        DWIN_Draw_Label(CLINE(CONTROL_CASE_LOAD), GET_TEXT_F(MSG_LOAD_EEPROM));
        DWIN_Draw_Label(CLINE(CONTROL_CASE_RESET), GET_TEXT_F(MSG_RESTORE_DEFAULTS));
      #endif
    #else
      DWIN_Frame_AreaCopy(1,  1, 89,  83, 101, LBLX, CLINE(CONTROL_CASE_TEMP));           // Temperature >
      DWIN_Frame_AreaCopy(1, 84, 89, 128,  99, LBLX, CLINE(CONTROL_CASE_MOVE));           // Motion >
      #if ENABLED(EEPROM_SETTINGS)
        DWIN_Frame_AreaCopy(1, 148,  89, 268, 101, LBLX     , CLINE(CONTROL_CASE_SAVE));  // "Store Configuration"
        DWIN_Frame_AreaCopy(1,  26, 104,  57, 114, LBLX     , CLINE(CONTROL_CASE_LOAD));  // "Read"
        DWIN_Frame_AreaCopy(1, 182,  89, 268, 101, LBLX + 34, CLINE(CONTROL_CASE_LOAD));  // "Configuration"
        DWIN_Frame_AreaCopy(1,  59, 104,  93, 114, LBLX     , CLINE(CONTROL_CASE_RESET)); // "Reset"
        DWIN_Frame_AreaCopy(1, 182,  89, 268, 101, LBLX + 37, CLINE(CONTROL_CASE_RESET)); // "Configuration"
      #endif
    #endif
  }

  if (CVISI(CONTROL_CASE_INFO)) Item_Control_Info(CLINE(CONTROL_CASE_INFO));

  if (select_control.now && CVISI(select_control.now))
    Draw_Menu_Cursor(CSCROL(select_control.now));

  // Draw icons and lines
  uint8_t i = 0;
  #define _TEMP_ICON(N) do{ ++i; if (CVISI(i)) Draw_Menu_Line(CSCROL(i), ICON_Temperature + (N) - 1); }while(0)

  _TEMP_ICON(CONTROL_CASE_TEMP);
  if (CVISI(i)) Draw_More_Icon(CSCROL(i));

  _TEMP_ICON(CONTROL_CASE_MOVE);
  Draw_More_Icon(CSCROL(i));

  #if ENABLED(EEPROM_SETTINGS)
    _TEMP_ICON(CONTROL_CASE_SAVE);
    _TEMP_ICON(CONTROL_CASE_LOAD);
    _TEMP_ICON(CONTROL_CASE_RESET);
  #endif

  _TEMP_ICON(CONTROL_CASE_INFO);
  if (CVISI(CONTROL_CASE_INFO)) Draw_More_Icon(CSCROL(i));
}

void Draw_Tune_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, 73, 2, 100, 13, 14, 9);
    DWIN_Frame_AreaCopy(1, 116, 164, 171, 176, LBLX, MBASE(TUNE_CASE_SPEED));
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 1, 134, 56, 146, LBLX, MBASE(TUNE_CASE_TEMP));
    #endif
    #if HAS_HEATED_BED
      DWIN_Frame_AreaCopy(1, 58, 134, 113, 146, LBLX, MBASE(TUNE_CASE_BED));
    #endif
    #if HAS_FAN
      DWIN_Frame_AreaCopy(1, 115, 134, 170, 146, LBLX, MBASE(TUNE_CASE_FAN));
    #endif
    #if HAS_ZOFFSET_ITEM
      DWIN_Frame_AreaCopy(1, 174, 164, 223, 177, LBLX, MBASE(TUNE_CASE_ZOFF));
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_TUNE));
    #else
      DWIN_Frame_AreaCopy(1, 94, 2, 126, 12, 14, 9);
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(TUNE_CASE_SPEED), GET_TEXT_F(MSG_SPEED));
      #if HAS_HOTEND
        DWIN_Draw_Label(MBASE(TUNE_CASE_TEMP), GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND));
      #endif
      #if HAS_HEATED_BED
        DWIN_Draw_Label(MBASE(TUNE_CASE_BED), GET_TEXT_F(MSG_UBL_SET_TEMP_BED));
      #endif
      #if HAS_FAN
        DWIN_Draw_Label(MBASE(TUNE_CASE_FAN), GET_TEXT_F(MSG_FAN_SPEED));
      #endif
      DWIN_Draw_Label(MBASE(TUNE_CASE_ZOFF), GET_TEXT_F(MSG_ZPROBE_ZOFFSET));
    #else
      DWIN_Frame_AreaCopy(1, 1, 179, 92, 190, LBLX, MBASE(TUNE_CASE_SPEED));      // Print speed
      #if HAS_HOTEND
        DWIN_Frame_AreaCopy(1, 197, 104, 238, 114, LBLX, MBASE(TUNE_CASE_TEMP));  // Hotend...
        DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 44, MBASE(TUNE_CASE_TEMP)); // ...Temperature
      #endif
      #if HAS_HEATED_BED
        DWIN_Frame_AreaCopy(1, 240, 104, 264, 114, LBLX, MBASE(TUNE_CASE_BED));   // Bed...
        DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 27, MBASE(TUNE_CASE_BED));  // ...Temperature
      #endif
      #if HAS_FAN
        DWIN_Frame_AreaCopy(1, 0, 119, 64, 132, LBLX, MBASE(TUNE_CASE_FAN));      // Fan speed
      #endif
      #if HAS_ZOFFSET_ITEM
        DWIN_Frame_AreaCopy(1, 93, 179, 141, 189, LBLX, MBASE(TUNE_CASE_ZOFF));   // Z-offset
      #endif
    #endif
  }

  Draw_Back_First(select_tune.now == 0);
  if (select_tune.now) Draw_Menu_Cursor(select_tune.now);

  Draw_Menu_Line(TUNE_CASE_SPEED, ICON_Speed);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TUNE_CASE_SPEED), feedrate_percentage);

  #if HAS_HOTEND
    Draw_Menu_Line(TUNE_CASE_TEMP, ICON_HotendTemp);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TUNE_CASE_TEMP), thermalManager.temp_hotend[0].target);
  #endif
  #if HAS_HEATED_BED
    Draw_Menu_Line(TUNE_CASE_BED, ICON_BedTemp);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TUNE_CASE_BED), thermalManager.temp_bed.target);
  #endif
  #if HAS_FAN
    Draw_Menu_Line(TUNE_CASE_FAN, ICON_FanSpeed);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TUNE_CASE_FAN), thermalManager.fan_speed[0]);
  #endif
  #if HAS_ZOFFSET_ITEM
    Draw_Menu_Line(TUNE_CASE_ZOFF, ICON_Zoffset);
    DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 2, 2, 202, MBASE(TUNE_CASE_ZOFF), BABY_Z_VAR * 100);
  #endif
}

void draw_max_en(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 245, 119, 269, 129, LBLX, line);   // "Max"
}
void draw_max_accel_en(const uint16_t line) {
  draw_max_en(line);
  DWIN_Frame_AreaCopy(1, 1, 135, 79, 145, LBLX + 27, line); // "Acceleration"
}
void draw_speed_en(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 184, 119, 224, 132, LBLX + inset, line); // "Speed"
}
void draw_jerk_en(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 64, 119, 106, 129, LBLX + 27, line); // "Jerk"
}
void draw_steps_per_mm(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 1, 151, 101, 161, LBLX, line);   // "Steps-per-mm"
}
void say_x(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 95, 104, 102, 114, LBLX + inset, line); // "X"
}
void say_y(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 104, 104, 110, 114, LBLX + inset, line); // "Y"
}
void say_z(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 112, 104, 120, 114, LBLX + inset, line); // "Z"
}
void say_e(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 237, 119, 244, 129, LBLX + inset, line); // "E"
}

void Draw_Motion_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 1, 16, 28, 28);                                     // "Motion"
    DWIN_Frame_AreaCopy(1, 173, 133, 228, 147, LBLX, MBASE(MOTION_CASE_RATE));  // Max speed
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX, MBASE(MOTION_CASE_ACCEL));        // Max...
    DWIN_Frame_AreaCopy(1, 28, 149, 69, 161, LBLX + 27, MBASE(MOTION_CASE_ACCEL) + 1); // ...Acceleration
    #if HAS_CLASSIC_JERK
      DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX, MBASE(MOTION_CASE_JERK));        // Max...
      DWIN_Frame_AreaCopy(1, 1, 180, 28, 192, LBLX + 27, MBASE(MOTION_CASE_JERK) + 1);  // ...
      DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 54, MBASE(MOTION_CASE_JERK));   // ...Jerk
    #endif
    DWIN_Frame_AreaCopy(1, 153, 148, 194, 161, LBLX, MBASE(MOTION_CASE_STEPS));         // Flow ratio
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MOTION));
    #else
      DWIN_Frame_TitleCopy(1, 144, 16, 189, 26);                                        // "Motion"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(MOTION_CASE_RATE), F("Feedrate"));
      DWIN_Draw_Label(MBASE(MOTION_CASE_ACCEL), GET_TEXT_F(MSG_ACCELERATION));
      #if HAS_CLASSIC_JERK
        DWIN_Draw_Label(MBASE(MOTION_CASE_JERK), GET_TEXT_F(MSG_JERK));
      #endif
      DWIN_Draw_Label(MBASE(MOTION_CASE_STEPS), GET_TEXT_F(MSG_STEPS_PER_MM));
    #else
      draw_max_en(MBASE(MOTION_CASE_RATE)); draw_speed_en(27, MBASE(MOTION_CASE_RATE)); // "Max Speed"
      draw_max_accel_en(MBASE(MOTION_CASE_ACCEL));                                      // "Max Acceleration"
      #if HAS_CLASSIC_JERK
        draw_max_en(MBASE(MOTION_CASE_JERK)); draw_jerk_en(MBASE(MOTION_CASE_JERK));    // "Max Jerk"
      #endif
      draw_steps_per_mm(MBASE(MOTION_CASE_STEPS));                                      // "Steps-per-mm"
    #endif
  }

  Draw_Back_First(select_motion.now == 0);
  if (select_motion.now) Draw_Menu_Cursor(select_motion.now);

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
        DWIN_Frame_AreaCopy(1, 103, 371, 237, 386, 52, 285);
        DWIN_Frame_AreaCopy(1, 151, 389, 185, 402, 187, 285);
        DWIN_Frame_AreaCopy(1, 189, 389, 271, 402, 95, 310);
      }
      else {
        DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 36, 300, F("Nozzle or Bed temperature"));
        DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 92, 300, F("is too high"));
      }
    }
    else {
      DWIN_ICON_Show(ICON, ICON_TempTooLow, 102, 165);
      if (HMI_IsChinese()) {
        DWIN_Frame_AreaCopy(1, 103, 371, 270, 386, 52, 285);
        DWIN_Frame_AreaCopy(1, 189, 389, 271, 402, 95, 310);
      }
      else {
        DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 36, 300, F("Nozzle or Bed temperature"));
        DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 92, 300, F("is too low"));
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
      DWIN_Frame_AreaCopy(1, 103, 371, 136, 386, 69, 240);
      DWIN_Frame_AreaCopy(1, 170, 371, 270, 386, 102, 240);
      DWIN_ICON_Show(ICON, ICON_Confirm_C, 86, 280);
    }
    else {
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 20, 235, F("Nozzle is too cold"));
      DWIN_ICON_Show(ICON, ICON_Confirm_E, 86, 280);
    }
  }

#endif

void Popup_Window_Resume() {
  Clear_Popup_Area();
  Draw_Popup_Bkgd_105();
  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, 160, 338, 235, 354, 98, 135);
    DWIN_Frame_AreaCopy(1, 103, 321, 271, 335, 52, 192);
    DWIN_ICON_Show(ICON, ICON_Cancel_C,    26, 307);
    DWIN_ICON_Show(ICON, ICON_Continue_C, 146, 307);
  }
  else {
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 14) / 2, 115, F("Continue Print"));
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 22) / 2, 192, F("It looks like the last"));
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 22) / 2, 212, F("file was interrupted."));
    DWIN_ICON_Show(ICON, ICON_Cancel_E,    26, 307);
    DWIN_ICON_Show(ICON, ICON_Continue_E, 146, 307);
  }
}

void Popup_Window_Home(const bool parking/*=false*/) {
  Clear_Main_Window();
  Draw_Popup_Bkgd_60();
  DWIN_ICON_Show(ICON, ICON_BLTouch, 101, 105);
  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, 0, 371, 33, 386, 85, 240);
    DWIN_Frame_AreaCopy(1, 203, 286, 271, 302, 118, 240);
    DWIN_Frame_AreaCopy(1, 0, 389, 150, 402, 61, 280);
  }
  else {
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * (parking ? 7 : 10)) / 2, 230, parking ? F("Parking") : F("Homing XYZ"));
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 23) / 2, 260, F("Please wait until done."));
  }
}

#if HAS_ONESTEP_LEVELING

  void Popup_Window_Leveling() {
    Clear_Main_Window();
    Draw_Popup_Bkgd_60();
    DWIN_ICON_Show(ICON, ICON_AutoLeveling, 101, 105);
    if (HMI_IsChinese()) {
      DWIN_Frame_AreaCopy(1, 0, 371, 100, 386, 84, 240);
      DWIN_Frame_AreaCopy(1, 0, 389, 150, 402, 61, 280);
    }
    else {
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 13) / 2, 230, GET_TEXT_F(MSG_BED_LEVELING));
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 23) / 2, 260, F("Please wait until done."));
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
         if (select_print.now == 1) DWIN_Frame_AreaCopy(1, 237, 338, 269, 356, 98, 150);
    else if (select_print.now == 2) DWIN_Frame_AreaCopy(1, 221, 320, 253, 336, 98, 150);
    DWIN_Frame_AreaCopy(1, 220, 304, 264, 319, 130, 150);
    DWIN_ICON_Show(ICON, ICON_Confirm_C, 26, 280);
    DWIN_ICON_Show(ICON, ICON_Cancel_C, 146, 280);
  }
  else {
         if (select_print.now == 1) DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 11) / 2, 150, GET_TEXT_F(MSG_PAUSE_PRINT));
    else if (select_print.now == 2) DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 10) / 2, 150, GET_TEXT_F(MSG_STOP_PRINT));
    DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
    DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
  }
  Draw_Select_Highlight(true);
}

void Draw_Printing_Screen() {
  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, 30,  1,  71, 14,  14,   9);  // Tune
    DWIN_Frame_AreaCopy(1,  0, 72,  63, 86,  41, 188);  // Pause
    DWIN_Frame_AreaCopy(1, 65, 72, 128, 86, 176, 188);  // Stop
  }
  else {
    DWIN_Frame_AreaCopy(1, 40,  2,  92, 14,  14,   9);  // Tune
    DWIN_Frame_AreaCopy(1,  0, 44,  96, 58,  41, 188);  // Pause
    DWIN_Frame_AreaCopy(1, 98, 44, 152, 58, 176, 188);  // Stop
  }
}

void Draw_Print_ProgressBar() {
  DWIN_ICON_Show(ICON, ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, BarFill_Color, 16 + _card_percent * 240 / 100, 93, 256, 113);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Percent_Color, Color_Bg_Black, 2, 117, 133, _card_percent);
  DWIN_Draw_String(false, false, font8x16, Percent_Color, Color_Bg_Black, 133, 133, F("%"));
}

void Draw_Print_ProgressElapsed() {
  duration_t elapsed = print_job_timer.duration(); // print timer
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 42, 212, elapsed.value / 3600);
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, 58, 212, F(":"));
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 66, 212, (elapsed.value % 3600) / 60);
}

void Draw_Print_ProgressRemain() {
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 176, 212, _remain_time / 3600);
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, 192, 212, F(":"));
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 200, 212, (_remain_time % 3600) / 60);
}

void Goto_PrintProcess() {
  checkkey = PrintProcess;

  Clear_Main_Window();
  Draw_Printing_Screen();

  ICON_Tune();
  if (printingIsPaused()) ICON_Continue(); else ICON_Pause();
  ICON_Stop();

  // Copy into filebuf string before entry
  char * const name = card.longest_filename();
  const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(name) * MENU_CHR_W) / 2;
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, npos, 60, name);

  DWIN_ICON_Show(ICON, ICON_PrintTime, 17, 193);
  DWIN_ICON_Show(ICON, ICON_RemainTime, 150, 191);

  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();
}

void Goto_MainMenu() {
  checkkey = MainMenu;

  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, 2, 2, 27, 14, 14, 9); // "Home"
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MAIN));
    #else
      DWIN_Frame_AreaCopy(1, 0, 2, 39, 12, 14, 9);
    #endif
  }

  DWIN_ICON_Show(ICON, ICON_LOGO, 71, 52);

  ICON_Print();
  ICON_Prepare();
  ICON_Control();
  TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(select_page.now == 3);
}

inline ENCODER_DiffState get_encoder_state() {
  static millis_t Encoder_ms = 0;
  const millis_t ms = millis();
  if (PENDING(ms, Encoder_ms)) return ENCODER_DIFF_NO;
  const ENCODER_DiffState state = Encoder_ReceiveAnalyze();
  if (state != ENCODER_DIFF_NO) Encoder_ms = ms + ENCODER_WAIT_MS;
  return state;
}

void HMI_Move_X() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_X_scaled)) {
      checkkey = AxisMove;
      EncoderRate.enabled = false;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 216, MBASE(1), HMI_ValueStruct.Move_X_scaled);
      if (!planner.is_full()) {
        // Wait for planner moves to finish!
        planner.synchronize();
        planner.buffer_line(current_position, homing_feedrate(X_AXIS), active_extruder);
      }
      DWIN_UpdateLCD();
      return;
    }
    LIMIT(HMI_ValueStruct.Move_X_scaled, (X_MIN_POS) * MINUNITMULT, (X_MAX_POS) * MINUNITMULT);
    current_position.x = HMI_ValueStruct.Move_X_scaled / MINUNITMULT;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 216, MBASE(1), HMI_ValueStruct.Move_X_scaled);
    DWIN_UpdateLCD();
  }
}

void HMI_Move_Y() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_Y_scaled)) {
      checkkey = AxisMove;
      EncoderRate.enabled = false;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 216, MBASE(2), HMI_ValueStruct.Move_Y_scaled);
      if (!planner.is_full()) {
        // Wait for planner moves to finish!
        planner.synchronize();
        planner.buffer_line(current_position, homing_feedrate(Y_AXIS), active_extruder);
      }
      DWIN_UpdateLCD();
      return;
    }
    LIMIT(HMI_ValueStruct.Move_Y_scaled, (Y_MIN_POS) * MINUNITMULT, (Y_MAX_POS) * MINUNITMULT);
    current_position.y = HMI_ValueStruct.Move_Y_scaled / MINUNITMULT;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 216, MBASE(2), HMI_ValueStruct.Move_Y_scaled);
    DWIN_UpdateLCD();
  }
}

void HMI_Move_Z() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_Z_scaled)) {
      checkkey = AxisMove;
      EncoderRate.enabled = false;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(3), HMI_ValueStruct.Move_Z_scaled);
      if (!planner.is_full()) {
        // Wait for planner moves to finish!
        planner.synchronize();
        planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
      }
      DWIN_UpdateLCD();
      return;
    }
    LIMIT(HMI_ValueStruct.Move_Z_scaled, Z_MIN_POS * MINUNITMULT, Z_MAX_POS * MINUNITMULT);
    current_position.z = HMI_ValueStruct.Move_Z_scaled / MINUNITMULT;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 216, MBASE(3), HMI_ValueStruct.Move_Z_scaled);
    DWIN_UpdateLCD();
  }
}

#if HAS_HOTEND

  void HMI_Move_E() {
    static float last_E_scaled = 0;
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_E_scaled)) {
        checkkey = AxisMove;
        EncoderRate.enabled = false;
        last_E_scaled = HMI_ValueStruct.Move_E_scaled;
        DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(4), HMI_ValueStruct.Move_E_scaled);
        if (!planner.is_full()) {
          planner.synchronize(); // Wait for planner moves to finish!
          planner.buffer_line(current_position, MMM_TO_MMS(FEEDRATE_E), active_extruder);
        }
        DWIN_UpdateLCD();
        return;
      }
      if ((HMI_ValueStruct.Move_E_scaled - last_E_scaled) > (EXTRUDE_MAXLENGTH) * MINUNITMULT)
        HMI_ValueStruct.Move_E_scaled = last_E_scaled + (EXTRUDE_MAXLENGTH) * MINUNITMULT;
      else if ((last_E_scaled - HMI_ValueStruct.Move_E_scaled) > (EXTRUDE_MAXLENGTH) * MINUNITMULT)
        HMI_ValueStruct.Move_E_scaled = last_E_scaled - (EXTRUDE_MAXLENGTH) * MINUNITMULT;
      current_position.e = HMI_ValueStruct.Move_E_scaled / MINUNITMULT;
      DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, UNITFDIGITS, 216, MBASE(4), HMI_ValueStruct.Move_E_scaled);
      DWIN_UpdateLCD();
    }
  }

#endif

#if HAS_ZOFFSET_ITEM

  bool printer_busy() { return planner.movesplanned() || printingIsActive(); }

  void HMI_Zoffset() {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
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
        DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 2, 2, 202, MBASE(zoff_line), TERN(HAS_BED_PROBE, BABY_Z_VAR * 100, HMI_ValueStruct.offset_value));
        DWIN_UpdateLCD();
        return;
      }
      LIMIT(HMI_ValueStruct.offset_value, (Z_PROBE_OFFSET_RANGE_MIN) * 100, (Z_PROBE_OFFSET_RANGE_MAX) * 100);
      last_zoffset = dwin_zoffset;
      dwin_zoffset = HMI_ValueStruct.offset_value / 100.0f;
      #if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
        if (BABYSTEP_ALLOWED()) babystep.add_mm(Z_AXIS, dwin_zoffset - last_zoffset);
      #endif
      DWIN_Draw_Signed_Float(font8x16, Select_Color, 2, 2, 202, MBASE(zoff_line), HMI_ValueStruct.offset_value);
      DWIN_UpdateLCD();
    }
  }

#endif // HAS_ZOFFSET_ITEM

#if HAS_HOTEND

  void HMI_ETemp() {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
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
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(temp_line), ui.material_preset[0].hotend_temp);
          return;
        }
        else if (HMI_ValueStruct.show_mode == -3) {
          checkkey = ABSPreheat;
          ui.material_preset[1].hotend_temp = HMI_ValueStruct.E_Temp;
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(temp_line), ui.material_preset[1].hotend_temp);
          return;
        }
        else if (HMI_ValueStruct.show_mode == -1) // Temperature
          checkkey = TemperatureID;
        else
          checkkey = Tune;
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(temp_line), HMI_ValueStruct.E_Temp);
        thermalManager.setTargetHotend(HMI_ValueStruct.E_Temp, 0);
        return;
      }
      // E_Temp limit
      LIMIT(HMI_ValueStruct.E_Temp, MIN_E_TEMP, MAX_E_TEMP);
      // E_Temp value
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(temp_line), HMI_ValueStruct.E_Temp);
    }
  }

#endif // HAS_HOTEND

#if HAS_HEATED_BED

  void HMI_BedTemp() {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
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
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(bed_line), ui.material_preset[0].bed_temp);
          return;
        }
        else if (HMI_ValueStruct.show_mode == -3) {
          checkkey = ABSPreheat;
          ui.material_preset[1].bed_temp = HMI_ValueStruct.Bed_Temp;
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(bed_line), ui.material_preset[1].bed_temp);
          return;
        }
        else if (HMI_ValueStruct.show_mode == -1)
          checkkey = TemperatureID;
        else
          checkkey = Tune;
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(bed_line), HMI_ValueStruct.Bed_Temp);
        thermalManager.setTargetBed(HMI_ValueStruct.Bed_Temp);
        return;
      }
      // Bed_Temp limit
      LIMIT(HMI_ValueStruct.Bed_Temp, MIN_BED_TEMP, BED_MAX_TARGET);
      // Bed_Temp value
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(bed_line), HMI_ValueStruct.Bed_Temp);
    }
  }

#endif // HAS_HEATED_BED

#if HAS_PREHEAT && HAS_FAN

  void HMI_FanSpeed() {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
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
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(fan_line), ui.material_preset[0].fan_speed);
          return;
        }
        else if (HMI_ValueStruct.show_mode == -3) {
          checkkey = ABSPreheat;
          ui.material_preset[1].fan_speed = HMI_ValueStruct.Fan_speed;
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(fan_line), ui.material_preset[1].fan_speed);
          return;
        }
        else if (HMI_ValueStruct.show_mode == -1)
          checkkey = TemperatureID;
        else
          checkkey = Tune;
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(fan_line), HMI_ValueStruct.Fan_speed);
        thermalManager.set_fan_speed(0, HMI_ValueStruct.Fan_speed);
        return;
      }
      // Fan_speed limit
      LIMIT(HMI_ValueStruct.Fan_speed, FANOFF, FANON);
      // Fan_speed value
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(fan_line), HMI_ValueStruct.Fan_speed);
    }
  }

#endif // HAS_PREHEAT && HAS_FAN

void HMI_PrintSpeed() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.print_speed)) {
      checkkey = Tune;
      EncoderRate.enabled = false;
      feedrate_percentage = HMI_ValueStruct.print_speed;
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(select_tune.now + MROWS - index_tune), HMI_ValueStruct.print_speed);
      return;
    }
    // print_speed limit
    LIMIT(HMI_ValueStruct.print_speed, MIN_PRINT_SPEED, MAX_PRINT_SPEED);
    // print_speed value
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(select_tune.now + MROWS - index_tune), HMI_ValueStruct.print_speed);
  }
}

void HMI_MaxFeedspeedXYZE() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Feedspeed)) {
      checkkey = MaxSpeed;
      EncoderRate.enabled = false;
      if (WITHIN(HMI_flag.feedspeed_axis, X_AXIS, E_AXIS))
        planner.set_max_feedrate(HMI_flag.feedspeed_axis, HMI_ValueStruct.Max_Feedspeed);
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
      return;
    }
    // MaxFeedspeed limit
    if (WITHIN(HMI_flag.feedspeed_axis, X_AXIS, E_AXIS))
      NOMORE(HMI_ValueStruct.Max_Feedspeed, default_max_feedrate[HMI_flag.feedspeed_axis] * 2);
    if (HMI_ValueStruct.Max_Feedspeed < MIN_MAXFEEDSPEED) HMI_ValueStruct.Max_Feedspeed = MIN_MAXFEEDSPEED;
    // MaxFeedspeed value
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
  }
}

void HMI_MaxAccelerationXYZE() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Acceleration)) {
      checkkey = MaxAcceleration;
      EncoderRate.enabled = false;
      if (HMI_flag.acc_axis == X_AXIS) planner.set_max_acceleration(X_AXIS, HMI_ValueStruct.Max_Acceleration);
      else if (HMI_flag.acc_axis == Y_AXIS) planner.set_max_acceleration(Y_AXIS, HMI_ValueStruct.Max_Acceleration);
      else if (HMI_flag.acc_axis == Z_AXIS) planner.set_max_acceleration(Z_AXIS, HMI_ValueStruct.Max_Acceleration);
      #if HAS_HOTEND
        else if (HMI_flag.acc_axis == E_AXIS) planner.set_max_acceleration(E_AXIS, HMI_ValueStruct.Max_Acceleration);
      #endif
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
      return;
    }
    // MaxAcceleration limit
    if (WITHIN(HMI_flag.acc_axis, X_AXIS, E_AXIS))
      NOMORE(HMI_ValueStruct.Max_Acceleration, default_max_acceleration[HMI_flag.acc_axis] * 2);
    if (HMI_ValueStruct.Max_Acceleration < MIN_MAXACCELERATION) HMI_ValueStruct.Max_Acceleration = MIN_MAXACCELERATION;
    // MaxAcceleration value
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
  }
}

#if HAS_CLASSIC_JERK

  void HMI_MaxJerkXYZE() {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Jerk_scaled)) {
        checkkey = MaxJerk;
        EncoderRate.enabled = false;
        if (WITHIN(HMI_flag.jerk_axis, X_AXIS, E_AXIS))
          planner.set_max_jerk(HMI_flag.jerk_axis, HMI_ValueStruct.Max_Jerk_scaled / 10);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 210, MBASE(select_jerk.now), HMI_ValueStruct.Max_Jerk_scaled);
        return;
      }
      // MaxJerk limit
      if (WITHIN(HMI_flag.jerk_axis, X_AXIS, E_AXIS))
        NOMORE(HMI_ValueStruct.Max_Jerk_scaled, default_max_jerk[HMI_flag.jerk_axis] * 2 * MINUNITMULT);
      NOLESS(HMI_ValueStruct.Max_Jerk_scaled, (MIN_MAXJERK) * MINUNITMULT);
      // MaxJerk value
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 210, MBASE(select_jerk.now), HMI_ValueStruct.Max_Jerk_scaled);
    }
  }

#endif // HAS_CLASSIC_JERK

void HMI_StepXYZE() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Step_scaled)) {
      checkkey = Step;
      EncoderRate.enabled = false;
      if (WITHIN(HMI_flag.step_axis, X_AXIS, E_AXIS))
        planner.settings.axis_steps_per_mm[HMI_flag.step_axis] = HMI_ValueStruct.Max_Step_scaled / 10;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step_scaled);
      return;
    }
    // Step limit
    if (WITHIN(HMI_flag.step_axis, X_AXIS, E_AXIS))
      NOMORE(HMI_ValueStruct.Max_Step_scaled, 999.9 * MINUNITMULT);
    NOLESS(HMI_ValueStruct.Max_Step_scaled, MIN_STEP);
    // Step value
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step_scaled);
  }
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
      DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, x, y, F("???.?"));
    else if (blink && draw_empty)
      DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, x, y, F("     "));
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
    static float _hotendtemp = 0;
    const bool _new_hotend_temp = _hotendtemp != thermalManager.temp_hotend[0].celsius;
    if (_new_hotend_temp) _hotendtemp = thermalManager.temp_hotend[0].celsius;
    static int16_t _hotendtarget = 0;
    const bool _new_hotend_target = _hotendtarget != thermalManager.temp_hotend[0].target;
    if (_new_hotend_target) _hotendtarget = thermalManager.temp_hotend[0].target;
  #endif
  #if HAS_HEATED_BED
    static float _bedtemp = 0;
    const bool _new_bed_temp = _bedtemp != thermalManager.temp_bed.celsius;
    if (_new_bed_temp) _bedtemp = thermalManager.temp_bed.celsius;
    static int16_t _bedtarget = 0;
    const bool _new_bed_target = _bedtarget != thermalManager.temp_bed.target;
    if (_new_bed_target) _bedtarget = thermalManager.temp_bed.target;
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
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TUNE_CASE_TEMP + MROWS - index_tune), _hotendtarget);
    #endif
    #if HAS_HEATED_BED
      if (_new_bed_target)
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TUNE_CASE_BED + MROWS - index_tune), _bedtarget);
    #endif
    #if HAS_FAN
      if (_new_fanspeed)
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TUNE_CASE_FAN + MROWS - index_tune), _fanspeed);
    #endif
  }
  else if (checkkey == TemperatureID) {
    // Temperature page temperature update
    #if HAS_HOTEND
      if (_new_hotend_target)
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TEMP_CASE_TEMP), _hotendtarget);
    #endif
    #if HAS_HEATED_BED
      if (_new_bed_target)
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TEMP_CASE_BED), _bedtarget);
    #endif
    #if HAS_FAN
      if (_new_fanspeed)
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(TEMP_CASE_FAN), _fanspeed);
    #endif
  }

  // Bottom temperature update

  #if HAS_HOTEND
    if (_new_hotend_temp)
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 384, _hotendtemp);
    if (_new_hotend_target)
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 384, _hotendtarget);

    static int16_t _flow = planner.flow_percentage[0];
    if (_flow != planner.flow_percentage[0]) {
      _flow = planner.flow_percentage[0];
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 417, _flow);
    }
  #endif

  #if HAS_HEATED_BED
    if (_new_bed_temp)
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 417, _bedtemp);
    if (_new_bed_target)
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 417, _bedtarget);
  #endif

  static int16_t _feedrate = 100;
  if (_feedrate != feedrate_percentage) {
    _feedrate = feedrate_percentage;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 384, _feedrate);
  }

  #if HAS_FAN
    if (_fanspeed != thermalManager.fan_speed[0]) {
      _fanspeed = thermalManager.fan_speed[0];
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 195 + 2 * STAT_CHR_W, 384, _fanspeed);
    }
  #endif

  static float _offset = 0;
  if (BABY_Z_VAR != _offset) {
    _offset = BABY_Z_VAR;
    if (BABY_Z_VAR < 0) {
      DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, -_offset * 100);
      DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, F("-"));
    }
    else {
      DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, _offset * 100);
      DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, F(" "));
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

#define ICON_Folder ICON_More

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
    shift_ms  = select_file.now > 0 && strlen(shift_name) > MENU_CHAR_LIMIT
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
    Draw_Menu_Line(row, ICON_Folder, "..");
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

  void Draw_SDItem_Shifted(int8_t &shift) {
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
    DWIN_Draw_String(false, false, font16x32, Color_Yellow, Color_Bg_Red, ((DWIN_WIDTH) - 8 * 16) / 2, MBASE(3), F("No Media"));
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
    //SERIAL_ECHOLNPAIR("HMI_SDCardUpdate: ", DWIN_lcd_sd_status);
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
        card.flag.abort_sd_printing = true;
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
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 384, thermalManager.temp_hotend[0].celsius);
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.temp_hotend[0].target);

    DWIN_ICON_Show(ICON, ICON_StepE, 112, 417);
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWIN_ICON_Show(ICON, ICON_BedTemp, 10, 416);
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 417, thermalManager.temp_bed.celsius);
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.temp_bed.target);
  #endif

  DWIN_ICON_Show(ICON, ICON_Speed, 113, 383);
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 384, F("%"));

  #if HAS_FAN
    DWIN_ICON_Show(ICON, ICON_FanSpeed, 187, 383);
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
  #endif

  #if HAS_ZOFFSET_ITEM
    DWIN_ICON_Show(ICON, ICON_Zoffset, 187, 416);
  #endif

  if (BABY_Z_VAR < 0) {
    DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, -BABY_Z_VAR * 100);
    DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, F("-"));
  }
  else {
    DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, BABY_Z_VAR * 100);
    DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, F(" "));
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

  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(MACHINE_SIZE) * MENU_CHR_W) / 2, 122, F(MACHINE_SIZE));
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(SHORT_BUILD_VERSION) * MENU_CHR_W) / 2, 195, F(SHORT_BUILD_VERSION));

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 30, 17, 57, 29); // "Info"

    DWIN_Frame_AreaCopy(1, 197, 149, 252, 161, 108, 102);
    DWIN_Frame_AreaCopy(1, 1, 164, 56, 176, 108, 175);
    DWIN_Frame_AreaCopy(1, 58, 164, 113, 176, 105, 248);
    DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(CORP_WEBSITE_C) * MENU_CHR_W) / 2, 268, F(CORP_WEBSITE_C));
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_INFO_SCREEN));
    #else
      DWIN_Frame_TitleCopy(1, 190, 16, 215, 26); // "Info"
    #endif

    DWIN_Frame_AreaCopy(1, 120, 150, 146, 161, 124, 102);
    DWIN_Frame_AreaCopy(1, 146, 151, 254, 161, 82, 175);
    DWIN_Frame_AreaCopy(1, 0, 165, 94, 175, 89, 248);
    DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(CORP_WEBSITE_E) * MENU_CHR_W) / 2, 268, F(CORP_WEBSITE_E));
  }

  Draw_Back_First();
  LOOP_L_N(i, 3) {
    DWIN_ICON_Show(ICON, ICON_PrintSize + i, 26, 99 + i * 73);
    DWIN_Draw_Line(Line_Color, 16, MBASE(2) + i * 73, 256, 156 + i * 73);
  }
}

void Draw_Print_File_Menu() {
  Clear_Title_Bar();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 0, 31, 55, 44); // "Print file"
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title("Print file"); // TODO: GET_TEXT_F
    #else
      DWIN_Frame_TitleCopy(1, 52, 31, 137, 41); // "Print file"
    #endif
  }

  Redraw_SD_List();
}

/* Main Process */
void HMI_MainMenu() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_page.inc(4)) {
      switch (select_page.now) {
        case 0: ICON_Print(); break;
        case 1: ICON_Print(); ICON_Prepare(); break;
        case 2: ICON_Prepare(); ICON_Control(); break;
        case 3: ICON_Control(); TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(1); break;
>>>>>>> origin/bugfix-2.0.x
      }
      break;
    case ManualLevel:

      #define MLEVEL_BACK 0
      #define MLEVEL_BL (MLEVEL_BACK + 1)
      #define MLEVEL_TL (MLEVEL_BL + 1)
      #define MLEVEL_TR (MLEVEL_TL + 1)
      #define MLEVEL_BR (MLEVEL_TR + 1)
      #define MLEVEL_C (MLEVEL_BR + 1)
      #define MLEVEL_TOTAL MLEVEL_C

      switch (item) {
        case MLEVEL_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Prepare, 4);
          }
          break;
        case MLEVEL_BL:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Bottom Left");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X35 Y35\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 1);
          }
          break;
        case MLEVEL_TL:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Top Left");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X35 Y200\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 2);
          }
          break;
        case MLEVEL_TR:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Top Right");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X200 Y200\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 3);
          }
          break;
        case MLEVEL_BR:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Bottom Right");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X200 Y35\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 4);
          }
          break;
        case MLEVEL_C:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Center");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X117.5 Y117.5\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 5);
          }
          break;
      }
      break;
    #if HAS_ZOFFSET_ITEM
    case ZOffset:

      #define ZOFFSET_BACK 0
      #define ZOFFSET_HOME (ZOFFSET_BACK + 1)
      #define ZOFFSET_MODE (ZOFFSET_HOME + 1)
      #define ZOFFSET_OFFSET (ZOFFSET_MODE + 1)
      #define ZOFFSET_UP (ZOFFSET_OFFSET + 1)
      #define ZOFFSET_DOWN (ZOFFSET_UP + 1)
      #define ZOFFSET_SAVE (ZOFFSET_DOWN + ENABLED(EEPROM_SETTINGS))
      #define ZOFFSET_TOTAL ZOFFSET_SAVE

      switch (item) {
        case ZOFFSET_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            liveadjust = false;
            Draw_Menu(Prepare, 5);
          }
          break;
        case ZOFFSET_HOME:
          if (draw) {
            Draw_Menu_Item(row, ICON_Homing, (char*)"Home Z Axis");
          }
          else {
            Popup_Handler(Home);
            gcode.process_subcommands_now_P( PSTR("G28 Z\nG1 Z0"));
            planner.synchronize();
            Draw_Menu(ZOffset);
          }
          break;
        case ZOFFSET_MODE:
          if (draw) {
            if (liveadjust)
              Draw_Menu_Item(row, ICON_Zoffset, (char*)"Live Adjust: Enabled");
            else
              Draw_Menu_Item(row, ICON_Zoffset, (char*)"Live Adjust: Disabled");
          }
          else {
            if (!liveadjust) {
              Popup_Handler(Home);
              gcode.process_subcommands_now_P( PSTR("G28 Z O\nG1 Z0"));
              planner.synchronize();
              Draw_Menu(ZOffset, 2);
            }
            liveadjust = !liveadjust;
            Draw_Menu(ZOffset, 2);
          }
          break;
        case ZOFFSET_OFFSET:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetZOffset, (char*)"Z Offset");
            Draw_Float(zoffsetvalue, row, false, 100);
          }
          else {
            Modify_Value(zoffsetvalue, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
          }
          break;
        case ZOFFSET_UP:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Up");
          }
          else {
            if (zoffsetvalue < MAX_Z_OFFSET) {
              if(liveadjust) {
                gcode.process_subcommands_now_P(PSTR("M290 Z0.01"));
                planner.synchronize();
              }
              zoffsetvalue += 0.01;
              Draw_Float(zoffsetvalue, row-1, false, 100);
            }
          }
          break;
        case ZOFFSET_DOWN:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Down");
          }
          else {
            if (zoffsetvalue > MIN_Z_OFFSET) {
              if(liveadjust) {
                gcode.process_subcommands_now_P(PSTR("M290 Z-0.01"));
                planner.synchronize();
              }
              zoffsetvalue -= 0.01;
              Draw_Float(zoffsetvalue, row-2, false, 100);
            }
          }
          break;
        #if ENABLED(EEPROM_SETTINGS)
        case ZOFFSET_SAVE:
          if (draw) {
            Draw_Menu_Item(row, ICON_WriteEEPROM, (char*)"Save");
          }
          else {
            AudioFeedback(settings.save());
          }
          break;
        #endif
      }
      break;
    #endif
    #if HAS_PREHEAT
    case Preheat:

      #define PREHEAT_BACK 0
      #define PREHEAT_MODE (PREHEAT_BACK + 1)
      #define PREHEAT_1 (PREHEAT_MODE + (PREHEAT_COUNT >= 1))
      #define PREHEAT_2 (PREHEAT_1 + (PREHEAT_COUNT >= 2))
      #define PREHEAT_3 (PREHEAT_2 + (PREHEAT_COUNT >= 3))
      #define PREHEAT_4 (PREHEAT_3 + (PREHEAT_COUNT >= 4))
      #define PREHEAT_TOTAL PREHEAT_4

      switch (item) {
        case PREHEAT_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Prepare, 6);
          }
          break;
        case PREHEAT_MODE:
          if (draw) {
            if (bedonly)
              Draw_Menu_Item(row, ICON_Homing, (char*)"Preheat Mode: Bed");
            else
              Draw_Menu_Item(row, ICON_Homing, (char*)"Preheat Mode: Normal");
          }
          else {
            bedonly = !bedonly;
            Draw_Menu(Preheat, 1);
          }
          break;
        #if (PREHEAT_COUNT >= 1)
        case PREHEAT_1:
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_1_LABEL);
          }
          else {
            if (!bedonly) {
              thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0);
              thermalManager.set_fan_speed(0, ui.material_preset[0].fan_speed);
            }
            thermalManager.setTargetBed(ui.material_preset[0].bed_temp);
          }
          break;
        #endif
        #if (PREHEAT_COUNT >= 2)
        case PREHEAT_2:
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_2_LABEL);
          }
          else {
            if (!bedonly) {
              thermalManager.setTargetHotend(ui.material_preset[1].hotend_temp, 0);
              thermalManager.set_fan_speed(0, ui.material_preset[1].fan_speed);
            }
            thermalManager.setTargetBed(ui.material_preset[1].bed_temp);
          }
          break;
        #endif
        #if (PREHEAT_COUNT >= 3)
        case PREHEAT_3:
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_3_LABEL);
          }
          else {
            if (!bedonly) {
              thermalManager.setTargetHotend(ui.material_preset[2].hotend_temp, 0);
              thermalManager.set_fan_speed(0, ui.material_preset[2].fan_speed);
            }
            thermalManager.setTargetBed(ui.material_preset[2].bed_temp);
          }
          break;
        #endif
        #if (PREHEAT_COUNT >= 4)
        case PREHEAT_4:
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_4_LABEL);
          }
          else {
            if (!bedonly) {
              thermalManager.setTargetHotend(ui.material_preset[3].hotend_temp, 0);
              thermalManager.set_fan_speed(0, ui.material_preset[3].fan_speed);
            }
            thermalManager.setTargetBed(ui.material_preset[3].bed_temp);
          }
          break;
        #endif
      }
      break;
    #endif
    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    case ChangeFilament:

      #define CHANGEFIL_BACK 0
      #define CHANGEFIL_LOAD (CHANGEFIL_BACK + 1)
      #define CHANGEFIL_UNLOAD (CHANGEFIL_LOAD + 1)
      #define CHANGEFIL_CHANGE (CHANGEFIL_UNLOAD + 1)
      #define CHANGEFIL_TOTAL CHANGEFIL_CHANGE

      switch (item) {
        case CHANGEFIL_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Prepare, 8);
          }
          break;
        case CHANGEFIL_LOAD:
          if (draw) {
            Draw_Menu_Item(row, ICON_WriteEEPROM, (char*)"Load Filament");
          }
          else {
            Popup_Handler(FilLoad);
            gcode.process_subcommands_now_P(PSTR("M701"));
            planner.synchronize();
            Draw_Menu(Prepare, 8);
          }
          break;
        case CHANGEFIL_UNLOAD:
          if (draw) {
            Draw_Menu_Item(row, ICON_ReadEEPROM, (char*)"Unload Filament");
          }
          else {
            Popup_Handler(FilLoad, true);;
            gcode.process_subcommands_now_P(PSTR("M702"));
            planner.synchronize();
            Draw_Menu(Prepare, 8);
          }
          break;
        case CHANGEFIL_CHANGE:
          if (draw) {
            Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament");
          }
          else {
            Popup_Handler(FilChange);
            gcode.process_subcommands_now_P(PSTR("M600 B1"));
            planner.synchronize();
            Draw_Menu(Prepare, 8);
          }
          break;
      }
      break;
    #endif
    case Control:

      #define CONTROL_BACK 0
      #define CONTROL_TEMP (CONTROL_BACK + 1)
      #define CONTROL_MOTION (CONTROL_TEMP + 1)
      #define CONTROL_ADVANCED (CONTROL_MOTION + 1)
      #define CONTROL_SAVE (CONTROL_ADVANCED + ENABLED(EEPROM_SETTINGS))
      #define CONTROL_RESTORE (CONTROL_SAVE + ENABLED(EEPROM_SETTINGS))
      #define CONTROL_RESET (CONTROL_RESTORE + ENABLED(EEPROM_SETTINGS))
      #define CONTROL_INFO (CONTROL_RESET + 1)
      #define CONTROL_TOTAL CONTROL_INFO

      switch (item) {
        case CONTROL_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Main_Menu(2);
          }
          break;
        case CONTROL_TEMP:
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)"Temperature");
          }
          else {
            Draw_Menu(TempMenu);
          }
          break;
        case CONTROL_MOTION:
          if (draw) {
            Draw_Menu_Item(row, ICON_Motion, (char*)"Motion");
          }
          else {
            Draw_Menu(Motion);
          }
          break;
        case CONTROL_ADVANCED:
          if (draw) {
            Draw_Menu_Item(row, ICON_Version, (char*)"Advanced");
          }
          else {
            Draw_Menu(Advanced);
          }
          break;
        #if ENABLED(EEPROM_SETTINGS)
        case CONTROL_SAVE:
          if (draw) {
            Draw_Menu_Item(row, ICON_WriteEEPROM, (char*)"Store Settings");
          }
          else {
            AudioFeedback(settings.save());
          }
          break;
        case CONTROL_RESTORE:
          if (draw) {
            Draw_Menu_Item(row, ICON_ReadEEPROM, (char*)"Restore Setting");
          }
          else {
            AudioFeedback(settings.load());
          }
          break;
        case CONTROL_RESET:
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)"Reset Defaults");
          }
          else {
            settings.reset();
            AudioFeedback();
          }
          break;
        #endif
        case CONTROL_INFO:
          if (draw) {
            Draw_Menu_Item(row, ICON_Info, (char*)"Info");
          }
          else {
            Draw_Menu(Info);
          }
          break;
      }
      break;
    case TempMenu:

      #define TEMP_BACK 0
      #define TEMP_HOTEND (TEMP_BACK + ENABLED(HAS_HOTEND))
      #define TEMP_BED (TEMP_HOTEND + ENABLED(HAS_HEATED_BED))
      #define TEMP_FAN (TEMP_BED + ENABLED(HAS_FAN))
      #define TEMP_PREHEAT1 (TEMP_FAN + (PREHEAT_COUNT >= 1))
      #define TEMP_PREHEAT2 (TEMP_PREHEAT1 + (PREHEAT_COUNT >= 2))
      #define TEMP_PREHEAT3 (TEMP_PREHEAT2 + (PREHEAT_COUNT >= 3))
      #define TEMP_PREHEAT4 (TEMP_PREHEAT3 + (PREHEAT_COUNT >= 4))
      #define TEMP_TOTAL TEMP_PREHEAT4

      switch (item) {
        case TEMP_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Control, 1);
          }
          break;
        #if HAS_HOTEND
        case TEMP_HOTEND:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(thermalManager.temp_hotend[0].target, row, false, 1);
          }
          else {
            Modify_Value(thermalManager.temp_hotend[0].target, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        #endif
        #if HAS_HEATED_BED
        case TEMP_BED:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(thermalManager.temp_bed.target, row, false, 1);
          }
          else {
            Modify_Value(thermalManager.temp_bed.target, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        #endif
        #if HAS_FAN
        case TEMP_FAN:
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(thermalManager.fan_speed[0], row, false, 1);
          }
          else {
            Modify_Value(thermalManager.fan_speed[0], MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
        #endif
        #if (PREHEAT_COUNT >= 1)
        case TEMP_PREHEAT1:
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)PREHEAT_1_LABEL);
          }
          else {
            Draw_Menu(Preheat1);
          }
          break;
        #endif
        #if (PREHEAT_COUNT >= 2)
        case TEMP_PREHEAT2:
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)PREHEAT_2_LABEL);
          }
          else {
            Draw_Menu(Preheat2);
          }
          break;
        #endif
        #if (PREHEAT_COUNT >= 3)
        case TEMP_PREHEAT3:
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)PREHEAT_3_LABEL);
          }
          else {
            Draw_Menu(Preheat3);
          }
          break;
        #endif
        #if (PREHEAT_COUNT >= 4)
        case TEMP_PREHEAT4:
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)PREHEAT_4_LABEL);
          }
          else {
            Draw_Menu(Preheat4);
          }
          break;
        #endif
      }
      break;
    #if (PREHEAT_COUNT >= 1)
    case Preheat1:

      #define PREHEAT1_BACK 0
      #define PREHEAT1_HOTEND (PREHEAT1_BACK + ENABLED(HAS_HOTEND))
      #define PREHEAT1_BED (PREHEAT1_HOTEND + ENABLED(HAS_HEATED_BED))
      #define PREHEAT1_FAN (PREHEAT1_BED + ENABLED(HAS_FAN))
      #define PREHEAT1_TOTAL PREHEAT1_FAN

      switch (item) {
        case PREHEAT1_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(TempMenu, 4);
          }
          break;
        #if HAS_HOTEND
        case PREHEAT1_HOTEND:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(ui.material_preset[0].hotend_temp, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[0].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        #endif
        #if HAS_HEATED_BED
        case PREHEAT1_BED:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(ui.material_preset[0].bed_temp, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[0].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        #endif
        #if HAS_FAN
        case PREHEAT1_FAN:
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(ui.material_preset[0].fan_speed, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[0].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
        #endif
      }
<<<<<<< HEAD
      break;
    #endif
    #if (PREHEAT_COUNT >= 2)
    case Preheat2:

      #define PREHEAT2_BACK 0
      #define PREHEAT2_HOTEND (PREHEAT2_BACK + ENABLED(HAS_HOTEND))
      #define PREHEAT2_BED (PREHEAT2_HOTEND + ENABLED(HAS_HEATED_BED))
      #define PREHEAT2_FAN (PREHEAT2_BED + ENABLED(HAS_FAN))
      #define PREHEAT2_TOTAL PREHEAT2_FAN

      switch (item) {
        case PREHEAT2_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(TempMenu, 5);
          }
          break;
        #if HAS_HOTEND
        case PREHEAT2_HOTEND:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(ui.material_preset[1].hotend_temp, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[1].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        #endif
        #if HAS_HEATED_BED
        case PREHEAT2_BED:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(ui.material_preset[1].bed_temp, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[1].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        #endif
        #if HAS_FAN
        case PREHEAT2_FAN:
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(ui.material_preset[1].fan_speed, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[1].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
=======
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_print.now) {
      case 0: // Tune
        checkkey = Tune;
        HMI_ValueStruct.show_mode = 0;
        select_tune.reset();
        index_tune = MROWS;
        Draw_Tune_Menu();
        break;
      case 1: // Pause
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

      case 2: // Stop
        HMI_flag.select_flag = true;
        checkkey = Print_window;
        Popup_window_PauseOrStop();
        break;

      default: break;
    }
  }
  DWIN_UpdateLCD();
}

/* Pause and Stop window */
void HMI_PauseOrStop() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_CW)
    Draw_Select_Highlight(false);
  else if (encoder_diffState == ENCODER_DIFF_CCW)
    Draw_Select_Highlight(true);
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (select_print.now == 1) { // pause window
      if (HMI_flag.select_flag) {
        HMI_flag.pause_action = true;
        ICON_Continue();
        #if ENABLED(POWER_LOSS_RECOVERY)
          if (recovery.enabled) recovery.save(true);
>>>>>>> origin/bugfix-2.0.x
        #endif
      }
      break;
    #endif
    #if (PREHEAT_COUNT >= 3)
    case Preheat3:

      #define PREHEAT3_BACK 0
      #define PREHEAT3_HOTEND (PREHEAT3_BACK + ENABLED(HAS_HOTEND))
      #define PREHEAT3_BED (PREHEAT3_HOTEND + ENABLED(HAS_HEATED_BED))
      #define PREHEAT3_FAN (PREHEAT3_BED + ENABLED(HAS_FAN))
      #define PREHEAT3_TOTAL PREHEAT3_FAN

      switch (item) {
        case PREHEAT3_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(TempMenu, 6);
          }
          break;
        #if HAS_HOTEND
        case PREHEAT3_HOTEND:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(ui.material_preset[2].hotend_temp, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[2].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        #endif
        #if HAS_HEATED_BED
        case PREHEAT3_BED:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(ui.material_preset[2].bed_temp, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[2].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        #endif
        #if HAS_FAN
        case PREHEAT3_FAN:
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(ui.material_preset[2].fan_speed, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[2].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
        #endif
      }
<<<<<<< HEAD
      break;
=======
      else
        Goto_PrintProcess(); // cancel stop
    }
  }
  DWIN_UpdateLCD();
}

void Draw_Move_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 192, 1, 233, 14); // "Move"
    DWIN_Frame_AreaCopy(1, 58, 118, 106, 132, LBLX, MBASE(1));
    DWIN_Frame_AreaCopy(1, 109, 118, 157, 132, LBLX, MBASE(2));
    DWIN_Frame_AreaCopy(1, 160, 118, 209, 132, LBLX, MBASE(3));
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 212, 118, 253, 131, LBLX, MBASE(4));
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_MOVE_AXIS));
    #else
      DWIN_Frame_TitleCopy(1, 231, 2, 265, 12);                     // "Move"
    #endif
    draw_move_en(MBASE(1)); say_x(36, MBASE(1));                    // "Move X"
    draw_move_en(MBASE(2)); say_y(36, MBASE(2));                    // "Move Y"
    draw_move_en(MBASE(3)); say_z(36, MBASE(3));                    // "Move Z"
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 123, 192, 176, 202, LBLX, MBASE(4));   // "Extruder"
>>>>>>> origin/bugfix-2.0.x
    #endif
    #if (PREHEAT_COUNT >= 4)
    case Preheat4:

      #define PREHEAT4_BACK 0
      #define PREHEAT4_HOTEND (PREHEAT4_BACK + ENABLED(HAS_HOTEND))
      #define PREHEAT4_BED (PREHEAT4_HOTEND + ENABLED(HAS_HEATED_BED))
      #define PREHEAT4_FAN (PREHEAT4_BED + ENABLED(HAS_FAN))
      #define PREHEAT4_TOTAL PREHEAT4_FAN

      switch (item) {
        case PREHEAT4_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(TempMenu, 7);
          }
          break;
        #if HAS_HOTEND
        case PREHEAT4_HOTEND:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(ui.material_preset[3].hotend_temp, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[3].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        #endif
        #if HAS_HEATED_BED
        case PREHEAT4_BED:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(ui.material_preset[3].bed_temp, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[3].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        #endif
        #if HAS_FAN
        case PREHEAT4_FAN:
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(ui.material_preset[3].fan_speed, row, false, 1);
          }
          else {
            Modify_Value(ui.material_preset[3].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
        #endif
      }
      break;
    #endif
    case Motion:

      #define MOTION_BACK 0
      #define MOTION_SPEED (MOTION_BACK + 1)
      #define MOTION_ACCEL (MOTION_SPEED + 1)
      #define MOTION_JERK (MOTION_ACCEL + ENABLED(HAS_CLASSIC_JERK))
      #define MOTION_STEPS (MOTION_JERK + 1)
      #define MOTION_FLOW (MOTION_STEPS + ENABLED(HAS_HOTEND))
      #define MOTION_TOTAL MOTION_FLOW

      switch (item) {
        case MOTION_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Control, 2);
          }
          break;
        case MOTION_SPEED:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, (char*)"Max Speed");
          }
          else {
            Draw_Menu(MaxSpeed);
          }
          break;
        case MOTION_ACCEL:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccelerated, (char*)"Max Acceleration");
          }
          else {
            Draw_Menu(MaxAcceleration);
          }
          break;
        #if HAS_CLASSIC_JERK
        case MOTION_JERK:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxJerk, (char*)"Max Jerk");
          }
          else {
            Draw_Menu(MaxJerk);
          }
          break;
        #endif
        case MOTION_STEPS:
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)"Steps/mm");
          }
          else {
            Draw_Menu(Steps);
          }
          break;
        #if HAS_HOTEND
        case MOTION_FLOW:
          if (draw) {
            Draw_Menu_Item(row, ICON_Speed, (char*)"Flow Rate");
            Draw_Float(planner.flow_percentage[0], row, false, 1);
          }
          else {
            Modify_Value(planner.flow_percentage[0], MIN_FLOW_RATE, MAX_FLOW_RATE, 1);
          }
          break;
        #endif
      }
      break;
    case MaxSpeed:

      #define SPEED_BACK 0
      #define SPEED_X (SPEED_BACK + 1)
      #define SPEED_Y (SPEED_X + 1)
      #define SPEED_Z (SPEED_Y + 1)
      #define SPEED_E (SPEED_Z + ENABLED(HAS_HOTEND))
      #define SPEED_TOTAL SPEED_E

      switch (item) {
        case SPEED_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Motion, 1);
          }
          break;
        case SPEED_X:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedX, (char*)"X Axis");
            Draw_Float(planner.settings.max_feedrate_mm_s[X_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.settings.max_feedrate_mm_s[X_AXIS], 0, default_max_feedrate[X_AXIS]*2, 1);
          }
          break;
        case SPEED_Y:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedY, (char*)"Y Axis");
            Draw_Float(planner.settings.max_feedrate_mm_s[Y_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.settings.max_feedrate_mm_s[Y_AXIS], 0, default_max_feedrate[Y_AXIS]*2, 1);
          }
          break;
        case SPEED_Z:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedZ, (char*)"Z Axis");
            Draw_Float(planner.settings.max_feedrate_mm_s[Z_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.settings.max_feedrate_mm_s[Z_AXIS], 0, default_max_feedrate[Z_AXIS]*2, 1);
          }
          break;
        #if HAS_HOTEND
        case SPEED_E:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedE, (char*)"Extruder");
            Draw_Float(planner.settings.max_feedrate_mm_s[E_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.settings.max_feedrate_mm_s[Z_AXIS], 0, default_max_feedrate[E_AXIS]*2, 1);
          }
          break;
        #endif
      }
<<<<<<< HEAD
      break;
    case MaxAcceleration:

      #define ACCEL_BACK 0
      #define ACCEL_X (ACCEL_BACK + 1)
      #define ACCEL_Y (ACCEL_X + 1)
      #define ACCEL_Z (ACCEL_Y + 1)
      #define ACCEL_E (ACCEL_Z + ENABLED(HAS_HOTEND))
      #define ACCEL_TOTAL ACCEL_E

      switch (item) {
        case ACCEL_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Motion, 2);
          }
          break;
        case ACCEL_X:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccX, (char*)"X Axis");
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[X_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[X_AXIS], 0, default_max_acceleration[X_AXIS]*2, 1);
          }
          break;
        case ACCEL_Y:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccY, (char*)"Y Axis");
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], 0, default_max_acceleration[Y_AXIS]*2, 1);
          }
          break;
        case ACCEL_Z:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccZ, (char*)"Z Axis");
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], 0, default_max_acceleration[Z_AXIS]*2, 1);
          }
          break;
        #if HAS_HOTEND
        case ACCEL_E:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccE, (char*)"Extruder");
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[E_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[E_AXIS], 0, default_max_acceleration[E_AXIS]*2, 1);
          }
          break;
=======
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_prepare.now) {
      case 0: // Back
        select_page.set(1);
        Goto_MainMenu();
        break;
      case PREPARE_CASE_MOVE: // Axis move
        checkkey = AxisMove;
        select_axis.reset();
        Draw_Move_Menu();

        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(1), current_position.x * MINUNITMULT);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(2), current_position.y * MINUNITMULT);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(3), current_position.z * MINUNITMULT);
        #if HAS_HOTEND
          HMI_ValueStruct.Move_E_scaled = current_position.e * MINUNITMULT;
          DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(4), HMI_ValueStruct.Move_E_scaled);
>>>>>>> origin/bugfix-2.0.x
        #endif
      }
      break;
    #if HAS_CLASSIC_JERK
    case MaxJerk:

      #define JERK_BACK 0
      #define JERK_X (JERK_BACK + 1)
      #define JERK_Y (JERK_X + 1)
      #define JERK_Z (JERK_Y + 1)
      #define JERK_E (JERK_Z + ENABLED(HAS_HOTEND))
      #define JERK_TOTAL JERK_E

      switch (item) {
        case JERK_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Motion, 3);
          }
          break;
<<<<<<< HEAD
        case JERK_X:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedJerkX, (char*)"X Axis");
            Draw_Float(planner.max_jerk[X_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.max_jerk[X_AXIS], 0, default_max_jerk[X_AXIS]*2, 1);
          }
          break;
        case JERK_Y:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedJerkY, (char*)"Y Axis");
            Draw_Float(planner.max_jerk[Y_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.max_jerk[Y_AXIS], 0, default_max_jerk[Y_AXIS]*2, 1);
          }
          break;
        case JERK_Z:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedJerkZ, (char*)"Z Axis");
            Draw_Float(planner.max_jerk[Z_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.max_jerk[Z_AXIS], 0, default_max_jerk[Z_AXIS]*2, 1);
          }
          break;
        #if HAS_HOTEND
        case JERK_E:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedJerkE, (char*)"Extruder");
            Draw_Float(planner.max_jerk[E_AXIS], row, false, 1);
          }
          else {
            Modify_Value(planner.max_jerk[E_AXIS], 0, default_max_jerk[E_AXIS]*2, 1);
          }
          break;
        #endif
      }
      break;
=======
      #endif
      #if HAS_PREHEAT
        case PREPARE_CASE_PLA: // PLA preheat
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0));
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(ui.material_preset[0].bed_temp));
          TERN_(HAS_FAN, thermalManager.set_fan_speed(0, ui.material_preset[0].fan_speed));
          break;
        case PREPARE_CASE_ABS: // ABS preheat
          TERN_(HAS_HOTEND, thermalManager.setTargetHotend(ui.material_preset[1].hotend_temp, 0));
          TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(ui.material_preset[1].bed_temp));
          TERN_(HAS_FAN, thermalManager.set_fan_speed(0, ui.material_preset[1].fan_speed));
          break;
        case PREPARE_CASE_COOL: // Cool
          TERN_(HAS_FAN, thermalManager.zero_fan_speeds());
          #if HAS_HOTEND || HAS_HEATED_BED
            thermalManager.disable_all_heaters();
          #endif
          break;
      #endif
      case PREPARE_CASE_LANG: // Toggle Language
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
    DWIN_Frame_TitleCopy(1, 236, 2, 263, 13); // "Temperature"
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 1, 134, 56, 146, LBLX, MBASE(TEMP_CASE_TEMP));
    #endif
    #if HAS_HEATED_BED
      DWIN_Frame_AreaCopy(1, 58, 134, 113, 146, LBLX, MBASE(TEMP_CASE_BED));
    #endif
    #if HAS_FAN
      DWIN_Frame_AreaCopy(1, 115, 134, 170, 146, LBLX, MBASE(TEMP_CASE_FAN));
    #endif
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 100, 89, 178, 101, LBLX, MBASE(TEMP_CASE_PLA));
      DWIN_Frame_AreaCopy(1, 180, 89, 260, 100, LBLX, MBASE(TEMP_CASE_ABS));
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_TEMPERATURE));
    #else
      DWIN_Frame_TitleCopy(1, 56, 16, 141, 28);                                       // "Temperature"
    #endif
    #ifdef USE_STRING_TITLES
      #if HAS_HOTEND
        DWIN_Draw_Label(MBASE(TEMP_CASE_TEMP), GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND));
      #endif
      #if HAS_HEATED_BED
        DWIN_Draw_Label(MBASE(TEMP_CASE_BED), GET_TEXT_F(MSG_UBL_SET_TEMP_BED));
      #endif
      #if HAS_FAN
        DWIN_Draw_Label(MBASE(TEMP_CASE_FAN), GET_TEXT_F(MSG_FAN_SPEED));
      #endif
      #if HAS_HOTEND
        DWIN_Draw_Label(MBASE(TEMP_CASE_PLA), F("PLA Preheat Settings"));
        DWIN_Draw_Label(MBASE(TEMP_CASE_ABS), F("ABS Preheat Settings"));
      #endif
    #else
      #if HAS_HOTEND
        DWIN_Frame_AreaCopy(1, 197, 104, 238, 114, LBLX, MBASE(TEMP_CASE_TEMP));      // Nozzle...
        DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 44, MBASE(TEMP_CASE_TEMP));     // ...Temperature
      #endif
      #if HAS_HEATED_BED
        DWIN_Frame_AreaCopy(1, 240, 104, 264, 114, LBLX, MBASE(TEMP_CASE_BED));       // Bed...
        DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 27, MBASE(TEMP_CASE_BED));      // ...Temperature
      #endif
      #if HAS_FAN
        DWIN_Frame_AreaCopy(1, 0, 119, 64, 132, LBLX, MBASE(TEMP_CASE_FAN));          // Fan speed
      #endif
      #if HAS_HOTEND
        DWIN_Frame_AreaCopy(1, 107, 76, 156, 86, LBLX, MBASE(TEMP_CASE_PLA));         // Preheat...
        DWIN_Frame_AreaCopy(1, 157, 76, 181, 86, LBLX + 52, MBASE(TEMP_CASE_PLA));    // ...PLA
        DWIN_Frame_AreaCopy(1, 131, 119, 182, 132, LBLX + 79, MBASE(TEMP_CASE_PLA));  // PLA setting
        DWIN_Frame_AreaCopy(1, 107, 76, 156, 86, LBLX, MBASE(TEMP_CASE_ABS));         // Preheat...
        DWIN_Frame_AreaCopy(1, 172, 76, 198, 86, LBLX + 52, MBASE(TEMP_CASE_ABS));    // ...ABS
        DWIN_Frame_AreaCopy(1, 131, 119, 182, 132, LBLX + 81, MBASE(TEMP_CASE_ABS));  // ABS setting
      #endif
>>>>>>> origin/bugfix-2.0.x
    #endif
    case Steps:

      #define STEPS_BACK 0
      #define STEPS_X (STEPS_BACK + 1)
      #define STEPS_Y (STEPS_X + 1)
      #define STEPS_Z (STEPS_Y + 1)
      #define STEPS_E (STEPS_Z + ENABLED(HAS_HOTEND))
      #define STEPS_TOTAL STEPS_E

      switch (item) {
        case STEPS_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Motion, 4);
          }
          break;
        case STEPS_X:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepX, (char*)"X Axis");
            Draw_Float(planner.settings.axis_steps_per_mm[X_AXIS], row, false, 10);
          }
          else {
            Modify_Value(planner.settings.axis_steps_per_mm[X_AXIS], 0, default_steps[X_AXIS]*2, 10);
          }
          break;
        case STEPS_Y:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepY, (char*)"Y Axis");
            Draw_Float(planner.settings.axis_steps_per_mm[Y_AXIS], row, false, 10);
          }
          else {
            Modify_Value(planner.settings.axis_steps_per_mm[Y_AXIS], 0, default_steps[Y_AXIS]*2, 10);
          }
          break;
        case STEPS_Z:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepZ, (char*)"Z Axis");
            Draw_Float(planner.settings.axis_steps_per_mm[Z_AXIS], row, false, 10);
          }
          else {
            Modify_Value(planner.settings.axis_steps_per_mm[Z_AXIS], 0, default_steps[Z_AXIS]*2, 10);
          }
          break;
        #if HAS_HOTEND
        case STEPS_E:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepE, (char*)"Extruder");
            Draw_Float(planner.settings.axis_steps_per_mm[E_AXIS], row, false, 10);
          }
          else {
            Modify_Value(planner.settings.axis_steps_per_mm[E_AXIS], 0, default_steps[E_AXIS]*2, 10);
          }
          break;
        #endif
      }
      break;
    case Advanced:

      #define ADVANCED_BACK 0
      #define ADVANCED_XOFFSET (ADVANCED_BACK + ENABLED(HAS_BED_PROBE))
      #define ADVANCED_YOFFSET (ADVANCED_XOFFSET + ENABLED(HAS_BED_PROBE))
      #define ADVANCED_TOTAL ADVANCED_YOFFSET

<<<<<<< HEAD
      switch (item) {
        case ADVANCED_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Control, 3);
          }
=======
  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_control.inc(1 + CONTROL_CASE_TOTAL)) {
      if (select_control.now > MROWS && select_control.now > index_control) {
        index_control = select_control.now;
        Scroll_Menu(DWIN_SCROLL_UP);
        Draw_Menu_Icon(MROWS, ICON_Temperature + index_control - 1);
        Draw_More_Icon(CONTROL_CASE_TEMP + MROWS - index_control); // Temperature >
        Draw_More_Icon(CONTROL_CASE_MOVE + MROWS - index_control); // Motion >
        if (index_control > MROWS) {
          Draw_More_Icon(CONTROL_CASE_INFO + MROWS - index_control); // Info >
          Item_Control_Info(MBASE(CONTROL_CASE_INFO - 1));
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
      case 0: // Back
        select_page.set(2);
        Goto_MainMenu();
        break;
      case CONTROL_CASE_TEMP: // Temperature
        checkkey = TemperatureID;
        HMI_ValueStruct.show_mode = -1;
        select_temp.reset();
        Draw_Temperature_Menu();
        break;
      case CONTROL_CASE_MOVE: // Motion
        checkkey = Motion;
        select_motion.reset();
        Draw_Motion_Menu();
        break;
      #if ENABLED(EEPROM_SETTINGS)
        case CONTROL_CASE_SAVE: { // Write EEPROM
          const bool success = settings.save();
          HMI_AudioFeedback(success);
        } break;
        case CONTROL_CASE_LOAD: { // Read EEPROM
          const bool success = settings.load();
          HMI_AudioFeedback(success);
        } break;
        case CONTROL_CASE_RESET: // Reset EEPROM
          settings.reset();
          HMI_AudioFeedback();
>>>>>>> origin/bugfix-2.0.x
          break;
        #if ENABLED(HAS_BED_PROBE)
        case ADVANCED_XOFFSET:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Probe X Offset");
            Draw_Float(probe.offset.x, row, false, 10);
          }
          else {
            Modify_Value(probe.offset.x, -50, 50, 10);
          }
          break;
        case ADVANCED_YOFFSET:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Probe Y Offset");
            Draw_Float(probe.offset.y, row, false, 10);
          }
          else {
            Modify_Value(probe.offset.y, -50, 50, 10);
          }
          break;
        #endif
      }
      break;
    case InfoMain:
    case Info:

      #define INFO_BACK 0
      #define INFO_TOTAL INFO_BACK

      switch (item) {
        case INFO_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
            DWIN_Draw_Line(Line_Color, 16, MBASE(1) + 33, 256, MBASE(1) + 34);

            DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(MACHINE_SIZE) * MENU_CHR_W) / 2, MBASE(2) - 1, (char*)MACHINE_SIZE);
            DWIN_ICON_Show(ICON, ICON_PrintSize, 26, MBASE(2) - 3);
            DWIN_Draw_Line(Line_Color, 16, MBASE(2) + 33, 256, MBASE(2) + 34);

            DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(SHORT_BUILD_VERSION) * MENU_CHR_W) / 2, MBASE(3) - 1, (char*)SHORT_BUILD_VERSION);
            DWIN_ICON_Show(ICON, ICON_Version, 26, MBASE(3) - 3);
            DWIN_Draw_Line(Line_Color, 16, MBASE(3) + 33, 256, MBASE(3) + 34);

<<<<<<< HEAD
            DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(CORP_WEBSITE_E) * MENU_CHR_W) / 2, MBASE(4) - 1, (char*)CORP_WEBSITE_E);
            DWIN_ICON_Show(ICON, ICON_Version, 26, MBASE(4) - 3);
            DWIN_Draw_Line(Line_Color, 16, MBASE(4) + 33, 256, MBASE(4) + 34);
          }
          else {
            if (menu == Info)
              Draw_Menu(Control, 7);
            else 
              Draw_Main_Menu(3);
          }
          break;
      }
      break;
    #if ENABLED(PROBE_MANUALLY)

      #define MMESH_BACK 0
      #define MMESH_NEXT (MMESH_BACK + 1)
      #define MMESH_OFFSET (MMESH_NEXT + 1)
      #define MMESH_UP (MMESH_OFFSET + 1)
      #define MMESH_DOWN (MMESH_UP + 1)
      #define MMESH_TOTAL MMESH_DOWN

      case ManualMesh:
        switch (item) {
          case MMESH_BACK:
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Cancel");
            }
            else {
              gcode.process_subcommands_now_P(PSTR("G29 A"));
              planner.synchronize();
              Draw_Main_Menu(3);
            }
            break;
          case MMESH_NEXT:
            if (draw) {
              if (gridpoint < GRID_MAX_POINTS)
                Draw_Menu_Item(row, ICON_More, (char*)"Next Point");
              else
                Draw_Menu_Item(row, ICON_More, (char*)"Save Mesh");
            }
            else {
              if (gridpoint < GRID_MAX_POINTS) {
                Popup_Handler(MoveWait);
                gcode.process_subcommands_now_P(PSTR("G29"));
                planner.synchronize();
                gridpoint++;
                Draw_Menu(ManualMesh, 1);
              }
              else {
                gcode.process_subcommands_now_P(PSTR("G29"));
                planner.synchronize();
                AudioFeedback(settings.save());
                Draw_Main_Menu(3);
              }
            }
=======
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    // popup window resume
    if (HMI_flag.ETempTooLow_flag) {
      if (encoder_diffState == ENCODER_DIFF_ENTER) {
        HMI_flag.ETempTooLow_flag = false;
        HMI_ValueStruct.Move_E_scaled = current_position.e * MINUNITMULT;
        Draw_Move_Menu();
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 216, MBASE(1), HMI_ValueStruct.Move_X_scaled);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 216, MBASE(2), HMI_ValueStruct.Move_Y_scaled);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 216, MBASE(3), HMI_ValueStruct.Move_Z_scaled);
        DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(4), 0);
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
      case 0: // Back
        checkkey = Prepare;
        select_prepare.set(1);
        index_prepare = MROWS;
        Draw_Prepare_Menu();
        break;
      case 1: // X axis move
        checkkey = Move_X;
        HMI_ValueStruct.Move_X_scaled = current_position.x * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 1, 216, MBASE(1), HMI_ValueStruct.Move_X_scaled);
        EncoderRate.enabled = true;
        break;
      case 2: // Y axis move
        checkkey = Move_Y;
        HMI_ValueStruct.Move_Y_scaled = current_position.y * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 1, 216, MBASE(2), HMI_ValueStruct.Move_Y_scaled);
        EncoderRate.enabled = true;
        break;
      case 3: // Z axis move
        checkkey = Move_Z;
        HMI_ValueStruct.Move_Z_scaled = current_position.z * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 1, 216, MBASE(3), HMI_ValueStruct.Move_Z_scaled);
        EncoderRate.enabled = true;
        break;
        #if HAS_HOTEND
          case 4: // Extruder
            // window tips
            #ifdef PREVENT_COLD_EXTRUSION
              if (thermalManager.temp_hotend[0].celsius < EXTRUDE_MINTEMP) {
                HMI_flag.ETempTooLow_flag = true;
                Popup_Window_ETempTooLow();
                DWIN_UpdateLCD();
                return;
              }
            #endif
            checkkey = Extruder;
            HMI_ValueStruct.Move_E_scaled = current_position.e * MINUNITMULT;
            DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, 1, 216, MBASE(4), HMI_ValueStruct.Move_E_scaled);
            EncoderRate.enabled = true;
>>>>>>> origin/bugfix-2.0.x
            break;
          case MMESH_OFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetZOffset, (char*)"Z Position");
              current_position.z = MANUAL_PROBE_START_Z;
              Draw_Float(current_position.z, row, false, 100);
            }
            else {
              Modify_Value(current_position.z, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
            }
            break;
          case MMESH_UP:
            if (draw) {
              Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Up");
            }
            else {
              if (current_position.z < MAX_Z_OFFSET) {
                gcode.process_subcommands_now_P(PSTR("M290 Z0.01"));
                planner.synchronize();
                current_position.z += 0.01f;
                sync_plan_position();
                Draw_Float(current_position.z, row-1, false, 100);
              }
            }
            break;
          case MMESH_DOWN:
            if (draw) {
              Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Down");
            }
            else {
              if (current_position.z > MIN_Z_OFFSET) {
                gcode.process_subcommands_now_P(PSTR("M290 Z-0.01"));
                planner.synchronize();
                current_position.z -= 0.01f;
                sync_plan_position();
                Draw_Float(current_position.z, row-2, false, 100);
              }
            }
            break;
        }
        break;
    #endif
    case Tune:

      #define TUNE_BACK 0
      #define TUNE_SPEED (TUNE_BACK + 1)
      #define TUNE_FLOW (TUNE_SPEED + ENABLED(HAS_HOTEND))
      #define TUNE_HOTEND (TUNE_FLOW + ENABLED(HAS_HOTEND))
      #define TUNE_BED (TUNE_HOTEND + ENABLED(HAS_HEATED_BED))
      #define TUNE_FAN (TUNE_BED + ENABLED(HAS_FAN))
      #define TUNE_ZOFFSET (TUNE_FAN + ENABLED(HAS_ZOFFSET_ITEM))
      #define TUNE_ZUP (TUNE_ZOFFSET + ENABLED(HAS_ZOFFSET_ITEM))
      #define TUNE_ZDOWN (TUNE_ZUP + ENABLED(HAS_ZOFFSET_ITEM))
      #define TUNE_CHANGEFIL (TUNE_ZDOWN + ENABLED(FILAMENT_LOAD_UNLOAD_GCODES))
      #define TUNE_TOTAL TUNE_CHANGEFIL

      switch (item) {
        case TUNE_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Print_Screen();
          }
          break;
        case TUNE_SPEED:
          if (draw) {
            Draw_Menu_Item(row, ICON_Speed, (char*)"Print Speed");
            Draw_Float(feedrate_percentage, row, false, 1);
          }
          else {
            Modify_Value(feedrate_percentage, MIN_PRINT_SPEED, MAX_PRINT_SPEED, 1);
          }
          break;
        #if HAS_HOTEND
        case TUNE_FLOW:
          if (draw) {
            Draw_Menu_Item(row, ICON_Speed, (char*)"Flow Rate");
            Draw_Float(planner.flow_percentage[0], row, false, 1);
          }
          else {
            Modify_Value(planner.flow_percentage[0], MIN_FLOW_RATE, MAX_FLOW_RATE, 1);
          }
          break;
        case TUNE_HOTEND:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(thermalManager.temp_hotend[0].target, row, false, 1);
          }
          else {
<<<<<<< HEAD
            Modify_Value(thermalManager.temp_hotend[0].target, MIN_E_TEMP, MAX_E_TEMP, 1);
=======
            #ifdef USE_STRING_HEADINGS
              Draw_Title("PLA Settings"); // TODO: GET_TEXT_F
            #else
              DWIN_Frame_TitleCopy(1, 56, 16, 141, 28);                                             // "PLA Settings"
            #endif
            #ifdef USE_STRING_TITLES
              DWIN_Draw_Label(MBASE(PREHEAT_CASE_TEMP), F("Nozzle Temp"));
              #if HAS_HEATED_BED
                DWIN_Draw_Label(MBASE(PREHEAT_CASE_BED), F("Bed Temp"));
              #endif
              #if HAS_FAN
                DWIN_Draw_Label(MBASE(PREHEAT_CASE_FAN), GET_TEXT_F(MSG_FAN_SPEED));
              #endif
              #if ENABLED(EEPROM_SETTINGS)
                DWIN_Draw_Label(MBASE(PREHEAT_CASE_SAVE), GET_TEXT_F(MSG_STORE_EEPROM));
              #endif
            #else
              DWIN_Frame_AreaCopy(1, 157, 76, 181, 86, LBLX, MBASE(PREHEAT_CASE_TEMP));
              DWIN_Frame_AreaCopy(1, 197, 104, 238, 114, LBLX + 27, MBASE(PREHEAT_CASE_TEMP));
              DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 71, MBASE(PREHEAT_CASE_TEMP)); // PLA nozzle temp
              #if HAS_HEATED_BED
                DWIN_Frame_AreaCopy(1, 157, 76, 181, 86, LBLX, MBASE(PREHEAT_CASE_BED) + 3);
                DWIN_Frame_AreaCopy(1, 240, 104, 264, 114, LBLX + 27, MBASE(PREHEAT_CASE_BED) + 3);
                DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 54, MBASE(PREHEAT_CASE_BED) + 3); // PLA bed temp
              #endif
              #if HAS_FAN
                DWIN_Frame_AreaCopy(1, 157, 76, 181, 86, LBLX, MBASE(PREHEAT_CASE_FAN));
                DWIN_Frame_AreaCopy(1, 0, 119, 64, 132, LBLX + 27, MBASE(PREHEAT_CASE_FAN));    // PLA fan speed
              #endif
              #if ENABLED(EEPROM_SETTINGS)
                DWIN_Frame_AreaCopy(1, 97, 165, 229, 177, LBLX, MBASE(PREHEAT_CASE_SAVE));          // Save PLA configuration
              #endif
            #endif
>>>>>>> origin/bugfix-2.0.x
          }
          break;
        #endif
        #if HAS_HEATED_BED
        case TUNE_BED:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(thermalManager.temp_bed.target, row, false, 1);
          }
          else {
<<<<<<< HEAD
            Modify_Value(thermalManager.temp_bed.target, MIN_BED_TEMP, MAX_BED_TEMP, 1);
=======
            #ifdef USE_STRING_HEADINGS
              Draw_Title("ABS Settings"); // TODO: GET_TEXT_F
            #else
              DWIN_Frame_TitleCopy(1, 56, 16, 141, 28);                                                  // "ABS Settings"
            #endif
            #ifdef USE_STRING_TITLES
              DWIN_Draw_Label(MBASE(PREHEAT_CASE_TEMP), F("Nozzle Temp"));
              #if HAS_HEATED_BED
                DWIN_Draw_Label(MBASE(PREHEAT_CASE_BED), F("Bed Temp"));
              #endif
              #if HAS_FAN
                DWIN_Draw_Label(MBASE(PREHEAT_CASE_FAN), GET_TEXT_F(MSG_FAN_SPEED));
              #endif
              #if ENABLED(EEPROM_SETTINGS)
                DWIN_Draw_Label(MBASE(PREHEAT_CASE_SAVE), GET_TEXT_F(MSG_STORE_EEPROM));
              #endif
            #else
              DWIN_Frame_AreaCopy(1, 172, 76, 198, 86, LBLX, MBASE(PREHEAT_CASE_TEMP));
              DWIN_Frame_AreaCopy(1, 197, 104, 238, 114, LBLX + 27, MBASE(PREHEAT_CASE_TEMP));
              DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 71, MBASE(PREHEAT_CASE_TEMP));      // ABS nozzle temp
              #if HAS_HEATED_BED
                DWIN_Frame_AreaCopy(1, 172, 76, 198, 86, LBLX, MBASE(PREHEAT_CASE_BED) + 3);
                DWIN_Frame_AreaCopy(1, 240, 104, 264, 114, LBLX + 27, MBASE(PREHEAT_CASE_BED) + 3);
                DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 54, MBASE(PREHEAT_CASE_BED) + 3); // ABS bed temp
              #endif
              #if HAS_FAN
                DWIN_Frame_AreaCopy(1, 172, 76, 198, 86, LBLX, MBASE(PREHEAT_CASE_FAN));
                DWIN_Frame_AreaCopy(1, 0, 119, 64, 132, LBLX + 27, MBASE(PREHEAT_CASE_FAN));             // ABS fan speed
              #endif
              #if ENABLED(EEPROM_SETTINGS)
                DWIN_Frame_AreaCopy(1, 97, 165, 229, 177, LBLX, MBASE(PREHEAT_CASE_SAVE));
                DWIN_Frame_AreaCopy(1, 172, 76, 198, 86, LBLX + 33, MBASE(PREHEAT_CASE_SAVE));                     // Save ABS configuration
              #endif
            #endif
>>>>>>> origin/bugfix-2.0.x
          }
          break;
        #endif
        #if HAS_FAN
        case TUNE_FAN:
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(thermalManager.fan_speed[0], row, false, 1);
          }
          else {
            Modify_Value(thermalManager.fan_speed[0], MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
        #endif
        #if HAS_ZOFFSET_ITEM
        case TUNE_ZOFFSET:
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Z-Offset");
            // TODO this does not update when using Up/Down below
            Draw_Float(zoffsetvalue, row, false, 100);
          }
          else {
            Modify_Value(zoffsetvalue, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
          }
          break;
        case TUNE_ZUP:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Z-Offset Up");
          }
          else {
            if (zoffsetvalue < MAX_Z_OFFSET) {
              gcode.process_subcommands_now_P(PSTR("M290 Z0.01"));
              zoffsetvalue += 0.01;
              Draw_Float(zoffsetvalue, row-1, false, 100);
            }
          }
          break;
        case TUNE_ZDOWN:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Z-Offset Down");
          }
          else {
            if (zoffsetvalue > MIN_Z_OFFSET) {
              gcode.process_subcommands_now_P(PSTR("M290 Z-0.01"));
              zoffsetvalue -= 0.01;
              Draw_Float(zoffsetvalue, row-2, false, 100);
            }
          }
          break;
        #endif
        #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        case TUNE_CHANGEFIL:
          if (draw) {
            Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament");
          }
          else {
            Popup_Handler(FilChange);
            gcode.process_subcommands_now_P(PSTR("M600 B1"));
            planner.synchronize();
            Draw_Print_Screen();
          }
          break;
        #endif
      }
      break;
  }
}

<<<<<<< HEAD
char* Get_Menu_Title(uint8_t menu) {
  switch(menu) {
    case MainMenu:
      return (char*)"Ender 3 V2";
    case Prepare:
      return (char*)"Prepare";
    case Move:
      return (char*)"Move";
    case ManualLevel:
      return (char*)"Manual Leveling";
    #if HAS_ZOFFSET_ITEM
    case ZOffset:
      return (char*)"Z Offset";
    #endif
    #if HAS_PREHEAT
    case Preheat:
      return (char*)"Preheat";
    #endif
    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    case ChangeFilament:
      return (char*)"Change Filament";
    #endif
    case Control:
      return (char*)"Control";
    case TempMenu:
      return (char*)"Temperature";
    case Preheat1:
      return (char*)"Warmup Settings";
    case Preheat2:
      return (char*)"PrePLA Settings";
    case Preheat3:
      return (char*)"PreABS Settings";
    case Preheat4:
      return (char*)"PrePETG Settings";
    case Motion:
      return (char*)"Motion Settings";
    case MaxSpeed:
      return (char*)"Max Speed";
    case MaxAcceleration:
      return (char*)"Max Acceleration";
    #if HAS_CLASSIC_JERK
    case MaxJerk:
      return (char*)"Max Jerk";
    #endif
    case Steps:
      return (char*)"Steps/mm";
    case Advanced:
      return (char*)"Advanced Settings";
    case Info:
      return (char*)"Info";
    #if ENABLED(PROBE_MANUALLY)
    case ManualMesh:
      return (char*)"Mesh Bed Leveling";
=======
void Draw_Max_Speed_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 1, 16, 28, 28); // "Max Speed (mm/s)"

    auto say_max_speed = [](const uint16_t row) {
      DWIN_Frame_AreaCopy(1, 173, 133, 228, 147, LBLX, row);              // "Max speed"
    };

    say_max_speed(MBASE(1));                                              // "Max speed"
    DWIN_Frame_AreaCopy(1, 229, 133, 236, 147, LBLX + 58, MBASE(1));      // X
    say_max_speed(MBASE(2));                                              // "Max speed"
    DWIN_Frame_AreaCopy(1, 1, 150, 7, 160, LBLX + 58, MBASE(2) + 3);      // Y
    say_max_speed(MBASE(3));                                              // "Max speed"
    DWIN_Frame_AreaCopy(1, 9, 150, 16, 160, LBLX + 58, MBASE(3) + 3);     // Z
    #if HAS_HOTEND
      say_max_speed(MBASE(4));                                            // "Max speed"
      DWIN_Frame_AreaCopy(1, 18, 150, 25, 160, LBLX + 58, MBASE(4) + 3);  // E
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title("Max Speed (mm/s)"); // TODO: GET_TEXT_F
    #else
      DWIN_Frame_TitleCopy(1, 144, 16, 189, 26); // "Max Speed (mm/s)"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(1), F("Max Feedrate X"));
      DWIN_Draw_Label(MBASE(2), F("Max Feedrate Y"));
      DWIN_Draw_Label(MBASE(3), F("Max Feedrate Z"));
      #if HAS_HOTEND
        DWIN_Draw_Label(MBASE(4), F("Max Feedrate E"));
      #endif
    #else
      draw_max_en(MBASE(1));          // "Max"
      DWIN_Frame_AreaCopy(1, 184, 119, 234, 132, LBLX + 27, MBASE(1)); // "Speed X"

      draw_max_en(MBASE(2));          // "Max"
      draw_speed_en(27, MBASE(2));    // "Speed"
      say_y(70, MBASE(2));            // "Y"

      draw_max_en(MBASE(3));          // "Max"
      draw_speed_en(27, MBASE(3));    // "Speed"
      say_z(70, MBASE(3));            // "Z"

      #if HAS_HOTEND
        draw_max_en(MBASE(4));        // "Max"
        draw_speed_en(27, MBASE(4));  // "Speed"
        say_e(70, MBASE(4));          // "E"
      #endif
    #endif
  }

  Draw_Back_First();
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_MaxSpeedX + i);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(1), planner.settings.max_feedrate_mm_s[X_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(2), planner.settings.max_feedrate_mm_s[Y_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(3), planner.settings.max_feedrate_mm_s[Z_AXIS]);
  #if HAS_HOTEND
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(4), planner.settings.max_feedrate_mm_s[E_AXIS]);
  #endif
}

void Draw_Max_Accel_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 1, 16, 28, 28); // "Acceleration"

    DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX, MBASE(1));
    DWIN_Frame_AreaCopy(1, 28, 149, 69, 161, LBLX + 27, MBASE(1) + 1);
    DWIN_Frame_AreaCopy(1, 229, 133, 236, 147, LBLX + 71, MBASE(1));   // Max acceleration X
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX, MBASE(2));
    DWIN_Frame_AreaCopy(1, 28, 149, 69, 161, LBLX + 27, MBASE(2) + 1);
    DWIN_Frame_AreaCopy(1, 1, 150, 7, 160, LBLX + 71, MBASE(2) + 2);   // Max acceleration Y
    DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX, MBASE(3));
    DWIN_Frame_AreaCopy(1, 28, 149, 69, 161, LBLX + 27, MBASE(3) + 1);
    DWIN_Frame_AreaCopy(1, 9, 150, 16, 160, LBLX + 71, MBASE(3) + 2);  // Max acceleration Z
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX, MBASE(4));
      DWIN_Frame_AreaCopy(1, 28, 149, 69, 161, LBLX + 27, MBASE(4) + 1);
      DWIN_Frame_AreaCopy(1, 18, 150, 25, 160, LBLX + 71, MBASE(4) + 2); // Max acceleration E
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_ACCELERATION));
    #else
      DWIN_Frame_TitleCopy(1, 144, 16, 189, 26);          // "Acceleration"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(1), F("Max Accel X"));
      DWIN_Draw_Label(MBASE(2), F("Max Accel Y"));
      DWIN_Draw_Label(MBASE(3), F("Max Accel Z"));
      #if HAS_HOTEND
        DWIN_Draw_Label(MBASE(4), F("Max Accel E"));
      #endif
    #else
      draw_max_accel_en(MBASE(1)); say_x(108, MBASE(1));  // "Max Acceleration X"
      draw_max_accel_en(MBASE(2)); say_y(108, MBASE(2));  // "Max Acceleration Y"
      draw_max_accel_en(MBASE(3)); say_z(108, MBASE(3));  // "Max Acceleration Z"
      #if HAS_HOTEND
        draw_max_accel_en(MBASE(4)); say_e(108, MBASE(4)); // "Max Acceleration E"
      #endif
>>>>>>> origin/bugfix-2.0.x
    #endif
    case Tune:
      return (char*)"Tune";
  }
  return (char*)"";
}

<<<<<<< HEAD
int Get_Menu_Size(uint8_t menu) {
  switch(menu) {
    case Prepare:
      return PREPARE_TOTAL;
    case Move:
      return MOVE_TOTAL;
    case ManualLevel:
      return MLEVEL_TOTAL;
    #if HAS_ZOFFSET_ITEM
    case ZOffset:
      return ZOFFSET_TOTAL;
    #endif
    #if HAS_PREHEAT
    case Preheat:
      return PREHEAT_TOTAL;
    #endif
    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    case ChangeFilament:
      return CHANGEFIL_TOTAL;
=======
#if HAS_CLASSIC_JERK
  void Draw_Max_Jerk_Menu() {
    Clear_Main_Window();

    if (HMI_IsChinese()) {
      DWIN_Frame_TitleCopy(1, 1, 16, 28, 28); // "Jerk"

      DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX     , MBASE(1));
      DWIN_Frame_AreaCopy(1,   1, 180,  28, 192, LBLX + 27, MBASE(1) + 1);
      DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 53, MBASE(1));
      DWIN_Frame_AreaCopy(1, 229, 133, 236, 147, LBLX + 83, MBASE(1));        // Max Jerk speed X
      DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX     , MBASE(2));
      DWIN_Frame_AreaCopy(1,   1, 180,  28, 192, LBLX + 27, MBASE(2) + 1);
      DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 53, MBASE(2));
      DWIN_Frame_AreaCopy(1,   1, 150,   7, 160, LBLX + 83, MBASE(2) + 3);    // Max Jerk speed Y
      DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX     , MBASE(3));
      DWIN_Frame_AreaCopy(1,   1, 180,  28, 192, LBLX + 27, MBASE(3) + 1);
      DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 53, MBASE(3));
      DWIN_Frame_AreaCopy(1,   9, 150,  16, 160, LBLX + 83, MBASE(3) + 3);    // Max Jerk speed Z
      #if HAS_HOTEND
        DWIN_Frame_AreaCopy(1, 173, 133, 200, 147, LBLX     , MBASE(4));
        DWIN_Frame_AreaCopy(1,   1, 180,  28, 192, LBLX + 27, MBASE(4) + 1);
        DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 53, MBASE(4));
        DWIN_Frame_AreaCopy(1,  18, 150,  25, 160, LBLX + 83, MBASE(4) + 3);  // Max Jerk speed E
      #endif
    }
    else {
      #ifdef USE_STRING_HEADINGS
        Draw_Title(GET_TEXT_F(MSG_JERK));
      #else
        DWIN_Frame_TitleCopy(1, 144, 16, 189, 26); // "Jerk"
      #endif
      #ifdef USE_STRING_TITLES
        DWIN_Draw_Label(MBASE(1), F("Max Jerk X"));
        DWIN_Draw_Label(MBASE(2), F("Max Jerk Y"));
        DWIN_Draw_Label(MBASE(3), F("Max Jerk Z"));
        #if HAS_HOTEND
          DWIN_Draw_Label(MBASE(4), F("Max Jerk E"));
        #endif
      #else
        draw_max_en(MBASE(1));          // "Max"
        draw_jerk_en(MBASE(1));         // "Jerk"
        draw_speed_en(72, MBASE(1));    // "Speed"
        say_x(115, MBASE(1));           // "X"

        draw_max_en(MBASE(2));          // "Max"
        draw_jerk_en(MBASE(2));         // "Jerk"
        draw_speed_en(72, MBASE(2));    // "Speed"
        say_y(115, MBASE(2));           // "Y"

        draw_max_en(MBASE(3));          // "Max"
        draw_jerk_en(MBASE(3));         // "Jerk"
        draw_speed_en(72, MBASE(3));    // "Speed"
        say_z(115, MBASE(3));           // "Z"

        #if HAS_HOTEND
          draw_max_en(MBASE(4));        // "Max"
          draw_jerk_en(MBASE(4));       // "Jerk"
          draw_speed_en(72, MBASE(4));  // "Speed"
          say_e(115, MBASE(4));         // "E"
        #endif
      #endif
    }

    Draw_Back_First();
    LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_MaxSpeedJerkX + i);
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(1), planner.max_jerk[X_AXIS] * MINUNITMULT);
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(2), planner.max_jerk[Y_AXIS] * MINUNITMULT);
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(3), planner.max_jerk[Z_AXIS] * MINUNITMULT);
    #if HAS_HOTEND
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(4), planner.max_jerk[E_AXIS] * MINUNITMULT);
    #endif
  }
#endif

void Draw_Steps_Menu() {
  Clear_Main_Window();

  if (HMI_IsChinese()) {
    DWIN_Frame_TitleCopy(1, 1, 16, 28, 28); // "Steps per mm"

    DWIN_Frame_AreaCopy(1, 153, 148, 194, 161, LBLX, MBASE(1));
    DWIN_Frame_AreaCopy(1, 229, 133, 236, 147, LBLX + 44, MBASE(1)); // Transmission Ratio X
    DWIN_Frame_AreaCopy(1, 153, 148, 194, 161, LBLX, MBASE(2));
    DWIN_Frame_AreaCopy(1, 1, 150, 7, 160, LBLX + 44, MBASE(2) + 3); // Transmission Ratio Y
    DWIN_Frame_AreaCopy(1, 153, 148, 194, 161, LBLX, MBASE(3));
    DWIN_Frame_AreaCopy(1, 9, 150, 16, 160, LBLX + 44, MBASE(3) + 3); // Transmission Ratio Z
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 153, 148, 194, 161, LBLX, MBASE(4));
      DWIN_Frame_AreaCopy(1, 18, 150, 25, 160, LBLX + 44, MBASE(4) + 3); // Transmission Ratio E
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Draw_Title(GET_TEXT_F(MSG_STEPS_PER_MM));
    #else
      DWIN_Frame_TitleCopy(1, 144, 16, 189, 26); // "Steps per mm"
    #endif
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(1), F("Steps/mm X"));
      DWIN_Draw_Label(MBASE(2), F("Steps/mm Y"));
      DWIN_Draw_Label(MBASE(3), F("Steps/mm Z"));
      #if HAS_HOTEND
        DWIN_Draw_Label(MBASE(4), F("Steps/mm E"));
      #endif
    #else
      draw_steps_per_mm(MBASE(1)); say_x(103, MBASE(1)); // "Steps-per-mm X"
      draw_steps_per_mm(MBASE(2)); say_y(103, MBASE(2)); // "Y"
      draw_steps_per_mm(MBASE(3)); say_z(103, MBASE(3)); // "Z"
      #if HAS_HOTEND
        draw_steps_per_mm(MBASE(4)); say_e(103, MBASE(4)); // "E"
      #endif
>>>>>>> origin/bugfix-2.0.x
    #endif
    case Control:
      return CONTROL_TOTAL;
    case TempMenu:
      return TEMP_TOTAL;
    case Preheat1:
      return PREHEAT1_TOTAL;
    case Preheat2:
      return PREHEAT2_TOTAL;
    case Preheat3:
      return PREHEAT3_TOTAL;
    case Preheat4:
      return PREHEAT4_TOTAL;
    case Motion:
      return MOTION_TOTAL;
    case MaxSpeed:
      return SPEED_TOTAL;
    case MaxAcceleration:
      return ACCEL_TOTAL;
    #if HAS_CLASSIC_JERK
    case MaxJerk:
      return JERK_TOTAL;
    #endif
    case Steps:
      return STEPS_TOTAL;
    case Advanced:
      return ADVANCED_TOTAL;
    case Info:
      return INFO_TOTAL;
    #if ENABLED(PROBE_MANUALLY)
    case ManualMesh:
      return MMESH_TOTAL;
    #endif
    case Tune:
      return TUNE_TOTAL;
  }
<<<<<<< HEAD
  return 0;
=======

  Draw_Back_First();
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_StepX + i);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(1), planner.settings.axis_steps_per_mm[X_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(2), planner.settings.axis_steps_per_mm[Y_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(3), planner.settings.axis_steps_per_mm[Z_AXIS] * MINUNITMULT);
  #if HAS_HOTEND
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(4), planner.settings.axis_steps_per_mm[E_AXIS] * MINUNITMULT);
  #endif
>>>>>>> origin/bugfix-2.0.x
}

/* Popup Functions */

void Popup_Select() {
  const uint16_t c1 = (selection==0) ? Select_Color : Color_Bg_Window,
                 c2 = (selection==0) ? Color_Bg_Window : Select_Color;
  DWIN_Draw_Rectangle(0, c1, 25, 279, 126, 318);
  DWIN_Draw_Rectangle(0, c1, 24, 278, 127, 319);
  DWIN_Draw_Rectangle(0, c2, 145, 279, 246, 318);
  DWIN_Draw_Rectangle(0, c2, 144, 278, 247, 319);
}

void Popup_Handler(uint8_t popupid, bool option/*=false*/) {
  popup = popupid;
  switch (popupid) {
    case Pause:
      Draw_Popup((char*)"Pause Print", (char*)"", (char*)"", Popup);
      break;
    case Stop:
      Draw_Popup((char*)"Stop Print", (char*)"", (char*)"", Popup);
      break;
    case Resume:
      Draw_Popup((char*)"Resume Print?", (char*)"Looks Like the last", (char*)"print was interupted.", Popup);
      break;
    case SaveLevel:
      Draw_Popup((char*)"Leveling Complete", (char*)"Save to EEPROM?", (char*)"", Popup);
      break;
    case ETemp:
      Draw_Popup((char*)"Nozzle is too cold", (char*)"Preheat to 200C?", (char*)"", Popup, ICON_TempTooLow);
      break;
    case Level:
      Draw_Popup((char*)"Auto Bed Leveling", (char*)"Please wait until done.", (char*)"", Wait, ICON_AutoLeveling);
      break;
    case Home:
      Draw_Popup(option ? (char*)"Parking" : (char*)"Homing", (char*)"Please wait until done.", (char*)"", Wait, ICON_BLTouch);
      break;
    case MoveWait:
      Draw_Popup((char*)"Moving to Point", (char*)"Please wait until done.", (char*)"", Wait, ICON_BLTouch);
      break;
    case M600:
      Draw_Popup(option ? (char*)"Filament Change" : (char*)"Print Paused", option ? (char*)"Insert New Filament" : (char*)"Press to resume print", (char*)"", Confirm);
      break;
    case FilLoad:
      Draw_Popup(option ? (char*)"Unloading Filament" : (char*)"Loading Filament", (char*)"Please wait until done.", (char*)"", Wait, ICON_BLTouch);
      break;
    case FilChange:
      Draw_Popup((char*)"Filament Change", (char*)"Please wait while heating.", (char*)"", Wait, ICON_BLTouch);
      break;
  }
}

void DWIN_Popup_Temperature(const bool toohigh) {
  Clear_Screen(4);
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 105, 258, 350);
  if (toohigh) {
    DWIN_ICON_Show(ICON, ICON_TempTooHigh, 102, 165);
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 36, 300, (char*)"Nozzle or Bed temperature");
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 92, 300, (char*)"is too high");
  }
  else {
    DWIN_ICON_Show(ICON, ICON_TempTooLow, 102, 165);
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 36, 300, (char*)"Nozzle or Bed temperature");
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 92, 300, (char*)"is too low");
  }
}

/* Navigation and Control */

inline void Main_Menu_Control() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW && selection < 3) {
    selection++; // Select Down
    Main_Menu_Icons();
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    selection--; // Select Up
    Main_Menu_Icons();
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER)
    switch(selection) {
      case 0:
        card.mount();
        Draw_SD_List();
        break;
      case 1:
        Draw_Menu(Prepare);
        break;
      case 2:
        Draw_Menu(Control);
        break;
      case 3:
        #if HAS_ONESTEP_LEVELING
          Popup_Handler(Level);
          gcode.process_subcommands_now_P(PSTR("G28\nG29"));
          planner.synchronize();
          Popup_Handler(SaveLevel);
        #elif ENABLED(PROBE_MANUALLY)
          gridpoint = 1;
          Popup_Handler(Home);
          gcode.process_subcommands_now_P(PSTR("G28"));
          planner.synchronize();
          Popup_Handler(MoveWait);
          gcode.process_subcommands_now_P(PSTR("G29"));
          planner.synchronize();
          Draw_Menu(ManualMesh);
        #else

        #endif
        break;
    }
  DWIN_UpdateLCD();
}

inline void Menu_Control() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW && selection < Get_Menu_Size(active_menu)) {
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, MBASE(selection-scrollpos) - 18, 14, MBASE(selection-scrollpos) + 33);
    selection++; // Select Down
    if (selection > scrollpos+MROWS) {
      scrollpos++;
      DWIN_Frame_AreaMove(1, 2, MLINE, Color_Bg_Black, 0, 31, DWIN_WIDTH, 349);
      Menu_Item_Handler(active_menu, selection);
    }
    DWIN_Draw_Rectangle(1, Rectangle_Color, 0, MBASE(selection-scrollpos) - 18, 14, MBASE(selection-scrollpos) + 33);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, MBASE(selection-scrollpos) - 18, 14, MBASE(selection-scrollpos) + 33);
    selection--; // Select Up
    if (selection < scrollpos) {
      scrollpos--;
      DWIN_Frame_AreaMove(1, 3, MLINE, Color_Bg_Black, 0, 31, DWIN_WIDTH, 349);
      Menu_Item_Handler(active_menu, selection);
    }
    DWIN_Draw_Rectangle(1, Rectangle_Color, 0, MBASE(selection-scrollpos) - 18, 14, MBASE(selection-scrollpos) + 33);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER)
    Menu_Item_Handler(active_menu, selection, false);
  DWIN_UpdateLCD();
}

inline void Value_Control() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW) {
    tempvalue += EncoderRate.encoderMoveValue;
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    tempvalue -= EncoderRate.encoderMoveValue;
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if ((active_menu == ZOffset && liveadjust) || (active_menu == Tune && selection == 6/*ZOffset*/)) {
      float pos = current_position.z;
      current_position.z += (tempvalue/valueunit - zoffsetvalue);
      planner.synchronize();
      planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
      current_position.z = pos;
      sync_plan_position();
    }
    switch (valuetype) {
      case 0: *(float*)valuepointer = tempvalue/valueunit; break;
      case 1: *(uint8_t*)valuepointer = tempvalue/valueunit; break;
      case 2: *(uint16_t*)valuepointer = tempvalue/valueunit; break;
      case 3: *(int16_t*)valuepointer = tempvalue/valueunit; break;
      case 4: *(uint32_t*)valuepointer = tempvalue/valueunit; break;
    }
    process = Menu;
    EncoderRate.enabled = false;
    Draw_Float(tempvalue/valueunit, selection-scrollpos, false, valueunit);
    DWIN_UpdateLCD();
    if (active_menu == Move) {
      planner.synchronize();
      planner.buffer_line(current_position, homing_feedrate(AxisEnum(selection-1)), active_extruder);
    }
    if (active_menu == ManualMesh) {
      planner.synchronize();
      planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
      planner.synchronize();
    }
    return;
  }
  NOLESS(tempvalue, (valuemin * valueunit));
  NOMORE(tempvalue, (valuemax * valueunit));
  Draw_Float(tempvalue/valueunit, selection-scrollpos, true, valueunit);
  DWIN_UpdateLCD();
}

inline void File_Control() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) {
    if (selection > 0) {
      card.getfilename_sorted(SD_ORDER(selection-1, card.get_num_Files()));
      char * const filename = card.longest_filename();
      size_t len = strlen(filename);
      int8_t pos = len;
      if (!card.flag.filenameIsDir)
        while (pos && filename[pos] != '.') pos--;
      if (pos > MENU_CHAR_LIMIT) {
        static millis_t time = 0;
        if (PENDING(millis(), time)) return;
        time = millis() + 200;
        pos -= filescrl;
        len = pos;
        if (len > MENU_CHAR_LIMIT)
          len = MENU_CHAR_LIMIT;
        char name[len+1];
        if (pos >= 0) {
          LOOP_L_N(i, pos) name[i] = filename[i+filescrl];
        }
        else {
          LOOP_L_N(i, MENU_CHAR_LIMIT+pos) name[i] = ' ';
          LOOP_S_L_N(i, MENU_CHAR_LIMIT+pos, MENU_CHAR_LIMIT) name[i] = filename[i-(MENU_CHAR_LIMIT+pos)];
        }
        name[len] = '\0';
        DWIN_Draw_Rectangle(1, Color_Bg_Black, LBLX, MBASE(selection-scrollpos) - 14, 271, MBASE(selection-scrollpos) + 28);
        Draw_Menu_Item(selection-scrollpos, card.flag.filenameIsDir ? ICON_More : ICON_File, name);
        if (-pos >= MENU_CHAR_LIMIT)
          filescrl = 0;
        filescrl++;
        DWIN_UpdateLCD();
      }
    }
    return;
  }
  if (encoder_diffState == ENCODER_DIFF_CW && selection < card.get_num_Files()) {
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, MBASE(selection-scrollpos) - 18, 14, MBASE(selection-scrollpos) + 33);
    if (selection > 0) {
      DWIN_Draw_Rectangle(1, Color_Bg_Black, LBLX, MBASE(selection-scrollpos) - 14, 271, MBASE(selection-scrollpos) + 28);
      Draw_SD_Item(selection, selection-scrollpos);
    }
    filescrl = 0;
    selection++; // Select Down
    if (selection > scrollpos+MROWS) {
      scrollpos++;
      DWIN_Frame_AreaMove(1, 2, MLINE, Color_Bg_Black, 0, 31, DWIN_WIDTH, 349);
      Draw_SD_Item(selection, selection-scrollpos);
    }
    DWIN_Draw_Rectangle(1, Rectangle_Color, 0, MBASE(selection-scrollpos) - 18, 14, MBASE(selection-scrollpos) + 33);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, MBASE(selection-scrollpos) - 18, 14, MBASE(selection-scrollpos) + 33);
    DWIN_Draw_Rectangle(1, Color_Bg_Black, LBLX, MBASE(selection-scrollpos) - 14, 271, MBASE(selection-scrollpos) + 28);
    Draw_SD_Item(selection, selection-scrollpos);
    filescrl = 0;
    selection--; // Select Up
    if (selection < scrollpos) {
      scrollpos--;
      DWIN_Frame_AreaMove(1, 3, MLINE, Color_Bg_Black, 0, 31, DWIN_WIDTH, 349);
      Draw_SD_Item(selection, selection-scrollpos);
    }
    DWIN_Draw_Rectangle(1, Rectangle_Color, 0, MBASE(selection-scrollpos) - 18, 14, MBASE(selection-scrollpos) + 33);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (selection == 0) {
      if(card.flag.workDirIsRoot) {
        process = Main;
        Draw_Main_Menu();
      }
      else {
        card.cdup();
        Draw_SD_List();
      }
    }
<<<<<<< HEAD
    else {
      card.getfilename_sorted(SD_ORDER(selection-1, card.get_num_Files()));
      if (card.flag.filenameIsDir) {
        card.cd(card.filename);
        Draw_SD_List();
=======
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      if (WITHIN(select_jerk.now, 1, 4)) {
        checkkey = MaxJerk_value;
        HMI_flag.jerk_axis = AxisEnum(select_jerk.now - 1);
        HMI_ValueStruct.Max_Jerk_scaled = planner.max_jerk[HMI_flag.jerk_axis] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 210, MBASE(select_jerk.now), HMI_ValueStruct.Max_Jerk_scaled);
        EncoderRate.enabled = true;
>>>>>>> origin/bugfix-2.0.x
      }
      else {
        card.openAndPrintFile(card.filename);
        Start_Print(true);
      }
    }
  }
  DWIN_UpdateLCD();
}

inline void Print_Screen_Control() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW && selection < 2) {
    selection++; // Select Down
    Print_Screen_Icons();
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    selection--; // Select Up
    Print_Screen_Icons();
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER)
    switch(selection) {
      case 0:
        Draw_Menu(Tune);
        DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(lastmsg) * MENU_CHR_W) / 2, 357, lastmsg);
        break;
      case 1:
        if (paused) {
          if (sdprint) {
            //char cmnd[200];
            //cmnd[sprintf(cmnd, "M140 S%i", pausebed)] = '\0';
            //gcode.process_subcommands_now_P(PSTR(cmnd));
            //cmnd[sprintf(cmnd, "M109 S%i", pausetemp)] = '\0';
            //gcode.process_subcommands_now_P(PSTR(cmnd));
            paused = false;
            wait_for_user = false;
            if (card.isFileOpen()) {
              card.startFileprint();
              startOrResumeJob();
              TERN_(POWER_LOSS_RECOVERY, recovery.prepare());
            }
          }
          else {
            #if ENABLED(HOST_ACTION_COMMANDS)
              host_action_resume();
            #endif
          }
          Draw_Print_Screen();
        }
        else
          Popup_Handler(Pause);
        break;
      case 2:
        Popup_Handler(Stop);
        break;
    }
  DWIN_UpdateLCD();
}

<<<<<<< HEAD
inline void Popup_Control() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW && selection < 1) {
    selection++;
    Popup_Select();
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    selection--;
    Popup_Select();
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER)
    switch(popup) {
      case Pause:
        if (selection==0) {
          if (sdprint) {
            paused = true;
            Popup_Handler(Home, true);
            #if ENABLED(SDSUPPORT)
              if (IS_SD_PRINTING()) card.pauseSDPrint();
            #endif
            #if ENABLED(POWER_LOSS_RECOVERY)
              if (recovery.enabled) recovery.save(true);
            #endif
            print_job_timer.pause();
            planner.synchronize();
            queue.inject_P(PSTR("M125"));
            planner.synchronize();
          }
          else {
            #if ENABLED(HOST_ACTION_COMMANDS)
              paused = true;
              host_action_pause();
            #endif
          }
        }
        Draw_Print_Screen();
        break;
      case Stop:
        if (selection==0) {
          if (sdprint) {
            card.flag.abort_sd_printing = true; 
            thermalManager.zero_fan_speeds();
            thermalManager.disable_all_heaters();
          }
          else {
            #if ENABLED(HOST_ACTION_COMMANDS)
              host_action_cancel();
            #endif
          }
          Draw_Main_Menu();
        }
        else {
          Draw_Print_Screen();
        }
        break;
      case Resume:
        if (selection==0) {
          queue.inject_P(PSTR("M1000"));
        }
        else {
          Draw_Main_Menu();
        }
        break;
      case ETemp:
        if (selection==0) {
          thermalManager.temp_hotend[0].target = 200;
          Draw_Menu(Move, 4);
        }
        else {
          Draw_Menu(Move, 4);
        }
        break;
      case SaveLevel:
        if (selection==0) {
          AudioFeedback(settings.save());
          Draw_Main_Menu();
        }
        else {
          Draw_Main_Menu();
        }
        break;
=======
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
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step_scaled);
      EncoderRate.enabled = true;
>>>>>>> origin/bugfix-2.0.x
    }
  DWIN_UpdateLCD();
}

inline void Confirm_Control() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch(popup) {
      case Complete:
        Draw_Main_Menu();
        break;
      case M600:
        wait_for_user = false;
        if (printing)
          Draw_Print_Screen();
        else
          Draw_Menu(Prepare, 8);
        break;
    }
  }
  DWIN_UpdateLCD();
}

/* In-Menu Value Modification */

void Setup_Value(float value, float min, float max, float unit, uint8_t type) {
  tempvalue = value * unit;
  valuemin = min;
  valuemax = max;
  valueunit = unit;
  valuetype = type;
  process = Value;
  EncoderRate.enabled = true;
  Draw_Float(tempvalue/unit, selection-scrollpos, true, valueunit);
}

void Modify_Value(float &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 0);
}
void Modify_Value(uint8_t &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 1);
}
void Modify_Value(uint16_t &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 2);
}
void Modify_Value(int16_t &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 3);
}
void Modify_Value(uint32_t &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 4);
}

<<<<<<< HEAD
/* Host Control */
=======
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
>>>>>>> origin/bugfix-2.0.x

void Host_Print_Update(uint8_t percent, uint32_t remaining) {
  printpercent = percent;
  remainingtime = remaining * 60;
  if (process == Print || process == Confirm) {
    Draw_Print_ProgressBar();
    Draw_Print_ProgressRemain();
    Draw_Print_ProgressElapsed();
  }
}

void Host_Print_Text(char * const text) {
  LOOP_L_N(i, _MIN((size_t)30, strlen(text))) lastmsg[i] = text[i];
  lastmsg[_MIN((size_t)30, strlen(text))] = '\0';
  if (process == Print) {
    DWIN_Draw_Rectangle(1, Color_Grey, 8, 214, DWIN_WIDTH-8, 238);
    const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(lastmsg) * MENU_CHR_W) / 2;
    DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, npos, 219, lastmsg);
  }
  else {
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 352, DWIN_WIDTH-8, 376);
    const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(lastmsg) * MENU_CHR_W) / 2;
    DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, npos, 357, lastmsg);
  }
}

<<<<<<< HEAD
/* Main Functions */
=======
      // show percent bar and value
      _card_percent = 0;
      Draw_Print_ProgressBar();
>>>>>>> origin/bugfix-2.0.x

void Start_Print(bool sd) {
  if (card.isPrinting())
    sdprint = true;
  else
    sdprint = sd;
  printing = true;
  printpercent = 0;
  remainingtime = 0;
  elapsedtime = 0;
  dwin_heat_time = 0;
  lastmsg[0] = '\0';
  Draw_Print_Screen();
}

void Stop_Print() {
  printing = false;
  thermalManager.zero_fan_speeds();
  thermalManager.disable_all_heaters();
  if (process == Print) {
    Draw_Print_confirm();
  }
  else {
    Draw_Print_Screen();
    Draw_Print_confirm();
  }
}

<<<<<<< HEAD
void DWIN_Update() {
  Screen_Update();
  Variable_Update();
}

void Variable_Update() {
  #if HAS_ZOFFSET_ITEM
    static float lastzoffset = zoffsetvalue;
    if (zoffsetvalue != lastzoffset) {
      lastzoffset = zoffsetvalue;
      #if HAS_BED_PROBE
        probe.offset.z = zoffsetvalue;
      #else
        set_home_offset(Z_AXIS, -zoffsetvalue);
      #endif
    }
    
    #if HAS_BED_PROBE
      if (probe.offset.z != lastzoffset) {
        zoffsetvalue = lastzoffset = probe.offset.z;
      }
    #else
      if (-home_offset.z != lastzoffset) {
        zoffsetvalue = lastzoffset = -home_offset.z;
      }
=======
  // pause after homing
  if (HMI_flag.pause_action && printingIsPaused() && !planner.has_blocks_queued()) {
    HMI_flag.pause_action = false;
    #if ENABLED(PAUSE_HEAT)
      TERN_(HAS_HOTEND, resume_hotend_temp = thermalManager.temp_hotend[0].target);
      TERN_(HAS_HEATED_BED, resume_bed_temp = thermalManager.temp_bed.target);
      thermalManager.disable_all_heaters();
>>>>>>> origin/bugfix-2.0.x
    #endif
  #endif
}

void Screen_Update() {
  static millis_t statustime = 0;
  if (ELAPSED(millis(), statustime)) {
    statustime = millis() + 500;
    Update_Status_Area();
  }

<<<<<<< HEAD
  static millis_t printtime = 0;
  if (ELAPSED(millis(), printtime)) {
    printtime = millis() + 1000;
    if (card.isPrinting() && process == Print) {
      duration_t elapsed = print_job_timer.duration();
      printpercent = card.percentDone();
      Draw_Print_ProgressBar();
      Draw_Print_ProgressElapsed();
      if (dwin_heat_time > 0 && printpercent > 0) {
        remainingtime = (elapsed.value - dwin_heat_time) / (printpercent * 0.01f) - (elapsed.value - dwin_heat_time);
        Draw_Print_ProgressRemain();
=======
  if (card.isPrinting() && checkkey == PrintProcess) { // print process
    const uint8_t card_pct = card.percentDone();
    static uint8_t last_cardpercentValue = 101;
    if (last_cardpercentValue != card_pct) { // print percent
      last_cardpercentValue = card_pct;
      if (card_pct) {
        _card_percent = card_pct;
        Draw_Print_ProgressBar();
>>>>>>> origin/bugfix-2.0.x
      }
    }
    if (process == Print && card.isPaused() && !paused) {
      paused = true;
      Print_Screen_Icons();
    }
<<<<<<< HEAD
    else if (process == Print && !card.isPaused() && paused) {
      paused = false;
      Print_Screen_Icons();
=======

    // Estimate remaining time every 20 seconds
    static millis_t next_remain_time_update = 0;
    if (_card_percent > 1 && ELAPSED(ms, next_remain_time_update) && !HMI_flag.heat_flag) {
      _remain_time = (elapsed.value - dwin_heat_time) / (_card_percent * 0.01f) - (elapsed.value - dwin_heat_time);
      next_remain_time_update += DWIN_REMAIN_TIME_UPDATE_INTERVAL;
      Draw_Print_ProgressRemain();
>>>>>>> origin/bugfix-2.0.x
    }
  }

  static bool mounted = card.isMounted();
  if (mounted != card.isMounted()) {
    mounted = card.isMounted();
    if (process == File)
      Draw_SD_List();
  }

<<<<<<< HEAD
  switch(process) {
    case Main:
      Main_Menu_Control();
      break;
    case Menu:
      Menu_Control();
      break;
    case Value:
      Value_Control();
      break;
    case File:
      File_Control();
      break;
    case Print:
      Print_Screen_Control();
      break;
    case Popup:
      Popup_Control();
      break;
    case Confirm:
      Confirm_Control();
      break;
  }
}

void HMI_Init() {

  for (uint16_t t = 0; t <= 100; t += 2) {
    DWIN_ICON_Show(ICON, ICON_Bar, 15, 260);
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 15 + t * 242 / 100, 260, 257, 280);
    DWIN_UpdateLCD();
    delay(20);
=======
      select_print.set(0);
      HMI_ValueStruct.show_mode = 0;
      queue.inject_P(PSTR("M1000"));
      Goto_PrintProcess();
      Draw_Status_Area(true);
    }
  #endif

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
>>>>>>> origin/bugfix-2.0.x
  }

  DWIN_JPG_CacheTo1(Language_English);

}

void HMI_StartFrame(const bool with_update) {
  Draw_Main_Menu();
  Draw_Status_Area(with_update);
}

void AudioFeedback(const bool success/*=true*/) {
  if (success) {
    buzzer.tone(100, 659);
    buzzer.tone(10, 0);
    buzzer.tone(100, 698);
  }
  else
    buzzer.tone(40, 440);
}

inline void HMI_SDCardInit() { card.cdroot(); }

void MarlinUI::refresh() {}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void MarlinUI::pause_show_message(const PauseMessage message, const PauseMode mode, const uint8_t extruder) {
    // TODO implement remainder of PauseMessage states
    if ((message == PAUSE_MESSAGE_INSERT || message == PAUSE_MESSAGE_WAITING) && !paused) {
      Popup_Handler(M600, (message == PAUSE_MESSAGE_INSERT));
    }
  }
#endif

#endif
