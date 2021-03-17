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
 * Rewrite and Extui Port by Jacob Myers
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(CREALITY_DWIN_EXTUI)

#include "creality_dwin.h"
#include "../../ui_api.h"

#include "../../../marlinui.h"
#include "../../MarlinCore.h"

#include "../../module/temperature.h"
#include "../../module/planner.h"
#include "../../module/settings.h"
#include "../../libs/buzzer.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../feature/pause.h"
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #include "../../feature/runout.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../feature/host_actions.h"
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

#if ANY(HAS_ONESTEP_LEVELING, AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../libs/least_squares_fit.h"
  #include "../../libs/vector_3.h"
#endif

#if ENABLED(HAS_BED_PROBE)
  #include "../../module/probe.h"
#endif

#if ANY(HAS_HOTEND, HAS_HEATED_BED, HAS_FAN) && PREHEAT_COUNT
  #define HAS_PREHEAT 1
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/powerloss.h"
#endif

#define MACHINE_SIZE STRINGIFY(X_BED_SIZE) "x" STRINGIFY(Y_BED_SIZE) "x" STRINGIFY(Z_MAX_POS)

#define CORP_WEBSITE_E "github.com/Jyers"

#define BUILD_NUMBER "1.2.2"

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

constexpr uint16_t TROWS = 6, MROWS = TROWS - 1,
                   TITLE_HEIGHT = 30,
                   MLINE = 53,
                   LBLX = 60,
                   MENU_CHR_W = 8, MENU_CHR_H = 16, STAT_CHR_W = 10;

#define MBASE(L) (49 + MLINE * (L))

constexpr float default_max_feedrate[]        = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[]    = DEFAULT_MAX_ACCELERATION;
constexpr float default_steps[]               = DEFAULT_AXIS_STEPS_PER_UNIT;
#if HAS_CLASSIC_JERK
  constexpr float default_max_jerk[]            = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
#endif

uint8_t active_menu = MainMenu;
uint8_t last_menu = MainMenu;
uint8_t selection = 0;
uint8_t last_selection = 0;
uint8_t scrollpos = 0;
uint8_t process = Main;
uint8_t last_process = Main;
uint8_t popup;
uint8_t last_popup;

void *valuepointer;
float tempvalue;
float valuemin;
float valuemax;
uint8_t valueunit;
uint8_t valuetype;

char statusmsg[64];
char filename[LONG_FILENAME_LENGTH];
bool printing = false;
bool paused = false;
bool sdprint = false;

int16_t pausetemp, pausebed, pausefan;

bool liveadjust = false;
bool bedonly = false;
float zoffsetvalue = 0;
uint8_t gridpoint;

bool probe_deployed = false;

CrealityDWINClass CrealityDWIN;

#if ENABLED(AUTO_BED_LEVELING_UBL)
  struct UBL_Settings {
    bool viewer_asymmetric_range = false;
    bool viewer_print_value = false;
    uint8_t tilt_grid = 2;
    bool goto_mesh_value = false;
    bool mesh_step_warning = false;
    bool mesh_goto_zhop = true;
    uint8_t mesh_x = 0;
    uint8_t mesh_y = 0;

    void manual_move() {
      CrealityDWIN.Popup_Handler(MoveWait);
      char buf[20];
      sprintf(buf, "G0 F300 Z%.3f", (!goto_mesh_value ? Z_CLEARANCE_BETWEEN_PROBES : (mesh_goto_zhop ? 1. : 0.)));
      gcode.process_subcommands_now_P(buf);
      sprintf(buf, "G42 F4000 I%i J%i", mesh_x, mesh_y);
      gcode.process_subcommands_now_P(buf);
      sprintf(buf, "G0 F300 Z%.3f", goto_mesh_value ? .0 : Z_CLEARANCE_BETWEEN_PROBES);
      gcode.process_subcommands_now_P(buf);
      planner.synchronize();
      CrealityDWIN.Redraw_Menu();
    }

    void manual_value_update(bool undefined=false) {
      char buf[32];
      sprintf(buf, "M421 I%i J%i Z%.3f %s", mesh_x, mesh_y, current_position.z, undefined ? "N" : "");
      gcode.process_subcommands_now_P(buf);
      planner.synchronize();
    }

    float get_max_value() {
    float max = __FLT_MIN__;
    GRID_LOOP(x, y) {
      if (!isnan(ubl.z_values[x][y]) && ubl.z_values[x][y] > max)
        max = ubl.z_values[x][y];
      }
      return max;
    }

    float get_min_value() {
      float min = __FLT_MAX__;
      GRID_LOOP(x, y) {
        if (!isnan(ubl.z_values[x][y]) && ubl.z_values[x][y] < min)
          min = ubl.z_values[x][y];
      }
      return min;
    }

    bool create_plane_from_mesh() {
      struct linear_fit_data lsf_results;
      incremental_LSF_reset(&lsf_results);
      GRID_LOOP(x, y) {
        if (!isnan(ubl.z_values[x][y])) {
          xy_pos_t rpos;
          rpos.x = ubl.mesh_index_to_xpos(x);
          rpos.y = ubl.mesh_index_to_ypos(y);
          incremental_LSF(&lsf_results, rpos, ubl.z_values[x][y]);
        }
      }
      
      if (finish_incremental_LSF(&lsf_results)) {
        SERIAL_ECHOPGM("Could not complete LSF!");
        return true;
      }

      ubl.set_all_mesh_points_to_value(0);

      matrix_3x3 rotation = matrix_3x3::create_look_at(vector_3(lsf_results.A, lsf_results.B, 1));
      GRID_LOOP(i, j) {
        float mx = ubl.mesh_index_to_xpos(i),
              my = ubl.mesh_index_to_ypos(j),
              mz = ubl.z_values[i][j];

        if (DEBUGGING(LEVELING)) {
          DEBUG_ECHOPAIR_F("before rotation = [", mx, 7);
          DEBUG_CHAR(',');
          DEBUG_ECHO_F(my, 7);
          DEBUG_CHAR(',');
          DEBUG_ECHO_F(mz, 7);
          DEBUG_ECHOPGM("]   ---> ");
          DEBUG_DELAY(20);
        }

        apply_rotation_xyz(rotation, mx, my, mz);

        if (DEBUGGING(LEVELING)) {
          DEBUG_ECHOPAIR_F("after rotation = [", mx, 7);
          DEBUG_CHAR(',');
          DEBUG_ECHO_F(my, 7);
          DEBUG_CHAR(',');
          DEBUG_ECHO_F(mz, 7);
          DEBUG_ECHOLNPGM("]");
          DEBUG_DELAY(20);
        }

        ubl.z_values[i][j] = mz - lsf_results.D;
        ExtUI::onMeshUpdate(i, j, ubl.z_values[i][j]);
      }
      return false;
    }

  };
  UBL_Settings ubl_conf = UBL_Settings();
#endif


/* General Display Functions */

// Clear a part of the screen
//  4=Entire screen
//  3=Title bar and Menu area (default)
//  2=Menu area
//  1=Title bar
inline void CrealityDWINClass::Clear_Screen(uint8_t e/*=3*/) {
  if (e==1||e==3||e==4) DWIN_Draw_Rectangle(1, Color_Bg_Blue, 0, 0, DWIN_WIDTH, TITLE_HEIGHT); // Clear Title Bar
  if (e==2||e==3) DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 31, DWIN_WIDTH, STATUS_Y); // Clear Menu Area
  if (e==4) DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, 31, DWIN_WIDTH, DWIN_HEIGHT); // Clear Popup Area
}

inline void CrealityDWINClass::Draw_Float(float value, uint8_t row, bool selected/*=false*/, uint8_t minunit/*=10*/) {
  uint16_t bColor;
  if (selected) bColor = Select_Color;
  else bColor = Color_Bg_Black;
  if (isnan(value)) {
    DWIN_Draw_String(false, true, DWIN_FONT_MENU, Color_White, bColor, 196, MBASE(row), F(" NaN"));
  } else if (value < 0) {
    DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_MENU, Color_White, bColor, 5-log10(minunit), log10(minunit), 202, MBASE(row), -value * minunit);
    DWIN_Draw_String(false, true, DWIN_FONT_MENU, Color_White, bColor, 196, MBASE(row), F("-"));
  }
  else {
    DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_MENU, Color_White, bColor, 5-log10(minunit), log10(minunit), 202, MBASE(row), value * minunit);
    DWIN_Draw_String(false, true, DWIN_FONT_MENU, Color_White, bColor, 196, MBASE(row), F(" "));
  }
}

inline void CrealityDWINClass::Draw_Title(char *title) {
  DWIN_Draw_String(false, false, DWIN_FONT_HEAD, Color_White, Color_Bg_Blue, (DWIN_WIDTH - strlen(title) * STAT_CHR_W) / 2, 4, title);
}

inline void CrealityDWINClass::Draw_Menu_Item(uint8_t row, uint8_t icon/*=0*/, char *label1, char *label2, bool more/*=false*/, bool centered/*=false*/) {
  const uint8_t label_offset_y = !(label1 && label2) ? 0 : MENU_CHR_H * 3 / 5;
  const uint8_t label1_offset_x = !centered ? LBLX : LBLX * 4/5 + max(LBLX * 1U/5, (DWIN_WIDTH - LBLX - (label1 ? strlen(label1) : 0) * MENU_CHR_W) / 2);
  const uint8_t label2_offset_x = !centered ? LBLX : LBLX * 4/5 + max(LBLX * 1U/5, (DWIN_WIDTH - LBLX - (label2 ? strlen(label2) : 0) * MENU_CHR_W) / 2);
  if (label1) DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, label1_offset_x, MBASE(row) - 1 - label_offset_y, label1); // Draw Label
  if (label2) DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, label2_offset_x, MBASE(row) - 1 + label_offset_y, label2); // Draw Label
  if (icon) DWIN_ICON_Show(ICON, icon, 26, MBASE(row) - 3);   //Draw Menu Icon
  if (more) DWIN_ICON_Show(ICON, ICON_More, 226, MBASE(row) - 3); // Draw More Arrow
  DWIN_Draw_Line(Line_Color, 16, MBASE(row) + 33, 256, MBASE(row) + 34); // Draw Menu Line
}

inline void CrealityDWINClass::Draw_Checkbox(uint8_t row, bool value) {
  #if ENABLED(CREALITY_DWIN_EXTUI_CUSTOM_ICONS) // Draw appropriate checkbox icon
    DWIN_ICON_Show(ICON, (value ? ICON_Checkbox_T : ICON_Checkbox_F), 226, MBASE(row) - 3); 
  #else                                         // Draw a basic checkbox using rectangles and lines
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 226, MBASE(row) - 3, 226 + 20, MBASE(row) - 3 + 20);
    DWIN_Draw_Rectangle(0, Color_White, 226, MBASE(row) - 3, 226 + 20, MBASE(row) - 3 + 20);
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

