/*
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

#include "macros.h"

// Print debug messages with M111 S2 (Uses 156 bytes of PROGMEM)
//#define DEBUG_STOPWATCH

enum StopwatchStatus {
  STPWTCH_STOPPED,
  STPWTCH_RUNNING,
  STPWTCH_PAUSED
};

/**
 * @brief Stopwatch class
 * @details This class acts as a timer proving stopwatch functionality including
 * the ability to pause the running time counter.
 */
class Stopwatch {
  private:
    StopwatchStatus status;
    uint16_t accumulator;
    uint32_t startTimestamp;
    uint32_t stopTimestamp;

  public:
    /**
     * @brief Class constructor
     */
    Stopwatch();

    /**
     * @brief Stops the stopwatch
     * @details Stops the running timer, it will silently ignore the request if
     * no timer is currently running.
     */
    void stop();

    /**
     * @brief Pauses the stopwatch
     * @details Pauses the running timer, it will silently ignore the request if
     * no timer is currently running.
     */
    void pause();

    /**
     * @brief Starts the stopwatch
     * @details Starts the timer, it will silently ignore the request if the
     * timer is already running.
     */
    void start();

    /**
     * @brief Resets the stopwatch
     * @details Resets all settings to their default values.
     */
    void reset();

    /**
     * @brief Checks if the timer is running
     * @details Returns true if the timer is currently running, false otherwise.
     * @return bool
     */
    bool isRunning();

    /**
     * @brief Checks if the timer is paused
     * @details Returns true if the timer is currently paused, false otherwise.
     * @return bool
     */
    bool isPaused();

    /**
     * @brief Gets the running time
     * @details Returns the total number of seconds the timer has been running.
     * @return uint16_t
     */
    uint16_t duration();

    #if ENABLED(DEBUG_STOPWATCH)

      /**
       * @brief Prints a debug message
       * @details Prints a simple debug message "Stopwatch::function"
       */
      static void debug(const char func[]);

    #endif
};

#endif //STOPWATCH_H
