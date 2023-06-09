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

/**
 * lcd_rts.cpp
 */

// This class was initially released by Creality for the Ender 3 S1 Pro
// This class was modified by Thomas Toka for MALRIN-E3S1PRO-FORK-BYTT
// This class is backward compatible to the stock screen firmware with its stock features.

#include "../../../inc/MarlinConfig.h"

#if ENABLED(E3S1PRO_RTS)

#include "lcd_rts.h"

// GCODE_PREVIEW requires some fixing..
//#define GCODE_PREVIEW_ENABLED

#include <wstring.h>
#include <stdio.h>
#include <string.h>
#include "../../../MarlinCore.h"
#include "../../../module/settings.h"
#include "../../utf8.h"
#include "../../marlinui.h"
#include "../../../sd/cardreader.h"
#include "../../../feature/babystep.h"
#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../gcode/queue.h"
#include "../../../gcode/gcode.h"
#include "../../../module/probe.h"

#if ENABLED(GCODE_PREVIEW_ENABLED)
  #include "preview.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)
  #include "../../../feature/bedlevel/abl/bbl.h"
#endif
#include "../../../feature/bedlevel/bedlevel.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../HAL/shared/eeprom_api.h"
  #include "../../../module/settings.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../../feature/host_actions.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
  #include "../../../gcode/queue.h"
#endif

#include "../../../libs/duration_t.h"

#if ENABLED(BLTOUCH)
  #include "../../../module/endstops.h"
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #include "../../../feature/runout.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#elif ENABLED(CREALITY_POWER_LOSS)
  #include "../../feature/PRE01_Power_loss/PRE01_Power_loss.h"
#endif

#if HAS_CUTTER
  #include "../../../feature/spindle_laser.h"
#endif

#ifdef LCD_SERIAL_PORT
  #define LCDSERIAL LCD_SERIAL
#elif SERIAL_PORT_2
  #define LCDSERIAL MYSERIAL2
#endif

RTSSHOW rtscheck;
bool hasSelected = false;
short previousSelectionIndex;
extern CardReader card;
char errorway        = 0;
char errornum        = 0;
char home_errornum   = 0;
char error_sd_num    = 0;
bool StartPrint_flag = false;

#if ENABLED(BABYSTEPPING)
  float zprobe_zoffset;
  float xprobe_xoffset;
  float yprobe_yoffset;
  float last_zoffset = 0.0;
  float last_xoffset = 0.0;
  float last_yoffset = 0.0;
  float rec_zoffset;
#endif

#if ENABLED(INPUT_SHAPING_X) && ENABLED(INPUT_SHAPING_Y)
  float x_frequency = 0.0;
  float y_frequency = 0.0;
  float x_zeta      = 0.0;
  float y_zeta      = 0.0;
#endif

float x_min_pos_eeprom;
float y_min_pos_eeprom;

int power_off_type_yes = 0;
int old_leveling       = 0;
int bltouch_tramming   = 0;

int touchscreen_requested_mesh = 0;

const float manual_feedrate_mm_m[]            = {50 * 60, 50 * 60, 4 * 60, 60};
constexpr float default_max_feedrate[]        = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[]    = DEFAULT_MAX_ACCELERATION;
constexpr float default_max_jerk[]            = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
constexpr float default_axis_steps_per_unit[] = DEFAULT_AXIS_STEPS_PER_UNIT;

float default_nozzle_ptemp = DEFAULT_Kp;
float default_nozzle_itemp = DEFAULT_Ki;
float default_nozzle_dtemp = DEFAULT_Kd;

float default_hotbed_ptemp = DEFAULT_bedKp;
float default_hotbed_itemp = DEFAULT_bedKi;
float default_hotbed_dtemp = DEFAULT_bedKd;

int AutoHomeFirstPoint = 0;

uint8_t startprogress = 0;

CRec CardRecbuf;
int16_t temphot               = 0;
int16_t tempbed               = 0;
float temp_bed_display        = 0;
uint8_t afterprobe_fan0_speed = 0;

bool sdcard_pause_check = true;
bool pause_action_flag  = false;

bool print_preheat_check = false;
bool probe_offset_flag   = false;

millis_t next_rts_update_ms = 0;
int PrintFlag               = 0;

float ChangeFilamentTemp = 200;
int heatway              = 0;

int last_target_temperature[4] = {0};
int last_target_temperature_bed;

char waitway = 0;

int change_page_font        = 1;
unsigned char Percentrecord = 0;
bool CardUpdate             = false;

int16_t fileCnt           = 0;
uint8_t file_current_page = 1;
uint8_t file_total_page   = 1;
uint8_t page_total_file   = 0;

DB RTSSHOW::recdat;
DB RTSSHOW::snddat;

uint8_t lang = 2;
bool lcd_sd_status;

int FilenamesCount = 0;

char cmdbuf[20] = {0};

float FilamentLOAD = 10;

float FilamentUnLOAD = 10;

unsigned char AxisUnitMode;
unsigned char AutoHomeIconNum;
float axis_unit       = 10.0;
int Update_Time_Value = 0;
bool PoweroffContinue = false;
char commandbuf[30];
static bool last_card_insert_st;
bool card_insert_st;
bool sd_printing;
bool sd_printing_autopause;

bool home_flag       = false;
bool rts_start_print = false;

const int manual_level_5position[9][2] = MANUALL_BED_LEVEING_5POSITION;

enum {
  PREHEAT_PLA = 0,
  PREHEAT_ABS = 1,
  PREHEAT_PETG = 2,
  PREHEAT_CUST = 3,
};

int temp_preheat_nozzle = 0, temp_preheat_bed = 0;
uint8_t preheat_flag = PREHEAT_PLA; // 0=PLA，1=ABS, 2=PETG, 3=CUST

uint8_t last_progress_percent = 0;
uint32_t last_start_time      = 0;
uint32_t last_remaining_time  = 0;

BedNozzleHeightCalSt st_bedNozzleHeightCal = {0};
float bedNozzleHeightCalZ                  = 0;
bool g_heaterLoadTempAdd                   = false;
bool g_uiXYAxisEnable                      = false;
bool g_uiZAxisEnable                       = false;
bool g_uiZOffsetHomeOkFlag                 = false;
bool g_uiAutoPIDFlag                       = false;
int16_t g_autoPIDHeaterTempTarget          = 300;

#if ENABLED(ENDER_3S1_PRO)
  int16_t g_autoPIDHotBedTempTarget = 110;
#elif ENABLED(ENDER_3S1_PLUS)
  int16_t g_autoPIDHotBedTempTarget = 100;
#endif

int16_t g_autoPIDHeaterCycles = 8;
int16_t g_autoPIDHotBedCycles = 8;

int16_t g_autoPIDHeaterTempTargetset   = 0;
int16_t g_autoPIDHotBedTempTargetset   = 0;
int16_t g_autoPIDHeaterCyclesTargetset = 0;
int16_t g_autoPIDHotBedCyclesTargetset = 0;

bool g_uiAutoPIDHotbedRuningFlag = false;
bool g_uiAutoPIDNozzleRuningFlag = false;
int8_t g_uiAutoPIDRuningDiff     = 0;
int16_t g_uiCurveDataCnt         = 0;

int16_t advance_k_set = 0;

/*************************************END***************************************/

inline void RTS_line_to_current(AxisEnum axis) {
  if (!planner.is_full())
    planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[(int8_t)axis]), active_extruder);
}

RTSSHOW::RTSSHOW() {
  recdat.head[0] = snddat.head[0] = FHONE;
  recdat.head[1] = snddat.head[1] = FHTWO;
  memset(databuf, 0, sizeof(databuf));
}

static void RTS_line_to_filelist() {
  // clean filename Icon
  for (int j = 0; j < 5; j++)
    for (int i = 0; i < TEXTBYTELEN; i++) rtscheck.RTS_SndData(0, CardRecbuf.addr[j] + i);

  memset(&CardRecbuf, 0, sizeof(CardRecbuf));

  int num = 0;
  for (int16_t i = (file_current_page - 1) * 5; i < (file_current_page * 5); i++) {
    card.selectFileByIndexSorted(i);
    char *pointFilename = card.longFilename;
    int filenamelen     = strlen(card.longFilename);
    int j               = 1;
    while ((strncmp(&pointFilename[j], ".gcode", 6) && strncmp(&pointFilename[j], ".GCODE", 6)) && ((j++) < filenamelen));
    if (j >= TEXTBYTELEN) {
      strncpy(&card.longFilename[TEXTBYTELEN - 3], "..", 2);
      card.longFilename[TEXTBYTELEN - 1] = '\0';
      j                                  = TEXTBYTELEN - 1;
    }
    strncpy(CardRecbuf.Cardshowfilename[num], card.longFilename, j);
    #if ENABLED(FILAMENT_RUNOUT_SENSOR_DEBUG)
      SERIAL_ECHO("inside rts_line_to_filelist");
      SERIAL_ECHOLN("");
    #endif
    strcpy(CardRecbuf.Cardfilename[num], card.filename);
    CardRecbuf.addr[num] = FILE1_TEXT_VP + (num * 20);
    rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[num], CardRecbuf.addr[num]);
    CardRecbuf.Filesum = (++num);
  }
  page_total_file    = CardRecbuf.Filesum;
  CardRecbuf.Filesum = ((file_total_page - 1) * 5) + page_total_file;
}

void RTSSHOW::RTS_SDCardInit() {
  if (RTS_SD_Detected())
    card.mount();

  // DEBUG_ECHOLNPGM(" card.flag.mounted=: ", card.flag.mounted);

  if (card.flag.mounted) {
    int16_t fileCnt = card.get_num_items();
    card.getWorkDirName();
    if (card.filename[0] != '/') card.cdup();

    if (fileCnt > 5) {
      file_total_page = (fileCnt / 5) + 1;
      if (file_total_page > 5) file_total_page = 5;
    }
    else {
      file_total_page = 1;
    }

    RTS_SndData(file_total_page, PRINT_COUNT_PAGE_DATA_VP);
    file_current_page = 1;
    RTS_SndData(file_current_page, PRINT_CURRENT_PAGE_DATA_VP);
    RTS_line_to_filelist();
    CardRecbuf.selectFlag = false;

    if (PoweroffContinue /*|| print_job_timer.isRunning()*/) return;

    // clean print file
    for (int j = 0; j < 20; j++) RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
    lcd_sd_status = IS_SD_INSERTED();
  }
  else {
    if (PoweroffContinue) return;

    // Clean filename Icon
    for (int j = 0; j < MaxFileNumber; j++)
      for (int i = 0; i < TEXTBYTELEN; i++) RTS_SndData(0, CardRecbuf.addr[j] + i);

    memset(&CardRecbuf, 0, sizeof(CardRecbuf));
  }
}

bool RTSSHOW::RTS_SD_Detected() {
  static bool last, state;
  static bool flag_stable;
  static uint32_t stable_point_time;

  bool tmp = IS_SD_INSERTED();

  if (tmp != last) {
    flag_stable = false;
  }
  else if (!flag_stable) {
    flag_stable       = true;
    stable_point_time = millis() + 30;
  }

  if (flag_stable && ELAPSED(millis(), stable_point_time))
    state = tmp;

  last = tmp;

  return state;
}

void RTSSHOW::RTS_SDCardUpate() {
  const bool sd_status = RTS_SD_Detected();

  if (sd_status != lcd_sd_status) {
    if (sd_status) {
      // SD card power on
      RTS_SDCardInit();
    }
    else {

      if (PoweroffContinue /*|| print_job_timer.isRunning()*/) return;

      card.release();
      for (int i = 0; i < CardRecbuf.Filesum; i++) {
        for (int j = 0; j < 20; j++) RTS_SndData(0, CardRecbuf.addr[i] + j);
        RTS_SndData((unsigned long)0xFFFF, FilenameNature + (i + 1) * 16);
      }
      for (int j = 0; j < 20; j++) {
        // clean screen.
        RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
        RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
      }
      memset(&CardRecbuf, 0, sizeof(CardRecbuf));
      RTS_SndData(1, PRINT_COUNT_PAGE_DATA_VP);
      file_total_page = 1;
      RTS_SndData(1, PRINT_CURRENT_PAGE_DATA_VP);
      file_current_page = 1;
    }
    lcd_sd_status = sd_status;
  }

  // represents to update file list
  if (CardUpdate && lcd_sd_status && RTS_SD_Detected()) {
    RTS_line_to_filelist();
    for (uint16_t i = 0; i < 5; i++) {
      delay(1);
      RTS_SndData((unsigned long)0xFFFF, FilenameNature + (i + 1) * 16);
    }
    // hal.watchdog_refresh();
    CardUpdate = false;
  }
}

void RTSSHOW::RTS_Init() {

  delay(1000);

  AxisUnitMode = 1;
  lang         = language_change_font;

  // x_min_pos_eeprom = BL24CXX::readOneByte(x_min_pos_eeprom);
  // y_min_pos_eeprom = BL24CXX::readOneByte(y_min_pos_eeprom);

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (!IS_SD_INSERTED()) { delay(500); card.mount(); }
    if (IS_SD_INSERTED()) recovery.check();
  #endif

  delay(500);

  last_zoffset               = zprobe_zoffset = probe.offset.z;
  touchscreen_requested_mesh = 0;
  RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
  last_xoffset = xprobe_xoffset = probe.offset_xy.x;
  RTS_SndData(xprobe_xoffset * 100, HOTEND_X_ZOFFSET_VP);
  last_yoffset = yprobe_yoffset  = probe.offset_xy.y;
  RTS_SndData(yprobe_yoffset * 100, HOTEND_Y_ZOFFSET_VP);

  x_frequency = stepper.get_shaping_frequency(X_AXIS);
  RTS_SndData(x_frequency * 100, SHAPING_X_FREQUENCY_VP);
  y_frequency = stepper.get_shaping_frequency(Y_AXIS);
  RTS_SndData(y_frequency * 100, SHAPING_Y_FREQUENCY_VP);

  x_zeta = stepper.get_shaping_damping_ratio(X_AXIS);
  RTS_SndData(x_zeta * 100, SHAPING_X_ZETA_VP);
  y_zeta = stepper.get_shaping_damping_ratio(Y_AXIS);
  RTS_SndData(y_zeta * 100, SHAPING_Y_ZETA_VP);

  for (int i = 0; i < 9; i++) RTS_SndData(0, LANGUAGE_CHINESE_TITLE_VP + i);
  RTS_SndData(1, LANGUAGE_CHINESE_TITLE_VP + (language_change_font - 1));
  languagedisplayUpdate();
  delay(500);

  last_target_temperature[0]  = thermalManager.temp_hotend[0].target;
  last_target_temperature_bed = thermalManager.temp_bed.target;
  feedrate_percentage         = 100;
  RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);

  /***************turn off motor*****************/
  RTS_SndData(1, MOTOR_FREE_ICON_VP);

  /***************transmit temperature to screen*****************/
  RTS_SndData(0, HEAD_SET_TEMP_VP);
  RTS_SndData(0, BED_SET_TEMP_VP);
  RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD_CURRENT_TEMP_VP);
  RTS_SndData(thermalManager.temp_bed.celsius, BED_CURRENT_TEMP_VP);
  RTS_SndData(ui.material_preset[0].hotend_temp, PREHEAT_PLA_SET_NOZZLE_TEMP_VP);
  RTS_SndData(ui.material_preset[0].bed_temp, PREHEAT_PLA_SET_BED_TEMP_VP);
  RTS_SndData(ui.material_preset[1].hotend_temp, PREHEAT_ABS_SET_NOZZLE_TEMP_VP);
  RTS_SndData(ui.material_preset[1].bed_temp, PREHEAT_ABS_SET_BED_TEMP_VP);
  RTS_SndData(ui.material_preset[2].hotend_temp, PREHEAT_PETG_SET_NOZZLE_TEMP_VP);
  RTS_SndData(ui.material_preset[2].bed_temp, PREHEAT_PETG_SET_BED_TEMP_VP);
  RTS_SndData(ui.material_preset[3].hotend_temp, PREHEAT_CUST_SET_NOZZLE_TEMP_VP);
  RTS_SndData(ui.material_preset[3].bed_temp, PREHEAT_CUST_SET_BED_TEMP_VP);

  RTS_SndData(planner.settings.max_feedrate_mm_s[0], MAX_VELOCITY_XAXIS_DATA_VP);
  RTS_SndData(planner.settings.max_feedrate_mm_s[1], MAX_VELOCITY_YAXIS_DATA_VP);
  RTS_SndData(planner.settings.max_feedrate_mm_s[2], MAX_VELOCITY_ZAXIS_DATA_VP);
  RTS_SndData(planner.settings.max_feedrate_mm_s[3], MAX_VELOCITY_EAXIS_DATA_VP);

  RTS_SndData(planner.settings.max_acceleration_mm_per_s2[0], MAX_ACCEL_XAXIS_DATA_VP);
  RTS_SndData(planner.settings.max_acceleration_mm_per_s2[1], MAX_ACCEL_YAXIS_DATA_VP);
  RTS_SndData(planner.settings.max_acceleration_mm_per_s2[2], MAX_ACCEL_ZAXIS_DATA_VP);
  RTS_SndData(planner.settings.max_acceleration_mm_per_s2[3], MAX_ACCEL_EAXIS_DATA_VP);

  RTS_SndData(planner.max_jerk.x * 100, MAX_JERK_XAXIS_DATA_VP);
  RTS_SndData(planner.max_jerk.y * 100, MAX_JERK_YAXIS_DATA_VP);
  RTS_SndData(planner.max_jerk.z * 100, MAX_JERK_ZAXIS_DATA_VP);
  RTS_SndData(planner.max_jerk.e * 100, MAX_JERK_EAXIS_DATA_VP);

  RTS_SndData(planner.settings.axis_steps_per_mm[0] * 10, MAX_STEPSMM_XAXIS_DATA_VP);
  RTS_SndData(planner.settings.axis_steps_per_mm[1] * 10, MAX_STEPSMM_YAXIS_DATA_VP);
  RTS_SndData(planner.settings.axis_steps_per_mm[2] * 10, MAX_STEPSMM_ZAXIS_DATA_VP);
  RTS_SndData(planner.settings.axis_steps_per_mm[3] * 10, MAX_STEPSMM_EAXIS_DATA_VP);

  RTS_SndData(thermalManager.temp_hotend[0].pid.p() * 100, NOZZLE_TEMP_P_DATA_VP);
  RTS_SndData(thermalManager.temp_hotend[0].pid.i() * 100, NOZZLE_TEMP_I_DATA_VP);
  RTS_SndData(thermalManager.temp_hotend[0].pid.d() * 100, NOZZLE_TEMP_D_DATA_VP);
  RTS_SndData(thermalManager.temp_bed.pid.p() * 100, HOTBED_TEMP_P_DATA_VP);
  RTS_SndData(thermalManager.temp_bed.pid.i() * 100, HOTBED_TEMP_I_DATA_VP);
  RTS_SndData(thermalManager.temp_bed.pid.d() * 10, HOTBED_TEMP_D_DATA_VP);
  RTS_SndData(thermalManager.fan_speed[0], PRINTER_FAN_SPEED_DATA_VP);

  // Prepare for planned past release upgrades after M206 return.
  //RTS_SndData(workspace_offset.x * 100, HOME_X_OFFSET_VP);
  //RTS_SndData(workspace_offset.y * 100, HOME_Y_OFFSET_VP);

  //RTS_SndData(X_MIN_POS * 100, X_MIN_POS_VP);
  //RTS_SndData(Y_MIN_POS * 100, Y_MIN_POS_VP);

  //RTS_SndData(x_min_pos_eeprom * 100, X_MIN_POS_EEPROM_VP);
  //RTS_SndData(y_min_pos_eeprom * 100, Y_MIN_POS_EEPROM_VP);

  //RTS_SndData(X_BED_SIZE * 100, X_BEDSIZE_VP);
  //RTS_SndData(Y_BED_SIZE * 100, Y_BEDSIZE_VP);

  //RTS_SndData(home_offset.x * 100, HOME_X_OFFSET_NEW_VP);
  //RTS_SndData(home_offset.y * 100, HOME_Y_OFFSET_NEW_VP);

  RTS_SndData(StartSoundSet, 0);
  #if ENABLED(GCODE_PREVIEW_ENABLED)
    RTS_SndData(0, DEFAULT_PRINT_MODEL_VP);
    RTS_SndData(0, DOWNLOAD_PREVIEW_VP);
  #endif

  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    bool zig = false;
    int8_t inStart, inStop, inInc, showcount;
    showcount = 0;
    // settings.load();
    for (int y = 0; y < GRID_MAX_POINTS_Y; y++) {
      // away from origin
      if (zig) {
        inStart = GRID_MAX_POINTS_X - 1;
        inStop  = -1;
        inInc   = -1;
      }
      else {
        // towards origin
        inStart = 0;
        inStop  = GRID_MAX_POINTS_X;
        inInc   = 1;
      }
      zig ^= true;
      for (int x = inStart; x != inStop; x += inInc) {
        //SERIAL_ECHOLNPGM("value: ", bedlevel.z_values[x][y] * 1000);
        RTS_SndData(bedlevel.z_values[x][y] * 1000, AUTO_BED_LEVEL_1POINT_NEW_VP + showcount * 2);
        showcount++;
      }
    }
    queue.enqueue_now_P(PSTR("M420 S1"));
  #endif

  /***************transmit Fan speed to screen*****************/
  // turn off fans
  thermalManager.set_fan_speed(0, 0);
  /*********transmit SD card filename to screen***************/
  delay(5);
  RTS_SDCardInit();

  /***************transmit Printer information to screen*****************/
  RTS_SndData(MACHINE_TYPE, MACHINE_TYPE_ABOUT_TEXT_VP);
  RTS_SndData(FIRMWARE_VERSION, FIRMWARE_VERSION_ABOUT_TEXT_VP);
  RTS_SndData(PRINT_SIZE, PRINTER_PRINTSIZE_TEXT_VP);
  #if ENABLED(GCODE_PREVIEW_ENABLED)
    gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
  #endif
  RTS_SndData(HARDWARE_VERSION, HARDWARE_VERSION_ABOUT_TEXT_VP);
  RTS_SndData(PRINT_SIZE, PRINTER_PRINTSIZE_TEXT_VP);
  delay(5);
  RTS_SndData(lang == 1 ? CORP_WEBSITE_C : CORP_WEBSITE_E, WEBSITE_ABOUT_TEXT_VP);

  RTS_SndData(recovery.enabled ? 0 : 1, POWERCONTINUE_CONTROL_ICON_VP);
  RTS_SndData(runout.enabled ? 0 : 1, FILAMENT_CONTROL_ICON_VP);

  if (g_soundSetOffOn == 2) {
    RTS_SndData(DC_SOUND_SET_OFF, DC_SOUND_SET_DDR);
    RTS_SndData(102, SOUND_SETTING_OFF_ON_VP);
  }
  else {
    RTS_SndData(DC_SOUND_SET_ON, DC_SOUND_SET_DDR);
    RTS_SndData(101, SOUND_SETTING_OFF_ON_VP);
  }

  /**************************some info init*******************************/
  RTS_SndData(0, PRINT_PROCESS_ICON_VP);
  RTS_SndData(0, PRINT_PROCESS_VP);
  RTS_SndData(0, PRINT_REMAIN_TIME_HOUR_VP);
  RTS_SndData(0, PRINT_REMAIN_TIME_MIN_VP);

  RTS_SndData(1, PREHAEAT_NOZZLE_ICON_VP);
  RTS_SndData(1, PREHAEAT_HOTBED_ICON_VP);
  // rtscheck.RTS_SndData(1, FILAMENT_CONTROL_ICON_VP);

  // Disable Filename Field on startup
  for (int j = 0; j < 20; j++) {
    // clean screen.
    RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
    RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
  }

  rtscheck.RTS_SndData(ExchangePageBase, ExchangepageAddr);
  change_page_font = 0;
  hal.watchdog_refresh();
  for (startprogress = 0; startprogress <= 100; startprogress++) {
    rtscheck.RTS_SndData(startprogress, START_PROCESS_ICON_VP);
    hal.watchdog_refresh();
    delay(50);
  }
  hal.watchdog_refresh();
  delay(500);
  Update_Time_Value = RTS_UPDATE_VALUE;
}