inline void CrealityDWINClass::Draw_Menu(uint8_t menu, uint8_t select/*=0*/, uint8_t scroll/*=0*/) {
  if (active_menu!=menu) {
    last_menu = active_menu;
    if (process == Menu) last_selection = selection;
  }
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

inline void CrealityDWINClass::Redraw_Menu() {
  Draw_Menu(active_menu, selection, scrollpos);
}

/* Primary Menus and Screen Elements */

void CrealityDWINClass::Main_Menu_Icons() {
  if (selection == 0) {
    DWIN_ICON_Show(ICON, ICON_Print_1, 17, 130);
    DWIN_Draw_Rectangle(0, Color_White, 17, 130, 126, 229);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 52, 200, F("Print"));
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Print_0, 17, 130);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 52, 200, F("Print"));
  }
  if (selection == 1) {
    DWIN_ICON_Show(ICON, ICON_Prepare_1, 145, 130);
    DWIN_Draw_Rectangle(0, Color_White, 145, 130, 254, 229);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 170, 200, F("Prepare"));
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Prepare_0, 145, 130);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 170, 200, F("Prepare"));
  }
  if (selection == 2) {
    DWIN_ICON_Show(ICON, ICON_Control_1, 17, 246);
    DWIN_Draw_Rectangle(0, Color_White, 17, 246, 126, 345);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 43, 317, F("Control"));
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Control_0, 17, 246);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 43, 317, F("Control"));
  }
  #if ANY(HAS_ONESTEP_LEVELING, AUTO_BED_LEVELING_UBL, PROBE_MANUALLY) 
    if (selection == 3) {
      DWIN_ICON_Show(ICON, ICON_Leveling_1, 145, 246);
      DWIN_Draw_Rectangle(0, Color_White, 145, 246, 254, 345);
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 179, 317, F("Level"));
    }
    else {
      DWIN_ICON_Show(ICON, ICON_Leveling_0, 145, 246);
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 179, 317, F("Level"));
    }
  #else
    if (selection == 3) {
      DWIN_ICON_Show(ICON, ICON_Info_1, 145, 246);
      DWIN_Draw_Rectangle(0, Color_White, 145, 246, 254, 345);
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 181, 317, F("Info"));
    }
    else {
      DWIN_ICON_Show(ICON, ICON_Info_0, 145, 246);
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 181, 317, F("Info"));
      //DWIN_Frame_AreaCopy(1, 132, 423, 159, 435, 186, 318);
    }
  #endif
}

void CrealityDWINClass::Draw_Main_Menu(uint8_t select/*=0*/) {
  process = Main;
  active_menu = MainMenu;
  selection = select;
  Clear_Screen();
  Draw_Title(Get_Menu_Title(MainMenu));
  DWIN_ICON_Show(ICON, ICON_LOGO, 71, 72);
  Main_Menu_Icons();
}

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #define WIDE_VIEWER_TEXT (GRID_MAX_POINTS_X < 10)

  void CrealityDWINClass::Draw_Bed_Mesh(int16_t selected/*=-1*/, uint8_t gridline_width/*=1*/, uint16_t padding_x/*=8*/, uint16_t padding_y_top/*=40 + 53 - 7*/) {
    const uint16_t total_width_px = DWIN_WIDTH - padding_x - padding_x;
    const uint16_t cell_width_px  = total_width_px / GRID_MAX_POINTS_X;
    const uint16_t cell_height_px = total_width_px / GRID_MAX_POINTS_Y;
    const float v_max = abs(ubl_conf.get_max_value()), v_min = abs(ubl_conf.get_min_value()), range = max(v_min, v_max);

    // Clear background from previous selection and select new square
    DWIN_Draw_Rectangle(1, Color_Bg_Black, max(0, padding_x - gridline_width), max(0, padding_y_top - gridline_width), padding_x + total_width_px, padding_y_top + total_width_px);
    if (selected >= 0) {
      const auto selected_y = selected / GRID_MAX_POINTS_X;
      const auto selected_x = selected - (GRID_MAX_POINTS_X * selected_y);
      const auto start_y_px = padding_y_top + selected_y * cell_height_px;
      const auto start_x_px = padding_x + selected_x * cell_width_px;
      DWIN_Draw_Rectangle(1, Color_White, max(0, start_x_px - gridline_width), max(0, start_y_px - gridline_width), start_x_px + cell_width_px, start_y_px + cell_height_px);
    }

    // Draw value square grid
    char buf[8];
    GRID_LOOP(x, y) {
      const auto start_x_px = padding_x + x * cell_width_px;
      const auto end_x_px   = start_x_px + cell_width_px - 1 - gridline_width;
      const auto start_y_px = padding_y_top + (GRID_MAX_POINTS_Y - y - 1) * cell_height_px;
      const auto end_y_px   = start_y_px + cell_height_px - 1 - gridline_width;
      DWIN_Draw_Rectangle(1,        // RGB565 colors: http://www.barth-dev.de/online/rgb565-color-picker/
        isnan(ubl.z_values[x][y]) ? Color_Grey : (                                                              // gray if undefined
          (ubl.z_values[x][y] < 0 ? 
            (uint16_t)round(0b11111  * -ubl.z_values[x][y] / (!ubl_conf.viewer_asymmetric_range ? range : v_min)) << 11 : // red if mesh point value is negative
            (uint16_t)round(0b111111 *  ubl.z_values[x][y] / (!ubl_conf.viewer_asymmetric_range ? range : v_max)) << 5) | // green if mesh point value is positive
              min(0b11111, (((uint8_t)abs(ubl.z_values[x][y]) / 10) * 4))),                                     // + blue stepping for every mm
        start_x_px, start_y_px, end_x_px, end_y_px);
      while (LCD_SERIAL.availableForWrite() < 32) { // wait for serial to be available without blocking and resetting the MCU 
        gcode.process_subcommands_now_P("G4 P10");
        planner.synchronize();
      } 
      // Draw value text on 
      if (ubl_conf.viewer_print_value) { 
        gcode.process_subcommands_now_P("G4 P10");  // still fails without additional delay...
        planner.synchronize();
        int8_t offset_x, offset_y = cell_height_px / 2 - 6;
        if (isnan(ubl.z_values[x][y])) {  // undefined
          DWIN_Draw_String(false, false, font6x12, Color_White, Color_Bg_Blue, start_x_px + cell_width_px / 2 - 5, start_y_px + offset_y, F("X"));
        }
        else {                          // has value
          if (WIDE_VIEWER_TEXT) {
            sprintf(buf, "%.2f", abs(ubl.z_values[x][y]));
          }
          else {
            sprintf(buf, "%02i", (uint16_t)(abs(ubl.z_values[x][y] - (int16_t)ubl.z_values[x][y]) * 100));
          }
          offset_x = cell_width_px / 2 - 3 * (strlen(buf)) - 2;
          if (!WIDE_VIEWER_TEXT)
            DWIN_Draw_String(false, false, font6x12, Color_White, Color_Bg_Blue, start_x_px-2 + offset_x, start_y_px + offset_y /*+ square / 2 - 6*/, F("."));
          DWIN_Draw_String(false, false, font6x12, Color_White, Color_Bg_Blue, start_x_px+1 + offset_x, start_y_px + offset_y /*+ square / 2 - 6*/, buf);
        }
        
      }
    }
  }

  void CrealityDWINClass::Set_Mesh_Viewer_Status() { // TODO: draw gradient with values as a legend instead
    float v_max = abs(ubl_conf.get_max_value()), v_min = abs(ubl_conf.get_min_value()), range = max(v_min, v_max);
    if (v_min > 3e+10F) v_min = 0.0000001;
    if (v_max > 3e+10F) v_max = 0.0000001;
    if (range > 3e+10F) range = 0.0000001;
    char msg[32];
    if (ubl_conf.viewer_asymmetric_range) {
      sprintf(msg, "Red %.3f..0..%.3f Green", -v_min, v_max);
    }
    else {
      sprintf(msg, "Red %.3f..0..%.3f Green", -range, range);
    }
    Update_Status(msg);
  }
#endif // AUTO_BED_LEVELING_UBL

void CrealityDWINClass::Print_Screen_Icons() {
  if (selection == 0) {
    DWIN_ICON_Show(ICON, ICON_Setup_1, 8, 252);
    DWIN_Draw_Rectangle(0, Color_White, 8, 252, 87, 351);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 30, 322, F("Tune"));
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Setup_0, 8, 252);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 30, 322, F("Tune"));
  }
  if (selection == 2) {
    DWIN_ICON_Show(ICON, ICON_Stop_1, 184, 252);
    DWIN_Draw_Rectangle(0, Color_White, 184, 252, 263, 351);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 205, 322, F("Stop"));
  }
  else {
    DWIN_ICON_Show(ICON, ICON_Stop_0, 184, 252);
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 205, 322, F("Stop"));
  }
  if (paused) {
    if (selection == 1) {
      DWIN_ICON_Show(ICON, ICON_Continue_1, 96, 252);
      DWIN_Draw_Rectangle(0, Color_White, 96, 252, 175, 351);
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 114, 322, F("Print"));
    }
    else {
      DWIN_ICON_Show(ICON, ICON_Continue_0, 96, 252);
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 114, 322, F("Print"));
    }
  }
  else {
    if (selection == 1) {
      DWIN_ICON_Show(ICON, ICON_Pause_1, 96, 252);
      DWIN_Draw_Rectangle(0, Color_White, 96, 252, 175, 351);
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 114, 322, F("Pause"));
    }
    else {
      DWIN_ICON_Show(ICON, ICON_Pause_0, 96, 252);
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Blue, 114, 322, F("Pause"));
    }
  }
}

void CrealityDWINClass::Draw_Print_Screen() {
  process = Print;
  selection = 0;
  Clear_Screen();
  DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 352, DWIN_WIDTH-8, 376);
  Print_Screen_Icons();
  DWIN_ICON_Show(ICON, ICON_PrintTime, 14, 171);
  DWIN_ICON_Show(ICON, ICON_RemainTime, 147, 169);
  DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 41, 163, (char*)"Elapsed");
  DWIN_Draw_String(false, false, DWIN_FONT_MENU,  Color_White, Color_Bg_Black, 176, 163, (char*)"Remaining");
  Update_Status_Bar();
  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();
  Draw_Print_Filename(true);
}

void CrealityDWINClass::Draw_Print_Filename(bool reset/*=false*/) {
  static uint8_t namescrl = 0;
  if (reset) namescrl = 0;
  if (process == Print) {
    size_t len = strlen(filename);
    int8_t pos = len;
    if (pos > 30) {
      pos -= namescrl;
      len = pos;
      if (len > 30)
        len = 30;
      char dispname[len+1];
      if (pos >= 0) {
        LOOP_L_N(i, len) dispname[i] = filename[i+namescrl];
      }
      else {
        LOOP_L_N(i, 30+pos) dispname[i] = ' ';
        LOOP_S_L_N(i, 30+pos, 30) dispname[i] = filename[i-(30+pos)];
      }
      dispname[len] = '\0';
      DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 50, DWIN_WIDTH-8, 80);
      const int8_t npos = (DWIN_WIDTH - 30 * MENU_CHR_W) / 2;
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, npos, 60, dispname);
      if (-pos >= 30)
        namescrl = 0;
      namescrl++;
    }
    else {
      DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 50, DWIN_WIDTH-8, 80);
      const int8_t npos = (DWIN_WIDTH - strlen(filename) * MENU_CHR_W) / 2;
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, npos, 60, filename);
    }
  }
}

void CrealityDWINClass::Draw_Print_ProgressBar() {
  uint8_t printpercent = sdprint ? card.percentDone() : (ui._get_progress()/100);
  DWIN_ICON_Show(ICON, ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, BarFill_Color, 16 + printpercent * 240 / 100, 93, 256, 113);
  DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_MENU, Percent_Color, Color_Bg_Black, 3, 109, 133, printpercent);
  DWIN_Draw_String(false, false, DWIN_FONT_MENU, Percent_Color, Color_Bg_Black, 133, 133, (char*)"%");
}

void CrealityDWINClass::Draw_Print_ProgressRemain() {
  uint16_t remainingtime = ui.get_remaining_time();
  DWIN_Draw_IntValue(true, true, 1, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 2, 176, 187, remainingtime / 3600);
  DWIN_Draw_IntValue(true, true, 1, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 2, 200, 187, (remainingtime % 3600) / 60);
  if (eeprom_settings.time_format_textual) {
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 192, 187, (char*)"h");
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 216, 187, (char*)"m");
  }
  else {
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 192, 187, (char*)":");
  }
}

void CrealityDWINClass::Draw_Print_ProgressElapsed() {
  duration_t elapsed = print_job_timer.duration();
  DWIN_Draw_IntValue(true, true, 1, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 2, 42, 187, elapsed.value / 3600);
  DWIN_Draw_IntValue(true, true, 1, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 2, 66, 187, (elapsed.value % 3600) / 60);
  if (eeprom_settings.time_format_textual) {
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 58, 187, (char*)"h");
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 82, 187, (char*)"m");
  }
  else {
    DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 58, 187, (char*)":");
  }
}

