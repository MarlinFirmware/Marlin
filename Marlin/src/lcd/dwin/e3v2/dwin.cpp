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
 * Enhanced implementation by Miguel A. Risco-Castillo
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD)

#include "dwin.h"

#if ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT) && DISABLED(PROBE_MANUALLY)
  #define HAS_ONESTEP_LEVELING 1
#endif

#if ANY(BABYSTEPPING, HAS_BED_PROBE, HAS_WORKSPACE_OFFSET)
  #define HAS_ZOFFSET_ITEM 1
#endif

#if !HAS_BED_PROBE && ENABLED(BABYSTEPPING)
  #define JUST_BABYSTEP 1
#endif

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

#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"

#if HAS_FILAMENT_SENSOR
  #include "../../../feature/runout.h"
#endif

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

#ifndef MACHINE_SIZE
  #define MACHINE_SIZE STRINGIFY(X_BED_SIZE) "x" STRINGIFY(Y_BED_SIZE) "x" STRINGIFY(Z_MAX_POS)
#endif
#ifndef CORP_WEBSITE
  #define CORP_WEBSITE WEBSITE_URL
#endif

#define PAUSE_HEAT

#define USE_STRING_HEADINGS
//#define USE_STRING_TITLES

#define MENU_CHAR_LIMIT  24
#define STATUS_Y 354

// Fan speed limit
#define FANON           255
#define FANOFF          0

// Print speed limit
#define MIN_PRINT_SPEED  10
#define MAX_PRINT_SPEED  999

// Print flow limit
#define MIN_PRINT_FLOW   10
#define MAX_PRINT_FLOW   299

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
                   MENU_CHR_W = 8, STAT_CHR_W = 10;

#define MBASE(L) (49 + MLINE * (L))

#define BABY_Z_VAR TERN(HAS_BED_PROBE, probe.offset.z, dwin_zoffset)

/* Value Init */
HMI_value_t HMI_ValueStruct;
HMI_Flag_t HMI_flag{0};

millis_t dwin_heat_time = 0;

uint8_t checkkey = 0;
uint8_t last_checkkey = 0;

typedef struct {
  uint8_t now, last;
  void set(uint8_t v) { now = last = v; }
  void reset() { set(0); }
  bool changed() { bool c = (now != last); if (c) last = now; return c; }
  bool dec() { if (now) now--; return changed(); }
  bool inc(uint8_t v) { if (now < (v - 1)) now++; else now = (v - 1); return changed(); }
} select_t;

select_t select_page{0}, select_file{0}, select_print{0}, select_prepare{0}
         , select_control{0}, select_temp{0}, select_motion{0}, select_tune{0}
         , select_advSet{0}, select_item{0}
         ;

uint8_t index_file     = MROWS,
        index_prepare  = MROWS,
        index_control  = MROWS,
        index_tune     = MROWS,
        index_advset   = MROWS,
        index_leveling = MROWS;

bool dwin_abort_flag = false; // Flag to reset feedrate, return to Home

constexpr float default_max_feedrate[]        = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[]    = DEFAULT_MAX_ACCELERATION;

#if HAS_CLASSIC_JERK
  constexpr float default_max_jerk[]          = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
#endif

static uint8_t _percentDone = 0;
static uint16_t _remain_time = 0;

// Aditional Aux Host Support
static bool sdprint = false;

#if ENABLED(PAUSE_HEAT)
  #if ENABLED(HAS_HOTEND)
    uint16_t resume_hotend_temp = 0;
  #endif
  #if ENABLED(HAS_HEATED_BED)
    uint16_t resume_bed_temp = 0;
  #endif
#endif

#if HAS_ZOFFSET_ITEM
  float dwin_zoffset = 0, last_zoffset = 0;
#endif

void ICON_Print() {
  if (select_page.now == 0) {
    DWIN_ICON_Show(ICON, ICON_Print_1, 17, 110);
    DWIN_Draw_Rectangle(0, Color_White, 17, 110, 126, 209);
    DWIN_Frame_AreaCopy(1, 1, 451, 31, 463, 57, 181);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Print_0, 17, 110);
    DWIN_Frame_AreaCopy(1, 1, 423, 31, 435, 57, 181);
  }
}

void ICON_Prepare() {
  if (select_page.now == 1) {
    DWIN_ICON_Show(ICON, ICON_Prepare_1, 145, 110);
    DWIN_Draw_Rectangle(0, Color_White, 145, 110, 254, 209);
    DWIN_Frame_AreaCopy(1, 33, 451, 82, 466, 175, 181);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Prepare_0, 145, 110);
    DWIN_Frame_AreaCopy(1, 33, 423, 82, 438, 175, 181);
  }
}

void ICON_Control() {
  if (select_page.now == 2) {
    DWIN_ICON_Show(ICON, ICON_Control_1, 17, 226);
    DWIN_Draw_Rectangle(0, Color_White, 17, 226, 126, 325);
    DWIN_Frame_AreaCopy(1, 85, 451, 132, 463, 48, 298);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Control_0, 17, 226);
    DWIN_Frame_AreaCopy(1, 85, 423, 132, 434, 48, 298);
  }
}

void ICON_StartInfo(bool show) {
  if (show) {
    DWIN_ICON_Show(ICON, ICON_Info_1, 145, 226);
    DWIN_Draw_Rectangle(0, Color_White, 145, 226, 254, 325);
    DWIN_Frame_AreaCopy(1, 132, 451, 159, 466, 186, 298);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Info_0, 145, 226);
    DWIN_Frame_AreaCopy(1, 132, 423, 159, 435, 186, 298);
  }
}

void ICON_Leveling(bool show) {
  if (show) {
    DWIN_ICON_Show(ICON, ICON_Leveling_1, 145, 226);
    DWIN_Draw_Rectangle(0, Color_White, 145, 226, 254, 325);
    DWIN_Frame_AreaCopy(1, 84, 437, 120,  449, 182, 298);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Leveling_0, 145, 226);
    DWIN_Frame_AreaCopy(1, 84, 465, 120, 478, 182, 298);
  }
}

void ICON_Tune() {
  if (select_print.now == 0) {
    DWIN_ICON_Show(ICON, ICON_Setup_1, 8, 242);
    DWIN_Draw_Rectangle(0, Color_White, 8, 242, 87, 341);
    DWIN_Frame_AreaCopy(1,   0, 466,  34, 476, 31, 315);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Setup_0, 8, 242);
    DWIN_Frame_AreaCopy(1,   0, 438,  32, 448, 31, 315);
  }
}

void ICON_Pause() {
  if (select_print.now == 1) {
    DWIN_ICON_Show(ICON, ICON_Pause_1, 96, 242);
    DWIN_Draw_Rectangle(0, Color_White, 96, 242, 175, 341);
    DWIN_Frame_AreaCopy(1, 177, 451, 216, 462, 116, 315);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Pause_0, 96, 242);
    DWIN_Frame_AreaCopy(1, 177, 423, 215, 433, 116, 315);
  }
}

void ICON_Continue() {
  if (select_print.now == 1) {
    DWIN_ICON_Show(ICON, ICON_Continue_1, 96, 242);
    DWIN_Draw_Rectangle(0, Color_White, 96, 242, 175, 341);
    DWIN_Frame_AreaCopy(1, 1, 452, 32, 464, 121, 315);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Continue_0, 96, 242);
    DWIN_Frame_AreaCopy(1, 1, 424, 31, 434, 121, 315);
  }
}

void ICON_Stop() {
  if (select_print.now == 2) {
    DWIN_ICON_Show(ICON, ICON_Stop_1, 184, 242);
    DWIN_Draw_Rectangle(0, Color_White, 184, 242, 263, 341);
    DWIN_Frame_AreaCopy(1, 218, 452, 249, 466, 209, 315);
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Stop_0, 184, 242);
    DWIN_Frame_AreaCopy(1, 218, 423, 247, 436, 209, 315);
  }
}

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
  DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 31, DWIN_WIDTH, STATUS_Y - 1);
}

void Clear_Main_Window() {
  Clear_Title_Bar();
  Clear_Menu_Area();
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
  }
}

inline uint16_t nr_sd_menu_items() {
  return card.get_num_Files() + !card.flag.workDirIsRoot;
}

void Draw_Menu_Icon(const uint8_t line, const uint8_t icon) {
  DWIN_ICON_Show(ICON, icon, 26, MBASE(line) - 3);
}

void Erase_Menu_Text(const uint8_t line) {
  DWIN_Draw_Rectangle(1, Color_Bg_Black, LBLX, MBASE(line) - 14, 271, MBASE(line) + 28);
}

void Draw_Menu_Item(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr, bool more=false) {
  if (label) DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, LBLX, MBASE(line) - 1, (char*)label);
  if (icon) Draw_Menu_Icon(line, icon);
  if (more) Draw_More_Icon(line); 
}  

void Draw_Menu_Line(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr, bool more=false) {
  Draw_Menu_Item(line, icon, label, more);
  DWIN_Draw_Line(Line_Color, 16, MBASE(line) + 33, 256, MBASE(line) + 34);
}

void Draw_Chkb_Line(const uint8_t line, bool mode){
  DWIN_Draw_Checkbox(Color_White,Color_Bg_Black,220,MBASE(line),mode) ; 
}

// The "Back" label is always on the first line
void Draw_Back_Label() {
  DWIN_Frame_AreaCopy(1, 226, 179, 256, 189, LBLX, MBASE(0));
}

// Draw "Back" line at the top
void Draw_Back_First(const bool is_sel=true) {
  Draw_Menu_Line(0, ICON_Back);
  Draw_Back_Label();
  if (is_sel) Draw_Menu_Cursor(0);
}

inline bool Apply_Encoder(const ENCODER_DiffState &encoder_diffState, auto &valref) {
  if (encoder_diffState == ENCODER_DIFF_CW)
    valref += EncoderRate.encoderMoveValue;
  else if (encoder_diffState == ENCODER_DIFF_CCW)
    valref -= EncoderRate.encoderMoveValue;
  return encoder_diffState == ENCODER_DIFF_ENTER;
}

//
// Draw Menus
//

#define MOTION_CASE_RATE   1
#define MOTION_CASE_ACCEL  2
#define MOTION_CASE_JERK   (MOTION_CASE_ACCEL + ENABLED(HAS_CLASSIC_JERK))
#define MOTION_CASE_STEPS  (MOTION_CASE_JERK + 1)
#define MOTION_CASE_FLOW   (MOTION_CASE_STEPS +1) 
#define MOTION_CASE_TOTAL  MOTION_CASE_FLOW

#define PREPARE_CASE_FMAN  1  // Filament management
#define PREPARE_CASE_MOVE  2
#define PREPARE_CASE_MLEV  3
#define PREPARE_CASE_DISA  4
#define PREPARE_CASE_HOME  5 
#define PREPARE_CASE_MMESH (PREPARE_CASE_HOME + ENABLED(MESH_BED_LEVELING))
#define PREPARE_CASE_ZOFF (PREPARE_CASE_MMESH + ENABLED(HAS_ZOFFSET_ITEM))
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
#define CONTROL_CASE_REBOOT (CONTROL_CASE_RESET + 1)
#define CONTROL_CASE_ADVSET (CONTROL_CASE_REBOOT + 1)
#define CONTROL_CASE_INFO  (CONTROL_CASE_ADVSET + 1)
#define CONTROL_CASE_TOTAL CONTROL_CASE_INFO

#define TUNE_CASE_SPEED 1
#define TUNE_CASE_TEMP (TUNE_CASE_SPEED + ENABLED(HAS_HOTEND))
#define TUNE_CASE_BED  (TUNE_CASE_TEMP + ENABLED(HAS_HEATED_BED))
#define TUNE_CASE_FAN  (TUNE_CASE_BED + ENABLED(HAS_FAN))
#define TUNE_CASE_ZOFF (TUNE_CASE_FAN + ENABLED(HAS_ZOFFSET_ITEM))
#define TUNE_CASE_FCHNG (TUNE_CASE_ZOFF + 1)
#define TUNE_CASE_TOTAL TUNE_CASE_FCHNG

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
#define ADVSET_CASE_HEPID     (ADVSET_CASE_PROBEOFF + 1)
#define ADVSET_CASE_BEDPID    (ADVSET_CASE_HEPID + 1)
#define ADVSET_CASE_RUNOUT    (ADVSET_CASE_BEDPID + ENABLED(HAS_FILAMENT_SENSOR))
#define ADVSET_CASE_PWRLOSSR  (ADVSET_CASE_RUNOUT + 1)   
#define ADVSET_CASE_TOTAL     ADVSET_CASE_PWRLOSSR

void DWIN_Draw_Label(const uint16_t y, char *string) {
  DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, LBLX, y, string);
}
void DWIN_Draw_Label(const uint16_t y, const __FlashStringHelper *title) {
  DWIN_Draw_Label(y, (char*)title);
}

void DWIN_Frame_TitleCopy(uint8_t id, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) { DWIN_Frame_AreaCopy(id, x1, y1, x2, y2, 14, 8); }

void Item_Prepare_Move(const uint8_t row) {
  DWIN_Frame_AreaCopy(1, 69, 61, 102, 71, LBLX, MBASE(row)); // "Move"
  Draw_Menu_Line(row, ICON_Axis);
  Draw_More_Icon(row);
}

void Item_Prepare_Disable(const uint8_t row) {
  #ifdef USE_STRING_TITLES
    DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_DISABLE_STEPPERS));
  #else
    DWIN_Frame_AreaCopy(1, 103, 59, 200, 74, LBLX, MBASE(row)); // "Disable Stepper"
  #endif
  Draw_Menu_Line(row, ICON_CloseMotor);
}

