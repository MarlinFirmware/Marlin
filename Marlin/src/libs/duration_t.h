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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../HAL/shared/Marduino.h"

struct duration_t {
  /**
   * @brief Duration is stored in seconds
   */
  uint32_t value;

  /**
   * @brief Constructor
   */
  duration_t()
    : duration_t(0) {};

  /**
   * @brief Constructor
   *
   * @param seconds The number of seconds
   */
  duration_t(uint32_t const &seconds) {
    this->value = seconds;
  }

  /**
   * @brief Equality comparison
   * @details Overloads the equality comparison operator
   *
   * @param value The number of seconds to compare to
   * @return True if both durations are equal
   */
  bool operator==(const uint32_t &value) const {
    return (this->value == value);
  }

  /**
   * @brief Inequality comparison
   * @details Overloads the inequality comparison operator
   *
   * @param value The number of seconds to compare to
   * @return False if both durations are equal
   */
  bool operator!=(const uint32_t &value) const {
    return ! this->operator==(value);
  }

  /**
   * @brief Formats the duration as years
   * @return The number of years
   */
  inline uint8_t year() const {
    return this->day() / 365;
  }

  /**
   * @brief Formats the duration as days
   * @return The number of days
   */
  inline uint16_t day() const {
    return this->hour() / 24;
  }

  /**
   * @brief Formats the duration as hours
   * @return The number of hours
   */
  inline uint32_t hour() const {
    return this->minute() / 60;
  }

  /**
   * @brief Formats the duration as minutes
   * @return The number of minutes
   */
  inline uint32_t minute() const {
    return this->second() / 60;
  }

  /**
   * @brief Formats the duration as seconds
   * @return The number of seconds
   */
  inline uint32_t second() const {
    return this->value;
  }

  /**
   * @brief Formats the duration as a string
   * @details String will be formated using a "full" representation of duration
   *
   * @param buffer The array pointed to must be able to accommodate 21 bytes
   *
   * Output examples:
   *  123456789012345678901 (strlen)
   *  135y 364d 23h 59m 59s
   *  364d 23h 59m 59s
   *  23h 59m 59s
   *  59m 59s
   *  59s
   */
  char* toString(char * const buffer) const {
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
    return buffer;
  }

  /**
   * @brief Formats the duration as a string
   * @details String will be formated using a "digital" representation of duration
   *
   * @param buffer The array pointed to must be able to accommodate 10 bytes
   *
   * Output examples:
   *  123456789 (strlen)
   *  99:59
   *  11d 12:33
   */
  uint8_t toDigital(char *buffer, bool with_days=false) const {
    uint16_t h = uint16_t(this->hour()),
             m = uint16_t(this->minute() % 60UL);
    if (with_days) {
      uint16_t d = this->day();
      sprintf_P(buffer, PSTR("%hud %02hu:%02hu"), d, h % 24, m);
      return d >= 10 ? 9 : 8;
    }
    else if (h < 100) {
      sprintf_P(buffer, PSTR("%02hu:%02hu"), h, m);
      return 5;
    }
    else {
      sprintf_P(buffer, PSTR("%hu:%02hu"), h, m);
      return 6;
    }
  }
};
