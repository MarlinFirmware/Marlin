/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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

  enum axis_t     : uint8_t { X, Y, Z };
  enum extruder_t : uint8_t { E0, E1, E2, E3, E4, E5 };
  enum heater_t   : uint8_t { H0, H1, H2, H3, H4, H5, BED };
  enum fan_t      : uint8_t { FAN0, FAN1, FAN2, FAN3, FAN4, FAN5 };

  constexpr uint8_t extruderCount = EXTRUDERS;
  constexpr uint8_t hotendCount   = HOTENDS;
  constexpr uint8_t fanCount      = FAN_COUNT;

  bool isMoving();
  bool isAxisPositionKnown(const axis_t);
  bool canMove(const axis_t);
  bool canMove(const extruder_t);
  void enqueueCommands(progmem_str);

  /**
   * Getters and setters
   * Should be used by the EXTENSIBLE_UI to query or change Marlin's state.
   */
  progmem_str getFirmwareName_str();

  float getActualTemp_celsius(const heater_t);
  float getActualTemp_celsius(const extruder_t);
  float getTargetTemp_celsius(const heater_t);
  float getTargetTemp_celsius(const extruder_t);
  float getFan_percent(const fan_t);
  float getAxisPosition_mm(const axis_t);
  float getAxisPosition_mm(const extruder_t);
  float getAxisSteps_per_mm(const axis_t);
  float getAxisSteps_per_mm(const extruder_t);
  float getAxisMaxFeedrate_mm_s(const axis_t);
  float getAxisMaxFeedrate_mm_s(const extruder_t);
  float getAxisMaxAcceleration_mm_s2(const axis_t);
  float getAxisMaxAcceleration_mm_s2(const extruder_t);
  float getMinFeedrate_mm_s();
  float getMinTravelFeedrate_mm_s();
  float getPrintingAcceleration_mm_s2();
  float getRetractAcceleration_mm_s2();
  float getTravelAcceleration_mm_s2();
  float getFeedrate_percent();
  uint8_t getProgress_percent();
  uint32_t getProgress_seconds_elapsed();

  #if ENABLED(PRINTCOUNTER)
    char* getTotalPrints_str(char buffer[21]);
    char* getFinishedPrints_str(char buffer[21]);
    char* getTotalPrintTime_str(char buffer[21]);
    char* getLongestPrint_str(char buffer[21]);
    char* getFilamentUsed_str(char buffer[21]);
  #endif

  void setTargetTemp_celsius(const float, const heater_t);
  void setTargetTemp_celsius(const float, const extruder_t);
  void setFan_percent(const float, const fan_t);
  void setAxisPosition_mm(const float, const axis_t);
  void setAxisPosition_mm(const float, const extruder_t);
  void setAxisSteps_per_mm(const float, const axis_t);
  void setAxisSteps_per_mm(const float, const extruder_t);
  void setAxisMaxFeedrate_mm_s(const float, const axis_t);
  void setAxisMaxFeedrate_mm_s(const float, const extruder_t);
  void setAxisMaxAcceleration_mm_s2(const float, const axis_t);
  void setAxisMaxAcceleration_mm_s2(const float, const extruder_t);
  void setFeedrate_mm_s(const float);
  void setMinFeedrate_mm_s(const float);
  void setMinTravelFeedrate_mm_s(const float);
  void setPrintingAcceleration_mm_s2(const float);
  void setRetractAcceleration_mm_s2(const float);
  void setTravelAcceleration_mm_s2(const float);
  void setFeedrate_percent(const float);

  #if ENABLED(LIN_ADVANCE)
    float getLinearAdvance_mm_mm_s(const extruder_t);
    void setLinearAdvance_mm_mm_s(const float, const extruder_t);
  #endif

  #if ENABLED(JUNCTION_DEVIATION)
    float getJunctionDeviation_mm();
    void setJunctionDeviation_mm(const float);
  #else
    float getAxisMaxJerk_mm_s(const axis_t);
    float getAxisMaxJerk_mm_s(const extruder_t);
    void setAxisMaxJerk_mm_s(const float, const axis_t);
    void setAxisMaxJerk_mm_s(const float, const extruder_t);
  #endif

  extruder_t getActiveTool();
  void setActiveTool(const extruder_t, bool no_move);


  #if HOTENDS > 1
    float getNozzleOffset_mm(const axis_t, const extruder_t);
    void setNozzleOffset_mm(const float, const axis_t, const extruder_t);
  #endif

  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    float getZOffset_mm();
    void setZOffset_mm(const float);
    void addZOffset_steps(const int16_t);
  #endif

  #if ENABLED(BACKLASH_GCODE)
    float getAxisBacklash_mm(const axis_t);
    void setAxisBacklash_mm(const float, const axis_t);

    float getBacklashCorrection_percent();
    void setBacklashCorrection_percent(const float);

    #ifdef BACKLASH_SMOOTHING_MM
      float getBacklashSmoothing_mm();
      void setBacklashSmoothing_mm(const float);
    #endif
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    bool getFilamentRunoutEnabled();
    void setFilamentRunoutEnabled(const bool);

    #if FILAMENT_RUNOUT_DISTANCE_MM > 0
      float getFilamentRunoutDistance_mm();
      void setFilamentRunoutDistance_mm(const float);
    #endif
  #endif

  /**
   * Delay and timing routines
   * Should be used by the EXTENSIBLE_UI to safely pause or measure time
   * safe_millis must be called at least every 1 sec to guarantee time
   * yield should be called within lengthy loops
   */
  uint32_t safe_millis();
  void delay_us(unsigned long us);
  void delay_ms(unsigned long ms);
  void yield();

  /**
   * Media access routines
   *
   * Should be used by the EXTENSIBLE_UI to operate on files
   */
  bool isMediaInserted();
  bool isPrintingFromMediaPaused();
  bool isPrintingFromMedia();
  bool isPrinting();

  void printFile(const char *filename);
  void stopPrint();
  void pausePrint();
  void resumePrint();

  class FileList {
    private:
      uint16_t num_files;

    public:
      FileList();
      void refresh();
      bool seek(uint16_t, bool skip_range_check = false);

      const char *longFilename();
      const char *shortFilename();
      const char *filename();
      bool isDir();

      void changeDir(const char *dirname);
      void upDir();
      bool isAtRootDir();
      uint16_t    count();
  };

  /**
   * Event callback routines
   *
   * Should be declared by EXTENSIBLE_UI and will be called by Marlin
   */
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

/**
 * Helper macros to increment or decrement a value. For example:
 *
 *   UI_INCREMENT_BY(TargetTemp_celsius, 10, E0)
 *
 * Expands to:
 *
 *   setTargetTemp_celsius(getTargetTemp_celsius(E0) + 10, E0);
 *
 * Or, in the case where a constant increment is desired:
 *
 *   constexpr float increment = 10;
 *
 *   UI_INCREMENT(TargetTemp_celsius, E0)
 *
 */
#define UI_INCREMENT_BY(method, inc, ...) UI::set ## method(UI::get ## method (__VA_ARGS__) + inc, ##__VA_ARGS__)
#define UI_DECREMENT_BY(method, inc, ...) UI::set ## method(UI::get ## method (__VA_ARGS__) - inc, ##__VA_ARGS__)

#define UI_INCREMENT(method, ...) UI_INCREMENT_BY(method, increment, ##__VA_ARGS__)
#define UI_DECREMENT(method, ...) UI_DECREMENT_BY(method, increment, ##__VA_ARGS__)
