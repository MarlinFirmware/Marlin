/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <stdint.h>

/*********************************/
#define FHONE   (0x5A)
#define FHTWO   (0xA5)
#define FHLENG  (0x06)

#define MAX_NUM_FILES       20
#define FILENAME_LEN        20
#define RTS_UPDATE_INTERVAL 2000
#define RTS_UPDATE_VALUE    (2 * RTS_UPDATE_INTERVAL)

#define DATA_BUF_SIZE       26

/*************Register and Variable addr*****************/
#define RegAddr_W 0x80
#define RegAddr_R 0x81
#define VarAddr_W 0x82
#define VarAddr_R 0x83
#define ExchangePageBase  0x5A010000UL  // the first page ID. other page = first page ID + relevant num;
#define StartSoundSet     0x060480A0UL  // 06,start-music; 04, 4 musics; 80, the volume value; 04, return value about music number.
#define Beep              0x02AF0100UL
#define Beep1             0xFFFF0101UL
#define FONT_EEPROM 90

/*variable addr*/
#define ExchangepageAddr  0x0084
#define SoundAddr         0x00A0

#define START1_PROCESS_ICON_VP          0x1000
#define PRINT_SPEED_RATE_VP             0x1006
#define PRINT_PROCESS_ICON_VP           0x100E
#define PRINT_TIME_HOUR_VP              0x1010
#define PRINT_TIME_MIN_VP               0x1012
#define PRINT_PROCESS_VP                0x1016
#define HEAD0_FAN_ICON_VP               0x101E
#define CHANGE_FILAMENT0_TEMP_VP        0x1020
#define AUTO_BED_LEVEL_ZOFFSET_VP       0x1026

#define HEAD0_SET_TEMP_VP               0x1034
#define HEAD0_CURRENT_TEMP_VP           0x1036
#define BED_SET_TEMP_VP                 0x103A
#define BED_CURRENT_TEMP_VP             0x103C
#define AUTO_HOME_DISPLAY_ICON_VP       0x1042
#define AXIS_X_COORD_VP                 0x1048
#define AXIS_Y_COORD_VP                 0x104A
#define AXIS_Z_COORD_VP                 0x104C
#define HEAD0_FILAMENT_LOAD_DATA_VP     0x1054
#define PRINTER_MACHINE_TEXT_VP         0x1060
#define PRINTER_VERSION_TEXT_VP         0x106A
#define PRINTER_PRINTSIZE_TEXT_VP       0x1074
#define PRINTER_WEBSITE_TEXT_VP         0x107E
#define MARLIN_VERSION_TEXT_VP          0x1088
#define AUTO_BED_LEVEL_ICON_VP          0x108D
#define CHANGE_FILAMENT_ICON_VP         0x108E
#define TWO_EXTRUDER_HOTEND_XOFFSET_VP  0x1092
#define TWO_EXTRUDER_HOTEND_YOFFSET_VP  0x1094
#define TWO_EXTRUDER_HOTEND_ZOFFSET_VP  0x1096
#define AUTO_BED_LEVEL_1POINT_VP        0x1100

#define PRINT_SURPLUS_TIME_HOUR_VP      0x1162
#define PRINT_SURPLUS_TIME_MIN_VP       0x1164
#define SELECT_MODE_ICON_VP             0x1166
#define CHANGE_SDCARD_ICON_VP           0x1168

#define MOTOR_FREE_ICON_VP              0x1200
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
#define FILE2_TEXT_VP                   0x201E
#define FILE3_TEXT_VP                   0x2032
#define FILE4_TEXT_VP                   0x2046
#define FILE5_TEXT_VP                   0x205A
#define FILE6_TEXT_VP                   0x206E
#define FILE7_TEXT_VP                   0x2082
#define FILE8_TEXT_VP                   0x2096
#define FILE9_TEXT_VP                   0x20AA
#define FILE10_TEXT_VP                  0x20BE
#define FILE11_TEXT_VP                  0x20D2
#define FILE12_TEXT_VP                  0x20E6
#define FILE13_TEXT_VP                  0x20FA
#define FILE14_TEXT_VP                  0x210E
#define FILE15_TEXT_VP                  0x2122
#define FILE16_TEXT_VP                  0x2136
#define FILE17_TEXT_VP                  0x214A
#define FILE18_TEXT_VP                  0x215E
#define FILE19_TEXT_VP                  0x2172
#define FILE20_TEXT_VP                  0x2186

