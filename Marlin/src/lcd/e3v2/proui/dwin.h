/**
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.19.3
 * Date: 2022/09/25
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../../inc/MarlinConfig.h"

#include "dwin_defines.h"
#include "dwinui.h"
#include "../common/encoder.h"
#include "../../../libs/BL24CXX.h"

#if HAS_CGCODE
  #include "custom_gcodes.h"
#endif

namespace GET_LANG(LCD_LANGUAGE) {
  #define _MSG_PREHEAT(N) \
    LSTR MSG_PREHEAT_##N                  = _UxGT("Preheat ") PREHEAT_## N ##_LABEL; \
    LSTR MSG_PREHEAT_## N ##_SETTINGS     = _UxGT("Preheat ") PREHEAT_## N ##_LABEL _UxGT(" Conf");
  #if PREHEAT_COUNT > 3
    REPEAT_S(4, PREHEAT_COUNT, _MSG_PREHEAT)
  #endif
}

const char DateTime[16+1] =
{
  // YY year
  __DATE__[7], __DATE__[8],__DATE__[9], __DATE__[10],
  // First month letter, Oct Nov Dec = '1' otherwise '0'
  (__DATE__[0] == 'O' || __DATE__[0] == 'N' || __DATE__[0] == 'D') ? '1' : '0',
  // Second month letter
  (__DATE__[0] == 'J') ? ( (__DATE__[1] == 'a') ? '1' :       // Jan, Jun or Jul
                          ((__DATE__[2] == 'n') ? '6' : '7') ) :
  (__DATE__[0] == 'F') ? '2' :                                // Feb 
  (__DATE__[0] == 'M') ? (__DATE__[2] == 'r') ? '3' : '5' :   // Mar or May
  (__DATE__[0] == 'A') ? (__DATE__[1] == 'p') ? '4' : '8' :   // Apr or Aug
  (__DATE__[0] == 'S') ? '9' :                                // Sep
  (__DATE__[0] == 'O') ? '0' :                                // Oct
  (__DATE__[0] == 'N') ? '1' :                                // Nov
  (__DATE__[0] == 'D') ? '2' :                                // Dec
  0,
  // First day letter, replace space with digit
  __DATE__[4]==' ' ? '0' : __DATE__[4],
  // Second day letter
  __DATE__[5],
  // Separator
  ' ','-',' ',
  // Time
  __TIME__[0],__TIME__[1],__TIME__[2],__TIME__[3],__TIME__[4],
  '\0'
};

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
  MPCProcess,
  PlotProcess,
  NothingToDo
};

#if HAS_PID_HEATING || ENABLED(MPCTEMP)
  enum tempcontrol_t : uint8_t {
  #if HAS_PID_HEATING
    PID_EXTR_START = 0,
    PID_BED_START,
    PID_BAD_EXTRUDER_NUM,
    PID_TEMP_TOO_HIGH,
    PID_TUNING_TIMEOUT,
    PID_DONE,
  #endif
  #if ENABLED(MPCTEMP)
    MPCTEMP_START,
    MPC_TEMP_ERROR,
    MPC_INTERRUPTED,
    MPC_DONE,
  #endif
  };
#endif

#define DWIN_CHINESE 123
#define DWIN_ENGLISH 0

typedef struct {
  // Color settings
  uint16_t Background_Color = Def_Background_Color;
  uint16_t Cursor_color = Def_Cursor_color;
  uint16_t TitleBg_color = Def_TitleBg_color;
  uint16_t TitleTxt_color = Def_TitleTxt_color;
  uint16_t Text_Color = Def_Text_Color;
  uint16_t Selected_Color = Def_Selected_Color;
  uint16_t SplitLine_Color = Def_SplitLine_Color;
  uint16_t Highlight_Color = Def_Highlight_Color;
  uint16_t StatusBg_Color = Def_StatusBg_Color;
  uint16_t StatusTxt_Color = Def_StatusTxt_Color;
  uint16_t PopupBg_color = Def_PopupBg_color;
  uint16_t PopupTxt_Color = Def_PopupTxt_Color;
  uint16_t AlertBg_Color = Def_AlertBg_Color;
  uint16_t AlertTxt_Color = Def_AlertTxt_Color;
  uint16_t PercentTxt_Color = Def_PercentTxt_Color;
  uint16_t Barfill_Color = Def_Barfill_Color;
  uint16_t Indicator_Color = Def_Indicator_Color;
  uint16_t Coordinate_Color = Def_Coordinate_Color;
  uint16_t Bottom_Color = Def_Bottom_Color;
  // Temperatures
  #if HAS_HOTEND && ENABLED(PIDTEMP)
    int16_t HotendPidT = DEF_HOTENDPIDT;
  #endif
  #if HAS_HEATED_BED && ENABLED(PIDTEMPBED)
    int16_t BedPidT = DEF_BEDPIDT;
  #endif
  #if (HAS_HOTEND || HAS_HEATED_BED) && HAS_PID_HEATING
    int16_t PidCycles = DEF_PIDCYCLES;
  #endif
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    int16_t ExtMinT = EXTRUDE_MINTEMP;
  #endif
  #if BOTH(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
  int16_t BedLevT = LEVELING_BED_TEMP;
  #endif
  TERN_(BAUD_RATE_GCODE, bool Baud250K = (BAUDRATE == 250000));
  bool CalcAvg = true;
  bool FullManualTramming = false;
  bool MediaAutoMount = ENABLED(HAS_SD_EXTENDER);
  bool SetLiveMove = false;
  #if BOTH(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    uint8_t z_after_homing = DEF_Z_AFTER_HOMING;
  #endif
  #if DISABLED(HAS_BED_PROBE)
    float ManualZOffset = 0;
  #endif
  // Led
  #if BOTH(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    uint32_t LED_Color = Def_Leds_Color;
  #endif
} HMI_data_t;

extern HMI_data_t HMI_data;
static constexpr size_t eeprom_data_size = sizeof(HMI_data_t) + TERN0(ProUIex, sizeof(PRO_data_t));

typedef struct {
  int8_t Color[3];                    // Color components
  TERN_(HAS_PID_HEATING, tempcontrol_t pidresult   = PID_DONE);
  uint8_t Select          = 0;        // Auxiliary selector variable
  AxisEnum axis           = X_AXIS;   // Axis Select
} HMI_value_t;

typedef struct {
  bool printing_flag:1; // sd or host printing
  bool abort_flag:1;    // sd or host was aborted
  bool pause_flag:1;    // printing is paused
  bool percent_flag:1;  // percent was override by M73
  bool remain_flag:1;   // remain was override by M73
  bool select_flag:1;   // Popup button selected
  bool home_flag:1;     // homing in course
  bool heat_flag:1;     // 0: heating done  1: during heating
  bool config_flag:1;   // SD G-code file is a Configuration file
  #if ProUIex && HAS_LEVELING
    bool cancel_abl:1;  // cancel current abl
  #endif
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
#if HAS_HOTEND  && ENABLED(PIDTEMP)
  void HotendPID();
#endif
#if HAS_HEATED_BED && ENABLED(PIDTEMPBED)
  void BedPID();
#endif
#if ENABLED(BAUD_RATE_GCODE)
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
#if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
  void HostShutDown();
#endif
#if !HAS_BED_PROBE
  void HomeZandDisable();
#endif

// Other
void SetLiveMove();
void Goto_PrintProcess();
void Goto_Main_Menu();
void Goto_Info_Menu();
void Goto_PowerLossRecovery();
void Goto_ConfirmToPrint();
void DWIN_Draw_Dashboard(const bool with_update); // Status Area
void Draw_Main_Area();      // Redraw main area
void DWIN_DrawStatusLine(const char *text = ""); // Draw simple status text
void DWIN_RedrawDash();    // Redraw Dash and Status line
void DWIN_RedrawScreen();  // Redraw all screen elements
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
void DWIN_ApplyColor(const int8_t element, const bool ldef=false);
void DWIN_CopySettingsTo(char * const buff);
void DWIN_CopySettingsFrom(const char * const buff);
void DWIN_SetDataDefaults();
void DWIN_RebootScreen();
inline void DWIN_Gcode(const int16_t codenum) { TERN_(HAS_CGCODE, custom_gcode(codenum)); };

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
void Draw_Advanced_Menu();
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
void Draw_PID_Menu();
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
  void Draw_MeshInset_Menu();
  void Draw_EditMesh_Menu();
#endif

//PID
void DWIN_PidTuning(tempcontrol_t result);
#if ENABLED(PIDTEMP)
  void Draw_HotendPID_Menu();
#endif
#if ENABLED(PIDTEMPBED)
  void Draw_BedPID_Menu();
#endif

//MPC
#if ENABLED(MPCTEMP)
  void DWIN_MPCTuning(tempcontrol_t result);
  void Draw_HotendMPC_Menu();
#endif

// ToolBar
#if HAS_TOOLBAR
  void Draw_TBSetup_Menu();
  void Goto_ToolBar();
  void Exit_ToolBar();
#endif

#if DEBUG_DWIN
  void DWIN_Debug(const char *msg);
#endif
