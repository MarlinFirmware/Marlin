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

#ifndef STOPWATCH_H
#define STOPWATCH_H

// Print debug messages with M111 S2 (Uses 156 bytes of PROGMEM)
//#define DEBUG_STOPWATCH

#include "../core/macros.h" // for FORCE_INLINE
#include "../core/millis_t.h"

/**
 * @brief Stopwatch class
 * @details This class acts as a timer proving stopwatch functionality including
 * the ability to pause the running time counter.
 */
class Stopwatch {
  private:
    enum State : char { STOPPED, RUNNING, PAUSED };

    static Stopwatch::State state;
    static millis_t accumulator;
    static millis_t startTimestamp;
    static millis_t stopTimestamp;

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
    static void resume(const millis_t duration);

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
    static millis_t duration();

    #ifdef DEBUG_STOPWATCH

      /**
       * @brief Print a debug message
       * @details Print a simple debug message "Stopwatch::function"
       */
      static void debug(const char func[]);

    #endif
};

#endif // STOPWATCH_H
