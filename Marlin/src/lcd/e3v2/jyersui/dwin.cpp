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

/**
 * lcd/e3v2/jyersui/dwin.cpp
 * JYERSUI Author: Jacob Myers
 *
 * JYERSUI Enhanced by LCH-77
 * Version: 1.9
 * Date: Jun 16, 2022
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_LCD_JYERSUI)

#include "dwin_defines.h"
#include "dwin.h"
#include "dwinui.h"

#include "../../marlinui.h"
#include "../../../MarlinCore.h"
#include "../../../gcode/gcode.h"
#include "../../../module/temperature.h"
#include "../../../module/planner.h"
#include "../../../module/settings.h"
#include "../../../libs/buzzer.h"
#include "../../../inc/Conditionals_post.h"
#include "../common/encoder.h"

//#define DEBUG_OUT 1
#include "../../../core/debug_out.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #include "../../../feature/runout.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../../feature/host_actions.h"
#endif

#if ANY(BABYSTEPPING, HAS_BED_PROBE, HAS_WORKSPACE_OFFSET)
  #define HAS_ZOFFSET_ITEM 1
#endif

#ifndef strcasecmp_P
  #define strcasecmp_P(a, b) strcasecmp((a), (b))
#endif

#if HAS_LEVELING
  #include "../../../feature/bedlevel/bedlevel.h"
#endif

#ifdef BLTOUCH_HS_MODE
  #include "../../../feature/bltouch.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../../libs/least_squares_fit.h"
  #include "../../../libs/vector_3.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#if HAS_ESDIAG
  #include "endstop_diag.h"
#endif

#if HAS_LOCKSCREEN
  #include "lockscreen.h"
#endif

#if ENABLED(CASE_LIGHT_MENU)
  #include "../../../feature/caselight.h"
#endif

#if ENABLED(LED_CONTROL_MENU)
  #include "../../../feature/leds/leds.h"
#endif

#if HAS_PIDPLOT
  #include "plot.h"
#endif
#if HAS_GCODE_PREVIEW
  #include "gcode_preview.h"
#endif

#define MACHINE_SIZE STRINGIFY(X_BED_SIZE) "x" STRINGIFY(Y_BED_SIZE) "x" STRINGIFY(Z_MAX_POS)

#define MENU_CHAR_LIMIT 24
#define STATUS_CHAR_LIMIT 30

#define MAX_PRINT_SPEED 500
#define MIN_PRINT_SPEED 10

#if HAS_FAN
  #define MAX_FAN_SPEED 255
  #define MIN_FAN_SPEED 0
#endif

#define MAX_XY_OFFSET 100

#if HAS_ZOFFSET_ITEM
  #define MAX_Z_OFFSET 9.99
  #if HAS_BED_PROBE
    #define MIN_Z_OFFSET -9.99
  #else
    #define MIN_Z_OFFSET -1
  #endif
#endif

#if HAS_HOTEND
  #define MAX_FLOW_RATE 200
  #define MIN_FLOW_RATE 10

  #define MAX_E_TEMP (HEATER_0_MAXTEMP - HOTEND_OVERSHOOT)
  #define MIN_E_TEMP 0
#endif

#if HAS_HEATED_BED
  #define MAX_BED_TEMP BED_MAXTEMP
  #define MIN_BED_TEMP 0
#endif

#if HAS_JUNCTION_DEVIATION
  #define MIN_JD_MM 0.01
  #define MAX_JD_MM 0.3
#endif

/**
 * Custom menu items with jyersLCD
 */
#if ENABLED(CUSTOM_MENU_CONFIG)
  #ifdef CONFIG_MENU_ITEM_5_DESC
    #define CUSTOM_MENU_COUNT 5
  #elif defined(CONFIG_MENU_ITEM_4_DESC)
    #define CUSTOM_MENU_COUNT 4
  #elif defined(CONFIG_MENU_ITEM_3_DESC)
    #define CUSTOM_MENU_COUNT 3
  #elif defined(CONFIG_MENU_ITEM_2_DESC)
    #define CUSTOM_MENU_COUNT 2
  #elif defined(CONFIG_MENU_ITEM_1_DESC)
    #define CUSTOM_MENU_COUNT 1
  #endif
  #if CUSTOM_MENU_COUNT
    #define HAS_CUSTOM_MENU 1
  #endif
#endif

constexpr float default_max_feedrate[]     = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[] = DEFAULT_MAX_ACCELERATION;
constexpr float default_steps[]            = DEFAULT_AXIS_STEPS_PER_UNIT;
#if HAS_CLASSIC_JERK
  constexpr float default_max_jerk[]       = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
#endif

enum SelectItem : uint8_t {
  PAGE_PRINT = 0,
  PAGE_PREPARE,
  PAGE_CONTROL,
  PAGE_INFO_LEVELING,
  PAGE_COUNT,

  PRINT_SETUP = 0,
  PRINT_PAUSE_RESUME,
  PRINT_STOP,
  PRINT_COUNT
};

eeprom_settings_t eeprom_settings = {0};
temp_val_t temp_val = {0};
uint8_t active_menu = MainMenu, last_menu = MainMenu;
uint8_t selection = 0, last_selection = 0, last_pos_selection = 0;
uint8_t scrollpos = 0;
uint8_t process = Main, last_process = Main;
PopupID popup, last_popup;

void (*funcpointer)() = nullptr;
void *valuepointer = nullptr;
float tempvalue;
float valuemin;
float valuemax;
uint8_t valueunit;
uint8_t valuetype;

char cmd[MAX_CMD_SIZE + 16], str_1[16], str_2[16], str_3[16];
char statusmsg[64];
char filename[LONG_FILENAME_LENGTH];

#if HAS_HOSTACTION_MENUS
  #define KEY_WIDTH 26
  #define KEY_HEIGHT 30
  #define KEY_INSET 5
  #define KEY_PADDING 3
  #define KEY_Y_START DWIN_HEIGHT - (4 * (KEY_HEIGHT) + 2 * (KEY_INSET + 1))

  bool keyboard_restrict, reset_keyboard, numeric_keyboard = false;
  uint8_t maxstringlen;
  char *stringpointer = nullptr;
  char action1[9], action2[9], action3[9];
#endif

CrealityDWINClass CrealityDWIN;

#if HAS_MESH

  struct Mesh_Settings {
    bool viewer_asymmetric_range = false;
    bool viewer_print_value = false;
    bool goto_mesh_value = false;
    bool drawing_mesh = false;
    uint8_t mesh_x = 0;
    uint8_t mesh_y = 0;

    #if ENABLED(AUTO_BED_LEVELING_UBL)

      uint8_t tilt_grid = 1;

      void manual_value_update(bool undefined=false) {
        sprintf_P(cmd, PSTR("M421 I%i J%i Z%s %s"), mesh_x, mesh_y, dtostrf(current_position.z, 1, 3, str_1), undefined ? "N" : "");
        gcode.process_subcommands_now(cmd);
        planner.synchronize();
      }

      bool create_plane_from_mesh() {
        struct linear_fit_data lsf_results;
        incremental_LSF_reset(&lsf_results);
        GRID_LOOP(x, y) {
          if (!isnan(bedlevel.z_values[x][y])) {
            xy_pos_t rpos = { bedlevel.get_mesh_x(x), bedlevel.get_mesh_y(y) };
            incremental_LSF(&lsf_results, rpos, bedlevel.z_values[x][y]);
          }
        }

        if (finish_incremental_LSF(&lsf_results)) {
          SERIAL_ECHOPGM("Could not complete LSF!");
          return true;
        }

        bedlevel.set_all_mesh_points_to_value(0);

        matrix_3x3 rotation = matrix_3x3::create_look_at(vector_3(lsf_results.A, lsf_results.B, 1));
        GRID_LOOP(i, j) {
          float mx = bedlevel.get_mesh_x(i),
                my = bedlevel.get_mesh_y(j),
                mz = bedlevel.z_values[i][j];

          if (DEBUGGING(LEVELING)) {
            DEBUG_ECHOPAIR_F("before rotation = [", mx, 7);
            DEBUG_CHAR(',');
            DEBUG_ECHO_F(my, 7);
            DEBUG_CHAR(',');
            DEBUG_ECHO_F(mz, 7);
            DEBUG_ECHOPGM("]   ---> ");
            DEBUG_DELAY(20);
          }

          rotation.apply_rotation_xyz(mx, my, mz);

          if (DEBUGGING(LEVELING)) {
            DEBUG_ECHOPAIR_F("after rotation = [", mx, 7);
            DEBUG_CHAR(',');
            DEBUG_ECHO_F(my, 7);
            DEBUG_CHAR(',');
            DEBUG_ECHO_F(mz, 7);
            DEBUG_ECHOLNPGM("]");
            DEBUG_DELAY(20);
          }

          bedlevel.z_values[i][j] = mz - lsf_results.D;
        }
        return false;
      }

    #else

      void manual_value_update() {
        sprintf_P(cmd, PSTR("G29 I%i J%i Z%s"), mesh_x, mesh_y, dtostrf(current_position.z, 1, 3, str_1));
        gcode.process_subcommands_now(cmd);
        planner.synchronize();
      }

    #endif

    void manual_mesh_move(const bool zmove=false) {
      if (zmove) {
        planner.synchronize();
        current_position.z = goto_mesh_value ? bedlevel.z_values[mesh_x][mesh_y] : Z_CLEARANCE_BETWEEN_PROBES;
        planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
        planner.synchronize();
      }
      else {
        CrealityDWIN.Popup_Handler(MoveWait);
        sprintf_P(cmd, PSTR("G0 F300 Z%s"), dtostrf(Z_CLEARANCE_BETWEEN_PROBES, 1, 3, str_1));
        gcode.process_subcommands_now(cmd);
        sprintf_P(cmd, PSTR("G42 F4000 I%i J%i"), mesh_x, mesh_y);
        gcode.process_subcommands_now(cmd);
        planner.synchronize();
        current_position.z = goto_mesh_value ? bedlevel.z_values[mesh_x][mesh_y] : Z_CLEARANCE_BETWEEN_PROBES;
        planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
        planner.synchronize();
        CrealityDWIN.Redraw_Menu();
      }
    }

    float get_max_value() {
      float max = __FLT_MIN__;
      GRID_LOOP(x, y) {
        if (!isnan(bedlevel.z_values[x][y]) && bedlevel.z_values[x][y] > max)
          max = bedlevel.z_values[x][y];
      }
      return max;
    }

    float get_min_value() {
      float min = __FLT_MAX__;
      GRID_LOOP(x, y) {
        if (!isnan(bedlevel.z_values[x][y]) && bedlevel.z_values[x][y] < min)
          min = bedlevel.z_values[x][y];
      }
      return min;
    }

    void Draw_Bed_Mesh(int16_t selected = -1, uint8_t gridline_width = 1, uint16_t padding_x = 8, uint16_t padding_y_top = 40 + 53 - 7) {
      drawing_mesh = true;
      const uint16_t total_width_px = DWIN_WIDTH - padding_x - padding_x,
                     cell_width_px  = total_width_px / (GRID_MAX_POINTS_X),
                     cell_height_px = total_width_px / (GRID_MAX_POINTS_Y);
      const float v_max = abs(get_max_value()), v_min = abs(get_min_value()), range = _MAX(v_min, v_max);

      // Clear background from previous selection and select new square
      DWIN_Draw_Rectangle(1, Def_Background_Color, _MAX(0, padding_x - gridline_width), _MAX(0, padding_y_top - gridline_width), padding_x + total_width_px, padding_y_top + total_width_px);
      if (selected >= 0) {
        const auto selected_y = selected / (GRID_MAX_POINTS_X);
        const auto selected_x = selected - (GRID_MAX_POINTS_X) * selected_y;
        const auto start_y_px = padding_y_top + selected_y * cell_height_px;
        const auto start_x_px = padding_x + selected_x * cell_width_px;
        DWIN_Draw_Rectangle(1, Def_Highlight_Color, _MAX(0, start_x_px - gridline_width), _MAX(0, start_y_px - gridline_width), start_x_px + cell_width_px, start_y_px + cell_height_px);
      }

      // Draw value square grid
      char buf[8];
      GRID_LOOP(x, y) {
        const auto start_x_px = padding_x + x * cell_width_px;
        const auto end_x_px   = start_x_px + cell_width_px - 1 - gridline_width;
        const auto start_y_px = padding_y_top + (GRID_MAX_POINTS_Y - y - 1) * cell_height_px;
        const auto end_y_px   = start_y_px + cell_height_px - 1 - gridline_width;
        DWIN_Draw_Rectangle(1,                                                                                 // RGB565 colors: http://www.barth-dev.de/online/rgb565-color-picker/
          isnan(bedlevel.z_values[x][y]) ? Color_Grey : (                                                           // gray if undefined
            (bedlevel.z_values[x][y] < 0 ?
              (uint16_t)round(0x1F * -bedlevel.z_values[x][y] / (!viewer_asymmetric_range ? range : v_min)) << 11 : // red if mesh point value is negative
              (uint16_t)round(0x3F *  bedlevel.z_values[x][y] / (!viewer_asymmetric_range ? range : v_max)) << 5) | // green if mesh point value is positive
                _MIN(0x1F, (((uint8_t)abs(bedlevel.z_values[x][y]) / 10) * 4))),                                    // + blue stepping for every mm
          start_x_px, start_y_px, end_x_px, end_y_px
        );

        safe_delay(10);
        LCD_SERIAL.flushTX();

        // Draw value text on
        if (viewer_print_value) {
          int8_t offset_x, offset_y = cell_height_px / 2 - 6;
          if (isnan(bedlevel.z_values[x][y])) {  // undefined
            DWINUI::Draw_String(font6x12, Def_Text_Color, Color_Bg_Blue, start_x_px + cell_width_px / 2 - 5, start_y_px + offset_y, F("X"));
          }
          else {                          // has value
            if (GRID_MAX_POINTS_X < 10)
              sprintf_P(buf, PSTR("%s"), dtostrf(abs(bedlevel.z_values[x][y]), 1, 2, str_1));
            else
              sprintf_P(buf, PSTR("%02i"), (uint16_t)(abs(bedlevel.z_values[x][y] - (int16_t)bedlevel.z_values[x][y]) * 100));
            offset_x = cell_width_px / 2 - 3 * (strlen(buf)) - 2;
            if (!(GRID_MAX_POINTS_X < 10))
              DWINUI::Draw_String(font6x12, Def_Text_Color, Color_Bg_Blue, start_x_px - 2 + offset_x, start_y_px + offset_y /*+ square / 2 - 6*/, F("."));
            DWINUI::Draw_String(font6x12, Def_Text_Color, Color_Bg_Blue, start_x_px + 1 + offset_x, start_y_px + offset_y /*+ square / 2 - 6*/, buf);
          }
          safe_delay(10);
          LCD_SERIAL.flushTX();
        }
      }
    }

    void Set_Mesh_Viewer_Status() { // TODO: draw gradient with values as a legend instead
      float v_max = abs(get_max_value()), v_min = abs(get_min_value()), range = _MAX(v_min, v_max);
      if (v_min > 3e+10F) v_min = 0.0000001;
      if (v_max > 3e+10F) v_max = 0.0000001;
      if (range > 3e+10F) range = 0.0000001;
      char msg[46];
      if (viewer_asymmetric_range) {
        dtostrf(-v_min, 1, 3, str_1);
        dtostrf( v_max, 1, 3, str_2);
      }
      else {
        dtostrf(-range, 1, 3, str_1);
        dtostrf( range, 1, 3, str_2);
      }
      sprintf_P(msg, PSTR("Red %s..0..%s Green"), str_1, str_2);
      CrealityDWIN.Update_Status(msg);
      drawing_mesh = false;
    }

  };
  Mesh_Settings mesh_conf;

#endif // HAS_MESH

/* General Display Functions */
constexpr const char * const CrealityDWINClass::color_names[11];
constexpr const char * const CrealityDWINClass::preheat_modes[3];
constexpr const char * const CrealityDWINClass::zoffset_modes[3];
#if ENABLED(PREHEAT_BEFORE_LEVELING)
  constexpr const char * const CrealityDWINClass::preheat_levmodes[4];
#endif

// Clear a part of the screen
//  4=Entire screen
//  3=Title bar and Menu area (default)
//  2=Menu area
//  1=Title bar
void CrealityDWINClass::Clear_Screen(uint8_t e/*=3*/) {
  if (e == 1 || e == 3 || e == 4) DWIN_Draw_Rectangle(1, GetColor(eeprom_settings.menu_top_bg, Def_TitleBg_color, false), 0, 0, DWIN_WIDTH, TITLE_HEIGHT); // Clear Title Bar
  if (e == 2 || e == 3) DWIN_Draw_Rectangle(1, Def_Background_Color, 0, 31, DWIN_WIDTH, STATUS_Y); // Clear Menu Area
  if (e == 4) DWIN_Draw_Rectangle(1, Def_Background_Color, 0, 31, DWIN_WIDTH, DWIN_HEIGHT); // Clear Popup Area
}

void CrealityDWINClass::Draw_Float(float value, uint8_t row, bool selected/*=false*/, uint8_t minunit/*=10*/) {
  const uint8_t digits = (uint8_t)floor(log10(abs(value))) + log10(minunit) + (minunit > 1);
  const uint16_t bColor = (selected) ? Def_Selected_Color : Def_Background_Color;
  const uint16_t xpos = 240 - (digits * 8);
  DWIN_Draw_Rectangle(1, Def_Background_Color, 194, MBASE(row), 234 - (digits * 8), MBASE(row) + 16);
  if (isnan(value))
    DWINUI::Draw_String(Def_Text_Color, bColor, xpos - 8, MBASE(row), F(" NaN"));
  else {
    DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_MENU, Def_Text_Color, bColor, digits - log10(minunit) + 1, log10(minunit), xpos, MBASE(row), (value < 0 ? -value : value));
    DWINUI::Draw_String(Def_Text_Color, bColor, xpos - 8, MBASE(row), value < 0 ? F("-") : F(" "));
  }
}

void CrealityDWINClass::Draw_Option(uint8_t value, const char * const * options, uint8_t row, bool selected/*=false*/, bool color/*=false*/) {
  uint16_t bColor = (selected) ? Def_Selected_Color : Def_Background_Color,
           tColor = (color) ? GetColor(value, Def_Text_Color, false) : Def_Text_Color;
  DWIN_Draw_Rectangle(1, bColor, 202, MBASE(row) + 14, 258, MBASE(row) - 2);
  DWINUI::Draw_String(tColor, bColor, 202, MBASE(row) - 1, options[value]);
}

#if HAS_HOSTACTION_MENUS

  void CrealityDWINClass::Draw_String(char * string, uint8_t row, bool selected/*=false*/, bool below/*=false*/) {
    if (!string) string[0] = '\0';
    const uint8_t offset_x = DWIN_WIDTH - strlen(string) * 8 - 20;
    const uint8_t offset_y = (below) ? MENU_CHR_H * 3 / 5 : 0;
    DWIN_Draw_Rectangle(1, Def_Background_Color, offset_x - 10, MBASE(row) + offset_y - 1, offset_x, MBASE(row) + 16 + offset_y);
    DWINUI::Draw_String(Def_Text_Color, (selected) ? Def_Selected_Color : Def_Background_Color, offset_x, MBASE(row) - 1 + offset_y, string);
  }

  const uint64_t CrealityDWINClass::Encode_String(const char * string) {
    const char table[65] = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
    uint64_t output = 0;
    LOOP_L_N(i, strlen(string)) {
      uint8_t upper_bound = 63, lower_bound = 0;
      uint8_t midpoint;
      LOOP_L_N(x, 6) {
        midpoint = (uint8_t)(0.5 * (upper_bound + lower_bound));
        if (string[i] == table[midpoint]) break;
        if (string[i] > table[midpoint])
          lower_bound = midpoint;
        else
          upper_bound = midpoint;
      }
      output += midpoint * pow(64, i);
    }
    return output;
  }

  void CrealityDWINClass::Decode_String(uint64_t num, char * string) {
    const char table[65] = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
    LOOP_L_N(i, 30) {
      string[i] = table[num % 64];
      num /= 64;
      if (num == 0) {
        string[i + 1] = '\0';
        break;
      }
    }
  }

#endif // HAS_HOSTACTION_MENUS

uint16_t CrealityDWINClass::GetColor(uint8_t color, uint16_t original, bool light/*=false*/) {
  switch (color) {
    case Default: return original;
    case White:   return (light) ? Color_Light_White   : Color_White;
    case Green:   return (light) ? Color_Light_Green   : Color_Green;
    case Cyan:    return (light) ? Color_Light_Cyan    : Color_Cyan;
    case Blue:    return (light) ? Color_Light_Blue    : Color_Blue;
    case Magenta: return (light) ? Color_Light_Magenta : Color_Magenta;
    case Red:     return (light) ? Color_Light_Red     : Color_Red;
    case Orange:  return (light) ? Color_Light_Orange  : Color_Orange;
    case Yellow:  return (light) ? Color_Light_Yellow  : Color_Yellow;
    case Brown:   return (light) ? Color_Light_Brown   : Color_Brown;
    case Black:   return Color_Black;
  }
  return Color_White;
}

void CrealityDWINClass::Draw_Title(const char * ctitle) {
  DWINUI::Draw_CenteredString((uint8_t)DWIN_FONT_HEAD, GetColor(eeprom_settings.menu_top_txt, Def_TitleTxt_color, false), 5, ctitle);
}
void CrealityDWINClass::Draw_Title(FSTR_P const ftitle) {
   DWINUI::Draw_CenteredString((uint8_t)DWIN_FONT_HEAD, GetColor(eeprom_settings.menu_top_txt, Def_TitleTxt_color, false), 5, ftitle);
}

void _Decorate_Menu_Item(uint8_t row, uint8_t icon, bool more) {
  if (icon) DWIN_ICON_Show(ICON, icon, 26, MBASE(row) - 3);   //Draw Menu Icon
  if (more) DWIN_ICON_Show(ICON, ICON_More, 226, MBASE(row) - 3); // Draw More Arrow
  DWIN_Draw_HLine(CrealityDWIN.GetColor(eeprom_settings.menu_split_line, Def_SplitLine_Color, true), 16, MBASE(row) + 33, 240); // Draw Menu Line
}

void CrealityDWINClass::Draw_Menu_Item(uint8_t row, uint8_t icon/*=0*/, const char * label1, const char * label2, bool more/*=false*/, bool centered/*=false*/) {
  const uint8_t label_offset_y = (label1 && label2) ? MENU_CHR_H * 3 / 5 : 0,
                label1_offset_x = !centered ? LBLX : LBLX * 4/5 + _MAX(LBLX * 1/5U, (DWIN_WIDTH - LBLX - (label1 ? strlen(label1) : 0) * MENU_CHR_W) / 2),
                label2_offset_x = !centered ? LBLX : LBLX * 4/5 + _MAX(LBLX * 1/5U, (DWIN_WIDTH - LBLX - (label2 ? strlen(label2) : 0) * MENU_CHR_W) / 2);
  if (label1) DWINUI::Draw_String(label1_offset_x, MBASE(row) - 1 - label_offset_y, label1); // Draw Label
  if (label2) DWINUI::Draw_String(label2_offset_x, MBASE(row) - 1 + label_offset_y, label2); // Draw Label
  _Decorate_Menu_Item(row, icon, more);
}

void CrealityDWINClass::Draw_Menu_Item(uint8_t row, uint8_t icon/*=0*/, FSTR_P const flabel1, FSTR_P const flabel2, bool more/*=false*/, bool centered/*=false*/) {
  const uint8_t label_offset_y = (flabel1 && flabel2) ? MENU_CHR_H * 3 / 5 : 0,
                label1_offset_x = !centered ? LBLX : LBLX * 4/5 + _MAX(LBLX * 1/5U, (DWIN_WIDTH - LBLX - (flabel1 ? strlen_P(FTOP(flabel1)) : 0) * MENU_CHR_W) / 2),
                label2_offset_x = !centered ? LBLX : LBLX * 4/5 + _MAX(LBLX * 1/5U, (DWIN_WIDTH - LBLX - (flabel2 ? strlen_P(FTOP(flabel2)) : 0) * MENU_CHR_W) / 2);
  if (flabel1) DWINUI::Draw_String(label1_offset_x, MBASE(row) - 1 - label_offset_y, flabel1); // Draw Label
  if (flabel2) DWINUI::Draw_String(label2_offset_x, MBASE(row) - 1 + label_offset_y, flabel2); // Draw Label
  _Decorate_Menu_Item(row, icon, more);
}

void CrealityDWINClass::Draw_Checkbox(uint8_t row, bool value) {
  #if ENABLED(DWIN_CREALITY_LCD_CUSTOM_ICONS) // Draw appropriate checkbox icon
    DWIN_ICON_Show(ICON, (value ? ICON_Checkbox_T : ICON_Checkbox_F), 226, MBASE(row) - 3);
  #else                                         // Draw a basic checkbox using rectangles and lines
    DWIN_Draw_Rectangle(1, Def_Background_Color, 226, MBASE(row) - 3, 226 + 20, MBASE(row) - 3 + 20);
    DWIN_Draw_Rectangle(0, Def_Text_Color, 226, MBASE(row) - 3, 226 + 20, MBASE(row) - 3 + 20);
    if (value) {
      DWIN_Draw_Line(Check_Color, 227, MBASE(row) - 3 + 11, 226 + 8, MBASE(row) - 3 + 17);
      DWIN_Draw_Line(Check_Color, 227 + 8, MBASE(row) - 3 + 17, 226 + 19, MBASE(row) - 3 + 1);
      DWIN_Draw_Line(Check_Color, 227, MBASE(row) - 3 + 12, 226 + 8, MBASE(row) - 3 + 18);
      DWIN_Draw_Line(Check_Color, 227 + 8, MBASE(row) - 3 + 18, 226 + 19, MBASE(row) - 3 + 2);
      DWIN_Draw_Line(Check_Color, 227, MBASE(row) - 3 + 13, 226 + 8, MBASE(row) - 3 + 19);
      DWIN_Draw_Line(Check_Color, 227 + 8, MBASE(row) - 3 + 19, 226 + 19, MBASE(row) - 3 + 3);
    }
  #endif
}

void CrealityDWINClass::Draw_Menu(uint8_t menu, uint8_t select/*=0*/, uint8_t scroll/*=0*/) {
  if (active_menu != menu) {
    last_menu = active_menu;
    if (process == Menu) last_selection = selection;
  }
  selection = _MIN(select, Get_Menu_Size(menu));
  scrollpos = scroll;
  if (selection - scrollpos > MROWS)
    scrollpos = selection - MROWS;
  process = Menu;
  active_menu = menu;
  Clear_Screen();
  Draw_Title(Get_Menu_Title(menu));
  LOOP_L_N(i, TROWS) Menu_Item_Handler(menu, i + scrollpos);
  DWIN_Draw_Rectangle(1, GetColor(eeprom_settings.cursor_color, Def_Cursor_color), 0, MBASE(selection - scrollpos) - 18, 14, MBASE(selection - scrollpos) + 33);
}

void CrealityDWINClass::Redraw_Menu(bool lastprocess/*=true*/, bool lastselection/*=false*/, bool lastmenu/*=false*/) {
  switch ((lastprocess) ? last_process : process) {
    case Menu:
      Draw_Menu((lastmenu) ? last_menu : active_menu, (lastselection) ? last_selection : selection, (lastmenu) ? 0 : scrollpos);
      break;
    case Main:  Draw_Main_Menu((lastselection) ? last_selection : selection); break;
    case Print: Draw_Print_Screen(); break;
    case File:  Draw_SD_List(); break;
    default: break;
  }
}

void CrealityDWINClass::Redraw_Screen() {
  if (printingIsActive()) Draw_Print_Screen();
  else Redraw_Menu(false);
  Draw_Status_Area(true);
  Update_Status_Bar(true);
}

/* Primary Menus and Screen Elements */

void CrealityDWINClass::Main_Menu_Icons() {

  if (selection == 0) {
    DWINUI::DRAW_IconWB(ICON, ICON_Print_1, 17, 110);
    DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 17, 110, 126, 209);
  }
  else
    DWINUI::DRAW_IconWB(ICON, ICON_Print_0, 17, 110);

  DWINUI::Draw_String(52, 180, GET_TEXT_F(MSG_BUTTON_PRINT));

  if (selection == 1) {
    DWINUI::DRAW_IconWB(ICON, ICON_Prepare_1, 145, 110);
    DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 145, 110, 254, 209);
  }
  else
    DWINUI::DRAW_IconWB(ICON, ICON_Prepare_0, 145, 110);

  DWINUI::Draw_String(170, 180, GET_TEXT_F(MSG_PREPARE));

  if (selection == 2) {
    DWINUI::DRAW_IconWB(ICON, ICON_Control_1, 17, 226);
    DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 17, 226, 126, 325);
  }
  else
    DWINUI::DRAW_IconWB(ICON, ICON_Control_0, 17, 226);

  DWINUI::Draw_String(43, 297, GET_TEXT_F(MSG_CONTROL));

  #if HAS_ABL_OR_UBL

    if (selection == 3) {
      DWINUI::DRAW_IconWB(ICON, ICON_Leveling_1, 145, 226);
      DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 145, 226, 254, 325);
    }
    else
      DWINUI::DRAW_IconWB(ICON, ICON_Leveling_0, 145, 226);

    DWINUI::Draw_String(179, 297, GET_TEXT_F(MSG_BUTTON_LEVEL));

  #else

    if (selection == 3) {
      DWINUI::DRAW_IconWB(ICON, ICON_Info_1, 145, 226);
      DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 145, 226, 254, 325);
    }
    else
      DWINUI::DRAW_IconWB(ICON, ICON_Info_0, 145, 226);

    DWINUI::Draw_String(181, 297, GET_TEXT_F(MSG_BUTTON_INFO));

  #endif
}

void CrealityDWINClass::Draw_Main_Menu(uint8_t select/*=0*/) {
  process = Main;
  active_menu = MainMenu;
  selection = select;
  Clear_Screen();
  Draw_Title(Get_Menu_Title(MainMenu));
  SERIAL_ECHOPGM("\nDWIN handshake ");
  DWIN_ICON_Show(ICON, ICON_LOGO, 71, 62);
  Main_Menu_Icons();
}

void CrealityDWINClass::Print_Screen_Icons() {
  if (selection == 0) {
    DWINUI::DRAW_IconWB(ICON, ICON_Setup_1, 8, 252);
    DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 8, 252, 87, 351);
  }
  else
    DWINUI::DRAW_IconWB(ICON, ICON_Setup_0, 8, 252);

  DWINUI::Draw_String(30, 322, GET_TEXT_F(MSG_TUNE));

  if (selection == 2) {
    DWINUI::DRAW_IconWB(ICON, ICON_Stop_1, 184, 252);
    DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 184, 252, 263, 351);
  }
  else
    DWINUI::DRAW_IconWB(ICON, ICON_Stop_0, 184, 252);

  DWINUI::Draw_String(205, 322, GET_TEXT_F(MSG_BUTTON_STOP));

  if (temp_val.paused) {
    if (selection == 1) {
      DWINUI::DRAW_IconWB(ICON, ICON_Continue_1, 96, 252);
      DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 96, 252, 175, 351);
    }
    else
      DWINUI::DRAW_IconWB(ICON, ICON_Continue_0, 96, 252);

    DWINUI::Draw_String(114, 322, GET_TEXT_F(MSG_BUTTON_RESUME));
  }
  else {
    if (selection == 1) {
      DWINUI::DRAW_IconWB(ICON, ICON_Pause_1, 96, 252);
      DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 96, 252, 175, 351);
    }
    else
      DWINUI::DRAW_IconWB(ICON, ICON_Pause_0, 96, 252);

    DWINUI::Draw_String(114, 322, GET_TEXT_F(MSG_BUTTON_PAUSE));
  }
}

void CrealityDWINClass::Draw_Print_Screen() {
  process = Print;
  selection = 0;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Def_Background_Color, 8, 352, DWIN_WIDTH - 8, 376);
  Draw_Title(GET_TEXT(MSG_PRINTING));
  Print_Screen_Icons();
  DWIN_ICON_Show(ICON, ICON_PrintTime, 14, 171);
  DWIN_ICON_Show(ICON, ICON_RemainTime, 147, 169);
  DWINUI::Draw_String(Def_PercentTxt_Color, 41, 163, GET_TEXT_F(MSG_INFO_PRINT_TIME));
  DWINUI::Draw_String(Def_PercentTxt_Color, 176, 163, GET_TEXT_F(MSG_REMAINING_TIME));
  Update_Status_Bar(true);
  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  TERN_(USE_M73_REMAINING_TIME, Draw_Print_ProgressRemain());
  Draw_Print_Filename(true);
}

