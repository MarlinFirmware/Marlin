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
 * extra debug information during code development. It assumes the existence of a
 * Max7219 LED Matrix. A suitable device can be obtained on eBay similar to this:
 * http://www.ebay.com/itm/191781645249 for under $2.00 including shipping.
 *
 * Just connect up +5v and GND to give it power, then connect up the pins assigned
 * in Configuration_adv.h. For example, on the Re-ARM you could use:
 *
 *   #define MAX7219_CLK_PIN   77
 *   #define MAX7219_DIN_PIN   78
 *   #define MAX7219_LOAD_PIN  79
 *
 * Max7219_init() is called automatically at startup, and then there are a number of
 * support functions available to control the LEDs in the 8x8 grid.
 *
 * void Max7219_init();
 * void Max7219_PutByte(uint8_t data);
 * void Max7219(uint8_t reg, uint8_t data);
 * void Max7219_LED_On(uint8_t col, uint8_t row);
 * void Max7219_LED_Off(uint8_t col, uint8_t row);
 * void Max7219_LED_Toggle(uint8_t col, uint8_t row);
 * void Max7219_Clear_Row(uint8_t row);
 * void Max7219_Clear_Column(uint8_t col);
 * void Max7219_Set_Row(uint8_t row, uint8_t val);
 * void Max7219_Set_2_Rows(uint8_t row, uint16_t val);
 * void Max7219_Set_4_Rows(uint8_t row, uint32_t val);
 * void Max7219_Set_Column(uint8_t col, uint8_t val);
 * void Max7219_idle_tasks();
 */

#include "MarlinConfig.h"

#if ENABLED(MAX7219_DEBUG)

#include "Max7219_Debug_LEDs.h"

#include "planner.h"
#include "stepper.h"
#include "Marlin.h"

static uint8_t LEDs[8] = { 0 };

#ifdef CPU_32_BIT
  #define MS_DELAY() delayMicroseconds(5)  // 32-bit processors need a delay to stabilize the signal
#else
  #define MS_DELAY() NOOP
#endif

void Max7219_PutByte(uint8_t data) {
  CRITICAL_SECTION_START
  for (uint8_t i = 8; i--;) {
    MS_DELAY();
    WRITE(MAX7219_CLK_PIN, LOW);       // tick
    MS_DELAY();
    WRITE(MAX7219_DIN_PIN, (data & 0x80) ? HIGH : LOW);  // send 1 or 0 based on data bit
    MS_DELAY();
    WRITE(MAX7219_CLK_PIN, HIGH);      // tock
    MS_DELAY();
    data <<= 1;
  }
  CRITICAL_SECTION_END
}

void Max7219(const uint8_t reg, const uint8_t data) {
  MS_DELAY();
  CRITICAL_SECTION_START
  WRITE(MAX7219_LOAD_PIN, LOW);  // begin
  MS_DELAY();
  Max7219_PutByte(reg);          // specify register
  MS_DELAY();
  Max7219_PutByte(data);         // put data
  MS_DELAY();
  WRITE(MAX7219_LOAD_PIN, LOW);  // and tell the chip to load the data
  MS_DELAY();
  WRITE(MAX7219_LOAD_PIN, HIGH);
  CRITICAL_SECTION_END
  MS_DELAY();
}

