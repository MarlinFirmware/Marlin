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
 * This module is normally not enabled.  It can be enabled to facilitate
 * the display of extra debug information during code development.
 * It assumes the existance of a Max7219 LED Matrix.   A suitable
 * device can be obtained on eBay similar to this: http://www.ebay.com/itm/191781645249
 * for under $2.00 including shipping.
 *
 * Just connect up +5v and Gnd to give it power.  And then 3 wires declared in the 
 * #define's below.  Actual pin assignments can be changed in MAX7219_DEBUG section 
 * of configuration_adv.h
 *
 *   #define Max7219_clock   77
 *   #define Max7219_data_in 78
 *   #define Max7219_load    79
 *
 * First call Max7219_init() and then there are a number of support functions available
 * to control the LED's in the 8x8 grid.
 *
 * void Max7219_init();
 * void Max7219_PutByte(uint8_t data);
 * void Max7219(uint8_t reg, uint8_t data);
 * void Max7219_LED_On( int8_t row, int8_t col);
 * void Max7219_LED_Off( int8_t row, int8_t col);
 * void Max7219_LED_Toggle( int8_t row, int8_t col);
 * void Max7219_Clear_Row( int8_t row);
 * void Max7219_Clear_Column( int8_t col);
 */


#include "Marlin.h"