int RTSSHOW::RTS_RecData() {
  static int recnum = 0;

  while ((LCDSERIAL.available() > 0) && (recnum < SizeofDatabuf)) {
    delay(1);
    databuf[recnum] = LCDSERIAL.read();
    if (databuf[0] == FHONE) {
      recnum++;
    }
    else if (databuf[0] == FHTWO) {
      databuf[0] = FHONE;
      databuf[1] = FHTWO;
      recnum    += 2;
    }
    else if (databuf[0] == FHLENG) {
      databuf[0] = FHONE;
      databuf[1] = FHTWO;
      databuf[2] = FHLENG;
      recnum    += 3;
    }
    else if (databuf[0] == VarAddr_R) {
      databuf[0] = FHONE;
      databuf[1] = FHTWO;
      databuf[2] = FHLENG;
      databuf[3] = VarAddr_R;
      recnum    += 4;
    }
    else {
      recnum = 0;
    }
  }

  // receive nothing
  if (recnum < 1) {
    return -1;
  }
  else if ((recdat.head[0] == databuf[0]) && (recdat.head[1] == databuf[1]) && (recnum > 2)) {
    recdat.len     = databuf[2];
    recdat.command = databuf[3];
    // response for writing byte
    if ((recdat.len == 0x03) && ((recdat.command == 0x82) || (recdat.command == 0x80)) && (databuf[4] == 0x4F) && (databuf[5] == 0x4B)) {
      memset(databuf, 0, sizeof(databuf));
      recnum = 0;
      return -1;
    }
    else if (recdat.command == 0x83) {
      // response for reading the data from the variate
      recdat.addr    = databuf[4];
      recdat.addr    = (recdat.addr << 8) | databuf[5];
      recdat.bytelen = databuf[6];
      for (unsigned int i = 0; i < recdat.bytelen; i += 2) {
        recdat.data[i / 2] = databuf[7 + i];
        recdat.data[i / 2] = (recdat.data[i / 2] << 8) | databuf[8 + i];
      }
    }
    else if (recdat.command == 0x81) {
      // response for reading the page from the register
      recdat.addr    = databuf[4];
      recdat.bytelen = databuf[5];
      for (unsigned int i = 0; i < recdat.bytelen; i++) {
        recdat.data[i] = databuf[6 + i];
        // recdat.data[i] = (recdat.data[i] << 8 )| databuf[7 + i];
      }
    }
  }
  else {
    memset(databuf, 0, sizeof(databuf));
    recnum = 0;
    // receive the wrong data
    return -1;
  }
  memset(databuf, 0, sizeof(databuf));
  recnum = 0;
  return 2;
}


int RTSSHOW::RTS_RecData2() {
  static int recnum = 0;
  #define DWINBUF_MAX 256
  static bool recvflag = false;
  static uint8_t databuf[DWINBUF_MAX];

  // Parse data frame
  if ((LCDSERIAL.available() > 0) && (recnum < (signed)sizeof(databuf))) {
    databuf[recnum++] = LCDSERIAL.read();

    #define RECV_DEBUG
    #if defined(RECV_DEBUG)
      char buf[16];
      sprintf_P(buf, PSTR("%02x "), databuf[recnum - 1]);
      //SERIAL_ECHO(buf, databuf[i]);
    #endif

    // Verify the frame head
    if ((recnum == 1) && (databuf[0] != 0x5A)) {
      recnum = 0;
    }
    else if ((recnum == 2) && (databuf[1] != 0xA5)) {
      // Verify the frame head
      recnum = 0;
    }
    else if ((recnum == 4) && (databuf[3] != 0x83)) {
      // Parse only the read command 0x83 and filter out the reply command 0x82
      recnum = 0;
    }
    else if ((recnum >= 3) && (databuf[2] == (recnum - 3))) {
      // Complete resolution
      recvflag = true;

      #ifdef RECV_DEBUG
        //SERIAL_EOL();
        //SERIAL_ECHO_MSG("dwin rx ok");
      #endif
    }
    else if ((recnum >= 3) && ((recnum - 3) > databuf[2])) {
      // The actual received data bytes were larger than the frame data bytes, parsing failed
      recnum = 0;
    }
  }

  if (!recvflag)
    return -1;
  else
    recvflag = false;

  // receive nothing
  if (recnum < 1) {
    return -1;
  }
  else if ((recdat.head[0] == databuf[0]) && (recdat.head[1] == databuf[1]) && (recnum > 2)) {
    recdat.len     = databuf[2];
    recdat.command = databuf[3];
    // response for writing byte
    if ((recdat.len == 0x03) && ((recdat.command == 0x82) || (recdat.command == 0x80)) && (databuf[4] == 0x4F) && (databuf[5] == 0x4B)) {
      memset(databuf, 0, sizeof(databuf));
      recnum = 0;
      return -1;
    }
    else if (recdat.command == 0x83) {
      // response for reading the data from the variate
      recdat.addr    = databuf[4];
      recdat.addr    = (recdat.addr << 8) | databuf[5];
      recdat.bytelen = databuf[6];
      for (unsigned int i = 0; i < recdat.bytelen; i += 2) {
        recdat.data[i / 2] = databuf[7 + i];
        recdat.data[i / 2] = (recdat.data[i / 2] << 8) | databuf[8 + i];
      }
    }
    else if (recdat.command == 0x81) {
      // response for reading the page from the register
      recdat.addr    = databuf[4];
      recdat.bytelen = databuf[5];
      for (unsigned int i = 0; i < recdat.bytelen; i++) {
        recdat.data[i] = databuf[6 + i];
        // recdat.data[i] = (recdat.data[i] << 8 )| databuf[7 + i];
      }
    }
  }
  else {
    memset(databuf, 0, sizeof(databuf));
    recnum = 0;
    // receive the wrong data
    return -1;
  }
  memset(databuf, 0, sizeof(databuf));
  recnum = 0;
  return 2;
}

void RTSSHOW::RTS_SndData() {
  if ((snddat.head[0] == FHONE) && (snddat.head[1] == FHTWO) && (snddat.len >= 3)) {
    databuf[0] = snddat.head[0];
    databuf[1] = snddat.head[1];
    databuf[2] = snddat.len;
    databuf[3] = snddat.command;

    // to write data to the register
    if (snddat.command == 0x80) {
      databuf[4] = snddat.addr;
      for (int i = 0; i < (snddat.len - 2); i++) databuf[5 + i] = snddat.data[i];
    }
    else if ((snddat.len == 3) && (snddat.command == 0x81)) {
      // to read data from the register
      databuf[4] = snddat.addr;
      databuf[5] = snddat.bytelen;
    }
    else if (snddat.command == 0x82) {
      // to write data to the variate
      databuf[4] = snddat.addr >> 8;
      databuf[5] = snddat.addr & 0xFF;
      for (int i = 0; i < (snddat.len - 3); i += 2) {
        databuf[6 + i] = snddat.data[i / 2] >> 8;
        databuf[7 + i] = snddat.data[i / 2] & 0xFF;
      }
    }
    else if ((snddat.len == 4) && (snddat.command == 0x83)) {
      // to read data from the variate
      databuf[4] = snddat.addr >> 8;
      databuf[5] = snddat.addr & 0xFF;
      databuf[6] = snddat.bytelen;
    }
    // usart_tx(LCDSERIAL.c_dev(), databuf, snddat.len + 3);
    // LCDSERIAL.flush();
    for (int i = 0; i < (snddat.len + 3); i++) {
      LCDSERIAL.write(databuf[i]);
      delayMicroseconds(1);
    }

    memset(&snddat, 0, sizeof(snddat));
    memset(databuf, 0, sizeof(databuf));
    snddat.head[0] = FHONE;
    snddat.head[1] = FHTWO;
  }
}

void RTSSHOW::RTS_SndData(const String &s, unsigned long addr, unsigned char cmd /*= VarAddr_W*/) {
  if (s.length() < 1)
    return;
  RTS_SndData(s.c_str(), addr, cmd);
}

void RTSSHOW::RTS_SndData(const char *str, unsigned long addr, unsigned char cmd /*= VarAddr_W*/) {
  int len = strlen(str);
  if (len > 0) {
    databuf[0] = FHONE;
    databuf[1] = FHTWO;
    databuf[2] = 3 + len;
    databuf[3] = cmd;
    databuf[4] = addr >> 8;
    databuf[5] = addr & 0x00FF;
    for (int i = 0; i < len; i++) databuf[6 + i] = str[i];
    for (int i = 0; i < (len + 6); i++) {
      //SERIAL_ECHOPGM("databuff3++: ", databuf[i]);
      LCDSERIAL.write(databuf[i]);
      delayMicroseconds(1);
    }
    memset(databuf, 0, sizeof(databuf));
  }
}

void RTSSHOW::RTS_SendCurveData(uint8_t channel, uint16_t *vaule, uint8_t size) {
  databuf[0]  = FHONE;
  databuf[1]  = FHTWO;
  databuf[2]  = 9 + size * 2;
  databuf[3]  = VarAddr_W;
  databuf[4]  = 0x03;
  databuf[5]  = 0x10;
  databuf[6]  = 0x5A;
  databuf[7]  = 0xA5;
  databuf[8]  = 0x01;
  databuf[9]  = 0x00;
  databuf[10] = channel;
  databuf[11] = size;

  for (int i = 0, j = 0; j < size; j++) {
    databuf[i + 12] = vaule[j] >> 8;
    i++;
    databuf[i + 12] = vaule[j] & 0x00FF;
    i++;
    if (i >= SizeofDatabuf) break;
  }
  for (int i = 0; i < (size * 2 + 12); i++) {

    //SERIAL_ECHOPGM("databuff2++: ", databuf[i]);
    LCDSERIAL.write(databuf[i]);
    delayMicroseconds(1);
  }
  memset(databuf, 0, sizeof(databuf));
}

void RTSSHOW::RTS_SndData(char c, unsigned long addr, unsigned char cmd /*= VarAddr_W*/) {
  snddat.command = cmd;
  snddat.addr    = addr;
  snddat.data[0] = (unsigned long)c;
  snddat.data[0] = snddat.data[0] << 8;
  snddat.len     = 5;
  RTS_SndData();
}

void RTSSHOW::RTS_SndData(unsigned char *str, unsigned long addr, unsigned char cmd) { RTS_SndData((char *)str, addr, cmd); }

void RTSSHOW::RTS_SndData(int n, unsigned long addr, unsigned char cmd /*= VarAddr_W*/) {
  if (cmd == VarAddr_W) {
    if (n > 0xFFFF) {
      snddat.data[0] = n >> 16;
      snddat.data[1] = n & 0xFFFF;
      snddat.len     = 7;
    }
    else {
      snddat.data[0] = n;
      snddat.len     = 5;
    }
  }
  else if (cmd == RegAddr_W) {
    snddat.data[0] = n;
    snddat.len     = 3;
  }
  else if (cmd == VarAddr_R) {
    snddat.bytelen = n;
    snddat.len     = 4;
  }
  snddat.command = cmd;
  snddat.addr    = addr;
  RTS_SndData();
}

void RTSSHOW::RTS_SndData(unsigned int n, unsigned long addr, unsigned char cmd) { RTS_SndData((int)n, addr, cmd); }

void RTSSHOW::RTS_SndData(float n, unsigned long addr, unsigned char cmd) { RTS_SndData((int)n, addr, cmd); }

void RTSSHOW::RTS_SndData(long n, unsigned long addr, unsigned char cmd) { RTS_SndData((unsigned long)n, addr, cmd); }

void RTSSHOW::RTS_SndData(unsigned long n, unsigned long addr, unsigned char cmd /*= VarAddr_W*/) {
  if (cmd == VarAddr_W) {
    if (n > 0xFFFF) {
      snddat.data[0] = n >> 16;
      snddat.data[1] = n & 0xFFFF;
      snddat.len     = 7;
    }
    else {
      snddat.data[0] = n;
      snddat.len     = 5;
    }
  }
  else if (cmd == VarAddr_R) {
    snddat.bytelen = n;
    snddat.len     = 4;
  }
  snddat.command = cmd;
  snddat.addr    = addr;
  RTS_SndData();
}

void RTSSHOW::RTS_SndText(const char string[], unsigned long addr, uint8_t textSize) {
  for (int8_t i = 0; i < textSize; i++) rtscheck.RTS_SndData(0, addr + i);
  rtscheck.RTS_SndData(string, addr);
}

void RTSSHOW::RTS_SDcard_Stop() {

  card.flag.abort_sd_printing = true;
  queue.clear();
  if (home_flag) planner.synchronize();
  quickstop_stepper();
  print_job_timer.stop();
  #if DISABLED(SD_ABORT_NO_COOLDOWN)
    thermalManager.disable_all_heaters();
  #endif
  TERN_(HOST_PAUSE_M76, hostui.cancel());
  print_job_timer.reset();
  thermalManager.setTargetHotend(0, 0);
  RTS_SndData(0, HEAD_SET_TEMP_VP);
  thermalManager.setTargetBed(0);
  RTS_SndData(0, BED_SET_TEMP_VP);
  temphot = 0;
  thermalManager.zero_fan_speeds();
  wait_for_heatup  = wait_for_user = false;
  PoweroffContinue = false;

  sd_printing_autopause = false;
  if (card.flag.mounted) {
    #if ENABLED(SDSUPPORT) && ENABLED(POWER_LOSS_RECOVERY)
      card.removeJobRecoveryFile();
    #endif
  }
  // shut down the stepper motor.
  // queue.enqueue_now_P(PSTR("M84"));
  RTS_SndData(1, MOTOR_FREE_ICON_VP);

  RTS_SndData(0, PRINT_PROCESS_ICON_VP);
  RTS_SndData(0, PRINT_PROCESS_VP);
  delay(2);
  RTS_SndData(0, PRINT_REMAIN_TIME_HOUR_VP);
  RTS_SndData(0, PRINT_REMAIN_TIME_MIN_VP);

  RTS_SndData(0, PRINT_TIME_HOUR_VP);
  RTS_SndData(0, PRINT_TIME_MIN_VP);

  for (int j = 0; j < 20; j++) {
    // clean screen.
    RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
    // clean filename
    RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
  }
  planner.synchronize();
  RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
  change_page_font            = 1;
  card.flag.abort_sd_printing = true;
}

