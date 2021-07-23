/****************
 * commands.cpp *
 ****************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "ftdi_basic.h"

#ifdef FTDI_BASIC

#define MULTIPLE_OF_4(val) ((((val)+3)>>2)<<2)

using namespace FTDI;
using namespace FTDI::SPI;

void CLCD::enable() {
  mem_write_8(REG::PCLK, Pclk);
}

void CLCD::disable() {
  mem_write_8(REG::PCLK, 0x00);
}

void CLCD::set_brightness(uint8_t brightness) {
  mem_write_8(REG::PWM_DUTY, min(128,brightness));
}

uint8_t CLCD::get_brightness() {
  return mem_read_8(REG::PWM_DUTY);
}

void CLCD::turn_on_backlight() {
  mem_write_8(REG::PWM_DUTY, 128);
}

void CLCD::FontMetrics::load(const uint8_t font) {
  static_assert(sizeof(FontMetrics) == 148, "Sizeof font metrics is incorrect");
  uint32_t rom_fontroot = mem_read_32(MAP::ROM_FONT_ADDR);
  mem_read_bulk(rom_fontroot + 148 * (font - 16), (uint8_t*) this, 148);
}

uint16_t CLCD::FontMetrics::get_text_width(const char *str, size_t n) const {
  uint16_t width = 0;
  const uint8_t *p = (const uint8_t *) str;
  for (;;) {
    const uint8_t val = *p++; n--;
    if (!val || n == 0) break;
    width += val < 128 ? char_widths[val] : 0;
  }
  return width;
}

uint16_t CLCD::FontMetrics::get_text_width(progmem_str str, size_t n) const {
  uint16_t width = 0;
  const uint8_t *p = (const uint8_t *) str;
  for (;;) {
    const uint8_t val = pgm_read_byte(p++); n--;
    if (!val || n == 0) break;
    width += val < 128 ? char_widths[val] : 0;
  }
  return width;
}

/************************** HOST COMMAND FUNCTION *********************************/

void CLCD::host_cmd(unsigned char host_command, unsigned char byte2) {  // Sends 24-Bit Host Command to LCD
  if (host_command != FTDI::ACTIVE) {
    host_command |= 0x40;
  }
  spi_ftdi_select();
  spi_send(host_command);
  spi_send(byte2);
  spi_send(0x00);
  spi_ftdi_deselect();
}

/************************** MEMORY READ FUNCTIONS *********************************/

void CLCD::spi_read_addr(uint32_t reg_address) {
  spi_send((reg_address >> 16) & 0x3F);  // Address [21:16]
  spi_send((reg_address >> 8 ) & 0xFF);  // Address [15:8]
  spi_send((reg_address >> 0)  & 0xFF);  // Address [7:0]
  spi_send(0x00);                        // Dummy Byte
}

// Write 4-Byte Address, Read Multiple Bytes
void CLCD::mem_read_bulk(uint32_t reg_address, uint8_t *data, uint16_t len) {
  spi_ftdi_select();
  spi_read_addr(reg_address);
  spi_read_bulk (data, len);
  spi_ftdi_deselect();
}

// Write 4-Byte Address, Read 1-Byte Data
uint8_t CLCD::mem_read_8(uint32_t reg_address) {
  spi_ftdi_select();
  spi_read_addr(reg_address);
  uint8_t r_data = spi_read_8();
  spi_ftdi_deselect();
  return r_data;
}

// Write 4-Byte Address, Read 2-Bytes Data
uint16_t CLCD::mem_read_16(uint32_t reg_address) {
  using namespace SPI::least_significant_byte_first;
  spi_ftdi_select();
  spi_read_addr(reg_address);
  uint16_t r_data = spi_read_16();
  spi_ftdi_deselect();
  return r_data;
}

// Write 4-Byte Address, Read 4-Bytes Data
uint32_t CLCD::mem_read_32(uint32_t reg_address) {
  using namespace SPI::least_significant_byte_first;
  spi_ftdi_select();
  spi_read_addr(reg_address);
  uint32_t r_data = spi_read_32();
  spi_ftdi_deselect();
  return r_data;
}

/************************** MEMORY WRITE FUNCTIONS *********************************/

// Generic operations for transforming a byte, for use with _mem_write_bulk:
static inline uint8_t reverse_byte(uint8_t a) {
  return ((a & 0x1)  << 7) | ((a & 0x2)  << 5) |
         ((a & 0x4)  << 3) | ((a & 0x8)  << 1) |
         ((a & 0x10) >> 1) | ((a & 0x20) >> 3) |
         ((a & 0x40) >> 5) | ((a & 0x80) >> 7);
}
static inline uint8_t xbm_write(const uint8_t *p) {return reverse_byte(pgm_read_byte(p));}

