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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../../inc/MarlinConfig.h"
#include "../marlinui.h"
#include "../../gcode/gcode.h"
#if M600_PURGE_MORE_RESUMABLE
  #include "../../feature/pause.h"
#endif

namespace ExtUI {

  // The ExtUI implementation can store up to this many bytes
  // in the EEPROM when the methods onStoreSettings and
  // onLoadSettings are called.

  static constexpr size_t eeprom_data_size = 48;

  enum axis_t     : uint8_t { X, Y, Z, I, J, K, X2, Y2, Z2, Z3, Z4 };
  enum extruder_t : uint8_t { E0, E1, E2, E3, E4, E5, E6, E7 };
  enum heater_t   : uint8_t { H0, H1, H2, H3, H4, H5, BED, CHAMBER, COOLER };
  enum fan_t      : uint8_t { FAN0, FAN1, FAN2, FAN3, FAN4, FAN5, FAN6, FAN7 };
  enum result_t   : uint8_t { PID_STARTED, PID_BAD_EXTRUDER_NUM, PID_TEMP_TOO_HIGH, PID_TUNING_TIMEOUT, PID_DONE };

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
  inline void injectCommands(FSTR_P const fstr) { injectCommands_P(FTOP(fstr)); }
  void injectCommands(char * const);
  bool commandsInQueue();

  #if ENABLED(HOST_KEEPALIVE_FEATURE)
    GcodeSuite::MarlinBusyState getHostKeepaliveState();
    bool getHostKeepaliveIsPaused();
  #endif

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
    void  setAxisCurrent_mA(const_float_t, const axis_t);
    void  setAxisCurrent_mA(const_float_t, const extruder_t);

     int getTMCBumpSensitivity(const axis_t);
    void setTMCBumpSensitivity(const_float_t, const axis_t);
  #endif

  celsius_float_t getActualTemp_celsius(const heater_t);
  celsius_float_t getActualTemp_celsius(const extruder_t);
  celsius_float_t getTargetTemp_celsius(const heater_t);
  celsius_float_t getTargetTemp_celsius(const extruder_t);
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
  feedRate_t getFeedrate_mm_s();
  float getPrintingAcceleration_mm_s2();
  float getRetractAcceleration_mm_s2();
  float getTravelAcceleration_mm_s2();
  float getFeedrate_percent();
  int16_t getFlow_percent(const extruder_t);

  inline uint8_t getProgress_percent() { return ui.get_progress_percent(); }

  #if HAS_PRINT_PROGRESS_PERMYRIAD
    inline uint16_t getProgress_permyriad() { return ui.get_progress_permyriad(); }
  #endif

  uint32_t getProgress_seconds_elapsed();

  #if HAS_PREHEAT
    uint16_t getMaterial_preset_E(const uint16_t);
    #if HAS_HEATED_BED
      uint16_t getMaterial_preset_B(const uint16_t);
    #endif
  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    uint8_t getIDEX_Mode();
  #endif

  #if ENABLED(SHOW_REMAINING_TIME)
    inline uint32_t getProgress_seconds_remaining() { return ui.get_remaining_time(); }
  #endif

  #if HAS_LEVELING
    bool getLevelingActive();
    void setLevelingActive(const bool);
    bool getMeshValid();
    #if HAS_MESH
      bed_mesh_t& getMeshArray();
      float getMeshPoint(const xy_uint8_t &pos);
      void setMeshPoint(const xy_uint8_t &pos, const_float_t zval);
      void moveToMeshPoint(const xy_uint8_t &pos, const_float_t z);
      void onLevelingStart();
      void onLevelingDone();
      void onMeshUpdate(const int8_t xpos, const int8_t ypos, const_float_t zval);
      inline void onMeshUpdate(const xy_int8_t &pos, const_float_t zval) { onMeshUpdate(pos.x, pos.y, zval); }

