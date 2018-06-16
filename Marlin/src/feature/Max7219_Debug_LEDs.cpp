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

/**
 * This module is off by default, but can be enabled to facilitate the display of
 * extra debug information during code development.
 *
 * Just connect up 5V and GND to give it power, then connect up the pins assigned
 * in Configuration_adv.h. For example, on the Re-ARM you could use:
 *
 *   #define MAX7219_CLK_PIN   77
 *   #define MAX7219_DIN_PIN   78
 *   #define MAX7219_LOAD_PIN  79
 *
 * Max7219_init() is called automatically at startup, and then there are a number of
 * support functions available to control the LEDs in the 8x8 grid.
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(MAX7219_DEBUG)

#define MAX7219_ERRORS // Disable to save 406 bytes of Program Memory

#include "Max7219_Debug_LEDs.h"

#include "../module/planner.h"
#include "../module/stepper.h"
#include "../Marlin.h"
#include "../HAL/Delay.h"

static uint8_t LEDs[8] = { 0 };

#ifndef MAX7219_ROTATE
  #define MAX7219_ROTATE 0
#endif
#define _ROT ((MAX7219_ROTATE + 360) % 360)
#if _ROT == 0
  #define _ROW_ y
  #define _COL_ x
  #define XOR_7219(x, y) LEDs[y] ^= _BV(7 - x)
  #define BIT_7219(x, y) TEST(LEDs[y], 7 - x)
  #define SEND_7219(R,V) Max7219(max7219_reg_digit0 + R, V)
#elif _ROT == 90
  #define _ROW_ x
  #define _COL_ y
  #define XOR_7219(x, y) LEDs[x] ^= _BV(y)
  #define BIT_7219(x, y) TEST(LEDs[x], y)
  #define SEND_7219(R,V) Max7219(max7219_reg_digit0 + R, V)
#elif _ROT == 180
  #define _ROW_ y
  #define _COL_ x
  #define XOR_7219(x, y) LEDs[y] ^= _BV(x)
  #define BIT_7219(x, y) TEST(LEDs[y], x)
  #define SEND_7219(R,V) Max7219(max7219_reg_digit7 - R, V)
#elif _ROT == 270
  #define _ROW_ x
  #define _COL_ y
  #define XOR_7219(x, y) LEDs[x] ^= _BV(7 - y)
  #define BIT_7219(x, y) TEST(LEDs[x], 7 - y)
  #define SEND_7219(R,V) Max7219(max7219_reg_digit7 - R, V)
#else
  #error "MAX7219_ROTATE must be a multiple of +/- 90°."
#endif

#ifdef CPU_32_BIT
  // Approximate a 1µs delay on 32-bit ARM
  #define SIG_DELAY() DELAY_US(1)
#else
  // Delay for 0.1875µs (16MHz AVR) or 0.15µs (20MHz AVR)
  #define SIG_DELAY() DELAY_NS(188)
#endif

void Max7219_PutByte(uint8_t data) {
  #ifndef CPU_32_BIT
    CRITICAL_SECTION_START;
  #endif
  for (uint8_t i = 8; i--;) {
    SIG_DELAY();
    WRITE(MAX7219_CLK_PIN, LOW);       // tick
    SIG_DELAY();
    WRITE(MAX7219_DIN_PIN, (data & 0x80) ? HIGH : LOW);  // send 1 or 0 based on data bit
    SIG_DELAY();
    WRITE(MAX7219_CLK_PIN, HIGH);      // tock
    SIG_DELAY();
    data <<= 1;
  }
  #ifndef CPU_32_BIT
    CRITICAL_SECTION_END;
  #endif
}

void Max7219(const uint8_t reg, const uint8_t data) {
  SIG_DELAY();
  #ifndef CPU_32_BIT
    CRITICAL_SECTION_START;
  #endif
  WRITE(MAX7219_LOAD_PIN, LOW);  // begin
  SIG_DELAY();
  Max7219_PutByte(reg);          // specify register
  SIG_DELAY();
  Max7219_PutByte(data);         // put data
  SIG_DELAY();
  WRITE(MAX7219_LOAD_PIN, LOW);  // and tell the chip to load the data
  SIG_DELAY();
  WRITE(MAX7219_LOAD_PIN, HIGH);
  #ifndef CPU_32_BIT
    CRITICAL_SECTION_END;
  #endif
  SIG_DELAY();
}

#if ENABLED(MAX7219_NUMERIC)

  // Draw an integer with optional leading zeros and optional decimal point
  void Max7219_Print(const uint8_t start, int16_t value, uint8_t size, const bool leadzero=false, bool dec=false) {
    constexpr uint8_t led_numeral[10] = { 0x7E, 0x60, 0x6D, 0x79, 0x63, 0x5B, 0x5F, 0x70, 0x7F, 0x7A },
                      led_decimal = 0x80, led_minus = 0x01;

    bool blank = false, neg = value < 0;
    if (neg) value *= -1;
    while (size--) {
      const bool minus = neg && blank;
      if (minus) neg = false;
      Max7219(
        max7219_reg_digit0 + start + size,
        minus ? led_minus : blank ? 0x00 : led_numeral[value % 10] | (dec ? led_decimal : 0x00)
      );
      value /= 10;
      if (!value && !leadzero) blank = true;
      dec = false;
    }
  }

  // Draw a float with a decimal point and optional digits
  void Max7219_Print(const uint8_t start, const float value, const uint8_t pre_size, const uint8_t post_size, const bool leadzero=false) {
    if (pre_size) Max7219_Print(start, value, pre_size, leadzero, !!post_size);
    if (post_size) {
      const int16_t after = ABS(value) * (10 ^ post_size);
      Max7219_Print(start + pre_size, after, post_size, true);
    }
  }

#endif // MAX7219_NUMERIC

inline void Max7219_Error(const char * const func, const int32_t v1, const int32_t v2=-1) {
  #if ENABLED(MAX7219_ERRORS)
    SERIAL_ECHOPGM("??? ");
    serialprintPGM(func);
    SERIAL_CHAR('(');
    SERIAL_ECHO(v1);
    if (v2 > 0) SERIAL_ECHOPAIR(", ", v2);
    SERIAL_CHAR(')');
    SERIAL_EOL();
  #else
    UNUSED(func); UNUSED(v1); UNUSED(v2);
  #endif
}

inline uint8_t flipped(const uint8_t bits) {
  uint8_t outbits = 0;
  for (uint8_t b = 0; b < 8; b++)
    if (bits & _BV(b)) outbits |= _BV(7 - b);
  return outbits;
}

// Modify a single LED bit and send the changed line
void Max7219_LED_Set(const uint8_t x, const uint8_t y, const bool on) {
  if (x > 7 || y > 7) return Max7219_Error(PSTR("Max7219_LED_Set"), x, y);
  if (BIT_7219(x, y) == on) return;
  XOR_7219(x, y);
  SEND_7219(_ROW_, LEDs[_ROW_]);
}

void Max7219_LED_On(const uint8_t x, const uint8_t y) {
  if (x > 7 || y > 7) return Max7219_Error(PSTR("Max7219_LED_On"), x, y);
  Max7219_LED_Set(x, y, true);
}

void Max7219_LED_Off(const uint8_t x, const uint8_t y) {
  if (x > 7 || y > 7) return Max7219_Error(PSTR("Max7219_LED_Off"), x, y);
  Max7219_LED_Set(x, y, false);
}

void Max7219_LED_Toggle(const uint8_t x, const uint8_t y) {
  if (x > 7 || y > 7) return Max7219_Error(PSTR("Max7219_LED_Toggle"), x, y);
  Max7219_LED_Set(x, y, !BIT_7219(x, y));
}

inline void _Max7219_Set_Reg(const uint8_t reg, const uint8_t val) {
  LEDs[reg] = val;
  SEND_7219(reg, val);
}

void Max7219_Set_Row(const uint8_t _ROW_, const uint8_t val) {
  if (_ROW_ > 7) return Max7219_Error(PSTR("Max7219_Set_Row"), _ROW_);
  #if _ROT == 90
    for (uint8_t _COL_ = 0; _COL_ <= 7; _COL_++) Max7219_LED_Set(7 - _COL_, _ROW_, TEST(val, _COL_));
  #elif _ROT == 180
    _Max7219_Set_Reg(_ROW_, flipped(val));
  #elif _ROT == 270
    for (uint8_t _COL_ = 0; _COL_ <= 7; _COL_++) Max7219_LED_Set(_COL_, _ROW_, TEST(val, _COL_));
  #else
    _Max7219_Set_Reg(_ROW_, val);
  #endif
}

void Max7219_Clear_Row(const uint8_t _ROW_) {
  if (_ROW_ > 7) return Max7219_Error(PSTR("Max7219_Clear_Row"), _ROW_);
  #if _ROT == 90 || _ROT == 270
    for (uint8_t _COL_ = 0; _COL_ <= 7; _COL_++) Max7219_LED_Off(_COL_, _ROW_);
  #else
    _Max7219_Set_Reg(_ROW_, 0);
  #endif
}

void Max7219_Set_Column(const uint8_t _COL_, const uint8_t val) {
  if (_COL_ > 7) return Max7219_Error(PSTR("Max7219_Set_Column"), _COL_);
  #if _ROT == 90
    _Max7219_Set_Reg(_COL_, val);
  #elif _ROT == 180
    for (uint8_t _ROW_ = 0; _ROW_ <= 7; _ROW_++) Max7219_LED_Set(_COL_, _ROW_, TEST(val, _ROW_));
  #elif _ROT == 270
    _Max7219_Set_Reg(_COL_, flipped(val));
  #else
    for (uint8_t _ROW_ = 0; _ROW_ <= 7; _ROW_++) Max7219_LED_Set(_COL_, _ROW_, TEST(val, _ROW_));
  #endif
}

void Max7219_Clear_Column(const uint8_t _COL_) {
  if (_COL_ > 7) return Max7219_Error(PSTR("Max7219_Clear_Column"), _COL_);
  #if _ROT == 90 || _ROT == 270
    _Max7219_Set_Reg(_COL_, 0);
  #else
    for (uint8_t _ROW_ = 0; _ROW_ <= 7; _ROW_++) Max7219_LED_Off(_COL_, _ROW_);
  #endif
}

void Max7219_Set_2_Rows(const uint8_t y, uint16_t val) {
  if (y > 6) return Max7219_Error(PSTR("Max7219_Set_2_Rows"), y, val);
  Max7219_Set_Row(y + 0, val & 0xFF); val >>= 8;
  Max7219_Set_Row(y + 1, val & 0xFF);
}

void Max7219_Set_4_Rows(const uint8_t y, uint32_t val) {
  if (y > 4) return Max7219_Error(PSTR("Max7219_Set_4_Rows"), y, val);
  Max7219_Set_Row(y + 0, val & 0xFF); val >>= 8;
  Max7219_Set_Row(y + 1, val & 0xFF); val >>= 8;
  Max7219_Set_Row(y + 2, val & 0xFF); val >>= 8;
  Max7219_Set_Row(y + 3, val & 0xFF);
}

void Max7219_Set_2_Columns(const uint8_t x, uint16_t val) {
  if (x > 6) return Max7219_Error(PSTR("Max7219_Set_2_Columns"), x, val);
  Max7219_Set_Column(x + 0, val & 0xFF); val >>= 8;
  Max7219_Set_Column(x + 1, val & 0xFF);
}

void Max7219_Set_4_Columns(const uint8_t x, uint32_t val) {
  if (x > 4) return Max7219_Error(PSTR("Max7219_Set_4_Columns"), x, val);
  Max7219_Set_Column(x + 0, val & 0xFF); val >>= 8;
  Max7219_Set_Column(x + 1, val & 0xFF); val >>= 8;
  Max7219_Set_Column(x + 2, val & 0xFF); val >>= 8;
  Max7219_Set_Column(x + 3, val & 0xFF);
}

void Max7219_register_setup() {
  // Initialize the Max7219
  Max7219(max7219_reg_scanLimit, 0x07);
  Max7219(max7219_reg_decodeMode, 0x00);       // using an led matrix (not digits)
  Max7219(max7219_reg_shutdown, 0x01);         // not in shutdown mode
  Max7219(max7219_reg_displayTest, 0x00);      // no display test
  Max7219(max7219_reg_intensity, 0x01 & 0x0F); // the first 0x0F is the value you can set
                                               // range: 0x00 to 0x0F
}

#ifdef MAX7219_INIT_TEST
#if (MAX7219_INIT_TEST + 0) == 2

  inline void Max7219_spiral(const bool on, const uint16_t del) {
    constexpr int8_t way[] = { 1, 0, 0, 1, -1, 0, 0, -1 };
    int8_t px = 0, py = 0, dir = 0;
    for (uint8_t i = 64; i--;) {
      Max7219_LED_Set(px, py, on);
      delay(del);
      const int8_t x = px + way[dir], y = py + way[dir + 1];
      if (!WITHIN(x, 0, 7) || !WITHIN(y, 0, 7) || BIT_7219(x, y) == on) dir = (dir + 2) & 0x7;
      px += way[dir]; py += way[dir + 1];
    }
  }

#else

  inline void Max7219_colset(const uint8_t x, const bool on) {
    for (uint8_t y = 0; y <= 7; y++) Max7219_LED_Set(x, y, on);
  }
  inline void Max7219_sweep(const int8_t dir, const uint16_t ms, const bool on) {
    uint8_t x = dir > 0 ? 0 : 7;
    for (uint8_t i = 8; i--; x += dir) {
      Max7219_Set_Column(x, on ? 0xFF : 0x00);
      delay(ms);
    }
  }

#endif
#endif // MAX7219_INIT_TEST

void Max7219_init() {
  SET_OUTPUT(MAX7219_DIN_PIN);
  SET_OUTPUT(MAX7219_CLK_PIN);
  OUT_WRITE(MAX7219_LOAD_PIN, HIGH);
  delay(1);

  Max7219_register_setup();

  for (uint8_t i = 0; i <= 7; i++) {      // Empty registers to turn all LEDs off
    LEDs[i] = 0x00;
    Max7219(max7219_reg_digit0 + i, 0);
  }

  #ifdef MAX7219_INIT_TEST
    #if (MAX7219_INIT_TEST + 0) == 2
      Max7219_spiral(true, 8);
      delay(150);
      Max7219_spiral(false, 8);
    #else
      // Do an aesthetically-pleasing pattern to fully test the Max7219 module and LEDs.
      // Light up and turn off columns, both forward and backward.
      Max7219_sweep(1, 20, true);
      Max7219_sweep(1, 20, false);
      delay(150);
      Max7219_sweep(-1, 20, true);
      Max7219_sweep(-1, 20, false);
    #endif
  #endif
}

/**
 * This code demonstrates some simple debugging using a single 8x8 LED Matrix. If your feature could
 * benefit from matrix display, add its code here. Very little processing is required, so the 7219 is
 * ideal for debugging when realtime feedback is important but serial output can't be used.
 */

