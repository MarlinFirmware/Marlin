/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

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

#include "../../inc/MarlinConfig.h"

namespace ExtUI {

  // The ExtUI implementation can store up to this many bytes
  // in the EEPROM when the methods onStoreSettings and
  // onLoadSettings are called.

  static constexpr size_t eeprom_data_size = 48;

  enum axis_t     : uint8_t { X, Y, Z, X2, Y2, Z2, Z3, Z4 };
  enum extruder_t : uint8_t { E0, E1, E2, E3, E4, E5, E6, E7 };
  enum heater_t   : uint8_t { H0, H1, H2, H3, H4, H5, BED, CHAMBER };
  enum fan_t      : uint8_t { FAN0, FAN1, FAN2, FAN3, FAN4, FAN5, FAN6, FAN7 };
  enum result_t   : uint8_t { PID_BAD_EXTRUDER_NUM, PID_TEMP_TOO_HIGH, PID_TUNING_TIMEOUT, PID_DONE };

  constexpr uint8_t extruderCount = EXTRUDERS;
  constexpr uint8_t hotendCount   = HOTENDS;
  constexpr uint8_t fanCount      = FAN_COUNT;

  #if HAS_MESH
    typedef float bed_mesh_t[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
  #endif

  bool isMoving();
  bool isAxisPositionKnown(const axis_t);
  bool isAxisPositionKnown(const extruder_t);
  bool isPositionKnown(); // Axis position guaranteed, steppers active since homing
  bool isMachineHomed(); // Axis position most likely correct, steppers may have deactivated
  bool canMove(const axis_t);
  bool canMove(const extruder_t);
  void injectCommands_P(PGM_P const);
  bool commandsInQueue();

  bool isHeaterIdle(const heater_t);
  bool isHeaterIdle(const extruder_t);
  void enableHeater(const heater_t);
  void enableHeater(const extruder_t);

  #if ENABLED(JOYSTICK)
    void jog(const xyz_float_t &dir);
    void _joystick_update(xyz_float_t &norm_jog);
  #endif

  /**
   * Getters and setters
   * Should be used by the EXTENSIBLE_UI to query or change Marlin's state.
   */
  PGM_P getFirmwareName_str();

  #if HAS_SOFTWARE_ENDSTOPS
    bool getSoftEndstopState();
    void setSoftEndstopState(const bool);
  #endif

  #if HAS_TRINAMIC_CONFIG
    float getAxisCurrent_mA(const axis_t);
    float getAxisCurrent_mA(const extruder_t);
    void  setAxisCurrent_mA(const float, const axis_t);
    void  setAxisCurrent_mA(const float, const extruder_t);

     int getTMCBumpSensitivity(const axis_t);
    void setTMCBumpSensitivity(const float, const axis_t);
  #endif

  float getActualTemp_celsius(const heater_t);
  float getActualTemp_celsius(const extruder_t);
  float getTargetTemp_celsius(const heater_t);
  float getTargetTemp_celsius(const extruder_t);
  float getTargetFan_percent(const fan_t);
  float getActualFan_percent(const fan_t);
  float getAxisPosition_mm(const axis_t);
  float getAxisPosition_mm(const extruder_t);
  float getAxisSteps_per_mm(const axis_t);
  float getAxisSteps_per_mm(const extruder_t);
  feedRate_t getAxisMaxFeedrate_mm_s(const axis_t);
  feedRate_t getAxisMaxFeedrate_mm_s(const extruder_t);
  float getAxisMaxAcceleration_mm_s2(const axis_t);
  float getAxisMaxAcceleration_mm_s2(const extruder_t);
  feedRate_t getMinFeedrate_mm_s();
  feedRate_t getMinTravelFeedrate_mm_s();
  float getPrintingAcceleration_mm_s2();
  float getRetractAcceleration_mm_s2();
  float getTravelAcceleration_mm_s2();
  float getFeedrate_percent();
  int16_t getFlowPercentage(const extruder_t);
  uint8_t getProgress_percent();
  uint32_t getProgress_seconds_elapsed();

  #if HAS_LEVELING
    bool getLevelingActive();
    void setLevelingActive(const bool);
    bool getMeshValid();
    #if HAS_MESH
      bed_mesh_t& getMeshArray();
      float getMeshPoint(const xy_uint8_t &pos);
      void setMeshPoint(const xy_uint8_t &pos, const float zval);
      void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval);
      inline void onMeshUpdate(const xy_int8_t &pos, const float zval) { onMeshUpdate(pos.x, pos.y, zval); }

      typedef enum : unsigned char { PROBE_START, PROBE_FINISH } probe_state_t;
      void onMeshUpdate(const int8_t xpos, const int8_t ypos, probe_state_t state);
      inline void onMeshUpdate(const xy_int8_t &pos, probe_state_t state) { onMeshUpdate(pos.x, pos.y, state); }
    #endif
  #endif

  #if ENABLED(HOST_PROMPT_SUPPORT)
    void setHostResponse(const uint8_t);
  #endif

  #if ENABLED(PRINTCOUNTER)
    char* getTotalPrints_str(char buffer[21]);
    char* getFinishedPrints_str(char buffer[21]);
    char* getTotalPrintTime_str(char buffer[21]);
    char* getLongestPrint_str(char buffer[21]);
    char* getFilamentUsed_str(char buffer[21]);
  #endif

