/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2021 X-Ryl669
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

#include "bug_on.h"

#define MAX_ALLOC_SIZE      1024
#define ALLOC_DEFAULT_ALIGN 4

struct RAIIBuffer;

/** A simple stack-like allocator with N sized buffer pool
    @param N        Must be a power of two
    @param AllocMax Usually set to MAX_ALLOC_SIZE */
template<size_t N, size_t AllocMax>
class StackLikeAllocator
{
  uint8_t   buffer[AllocMax];
  uint16_t  current;

  inline static StackLikeAllocator & instance() { static StackLikeAllocator s; return s; }

  // Only let direct access to the RAIIBuffer class
  friend struct RAIIBuffer;
  friend struct DString;


public:
  bool grow(uint16_t amount) {
    if (amount + current >= AllocMax) {
      BUG_ON("Overflowing allocator ", amount + current, AllocMax);
      return false;
    }
    current += amount;
    return true;
  }

  // Round up to align with N
  constexpr static inline uint16_t roundUp(uint16_t x) { return (x + N - 1) & ~(N-1); }

  uint8_t * lease(uint16_t size) {
    size = roundUp(size);
    uint8_t * t = &buffer[size];
    if (!grow(size)) return nullptr;
    return t;
  }

  void release(uint16_t size) {
    size = roundUp(size);
    if (size > current) {
      BUG_ON("Releasing more than expected ", size, current);
      current = 0;
    } else current -= size;
  }

  StackLikeAllocator() : current(0) {
    static_assert(((N & (N-1)) == 0) && N, "N should be a power of 2");
  }
};

typedef StackLikeAllocator<ALLOC_DEFAULT_ALIGN, MAX_ALLOC_SIZE> SLAlloc;

struct RAIIBuffer
{
  uint8_t * buf;
  uint16_t size;

  template <typename T>
  inline operator uint8_t * () const { return buf; }

  bool grow(const uint16_t amount) {
    if (SLAlloc::instance().grow(amount)) {
      size += amount;
      return true;
    }
    return false;
  }

  RAIIBuffer(const uint16_t size) : buf(SLAlloc::instance().lease(size)), size(size) {}
  ~RAIIBuffer() { SLAlloc::instance().release(size); }
};