void Item_Prepare_Home(const uint8_t row) {
  #ifdef USE_STRING_TITLES
    DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_AUTO_HOME));
  #else
    DWIN_Frame_AreaCopy(1, 202, 61, 271, 71, LBLX, MBASE(row)); // "Auto Home"
  #endif
  Draw_Menu_Line(row, ICON_Homing);
}

void Item_Prepare_ManualLev(const uint8_t row) {
  DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_MANUAL_LEVELING));
  Draw_Menu_Line(row, ICON_SetEndTemp);
  Draw_More_Icon(row);
}

void Item_Prepare_FilMan(const uint8_t row) {
  DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_FILAMENT_MAN));
  Draw_Menu_Line(row, ICON_FilMan);
  Draw_More_Icon(row);
}

#if ENABLED(MESH_BED_LEVELING)
void Item_Prepare_ManualMesh(const uint8_t row) {
  DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_MANUAL_MESH));
  Draw_Menu_Line(row, ICON_PrintSize);
  Draw_More_Icon(row);
}  
#endif

#if HAS_ZOFFSET_ITEM
  void Item_Prepare_Offset(const uint8_t row) {
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
  Draw_Menu_Line(row, ICON_SetHome);
  }
#endif

#if HAS_HOTEND
  void Item_Prepare_PLA(const uint8_t row) {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(row), F("Preheat " PREHEAT_1_LABEL));
    #else
      DWIN_Frame_AreaCopy(1, 107, 76, 156, 86, LBLX, MBASE(row));       // "Preheat"
      DWIN_Frame_AreaCopy(1, 157, 76, 181, 86, LBLX + 52, MBASE(row));  // "PLA"
    #endif
    Draw_Menu_Line(row, ICON_PLAPreheat);
  }

  void Item_Prepare_ABS(const uint8_t row) {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(row), F("Preheat " PREHEAT_2_LABEL));
    #else
      DWIN_Frame_AreaCopy(1, 107, 76, 156, 86, LBLX, MBASE(row));       // "Preheat"
      DWIN_Frame_AreaCopy(1, 172, 76, 198, 86, LBLX + 52, MBASE(row));  // "ABS"
    #endif
    Draw_Menu_Line(row, ICON_ABSPreheat);
  }
#endif

#if HAS_PREHEAT
  void Item_Prepare_Cool(const uint8_t row) {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(MBASE(row), GET_TEXT_F(MSG_COOLDOWN));
    #else
      DWIN_Frame_AreaCopy(1, 200,  76, 264,  86, LBLX, MBASE(row));      // "Cooldown"
    #endif
    Draw_Menu_Line(row, ICON_Cool);
  }
#endif

void Item_Prepare_Lang(const uint8_t row) {
  #ifdef USE_STRING_TITLES
    DWIN_Draw_Label(MBASE(row), F("UI Language"));
  #else
    DWIN_Frame_AreaCopy(1, 0, 194, 121, 207, LBLX, MBASE(row)); // "Language selection"
  #endif
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, 226, MBASE(row), F("EN"));
  Draw_Menu_Icon(row, ICON_Language);
}

void Draw_Prepare_Menu() {
  Clear_Main_Window();

  const int16_t scroll = MROWS - index_prepare; // Scrolled-up lines
  #define PSCROL(L) (scroll + (L))
  #define PVISI(L)  WITHIN(PSCROL(L), 0, MROWS)

  #ifdef USE_STRING_HEADINGS
    Draw_Title(GET_TEXT_F(MSG_PREPARE));
  #else
    DWIN_Frame_TitleCopy(1, 178, 2, 229, 14); // "Prepare"
  #endif

  if (PVISI(0)) Draw_Back_First(select_prepare.now == 0);                         // < Back
  if (PVISI(PREPARE_CASE_FMAN)) Item_Prepare_FilMan(PSCROL(PREPARE_CASE_FMAN));   // Filament Management >
  if (PVISI(PREPARE_CASE_MOVE)) Item_Prepare_Move(PSCROL(PREPARE_CASE_MOVE));     // Move >
  if (PVISI(PREPARE_CASE_MLEV)) Item_Prepare_ManualLev(PSCROL(PREPARE_CASE_MLEV));// Manual Leveling >
  if (PVISI(PREPARE_CASE_DISA)) Item_Prepare_Disable(PSCROL(PREPARE_CASE_DISA));  // Disable Stepper
  if (PVISI(PREPARE_CASE_HOME)) Item_Prepare_Home(PSCROL(PREPARE_CASE_HOME));     // Auto Home
  #if ENABLED(MESH_BED_LEVELING)
    if (PVISI(PREPARE_CASE_MMESH)) Item_Prepare_ManualMesh(PSCROL(PREPARE_CASE_MMESH));// Manual Mesh >
  #endif
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

  if (select_prepare.now) Draw_Menu_Cursor(PSCROL(select_prepare.now));
}

void Item_Control_Info(const uint16_t line) {
  #ifdef USE_STRING_TITLES
    DWIN_Draw_Label(line, GET_TEXT_F(MSG_INFO_SCREEN));
  #else
    DWIN_Frame_AreaCopy(1, 0, 104, 24, 114, LBLX, line);
  #endif
}

void Draw_Control_Menu() {
  Clear_Main_Window();

  #if CONTROL_CASE_TOTAL >= 6
    const int16_t scroll = MROWS - index_control; // Scrolled-up lines
    #define CSCROL(L) (scroll + (L))
  #else
    #define CSCROL(L) (L)
  #endif
  
  #define CLINE(L)  MBASE(CSCROL(L))
  #define CVISI(L)  WITHIN(CSCROL(L), 0, MROWS)

  #ifdef USE_STRING_HEADINGS
    Draw_Title(GET_TEXT_F(MSG_CONTROL));
  #else
    DWIN_Frame_TitleCopy(1, 128, 2, 176, 12);                                         // "Control"
  #endif
  
  if (CVISI(0)) Draw_Back_First(select_control.now == 0);                         // < Back
  #ifdef USE_STRING_TITLES
    if (CVISI(CONTROL_CASE_TEMP)) DWIN_Draw_Label(CLINE(CONTROL_CASE_TEMP), GET_TEXT_F(MSG_TEMPERATURE));
    if (CVISI(CONTROL_CASE_MOVE)) DWIN_Draw_Label(CLINE(CONTROL_CASE_MOVE), GET_TEXT_F(MSG_MOTION));
    #if ENABLED(EEPROM_SETTINGS)
      if (CVISI(CONTROL_CASE_SAVE)) DWIN_Draw_Label(CLINE(CONTROL_CASE_SAVE), GET_TEXT_F(MSG_STORE_EEPROM));
      if (CVISI(CONTROL_CASE_LOAD)) DWIN_Draw_Label(CLINE(CONTROL_CASE_LOAD), GET_TEXT_F(MSG_LOAD_EEPROM));
      if (CVISI(CONTROL_CASE_RESET)) DWIN_Draw_Label(CLINE(CONTROL_CASE_RESET), GET_TEXT_F(MSG_RESTORE_DEFAULTS));
    #endif
  #else
    if (CVISI(CONTROL_CASE_TEMP)) DWIN_Frame_AreaCopy(1,  1, 89,  83, 101, LBLX, CLINE(CONTROL_CASE_TEMP));           // Temperature >
    if (CVISI(CONTROL_CASE_MOVE)) DWIN_Frame_AreaCopy(1, 84, 89, 128,  99, LBLX, CLINE(CONTROL_CASE_MOVE));           // Motion >
    #if ENABLED(EEPROM_SETTINGS)
      if (CVISI(CONTROL_CASE_SAVE)) DWIN_Frame_AreaCopy(1, 130,  89, 268, 101, LBLX     , CLINE(CONTROL_CASE_SAVE));  // "Store Configuration"  // M.A.R.C. Correct label
      if (CVISI(CONTROL_CASE_LOAD)) DWIN_Frame_AreaCopy(1,  26, 104,  57, 114, LBLX     , CLINE(CONTROL_CASE_LOAD));  // "Read"
      if (CVISI(CONTROL_CASE_LOAD)) DWIN_Frame_AreaCopy(1, 182,  89, 268, 101, LBLX + 34, CLINE(CONTROL_CASE_LOAD));  // "Configuration"
      if (CVISI(CONTROL_CASE_RESET)) DWIN_Frame_AreaCopy(1,  59, 104,  93, 114, LBLX     , CLINE(CONTROL_CASE_RESET)); // "Reset"
      if (CVISI(CONTROL_CASE_RESET)) DWIN_Frame_AreaCopy(1, 182,  89, 268, 101, LBLX + 37, CLINE(CONTROL_CASE_RESET)); // "Configuration"
    #endif
  #endif

  if (CVISI(CONTROL_CASE_REBOOT)) DWIN_Draw_Label(CLINE(CONTROL_CASE_REBOOT), GET_TEXT_F(MSG_RESET_PRINTER));
  if (CVISI(CONTROL_CASE_ADVSET)) DWIN_Draw_Label(CLINE(CONTROL_CASE_ADVSET), GET_TEXT_F(MSG_ADVANCED_SETTINGS));
  if (CVISI(CONTROL_CASE_INFO)) Item_Control_Info(CLINE(CONTROL_CASE_INFO));

  if (CVISI(CONTROL_CASE_TEMP)) Draw_Menu_Line(CSCROL(CONTROL_CASE_TEMP), ICON_Temperature);
  if (CVISI(CONTROL_CASE_TEMP)) Draw_More_Icon(CSCROL(CONTROL_CASE_TEMP));
  if (CVISI(CONTROL_CASE_MOVE)) Draw_Menu_Line(CSCROL(CONTROL_CASE_MOVE), ICON_Motion);
  if (CVISI(CONTROL_CASE_MOVE)) Draw_More_Icon(CSCROL(CONTROL_CASE_MOVE));

  #if ENABLED(EEPROM_SETTINGS)
    if (CVISI(CONTROL_CASE_SAVE)) Draw_Menu_Line(CSCROL(CONTROL_CASE_SAVE), ICON_WriteEEPROM);
    if (CVISI(CONTROL_CASE_LOAD)) Draw_Menu_Line(CSCROL(CONTROL_CASE_LOAD), ICON_ReadEEPROM);
    if (CVISI(CONTROL_CASE_RESET)) Draw_Menu_Line(CSCROL(CONTROL_CASE_RESET), ICON_StockConfiguraton);
  #endif

  if (CVISI(CONTROL_CASE_REBOOT)) Draw_Menu_Line(CSCROL(CONTROL_CASE_REBOOT), ICON_Reboot);
  if (CVISI(CONTROL_CASE_ADVSET)) Draw_Menu_Line(CSCROL(CONTROL_CASE_ADVSET), ICON_AdvSet);
  if (CVISI(CONTROL_CASE_ADVSET)) Draw_More_Icon(CSCROL(CONTROL_CASE_ADVSET));
  if (CVISI(CONTROL_CASE_INFO)) Draw_Menu_Line(CSCROL(CONTROL_CASE_INFO), ICON_Info);
  if (CVISI(CONTROL_CASE_INFO)) Draw_More_Icon(CSCROL(CONTROL_CASE_INFO));

  if (select_control.now && CVISI(select_control.now))  // Draw selected indicator
    Draw_Menu_Cursor(CSCROL(select_control.now));

}

void Draw_Tune_Menu() {
  Clear_Main_Window();

  #if TUNE_CASE_TOTAL >= 6
    const int16_t scroll = MROWS - index_tune; // Scrolled-up lines
    #define TSCROL(L) (scroll + (L))
  #else
    #define TSCROL(L) (L)
  #endif
  #define TLINE(L)  MBASE(TSCROL(L))
  #define TVISI(L)  WITHIN(TSCROL(L), 0, MROWS)  
  
  if (TVISI(0)) Draw_Back_First(select_tune.now == 0);

  #ifdef USE_STRING_HEADINGS
    Draw_Title(GET_TEXT_F(MSG_TUNE));
  #else
    DWIN_Frame_AreaCopy(1, 94, 2, 126, 12, 14, 9);
  #endif

  #ifdef USE_STRING_TITLES
    DWIN_Draw_Label(TLINE(TUNE_CASE_SPEED), GET_TEXT_F(MSG_SPEED));
    #if HAS_HOTEND
      DWIN_Draw_Label(TLINE(TUNE_CASE_TEMP), GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND));
    #endif
    #if HAS_HEATED_BED
      DWIN_Draw_Label(TLINE(TUNE_CASE_BED), GET_TEXT_F(MSG_UBL_SET_TEMP_BED));
    #endif
    #if HAS_FAN
      DWIN_Draw_Label(TLINE(TUNE_CASE_FAN), GET_TEXT_F(MSG_FAN_SPEED));
    #endif
    DWIN_Draw_Label(TLINE(TUNE_CASE_ZOFF), GET_TEXT_F(MSG_ZPROBE_ZOFFSET));
  #else
    DWIN_Frame_AreaCopy(1, 1, 179, 92, 190, LBLX, TLINE(TUNE_CASE_SPEED));      // Print speed
    #if HAS_HOTEND
      DWIN_Frame_AreaCopy(1, 197, 104, 238, 114, LBLX, TLINE(TUNE_CASE_TEMP));  // Hotend...
      DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 44, TLINE(TUNE_CASE_TEMP)); // ...Temperature
    #endif
    #if HAS_HEATED_BED
      DWIN_Frame_AreaCopy(1, 240, 104, 264, 114, LBLX, TLINE(TUNE_CASE_BED));   // Bed...
      DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 27, TLINE(TUNE_CASE_BED));  // ...Temperature
    #endif
    #if HAS_FAN
      DWIN_Frame_AreaCopy(1, 0, 119, 64, 132, LBLX, TLINE(TUNE_CASE_FAN));      // Fan speed
    #endif
    #if HAS_ZOFFSET_ITEM
      DWIN_Frame_AreaCopy(1, 93, 179, 141, 189, LBLX, TLINE(TUNE_CASE_ZOFF));   // Z-offset
    #endif
  #endif

  if (TVISI(TUNE_CASE_FCHNG)) DWIN_Draw_Label(TLINE(TUNE_CASE_FCHNG), GET_TEXT_F(MSG_FILAMENTCHANGE));
  if (TVISI(TUNE_CASE_FCHNG)) Draw_Menu_Line(TUNE_CASE_FCHNG, ICON_FilMan);

  if (select_tune.now && TVISI(select_tune.now))
    Draw_Menu_Cursor(TSCROL(select_tune.now));
  
  if (TVISI(TUNE_CASE_SPEED)) Draw_Menu_Line(TUNE_CASE_SPEED, ICON_Speed);
  if (TVISI(TUNE_CASE_SPEED)) DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, TLINE(TUNE_CASE_SPEED), feedrate_percentage);

  #if HAS_HOTEND
    if (TVISI(TUNE_CASE_TEMP)) Draw_Menu_Line(TUNE_CASE_TEMP, ICON_HotendTemp);
    if (TVISI(TUNE_CASE_TEMP)) DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, TLINE(TUNE_CASE_TEMP), thermalManager.temp_hotend[0].target);
  #endif
  #if HAS_HEATED_BED
    if (TVISI(TUNE_CASE_BED)) Draw_Menu_Line(TUNE_CASE_BED, ICON_BedTemp);
    if (TVISI(TUNE_CASE_BED)) DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, TLINE(TUNE_CASE_BED), thermalManager.temp_bed.target);
  #endif
  #if HAS_FAN
    if (TVISI(TUNE_CASE_FAN)) Draw_Menu_Line(TUNE_CASE_FAN, ICON_FanSpeed);
    if (TVISI(TUNE_CASE_FAN)) DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, TLINE(TUNE_CASE_FAN), thermalManager.fan_speed[0]);
  #endif
  #if HAS_ZOFFSET_ITEM
    if (TVISI(TUNE_CASE_ZOFF)) Draw_Menu_Line(TUNE_CASE_ZOFF, ICON_Zoffset);
    if (TVISI(TUNE_CASE_ZOFF)) DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 2, 2, 202, TLINE(TUNE_CASE_ZOFF), BABY_Z_VAR * 100);
  #endif 
}

