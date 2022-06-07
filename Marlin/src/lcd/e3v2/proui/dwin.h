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
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.17.2
 * Date: 2022/04/08
 */

#include "dwin_defines.h"
#include "dwinui.h"
#include "../common/encoder.h"
#include "../../../libs/BL24CXX.h"

#include "../../../inc/MarlinConfig.h"

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
  Popup,
  Leveling,
  Locked,
  Reboot,
  PrintDone,
  ESDiagProcess,
  WaitResponse,
  Homing,
  PidProcess,
  NothingToDo
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
  pidresult_t pidresult   = PID_DONE;
  uint8_t Select          = 0;        // Auxiliary selector variable
  AxisEnum axis           = X_AXIS;   // Axis Select
} HMI_value_t;

typedef struct {
  uint8_t language;
  bool percent_flag:1;  // percent was override by M73
  bool remain_flag:1;   // remain was override by M73
  bool pause_flag:1;    // printing is paused
  bool pause_action:1;  // flag a pause action
  bool abort_flag:1;    // printing is aborting
  bool abort_action:1;  // flag a aborting action
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
#if ENABLED(POWER_LOSS_RECOVERY)
  void Popup_PowerLossRecovery();
#endif

// Tool Functions
#if ENABLED(EEPROM_SETTINGS)
  void WriteEeprom();
  void ReadEeprom();
  void ResetEeprom();
  #if HAS_MESH
    void SaveMesh();
  #endif
#endif
void RebootPrinter();
void DisableMotors();
void AutoLev();
void AutoHome();
#if HAS_PREHEAT
  void DoPreheat0();
  void DoPreheat1();
  void DoPreheat2();
#endif
void DoCoolDown();
#if HAS_HOTEND
  void HotendPID();
#endif
#if HAS_HEATED_BED
  void BedPID();
#endif
#if ENABLED(BAUD_RATE_GCODE)
  void HMI_SetBaudRate();
  void SetBaud115K();
  void SetBaud250K();
#endif
#if HAS_LCD_BRIGHTNESS
  void TurnOffBacklight();
#endif
void ApplyExtMinT();
void ParkHead();
#if HAS_ONESTEP_LEVELING
  void Trammingwizard();
#endif
#if BOTH(LED_CONTROL_MENU, HAS_COLOR_LEDS)
  void ApplyLEDColor();
#endif
#if ENABLED(AUTO_BED_LEVELING_UBL)
  void UBLTiltMesh();
  bool UBLValidMesh();
  void UBLSaveMesh();
  void UBLLoadMesh();
#endif

// Other
void Goto_PrintProcess();
void Goto_Main_Menu();
void Goto_Info_Menu();
void Goto_PowerLossRecovery();
void Goto_ConfirmToPrint();
void DWIN_Draw_Dashboard(const bool with_update); // Status Area
void Draw_Main_Area();      // Redraw main area
void DWIN_DrawStatusLine(const char *text); // Draw simple status text
void DWIN_DrawStatusLine(FSTR_P fstr);
void DWIN_RedrawDash();     // Redraw Dash and Status line
void DWIN_RedrawScreen();   // Redraw all screen elements
void HMI_MainMenu();        // Main process screen
void HMI_SelectFile();      // File page
void HMI_Printing();        // Print page
void HMI_ReturnScreen();    // Return to previous screen before popups
void HMI_WaitForUser();
void HMI_SaveProcessID(const uint8_t id);
void HMI_SDCardInit();
void HMI_SDCardUpdate();
void EachMomentUpdate();
void update_variable();
void DWIN_InitScreen();
void DWIN_HandleScreen();
void DWIN_CheckStatusMessage();
void DWIN_HomingStart();
void DWIN_HomingDone();
#if HAS_MESH
  void DWIN_MeshUpdate(const int8_t xpos, const int8_t ypos, const_float_t zval);
#endif
void DWIN_LevelingStart();
void DWIN_LevelingDone();
void DWIN_PidTuning(pidresult_t result);
void DWIN_Print_Started(const bool sd=false);
void DWIN_Print_Pause();
void DWIN_Print_Resume();
void DWIN_Print_Finished();
void DWIN_Print_Aborted();
#if HAS_FILAMENT_SENSOR
  void DWIN_FilamentRunout(const uint8_t extruder);
#endif
void DWIN_M73();
void DWIN_Print_Header(const char *text);
void DWIN_SetColorDefaults();
void DWIN_ApplyColor();
void DWIN_CopySettingsTo(char * const buff);
void DWIN_CopySettingsFrom(const char * const buff);
void DWIN_SetDataDefaults();
void DWIN_RebootScreen();

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void DWIN_Popup_Pause(FSTR_P const fmsg, uint8_t button=0);
  void Draw_Popup_FilamentPurge();
  void Goto_FilamentPurge();
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
#if ENABLED(FWRETRACT)
  void Draw_FWRetract_Menu();
#endif
#if HAS_MESH
  void Draw_MeshSet_Menu();
  #if ENABLED(MESH_EDIT_MENU)
    void Draw_EditMesh_Menu();
  #endif
#endif
