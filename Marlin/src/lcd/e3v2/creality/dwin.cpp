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

#include "lockscreen.h"

#ifndef CORP_WEBSITE
  #define CORP_WEBSITE WEBSITE_URL
#endif

#define PAUSE_HEAT

#define USE_STRING_HEADINGS
#define USE_STRING_TITLES

#define MENU_CHAR_LIMIT  24

// Print speed limit
#define MIN_PRINT_SPEED  10
#define MAX_PRINT_SPEED 999

// Print flow limit
#define MIN_PRINT_FLOW   10
#define MAX_PRINT_FLOW   299

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

#define BABY_Z_VAR TERN(HAS_BED_PROBE, probe.offset.z, dwin_zoffset)

// Structs
HMI_value_t HMI_ValueStruct;
HMI_Flag_t HMI_flag{0};
HMI_data_t HMI_data;

millis_t dwin_heat_time = 0;

uint8_t checkkey = MainMenu;
uint8_t last_checkkey = MainMenu;

typedef struct {
  uint8_t now, last;
  void set(uint8_t v) { now = last = v; }
  void reset() { set(0); }
  bool changed() { bool c = (now != last); if (c) last = now; return c; }
  bool dec() { if (now) now--; return changed(); }
  bool inc(uint8_t v) { if (now < (v - 1)) now++; else now = (v - 1); return changed(); }
} select_t;

select_t select_page{0}, select_file{0}, select_print{0}
         , select_control{0}, select_axis{0}, select_temp{0}, select_motion{0}, select_tune{0}
         , select_advset{0}, select_scolor{0}, select_PLA{0}, select_ABS{0}
         , select_speed{0}
         , select_acc{0}
         , select_jerk{0}
         , select_step{0}
         , select_item{0}
         ;

uint8_t index_file     = MROWS,
        index_control  = MROWS,
        index_leveling = MROWS,
        index_tune     = MROWS,
        index_advset   = MROWS,
        index_selcolor = MROWS;

bool dwin_abort_flag = false; // Flag to reset feedrate, return to Home

constexpr float default_max_feedrate[]        = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[]    = DEFAULT_MAX_ACCELERATION;

#if HAS_CLASSIC_JERK
  constexpr float default_max_jerk[]          = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
#endif

static uint8_t _percent_done = 0;
static uint32_t _remain_time = 0;

// Additional Aux Host Support
static bool sdprint = false;

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

void ICON_Print() {
  if (select_page.now == 0) {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Print_1, 17, 130);
    DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 17, 130, 126, 229);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 1, 447, 28, 460, 58, 201);
    else
      DWIN_Frame_AreaCopy(1, 1, 451, 31, 463, 57, 201);
  }
  else {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Print_0, 17, 130);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 1, 405, 28, 420, 58, 201);
    else
      DWIN_Frame_AreaCopy(1, 1, 423, 31, 435, 57, 201);
  }
}

void ICON_Prepare() {
  if (select_page.now == 1) {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Prepare_1, 145, 130);
    DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 145, 130, 254, 229);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 31, 447, 58, 460, 186, 201);
    else
      DWIN_Frame_AreaCopy(1, 33, 451, 82, 466, 175, 201);
  }
  else {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Prepare_0, 145, 130);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 31, 405, 58, 420, 186, 201);
    else
      DWIN_Frame_AreaCopy(1, 33, 423, 82, 438, 175, 201);
  }
}

void ICON_Control() {
  if (select_page.now == 2) {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Control_1, 17, 246);
    DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 17, 246, 126, 345);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 61, 447, 88, 460, 58, 318);
    else
      DWIN_Frame_AreaCopy(1, 85, 451, 132, 463, 48, 318);
  }
  else {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Control_0, 17, 246);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 61, 405, 88, 420, 58, 318);
    else
      DWIN_Frame_AreaCopy(1, 85, 423, 132, 434, 48, 318);
  }
}

void ICON_StartInfo(bool show) {
  if (show) {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Info_1, 145, 246);
    DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 145, 246, 254, 345);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 91, 447, 118, 460, 186, 318);
    else
      DWIN_Frame_AreaCopy(1, 132, 451, 159, 466, 186, 318);
  }
  else {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Info_0, 145, 246);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 91, 405, 118, 420, 186, 318);
    else
      DWIN_Frame_AreaCopy(1, 132, 423, 159, 435, 186, 318);
  }
}

void ICON_Leveling(bool show) {
  if (show) {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Leveling_1, 145, 246);
    DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 145, 246, 254, 345);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 211, 447, 238, 460, 186, 318);
    else
      DWIN_Frame_AreaCopy(1, 84, 437, 120,  449, 182, 318);
  }
  else {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Leveling_0, 145, 246);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 211, 405, 238, 420, 186, 318);
    else
      DWIN_Frame_AreaCopy(1, 84, 465, 120, 478, 182, 318);
  }
}

void ICON_Tune() {
  if (select_print.now == 0) {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Setup_1, 8, 252);
    DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 8, 252, 87, 351);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 121, 447, 148, 458, 34, 325);
    else
      DWIN_Frame_AreaCopy(1,   0, 466,  34, 476, 31, 325);
  }
  else {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Setup_0, 8, 252);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 121, 405, 148, 420, 34, 325);
    else
      DWIN_Frame_AreaCopy(1,   0, 438,  32, 448, 31, 325);
  }
}

void ICON_Pause() {
  if (select_print.now == 1) {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Pause_1, 96, 252);
    DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 96, 252, 175, 351);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 181, 447, 208, 459, 124, 325);
    else
      DWIN_Frame_AreaCopy(1, 177, 451, 216, 462, 116, 325);
  }
  else {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Pause_0, 96, 252);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 181, 405, 208, 420, 124, 325);
    else
      DWIN_Frame_AreaCopy(1, 177, 423, 215, 433, 116, 325);
  }
}

void ICON_Continue() {
  if (select_print.now == 1) {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Continue_1, 96, 252);
    DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 96, 252, 175, 351);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 1, 447, 28, 460, 124, 325);
    else
      DWIN_Frame_AreaCopy(1, 1, 452, 32, 464, 121, 325);
  }
  else {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Continue_0, 96, 252);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 1, 405, 28, 420, 124, 325);
    else
      DWIN_Frame_AreaCopy(1, 1, 424, 31, 434, 121, 325);
  }
}

void ICON_Stop() {
  if (select_print.now == 2) {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Stop_1, 184, 252);
    DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 184, 252, 263, 351);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 151, 447, 178, 459, 210, 325);
    else
      DWIN_Frame_AreaCopy(1, 218, 452, 249, 466, 209, 325);
  }
  else {
    DWIN_ICON_Show(1, 0, 0, ICON, ICON_Stop_0, 184, 252);
    if (HMI_IsChinese())
      DWIN_Frame_AreaCopy(1, 151, 405, 178, 420, 210, 325);
    else
      DWIN_Frame_AreaCopy(1, 218, 423, 247, 436, 209, 325);
  }
}

void Draw_More_Icon(const uint8_t line) {
  DWINUI::Draw_Icon(ICON_More, 226, MBASE(line) - 3);
}

void Draw_Menu_Cursor(const uint8_t line) {
  DWIN_Draw_Rectangle(1, HMI_data.Cursor_color, 0, MBASE(line) - 18, 14, MBASE(line + 1) - 20);
}

void Erase_Menu_Cursor(const uint8_t line) {
  DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, MBASE(line) - 18, 14, MBASE(line + 1) - 20);
}

void Move_Highlight(const int16_t from, const uint16_t newline) {
  Erase_Menu_Cursor(newline - from);
  Draw_Menu_Cursor(newline);
}

void Add_Menu_Line() {
  Move_Highlight(1, MROWS);
  DWIN_Draw_Line(HMI_data.SplitLine_Color, 16, MBASE(MROWS + 1) - 20, 256, MBASE(MROWS + 1) - 19);
}

void Scroll_Menu(const uint8_t dir) {
  DWIN_Frame_AreaMove(1, dir, MLINE, HMI_data.Background_Color, 0, 31, DWIN_WIDTH, 349);
  switch (dir) {
    case DWIN_SCROLL_DOWN: Move_Highlight(-1, 0); break;
    case DWIN_SCROLL_UP:   Add_Menu_Line(); break;
  }
}

inline uint16_t nr_sd_menu_items() {
  return card.get_num_Files() + !card.flag.workDirIsRoot;
}

void Draw_Menu_Icon(const uint8_t line, const uint8_t icon) {
  DWINUI::Draw_Icon(icon, 26, MBASE(line) - 3);
}

void Erase_Menu_Text(const uint8_t line) {
  DWIN_Draw_Rectangle(1, HMI_data.Background_Color, LBLX, MBASE(line) - 14, 271, MBASE(line) + 28);
}

void Draw_Menu_Item(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr, bool more=false) {
  if (label) DWINUI::Draw_String(LBLX, MBASE(line) - 1, (char*)label);
  if (icon) Draw_Menu_Icon(line, icon);
  if (more) Draw_More_Icon(line);
}

void Draw_Menu_Line(const uint8_t line, const uint8_t icon=0, const char * const label=nullptr, bool more=false) {
  Draw_Menu_Item(line, icon, label, more);
  DWIN_Draw_Line(HMI_data.SplitLine_Color, 16, MBASE(line) + 33, 256, MBASE(line) + 33);
}

void Draw_Chkb_Line(const uint8_t line, const bool mode) {
  DWINUI::Draw_Checkbox(HMI_data.Text_Color, HMI_data.Background_Color, 225, MBASE(line) - 1, mode);
}

void Draw_Menu_IntValue(uint16_t bcolor, const uint8_t line, uint8_t iNum, const uint16_t value=0) {
  DWINUI::Draw_Int(HMI_data.Text_Color, bcolor, iNum , 216, MBASE(line) - 1, value);
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

template<typename T>
inline bool Apply_Encoder(const ENCODER_DiffState &encoder_diffState, T &valref) {
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
#define TUNE_CASE_FLOW (TUNE_CASE_ZOFF + 1)
#define TUNE_CASE_FCHNG (TUNE_CASE_FLOW + ENABLED(ADVANCED_PAUSE_FEATURE))
#define TUNE_CASE_LOCK (TUNE_CASE_FCHNG + 1)
#define TUNE_CASE_TOTAL TUNE_CASE_LOCK

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
#define ADVSET_CASE_BRIGHTNESS (ADVSET_CASE_PWRLOSSR + 1)
#define ADVSET_CASE_SCOLOR    (ADVSET_CASE_BRIGHTNESS + 1)
#define ADVSET_CASE_SOUND     (ADVSET_CASE_SCOLOR + ENABLED(SOUND_MENU_ITEM))
#define ADVSET_CASE_LOCK      (ADVSET_CASE_SOUND + 1)
#define ADVSET_CASE_TOTAL     ADVSET_CASE_LOCK


#define SCOLOR_CASE_LOADDEF   1
#define SCOLOR_CASE_BACKG     (SCOLOR_CASE_LOADDEF + 1)
#define SCOLOR_CASE_CURSOR    (SCOLOR_CASE_BACKG + 1)
#define SCOLOR_CASE_TITLEGB   (SCOLOR_CASE_CURSOR +1)
#define SCOLOR_CASE_TITLETXT  (SCOLOR_CASE_TITLEGB +1)
#define SCOLOR_CASE_TEXT      (SCOLOR_CASE_TITLETXT + 1)
#define SCOLOR_CASE_SELECT    (SCOLOR_CASE_TEXT + 1)
#define SCOLOR_CASE_SLINE     (SCOLOR_CASE_SELECT + 1)
#define SCOLOR_CASE_HIGHLIGHT (SCOLOR_CASE_SLINE + 1)
#define SCOLOR_CASE_STATUSBG  (SCOLOR_CASE_HIGHLIGHT + 1)
#define SCOLOR_CASE_STATUSTXT (SCOLOR_CASE_STATUSBG + 1)
#define SCOLOR_CASE_POPUPBG   (SCOLOR_CASE_STATUSTXT + 1)
#define SCOLOR_CASE_POPUPTXT  (SCOLOR_CASE_POPUPBG + 1)
#define SCOLOR_CASE_ALERTBG   (SCOLOR_CASE_POPUPTXT + 1)
#define SCOLOR_CASE_ALERTTXT  (SCOLOR_CASE_ALERTBG + 1)
#define SCOLOR_CASE_PERCNTTXT (SCOLOR_CASE_ALERTTXT + 1)
#define SCOLOR_CASE_BARFILL   (SCOLOR_CASE_PERCNTTXT + 1)
#define SCOLOR_CASE_INDICATOR (SCOLOR_CASE_BARFILL +1)
#define SCOLOR_CASE_COORDINATE (SCOLOR_CASE_INDICATOR + 1)
#define SCOLOR_CASE_TOTAL     SCOLOR_CASE_COORDINATE

//
// Draw Menus
//

void DWIN_Draw_Label(const uint16_t y, char *string) {
  DWINUI::Draw_String(LBLX, y, string);
}
void DWIN_Draw_Label(const uint16_t y, const __FlashStringHelper *title) {
  DWIN_Draw_Label(y, (char*)title);
}

void Item_Control_Info(const uint16_t line) {
  if (HMI_IsChinese())
    DWIN_Frame_AreaCopy(1, 231, 104, 258, 116, LBLX, line);
  else {
    #ifdef USE_STRING_TITLES
      DWIN_Draw_Label(line, GET_TEXT_F(MSG_INFO_SCREEN));
    #else
      DWIN_Frame_AreaCopy(1, 0, 104, 24, 114, LBLX, line);
    #endif
  }
}

void Draw_Control_Menu() {
  DWINUI::ClearMenu();

  #if CONTROL_CASE_TOTAL >= 6
    const int16_t scroll = MROWS - index_control; // Scrolled-up lines
    #define CSCROL(L) (scroll + (L))
  #else
    #define CSCROL(L) (L)
  #endif
  #define CLINE(L)  MBASE(CSCROL(L))
  #define CVISI(L)  WITHIN(CSCROL(L), 0, MROWS)

  if (CVISI(0)) Draw_Back_First(select_control.now == 0);                         // < Back

  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 103, 1, 130, 14);                                     // "Control"

    DWIN_Frame_AreaCopy(1,  57, 104,  84, 116, LBLX, CLINE(CONTROL_CASE_TEMP));   // Temperature >
    DWIN_Frame_AreaCopy(1,  87, 104, 114, 116, LBLX, CLINE(CONTROL_CASE_MOVE));   // Motion >

    #if ENABLED(EEPROM_SETTINGS)
      DWIN_Frame_AreaCopy(1, 117, 104, 172, 116, LBLX, CLINE(CONTROL_CASE_SAVE));   // Store Configuration
      DWIN_Frame_AreaCopy(1, 174, 103, 229, 116, LBLX, CLINE(CONTROL_CASE_LOAD));   // Read Configuration
      DWIN_Frame_AreaCopy(1,   1, 118,  56, 131, LBLX, CLINE(CONTROL_CASE_RESET));  // Reset Configuration
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Title.ShowCaption(GET_TEXT_F(MSG_CONTROL));
    #else
      Title.FrameCopy(1, 128, 2, 176, 12);                                         // "Control"
    #endif
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
        if (CVISI(CONTROL_CASE_SAVE)) DWIN_Frame_AreaCopy(1, 148,  89, 268, 101, LBLX     , CLINE(CONTROL_CASE_SAVE));  // "Store Configuration"
        if (CVISI(CONTROL_CASE_LOAD)) {
          DWIN_Frame_AreaCopy(1,  26, 104,  57, 114, LBLX     , CLINE(CONTROL_CASE_LOAD));  // "Read"
          DWIN_Frame_AreaCopy(1, 182,  89, 268, 101, LBLX + 34, CLINE(CONTROL_CASE_LOAD));  // "Configuration"
        }
        if (CVISI(CONTROL_CASE_RESET)) {
          DWIN_Frame_AreaCopy(1,  59, 104,  93, 114, LBLX     , CLINE(CONTROL_CASE_RESET)); // "Reset"
          DWIN_Frame_AreaCopy(1, 182,  89, 268, 101, LBLX + 37, CLINE(CONTROL_CASE_RESET)); // "Configuration"
        }
      #endif
    #endif
  }

  if (CVISI(CONTROL_CASE_ADVSET)) {
    DWIN_Draw_Label(CLINE(CONTROL_CASE_ADVSET), GET_TEXT_F(MSG_ADVANCED_SETTINGS));  // Advanced Settings
  }

  if (CVISI(CONTROL_CASE_INFO)) Item_Control_Info(CLINE(CONTROL_CASE_INFO));

  if (select_control.now && CVISI(select_control.now))
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

  _TEMP_ICON(CONTROL_CASE_TEMP, ICON_Temperature, true);
  _TEMP_ICON(CONTROL_CASE_MOVE, ICON_Motion, true);

  #if ENABLED(EEPROM_SETTINGS)
    _TEMP_ICON(CONTROL_CASE_SAVE, ICON_WriteEEPROM, false);
    _TEMP_ICON(CONTROL_CASE_LOAD, ICON_ReadEEPROM, false);
    _TEMP_ICON(CONTROL_CASE_RESET, ICON_ResumeEEPROM, false);
  #endif

  _TEMP_ICON(CONTROL_CASE_ADVSET, ICON_AdvSet, true);
  _TEMP_ICON(CONTROL_CASE_INFO, ICON_Info, true);
}