void RTSSHOW::RTS_HandleData() {
  int Checkkey = -1;
  // for waiting
  if (waitway > 0) {
    memset(&recdat, 0, sizeof(recdat));
    recdat.head[0] = FHONE;
    recdat.head[1] = FHTWO;
    return;
  }
  for (int i = 0; addrBuf[i] != 0; i++)
    if (recdat.addr == addrBuf[i]) {
      if (addrBuf[i] >= ChangePageKey)
        Checkkey = i;
      break;
    }

  if (Checkkey < 0) {
    memset(&recdat, 0, sizeof(recdat));
    recdat.head[0] = FHONE;
    recdat.head[1] = FHTWO;
    return;
  }
  #if ENABLED(FILAMENT_RUNOUT_SENSOR_DEBUG)
    SERIAL_ECHO_MSG("\nCheckkey=", Checkkey, "recdat.data[0]=", recdat.data[0]);
  #endif
  switch (Checkkey) {
    //SERIAL_ECHO_MSG("Recorded value Catchall\n", Checkkey);
    case MainEnterKey:
      if (recdat.data[0] == 1) {
        CardUpdate             = true;
        CardRecbuf.recordcount = -1;
        #if ENABLED(FILAMENT_RUNOUT_SENSOR_DEBUG)
          SERIAL_ECHOPGM("Working dir is: ");
          SERIAL_ECHO(card.getWorkDirName());
          SERIAL_ECHOLN("");
        #endif
        std::string currentdir;
        currentdir = card.getWorkDirName();
        if (card.getWorkDirName() != std::string("/")) {
          card.cdup();
          #if ENABLED(FILAMENT_RUNOUT_SENSOR_DEBUG)
            SERIAL_ECHO("chroot done to:");
            SERIAL_ECHO(card.getWorkDirName());
            SERIAL_ECHOLN("");
          #endif
        }

        if (card.flag.mounted) {
          int16_t fileCnt = card.get_num_items();

          if (fileCnt > 5) {
            file_total_page = (fileCnt / 5) + 1;
            if (file_total_page > 5) file_total_page = 5;
          }
          else {
            file_total_page = 1;
          }

          RTS_SndData(file_total_page, PRINT_COUNT_PAGE_DATA_VP);
          file_current_page = 1;
          RTS_SndData(file_current_page, PRINT_CURRENT_PAGE_DATA_VP);

          RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
          change_page_font = 2;
          if (IS_SD_INSERTED()) RTS_line_to_filelist();
        }
      }
      else if (recdat.data[0] == 2) {
        AxisUnitMode = 1;
        axis_unit    = 10.0;

        if (axes_should_home()) {
          waitway = 4;
          queue.enqueue_one_P(PSTR("G28"));
          RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
          change_page_font = 40;
        }
        else {
          RTS_SndData(ExchangePageBase + 16, ExchangepageAddr);
          change_page_font = 16;
          RTS_SndData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
          RTS_SndData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
          RTS_SndData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
        }
        //RTS_SndData(1, FILAMENT_CONTROL_ICON_VP);
      }
      else if (recdat.data[0] == 3) {
        RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
        change_page_font = 21;
      }
      else if (recdat.data[0] == 4) {
        RTS_SndData(ExchangePageBase + 25, ExchangepageAddr);
        change_page_font = 25;
        planner.synchronize();
        queue.enqueue_now_P(PSTR("G28\nG1 F200 Z0.0"));
        //RTS_SndData(1, AUTO_BED_LEVEL_TITLE_VP);
        RTS_SndData(0, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 5) {
        queue.clear();
        quickstop_stepper();
        print_job_timer.stop();
        RTS_SndData(1, MOTOR_FREE_ICON_VP);
        RTS_SndData(0, PRINT_PROCESS_ICON_VP);
        RTS_SndData(0, PRINT_PROCESS_VP);
        delay(2);
        RTS_SndData(0, PRINT_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_TIME_MIN_VP);
        RTS_SndData(0, PRINT_REMAIN_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_REMAIN_TIME_MIN_VP);

        print_job_timer.reset();
        sd_printing_autopause = false;
        for (int j = 0; j < 20; j++) {
          // clean screen.
          RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
          // clean filename
          RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
        }
        CardRecbuf.recordcount = -1;
        RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        change_page_font = 1;
        #if ENABLED(GCODE_PREVIEW_ENABLED)
          gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
        #endif
      }
      else if (recdat.data[0] == 6) { // Start bedleveling
        waitway = 3;
        RTS_SndData(1, AUTO_BED_LEVEL_TITLE_VP);
        RTS_SndData(AUTO_BED_LEVEL_PREHEAT, AUTO_BED_PREHEAT_HEAD_DATA_VP);
        rtscheck.RTS_SndData(0, AUTO_LEVELING_PERCENT_DATA_VP);
        if (thermalManager.temp_hotend[0].celsius < (AUTO_BED_LEVEL_PREHEAT - 5))
          queue.enqueue_now_P(PSTR("G4 S40"));

        if (axes_should_home()) queue.enqueue_one_P(PSTR("G28"));

        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          queue.enqueue_one_P(PSTR("G29"));
        #else
          touchscreen_requested_mesh = 1;
          queue.enqueue_one_P(PSTR("G29 P1 T"));
          // queue.enqueue_one_P(PSTR("G29 P3"));
          // queue.enqueue_one_P(PSTR("G29 S1"));
        #endif
        RTS_SndData(0, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 7) {
        if (errorway == 1) {
        }
        else if (errorway == 2) {
          // auto home fail
        }
        else if (errorway == 3) {
          // bed leveling fail
        }
        else if (errorway == 4) {
        }
      }
      else if (recdat.data[0] == 8) {
        RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        #if ENABLED(GCODE_PREVIEW_ENABLED)
          if (false == CardRecbuf.selectFlag)
            gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);

        #endif
        change_page_font = 1;
      }
      else if (recdat.data[0] == 9) {
        RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
        change_page_font = 11;
      }
      else if (recdat.data[0] == 0x0A) {
        RTS_SndData(ExchangePageBase + 13, ExchangepageAddr);
        change_page_font = 13;
      }
      else if (recdat.data[0] == 161) {

        g_autoPIDHeaterTempTargetset = g_autoPIDHeaterTempTargetset;
        if (g_autoPIDHeaterTempTargetset != 0)
          g_autoPIDHeaterTempTarget =  g_autoPIDHeaterTempTargetset;
        g_autoPIDHeaterCyclesTargetset = g_autoPIDHeaterCyclesTargetset;
        if (g_autoPIDHeaterCyclesTargetset != 0)
          g_autoPIDHeaterCycles =  g_autoPIDHeaterCyclesTargetset;
        RTS_SndData(g_autoPIDHeaterTempTarget, AUTO_PID_SET_NOZZLE_TEMP);
        RTS_SndData(g_autoPIDHeaterCycles, AUTO_PID_SET_NOZZLE_CYCLES);
        RTS_SndData(0, PID_TEXT_OUT_CUR_CYCLE_HOTBED_VP);
        RTS_SndData(0, AUTO_PID_NOZZLE_CYCLES);
        RTS_SndData(0, AUTO_PID_HOTBED_CYCLES);
        RTS_SndData(0, WRITE_CURVE_DDR_CMD);
        RTS_SndData("                           ", PID_TEXT_OUT_VP);
        RTS_SndData(ExchangePageBase + 83, ExchangepageAddr);
        change_page_font = 83;
        if (g_uiAutoPIDNozzleRuningFlag == true) {
        }
        else {
          RTS_SndData(0, AUTO_PID_RUN_NOZZLE_TIS_VP);
          RTS_SndData(lang, AUTO_PID_NOZZLE_TIS_VP);
        }
      }
      else if (recdat.data[0] == 162) {
        g_autoPIDHotBedTempTargetset = g_autoPIDHotBedTempTargetset;
        if (g_autoPIDHotBedTempTargetset != 0)
          g_autoPIDHotBedTempTarget =  g_autoPIDHotBedTempTargetset;
        g_autoPIDHotBedCyclesTargetset = g_autoPIDHotBedCyclesTargetset;
        if (g_autoPIDHotBedCyclesTargetset != 0)
          g_autoPIDHotBedCycles =  g_autoPIDHotBedCyclesTargetset;
        RTS_SndData(g_autoPIDHotBedTempTarget, AUTO_PID_SET_HOTBED_TEMP);
        RTS_SndData(g_autoPIDHotBedCycles, AUTO_PID_SET_HOTBED_CYCLES);
        RTS_SndData(0, PID_TEXT_OUT_CUR_CYCLE_HOTBED_VP);
        RTS_SndData(0, AUTO_PID_HOTBED_CYCLES);
        RTS_SndData(0, WRITE_CURVE_DDR_CMD);
        RTS_SndData("                           ", PID_TEXT_OUT_VP);
        RTS_SndData(ExchangePageBase + 84, ExchangepageAddr);
        change_page_font = 84;
        if (g_uiAutoPIDHotbedRuningFlag == true) {
        }
        else {
          RTS_SndData(0, AUTO_PID_RUN_HOTBED_TIS_VP);
          RTS_SndData(lang, AUTO_PID_HOTBED_TIS_VP);
        }
      }
      else if (recdat.data[0] == 163) {
        AxisUnitMode = 1;
        axis_unit    = 10.0;

        if (axes_should_home()) {
          waitway = 4;
          queue.enqueue_one_P(PSTR("G28"));
          RTS_SndData(ExchangePageBase + 81, ExchangepageAddr);
          change_page_font = 81;
        }
        else {
          RTS_SndData(ExchangePageBase + 16, ExchangepageAddr);
          change_page_font = 16;
          RTS_SndData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
          RTS_SndData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
          RTS_SndData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
        }
        //RTS_SndData(1, FILAMENT_CONTROL_ICON_VP);
      }
      break;

    case AdjustEnterKey:
      if (recdat.data[0] == 1) {
        // thermalManager.fan_speed[0] ? RTS_SndData(1, PRINTER_FANOPEN_TITLE_VP) : RTS_SndData(0, PRINTER_FANOPEN_TITLE_VP);
        RTS_SndData(ExchangePageBase + 14, ExchangepageAddr);
        change_page_font = 14;
      }
      else if (recdat.data[0] == 2) {
        if (card.isPrinting()) {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
        }
        else {
          RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
          change_page_font = 12;
        }
        settings.save();
      }
      else if (recdat.data[0] == 5) {
        RTS_SndData(ExchangePageBase + 15, ExchangepageAddr);
        change_page_font = 15;
      }
      else if (recdat.data[0] == 7) {
        RTS_SndData(ExchangePageBase + 14, ExchangepageAddr);
        change_page_font = 14;
        settings.save();
      }
      else if (recdat.data[0] == 8) {
        if (runout.enabled) {
          RTS_SndData(1, FILAMENT_CONTROL_ICON_VP);
          runout.enabled = false;
        }
        else {
          RTS_SndData(0, FILAMENT_CONTROL_ICON_VP);
          runout.enabled = true;
        }
        settings.save();
      }
      else if (recdat.data[0] == 9) {
        if (recovery.enabled) {
          RTS_SndData(1, POWERCONTINUE_CONTROL_ICON_VP);
          recovery.enabled = false;
        }
        else {
          RTS_SndData(0, POWERCONTINUE_CONTROL_ICON_VP);
          recovery.enabled = true;
        }
      }
      break;

    case PrintSpeedEnterKey:
      feedrate_percentage = recdat.data[0];
      RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
      break;

    case StopPrintKey:
      if (recdat.data[0] == 1) {
        RTS_SndData(ExchangePageBase + 13, ExchangepageAddr);
        change_page_font = 13;
      }
      else if (recdat.data[0] == 2) {
        RTS_SndData(0, PRINT_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_TIME_MIN_VP);
        Update_Time_Value = 0;
        temphot           = 0;

        runout.reset();
        wait_for_user = false;

        RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        #if ENABLED(GCODE_PREVIEW_ENABLED)
          gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
        #endif
        change_page_font = 1;
        RTS_SDcard_Stop();
      }
      else if (recdat.data[0] == 3) {
        if (card.isPrinting()) {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
        }
        else {
          RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
          change_page_font = 12;
        }
      }
      else if (recdat.data[0] == 4) {
        if (!planner.has_blocks_queued()) {
          if (PoweroffContinue == true) {
            runout.filament_ran_out = false;
            RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
            change_page_font = 40;
            waitway          = 7;
            #if ENABLED(FILAMENT_RUNOUT_SENSOR)
              if (runout.enabled == true) {
                pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;
                ui.pause_show_message(PAUSE_MESSAGE_RESUME);
                queue.inject_P(PSTR("M108"));
              }
            #endif
            RTS_SndData(0, PRINT_TIME_HOUR_VP);
            RTS_SndData(0, PRINT_TIME_MIN_VP);
            Update_Time_Value           = 0;
            temphot                     = 0;
            card.flag.abort_sd_printing = true;
            queue.clear();
            quickstop_stepper();

            print_job_timer.abort();
            // delay(10);
            while (planner.has_blocks_queued()) idle();
            thermalManager.setTargetHotend(0, 0);
            thermalManager.setTargetBed(0);
            thermalManager.zero_fan_speeds();
            while (thermalManager.temp_hotend[0].target > 0) {
              thermalManager.setTargetHotend(0, 0);
              idle();
            }
            RTS_SDcard_Stop();

          }
          else if (PoweroffContinue == false) {
            PoweroffContinue        = true;
            runout.filament_ran_out = false;
            RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
            change_page_font = 40;
            waitway          = 7;
            #if ENABLED(FILAMENT_RUNOUT_SENSOR)
              if (runout.enabled == true) {
                pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;
                ui.pause_show_message(PAUSE_MESSAGE_RESUME);
                queue.inject_P(PSTR("M108"));
              }
            #endif
            RTS_SndData(0, PRINT_TIME_HOUR_VP);
            RTS_SndData(0, PRINT_TIME_MIN_VP);
            Update_Time_Value           = 0;
            temphot                     = 0;
            card.flag.abort_sd_printing = true;
            queue.clear();
            quickstop_stepper();

            print_job_timer.abort();
            // delay(10);
            while (planner.has_blocks_queued()) idle();
            thermalManager.setTargetHotend(0, 0);
            thermalManager.setTargetBed(0);
            thermalManager.zero_fan_speeds();
            while (thermalManager.temp_hotend[0].target > 0) {
              thermalManager.setTargetHotend(0, 0);
              idle();
            }
            RTS_SDcard_Stop();

            PoweroffContinue = false;
            RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
            #if ENABLED(GCODE_PREVIEW_ENABLED)
              gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
            #endif
            change_page_font = 1;
          }
        }
      }
      else if (recdat.data[0] == 5) {
        if (PoweroffContinue == true) {
          RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
          change_page_font = 40;
          waitway          = 7;
          RTS_SndData(0, PRINT_TIME_HOUR_VP);
          RTS_SndData(0, PRINT_TIME_MIN_VP);
          thermalManager.setTargetHotend(0, 0);
          RTS_SndData(0, HEAD_SET_TEMP_VP);
          thermalManager.setTargetBed(0);
          RTS_SndData(0, BED_SET_TEMP_VP);
          temphot = 0;
          thermalManager.zero_fan_speeds();
          Update_Time_Value = 0;
          RTS_SDcard_Stop();
        }
      }
      break;

    case PausePrintKey:
      if (recdat.data[0] == 1) {
        if (card.isPrinting() && (thermalManager.temp_hotend[0].celsius > (thermalManager.temp_hotend[0].target - 5)) && (thermalManager.temp_bed.celsius > (thermalManager.temp_bed.target - 3))) {
          RTS_SndData(ExchangePageBase + 11, ExchangepageAddr);
          change_page_font = 11;
        }
        else {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
        }
      }
      else if (recdat.data[0] == 2) {
        if (card.isPrinting() && (thermalManager.temp_hotend[0].celsius > (thermalManager.temp_hotend[0].target - 5)) && (thermalManager.temp_bed.celsius > (thermalManager.temp_bed.target - 3))) {
        }
        else {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
          break;
        }

        waitway = 1;

        if (!temphot)
          temphot = thermalManager.temp_hotend[0].target;
        // card.pauseSDPrint();
        // print_job_timer.pause();
        queue.inject_P(PSTR("M25"));
        TERN_(HOST_PAUSE_M76, hostui.pause());
        pause_action_flag = true;
        Update_Time_Value = 0;
        RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
        change_page_font = 40;
        planner.synchronize();
        sdcard_pause_check = false;
      }
      else if (recdat.data[0] == 3) {
        if (card.isPrinting()) {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
        }
        else {
          RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
          change_page_font = 12;
        }
      }
      break;

    case ResumePrintKey:
      if (recdat.data[0] == 1) {
        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          if (READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_STATE) {
            RTS_SndData(ExchangePageBase + 7, ExchangepageAddr);
            change_page_font = 7;
            break;
          }
        #endif

        RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
        change_page_font = 10;

        #if ENABLED(HAS_RESUME_CONTINUE)
          if (wait_for_user) {
            wait_for_user = false;
          }
          else
        #endif
        {
          planner.synchronize();
          memset(commandbuf, 0, sizeof(commandbuf));
          sprintf_P(commandbuf, PSTR("M109 S%i"), temphot);
          queue.enqueue_one_now(commandbuf);
          // card.startOrResumeFilePrinting();
          // print_job_timer.start();
          queue.inject_P(PSTR("M24"));
          Update_Time_Value  = 0;
          sdcard_pause_check = true;
        }
      }
      else if (recdat.data[0] == 2) {
        if (thermalManager.temp_hotend[0].target >= EXTRUDE_MINTEMP)
          thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        else
          thermalManager.setTargetHotend(200, 0);

        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          if (READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_STATE) {
            RTS_SndData(ExchangePageBase + 7, ExchangepageAddr);
            change_page_font = 7;
          }
          else {
            RTS_SndData(ExchangePageBase + 8, ExchangepageAddr);
            change_page_font = 8;
          }
        #endif
      }
      else if (recdat.data[0] == 3) {
        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          if (READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_STATE) {
            RTS_SndData(ExchangePageBase + 7, ExchangepageAddr);
            change_page_font = 7;
            break;
          }
        #endif
        runout.filament_ran_out = false;
        pause_menu_response     = PAUSE_RESPONSE_RESUME_PRINT;
        ui.pause_show_message(PAUSE_MESSAGE_RESUME);
        queue.inject_P(PSTR("M108"));
        runout.reset();

        RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
        change_page_font = 10;

        card.startOrResumeFilePrinting();
        print_job_timer.start();
        Update_Time_Value  = 0;
        sdcard_pause_check = true;
      }
      break;

    case ZoffsetEnterKey:
      last_zoffset = zprobe_zoffset;
      if (recdat.data[0] >= 32768) {
        zprobe_zoffset  = ((float)recdat.data[0] - 65536) / 100;
        zprobe_zoffset -= 0.001;
      }
      else {
        zprobe_zoffset  = ((float)recdat.data[0]) / 100;
        zprobe_zoffset += 0.001;
      }
      if (WITHIN((zprobe_zoffset), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
        babystep.add_mm(Z_AXIS, zprobe_zoffset - last_zoffset);
        //SERIAL_ECHO_MSG("babystep.add_mm():", zprobe_zoffset - last_zoffset);
      probe.offset.z = zprobe_zoffset;
      RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      hal.watchdog_refresh();
      break;

    case Zoffset005EnterKey:
      last_zoffset = zprobe_zoffset;
      if (recdat.data[0] >= 32768) {
        zprobe_zoffset  = ((float)recdat.data[0] - 65536) / 100;
        zprobe_zoffset -= 0.001;
      }
      else {
        zprobe_zoffset  = ((float)recdat.data[0]) / 100;
        zprobe_zoffset += 0.001;
      }
      if (WITHIN((zprobe_zoffset), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
        babystep.add_mm(Z_AXIS, zprobe_zoffset - last_zoffset);
        //SERIAL_ECHO_MSG("babystep.add_mm():", zprobe_zoffset - last_zoffset);
      probe.offset.z = zprobe_zoffset;
      RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      hal.watchdog_refresh();
      break;

    case XoffsetEnterKey:
      last_xoffset = xprobe_xoffset;
      if (recdat.data[0] >= 32768) {
        xprobe_xoffset  = ((float)recdat.data[0] - 65536) / 100;
        xprobe_xoffset -= 0.001;
      }
      else {
        xprobe_xoffset  = ((float)recdat.data[0]) / 100;
        xprobe_xoffset += 0.001;
      }
      if (WITHIN((xprobe_xoffset), -100, 100))
        babystep.add_mm(X_AXIS, xprobe_xoffset - last_xoffset);
        //SERIAL_ECHO_MSG("babystep.add_mm():", xprobe_xoffset - last_xoffset);
      probe.offset.x = xprobe_xoffset;
      RTS_SndData(xprobe_xoffset * 100, HOTEND_X_ZOFFSET_VP);
      hal.watchdog_refresh();
      break;

    case YoffsetEnterKey:
      last_yoffset = yprobe_yoffset;
      if (recdat.data[0] >= 32768) {
        yprobe_yoffset  = ((float)recdat.data[0] - 65536) / 100;
        yprobe_yoffset -= 0.001;
      }
      else {
        yprobe_yoffset  = ((float)recdat.data[0]) / 100;
        yprobe_yoffset += 0.001;
      }
      if (WITHIN((yprobe_yoffset), -100, 100))
        babystep.add_mm(Y_AXIS, yprobe_yoffset - last_yoffset);
        //SERIAL_ECHO_MSG("babystep.add_mm():", yprobe_yoffset - last_yoffset);
      probe.offset.y = yprobe_yoffset;
      RTS_SndData(yprobe_yoffset * 100, HOTEND_Y_ZOFFSET_VP);
      hal.watchdog_refresh();
      break;

    case TempControlKey:
      if (recdat.data[0] == 2) {
        RTS_SndData(ExchangePageBase + 20, ExchangepageAddr);
        change_page_font = 20;
      }
      else if (recdat.data[0] == 3) {
        preheat_flag        = PREHEAT_PLA;
        temp_preheat_nozzle = ui.material_preset[0].hotend_temp;
        temp_preheat_bed    = ui.material_preset[0].bed_temp;

        RTS_SndData(ui.material_preset[0].hotend_temp, PREHEAT_PLA_SET_NOZZLE_TEMP_VP);
        RTS_SndData(ui.material_preset[0].bed_temp, PREHEAT_PLA_SET_BED_TEMP_VP);
        delay(2);
        RTS_SndData(ExchangePageBase + 22, ExchangepageAddr);
        change_page_font = 22;
      }
      else if (recdat.data[0] == 4) {
        preheat_flag        = PREHEAT_ABS;
        temp_preheat_nozzle = ui.material_preset[1].hotend_temp;
        temp_preheat_bed    = ui.material_preset[1].bed_temp;

        RTS_SndData(ui.material_preset[1].hotend_temp, PREHEAT_ABS_SET_NOZZLE_TEMP_VP);
        RTS_SndData(ui.material_preset[1].bed_temp, PREHEAT_ABS_SET_BED_TEMP_VP);
        delay(2);
        RTS_SndData(ExchangePageBase + 23, ExchangepageAddr);
        change_page_font = 23;
      }
      else if (recdat.data[0] == 5) {
        thermalManager.temp_hotend[0].target = ui.material_preset[0].hotend_temp;
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
        thermalManager.temp_bed.target = ui.material_preset[0].bed_temp;
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
      }
      else if (recdat.data[0] == 6) {
        thermalManager.temp_hotend[0].target = ui.material_preset[1].hotend_temp;
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
        thermalManager.temp_bed.target = ui.material_preset[1].bed_temp;
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
      }
      else if (recdat.data[0] == 7) {
        RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
        change_page_font = 21;
      }
      else if (recdat.data[0] == 8) {
        RTS_SndData(ExchangePageBase + 20, ExchangepageAddr);
        change_page_font = 20;
      }
      else if (recdat.data[0] == 161) {
        preheat_flag        = PREHEAT_PETG;
        temp_preheat_nozzle = ui.material_preset[2].hotend_temp;
        temp_preheat_bed    = ui.material_preset[2].bed_temp;

        RTS_SndData(ui.material_preset[2].hotend_temp, PREHEAT_PETG_SET_NOZZLE_TEMP_VP);
        RTS_SndData(ui.material_preset[2].bed_temp, PREHEAT_PETG_SET_BED_TEMP_VP);
        delay(2);
        RTS_SndData(ExchangePageBase + 90, ExchangepageAddr);
        change_page_font = 90;
      }
      else if (recdat.data[0] == 162) {
        preheat_flag        = PREHEAT_CUST;
        temp_preheat_nozzle = ui.material_preset[3].hotend_temp;
        temp_preheat_bed    = ui.material_preset[3].bed_temp;

        RTS_SndData(ui.material_preset[3].hotend_temp, PREHEAT_CUST_SET_NOZZLE_TEMP_VP);
        RTS_SndData(ui.material_preset[3].bed_temp, PREHEAT_CUST_SET_BED_TEMP_VP);
        delay(2);
        RTS_SndData(ExchangePageBase + 91, ExchangepageAddr);
        change_page_font = 91;
      }
      else if (recdat.data[0] == 163) {
        thermalManager.temp_hotend[0].target = ui.material_preset[2].hotend_temp;
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
        thermalManager.temp_bed.target = ui.material_preset[2].bed_temp;
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
      }
      else if (recdat.data[0] == 164) {
        thermalManager.temp_hotend[0].target = ui.material_preset[3].hotend_temp;
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
        thermalManager.temp_bed.target = ui.material_preset[3].bed_temp;
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
      }
      break;

    case CoolDownKey:
      if (recdat.data[0] == 1) {
        thermalManager.setTargetHotend(0, 0);
        RTS_SndData(0, HEAD_SET_TEMP_VP);
        thermalManager.setTargetBed(0);
        RTS_SndData(0, BED_SET_TEMP_VP);
        thermalManager.fan_speed[0] = 255;
        //RTS_SndData(0, PRINTER_FANOPEN_TITLE_VP);
      }
      else if (recdat.data[0] == 2) {
        RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
        change_page_font = 21;
      }
      else if (recdat.data[0] == 3) {
        if (g_soundSetOffOn == 2) {
          g_soundSetOffOn = 1;
          RTS_SndData(DC_SOUND_SET_ON, DC_SOUND_SET_DDR);
          RTS_SndData(101, SOUND_SETTING_OFF_ON_VP);
        }
        else {
          g_soundSetOffOn = 2;
          RTS_SndData(DC_SOUND_SET_OFF, DC_SOUND_SET_DDR);
          RTS_SndData(102, SOUND_SETTING_OFF_ON_VP);
        }
        settings.save();
      }
      else if (recdat.data[0] == 4) {
        RTS_SndData(ExchangePageBase + 48, ExchangepageAddr);
        g_uiAutoPIDFlag = true;
        thermalManager.setTargetBed(0);
        thermalManager.setTargetHotend(0, 0);
        last_target_temperature[0]  = thermalManager.temp_hotend[0].target;
        last_target_temperature_bed = thermalManager.temp_bed.target;
        RTS_SndData(g_autoPIDHeaterTempTarget, HEAD_SET_TEMP_VP);
        RTS_SndData(g_autoPIDHotBedTempTarget, BED_SET_TEMP_VP);
      }
      break;

    case HeaterTempEnterKey:
      if (false == g_uiAutoPIDFlag) {
        temphot = recdat.data[0];
        thermalManager.setTargetHotend(temphot, 0);
        RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
      }
      else { // è‡ªåŠ¨PID
        if ((g_uiAutoPIDNozzleRuningFlag == true) || (recdat.data[0] < AUTO_PID_NOZZLE_TARGET_TEMP_MIN)) {
          RTS_SndData(g_autoPIDHeaterTempTargetset, HEAD_SET_TEMP_VP);
          break;
        }
        g_autoPIDHeaterTempTargetset = recdat.data[0];
        RTS_SndData(g_autoPIDHeaterTempTargetset, HEAD_SET_TEMP_VP);
      }
      break;

    case HotBedTempEnterKey:
      if (false == g_uiAutoPIDFlag) {
        tempbed          = recdat.data[0];
        temp_bed_display = recdat.data[0];
        #if ENABLED(BED_TEMP_COMP)
          if (tempbed > 60 && tempbed <= 80)
            tempbed += 5;
          else if (tempbed > 80 && tempbed <= 120)
            tempbed += 7;
        #endif
        thermalManager.setTargetBed(tempbed);
        RTS_SndData(temp_bed_display, BED_SET_TEMP_VP);
      }
      else { // è‡ªåŠ¨PID
        if ((g_uiAutoPIDHotbedRuningFlag == true) || (recdat.data[0] < AUTO_PID_HOTBED_TARGET_TEMP_MIN)) {
          RTS_SndData(g_autoPIDHotBedTempTargetset, BED_SET_TEMP_VP);
          break;
        }
        tempbed                      = 0;
        g_autoPIDHotBedTempTargetset = recdat.data[0];
        RTS_SndData(g_autoPIDHotBedTempTargetset, BED_SET_TEMP_VP);
      }
      break;

    case PrepareEnterKey:
      if (recdat.data[0] == 1) {
        RTS_SndData(ExchangePageBase + 28, ExchangepageAddr);
        change_page_font = 28;
      }
      else if (recdat.data[0] == 2) {
        // jail
        if (g_uiAutoPIDNozzleRuningFlag == true) break;
        if (g_uiAutoPIDHotbedRuningFlag == true) break;
        RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
        change_page_font = 33;
      }
      else if (recdat.data[0] == 3) {
        rtscheck.RTS_SndData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
        rtscheck.RTS_SndData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
        rtscheck.RTS_SndData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
        delay(2);
        RTS_SndData(ExchangePageBase + 16, ExchangepageAddr);
        change_page_font = 16;
      }
      else if (recdat.data[0] == 5) {
        RTS_SndData(MACHINE_TYPE, MACHINE_TYPE_ABOUT_TEXT_VP);
        RTS_SndData(FIRMWARE_VERSION, FIRMWARE_VERSION_ABOUT_TEXT_VP);
        RTS_SndData(HARDWARE_VERSION, HARDWARE_VERSION_ABOUT_TEXT_VP);
        RTS_SndData(PRINT_SIZE, PRINTER_PRINTSIZE_TEXT_VP);
        delay(5);
        if (1 == lang)
          RTS_SndData(CORP_WEBSITE_C, WEBSITE_ABOUT_TEXT_VP);
        else
          RTS_SndData(CORP_WEBSITE_E, WEBSITE_ABOUT_TEXT_VP);
        RTS_SndData(ExchangePageBase + 24, ExchangepageAddr);
        change_page_font = 24;
      }
      else if (recdat.data[0] == 6) {
        queue.enqueue_now_P(PSTR("M84"));
        queue.enqueue_now_P(PSTR("G92.9Z0"));
        RTS_SndData(1, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 7) {
        RTS_SndData(ExchangePageBase + 43, ExchangepageAddr);
        change_page_font = 43;
      }
      else if (recdat.data[0] == 8) {
        ui.material_preset[preheat_flag].hotend_temp = temp_preheat_nozzle;
        ui.material_preset[preheat_flag].bed_temp    = temp_preheat_bed;

        settings.save();
        RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
        change_page_font = 21;
      }
      else if (recdat.data[0] == 9) {
        RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        #if ENABLED(GCODE_PREVIEW_ENABLED)
          gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
        #endif
        change_page_font = 1;
      }
      else if (recdat.data[0] == 0xA) {
        RTS_SndData(ExchangePageBase + 42, ExchangepageAddr);
        change_page_font = 42;
      }
      else if (recdat.data[0] == 0xB) {
        #if ENABLED(HAS_MENU_RESET_WIFI)
          WIFI_STATE = PRESSED;
          OUT_WRITE(RESET_WIFI_PIN, LOW);
        #endif
        ()settings.reset();
        ()settings.save();
        RTS_Init();
        RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        change_page_font = 1;
      }
      else if (recdat.data[0] == 0xC) {
        RTS_SndData(ExchangePageBase + 44, ExchangepageAddr);
        change_page_font = 44;
      }
      else if (recdat.data[0] == 0xD) {
        settings.reset();
        settings.save();
        RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
        change_page_font = 33;
      }
      else if (recdat.data[0] == 0xE) {
        if (!planner.has_blocks_queued()) {
          RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
          change_page_font = 33;
        }
      }
      else if (recdat.data[0] == 0xF) {
        RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
        change_page_font = 21;
        settings.save(); delay(100);
      }
      else if (recdat.data[0] == 0x10) {
        RTS_SndData(ExchangePageBase + 25, ExchangepageAddr);
        change_page_font = 25;
      }
      else if (recdat.data[0] == 0x11) {
        RTS_SndData(ExchangePageBase + 21, ExchangepageAddr);
        change_page_font = 21;
      }
      break;

    case BedLevelKey:
      if (recdat.data[0] == 1) {
        planner.synchronize();
        waitway      = 6;
        old_leveling = 1;
        RTS_SndData(ExchangePageBase + 26, ExchangepageAddr);
        change_page_font = 26;
        queue.enqueue_now_P(PSTR("G28"));
        rtscheck.RTS_SndData(0, AUTO_LEVELING_PERCENT_DATA_VP);
        Update_Time_Value = 0;
      }
      else if (recdat.data[0] == 2) {
        last_zoffset = zprobe_zoffset;
        //SERIAL_ECHOLNPGM("ZoffsetEnterKey offset +0.01 [", zprobe_zoffset, "] last [", last_zoffset, "] sent [", (zprobe_zoffset - last_zoffset), "] recdata [", recdat.data[0], "]");
        float rec_zoffset = 0;
        if (recdat.data[0] >= 32768)
          rec_zoffset = ((float)recdat.data[0] - 65536) / 100;
        else
          rec_zoffset = ((float)recdat.data[0]) / 100;
        if (WITHIN((zprobe_zoffset + 0.01), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
          #if ENABLED(HAS_LEVELING)
            if (rec_zoffset > last_zoffset) {
              zprobe_zoffset  = last_zoffset;
              zprobe_zoffset += 0.01;
            }
          #endif
          //SERIAL_ECHOLNPGM("BedLevelKey Z UP increment 0.01 [", zprobe_zoffset, "] last [", last_zoffset, "] sent [", (zprobe_zoffset - last_zoffset), "]");
          babystep.add_mm(Z_AXIS, zprobe_zoffset - last_zoffset);
          probe.offset.z = zprobe_zoffset;
        }
        RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      }
      else if (recdat.data[0] == 3) {
        last_zoffset = zprobe_zoffset;
        //SERIAL_ECHOLNPGM("ZoffsetEnterKey offset -0.01 [", zprobe_zoffset, "] last [", last_zoffset, "] sent [", (zprobe_zoffset - last_zoffset), "] recdata [", recdat.data[0], "]");
        float rec_zoffset = 0;
        if (recdat.data[0] >= 32768)
          rec_zoffset = ((float)recdat.data[0] - 65536) / 100;
        else
          rec_zoffset = ((float)recdat.data[0]) / 100;
        if (WITHIN((zprobe_zoffset - 0.01), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
          #if ENABLED(HAS_LEVELING)
            if (rec_zoffset > last_zoffset) {
              zprobe_zoffset  = last_zoffset;
              zprobe_zoffset -= 0.01;
            }
          #endif
          //SERIAL_ECHOLNPGM("BedLevelKey Z DOWN increment 0.01 [", zprobe_zoffset, "] last [", last_zoffset, "] sent [", (zprobe_zoffset - last_zoffset), "]");
          babystep.add_mm(Z_AXIS, zprobe_zoffset - last_zoffset);
          probe.offset.z = zprobe_zoffset;
        }
        RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      }
      else if (recdat.data[0] == 4) {
        if (!planner.has_blocks_queued()) {
          bltouch_tramming = 0;
          RTS_SndData(ExchangePageBase + 25, ExchangepageAddr);
          change_page_font = 25;
        }
      }
      else if (recdat.data[0] == 5) {
        char cmd[23];

        // Assitant Level ,  Centre 1
        if (!planner.has_blocks_queued()) {
          waitway = 4;
          if (bltouch_tramming == 0) {
            queue.enqueue_now_P(PSTR("G1 F600 Z3"));
            #if ENABLED(ENDER_3S1_PRO) || ENABLED(ENDER_3S1)
              sprintf_P(cmd, "G1 X117.5 Y117.5 F2000");
            #elif ENABLED(ENDER_3S1_PLUS)
              sprintf_P(cmd, "G1 X155 Y157.5 F2000");
            #else
              sprintf_P(cmd, "G1 X%d Y%d F2000", manual_level_5position[0][0], manual_level_5position[0][1]);
            #endif
            queue.enqueue_now_P(cmd);
            #if ENABLED(ENDER_3S1_PRO) || ENABLED(ENDER_3S1)
              rtscheck.RTS_SndData((unsigned char)10 * (unsigned char)117.5, AXIS_X_COORD_VP);
              rtscheck.RTS_SndData((unsigned char)10 * (unsigned char)117.5, AXIS_Y_COORD_VP);
            #elif ENABLED(ENDER_3S1_PLUS)
              rtscheck.RTS_SndData((unsigned char)10 * (unsigned char)155, AXIS_X_COORD_VP);
              rtscheck.RTS_SndData((unsigned char)10 * (unsigned char)157.5, AXIS_Y_COORD_VP);
            #else
              rtscheck.RTS_SndData(10 * manual_level_5position[0][0], AXIS_X_COORD_VP);
              rtscheck.RTS_SndData(10 * manual_level_5position[0][1], AXIS_Y_COORD_VP);
            #endif
            queue.enqueue_now_P(PSTR("G1 F600 Z0"));
            rtscheck.RTS_SndData(10 * 0, AXIS_Z_COORD_VP);
          }
          if (bltouch_tramming == 1) {
            #if ENABLED(ENDER_3S1_PRO) || ENABLED(ENDER_3S1)
              queue.enqueue_now_P(PSTR("G30 X117.5 Y117.5"));
            #elif ENABLED(ENDER_3S1_PLUS)
              queue.enqueue_now_P(PSTR("G30 X155 Y157.5"));
            #else
              sprintf_P(cmd, "G30 X%d Y%d", manual_level_5position[0][0], manual_level_5position[0][1]);
              queue.enqueue_now_P(cmd);
            #endif
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
            change_page_font = 89;
          }
          waitway = 0;
        }
      }
      else if (recdat.data[0] == 6) {
        char cmd[20];
        // Assitant Level , Front Left 2
        if (!planner.has_blocks_queued()) {

          waitway = 4;
          if (bltouch_tramming == 0) {
            queue.enqueue_now_P(PSTR("G1 F600 Z3"));
            sprintf_P(cmd, "G1 X%d Y%d F3000", manual_level_5position[1][0], manual_level_5position[1][1]);
            queue.enqueue_now_P(cmd);
            rtscheck.RTS_SndData(10 * manual_level_5position[1][0], AXIS_X_COORD_VP);
            rtscheck.RTS_SndData(10 * manual_level_5position[1][1], AXIS_Y_COORD_VP);
            queue.enqueue_now_P(PSTR("G1 F600 Z0"));
            rtscheck.RTS_SndData(10 * 0, AXIS_Z_COORD_VP);
          }
          if (bltouch_tramming == 1) {
            sprintf_P(cmd, "G30 X%d Y%d", manual_level_5position[1][0], manual_level_5position[1][1]);
            queue.enqueue_now_P(cmd);
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
            change_page_font = 89;
          }
          waitway = 0;
        }
      }
      else if (recdat.data[0] == 7) {
        char cmd[20];
        // Assitant Level , Front Right 3
        if (!planner.has_blocks_queued()) {
          waitway = 4;
          if (bltouch_tramming == 0) {
            queue.enqueue_now_P(PSTR("G1 F600 Z3"));
            sprintf_P(cmd, "G1 X%d Y%d F3000", manual_level_5position[2][0], manual_level_5position[2][1]);
            queue.enqueue_now_P(cmd);
            rtscheck.RTS_SndData(10 * manual_level_5position[2][0], AXIS_X_COORD_VP);
            rtscheck.RTS_SndData(10 * manual_level_5position[2][1], AXIS_Y_COORD_VP);
            queue.enqueue_now_P(PSTR("G1 F600 Z0"));
            rtscheck.RTS_SndData(10 * 0, AXIS_Z_COORD_VP);
          }
          if (bltouch_tramming == 1) {
            sprintf_P(cmd, "G30 X%d Y%d", manual_level_5position[2][0], manual_level_5position[2][1]);
            queue.enqueue_now_P(cmd);
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
            change_page_font = 89;
          }
          waitway = 0;
        }
      }
      else if (recdat.data[0] == 8) {
        char cmd[20];
        // Assitant Level , Back Right 4
        if (!planner.has_blocks_queued()) {
          waitway = 4;
          if (bltouch_tramming == 0) {
            queue.enqueue_now_P(PSTR("G1 F600 Z3"));
            sprintf_P(cmd, "G1 X%d Y%d F3000", manual_level_5position[3][0], manual_level_5position[3][1]);
            queue.enqueue_now_P(cmd);
            rtscheck.RTS_SndData(10 * manual_level_5position[3][0], AXIS_X_COORD_VP);
            rtscheck.RTS_SndData(10 * manual_level_5position[3][1], AXIS_Y_COORD_VP);

            queue.enqueue_now_P(PSTR("G1 F600 Z0"));
            rtscheck.RTS_SndData(10 * 0, AXIS_Z_COORD_VP);
          }
          if (bltouch_tramming == 1) {
            sprintf_P(cmd, "G30 X%d Y%d", manual_level_5position[3][0], manual_level_5position[3][1]);
            queue.enqueue_now_P(cmd);
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
            change_page_font = 89;
          }
          waitway = 0;
        }
      }
      else if (recdat.data[0] == 9) {
        char cmd[20];
        // Assitant Level , Back Left 5
        if (!planner.has_blocks_queued()) {

          waitway = 4;
          if (bltouch_tramming == 0) {
            queue.enqueue_now_P(PSTR("G1 F600 Z3"));
            sprintf_P(cmd, "G1 X%d Y%d F3000", manual_level_5position[4][0], manual_level_5position[4][1]);
            queue.enqueue_now_P(cmd);
            rtscheck.RTS_SndData(10 * manual_level_5position[4][0], AXIS_X_COORD_VP);
            rtscheck.RTS_SndData(10 * manual_level_5position[4][1], AXIS_Y_COORD_VP);

            queue.enqueue_now_P(PSTR("G1 F600 Z0"));
            rtscheck.RTS_SndData(10 * 0, AXIS_Z_COORD_VP);
          }
          if (bltouch_tramming == 1) {
            sprintf_P(cmd, "G30 X%d Y%d", manual_level_5position[4][0], manual_level_5position[4][1]);
            queue.enqueue_now_P(cmd);
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
            change_page_font = 89;
          }

          waitway = 0;
        }
      }
      else if (recdat.data[0] == 0x0B) {
        char cmd[20];
        // Assitant Level , Back Left 6
        if (!planner.has_blocks_queued()) {

          if (bltouch_tramming == 0) {
            waitway = 4;
            queue.enqueue_now_P(PSTR("G1 F600 Z3"));
            sprintf_P(cmd, "G1 X%d Y%d F3000", manual_level_5position[5][0], manual_level_5position[5][1]);
            queue.enqueue_now_P(cmd);
            rtscheck.RTS_SndData(10 * manual_level_5position[5][0], AXIS_X_COORD_VP);
            rtscheck.RTS_SndData(10 * manual_level_5position[5][1], AXIS_Y_COORD_VP);

            queue.enqueue_now_P(PSTR("G1 F600 Z0"));
            rtscheck.RTS_SndData(10 * 0, AXIS_Z_COORD_VP);
          }

          if (bltouch_tramming == 1) {
            sprintf_P(cmd, "G30 X%d Y%d", manual_level_5position[5][0], manual_level_5position[5][1]);
            queue.enqueue_now_P(cmd);
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
            change_page_font = 89;
          }
          waitway = 0;
        }
      }
      else if (recdat.data[0] == 0x0C) {
        char cmd[20];
        // Assitant Level , Back Left 7
        if (!planner.has_blocks_queued()) {
          if (bltouch_tramming == 0) {
            waitway = 4;
            queue.enqueue_now_P(PSTR("G1 F600 Z3"));
            sprintf_P(cmd, "G1 X%d Y%d F3000", manual_level_5position[6][0], manual_level_5position[6][1]);
            queue.enqueue_now_P(cmd);
            rtscheck.RTS_SndData(10 * manual_level_5position[6][0], AXIS_X_COORD_VP);
            rtscheck.RTS_SndData(10 * manual_level_5position[6][1], AXIS_Y_COORD_VP);

            queue.enqueue_now_P(PSTR("G1 F600 Z0"));
            rtscheck.RTS_SndData(10 * 0, AXIS_Z_COORD_VP);
          }

          if (bltouch_tramming == 1) {
            sprintf_P(cmd, "G30 X%d Y%d", manual_level_5position[6][0], manual_level_5position[6][1]);
            queue.enqueue_now_P(cmd);
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
            change_page_font = 89;
          }
          waitway = 0;
        }
      }
      else if (recdat.data[0] == 0x0D) {
        char cmd[20];
        // Assitant Level , Back Left 8
        if (!planner.has_blocks_queued()) {
          if (bltouch_tramming == 0) {
            waitway = 4;
            queue.enqueue_now_P(PSTR("G1 F600 Z3"));
            sprintf_P(cmd, "G1 X%d Y%d F3000", manual_level_5position[7][0], manual_level_5position[7][1]);
            queue.enqueue_now_P(cmd);
            rtscheck.RTS_SndData(10 * manual_level_5position[7][0], AXIS_X_COORD_VP);
            rtscheck.RTS_SndData(10 * manual_level_5position[7][1], AXIS_Y_COORD_VP);

            queue.enqueue_now_P(PSTR("G1 F600 Z0"));
            rtscheck.RTS_SndData(10 * 0, AXIS_Z_COORD_VP);
          }

          if (bltouch_tramming == 1) {
            sprintf_P(cmd, "G30 X%d Y%d", manual_level_5position[7][0], manual_level_5position[7][1]);
            queue.enqueue_now_P(cmd);
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
            change_page_font = 89;
          }
          waitway = 0;
        }
      }
      else if (recdat.data[0] == 0x0E) {
        char cmd[20];
        // Assitant Level , Back Left 9
        if (!planner.has_blocks_queued()) {
          if (bltouch_tramming == 0) {
            waitway = 4;
            queue.enqueue_now_P(PSTR("G1 F600 Z3"));
            sprintf_P(cmd, "G1 X%d Y%d F3000", manual_level_5position[8][0], manual_level_5position[8][1]);
            queue.enqueue_now_P(cmd);
            rtscheck.RTS_SndData(10 * manual_level_5position[8][0], AXIS_X_COORD_VP);
            rtscheck.RTS_SndData(10 * manual_level_5position[8][1], AXIS_Y_COORD_VP);

            queue.enqueue_now_P(PSTR("G1 F600 Z0"));
            rtscheck.RTS_SndData(10 * 0, AXIS_Z_COORD_VP);
          }

          if (bltouch_tramming == 1) {
            sprintf_P(cmd, "G30 X%d Y%d", manual_level_5position[8][0], manual_level_5position[8][1]);
            queue.enqueue_now_P(cmd);
            RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
            change_page_font = 89;
          }
          waitway = 0;
        }
      }
      else if (recdat.data[0] == 0x0A) {
        if (!planner.has_blocks_queued()) {
          RTS_SndData(ExchangePageBase + 26, ExchangepageAddr);
          change_page_font = 26;
        }
      }
      else if (recdat.data[0] == 161) { // 00A1
        last_zoffset = zprobe_zoffset;
        //SERIAL_ECHOLNPGM("ZoffsetEnterKey offset +0.01 [", zprobe_zoffset, "] last [", last_zoffset, "] sent [", (zprobe_zoffset - last_zoffset), "] recdata [", recdat.data[0], "]");
        float rec_zoffset = 0;
        if (recdat.data[0] >= 32768)
          rec_zoffset = ((float)recdat.data[0] - 65536) / 100;
        else
          rec_zoffset = ((float)recdat.data[0]) / 100;
        if (WITHIN((zprobe_zoffset + 0.05), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
          #if ENABLED(HAS_LEVELING)
            if (rec_zoffset > last_zoffset) {
              zprobe_zoffset  = last_zoffset;
              zprobe_zoffset += 0.05;
            }
          #endif
          //SERIAL_ECHOLNPGM("BedLevelKey Z UP increment 0.01 [", zprobe_zoffset, "] last [", last_zoffset, "] sent [", (zprobe_zoffset - last_zoffset), "]");
          babystep.add_mm(Z_AXIS, zprobe_zoffset - last_zoffset);
          probe.offset.z = zprobe_zoffset;
        }
        RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      }
      else if (recdat.data[0] == 162) { // 00A2
        last_zoffset = zprobe_zoffset;
        //SERIAL_ECHOLNPGM("ZoffsetEnterKey offset -0.01 [", zprobe_zoffset, "] last [", last_zoffset, "] sent [", (zprobe_zoffset - last_zoffset), "] recdata [", recdat.data[0], "]");
        float rec_zoffset = 0;
        if (recdat.data[0] >= 32768)
          rec_zoffset = ((float)recdat.data[0] - 65536) / 100;
        else
          rec_zoffset = ((float)recdat.data[0]) / 100;
        if (WITHIN((zprobe_zoffset - 0.05), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
          #if ENABLED(HAS_LEVELING)
            if (rec_zoffset > last_zoffset) {
              zprobe_zoffset  = last_zoffset;
              zprobe_zoffset -= 0.05;
            }
          #endif
          //SERIAL_ECHOLNPGM("BedLevelKey Z DOWN increment 0.01 [", zprobe_zoffset, "] last [", last_zoffset, "] sent [", (zprobe_zoffset - last_zoffset), "]");
          babystep.add_mm(Z_AXIS, zprobe_zoffset - last_zoffset);
          probe.offset.z = zprobe_zoffset;
        }
        RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      }
      else if (recdat.data[0] == 163) { // 00A3
        #if ENABLED(BLTOUCH)
          old_leveling = 0;

          RTS_SndData(AutoHomeFirstPoint, AUTO_BED_LEVEL_CUR_POINT_VP);
          RTS_SndData(lang + 10, AUTO_LEVELING_START_TITLE_VP);
          RTS_SndData(GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y, AUTO_BED_LEVEL_END_POINT);

          if (axes_should_home()) {
            waitway = 15;
            queue.enqueue_one_P(PSTR("G28"));
            RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
            change_page_font = 40;
          }
          else {
            RTS_SndData(ExchangePageBase + 81, ExchangepageAddr);
            change_page_font = 81;
          }
          #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
            queue.enqueue_one_P(PSTR("G29"));
          #else
            touchscreen_requested_mesh = 1;
            queue.enqueue_one_P(PSTR("G29 P1 T"));
            // queue.enqueue_one_P(PSTR("G29 P3"));
            // queue.enqueue_one_P(PSTR("G29 S1"));
          #endif
        #endif
      }
      else if (recdat.data[0] == 164) { // 00A4

        old_leveling = 0;

        if (axes_should_home()) {
          waitway = 15;
          queue.enqueue_one_P(PSTR("G28"));
          RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
          change_page_font = 40;
        }
        else {
          RTS_SndData(ExchangePageBase + 81, ExchangepageAddr);
          change_page_font = 81;
        }

        rtscheck.RTS_SndData(AutoHomeFirstPoint, AUTO_BED_LEVEL_CUR_POINT_VP);
        RTS_SndData(lang, AUTO_LEVELING_START_TITLE_VP);
        rtscheck.RTS_SndData(GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y, AUTO_BED_LEVEL_END_POINT);
        rtscheck.RTS_SndData(0, AUTO_LEVELING_PERCENT_DATA_VP);
        Update_Time_Value = 0;
      }
      else if (recdat.data[0] == 165) { // 00A5
        bltouch_tramming = 1;
        RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
        change_page_font = 89;
      }
      else if (recdat.data[0] == 166) { // 00A6
        if (bltouch_tramming == 1) {
          // Point 6
          char cmd6[20];
          sprintf_P(cmd6, "G30 X%d Y%d", manual_level_5position[1][0], manual_level_5position[1][1]);
          queue.enqueue_now_P(cmd6);
          // Point 7
          char cmd7[20];
          sprintf_P(cmd7, "G30 X%d Y%d", manual_level_5position[2][0], manual_level_5position[2][1]);
          queue.enqueue_now_P(cmd7);
          // Point 8
          char cmd8[20];
          sprintf_P(cmd8, "G30 X%d Y%d", manual_level_5position[3][0], manual_level_5position[3][1]);
          queue.enqueue_now_P(cmd8);
          // Point 9
          char cmd9[20];
          sprintf_P(cmd9, "G30 X%d Y%d", manual_level_5position[4][0], manual_level_5position[4][1]);
          queue.enqueue_now_P(cmd9);
          // Finally center

          #if ENABLED(ENDER_3S1_PRO) || ENABLED(ENDER_3S1)
            queue.enqueue_now_P(PSTR("G30 X117.5 Y117.5"));
          #elif ENABLED(ENDER_3S1_PLUS)
            queue.enqueue_now_P(PSTR("G30 X155 Y157"));
          #else
            char cmd1[20];
            sprintf_P(cmd1, "G30 X%d Y%d", manual_level_5position[0][0], manual_level_5position[0][1]);
            queue.enqueue_now_P(cmd1);
          #endif
          RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
          change_page_font = 89;
        }
      }
      RTS_SndData(0, MOTOR_FREE_ICON_VP);
      break;

    case AutoHomeKey:
      if (recdat.data[0] == 1) {
        AxisUnitMode = 1;
        axis_unit    = 10.0;
        RTS_SndData(ExchangePageBase + 16, ExchangepageAddr);
        change_page_font = 16;
        RTS_SndData(3, MOVEAXIS_UNIT_ICON_VP);
      }
      else if (recdat.data[0] == 2) {
        AxisUnitMode = 2;
        axis_unit    = 1.0;
        RTS_SndData(ExchangePageBase + 17, ExchangepageAddr);
        change_page_font = 17;
        RTS_SndData(2, MOVEAXIS_UNIT_ICON_VP);
      }
      else if (recdat.data[0] == 3) {
        AxisUnitMode = 3;
        axis_unit    = 0.1;
        RTS_SndData(ExchangePageBase + 18, ExchangepageAddr);
        change_page_font = 18;
        RTS_SndData(1, MOVEAXIS_UNIT_ICON_VP);
      }
      else if (recdat.data[0] == 4) {
        waitway = 4;
        RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
        change_page_font = 40;

        queue.enqueue_now_P(PSTR("G28 X Y"));
        Update_Time_Value = 0;
        RTS_SndData(0, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 5) {
        waitway = 4;
        RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
        change_page_font = 40;

        queue.enqueue_now_P(PSTR("G28"));
        RTS_SndData(0, MOTOR_FREE_ICON_VP);
        Update_Time_Value = 0;
      }
      else if (recdat.data[0] == 161) { // 00A1
        AxisUnitMode = 1;
        axis_unit    = 10.0;
        RTS_SndData(ExchangePageBase + 86, ExchangepageAddr);
        change_page_font = 86;
        RTS_SndData(3, MOVEAXIS_UNIT_ICON_VP);
      }
      else if (recdat.data[0] == 162) { // 00A2
        AxisUnitMode = 2;
        axis_unit    = 1.0;
        RTS_SndData(ExchangePageBase + 87, ExchangepageAddr);
        change_page_font = 87;
        RTS_SndData(2, MOVEAXIS_UNIT_ICON_VP);
      }
      else if (recdat.data[0] == 163) { // 00A3
        AxisUnitMode = 3;
        axis_unit    = 0.1;
        RTS_SndData(ExchangePageBase + 88, ExchangepageAddr);
        change_page_font = 88;
        RTS_SndData(1, MOVEAXIS_UNIT_ICON_VP);
      }
      else if (recdat.data[0] == 164) { // 00A4
        waitway = 14;
        RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
        change_page_font = 40;

        queue.enqueue_now_P(PSTR("G28 X Y"));
        Update_Time_Value = 0;
        RTS_SndData(0, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 165) { // 00A5
        waitway = 14;
        RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
        change_page_font = 40;

        queue.enqueue_now_P(PSTR("G28"));
        RTS_SndData(0, MOTOR_FREE_ICON_VP);
        Update_Time_Value = 0;
      }
      else if (recdat.data[0] == 166) { // 00A6
        if (axes_should_home()) {
          waitway = 16;
          RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
          change_page_font = 40;
          queue.enqueue_now_P(PSTR("G28"));
          RTS_SndData(0, MOTOR_FREE_ICON_VP);
        }
        else {
          RTS_SndData(ExchangePageBase + 25, ExchangepageAddr);
          change_page_font = 25;
        }
        Update_Time_Value = 0;
      }
      else if (recdat.data[0] == 167) { // 00A7
        if (axes_should_home()) {
          waitway = 17;
          RTS_SndData(ExchangePageBase + 40, ExchangepageAddr);
          change_page_font = 40;
          queue.enqueue_now_P(PSTR("G28"));
          RTS_SndData(0, MOTOR_FREE_ICON_VP);
        }
        else {
          RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
          change_page_font = 89;
        }
        Update_Time_Value = 0;
      }
      break;

    case XaxismoveKey:
      float x_min, x_max;
      waitway                  = 4;
      x_min                    = 0;
      x_max                    = X_MAX_POS;
      current_position[X_AXIS] = ((float)recdat.data[0]) / 10;
      if (current_position[X_AXIS] < x_min)
        current_position[X_AXIS] = x_min;
      else if (current_position[X_AXIS] > x_max)
        current_position[X_AXIS] = x_max;
      RTS_line_to_current(X_AXIS);
      RTS_SndData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
      delay(1);
      RTS_SndData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
      break;

    case YaxismoveKey:
      float y_min, y_max;
      waitway                  = 4;
      y_min                    = 0;
      y_max                    = Y_MAX_POS;
      current_position[Y_AXIS] = ((float)recdat.data[0]) / 10;
      if (current_position[Y_AXIS] < y_min)
        current_position[Y_AXIS] = y_min;
      else if (current_position[Y_AXIS] > y_max)
        current_position[Y_AXIS] = y_max;
      RTS_line_to_current(Y_AXIS);
      RTS_SndData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
      delay(1);
      RTS_SndData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
      break;

    case ZaxismoveKey:
      float z_min, z_max;
      waitway                  = 4;
      z_min                    = Z_MIN_POS;
      z_max                    = Z_MAX_POS;
      current_position[Z_AXIS] = ((float)recdat.data[0]) / 10;
      if (current_position[Z_AXIS] < z_min)
        current_position[Z_AXIS] = z_min;
      else if (current_position[Z_AXIS] > z_max)
        current_position[Z_AXIS] = z_max;
      RTS_line_to_current(Z_AXIS);
      RTS_SndData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
      delay(1);
      RTS_SndData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
      break;

    case HeaterLoadEnterKey:
      FilamentLOAD = ((float)recdat.data[0]) / 10;
      RTS_SndData(10 * FilamentLOAD, HEAD_FILAMENT_LOAD_DATA_VP);
      if (!planner.has_blocks_queued()) {
        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          if (READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_STATE) {
            RTS_SndData(ExchangePageBase + 46, ExchangepageAddr);
            change_page_font = 46;
            break;
          }
        #endif
        current_position[E_AXIS] += FilamentLOAD;

        if ((thermalManager.temp_hotend[0].target > EXTRUDE_MINTEMP) && (thermalManager.temp_hotend[0].celsius < (thermalManager.temp_hotend[0].celsius - 5))) {
          thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
          RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
          // break;
        }
        else if ((thermalManager.temp_hotend[0].target < EXTRUDE_MINTEMP) && (thermalManager.temp_hotend[0].celsius < (ChangeFilamentTemp - 5))) {
          thermalManager.setTargetHotend(ChangeFilamentTemp, 0);
          RTS_SndData(ChangeFilamentTemp, HEAD_SET_TEMP_VP);
          // break;
        }

        while (ABS(thermalManager.degHotend(0) - thermalManager.degTargetHotend(0)) > TEMP_WINDOW) idle();

        // else
        {
          RTS_line_to_current(E_AXIS);
          RTS_SndData(10 * FilamentLOAD, HEAD_FILAMENT_LOAD_DATA_VP);
          planner.synchronize();
        }
      }
      break;

    case HeaterUnLoadEnterKey:
      FilamentUnLOAD = ((float)recdat.data[0]) / 10;
      RTS_SndData(10 * FilamentUnLOAD, HEAD_FILAMENT_UNLOAD_DATA_VP);
      if (!planner.has_blocks_queued()) {
        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          if (READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_STATE) {
            RTS_SndData(ExchangePageBase + 46, ExchangepageAddr);
            change_page_font = 46;
            break;
          }
        #endif

        current_position[E_AXIS] -= FilamentUnLOAD;

        if ((thermalManager.temp_hotend[0].target > EXTRUDE_MINTEMP) && (thermalManager.temp_hotend[0].celsius < (thermalManager.temp_hotend[0].celsius - 5))) {
          thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
          RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
          // break;
        }
        else if ((thermalManager.temp_hotend[0].target < EXTRUDE_MINTEMP) && (thermalManager.temp_hotend[0].celsius < (ChangeFilamentTemp - 5))) {
          thermalManager.setTargetHotend(ChangeFilamentTemp, 0);
          RTS_SndData(ChangeFilamentTemp, HEAD_SET_TEMP_VP);
          // break;
        }
        // else
        while (ABS(thermalManager.degHotend(0) - thermalManager.degTargetHotend(0)) > TEMP_WINDOW) idle();

        {
          RTS_line_to_current(E_AXIS);
          RTS_SndData(10 * FilamentUnLOAD, HEAD_FILAMENT_UNLOAD_DATA_VP);
          planner.synchronize();
        }
      }
      break;

    case HeaterLoadStartKey:
      if (recdat.data[0] == 1) {
        if (!planner.has_blocks_queued()) {
          #if ENABLED(FILAMENT_RUNOUT_SENSOR)
            if (READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_STATE) {
              RTS_SndData(ExchangePageBase + 46, ExchangepageAddr);
              change_page_font = 46;
              break;
            }
            else if (rts_start_print) {
              RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
              change_page_font = 1;
              break;
            }
          #endif

          if ((thermalManager.temp_hotend[0].target > EXTRUDE_MINTEMP) && (thermalManager.temp_hotend[0].celsius < (thermalManager.temp_hotend[0].celsius - 5))) {
            thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
            RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
            break;
          }
          else if ((thermalManager.temp_hotend[0].target < EXTRUDE_MINTEMP) && (thermalManager.temp_hotend[0].celsius < (ChangeFilamentTemp - 5))) {
            thermalManager.setTargetHotend(ChangeFilamentTemp, 0);
            RTS_SndData(ChangeFilamentTemp, HEAD_SET_TEMP_VP);
            break;
          }
          else {
            RTS_line_to_current(E_AXIS);
            planner.synchronize();
          }
          RTS_SndData(ExchangePageBase + 19, ExchangepageAddr);
          change_page_font = 19;
        }
      }
      else if (recdat.data[0] == 2) {
        if (!planner.has_blocks_queued()) {
          RTS_SndData(ExchangePageBase + 19, ExchangepageAddr);
          change_page_font = 19;
        }
      }
      else if (recdat.data[0] == 3) {
        RTS_SndData(ExchangePageBase + 19, ExchangepageAddr);
        change_page_font = 19;
      }
      break;

    case SelectLanguageKey:
      if (recdat.data[0] != 0)
        lang = recdat.data[0];
      language_change_font = lang;
      for (int i = 0; i < 9; i++) RTS_SndData(0, LANGUAGE_CHINESE_TITLE_VP + i);
      RTS_SndData(1, LANGUAGE_CHINESE_TITLE_VP + (language_change_font - 1));
      languagedisplayUpdate();
      // settings.save();
      break;

    case PowerContinuePrintKey:
      if (recdat.data[0] == 1) {

        #if ENABLED(POWER_LOSS_RECOVERY)
          if (recovery.info.recovery_flag) {
            power_off_type_yes = 1;
            Update_Time_Value  = 0;
            RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
            change_page_font = 10;

            #if ENABLED(GCODE_PREVIEW_ENABLED)
              gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, false);
              int32_t ret = gcodePicDataSendToDwin(recovery.info.sd_filename, VP_OVERLAY_PIC_PTINT, PIC_FORMAT_JPG, PIC_RESOLUTION_300_300);
              if (ret == PIC_OK)
                gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, false);
              else
                gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);

            #endif
            // recovery.resume();
            queue.enqueue_now_P(PSTR("M1000"));

            PoweroffContinue   = true;
            sdcard_pause_check = true;
            zprobe_zoffset     = probe.offset.z;
            RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
            RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
          }
        #endif
      }
      else if (recdat.data[0] == 2) {
        Update_Time_Value = RTS_UPDATE_VALUE;
        RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
        #if ENABLED(GCODE_PREVIEW_ENABLED)
          gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
        #endif
        change_page_font = 1;
        RTS_SndData(0, PRINT_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_TIME_MIN_VP);
        RTS_SndData(0, PRINT_REMAIN_TIME_HOUR_VP);
        RTS_SndData(0, PRINT_REMAIN_TIME_MIN_VP);

        Update_Time_Value = 0;
        RTS_SDcard_Stop();
      }
      break;

    case PLAHeadSetEnterKey:
      temp_preheat_nozzle = recdat.data[0];
      RTS_SndData(temp_preheat_nozzle, PREHEAT_PLA_SET_NOZZLE_TEMP_VP);
      break;

    case PLABedSetEnterKey:
      temp_preheat_bed = recdat.data[0];
      RTS_SndData(temp_preheat_bed, PREHEAT_PLA_SET_BED_TEMP_VP);
      break;

    case ABSHeadSetEnterKey:
      temp_preheat_nozzle = recdat.data[0];
      RTS_SndData(temp_preheat_nozzle, PREHEAT_ABS_SET_NOZZLE_TEMP_VP);
      break;

    case ABSBedSetEnterKey:
      temp_preheat_bed = recdat.data[0];
      RTS_SndData(temp_preheat_bed, PREHEAT_ABS_SET_BED_TEMP_VP);
      break;

    case PETGHeadSetEnterKey:
      temp_preheat_nozzle = recdat.data[0];
      RTS_SndData(temp_preheat_nozzle, PREHEAT_PETG_SET_NOZZLE_TEMP_VP);
      break;

    case PETGBedSetEnterKey:
      temp_preheat_bed = recdat.data[0];
      RTS_SndData(temp_preheat_bed, PREHEAT_PETG_SET_BED_TEMP_VP);
      break;

    case CUSTHeadSetEnterKey:
      temp_preheat_nozzle = recdat.data[0];
      RTS_SndData(temp_preheat_nozzle, PREHEAT_CUST_SET_NOZZLE_TEMP_VP);
      break;

    case CUSTBedSetEnterKey:
      temp_preheat_bed = recdat.data[0];
      RTS_SndData(temp_preheat_bed, PREHEAT_CUST_SET_BED_TEMP_VP);
      break;

    case StoreMemoryKey:
      if (recdat.data[0] == 1) {
        rtscheck.RTS_SndData(ExchangePageBase + 37, ExchangepageAddr);
        change_page_font = 37;
      }
      if (recdat.data[0] == 2) {
        queue.enqueue_now_P(PSTR("M502"));
        rtscheck.RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
        change_page_font = 33;
        settings.save();
        RTS_SndData(default_max_feedrate[X_AXIS], MAX_VELOCITY_XAXIS_DATA_VP);
        RTS_SndData(default_max_feedrate[Y_AXIS], MAX_VELOCITY_YAXIS_DATA_VP);
        RTS_SndData(default_max_feedrate[Z_AXIS], MAX_VELOCITY_ZAXIS_DATA_VP);
        RTS_SndData(default_max_feedrate[E_AXIS], MAX_VELOCITY_EAXIS_DATA_VP);
        // delay(20);

        RTS_SndData(default_max_acceleration[X_AXIS], MAX_ACCEL_XAXIS_DATA_VP);
        RTS_SndData(default_max_acceleration[Y_AXIS], MAX_ACCEL_YAXIS_DATA_VP);
        RTS_SndData(default_max_acceleration[Z_AXIS], MAX_ACCEL_ZAXIS_DATA_VP);
        RTS_SndData(default_max_acceleration[E_AXIS], MAX_ACCEL_EAXIS_DATA_VP);
        // delay(20);

        RTS_SndData(default_max_jerk[X_AXIS] * 100, MAX_JERK_XAXIS_DATA_VP);
        RTS_SndData(default_max_jerk[Y_AXIS] * 100, MAX_JERK_YAXIS_DATA_VP);
        RTS_SndData(default_max_jerk[Z_AXIS] * 100, MAX_JERK_ZAXIS_DATA_VP);
        RTS_SndData(default_max_jerk[E_AXIS] * 100, MAX_JERK_EAXIS_DATA_VP);
        // delay(20);

        RTS_SndData(default_axis_steps_per_unit[X_AXIS] * 10, MAX_STEPSMM_XAXIS_DATA_VP);
        RTS_SndData(default_axis_steps_per_unit[Y_AXIS] * 10, MAX_STEPSMM_YAXIS_DATA_VP);
        RTS_SndData(default_axis_steps_per_unit[Z_AXIS] * 10, MAX_STEPSMM_ZAXIS_DATA_VP);
        RTS_SndData(default_axis_steps_per_unit[E_AXIS] * 10, MAX_STEPSMM_EAXIS_DATA_VP);
        // delay(20);

        RTS_SndData(default_nozzle_ptemp * 100, NOZZLE_TEMP_P_DATA_VP);
        RTS_SndData(default_nozzle_itemp * 100, NOZZLE_TEMP_I_DATA_VP);
        RTS_SndData(default_nozzle_dtemp * 100, NOZZLE_TEMP_D_DATA_VP);
        delay(20);
        RTS_SndData(default_hotbed_ptemp * 100, HOTBED_TEMP_P_DATA_VP);
        RTS_SndData(default_hotbed_itemp * 100, HOTBED_TEMP_I_DATA_VP);
        RTS_SndData(default_hotbed_dtemp * 10, HOTBED_TEMP_D_DATA_VP);
        // delay(100);
      }
      else if (recdat.data[0] == 3) {
        rtscheck.RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
        change_page_font = 33;
      }
      else if (recdat.data[0] == 4) {
        RTS_SndData(planner.extruder_advance_K[0] * 100, ADVANCE_K_SET);
        rtscheck.RTS_SndData(ExchangePageBase + 34, ExchangepageAddr);
        change_page_font = 34;
        settings.save();
      }
      else if (recdat.data[0] == 5) {
        rtscheck.RTS_SndData(ExchangePageBase + 39, ExchangepageAddr);
        change_page_font = 39;
      }
      else if (recdat.data[0] == 7) {
        rtscheck.RTS_SndData(ExchangePageBase + 38, ExchangepageAddr);
        change_page_font = 38;
      }
      else if (recdat.data[0] == 8) {
        rtscheck.RTS_SndData(ExchangePageBase + 36, ExchangepageAddr);
        change_page_font = 36;
      }
      else if (recdat.data[0] == 9) {
        rtscheck.RTS_SndData(ExchangePageBase + 37, ExchangepageAddr);
        change_page_font = 37;
      }
      else if (recdat.data[0] == 0x0A) {
        rtscheck.RTS_SndData(ExchangePageBase + 35, ExchangepageAddr);
        change_page_font = 35;
      }
      else if (recdat.data[0] == 0x0B) {
        rtscheck.RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
        change_page_font = 33;
        settings.save();
        delay(100);
      }
      else if (recdat.data[0] == 0x0C) {
        rtscheck.RTS_SndData(ExchangePageBase + 34, ExchangepageAddr);
        change_page_font = 34;
        settings.save();
        delay(100);
      }
      else if (recdat.data[0] == 0x0D) {
        rtscheck.RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
        change_page_font = 33;
        // settings.save();
        // delay(100);
      }
      else if (recdat.data[0] == 161) { // 00A1
        g_autoPIDHeaterTempTargetset = g_autoPIDHeaterTempTargetset;
        if (g_autoPIDHeaterTempTargetset != 0)
          g_autoPIDHeaterTempTarget =  g_autoPIDHeaterTempTargetset;
        g_autoPIDHeaterCyclesTargetset = g_autoPIDHeaterCyclesTargetset;
        if (g_autoPIDHeaterCyclesTargetset != 0)
          g_autoPIDHeaterCycles =  g_autoPIDHeaterCyclesTargetset;
        char cmd[30];
        g_uiAutoPIDNozzleRuningFlag = true;
        g_uiAutoPIDRuningDiff       = 1;
        g_uiCurveDataCnt            = 0;
        RTS_SndData(lang + 10, AUTO_PID_START_NOZZLE_VP);
        RTS_SndData(0, AUTO_PID_NOZZLE_TIS_VP);
        RTS_SndData(lang, AUTO_PID_RUN_NOZZLE_TIS_VP);
        memset(cmd, 0, sizeof(cmd));
        sprintf_P(cmd, PSTR("M303 E0 C%d S%d"), g_autoPIDHeaterCycles, g_autoPIDHeaterTempTarget);
        gcode.process_subcommands_now(cmd);
        PID_PARAM(Kp, 0) = g_autoPID.p;
        PID_PARAM(Ki, 0) = scalePID_i(g_autoPID.i);
        PID_PARAM(Kd, 0) = scalePID_d(g_autoPID.d);
        RTS_SndData(lang + 10, AUTO_PID_RUN_NOZZLE_TIS_VP);
        RTS_SndData(PID_PARAM(Kp, 0) * 100, NOZZLE_TEMP_P_DATA_VP);
        RTS_SndData(unscalePID_i(PID_PARAM(Ki, 0)) * 100, NOZZLE_TEMP_I_DATA_VP);
        RTS_SndData(unscalePID_d(PID_PARAM(Kd, 0)) * 100, NOZZLE_TEMP_D_DATA_VP);
        hal.watchdog_refresh();
        settings.save();
        delay(1000);
        g_uiAutoPIDRuningDiff = 0;
        RTS_SndData(lang, AUTO_PID_START_NOZZLE_VP);
        g_uiAutoPIDNozzleRuningFlag = false;
      }
      else if (recdat.data[0] == 162) {
        g_autoPIDHotBedTempTargetset = g_autoPIDHotBedTempTargetset;
        if (g_autoPIDHotBedTempTargetset != 0)
          g_autoPIDHotBedTempTarget =  g_autoPIDHotBedTempTargetset;
        g_autoPIDHotBedCyclesTargetset = g_autoPIDHotBedCyclesTargetset;
        if (g_autoPIDHotBedCyclesTargetset != 0)
          g_autoPIDHotBedCycles =  g_autoPIDHotBedCyclesTargetset;
        g_uiAutoPIDHotbedRuningFlag = true;
        g_uiAutoPIDRuningDiff       = 2;
        g_uiCurveDataCnt            = 0;
        char cmd[30];
        RTS_SndData(lang + 10, AUTO_PID_START_HOTBED_VP);
        RTS_SndData(0, AUTO_PID_HOTBED_TIS_VP);
        RTS_SndData(lang, AUTO_PID_RUN_HOTBED_TIS_VP);
        memset(cmd, 0, sizeof(cmd));
        sprintf_P(cmd, PSTR("M303 E-1 C%d S%d"), g_autoPIDHotBedCycles, g_autoPIDHotBedTempTarget);
        gcode.process_subcommands_now(cmd);
        thermalManager.temp_bed.pid.Kp = g_autoPID.p;
        thermalManager.temp_bed.pid.Ki = scalePID_i(g_autoPID.i);
        thermalManager.temp_bed.pid.Kd = scalePID_d(g_autoPID.d);
        RTS_SndData(lang + 10, AUTO_PID_RUN_HOTBED_TIS_VP);
        RTS_SndData(thermalManager.temp_bed.pid.Kp * 100, HOTBED_TEMP_P_DATA_VP);
        RTS_SndData(unscalePID_i(thermalManager.temp_bed.pid.Ki) * 100, HOTBED_TEMP_I_DATA_VP);
        RTS_SndData(unscalePID_d(thermalManager.temp_bed.pid.Kd) * 10, HOTBED_TEMP_D_DATA_VP);
        hal.watchdog_refresh();
        settings.save();
        delay(1000);
        g_uiAutoPIDRuningDiff = 0;
        RTS_SndData(lang, AUTO_PID_START_HOTBED_VP);
        g_uiAutoPIDHotbedRuningFlag = false;
      }
      else if (recdat.data[0] == 163) { // 00A3
                                        // Jail
        if (g_uiAutoPIDNozzleRuningFlag == true) break;
        if (g_uiAutoPIDHotbedRuningFlag == true) break;
        rtscheck.RTS_SndData(ExchangePageBase + 85, ExchangepageAddr);
        change_page_font = 85;
      }
      else if (recdat.data[0] == 164) { // Hotend Offsets 00A4
        rtscheck.RTS_SndData(ExchangePageBase + 86, ExchangepageAddr);
        change_page_font = 86;
      }
      break;

    case FanSpeedEnterKey:
      thermalManager.fan_speed[0] = recdat.data[0];
      RTS_SndData(thermalManager.fan_speed[0], FAN_SPEED_CONTROL_DATA_VP);
      break;

    case VelocityXaxisEnterKey:
      float velocity_xaxis;
      velocity_xaxis = planner.settings.max_feedrate_mm_s[0];
      velocity_xaxis = recdat.data[0];
      RTS_SndData(velocity_xaxis, MAX_VELOCITY_XAXIS_DATA_VP);
      planner.set_max_feedrate(X_AXIS, velocity_xaxis);
      break;

    case VelocityYaxisEnterKey:
      float velocity_yaxis;
      velocity_yaxis = planner.settings.max_feedrate_mm_s[1];
      velocity_yaxis = recdat.data[0];
      RTS_SndData(velocity_yaxis, MAX_VELOCITY_YAXIS_DATA_VP);
      planner.set_max_feedrate(Y_AXIS, velocity_yaxis);
      break;

    case VelocityZaxisEnterKey:
      float velocity_zaxis;
      velocity_zaxis = planner.settings.max_feedrate_mm_s[2];
      velocity_zaxis = recdat.data[0];
      RTS_SndData(velocity_zaxis, MAX_VELOCITY_ZAXIS_DATA_VP);
      planner.set_max_feedrate(Z_AXIS, velocity_zaxis);
      break;

    case VelocityEaxisEnterKey:
      float velocity_eaxis;
      velocity_eaxis = planner.settings.max_feedrate_mm_s[3];
      velocity_eaxis = recdat.data[0];
      RTS_SndData(velocity_eaxis, MAX_VELOCITY_EAXIS_DATA_VP);
      planner.set_max_feedrate(E_AXIS, velocity_eaxis);
      break;

    case AccelXaxisEnterKey:
      float accel_xaxis;
      accel_xaxis = planner.settings.max_acceleration_mm_per_s2[0];
      accel_xaxis = recdat.data[0];
      RTS_SndData(accel_xaxis, MAX_ACCEL_XAXIS_DATA_VP);
      planner.set_max_acceleration(X_AXIS, accel_xaxis);
      break;

    case AccelYaxisEnterKey:
      float accel_yaxis;
      accel_yaxis = planner.settings.max_acceleration_mm_per_s2[1];
      accel_yaxis = recdat.data[0];
      RTS_SndData(accel_yaxis, MAX_ACCEL_YAXIS_DATA_VP);
      planner.set_max_acceleration(Y_AXIS, accel_yaxis);
      break;

    case AccelZaxisEnterKey:
      float accel_zaxis;
      accel_zaxis = planner.settings.max_acceleration_mm_per_s2[2];
      accel_zaxis = recdat.data[0];
      RTS_SndData(accel_zaxis, MAX_ACCEL_ZAXIS_DATA_VP);
      planner.set_max_acceleration(Z_AXIS, accel_zaxis);
      break;

    case AccelEaxisEnterKey:
      float accel_eaxis;
      accel_eaxis = planner.settings.max_acceleration_mm_per_s2[3];
      accel_eaxis = recdat.data[0];
      RTS_SndData(accel_eaxis, MAX_ACCEL_EAXIS_DATA_VP);
      planner.set_max_acceleration(E_AXIS, accel_eaxis);
      break;

    case JerkXaxisEnterKey:
      float jerk_xaxis;
      jerk_xaxis = planner.max_jerk.x;
      jerk_xaxis = (float)recdat.data[0] / 100;
      RTS_SndData(jerk_xaxis * 100, MAX_JERK_XAXIS_DATA_VP);
      planner.set_max_jerk(X_AXIS, jerk_xaxis);
      break;

    case JerkYaxisEnterKey:
      float jerk_yaxis;
      jerk_yaxis = planner.max_jerk.y;
      jerk_yaxis = (float)recdat.data[0] / 100;
      RTS_SndData(jerk_yaxis * 100, MAX_JERK_YAXIS_DATA_VP);
      planner.set_max_jerk(Y_AXIS, jerk_yaxis);
      break;

    case JerkZaxisEnterKey:
      float jerk_zaxis;
      jerk_zaxis = planner.max_jerk.z;
      jerk_zaxis = (float)recdat.data[0] / 100;
      RTS_SndData(jerk_zaxis * 100, MAX_JERK_ZAXIS_DATA_VP);
      planner.set_max_jerk(Z_AXIS, jerk_zaxis);
      break;

    case JerkEaxisEnterKey:
      float jerk_eaxis;
      jerk_eaxis = planner.max_jerk.e;
      jerk_eaxis = (float)recdat.data[0] / 100;
      RTS_SndData(jerk_eaxis * 100, MAX_JERK_EAXIS_DATA_VP);
      planner.set_max_jerk(E_AXIS, jerk_eaxis);
      break;

    case StepsmmXaxisEnterKey:
      float stepsmm_xaxis;
      stepsmm_xaxis = planner.settings.axis_steps_per_mm[0];
      stepsmm_xaxis = (float)recdat.data[0] / 10;
      RTS_SndData(stepsmm_xaxis * 10, MAX_STEPSMM_XAXIS_DATA_VP);
      planner.settings.axis_steps_per_mm[X_AXIS] = stepsmm_xaxis;
      break;

    case StepsmmYaxisEnterKey:
      float stepsmm_yaxis;
      stepsmm_yaxis = planner.settings.axis_steps_per_mm[1];
      stepsmm_yaxis = (float)recdat.data[0] / 10;
      RTS_SndData(stepsmm_yaxis * 10, MAX_STEPSMM_YAXIS_DATA_VP);
      planner.settings.axis_steps_per_mm[Y_AXIS] = stepsmm_yaxis;
      break;

    case StepsmmZaxisEnterKey:
      float stepsmm_zaxis;
      stepsmm_zaxis = planner.settings.axis_steps_per_mm[2];
      stepsmm_zaxis = (float)recdat.data[0] / 10;
      RTS_SndData(stepsmm_zaxis * 10, MAX_STEPSMM_ZAXIS_DATA_VP);
      planner.settings.axis_steps_per_mm[Z_AXIS] = stepsmm_zaxis;
      break;

    case StepsmmEaxisEnterKey:
      float stepsmm_eaxis;
      stepsmm_eaxis = planner.settings.axis_steps_per_mm[3];
      stepsmm_eaxis = (float)recdat.data[0] / 10;
      RTS_SndData(stepsmm_eaxis * 10, MAX_STEPSMM_EAXIS_DATA_VP);
      planner.settings.axis_steps_per_mm[E_AXIS] = stepsmm_eaxis;
      break;

    case NozzlePTempEnterKey:
      float nozzle_ptemp;
      nozzle_ptemp = (float)recdat.data[0] / 100;
      RTS_SndData(nozzle_ptemp * 100, NOZZLE_TEMP_P_DATA_VP);
      // PID_PARAM(Kp, 0) = nozzle_ptemp;
      thermalManager.temp_hotend[0].pid.set_Kp(nozzle_ptemp);
      break;

    case NozzleITempEnterKey:
      float nozzle_itemp;
      nozzle_itemp = (float)recdat.data[0] / 100;
      RTS_SndData(nozzle_itemp * 100, NOZZLE_TEMP_I_DATA_VP);
      thermalManager.temp_hotend[0].pid.set_Ki(nozzle_itemp);
      break;

    case NozzleDTempEnterKey:
      float nozzle_dtemp;
      nozzle_dtemp = (float)recdat.data[0] / 100;
      RTS_SndData(nozzle_dtemp * 100, NOZZLE_TEMP_D_DATA_VP);
      thermalManager.temp_hotend[0].pid.set_Kd(nozzle_dtemp);
      break;

    case HotbedPTempEnterKey:
      float hotbed_ptemp;
      hotbed_ptemp = (float)recdat.data[0] / 100;
      RTS_SndData(hotbed_ptemp * 100, HOTBED_TEMP_P_DATA_VP);
      thermalManager.temp_bed.pid.set_Kp(hotbed_ptemp);
      break;

    case HotbedITempEnterKey:
      float hotbed_itemp;
      hotbed_itemp = (float)recdat.data[0] / 100;
      RTS_SndData(hotbed_itemp * 100, HOTBED_TEMP_I_DATA_VP);
      thermalManager.temp_bed.pid.set_Ki(hotbed_itemp);
      break;

    case HotbedDTempEnterKey:
      float hotbed_dtemp;
      hotbed_dtemp = (float)recdat.data[0] / 10;
      RTS_SndData(hotbed_dtemp * 10, HOTBED_TEMP_D_DATA_VP);
      thermalManager.temp_bed.pid.set_Kd(hotbed_dtemp);
      break;

    case PrintFanSpeedkey:
      uint8_t fan_speed;
      fan_speed = (uint8_t)recdat.data[0];
      RTS_SndData(fan_speed, PRINTER_FAN_SPEED_DATA_VP);
      thermalManager.set_fan_speed(0, fan_speed);
      break;

    case AutopidSetNozzleTemp:
      g_autoPIDHeaterTempTargetset = recdat.data[0];
      RTS_SndData(g_autoPIDHeaterTempTargetset, AUTO_PID_SET_NOZZLE_TEMP);
      break;

    case AutopidSetNozzleCycles:
      g_autoPIDHeaterCyclesTargetset = recdat.data[0];
      RTS_SndData(g_autoPIDHeaterCyclesTargetset, AUTO_PID_SET_NOZZLE_CYCLES);
      break;

    case AutopidSetHotbedTemp:
      g_autoPIDHotBedTempTargetset = recdat.data[0];
      RTS_SndData(g_autoPIDHotBedTempTargetset, AUTO_PID_SET_HOTBED_TEMP);
      break;

    case AutopidSetHotbedCycles:
      g_autoPIDHotBedCyclesTargetset = recdat.data[0];
      RTS_SndData(g_autoPIDHotBedCyclesTargetset, AUTO_PID_SET_HOTBED_CYCLES);
      break;

    case Advance_K_Key:
      planner.extruder_advance_K[0] = ((float)recdat.data[0]) / 100;
      RTS_SndData(planner.extruder_advance_K[0] * 100, ADVANCE_K_SET);
      break;
    case XShapingFreqsetEnterKey:
      x_frequency = ((float)recdat.data[0]) / 100;
      stepper.set_shaping_frequency(X_AXIS, x_frequency);
      RTS_SndData(stepper.get_shaping_frequency(X_AXIS) * 100, SHAPING_X_FREQUENCY_VP);
      break;

    case YShapingFreqsetEnterKey:
      y_frequency = ((float)recdat.data[0]) / 100;
      stepper.set_shaping_frequency(Y_AXIS, y_frequency);
      RTS_SndData(stepper.get_shaping_frequency(Y_AXIS) * 100, SHAPING_Y_FREQUENCY_VP);
      break;

    case XShapingZetasetEnterKey:
      float x_zeta;
      x_zeta = ((float)recdat.data[0]) / 100;
      stepper.set_shaping_damping_ratio(X_AXIS, x_zeta);
      RTS_SndData(stepper.get_shaping_damping_ratio(X_AXIS) * 100, SHAPING_X_ZETA_VP);
      settings.save();
      break;

    case YShapingZetasetEnterKey:
      float y_zeta;
      y_zeta = ((float)recdat.data[0]) / 100;
      stepper.set_shaping_damping_ratio(Y_AXIS, y_zeta);
      RTS_SndData(stepper.get_shaping_damping_ratio(Y_AXIS) * 100, SHAPING_Y_ZETA_VP);
      settings.save();
      break;

    case XMinPosEepromEnterKey:
      float x_min_pos_eeprom_temp;
      if (recdat.data[0] >= 32768) {
        x_min_pos_eeprom_temp  = ((float)recdat.data[0] - 65536) / 100;
        x_min_pos_eeprom_temp -= 0.001;
      }
      else {
        x_min_pos_eeprom_temp  = ((float)recdat.data[0]) / 100;
        x_min_pos_eeprom_temp += 0.001;
      }
      x_min_pos_eeprom = x_min_pos_eeprom_temp;
      #if ENABLED(FILAMENT_RUNOUT_SENSOR_DEBUG)
        SERIAL_ECHO_MSG("XMinPosEepromEnterKey\n", x_min_pos_eeprom_temp);
      #endif
      // BL24CXX::writeOneByte(10, x_min_pos_eeprom);
      //RTS_SndData(x_min_pos_eeprom_temp * 100, X_MIN_POS_EEPROM_VP);
      settings.save();
      break;

    case YMinPosEepromEnterKey:
      float y_min_pos_eeprom_temp;
      if (recdat.data[0] >= 32768) {
        y_min_pos_eeprom_temp  = ((float)recdat.data[0] - 65536) / 100;
        y_min_pos_eeprom_temp -= 0.001;
      }
      else {
        y_min_pos_eeprom_temp  = ((float)recdat.data[0]) / 100;
        y_min_pos_eeprom_temp += 0.001;
      }
      y_min_pos_eeprom = y_min_pos_eeprom_temp;
      #if ENABLED(FILAMENT_RUNOUT_SENSOR_DEBUG)
        SERIAL_ECHO_MSG("YMinPosEepromEnterKey\n", y_min_pos_eeprom_temp);
      #endif
      RTS_SndData(y_min_pos_eeprom_temp * 100, Y_MIN_POS_EEPROM_VP);
      settings.save();
      break;

    case SelectFileKey:
      if (RTS_SD_Detected()) {
        if (recdat.data[0] > CardRecbuf.Filesum) break;
        CardRecbuf.recordcount = recdat.data[0] - 1;
        std::string filename = CardRecbuf.Cardfilename[CardRecbuf.recordcount];
        // Find the last occurrence of the '.' character in the filename
        std::size_t dot_pos = filename.find_last_of('.');

        if (dot_pos == std::string::npos) {
          card.cd(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount]);
          int16_t fileCnt = card.get_num_items();
          card.getWorkDirName();

          if (fileCnt > 5) {
            file_total_page = (fileCnt / 5) + 1;
            if (file_total_page > 5) file_total_page = 5;
          }
          else {
            file_total_page = 1;
          }

          RTS_SndData(file_total_page, PRINT_COUNT_PAGE_DATA_VP);
          file_current_page = 1;
          RTS_SndData(file_current_page, PRINT_CURRENT_PAGE_DATA_VP);
          RTS_line_to_filelist();
          CardRecbuf.selectFlag = false;
          if (PoweroffContinue /*|| print_job_timer.isRunning()*/) return;

          // clean print file
          for (int j = 0; j < 20; j++) RTS_SndData(0, PRINT_FILE_TEXT_VP + j);
          lcd_sd_status = IS_SD_INSERTED();
        }
        else {
          CardRecbuf.selectFlag  = true;
          CardRecbuf.recordcount = recdat.data[0] - 1;
          for (int j = 0; j < 20; j++) RTS_SndData(0, SELECT_FILE_TEXT_VP + j);
          delay(2);
          RTS_SndData((unsigned long)0xFFFF, FilenameNature + recdat.data[0] * 16);
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
          change_page_font = 1;

          #if ENABLED(GCODE_PREVIEW_ENABLED)
            char ret;
            gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, false);
            ret = gcodePicDataSendToDwin(CardRecbuf.Cardfilename[CardRecbuf.recordcount], VP_OVERLAY_PIC_PTINT, PIC_FORMAT_JPG, PIC_RESOLUTION_300_300);
            if (ret == PIC_OK)
              gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, false);
            else
              gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);

          #endif

          rts_start_print = true;
          delay(20);
          RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], SELECT_FILE_TEXT_VP);
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
          change_page_font = 1;
        }
      }
      break;

    case StartFileKey:
      if ((recdat.data[0] == 1) && RTS_SD_Detected()) {
        if (CardRecbuf.recordcount < 0)
          break;
        if (!rts_start_print)
          //SERIAL_ECHOLNPGM("\r\nrts_start_print: ", rts_start_print);
          break;

        char cmd[20];
        char *c;
        sprintf_P(cmd, PSTR("M23 %s"), CardRecbuf.Cardfilename[CardRecbuf.recordcount]);
        for (c = &cmd[4]; *c; c++) *c = tolower(*c);

        memset(cmdbuf, 0, sizeof(cmdbuf));
        strcpy(cmdbuf, cmd);
        FilenamesCount = CardRecbuf.recordcount;
        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          if (READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_STATE) {
            RTS_SndData(ExchangePageBase + 46, ExchangepageAddr);
            change_page_font   = 46;
            sdcard_pause_check = false;
            break;
          }
        #endif

        rts_start_print = false;

        queue.enqueue_one_now(cmd);
        delay(20);
        queue.enqueue_now_P(PSTR("M24"));
        // clean screen.
        for (int j = 0; j < 20; j++) RTS_SndData(0, PRINT_FILE_TEXT_VP + j);

        RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);

        delay(2);

        #if ENABLED(BABYSTEPPING)
          RTS_SndData(0, AUTO_BED_LEVEL_ZOFFSET_VP);
        #endif
        feedrate_percentage = 100;
        RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
        zprobe_zoffset = probe.offset.z;
        RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
        PoweroffContinue = true;
        RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
        Update_Time_Value = 0;
      }
      else if (recdat.data[0] == 2) {
        if (!planner.has_blocks_queued()) {
          RTS_SndData(file_total_page, PRINT_COUNT_PAGE_DATA_VP);
          if ((file_total_page > file_current_page) && (file_current_page < (MaxFileNumber / 4)))
            file_current_page++;
          else
            break;
          RTS_SndData(file_current_page, PRINT_CURRENT_PAGE_DATA_VP);
          RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
          change_page_font = 2;
          if (card.flag.mounted)
            RTS_line_to_filelist();
        }
      }
      else if (recdat.data[0] == 3) {

        if (!planner.has_blocks_queued()) {
          RTS_SndData(file_total_page, PRINT_COUNT_PAGE_DATA_VP);
          if (file_current_page > 1)
            file_current_page--;
          else
            break;
          RTS_SndData(file_current_page, PRINT_CURRENT_PAGE_DATA_VP);
          RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
          change_page_font = 2;
          if (card.flag.mounted)
            RTS_line_to_filelist();
        }
      }
      else if (recdat.data[0] == 4) {
        if (!planner.has_blocks_queued()) {
          RTS_SndData(file_total_page, PRINT_COUNT_PAGE_DATA_VP);
          file_current_page = 1;
          RTS_SndData(file_current_page, PRINT_CURRENT_PAGE_DATA_VP);
          RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
          change_page_font = 2;
          RTS_line_to_filelist();
        }

      }
      else if (recdat.data[0] == 5) {
        if (!planner.has_blocks_queued()) {
          RTS_SndData(file_total_page, PRINT_COUNT_PAGE_DATA_VP);
          file_current_page = file_total_page;
          RTS_SndData(file_current_page, PRINT_CURRENT_PAGE_DATA_VP);
          RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
          change_page_font = 2;
          RTS_line_to_filelist();
        }

      }
      else if (recdat.data[0] == 6) {
        RTS_SndData(ExchangePageBase + 5, ExchangepageAddr);
        change_page_font = 5;
      }
      else if (recdat.data[0] == 7) {
        RTS_SndData(ExchangePageBase + 4, ExchangepageAddr);
        change_page_font = 4;
      }
      else if (recdat.data[0] == 8) {
        RTS_SndData(ExchangePageBase + 5, ExchangepageAddr);
        change_page_font = 5;
      }
      else if (recdat.data[0] == 9) {
        RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
        change_page_font = 2;
        RTS_line_to_filelist();
      }
      else if (recdat.data[0] == 0x0A) {
        RTS_SndData(ExchangePageBase + 2, ExchangepageAddr);
        change_page_font = 2;
        RTS_line_to_filelist();
      }
      break;

    case ChangePageKey:
      RTS_SndData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);
      // represents to update file list
      if (CardUpdate && lcd_sd_status && IS_SD_INSERTED()) {
        RTS_line_to_filelist();
        for (uint16_t i = 0; i < 5; i++) {
          delay(1);
          RTS_SndData((unsigned long)0xFFFF, FilenameNature + (i + 1) * 16);
          RTS_SndData(0, FILE1_SELECT_ICON_VP + i);
        }
      }

      RTS_SndData(MACHINE_TYPE, MACHINE_TYPE_ABOUT_TEXT_VP);
      RTS_SndData(FIRMWARE_VERSION, FIRMWARE_VERSION_ABOUT_TEXT_VP);
      //RTS_SndData(SCREEN_VERSION, PRINTER_DISPLAY_VERSION_TEXT_VP);
      RTS_SndData(HARDWARE_VERSION, HARDWARE_VERSION_ABOUT_TEXT_VP);
      RTS_SndData(PRINT_SIZE, PRINTER_PRINTSIZE_TEXT_VP);

      RTS_SndData(lang == 1 ? CORP_WEBSITE_C : CORP_WEBSITE_E, WEBSITE_ABOUT_TEXT_VP);

      RTS_SndData(card.percentDone(), PRINT_PROCESS_VP);

      RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);

      RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
      RTS_SndData(thermalManager.degTargetHotend(0), HEAD_SET_TEMP_VP);
      #if ENABLED(BED_TEMP_COMP)
        if (WITHIN(thermalManager.degTargetBed(), 66, 85))
          RTS_SndData(thermalManager.degTargetBed() - 5, BED_SET_TEMP_VP);
        else if (WITHIN(thermalManager.degTargetBed(), 86, 127))
          RTS_SndData(thermalManager.degTargetBed() - 7, BED_SET_TEMP_VP);
      #else
        RTS_SndData(thermalManager.degTargetBed(), BED_SET_TEMP_VP);
      #endif
      languagedisplayUpdate();
      RTS_SndData(change_page_font + ExchangePageBase, ExchangepageAddr);
      break;

      #if HAS_CUTTER
        case SwitchDeviceKey:
          if (recdat.data[0] == 1) {
            RTS_SndData(ExchangePageBase + 57, ExchangepageAddr);
            // change_page_font = 57;
          }
          else if (recdat.data[0] == 2) {
            RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
            // change_page_font = 56;
          }
          else if (recdat.data[0] == 0x03) {
            if (change_page_font == 64) {
              RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
              change_page_font = 33;
            }
            else {
              RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
              change_page_font = 1;
            }
            laser_device.set_current_device(DEVICE_FDM);
          }
          else if (recdat.data[0] == 0x04) {
            if (change_page_font == 64) {
              RTS_SndData(ExchangePageBase + 64, ExchangepageAddr);
              change_page_font = 64;
            }
            else {
              RTS_SndData(ExchangePageBase + 50, ExchangepageAddr);
            }
          }
          else if (recdat.data[0] == 0x05) {
            uint8_t language;
            RTS_SndData(ExchangePageBase + 77, ExchangepageAddr);
            // change_page_font = 77;
            laser_device.set_current_device(DEVICE_LASER);
            language = language_change_font;
            settings.reset();
            language_change_font = language;
            settings.save();
            probe.offset.z = zprobe_zoffset = 0;
            RTS_SndData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);

            // queue.inject_P(PSTR("M999"));
            queue.enqueue_now_P(PSTR("M999\nG92.9 Z0"));

            planner.synchronize();
            RTS_SndData(0, SW_FOCUS_Z_VP);
            laser_device.laser_power_open();
          }
          else if (recdat.data[0] == 0x06) {
            if (change_page_font == 33) {
              RTS_SndData(ExchangePageBase + 33, ExchangepageAddr);
              change_page_font = 33;
            }
            else {
              RTS_SndData(ExchangePageBase + 50, ExchangepageAddr);
              change_page_font = 50;
            }
          }
          //else if (recdat.data[0] == 8) { //调整焦距 √
          //  queue.inject_P(PSTR("G92.9 Z0"));
          //  RTS_SndData(0, AXIS_Z_COORD_VP);
          //  RTS_SndData(0, SW_FOCUS_Z_VP);
          //  SERIAL_ECHOPGM("\nchange_page_font=",change_page_font);
          //  if (change_page_font == 64) {
          //    RTS_SndData(ExchangePageBase + 64, ExchangepageAddr);
          //    change_page_font = 64;
          //  }
          //  else {
          //    RTS_SndData(ExchangePageBase + 51, ExchangepageAddr);
          //    change_page_font = 51;
          //  }
          //}
          else if (recdat.data[0] == 0x0B) {
            RTS_SndData(ExchangePageBase + 56, ExchangepageAddr);
            // change_page_font = 56;
          }
          break;
      #endif // HAS_CUTTER

    case ErrorKey: {
      if (recdat.data[0] == 1) {
        if (printingIsActive()) {
          RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
          change_page_font = 10;
        }
        else if (printingIsPaused()) {
          RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
          change_page_font = 12;
        }
        else {
          RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
          change_page_font = 1;
          #if ENABLED(GCODE_PREVIEW_ENABLED)
            gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
          #endif
        }

        if (errorway == 4) hal.reboot();
      }
    } break;

    default: break;
  }
  memset(&recdat, 0, sizeof(recdat));
  recdat.head[0] = FHONE;
  recdat.head[1] = FHTWO;
}

