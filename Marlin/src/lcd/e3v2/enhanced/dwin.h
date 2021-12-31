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
#pragma once

/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo
 * Version: 3.7.1
 * Date: 2021/11/09
 */

#include "../../../inc/MarlinConfigPre.h"
#include "dwinui.h"
#include "../common/encoder.h"
#include "../../../libs/BL24CXX.h"

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
  Menu,
  SetInt,
  SetPInt,
  SetIntNoDraw,
  SetFloat,
  SetPFloat,
  SelectFile,
  PrintProcess,
  PrintDone,
  Info,

  // Popup Windows
  Homing,
  Leveling,
  PauseOrStop,
  FilamentPurge,
  WaitResponse,
  Locked,
  NothingToDo,
};

enum pidresult_t : uint8_t {
  PID_BAD_EXTRUDER_NUM,
  PID_TEMP_TOO_HIGH,
  PID_TUNING_TIMEOUT,
  PID_EXTR_START,
  PID_BED_START,
  PID_DONE
};

#define DWIN_CHINESE 123
#define DWIN_ENGLISH 0

typedef struct {
  int8_t Color[3];                    // Color components
  int8_t Preheat          = 0;        // Material Select 0: PLA, 1: ABS, 2: Custom
  AxisEnum axis           = X_AXIS;   // Axis Select
  int32_t MaxValue        = 0;        // Auxiliar max integer/scaled float value
  int32_t MinValue        = 0;        // Auxiliar min integer/scaled float value
  int8_t dp               = 0;        // Auxiliar decimal places
  int32_t Value           = 0;        // Auxiliar integer / scaled float value
  int16_t *P_Int          = nullptr;  // Auxiliar pointer to 16 bit integer variable
  float *P_Float          = nullptr;  // Auxiliar pointer to float variable
  void (*Apply)()         = nullptr;  // Auxiliar apply function
  void (*LiveUpdate)()    = nullptr;  // Auxiliar live update function
} HMI_value_t;

typedef struct {
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
  #if HAS_PREHEAT
    #ifdef PREHEAT_1_TEMP_HOTEND
      int16_t HotendPidT = PREHEAT_1_TEMP_HOTEND;
      int16_t PidCycles = 10;
    #endif
    #ifdef PREHEAT_1_TEMP_BED
      int16_t BedPidT = PREHEAT_1_TEMP_BED;
    #endif
  #endif
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    int16_t ExtMinT = EXTRUDE_MINTEMP;
  #endif
} HMI_data_t;

typedef struct {
  uint8_t language;
  bool pause_flag:1;    // printing is paused
  bool pause_action:1;  // flag a pause action
  bool print_finish:1;  // print was finished
  bool select_flag:1;   // Popup button selected
  bool home_flag:1;     // homing in course
  bool heat_flag:1;     // 0: heating done  1: during heating
  bool lock_flag:1;     // 0: lock called from AdvSet  1: lock called from Tune
} HMI_flag_t;

extern HMI_value_t HMI_value;
extern HMI_flag_t HMI_flag;
extern HMI_data_t HMI_data;
extern uint8_t checkkey;
extern millis_t dwin_heat_time;

// SD Card
void HMI_SDCardInit();
void HMI_SDCardUpdate();

// Other
void Goto_PrintProcess();
void Goto_Main_Menu();
void Goto_InfoMenu();
void Draw_Select_Highlight(const bool sel);
void Draw_Status_Area(const bool with_update); // Status Area
void Draw_Main_Area();      // Redraw main area;
void DWIN_Redraw_screen();  // Redraw all screen elements
void HMI_StartFrame(const bool with_update);   // Prepare the menu view
void HMI_MainMenu();        // Main process screen
void HMI_SelectFile();      // File page
void HMI_Printing();        // Print page
void HMI_ReturnScreen();    // Return to previous screen before popups
void ApplyExtMinT();
void HMI_SetLanguageCache(); // Set the languaje image cache

void HMI_Init();
void HMI_Popup();
void HMI_SaveProcessID(const uint8_t id);
void HMI_AudioFeedback(const bool success=true);
void EachMomentUpdate();
void update_variable();
void DWIN_HandleScreen();
void DWIN_Update();
void DWIN_CheckStatusMessage();
void DWIN_StartHoming();
void DWIN_CompletedHoming();
#if HAS_MESH
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

// Utility and extensions
void HMI_LockScreen();
void DWIN_LockScreen(const bool flag = true);
#if HAS_MESH
  void DWIN_MeshViewer();
#endif

// HMI user control functions
void HMI_Menu();
void HMI_SetInt();
void HMI_SetPInt();
void HMI_SetIntNoDraw();
void HMI_SetFloat();
void HMI_SetPFloat();

// Menu drawing functions
void Draw_Control_Menu();
void Draw_AdvancedSettings_Menu();
void Draw_Prepare_Menu();
void Draw_Move_Menu();
void Draw_LevBedCorners_Menu();
#if HAS_HOME_OFFSET
  void Draw_HomeOffset_Menu();
#endif
#if HAS_BED_PROBE
  void Draw_ProbeSet_Menu();
#endif
#if HAS_FILAMENT_SENSOR
  void Draw_FilSet_Menu();
#endif
void Draw_SelectColors_Menu();
void Draw_GetColor_Menu();
void Draw_Tune_Menu();
void Draw_Motion_Menu();
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void Draw_FilamentMan_Menu();
#endif
#if ENABLED(MESH_BED_LEVELING)
  void Draw_ManualMesh_Menu();
#endif
#if HAS_HOTEND
  void Draw_Preheat1_Menu();
  void Draw_Preheat2_Menu();
  void Draw_Preheat3_Menu();
  void Draw_HotendPID_Menu();
#endif
void Draw_Temperature_Menu();
void Draw_MaxSpeed_Menu();
void Draw_MaxAccel_Menu();
#if HAS_CLASSIC_JERK
  void Draw_MaxJerk_Menu();
#endif
void Draw_Steps_Menu();
#if HAS_HEATED_BED
  void Draw_BedPID_Menu();
#endif
#if EITHER(HAS_BED_PROBE, BABYSTEPPING)
  void Draw_ZOffsetWiz_Menu();
#endif

// Popup windows

void DWIN_Draw_Popup(const uint8_t icon, const char * const cmsg1, FSTR_P const fmsg2, uint8_t button=0);
void DWIN_Draw_Popup(const uint8_t icon, FSTR_P const fmsg1=nullptr, FSTR_P const fmsg2=nullptr, uint8_t button=0);

template<typename T, typename U>
void DWIN_Popup_Confirm(const uint8_t icon, T amsg1, U amsg2) {
  HMI_SaveProcessID(WaitResponse);
  DWIN_Draw_Popup(icon, amsg1, amsg2, ICON_Confirm_E);  // Button Confirm
  DWIN_UpdateLCD();
}

#if HAS_HOTEND || HAS_HEATED_BED
  void DWIN_Popup_Temperature(const bool toohigh);
#endif
#if HAS_HOTEND
  void Popup_Window_ETempTooLow();
#endif
void Popup_Window_Resume();
