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

#ifndef strcasecmp_P
  #define strcasecmp_P(a, b) strcasecmp((a), (b))
#endif

#if HAS_ONESTEP_LEVELING
  #include "../../../feature/bedlevel/bedlevel.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#define MACHINE_SIZE STRINGIFY(X_BED_SIZE) "x" STRINGIFY(Y_BED_SIZE) "x" STRINGIFY(Z_MAX_POS)

#define CORP_WEBSITE_E "www.creality.com"

#define PAUSE_HEAT

#define DWIN_FONT_MENU font8x16
#define DWIN_FONT_STAT font10x20
#define DWIN_FONT_HEAD font10x20

#define MENU_CHAR_LIMIT  24
#define STATUS_Y 352

#define MAX_FAN_SPEED     255
#define MIN_FAN_SPEED     0

#define MAX_PRINT_SPEED   500
#define MIN_PRINT_SPEED   10

#define MAX_FLOW_RATE   200
#define MIN_FLOW_RATE   10

#define MAX_Z_OFFSET 9.99
#if HAS_BED_PROBE
  #define MIN_Z_OFFSET -9.99
#else
  #define MIN_Z_OFFSET -1
#endif

#define MAX_E_TEMP    (HEATER_0_MAXTEMP - (HOTEND_OVERSHOOT))
#define MIN_E_TEMP    HEATER_0_MINTEMP

#define MAX_BED_TEMP  BED_MAXTEMP
#define MIN_BED_TEMP  BED_MINTEMP

constexpr uint16_t TROWS = 6, MROWS = TROWS - 1,
                   TITLE_HEIGHT = 30,
                   MLINE = 53,
                   LBLX = 60,
                   MENU_CHR_W = 8, STAT_CHR_W = 10;

#define MBASE(L) (49 + MLINE * (L))

constexpr float default_max_feedrate[]        = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[]    = DEFAULT_MAX_ACCELERATION;
constexpr float default_max_jerk[]            = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
constexpr float default_steps[]               = DEFAULT_AXIS_STEPS_PER_UNIT;

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

int16_t pausetemp;
int16_t pausebed;

bool liveadjust = false;
bool bedonly = false;
float zoffsetvalue;
uint16_t flowrate = 100;
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
    DWIN_Draw_String(false, true, font8x16, Color_White, bColor, 196, MBASE(row), (char*)"-");
  }
  else {
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, bColor, 4-log10(minunit), log10(minunit), 202, MBASE(row), value * minunit);
    DWIN_Draw_String(false, true, font8x16, Color_White, bColor, 196, MBASE(row), (char*)" ");
  }
}

inline void Draw_Title(char* title) {
  DWIN_Draw_String(false, false, DWIN_FONT_HEAD, Color_White, Color_Bg_Blue, 14, 4, (char*)title);
}

inline void Draw_Menu_Item(uint8_t row, uint8_t icon/*=0*/, char * const label, bool more/*=false*/) {
  if (label) DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, LBLX, MBASE(row) - 1, (char*)label); // Draw Label
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
  } else {
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
  } else {
    char * const name = (char*)"Host Print";
    const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(name) * MENU_CHR_W) / 2;
    DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, npos, 60, name);
  }
}

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
  } else {
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

void Draw_Status_Area(const bool with_update) {

  DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, STATUS_Y, DWIN_WIDTH, DWIN_HEIGHT - 1);

  DWIN_ICON_Show(ICON, ICON_HotendTemp, 10, 383);
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 384, thermalManager.temp_hotend[0].celsius);
  DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 384, (char*)"/");
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.temp_hotend[0].target);

  DWIN_ICON_Show(ICON, ICON_BedTemp, 10, 416);
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 417, thermalManager.temp_bed.celsius);
  DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 417, (char*)"/");
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.temp_bed.target);

  DWIN_ICON_Show(ICON, ICON_Speed, 113, 383);
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 384, (char*)"%");

  DWIN_ICON_Show(ICON, ICON_StepE, 112, 417);
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 417, flowrate);
  DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 417, (char*)"%");

  DWIN_ICON_Show(ICON, ICON_FanSpeed, 187, 383);
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);

  DWIN_ICON_Show(ICON, ICON_Zoffset, 187, 416);
  if (zoffsetvalue < 0) {
    DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, -zoffsetvalue * 100);
    DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, (char*)"-");
  }
  else {
    DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, zoffsetvalue * 100);
    DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, 205, 419, (char*)" ");
  }

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
}

void Update_Status_Area() {
  static float hotend = 0;
  static float bed = 0;
  static int16_t hotendtarget = 0;
  static int16_t bedtarget = 0;
  static int16_t feedrate = 100;
  static int16_t flow = flowrate;
  static uint8_t fan = 0;
  static float offset = zoffsetvalue;
  static float x = current_position.x;
  static float y = current_position.y;
  static float z = current_position.z;
  if (thermalManager.temp_hotend[0].celsius != hotend) {
    hotend = thermalManager.temp_hotend[0].celsius;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 384, thermalManager.temp_hotend[0].celsius);
  }
  if (thermalManager.temp_bed.celsius != bed) {
    bed = thermalManager.temp_bed.celsius;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 417, thermalManager.temp_bed.celsius);
  }
  if (thermalManager.temp_hotend[0].target != hotendtarget) {
    hotendtarget = thermalManager.temp_hotend[0].target;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.temp_hotend[0].target);
  }
  if (thermalManager.temp_bed.target != bedtarget) {
    bedtarget = thermalManager.temp_bed.target;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.temp_bed.target);
  }
  if (feedrate_percentage != feedrate) {
    feedrate = feedrate_percentage;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  }
  if (flowrate != flow) {
    flow = flowrate;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 417, flowrate);
  }
  if (thermalManager.fan_speed[0] != fan) {
    fan = thermalManager.fan_speed[0];
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
  }
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
  }
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