void CLCD::spi_write_addr(uint32_t reg_address) {
  spi_send((reg_address >> 16) | 0x80);  // Address [21:16]
  spi_send((reg_address >> 8 ) & 0xFF);  // Address [15:8]
  spi_send((reg_address >> 0)  & 0xFF);  // Address [7:0]
}

// Write 3-Byte Address, Multiple Bytes, plus padding bytes, from RAM
void CLCD::mem_write_bulk(uint32_t reg_address, const void *data, uint16_t len, uint8_t padding) {
  spi_ftdi_select();
  spi_write_addr(reg_address);
  spi_write_bulk<ram_write>(data, len, padding);
  spi_ftdi_deselect();
}

// Write 3-Byte Address, Multiple Bytes, plus padding bytes, from PROGMEM
void CLCD::mem_write_bulk(uint32_t reg_address, progmem_str str, uint16_t len, uint8_t padding) {
  spi_ftdi_select();
  spi_write_addr(reg_address);
  spi_write_bulk<pgm_write>(str, len, padding);
  spi_ftdi_deselect();
}

 // Write 3-Byte Address, Multiple Bytes, plus padding bytes, from PROGMEM
void CLCD::mem_write_pgm(uint32_t reg_address, const void *data, uint16_t len, uint8_t padding) {
  spi_ftdi_select();
  spi_write_addr(reg_address);
  spi_write_bulk<pgm_write>(data, len, padding);
  spi_ftdi_deselect();
}

// Write 3-Byte Address, Multiple Bytes, plus padding bytes, from PROGMEM, reversing bytes (suitable for loading XBM images)
void CLCD::mem_write_xbm(uint32_t reg_address, progmem_str data, uint16_t len, uint8_t padding) {
  spi_ftdi_select();
  spi_write_addr(reg_address);
  spi_write_bulk<xbm_write>(data, len, padding);
  spi_ftdi_deselect();
}

// Write 3-Byte Address, Write 1-Byte Data
void CLCD::mem_write_8(uint32_t reg_address, uint8_t data) {
  spi_ftdi_select();
  spi_write_addr(reg_address);
  spi_write_8(data);
  spi_ftdi_deselect();
}

// Write 3-Byte Address, Write 2-Bytes Data
void CLCD::mem_write_16(uint32_t reg_address, uint16_t data) {
  using namespace SPI::least_significant_byte_first;
  spi_ftdi_select();
  spi_write_addr(reg_address);
  spi_write_16(data);
  spi_ftdi_deselect();
}

// Write 3-Byte Address, Write 4-Bytes Data
void CLCD::mem_write_32(uint32_t reg_address, uint32_t data) {
  using namespace SPI::least_significant_byte_first;
  spi_ftdi_select();
  spi_write_addr(reg_address);
  spi_write_32(data);
  spi_ftdi_deselect();
}

// Fill area of len size with repeated data bytes
void CLCD::mem_write_fill(uint32_t reg_address, uint8_t data, uint16_t len) {
  spi_ftdi_select();
  spi_write_addr(reg_address);
  while (len--) spi_write_8(data);
  spi_ftdi_deselect();
}

/******************* FT800/810 Co-processor Commands *********************************/

#if FTDI_API_LEVEL == 800
uint32_t CLCD::CommandFifo::command_write_ptr = 0xFFFFFFFFul;
#endif

void CLCD::CommandFifo::cmd(uint32_t cmd32) {
  write((void*)&cmd32, sizeof(uint32_t));
}

void CLCD::CommandFifo::cmd(void *data, uint16_t len) {
  write(data, len);
}

void CLCD::CommandFifo::bgcolor(uint32_t rgb) {
  cmd(CMD_BGCOLOR);
  cmd(rgb);
}

void CLCD::CommandFifo::fgcolor(uint32_t rgb) {
  cmd(CMD_FGCOLOR);
  cmd(rgb);
}

void CLCD::CommandFifo::gradcolor(uint32_t rgb) {
  cmd(CMD_GRADCOLOR);
  cmd(rgb);
}

// This sends the a text command to the command preprocessor, must be followed by str()
void CLCD::CommandFifo::button(int16_t x, int16_t y, int16_t w, int16_t h, int16_t font,  uint16_t option) {
  struct {
    int32_t type = CMD_BUTTON;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    int16_t font;
    uint16_t option;
  } cmd_data;

  cmd_data.x      = x;
  cmd_data.y      = y;
  cmd_data.w      = w;
  cmd_data.h      = h;
  cmd_data.font   = font;
  cmd_data.option = option;

  cmd( &cmd_data, sizeof(cmd_data) );
}

