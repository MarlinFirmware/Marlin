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
 * Enhanced implementation by Miguel A. Risco-Castillo
 */

#include "../dwin_lcd.h"
#include "rotary_encoder.h"
#include "../../../libs/BL24CXX.h"

#include "../../../inc/MarlinConfigPre.h"

#if ANY(HAS_HOTEND, HAS_HEATED_BED, HAS_FAN) && PREHEAT_COUNT
  #define HAS_PREHEAT 1
  #if PREHEAT_COUNT < 2
    #error "Creality DWIN requires two material preheat presets."
  #endif
#endif

static constexpr size_t eeprom_data_size = 64;

enum processID : uint8_t {
  // Process ID
  MainMenu,
  SelectFile,
  Prepare,
  Control,
  PrintProcess,
  PrintDone,
  FilamentMan,
  AxisMove,
  #if ENABLED(ASSISTED_TRAMMING)
    Tramming,
  #endif
  ManualMesh,
  MMeshMoveZ,
  TemperatureID,
  Motion,
  Info,
  Tune,
  TuneFlow,
  #if HAS_PREHEAT
    PLAPreheat,
    ABSPreheat,
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
  // Advance Settings
  AdvSet,
  ProbeOff,
  ProbeOffX,
  ProbeOffY,
  Brightness,
  LoadLength,
  UnloadLength,
  SelColor,
  GetColor,
  GetColor_value,

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
  PrintFlow,