/* Menu Item Config */

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
    case ZOffset:
      return (char*)"Z Offset";
    case Preheat:
      return (char*)"Preheat";
    case ChangeFilament:
      return (char*)"Change Filament";
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
    case MaxJerk:
      return (char*)"Max Jerk";
    case Steps:
      return (char*)"Steps/mm";
    case Advanced:
      return (char*)"Advanced Settings";
    case Info:
      return (char*)"Info";
    case ManualMesh:
      return (char*)"Mesh Bed Leveling";
    case Tune:
      return (char*)"Tune";
  }
  return (char*)"";
}

int Get_Menu_Size(uint8_t menu) {
  switch(menu) {
    case Prepare:
      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        return 8;
      #else
        return 7;
      #endif
    case Move:
      return 4;
    case ManualLevel:
      return 5;
    case ZOffset:
      return 6;
    case Preheat:
      return 5;
    case ChangeFilament:
      return 3;
    case Control:
      return 7;
    case TempMenu:
      return 7;
    case Preheat1:
      return 4;
    case Preheat2:
      return 4;
    case Preheat3:
      return 4;
    case Preheat4:
      return 4;
    case Motion:
      return 5;
    case MaxSpeed:
      return 4;
    case MaxAcceleration:
      return 4;
    case MaxJerk:
      return 4;
    case Steps:
      return 4;
    case Advanced:
      #if HAS_BED_PROBE
        return 2;
      #else
        return 0;
      #endif
    case Info:
      return 0;
    case ManualMesh:
      return 4;
    case Tune:
      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        return 9;
      #else
        return 8;
      #endif
  }
  return 0;
}