void CrealityDWINClass::Draw_Print_Filename(const bool reset/*=false*/) {
  static uint8_t namescrl = 0;
  if (reset) namescrl = 0;
  if (process == Print) {
    size_t len = strlen(filename);
    int8_t pos = len;
    if (pos > STATUS_CHAR_LIMIT) {
      pos -= namescrl;
      len = _MIN((size_t)pos, (size_t)STATUS_CHAR_LIMIT);
      char dispname[len + 1];
      if (pos >= 0) {
        LOOP_L_N(i, len) dispname[i] = filename[i + namescrl];
      }
      else {
        LOOP_L_N(i, STATUS_CHAR_LIMIT + pos) dispname[i] = ' ';
        LOOP_S_L_N(i, STATUS_CHAR_LIMIT + pos, STATUS_CHAR_LIMIT) dispname[i] = filename[i - (STATUS_CHAR_LIMIT + pos)];
      }
      dispname[len] = '\0';
      DWIN_Draw_Rectangle(1, Def_Background_Color, 8, 50, DWIN_WIDTH - 8, 80);
      const int8_t npos = (DWIN_WIDTH - STATUS_CHAR_LIMIT * MENU_CHR_W) / 2;
      DWINUI::Draw_String(npos, 60, dispname);
      if (-pos >= STATUS_CHAR_LIMIT) namescrl = 0;
      namescrl++;
    }
    else {
      DWIN_Draw_Rectangle(1, Def_Background_Color, 8, 50, DWIN_WIDTH - 8, 80);
      const int8_t npos = (DWIN_WIDTH - strlen(filename) * MENU_CHR_W) / 2;
      DWINUI::Draw_String(npos, 60, filename);
    }
  }
}

void CrealityDWINClass::Draw_Print_ProgressBar() {
  uint8_t printpercent = temp_val.sdprint ? card.percentDone() : (ui._get_progress() / 100);
  DWINUI::DRAW_IconWB(ICON, ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, Def_Barfill_Color, 16 + printpercent * 240 / 100, 93, 256, 113);
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_MENU, GetColor(eeprom_settings.progress_percent, Def_PercentTxt_Color), Def_Background_Color, 3, 109, 133, printpercent);
  DWINUI::Draw_String(GetColor(eeprom_settings.progress_percent, Def_PercentTxt_Color), Def_Background_Color, 134, 133, F("%"));
}

#if ENABLED(USE_M73_REMAINING_TIME)

  void CrealityDWINClass::Draw_Print_ProgressRemain() {
    uint16_t remainingtime = ui.get_remaining_time();
    DWIN_Draw_IntValue(true, true, 1, DWIN_FONT_MENU, GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 2, 176, 187, remainingtime / 3600);
    DWIN_Draw_IntValue(true, true, 1, DWIN_FONT_MENU, GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 2, 201, 187, (remainingtime % 3600) / 60);
    if (eeprom_settings.time_format_textual) {
      DWINUI::Draw_String(GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 193, 187, F("h"));
      DWINUI::Draw_String(GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 217, 187, F("m"));
    }
    else
      DWINUI::Draw_String(GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 193, 187, F(":"));
  }

#endif

void CrealityDWINClass::Draw_Print_ProgressElapsed() {
  duration_t elapsed = print_job_timer.duration();
  DWIN_Draw_IntValue(true, true, 1, DWIN_FONT_MENU, GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 2, 42, 187, elapsed.value / 3600);
  DWIN_Draw_IntValue(true, true, 1, DWIN_FONT_MENU, GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 2, 67, 187, (elapsed.value % 3600) / 60);
  if (eeprom_settings.time_format_textual) {
    DWINUI::Draw_String(GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 59, 187, F("h"));
    DWINUI::Draw_String(GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 83, 187, F("m"));
  }
  else
    DWINUI::Draw_String(GetColor(eeprom_settings.progress_time, Def_PercentTxt_Color), Def_Background_Color, 59, 187, F(":"));
}

void CrealityDWINClass::Draw_PrintDone_confirm() {
  process = Confirm;
  popup = Complete;
  if (TERN0(HAS_GCODE_PREVIEW, Preview_Valid())) {
    Clear_Screen();
    Draw_Title(GET_TEXT(MSG_PRINT_DONE));
    DWIN_ICON_Show(0, 0, 1, 21, 100, 0x00);
    DWINUI::Draw_Button(BTN_Continue, 87, 300);
  }
  else {
    Draw_Print_Screen();
    DWIN_Draw_Rectangle(1, Def_Background_Color, 8, 252, 263, 351);
    DWINUI::Draw_Button(BTN_Continue, 87, 283);
    DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 86, 282, 187, 321);
    DWIN_Draw_Rectangle(0, GetColor(eeprom_settings.highlight_box, Def_Highlight_Color), 85, 281, 188, 322);
  }
}

void CrealityDWINClass::Draw_SD_Item(uint8_t item, uint8_t row) {
  if (item == 0)
    Draw_Menu_Item(0, ICON_Back, card.flag.workDirIsRoot ? GET_TEXT_F(MSG_BACK) : F(".."));
  else {
    card.getfilename_sorted(SD_ORDER(item - 1, card.get_num_Files()));
    char * const filename = card.longest_filename();
    size_t max = MENU_CHAR_LIMIT;
    size_t pos = strlen(filename), len = pos;
    if (!card.flag.filenameIsDir)
      while (pos && filename[pos] != '.') pos--;
    len = pos;
    NOMORE(len, max);
    char name[len + 1];
    memcpy(name, filename, len);
    if (pos > max) LOOP_S_L_N(i, len - 3, len) name[i] = '.';
    name[len] = '\0';
    Draw_Menu_Item(row, card.flag.filenameIsDir ? ICON_More : ICON_File, name);
  }
}

void CrealityDWINClass::Draw_SD_List(bool removed/*=false*/) {
  Clear_Screen();
  Draw_Title("Select File");
  selection = 0;
  scrollpos = 0;
  process = File;
  if (card.isMounted() && !removed) {
    LOOP_L_N(i, _MIN(card.get_num_Files() + 1, TROWS))
      Draw_SD_Item(i, i);
  }
  else {
    Draw_Menu_Item(0, ICON_Back, GET_TEXT_F(MSG_BACK));
    DWIN_Draw_Rectangle(1, Def_AlertBg_Color, 10, MBASE(3) - 10, DWIN_WIDTH - 10, MBASE(4));
    DWINUI::Draw_String(font16x32, Def_AlertTxt_Color, Def_AlertBg_Color, ((DWIN_WIDTH) - 8 * 16) / 2, MBASE(3), GET_TEXT_F(MSG_NO_MEDIA));
  }
  DWIN_Draw_Rectangle(1, GetColor(eeprom_settings.cursor_color, Def_Cursor_color), 0, MBASE(0) - 18, 14, MBASE(0) + 33);
}

void CrealityDWINClass::Draw_Status_Area(bool icons/*=false*/) {

  if (icons) DWIN_Draw_Rectangle(1, Def_Background_Color, 0, STATUS_Y, DWIN_WIDTH, DWIN_HEIGHT - 1);

  #if HAS_HOTEND
    static float hotend = -1;
    static int16_t hotendtarget = -1, flow = -1;
    if (icons) {
      hotend = -1;
      hotendtarget = -1;
      DWIN_ICON_Show(ICON, ICON_HotendTemp, 10, 383);
      DWINUI::Draw_String(DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    }
    if (thermalManager.temp_hotend[0].celsius != hotend) {
      hotend = thermalManager.temp_hotend[0].celsius;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 3, 28, 384, thermalManager.temp_hotend[0].celsius);
      DWIN_Draw_DegreeSymbol(GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), 25 + 3 * STAT_CHR_W + 5, 386);
    }
    if (thermalManager.temp_hotend[0].target != hotendtarget) {
      hotendtarget = thermalManager.temp_hotend[0].target;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.temp_hotend[0].target);
      DWIN_Draw_DegreeSymbol(GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), 25 + 4 * STAT_CHR_W + 39, 386);
    }
    if (icons) {
      flow = -1;
      DWIN_ICON_Show(ICON, ICON_StepE, 112, 417);
      DWINUI::Draw_String(DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
    }
    if (planner.flow_percentage[0] != flow) {
      flow = planner.flow_percentage[0];
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    }
  #endif

  #if HAS_HEATED_BED
    static float bed = -1;
    static int16_t bedtarget = -1;
    if (icons) {
      bed = -1;
      bedtarget = -1;
      DWIN_ICON_Show(ICON, ICON_BedTemp, 10, 416);
      DWINUI::Draw_String(DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    }
    if (thermalManager.temp_bed.celsius != bed) {
      bed = thermalManager.temp_bed.celsius;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 3, 28, 417, thermalManager.temp_bed.celsius);
      DWIN_Draw_DegreeSymbol(GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), 25 + 3 * STAT_CHR_W + 5, 419);
    }
    if (thermalManager.temp_bed.target != bedtarget) {
      bedtarget = thermalManager.temp_bed.target;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.temp_bed.target);
      DWIN_Draw_DegreeSymbol(GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), 25 + 4 * STAT_CHR_W + 39, 419);
    }
  #endif

  #if HAS_FAN
    static uint8_t fan = -1;
    if (icons) {
      fan = -1;
      DWIN_ICON_Show(ICON, ICON_FanSpeed, 187, 383);
    }
    if (thermalManager.fan_speed[0] != fan) {
      fan = thermalManager.fan_speed[0];
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
    }
  #endif

  #if HAS_ZOFFSET_ITEM
    static float offset = -1;

    #if HAS_MESH
      static bool _leveling_active = false,
                  _printing_leveling_active = false;
      if (printingIsActive()) {
        _printing_leveling_active = ((planner.leveling_active && planner.leveling_active_at_z(current_position.z)) || _printing_leveling_active );
        if ((_printing_leveling_active = (planner.leveling_active && planner.leveling_active_at_z(current_position.z)) && ui.get_blink()))
          DWIN_Draw_Rectangle(1, Def_SplitLine_Color, 186, 415, 205, 436);
        else
          DWIN_Draw_Rectangle(1, Def_Background_Color, 186, 415, 205, 436);
      }
      else {
        _leveling_active = (planner.leveling_active || _leveling_active);
        if ((_leveling_active = planner.leveling_active && ui.get_blink()))
          DWIN_Draw_Rectangle(1, Def_SplitLine_Color, 186, 415, 205, 436);
        else
          DWIN_Draw_Rectangle(1, Def_Background_Color, 186, 415, 205, 436);
      }
      DWIN_ICON_Show(ICON, ICON_Zoffset, 187, 416);
    #else
      if (icons) DWIN_ICON_Show(ICON, ICON_Zoffset, 187, 416);
    #endif
    if (temp_val.zoffsetvalue != offset || icons) {
      offset = temp_val.zoffsetvalue;
      DWINUI::Draw_Signed_Float(DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color),Def_Background_Color, 1, 2, 202, 417, temp_val.zoffsetvalue);
    }
  #endif

  static int16_t feedrate = -1;
  if (icons) {
    feedrate = -1;
    DWIN_ICON_Show(ICON, ICON_Speed, 113, 383);
    DWINUI::Draw_String(DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 116 + 5 * STAT_CHR_W + 2, 384, F("%"));
  }
  if (feedrate_percentage != feedrate) {
    feedrate = feedrate_percentage;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, GetColor(eeprom_settings.status_area_text, Def_Indicator_Color), Def_Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  }

  static float x = -1, y = -1, z = -1;
  static bool update_x = false, update_y = false, update_z = false;
  update_x = (current_position.x != x || axis_should_home(X_AXIS) || update_x);
  update_y = (current_position.y != y || axis_should_home(Y_AXIS) || update_y);
  update_z = (current_position.z != z || axis_should_home(Z_AXIS) || update_z);
  if (icons) {
    x = y = z = -1;
    DWIN_Draw_Line(GetColor(eeprom_settings.coordinates_split_line, Def_SplitLine_Color, true), 16, 450, 256, 450);
    DWIN_ICON_Show(ICON, ICON_MaxSpeedX,  10, 456);
    DWIN_ICON_Show(ICON, ICON_MaxSpeedY,  95, 456);
    DWIN_ICON_Show(ICON, ICON_MaxSpeedZ, 180, 456);
  }
  if (update_x) {
    x = current_position.x;
    if ((update_x = axis_should_home(X_AXIS) && ui.get_blink()))
      DWINUI::Draw_String(GetColor(eeprom_settings.coordinates_text, Def_Coordinate_Color), Def_Background_Color, 39, 459, F("  -?-  "));
    else
      DWINUI::Draw_Signed_Float(DWIN_FONT_MENU, GetColor(eeprom_settings.coordinates_text, Def_Coordinate_Color), Def_Background_Color, 3, 1, 31, 459, current_position.x);
  }
  if (update_y) {
    y = current_position.y;
    if ((update_y = axis_should_home(Y_AXIS) && ui.get_blink()))
      DWINUI::Draw_String(GetColor(eeprom_settings.coordinates_text, Def_Coordinate_Color), Def_Background_Color, 124, 459, F("  -?-  "));
    else
      DWINUI::Draw_Signed_Float(DWIN_FONT_MENU, GetColor(eeprom_settings.coordinates_text, Def_Coordinate_Color), Def_Background_Color, 3, 1, 116, 459, current_position.y);
  }
  if (update_z) {
    z = current_position.z;
    if ((update_z = axis_should_home(Z_AXIS) && ui.get_blink()))
      DWINUI::Draw_String(GetColor(eeprom_settings.coordinates_text, Def_Coordinate_Color), Def_Background_Color, 205, 459, F("  -?-  "));
    else
      DWINUI::Draw_Signed_Float(DWIN_FONT_MENU, GetColor(eeprom_settings.coordinates_text, Def_Coordinate_Color), Def_Background_Color, 3, 2, 197, 459, current_position.z);
  }
  DWIN_UpdateLCD();
}

void CrealityDWINClass::Draw_Popup(FSTR_P const line1, FSTR_P const line2, FSTR_P const line3, uint8_t mode, uint8_t icon/*=0*/) {
  if (process != Confirm && process != Popup && process != Wait && process != Cancel) last_process = process;
  if ((process == Menu || process == Wait) && mode == Popup) last_selection = selection;
  process = mode;
  if (popup != PrintConfirm) {
    Clear_Screen();
    DWIN_Draw_Rectangle(0, Def_Highlight_Color, 13, 59, 259, 346);
    DWIN_Draw_Rectangle(1, Def_PopupBg_color, 14, 60, 258, 345);
  }
  else DWIN_Draw_Rectangle(1, Def_Background_Color, 0, 0, DWIN_WIDTH, STATUS_Y - 1);
  const uint8_t ypos = (mode == Popup || mode == Confirm) ? 150 : (mode == Cancel) ? 200 : 230;
  if (icon > 0) DWIN_ICON_Show(ICON, icon, 101, 105);
  if (line1) DWINUI::Draw_String(Def_PopupTxt_Color, (272 - 8 * strlen_P(FTOP(line1))) / 2, ypos, line1);
  if (line2) DWINUI::Draw_String(Def_PopupTxt_Color, (272 - 8 * strlen_P(FTOP(line2))) / 2, ypos + 30, line2);
  if (line3) DWINUI::Draw_String(Def_PopupTxt_Color, (272 - 8 * strlen_P(FTOP(line3))) / 2, ypos + 60, line3);
  if (mode == Popup) {
    selection = 0;
    DWINUI::Draw_Button(BTN_Confirm, 26, 280);
    DWINUI::Draw_Button(BTN_Cancel, 146, 280);
    Popup_Select();
  }
  else if (mode == Confirm) DWINUI::Draw_Button(BTN_Continue, 87, 280);
  else if (mode == Cancel) DWINUI::Draw_Button(BTN_Cancel, 87, 280);
}

void MarlinUI::kill_screen(FSTR_P const error, FSTR_P const) {
  CrealityDWIN.Draw_Popup(GET_TEXT_F(MSG_KILLED), error, GET_TEXT_F(MSG_SWITCH_PS_OFF), Wait, ICON_BLTouch);
}

void CrealityDWINClass::Popup_Select() {
  const uint16_t c1 = (selection == 0) ? GetColor(eeprom_settings.highlight_box, Def_Highlight_Color) : Def_Background_Color,
                 c2 = (selection == 0) ? Def_Background_Color : GetColor(eeprom_settings.highlight_box, Def_Highlight_Color);
  DWIN_Draw_Rectangle(0, c1, 25, 279, 126, 318);
  DWIN_Draw_Rectangle(0, c1, 24, 278, 127, 319);
  DWIN_Draw_Rectangle(0, c2, 145, 279, 246, 318);
  DWIN_Draw_Rectangle(0, c2, 144, 278, 247, 319);
}

void CrealityDWINClass::Update_Status_Bar(bool refresh/*=false*/) {
  static bool new_msg;
  static uint8_t msgscrl = 0;
  static char lastmsg[128];
  if (strcmp(lastmsg, statusmsg) != 0 || refresh) {
    strcpy(lastmsg, statusmsg);
    msgscrl = 0;
    new_msg = true;
  }
  size_t len = strlen(statusmsg);
  int8_t pos = len;
  if (pos > STATUS_CHAR_LIMIT) {
    pos -= msgscrl;
    len = _MIN((size_t)pos, (size_t)STATUS_CHAR_LIMIT);
    char dispmsg[len + 1];
    if (pos >= 0) {
      LOOP_L_N(i, len) dispmsg[i] = statusmsg[i + msgscrl];
    }
    else {
      LOOP_L_N(i, STATUS_CHAR_LIMIT + pos) dispmsg[i] = ' ';
      LOOP_S_L_N(i, STATUS_CHAR_LIMIT + pos, STATUS_CHAR_LIMIT) dispmsg[i] = statusmsg[i - (STATUS_CHAR_LIMIT + pos)];
    }
    dispmsg[len] = '\0';
    if (process == Print) {
      DWIN_Draw_Rectangle(1, Def_StatusBg_Color, 8, 214, DWIN_WIDTH - 8, 238);
      const int8_t npos = (DWIN_WIDTH - STATUS_CHAR_LIMIT * MENU_CHR_W) / 2;
      DWINUI::Draw_String(GetColor(eeprom_settings.status_bar_text, Def_StatusTxt_Color), Def_StatusBg_Color, npos, 219, dispmsg);
    }
    else {
      DWIN_Draw_Rectangle(1, Def_StatusBg_Color, 8, 352, DWIN_WIDTH - 8, 376);
      const int8_t npos = (DWIN_WIDTH - STATUS_CHAR_LIMIT * MENU_CHR_W) / 2;
      DWINUI::Draw_String(GetColor(eeprom_settings.status_bar_text, Def_StatusTxt_Color), Def_StatusBg_Color, npos, 357, dispmsg);
    }
    if (-pos >= STATUS_CHAR_LIMIT) msgscrl = 0;
    msgscrl++;
  }
  else {
    if (new_msg) {
      new_msg = false;
      if (process == Print) {
        DWIN_Draw_Rectangle(1, Def_StatusBg_Color, 8, 214, DWIN_WIDTH - 8, 238);
        const int8_t npos = (DWIN_WIDTH - strlen(statusmsg) * MENU_CHR_W) / 2;
        DWINUI::Draw_String(GetColor(eeprom_settings.status_bar_text, Def_StatusTxt_Color), Def_StatusBg_Color, npos, 219, statusmsg);
      }
      else {
        DWIN_Draw_Rectangle(1, Def_StatusBg_Color, 8, 352, DWIN_WIDTH - 8, 376);
        const int8_t npos = (DWIN_WIDTH - strlen(statusmsg) * MENU_CHR_W) / 2;
        DWINUI::Draw_String(GetColor(eeprom_settings.status_bar_text, Def_StatusTxt_Color), Def_StatusBg_Color, npos, 357, statusmsg);
      }
    }
  }
}

#if HAS_HOSTACTION_MENUS

  void CrealityDWINClass::Draw_Keyboard(bool restrict, bool numeric, uint8_t selected, bool uppercase/*=false*/, bool lock/*=false*/) {
    process = Keyboard;
    keyboard_restrict = restrict;
    numeric_keyboard = numeric;
    DWIN_Draw_Rectangle(0, Def_SplitLine_Color, 0, KEY_Y_START, DWIN_WIDTH-2, DWIN_HEIGHT-2);
    DWIN_Draw_Rectangle(1, Def_Background_Color, 1, KEY_Y_START+1, DWIN_WIDTH-3, DWIN_HEIGHT-3);
    LOOP_L_N(i, 36) Draw_Keys(i, (i == selected), uppercase, lock);
  }

  void CrealityDWINClass::Draw_Keys(uint8_t index, bool selected, bool uppercase/*=false*/, bool lock/*=false*/) {
    const char *keys;
    if (numeric_keyboard) keys = "1234567890&<>() {}[]*\"\':;!?";
    else keys = (uppercase) ? "QWERTYUIOPASDFGHJKLZXCVBNM" : "qwertyuiopasdfghjklzxcvbnm";
    #define KEY_X1(x) x*KEY_WIDTH+KEY_INSET+KEY_PADDING
    #define KEY_X2(x) (x+1) * KEY_WIDTH+KEY_INSET-KEY_PADDING
    #define KEY_Y1(y) KEY_Y_START+KEY_INSET+KEY_PADDING+y*KEY_HEIGHT
    #define KEY_Y2(y) KEY_Y_START+KEY_INSET-KEY_PADDING+(y+1) * KEY_HEIGHT

    const uint8_t rowCount[3] = { 10, 9, 7 };
    const float xOffset[3] = { 0, 0.5f * KEY_WIDTH, 1.5f * KEY_WIDTH };

    if (index < 28) {
      if (index == 19) {
        DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(0), KEY_Y1(2), KEY_X2(0) + xOffset[1], KEY_Y2(2));
        DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(0) + 1, KEY_Y1(2) + 1, KEY_X2(0) + xOffset[1] - 1, KEY_Y2(2) - 1);
        if (!numeric_keyboard) {
          if (lock) {
            DWIN_Draw_Line(Def_Selected_Color, KEY_X1(0) + 17, KEY_Y1(2) + 16, KEY_X1(0) + 25, KEY_Y1(2) + 8);
            DWIN_Draw_Line(Def_Selected_Color, KEY_X1(0) + 17, KEY_Y1(2) + 16, KEY_X1(0) + 9, KEY_Y1(2) + 8);
          }
          else {
            DWIN_Draw_Line((uppercase) ? Def_Selected_Color : Def_Text_Color, KEY_X1(0) + 17, KEY_Y1(2) + 8, KEY_X1(0) + 25, KEY_Y1(2) + 16);
            DWIN_Draw_Line((uppercase) ? Def_Selected_Color : Def_Text_Color, KEY_X1(0) + 17, KEY_Y1(2) + 8, KEY_X1(0) + 9, KEY_Y1(2) + 16);
          }
        }
      }
      else if (index == 27) {
        DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(7) + xOffset[2], KEY_Y1(2), KEY_X2(9), KEY_Y2(2));
        DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(7) + xOffset[2] + 1, KEY_Y1(2) + 1, KEY_X2(9) - 1, KEY_Y2(2) - 1);
        DWINUI::Draw_String(Color_Red, KEY_X1(7) + xOffset[2] + 3, KEY_Y1(2) + 5, F("<--"));
      }
      else {
        if (index > 19) index--;
        if (index > 27) index--;
        uint8_t y, x;
        if (index < rowCount[0]) y = 0, x = index;
        else if (index < (rowCount[0] + rowCount[1])) y = 1, x = index-rowCount[0];
        else y = 2, x = index-(rowCount[0] + rowCount[1]);
        const char keyStr[2] = {keys[(y > 0) * rowCount[0] + (y > 1) * rowCount[1] + x], '\0'};
        DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(x) + xOffset[y], KEY_Y1(y), KEY_X2(x) + xOffset[y], KEY_Y2(y));
        DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(x) + xOffset[y] + 1, KEY_Y1(y) + 1, KEY_X2(x) + xOffset[y] - 1, KEY_Y2(y) - 1);
        DWINUI::Draw_String(KEY_X1(x) + xOffset[y] + 5, KEY_Y1(y) + 5, keyStr);
        if (keyboard_restrict && numeric_keyboard && index > 9) {
          DWIN_Draw_Line(Color_Light_Red, KEY_X1(x) + xOffset[y] + 1, KEY_Y1(y) + 1, KEY_X2(x) + xOffset[y] - 1, KEY_Y2(y) - 1);
          DWIN_Draw_Line(Color_Light_Red, KEY_X1(x) + xOffset[y] + 1, KEY_Y2(y) - 1, KEY_X2(x) + xOffset[y] - 1, KEY_Y1(y) + 1);
        }
      }
    }
    else {
      switch (index) {
        case 28:
          DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(0), KEY_Y1(3), KEY_X2(0) + xOffset[1], KEY_Y2(3));
          DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(0) + 1, KEY_Y1(3) + 1, KEY_X2(0) + xOffset[1] - 1, KEY_Y2(3) - 1);
          DWINUI::Draw_String(KEY_X1(0) - 1, KEY_Y1(3) + 5, F("?123"));
          break;
        case 29:
          DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(1) + xOffset[1], KEY_Y1(3), KEY_X2(1) + xOffset[1], KEY_Y2(3));
          DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(1) + xOffset[1] + 1, KEY_Y1(3) + 1, KEY_X2(1) + xOffset[1] - 1, KEY_Y2(3) - 1);
          DWINUI::Draw_String(KEY_X1(1) + xOffset[1] + 5, KEY_Y1(3) + 5, F("-"));
          break;
        case 30:
          DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(2) + xOffset[1], KEY_Y1(3), KEY_X2(2) + xOffset[1], KEY_Y2(3));
          DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(2) + xOffset[1] + 1, KEY_Y1(3) + 1, KEY_X2(2) + xOffset[1] - 1, KEY_Y2(3) - 1);
          DWINUI::Draw_String(KEY_X1(2) + xOffset[1] + 5, KEY_Y1(3) + 5, F("_"));
          break;
        case 31:
          DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(3) + xOffset[1], KEY_Y1(3), KEY_X2(5) + xOffset[1], KEY_Y2(3));
          DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(3) + xOffset[1] + 1, KEY_Y1(3) + 1, KEY_X2(5) + xOffset[1] - 1, KEY_Y2(3) - 1);
          DWINUI::Draw_String(KEY_X1(3) + xOffset[1] + 14, KEY_Y1(3) + 5, F("Space"));
          if (keyboard_restrict) {
            DWIN_Draw_Line(Color_Light_Red, KEY_X1(3) + xOffset[1] + 1, KEY_Y1(3) + 1, KEY_X2(5) + xOffset[1] - 1, KEY_Y2(3) - 1);
            DWIN_Draw_Line(Color_Light_Red, KEY_X1(3) + xOffset[1] + 1, KEY_Y2(3) - 1, KEY_X2(5) + xOffset[1] - 1, KEY_Y1(3) + 1);
          }
          break;
        case 32:
          DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(6) + xOffset[1], KEY_Y1(3), KEY_X2(6) + xOffset[1], KEY_Y2(3));
          DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(6) + xOffset[1] + 1, KEY_Y1(3) + 1, KEY_X2(6) + xOffset[1] - 1, KEY_Y2(3) - 1);
          DWINUI::Draw_String(KEY_X1(6) + xOffset[1] + 7, KEY_Y1(3) + 5, F("."));
          if (keyboard_restrict) {
            DWIN_Draw_Line(Color_Light_Red, KEY_X1(6) + xOffset[1] + 1, KEY_Y1(3) + 1, KEY_X2(6) + xOffset[1] - 1, KEY_Y2(3) - 1);
            DWIN_Draw_Line(Color_Light_Red, KEY_X1(6) + xOffset[1] + 1, KEY_Y2(3) - 1, KEY_X2(6) + xOffset[1] - 1, KEY_Y1(3) + 1);
          }
          break;
        case 33:
          DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(7) + xOffset[1], KEY_Y1(3), KEY_X2(7) + xOffset[1], KEY_Y2(3));
          DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(7) + xOffset[1] + 1, KEY_Y1(3) + 1, KEY_X2(7) + xOffset[1] - 1, KEY_Y2(3) - 1);
          DWINUI::Draw_String(KEY_X1(7) + xOffset[1] + 4, KEY_Y1(3) + 5, F("/"));
          if (keyboard_restrict) {
            DWIN_Draw_Line(Color_Light_Red, KEY_X1(7) + xOffset[1] + 1, KEY_Y1(3) + 1, KEY_X2(7) + xOffset[1] - 1, KEY_Y2(3) - 1);
            DWIN_Draw_Line(Color_Light_Red, KEY_X1(7) + xOffset[1] + 1, KEY_Y2(3) - 1, KEY_X2(7) + xOffset[1] - 1, KEY_Y1(3) + 1);
          }
          break;
        case 34:
          DWIN_Draw_Rectangle(0, Color_Light_Blue, KEY_X1(7) + xOffset[2], KEY_Y1(3), KEY_X2(9), KEY_Y2(3));
          DWIN_Draw_Rectangle(0, (selected) ? Def_Selected_Color : Def_Background_Color, KEY_X1(7) + xOffset[2] + 1, KEY_Y1(3) + 1, KEY_X2(9) - 1, KEY_Y2(3) - 1);
          DWINUI::Draw_String(Color_Cyan, KEY_X1(7) + xOffset[2] + 3, KEY_Y1(3) + 5, F("-->"));
          break;
      }
    }
  }
#endif // HAS_HOSTACTION_MENUS

/* Menu Item Config */

