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

#include "../../Marlin.h"

#if ENABLED(EXTENSIBLE_UI)

#include "../../gcode/queue.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../module/printcounter.h"
#include "../../module/temperature.h"
#include "../../sd/cardreader.h"
#include "../../libs/duration_t.h"

#include "ui_api.h"

inline float clamp(const float value, const float minimum, const float maximum) {
  return MAX(MIN(value, maximum), minimum);
}

namespace Extensible_UI_API {

  float getActualTemp_celsius(const uint8_t extruder) {
    if (extruder)
      return thermalManager.degHotend(extruder-1);
    else
    #if HAS_HEATED_BED
        return thermalManager.degBed();
    #else
        return 0;
    #endif
  }

  float getTargetTemp_celsius(const uint8_t extruder) {
    if (extruder)
      return thermalManager.degTargetHotend(extruder-1);
    else
    #if HAS_HEATED_BED
        return thermalManager.degTargetBed();
    #else
        return 0;
    #endif
  }

  float getFan_percent(const uint8_t fan) { return ((float(fanSpeeds[fan]) + 1) * 100) / 256; }

  float getAxisPosition_mm(const axis_t axis) {
    switch(axis) {
      case X:  return current_position[X_AXIS];
      case Y:  return current_position[Y_AXIS];
      case Z:  return current_position[Z_AXIS];
      case E0: return current_position[E_AXIS];
      case E1: return current_position[E_AXIS+1];
      case E2: return current_position[E_AXIS+2];
      case E3: return current_position[E_AXIS+3];
      default: return 0;
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
      case E2: destination[E_AXIS+2] = position; break;
      case E3: destination[E_AXIS+3] = position; break;
    }

    float old_feedrate = feedrate_mm_s;
    feedrate_mm_s = _feedrate_mm_s;
    prepare_move_to_destination();
    feedrate_mm_s = old_feedrate;
  }

  bool isMoving() { return planner.has_blocks_queued(); }

  float getAxisSteps_per_mm(const axis_t axis) {
    switch(axis) {
      case X:  return planner.axis_steps_per_mm[X_AXIS];
      case Y:  return planner.axis_steps_per_mm[Y_AXIS];
      case Z:  return planner.axis_steps_per_mm[Z_AXIS];
      case E0: return planner.axis_steps_per_mm[E_AXIS];
      case E1: return planner.axis_steps_per_mm[E_AXIS+1];
      case E2: return planner.axis_steps_per_mm[E_AXIS+2];
      case E3: return planner.axis_steps_per_mm[E_AXIS+3];
      default: return 0;
    }
  }

  void setAxisSteps_per_mm(const axis_t axis, float steps_per_mm) {
    switch(axis) {
      case X:  planner.axis_steps_per_mm[X_AXIS]   = steps_per_mm; break;
      case Y:  planner.axis_steps_per_mm[Y_AXIS]   = steps_per_mm; break;
      case Z:  planner.axis_steps_per_mm[Z_AXIS]   = steps_per_mm; break;
      case E0: planner.axis_steps_per_mm[E_AXIS]   = steps_per_mm; break;
      case E1: planner.axis_steps_per_mm[E_AXIS+1] = steps_per_mm; break;
      case E2: planner.axis_steps_per_mm[E_AXIS+2] = steps_per_mm; break;
      case E3: planner.axis_steps_per_mm[E_AXIS+3] = steps_per_mm; break;
    }
  }

  float getAxisMaxFeedrate_mm_s(const axis_t axis) {
    switch(axis) {
      case X:  return planner.max_feedrate_mm_s[X_AXIS];
      case Y:  return planner.max_feedrate_mm_s[Y_AXIS];
      case Z:  return planner.max_feedrate_mm_s[Z_AXIS];
      case E0: return planner.max_feedrate_mm_s[E_AXIS];
      case E1: return planner.max_feedrate_mm_s[E_AXIS+1];
      case E2: return planner.max_feedrate_mm_s[E_AXIS+2];
      case E3: return planner.max_feedrate_mm_s[E_AXIS+3];
      default: return 0;
    }
  }

  void setAxisMaxFeedrate_mm_s(const axis_t axis, float max_feedrate_mm_s) {
    switch(axis) {
      case X:  planner.max_feedrate_mm_s[X_AXIS]   = max_feedrate_mm_s;
      case Y:  planner.max_feedrate_mm_s[Y_AXIS]   = max_feedrate_mm_s;
      case Z:  planner.max_feedrate_mm_s[Z_AXIS]   = max_feedrate_mm_s;
      case E0: planner.max_feedrate_mm_s[E_AXIS]   = max_feedrate_mm_s;
      case E1: planner.max_feedrate_mm_s[E_AXIS+1] = max_feedrate_mm_s;
      case E2: planner.max_feedrate_mm_s[E_AXIS+2] = max_feedrate_mm_s;
      case E3: planner.max_feedrate_mm_s[E_AXIS+3] = max_feedrate_mm_s;
      default: return;
    }
  }

