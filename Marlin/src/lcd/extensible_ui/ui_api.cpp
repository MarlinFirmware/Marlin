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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(EXTENSIBLE_UI)

#include "../../gcode/queue.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../module/temperature.h"
#include "../../libs/duration_t.h"
#include "../../HAL/shared/Delay.h"

#if DO_SWITCH_EXTRUDER || ENABLED(SWITCHING_NOZZLE) || ENABLED(PARKING_EXTRUDER)
  #include "../../module/tool_change.h"
#endif

#if ENABLED(SDSUPPORT)
  #include "../../sd/cardreader.h"
  #include "../../feature/emergency_parser.h"
  #define IFSD(A,B) (A)
#else
  #define IFSD(A,B) (B)
#endif

#if ENABLED(PRINTCOUNTER)
  #include "../../core/utility.h"
  #include "../../module/printcounter.h"
#endif

#include "ui_api.h"

#if ENABLED(BACKLASH_GCODE)
  extern float backlash_distance_mm[XYZ], backlash_correction;
  #ifdef BACKLASH_SMOOTHING_MM
    extern float backlash_smoothing_mm;
  #endif
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #include "../../feature/runout.h"
#endif

inline float clamp(const float value, const float minimum, const float maximum) {
  return MAX(MIN(value, maximum), minimum);
}

static struct {
  uint8_t printer_killed  : 1;
  uint8_t manual_motion : 1;
} flags;

namespace UI {
  #ifdef __SAM3X8E__
    /**
     * Implement a special millis() to allow time measurement
     * within an ISR (such as when the printer is killed).
     *
     * To keep proper time, must be called at least every 1s.
     */
    uint32_t safe_millis() {
      // Not killed? Just call millis()
      if (!flags.printer_killed) return millis();

      static uint32_t currTimeHI = 0; /* Current time */

      // Machine was killed, reinit SysTick so we are able to compute time without ISRs
      if (currTimeHI == 0) {
        // Get the last time the Arduino time computed (from CMSIS) and convert it to SysTick
        currTimeHI = (uint32_t)((GetTickCount() * (uint64_t)(F_CPU / 8000)) >> 24);

        // Reinit the SysTick timer to maximize its period
        SysTick->LOAD  = SysTick_LOAD_RELOAD_Msk;                    // get the full range for the systick timer
        SysTick->VAL   = 0;                                          // Load the SysTick Counter Value
        SysTick->CTRL  = // MCLK/8 as source
                         // No interrupts
                         SysTick_CTRL_ENABLE_Msk;                    // Enable SysTick Timer
     }

      // Check if there was a timer overflow from the last read
      if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
        // There was. This means (SysTick_LOAD_RELOAD_Msk * 1000 * 8)/F_CPU ms has elapsed
        currTimeHI++;
      }

      // Calculate current time in milliseconds
      uint32_t currTimeLO = SysTick_LOAD_RELOAD_Msk - SysTick->VAL; // (in MCLK/8)
      uint64_t currTime = ((uint64_t)currTimeLO) | (((uint64_t)currTimeHI) << 24);