inline void draw_max_en(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 245, 119, 270, 129, LBLX, line);   // "Max"  // M.A.R.C. Correct Label Max
}
inline void draw_max_accel_en(const uint16_t line) {
  draw_max_en(line);
  DWIN_Frame_AreaCopy(1, 1, 135, 79, 145, LBLX + 27, line); // "Acceleration"
}
inline void draw_speed_en(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 184, 119, 224, 132, LBLX + inset, line); // "Speed"
}
inline void draw_jerk_en(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 64, 119, 106, 129, LBLX + 27, line); // "Jerk"
}
inline void draw_steps_per_mm(const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 1, 151, 118, 161, LBLX, line);   // "Steps-per-mm" // M.A.R.C. Correct Label Transmission ratio
}
inline void say_x(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 95, 104, 102, 114, LBLX + inset, line); // "X"
}
inline void say_y(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 104, 104, 110, 114, LBLX + inset, line); // "Y"
}
inline void say_z(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 112, 104, 120, 114, LBLX + inset, line); // "Z"
}
inline void say_e(const uint16_t inset, const uint16_t line) {
  DWIN_Frame_AreaCopy(1, 237, 119, 244, 129, LBLX + inset, line); // "E"
}

void Draw_Motion_Menu() {
  Clear_Main_Window();

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

  Draw_Menu_Line(MOTION_CASE_FLOW, ICON_Flow, GET_TEXT(MSG_FLOW),false);  // Flow rate
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(MOTION_CASE_FLOW),planner.flow_percentage[0]);

}

//
// Draw Popup Windows
//

inline void Draw_Popup_Bkgd_60() {
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 60, 258, 330);
}

inline void Draw_Popup_Bkgd_105() {
  DWIN_Draw_Rectangle(1, Color_Bg_Window, 14, 105, 258, 374);
}

void Clear_Popup_Area() {
  Clear_Title_Bar();
  DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 31, DWIN_WIDTH, DWIN_HEIGHT);
}

void DWIN_Popup_Window(uint8_t icon, const char *msg1, const char *msg2) {
  Clear_Main_Window();
  Draw_Popup_Bkgd_60();
  DWIN_ICON_Show(ICON, icon, 101, 105);
  DWIN_Draw_CenteredString(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 8, 210, F(msg1));
  DWIN_Draw_CenteredString(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 8, 240, F(msg2));
}

void DWIN_Popup_Confirm(uint8_t icon, const char *msg1, const char *msg2) {
  last_checkkey = checkkey;
  checkkey = Popup_Window;
  DWIN_Popup_Window(icon, msg1, msg2);  
  DWIN_ICON_Show(ICON, ICON_Confirm_E, 86, 280);
}

#if HAS_HOTEND
  void Popup_Window_ETempTooLow() {
    DWIN_Popup_Window(ICON_TempTooLow, "Nozzle is too cold", "Preheat the hotend");
    DWIN_ICON_Show(ICON, ICON_Confirm_E, 86, 280);
  }
#endif

void Popup_Window_Resume() {
  Clear_Popup_Area();
  Draw_Popup_Bkgd_105();
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 14) / 2, 115, F("Continue Print"));
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 22) / 2, 192, F("It looks like the last"));
  DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, (272 - 8 * 22) / 2, 212, F("file was interrupted."));
  DWIN_ICON_Show(ICON, ICON_Cancel_E,    26, 307);
  DWIN_ICON_Show(ICON, ICON_Continue_E, 146, 307);
}

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
  DWIN_Popup_Window(ICON_BLTouch, "Please confirm",(select_print.now == 1) ? GET_TEXT(MSG_PAUSE_PRINT) : GET_TEXT(MSG_STOP_PRINT));
  DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
  DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
  Draw_Select_Highlight(true);
}

#if HAS_HOTEND || HAS_HEATED_BED
  void DWIN_Popup_Temperature(const bool toohigh) {
    Clear_Popup_Area();
    Draw_Popup_Bkgd_105();
    if (toohigh) {
      DWIN_ICON_Show(ICON, ICON_TempTooHigh, 102, 165);
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 36, 300, F("Nozzle or Bed temperature"));
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 92, 300, F("is too high"));
    }
    else {
      DWIN_ICON_Show(ICON, ICON_TempTooLow, 102, 165);
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 36, 300, F("Nozzle or Bed temperature"));
      DWIN_Draw_String(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, 92, 300, F("is too low"));
    }
  }
#endif

void Draw_Print_Labels() {
   DWIN_Frame_AreaCopy(1, 40,  2,  92, 14,  14,   9);  // Printing
   DWIN_Frame_AreaCopy(1,  0, 44,  96, 58,  46, 173);  // Printing Time
   DWIN_Frame_AreaCopy(1, 98, 44, 152, 58, 181, 173);  // Remain
}

void Draw_Print_ProgressBar() {
  DWIN_ICON_Show(ICON, ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, BarFill_Color, 16 + _percentDone * 240 / 100, 93, 256, 113);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Percent_Color, Color_Bg_Black, 3, 117, 133, _percentDone);
  DWIN_Draw_String(false, false, font8x16, Percent_Color, Color_Bg_Black, 142, 133, F("%"));
}

void Draw_Print_ProgressElapsed() {
  duration_t elapsed = print_job_timer.duration(); // print timer
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 47, 192, elapsed.value / 3600);
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, 63, 192, F(":"));
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 71, 192, (elapsed.value % 3600) / 60);
}

void Draw_Print_ProgressRemain() {
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 181, 192, _remain_time / 3600);
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, 197, 192, F(":"));
  DWIN_Draw_IntValue(true, true, 1, font8x16, Color_White, Color_Bg_Black, 2, 205, 192, (_remain_time % 3600) / 60);
}

void Goto_PrintProcess() {
  checkkey = PrintProcess;

  Clear_Main_Window();
  Draw_Print_Labels();

  ICON_Tune();
  if (printingIsPaused()) ICON_Continue(); else ICON_Pause();
  ICON_Stop();

  DWIN_Print_Header(sdprint ? card.longest_filename() : nullptr);
  
  DWIN_ICON_Show(ICON, ICON_PrintTime, 15, 173);
  DWIN_ICON_Show(ICON, ICON_RemainTime, 150, 171);

  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();
}

void Goto_Main_Menu() {
  checkkey = MainMenu;

  Clear_Main_Window();

  #ifdef USE_STRING_HEADINGS
//    Draw_Title(GET_TEXT_F(MSG_MAIN));
    Draw_Title(MACHINE_NAME);   // M.A.R.C. MachineName
  #else
    DWIN_Frame_AreaCopy(1, 0, 2, 39, 12, 14, 9);
  #endif

  DWIN_ICON_Show(ICON, ICON_LOGO, 71, 52);  // CREALIY logo

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

void HMI_Plan_Move(const feedRate_t fr_mm_s) {
  if (!planner.is_full()) {
    planner.synchronize();
    planner.buffer_line(current_position, fr_mm_s, active_extruder);
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
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_X_scaled))
      return HMI_Move_Done(X_AXIS);
    LIMIT(HMI_ValueStruct.Move_X_scaled, (X_MIN_POS) * MINUNITMULT, (X_MAX_POS) * MINUNITMULT);
    current_position.x = HMI_ValueStruct.Move_X_scaled / MINUNITMULT;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(1), HMI_ValueStruct.Move_X_scaled);
    DWIN_UpdateLCD();
    HMI_Plan_Move(homing_feedrate(X_AXIS));
  }
}

void HMI_Move_Y() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_Y_scaled))
      return HMI_Move_Done(Y_AXIS);
    LIMIT(HMI_ValueStruct.Move_Y_scaled, (Y_MIN_POS) * MINUNITMULT, (Y_MAX_POS) * MINUNITMULT);
    current_position.y = HMI_ValueStruct.Move_Y_scaled / MINUNITMULT;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(2), HMI_ValueStruct.Move_Y_scaled);
    DWIN_UpdateLCD();
    HMI_Plan_Move(homing_feedrate(Y_AXIS));
  }
}

void HMI_Move_Z() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_Z_scaled))
      return HMI_Move_Done(Z_AXIS);
    LIMIT(HMI_ValueStruct.Move_Z_scaled, (Z_MIN_POS) * MINUNITMULT, (Z_MAX_POS) * MINUNITMULT);
    current_position.z = HMI_ValueStruct.Move_Z_scaled / MINUNITMULT;
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(3), HMI_ValueStruct.Move_Z_scaled);
    DWIN_UpdateLCD();
    HMI_Plan_Move(homing_feedrate(Z_AXIS));
  }
}

#if HAS_HOTEND

  void HMI_Move_E() {
    static float last_E_scaled = 0;
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_E_scaled)) {
        last_E_scaled = HMI_ValueStruct.Move_E_scaled;
        EncoderRate.enabled = false;
        checkkey = AxisMove;
        DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(4), HMI_ValueStruct.Move_E_scaled);
        HMI_Plan_Move(MMM_TO_MMS(FEEDRATE_E));
        DWIN_UpdateLCD();
        return;
      }
      LIMIT(HMI_ValueStruct.Move_E_scaled, last_E_scaled - (EXTRUDE_MAXLENGTH) * MINUNITMULT, last_E_scaled + (EXTRUDE_MAXLENGTH) * MINUNITMULT);
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
      LIMIT(HMI_ValueStruct.E_Temp, HEATER_0_MINTEMP, thermalManager.hotend_max_target(0));
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
      LIMIT(HMI_ValueStruct.Bed_Temp, BED_MINTEMP, BED_MAX_TARGET);
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
      LIMIT(HMI_ValueStruct.Fan_speed, 0, 255);
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

#define LAST_AXIS TERN(HAS_HOTEND, E_AXIS, Z_AXIS)

void HMI_MaxFeedspeedXYZE() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Feedspeed)) {
      checkkey = MaxSpeed;
      EncoderRate.enabled = false;
      if (WITHIN(HMI_flag.feedspeed_axis, X_AXIS, LAST_AXIS))
        planner.set_max_feedrate(HMI_flag.feedspeed_axis, HMI_ValueStruct.Max_Feedspeed);
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Feedspeed);
      return;
    }
    // MaxFeedspeed limit
    if (WITHIN(HMI_flag.feedspeed_axis, X_AXIS, LAST_AXIS))
      NOMORE(HMI_ValueStruct.Max_Feedspeed, default_max_feedrate[HMI_flag.feedspeed_axis] * 2);
    if (HMI_ValueStruct.Max_Feedspeed < MIN_MAXFEEDSPEED) HMI_ValueStruct.Max_Feedspeed = MIN_MAXFEEDSPEED;
    // MaxFeedspeed value
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 4, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Feedspeed);
  }
}

void HMI_MaxAccelerationXYZE() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Acceleration)) {
      checkkey = MaxAcceleration;
      EncoderRate.enabled = false;
      if (WITHIN(HMI_flag.acc_axis, X_AXIS, LAST_AXIS))
        planner.set_max_acceleration(HMI_flag.acc_axis, HMI_ValueStruct.Max_Acceleration);
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Acceleration);
      return;
    }
    // MaxAcceleration limit
    if (WITHIN(HMI_flag.acc_axis, X_AXIS, LAST_AXIS))
      NOMORE(HMI_ValueStruct.Max_Acceleration, default_max_acceleration[HMI_flag.acc_axis] * 2);
    if (HMI_ValueStruct.Max_Acceleration < MIN_MAXACCELERATION) HMI_ValueStruct.Max_Acceleration = MIN_MAXACCELERATION;
    // MaxAcceleration value
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 4, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Acceleration);
  }
}

