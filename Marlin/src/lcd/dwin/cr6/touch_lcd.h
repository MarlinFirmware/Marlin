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

#include "string.h"
#include <arduino.h>

#include "../../../libs/BL24CXX.h"

/*********************************/
#define FHONE   (0x5A)
#define FHTWO   (0xA5)
#define FHLENG  (0x06)
#define TEXTBYTELEN     18
#define MaxFileNumber   20

#define AUTO_BED_LEVEL_PREHEAT  120

#define FileNum             MaxFileNumber
#define FileNameLen         TEXTBYTELEN
#define RTS_UPDATE_INTERVAL 2000
#define RTS_UPDATE_VALUE    RTS_UPDATE_INTERVAL

#define SizeofDatabuf       26

#define Z_VALUE_EEPROM   500

#define Retry_num   2

/*************Register and Variable addr*****************/
#define RegAddr_W   0x80
#define RegAddr_R   0x81
#define VarAddr_W   0x82
#define VarAddr_R   0x83

// According to the DWIN docs for operation 0x84 (ExchangepageAddr) this is:
//  0x5A: Enable page operation once, clear after CPU operation
//  0x01: Page switch (display the selected picture)
//  rest: Picture ID
#define ExchangePageBase    ((unsigned long)0x5A010000)     // the first page ID. other page = first page ID + relevant num;
#define StartSoundSet       ((unsigned long)0x060480A0)     // 06,start-music; 04, 4 musics; 80, the volume value; 04, return value about music number.

/*Error value*/
#define Error_201   "201 (Command Timeout)"   // The command too much inactive time
#define Error_202   "202 (Homing Failed)"     // Homing Failed
#define Error_203   "203 (Probing Failed)"    // Probing Failed

/*variable addr*/
#define ExchangepageAddr                0x0084
#define SoundAddr                       0x00A0
#define START_PROCESS_ICON_VP           0x1000
#define PRINT_SPEED_RATE_VP             0x1006
#define PRINT_PROCESS_TITLE_VP          0x100E
#define PRINT_TIME_HOUR_VP              0x1010
#define PRINT_TIME_MIN_VP               0x1012
#define PRINT_PROCESS_VP                0x1016
#define PRINTER_FANOPEN_TITLE_VP        0x101E
#define PRINTER_LEDOPEN_TITLE_VP        0x101F
#define AUTO_BED_LEVEL_ZOFFSET_VP       0x1026
#define HEAD_SET_TEMP_VP                0x1034
#define HEAD_CURRENT_TEMP_VP            0x1036
#define BED_SET_TEMP_VP                 0x103A
#define BED_CURRENT_TEMP_VP             0x103C
#define AXIS_X_COORD_VP                 0x1048
#define AXIS_Y_COORD_VP                 0x104A
#define AXIS_Z_COORD_VP                 0x104C
#define HEAD_FILAMENT_LOAD_DATA_VP      0x1054
#define PRINTER_MACHINE_TEXT_VP         0x1060
#define PRINTER_VERSION_TEXT_VP         0x106A
#define PRINTER_PRINTSIZE_TEXT_VP       0x1074
#define PRINTER_WEBSITE_TEXT_VP         0x107E
#define AUTO_BED_PREHEAT_HEAD_DATA_VP   0x108A
#define AUTO_BED_LEVEL_TITLE_VP         0x108D
#define FILAMENT_LOAD_ICON_VP           0x108E
#define FAN_SPEED_CONTROL_DATA_VP       0x1100
#define PLA_HEAD_SET_DATA_VP            0x1102
#define PLA_BED_SET_DATA_VP             0x1104
#define PLA_FAN_SET_DATA_VP             0x1106
#define ABS_HEAD_SET_DATA_VP            0x1108
#define ABS_BED_SET_DATA_VP             0x110A
#define ABS_FAN_SET_DATA_VP             0x110C
#define ABNORMAL_TEXT_VP                0X1110
#define MOTOR_FREE_ICON_VP              0x1200
#define CONTINUE_PRINT_FILE_TEXT_VP     0x2000