void CrealityDWINClass::Menu_Item_Handler(uint8_t menu, uint8_t item, bool draw/*=true*/) {
  const uint8_t row = item - scrollpos;
  #if HAS_LEVELING
    static bool level_state;
  #endif

  #if HAS_PREHEAT

    #define PREHEAT_BACK 0
    #define PREHEAT_SUBMENU_HOTEND (PREHEAT_BACK + ENABLED(HAS_HOTEND))
    #define PREHEAT_SUBMENU_BED (PREHEAT_SUBMENU_HOTEND + ENABLED(HAS_HEATED_BED))
    #define PREHEAT_SUBMENU_FAN (PREHEAT_SUBMENU_BED + ENABLED(HAS_FAN))
    #define PREHEAT_SUBMENU_TOTAL PREHEAT_SUBMENU_FAN

    auto preheat_submenu = [&](const int index, const uint8_t item, const uint8_t sel) {
      switch (item) {
        case PREHEAT_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(TempMenu, sel);
          break;
        #if HAS_HOTEND
          case PREHEAT_SUBMENU_HOTEND:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetEndTemp, F("Hotend"));
              Draw_Float(ui.material_preset[index].hotend_temp, row, false, 1);
            }
            else
              Modify_Value(ui.material_preset[index].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
            break;
        #endif
        #if HAS_HEATED_BED
          case PREHEAT_SUBMENU_BED:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetBedTemp, F("Bed"));
              Draw_Float(ui.material_preset[index].bed_temp, row, false, 1);
            }
            else
              Modify_Value(ui.material_preset[index].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
            break;
        #endif
        #if HAS_FAN
          case PREHEAT_SUBMENU_FAN:
            if (draw) {
              Draw_Menu_Item(row, ICON_FanSpeed, GET_TEXT_F(MSG_FAN_SPEED));
              Draw_Float(ui.material_preset[index].fan_speed, row, false, 1);
            }
            else
              Modify_Value(ui.material_preset[index].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
            break;
        #endif
      }
    };

  #endif

  switch (menu) {
    case Prepare:

      #define PREPARE_BACK 0
      #define PREPARE_MOVE (PREPARE_BACK + 1)
      #define PREPARE_DISABLE (PREPARE_MOVE + 1)
      #define PREPARE_HOME (PREPARE_DISABLE + 1)
      #define PREPARE_MANUALLEVEL (PREPARE_HOME + 1)
      #define PREPARE_ZOFFSET (PREPARE_MANUALLEVEL + ENABLED(HAS_ZOFFSET_ITEM))
      #define PREPARE_PREHEAT (PREPARE_ZOFFSET + ENABLED(HAS_PREHEAT))
      #define PREPARE_COOLDOWN (PREPARE_PREHEAT + EITHER(HAS_HOTEND, HAS_HEATED_BED))
      #define PREPARE_CHANGEFIL (PREPARE_COOLDOWN + ENABLED(ADVANCED_PAUSE_FEATURE))
      #define PREPARE_ACTIONCOMMANDS (PREPARE_CHANGEFIL + ENABLED(HAS_HOSTACTION_MENUS))
      #define PREPARE_CUSTOM_MENU (PREPARE_ACTIONCOMMANDS + ENABLED(HAS_CUSTOM_MENU))
      #define PREPARE_TOTAL PREPARE_ACTIONCOMMANDS

      switch (item) {
        case PREPARE_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Main_Menu(1);
          break;
        case PREPARE_MOVE:
          if (draw)
            Draw_Menu_Item(row, ICON_Axis, GET_TEXT_F(MSG_MOVE_AXIS), nullptr, true);
          else
            Draw_Menu(Move);
          break;
        case PREPARE_DISABLE:
          if (draw)
            Draw_Menu_Item(row, ICON_CloseMotor, GET_TEXT_F(MSG_DISABLE_STEPPERS));
          else
            queue.inject(F("M84"));
          break;
        case PREPARE_HOME:
          if (draw)
            Draw_Menu_Item(row, ICON_SetHome, GET_TEXT_F(MSG_HOMING), nullptr, true);
          else
            Draw_Menu(HomeMenu);
          break;
        case PREPARE_MANUALLEVEL:
          if (draw)
            Draw_Menu_Item(row, ICON_PrintSize, GET_TEXT_F(MSG_BED_TRAMMING_MANUAL), nullptr, true);
          else {
            if (axes_should_home()) {
              Popup_Handler(Home);
              gcode.home_all_axes(true);
            }
            #if HAS_LEVELING
              level_state = planner.leveling_active;
              set_bed_leveling_enabled(false);
            #endif
            Draw_Menu(ManualLevel);
          }
          break;

        #if HAS_ZOFFSET_ITEM
          case PREPARE_ZOFFSET:
            if (draw)
              Draw_Menu_Item(row, ICON_Zoffset, F("Z-Offset"), nullptr, true);
            else {
              #if HAS_LEVELING
                level_state = planner.leveling_active;
                set_bed_leveling_enabled(false);
              #endif
              Draw_Menu(ZOffset);
            }
            break;
        #endif

        #if HAS_PREHEAT
          case PREPARE_PREHEAT:
            if (draw)
              Draw_Menu_Item(row, ICON_Temperature, F("Preheat"), nullptr, true);
            else
              Draw_Menu(Preheat);
            break;
        #endif

        #if HAS_HOTEND || HAS_HEATED_BED
          case PREPARE_COOLDOWN:
            if (draw)
              Draw_Menu_Item(row, ICON_Cool, GET_TEXT_F(MSG_COOLDOWN));
            else {
              thermalManager.cooldown();
              Update_Status(GET_TEXT(MSG_COOLDOWN));
            }
            break;
        #endif

        #if HAS_HOSTACTION_MENUS
          case PREPARE_ACTIONCOMMANDS:
          if (draw)
            Draw_Menu_Item(row, ICON_SetHome, F("Host Actions"), nullptr, true);
          else
            Draw_Menu(HostActions);
          break;
        #endif

        #if HAS_CUSTOM_MENU
          case PREPARE_CUSTOM_MENU:
            #ifndef CUSTOM_MENU_CONFIG_TITLE
              #define CUSTOM_MENU_CONFIG_TITLE "Custom Commands"
            #endif
            if (draw)
              Draw_Menu_Item(row, ICON_Version, F(CUSTOM_MENU_CONFIG_TITLE));
            else
              Draw_Menu(MenuCustom);
            break;
        #endif

        #if ENABLED(ADVANCED_PAUSE_FEATURE)
          case PREPARE_CHANGEFIL:
            if (draw) {
              Draw_Menu_Item(row, ICON_ResumeEEPROM, GET_TEXT_F(MSG_FILAMENTCHANGE)
                #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
                  , nullptr, true
                #endif
              );
            }
            else {
              #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
                Draw_Menu(ChangeFilament);
              #else
                if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp)
                  Popup_Handler(ETemp);
                else {
                  if (thermalManager.temp_hotend[0].is_below_target(-2)) {
                    Popup_Handler(Heating);
                    thermalManager.wait_for_hotend(0);
                  }
                  Popup_Handler(FilChange);
                  sprintf_P(cmd, PSTR("M600 B1 R%i"), thermalManager.temp_hotend[0].target);
                  gcode.process_subcommands_now(cmd);
                }
              #endif
            }
            break;
        #endif
      }
      break;

    case HomeMenu:

      #define HOME_BACK  0
      #define HOME_ALL   (HOME_BACK + 1)
      #define HOME_X     (HOME_ALL + 1)
      #define HOME_Y     (HOME_X + 1)
      #define HOME_Z     (HOME_Y + 1)
      #define HOME_SET   (HOME_Z + 1)
      #define HOME_TOTAL HOME_SET

      switch (item) {
        case HOME_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Prepare, PREPARE_HOME);
          break;
        case HOME_ALL:
          if (draw)
            Draw_Menu_Item(row, ICON_Homing, GET_TEXT_F(MSG_AUTO_HOME));
          else {
            Popup_Handler(Home);
            gcode.home_all_axes(true);
            Redraw_Menu();
          }
          break;
        case HOME_X:
          if (draw)
            Draw_Menu_Item(row, ICON_MoveX, GET_TEXT_F(MSG_AUTO_HOME_X));
          else {
            Popup_Handler(Home);
            gcode.process_subcommands_now(F("G28 X"));
            planner.synchronize();
            Redraw_Menu();
          }
          break;
        case HOME_Y:
          if (draw)
            Draw_Menu_Item(row, ICON_MoveY, GET_TEXT_F(MSG_AUTO_HOME_Y));
          else {
            Popup_Handler(Home);
            gcode.process_subcommands_now(F("G28 Y"));
            planner.synchronize();
            Redraw_Menu();
          }
          break;
        case HOME_Z:
          if (draw)
            Draw_Menu_Item(row, ICON_MoveZ, GET_TEXT_F(MSG_AUTO_HOME_Z));
          else {
            Popup_Handler(Home);
            gcode.process_subcommands_now(F("G28 Z"));
            planner.synchronize();
            Redraw_Menu();
          }
          break;
        case HOME_SET:
          if (draw)
            Draw_Menu_Item(row, ICON_SetHome, GET_TEXT_F(MSG_SET_HOME_OFFSETS));
          else {
            gcode.process_subcommands_now(F("G92X0Y0Z0"));
            AudioFeedback();
          }
          break;
      }
      break;

    case Move:

      #define MOVE_BACK 0
      #define MOVE_X (MOVE_BACK + 1)
      #define MOVE_Y (MOVE_X + 1)
      #define MOVE_Z (MOVE_Y + 1)
      #define MOVE_E (MOVE_Z + ENABLED(HAS_HOTEND))
      #define MOVE_P (MOVE_E + ENABLED(HAS_BED_PROBE))
      #define MOVE_LIVE (MOVE_P + 1)
      #define MOVE_TOTAL MOVE_LIVE

      switch (item) {
        case MOVE_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else {
            #if HAS_BED_PROBE
              temp_val.probe_deployed = false;
              probe.set_deployed(temp_val.probe_deployed);
            #endif
            Draw_Menu(Prepare, PREPARE_MOVE);
          }
          break;
        case MOVE_X:
          if (draw) {
            Draw_Menu_Item(row, ICON_MoveX, GET_TEXT_F(MSG_MOVE_X));
            Draw_Float(current_position.x, row, false);
          }
          else
            Modify_Value(current_position.x, X_MIN_POS, X_MAX_POS, 10);
          break;
        case MOVE_Y:
          if (draw) {
            Draw_Menu_Item(row, ICON_MoveY, GET_TEXT_F(MSG_MOVE_Y));
            Draw_Float(current_position.y, row);
          }
          else
            Modify_Value(current_position.y, Y_MIN_POS, Y_MAX_POS, 10);
          break;
        case MOVE_Z:
          if (draw) {
            Draw_Menu_Item(row, ICON_MoveZ, GET_TEXT_F(MSG_MOVE_Z));
            Draw_Float(current_position.z, row);
          }
          else
            Modify_Value(current_position.z, Z_MIN_POS, Z_MAX_POS, 10);
          break;

        #if HAS_HOTEND
          case MOVE_E:
            if (draw) {
              Draw_Menu_Item(row, ICON_Extruder, GET_TEXT_F(MSG_MOVE_E));
              current_position.e = 0;
              sync_plan_position();
              Draw_Float(current_position.e, row);
            }
            else {
              if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp)
                Popup_Handler(ETemp);
              else {
                if (thermalManager.temp_hotend[0].is_below_target(-2)) {
                  Popup_Handler(Heating);
                  thermalManager.wait_for_hotend(0);
                  Redraw_Menu();
                }
                current_position.e = 0;
                sync_plan_position();
                Modify_Value(current_position.e, -500, 500, 10);
              }
            }
          break;
        #endif // HAS_HOTEND

        #if HAS_BED_PROBE
          case MOVE_P:
            if (draw) {
              Draw_Menu_Item(row, ICON_ProbeDeploy, GET_TEXT_F(MSG_MANUAL_DEPLOY));
              Draw_Checkbox(row, temp_val.probe_deployed);
            }
            else {
              temp_val.probe_deployed = !temp_val.probe_deployed;
              probe.set_deployed(temp_val.probe_deployed);
              Draw_Checkbox(row, temp_val.probe_deployed);
            }
            break;
        #endif

        case MOVE_LIVE:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, F("Live Movement"));
            Draw_Checkbox(row, temp_val.livemove);
          }
          else {
            temp_val.livemove = !temp_val.livemove;
            Draw_Checkbox(row, temp_val.livemove);
          }
          break;
      }
      break;
    case ManualLevel:

      #define MLEVEL_BACK 0
      #define MLEVEL_PROBE (MLEVEL_BACK + ENABLED(HAS_BED_PROBE))
      #define MLEVEL_FL (MLEVEL_PROBE + 1)
      #define MLEVEL_BL (MLEVEL_FL + 1)
      #define MLEVEL_BR (MLEVEL_BL + 1)
      #define MLEVEL_FR (MLEVEL_BR + 1)
      #define MLEVEL_C (MLEVEL_FR + 1)
      #define MLEVEL_ZPOS (MLEVEL_C + 1)
      #define MLEVEL_TOTAL MLEVEL_ZPOS

      static float mlev_z_pos = 0;
      static bool use_probe = false;

      switch (item) {
        case MLEVEL_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else {
            TERN_(HAS_LEVELING, set_bed_leveling_enabled(level_state));
            Draw_Menu(Prepare, PREPARE_MANUALLEVEL);
          }
          break;
        #if HAS_BED_PROBE
          case MLEVEL_PROBE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Zoffset, F("Use Probe"));
              Draw_Checkbox(row, use_probe);
            }
            else {
              use_probe = !use_probe;
              Draw_Checkbox(row, use_probe);
              if (use_probe) {
                Popup_Handler(Level);
                do_z_clearance(Z_HOMING_HEIGHT);
                temp_val.corner_avg = 0;
                #define PROBE_X_MIN _MAX(temp_val.corner_pos, PROBING_MARGIN, MESH_MIN_X) - probe.offset.x
                #define PROBE_X_MAX _MIN(X_BED_SIZE - temp_val.corner_pos, X_BED_SIZE - PROBING_MARGIN, MESH_MAX_X) - probe.offset.x
                #define PROBE_Y_MIN _MAX(temp_val.corner_pos, PROBING_MARGIN, MESH_MIN_Y) - probe.offset.y
                #define PROBE_Y_MAX _MIN(Y_BED_SIZE - temp_val.corner_pos, Y_BED_SIZE - PROBING_MARGIN, MESH_MAX_Y) - probe.offset.y
                temp_val.zval = probe.probe_at_point(PROBE_X_MIN, PROBE_Y_MIN, PROBE_PT_RAISE, 0, false);
                const char * MSG_UNREACHABLE = "Position unreachable. Check Probe Offsets and Bed Screw Inset.";
                if (isnan(temp_val.zval)) {
                  Update_Status(MSG_UNREACHABLE);
                  Redraw_Menu();
                }
                temp_val.corner_avg += temp_val.zval;
                temp_val.zval = probe.probe_at_point(PROBE_X_MIN, PROBE_Y_MAX, PROBE_PT_RAISE, 0, false);
                if (isnan(temp_val.zval)) {
                  Update_Status(MSG_UNREACHABLE);
                  Redraw_Menu();
                }
                temp_val.corner_avg += temp_val.zval;
                temp_val.zval = probe.probe_at_point(PROBE_X_MAX, PROBE_Y_MAX, PROBE_PT_RAISE, 0, false);
                if (isnan(temp_val.zval)) {
                  Update_Status(MSG_UNREACHABLE);
                  Redraw_Menu();
                }
                temp_val.corner_avg += temp_val.zval;
                temp_val.zval = probe.probe_at_point(PROBE_X_MAX, PROBE_Y_MIN, PROBE_PT_STOW, 0, false);
                if (isnan(temp_val.zval)) {
                  Update_Status(MSG_UNREACHABLE);
                  Redraw_Menu();
                }
                temp_val.corner_avg += temp_val.zval;
                temp_val.corner_avg /= 4;
                Redraw_Menu();
              }
            }
            break;
        #endif
        case MLEVEL_FL:
          if (draw)
            Draw_Menu_Item(row, ICON_AxisBL, GET_TEXT_F(MSG_LEVBED_FL));
          else {
            Popup_Handler(MoveWait);
            if (use_probe) {
              #if HAS_BED_PROBE
                sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s"), dtostrf(PROBE_X_MIN, 1, 3, str_1), dtostrf(PROBE_Y_MIN, 1, 3, str_2));
                gcode.process_subcommands_now(cmd);
                planner.synchronize();
                Popup_Handler(ManualProbing);
              #endif
            }
            else {
              sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s\nG0 F300 Z%s"), dtostrf(temp_val.corner_pos, 1, 3, str_1), dtostrf(temp_val.corner_pos, 1, 3, str_2), dtostrf(mlev_z_pos, 1, 3, str_3));
              gcode.process_subcommands_now(cmd);
              planner.synchronize();
              Redraw_Menu();
            }
          }
          break;
        case MLEVEL_BL:
          if (draw)
            Draw_Menu_Item(row, ICON_AxisTL, GET_TEXT_F(MSG_LEVBED_BL));
          else {
            Popup_Handler(MoveWait);
            if (use_probe) {
              #if HAS_BED_PROBE
                sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s"), dtostrf(PROBE_X_MIN, 1, 3, str_1), dtostrf(PROBE_Y_MAX, 1, 3, str_2));
                gcode.process_subcommands_now(cmd);
                planner.synchronize();
                Popup_Handler(ManualProbing);
              #endif
            }
            else {
              sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s\nG0 F300 Z%s"), dtostrf(temp_val.corner_pos, 1, 3, str_1), dtostrf(Y_BED_SIZE - temp_val.corner_pos, 1, 3, str_2), dtostrf(mlev_z_pos, 1, 3, str_3));
              gcode.process_subcommands_now(cmd);
              planner.synchronize();
              Redraw_Menu();
            }
          }
          break;
        case MLEVEL_BR:
          if (draw)
            Draw_Menu_Item(row, ICON_AxisTR, GET_TEXT_F(MSG_LEVBED_BR));
          else {
            Popup_Handler(MoveWait);
            if (use_probe) {
              #if HAS_BED_PROBE
                sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s"), dtostrf(PROBE_X_MAX, 1, 3, str_1), dtostrf(PROBE_Y_MAX, 1, 3, str_2));
                gcode.process_subcommands_now(cmd);
                planner.synchronize();
                Popup_Handler(ManualProbing);
              #endif
            }
            else {
              sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s\nG0 F300 Z%s"), dtostrf(X_BED_SIZE - temp_val.corner_pos, 1, 3, str_1), dtostrf(Y_BED_SIZE - temp_val.corner_pos, 1, 3, str_2), dtostrf(mlev_z_pos, 1, 3, str_3));
              gcode.process_subcommands_now(cmd);
              planner.synchronize();
              Redraw_Menu();
            }
          }
          break;
        case MLEVEL_FR:
          if (draw)
            Draw_Menu_Item(row, ICON_AxisBR, GET_TEXT_F(MSG_LEVBED_FR));
          else {
            Popup_Handler(MoveWait);
            if (use_probe) {
              #if HAS_BED_PROBE
                sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s"), dtostrf(PROBE_X_MAX, 1, 3, str_1), dtostrf(PROBE_Y_MIN, 1, 3, str_2));
                gcode.process_subcommands_now(cmd);
                planner.synchronize();
                Popup_Handler(ManualProbing);
              #endif
            }
            else {
              sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s\nG0 F300 Z%s"), dtostrf(X_BED_SIZE - temp_val.corner_pos, 1, 3, str_1), dtostrf(temp_val.corner_pos, 1, 3, str_2), dtostrf(mlev_z_pos, 1, 3, str_3));
              gcode.process_subcommands_now(cmd);
              planner.synchronize();
              Redraw_Menu();
            }
          }
          break;
        case MLEVEL_C:
          if (draw)
            Draw_Menu_Item(row, ICON_AxisC, GET_TEXT_F(MSG_LEVBED_C));
          else {
            Popup_Handler(MoveWait);
            if (use_probe) {
              #if HAS_BED_PROBE
                sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s"), dtostrf((PROBE_X_MIN + PROBE_X_MAX) / 2.0f, 1, 3, str_1), dtostrf((PROBE_Y_MIN + PROBE_Y_MAX) / 2.0f, 1, 3, str_2));
                gcode.process_subcommands_now(cmd);
                planner.synchronize();
                Popup_Handler(ManualProbing);
              #endif
            }
            else {
              sprintf_P(cmd, PSTR("G0 F4000\nG0 Z10\nG0 X%s Y%s\nG0 F300 Z%s"), dtostrf(X_BED_SIZE / 2.0f, 1, 3, str_1), dtostrf(Y_BED_SIZE / 2.0f, 1, 3, str_2), dtostrf(mlev_z_pos, 1, 3, str_3));
              gcode.process_subcommands_now(cmd);
              planner.synchronize();
              Redraw_Menu();
            }
          }
          break;
        case MLEVEL_ZPOS:
          if (draw) {
            Draw_Menu_Item(row, ICON_SetZOffset, GET_TEXT_F(MSG_MOVE_Z));
            Draw_Float(mlev_z_pos, row, false, 100);
          }
          else
            Modify_Value(mlev_z_pos, 0, MAX_Z_OFFSET, 100);
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
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else {
              temp_val.zoffsetmode = 0;
              #if !HAS_BED_PROBE
                gcode.process_subcommands_now(F("M211 S1"));  // Soft end-stops
              #endif
              TERN_(HAS_LEVELING, set_bed_leveling_enabled(level_state));
              Draw_Menu(Prepare, PREPARE_ZOFFSET);
            }
            break;
          case ZOFFSET_HOME:
            if (draw)
              Draw_Menu_Item(row, ICON_Homing, GET_TEXT_F(MSG_AUTO_HOME_Z));
            else {
              Popup_Handler(Home);
              gcode.process_subcommands_now(F("G28 Z"));
              Popup_Handler(MoveWait);
              #if ENABLED(Z_SAFE_HOMING)
                planner.synchronize();
                sprintf_P(cmd, PSTR("G0 F4000 X%s Y%s"), dtostrf(Z_SAFE_HOMING_X_POINT, 1, 3, str_1), dtostrf(Z_SAFE_HOMING_Y_POINT, 1, 3, str_2));
                gcode.process_subcommands_now(cmd);
              #else
                sprintf_P(cmd, PSTR("G0 F4000 X%s Y%s"), dtostrf((X_BED_SIZE + X_MIN_POS) / 2.0f, 1, 3, str_1), dtostrf((Y_BED_SIZE + Y_MIN_POS) / 2.0f, 1, 3, str_2));
                gcode.process_subcommands_now(cmd);
              #endif
              gcode.process_subcommands_now(F("G0 F300 Z0"));
              planner.synchronize();
              Redraw_Menu();
            }
            break;
          case ZOFFSET_MODE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Zoffset, F("Live Adjustment"));
              Draw_Option(temp_val.zoffsetmode, zoffset_modes, row);
            }
            else
              Modify_Option(temp_val.zoffsetmode, zoffset_modes, 2);
            break;
          case ZOFFSET_OFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetZOffset, F("Z Offset"));
              Draw_Float(temp_val.zoffsetvalue, row, false, 100);
            }
            else
              Modify_Value(temp_val.zoffsetvalue, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
            break;
          case ZOFFSET_UP:
            if (draw)
              Draw_Menu_Item(row, ICON_Axis, F("Microstep Up"));
            else {
              if (temp_val.zoffsetvalue < MAX_Z_OFFSET) {
                if (temp_val.zoffsetmode != 0) {
                  gcode.process_subcommands_now(F("M290 Z0.01"));
                  planner.synchronize();
                }
                temp_val.zoffsetvalue += 0.01;
                Draw_Float(temp_val.zoffsetvalue, row - 1, false, 100);
              }
            }
            break;
          case ZOFFSET_DOWN:
            if (draw)
              Draw_Menu_Item(row, ICON_AxisD, F("Microstep Down"));
            else {
              if (temp_val.zoffsetvalue > MIN_Z_OFFSET) {
                if (temp_val.zoffsetmode != 0) {
                  gcode.process_subcommands_now(F("M290 Z-0.01"));
                  planner.synchronize();
                }
                temp_val.zoffsetvalue -= 0.01;
                Draw_Float(temp_val.zoffsetvalue, row - 2, false, 100);
              }
            }
            break;
          #if ENABLED(EEPROM_SETTINGS)
            case ZOFFSET_SAVE:
              if (draw)
                Draw_Menu_Item(row, ICON_WriteEEPROM, GET_TEXT_F(MSG_BUTTON_SAVE));
              else
                AudioFeedback(settings.save());
              break;
          #endif
        }
        break;
    #endif

    #if HAS_PREHEAT
      case Preheat: {
        #define PREHEAT_MODE (PREHEAT_BACK + 1)
        #define PREHEAT_1 (PREHEAT_MODE + 1)
        #define PREHEAT_2 (PREHEAT_1 + (PREHEAT_COUNT >= 2))
        #define PREHEAT_3 (PREHEAT_2 + (PREHEAT_COUNT >= 3))
        #define PREHEAT_4 (PREHEAT_3 + (PREHEAT_COUNT >= 4))
        #define PREHEAT_5 (PREHEAT_4 + (PREHEAT_COUNT >= 5))
        #define PREHEAT_TOTAL PREHEAT_5

        auto do_preheat = [](const uint8_t m) {
          thermalManager.cooldown();
          if (temp_val.preheatmode == 0 || temp_val.preheatmode == 1) { ui.preheat_hotend_and_fan(m); }
          if (temp_val.preheatmode == 0 || temp_val.preheatmode == 2) ui.preheat_bed(m);
        };

        switch (item) {
          case PREHEAT_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Prepare, PREPARE_PREHEAT);
            break;

          case PREHEAT_MODE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Homing, GET_TEXT_F(MSG_CONFIGURATION));
              Draw_Option(temp_val.preheatmode, preheat_modes, row);
            }
            else
              Modify_Option(temp_val.preheatmode, preheat_modes, 2);
            break;

          #define _PREHEAT_CASE(N) \
            case PREHEAT_##N: { \
              if (draw) Draw_Menu_Item(row, ICON_Temperature, F(PREHEAT_## N ##_LABEL)); \
              else do_preheat(N - 1); \
            } break;

          REPEAT_1(PREHEAT_COUNT, _PREHEAT_CASE)
        }
      } break;
    #endif // HAS_PREHEAT

    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      case ChangeFilament:

        #define CHANGEFIL_BACK 0
        #define CHANGEFIL_PARKHEAD (CHANGEFIL_BACK + 1)
        #define CHANGEFIL_LOAD (CHANGEFIL_PARKHEAD + 1)
        #define CHANGEFIL_UNLOAD (CHANGEFIL_LOAD + 1)
        #define CHANGEFIL_CHANGE (CHANGEFIL_UNLOAD + 1)
        #define CHANGEFIL_TOTAL CHANGEFIL_CHANGE

        switch (item) {
          case CHANGEFIL_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Prepare, PREPARE_CHANGEFIL);
            break;
          case CHANGEFIL_PARKHEAD:
            if (draw)
              Draw_Menu_Item(row, ICON_Park, GET_TEXT_F(MSG_FILAMENT_PARK_ENABLED));
            else {
              #if ENABLED(NOZZLE_PARK_FEATURE)
                queue.inject(F("G28O\nG27 P2"));
              #else
                sprintf_P(cmd, PSTR("G28O\nG0 F4000 X%i Y%i\nG0 F3000 Z%i"), 0 , 0, 20);
                queue.inject(cmd);
              #endif
            }
            break;
          case CHANGEFIL_LOAD:
            if (draw)
              Draw_Menu_Item(row, ICON_WriteEEPROM, GET_TEXT_F(MSG_FILAMENTLOAD));
            else {
              if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp)
                Popup_Handler(ETemp);
              else {
                if (thermalManager.temp_hotend[0].is_below_target(-2)) {
                  Popup_Handler(Heating);
                  thermalManager.wait_for_hotend(0);
                }
                Popup_Handler(FilLoad);
                gcode.process_subcommands_now(F("M701"));
                planner.synchronize();
                Redraw_Menu();
              }
            }
            break;
          case CHANGEFIL_UNLOAD:
            if (draw)
              Draw_Menu_Item(row, ICON_ReadEEPROM, GET_TEXT_F(MSG_FILAMENTUNLOAD));
            else {
              if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp) {
                Popup_Handler(ETemp);
              }
              else {
                if (thermalManager.temp_hotend[0].is_below_target(-2)) {
                  Popup_Handler(Heating);
                  thermalManager.wait_for_hotend(0);
                }
                Popup_Handler(FilLoad, true);
                gcode.process_subcommands_now(F("M702"));
                planner.synchronize();
                Redraw_Menu();
              }
            }
            break;
          case CHANGEFIL_CHANGE:
            if (draw)
              Draw_Menu_Item(row, ICON_ResumeEEPROM, GET_TEXT_F(MSG_FILAMENTCHANGE));
            else {
              if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp)
                Popup_Handler(ETemp);
              else {
                if (thermalManager.temp_hotend[0].is_below_target(-2)) {
                  Popup_Handler(Heating);
                  thermalManager.wait_for_hotend(0);
                }
                Popup_Handler(FilChange);
                sprintf_P(cmd, PSTR("M600 B1 R%i"), thermalManager.temp_hotend[0].target);
                gcode.process_subcommands_now(cmd);
              }
            }
            break;
        }
        break;
    #endif // FILAMENT_LOAD_UNLOAD_GCODES

    #if HAS_HOSTACTION_MENUS
      case HostActions:

        #define HOSTACTIONS_BACK 0
        #define HOSTACTIONS_1 (HOSTACTIONS_BACK + 1)
        #define HOSTACTIONS_2 (HOSTACTIONS_1 + 1)
        #define HOSTACTIONS_3 (HOSTACTIONS_2 + 1)
        #define HOSTACTIONS_TOTAL HOSTACTIONS_3

        switch (item) {
          case HOSTACTIONS_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else {
              if (temp_val.flag_tune) {
                temp_val.flag_tune = false;
                Redraw_Menu(false, true, true);
              }
              else
                Draw_Menu(Prepare, PREPARE_ACTIONCOMMANDS);
            }
            break;
          case HOSTACTIONS_1:
            if (draw) Draw_Menu_Item(row, ICON_File, action1);
            else if (!strcmp(action1, "-") == 0) hostui.action(F(action1));
            break;
          case HOSTACTIONS_2:
            if (draw) Draw_Menu_Item(row, ICON_File, action2);
            else if (!strcmp(action2, "-") == 0) hostui.action(F(action2));
            break;
          case HOSTACTIONS_3:
            if (draw) Draw_Menu_Item(row, ICON_File, action3);
            else if (!strcmp(action3, "-") == 0) hostui.action(F(action3));
            break;
        }
        break;
    #endif

    #if HAS_CUSTOM_MENU

      case MenuCustom:

        #define CUSTOM_MENU_BACK 0
        #define CUSTOM_MENU_1 1
        #define CUSTOM_MENU_2 2
        #define CUSTOM_MENU_3 3
        #define CUSTOM_MENU_4 4
        #define CUSTOM_MENU_5 5
        #define CUSTOM_MENU_TOTAL CUSTOM_MENU_COUNT

        switch (item) {
          case CUSTOM_MENU_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, F("Back"));
            else
              Draw_Menu(Prepare, PREPARE_CUSTOM_MENU);
            break;

          #if CUSTOM_MENU_COUNT >= 1
            case CUSTOM_MENU_1:
              if (draw)
                Draw_Menu_Item(row, ICON_Info, F(CONFIG_MENU_ITEM_1_DESC));
              else {
                Popup_Handler(Custom);
                //queue.inject(F(CONFIG_MENU_ITEM_1_GCODE)); // Old code
                gcode.process_subcommands_now(F(CONFIG_MENU_ITEM_1_GCODE));
                planner.synchronize();
                Redraw_Menu();
                #if ENABLED(CUSTOM_MENU_CONFIG_SCRIPT_AUDIBLE_FEEDBACK)
                  AudioFeedback();
                #endif
                #ifdef CUSTOM_MENU_CONFIG_SCRIPT_RETURN
                  queue.inject(F(CUSTOM_MENU_CONFIG_SCRIPT_DONE));
                #endif
              }
              break;
          #endif

          #if CUSTOM_MENU_COUNT >= 2
            case CUSTOM_MENU_2:
              if (draw)
                Draw_Menu_Item(row, ICON_Info, F(CONFIG_MENU_ITEM_2_DESC));
              else {
                Popup_Handler(Custom);
                gcode.process_subcommands_now(F(CONFIG_MENU_ITEM_2_GCODE));
                planner.synchronize();
                Redraw_Menu();
                #if ENABLED(CUSTOM_MENU_CONFIG_SCRIPT_AUDIBLE_FEEDBACK)
                  AudioFeedback();
                #endif
                #ifdef CUSTOM_MENU_CONFIG_SCRIPT_RETURN
                  queue.inject(F(CUSTOM_MENU_CONFIG_SCRIPT_DONE));
                #endif
              }
              break;
          #endif

          #if CUSTOM_MENU_COUNT >= 3
            case CUSTOM_MENU_3:
              if (draw)
                Draw_Menu_Item(row, ICON_Info, F(CONFIG_MENU_ITEM_3_DESC));
              else {
                Popup_Handler(Custom);
                gcode.process_subcommands_now(F(CONFIG_MENU_ITEM_3_GCODE));
                planner.synchronize();
                Redraw_Menu();
                #if ENABLED(CUSTOM_MENU_CONFIG_SCRIPT_AUDIBLE_FEEDBACK)
                  AudioFeedback();
                #endif
                #ifdef CUSTOM_MENU_CONFIG_SCRIPT_RETURN
                  queue.inject(F(CUSTOM_MENU_CONFIG_SCRIPT_DONE));
                #endif
              }
              break;
          #endif

          #if CUSTOM_MENU_COUNT >= 4
            case CUSTOM_MENU_4:
              if (draw)
                Draw_Menu_Item(row, ICON_Info, F(CONFIG_MENU_ITEM_4_DESC));
              else {
                Popup_Handler(Custom);
                gcode.process_subcommands_now(F(CONFIG_MENU_ITEM_4_GCODE));
                planner.synchronize();
                Redraw_Menu();
                #if ENABLED(CUSTOM_MENU_CONFIG_SCRIPT_AUDIBLE_FEEDBACK)
                  AudioFeedback();
                #endif
                #ifdef CUSTOM_MENU_CONFIG_SCRIPT_RETURN
                  queue.inject(F(CUSTOM_MENU_CONFIG_SCRIPT_DONE));
                #endif
              }
              break;
          #endif

          #if CUSTOM_MENU_COUNT >= 5
            case CUSTOM_MENU_5:
              if (draw)
                Draw_Menu_Item(row, ICON_Info, F(CONFIG_MENU_ITEM_5_DESC));
              else {
                Popup_Handler(Custom);
                gcode.process_subcommands_now(F(CONFIG_MENU_ITEM_5_GCODE));
                planner.synchronize();
                Redraw_Menu();
                #if ENABLED(CUSTOM_MENU_CONFIG_SCRIPT_AUDIBLE_FEEDBACK)
                  AudioFeedback();
                #endif
                #ifdef CUSTOM_MENU_CONFIG_SCRIPT_RETURN
                  queue.inject(F(CUSTOM_MENU_CONFIG_SCRIPT_DONE));
                #endif
              }
              break;
          #endif // Custom Menu
        }
        break;

    #endif // HAS_CUSTOM_MENU

    case Control:

      #define CONTROL_BACK 0
      #define CONTROL_TEMP (CONTROL_BACK + 1)
      #define CONTROL_MOTION (CONTROL_TEMP + 1)
      #define CONTROL_FWRETRACT (CONTROL_MOTION + ENABLED(FWRETRACT))
      #define CONTROL_LEDS (CONTROL_FWRETRACT + ANY(CASE_LIGHT_MENU, LED_CONTROL_MENU))
      #define CONTROL_VISUAL (CONTROL_LEDS + 1)
      #define CONTROL_HOSTSETTINGS (CONTROL_VISUAL + ENABLED(HAS_HOSTACTION_MENUS))
      #define CONTROL_ADVANCED (CONTROL_HOSTSETTINGS + 1)
      #define CONTROL_SAVE (CONTROL_ADVANCED + ENABLED(EEPROM_SETTINGS))
      #define CONTROL_RESTORE (CONTROL_SAVE + ENABLED(EEPROM_SETTINGS))
      #define CONTROL_RESET (CONTROL_RESTORE + ENABLED(EEPROM_SETTINGS))
      #define CONTROL_REBOOT (CONTROL_RESET + 1)
      #define CONTROL_INFO (CONTROL_REBOOT + 1)
      #define CONTROL_TOTAL CONTROL_INFO

      switch (item) {
        case CONTROL_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Main_Menu(2);
          break;
        case CONTROL_TEMP:
          if (draw)
            Draw_Menu_Item(row, ICON_Temperature, GET_TEXT_F(MSG_TEMPERATURE), nullptr, true);
          else
            Draw_Menu(TempMenu);
          break;
        case CONTROL_MOTION:
          if (draw)
            Draw_Menu_Item(row, ICON_Motion, GET_TEXT_F(MSG_MOTION), nullptr, true);
          else
            Draw_Menu(Motion);
          break;
        #if ENABLED(FWRETRACT)
          case CONTROL_FWRETRACT:
          if (draw)
            Draw_Menu_Item(row, ICON_StepE, GET_TEXT_F(MSG_AUTORETRACT), nullptr, true);
          else
            Draw_Menu(FwRetraction);
          break;
        #endif
        #if ANY(CASE_LIGHT_MENU, LED_CONTROL_MENU)
          case CONTROL_LEDS:
            if (draw)
              Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_LEDS), nullptr, true);
            else
              Draw_Menu(Ledsmenu);
            break;
        #endif
        case CONTROL_VISUAL:
          if (draw)
            Draw_Menu_Item(row, ICON_PrintSize, F("Visual"), nullptr, true);
          else
            Draw_Menu(Visual);
          break;
        #if HAS_HOSTACTION_MENUS
          case CONTROL_HOSTSETTINGS:
            if (draw)
              Draw_Menu_Item(row, ICON_Contact, F("Host Settings"), nullptr, true);
            else
              Draw_Menu(HostSettings);
            break;
        #endif
        case CONTROL_ADVANCED:
          if (draw)
            Draw_Menu_Item(row, ICON_Version, GET_TEXT_F(MSG_ADVANCED_SETTINGS), nullptr, true);
          else
            Draw_Menu(Advanced);
          break;
        #if ENABLED(EEPROM_SETTINGS)
          case CONTROL_SAVE:
            if (draw)
              Draw_Menu_Item(row, ICON_WriteEEPROM, GET_TEXT_F(MSG_STORE_EEPROM));
            else
              AudioFeedback(settings.save());
            break;
          case CONTROL_RESTORE:
            if (draw)
              Draw_Menu_Item(row, ICON_ReadEEPROM, GET_TEXT_F(MSG_LOAD_EEPROM));
            else
              AudioFeedback(settings.load());
            break;
          case CONTROL_RESET:
            if (draw)
              Draw_Menu_Item(row, ICON_Temperature, GET_TEXT_F(MSG_RESTORE_DEFAULTS));
            else {
              settings.reset();
              AudioFeedback();
            }
            break;
        #endif
        case CONTROL_REBOOT:
          if (draw)
            Draw_Menu_Item(row, ICON_Reboot, GET_TEXT_F(MSG_RESET_PRINTER));
          else
            RebootPrinter();
          break;
        case CONTROL_INFO:
          if (draw)
            Draw_Menu_Item(row, ICON_Info, GET_TEXT_F(MSG_INFO_SCREEN));
          else
            Draw_Menu(Info);
          break;
      }
      break;

    case TempMenu:

      #define TEMP_BACK 0
      #define TEMP_HOTEND (TEMP_BACK + ENABLED(HAS_HOTEND))
      #define TEMP_BED (TEMP_HOTEND + ENABLED(HAS_HEATED_BED))
      #define TEMP_FAN (TEMP_BED + ENABLED(HAS_FAN))
      #define TEMP_PID (TEMP_FAN + ANY(HAS_HOTEND, HAS_HEATED_BED))
      #define TEMP_PREHEAT1 (TEMP_PID + (PREHEAT_COUNT >= 1))
      #define TEMP_PREHEAT2 (TEMP_PREHEAT1 + (PREHEAT_COUNT >= 2))
      #define TEMP_PREHEAT3 (TEMP_PREHEAT2 + (PREHEAT_COUNT >= 3))
      #define TEMP_PREHEAT4 (TEMP_PREHEAT3 + (PREHEAT_COUNT >= 4))
      #define TEMP_PREHEAT5 (TEMP_PREHEAT4 + (PREHEAT_COUNT >= 5))
      #define TEMP_TOTAL TEMP_PREHEAT5

      switch (item) {
        case TEMP_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Control, CONTROL_TEMP);
          break;
        #if HAS_HOTEND
          case TEMP_HOTEND:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetEndTemp, F("Hotend"));
              Draw_Float(thermalManager.temp_hotend[0].target, row, false, 1);
            }
            else
              Modify_Value(thermalManager.temp_hotend[0].target, MIN_E_TEMP, MAX_E_TEMP, 1);
            break;
        #endif
        #if HAS_HEATED_BED
          case TEMP_BED:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetBedTemp, F("Bed"));
              Draw_Float(thermalManager.temp_bed.target, row, false, 1);
            }
            else
              Modify_Value(thermalManager.temp_bed.target, MIN_BED_TEMP, MAX_BED_TEMP, 1);
            break;
        #endif
        #if HAS_FAN
          case TEMP_FAN:
            if (draw) {
              Draw_Menu_Item(row, ICON_FanSpeed, GET_TEXT_F(MSG_FAN_SPEED));
              Draw_Float(thermalManager.fan_speed[0], row, false, 1);
            }
            else
              Modify_Value(thermalManager.fan_speed[0], MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
            break;
        #endif
        #if HAS_HOTEND || HAS_HEATED_BED
          case TEMP_PID:
            if (draw)
              Draw_Menu_Item(row, ICON_Step, F("PID"), nullptr, true);
            else
              Draw_Menu(PID);
            break;
        #endif

        #define _TEMP_PREHEAT_CASE(N) \
          case TEMP_PREHEAT##N: { \
            if (draw) Draw_Menu_Item(row, ICON_Step, F(PREHEAT_## N ##_LABEL), nullptr, true); \
            else Draw_Menu(Preheat##N); \
          } break;

        REPEAT_1(PREHEAT_COUNT, _TEMP_PREHEAT_CASE)
      }
      break;

    #if HAS_HOTEND || HAS_HEATED_BED
      case PID:

        #define PID_BACK 0
        #define PID_HOTEND (PID_BACK + ENABLED(HAS_HOTEND))
        #define PID_BED (PID_HOTEND + ENABLED(HAS_HEATED_BED))
        #define PID_CYCLES (PID_BED + 1)
        #define PID_TOTAL PID_CYCLES

        static uint8_t PID_cycles = 5;

        switch (item) {
          case PID_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(TempMenu, TEMP_PID);
            break;
          #if HAS_HOTEND
            case PID_HOTEND:
              if (draw)
                Draw_Menu_Item(row, ICON_HotendTemp, F(STR_HOTEND_PID), nullptr, true);
              else
                Draw_Menu(HotendPID);
              break;
          #endif
          #if HAS_HEATED_BED
            case PID_BED:
              if (draw)
                Draw_Menu_Item(row, ICON_BedTemp, F(STR_BED_PID), nullptr, true);
              else
                Draw_Menu(BedPID);
              break;
          #endif
          case PID_CYCLES:
            if (draw) {
              Draw_Menu_Item(row, ICON_FanSpeed, GET_TEXT_F(MSG_PID_CYCLE));
              Draw_Float(PID_cycles, row, false, 1);
            }
            else
              Modify_Value(PID_cycles, 3, 50, 1);
            break;
        }
        break;
    #endif // HAS_HOTEND || HAS_HEATED_BED

    #if HAS_HOTEND
      case HotendPID:

        #define HOTENDPID_BACK 0
        #define HOTENDPID_TUNE (HOTENDPID_BACK + 1)
        #define HOTENDPID_TEMP (HOTENDPID_TUNE + 1)
        #define HOTENDPID_KP (HOTENDPID_TEMP + 1)
        #define HOTENDPID_KI (HOTENDPID_KP + 1)
        #define HOTENDPID_KD (HOTENDPID_KI + 1)
        #define HOTENDPID_TOTAL HOTENDPID_KD

        switch (item) {
          case HOTENDPID_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(PID, PID_HOTEND);
            break;
          case HOTENDPID_TUNE:
            if (draw)
              Draw_Menu_Item(row, ICON_HotendTemp, GET_TEXT_F(MSG_PID_AUTOTUNE));
            else {
              Popup_Handler(PIDWait);
              sprintf_P(cmd, PSTR("M303 E0 C%i S%i U1"), PID_cycles, temp_val.PID_e_temp);
              gcode.process_subcommands_now(cmd);
              planner.synchronize();
              Redraw_Menu();
            }
            break;
          case HOTENDPID_TEMP:
            if (draw) {
              Draw_Menu_Item(row, ICON_Temperature, GET_TEXT_F(MSG_TEMPERATURE));
              Draw_Float(temp_val.PID_e_temp, row, false, 1);
            }
            else
              Modify_Value(temp_val.PID_e_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
            break;
          case HOTENDPID_KP:
            if (draw) {
              Draw_Menu_Item(row, ICON_Version, F("Kp Value"));
              Draw_Float(thermalManager.temp_hotend[0].pid.Kp, row, false, 100);
            }
            else
              Modify_Value(thermalManager.temp_hotend[0].pid.Kp, 0, 5000, 100, thermalManager.updatePID);
            break;
          case HOTENDPID_KI:
            if (draw) {
              Draw_Menu_Item(row, ICON_Version, F("Ki Value"));
              Draw_Float(unscalePID_i(thermalManager.temp_hotend[0].pid.Ki), row, false, 100);
            }
            else
              Modify_Value(thermalManager.temp_hotend[0].pid.Ki, 0, 5000, 100, thermalManager.updatePID);
            break;
          case HOTENDPID_KD:
            if (draw) {
              Draw_Menu_Item(row, ICON_Version, F("Kd Value"));
              Draw_Float(unscalePID_d(thermalManager.temp_hotend[0].pid.Kd), row, false, 100);
            }
            else
              Modify_Value(thermalManager.temp_hotend[0].pid.Kd, 0, 5000, 100, thermalManager.updatePID);
            break;
        }
        break;
    #endif // HAS_HOTEND

    #if HAS_HEATED_BED
      case BedPID:

        #define BEDPID_BACK 0
        #define BEDPID_TUNE (BEDPID_BACK + 1)
        #define BEDPID_TEMP (BEDPID_TUNE + 1)
        #define BEDPID_KP (BEDPID_TEMP + 1)
        #define BEDPID_KI (BEDPID_KP + 1)
        #define BEDPID_KD (BEDPID_KI + 1)
        #define BEDPID_TOTAL BEDPID_KD

        switch (item) {
          case BEDPID_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(PID, PID_BED);
            break;
          case BEDPID_TUNE:
            if (draw)
              Draw_Menu_Item(row, ICON_HotendTemp, GET_TEXT_F(MSG_PID_AUTOTUNE));
            else {
              Popup_Handler(PIDWait);
              sprintf_P(cmd, PSTR("M303 E-1 C%i S%i U1"), PID_cycles, temp_val.PID_bed_temp);
              gcode.process_subcommands_now(cmd);
              planner.synchronize();
              Redraw_Menu();
            }
            break;
          case BEDPID_TEMP:
            if (draw) {
              Draw_Menu_Item(row, ICON_Temperature, GET_TEXT_F(MSG_TEMPERATURE));
              Draw_Float(temp_val.PID_bed_temp, row, false, 1);
            }
            else
              Modify_Value(temp_val.PID_bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
            break;
          case BEDPID_KP:
            if (draw) {
              Draw_Menu_Item(row, ICON_Version, F("Kp Value"));
              Draw_Float(thermalManager.temp_bed.pid.Kp, row, false, 100);
            }
            else
              Modify_Value(thermalManager.temp_bed.pid.Kp, 0, 5000, 100, thermalManager.updatePID);
            break;
          case BEDPID_KI:
            if (draw) {
              Draw_Menu_Item(row, ICON_Version, F("Ki Value"));
              Draw_Float(unscalePID_i(thermalManager.temp_bed.pid.Ki), row, false, 100);
            }
            else
              Modify_Value(thermalManager.temp_bed.pid.Ki, 0, 5000, 100, thermalManager.updatePID);
            break;
          case BEDPID_KD:
            if (draw) {
              Draw_Menu_Item(row, ICON_Version, F("Kd Value"));
              Draw_Float(unscalePID_d(thermalManager.temp_bed.pid.Kd), row, false, 100);
            }
            else
              Modify_Value(thermalManager.temp_bed.pid.Kd, 0, 5000, 100, thermalManager.updatePID);
            break;
        }
        break;
    #endif // HAS_HEATED_BED

    #if HAS_PREHEAT
      #define _PREHEAT_SUBMENU_CASE(N) case Preheat##N: preheat_submenu((N) - 1, item, TEMP_PREHEAT##N); break;
      REPEAT_1(PREHEAT_COUNT, _PREHEAT_SUBMENU_CASE)
    #endif

    case Motion:

      #define MOTION_BACK 0
      #define MOTION_HOMEOFFSETS (MOTION_BACK + 1)
      #define MOTION_SPEED (MOTION_HOMEOFFSETS + 1)
      #define MOTION_ACCEL (MOTION_SPEED + 1)
      #define MOTION_JERK (MOTION_ACCEL + ENABLED(HAS_CLASSIC_JERK))
      #define MOTION_JD (MOTION_JERK + ENABLED(HAS_JUNCTION_DEVIATION))
      #define MOTION_STEPS (MOTION_JD + 1)
      #define MOTION_FLOW (MOTION_STEPS + ENABLED(HAS_HOTEND))
      #define MOTION_TOTAL MOTION_FLOW

      switch (item) {
        case MOTION_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Control, CONTROL_MOTION);
          break;
        case MOTION_HOMEOFFSETS:
          if (draw)
            Draw_Menu_Item(row, ICON_SetHome, GET_TEXT_F(MSG_SET_HOME_OFFSETS), nullptr, true);
          else
            Draw_Menu(HomeOffsets);
          break;
        case MOTION_SPEED:
          if (draw)
            Draw_Menu_Item(row, ICON_MaxSpeed, GET_TEXT_F(MSG_MAX_SPEED), nullptr, true);
          else
            Draw_Menu(MaxSpeed);
          break;
        case MOTION_ACCEL:
          if (draw)
            Draw_Menu_Item(row, ICON_MaxAccelerated, GET_TEXT_F(MSG_AMAX_EN), nullptr, true);
          else
            Draw_Menu(MaxAcceleration);
          break;
        #if HAS_CLASSIC_JERK
          case MOTION_JERK:
            if (draw)
              Draw_Menu_Item(row, ICON_MaxJerk, GET_TEXT_F(MSG_JERK), nullptr, true);
            else
              Draw_Menu(MaxJerk);
            break;
        #endif
        #if HAS_JUNCTION_DEVIATION
          case MOTION_JD:
            if (draw)
              Draw_Menu_Item(row, ICON_MaxJerk, GET_TEXT_F(MSG_JUNCTION_DEVIATION), nullptr, true);
            else
              Draw_Menu(JDmenu);
            break;
        #endif
        case MOTION_STEPS:
          if (draw)
            Draw_Menu_Item(row, ICON_Step, GET_TEXT_F(MSG_STEPS_PER_MM), nullptr, true);
          else
            Draw_Menu(Steps);
          break;
        #if HAS_HOTEND
          case MOTION_FLOW:
            if (draw) {
              Draw_Menu_Item(row, ICON_Speed, GET_TEXT_F(MSG_FLOW));
              Draw_Float(planner.flow_percentage[0], row, false, 1);
            }
            else
              Modify_Value(planner.flow_percentage[0], MIN_FLOW_RATE, MAX_FLOW_RATE, 1);
            break;
        #endif
      }
      break;

    #if ENABLED(FWRETRACT)
      case FwRetraction:

        #define FWR_BACK 0
        #define FWR_RET_AUTO (FWR_BACK + 1)
        #define FWR_RET_LENGTH (FWR_RET_AUTO + 1)
        #define FWR_RET_SPEED (FWR_RET_LENGTH + 1)
        #define FWR_ZLIFT (FWR_RET_SPEED + 1)
        #define FWR_REC_EXT_LENGTH (FWR_ZLIFT + 1)
        #define FWR_REC_SPEED (FWR_REC_EXT_LENGTH + 1)
        #define FWR_RESET (FWR_REC_SPEED + 1)
        #define FWR_TOTAL FWR_RESET

        switch (item) {

          case FWR_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else {
            if (temp_val.flag_tune) {
              temp_val.flag_tune = false;
              Redraw_Menu(false, true, true);
            }
            else
              Draw_Menu(Control, CONTROL_FWRETRACT);
          }
          break;
          case FWR_RET_AUTO:
            if (draw) {
              temp_val.auto_fw_retract = fwretract.autoretract_enabled;
              Draw_Menu_Item(row, ICON_StepE, GET_TEXT_F(MSG_AUTORETRACT));
              Draw_Checkbox(row, temp_val.auto_fw_retract);
            }
            else {
              if (MIN_AUTORETRACT <= MAX_AUTORETRACT) {
                temp_val.auto_fw_retract = !temp_val.auto_fw_retract;
                fwretract.enable_autoretract(temp_val.auto_fw_retract);
                Draw_Checkbox(row, temp_val.auto_fw_retract);
              }
            }
            break;
          case FWR_RET_LENGTH:
            if (draw) {
              Draw_Menu_Item(row, ICON_FWRetLength, GET_TEXT_F(MSG_CONTROL_RETRACT));
              Draw_Float(fwretract.settings.retract_length, row, false, 10);
            }
            else
              Modify_Value(fwretract.settings.retract_length, 0, 10, 10);
            break;
          case FWR_RET_SPEED:
            if (draw) {
              Draw_Menu_Item(row, ICON_FWRetSpeed, GET_TEXT_F(MSG_SINGLENOZZLE_RETRACT_SPEED));
              Draw_Float(fwretract.settings.retract_feedrate_mm_s, row, false, 10);
            }
            else
              Modify_Value(fwretract.settings.retract_feedrate_mm_s, 1, 90, 10);
            break;
          case FWR_ZLIFT:
            if (draw) {
              Draw_Menu_Item(row, ICON_FWRetZRaise, GET_TEXT_F(MSG_CONTROL_RETRACT_ZHOP));
              Draw_Float(fwretract.settings.retract_zraise, row, false, 100);
            }
            else
              Modify_Value(fwretract.settings.retract_zraise, 0, 10, 100);
            break;
          case FWR_REC_EXT_LENGTH:
            if (draw) {
              Draw_Menu_Item(row, ICON_FWRecExtLength, GET_TEXT_F(MSG_CONTROL_RETRACT_RECOVER));
              Draw_Float(fwretract.settings.retract_recover_extra, row, false, 10);
            }
            else
              Modify_Value(fwretract.settings.retract_recover_extra, -10, 10, 10);
            break;
          case FWR_REC_SPEED:
            if (draw) {
              Draw_Menu_Item(row, ICON_FWRecSpeed, GET_TEXT_F(MSG_SINGLENOZZLE_UNRETRACT_SPEED));
              Draw_Float(fwretract.settings.retract_recover_feedrate_mm_s, row, false, 10);
            }
            else
              Modify_Value(fwretract.settings.retract_recover_feedrate_mm_s, 1, 90, 10);
            break;
          case FWR_RESET:
            if (draw)
              Draw_Menu_Item(row, ICON_StepE, GET_TEXT_F(MSG_BUTTON_RESET));
            else {
              fwretract.reset();
              Draw_Menu(FwRetraction);
            }
            break;
        }
        break;
    #endif

    case HomeOffsets:

      #define HOMEOFFSETS_BACK 0
      #define HOMEOFFSETS_XOFFSET (HOMEOFFSETS_BACK + 1)
      #define HOMEOFFSETS_YOFFSET (HOMEOFFSETS_XOFFSET + 1)
      #define HOMEOFFSETS_TOTAL HOMEOFFSETS_YOFFSET

      switch (item) {
        case HOMEOFFSETS_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Motion, MOTION_HOMEOFFSETS);
          break;
        case HOMEOFFSETS_XOFFSET:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepX, GET_TEXT_F(MSG_HOME_OFFSET_X));
            Draw_Float(home_offset.x, row, false, 100);
          }
          else
            Modify_Value(home_offset.x, -MAX_XY_OFFSET, MAX_XY_OFFSET, 100);
          break;
        case HOMEOFFSETS_YOFFSET:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepY, GET_TEXT_F(MSG_HOME_OFFSET_Y));
            Draw_Float(home_offset.y, row, false, 100);
          }
          else
            Modify_Value(home_offset.y, -MAX_XY_OFFSET, MAX_XY_OFFSET, 100);
          break;
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
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Motion, MOTION_SPEED);
          break;
        case SPEED_X:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeedX, GET_TEXT_F(MSG_VMAX_A));
            Draw_Float(planner.settings.max_feedrate_mm_s[X_AXIS], row, false, 1);
          }
          else
            Modify_Value(planner.settings.max_feedrate_mm_s[X_AXIS], 0, default_max_feedrate[X_AXIS] * 2, 1);
          break;

        #if HAS_Y_AXIS
          case SPEED_Y:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxSpeedY, GET_TEXT_F(MSG_VMAX_B));
              Draw_Float(planner.settings.max_feedrate_mm_s[Y_AXIS], row, false, 1);
            }
            else
              Modify_Value(planner.settings.max_feedrate_mm_s[Y_AXIS], 0, default_max_feedrate[Y_AXIS] * 2, 1);
            break;
        #endif

        #if HAS_Z_AXIS
          case SPEED_Z:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxSpeedZ, GET_TEXT_F(MSG_VMAX_C));
              Draw_Float(planner.settings.max_feedrate_mm_s[Z_AXIS], row, false, 1);
            }
            else
              Modify_Value(planner.settings.max_feedrate_mm_s[Z_AXIS], 0, default_max_feedrate[Z_AXIS] * 2, 1);
            break;
        #endif

        #if HAS_HOTEND
          case SPEED_E:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxSpeedE, GET_TEXT_F(MSG_VMAX_E));
              Draw_Float(planner.settings.max_feedrate_mm_s[E_AXIS], row, false, 1);
            }
            else
              Modify_Value(planner.settings.max_feedrate_mm_s[E_AXIS], 0, default_max_feedrate[E_AXIS] * 2, 1);
            break;
        #endif
      }
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
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Motion, MOTION_ACCEL);
          break;
        case ACCEL_X:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccX, GET_TEXT_F(MSG_AMAX_A));
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[X_AXIS], row, false, 1);
          }
          else
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[X_AXIS], 0, default_max_acceleration[X_AXIS] * 2, 1);
          break;
        case ACCEL_Y:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccY, GET_TEXT_F(MSG_AMAX_B));
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], row, false, 1);
          }
          else
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], 0, default_max_acceleration[Y_AXIS] * 2, 1);
          break;
        case ACCEL_Z:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccZ, GET_TEXT_F(MSG_AMAX_C));
            Draw_Float(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], row, false, 1);
          }
          else
            Modify_Value(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], 0, default_max_acceleration[Z_AXIS] * 2, 1);
          break;
        #if HAS_HOTEND
          case ACCEL_E:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxAccE, GET_TEXT_F(MSG_AMAX_E));
              Draw_Float(planner.settings.max_acceleration_mm_per_s2[E_AXIS], row, false, 1);
            }
            else
              Modify_Value(planner.settings.max_acceleration_mm_per_s2[E_AXIS], 0, default_max_acceleration[E_AXIS] * 2, 1);
            break;
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
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Motion, MOTION_JERK);
            break;
          case JERK_X:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxSpeedJerkX,  GET_TEXT_F(MSG_VA_JERK));
              Draw_Float(planner.max_jerk[X_AXIS], row, false, 10);
            }
            else
              Modify_Value(planner.max_jerk[X_AXIS], 0, default_max_jerk[X_AXIS] * 2, 10);
            break;
          case JERK_Y:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxSpeedJerkY, GET_TEXT_F(MSG_VB_JERK));
              Draw_Float(planner.max_jerk[Y_AXIS], row, false, 10);
            }
            else
              Modify_Value(planner.max_jerk[Y_AXIS], 0, default_max_jerk[Y_AXIS] * 2, 10);
            break;
          case JERK_Z:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxSpeedJerkZ, GET_TEXT_F(MSG_VC_JERK));
              Draw_Float(planner.max_jerk[Z_AXIS], row, false, 10);
            }
            else
              Modify_Value(planner.max_jerk[Z_AXIS], 0, default_max_jerk[Z_AXIS] * 2, 10);
            break;
          #if HAS_HOTEND
            case JERK_E:
              if (draw) {
                Draw_Menu_Item(row, ICON_MaxSpeedJerkE, GET_TEXT_F(MSG_VE_JERK));
                Draw_Float(planner.max_jerk[E_AXIS], row, false, 10);
              }
              else
                Modify_Value(planner.max_jerk[E_AXIS], 0, default_max_jerk[E_AXIS] * 2, 10);
              break;
          #endif
        }
        break;
    #endif
    #if HAS_JUNCTION_DEVIATION
      case JDmenu:

        #define JD_BACK 0
        #define JD_SETTING_JD_MM (JD_BACK + ENABLED(HAS_HOTEND))
        #define JD_TOTAL JD_SETTING_JD_MM

        switch (item) {
          case JD_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Motion, MOTION_JD);
            break;
          #if HAS_HOTEND
            case JD_SETTING_JD_MM:
              if (draw) {
                Draw_Menu_Item(row, ICON_MaxJerk, GET_TEXT_F(MSG_JUNCTION_DEVIATION));
                Draw_Float(planner.junction_deviation_mm, row, false, 100);
              }
              else
                Modify_Value(planner.junction_deviation_mm, MIN_JD_MM, MAX_JD_MM, 100);
              break;
            #endif
        }
        break;
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
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Motion, MOTION_STEPS);
          break;
        case STEPS_X:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepX, GET_TEXT_F(MSG_A_STEPS));
            Draw_Float(planner.settings.axis_steps_per_mm[X_AXIS], row, false, 10);
          }
          else
            Modify_Value(planner.settings.axis_steps_per_mm[X_AXIS], 0, default_steps[X_AXIS] * 2, 10);
          break;
        case STEPS_Y:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepY, GET_TEXT_F(MSG_B_STEPS));
            Draw_Float(planner.settings.axis_steps_per_mm[Y_AXIS], row, false, 10);
          }
          else
            Modify_Value(planner.settings.axis_steps_per_mm[Y_AXIS], 0, default_steps[Y_AXIS] * 2, 10);
          break;
        case STEPS_Z:
          if (draw) {
            Draw_Menu_Item(row, ICON_StepZ, GET_TEXT_F(MSG_C_STEPS));
            Draw_Float(planner.settings.axis_steps_per_mm[Z_AXIS], row, false, 10);
          }
          else
            Modify_Value(planner.settings.axis_steps_per_mm[Z_AXIS], 0, default_steps[Z_AXIS] * 2, 10);
          break;
        #if HAS_HOTEND
          case STEPS_E:
            if (draw) {
              Draw_Menu_Item(row, ICON_StepE, GET_TEXT_F(MSG_E_STEPS));
              Draw_Float(planner.settings.axis_steps_per_mm[E_AXIS], row, false, 10);
            }
            else
              Modify_Value(planner.settings.axis_steps_per_mm[E_AXIS], 0, 1000, 10);
            break;
        #endif
      }
      break;

    #if ANY(CASE_LIGHT_MENU, LED_CONTROL_MENU)
      case Ledsmenu:

        #define LEDS_BACK 0
        #define LEDS_CASELIGHT (LEDS_BACK + ENABLED(CASE_LIGHT_MENU))
        #define LEDS_LED_CONTROL_MENU (LEDS_CASELIGHT + ENABLED(LED_CONTROL_MENU))
        #define LEDS_TOTAL LEDS_LED_CONTROL_MENU

        switch (item) {

        case LEDS_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Control, CONTROL_LEDS);
          break;
        #if ENABLED(CASE_LIGHT_MENU)
          case LEDS_CASELIGHT:
            if (draw) {
              #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
                Draw_Menu_Item(row, ICON_CaseLight, GET_TEXT_F(MSG_CASE_LIGHT), nullptr, true);
              #else
                Draw_Menu_Item(row, ICON_CaseLight, GET_TEXT_F(MSG_CASE_LIGHT));
                Draw_Checkbox(row, caselight.on);
              #endif
            }
            else {
              #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
                Draw_Menu(CaseLightmenu);
              #else
                caselight.on = !caselight.on;
                caselight.update_enabled();
                Draw_Checkbox(row, caselight.on);
                DWIN_UpdateLCD();
              #endif
            }
            break;
        #endif
        #if ENABLED(LED_CONTROL_MENU)
          case LEDS_LED_CONTROL_MENU:
            if (draw)
              Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_LED_CONTROL), nullptr, true);
            else
              Draw_Menu(LedControlmenu);
            break;
        #endif
        }
      break;
    #endif

    #if ENABLED(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
      case CaseLightmenu:

        #define CASE_LIGHT_BACK 0
        #define CASE_LIGHT_ON (CASE_LIGHT_BACK + 1)
        #define CASE_LIGHT_USES_BRIGHT (CASE_LIGHT_ON + 1)
        #define CASE_LIGHT_TOTAL CASE_LIGHT_USES_BRIGHT

        switch (item) {

          case CASE_LIGHT_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Ledsmenu, LEDS_CASELIGHT);
            break;
          case CASE_LIGHT_ON:
            if (draw) {
              Draw_Menu_Item(row, ICON_CaseLight, GET_TEXT_F(MSG_CASE_LIGHT));
              Draw_Checkbox(row, caselight.on);
            }
            else {
              caselight.on = !caselight.on;
              caselight.update_enabled();
              Draw_Checkbox(row, caselight.on);
              DWIN_UpdateLCD();
            }
            break;
          case CASE_LIGHT_USES_BRIGHT:
            if (draw) {
              Draw_Menu_Item(row, ICON_Brightness, GET_TEXT_F(MSG_CASE_LIGHT_BRIGHTNESS));
              Draw_Float(caselight.brightness, row);
            }
            else
              Modify_Value(caselight.brightness, 0, 255, 1);
            break;
        }
      break;
    #endif

    #if ENABLED(LED_CONTROL_MENU)
      case LedControlmenu:

        #define LEDCONTROL_BACK 0
        #define LEDCONTROL_LIGHTON (LEDCONTROL_BACK + !BOTH(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL))
        #define LEDCONTROL_PRESETS_MENU (LEDCONTROL_LIGHTON + BOTH(HAS_COLOR_LEDS, LED_COLOR_PRESETS))
        #define LEDCONTROL_CUSTOM_MENU (LEDCONTROL_PRESETS_MENU + ENABLED(HAS_COLOR_LEDS) - DISABLED(LED_COLOR_PRESETS))
        #define LEDCONTROL_TOTAL LEDCONTROL_CUSTOM_MENU

        switch (item) {
          case LEDCONTROL_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Ledsmenu, LEDS_LED_CONTROL_MENU);
            break;
          #if !BOTH(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
            case LEDCONTROL_LIGHTON:
              if (draw) {
                Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_LEDS));
                Draw_Checkbox(row, leds.lights_on);
              }
              else {
                leds.toggle();
                Draw_Checkbox(row, leds.lights_on);
                DWIN_UpdateLCD();
              }
              break;
          #endif
          #if HAS_COLOR_LEDS
            #if ENABLED(LED_COLOR_PRESETS)
              case LEDCONTROL_PRESETS_MENU:
                  if (draw)
                    Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_LED_PRESETS));
                  else
                    Draw_Menu(LedControlpresets);
                break;
            #else
              case LEDCONTROL_CUSTOM_MENU:
                if (draw)
                  Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_CUSTOM_LEDS));
                else
                  Draw_Menu(LedControlcustom);
                break;
            #endif
          #endif
        }
        break;

        #if HAS_COLOR_LEDS
          #if ENABLED(LED_COLOR_PRESETS)
            case LedControlpresets:

              #define LEDCONTROL_PRESETS_BACK 0
              #define LEDCONTROL_PRESETS_WHITE (LEDCONTROL_PRESETS_BACK + 1)
              #define LEDCONTROL_PRESETS_RED (LEDCONTROL_PRESETS_WHITE + 1)
              #define LEDCONTROL_PRESETS_ORANGE (LEDCONTROL_PRESETS_RED + 1)
              #define LEDCONTROL_PRESETS_YELLOW (LEDCONTROL_PRESETS_ORANGE + 1)
              #define LEDCONTROL_PRESETS_GREEN (LEDCONTROL_PRESETS_YELLOW + 1)
              #define LEDCONTROL_PRESETS_BLUE (LEDCONTROL_PRESETS_GREEN + 1)
              #define LEDCONTROL_PRESETS_INDIGO (LEDCONTROL_PRESETS_BLUE + 1)
              #define LEDCONTROL_PRESETS_VIOLET (LEDCONTROL_PRESETS_INDIGO + 1)
              #define LEDCONTROL_PRESETS_TOTAL LEDCONTROL_PRESETS_VIOLET

              #define LEDCOLORITEM(MSG,FUNC) if (draw) Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG)); else FUNC; break;

              switch (item) {
                case LEDCONTROL_PRESETS_BACK:
                  if (draw)
                    Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
                  else
                    Draw_Menu(LedControlmenu, LEDCONTROL_PRESETS_MENU);
                  break;
                case LEDCONTROL_PRESETS_WHITE:  LEDCOLORITEM(MSG_SET_LEDS_WHITE, leds.set_white());
                case LEDCONTROL_PRESETS_RED:    LEDCOLORITEM(MSG_SET_LEDS_RED, leds.set_red());
                case LEDCONTROL_PRESETS_ORANGE: LEDCOLORITEM(MSG_SET_LEDS_ORANGE, leds.set_orange());
                case LEDCONTROL_PRESETS_YELLOW: LEDCOLORITEM(MSG_SET_LEDS_YELLOW, leds.set_yellow());
                case LEDCONTROL_PRESETS_GREEN:  LEDCOLORITEM(MSG_SET_LEDS_GREEN, leds.set_green());
                case LEDCONTROL_PRESETS_BLUE:   LEDCOLORITEM(MSG_SET_LEDS_BLUE, leds.set_blue());
                case LEDCONTROL_PRESETS_INDIGO: LEDCOLORITEM(MSG_SET_LEDS_INDIGO, leds.set_indigo());
                case LEDCONTROL_PRESETS_VIOLET: LEDCOLORITEM(MSG_SET_LEDS_VIOLET, leds.set_violet());
              }
              break;
          #else
            case LedControlcustom:

              #define LEDCONTROL_CUSTOM_BACK 0
              #define LEDCONTROL_CUSTOM_RED (LEDCONTROL_CUSTOM_BACK + 1)
              #define LEDCONTROL_CUSTOM_GREEN (LEDCONTROL_CUSTOM_RED + 1)
              #define LEDCONTROL_CUSTOM_BLUE (LEDCONTROL_CUSTOM_GREEN + 1)
              #define LEDCONTROL_CUSTOM_WHITE (LEDCONTROL_CUSTOM_BLUE + ENABLED(HAS_WHITE_LED))
              #define LEDCONTROL_CUSTOM_TOTAL LEDCONTROL_CUSTOM_WHITE

              switch (item) {
                case LEDCONTROL_PRESETS_BACK:
                  Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
                  else
                    Draw_Menu(LedControlmenu, LEDCONTROL_CUSTOM_MENU);
                  break;
                case LEDCONTROL_CUSTOM_RED:
                  if (draw) {
                    Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_INTENSITY_R));
                    Draw_Float(leds.color.r, row);
                  }
                  else
                    Modify_Value(leds.color.r, 0, 255, 1);
                  break;
                case LEDCONTROL_CUSTOM_GREEN:
                  if (draw) {
                    Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_INTENSITY_G));
                    Draw_Float(leds.color.g, row);
                  }
                  else
                    Modify_Value(leds.color.g, 0, 255, 1);
                  break;
                case LEDCONTROL_CUSTOM_BLUE:
                  if (draw) {
                    Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_INTENSITY_B));
                    Draw_Float(leds.color.b, row);
                  }
                  else
                    Modify_Value(leds.color.b, 0, 255, 1);
                  break;
                #if HAS_WHITE_LED
                  case case LEDCONTROL_CUSTOM_WHITE:
                    if (draw) {
                      Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_INTENSITY_W));
                      Draw_Float(leds.color.w, row);
                    }
                    else
                      Modify_Value(leds.color.w, 0, 255, 1);
                    break;
                #endif
              }
              break;
          #endif
        #endif
    #endif

    case Visual:

      #define VISUAL_BACK 0
      #define VISUAL_BACKLIGHT (VISUAL_BACK + 1)
      #define VISUAL_BRIGHTNESS (VISUAL_BACKLIGHT + 1)
      #define VISUAL_TIME_FORMAT (VISUAL_BRIGHTNESS + 1)
      #define VISUAL_COLOR_THEMES (VISUAL_TIME_FORMAT + 1)
      #define VISUAL_TOTAL VISUAL_COLOR_THEMES

      switch (item) {
        case VISUAL_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Control, CONTROL_VISUAL);
          break;
        case VISUAL_BACKLIGHT:
          if (draw)
            Draw_Menu_Item(row, ICON_Brightness, GET_TEXT_F(MSG_BRIGHTNESS_OFF));
          else
            ui.set_brightness(0);
          break;
        case VISUAL_BRIGHTNESS:
          if (draw) {
            Draw_Menu_Item(row, ICON_Brightness,  GET_TEXT_F(MSG_BRIGHTNESS));
            Draw_Float(ui.brightness, row, false, 1);
          }
          else
            Modify_Value(ui.brightness, LCD_BRIGHTNESS_MIN, LCD_BRIGHTNESS_MAX, 1, ui.refresh_brightness);
          break;
        case VISUAL_TIME_FORMAT:
          if (draw) {
            Draw_Menu_Item(row, ICON_PrintTime, F("Progress as __h__m"));
            Draw_Checkbox(row, eeprom_settings.time_format_textual);
          }
          else {
            eeprom_settings.time_format_textual = !eeprom_settings.time_format_textual;
            Draw_Checkbox(row, eeprom_settings.time_format_textual);
          }
          break;
        case VISUAL_COLOR_THEMES:
          if (draw)
            Draw_Menu_Item(row, ICON_MaxSpeed, GET_TEXT_F(MSG_COLORS_SELECT), nullptr, true);
          else
            Draw_Menu(ColorSettings);
        break;
      }
      break;

    case ColorSettings:

      #define COLORSETTINGS_BACK 0
      #define COLORSETTINGS_CURSOR (COLORSETTINGS_BACK + 1)
      #define COLORSETTINGS_SPLIT_LINE (COLORSETTINGS_CURSOR + 1)
      #define COLORSETTINGS_MENU_TOP_TXT (COLORSETTINGS_SPLIT_LINE + 1)
      #define COLORSETTINGS_MENU_TOP_BG (COLORSETTINGS_MENU_TOP_TXT + 1)
      #define COLORSETTINGS_HIGHLIGHT_BORDER (COLORSETTINGS_MENU_TOP_BG + 1)
      #define COLORSETTINGS_PROGRESS_PERCENT (COLORSETTINGS_HIGHLIGHT_BORDER + 1)
      #define COLORSETTINGS_PROGRESS_TIME (COLORSETTINGS_PROGRESS_PERCENT + 1)
      #define COLORSETTINGS_PROGRESS_STATUS_BAR (COLORSETTINGS_PROGRESS_TIME + 1)
      #define COLORSETTINGS_PROGRESS_STATUS_AREA (COLORSETTINGS_PROGRESS_STATUS_BAR + 1)
      #define COLORSETTINGS_PROGRESS_COORDINATES (COLORSETTINGS_PROGRESS_STATUS_AREA + 1)
      #define COLORSETTINGS_PROGRESS_COORDINATES_LINE (COLORSETTINGS_PROGRESS_COORDINATES + 1)
      #define COLORSETTINGS_TOTAL COLORSETTINGS_PROGRESS_COORDINATES_LINE

      switch (item) {
        case COLORSETTINGS_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Visual, VISUAL_COLOR_THEMES);
          break;
        case COLORSETTINGS_CURSOR:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Cursor"));
            Draw_Option(eeprom_settings.cursor_color, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.cursor_color, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_SPLIT_LINE:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Menu Split Line"));
            Draw_Option(eeprom_settings.menu_split_line, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.menu_split_line, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_MENU_TOP_TXT:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Menu Header Text"));
            Draw_Option(eeprom_settings.menu_top_txt, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.menu_top_txt, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_MENU_TOP_BG:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Menu Header Bg"));
            Draw_Option(eeprom_settings.menu_top_bg, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.menu_top_bg, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_HIGHLIGHT_BORDER:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Highlight Box"));
            Draw_Option(eeprom_settings.highlight_box, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.highlight_box, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_PROGRESS_PERCENT:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Progress Percent"));
            Draw_Option(eeprom_settings.progress_percent, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.progress_percent, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_PROGRESS_TIME:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Progress Time"));
            Draw_Option(eeprom_settings.progress_time, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.progress_time, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_PROGRESS_STATUS_BAR:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Status Bar Text"));
            Draw_Option(eeprom_settings.status_bar_text, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.status_bar_text, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_PROGRESS_STATUS_AREA:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Status Area Text"));
            Draw_Option(eeprom_settings.status_area_text, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.status_area_text, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_PROGRESS_COORDINATES:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Coordinates Text"));
            Draw_Option(eeprom_settings.coordinates_text, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.coordinates_text, color_names, Custom_Colors);
          break;
        case COLORSETTINGS_PROGRESS_COORDINATES_LINE:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxSpeed, F("Coordinates Line"));
            Draw_Option(eeprom_settings.coordinates_split_line, color_names, row, false, true);
          }
          else
            Modify_Option(eeprom_settings.coordinates_split_line, color_names, Custom_Colors);
          break;
      } // switch (item)
      break;

    #if HAS_HOSTACTION_MENUS
      case HostSettings:

        #define HOSTSETTINGS_BACK 0
        #define HOSTSETTINGS_ACTIONCOMMANDS (HOSTSETTINGS_BACK + 1)
        #define HOSTSETTINGS_TOTAL HOSTSETTINGS_ACTIONCOMMANDS

        switch (item) {
          case HOSTSETTINGS_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Control, CONTROL_HOSTSETTINGS);
            break;
            case HOSTSETTINGS_ACTIONCOMMANDS:
              if (draw) Draw_Menu_Item(row, ICON_File, F("Host Actions"));
              else Draw_Menu(ActionCommands);
              break;
        }
        break;

      case ActionCommands:

        #define ACTIONCOMMANDS_BACK 0
        #define ACTIONCOMMANDS_1 (ACTIONCOMMANDS_BACK + 1)
        #define ACTIONCOMMANDS_2 (ACTIONCOMMANDS_1 + 1)
        #define ACTIONCOMMANDS_3 (ACTIONCOMMANDS_2 + 1)
        #define ACTIONCOMMANDS_TOTAL ACTIONCOMMANDS_3

        switch (item) {
          case ACTIONCOMMANDS_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(HostSettings, HOSTSETTINGS_ACTIONCOMMANDS);
            break;
          case ACTIONCOMMANDS_1:
            if (draw) {
              Draw_Menu_Item(row, ICON_File, F("Action #1"));
              Draw_String(action1, row);
            }
            else
              Modify_String(action1, 8, true);
            break;
          case ACTIONCOMMANDS_2:
            if (draw) {
              Draw_Menu_Item(row, ICON_File, F("Action #2"));
              Draw_String(action2, row);
            }
            else
              Modify_String(action2, 8, true);
            break;
          case ACTIONCOMMANDS_3:
            if (draw) {
              Draw_Menu_Item(row, ICON_File, F("Action #3"));
              Draw_String(action3, row);
            }
            else
              Modify_String(action3, 8, true);
            break;
        }
        break;
    #endif

    case Advanced:

      #define ADVANCED_BACK 0
      #define ADVANCED_BEEPER (ADVANCED_BACK + ENABLED(SOUND_MENU_ITEM))
      #define ADVANCED_PROBE (ADVANCED_BEEPER + ENABLED(HAS_BED_PROBE))
      #define ADVANCED_CORNER (ADVANCED_PROBE + 1)
      #define ADVANCED_LA (ADVANCED_CORNER + ENABLED(LIN_ADVANCE))
      #define ADVANCED_LOAD (ADVANCED_LA + ENABLED(ADVANCED_PAUSE_FEATURE))
      #define ADVANCED_UNLOAD (ADVANCED_LOAD + ENABLED(ADVANCED_PAUSE_FEATURE))
      #define ADVANCED_COLD_EXTRUDE  (ADVANCED_UNLOAD + ENABLED(PREVENT_COLD_EXTRUSION))
      #define ADVANCED_FILSENSORENABLED (ADVANCED_COLD_EXTRUDE + ENABLED(FILAMENT_RUNOUT_SENSOR))
      #define ADVANCED_FILSENSORDISTANCE (ADVANCED_FILSENSORENABLED + ENABLED(HAS_FILAMENT_RUNOUT_DISTANCE))
      #define ADVANCED_POWER_LOSS (ADVANCED_FILSENSORDISTANCE + ENABLED(POWER_LOSS_RECOVERY))
      #define ADVANCED_BAUDRATE_MODE (ADVANCED_POWER_LOSS + ENABLED(BAUD_RATE_GCODE))
      #define ADVANCED_ESDIAG (ADVANCED_BAUDRATE_MODE + ENABLED(HAS_ESDIAG))
      #define ADVANCED_LOCKSCREEN (ADVANCED_ESDIAG + ENABLED(HAS_LOCKSCREEN))
      #define ADVANCED_TOTAL ADVANCED_LOCKSCREEN

      switch (item) {
        case ADVANCED_BACK:
          if (draw)
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else
            Draw_Menu(Control, CONTROL_ADVANCED);
          break;

        #if ENABLED(SOUND_MENU_ITEM)
          case ADVANCED_BEEPER:
            if (draw) {
              Draw_Menu_Item(row, ICON_Sound, GET_TEXT_F(MSG_SOUND_ENABLE));
              Draw_Checkbox(row, ui.sound_on);
            }
            else {
              ui.sound_on = !ui.sound_on;
              Draw_Checkbox(row, ui.sound_on);
            }
            break;
        #endif

        #if HAS_BED_PROBE
          case ADVANCED_PROBE:
            if (draw)
              Draw_Menu_Item(row, ICON_ProbeSet, GET_TEXT_F(MSG_ZPROBE_SETTINGS), nullptr, true);
            else
              Draw_Menu(ProbeMenu);
            break;
        #endif

        case ADVANCED_CORNER:
          if (draw) {
            Draw_Menu_Item(row, ICON_MaxAccelerated, F("Bed Screw Inset"));
            Draw_Float(temp_val.corner_pos, row, false, 10);
          }
          else
            Modify_Value(temp_val.corner_pos, 1, 100, 10);
          break;

        #if ENABLED(LIN_ADVANCE)
          case ADVANCED_LA:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxAccelerated, GET_TEXT_F(MSG_ADVANCE_K_E));
              Draw_Float(planner.extruder_advance_K[0], row, false, 100);
            }
            else
              Modify_Value(planner.extruder_advance_K[0], 0, 10, 100);
            break;
        #endif

        #if ENABLED(ADVANCED_PAUSE_FEATURE)
          case ADVANCED_LOAD:
            if (draw) {
              Draw_Menu_Item(row, ICON_WriteEEPROM, GET_TEXT_F(MSG_FILAMENT_LOAD));
              Draw_Float(fc_settings[0].load_length, row, false, 1);
            }
            else
              Modify_Value(fc_settings[0].load_length, 0, EXTRUDE_MAXLENGTH, 1);
            break;
          case ADVANCED_UNLOAD:
            if (draw) {
              Draw_Menu_Item(row, ICON_ReadEEPROM, GET_TEXT_F(MSG_FILAMENT_UNLOAD));
              Draw_Float(fc_settings[0].unload_length, row, false, 1);
            }
            else
              Modify_Value(fc_settings[0].unload_length, 0, EXTRUDE_MAXLENGTH, 1);
            break;
        #endif // ADVANCED_PAUSE_FEATURE

        #if ENABLED(PREVENT_COLD_EXTRUSION)
          case ADVANCED_COLD_EXTRUDE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Cool, F("Min Extrusion T"));
              Draw_Float(thermalManager.extrude_min_temp, row, false, 1);
            }
            else {
              Modify_Value(thermalManager.extrude_min_temp, 0, MAX_E_TEMP, 1);
              thermalManager.allow_cold_extrude = (thermalManager.extrude_min_temp == 0);
            }
            break;
        #endif

        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          case ADVANCED_FILSENSORENABLED:
            if (draw) {
              Draw_Menu_Item(row, ICON_Extruder, GET_TEXT_F(MSG_RUNOUT_ENABLE));
              Draw_Checkbox(row, runout.enabled);
            }
            else {
              runout.enabled = !runout.enabled;
              Draw_Checkbox(row, runout.enabled);
            }
            break;

          #if ENABLED(HAS_FILAMENT_RUNOUT_DISTANCE)
            case ADVANCED_FILSENSORDISTANCE:
              if (draw) {
                Draw_Menu_Item(row, ICON_MaxAccE, GET_TEXT_F(MSG_RUNOUT_DISTANCE_MM));
                Draw_Float(runout.runout_distance(), row, false, 10);
              }
              else
                Modify_Value(runout.runout_distance(), 0, 999, 10);
              break;
          #endif
        #endif // FILAMENT_RUNOUT_SENSOR

        #if ENABLED(POWER_LOSS_RECOVERY)
          case ADVANCED_POWER_LOSS:
            if (draw) {
              Draw_Menu_Item(row, ICON_Motion, GET_TEXT_F(MSG_OUTAGE_RECOVERY));
              Draw_Checkbox(row, recovery.enabled);
            }
            else {
              recovery.enable(!recovery.enabled);
              Draw_Checkbox(row, recovery.enabled);
            }
            break;
        #endif
        #if ENABLED(BAUD_RATE_GCODE)
          case ADVANCED_BAUDRATE_MODE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Setspeed, F("115k Baud"));
              Draw_Checkbox(row, eeprom_settings.Baud115k);
            }
            else {
              eeprom_settings.Baud115k = !eeprom_settings.Baud115k;
              queue.inject(eeprom_settings.Baud115k ? F("M575 P0 B115200") : F("M575 P0 B250000"));
            }
            break;
        #endif
        #if HAS_ESDIAG
          case ADVANCED_ESDIAG:
            if (draw)
              Draw_Menu_Item(row, ICON_ESDiag, F("End-stops diagnostic"));
            else
              DWIN_EndstopsDiag();
            break;
        #endif
        #if HAS_LOCKSCREEN
          case ADVANCED_LOCKSCREEN:
            if (draw)
              Draw_Menu_Item(row, ICON_Lock, GET_TEXT_F(MSG_LOCKSCREEN));
            else
              DWIN_LockScreen();
            break;
        #endif
      }
      break;

    #if HAS_BED_PROBE
      case ProbeMenu:

        #define PROBE_BACK 0
        #define PROBE_XOFFSET (PROBE_BACK + 1)
        #define PROBE_YOFFSET (PROBE_XOFFSET + 1)
        #define PROBE_ZOFFSET (PROBE_YOFFSET + 1)
        #define PROBE_HSMODE (PROBE_ZOFFSET + ENABLED(BLTOUCH))
        #define PROBE_ALARMR (PROBE_HSMODE + ENABLED(BLTOUCH))
        #define PROBE_SELFTEST (PROBE_ALARMR + ENABLED(BLTOUCH))
        #define PROBE_MOVEP (PROBE_SELFTEST + ENABLED(BLTOUCH))
        #define PROBE_TEST (PROBE_MOVEP + 1)
        #define PROBE_TEST_COUNT (PROBE_TEST + 1)
        #define PROBE_TOTAL PROBE_TEST_COUNT

        static uint8_t testcount = 4;

        switch (item) {
          case PROBE_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Advanced, ADVANCED_PROBE);
            break;
          case PROBE_XOFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_StepX, GET_TEXT_F(MSG_ZPROBE_XOFFSET));
              Draw_Float(probe.offset.x, row, false, 10);
            }
            else
              Modify_Value(probe.offset.x, -MAX_XY_OFFSET, MAX_XY_OFFSET, 10);
            break;
          case PROBE_YOFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_StepY, GET_TEXT_F(MSG_ZPROBE_YOFFSET));
              Draw_Float(probe.offset.y, row, false, 10);
            }
            else
              Modify_Value(probe.offset.y, -MAX_XY_OFFSET, MAX_XY_OFFSET, 10);
            break;
          case PROBE_ZOFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_StepZ, GET_TEXT_F(MSG_ZPROBE_ZOFFSET));
              Draw_Float(probe.offset.z, row, false, 100);
            }
            else
              Modify_Value(probe.offset.z, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
            break;
          #if ENABLED(BLTOUCH)
            case PROBE_HSMODE:
              if (draw) {
                Draw_Menu_Item(row, ICON_HSMode, GET_TEXT(MSG_BLTOUCH_SPEED_MODE));
                Draw_Checkbox(row, bltouch.high_speed_mode);
              }
              else {
                bltouch.high_speed_mode = !bltouch.high_speed_mode;
                Draw_Checkbox(row, bltouch.high_speed_mode);
              }
              break;
            case PROBE_ALARMR:
              if (draw)
                Draw_Menu_Item(row, ICON_ProbeAlarm, GET_TEXT_F(MSG_BLTOUCH_RESET));
              else {
                gcode.process_subcommands_now(F("M280 P0 S160"));
                AudioFeedback();
              }
              break;
            case PROBE_SELFTEST:
              if (draw)
                Draw_Menu_Item(row, ICON_ProbeSelfTest, GET_TEXT_F(MSG_BLTOUCH_SELFTEST));
              else {
                gcode.process_subcommands_now(F("M280 P0 S120\nG4 P1000\nM280 P0 S160"));
                planner.synchronize();
                AudioFeedback();
              }
              break;
            case PROBE_MOVEP:
              if (draw) {
                Draw_Menu_Item(row, ICON_ProbeDeploy, GET_TEXT_F(MSG_BLTOUCH_DEPLOY));
                Draw_Checkbox(row, temp_val.probe_deployed);
              }
              else {
                temp_val.probe_deployed = !temp_val.probe_deployed;
                if (temp_val.probe_deployed == true)  gcode.process_subcommands_now(F("M280 P0 S10"));
                else  gcode.process_subcommands_now(F("M280 P0 S90"));
                Draw_Checkbox(row, temp_val.probe_deployed);
              }
              break;
          #endif
          case PROBE_TEST:
            if (draw)
              Draw_Menu_Item(row, ICON_ProbeTest, GET_TEXT_F(MSG_M48_TEST));
            else {
              sprintf_P(cmd, PSTR("G28O\nM48 X%s Y%s P%i"), dtostrf((X_BED_SIZE + X_MIN_POS) / 2.0f, 1, 3, str_1), dtostrf((Y_BED_SIZE + Y_MIN_POS) / 2.0f, 1, 3, str_2), testcount);
              gcode.process_subcommands_now(cmd);
            }
            break;
          case PROBE_TEST_COUNT:
            if (draw) {
              Draw_Menu_Item(row, ICON_ProbeTestCount, F("Probe Test Count"));
              Draw_Float(testcount, row, false, 1);
            }
            else
              Modify_Value(testcount, 4, 50, 1);
            break;
        }
        break;
    #endif

    case InfoMain:
    case Info:

      #define INFO_BACK 0
      #define INFO_PRINTCOUNT (INFO_BACK + ENABLED(PRINTCOUNTER))
      #define INFO_PRINTTIME (INFO_PRINTCOUNT + ENABLED(PRINTCOUNTER))
      #define INFO_SIZE (INFO_PRINTTIME + 1)
      #define INFO_VERSION (INFO_SIZE + 1)
      #define INFO_CONTACT (INFO_VERSION + 1)
      #define INFO_TOTAL INFO_BACK

      switch (item) {
        case INFO_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));

            #if ENABLED(PRINTCOUNTER)
              char row1[50], row2[50], buf[32];
              printStatistics ps = print_job_timer.getStats();

              sprintf_P(row1, PSTR("%i prints, %i finished"), ps.totalPrints, ps.finishedPrints);
              sprintf_P(row2, PSTR("%s m filament used"), dtostrf(ps.filamentUsed / 1000, 1, 2, str_1));
              Draw_Menu_Item(INFO_PRINTCOUNT, ICON_HotendTemp, row1, row2, false, true);

              duration_t(print_job_timer.getStats().printTime).toString(buf);
              sprintf_P(row1, PSTR("Printed: %s"), buf);
              duration_t(print_job_timer.getStats().longestPrint).toString(buf);
              sprintf_P(row2, PSTR("Longest: %s"), buf);
              Draw_Menu_Item(INFO_PRINTTIME, ICON_PrintTime, row1, row2, false, true);
            #endif

            Draw_Menu_Item(INFO_SIZE, ICON_PrintSize, F(MACHINE_SIZE), nullptr, false, true);
            Draw_Menu_Item(INFO_VERSION, ICON_Version, F(SHORT_BUILD_VERSION), nullptr, false, true);
            Draw_Menu_Item(INFO_CONTACT, ICON_Contact, F(CORP_WEBSITE), nullptr, false, true);
          }
          else {
            if (menu == Info)
              Draw_Menu(Control, CONTROL_INFO);
            else
              Draw_Main_Menu(3);
          }
          break;
      }
      break;

    #if HAS_MESH
      case Leveling:

        #define LEVELING_BACK 0
        #define LEVELING_ACTIVE (LEVELING_BACK + 1)
        #define LEVELING_GET_TILT (LEVELING_ACTIVE + BOTH(HAS_BED_PROBE, AUTO_BED_LEVELING_UBL))
        #define LEVELING_GET_MESH (LEVELING_GET_TILT + 1)
        #define LEVELING_MANUAL (LEVELING_GET_MESH + 1)
        #define LEVELING_VIEW (LEVELING_MANUAL + 1)
        #define LEVELING_SETTINGS (LEVELING_VIEW + 1)
        #define LEVELING_SLOT (LEVELING_SETTINGS + ENABLED(AUTO_BED_LEVELING_UBL))
        #define LEVELING_LOAD (LEVELING_SLOT + ENABLED(AUTO_BED_LEVELING_UBL))
        #define LEVELING_SAVE (LEVELING_LOAD + ENABLED(AUTO_BED_LEVELING_UBL))
        #define LEVELING_TOTAL LEVELING_SAVE

        switch (item) {
          case LEVELING_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Main_Menu(3);
            break;
          case LEVELING_ACTIVE:
            if (draw) {
              Draw_Menu_Item(row, ICON_MeshActive, GET_TEXT_F(MSG_MESH_LEVELING));
              Draw_Checkbox(row, planner.leveling_active);
            }
            else {
              if (!planner.leveling_active) {
                set_bed_leveling_enabled(!planner.leveling_active);
                if (!planner.leveling_active) { Confirm_Handler(LevelError); break; }
              }
              else
                set_bed_leveling_enabled(!planner.leveling_active);
              Draw_Checkbox(row, planner.leveling_active);
            }
            break;
          #if BOTH(HAS_BED_PROBE, AUTO_BED_LEVELING_UBL)
            case LEVELING_GET_TILT:
              if (draw)
                Draw_Menu_Item(row, ICON_Tilt, F("Autotilt Current Mesh"));
              else {
                if (bedlevel.storage_slot < 0) { Popup_Handler(MeshSlot); break; }
                PreheatBefore();
                Popup_Handler(Home);
                gcode.home_all_axes(true);
                Popup_Handler(Level);
                if (mesh_conf.tilt_grid > 1) {
                  sprintf_P(cmd, PSTR("G29 J%i"), mesh_conf.tilt_grid);
                  gcode.process_subcommands_now(cmd);
                }
                else
                  gcode.process_subcommands_now(F("G29 J"));
                planner.synchronize();
                Redraw_Menu();
              }
              break;
          #endif
          case LEVELING_GET_MESH:
            if (draw)
              Draw_Menu_Item(row, ICON_Mesh, GET_TEXT_F(MSG_UBL_BUILD_MESH_MENU));
            else {
              #if ENABLED(AUTO_BED_LEVELING_UBL)
                if (bedlevel.storage_slot < 0) { Popup_Handler(MeshSlot, true); break; }
              #endif
              PreheatBefore();
              Popup_Handler(Home);
              gcode.home_all_axes(true);
              #if ENABLED(AUTO_BED_LEVELING_UBL)
                #if HAS_BED_PROBE
                  Popup_Handler(Level);
                  gcode.process_subcommands_now(F("G29 P1"));
                  gcode.process_subcommands_now(F("G29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nM420 S1"));
                  planner.synchronize();
                  Update_Status(GET_TEXT_F(MSG_MESH_DONE));
                  Popup_Handler(SaveLevel);
                #else
                  level_state = planner.leveling_active;
                  set_bed_leveling_enabled(false);
                  mesh_conf.goto_mesh_value = true;
                  mesh_conf.mesh_x = mesh_conf.mesh_y = 0;
                  Popup_Handler(MoveWait);
                  mesh_conf.manual_mesh_move();
                  Draw_Menu(UBLMesh);
                #endif
              #elif HAS_BED_PROBE
                Popup_Handler(Level);
                gcode.process_subcommands_now(F("G29"));
                planner.synchronize();
                Update_Status(GET_TEXT_F(MSG_MESH_DONE));
                Popup_Handler(SaveLevel);
              #else
                level_state = planner.leveling_active;
                set_bed_leveling_enabled(false);
                temp_val.gridpoint = 1;
                Popup_Handler(MoveWait);
                gcode.process_subcommands_now(F("M211 S0\nG29"));
                planner.synchronize();
                Draw_Menu(ManualMesh);
              #endif
            }
            break;
          case LEVELING_MANUAL:
            if (draw)
              Draw_Menu_Item(row, ICON_Mesh, GET_TEXT_F(MSG_UBL_MESH_EDIT), nullptr, true);
            else {
              #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
                if (!leveling_is_valid()) { Confirm_Handler(InvalidMesh); break; }
              #endif
              #if ENABLED(AUTO_BED_LEVELING_UBL)
                if (bedlevel.storage_slot < 0) { Popup_Handler(MeshSlot); break; }
              #endif
              PreheatBefore();
              if (axes_should_home()) { Popup_Handler(Home); gcode.home_all_axes(true); }
              level_state = planner.leveling_active;
              set_bed_leveling_enabled(false);
              mesh_conf.goto_mesh_value = false;
              Popup_Handler(MoveWait);
              mesh_conf.manual_mesh_move();
              gcode.process_subcommands_now(F("M211 S0"));
              Draw_Menu(LevelManual);
            }
            break;
          case LEVELING_VIEW:
            if (draw)
              Draw_Menu_Item(row, ICON_Mesh, GET_TEXT_F(MSG_MESH_VIEW), nullptr, true);
            else {
              #if ENABLED(AUTO_BED_LEVELING_UBL)
                if (bedlevel.storage_slot < 0) { Popup_Handler(MeshSlot); break; }
              #endif
              Draw_Menu(LevelView);
            }
            break;
          case LEVELING_SETTINGS:
            if (draw)
              Draw_Menu_Item(row, ICON_Step, GET_TEXT_F(MSG_ADVANCED_SETTINGS), nullptr, true);
            else
              Draw_Menu(LevelSettings);
            break;
          #if ENABLED(AUTO_BED_LEVELING_UBL)
          case LEVELING_SLOT:
            if (draw) {
              Draw_Menu_Item(row, ICON_PrintSize, GET_TEXT_F(MSG_UBL_STORAGE_SLOT));
              Draw_Float(bedlevel.storage_slot, row, false, 1);
            }
            else
              Modify_Value(bedlevel.storage_slot, 0, settings.calc_num_meshes() - 1, 1);
            break;
          case LEVELING_LOAD:
            if (draw)
              Draw_Menu_Item(row, ICON_ReadEEPROM, GET_TEXT_F(MSG_UBL_LOAD_MESH));
            else {
              if (bedlevel.storage_slot < 0) { Popup_Handler(MeshSlot); break; }
              gcode.process_subcommands_now(F("G29 L"));
              planner.synchronize();
              AudioFeedback(true);
            }
            break;
          case LEVELING_SAVE:
            if (draw)
              Draw_Menu_Item(row, ICON_WriteEEPROM, GET_TEXT_F(MSG_UBL_SAVE_MESH));
            else {
              if (bedlevel.storage_slot < 0) { Popup_Handler(MeshSlot); break; }
              gcode.process_subcommands_now(F("G29 S"));
              planner.synchronize();
              AudioFeedback(true);
            }
            break;
          #endif
        }
        break;

      case LevelView:

        #define LEVELING_VIEW_BACK 0
        #define LEVELING_VIEW_MESH (LEVELING_VIEW_BACK + 1)
        #define LEVELING_VIEW_TEXT (LEVELING_VIEW_MESH + 1)
        #define LEVELING_VIEW_ASYMMETRIC (LEVELING_VIEW_TEXT + 1)
        #define LEVELING_VIEW_TOTAL LEVELING_VIEW_ASYMMETRIC

        switch (item) {
          case LEVELING_VIEW_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Leveling, LEVELING_VIEW);
            break;
          case LEVELING_VIEW_MESH:
            if (draw)
              Draw_Menu_Item(row, ICON_PrintSize, GET_TEXT_F(MSG_MESH_VIEW), nullptr, true);
            else
              Draw_Menu(MeshViewer);
            break;
          case LEVELING_VIEW_TEXT:
            if (draw) {
              Draw_Menu_Item(row, ICON_Contact, F("Viewer Show Values"));
              Draw_Checkbox(row, mesh_conf.viewer_print_value);
            }
            else {
              mesh_conf.viewer_print_value = !mesh_conf.viewer_print_value;
              Draw_Checkbox(row, mesh_conf.viewer_print_value);
            }
            break;
          case LEVELING_VIEW_ASYMMETRIC:
            if (draw) {
              Draw_Menu_Item(row, ICON_Axis, F("Viewer Asymmetric"));
              Draw_Checkbox(row, mesh_conf.viewer_asymmetric_range);
            }
            else {
              mesh_conf.viewer_asymmetric_range = !mesh_conf.viewer_asymmetric_range;
              Draw_Checkbox(row, mesh_conf.viewer_asymmetric_range);
            }
            break;
        }
        break;

      case LevelSettings:

        #define LEVELING_SETTINGS_BACK 0
        #define LEVELING_SETTINGS_LEVELTEMP_MODE (LEVELING_SETTINGS_BACK + ENABLED(PREHEAT_BEFORE_LEVELING))
        #define LEVELING_SETTINGS_HOTENDTEMP (LEVELING_SETTINGS_LEVELTEMP_MODE + ENABLED(PREHEAT_BEFORE_LEVELING))
        #define LEVELING_SETTINGS_BEDTEMP (LEVELING_SETTINGS_HOTENDTEMP + ENABLED(PREHEAT_BEFORE_LEVELING))
        #define LEVELING_SETTINGS_FADE (LEVELING_SETTINGS_BEDTEMP + 1)
        #define LEVELING_SETTINGS_TILT (LEVELING_SETTINGS_FADE + ENABLED(AUTO_BED_LEVELING_UBL))
        #define LEVELING_SETTINGS_PLANE (LEVELING_SETTINGS_TILT + ENABLED(AUTO_BED_LEVELING_UBL))
        #define LEVELING_SETTINGS_ZERO (LEVELING_SETTINGS_PLANE + ENABLED(AUTO_BED_LEVELING_UBL))
        #define LEVELING_SETTINGS_UNDEF (LEVELING_SETTINGS_ZERO + ENABLED(AUTO_BED_LEVELING_UBL))
        #define LEVELING_SETTINGS_TOTAL LEVELING_SETTINGS_UNDEF

        switch (item) {
          case LEVELING_SETTINGS_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else
              Draw_Menu(Leveling, LEVELING_SETTINGS);
            break;
          #if ENABLED(PREHEAT_BEFORE_LEVELING)
            case LEVELING_SETTINGS_LEVELTEMP_MODE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Homing, F("Preheat Mode"));
              Draw_Option(temp_val.LevelingTempmode, preheat_levmodes, row);
            }
            else
              Modify_Option(temp_val.LevelingTempmode, preheat_levmodes, 3);
            break;
            case LEVELING_SETTINGS_HOTENDTEMP:
              if (draw) {
                Draw_Menu_Item(row, ICON_SetEndTemp, F("Preheat Hotend"));
                Draw_Float(eeprom_settings.hotend_levtemp, row, false, 1);
              }
              else
                Modify_Value(eeprom_settings.hotend_levtemp, MIN_E_TEMP, MAX_E_TEMP, 1);
              break;
            case LEVELING_SETTINGS_BEDTEMP:
              if (draw) {
              Draw_Menu_Item(row, ICON_SetBedTemp, F("Preheat Bed"));
              Draw_Float(eeprom_settings.bed_levtemp, row, false, 1);
            }
            else
              Modify_Value(eeprom_settings.bed_levtemp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
            break;
          #endif
          case LEVELING_SETTINGS_FADE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Fade, GET_TEXT_F(MSG_Z_FADE_HEIGHT));
              Draw_Float(planner.z_fade_height, row, false, 1);
            }
            else {
              Modify_Value(planner.z_fade_height, 0, Z_MAX_POS, 1);
              planner.z_fade_height = -1;
              set_z_fade_height(planner.z_fade_height);
            }
            break;

          #if ENABLED(AUTO_BED_LEVELING_UBL)

            case LEVELING_SETTINGS_TILT:
              if (draw) {
                Draw_Menu_Item(row, ICON_Tilt, F("Tilting Grid"));
                Draw_Float(mesh_conf.tilt_grid, row, false, 1);
              }
              else
                Modify_Value(mesh_conf.tilt_grid, 1, 8, 1);
              break;

            case LEVELING_SETTINGS_PLANE:
              if (draw)
                Draw_Menu_Item(row, ICON_ResumeEEPROM, F("Convert Mesh to Plane"));
              else {
                if (mesh_conf.create_plane_from_mesh()) break;
                gcode.process_subcommands_now(F("M420 S1"));
                planner.synchronize();
                AudioFeedback(true);
              }
              break;

            case LEVELING_SETTINGS_ZERO:
              if (draw)
                Draw_Menu_Item(row, ICON_Mesh, F("Zero Current Mesh"));
              else
                ZERO(bedlevel.z_values);
              break;

            case LEVELING_SETTINGS_UNDEF:
              if (draw)
                Draw_Menu_Item(row, ICON_Mesh, F("Clear Current Mesh"));
              else
                bedlevel.invalidate();
              break;

          #endif // AUTO_BED_LEVELING_UBL
        }
        break;

      case MeshViewer:
        #define MESHVIEW_BACK 0
        #define MESHVIEW_TOTAL MESHVIEW_BACK

        if (item == MESHVIEW_BACK) {
          if (draw) {
            Draw_Menu_Item(0, ICON_Back, GET_TEXT_F(MSG_BACK));
            mesh_conf.Draw_Bed_Mesh();
            mesh_conf.Set_Mesh_Viewer_Status();
          }
          else if (!mesh_conf.drawing_mesh) {
            Draw_Menu(LevelView, LEVELING_VIEW_MESH);
            Update_Status("");
          }
        }
        break;

      case LevelManual:

        #define LEVELING_M_BACK 0
        #define LEVELING_M_X (LEVELING_M_BACK + 1)
        #define LEVELING_M_Y (LEVELING_M_X + 1)
        #define LEVELING_M_NEXT (LEVELING_M_Y + 1)
        #define LEVELING_M_OFFSET (LEVELING_M_NEXT + 1)
        #define LEVELING_M_UP (LEVELING_M_OFFSET + 1)
        #define LEVELING_M_DOWN (LEVELING_M_UP + 1)
        #define LEVELING_M_GOTO_VALUE (LEVELING_M_DOWN + 1)
        #define LEVELING_M_UNDEF (LEVELING_M_GOTO_VALUE + ENABLED(AUTO_BED_LEVELING_UBL))
        #define LEVELING_M_TOTAL LEVELING_M_UNDEF

        switch (item) {
          case LEVELING_M_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else {
              set_bed_leveling_enabled(level_state);
              TERN_(AUTO_BED_LEVELING_BILINEAR, bedlevel.refresh_bed_level());
              Draw_Menu(Leveling, LEVELING_MANUAL);
            }
            break;
          case LEVELING_M_X:
            if (draw) {
              Draw_Menu_Item(row, ICON_MoveX, GET_TEXT_F(MSG_MESH_X));
              Draw_Float(mesh_conf.mesh_x, row, 0, 1);
            }
            else
              Modify_Value(mesh_conf.mesh_x, 0, GRID_MAX_POINTS_X - 1, 1);
            break;
          case LEVELING_M_Y:
            if (draw) {
              Draw_Menu_Item(row, ICON_MoveY, GET_TEXT_F(MSG_MESH_Y));
              Draw_Float(mesh_conf.mesh_y, row, 0, 1);
            }
            else
              Modify_Value(mesh_conf.mesh_y, 0, GRID_MAX_POINTS_Y - 1, 1);
            break;
          case LEVELING_M_NEXT:
            if (draw)
              Draw_Menu_Item(row, ICON_More, GET_TEXT_F(MSG_LEVEL_BED_NEXT_POINT));
            else {
              if (mesh_conf.mesh_x != (GRID_MAX_POINTS_X - 1) || mesh_conf.mesh_y != (GRID_MAX_POINTS_Y - 1)) {
                if ((mesh_conf.mesh_x == (GRID_MAX_POINTS_X - 1) && !(mesh_conf.mesh_y & 1)) || (!mesh_conf.mesh_x && (mesh_conf.mesh_y & 1)))
                  mesh_conf.mesh_y++;
                else if (mesh_conf.mesh_y & 1)
                  mesh_conf.mesh_x--;
                else
                  mesh_conf.mesh_x++;
                mesh_conf.manual_mesh_move();
              }
            }
            break;
          case LEVELING_M_OFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetZOffset, F("Point Z Offset"));
              Draw_Float(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y], row, false, 100);
            }
            else {
              if (isnan(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y]))
                bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] = 0;
              Modify_Value(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y], MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
            }
            break;
          case LEVELING_M_UP:
            if (draw)
              Draw_Menu_Item(row, ICON_Axis, F("Microstep Up"));
            else if (bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] < MAX_Z_OFFSET) {
              bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] += 0.01;
              gcode.process_subcommands_now(F("M290 Z0.01"));
              planner.synchronize();
              current_position.z += 0.01f;
              sync_plan_position();
              Draw_Float(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y], row - 1, false, 100);
            }
            break;
          case LEVELING_M_DOWN:
            if (draw)
              Draw_Menu_Item(row, ICON_AxisD, F("Microstep Down"));
            else if (bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] > MIN_Z_OFFSET) {
              bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] -= 0.01;
              gcode.process_subcommands_now(F("M290 Z-0.01"));
              planner.synchronize();
              current_position.z -= 0.01f;
              sync_plan_position();
              Draw_Float(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y], row - 2, false, 100);
            }
            break;
          case LEVELING_M_GOTO_VALUE:
            if (draw) {
              Draw_Menu_Item(row, ICON_StockConfiguration, F("Go to Mesh Z Value"));
              Draw_Checkbox(row, mesh_conf.goto_mesh_value);
            }
            else {
              mesh_conf.goto_mesh_value = !mesh_conf.goto_mesh_value;
              current_position.z = 0;
              mesh_conf.manual_mesh_move(true);
              Draw_Checkbox(row, mesh_conf.goto_mesh_value);
            }
            break;
          #if ENABLED(AUTO_BED_LEVELING_UBL)
          case LEVELING_M_UNDEF:
            if (draw)
              Draw_Menu_Item(row, ICON_ResumeEEPROM, F("Clear Point Value"));
            else {
              mesh_conf.manual_value_update(true);
              Redraw_Menu(false);
            }
            break;
          #endif
        }
        break;
    #endif // HAS_MESH

    #if ENABLED(AUTO_BED_LEVELING_UBL) && !HAS_BED_PROBE
      case UBLMesh:

        #define UBL_M_BACK 0
        #define UBL_M_NEXT (UBL_M_BACK + 1)
        #define UBL_M_PREV (UBL_M_NEXT + 1)
        #define UBL_M_OFFSET (UBL_M_PREV + 1)
        #define UBL_M_UP (UBL_M_OFFSET + 1)
        #define UBL_M_DOWN (UBL_M_UP + 1)
        #define UBL_M_TOTAL UBL_M_DOWN

        switch (item) {
          case UBL_M_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
            else {
              set_bed_leveling_enabled(level_state);
              Draw_Menu(Leveling, LEVELING_GET_MESH);
            }
            break;
          case UBL_M_NEXT:
            if (draw) {
              if (mesh_conf.mesh_x != (GRID_MAX_POINTS_X - 1) || mesh_conf.mesh_y != (GRID_MAX_POINTS_Y - 1))
                Draw_Menu_Item(row, ICON_More, GET_TEXT_F(MSG_LEVEL_BED_NEXT_POINT));
              else
                Draw_Menu_Item(row, ICON_More, GET_TEXT_F(MSG_UBL_SAVE_MESH));
            }
            else {
              if (mesh_conf.mesh_x != (GRID_MAX_POINTS_X - 1) || mesh_conf.mesh_y != (GRID_MAX_POINTS_Y - 1)) {
                if ((mesh_conf.mesh_x == (GRID_MAX_POINTS_X - 1) && !(mesh_conf.mesh_y & 1)) || (!mesh_conf.mesh_x && (mesh_conf.mesh_y & 1)))
                  mesh_conf.mesh_y++;
                else if (mesh_conf.mesh_y & 1)
                  mesh_conf.mesh_x--;
                else
                  mesh_conf.mesh_x++;
                mesh_conf.manual_mesh_move();
              }
              else {
                gcode.process_subcommands_now(F("G29 S"));
                planner.synchronize();
                AudioFeedback(true);
                Draw_Menu(Leveling, LEVELING_GET_MESH);
              }
            }
            break;
          case UBL_M_PREV:
            if (draw)
              Draw_Menu_Item(row, ICON_More, F("Previous Point"));
            else {
              if (mesh_conf.mesh_x != 0 || mesh_conf.mesh_y != 0) {
                if ((mesh_conf.mesh_x == (GRID_MAX_POINTS_X - 1) && (mesh_conf.mesh_y & 1)) || (!mesh_conf.mesh_x && !(mesh_conf.mesh_y & 1)))
                  mesh_conf.mesh_y--;
                else if (mesh_conf.mesh_y & 1)
                  mesh_conf.mesh_x++;
                else
                  mesh_conf.mesh_x--;
                mesh_conf.manual_mesh_move();
              }
            }
            break;
          case UBL_M_OFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetZOffset, F("Point Z Offset"));
              Draw_Float(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y], row, false, 100);
            }
            else {
              if (isnan(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y]))
                bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] = 0;
              Modify_Value(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y], MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
            }
            break;
          case UBL_M_UP:
            if (draw)
              Draw_Menu_Item(row, ICON_Axis, F("Microstep Up"));
            else if (bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] < MAX_Z_OFFSET) {
              bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] += 0.01;
              gcode.process_subcommands_now(F("M290 Z0.01"));
              planner.synchronize();
              current_position.z += 0.01f;
              sync_plan_position();
              Draw_Float(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y], row - 1, false, 100);
            }
            break;
          case UBL_M_DOWN:
            if (draw)
              Draw_Menu_Item(row, ICON_Axis, F("Microstep Down"));
            else if (bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] > MIN_Z_OFFSET) {
              bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y] -= 0.01;
              gcode.process_subcommands_now(F("M290 Z-0.01"));
              planner.synchronize();
              current_position.z -= 0.01f;
              sync_plan_position();
              Draw_Float(bedlevel.z_values[mesh_conf.mesh_x][mesh_conf.mesh_y], row - 2, false, 100);
            }
            break;
        }
        break;
    #endif // AUTO_BED_LEVELING_UBL && !HAS_BED_PROBE

    #if ENABLED(PROBE_MANUALLY)
      case ManualMesh:

        #define MMESH_BACK 0
        #define MMESH_NEXT (MMESH_BACK + 1)
        #define MMESH_OFFSET (MMESH_NEXT + 1)
        #define MMESH_UP (MMESH_OFFSET + 1)
        #define MMESH_DOWN (MMESH_UP + 1)
        #define MMESH_OLD (MMESH_DOWN + 1)
        #define MMESH_TOTAL MMESH_OLD

        switch (item) {
          case MMESH_BACK:
            if (draw)
              Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BUTTON_CANCEL));
            else {
              gcode.process_subcommands_now(F("G29 A"));
              planner.synchronize();
              set_bed_leveling_enabled(level_state);
              Draw_Menu(Leveling, LEVELING_GET_MESH);
            }
            break;
          case MMESH_NEXT:
            if (draw) {
              if (temp_val.gridpoint < GRID_MAX_POINTS)
                Draw_Menu_Item(row, ICON_More, GET_TEXT_F(MSG_LEVEL_BED_NEXT_POINT));
              else
                Draw_Menu_Item(row, ICON_More, GET_TEXT_F(MSG_UBL_SAVE_MESH));
            }
            else if (temp_val.gridpoint < GRID_MAX_POINTS) {
              Popup_Handler(MoveWait);
              gcode.process_subcommands_now(F("G29"));
              planner.synchronize();
              temp_val.gridpoint++;
              Redraw_Menu();
            }
            else {
              gcode.process_subcommands_now(F("G29"));
              planner.synchronize();
              AudioFeedback(settings.save());
              Draw_Menu(Leveling, LEVELING_GET_MESH);
            }
            break;
          case MMESH_OFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetZOffset, F("Z Position"));
              current_position.z = MANUAL_PROBE_START_Z;
              Draw_Float(current_position.z, row, false, 100);
            }
            else
              Modify_Value(current_position.z, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
            break;
          case MMESH_UP:
            if (draw)
              Draw_Menu_Item(row, ICON_Axis, F("Microstep Up"));
            else if (current_position.z < MAX_Z_OFFSET) {
              gcode.process_subcommands_now(F("M290 Z0.01"));
              planner.synchronize();
              current_position.z += 0.01f;
              sync_plan_position();
              Draw_Float(current_position.z, row - 1, false, 100);
            }
            break;
          case MMESH_DOWN:
            if (draw)
              Draw_Menu_Item(row, ICON_AxisD, F("Microstep Down"));
            else if (current_position.z > MIN_Z_OFFSET) {
              gcode.process_subcommands_now(F("M290 Z-0.01"));
              planner.synchronize();
              current_position.z -= 0.01f;
              sync_plan_position();
              Draw_Float(current_position.z, row - 2, false, 100);
            }
            break;
          case MMESH_OLD:
            uint8_t mesh_x, mesh_y;
            // 0,0 -> 1,0 -> 2,0 -> 2,1 -> 1,1 -> 0,1 -> 0,2 -> 1,2 -> 2,2
            mesh_y = (temp_val.gridpoint - 1) / (GRID_MAX_POINTS_Y);
            mesh_x = (temp_val.gridpoint - 1) % (GRID_MAX_POINTS_X);

            if (mesh_y & 1)
              mesh_x = (GRID_MAX_POINTS_X) - mesh_x - 1;

            const float currval = bedlevel.z_values[mesh_x][mesh_y];

            if (draw) {
              Draw_Menu_Item(row, ICON_Zoffset, GET_TEXT_F(MSG_MESH_EDIT_Z));
              Draw_Float(currval, row, false, 100);
            }
            else if (!isnan(currval)) {
              current_position.z = currval;
              planner.synchronize();
              planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
              planner.synchronize();
              Draw_Float(current_position.z, row - 3, false, 100);
            }
            break;
        }
        break;
    #endif // PROBE_MANUALLY

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
      #define TUNE_FWRETRACT (TUNE_ZDOWN + ENABLED(FWRETRACT))
      #define TUNE_HOSTACTIONS (TUNE_FWRETRACT + ENABLED(HAS_HOSTACTION_MENUS))
      #define TUNE_CHANGEFIL (TUNE_HOSTACTIONS + ENABLED(FILAMENT_LOAD_UNLOAD_GCODES))
      #define TUNE_FILSENSORENABLED (TUNE_CHANGEFIL + ENABLED(FILAMENT_RUNOUT_SENSOR))
      #define TUNE_BACKLIGHT_OFF (TUNE_FILSENSORENABLED + 1)
      #define TUNE_BACKLIGHT (TUNE_BACKLIGHT_OFF + 1)
      #define TUNE_CASELIGHT (TUNE_BACKLIGHT + ENABLED(CASE_LIGHT_MENU))
      #define TUNE_LEDCONTROL (TUNE_CASELIGHT + (ENABLED(LED_CONTROL_MENU) && DISABLED(CASE_LIGHT_USE_NEOPIXEL)))
      #define TUNE_LOCKSCREEN (TUNE_LEDCONTROL + ENABLED(HAS_LOCKSCREEN))
      #define TUNE_TOTAL TUNE_LOCKSCREEN

      switch (item) {
        case TUNE_BACK:
          if (draw) Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BACK));
          else Draw_Print_Screen();
          break;
        case TUNE_SPEED:
          if (draw) {
            Draw_Menu_Item(row, ICON_Speed, GET_TEXT_F(MSG_SPEED));
            Draw_Float(feedrate_percentage, row, false, 1);
          }
          else
            Modify_Value(feedrate_percentage, MIN_PRINT_SPEED, MAX_PRINT_SPEED, 1);
          break;

        #if HAS_HOTEND
          case TUNE_FLOW:
            if (draw) {
              Draw_Menu_Item(row, ICON_Speed, GET_TEXT_F(MSG_FLOW));
              Draw_Float(planner.flow_percentage[0], row, false, 1);
            }
            else
              Modify_Value(planner.flow_percentage[0], MIN_FLOW_RATE, MAX_FLOW_RATE, 1);
            break;
          case TUNE_HOTEND:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetEndTemp, F("Hotend"));
              Draw_Float(thermalManager.temp_hotend[0].target, row, false, 1);
            }
            else
              Modify_Value(thermalManager.temp_hotend[0].target, MIN_E_TEMP, MAX_E_TEMP, 1);
            break;
        #endif

        #if HAS_HEATED_BED
          case TUNE_BED:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetBedTemp, F("Bed"));
              Draw_Float(thermalManager.temp_bed.target, row, false, 1);
            }
            else
              Modify_Value(thermalManager.temp_bed.target, MIN_BED_TEMP, MAX_BED_TEMP, 1);
            break;
        #endif

        #if HAS_FAN
          case TUNE_FAN:
            if (draw) {
              Draw_Menu_Item(row, ICON_FanSpeed, GET_TEXT_F(MSG_FAN_SPEED));
              Draw_Float(thermalManager.fan_speed[0], row, false, 1);
            }
            else
              Modify_Value(thermalManager.fan_speed[0], MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
            break;
        #endif

        #if HAS_ZOFFSET_ITEM
          case TUNE_ZOFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_FanSpeed, F("Z-Offset"));
              Draw_Float(temp_val.zoffsetvalue, row, false, 100);
            }
            else
              Modify_Value(temp_val.zoffsetvalue, MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
            break;
          case TUNE_ZUP:
            if (draw)
              Draw_Menu_Item(row, ICON_Axis, F("Z-Offset Up"));
            else if (temp_val.zoffsetvalue < MAX_Z_OFFSET) {
              gcode.process_subcommands_now(F("M290 Z0.01"));
              temp_val.zoffsetvalue += 0.01;
              Draw_Float(temp_val.zoffsetvalue, row - 1, false, 100);
            }
            break;
          case TUNE_ZDOWN:
            if (draw)
              Draw_Menu_Item(row, ICON_AxisD, F("Z-Offset Down"));
            else if (temp_val.zoffsetvalue > MIN_Z_OFFSET) {
              gcode.process_subcommands_now(F("M290 Z-0.01"));
              temp_val.zoffsetvalue -= 0.01;
              Draw_Float(temp_val.zoffsetvalue, row - 2, false, 100);
            }
            break;
        #endif

        #if ENABLED(FWRETRACT)
          case TUNE_FWRETRACT:
            if (draw)
              Draw_Menu_Item(row, ICON_StepE, GET_TEXT_F(MSG_AUTORETRACT), nullptr, true);
            else {
              temp_val.flag_tune = true;
              last_pos_selection = selection;
              Draw_Menu(FwRetraction);
            }
            break;
        #endif

        #if HAS_HOSTACTION_MENUS
          case TUNE_HOSTACTIONS:
            if (draw)
              Draw_Menu_Item(row, ICON_SetHome, F("Host Actions"), nullptr, true);
            else {
              temp_val.flag_tune = true;
              last_pos_selection = selection;
              Draw_Menu(HostActions);
            }
            break;
        #endif

        #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
          case TUNE_CHANGEFIL:
            if (draw) Draw_Menu_Item(row, ICON_ResumeEEPROM, GET_TEXT_F(MSG_FILAMENTCHANGE));
            else Popup_Handler(ConfFilChange);
            break;
        #endif

        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          case TUNE_FILSENSORENABLED:
            if (draw) {
              Draw_Menu_Item(row, ICON_Extruder, GET_TEXT_F(MSG_RUNOUT_SENSOR));
              Draw_Checkbox(row, runout.enabled);
            }
            else {
              runout.enabled = !runout.enabled;
              Draw_Checkbox(row, runout.enabled);
            }
            break;
        #endif

        case TUNE_BACKLIGHT_OFF:
          if (draw) Draw_Menu_Item(row, ICON_Brightness, GET_TEXT_F(MSG_BRIGHTNESS_OFF));
          else ui.set_brightness(0);
          break;
        case TUNE_BACKLIGHT:
          if (draw) {
            Draw_Menu_Item(row, ICON_Brightness,  GET_TEXT_F(MSG_BRIGHTNESS));
            Draw_Float(ui.brightness, row, false, 1);
          }
          else
            Modify_Value(ui.brightness, LCD_BRIGHTNESS_MIN, LCD_BRIGHTNESS_MAX, 1, ui.refresh_brightness);
          break;
        #if ENABLED(CASE_LIGHT_MENU)
          case TUNE_CASELIGHT:
            if (draw) {
              Draw_Menu_Item(row, ICON_CaseLight, GET_TEXT_F(MSG_CASE_LIGHT));
              Draw_Checkbox(row, caselight.on);
            }
            else {
              caselight.on = !caselight.on;
              caselight.update_enabled();
              Draw_Checkbox(row, caselight.on);
            }
            break;
        #endif
        #if ENABLED(LED_CONTROL_MENU) && DISABLED(CASE_LIGHT_USE_NEOPIXEL)
          case TUNE_LEDCONTROL:
            if (draw) {
              Draw_Menu_Item(row, ICON_LedControl, GET_TEXT_F(MSG_LEDS));
              Draw_Checkbox(row, leds.lights_on);
            }
            else {
              leds.toggle();
              Draw_Checkbox(row, leds.lights_on);
            }
            break;
        #endif
        #if HAS_LOCKSCREEN
          case TUNE_LOCKSCREEN:
            if (draw) Draw_Menu_Item(row, ICON_Lock, GET_TEXT_F(MSG_LOCKSCREEN));
            else DWIN_LockScreen();
            break;
        #endif
      }
      break;

    #if HAS_PREHEAT && HAS_HOTEND
      case PreheatHotend:

          #define PREHEATHOTEND_BACK 0
          #define PREHEATHOTEND_CONTINUE (PREHEATHOTEND_BACK + 1)
          #define PREHEATHOTEND_1 (PREHEATHOTEND_CONTINUE + (PREHEAT_COUNT >= 1))
          #define PREHEATHOTEND_2 (PREHEATHOTEND_1 + (PREHEAT_COUNT >= 2))
          #define PREHEATHOTEND_3 (PREHEATHOTEND_2 + (PREHEAT_COUNT >= 3))
          #define PREHEATHOTEND_4 (PREHEATHOTEND_3 + (PREHEAT_COUNT >= 4))
          #define PREHEATHOTEND_5 (PREHEATHOTEND_4 + (PREHEAT_COUNT >= 5))
          #define PREHEATHOTEND_CUSTOM (PREHEATHOTEND_5 + 1)
          #define PREHEATHOTEND_TOTAL PREHEATHOTEND_CUSTOM

          switch (item) {
            case PREHEATHOTEND_BACK:
              if (draw)
                Draw_Menu_Item(row, ICON_Back, GET_TEXT_F(MSG_BUTTON_CANCEL));
              else {
                thermalManager.setTargetHotend(0, 0);
                thermalManager.set_fan_speed(0, 0);
                Redraw_Menu(false, true, true);
              }
              break;
            case PREHEATHOTEND_CONTINUE:
              if (draw)
                Draw_Menu_Item(row, ICON_SetEndTemp, GET_TEXT_F(MSG_BUTTON_CONTINUE));
              else {
                Popup_Handler(Heating);
                thermalManager.wait_for_hotend(0);
                switch (last_menu) {
                  case Prepare:
                    Popup_Handler(FilChange);
                    sprintf_P(cmd, PSTR("M600 B1 R%i"), thermalManager.temp_hotend[0].target);
                    gcode.process_subcommands_now(cmd);
                    Draw_Menu(Prepare, PREPARE_CHANGEFIL);
                    break;
                  #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
                    case ChangeFilament:
                      switch (last_selection) {
                        case CHANGEFIL_LOAD:
                          Popup_Handler(FilLoad);
                          Update_Status(GET_TEXT(MSG_FILAMENTLOAD));
                          gcode.process_subcommands_now(F("M701"));
                          planner.synchronize();
                          Draw_Menu(ChangeFilament, CHANGEFIL_LOAD);
                          break;
                        case CHANGEFIL_UNLOAD:
                          Popup_Handler(FilLoad, true);
                          Update_Status(GET_TEXT(MSG_FILAMENTUNLOAD));
                          gcode.process_subcommands_now(F("M702"));
                          planner.synchronize();
                          Draw_Menu(ChangeFilament, CHANGEFIL_UNLOAD);
                          break;
                        case CHANGEFIL_CHANGE:
                          Popup_Handler(FilChange);
                          Update_Status(GET_TEXT(MSG_FILAMENTCHANGE));
                          sprintf_P(cmd, PSTR("M600 B1 R%i"), thermalManager.temp_hotend[0].target);
                          gcode.process_subcommands_now(cmd);
                          Draw_Menu(ChangeFilament, CHANGEFIL_CHANGE);
                          break;
                      }
                      break;
                  #endif
                  default:
                    Redraw_Menu(true, true, true);
                    break;
                }
              }
              break;

            #define _PREHEAT_HOTEND_CASE(N) \
              case PREHEATHOTEND_##N: \
                if (draw) Draw_Menu_Item(row, ICON_Temperature, F(PREHEAT_## N ##_LABEL)); \
                else ui.preheat_hotend_and_fan((N) - 1); \
                break;

            REPEAT_1(PREHEAT_COUNT, _PREHEAT_HOTEND_CASE)

            case PREHEATHOTEND_CUSTOM:
              if (draw) {
                Draw_Menu_Item(row, ICON_Temperature, GET_TEXT_F(MSG_PREHEAT_CUSTOM));
                Draw_Float(thermalManager.temp_hotend[0].target, row, false, 1);
              }
              else
                Modify_Value(thermalManager.temp_hotend[0].target, EXTRUDE_MINTEMP, MAX_E_TEMP, 1);
              break;
          }
        break;

    #endif // HAS_PREHEAT && HAS_HOTEND
  }
}

