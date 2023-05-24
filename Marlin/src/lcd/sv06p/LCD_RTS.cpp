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
#include "../../inc/MarlinConfig.h"

#if ENABLED(SOVOL_SV06_RTS)

#include "LCD_RTS.h"

DB RTS::recdat;
DB RTS::snddat;
uint8_t RTS::databuf[DATA_BUF_SIZE];

bool RTS::dark_mode = false;

RTS rts;

#include <WString.h>
#include <stdio.h>
#include <string.h>
#include "../../MarlinCore.h"
#include "../../core/serial.h"
#include "../../core/macros.h"
#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/settings.h"
#include "../../module/motion.h"
#include "../../module/printcounter.h"
#include "../../libs/duration_t.h"
#include "../../feature/babystep.h"
#include "../../feature/powerloss.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../feature/tmc_util.h"
#include "../../gcode/queue.h"
#include "../../gcode/gcode.h"
//#include "../marlinui.h"
//#include "../utf8.h"
#include "../../libs/BL24CXX.h"

#if ENABLED(FIX_MOUNTED_PROBE)
  #include "../../module/endstops.h"
#endif
#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif

//#define CHECKFILAMENT

float zprobe_zoffset;
float last_zoffset = 0.0;

int startprogress = 0;
CRec CardRecbuf;
bool sdcard_pause_check = true;

float ChangeFilament0Temp = 200;

int StartFlag = 0;
int print_flag = 0;
bool start_print_flag = false;

int heatway = 0;
millis_t next_rts_update_ms = 0;
int last_target_temperature[4] = { 0 };
int last_target_temperature_bed;
char waitway = 0;
int recnum = 0;
unsigned char Percentrecord = 0;

bool pause_action_flag = false;
bool pause_flag = false;
bool power_off_type_yes = false;
// represents to update file list
bool CardUpdate = false;

// represents SD-card status, true means SD is available, false means opposite.
bool lcd_sd_status;

char Checkfilenum = 0;
int FilenamesCount = 0;
char cmdbuf[20] = { 0 };
float Filament0LOAD = 10;
float XoffsetValue;

// 0 for 10mm, 1 for 1mm, 2 for 0.1mm
unsigned char AxisUnitMode;
float axis_unit = 10;
int Update_Time_Value = 0;

bool poweroff_continue = false;
char commandbuf[30];

static int change_page_number = 0;

uint16_t remain_time = 0;

static bool last_card_insert_st;
bool card_insert_st;
bool sd_printing;

int fan_speed;
char cmd[MAX_CMD_SIZE + 16];

inline void RTS_line_to_current(const AxisEnum axis) {
  if (!planner.is_full())
    planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[axis]), active_extruder);
}

RTS::RTS() {
  recdat.head[0] = snddat.head[0] = FHONE;
  recdat.head[1] = snddat.head[1] = FHTWO;
  ZERO(databuf);
}

void RTS::sdCardInit(void) {
  if (sdDetected()) card.mount();
  if (CardReader::flag.mounted) {
    int16_t fileCnt = card.get_num_items();
    card.getWorkDirName();
    if (card.filename[0] != '/') card.cdup();

    int addrnum = 0;
    int num = 0;
    for (int16_t i = 0; (i < fileCnt) && (i < (MaxFileNumber + addrnum)); i++) {
      card.selectFileByIndex(fileCnt - 1 - i);
      char *pointFilename = card.longFilename;
      int filenamelen = strlen(card.longFilename);
      int j = 1;
      while ((strncmp(&pointFilename[j], ".gcode", 6) && strncmp(&pointFilename[j], ".GCODE", 6)) && ((j ++) < filenamelen));
      if (j >= filenamelen) {
        addrnum++;
        continue;
      }

      if (j >= TEXTBYTELEN) {
        strncpy(&card.longFilename[TEXTBYTELEN - 3], "..", 2);
        card.longFilename[TEXTBYTELEN - 1] = '\0';
        j = TEXTBYTELEN - 1;
      }

      strncpy(CardRecbuf.Cardshowfilename[num], card.longFilename, j);

      strcpy(CardRecbuf.Cardfilename[num], card.filename);
      CardRecbuf.addr[num] = FILE1_TEXT_VP + (num * 20);
      sendData(CardRecbuf.Cardshowfilename[num], CardRecbuf.addr[num]);
      CardRecbuf.Filesum = (++num);
    }
    for (int j = CardRecbuf.Filesum;j < MaxFileNumber;j ++) {
      CardRecbuf.addr[j] = FILE1_TEXT_VP + (j * 20);
      sendData(0, CardRecbuf.addr[j]);
    }
    for (int j = 0; j < 20; j++) {
      // clean print file 清除打印界面中显示的文件名
      sendData(0, PRINT_FILE_TEXT_VP + j);
    }
    lcd_sd_status = IS_SD_INSERTED();
  }
  else {
    // Clean all filename Icons
    for (int j = 0; j < MaxFileNumber; j++)
      for (int i = 0; i < TEXTBYTELEN; i++)
        sendData(0, CardRecbuf.addr[j] + i);
    ZERO(CardRecbuf);
  }
}

bool RTS::sdDetected(void) {
  static bool last;
  static bool state;
  static bool flag_stable;
  static uint32_t stable_point_time;

  bool tmp = IS_SD_INSERTED();

  if (tmp != last)
    flag_stable = false;
  else if (!flag_stable) {
    flag_stable = true;
    stable_point_time = millis();
  }

  if (flag_stable && millis() - stable_point_time > 30)
    state = tmp;

  last = tmp;

  return state;
}

void RTS::sdCardUpdate(void) {
  const bool sd_status = sdDetected();
  if (sd_status != lcd_sd_status) {
    if (sd_status) {
      // SD card power on
      card.mount();
      sdCardInit();
    }
    else {
      card.release();

      for (int i = 0; i < CardRecbuf.Filesum; i++) {
        for (int j = 0; j < 20; j++) sendData(0, CardRecbuf.addr[i] + j);
        sendData((unsigned long)0x738E, FilenameNature + (i + 1) * 16);
      }

      // Clean screen
      for (int j = 0;j < 20;j ++) {
        sendData(0, PRINT_FILE_TEXT_VP + j);
        sendData(0, SELECT_FILE_TEXT_VP + j);
      }
      ZERO(CardRecbuf);
    }
    lcd_sd_status = sd_status;
  }

  // represents to update file list
  if (CardUpdate && lcd_sd_status && sdDetected()) {
    for (uint16_t i = 0; i < CardRecbuf.Filesum; i++) {
      delay(1);
      sendData(CardRecbuf.Cardshowfilename[i], CardRecbuf.addr[i]);
      sendData((unsigned long)0x738E, FilenameNature + (i + 1) * 16);
    }
    CardUpdate = false;
  }
}

void RTS::init() {
  dark_mode = BL24CXX::readOneByte(FONT_EEPROM);
  AxisUnitMode = 1;
  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    bool zig = false;
    int8_t inStart, inStop, inInc, showcount;
    showcount = 0;
    for (int y = 0; y < GRID_MAX_POINTS_Y; y++) {
      // away from origin
      if (zig) {
        inStart = 0;
        inStop = GRID_MAX_POINTS_X;
        inInc = 1;
      }
      else {
        // towards origin
        inStart = GRID_MAX_POINTS_X - 1;
        inStop = -1;
        inInc = -1;
      }
      zig ^= true;
      for (int x = inStart; x != inStop; x += inInc) {
        sendData(bedlevel.z_values[x][y] * 100, AUTO_BED_LEVEL_1POINT_VP + showcount * 2);
        showcount++;
      }
    }
    queue.enqueue_now(F("M420 S1"));
  #endif
  last_zoffset = zprobe_zoffset = probe.offset.z;
  sendData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);

  last_target_temperature_bed = thermalManager.temp_bed.target;
  last_target_temperature[0] = thermalManager.temp_hotend[0].target;

  feedrate_percentage = 100;
  sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);

  /***************turn off motor*****************/
  sendData(1, MOTOR_FREE_ICON_VP);

  /***************transmit temperature to screen*****************/
  updateTempE0();
  updateTempBed();

  /***************transmit Fan speed to screen*****************/
  // turn off fans
  thermalManager.zero_fan_speeds();
  //queue.enqueue_now(F("M107"));
  //sendData(1, HEAD0_FAN_ICON_VP);
  updateFan0();

  delay(5);

  /*********transmit SD card filename to screen***************/
  sdCardInit();

  /**************************some info init*******************************/
  sendData(0, PRINT_PROCESS_ICON_VP);
  change_page_number = CardReader::flag.mounted ? (dark_mode ? 1 : 56) : 0;
}