#if HAS_CLASSIC_JERK

  void HMI_MaxJerkXYZE() {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Jerk_scaled)) {
        checkkey = MaxJerk;
        EncoderRate.enabled = false;
        if (WITHIN(HMI_flag.jerk_axis, X_AXIS, LAST_AXIS))
          planner.set_max_jerk(HMI_flag.jerk_axis, HMI_ValueStruct.Max_Jerk_scaled / 10);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Jerk_scaled);
        return;
      }
      // MaxJerk limit
      if (WITHIN(HMI_flag.jerk_axis, X_AXIS, LAST_AXIS))
        NOMORE(HMI_ValueStruct.Max_Jerk_scaled, default_max_jerk[HMI_flag.jerk_axis] * 2 * MINUNITMULT);
      NOLESS(HMI_ValueStruct.Max_Jerk_scaled, (MIN_MAXJERK) * MINUNITMULT);
      // MaxJerk value
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Jerk_scaled);
    }
  }

#endif // HAS_CLASSIC_JERK

void HMI_StepXYZE() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Max_Step_scaled)) {
      checkkey = Step;
      EncoderRate.enabled = false;
      if (WITHIN(HMI_flag.step_axis, X_AXIS, LAST_AXIS))
        planner.settings.axis_steps_per_mm[HMI_flag.step_axis] = HMI_ValueStruct.Max_Step_scaled / 10;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 1, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Step_scaled);
      return;
    }
    // Step limit
    if (WITHIN(HMI_flag.step_axis, X_AXIS, LAST_AXIS))
      NOMORE(HMI_ValueStruct.Max_Step_scaled, 999.9 * MINUNITMULT);
    NOLESS(HMI_ValueStruct.Max_Step_scaled, MIN_STEP);
    // Step value
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Step_scaled);
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
      DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, x, y, F("- ? -"));
    else if (blink && draw_empty)
      DWIN_Draw_String(false, true, font8x16, Color_White, Color_Bg_Black, x, y, F("     "));
    else
      DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, x, y, p * 10);
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
    const celsius_t hc = thermalManager.degHotend(0),
                    ht = thermalManager.degTargetHotend(0);
    const bool _new_hotend_temp = _hotendtemp != hc,
               _new_hotend_target = _hotendtarget != ht;
    if (_new_hotend_temp) _hotendtemp = hc;
    if (_new_hotend_target) _hotendtarget = ht;
  #endif
  #if HAS_HEATED_BED
    static celsius_t _bedtemp = 0, _bedtarget = 0;
    const celsius_t bc = thermalManager.degBed(),
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
    if (_new_fanspeed) {
      _fanspeed = thermalManager.fan_speed[0];
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 195 + 2 * STAT_CHR_W, 384, _fanspeed);
    }
  #endif

  static float _offset = 0;
  if (BABY_Z_VAR != _offset) {
    _offset = BABY_Z_VAR;
    DWIN_Draw_Signed_Float(DWIN_FONT_STAT, Color_Bg_Black, 2, 2, 210, 417, _offset * 100);
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

void make_filename(char *dst, char *src, size_t maxlen=MENU_CHAR_LIMIT) {
  char * const name = card.longest_filename();
  size_t pos        = strlen(name); // index of ending nul

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
  uint8_t icon;
  make_name_without_ext(str, name);
  if (card.flag.filenameIsDir) {
    icon = ICON_Folder;
  } else if (card.flag.filenameIsBin) {
    icon = ICON_Binary;  
  } else icon = ICON_File;

  Draw_Menu_Line(row, icon, str);
}

#if ENABLED(SCROLL_LONG_FILENAMES)

  void Draw_SDItem_Shifted(int8_t &shift) {
    // Limit to the number of chars past the cutoff
    const size_t len = strlen(shift_name);
    NOMORE(shift, (signed)_MAX(len - MENU_CHAR_LIMIT, 0U));

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

void DWIN_MediaInserted() {
  card.cdroot();
  if (checkkey == SelectFile) Redraw_SD_List();    
}

void DWIN_MediaRemoved() {
  // clean file icon
  if (checkkey == SelectFile) {
    Redraw_SD_List();
  }
  else if (sdprint && card.isPrinting() && (checkkey == PrintProcess || checkkey == Tune || printingIsActive())) {
    card.flag.abort_sd_printing = true;
    wait_for_heatup = wait_for_user = false;
    dwin_abort_flag = true; // Reset feedrate, return to Home
  }
}


void HMI_SDCardUpdate() {
  if (HMI_flag.home_flag) return;
  if (DWIN_lcd_sd_status != card.isMounted()) {
    DWIN_lcd_sd_status = card.isMounted();
    //SERIAL_ECHOLNPAIR("HMI_SDCardUpdate: ", DWIN_lcd_sd_status);
    if (DWIN_lcd_sd_status) {
      DWIN_MediaInserted();
    }
    else {
      DWIN_MediaRemoved();
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
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 384, thermalManager.degHotend(0));
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.degTargetHotend(0));

    DWIN_ICON_Show(ICON, ICON_StepE, 112, 417);
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWIN_ICON_Show(ICON, ICON_BedTemp, 10, 416);
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 417, thermalManager.degBed());
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.degTargetBed());
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
  Goto_Main_Menu();
  Draw_Status_Area(with_update);
}

void Draw_Info_Menu() {
  Clear_Main_Window();

  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(MACHINE_SIZE) * MENU_CHR_W) / 2, 122, (char*)MACHINE_SIZE);
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(SHORT_BUILD_VERSION) * MENU_CHR_W) / 2, 195, (char*)SHORT_BUILD_VERSION);
  
  #ifdef USE_STRING_HEADINGS
    Draw_Title(GET_TEXT_F(MSG_INFO_SCREEN));
  #else
    DWIN_Frame_TitleCopy(1, 190, 16, 215, 26); // "Info"
  #endif

  DWIN_Frame_AreaCopy(1, 120, 150, 146, 161, 124, 102);
  DWIN_Frame_AreaCopy(1, 146, 151, 254, 161, 82, 175);
  // DWIN_Frame_AreaCopy(1, 0, 165, 94, 175, 89, 248);

  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen("Build Datetime") * MENU_CHR_W) / 2, 248, (char*)("Build Datetime"));    // M.A.R.C. Display Firmware build date-time
  DWIN_Draw_String(false, false, font8x16, Color_White, Color_Bg_Black, (DWIN_WIDTH - strlen(STRING_DISTRIBUTION_DATE " " STRING_DISTRIBUTION_TIME) * MENU_CHR_W) / 2, 268, (char*)(STRING_DISTRIBUTION_DATE " " STRING_DISTRIBUTION_TIME));

  Draw_Back_First();
  LOOP_L_N(i, 3) {
    DWIN_ICON_Show(ICON, ICON_PrintSize + i, 26, 99 + i * 73);
    DWIN_Draw_Line(Line_Color, 16, MBASE(2) + i * 73, 256, 156 + i * 73);
  }
}

void Draw_Print_File_Menu() {
  Clear_Title_Bar();

  #ifdef USE_STRING_HEADINGS
    Draw_Title(GET_TEXT_F(MSG_MEDIA_MENU));
  #else
    DWIN_Frame_TitleCopy(1, 52, 31, 137, 41); // "Print file"
  #endif

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
      case 0: // Print File
        checkkey = SelectFile;
        gcode.process_subcommands_now_P(PSTR("M21" )); // M.A.R.C. Re-Init SD card for support Extension
        Draw_Print_File_Menu();
        break;

      case 1: // Prepare
        checkkey = Prepare;
        select_prepare.reset();
        index_prepare = MROWS;
        Draw_Prepare_Menu();
        break;

      case 2: // Control
        checkkey = Control;
        select_control.reset();
        index_control = MROWS;
        Draw_Control_Menu();
        break;

      case 3: // Leveling or Info
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
  ENCODER_DiffState encoder_diffState = get_encoder_state();

  const uint16_t hasUpDir = !card.flag.workDirIsRoot;

  if (encoder_diffState == ENCODER_DIFF_NO) {
    #if ENABLED(SCROLL_LONG_FILENAMES)
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
        else {
          Draw_SDItem(itemnum, 0);                              // Draw the item (and init shift name)
        }
      }
      else {
        Move_Highlight(-1, select_file.now + MROWS - index_file); // Just move highlight
        TERN_(SCROLL_LONG_FILENAMES, Init_Shift_Name());        // ...and init the shift name
      }
      TERN_(SCROLL_LONG_FILENAMES, Init_SDItem_Shift());        // Reset left. Init timer.
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (select_file.now == 0) { // Back
      select_page.set(0);
      Goto_Main_Menu();
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

      if (strcmp(strrchr(card.filename,'.'),".GCO")==0) {
        card.openAndPrintFile(card.filename);
        DWIN_Start_Print(true);
      } else {    
        DWIN_Popup_Confirm(ICON_Error,"Please, check filenames","Only gcode can be printed");
      }      

      #if HAS_FAN
        // All fans on for Ender 3 v2 ?
        // The slicer should manage this for us.
        //for (uint8_t i = 0; i < FAN_COUNT; i++)
        //  thermalManager.fan_speed[i] = 255;
      #endif

    }
  }
HMI_SelectFileExit:
  DWIN_UpdateLCD();
}

/* Printing */
void HMI_Printing() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (HMI_flag.done_confirm_flag) {
    if (encoder_diffState == ENCODER_DIFF_ENTER) {
      HMI_flag.done_confirm_flag = false;
      dwin_abort_flag = true; // Reset feedrate, return to Home
      Goto_Main_Menu(); // M.A.R.C. review if it is necessary
    }
    return;
  }

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_print.inc(3)) {
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
        checkkey = Back_Main;
        if (HMI_flag.home_flag) planner.synchronize(); // Wait for planner moves to finish!
        wait_for_heatup = wait_for_user = false;       // Stop waiting for heating/user
        card.flag.abort_sd_printing = true;            // Let the main loop handle SD abort
        dwin_abort_flag = true;                        // Reset feedrate, return to Home
        #ifdef ACTION_ON_CANCEL
          host_action_cancel();
        #endif
        DWIN_Popup_Window(ICON_BLTouch, "Parking" , "Please wait until done.");
      }
      else
        Goto_PrintProcess(); // cancel stop
    }
  }
  DWIN_UpdateLCD();
}

void Draw_Move_Menu() {
  Clear_Main_Window();

  #ifdef USE_STRING_HEADINGS
    Draw_Title(GET_TEXT_F(MSG_MOVE_AXIS));
  #else
    DWIN_Frame_TitleCopy(1, 231, 2, 265, 12);                     // Move
  #endif

  #ifdef USE_STRING_TITLES
    DWIN_Draw_Label(MBASE(1), GET_TEXT_F(MSG_MOVE_X));
    DWIN_Draw_Label(MBASE(2), GET_TEXT_F(MSG_MOVE_Y));
    DWIN_Draw_Label(MBASE(3), GET_TEXT_F(MSG_MOVE_Z));
  #else
    DWIN_Frame_AreaCopy(1, 69, 61, 102, 71, LBLX, MBASE(1)); // "Move"
    DWIN_Frame_AreaCopy(1, 69, 61, 102, 71, LBLX, MBASE(2)); // "Move"
    DWIN_Frame_AreaCopy(1, 69, 61, 102, 71, LBLX, MBASE(3)); // "Move"
    say_x(36, MBASE(1));  // X
    say_y(36, MBASE(2));  // Y
    say_z(36, MBASE(3));  // Z
  #endif
  #if HAS_HOTEND
    DWIN_Frame_AreaCopy(1, 123, 192, 176, 202, LBLX, MBASE(4));   // Extruder
  #endif

  Draw_Back_First(select_item.now == 0);
  if (select_item.now) Draw_Menu_Cursor(select_item.now);

  // Draw separators and icons
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_MoveX + i);
}

void Draw_AdvSet_Menu() {
  Clear_Main_Window();
  
  #if ADVSET_CASE_TOTAL >= 6
    const int16_t scroll = MROWS - index_advset; // Scrolled-up lines
    #define ASCROL(L) (scroll + (L))
  #else
    #define ASCROL(L) (L)
  #endif
  
  #define AVISI(L)  WITHIN(ASCROL(L), 0, MROWS)
  
  Draw_Title(GET_TEXT_F(MSG_ADVANCED_SETTINGS));

  if (AVISI(0)) Draw_Back_First(select_advSet.now == 0);
  if (AVISI(ADVSET_CASE_HOMEOFF)) Draw_Menu_Line(ASCROL(ADVSET_CASE_HOMEOFF), ICON_HomeOff, GET_TEXT(MSG_SET_HOME_OFFSETS),true);  // Home Offset
  #if HAS_ONESTEP_LEVELING
  if (AVISI(ADVSET_CASE_PROBEOFF)) Draw_Menu_Line(ASCROL(ADVSET_CASE_PROBEOFF), ICON_ProbeOff, GET_TEXT(MSG_ZPROBE_OFFSETS),true);  // Probe Offset
  #endif
  if (AVISI(ADVSET_CASE_HEPID)) Draw_Menu_Line(ASCROL(ADVSET_CASE_HEPID), ICON_PIDNozzle, "Hotend PID", false);  // Nozzle PID
  if (AVISI(ADVSET_CASE_BEDPID)) Draw_Menu_Line(ASCROL(ADVSET_CASE_BEDPID), ICON_PIDbed, "Bed PID", false);  // Bed PID
  #if HAS_FILAMENT_SENSOR
  if (AVISI(ADVSET_CASE_RUNOUT)) Draw_Menu_Line(ASCROL(ADVSET_CASE_RUNOUT), ICON_Runout, GET_TEXT(MSG_RUNOUT_SENSOR), false);  // Runout Sensor
  if (AVISI(ADVSET_CASE_RUNOUT)) Draw_Chkb_Line(ASCROL(ADVSET_CASE_RUNOUT),runout.enabled);
  #endif 
  if (AVISI(ADVSET_CASE_PWRLOSSR)) Draw_Menu_Line(ASCROL(ADVSET_CASE_PWRLOSSR), ICON_Motion, GET_TEXT(MSG_RUNOUT_SENSOR), false);  // Runout Sensor
  if (AVISI(ADVSET_CASE_PWRLOSSR)) Draw_Chkb_Line(ASCROL(ADVSET_CASE_PWRLOSSR),recovery.enabled);
  if (select_advSet.now  && AVISI(select_advSet.now)) Draw_Menu_Cursor(ASCROL(select_advSet.now));
}