void Menu_Item_Handler(uint8_t menu, uint8_t item, bool draw/*=true*/) {
  uint8_t row = item - scrollpos;
  switch (menu) {
    case Prepare:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Main_Menu(1);
          }
          break;
        case 1: // Move
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Move", true);
          } else {
            Draw_Menu(Move);
          }
          break;
        case 2: // Disable Stepper
          if (draw) {
            Draw_Menu_Item(row, ICON_CloseMotor, (char*)"Disable Stepper");
          } else {
            queue.inject_P(PSTR("M84"));
          }
          break;
        case 3: // Home
          if (draw) {
            Draw_Menu_Item(row, ICON_SetHome, (char*)"Auto Home");
          } else {
            Popup_Window_Home();
            gcode.process_subcommands_now_P( PSTR("G28"));
            planner.synchronize();
            Draw_Menu(Prepare);
          }
          break;
        case 4: // Manual Leveling
          if (draw) {
            Draw_Menu_Item(row, ICON_PrintSize, (char*)"Manual Leveling", true);
          } else {
            Popup_Window_Home();
            gcode.process_subcommands_now_P( PSTR("G28"));
            planner.synchronize();
            Draw_Menu(ManualLevel);
          }
          break;
        case 5: // Z Offset
          if (draw) {
            Draw_Menu_Item(row, ICON_Zoffset, (char*)"Z-Offset", true);
          } else {
            Draw_Menu(ZOffset);
          }
          break;
        case 6: // Preheat
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)"Preheat", true);
          } else {
            Draw_Menu(Preheat);
          }
          break;
        case 7: // Cooldown
          if (draw) {
            Draw_Menu_Item(row, ICON_Cool, (char*)"Cooldown", true);
          } else {
            thermalManager.zero_fan_speeds();
            thermalManager.disable_all_heaters();
          }
          break;
        #if ENABLED(ADVANCED_PAUSE_FEATURE)
          case 8: // Change Filament
            if (draw) {
              #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
                Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament", true);
              #else
                Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament");
              #endif
            } else {
              #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
                Draw_Menu(ChangeFilament);
              #else
                Popup_Window_ChangeFilament();
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
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Prepare, 1);
          }
          break;
        case 1: // X
          if (draw) {
            Draw_Menu_Item(row, ICON_MoveX, (char*)"Move X");
            Draw_Float(current_position.x, row, false);
          } else {
            Modify_Value(current_position.x, X_MIN_POS, X_MAX_POS, 10);
          }
          break;
        case 2: // Y
          if (draw) {
            Draw_Menu_Item(row, ICON_MoveY, (char*)"Move Y");
            Draw_Float(current_position.y, row);
          } else {
            Modify_Value(current_position.y, Y_MIN_POS, Y_MAX_POS, 10);
          }
          break;
        case 3: // Z
          if (draw) {
            Draw_Menu_Item(row, ICON_MoveZ, (char*)"Move Z");
            Draw_Float(current_position.z, row);
          } else {
            Modify_Value(current_position.z, Z_MIN_POS, Z_MAX_POS, 10);
          }
          break;
        case 4: // E
          if (draw) {
            Draw_Menu_Item(row, ICON_Extruder, (char*)"Extruder");
            Draw_Float(current_position.e, item);
          } else {
            if (thermalManager.temp_hotend[0].celsius < EXTRUDE_MINTEMP) {
              Popup_Window_ETempTooLow();
            } else {
              Modify_Value(current_position.e, -500, 500, 10);
            }
          }
          break;
      }
      break;
    case ManualLevel:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Prepare, 4);
          }
          break;
        case 1: // Bottom Left
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Bottom Left");
          } else {
            Popup_Window_Move();
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X30 Y30\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 1);
          }
          break;
        case 2: // Top Left
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Top Left");
          } else {
            Popup_Window_Move();
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X30 Y205\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 2);
          }
          break;
        case 3: // Top Right
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Top Right");
          } else {
            Popup_Window_Move();
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X205 Y205\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 3);
          }
          break;
        case 4: // Bottom Right
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Bottom Right");
          } else {
            Popup_Window_Move();
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X205 Y30\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 4);
          }
          break;
        case 5: // Center
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Center");
          } else {
            Popup_Window_Move();
            gcode.process_subcommands_now_P(PSTR("G1 F4000\nG1 Z10\nG1 X117.5 Y117.5\nG1 F300 Z0\nM220 S100"));
            planner.synchronize();
            Draw_Menu(ManualLevel, 5);
          }
          break;
      }
      break;
    case ZOffset:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            liveadjust = false;
            Draw_Menu(Prepare, 5);
          }
          break;
        case 1: // Home Z
          if (draw) {
            Draw_Menu_Item(row, ICON_Homing, (char*)"Home Z Axis");
          } else {
            Popup_Window_Home();
            gcode.process_subcommands_now_P( PSTR("G28 Z\nG1 Z0"));
            planner.synchronize();
            Draw_Menu(ZOffset);
          }
          break;
        case 2: // Live Adjust
          if (draw) {
            if (liveadjust)
              Draw_Menu_Item(row, ICON_Zoffset, (char*)"Live Adjust: Enabled");
            else
              Draw_Menu_Item(row, ICON_Zoffset, (char*)"Live Adjust: Disabled");
          } else {
            if (!liveadjust) {
              Popup_Window_Home();
              gcode.process_subcommands_now_P( PSTR("G28 Z O\nG1 Z0"));
              planner.synchronize();
              Draw_Menu(ZOffset, 2);
            }
            liveadjust = !liveadjust;
            Draw_Menu(ZOffset, 2);
          }
          break;
        case 3: // Offset
          if (draw) {
            Draw_Menu_Item(row, ICON_SetZOffset, (char*)"Z Offset");
            Draw_Float(zoffsetvalue, row, false, 100);
          } else {
            Modify_Value(zoffsetvalue, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
          }
          break;
        case 4: // Step Up
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Up");
          } else {
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
        case 5: // Step Down
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Down");
          } else {
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
        case 6: // Save
          if (draw) {
            Draw_Menu_Item(row, ICON_WriteEEPROM, (char*)"Save");
          } else {
            AudioFeedback(settings.save());
          }
          break;
      }
      break;
    case Preheat:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Prepare, 6);
          }
          break;
        case 1: // Mode
          if (draw) {
            if (bedonly)
              Draw_Menu_Item(row, ICON_Homing, (char*)"Preheat Mode: Bed");
            else
              Draw_Menu_Item(row, ICON_Homing, (char*)"Preheat Mode: Normal");
          } else {
            bedonly = !bedonly;
            Draw_Menu(Preheat, 1);
          }
          break;
        case 2: // Preheat 1
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_1_LABEL);
          } else {
            if (!bedonly) {
              thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0);
              thermalManager.set_fan_speed(0, ui.material_preset[0].fan_speed);
            }
            thermalManager.setTargetBed(ui.material_preset[0].bed_temp);
          }
          break;
        case 3: // Preheat 2
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_2_LABEL);
          } else {
            if (!bedonly) {
              thermalManager.setTargetHotend(ui.material_preset[1].hotend_temp, 0);
              thermalManager.set_fan_speed(0, ui.material_preset[1].fan_speed);
            }
            thermalManager.setTargetBed(ui.material_preset[1].bed_temp);
          }
          break;
        case 4: // Preheat 3
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_3_LABEL);
          } else {
            if (!bedonly) {
              thermalManager.setTargetHotend(ui.material_preset[2].hotend_temp, 0);
              thermalManager.set_fan_speed(0, ui.material_preset[2].fan_speed);
            }
            thermalManager.setTargetBed(ui.material_preset[2].bed_temp);
          }
          break;
        case 5: // Preheat 4
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_4_LABEL);
          } else {
            if (!bedonly) {
              thermalManager.setTargetHotend(ui.material_preset[3].hotend_temp, 0);
              thermalManager.set_fan_speed(0, ui.material_preset[3].fan_speed);
            }
            thermalManager.setTargetBed(ui.material_preset[3].bed_temp);
          }
          break;
      }
      break;
    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      case ChangeFilament:
        switch (item) {
          case 0: // Back
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Back");
            } else {
              Draw_Menu(Prepare, 8);
            }
            break;
          case 1: // Load Filament
            if (draw) {
              Draw_Menu_Item(row, ICON_WriteEEPROM, (char*)"Load Filament");
            } else {
              Popup_Window_LoadFilament();
              gcode.process_subcommands_now_P(PSTR("M701"));
              planner.synchronize();
              Draw_Menu(Prepare, 8);
            }
            break;
          case 2: // Unload Filament
            if (draw) {
              Draw_Menu_Item(row, ICON_ReadEEPROM, (char*)"Unload Filament");
            } else {
              Popup_Window_LoadFilament(true);
              gcode.process_subcommands_now_P(PSTR("M702"));
              planner.synchronize();
              Draw_Menu(Prepare, 8);
            }
            break;
          case 3: // Change Filament
            if (draw) {
              Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament");
            } else {
              Popup_Window_ChangeFilament();
              gcode.process_subcommands_now_P(PSTR("M600 B1"));
              planner.synchronize();
              Draw_Menu(Prepare, 8);
            }
            break;
        }
        break;
    #endif
    case Control:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Main_Menu(2);
          }
          break;
        case 1: // Temperature
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)"Temperature");
          } else {
            Draw_Menu(TempMenu);
          }
          break;
        case 2: // Motion
          if (draw) {
            Draw_Menu_Item(row, ICON_Motion, (char*)"Motion");
          } else {
            Draw_Menu(Motion);
          }
          break;
        case 3: // Motion
          if (draw) {
            Draw_Menu_Item(row, ICON_Version, (char*)"Advanced");
          } else {
            Draw_Menu(Advanced);
          }
          break;
        case 4: // Store Settings
          if (draw) {
            Draw_Menu_Item(row, ICON_WriteEEPROM, (char*)"Store Settings");
          } else {
            AudioFeedback(settings.save());
          }
          break;
        case 5: // Restore Settings
          if (draw) {
            Draw_Menu_Item(row, ICON_ReadEEPROM, (char*)"Restore Setting");
          } else {
            AudioFeedback(settings.load());
          }
          break;
        case 6: // Reset Settings
          if (draw) {
            Draw_Menu_Item(row, ICON_Temperature, (char*)"Reset Defaults");
          } else {
            settings.reset();
            AudioFeedback();
          }
          break;
        case 7: // Info
          if (draw) {
            Draw_Menu_Item(row, ICON_Info, (char*)"Info");
          } else {
            Draw_Menu(Info);
          }
          break;
      }
      break;
    case TempMenu:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Control, 1);
          }
          break;
        case 1: // Hotend
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(thermalManager.temp_hotend[0].target, row, false, 1);
          } else {
            Modify_Value(thermalManager.temp_hotend[0].target, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        case 2: // Bed
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(thermalManager.temp_bed.target, row, false, 1);
          } else {
            Modify_Value(thermalManager.temp_bed.target, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        case 3: // Fan
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(thermalManager.fan_speed[0], row, false, 1);
          } else {
            Modify_Value(thermalManager.fan_speed[0], MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
        case 4: // Preheat 1
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)PREHEAT_1_LABEL);
          } else {
            Draw_Menu(Preheat1);
          }
          break;
        case 5: // Preheat 2
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)PREHEAT_2_LABEL);
          } else {
            Draw_Menu(Preheat2);
          }
          break;
        case 6: // Preheat 3
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)PREHEAT_3_LABEL);
          } else {
            Draw_Menu(Preheat3);
          }
          break;
        case 7: // Preheat 4
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)PREHEAT_4_LABEL);
          } else {
            Draw_Menu(Preheat4);
          }
          break;
      }
      break;
    case Preheat1:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(TempMenu, 4);
          }
          break;
        case 1: // Hotend
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(ui.material_preset[0].hotend_temp, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[0].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        case 2: // Bed
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(ui.material_preset[0].bed_temp, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[0].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        case 3: // Fan
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(ui.material_preset[0].fan_speed, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[0].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
      }
      break;
    case Preheat2:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(TempMenu, 5);
          }
          break;
        case 1: // Hotend
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(ui.material_preset[1].hotend_temp, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[1].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        case 2: // Bed
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(ui.material_preset[1].bed_temp, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[1].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        case 3: // Fan
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(ui.material_preset[1].fan_speed, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[1].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
      }
      break;
    case Preheat3:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(TempMenu, 6);
          }
          break;
        case 1: // Hotend
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(ui.material_preset[2].hotend_temp, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[2].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        case 2: // Bed
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(ui.material_preset[2].bed_temp, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[2].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        case 3: // Fan
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(ui.material_preset[2].fan_speed, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[2].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
      }
      break;
    case Preheat4:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(TempMenu, 7);
          }
          break;
        case 1: // Hotend
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(ui.material_preset[3].hotend_temp, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[3].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        case 2: // Bed
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(ui.material_preset[3].bed_temp, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[3].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        case 3: // Fan
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(ui.material_preset[3].fan_speed, row, false, 1);
          } else {
            Modify_Value(ui.material_preset[3].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
      }
      break;
    case Motion:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Control, 2);
          }
          break;
        case 1: // Speed
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, (char*)"Max Speed");
          } else {
            Draw_Menu(MaxSpeed);
          }
          break;
        case 2: // Acceleration
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccelerated, (char*)"Max Acceleration");
          } else {
            Draw_Menu(MaxAcceleration);
          }
          break;
        case 3: // Jerk
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxJerk, (char*)"Max Jerk");
          } else {
            Draw_Menu(MaxJerk);
          }
          break;
        case 4: // Steps
          if (draw) {
            Draw_Menu_Item(row, ICON_Step, (char*)"Steps/mm");
          } else {
            Draw_Menu(Steps);
          }
          break;
        case 5: // Flow
          if (draw) {
            Draw_Menu_Item(row, ICON_Speed, (char*)"Flow Rate");
            Draw_Float(flowrate, row, false, 1);
          } else {
            Modify_Value(flowrate, MIN_FLOW_RATE, MAX_FLOW_RATE, 1);
          }
          break;
      }
      break;
    case MaxSpeed:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Motion, 1);
          }
          break;
        case 1: // X
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedX, (char*)"X Axis");
            Draw_Float(planner.settings.max_feedrate_mm_s[X_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.settings.max_feedrate_mm_s[X_AXIS], 0, default_max_feedrate[X_AXIS]*2, 1);
          }
          break;
        case 2: // Y
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedY, (char*)"Y Axis");
            Draw_Float(planner.settings.max_feedrate_mm_s[Y_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.settings.max_feedrate_mm_s[Y_AXIS], 0, default_max_feedrate[Y_AXIS]*2, 1);
          }
          break;
        case 3: // Z
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedZ, (char*)"Z Axis");
            Draw_Float(planner.settings.max_feedrate_mm_s[Z_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.settings.max_feedrate_mm_s[Z_AXIS], 0, default_max_feedrate[Z_AXIS]*2, 1);
          }
          break;
        case 4: // E
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedE, (char*)"Extruder");
            Draw_Float(planner.settings.max_feedrate_mm_s[E_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.settings.max_feedrate_mm_s[Z_AXIS], 0, default_max_feedrate[E_AXIS]*2, 1);
          }
          break;
      }
      break;
    case MaxAcceleration:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Motion, 2);
          }
          break;
        case 1: // X
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccX, (char*)"X Axis");
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[X_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[X_AXIS], 0, default_max_acceleration[X_AXIS]*2, 1);
          }
          break;
        case 2: // Y
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccY, (char*)"Y Axis");
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], 0, default_max_acceleration[Y_AXIS]*2, 1);
          }
          break;
        case 3: // Z
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccZ, (char*)"Z Axis");
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], 0, default_max_acceleration[Z_AXIS]*2, 1);
          }
          break;
        case 4: // E
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccE, (char*)"Extruder");
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[E_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[E_AXIS], 0, default_max_acceleration[E_AXIS]*2, 1);
          }
          break;
      }
      break;
    case MaxJerk:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Motion, 3);
          }
          break;
        case 1: // X
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedJerkX, (char*)"X Axis");
            Draw_Float(planner.max_jerk[X_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.max_jerk[X_AXIS], 0, default_max_jerk[X_AXIS]*2, 1);
          }
          break;
        case 2: // Y
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedJerkY, (char*)"Y Axis");
            Draw_Float(planner.max_jerk[Y_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.max_jerk[Y_AXIS], 0, default_max_jerk[Y_AXIS]*2, 1);
          }
          break;
        case 3: // Z
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedJerkZ, (char*)"Z Axis");
            Draw_Float(planner.max_jerk[Z_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.max_jerk[Z_AXIS], 0, default_max_jerk[Z_AXIS]*2, 1);
          }
          break;
        case 4: // E
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedJerkE, (char*)"Extruder");
            Draw_Float(planner.max_jerk[E_AXIS], row, false, 1);
          } else {
            Modify_Value(planner.max_jerk[E_AXIS], 0, default_max_jerk[E_AXIS]*2, 1);
          }
          break;
      }
      break;
    case Steps:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Motion, 4);
          }
          break;
        case 1: // X
          if (draw) {
            Draw_Menu_Item(row, ICON_StepX, (char*)"X Axis");
            Draw_Float(planner.settings.axis_steps_per_mm[X_AXIS], row, false, 10);
          } else {
            Modify_Value(planner.settings.axis_steps_per_mm[X_AXIS], 0, default_steps[X_AXIS]*2, 10);
          }
          break;
        case 2: // Y
          if (draw) {
            Draw_Menu_Item(row, ICON_StepY, (char*)"Y Axis");
            Draw_Float(planner.settings.axis_steps_per_mm[Y_AXIS], row, false, 10);
          } else {
            Modify_Value(planner.settings.axis_steps_per_mm[Y_AXIS], 0, default_steps[Y_AXIS]*2, 10);
          }
          break;
        case 3: // Z
          if (draw) {
            Draw_Menu_Item(row, ICON_StepZ, (char*)"Z Axis");
            Draw_Float(planner.settings.axis_steps_per_mm[Z_AXIS], row, false, 10);
          } else {
            Modify_Value(planner.settings.axis_steps_per_mm[Z_AXIS], 0, default_steps[Z_AXIS]*2, 10);
          }
          break;
        case 4: // E
          if (draw) {
            Draw_Menu_Item(row, ICON_StepE, (char*)"Extruder");
            Draw_Float(planner.settings.axis_steps_per_mm[E_AXIS], row, false, 10);
          } else {
            Modify_Value(planner.settings.axis_steps_per_mm[E_AXIS], 0, default_steps[E_AXIS]*2, 10);
          }
          break;
      }
      break;
    case Advanced:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Control, 3);
          }
          break;
        #if HAS_BED_PROBE
          case 1: // Probe X Offset
            if (draw) {
              Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Probe X Offset");
              Draw_Float(probe.offset.x, row, false, 10);
            } else {
              Modify_Value(probe.offset.x, -50, 50, 10);
            }
            break;
          case 2: // Probe Y Offset
            if (draw) {
              Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Probe Y Offset");
              Draw_Float(probe.offset.y, row, false, 10);
            } else {
              Modify_Value(probe.offset.y, -50, 50, 10);
            }
            break;
        #endif
      }
      break;
    case Info:
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Menu(Control, 7);
          }
          break;
        case 1:
          if (draw) {
            Draw_Menu_Item(row, ICON_PrintSize, (char*)MACHINE_SIZE);
          }
          break;
        case 2:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepY, (char*)SHORT_BUILD_VERSION);
          }
          break;
        case 3:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepZ, (char*)CORP_WEBSITE_E);
          }
          break;
      }
      break;
    #if ENABLED(PROBE_MANUALLY)
      case ManualMesh:
        switch (item) {
          case 0: // Back
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Cancel");
            } else {
              gcode.process_subcommands_now_P(PSTR("G29 A"));
              planner.synchronize();
              Draw_Main_Menu(3);
            }
            break;
          case 1: // Next Point
            if (draw) {
              if (gridpoint < GRID_MAX_POINTS)
                Draw_Menu_Item(row, ICON_More, (char*)"Next Point");
              else
                Draw_Menu_Item(row, ICON_More, (char*)"Save Mesh");
            } else {
              if (gridpoint < GRID_MAX_POINTS) {
                Popup_Window_Move();
                gcode.process_subcommands_now_P(PSTR("G29"));
                planner.synchronize();
                gridpoint++;
                Draw_Menu(ManualMesh, 1);
              } else {
                gcode.process_subcommands_now_P(PSTR("G29"));
                planner.synchronize();
                AudioFeedback(settings.save());
                Draw_Main_Menu(3);
              }
            }
            break;
          case 2: // Offset
            if (draw) {
              Draw_Menu_Item(row, ICON_SetZOffset, (char*)"Z Position");
              current_position.z = MANUAL_PROBE_START_Z;
              Draw_Float(current_position.z, row, false, 100);
            } else {
              Modify_Value(current_position.z, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
            }
            break;
          case 3: // Step Up
            if (draw) {
              Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Up");
            } else {
              if (current_position.z < MAX_Z_OFFSET) {
                gcode.process_subcommands_now_P(PSTR("M290 Z0.01"));
                planner.synchronize();
                current_position.z += 0.01f;
                sync_plan_position();
                Draw_Float(current_position.z, row-1, false, 100);
              }
            }
            break;
          case 4: // Step Down
            if (draw) {
              Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Down");
            } else {
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
      switch (item) {
        case 0: // Back
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          } else {
            Draw_Print_Screen();
          }
          break;
        case 1: // Speed
          if (draw) {
            Draw_Menu_Item(row, ICON_Speed, (char*)"Print Speed");
            Draw_Float(feedrate_percentage, row, false, 1);
          } else {
            Modify_Value(feedrate_percentage, MIN_PRINT_SPEED, MAX_PRINT_SPEED, 1);
          }
          break;
        case 2: // Flow
          if (draw) {
            Draw_Menu_Item(row, ICON_Speed, (char*)"Flow Rate");
            Draw_Float(flowrate, row, false, 1);
          } else {
            Modify_Value(flowrate, MIN_FLOW_RATE, MAX_FLOW_RATE, 1);
          }
          break;
        case 3: // Hotend
          if (draw) {
            Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
            Draw_Float(thermalManager.temp_hotend[0].target, row, false, 1);
          } else {
            Modify_Value(thermalManager.temp_hotend[0].target, MIN_E_TEMP, MAX_E_TEMP, 1);
          }
          break;
        case 4: // Bed
          if (draw) {
            Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
            Draw_Float(thermalManager.temp_bed.target, row, false, 1);
          } else {
            Modify_Value(thermalManager.temp_bed.target, MIN_BED_TEMP, MAX_BED_TEMP, 1);
          }
          break;
        case 5: // Fan
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
            Draw_Float(thermalManager.fan_speed[0], row, false, 1);
          } else {
            Modify_Value(thermalManager.fan_speed[0], MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
          }
          break;
        case 6: // Z Offset
          if (draw) {
            Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Z-Offset");
            // TODO this does not update when using Up/Down below
            Draw_Float(zoffsetvalue, row, false, 100);
          } else {
            Modify_Value(zoffsetvalue, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
          }
          break;
        case 7: // Z Offset Up
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Z-Offset Up");
          } else {
            if (zoffsetvalue < MAX_Z_OFFSET) {
              gcode.process_subcommands_now_P(PSTR("M290 Z0.01"));
              zoffsetvalue += 0.01;
              Draw_Float(zoffsetvalue, row-1, false, 100);
            }
          }
          break;
        case 8: // Z Offset Down
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Z-Offset Down");
          } else {
            if (zoffsetvalue > MIN_Z_OFFSET) {
              gcode.process_subcommands_now_P(PSTR("M290 Z-0.01"));
              zoffsetvalue -= 0.01;
              Draw_Float(zoffsetvalue, row-2, false, 100);
            }
          }
          break;
        #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
          case 9: // Change Filament
            if (draw) {
              Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament");
            } else {
              Popup_Window_ChangeFilament();
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

/* Popup Functions */

void Popup_Select() {
  const uint16_t c1 = (selection==0) ? Select_Color : Color_Bg_Window,
                 c2 = (selection==0) ? Color_Bg_Window : Select_Color;
  DWIN_Draw_Rectangle(0, c1, 25, 279, 126, 318);
  DWIN_Draw_Rectangle(0, c1, 24, 278, 127, 319);
  DWIN_Draw_Rectangle(0, c2, 145, 279, 246, 318);
  DWIN_Draw_Rectangle(0, c2, 144, 278, 247, 319);
}

void DWIN_Popup_Temperature(const bool toohigh) {
  Clear_Screen(4);
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 105, 258, 374);
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

void Popup_Window_ETempTooLow() {
  selection = 0;
  process = Popup;
  popup = ETemp;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
  DWIN_ICON_Show(ICON, ICON_TempTooLow, 102, 105);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 18) / 2, 192, (char*)"Nozzle is too cold");
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 15) / 2, 212, (char*)"Preheat to 200C?");
  DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
  DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
  Popup_Select();
}

