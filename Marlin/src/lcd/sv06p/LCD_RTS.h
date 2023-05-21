#include <string.h>
//#include <arduino.h>

#include "../../inc/MarlinConfig.h"
/*********************************/
#define FHONE   (0x5A)
#define FHTWO   (0xA5)
#define FHLENG  (0x06)
#define TEXTBYTELEN     20
#define MaxFileNumber   20

#define FileNum             MaxFileNumber
#define FileNameLen         TEXTBYTELEN
#define RTS_UPDATE_INTERVAL 2000
#define RTS_UPDATE_VALUE    (2 * RTS_UPDATE_INTERVAL)

#define SizeofDatabuf       26

/*************Register and Variable addr*****************/
#define	RegAddr_W	0x80
#define	RegAddr_R	0x81
#define	VarAddr_W	0x82
#define	VarAddr_R	0x83
#define	ExchangePageBase    ((unsigned long)0x5A010000)     // the first page ID. other page = first page ID + relevant num;
#define	StartSoundSet       ((unsigned long)0x060480A0)     // 06,start-music; 04, 4 musics; 80, the volume value; 04, return value about music number.
#define	Beep       					((unsigned long)0x02AF0100)
#define	Beep1       				((unsigned long)0xFFFF0101)
#define	FONT_EEPROM      90

/*variable addr*/
#define	ExchangepageAddr	       0x0084
#define	SoundAddr			           0x00A0

#define START1_PROCESS_ICON_VP             0x1000
#define PRINT_SPEED_RATE_VP                0x1006
#define PRINT_PROCESS_ICON_VP              0x100E
#define PRINT_TIME_HOUR_VP                 0x1010
#define PRINT_TIME_MIN_VP                  0x1012
#define PRINT_PROCESS_VP                   0x1016
#define HEAD0_FAN_ICON_VP                  0x101E
#define CHANGE_FILAMENT0_TEMP_VP           0x1020
#define AUTO_BED_LEVEL_ZOFFSET_VP          0x1026

#define HEAD0_SET_TEMP_VP                  0x1034
#define HEAD0_CURRENT_TEMP_VP              0x1036
#define BED_SET_TEMP_VP                    0x103A
#define BED_CURRENT_TEMP_VP                0x103C
#define AUTO_HOME_DISPLAY_ICON_VP          0x1042
#define AXIS_X_COORD_VP                    0x1048
#define AXIS_Y_COORD_VP                    0x104A
#define AXIS_Z_COORD_VP                    0x104C
#define HEAD0_FILAMENT_LOAD_DATA_VP        0x1054
#define PRINTER_MACHINE_TEXT_VP            0x1060
#define PRINTER_VERSION_TEXT_VP            0x106A
#define PRINTER_PRINTSIZE_TEXT_VP          0x1074
#define PRINTER_WEBSITE_TEXT_VP            0x107E
#define MARLIN_VERSION_TEXT_VP             0x1088
#define AUTO_BED_LEVEL_ICON_VP             0x108D
#define CHANGE_FILAMENT_ICON_VP            0x108E
#define TWO_EXTRUDER_HOTEND_XOFFSET_VP     0x1092
#define TWO_EXTRUDER_HOTEND_YOFFSET_VP     0x1094
#define TWO_EXTRUDER_HOTEND_ZOFFSET_VP     0x1096
#define AUTO_BED_LEVEL_1POINT_VP           0x1100

#define PRINT_SURPLUS_TIME_HOUR_VP         0x1162
#define PRINT_SURPLUS_TIME_MIN_VP          0x1164
#define SELECT_MODE_ICON_VP                0x1166
#define CHANGE_SDCARD_ICON_VP              0x1168

