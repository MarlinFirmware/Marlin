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

#ifndef PRINTCOUNTER_H
#define PRINTCOUNTER_H

// Print debug messages with M111 S2
//#define DEBUG_PRINTCOUNTER

#include "macros.h"
#include "language.h"
#include "stopwatch.h"
#include <avr/eeprom.h>

struct printStatistics {    // 16 bytes (20 with real doubles)
  //const uint8_t magic;    // Magic header, it will always be 0x16
  uint16_t totalPrints;     // Number of prints
  uint16_t finishedPrints;  // Number of complete prints
  uint32_t printTime;       // Accumulated printing time
  uint32_t longestPrint;    // Longest successful print job
  double   filamentUsed;    // Accumulated filament consumed in mm
};

class PrintCounter: public Stopwatch {
  private:
    typedef Stopwatch super;

    #if ENABLED(I2C_EEPROM) || ENABLED(SPI_EEPROM) || defined(CPU_32_BIT)
      typedef uint32_t promdress;
    #else
      typedef uint16_t promdress;
    #endif

    static printStatistics data;

    /**
     * @brief EEPROM address
     * @details Defines the start offset address where the data is stored.
     */
    static const promdress address;

    /**
     * @brief Interval in seconds between counter updates
     * @details This const value defines what will be the time between each
     * accumulator update. This is different from the EEPROM save interval.
     *
     * @note The max value for this option is 60(s), otherwise integer
     * overflow will happen.
     */
    static const uint16_t updateInterval;

    /**
     * @brief Interval in seconds between EEPROM saves
     * @details This const value defines what will be the time between each
     * EEPROM save cycle, the development team recommends to set this value
     * no lower than 3600 secs (1 hour).
     */
    static const uint16_t saveInterval;

    /**
     * @brief Timestamp of the last call to deltaDuration()
     * @details Store the timestamp of the last deltaDuration(), this is
     * required due to the updateInterval cycle.
     */
    static millis_t lastDuration;

    /**
     * @brief Stats were loaded from EEPROM
     * @details If set to true it indicates if the statistical data was already
     * loaded from the EEPROM.
     */
    static bool loaded;

  protected:
    /**
     * @brief dT since the last call
     * @details Return the elapsed time in seconds since the last call, this is
     * used internally for print statistics accounting is not intended to be a
     * user callable function.
     */
    static millis_t deltaDuration();

  public:

    /**
     * @brief Initialize the print counter
     */
    static inline void init() {
      super::init();
      loadStats();
    }

    /**
     * @brief Check if Print Statistics has been loaded
     * @details Return true if the statistical data has been loaded.
     * @return bool
     */
    FORCE_INLINE static bool isLoaded() { return loaded; }

    /**
     * @brief Increment the total filament used
     * @details The total filament used counter will be incremented by "amount".
     *
     * @param amount The amount of filament used in mm
     */
    static void incFilamentUsed(double const &amount);

    /**
     * @brief Reset the Print Statistics
     * @details Reset the statistics to zero and saves them to EEPROM creating
     * also the magic header.
     */
    static void initStats();

    /**
     * @brief Load the Print Statistics
     * @details Load the statistics from EEPROM
     */
    static void loadStats();

    /**
     * @brief Save the Print Statistics
     * @details Save the statistics to EEPROM
     */
    static void saveStats();

    /**
     * @brief Serial output the Print Statistics
     * @details This function may change in the future, for now it directly
     * prints the statistical data to serial.
     */
    static void showStats();

    /**
     * @brief Return the currently loaded statistics
     * @details Return the raw data, in the same structure used internally
     */
    static printStatistics getStats() { return data; }

    /**
     * @brief Loop function
     * @details This function should be called at loop, it will take care of
     * periodically save the statistical data to EEPROM and do time keeping.
     */
    static void tick();

    /**
     * The following functions are being overridden
     */
    static bool start();
    static bool stop();
    static void reset();

    #if ENABLED(DEBUG_PRINTCOUNTER)

      /**
       * @brief Print a debug message
       * @details Print a simple debug message
       */
      static void debug(const char func[]);

    #endif
};

// Global Print Job Timer instance
#if ENABLED(PRINTCOUNTER)
  extern PrintCounter print_job_timer;
#else
  extern Stopwatch print_job_timer;
#endif

#endif // PRINTCOUNTER_H