void Popup_Window_Resume() {
  selection = 0;
  process = Popup;
  popup = Resume;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 14) / 2, 115, (char*)"Continue Print?");
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 22) / 2, 192, (char*)"It looks like the last");
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 22) / 2, 212, (char*)"file was interrupted.");
  DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
  DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
  Popup_Select();
}

void Popup_Window_Home(const bool parking/*=false*/) {
  process = Wait;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
  DWIN_ICON_Show(ICON, ICON_BLTouch, 101, 105);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * (parking ? 7 : 6)) / 2, 230, parking ? (char*)"Parking" : (char*)"Homing");
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 23) / 2, 260, (char*)"Please wait until done.");
}

void Popup_Window_Leveling() {
  process = Wait;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
  DWIN_ICON_Show(ICON, ICON_AutoLeveling, 101, 105);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 17) / 2, 230, (char*)"Auto Bed Leveling");
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 23) / 2, 260, (char*)"Please wait until done.");
}

void Popup_Window_Move() {
  process = Wait;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
  DWIN_ICON_Show(ICON, ICON_BLTouch, 101, 105);
  DWIN_Draw_String(false,true,font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 15) / 2, 230, (char*)"Moving to Point");
  DWIN_Draw_String(false,true,font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 23) / 2, 260, (char*)"Please wait until done.");
}