#define MOTOR_FREE_ICON_VP                 0x1200
#define FILE1_SELECT_ICON_VP               0x1221
#define FILE2_SELECT_ICON_VP               0x1222
#define FILE3_SELECT_ICON_VP               0x1223
#define FILE4_SELECT_ICON_VP               0x1224
#define FILE5_SELECT_ICON_VP               0x1225
#define FILE6_SELECT_ICON_VP               0x1226
#define FILE7_SELECT_ICON_VP               0x1227
#define FILE8_SELECT_ICON_VP               0x1228
#define FILE9_SELECT_ICON_VP               0x1229
#define FILE10_SELECT_ICON_VP              0x122A
#define FILE11_SELECT_ICON_VP              0x122B
#define FILE12_SELECT_ICON_VP              0x122C
#define FILE13_SELECT_ICON_VP              0x122D
#define FILE14_SELECT_ICON_VP              0x122E
#define FILE15_SELECT_ICON_VP              0x122F
#define FILE16_SELECT_ICON_VP              0x1230
#define FILE17_SELECT_ICON_VP              0x1231
#define FILE18_SELECT_ICON_VP              0x1232
#define FILE19_SELECT_ICON_VP              0x1233
#define FILE20_SELECT_ICON_VP              0x1234

#define FILE1_TEXT_VP                      0x200A
#define FILE2_TEXT_VP                      0x201E
#define FILE3_TEXT_VP                      0x2032
#define FILE4_TEXT_VP                      0x2046
#define FILE5_TEXT_VP                      0x205A
#define FILE6_TEXT_VP                      0x206E
#define FILE7_TEXT_VP                      0x2082
#define FILE8_TEXT_VP                      0x2096
#define FILE9_TEXT_VP                      0x20AA
#define FILE10_TEXT_VP                     0x20BE
#define FILE11_TEXT_VP                     0x20D2
#define FILE12_TEXT_VP                     0x20E6
#define FILE13_TEXT_VP                     0x20FA
#define FILE14_TEXT_VP                     0x210E
#define FILE15_TEXT_VP                     0x2122
#define FILE16_TEXT_VP                     0x2136
#define FILE17_TEXT_VP                     0x214A
#define FILE18_TEXT_VP                     0x215E
#define FILE19_TEXT_VP                     0x2172
#define FILE20_TEXT_VP                     0x2186

#define SELECT_FILE_TEXT_VP                0x219A
#define TWO_COLOR_MODE_ICON_VP             0x21B8
#define COPY_MODE_ICON_VP                  0x21B9
#define MIRROR_MODE_ICON_VP                0x21BA
#define SINGLE_MODE_ICON_VP                0x21BB
#define EXCHANGE_NOZZLE_ICON_VP            0x21BC
#define PRINT_MODE_ICON_VP                 0x21BD
#define PRINT_FILE_TEXT_VP                 0x21C0

#define Nozzle_P_VP                        0X2200
#define Nozzle_I_VP                        0X2204
#define Nozzle_D_VP                        0X2208
#define Hot_Bed_P_VP                       0X220C
#define Hot_Bed_I_VP                       0X2240
#define Hot_Bed_D_VP                       0X2244

#define Vmax_X_VP                          0X2210
#define Vmax_Y_VP                          0X2212
#define Vmax_Z_VP                          0X2214
#define Vmax_E_VP                          0X2216

#define Accel_VP                           0X2220
#define A_Retract_VP                       0X2222
#define A_Travel_VP                        0X2224
#define Amax_X_VP                          0X2226
#define Amax_Y_VP                          0X2228
#define Amax_Z_VP                          0X222A
#define Amax_E_VP                          0X222C

#define Jerk_X_VP                          0X2230
#define Jerk_Y_VP                          0X2232
#define Jerk_Z_VP                          0X2234
#define Jerk_E_VP                          0X2236

#define Steps_X_VP                         0X1130
#define Steps_Y_VP                         0X1132
#define Steps_Z_VP                         0X1134
#define Steps_E_VP                         0X1136

#define Advance_K_VP                       0X1138
#define Time_VP                            0X2450
#define Time1_VP                           0X2455
#define Fan_speed_VP                       0X2460
#define Wait_VP                            0X2480
#define Zoffset_UNIT_VP                    0x2500
#define Current_X_VP                       0x2468
#define Current_Y_VP                       0x246A
#define Current_Z_VP                       0x246C
#define Current_E_VP                       0x246F
#define Threshold_X_VP                     0x2471
#define Threshold_Y_VP                     0x2473
#define Threshold_Z_VP                     0x2475
#define Threshold_E_VP                     0x2477
#define Sensorless_X_VP                    0x2479
#define Sensorless_Y_VP                    0x247B

#define FilenameNature                     0x6003


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

