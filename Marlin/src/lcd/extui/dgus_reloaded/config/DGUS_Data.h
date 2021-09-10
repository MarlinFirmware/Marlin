/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <inttypes.h>

namespace DGUS_Data {

  // RX constants

  enum class Scroll : uint8_t {
    GO_BACK = 0,
    UP      = 1,
    DOWN    = 2
  };

  enum class Popup : uint8_t {
    CONFIRMED = 1
  };

  enum class Adjust : uint8_t {
    INCREMENT = 0,
    DECREMENT = 1
  };

  enum class TempPreset : uint8_t {
    PLA  = 1,
    ABS  = 2,
    PETG = 3
  };

  enum class Extruder : int8_t {
    CURRENT = -1,
    E0      = 0,
    E1      = 1
  };

  enum class Heater : int8_t {
    ALL = -2,
    BED = -1,
    H0  = 0,
    H1  = 1
  };

  enum class Control : uint8_t {
    ENABLE  = 1,
    DISABLE = 2
  };

  enum class StepSize : uint8_t {
    MM10  = 0, // 10mm
    MM1   = 1, // 1mm
    MMP1  = 2, // 0.1mm
    MMP01 = 3  // 0.01mm
  };

  enum class FilamentMove : uint8_t {
    RETRACT = 0,
    EXTRUDE = 1
  };

  enum class Axis : uint8_t {
    X_Y_Z = 0,
    X_Y   = 1,
    Z     = 2
  };

  enum class MoveDirection : uint8_t {
    XP = 0, // X+
    XM = 1, // X-
    YP = 2, // Y+
    YM = 3, // Y-
    ZP = 4, // Z+
    ZM = 5  // Z-
  };

  enum class Extra : uint8_t {
    BUTTON1 = 0,
    BUTTON2 = 1
  };

  // TX constants

  enum class SDType : uint16_t {
    NONE      = 0,
    FILE      = 1,
    DIRECTORY = 2
  };

  enum class ScrollIcon : uint16_t {
    GO_BACK = 1U << 0,
    UP      = 1U << 1,
    DOWN    = 1U << 2
  };

  enum class StatusIcon : uint16_t {
    PAUSE  = 1U << 0,
    RESUME = 1U << 1
  };

  enum class Status : uint16_t {
    DISABLED = 0,
    ENABLED  = 1
  };

  enum class StepIcon : uint16_t {
    MM10  = 1U << 0, // 10mm
    MM1   = 1U << 1, // 1mm
    MMP1  = 1U << 2, // 0.1mm
    MMP01 = 1U << 3  // 0.01mm
  };

  enum class ExtruderIcon : uint16_t {
    E0  = 1U << 0,
    E1  = 1U << 1
  };

  enum class HeaterIcon : uint16_t {
    BED = 1U << 0,
    H0  = 1U << 1,
    H1  = 1U << 2
  };

  enum class WaitIcon : uint16_t {
    ABORT    = 1U << 0,
    CONTINUE = 1U << 1
  };

};