  float getAxisMaxAcceleration_mm_s2(const axis_t axis) {
    switch(axis) {
      case X:  return planner.max_acceleration_mm_per_s2[X_AXIS];
      case Y:  return planner.max_acceleration_mm_per_s2[Y_AXIS];
      case Z:  return planner.max_acceleration_mm_per_s2[Z_AXIS];
      case E0: return planner.max_acceleration_mm_per_s2[E_AXIS];
      case E1: return planner.max_acceleration_mm_per_s2[E_AXIS+1];
      case E2: return planner.max_acceleration_mm_per_s2[E_AXIS+2];
      case E3: return planner.max_acceleration_mm_per_s2[E_AXIS+3];
      default: return 0;
    }
  }

  void setAxisMaxAcceleration_mm_s2(const axis_t axis, float max_acceleration_mm_per_s2) {
    switch(axis) {
      case X:  planner.max_acceleration_mm_per_s2[X_AXIS]   = max_acceleration_mm_per_s2;
      case Y:  planner.max_acceleration_mm_per_s2[Y_AXIS]   = max_acceleration_mm_per_s2;
      case Z:  planner.max_acceleration_mm_per_s2[Z_AXIS]   = max_acceleration_mm_per_s2;
      case E0: planner.max_acceleration_mm_per_s2[E_AXIS]   = max_acceleration_mm_per_s2;
      case E1: planner.max_acceleration_mm_per_s2[E_AXIS+1] = max_acceleration_mm_per_s2;
      case E2: planner.max_acceleration_mm_per_s2[E_AXIS+2] = max_acceleration_mm_per_s2;
      case E3: planner.max_acceleration_mm_per_s2[E_AXIS+3] = max_acceleration_mm_per_s2;
      default: return;
    }
  }

  float getAxisMaxJerk_mm_s(const axis_t axis) {
    switch(axis) {
      case X:  return planner.max_jerk[X_AXIS];
      case Y:  return planner.max_jerk[Y_AXIS];
      case Z:  return planner.max_jerk[Z_AXIS];
      case E0:
      case E1:
      case E2:
      case E3:
        return planner.max_jerk[E_AXIS];
      default: return 0;
    }
  }

  void setAxisMaxJerk_mm_s(const axis_t axis, float max_jerk) {
    switch(axis) {
      case X:  planner.max_jerk[X_AXIS]   = max_jerk;
      case Y:  planner.max_jerk[Y_AXIS]   = max_jerk;
      case Z:  planner.max_jerk[Z_AXIS]   = max_jerk;
      case E0:
      case E1:
      case E2:
      case E3:
        planner.max_jerk[E_AXIS]   = max_jerk;
        break;
      default: return;
    }
  }

  float getMinFeedrate_mm_s()                       { return planner.min_feedrate_mm_s; }
  float getMinTravelFeedrate_mm_s()                 { return planner.min_travel_feedrate_mm_s; }
  float getPrintingAcceleration_mm_s2()             { return planner.acceleration; }
  float getRetractAcceleration_mm_s2()              { return planner.retract_acceleration; }
  float getTravelAcceleration_mm_s2()               { return planner.travel_acceleration; }
  void setMinFeedrate_mm_s(float fr)                { planner.min_feedrate_mm_s = fr; }
  void setMinTravelFeedrate_mm_s(float fr)          { planner.min_travel_feedrate_mm_s = fr; }
  void setPrintingAcceleration_mm_per_s2(float acc) { planner.acceleration = acc; }
  void setRetractAcceleration_mm_s2(float acc)      { planner.retract_acceleration = acc; }
  void setTravelAcceleration_mm_s2(float acc)       { planner.travel_acceleration = acc; }

  #if HAS_BED_PROBE
    float getZOffset_mm()                           { return zprobe_zoffset; }

