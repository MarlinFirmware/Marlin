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
 * lcd_rts_laser.cpp
 */

#include "../../../inc/MarlinConfig.h"

#if HAS_LASER_E3S1PRO

#include "lcd_rts_laser.h"
#include "lcd_rts.h"

#include <wstring.h>
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
//#include <libmaple/usart.h>
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

//#include "../../../feature/bedlevel/abl/bbl.h"

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

#include "../../../feature/spindle_laser.h"

#ifdef LCD_SERIAL_PORT
  #define LCDSERIAL LCD_SERIAL
#elif SERIAL_PORT_2
  #define LCDSERIAL MYSERIAL2
#endif

extern void RTS_line_to_current(AxisEnum axis);

constexpr float default_max_feedrate[]        = DEFAULT_MAX_FEEDRATE;
constexpr float default_max_acceleration[]    = DEFAULT_MAX_ACCELERATION;
constexpr float default_max_jerk[]            = { DEFAULT_XJERK, DEFAULT_YJERK, DEFAULT_ZJERK, DEFAULT_EJERK };
constexpr float default_axis_steps_per_unit[] = DEFAULT_AXIS_STEPS_PER_UNIT;

static bool first_start_laser = true;
bool laser_axes_should_home   = false;

void RTS::sdCardStopLaser(void) {
  card.flag.abort_sd_printing = true;
  queue.clear();
  if (home_flag) planner.synchronize();

  quickstop_stepper();
  print_job_timer.stop();

  print_job_timer.reset();
  laser_device.quick_stop();

  poweroffContinue = false;
  //sd_printing_autopause = false;

  TERN_(POWER_LOSS_RECOVERY, if (card.flag.mounted) card.removeJobRecoveryFile());

  sendData(1, MOTOR_FREE_ICON_VP);
  sendData(0, PRINT_PROCESS_ICON_VP);
  sendData(0, PRINT_PROCESS_VP);
  delay(2);
  for (int j = 0; j < 20; j++) {
    // clean screen.
    sendData(0, PRINT_FILE_TEXT_VP + j);
    // clean filename
    sendData(0, SELECT_FILE_TEXT_VP + j);
  }
  planner.synchronize();

  //sendData(exchangePageBase + 51, exchangePageAddr);
  //change_page_font = 51;
}

