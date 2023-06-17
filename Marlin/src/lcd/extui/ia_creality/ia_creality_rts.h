/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/* ****************************************
 * lcd/extui/ia_creality/ia_creality_rts.h
 * ****************************************
 * Extensible_UI implementation for Creality DWIN
 * 10SPro, Max, CRX, and others
 * Based original Creality release
 * Written by Insanity Automation, sponsored by Tiny Machines 3D
 *
 * ***************************************/

#include "../../../inc/MarlinConfig.h"

#include <WString.h>

/*********************************/
#define FHONE   (0x5A)
#define FHTWO   (0xA5)
#define FHLENG  (0x06)

#define TEXTBYTELEN   18
#define MaxFileNumber 20 // 16

#define CEIconGrap    12
#define FileNum       MaxFileNumber
#define FileNameLen   TEXTBYTELEN

#define DATA_BUF_SIZE 46

//#define FONT_EEPROM      90
//#define AutoLeve_EEPROM 100
//#define FanOn       255
#define FanOff        0

#define ExchangePageBase  0x5A010000UL  // the first page ID. other page = first page ID + relevant num;
#define StartSoundSet     0x060480A0UL  // 06,start-music; 04, 4 musics; 80, the volume value; 04, return value about music number.

// Register addresses
#define RegAddr_W     0x80
#define RegAddr_R     0x81
#define VarAddr_W     0x82
#define VarAddr_R     0x83

// Variable addresses
#define ExchangepageAddr    0x0084
#define SoundAddr           0x00A0
#define StartIcon           0x1000
#define FeedrateDisplay     0x1006 // Speed
#define Stopprint           0x1008
#define Pauseprint          0x100A
#define Resumeprint         0x100C
#define PrintscheduleIcon   0x100E
#define Timehour            0x1010
#define Timemin             0x1012
#define IconPrintstatus     0x1014
#define Percentage          0x1016
#define FanKeyIcon          0x101E
#define Flowrate            0x1300

#define RunoutToggle        0x1018
#define PowerLossToggle     0x101A
#define LedToggle           0x101C

#define StepMM_X            0x1242
#define StepMM_Y            0x1246
#define StepMM_Z            0x124A
#define StepMM_E            0x124E

#define ProbeOffset_X       0x1236
#define ProbeOffset_Y       0x123A
#define ProbeOffset_Z       0x1026

#define T2Offset_X          0x1090
#define T2Offset_Y          0x1092
#define T2Offset_Z          0x1094
#define T2StepMM_E          0x1096

#define ActiveToolVP        0x1014

#define HotendPID_AutoTmp   0x1252
#define BedPID_AutoTmp      0x1254

#define HotendPID_P         0x1256
#define HotendPID_I         0x125A
#define HotendPID_D         0x125E

#define BedPID_P            0x1262
#define BedPID_I            0x1266
#define BedPID_D            0x126A

#define Jerk_X              0x1270
#define Jerk_Y              0x1272
#define Jerk_Z              0x1274
#define Jerk_E              0x1276

#define Feed_X              0x1278
#define Feed_Y              0x127A
#define Feed_Z              0x127C
#define Feed_E              0x127E

#define Accel_X             0x1280
#define Accel_Y             0x1282
#define Accel_Z             0x1284
#define Accel_E             0x1286

#define HeatPercentIcon     0x1024

#define NzBdSet             0x1032 // cooldown Hotend and Bed
#define NozzlePreheat       0x1034 // setpoint
#define NozzleTemp          0x1036 // Actual
#define BedPreheat          0x103A // Setpoint
#define Bedtemp             0x103C // Actual

#define e2Temp              0x1050
#define e2Preheat           0x104E

#define AutoZeroIcon        0x1042
#define AutoLevelIcon       0x1045
#define AutoZero            0x1046
#define DisplayXaxis        0x1048
#define DisplayYaxis        0x104A
#define DisplayZaxis        0x104C

#define FilamentUnit1       0x1054
#define Exchfilament        0x1056
#define FilamentUnit2       0x1058

#define MacVersion          0x1060
#define SoftVersion         0x106A
#define PrinterSize         0x1074
#define CorpWebsite         0x107E
#define VolumeIcon          0x108A
#define SoundIcon           0x108C
#define AutolevelIcon       0x108D
#define ExchFlmntIcon       0x108E
#define AutolevelVal        0x1100

#define FilenameIcon        0x1200
#define FilenameIcon1       0x1220
#define Printfilename       0x2000
#define FilesCurentPage     0x1310
#define FilesMaxPage        0x1312
#define SDFILE_ADDR         0x200A
#define FilenamePlay        0x20D2
#define FilenameChs         0x20D3
#define Choosefilename      0x20D4
#define FilenameCount       0x20DE
#define FilenameNature      0x6003

#define VolumeDisplay                 0x1140
#define DisplayBrightness             0x1142
#define DisplayStandbyBrightness      0x1144
#define DisplayStandbyEnableIndicator 0x1146
#define DisplayStandbySeconds         0x1148

#define StatusMessageString 0x2064

// TODO: Use LCD_SERIAL 1 or 3 (?) by configuration, not overriding here
#ifdef TARGET_STM32F4
  #define DWIN_SERIAL Serial1
#else
  #define DWIN_SERIAL LCD_SERIAL
#endif

/************struct**************/