FSTR_P CrealityDWINClass::Get_Menu_Title(uint8_t menu) {
  switch (menu) {
    case MainMenu:          return GET_TEXT_F(MSG_MAIN);
    case Prepare:           return GET_TEXT_F(MSG_PREPARE);
    case HomeMenu:          return GET_TEXT_F(MSG_HOMING);
    case Move:              return GET_TEXT_F(MSG_MOVE_AXIS);
    case ManualLevel:       return GET_TEXT_F(MSG_BED_TRAMMING_MANUAL);
    #if HAS_ZOFFSET_ITEM
      case ZOffset:         return F("Z Offset");
    #endif
    #if HAS_PREHEAT
      case Preheat:         return F("Preheat");
    #endif
    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      case ChangeFilament:  return GET_TEXT_F(MSG_FILAMENTCHANGE);
    #endif
    #if HAS_HOSTACTION_MENUS
      case HostActions:     return F("Host Actions");
    #endif
    #if HAS_CUSTOM_MENU
      case MenuCustom:
        #ifdef CUSTOM_MENU_CONFIG_TITLE
          return F(CUSTOM_MENU_CONFIG_TITLE);
        #else
          return F("Custom Commands");
        #endif
    #endif
    case Control:           return GET_TEXT_F(MSG_CONTROL);
    case TempMenu:          return GET_TEXT_F(MSG_TEMPERATURE);
    #if HAS_HOTEND || HAS_HEATED_BED
      case PID:             return F("PID Menu");
    #endif
    #if HAS_HOTEND
      case HotendPID:       return F("Hotend PID Settings");
    #endif
    #if HAS_HEATED_BED
      case BedPID:          return F("Bed PID Settings");
    #endif
    #if HAS_PREHEAT
      #define _PREHEAT_TITLE_CASE(N) case Preheat##N: return F(PREHEAT_## N ##_LABEL " Settings");
      REPEAT_1(PREHEAT_COUNT, _PREHEAT_TITLE_CASE)
    #endif
    case Motion:            return F("Motion Settings");
    #if ENABLED(FWRETRACT)
      case FwRetraction:    return F("Firmware Retract");
    #endif
    case HomeOffsets:       return GET_TEXT_F(MSG_SET_HOME_OFFSETS);
    case MaxSpeed:          return GET_TEXT_F(MSG_SPEED);
    case MaxAcceleration:   return GET_TEXT_F(MSG_ACCELERATION);
    #if HAS_CLASSIC_JERK
      case MaxJerk:         return GET_TEXT_F(MSG_JERK);
    #endif
    #if HAS_JUNCTION_DEVIATION
      case JDmenu:          return GET_TEXT_F(MSG_JUNCTION_DEVIATION);
    #endif
    case Steps:             return GET_TEXT_F(MSG_STEPS_PER_MM);
    case Visual:            return F("Visual Settings");
    #if HAS_HOSTACTION_MENUS
      case HostSettings:    return F("Host Settings");
      case ActionCommands:  return F("Host Actions");
    #endif
    case Advanced:          return GET_TEXT_F(MSG_ADVANCED_SETTINGS);
    #if HAS_BED_PROBE
      case ProbeMenu:       return GET_TEXT_F(MSG_ZPROBE_SETTINGS);
    #endif
    case ColorSettings:     return F("UI Color Settings");
    case Info:
    case InfoMain:          return GET_TEXT_F(MSG_INFO_SCREEN);
    #if HAS_MESH
      case Leveling:        return GET_TEXT_F(MSG_BED_LEVELING);
      case LevelView:       return GET_TEXT_F(MSG_MESH_VIEW);
      case LevelSettings:   return F("Leveling Settings");
      case MeshViewer:      return GET_TEXT_F(MSG_MESH_VIEW);
      case LevelManual:     return GET_TEXT_F(MSG_UBL_FINE_TUNE_MESH);
    #endif
    #if ENABLED(AUTO_BED_LEVELING_UBL) && !HAS_BED_PROBE
      case UBLMesh:         return GET_TEXT_F(MSG_UBL_LEVEL_BED);
    #endif
    #if ENABLED(PROBE_MANUALLY)
      case ManualMesh:      return GET_TEXT_F(MSG_MESH_LEVELING);
    #endif
    case Tune:              return GET_TEXT_F(MSG_TUNE);
    case PreheatHotend:     return F("Preheat Hotend");
    #if ANY(CASE_LIGHT_MENU, LED_CONTROL_MENU)
      case Ledsmenu:        return F("Light Settings");
      #if ENABLED(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
        case CaseLightmenu: return GET_TEXT_F(MSG_CASE_LIGHT);
      #endif
      #if ENABLED(LED_CONTROL_MENU)
        case LedControlmenu: return GET_TEXT_F(MSG_LED_CONTROL);
        #if HAS_COLOR_LEDS
          #if ENABLED(LED_COLOR_PRESETS)
            case LedControlpresets: return GET_TEXT_F(MSG_LED_PRESETS);
          #else
            case LedControlcustom: return GET_TEXT_F(MSG_CUSTOM_LEDS);
          #endif
        #endif
      #endif
    #endif
  }
  return F("");
}