void CrealityDWINClass::Draw_Print_confirm() {
  process = Confirm;
  popup = Complete;
  DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 252, 263, 351);
  DWIN_ICON_Show(ICON, ICON_Confirm_E, 87, 283);
  DWIN_Draw_Rectangle(0, Select_Color, 86, 282, 187, 321);
  DWIN_Draw_Rectangle(0, Select_Color, 85, 281, 188, 322);
}

void CrealityDWINClass::Draw_SD_Item(uint8_t item, uint8_t row) {
  if (item == 0) {
    if (card.flag.workDirIsRoot)
      Draw_Menu_Item(0, ICON_Back, (char*)"Back");
    else
      Draw_Menu_Item(0, ICON_Back, (char*)"..");
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

void CrealityDWINClass::Draw_SD_List(bool removed/*=false*/) {
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

void CrealityDWINClass::Draw_Status_Area(bool icons/*=false*/) {

  if(icons) DWIN_Draw_Rectangle(1, Color_Bg_Black, 0, STATUS_Y, DWIN_WIDTH, DWIN_HEIGHT - 1);

  #if HAS_HOTEND
    static float hotend = -1;
    static int16_t hotendtarget = -1;
    static int16_t flow = -1;
    if (icons) { 
      DWIN_ICON_Show(ICON, ICON_HotendTemp, 10, 383);
      DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    }
    if (thermalManager.temp_hotend[0].celsius != hotend) {
      hotend = thermalManager.temp_hotend[0].celsius;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 384, thermalManager.temp_hotend[0].celsius);
    }
    if (thermalManager.temp_hotend[0].target != hotendtarget) {
      hotendtarget = thermalManager.temp_hotend[0].target;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.temp_hotend[0].target);
    }
    if (icons) {
      DWIN_ICON_Show(ICON, ICON_StepE, 112, 417);
      DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
    } 
    if (planner.flow_percentage[0] != flow) {
      flow = planner.flow_percentage[0];
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    }
  #endif

  #if HAS_HEATED_BED
    static float bed = -1;
    static int16_t bedtarget = -1;
    if (icons) {
      DWIN_ICON_Show(ICON, ICON_BedTemp, 10, 416);
      DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    }
    if (thermalManager.temp_bed.celsius != bed) {
      bed = thermalManager.temp_bed.celsius;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 28, 417, thermalManager.temp_bed.celsius);
    }
    if (thermalManager.temp_bed.target != bedtarget) {
      bedtarget = thermalManager.temp_bed.target;
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.temp_bed.target);
    }
  #endif

  #if HAS_FAN
    static uint8_t fan = -1;
    if (icons) DWIN_ICON_Show(ICON, ICON_FanSpeed, 187, 383);
    if (thermalManager.fan_speed[0] != fan) {
      fan = thermalManager.fan_speed[0];
      DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
    }
  #endif

  #if HAS_ZOFFSET_ITEM
    static float offset = -1;
    if (icons) DWIN_ICON_Show(ICON, ICON_Zoffset, 187, 416);
    if (zoffsetvalue != offset) {
      offset = zoffsetvalue;
      if (zoffsetvalue < 0) {
        DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, -zoffsetvalue * 100);
        DWIN_Draw_String(false, true, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 205, 419, (char*)"-");
      }
      else {
        DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 2, 2, 207, 417, zoffsetvalue* 100);
        DWIN_Draw_String(false, true, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 205, 419, (char*)" ");
      }
    }
  #endif

  static int16_t feedrate = -1;
  if (icons) {
    DWIN_ICON_Show(ICON, ICON_Speed, 113, 383);
    DWIN_Draw_String(false, false, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 116 + 5 * STAT_CHR_W + 2, 384, F("%"));
  }
  if (feedrate_percentage != feedrate) {
    feedrate = feedrate_percentage;
    DWIN_Draw_IntValue(true, true, 0, DWIN_FONT_STAT, Color_White, Color_Bg_Black, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  }

  static float x = -1;
  static float y = -1;
  static float z = -1;
  if (icons) {
    DWIN_Draw_Rectangle(1, Line_Color, 0, 449, DWIN_WIDTH, 451);
    DWIN_ICON_Show(ICON, ICON_MaxSpeedX,   10, 456);
    DWIN_ICON_Show(ICON, ICON_MaxSpeedY,   95, 456);
    DWIN_ICON_Show(ICON, ICON_MaxSpeedZ,   180, 456);
  }
  if (current_position.x != x) {
    x = current_position.x;
    DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 3, 1, 35, 459, current_position.x * 10);
  }
  if (current_position.y != y) {
    y = current_position.y;
    DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 3, 1, 120, 459, current_position.y * 10);
  }
  if (current_position.z != z) {
    z = current_position.z < Z_MAX_POS * 10 ? current_position.z : (current_position.z - 67295);
    if (z >= 0) {
      DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 3, 2, 205, 459, z * 100);
    }
    else {
      DWIN_Draw_String(false, true, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 205, 459, (char*)"-");
      DWIN_Draw_FloatValue(true, true, 0, DWIN_FONT_MENU, Color_White, Color_Bg_Black, 2, 2, 205, 459, z * 100);
    }
  }
  DWIN_UpdateLCD();
}

void CrealityDWINClass::Draw_Popup(const char *line1, const char *line2,const char *line3, uint8_t mode, uint8_t icon/*=0*/) {
  if (process == Menu && mode == Popup) last_selection = selection;
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
  DWIN_Draw_String(false, true, DWIN_FONT_MENU, Popup_Text_Color, Color_Bg_Window, (272 - 8 * strlen(line1)) / 2, ypos, F(line1));
  DWIN_Draw_String(false, true, DWIN_FONT_MENU, Popup_Text_Color, Color_Bg_Window, (272 - 8 * strlen(line2)) / 2, ypos+30, F(line2));
  DWIN_Draw_String(false, true, DWIN_FONT_MENU, Popup_Text_Color, Color_Bg_Window, (272 - 8 * strlen(line3)) / 2, ypos+60, F(line3));
  if (mode == Popup) {
    DWIN_ICON_Show(ICON, ICON_Confirm_E, 26, 280);
    DWIN_ICON_Show(ICON, ICON_Cancel_E, 146, 280);
    selection = 0;
    Popup_Select();
  }
  else if (mode == Confirm) {
    DWIN_ICON_Show(ICON, ICON_Continue_E, 87, 283);
  }
}

void CrealityDWINClass::Popup_Select() {
  const uint16_t c1 = (selection==0) ? Select_Color : Color_Bg_Window,
                 c2 = (selection==0) ? Color_Bg_Window : Select_Color;
  DWIN_Draw_Rectangle(0, c1, 25, 279, 126, 318);
  DWIN_Draw_Rectangle(0, c1, 24, 278, 127, 319);
  DWIN_Draw_Rectangle(0, c2, 145, 279, 246, 318);
  DWIN_Draw_Rectangle(0, c2, 144, 278, 247, 319);
}

void CrealityDWINClass::Update_Status_Bar() {
  static bool new_msg = true;
  static uint8_t msgscrl = 0;
  static char lastmsg[64];
  if (strcmp_P(lastmsg, statusmsg) != 0) {
    strcpy_P(lastmsg, statusmsg);
    msgscrl = 0;
    new_msg = true;
  }
  size_t len = strlen(statusmsg);
  int8_t pos = len;
  if (pos > 30) {
    pos -= msgscrl;
    len = pos;
    if (len > 30)
      len = 30;
    char dispmsg[len+1];
    if (pos >= 0) {
      LOOP_L_N(i, len) dispmsg[i] = statusmsg[i+msgscrl];
    }
    else {
      LOOP_L_N(i, 30+pos) dispmsg[i] = ' ';
      LOOP_S_L_N(i, 30+pos, 30) dispmsg[i] = statusmsg[i-(30+pos)];
    }
    dispmsg[len] = '\0';
    if (process == Print) {
      DWIN_Draw_Rectangle(1, Color_Grey, 8, 214, DWIN_WIDTH-8, 238);
      const int8_t npos = (DWIN_WIDTH - 30 * MENU_CHR_W) / 2;
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, npos, 219, dispmsg);
    }
    else {
      DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 352, DWIN_WIDTH-8, 376);
      const int8_t npos = (DWIN_WIDTH - 30 * MENU_CHR_W) / 2;
      DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, npos, 357, dispmsg);
    }
    if (-pos >= 30)
      msgscrl = 0;
    msgscrl++;
  }
  else {
    if (new_msg) {
      new_msg = false;
      if (process == Print) {
        DWIN_Draw_Rectangle(1, Color_Grey, 8, 214, DWIN_WIDTH-8, 238);
        const int8_t npos = (DWIN_WIDTH - strlen(statusmsg) * MENU_CHR_W) / 2;
        DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, npos, 219, statusmsg);
      }
      else {
        DWIN_Draw_Rectangle(1, Color_Bg_Black, 8, 352, DWIN_WIDTH-8, 376);
        const int8_t npos = (DWIN_WIDTH - strlen(statusmsg) * MENU_CHR_W) / 2;
        DWIN_Draw_String(false, false, DWIN_FONT_MENU, Color_White, Color_Bg_Black, npos, 357, statusmsg);
      }
    }
  }
}

/* Menu Item Config */