void Draw_HomeOff_Menu() {
  Clear_Main_Window();
  Draw_Title(GET_TEXT_F(MSG_SET_HOME_OFFSETS));                 // Home Offsets
  Draw_Back_First(select_item.now == 0);
  Draw_Menu_Line(1, ICON_HomeOffX, GET_TEXT(MSG_HOME_XOFFSET));  // Home X Offset
  DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(1), HMI_ValueStruct.Home_OffX_scaled);
  Draw_Menu_Line(2, ICON_HomeOffY, GET_TEXT(MSG_HOME_YOFFSET));  // Home Y Offset
  DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(2), HMI_ValueStruct.Home_OffY_scaled);
  if (select_item.now) Draw_Menu_Cursor(select_item.now);
}

void Draw_ProbeOff_Menu() {
  Clear_Main_Window();
  Draw_Title(GET_TEXT_F(MSG_ZPROBE_OFFSETS));                 // Probe Offsets
  Draw_Back_First(select_item.now == 0);
  Draw_Menu_Line(1, ICON_ProbeOffX, GET_TEXT(MSG_ZPROBE_XOFFSET));  // Probe X Offset
  DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(1), HMI_ValueStruct.Probe_OffX_scaled);
  Draw_Menu_Line(2, ICON_ProbeOffY, GET_TEXT(MSG_ZPROBE_YOFFSET));  // Probe Y Offset
  DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(2), HMI_ValueStruct.Probe_OffY_scaled);
  if (select_item.now) Draw_Menu_Cursor(select_item.now);
}

void Draw_FilamentMan_Menu(){
  Clear_Main_Window();
  Draw_Title(GET_TEXT_F(MSG_FILAMENT_MAN));
  Draw_Back_First(select_item.now == 0);
  Draw_Menu_Line(1, ICON_FilMan, GET_TEXT(MSG_FILAMENTCHANGE));
  Draw_Menu_Line(2, ICON_FilUnload, GET_TEXT(MSG_FILAMENTUNLOAD));
  Draw_Menu_Line(3, ICON_FilLoad, GET_TEXT(MSG_FILAMENTLOAD));
  if (select_item.now) Draw_Menu_Cursor(select_item.now);
}

void Draw_ManualLev_Menu() {
  Clear_Main_Window();
  Draw_Title(GET_TEXT_F(MSG_MOVE_AXIS));
  Draw_Back_First(select_item.now == 0);
  DWIN_Draw_Label(MBASE(1), GET_TEXT_F(MSG_MANLEV_FL));
  DWIN_Draw_Label(MBASE(2), GET_TEXT_F(MSG_MANLEV_FR));
  DWIN_Draw_Label(MBASE(3), GET_TEXT_F(MSG_MANLEV_BR));
  DWIN_Draw_Label(MBASE(4), GET_TEXT_F(MSG_MANLEV_BL));
  DWIN_Draw_Label(MBASE(5), GET_TEXT_F(MSG_MANLEV_C));
  // Draw separators and icons
  LOOP_L_N(i, 5) Draw_Menu_Line(i + 1, ICON_Axis);
  if (select_item.now) Draw_Menu_Cursor(select_item.now);
}

#if ENABLED(MESH_BED_LEVELING)
void Draw_ManualMesh_Menu() {
  Clear_Main_Window();
  Draw_Title(GET_TEXT_F(MSG_UBL_MANUAL_MESH));
  Draw_Back_First(select_item.now == 0);
  Draw_Menu_Line(1, ICON_ManualMesh, GET_TEXT(MSG_UBL_BUILD_MESH_MENU)); //Start -> G29 S1
  Draw_Menu_Line(2, ICON_Zoffset, GET_TEXT(MSG_MOVE_Z)); //Move Z ->
  Draw_Menu_Line(3, ICON_Axis, GET_TEXT(MSG_UBL_CONTINUE_MESH)); //Next -> G29 S2
  Draw_Menu_Line(4, ICON_MeshSave,GET_TEXT(MSG_UBL_SAVE_MESH)); //Save -> M500
  if (select_item.now) Draw_Menu_Cursor(select_item.now);
}
#endif

#include "../../../libs/buzzer.h"

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
void HMI_Prepare() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
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

        #if ENABLED(MESH_BED_LEVELING)
        if (index_prepare == PREPARE_CASE_MMESH) Item_Prepare_ManualMesh(MROWS);// M.A.R.C. Manual Mesh
        #endif
        #if HAS_ZOFFSET_ITEM
         if (index_prepare == PREPARE_CASE_ZOFF) Item_Prepare_Offset(MROWS); // M.A.R.C. Show Edit Z-Offset
        #endif
        #if HAS_HOTEND
          if (index_prepare == PREPARE_CASE_PLA) Item_Prepare_PLA(MROWS);  // M.A.R.C. Show PLA 
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

             if (index_prepare == 6) Item_Prepare_FilMan(0);
        else if (index_prepare == 7) Item_Prepare_Move(0);
        else if (index_prepare == 8) Item_Prepare_ManualLev(0);
        else if (index_prepare == 9) Item_Prepare_Disable(0);
        else if (index_prepare == 10) Item_Prepare_Home(0);
      #if ENABLED(MESH_BED_LEVELING)
        else if (index_prepare == 11) Item_Prepare_ManualMesh(0);
      #endif  
      }
      else {
        Move_Highlight(-1, select_prepare.now + MROWS - index_prepare);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_prepare.now) {
      case 0: // Back
        select_page.set(1);
        Goto_Main_Menu();
        break;
      case PREPARE_CASE_FMAN: // Filament Management
        checkkey = FilamentMan;
        select_item.reset();
        Draw_FilamentMan_Menu();
        break; 
      case PREPARE_CASE_MOVE: // Axis move
        checkkey = AxisMove;
        select_item.reset();
        Draw_Move_Menu();

        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(1), current_position.x * MINUNITMULT);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(2), current_position.y * MINUNITMULT);
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 216, MBASE(3), current_position.z * MINUNITMULT);
        #if HAS_HOTEND
          HMI_ValueStruct.Move_E_scaled = current_position.e * MINUNITMULT;
          DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(4), HMI_ValueStruct.Move_E_scaled);
        #endif
        break;
      case PREPARE_CASE_MLEV: // Manual leveling
        checkkey = ManualLev;
        select_item.reset();
        Draw_ManualLev_Menu();
        break; 
      case PREPARE_CASE_DISA: // Disable steppers
        queue.inject_P(PSTR("M84"));
        break;
      case PREPARE_CASE_HOME: // Homing
        index_prepare = MROWS;
        queue.inject_P(G28_STR); // G28 will set home_flag
        break;
      #if ENABLED(MESH_BED_LEVELING)
      case PREPARE_CASE_MMESH: // Manual Mesh
        checkkey = ManualMesh;
        select_item.reset();
        Draw_ManualMesh_Menu();
        DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 2, 216, MBASE(2), current_position.z * 100);
        break; 
      #endif      
      #if HAS_ZOFFSET_ITEM
        case PREPARE_CASE_ZOFF: // Z-offset
          #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
            checkkey = Zoffset;
            HMI_ValueStruct.show_mode = -4;
            HMI_ValueStruct.offset_value = BABY_Z_VAR * 100;
            DWIN_Draw_Signed_Float(font8x16, Select_Color, 2, 2, 202, MBASE(PREPARE_CASE_ZOFF + MROWS - index_prepare), HMI_ValueStruct.offset_value);
            EncoderRate.enabled = true;
          #else
            // Apply workspace offset, making the current position 0,0,0
            queue.inject_P(PSTR("G92 X0 Y0 Z0"));
            HMI_AudioFeedback();
          #endif
          break;
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
        DWIN_Popup_Confirm(ICON_BLTouch,"Sorry,", "No other language implemented");
        break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

#if HAS_HOTEND
void Draw_PLA_Menu() {
  Clear_Main_Window();

  #ifdef USE_STRING_HEADINGS
    Draw_Title("PLA Settings"); // TODO: GET_TEXT_F
  #else
    DWIN_Frame_TitleCopy(1, 56, 16, 141, 28);                                       // "PLA Settings"
  #endif
  #ifdef USE_STRING_TITLES
    DWIN_Draw_Label(MBASE(PREHEAT_CASE_TEMP), GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND)); // "Hotend Temp"
    #if HAS_HEATED_BED
      DWIN_Draw_Label(MBASE(PREHEAT_CASE_BED), GET_TEXT_F(MSG_UBL_SET_TEMP_BED));   // "Bed Temp"
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
    DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 71, MBASE(PREHEAT_CASE_TEMP));      // PLA nozzle temp
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
      DWIN_Frame_AreaCopy(1, 97, 165, 229, 177, LBLX, MBASE(PREHEAT_CASE_SAVE));      // Save PLA configuration
    #endif
  #endif

  Draw_Back_First();

  uint8_t i = 0;
  Draw_Menu_Line(++i, ICON_SetEndTemp);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(i), ui.material_preset[0].hotend_temp);
  #if HAS_HEATED_BED
    Draw_Menu_Line(++i, ICON_SetBedTemp);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(i), ui.material_preset[0].bed_temp);
  #endif
  #if HAS_FAN
    Draw_Menu_Line(++i, ICON_FanSpeed);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(i), ui.material_preset[0].fan_speed);
  #endif
  #if ENABLED(EEPROM_SETTINGS)
    Draw_Menu_Line(++i, ICON_WriteEEPROM);
  #endif
}

void Draw_ABS_Menu(){
  Clear_Main_Window();

  #ifdef USE_STRING_HEADINGS
    Draw_Title("ABS Settings"); // TODO: GET_TEXT_F
  #else
    DWIN_Frame_TitleCopy(1, 56, 16, 141, 28);                                         // "ABS Settings"
  #endif
  #ifdef USE_STRING_TITLES
    DWIN_Draw_Label(MBASE(PREHEAT_CASE_TEMP), GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND));   // "Hotend Temp"
    #if HAS_HEATED_BED
      DWIN_Draw_Label(MBASE(PREHEAT_CASE_BED), GET_TEXT_F(MSG_UBL_SET_TEMP_BED));     // "Bed Temp"
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
      DWIN_Frame_AreaCopy(1, 0, 119, 64, 132, LBLX + 27, MBASE(PREHEAT_CASE_FAN));    // ABS fan speed
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      DWIN_Frame_AreaCopy(1, 97, 165, 229, 177, LBLX, MBASE(PREHEAT_CASE_SAVE));
      DWIN_Frame_AreaCopy(1, 172, 76, 198, 86, LBLX + 33, MBASE(PREHEAT_CASE_SAVE));  // Save ABS configuration
    #endif
  #endif

  Draw_Back_First();

  uint8_t i = 0;
  Draw_Menu_Line(++i, ICON_SetEndTemp);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(i), ui.material_preset[1].hotend_temp);
  #if HAS_HEATED_BED
    Draw_Menu_Line(++i, ICON_SetBedTemp);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(i), ui.material_preset[1].bed_temp);
  #endif
  #if HAS_FAN
    Draw_Menu_Line(++i, ICON_FanSpeed);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(i), ui.material_preset[1].fan_speed);
  #endif
  #if ENABLED(EEPROM_SETTINGS)
    Draw_Menu_Line(++i, ICON_WriteEEPROM);
  #endif
}
#endif


void Draw_Temperature_Menu() {
  Clear_Main_Window();

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
  #endif

  Draw_Back_First(select_temp.now == 0);
  if (select_temp.now) Draw_Menu_Cursor(select_temp.now);

  // Draw icons and lines
  uint8_t i = 0;
  #define _TMENU_ICON(N) Draw_Menu_Line(++i, ICON_SetEndTemp + (N) - 1)
  #if HAS_HOTEND
    _TMENU_ICON(TEMP_CASE_TEMP);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(i), thermalManager.degTargetHotend(0));
  #endif
  #if HAS_HEATED_BED
    _TMENU_ICON(TEMP_CASE_BED);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(i), thermalManager.degTargetBed());
  #endif
  #if HAS_FAN
    _TMENU_ICON(TEMP_CASE_FAN);
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(i), thermalManager.fan_speed[0]);
  #endif
  #if HAS_HOTEND
    // PLA/ABS items have submenus
    _TMENU_ICON(TEMP_CASE_PLA);
    Draw_More_Icon(i);
    _TMENU_ICON(TEMP_CASE_ABS);
    Draw_More_Icon(i);
  #endif
}