#define SELECT_FILE_TEXT_VP             0x219A
#define TWO_COLOR_MODE_ICON_VP          0x21B8
#define COPY_MODE_ICON_VP               0x21B9
#define MIRROR_MODE_ICON_VP             0x21BA
#define SINGLE_MODE_ICON_VP             0x21BB
#define EXCHANGE_NOZZLE_ICON_VP         0x21BC
#define PRINT_MODE_ICON_VP              0x21BD
#define PRINT_FILE_TEXT_VP              0x21C0

#define Nozzle_P_VP                     0x2200
#define Nozzle_I_VP                     0x2204
#define Nozzle_D_VP                     0x2208
#define Hot_Bed_P_VP                    0x220C
#define Hot_Bed_I_VP                    0x2240
#define Hot_Bed_D_VP                    0x2244

#define Vmax_X_VP                       0x2210
#define Vmax_Y_VP                       0x2212
#define Vmax_Z_VP                       0x2214
#define Vmax_E_VP                       0x2216

#define Accel_VP                        0x2220
#define A_Retract_VP                    0x2222
#define A_Travel_VP                     0x2224
#define Amax_X_VP                       0x2226
#define Amax_Y_VP                       0x2228
#define Amax_Z_VP                       0x222A
#define Amax_E_VP                       0x222C

#define Jerk_X_VP                       0x2230
#define Jerk_Y_VP                       0x2232
#define Jerk_Z_VP                       0x2234
#define Jerk_E_VP                       0x2236

#define Steps_X_VP                      0x1130
#define Steps_Y_VP                      0x1132
#define Steps_Z_VP                      0x1134
#define Steps_E_VP                      0x1136

#define Advance_K_VP                    0x1138
#define Time_VP                         0x2450
#define Time1_VP                        0x2455
#define FAN_SPEED_VP                    0x2460
#define Wait_VP                         0x2480
#define Zoffset_UNIT_VP                 0x2500
#define Current_X_VP                    0x2468
#define Current_Y_VP                    0x246A
#define Current_Z_VP                    0x246C
#define Current_E_VP                    0x246F
#define Threshold_X_VP                  0x2471
#define Threshold_Y_VP                  0x2473
#define Threshold_Z_VP                  0x2475
#define Threshold_E_VP                  0x2477
#define Sensorless_X_VP                 0x2479
#define Sensorless_Y_VP                 0x247B

#define FilenameNature                  0x6003

enum SovolPage : uint8_t {
  ID_Home_L         =   1,      ID_Home_D         =  55 + ID_Home_L,
  ID_Cold_L         =   7,      ID_Cold_D         =  55 + ID_Cold_L,
  ID_Unload_L       =  16,      ID_Unload_D       =  55 + ID_Unload_L,
  ID_FilamentOut_L  =  39,      ID_FilamentOut_D  =  55 + ID_FilamentOut_L,
  ID_MediaFail_L    =  46,      ID_MediaFail_D    =  55 + ID_MediaFail_L,
  ID_KillRunaway_L  =  52,      ID_KillRunaway_D  =  55 + ID_KillRunaway_L,
  ID_KillHeat_L     =  53,      ID_KillHeat_D     =  55 + ID_KillHeat_L,
  ID_KillBadTemp_L  =  54,      ID_KillBadTemp_D  =  55 + ID_KillBadTemp_L,
  ID_KillHome_L     =  55,      ID_KillHome_D     =  55 + ID_KillHome_L
};

/************struct**************/

typedef struct DataBuf {
  uint8_t len;
  uint8_t head[2];
  uint8_t command;
  uint32_t addr;
  uint32_t bytelen;
  uint16_t data[32];
  uint8_t reserv[4];
} DB;

typedef struct CardRecord {
  int16_t recordcount;
  int16_t Filesum;
  uint32_t addr[MAX_NUM_FILES];
  char display_filename[MAX_NUM_FILES][FILENAME_LEN];
  char filename[MAX_NUM_FILES][FILENAME_LEN];
} CRec;

class RTS {
  public:
    RTS();
    static int16_t receiveData();
    static void sdCardInit();
    static bool sdDetected();
    static void sdCardUpdate();
    static void sendData();
    static void sendData(const String&,  const uint32_t, const uint8_t=VarAddr_W);
    static void sendData(const char[],   const uint32_t, const uint8_t=VarAddr_W);
    static void sendData(const char,     const uint32_t, const uint8_t=VarAddr_W);
    static void sendData(const int16_t,  const uint32_t, const uint8_t=VarAddr_W);
    static void sendData(const uint32_t, const uint32_t, const uint8_t=VarAddr_W);

    static void sendData(const uint8_t str[], const uint32_t addr, const uint8_t cmd) { sendData((char *)str, addr, cmd); }
    static void sendData(const uint16_t n,    const uint32_t addr, const uint8_t cmd) { sendData(int16_t(n), addr, cmd); }
    static void sendData(const_float_t n,     const uint32_t addr, const uint8_t cmd) { sendData(int32_t(n), addr, cmd); }
    static void sendData(const int32_t n,     const uint32_t addr, const uint8_t cmd) { sendData(uint32_t(n), addr, cmd); }