void CrealityDWINClass::Menu_Item_Handler(uint8_t menu, uint8_t item, bool draw/*=true*/) {
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
            Draw_Menu_Item(row, ICON_Axis, (char*)"Move", NULL, true);
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
            gcode.home_all_axes(true);
            Redraw_Menu();
          }
          break;
        case PREPARE_MANUALLEVEL:
          if (draw) {
            Draw_Menu_Item(row, ICON_PrintSize, (char*)"Manual Leveling", NULL, true);
          }
          else {
            if (axes_should_home()) {
              Popup_Handler(Home);
              gcode.home_all_axes(true);
            }
            #if ANY(HAS_ONESTEP_LEVELING, PROBE_MANUALLY)
              gcode.process_subcommands_now_P(PSTR("M420 S0"));
            #endif
            Draw_Menu(ManualLevel);
          }
          break;
        #if HAS_ZOFFSET_ITEM
          case PREPARE_ZOFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_Zoffset, (char*)"Z-Offset", NULL, true);
            }
            else {
              Draw_Menu(ZOffset);
            }
            break;
        #endif
        #if HAS_PREHEAT
          case PREPARE_PREHEAT:
            if (draw) {
              Draw_Menu_Item(row, ICON_Temperature, (char*)"Preheat", NULL, true);
            }
            else {
              Draw_Menu(Preheat);
            }
            break;
          case PREPARE_COOLDOWN:
            if (draw) {
              Draw_Menu_Item(row, ICON_Cool, (char*)"Cooldown");
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
                Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament", NULL, true);
              #else
                Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament");
              #endif
            }
            else {
              #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
                Draw_Menu(ChangeFilament);
              #else
                if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp) {
                  Popup_Handler(ETemp);
                }
                else {
                  if (thermalManager.temp_hotend[0].celsius < thermalManager.temp_hotend[0].target-2) {
                    Popup_Handler(Heating);
                    thermalManager.wait_for_hotend(0);
                  }
                  Popup_Handler(FilChange);
                  char buf[20];
                  sprintf(buf, "M600 B1 R%i", thermalManager.temp_hotend[0].target);
                  gcode.process_subcommands_now_P(buf);
                  planner.synchronize();
                  Redraw_Menu();
                }
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
      #define MOVE_P (MOVE_E + ENABLED(HAS_BED_PROBE))
      #define MOVE_TOTAL MOVE_P

      switch (item) {
        case MOVE_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            #if HAS_BED_PROBE
              probe_deployed = false;
              probe.set_deployed(probe_deployed);
            #endif
            Draw_Menu(Prepare, PREPARE_MOVE);
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
              current_position.e = 0;
              sync_plan_position();
              Draw_Float(current_position.e, item);
            }
            else {
              if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp) {
                Popup_Handler(ETemp);
              }
              else {
                if (thermalManager.temp_hotend[0].celsius < thermalManager.temp_hotend[0].target-2) {
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
        #endif
        #if HAS_BED_PROBE
          case MOVE_P:
            if (draw) {
              Draw_Menu_Item(row, ICON_StockConfiguraton, (char*)"Probe");
              Draw_Checkbox(row, probe_deployed);
            }
            else {
              probe_deployed = !probe_deployed;
              probe.set_deployed(probe_deployed);
              Draw_Checkbox(row, probe_deployed);
            }
            break;
        #endif

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
            #if ANY(HAS_ONESTEP_LEVELING, PROBE_MANUALLY)
              gcode.process_subcommands_now_P(PSTR("M420 S1"));
            #endif
            Draw_Menu(Prepare, PREPARE_MANUALLEVEL);
          }
          break;
        case MLEVEL_BL:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Bottom Left");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G0 F4000\nG0 Z10\nG0 X32.5 Y32.5\nG0 F300 Z0"));
            planner.synchronize();
            Redraw_Menu();
          }
          break;
        case MLEVEL_TL:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Top Left");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G0 F4000\nG0 Z10\nG0 X32.5 Y197.5\nG0 F300 Z0"));
            planner.synchronize();
            Redraw_Menu();
          }
          break;
        case MLEVEL_TR:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Top Right");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G0 F4000\nG0 Z10\nG0 X197.5 Y197.5\nG0 F300 Z0"));
            planner.synchronize();
            Redraw_Menu();
          }
          break;
        case MLEVEL_BR:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Bottom Right");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G0 F4000\nG0 Z10\nG0 X197.5 Y32.5\nG0 F300 Z0"));
            planner.synchronize();
            Redraw_Menu();
          }
          break;
        case MLEVEL_C:
          if (draw) {
            Draw_Menu_Item(row, ICON_Axis, (char*)"Center");
          }
          else {
            Popup_Handler(MoveWait);
            gcode.process_subcommands_now_P(PSTR("G0 F4000\nG0 Z10\nG0 X117.5 Y117.5\nG0 F300 Z0"));
            planner.synchronize();
            Redraw_Menu();
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
              Draw_Menu(Prepare, PREPARE_ZOFFSET);
            }
            break;
          case ZOFFSET_HOME:
            if (draw) {
              Draw_Menu_Item(row, ICON_Homing, (char*)"Home Z Axis");
            }
            else {
              Popup_Handler(Home);
              gcode.process_subcommands_now_P(PSTR("G28 Z"));
              Popup_Handler(MoveWait);
              #if ENABLED(Z_SAFE_HOMING)
                planner.synchronize();
                char buf[20];
                sprintf(buf, "G0 F4000 X%i Y%i", Z_SAFE_HOMING_X_POINT, Z_SAFE_HOMING_Y_POINT);
                gcode.process_subcommands_now_P(buf);
              #else
                gcode.process_subcommands_now_P(PSTR("G0 F4000 X117.5 Y117.5"));
              #endif
              gcode.process_subcommands_now_P(PSTR("G0 F300 Z0"));
              planner.synchronize();
              Redraw_Menu();
            }
            break;
          case ZOFFSET_MODE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Zoffset, (char*)"Live Adjustment");
              Draw_Checkbox(row, liveadjust);
            }
            else {
              if (!liveadjust) {
                if (axes_should_home()) {
                  Popup_Handler(Home);
                  gcode.home_all_axes(true);
                }
                Popup_Handler(MoveWait);
                #if ENABLED(Z_SAFE_HOMING)
                  planner.synchronize();
                  char buf[20];
                  sprintf(buf, "G0 F4000 X%i Y%i", Z_SAFE_HOMING_X_POINT, Z_SAFE_HOMING_Y_POINT);
                  gcode.process_subcommands_now_P(buf);
                #else
                  gcode.process_subcommands_now_P(PSTR("G0 F4000 X117.5 Y117.5"));
                #endif
                gcode.process_subcommands_now_P(PSTR("G0 F300 Z0"));
                planner.synchronize();
                Redraw_Menu();
              }
              liveadjust = !liveadjust;
              Draw_Checkbox(row, liveadjust);
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
        #define PREHEAT_5 (PREHEAT_4 + (PREHEAT_COUNT >= 5))
        #define PREHEAT_TOTAL PREHEAT_5

        switch (item) {
          case PREHEAT_BACK:
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Back");
            }
            else {
              Draw_Menu(Prepare, PREPARE_PREHEAT);
            }
            break;
          case PREHEAT_MODE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Homing, (char*)"Bed Only Mode");
              Draw_Checkbox(row, bedonly);
            }
            else {
              bedonly = !bedonly;
              Draw_Checkbox(row, bedonly);
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
          #if (PREHEAT_COUNT >= 5)
            case PREHEAT_5:
              if (draw) {
                Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_5_LABEL);
              }
              else {
                if (!bedonly) {
                  thermalManager.setTargetHotend(ui.material_preset[4].hotend_temp, 0);
                  thermalManager.set_fan_speed(0, ui.material_preset[4].fan_speed);
                }
                thermalManager.setTargetBed(ui.material_preset[4].bed_temp);
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
              Draw_Menu(Prepare, PREPARE_CHANGEFIL);
            }
            break;
          case CHANGEFIL_LOAD:
            if (draw) {
              Draw_Menu_Item(row, ICON_WriteEEPROM, (char*)"Load Filament");
            }
            else {
              if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp) {
                Popup_Handler(ETemp);
              }
              else {
                if (thermalManager.temp_hotend[0].celsius < thermalManager.temp_hotend[0].target-2) {
                  Popup_Handler(Heating);
                  thermalManager.wait_for_hotend(0);
                }
                Popup_Handler(FilLoad);
                gcode.process_subcommands_now_P(PSTR("M701"));
                planner.synchronize();
                Redraw_Menu();
              }
            }
            break;
          case CHANGEFIL_UNLOAD:
            if (draw) {
              Draw_Menu_Item(row, ICON_ReadEEPROM, (char*)"Unload Filament");
            }
            else {
              if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp) {
                Popup_Handler(ETemp);
              }
              else {
                if (thermalManager.temp_hotend[0].celsius < thermalManager.temp_hotend[0].target-2) {
                  Popup_Handler(Heating);
                  thermalManager.wait_for_hotend(0);
                }
                Popup_Handler(FilLoad, true);
                gcode.process_subcommands_now_P(PSTR("M702"));
                planner.synchronize();
                Redraw_Menu();
              }
            }
            break;
          case CHANGEFIL_CHANGE:
            if (draw) {
              Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Change Filament");
            }
            else {
              if (thermalManager.temp_hotend[0].target < thermalManager.extrude_min_temp) {
                Popup_Handler(ETemp);
              }
              else {
                if (thermalManager.temp_hotend[0].celsius < thermalManager.temp_hotend[0].target-2) {
                  Popup_Handler(Heating);
                  thermalManager.wait_for_hotend(0);
                }
                Popup_Handler(FilChange);
                char buf[20];
                sprintf(buf, "M600 B1 R%i", thermalManager.temp_hotend[0].target);
                gcode.process_subcommands_now_P(buf);
                planner.synchronize();
                Redraw_Menu();
              }
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
            Draw_Menu_Item(row, ICON_Temperature, (char*)"Temperature", NULL, true);
          }
          else {
            Draw_Menu(TempMenu);
          }
          break;
        case CONTROL_MOTION:
          if (draw) {
            Draw_Menu_Item(row, ICON_Motion, (char*)"Motion", NULL, true);
          }
          else {
            Draw_Menu(Motion);
          }
          break;
        case CONTROL_ADVANCED:
          if (draw) {
            Draw_Menu_Item(row, ICON_Version, (char*)"Advanced", NULL, true);
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
              Draw_Menu_Item(row, ICON_ReadEEPROM, (char*)"Restore Settings");
            }
            else {
              AudioFeedback(settings.load());
            }
            break;
          case CONTROL_RESET:
            if (draw) {
              Draw_Menu_Item(row, ICON_Temperature, (char*)"Reset to Defaults");
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
      #define TEMP_PREHEAT5 (TEMP_PREHEAT4 + (PREHEAT_COUNT >= 5))
      #define TEMP_TOTAL TEMP_PREHEAT5

      switch (item) {
        case TEMP_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Control, CONTROL_TEMP);
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
        #if (PREHEAT_COUNT >= 5)
          case TEMP_PREHEAT5:
            if (draw) {
              Draw_Menu_Item(row, ICON_Step, (char*)PREHEAT_5_LABEL);
            }
            else {
              Draw_Menu(Preheat5);
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
              Draw_Menu(TempMenu, TEMP_PREHEAT1);
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
              Draw_Menu(TempMenu, TEMP_PREHEAT2);
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
              Draw_Menu(TempMenu, TEMP_PREHEAT3);
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
        break;
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
              Draw_Menu(TempMenu, TEMP_PREHEAT4);
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
    #if (PREHEAT_COUNT >= 5)
      case Preheat5:

        #define PREHEAT5_BACK 0
        #define PREHEAT5_HOTEND (PREHEAT5_BACK + ENABLED(HAS_HOTEND))
        #define PREHEAT5_BED (PREHEAT5_HOTEND + ENABLED(HAS_HEATED_BED))
        #define PREHEAT5_FAN (PREHEAT5_BED + ENABLED(HAS_FAN))
        #define PREHEAT5_TOTAL PREHEAT5_FAN

        switch (item) {
          case PREHEAT5_BACK:
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Back");
            }
            else {
              Draw_Menu(TempMenu, TEMP_PREHEAT5);
            }
            break;
          #if HAS_HOTEND
            case PREHEAT5_HOTEND:
              if (draw) {
                Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Hotend");
                Draw_Float(ui.material_preset[4].hotend_temp, row, false, 1);
              }
              else {
                Modify_Value(ui.material_preset[4].hotend_temp, MIN_E_TEMP, MAX_E_TEMP, 1);
              }
              break;
          #endif
          #if HAS_HEATED_BED
            case PREHEAT5_BED:
              if (draw) {
                Draw_Menu_Item(row, ICON_SetBedTemp, (char*)"Bed");
                Draw_Float(ui.material_preset[4].bed_temp, row, false, 1);
              }
              else {
                Modify_Value(ui.material_preset[4].bed_temp, MIN_BED_TEMP, MAX_BED_TEMP, 1);
              }
              break;
          #endif
          #if HAS_FAN
            case PREHEAT5_FAN:
              if (draw) {
                Draw_Menu_Item(row, ICON_FanSpeed, (char*)"Fan");
                Draw_Float(ui.material_preset[4].fan_speed, row, false, 1);
              }
              else {
                Modify_Value(ui.material_preset[4].fan_speed, MIN_FAN_SPEED, MAX_FAN_SPEED, 1);
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
            Draw_Menu(Control, CONTROL_MOTION);
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
            Draw_Menu(Motion, MOTION_SPEED);
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
              Modify_Value(planner.settings.max_feedrate_mm_s[E_AXIS], 0, default_max_feedrate[E_AXIS]*2, 1);
            }
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
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Motion, MOTION_ACCEL);
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
              Draw_Menu(Motion, MOTION_JERK);
            }
            break;
          case JERK_X:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxSpeedJerkX, (char*)"X Axis");
              Draw_Float(planner.max_jerk[X_AXIS], row, false, 10);
            }
            else {
              Modify_Value(planner.max_jerk[X_AXIS], 0, default_max_jerk[X_AXIS]*2, 10);
            }
            break;
          case JERK_Y:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxSpeedJerkY, (char*)"Y Axis");
              Draw_Float(planner.max_jerk[Y_AXIS], row, false, 10);
            }
            else {
              Modify_Value(planner.max_jerk[Y_AXIS], 0, default_max_jerk[Y_AXIS]*2, 10);
            }
            break;
          case JERK_Z:
            if (draw) {
              Draw_Menu_Item(row, ICON_MaxSpeedJerkZ, (char*)"Z Axis");
              Draw_Float(planner.max_jerk[Z_AXIS], row, false, 10);
            }
            else {
              Modify_Value(planner.max_jerk[Z_AXIS], 0, default_max_jerk[Z_AXIS]*2, 10);
            }
            break;
          #if HAS_HOTEND
            case JERK_E:
              if (draw) {
                Draw_Menu_Item(row, ICON_MaxSpeedJerkE, (char*)"Extruder");
                Draw_Float(planner.max_jerk[E_AXIS], row, false, 10);
              }
              else {
                Modify_Value(planner.max_jerk[E_AXIS], 0, default_max_jerk[E_AXIS]*2, 10);
              }
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
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Motion, MOTION_STEPS);
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
              Modify_Value(planner.settings.axis_steps_per_mm[E_AXIS], 0, 1000, 10);
            }
            break;
        #endif
      }
      break;
    case Advanced:

      #define ADVANCED_BACK 0
      #define ADVANCED_XOFFSET (ADVANCED_BACK + ENABLED(HAS_BED_PROBE))
      #define ADVANCED_YOFFSET (ADVANCED_XOFFSET + ENABLED(HAS_BED_PROBE))
      #define ADVANCED_LOAD (ADVANCED_YOFFSET + ENABLED(ADVANCED_PAUSE_FEATURE))
      #define ADVANCED_UNLOAD (ADVANCED_LOAD + ENABLED(ADVANCED_PAUSE_FEATURE))
      #define ADVANCED_COLD_EXTRUDE  (ADVANCED_UNLOAD + ENABLED(PREVENT_COLD_EXTRUSION))
      #define ADVANCED_FILSENSORENABLED (ADVANCED_COLD_EXTRUDE + ENABLED(FILAMENT_RUNOUT_SENSOR))
      #define ADVANCED_FILSENSORDISTANCE (ADVANCED_FILSENSORENABLED + ENABLED(HAS_FILAMENT_RUNOUT_DISTANCE))
      #define ADVANCED_POWER_LOSS (ADVANCED_FILSENSORDISTANCE + ENABLED(POWER_LOSS_RECOVERY))
      #define ADVANCED_TIME_FORMAT (ADVANCED_POWER_LOSS + 1)
      #define ADVANCED_TOTAL ADVANCED_TIME_FORMAT

      switch (item) {
        case ADVANCED_BACK:
          if (draw) {
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
          }
          else {
            Draw_Menu(Control, CONTROL_ADVANCED);
          }
          break;
        #if ENABLED(HAS_BED_PROBE)
          case ADVANCED_XOFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_StepX, (char*)"Probe X Offset");
              Draw_Float(probe.offset.x, row, false, 10);
            }
            else {
              Modify_Value(probe.offset.x, -100, 100, 10);
            }
            break;
          case ADVANCED_YOFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_StepY, (char*)"Probe Y Offset");
              Draw_Float(probe.offset.y, row, false, 10);
            }
            else {
              Modify_Value(probe.offset.y, -100, 100, 10);
            }
            break;
        #endif
        #if ENABLED(ADVANCED_PAUSE_FEATURE)
          case ADVANCED_LOAD:
            if (draw) {
              Draw_Menu_Item(row, ICON_WriteEEPROM, (char*)"Load Length");
              Draw_Float(fc_settings[0].load_length, row, false, 1);
            }
            else {
              Modify_Value(fc_settings[0].load_length, 0, EXTRUDE_MAXLENGTH, 1);
            }
            break;
          case ADVANCED_UNLOAD:
            if (draw) {
              Draw_Menu_Item(row, ICON_ReadEEPROM, (char*)"Unload Length");
              Draw_Float(fc_settings[0].unload_length, row, false, 1);
            }
            else {
              Modify_Value(fc_settings[0].unload_length, 0, EXTRUDE_MAXLENGTH, 1);
            }
            break;
        #endif
        #if ENABLED(PREVENT_COLD_EXTRUSION)
          case ADVANCED_COLD_EXTRUDE:
            if (draw) {
              Draw_Menu_Item(row, ICON_Cool, (char*)"Min Extrusion T");
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
              Draw_Menu_Item(row, ICON_Extruder, (char*)"Filament Sensor");
              Draw_Checkbox(row, ExtUI::getFilamentRunoutEnabled());
            }
            else {
              ExtUI::setFilamentRunoutEnabled(!ExtUI::getFilamentRunoutEnabled());
              Draw_Checkbox(row, ExtUI::getFilamentRunoutEnabled());
            }
            break;
          #if ENABLED(HAS_FILAMENT_RUNOUT_DISTANCE)
            case ADVANCED_FILSENSORDISTANCE:
              if (draw) {
                Draw_Menu_Item(row, ICON_MaxAccE, (char*)"Runout Distance");
                Draw_Float(runout.runout_distance(), row, false, 10);
              }
              else {
                Modify_Value(runout.runout_distance(), 0, 999, 10);
              }
              break;
          #endif
        #endif
        #if ENABLED(POWER_LOSS_RECOVERY)
          case ADVANCED_POWER_LOSS:
            if (draw) {
              Draw_Menu_Item(row, ICON_Motion, (char*)"Power-loss recovery");
              Draw_Checkbox(row, recovery.enabled);
            }
            else {
              recovery.enable(!recovery.enabled);
              Draw_Checkbox(row, recovery.enabled);
            }
            break;
        #endif
        case ADVANCED_TIME_FORMAT:
          if (draw) {
            Draw_Menu_Item(row, ICON_PrintTime, (char*)"Progress as __h__m");
            Draw_Checkbox(row, eeprom_settings.time_format_textual);
          }
          else {
            eeprom_settings.time_format_textual = !eeprom_settings.time_format_textual;
            Draw_Checkbox(row, eeprom_settings.time_format_textual);
          }
          break;
      }
      break;
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
            Draw_Menu_Item(row, ICON_Back, (char*)"Back");
            
            #if ENABLED(PRINTCOUNTER)
              char row1[32], row2[32], buf[32];
              printStatistics ps = print_job_timer.getStats();

              sprintf(row1, "%i prints, %i finished", ps.totalPrints, ps.finishedPrints);
              sprintf(row2, "%.2f m filament used", ps.filamentUsed / 1000);
              Draw_Menu_Item(INFO_PRINTCOUNT, ICON_HotendTemp, row1, row2, false, true);

              ExtUI::getTotalPrintTime_str(buf);
              sprintf(row1, "Printed: %s", buf);
              ExtUI::getLongestPrint_str(buf);
              sprintf(row2, "Longest: %s", buf);
              Draw_Menu_Item(INFO_PRINTTIME, ICON_PrintTime, row1, row2, false, true);
            #endif
            
            Draw_Menu_Item(INFO_SIZE, ICON_PrintSize, (char*)MACHINE_SIZE, NULL, false, true);
            Draw_Menu_Item(INFO_VERSION, ICON_Version, (char*)SHORT_BUILD_VERSION, (char*)"Build Number: v" BUILD_NUMBER, false, true);
            Draw_Menu_Item(INFO_CONTACT, ICON_Contact, (char*)CORP_WEBSITE_E, NULL, false, true);
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
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      case UBL:

        #define UBL_BACK 0
        #define UBL_ACTIVE (UBL_BACK + 1)
        #define UBL_GET_TILT (UBL_ACTIVE + ENABLED(HAS_BED_PROBE))
        #define UBL_GET_MESH (UBL_GET_TILT + ENABLED(HAS_BED_PROBE))
        #define UBL_MANUAL (UBL_GET_MESH + 1)
        #define UBL_VIEW (UBL_MANUAL + 1)
        #define UBL_SETTINGS (UBL_VIEW + 1)
        #define UBL_LOAD (UBL_SETTINGS + 1)
        #define UBL_SAVE (UBL_LOAD + 1)
        #define UBL_TOTAL UBL_SAVE

        switch (item) {
          case UBL_BACK:
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Back");
            }
            else {
              Draw_Main_Menu(3);
            }
            break;
          case UBL_ACTIVE:
            if (draw) {
              Draw_Menu_Item(row, ICON_StockConfiguraton, (char*)"UBL Active");
              Draw_Checkbox(row, planner.leveling_active);
            }
            else {
              if (!planner.leveling_active) {
                set_bed_leveling_enabled(!planner.leveling_active);
                if (!planner.leveling_active) {
                  Confirm_Handler((char*)"Error: Couldn't enable UBL!");
                  break;
                }
              }
              else {
                set_bed_leveling_enabled(!planner.leveling_active);
              }
              Draw_Checkbox(row, planner.leveling_active);
            }
            break;
          #if HAS_BED_PROBE
            case UBL_GET_TILT:
              if (draw) {
                Draw_Menu_Item(row, ICON_Tilt, (char*)"Autotilt Current Mesh");
              }
              else {
                if (axes_should_home()) {
                  Popup_Handler(Home);
                  gcode.home_all_axes(true);
                }
                Popup_Handler(Level);
                char buf[10];
                if (ubl_conf.tilt_grid > 1) {
                  sprintf(buf, "G29 J%i", ubl_conf.tilt_grid);
                }
                else {
                  sprintf(buf, "G29 J");
                }
                gcode.process_subcommands_now_P(PSTR(buf));
                planner.synchronize();
                Redraw_Menu();
              }
              break;
            case UBL_GET_MESH:
              if (draw) {
                Draw_Menu_Item(row, ICON_Mesh, (char*)"Create New Mesh");
              }
              else {
                if (axes_should_home()) {
                  Popup_Handler(Home);
                  gcode.home_all_axes(true);
                }
                Popup_Handler(Level);
                gcode.process_subcommands_now_P(PSTR("G29 P0\nG29 P1"));
                gcode.process_subcommands_now_P(PSTR("G29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nG29 P3\nM420 S1"));
                planner.synchronize();
                Redraw_Menu();
                Confirm_Handler("The mesh has NOT been saved");
              }
              break;
          #endif
          case UBL_MANUAL:
            if (draw) {
              Draw_Menu_Item(row, ICON_Mesh, (char*)"Manual Tuning", NULL, true);
            }
            else {
              if (axes_should_home()) {
                Popup_Handler(Home);
                gcode.home_all_axes(true);
              }
              ubl_conf.mesh_step_warning = false;
              ubl_conf.manual_move();
              Draw_Menu(UBLManual);
            }
            break;
          case UBL_VIEW:
            if (draw) {
              Draw_Menu_Item(row, ICON_Mesh, (char*)"View Mesh", NULL, true);
            }
            else {
              Draw_Menu(UBLView);
            }
            break;
          case UBL_SETTINGS:
            if (draw) {
              Draw_Menu_Item(row, ICON_Step, (char*)"UBL Settings", NULL, true);
            }
            else {
              Draw_Menu(UBLSettings);
            }
            break;
          case UBL_LOAD:
            if (draw) {
              Draw_Menu_Item(row, ICON_ReadEEPROM, (char*)"Load Mesh");
            }
            else {
              gcode.process_subcommands_now_P(PSTR("G29 L0"));
              planner.synchronize();
              AudioFeedback(true);
            }
            break;
          case UBL_SAVE:
            if(draw) {
              Draw_Menu_Item(row, ICON_WriteEEPROM, (char*)"Save Mesh");
            }
            else {
              gcode.process_subcommands_now_P(PSTR("G29 S0"));
              planner.synchronize();
              AudioFeedback(true);
            }
            break;
        }
        break;
      case UBLView:

        #define UBLVIEW_BACK 0
        #define UBLVIEW_MESH (UBLVIEW_BACK + 1)
        #define UBLVIEW_TEXT (UBLVIEW_MESH + 1)
        #define UBLVIEW_ASYMMETRIC (UBLVIEW_TEXT + 1)
        #define UBLVIEW_TOTAL UBLVIEW_ASYMMETRIC

        switch (item) {
          case UBLVIEW_BACK:
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Back");
            }
            else {
              Draw_Menu(UBL, UBL_VIEW);
            }
            break;
          case UBLVIEW_MESH:
            if (draw) {
              Draw_Menu_Item(row, ICON_PrintSize, (char*)"Mesh Viewer", NULL, true);
            }
            else {
              Draw_Menu(MeshViewer);
            }
            break;
          case UBLVIEW_TEXT:
            if (draw) {
              Draw_Menu_Item(row, ICON_Contact, (char*)"Viewer Show Values");
              Draw_Checkbox(row, ubl_conf.viewer_print_value);
            }
            else {
              ubl_conf.viewer_print_value = !ubl_conf.viewer_print_value;
              Draw_Checkbox(row, ubl_conf.viewer_print_value);
            }
            break;
          case UBLVIEW_ASYMMETRIC:
            if (draw) {
              Draw_Menu_Item(row, ICON_Axis, (char*)"Viewer Asymmetric");
              Draw_Checkbox(row, ubl_conf.viewer_asymmetric_range);
            }
            else {
              ubl_conf.viewer_asymmetric_range = !ubl_conf.viewer_asymmetric_range;
              Draw_Checkbox(row, ubl_conf.viewer_asymmetric_range);
            }
            break;
        }
        break;
      case UBLSettings:

        #define UBLSETTINGS_BACK 0
        #define UBLSETTINGS_FADE (UBLSETTINGS_BACK + 1)
        #define UBLSETTINGS_TILT (UBLSETTINGS_FADE + 1)
        #define UBLSETTINGS_PLANE (UBLSETTINGS_TILT + 1)
        #define UBLSETTINGS_ZERO (UBLSETTINGS_PLANE + 1)
        #define UBLSETTINGS_UNDEF (UBLSETTINGS_ZERO + 1)
        #define UBLSETTINGS_TOTAL UBLSETTINGS_UNDEF

        switch (item) {
          case UBLSETTINGS_BACK:
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Back");
            }
            else {
              Draw_Menu(UBL, UBL_SETTINGS);
            }
            break;
          case UBLSETTINGS_FADE:
              if (draw) {
                Draw_Menu_Item(row, ICON_Fade, (char*)"Fade Mesh within");
                Draw_Float(planner.z_fade_height, row, 0, 1);
              }
              else {
                Modify_Value(planner.z_fade_height, 0, Z_MAX_POS, 1);
                planner.z_fade_height = -1;
                set_z_fade_height(planner.z_fade_height);
              }
              break;
          case UBLSETTINGS_TILT:
              if (draw) {
                Draw_Menu_Item(row, ICON_Tilt, (char*)"Tilting Grid Size");
                Draw_Float(ubl_conf.tilt_grid, row, 0, 1);
              }
              else {
                Modify_Value(ubl_conf.tilt_grid, 1, 15, 1);
              }
              break;
          case UBLSETTINGS_PLANE:
              if (draw) {
                Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Convert Mesh to Plane");
              }
              else {
                if (ubl_conf.create_plane_from_mesh()) {
                  Confirm_Handler((char*)"Error: Couldn't create plane!");
                  break;
                }
                gcode.process_subcommands_now_P(PSTR("M420 S1"));
                planner.synchronize();
                AudioFeedback(true);
              }
              break;
          case UBLSETTINGS_ZERO:
              if (draw) {
                Draw_Menu_Item(row, ICON_Mesh, (char*)"Zero Current Mesh");
              }
              else {
                gcode.process_subcommands_now_P(PSTR("G29 P0"));
                planner.synchronize();
              }
              break;
            case UBLSETTINGS_UNDEF:
              if (draw) {
                Draw_Menu_Item(row, ICON_Mesh, (char*)"Clear Current Mesh");
              }
              else {
                ubl.invalidate();
              }
              break;
        }
        break;
      case MeshViewer:
        #define MESHVIEW_BACK 0
        #define MESHVIEW_TOTAL MESHVIEW_BACK
        
        switch (item) {
          case MESHVIEW_BACK:
            if (draw) {
              Draw_Menu_Item(0, ICON_Back, (char*)"Back");
              Draw_Bed_Mesh();
              Set_Mesh_Viewer_Status();
            }
            else {
              Draw_Menu(UBLView, UBLVIEW_MESH);
              Update_Status("");
            }
            break;
        }
        break;
      case UBLManual:

        #define UBLM_BACK 0
        #define UBLM_X (UBLM_BACK + 1)
        #define UBLM_Y (UBLM_X + 1)
        #define UBLM_NEXT (UBLM_Y + 1)
        #define UBLM_OFFSET (UBLM_NEXT + 1)
        #define UBLM_UP (UBLM_OFFSET + 1)
        #define UBLM_DOWN (UBLM_UP + 1)
        #define UBLM_GOTO_VALUE (UBLM_DOWN + 1)
        #define UBLM_GOTO_ZHOP (UBLM_GOTO_VALUE + 1)
        #define UBLM_UNDEF (UBLM_GOTO_ZHOP + 1)
        #define UBLM_TOTAL UBLM_UNDEF

        switch (item) {
          case UBLM_BACK:
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Back");
            }
            else {
              Draw_Menu(UBL, UBL_MANUAL);
            }
            break;
          case UBLM_X:
            if (draw) {
              Draw_Menu_Item(row, ICON_MoveX, (char*)"Mesh Point X");
              Draw_Float(ubl_conf.mesh_x, row, 0, 1);
            }
            else {
              Modify_Value(ubl_conf.mesh_x, 0, GRID_MAX_POINTS_X - 1, 1);
            }
            break;
          case UBLM_Y:
            if (draw) {
              Draw_Menu_Item(row, ICON_MoveY, (char*)"Mesh Point Y");
              Draw_Float(ubl_conf.mesh_y, row, 0, 1);
            }
            else {
              Modify_Value(ubl_conf.mesh_y, 0, GRID_MAX_POINTS_Y - 1, 1);
            }
            break;
          case UBLM_NEXT:
            {
              bool more = ubl_conf.mesh_y * GRID_MAX_POINTS_X + ubl_conf.mesh_x < GRID_MAX_POINTS - 1;
              if (draw) {
                if (more)
                  Draw_Menu_Item(row, ICON_More, (char*)"Next Point");
                else
                  Draw_Menu_Item(row, ICON_More, (char*)"Save Mesh");
              }
              else {
                if (more) {
                  if (++ubl_conf.mesh_x == GRID_MAX_POINTS_X) {
                    ubl_conf.mesh_x = 0;
                    ubl_conf.mesh_y++;
                  }
                  ubl_conf.manual_move();
                }
                else {
                  gcode.process_subcommands_now_P(PSTR("G29 S0"));
                  planner.synchronize();
                  AudioFeedback(true);
                  Draw_Menu(UBL, UBL_MANUAL);
                }
              }
              break;
            }
          case UBLM_OFFSET:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetZOffset, (char*)"Point Z Offset");
              Draw_Float(ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y], row, false, 100);
            }
            else {
              if (!ubl_conf.mesh_step_warning && !ubl_conf.goto_mesh_value) {
                CrealityDWIN.Confirm_Handler((char*)"Z-value not in position");
                ubl_conf.mesh_step_warning = true;
              }
              else {
                if (isnan(ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y]))
                  ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y] = 0;
                Modify_Value(ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y], MIN_Z_OFFSET, MAX_Z_OFFSET, 100);
              }
            }
            break;
          case UBLM_UP:
            if (draw) {
              Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Up");
            }
            else {
              if (!ubl_conf.mesh_step_warning && !ubl_conf.goto_mesh_value) {
                CrealityDWIN.Confirm_Handler((char*)"Z-value not in position");
                ubl_conf.mesh_step_warning = true;
              } else if (ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y] < MAX_Z_OFFSET) {
                ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y] += 0.01;
                gcode.process_subcommands_now_P(PSTR("M290 Z0.01"));
                planner.synchronize();
                current_position.z += 0.01f;
                sync_plan_position();
                Draw_Float(ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y], row-1, false, 100);
              }
            }
            break;
          case UBLM_DOWN:
            if (draw) {
              Draw_Menu_Item(row, ICON_Axis, (char*)"Microstep Down");
            }
            else {
              if (!ubl_conf.mesh_step_warning && !ubl_conf.goto_mesh_value) {
                Confirm_Handler((char*)"Z-value not in position");
                ubl_conf.mesh_step_warning = true;
              } else if (ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y] > MIN_Z_OFFSET) {
                ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y] -= 0.01;
                gcode.process_subcommands_now_P(PSTR("M290 Z-0.01"));
                planner.synchronize();
                current_position.z -= 0.01f;
                sync_plan_position();
                Draw_Float(ubl.z_values[ubl_conf.mesh_x][ubl_conf.mesh_y], row-2, false, 100);
              }
            }
            break;
          case UBLM_GOTO_VALUE: 
            if (draw) {
              Draw_Menu_Item(row, ICON_StockConfiguraton, (char*)"Go to Mesh Z Value");
              Draw_Checkbox(row, ubl_conf.goto_mesh_value);
            }
            else {
              if (!ubl_conf.goto_mesh_value) {
                set_bed_leveling_enabled();
                if (!planner.leveling_active) {
                  Confirm_Handler((char*)"Error: Couldn't enable UBL!");
                  break;
                }
              }
              ubl_conf.goto_mesh_value = !ubl_conf.goto_mesh_value;
              ubl_conf.manual_move();
            }
            break;
          case UBLM_GOTO_ZHOP:
            if (ubl_conf.goto_mesh_value) {
              if (draw) {
                Draw_Menu_Item(row, ICON_Zoffset, (char*)"Z-hop When Moving");
                Draw_Checkbox(row, ubl_conf.mesh_goto_zhop);
              }
              else {
                ubl_conf.mesh_goto_zhop = !ubl_conf.mesh_goto_zhop;
                Draw_Checkbox(row, ubl_conf.mesh_goto_zhop);
              }
              break;
            }
          case UBLM_UNDEF:
            if (draw) {
              Draw_Menu_Item(row, ICON_ResumeEEPROM, (char*)"Clear Point Value");
            }
            else {
              ubl_conf.manual_value_update(true);
              Redraw_Menu();
            }
            break;
        }
        break;
    #endif
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
                Redraw_Menu();
              }
              else {
                gcode.process_subcommands_now_P(PSTR("G29"));
                planner.synchronize();
                AudioFeedback(settings.save());
                Draw_Main_Menu(3);
              }
            }
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
      #define TUNE_FILSENSORENABLED (TUNE_CHANGEFIL + ENABLED(FILAMENT_RUNOUT_SENSOR))
      #define TUNE_TOTAL TUNE_FILSENSORENABLED

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
              Modify_Value(thermalManager.temp_hotend[0].target, MIN_E_TEMP, MAX_E_TEMP, 1);
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
              Modify_Value(thermalManager.temp_bed.target, MIN_BED_TEMP, MAX_BED_TEMP, 1);
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
              char buf[20];
              sprintf(buf, "M600 B1 R%i", thermalManager.temp_hotend[0].target);
              gcode.process_subcommands_now_P(buf);
              planner.synchronize();
              Redraw_Menu();
            }
            break;
        #endif
        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          case TUNE_FILSENSORENABLED:
            if (draw) {
              Draw_Menu_Item(row, ICON_Extruder, (char*)"Filament Sensor");
              Draw_Checkbox(row, ExtUI::getFilamentRunoutEnabled());
            }
            else {
              ExtUI::setFilamentRunoutEnabled(!ExtUI::getFilamentRunoutEnabled());
              Draw_Checkbox(row, ExtUI::getFilamentRunoutEnabled());
            }
            break;
        #endif
      }
      break;
    case PreheatHotend:

        #define PREHEATHOTEND_BACK 0
        #define PREHEATHOTEND_1 (PREHEATHOTEND_BACK + (PREHEAT_COUNT >= 1))
        #define PREHEATHOTEND_2 (PREHEATHOTEND_1 + (PREHEAT_COUNT >= 2))
        #define PREHEATHOTEND_3 (PREHEATHOTEND_2 + (PREHEAT_COUNT >= 3))
        #define PREHEATHOTEND_4 (PREHEATHOTEND_3 + (PREHEAT_COUNT >= 4))
        #define PREHEATHOTEND_5 (PREHEATHOTEND_4 + (PREHEAT_COUNT >= 5))
        #define PREHEATHOTEND_CUSTOM (PREHEATHOTEND_5 + 1)
        #define PREHEATHOTEND_CONTINUE (PREHEATHOTEND_CUSTOM + 1)
        #define PREHEATHOTEND_TOTAL PREHEATHOTEND_CONTINUE

        switch (item) {
          case PREHEATHOTEND_BACK:
            if (draw) {
              Draw_Menu_Item(row, ICON_Back, (char*)"Cancel");
            }
            else {
              thermalManager.setTargetHotend(0, 0);
              thermalManager.set_fan_speed(0, 0);
              Draw_Menu(last_menu, last_selection);
            }
            break;
          #if (PREHEAT_COUNT >= 1)
            case PREHEATHOTEND_1:
              if (draw) {
                Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_1_LABEL);
              }
              else {
                thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0);
                thermalManager.set_fan_speed(0, ui.material_preset[0].fan_speed);
              }
              break;
          #endif
          #if (PREHEAT_COUNT >= 2)
            case PREHEATHOTEND_2:
              if (draw) {
                Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_2_LABEL);
              }
              else {
                thermalManager.setTargetHotend(ui.material_preset[1].hotend_temp, 0);
                thermalManager.set_fan_speed(0, ui.material_preset[1].fan_speed);
              }
              break;
          #endif
          #if (PREHEAT_COUNT >= 3)
            case PREHEATHOTEND_3:
              if (draw) {
                Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_3_LABEL);
              }
              else {
                thermalManager.setTargetHotend(ui.material_preset[2].hotend_temp, 0);
                thermalManager.set_fan_speed(0, ui.material_preset[2].fan_speed);
              }
              break;
          #endif
          #if (PREHEAT_COUNT >= 4)
            case PREHEATHOTEND_4:
              if (draw) {
                Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_4_LABEL);
              }
              else {
                thermalManager.setTargetHotend(ui.material_preset[3].hotend_temp, 0);
                thermalManager.set_fan_speed(0, ui.material_preset[3].fan_speed);
              }
              break;
          #endif
          #if (PREHEAT_COUNT >= 5)
            case PREHEATHOTEND_5:
              if (draw) {
                Draw_Menu_Item(row, ICON_Temperature, (char*)PREHEAT_5_LABEL);
              }
              else {
                thermalManager.setTargetHotend(ui.material_preset[4].hotend_temp, 0);
                thermalManager.set_fan_speed(0, ui.material_preset[4].fan_speed);
              }
              break;
          #endif
          case PREHEATHOTEND_CUSTOM:
            if (draw) {
              Draw_Menu_Item(row, ICON_Temperature, (char*)"Custom");
              Draw_Float(thermalManager.temp_hotend[0].target, row, false, 1);
            }
            else {
              Modify_Value(thermalManager.temp_hotend[0].target, EXTRUDE_MINTEMP, MAX_E_TEMP, 1);
            }
            break;
          case PREHEATHOTEND_CONTINUE:
            if (draw) {
              Draw_Menu_Item(row, ICON_SetEndTemp, (char*)"Continue");
            }
            else {
              Popup_Handler(Heating);
              thermalManager.wait_for_hotend(0);
              switch (last_menu) {
                case Prepare:
                  Popup_Handler(FilChange);
                  char buf[20];
                  sprintf(buf, "M600 B1 R%i", thermalManager.temp_hotend[0].target);
                  gcode.process_subcommands_now_P(buf);
                  planner.synchronize();
                  break;
                #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
                  case ChangeFilament:
                    switch (last_selection) {
                      case CHANGEFIL_LOAD:
                        Popup_Handler(FilLoad);
                        gcode.process_subcommands_now_P("M701");
                        planner.synchronize();
                        break;
                      case CHANGEFIL_UNLOAD:
                        Popup_Handler(FilLoad, true);
                        gcode.process_subcommands_now_P("M702");
                        planner.synchronize();
                        break;
                      case CHANGEFIL_CHANGE:
                        Popup_Handler(FilChange);
                        char buf[20];
                        sprintf(buf, "M600 B1 R%i", thermalManager.temp_hotend[0].target);
                        gcode.process_subcommands_now_P(buf);
                        planner.synchronize();
                        break;
                    }
                    break;
                #endif
              }
              Draw_Menu(last_menu, last_selection);
            }
            break;
        }
        break;
  }
}