      // The ms count is
      return (uint32_t)(currTime / (F_CPU / 8000));
    }

  #else

    // TODO: Implement for AVR
    FORCE_INLINE uint32_t safe_millis() { return millis(); }

  #endif

  void delay_us(unsigned long us) {
    DELAY_US(us);
  }

  void delay_ms(unsigned long ms) {
    if (flags.printer_killed)
      DELAY_US(ms * 1000);
    else
      safe_delay(ms);
  }

  void yield() {
    if (!flags.printer_killed)
      thermalManager.manage_heater();
  }

  float getActualTemp_celsius(const heater_t heater) {
    return heater == BED ?
      #if HAS_HEATED_BED
        thermalManager.degBed()
      #else
        0
      #endif
      : thermalManager.degHotend(heater - H0);
  }

  float getActualTemp_celsius(const extruder_t extruder) {
    return thermalManager.degHotend(extruder - E0);
  }

  float getTargetTemp_celsius(const heater_t heater) {
    return heater == BED ?
      #if HAS_HEATED_BED
        thermalManager.degTargetBed()
      #else
        0
      #endif
      : thermalManager.degTargetHotend(heater - H0);
  }

  float getTargetTemp_celsius(const extruder_t extruder) {
    return thermalManager.degTargetHotend(extruder - E0);
  }

  float getFan_percent(const fan_t fan) { return ((float(fan_speed[fan - FAN0]) + 1) * 100) / 256; }

  float getAxisPosition_mm(const axis_t axis) {
    return flags.manual_motion ? destination[axis] : current_position[axis];
  }

  float getAxisPosition_mm(const extruder_t extruder) {
    return flags.manual_motion ? destination[E_AXIS] : current_position[E_AXIS];
  }

  void setAxisPosition_mm(const float position, const axis_t axis) {
    // Start with no limits to movement
    float min = current_position[axis] - 1000,
          max = current_position[axis] + 1000;

    // Limit to software endstops, if enabled
    #if ENABLED(MIN_SOFTWARE_ENDSTOPS) || ENABLED(MAX_SOFTWARE_ENDSTOPS)
      if (soft_endstops_enabled) switch (axis) {
        case X_AXIS:
          #if ENABLED(MIN_SOFTWARE_ENDSTOP_X)
            min = soft_endstop_min[X_AXIS];
          #endif
          #if ENABLED(MAX_SOFTWARE_ENDSTOP_X)
            max = soft_endstop_max[X_AXIS];
          #endif
          break;
        case Y_AXIS:
          #if ENABLED(MIN_SOFTWARE_ENDSTOP_Y)
            min = soft_endstop_min[Y_AXIS];
          #endif
          #if ENABLED(MAX_SOFTWARE_ENDSTOP_Y)
            max = soft_endstop_max[Y_AXIS];
          #endif
          break;
        case Z_AXIS:
          #if ENABLED(MIN_SOFTWARE_ENDSTOP_Z)
            min = soft_endstop_min[Z_AXIS];
          #endif
          #if ENABLED(MAX_SOFTWARE_ENDSTOP_Z)
            max = soft_endstop_max[Z_AXIS];
          #endif
        default: break;
      }
    #endif // MIN_SOFTWARE_ENDSTOPS || MAX_SOFTWARE_ENDSTOPS

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z_AXIS) {
        max = SQRT(sq((float)(DELTA_PRINTABLE_RADIUS)) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
        min = -max;
      }
    #endif

    if (!flags.manual_motion)
      set_destination_from_current();
    destination[axis] = clamp(position, min, max);
    flags.manual_motion = true;
  }

  void setAxisPosition_mm(const float position, const extruder_t extruder) {
    setActiveTool(extruder, true);

    if (!flags.manual_motion)
      set_destination_from_current();
    destination[E_AXIS] = position;
    flags.manual_motion = true;
  }

  void _processManualMoveToDestination() {
    // Lower max_response_lag makes controls more responsive, but makes CPU work harder
    constexpr float   max_response_lag = 0.1; // seconds
    constexpr uint8_t segments_to_buffer = 4; // keep planner filled with this many segments

    if (flags.manual_motion && planner.movesplanned() < segments_to_buffer) {
      float saved_destination[XYZ];
      COPY(saved_destination, destination);
      // Compute direction vector from current_position towards destination.
      destination[X_AXIS] -= current_position[X_AXIS];
      destination[Y_AXIS] -= current_position[Y_AXIS];
      destination[Z_AXIS] -= current_position[Z_AXIS];
      const float inv_length = RSQRT(sq(destination[X_AXIS]) + sq(destination[Y_AXIS]) + sq(destination[Z_AXIS]));
      // Find move segment length so that all segments can execute in less time than max_response_lag
      const float scale = inv_length * feedrate_mm_s * max_response_lag / segments_to_buffer;
      if (scale < 1) {
        // Move a small bit towards the destination.
        destination[X_AXIS] = scale * destination[X_AXIS] + current_position[X_AXIS];
        destination[Y_AXIS] = scale * destination[Y_AXIS] + current_position[Y_AXIS];
        destination[Z_AXIS] = scale * destination[Z_AXIS] + current_position[Z_AXIS];
        prepare_move_to_destination();
        COPY(destination, saved_destination);
      }
      else {
        // We are close enough to finish off the move.
        COPY(destination, saved_destination);
        prepare_move_to_destination();
        flags.manual_motion = false;
      }
    }
  }

  void setActiveTool(const extruder_t extruder, bool no_move) {
    const uint8_t e = extruder - E0;
    #if DO_SWITCH_EXTRUDER || ENABLED(SWITCHING_NOZZLE) || ENABLED(PARKING_EXTRUDER)
      if (e != active_extruder)
        tool_change(e, 0, no_move);
    #endif
    active_extruder = e;
  }

  extruder_t getActiveTool() {
    switch (active_extruder) {
      case 5:  return E5;
      case 4:  return E4;
      case 3:  return E3;
      case 2:  return E2;
      case 1:  return E1;
      default: return E0;
    }
  }

  bool isMoving() { return planner.has_blocks_queued(); }

  bool canMove(const axis_t axis) {
    switch (axis) {
      #if IS_KINEMATIC || ENABLED(NO_MOTION_BEFORE_HOMING)
        case X: return TEST(axis_homed, X_AXIS);
        case Y: return TEST(axis_homed, Y_AXIS);
        case Z: return TEST(axis_homed, Z_AXIS);
      #else
        case X: case Y: case Z: return true;
      #endif
      default: return false;
    }
  }

  bool canMove(const extruder_t extruder) {
    return !thermalManager.tooColdToExtrude(extruder - E0);
  }

  float getAxisSteps_per_mm(const axis_t axis) {
    return planner.settings.axis_steps_per_mm[axis];
  }

  float getAxisSteps_per_mm(const extruder_t extruder) {
    return planner.settings.axis_steps_per_mm[E_AXIS_N(extruder - E0)];
  }

  void setAxisSteps_per_mm(const float value, const axis_t axis) {
    planner.settings.axis_steps_per_mm[axis] = value;
  }

  void setAxisSteps_per_mm(const float value, const extruder_t extruder) {
    planner.settings.axis_steps_per_mm[E_AXIS_N(axis - E0)] = value;
  }

  float getAxisMaxFeedrate_mm_s(const axis_t axis) {
    return planner.settings.max_feedrate_mm_s[axis];
  }

  float getAxisMaxFeedrate_mm_s(const extruder_t extruder) {
    return planner.settings.max_feedrate_mm_s[E_AXIS_N(axis - E0)];
  }

  void setAxisMaxFeedrate_mm_s(const float value, const axis_t axis) {
    planner.settings.max_feedrate_mm_s[axis] = value;
  }

  void setAxisMaxFeedrate_mm_s(const float value, const extruder_t extruder) {
    planner.settings.max_feedrate_mm_s[E_AXIS_N(axis - E0)] = value;
  }

  float getAxisMaxAcceleration_mm_s2(const axis_t axis) {
    return planner.settings.max_acceleration_mm_per_s2[axis];
  }

  float getAxisMaxAcceleration_mm_s2(const extruder_t extruder) {
    return planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(extruder - E0)];
  }

  void setAxisMaxAcceleration_mm_s2(const float value, const axis_t axis) {
    planner.settings.max_acceleration_mm_per_s2[axis] = value;
  }

  void setAxisMaxAcceleration_mm_s2(const float value, const extruder_t extruder) {
    planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(extruder - E0)] = value;
  }

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    bool getFilamentRunoutEnabled()                 { return runout.enabled; }
    void setFilamentRunoutEnabled(const bool value) { runout.enabled = value; }

    #if FILAMENT_RUNOUT_DISTANCE_MM > 0
      float getFilamentRunoutDistance_mm() {
        return RunoutResponseDelayed::runout_distance_mm;
      }

      void setFilamentRunoutDistance_mm(const float value) {
        RunoutResponseDelayed::runout_distance_mm = clamp(value, 0, 999);
      }
    #endif
  #endif

  #if ENABLED(LIN_ADVANCE)
    float getLinearAdvance_mm_mm_s(const extruder_t extruder) {
      return (extruder < EXTRUDERS) ? planner.extruder_advance_K[extruder - E0] : 0;
    }

    void setLinearAdvance_mm_mm_s(const float value, const extruder_t extruder) {
      if (extruder < EXTRUDERS)
        planner.extruder_advance_K[extruder - E0] = clamp(value, 0, 999);
    }
  #endif

  #if ENABLED(JUNCTION_DEVIATION)

    float getJunctionDeviation_mm() {
      return planner.junction_deviation_mm;
    }

    void setJunctionDeviation_mm(const float value) {
      planner.junction_deviation_mm = clamp(value, 0.01, 0.3);
      planner.recalculate_max_e_jerk();
    }

  #else

    float getAxisMaxJerk_mm_s(const axis_t axis) {
      return planner.max_jerk[axis];
    }

    float getAxisMaxJerk_mm_s(const extruder_t extruder) {
      return planner.max_jerk[E_AXIS];
    }

    void setAxisMaxJerk_mm_s(const float value, const axis_t axis) {
      planner.max_jerk[axis] = value;
    }

    void setAxisMaxJerk_mm_s(const float value, const extruder_t extruder) {
      planner.max_jerk[E_AXIS] = value;
    }
  #endif

  float getFeedrate_mm_s()                            { return feedrate_mm_s; }
  float getMinFeedrate_mm_s()                         { return planner.settings.min_feedrate_mm_s; }
  float getMinTravelFeedrate_mm_s()                   { return planner.settings.min_travel_feedrate_mm_s; }
  float getPrintingAcceleration_mm_s2()               { return planner.settings.acceleration; }
  float getRetractAcceleration_mm_s2()                { return planner.settings.retract_acceleration; }
  float getTravelAcceleration_mm_s2()                 { return planner.settings.travel_acceleration; }
  void setFeedrate_mm_s(const float fr)               { feedrate_mm_s = fr; }
  void setMinFeedrate_mm_s(const float fr)            { planner.settings.min_feedrate_mm_s = fr; }
  void setMinTravelFeedrate_mm_s(const float fr)      { planner.settings.min_travel_feedrate_mm_s = fr; }
  void setPrintingAcceleration_mm_s2(const float acc) { planner.settings.acceleration = acc; }
  void setRetractAcceleration_mm_s2(const float acc)  { planner.settings.retract_acceleration = acc; }
  void setTravelAcceleration_mm_s2(const float acc)   { planner.settings.travel_acceleration = acc; }

  #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
    float getZOffset_mm() {
      #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
        if (active_extruder != 0)
          return hotend_offset[Z_AXIS][active_extruder];
        else
      #endif
          return zprobe_zoffset;
    }

    void setZOffset_mm(const float value) {
      const float diff = (value - getZOffset_mm()) / planner.steps_to_mm[Z_AXIS];
      addZOffset_steps(diff > 0 ? ceil(diff) : floor(diff));
    }

    void addZOffset_steps(int16_t babystep_increment) {
      #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
        const bool do_probe = (active_extruder == 0);
      #else
        constexpr bool do_probe = true;
      #endif
      const float diff = planner.steps_to_mm[Z_AXIS] * babystep_increment,
                  new_probe_offset = zprobe_zoffset + diff,
                  new_offs =
                    #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
                      do_probe ? new_probe_offset : hotend_offset[Z_AXIS][active_extruder] - diff
                    #else
                      new_probe_offset
                    #endif
                  ;
      if (WITHIN(new_offs, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {

        thermalManager.babystep_axis(Z_AXIS, babystep_increment);

        if (do_probe) zprobe_zoffset = new_offs;
        #if ENABLED(BABYSTEP_HOTEND_Z_OFFSET)
          else hotend_offset[Z_AXIS][active_extruder] = new_offs;
        #endif
      }
    }
  #endif // ENABLED(BABYSTEP_ZPROBE_OFFSET)

  #if HOTENDS > 1
    float getNozzleOffset_mm(const axis_t axis, const extruder_t extruder) {
      if (extruder - E0 >= HOTENDS) return 0;
      return hotend_offset[axis][extruder - E0];
    }

    void setNozzleOffset_mm(const float value, const axis_t axis, const extruder_t extruder) {
      if (extruder - E0 >= HOTENDS) return;
      hotend_offset[axis][extruder - E0] = value;
    }
  #endif

  #if ENABLED(BACKLASH_GCODE)
    float getAxisBacklash_mm(const axis_t axis)       { return backlash_distance_mm[axis]; }
    void setAxisBacklash_mm(const float value, const axis_t axis)
                                                      { backlash_distance_mm[axis] = clamp(value,0,5); }

    float getBacklashCorrection_percent()             { return backlash_correction * 100; }
    void setBacklashCorrection_percent(const float value) { backlash_correction = clamp(value, 0, 100) / 100.0f; }

    #ifdef BACKLASH_SMOOTHING_MM
      float getBacklashSmoothing_mm()                 { return backlash_smoothing_mm; }
      void setBacklashSmoothing_mm(const float value) { backlash_smoothing_mm = clamp(value, 0, 999); }
    #endif
  #endif

  uint8_t getProgress_percent() {
    return IFSD(card.percentDone(), 0);
  }

  uint32_t getProgress_seconds_elapsed() {
    const duration_t elapsed = print_job_timer.duration();
    return elapsed.value;
  }

  #if ENABLED(PRINTCOUNTER)
    char* getTotalPrints_str(char buffer[21])    { strcpy(buffer,itostr3left(print_job_timer.getStats().totalPrints));    return buffer; }
    char* getFinishedPrints_str(char buffer[21]) { strcpy(buffer,itostr3left(print_job_timer.getStats().finishedPrints)); return buffer; }
    char* getTotalPrintTime_str(char buffer[21]) { duration_t(print_job_timer.getStats().printTime).toString(buffer);     return buffer; }
    char* getLongestPrint_str(char buffer[21])   { duration_t(print_job_timer.getStats().printTime).toString(buffer);     return buffer; }
    char* getFilamentUsed_str(char buffer[21])   {
      printStatistics stats = print_job_timer.getStats();
      sprintf_P(buffer, PSTR("%ld.%im"), long(stats.filamentUsed / 1000), int16_t(stats.filamentUsed / 100) % 10);
      return buffer;
    }
  #endif

  float getFeedrate_percent() { return feedrate_percentage; }

  void enqueueCommands(progmem_str gcode) {
    enqueue_and_echo_commands_P((PGM_P)gcode);
  }

  bool isAxisPositionKnown(const axis_t axis) {
    return TEST(axis_known_position, axis);
  }

  progmem_str getFirmwareName_str() {
    return F("Marlin " SHORT_BUILD_VERSION);
  }

  void setTargetTemp_celsius(float value, const heater_t heater) {
    #if HAS_HEATED_BED
    if (heater == BED)
      thermalManager.setTargetBed(clamp(value,0,200));
    #endif
      thermalManager.setTargetHotend(clamp(value,0,500), heater - H0);
  }

  void setTargetTemp_celsius(float value, const extruder_t extruder) {
    thermalManager.setTargetHotend(clamp(value,0,500), extruder - E0);
  }

  void setFan_percent(float value, const fan_t fan) {
    if (fan < FAN_COUNT)
      fan_speed[fan - FAN0] = clamp(round(value * 255 / 100), 0, 255);
  }

  void setFeedrate_percent(const float value) {
    feedrate_percentage = clamp(value, 10, 500);
  }

  void printFile(const char *filename) {
    IFSD(card.openAndPrintFile(filename), NOOP);
  }

  bool isPrintingFromMediaPaused() {
    return IFSD(isPrintingFromMedia() && !card.sdprinting, false);
  }

  bool isPrintingFromMedia() {
    return IFSD(card.cardOK && card.isFileOpen(), false);
  }

  bool isPrinting() {
    return (planner.movesplanned() || IS_SD_PRINTING() || isPrintingFromMedia());
  }

  bool isMediaInserted() {
    return IFSD(IS_SD_INSERTED() && card.cardOK, false);
  }

  void pausePrint() {
    #if ENABLED(SDSUPPORT)
      card.pauseSDPrint();
      print_job_timer.pause();
      #if ENABLED(PARK_HEAD_ON_PAUSE)
        enqueue_and_echo_commands_P(PSTR("M125"));
      #endif
      UI::onStatusChanged(PSTR(MSG_PRINT_PAUSED));
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
      UI::onStatusChanged(PSTR(MSG_PRINTING));
    #endif
  }

  void stopPrint() {
    #if ENABLED(SDSUPPORT)
      wait_for_heatup = wait_for_user = false;
      card.abort_sd_printing = true;
      UI::onStatusChanged(PSTR(MSG_PRINT_ABORTED));
    #endif
  }

  FileList::FileList() { refresh(); }

  void FileList::refresh() { num_files = 0xFFFF; }

  bool FileList::seek(uint16_t pos, bool skip_range_check) {
    #if ENABLED(SDSUPPORT)
      if (!skip_range_check && pos > (count() - 1)) return false;
      const uint16_t nr =
        #if ENABLED(SDCARD_RATHERRECENTFIRST) && DISABLED(SDCARD_SORT_ALPHA)
          count() - 1 -
        #endif
      pos;

      card.getfilename_sorted(nr);
      return card.filename && card.filename[0] != '\0';
    #endif
  }

  const char* FileList::filename() {
    return IFSD(card.longFilename && card.longFilename[0] ? card.longFilename : card.filename, "");
  }

  const char* FileList::shortFilename() {
    return IFSD(card.filename, "");
  }

  const char* FileList::longFilename() {
    return IFSD(card.longFilename, "");
  }

  bool FileList::isDir() {
    return IFSD(card.filenameIsDir, false);
  }

  uint16_t FileList::count() {
    return IFSD((num_files = (num_files == 0xFFFF ? card.get_num_Files() : num_files)), 0);
  }

  bool FileList::isAtRootDir() {
    #if ENABLED(SDSUPPORT)
      card.getWorkDirName();
      return card.filename[0] == '/';
    #else
      return true;
    #endif
  }

  void FileList::upDir() {
    #if ENABLED(SDSUPPORT)
      card.updir();
      num_files = 0xFFFF;
    #endif
  }

  void FileList::changeDir(const char *dirname) {
    #if ENABLED(SDSUPPORT)
      card.chdir(dirname);
      num_files = 0xFFFF;
    #endif
  }

} // namespace UI

// At the moment, we piggy-back off the ultralcd calls, but this could be cleaned up in the future

void lcd_init() {
  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    SET_INPUT_PULLUP(SD_DETECT_PIN);
  #endif
  UI::onStartup();
}

void lcd_update() {
  #if ENABLED(SDSUPPORT)
    static bool last_sd_status;
    const bool sd_status = IS_SD_INSERTED();
    if (sd_status != last_sd_status) {
      last_sd_status = sd_status;
      if (sd_status) {
        card.initsd();
        if (card.cardOK)
          UI::onMediaInserted();
        else
          UI::onMediaError();
      }
      else {
        const bool ok = card.cardOK;
        card.release();
        if (ok) UI::onMediaRemoved();
      }
    }
  #endif // SDSUPPORT
  UI::_processManualMoveToDestination();
  UI::onIdle();
}

bool lcd_hasstatus() { return true; }
bool lcd_detected() { return true; }
void lcd_reset_alert_level() { }
void lcd_refresh() { }
void lcd_setstatus(const char * const message, const bool persist /* = false */) { UI::onStatusChanged(message); }
void lcd_setstatusPGM(const char * const message, int8_t level /* = 0 */)        { UI::onStatusChanged((progmem_str)message); }
void lcd_setalertstatusPGM(const char * const message)                           { lcd_setstatusPGM(message, 0); }

void lcd_reset_status() {
  static const char paused[] PROGMEM = MSG_PRINT_PAUSED;
  static const char printing[] PROGMEM = MSG_PRINTING;
  static const char welcome[] PROGMEM = WELCOME_MSG;
  PGM_P msg;
  if (print_job_timer.isPaused())
    msg = paused;
  #if ENABLED(SDSUPPORT)
    else if (card.sdprinting)
      return lcd_setstatus(card.longest_filename(), true);
  #endif
  else if (print_job_timer.isRunning())
    msg = printing;
  else
    msg = welcome;

  lcd_setstatusPGM(msg, -1);
}

void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) {
  char buff[64];
  va_list args;
  va_start(args, fmt);
  vsnprintf_P(buff, sizeof(buff), fmt, args);
  va_end(args);
  buff[63] = '\0';
  UI::onStatusChanged(buff);
}

void kill_screen(PGM_P msg) {
  if (!flags.printer_killed) {
    flags.printer_killed = true;
    UI::onPrinterKilled(msg);
  }
}

#endif // EXTENSIBLE_UI
