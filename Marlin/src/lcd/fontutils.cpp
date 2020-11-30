/**
 * @file    fontutils.cpp
 * @brief   help functions for font and char
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */

#include "../inc/MarlinConfig.h"

#define MAX_UTF8_CHAR_SIZE 4

#if HAS_WIRED_LCD
  #include "marlinui.h"
  #include "../MarlinCore.h"
#endif

#include "fontutils.h"

uint8_t read_byte_ram(uint8_t * str) {
  return *str;
}

uint8_t read_byte_rom(uint8_t * str) {
  return pgm_read_byte(str);
}

/**
 * @brief Using binary search to find the position by data_pin
 *
 * @param userdata : User's data
 * @param num_data : the item number of the sorted data
 * @param cb_comp : the callback function to compare the user's data and pin
 * @param data_pin : The reference data to be found
 * @param ret_idx : the position of the required data; If failed, then it is the failed position, which is the insert position if possible.
 *
 * @return 0 on found, <0 on failed(fail position is saved by ret_idx)
 *
 * Using binary search to find the position by data_pin. The user's data should be sorted.
 */
int pf_bsearch_r(void *userdata, size_t num_data, pf_bsearch_cb_comp_t cb_comp, void *data_pinpoint, size_t *ret_idx) {
  int retcomp;

  if (num_data < 1) {
    *ret_idx = 0;
    return -1;
  }

  size_t i = 0, ileft = 1, iright = num_data;
  bool flg_found = false;
  for (; ileft <= iright;) {
    i = (ileft + iright) / 2 - 1;
    /* cb_comp should return the *userdata[i] - *data_pinpoint */
    retcomp = cb_comp (userdata, i, data_pinpoint);
    if (retcomp > 0)
      iright = i;
    else if (retcomp < 0)
      ileft = i + 2;
    else {
      /* found ! */
      flg_found = true;
      break;
    }
  }

  if (flg_found) {
    *ret_idx = i;
    return 0;
  }
  if (iright <= i)
    *ret_idx = i;
  else if (ileft >= i + 2)
    *ret_idx = i + 1;
  return -1;
}

/* Returns true if passed byte is first byte of UTF-8 char sequence */
static inline bool utf8_is_start_byte_of_char(const uint8_t b) {
  return 0x80 != (b & 0xC0);
}

/* This function gets the character at the pstart position, interpreting UTF8 multibyte sequences
   and returns the pointer to the next character */
uint8_t* get_utf8_value_cb(uint8_t *pstart, read_byte_cb_t cb_read_byte, wchar_t *pval) {
  uint32_t val = 0;
  uint8_t *p = pstart;

  #define NEXT_6_BITS() do{ val <<= 6; p++; valcur = cb_read_byte(p); val |= (valcur & 0x3F); }while(0)

  uint8_t valcur = cb_read_byte(p);
  if (0 == (0x80 & valcur)) {
    val = valcur;
    p++;
  }
  else if (0xC0 == (0xE0 & valcur)) {
    val = valcur & 0x1F;
    NEXT_6_BITS();
    p++;
  }
  #if MAX_UTF8_CHAR_SIZE >= 3
    else if (0xE0 == (0xF0 & valcur)) {
      val = valcur & 0x0F;
      NEXT_6_BITS();
      NEXT_6_BITS();
      p++;
    }
  #endif
  #if MAX_UTF8_CHAR_SIZE >= 4
    else if (0xF0 == (0xF8 & valcur)) {
      val = valcur & 0x07;
      NEXT_6_BITS();
      NEXT_6_BITS();
      NEXT_6_BITS();
      p++;
    }
  #endif
  #if MAX_UTF8_CHAR_SIZE >= 5
    else if (0xF8 == (0xFC & valcur)) {
      val = valcur & 0x03;
      NEXT_6_BITS();
      NEXT_6_BITS();
      NEXT_6_BITS();
      NEXT_6_BITS();
      p++;
    }
  #endif
  #if MAX_UTF8_CHAR_SIZE >= 6
    else if (0xFC == (0xFE & valcur)) {
      val = valcur & 0x01;
      NEXT_6_BITS();
      NEXT_6_BITS();
      NEXT_6_BITS();
      NEXT_6_BITS();
      NEXT_6_BITS();
      p++;
    }
  #endif
  else if (!utf8_is_start_byte_of_char(valcur))
    for (; !utf8_is_start_byte_of_char(valcur); ) { p++; valcur = cb_read_byte(p); }
  else
    for (; 0xFC < (0xFE & valcur); ) { p++; valcur = cb_read_byte(p); }

  if (pval) *pval = val;

  return p;
}

static inline uint8_t utf8_strlen_cb(const char *pstart, read_byte_cb_t cb_read_byte) {
  uint8_t cnt = 0;
  uint8_t *p = (uint8_t *)pstart;
  for (;;) {
    const uint8_t b = cb_read_byte(p);
    if (!b) break;
    if (utf8_is_start_byte_of_char(b)) cnt++;
    p++;
  }
  return cnt;
}

uint8_t utf8_strlen(const char *pstart) {
  return utf8_strlen_cb(pstart, read_byte_ram);
}

uint8_t utf8_strlen_P(PGM_P pstart) {
  return utf8_strlen_cb(pstart, read_byte_rom);
}

static inline uint8_t utf8_byte_pos_by_char_num_cb(const char *pstart, read_byte_cb_t cb_read_byte, const uint8_t charnum) {
  uint8_t *p = (uint8_t *)pstart;
  uint8_t char_idx = 0;
  uint8_t byte_idx = 0;
  for (;;) {
    const uint8_t b = cb_read_byte(p + byte_idx);
    if (!b) return byte_idx; // Termination byte of string
    if (utf8_is_start_byte_of_char(b)) {
      char_idx++;
      if (char_idx == charnum + 1) return byte_idx;
    }
    byte_idx++;
  }
}

uint8_t utf8_byte_pos_by_char_num(const char *pstart, const uint8_t charnum) {
  return utf8_byte_pos_by_char_num_cb(pstart, read_byte_ram, charnum);
}

uint8_t utf8_byte_pos_by_char_num_P(PGM_P pstart, const uint8_t charnum) {
  return utf8_byte_pos_by_char_num_cb(pstart, read_byte_rom, charnum);
}