  // Popup Windows
  Homing,
  Leveling,
  PauseOrStop,
  FilamentPurge,
  WaitResponse,
  NothingToDo,
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

#define ICON_Error                ICON_TempTooHigh
#define ICON_Tramming             ICON_SetEndTemp
#define ICON_ManualMesh           ICON_HotendTemp
#define ICON_MeshNext             ICON_Axis
#define ICON_MeshSave             ICON_WriteEEPROM
#define ICON_AdvSet               ICON_Language
#define ICON_Reboot               ICON_ResumeEEPROM
#define ICON_FilMan               ICON_ResumeEEPROM
#define ICON_FilLoad              ICON_WriteEEPROM
#define ICON_FilUnload            ICON_ReadEEPROM
#define ICON_HomeOff              ICON_AdvSet
#define ICON_HomeOffX             ICON_StepX
#define ICON_HomeOffY             ICON_StepY
#define ICON_HomeOffZ             ICON_StepZ
#define ICON_ProbeOff             ICON_AdvSet
#define ICON_ProbeOffX            ICON_StepX
#define ICON_ProbeOffY            ICON_StepY
#define ICON_PIDNozzle            ICON_SetEndTemp
#define ICON_PIDbed               ICON_SetBedTemp
#define ICON_Flow                 ICON_StepE
#define ICON_Pwrlossr             ICON_Motion
#define ICON_Park                 ICON_Motion
#define ICON_Scolor               ICON_MaxSpeed

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
#define Color_White       0xFFFF
#define Color_Yellow      0xFF0F
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

// Default UI Colors
#define RGB(R,G,B)  (R << 11) | (G << 5) | (B) // R,B: 0..31; G: 0..63
#define Def_Background_Color  Color_Bg_Black
#define Def_Cursor_color      Rectangle_Color
#define Def_TitleBg_color     Color_Bg_Blue
#define Def_TitleTxt_color    Color_White
#define Def_Text_Color        Color_White
#define Def_Selected_Color    Select_Color
#define Def_SplitLine_Color   Line_Color
#define Def_Highlight_Color   Color_White
#define Def_StatusBg_Color    RGB(0,20,20)
#define Def_StatusTxt_Color   Color_Yellow
#define Def_PopupBg_color     Color_Bg_Window
#define Def_PopupTxt_Color    Popup_Text_Color
#define Def_AlertBg_Color     Color_Bg_Red
#define Def_AlertTxt_Color    Color_Yellow
#define Def_PercentTxt_Color  Percent_Color
#define Def_Barfill_Color     BarFill_Color
#define Def_Indicator_Color   Color_White
#define Def_Coordinate_Color   Color_White

extern uint8_t checkkey;
extern float zprobe_zoffset;
extern char print_filename[16];

extern millis_t dwin_heat_time;

typedef struct {
  #if ENABLED(HAS_HOTEND)
    celsius_t E_Temp = 0;
  #endif
  #if ENABLED(HAS_HEATED_BED)
    celsius_t Bed_Temp = 0;
  #endif
  #if ENABLED(HAS_FAN)
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
  int16_t print_flow      = 100;
  int16_t Brightness      = 127;
  int8_t Color[3];
} HMI_value_t;

typedef struct {
  uint8_t Brightness = 127;
  uint16_t Background_Color = Def_Background_Color;
  uint16_t Cursor_color     = Def_Cursor_color;
  uint16_t TitleBg_color    = Def_TitleBg_color;
  uint16_t TitleTxt_color   = Def_TitleTxt_color;
  uint16_t Text_Color       = Def_Text_Color;
  uint16_t Selected_Color   = Def_Selected_Color;
  uint16_t SplitLine_Color  = Def_SplitLine_Color;
  uint16_t Highlight_Color  = Def_Highlight_Color;
  uint16_t StatusBg_Color   = Def_StatusBg_Color;
  uint16_t StatusTxt_Color  = Def_StatusTxt_Color;
  uint16_t PopupBg_color    = Def_PopupBg_color;
  uint16_t PopupTxt_Color   = Def_PopupTxt_Color;
  uint16_t AlertBg_Color    = Def_AlertBg_Color;
  uint16_t AlertTxt_Color   = Def_AlertTxt_Color;
  uint16_t PercentTxt_Color = Def_PercentTxt_Color;
  uint16_t Barfill_Color    = Def_Barfill_Color;
  uint16_t Indicator_Color  = Def_Indicator_Color;
  uint16_t Coordinate_Color = Def_Coordinate_Color;
} HMI_data_t;

#define DWIN_CHINESE 123
#define DWIN_ENGLISH 0

typedef struct {
  uint8_t language;
  bool pause_flag:1;
  bool pause_action:1;
  bool print_finish:1;
  bool select_flag:1;
  bool home_flag:1;
  bool heat_flag:1;  // 0: heating done  1: during heating
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    bool ETempTooLow_flag:1;
  #endif
  #if HAS_LEVELING
    bool leveling_offset_flag:1;
  #endif
  AxisEnum feedspeed_axis, acc_axis, jerk_axis, step_axis;
} HMI_Flag_t;

extern HMI_value_t HMI_ValueStruct;
extern HMI_Flag_t HMI_flag;
extern HMI_data_t HMI_data;

enum pidresult_t : uint8_t { PID_BAD_EXTRUDER_NUM, PID_TEMP_TOO_HIGH, PID_TUNING_TIMEOUT, PID_EXTR_START, PID_BED_START, PID_DONE };

// Show ICO
void ICON_Print();
void ICON_Prepare();
void ICON_Control();
void ICON_Leveling(bool show);
void ICON_StartInfo(bool show);

void ICON_Pause();
void ICON_Continue();
void ICON_Stop();

void DWIN_Draw_Popup(uint8_t icon, const char *msg1, const char *msg2, uint8_t button = 0);
void DWIN_Popup_Continue(uint8_t icon, const char *msg1, const char *msg2);
void DWIN_Popup_Confirm(uint8_t icon, const char *msg1, const char *msg2);

#if HAS_HOTEND || HAS_HEATED_BED
  // Popup message window
  void DWIN_Popup_Temperature(const bool toohigh);
#endif

#if HAS_HOTEND
  void Popup_Window_ETempTooLow();
#endif

void Popup_Window_Resume();

void Goto_PrintProcess();
void Goto_Main_Menu();

// Variable control
void HMI_Move_X();
void HMI_Move_Y();
void HMI_Move_Z();
void HMI_Move_E();

void HMI_Zoffset();

#if ENABLED(HAS_HOTEND)
  void HMI_ETemp();
#endif
#if ENABLED(HAS_HEATED_BED)
  void HMI_BedTemp();
#endif
#if ENABLED(HAS_FAN)
  void HMI_FanSpeed();
#endif

void HMI_PrintSpeed();

void HMI_MaxFeedspeedXYZE();
void HMI_MaxAccelerationXYZE();
void HMI_MaxJerkXYZE();
void HMI_StepXYZE();

void update_variable();
void DWIN_Draw_Signed_Float(uint8_t size, uint16_t bColor, uint8_t iNum, uint8_t fNum, uint16_t x, uint16_t y, long value);

// SD Card
void HMI_SDCardInit();
void HMI_SDCardUpdate();

// Main Process
void Icon_print();
void Icon_control();
void Icon_leveling(bool value);

// Other
void Draw_Status_Area(const bool with_update); // Status Area
void HMI_StartFrame(const bool with_update);   // Prepare the menu view
void HMI_MainMenu();    // Main process screen
void HMI_SelectFile();  // File page
void HMI_Printing();    // Print page
void HMI_Prepare();     // Prepare page
void HMI_Control();     // Control page
void HMI_Leveling();    // Level the page
void HMI_AxisMove();    // Axis movement menu
#if ENABLED(ASSISTED_TRAMMING)
  void HMI_Tramming();   // Tramming menu
#endif
#if ENABLED(MESH_BED_LEVELING)
  void HMI_ManualMesh();  // Manual Mesh menu
  void HMI_MMeshMoveZ();  // Manual Mesh move Z
#endif
void HMI_Temperature(); // Temperature menu
void HMI_Motion();      // Sports menu
void HMI_Info();        // Information menu
void HMI_Tune();        // Adjust the menu
void Draw_Main_Area(uint8_t procID); // Redraw main area;
void HMI_ReturnScreen();// Return to previous screen before popups

#if HAS_PREHEAT
  void HMI_PLAPreheatSetting(); // PLA warm-up setting
  void HMI_ABSPreheatSetting(); // ABS warm-up setting
#endif

void HMI_MaxSpeed();        // Maximum speed submenu
void HMI_MaxAcceleration(); // Maximum acceleration submenu
void HMI_MaxJerk();         // Maximum jerk speed submenu
void HMI_Step();            // Transmission ratio

void HMI_Init();
void HMI_Popup();
void HMI_SaveProcessID(const uint8_t id);
void HMI_AudioFeedback(const bool success/*=true*/);
void DWIN_Update();
void EachMomentUpdate();
void DWIN_HandleScreen();
void DWIN_DrawStatusLine(const uint16_t color, const uint16_t bgcolor, const char *text);
void DWIN_StatusChanged_P(PGM_P const text);
void DWIN_StatusChanged(const char * const text);
void DWIN_Draw_Checkbox(uint16_t color, uint16_t bcolor, uint16_t x, uint16_t y, bool mode);
void DWIN_StartHoming();
void DWIN_CompletedHoming();
#if ENABLED(MESH_BED_LEVELING)
  void DWIN_MeshUpdate(const int8_t xpos, const int8_t ypos, const float zval);
#endif
void DWIN_MeshLevelingStart();
void DWIN_CompletedLeveling();
void DWIN_PidTuning(pidresult_t result);
void DWIN_Print_Started(const bool sd);
void DWIN_Print_Finished();
#if HAS_FILAMENT_SENSOR
  void DWIN_FilamentRunout(const uint8_t extruder);
#endif
void DWIN_Progress_Update(uint8_t percent, uint32_t remaining);
void DWIN_Print_Header(const char *text);
void DWIN_SetColorDefaults();
void DWIN_StoreSettings(char *buff);
void DWIN_LoadSettings(const char *buff);
void DWIN_Setdatadefaults();
void DWIN_RebootScreen();

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void Draw_Popup_FilamentPurge();
  void DWIN_Popup_FilamentPurge();
  void HMI_FilamentPurge();
#endif