int RTS::receiveData() {
  int frame_index = 0;
  int timeout = 0;
  int framelen = 0;
  bool frame_flag = false;
  if (LCD_SERIAL.available() <= 0) return -1;
  do {
    if (LCD_SERIAL.available() > 0) {
      databuf[frame_index] = LCD_SERIAL.read();
      timeout = 0;
      // 0x5A
      if (frame_index == 0 && databuf[frame_index] == FHONE) {
        frame_index++;
        continue;
      }
      // 0xA5
      else if (frame_index == 1) {
        if (databuf[frame_index] == FHTWO) frame_index++; else frame_index = 0;
        continue;
      }
      // 长度
      else if (frame_index == 2) {
        framelen = databuf[frame_index];
        frame_index++;
        continue;
      }
      else if (frame_index != 0) {
        frame_index++;
        // 一帧数据提取完毕，剩余的串口数据下次进入这个函数会在处理
        if (frame_index == (framelen + 3)) {
          frame_flag = true;
          break;
        }
      }
    }
    else {
      timeout++;
      delay(1);
    }
  } while (timeout < 50); // 超时函数

  if (frame_flag) {
    recdat.head[0] = databuf[0];
    recdat.head[1] = databuf[1];
    recdat.len = databuf[2];
    recdat.command = databuf[3];
    for (int idx = 0; idx < frame_index; idx++) { }
  }
  else
    return -1;

  // response for writing byte
  if (recdat.len == 0x03
    && (recdat.command == 0x82 || recdat.command == 0x80)
    && databuf[4] == 0x4F && databuf[5] == 0x4B
  ) {
    ZERO(databuf);
    recnum = 0;
    return -1;
  }
  else if (recdat.command == 0x83) {
    // response for reading the data from the variate
    recdat.addr = databuf[4];
    recdat.addr = (recdat.addr << 8) | databuf[5];
    recdat.bytelen = databuf[6];
    for (unsigned int i = 0; i < recdat.bytelen; i += 2) {
      recdat.data[i / 2] = databuf[7 + i];
      recdat.data[i / 2] = (recdat.data[i / 2] << 8) | databuf[8 + i];
    }
  }
  else if (recdat.command == 0x81) {
    // response for reading the page from the register
    recdat.addr = databuf[4];
    recdat.bytelen = databuf[5];
    for (unsigned int i = 0; i < recdat.bytelen; i ++) {
      recdat.data[i] = databuf[6 + i];
      // recdat.data[i] = (recdat.data[i] << 8 )| databuf[7 + i];
    }
  }
  else {
    ZERO(databuf);
    recnum = 0;
    // receive the wrong data
    return -1;
  }
  ZERO(databuf);
  recnum = 0;

  return 2;
}

void RTS::sendData(void) {
  if (snddat.head[0] == FHONE && snddat.head[1] == FHTWO && snddat.len >= 3) {
    databuf[0] = snddat.head[0];
    databuf[1] = snddat.head[1];
    databuf[2] = snddat.len;
    databuf[3] = snddat.command;
    // to write data to the register
    if (snddat.command == 0x80) {
      databuf[4] = snddat.addr;
      for (int i = 0;i < (snddat.len - 2); i++)
        databuf[5 + i] = snddat.data[i];
    }
    else if (snddat.len == 3 && snddat.command == 0x81) {
      // to read data from the register
      databuf[4] = snddat.addr;
      databuf[5] = snddat.bytelen;
    }
    else if (snddat.command == 0x82) {
      // to write data to the variate
      databuf[4] = snddat.addr >> 8;
      databuf[5] = snddat.addr & 0xFF;
      for (int i =0; i < (snddat.len - 3); i += 2) {
        databuf[6 + i] = snddat.data[i/2] >> 8;
        databuf[7 + i] = snddat.data[i/2] & 0xFF;
      }
    }
    else if (snddat.len == 4 && snddat.command == 0x83) {
      // to read data from the variate
      databuf[4] = snddat.addr >> 8;
      databuf[5] = snddat.addr & 0xFF;
      databuf[6] = snddat.bytelen;
    }
    for (int i = 0; i < (snddat.len + 3); i++)
      LCD_SERIAL.write(databuf[i]);

    ZERO(snddat);
    ZERO(databuf);
    snddat.head[0] = FHONE;
    snddat.head[1] = FHTWO;
  }
}

void RTS::sendData(const String &s, unsigned long addr, unsigned char cmd/*=VarAddr_W*/) {
  if (s.length() < 1) return;
  sendData(s.c_str(), addr, cmd);
}

void RTS::sendData(const char *str, unsigned long addr, unsigned char cmd/*= VarAddr_W*/) {
  int len = strlen(str);
  if (len > 0) {
    databuf[0] = FHONE;
    databuf[1] = FHTWO;
    databuf[2] = 3 + len;
    databuf[3] = cmd;
    databuf[4] = addr >> 8;
    databuf[5] = addr & 0x00FF;
    for (int i = 0; i < len; i++)
      databuf[6 + i] = str[i];

    for (int i = 0;i < (len + 6); i++)
      LCD_SERIAL.write(databuf[i]);

    ZERO(databuf);
  }
}

void RTS::sendData(char c, unsigned long addr, unsigned char cmd/*=VarAddr_W*/) {
  snddat.command = cmd;
  snddat.addr = addr;
  snddat.data[0] = (unsigned long)c;
  snddat.data[0] = snddat.data[0] << 8;
  snddat.len = 5;
  sendData();
}

void RTS::sendData(unsigned char *str, unsigned long addr, unsigned char cmd) { sendData((char *)str, addr, cmd); }

void RTS::sendData(int n, unsigned long addr, unsigned char cmd/*=VarAddr_W*/) {
  if (cmd == VarAddr_W) {
    if (n > 0xFFFF) {
      snddat.data[0] = n >> 16;
      snddat.data[1] = n & 0xFFFF;
      snddat.len = 7;
    }
    else {
      snddat.data[0] = n;
      snddat.len = 5;
    }
  }
  else if (cmd == RegAddr_W) {
    snddat.data[0] = n;
    snddat.len = 3;
  }
  else if (cmd == VarAddr_R) {
    snddat.bytelen = n;
    snddat.len = 4;
  }
  snddat.command = cmd;
  snddat.addr = addr;
  sendData();
}

void RTS::sendData(unsigned int n, unsigned long addr, unsigned char cmd) { sendData((int)n, addr, cmd); }

void RTS::sendData(float n, unsigned long addr, unsigned char cmd) { sendData((int)n, addr, cmd); }

void RTS::sendData(long n, unsigned long addr, unsigned char cmd) { sendData((unsigned long)n, addr, cmd); }

void RTS::sendData(unsigned long n, unsigned long addr, unsigned char cmd/*=VarAddr_W*/) {
  if (cmd == VarAddr_W) {
    if (n > 0xFFFF) {
      snddat.data[0] = n >> 16;
      snddat.data[1] = n & 0xFFFF;
      snddat.len = 7;
    }
    else {
      snddat.data[0] = n;
      snddat.len = 5;
    }
  }
  else if (cmd == VarAddr_R) {
    snddat.bytelen = n;
    snddat.len = 4;
  }
  snddat.command = cmd;
  snddat.addr = addr;
  sendData();
}

