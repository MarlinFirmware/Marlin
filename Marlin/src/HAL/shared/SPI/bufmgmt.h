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

namespace bufmgmt {

template <typename numberType>
inline uint16_t GetNumberIndexFromTotalByteIndex(uint16_t bidx) {
  return (bidx / sizeof(numberType));
}
template <typename numberType>
inline uint16_t GetLocalByteIndexFromTotalByteIndex(uint16_t bidx) {
  return (bidx % sizeof(numberType));
}

template <typename numberType>
inline uint8_t GetByteFromNumber(numberType num, uint32_t byteNum, bool not_reverse) {
  uint32_t bitNum;
  if (not_reverse) {
    // LSBFIRST
    bitNum = (byteNum * 8);
  }
  else {
    // MSBFIRST
    bitNum = (((sizeof(numberType)-1) - byteNum) * 8);
  }
  return (num >> bitNum) & 0xFF;
}

template <typename numberType>
inline void WriteByteToNumber(numberType& num, uint32_t byteNum, uint8_t byteVal, bool not_reverse) {
  uint32_t bitNum;
  if (not_reverse) {
    // LSBFIRST
    bitNum = (byteNum * 8);
  }
  else {
    // MSBFIRST
    bitNum = (((sizeof(numberType)-1) - byteNum) * 8);
  }
  num &= ~((numberType)0xFF << bitNum);
  num |= ((numberType)byteVal << bitNum);
}

} // namespace bufmgmt