int EndsWith(const char *str, const char *suffix) {
  if (!str || !suffix)
    return 0;
  size_t lenstr    = strlen(str);
  size_t lensuffix = strlen(suffix);
  if (lensuffix >  lenstr)
    return 0;
  return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

void EachMomentUpdate() {
  millis_t ms = millis();
  if (ms > next_rts_update_ms) {
    #if ENABLED(POWER_LOSS_RECOVERY)
      // print the file before the power is off.
      if (power_off_type_yes == 0 && lcd_sd_status && recovery.info.recovery_flag) {
        rtscheck.RTS_SndData(ExchangePageBase, ExchangepageAddr);
        if (startprogress < 100)
          rtscheck.RTS_SndData(startprogress, START_PROCESS_ICON_VP);
        // delay(30);
        if ((startprogress += 1) > 100) {
          rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
          power_off_type_yes = 1;
          for (uint16_t i = 0; i < CardRecbuf.Filesum; i++)
            if (!strcmp(CardRecbuf.Cardfilename[i], &recovery.info.sd_filename[1])) {
              rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);
              rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[i], SELECT_FILE_TEXT_VP);
              rtscheck.RTS_SndData(ExchangePageBase + 27, ExchangepageAddr);
              change_page_font = 27;
              break;
            }
        }
        return;
      }
      else if (power_off_type_yes == 0 && !recovery.info.recovery_flag) {
        rtscheck.RTS_SndData(ExchangePageBase, ExchangepageAddr);
        if (startprogress < 100)
          rtscheck.RTS_SndData(startprogress, START_PROCESS_ICON_VP);
        // delay(30);
        if ((startprogress += 1) > 100) {
          rtscheck.RTS_SndData(StartSoundSet, SoundAddr);
          power_off_type_yes = 1;
          Update_Time_Value = RTS_UPDATE_VALUE;
          change_page_font = 1;
          int16_t fileCnt = card.get_num_items();
          card.getWorkDirName();
          if (card.filename[0] != '/') card.cdup();

          for (int16_t i = 0; (i < fileCnt) && (i < MaxFileNumber); i++) {
            card.selectFileByIndexSorted(i);
            char *pointFilename = card.longFilename;
            int filenamelen = strlen(card.longFilename);
            int j = 1;
            while ((strncmp(&pointFilename[j], ".gcode", 6) && strncmp(&pointFilename[j], ".GCODE", 6)) && ((j++) < filenamelen));

            for (int j = 0; j < 20; j++) rtscheck.RTS_SndData(0, PRINT_FILE_TEXT_VP + j);

            if (j >= TEXTBYTELEN) {
              strncpy(&card.longFilename[TEXTBYTELEN - 3], "..", 2);
              card.longFilename[TEXTBYTELEN - 1] = '\0';
              j = TEXTBYTELEN - 1;
            }

            strncpy(CardRecbuf.Cardshowfilename[i], card.longFilename, j);

            strcpy(CardRecbuf.Cardfilename[i], card.filename);
            CardRecbuf.addr[i] = PRINT_FILE_TEXT_VP + 20;
            rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[i], CardRecbuf.addr[i]);
            if (!strcmp(CardRecbuf.Cardfilename[i], &recovery.info.sd_filename[1])) {
              rtscheck.RTS_SndData(CardRecbuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);
              break;
            }
          }

          #if HAS_CUTTER
            if (laser_device.is_laser_device()) {
              rtscheck.RTS_SndData(ExchangePageBase + 51, ExchangepageAddr);
              change_page_font = 51;
            }
            else if (laser_device.get_current_device() == DEVICE_UNKNOWN) {
              rtscheck.RTS_SndData(ExchangePageBase + 50, ExchangepageAddr);
              change_page_font = 50;
            }
            else
          #endif
          {
            rtscheck.RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
            change_page_font = 1;
            #if ENABLED(GCODE_PREVIEW_ENABLED)
              gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
            #endif
          }
        }

        return;
      }
      else {
        static unsigned char last_cardpercentValue = 100;
        if (card.isPrinting() && (last_cardpercentValue != card.percentDone())) {
          if ((uint8_t)card.percentDone() > 0) {
            Percentrecord = card.percentDone();
            if (Percentrecord <= 100)
              rtscheck.RTS_SndData((unsigned char)Percentrecord, PRINT_PROCESS_ICON_VP);
          }
          else {
            rtscheck.RTS_SndData(0, PRINT_PROCESS_ICON_VP);
          }
          rtscheck.RTS_SndData((uint8_t)card.percentDone(), PRINT_PROCESS_VP);
          last_cardpercentValue = card.percentDone();
          rtscheck.RTS_SndData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
        }
        else if (card.isPrinting()) {
          duration_t elapsed = print_job_timer.duration();
          Percentrecord = card.percentDone();

          rtscheck.RTS_SndData(elapsed.value / 3600, PRINT_TIME_HOUR_VP);
          rtscheck.RTS_SndData((elapsed.value % 3600) / 60, PRINT_TIME_MIN_VP);

          if (Percentrecord < 2) {
            rtscheck.RTS_SndData(0, PRINT_REMAIN_TIME_HOUR_VP);
            rtscheck.RTS_SndData(0, PRINT_REMAIN_TIME_MIN_VP);
          }
          else {
            int _remain_time = 0;
            _remain_time = ((elapsed.value) * ((float)card.getFileSize() / (float)card.getIndex())) - (elapsed.value);

            if (_remain_time < 0) _remain_time = 0;
            rtscheck.RTS_SndData(_remain_time / 3600, PRINT_REMAIN_TIME_HOUR_VP);
            rtscheck.RTS_SndData((_remain_time % 3600) / 60, PRINT_REMAIN_TIME_MIN_VP);
          }
        }
        else if (ui.get_progress_percent() != last_progress_percent || ui.get_remaining_time() != last_remaining_time) {
          rtscheck.RTS_SndData(ui.get_remaining_time() / 3600, PRINT_REMAIN_TIME_HOUR_VP);
          rtscheck.RTS_SndData((ui.get_remaining_time() % 3600) / 60, PRINT_REMAIN_TIME_MIN_VP);

          rtscheck.RTS_SndData((unsigned char) ui.get_progress_percent(), PRINT_PROCESS_ICON_VP);
          rtscheck.RTS_SndData((unsigned char) ui.get_progress_percent(), PRINT_PROCESS_VP);

          if ((ui.get_remaining_time() > 0 && last_start_time == 0) || last_progress_percent > ui.get_progress_percent())
            last_start_time = HAL_GetTick();

          if (last_start_time > 0 && ui.get_progress_percent() < 100) {
            uint32_t elapsed_seconds = (HAL_GetTick() - last_start_time) / 1000;
            rtscheck.RTS_SndData(elapsed_seconds / 3600, PRINT_TIME_HOUR_VP);
            rtscheck.RTS_SndData((elapsed_seconds % 3600) / 60, PRINT_TIME_MIN_VP);
          }

          last_progress_percent = ui.get_progress_percent();
          last_remaining_time = ui.get_remaining_time();
        }

        if (pause_action_flag && !sdcard_pause_check && printingIsPaused() && !planner.has_blocks_queued()) {
          pause_action_flag = false;
          queue.enqueue_now_P(PSTR("G0 F3000 X0 Y0"));
          thermalManager.setTargetHotend(0, 0);
          rtscheck.RTS_SndData(0, HEAD_SET_TEMP_VP);
        }

        rtscheck.RTS_SndData(thermalManager.temp_hotend[0].celsius, HEAD_CURRENT_TEMP_VP);
        rtscheck.RTS_SndData(thermalManager.temp_bed.celsius, BED_CURRENT_TEMP_VP);

        #if ENABLED(SDSUPPORT)
          if (!sdcard_pause_check && !card.isPrinting() && !planner.has_blocks_queued()) {
            if (card.flag.mounted)
              rtscheck.RTS_SndData(1, CHANGE_SDCARD_ICON_VP);
            else
              rtscheck.RTS_SndData(0, CHANGE_SDCARD_ICON_VP);
          }
        #endif

        if ((last_target_temperature[0] != thermalManager.temp_hotend[0].target) || (last_target_temperature_bed != thermalManager.temp_bed.target)) {
          thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
          thermalManager.setTargetBed(thermalManager.temp_bed.target);
          rtscheck.RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
          rtscheck.RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
          last_target_temperature[0]  = thermalManager.temp_hotend[0].target;
          last_target_temperature_bed = thermalManager.temp_bed.target;
        }

        if ((thermalManager.temp_hotend[0].celsius >= thermalManager.temp_hotend[0].target) && (heatway == 1)) {
          rtscheck.RTS_SndData(ExchangePageBase + 19, ExchangepageAddr);
          change_page_font = 19;
          heatway = 0;
          rtscheck.RTS_SndData(10 * FilamentLOAD, HEAD_FILAMENT_LOAD_DATA_VP);
          rtscheck.RTS_SndData(10 * FilamentUnLOAD, HEAD_FILAMENT_UNLOAD_DATA_VP);
        }
        #if ENABLED(FILAMENT_RUNOUT_SENSOR)
          if (READ(FIL_RUNOUT_PIN) != FIL_RUNOUT_STATE)
            rtscheck.RTS_SndData(0, FILAMENT_LOAD_ICON_VP);
          else
            rtscheck.RTS_SndData(1, FILAMENT_LOAD_ICON_VP);
        #endif
        rtscheck.RTS_SndData(thermalManager.fan_speed[0], PRINTER_FAN_SPEED_DATA_VP);
      }
    #endif // POWER_LOSS_RECOVERY

    next_rts_update_ms = ms + RTS_UPDATE_INTERVAL + Update_Time_Value;
  }
}