/* Control */
void HMI_Control() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_control.inc(1 + CONTROL_CASE_TOTAL)) {
      if (select_control.now > MROWS && select_control.now > index_control) {
        index_control = select_control.now;
        
        // Scroll up and draw a blank bottom line
        Scroll_Menu(DWIN_SCROLL_UP);
        
        switch (index_control) {  // Last menu items
          case CONTROL_CASE_REBOOT:  // Reset Printer
            Draw_Menu_Item(MROWS, ICON_Reboot, GET_TEXT(MSG_RESET_PRINTER));
            break;
          case CONTROL_CASE_ADVSET:  // Advance Settings >
            Draw_Menu_Item(MROWS, ICON_AdvSet, GET_TEXT(MSG_ADVANCED_SETTINGS), true);
            break;
          case CONTROL_CASE_INFO:    // Info >
            Draw_Menu_Item(MROWS, ICON_Info, GET_TEXT(MSG_INFO_SCREEN), true);
            break;
          default: break;
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

        switch (index_control) {  // First menu items
          case MROWS :
            Draw_Back_First();
            break;
          case MROWS+1 :  // Temperature >
            Draw_Menu_Line(0, ICON_Temperature, GET_TEXT(MSG_TEMPERATURE), true);
            break;
          case MROWS+2 :  // Move >
            Draw_Menu_Line(0, ICON_Motion, GET_TEXT(MSG_MOTION), true);
          default: break;
        }
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
        Goto_Main_Menu();
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
          break;
      #endif
      case CONTROL_CASE_REBOOT: // Reset Printer
        checkkey = Reboot;
        HMI_AudioFeedback();
        break;
      case CONTROL_CASE_ADVSET: // Advance Settings
        checkkey = AdvSet;
        select_advSet.reset();
        Draw_AdvSet_Menu();
        break;
      case CONTROL_CASE_INFO: // Info
        checkkey = Info;
        Draw_Info_Menu();
        break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}


#if HAS_ONESTEP_LEVELING

  /* Leveling */
  void HMI_Leveling() {
    DWIN_Popup_Window(ICON_AutoLeveling, GET_TEXT(MSG_BED_LEVELING), "Please wait until done."); 
    DWIN_UpdateLCD();
    queue.inject_P(PSTR("G28O\nG29"));
  }

#endif

/* Axis Move */
void HMI_AxisMove() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

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
    if (select_item.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_item.dec()) Move_Highlight(-1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_item.now) {
      case 0: // Back
        checkkey = Prepare;
        select_prepare.set(PREPARE_CASE_MOVE);
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
              if (thermalManager.degHotend(0) < EXTRUDE_MINTEMP) {
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
            break;
        #endif
    }
  }
  DWIN_UpdateLCD();
}

/* Filament Management */
void HMI_FilamentMan(){
  ENCODER_DiffState encoder_diffState = get_encoder_state();
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
      case 0: // Back
        checkkey = Prepare;
        select_prepare.set(1);
        index_prepare = MROWS;
        DWIN_StatusChanged((char*)"");
        Draw_Prepare_Menu();
        break;
      case 1: // Filament Change
        queue.inject_P(PSTR("M600 B2"));
        break;
      case 2: // Unload Filament
        DWIN_StatusChanged(GET_TEXT(MSG_FILAMENTUNLOAD));
        queue.inject_P(PSTR("M702 Z20"));
        break;
      case 3: // Load Filament
        DWIN_StatusChanged(GET_TEXT(MSG_FILAMENTLOAD));
        queue.inject_P(PSTR("M701 Z20"));
        break;
    }
  }
  DWIN_UpdateLCD();
}

/* Manual Leveling */
void HMI_ManualLev() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_item.inc(1 + 5)) Move_Highlight(1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_item.dec()) Move_Highlight(-1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {

    switch (select_item.now) {
      case 0: // Back
        DWIN_StatusChanged((char*)"");
        //queue.inject_P(PSTR("M420 S1\nG90\nG0 Z2"));
        checkkey = Prepare;
        select_prepare.set(3);
        index_prepare = MROWS;
        Draw_Prepare_Menu();
        break;
      case 1: // move to front left
        DWIN_StatusChanged((char*)"Level front left");
        queue.inject_P(PSTR("M420 S0\nG28O\nG90\nG0 Z2\nG0 X30 Y30 F3000\nG0 Z0 F300"));
        break;
      case 2: // move to front right
        DWIN_StatusChanged((char*)"Level front right");
        queue.inject_P(PSTR("M420 S0\nG28O\nG90\nG0 Z2\nG0 X200 Y30 F3000\nG0 Z0 F300"));
        break;
      case 3: // move to back right
        DWIN_StatusChanged((char*)"Level back right");
        queue.inject_P(PSTR("M420 S0\nG28O\nG90\nG0 Z2\nG0 X200 Y200 F3000\nG0 Z0 F300"));
        break;
      case 4: // move to back left
        DWIN_StatusChanged((char*)"Level back left");
        queue.inject_P(PSTR("M420 S0\nG28O\nG90\nG0 Z2\nG0 X30 Y200 F3000\nG0 Z0 F300"));
        break;
      case 5: // move to center
        DWIN_StatusChanged((char*)"Level center");
        queue.inject_P(PSTR("M420 S0\nG28O\nG90\nG0 Z2\nG0 X115 Y115 F3000\nG0 Z0 F300"));
        break;
    }
  }
  DWIN_UpdateLCD();
}

#if ENABLED(MESH_BED_LEVELING)

/* Manual Mesh */
void HMI_ManualMesh() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_item.inc(1 + 4)) Move_Highlight(1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_item.dec()) Move_Highlight(-1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_item.now) {
      case 0: // Back
        checkkey = Prepare;
        select_prepare.set(1);
        index_prepare = MROWS;
        Draw_Prepare_Menu();
        break;
      case 1: // Start manual mesh
        DWIN_StatusChanged(GET_TEXT(MSG_UBL_BUILD_MESH_MENU));
        queue.inject_P(PSTR("G28O\nM211 S0\nG29 S1"));
        break;
      case 2: // move Z
        DWIN_StatusChanged(GET_TEXT(MSG_MOVE_Z));
        checkkey = MMeshMoveZ;
        HMI_ValueStruct.Move_Z_scaled = current_position.z * 100;
        DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, 2, 216, MBASE(2), HMI_ValueStruct.Move_Z_scaled);
        EncoderRate.enabled = true;
        break;
      case 3: // Next mesh point
        queue.inject_P(PSTR("G29 S2"));
        break;
      case 4: // Save Mesh
        DWIN_StatusChanged(GET_TEXT(MSG_UBL_STORAGE_MESH_MENU));
        queue.inject_P(PSTR("M211 S1\nM500"));    
        break;
    }
  }
  DWIN_UpdateLCD();
}

void HMI_MMeshMoveZ() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Move_Z_scaled)) {
      EncoderRate.enabled = false;
      planner.synchronize();
      checkkey = ManualMesh;
      DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 2, 216, MBASE(2), HMI_ValueStruct.Move_Z_scaled);
      DWIN_UpdateLCD();
      return;
    }
    LIMIT(HMI_ValueStruct.Move_Z_scaled, -100, 100);
    current_position.z = HMI_ValueStruct.Move_Z_scaled / 100;
    DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, 2, 216, MBASE(2), HMI_ValueStruct.Move_Z_scaled);
    DWIN_UpdateLCD();
    HMI_Plan_Move(homing_feedrate(Z_AXIS));
  }
}

#endif


/* TemperatureID */
void HMI_Temperature() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
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
      case 0: // Back
        checkkey = Control;
        select_control.set(1);
        index_control = MROWS;
        Draw_Control_Menu();
        break;
      #if HAS_HOTEND
        case TEMP_CASE_TEMP: // Nozzle temperature
          checkkey = ETemp;
          HMI_ValueStruct.E_Temp = thermalManager.degTargetHotend(0);
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(1), HMI_ValueStruct.E_Temp);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_HEATED_BED
        case TEMP_CASE_BED: // Bed temperature
          checkkey = BedTemp;
          HMI_ValueStruct.Bed_Temp = thermalManager.degTargetBed();
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(2), HMI_ValueStruct.Bed_Temp);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_FAN
        case TEMP_CASE_FAN: // Fan speed
          checkkey = FanSpeed;
          HMI_ValueStruct.Fan_speed = thermalManager.fan_speed[0];
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(3), HMI_ValueStruct.Fan_speed);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_HOTEND
        case TEMP_CASE_PLA:  // PLA preheat setting
          checkkey = PLAPreheat;
          select_item.reset();
          HMI_ValueStruct.show_mode = -2;
          Draw_PLA_Menu();
          break;

        case TEMP_CASE_ABS:  // ABS preheat setting
          checkkey = ABSPreheat;
          select_item.reset();
          HMI_ValueStruct.show_mode = -3;
          Draw_ABS_Menu();
          break;

      #endif // HAS_HOTEND
    }
  }
  DWIN_UpdateLCD();
}

void Draw_Max_Speed_Menu() {
  Clear_Main_Window();

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
  #endif

  Draw_Back_First();
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_MaxAccX + i);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(1), planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(2), planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
  DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(3), planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
  #if HAS_HOTEND
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 4, 210, MBASE(4), planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
  #endif
}

#if HAS_CLASSIC_JERK
  void Draw_Max_Jerk_Menu() {
    Clear_Main_Window();

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
    draw_steps_per_mm(MBASE(1)); say_x(120, MBASE(1)); // "Steps-per-mm X"  // M.A.R.C. Correct Label position
    draw_steps_per_mm(MBASE(2)); say_y(120, MBASE(2)); // "Y"               // M.A.R.C. Correct Label position
    draw_steps_per_mm(MBASE(3)); say_z(120, MBASE(3)); // "Z"               // M.A.R.C. Correct Label position
    #if HAS_HOTEND
      draw_steps_per_mm(MBASE(4)); say_e(120, MBASE(4)); // "E"             // M.A.R.C. Correct Label position
    #endif
  #endif

  Draw_Back_First();
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_StepX + i);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(1), planner.settings.axis_steps_per_mm[X_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(2), planner.settings.axis_steps_per_mm[Y_AXIS] * MINUNITMULT);
  DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(3), planner.settings.axis_steps_per_mm[Z_AXIS] * MINUNITMULT);
  #if HAS_HOTEND
    DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, UNITFDIGITS, 210, MBASE(4), planner.settings.axis_steps_per_mm[E_AXIS] * MINUNITMULT);
  #endif
}

/* Motion */
void HMI_Motion() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
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
      case 0: // Back
        checkkey = Control;
        select_control.set(CONTROL_CASE_MOVE);
        index_control = MROWS;
        Draw_Control_Menu();
        break;
      case MOTION_CASE_RATE:   // Max speed
        checkkey = MaxSpeed;
        select_item.reset();
        Draw_Max_Speed_Menu();
        break;
      case MOTION_CASE_ACCEL:  // Max acceleration
        checkkey = MaxAcceleration;
        select_item.reset();
        Draw_Max_Accel_Menu();
        break;
      #if HAS_CLASSIC_JERK
        case MOTION_CASE_JERK: // Max jerk
          checkkey = MaxJerk;
          select_item.reset();
          Draw_Max_Jerk_Menu();
         break;
      #endif
      case MOTION_CASE_STEPS:  // Steps per mm
        checkkey = Step;
        select_item.reset();
        Draw_Steps_Menu();
        break;
      case MOTION_CASE_FLOW: // Flow rate
        checkkey = PrintFlow;
        HMI_ValueStruct.print_flow = planner.flow_percentage[0];
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(MOTION_CASE_FLOW),planner.flow_percentage[0]);
        EncoderRate.enabled = true;
        break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

void HMI_PrintFlow() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.print_flow)) {
      checkkey = Motion;
      EncoderRate.enabled = false;
      planner.flow_percentage[0] = HMI_ValueStruct.print_flow;
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Color_Bg_Black, 3, 216, MBASE(MOTION_CASE_FLOW),planner.flow_percentage[0]);
      return;
    }
    // print_flow limit
    LIMIT(HMI_ValueStruct.print_flow, MIN_PRINT_FLOW, MAX_PRINT_FLOW);
    // print_flow value
    DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(MOTION_CASE_FLOW),HMI_ValueStruct.print_flow);
  }
}

/* Reset Printer */
void HMI_Reboot() {
  last_checkkey = MainMenu;
  checkkey = Popup_Window;
  DWIN_Popup_Window(ICON_BLTouch, GET_TEXT(MSG_RESET_PRINTER), "Please wait until reboot.");
  DWIN_UpdateLCD();  
  delay(1000);
  queue.inject_P(PSTR("M997"));
}

