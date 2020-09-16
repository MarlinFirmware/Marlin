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

#include "../dwin_lcd.h"
#include "rotary_encoder.h"
#include "../../../libs/BL24CXX.h"

#include <stdint.h>

enum processID {
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
  #if HAS_HOTEND
    PLAPreheat,
    ABSPreheat,
  #endif
  MaxSpeed,
  MaxSpeed_value,
  MaxAcceleration,
  MaxAcceleration_value,
  MaxCorner,
  MaxCorner_value,
  Step,
  Step_value,

  // Last Process ID
  Last_Prepare,

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

// Picture ID
#define Start_Process       0
#define Language_English    1
#define Language_Chinese    2

// ICON ID
#define ICON                      0x09
#define ICON_LOGO                  0
#define ICON_Print_0               1
#define ICON_Print_1               2
#define ICON_Prepare_0             3
#define ICON_Prepare_1             4
#define ICON_Control_0             5
#define ICON_Control_1             6
#define ICON_Leveling_0            7
#define ICON_Leveling_1            8
#define ICON_HotendTemp            9
#define ICON_BedTemp              10
#define ICON_Speed                11
#define ICON_Zoffset              12
#define ICON_Back                 13
#define ICON_File                 14
#define ICON_PrintTime            15
#define ICON_RemainTime           16
#define ICON_Setup_0              17
#define ICON_Setup_1              18
#define ICON_Pause_0              19
#define ICON_Pause_1              20
#define ICON_Continue_0           21
#define ICON_Continue_1           22
#define ICON_Stop_0               23
#define ICON_Stop_1               24
#define ICON_Bar                  25
#define ICON_More                 26

#define ICON_Axis                 27
#define ICON_CloseMotor           28
#define ICON_Homing               29
#define ICON_SetHome              30
#define ICON_PLAPreheat           31
#define ICON_ABSPreheat           32
#define ICON_Cool                 33
#define ICON_Language             34

#define ICON_MoveX                35
#define ICON_MoveY                36
#define ICON_MoveZ                37
#define ICON_Extruder             38

#define ICON_Temperature          40
#define ICON_Motion               41
#define ICON_WriteEEPROM          42
#define ICON_ReadEEPROM           43
#define ICON_ResumeEEPROM         44
#define ICON_Info                 45

#define ICON_SetEndTemp           46
#define ICON_SetBedTemp           47
#define ICON_FanSpeed             48
#define ICON_SetPLAPreheat        49
#define ICON_SetABSPreheat        50

#define ICON_MaxSpeed             51
#define ICON_MaxAccelerated       52
#define ICON_MaxCorner            53
#define ICON_Step                 54
#define ICON_PrintSize            55
#define ICON_Version              56
#define ICON_Contact              57
#define ICON_StockConfiguraton    58
#define ICON_MaxSpeedX            59
#define ICON_MaxSpeedY            60
#define ICON_MaxSpeedZ            61
#define ICON_MaxSpeedE            62
#define ICON_MaxAccX              63
#define ICON_MaxAccY              64
#define ICON_MaxAccZ              65
#define ICON_MaxAccE              66
#define ICON_MaxSpeedCornerX      67
#define ICON_MaxSpeedCornerY      68
#define ICON_MaxSpeedCornerZ      69
#define ICON_MaxSpeedCornerE      70
#define ICON_StepX                71
#define ICON_StepY                72
#define ICON_StepZ                73
#define ICON_StepE                74
#define ICON_Setspeed             75
#define ICON_SetZOffset           76
#define ICON_Rectangle            77
#define ICON_BLTouch              78
#define ICON_TempTooLow           79
#define ICON_AutoLeveling         80
#define ICON_TempTooHigh          81
#define ICON_NoTips_C             82
#define ICON_NoTips_E             83
#define ICON_Continue_C           84
#define ICON_Continue_E           85
#define ICON_Cancel_C             86
#define ICON_Cancel_E             87
#define ICON_Confirm_C            88
#define ICON_Confirm_E            89
#define ICON_Info_0               90
#define ICON_Info_1               91

/**
 * 3-.0：The font size, 0x00-0x09, corresponds to the font size below:
 * 0x00=6*12   0x01=8*16   0x02=10*20  0x03=12*24  0x04=14*28
 * 0x05=16*32  0x06=20*40  0x07=24*48  0x08=28*56  0x09=32*64
 */
#define font6x12  0x00
#define font8x16  0x01
#define font10x20 0x02
#define font12x24 0x03
#define font14x28 0x04
#define font16x32 0x05
#define font20x40 0x06
#define font24x48 0x07
#define font28x56 0x08
#define font32x64 0x09

// Color
#define White             0xFFFF
#define Background_window 0x31E8  // Popup background color
#define Background_blue   0x1125  // Dark blue background color
#define Background_black  0x0841  // black background color
#define Font_window       0xD6BA  // Popup font background color
#define Line_Color        0x3A6A  // Split line color
#define Rectangle_Color   0xEE2F  // blue square cursor color
#define Percent_Color     0xFE29  // percentage color
#define BarFill_Color     0x10E4  // fill color of progress bar
#define Select_Color      0x33BB  // selected color

extern int checkkey, last_checkkey;
extern float zprobe_zoffset;
extern char print_filename[16];

extern millis_t dwin_heat_time;

typedef struct {
  TERN_(HAS_HOTEND,     int16_t E_Temp    = 0);
  TERN_(HAS_HEATED_BED, int16_t Bed_Temp  = 0);
  TERN_(HAS_FAN,        int16_t Fan_speed = 0);
  int16_t print_speed     = 100;
  float Max_Feedspeed     = 0;
  float Max_Acceleration  = 0;
  float Max_Corner        = 0;
  float Max_Step          = 0;
  float Move_X_scale      = 0;
  float Move_Y_scale      = 0;
  float Move_Z_scale      = 0;
  #if EXTRUDERS
    float Move_E_scale    = 0;
  #endif
  float offset_value      = 0;
  char show_mode          = 0;    // -1: Temperature control    0: Printing temperature
} HMI_value_t;

typedef struct {
  bool language_chinese;  // 0: EN, 1: CN
  bool pause_flag:1;
  bool print_finish:1;
  bool confirm_flag:1;
  bool select_flag:1;
  bool home_flag:1;
  bool heat_flag:1;  // 0: heating done  1: during heating
  #if HAS_HOTEND
    bool ETempTooLow_flag:1;
  #endif
  #if HAS_LEVELING
    bool leveling_offset_flag:1;
  #endif
  #if HAS_FAN
    char feedspeed_flag;
  #endif
  char acc_flag;
  char corner_flag;
  char step_flag;
} HMI_Flag;

extern HMI_value_t HMI_ValueStruct;
extern HMI_Flag    HMI_flag;

// Language
void HMI_SetLanguage(void);
void HMI_SetAndSaveLanguageWestern(void);
void HMI_SetAndSaveLanguageChinese(void);

// Show ICO
void ICON_Print(bool show);
void ICON_Prepare(bool show);
void ICON_Control(bool show);
void ICON_Leveling(bool show);
void ICON_StartInfo(bool show);

void ICON_Setting(bool show);
void ICON_Pause(bool show);
void ICON_Continue(bool show);
void ICON_Stop(bool show);

// Popup window tips
#if HAS_HOTEND
  void Popup_Window_Temperature(const bool toohigh);
  void Popup_Window_ETempTooLow(void);
#endif

void Popup_Window_Resume(void);
void Popup_Window_Home(void);
void Popup_Window_Leveling(void);

void Goto_PrintProcess(void);
void Goto_MainMenu(void);

// Variable control
void HMI_Move_X(void);
void HMI_Move_Y(void);
void HMI_Move_Z(void);
void HMI_Move_E(void);

void HMI_Zoffset(void);

TERN_(HAS_HOTEND,     void HMI_ETemp(void));
TERN_(HAS_HEATED_BED, void HMI_BedTemp(void));
TERN_(HAS_FAN,        void HMI_FanSpeed(void));

void HMI_PrintSpeed(void);

void HMI_MaxFeedspeedXYZE(void);
void HMI_MaxAccelerationXYZE(void);
void HMI_MaxCornerXYZE(void);
void HMI_StepXYZE(void);

void update_variable(void);
void DWIN_Draw_Signed_Float(uint8_t size, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value);

// SD Card
void HMI_SDCardInit(void);
void HMI_SDCardUpdate(void);

// Main Process
void Icon_print(bool value);
void Icon_control(bool value);
void Icon_temperature(bool value);
void Icon_leveling(bool value);

// Other
bool Pause_HeatStatus();
void HMI_StartFrame(const bool with_update); // startup screen
void HMI_MainMenu(void);          // main process screen
void HMI_SelectFile(void);        // file page
void HMI_Printing(void);          // print page
void HMI_Prepare(void);           // prepare page
void HMI_Control(void);           // control page
void HMI_Leveling(void);          // Level the page
void HMI_AxisMove(void);          // Axis movement menu
void HMI_Temperature(void);       // Temperature menu
void HMI_Motion(void);            // Sports menu
void HMI_Info(void);              // Information menu
void HMI_Tune(void);              // Adjust the menu

#if HAS_HOTEND
  void HMI_PLAPreheatSetting(void); // PLA warm-up setting
  void HMI_ABSPreheatSetting(void); // ABS warm-up setting
#endif

void HMI_MaxSpeed(void);          // Maximum speed submenu
void HMI_MaxAcceleration(void);   // Maximum acceleration submenu
void HMI_MaxCorner(void);         // Maximum corner speed submenu
void HMI_Step(void);              // transmission ratio

void HMI_Init(void);
void DWIN_Update(void);
void EachMomentUpdate(void);
void DWIN_HandleScreen(void);

void DWIN_CompletedHoming(void);
void DWIN_CompletedLeveling(void);