void RTSSHOW::languagedisplayUpdate() {
  RTS_SndData(lang, MAIN_PAGE_BLUE_TITLE_VP);
  RTS_SndData(lang, SELECT_FILE_BLUE_TITLE_VP);
  RTS_SndData(lang, PREPARE_PAGE_BLUE_TITLE_VP);
  RTS_SndData(lang, SETTING_PAGE_BLUE_TITLE_VP);
  RTS_SndData(lang, MAIN_PAGE_BLACK_TITLE_VP);
  RTS_SndData(lang, SELECT_FILE_BLACK_TITLE_VP);
  RTS_SndData(lang, PREPARE_PAGE_BLACK_TITLE_VP);
  RTS_SndData(lang, SETTING_PAGE_BLACK_TITLE_VP);

  RTS_SndData(lang, PRINT_ADJUST_MENT_TITLE_VP);
  RTS_SndData(lang, PRINT_SPEED_TITLE_VP);
  RTS_SndData(lang, HEAD_SET_TITLE_VP);
  RTS_SndData(lang, BED_SET_TITLE_VP);
  RTS_SndData(lang, LEVEL_ZOFFSET_TITLE_VP);
  RTS_SndData(lang, FAN_CONTROL_TITLE_VP);
  RTS_SndData(lang, LED_CONTROL_TITLE_VP);

  RTS_SndData(lang, MOVE_AXIS_ENTER_GREY_TITLE_VP);
  RTS_SndData(lang, CHANGE_FILAMENT_GREY_TITLE_VP);
  RTS_SndData(lang, PREHAET_PAGE_GREY_TITLE_VP);
  RTS_SndData(lang, MOVE_AXIS_ENTER_BLACK_TITLE_VP);
  RTS_SndData(lang, CHANGE_FILAMENT_BLACK_TITLE_VP);
  RTS_SndData(lang, PREHAET_PAGE_BLACK_TITLE_VP);

  RTS_SndData(lang, PREHEAT_PLA_BUTTON_TITLE_VP);
  RTS_SndData(lang, PREHEAT_ABS_BUTTON_TITLE_VP);
  RTS_SndData(lang, PREHEAT_PETG_BUTTON_TITLE_VP);
  RTS_SndData(lang, PREHEAT_CUST_BUTTON_TITLE_VP);
  RTS_SndData(lang, COOL_DOWN_BUTTON_TITLE_VP);

  RTS_SndData(lang, FILAMENT_LOAD_BUTTON_TITLE_VP);
  RTS_SndData(lang, FILAMENT_UNLOAD_BUTTON_TITLE_VP);

  RTS_SndData(lang, LANGUAGE_SELECT_ENTER_VP);
  RTS_SndData(lang, FACTORY_DEFAULT_ENTER_TITLE_VP);
  RTS_SndData(lang, LEVELING_PAGE_TITLE_VP);

  RTS_SndData(lang, PRINTER_DEVICE_GREY_TITLE_VP);
  RTS_SndData(lang, PRINTER_ADVINFO_GREY_TITLE_VP);
  RTS_SndData(lang, PRINTER_INFO_ENTER_GREY_TITLE_VP);
  RTS_SndData(lang, PRINTER_DEVICE_BLACK_TITLE_VP);
  RTS_SndData(lang, PRINTER_ADVINFO_BLACK_TITLE_VP);
  RTS_SndData(lang, PRINTER_INFO_ENTER_BLACK_TITLE_VP);

  RTS_SndData(lang, PREHEAT_PLA_SET_TITLE_VP);
  RTS_SndData(lang, PREHEAT_ABS_SET_TITLE_VP);
  RTS_SndData(lang, PREHEAT_PETG_SET_TITLE_VP);
  RTS_SndData(lang, PREHEAT_CUST_SET_TITLE_VP);

  RTS_SndData(lang, STORE_MEMORY_CONFIRM_TITLE_VP);
  RTS_SndData(lang, STORE_MEMORY_CANCEL_TITLE_VP);

  RTS_SndData(lang, FILAMENT_UNLOAD_IGNORE_TITLE_VP);
  RTS_SndData(lang, FILAMENT_USEUP_TITLE_VP);
  RTS_SndData(lang, BUTTON_CHECK_CONFIRM_TITLE_VP);
  RTS_SndData(lang, BUTTON_CHECK_CANCEL_TITLE_VP);
  RTS_SndData(lang, FILAMENT_LOAD_TITLE_VP);
  RTS_SndData(lang, FILAMENT_LOAD_RESUME_TITLE_VP);
  RTS_SndData(lang, PAUSE_PRINT_POP_TITLE_VP);
  RTS_SndData(lang, STOP_PRINT_POP_TITLE_VP);
  RTS_SndData(lang, POWERCONTINUE_POP_TITLE_VP);
  RTS_SndData(lang, AUTO_HOME_WAITING_POP_TITLE_VP);

  RTS_SndData(lang, BEDLEVELING_WAIT_TITLE_VP);
  RTS_SndData(lang, RESTORE_FACTORY_TITLE_VP);
  RTS_SndData(lang, RESET_WIFI_SETTING_TITLE_VP);
  RTS_SndData(lang, KILL_THERMAL_RUNAWAY_TITLE_VP);
  RTS_SndData(lang, KILL_HEATING_FAIL_TITLE_VP);
  RTS_SndData(lang, KILL_THERMISTOR_ERROR_TITLE_VP);
  RTS_SndData(lang, WIND_AUTO_SHUTDOWN_TITLE_VP);
  RTS_SndData(lang, RESET_WIFI_SETTING_BUTTON_VP);
  RTS_SndData(lang, PRINTER_AUTO_SHUTDOWN_TITLE_VP);
  RTS_SndData(lang, WIND_AUTO_SHUTDOWN_PAGE_VP);
  RTS_SndData(lang, AUTO_LEVELING_START_TITLE_VP);
  RTS_SndData(lang, AUX_LEVELING_GREY_TITLE_VP);
  RTS_SndData(lang, AUTO_LEVELING_GREY_TITLE_VP);
  RTS_SndData(lang, AUX_LEVELING_BLACK_TITLE_VP);
  RTS_SndData(lang, AUTO_LEVELING_BLACK_TITLE_VP);
  RTS_SndData(lang, LANGUAGE_SELECT_PAGE_TITLE_VP);
  RTS_SndData(lang, ADV_SETTING_MOTION_TITLE_VP);
  RTS_SndData(lang, ADV_SETTING_PID_TITLE_VP);
  RTS_SndData(lang, ADV_SETTING_WIFI_TITLE_VP);

  RTS_SndData(lang, MOTION_SETTING_TITLE_VP);
  RTS_SndData(lang, MOTION_SETTING_STEPSMM_TITLE_VP);
  RTS_SndData(lang, MOTION_SETTING_ACCEL_TITLE_VP);
  RTS_SndData(lang, MOTION_SETTING_JERK_TITLE_VP);
  RTS_SndData(lang, MOTION_SETTING_VELOCITY_TITLE_VP);

  RTS_SndData(lang, MAX_VELOCITY_SETTING_TITLE_VP);
  RTS_SndData(lang, MAX_VELOCITY_XAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_VELOCITY_YAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_VELOCITY_ZAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_VELOCITY_EAXIS_TITLE_VP);

  RTS_SndData(lang, MAX_ACCEL_SETTING_TITLE_VP);
  RTS_SndData(lang, MAX_ACCEL_XAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_ACCEL_YAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_ACCEL_ZAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_ACCEL_EAXIS_TITLE_VP);

  RTS_SndData(lang, MAX_JERK_SETTING_TITLE_VP);
  RTS_SndData(lang, MAX_JERK_XAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_JERK_YAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_JERK_ZAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_JERK_EAXIS_TITLE_VP);

  RTS_SndData(lang, MAX_STEPSMM_SETTING_TITLE_VP);
  RTS_SndData(lang, MAX_STEPSMM_XAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_STEPSMM_YAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_STEPSMM_ZAXIS_TITLE_VP);
  RTS_SndData(lang, MAX_STEPSMM_EAXIS_TITLE_VP);

  RTS_SndData(lang, TEMP_PID_SETTING_TITLE_VP);
  RTS_SndData(lang, NOZZLE_TEMP_P_TITLE_VP);
  RTS_SndData(lang, NOZZLE_TEMP_I_TITLE_VP);
  RTS_SndData(lang, NOZZLE_TEMP_D_TITLE_VP);
  RTS_SndData(lang, HOTBED_TEMP_P_TITLE_VP);
  RTS_SndData(lang, HOTBED_TEMP_I_TITLE_VP);
  RTS_SndData(lang, HOTBED_TEMP_D_TITLE_VP);

  RTS_SndData(lang, FILAMENT_CONTROL_TITLE_VP);
  RTS_SndData(lang, POWERCONTINUE_CONTROL_TITLE_VP);

  RTS_SndData(lang, MACHINE_TYPE_ABOUT_CHAR_VP);
  RTS_SndData(lang, FIRMWARE_VERSION_ABOUT_CHAR_VP);
  RTS_SndData(lang, PRINTER_DISPLAY_VERSION_TITLE_VP);
  RTS_SndData(lang, HARDWARE_VERSION_ABOUT_TITLE_VP);
  RTS_SndData(lang, WIFI_DN_CODE_CHAR_VP);
  RTS_SndData(lang, WEBSITE_ABOUT_CHAR_VP);
  RTS_SndData(lang, PRINTER_PRINTSIZE_TITLE_VP);

  RTS_SndData(lang, PLA_SETTINGS_TITLE_VP);
  RTS_SndData(lang, ABS_SETTINGS_TITLE_VP);
  RTS_SndData(lang, PETG_SETTINGS_TITLE_VP);
  RTS_SndData(lang, CUST_SETTINGS_TITLE_VP);
  RTS_SndData(lang, LEVELING_WAY_TITLE_VP);
  RTS_SndData(lang, SOUND_SETTING_VP);
  RTS_SndData(lang, PRINT_FINISH_ICON_VP);
  #if HAS_CUTTER
    RTS_SndData(lang, SELECT_LASER_WARNING_TIPS_VP);
    RTS_SndData(lang, SELECT_FDM_WARNING_TIPS_VP);
    RTS_SndData(lang, PRINT_MOVE_AXIS_VP);
    RTS_SndData(lang, PRINT_DIRECT_ENGRAV_VP);
    RTS_SndData(lang, PRINT_RUN_RANGE_VP);
    RTS_SndData(lang, PRINT_RETURN_VP);
    RTS_SndData(lang, PRINT_WARNING_TIPS_VP);
    RTS_SndData(lang, DEVICE_SWITCH_LASER_VP);
    RTS_SndData(lang, FIRST_SELECT_DEVICE_TYPE);
    RTS_SndData(lang, HOME_LASER_ENGRAVE_VP);
    RTS_SndData(lang, PREPARE_ADJUST_FOCUS_VP);
    RTS_SndData(lang, PREPARE_SWITCH_FDM_VP);
    RTS_SndData(lang, FIRST_DEVICE_FDM);
    RTS_SndData(lang, FIRST_DEVICE_LASER);
    RTS_SndData(lang, FOCUS_SET_FOCUS_TIPS);
  #endif
  RTS_SndData(lang, AUTO_PID_INLET_VP);
  RTS_SndData(lang, AUTO_PID_HOTBED_INLET_VP);
  RTS_SndData(lang, AUTO_PID_HOTBED_TIS_VP);
  RTS_SndData(lang, AUTO_PID_NOZZLE_INLET_VP);
  RTS_SndData(lang, AUTO_PID_NOZZLE_TIS_VP);
  RTS_SndData(lang, AUTO_PID_START_NOZZLE_VP);
  RTS_SndData(lang, AUTO_PID_START_HOTBED_VP);
  RTS_SndData(lang, MESH_LEVELING_BLACK_TITLE_VP);
  RTS_SndData(lang, SHAPING_X_TITLE_VP);
  RTS_SndData(lang, SHAPING_Y_TITLE_VP);
}

