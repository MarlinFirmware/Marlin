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
 * Enhanced DWIN implementation
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.9.2
 * date: 2021/11/21
 *
 * Based on the original code provided by Creality under GPL
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

#include "dwin_defines.h"

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
  PwrlossRec,
  Reboot,
  Info,
  ConfirmToPrint,

  // Popup Windows
  Homing,
  Leveling,
  PidProcess,
  ESDiagProcess,
  PrintStatsProcess,
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
  uint16_t pidgrphpoints  = 0;
  pidresult_t pidresult   = PID_DONE;
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
  uint8_t language;
  bool pause_flag:1;    // printing is paused
  bool pause_action:1;  // flag a pause action
  bool print_finish:1;  // print was finished
  bool select_flag:1;   // Popup button selected
  bool home_flag:1;     // homing in course
  bool heat_flag:1;     // 0: heating done  1: during heating
} HMI_flag_t;

extern HMI_value_t HMI_value;
extern HMI_flag_t HMI_flag;
extern uint8_t checkkey;
extern millis_t dwin_heat_time;

// Popups
#if HAS_HOTEND || HAS_HEATED_BED
  void DWIN_Popup_Temperature(const bool toohigh);
#endif
#if HAS_HOTEND
  void Popup_Window_ETempTooLow();
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  void Popup_PowerLossRecovery();
#endif

// SD Card
void HMI_SDCardInit();
void HMI_SDCardUpdate();

// Other
void Goto_PrintProcess();
void Goto_Main_Menu();
void Goto_Info_Menu();
void Goto_PowerLossRecovery();
void Goto_ConfirmToPrint();
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
void DWIN_LockScreen();
void DWIN_UnLockScreen();
void HMI_LockScreen();
#if HAS_MESH
  void DWIN_MeshViewer();
#endif
#if HAS_GCODE_PREVIEW
  void HMI_ConfirmToPrint();
#endif
#if HAS_ESDIAG
  void Draw_EndStopDiag();
#endif
#if ENABLED(PRINTCOUNTER)
  void Draw_PrintStats();
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
void Draw_Tramming_Menu();
#if HAS_HOME_OFFSET
  void Draw_HomeOffset_Menu();
#endif
#if HAS_BED_PROBE
  void Draw_ProbeSet_Menu();
#endif
#if HAS_FILAMENT_SENSOR
  void Draw_FilSet_Menu();
#endif
#if ENABLED(NOZZLE_PARK_FEATURE)
  void Draw_ParkPos_Menu();
#endif
void Draw_PhySet_Menu();
void Draw_SelectColors_Menu();
void Draw_GetColor_Menu();
#if BOTH(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
  void Draw_CaseLight_Menu();
#endif
#if ENABLED(LED_CONTROL_MENU)
  void Draw_LedControl_Menu();
#endif
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
#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void Draw_Homing_Menu();
#endif