#if ENABLED(MAX7219_DEBUG)
  #include "planner.h"
  #include "stepper.h"
  #include "Max7219_Debug_LEDs.h"

  static uint8_t LEDs[8] = {0};

  void Max7219_PutByte(uint8_t data) {
    uint8_t i = 8;
      while(i > 0) {
        digitalWrite( Max7219_clock, LOW);    // tick
        if (data & 0x80)                      // check bit
          digitalWrite(Max7219_data_in,HIGH); // send 1
        else
          digitalWrite(Max7219_data_in,LOW);  // send 0
        digitalWrite(Max7219_clock, HIGH);    // tock
        data = data << 0x01;
        --i;                                  // move to lesser bit
      }
    }

  void Max7219( uint8_t reg, uint8_t data) {
    digitalWrite(Max7219_load, LOW);  // begin
    Max7219_PutByte(reg);             // specify register
    Max7219_PutByte(data);            // put data
    digitalWrite(Max7219_load, LOW);  // and tell the chip to load the data
    digitalWrite(Max7219_load,HIGH);
  }

  void Max7219_LED_On( int8_t row, int8_t col) {
    int x_index;
    if ( row>=8 || row<0 || col>=8 || col<0)
      return;
    if ( LEDs[row] & (0x01<<col) ) // if LED is already on, just leave
      return;

    LEDs[row] |= (0x01<<col);
    x_index = 7-row;
    Max7219( x_index+1, LEDs[row] );
  }

  void Max7219_LED_Off( int8_t row, int8_t col) {
    int x_index;
    if ( row>=8 || row<0 || col>=8 || col<0)
      return;
    if ( !(LEDs[row] & (0x01<<col)) ) // if LED is already off, just leave
      return;

    LEDs[row] ^= (0x01<<col);
    x_index = 7-row;
    Max7219( x_index+1, LEDs[row] );
  }

  void Max7219_LED_Toggle( int8_t row, int8_t col) {
    if ( row>=8 || row<0 || col>=8 || col<0)
      return;
    if ( (LEDs[row] & (0x01<<col)) )
      Max7219_LED_Off( row, col);
    else
      Max7219_LED_On( row, col);
  }

  void Max7219_Clear_Column( int8_t col) {
    int x_index;
    if ( col>=8 || col<0 )
      return;
    LEDs[col] = 0;
    x_index = 7-col;
    Max7219( x_index+1, LEDs[col] );
  }

  void Max7219_Clear_Row( int8_t row) {
    int c;
    if ( row>=8 || row<0 )
      return;

    for(c=0; c<8; c++)
      Max7219_LED_Off( c, row);
  }

  void Max7219_Set_Row( int8_t row, uint8_t val) {
    int b;

    if ( row<0 || row>7 )
      return;

    if ( val<0 || val>255 )
      return;

    for(b=0; b<8; b++)
      if ( val & (0x01 << b) )
        Max7219_LED_On( 7-b, row);
      else
        Max7219_LED_Off( 7-b, row);
  }

  void Max7219_Set_Column( int8_t col, uint8_t val) {
    int x_index;

    if ( col>=8 || col<0 )
      return;

    if ( val<0 || val>255 )
      return;

    LEDs[col] = val;
    x_index = 7-col;
    Max7219( x_index+1, LEDs[col] );
  }


  void Max7219_init() {
    int i, x, y;

    pinMode(Max7219_data_in, OUTPUT);
    pinMode(Max7219_clock,   OUTPUT);
    pinMode(Max7219_load,    OUTPUT);

    digitalWrite(Max7219_load, HIGH);

    //initiation of the max 7219
    Max7219(max7219_reg_scanLimit, 0x07);
    Max7219(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    Max7219(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    Max7219(max7219_reg_displayTest, 0x00); // no display test
    Max7219(max7219_reg_intensity, 0x01 & 0x0f); // the first 0x0f is the value you can set
                                                 // range: 0x00 to 0x0f
    for (i=0; i<8; i++) {       // empty registers, turn all LEDs off
      LEDs[i] = 0x00;
      Max7219(i+1,0);
    }

    for(x=0; x<8; x++)  {       // Do an austetically pleasing pattern to fully test
      for(y=0; y<8; y++) {      // the Max7219 module and LED's.   First, turn them
        Max7219_LED_On( x, y);  // all on.
        delay(3);
      }
    }
    for(x=0; x<8; x++)  {       // Now, turn them all off.
      for(y=0; y<8; y++) {
        Max7219_LED_Off( x, y);
        delay(3);               // delay() is OK here.  Max7219_init() is only called from
      }                         // setup() and nothing is running yet.
    }

    delay(150);

    for(x=7; x>=0; x--)  {     // Now, do the same thing from the opposite direction
      for(y=0; y<8; y++) {
        Max7219_LED_On( x, y);
        delay(2);
      }
    }

    for(x=7; x>=0; x--)  {
      for(y=0; y<8; y++) {
        Max7219_LED_Off( x, y);
        delay(2);
      }
      }
    }

/*
 * These are sample debug features to demonstrate the usage of the 8x8 LED Matrix for debug purposes.
 * There is very little CPU burden added to the system by displaying information within the idle()
 * task.
 *
 * But with that said, if your debugging can be facilitated by making calls into the library from
 * other places in the code, feel free to do it.  The CPU burden for a few calls to toggle an LED
 * or clear a row is not very significant.
 */
  void Max7219_idle_tasks() {
    #ifdef MAX7219_DEBUG_PRINTER_ALIVE
      static int debug_cnt=0;
      if (debug_cnt++ > 100) {
        Max7219_LED_Toggle(7,7);
        debug_cnt = 0;
      }
    #endif

    #ifdef MAX7219_DEBUG_STEPPER_HEAD
      Max7219_Clear_Row(MAX7219_DEBUG_STEPPER_HEAD);
      Max7219_Clear_Row(MAX7219_DEBUG_STEPPER_HEAD+1);
      if ( planner.block_buffer_head < 8)
        Max7219_LED_On( planner.block_buffer_head, MAX7219_DEBUG_STEPPER_HEAD);
      else
        Max7219_LED_On( planner.block_buffer_head-8, MAX7219_DEBUG_STEPPER_HEAD+1);
    #endif

    #ifdef MAX7219_DEBUG_STEPPER_TAIL
      Max7219_Clear_Row(MAX7219_DEBUG_STEPPER_TAIL);
      Max7219_Clear_Row(MAX7219_DEBUG_STEPPER_TAIL+1);
      if ( planner.block_buffer_tail < 8)
        Max7219_LED_On( planner.block_buffer_tail, MAX7219_DEBUG_STEPPER_TAIL );
      else
        Max7219_LED_On( planner.block_buffer_tail-8, MAX7219_DEBUG_STEPPER_TAIL+1 );
    #endif

    #ifdef MAX7219_DEBUG_STEPPER_QUEUE
      static int16_t last_depth=0, current_depth;
      uint8_t i;
      current_depth = planner.block_buffer_head - planner.block_buffer_tail;
      if (current_depth != last_depth) {  // usually, no update will be needed.

        if ( current_depth < 0 )
          current_depth += BLOCK_BUFFER_SIZE;

        if ( current_depth >= BLOCK_BUFFER_SIZE )
          current_depth = BLOCK_BUFFER_SIZE;

        if ( current_depth > 16 )         // if the BLOCK_BUFFER_SIZE is greater than 16 two lines
          current_depth = 16;             // of LED's is enough to see if the buffer is draining

        if ( current_depth < last_depth )
          for(i=current_depth; i<=last_depth; i++) {   // clear the highest order LED's
            if ( i & 1)
              Max7219_LED_Off(i>>1, MAX7219_DEBUG_STEPPER_QUEUE+1);
            else
              Max7219_LED_Off(i>>1, MAX7219_DEBUG_STEPPER_QUEUE+0);
          }
        else
          for(i=last_depth; i<=current_depth; i++) {   // light up the highest order LED's
            if ( i & 1)
              Max7219_LED_On(i>>1, MAX7219_DEBUG_STEPPER_QUEUE+1);
            else
              Max7219_LED_On(i>>1, MAX7219_DEBUG_STEPPER_QUEUE+0);
          }
        last_depth = current_depth;
      }
    #endif
  }
#endif //MAX7219_DEBUG