uint8_t CrealityDWINClass::Get_Menu_Size(uint8_t menu) {
  switch (menu) {
    case Prepare:           return PREPARE_TOTAL;
    case HomeMenu:          return HOME_TOTAL;
    case Move:              return MOVE_TOTAL;
    case ManualLevel:       return MLEVEL_TOTAL;
    #if HAS_ZOFFSET_ITEM
      case ZOffset:         return ZOFFSET_TOTAL;
    #endif
    #if HAS_PREHEAT
      case Preheat:         return PREHEAT_TOTAL;
    #endif
    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      case ChangeFilament:  return CHANGEFIL_TOTAL;
    #endif
    #if HAS_HOSTACTION_MENUS
      case HostActions:     return HOSTACTIONS_TOTAL;
    #endif
    #if HAS_CUSTOM_MENU
      case MenuCustom:      return CUSTOM_MENU_TOTAL;
    #endif
    case Control:           return CONTROL_TOTAL;
    case TempMenu:          return TEMP_TOTAL;
    #if HAS_HOTEND || HAS_HEATED_BED
      case PID:             return PID_TOTAL;
    #endif
    #if HAS_HOTEND
      case HotendPID:       return HOTENDPID_TOTAL;
    #endif
    #if HAS_HEATED_BED
      case BedPID:          return BEDPID_TOTAL;
    #endif
    #if HAS_PREHEAT
      case Preheat1 ... CAT(Preheat, PREHEAT_COUNT):
                            return PREHEAT_SUBMENU_TOTAL;
    #endif
    case Motion:            return MOTION_TOTAL;
    #if ENABLED(FWRETRACT)
      case FwRetraction:    return FWR_TOTAL;
    #endif
    case HomeOffsets:       return HOMEOFFSETS_TOTAL;
    case MaxSpeed:          return SPEED_TOTAL;
    case MaxAcceleration:   return ACCEL_TOTAL;
    #if HAS_CLASSIC_JERK
      case MaxJerk:         return JERK_TOTAL;
    #endif
    #if HAS_JUNCTION_DEVIATION
      case JDmenu:          return JD_TOTAL;
    #endif
    case Steps:             return STEPS_TOTAL;
    case Visual:            return VISUAL_TOTAL;
    #if HAS_HOSTACTION_MENUS
      case HostSettings:    return HOSTSETTINGS_TOTAL;
      case ActionCommands:  return ACTIONCOMMANDS_TOTAL;
    #endif
    case Advanced:          return ADVANCED_TOTAL;
    #if HAS_BED_PROBE
      case ProbeMenu:       return PROBE_TOTAL;
    #endif
    case Info:              return INFO_TOTAL;
    case InfoMain:          return INFO_TOTAL;
    #if ENABLED(AUTO_BED_LEVELING_UBL) && !HAS_BED_PROBE
      case UBLMesh:         return UBL_M_TOTAL;
    #endif
    #if ENABLED(PROBE_MANUALLY)
      case ManualMesh:      return MMESH_TOTAL;
    #endif
    #if HAS_MESH
      case Leveling:        return LEVELING_TOTAL;
      case LevelView:       return LEVELING_VIEW_TOTAL;
      case LevelSettings:   return LEVELING_SETTINGS_TOTAL;
      case MeshViewer:      return MESHVIEW_TOTAL;
      case LevelManual:     return LEVELING_M_TOTAL;
    #endif
    case Tune:              return TUNE_TOTAL;

    #if HAS_PREHEAT && HAS_HOTEND
      case PreheatHotend:   return PREHEATHOTEND_TOTAL;
    #endif

    case ColorSettings:     return COLORSETTINGS_TOTAL;
    #if ANY(CASE_LIGHT_MENU, LED_CONTROL_MENU)
      case Ledsmenu:         return LEDS_TOTAL;
      #if ENABLED(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
        case CaseLightmenu: return CASE_LIGHT_TOTAL;
      #endif
      #if ENABLED(LED_CONTROL_MENU)
        case LedControlmenu: return LEDCONTROL_TOTAL;
        #if HAS_COLOR_LEDS
          #if ENABLED(LED_COLOR_PRESETS)
            case LedControlpresets: return LEDCONTROL_PRESETS_TOTAL;
          #else
            case LedControlcustom: return LEDCONTROL_CUSTOM_TOTAL;
          #endif
        #endif
      #endif
    #endif
  }
  return 0;
}