  void setTargetTemp_celsius(const float, const heater_t);
  void setTargetTemp_celsius(const float, const extruder_t);
  void setTargetFan_percent(const float, const fan_t);
  void setAxisPosition_mm(const float, const axis_t);
  void setAxisPosition_mm(const float, const extruder_t);
  void setAxisSteps_per_mm(const float, const axis_t);
  void setAxisSteps_per_mm(const float, const extruder_t);
  void setAxisMaxFeedrate_mm_s(const feedRate_t, const axis_t);
  void setAxisMaxFeedrate_mm_s(const feedRate_t, const extruder_t);
  void setAxisMaxAcceleration_mm_s2(const float, const axis_t);
  void setAxisMaxAcceleration_mm_s2(const float, const extruder_t);
  void setFeedrate_mm_s(const feedRate_t);
  void setMinFeedrate_mm_s(const feedRate_t);
  void setMinTravelFeedrate_mm_s(const feedRate_t);
  void setPrintingAcceleration_mm_s2(const float);
  void setRetractAcceleration_mm_s2(const float);
  void setTravelAcceleration_mm_s2(const float);
  void setFeedrate_percent(const float);
  void setFlow_percent(const int16_t, const extruder_t);
  void setUserConfirmed();

  #if ENABLED(LIN_ADVANCE)
    float getLinearAdvance_mm_mm_s(const extruder_t);
    void setLinearAdvance_mm_mm_s(const float, const extruder_t);
  #endif

  #if HAS_JUNCTION_DEVIATION
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

  #if ENABLED(BABYSTEPPING)
    int16_t mmToWholeSteps(const float mm, const axis_t axis);

    bool babystepAxis_steps(const int16_t steps, const axis_t axis);
    void smartAdjustAxis_steps(const int16_t steps, const axis_t axis, bool linked_nozzles);
  #endif

  #if HAS_HOTEND_OFFSET
    float getNozzleOffset_mm(const axis_t, const extruder_t);
    void setNozzleOffset_mm(const float, const axis_t, const extruder_t);
    void normalizeNozzleOffset(const axis_t axis);
  #endif

  float getZOffset_mm();
  void setZOffset_mm(const float);

  #if HAS_BED_PROBE
    float getProbeOffset_mm(const axis_t);
    void setProbeOffset_mm(const float, const axis_t);
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

  #if HAS_FILAMENT_SENSOR
    bool getFilamentRunoutEnabled();
    void setFilamentRunoutEnabled(const bool);

    #ifdef FILAMENT_RUNOUT_DISTANCE_MM
      float getFilamentRunoutDistance_mm();
      void setFilamentRunoutDistance_mm(const float);
    #endif
  #endif

  #if ENABLED(CASE_LIGHT_ENABLE)
    bool getCaseLightState();
    void setCaseLightState(const bool);

    #if DISABLED(CASE_LIGHT_NO_BRIGHTNESS)
      float getCaseLightBrightness_percent();
      void setCaseLightBrightness_percent(const float);
    #endif
  #endif

  #if ENABLED(PIDTEMP)
    float getPIDValues_Kp(const extruder_t);
    float getPIDValues_Ki(const extruder_t);
    float getPIDValues_Kd(const extruder_t);
    void setPIDValues(const float, const float, const float, extruder_t);
    void startPIDTune(const float, extruder_t);
  #endif

  #if ENABLED(PIDTEMPBED)
    float getBedPIDValues_Kp();
    float getBedPIDValues_Ki();
    float getBedPIDValues_Kd();
    void setBedPIDValues(const float, const float, const float);
    void startBedPIDTune(const float);
  #endif

  /**
   * Delay and timing routines
   * Should be used by the EXTENSIBLE_UI to safely pause or measure time
   * safe_millis must be called at least every 1 sec to guarantee time
   * yield should be called within lengthy loops
   */
  #ifdef __SAM3X8E__
    uint32_t safe_millis();
  #else
    FORCE_INLINE uint32_t safe_millis() { return millis(); } // TODO: Implement for AVR
  #endif

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
      bool seek(const uint16_t, const bool skip_range_check = false);

      const char *longFilename();
      const char *shortFilename();
      const char *filename();
      bool isDir();

      void changeDir(const char * const dirname);
      void upDir();
      bool isAtRootDir();
      uint16_t count();
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
  void onPrinterKilled(PGM_P const error, PGM_P const component);
  void onPrintTimerStarted();
  void onPrintTimerPaused();
  void onPrintTimerStopped();
  void onFilamentRunout(const extruder_t extruder);
  void onUserConfirmRequired(const char * const msg);
  void onUserConfirmRequired_P(PGM_P const pstr);
  void onStatusChanged(const char * const msg);
  void onStatusChanged_P(PGM_P const pstr);
  void onFactoryReset();
  void onStoreSettings(char *);
  void onLoadSettings(const char *);
  void onConfigurationStoreWritten(bool success);
  void onConfigurationStoreRead(bool success);
  #if ENABLED(POWER_LOSS_RECOVERY)
    void onPowerLossResume();
  #endif
  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst);
  #endif
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
#define UI_INCREMENT_BY(method, inc, ...) ExtUI::set ## method(ExtUI::get ## method (__VA_ARGS__) + inc, ##__VA_ARGS__)
#define UI_DECREMENT_BY(method, inc, ...) ExtUI::set ## method(ExtUI::get ## method (__VA_ARGS__) - inc, ##__VA_ARGS__)

#define UI_INCREMENT(method, ...) UI_INCREMENT_BY(method, increment, ##__VA_ARGS__)
#define UI_DECREMENT(method, ...) UI_DECREMENT_BY(method, increment, ##__VA_ARGS__)
