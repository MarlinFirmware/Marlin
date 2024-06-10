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
 */

#include "dwin_lcd.h"
#include "../common/encoder.h"
#include "../common/limits.h"
#include "../../../libs/BL24CXX.h"

#include "../../../inc/MarlinConfigPre.h"

enum processID : uint8_t {
  // Process ID
  ID_MainMenu,
  ID_SelectFile,
  ID_Prepare,
  ID_Control,
  ID_Leveling,
  ID_PrintProcess,
  ID_AxisMove,
  ID_TemperatureID,
  ID_Motion,
  ID_Info,
  ID_Tune,
  #if HAS_PREHEAT
    ID_PLAPreheat,
    #if PREHEAT_COUNT > 1
      ID_ABSPreheat,
    #endif
  #endif
  ID_MaxSpeed, ID_MaxSpeedValue,
  ID_MaxAcceleration, ID_MaxAccelerationValue,
  ID_MaxJerk, ID_MaxJerkValue,
  ID_Step, ID_StepValue,
  ID_HomeOff, ID_HomeOffX, ID_HomeOffY, ID_HomeOffZ,

  // Last Process ID
  ID_LastPrepare,

  // Advance Settings
  ID_AdvSet,
  ID_ProbeOff, ID_ProbeOffX, ID_ProbeOffY,

  // Back Process ID
  ID_BackMain, ID_BackPrint,

  // Date variable ID
  ID_MoveX, ID_MoveY, ID_MoveZ,
  #if HAS_HOTEND
    ID_Extruder,
    ID_ETemp,
  #endif
  ID_HomeOffset,
  #if HAS_HEATED_BED
    ID_BedTemp,
  #endif
  #if HAS_FAN
    ID_FanSpeed,
  #endif
  ID_PrintSpeed,

  // Window ID
  ID_PrintWindow, ID_PopupWindow
};

extern uint8_t checkkey;
extern float zprobe_zoffset;
extern char print_filename[16];

extern millis_t dwin_heat_time;

typedef struct {
  #if HAS_HOTEND
    celsius_t tempE = 0;
  #endif
  #if HAS_HEATED_BED
    celsius_t tempBed = 0;
  #endif
  #if HAS_FAN
    int16_t fanSpeed = 0;
  #endif
  int16_t printSpeed    = 100;
  float maxFeedSpeed    = 0;
  float maxAcceleration = 0;
  float maxJerkScaled   = 0;
  float maxStepScaled   = 0;
  float offset_value    = 0;
  int8_t show_mode      = 0; // -1: Temperature control    0: Printing temperature
  struct {
    #if HAS_X_AXIS
      float x = 0;
    #endif
    #if HAS_Y_AXIS
      float y = 0;
    #endif
    #if HAS_Z_AXIS
      float z = 0;
    #endif
    #if HAS_HOTEND
      float e = 0;
    #endif
  } moveScaled;
  struct {
    #if HAS_X_AXIS
      float x = 0;
    #endif
    #if HAS_Y_AXIS
      float y = 0;
    #endif
    #if HAS_Z_AXIS
      float z = 0;
    #endif
  } homeOffsScaled;
  struct {
    #if HAS_X_AXIS
      float x = 0;
    #endif
    #if HAS_Y_AXIS
      float y = 0;
    #endif
  } probeOffsScaled;
} hmi_value_t;

#define DWIN_CHINESE 123
#define DWIN_ENGLISH 0

typedef struct {
  uint8_t language;
  bool pause_flag:1;    // printing is paused
  bool pause_action:1;  // flag a pause action
  bool print_finish:1;  // print was finished
  bool select_flag:1;   // Popup button selected
  bool home_flag:1;     // homing in course
  bool heat_flag:1;     // 0: heating done  1: during heating
  bool done_confirm_flag:1;
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    bool cold_flag:1;
  #endif
  AxisEnum feedspeed_axis, acc_axis, jerk_axis, step_axis;
} hmi_flag_t;

extern hmi_value_t hmiValues;
extern hmi_flag_t hmiFlag;

#if HAS_HOTEND || HAS_HEATED_BED
  // Popup message window
  void dwinPopupTemperature(const bool toohigh);
#endif

#if HAS_HOTEND
  void popupWindowETempTooLow();
#endif

void popupWindowResume();
void popupWindowHome(const bool parking=false);
void popupWindowLeveling();

void gotoPrintProcess();
void gotoMainMenu();

// Variable control
void hmiMoveX();
void hmiMoveY();
void hmiMoveZ();
void hmiMoveE();

void hmiZoffset();

#if HAS_HOTEND
  void hmiETemp();
#endif
#if HAS_HEATED_BED
  void hmiBedTemp();
#endif
#if HAS_FAN
  void hmiFanSpeed();
#endif

void hmiPrintSpeed();

void hmiMaxFeedspeedXYZE();
void hmiMaxAccelerationXYZE();
void hmiMaxJerkXYZE();
#if ENABLED(EDITABLE_STEPS_PER_UNIT)
  void hmiStepXYZE();
#endif

void hmiSetLanguageCache();

void updateVariable();
void dwinDrawSignedFloat(uint8_t size, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value);

// SD Card
void hmiSDCardInit();
void hmiSDCardUpdate();

// Other
void drawStatusArea(const bool with_update); // Status Area
void hmiStartFrame(const bool with_update);   // Prepare the menu view
void hmiMainMenu();    // Main process screen
void hmiSelectFile();  // File page
void hmiPrinting();    // Print page
void hmiPrepare();     // Prepare page
void hmiControl();     // Control page
void hmiLeveling();    // Level the page
void hmiAxisMove();    // Axis movement menu
void hmiTemperature(); // Temperature menu
void hmiMotion();      // Sports menu
void hmiInfo();        // Information menu
void hmiTune();        // Adjust the menu

#if HAS_PREHEAT
  void hmiPLAPreheatSetting(); // PLA warm-up setting
  void hmiABSPreheatSetting(); // ABS warm-up setting
#endif

void hmiMaxSpeed();        // Maximum speed submenu
void hmiMaxAcceleration(); // Maximum acceleration submenu
void hmiMaxJerk();         // Maximum jerk speed submenu
void hmiStep();            // Transmission ratio

void hmiInit();
void dwinInitScreen();
void eachMomentUpdate();
void dwinHandleScreen();
void dwinStatusChanged(const char * const cstr=nullptr);
void dwinStatusChanged(FSTR_P const fstr);

inline void dwinHomingStart() { hmiFlag.home_flag = true; }

void dwinHomingDone();
void dwinLevelingDone();
