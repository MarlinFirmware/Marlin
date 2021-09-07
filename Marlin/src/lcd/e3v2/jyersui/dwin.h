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
 * lcd/e3v2/jyersui/dwin.h
 */

#include "dwin_lcd.h"
#include "rotary_encoder.h"
#include "../../../libs/BL24CXX.h"
#include "../../../inc/MarlinConfigPre.h"

enum processID : uint8_t {
  Main, Print, Menu, Value, Option, File, Popup, Confirm, Wait
};

enum PopupID : uint8_t {
  Pause, Stop, Resume, SaveLevel, ETemp, ConfFilChange, PurgeMore, MeshSlot,
  Level, Home, MoveWait, Heating,  FilLoad, FilChange, TempWarn, Runout, PIDWait, Resuming, ManualProbing,
  FilInsert, HeaterTime, UserInput, LevelError, InvalidMesh, UI, Complete
};

enum menuID : uint8_t {
  MainMenu,
    Prepare,
      Move,
      HomeMenu,
      ManualLevel,
      ZOffset,
      Preheat,
      ChangeFilament,
    Control,
      TempMenu,
        PID,
          HotendPID,
          BedPID,
        Preheat1,
        Preheat2,
        Preheat3,
        Preheat4,
        Preheat5,
      Motion,
        HomeOffsets,
        MaxSpeed,
        MaxAcceleration,
        MaxJerk,
        Steps,
      Visual,
        ColorSettings,
      Advanced,
        ProbeMenu,
      Info,
    Leveling,
      LevelManual,
      LevelView,
      MeshViewer,
      LevelSettings,
      ManualMesh,
      UBLMesh,
    InfoMain,
  Tune,
  PreheatHotend
};

#define Start_Process       0
#define Language_English    1
#define Language_Chinese    2

#define ICON                7 // Icon set file 7.ICO

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

// Custom icons
#if ENABLED(DWIN_CREALITY_LCD_CUSTOM_ICONS)
  // index of every custom icon should be >= CUSTOM_ICON_START
  #define CUSTOM_ICON_START         ICON_Checkbox_F
  #define ICON_Checkbox_F           200
  #define ICON_Checkbox_T           201
  #define ICON_Fade                 202
  #define ICON_Mesh                 203
  #define ICON_Tilt                 204
  #define ICON_Brightness           205
  #define ICON_AxisD                249
  #define ICON_AxisBR               250
  #define ICON_AxisTR               251
  #define ICON_AxisBL               252
  #define ICON_AxisTL               253
  #define ICON_AxisC                254
#else
  #define ICON_Fade                 ICON_Version
  #define ICON_Mesh                 ICON_Version
  #define ICON_Tilt                 ICON_Version
  #define ICON_Brightness           ICON_Version
  #define ICON_AxisD                ICON_Axis
  #define ICON_AxisBR               ICON_Axis
  #define ICON_AxisTR               ICON_Axis
  #define ICON_AxisBL               ICON_Axis
  #define ICON_AxisTL               ICON_Axis
  #define ICON_AxisC                ICON_Axis
#endif

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

enum colorID : uint8_t {
  Default, White, Green, Cyan, Blue, Magenta, Red, Orange, Yellow, Brown, Black
};

#define Custom_Colors       10
#define Color_White         0xFFFF
#define Color_Light_White   0xBDD7
#define Color_Green         0x07E0
#define Color_Light_Green   0x3460
#define Color_Cyan          0x07FF
#define Color_Light_Cyan    0x04F3
#define Color_Blue          0x015F
#define Color_Light_Blue    0x3A6A
#define Color_Magenta       0xF81F
#define Color_Light_Magenta 0x9813
#define Color_Red           0xF800
#define Color_Light_Red     0x8800
#define Color_Orange        0xFA20
#define Color_Light_Orange  0xFBC0
#define Color_Yellow        0xFF0F
#define Color_Light_Yellow  0x8BE0
#define Color_Brown         0xCC27
#define Color_Light_Brown   0x6204
#define Color_Black         0x0000
#define Color_Grey          0x18E3
#define Color_Bg_Window     0x31E8  // Popup background color
#define Color_Bg_Blue       0x1125  // Dark blue background color
#define Color_Bg_Black      0x0841  // Black background color
#define Color_Bg_Red        0xF00F  // Red background color
#define Popup_Text_Color    0xD6BA  // Popup font background color
#define Line_Color          0x3A6A  // Split line color
#define Rectangle_Color     0xEE2F  // Blue square cursor color
#define Percent_Color       0xFE29  // Percentage color
#define BarFill_Color       0x10E4  // Fill color of progress bar
#define Select_Color        0x33BB  // Selected color
#define Check_Color         0x4E5C  // Check-box check color
#define Confirm_Color   	  0x34B9
#define Cancel_Color        0x3186

