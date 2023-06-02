/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../core/types.h"

#ifdef __cplusplus
  namespace Endianness
  {
      static constexpr uint32_t _dword = 0x01020304;
      static constexpr uint8_t _lsb = (const uint8_t&)_dword;

      static constexpr bool cpuIsLittleEndian = _lsb == 0x04;
      static constexpr bool cpuIsBigEndian = _lsb == 0x01;
      static_assert(cpuIsLittleEndian ^ cpuIsBigEndian, "Unknown CPU endianness");

      template<typename T> static constexpr T swap(T V)
      {
        union {
          T val;
          char byte[sizeof(T)];
        } src{}, dst{};

        src.val = V;
        LOOP_L_N(i, sizeof(T)) dst.byte[i] = src.byte[sizeof(T) - i - 1];
        return dst.val;
      }

      template<typename T> static constexpr T toBE(T V) { return cpuIsLittleEndian ? swap(V) : V; }
      template<typename T> static constexpr T toLE(T V) { return cpuIsLittleEndian ? V : swap(V); }
      template<typename T> static constexpr T fromBE(T V) { return cpuIsLittleEndian ? swap(V) : V; }
      template<typename T> static constexpr T fromLE(T V) { return cpuIsLittleEndian ? V : swap(V); }

      template<typename T> static constexpr T fromBE_P(void* V) { return fromBE(*(T*)V); }
      template<typename T> static constexpr T fromLE_P(void* V) { return fromLE(*(T*)V); }
  };
#endif