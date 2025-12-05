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

#include "../inc/MarlinConfig.h"

// Print debug messages with M111 S2 (Uses 156 bytes of PROGMEM)
//#define DEBUG_STOPWATCH

/**
 * @brief Stopwatch class
 * @details This class acts as a timer proving stopwatch functionality including
 * the ability to pause the running time counter.
 */
class Stopwatch {
  private:
    enum State : char { STOPPED, RUNNING, PAUSED };

    static Stopwatch::State state;
    static uint32_t accumulator;        // (seconds)
    static uint32_t startTimestamp;
    static uint32_t stopTimestamp;

    #if ANY(REMAINING_TIME_PRIME, REMAINING_TIME_AUTOPRIME)
      static uint32_t lap_start_time;   // Reckon from this start time
      static float    lap_start_sdpos,  // Reckon from this start file position
                      lap_total_data;   // Total size from start_sdpos to end of file
    #endif

  public:
    /**
     * @brief Initialize the stopwatch
     */
    FORCE_INLINE static void init() { reset(); }

    /**
     * @brief Stop the stopwatch
     * @details Stop the running timer, it will silently ignore the request if
     *          no timer is currently running.
     * @return true on success
     */
    static bool stop();
    static bool abort() { return stop(); } // Alias by default

    /**
     * @brief Pause the stopwatch
     * @details Pause the running timer, it will silently ignore the request if
     *          no timer is currently running.
     * @return true on success
     */
    static bool pause();

    /**
     * @brief Start the stopwatch
     * @details Start the timer, it will silently ignore the request if the
     *          timer is already running.
     * @return true on success
     */
    static bool start();

    /**
     * @brief Resume the stopwatch
     * @details Resume a timer from a given duration
     */
    static void resume(const uint32_t with_time);

    /**
     * @brief Reset the stopwatch
     * @details Reset all settings to their default values.
     */
    static void reset();

    /**
     * @brief Check if the timer is running
     * @details Return true if the timer is currently running, false otherwise.
     * @return true if stopwatch is running
     */
    FORCE_INLINE static bool isRunning() { return state == RUNNING; }

    /**
     * @brief Check if the timer is paused
     * @details Return true if the timer is currently paused, false otherwise.
     * @return true if stopwatch is paused
     */
    FORCE_INLINE static bool isPaused() { return state == PAUSED; }

    /**
     * @brief Get the running time
     * @details Return the total number of seconds the timer has been running.
     * @return the delta since starting the stopwatch
     */
    static uint32_t duration();

    #if ANY(REMAINING_TIME_PRIME, REMAINING_TIME_AUTOPRIME)

      /**
       * @brief Get the estimated remaining time based on the elapsed time
       * @details Use the given start time and sdpos values to estimate the
       *          remaining time as reckoned from duration().
       *          Should be superseded by 'M73 R' (SET_REMAINING_TIME).
       *          Get per-object time estimate with M808 by putting 'M75 R' at the start of the loop.
       *          The UI should consider a "start_sdpos" of 0 to be unset and show ---.
       *
       * @param start_time  The time to consider the "real" start of the print. Saved time of the first E move with X and/or Y.
       * @param sdpos       The current sdpos of the print job.
       * @param start_sdpos The sdpos of the first printing move (E move with X and/or Y), as a float.
       * @param total_data  The pre-calculated end_sdpos - start_sdpos as a float.
       */
      uint32_t remainingTimeEstimate(const uint32_t start_time, const uint32_t sdpos, const float start_sdpos, const float total_data) const {
        const float elapsed_data = float(sdpos) - start_sdpos;      // Ex:  460b - 280b = 180b
        if (elapsed_data < 200 || total_data == 0) return 0;        // ...not yet...
        const float //total_data = float(end_sdpos - start_sdpos),  // Ex:  999b-280b+1 = 720b
                    sd_percent = elapsed_data / total_data,         // Ex:  180b / 720b = 0.25
                    sd_ratio = (1.0f - sd_percent) / sd_percent;    // Ex: (1.0 - 0.25) / 0.25 = 3.0
        const uint32_t elapsed_time = duration() - start_time;      // Ex:   T2 - T1   = 300s
        return uint32_t(elapsed_time * sd_ratio);                   // Ex: 300s * 3.0f = 900s
      }

      FORCE_INLINE uint32_t remainingTimeEstimate(const uint32_t sdpos) const {
        return remainingTimeEstimate(lap_start_time, sdpos, lap_start_sdpos, lap_total_data);
      }

      /**
       * Start a completion time estimate given a time, start spos, and end sdpos,
       * Use the 'M75 R' command to call primeRemainingTimeEstimate at the first actual printing move.
       * TODO:
       *
       * TODO: Whenever the printer does an E + XY move call primeRemainingTimeEstimate.
       *       If the flag is still 'false' set it to 'true', record the current
       *       print time and sdpos to pass to this method.
       */
      FORCE_INLINE static void primeRemainingTimeEstimate(const uint32_t start_time, const uint32_t start_sdpos, const uint32_t end_sdpos) {
        lap_start_time = start_time;
        lap_start_sdpos = float(start_sdpos);
        lap_total_data = float(end_sdpos - start_sdpos + 1UL);
      }

      // Start estimation using the current time as with print_job_timer.primeRemainingTimeEstimate(...)
      FORCE_INLINE void primeRemainingTimeEstimate(const uint32_t start_sdpos, const uint32_t end_sdpos) {
        primeRemainingTimeEstimate(duration(), start_sdpos, end_sdpos);
      }

    #endif // REMAINING_TIME_PRIME || REMAINING_TIME_AUTOPRIME

    #ifdef DEBUG_STOPWATCH

      /**
       * @brief Print a debug message
       * @details Print a simple debug message "Stopwatch::function"
       */
      static void debug(FSTR_P const);

    #else

      static void debug(FSTR_P const) {}

    #endif
};
