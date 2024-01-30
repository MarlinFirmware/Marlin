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

/**
 * Optimized math functions for AVR
 */

// intRes = longIn1 * longIn2 >> 24
// uses:
// r1, r0 for the result of mul.
// [tmp1] to store 0.
// [tmp2] to store bits 16-23 of the 56 bit result. The top bit of [tmp2] is used for rounding.
// Note that the lower two bytes and the upper two bytes of the 56 bit result are not calculated.
// This can cause the result to be out by one as the lower bytes may cause carries into the upper ones.
// [intRes] (A B) is bits 24-39 and is the returned value.
// [longIn1] (C B A) is a 24 bit parameter.
// [longIn2] (D C B A) is a 32 bit parameter.
//
FORCE_INLINE static uint16_t MultiU24X32toH16(uint32_t longIn1, uint32_t longIn2) {
  uint8_t tmp1;
  uint8_t tmp2;
  uint16_t intRes;
  __asm__ __volatile__(
    A("clr %[tmp1]")
    A("mul %A[longIn1], %B[longIn2]")
    A("mov %[tmp2], r1")
    A("mul %B[longIn1], %C[longIn2]")
    A("movw %A[intRes], r0")
    A("mul %C[longIn1], %C[longIn2]")
    A("add %B[intRes], r0")
    A("mul %C[longIn1], %B[longIn2]")
    A("add %A[intRes], r0")
    A("adc %B[intRes], r1")
    A("mul %A[longIn1], %C[longIn2]")
    A("add %[tmp2], r0")
    A("adc %A[intRes], r1")
    A("adc %B[intRes], %[tmp1]")
    A("mul %B[longIn1], %B[longIn2]")
    A("add %[tmp2], r0")
    A("adc %A[intRes], r1")
    A("adc %B[intRes], %[tmp1]")
    A("mul %C[longIn1], %A[longIn2]")
    A("add %[tmp2], r0")
    A("adc %A[intRes], r1")
    A("adc %B[intRes], %[tmp1]")
    A("mul %B[longIn1], %A[longIn2]")
    A("add %[tmp2], r1")
    A("adc %A[intRes], %[tmp1]")
    A("adc %B[intRes], %[tmp1]")
    A("mul %D[longIn2], %A[longIn1]")
    A("lsl %[tmp2]")
    A("adc %A[intRes], r0")
    A("adc %B[intRes], r1")
    A("mul %D[longIn2], %B[longIn1]")
    A("add %B[intRes], r0")
    A("clr r1")
      : [intRes] "=&r" (intRes),
        [tmp1] "=&r" (tmp1),
        [tmp2] "=&r" (tmp2)
      : [longIn1] "d" (longIn1),
        [longIn2] "d" (longIn2)
      : "cc"
  );
  return intRes;
}

// charRes = charIn1 * charIn2 >> 8
// uses:
// r1, r0 for the result of mul. After the mul, r0 holds bits 0-7 of the 16 bit result,
//        and the top bit of r0 is used for rounding.
// [charRes] is bits 8-15 and is the returned value.
// [charIn1] is an 8 bit parameter.
// [charIn2] is an 8 bit parameter.
//
FORCE_INLINE static uint8_t MultiU8X8toH8(uint8_t charIn1, uint8_t charIn2) {
  uint8_t charRes;
  __asm__ __volatile__ (
    A("mul %[charIn1], %[charIn2]")
    A("mov %[charRes], r1")
    A("clr r1")
    A("lsl r0")
    A("adc %[charRes], r1")
      : [charRes] "=&r" (charRes)
      : [charIn1] "d" (charIn1),
        [charIn2] "d" (charIn2)
      : "cc"
  );
  return charRes;
}