char* CrealityDWINClass::Get_Menu_Title(uint8_t menu) {
  switch(menu) {
    case MainMenu:
      return (char*)"Main Menu";
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
    #if (PREHEAT_COUNT >= 1)
      case Preheat1:
        return (char*)(PREHEAT_1_LABEL " Settings");
    #endif
    #if (PREHEAT_COUNT >= 2)
      case Preheat2:
        return (char*)(PREHEAT_2_LABEL " Settings");
    #endif
    #if (PREHEAT_COUNT >= 3) 
      case Preheat3:
        return (char*)(PREHEAT_3_LABEL " Settings");
    #endif
    #if (PREHEAT_COUNT >= 4)
      case Preheat4:
        return (char*)(PREHEAT_4_LABEL " Settings");
    #endif
    #if (PREHEAT_COUNT >= 5)
      case Preheat5:
        return (char*)(PREHEAT_5_LABEL " Settings");
    #endif
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
    case InfoMain:
      return (char*)"Info";
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      case UBL:
        return (char*)"Unified Bed Leveling";
      case UBLView:
        return (char*)"UBL View";
      case UBLSettings:
        return (char*)"UBL Settings";
      case MeshViewer:
        return (char*)"Mesh Viewer";
      case UBLManual:
        return (char*)"UBL Manual Tuning";
    #endif
    #if ENABLED(PROBE_MANUALLY)
      case ManualMesh:
        return (char*)"Mesh Bed Leveling";
    #endif
    case Tune:
      return (char*)"Tune";
    case PreheatHotend:
      return (char*)"Preheat Hotend";
  }
  return (char*)"";
}