void Draw_Tune_Menu() {
  DWINUI::ClearMenu();

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
    #if TUNE_CASE_TOTAL >= 6
      const int16_t scroll = MROWS - index_tune; // Scrolled-up lines
      #define TSCROL(L) (scroll + (L))
    #else
      #define TSCROL(L) (L)
    #endif
    #define TLINE(L)  MBASE(TSCROL(L))
    #define TVISI(L)  WITHIN(TSCROL(L), 0, MROWS)

    #ifdef USE_STRING_HEADINGS
      Title.ShowCaption(GET_TEXT_F(MSG_TUNE));
    #else
      Title.FrameCopy(1, 94, 2, 126, 12);
    #endif

    if (TVISI(0)) Draw_Back_First(select_tune.now == 0);
    #ifdef USE_STRING_TITLES
      if (TVISI(TUNE_CASE_SPEED)) DWIN_Draw_Label(TLINE(TUNE_CASE_SPEED), GET_TEXT_F(MSG_SPEED));
      #if HAS_HOTEND
        if (TVISI(TUNE_CASE_TEMP)) DWIN_Draw_Label(TLINE(TUNE_CASE_TEMP), GET_TEXT_F(MSG_UBL_SET_TEMP_HOTEND));
      #endif
      #if HAS_HEATED_BED
        if (TVISI(TUNE_CASE_BED)) DWIN_Draw_Label(TLINE(TUNE_CASE_BED), GET_TEXT_F(MSG_UBL_SET_TEMP_BED));
      #endif
      #if HAS_FAN
        if (TVISI(TUNE_CASE_FAN)) DWIN_Draw_Label(TLINE(TUNE_CASE_FAN), GET_TEXT_F(MSG_FAN_SPEED));
      #endif
      if (TVISI(TUNE_CASE_ZOFF)) DWIN_Draw_Label(TLINE(TUNE_CASE_ZOFF), GET_TEXT_F(MSG_ZPROBE_ZOFFSET));
    #else
      if (TVISI(TUNE_CASE_SPEED)) DWIN_Frame_AreaCopy(1, 1, 179, 92, 190, LBLX, TLINE(TUNE_CASE_SPEED));     // Print speed
      #if HAS_HOTEND
        if (TVISI(TUNE_CASE_TEMP)) DWIN_Frame_AreaCopy(1, 197, 104, 238, 114, LBLX, TLINE(TUNE_CASE_TEMP));  // Hotend...
        if (TVISI(TUNE_CASE_TEMP)) DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 44, TLINE(TUNE_CASE_TEMP)); // ...Temperature
      #endif
      #if HAS_HEATED_BED
        if (TVISI(TUNE_CASE_BED)) DWIN_Frame_AreaCopy(1, 240, 104, 264, 114, LBLX, TLINE(TUNE_CASE_BED));    // Bed...
        if (TVISI(TUNE_CASE_BED)) DWIN_Frame_AreaCopy(1, 1, 89, 83, 101, LBLX + 27, TLINE(TUNE_CASE_BED));   // ...Temperature
      #endif
      #if HAS_FAN
        if (TVISI(TUNE_CASE_FAN)) DWIN_Frame_AreaCopy(1, 0, 119, 64, 132, LBLX, TLINE(TUNE_CASE_FAN));       // Fan speed
      #endif
      #if HAS_ZOFFSET_ITEM
        if (TVISI(TUNE_CASE_ZOFF)) DWIN_Frame_AreaCopy(1, 93, 179, 141, 189, LBLX, TLINE(TUNE_CASE_ZOFF));   // Z-offset
      #endif
    #endif

    if (select_tune.now && TVISI(select_tune.now))
      Draw_Menu_Cursor(TSCROL(select_tune.now));

    if (TVISI(TUNE_CASE_SPEED)) {
      Draw_Menu_Line(TSCROL(TUNE_CASE_SPEED), ICON_Speed);
      DWINUI::Draw_Int(3, 216, TLINE(TUNE_CASE_SPEED), feedrate_percentage);
    }
    #if HAS_HOTEND
      if (TVISI(TUNE_CASE_TEMP)) {
        Draw_Menu_Line(TSCROL(TUNE_CASE_TEMP), ICON_HotendTemp);
        DWINUI::Draw_Int(3, 216, TLINE(TUNE_CASE_TEMP), thermalManager.degTargetHotend(0));
      }
    #endif
    #if HAS_HEATED_BED
      if (TVISI(TUNE_CASE_BED)) {
        Draw_Menu_Line(TSCROL(TUNE_CASE_BED), ICON_BedTemp);
        DWINUI::Draw_Int(3, 216, TLINE(TUNE_CASE_BED), thermalManager.degTargetBed());
      }
    #endif
    #if HAS_FAN
      if (TVISI(TUNE_CASE_FAN)) {
        Draw_Menu_Line(TSCROL(TUNE_CASE_FAN), ICON_FanSpeed);
        DWINUI::Draw_Int(3, 216, TLINE(TUNE_CASE_FAN), thermalManager.fan_speed[0]);
      }
    #endif
    #if HAS_ZOFFSET_ITEM
      if (TVISI(TUNE_CASE_ZOFF)) {
        Draw_Menu_Line(TSCROL(TUNE_CASE_ZOFF), ICON_Zoffset);
        DWINUI::Draw_Signed_Float(2, 2, 202, TLINE(TUNE_CASE_ZOFF), BABY_Z_VAR * 100);
      }
    #endif
    if (TVISI(TUNE_CASE_FLOW)) {
      Draw_Menu_Line(TSCROL(TUNE_CASE_FLOW), ICON_Flow, GET_TEXT(MSG_FLOW),false);  // Flow rate
      Draw_Menu_IntValue(HMI_data.Background_Color, TSCROL(TUNE_CASE_FLOW), 3, planner.flow_percentage[0]);
    }
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      if (TVISI(TUNE_CASE_FCHNG)) Draw_Menu_Line(TSCROL(TUNE_CASE_FCHNG), ICON_FilMan, GET_TEXT(MSG_FILAMENTCHANGE));
    #endif
    if (TVISI(TUNE_CASE_LOCK)) Draw_Menu_Line(TSCROL(TUNE_CASE_LOCK), ICON_Lock, "Screen Lock", true);
  }
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
  DWINUI::ClearMenu();

  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 1, 16, 28, 28);                                     // "Motion"
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
      Title.ShowCaption(GET_TEXT_F(MSG_MOTION));
    #else
      Title.FrameCopy(1, 144, 16, 189, 26);                                        // "Motion"
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

  Draw_Menu_Line(MOTION_CASE_FLOW, ICON_Flow, GET_TEXT(MSG_FLOW),false);  // Flow rate
  DWINUI::Draw_Int(3, 216, MBASE(MOTION_CASE_FLOW),planner.flow_percentage[0]);

}

//
// Draw Popup Windows
//

inline void Draw_Popup_Bkgd_60() {
  DWIN_Draw_Rectangle(1, HMI_data.PopupBg_color, 14, 60, 258, 330);
  DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 14, 60, 258, 330);
}

inline void Draw_Popup_Bkgd_105() {
  DWIN_Draw_Rectangle(1, HMI_data.PopupBg_color, 14, 105, 258, 374);
  DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 14, 105, 258, 374);
}

void Clear_Popup_Area() {
  DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, 31, DWIN_WIDTH, DWIN_HEIGHT);
}

void DWIN_Draw_Popup(uint8_t icon, const char * const msg1, const char * const msg2, uint8_t button) {
  DWINUI::ClearMenu();
  Draw_Popup_Bkgd_60();
  if (icon) DWINUI::Draw_Icon(icon, 101, 105);
  if (msg1) DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 210, msg1);
  if (msg2) DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 240, msg2);
  if (button) DWINUI::Draw_Icon(button, 86, 280);
}

void DWIN_Popup_Confirm(uint8_t icon, const char * const msg1, const char * const msg2) {
  HMI_SaveProcessID(WaitResponse);
  DWIN_Draw_Popup(icon, msg1, msg2, ICON_Confirm_E);  // Button Confirm
  DWIN_UpdateLCD();
}

void DWIN_Popup_Continue(uint8_t icon, const char * const msg1, const char * const msg2) {
  HMI_SaveProcessID(WaitResponse);
  DWIN_Draw_Popup(icon, msg1, msg2, ICON_Continue_E);  // Button Continue
  DWIN_UpdateLCD();
}

#if HAS_HOTEND

  void Popup_Window_ETempTooLow() {
    if (HMI_IsChinese()) {
      HMI_SaveProcessID(WaitResponse);
      DWINUI::ClearMenu();
      Draw_Popup_Bkgd_60();
      DWINUI::Draw_Icon(ICON_TempTooLow, 102, 105);
      DWIN_Frame_AreaCopy(1, 103, 371, 136, 386, 69, 240);
      DWIN_Frame_AreaCopy(1, 170, 371, 270, 386, 102, 240);
      DWINUI::Draw_Icon(ICON_Confirm_C, 86, 280);
      DWIN_UpdateLCD();
    }
    else
      DWIN_Popup_Confirm(ICON_TempTooLow, "Nozzle is too cold", "Preheat the hotend");
  }

#endif

void Popup_Window_Resume() {
  Clear_Popup_Area();
  Draw_Popup_Bkgd_105();
  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, 160, 338, 235, 354, 98, 135);
    DWIN_Frame_AreaCopy(1, 103, 321, 271, 335, 52, 192);
    DWINUI::Draw_Icon(ICON_Cancel_C,    26, 307);
    DWINUI::Draw_Icon(ICON_Continue_C, 146, 307);
  }
  else {
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 115, F("Continue Print"));
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 192, F("It looks like the last"));
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 212, F("file was interrupted."));
    DWINUI::Draw_Icon(ICON_Cancel_E,    26, 307);
    DWINUI::Draw_Icon(ICON_Continue_E, 146, 307);
  }
}

void Draw_Select_Highlight(const bool sel) {
  HMI_flag.select_flag = sel;
  const uint16_t c1 = sel ? HMI_data.Highlight_Color : HMI_data.PopupBg_color,
                 c2 = sel ? HMI_data.PopupBg_color : HMI_data.Highlight_Color;
  DWIN_Draw_Rectangle(0, c1, 25, 279, 126, 318);
  DWIN_Draw_Rectangle(0, c1, 24, 278, 127, 319);
  DWIN_Draw_Rectangle(0, c2, 145, 279, 246, 318);
  DWIN_Draw_Rectangle(0, c2, 144, 278, 247, 319);
}

void Popup_window_PauseOrStop() {
  if (HMI_IsChinese()) {
    DWINUI::ClearMenu();
    Draw_Popup_Bkgd_60();
         if (select_print.now == 1) DWIN_Frame_AreaCopy(1, 237, 338, 269, 356, 98, 150);
    else if (select_print.now == 2) DWIN_Frame_AreaCopy(1, 221, 320, 253, 336, 98, 150);
    DWIN_Frame_AreaCopy(1, 220, 304, 264, 319, 130, 150);
    DWINUI::Draw_Icon(ICON_Confirm_C, 26, 280);
    DWINUI::Draw_Icon(ICON_Cancel_C, 146, 280);
  }
  else {
    DWIN_Draw_Popup(ICON_BLTouch, "Please confirm",(select_print.now == 1) ? GET_TEXT(MSG_PAUSE_PRINT) : GET_TEXT(MSG_STOP_PRINT));
    DWINUI::Draw_Icon(ICON_Confirm_E, 26, 280);
    DWINUI::Draw_Icon(ICON_Cancel_E, 146, 280);
  }
  Draw_Select_Highlight(true);
}

#if HAS_HOTEND || HAS_HEATED_BED
  void DWIN_Popup_Temperature(const bool toohigh) {
    Clear_Popup_Area();
    Draw_Popup_Bkgd_105();
    if (toohigh) {
      DWINUI::Draw_Icon(ICON_TempTooHigh, 102, 165);
      if (HMI_IsChinese()) {
        DWIN_Frame_AreaCopy(1, 103, 371, 237, 386, 52, 285);
        DWIN_Frame_AreaCopy(1, 151, 389, 185, 402, 187, 285);
        DWIN_Frame_AreaCopy(1, 189, 389, 271, 402, 95, 310);
      }
      else {
        DWINUI::Draw_String(HMI_data.PopupTxt_Color, 36, 300, F("Nozzle or Bed temperature"));
        DWINUI::Draw_String(HMI_data.PopupTxt_Color, 92, 300, F("is too high"));
      }
    }
    else {
      DWINUI::Draw_Icon(ICON_TempTooLow, 102, 165);
      if (HMI_IsChinese()) {
        DWIN_Frame_AreaCopy(1, 103, 371, 270, 386, 52, 285);
        DWIN_Frame_AreaCopy(1, 189, 389, 271, 402, 95, 310);
      }
      else {
        DWINUI::Draw_String(HMI_data.PopupTxt_Color, 36, 300, F("Nozzle or Bed temperature"));
        DWINUI::Draw_String(HMI_data.PopupTxt_Color, 92, 300, F("is too low"));
      }
    }
  }

#endif

void Draw_Print_Labels() {
  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 30,  1,  71, 14);  // Printing
    DWIN_Frame_AreaCopy(1,  0, 72,  63, 86,  41, 188);  // Printing Time
    DWIN_Frame_AreaCopy(1, 65, 72, 128, 86, 176, 188);  // Remain
  }
  else {
    #ifdef USE_STRING_TITLES
      Title.ShowCaption(GET_TEXT(MSG_PRINTING));
      DWINUI::Draw_String(46, 173, F("Print Time"));
      DWINUI::Draw_String(181, 173, F("Remain"));
    #else
      Title.FrameCopy(1, 40,  2,  92, 14);  // Printing
      DWIN_Frame_AreaCopy(1,  0, 44,  96, 58,  41, 188);  // Printing Time
      DWIN_Frame_AreaCopy(1, 98, 44, 152, 58, 176, 188);  // Remain
    #endif
  }
}

void Draw_Print_ProgressBar() {
  DWINUI::Draw_Icon(ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, HMI_data.Barfill_Color, 16 + _percent_done * 240 / 100, 93, 256, 113);
  DWINUI::Draw_Int(HMI_data.PercentTxt_Color, HMI_data.Background_Color, 3, 117, 133, _percent_done);
  DWINUI::Draw_String(HMI_data.PercentTxt_Color, 142, 133, F("%"));
}

void Draw_Print_ProgressElapsed() {
  duration_t elapsed = print_job_timer.duration(); // print timer
  DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 2, 42, 212, elapsed.value / 3600);
  DWINUI::Draw_String(58, 212, F(":"));
  DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 2, 66, 212, (elapsed.value % 3600) / 60);
}

void Draw_Print_ProgressRemain() {
  DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 2, 176, 212, _remain_time / 3600);
  DWINUI::Draw_String(192, 212, F(":"));
  DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 2, 200, 212, (_remain_time % 3600) / 60);
}

void Draw_PrintProcess() {
  DWINUI::ClearMenu();
  Draw_Print_Labels();

  ICON_Tune();
  if (printingIsPaused() || HMI_flag.pause_action) ICON_Continue(); else ICON_Pause();
  ICON_Stop();

  DWIN_Print_Header(sdprint ? card.longest_filename() : nullptr);

  DWINUI::Draw_Icon(ICON_PrintTime, 17, 193);
  DWINUI::Draw_Icon(ICON_RemainTime, 150, 191);

  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();
}

void Goto_PrintProcess() {
  checkkey = PrintProcess;
  Draw_PrintProcess();
}

void Draw_PrintDone() {
  // show percent bar and value
  _percent_done = 100;
  _remain_time = 0;

  DWINUI::ClearMenu();
  DWIN_Print_Header(sdprint ? card.longest_filename() : nullptr);
  Draw_Print_Labels();
  DWINUI::Draw_Icon(ICON_PrintTime, 15, 173);
  DWINUI::Draw_Icon(ICON_RemainTime, 150, 171);
  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();

  // show print done confirm
  DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, 240, DWIN_WIDTH - 1, STATUS_Y-1);
  DWINUI::Draw_Icon(HMI_IsChinese() ? ICON_Confirm_C : ICON_Confirm_E, 86, 283);
}

void Draw_Main_Menu() {
  DWINUI::ClearMenu();

  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 2, 2, 27, 14); // "Home"
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Title.ShowCaption(MACHINE_NAME);
    #else
      Title.FrameCopy(1, 0, 2, 39, 12);
    #endif
  }

  DWINUI::Draw_Icon(ICON_LOGO, 71, 52);  // CREALITY logo

  ICON_Print();
  ICON_Prepare();
  ICON_Control();
  TERN(HAS_ONESTEP_LEVELING, ICON_Leveling, ICON_StartInfo)(select_page.now == 3);
}

void Goto_Main_Menu() {
  checkkey = MainMenu;
  DWIN_StatusChanged(nullptr);
  Draw_Main_Menu();
}

inline ENCODER_DiffState get_encoder_state() {
  static millis_t Encoder_ms = 0;
  const millis_t ms = millis();
  if (PENDING(ms, Encoder_ms)) return ENCODER_DIFF_NO;
  const ENCODER_DiffState state = Encoder_ReceiveAnalyze();
  if (state != ENCODER_DIFF_NO) Encoder_ms = ms + ENCODER_WAIT_MS;
  return state;
}

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
          return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(temp_line), ui.material_preset[0].hotend_temp);
        }
        else if (HMI_ValueStruct.show_mode == -3) {
          checkkey = ABSPreheat;
          ui.material_preset[1].hotend_temp = HMI_ValueStruct.E_Temp;
          return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(temp_line), ui.material_preset[1].hotend_temp);
        }
        else if (HMI_ValueStruct.show_mode == -1) // Temperature
          checkkey = TemperatureID;
        else
          checkkey = Tune;
        thermalManager.setTargetHotend(HMI_ValueStruct.E_Temp, 0);
        return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(temp_line), HMI_ValueStruct.E_Temp);
      }
      // E_Temp limit
      LIMIT(HMI_ValueStruct.E_Temp, HEATER_0_MINTEMP, thermalManager.hotend_max_target(0));
      // E_Temp value
      DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(temp_line), HMI_ValueStruct.E_Temp);
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
          return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(bed_line), ui.material_preset[0].bed_temp);
        }
        else if (HMI_ValueStruct.show_mode == -3) {
          checkkey = ABSPreheat;
          ui.material_preset[1].bed_temp = HMI_ValueStruct.Bed_Temp;
          return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(bed_line), ui.material_preset[1].bed_temp);
        }
        else if (HMI_ValueStruct.show_mode == -1)
          checkkey = TemperatureID;
        else
          checkkey = Tune;
        thermalManager.setTargetBed(HMI_ValueStruct.Bed_Temp);
        return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(bed_line), HMI_ValueStruct.Bed_Temp);
      }
      // Bed_Temp limit
      LIMIT(HMI_ValueStruct.Bed_Temp, BED_MINTEMP, BED_MAX_TARGET);
      // Bed_Temp value
      DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(bed_line), HMI_ValueStruct.Bed_Temp);
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
          return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(fan_line), ui.material_preset[0].fan_speed);
        }
        else if (HMI_ValueStruct.show_mode == -3) {
          checkkey = ABSPreheat;
          ui.material_preset[1].fan_speed = HMI_ValueStruct.Fan_speed;
          return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(fan_line), ui.material_preset[1].fan_speed);
        }
        else if (HMI_ValueStruct.show_mode == -1)
          checkkey = TemperatureID;
        else
          checkkey = Tune;
        thermalManager.set_fan_speed(0, HMI_ValueStruct.Fan_speed);
        return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(fan_line), HMI_ValueStruct.Fan_speed);
      }
      // Fan_speed limit
      LIMIT(HMI_ValueStruct.Fan_speed, 0, 255);
      // Fan_speed value
      DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(fan_line), HMI_ValueStruct.Fan_speed);
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
      return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(select_tune.now + MROWS - index_tune), HMI_ValueStruct.print_speed);
    }
    // print_speed limit
    LIMIT(HMI_ValueStruct.print_speed, MIN_PRINT_SPEED, MAX_PRINT_SPEED);
    // print_speed value
    DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(select_tune.now + MROWS - index_tune), HMI_ValueStruct.print_speed);
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
      return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
    }
    // MaxFeedspeed limit
    if (WITHIN(HMI_flag.feedspeed_axis, X_AXIS, LAST_AXIS))
      NOMORE(HMI_ValueStruct.Max_Feedspeed, default_max_feedrate[HMI_flag.feedspeed_axis] * 2);
    if (HMI_ValueStruct.Max_Feedspeed < MIN_MAXFEEDSPEED) HMI_ValueStruct.Max_Feedspeed = MIN_MAXFEEDSPEED;
    // MaxFeedspeed value
    DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
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
      return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
    }
    // MaxAcceleration limit
    if (WITHIN(HMI_flag.acc_axis, X_AXIS, LAST_AXIS))
      NOMORE(HMI_ValueStruct.Max_Acceleration, default_max_acceleration[HMI_flag.acc_axis] * 2);
    if (HMI_ValueStruct.Max_Acceleration < MIN_MAXACCELERATION) HMI_ValueStruct.Max_Acceleration = MIN_MAXACCELERATION;
    // MaxAcceleration value
    DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
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
        return DWINUI::Draw_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, 1, 210, MBASE(select_jerk.now), HMI_ValueStruct.Max_Jerk_scaled);
      }
      // MaxJerk limit
      if (WITHIN(HMI_flag.jerk_axis, X_AXIS, LAST_AXIS))
        NOMORE(HMI_ValueStruct.Max_Jerk_scaled, default_max_jerk[HMI_flag.jerk_axis] * 2 * MINUNITMULT);
      NOLESS(HMI_ValueStruct.Max_Jerk_scaled, (MIN_MAXJERK) * MINUNITMULT);
      // MaxJerk value
      DWINUI::Draw_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, UNITFDIGITS, 210, MBASE(select_jerk.now), HMI_ValueStruct.Max_Jerk_scaled);
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
      return DWINUI::Draw_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, UNITFDIGITS, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step_scaled);
    }
    // Step limit
    if (WITHIN(HMI_flag.step_axis, X_AXIS, LAST_AXIS))
      NOMORE(HMI_ValueStruct.Max_Step_scaled, 999.9 * MINUNITMULT);
    NOLESS(HMI_ValueStruct.Max_Step_scaled, MIN_STEP);
    // Step value
    DWINUI::Draw_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, UNITFDIGITS, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step_scaled);
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
      DWINUI::Draw_String(HMI_data.Coordinate_Color, HMI_data.Background_Color, x, y, F("???.?"));
    else if (blink && draw_empty)
      DWINUI::Draw_String(HMI_data.Coordinate_Color, HMI_data.Background_Color, x, y, F("     "));
    else
      DWINUI::Draw_Signed_Float(HMI_data.Coordinate_Color, HMI_data.Background_Color, 3, 1, x, y, p * 10);
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
        DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(TUNE_CASE_TEMP + MROWS - index_tune), _hotendtarget);
    #endif
    #if HAS_HEATED_BED
      if (_new_bed_target)
        DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(TUNE_CASE_BED + MROWS - index_tune), _bedtarget);
    #endif
    #if HAS_FAN
      if (_new_fanspeed)
        DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(TUNE_CASE_FAN + MROWS - index_tune), _fanspeed);
    #endif
  }
  else if (checkkey == TemperatureID) {
    // Temperature page temperature update
    #if HAS_HOTEND
      if (_new_hotend_target)
        DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(TEMP_CASE_TEMP), _hotendtarget);
    #endif
    #if HAS_HEATED_BED
      if (_new_bed_target)
        DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(TEMP_CASE_BED), _bedtarget);
    #endif
    #if HAS_FAN
      if (_new_fanspeed)
        DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, MBASE(TEMP_CASE_FAN), _fanspeed);
    #endif
  }

  // Bottom temperature update

  #if HAS_HOTEND
    if (_new_hotend_temp)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 384, _hotendtemp);
    if (_new_hotend_target)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 384, _hotendtarget);

    static int16_t _flow = planner.flow_percentage[0];
    if (_flow != planner.flow_percentage[0]) {
      _flow = planner.flow_percentage[0];
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 417, _flow);
    }
  #endif

  #if HAS_HEATED_BED
    if (_new_bed_temp)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 417, _bedtemp);
    if (_new_bed_target)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 417, _bedtarget);
  #endif

  static int16_t _feedrate = 100;
  if (_feedrate != feedrate_percentage) {
    _feedrate = feedrate_percentage;
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, _feedrate);
  }

  #if HAS_FAN
    if (_new_fanspeed) {
      _fanspeed = thermalManager.fan_speed[0];
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 195 + 2 * STAT_CHR_W, 384, _fanspeed);
    }
  #endif

  static float _offset = 0;
  if (BABY_Z_VAR != _offset) {
    _offset = BABY_Z_VAR;
    DWINUI::Draw_Signed_Float(DWIN_FONT_STAT, HMI_data.Indicator_Color,  HMI_data.Background_Color, 2, 2, 210, 417, _offset * 100);
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
    shift_ms = select_file.now > 0 && strlen(shift_name) > MENU_CHAR_LIMIT ? millis() + 750UL : 0;
  }

