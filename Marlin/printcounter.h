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
#include "stopwatch.h"

// Print debug messages with M111 S2
#define DEBUG_PRINTCOUNTER

struct printStatistics {  // 12 bytes
  uint16_t successPrints; // Total number of prints
  uint16_t failedPrints;  // Total number of aborted prints - not in use
  uint32_t printTime;     // Total time printing
  uint32_t longestPrint;  // Longest print job - not in use
};

class PrintCounter: public Stopwatch {
  private:
    typedef Stopwatch super;

    printStatistics data;

    /**
     * @brief EEPROM address
     * @details Defines the start offset address where the data is stored.
     */
    const uint16_t addr = 60;

    /**
     * @brief Interval in seconds between counter updates
     * @details This const value defines what will be the time between each
     * accumulator update. This is different from the EEPROM save interval
     * which is user defined at the Configuration.h file.
     */
    const uint16_t updateInterval = 2;

    /**
     * @brief Interval in seconds between EEPROM saves
     * @details This const value defines what will be the time between each
     * EEPROM save cycle, the development team recommends to set this value
     * no lower than 3600 secs (1 hour).
     */
    const uint16_t saveInterval = PRINTCOUNTER_SAVE_INTERVAL;

  public:
    /**
     * @brief Class constructor
     */
    PrintCounter();

    void tick();
    void save();
    void load();
    void addToTimeCounter(uint16_t const &minutes);
    void addToPrintCounter(uint8_t const &prints);

    void start();
    void stop();

    #if ENABLED(DEBUG_PRINTCOUNTER)

      /**
       * @brief Prints a debug message
       * @details Prints a simple debug message "PrintCounter::function"
       */
      static void debug(const char func[]);

    #endif
};

#endif // PRINTCOUNTER_H