// Apply changes to update a marker
inline void Max7219_Mark16(const uint8_t y, const uint8_t v1, const uint8_t v2) {
  Max7219_LED_Off(v1 & 0x7, y + (v1 >= 8));
   Max7219_LED_On(v2 & 0x7, y + (v2 >= 8));
}

// Apply changes to update a tail-to-head range
inline void Max7219_Range16(const uint8_t y, const uint8_t ot, const uint8_t nt, const uint8_t oh, const uint8_t nh) {
  if (ot != nt) for (uint8_t n = ot & 0xF; n != (nt & 0xF) && n != (nh & 0xF); n = (n + 1) & 0xF)
    Max7219_LED_Off(n & 0x7, y + (n >= 8));
  if (oh != nh) for (uint8_t n = (oh + 1) & 0xF; n != ((nh + 1) & 0xF); n = (n + 1) & 0xF)
     Max7219_LED_On(n & 0x7, y + (n >= 8));
}

// Apply changes to update a quantity
inline void Max7219_Quantity16(const uint8_t y, const uint8_t ov, const uint8_t nv) {
  for (uint8_t i = MIN(nv, ov); i < MAX(nv, ov); i++)
    Max7219_LED_Set(i >> 1, y + (i & 1), nv >= ov);
}

void Max7219_idle_tasks() {
  #define MAX7219_USE_HEAD (defined(MAX7219_DEBUG_PLANNER_HEAD) || defined(MAX7219_DEBUG_PLANNER_QUEUE))
  #define MAX7219_USE_TAIL (defined(MAX7219_DEBUG_PLANNER_TAIL) || defined(MAX7219_DEBUG_PLANNER_QUEUE))
  #if MAX7219_USE_HEAD || MAX7219_USE_TAIL
    #ifndef CPU_32_BIT
      CRITICAL_SECTION_START;
    #endif
    #if MAX7219_USE_HEAD
      const uint8_t head = planner.block_buffer_head;
    #endif
    #if MAX7219_USE_TAIL
      const uint8_t tail = planner.block_buffer_tail;
    #endif
    #ifndef CPU_32_BIT
      CRITICAL_SECTION_END;
    #endif
  #endif

  #if ENABLED(MAX7219_DEBUG_PRINTER_ALIVE)
    static uint8_t refresh_cnt; // = 0
    constexpr uint16_t refresh_limit = 5;
    static millis_t next_blink = 0;
    const millis_t ms = millis();
    const bool do_blink = ELAPSED(ms, next_blink);
  #else
    static uint16_t refresh_cnt; // = 0
    constexpr bool do_blink = true;
    constexpr uint16_t refresh_limit = 50000;
  #endif

  // Some Max7219 units are vulnerable to electrical noise, especially
  // with long wires next to high current wires. If the display becomes
  // corrupted, this will fix it within a couple seconds.
  if (do_blink && ++refresh_cnt >= refresh_limit) {
    refresh_cnt = 0;
    Max7219_register_setup();
  }

  #if ENABLED(MAX7219_DEBUG_PRINTER_ALIVE)
    if (do_blink) {
      Max7219_LED_Toggle(7, 7);
      next_blink = ms + 1000;
    }
  #endif

  #if defined(MAX7219_DEBUG_PLANNER_HEAD) && defined(MAX7219_DEBUG_PLANNER_TAIL) && MAX7219_DEBUG_PLANNER_HEAD == MAX7219_DEBUG_PLANNER_TAIL

    static int16_t last_head_cnt = 0xF, last_tail_cnt = 0xF;

    if (last_head_cnt != head || last_tail_cnt != tail) {
      Max7219_Range16(MAX7219_DEBUG_PLANNER_HEAD, last_tail_cnt, tail, last_head_cnt, head);
      last_head_cnt = head;
      last_tail_cnt = tail;
    }

  #else

    #ifdef MAX7219_DEBUG_PLANNER_HEAD
      static int16_t last_head_cnt = 0x1;
      if (last_head_cnt != head) {
        Max7219_Mark16(MAX7219_DEBUG_PLANNER_HEAD, last_head_cnt, head);
        last_head_cnt = head;
      }
    #endif

    #ifdef MAX7219_DEBUG_PLANNER_TAIL
      static int16_t last_tail_cnt = 0x1;
      if (last_tail_cnt != tail) {
        Max7219_Mark16(MAX7219_DEBUG_PLANNER_TAIL, last_tail_cnt, tail);
        last_tail_cnt = tail;
      }
    #endif

  #endif

  #ifdef MAX7219_DEBUG_PLANNER_QUEUE
    static int16_t last_depth = 0;
    const int16_t current_depth = (head - tail + BLOCK_BUFFER_SIZE) & (BLOCK_BUFFER_SIZE - 1) & 0xF;
    if (current_depth != last_depth) {
      Max7219_Quantity16(MAX7219_DEBUG_PLANNER_QUEUE, last_depth, current_depth);
      last_depth = current_depth;
    }
  #endif
}

#endif // MAX7219_DEBUG