class CrealityDWINClass {
public:
  static constexpr size_t eeprom_data_size = 48;
  static struct EEPROM_Settings { // use bit fields to save space, max 48 bytes
    bool time_format_textual : 1;
    bool beeperenable : 1;
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      uint8_t tilt_grid_size : 3;
    #endif
    uint16_t corner_pos : 10;
    uint8_t cursor_color : 4;
    uint8_t menu_split_line : 4;
    uint8_t menu_top_bg : 4;
    uint8_t menu_top_txt : 4;
    uint8_t highlight_box : 4;
    uint8_t progress_percent : 4;
    uint8_t progress_time : 4;
    uint8_t status_bar_text : 4;
    uint8_t status_area_text : 4;
    uint8_t coordinates_text : 4;
    uint8_t coordinates_split_line : 4;
  } eeprom_settings;

  static constexpr const char * const color_names[11] = { "Default", "White", "Green", "Cyan", "Blue", "Magenta", "Red", "Orange", "Yellow", "Brown", "Black" };
  static constexpr const char * const preheat_modes[3] = { "Both", "Hotend", "Bed" };

  static void Clear_Screen(uint8_t e=3);
  static void Draw_Float(float value, uint8_t row, bool selected=false, uint8_t minunit=10);
  static void Draw_Option(uint8_t value, const char * const * options, uint8_t row, bool selected=false, bool color=false);
  static uint16_t GetColor(uint8_t color, uint16_t original, bool light=false);
  static void Draw_Checkbox(uint8_t row, bool value);
  static void Draw_Title(const char * title);
  static void Draw_Menu_Item(uint8_t row, uint8_t icon=0, const char * const label1=nullptr, const char * const label2=nullptr, bool more=false, bool centered=false);
  static void Draw_Menu(uint8_t menu, uint8_t select=0, uint8_t scroll=0);
  static void Redraw_Menu(bool lastprocess=true, bool lastselection=false, bool lastmenu=false);
  static void Redraw_Screen();

  static void Main_Menu_Icons();
  static void Draw_Main_Menu(uint8_t select=0);
  static void Print_Screen_Icons();
  static void Draw_Print_Screen();
  static void Draw_Print_Filename(const bool reset=false);
  static void Draw_Print_ProgressBar();
  #if ENABLED(USE_M73_REMAINING_TIME)
    static void Draw_Print_ProgressRemain();
  #endif
  static void Draw_Print_ProgressElapsed();
  static void Draw_Print_confirm();
  static void Draw_SD_Item(uint8_t item, uint8_t row);
  static void Draw_SD_List(bool removed=false);
  static void Draw_Status_Area(bool icons=false);
  static void Draw_Popup(PGM_P const line1, PGM_P const line2, PGM_P const line3, uint8_t mode, uint8_t icon=0);
  static void Popup_Select();
  static void Update_Status_Bar(bool refresh=false);

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    static void Draw_Bed_Mesh(int16_t selected = -1, uint8_t gridline_width = 1, uint16_t padding_x = 8, uint16_t padding_y_top = 40 + 53 - 7);
    static void Set_Mesh_Viewer_Status();
  #endif

  static const char * Get_Menu_Title(uint8_t menu);
  static uint8_t Get_Menu_Size(uint8_t menu);
  static void Menu_Item_Handler(uint8_t menu, uint8_t item, bool draw=true);

  static void Popup_Handler(PopupID popupid, bool option = false);
  static void Confirm_Handler(PopupID popupid);

  static void Main_Menu_Control();
  static void Menu_Control();
  static void Value_Control();
  static void Option_Control();
  static void File_Control();
  static void Print_Screen_Control();
  static void Popup_Control();
  static void Confirm_Control();

  static void Setup_Value(float value, float min, float max, float unit, uint8_t type);
  static void Modify_Value(float &value, float min, float max, float unit, void (*f)()=nullptr);
  static void Modify_Value(uint8_t &value, float min, float max, float unit, void (*f)()=nullptr);
  static void Modify_Value(uint16_t &value, float min, float max, float unit, void (*f)()=nullptr);
  static void Modify_Value(int16_t &value, float min, float max, float unit, void (*f)()=nullptr);
  static void Modify_Value(uint32_t &value, float min, float max, float unit, void (*f)()=nullptr);
  static void Modify_Value(int8_t &value, float min, float max, float unit, void (*f)()=nullptr);
  static void Modify_Option(uint8_t value, const char * const * options, uint8_t max);

  static void Update_Status(const char * const text);
  static void Start_Print(bool sd);
  static void Stop_Print();
  static void Update();
  static void State_Update();
  static void Screen_Update();
  static void AudioFeedback(const bool success=true);
  static void Save_Settings(char *buff);
  static void Load_Settings(const char *buff);
  static void Reset_Settings();
};

extern CrealityDWINClass CrealityDWIN;
