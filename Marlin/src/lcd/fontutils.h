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

/**
 * @file    fontutils.h
 * @brief   help functions for font and char
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */
#pragma once

#include <stdlib.h>
#include <stdint.h> // uint32_t

#include "../HAL/shared/Marduino.h"
#include "../core/macros.h"

#define MAX_UTF8_CHAR_SIZE 4

// Use a longer character type (if needed) because wchar_t is only 16 bits wide
#ifdef MAX_UTF8_CHAR_SIZE
  #if MAX_UTF8_CHAR_SIZE > 2
    typedef uint32_t lchar_t;
  #else
    typedef wchar_t lchar_t;
  #endif
#else
  #define wchar_t uint32_t
#endif

// read a byte from ROM or RAM
typedef uint8_t (*read_byte_cb_t)(const uint8_t * str);

uint8_t read_byte_ram(const uint8_t *str);
uint8_t read_byte_rom(const uint8_t *str);

typedef uint16_t pixel_len_t;
#define PIXEL_LEN_NOLIMIT ((pixel_len_t)(-1))

/* Perform binary search */
typedef int (* pf_bsearch_cb_comp_t)(void *userdata, size_t idx, void * data_pin);
int pf_bsearch_r(void *userdata, size_t num_data, pf_bsearch_cb_comp_t cb_comp, void *data_pinpoint, size_t *ret_idx);

/* Get the character, decoding multibyte UTF8 characters and returning a pointer to the start of the next UTF8 character */
const uint8_t* get_utf8_value_cb(const uint8_t *pstart, read_byte_cb_t cb_read_byte, lchar_t &pval);

inline const char* get_utf8_value_cb(const char *pstart, read_byte_cb_t cb_read_byte, lchar_t &pval) {
  return (const char *)get_utf8_value_cb((const uint8_t *)pstart, cb_read_byte, pval);
}

/* Returns length of string in CHARACTERS, NOT BYTES */
uint8_t utf8_strlen(const char *pstart);
uint8_t utf8_strlen_P(PGM_P pstart);
inline uint8_t utf8_strlen(FSTR_P fstart) { return utf8_strlen_P(FTOP(fstart)); }

/* Returns start byte position of desired char number */
uint8_t utf8_byte_pos_by_char_num(const char *pstart, const uint8_t charnum);
uint8_t utf8_byte_pos_by_char_num_P(PGM_P pstart, const uint8_t charnum);
