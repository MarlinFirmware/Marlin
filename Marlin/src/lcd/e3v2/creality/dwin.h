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
  MainMenu,
  SelectFile,
  Prepare,
  Control,
  Leveling,
  PrintProcess,
  AxisMove,
  TemperatureID,
  Motion,
  Info,
  Tune,
  #if HAS_PREHEAT
    PLAPreheat,
    #if PREHEAT_COUNT > 1
      ABSPreheat,
    #endif
  #endif
  MaxSpeed,
  MaxSpeed_value,
  MaxAcceleration,
  MaxAcceleration_value,
  MaxJerk,
  MaxJerk_value,
  Step,
  Step_value,
  HomeOff,
  HomeOffX,
  HomeOffY,
  HomeOffZ,

  // Last Process ID
  Last_Prepare,

  // Advance Settings
  AdvSet,
  ProbeOff,
  ProbeOffX,
  ProbeOffY,

  // Back Process ID
  Back_Main,
  Back_Print,

  // Date variable ID
  Move_X,
  Move_Y,
  Move_Z,
  #if HAS_HOTEND
    Extruder,
    ETemp,
  #endif
  Homeoffset,
  #if HAS_HEATED_BED
    BedTemp,
  #endif
  #if HAS_FAN
    FanSpeed,
  #endif
  PrintSpeed,

  // Window ID
  Print_window,
  Popup_Window
};

extern uint8_t checkkey;
extern float zprobe_zoffset;
extern char print_filename[16];

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
  float Move_X_scaled     = 0;
  float Move_Y_scaled     = 0;
  float Move_Z_scaled     = 0;
  #if HAS_HOTEND
    float Move_E_scaled   = 0;
  #endif
  float offset_value      = 0;
  int8_t show_mode        = 0; // -1: Temperature control    0: Printing temperature
  float Home_OffX_scaled  = 0;
  float Home_OffY_scaled  = 0;
  float Home_OffZ_scaled  = 0;
  float Probe_OffX_scaled = 0;
  float Probe_OffY_scaled = 0;
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
void hmiStepXYZE();
void hmiSetLanguageCache();

void update_variable();
void dwinDrawSigned_Float(uint8_t size, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value);

// SD Card
void hmiSDCardInit();
void hmiSDCardUpdate();

// Main Process
void Icon_print(bool value);
void Icon_control(bool value);
void Icon_temperature(bool value);
void Icon_leveling(bool value);

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
void dwinUpdate();
void eachMomentUpdate();
void dwinHandleScreen();
void dwinStatusChanged(const char * const cstr=nullptr);
void dwinStatusChanged(FSTR_P const fstr);

inline void dwinHomingStart() { hmiFlag.home_flag = true; }

void dwinHomingDone();
void dwinLevelingDone();