// looping at the loop function
void RTSUpdate() {
  // Check the status of card
  rtscheck.RTS_SDCardUpate();

  sd_printing = IS_SD_PRINTING();
  card_insert_st = IS_SD_INSERTED();

  if ((card_insert_st == false) && (sd_printing == true)) {
    rtscheck.RTS_SndData(ExchangePageBase + 47, ExchangepageAddr);
    rtscheck.RTS_SndData(0, CHANGE_SDCARD_ICON_VP);
    card.pauseSDPrint();
    print_job_timer.pause();
    pause_action_flag = true;
    sdcard_pause_check = false;
  }

  if (last_card_insert_st != card_insert_st) {
    rtscheck.RTS_SndData((int)card_insert_st, CHANGE_SDCARD_ICON_VP);
    last_card_insert_st = card_insert_st;
  }

  EachMomentUpdate();

  // wait to receive massage and response
  if (rtscheck.RTS_RecData() > 0) rtscheck.RTS_HandleData();

  hal.watchdog_refresh();
}

void RTS_PauseMoveAxisPage() {
  #if HAS_CUTTER
    if (laser_device.is_laser_device()) return;
  #endif

  if (waitway == 1) {
    rtscheck.RTS_SndData(ExchangePageBase + 12, ExchangepageAddr);
    change_page_font = 12;
    waitway          = 0;
  }
  else if (waitway == 5) {
    rtscheck.RTS_SndData(ExchangePageBase + 7, ExchangepageAddr);
    change_page_font = 7;
    waitway          = 0;
  }
  else if (waitway == 7) {
    // Click stop print
    rtscheck.RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
    change_page_font = 1;
    #if ENABLED(GCODE_PREVIEW_ENABLED)
      gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
    #endif
    waitway = 0;
  }
}