typedef enum : uint8_t {
  DGUS_IDLE,           //< waiting for DGUS_HEADER1.
  DGUS_HEADER1_SEEN,   //< DGUS_HEADER1 received
  DGUS_HEADER2_SEEN,   //< DGUS_HEADER2 received
  DGUS_WAIT_TELEGRAM,  //< LEN received, Waiting for to receive all bytes.
} rx_datagram_state_t;

typedef struct DataBuf {
  uint8_t len;
  uint8_t head[2];
  uint8_t command;
  uint32_t addr;
  uint32_t bytelen;
  uint16_t data[32];
  uint8_t reserv[4];
} DB;

struct creality_dwin_settings_t {
  size_t settings_size;
  uint8_t settings_version;

  bool display_standby;
  bool display_sound;
  int8_t screen_rotation;
  int16_t display_volume;
  uint8_t standby_brightness;
  uint8_t screen_brightness;
  int16_t standby_time_seconds;
};

class RTS {
  public:
    RTS();
    static void onStartup();
    static void onIdle();
    static int16_t receiveData();
    static void sendData();
    static void sendData(const String&, const uint32_t, const uint8_t=VarAddr_W);
    static void sendData(const char[],  const uint32_t, const uint8_t=VarAddr_W);
    static void sendData(char,          const uint32_t, const uint8_t=VarAddr_W);
    static void sendData(int,           const uint32_t, const uint8_t=VarAddr_W);
    static void sendData(unsigned long, const uint32_t, const uint8_t=VarAddr_W);
    static void sendData(const_float_t, const uint32_t, const uint8_t=VarAddr_W);

    static void sendData(uint8_t * const str, const uint32_t addr, const uint8_t cmd=VarAddr_W) { sendData((char *)str, addr, cmd); }
    static void sendData(const unsigned int n, uint32_t addr, const uint8_t cmd=VarAddr_W) { sendData(int(n), addr, cmd); }
    static void sendData(const long n, const uint32_t addr, const uint8_t cmd=VarAddr_W) { sendData((unsigned long)n, addr, cmd); }

    static void handleData();

    static void writeVariable(const uint16_t adr, const void * const values, uint8_t valueslen, const bool isstr=false, const char fillChar=' ');
    static void setTouchScreenConfiguration();

    static DB recdat, snddat;
    static uint8_t databuf[DATA_BUF_SIZE];

    static rx_datagram_state_t rx_datagram_state;
    static uint8_t rx_datagram_len;
};

extern RTS rts;

#define Addvalue           3
#define PrintChoice_Value (0 + Addvalue)
#define Zoffset_Value     (3 + Addvalue)
#define Setting_Value     (8 + Addvalue)
#define XYZEaxis_Value    (12 + Addvalue)
#define Filament_Value    (15 + Addvalue)
#define Language_Value    (18 + Addvalue)
#define Filename_Value    (22 + Addvalue)

enum PROC_COM {
  Printfile = 0,
  Adjust,
  Feedrate,
  PrintChoice = PrintChoice_Value,
  Zoffset = Zoffset_Value,
  TempControl,
  ManualSetTemp,
  Setting = Setting_Value,
  ReturnBack,
  Bedlevel,
  Autohome,
  XYZEaxis = XYZEaxis_Value,
  Filament = Filament_Value,
  LanguageChoice = Language_Value,
  No_Filament,
  PwrOffNoF,
  Volume,
  Filename = Filename_Value
};

const uint16_t Addrbuf[] = {
  0x1002, 0x1004, 0x1006, 0x1008, 0x100A, 0x100C, 0x1026, 0x1030, 0x1032, 0x1034, 0x103A,
  0x103E, 0x1040, 0x1044, 0x1046, 0x1048, 0x104A, 0x104C, 0x1054, 0x1056, 0x1058,
  0x105C, 0x105E, 0x105F, 0x1088, 0
};

void RTS_Update();

#ifndef MAIN_MENU_ITEM_1_GCODE
  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    #define MEASURING_GCODE "M190S55\nG28O\nG34\nG29\nM400\nM104S215\nG28\nM109S215\nM420S1\nG1X100Y100F5000\nG1Z0\nM500\nM117 Set Z Offset"
  #elif ENABLED(AUTO_BED_LEVELING_UBL)
    #define MEASURING_GCODE "M190S55\nG28O\nG34\nG29P1\nG29P3\nG29S1\nG29S0\nG29F0.0\nG29A\nM104S215\nG28\nM109S215\nG1X150Y150F5000\nG1Z0\nM500\nM400\nM117 Set Z Offset"
  #else
    #define MEASURING_GCODE "G28"
  #endif
#else
  #define MEASURING_GCODE MAIN_MENU_ITEM_1_GCODE
#endif

// Data shared by RTS and ExtUI
extern uint16_t fileIndex;
extern uint8_t recordcount;
extern uint8_t startprogress;
extern char waitway;
extern char printerStatusKey[2];  // [0] = 0:ready  [1] = 0:keep temperature, 1:heating, 2:cooling, 3:printing
extern bool show_status;
extern bool tpShowStatus;         // true: opening time/percentage, false: closing time/percentage
extern uint8_t lastPauseMsgState;
extern creality_dwin_settings_t dwin_settings;
extern bool no_reentry;
#if HAS_PID_HEATING
  extern uint16_t pid_hotendAutoTemp;
  extern uint16_t pid_bedAutoTemp;
#endif
