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
 * This module is normally not enabled and does not generate any code.   But it
 * can be enabled to facilitate the display of extra debug information during
 * code development.   It assumes the existance of a Max7219 LED Matrix.   You
 * can get one on eBay similar to this: http://www.ebay.com/itm/191781645249
 * for under $2.00 including shipping.
 *
 * Just connect up +5v and Gnd to give it power.  And then 3 wires declared in the 
 * #define's below.  Actual pin assignments can be changed in MAX7219_DEBUG section 
 * of configuration_adv.h
 *
 * You first call Max7219_init() and then you have 3 support functions available
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
 * void Max7219_Set_Row( int8_t row, int8_t val);
 * void Max7219_Set_Column( int8_t column, int8_t val);
 * void Max7219_idle_tasks();
 */


#if ENABLED(MAX7219_DEBUG)
  //
  // define max7219 registers
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

  #define max7219_reg_intensity   0x0a
  #define max7219_reg_displayTest 0x0f
  #define max7219_reg_decodeMode  0x09
  #define max7219_reg_scanLimit   0x0b
  #define max7219_reg_shutdown    0x0c


  void Max7219_init();
  void Max7219_PutByte(uint8_t data);
  void Max7219(uint8_t reg, uint8_t data);
  void Max7219_LED_On( int8_t row, int8_t col);
  void Max7219_LED_Off( int8_t row, int8_t col);
  void Max7219_LED_Toggle( int8_t row, int8_t col);
  void Max7219_Clear_Row( int8_t row);
  void Max7219_Clear_Column( int8_t col);
  void Max7219_Set_Row( int8_t row, uint8_t val);
  void Max7219_Set_Column( int8_t col, uint8_t val);
  void Max7219_idle_tasks();
#endif