#endif

/**
 * Display an SD item, adding a CDUP for subfolders.
 */
void Draw_SDItem(const uint16_t item, int16_t row=-1) {
  if (row < 0) row = item + 1 + MROWS - index_file;
  const bool is_subdir = !card.flag.workDirIsRoot;
  if (is_subdir && item == 0)
    return Draw_Menu_Line(row, ICON_Folder, "..");

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

  DWINUI::ClearMenu(); // Leave title bar unchanged

  Draw_Back_First();

  if (card.isMounted()) {
    // As many files as will fit
    LOOP_L_N(i, _MIN(nr_sd_menu_items(), MROWS))
      Draw_SDItem(i, i + 1);

    TERN_(SCROLL_LONG_FILENAMES, Init_SDItem_Shift());
  }
  else {
    DWIN_Draw_Rectangle(1, HMI_data.AlertBg_Color, 10, MBASE(3) - 10, DWIN_WIDTH - 10, MBASE(4));
    DWINUI::Draw_CenteredString(font16x32, HMI_data.AlertTxt_Color, MBASE(3), F("No Media"));
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
      else if (sdprint && card.isPrinting() && (checkkey == PrintProcess || checkkey == Tune || printingIsActive())) {
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

  DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, STATUS_Y, DWIN_WIDTH, DWIN_HEIGHT - 1);

  #if HAS_HOTEND
    DWINUI::Draw_Icon(ICON_HotendTemp, 10, 383);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 384, thermalManager.wholeDegHotend(0));
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.degTargetHotend(0));

    DWINUI::Draw_Icon(ICON_StepE, 112, 417);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWINUI::Draw_Icon(ICON_BedTemp, 10, 416);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 417, thermalManager.wholeDegBed());
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    DWINUI::Draw_Int(true, true, 0, DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.degTargetBed());
  #endif

  DWINUI::Draw_Icon(ICON_Speed, 113, 383);
  DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 384, F("%"));

  #if HAS_FAN
    DWINUI::Draw_Icon(ICON_FanSpeed, 187, 383);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
  #endif

  #if HAS_ZOFFSET_ITEM
    DWINUI::Draw_Icon(ICON_Zoffset, 187, 416);
  #endif

  if (BABY_Z_VAR < 0) {
    DWINUI::Draw_Float(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 2, 2, 207, 417, -BABY_Z_VAR * 100);
    DWINUI::Draw_String(HMI_data.Indicator_Color, 205, 419, F("-"));
  }
  else {
    DWINUI::Draw_Float(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 2, 2, 207, 417, BABY_Z_VAR * 100);
    DWINUI::Draw_String(HMI_data.Indicator_Color, 205, 419, F(" "));
  }

  DWIN_Draw_Rectangle(1, HMI_data.SplitLine_Color, 0, 449, DWIN_WIDTH, 451);

  DWINUI::Draw_Icon(ICON_MaxSpeedX,  10, 456);
  DWINUI::Draw_Icon(ICON_MaxSpeedY,  95, 456);
  DWINUI::Draw_Icon(ICON_MaxSpeedZ, 180, 456);
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
  DWINUI::ClearMenu();
  Draw_Back_First();

  DWINUI::Draw_CenteredString(122, F(MACHINE_SIZE));
  DWINUI::Draw_CenteredString(195, F(SHORT_BUILD_VERSION));

  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 30, 17, 57, 29); // "Info"

    DWIN_Frame_AreaCopy(1, 197, 149, 252, 161, 108, 102);
    DWIN_Frame_AreaCopy(1, 1, 164, 56, 176, 108, 175);
    DWIN_Frame_AreaCopy(1, 58, 164, 113, 176, 105, 248);
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Title.ShowCaption(GET_TEXT_F(MSG_INFO_SCREEN));
    #else
      Title.FrameCopy(1, 190, 16, 215, 26); // "Info"
    #endif

    DWIN_Frame_AreaCopy(1, 120, 150, 146, 161, 124, 102);
    DWIN_Frame_AreaCopy(1, 146, 151, 254, 161, 82, 175);
    DWIN_Frame_AreaCopy(1, 0, 165, 94, 175, 89, 248);
  }
  DWINUI::Draw_CenteredString(268, F(CORP_WEBSITE));

  LOOP_L_N(i, 3) {
    DWINUI::Draw_Icon(ICON_PrintSize + i, 26, 99 + i * 73);
    DWIN_Draw_Line(HMI_data.SplitLine_Color, 16, MBASE(2) + i * 73, 256, 156 + i * 73);
  }
}

void Draw_Print_File_Menu() {
  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 0, 31, 55, 44); // "Print file"
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Title.ShowCaption(GET_TEXT_F(MSG_MEDIA_MENU));
    #else
      Title.FrameCopy(1, 52, 31, 137, 41); // "Print file"
    #endif
  }
  Redraw_SD_List();
}

// Main Process
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
        Draw_Print_File_Menu();
        break;

      case 1: // Prepare
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
          queue.inject_P(PSTR("G28XYO\nG28Z\nG29"));
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
          uint8_t shift_new = shift_amt + 1;          // Try to shift by...
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

      card.openAndPrintFile(card.filename);

      #if HAS_FAN
        // All fans on for Ender 3 v2 ?
        // The slicer should manage this for us.
        //for (uint8_t i = 0; i < FAN_COUNT; i++)
        //  thermalManager.fan_speed[i] = 255;
      #endif

      DWIN_Print_Started(true);
    }
  }
HMI_SelectFileExit:
  DWIN_UpdateLCD();
}

// Printing
void HMI_Printing() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
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
          checkkey = PauseOrStop;
          Popup_window_PauseOrStop();
        }
        break;

      case 2: // Stop
        HMI_flag.select_flag = true;
        checkkey = PauseOrStop;
        Popup_window_PauseOrStop();
        break;

      default: break;
    }
  }
  DWIN_UpdateLCD();
}

// Print done
void HMI_PrintDone() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    dwin_abort_flag = true; // Reset feedrate, return to Home
    Goto_Main_Menu(); // Return to Main menu after print done
  }
}

// Pause and Stop window
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
        queue.inject_P(PSTR("M25"));
      }
      else {
        // cancel pause
      }
      Goto_PrintProcess();
    }
    else if (select_print.now == 2) { // stop window
      if (HMI_flag.select_flag) {
        checkkey = MainMenu;
        if (HMI_flag.home_flag) planner.synchronize(); // Wait for planner moves to finish!
        wait_for_heatup = wait_for_user = false;       // Stop waiting for heating/user
        card.abortFilePrintSoon();                     // Let the main loop handle SD abort
        dwin_abort_flag = true;                        // Reset feedrate, return to Home
        #ifdef ACTION_ON_CANCEL
          host_action_cancel();
        #endif
        DWIN_Draw_Popup(ICON_BLTouch, "Stopping..." , "Please wait until done.");
      }
      else
        Goto_PrintProcess(); // cancel stop
    }
  }
  DWIN_UpdateLCD();
}

void Draw_AdvSet_Menu_old() {
  DWINUI::ClearMenu();

  #if ADVSET_CASE_TOTAL >= 6
    const int16_t scroll = MROWS - index_advset; // Scrolled-up lines
    #define ASCROL(L) (scroll + (L))
  #else
    #define ASCROL(L) (L)
  #endif

  #define AVISI(L)  WITHIN(ASCROL(L), 0, MROWS)

  Title.ShowCaption(GET_TEXT_F(MSG_ADVANCED_SETTINGS));

  if (AVISI(0)) Draw_Back_First(select_advset.now == 0);
  if (AVISI(ADVSET_CASE_HOMEOFF)) Draw_Menu_Line(ASCROL(ADVSET_CASE_HOMEOFF), ICON_HomeOff, GET_TEXT(MSG_SET_HOME_OFFSETS), true);  // Home Offset >
  #if HAS_ONESTEP_LEVELING
    if (AVISI(ADVSET_CASE_PROBEOFF)) Draw_Menu_Line(ASCROL(ADVSET_CASE_PROBEOFF), ICON_ProbeOff, GET_TEXT(MSG_ZPROBE_OFFSETS), true);  // Probe Offset >
  #endif
  if (AVISI(ADVSET_CASE_HEPID)) Draw_Menu_Line(ASCROL(ADVSET_CASE_HEPID), ICON_PIDNozzle, "Hotend PID", false);  // Nozzle PID
  if (AVISI(ADVSET_CASE_BEDPID)) Draw_Menu_Line(ASCROL(ADVSET_CASE_BEDPID), ICON_PIDbed, "Bed PID", false);  // Bed PID
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (AVISI(ADVSET_CASE_PWRLOSSR)) {
      Draw_Menu_Line(ASCROL(ADVSET_CASE_PWRLOSSR), ICON_Motion, "Power-loss recovery", false);  // Power-loss recovery
      Draw_Chkb_Line(ASCROL(ADVSET_CASE_PWRLOSSR), recovery.enabled);
    }
  #endif
  if (AVISI(ADVSET_CASE_BRIGHTNESS)) {
    Draw_Menu_Line(ASCROL(ADVSET_CASE_BRIGHTNESS), ICON_Motion, "LCD Brightness", false);  // LCD brightness
    Draw_Menu_IntValue(HMI_data.Background_Color, ASCROL(ADVSET_CASE_BRIGHTNESS), 3, HMI_data.Brightness);
  }
  if (AVISI(ADVSET_CASE_SCOLOR)) Draw_Menu_Line(ASCROL(ADVSET_CASE_SCOLOR), ICON_Scolor, "Select Colors", true);  // Select colors >
  #if ENABLED(SOUND_MENU_ITEM)
    if (AVISI(ADVSET_CASE_SOUND)) Draw_Menu_Line(ASCROL(ADVSET_CASE_SOUND), ICON_Sound, "Enable Sound", false);  // Enable Sound
    if (AVISI(ADVSET_CASE_SOUND)) Draw_Chkb_Line(ASCROL(ADVSET_CASE_SOUND), ui.buzzer_enabled);
  #endif
  if (AVISI(ADVSET_CASE_LOCK)) Draw_Menu_Line(ASCROL(ADVSET_CASE_LOCK), ICON_Lock, "Lock Screen", true);  // Lock Screen >
  if (select_advset.now) Draw_Menu_Cursor(ASCROL(select_advset.now));
}

void Draw_HomeOff_Menu() {
  DWINUI::ClearMenu();
  Title.ShowCaption(GET_TEXT_F(MSG_SET_HOME_OFFSETS));                 // Home Offsets
  Draw_Back_First(select_item.now == 0);
  Draw_Menu_Line(1, ICON_HomeOffX, GET_TEXT(MSG_HOME_OFFSET_X));  // Home X Offset
  DWINUI::Draw_Signed_Float(3, 1, 216, MBASE(1), HMI_ValueStruct.Home_OffX_scaled);
  Draw_Menu_Line(2, ICON_HomeOffY, GET_TEXT(MSG_HOME_OFFSET_Y));  // Home Y Offset
  DWINUI::Draw_Signed_Float(3, 1, 216, MBASE(2), HMI_ValueStruct.Home_OffY_scaled);
  Draw_Menu_Line(3, ICON_HomeOffZ, GET_TEXT(MSG_HOME_OFFSET_Z));  // Home Y Offset
  DWINUI::Draw_Signed_Float(3, 1, 216, MBASE(3), HMI_ValueStruct.Home_OffZ_scaled);
  if (select_item.now) Draw_Menu_Cursor(select_item.now);
}

#if HAS_ONESTEP_LEVELING
  void Draw_ProbeOff_Menu() {
    DWINUI::ClearMenu();
    Title.ShowCaption(GET_TEXT_F(MSG_ZPROBE_OFFSETS));                 // Probe Offsets
    Draw_Back_First(select_item.now == 0);
    Draw_Menu_Line(1, ICON_ProbeOffX, GET_TEXT(MSG_ZPROBE_XOFFSET));  // Probe X Offset
    DWINUI::Draw_Signed_Float(3, 1, 216, MBASE(1), HMI_ValueStruct.Probe_OffX_scaled);
    Draw_Menu_Line(2, ICON_ProbeOffY, GET_TEXT(MSG_ZPROBE_YOFFSET));  // Probe Y Offset
    DWINUI::Draw_Signed_Float(3, 1, 216, MBASE(2), HMI_ValueStruct.Probe_OffY_scaled);
    if (select_item.now) Draw_Menu_Cursor(select_item.now);
  }
#endif

void Draw_Color_Item(const uint8_t line, const char * const label, uint16_t color) {
  DWINUI::Draw_String(LBLX, MBASE(line) - 1, (char*)label);
  DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, 26 + 1, MBASE(line) - 1 + 1, 26 + 18, MBASE(line) - 1 + 18);
  DWIN_Draw_Rectangle(1, color, 26 + 2, MBASE(line) - 1 + 2, 26 + 17, MBASE(line) - 1 + 17);
  DWIN_Draw_Line(HMI_data.SplitLine_Color, 16, MBASE(line) + 33, 256, MBASE(line) + 34);
}

void Draw_SelColor_Menu() {
  DWINUI::ClearMenu();

  #if SCOLOR_CASE_TOTAL >= 6
    const int16_t scroll = MROWS - index_selcolor; // Scrolled-up lines
    #define SSCROL(L) (scroll + (L))
  #else
    #define SSCROL(L) (L)
  #endif

  #define SVISI(L)  WITHIN(SSCROL(L), 0, MROWS)

  Title.ShowCaption(F("Select Color"));

  if (SVISI(0)) Draw_Back_First(select_scolor.now == 0);
  if (SVISI(SCOLOR_CASE_LOADDEF)) Draw_Menu_Line(SSCROL(SCOLOR_CASE_LOADDEF), ICON_StockConfiguraton, GET_TEXT(MSG_RESTORE_DEFAULTS));
  if (SVISI(SCOLOR_CASE_BACKG)) Draw_Color_Item(SSCROL(SCOLOR_CASE_BACKG), "Screen Background", HMI_data.Background_Color);
  if (SVISI(SCOLOR_CASE_CURSOR)) Draw_Color_Item(SSCROL(SCOLOR_CASE_CURSOR), "Cursor",  HMI_data.Cursor_color);
  if (SVISI(SCOLOR_CASE_TITLEGB)) Draw_Color_Item(SSCROL(SCOLOR_CASE_TITLEGB), "Title Background", HMI_data.TitleBg_color);
  if (SVISI(SCOLOR_CASE_TITLETXT)) Draw_Color_Item(SSCROL(SCOLOR_CASE_TITLETXT), "Title Text", HMI_data.TitleTxt_color);
  if (SVISI(SCOLOR_CASE_TEXT)) Draw_Color_Item(SSCROL(SCOLOR_CASE_TEXT), "Text", HMI_data.Text_Color);
  if (SVISI(SCOLOR_CASE_SELECT)) Draw_Color_Item(SSCROL(SCOLOR_CASE_SELECT), "Selected", HMI_data.Selected_Color);
  if (SVISI(SCOLOR_CASE_SLINE)) Draw_Color_Item(SSCROL(SCOLOR_CASE_SLINE), "Split Line", HMI_data.SplitLine_Color);
  if (SVISI(SCOLOR_CASE_HIGHLIGHT)) Draw_Color_Item(SSCROL(SCOLOR_CASE_HIGHLIGHT), "Highlight", HMI_data.Highlight_Color);
  if (SVISI(SCOLOR_CASE_STATUSBG)) Draw_Color_Item(SSCROL(SCOLOR_CASE_STATUSBG), "Status Background", HMI_data.StatusBg_Color);
  if (SVISI(SCOLOR_CASE_STATUSTXT)) Draw_Color_Item(SSCROL(SCOLOR_CASE_STATUSTXT), "Status Text", HMI_data.StatusTxt_Color);
  if (SVISI(SCOLOR_CASE_POPUPBG)) Draw_Color_Item(SSCROL(SCOLOR_CASE_POPUPBG), "Popup Background", HMI_data.PopupBg_color);
  if (SVISI(SCOLOR_CASE_POPUPTXT)) Draw_Color_Item(SSCROL(SCOLOR_CASE_POPUPTXT), "Popup Text", HMI_data.PopupTxt_Color);
  if (SVISI(SCOLOR_CASE_ALERTBG)) Draw_Color_Item(SSCROL(SCOLOR_CASE_ALERTBG), "Alert Background", HMI_data.AlertBg_Color);
  if (SVISI(SCOLOR_CASE_ALERTTXT)) Draw_Color_Item(SSCROL(SCOLOR_CASE_ALERTTXT), "Alert Text", HMI_data.AlertTxt_Color);
  if (SVISI(SCOLOR_CASE_PERCNTTXT)) Draw_Color_Item(SSCROL(SCOLOR_CASE_PERCNTTXT), "Percent Text", HMI_data.PercentTxt_Color);
  if (SVISI(SCOLOR_CASE_BARFILL)) Draw_Color_Item(SSCROL(SCOLOR_CASE_BARFILL), "Bar Fill", HMI_data.Barfill_Color);
  if (SVISI(SCOLOR_CASE_INDICATOR)) Draw_Color_Item(SSCROL(SCOLOR_CASE_INDICATOR), "Indicator value", HMI_data.Indicator_Color);
  if (SVISI(SCOLOR_CASE_COORDINATE)) Draw_Color_Item(SSCROL(SCOLOR_CASE_COORDINATE), "Coordinate value", HMI_data.Coordinate_Color);

  if (select_scolor.now) Draw_Menu_Cursor(SSCROL(select_scolor.now));
}

