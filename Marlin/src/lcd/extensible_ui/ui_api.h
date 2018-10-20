/************
 * ui_api.h *
 ************/

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

#pragma once

#include "../../inc/MarlinConfig.h"

typedef const __FlashStringHelper *progmem_str;

namespace UI {

  enum axis_t : uint8_t { X, Y, Z, E0, E1, E2, E3, E4, E5 };

  constexpr uint8_t extruderCount = EXTRUDERS;
  constexpr uint8_t fanCount      = FAN_COUNT;

  // The following methods should be used by the extension module to
  // query or change Marlin's state.

  progmem_str getFirmwareName();

  bool isAxisPositionKnown(const axis_t axis);
  bool isMoving();

  float getActualTemp_celsius(const uint8_t extruder);
  float getTargetTemp_celsius(const uint8_t extruder);
  float getFan_percent(const uint8_t fan);
  float getAxisPosition_mm(const axis_t axis);
  float getAxisSteps_per_mm(const axis_t axis);
  float getAxisMaxFeedrate_mm_s(const axis_t axis);
  float getAxisMaxAcceleration_mm_s2(const axis_t axis);
  float getMinFeedrate_mm_s();
  float getMinTravelFeedrate_mm_s();
  float getPrintingAcceleration_mm_s2();
  float getRetractAcceleration_mm_s2();
  float getTravelAcceleration_mm_s2();
  float getFeedRate_percent();
  uint8_t getProgress_percent();
  uint32_t getProgress_seconds_elapsed();

  #if ENABLED(PRINTCOUNTER)
    char *getTotalPrints_str(char buffer[21]);
    char *getFinishedPrints_str(char buffer[21]);
    char *getTotalPrintTime_str(char buffer[21]);
    char *getLongestPrint_str(char buffer[21]);
    char *getFilamentUsed_str(char buffer[21]);
  #endif

  void setTargetTemp_celsius(const uint8_t extruder, float temp);
  void setFan_percent(const uint8_t fan, const float percent);
  void setAxisPosition_mm(const axis_t axis, float position, float _feedrate_mm_s);
  void setAxisSteps_per_mm(const axis_t axis, const float steps_per_mm);
  void setAxisMaxFeedrate_mm_s(const axis_t axis, const float max_feedrate_mm_s);
  void setAxisMaxAcceleration_mm_s2(const axis_t axis, const float max_acceleration_mm_per_s2);
  void setMinFeedrate_mm_s(const float min_feedrate_mm_s);
  void setMinTravelFeedrate_mm_s(const float min_travel_feedrate_mm_s);
  void setPrintingAcceleration_mm_s2(const float acceleration);
  void setRetractAcceleration_mm_s2(const float retract_acceleration);
  void setTravelAcceleration_mm_s2(const float travel_acceleration);
  void setFeedrate_percent(const float percent);

  #if ENABLED(LIN_ADVANCE)
    float getLinearAdvance_mm_mm_s(const uint8_t extruder);
    void setLinearAdvance_mm_mm_s(const uint8_t extruder, const float k);
  #endif

  #if ENABLED(JUNCTION_DEVIATION)
    float getJunctionDeviation_mm();
    void setJunctionDeviation_mm(const float junc_dev);
  #else
    float getAxisMaxJerk_mm_s(const axis_t axis);
    void setAxisMaxJerk_mm_s(const axis_t axis, const float max_jerk);
  #endif

  void setActiveTool(uint8_t extruder, bool no_move);
  uint8_t getActiveTool();

  #if HOTENDS > 1
    float getNozzleOffset_mm(const axis_t axis, uint8_t extruder);
    void setNozzleOffset_mm(const axis_t axis, uint8_t extruder, float offset);
  #endif

  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    float getZOffset_mm();
    void setZOffset_mm(const float zoffset_mm);
    void incrementZOffset_steps(const int16_t babystep_increment);
  #endif

  #if ENABLED(BACKLASH_GCODE)
    float getAxisBacklash_mm(const axis_t axis);
    void setAxisBacklash_mm(const axis_t axis, float distance);

    float getBacklashCorrection_percent();
    void setBacklashCorrection_percent(float percent);

    #ifdef BACKLASH_SMOOTHING_MM
      float getBacklashSmoothing_mm();
      void setBacklashSmoothing_mm(float distance);
    #endif
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    bool isFilamentRunoutEnabled();
    void toggleFilamentRunout(const bool state);

    #if FILAMENT_RUNOUT_DISTANCE_MM > 0
      float getFilamentRunoutDistance_mm();
      void setFilamentRunoutDistance_mm(const float distance);
    #endif
  #endif

  void delay_ms(unsigned long ms);
  void yield(); // Within lengthy loop, call this periodically

  void enqueueCommands(progmem_str gcode);

  void printFile(const char *filename);
  bool isPrintingFromMediaPaused();
  bool isPrintingFromMedia();
  bool isPrinting();
  void stopPrint();
  void pausePrint();
  void resumePrint();

  bool isMediaInserted();

  class FileList {
    private:
      uint16_t num_files;

    public:
      FileList();
      void        refresh();
      bool        seek(uint16_t, bool skip_range_check = false);

      const char *longFilename();
      const char *shortFilename();
      const char *filename();
      bool        isDir();

      void        changeDir(const char *dirname);
      void        upDir();
      bool        isAtRootDir();
      uint16_t    count();
  };

  // The following event handlers are to be declared by the extension
  // module and will be called by Marlin.

  void onStartup();
  void onIdle();
  void onMediaInserted();
  void onMediaError();
  void onMediaRemoved();
  void onPlayTone(const uint16_t frequency, const uint16_t duration);
  void onPrinterKilled(const char* msg);
  void onPrintTimerStarted();
  void onPrintTimerPaused();
  void onPrintTimerStopped();
  void onFilamentRunout();
  void onStatusChanged(const char* msg);
  void onStatusChanged(progmem_str msg);
  void onFactoryReset();
  void onStoreSettings();
  void onLoadSettings();
};
