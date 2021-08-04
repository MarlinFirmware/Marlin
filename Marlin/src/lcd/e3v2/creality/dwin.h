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
#pragma once

/**
 * DWIN by Creality3D
 * Enhanced implementation by Miguel A. Risco-Castillo
 */

#include "../../../inc/MarlinConfigPre.h"

//#include "dwin_lcd.h"
#include "dwinui.h"
#include "rotary_encoder.h"
#include "../../../libs/BL24CXX.h"

#if ANY(HAS_HOTEND, HAS_HEATED_BED, HAS_FAN) && PREHEAT_COUNT
  #define HAS_PREHEAT 1
  #if PREHEAT_COUNT < 2
    #error "Creality DWIN requires two material preheat presets."
  #endif
#endif

#if ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT) && DISABLED(PROBE_MANUALLY)
  #define HAS_ONESTEP_LEVELING 1
#endif

#if !HAS_BED_PROBE && ENABLED(BABYSTEPPING)
  #define JUST_BABYSTEP 1
#endif

#if ANY(BABYSTEPPING, HAS_BED_PROBE, HAS_WORKSPACE_OFFSET)
  #define HAS_ZOFFSET_ITEM 1
#endif

static constexpr size_t eeprom_data_size = 64;

enum processID : uint8_t {
  // Process ID
  MainMenu,
  SelectFile,
  PrintProcess,
  PrintDone,
  FilamentMan,
  Menu,
  #if ENABLED(ASSISTED_TRAMMING)
    Tramming,
  #endif
  ManualMesh,
  MMeshMoveZ,
  TemperatureID,
  Motion,
  Info,
  Tune,
  TuneFlow,
  #if HAS_PREHEAT
    PLAPreheat,
    ABSPreheat,
  #endif
  MaxSpeed,
  MaxSpeed_value,
  MaxAcceleration,
  MaxAcceleration_value,
  MaxJerk,
  MaxJerk_value,
  Step,
  Step_value,
  #if HAS_HOME_OFFSET
    HomeOffsetX,
    HomeOffsetY,
    HomeOffsetZ,
  #endif
  #if HAS_BED_PROBE
    ProbeOffsetX,
    ProbeOffsetY,
  #endif
  Brightness,
  LoadLength,
  UnloadLength,
  GetColor_value,

  // Date variable ID
  Move_X,
  Move_Y,
  Move_Z,
  #if HAS_HOTEND
    Extruder,
    ETemp,
  #endif
  Zoffset,
  #if HAS_HEATED_BED
    BedTemp,
  #endif
  #if HAS_FAN
    FanSpeed,
  #endif
  PrintSpeed,
  PrintFlow,

  // Popup Windows
  Homing,
  Leveling,
  PauseOrStop,
  FilamentPurge,
  WaitResponse,
  Locked,
  NothingToDo,
};

// Picture ID
#define Start_Process       0
#define Language_English    1
#define Language_Chinese    2

#define DWIN_CHINESE 123
#define DWIN_ENGLISH 0

extern uint8_t checkkey;

extern millis_t dwin_heat_time;

typedef struct {
  #if HAS_HOTEND
    celsius_t E_Temp = 0;
  #endif
  #if HAS_HEATED_BED
    celsius_t Bed_Temp = 0;
  #endif
  #if HAS_FAN
    int16_t Fan_speed = 0;
  #endif
  int16_t print_speed     = 100;
  float Max_Feedspeed     = 0;
  float Max_Acceleration  = 0;
  float Max_Jerk_scaled   = 0;
  float Max_Step_scaled   = 0;
  float offset_value      = 0;
  float Move_Z_scaled     = 0;
  int8_t show_mode        = 0; // -1: Temperature control    0: Printing temperature
  int16_t print_flow      = 100;
  uint16_t Color[3];
  int16_t Value           = 0;
  uint16_t *P_Uint        = nullptr;
} HMI_value_t;