void Draw_GetColor(uint16_t color) {
  HMI_ValueStruct.Color[2] = (color >> 11) & 0x1F;  // Red
  HMI_ValueStruct.Color[1] = (color >>  5) & 0x3F;  // Green
  HMI_ValueStruct.Color[0] = (color >>  0) & 0x1F;  // Blue
  DWINUI::ClearMenu();
  Title.ShowCaption(F("Get Color"));
  Draw_Back_First(select_item.now == 0);
  Draw_Color_Item(1, "Blue", RGB(0,0,31));
  Draw_Menu_IntValue(HMI_data.Background_Color, 1, 2, HMI_ValueStruct.Color[0]);
  Draw_Color_Item(2, "Green", RGB(0,63,0));
  Draw_Menu_IntValue(HMI_data.Background_Color, 2, 2, HMI_ValueStruct.Color[1]);
  Draw_Color_Item(3, "Red", RGB(31,0,0));
  Draw_Menu_IntValue(HMI_data.Background_Color, 3, 2, HMI_ValueStruct.Color[2]);
  Draw_Menu_Line(4, ICON_StockConfiguraton, GET_TEXT(MSG_RESTORE_DEFAULTS));
  if (select_item.now) Draw_Menu_Cursor(select_item.now);
  DWIN_Draw_Rectangle(1, color, 20, 315, DWIN_WIDTH - 20, 335);
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
void Draw_FilamentMan_Menu(){
  DWINUI::ClearMenu();
  Title.ShowCaption(GET_TEXT_F(MSG_FILAMENT_MAN));
  Draw_Back_First(select_item.now == 0);
  Draw_Menu_Line(1, ICON_Park, GET_TEXT(MSG_FILAMENT_PARK_ENABLED));
  Draw_Menu_Line(2, ICON_FilMan, GET_TEXT(MSG_FILAMENTCHANGE));
  #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    Draw_Menu_Line(3, ICON_FilUnload, GET_TEXT(MSG_FILAMENTUNLOAD));
    Draw_Menu_Line(4, ICON_FilLoad, GET_TEXT(MSG_FILAMENTLOAD));
  #endif
  if (select_item.now) Draw_Menu_Cursor(select_item.now);
  }

#endif

#if ENABLED(ASSISTED_TRAMMING)

  void Draw_Tramming_Menu() {
    DWINUI::ClearMenu();
    Title.ShowCaption(GET_TEXT_F(MSG_MANUAL_LEVELING));
    Draw_Back_First(select_item.now == 0);
    DWIN_Draw_Label(MBASE(1), F(TRAMMING_POINT_NAME_1));
    DWIN_Draw_Label(MBASE(2), F(TRAMMING_POINT_NAME_2));
    DWIN_Draw_Label(MBASE(3), F(TRAMMING_POINT_NAME_3));
    #ifdef TRAMMING_POINT_NAME_4
      DWIN_Draw_Label(MBASE(4), F(TRAMMING_POINT_NAME_4));
      #ifdef TRAMMING_POINT_NAME_5
        #define TRAM_POINTS 5
        DWIN_Draw_Label(MBASE(5), F(TRAMMING_POINT_NAME_5));
      #else
        #define TRAM_POINTS 4
        Draw_Menu_Line(5, ICON_Axis, "Center");
      #endif
    #else
      #define TRAM_POINTS 3
    #endif
    // Draw separators and icons
    LOOP_L_N(i, TRAM_POINTS) Draw_Menu_Line(i + 1, ICON_Axis);
    if (select_item.now) Draw_Menu_Cursor(select_item.now);
  }

#endif

#if ENABLED(MESH_BED_LEVELING)

  void Draw_ManualMesh_Menu() {
    DWINUI::ClearMenu();
    Title.ShowCaption(GET_TEXT_F(MSG_UBL_MANUAL_MESH));
    Draw_Back_First(select_item.now == 0);
    Draw_Menu_Line(1, ICON_ManualMesh, GET_TEXT(MSG_UBL_BUILD_MESH_MENU)); //Start -> G29 S1
    Draw_Menu_Line(2, ICON_Zoffset, GET_TEXT(MSG_MOVE_Z)); //Move Z ->
    DWINUI::Draw_Signed_Float(3, 2, 216, MBASE(2), current_position.z * 100);
    Draw_Menu_Line(3, ICON_Axis, GET_TEXT(MSG_UBL_CONTINUE_MESH)); //Next -> G29 S2
    Draw_Menu_Line(4, ICON_MeshSave,GET_TEXT(MSG_UBL_SAVE_MESH)); //Save -> M500
    if (select_item.now) Draw_Menu_Cursor(select_item.now);
  }

#endif

#include "../../../libs/buzzer.h"

void HMI_AudioFeedback(const bool success/*=true*/) {
  #if HAS_BUZZER
    if (success) {
      BUZZ(100, 659);
      BUZZ(10, 0);
      BUZZ(100, 698);
    }
    else
      BUZZ(40, 440);
  #endif
}

#if HAS_HOTEND
void Draw_PLA_Menu() {
  DWINUI::ClearMenu();
  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 59, 16, 139, 29);                                         // "PLA Settings"
    DWIN_Frame_AreaCopy(1, 100, 89, 124, 101, LBLX, MBASE(PREHEAT_CASE_TEMP));
    DWIN_Frame_AreaCopy(1, 1, 134, 56, 146, LBLX + 24, MBASE(PREHEAT_CASE_TEMP));     // PLA nozzle temp
    #if HAS_HEATED_BED
      DWIN_Frame_AreaCopy(1, 100, 89, 124, 101, LBLX, MBASE(PREHEAT_CASE_BED));
      DWIN_Frame_AreaCopy(1, 58, 134, 113, 146, LBLX + 24, MBASE(PREHEAT_CASE_BED));  // PLA bed temp
    #endif
    #if HAS_FAN
      DWIN_Frame_AreaCopy(1, 100, 89, 124, 101, LBLX, MBASE(PREHEAT_CASE_FAN));
      DWIN_Frame_AreaCopy(1, 115, 134, 170, 146, LBLX + 24, MBASE(PREHEAT_CASE_FAN)); // PLA fan speed
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      DWIN_Frame_AreaCopy(1, 72, 148, 151, 162, LBLX, MBASE(PREHEAT_CASE_SAVE));      // Save PLA configuration
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Title.ShowCaption("PLA Settings"); // TODO: GET_TEXT_F
    #else
      Title.FrameCopy(1, 56, 16, 141, 28);                                       // "PLA Settings"
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
  }

  Draw_Back_First();

  uint8_t i = 0;
  Draw_Menu_Line(++i, ICON_SetEndTemp);
  DWINUI::Draw_Int(3, 216, MBASE(i), ui.material_preset[0].hotend_temp);
  #if HAS_HEATED_BED
    Draw_Menu_Line(++i, ICON_SetBedTemp);
    DWINUI::Draw_Int(3, 216, MBASE(i), ui.material_preset[0].bed_temp);
  #endif
  #if HAS_FAN
    Draw_Menu_Line(++i, ICON_FanSpeed);
    DWINUI::Draw_Int(3, 216, MBASE(i), ui.material_preset[0].fan_speed);
  #endif
  #if ENABLED(EEPROM_SETTINGS)
    Draw_Menu_Line(++i, ICON_WriteEEPROM);
  #endif
  }

void Draw_ABS_Menu(){
  DWINUI::ClearMenu();
  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 142, 16, 223, 29);                                        // "ABS Settings"
    DWIN_Frame_AreaCopy(1, 180, 89, 204, 100, LBLX, MBASE(PREHEAT_CASE_TEMP));
    DWIN_Frame_AreaCopy(1, 1, 134, 56, 146, LBLX + 24, MBASE(PREHEAT_CASE_TEMP));    // ABS nozzle temp
    #if HAS_HEATED_BED
      DWIN_Frame_AreaCopy(1, 180, 89, 204, 100, LBLX, MBASE(PREHEAT_CASE_BED));
      DWIN_Frame_AreaCopy(1, 58, 134, 113, 146, LBLX + 24, MBASE(PREHEAT_CASE_BED));  // ABS bed temp
    #endif
    #if HAS_FAN
      DWIN_Frame_AreaCopy(1, 180, 89, 204, 100, LBLX, MBASE(PREHEAT_CASE_FAN));
      DWIN_Frame_AreaCopy(1, 115, 134, 170, 146, LBLX + 24, MBASE(PREHEAT_CASE_FAN)); // ABS fan speed
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      DWIN_Frame_AreaCopy(1, 72, 148, 151, 162, LBLX, MBASE(PREHEAT_CASE_SAVE));
      DWIN_Frame_AreaCopy(1, 180, 89, 204, 100, LBLX + 28, MBASE(PREHEAT_CASE_SAVE) + 2);   // Save ABS configuration
    #endif
  }
  else {
    #ifdef USE_STRING_HEADINGS
      Title.ShowCaption("ABS Settings"); // TODO: GET_TEXT_F
    #else
      Title.FrameCopy(1, 56, 16, 141, 28);                                                  // "ABS Settings"
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
  }

  Draw_Back_First();

  uint8_t i = 0;
  Draw_Menu_Line(++i, ICON_SetEndTemp);
  DWINUI::Draw_Int(3, 216, MBASE(i), ui.material_preset[1].hotend_temp);
  #if HAS_HEATED_BED
    Draw_Menu_Line(++i, ICON_SetBedTemp);
    DWINUI::Draw_Int(3, 216, MBASE(i), ui.material_preset[1].bed_temp);
  #endif
  #if HAS_FAN
    Draw_Menu_Line(++i, ICON_FanSpeed);
    DWINUI::Draw_Int(3, 216, MBASE(i), ui.material_preset[1].fan_speed);
  #endif
  #if ENABLED(EEPROM_SETTINGS)
    Draw_Menu_Line(++i, ICON_WriteEEPROM);
  #endif
  }

#endif // HAS_HOTEND

void Draw_Temperature_Menu() {
  DWINUI::ClearMenu();

  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 236, 2, 263, 13); // "Temperature"
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
      Title.ShowCaption(GET_TEXT_F(MSG_TEMPERATURE));
    #else
      Title.FrameCopy(1, 56, 16, 141, 28);                                       // "Temperature"
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
  }

  Draw_Back_First(select_temp.now == 0);
  if (select_temp.now) Draw_Menu_Cursor(select_temp.now);

  // Draw icons and lines
  uint8_t i = 0;
  #define _TMENU_ICON(N) Draw_Menu_Line(++i, ICON_SetEndTemp + (N) - 1)
  #if HAS_HOTEND
    _TMENU_ICON(TEMP_CASE_TEMP);
    DWINUI::Draw_Int(3, 216, MBASE(i), thermalManager.degTargetHotend(0));
  #endif
  #if HAS_HEATED_BED
    _TMENU_ICON(TEMP_CASE_BED);
    DWINUI::Draw_Int(3, 216, MBASE(i), thermalManager.degTargetBed());
  #endif
  #if HAS_FAN
    _TMENU_ICON(TEMP_CASE_FAN);
    DWINUI::Draw_Int(3, 216, MBASE(i), thermalManager.fan_speed[0]);
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
          case CONTROL_CASE_ADVSET:  // Advanced Settings >
            Draw_Menu_Item(MROWS, ICON_AdvSet, GET_TEXT(MSG_ADVANCED_SETTINGS), true);
            break;
          case CONTROL_CASE_INFO:    // Info >
            Item_Control_Info(MBASE(MROWS));
            Draw_Menu_Icon(MROWS, ICON_Info);
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
          case MROWS:
            Draw_Back_First();
            break;
          case MROWS + 1: // Temperature >
            Draw_Menu_Line(0, ICON_Temperature, GET_TEXT(MSG_TEMPERATURE), true);
            break;
          case MROWS + 2: // Move >
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
          Draw_Control_Menu();
          Draw_Status_Area(false);
          HMI_AudioFeedback(success);
        } break;
        case CONTROL_CASE_RESET: // Reset EEPROM
          settings.reset();
          Draw_Control_Menu();
          Draw_Status_Area(false);
          HMI_AudioFeedback();
          break;
      #endif
      case CONTROL_CASE_ADVSET: // Advanced Settings
        checkkey = AdvSet;
        select_advset.reset();
        Draw_AdvSet_Menu_old();
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

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  // Filament Management
  void HMI_FilamentMan() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_item.inc(1 + 2 + 2 * ENABLED(FILAMENT_LOAD_UNLOAD_GCODES))) Move_Highlight(1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_item.dec()) Move_Highlight(-1, select_item.now);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_item.now) {
      case 0: // Back
        Draw_Prepare_Menu();
        break;
      case 1: // Park Head
        DWIN_StatusChanged_P(GET_TEXT(MSG_FILAMENT_PARK_ENABLED));
        queue.inject_P(PSTR("G28O\nG27"));
        break;
      case 2: // Filament Change
        queue.inject_P(PSTR("M600 B2"));
        break;
      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        case 3: // Unload Filament
          DWIN_StatusChanged_P(GET_TEXT(MSG_FILAMENTUNLOAD));
          queue.inject_P(PSTR("M702 Z20"));
          break;
        case 4: // Load Filament
          DWIN_StatusChanged_P(GET_TEXT(MSG_FILAMENTLOAD));
          queue.inject_P(PSTR("M701 Z20"));
          break;
      #endif
    }
  }
  DWIN_UpdateLCD();
  }

#endif // ADVANCED_PAUSE_FEATURE

#if ENABLED(ASSISTED_TRAMMING)

  // Bed Tramming
  void HMI_Tramming() {
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

      char cmd[100] = "";
      #if HAS_ONESTEP_LEVELING
        char str_1[6] = "", str_2[6] = "", str_3[6] = "";
        #define fmt "X:%s, Y:%s, Z:%s"
        float xpos = 0, ypos = 0, zpos = 0;
        float margin = PROBING_MARGIN;
      #else
        #define fmt "M420 S0\nG28O\nG90\nG0 Z5 F300\nG0 X%i Y%i F5000\nG0 Z0 F300"
        int16_t xpos = 0, ypos = 0;
        int16_t margin = 30;
      #endif

      switch (select_item.now) {
        case 0:
          Draw_Prepare_Menu();
          break;
        case 1:
          DWIN_StatusChanged_P(PSTR(TRAMMING_POINT_NAME_1));
          xpos = ypos = margin;
          break;
        case 2:
          DWIN_StatusChanged_P(PSTR(TRAMMING_POINT_NAME_2));
          xpos = X_BED_SIZE - margin; ypos = margin;
          break;
        case 3:
          DWIN_StatusChanged_P(PSTR(TRAMMING_POINT_NAME_3));
          xpos = X_BED_SIZE - margin; ypos = Y_BED_SIZE - margin;
          break;
        #ifdef TRAMMING_POINT_NAME_4
          case 4:
            DWIN_StatusChanged_P(PSTR(TRAMMING_POINT_NAME_4));
            xpos = margin; ypos = Y_BED_SIZE - margin;
            break;
        #endif
        #ifdef TRAMMING_POINT_NAME_5
          case 5:
            DWIN_StatusChanged_P(PSTR(TRAMMING_POINT_NAME_5));
            xpos = X_BED_SIZE / 2; ypos = Y_BED_SIZE / 2;
            break;
        #else
          case 5:
            DWIN_StatusChanged_P(PSTR("Center"));
            xpos = X_BED_SIZE / 2; ypos = Y_BED_SIZE / 2;
            break;
        #endif
      }

      if (select_item.now) {
        #if HAS_ONESTEP_LEVELING
          gcode.process_subcommands_now_P(PSTR("M420S0\nG28O"));
          planner.synchronize();
          zpos = probe.probe_at_point(xpos, ypos, PROBE_PT_STOW);
          sprintf_P(cmd, PSTR(fmt),
            dtostrf(xpos, 1, 1, str_1),
            dtostrf(ypos, 1, 1, str_2),
            dtostrf(zpos, 1, 2, str_3)
          );
          DWIN_StatusChanged(cmd);
        #else
          sprintf_P(cmd, PSTR(fmt), xpos, ypos);
          queue.inject(cmd);
        #endif
      }
    }
    DWIN_UpdateLCD();
  }

#endif // ASSISTED_TRAMMING

#if ENABLED(MESH_BED_LEVELING)

  // Manual Mesh
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
          Draw_Prepare_Menu();
          break;
        case 1: // Start manual mesh
          DWIN_StatusChanged_P(GET_TEXT(MSG_UBL_BUILD_MESH_MENU));
          gcode.process_subcommands_now_P(PSTR("G28 XYO\nG28 Z\nM211 S0\nG29 S1"));
          planner.synchronize();
          #ifdef MANUAL_PROBE_START_Z
            DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, 2, 216, MBASE(2), MANUAL_PROBE_START_Z * 100);
          #endif
          break;
        case 2: // move Z
          DWIN_StatusChanged_P(GET_TEXT(MSG_MOVE_Z));
          checkkey = MMeshMoveZ;
          HMI_ValueStruct.Move_Z_scaled = current_position.z * 100;
          DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 2, 216, MBASE(2), HMI_ValueStruct.Move_Z_scaled);
          EncoderRate.enabled = true;
          break;
        case 3: // Next mesh point
          gcode.process_subcommands_now_P(PSTR("G29 S2"));
          planner.synchronize();
          #ifdef MANUAL_PROBE_START_Z
            DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, 2, 216, MBASE(2), MANUAL_PROBE_START_Z * 100);
          #endif
          break;
        case 4: // Save Mesh
          DWIN_StatusChanged_P(GET_TEXT(MSG_UBL_STORAGE_MESH_MENU));
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
        DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, 2, 216, MBASE(2), HMI_ValueStruct.Move_Z_scaled);
        return DWIN_UpdateLCD();
      }
      LIMIT(HMI_ValueStruct.Move_Z_scaled, -100, 100);
      current_position.z = HMI_ValueStruct.Move_Z_scaled / 100;
      DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 2, 216, MBASE(2), HMI_ValueStruct.Move_Z_scaled);
      DWIN_UpdateLCD();
      HMI_Plan_Move(homing_feedrate(Z_AXIS));
    }
  }

#endif // MESH_BED_LEVELING

// TemperatureID
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
          DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(1), HMI_ValueStruct.E_Temp);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_HEATED_BED
        case TEMP_CASE_BED: // Bed temperature
          checkkey = BedTemp;
          HMI_ValueStruct.Bed_Temp = thermalManager.degTargetBed();
          DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(2), HMI_ValueStruct.Bed_Temp);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_FAN
        case TEMP_CASE_FAN: // Fan speed
          checkkey = FanSpeed;
          HMI_ValueStruct.Fan_speed = thermalManager.fan_speed[0];
          DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(3), HMI_ValueStruct.Fan_speed);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_HOTEND
        case TEMP_CASE_PLA: { // PLA preheat setting
          checkkey = PLAPreheat;
          select_PLA.reset();
          HMI_ValueStruct.show_mode = -2;
          Draw_PLA_Menu();
        } break;

        case TEMP_CASE_ABS: { // ABS preheat setting
          checkkey = ABSPreheat;
          select_ABS.reset();
          HMI_ValueStruct.show_mode = -3;
          Draw_ABS_Menu();
        } break;

      #endif // HAS_HOTEND
    }
  }
  DWIN_UpdateLCD();
}

void Draw_Max_Speed_Menu() {
  DWINUI::ClearMenu();

  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 1, 16, 28, 28); // "Max Speed (mm/s)"

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
      Title.ShowCaption("Max Speed (mm/s)"); // TODO: GET_TEXT_F
    #else
      Title.FrameCopy(1, 144, 16, 189, 26); // "Max Speed (mm/s)"
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
  DWINUI::Draw_Int(4, 210, MBASE(1), planner.settings.max_feedrate_mm_s[X_AXIS]);
  DWINUI::Draw_Int(4, 210, MBASE(2), planner.settings.max_feedrate_mm_s[Y_AXIS]);
  DWINUI::Draw_Int(4, 210, MBASE(3), planner.settings.max_feedrate_mm_s[Z_AXIS]);
  #if HAS_HOTEND
    DWINUI::Draw_Int(4, 210, MBASE(4), planner.settings.max_feedrate_mm_s[E_AXIS]);
  #endif
}