    static void sdCardStop();
    static void handleData();
    static void init();

    static uint8_t print_state;
    static bool start_print_flag;

    static bool dark_mode;
    static void gotoPage(SovolPage page) { sendData(ExchangePageBase + page, ExchangepageAddr); }
    static void gotoPage(SovolPage p1, SovolPage p2) { gotoPage(dark_mode ? p2 : p1); }
    static void gotoPageBeep(SovolPage p1, SovolPage p2) { gotoPage(p1, p2); sendData(Beep1, SoundAddr); }

    static void sendPrinterInfo();
    static void updateTempE0();
    static void updateTempBed();
    static void updateFan0();

    static void onIdle();

    static void refreshTime() { sendData(1, dark_mode ? Time_VP : Time1_VP); gotoPage(11, 66); }

    static DB recdat;
    static DB snddat;
  private:
    static uint8_t databuf[DATA_BUF_SIZE];
};

extern RTS rts;

enum PROC_COM {
  MainPageKey = 0,
  AdjustmentKey,
  PrintSpeedKey,
  StopPrintKey,
  PausePrintKey,
  ResumePrintKey,
  ZOffsetKey,
  TempScreenKey,
  CoolScreenKey,
  Heater0TempEnterKey, Heater1TempEnterKey,
  HotBedTempEnterKey,
  SettingScreenKey,
  SettingBackKey,
  BedLevelFunKey,
  AxisPageSelectKey,
  XaxismoveKey, YaxismoveKey, ZaxismoveKey,
  SelectExtruderKey,
  Heater0LoadEnterKey,
  FilamentLoadKey,
  Heater1LoadEnterKey,
  SelectLanguageKey,
  FilamentCheckKey,
  PowerContinuePrintKey,
  PrintSelectModeKey,
  XhotendOffsetKey, YhotendOffsetKey, ZhotendOffsetKey,
  StoreMemoryKey,
  PrintFileKey,
  SelectFileKey,
  AdvancedKey,
  Nozzle_P, Nozzle_I, Nozzle_D,
  Hot_Bed_P, Hot_Bed_I, Hot_Bed_D,
  Vmax_X, Vmax_Y, Vmax_Z, Vmax_E,
  Accel, A_Retract, A_Travel,
  Amax_X, Amax_Y, Amax_Z, Amax_E,
  Jerk_X, Jerk_Y, Jerk_Z, Jerk_E,
  Steps_X, Steps_Y, Steps_Z, Steps_E,
  Advance_K,
  AdvancedBackKey,
  FilamentChange,
  FanSpeedKey,
  ZoffsetUnitKey,
  TMCDriver,
  Current_X, Current_Y, Current_Z, Current_E,
  Threshold_X, Threshold_Y, Threshold_Z, Threshold_E,
  Sensorless_X, Sensorless_Y,
  ChangePageKey
};

const uint32_t Addrbuf[] = {
  0x1002, 0x1004, 0x1006, 0x1008, 0x100A, 0x100C, 0x1026, 0x1030, 0x1032, 0x1034,
  0x1038, 0x103A, 0x103E, 0x1040, 0x1044, 0x1046, 0x1048, 0x104A, 0x104C, 0x104E,
  0x1054, 0x1056, 0x1058, 0x105C, 0x105E, 0x105F, 0x1090, 0x1092, 0x1094, 0x1096,
  0x1098, 0x2198, 0x2199, 0x21E0, 0x2200, 0x2204, 0x2208, 0x220C, 0x2240, 0x2244,
  0x2210, 0x2212, 0x2214, 0x2216, 0x2220, 0x2222, 0x2224, 0x2226, 0x2228, 0x222A,
  0x222C, 0x2230, 0x2232, 0x2234, 0x2236, 0x1130, 0x1132, 0x1134, 0x1136, 0x1138,
  0x2250, 0x2300, 0x2460, 0x2464, 0x2466, 0x2468, 0x246A, 0x246C, 0x246F, 0x2471,
  0x2473, 0x2475, 0x2477, 0x2479, 0x247B, 0x110E, 0
};

extern void RTS_Update();
extern void RTS_Init();

extern int16_t update_time_value;
extern bool poweroff_continue;
extern bool sdcard_pause_check;
extern bool sd_printing_autopause;
extern bool pause_flag;

void RTS_AutoBedLevelPage();
void RTS_MoveAxisHoming();
void RTS_PauseMoveAxisPage();
