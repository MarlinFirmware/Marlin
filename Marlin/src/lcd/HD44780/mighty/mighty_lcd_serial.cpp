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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(MIGHTYBOARD_LCD)

#include "mighty_lcd_serial.h"
#include "../../../core/serial.h"
#include <util/delay.h>

#include "../../../inc/MarlinConfig.h"

/**
 * Constructor: store pin assignments
 */
MightyboardLCDSerial::MightyboardLCDSerial()
  : _displayfunction(LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS),
    _displaycontrol(LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF),
    _displaymode(LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT),
    _xcursor(0), _ycursor(0) {}

/**
 * Initialize LCD display with 4-bit mode
 * Follows HD44780 initialization sequence from datasheet
 */
void MightyboardLCDSerial::begin(uint8_t cols, uint8_t rows, uint8_t charsize) {
  SET_OUTPUT(SR_STROBE_PIN);
  SET_OUTPUT(SR_DATA_PIN);
  SET_OUTPUT(SR_CLK_PIN);

  // Power on LCD if power pin is defined
  // Already handled in MarlinUI::init_lcd()
  //#if PIN_EXISTS(LCD_PWR)
  //  OUT_WRITE(LCD_PWR_PIN, LOW); // Power on (active low)
  //#endif

  _cols = cols;
  _rows = rows;

  // Diagnostic output showing LCD configuration
  #if ENABLED(MIGHTYBOARD_DEBUG)
    SERIAL_ECHOLNPGM("MightyboardLCDSerial::begin() - LCD Configuration:");
    SERIAL_ECHOLNPGM("  Dimensions: ", _cols, " x ", _rows);
    #ifdef LCD_WIDTH
      SERIAL_ECHOLNPGM("  Expected from config: ", LCD_WIDTH, " x ", LCD_HEIGHT);
    #endif
    SERIAL_ECHOLN(F("  Charsize: "),
      charsize == LCD_5x8DOTS ? F("5x8 dots") : charsize == LCD_5x10DOTS ? F("5x10 dots") : F("unknown")
    );
  #endif

  if (rows > 1)
    _displayfunction |= LCD_2LINE;
  else if (rows == 1 && charsize != LCD_5x8DOTS)
    _displayfunction |= LCD_5x10DOTS;

  // Power-up delay as per HD44780 datasheet (> 40ms required)
  delay(50);

  // Initialization sequence for 4-bit mode
  // This is a simplified sequence that works reliably
  sendCommand(0x33); // Initialize, 8-bit mode attempt 1
  sendCommand(0x32); // Set 4-bit mode
  sendCommand(0x28); // Function Set: 4-bit, 2-line, 5x8 dots
  sendCommand(0x0C); // Display ON, cursor OFF, blink OFF
  sendCommand(0x06); // Entry Mode Set: increment, no shift
  sendCommand(0x01); // Clear display
  delay(5);
}

/**
 * Clear the display and return cursor to home
 */
void MightyboardLCDSerial::clear() {
  sendCommand(LCD_CLEARDISPLAY);
  delay(2); // Clear takes ~1.52ms
}

/**
 * Return cursor to home (0, 0)
 */
void MightyboardLCDSerial::home() {
  sendCommand(LCD_RETURNHOME);
  delay(2);
  _xcursor = 0;
  _ycursor = 0;
}

/**
 * Turn display off (keeps data in DDRAM)
 */
void MightyboardLCDSerial::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * Turn display on
 */