void Popup_window_Pause() {
  selection = 0;
  process = Popup;
  popup = Pause;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 11) / 2, 150, GET_TEXT_F(MSG_PAUSE_PRINT));
  DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
  DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
  Popup_Select();
}

void Popup_window_Stop() {
  selection = 0;
  process = Popup;
  popup = Stop;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 10) / 2, 150, GET_TEXT_F(MSG_STOP_PRINT));
  DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
  DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
  Popup_Select();
}

void Popup_window_SaveLevel() {
  selection = 0;
  process = Popup;
  popup = Level;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 20) / 2, 150, (char*)"Save Bed Level Mesh?");
  DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
  DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
  Popup_Select();
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void Popup_Window_ChangeFilament() {
    process = Wait;
    Clear_Screen();
    DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
    DWIN_ICON_Show(ICON, ICON_BLTouch, 101, 105);
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 15) / 2, 230, (char*)"Filament Change");
    DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 26) / 2, 260, (char*)"Please wait while heating.");
  }

  #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    void Popup_Window_LoadFilament(const bool unloading/*=false*/) {
      process = Wait;
      Clear_Screen();
      DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
      DWIN_ICON_Show(ICON, ICON_BLTouch, 101, 105);
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * (unloading ? 18 : 16)) / 2, 230, unloading ? (char*)"Unloading Filament" : (char*)"Loading Filament");
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 23) / 2, 260, (char*)"Please wait until done.");
    }
  #endif
