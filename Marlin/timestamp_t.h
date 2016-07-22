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

#ifndef __TIMESTAMP_T__
#define __TIMESTAMP_T__

struct timestamp_t {
  /**
   * @brief Number of seconds
   */
  uint32_t timestamp;

  /**
   * @brief Date time blank constructor
   */
  timestamp_t()
    : timestamp_t(0) {};

  /**
   * @brief Date time constructor
   * @details Initializes the timestamp_t structure based on a number of seconds
   *
   * @param seconds The number of seconds
   */
  timestamp_t(uint32_t const &seconds) {
    this->timestamp = seconds;
  }

  /**
   * @brief Formats the date as number of years
   * @return The number of years
   */
  inline uint8_t year() const {
    return this->day() / 365;
  }

  /**
   * @brief Formats the date as number of days
   * @return The number of days
   */
  inline uint16_t day() const {
    return this->hour() / 24;
  }

  /**
   * @brief Formats the date as number of hours
   * @return The number of hours
   */
  inline uint32_t hour() const {
    return this->minute() / 60;
  }

  /**
   * @brief Formats the date as number of minutes
   * @return The number of minutes
   */
  inline uint32_t minute() const {
    return this->second() / 60;
  }

  /**
   * @brief Formats the date as number of seconds
   * @return The number of seconds
   */
  inline uint32_t second() const {
    return this->timestamp;
  }

  /**
   * @brief Formats the date as a string
   * @details Returns the timestamp formated as a string
   *
   * @param buffer The array pointed to must be able to accommodate 21 bytes
   *
   * String output examples:
   *  123456789012345678901 (strlen)
   *  135y 364d 23h 59m 59s
   *  364d 23h 59m 59s
   *  23h 59m 59s
   *  59m 59s
   *  59s
   *
   */
  void toString(char *buffer) const {
    int y = this->year(),
        d = this->day() % 365,
        h = this->hour() % 24,
        m = this->minute() % 60,
        s = this->second() % 60;

    if (y) sprintf_P(buffer, PSTR("%iy %id %ih %im %is"), y, d, h, m, s);
    else if (d) sprintf_P(buffer, PSTR("%id %ih %im %is"), d, h, m, s);
    else if (h) sprintf_P(buffer, PSTR("%ih %im %is"), h, m, s);
    else if (m) sprintf_P(buffer, PSTR("%im %is"), m, s);
    else sprintf_P(buffer, PSTR("%is"), s);
  }
};

#endif // __TIMESTAMP_T__
