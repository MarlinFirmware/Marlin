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
 * Version: 3.21.2
 * Date: 2022/12/02
 */

#include "../../../inc/MarlinConfig.h"

#include "dwin_defines.h"
#include "dwinui.h"
#include "../common/encoder.h"
#include "../../../libs/BL24CXX.h"

#if EITHER(BABYSTEPPING, HAS_BED_PROBE)
  #define HAS_ZOFFSET_ITEM 1
  #if !HAS_BED_PROBE
    #define JUST_BABYSTEP 1
  #endif
#endif

namespace GET_LANG(LCD_LANGUAGE) {
  #define _MSG_PREHEAT(N) \
    LSTR MSG_PREHEAT_##N                  = _UxGT("Preheat ") PREHEAT_## N ##_LABEL; \
    LSTR MSG_PREHEAT_## N ##_SETTINGS     = _UxGT("Preheat ") PREHEAT_## N ##_LABEL _UxGT(" Conf");
  #if PREHEAT_COUNT > 3
    REPEAT_S(4, PREHEAT_COUNT, _MSG_PREHEAT)
  #endif
}

enum processID : uint8_t {
  // Process ID
  MainMenu,
  Menu,
  SetInt,
  SetPInt,
  SetIntNoDraw,
  SetFloat,
  SetPFloat,
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
  PIDTEMP_START = 0,
  PIDTEMPBED_START,
  PID_BAD_EXTRUDER_NUM,
  PID_TEMP_TOO_HIGH,
  PID_TUNING_TIMEOUT,
  PID_DONE,
};

#define DWIN_CHINESE 123
#define DWIN_ENGLISH 0

typedef struct {
  // Color settings
  uint16_t Background_Color;
  uint16_t Cursor_Color;
  uint16_t TitleBg_Color;
  uint16_t TitleTxt_Color;
  uint16_t Text_Color;
  uint16_t Selected_Color;
  uint16_t SplitLine_Color;
  uint16_t Highlight_Color;
  uint16_t StatusBg_Color;
  uint16_t StatusTxt_Color;
  uint16_t PopupBg_Color;
  uint16_t PopupTxt_Color;
  uint16_t AlertBg_Color;
  uint16_t AlertTxt_Color;
  uint16_t PercentTxt_Color;
  uint16_t Barfill_Color;
  uint16_t Indicator_Color;
  uint16_t Coordinate_Color;

  // Temperatures
  #if ENABLED(PIDTEMP)
    int16_t HotendPidT = DEF_HOTENDPIDT;
  #endif
  #if ENABLED(PIDTEMPBED)
    int16_t BedPidT = DEF_BEDPIDT;
  #endif
  #if (HAS_HOTEND || HAS_HEATED_BED) && HAS_PID_HEATING
    int16_t PidCycles = DEF_PIDCYCLES;
  #endif
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    int16_t ExtMinT = EXTRUDE_MINTEMP;
  #endif
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    int16_t BedLevT = LEVELING_BED_TEMP;
  #endif
  #if ENABLED(BAUD_RATE_GCODE)
    bool Baud115K = false;
  #endif

  bool FullManualTramming = false;
  bool MediaAutoMount = ENABLED(HAS_SD_EXTENDER);
  #if BOTH(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    uint8_t z_after_homing = DEF_Z_AFTER_HOMING;
  #endif
  #if BOTH(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    LEDColor Led_Color = Def_Leds_Color;
  #endif
} HMI_data_t;

extern HMI_data_t HMI_data;
static constexpr size_t eeprom_data_size = sizeof(HMI_data_t);

typedef struct {
  int8_t Color[3];                    // Color components
  #if HAS_PID_HEATING
    tempcontrol_t pidresult = PID_DONE;
  #endif
  uint8_t Select          = 0;        // Auxiliary selector variable
  AxisEnum axis           = X_AXIS;   // Axis Select
} HMI_value_t;

typedef struct {
  uint8_t language;
  bool printing_flag:1; // sd or host printing
  bool abort_flag:1;    // sd or host was aborted
  bool pause_flag:1;    // printing is paused
  bool percent_flag:1;  // percent was override by M73
  bool remain_flag:1;   // remain was override by M73
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
uint32_t GetHash(char * str);
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
  #define _DOPREHEAT(N) void DoPreheat##N();
  REPEAT_1(PREHEAT_COUNT, _DOPREHEAT)
#endif
void DoCoolDown();
#if ENABLED(PIDTEMP)
  void HotendPID();
#endif
#if ENABLED(PIDTEMPBED)
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
  void UBLMeshTilt();
  bool UBLValidMesh();
  void UBLMeshSave();
  void UBLMeshLoad();
#endif
#if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
  void HostShutDown();
#endif
#if !HAS_BED_PROBE
  void HomeZandDisable();
#endif

// Other
void Goto_PrintProcess();
void Goto_Main_Menu();
void Goto_Info_Menu();
void Goto_PowerLossRecovery();
void Goto_ConfirmToPrint();
void DWIN_Draw_Dashboard(const bool with_update); // Status Area
void Draw_Main_Area();      // Redraw main area
void DWIN_DrawStatusLine(const char *text = ""); // Draw simple status text
void DWIN_RedrawDash();     // Redraw Dash and Status line
void DWIN_RedrawScreen();   // Redraw all screen elements
void HMI_MainMenu();        // Main process screen
void HMI_SelectFile();      // File page
void HMI_Printing();        // Print page
void HMI_ReturnScreen();    // Return to previous screen before popups
void HMI_WaitForUser();
void HMI_SaveProcessID(const uint8_t id);
void HMI_SDCardUpdate();
void EachMomentUpdate();
void update_variable();
void DWIN_InitScreen();
void DWIN_HandleScreen();
void DWIN_CheckStatusMessage();
void DWIN_HomingStart();
void DWIN_HomingDone();
#if HAS_MESH
  void DWIN_MeshUpdate(const int8_t cpos, const int8_t tpos, const_float_t zval);
#endif
void DWIN_LevelingStart();
void DWIN_LevelingDone();
void DWIN_Print_Started();
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
#if HAS_LOCKSCREEN
  void DWIN_LockScreen();
  void DWIN_UnLockScreen();
  void HMI_LockScreen();
#endif
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
void Draw_Print_File_Menu();
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
void Draw_FilSet_Menu();
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
void Draw_Temperature_Menu();
void Draw_MaxSpeed_Menu();
void Draw_MaxAccel_Menu();
#if HAS_CLASSIC_JERK
  void Draw_MaxJerk_Menu();
#endif
void Draw_Steps_Menu();
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

// PID
#if HAS_PID_HEATING
  void DWIN_PidTuning(tempcontrol_t result);
  #if ENABLED(PIDTEMP)
    void Draw_HotendPID_Menu();
  #endif
  #if ENABLED(PIDTEMPBED)
    void Draw_BedPID_Menu();
  #endif
#endif
#endif