      typedef enum : uint8_t {
        G29_START,        // Prior to start of probe
        G29_FINISH,       // Following probe of all points
        G29_POINT_START,  // Beginning probe of grid location
        G29_POINT_FINISH, // Finished probe of grid location
        G26_START,
        G26_FINISH,
        G26_POINT_START,
        G26_POINT_FINISH
      } probe_state_t;
      void onMeshUpdate(const int8_t xpos, const int8_t ypos, probe_state_t state);
      inline void onMeshUpdate(const xy_int8_t &pos, probe_state_t state) { onMeshUpdate(pos.x, pos.y, state); }
    #endif
  #endif

  #if ENABLED(HOST_PROMPT_SUPPORT)
    void setHostResponse(const uint8_t);
  #endif

  inline void simulateUserClick() {
    #if ANY(HAS_MARLINUI_MENU, EXTENSIBLE_UI, DWIN_CREALITY_LCD_JYERSUI)
      ui.lcd_clicked = true;
    #endif
  }

  #if ENABLED(PRINTCOUNTER)
    char* getFailedPrints_str(char buffer[21]);
    char* getTotalPrints_str(char buffer[21]);
    char* getFinishedPrints_str(char buffer[21]);
    char* getTotalPrintTime_str(char buffer[21]);
    char* getLongestPrint_str(char buffer[21]);
    char* getFilamentUsed_str(char buffer[21]);
  #endif

  void setTargetTemp_celsius(const_float_t, const heater_t);
  void setTargetTemp_celsius(const_float_t, const extruder_t);
  void setTargetFan_percent(const_float_t, const fan_t);
  void coolDown();
  void setAxisPosition_mm(const_float_t, const axis_t, const feedRate_t=0);
  void setAxisPosition_mm(const_float_t, const extruder_t, const feedRate_t=0);
  void setAxisSteps_per_mm(const_float_t, const axis_t);
  void setAxisSteps_per_mm(const_float_t, const extruder_t);
  void setAxisMaxFeedrate_mm_s(const feedRate_t, const axis_t);
  void setAxisMaxFeedrate_mm_s(const feedRate_t, const extruder_t);
  void setAxisMaxAcceleration_mm_s2(const_float_t, const axis_t);
  void setAxisMaxAcceleration_mm_s2(const_float_t, const extruder_t);
  void setFeedrate_mm_s(const feedRate_t);
  void setMinFeedrate_mm_s(const feedRate_t);
  void setMinTravelFeedrate_mm_s(const feedRate_t);
  void setPrintingAcceleration_mm_s2(const_float_t);
  void setRetractAcceleration_mm_s2(const_float_t);
  void setTravelAcceleration_mm_s2(const_float_t);
  void setFeedrate_percent(const_float_t);
  void setFlow_percent(const int16_t, const extruder_t);
  bool awaitingUserConfirm();
  void setUserConfirmed();

  #if M600_PURGE_MORE_RESUMABLE
    void setPauseMenuResponse(PauseMenuResponse);
    extern PauseMessage pauseModeStatus;
    PauseMode getPauseMode();
  #endif

  #if ENABLED(LIN_ADVANCE)
    float getLinearAdvance_mm_mm_s(const extruder_t);
    void setLinearAdvance_mm_mm_s(const_float_t, const extruder_t);
  #endif

  #if HAS_JUNCTION_DEVIATION
    float getJunctionDeviation_mm();
    void setJunctionDeviation_mm(const_float_t);
  #else
    float getAxisMaxJerk_mm_s(const axis_t);
    float getAxisMaxJerk_mm_s(const extruder_t);
    void setAxisMaxJerk_mm_s(const_float_t, const axis_t);
    void setAxisMaxJerk_mm_s(const_float_t, const extruder_t);
  #endif

  extruder_t getTool(const uint8_t extruder);
  extruder_t getActiveTool();
  void setActiveTool(const extruder_t, bool no_move);

  #if ENABLED(BABYSTEPPING)
    int16_t mmToWholeSteps(const_float_t mm, const axis_t axis);
    float mmFromWholeSteps(int16_t steps, const axis_t axis);

    bool babystepAxis_steps(const int16_t steps, const axis_t axis);
    void smartAdjustAxis_steps(const int16_t steps, const axis_t axis, bool linked_nozzles);
  #endif