int CrealityDWINClass::Get_Menu_Size(uint8_t menu) {
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
    #endif
    case Control:
      return CONTROL_TOTAL;
    case TempMenu:
      return TEMP_TOTAL;
    #if (PREHEAT_COUNT >= 1)
      case Preheat1:
        return PREHEAT1_TOTAL;
    #endif
    #if (PREHEAT_COUNT >= 2)
      case Preheat2:
        return PREHEAT2_TOTAL;
    #endif
    #if (PREHEAT_COUNT >= 3)
      case Preheat3:
        return PREHEAT3_TOTAL;
    #endif
    #if (PREHEAT_COUNT >= 4)
      case Preheat4:
        return PREHEAT4_TOTAL;
    #endif
    #if (PREHEAT_COUNT >= 5)
      case Preheat5:
        return PREHEAT5_TOTAL;
    #endif
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
    case InfoMain:
      return INFO_TOTAL;
    #if ENABLED(PROBE_MANUALLY)
      case ManualMesh:
        return MMESH_TOTAL;
    #endif
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      case UBL:
        return UBL_TOTAL;
      case UBLView:
        return UBLVIEW_TOTAL;
      case UBLSettings:
        return UBLSETTINGS_TOTAL;
      case MeshViewer:
        return MESHVIEW_TOTAL;
      case UBLManual:
        return UBLM_TOTAL - !ubl_conf.goto_mesh_value;
    #endif
    case Tune:
      return TUNE_TOTAL;
    case PreheatHotend:
      return PREHEATHOTEND_TOTAL;
  }
  return 0;
}