/* Popup Config */

void CrealityDWINClass::Popup_Handler(PopupID popupid, bool option/*=false*/) {
  popup = last_popup = popupid;
  switch (popupid) {
    case Pause:         Draw_Popup(GET_TEXT_F(MSG_PAUSE_PRINT), F(""), F(""), Popup); break;
    case Stop:          Draw_Popup(GET_TEXT_F(MSG_STOP_PRINT), F(""), F(""), Popup); break;
    case Resume:        Draw_Popup(GET_TEXT_F(MSG_RESUME_PRINT), F("Looks Like the last"), F("print was interrupted."), Popup); break;
    case ConfFilChange: Draw_Popup(F("Confirm Filament Change"), F(""), F(""), Popup); break;
    case PurgeMore:     Draw_Popup(F("Purge more filament?"), F("(Cancel to finish process)"), F(""), Popup); break;
    case SaveLevel:     Draw_Popup(F("Leveling Complete"), F("Save to EEPROM?"), F(""), Popup); break;
    case MeshSlot:      Draw_Popup(F("Mesh slot not selected"), F("(Confirm to select slot 0)"), F(""), Popup); break;
    case ETemp:         Draw_Popup(GET_TEXT_F(MSG_HOTEND_TOO_COLD), GET_TEXT_F(MSG_PLEASE_PREHEAT), F(""), Popup); break;
    case ManualProbing: Draw_Popup(F("Manual Probing"), F("(Confirm to probe)"), F("(cancel to exit)"), Popup); break;
    case Level:         Draw_Popup(F("Auto Bed Leveling"), GET_TEXT_F(MSG_PLEASE_WAIT), F(""), Wait, ICON_AutoLeveling); break;
    case Home:          Draw_Popup(option ? GET_TEXT_F(MSG_PAUSE_PRINT_PARKING) : GET_TEXT_F(MSG_HOMING), GET_TEXT_F(MSG_PLEASE_WAIT), F(""), Wait, ICON_BLTouch); break;
    case MoveWait:      Draw_Popup(GET_TEXT_F(MSG_MOVING), GET_TEXT_F(MSG_PLEASE_WAIT), F(""), Wait, ICON_BLTouch); break;
    case Heating:       Draw_Popup(GET_TEXT_F(MSG_HEATING), GET_TEXT_F(MSG_PLEASE_WAIT), F(""), Wait, ICON_BLTouch); break;
    case FilLoad:       Draw_Popup(option ? F("Unloading Filament") : F("Loading Filament"), GET_TEXT_F(MSG_PLEASE_WAIT), F(""), Wait, ICON_BLTouch); break;
    case FilChange:     Draw_Popup(F("Filament Change"), F("Please wait for prompt."), F(""), Wait, ICON_BLTouch); break;
    case TempWarn:      Draw_Popup(option ? GET_TEXT_F(MSG_HOTEND_TOO_COLD) : F("Nozzle temp too high!"), F(""), F(""), Wait, option ? ICON_TempTooLow : ICON_TempTooHigh); break;
    case Runout:        Draw_Popup(F("Filament Runout"), F(""), F(""), Wait, ICON_BLTouch); break;
    #if !HAS_PIDPLOT
      case PIDWait:       Draw_Popup(F("PID Autotune"), F("in process"), GET_TEXT_F(MSG_PLEASE_WAIT), Wait, ICON_BLTouch); break;
    #endif
    case Resuming:      Draw_Popup(F("Resuming Print"), GET_TEXT_F(MSG_PLEASE_WAIT), F(""), Wait, ICON_BLTouch); break;
    #if HAS_CUSTOM_MENU
      case Custom:        Draw_Popup(F("Running Custom GCode"), GET_TEXT_F(MSG_PLEASE_WAIT), F(""), Wait, ICON_BLTouch); break;
    #endif
    case PrintConfirm: Draw_Popup(option ? F("") : F("Print file?"), F(""), F(""), Popup); break;
    default: break;
  }
}