// This sends the a text command to the command preprocessor, must be followed by str()
void CLCD::CommandFifo::text(int16_t x, int16_t y, int16_t font,  uint16_t options) {
  struct {
    int32_t type = CMD_TEXT;
    int16_t x;
    int16_t y;
    int16_t font;
    uint16_t options;
    } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.font    = font;
  cmd_data.options = options;

  cmd( &cmd_data, sizeof(cmd_data) );
}

// This sends the a toggle command to the command preprocessor, must be followed by str()
void CLCD::CommandFifo::toggle(int16_t x, int16_t y, int16_t w, int16_t font, uint16_t options, bool state) {
  struct {
    int32_t type = CMD_TOGGLE;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t font;
    uint16_t options;
    uint16_t state;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.w       = w;
  cmd_data.font    = font;
  cmd_data.options = options;
  cmd_data.state   = state ? 65535 : 0;

  cmd( &cmd_data, sizeof(cmd_data) );
}

// This sends the a keys command to the command preprocessor, must be followed by str()
void CLCD::CommandFifo::keys(int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t options) {
  struct {
    int32_t type = CMD_KEYS;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    int16_t font;
    uint16_t options;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.w       = w;
  cmd_data.h       = h;
  cmd_data.font    = font;
  cmd_data.options = options;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::clock(int16_t x, int16_t y, int16_t r, uint16_t options, int16_t h, int16_t m, int16_t s, int16_t ms)
{
  struct {
    int32_t type = CMD_CLOCK;
    int16_t x;
    int16_t y;
    int16_t r;
    uint16_t options;
    int16_t h;
    int16_t m;
    int16_t s;
    int16_t ms;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.r       = r;
  cmd_data.options = options;
  cmd_data.h       = h;
  cmd_data.m       = m;
  cmd_data.s       = s;
  cmd_data.ms      = ms;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::gauge(int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val, uint16_t range)
{
  struct {
    int32_t  type = CMD_GAUGE;
    int16_t  x;
    int16_t  y;
    int16_t  r;
    uint16_t options;
    uint16_t major;
    uint16_t minor;
    uint16_t val;
    uint16_t range;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.r       = r;
  cmd_data.options = options;
  cmd_data.major   = major;
  cmd_data.minor   = minor;
  cmd_data.val     = val;
  cmd_data.range   = range;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::dial(int16_t x, int16_t y, int16_t r, uint16_t options, uint16_t val)
{
  struct {
    int32_t  type = CMD_DIAL;
    int16_t  x;
    int16_t  y;
    int16_t  r;
    uint16_t options;
    uint16_t val;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.r       = r;
  cmd_data.options = options;
  cmd_data.val     = val;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::scrollbar(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t size, uint16_t range) {
  struct {
    int32_t  type = CMD_SCROLLBAR;
    int16_t  x;
    int16_t  y;
    int16_t  w;
    uint16_t h;
    uint16_t options;
    uint16_t val;
    uint16_t size;
    uint16_t range;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.w       = w;
  cmd_data.h       = h;
  cmd_data.options = options;
  cmd_data.val     = val;
  cmd_data.size    = size;
  cmd_data.range   = range;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::progress(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options,  uint16_t val, uint16_t range) {
  struct {
    int32_t  type = CMD_PROGRESS;
    int16_t  x;
    int16_t  y;
    int16_t  w;
    int16_t  h;
    uint16_t options;
    uint16_t val;
    uint16_t range;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.w       = w;
  cmd_data.h       = h;
  cmd_data.options = options;
  cmd_data.val     = val;
  cmd_data.range   = range;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::slider(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range) {
  struct {
    int32_t type = CMD_SLIDER;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    uint16_t options;
    uint16_t val;
    uint16_t range;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.w       = w;
  cmd_data.h       = h;
  cmd_data.options = options;
  cmd_data.val     = val;
  cmd_data.range   = range;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::gradient(int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1) {
  struct {
    int32_t type = CMD_GRADIENT;
    int16_t x0;
    int16_t y0;
    uint32_t rgb0;
    int16_t x1;
    int16_t y1;
    uint32_t rgb1;
  } cmd_data;

  cmd_data.x0      = x0;
  cmd_data.y0      = y0;
  cmd_data.rgb0    = rgb0;
  cmd_data.x1      = x1;
  cmd_data.y1      = y1;
  cmd_data.rgb1    = rgb1;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::number(int16_t x, int16_t y, int16_t font, uint16_t options, int32_t n) {
  struct {
    int32_t  type = CMD_NUMBER;
    int16_t  x;
    int16_t  y;
    int16_t  font;
    uint16_t options;
    int16_t n;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.font    = font;
  cmd_data.options = options;
  cmd_data.n       = n;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::memzero(uint32_t ptr, uint32_t size) {
  struct {
    uint32_t  type = CMD_MEMZERO;
    uint32_t  ptr;
    uint32_t  size;
  } cmd_data;

  cmd_data.ptr    = ptr;
  cmd_data.size   = size;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::memset(uint32_t ptr, uint32_t val, uint32_t size) {
  struct {
    uint32_t  type = CMD_MEMSET;
    uint32_t  ptr;
    uint32_t  val;
    uint32_t  size;
  } cmd_data;

  cmd_data.ptr    = ptr;
  cmd_data.val    = val;
  cmd_data.size   = size;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::memcpy(uint32_t dst, uint32_t src, uint32_t size) {
  struct {
    uint32_t  type = CMD_MEMCPY;
    uint32_t  dst;
    uint32_t  src;
    uint32_t  size;
  } cmd_data;

  cmd_data.dst    = dst;
  cmd_data.src    = src;
  cmd_data.size   = size;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::memcrc(uint32_t ptr, uint32_t num, uint32_t result) {
  struct {
    uint32_t  type = CMD_MEMCRC;
    uint32_t  ptr;
    uint32_t  num;
    uint32_t  result;
  } cmd_data;

  cmd_data.ptr    = ptr;
  cmd_data.num    = num;
  cmd_data.result   = result;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::memwrite(uint32_t ptr, uint32_t value) {
  struct {
    uint32_t  type = CMD_MEMWRITE;
    uint32_t  ptr;
    uint32_t  num;
    uint32_t  value;
  } cmd_data;

  cmd_data.ptr    = ptr;
  cmd_data.num    = 4;
  cmd_data.value  = value;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::append(uint32_t ptr, uint32_t size) {
  struct {
    uint32_t  type = CMD_APPEND;
    uint32_t  ptr;
    uint32_t  size;
  } cmd_data;

  cmd_data.ptr    = ptr;
  cmd_data.size   = size;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::inflate(uint32_t ptr) {
  struct {
    uint32_t  type = CMD_INFLATE;
    uint32_t  ptr;
  } cmd_data;

  cmd_data.ptr    = ptr;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::getptr(uint32_t result) {
  struct {
    uint32_t  type = CMD_GETPTR;
    uint32_t  result;
  } cmd_data;

  cmd_data.result    = result;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::track(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t tag) {
  struct {
    uint32_t type = CMD_TRACK;
    int16_t  x;
    int16_t  y;
    int16_t  w;
    int16_t  h;
    int16_t  tag;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.w       = w;
  cmd_data.h       = h;
  cmd_data.tag     = tag;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::sketch(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t ptr, uint16_t format) {
  struct {
    uint32_t type = CMD_SKETCH;
    int16_t   x;
    int16_t   y;
    uint16_t  w;
    uint16_t  h;
    uint32_t  ptr;
    uint16_t  format;
  } cmd_data;

  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.w       = w;
  cmd_data.h       = h;
  cmd_data.ptr     = ptr;
  cmd_data.format  = format;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::snapshot(uint32_t ptr) {
  struct {
    uint32_t type = CMD_SNAPSHOT;
    uint32_t  ptr;
  } cmd_data;

  cmd_data.ptr     = ptr;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::spinner(int16_t x, int16_t y, uint16_t style, uint16_t scale) {
  struct {
    uint32_t type = CMD_SPINNER;
    uint16_t x;
    uint16_t y;
    uint16_t style;
    uint16_t scale;
  } cmd_data;

  cmd_data.x     = x;
  cmd_data.y     = y;
  cmd_data.style = style;
  cmd_data.scale = scale;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::loadimage(uint32_t ptr, uint32_t options) {
  struct {
    uint32_t type = CMD_LOADIMAGE;
    uint32_t ptr;
    uint32_t options;
  } cmd_data;

  cmd_data.ptr     = ptr;
  cmd_data.options = options;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::getprops(uint32_t ptr, uint32_t width, uint32_t height) {
  struct {
    uint32_t  type = CMD_GETPROPS;
    uint32_t  ptr;
    uint32_t  width;
    uint32_t  height;
  } cmd_data;

  cmd_data.ptr    = ptr;
  cmd_data.width  = width;
  cmd_data.height = height;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::scale(int32_t sx, int32_t sy) {
  struct {
    uint32_t type = CMD_SCALE;
    int32_t  sx;
    int32_t  sy;
  } cmd_data;

  cmd_data.sx       = sx;
  cmd_data.sy       = sy;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::rotate(int32_t a) {
  struct {
    uint32_t type = CMD_ROTATE;
    int32_t  a;
  } cmd_data;

  cmd_data.a       = a;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::translate(int32_t tx, int32_t ty) {
  struct {
    uint32_t type = CMD_TRANSLATE;
    int32_t  tx;
    int32_t  ty;
  } cmd_data;

  cmd_data.tx       = tx;
  cmd_data.ty       = ty;

  cmd( &cmd_data, sizeof(cmd_data) );
}

#if FTDI_API_LEVEL >= 810
void CLCD::CommandFifo::setbase(uint8_t base) {
  struct {
    int32_t  type = CMD_SETBASE;
    uint32_t base;
  } cmd_data;

  cmd_data.base     = base;

  cmd( &cmd_data, sizeof(cmd_data) );
}
#endif

#if FTDI_API_LEVEL >= 810
void CLCD::CommandFifo::setbitmap(uint32_t addr, uint16_t fmt, uint16_t w, uint16_t h) {
  struct {
    uint32_t type = CMD_SETBITMAP;
    uint32_t addr;
    uint16_t fmt;
    uint16_t w;
    uint16_t h;
    uint16_t dummy;
  } cmd_data;

  cmd_data.addr    = addr;
  cmd_data.fmt     = fmt;
  cmd_data.w       = w;
  cmd_data.h       = h;
  cmd_data.dummy   = 0;

  cmd( &cmd_data, sizeof(cmd_data) );
}
#endif

#if FTDI_API_LEVEL >= 810
void CLCD::CommandFifo::snapshot2(uint32_t format, uint32_t ptr, int16_t x, int16_t y, uint16_t w, uint16_t h) {
  struct {
    uint32_t type = CMD_SNAPSHOT2;
    uint32_t  format;
    uint32_t  ptr;
    int16_t   x;
    int16_t   y;
    uint16_t  w;
    uint16_t  h;
  } cmd_data;

  cmd_data.format  = format;
  cmd_data.ptr     = ptr;
  cmd_data.x       = x;
  cmd_data.y       = y;
  cmd_data.w       = w;
  cmd_data.h       = h;

  cmd( &cmd_data, sizeof(cmd_data) );
}
#endif

#if FTDI_API_LEVEL >= 810
void CLCD::CommandFifo::mediafifo(uint32_t ptr, uint32_t size) {
  struct {
    uint32_t type = CMD_MEDIAFIFO;
    uint32_t ptr;
    uint32_t size;
  } cmd_data;

  cmd_data.ptr  = ptr;
  cmd_data.size = size;

  cmd( &cmd_data, sizeof(cmd_data) );
}
#endif

#if FTDI_API_LEVEL >= 810
void CLCD::CommandFifo::videostart() {
  cmd( CMD_VIDEOSTART );
}
#endif

#if FTDI_API_LEVEL >= 810
void CLCD::CommandFifo::videoframe(uint32_t dst, uint32_t ptr) {
  struct {
    uint32_t type = CMD_VIDEOFRAME;
    uint32_t dst;
    uint32_t ptr;
  } cmd_data;

  cmd_data.dst = dst;
  cmd_data.ptr = ptr;

  cmd( &cmd_data, sizeof(cmd_data) );
}
#endif

#if FTDI_API_LEVEL >= 810
void CLCD::CommandFifo::playvideo(uint32_t options) {
  struct {
    uint32_t type = CMD_PLAYVIDEO;
    uint32_t options;
  } cmd_data;

  cmd_data.options = options;

  cmd( &cmd_data, sizeof(cmd_data) );
}
#endif

#if FTDI_API_LEVEL >= 810
void CLCD::CommandFifo::setrotate(uint8_t rotation) {
  struct {
    uint32_t  type = CMD_SETROTATE;
    uint32_t  rotation;
  } cmd_data;

  cmd_data.rotation = rotation;

  cmd( &cmd_data, sizeof(cmd_data) );
}
#endif

#if FTDI_API_LEVEL >= 810
void CLCD::CommandFifo::romfont(uint8_t font, uint8_t romslot) {
  struct {
    uint32_t  type = CMD_ROMFONT;
    uint32_t  font;
    uint32_t  romslot;
  } cmd_data;

  cmd_data.font    = font;
  cmd_data.romslot = romslot;

  cmd( &cmd_data, sizeof(cmd_data) );
}
#endif

/**************************** FT800/810 Co-Processor Command FIFO ****************************/

bool CLCD::CommandFifo::is_processing() {
  return (mem_read_32(REG::CMD_READ) & 0x0FFF) != (mem_read_32(REG::CMD_WRITE) & 0x0FFF);
}

bool CLCD::CommandFifo::has_fault() {
  uint16_t Cmd_Read_Reg  = mem_read_32(REG::CMD_READ) & 0x0FFF;
  return Cmd_Read_Reg == 0x0FFF;
}

#if FTDI_API_LEVEL == 800

void CLCD::CommandFifo::start() {
  if (command_write_ptr == 0xFFFFFFFFul) {
    command_write_ptr = mem_read_32(REG::CMD_WRITE) & 0x0FFF;
  }
}

void CLCD::CommandFifo::execute() {
  if (command_write_ptr != 0xFFFFFFFFul) {
    mem_write_32(REG::CMD_WRITE, command_write_ptr);
  }
}

void CLCD::CommandFifo::reset() {
  safe_delay(100);
  mem_write_32(REG::CPURESET,  0x00000001);
  mem_write_32(REG::CMD_WRITE, 0x00000000);
  mem_write_32(REG::CMD_READ,  0x00000000);
  mem_write_32(REG::CPURESET,  0x00000000);
  safe_delay(300);
  command_write_ptr = 0xFFFFFFFFul;
};

template <class T> bool CLCD::CommandFifo::_write_unaligned(T data, uint16_t len) {
  const char *ptr = (const char*)data;
  uint32_t bytes_tail, bytes_head;
  uint32_t command_read_ptr;

  #if ENABLED(TOUCH_UI_DEBUG)
    if (command_write_ptr == 0xFFFFFFFFul)
      SERIAL_ECHO_MSG("Attempt to write to FIFO before CommandFifo::Cmd_Start().");
  #endif

  /* Wait until there is enough space in the circular buffer for the transfer */
  do {
    command_read_ptr = mem_read_32(REG::CMD_READ) & 0x0FFF;
    if (command_read_ptr <= command_write_ptr) {
      bytes_tail = 4096U - command_write_ptr;
      bytes_head = command_read_ptr;
    }
    else {
      bytes_tail = command_read_ptr - command_write_ptr;
      bytes_head = 0;
    }
    // Check for faults which can lock up the command processor
    if (has_fault()) {
      #if ENABLED(TOUCH_UI_DEBUG)
        SERIAL_ECHOLNPGM("Fault waiting for space in the command processor");
      #endif
      return false;
    }
  } while ((bytes_tail + bytes_head) < len);

  /* Write as many bytes as possible following REG::CMD_WRITE */
  uint16_t bytes_to_write = min(len, bytes_tail);
  mem_write_bulk (MAP::RAM_CMD + command_write_ptr, T(ptr), bytes_to_write);
  command_write_ptr += bytes_to_write;
  ptr  += bytes_to_write;
  len  -= bytes_to_write;

  if (len > 0) {
    /* Write remaining bytes at start of circular buffer */
    mem_write_bulk (MAP::RAM_CMD, T(ptr), len);
    command_write_ptr = len;
  }

  if (command_write_ptr == 4096U) {
    command_write_ptr = 0;
  }
  return true;
}

// Writes len bytes into the FIFO, if len is not
// divisible by four, zero bytes will be written
// to align to the boundary.

template <class T> bool CLCD::CommandFifo::write(T data, uint16_t len) {
  const uint8_t padding = MULTIPLE_OF_4(len) - len;
  const uint8_t pad_bytes[] = { 0, 0, 0, 0 };
  return _write_unaligned(data,      len) &&
         _write_unaligned(pad_bytes, padding);
}

#else // FTDI_API_LEVEL != 800 ...

void CLCD::CommandFifo::start() {
}

void CLCD::CommandFifo::execute() {
}

void CLCD::CommandFifo::reset() {
  #if ENABLED(TOUCH_UI_DEBUG)
    SERIAL_ECHOLNPGM("Resetting command processor");
  #endif
  safe_delay(100);
  mem_write_32(REG::CPURESET,  0x00000001);
  mem_write_32(REG::CMD_WRITE, 0x00000000);
  mem_write_32(REG::CMD_READ,  0x00000000);
  mem_write_32(REG::CPURESET,  0x00000000);
  safe_delay(300);
};

// Writes len bytes into the FIFO, if len is not
// divisible by four, zero bytes will be written
// to align to the boundary.

template <class T> bool CLCD::CommandFifo::write(T data, uint16_t len) {
  const uint8_t padding = MULTIPLE_OF_4(len) - len;

  if (has_fault()) {
    #if ENABLED(TOUCH_UI_DEBUG)
      SERIAL_ECHOLNPGM("Faulted... ignoring write.");
    #endif
    return false;
  }
  // The FT810 provides a special register that can be used
  // for writing data without us having to do our own FIFO
  // management.
  uint16_t Command_Space = mem_read_32(REG::CMDB_SPACE) & 0x0FFF;
  if (Command_Space < (len + padding)) {
    #if ENABLED(TOUCH_UI_DEBUG)
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR("Waiting for ", len + padding);
      SERIAL_ECHOLNPAIR(" bytes in command queue, now free: ", Command_Space);
    #endif
    do {
      Command_Space = mem_read_32(REG::CMDB_SPACE) & 0x0FFF;
      if (has_fault()) {
        #if ENABLED(TOUCH_UI_DEBUG)
          SERIAL_ECHOLNPGM("... fault");
        #endif
        return false;
      }
    } while (Command_Space < len + padding);
    #if ENABLED(TOUCH_UI_DEBUG)
      SERIAL_ECHOLNPGM("... done");
    #endif
  }
  mem_write_bulk(REG::CMDB_WRITE, data, len, padding);
  return true;
}

#endif // ... FTDI_API_LEVEL != 800

template bool CLCD::CommandFifo::write(const void*, uint16_t);
template bool CLCD::CommandFifo::write(progmem_str, uint16_t);

// CO_PROCESSOR COMMANDS

void CLCD::CommandFifo::str(const char * data, size_t maxlen) {
  // Write the string without the terminating '\0'
  const size_t len = strnlen(data, maxlen);
  write(data, len);

  // If padding was added by the previous write, then
  // the string is terminated. Otherwise write four
  // more zeros.
  const uint8_t padding = MULTIPLE_OF_4(len) - len;
  if (padding == 0) {
    const uint8_t pad_bytes[] = {0, 0, 0, 0};
    write(pad_bytes, 4);
  }
}

void CLCD::CommandFifo::str(const char * data) {
  write(data, strlen(data)+1);
}

void CLCD::CommandFifo::str(progmem_str data) {
  write(data, strlen_P((const char*)data)+1);
}

/******************* LCD INITIALIZATION ************************/

void CLCD::init() {
  spi_init();                                  // Set Up I/O Lines for SPI and FT800/810 Control
  ftdi_reset();                                // Power down/up the FT8xx with the apropriate delays

  host_cmd(Use_Crystal ? CLKEXT : CLKINT, 0);
  host_cmd(FTDI::ACTIVE, 0);                        // Activate the System Clock

  delay(40); // FTDI/BRT recommendation: no SPI traffic during startup. EVE needs at the very least 45ms to start, so leave her alone for a little while.

  /* read the device-id until it returns 0x7C or times out, should take less than 150ms */
  uint8_t counter;
  for (counter = 0; counter < 250; counter++) {
   uint8_t device_id = mem_read_8(REG::ID);            // Read Device ID, Should Be 0x7C;
   if (device_id == 0x7C) {
     if (ENABLED(TOUCH_UI_DEBUG)) SERIAL_ECHO_MSG("FTDI chip initialized ");
     break;
   }
   else
     delay(1);

    if (TERN0(TOUCH_UI_DEBUG, counter > 248))
      SERIAL_ECHO_MSG("Timeout waiting for device ID, should be 124, got ", device_id);
  }

  /* Ensure all units are in working condition, usually the touch-controller needs a little more time */
  for (counter = 0; counter < 100; counter++) {
    uint8_t reset_status = mem_read_8(REG::CPURESET) & 0x03;
    if (reset_status == 0x00) {
      if (ENABLED(TOUCH_UI_DEBUG)) SERIAL_ECHO_MSG("FTDI chip all units running ");
      break;
    }
    else
      delay(1);

    if (TERN0(TOUCH_UI_DEBUG, counter > 98))
      SERIAL_ECHO_MSG("Timeout waiting for reset status. Should be 0x00, got ", reset_status);
  }

  #if ENABLED(USE_GT911)   /* switch BT815 to use Goodix GT911 touch controller */
    mem_write_32(REG::TOUCH_CONFIG, 0x000005D1);
  #endif

  #if ENABLED(PATCH_GT911) /* patch FT813 use Goodix GT911 touch controller */
    mem_write_pgm(REG::CMDB_WRITE, GT911_data, sizeof(GT911_data)); /* write binary blob to command-fifo */
    delay(10);
    mem_write_8(REG::TOUCH_OVERSAMPLE, 0x0F);  /* setup oversample to 0x0f as "hidden" in binary-blob for AN_336 */
    mem_write_16(REG::TOUCH_CONFIG, 0x05d0); /* write magic cookie as requested by AN_336 */

    /* specific to the EVE2 modules from Matrix-Orbital we have to use GPIO3 to reset GT911 */
    mem_write_16(REG::GPIOX_DIR,0x8008); /* Reset-Value is 0x8000, adding 0x08 sets GPIO3 to output, default-value for REG_GPIOX is 0x8000 -> Low output on GPIO3 */
    delay(1); /* wait more than 100µs */
    mem_write_8(REG::CPURESET, 0x00); /* clear all resets */
    delay(56); /* wait more than 55ms */
    mem_write_16(REG::GPIOX_DIR,0x8000); /* setting GPIO3 back to input */
  #endif

  mem_write_8(REG::PWM_DUTY, 0);   // turn off Backlight, Frequency already is set to 250Hz default

  /* Configure the FT8xx Registers */
  mem_write_16(REG::HCYCLE,  FTDI::Hcycle);
  mem_write_16(REG::HOFFSET, FTDI::Hoffset);
  mem_write_16(REG::HSYNC0,  FTDI::Hsync0);
  mem_write_16(REG::HSYNC1,  FTDI::Hsync1);
  mem_write_16(REG::VCYCLE,  FTDI::Vcycle);
  mem_write_16(REG::VOFFSET, FTDI::Voffset);
  mem_write_16(REG::VSYNC0,  FTDI::Vsync0);
  mem_write_16(REG::VSYNC1,  FTDI::Vsync1);
  mem_write_16(REG::HSIZE,   FTDI::Hsize);
  mem_write_16(REG::VSIZE,   FTDI::Vsize);
  mem_write_8(REG::SWIZZLE,  FTDI::Swizzle);
  mem_write_8(REG::PCLK_POL, FTDI::Pclkpol);
  mem_write_8(REG::CSPREAD,  FTDI::CSpread);

  /* write a basic display-list to get things started */
  mem_write_32(MAP::RAM_DL,      DL::CLEAR_COLOR_RGB);
  mem_write_32(MAP::RAM_DL + 4, (DL::CLEAR | 0x07)); /* clear color, stencil and tag buffer */
  mem_write_32(MAP::RAM_DL + 8,  DL::DL_DISPLAY);    /* end of display list */

  mem_write_8(REG::DLSWAP, 0x02); // activate display list, Bad Magic Cookie 2 = switch to new list after current frame is scanned out

  //mem_write_8(REG::TOUCH_MODE, 0x03);      // Configure the Touch Screen, Bad Magic Cookie, 3 = CONTINUOUS = Reset Default
  //mem_write_8(REG::TOUCH_ADC_MODE, 0x01);  // Bad Magic Cookie, 1 = single touch = Reset Default
  //mem_write_8(REG::TOUCH_OVERSAMPLE, 0x0F); // Reset Default = 7 - why 15?
  mem_write_16(REG::TOUCH_RZTHRESH, touch_threshold); /* setup touch sensitivity */
  mem_write_8(REG::VOL_SOUND, 0x00);       // Turn Synthesizer Volume Off

  /* turn on the display by setting DISP high */
  /* turn on the Audio Amplifier by setting GPIO_1 high for the select few modules supporting this */
  /* no need to use GPIOX here since DISP/GPIO_0 and GPIO_1 are on REG::GPIO for FT81x as well */
  if (GPIO_1_Audio_Shutdown) {
    mem_write_8(REG::GPIO_DIR,   GPIO_DISP  | GPIO_GP1);
    mem_write_8(REG::GPIO,       GPIO_DISP  | GPIO_GP1);
  }
  else if (GPIO_0_Audio_Enable) {
    mem_write_8(REG::GPIO_DIR,   GPIO_DISP  | GPIO_GP0);
    mem_write_8(REG::GPIO,       GPIO_DISP  | GPIO_GP0);
  }
  else
    mem_write_8(REG::GPIO, GPIO_DISP); /* REG::GPIO_DIR is set to output for GPIO_DISP by default */

  mem_write_8(REG::PCLK, Pclk); // Turns on Clock by setting PCLK Register to the value necessary for the module

  mem_write_16(REG::PWM_HZ, 0x00FA);

  // Turning off dithering seems to help prevent horizontal line artifacts on certain colors
  mem_write_8(REG::DITHER,  0);

  default_touch_transform();
  default_display_orientation();
}

void CLCD::default_touch_transform() {
  // Set Initial Values for Touch Transform Registers
  mem_write_32(REG::ROTATE, 0);
  mem_write_32(REG::TOUCH_TRANSFORM_A, FTDI::default_transform_a);
  mem_write_32(REG::TOUCH_TRANSFORM_B, FTDI::default_transform_b);
  mem_write_32(REG::TOUCH_TRANSFORM_C, FTDI::default_transform_c);
  mem_write_32(REG::TOUCH_TRANSFORM_D, FTDI::default_transform_d);
  mem_write_32(REG::TOUCH_TRANSFORM_E, FTDI::default_transform_e);
  mem_write_32(REG::TOUCH_TRANSFORM_F, FTDI::default_transform_f);
}

void CLCD::default_display_orientation() {
  #if FTDI_API_LEVEL >= 810
    // Set the initial display orientation. On the FT810, we use the command
    // processor to do this since it will also update the transform matrices.
    CommandFifo cmd;
    cmd.setrotate(
        ENABLED(TOUCH_UI_MIRRORED) * 4
      + ENABLED(TOUCH_UI_PORTRAIT) * 2
      + ENABLED(TOUCH_UI_INVERTED) * 1
    );
    cmd.execute();
  #elif ANY(TOUCH_UI_PORTRAIT, TOUCH_UI_MIRRORED)
    #error "PORTRAIT or MIRRORED orientation not supported on the FT800."
  #elif ENABLED(TOUCH_UI_INVERTED)
    mem_write_32(REG::ROTATE, 1);
  #endif
}

#endif // FTDI_BASIC