void RTS::sdCardStop() {
  waitway = 7;
  change_page_number = 1;
  card.flag.abort_sd_printing = true;

  IF_DISABLED(SD_ABORT_NO_COOLDOWN, thermalManager.disable_all_heaters());
  print_job_timer.reset();

  thermalManager.setTargetHotend(0, 0);
  thermalManager.setTargetBed(0);
  updateTempE0();
  updateTempBed();
  thermalManager.zero_fan_speeds();
  wait_for_heatup = wait_for_user = false;
  poweroff_continue = false;
  #if BOTH(SDSUPPORT, POWER_LOSS_RECOVERY)
    if (CardReader::flag.mounted) card.removeJobRecoveryFile();
  #endif
  #ifdef EVENT_GCODE_SD_STOP
    queue.inject(F(EVENT_GCODE_SD_STOP));
  #endif

  // shut down the stepper motor.
  //queue.enqueue_now(F("M84"));
  sendData(0, MOTOR_FREE_ICON_VP);

  sendData(0, PRINT_PROCESS_ICON_VP);
  sendData(0, PRINT_PROCESS_VP);
  delay(2);
  for (int j = 0; j < 20; j++) {
    sendData(0, PRINT_FILE_TEXT_VP + j);  // clean screen.
    sendData(0, SELECT_FILE_TEXT_VP + j); // clean filename
  }
}

