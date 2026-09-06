/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * MightyboardLCDSerial
 * Optimized 3-wire shift-register LCD driver for Mightyboard OEM displays
 * Provides HD44780 compatible interface with minimal CPU overhead
 */

#include <Print.h>
#include <stdio.h>
#include <stdint.h>

class MightyboardLCDSerial : public Print {
public:
  /**
   * Constructor: initialize pin assignments
   * @param strobe Strobe/latch pin (LATCH_PIN / SR_STROBE_PIN)
   * @param data Data pin (MOSI_PIN / SR_DATA_PIN)
   * @param clk Clock pin (SCLK_PIN / SR_CLK_PIN)
   * @param pwrPin Power control pin (LCD_PWR_PIN)
   */
  MightyboardLCDSerial();

  /**
   * Initialize display with given dimensions
   * @param cols Number of columns (typically 20)
   * @param rows Number of rows (typically 4)
   */
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize=LCD_5x8DOTS);

  // Basic display control
  void clear();
  void home();
  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();

  // Cursor movement
  void setCursor(uint8_t col, uint8_t row);
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  // Custom characters
  void createChar(uint8_t location, uint8_t charmap[]);

  // Text output
  virtual size_t write(uint8_t value);
  using Print::write;

  // Low-level command
  void command(uint8_t value);

private:
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _xcursor;
  uint8_t _ycursor;

  // Low-level communication
  void send(uint8_t value, bool mode);
  void sendCommand(uint8_t value);
  void sendData(uint8_t value);
  void write4bits(uint8_t value, bool dataMode);
  void pulseEnable(uint8_t value);
  void writeSerial(uint8_t value);

  // HD44780 command constants
  static const uint8_t LCD_CLEARDISPLAY = 0x01;
  static const uint8_t LCD_RETURNHOME = 0x02;
  static const uint8_t LCD_ENTRYMODESET = 0x04;
  static const uint8_t LCD_DISPLAYCONTROL = 0x08;
  static const uint8_t LCD_CURSORSHIFT = 0x10;
  static const uint8_t LCD_FUNCTIONSET = 0x20;
  static const uint8_t LCD_SETCGRAMADDR = 0x40;
  static const uint8_t LCD_SETDDRAMADDR = 0x80;

  // Entry mode flags
  static const uint8_t LCD_ENTRYRIGHT = 0x00;
  static const uint8_t LCD_ENTRYLEFT = 0x02;
  static const uint8_t LCD_ENTRYSHIFTINCREMENT = 0x01;
  static const uint8_t LCD_ENTRYSHIFTDECREMENT = 0x00;

  // Display control flags
  static const uint8_t LCD_DISPLAYON = 0x04;
  static const uint8_t LCD_DISPLAYOFF = 0x00;
  static const uint8_t LCD_CURSORON = 0x02;
  static const uint8_t LCD_CURSOROFF = 0x00;
  static const uint8_t LCD_BLINKON = 0x01;
  static const uint8_t LCD_BLINKOFF = 0x00;

  // Display shift flags
  static const uint8_t LCD_DISPLAYMOVE = 0x08;
  static const uint8_t LCD_CURSORMOVE = 0x00;
  static const uint8_t LCD_MOVERIGHT = 0x04;
  static const uint8_t LCD_MOVELEFT = 0x00;

  // Function set flags
  static const uint8_t LCD_8BITMODE = 0x10;
  static const uint8_t LCD_4BITMODE = 0x00;
  static const uint8_t LCD_2LINE = 0x08;
  static const uint8_t LCD_1LINE = 0x00;
  static const uint8_t LCD_5x10DOTS = 0x04;
  static const uint8_t LCD_5x8DOTS = 0x00;
};

// Alias for compatibility with Marlin's LCD_CLASS macro
using MightyboardLCD = MightyboardLCDSerial;