void CrealityDWINClass::Confirm_Handler(PopupID popupid, bool option/*=false*/) {
  popup = popupid;
  switch (popupid) {
    case FilInsert:   Draw_Popup(F("Insert Filament"), F("Press to Continue"), F(""), Confirm); break;
    case HeaterTime:  Draw_Popup(GET_TEXT_F(MSG_HEATER_TIMEOUT), GET_TEXT_F(MSG_FILAMENT_CHANGE_HEAT), F(""), Confirm); break;
    case UserInput:   Draw_Popup(option ? GET_TEXT_F(MSG_STOPPED) : F("Waiting for Input"), GET_TEXT_F(MSG_ADVANCED_PAUSE_WAITING), F(""), Confirm); break;
    case Level:       Draw_Popup(F("Bed Leveling"), GET_TEXT_F(MSG_PLEASE_WAIT), F(""), Cancel, ICON_AutoLeveling); break;
    case LevelError:  Draw_Popup(F("Couldn't enable Leveling"), F("(Valid mesh must exist)"), F(""), Confirm); break;
    case InvalidMesh: Draw_Popup(F("Valid mesh must exist"), F("before tuning can be"), F("performed"), Confirm); break;
    default: break;
  }
}

/* Navigation and Control */

void CrealityDWINClass::Main_Menu_Control() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW && selection < PAGE_COUNT - 1) {
    selection++; // Select Down
    Main_Menu_Icons();
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    selection--; // Select Up
    Main_Menu_Icons();
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER)
    switch (selection) {
      case PAGE_PRINT: card.mount(); Draw_SD_List(); break;
      case PAGE_PREPARE: Draw_Menu(Prepare); break;
      case PAGE_CONTROL: Draw_Menu(Control); break;
      case PAGE_INFO_LEVELING: Draw_Menu(TERN(HAS_MESH, Leveling, InfoMain)); break;
    }
  DWIN_UpdateLCD();
}

void CrealityDWINClass::Menu_Control() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW && selection < Get_Menu_Size(active_menu)) {
    DWIN_Draw_Rectangle(1, Def_Background_Color, 0, MBASE(selection - scrollpos) - 18, 14, MBASE(selection - scrollpos) + 33);
    selection++; // Select Down
    if (selection > scrollpos+MROWS) {
      scrollpos++;
      DWIN_Frame_AreaMove(1, 2, MLINE, Def_Background_Color, 0, 31, DWIN_WIDTH, 349);
      Menu_Item_Handler(active_menu, selection);
    }
    DWIN_Draw_Rectangle(1, GetColor(eeprom_settings.cursor_color, Def_Cursor_color), 0, MBASE(selection - scrollpos) - 18, 14, MBASE(selection - scrollpos) + 33);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    DWIN_Draw_Rectangle(1, Def_Background_Color, 0, MBASE(selection - scrollpos) - 18, 14, MBASE(selection - scrollpos) + 33);
    selection--; // Select Up
    if (selection < scrollpos) {
      scrollpos--;
      DWIN_Frame_AreaMove(1, 3, MLINE, Def_Background_Color, 0, 31, DWIN_WIDTH, 349);
      Menu_Item_Handler(active_menu, selection);
    }
    DWIN_Draw_Rectangle(1, GetColor(eeprom_settings.cursor_color, Def_Cursor_color), 0, MBASE(selection - scrollpos) - 18, 14, MBASE(selection - scrollpos) + 33);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER)
    Menu_Item_Handler(active_menu, selection, false);
  DWIN_UpdateLCD();
}

void CrealityDWINClass::Value_Control() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  float difvalue = 0;
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW) {
    tempvalue += EncoderRate.encoderMoveValue;
    difvalue = EncoderRate.encoderMoveValue;
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    tempvalue -= EncoderRate.encoderMoveValue;
    difvalue = - EncoderRate.encoderMoveValue;
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    process = Menu;
    EncoderRate.enabled = false;
    Draw_Float(tempvalue / valueunit, selection - scrollpos, false, valueunit);
    DWIN_UpdateLCD();
    if (active_menu == ZOffset && temp_val.zoffsetmode != 0) {
      planner.synchronize();
      if (temp_val.zoffsetmode == 1) {
        current_position.z += (tempvalue / valueunit - temp_val.zoffsetvalue);
        planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
      }
      current_position.z = 0;
      sync_plan_position();
    }
    else if (active_menu == Tune && selection == TUNE_ZOFFSET) {
      sprintf_P(cmd, PSTR("M290 Z%s"), dtostrf((tempvalue / valueunit - temp_val.zoffsetvalue), 1, 3, str_1));
      gcode.process_subcommands_now(cmd);
    }
    if (TERN0(HAS_HOTEND, valuepointer == &thermalManager.temp_hotend[0].pid.Ki) || TERN0(HAS_HEATED_BED, valuepointer == &thermalManager.temp_bed.pid.Ki))
      tempvalue = scalePID_i(tempvalue);
    if (TERN0(HAS_HOTEND, valuepointer == &thermalManager.temp_hotend[0].pid.Kd) || TERN0(HAS_HEATED_BED, valuepointer == &thermalManager.temp_bed.pid.Kd))
      tempvalue = scalePID_d(tempvalue);
    switch (valuetype) {
      case 0: *(float*)valuepointer = tempvalue / valueunit; break;
      case 1: *(uint8_t*)valuepointer = tempvalue / valueunit; break;
      case 2: *(uint16_t*)valuepointer = tempvalue / valueunit; break;
      case 3: *(int16_t*)valuepointer = tempvalue / valueunit; break;
      case 4: *(uint32_t*)valuepointer = tempvalue / valueunit; break;
      case 5: *(int8_t*)valuepointer = tempvalue / valueunit; break;
    }
    switch (active_menu) {
      case Move:
        planner.synchronize();
        planner.buffer_line(current_position, manual_feedrate_mm_s[selection - 1], active_extruder);
        break;
      #if HAS_MESH
        #if ENABLED(PROBE_MANUALLY)
          case ManualMesh:
            planner.synchronize();
            planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
            planner.synchronize();
            break;
        #endif
        #if ENABLED(AUTO_BED_LEVELING_UBL) && !HAS_BED_PROBE
          case UBLMesh: mesh_conf.manual_mesh_move(true); break;
        #endif
        case LevelManual: mesh_conf.manual_mesh_move(selection == LEVELING_M_OFFSET); break;
      #endif
    }
    if (valuepointer == &planner.flow_percentage[0])
      planner.refresh_e_factor(0);
    #if ENABLED(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
      if (valuepointer == &caselight.brightness)
        caselight.update_brightness();
    #endif
    #if HAS_COLOR_LEDS
      if ((valuepointer == &leds.color.r) || (valuepointer == &leds.color.g) || (valuepointer == &leds.color.b))
        ApplyLEDColor();
        #if HAS_WHITE_LED
          if (valuepointer == &leds.color.w) ApplyLEDColor();
        #endif
    #endif

    if (funcpointer) funcpointer();
    return;
  }
  NOLESS(tempvalue, (valuemin * valueunit));
  NOMORE(tempvalue, (valuemax * valueunit));
  Draw_Float(tempvalue / valueunit, selection - scrollpos, true, valueunit);
  DWIN_UpdateLCD();

  if (valuepointer == &ui.brightness) {
    *(uint8_t*)valuepointer = tempvalue / valueunit;
    ui.refresh_brightness();
  }

  switch (active_menu) {
    case Move:
      if (temp_val.livemove) {
        *(float*)valuepointer = tempvalue / valueunit;
        planner.buffer_line(current_position, manual_feedrate_mm_s[selection - 1], active_extruder);
      }
      break;
    case ZOffset:
      if (temp_val.zoffsetmode == 2) {
        planner.synchronize();
        sprintf_P(cmd, PSTR("M290 Z%s"), dtostrf((difvalue / valueunit), 1, 3, str_1));
        gcode.process_subcommands_now(cmd);
        planner.synchronize();
      }
      break;
    case Tune:
      if (selection == TUNE_ZOFFSET) {
        planner.synchronize();
        sprintf_P(cmd, PSTR("M290 Z%s"), dtostrf((difvalue / valueunit), 1, 3, str_1));
        gcode.process_subcommands_now(cmd);
        planner.synchronize();
      }
      break;
    #if ENABLED(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
      case CaseLightmenu:
        *(uint8_t*)valuepointer = tempvalue / valueunit;
        caselight.update_brightness();
        break;
    #endif
    #if ENABLED(LED_CONTROL_MENU, HAS_COLOR_LEDS)
      case LedControlmenu:
        *(uint8_t*)valuepointer = tempvalue / valueunit;
        leds.update();
        break;
    #endif
    default : break;
  }
}

void CrealityDWINClass::Option_Control() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW)
    tempvalue += EncoderRate.encoderMoveValue;
  else if (encoder_diffState == ENCODER_DIFF_CCW)
    tempvalue -= EncoderRate.encoderMoveValue;
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    process = Menu;
    EncoderRate.enabled = false;
    if (valuepointer == &color_names) {
      switch (selection) {
        case COLORSETTINGS_CURSOR: eeprom_settings.cursor_color = tempvalue; break;
        case COLORSETTINGS_SPLIT_LINE: eeprom_settings.menu_split_line = tempvalue; break;
        case COLORSETTINGS_MENU_TOP_BG: eeprom_settings.menu_top_bg = tempvalue; break;
        case COLORSETTINGS_MENU_TOP_TXT: eeprom_settings.menu_top_txt = tempvalue; break;
        case COLORSETTINGS_HIGHLIGHT_BORDER: eeprom_settings.highlight_box = tempvalue; break;
        case COLORSETTINGS_PROGRESS_PERCENT: eeprom_settings.progress_percent = tempvalue; break;
        case COLORSETTINGS_PROGRESS_TIME: eeprom_settings.progress_time = tempvalue; break;
        case COLORSETTINGS_PROGRESS_STATUS_BAR: eeprom_settings.status_bar_text = tempvalue; break;
        case COLORSETTINGS_PROGRESS_STATUS_AREA: eeprom_settings.status_area_text = tempvalue; break;
        case COLORSETTINGS_PROGRESS_COORDINATES: eeprom_settings.coordinates_text = tempvalue; break;
        case COLORSETTINGS_PROGRESS_COORDINATES_LINE: eeprom_settings.coordinates_split_line = tempvalue; break;
      }
      Redraw_Screen();
    }
    else if (valuepointer == &preheat_modes)
      temp_val.preheatmode = tempvalue;
    #if ENABLED(PREHEAT_BEFORE_LEVELING)
      else if (valuepointer == &preheat_levmodes) {
        temp_val.LevelingTempmode = tempvalue;
        eeprom_settings.ena_hotend_levtemp = false;
        eeprom_settings.ena_bed_levtemp = false;
        if (temp_val.LevelingTempmode == 0 || temp_val.LevelingTempmode == 1) eeprom_settings.ena_hotend_levtemp = true;
        if (temp_val.LevelingTempmode == 0 || temp_val.LevelingTempmode == 2) eeprom_settings.ena_bed_levtemp = true;
      }
    #endif
    else if (valuepointer == &zoffset_modes) {
      temp_val.zoffsetmode = tempvalue;
      if (temp_val.zoffsetmode == 1 || temp_val.zoffsetmode == 2) {
        if (axes_should_home()) {
          Popup_Handler(Home);
          gcode.home_all_axes(true);
        }
        Popup_Handler(MoveWait);
        #if ENABLED(Z_SAFE_HOMING)
          planner.synchronize();
          sprintf_P(cmd, PSTR("G0 F4000 X%s Y%s"), dtostrf(Z_SAFE_HOMING_X_POINT, 1, 3, str_1), dtostrf(Z_SAFE_HOMING_Y_POINT, 1, 3, str_2));
          gcode.process_subcommands_now(cmd);
        #else
          sprintf_P(cmd, PSTR("G0 F4000 X%s Y%s"), dtostrf((X_BED_SIZE + X_MIN_POS) / 2.0f, 1, 3, str_1), dtostrf((Y_BED_SIZE + Y_MIN_POS) / 2.0f, 1, 3, str_2));
          gcode.process_subcommands_now(cmd);
        #endif
        gcode.process_subcommands_now(F("G0 F300 Z0"));
        planner.synchronize();
        Redraw_Menu();
      }
    }

    Draw_Option(tempvalue, static_cast<const char * const *>(valuepointer), selection - scrollpos, false, (valuepointer == &color_names));
    DWIN_UpdateLCD();
    return;
  }
  NOLESS(tempvalue, valuemin);
  NOMORE(tempvalue, valuemax);
  Draw_Option(tempvalue, static_cast<const char * const *>(valuepointer), selection - scrollpos, true);
  DWIN_UpdateLCD();
}

void CrealityDWINClass::File_Control() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  static uint8_t filescrl = 0;
  if (encoder_diffState == ENCODER_DIFF_NO) {
    if (selection > 0) {
      card.getfilename_sorted(SD_ORDER(selection - 1, card.get_num_Files()));
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
        len = _MIN(pos, MENU_CHAR_LIMIT);
        char name[len + 1];
        if (pos >= 0) {
          LOOP_L_N(i, len) name[i] = filename[i + filescrl];
        }
        else {
          LOOP_L_N(i, MENU_CHAR_LIMIT + pos) name[i] = ' ';
          LOOP_S_L_N(i, MENU_CHAR_LIMIT + pos, MENU_CHAR_LIMIT) name[i] = filename[i - (MENU_CHAR_LIMIT + pos)];
        }
        name[len] = '\0';
        DWIN_Draw_Rectangle(1, Def_Background_Color, LBLX, MBASE(selection - scrollpos) - 14, 271, MBASE(selection - scrollpos) + 28);
        Draw_Menu_Item(selection - scrollpos, card.flag.filenameIsDir ? ICON_More : ICON_File, name);
        if (-pos >= MENU_CHAR_LIMIT) filescrl = 0;
        filescrl++;
        DWIN_UpdateLCD();
      }
    }
    return;
  }
  if (encoder_diffState == ENCODER_DIFF_CW && selection < card.get_num_Files()) {
    DWIN_Draw_Rectangle(1, Def_Background_Color, 0, MBASE(selection - scrollpos) - 18, 14, MBASE(selection - scrollpos) + 33);
    if (selection > 0) {
      DWIN_Draw_Rectangle(1, Def_Background_Color, LBLX, MBASE(selection - scrollpos) - 14, 271, MBASE(selection - scrollpos) + 28);
      Draw_SD_Item(selection, selection - scrollpos);
    }
    filescrl = 0;
    selection++; // Select Down
    if (selection > scrollpos + MROWS) {
      scrollpos++;
      DWIN_Frame_AreaMove(1, 2, MLINE, Def_Background_Color, 0, 31, DWIN_WIDTH, 349);
      Draw_SD_Item(selection, selection - scrollpos);
    }
    DWIN_Draw_Rectangle(1, GetColor(eeprom_settings.cursor_color, Def_Cursor_color), 0, MBASE(selection - scrollpos) - 18, 14, MBASE(selection - scrollpos) + 33);
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    DWIN_Draw_Rectangle(1, Def_Background_Color, 0, MBASE(selection - scrollpos) - 18, 14, MBASE(selection - scrollpos) + 33);
    DWIN_Draw_Rectangle(1, Def_Background_Color, LBLX, MBASE(selection - scrollpos) - 14, 271, MBASE(selection - scrollpos) + 28);
    Draw_SD_Item(selection, selection - scrollpos);
    filescrl = 0;
    selection--; // Select Up
    if (selection < scrollpos) {
      scrollpos--;
      DWIN_Frame_AreaMove(1, 3, MLINE, Def_Background_Color, 0, 31, DWIN_WIDTH, 349);
      Draw_SD_Item(selection, selection - scrollpos);
    }
    DWIN_Draw_Rectangle(1, GetColor(eeprom_settings.cursor_color, Def_Cursor_color), 0, MBASE(selection - scrollpos) - 18, 14, MBASE(selection - scrollpos) + 33);
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    if (selection == 0) {
      if (card.flag.workDirIsRoot) {
        process = Main;
        Draw_Main_Menu();
      }
      else {
        card.cdup();
        Draw_SD_List();
      }
    }
    else {
      card.getfilename_sorted(SD_ORDER(selection - 1, card.get_num_Files()));
      if (card.flag.filenameIsDir) {
        card.cd(card.filename);
        Draw_SD_List();
      }
      else {
        #if HAS_GCODE_PREVIEW
          Preview_DrawFromSD();
        #else
          card.openAndPrintFile(card.filename);
        #endif
      }
    }
  }
  DWIN_UpdateLCD();
}

void CrealityDWINClass::Print_Screen_Control() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW && selection < PRINT_COUNT - 1) {
    selection++; // Select Down
    Print_Screen_Icons();
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    selection--; // Select Up
    Print_Screen_Icons();
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (selection) {
      case PRINT_SETUP:
        Draw_Menu(Tune);
        Update_Status_Bar(true);
        break;
      case PRINT_PAUSE_RESUME:
        if (temp_val.paused) {
          if (temp_val.sdprint) {
            wait_for_user = false;
            #if ENABLED(PARK_HEAD_ON_PAUSE)
              card.startOrResumeFilePrinting();
              TERN_(POWER_LOSS_RECOVERY, recovery.prepare());
            #else
              char cmd[20];
              #if HAS_HEATED_BED
                sprintf_P(cmd, PSTR("M140 S%i"), temp_val.pausebed);
                gcode.process_subcommands_now(cmd);
              #endif
              #if HAS_EXTRUDERS
                sprintf_P(cmd, PSTR("M109 S%i"), temp_val.pausetemp);
                gcode.process_subcommands_now(cmd);
              #endif
              TERN_(HAS_FAN, thermalManager.fan_speed[0] = temp_val.pausefan);
              planner.synchronize();
              TERN_(SDSUPPORT, queue.inject(F("M24")));
            #endif
          }
          else {
            TERN_(HOST_ACTION_COMMANDS, hostui.resume());
          }
          Draw_Print_Screen();
        }
        else
          Popup_Handler(Pause);
        break;
      case PRINT_STOP: Popup_Handler(Stop); break;
    }
  }
  DWIN_UpdateLCD();
}