/* Motion */
void HMI_AdvSet() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_advSet.inc(1 + ADVSET_CASE_TOTAL)) {
      if (select_advSet.now > MROWS && select_advSet.now > index_advset) {
        index_advset = select_advSet.now;

        // Scroll up and draw a blank bottom line
        Scroll_Menu(DWIN_SCROLL_UP);

        switch (index_advset) {  // Last menu items
          #if HAS_FILAMENT_SENSOR
          case ADVSET_CASE_RUNOUT :   // Runout sersor
            Draw_Menu_Line(MROWS, ICON_Runout, GET_TEXT(MSG_RUNOUT_SENSOR), false);  // Runout Sensor
            Draw_Chkb_Line(MROWS,runout.enabled);
            break;
          #endif
          case ADVSET_CASE_PWRLOSSR : // Power-lost recovery
            Draw_Menu_Line(MROWS, ICON_Motion, "Power-loss recovery", false);  // Power-loss
            Draw_Chkb_Line(MROWS,recovery.enabled);
            break;  
          default: break;
        }

      }
      else {
        Move_Highlight(1, select_advSet.now + MROWS - index_advset);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_advSet.dec()) {
      if (select_advSet.now < index_advset - MROWS) {
        index_advset--;
        Scroll_Menu(DWIN_SCROLL_DOWN);

        switch (index_advset) {  // First menu items
          case MROWS :
            Draw_Back_First();
            break;
          case MROWS+ADVSET_CASE_HOMEOFF :  // Home Offset >
            Draw_Menu_Line(0, ICON_HomeOff, GET_TEXT(MSG_SET_HOME_OFFSETS),true);
            break;
          #if HAS_ONESTEP_LEVELING
          case MROWS+ADVSET_CASE_PROBEOFF : // Probe Offset >
            Draw_Menu_Line(0, ICON_ProbeOff, GET_TEXT(MSG_ZPROBE_OFFSETS),true);
            break;
          #endif
          case MROWS+ADVSET_CASE_HEPID :    // Nozzle PID
            Draw_Menu_Line(0, ICON_PIDNozzle, "Hotend PID", false);
            break;
          default: break;
        }
      }
      else {
        Move_Highlight(-1, select_advSet.now + MROWS - index_advset);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_advSet.now) {
      case 0: // Back
        checkkey = Control;
        select_control.set(CONTROL_CASE_ADVSET);
        index_control = CONTROL_CASE_ADVSET;
        Draw_Control_Menu();
        break;
      case ADVSET_CASE_HOMEOFF:   // Probe Offsets
        checkkey = HomeOff;
        select_item.reset();
        HMI_ValueStruct.Home_OffX_scaled = home_offset[X_AXIS]*10;
        HMI_ValueStruct.Home_OffY_scaled = home_offset[Y_AXIS]*10;
        Draw_HomeOff_Menu();
        break;
#if HAS_ONESTEP_LEVELING
      case ADVSET_CASE_PROBEOFF:   // Probe Offsets
        checkkey = ProbeOff;
        select_item.reset();
        HMI_ValueStruct.Probe_OffX_scaled = probe.offset.x * 10;
        HMI_ValueStruct.Probe_OffY_scaled = probe.offset.y * 10;
        Draw_ProbeOff_Menu();
        break;
#endif
      case ADVSET_CASE_HEPID:   // Nozzle PID Autotune
        thermalManager.temp_hotend[0].target = ui.material_preset[0].hotend_temp;
        thermalManager.PID_autotune(ui.material_preset[0].hotend_temp, H_E0, 10, true);
        break;
      case ADVSET_CASE_BEDPID:   // Bed PID Autotune
        thermalManager.temp_hotend[0].target = ui.material_preset[0].hotend_temp;
        thermalManager.PID_autotune(ui.material_preset[0].bed_temp, H_BED, 10, true);
        break;
#if HAS_FILAMENT_SENSOR
      case ADVSET_CASE_RUNOUT :  // Runout sensor
        runout.reset();
        runout.enabled = !runout.enabled;
        Draw_Chkb_Line(ADVSET_CASE_RUNOUT + MROWS - index_advset,runout.enabled);
        break;
#endif
      case ADVSET_CASE_PWRLOSSR :  // Power-loss recovery
        recovery.enable(!recovery.enabled);
        Draw_Chkb_Line(ADVSET_CASE_PWRLOSSR + MROWS - index_advset,recovery.enabled);
        break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

/*Home Offset */
void HMI_HomeOff() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
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
      case 0: // Back
        checkkey = AdvSet;
        select_advSet.set(ADVSET_CASE_HOMEOFF);
        Draw_AdvSet_Menu();
        break;
      case 1: // Home Offset X
        checkkey = HomeOffX;
        DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, 1, 216, MBASE(1), HMI_ValueStruct.Home_OffX_scaled);
        EncoderRate.enabled = true;
        break;
      case 2: // Home Offset X
        checkkey = HomeOffY;
        DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, 1, 216, MBASE(2), HMI_ValueStruct.Home_OffY_scaled);
        EncoderRate.enabled = true;
        break;
    }
  }
  DWIN_UpdateLCD();
}

void HMI_HomeOffX(){
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Home_OffX_scaled)) {
      checkkey = HomeOff;
      EncoderRate.enabled = false;
      set_home_offset(X_AXIS, HMI_ValueStruct.Home_OffX_scaled / 10);
      DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(select_item.now), HMI_ValueStruct.Home_OffX_scaled);
      return;
    }
    NOMORE(HMI_ValueStruct.Home_OffX_scaled, 500);
    NOLESS(HMI_ValueStruct.Home_OffX_scaled, -500);
    DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, UNITFDIGITS, 216, MBASE(select_item.now), HMI_ValueStruct.Home_OffX_scaled);
  }
}

void HMI_HomeOffY(){
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Home_OffY_scaled)) {
      checkkey = HomeOff;
      EncoderRate.enabled = false;
      set_home_offset(Y_AXIS, HMI_ValueStruct.Home_OffY_scaled / 10);
      DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(select_item.now), HMI_ValueStruct.Home_OffY_scaled);
      return;
    }
    NOMORE(HMI_ValueStruct.Home_OffY_scaled, 500);
    NOLESS(HMI_ValueStruct.Home_OffY_scaled, -500);
    DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, UNITFDIGITS, 216, MBASE(select_item.now), HMI_ValueStruct.Home_OffY_scaled);
  }
}

#if HAS_ONESTEP_LEVELING
/*Probe Offset */
void HMI_ProbeOff() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
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
      case 0: // Back
        checkkey = AdvSet;
        select_advSet.set(ADVSET_CASE_PROBEOFF);
        Draw_AdvSet_Menu();
        break;
      case 1: // Probe Offset X
        checkkey = ProbeOffX;
        DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, 1, 216, MBASE(1), HMI_ValueStruct.Probe_OffX_scaled);
        EncoderRate.enabled = true;
        break;
      case 2: // Probe Offset X
        checkkey = ProbeOffY;
        DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, 1, 216, MBASE(2), HMI_ValueStruct.Probe_OffY_scaled);
        EncoderRate.enabled = true;
        break;
    }
  }
  DWIN_UpdateLCD();
}

void HMI_ProbeOffX(){
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Probe_OffX_scaled)) {
      checkkey = ProbeOff;
      EncoderRate.enabled = false;
      probe.offset.x = HMI_ValueStruct.Probe_OffX_scaled / 10;
      DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(select_item.now), HMI_ValueStruct.Probe_OffX_scaled);
      return;
    }
    NOMORE(HMI_ValueStruct.Probe_OffX_scaled, 500);
    NOLESS(HMI_ValueStruct.Probe_OffX_scaled, -500);
    DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, UNITFDIGITS, 216, MBASE(select_item.now), HMI_ValueStruct.Probe_OffX_scaled);
  }
}

void HMI_ProbeOffY(){
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Probe_OffY_scaled)) {
      checkkey = ProbeOff;
      EncoderRate.enabled = false;
      probe.offset.y = HMI_ValueStruct.Probe_OffY_scaled / 10;
      DWIN_Draw_Signed_Float(font8x16, Color_Bg_Black, 3, 1, 216, MBASE(select_item.now), HMI_ValueStruct.Probe_OffY_scaled);
      return;
    }
    NOMORE(HMI_ValueStruct.Probe_OffY_scaled, 500);
    NOLESS(HMI_ValueStruct.Probe_OffY_scaled, -500);
    DWIN_Draw_Signed_Float(font8x16, Select_Color, 3, UNITFDIGITS, 216, MBASE(select_item.now), HMI_ValueStruct.Probe_OffY_scaled);
  }
}
#endif

/* Info */
void HMI_Info() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    #if HAS_ONESTEP_LEVELING
      checkkey = Control;
      select_control.set(CONTROL_CASE_INFO);
      Draw_Control_Menu();
    #else
      select_page.set(3);
      Goto_Main_Menu();
    #endif
  }
  DWIN_UpdateLCD();
}

/* Tune */
void HMI_Tune() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_tune.inc(1 + TUNE_CASE_TOTAL)) {
      if (select_tune.now > MROWS && select_tune.now > index_tune) {
        index_tune = select_tune.now;
        Scroll_Menu(DWIN_SCROLL_UP);
        if (index_tune > MROWS) {
          DWIN_Draw_Label(MBASE(TUNE_CASE_FCHNG - 1), GET_TEXT_F(MSG_FILAMENTCHANGE));
          Draw_Menu_Line(TUNE_CASE_FCHNG - 1, ICON_FilMan);
        }
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
      case TUNE_CASE_SPEED: // Print speed
        checkkey = PrintSpeed;
        HMI_ValueStruct.print_speed = feedrate_percentage;
        DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(TUNE_CASE_SPEED + MROWS - index_tune), HMI_ValueStruct.print_speed);
        EncoderRate.enabled = true;
        break;
      #if HAS_HOTEND
        case TUNE_CASE_TEMP: // Nozzle temp
          checkkey = ETemp;
          HMI_ValueStruct.E_Temp = thermalManager.degTargetHotend(0);
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(TUNE_CASE_TEMP + MROWS - index_tune), HMI_ValueStruct.E_Temp);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_HEATED_BED
        case TUNE_CASE_BED: // Bed temp
          checkkey = BedTemp;
          HMI_ValueStruct.Bed_Temp = thermalManager.degTargetBed();
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(TUNE_CASE_BED + MROWS - index_tune), HMI_ValueStruct.Bed_Temp);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_FAN
        case TUNE_CASE_FAN: // Fan speed
          checkkey = FanSpeed;
          HMI_ValueStruct.Fan_speed = thermalManager.fan_speed[0];
          DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(TUNE_CASE_FAN + MROWS - index_tune), HMI_ValueStruct.Fan_speed);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_ZOFFSET_ITEM
        case TUNE_CASE_ZOFF: // Z-offset
          #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
            checkkey = Zoffset;
            HMI_ValueStruct.offset_value = BABY_Z_VAR * 100;
            DWIN_Draw_Signed_Float(font8x16, Select_Color, 2, 2, 202, MBASE(TUNE_CASE_ZOFF + MROWS - index_tune), HMI_ValueStruct.offset_value);
            EncoderRate.enabled = true;
          #else
            // Apply workspace offset, making the current position 0,0,0
            queue.inject_P(PSTR("G92 X0 Y0 Z0"));
            HMI_AudioFeedback();
          #endif
        break;
      #endif
      case TUNE_CASE_FCHNG:
        queue.inject_P(PSTR("M600 B2"));       
        break;

      default: break;
    }
  }
  DWIN_UpdateLCD();
}