void Draw_Max_Accel_Menu() {
  DWINUI::ClearMenu();

  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 1, 16, 28, 28); // "Acceleration"

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
      Title.ShowCaption(GET_TEXT_F(MSG_ACCELERATION));
    #else
      Title.FrameCopy(1, 144, 16, 189, 26);          // "Acceleration"
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
  }

  Draw_Back_First();
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_MaxAccX + i);
  DWINUI::Draw_Int(4, 210, MBASE(1), planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
  DWINUI::Draw_Int(4, 210, MBASE(2), planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
  DWINUI::Draw_Int(4, 210, MBASE(3), planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
  #if HAS_HOTEND
    DWINUI::Draw_Int(4, 210, MBASE(4), planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
  #endif
}

#if HAS_CLASSIC_JERK

  void Draw_Max_Jerk_Menu() {
    DWINUI::ClearMenu();

    if (HMI_IsChinese()) {
      Title.FrameCopy(1, 1, 16, 28, 28); // "Jerk"

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
        Title.ShowCaption(GET_TEXT_F(MSG_JERK));
      #else
        Title.FrameCopy(1, 144, 16, 189, 26); // "Jerk"
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
    DWINUI::Draw_Float(3, UNITFDIGITS, 210, MBASE(1), planner.max_jerk[X_AXIS] * MINUNITMULT);
    DWINUI::Draw_Float(3, UNITFDIGITS, 210, MBASE(2), planner.max_jerk[Y_AXIS] * MINUNITMULT);
    DWINUI::Draw_Float(3, UNITFDIGITS, 210, MBASE(3), planner.max_jerk[Z_AXIS] * MINUNITMULT);
    #if HAS_HOTEND
      DWINUI::Draw_Float(3, UNITFDIGITS, 210, MBASE(4), planner.max_jerk[E_AXIS] * MINUNITMULT);
    #endif
  }

#endif // HAS_CLASSIC_JERK

void Draw_Steps_Menu() {
  DWINUI::ClearMenu();

  if (HMI_IsChinese()) {
    Title.FrameCopy(1, 1, 16, 28, 28); // "Steps per mm"

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
      Title.ShowCaption(GET_TEXT_F(MSG_STEPS_PER_MM));
    #else
      Title.FrameCopy(1, 144, 16, 189, 26); // "Steps per mm"
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
    #endif
  }

  Draw_Back_First();
  LOOP_L_N(i, 3 + ENABLED(HAS_HOTEND)) Draw_Menu_Line(i + 1, ICON_StepX + i);
  DWINUI::Draw_Float(3, UNITFDIGITS, 210, MBASE(1), planner.settings.axis_steps_per_mm[X_AXIS] * MINUNITMULT);
  DWINUI::Draw_Float(3, UNITFDIGITS, 210, MBASE(2), planner.settings.axis_steps_per_mm[Y_AXIS] * MINUNITMULT);
  DWINUI::Draw_Float(3, UNITFDIGITS, 210, MBASE(3), planner.settings.axis_steps_per_mm[Z_AXIS] * MINUNITMULT);
  #if HAS_HOTEND
    DWINUI::Draw_Float(3, UNITFDIGITS, 210, MBASE(4), planner.settings.axis_steps_per_mm[E_AXIS] * MINUNITMULT);
  #endif
}

void Draw_Main_Area(uint8_t procID) {
  switch (procID) {
    case MainMenu:               Draw_Main_Menu(); break;
    case Menu:                   CurrentMenu->Draw(); break;
    case SelectFile:             Draw_Print_File_Menu(); break;
    case Control:                Draw_Control_Menu(); break;
    case PrintProcess:           Draw_PrintProcess(); break;
    case PrintDone:              Draw_PrintDone(); break;
    case PauseOrStop:            Popup_window_PauseOrStop(); break;
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      case FilamentPurge:        Draw_Popup_FilamentPurge(); break;
      case FilamentMan:          Draw_FilamentMan_Menu(); break;
    #endif
    #if ENABLED(ASSISTED_TRAMMING)
      case Tramming:             Draw_Tramming_Menu(); break;
    #endif
    #if ENABLED(MESH_BED_LEVELING)
      case ManualMesh:           Draw_ManualMesh_Menu(); break;
      case MMeshMoveZ:           Draw_ManualMesh_Menu(); break;
    #endif
    case TemperatureID:          Draw_Temperature_Menu(); break;
    case Motion:                 Draw_Motion_Menu(); break;
    case AdvSet:                 Draw_AdvSet_Menu_old(); break;
    case Brightness:             Draw_AdvSet_Menu_old(); break;
    case HomeOff:                Draw_HomeOff_Menu(); break;
    case HomeOffX:               Draw_HomeOff_Menu(); break;
    case HomeOffY:               Draw_HomeOff_Menu(); break;
    case HomeOffZ:               Draw_HomeOff_Menu(); break;
    #if HAS_ONESTEP_LEVELING
      case ProbeOff:             Draw_ProbeOff_Menu(); break;
      case ProbeOffX:            Draw_ProbeOff_Menu(); break;
      case ProbeOffY:            Draw_ProbeOff_Menu(); break;
    #endif
    case SelColor:               Draw_SelColor_Menu(); break;
    case GetColor:               Draw_GetColor(RGB(HMI_ValueStruct.Color[2], HMI_ValueStruct.Color[1], HMI_ValueStruct.Color[0])); break;
    case GetColor_value:         Draw_GetColor(RGB(HMI_ValueStruct.Color[2], HMI_ValueStruct.Color[1], HMI_ValueStruct.Color[0])); break;
    case Info:                   Draw_Info_Menu(); break;
    case Tune:                   Draw_Tune_Menu(); break;
    case TuneFlow:               Draw_Tune_Menu(); break;
    #if HAS_HOTEND
      case PLAPreheat:           Draw_PLA_Menu(); break;
      case ABSPreheat:           Draw_ABS_Menu(); break;
    #endif
    case MaxSpeed:               Draw_Max_Speed_Menu(); break;
    case MaxSpeed_value:         Draw_Max_Speed_Menu(); break;
    case MaxAcceleration:        Draw_Max_Accel_Menu(); break;
    case MaxAcceleration_value:  Draw_Max_Accel_Menu(); break;
    #if HAS_CLASSIC_JERK
      case MaxJerk:              Draw_Max_Jerk_Menu(); break;
      case MaxJerk_value:        Draw_Max_Jerk_Menu(); break;
    #endif
    case Step:                   Draw_Steps_Menu(); break;
    case Step_value:             Draw_Steps_Menu(); break;
    case Move_X:                 Draw_Move_Menu(); break;
    case Move_Y:                 Draw_Move_Menu(); break;
    case Move_Z:                 Draw_Move_Menu(); break;
    #if HAS_HOTEND
      case Extruder:             Draw_Move_Menu(); break;
    #endif
    #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
      case Zoffset:
        switch (HMI_ValueStruct.show_mode) {
          case -4:               Draw_Prepare_Menu(); break;
          default:               Draw_Tune_Menu(); break;
        }
        break;
    #endif
    TERN_(HAS_HOTEND, case ETemp:)
    TERN_(HAS_HEATED_BED, case BedTemp:)
    #if HAS_PREHEAT && HAS_FAN
      case FanSpeed:
    #endif
    #if HAS_HOTEND || HAS_HEATED_BED || (HAS_PREHEAT && HAS_FAN)
      switch (HMI_ValueStruct.show_mode) {
        case -1: Draw_Temperature_Menu(); break;
        case -2: Draw_PLA_Menu(); break;
        case -3: Draw_ABS_Menu(); break;
        default: Draw_Tune_Menu(); break;
      }
      break;
    #endif
    case PrintSpeed: Draw_Tune_Menu(); break;
    case PrintFlow: Draw_Motion_Menu(); break;
    case Locked: LockScreen.Draw(); break;
    default: break;
  }
}

void HMI_ReturnScreen() {
  checkkey = last_checkkey;
  Draw_Main_Area(checkkey);
  DWIN_UpdateLCD();
  return;
}

// Motion
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
        select_speed.reset();
        Draw_Max_Speed_Menu();
        break;
      case MOTION_CASE_ACCEL:  // Max acceleration
        checkkey = MaxAcceleration;
        select_acc.reset();
        Draw_Max_Accel_Menu();
        break;
      #if HAS_CLASSIC_JERK
        case MOTION_CASE_JERK: // Max jerk
          checkkey = MaxJerk;
          select_jerk.reset();
          Draw_Max_Jerk_Menu();
         break;
      #endif
      case MOTION_CASE_STEPS:  // Steps per mm
        checkkey = Step;
        select_step.reset();
        Draw_Steps_Menu();
        break;
      case MOTION_CASE_FLOW: // Flow rate
        checkkey = PrintFlow;
        HMI_ValueStruct.print_flow = planner.flow_percentage[0];
        DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(MOTION_CASE_FLOW),planner.flow_percentage[0]);
        EncoderRate.enabled = true;
        break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

void HMI_Flow(const processID process, uint16_t y) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.print_flow)) {
      checkkey = process;
      EncoderRate.enabled = false;
      planner.flow_percentage[0] = HMI_ValueStruct.print_flow;
      return DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3, 216, y, planner.flow_percentage[0]);
    }
    // print_flow limit
    LIMIT(HMI_ValueStruct.print_flow, MIN_PRINT_FLOW, MAX_PRINT_FLOW);
    // print_flow value
    DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, y, HMI_ValueStruct.print_flow);
  }
}

inline void HMI_PrintFlow() { HMI_Flow(Motion, MBASE(MOTION_CASE_FLOW)); }

// Reset Printer
void HMI_Reboot() {
  dwin_abort_flag = true;                     // Reset feedrate, return to Home
  wait_for_heatup = wait_for_user = false;    // Stop waiting for heating/user
  thermalManager.disable_all_heaters();
  planner.finish_and_disable();
  DWIN_RebootScreen();
  HAL_reboot();
}

// Advanced Settings
void HMI_AdvSet() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_advset.inc(1 + ADVSET_CASE_TOTAL)) {
      if (select_advset.now > MROWS && select_advset.now > index_advset) {
        index_advset = select_advset.now;

        // Scroll up and draw a blank bottom line
        Scroll_Menu(DWIN_SCROLL_UP);

        switch (index_advset) {  // Redraw last menu items
          case ADVSET_CASE_BRIGHTNESS:  // LCD Brightness
            Draw_Menu_Item(MROWS, ICON_Motion, "LCD Brightness", false);  // LCD brightness
            Draw_Menu_IntValue(HMI_data.Background_Color, MROWS, 3, HMI_data.Brightness);
            break;
          case ADVSET_CASE_SCOLOR:      // Select colors
            Draw_Menu_Item(MROWS, ICON_Scolor, "Select Colors", true);  // Select colors >
            break;
          #if ENABLED(SOUND_MENU_ITEM)
            case ADVSET_CASE_SOUND:     // Sound enable
              Draw_Menu_Item(MROWS, ICON_Sound, "Enable Sound", false);  // Sound Enable
              Draw_Chkb_Line(MROWS, ui.buzzer_enabled);
              break;
          #endif
          case ADVSET_CASE_LOCK : // Lock Screen
            Draw_Menu_Item(MROWS, ICON_Lock, "Lock Screen", true);  // Lock screen >
            break;
          default: break;
        }

      }
      else
        Move_Highlight(1, select_advset.now + MROWS - index_advset);
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_advset.dec()) {
      if (select_advset.now < index_advset - MROWS) {
        index_advset--;
        Scroll_Menu(DWIN_SCROLL_DOWN);

        switch (index_advset) {  // Redraw first menu items
          case MROWS:
            Draw_Back_First();
            break;
          case MROWS + ADVSET_CASE_HOMEOFF:   // Home Offset >
            Draw_Menu_Line(0, ICON_HomeOff, GET_TEXT(MSG_SET_HOME_OFFSETS), true);
            break;
          #if HAS_ONESTEP_LEVELING
            case MROWS + ADVSET_CASE_PROBEOFF:  // Probe Offset >
              Draw_Menu_Line(0, ICON_ProbeOff, GET_TEXT(MSG_ZPROBE_OFFSETS), true);
              break;
          #endif
          default: break;
        }

      }
      else
        Move_Highlight(-1, select_advset.now + MROWS - index_advset);
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    char cmd[48] = "";
    char str_1[5] = "", str_2[5] = "";
    sprintf_P(cmd, PSTR("G28O XY\nG0 Z5 F300\nG0 X%s Y%s F5000"), 
      dtostrf(X_CENTER, 1, 1, str_1),
      dtostrf(Y_CENTER, 1, 1, str_2));
    switch (select_advset.now) {
      case 0: // Back
        checkkey = Control;
        select_control.set(CONTROL_CASE_ADVSET);
        index_control = CONTROL_CASE_ADVSET;
        Draw_Control_Menu();
        break;

      #if HAS_HOME_OFFSET
        case ADVSET_CASE_HOMEOFF:   // Home Offsets
          checkkey = HomeOff;
          select_item.reset();
          HMI_ValueStruct.Home_OffX_scaled = home_offset[X_AXIS] * 10;
          HMI_ValueStruct.Home_OffY_scaled = home_offset[Y_AXIS] * 10;
          HMI_ValueStruct.Home_OffZ_scaled = home_offset[Z_AXIS] * 10;
          Draw_HomeOff_Menu();
          break;
      #endif

      #if HAS_ONESTEP_LEVELING
        case ADVSET_CASE_PROBEOFF:   // Probe Offsets
          checkkey = ProbeOff;
          select_item.reset();
          HMI_ValueStruct.Probe_OffX_scaled = probe.offset.x * 10;
          HMI_ValueStruct.Probe_OffY_scaled = probe.offset.y * 10;
          Draw_ProbeOff_Menu();
          break;
      #endif

      #if HAS_HOTEND
        case ADVSET_CASE_HEPID:   // Nozzle PID Autotune
          gcode.process_subcommands_now_P(cmd);
          planner.synchronize();
          thermalManager.PID_autotune(ui.material_preset[0].hotend_temp, H_E0, 10, true);
          break;
      #endif

      #if HAS_HEATED_BED
        case ADVSET_CASE_BEDPID:  // Bed PID Autotune
          gcode.process_subcommands_now_P(cmd);
          planner.synchronize();
          thermalManager.PID_autotune(ui.material_preset[0].bed_temp, H_BED, 10, true);
          break;
      #endif

      #if ENABLED(POWER_LOSS_RECOVERY)
        case ADVSET_CASE_PWRLOSSR:  // Power-loss recovery
          recovery.enable(!recovery.enabled);
          Draw_Chkb_Line(ADVSET_CASE_PWRLOSSR + MROWS - index_advset, recovery.enabled);
          break;
      #endif

      case ADVSET_CASE_BRIGHTNESS: // LCD brightness
        checkkey = Brightness;
        HMI_ValueStruct.Brightness = HMI_data.Brightness;
        Draw_Menu_IntValue(HMI_data.Selected_Color, ADVSET_CASE_BRIGHTNESS + MROWS - index_advset, 3, HMI_ValueStruct.Brightness);
        EncoderRate.enabled = true;
        break;
      case ADVSET_CASE_SCOLOR:  // Select Color
        checkkey = SelColor;
        select_item.reset();
        Draw_SelColor_Menu();
        break;

      #if ENABLED(SOUND_MENU_ITEM)
        case ADVSET_CASE_SOUND: // Sound enable
          ui.buzzer_enabled = !ui.buzzer_enabled;
          Draw_Chkb_Line(ADVSET_CASE_SOUND + MROWS - index_advset, ui.buzzer_enabled);
          break;
      #endif
      case ADVSET_CASE_LOCK : // Lock Screen
        DWIN_LockScreen(true);
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

#if HAS_HOME_OFFSET

  /* Home Offset */
  void HMI_HomeOffset() {
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
          checkkey = AdvSet;
          select_advset.set(ADVSET_CASE_HOMEOFF);
          Draw_AdvSet_Menu_old();
          break;
        case 1: // Home Offset X
          checkkey = HomeOffX;
          DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 1, 216, MBASE(1), HMI_ValueStruct.Home_OffX_scaled);
          EncoderRate.enabled = true;
          break;
        case 2: // Home Offset Y
          checkkey = HomeOffY;
          DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 1, 216, MBASE(2), HMI_ValueStruct.Home_OffY_scaled);
          EncoderRate.enabled = true;
          break;
        case 3: // Home Offset Z
          checkkey = HomeOffZ;
          DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 1, 216, MBASE(3), HMI_ValueStruct.Home_OffZ_scaled);
          EncoderRate.enabled = true;
          break;
        default: break;
      }
    }
    DWIN_UpdateLCD();
  }

  void HMI_HomeOffsetN(const AxisEnum axis, float &posScaled, const_float_t lo, const_float_t hi) {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (Apply_Encoder(encoder_diffState, posScaled)) {
        checkkey = HomeOff;
        EncoderRate.enabled = false;
        set_home_offset(axis, posScaled / 10);
        return DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, 1, 216, MBASE(select_item.now), posScaled);
      }
      LIMIT(posScaled, lo, hi);
      DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, UNITFDIGITS, 216, MBASE(select_item.now), posScaled);
    }
  }

  void HMI_HomeOffsetX() { HMI_HomeOffsetN(X_AXIS, HMI_ValueStruct.Home_OffX_scaled, -500, 500); }
  void HMI_HomeOffsetY() { HMI_HomeOffsetN(Y_AXIS, HMI_ValueStruct.Home_OffY_scaled, -500, 500); }
  void HMI_HomeOffsetZ() { HMI_HomeOffsetN(Z_AXIS, HMI_ValueStruct.Home_OffZ_scaled,  -20,  20); }

#endif // HAS_HOME_OFFSET

#if HAS_ONESTEP_LEVELING

  void HMI_ProbeOffset() {
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
          select_advset.set(ADVSET_CASE_PROBEOFF);
          Draw_AdvSet_Menu_old();
          break;
        case 1: // Probe Offset X
          checkkey = ProbeOffX;
          DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 1, 216, MBASE(1), HMI_ValueStruct.Probe_OffX_scaled);
          EncoderRate.enabled = true;
          break;
        case 2: // Probe Offset X
          checkkey = ProbeOffY;
          DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 1, 216, MBASE(2), HMI_ValueStruct.Probe_OffY_scaled);
          EncoderRate.enabled = true;
          break;
      }
    }
    DWIN_UpdateLCD();
  }

  void HMI_ProbeOffsetN(float &posScaled, float &offset_ref) {
    ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState != ENCODER_DIFF_NO) {
      if (Apply_Encoder(encoder_diffState, posScaled)) {
        checkkey = ProbeOff;
        EncoderRate.enabled = false;
        offset_ref = posScaled / 10;
        return DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, 1, 216, MBASE(select_item.now), posScaled);
      }
      LIMIT(posScaled, -500, 500);
      DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, UNITFDIGITS, 216, MBASE(select_item.now), posScaled);
    }
  }

  void HMI_ProbeOffsetX() { HMI_ProbeOffsetN(HMI_ValueStruct.Probe_OffX_scaled, probe.offset.x); }
  void HMI_ProbeOffsetY() { HMI_ProbeOffsetN(HMI_ValueStruct.Probe_OffY_scaled, probe.offset.y); }

#endif // HAS_ONESTEP_LEVELING

void HMI_Brightness_old() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Brightness)) {
      checkkey = AdvSet;
      EncoderRate.enabled = false;
      HMI_data.Brightness = HMI_ValueStruct.Brightness;
      return Draw_Menu_IntValue(HMI_data.Background_Color, ADVSET_CASE_BRIGHTNESS + MROWS - index_advset, 3, HMI_ValueStruct.Brightness);
    }
    LIMIT(HMI_ValueStruct.Brightness, 10, 250);
    DWIN_LCD_Brightness(HMI_ValueStruct.Brightness);
    Draw_Menu_IntValue(HMI_data.Selected_Color, ADVSET_CASE_BRIGHTNESS + MROWS - index_advset, 3, HMI_ValueStruct.Brightness);
  }
}

