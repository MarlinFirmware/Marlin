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

uint8_t LEDs[8 * (MAX7219_NUMBER_UNITS)] = { 0 };

#ifndef MAX7219_ROTATE
  #define MAX7219_ROTATE 0
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
void Max7219_pulse_load() {
  SIG_DELAY();
  WRITE(MAX7219_LOAD_PIN, LOW);  // tell the chip to load the data
  SIG_DELAY();
  WRITE(MAX7219_LOAD_PIN, HIGH);
  SIG_DELAY();
}

void Max7219(const uint8_t reg, const uint8_t data) {
  SIG_DELAY();
  #ifndef CPU_32_BIT
    CRITICAL_SECTION_START;
  #endif
  SIG_DELAY();
  Max7219_PutByte(reg);          // specify register
  SIG_DELAY();
  Max7219_PutByte(data);         // put data
  #ifndef CPU_32_BIT
    CRITICAL_SECTION_END;
  #endif
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
      Max7219_pulse_load();  // tell the chips to load the clocked out data
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

/**
 * uint32_t flipped(const uint32_t bits, const uint8_t n_bytes) operates on the number
 * of bytes specified in n_bytes.  The lower order bits of the supplied bits are flipped.
 * flipped( x, 1) flips the low 8  bits of x.
 * flipped( x, 2) flips the low 16 bits of x.
 * flipped( x, 3) flips the low 24 bits of x.
 * flipped( x, 4) flips the low 32 bits of x.
 */

inline uint32_t flipped(const uint32_t bits, const uint8_t n_bytes) {
  uint32_t mask = 1, outbits = 0;
  for (uint8_t b = 0; b < n_bytes * 8; b++) {
    outbits = (outbits << 1);
    if (bits & mask)
      outbits |= 1;
    mask <<= 1;
  }
  return outbits;
}

// Modify a single LED bit and send the changed line
void Max7219_LED_Set(const uint8_t x, const uint8_t y, const bool on) {
  if (x > (MAX7219_X_LEDS - 1) || y > (MAX7219_Y_LEDS - 1)) return Max7219_Error(PSTR("Max7219_LED_Set"), x, y);
  if (BIT_7219(x, y) == on) return;
  XOR_7219(x, y);
  SEND_7219(MAX7219_UPDATE_AXIS);
}

void Max7219_LED_On(const uint8_t x, const uint8_t y) {
  if (x > (MAX7219_X_LEDS - 1) || y > (MAX7219_Y_LEDS - 1)) return Max7219_Error(PSTR("Max7219_LED_On"), x, y);
  Max7219_LED_Set(x, y, true);
}

void Max7219_LED_Off(const uint8_t x, const uint8_t y) {
  if (x > (MAX7219_X_LEDS - 1) || y > (MAX7219_Y_LEDS - 1)) return Max7219_Error(PSTR("Max7219_LED_Off"), x, y);
  Max7219_LED_Set(x, y, false);
}

void Max7219_LED_Toggle(const uint8_t x, const uint8_t y) {
  if (x > (MAX7219_X_LEDS - 1) || y > (MAX7219_Y_LEDS - 1)) return Max7219_Error(PSTR("Max7219_LED_Toggle"), x, y);
  Max7219_LED_Set(x, y, !BIT_7219(x, y));
}

inline void _Max7219_Set_Digit_Segments(const uint8_t digit, const uint8_t val) {
  LEDs[digit] = val;
  SEND_7219(digit);
}

/**
 * void Max7219_Set_Row( const uint8_t col, const uint32_t val) plots the low order bits of
 * val to the specified row of the Max7219 matrix.  With 4 Max7219 units in the chain, it
 * is possible to display an entire 32-bit number with one call to the function (if appropriately
 * orientated).
 */
void Max7219_Set_Row(const uint8_t row, const uint32_t val) {
  if (row >= MAX7219_Y_LEDS) return Max7219_Error(PSTR("Max7219_Set_Row"), row);
  uint32_t mask = 0x0000001;
  for (uint8_t x = 0; x < MAX7219_X_LEDS; x++) {
    if (val & mask)
      SET_PIXEL_7219((MAX7219_X_LEDS-1-x), row);
    else
      CLEAR_PIXEL_7219((MAX7219_X_LEDS-1-x), row);
    mask <<= 1;
  }

  #if _ROT == 90 || _ROT == 270
    for (uint8_t x = 0; x < 8; x++)
      SEND_7219(x); // force all columns out to the Max7219 chips and strobe them
  #else
    SEND_7219(row); // force the single column out to the Max7219 chips and strobe them
  #endif
}

void Max7219_Clear_Row(const uint8_t row) {
  if (row > 7) return Max7219_Error(PSTR("Max7219_Clear_Row"), row);
  #if _ROT == 90 || _ROT == 270
    for (uint8_t col = 0; col < 8; col++) Max7219_LED_Off(col, row);
  #else
    _Max7219_Set_Digit_Segments(row, 0);
  #endif
}

/**
 * void Max7219_Set_Column( const uint8_t col, const uint32_t val) plots the low order bits of
 * val to the specified column of the Max7219 matrix.  With 4 Max7219 units in the chain, it
 * is possible to display an entire 32-bit number with one call to the function (if appropriately
 * orientated).
 */
void Max7219_Set_Column(const uint8_t col, const uint32_t val) {
  if (col >= MAX7219_X_LEDS) return Max7219_Error(PSTR("Max7219_Set_Column"), col);
  uint32_t mask = 0x0000001;
  for (uint8_t y = 0; y < MAX7219_Y_LEDS; y++) {
    if (val & mask)
      SET_PIXEL_7219(col, MAX7219_Y_LEDS-1-y);
    else
      CLEAR_PIXEL_7219(col, MAX7219_Y_LEDS-1-y);
    mask <<= 1;
  }
  #if _ROT == 90 || _ROT == 270
    SEND_7219(col); // force the column out to the Max7219 chips and strobe them
  #else
    for (uint8_t yy = 0; yy < 8; yy++)
      SEND_7219(yy); // force all columns out to the Max7219 chips and strobe them
  #endif
}

void Max7219_Clear_Column(const uint8_t col) {
  if (col >= MAX7219_X_LEDS) return Max7219_Error(PSTR("Max7219_Clear_Column"), col);

  for (uint8_t yy = 0; yy < MAX7219_Y_LEDS; yy++)
    CLEAR_PIXEL_7219(col, yy);

  #if _ROT == 90 || _ROT == 270
    SEND_7219(col); // force the column out to the Max7219 chips and strobe them
  #else
    for (uint8_t y = 0; y < 8; y++)
      SEND_7219(y); // force all columns out to the Max7219 chips and strobe them
  #endif
}

void Max7219_Clear() {
  for (uint8_t i = 0; i <= 7; i++) {                  // Clear LED bitmap
    for (uint8_t j = 0; j < MAX7219_NUMBER_UNITS; j++)
      LEDs[i + j * 8] = 0x00;
    SEND_7219(i);
  }

}

void Max7219_Set_Rows_16bits(const uint8_t y, uint32_t val) {
  #if MAX7219_X_LEDS == 8
    if (y > MAX7219_Y_LEDS - 2) return Max7219_Error(PSTR("Max7219_Set_Rows_16bits"), y, val);
    Max7219_Set_Row(y + 1, val); val >>= 8;
    Max7219_Set_Row(y + 0, val);
  #else // at least 16 bits on each row
    if (y > MAX7219_Y_LEDS - 1) return Max7219_Error(PSTR("Max7219_Set_Rows_16bits"), y, val);
    Max7219_Set_Row(y, val);
  #endif
}

void Max7219_Set_Rows_32bits(const uint8_t y, uint32_t val) {
  #if MAX7219_X_LEDS == 8
    if (y > MAX7219_Y_LEDS - 4) return Max7219_Error(PSTR("Max7219_Set_Rows_32bits"), y, val);
    Max7219_Set_Row(y + 3, val); val >>= 8;
    Max7219_Set_Row(y + 2, val); val >>= 8;
    Max7219_Set_Row(y + 1, val); val >>= 8;
    Max7219_Set_Row(y + 0, val);
  #elif MAX7219_X_LEDS == 16
    if (y > MAX7219_Y_LEDS - 2) return Max7219_Error(PSTR("Max7219_Set_Rows_32bits"), y, val);
    Max7219_Set_Row(y + 1, val); val >>= 16;
    Max7219_Set_Row(y + 0, val);
  #else // at least 24 bits on each row.  In the 3 matrix case, just display the low 24 bits
    if (y > MAX7219_Y_LEDS - 1) return Max7219_Error(PSTR("Max7219_Set_Rows_32bits"), y, val);
    Max7219_Set_Row(y, val);
  #endif
}

void Max7219_Set_Columns_16bits(const uint8_t x, uint32_t val) {
  #if MAX7219_Y_LEDS == 8
    if (x > MAX7219_X_LEDS - 2) return Max7219_Error(PSTR("Max7219_Set_Columns_16bits"), x, val);
    Max7219_Set_Column(x + 0, val); val >>= 8;
    Max7219_Set_Column(x + 1, val);
  #else // at least 16 bits in each column
    if (x > MAX7219_X_LEDS - 1) return Max7219_Error(PSTR("Max7219_Set_Columns_16bits"), x, val);
    Max7219_Set_Column(x, val);
  #endif
}

void Max7219_Set_Columns_32bits(const uint8_t x, uint32_t val) {
  #if MAX7219_Y_LEDS == 8
    if (x > MAX7219_X_LEDS - 4) return Max7219_Error(PSTR("Max7219_Set_Rows_32bits"), x, val);
    Max7219_Set_Column(x + 3, val); val >>= 8;
    Max7219_Set_Column(x + 2, val); val >>= 8;
    Max7219_Set_Column(x + 1, val); val >>= 8;
    Max7219_Set_Column(x + 0, val);
  #elif MAX7219_Y_LEDS == 16
    if (x > MAX7219_X_LEDS - 2) return Max7219_Error(PSTR("Max7219_Set_Rows_32bits"), x, val);
    Max7219_Set_Column(x + 1, val); val >>= 16;
    Max7219_Set_Column(x + 0, val);
  #else // at least 24 bits on each row.  In the 3 matrix case, just display the low 24 bits
    if (x > MAX7219_X_LEDS - 1) return Max7219_Error(PSTR("Max7219_Set_Rows_32bits"), x, val);
    Max7219_Set_Column(x, val);
  #endif
}

void Max7219_register_setup() {
  // Initialize the Max7219
  for (uint8_t i = 0; i < MAX7219_NUMBER_UNITS; i++)
    Max7219(max7219_reg_scanLimit, 0x07);
  Max7219_pulse_load();                        // tell the chips to load the clocked out data

  for (uint8_t i = 0; i < MAX7219_NUMBER_UNITS; i++)
    Max7219(max7219_reg_decodeMode, 0x00);     // using an led matrix (not digits)
  Max7219_pulse_load();                        // tell the chips to load the clocked out data

  for (uint8_t i = 0; i < MAX7219_NUMBER_UNITS; i++)
    Max7219(max7219_reg_shutdown, 0x01);       // not in shutdown mode
  Max7219_pulse_load();                        // tell the chips to load the clocked out data

  for (uint8_t i = 0; i < MAX7219_NUMBER_UNITS; i++)
    Max7219(max7219_reg_displayTest, 0x00);    // no display test
  Max7219_pulse_load();                        // tell the chips to load the clocked out data

  for (uint8_t i = 0; i < MAX7219_NUMBER_UNITS; i++)
    Max7219(max7219_reg_intensity, 0x01 & 0x0F); // the first 0x0F is the value you can set
                                               // range: 0x00 to 0x0F
  Max7219_pulse_load();                        // tell the chips to load the clocked out data
}

#ifdef MAX7219_INIT_TEST
#if (MAX7219_INIT_TEST + 0) == 2

  inline void Max7219_spiral(const bool on, const uint16_t del) {
    constexpr int8_t way[] = { 1, 0, 0, 1, -1, 0, 0, -1 };
    int8_t px = 0, py = 0, dir = 0;
    for (uint8_t i = MAX7219_X_LEDS * MAX7219_Y_LEDS; i--;) {
      Max7219_LED_Set(px, py, on);
      delay(del);
      const int8_t x = px + way[dir], y = py + way[dir + 1];
      if (!WITHIN(x, 0, MAX7219_X_LEDS-1) || !WITHIN(y, 0, MAX7219_Y_LEDS-1) || BIT_7219(x, y) == on) dir = (dir + 2) & 0x7;
      px += way[dir]; py += way[dir + 1];
    }
  }

#else

  inline void Max7219_sweep(const int8_t dir, const uint16_t ms, const bool on) {
    uint8_t x = dir > 0 ? 0 : MAX7219_X_LEDS-1;
    for (uint8_t i = MAX7219_X_LEDS; i--; x += dir) {
      Max7219_Set_Column(x, on ? 0xFFFFFFFF : 0x00000000);
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
    Max7219_pulse_load();                 // tell the chips to load the clocked out data
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
  #if MAX7219_X_LEDS == 8
    #if MAX7219_Y_LEDS == 8
      Max7219_LED_Off(v1 & 0x7, y + (v1 >= 8));
       Max7219_LED_On(v2 & 0x7, y + (v2 >= 8));
    #else
      Max7219_LED_Off(y, v1 & 0xF);  // The Max7219 Y-Axis has at least 16 LED's.  So use a single column
       Max7219_LED_On(y, v2 & 0xF);
    #endif
  #else   // LED matrix has at least 16 LED's on the X-Axis.  Use single line of LED's
    Max7219_LED_Off(v1 & 0xf, y);
     Max7219_LED_On(v2 & 0xf, y);
  #endif
}

// Apply changes to update a tail-to-head range
inline void Max7219_Range16(const uint8_t y, const uint8_t ot, const uint8_t nt, const uint8_t oh, const uint8_t nh) {
  #if MAX7219_X_LEDS == 8
    #if MAX7219_Y_LEDS == 8
      if (ot != nt) for (uint8_t n = ot & 0xF; n != (nt & 0xF) && n != (nh & 0xF); n = (n + 1) & 0xF)
        Max7219_LED_Off(n & 0x7, y + (n >= 8));
      if (oh != nh) for (uint8_t n = (oh + 1) & 0xF; n != ((nh + 1) & 0xF); n = (n + 1) & 0xF)
         Max7219_LED_On(n & 0x7, y + (n >= 8));
    #else // The Max7219 Y-Axis has at least 16 LED's.  So use a single column
      if (ot != nt) for (uint8_t n = ot & 0xF; n != (nt & 0xF) && n != (nh & 0xF); n = (n + 1) & 0xF)
        Max7219_LED_Off(y, n & 0xF);
      if (oh != nh) for (uint8_t n = (oh + 1) & 0xF; n != ((nh + 1) & 0xF); n = (n + 1) & 0xF)
         Max7219_LED_On(y, n & 0xF);
    #endif
  #else   // LED matrix has at least 16 LED's on the X-Axis.  Use single line of LED's
    if (ot != nt) for (uint8_t n = ot & 0xF; n != (nt & 0xF) && n != (nh & 0xF); n = (n + 1) & 0xF)
      Max7219_LED_Off(n & 0xf, y);
    if (oh != nh) for (uint8_t n = (oh + 1) & 0xF; n != ((nh + 1) & 0xF); n = (n + 1) & 0xF)
       Max7219_LED_On(n & 0xf, y);
 #endif
}

// Apply changes to update a quantity
inline void Max7219_Quantity16(const uint8_t y, const uint8_t ov, const uint8_t nv) {
  for (uint8_t i = MIN(nv, ov); i < MAX(nv, ov); i++)
    #if MAX7219_X_LEDS == 8
      #if MAX7219_Y_LEDS == 8
        Max7219_LED_Set(i >> 1, y + (i & 1), nv >= ov); // single 8x8 LED matrix.  Use two lines to get 16 LED's
      #else
        Max7219_LED_Set(y, i, nv >= ov);                // The Max7219 Y-Axis has at least 16 LED's.  So use a single column
      #endif
    #else
      Max7219_LED_Set(i, y, nv >= ov);                // LED matrix has at least 16 LED's on the X-Axis.  Use single line of LED's
    #endif
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
      Max7219_LED_Toggle(MAX7219_X_LEDS - 1, MAX7219_Y_LEDS - 1);
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