#define FILE1_SELECT_ICON_VP            0x1221
#define FILE2_SELECT_ICON_VP            0x1222
#define FILE3_SELECT_ICON_VP            0x1223
#define FILE4_SELECT_ICON_VP            0x1224
#define FILE5_SELECT_ICON_VP            0x1225
#define FILE6_SELECT_ICON_VP            0x1226
#define FILE7_SELECT_ICON_VP            0x1227
#define FILE8_SELECT_ICON_VP            0x1228
#define FILE9_SELECT_ICON_VP            0x1229
#define FILE10_SELECT_ICON_VP           0x122A
#define FILE11_SELECT_ICON_VP           0x122B
#define FILE12_SELECT_ICON_VP           0x122C
#define FILE13_SELECT_ICON_VP           0x122D
#define FILE14_SELECT_ICON_VP           0x122E
#define FILE15_SELECT_ICON_VP           0x122F
#define FILE16_SELECT_ICON_VP           0x1230
#define FILE17_SELECT_ICON_VP           0x1231
#define FILE18_SELECT_ICON_VP           0x1232
#define FILE19_SELECT_ICON_VP           0x1233
#define FILE20_SELECT_ICON_VP           0x1234
#define FILE1_TEXT_VP                   0x200A
#define FILE2_TEXT_VP                   0x2014
#define FILE3_TEXT_VP                   0x201E
#define FILE4_TEXT_VP                   0x2028
#define FILE5_TEXT_VP                   0x2032
#define FILE6_TEXT_VP                   0x203C
#define FILE7_TEXT_VP                   0x2046
#define FILE8_TEXT_VP                   0x2050
#define FILE9_TEXT_VP                   0x205A
#define FILE10_TEXT_VP                  0x2064
#define FILE11_TEXT_VP                  0x206E
#define FILE12_TEXT_VP                  0x2078
#define FILE13_TEXT_VP                  0x2082
#define FILE14_TEXT_VP                  0x208C
#define FILE15_TEXT_VP                  0x2096
#define FILE16_TEXT_VP                  0x20A0
#define FILE17_TEXT_VP                  0x20AA
#define FILE18_TEXT_VP                  0x20B4
#define FILE19_TEXT_VP                  0x20BE
#define FILE20_TEXT_VP                  0x20C8
#define FilenameNature                  0x6003

/************struct**************/
typedef struct DataBuf
{
    unsigned char len;  
    unsigned char head[2];
    unsigned char command;
    unsigned long addr;
    unsigned long bytelen;
    unsigned short data[32];
    unsigned char reserv[4];
} DB;

typedef struct CardRecord
{
    int recordcount;
    int Filesum;
    unsigned long addr[FileNum];
    char Cardshowfilename[FileNum][FileNameLen];
    char Cardfilename[FileNum][FileNameLen];
}CRec;

extern CRec CardRecbuf;


enum class DWINTouchPage : unsigned int {
  BOOT = 0,

  MAIN_MENU = 28,

  FILE_SELECTION_P1 = 29,
  FILE_SELECTION_P2 = 30,
  FILE_SELECTION_P3 = 31,
  FILE_SELECTION_P4 = 32,
  FILE_SELECTION_P5 = 33,

  ERR_FILAMENTRUNOUT_HOTEND_COLD = 34,
  ERR_FILAMENTRUNOUT_FILAMENT_LOADED = 35,

  PRINT_FINISHED = 36,

  PRINT_PROGRESS_RUNNING = 37,
  PRINT_PROGRESS_PAUSED = 39,

  DIALOG_PAUSE_PRINTING = 38,
  DIALOG_STOP_PRINTING = 40,

  MENU_TUNING = 41,
  MENU_PREPARE = 42,
  
  MOVE_10MM = 43,
  MOVE_1MM = 44,
  MOVE_01MM = 45,

  FEED = 46,

  MENU_CONTROL = 47,
  MENU_TEMP = 48,
  MENU_PLA_TEMP = 49,
  MENU_ABS_TEMP = 50,

  MENU_ABOUT = 51,