void Max7219_LED_Set(const uint8_t col, const uint8_t row, const bool on) {
  if (row > 7 || col > 7) {
    SERIAL_ECHOPAIR("??? Max7219_LED_Set(", (int)row);
    SERIAL_ECHOPAIR(",", (int)col);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  if (TEST(LEDs[row], col) == on) return; // if LED is already on/off, leave alone
  if (on) SBI(LEDs[row], col); else CBI(LEDs[row], col);
  Max7219(8 - row, LEDs[row]);
}

void Max7219_LED_On(const uint8_t col, const uint8_t row) {
  if (row > 7 || col > 7) {
    SERIAL_ECHOPAIR("??? Max7219_LED_On(", (int)col);
    SERIAL_ECHOPAIR(",", (int)row);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  Max7219_LED_Set(col, row, true);
}

void Max7219_LED_Off(const uint8_t col, const uint8_t row) {
  if (row > 7 || col > 7) {
    SERIAL_ECHOPAIR("??? Max7219_LED_Off(", (int)row);
    SERIAL_ECHOPAIR(",", (int)col);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  Max7219_LED_Set(col, row, false);
}

void Max7219_LED_Toggle(const uint8_t col, const uint8_t row) {
  if (row > 7 || col > 7) {
    SERIAL_ECHOPAIR("??? Max7219_LED_Toggle(", (int)row);
    SERIAL_ECHOPAIR(",", (int)col);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  if (TEST(LEDs[row], col))
    Max7219_LED_Off(col, row);
  else
    Max7219_LED_On(col, row);
}

void Max7219_Clear_Column(const uint8_t col) {
  if (col > 7) {
    SERIAL_ECHOPAIR("??? Max7219_Clear_Column(", (int)col);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  LEDs[col] = 0;
  Max7219(8 - col, LEDs[col]);
}

void Max7219_Clear_Row(const uint8_t row) {
  if (row > 7) {
    SERIAL_ECHOPAIR("??? Max7219_Clear_Row(", (int)row);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  for (uint8_t c = 0; c <= 7; c++)
    Max7219_LED_Off(c, row);
}

void Max7219_Set_Row(const uint8_t row, const uint8_t val) {
  if (row > 7) {
    SERIAL_ECHOPAIR("??? Max7219_Set_Row(", (int)row);
    SERIAL_ECHOPAIR(",", (int)val);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  for (uint8_t b = 0; b <= 7; b++)
    if (TEST(val, b))
      Max7219_LED_On(7 - b, row);
    else
      Max7219_LED_Off(7 - b, row);
}

void Max7219_Set_2_Rows(const uint8_t row, const uint16_t val) {
  if (row > 6) {
    SERIAL_ECHOPAIR("??? Max7219_Set_2_Rows(", (int)row);
    SERIAL_ECHOPAIR(",", (int)val);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  Max7219_Set_Row(row + 1, (val >> 8) & 0xFF);
  Max7219_Set_Row(row + 0, (val     ) & 0xFF);
}

void Max7219_Set_4_Rows(const uint8_t row, const uint32_t val) {
  if (row > 4) {
    SERIAL_ECHOPAIR("??? Max7219_Set_4_Rows(", (int)row);
    SERIAL_ECHOPAIR(",", (long)val);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  Max7219_Set_Row(row + 3, (val >> 24) & 0xFF);
  Max7219_Set_Row(row + 2, (val >> 16) & 0xFF);
  Max7219_Set_Row(row + 1, (val >>  8) & 0xFF);
  Max7219_Set_Row(row + 0, (val      ) & 0xFF);
}

void Max7219_Set_Column(const uint8_t col, const uint8_t val) {
  if (col > 7) {
    SERIAL_ECHOPAIR("??? Max7219_Column(", (int)col);
    SERIAL_ECHOPAIR(",", (int)val);
    SERIAL_ECHOLNPGM(")");
    return;
  }
  LEDs[col] = val;
  Max7219(8 - col, LEDs[col]);
}

void Max7219_init() {
  uint8_t i, x, y;

  SET_OUTPUT(MAX7219_DIN_PIN);
  SET_OUTPUT(MAX7219_CLK_PIN);

  OUT_WRITE(MAX7219_LOAD_PIN, HIGH);
  delay(1);

  //initiation of the max 7219
  Max7219(max7219_reg_scanLimit, 0x07);
  Max7219(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  Max7219(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  Max7219(max7219_reg_displayTest, 0x00); // no display test
  Max7219(max7219_reg_intensity, 0x01 & 0x0F); // the first 0x0F is the value you can set
                                               // range: 0x00 to 0x0F
  for (i = 0; i <= 7; i++) {      // empty registers, turn all LEDs off
    LEDs[i] = 0x00;
    Max7219(i + 1, 0);
  }

  for (x = 0; x <= 7; x++)        // Do an aesthetically pleasing pattern to fully test
    for (y = 0; y <= 7; y++) {    // the Max7219 module and LEDs. First, turn them
      Max7219_LED_On(y, x);       // all on.
      delay(3);
    }

  for (x = 0; x <= 7; x++)        // Now, turn them all off.
    for (y = 0; y <= 7; y++) {
      Max7219_LED_Off(y, x);
      delay(3);                   // delay() is OK here. Max7219_init() is only called from
    }                             // setup() and nothing is running yet.

  delay(150);

  for (x = 8; x--;)               // Now, do the same thing from the opposite direction
    for (y = 0; y <= 7; y++) {
      Max7219_LED_On(y, x);
      delay(2);
    }

  for (x = 8; x--;)
    for (y = 0; y <= 7; y++) {
      Max7219_LED_Off(y, x);
      delay(2);
    }
}

/**
 * These are sample debug features to demonstrate the usage of the 8x8 LED Matrix for debug purposes.
 * There is very little CPU burden added to the system by displaying information within the idle()
 * task.
 *
 * But with that said, if your debugging can be facilitated by making calls into the library from
 * other places in the code, feel free to do it.  The CPU burden for a few calls to toggle an LED
 * or clear a row is not very significant.
 */
void Max7219_idle_tasks() {
  #if MAX7219_DEBUG_STEPPER_HEAD || MAX7219_DEBUG_STEPPER_TAIL || MAX7219_DEBUG_STEPPER_QUEUE
    CRITICAL_SECTION_START
    #if MAX7219_DEBUG_STEPPER_HEAD || MAX7219_DEBUG_STEPPER_QUEUE
      const uint8_t head = planner.block_buffer_head;
    #endif
    #if MAX7219_DEBUG_STEPPER_TAIL || MAX7219_DEBUG_STEPPER_QUEUE
      const uint8_t tail = planner.block_buffer_tail;
    #endif
    CRITICAL_SECTION_END
  #endif

  #if ENABLED(MAX7219_DEBUG_PRINTER_ALIVE)
    static millis_t next_blink = 0;
    if (ELAPSED(millis(), next_blink)) {
      Max7219_LED_Toggle(7, 7);
      next_blink = millis() + 750;
    }
  #endif

  #ifdef MAX7219_DEBUG_STEPPER_HEAD
    static int16_t last_head_cnt = 0;
    if (last_head_cnt != head) {
      if (last_head_cnt < 8)
        Max7219_LED_Off(MAX7219_DEBUG_STEPPER_HEAD, last_head_cnt);
      else
        Max7219_LED_Off(MAX7219_DEBUG_STEPPER_HEAD + 1, last_head_cnt - 8);

      last_head_cnt = head;
      if (head < 8)
        Max7219_LED_On(MAX7219_DEBUG_STEPPER_HEAD, head);
      else
        Max7219_LED_On(MAX7219_DEBUG_STEPPER_HEAD + 1, head - 8);
    }
  #endif

  #ifdef MAX7219_DEBUG_STEPPER_TAIL
    static int16_t last_tail_cnt = 0;
    if (last_tail_cnt != tail) {
      if (last_tail_cnt < 8)
        Max7219_LED_Off(MAX7219_DEBUG_STEPPER_TAIL, last_tail_cnt);
      else
        Max7219_LED_Off(MAX7219_DEBUG_STEPPER_TAIL + 1, last_tail_cnt - 8);

      last_tail_cnt = tail;
      if (tail < 8)
        Max7219_LED_On(MAX7219_DEBUG_STEPPER_TAIL, tail);
      else
        Max7219_LED_On(MAX7219_DEBUG_STEPPER_TAIL + 1, tail - 8);
    }
  #endif

  #ifdef MAX7219_DEBUG_STEPPER_QUEUE
    static int16_t last_depth = 0;
    int16_t current_depth = head - tail;
    if (current_depth != last_depth) {  // usually, no update will be needed.
      if (current_depth < 0) current_depth += BLOCK_BUFFER_SIZE;
      NOMORE(current_depth, BLOCK_BUFFER_SIZE);
      NOMORE(current_depth, 16);        // if the BLOCK_BUFFER_SIZE is greater than 16, two lines
                                        // of LEDs is enough to see if the buffer is draining

      const uint8_t st = min(current_depth, last_depth),
                    en = max(current_depth, last_depth);
      if (current_depth < last_depth)
        for (uint8_t i = st; i <= en; i++)   // clear the highest order LEDs
          Max7219_LED_Off(MAX7219_DEBUG_STEPPER_QUEUE + (i & 1), i / 2);
      else
        for (uint8_t i = st; i <= en; i++)   // set the LEDs to current depth
          Max7219_LED_On(MAX7219_DEBUG_STEPPER_QUEUE + (i & 1), i / 2);

      last_depth = current_depth;
    }
  #endif
}

#endif // MAX7219_DEBUG