#endif

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
          Popup_Window_Leveling();
          gcode.process_subcommands_now_P(PSTR("G28\nG29"));
          planner.synchronize();
          Popup_window_SaveLevel();
        #elif ENABLED(PROBE_MANUALLY)
          gridpoint = 1;
          Popup_Window_Home();
          gcode.process_subcommands_now_P(PSTR("G28"));
          planner.synchronize();
          Popup_Window_Move();
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
        } else {
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
      } else {
        card.cdup();
        Draw_SD_List();
      }
    } else {
      card.getfilename_sorted(SD_ORDER(selection-1, card.get_num_Files()));
      if (card.flag.filenameIsDir) {
        card.cd(card.filename);
        Draw_SD_List();
      } else {
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
          char cmnd[20];
          cmnd[sprintf(cmnd, "M140 S%i", pausebed)] = '\0';
          gcode.process_subcommands_now_P(PSTR(cmnd));
          cmnd[sprintf(cmnd, "M109 S%i", pausetemp)] = '\0';
          gcode.process_subcommands_now_P(PSTR(cmnd));
          gcode.process_subcommands_now_P(PSTR("M24"));
        }
        else
          Popup_window_Pause();
        break;
      case 2:
        Popup_window_Stop();
        break;
    }
  DWIN_UpdateLCD();
}

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
            #if ENABLED(POWER_LOSS_RECOVERY)
              if (recovery.enabled) recovery.save(true);
            #endif
            gcode.process_subcommands_now_P(PSTR("M25"));
            pausetemp = thermalManager.temp_hotend[0].target;
            pausebed = thermalManager.temp_bed.target;
            thermalManager.zero_fan_speeds();
            thermalManager.disable_all_heaters();
            Draw_Print_Screen();
          } else {
            // TODO: Local Pause During Host Print
          }
        } else {
          Draw_Print_Screen();
        }
        break;
      case Stop:
        if (selection==0) {
          if (sdprint) {
          card.flag.abort_sd_printing = true; 
          thermalManager.zero_fan_speeds();
          thermalManager.disable_all_heaters();
          Draw_Main_Menu();
          } else {
            // TODO: Local Stop During Host Print
          }
        } else {
          Draw_Print_Screen();
        }
        break;
      case Resume:
        if (selection==0) {
          queue.inject_P(PSTR("M1000"));
          Draw_Print_Screen();
        } else {
          Draw_Print_Screen();
        }
        break;
      case ETemp:
        if (selection==0) {
          thermalManager.temp_hotend[0].target = 200;
          Draw_Menu(Move, 4);
        } else {
          Draw_Menu(Move, 4);
        }
        break;
      case Level:
        if (selection==0) {
          AudioFeedback(settings.save());
          Draw_Main_Menu();
        } else {
          Draw_Main_Menu();
        }
        break;
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