#if HAS_PREHEAT

  /* PLA Preheat */
  void HMI_PLAPreheatSetting() {
    ENCODER_DiffState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    // Avoid flicker by updating only the previous menu
    if (encoder_diffState == ENCODER_DIFF_CW) {
      if (select_item.inc(1 + PREHEAT_CASE_TOTAL)) Move_Highlight(1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      if (select_item.dec()) Move_Highlight(-1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      switch (select_item.now) {
        case 0: // Back
          checkkey = TemperatureID;
          select_temp.now = TEMP_CASE_PLA;
          HMI_ValueStruct.show_mode = -1;
          Draw_Temperature_Menu();
          break;
        #if HAS_HOTEND
          case PREHEAT_CASE_TEMP: // Nozzle temperature
            checkkey = ETemp;
            HMI_ValueStruct.E_Temp = ui.material_preset[0].hotend_temp;
            DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(PREHEAT_CASE_TEMP), ui.material_preset[0].hotend_temp);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_HEATED_BED
          case PREHEAT_CASE_BED: // Bed temperature
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = ui.material_preset[0].bed_temp;
            DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(PREHEAT_CASE_BED), ui.material_preset[0].bed_temp);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_FAN
          case PREHEAT_CASE_FAN: // Fan speed
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = ui.material_preset[0].fan_speed;
            DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(PREHEAT_CASE_FAN), ui.material_preset[0].fan_speed);
            EncoderRate.enabled = true;
            break;
        #endif
        #if ENABLED(EEPROM_SETTINGS)
          case 4: { // Save PLA configuration
            const bool success = settings.save();
            HMI_AudioFeedback(success);
          } break;
        #endif
        default: break;
      }
    }
    DWIN_UpdateLCD();
  }

  /* ABS Preheat */
  void HMI_ABSPreheatSetting() {
    ENCODER_DiffState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    // Avoid flicker by updating only the previous menu
    if (encoder_diffState == ENCODER_DIFF_CW) {
      if (select_item.inc(1 + PREHEAT_CASE_TOTAL)) Move_Highlight(1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      if (select_item.dec()) Move_Highlight(-1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      switch (select_item.now) {
        case 0: // Back
          checkkey = TemperatureID;
          select_temp.now = TEMP_CASE_ABS;
          HMI_ValueStruct.show_mode = -1;
          Draw_Temperature_Menu();
          break;
        #if HAS_HOTEND
          case PREHEAT_CASE_TEMP: // Set nozzle temperature
            checkkey = ETemp;
            HMI_ValueStruct.E_Temp = ui.material_preset[1].hotend_temp;
            DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(PREHEAT_CASE_TEMP), ui.material_preset[1].hotend_temp);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_HEATED_BED
          case PREHEAT_CASE_BED: // Set bed temperature
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = ui.material_preset[1].bed_temp;
            DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(PREHEAT_CASE_BED), ui.material_preset[1].bed_temp);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_FAN
          case PREHEAT_CASE_FAN: // Set fan speed
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = ui.material_preset[1].fan_speed;
            DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 3, 216, MBASE(PREHEAT_CASE_FAN), ui.material_preset[1].fan_speed);
            EncoderRate.enabled = true;
            break;
        #endif
        #if ENABLED(EEPROM_SETTINGS)
          case PREHEAT_CASE_SAVE: { // Save ABS configuration
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

/* Max Speed */
void HMI_MaxSpeed() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_item.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_item.dec()) Move_Highlight(-1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (WITHIN(select_item.now, 1, 4)) {
      checkkey = MaxSpeed_value;
      HMI_flag.feedspeed_axis = AxisEnum(select_item.now - 1);
      HMI_ValueStruct.Max_Feedspeed = planner.settings.max_feedrate_mm_s[HMI_flag.feedspeed_axis];
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 4, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Feedspeed);
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

/* Max Acceleration */
void HMI_MaxAcceleration() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_item.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_item.dec()) Move_Highlight(-1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (WITHIN(select_item.now, 1, 4)) {
      checkkey = MaxAcceleration_value;
      HMI_flag.acc_axis = AxisEnum(select_item.now - 1);
      HMI_ValueStruct.Max_Acceleration = planner.settings.max_acceleration_mm_per_s2[HMI_flag.acc_axis];
      DWIN_Draw_IntValue(true, true, 0, font8x16, Color_White, Select_Color, 4, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Acceleration);
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
  /* Max Jerk */
  void HMI_MaxJerk() {
    ENCODER_DiffState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;

    // Avoid flicker by updating only the previous menu
    if (encoder_diffState == ENCODER_DIFF_CW) {
      if (select_item.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW) {
      if (select_item.dec()) Move_Highlight(-1, select_item.now);
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      if (WITHIN(select_item.now, 1, 4)) {
        checkkey = MaxJerk_value;
        HMI_flag.jerk_axis = AxisEnum(select_item.now - 1);
        HMI_ValueStruct.Max_Jerk_scaled = planner.max_jerk[HMI_flag.jerk_axis] * MINUNITMULT;
        DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Jerk_scaled);
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

/* Step */
void HMI_Step() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_item.inc(1 + 3 + ENABLED(HAS_HOTEND))) Move_Highlight(1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_item.dec()) Move_Highlight(-1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (WITHIN(select_item.now, 1, 4)) {
      checkkey = Step_value;
      HMI_flag.step_axis = AxisEnum(select_item.now - 1);
      HMI_ValueStruct.Max_Step_scaled = planner.settings.axis_steps_per_mm[HMI_flag.step_axis] * MINUNITMULT;
      DWIN_Draw_FloatValue(true, true, 0, font8x16, Color_White, Select_Color, 3, UNITFDIGITS, 210, MBASE(select_item.now), HMI_ValueStruct.Max_Step_scaled);
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

void HMI_Popup() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    checkkey = last_checkkey;
    switch (checkkey) {
      case MainMenu     : Goto_Main_Menu(); break;
      case SelectFile   : Draw_Print_File_Menu(); break;
      case Prepare      : Draw_Prepare_Menu(); break;
      case Control      : Draw_Control_Menu(); break;
      case Tune         : Draw_Tune_Menu(); break;
      case PrintProcess : Goto_PrintProcess(); break;
      case AdvSet       : Draw_AdvSet_Menu(); break;
      default           : Goto_Main_Menu(); break;
    }
    return;
  }  
}

void HMI_Init() {
  HMI_SDCardInit();

  for (uint16_t t = 0; t <= 100; t += 2) {
    DWIN_ICON_Show(ICON, ICON_Bar, 15, 260);
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 15 + t * 242 / 100, 260, 257, 280);
    DWIN_UpdateLCD();
    delay(20);
  }
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
      _percentDone = 100;
      Draw_Print_ProgressBar();

      // show print done confirm
      DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 240, DWIN_WIDTH - 1, STATUS_Y-1);
      DWIN_ICON_Show(ICON, ICON_Confirm_E, 86, 273);
    }
    else if (HMI_flag.pause_flag != printingIsPaused()) {
      // print status update
      HMI_flag.pause_flag = printingIsPaused();
      if (HMI_flag.pause_flag) ICON_Continue(); else ICON_Pause();
    }
  }

  // pause after homing
  if (HMI_flag.pause_action && printingIsPaused() && !planner.has_blocks_queued()) {
    HMI_flag.pause_action = false;
    #if ENABLED(PAUSE_HEAT)
      if (sdprint) {
        TERN_(HAS_HOTEND, resume_hotend_temp = thermalManager.temp_hotend[0].target);
        TERN_(HAS_HEATED_BED, resume_bed_temp = thermalManager.temp_bed.target);
      } else {
        TERN_(HAS_HOTEND, resume_hotend_temp = thermalManager.temp_hotend[0].celsius);
        TERN_(HAS_HEATED_BED, resume_bed_temp = thermalManager.temp_bed.celsius);
      }
      thermalManager.disable_all_heaters();
    #endif
    #if DISABLED(PARK_HEAD_ON_PAUSE)
      queue.inject_P(PSTR("G1 F1200 X0 Y0"));
    #endif
  }

  if (checkkey == PrintProcess) { // print process
    
    duration_t elapsed = print_job_timer.duration(); // print timer

    if (sdprint && card.isPrinting()) {
      uint8_t percentDone = card.percentDone();
      static uint8_t last_percentValue = 101;
      if (last_percentValue != percentDone) { // print percent
        last_percentValue = percentDone;
        if (percentDone) {
          _percentDone = percentDone;
          Draw_Print_ProgressBar();
        }
      }
      // Estimate remaining time every 20 seconds
      static millis_t next_remain_time_update = 0;
      if (_percentDone > 1 && ELAPSED(ms, next_remain_time_update) && !HMI_flag.heat_flag) {
          _remain_time = (elapsed.value - dwin_heat_time) / (_percentDone * 0.01f) - (elapsed.value - dwin_heat_time);
          next_remain_time_update += DWIN_REMAIN_TIME_UPDATE_INTERVAL;
          Draw_Print_ProgressRemain();
      }
    }

    // Print time so far
    static uint16_t last_Printtime = 0;
    const uint16_t min = (elapsed.value % 3600) / 60;
    if (last_Printtime != min) { // 1 minute update
      last_Printtime = min;
      Draw_Print_ProgressElapsed();
    }

  }
  else if (dwin_abort_flag && !HMI_flag.home_flag) { // Print Stop
    dwin_abort_flag = false;
    HMI_ValueStruct.print_speed = feedrate_percentage = 100;
    dwin_zoffset = BABY_Z_VAR;
    select_page.set(0);
    Goto_Main_Menu();
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

      // TODO: Get the name of the current file from someplace
      //
      //(void)recovery.interrupted_file_exists();
      DWIN_Draw_CenteredString(false, true, font8x16, Popup_Text_Color, Color_Bg_Window, MENU_CHR_W, 252, card.longest_filename());
      DWIN_UpdateLCD();

      while (recovery_flag) {
        ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
        if (encoder_diffState != ENCODER_DIFF_NO) {
          if (encoder_diffState == ENCODER_DIFF_ENTER) {
            recovery_flag = false;
            if (HMI_flag.select_flag) break;
            TERN_(POWER_LOSS_RECOVERY, queue.inject_P(PSTR("M1000C")));
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
      queue.inject_P(PSTR("M1000"));
      sdprint = true;
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
    case Homing:          break;
    case Leveling:        break;
    case PrintProcess:    HMI_Printing(); break;
    case Print_window:    HMI_PauseOrStop(); break;
    case FilamentMan:     HMI_FilamentMan(); break;
    case AxisMove:        HMI_AxisMove(); break;
    case ManualLev:       HMI_ManualLev(); break;
    #if ENABLED(MESH_BED_LEVELING)
      case ManualMesh:    HMI_ManualMesh(); break;
      case MMeshMoveZ:    HMI_MMeshMoveZ(); break;
    #endif
    case TemperatureID:   HMI_Temperature(); break;
    case Motion:          HMI_Motion(); break;
    case Reboot:          HMI_Reboot(); break;
    case AdvSet:          HMI_AdvSet(); break;
    case HomeOff:         HMI_HomeOff(); break;
    case HomeOffX:        HMI_HomeOffX(); break;
    case HomeOffY:        HMI_HomeOffY(); break;
#if HAS_ONESTEP_LEVELING
    case ProbeOff:        HMI_ProbeOff(); break;
    case ProbeOffX:       HMI_ProbeOffX(); break;
    case ProbeOffY:       HMI_ProbeOffY(); break;
#endif
    case PidRunning:      break;
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
      case Zoffset:       HMI_Zoffset(); break;
    #endif
    #if HAS_HEATED_BED
      case BedTemp:       HMI_BedTemp(); break;
    #endif
    #if HAS_PREHEAT && HAS_FAN
      case FanSpeed:      HMI_FanSpeed(); break;
    #endif
    case PrintSpeed:      HMI_PrintSpeed(); break;
    case PrintFlow:       HMI_PrintFlow(); break;
    case MaxSpeed_value:  HMI_MaxFeedspeedXYZE(); break;
    case MaxAcceleration_value: HMI_MaxAccelerationXYZE(); break;
    #if HAS_CLASSIC_JERK
      case MaxJerk_value: HMI_MaxJerkXYZE(); break;
    #endif
    case Step_value:      HMI_StepXYZE(); break;
    case Popup_Window:    HMI_Popup(); break;
    default: break;
  }
}

void DWIN_StartHoming() {
  HMI_flag.home_flag = true;
  last_checkkey=checkkey;
  checkkey=Homing;
  DWIN_Popup_Window(ICON_BLTouch, "Homing XYZ", "Please wait until done.");
}

void DWIN_CompletedHoming() {
  checkkey=last_checkkey;
  HMI_flag.home_flag = false;
  dwin_zoffset = TERN0(HAS_BED_PROBE, probe.offset.z);
  if (checkkey == Prepare) {
    select_prepare.now = PREPARE_CASE_HOME;
    index_prepare = MROWS;
    Draw_Prepare_Menu();
  }
  else if (checkkey == PrintProcess) {
    Goto_PrintProcess();
  }
  else if (checkkey == Back_Main) {
    HMI_ValueStruct.print_speed = feedrate_percentage = 100;
    planner.finish_and_disable();
    Goto_Main_Menu();
  }
  else if (checkkey == ManualLev) {
    Draw_ManualLev_Menu();
  }
  #if ENABLED(MESH_BED_LEVELING)
  else if (checkkey == ManualMesh) {
    Draw_ManualMesh_Menu();
  } 
  #endif
  else {
    Goto_Main_Menu();
  }
}

void DWIN_CompletedLeveling() {
  if (checkkey == Leveling) Goto_Main_Menu();
}

#if ENABLED(MESH_BED_LEVELING)
void DWIN_ManualMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
  char msg[33];
  sprintf_P(msg, PSTR(S_FMT " %i/%i Z=%.2f"), GET_TEXT(MSG_PROBING_MESH),xpos, ypos, zval);
  DWIN_StatusChanged(msg);
}
#endif

// PID process
void DWIN_PidTuning(pidresult_t result){
  switch (result) {
    case PID_BED_START :
      last_checkkey = checkkey;
      checkkey = PidRunning;
      DWIN_Popup_Window(ICON_TempTooHigh, GET_TEXT(MSG_PID_AUTOTUNE), "for BED is running.");
      break;
    case PID_EXTR_START :
      last_checkkey = checkkey;
      checkkey = PidRunning;
      DWIN_Popup_Window(ICON_TempTooHigh, GET_TEXT(MSG_PID_AUTOTUNE), "for Nozzle is running.");
      break;
    case PID_BAD_EXTRUDER_NUM :
      checkkey = last_checkkey;
      DWIN_Popup_Confirm(ICON_TempTooLow, "PID Autotune failed!", "Bad extruder");
      break;
    case PID_TUNING_TIMEOUT :
      checkkey = last_checkkey;
      DWIN_Popup_Confirm(ICON_TempTooHigh, "Error", GET_TEXT(MSG_PID_TIMEOUT));
      break;
    case PID_TEMP_TOO_HIGH :
      checkkey = last_checkkey;
      DWIN_Popup_Confirm(ICON_TempTooHigh,"PID Autotune failed!","Temperature too high");
      break;
    case PID_DONE :
      checkkey = last_checkkey;
      DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT(MSG_PID_AUTOTUNE), GET_TEXT(MSG_BUTTON_DONE));
      break;
    default :
      checkkey = last_checkkey;
      break;
  }
}

// Update filename on print
void DWIN_Print_Header(const char *text = nullptr) {

static char headertxt[31] = "";  // Print header text

  if (text!=nullptr) {
    const int8_t size = _MIN((unsigned) 30, strlen_P(text));
    LOOP_L_N(i, size) headertxt[i] = text[i];
    headertxt[size+1] = '\0';
  }    
  if (checkkey == PrintProcess){
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 60, DWIN_WIDTH, 60+16);
    DWIN_Draw_CenteredString(false, false, font8x16, Color_White, Color_Bg_Black, MENU_CHR_W, 60, headertxt);
  }
}

// Update Status line
void DWIN_StatusChanged(const char *text) {
  DWIN_Draw_Rectangle(1, Color_Bg_LBlue, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y+20);
  DWIN_Draw_CenteredString(false, false, font8x16, Color_Yellow, Color_Bg_LBlue, MENU_CHR_W, STATUS_Y+2, F(text));
  DWIN_UpdateLCD();
}

// Start a Print Job
void DWIN_Start_Print(bool sd) {
  if (card.isPrinting()) sdprint = true;
  else sdprint = sd;
  _percentDone = 0;
  _remain_time = 0;
  Goto_PrintProcess();
}

//End print job
void DWIN_Stop_Print() {
  if (checkkey == PrintProcess || checkkey == Tune || printingIsActive()) {
    thermalManager.disable_all_heaters();
    thermalManager.zero_fan_speeds();
    HMI_flag.print_finish = true;
    HMI_flag.done_confirm_flag = false;
    Goto_PrintProcess();
  }
}

// Progress Bar update
void DWIN_Progress_Update(uint8_t percent, uint32_t remaining) {
  _percentDone = percent;
  _remain_time = remaining * 60;
  if (checkkey == PrintProcess) {
    Draw_Print_ProgressBar();
    Draw_Print_ProgressRemain();
    Draw_Print_ProgressElapsed();
  }
}

// Filament Runout process
void DWIN_FilamentRunout(const uint8_t extruder){
  DWIN_StatusChanged(GET_TEXT(MSG_RUNOUT_SENSOR));
}

#endif // DWIN_CREALITY_LCD