/* Popup Config */

void CrealityDWINClass::Popup_Handler(uint8_t popupid, bool option/*=false*/) {
  popup = last_popup = popupid;
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
      Draw_Popup((char*)"Nozzle is too cold", (char*)"Open Preheat Menu?", (char*)"", Popup);
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
    case Heating:
      Draw_Popup((char*)"Heating", (char*)"Please wait until done.", (char*)"", Wait, ICON_BLTouch);
      break;
    case FilLoad:
      Draw_Popup(option ? (char*)"Unloading Filament" : (char*)"Loading Filament", (char*)"Please wait until done.", (char*)"", Wait, ICON_BLTouch);
      break;
    case FilChange:
      Draw_Popup((char*)"Filament Change", (char*)"Please wait for prompt.", (char*)"", Wait, ICON_BLTouch);
      break;
    case TempWarn:
      Draw_Popup(option ? (char*)"Nozzle temp too low!" : (char*)"Nozzle temp too high!", (char*)"", (char*)"", Wait, option ? ICON_TempTooLow : ICON_TempTooHigh);
      break;
    case Runout:
      Draw_Popup((char*)"Filament Runout", (char*)"", (char*)"", Wait, ICON_BLTouch);
      break;
  }
}

void CrealityDWINClass::Confirm_Handler(const char * const msg) {
  if (process != Confirm) last_process = process;
  popup = UI;
  if (strcmp_P(msg, GET_TEXT(MSG_FILAMENT_CHANGE_INSERT)) == 0) {
    Draw_Popup((char*)"Insert Filament", (char*)"Press to Continue", (char*)"", Confirm);
  }
  else if (strcmp_P(msg, GET_TEXT(MSG_HEATER_TIMEOUT)) == 0) {
    Draw_Popup((char*)"Heater Timed Out", (char*)"Press to Reheat", (char*)"", Confirm);
  }
  else if (strcmp_P(msg, (char*)"Reheat finished.") == 0) {
    Draw_Popup((char*)"Reheat Finished", (char*)"Press to Continue", (char*)"", Confirm);
  }
  else if (strcmp_P(msg, GET_TEXT(MSG_USERWAIT)) == 0) {
    Draw_Popup((char*)"Waiting for Input", (char*)"Press to Continue", (char*)"", Confirm);
  }
  else if (strcmp_P(msg, GET_TEXT(MSG_NOZZLE_PARKED)) == 0) {
  } 
  else if (strcmp_P(msg, (char*)"Z-value not in position") == 0) {
    Draw_Popup((char*)"Warning!", (char*)"Nozzle Z is not at 0", (char*)"Consider 'Go to Mesh Z Value'", Confirm);
  }
  else if (strcmp_P(msg, (char*)"Error: Couldn't enable UBL!") == 0) {
    Draw_Popup((char*)"Error: Couldn't enable UBL!", (char*)"Is every mesh point defined?", (char*)"(Gray in viewer are undefined)", Confirm);
  }
  else if (strcmp_P(msg, (char*)"The mesh has NOT been saved") == 0) {
    Draw_Popup((char*)"The mesh is ONLY in RAM!", (char*)"Use Save Mesh to save", (char*)"", Confirm);
  }
  else if (strcmp_P(msg, (char*)"Bad extruder number") == 0) {
    Draw_Popup((char*)"PID Autotune failed", (char*)"Bad extruder number", (char*)"", Confirm);
  }
  else if (strcmp_P(msg, (char*)"Temp too high") == 0) {
    Draw_Popup((char*)"PID Autotune failed", (char*)"Temp too high!", (char*)"", Confirm);
  }
  else if (strcmp_P(msg, (char*)"PID Timout") == 0) {
    Draw_Popup((char*)"PID Autotune failed", (char*)"Timeout!", (char*)"", Confirm);
  }
  else if (strcmp_P(msg, (char*)"PID Done") == 0) {
    Draw_Popup((char*)"PID tuning done", (char*)"", (char*)"", Confirm);
  }
  else {
    Draw_Popup(msg, (char*)"Press to Continue", (char*)"", Confirm);
  }
}

/* Navigation and Control */