void RTS_AutoBedLevelPage() {
  if (waitway == 3) {
    if (old_leveling == 1) {
      rtscheck.RTS_SndData(0, AXIS_Z_COORD_VP);
      rtscheck.RTS_SndData(ExchangePageBase + 26, ExchangepageAddr);
      change_page_font = 26;
      waitway = 0;
    }
    else {
      rtscheck.RTS_SndData(lang, AUTO_LEVELING_START_TITLE_VP);
      rtscheck.RTS_SndData(0, AXIS_Z_COORD_VP);
      rtscheck.RTS_SndData(ExchangePageBase + 81, ExchangepageAddr);
      change_page_font = 81;
      waitway = 0;
    }
  }
}

void RTS_MoveAxisHoming() {
  if (waitway == 4) {
    rtscheck.RTS_SndData(ExchangePageBase + 16 + (AxisUnitMode - 1), ExchangepageAddr);
    change_page_font = 16;
    waitway = 0;
  }
  else if (waitway == 6) {
    waitway = 0;
  }
  else if (waitway == 7) {
    // Click Print finish
    rtscheck.RTS_SndData(ExchangePageBase + 1, ExchangepageAddr);
    #if ENABLED(GCODE_PREVIEW_ENABLED)
      gcodePicDisplayOnOff(DEFAULT_PRINT_MODEL_VP, true);
    #endif
    change_page_font = 1;
    waitway = 0;
  }
  else if (waitway == 8) {
    rtscheck.RTS_SndData(ExchangePageBase + 78 + (AxisUnitMode - 1), ExchangepageAddr);
    change_page_font = 78 + (AxisUnitMode - 1);
    waitway = 0;
  }
  else if (waitway == 9) {
    rtscheck.RTS_SndData(ExchangePageBase + 70 + (AxisUnitMode - 1), ExchangepageAddr);
    change_page_font = 70 + (AxisUnitMode - 1);
    waitway = 0;
  }
  else if (waitway == 10) {
    rtscheck.RTS_SndData(ExchangePageBase + 51, ExchangepageAddr);
    change_page_font = 51;
    waitway = 0;
  }
  else if (waitway == 14) {
    rtscheck.RTS_SndData(ExchangePageBase + 86 + (AxisUnitMode - 1), ExchangepageAddr);
    change_page_font = 86;
    waitway = 0;
  }
  else if (waitway == 15) {
    rtscheck.RTS_SndData(ExchangePageBase + 81, ExchangepageAddr);
    change_page_font = 81;
    waitway = 0;
  }
  else if (waitway == 16) {
    rtscheck.RTS_SndData(ExchangePageBase + 25, ExchangepageAddr);
    change_page_font = 25;
    waitway = 0;
  }
  else if (waitway == 17) {
    rtscheck.RTS_SndData(ExchangePageBase + 89, ExchangepageAddr);
    change_page_font = 89;
    waitway = 0;
  }

  if (TERN1(HAS_CUTTER, !laser_device.is_laser_device())) {
    rtscheck.RTS_SndData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
    rtscheck.RTS_SndData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
  }
  rtscheck.RTS_SndData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
}