/* Host Control */

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
  } else {
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 352, DWIN_WIDTH-8, 376);
    const int8_t npos = _MAX(0U, DWIN_WIDTH - strlen(lastmsg) * MENU_CHR_W) / 2;
    DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, npos, 357, lastmsg);
  }
}

/* Main Functions */

void Start_Print(bool sd) {
  if (card.isPrinting())
    sdprint = true;
  else
    sdprint = sd;
  printpercent = 0;
  remainingtime = 0;
  elapsedtime = 0;
  dwin_heat_time = 0;
  lastmsg[0] = '\0';
  Draw_Print_Screen();
}

void Stop_Print() {
  thermalManager.zero_fan_speeds();
  thermalManager.disable_all_heaters();
  if (process == Print) {
    Draw_Print_confirm();
  } else {
    Draw_Print_Screen();
    Draw_Print_confirm();
  }
}

void DWIN_Update() {
  Screen_Update();
  Variable_Update();
}

void Variable_Update() {
  static float lastzoffset = zoffsetvalue;
  static float lastflow = flowrate;
  if (zoffsetvalue != lastzoffset) {
    lastzoffset = zoffsetvalue;
    #if HAS_BED_PROBE
      probe.offset.z = zoffsetvalue;
    #else
      set_home_offset(Z_AXIS, -zoffsetvalue);
    #endif
  }
  if (flowrate != lastflow) {
    lastflow = flowrate;
    planner.set_flow(0, flowrate);
  }

  #if HAS_BED_PROBE
    if (probe.offset.z != lastzoffset) {
      zoffsetvalue = lastzoffset = probe.offset.z;
    }
  #else
    if (-home_offset.z != lastzoffset) {
      zoffsetvalue = lastzoffset = -home_offset.z;
    }
  #endif
  if (planner.flow_percentage[0] != lastflow) {
    flowrate = lastflow = planner.flow_percentage[0];
  }
}

