/**
 * DWIN Enhanced implementation, general defines and data structs for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 4.6.3
 * Date: 2023/10/27
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

#include "../../../libs/BL24CXX.h"
#include "../../../MarlinCore.h"
#include "../../../module/printcounter.h"
#include "../../../sd/cardreader.h"
#include "../common/encoder.h"
#include "../common/limits.h"

#if HAS_CGCODE
  #include "../../../prouiex/custom_gcodes.h"
#endif

#if ENABLED(CV_LASER_MODULE)
  #include "../../../prouiex/cv_laser_module.h"
#endif

#include "dwinui.h"

// #define DEBUG_DWIN 1

#define EXT active_extruder // default extruder
#define FAN 0 // default fan

#define defColorLeds        0xFFFFFFFF
#define defCaseLightBrightness 255
#ifdef Z_AFTER_HOMING
  #define DEF_Z_AFTER_HOMING Z_AFTER_HOMING
#else
  #define DEF_Z_AFTER_HOMING 0
#endif
#define DEF_HOTENDPIDT PREHEAT_1_TEMP_HOTEND
#define DEF_BEDPIDT PREHEAT_1_TEMP_BED
#define DEF_PIDCYCLES 5

#if HAS_BED_PROBE
  #define OFFSET_ZMIN PROBE_OFFSET_ZMIN
  #define OFFSET_ZMAX PROBE_OFFSET_ZMAX
#else
  #define OFFSET_ZMIN -20
  #define OFFSET_ZMAX 20
#endif

extern char dateTime[16+1];

enum processID : uint8_t {
  // Process ID
  ID_MainMenu,
  ID_Menu,
  ID_SetInt,
  ID_SetPInt,
  ID_SetIntNoDraw,
  ID_SetFloat,
  ID_SetPFloat,
  ID_PrintProcess,
  ID_Popup,
  ID_Leveling,
  ID_Locked,
  ID_Reboot,
  ID_PrintDone,
  ID_ESDiagProcess,
  ID_WaitResponse,
  ID_Homing,
  ID_PIDProcess,
  ID_MPCProcess,
  ID_NothingToDo
};

#if HAS_PID_HEATING || ENABLED(MPC_AUTOTUNE)
  enum tempcontrol_t : uint8_t {
  #if HAS_PID_HEATING
    PIDTEMP_START = 0,
    PIDTEMPBED_START,
    PID_BAD_HEATER_ID,
    PID_TEMP_TOO_HIGH,
    PID_TUNING_TIMEOUT,
  #endif
  #if ENABLED(MPC_AUTOTUNE)
    MPCTEMP_START,
    MPC_TEMP_ERROR,
    MPC_INTERRUPTED,
  #endif
  AUTOTUNE_DONE
  };
#endif

#define DWIN_CHINESE 123
#define DWIN_ENGLISH 0

typedef struct {
  uint16_t colorBackground;
  uint16_t colorCursor;
  uint16_t colorTitleBg;
  uint16_t colorTitleTxt;
  uint16_t colorText;
  uint16_t colorSelected;
  uint16_t colorSplitLine;
  uint16_t colorHighlight;
  uint16_t colorStatusBg;
  uint16_t colorStatusTxt;
  uint16_t colorPopupBg;
  uint16_t colorPopupTxt;
  uint16_t colorAlertBg;
  uint16_t colorAlertTxt;
  uint16_t colorPercentTxt;
  uint16_t colorBarfill;
  uint16_t colorIndicator;
  uint16_t colorCoordinate;
  int16_t hotendPidT;
  int16_t bedPidT;
  int16_t pidCycles;
  int16_t extMinT;
  int16_t bedLevT;
  bool baud115K;
  bool fullManualTramming;
  bool mediaSort;
  bool mediaAutoMount;
  uint8_t zAfterHoming;
  float manualZOffset;
  uint32_t ledColor;
  bool adaptiveStepSmoothing;
  bool enablePreview;
} hmi_data_t;

extern hmi_data_t hmiData;
static constexpr size_t eeprom_data_size = sizeof(hmi_data_t);

typedef struct {
  int8_t Color[3];      // Color components
  #if ANY(HAS_PID_HEATING, MPCTEMP)
    tempcontrol_t tempControl = AUTOTUNE_DONE;
  #endif
  uint8_t select  = 0;  // Auxiliary selector variable
} hmi_value_t;

typedef struct {
  bool printing_flag:1; // sd or host printing
  bool abort_flag:1;    // sd or host was aborted
  bool pause_flag:1;    // printing is paused
  bool select_flag:1;   // Popup button selected
  bool config_flag:1;   // SD G-code file is a Configuration file
} hmi_flag_t;

extern hmi_value_t hmiValue;
extern hmi_flag_t hmiFlag;
extern uint8_t checkkey;

inline bool isPrinting() { return (printingIsActive() || print_job_timer.isPaused()); }
inline bool sdPrinting() { return (isPrinting() && IS_SD_FILE_OPEN()); }
inline bool hostPrinting() { return (isPrinting() && !IS_SD_FILE_OPEN()); }

// Popups
#if HAS_HOTEND || HAS_HEATED_BED
  void dwinPopupTemperature(const int_fast8_t heater_id, const bool toohigh);
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  void popupPowerLossRecovery();
#endif

// Tool Functions
#if ENABLED(EEPROM_SETTINGS)
  void writeEeprom();
  void readEeprom();
  void resetEeprom();
  #if HAS_MESH
    void saveMesh();
  #endif
#endif
void rebootPrinter();
void disableMotors();
void autoLev();
void autoHome();
#if HAS_PREHEAT
  #define _DOPREHEAT(N) void DoPreheat##N();
  REPEAT_1(PREHEAT_COUNT, _DOPREHEAT)
#endif
void doCoolDown();
#if ENABLED(BAUD_RATE_GCODE)
  void setBaud115K();
  void setBaud250K();
#endif
#if HAS_LCD_BRIGHTNESS
  void turnOffBacklight();
  #if LCD_BACKLIGHT_TIMEOUT_MINS
    void dimBacklight();
  #endif
#endif
void applyExtMinT();
void parkHead();
#if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
  void applyLEDColor();
#endif
#if ENABLED(AUTO_BED_LEVELING_UBL)
  void ublMeshTilt();
  void ublMeshSave();
  void ublMeshLoad();
#endif
#if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
  void hostShutDown();
#endif
#if DISABLED(HAS_BED_PROBE)
  void homeZandDisable();
#endif

// Other
void gotoPrintProcess();
void gotoMainMenu();
void gotoInfoMenu();
void gotoPowerLossRecovery();
void gotoConfirmToPrint();
void drawMainArea();      // Redraw main area
void dwinDrawStatusLine(const char *text); // Draw simple status text
inline void dwinDrawStatusLine(FSTR_P fstr) { dwinDrawStatusLine(FTOP(fstr)); }
void dwinRedrawDash();     // Redraw Dash and Status line
void dwinRedrawScreen();   // Redraw all screen elements
void hmiMainMenu();        // Main process screen
void hmiPrinting();        // Print page
void hmiReturnScreen();    // Return to previous screen before popups
void hmiWaitForUser();
void hmiSaveProcessID(const uint8_t id);
void hmiSDCardUpdate();
void eachMomentUpdate();
void updateVariable();
void dwinInitScreen();
void dwinHandleScreen();
void dwinCheckStatusMessage();
void dwinHomingStart();
void dwinHomingDone();
#if HAS_MESH
  void dwinMeshUpdate(const int8_t cpos, const int8_t tpos, const_float_t zval);
#endif
#if HAS_LEVELING
  void dwinLevelingStart();
  void dwinLevelingDone();
#endif
void dwinPrintStarted();
void dwinPrintPause();
void dwinPrintResume();
void dwinPrintFinished();
void dwinPrintAborted();
void dwinPrintHeader(const char *text);
void dwinSetColorDefaults();
void dwinSetColors();
void dwinCopySettingsTo(char * const buff);
void dwinCopySettingsFrom(const char * const buff);
void dwinSetDataDefaults();
void dwinRebootScreen();

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void dwinPopupPause(FSTR_P const fmsg, uint8_t button=0);
  void drawPopupFilamentPurge();
  void gotoFilamentPurge();
#endif

// Utility and extensions
#if HAS_LOCKSCREEN
  void dwinLockScreen();
  void dwinUnLockScreen();
  void hmiLockScreen();
#endif
#if HAS_MESH
  void dwinMeshViewer();
#endif
#if HAS_ESDIAG
  void drawEndStopDiag();
#endif

// Menu drawing functions
void drawFileMenu();
void drawControlMenu();
void drawAdvancedSettingsMenu();
void drawPrepareMenu();
void drawMoveMenu();

#if ENABLED(LCD_BED_TRAMMING)
  void drawTrammingMenu();
  #if HAS_TRAMMING_WIZARD
    void runTrammingWizard();
  #endif
#endif

#if HAS_HOME_OFFSET
  void drawHomeOffsetMenu();
#endif
#if HAS_BED_PROBE
  void drawProbeSetMenu();
#endif
void drawFilSetMenu();
#if ENABLED(NOZZLE_PARK_FEATURE)
  void drawParkPosMenu();
#endif
void drawPhySetMenu();
#if ALL(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
  void drawCaseLightMenu();
#endif
#if ENABLED(LED_CONTROL_MENU)
  void drawLedControlMenu ();
#endif
void drawTuneMenu();
void drawMotionMenu();
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void drawFilamentManMenu();
#endif
#if ENABLED(MESH_BED_LEVELING)
  void drawManualMeshMenu();
#endif
void drawTemperatureMenu();
void drawMaxSpeedMenu();
void drawMaxAccelMenu();
#if ENABLED(CLASSIC_JERK)
  void drawMaxJerkMenu();
#endif
void drawStepsMenu();
#if ANY(HAS_BED_PROBE, BABYSTEPPING)
  void drawZOffsetWizMenu();
#endif
#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void drawHomingMenu();
#endif
#if ENABLED(FWRETRACT)
  void drawFWRetractMenu();
#endif
#if HAS_MESH
  void drawMeshSetMenu();
  #if ENABLED(PROUI_EX)
    void drawMeshInsetMenu();
  #endif
  #if ENABLED(MESH_EDIT_MENU)
    void drawEditMeshMenu();
  #endif
#endif
#if HAS_TRINAMIC_CONFIG
  void drawTrinamicConfigMenu();
#endif

// Custom colors editing
#if HAS_CUSTOM_COLORS
  void dwinApplyColor(const int8_t element, const bool ldef=false);
  #if HAS_CUSTOM_COLORS_MENU
    void drawSelectColorsMenu();
    void drawGetColorMenu();
  #endif
#endif

// PID
#if HAS_PID_HEATING
  #include "../../../module/temperature.h"
  void dwinM303(const bool seenC, const int c, const bool seenS, const heater_id_t hid, const celsius_t temp);
  void dwinPidTuning(tempcontrol_t result);
#endif
#if ENABLED(PIDTEMP)
  #if ENABLED(PID_AUTOTUNE_MENU)
    void hotendPID();
  #endif
  #if ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
    void drawHotendPIDMenu();
  #endif
#endif
#if ENABLED(PIDTEMPBED)
  #if ENABLED(PID_AUTOTUNE_MENU)
    void bedPID();
  #endif
  #if ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
    void drawBedPIDMenu();
  #endif
#endif

// MPC
#if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
  void drawHotendMPCMenu();
#endif
#if ENABLED(MPC_AUTOTUNE)
  void dwinMPCTuning(tempcontrol_t result);
#endif

// CV Laser Module
#if ENABLED(CV_LASER_MODULE)
  void laserOn(const bool turn_on);
  void drawLaserSettingsMenu();
  void drawLaserPrintMenu();
#endif

// toolBar
#if HAS_TOOLBAR
  void drawTBSetupMenu();
  void gotoToolBar();
  void exitToolBar();
#endif

#if DEBUG_DWIN
  void dwinDebug(const char *msg);
#endif