void RTS_CommandPause() {
  if (card.isPrinting()) {
    rtscheck.RTS_SndData(ExchangePageBase + 10, ExchangepageAddr);
    change_page_font = 10;
    // card.pauseSDPrint();
    // print_job_timer.pause();
    // pause_action_flag = true;
  }
}

void ErrorHanding() {
  // No more operations
  if (errorway == 1) {
    errorway = errornum = 0;
  }
  else if (errorway == 2) {
    // Z axis home failed
    home_errornum++;
    if (home_errornum <= 3) {
      errorway = 0;
      waitway = 4;
      queue.enqueue_now_P(PSTR("G28"));
      rtscheck.RTS_SndData(0, MOTOR_FREE_ICON_VP);
      Update_Time_Value = 0;
    }
    else {
      // After three failed returns to home, it will report the failure interface
      home_errornum = 0;
      errorway = 0;
      rtscheck.RTS_SndData(ExchangePageBase + 41, ExchangepageAddr);
      change_page_font = 41;
      // Z axis home failed
      rtscheck.RTS_SndData(Error_202, ABNORMAL_PAGE_TEXT_VP);

      if (printingIsActive()) {
        rtscheck.RTS_SndData(0, PRINT_TIME_HOUR_VP);
        rtscheck.RTS_SndData(0, PRINT_TIME_MIN_VP);
        rtscheck.RTS_SndData(0, PRINT_REMAIN_TIME_HOUR_VP);
        rtscheck.RTS_SndData(0, PRINT_REMAIN_TIME_MIN_VP);

        Update_Time_Value = 0;

        rtscheck.RTS_SDcard_Stop();
      }
    }
  }
  else if (errorway == 3) {
    // No more operations
    reset_bed_level();
    errorway = 0;
    errornum = 0;
  }
  else if (errorway == 4) {

  }
}

#endif // E3S1PRO_RTS