void RTS::handleData() {
  int Checkkey = -1;
  // for waiting
  if (waitway > 0) {
    memset(&recdat, 0, sizeof(recdat));
    recdat.head[0] = FHONE;
    recdat.head[1] = FHTWO;
    return;
  }
  for (int i = 0; Addrbuf[i] != 0; i++) {
    if (recdat.addr == Addrbuf[i]) {
      if (Addrbuf[i] >= ChangePageKey) Checkkey = i;
      break;
    }
  }

  if (Checkkey < 0) {
    ZERO(recdat);
    recdat.head[0] = FHONE;
    recdat.head[1] = FHTWO;
    return;
  }

  switch (Checkkey) {
    case MainPageKey: //首页
      if (recdat.data[0] == 1) { //选择打印文件
        CardUpdate = true;
        CardRecbuf.recordcount = -1;
        if (CardReader::flag.mounted) {
          for (int j = 0; j < 20; j ++) sendData(0, SELECT_FILE_TEXT_VP + j);
          gotoPage(2, 57);
        }
        else
          gotoPage(47, 102);
      }
      else if (recdat.data[0] == 2) { //完成打印
        waitway = 7;
        card.flag.abort_sd_printing = true;
        quickstop_stepper();
        print_job_timer.reset();
        queue.clear();
        sendData(0, MOTOR_FREE_ICON_VP);
        sendData(0, PRINT_PROCESS_ICON_VP);
        sendData(0, PRINT_PROCESS_VP);
        delay(2);
        sendData(0, PRINT_TIME_HOUR_VP);
        sendData(0, PRINT_TIME_MIN_VP);
        sendData(0, PRINT_SURPLUS_TIME_HOUR_VP);
        sendData(0, PRINT_SURPLUS_TIME_MIN_VP);

        change_page_number = 1;
      }
      else if (recdat.data[0] == 3) { //进入调平界面
        waitway = 6;
        queue.enqueue_now(F("G28\nG1 F200 Z0.0"));
        updateFan0();
        sendData(1, Wait_VP);
        gotoPage(32, 87);
      }
      else if (recdat.data[0] == 4) { //进入设置界面
        gotoPage(21, 76);
      }
      else if (recdat.data[0] == 5) { //进入温度界面
        updateFan0();
        gotoPage(15, 70);
      }
      else if (recdat.data[0] == 6) { // Light mode
        dark_mode = false;
        BL24CXX::writeOneByte(FONT_EEPROM, dark_mode);
        gotoPage(56);
        change_page_number = 56;
        settings.save();
      }
      else if (recdat.data[0] == 7) { // Dark mode
        dark_mode = true;
        BL24CXX::writeOneByte(FONT_EEPROM, dark_mode);
        gotoPage(1);
        change_page_number = 1;
        settings.save();
      }
      break;

    case AdjustmentKey:  //调整界面
      if (recdat.data[0] == 1) { //进入调整界面
        updateFan0();
        sendData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
        gotoPage(28, 83);
      }
      else if (recdat.data[0] == 2) { //返回打印界面
        if (start_print_flag) {
          gotoPage(10, 65);
        }
        else if (!sdcard_pause_check) {
          gotoPage(12, 67);
        }
        else {
          refreshTime();
          start_print_flag = false;
        }
      }
      else if (recdat.data[0] == 4) {
        updateTempE0();
        gcode.process_subcommands_now(F("M600"));
      }
      break;

    case Fan_speed: // 设置风扇速度
      fan_speed = recdat.data[0];
      thermalManager.set_fan_speed(0, fan_speed);
      updateFan0();
      break;

    case PrintSpeedKey: // 设置打印速度
      feedrate_percentage = recdat.data[0];
      sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);
      break;

    case StopPrintKey: // 停止打印
      if ((recdat.data[0] == 1) || (recdat.data[0] == 0xF1)) {
        sendData(1, Wait_VP);
        gotoPage(40, 95);
        sendData(0, PRINT_TIME_HOUR_VP);
        sendData(0, PRINT_TIME_MIN_VP);
        sendData(0, PRINT_SURPLUS_TIME_HOUR_VP);
        sendData(0, PRINT_SURPLUS_TIME_MIN_VP);
        sdCardStop();
        print_flag = 0;
        Update_Time_Value = 0;
      }
      else if (recdat.data[0] == 0xF0) {
        if (start_print_flag) {
          gotoPage(10, 65);
        }
        else if (!sdcard_pause_check) {
          gotoPage(12, 67);
        }
        else {
          refreshTime();
          start_print_flag = false;
        }
      }
      break;

    case PausePrintKey: // 暂停打印
      if (recdat.data[0] == 0xF0) break;

      if (recdat.data[0] == 0xF1) {
        sendData(1, Wait_VP);
        gotoPage(40, 95);
        // reject to receive cmd
        change_page_number = 12;
        waitway = 1;
        card.pauseSDPrint();
        pause_action_flag = true;
        pause_flag = true;
        Update_Time_Value = 0;
        sdcard_pause_check = false;
        print_flag = 1;
      }
      break;

    case ResumePrintKey: // 恢复打印
      if (recdat.data[0] == 1) { // 暂停恢复打印
        if (TERN0(CHECKFILAMENT, runout.filament_ran_out)) gotoPage(39, 94);
        //sendData(1, Wait_VP);
        //gotoPage(40, 95);
        card.startOrResumeFilePrinting();
        Update_Time_Value = 0;
        sdcard_pause_check = true;
        pause_action_flag = false;
        pause_flag = false;
        refreshTime();
        print_flag = 2;
      }
      else if (recdat.data[0] == 2) {
        #if ENABLED(CHECKFILAMENT)
          if (runout.filament_ran_out) {
            gotoPage(39, 94);
          }
          else {
            sendData(1, Wait_VP);
            gotoPage(40, 95);
            card.startOrResumeFilePrinting();
            print_job_timer.start();

            Update_Time_Value = 0;
            pause_action_flag = false;
            sdcard_pause_check = true;
            refreshTime();
            print_flag = 2;
          }
        #endif
      }
      else if (recdat.data[0] == 3) { // 断电续打恢复打印
        if (poweroff_continue) {
          #if ENABLED(CHECKFILAMENT)
            sendData(runout.filament_ran_out ? 0 : 1, CHANGE_FILAMENT_ICON_VP);
            gotoPage(8, 63);
          #endif
        }
        else if (poweroff_continue == false) {
          if (TERN0(CHECKFILAMENT, runout.filament_ran_out)) { gotoPage(39, 94); break; }

          char cmd[30];
          char *c;
          sprintf_P(cmd, PSTR("M23 %s"), CardRecbuf.Cardfilename[FilenamesCount]);
          for (c = &cmd[4]; *c; c++) *c = tolower(*c);
          queue.enqueue_one_now(cmd);
          delay(20);
          queue.enqueue_now(F("M24"));

          // clean screen.
          for (int j = 0; j < 20; j ++) sendData(0, PRINT_FILE_TEXT_VP + j);

          sendData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);
          delay(2);
          TERN_(BABYSTEPPING, sendData(0, AUTO_BED_LEVEL_ZOFFSET_VP));
          feedrate_percentage = 100;
          sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);
          zprobe_zoffset = last_zoffset;
          sendData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
          poweroff_continue = true;
          gotoPage(10, 65);
          sdcard_pause_check = true;
        }
      }
      else if (recdat.data[0] == 4) { //拔卡恢复打印
        if (!CardReader::flag.mounted) {
          CardUpdate = true;
          sdCardUpdate();
          gotoPage(46, 101);
        }
        else {
          if (TERN0(CHECKFILAMENT, runout.filament_ran_out)) gotoPage(39, 94);
          sendData(1, Wait_VP);
          gotoPage(40, 95);
          card.startOrResumeFilePrinting();
          Update_Time_Value = 0;
          sdcard_pause_check = true;
          pause_action_flag = false;
          print_flag = 2;
          for (uint16_t i = 0;i < CardRecbuf.Filesum; i++)
            if (!strcmp(CardRecbuf.Cardfilename[i], &recovery.info.sd_filename[1]))
              rts.sendData(CardRecbuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);

          refreshTime();
        }
      }
      break;

    case CoolScreenKey: // 温度界面
      if (recdat.data[0] == 1) { // PLA 预热
        thermalManager.temp_hotend[0].target = PREHEAT_1_TEMP_HOTEND;
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        updateTempE0();
        thermalManager.temp_bed.target = PREHEAT_1_TEMP_BED;
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        updateTempBed();
      }
      else if (recdat.data[0] == 2) { // ABS预热
        thermalManager.temp_hotend[0].target = PREHEAT_2_TEMP_HOTEND;
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        updateTempE0();
        thermalManager.temp_bed.target = PREHEAT_2_TEMP_BED;
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        updateTempBed();
      }
      else if (recdat.data[0] == 3) { // 返回主界面
        gotoPage(1, 56);
      }
      else if (recdat.data[0] == 0xF1) { // 冷却
        #if FAN_COUNT > 0
          FANS_LOOP(i) thermalManager.set_fan_speed(i, 255);
        #endif
        updateFan0();
        thermalManager.setTargetHotend(0, 0);
        updateTempE0(); delay(1);

        #if HAS_HEATED_BED
          thermalManager.setTargetBed(0);
          updateTempBed(); delay(1);
        #endif

        gotoPage(15, 70);
      }
      else if (recdat.data[0] == 0xF0) {
        gotoPage(15, 70);
      }
      break;

    case Heater0TempEnterKey:
      thermalManager.temp_hotend[0].target = recdat.data[0];
      thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
      updateTempE0();
      break;

    case HotBedTempEnterKey:
      thermalManager.temp_bed.target = recdat.data[0];
      thermalManager.setTargetBed(thermalManager.temp_bed.target);
      updateTempBed();
      break;

    case Heater0LoadEnterKey:
      Filament0LOAD = ((float)recdat.data[0]) / 10;
      break;

    case AxisPageSelectKey: // 移动轴界面
      if (recdat.data[0] == 1) {
        AxisUnitMode = 1;
        axis_unit = 10.0;
        gotoPage(29, 84);
      }
      else if (recdat.data[0] == 2) {
        AxisUnitMode = 2;
        axis_unit = 1.0;
        gotoPage(30, 85);
      }
      else if (recdat.data[0] == 3) {
        AxisUnitMode = 3;
        axis_unit = 0.1;
        gotoPage(31, 86);
      }
      else if (recdat.data[0] == 4) {
        waitway = 4;
        queue.enqueue_now(F("G28"));
        Update_Time_Value = 0;
        sendData(1, Wait_VP);
        gotoPage(32, 87);
        sendData(0, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 5) { // 解锁电机
        queue.enqueue_now(F("M84"));
        sendData(1, MOTOR_FREE_ICON_VP);
      }
      break;

    case SettingScreenKey: // 设置界面
      if (recdat.data[0] == 2) { // 进入换料界面
        Filament0LOAD = 10;
        sendData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        updateTempE0();

        delay(2);
        gotoPage(23, 78);
      }
      else if (recdat.data[0] == 3) { // 进入移动界面
        AxisUnitMode = 1;
        TERN_(HAS_X_AXIS, sendData(10 * current_position.x, AXIS_X_COORD_VP));
        TERN_(HAS_Y_AXIS, sendData(10 * current_position.y, AXIS_Y_COORD_VP));
        TERN_(HAS_Z_AXIS, sendData(10 * current_position.z, AXIS_Z_COORD_VP));
        gotoPage(29, 84);
      }
      else if (recdat.data[0] == 4) { // 进入高级设置界面
        sendData(planner.extruder_advance_K[0] * 100, Advance_K_VP);
        gotoPage(49, 104);
      }
      else if (recdat.data[0] == 5) { // 信息
        char sizebuf[20] = {0};
        sprintf(sizebuf, "%d X %d X %d", X_MAX_POS, Y_MAX_POS, Z_MAX_POS);
        sendData(MACVERSION, PRINTER_MACHINE_TEXT_VP);
        sendData(sizebuf, PRINTER_PRINTSIZE_TEXT_VP);
        sendData(SOFTVERSION, PRINTER_VERSION_TEXT_VP);
        sendData(MARLINVERSION, MARLIN_VERSION_TEXT_VP);
        sendData(CORP_WEBSITE_E, PRINTER_WEBSITE_TEXT_VP);
        gotoPage(33, 88);
      }
      else if (recdat.data[0] == 7) { // 返回
        gotoPage(1, 56);
      }
      break;

    case SettingBackKey: // 调节界面返回按键
      if (recdat.data[0] == 1) {
        Update_Time_Value = RTS_UPDATE_VALUE;
        //settings.save();
        gotoPage(1, 56);
      }
      else if (recdat.data[0] == 2) {
        if (!planner.has_blocks_queued()) {
          gotoPage(111, 117);
        }
      }
      break;

    case BedLevelFunKey: // 调平界面
      if (recdat.data[0] == 1) { // 保存按钮
        settings.save();
        waitway = 6;
        queue.enqueue_now(F("G28 Z"));
        queue.enqueue_now(F("G1 F200 Z0.0"));
      }
      else if (recdat.data[0] == 4) {
        queue.enqueue_now(F("G34"));
        queue.enqueue_now(F("G1 F200 Z0.0"));
      }
      else if (recdat.data[0] == 5) {
        #if ENABLED(FIX_MOUNTED_PROBE)
          waitway = 3;
          sendData(1, AUTO_BED_LEVEL_ICON_VP);
          queue.enqueue_now(F("G29"));
        #endif
      }

      if (recdat.data[0] == 11) settings.save(); // 调整界面保存

      sendData(0, MOTOR_FREE_ICON_VP);
      break;

    #if HAS_X_AXIS
      case XaxismoveKey: {
        waitway = 4;
        constexpr float x_min = X_MIN_POS, x_max = X_MAX_POS;
        current_position.x = float(recdat.data[0] >= 32768 ? recdat.data[0] - 65536 : recdat.data[0]) / 10.0f;
        LIMIT(current_position.x, x_min, x_max);
        RTS_line_to_current(X_AXIS);
        sendData(10 * current_position.x, AXIS_X_COORD_VP);
        sendData(0, MOTOR_FREE_ICON_VP);
        waitway = 0;
      } break;
    #endif

    #if HAS_Y_AXIS
      case YaxismoveKey: {
        waitway = 4;
        constexpr float y_min = Y_MIN_POS, y_max = Y_MAX_POS;
        current_position.y = float(recdat.data[0]) / 10.0f;
        LIMIT(current_position.y, y_min, y_max);
        RTS_line_to_current(Y_AXIS);
        sendData(10 * current_position.y, AXIS_Y_COORD_VP);
        sendData(0, MOTOR_FREE_ICON_VP);
        waitway = 0;
      } break;
    #endif

    #if HAS_Z_AXIS
      case ZaxismoveKey: {
        waitway = 4;
        constexpr float z_min = Z_MIN_POS, z_max = Z_MAX_POS;
        current_position.z = ((float)recdat.data[0]) / 10;
        if (current_position.z < z_min) {
          current_position.z = z_min;
        }
        else if (current_position.z > z_max) {
          current_position.z = z_max;
        }
        RTS_line_to_current(Z_AXIS);
        sendData(10 * current_position.z, AXIS_Z_COORD_VP);
        sendData(0, MOTOR_FREE_ICON_VP);
        waitway = 0;
      } break;
    #endif

    case FilamentLoadKey: // 换料
      if (recdat.data[0] == 1) {
        if (!planner.has_blocks_queued()) {
          if (TERN0(CHECKFILAMENT, runout.filament_ran_out)) gotoPage(20, 75);
          current_position[E_AXIS] -= Filament0LOAD;

          if (thermalManager.temp_hotend[0].celsius < (ChangeFilament0Temp - 5)) {
            sendData((int)ChangeFilament0Temp, CHANGE_FILAMENT0_TEMP_VP);
            gotoPage(24, 79);
          }
          else {
            RTS_line_to_current(E_AXIS);
            sendData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
          }
        }
      }
      else if (recdat.data[0] == 2) {
        if (!planner.has_blocks_queued()) {
          if (TERN0(CHECKFILAMENT, runout.filament_ran_out)) gotoPage(20, 75);
          current_position[E_AXIS] += Filament0LOAD;

          if (thermalManager.temp_hotend[0].celsius < (ChangeFilament0Temp - 5)) {
            sendData((int)ChangeFilament0Temp, CHANGE_FILAMENT0_TEMP_VP);
            gotoPage(24, 79);
          }
          else {
            RTS_line_to_current(E_AXIS);
            sendData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
          }
        }
      }
      else if (recdat.data[0] == 3) {
        updateTempE0();
        sendData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
        gotoPage(27, 82);
      }
      else if (recdat.data[0] == 5) {
        if (!planner.has_blocks_queued()) {
          thermalManager.setTargetHotend(ChangeFilament0Temp, 0);
          updateTempE0();
          gotoPage(26, 81);
          heatway = 1;
        }
      }
      else if (recdat.data[0] == 6) {
        if (!planner.has_blocks_queued()) {
          Filament0LOAD = 10;
          sendData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
          gotoPage(23, 78);
        }
      }
      else if (recdat.data[0] == 4) {
        if (!planner.has_blocks_queued()) {
          thermalManager.temp_hotend[0].target = 0;
          updateTempE0();
          gotoPage(23, 78);
          Filament0LOAD = 10;
          sendData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
          break;
        }
      }
      else if (recdat.data[0] == 8) {
        if (!planner.has_blocks_queued()) {
          thermalManager.setTargetHotend(ChangeFilament0Temp, 0);
          updateTempE0();
          gotoPage(26, 81);
          heatway = 1;
        }
        break;
      }
    break;

    case FilamentCheckKey:
      if (recdat.data[0] == 1) {
        TERN_(CHECKFILAMENT, gotoPage(runout.filament_ran_out ? 20 : 23, runout.filament_ran_out ? 75 : 78));
      }
      else if (recdat.data[0] == 2) {
        gotoPage(21, 76);
        Filament0LOAD = 10;
      }
    break;

    case PowerContinuePrintKey: // 断电续打
      if (recdat.data[0] == 1) {
        if (poweroff_continue) {
          power_off_type_yes = true;
          Update_Time_Value = 0;
          gotoPage(10, 65);
          queue.enqueue_now(F("M1000"));

          poweroff_continue = true;
          sdcard_pause_check = true;
          zprobe_zoffset = probe.offset.z;
          sendData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
          sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);
          print_flag = 2;
        }
      }
      else if (recdat.data[0] == 2) {
        Update_Time_Value = RTS_UPDATE_VALUE;
        gotoPage(1, 56);
        poweroff_continue = false;
        sdcard_pause_check = true;
        queue.clear();
        quickstop_stepper();
        print_job_timer.abort();
        thermalManager.disable_all_heaters();
        print_job_timer.reset();

        if (CardReader::flag.mounted) {
          #if BOTH(SDSUPPORT, POWER_LOSS_RECOVERY)
            card.removeJobRecoveryFile();
            recovery.info.valid_head = 0;
            recovery.info.valid_foot = 0;
            recovery.close();
          #endif
        }

        wait_for_heatup = wait_for_user = false;
        print_flag = 0;
      }
    break;

    case SelectFileKey:
      if (sdDetected()) {
        if (recdat.data[0] > CardRecbuf.Filesum) break;

        CardRecbuf.recordcount = recdat.data[0] - 1;
        for (int j = 0; j < 10; j ++) {
          sendData(0, SELECT_FILE_TEXT_VP + j);
          sendData(0, PRINT_FILE_TEXT_VP + j);
        }
        sendData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], SELECT_FILE_TEXT_VP);
        sendData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);
        delay(2);
        for (int j = 1;j <= CardRecbuf.Filesum;j ++) {
          sendData((unsigned long)0x738E, FilenameNature + j * 16);
        }
        sendData((unsigned long)0x041F, FilenameNature + recdat.data[0] * 16);
        sendData(1, FILE1_SELECT_ICON_VP + (recdat.data[0] - 1));
      }
      break;

    case PrintFileKey: // 打印文件列表
      if (recdat.data[0] == 1 && sdDetected()) {
        if (CardRecbuf.recordcount < 0) break;
        char cmd[30];
        char *c;
        sprintf_P(cmd, PSTR("M23 %s"), CardRecbuf.Cardfilename[CardRecbuf.recordcount]);
        for (c = &cmd[4]; *c; c++) *c = tolower(*c);

        ZERO(cmdbuf);
        strcpy(cmdbuf, cmd);
        FilenamesCount = CardRecbuf.recordcount;
        #if ENABLED(CHECKFILAMENT)
          if (runout.filament_ran_out) {
            gotoPage(39, 94);
            sdcard_pause_check = false;
            break;
          }
        #endif
        queue.enqueue_one_now(cmd);
        delay(20);
        queue.enqueue_now(F("M24"));
        // clean screen.
        for (int j = 0; j < 20; j ++) sendData(0, PRINT_FILE_TEXT_VP + j);

        sendData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);

        delay(2);

        TERN_(BABYSTEPPING, sendData(0, AUTO_BED_LEVEL_ZOFFSET_VP));

        feedrate_percentage = 100;
        sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);
        zprobe_zoffset = last_zoffset;
        sendData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
        poweroff_continue = true;
        gotoPage(10, 65);
        change_page_number = 11;
        Update_Time_Value = 0;
        start_print_flag = true;
        print_flag = 2;
      }
      else if (recdat.data[0] == 2)
        gotoPage(3, 58);
      else if (recdat.data[0] == 3)
        gotoPage(2, 57);
      else if (recdat.data[0] == 4)
        gotoPage(4, 59);
      else if (recdat.data[0] == 5)
        gotoPage(3, 58);
      else if (recdat.data[0] == 6)
        gotoPage(5, 60);
      else if (recdat.data[0] == 7)
        gotoPage(4, 59);
      else if (recdat.data[0] == 8)
        gotoPage(1, 56);
      break;

    case StoreMemoryKey: // 初始化
      if (recdat.data[0] == 0xF1) {
        settings.init_eeprom();
        #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
          bool zig = true;
          int8_t inStart, inStop, inInc, showcount;
          showcount = 0;
          for (int y = 0; y < GRID_MAX_POINTS_Y; y++) {
            // away from origin
            if (zig) {
              inStart = 0;
              inStop = GRID_MAX_POINTS_X;
              inInc = 1;
            }
            else {
              // towards origin
              inStart = GRID_MAX_POINTS_X - 1;
              inStop = -1;
              inInc = -1;
            }
            zig ^= true;
            for (int x = inStart; x != inStop; x += inInc) {
              sendData(bedlevel.z_values[x][y] * 100, AUTO_BED_LEVEL_1POINT_VP + showcount * 2);
              showcount++;
            }
          }
        #endif
        rts.sendData(Beep, SoundAddr);
        zprobe_zoffset = 0;
        last_zoffset = 0;
        sendData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
        gotoPage(1, 56);
      }
      else if (recdat.data[0] == 0xF0) {
        gotoPage(21, 76);
      }
      break;

    case AdvancedKey: // 高级设置界面
      if (recdat.data[0] == 1) { //PID
        float hot_p,hot_i,hot_d,bed_p,bed_i,bed_d;
        hot_p = thermalManager.temp_hotend[0].pid.p() * 100.0f;
        hot_i = (thermalManager.temp_hotend[0].pid.i() / 8.0f * 10000.0f) + 0.00001f;
        hot_d = thermalManager.temp_hotend[0].pid.d() * 8.0f;

        bed_p = thermalManager.temp_bed.pid.Kp * 100.0f;
        bed_i = (thermalManager.temp_bed.pid.Ki / 8.0f * 10000.0f) + 0.0001f;
        bed_d = thermalManager.temp_bed.pid.Kd * 0.8f;

        sendData(hot_p, Nozzle_P_VP);
        sendData(hot_i, Nozzle_I_VP);
        sendData(hot_d, Nozzle_D_VP);
        sendData(bed_p, Hot_Bed_P_VP);
        sendData(bed_i, Hot_Bed_I_VP);
        sendData(bed_d, Hot_Bed_D_VP);
        gotoPage(41, 96);
      }
      else if (recdat.data[0] == 2) { // 速度
        sendData(planner.settings.max_feedrate_mm_s[X_AXIS], Vmax_X_VP);
        sendData(planner.settings.max_feedrate_mm_s[Y_AXIS], Vmax_Y_VP);
        sendData(planner.settings.max_feedrate_mm_s[Z_AXIS], Vmax_Z_VP);
        sendData(planner.settings.max_feedrate_mm_s[E_AXIS], Vmax_E_VP);
        gotoPage(25, 80);
      }
      else if (recdat.data[0] == 3) { // 加速度
        sendData(planner.settings.acceleration, Accel_VP);
        sendData(planner.settings.retract_acceleration, A_Retract_VP);
        sendData(planner.settings.travel_acceleration, A_Travel_VP);
        sendData(planner.settings.max_acceleration_mm_per_s2[X_AXIS], Amax_X_VP);
        sendData(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], Amax_Y_VP);
        sendData(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], Amax_Z_VP);
        sendData(planner.settings.max_acceleration_mm_per_s2[E_AXIS], Amax_E_VP);
        gotoPage(34, 89);
      }
      else if (recdat.data[0] == 4) { // Jrek
        sendData(planner.max_jerk.x * 10.0f, Jerk_X_VP);
        sendData(planner.max_jerk.y * 10.0f, Jerk_Y_VP);
        sendData(planner.max_jerk.z * 10.0f, Jerk_Z_VP);
        sendData(planner.max_jerk.e * 10.0f, Jerk_E_VP);
        gotoPage(35, 90);
      }
      else if (recdat.data[0] == 5) { // Steps
        sendData(planner.settings.axis_steps_per_mm[X_AXIS] * 10.0f, Steps_X_VP);
        sendData(planner.settings.axis_steps_per_mm[Y_AXIS] * 10.0f, Steps_Y_VP);
        sendData(planner.settings.axis_steps_per_mm[Z_AXIS] * 10.0f, Steps_Z_VP);
        sendData(planner.settings.axis_steps_per_mm[E_AXIS] * 10.0f, Steps_E_VP);
        gotoPage(37, 92);
      }
      else if (recdat.data[0] == 6) { // 返回
        gotoPage(21, 76);
      }
      else if (recdat.data[0] == 7) { // 确认
        sendData(planner.extruder_advance_K[0] * 100, Advance_K_VP);
        gotoPage(18, 73);
      }
      else if (recdat.data[0] == 8) { // TMC
        gotoPage(113, 119);
      }
      else if (recdat.data[0] == 9) { // 保存
        settings.save();
      }
      break;
    case Nozzle_P:
      SET_HOTEND_PID(Kp, 0, float(recdat.data[0]) / 100.0f);
      thermalManager.updatePID();
      break;
    case Nozzle_I:
      SET_HOTEND_PID(Ki, 0, float(recdat.data[0]) * 8.0f / 10000.0f);
      thermalManager.updatePID();
      break;
    case Nozzle_D:
      SET_HOTEND_PID(Kd, 0, float(recdat.data[0]) / 8.0f);
      thermalManager.updatePID();
      break;
    case Hot_Bed_P:
      thermalManager.temp_bed.pid.Kp = float(recdat.data[0]) / 100.0f;
      thermalManager.updatePID();
      break;
    case Hot_Bed_I:
      thermalManager.temp_bed.pid.Ki = float(recdat.data[0]) * 8.0f / 10000.0f;
      thermalManager.updatePID();
      break;
    case Hot_Bed_D:
      thermalManager.temp_bed.pid.Kd = float(recdat.data[0]) / 0.8f;
      thermalManager.updatePID();
      break;
    case Vmax_X:
      planner.settings.max_feedrate_mm_s[X_AXIS] = recdat.data[0];
      break;
    case Vmax_Y:
      planner.settings.max_feedrate_mm_s[Y_AXIS] = recdat.data[0];
      break;
    case Vmax_Z:
      planner.settings.max_feedrate_mm_s[Z_AXIS] = recdat.data[0];
      break;
    case Vmax_E:
      planner.settings.max_feedrate_mm_s[E_AXIS] = recdat.data[0];
      break;
    case Accel:
      planner.settings.acceleration = recdat.data[0];
      break;
    case A_Retract:
      planner.settings.retract_acceleration = recdat.data[0];
      break;
    case A_Travel:
      planner.settings.travel_acceleration = recdat.data[0];
      break;
    case Amax_X:
      planner.settings.max_acceleration_mm_per_s2[X_AXIS] = recdat.data[0];
      break;
    case Amax_Y:
      planner.settings.max_acceleration_mm_per_s2[Y_AXIS] = recdat.data[0];
      break;
    case Amax_Z:
      planner.settings.max_acceleration_mm_per_s2[Z_AXIS] = recdat.data[0];
      break;
    case Amax_E:
      planner.settings.max_acceleration_mm_per_s2[E_AXIS] = recdat.data[0];
      break;
    case Jerk_X:
      planner.max_jerk[X_AXIS] = float(recdat.data[0]) / 10.0f;
      break;
    case Jerk_Y:
      planner.max_jerk[Y_AXIS] = float(recdat.data[0]) / 10.0f;
      break;
    case Jerk_Z:
      planner.max_jerk[Z_AXIS] = float(recdat.data[0]) / 10.0f;
      break;
    case Jerk_E:
      planner.max_jerk[E_AXIS] = float(recdat.data[0]) / 10.0f;
      break;
    case Steps_X:
      planner.settings.axis_steps_per_mm[X_AXIS] = float(recdat.data[0]) / 10.0f;
      break;
    case Steps_Y:
      planner.settings.axis_steps_per_mm[Y_AXIS] = float(recdat.data[0]) / 10.0f;
      break;
    case Steps_Z:
      planner.settings.axis_steps_per_mm[Z_AXIS] = float(recdat.data[0]) / 10.0f;
      break;
    case Steps_E:
      planner.settings.axis_steps_per_mm[E_AXIS] = float(recdat.data[0]) / 10.0f;
      break;
    case AdvancedBackKey:
      gotoPage(18, 73);
      break;
    case Advance_K:
      planner.extruder_advance_K[0] = float(recdat.data[0]) / 100.0f;
      break;

    case FilamentChange: // 自动换料
      if ((recdat.data[0] == 1 && !runout.filament_ran_out) || recdat.data[0] == 2) {
        updateTempE0();
        wait_for_heatup = wait_for_user = false;
      }
      else if (recdat.data[0] == 3) {
        pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;
      }
      else if (recdat.data[0] == 4) {
        pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;
      }
      break;

    case ZoffsetUnitKey: // Zoffset unit
      if (recdat.data[0] == 1)
        gotoPage(28, 83);
      else if (recdat.data[0] == 2)
        gotoPage(14, 69);
      if (recdat.data[0] == 3)
        gotoPage(111, 117);
      else if (recdat.data[0] == 4)
        gotoPage(22, 77);
      break;

    case ZOffsetKey:
        last_zoffset = zprobe_zoffset;
        zprobe_zoffset = float(recdat.data[0] >= 32767 ? recdat.data[0] - 65537 : recdat.data[0]) / 100.0f + 0.0001f;
        if (WITHIN(zprobe_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
          babystep.add_mm(Z_AXIS, zprobe_zoffset - last_zoffset);
        probe.offset.z = zprobe_zoffset;
        sendData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
      break;

    case TMCDriver:
      if (recdat.data[0] == 1) { // current
        sendData(stepperX.getMilliamps(), Current_X_VP);
        sendData(stepperY.getMilliamps(), Current_Y_VP);
        sendData(stepperZ.getMilliamps(), Current_Z_VP);
        sendData(stepperE0.getMilliamps(), Current_E_VP);
        gotoPage(114, 120);
      }
      else if (recdat.data[0] == 2) { // threshold
        sendData(stepperX.get_pwm_thrs(), Threshold_X_VP);
        sendData(stepperY.get_pwm_thrs(), Threshold_Y_VP);
        sendData(stepperZ.get_pwm_thrs(), Threshold_Z_VP);
        sendData(stepperE0.get_pwm_thrs(), Threshold_E_VP);
        gotoPage(115, 121);
      }
      else if (recdat.data[0] == 3) { // Sensorless
        sendData(stepperX.homing_threshold(), Sensorless_X_VP);
        sendData(stepperY.homing_threshold(), Sensorless_Y_VP);
        gotoPage(116, 122);
      }
      else if (recdat.data[0] == 4) {
        gotoPage(18, 73);
      }
      else if (recdat.data[0] == 5) {
        gotoPage(113, 119);
      }
      break;
    case Current_X:
      sprintf_P(cmd, PSTR("M906 X%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;
    case Current_Y:
      sprintf_P(cmd, PSTR("M906 Y%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;
    case Current_Z:
      sprintf_P(cmd, PSTR("M906 Z%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;
    case Current_E:
      sprintf_P(cmd, PSTR("M906 E0%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;
    case Threshold_X:
      sprintf_P(cmd, PSTR("M913 X%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;
    case Threshold_Y:
      sprintf_P(cmd, PSTR("M913 Y%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;
    case Threshold_Z:
      sprintf_P(cmd, PSTR("M913 Z%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;
    case Threshold_E:
      sprintf_P(cmd, PSTR("M913 E0%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;
    case Sensorless_X:
      sprintf_P(cmd, PSTR("M914 X%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;
    case Sensorless_Y:
      sprintf_P(cmd, PSTR("M914 Y%i"), recdat.data[0]);
      queue.enqueue_now(cmd);
      break;

    case ChangePageKey:
      if (change_page_number == 36 || change_page_number == 76) break;

      if (change_page_number == 11) {
        refreshTime();
        start_print_flag = false;
      }
      else if (change_page_number == 12) {
        gotoPage(12, 67);
      }
      else if ((change_page_number != 12) && (change_page_number != 11) && card.isPrinting()) {
        for (uint16_t i = 0;i < CardRecbuf.Filesum; i++)
          if (!strcmp(CardRecbuf.Cardfilename[i], &recovery.info.sd_filename[1]))
            rts.sendData(CardRecbuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);

        refreshTime();
      }
      else
        sendData(change_page_number + ExchangePageBase, ExchangepageAddr);

      for (int i = 0; i < MaxFileNumber; i ++)
        for (int j = 0; j < 20; j ++)
          sendData(0, FILE1_TEXT_VP + i * 20 + j);

      for (int i = 0; i < CardRecbuf.Filesum; i++) {
        for (int j = 0; j < 20; j++) sendData(0, CardRecbuf.addr[i] + j);
        sendData((unsigned long)0x738E, FilenameNature + (i + 1) * 16);
      }

      for (int j = 0; j < 20; j ++) {
        // clean screen.
        sendData(0, PRINT_FILE_TEXT_VP + j);
        // clean filename
        sendData(0, SELECT_FILE_TEXT_VP + j);
      }

      // clean filename Icon
      for (int j = 0; j < 20; j ++)
        sendData(10, FILE1_SELECT_ICON_VP + j);

      sendData(CardRecbuf.Cardshowfilename[CardRecbuf.recordcount], PRINT_FILE_TEXT_VP);

      // represents to update file list
      if (CardUpdate && lcd_sd_status && IS_SD_INSERTED()) {
        for (uint16_t i = 0; i < CardRecbuf.Filesum; i++) {
          delay(3);
          sendData(CardRecbuf.Cardshowfilename[i], CardRecbuf.addr[i]);
          sendData((unsigned long)0x738E, FilenameNature + (i + 1) * 16);
          sendData(0, FILE1_SELECT_ICON_VP + i);
        }
      }

      char sizeBuf[20];
      sprintf(sizeBuf, "%d X %d X %d", X_BED_SIZE-10, Y_BED_SIZE, Z_MAX_POS);
      sendData(MACVERSION, PRINTER_MACHINE_TEXT_VP);
      sendData(SOFTVERSION, PRINTER_VERSION_TEXT_VP);
      sendData(sizeBuf, PRINTER_PRINTSIZE_TEXT_VP);

      sendData(CORP_WEBSITE_E, PRINTER_WEBSITE_TEXT_VP);

      updateFan0();
      Percentrecord = card.percentDone() + 1;
      if (Percentrecord <= 100)
        rts.sendData((unsigned char)Percentrecord, PRINT_PROCESS_ICON_VP);

      rts.sendData((unsigned char)card.percentDone(), PRINT_PROCESS_VP);

      sendData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);

      sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);
      updateTempE0();
      updateTempBed();

      break;

    default: break;
  }
  ZERO(recdat);
  recdat.head[0] = FHONE;
  recdat.head[1] = FHTWO;
}

void RTS::updateTempE0() {
  #if HAS_EXTRUDERS
    sendData(thermalManager.temp_hotend[0].celsius, HEAD0_CURRENT_TEMP_VP);
    sendData(thermalManager.temp_hotend[0].target, HEAD0_SET_TEMP_VP);
  #endif
}

void RTS::updateTempBed() {
  #if HAS_HEATED_BED
    sendData(thermalManager.temp_bed.celsius, BED_CURRENT_TEMP_VP);
    sendData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
  #endif
}

void RTS::updateFan0() {
  TERN_(HAS_FAN, sendData(thermalManager.fan_speed[0], FAN_SPEED_VP));
}

void EachMomentUpdate() {
  millis_t ms = millis();
  if (ms > next_rts_update_ms) {
    // print the file before the power is off.
    if (!power_off_type_yes && lcd_sd_status && poweroff_continue) {
      rts.sendData(ExchangePageBase, ExchangepageAddr);
      if (startprogress < 0) startprogress = 0;
      if (startprogress < 56) rts.sendData(startprogress, START1_PROCESS_ICON_VP);
      delay(80);
      if (++startprogress > 56) {
        rts.sendData(StartSoundSet, SoundAddr);
        power_off_type_yes = true;
        for (uint16_t i = 0; i < CardRecbuf.Filesum; i++) {
          if (!strcmp(CardRecbuf.Cardfilename[i], &recovery.info.sd_filename[1])) {
            rts.sendData(CardRecbuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);
            rts.gotoPage(36, 91);
            break;
          }
        }
        StartFlag = 1;
      }
      return;
    }
    else if (!power_off_type_yes && !poweroff_continue) {
      rts.sendData(ExchangePageBase, ExchangepageAddr);
      if (startprogress < 0)
        startprogress = 0;
      else if (startprogress < 56)
        rts.sendData(startprogress, START1_PROCESS_ICON_VP);
      delay(80);
      if (++startprogress > 56) {
        rts.sendData(StartSoundSet, SoundAddr);
        power_off_type_yes = true;
        Update_Time_Value = RTS_UPDATE_VALUE;
        change_page_number = dark_mode ? 1 : 56;
        rts.gotoPage(change_page_number);
      }
      return;
    }
    else {
      // need to optimize
      if (print_job_timer.duration() != 0) {
        duration_t elapsed = print_job_timer.duration();
        static unsigned char last_cardpercentValue = 100;
        rts.sendData(elapsed.value / 3600, PRINT_TIME_HOUR_VP);
        rts.sendData((elapsed.value % 3600) / 60, PRINT_TIME_MIN_VP);

        if (card.isPrinting() && (last_cardpercentValue != card.percentDone())) {
          if ((unsigned char)card.percentDone() > 0) {
            Percentrecord = card.percentDone();
            if (Percentrecord <= 100)
              rts.sendData((unsigned char)Percentrecord, PRINT_PROCESS_ICON_VP);
            // Estimate remaining time every 20 seconds
            static millis_t next_remain_time_update = 0;
            if (ELAPSED(ms, next_remain_time_update)) {
              if ((thermalManager.temp_hotend[0].celsius >= (thermalManager.temp_hotend[0].target - 5))) {
                remain_time = elapsed.value / (Percentrecord * 0.01f) - elapsed.value;
                next_remain_time_update += 20 * 1000UL;
                rts.sendData(remain_time / 3600, PRINT_SURPLUS_TIME_HOUR_VP);
                rts.sendData((remain_time % 3600) / 60, PRINT_SURPLUS_TIME_MIN_VP);
              }
            }
          }
          else {
            rts.sendData(0, PRINT_PROCESS_ICON_VP);
            rts.sendData(0, PRINT_SURPLUS_TIME_HOUR_VP);
            rts.sendData(0, PRINT_SURPLUS_TIME_MIN_VP);
          }
          rts.sendData((unsigned char)card.percentDone(), PRINT_PROCESS_VP);
          last_cardpercentValue = card.percentDone();
        }
      }

      if (pause_action_flag && !sdcard_pause_check && printingIsPaused() && !planner.has_blocks_queued()) {
        pause_action_flag = false;
        queue.enqueue_now(F("G0 F3000 X0 Y0"));
        queue.enqueue_now(F("M18 S0"));
      }

      rts.sendData(10 * current_position.z, AXIS_Z_COORD_VP);

      if ((last_target_temperature[0] != thermalManager.temp_hotend[0].target) || (last_target_temperature_bed != thermalManager.temp_bed.target)) {
        thermalManager.setTargetHotend(thermalManager.temp_hotend[0].target, 0);
        thermalManager.setTargetBed(thermalManager.temp_bed.target);
        last_target_temperature[0] = thermalManager.temp_hotend[0].target;
        last_target_temperature_bed = thermalManager.temp_bed.target;
      }

      rts.updateTempBed();
      rts.updateTempE0();

      if ((thermalManager.temp_hotend[0].celsius >= thermalManager.temp_hotend[0].target) && (heatway == 1)) {
        rts.gotoPage(23, 78);
        heatway = 0;
        rts.sendData(10 * Filament0LOAD, HEAD0_FILAMENT_LOAD_DATA_VP);
      }

      TERN_(CHECKFILAMENT, rts.sendData(!runout.filament_ran_out, CHANGE_FILAMENT_ICON_VP));
    }
    next_rts_update_ms = ms + RTS_UPDATE_INTERVAL + Update_Time_Value;
  }
}

// looping at the loop function
void RTS_Update() {
  // Check the status of card
  rts.sdCardUpdate();

  sd_printing = IS_SD_PRINTING();
  card_insert_st = IS_SD_INSERTED() ;

  if (!card_insert_st && sd_printing) {
    rts.gotoPage(46, 101);
    rts.sendData(0, CHANGE_SDCARD_ICON_VP);
    // 暂停打印，使得喷头可以回到零点
    card.pauseSDPrint();
    print_job_timer.pause();
    pause_action_flag = true;
    sdcard_pause_check = false;
  }
  // 更新拔卡和插卡提示图标
  if (last_card_insert_st != card_insert_st) {
    // 当前页面显示为拔卡提示页面，但卡已经插入了，更新插卡图标
    rts.sendData((int)card_insert_st, CHANGE_SDCARD_ICON_VP);
    last_card_insert_st = card_insert_st;
  }

  #if ENABLED(CHECKFILAMENT)
    // Checking filament status during printing
    if (card.isPrinting() && poweroff_continue) {
      if (runout.filament_ran_out) {
        Checkfilenum++;
        delay(5);
      }
      else {
        delay(5);
        if (runout.filament_ran_out)
          Checkfilenum++;
        else
          Checkfilenum = 0;
      }
      if (Checkfilenum > 10) {
        rts.sendData(Beep, SoundAddr);
        if ((thermalManager.temp_hotend[0].celsius <= (thermalManager.temp_hotend[0].target - 5))) {
          rts.gotoPage(39, 94);
          card.pauseSDPrint();
          print_job_timer.pause();
          pause_action_flag = true;
          Update_Time_Value = 0;
          sdcard_pause_check = false;
          print_flag = 1;
        }
        else if (thermalManager.temp_bed.celsius <= thermalManager.temp_bed.target) {
          rts.gotoPage(39, 94);
          card.pauseSDPrint();
          print_job_timer.pause();
          pause_action_flag = true;
          Checkfilenum = 0;
          Update_Time_Value = 0;
          sdcard_pause_check = false;
          print_flag = 1;
        }
        else {
          rts.sendData(1, Wait_VP);
          rts.gotoPage(40, 95);
          waitway = 5;

          TERN_(POWER_LOSS_RECOVERY, if (recovery.enabled) recovery.save(true, false));
          card.pauseSDPrint();
          print_job_timer.pause();
          pause_action_flag = true;
          Checkfilenum = 0;
          Update_Time_Value = 0;
          sdcard_pause_check = false;
          print_flag = 1;
        }
      }
    }
  #endif // CHECKFILAMENT

  EachMomentUpdate();

  // wait to receive massage and response
  while (rts.receiveData() > 0) rts.handleData();
}

void RTS_PauseMoveAxisPage() {
  if (waitway == 1) {
    rts.gotoPage(12, 67);
    waitway = 0;
  }
  else if (waitway == 5) {
    rts.gotoPage(39, 94);
    waitway = 0;
  }
}

void RTS_AutoBedLevelPage() {
  if (waitway == 3) {
    rts.gotoPage(111, 117);
    waitway = 0;
  }
}

void RTS_MoveAxisHoming() {
  if (waitway == 4) {
    rts.gotoPage(29 + AxisUnitMode - 1, 84 + AxisUnitMode - 1);
    waitway = 0;
  }
  else if (waitway == 6) {
    rts.gotoPage(111, 117);
    waitway = 0;
  }
  else if (waitway == 7) {
    // Click Print finish
    rts.gotoPage(1, 56);
    waitway = 0;
  }

  TERN_(HAS_X_AXIS, rts.sendData(10.0f * current_position.x, AXIS_X_COORD_VP));
  TERN_(HAS_Y_AXIS, rts.sendData(10.0f * current_position.y, AXIS_Y_COORD_VP));
  TERN_(HAS_Z_AXIS, rts.sendData(10.0f * current_position.z, AXIS_Z_COORD_VP));
}

#endif // SOVOL_SV06_RTS