inline void CrealityDWINClass::Main_Menu_Control() {
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
        #if ENABLED(AUTO_BED_LEVELING_UBL)
          Draw_Menu(UBL);
        #elif HAS_ONESTEP_LEVELING
          Popup_Handler(Level);
          gcode.process_subcommands_now_P(PSTR("G28\nG29"));
          planner.synchronize();
          Popup_Handler(SaveLevel);
        #elif ENABLED(PROBE_MANUALLY)
          gridpoint = 1;
          if (axes_should_home()) {
            Popup_Handler(Home);
            gcode.home_all_axes(true);
          }
          Popup_Handler(MoveWait);
          gcode.process_subcommands_now_P(PSTR("G29"));
          planner.synchronize();
          Draw_Menu(ManualMesh);
        #else
          Draw_Menu(InfoMain);
        #endif
        break;
    }
  DWIN_UpdateLCD();
}

inline void CrealityDWINClass::Menu_Control() {
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

inline void CrealityDWINClass::Value_Control() {
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
      planner.synchronize();
      float pos = current_position.z;
      current_position.z += (tempvalue/valueunit - zoffsetvalue);
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
    switch (active_menu) {
      case Move:
        planner.synchronize();
        planner.buffer_line(current_position, manual_feedrate_mm_s[selection-1], active_extruder);
        break;
      case ManualMesh:
        planner.synchronize();
        planner.buffer_line(current_position, homing_feedrate(Z_AXIS), active_extruder);
        planner.synchronize();
        break;
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        case UBLManual:
          ubl_conf.manual_move();
          break;
      #endif
    }
    return;
  }
  NOLESS(tempvalue, (valuemin * valueunit));
  NOMORE(tempvalue, (valuemax * valueunit));
  Draw_Float(tempvalue/valueunit, selection-scrollpos, true, valueunit);
  DWIN_UpdateLCD();
}

inline void CrealityDWINClass::File_Control() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  static uint8_t filescrl = 0;
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
          LOOP_L_N(i, len) name[i] = filename[i+filescrl];
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
    else {
      card.getfilename_sorted(SD_ORDER(selection-1, card.get_num_Files()));
      if (card.flag.filenameIsDir) {
        card.cd(card.filename);
        Draw_SD_List();
      }
      else {
        card.openAndPrintFile(card.filename);
        Start_Print(true);
      }
    }
  }
  DWIN_UpdateLCD();
}

inline void CrealityDWINClass::Print_Screen_Control() {
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
        Update_Status_Bar();
        break;
      case 1:
        if (paused) {
          if (sdprint) {
            paused = false;
            wait_for_user = false;
            #if ENABLED(PARK_HEAD_ON_PAUSE)
              card.startFileprint();
              startOrResumeJob();
              TERN_(POWER_LOSS_RECOVERY, recovery.prepare());
            #else
              char cmnd[20];
              cmnd[sprintf(cmnd, "M140 S%i", pausebed)] = '\0';
              gcode.process_subcommands_now_P(PSTR(cmnd));
              cmnd[sprintf(cmnd, "M109 S%i", pausetemp)] = '\0';
              gcode.process_subcommands_now_P(PSTR(cmnd));
              thermalManager.fan_speed[0] = pausefan;
              planner.synchronize();
              queue.inject_P(PSTR("M24"));
            #endif
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

inline void CrealityDWINClass::Popup_Control() {
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
            #if ENABLED(PARK_HEAD_ON_PAUSE)
              Popup_Handler(Home, true);
              #if ENABLED(SDSUPPORT)
                if (IS_SD_PRINTING()) card.pauseSDPrint();
              #endif
              print_job_timer.pause();
              planner.synchronize();
              queue.inject_P(PSTR("M125"));
              planner.synchronize();
            #else
              queue.inject_P(PSTR("M25"));
              pausetemp = thermalManager.temp_hotend[0].target;
              pausebed = thermalManager.temp_bed.target;
              pausefan = thermalManager.fan_speed[0];
              thermalManager.disable_all_heaters();
              thermalManager.zero_fan_speeds();
            #endif
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
            ExtUI::stopPrint();
            thermalManager.zero_fan_speeds();
            thermalManager.disable_all_heaters();
          }
          else {
            #if ENABLED(HOST_ACTION_COMMANDS)
              host_action_cancel();
              Draw_Main_Menu();
            #endif
          }
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
          thermalManager.setTargetHotend(EXTRUDE_MINTEMP, 0);
          thermalManager.set_fan_speed(0, MAX_FAN_SPEED);
          Draw_Menu(PreheatHotend);
        }
        else {
          Redraw_Menu();
        }
        break;
      case SaveLevel:
        if (selection==0) {
          AudioFeedback(settings.save());
        }
        Draw_Main_Menu();
        break;
    }
  DWIN_UpdateLCD();
}

inline void CrealityDWINClass::Confirm_Control() {
  ENCODER_DiffState encoder_diffState = Encoder_ReceiveAnalyze();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch(popup) {
      case Complete:
        Draw_Main_Menu();
        break;
      case UI:
        switch(last_process) {
          case Menu:
            Redraw_Menu();
            break;
          case Main:
            Draw_Main_Menu();
            break;
          case Print:
            Draw_Print_Screen();
            break;
          case File:
            Draw_SD_List();
            break;
          case Wait:
            if (last_popup == Runout)
              Draw_Print_Screen();
            else
              Popup_Handler(last_popup);
            break;
        }
        wait_for_user = false;
    }
  }
  DWIN_UpdateLCD();
}

/* In-Menu Value Modification */

void CrealityDWINClass::Setup_Value(float value, float min, float max, float unit, uint8_t type) {
  tempvalue = value * unit;
  valuemin = min;
  valuemax = max;
  valueunit = unit;
  valuetype = type;
  process = Value;
  EncoderRate.enabled = true;
  Draw_Float(tempvalue/unit, selection-scrollpos, true, valueunit);
}

void CrealityDWINClass::Modify_Value(float &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 0);
}
void CrealityDWINClass::Modify_Value(uint8_t &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 1);
}
void CrealityDWINClass::Modify_Value(uint16_t &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 2);
}
void CrealityDWINClass::Modify_Value(int16_t &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 3);
}
void CrealityDWINClass::Modify_Value(uint32_t &value, float min, float max, float unit) {
  valuepointer = &value;
  Setup_Value((float)value, min, max, unit, 4);
}

/* Main Functions */

void CrealityDWINClass::Update_Status(const char * const text) {
  char header[4];
  LOOP_L_N(i, 3) header[i] = text[i];
  header[3] = '\0';
  if (strcmp_P(header,(char*)"<F>")==0) {
    LOOP_L_N(i, _MIN((size_t)LONG_FILENAME_LENGTH, strlen(text))) filename[i] = text[i+3];
    filename[_MIN((size_t)LONG_FILENAME_LENGTH-1, strlen(text))] = '\0';
    Draw_Print_Filename(true);
  }
  else {
    LOOP_L_N(i, _MIN((size_t)64, strlen(text))) statusmsg[i] = text[i];
    statusmsg[_MIN((size_t)64, strlen(text))] = '\0';
  }
}

void CrealityDWINClass::Start_Print(bool sd) {
  sdprint = sd;
  if (!printing) {
    printing = true;
    statusmsg[0] = '\0';
    if (sd)
      strcpy_P(filename, card.longest_filename());
    else
      strcpy_P(filename, (char*)"Host Print");
    Draw_Print_Screen();
  }
}

void CrealityDWINClass::Stop_Print() {
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

void CrealityDWINClass::Update() {
  Screen_Update();

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

void CrealityDWINClass::Screen_Update() {
  static millis_t scrltime = 0;
  if (ELAPSED(millis(), scrltime)) {
    scrltime = millis() + 200;
    Update_Status_Bar();
    if (process==Print)
      Draw_Print_Filename();
  }

  static millis_t statustime = 0;
  if (ELAPSED(millis(), statustime)) {
    statustime = millis() + 500;
    Draw_Status_Area();
  }

  static millis_t printtime = 0;
  if (ELAPSED(millis(), printtime)) {
    printtime = millis() + 1000;
    if (process == Print) {
      Draw_Print_ProgressBar();
      Draw_Print_ProgressElapsed();
      Draw_Print_ProgressRemain();
      if (ExtUI::isPrintingPaused() != paused) {
        paused = ExtUI::isPrintingPaused();
        Print_Screen_Icons();
      }
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
    #endif
  #endif

  if (process == Menu || process == Value) {
    switch(active_menu) {
      case TempMenu:
        #if HAS_HOTEND
          if (thermalManager.temp_hotend[0].target != hotendtarget) {
            hotendtarget = thermalManager.temp_hotend[0].target;
            if (scrollpos <= TEMP_HOTEND && TEMP_HOTEND <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND-scrollpos)
                Draw_Float(thermalManager.temp_hotend[0].target, TEMP_HOTEND-scrollpos, false, 1);
            }
          }
        #endif
        #if HAS_HEATED_BED
          if (thermalManager.temp_bed.target != bedtarget) {
            bedtarget = thermalManager.temp_bed.target;
            if (scrollpos <= TEMP_BED && TEMP_BED <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND-scrollpos)
                Draw_Float(thermalManager.temp_bed.target, TEMP_BED-scrollpos, false, 1);
            }
          }
        #endif
        #if HAS_FAN
          if (thermalManager.fan_speed[0] != fanspeed) {
            fanspeed = thermalManager.fan_speed[0];
            if (scrollpos <= TEMP_FAN && TEMP_FAN <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND-scrollpos)
                Draw_Float(thermalManager.fan_speed[0], TEMP_FAN-scrollpos, false, 1);
            }
          }
        #endif
        break;
      case Tune:
        #if HAS_HOTEND
          if (thermalManager.temp_hotend[0].target != hotendtarget) {
            hotendtarget = thermalManager.temp_hotend[0].target;
            if (scrollpos <= TUNE_HOTEND && TUNE_HOTEND <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND-scrollpos)
                Draw_Float(thermalManager.temp_hotend[0].target, TUNE_HOTEND-scrollpos, false, 1);
            }
          }
        #endif
        #if HAS_HEATED_BED
          if (thermalManager.temp_bed.target != bedtarget) {
            bedtarget = thermalManager.temp_bed.target;
            if (scrollpos <= TUNE_BED && TUNE_BED <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND-scrollpos)
                Draw_Float(thermalManager.temp_bed.target, TUNE_BED-scrollpos, false, 1);
            }
          }
        #endif
        #if HAS_FAN
          if (thermalManager.fan_speed[0] != fanspeed) {
            fanspeed = thermalManager.fan_speed[0];
            if (scrollpos <= TUNE_FAN && TUNE_FAN <= scrollpos + MROWS) {
              if (process != Value || selection != TEMP_HOTEND-scrollpos)
                Draw_Float(thermalManager.fan_speed[0], TUNE_FAN-scrollpos, false, 1);
            }
          }
        #endif
        break;
    }
  }
}

void CrealityDWINClass::Startup() {
  delay(800);
  SERIAL_ECHOPGM("\nDWIN handshake ");
  if (DWIN_Handshake()) SERIAL_ECHOLNPGM("ok."); else SERIAL_ECHOLNPGM("error.");
  DWIN_Frame_SetDir(1); // Orientation 90°
  DWIN_UpdateLCD();     // Show bootscreen (first image)
  Encoder_Configuration();
  for (uint16_t t = 0; t <= 100; t += 2) {
    DWIN_ICON_Show(ICON, ICON_Bar, 15, 260);
    DWIN_Draw_Rectangle(1, Color_Bg_Black, 15 + t * 242 / 100, 260, 257, 280);
    DWIN_UpdateLCD();
    delay(20);
  }
  DWIN_JPG_CacheTo1(Language_English);
  Draw_Main_Menu();
  Draw_Status_Area(true);
  queue.inject_P(PSTR("M1000 S"));
}

void CrealityDWINClass::AudioFeedback(const bool success/*=true*/) {
  if (success) {
    buzzer.tone(100, 659);
    buzzer.tone(10, 0);
    buzzer.tone(100, 698);
  }
  else
    buzzer.tone(40, 440);
}

void CrealityDWINClass::SDCardInsert() { card.cdroot(); }

#endif
