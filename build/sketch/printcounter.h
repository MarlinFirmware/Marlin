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

#ifndef PRINTCOUNTER_H
#define PRINTCOUNTER_H

#include "macros.h"
#include "language.h"
#include "stopwatch.h"
#include <avr/eeprom.h>


// Print debug messages with M111 S2
//#define DEBUG_PRINTCOUNTER

struct printStatistics {    // 13 bytes
  //const uint8_t magic;    // Magic header, it will always be 0x16
  uint16_t totalPrints;     // Number of prints
  uint16_t finishedPrints;  // Number of complete prints
  uint32_t printTime;       // Accumulated printing time
  uint32_t longestPrint;    // Longest successfull print job
  double   filamentUsed;    // Accumulated filament consumed in mm
};

class PrintCounter: public Stopwatch {
  private:
    typedef Stopwatch super;

    printStatistics data;

    /**
     * @brief EEPROM address
     * @details Defines the start offset address where the data is stored.
     */
    const uint16_t address = 0x32;

    /**
     * @brief Interval in seconds between counter updates
     * @details This const value defines what will be the time between each
     * accumulator update. This is different from the EEPROM save interval.
     *
     * @note The max value for this option is 60(s), otherwise integer
     * overflow will happen.
     */
    const uint16_t updateInterval = 10;

    /**
     * @brief Interval in seconds between EEPROM saves
     * @details This const value defines what will be the time between each
     * EEPROM save cycle, the development team recommends to set this value
     * no lower than 3600 secs (1 hour).
     */
    const uint16_t saveInterval = 3600;

    /**
     * @brief Timestamp of the last call to deltaDuration()
     * @details Stores the timestamp of the last deltaDuration(), this is
     * required due to the updateInterval cycle.
     */
    millis_t lastDuration;

    /**
     * @brief Stats were loaded from EERPROM
     * @details If set to true it indicates if the statistical data was already
     * loaded from the EEPROM.
     */
    bool loaded = false;

  protected:
    /**
     * @brief dT since the last call
     * @details Returns the elapsed time in seconds since the last call, this is
     * used internally for print statistics accounting is not intended to be a
     * user callable function.
     */
    millis_t deltaDuration();

  public:
    /**
     * @brief Class constructor
     */
    PrintCounter();

    /**
     * @brief Checks if Print Statistics has been loaded
     * @details Returns true if the statistical data has been loaded.
     * @return bool
     */
    bool isLoaded();

    /**
     * @brief Increments the total filament used
     * @details The total filament used counter will be incremented by "amount".
     *
     * @param amount The amount of filament used in mm
     */
    void incFilamentUsed(double const &amount);

    /**
     * @brief Resets the Print Statistics
     * @details Resets the statistics to zero and saves them to EEPROM creating
     * also the magic header.
     */
    void initStats();

    /**
     * @brief Loads the Print Statistics
     * @details Loads the statistics from EEPROM
     */
    void loadStats();

    /**
     * @brief Saves the Print Statistics
     * @details Saves the statistics to EEPROM
     */
    void saveStats();

    /**
     * @brief Serial output the Print Statistics
     * @details This function may change in the future, for now it directly
     * prints the statistical data to serial.
     */
    void showStats();

    /**
     * @brief Return the currently loaded statistics
     * @details Return the raw data, in the same structure used internally
     */
    printStatistics getStats() { return this->data; }

    /**
     * @brief Loop function
     * @details This function should be called at loop, it will take care of
     * periodically save the statistical data to EEPROM and do time keeping.
     */
    void tick();

    /**
     * The following functions are being overridden
     */
    bool start();
    bool stop();
    void reset();

    #if ENABLED(DEBUG_PRINTCOUNTER)

      /**
       * @brief Prints a debug message
       * @details Prints a simple debug message "PrintCounter::function"
       */
      static void debug(const char func[]);

    #endif
};

#endif // PRINTCOUNTER_H