  #if HAS_HOTEND_OFFSET
    float getNozzleOffset_mm(const axis_t, const extruder_t);
    void setNozzleOffset_mm(const_float_t, const axis_t, const extruder_t);
    void normalizeNozzleOffset(const axis_t axis);
  #endif

  float getZOffset_mm();
  void setZOffset_mm(const_float_t);

  #if HAS_BED_PROBE
    float getProbeOffset_mm(const axis_t);
    void setProbeOffset_mm(const_float_t, const axis_t);
  #endif

  #if ENABLED(BACKLASH_GCODE)
    float getAxisBacklash_mm(const axis_t);
    void setAxisBacklash_mm(const_float_t, const axis_t);

    float getBacklashCorrection_percent();
    void setBacklashCorrection_percent(const_float_t);

    #ifdef BACKLASH_SMOOTHING_MM
      float getBacklashSmoothing_mm();
      void setBacklashSmoothing_mm(const_float_t);
    #endif
  #endif

  #if HAS_FILAMENT_SENSOR
    bool getFilamentRunoutEnabled();
    void setFilamentRunoutEnabled(const bool);
    bool getFilamentRunoutState();
    void setFilamentRunoutState(const bool);

    #if HAS_FILAMENT_RUNOUT_DISTANCE
      float getFilamentRunoutDistance_mm();
      void setFilamentRunoutDistance_mm(const_float_t);
    #endif
  #endif

  #if ENABLED(CASE_LIGHT_ENABLE)
    bool getCaseLightState();
    void setCaseLightState(const bool);

    #if DISABLED(CASE_LIGHT_NO_BRIGHTNESS)
      float getCaseLightBrightness_percent();
      void setCaseLightBrightness_percent(const_float_t);
    #endif
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    bool getPowerLossRecoveryEnabled();
    void setPowerLossRecoveryEnabled(const bool);
  #endif

  #if ENABLED(PIDTEMP)
    float getPIDValues_Kp(const extruder_t);
    float getPIDValues_Ki(const extruder_t);
    float getPIDValues_Kd(const extruder_t);
    void setPIDValues(const_float_t, const_float_t , const_float_t , extruder_t);
    void startPIDTune(const celsius_t, extruder_t);
  #endif

  #if ENABLED(PIDTEMPBED)
    float getBedPIDValues_Kp();
    float getBedPIDValues_Ki();
    float getBedPIDValues_Kd();
    void setBedPIDValues(const_float_t, const_float_t , const_float_t);
    void startBedPIDTune(const celsius_t);
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

  void delay_us(uint32_t us);
  void delay_ms(uint32_t ms);
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
  bool isPrintingPaused();

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
  void onPrinterKilled(FSTR_P const error, FSTR_P const component);
  void onPrintTimerStarted();
  void onPrintTimerPaused();
  void onPrintTimerStopped();
  void onPrintDone();
  void onFilamentRunout(const extruder_t extruder);
  void onUserConfirmRequired(const char * const msg);
  void onUserConfirmRequired(FSTR_P const fstr);
  void onStatusChanged(const char * const msg);
  void onStatusChanged(FSTR_P const fstr);
  void onHomingStart();
  void onHomingDone();
  void onSteppersDisabled();
  void onSteppersEnabled();
  void onFactoryReset();
  void onStoreSettings(char *);
  void onLoadSettings(const char *);
  void onPostprocessSettings();
  void onSettingsStored(bool success);
  void onSettingsLoaded(bool success);
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
 */
#define UI_INCREMENT_BY(method, inc, ...) ExtUI::set ## method(ExtUI::get ## method (__VA_ARGS__) + inc, ##__VA_ARGS__)
#define UI_DECREMENT_BY(method, inc, ...) ExtUI::set ## method(ExtUI::get ## method (__VA_ARGS__) - inc, ##__VA_ARGS__)

#define UI_INCREMENT(method, ...) UI_INCREMENT_BY(method, increment, ##__VA_ARGS__)
#define UI_DECREMENT(method, ...) UI_DECREMENT_BY(method, increment, ##__VA_ARGS__)
