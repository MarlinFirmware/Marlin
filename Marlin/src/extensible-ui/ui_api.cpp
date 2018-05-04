/**************
 * ui_api.cpp *
 **************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../Marlin.h"

#if defined(FTDI_EVE_TOUCHSCREEN)
#include "../gcode/queue.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../module/probe.h"
#include "../module/printcounter.h"
#include "../module/temperature.h"
#include "../sd/cardreader.h"
#include "../libs/duration_t.h"

#include "ui_api.h"

namespace Marlin_LCD_API {
  float clamp(float value, float minimum, float maximum) {
    return max(min(value,maximum),minimum);
  };

  float getActualTemp_celsius(const uint8_t extruder) {
    if(extruder) {
      return thermalManager.degHotend(extruder-1);
    } else {
      return thermalManager.degBed();
    }
  }

  float getTargetTemp_celsius(const uint8_t extruder) {
    if(extruder) {
      return thermalManager.degTargetHotend(extruder-1);
    } else {
      return thermalManager.degTargetBed();
    }
  }

  float getFan_percent(const uint8_t fan) {
   return ((float(fanSpeeds[fan]) + 1) * 100) / 256;
  }

  float getAxisPosition_mm(const axis_t axis) {
    switch(axis) {
      case X:  return current_position[X_AXIS];  break;
      case Y:  return current_position[Y_AXIS];  break;
      case Z:  return current_position[Z_AXIS];  break;
      case E0: return current_position[E_AXIS]; break;
      case E1: return current_position[E_AXIS+1]; break;
    }
  }

  void setAxisPosition_mm(const axis_t axis, float position, float _feedrate_mm_s) {
    set_destination_from_current();
    switch(axis) {
      case X:  destination[X_AXIS]   = position; break;
      case Y:  destination[Y_AXIS]   = position; break;
      case Z:  destination[Z_AXIS]   = position; break;
      case E0: destination[E_AXIS]   = position; break;
      case E1: destination[E_AXIS+1] = position; break;
    }

    float old_feedrate = feedrate_mm_s;
    feedrate_mm_s = _feedrate_mm_s;
    prepare_move_to_destination();
    feedrate_mm_s = old_feedrate;
  }

  bool isMoving() {
    return planner.has_blocks_queued();
  }

  float getAxisSteps_per_mm(const axis_t axis) {
    switch(axis) {
      case X:  return planner.axis_steps_per_mm[X_AXIS];
      case Y:  return planner.axis_steps_per_mm[Y_AXIS];
      case Z:  return planner.axis_steps_per_mm[Z_AXIS];
      case E0: return planner.axis_steps_per_mm[E_AXIS];
      case E1: return planner.axis_steps_per_mm[E_AXIS+1];
    }
  }

  void setAxisSteps_per_mm(const axis_t axis, float steps_per_mm) {
    switch(axis) {
      case X:  planner.axis_steps_per_mm[X_AXIS]   = steps_per_mm; break;
      case Y:  planner.axis_steps_per_mm[Y_AXIS]   = steps_per_mm; break;
      case Z:  planner.axis_steps_per_mm[Z_AXIS]   = steps_per_mm; break;
      case E0: planner.axis_steps_per_mm[E_AXIS]   = steps_per_mm; break;
      case E1: planner.axis_steps_per_mm[E_AXIS+1] = steps_per_mm; break;
    }
  }


  void incrementZOffset_mm(float babystep_increment) {
    const float new_zoffset = zprobe_zoffset + babystep_increment;
    if (WITHIN(new_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
      #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
        if (planner.leveling_active) {
          thermalManager.babystep_axis(Z_AXIS, babystep_increment);
        }
      #endif
      zprobe_zoffset = new_zoffset;
    }
  }

  uint8_t getProgress_percent() {
    #if ENABLED(SDSUPPORT)
      return card.percentDone();
    #endif
  }

  uint32_t getProgress_seconds_elapsed() {
    const duration_t elapsed = print_job_timer.duration();
    return elapsed.value;
  }

  float getFeedRate_percent() {
    return feedrate_percentage;
  }

  float getZOffset_mm() {
    return zprobe_zoffset;
  }

  void runGCode(progmem_str gcode) {
    enqueue_and_echo_commands_P((const char*)gcode);
  }

  bool isAxisPositionKnown(const axis_t axis) {
    switch(axis) {
      case X:  return axis_known_position[X_AXIS];  break;
      case Y:  return axis_known_position[Y_AXIS];  break;
      case Z:  return axis_known_position[Z_AXIS];  break;
    }
  }

  progmem_str getFirmwareName() {
    return F("Marlin " SHORT_BUILD_VERSION LULZBOT_FW_VERSION);
  }

  void setTargetTemp_celsius(const uint8_t extruder, float temp) {
    if(extruder) {
      thermalManager.setTargetHotend(clamp(temp,0,500), extruder-1);
    } else {
      thermalManager.setTargetBed(clamp(temp,0,200));
    }
  }

  void setFan_percent(const uint8_t fan, float percent) {
    if (fan < FAN_COUNT) {
      fanSpeeds[fan] = clamp(round(percent*256/100-1), 0, 255);
    }
  }

  void setFeedrate_percent(const float percent) {
    feedrate_percentage = clamp(percent, 10, 500);
  }

  void printFromSDCard(const char *filename) {
    #if ENABLED(SDSUPPORT)
      card.openAndPrintFile(filename);
    #endif
  }

  void changeDir(const char *dirname) {
    #if ENABLED(SDSUPPORT)
      card.chdir(dirname);
    #endif
  }

  void upDir() {
    #if ENABLED(SDSUPPORT)
      card.updir();
    #endif
  }

  bool isAtRootDir() {
    #if ENABLED(SDSUPPORT)
      card.getWorkDirName();
      return card.filename[0] == '/';
    #endif
  }

  void lcd_setstatusPGM(const char * const message, int8_t level /* = 0 */);

  static uint8_t lcd_sd_status;

  void initMedia() {
    #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
      SET_INPUT_PULLUP(SD_DETECT_PIN);
      lcd_sd_status = 2; // UNKNOWN
    #endif
  }

  void checkMedia() {
    #if (ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)) || defined(LULZBOT_USE_USB_STICK)
      const bool sd_status = isMediaInserted();
      if (sd_status != lcd_sd_status) {

        SERIAL_PROTOCOLLNPAIR("SD Status: ", sd_status);

        if (sd_status) {
          card.initsd();
          if (lcd_sd_status != 2) onCardInserted();
          SERIAL_PROTOCOLLNPAIR("Card: ",1);
        }
        else {
          card.release();
          if (lcd_sd_status != 2) onCardRemoved();
          SERIAL_PROTOCOLLNPAIR("Card: ",0);
        }

        lcd_sd_status = sd_status;
      }
    #endif // SDSUPPORT && SD_DETECT_PIN
  }

  bool isPrintingFromMedia() {
    #if ENABLED(SDSUPPORT)
      return card.cardOK && card.isFileOpen() && card.sdprinting;
    #else
      return false;
    #endif
  }

  bool isPrinting() {
    return (planner.movesplanned() || IS_SD_PRINTING ||
    #if ENABLED(SDSUPPORT)
      (card.cardOK && card.isFileOpen())
    #else
      false
    #endif
    );
  }

  bool isMediaInserted() {
    #if (ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)) || defined(LULZBOT_USE_USB_STICK)
      #if defined(LULZBOT_USE_USB_STICK)
        return Sd2Card::isInserted();
      #else
        return IS_SD_INSERTED;
      #endif
    #else
      return false;
    #endif
  }

  void pausePrint() {
    #if ENABLED(SDSUPPORT)
      card.pauseSDPrint();
      print_job_timer.pause();
      #if ENABLED(PARK_HEAD_ON_PAUSE)
        enqueue_and_echo_commands_P(PSTR("M125"));
      #endif
      lcd_setstatusPGM(PSTR(MSG_PRINT_PAUSED), -1);
    #endif
  }

  void resumePrint() {
    #if ENABLED(SDSUPPORT)
      #if ENABLED(PARK_HEAD_ON_PAUSE)
        enqueue_and_echo_commands_P(PSTR("M24"));
      #else
        card.startFileprint();
        print_job_timer.start();
      #endif
    #endif
  }

  void stopPrint() {
    #if ENABLED(SDSUPPORT)
      card.stopSDPrint();
      clear_command_queue();
      quickstop_stepper();
      print_job_timer.stop();
      thermalManager.disable_all_heaters();
      #if FAN_COUNT > 0
        for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
      #endif
      wait_for_heatup = false;
      lcd_setstatusPGM(PSTR(MSG_PRINT_ABORTED), -1);
    #endif
  }

  Media_Iterator::Media_Iterator(uint16_t start_index /* = 0*/) {
    #if ENABLED(SDSUPPORT)
    num_files = card.get_num_Files();
    index     = min(start_index, num_files-1);
    seek(index);
    #endif
  }

  bool Media_Iterator::hasMore() {
    return (index < (num_files - 1)) && (num_files > 0);
  }

  void Media_Iterator::next() {
    #if ENABLED(SDSUPPORT)
    if(hasMore()) {
      index++;
      seek(index);
    }
    #endif
  }

  void Media_Iterator::seek(uint16_t index) {
    #if ENABLED(SDSUPPORT)
      const uint16_t nr =
        #if ENABLED(SDCARD_RATHERRECENTFIRST) && DISABLED(SDCARD_SORT_ALPHA)
          num_files - 1 -
        #endif
      index;

      #if ENABLED(SDCARD_SORT_ALPHA)
        card.getfilename_sorted(nr);
      #else
        card.getfilename(nr);
      #endif
    #endif
  }

  const char *Media_Iterator::filename() {
    #if ENABLED(SDSUPPORT)
      return (card.longFilename && card.longFilename[0]) ? card.longFilename : card.filename;
    #else
      return "";
    #endif
  }

  const char *Media_Iterator::shortFilename() {
    #if ENABLED(SDSUPPORT)
      return card.filename;
    #else
      return "";
    #endif
  }

  const char *Media_Iterator::longFilename() {
    #if ENABLED(SDSUPPORT)
      return card.longFilename;
    #else
      return "";
    #endif
  }

  bool Media_Iterator::isDirectory() {
    #if ENABLED(SDSUPPORT)
      return card.filenameIsDir;
    #endif
  }
}

#endif // FTDI_EVE_TOUCHSCREEN