void RTS::handleDataLaser(void) {
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

  //SERIAL_ECHOPGM("\nCheckkey=", Checkkey, "  recdat.data[0]=", recdat.data[0]);

  switch (Checkkey) {
    case MainEnterKey:
      if (recdat.data[0] == 1) {
        CardUpdate             = true;
        cardRecBuf.recordcount = -1;
        sdCardUpate();
        sendData(exchangePageBase + 52, exchangePageAddr);
        change_page_font = 52;
        EEPROM_SAVE_LANGUAGE();
      }
      else if (recdat.data[0] == 2) {
        AxisUnitMode = 1;
        axis_unit    = 10.0;

        if (!laser_axes_should_home) {
          laser_axes_should_home = true;
          waitway                = 9;
          queue.enqueue_now_P(HOME_LASER);
          sendData(exchangePageBase + 40, exchangePageAddr);
          change_page_font = 40;

          sendData(0, AXIS_X_COORD_VP);
          sendData(10 * 10, AXIS_Y_COORD_VP);

        }
        else {
          sendData(exchangePageBase + 70, exchangePageAddr);
          change_page_font = 70;
          sendData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
          sendData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
        }
        EEPROM_SAVE_LANGUAGE();
        //sendData(1, FILAMENT_CONTROL_ICON_VP);
      }
      else if (recdat.data[0] == 3) {
        sendData(exchangePageBase + 64, exchangePageAddr);
        change_page_font = 64;
        EEPROM_SAVE_LANGUAGE();
      }
      else if (recdat.data[0] == 4) {
        sendData(exchangePageBase + 25, exchangePageAddr);
        change_page_font = 25;
        planner.synchronize();
        queue.enqueue_now(F("G28\nG1 F200 Z0.0"));
        //sendData(1, AUTO_BED_LEVEL_TITLE_VP);
        sendData(0, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 5) {
        //card.flag.abort_sd_printing = true;
        queue.clear();
        quickstop_stepper();
        print_job_timer.stop();
        sendData(0, MOTOR_FREE_ICON_VP); // 激光时锁定
        sendData(0, PRINT_PROCESS_ICON_VP);
        sendData(0, PRINT_PROCESS_VP);
        delay(2);
        sendData(0, PRINT_TIME_HOUR_VP);
        sendData(0, PRINT_TIME_MIN_VP);
        print_job_timer.reset();
        //sd_printing_autopause = false;
        for (int j = 0; j < 20; j++) {
          // clean screen.
          sendData(0, PRINT_FILE_TEXT_VP + j);
          // clean filename
          sendData(0, SELECT_FILE_TEXT_VP + j);
        }
        sendData(exchangePageBase + 51, exchangePageAddr);
        change_page_font = 51;
      }
      else if (recdat.data[0] == 6) {
        waitway = 3;
        sendData(1, AUTO_BED_LEVEL_TITLE_VP);
        sendData(AUTO_BED_LEVEL_PREHEAT, AUTO_BED_PREHEAT_HEAD_DATA_VP);
        //sendData(exchangePageBase + 26, exchangePageAddr);
        //change_page_font = 26;
        rts.sendData(0, AUTO_LEVELING_PERCENT_DATA_VP);
        thermalManager.setTargetHotend(AUTO_BED_LEVEL_PREHEAT, 0);
        sendData(AUTO_BED_LEVEL_PREHEAT, HEAD_SET_TEMP_VP);
        if (thermalManager.degHotend(0) < (AUTO_BED_LEVEL_PREHEAT - 5))
          queue.enqueue_now(F("G4 S40"));

        if (axes_should_home()) queue.enqueue_one(F("G28"));
        queue.enqueue_one(F("G29"));
        sendData(0, MOTOR_FREE_ICON_VP);
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
        sendData(exchangePageBase + 51, exchangePageAddr);
        change_page_font = 51;
        EEPROM_SAVE_LANGUAGE();
      }
      break;

    case AdjustEnterKey:
      if (recdat.data[0] == 1) {
        sendData(exchangePageBase + 74, exchangePageAddr);
        change_page_font = 74;
      }
      else if (recdat.data[0] == 2) {
        if (card.isPrinting()) {
          sendData(exchangePageBase + 59, exchangePageAddr);
          change_page_font = 59;
        }
        else {
          sendData(exchangePageBase + 61, exchangePageAddr);
          change_page_font = 61;
        }
        //settings.save();
      }
      else if (recdat.data[0] == 5) {
        sendData(exchangePageBase + 15, exchangePageAddr);
        change_page_font = 15;
      }
      else if (recdat.data[0] == 7) {
        sendData(exchangePageBase + 74, exchangePageAddr);
        change_page_font = 74;
        settings.save();
      }
      break;

    case PrintSpeedEnterKey:
      feedrate_percentage = recdat.data[0];
      sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);
      break;

    case StopPrintKey:
      if (recdat.data[0] == 1) {
        sendData(exchangePageBase + 58, exchangePageAddr);
        change_page_font = 58;
      }
      else if (recdat.data[0] == 2) {
        waitway = 10;

        //card.flag.abort_sd_printing = true;  //card.flag.abort_sd_printing
        sendData(0, PRINT_TIME_HOUR_VP);
        sendData(0, PRINT_TIME_MIN_VP);
        updateTimeValue = 0;

        //runout.reset();

        sendData(exchangePageBase + 40, exchangePageAddr);
        change_page_font = 40;
        sdCardStopLaser();
        cutter.apply_power(0);  // 关闭激光
      }
      else if (recdat.data[0] == 3) {
        if (card.isPrinting()) {
          sendData(exchangePageBase + 59, exchangePageAddr);
          change_page_font = 59;
        }
        else {
          sendData(exchangePageBase + 61, exchangePageAddr);
          change_page_font = 61;
        }
      }
      break;

    case PausePrintKey:
      if (recdat.data[0] == 1) {
        if (card.isPrinting()) {  // && (thermalManager.degHotend(0) > (thermalManager.degTargetHotend(0) - 5)) && (thermalManager.degBed() > (thermalManager.degTargetBed() - 3)))
          sendData(exchangePageBase + 62, exchangePageAddr);
          change_page_font = 62;
        }
        else {
          sendData(exchangePageBase + 59, exchangePageAddr);
          change_page_font = 59;
        }
      }
      else if (recdat.data[0] == 2) {
        waitway = 1;
        //pause_e = current_position[E_AXIS];
        card.pauseSDPrint();
        print_job_timer.pause();
        pause_action_flag = true;
        updateTimeValue = 0;
        sendData(exchangePageBase + 61, exchangePageAddr);
        change_page_font = 61;
        planner.synchronize();
        sdcard_pause_check = false;
        //queue.inject((char*)"M25");
      }
      else if (recdat.data[0] == 3) {
        if (card.isPrinting()) {
          sendData(exchangePageBase + 59, exchangePageAddr);
          change_page_font = 59;
        }
        else {
          sendData(exchangePageBase + 61, exchangePageAddr);
          change_page_font = 61;
        }
      }
      break;

    case ResumePrintKey:
      if (recdat.data[0] == 1) {
        sendData(exchangePageBase + 59, exchangePageAddr);
        change_page_font = 59;
        cutter.apply_power(laser_device.power);
        card.startOrResumeFilePrinting();
        print_job_timer.start();
        updateTimeValue  = 0;
        sdcard_pause_check = true;
        //queue.inject((char*)"M24");
      }
      else if (recdat.data[0] == 2) {

      }
      else if (recdat.data[0] == 3) {
        //queue.inject_P(PSTR("M108"));
        wait_for_user = false;
        //runout.filament_ran_out = false;
        //runout.reset();

        sendData(exchangePageBase + 10, exchangePageAddr);
        change_page_font = 10;

        //card.startFileprint();
        print_job_timer.start();
        updateTimeValue  = 0;
        sdcard_pause_check = true;
      }
      break;

    case ZoffsetEnterKey:

      last_zoffset = zprobe_zoffset;
      if (recdat.data[0] >= 32768) {
        //zprobe_zoffset = ((float)recdat.data[0] - 65536) / 100;
        zprobe_zoffset  = (signed short)recdat.data[0] / 100.0;
        zprobe_zoffset -= 0.001;
      }
      else {
        zprobe_zoffset  = ((float)recdat.data[0]) / 100;
        zprobe_zoffset += 0.001;
      }
      if (WITHIN((zprobe_zoffset), Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
        babystep.add_mm_laser(Z_AXIS, zprobe_zoffset - last_zoffset);
        //SERIAL_ECHOPGM("\nZoffset=", zprobe_zoffset - last_zoffset);
      probe.offset.z = zprobe_zoffset;
      //settings.save();
      break;

    case PrepareEnterKey:
      //SERIAL_ECHOPGM("PrepareEnterKey=", recdat.data[0]);
      if (recdat.data[0] == 1) {
        sendData(exchangePageBase + 28, exchangePageAddr);
        change_page_font = 28;
      }
      else if (recdat.data[0] == 2) { // 语言选择
        sendData(exchangePageBase + 65, exchangePageAddr);
        change_page_font = 65;
        EEPROM_SAVE_LANGUAGE();
      }
      else if (recdat.data[0] == 3) {
        rts.sendData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
        rts.sendData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
        rts.sendData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
        delay(2);
        sendData(exchangePageBase + 70, exchangePageAddr);
        change_page_font = 70;
      }
      else if (recdat.data[0] == 4) {
        sendData(exchangePageBase + 58, exchangePageAddr);
        change_page_font = 58;
      }
      else if (recdat.data[0] == 5) {
        sendData(MACHINE_TYPE, MACHINE_TYPE_ABOUT_TEXT_VP);
        sendData(FIRMWARE_VERSION, FIRMWARE_VERSION_ABOUT_TEXT_VP);
        //sendData(SCREEN_VERSION, PRINTER_DISPLAY_VERSION_TEXT_VP); // 20220223 屏幕版本改由屏幕工程设定
        sendData(HARDWARE_VERSION, HARDWARE_VERSION_ABOUT_TEXT_VP);
        sendData(PRINT_SIZE, PRINTER_PRINTSIZE_TEXT_VP);
        delay(5);
        if (1 == lang)
          sendData(CORP_WEBSITE_C, WEBSITE_ABOUT_TEXT_VP);
        else
          sendData(CORP_WEBSITE_E, WEBSITE_ABOUT_TEXT_VP);
        sendData(exchangePageBase + 66, exchangePageAddr);
        change_page_font = 66;
        EEPROM_SAVE_LANGUAGE();
      }
      else if (recdat.data[0] == 6) {
        queue.enqueue_now(F("M84"));
        sendData(1, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 7) {
        sendData(exchangePageBase + 43, exchangePageAddr);
        change_page_font = 43;
      }
      else if (recdat.data[0] == 8) {
        settings.save();
        sendData(exchangePageBase + 21, exchangePageAddr);
        change_page_font = 21;
      }
      else if (recdat.data[0] == 9) {
        sendData(exchangePageBase + 1, exchangePageAddr);
        change_page_font = 1;
      }
      else if (recdat.data[0] == 0xA) {
        sendData(exchangePageBase + 42, exchangePageAddr);
        change_page_font = 42;
      }
      else if (recdat.data[0] == 0xB) {// 恢复出厂设置  确定
        #if ENABLED(HAS_MENU_RESET_WIFI)
          WIFI_STATE = PRESSED;
          OUT_WRITE(RESET_WIFI_PIN, LOW);
        #endif
        (void)settings.reset();
        (void)settings.save();
        init();
        sendData(exchangePageBase + 51, exchangePageAddr);
        change_page_font = 51;
      }
      else if (recdat.data[0] == 0xC) {
        sendData(exchangePageBase + 44, exchangePageAddr);
        change_page_font = 44;
      }
      else if (recdat.data[0] == 0xD) {
        settings.reset();
        settings.save();
        sendData(exchangePageBase + 33, exchangePageAddr);
        change_page_font = 33;
      }
      else if (recdat.data[0] == 0xE) { // 恢复出厂设置  取消
        if (!planner.has_blocks_queued()) {
          sendData(exchangePageBase + 64, exchangePageAddr);
          change_page_font = 64;
        }
      }
      else if (recdat.data[0] == 0xF) {// 高级设置
        sendData(exchangePageBase + 64, exchangePageAddr);
        change_page_font = 64;
        settings.save();// delay(100);
      }
      else if (recdat.data[0] == 0x10) {
        sendData(exchangePageBase + 25, exchangePageAddr);
        change_page_font = 25;
      }
      else if (recdat.data[0] == 0x11) {
        sendData(exchangePageBase + 21, exchangePageAddr);
        change_page_font = 21;
      }
      break;

    case AutoHomeKey:
      if (recdat.data[0] == 1) {
        AxisUnitMode = 1;
        axis_unit    = 10.0;
        sendData(exchangePageBase + 70, exchangePageAddr);
        change_page_font = 70;
        sendData(3, MOVEAXIS_UNIT_ICON_VP);
      }
      else if (recdat.data[0] == 2) {
        AxisUnitMode = 2;
        axis_unit    = 1.0;
        sendData(exchangePageBase + 71, exchangePageAddr);
        change_page_font = 71;
        sendData(2, MOVEAXIS_UNIT_ICON_VP);
      }
      else if (recdat.data[0] == 3) {
        AxisUnitMode = 3;
        axis_unit    = 0.1;
        sendData(exchangePageBase + 72, exchangePageAddr);
        change_page_font = 72;
        sendData(1, MOVEAXIS_UNIT_ICON_VP);
      }
      else if (recdat.data[0] == 4) {
        waitway = 4;
        queue.enqueue_now(F("G28 X Y"));
        updateTimeValue = 0;
        sendData(0, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 5) {
        waitway = 4;
        queue.enqueue_now(F("G28 Z"));
        sendData(0, MOTOR_FREE_ICON_VP);
        updateTimeValue = 0;
      }
      break;

    case XaxismoveKey:
      float x_min, x_max;
      waitway = 4;
      x_min = 0;
      x_max = X_MAX_POS;
      current_position[X_AXIS] = ((float)recdat.data[0]) / 10;
      if (current_position[X_AXIS] < x_min)
        current_position[X_AXIS] = x_min;
      else if (current_position[X_AXIS] > x_max)
        current_position[X_AXIS] = x_max;
      RTS_line_to_current(X_AXIS);
      sendData(10 * current_position[X_AXIS], AXIS_X_COORD_VP);
      delay(1);
      sendData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
      break;

    case YaxismoveKey:
      float y_min, y_max;
      waitway = 4;
      y_min = 0;
      y_max = Y_MAX_POS;
      current_position[Y_AXIS] = ((float)recdat.data[0]) / 10;
      if (current_position[Y_AXIS] < y_min)
        current_position[Y_AXIS] = y_min;
      else if (current_position[Y_AXIS] > y_max)
        current_position[Y_AXIS] = y_max;
      RTS_line_to_current(Y_AXIS);
      sendData(10 * current_position[Y_AXIS], AXIS_Y_COORD_VP);
      delay(1);
      sendData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
      break;

    case ZaxismoveKey:
      float z_min, z_max;
      waitway = 4;
      z_min = Z_MIN_POS;
      z_max = Z_MAX_POS;

      current_position[Z_AXIS] = ((float)recdat.data[0]) / 10;
      if (current_position[Z_AXIS] < z_min)
        current_position[Z_AXIS] = z_min;
      else if (current_position[Z_AXIS] > z_max)
        current_position[Z_AXIS] = z_max;

      RTS_line_to_current(Z_AXIS);
      sendData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
      delay(1);
      sendData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
      break;

    case SelectLanguageKey:
      if (recdat.data[0] != 0) lang = recdat.data[0];
      language_change_font = lang;
      for (int i = 0; i < 9; i++) sendData(0, LANGUAGE_CHINESE_TITLE_VP + i);
      sendData(1, LANGUAGE_CHINESE_TITLE_VP + (language_change_font - 1));
      languagedisplayUpdate();
      //settings.save();
      eeprom_save_flag = true;
      break;

    case PowerContinuePrintKey:
      if (recdat.data[0] == 1) {

        #if ENABLED(POWER_LOSS_RECOVERY)
          if (recovery.info.recovery_flag) {
            power_off_type_yes = 1;
            updateTimeValue  = 0;
            sendData(exchangePageBase + 10, exchangePageAddr);
            change_page_font = 10;
            //recovery.resume();
            queue.enqueue_now(F("M1000"));

            poweroffContinue   = true;
            sdcard_pause_check = true;
            zprobe_zoffset     = probe.offset.z;
            sendData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
            sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);
          }
        #elif ENABLED(CREALITY_POWER_LOSS)
          if (pre01_power_loss.info.recovery_flag) {
            power_off_type_yes = 1;
            updateTimeValue  = 0;
            sendData(exchangePageBase + 10, exchangePageAddr);
            change_page_font = 10;
            pre01_power_loss.resume(); // �ָ�SD����ӡ
            //queue.enqueue_now(F("M1000"));

            poweroffContinue   = true;
            sdcard_pause_check = true;
            zprobe_zoffset     = probe.offset.z;
            sendData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
            sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);
          }
        #endif
      }
      else if (recdat.data[0] == 2) {
        updateTimeValue = RTS_UPDATE_VALUE;
        sendData(exchangePageBase + 51, exchangePageAddr);
        change_page_font = 51;
        sendData(0, PRINT_TIME_HOUR_VP);
        sendData(0, PRINT_TIME_MIN_VP);
        updateTimeValue = 0;
        sdCardStopLaser();
      }
      break;

    case StoreMemoryKey:
      if (recdat.data[0] == 1) {
        rts.sendData(exchangePageBase + 37, exchangePageAddr);
        change_page_font = 37;
      }
      if (recdat.data[0] == 2) {
        queue.enqueue_now(F("M502"));
        rts.sendData(exchangePageBase + 33, exchangePageAddr);
        change_page_font = 33;
        settings.save();
        sendData(default_max_feedrate[X_AXIS], MAX_VELOCITY_XAXIS_DATA_VP);
        sendData(default_max_feedrate[Y_AXIS], MAX_VELOCITY_YAXIS_DATA_VP);
        sendData(default_max_feedrate[Z_AXIS], MAX_VELOCITY_ZAXIS_DATA_VP);
        sendData(default_max_feedrate[E_AXIS], MAX_VELOCITY_EAXIS_DATA_VP);
        //delay(20);

        sendData(default_max_acceleration[X_AXIS], MAX_ACCEL_XAXIS_DATA_VP);
        sendData(default_max_acceleration[Y_AXIS], MAX_ACCEL_YAXIS_DATA_VP);
        sendData(default_max_acceleration[Z_AXIS], MAX_ACCEL_ZAXIS_DATA_VP);
        sendData(default_max_acceleration[E_AXIS], MAX_ACCEL_EAXIS_DATA_VP);
        //delay(20);

        sendData(default_max_jerk[X_AXIS] * 100, MAX_JERK_XAXIS_DATA_VP);
        sendData(default_max_jerk[Y_AXIS] * 100, MAX_JERK_YAXIS_DATA_VP);
        sendData(default_max_jerk[Z_AXIS] * 100, MAX_JERK_ZAXIS_DATA_VP);
        sendData(default_max_jerk[E_AXIS] * 100, MAX_JERK_EAXIS_DATA_VP);
        //delay(20);

        sendData(default_axis_steps_per_unit[X_AXIS] * 10, MAX_STEPSMM_XAXIS_DATA_VP);
        sendData(default_axis_steps_per_unit[Y_AXIS] * 10, MAX_STEPSMM_YAXIS_DATA_VP);
        sendData(default_axis_steps_per_unit[Z_AXIS] * 10, MAX_STEPSMM_ZAXIS_DATA_VP);
        sendData(default_axis_steps_per_unit[E_AXIS] * 10, MAX_STEPSMM_EAXIS_DATA_VP);
        //delay(20);

        sendData(default_nozzle_ptemp * 100, NOZZLE_TEMP_P_DATA_VP);
        sendData(default_nozzle_itemp * 100, NOZZLE_TEMP_I_DATA_VP);
        sendData(default_nozzle_dtemp * 100, NOZZLE_TEMP_D_DATA_VP);
        delay(20);
        sendData(default_hotbed_ptemp * 100, HOTBED_TEMP_P_DATA_VP);
        sendData(default_hotbed_itemp * 100, HOTBED_TEMP_I_DATA_VP);
        sendData(default_hotbed_dtemp * 10, HOTBED_TEMP_D_DATA_VP);
        //delay(100);
      }
      else if (recdat.data[0] == 3) {
        rts.sendData(exchangePageBase + 33, exchangePageAddr);
        change_page_font = 33;
      }
      else if (recdat.data[0] == 4) {
        rts.sendData(exchangePageBase + 34, exchangePageAddr);
        change_page_font = 34;
      }
      else if (recdat.data[0] == 5) {
        rts.sendData(exchangePageBase + 39, exchangePageAddr);
        change_page_font = 39;
      }
      //else if (recdat.data[0] == 6)
      //{
      //  if (wifi_enable_flag)
      //  {
      //    wifi_enable_flag = 0;
      //    queue.inject_P(PSTR("M115"));
      //    sendData(1, ADV_SETTING_WIFI_ICON_VP);
      //    settings.save();
      //  }
      //  else
      //  {
      //    wifi_enable_flag = 1;
      //    queue.inject_P(PSTR("M115"));
      //    sendData(0, ADV_SETTING_WIFI_ICON_VP);
      //    settings.save();
      //  }
      //}
      else if (recdat.data[0] == 7) { //传动比
        rts.sendData(exchangePageBase + 76, exchangePageAddr);
        change_page_font = 76;
      }
      else if (recdat.data[0] == 8) { // 最大加速度
        rts.sendData(exchangePageBase + 67, exchangePageAddr);
        change_page_font = 67;
      }
      else if (recdat.data[0] == 9) { // 最大拐角速度
        rts.sendData(exchangePageBase + 68, exchangePageAddr);
        change_page_font = 68;
      }
      else if (recdat.data[0] == 0x0A) { // 最大速度
        rts.sendData(exchangePageBase + 69, exchangePageAddr);
        change_page_font = 69;
      }
      else if (recdat.data[0] == 0x0B) {
        rts.sendData(exchangePageBase + 33, exchangePageAddr);
        change_page_font = 33;
        settings.save();
        delay(100);
      }
      else if (recdat.data[0] == 0x0C) {
        rts.sendData(exchangePageBase + 34, exchangePageAddr);
        change_page_font = 34;
        settings.save();
        delay(100);
      }
      else if (recdat.data[0] == 0x0D) {// 返回按钮
        rts.sendData(exchangePageBase + 64, exchangePageAddr);
        change_page_font = 64;
        //settings.save();
        //delay(100);
      }
      break;

    case VelocityXaxisEnterKey: {
      const float velocity_xaxis = recdat.data[0];
      sendData(velocity_xaxis, MAX_VELOCITY_XAXIS_DATA_VP);
      planner.set_max_feedrate(X_AXIS, velocity_xaxis);
    } break;

    case VelocityYaxisEnterKey: {
      const float velocity_yaxis = recdat.data[0];
      sendData(velocity_yaxis, MAX_VELOCITY_YAXIS_DATA_VP);
      planner.set_max_feedrate(Y_AXIS, velocity_yaxis);
    } break;

    case VelocityZaxisEnterKey: {
      const float velocity_zaxis = recdat.data[0];
      sendData(velocity_zaxis, MAX_VELOCITY_ZAXIS_DATA_VP);
      planner.set_max_feedrate(Z_AXIS, velocity_zaxis);
    } break;

    case VelocityEaxisEnterKey: {
      const float velocity_eaxis = = recdat.data[0];
      sendData(velocity_eaxis, MAX_VELOCITY_EAXIS_DATA_VP);
      planner.set_max_feedrate(E_AXIS, velocity_eaxis);
    } break;

    case AccelXaxisEnterKey: {
      const float accel_xaxis = recdat.data[0];
      sendData(accel_xaxis, MAX_ACCEL_XAXIS_DATA_VP);
      planner.set_max_acceleration(X_AXIS, accel_xaxis);
    } break;

    case AccelYaxisEnterKey: {
      const float accel_yaxis = recdat.data[0];
      sendData(accel_yaxis, MAX_ACCEL_YAXIS_DATA_VP);
      planner.set_max_acceleration(Y_AXIS, accel_yaxis);
    } break;

    case AccelZaxisEnterKey: {
      const float accel_zaxis = recdat.data[0];
      sendData(accel_zaxis, MAX_ACCEL_ZAXIS_DATA_VP);
      planner.set_max_acceleration(Z_AXIS, accel_zaxis);
    } break;

    case AccelEaxisEnterKey: {
      const float accel_eaxis = recdat.data[0];
      sendData(accel_eaxis, MAX_ACCEL_EAXIS_DATA_VP);
      planner.set_max_acceleration(E_AXIS, accel_eaxis);
    } break;

    case JerkXaxisEnterKey: {
      const float jerk_xaxis = (float)recdat.data[0] / 100;
      sendData(jerk_xaxis * 100, MAX_JERK_XAXIS_DATA_VP);
      planner.set_max_jerk(X_AXIS, jerk_xaxis);
    } break;

    case JerkYaxisEnterKey: {
      const float jerk_yaxis = (float)recdat.data[0] / 100;
      sendData(jerk_yaxis * 100, MAX_JERK_YAXIS_DATA_VP);
      planner.set_max_jerk(Y_AXIS, jerk_yaxis);
    } break;

    case JerkZaxisEnterKey: {
      const float jerk_zaxis = (float)recdat.data[0] / 100;
      sendData(jerk_zaxis * 100, MAX_JERK_ZAXIS_DATA_VP);
      planner.set_max_jerk(Z_AXIS, jerk_zaxis);
    } break;

    case JerkEaxisEnterKey: {
      const float jerk_eaxis = (float)recdat.data[0] / 100;
      sendData(jerk_eaxis * 100, MAX_JERK_EAXIS_DATA_VP);
      planner.set_max_jerk(E_AXIS, jerk_eaxis);
    } break;

    case StepsmmXaxisEnterKey: {
      const float stepsmm_xaxis = (float)recdat.data[0] / 10;
      sendData(stepsmm_xaxis * 10, MAX_STEPSMM_XAXIS_DATA_VP);
      planner.settings.axis_steps_per_mm[X_AXIS] = stepsmm_xaxis;
    } break;

    case StepsmmYaxisEnterKey: {
      const float stepsmm_yaxis = (float)recdat.data[0] / 10;
      sendData(stepsmm_yaxis * 10, MAX_STEPSMM_YAXIS_DATA_VP);
      planner.settings.axis_steps_per_mm[Y_AXIS] = stepsmm_yaxis;
    } break;

    case StepsmmZaxisEnterKey: {
      const float stepsmm_zaxis = (float)recdat.data[0] / 10;
      sendData(stepsmm_zaxis * 10, MAX_STEPSMM_ZAXIS_DATA_VP);
      planner.settings.axis_steps_per_mm[Z_AXIS] = stepsmm_zaxis;
    } break;

    case StepsmmEaxisEnterKey: {
      const float stepsmm_eaxis = (float)recdat.data[0] / 10;
      sendData(stepsmm_eaxis * 10, MAX_STEPSMM_EAXIS_DATA_VP);
      planner.settings.axis_steps_per_mm[E_AXIS] = stepsmm_eaxis;
    } break;

    case NozzlePTempEnterKey: {
      const float nozzle_ptemp = (float)recdat.data[0] / 100;
      sendData(nozzle_ptemp * 100, NOZZLE_TEMP_P_DATA_VP);
      PID_PARAM(Kp, 0) = nozzle_ptemp;
    } break;

    case NozzleITempEnterKey: {
      const float nozzle_itemp = (float)recdat.data[0] / 100;
      sendData(nozzle_itemp * 100, NOZZLE_TEMP_I_DATA_VP);
      PID_PARAM(Ki, 0) = scalePID_i(nozzle_itemp);
    } break;

    case NozzleDTempEnterKey: {
      const float nozzle_dtemp = (float)recdat.data[0] / 100;
      sendData(nozzle_dtemp * 100, NOZZLE_TEMP_D_DATA_VP);
      PID_PARAM(Kd, 0) = scalePID_d(nozzle_dtemp);
    } break;

    case HotbedPTempEnterKey: {
      const float hotbed_ptemp = (float)recdat.data[0] / 100;
      sendData(hotbed_ptemp * 100, HOTBED_TEMP_P_DATA_VP);
      thermalManager.temp_bed.pid.Kp = hotbed_ptemp;
    } break;

    case HotbedITempEnterKey: {
      const float hotbed_itemp = (float)recdat.data[0] / 100;
      sendData(hotbed_itemp * 100, HOTBED_TEMP_I_DATA_VP);
      thermalManager.temp_bed.pid.Ki = scalePID_i(hotbed_itemp);
    } break;

    case HotbedDTempEnterKey: {
      const float hotbed_dtemp = (float)recdat.data[0] / 10;
      sendData(hotbed_dtemp * 10, HOTBED_TEMP_D_DATA_VP);
      thermalManager.temp_bed.pid.Kd = scalePID_d(hotbed_dtemp);
    } break;

    case PrintFanSpeedkey: {
      const uint8_t fan_speed = (uint8_t)recdat.data[0];
      sendData(fan_speed, PRINTER_FAN_SPEED_DATA_VP);
      thermalManager.set_fan_speed(0, fan_speed);
    } break;

    case SelectFileKey:
      if (sdDetected()) {
        if (recdat.data[0] > cardRecBuf.Filesum)
          break;

        cardRecBuf.recordcount = recdat.data[0] - 1;

        for (int j = 0; j < 10; j++) sendData(0, SELECT_FILE_TEXT_VP + j);
        delay(2);
        for (int j = 1; j <= cardRecBuf.Filesum; j++) {
          sendData((unsigned long)0x073F, FilenameNature + j * 16);
          sendData(0, FILE1_SELECT_ICON_VP - 1 + j);
        }
        sendData((unsigned long)0xFFFF, FilenameNature + recdat.data[0] * 16);
        sendData(1, FILE1_SELECT_ICON_VP + (recdat.data[0] - 1));
      }

      sendData(exchangePageBase + 51, exchangePageAddr);
      change_page_font = 51;
      rts_start_print  = true;
      delay(20);
      sendData(cardRecBuf.Cardshowfilename[cardRecBuf.recordcount], SELECT_FILE_TEXT_VP);
      break;

    case StartFileKey:
      if (recdat.data[0] == 1 && sdDetected()) {
        if (cardRecBuf.recordcount < 0)
          break;
        if (!rts_start_print)
          //SERIAL_ECHOLNPGM("\r\nrts_start_print: ", rts_start_print);
          break;

        sendData(exchangePageBase + 75, exchangePageAddr);
        change_page_font = 75;

        card.openAndPausePrintFile(cardRecBuf.Cardfilename[cardRecBuf.recordcount]);

        laser_device.reset_data();
        laser_device.set_read_gcode_range_on();
        laser_device.power = 0;
      }
      else if (recdat.data[0] == 2) {
        sendData(exchangePageBase + 53, exchangePageAddr);
        change_page_font = 53;
      }
      else if (recdat.data[0] == 3) {
        sendData(exchangePageBase + 52, exchangePageAddr);
        change_page_font = 52;
      }
      else if (recdat.data[0] == 4) {
        sendData(exchangePageBase + 54, exchangePageAddr);
        change_page_font = 54;
      }
      else if (recdat.data[0] == 5) {
        sendData(exchangePageBase + 53, exchangePageAddr);
        change_page_font = 53;
      }
      else if (recdat.data[0] == 6) {
        sendData(exchangePageBase + 55, exchangePageAddr);
        change_page_font = 55;
      }
      else if (recdat.data[0] == 7) {
        sendData(exchangePageBase + 54, exchangePageAddr);
        change_page_font = 54;
      }
      else if (recdat.data[0] == 8) {
        sendData(exchangePageBase + 55, exchangePageAddr); // by jankin 20210731
        change_page_font = 55;
      }
      else if (recdat.data[0] == 9) {
        sendData(exchangePageBase + 52, exchangePageAddr);
        change_page_font = 52;
      }
      else if (recdat.data[0] == 0x0A) {
        sendData(exchangePageBase + 55, exchangePageAddr);
        change_page_font = 55;
      }
      break;

    case ChangePageKey:
      for (int i = 0; i < MaxFileNumber; i++)
        for (int j = 0; j < 20; j++) sendData(0, FILE1_TEXT_VP + i * 20 + j);

      for (int i = 0; i < cardRecBuf.Filesum; i++) {
        for (int j = 0; j < 20; j++) sendData(0, cardRecBuf.addr[i] + j);
        sendData((unsigned long)0xFFFF, FilenameNature + (i + 1) * 16);
      }

      for (int j = 0; j < 20; j++) {
        // clean screen.
        sendData(0, PRINT_FILE_TEXT_VP + j);
        // clean filename
        sendData(0, SELECT_FILE_TEXT_VP + j);
      }
      // clean filename Icon
      for (int j = 0; j < 20; j++) sendData(0, FILE1_SELECT_ICON_VP + j);

      sendData(cardRecBuf.Cardshowfilename[cardRecBuf.recordcount], PRINT_FILE_TEXT_VP);

      // represents to update file list
      //if (CardUpdate && lcd_sd_status && IS_SD_INSERTED())
      if (CardUpdate && lcd_sd_status && sdDetected()) {
        for (uint16_t i = 0; i < cardRecBuf.Filesum; i++) {
          delay(3);
          sendData(cardRecBuf.Cardshowfilename[i], cardRecBuf.addr[i]);
          sendData((unsigned long)0xFFFF, FilenameNature + (i + 1) * 16);
          sendData(0, FILE1_SELECT_ICON_VP + i);
        }
      }

      sendData(MACVERSION, MACHINE_TYPE_ABOUT_TEXT_VP);
      sendData(SOFTVERSION, FIRMWARE_VERSION_ABOUT_TEXT_VP);
      sendData(PRINT_SIZE, PRINTER_PRINTSIZE_TEXT_VP);

      if (1 == lang)
        sendData(CORP_WEBSITE_C, WEBSITE_ABOUT_TEXT_VP);
      else
        sendData(CORP_WEBSITE_E, WEBSITE_ABOUT_TEXT_VP);

      percentrec = card.percentDone() + 1;
      if (percentrec <= 100)
        rts.sendData((unsigned char)percentrec, PRINT_PROCESS_ICON_VP);
      rts.sendData((unsigned char)card.percentDone(), PRINT_PROCESS_VP);

      sendData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);

      sendData(feedrate_percentage, PRINT_SPEED_RATE_VP);
      sendData(thermalManager.degTargetHotend(0), HEAD_SET_TEMP_VP);
      sendData(thermalManager.degTargetBed(), BED_SET_TEMP_VP);
      languagedisplayUpdate();

      sendData(change_page_font + exchangePageBase, exchangePageAddr);
      break;

    case FocusZAxisKey: {
      waitway = 4;
      current_position[Z_AXIS] = ((signed short)recdat.data[0]) / 10.0;
      RTS_line_to_current(Z_AXIS);
      sendData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
      delay(1);
      sendData(0, MOTOR_FREE_ICON_VP);
      waitway = 0;
    } break;

    case AdjustFocusKey:
      if (recdat.data[0] == 1) {// 调节激光焦距
        sendData(10 * current_position[Z_AXIS], SW_FOCUS_Z_VP);
        sendData(exchangePageBase + 63, exchangePageAddr);
        change_page_font = 63;
        //} else if (recdat.data[0] == 2)// Z+
        //{

        //} else if (recdat.data[0] == 3)// Z-
        //{

      }
      else if (recdat.data[0] == 4) {// 返回
        sendData(exchangePageBase + 64, exchangePageAddr);
        change_page_font = 64;
      }
      else if (recdat.data[0] == 5) {// √
        queue.inject_P(PSTR("G92.9 Z0"));
        sendData(0, AXIS_Z_COORD_VP);
        sendData(0, SW_FOCUS_Z_VP);

        sendData(exchangePageBase + 64, exchangePageAddr);
        change_page_font = 64;
      }
      else if (recdat.data[0] == 6) {// x
        //queue.inject_P(PSTR("G92.9 Z0"));
        //sendData(0, AXIS_Z_COORD_VP);
        //sendData(0, SW_FOCUS_Z_VP);
        sendData(exchangePageBase + 64, exchangePageAddr);
        change_page_font = 64;
      }
      break;
    case SwAdjustFocusKey:

      break;

    case EngraveWarningKey:
      if (recdat.data[0] == 1) {

      }
      else if (recdat.data[0] == 2) {// 轴移动
        AxisUnitMode = 1;
        axis_unit = 10.0;
        sendData(exchangePageBase + 78, exchangePageAddr);
        change_page_font = 78;
      }
      else if (recdat.data[0] == 3) { // 直接雕刻
        laser_device.laser_printing = true; // 雕刻中
        queue.inject_P(M24_STR);
        sendData(exchangePageBase + 59, exchangePageAddr);
        change_page_font = 59;
      }
      else if (recdat.data[0] == 4) { // 跑边框
        HMI_Area_Move();
      }
      else if (recdat.data[0] == 5) { // 返回
        cardRecBuf.recordcount = -1;
        print_job_timer.stop();
        //sendData(1, MOTOR_FREE_ICON_VP);
        sendData(0, PRINT_PROCESS_ICON_VP);
        sendData(0, PRINT_PROCESS_VP);
        delay(2);
        sendData(0, PRINT_TIME_HOUR_VP);
        sendData(0, PRINT_TIME_MIN_VP);
        print_job_timer.reset();
        //sd_printing_autopause = false;
        for (int j = 0; j < 20; j++) {
          // clean screen.
          sendData(0, PRINT_FILE_TEXT_VP + j);
          // clean filename
          sendData(0, SELECT_FILE_TEXT_VP + j);
        }

        //sendData(exchangePageBase + 51, exchangePageAddr);
        //change_page_font = 51;

        CardUpdate = true;
        cardRecBuf.recordcount = -1;
        sdCardUpate();
        sendData(exchangePageBase + 52, exchangePageAddr);
        change_page_font = 52;
      }
      break;

    case SwitchDeviceKey:
      if (recdat.data[0] == 1) {// FDM

      }
      else if (recdat.data[0] == 2) {// 激光

      }
      else if (recdat.data[0] == 3) {// 切到FDM 确定
        uint8_t language;

        sendData(exchangePageBase + 33, exchangePageAddr);
        change_page_font = 33;
        laser_device.set_current_device(DEVICE_FDM);

        language = language_change_font;
        settings.reset();
        language_change_font = language;
        settings.save();
        probe.offset.z = zprobe_zoffset = 0;
        sendData(zprobe_zoffset * 100, AUTO_BED_LEVEL_ZOFFSET_VP);
        laser_device.laser_power_close();
      }
      else if (recdat.data[0] == 4) {// 切到FDM 取消
        sendData(exchangePageBase + 64, exchangePageAddr);
        change_page_font = 64;
        //} else if (recdat.data[0] == 5) // 切到激光 确定
        //{
        //  sendData(10*current_position[Z_AXIS], SW_FOCUS_Z_VP);
        //  sendData(exchangePageBase + 77, exchangePageAddr);
        //  change_page_font = 77;
        //  laser_device.set_current_device(DEVICE_LASER);
        //} else if (recdat.data[0] == 6) // 切到激光 取消
        //{
        //  sendData(exchangePageBase + 33, exchangePageAddr);
        //  change_page_font = 33;
      }
      else if (recdat.data[0] == 7) {// 切到熔积打印
        sendData(exchangePageBase + 57, exchangePageAddr);
        change_page_font = 57;
      }
      else if (recdat.data[0] == 8) { // 调整焦距 √
        queue.inject_P(PSTR("G92.9 Z0"));
        sendData(0, AXIS_Z_COORD_VP);
        sendData(0, SW_FOCUS_Z_VP);

        if (change_page_font == 33) {
          sendData(exchangePageBase + 64, exchangePageAddr);
          change_page_font = 64;
        }
        else {
          sendData(exchangePageBase + 51, exchangePageAddr);
          change_page_font = 51;
        }
        //sendData(exchangePageBase + 64, exchangePageAddr);
        //change_page_font = 64;
        //} else if (recdat.data[0] == 9) //调整焦距 Z+
        //{

        //} else if (recdat.data[0] == 0x0A) //调整焦距 Z-
        //{

      }
      //else if (recdat.data[0] == 0x0B) { // 切换激光雕刻
      //  sendData(exchangePageBase + 56, exchangePageAddr);
      //  change_page_font = 56;
      //}
      break;

    case LaserMoveAxis:
      if (recdat.data[0] == 1) {//
        AxisUnitMode = 1;
        axis_unit    = 10.0;
        sendData(exchangePageBase + 78, exchangePageAddr);
        change_page_font = 78;
      }
      else if (recdat.data[0] == 2) {//
        AxisUnitMode = 2;
        axis_unit    = 1.0;
        sendData(exchangePageBase + 79, exchangePageAddr);
        change_page_font = 79;
      }
      else if (recdat.data[0] == 3) {
        AxisUnitMode = 3;
        axis_unit    = 0.1;
        sendData(exchangePageBase + 80, exchangePageAddr);
        change_page_font = 80;
      }
      else if (recdat.data[0] == 4) {// 返回
        sendData(exchangePageBase + 75, exchangePageAddr);
        change_page_font = 75;
      }
      else if (recdat.data[0] == 5) {// 雕刻警告界面 xy home
        waitway = 8;
        sendData(exchangePageBase + 40, exchangePageAddr);
        change_page_font = 40;

        queue.enqueue_now_P(HOME_LASER);// PSTR("G28 XY\nG0 X0 Y5"));
        updateTimeValue = 0;
        sendData(0, AXIS_X_COORD_VP);
        sendData(10 * 10, AXIS_Y_COORD_VP);
        delay(1);
        sendData(0, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 6) {// 雕刻警告界面z home
        queue.enqueue_now(F("G0 Z0"));
      }
      else if (recdat.data[0] == 7) {// 激光 xy home
        waitway = 9;
        sendData(exchangePageBase + 40, exchangePageAddr);
        change_page_font = 40;
        queue.enqueue_now_P(HOME_LASER);// "G28 XY\n G1 X0 Y10 F3000");//EVENT_HOME_LASER);//PSTR("G28 XY\nG0 X0 Y5"));
        updateTimeValue = 0;

        sendData(0, AXIS_X_COORD_VP);
        sendData(10 * 10, AXIS_Y_COORD_VP);
        delay(1);
        sendData(0, MOTOR_FREE_ICON_VP);
      }
      else if (recdat.data[0] == 8) {// 激光 z home
        queue.enqueue_now(F("G0 Z0"));
        sendData(0, AXIS_Z_COORD_VP);
        delay(1);
        sendData(0, MOTOR_FREE_ICON_VP);
      }
      break;

    case ErrorKey:
      if (recdat.data[0] == 1) {
        if (printingIsActive()) {
          sendData(exchangePageBase + 10, exchangePageAddr);
          change_page_font = 10;
        }
        else if (printingIsPaused()) {
          sendData(exchangePageBase + 12, exchangePageAddr);
          change_page_font = 12;
        }
        else {
          sendData(exchangePageBase + 1, exchangePageAddr);
          change_page_font = 1;
        }

        if (errorway == 4) hal.reboot();
      }
      break;

    default: break;
  }

  ZERO(recdat);
  recdat.head[0] = FHONE;
  recdat.head[1] = FHTWO;

}

void EachMomentUpdateLaser(void) {
  millis_t ms = millis();
  if (ms > next_rts_update_ms) {
    #if ENABLED(POWER_LOSS_RECOVERY)
      // print the file before the power is off.
      if ((power_off_type_yes == 0) && lcd_sd_status && (recovery.info.recovery_flag == true)) {
        rts.sendData(exchangePageBase, exchangePageAddr);
        if (startprogress < 100)
          rts.sendData(startprogress, START_PROCESS_ICON_VP);
        //delay(30);
        if ((startprogress += 1) > 100) {
          rts.sendData(startSoundSet, soundAddr);
          power_off_type_yes = 1;
          for (uint16_t i = 0; i < cardRecBuf.Filesum; i++)
            if (!strcmp(cardRecBuf.Cardfilename[i], &recovery.info.sd_filename[1])) {
              rts.sendData(cardRecBuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);
              rts.sendData(cardRecBuf.Cardshowfilename[i], SELECT_FILE_TEXT_VP);
              rts.sendData(exchangePageBase + 27, exchangePageAddr);
              change_page_font = 27;
              break;
            }
        }
        return;
      }
    #elif ENABLED(CREALITY_POWER_LOSS)
      // print the file before the power is off.
      if ((power_off_type_yes == 0) && lcd_sd_status && (pre01_power_loss.info.recovery_flag == true)) {
        rts.sendData(exchangePageBase, exchangePageAddr);
        if (startprogress < 100)
          rts.sendData(startprogress, START_PROCESS_ICON_VP);
        //delay(30);
        if ((startprogress += 1) > 100) {
          rts.sendData(startSoundSet, soundAddr);
          power_off_type_yes = 1;
          for (uint16_t i = 0; i < cardRecBuf.Filesum; i++)
            if (!strcmp(cardRecBuf.Cardfilename[i], &pre01_power_loss.info.sd_filename[1])) {
              rts.sendData(cardRecBuf.Cardshowfilename[i], PRINT_FILE_TEXT_VP);
              rts.sendData(cardRecBuf.Cardshowfilename[i], SELECT_FILE_TEXT_VP);
              rts.sendData(exchangePageBase + 27, exchangePageAddr);
              change_page_font = 27;
              break;
            }
        }
        return;
      }
    #endif // if ENABLED(POWER_LOSS_RECOVERY)

    #if ENABLED(POWER_LOSS_RECOVERY)
      else if ((power_off_type_yes == 0) && (recovery.info.recovery_flag == false)) {
        rts.sendData(exchangePageBase, exchangePageAddr);
        if (startprogress < 100)
          rts.sendData(startprogress, START_PROCESS_ICON_VP);
        //delay(30);
        if ((startprogress += 1) > 100) {
          rts.sendData(startSoundSet, soundAddr);
          power_off_type_yes = 1;
          updateTimeValue  = RTS_UPDATE_VALUE;

          if (laser_device.is_laser_device()) {
            rts.sendData(exchangePageBase + 51, exchangePageAddr);
            change_page_font = 51;
          }
          else {
            rts.sendData(exchangePageBase + 1, exchangePageAddr);
            change_page_font = 1;
          }
          //rts.sendData(exchangePageBase + 51, exchangePageAddr);
          //change_page_font = 51;
        }
        return;
      }
      else {
        // need to optimize
        static unsigned char last_cardpercentValue = 100;
        if (card.isPrinting() && (last_cardpercentValue != card.percentDone())) {
          duration_t elapsed = print_job_timer.duration();

          rts.sendData(elapsed.value / 3600, PRINT_TIME_HOUR_VP);
          rts.sendData((elapsed.value % 3600) / 60, PRINT_TIME_MIN_VP);
          if ((unsigned char) card.percentDone() > 0) {
            percentrec = card.percentDone();
            if (percentrec <= 100)
              rts.sendData((unsigned char)percentrec, PRINT_PROCESS_ICON_VP);
          }
          else {
            rts.sendData(0, PRINT_PROCESS_ICON_VP);
          }
          rts.sendData((unsigned char)card.percentDone(), PRINT_PROCESS_VP);
          last_cardpercentValue = card.percentDone();
          rts.sendData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
        }

        if (pause_action_flag && (false == sdcard_pause_check) && printingIsPaused() && !planner.has_blocks_queued()) {
          pause_action_flag = false;
          //queue.enqueue_now(F("G0 F3000 X0 Y0"));
          laser_device.power = cutter.power;
          //SERIAL_ECHOPGM("laser_device.power=", laser_device.power);
          cutter.apply_power(0);

          rts.sendData(exchangePageBase + 61, exchangePageAddr);
          change_page_font = 61;
          waitway          = 0;
        }

        #if HAS_MEDIA
          if ((false == sdcard_pause_check) && (false == card.isPrinting()) && !planner.has_blocks_queued()) {
            if (CardReader::flag.mounted)
              rts.sendData(51, CHANGE_SDCARD_ICON_VP);
            else
              rts.sendData(0, CHANGE_SDCARD_ICON_VP);
          }
        #endif

        if (marlin_state == MF_RUNNING && first_start_laser == true) {
          char str_1[7], cmd[20] = {0};
          first_start_laser = false;
          sprintf_P(cmd, "G92.9 Z%s\n",  dtostrf(laser_device.laser_z_axis_high, 1, 2, str_1));
          //SERIAL_ECHOPGM(cmd);
          queue.inject(cmd);

          rts.sendData((float)(10 * laser_device.laser_z_axis_high), AXIS_Z_COORD_VP);
          delay(1);

        }
        else if (laser_device.laser_z_axis_high != current_position.z && first_start_laser == false) {
          laser_device.save_z_axis_high_to_eeprom(current_position.z);
        }

      }
    #elif ENABLED(CREALITY_POWER_LOSS)
      else if ((power_off_type_yes == 0) && (pre01_power_loss.info.recovery_flag == false)) {
        rts.sendData(exchangePageBase, exchangePageAddr);
        if (startprogress < 100)
          rts.sendData(startprogress, START_PROCESS_ICON_VP);
        //delay(30);
        if ((startprogress += 1) > 100) {
          rts.sendData(startSoundSet, soundAddr);
          power_off_type_yes = 1;
          updateTimeValue  = RTS_UPDATE_VALUE;

          if (laser_device.is_laser_device()) {
            rts.sendData(exchangePageBase + 51, exchangePageAddr);
            change_page_font = 51;
          }
          else {
            rts.sendData(exchangePageBase + 1, exchangePageAddr);
            change_page_font = 1;
          }
          //rts.sendData(exchangePageBase + 51, exchangePageAddr);
          //change_page_font = 51;
        }
        return;
      }
      else {
        // need to optimize
        static unsigned char last_cardpercentValue = 100;
        if (card.isPrinting() && (last_cardpercentValue != card.percentDone())) {
          duration_t elapsed = print_job_timer.duration();

          rts.sendData(elapsed.value / 3600, PRINT_TIME_HOUR_VP);
          rts.sendData((elapsed.value % 3600) / 60, PRINT_TIME_MIN_VP);
          if ((unsigned char) card.percentDone() > 0) {
            percentrec = card.percentDone();
            if (percentrec <= 100)
              rts.sendData((unsigned char)percentrec, PRINT_PROCESS_ICON_VP);
          }
          else {
            rts.sendData(0, PRINT_PROCESS_ICON_VP);
          }
          rts.sendData((unsigned char)card.percentDone(), PRINT_PROCESS_VP);
          last_cardpercentValue = card.percentDone();
          rts.sendData(10 * current_position[Z_AXIS], AXIS_Z_COORD_VP);
        }

        if (pause_action_flag && !sdcard_pause_check && printingIsPaused() && !planner.has_blocks_queued()) {
          pause_action_flag = false;
          //queue.enqueue_now(F("G0 F3000 X0 Y0"));
          laser_device.power = cutter.power;
          //SERIAL_ECHOPGM("laser_device.power=", laser_device.power);
          cutter.apply_power(0);

          rts.sendData(exchangePageBase + 61, exchangePageAddr);
          change_page_font = 61;
          waitway = 0;
        }

        #if HAS_MEDIA
          if (!sdcard_pause_check && !card.isPrinting() && !planner.has_blocks_queued())
            rts.sendData(CardReader::flag.mounted ? 51 : 0, CHANGE_SDCARD_ICON_VP);
        #endif

        if (marlin_state == MF_RUNNING && first_start_laser) {
          char str_1[7], cmd[20] = {0};
          first_start_laser = false;
          sprintf_P(cmd, PSTR("G92.9 Z%s\n"),  dtostrf(laser_device.laser_z_axis_high, 1, 2, str_1));
          //SERIAL_ECHOPGM(cmd);
          queue.inject(cmd);

          rts.sendData((float)(10 * laser_device.laser_z_axis_high), AXIS_Z_COORD_VP);
          delay(1);

        }
        else if (laser_device.laser_z_axis_high != current_position.z && !first_start_laser) {
          laser_device.save_z_axis_high_to_eeprom(current_position.z);
        }

      }
    #endif // POWER_LOSS_RECOVERY

    next_rts_update_ms = ms + RTS_UPDATE_INTERVAL + updateTimeValue;
  }
}

void RTSUpdateLaser(void) {
  // Check the status of card
  rts.sdCardUpate();

  EachMomentUpdateLaser();
  // wait to receive massage and response
  if (rts.receiveData() > 0)
    rts.handleDataLaser();
}

// 激光模式，跑边框
void HMI_Area_Move(void) {
  static uint8_t MINUNITMULT = 10;
  xy_uint_t move_scaled = { 0, 0 };

  laser_device.is_run_range = true; // 标志正在跑边框

  float x = laser_device.get_laser_range(LASER_MAX_X) - laser_device.get_laser_range(LASER_MIN_X),
        y = laser_device.get_laser_range(LASER_MAX_Y) - laser_device.get_laser_range(LASER_MIN_Y);

  xy_pos_t origin = current_position; // 记录当前位置

  move_scaled = current_position * MINUNITMULT;

  move_scaled.x += laser_device.get_laser_range(LASER_MIN_X) * MINUNITMULT;
  move_scaled.y += laser_device.get_laser_range(LASER_MIN_Y) * MINUNITMULT;

  LIMIT(move_scaled.x, (X_MIN_POS) * MINUNITMULT, (X_MAX_POS) * MINUNITMULT);
  LIMIT(move_scaled.y, (Y_MIN_POS) * MINUNITMULT, (Y_MAX_POS) * MINUNITMULT);
  current_position = move_scaled / MINUNITMULT;

  // 超出打印区域
  NOMORE(x, X_MAX_POS - current_position.x);
  NOMORE(y, Y_MAX_POS - current_position.y);

  // 先跑到最小位置
  //current_position.x += laser_device.get_laser_range(LASER_MIN_X);
  //current_position.y += laser_device.get_laser_range(LASER_MIN_Y);

  //HMI_Plan_Move(homing_feedrate(Y_AXIS));
  RTS_line_to_current(Y_AXIS);
  planner.synchronize();

  //current_position.y += y;
  move_scaled.y += y * MINUNITMULT;
  LIMIT(move_scaled.y, (Y_MIN_POS) * MINUNITMULT, (Y_MAX_POS) * MINUNITMULT);
  current_position.y = move_scaled.y / MINUNITMULT;

  laser_device.laser_power_start(5);
  RTS_line_to_current(Y_AXIS);  //HMI_Plan_Move(homing_feedrate(Y_AXIS));
  planner.synchronize();

  //current_position.x += x;
  move_scaled.x += x * MINUNITMULT;
  LIMIT(move_scaled.x, (X_MIN_POS) * MINUNITMULT, (X_MAX_POS) * MINUNITMULT);
  current_position.x = move_scaled.x / MINUNITMULT;

  RTS_line_to_current(X_AXIS);  //HMI_Plan_Move(homing_feedrate(X_AXIS));
  planner.synchronize();

  //current_position.y -= y;
  move_scaled.y -= y * MINUNITMULT;
  LIMIT(move_scaled.y, (Y_MIN_POS) * MINUNITMULT, (Y_MAX_POS) * MINUNITMULT);
  current_position.y = move_scaled.y / MINUNITMULT;

  RTS_line_to_current(Y_AXIS);  //HMI_Plan_Move(homing_feedrate(Y_AXIS));
  planner.synchronize();

  //current_position.x -= x;
  move_scaled.x -= x * MINUNITMULT;
  LIMIT(move_scaled.x, (X_MIN_POS) * MINUNITMULT, (X_MAX_POS) * MINUNITMULT);
  current_position.x = move_scaled.x / MINUNITMULT;

  RTS_line_to_current(X_AXIS);  //HMI_Plan_Move(homing_feedrate(X_AXIS));
  planner.synchronize();

  laser_device.laser_power_stop(); // 关闭激光

  // 回到原点位置 107011 -20211009
  //current_position = origin;
  move_scaled = origin * MINUNITMULT;
  LIMIT(move_scaled.x, (X_MIN_POS) * MINUNITMULT, (X_MAX_POS) * MINUNITMULT);
  LIMIT(move_scaled.y, (Y_MIN_POS) * MINUNITMULT, (Y_MAX_POS) * MINUNITMULT);

  current_position = move_scaled / MINUNITMULT;

  RTS_line_to_current(X_AXIS);  //HMI_Plan_Move(homing_feedrate(X_AXIS));
  planner.synchronize();

  laser_device.is_run_range = false;
}

#endif // HAS_LASER_E3S1PRO