    void incrementZOffset_mm(float babystep_increment) {
      const float new_zoffset = zprobe_zoffset + babystep_increment;
      if (WITHIN(new_zoffset, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
        #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
          if (planner.leveling_active)
            thermalManager.babystep_axis(Z_AXIS, babystep_increment);
        #endif
        zprobe_zoffset = new_zoffset;
      }
    }
  #endif // HAS_BED_PROBE

  uint8_t getProgress_percent() {
    #if ENABLED(SDSUPPORT)
      return card.percentDone();
    #else
      return 0;
    #endif
  }

  uint32_t getProgress_seconds_elapsed() {
    const duration_t elapsed = print_job_timer.duration();
    return elapsed.value;
  }

  float getFeedRate_percent() {
    return feedrate_percentage;
  }

  void enqueueCommands(progmem_str gcode) {
    enqueue_and_echo_commands_P((const char*)gcode);
  }

  bool isAxisPositionKnown(const axis_t axis) {
    switch(axis) {
      case X:  return axis_known_position[X_AXIS];
      case Y:  return axis_known_position[Y_AXIS];
      case Z:  return axis_known_position[Z_AXIS];
      default: return true;
    }
  }

  progmem_str getFirmwareName() {
    return F("Marlin " SHORT_BUILD_VERSION);
  }

  void setTargetTemp_celsius(const uint8_t extruder, float temp) {
    if (extruder)
      thermalManager.setTargetHotend(clamp(temp,0,500), extruder-1);
    #if HAS_HEATED_BED
      else
        thermalManager.setTargetBed(clamp(temp,0,200));
    #endif
  }

  void setFan_percent(const uint8_t fan, float percent) {
    if (fan < FAN_COUNT)
      fanSpeeds[fan] = clamp(round(percent * 255 / 100), 0, 255);
  }

  void setFeedrate_percent(const float percent) {
    feedrate_percentage = clamp(percent, 10, 500);
  }

  void printFile(const char *filename) {
    #if ENABLED(SDSUPPORT)
      card.openAndPrintFile(filename);
    #endif
  }

  void changeDir(const char *dirname) {
    #if ENABLED(SDSUPPORT)
      card.chdir(dirname);
    #endif
  }

  uint16_t getFileCount() {
    #if ENABLED(SDSUPPORT)
      return card.get_num_Files();
    #else
      return 0;
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
    #else
      return true;
    #endif
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
    #if ENABLED(SDSUPPORT)
      return IS_SD_INSERTED;
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
      Extensible_UI_API::onStatusChanged(PSTR(MSG_PRINT_PAUSED));
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
      Extensible_UI_API::onStatusChanged(PSTR(MSG_PRINT_ABORTED));
    #endif
  }

  Media_Iterator::Media_Iterator(uint16_t start_index /* = 0*/) {
    #if ENABLED(SDSUPPORT)
      num_files = card.get_num_Files();
      index     = MIN(start_index, num_files-1);
      seek(index);
    #endif
  }

  bool Media_Iterator::hasMore() {
    return (index < (num_files - 1)) && (num_files > 0);
  }

  bool Media_Iterator::next() {
    #if ENABLED(SDSUPPORT)
      if (hasMore()) {
        index++;
        seek(index);
        return true;
      }
    #endif
    return false;
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

  const char* Media_Iterator::filename() {
    #if ENABLED(SDSUPPORT)
      return (card.longFilename && card.longFilename[0]) ? card.longFilename : card.filename;
    #else
      return "";
    #endif
  }

  const char* Media_Iterator::shortFilename() {
    #if ENABLED(SDSUPPORT)
      return card.filename;
    #else
      return "";
    #endif
  }

  const char* Media_Iterator::longFilename() {
    #if ENABLED(SDSUPPORT)
      return card.longFilename;
    #else
      return "";
    #endif
  }

  bool Media_Iterator::isDirectory() {
    #if ENABLED(SDSUPPORT)
      return card.filenameIsDir;
    #else
      return false;
    #endif
  }

} // namespace Extensible_UI_API

// At the moment, we piggy-back off the ultralcd calls, but this could be cleaned up in the future

void lcd_init() {
  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    SET_INPUT_PULLUP(SD_DETECT_PIN);
  #endif
  Extensible_UI_API::onStartup();
}

void lcd_update()                                                                {
  #if ENABLED(SDSUPPORT)
    static bool last_sd_status;
    const bool sd_status = Extensible_UI_API::isMediaInserted();
    if (sd_status != last_sd_status) {
      last_sd_status = sd_status;
      if (sd_status) {
        card.initsd();
        Extensible_UI_API::onMediaInserted();
      }
      else {
        card.release();
        Extensible_UI_API::onMediaRemoved();
      }
    }
  #endif // SDSUPPORT
  Extensible_UI_API::onUpdate();
}

bool lcd_hasstatus()                                                             { return true; }
bool lcd_detected()                                                              { return true; }
void lcd_reset_alert_level()                                                     {}
void lcd_refresh()                                                               {}
void lcd_setstatus(const char * const message, const bool persist /* = false */) { Extensible_UI_API::onStatusChanged(message); }
void lcd_setstatusPGM(const char * const message, int8_t level /* = 0 */)        { Extensible_UI_API::onStatusChanged((progmem_str)message); }
void lcd_reset_status()                                                          {}
void lcd_setalertstatusPGM(const char * const message)                           { lcd_setstatusPGM(message, 0); }
void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) {
  char buff[64];
  va_list args;
  va_start(args, fmt);
  vsnprintf_P(buff, sizeof(buff), fmt, args);
  va_end(args);
  buff[63] = '\0';
  Extensible_UI_API::onStatusChanged(buff);
}

#endif // EXTENSIBLE_UI