typedef struct {
  uint8_t Brightness = 127;
  uint16_t Background_Color = Def_Background_Color;
  uint16_t Cursor_color     = Def_Cursor_color;
  uint16_t TitleBg_color    = Def_TitleBg_color;
  uint16_t TitleTxt_color   = Def_TitleTxt_color;
  uint16_t Text_Color       = Def_Text_Color;
  uint16_t Selected_Color   = Def_Selected_Color;
  uint16_t SplitLine_Color  = Def_SplitLine_Color;
  uint16_t Highlight_Color  = Def_Highlight_Color;
  uint16_t StatusBg_Color   = Def_StatusBg_Color;
  uint16_t StatusTxt_Color  = Def_StatusTxt_Color;
  uint16_t PopupBg_color    = Def_PopupBg_color;
  uint16_t PopupTxt_Color   = Def_PopupTxt_Color;
  uint16_t AlertBg_Color    = Def_AlertBg_Color;
  uint16_t AlertTxt_Color   = Def_AlertTxt_Color;
  uint16_t PercentTxt_Color = Def_PercentTxt_Color;
  uint16_t Barfill_Color    = Def_Barfill_Color;
  uint16_t Indicator_Color  = Def_Indicator_Color;
  uint16_t Coordinate_Color = Def_Coordinate_Color;
} HMI_data_t;

typedef struct {
  uint8_t language;
  bool pause_flag:1;    // printing is paused
  bool pause_action:1;  // flag a pause action
  bool print_finish:1;  // print was finished
  bool select_flag:1;   // Popup button selected
  bool home_flag:1;     // homing in course
  bool heat_flag:1;  // 0: heating done  1: during heating
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    bool ETempTooLow_flag:1;
  #endif
  AxisEnum feedspeed_axis, acc_axis, jerk_axis, step_axis;
  bool lock_flag:1;     // 0: lock called from AdvSet  1: lock called from Tune
} HMI_flag_t;

extern HMI_value_t HMI_value;
extern HMI_flag_t HMI_flag;
extern HMI_data_t HMI_data;

enum pidresult_t : uint8_t { PID_BAD_EXTRUDER_NUM, PID_TEMP_TOO_HIGH, PID_TUNING_TIMEOUT, PID_EXTR_START, PID_BED_START, PID_DONE };

// Show ICO
void ICON_Print();
void ICON_Prepare();
void ICON_Control();
void ICON_Leveling(bool show);
void ICON_StartInfo(bool show);

void ICON_Pause();
void ICON_Continue();
void ICON_Stop();

void DWIN_Draw_Popup(uint8_t icon = 0, const char * const msg1 = 0, const char * const msg2 = 0, uint8_t button = 0);
void DWIN_Popup_Continue(uint8_t icon, const char * const msg1, const char * const msg2);
void DWIN_Popup_Confirm(uint8_t icon, const char * const msg1, const char * const msg2);

#if HAS_HOTEND || HAS_HEATED_BED
  // Popup message window
  void DWIN_Popup_Temperature(const bool toohigh);
#endif

#if HAS_HOTEND
  void Popup_Window_ETempTooLow();
#endif

void Popup_Window_Resume();

void Goto_PrintProcess();
void Goto_Main_Menu();

#if HAS_HOTEND
  void HMI_ETemp();
#endif
#if HAS_HEATED_BED
  void HMI_BedTemp();
#endif
#if HAS_FAN
  void HMI_FanSpeed();
#endif

void HMI_PrintSpeed();

void HMI_MaxFeedspeedXYZE();
void HMI_MaxAccelerationXYZE();
void HMI_MaxJerkXYZE();
void HMI_StepXYZE();

void update_variable();

// SD Card
void HMI_SDCardInit();
void HMI_SDCardUpdate();

// Main Process
void Icon_print();
void Icon_control();
void Icon_leveling(bool value);