void HMI_SelectColor() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_scolor.inc(1 + SCOLOR_CASE_TOTAL)) {
      if (select_scolor.now > MROWS && select_scolor.now > index_selcolor) {
        index_selcolor = select_scolor.now;

        // Scroll up and draw a blank bottom line
        Scroll_Menu(DWIN_SCROLL_UP);

        switch (index_selcolor) {  // Last menu items
          case SCOLOR_CASE_TEXT:       Draw_Color_Item(MROWS, "Text", HMI_data.Text_Color);                   break;
          case SCOLOR_CASE_SELECT:     Draw_Color_Item(MROWS, "Selected", HMI_data.Selected_Color);           break;
          case SCOLOR_CASE_SLINE:      Draw_Color_Item(MROWS, "Split Line", HMI_data.SplitLine_Color);        break;
          case SCOLOR_CASE_HIGHLIGHT:  Draw_Color_Item(MROWS, "Highlight", HMI_data.Highlight_Color);         break;
          case SCOLOR_CASE_STATUSBG:   Draw_Color_Item(MROWS, "Status Background", HMI_data.StatusBg_Color);  break;
          case SCOLOR_CASE_STATUSTXT:  Draw_Color_Item(MROWS, "Status Text", HMI_data.StatusTxt_Color);       break;
          case SCOLOR_CASE_POPUPBG:    Draw_Color_Item(MROWS, "Popup Background", HMI_data.PopupBg_color);    break;
          case SCOLOR_CASE_POPUPTXT:   Draw_Color_Item(MROWS, "Popup Text", HMI_data.PopupTxt_Color);         break;
          case SCOLOR_CASE_ALERTBG:    Draw_Color_Item(MROWS, "Alert Background", HMI_data.AlertBg_Color);    break;
          case SCOLOR_CASE_ALERTTXT:   Draw_Color_Item(MROWS, "Alert Text", HMI_data.AlertTxt_Color);         break;
          case SCOLOR_CASE_PERCNTTXT:  Draw_Color_Item(MROWS, "Percent Text", HMI_data.PercentTxt_Color);     break;
          case SCOLOR_CASE_BARFILL:    Draw_Color_Item(MROWS, "Bar Fill", HMI_data.Barfill_Color);            break;
          case SCOLOR_CASE_INDICATOR:  Draw_Color_Item(MROWS, "Indicator value", HMI_data.Indicator_Color);   break;
          case SCOLOR_CASE_COORDINATE: Draw_Color_Item(MROWS, "Coordinate value", HMI_data.Coordinate_Color); break;
          default: break;
        }

      }
      else
        Move_Highlight(1, select_scolor.now + MROWS - index_selcolor);
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_scolor.dec()) {
      if (select_scolor.now < index_selcolor - MROWS) {
        index_selcolor--;
        Scroll_Menu(DWIN_SCROLL_DOWN);

        switch (index_selcolor) {  // First menu items
          case MROWS:                       Draw_Back_First();                                                break;
          case MROWS+SCOLOR_CASE_LOADDEF:   Draw_Menu_Line(0, ICON_StockConfiguraton, GET_TEXT(MSG_RESTORE_DEFAULTS)); break;
          case MROWS+SCOLOR_CASE_BACKG:     Draw_Color_Item(0, "Screen Background", HMI_data.Background_Color); break;
          case MROWS+SCOLOR_CASE_CURSOR:    Draw_Color_Item(0, "Cursor",  HMI_data.Cursor_color);             break;
          case MROWS+SCOLOR_CASE_TITLEGB:   Draw_Color_Item(0, "Title Background", HMI_data.TitleBg_color);   break;
          case MROWS+SCOLOR_CASE_TITLETXT:  Draw_Color_Item(0, "Title Text", HMI_data.TitleTxt_color);        break;
          case MROWS+SCOLOR_CASE_TEXT:      Draw_Color_Item(0, "Text", HMI_data.Text_Color);                  break;
          case MROWS+SCOLOR_CASE_SELECT:    Draw_Color_Item(0, "Selected", HMI_data.Selected_Color);          break;
          case MROWS+SCOLOR_CASE_SLINE:     Draw_Color_Item(0, "Split Line", HMI_data.SplitLine_Color);       break;
          case MROWS+SCOLOR_CASE_HIGHLIGHT: Draw_Color_Item(0, "Highlight", HMI_data.Highlight_Color);        break;
          case MROWS+SCOLOR_CASE_STATUSBG:  Draw_Color_Item(0, "Status Background", HMI_data.StatusBg_Color); break;
          case MROWS+SCOLOR_CASE_STATUSTXT: Draw_Color_Item(0, "Status Text", HMI_data.StatusTxt_Color);      break;
          case MROWS+SCOLOR_CASE_POPUPBG:   Draw_Color_Item(0, "Popup Background", HMI_data.PopupBg_color);   break;
          case MROWS+SCOLOR_CASE_POPUPTXT:  Draw_Color_Item(0, "Popup Text", HMI_data.PopupTxt_Color);        break;
          default: break;
        }
      }
      else {
        Move_Highlight(-1, select_scolor.now + MROWS - index_selcolor);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    uint16_t color = 0;
    switch (select_scolor.now) {
      case 0: // Back
        checkkey = AdvSet;
        select_advset.set(ADVSET_CASE_SCOLOR);
        index_advset = ADVSET_CASE_SCOLOR;
        Draw_AdvSet_Menu_old();
        break;
      case SCOLOR_CASE_LOADDEF:
        DWIN_SetColorDefaults();
        DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color);
        Draw_SelColor_Menu();
        Draw_Status_Area(false);
        break;
      case SCOLOR_CASE_BACKG:       color = HMI_data.Background_Color;  break;
      case SCOLOR_CASE_CURSOR:      color = HMI_data.Cursor_color;      break;
      case SCOLOR_CASE_TITLEGB:     color = HMI_data.TitleBg_color;     break;
      case SCOLOR_CASE_TITLETXT:    color = HMI_data.TitleTxt_color;    break;
      case SCOLOR_CASE_TEXT:        color = HMI_data.Text_Color;        break;
      case SCOLOR_CASE_SELECT:      color = HMI_data.Selected_Color;    break;
      case SCOLOR_CASE_SLINE:       color = HMI_data.SplitLine_Color;   break;
      case SCOLOR_CASE_HIGHLIGHT:   color = HMI_data.Highlight_Color;   break;
      case SCOLOR_CASE_STATUSBG:    color = HMI_data.StatusBg_Color;    break;
      case SCOLOR_CASE_STATUSTXT:   color = HMI_data.StatusTxt_Color;   break;
      case SCOLOR_CASE_POPUPBG:     color = HMI_data.PopupBg_color;     break;
      case SCOLOR_CASE_POPUPTXT:    color = HMI_data.PopupTxt_Color;    break;
      case SCOLOR_CASE_ALERTBG:     color = HMI_data.AlertBg_Color;     break;
      case SCOLOR_CASE_ALERTTXT:    color = HMI_data.AlertTxt_Color;    break;
      case SCOLOR_CASE_PERCNTTXT:   color = HMI_data.PercentTxt_Color;  break;
      case SCOLOR_CASE_BARFILL:     color = HMI_data.Barfill_Color;     break;
      case SCOLOR_CASE_INDICATOR:   color = HMI_data.Indicator_Color;   break;
      case SCOLOR_CASE_COORDINATE:  color = HMI_data.Coordinate_Color;  break;
      default: break;
    }
    if (select_scolor.now > SCOLOR_CASE_LOADDEF) {
      checkkey = GetColor;
      select_item.reset();
      Draw_GetColor(color);
    }
  }
  DWIN_UpdateLCD();
}

void HMI_ApplyColor(const int8_t element, const bool ldef = false) {
  const uint16_t color = RGB(HMI_ValueStruct.Color[2], HMI_ValueStruct.Color[1], HMI_ValueStruct.Color[0]);
  switch (element) {
    case SCOLOR_CASE_BACKG:      HMI_data.Background_Color = ldef ? Def_Background_Color : color; DWINUI::SetBackgroundColor(HMI_data.Background_Color); break;
    case SCOLOR_CASE_CURSOR:     HMI_data.Cursor_color     = ldef ? Def_Cursor_color     : color; break;
    case SCOLOR_CASE_TITLEGB:    HMI_data.TitleBg_color    = ldef ? Def_TitleBg_color    : color; break;
    case SCOLOR_CASE_TITLETXT:   HMI_data.TitleTxt_color   = ldef ? Def_TitleTxt_color   : color; break;
    case SCOLOR_CASE_TEXT:       HMI_data.Text_Color       = ldef ? Def_Text_Color       : color; DWINUI::SetTextColor(HMI_data.Text_Color); break;
    case SCOLOR_CASE_SELECT:     HMI_data.Selected_Color   = ldef ? Def_Selected_Color   : color; break;
    case SCOLOR_CASE_SLINE:      HMI_data.SplitLine_Color  = ldef ? Def_SplitLine_Color  : color; break;
    case SCOLOR_CASE_HIGHLIGHT:  HMI_data.Highlight_Color  = ldef ? Def_Highlight_Color  : color; break;
    case SCOLOR_CASE_STATUSBG:   HMI_data.StatusBg_Color   = ldef ? Def_StatusBg_Color   : color; break;
    case SCOLOR_CASE_STATUSTXT:  HMI_data.StatusTxt_Color  = ldef ? Def_StatusTxt_Color  : color; break;
    case SCOLOR_CASE_POPUPBG:    HMI_data.PopupBg_color    = ldef ? Def_PopupBg_color    : color; break;
    case SCOLOR_CASE_POPUPTXT:   HMI_data.PopupTxt_Color   = ldef ? Def_PopupTxt_Color   : color; break;
    case SCOLOR_CASE_ALERTBG:    HMI_data.AlertBg_Color    = ldef ? Def_AlertBg_Color    : color; break;
    case SCOLOR_CASE_ALERTTXT:   HMI_data.AlertTxt_Color   = ldef ? Def_AlertTxt_Color   : color; break;
    case SCOLOR_CASE_PERCNTTXT:  HMI_data.PercentTxt_Color = ldef ? Def_PercentTxt_Color : color; break;
    case SCOLOR_CASE_BARFILL:    HMI_data.Barfill_Color    = ldef ? Def_Barfill_Color    : color; break;
    case SCOLOR_CASE_INDICATOR:  HMI_data.Indicator_Color  = ldef ? Def_Indicator_Color  : color; break;
    case SCOLOR_CASE_COORDINATE: HMI_data.Coordinate_Color = ldef ? Def_Coordinate_Color : color; break;
    default: break;
  }
}

void HMI_GetColor() {
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
      case 0:       // Back
        HMI_ApplyColor(select_scolor.now);
        Draw_Status_Area(false);
        checkkey = SelColor;
        Draw_SelColor_Menu();
        DWIN_StatusChanged_P(PSTR("Colors applied"));
        break;
      case 1 ... 3: // C0-C2;
        checkkey = GetColor_value;
        Draw_Menu_IntValue(HMI_data.Selected_Color, select_item.now, 2, HMI_ValueStruct.Color[select_item.now - 1]);
        break;
      case 4:       // Restore default color
        HMI_ApplyColor(select_scolor.now, true);
        Draw_Status_Area(false);
        checkkey = SelColor;
        Draw_SelColor_Menu();
        break;
      default:
        break;
    }
  }
  DWIN_UpdateLCD();
}

void HMI_GetColorValue() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    const uint8_t line = select_item.now - 1;
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Color[line])) {
      checkkey = GetColor;
      EncoderRate.enabled = false;
      return Draw_Menu_IntValue(HMI_data.Background_Color, select_item.now, 2, HMI_ValueStruct.Color[line]);
    }
    LIMIT(HMI_ValueStruct.Color[line], 0, (line == 1) ? 63 : 31);
    uint16_t color = HMI_ValueStruct.Color[2]*0x0800 + HMI_ValueStruct.Color[1]*0x0020 + HMI_ValueStruct.Color[0];
    Draw_Menu_IntValue(HMI_data.Selected_Color, select_item.now, 2, HMI_ValueStruct.Color[line]);
    DWIN_Draw_Rectangle(1, color, 20, 315, DWIN_WIDTH - 20, 335);
  }
}

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

void HMI_Tune() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_tune.inc(1 + TUNE_CASE_TOTAL)) {
      if (select_tune.now > MROWS && select_tune.now > index_tune) {
        index_tune = select_tune.now;

        // Scroll up and draw a blank bottom line
        Scroll_Menu(DWIN_SCROLL_UP);

        switch (index_tune) {  // Last menu items
          case TUNE_CASE_FLOW:      // Tune Flow
            Draw_Menu_Line(MROWS, ICON_Flow, GET_TEXT(MSG_FLOW), false);
            Draw_Menu_IntValue(HMI_data.Background_Color, MROWS, 3, planner.flow_percentage[0]);
            break;
          #if ENABLED(ADVANCED_PAUSE_FEATURE)
            case TUNE_CASE_FCHNG:   // Filament change
              Draw_Menu_Line(MROWS, ICON_FilMan, GET_TEXT(MSG_FILAMENTCHANGE), false);
              break;
          #endif
          case TUNE_CASE_LOCK: // Lock Screen
            Draw_Menu_Line(MROWS, ICON_Lock, "Screen Lock", true);
            break;
          default: break;
        }

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

        switch (index_tune) {   // First menu items
          case MROWS:
            Draw_Back_First();
            break;
          case MROWS + TUNE_CASE_SPEED:
            Draw_Menu_Line(0, ICON_Speed, GET_TEXT(MSG_SPEED), false);
            Draw_Menu_IntValue(HMI_data.Background_Color, 0, 3, feedrate_percentage);
            break;
          #if HAS_HOTEND
            case MROWS + TUNE_CASE_TEMP:
              Draw_Menu_Line(0, ICON_HotendTemp, GET_TEXT(MSG_UBL_SET_TEMP_HOTEND), false);
              Draw_Menu_IntValue(HMI_data.Background_Color, 0, 3, thermalManager.degTargetHotend(0));
              break;
          #endif
          #if HAS_HEATED_BED
          case MROWS + TUNE_CASE_BED:
            Draw_Menu_Line(0, ICON_BedTemp, GET_TEXT(MSG_UBL_SET_TEMP_BED), false);
            Draw_Menu_IntValue(HMI_data.Background_Color, 0, 3, thermalManager.degTargetBed());
            break;
          #endif
        }
      }
      else
        Move_Highlight(-1, select_tune.now + MROWS - index_tune);
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_tune.now) {
      case 0:  // Back
        select_print.set(0);
        Goto_PrintProcess();
        break;
      case TUNE_CASE_SPEED: // Print speed
        checkkey = PrintSpeed;
        HMI_ValueStruct.print_speed = feedrate_percentage;
        DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(TUNE_CASE_SPEED + MROWS - index_tune), HMI_ValueStruct.print_speed);
        EncoderRate.enabled = true;
        break;
      #if HAS_HOTEND
        case TUNE_CASE_TEMP: // Nozzle temp
          checkkey = ETemp;
          HMI_ValueStruct.E_Temp = thermalManager.degTargetHotend(0);
          DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(TUNE_CASE_TEMP + MROWS - index_tune), HMI_ValueStruct.E_Temp);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_HEATED_BED
        case TUNE_CASE_BED: // Bed temp
          checkkey = BedTemp;
          HMI_ValueStruct.Bed_Temp = thermalManager.degTargetBed();
          DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(TUNE_CASE_BED + MROWS - index_tune), HMI_ValueStruct.Bed_Temp);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_FAN
        case TUNE_CASE_FAN: // Fan speed
          checkkey = FanSpeed;
          HMI_ValueStruct.Fan_speed = thermalManager.fan_speed[0];
          DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(TUNE_CASE_FAN + MROWS - index_tune), HMI_ValueStruct.Fan_speed);
          EncoderRate.enabled = true;
          break;
      #endif
      #if HAS_ZOFFSET_ITEM
        case TUNE_CASE_ZOFF: // Z-offset
          #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
            checkkey = Zoffset;
            HMI_ValueStruct.offset_value = BABY_Z_VAR * 100;
            DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 2, 2, 202, MBASE(TUNE_CASE_ZOFF + MROWS - index_tune), HMI_ValueStruct.offset_value);
            EncoderRate.enabled = true;
          #else
            queue.inject_P(PSTR("G92 X0 Y0 Z0")); // Apply workspace offset, making the current position 0,0,0
            HMI_AudioFeedback();
          #endif
        break;
      #endif
      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        case TUNE_CASE_FCHNG:
          Goto_PrintProcess();
          HMI_SaveProcessID(NothingToDo);
          queue.inject_P(PSTR("M600 B2"));
          break;
      #endif
      case TUNE_CASE_FLOW: // Flow rate
        checkkey = TuneFlow;
        HMI_ValueStruct.print_flow = planner.flow_percentage[0];
        Draw_Menu_IntValue(HMI_data.Selected_Color, TUNE_CASE_FLOW + MROWS - index_tune, 3, planner.flow_percentage[0]);
        EncoderRate.enabled = true;
        break;
      case TUNE_CASE_LOCK: // Screen Lock
        DWIN_LockScreen(false);      
        break;
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

inline void HMI_TuneFlow() { HMI_Flow(Tune, MBASE(TUNE_CASE_FLOW + MROWS - index_tune)); }

#if HAS_PREHEAT

  void HMI_PLAPreheatSetting() {
    ENCODER_DiffState encoder_diffState = get_encoder_state();
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
            DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(PREHEAT_CASE_TEMP), ui.material_preset[0].hotend_temp);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_HEATED_BED
          case PREHEAT_CASE_BED: // Bed temperature
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = ui.material_preset[0].bed_temp;
            DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(PREHEAT_CASE_BED), ui.material_preset[0].bed_temp);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_FAN
          case PREHEAT_CASE_FAN: // Fan speed
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = ui.material_preset[0].fan_speed;
            DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(PREHEAT_CASE_FAN), ui.material_preset[0].fan_speed);
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

  void HMI_ABSPreheatSetting() {
    ENCODER_DiffState encoder_diffState = get_encoder_state();
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
            DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(PREHEAT_CASE_TEMP), ui.material_preset[1].hotend_temp);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_HEATED_BED
          case PREHEAT_CASE_BED: // Set bed temperature
            checkkey = BedTemp;
            HMI_ValueStruct.Bed_Temp = ui.material_preset[1].bed_temp;
            DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(PREHEAT_CASE_BED), ui.material_preset[1].bed_temp);
            EncoderRate.enabled = true;
            break;
        #endif
        #if HAS_FAN
          case PREHEAT_CASE_FAN: // Set fan speed
            checkkey = FanSpeed;
            HMI_ValueStruct.Fan_speed = ui.material_preset[1].fan_speed;
            DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 216, MBASE(PREHEAT_CASE_FAN), ui.material_preset[1].fan_speed);
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

void HMI_MaxSpeed() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
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
      DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 4, 210, MBASE(select_speed.now), HMI_ValueStruct.Max_Feedspeed);
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

void HMI_MaxAcceleration() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
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
      DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 4, 210, MBASE(select_acc.now), HMI_ValueStruct.Max_Acceleration);
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

  void HMI_MaxJerk() {
    ENCODER_DiffState encoder_diffState = get_encoder_state();
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
        DWINUI::Draw_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, UNITFDIGITS, 210, MBASE(select_jerk.now), HMI_ValueStruct.Max_Jerk_scaled);
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

void HMI_Step() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
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
      DWINUI::Draw_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, UNITFDIGITS, 210, MBASE(select_step.now), HMI_ValueStruct.Max_Step_scaled);
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
    wait_for_user = false;
    HMI_ReturnScreen();
  }
}

void HMI_Init() {
  HMI_SDCardInit();

  for (uint16_t t = 0; t <= 100; t += 2) {
    DWINUI::Draw_Icon(ICON_Bar, 15, 260);
    DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 15 + t * 242 / 100, 260, 257, 280);
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
    if (HMI_flag.print_finish) {
      HMI_flag.print_finish = false;
      TERN_(POWER_LOSS_RECOVERY, recovery.cancel());
      planner.finish_and_disable();
      checkkey = PrintDone;
      Draw_PrintDone();
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
        TERN_(HAS_HOTEND, resume_hotend_temp = thermalManager.degTargetHotend(0));
        TERN_(HAS_HEATED_BED, resume_bed_temp = thermalManager.degTargetBed());
      }
      else {
        TERN_(HAS_HOTEND, resume_hotend_temp = thermalManager.wholeDegHotend(0));
        TERN_(HAS_HEATED_BED, resume_bed_temp = thermalManager.wholeDegBed());
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
          _percent_done = percentDone;
          Draw_Print_ProgressBar();
        }
      }

      // Estimate remaining time every 20 seconds
      static millis_t next_remain_time_update = 0;
      if (_percent_done > 1 && ELAPSED(ms, next_remain_time_update) && !HMI_flag.heat_flag) {
        _remain_time = (elapsed.value - dwin_heat_time) / (_percent_done * 0.01f) - (elapsed.value - dwin_heat_time);
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
        const uint16_t c1 = sel ? HMI_data.PopupBg_color : HMI_data.Highlight_Color;
        DWIN_Draw_Rectangle(0, c1, 25, 306, 126, 345);
        DWIN_Draw_Rectangle(0, c1, 24, 305, 127, 346);
        const uint16_t c2 = sel ? HMI_data.Highlight_Color : HMI_data.PopupBg_color;
        DWIN_Draw_Rectangle(0, c2, 145, 306, 246, 345);
        DWIN_Draw_Rectangle(0, c2, 144, 305, 247, 346);
      };

      Popup_Window_Resume();
      update_selection(true);

      // TODO: Get the name of the current file from someplace
      //
      //(void)recovery.interrupted_file_exists();
      SdFile *dir = nullptr;
      const char * const filename = card.diveToFile(true, dir, recovery.info.sd_filename);
      card.selectFileByName(filename);
      DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 252, card.longest_filename());
      DWIN_UpdateLCD();

      while (recovery_flag) {
        ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
        if (encoder_diffState != ENCODER_DIFF_NO) {
          if (encoder_diffState == ENCODER_DIFF_ENTER) {
            recovery_flag = false;
            if (HMI_flag.select_flag) break;
            TERN_(POWER_LOSS_RECOVERY, queue.inject_P(PSTR("M1000C")));
            return HMI_StartFrame(true);
          }
          else
            update_selection(encoder_diffState == ENCODER_DIFF_CW);

          DWIN_UpdateLCD();
        }
        watchdog_refresh();
      }

      select_print.set(0);
      HMI_ValueStruct.show_mode = 0;
      queue.inject_P(PSTR("M1000"));
      sdprint = true;
      Goto_PrintProcess();
      Draw_Status_Area(true);
    }
  #endif // POWER_LOSS_RECOVERY

  DWIN_UpdateLCD();
}

