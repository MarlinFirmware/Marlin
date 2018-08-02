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
 *
 * If you are using the Max7219 matrix for firmware debug purposes in time sensitive
 * areas of the code, please be aware that the orientation (rotation) of the display can 
 * affect the speed.   The Max7219 can update a single column fairly fast.  It is much
 * faster to do a Max7219_Set_Column() with a rotation of 90 or 270 degrees than to do
 * a Max7219_Set_Row().    The opposite is true for rotations of 0 or 180 degrees.
 */

#ifndef __MAX7219_DEBUG_LEDS_H__
#define __MAX7219_DEBUG_LEDS_H__

//
// MAX7219 registers
//
#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08

#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0A
#define max7219_reg_scanLimit   0x0B
#define max7219_reg_shutdown    0x0C
#define max7219_reg_displayTest 0x0F

void Max7219_init();
void Max7219_PutByte(uint8_t data);
void Max7219_pulse_load();

// Set a single register (e.g., a whole native row)
void Max7219(const uint8_t reg, const uint8_t data);

// Set a single LED by XY coordinate
void Max7219_LED_Set(const uint8_t x, const uint8_t y, const bool on);
void Max7219_LED_On(const uint8_t x, const uint8_t y);
void Max7219_LED_Off(const uint8_t x, const uint8_t y);
void Max7219_LED_Toggle(const uint8_t x, const uint8_t y);

// Set all LEDs in a single column
void Max7219_Set_Column(const uint8_t col, const uint32_t val);
void Max7219_Clear_Column(const uint8_t col);

// Set all LEDs in a single row
void Max7219_Set_Row(const uint8_t row, const uint32_t val);
void Max7219_Clear_Row(const uint8_t row);

// 16 and 32 bit versions of Row and Column functions
// Multiple rows and columns will be used to display the value if
// the array of matrix LED's is too narrow to accomplish the goal
void Max7219_Set_Rows_16bits(const uint8_t y, uint32_t val);
void Max7219_Set_Rows_32bits(const uint8_t y, uint32_t val);
void Max7219_Set_Columns_16bits(const uint8_t x, uint32_t val);
void Max7219_Set_Columns_32bits(const uint8_t x, uint32_t val);

// Quickly clear the whole matrix
void Max7219_Clear();

// Apply custom code to update the matrix
void Max7219_idle_tasks();

#ifndef MAX7219_ROTATE
  #define MAX7219_ROTATE 0
#endif
#define _ROT ((MAX7219_ROTATE + 360) % 360)
#if _ROT == 0
  #define MAX7219_UPDATE_AXIS     y   // Fast line update axis for this orientation of the matrix display
  #define MAX7219_X_LEDS          (8 * MAX7219_NUMBER_UNITS)
  #define MAX7219_Y_LEDS          8
  #define XOR_7219(x, y)          LEDs[y + (x >> 3) * 8] ^= _BV(7 - (x & 0x07))
  #define SET_PIXEL_7219(x, y)    LEDs[y + (x >> 3) * 8] |= _BV(7 - (x & 0x07)) 
  #define CLEAR_PIXEL_7219(x, y)  LEDs[y + (x >> 3) * 8] &= (_BV(7 - (x & 0x07)) ^ 0xff)
  #define BIT_7219(x, y)          TEST(LEDs[y + (x >> 3) * 8], 7 - (x & 0x07))
  #define SEND_7219(R) do{ for (int8_t jj = 0; jj < MAX7219_NUMBER_UNITS; jj++) Max7219(max7219_reg_digit0 + (R & 0x7), LEDs[(R & 0x7) + jj * 8]); Max7219_pulse_load(); }while(0)
#elif _ROT == 90
  #define MAX7219_UPDATE_AXIS     x   // Fast line update axis for this orientation of the matrix display
  #define MAX7219_X_LEDS          8
  #define MAX7219_Y_LEDS          (8 * MAX7219_NUMBER_UNITS)
  #define XOR_7219(x, y)          LEDs[x + (((MAX7219_Y_LEDS - 1 - y) >> 3) * 8)] ^= _BV((y & 0x7))
  #define SET_PIXEL_7219(x, y)    LEDs[x + (((MAX7219_Y_LEDS - 1 - y) >> 3) * 8)] |= _BV((y & 0x7))
  #define CLEAR_PIXEL_7219(x, y)  LEDs[x + (((MAX7219_Y_LEDS - 1 - y) >> 3) * 8)] &= (_BV((y & 0x7)) ^ 0xff)
  #define BIT_7219(x, y)          TEST(LEDs[x + (((MAX7219_Y_LEDS - 1 - y) >> 3) * 8)], (y & 0x7))
  #define SEND_7219(R) do{ for (int8_t jj = 0; jj < MAX7219_NUMBER_UNITS; jj++) Max7219(max7219_reg_digit0 + (R & 0x7), LEDs[(R & 0x7) + jj * 8]); Max7219_pulse_load(); }while(0)
#elif _ROT == 180
  #define MAX7219_UPDATE_AXIS     y   // Fast line update axis for this orientation of the matrix display
  #define MAX7219_X_LEDS          (8 * MAX7219_NUMBER_UNITS)
  #define MAX7219_Y_LEDS          8
  #define XOR_7219(x, y)          LEDs[y + ((MAX7219_X_LEDS - 1 - x) >> 3) * 8] ^= _BV((x & 0x07))
  #define SET_PIXEL_7219(x, y)    LEDs[y + ((MAX7219_X_LEDS - 1 - x) >> 3) * 8] |= _BV((x & 0x07)) 
  #define CLEAR_PIXEL_7219(x, y)  LEDs[y + ((MAX7219_X_LEDS - 1 - x) >> 3) * 8] &= (_BV((x & 0x07)) ^ 0xff)
  #define BIT_7219(x, y)          TEST(LEDs[y + ((MAX7219_X_LEDS - 1 - x) >> 3) * 8], ((x & 0x07)))
  #define SEND_7219(R) do{ for (int8_t jj = 0; jj < MAX7219_NUMBER_UNITS; jj++) Max7219(max7219_reg_digit7 - (R & 0x7), LEDs[(R & 0x7) + jj * 8]); Max7219_pulse_load(); }while(0)
#elif _ROT == 270
  #define MAX7219_UPDATE_AXIS     x   // Fast line update axis for this orientation of the matrix display
  #define MAX7219_X_LEDS          8
  #define MAX7219_Y_LEDS          (8 * MAX7219_NUMBER_UNITS)
  #define XOR_7219(x, y)          LEDs[x + (y >> 3) * 8] ^= _BV(7 - (y & 0x7))
  #define SET_PIXEL_7219(x, y)    LEDs[x + (y >> 3) * 8] |= _BV(7 - (y & 0x7))
  #define CLEAR_PIXEL_7219(x, y)  LEDs[x + (y >> 3) * 8] &= (_BV(7 - (y & 0x7)) ^ 0xff)
  #define BIT_7219(x, y)          TEST(LEDs[x + ( y >> 3) * 8], 7 - (y & 0x7))
  #define SEND_7219(R) do{ for (int8_t jj = 0; jj < MAX7219_NUMBER_UNITS; jj++) Max7219(max7219_reg_digit7 - (R & 0x7), LEDs[(R & 0x7) + jj * 8]); Max7219_pulse_load(); }while(0)
#else
  #error "MAX7219_ROTATE must be a multiple of +/- 90°."
#endif

extern uint8_t LEDs[8 * MAX7219_NUMBER_UNITS];

#endif // __MAX7219_DEBUG_LEDS_H__