// Other
void Draw_Select_Highlight(const bool sel);
void Draw_Status_Area(const bool with_update); // Status Area
void HMI_StartFrame(const bool with_update);   // Prepare the menu view
void HMI_MainMenu();    // Main process screen
void HMI_SelectFile();  // File page
void HMI_Printing();    // Print page
void HMI_Control();     // Control page
void HMI_Leveling();    // Level the page
#if ENABLED(ASSISTED_TRAMMING)
  void HMI_Tramming();   // Tramming menu
#endif
#if ENABLED(MESH_BED_LEVELING)
  void HMI_ManualMesh();  // Manual Mesh menu
  void HMI_MMeshMoveZ();  // Manual Mesh move Z
#endif
void HMI_Temperature();   // Temperature menu
void HMI_Motion();        // Sports menu
void HMI_Info();          // Information menu
void HMI_Tune();          // Adjust the menu
void Draw_Main_Area();    // Redraw main area;
void DWIN_Redraw_screen();  // Redraw all screen elements
void HMI_ReturnScreen();  // Return to previous screen before popups

#if HAS_PREHEAT
  void HMI_PLAPreheatSetting(); // PLA warm-up setting
  void HMI_ABSPreheatSetting(); // ABS warm-up setting
#endif

void HMI_MaxSpeed();        // Maximum speed submenu
void HMI_MaxAcceleration(); // Maximum acceleration submenu
void HMI_MaxJerk();         // Maximum jerk speed submenu
void HMI_Step();            // Transmission ratio

void HMI_Init();
void HMI_Popup();
void HMI_SaveProcessID(const uint8_t id);
void HMI_AudioFeedback(const bool success=true);
void DWIN_Startup();
void DWIN_Update();
void EachMomentUpdate();
void DWIN_HandleScreen();
void DWIN_DrawStatusLine(const uint16_t color, const uint16_t bgcolor, const char *text);
void DWIN_StatusChanged(const char * const text);
void DWIN_StatusChanged_P(PGM_P const text);
void DWIN_StartHoming();
void DWIN_CompletedHoming();
#if ENABLED(MESH_BED_LEVELING)
  void DWIN_MeshUpdate(const int8_t xpos, const int8_t ypos, const float zval);
#endif
void DWIN_MeshLevelingStart();
void DWIN_CompletedLeveling();
void DWIN_PidTuning(pidresult_t result);
void DWIN_Print_Started(const bool sd = false);
void DWIN_Print_Finished();
#if HAS_FILAMENT_SENSOR
  void DWIN_FilamentRunout(const uint8_t extruder);
#endif
void DWIN_Progress_Update();
void DWIN_Print_Header(const char *text);
void DWIN_SetColorDefaults();
void DWIN_StoreSettings(char *buff);
void DWIN_LoadSettings(const char *buff);
void DWIN_SetDataDefaults();
void DWIN_RebootScreen();

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void Draw_Popup_FilamentPurge();
  void DWIN_Popup_FilamentPurge();
  void HMI_FilamentPurge();
#endif

void HMI_LockScreen();
void DWIN_LockScreen(const bool flag = true);

// HMI user control functions
void HMI_Menu();
void HMI_Brightness();
void HMI_Move_X();
void HMI_Move_Y();
void HMI_Move_Z();
void HMI_Move_E();
TERN_(HAS_ZOFFSET_ITEM, void HMI_Zoffset());
#if HAS_HOME_OFFSET
  void HMI_HomeOffsetX();
  void HMI_HomeOffsetY();
  void HMI_HomeOffsetZ();
#endif
#if HAS_BED_PROBE
  void HMI_ProbeOffsetX();
  void HMI_ProbeOffsetY();
#endif
void HMI_GetColorValue();

// Menu drawing functions
void Draw_Control_Menu();
void Draw_AdvSet_Menu();
void Draw_Prepare_Menu();
void Draw_Move_Menu();
TERN_(HAS_HOME_OFFSET, void Draw_HomeOffset_Menu());
TERN_(HAS_BED_PROBE, void Draw_ProbeSet_Menu());
void Draw_SelectColors_Menu();
void Draw_GetColor_Menu();