void DWIN_HandleScreen() {
  switch (checkkey) {
    case MainMenu:        HMI_MainMenu(); break;
    case Menu:            HMI_Menu(); break;

    case SelectFile:      HMI_SelectFile(); break;
    case Control:         HMI_Control(); break;
    case Homing:          break;
    case Leveling:        break;
    case PrintProcess:    HMI_Printing(); break;
    case PrintDone:       HMI_PrintDone(); break;
    case PauseOrStop:     HMI_PauseOrStop(); break;
    #if ENABLED(ASSISTED_TRAMMING)
      case Tramming:      HMI_Tramming(); break;
    #endif
    #if ENABLED(MESH_BED_LEVELING)
      case ManualMesh:    HMI_ManualMesh(); break;
      case MMeshMoveZ:    HMI_MMeshMoveZ(); break;
    #endif
    case TemperatureID:   HMI_Temperature(); break;
    case Motion:          HMI_Motion(); break;
    case AdvSet:          HMI_AdvSet(); break;
    case Brightness:      HMI_Brightness(); break;
    #if HAS_HOME_OFFSET
      case HomeOff:       HMI_HomeOffset(); break;
      case HomeOffX:      HMI_HomeOffsetX(); break;
      case HomeOffY:      HMI_HomeOffsetY(); break;
      case HomeOffZ:      HMI_HomeOffsetZ(); break;
    #endif
    #if HAS_ONESTEP_LEVELING
      case ProbeOff:      HMI_ProbeOffset(); break;
      case ProbeOffX:     HMI_ProbeOffsetX(); break;
      case ProbeOffY:     HMI_ProbeOffsetY(); break;
    #endif
    case SelColor:        HMI_SelectColor(); break;
    case GetColor:        HMI_GetColor(); break;
    case GetColor_value:  HMI_GetColorValue(); break;
    case Info:            HMI_Info(); break;
    case Tune:            HMI_Tune(); break;
    case TuneFlow:        HMI_TuneFlow(); break;
    #if HAS_PREHEAT
      case PLAPreheat:    HMI_PLAPreheatSetting(); break;
      case ABSPreheat:    HMI_ABSPreheatSetting(); break;
    #endif
    case MaxSpeed:        HMI_MaxSpeed(); break;
    case MaxSpeed_value:  HMI_MaxFeedspeedXYZE(); break;
    case MaxAcceleration: HMI_MaxAcceleration(); break;
    case MaxAcceleration_value: HMI_MaxAccelerationXYZE(); break;
    #if HAS_CLASSIC_JERK
      case MaxJerk:       HMI_MaxJerk(); break;
      case MaxJerk_value: HMI_MaxJerkXYZE(); break;
    #endif
    case Step:            HMI_Step(); break;
    case Step_value:      HMI_StepXYZE(); break;
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
    case WaitResponse:    HMI_Popup(); break;
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      case FilamentMan:   HMI_FilamentMan(); break;
      case FilamentPurge: HMI_FilamentPurge(); break;
    #endif
    case NothingToDo:     break;
    case Locked:          HMI_LockScreen(); break;
    default: break;
  }
}

void HMI_SaveProcessID(const uint8_t id) {
  if (checkkey != id) {
    if ((checkkey != NothingToDo) &&
        (checkkey != WaitResponse) &&
        (checkkey != Homing) &&
        (checkkey != Leveling) &&
        (checkkey != PauseOrStop) &&
        (checkkey != FilamentPurge)) last_checkkey = checkkey; // if not a popup
    checkkey = id;
  }
}

void DWIN_StartHoming() {
  HMI_flag.home_flag = true;
  HMI_SaveProcessID(Homing);
  DWIN_Draw_Popup(ICON_BLTouch, "Axis Homing", "Please wait until done.");
}

void DWIN_CompletedHoming() {
  HMI_flag.home_flag = false;
  dwin_zoffset = TERN0(HAS_BED_PROBE, probe.offset.z);
  if (dwin_abort_flag) {
    HMI_ValueStruct.print_speed = feedrate_percentage = 100;
    planner.finish_and_disable();
  }
  HMI_ReturnScreen();
}

void DWIN_MeshLevelingStart() {
  #if HAS_ONESTEP_LEVELING
    HMI_SaveProcessID(Leveling);
    DWIN_Draw_Popup(ICON_AutoLeveling, GET_TEXT(MSG_BED_LEVELING), "Please wait until done.");
  #elif ENABLED(MESH_BED_LEVELING)
    if (checkkey != ManualMesh) {
      HMI_SaveProcessID(ManualMesh);
      select_item.reset();
      Draw_ManualMesh_Menu();
    }
  #endif
}

void DWIN_CompletedLeveling() { HMI_ReturnScreen(); }

#if ENABLED(MESH_BED_LEVELING)
  void DWIN_MeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
    char msg[33] = "";
    char str_1[6] = "";
    sprintf_P(msg, PSTR(S_FMT " %i/%i Z=%s"), GET_TEXT(MSG_PROBING_MESH), xpos, ypos,
      dtostrf(zval, 1, 2, str_1));
    DWIN_StatusChanged(msg);
  }
#endif

// PID process
void DWIN_PidTuning(pidresult_t result) {
  switch (result) {
    case PID_BED_START:
      HMI_SaveProcessID(NothingToDo);
      DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT(MSG_PID_AUTOTUNE), "for BED is running.");
      break;
    case PID_EXTR_START:
      HMI_SaveProcessID(NothingToDo);
      DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT(MSG_PID_AUTOTUNE), "for Nozzle is running.");
      break;
    case PID_BAD_EXTRUDER_NUM:
      checkkey = last_checkkey;
      DWIN_Popup_Confirm(ICON_TempTooLow, "PID Autotune failed!", "Bad extruder");
      break;
    case PID_TUNING_TIMEOUT:
      checkkey = last_checkkey;
      DWIN_Popup_Confirm(ICON_TempTooHigh, "Error", GET_TEXT(MSG_PID_TIMEOUT));
      break;
    case PID_TEMP_TOO_HIGH:
      checkkey = last_checkkey;
      DWIN_Popup_Confirm(ICON_TempTooHigh,"PID Autotune failed!","Temperature too high");
      break;
    case PID_DONE:
      checkkey = last_checkkey;
      DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT(MSG_PID_AUTOTUNE), GET_TEXT(MSG_BUTTON_DONE));
      break;
    default:
      checkkey = last_checkkey;
      break;
  }
}

// Update filename on print
void DWIN_Print_Header(const char *text = nullptr) {

  static char headertxt[31] = "";  // Print header text

  if (text != nullptr) {
    const int8_t size = _MIN((unsigned) 30, strlen_P(text));
    LOOP_L_N(i, size) headertxt[i] = text[i];
    headertxt[size] = '\0';
  }
  if (checkkey == PrintProcess) {
    DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, 60, DWIN_WIDTH, 60+16);
    DWINUI::Draw_CenteredString(60, headertxt);
  }
}

void Draw_Title(TitleClass* title) {
  DWIN_Draw_Rectangle(1, HMI_data.TitleBg_color, 0, 0, DWIN_WIDTH - 1, TITLE_HEIGHT - 1);
  if (title->frameid) 
    DWIN_Frame_AreaCopy(title->frameid, title->frame.left, title->frame.top, title->frame.right, title->frame.bottom, 14, (TITLE_HEIGHT - (title->frame.bottom - title->frame.top)) / 2 - 1);
  else
    DWIN_Draw_String(false, false, DWIN_FONT_HEAD, HMI_data.TitleTxt_color, HMI_data.TitleBg_color, 14, (TITLE_HEIGHT - DWINUI::Get_font_height(DWIN_FONT_HEAD)) / 2 - 1, title->caption);
}

// Startup routines
void DWIN_Startup() {
   DWINUI::Init();
   DWINUI::onCursorDraw = Draw_Menu_Cursor;
   DWINUI::onCursorErase = Erase_Menu_Cursor;
   Title.onDraw = Draw_Title;
   HMI_SetLanguage();
}

void DWIN_DrawStatusLine(const uint16_t color, const uint16_t bgcolor, const char *text) {
  DWIN_Draw_Rectangle(1, bgcolor, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
  if (text) DWINUI::Draw_CenteredString(color, STATUS_Y + 2, text);
  DWIN_UpdateLCD();
}

// Update Status line
void DWIN_StatusChanged(const char *text) {
  DWIN_DrawStatusLine(HMI_data.StatusTxt_Color, HMI_data.StatusBg_Color, text);
}

void DWIN_StatusChanged_P(PGM_P const pstr) {
  char str[strlen_P((const char*)pstr) + 1];
  strcpy_P(str, (const char*)pstr);
  DWIN_StatusChanged(str);
}

// Started a Print Job
void DWIN_Print_Started(const bool sd) {
  sdprint = card.isPrinting() || sd;
  _percent_done = 0;
  _remain_time = 0;
  Goto_PrintProcess();
}

// Ended print job
void DWIN_Print_Finished() {
  if (checkkey == PrintProcess || checkkey == Tune || printingIsActive()) {
    thermalManager.disable_all_heaters();
    thermalManager.zero_fan_speeds();
    HMI_flag.print_finish = true;
    Goto_PrintProcess();
  }
}

// Progress Bar update
void DWIN_Progress_Update() {
  if (parser.seen('P')) _percent_done = parser.byteval('P');
  if (parser.seen('R')) _remain_time = parser.ulongval('R') * 60;
  if (checkkey == PrintProcess) {
    Draw_Print_ProgressBar();
    Draw_Print_ProgressRemain();
    Draw_Print_ProgressElapsed();
  }
}

#if HAS_FILAMENT_SENSOR
  // Filament Runout process
  void DWIN_FilamentRunout(const uint8_t extruder) { DWIN_StatusChanged_P(GET_TEXT(MSG_RUNOUT_SENSOR)); }
#endif

void DWIN_SetColorDefaults() {
  HMI_data.Background_Color = Def_Background_Color;
  HMI_data.Cursor_color     = Def_Cursor_color;
  HMI_data.TitleBg_color    = Def_TitleBg_color;
  HMI_data.TitleTxt_color   = Def_TitleTxt_color;
  HMI_data.Text_Color       = Def_Text_Color;
  HMI_data.Selected_Color   = Def_Selected_Color;
  HMI_data.SplitLine_Color  = Def_SplitLine_Color;
  HMI_data.Highlight_Color  = Def_Highlight_Color;
  HMI_data.StatusBg_Color   = Def_StatusBg_Color;
  HMI_data.StatusTxt_Color  = Def_StatusTxt_Color;
  HMI_data.PopupBg_color    = Def_PopupBg_color;
  HMI_data.PopupTxt_Color   = Def_PopupTxt_Color;
  HMI_data.AlertBg_Color    = Def_AlertBg_Color;
  HMI_data.AlertTxt_Color   = Def_AlertTxt_Color;
  HMI_data.PercentTxt_Color = Def_PercentTxt_Color;
  HMI_data.Barfill_Color    = Def_Barfill_Color;
  HMI_data.Indicator_Color  = Def_Indicator_Color;
  HMI_data.Coordinate_Color = Def_Coordinate_Color;
}

void DWIN_Setdatadefaults() {
  HMI_data.Brightness = 127;
  DWIN_LCD_Brightness(HMI_data.Brightness);
  DWIN_SetColorDefaults();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color);
}

void DWIN_StoreSettings(char *buff) {
  memcpy(buff, &HMI_data, min(sizeof(HMI_data), eeprom_data_size));
}

void DWIN_LoadSettings(const char *buff) {
  memcpy(&HMI_data, buff, min(sizeof(HMI_data), eeprom_data_size));
  DWIN_LCD_Brightness(_MAX(10,HMI_data.Brightness));
  dwin_zoffset = TERN0(HAS_BED_PROBE, probe.offset.z);
  if (HMI_data.Text_Color == HMI_data.Background_Color) DWIN_SetColorDefaults();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color);
}

void MarlinUI::kill_screen(PGM_P lcd_error, PGM_P lcd_component) {
  DWIN_Draw_Popup(ICON_BLTouch, lcd_error, lcd_component);
  DWIN_UpdateLCD();
}

void DWIN_RebootScreen() {
  DWIN_Frame_Clear(Color_Bg_Black);
  DWINUI::Draw_Icon(ICON_LOGO, 71, 150);  // CREALITY logo
  DWINUI::Draw_CenteredString(Color_White, 200, F("Please wait until reboot."));
  DWIN_UpdateLCD();
  delay(500);
}

void DWIN_Redraw_screen() {
  Draw_Main_Area(checkkey);
  DWIN_StatusChanged(ui.status_message);
  Draw_Status_Area(false);
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void DWIN_Popup_Pause(const char *msg, uint8_t button = 0) {
    HMI_SaveProcessID(button ? WaitResponse : NothingToDo);
    DWIN_Draw_Popup(ICON_BLTouch, "Advanced Pause", msg, button);
    DWIN_StatusChanged(nullptr);
  }

  void MarlinUI::pause_show_message(const PauseMessage message, const PauseMode mode/*=PAUSE_MODE_SAME*/, const uint8_t extruder/*=active_extruder*/) {
    switch (message) {
      case PAUSE_MESSAGE_PARKING:  DWIN_Popup_Pause(GET_TEXT(MSG_PAUSE_PRINT_PARKING));    break;
      case PAUSE_MESSAGE_CHANGING: DWIN_Popup_Pause(GET_TEXT(MSG_FILAMENT_CHANGE_INIT));   break;
      case PAUSE_MESSAGE_UNLOAD:   DWIN_Popup_Pause(GET_TEXT(MSG_FILAMENT_CHANGE_UNLOAD)); break;
      case PAUSE_MESSAGE_WAITING:  DWIN_Popup_Pause(GET_TEXT(MSG_ADVANCED_PAUSE_WAITING), ICON_Continue_E); break;
      case PAUSE_MESSAGE_INSERT:   DWIN_Popup_Continue(ICON_BLTouch, "Advanced Pause", GET_TEXT(MSG_FILAMENT_CHANGE_INSERT)); break;
      case PAUSE_MESSAGE_LOAD:     DWIN_Popup_Pause(GET_TEXT(MSG_FILAMENT_CHANGE_LOAD));   break;
      case PAUSE_MESSAGE_PURGE:    DWIN_Popup_Pause(GET_TEXT(MSG_FILAMENT_CHANGE_PURGE));  break;
      case PAUSE_MESSAGE_OPTION:   DWIN_Popup_FilamentPurge(); break;
      case PAUSE_MESSAGE_RESUME:   DWIN_Popup_Pause(GET_TEXT(MSG_FILAMENT_CHANGE_RESUME)); break;
      case PAUSE_MESSAGE_HEAT:     DWIN_Popup_Pause(GET_TEXT(MSG_FILAMENT_CHANGE_HEAT), ICON_Continue_E);   break;
      case PAUSE_MESSAGE_HEATING:  DWIN_StatusChanged_P(GET_TEXT(MSG_FILAMENT_CHANGE_HEATING)); break;
      case PAUSE_MESSAGE_STATUS:   HMI_ReturnScreen(); break;
      default: break;
    }
  }

  void Draw_Popup_FilamentPurge() {
    DWIN_Draw_Popup(ICON_BLTouch, "Advanced Pause", "Purge or Continue?");
    DWINUI::Draw_Icon(ICON_Confirm_E, 26, 280);
    DWINUI::Draw_Icon(ICON_Continue_E, 146, 280);
    Draw_Select_Highlight(true);
  }

  // Handle responses such as:
  //  - Purge More, Continue
  //  - General "Continue" response
  void DWIN_Popup_FilamentPurge() {
    HMI_SaveProcessID(FilamentPurge);
    pause_menu_response = PAUSE_RESPONSE_WAIT_FOR;
    Draw_Popup_FilamentPurge();
  }

  void HMI_FilamentPurge() {
    ENCODER_DiffState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    if (encoder_diffState == ENCODER_DIFF_CW)
      Draw_Select_Highlight(false);
    else if (encoder_diffState == ENCODER_DIFF_CCW)
      Draw_Select_Highlight(true);
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      if (HMI_flag.select_flag)
        pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;  // "Purge More" button
      else {
        HMI_SaveProcessID(NothingToDo);
        pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;  // "Continue" button
      }
    }
    DWIN_UpdateLCD();
  }

#endif // ADVANCED_PAUSE_FEATURE

void HMI_LockScreen() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  LockScreen.onEncoderState(encoder_diffState);
  if (LockScreen.isUnlocked()) {
    if (HMI_flag.lock_flag) {
      checkkey = AdvSet;
      select_advset.set(ADVSET_CASE_LOCK);
      index_advset = ADVSET_CASE_LOCK;
      Draw_AdvSet_Menu_old();
    }
    else {
      checkkey = Tune;
      select_tune.set(TUNE_CASE_LOCK);
      index_tune = TUNE_CASE_LOCK;
      Draw_Tune_Menu();
    }
  }
}

void DWIN_LockScreen(const bool flag) {
  HMI_flag.lock_flag = flag;
  checkkey = Locked;
  LockScreen.Init();
}

// NEW MENU SUBSYSTEM =========================================================

void HMI_Menu() {
  ENCODER_DiffState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (CurrentMenu != nullptr) CurrentMenu->onClick();
  } else if (CurrentMenu != nullptr) CurrentMenu->onScroll(encoder_diffState == ENCODER_DIFF_CW);
}

// Get an integer value
//  lo: low limit
//  hi: high limit
// Return value:
//  0 : no change
//  1 : live change
//  2 : apply change
int8_t HMI_GetInt(const int16_t lo, const int16_t hi) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Value)) {
      EncoderRate.enabled = false;
      DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Background_Color, 3 , VALX, MBASE(CurrentMenu->line()) - 1, HMI_ValueStruct.Value);
      return 2;
    }
    LIMIT(HMI_ValueStruct.Value, lo, hi);
    DWINUI::Draw_Int(HMI_data.Text_Color, HMI_data.Selected_Color, 3 , VALX, MBASE(CurrentMenu->line()) - 1, HMI_ValueStruct.Value);
    return 1;
  }
  return 0;
}

// Get an scaled float value
//  dp: decimal points
//  lo: low limit
//  hi: high limit
// Return value:
//  0 : no change
//  1 : live change
//  2 : apply change
int8_t HMI_GetFloat(uint8_t dp, int16_t lo, int16_t hi) {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState != ENCODER_DIFF_NO) {
    if (Apply_Encoder(encoder_diffState, HMI_ValueStruct.Value)) {
      EncoderRate.enabled = false;
      DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, dp, VALX - (dp - 1) * DWINUI::Get_font_width(DWIN_FONT_MENU), MBASE(CurrentMenu->line()), HMI_ValueStruct.Value);
      return 2;
    }
    LIMIT(HMI_ValueStruct.Value, lo, hi);
    DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, dp, VALX - (dp - 1) * DWINUI::Get_font_width(DWIN_FONT_MENU), MBASE(CurrentMenu->line()), HMI_ValueStruct.Value);
    return 1;
  }
  return 0;
}

