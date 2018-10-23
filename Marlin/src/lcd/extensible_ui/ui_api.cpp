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

#include "../../Marlin.h"

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

static bool printer_killed = false;

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
      if (!printer_killed) return millis();

      static uint32_t currTimeHI = 0; /* Current time */

      // Machine was killed, reinit SysTick so we are able to compute time without ISRs
      if (currTimeHI == 0) {
        // Get the last time the Arduino time computed (from CMSIS) and convert it to SysTick
        currTimeHI = (uint32_t)((GetTickCount() * (uint64_t)(F_CPU/8000)) >> 24);

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
    uint32_t safe_millis() { return millis(); }

  #endif

  void delay_us(unsigned long us) {
    DELAY_US(us);
  }

  void delay_ms(unsigned long ms) {
    if (printer_killed)
      DELAY_US(ms * 1000);
    else
      safe_delay(ms);
  }

  void yield() {
    if (!printer_killed)
      thermalManager.manage_heater();
  }

  float getActualTemp_celsius(const uint8_t extruder) {
    return extruder ?
      thermalManager.degHotend(extruder - 1) :
      #if HAS_HEATED_BED
        thermalManager.degBed()
      #else
        0
      #endif
    ;
  }

  float getTargetTemp_celsius(const uint8_t extruder) {
    return extruder ?
      thermalManager.degTargetHotend(extruder - 1) :
      #if HAS_HEATED_BED
        thermalManager.degTargetBed()
      #else
        0
      #endif
    ;
  }

  float getFan_percent(const uint8_t fan) { return ((float(fan_speed[fan]) + 1) * 100) / 256; }

  float getAxisPosition_mm(const axis_t axis) {
    switch (axis) {
      case X: case Y: case Z:
        return current_position[axis];
      case E0: case E1: case E2: case E3: case E4: case E5:
        return current_position[E_AXIS];
      default: return 0;
    }
  }

  void setAxisPosition_mm(const axis_t axis, float position, float _feedrate_mm_s) {
    #if EXTRUDERS > 1
      const int8_t old_extruder = active_extruder;
    #endif
    switch (axis) {
      case X: case Y: case Z: break;
      case E0: case E1: case E2: case E3: case E4: case E5:
        active_extruder = axis - E0;
        break;
      default: return;
    }
    set_destination_from_current();
    switch (axis) {
      case X: case Y: case Z:
        destination[axis] = position;
        break;
      case E0: case E1: case E2: case E3: case E4: case E5:
        destination[E_AXIS] = position;
        break;
    }

    const float old_feedrate = feedrate_mm_s;
    feedrate_mm_s = _feedrate_mm_s;
    prepare_move_to_destination();
    feedrate_mm_s = old_feedrate;
    #if EXTRUDERS > 1
      active_extruder = old_extruder;
    #endif
  }

  void setActiveTool(uint8_t extruder, bool no_move) {
    extruder--; // Make zero based
    #if DO_SWITCH_EXTRUDER || ENABLED(SWITCHING_NOZZLE) || ENABLED(PARKING_EXTRUDER)
      if (extruder != active_extruder)
        tool_change(extruder, 0, no_move);
    #endif
    active_extruder = extruder;
  }

  uint8_t getActiveTool() { return active_extruder + 1; }

  bool isMoving() { return planner.has_blocks_queued(); }

  float getAxisSteps_per_mm(const axis_t axis) {
    switch (axis) {
      case X: case Y: case Z:
        return planner.settings.axis_steps_per_mm[axis];
      case E0: case E1: case E2: case E3: case E4: case E5:
        return planner.settings.axis_steps_per_mm[E_AXIS_N(axis - E0)];
      default: return 0;
    }
  }

  void setAxisSteps_per_mm(const axis_t axis, const float steps_per_mm) {
    switch (axis) {
      case X: case Y: case Z:
        planner.settings.axis_steps_per_mm[axis] = steps_per_mm;
        break;
      case E0: case E1: case E2: case E3: case E4: case E5:
        planner.settings.axis_steps_per_mm[E_AXIS_N(axis - E0)] = steps_per_mm;
        break;
    }
  }

  float getAxisMaxFeedrate_mm_s(const axis_t axis) {
    switch (axis) {
      case X: case Y: case Z:
        return planner.settings.max_feedrate_mm_s[axis];
      case E0: case E1: case E2: case E3: case E4: case E5:
        return planner.settings.max_feedrate_mm_s[E_AXIS_N(axis - E0)];
      default: return 0;
    }
  }

  void setAxisMaxFeedrate_mm_s(const axis_t axis, const float max_feedrate_mm_s) {
    switch (axis) {
      case X: case Y: case Z:
        planner.settings.max_feedrate_mm_s[axis] = max_feedrate_mm_s;
        break;
      case E0: case E1: case E2: case E3: case E4: case E5:
        planner.settings.max_feedrate_mm_s[E_AXIS_N(axis - E0)] = max_feedrate_mm_s;
        break;
      default: return;
    }
  }

  float getAxisMaxAcceleration_mm_s2(const axis_t axis) {
    switch (axis) {
      case X: case Y: case Z:
        return planner.settings.max_acceleration_mm_per_s2[axis];
      case E0: case E1: case E2: case E3: case E4: case E5:
        return planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(axis - E0)];
      default: return 0;
    }
  }

  void setAxisMaxAcceleration_mm_s2(const axis_t axis, const float max_acceleration_mm_per_s2) {
    switch (axis) {
      case X: case Y: case Z:
        planner.settings.max_acceleration_mm_per_s2[axis] = max_acceleration_mm_per_s2;
        break;
      case E0: case E1: case E2: case E3: case E4: case E5:
        planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(axis - E0)] = max_acceleration_mm_per_s2;
        break;
      default: return;
    }
  }

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    bool isFilamentRunoutEnabled()              { return runout.enabled; }
    void toggleFilamentRunout(const bool state) { runout.enabled = state; }

    #if FILAMENT_RUNOUT_DISTANCE_MM > 0
      float getFilamentRunoutDistance_mm() {
        return RunoutResponseDelayed::runout_distance_mm;
      }

      void setFilamentRunoutDistance_mm(const float distance) {
        RunoutResponseDelayed::runout_distance_mm = clamp(distance, 0, 999);
      }
    #endif
  #endif

  #if ENABLED(LIN_ADVANCE)
    float getLinearAdvance_mm_mm_s(const uint8_t extruder) {
      return (extruder < EXTRUDERS) ? planner.extruder_advance_K[extruder] : 0;
    }

    void setLinearAdvance_mm_mm_s(const uint8_t extruder, const float k) {
      if (extruder < EXTRUDERS)
        planner.extruder_advance_K[extruder] = clamp(k, 0, 999);
    }
  #endif

  #if ENABLED(JUNCTION_DEVIATION)
    float getJunctionDeviation_mm() {
      return planner.junction_deviation_mm;
    }

    void setJunctionDeviation_mm(const float junc_dev) {
      planner.junction_deviation_mm = clamp(junc_dev, 0.01, 0.3);
      planner.recalculate_max_e_jerk();
    }
  #else
    float getAxisMaxJerk_mm_s(const axis_t axis) {
      switch (axis) {
        case X: case Y: case Z:
          return planner.max_jerk[axis];
        case E0: case E1: case E2: case E3: case E4: case E5:
          return planner.max_jerk[E_AXIS];
        default: return 0;
      }
    }

    void setAxisMaxJerk_mm_s(const axis_t axis, const float max_jerk) {
      switch (axis) {
        case X: case Y: case Z:
          planner.max_jerk[axis] = max_jerk;
          break;
        case E0: case E1: case E2: case E3: case E4: case E5:
          planner.max_jerk[E_AXIS] = max_jerk;
          break;
        default: return;
      }
    }
  #endif

  float getMinFeedrate_mm_s()                         { return planner.settings.min_feedrate_mm_s; }
  float getMinTravelFeedrate_mm_s()                   { return planner.settings.min_travel_feedrate_mm_s; }
  float getPrintingAcceleration_mm_s2()               { return planner.settings.acceleration; }
  float getRetractAcceleration_mm_s2()                { return planner.settings.retract_acceleration; }
  float getTravelAcceleration_mm_s2()                 { return planner.settings.travel_acceleration; }
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

    void setZOffset_mm(const float zoffset_mm) {
      const float diff = (zoffset_mm - getZOffset_mm()) / planner.steps_to_mm[Z_AXIS];
      incrementZOffset_steps(diff > 0 ? ceil(diff) : floor(diff));
    }

    void incrementZOffset_steps(int16_t babystep_increment) {
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
    float getNozzleOffset_mm(const axis_t axis, uint8_t extruder) {
      if (extruder >= HOTENDS) return 0;
      return hotend_offset[axis][extruder];
    }

    void setNozzleOffset_mm(const axis_t axis, uint8_t extruder, float offset) {
      if (extruder >= HOTENDS) return;
      hotend_offset[axis][extruder] = offset;
    }
  #endif

  #if ENABLED(BACKLASH_GCODE)
    float getAxisBacklash_mm(const axis_t axis)       {return backlash_distance_mm[axis];}
    void setAxisBacklash_mm(const axis_t axis, float distance)
                                                      {backlash_distance_mm[axis] = clamp(distance,0,5);}

    float getBacklashCorrection_percent()             {return backlash_correction*100;}
    void setBacklashCorrection_percent(float percent) {backlash_correction = clamp(percent, 0, 100)/100;}

    #ifdef BACKLASH_SMOOTHING_MM
      float getBacklashSmoothing_mm()                 {return backlash_smoothing_mm;}
      void setBacklashSmoothing_mm(float distance)    {backlash_smoothing_mm = clamp(distance,0,999);}
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

  float getFeedRate_percent() {
    return feedrate_percentage;
  }

  void enqueueCommands(progmem_str gcode) {
    enqueue_and_echo_commands_P((PGM_P)gcode);
  }

  bool isAxisPositionKnown(const axis_t axis) {
    switch (axis) {
      case X: case Y: case Z:
        return TEST(axis_known_position, axis);
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
      fan_speed[fan] = clamp(round(percent * 255 / 100), 0, 255);
  }

  void setFeedrate_percent(const float percent) {
    feedrate_percentage = clamp(percent, 10, 500);
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

  FileList::FileList() {
    refresh();
  }

  void FileList::refresh() {
    num_files = 0xFFFF;
  }

  bool FileList::seek(uint16_t pos, bool skip_range_check) {
    #if ENABLED(SDSUPPORT)
      if (!skip_range_check && pos > (count() - 1)) return false;
      const uint16_t nr =
        #if ENABLED(SDCARD_RATHERRECENTFIRST) && DISABLED(SDCARD_SORT_ALPHA)
          count() - 1 -
        #endif
      pos;

      #if ENABLED(SDCARD_SORT_ALPHA)
        card.getfilename_sorted(nr);
      #else
        card.getfilename(nr);
      #endif
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
        if (ok)
          UI::onMediaRemoved();
      }
    }
  #endif // SDSUPPORT
  UI::onIdle();
}

bool lcd_hasstatus()                                                             { return true; }
bool lcd_detected()                                                              { return true; }
void lcd_reset_alert_level()                                                     {}
void lcd_refresh()                                                               {}
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
  if (!printer_killed) {
    printer_killed = true;
    UI::onPrinterKilled(msg);
  }
}

#endif // EXTENSIBLE_UI
