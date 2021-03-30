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
#include "../inc/MarlinConfig.h"

#include "types.h" // For size_t
#include "macros.h" // For min
#include "string.h"

static char to_lower(char c) {
	if  ( (c >= 65) && (c <= 90) )
		return c + 32;
	return c;
}


const unsigned int ROString::find(const ROString & needle, unsigned int pos, const bool caseless) const {
  for (unsigned int j = 0; pos + j < (unsigned int)length;) {
    if ( (!caseless && needle.data[j] == data[pos + j])
      || ( caseless && to_lower(needle.data[j]) == to_lower(data[pos + j]))) {
      j++;
      if (j == (unsigned int)needle.length)	return pos;
      continue;
    }
    pos++;
    j = 0;
  }
  return (unsigned int)length;
}
const unsigned int ROString::reverseFind(const ROString & needle, unsigned int pos, const bool caseless) const {
  if (needle.length > length) return length;
  unsigned int i = _MIN(pos, (unsigned int)(length - needle.length)); // If there is no space to find out the needle at the end, simply snap back
  for (unsigned int j = 0;;) {
    if ( (!caseless && needle.data[j] == data[i + j])
      || ( caseless && to_lower(needle.data[j]) == to_lower(data[i + j]))) {
      j ++;
      if (j >= (unsigned int)needle.length) return i;
    } else {
      if (i-- == 0) break;
      j = 0;
    }
  }
  return length;
}

const unsigned int ROString::count(const ROString & needle) const {
  int pos = -1; unsigned int count = 0;
  while ((pos = find(needle, pos+1)) != -1) count++;
  return count;
}

const ROString ROString::splitFrom(const ROString & f, const bool includeFind, const bool caseless) {
  const unsigned int pos = find(f, 0, caseless);
  if (pos == (unsigned int)length) {
    if (includeFind) {
      ROString ret(*this);
      (void)mutate(data + length, 0);
      return ret;
    }
    return ROString("", 0);
  }
  const int size = pos + f.length;
  ROString ret(data, includeFind ? size : pos);
  (void)mutate(data + size, length - size);
  return ret;
}

ROString ROString::splitAt(int pos, int stripFromRet) {
  if (stripFromRet > pos) stripFromRet = pos;
  if (pos < 0) return ROString();
  ROString ret(data, _MIN(pos - stripFromRet, length));
  if (pos > length) (void)mutate(data, 0);
  else (void)mutate(data + pos, length - pos);
  return ret;
}

const ROString ROString::fromTo(const ROString & from, const ROString & to, const bool includeFind, const bool caseless) const {
  const unsigned int fromPos = find(from, 0, caseless);
  const unsigned int toPos = find(to, fromPos + from.length, caseless);
  return ROString(fromPos >= (unsigned int)length ? "" : &data[includeFind ? fromPos : fromPos + (unsigned int)from.length],
                                  toPos < (unsigned int)length ? (includeFind ? toPos + (unsigned int)to.length - fromPos : toPos - fromPos - (unsigned int)from.length)
                                      // If the "to" needle was not found, either we return the whole string (includeFind) or an empty string
                                      : (includeFind ? (unsigned int)length - fromPos : 0));
}

// Get the string up to the first occurrence of the given string
const ROString ROString::upToFirst(const ROString & f, const bool includeFind, const bool caseless) const {
  const unsigned int pos = find(f, 0, caseless);
  return ROString(pos == (unsigned int)length && includeFind ? "" : data, includeFind ? (pos == (unsigned int)length ? 0 : pos + (unsigned int)f.length) : pos);
}
// Get the string up to the last occurrence of the given string
const ROString ROString::upToLast(const ROString & f, const bool includeFind, const bool caseless) const {
  const unsigned int pos = reverseFind(f, (unsigned int)-1, caseless);
  return ROString(pos == (unsigned int)length && includeFind ? "" : data, includeFind ? (pos == (unsigned int)length ? 0 : pos + (unsigned int)f.length) : pos);
}
// Get the string from the last occurrence of the given string.
const ROString ROString::fromLast(const ROString & f, const bool includeFind, const bool caseless) const {
  const unsigned int pos = reverseFind(f, (unsigned int)-1, caseless);
  return ROString(pos == (unsigned int)length ? (includeFind ? data : "") : &data[includeFind ? pos : pos + (unsigned int)f.length],
                                  pos == (unsigned int)length ? (includeFind ? (unsigned int)length : 0) : (includeFind ? (unsigned int)length - pos : (unsigned int)length - pos - (unsigned int)f.length));
}
// Get the string from the first occurrence of the given string
const ROString ROString::fromFirst(const ROString & f, const bool includeFind, const bool caseless) const {
  const unsigned int pos = find(f, 0, caseless);
  return ROString(pos == (unsigned int)length ? (includeFind ? data : "") : &data[includeFind ? pos : pos + (unsigned int)f.length],
                                  pos == (unsigned int)length ? (includeFind ? (unsigned int)length : 0)
                                                    : (includeFind ? (unsigned int)length - pos
                                                                : (unsigned int)length - pos - (unsigned int)f.length));
}
// Get the string from the first occurrence of the given string
const ROString ROString::dropUpTo(const ROString & f, const bool includeFind, const bool caseless) const {
  const unsigned int pos = find(f, 0, caseless);
  return ROString(pos == (unsigned int)length ? data : &data[includeFind ? pos : pos + (unsigned int)f.length],
                                  pos == (unsigned int)length ? (unsigned int)length : (includeFind ? (unsigned int)length - pos
                                                                : (unsigned int)length - pos - (unsigned int)f.length));
}
// Get the substring up to the given needle if found, or the whole string if not, and split from here.
const ROString ROString::splitUpTo(const ROString & f, const bool includeFind, const bool caseless) {
  const unsigned int pos = find(f, 0, caseless);
  if (pos == (unsigned int)length) {
    ROString ret(*this);
    (void)mutate(data + length, 0);
    return ret;
  }
  const int size = pos + f.length;
  ROString ret(data, includeFind ? size : pos);
  (void)mutate(data+size, length - size);
  return ret;
}

const ROString ROString::splitWhenNoMore(const ROString & f) {
  const unsigned int pos = invFindAnyChar(f.data, 0, f.length);
  if (pos == (unsigned int)length) {
    ROString ret(*this);
    (void)mutate(data + length, 0);
    return ret;
  }
  ROString ret(data, pos);
  (void)mutate(data+pos, length - pos);
  return ret;
}


// Compare operator
const bool ROString::operator == (const StringBase & copy) const {
    return (uint16_t)length == copy.len && memcmp(data, copy.buffer, length) == 0;
}
// Conversion
ROString::ROString(const StringBase & other) : data((const char*)other.buffer), length(other.len) {}
