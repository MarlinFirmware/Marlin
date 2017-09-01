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
 * void Max7219_LED_On(uint8_t row, uint8_t col);
 * void Max7219_LED_Off(uint8_t row, uint8_t col);
 * void Max7219_LED_Toggle(uint8_t row, uint8_t col);
 * void Max7219_Clear_Row(uint8_t row);
 * void Max7219_Clear_Column(uint8_t col);
 * void Max7219_Set_Row(uint8_t row, uint8_t val);
 * void Max7219_Set_Column(uint8_t col, uint8_t val);
 * void Max7219_idle_tasks();
 */

#include "MarlinConfig.h"

#if ENABLED(MAX7219_DEBUG)

  #include "Marlin.h"
  #include "planner.h"
  #include "stepper.h"
  #include "Max7219_Debug_LEDs.h"

  static uint8_t LEDs[8] = { 0 };

  void Max7219_PutByte(uint8_t data) {
    for (uint8_t i = 8; i--;) {
      WRITE(MAX7219_CLK_PIN, LOW);       // tick
      WRITE(MAX7219_DIN_PIN, (data & 0x80) ? HIGH : LOW);  // send 1 or 0 based on data bit
      WRITE(MAX7219_CLK_PIN, HIGH);      // tock
      data <<= 1;
    }
  }

  void Max7219(const uint8_t reg, const uint8_t data) {
    WRITE(MAX7219_LOAD_PIN, LOW);  // begin
    Max7219_PutByte(reg);          // specify register
    Max7219_PutByte(data);         // put data
    WRITE(MAX7219_LOAD_PIN, LOW);  // and tell the chip to load the data
    WRITE(MAX7219_LOAD_PIN, HIGH);
  }

  void Max7219_LED_Set(const uint8_t row, const uint8_t col, const bool on) {
    if (row > 7 || col > 7) return;
    if (TEST(LEDs[row], col) == on) return; // if LED is already on/off, leave alone
    if (on) SBI(LEDs[row], col); else CBI(LEDs[row], col);
    Max7219(8 - row, LEDs[row]);
  }

  void Max7219_LED_On(const uint8_t row, const uint8_t col) {
    Max7219_LED_Set(row, col, true);
  }

  void Max7219_LED_Off(const uint8_t row, const uint8_t col) {
    Max7219_LED_Set(row, col, false);
  }

  void Max7219_LED_Toggle(const uint8_t row, const uint8_t col) {
    if (row > 7 || col > 7) return;
    if (TEST(LEDs[row], col))
      Max7219_LED_Off(row, col);
    else
      Max7219_LED_On(row, col);
  }

  void Max7219_Clear_Column(const uint8_t col) {
    if (col > 7) return;
    LEDs[col] = 0;
    Max7219(8 - col, LEDs[col]);
  }

  void Max7219_Clear_Row(const uint8_t row) {
    if (row > 7) return;
    for (uint8_t c = 0; c <= 7; c++)
      Max7219_LED_Off(c, row);
  }

  void Max7219_Set_Row(const uint8_t row, const uint8_t val) {
    if (row > 7) return;
    for (uint8_t b = 0; b <= 7; b++)
      if (TEST(val, b))
        Max7219_LED_On(7 - b, row);
      else
        Max7219_LED_Off(7 - b, row);
  }

  void Max7219_Set_Column(const uint8_t col, const uint8_t val) {
    if (col > 7) return;
    LEDs[col] = val;
    Max7219(8 - col, LEDs[col]);
  }

  void Max7219_init() {
    uint8_t i, x, y;

    SET_OUTPUT(MAX7219_DIN_PIN);
    SET_OUTPUT(MAX7219_CLK_PIN);

    OUT_WRITE(MAX7219_LOAD_PIN, HIGH);

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
        Max7219_LED_On(x, y);       // all on.
        delay(3);
      }

    for (x = 0; x <= 7; x++)        // Now, turn them all off.
      for (y = 0; y <= 7; y++) {
        Max7219_LED_Off(x, y);
        delay(3);                   // delay() is OK here. Max7219_init() is only called from
      }                             // setup() and nothing is running yet.

    delay(150);

    for (x = 8; x--;)               // Now, do the same thing from the opposite direction
      for (y = 0; y <= 7; y++) {
        Max7219_LED_On(x, y);
        delay(2);
      }

    for (x = 8; x--;)
      for (y = 0; y <= 7; y++) {
        Max7219_LED_Off(x, y);
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
    #if ENABLED(MAX7219_DEBUG_PRINTER_ALIVE)
      static int debug_cnt = 0;
      if (debug_cnt++ > 100) {
        Max7219_LED_Toggle(7, 7);
        debug_cnt = 0;
      }
    #endif

    #ifdef MAX7219_DEBUG_STEPPER_HEAD
      Max7219_Clear_Row(MAX7219_DEBUG_STEPPER_HEAD);
      Max7219_Clear_Row(MAX7219_DEBUG_STEPPER_HEAD + 1);
      if ( planner.block_buffer_head < 8)
        Max7219_LED_On( planner.block_buffer_head, MAX7219_DEBUG_STEPPER_HEAD);
      else
        Max7219_LED_On( planner.block_buffer_head-8, MAX7219_DEBUG_STEPPER_HEAD+1);
    #endif

    #ifdef MAX7219_DEBUG_STEPPER_TAIL
      Max7219_Clear_Row(MAX7219_DEBUG_STEPPER_TAIL);
      Max7219_Clear_Row(MAX7219_DEBUG_STEPPER_TAIL + 1);
      if ( planner.block_buffer_tail < 8)
        Max7219_LED_On( planner.block_buffer_tail, MAX7219_DEBUG_STEPPER_TAIL );
      else
        Max7219_LED_On( planner.block_buffer_tail-8, MAX7219_DEBUG_STEPPER_TAIL+1 );
    #endif

    #ifdef MAX7219_DEBUG_STEPPER_QUEUE
      static int16_t last_depth = 0;
      int16_t current_depth = planner.block_buffer_head - planner.block_buffer_tail;
      if (current_depth != last_depth) {  // usually, no update will be needed.
        if (current_depth < 0) current_depth += BLOCK_BUFFER_SIZE;
        NOMORE(current_depth, BLOCK_BUFFER_SIZE);
        NOMORE(current_depth, 16);        // if the BLOCK_BUFFER_SIZE is greater than 16, two lines
                                          // of LEDs is enough to see if the buffer is draining

        const uint8_t st = min(current_depth, last_depth),
                      en = max(current_depth, last_depth);
        if (current_depth < last_depth)
          for (uint8_t i = st; i <= en; i++)   // clear the highest order LEDs
            Max7219_LED_Off(i >> 1, MAX7219_DEBUG_STEPPER_QUEUE + (i & 1));
        else
          for (uint8_t i = st; i <= en; i++)   // set the highest order LEDs
            Max7219_LED_On(i >> 1, MAX7219_DEBUG_STEPPER_QUEUE + (i & 1));

        last_depth = current_depth;
      }
    #endif
  }

#endif // MAX7219_DEBUG
