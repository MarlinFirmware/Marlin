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
 * Rewrite by Jacob Myers
 */

#include "../dwin_lcd.h"
#include "rotary_encoder.h"
#include "../../../libs/BL24CXX.h"
#include "../../../inc/MarlinConfigPre.h"

enum processID : uint8_t {
  Main, Print, Menu, Value, File, Popup, Confirm, Wait
};

enum popupID : uint8_t {
  Pause, Stop, Resume, SaveLevel, ETemp, Level, Home, MoveWait, Complete, M600, FilLoad, FilChange
};

enum menuID : uint8_t {
  MainMenu,
    Prepare,
      Move,
      ManualLevel,
      ZOffset,
      Preheat,
      ChangeFilament,
    Control,
      TempMenu,
        Preheat1,
        Preheat2,
        Preheat3,
        Preheat4,
      Motion,
        MaxSpeed,
        MaxAcceleration,
        MaxJerk,
        Steps,
      Advanced,
      Info,
    ManualMesh,
    InfoMain,
  Tune
};


#define Start_Process       0
#define Language_English    1
#define Language_Chinese    2

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
#define ICON_MaxJerk              53
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
#define ICON_MaxSpeedJerkX        67
#define ICON_MaxSpeedJerkY        68
#define ICON_MaxSpeedJerkZ        69
#define ICON_MaxSpeedJerkE        70
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

#define Color_White       0xFFFF
#define Color_Yellow      0xFF0F
#define Color_Grey        0x18E3
#define Color_Bg_Window   0x31E8  // Popup background color
#define Color_Bg_Blue     0x1125  // Dark blue background color
#define Color_Bg_Black    0x0841  // Black background color
#define Color_Bg_Red      0xF00F  // Red background color
#define Popup_Text_Color  0xD6BA  // Popup font background color
#define Line_Color        0x3A6A  // Split line color
#define Rectangle_Color   0xEE2F  // Blue square cursor color
#define Percent_Color     0xFE29  // Percentage color
#define BarFill_Color     0x10E4  // Fill color of progress bar
#define Select_Color      0x33BB  // Selected color

extern millis_t dwin_heat_time;

inline void Clear_Screen(uint8_t e=3);
inline void Draw_Float(float value, uint8_t row, bool selected=false, uint8_t minunit=10);
inline void Draw_Title(char* title);
inline void Draw_Menu_Item(uint8_t row, uint8_t icon=0, char * const label=(char*)"", bool more=false);
inline void Draw_Menu(uint8_t menu, uint8_t select=0, uint8_t scroll=0);


void Main_Menu_Icons();
void Draw_Main_Menu(uint8_t select=0);
void Print_Screen_Icons();
void Draw_Print_Screen();
void Draw_Print_ProgressBar();
void Draw_Print_ProgressRemain();
void Draw_Print_ProgressElapsed();
void Draw_Print_confirm();
void Draw_SD_Item(uint8_t item, uint8_t row);
void Draw_SD_List(bool removed=false);
void Draw_Status_Area(const bool with_update);
void Update_Status_Area();
void Draw_Popup(char *line1, char *line2, char *line3, uint8_t mode, uint8_t icon=0);


char* Get_Menu_Title(uint8_t menu);
int Get_Menu_Size(uint8_t menu);
void Menu_Item_Handler(uint8_t menu, uint8_t item, bool draw=true);


void Popup_Select();
void Popup_Handler(uint8_t popupid, bool option = false);
void DWIN_Popup_Temperature(const bool toohigh);


inline void Main_Menu_Control();
inline void Menu_Control();
inline void Value_Control();
inline void File_Control();
inline void Print_Screen_Control();
inline void Popup_Control();
inline void Confirm_Control();


void Setup_Value(float value, float min, float max, float unit, uint8_t type);
void Modify_Value(float &value, float min, float max, float unit);
void Modify_Value(uint8_t &value, float min, float max, float unit);
void Modify_Value(uint16_t &value, float min, float max, float unit);
void Modify_Value(int16_t &value, float min, float max, float unit);
void Modify_Value(uint32_t &value, float min, float max, float unit);


void Host_Print_Update(uint8_t percent, uint32_t remaining);
void Host_Print_Text(char * const text);


void Start_Print(bool sd);
void Stop_Print();
void DWIN_Update();
void Variable_Update();
void Screen_Update();
void HMI_Init();
void HMI_StartFrame(const bool with_update);
void AudioFeedback(const bool success=true);
inline void HMI_SDCardInit();