void onDrawMenuItem(MenuItemClass* menuitem, int8_t line) {
  if (menuitem->icon) DWINUI::Draw_Icon(menuitem->icon, ICOX, MBASE(line) - 3);
  if (menuitem->frameid) 
    DWIN_Frame_AreaCopy(menuitem->frameid, menuitem->frame.left, menuitem->frame.top, menuitem->frame.right, menuitem->frame.bottom, LBLX, MBASE(line));
  else if (menuitem->caption) 
    DWINUI::Draw_String(LBLX, MBASE(line) - 1, menuitem->caption);
  DWIN_Draw_HLine(HMI_data.SplitLine_Color, 16, MYPOS(line + 1), 240);
}

void onDrawSubMenu(MenuItemClass* menuitem, int8_t line) {
  onDrawMenuItem(menuitem, line);
  DWINUI::Draw_Icon(ICON_More, VALX + 10, MBASE(line) - 3);
}

void onDrawIntegerMenu(MenuItemClass* menuitem, int8_t line, uint16_t value) {
  onDrawMenuItem(menuitem, line);
  Draw_Menu_IntValue(HMI_data.Background_Color, line, 3, value);
}

void onDrawFloatMenu(MenuItemClass* menuitem, int8_t line, uint8_t dp, float value) {
  onDrawMenuItem(menuitem, line);
  DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, dp, VALX - (dp - 1) * DWINUI::Get_font_width(DWIN_FONT_MENU), MBASE(line), value * pow(10, dp));
}

void onDrawBack(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 129, 72, 156, 84);
  onDrawMenuItem(menuitem, line);
}

// Operational functions ======================================================

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void Goto_FilamentMan() {
    checkkey = FilamentMan;
    select_item.reset();
    Draw_FilamentMan_Menu();
  }
#endif

void Goto_ManualLev() {
  checkkey = Tramming;
  select_item.reset();
  Draw_Tramming_Menu();
}

#if ENABLED(MESH_BED_LEVELING)
  void Goto_ManualMesh() {
    checkkey = ManualMesh;
    select_item.reset();
    Draw_ManualMesh_Menu();
  }
#endif

void DisableMotors() {
  queue.inject_P(PSTR("M84"));
}

void AutoHome() {
  queue.inject_P(G28_STR);
}

#if HAS_ZOFFSET_ITEM
  void SetZOffset() {
    #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
      HMI_ValueStruct.show_mode = -4;
      HMI_ValueStruct.Value = BABY_Z_VAR * 100;
      EncoderRate.enabled = true;
      last_checkkey = checkkey;
      checkkey = Zoffset;
      DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, 2, VALX - DWINUI::Get_font_width(DWIN_FONT_MENU), MBASE(CurrentMenu->line()), HMI_ValueStruct.Value);
    #endif
  }
#endif

void SetHomeOffset() {
    // Apply workspace offset, making the current position 0,0,0
    queue.inject_P(PSTR("G92 X0 Y0 Z0"));
    HMI_AudioFeedback();
}

#if HAS_PREHEAT
  void SetPreheat(const uint8_t i) {
    TERN_(HAS_HOTEND, thermalManager.setTargetHotend(ui.material_preset[i].hotend_temp, 0));
    TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(ui.material_preset[i].bed_temp));
    TERN_(HAS_FAN, thermalManager.set_fan_speed(0, ui.material_preset[i].fan_speed));
  }
  void SetPreheat0() { SetPreheat(0); }
  void SetPreheat1() { SetPreheat(1); }
  void SetPreheat2() { SetPreheat(2); }

  void SetCoolDown() {
    TERN_(HAS_FAN, thermalManager.zero_fan_speeds());
    #if HAS_HOTEND || HAS_HEATED_BED
      thermalManager.disable_all_heaters();
    #endif
  }
#endif

void SetLanguage() {
  HMI_ToggleLanguage();
  CurrentMenu = nullptr;
  Draw_Prepare_Menu();
}

void SetMoveAxis(uint8_t process, const int16_t pos) {
  HMI_ValueStruct.Value = pos;
  EncoderRate.enabled = true;
  last_checkkey = checkkey;
  checkkey = process;
  DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Selected_Color, 3, UNITFDIGITS, VALX, MBASE(CurrentMenu->line()), HMI_ValueStruct.Value);
}
void SetMoveX() { SetMoveAxis(Move_X, current_position.x * MINUNITMULT);}
void SetMoveY() { SetMoveAxis(Move_Y, current_position.y * MINUNITMULT);}
void SetMoveZ() { SetMoveAxis(Move_Z, current_position.z * MINUNITMULT);}

#if HAS_HOTEND
  void SetMoveE() {
    #ifdef PREVENT_COLD_EXTRUSION
      if (thermalManager.tooColdToExtrude(0)) {
        Popup_Window_ETempTooLow();
        return;
      }
    #endif
    SetMoveAxis(Extruder, current_position.e * MINUNITMULT);
  }
#endif

void Goto_HomeOffsets() {
  checkkey = HomeOff;
  select_item.reset();
  HMI_ValueStruct.Home_OffX_scaled = home_offset[X_AXIS] * 10;
  HMI_ValueStruct.Home_OffY_scaled = home_offset[Y_AXIS] * 10;
  HMI_ValueStruct.Home_OffZ_scaled = home_offset[Z_AXIS] * 10;
  Draw_HomeOff_Menu();
}

void Goto_ProbeSettings() {
  checkkey = ProbeOff;
  select_item.reset();
  HMI_ValueStruct.Probe_OffX_scaled = probe.offset.x * 10;
  HMI_ValueStruct.Probe_OffY_scaled = probe.offset.y * 10;
  Draw_ProbeOff_Menu();
}

void SetPID(celsius_t t, heater_id_t h) {
  char cmd[48] = "";
  char str_1[5] = "", str_2[5] = "";
  sprintf_P(cmd, PSTR("G28O XY\nG0 Z5 F300\nG0 X%s Y%s F5000"), 
    dtostrf(X_CENTER, 1, 1, str_1),
    dtostrf(Y_CENTER, 1, 1, str_2));
  gcode.process_subcommands_now_P(cmd);
  planner.synchronize();
  thermalManager.PID_autotune(t, H_E0, 10, true);
}
void HotendPID() { SetPID(ui.material_preset[0].hotend_temp, H_E0); }
void BedPID() { SetPID(ui.material_preset[0].bed_temp, H_BED); }

void SetPwrLossr() {
  recovery.enable(!recovery.enabled);
  Draw_Chkb_Line(CurrentMenu->line(), recovery.enabled);
}

void SetBrightness() {
  HMI_ValueStruct.Value = HMI_data.Brightness;
  EncoderRate.enabled = true;
  last_checkkey = checkkey;
  checkkey = Brightness;
  Draw_Menu_IntValue(HMI_data.Selected_Color, CurrentMenu->line(), 3, HMI_ValueStruct.Value);
}

void Goto_SelectColors() {
  checkkey = SelColor;
  select_item.reset();
  Draw_SelColor_Menu();
}

#if ENABLED(SOUND_MENU_ITEM)
  void SetEnableSound() {
    ui.buzzer_enabled = !ui.buzzer_enabled;
    Draw_Chkb_Line(CurrentMenu->line(), ui.buzzer_enabled);
  }
#endif

void Goto_LockScreen() {
  DWIN_LockScreen(true);
}

// Menuitem Drawing functions =================================================

void onDrawMoveX(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 58, 118, 106, 132);
  onDrawFloatMenu(menuitem, line, UNITFDIGITS, current_position.x);
}

void onDrawMoveY(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 109, 118, 157, 132);
  onDrawFloatMenu(menuitem, line, UNITFDIGITS, current_position.y);
}

void onDrawMoveZ(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 160, 118, 209, 132);
  onDrawFloatMenu(menuitem, line, UNITFDIGITS, current_position.z);
}

#if HAS_HOTEND
  void onDrawMoveE(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 212, 118, 253, 131);
    onDrawFloatMenu(menuitem, line, UNITFDIGITS, current_position.e);
  }
#endif

void onDrawMoveSubMenu(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 159, 70, 200, 84);
  onDrawSubMenu(menuitem, line);
}

void onDrawDisableMotors(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 204, 70, 259, 82);
  onDrawMenuItem(menuitem, line);
}

void onDrawAutoHome(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 0, 89, 41, 101);
  onDrawMenuItem(menuitem, line);
}

#if HAS_ZOFFSET_ITEM
  #if HAS_BED_PROBE
    void onDrawZOffset(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) menuitem->SetFrame(1, 174, 164, 223, 177);
      onDrawFloatMenu(menuitem, line, 2, probe.offset.z);
    }
  #else
    void onDrawHomeOffset(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) menuitem->SetFrame(1, 43, 89, 98, 101);
      onDrawMenuItem(menuitem, line);
    }
  #endif
#endif

#if HAS_HOTEND
  void onDrawPreheat1(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 100, 89, 151, 101);
    onDrawMenuItem(menuitem, line);
  }
  void onDrawPreheat2(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 180, 89, 233, 100);
    onDrawMenuItem(menuitem, line);
  }
#endif

#if HAS_PREHEAT
  void onDrawCooldown(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 1, 104,  56, 117);
    onDrawMenuItem(menuitem, line);
  }
#endif

void onDrawLanguage(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 239, 134, 266, 146);
  onDrawMenuItem(menuitem, line);
  DWINUI::Draw_String(VALX, MBASE(line), HMI_IsChinese() ? F("CN") : F("EN"));
}

void onDrawPwrLossr(MenuItemClass* menuitem, int8_t line) {
  onDrawMenuItem(menuitem, line);
  Draw_Chkb_Line(line, recovery.enabled);
}

void onDrawBrightness(MenuItemClass* menuitem, int8_t line) {
  onDrawIntegerMenu(menuitem, line, HMI_data.Brightness);
}

void onDrawEnableSound(MenuItemClass* menuitem, int8_t line) {
  onDrawMenuItem(menuitem, line);
  Draw_Chkb_Line(line, ui.buzzer_enabled);
}

// HMI Control functions ======================================================

void HMI_Brightness() {
  int8_t val = HMI_GetInt(10, 250);
  if (!val) return;
  else if (val == 1)  // Live
    DWIN_LCD_Brightness(HMI_ValueStruct.Value);
  else {  // Apply
    HMI_data.Brightness = HMI_ValueStruct.Value;
    checkkey = last_checkkey;
  }
}

void HMI_Move_Axis(const int16_t lo, const int16_t hi, const AxisEnum axis, float &pos) {
  int8_t val = HMI_GetFloat(UNITFDIGITS, lo, hi);
  if (!val) return;
  else if (val == 1) {  // Live
    pos = HMI_ValueStruct.Value / MINUNITMULT;
    if (!planner.is_full()) {
      planner.synchronize();
      planner.buffer_line(current_position, homing_feedrate(axis));
    }
  }
  else {  // Apply
    planner.synchronize();
    checkkey = last_checkkey;
  }
}

void HMI_Move_X() { HMI_Move_Axis((X_MIN_POS) * MINUNITMULT, (X_MAX_POS) * MINUNITMULT, X_AXIS, current_position.x);}
void HMI_Move_Y() { HMI_Move_Axis((Y_MIN_POS) * MINUNITMULT, (Y_MAX_POS) * MINUNITMULT, Y_AXIS, current_position.y);}
void HMI_Move_Z() { HMI_Move_Axis((Z_MIN_POS) * MINUNITMULT, (Z_MAX_POS) * MINUNITMULT, Z_AXIS, current_position.z);}

#if HAS_HOTEND
  void HMI_Move_E() {
    static float last_E_scaled = 0;
    int8_t val = HMI_GetFloat(UNITFDIGITS, last_E_scaled - (EXTRUDE_MAXLENGTH) * MINUNITMULT, last_E_scaled + (EXTRUDE_MAXLENGTH) * MINUNITMULT);
    if (!val) return;
    else if (val == 1) {  // Live
      current_position.e = HMI_ValueStruct.Value / MINUNITMULT;
      if (!planner.is_full()) {
        planner.synchronize();
        planner.buffer_line(current_position, MMM_TO_MMS(FEEDRATE_E));
      }
    }
    else {  // Apply
      last_E_scaled = HMI_ValueStruct.Value;
      planner.synchronize();
      checkkey = last_checkkey;
    }
  }
#endif

#if HAS_ZOFFSET_ITEM
  bool printer_busy() { return planner.movesplanned() || printingIsActive(); }

  void HMI_Zoffset() {
    int8_t val = HMI_GetFloat(2, (Z_PROBE_OFFSET_RANGE_MIN) * 100, (Z_PROBE_OFFSET_RANGE_MAX) * 100);
    if (!val) return;
    else if (val == 1) {  // Live
      last_zoffset = dwin_zoffset;
      dwin_zoffset = HMI_ValueStruct.Value / 100.0f;
      #if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
        if (BABYSTEP_ALLOWED()) babystep.add_mm(Z_AXIS, dwin_zoffset - last_zoffset);
      #endif
    }
    else {  // Apply
      #if HAS_BED_PROBE
        probe.offset.z = dwin_zoffset;
        TERN_(EEPROM_SETTINGS, settings.save());
      #endif
      checkkey = last_checkkey;
    }
  }
#endif // HAS_ZOFFSET_ITEM

// Menu Creation and Drawing functions ======================================================

MenuClass *MoveMenu = nullptr;
void Draw_Move_Menu() {
  checkkey = Menu;
  if (MoveMenu == nullptr) MoveMenu = new MenuClass();
  if (CurrentMenu != MoveMenu) {
    DWINUI::MenuItemsPrepare(5);
    ADDMENUITEM(ICON_Back, GET_TEXT(MSG_BUTTON_BACK), onDrawBack, Draw_Prepare_Menu);
    ADDMENUITEM(ICON_MoveX, GET_TEXT(MSG_MOVE_X), onDrawMoveX, SetMoveX);
    ADDMENUITEM(ICON_MoveY, GET_TEXT(MSG_MOVE_Y), onDrawMoveY, SetMoveY);
    ADDMENUITEM(ICON_MoveZ, GET_TEXT(MSG_MOVE_Z), onDrawMoveZ, SetMoveZ);
    #if HAS_HOTEND
      ADDMENUITEM(ICON_Extruder, GET_TEXT(MSG_MOVE_E), onDrawMoveE, SetMoveE);
    #endif
  }
  if (HMI_IsChinese()) Title.FrameCopy(1, 192, 1, 233, 14);
  else Title.ShowCaption(GET_TEXT_F(MSG_MOVE_AXIS));
  CurrentMenu = MoveMenu;
  MoveMenu->Draw();
  if (!all_axes_trusted()) DWIN_DrawStatusLine(HMI_data.AlertTxt_Color, HMI_data.AlertBg_Color, "WARNING: position is unknow");
  else DWIN_StatusChanged(nullptr);
}

MenuClass *PrepareMenu = nullptr;
void Draw_Prepare_Menu() {
  checkkey = Menu;
  if (PrepareMenu == nullptr) PrepareMenu = new MenuClass();
  if (CurrentMenu != PrepareMenu) {
    DWINUI::MenuItemsPrepare(13);
    ADDMENUITEM(ICON_Back, GET_TEXT(MSG_BUTTON_BACK), onDrawBack, Goto_Main_Menu);
    ADDMENUITEM(ICON_FilMan, GET_TEXT(MSG_FILAMENT_MAN), onDrawSubMenu, Goto_FilamentMan);
    ADDMENUITEM(ICON_Axis, GET_TEXT(MSG_MOVE_AXIS), onDrawMoveSubMenu, Draw_Move_Menu);
    ADDMENUITEM(ICON_Tramming, GET_TEXT(MSG_MANUAL_LEVELING), onDrawSubMenu, Goto_ManualLev);
    ADDMENUITEM(ICON_CloseMotor, GET_TEXT(MSG_DISABLE_STEPPERS), onDrawDisableMotors, DisableMotors);
    ADDMENUITEM(ICON_Homing, GET_TEXT(MSG_AUTO_HOME), onDrawAutoHome, AutoHome);
    #if ENABLED(MESH_BED_LEVELING)
      ADDMENUITEM(ICON_ManualMesh, GET_TEXT(MSG_MANUAL_MESH), onDrawSubMenu, Goto_ManualMesh);
    #endif
    #if HAS_ZOFFSET_ITEM
      #if HAS_BED_PROBE
        ADDMENUITEM(ICON_SetHome, GET_TEXT(MSG_ZPROBE_ZOFFSET), onDrawZOffset, SetZOffset);
      #else
        ADDMENUITEM(ICON_SetHome, GET_TEXT(MSG_SET_HOME_OFFSETS), onDrawHomeOffset, SetHomeOffset);
      #endif
    #endif
    #if HAS_HOTEND
      ADDMENUITEM(ICON_PLAPreheat, GET_TEXT(MSG_PREHEAT_1), onDrawPreheat1, SetPreheat0);
      ADDMENUITEM(ICON_ABSPreheat, PSTR("Preheat " PREHEAT_2_LABEL), onDrawPreheat2, SetPreheat1);
      ADDMENUITEM(ICON_PLAPreheat, GET_TEXT(MSG_PREHEAT_CUSTOM), onDrawMenuItem, SetPreheat2);
    #endif
    #if HAS_PREHEAT
      ADDMENUITEM(ICON_Cool, GET_TEXT(MSG_COOLDOWN), onDrawCooldown, SetCoolDown);
    #endif
    ADDMENUITEM(ICON_Language, PSTR("UI Language"), onDrawLanguage, SetLanguage);
  }
  if (HMI_IsChinese()) Title.FrameCopy(1, 133, 1, 160, 13);
  else Title.ShowCaption(GET_TEXT_F(MSG_PREPARE));
  CurrentMenu = PrepareMenu;
  PrepareMenu->Draw();
  DWIN_StatusChanged(nullptr);
}

MenuClass *AdvancedSettings = nullptr;
void Draw_AdvSet_Menu() {
  checkkey = Menu;
  if (AdvancedSettings == nullptr) AdvancedSettings = new MenuClass();
  if (CurrentMenu != AdvancedSettings) {
    DWINUI::MenuItemsPrepare(10);
    ADDMENUITEM(ICON_Back, GET_TEXT(MSG_BUTTON_BACK), onDrawBack, Draw_Control_Menu);
    ADDMENUITEM(ICON_HomeOff, GET_TEXT(MSG_SET_HOME_OFFSETS), onDrawSubMenu, Goto_HomeOffsets);
    #if HAS_BED_PROBE
      ADDMENUITEM(ICON_ProbeOff, GET_TEXT(MSG_ZPROBE_SETTINGS), onDrawSubMenu, Goto_ProbeSettings);
    #endif
    ADDMENUITEM(ICON_PIDNozzle, PSTR("Hotend PID"), onDrawMenuItem, HotendPID);
    ADDMENUITEM(ICON_PIDbed, PSTR("Bed PID"), onDrawMenuItem, BedPID);
    #if ENABLED(POWER_LOSS_RECOVERY)
      ADDMENUITEM(ICON_Pwrlossr, PSTR("Power-loss recovery"), onDrawPwrLossr, SetPwrLossr);
    #endif
    ADDMENUITEM(ICON_Brightness, PSTR("Display Brightness"), onDrawBrightness, SetBrightness);
    ADDMENUITEM(ICON_Scolor, PSTR("Select Colors"), onDrawSubMenu, Goto_SelectColors);
    #if ENABLED(SOUND_MENU_ITEM)
      ADDMENUITEM(ICON_Sound, PSTR("Enable Sound"), onDrawEnableSound, SetEnableSound);
    #endif
    ADDMENUITEM(ICON_Lock, PSTR("Lock Screen"), onDrawMenuItem, Goto_LockScreen);
  }
  Title.ShowCaption(GET_TEXT_F(MSG_ADVANCED_SETTINGS));
  CurrentMenu = AdvancedSettings;
  AdvancedSettings->Draw();
  DWIN_StatusChanged(nullptr);
}


#endif // DWIN_CREALITY_LCD