void Screen_Update() {
  static millis_t statustime = 0;
  if (ELAPSED(millis(), statustime)) {
    statustime = millis() + 500;
    Update_Status_Area();
  }

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
      }
    }
    if (process == Print && card.isPaused() && !paused) {
      paused = true;
      Print_Screen_Icons();
    }
    else if (process == Print && !card.isPaused() && paused) {
      paused = false;
      Print_Screen_Icons();
    }
  }

  static bool mounted = card.isMounted();
  if (mounted != card.isMounted()) {
    mounted = card.isMounted();
    if (process == File)
      Draw_SD_List();
  }

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
  }

  DWIN_JPG_CacheTo1(Language_English);

  #if HAS_BED_PROBE
    zoffsetvalue = probe.offset.z;
  #else
    zoffsetvalue = -home_offset.z;
  #endif

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
    if (message == PAUSE_MESSAGE_INSERT || message == PAUSE_MESSAGE_WAITING) {
      process = Confirm;
      popup = M600;
      Clear_Screen();

      DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 350);
      DWIN_ICON_Show(ICON, ICON_BLTouch, 101, 105);
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * (message == PAUSE_MESSAGE_INSERT ? 15 : 6)) / 2, 230, message == PAUSE_MESSAGE_INSERT ? (char*)"Filament Change" : (char*)"Paused");
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * (message == PAUSE_MESSAGE_INSERT ? 15 : 21 )) / 2, 260, message == PAUSE_MESSAGE_INSERT ? (char*)"Insert Filament" : (char*)"Press to resume print");
      DWIN_ICON_Show(ICON, ICON_Continue_E, 87, 283);
    }
  }
#endif

#endif