void CrealityDWINClass::Popup_Control() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_CW && selection < 1) {
    selection++;
    Popup_Select();
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW && selection > 0) {
    selection--;
    Popup_Select();
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (popup) {
      case Pause:
        if (selection == 0) {
          if (temp_val.sdprint) {
            #if ENABLED(POWER_LOSS_RECOVERY)
              if (recovery.enabled) recovery.save(true);
            #endif
            #if ENABLED(PARK_HEAD_ON_PAUSE)
              Popup_Handler(Home, true);
              #if ENABLED(SDSUPPORT)
                if (IS_SD_PRINTING()) card.pauseSDPrint();
              #endif
              planner.synchronize();
              queue.inject(F("M125"));
              planner.synchronize();
            #else
              queue.inject(F("M25"));
              TERN_(HAS_HOTEND, temp_val.pausetemp = thermalManager.temp_hotend[0].target);
              TERN_(HAS_HEATED_BED, temp_val.pausebed = thermalManager.temp_bed.target);
              TERN_(HAS_FAN, temp_val.pausefan = thermalManager.fan_speed[0]);
              thermalManager.cooldown();
            #endif
          }
          else {
            TERN_(HOST_ACTION_COMMANDS, hostui.pause());
          }
        }
        Draw_Print_Screen();
        break;
      case Stop:
        if (selection == 0) {
          if (temp_val.sdprint) {
            ui.abort_print();
            thermalManager.cooldown();
          }
          else {
            TERN_(HOST_ACTION_COMMANDS, hostui.cancel());
          }
        }
        else
          Draw_Print_Screen();
        break;
      case Resume:
        if (selection == 0)
          queue.inject(F("M1000"));
        else {
          queue.inject(F("M1000 C"));
          Draw_Main_Menu();
        }
        break;

      #if HAS_HOTEND
        case ETemp:
          if (selection == 0) {
            thermalManager.setTargetHotend(EXTRUDE_MINTEMP, 0);
            thermalManager.set_fan_speed(0, MAX_FAN_SPEED);
            Draw_Menu(PreheatHotend);
          }
          else
            Redraw_Menu(true, true, false);
          break;
      #endif

      #if HAS_BED_PROBE
        case ManualProbing:
          if (selection == 0) {
            char buf[80];
            const float dif = probe.probe_at_point(current_position.x, current_position.y, PROBE_PT_STOW, 0, false) - temp_val.corner_avg;
            sprintf_P(buf, dif > 0 ? PSTR("Corner is %smm high") : PSTR("Corner is %smm low"), dtostrf(abs(dif), 1, 3, str_1));
            Update_Status(buf);
          }
          else {
            Redraw_Menu(true, true, false);
            Update_Status("");
          }
          break;
      #endif

      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        case ConfFilChange:
          if (selection == 0) {
            if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp)
              Popup_Handler(ETemp);
            else {
              if (thermalManager.temp_hotend[0].is_below_target(-2)) {
                Popup_Handler(Heating);
                thermalManager.wait_for_hotend(0);
              }
              Popup_Handler(FilChange);
              sprintf_P(cmd, PSTR("M600 B1 R%i"), thermalManager.temp_hotend[0].target);
              gcode.process_subcommands_now(cmd);
            }
          }
          else
            Redraw_Menu(true, true, false);
          break;
        case PurgeMore:
          if (selection == 0) {
            pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;
            Popup_Handler(FilChange);
          }
          else {
            pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;
            if (temp_val.printing) Popup_Handler(Resuming);
            else Redraw_Menu(true, true, (active_menu==PreheatHotend));
          }
          break;
      #endif // ADVANCED_PAUSE_FEATURE

      case PrintConfirm:
        if (selection==0) {
          card.openAndPrintFile(card.filename);}
        else{
          Redraw_Menu(true, true, true);
          gcode.process_subcommands_now(F("M117"));}
        break;

      #if HAS_MESH
        case SaveLevel:
          if (selection == 0) {
            #if ENABLED(AUTO_BED_LEVELING_UBL)
              gcode.process_subcommands_now(F("G29 S"));
              planner.synchronize();
              AudioFeedback(true);
            #else
              AudioFeedback(settings.save());
            #endif
          }
          Draw_Menu(Leveling, LEVELING_GET_MESH);
          break;
      #endif

      #if ENABLED(AUTO_BED_LEVELING_UBL)
        case MeshSlot:
          if (selection == 0) bedlevel.storage_slot = 0;
          Redraw_Menu(true, true);
          break;
      #endif
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

void CrealityDWINClass::Confirm_Control() {
  EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (popup) {
      case Complete:
        Draw_Main_Menu();
        break;
      case FilInsert:
        Popup_Handler(FilChange);
        wait_for_user = false;
        break;
      case HeaterTime:
        Popup_Handler(Heating);
        Update_Status(GET_TEXT_F(MSG_HEATING));
        wait_for_user = false;
        break;
      #if HAS_ESDIAG
        case ESDiagPopup:
          wait_for_user = false;
          Redraw_Menu(true, true, false);
          break;
      #endif
      default:
        Redraw_Menu(true, true, false);
        wait_for_user = false;
        break;
    }
  }
  DWIN_UpdateLCD();
}

#if HAS_HOSTACTION_MENUS

  void CrealityDWINClass::Keyboard_Control() {
    const uint8_t keyboard_size = 34;
    static uint8_t key_selection = 0, cursor = 0;
    static char string[31];
    static bool uppercase = false, locked = false;
    if (reset_keyboard) {
      if (strcmp(stringpointer, "-") == 0) stringpointer[0] = '\0';
      key_selection = 0, cursor = strlen(stringpointer);
      uppercase = false, locked = false;
      reset_keyboard = false;
      strcpy(string, stringpointer);
    }
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    if (encoder_diffState == ENCODER_DIFF_CW && key_selection < keyboard_size) {
      Draw_Keys(key_selection, false, uppercase, locked);
      key_selection++;
      Draw_Keys(key_selection, true, uppercase, locked);
    }
    else if (encoder_diffState == ENCODER_DIFF_CCW && key_selection > 0) {
      Draw_Keys(key_selection, false, uppercase, locked);
      key_selection--;
      Draw_Keys(key_selection, true, uppercase, locked);
    }
    else if (encoder_diffState == ENCODER_DIFF_ENTER) {
      if (key_selection < 28) {
        if (key_selection == 19) {
          if (!numeric_keyboard) {
            if (locked) {
              uppercase = false, locked = false;
              Draw_Keyboard(keyboard_restrict, false, key_selection, uppercase, locked);
            } else if (uppercase) {
              locked = true;
              Draw_Keyboard(keyboard_restrict, false, key_selection, uppercase, locked);
            }
            else {
              uppercase = true;
              Draw_Keyboard(keyboard_restrict, false, key_selection, uppercase, locked);
            }
          }
        }
        else if (key_selection == 27) {
          cursor--;
          string[cursor] = '\0';
        }
        else {
          uint8_t index = key_selection;
          if (index > 19) index--;
          if (index > 27) index--;
          const char *keys;
          if (numeric_keyboard) keys = "1234567890&<>() {}[]*\"\':;!?";
          else keys = (uppercase) ? "QWERTYUIOPASDFGHJKLZXCVBNM" : "qwertyuiopasdfghjklzxcvbnm";
          if (!(keyboard_restrict && numeric_keyboard && index > 9)) {
            string[cursor] = keys[index];
            cursor++;
            string[cursor] = '\0';
          }
          if (!locked && uppercase) {
            uppercase = false;
            Draw_Keyboard(keyboard_restrict, false, key_selection, uppercase, locked);
          }
        }
      }
      else {
        switch (key_selection) {
          case 28:
            if (!numeric_keyboard) uppercase = false, locked = false;
            Draw_Keyboard(keyboard_restrict, !numeric_keyboard, key_selection, uppercase, locked);
            break;
          case 29:
            string[cursor] = '-';
            cursor++;
            string[cursor] = '\0';
            break;
          case 30:
            string[cursor] = '_';
            cursor++;
            string[cursor] = '\0';
            break;
          case 31:
            if (!keyboard_restrict) {
              string[cursor] = ' ';
              cursor++;
              string[cursor] = '\0';
            }
            break;
          case 32:
            if (!keyboard_restrict) {
              string[cursor] = '.';
              cursor++;
              string[cursor] = '\0';
            }
            break;
          case 33:
            if (!keyboard_restrict) {
              string[cursor] = '/';
              cursor++;
              string[cursor] = '\0';
            }
            break;
          case 34:
            if (string[0] == '\0') strcpy(string, "-");
            strcpy(stringpointer, string);
            process = Menu;
            DWIN_Draw_Rectangle(1, Def_Background_Color, 0, KEY_Y_START, DWIN_WIDTH-2, DWIN_HEIGHT-2);
            Draw_Status_Area(true);
            Update_Status_Bar(true);
            break;
        }
      }
      if (strlen(string) > maxstringlen) string[maxstringlen] = '\0', cursor = maxstringlen;
      Draw_String(string, selection, (process==Keyboard), (maxstringlen > 10));
    }
    DWIN_UpdateLCD();
  }

#endif // HAS_HOSTACTION_MENUS

/* In-Menu Value Modification */

void CrealityDWINClass::Setup_Value(float value, float min, float max, float unit, uint8_t type) {
  if (TERN0(HAS_HOTEND, valuepointer == &thermalManager.temp_hotend[0].pid.Ki) || TERN0(HAS_HEATED_BED, valuepointer == &thermalManager.temp_bed.pid.Ki))
    tempvalue = unscalePID_i(value) * unit;
  else if (TERN0(HAS_HOTEND, valuepointer == &thermalManager.temp_hotend[0].pid.Kd) || TERN0(HAS_HEATED_BED, valuepointer == &thermalManager.temp_bed.pid.Kd))
    tempvalue = unscalePID_d(value) * unit;
  else
    tempvalue = value * unit;
  valuemin = min;
  valuemax = max;
  valueunit = unit;
  valuetype = type;
  process = Value;
  EncoderRate.enabled = true;
  Draw_Float(tempvalue / unit, selection - scrollpos, true, valueunit);
}

void CrealityDWINClass::Modify_Value(float &value, float min, float max, float unit, void (*f)()/*=nullptr*/) {
  valuepointer = &value;
  funcpointer = f;
  Setup_Value((float)value, min, max, unit, 0);
}
void CrealityDWINClass::Modify_Value(uint8_t &value, float min, float max, float unit, void (*f)()/*=nullptr*/) {
  valuepointer = &value;
  funcpointer = f;
  Setup_Value((float)value, min, max, unit, 1);
}
void CrealityDWINClass::Modify_Value(uint16_t &value, float min, float max, float unit, void (*f)()/*=nullptr*/) {
  valuepointer = &value;
  funcpointer = f;
  Setup_Value((float)value, min, max, unit, 2);
}
void CrealityDWINClass::Modify_Value(int16_t &value, float min, float max, float unit, void (*f)()/*=nullptr*/) {
  valuepointer = &value;
  funcpointer = f;
  Setup_Value((float)value, min, max, unit, 3);
}
void CrealityDWINClass::Modify_Value(uint32_t &value, float min, float max, float unit, void (*f)()/*=nullptr*/) {
  valuepointer = &value;
  funcpointer = f;
  Setup_Value((float)value, min, max, unit, 4);
}
void CrealityDWINClass::Modify_Value(int8_t &value, float min, float max, float unit, void (*f)()/*=nullptr*/) {
  valuepointer = &value;
  funcpointer = f;
  Setup_Value((float)value, min, max, unit, 5);
}

void CrealityDWINClass::Modify_Option(uint8_t value, const char * const * options, uint8_t max) {
  tempvalue = value;
  valuepointer = const_cast<const char * *>(options);
  valuemin = 0;
  valuemax = max;
  process = Option;
  EncoderRate.enabled = true;
  Draw_Option(value, options, selection - scrollpos, true);
}

#if HAS_HOSTACTION_MENUS
  void CrealityDWINClass::Modify_String(char * string, uint8_t maxlength, bool restrict) {
    stringpointer = string;
    maxstringlen = maxlength;
    reset_keyboard = true;
    Draw_Keyboard(restrict, false);
    Draw_String(string, selection, true, (maxstringlen > 10));
  }
#endif

/* Main Functions */

void CrealityDWINClass::Update_Print_Filename(const char * const text) {
  LOOP_L_N(i, _MIN((size_t)LONG_FILENAME_LENGTH, strlen(text))) filename[i] = text[i];
  filename[_MIN((size_t)LONG_FILENAME_LENGTH - 1, strlen(text))] = '\0';
  Draw_Print_Filename(true);
}

void CrealityDWINClass::Update_Status(const char * const text) {
  LOOP_L_N(i, _MIN((size_t)64, strlen(text))) statusmsg[i] = text[i];
  statusmsg[_MIN((size_t)64, strlen(text))] = '\0';
}

void CrealityDWINClass::Update_Status(FSTR_P text) {
  Update_Status(FTOP(text));
}

void CrealityDWINClass::Start_Print(bool sd) {
  temp_val.sdprint = sd;
  if (!temp_val.printing) {
    temp_val.printing = true;
    statusmsg[0] = '\0';
    if (sd) {
      #if ENABLED(POWER_LOSS_RECOVERY)
        if (recovery.valid()) {
          SdFile *diveDir = nullptr;
          const char * const fname = card.diveToFile(true, diveDir, recovery.info.sd_filename);
          card.selectFileByName(fname);
        }
      #endif
      strcpy(filename, card.longest_filename());
    }
    TERN_(LCD_SET_PROGRESS_MANUALLY, ui.set_progress(0));
    TERN_(USE_M73_REMAINING_TIME, ui.set_remaining_time(0));
    Draw_Print_Screen();
  }
}

void CrealityDWINClass::Stop_Print() {
  temp_val.printing = false;
  temp_val.sdprint = false;
  thermalManager.cooldown();
  TERN_(LCD_SET_PROGRESS_MANUALLY, ui.set_progress(100 * (PROGRESS_SCALE)));
  TERN_(USE_M73_REMAINING_TIME, ui.set_remaining_time(0));
  Draw_PrintDone_confirm();
  filename[0] = '\0';
}

void CrealityDWINClass::Update() {
  State_Update();
  Screen_Update();
  switch (process) {
    case Main:    Main_Menu_Control();    break;
    case Menu:    Menu_Control();         break;
    case Value:   Value_Control();        break;
    case Option:  Option_Control();       break;
    case File:    File_Control();         break;
    case Print:   Print_Screen_Control(); break;
    case Popup:   Popup_Control();        break;
    case Confirm: Confirm_Control();      break;
    #if HAS_HOSTACTION_MENUS
      case Keyboard: Keyboard_Control();  break;
    #endif
    case Cancel:  Confirm_Control();      break;
    #if HAS_LOCKSCREEN
      case Locked: HMI_LockScreen();      break;
    #endif
  }
}

void MarlinUI::update() { CrealityDWIN.Update(); }

#if HAS_LCD_BRIGHTNESS
  void MarlinUI::_set_brightness() { DWIN_LCD_Brightness(backlight ? brightness : 0); }
#endif

void CrealityDWINClass::State_Update() {
  if ((print_job_timer.isRunning() || print_job_timer.isPaused()) != temp_val.printing) {
    if (!temp_val.printing) Start_Print(card.isFileOpen() || TERN0(POWER_LOSS_RECOVERY, recovery.valid()));
    else Stop_Print();
  }
  if (print_job_timer.isPaused() != temp_val.paused) {
    temp_val.paused = print_job_timer.isPaused();
    if (process == Print) Print_Screen_Icons();
    if (process == Wait && !temp_val.paused) Redraw_Menu(true, true);
  }
  if (wait_for_user && !(process == Confirm) && !print_job_timer.isPaused())
    Confirm_Handler(UserInput);
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    if (process == Popup && popup == PurgeMore) {
      if (pause_menu_response == PAUSE_RESPONSE_EXTRUDE_MORE)
        Popup_Handler(FilChange);
      else if (pause_menu_response == PAUSE_RESPONSE_RESUME_PRINT) {
        if (temp_val.printing) Popup_Handler(Resuming);
        else Redraw_Menu(true, true, (active_menu==PreheatHotend));
      }
    }
  #endif
  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    static bool ranout = false;
    if (runout.filament_ran_out != ranout) {
      ranout = runout.filament_ran_out;
      if (ranout) Popup_Handler(Runout);
    }
  #endif
}

void CrealityDWINClass::Screen_Update() {
  const millis_t ms = millis();
  static millis_t scrltime = 0;
  if (ELAPSED(ms, scrltime)) {
    scrltime = ms + 200;
    if (process != Keyboard) Update_Status_Bar();
    if (process == Print) Draw_Print_Filename();
  }

  static millis_t statustime = 0;
  if (ELAPSED(ms, statustime) && process != Keyboard) {
    statustime = ms + 500;
    Draw_Status_Area();
    #if HAS_ESDIAG
      if (process == Confirm && popup == ESDiagPopup) ESDiag.Update();
    #endif
    #if HAS_PIDPLOT
      if (process == Wait && (popup == PIDWaitH || popup == PIDWaitB)) Plot.Update((popup == PIDWaitH) ? thermalManager.wholeDegHotend(0) : thermalManager.wholeDegBed());
    #endif
  }

  static millis_t printtime = 0;
  if (ELAPSED(ms, printtime)) {
    printtime = ms + 1000;
    if (process == Print) {
      Draw_Print_ProgressBar();
      Draw_Print_ProgressElapsed();
      TERN_(USE_M73_REMAINING_TIME, Draw_Print_ProgressRemain());
    }
  }

  static bool mounted = card.isMounted();
  if (mounted != card.isMounted()) {
    mounted = card.isMounted();
    if (process == File)
      Draw_SD_List();
  }

  #if HAS_HOTEND
    static int16_t hotendtarget = -1;
  #endif
  #if HAS_HEATED_BED
    static int16_t bedtarget = -1;
  #endif
  #if HAS_FAN
    static int16_t fanspeed = -1;
  #endif

  #if HAS_ZOFFSET_ITEM
    static float lastzoffset = temp_val.zoffsetvalue;
    if (temp_val.zoffsetvalue != lastzoffset) {
      lastzoffset = temp_val.zoffsetvalue;
      #if HAS_BED_PROBE
        probe.offset.z = temp_val.zoffsetvalue;
      #else
        set_home_offset(Z_AXIS, -temp_val.zoffsetvalue);
      #endif
    }

    #if HAS_BED_PROBE
      if (probe.offset.z != lastzoffset)
        temp_val.zoffsetvalue = lastzoffset = probe.offset.z;
    #else
      if (-home_offset.z != lastzoffset)
        temp_val.zoffsetvalue = lastzoffset = -home_offset.z;
    #endif
  #endif // HAS_ZOFFSET_ITEM

  if (process == Menu || process == Value) {
    switch (active_menu) {
      case TempMenu:
        #if HAS_HOTEND
          if (thermalManager.temp_hotend[0].target != hotendtarget) {
            hotendtarget = thermalManager.temp_hotend[0].target;
            if (scrollpos <= TEMP_HOTEND && TEMP_HOTEND <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND - scrollpos)
                Draw_Float(thermalManager.temp_hotend[0].target, TEMP_HOTEND - scrollpos, false, 1);
            }
          }
        #endif
        #if HAS_HEATED_BED
          if (thermalManager.temp_bed.target != bedtarget) {
            bedtarget = thermalManager.temp_bed.target;
            if (scrollpos <= TEMP_BED && TEMP_BED <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND - scrollpos)
                Draw_Float(thermalManager.temp_bed.target, TEMP_BED - scrollpos, false, 1);
            }
          }
        #endif
        #if HAS_FAN
          if (thermalManager.fan_speed[0] != fanspeed) {
            fanspeed = thermalManager.fan_speed[0];
            if (scrollpos <= TEMP_FAN && TEMP_FAN <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND - scrollpos)
                Draw_Float(thermalManager.fan_speed[0], TEMP_FAN - scrollpos, false, 1);
            }
          }
        #endif
        break;
      case Tune:
        #if HAS_HOTEND
          if (thermalManager.temp_hotend[0].target != hotendtarget) {
            hotendtarget = thermalManager.temp_hotend[0].target;
            if (scrollpos <= TUNE_HOTEND && TUNE_HOTEND <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND - scrollpos)
                Draw_Float(thermalManager.temp_hotend[0].target, TUNE_HOTEND - scrollpos, false, 1);
            }
          }
        #endif
        #if HAS_HEATED_BED
          if (thermalManager.temp_bed.target != bedtarget) {
            bedtarget = thermalManager.temp_bed.target;
            if (scrollpos <= TUNE_BED && TUNE_BED <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND - scrollpos)
                Draw_Float(thermalManager.temp_bed.target, TUNE_BED - scrollpos, false, 1);
            }
          }
        #endif
        #if HAS_FAN
          if (thermalManager.fan_speed[0] != fanspeed) {
            fanspeed = thermalManager.fan_speed[0];
            if (scrollpos <= TUNE_FAN && TUNE_FAN <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND - scrollpos)
                Draw_Float(thermalManager.fan_speed[0], TUNE_FAN - scrollpos, false, 1);
            }
          }
        #endif
        break;
    }
  }
}

void CrealityDWINClass::AudioFeedback(const bool success/*=true*/) {
  if (ui.sound_on)
    DONE_BUZZ(success);
  else
    Update_Status(success ? "Success" : "Failed");
}

void CrealityDWINClass::Save_Settings(char *buff) {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("Save_Settings"));
  TERN_(AUTO_BED_LEVELING_UBL, eeprom_settings.tilt_grid_size = mesh_conf.tilt_grid - 1);
  eeprom_settings.corner_pos = temp_val.corner_pos * 10;
  #if HAS_HOSTACTION_MENUS
    eeprom_settings.host_action_label_1 = Encode_String(action1);
    eeprom_settings.host_action_label_2 = Encode_String(action2);
    eeprom_settings.host_action_label_3 = Encode_String(action3);
  #endif
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("eeprom_settings size: ", sizeof(eeprom_settings_t)));
  memcpy(buff, &eeprom_settings, _MIN(sizeof(eeprom_settings), eeprom_data_size));
}

void CrealityDWINClass::Load_Settings(const char *buff) {
  memcpy(&eeprom_settings, buff, _MIN(sizeof(eeprom_settings), eeprom_data_size));
  TERN_(AUTO_BED_LEVELING_UBL, mesh_conf.tilt_grid = eeprom_settings.tilt_grid_size + 1);
  if (eeprom_settings.corner_pos == 0) eeprom_settings.corner_pos = 325;
  temp_val.corner_pos = eeprom_settings.corner_pos / 10.0f;
  #if ENABLED(BAUD_RATE_GCODE)
    if (eeprom_settings.Baud115k) queue.inject(F("M575 P0 B115200"));
  #endif
  #if ENABLED(FWRETRACT)
    temp_val.auto_fw_retract = fwretract.autoretract_enabled;
  #endif
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    temp_val.LevelingTempmode = 2 * !eeprom_settings.ena_hotend_levtemp + !eeprom_settings.ena_bed_levtemp;
  #endif
  #if ENABLED(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    leds.set_color(
    (temp_val.LED_Color >> 16) & 0xFF,
    (temp_val.LED_Color >>  8) & 0xFF,
    (temp_val.LED_Color >>  0) & 0xFF
    OPTARG(HAS_WHITE_LED, (temp_val.LED_Color >> 24) & 0xFF)
    );
  #endif
  #if HAS_HOSTACTION_MENUS
    Decode_String(eeprom_settings.host_action_label_1, action1);
    Decode_String(eeprom_settings.host_action_label_2, action2);
    Decode_String(eeprom_settings.host_action_label_3, action3);
  #endif
  Redraw_Screen();
  #if ENABLED(POWER_LOSS_RECOVERY)
    static bool init = true;
    if (init) {
      init = false;
      queue.inject(F("M1000 S"));
    }
  #endif
}

void CrealityDWINClass::Reset_Settings() {
  eeprom_settings.time_format_textual = false;
  TERN_(AUTO_BED_LEVELING_UBL, eeprom_settings.tilt_grid_size = 0);
  eeprom_settings.corner_pos = 325;
  eeprom_settings.cursor_color = 0;
  eeprom_settings.menu_split_line = 0;
  eeprom_settings.menu_top_bg = 0;
  eeprom_settings.menu_top_txt = 0;
  eeprom_settings.highlight_box = 0;
  eeprom_settings.progress_percent = 0;
  eeprom_settings.progress_time = 0;
  eeprom_settings.status_bar_text = 0;
  eeprom_settings.status_area_text = 0;
  eeprom_settings.coordinates_text = 0;
  eeprom_settings.coordinates_split_line = 0;
  TERN_(AUTO_BED_LEVELING_UBL, mesh_conf.tilt_grid = eeprom_settings.tilt_grid_size + 1);
  temp_val.corner_pos = eeprom_settings.corner_pos / 10.0f;
  TERN_(SOUND_MENU_ITEM, ui.sound_on = ENABLED(SOUND_ON_DEFAULT));
  TERN_(BAUD_RATE_GCODE, eeprom_settings.Baud115k = false);
  TERN_(FWRETRACT, temp_val.auto_fw_retract = fwretract.autoretract_enabled);
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    eeprom_settings.ena_hotend_levtemp = true;
    eeprom_settings.ena_bed_levtemp = true;
    eeprom_settings.hotend_levtemp = LEVELING_NOZZLE_TEMP;
    eeprom_settings.bed_levtemp = LEVELING_BED_TEMP;
  #endif
  #if ENABLED(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    leds.setup();
    #if ENABLED(LED_COLOR_PRESETS)
      leds.set_default();
    #endif
      temp_val.LED_Color = Def_Leds_Color;
      leds.set_color(
      (temp_val.LED_Color >> 16) & 0xFF,
      (temp_val.LED_Color >>  8) & 0xFF,
      (temp_val.LED_Color >>  0) & 0xFF
      OPTARG(HAS_WHITE_LED, (temp_val.LED_Color >> 24) & 0xFF)
      );
  #endif
  #if HAS_HOSTACTION_MENUS
    eeprom_settings.host_action_label_1 = 0;
    eeprom_settings.host_action_label_2 = 0;
    eeprom_settings.host_action_label_3 = 0;
    action1[0] = action2[0] = action3[0] = '-';
  #endif
  Redraw_Screen();
}

void CrealityDWINClass::PreheatBefore() {
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    Popup_Handler(Heating);
    #if HAS_BED_PROBE
      probe.preheat_for_probing(eeprom_settings.ena_hotend_levtemp, eeprom_settings.ena_bed_levtemp);
    #else
      #if HAS_HOTEND
        if (thermalManager.degTargetHotend(0) < eeprom_settings.hotend_levtemp && (eeprom_settings.ena_hotend_levtemp))
          thermalManager.setTargetHotend(eeprom_settings.hotend_levtemp, 0);
      #endif
      #if HAS_HEATED_BED
        if (thermalManager.degTargetBed() < eeprom_settings.bed_levtemp && (eeprom_settings.ena_bed_levtemp))
          thermalManager.setTargetBed(eeprom_settings.bed_levtemp);
      #endif
      TERN_(HAS_HOTEND, if (eeprom_settings.ena_hotend_levtemp) thermalManager.wait_for_hotend(0));
      TERN_(HAS_HEATED_BED, if (eeprom_settings.ena_bed_levtemp) thermalManager.wait_for_bed_heating());
    #endif
    Update_Status("");
  #endif
}

void MarlinUI::init_lcd() {
  DWINUI::init();
  Encoder_Configuration();
  DWIN_JPG_ShowAndCache(0);
  for (uint16_t t = 0; t <= 100; t += 2) {
    DWINUI::DRAW_IconWB(ICON, ICON_Bar, 15, 260);
    DWIN_Draw_Rectangle(1, Def_Background_Color, 15 + t * 242 / 100, 260, 257, 280);
    DWIN_UpdateLCD();
    delay(20);
  }
  DWIN_JPG_CacheTo1(Language_English);
  CrealityDWIN.Redraw_Screen();
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void MarlinUI::pause_show_message(const PauseMessage message, const PauseMode mode/*=PAUSE_MODE_SAME*/, const uint8_t extruder/*=active_extruder*/) {
    switch (message) {
      case PAUSE_MESSAGE_INSERT:  CrealityDWIN.Confirm_Handler(FilInsert);  break;
      case PAUSE_MESSAGE_PURGE:   break;
      case PAUSE_MESSAGE_OPTION:  pause_menu_response = PAUSE_RESPONSE_WAIT_FOR; CrealityDWIN.Popup_Handler(PurgeMore); break;
      case PAUSE_MESSAGE_HEAT:    CrealityDWIN.Confirm_Handler(HeaterTime); break;
      case PAUSE_MESSAGE_WAITING: CrealityDWIN.Draw_Print_Screen();         break;
      default: break;
    }
  }
#endif

// End-stops diagnostic from DWIN PROUI
#if HAS_ESDIAG
  void CrealityDWINClass::DWIN_EndstopsDiag() {
    last_process = process;
    last_selection = selection;
    process = Confirm;
    popup = ESDiagPopup;
    ESDiag.Draw();
  }
#endif

// Lock screen from DWIN PROUI
#if HAS_LOCKSCREEN
  void CrealityDWINClass::DWIN_LockScreen() {
    if (process != Locked) {
      lockScreen.rprocess = process;
      process = Locked;
      lockScreen.init();
    }
  }

  void CrealityDWINClass::DWIN_UnLockScreen() {
    if (process == Locked) {
      process = lockScreen.rprocess;
      if (!temp_val.printing) Draw_Main_Menu(); else Draw_Print_Screen();
    }
  }

  void CrealityDWINClass::HMI_LockScreen() {
    EncoderState encoder_diffState = Encoder_ReceiveAnalyze();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    lockScreen.onEncoder(encoder_diffState);
    if (lockScreen.isUnlocked()) DWIN_UnLockScreen();
  }
#endif

// Reboot screen from DWIN PROUI
void CrealityDWINClass::DWIN_RebootScreen() {
  DWIN_Frame_Clear(Def_Background_Color);
  DWIN_JPG_ShowAndCache(0);
  DWINUI::Draw_CenteredString(Def_Text_Color, 220, GET_TEXT_F(MSG_PLEASE_WAIT_REBOOT));
  DWIN_UpdateLCD();
  delay(500);
}

// Reboot Printer from DWIN PROUI
void CrealityDWINClass::RebootPrinter() {
  wait_for_heatup = wait_for_user = false;    // Stop waiting for heating/user
  thermalManager.disable_all_heaters();
  planner.finish_and_disable();
  DWIN_RebootScreen();
  hal.reboot();
}

#if ENABLED(LED_CONTROL_MENU, HAS_COLOR_LEDS)
  void CrealityDWINClass::ApplyLEDColor() {
    temp_val.LED_Color = TERN0(HAS_WHITE_LED,(leds.color.w << 24)) | (leds.color.r << 16) | (leds.color.g << 8) | (leds.color.b);
  }
#endif

#if HAS_PIDPLOT
  void CrealityDWINClass::DWIN_Draw_PIDPopup(const pidresult_t pidresult) {
    frame_rect_t gfrm = {40, 160, DWIN_WIDTH - 80, 150};
    DWINUI::ClearMainArea();
    DWIN_Draw_Rectangle(1, Def_PopupBg_color, 14, 60, 258, 330);
    DWIN_Draw_Rectangle(0, Def_Highlight_Color, 14, 60, 258, 330);
    DWINUI::Draw_CenteredString(Def_PopupTxt_Color, 80, GET_TEXT_F(MSG_PID_AUTOTUNE));
    DWINUI::Draw_String(Def_PopupTxt_Color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("PID target:    Celsius"));
    switch (pidresult) {
      case PID_EXTR_START:
        DWINUI::Draw_CenteredString(Def_PopupTxt_Color, 100, F("for Nozzle is running."));
        Plot.Draw(gfrm, thermalManager.hotend_maxtemp[0], temp_val.PID_e_temp);
        DWINUI::Draw_Int(Def_PopupTxt_Color, 3, gfrm.x + 90, gfrm.y - DWINUI::fontHeight() - 4, temp_val.PID_e_temp);
        break;
      case PID_BED_START:
        DWINUI::Draw_CenteredString(Def_PopupTxt_Color, 100, F("for BED is running."));
        Plot.Draw(gfrm, BED_MAXTEMP, temp_val.PID_bed_temp);
        DWINUI::Draw_Int(Def_PopupTxt_Color, 3, gfrm.x + 90, gfrm.y - DWINUI::fontHeight() - 4, temp_val.PID_bed_temp);
        break;
      default:
        break;
    }
  }
#endif

#if HAS_PID_HEATING
  void CrealityDWINClass::DWIN_PidTuning(const pidresult_t pidresult) {
    switch (pidresult) {
      case PID_STARTED:  break;
      #if HAS_PIDPLOT
        case PID_EXTR_START:  last_process = process; last_selection = selection; process = Wait; popup = PIDWaitH; DWIN_Draw_PIDPopup(pidresult); break;
        case PID_BED_START:  last_process = process; last_selection = selection; process = Wait; popup = PIDWaitB; DWIN_Draw_PIDPopup(pidresult); break;
      #else
        case PID_EXTR_START:  Popup_Handler(PIDWait); break;
        case PID_BED_START:  Popup_Handler(PIDWait, true); break;
      #endif
      case PID_BAD_EXTRUDER_NUM:  Confirm_Handler(BadextruderNumber);  break;
      case PID_TEMP_TOO_HIGH:  Confirm_Handler(TempTooHigh);  break;
      case PID_TUNING_TIMEOUT:  Confirm_Handler(PIDTimeout);  break;
      case PID_DONE: Confirm_Handler(PIDDone);  break;
      default: break;
    }
  }
#endif

#endif // DWIN_CREALITY_LCD_JYERSUI