class RTSSHOW
{
  public:
    RTSSHOW();
    int RTS_RecData();
    void RTS_SDCardInit(void);
    bool RTS_SD_Detected(void);
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

    DB recdat;
    DB snddat;
  private:
    unsigned char databuf[SizeofDatabuf];
};

extern RTSSHOW rtscheck;

enum PROC_COM
{
  MainPageKey = 0,
  AdjustmentKey,
  PrintSpeedKey,
  StopPrintKey,
  PausePrintKey,
  ResumePrintKey,
  ZOffsetKey,
  TempScreenKey,
  CoolScreenKey,
  Heater0TempEnterKey,
  Heater1TempEnterKey,
  HotBedTempEnterKey,
  SettingScreenKey,
  SettingBackKey,
  BedLevelFunKey,
  AxisPageSelectKey,
  XaxismoveKey,
  YaxismoveKey,
  ZaxismoveKey,
  SelectExtruderKey,
  Heater0LoadEnterKey,
  FilamentLoadKey,
  Heater1LoadEnterKey,
  SelectLanguageKey,
  FilamentCheckKey,
  PowerContinuePrintKey,
  PrintSelectModeKey,
  XhotendOffsetKey,
  YhotendOffsetKey,
  ZhotendOffsetKey,
  StoreMemoryKey,
  PrintFileKey,
  SelectFileKey,
  AdvancedKey,
  Nozzle_P,
  Nozzle_I,
  Nozzle_D,
  Hot_Bed_P,
  Hot_Bed_I,
  Hot_Bed_D,
  Vmax_X,
  Vmax_Y,
  Vmax_Z,
  Vmax_E,
  Accel,
  A_Retract,
  A_Travel,
  Amax_X,
  Amax_Y,
  Amax_Z,
  Amax_E,
  Jerk_X,
  Jerk_Y,
  Jerk_Z,
  Jerk_E,
  Steps_X,
  Steps_Y,
  Steps_Z,
  Steps_E,
  Advance_K,
  AdvancedBackKey,
  FilamentChange,
  Fan_speed,
  ZoffsetUnitKey,
  TMCDriver,
  Current_X,
  Current_Y,
  Current_Z,
  Current_E,
  Threshold_X,
  Threshold_Y,
  Threshold_Z,
  Threshold_E,
  Sensorless_X,
  Sensorless_Y,
  ChangePageKey
};

const unsigned long Addrbuf[] =
{
  0x1002,
  0x1004,
  0x1006,
  0x1008,
  0x100A,
  0x100C,
  0x1026,
  0x1030,
  0x1032,
  0x1034,
  0x1038,
  0x103A,
  0x103E,
  0x1040,
  0x1044,
  0x1046,
  0x1048,
  0x104A,
  0x104C,
  0x104E,
  0x1054,
  0x1056,
  0x1058,
  0x105C,
  0x105E,
  0x105F,
  0x1090,
  0x1092,
  0x1094,
  0x1096,
  0x1098,
  0x2198,
  0x2199,
  0X21E0,
  0X2200,
  0X2204,
  0X2208,
  0X220C,
  0X2240,
  0X2244,
  0X2210,
  0X2212,
  0X2214,
  0X2216,
  0X2220,
  0X2222,
  0X2224,
  0X2226,
  0X2228,
  0X222A,
  0X222C,
  0X2230,
  0X2232,
  0X2234,
  0X2236,
  0X1130,
  0X1132,
  0X1134,
  0X1136,
  0X1138,
  0x2250,
  0x2300,
  0x2460,
  0x2464,
  0x2466,
  0x2468,
  0x246A,
  0x246C,
  0x246F,
  0x2471,
  0x2473,
  0x2475,
  0x2477,
  0x2479,
  0x247B,
  0x110E,
  0
};

extern void RTSUpdate();
extern void RTSInit();

extern int Update_Time_Value;
extern bool PoweroffContinue;
extern bool sdcard_pause_check;
extern bool sd_printing_autopause;
extern int StartPrintFlag;
extern bool Mode_flag;
extern bool pause_flag;
extern int PrintFlag;


void RTS_AutoBedLevelPage();
void RTS_MoveAxisHoming();
void RTS_PauseMoveAxisPage();