void MightyboardLCDSerial::display() {
  _displaycontrol |= LCD_DISPLAYON;
  sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * Turn off the blinking cursor
 */
void MightyboardLCDSerial::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * Turn on the blinking cursor
 */
void MightyboardLCDSerial::blink() {
  _displaycontrol |= LCD_BLINKON;
  sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * Turn off the underline cursor
 */
void MightyboardLCDSerial::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * Turn on the underline cursor
 */
void MightyboardLCDSerial::cursor() {
  _displaycontrol |= LCD_CURSORON;
  sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/**
 * Scroll display contents left (cursor follows)
 */
void MightyboardLCDSerial::scrollDisplayLeft() {
  sendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

/**
 * Scroll display contents right (cursor follows)
 */
void MightyboardLCDSerial::scrollDisplayRight() {
  sendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/**
 * Set text direction left-to-right
 */
void MightyboardLCDSerial::leftToRight() {
  _displaymode |= LCD_ENTRYLEFT;
  sendCommand(LCD_ENTRYMODESET | _displaymode);
}

/**
 * Set text direction right-to-left
 */
void MightyboardLCDSerial::rightToLeft() {
  _displaymode &= ~LCD_ENTRYLEFT;
  sendCommand(LCD_ENTRYMODESET | _displaymode);
}

/**
 * Enable autoscroll (characters printed shift display left)
 */
void MightyboardLCDSerial::autoscroll() {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  sendCommand(LCD_ENTRYMODESET | _displaymode);
}

/**
 * Disable autoscroll
 */
void MightyboardLCDSerial::noAutoscroll() {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  sendCommand(LCD_ENTRYMODESET | _displaymode);
}

/**
 * Load a custom character into CGRAM
 * @param location CGRAM address (0-7)
 * @param charmap 8-byte array defining the character
 */
void MightyboardLCDSerial::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // Only 8 locations available
  sendCommand(LCD_SETCGRAMADDR | (location << 3));
  for (int8_t i = 0; i < 8; i++) sendData(charmap[i]);
}

/**
 * Set cursor position
 * @param col Column (0 to cols-1)
 * @param row Row (0 to rows-1)
 */
void MightyboardLCDSerial::setCursor(uint8_t col, uint8_t row) {
  static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  if (row >= _rows) row = _rows - 1;
  _xcursor = col;
  _ycursor = row;
  sendCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

/**
 * Write a character to the LCD at current cursor position
 * Automatically advances cursor and wraps to next line if needed
 */
size_t MightyboardLCDSerial::write(uint8_t value) {
  sendData(value);
  _xcursor++;
  if (_xcursor >= _cols) {
    _xcursor = 0;
    _ycursor++;
    if (_ycursor >= _rows) _ycursor = 0;
    setCursor(_xcursor, _ycursor);
  }
  return 1;
}

/**
 * Send a raw command byte to LCD
 */
void MightyboardLCDSerial::command(uint8_t value) {
  send(value, false);
}

/**
 * Low-level: send 8-bit value as two 4-bit nibbles
 * @param value Byte to send
 * @param mode false = command, true = data
 */
void MightyboardLCDSerial::send(uint8_t value, bool mode) {
  write4bits(value >> 4, mode);
  write4bits(value & 0x0F, mode);
}

/**
 * Send command (RS = 0)
 */
inline void MightyboardLCDSerial::sendCommand(uint8_t value) {
  send(value, false);
}

/**
 * Send data (RS = 1)
 */
inline void MightyboardLCDSerial::sendData(uint8_t value) {
  send(value, true);
}

/**
 * Write 4-bit nibble to display
 * @param value Lower 4 bits are used
 * @param dataMode false = command (RS=0), true = data (RS=1)
 */
void MightyboardLCDSerial::write4bits(uint8_t value, bool dataMode) {
  // Shift data to upper 4 bits (D7-D4 in HD44780)
  uint8_t bits = value << 4;

  // Set RS bit (bit 1) based on mode
  // On Mightyboard: RS = 0b0010
  if (dataMode) bits |= 0b0010;  // RS = 1 for data

  // Pulse enable to latch the nibble
  pulseEnable(bits);
}

/**
 * Pulse the Enable line to latch data
 * On Mightyboard hardware: Enable = bit 3 (0b01000)
 * @param value Pre-set data bits (D7-D0 in shift register)
 */
void MightyboardLCDSerial::pulseEnable(uint8_t value) {
  _delay_us(1);
  SBI(value, 3);        // Set enable HIGH
  writeSerial(value);
  _delay_us(1);
  CBI(value, 3);        // Set enable LOW
  writeSerial(value);
  _delay_us(50);  // Wait for command execution
}

/**
 * Shift out 8 bits serially via shift register
 * Sends MSB first (bit 7 down to bit 0)
 * @param value Byte to shift out
 */
void MightyboardLCDSerial::writeSerial(uint8_t value) {
  // Shift out each bit, MSB first
  for (int8_t i = 7; i >= 0; i--) {
    WRITE(SR_CLK_PIN, LOW);
    bool data = (value >> i) & 0x01;
    WRITE(SR_DATA_PIN, data);
    WRITE(SR_CLK_PIN, HIGH);
    _delay_us(1);  // Clock pulse width
  }

  // Pulse strobe to latch data into display shift register
  WRITE(SR_STROBE_PIN, HIGH);
  _delay_us(1);
  WRITE(SR_STROBE_PIN, LOW);
}

#endif // MIGHTYBOARD_LCD