  MENU_ZOFFSET_LEVELING = 52,
  LEVELING = 53,
  DIALOG_POWER_FAILURE = 54,

  KEYBOARD = 55,
  KEYBOARD_CONFIRM = 56,

  ERR_THERMAL_RUNAWAY = 57,
  ERR_HEATING_FAILED = 58,
  ERR_THERMISTOR = 59,

  AUTOHOME_IN_PROGRESS = 61,
  ERR_FATAL_UNSPECIFIED = 62,
};

class RTSSHOW {
  public:
    RTSSHOW();
    int RTS_RecData();
    void RTS_SDCardInit(void);
    void RTS_SDCardUpate(void);
    void RTS_SndData(void);
    void RTS_SndData(const String &, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(const char[], unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(char, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(unsigned char*, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(int, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(float, unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(unsigned int,unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(long,unsigned long, unsigned char = VarAddr_W);
    void RTS_SndData(unsigned long,unsigned long, unsigned char = VarAddr_W);
    void RTS_SDcard_Stop();
    void RTS_HandleData();
    void RTS_Init();
    void RTS_FilamentRunout();
    void RTS_FilamentLoaded();

    void change_page(DWINTouchPage newPage);
    void refresh_page();
    inline bool has_fatal_error() { return m_current_page == DWINTouchPage::ERR_FATAL_UNSPECIFIED; };

    DB recdat;
    DB snddat;


  private:
    unsigned char databuf[SizeofDatabuf];

    DWINTouchPage m_current_page;
  };

extern RTSSHOW rtscheck;

enum PROC_COM 
{
  MainEnterKey          = 0,
  AdjustEnterKey        = 1,
  PrintSpeedEnterKey    = 2,
  StopPrintKey          = 3,
  PausePrintKey         = 4,
  ResumePrintKey        = 5,
  ZoffsetEnterKey       = 6,
  TempControlKey        = 7,
  CoolDownKey           = 8,
  HeaterTempEnterKey    = 9,
  HotBedTempEnterKey    = 10,
  PrepareEnterKey       = 11,
  BedLevelKey           = 12,
  AutoHomeKey           = 13,
  XaxismoveKey          = 14,
  YaxismoveKey          = 15,
  ZaxismoveKey          = 16,
  HeaterLoadEnterKey    = 17,
  HeaterLoadStartKey    = 18,

  PowerContinuePrintKey = 20,
  FanSpeedEnterKey      = 21,
  PLAHeadSetEnterKey    = 22,
  PLABedSetEnterKey     = 23,
  PLAFanSetEnterKey     = 24,
  ABSHeadSetEnterKey    = 25,
  ABSBedSetEnterKey     = 26,
  ABSFanSetEnterKey     = 27,
  ChangePageKey         = 28,
  StartFileKey          = 29,
  SelectFileKey         = 30,
  ErrorKey              = 31,
};

const unsigned long Addrbuf[] = 
{
  0x1002,  0x1004,  0x1006,  0x1008,  0x100A,
  0x100C,  0x1026,  0x1030,  0x1032,  0x1034,
  0x103A,  0x103E,  0x1044,  0x1046,  0x1048,
  0x104A,  0x104C,  0x1054,  0x1056,  0x105C,
  0x105F,  0x1100,  0x1102,  0x1104,  0x1106,
  0x1108,  0x110A,  0x110C,  0x110E,  0x20D2,  
  0x20D3,  0x111A
};

typedef struct
{
  int16_t preheat_hotend_temp[2];
  int16_t preheat_bed_temp[2];  
  uint8_t preheat_fan_speed[2];
}HMI_ValueTypeDef;

extern HMI_ValueTypeDef HMI_ValueStruct;

void ErrorHanding();

extern void RTSUpdate();

extern char waitway;
extern int Update_Time_Value;
extern unsigned char AxisUnitMode;
extern bool home_flag;
extern bool heat_flag;
extern bool print_finish;
extern bool finish_home;
extern bool AutohomeZflag;
extern char commandbuf[30];

extern bool StartPrint_flag;

extern char errorway;
extern char errornum;

