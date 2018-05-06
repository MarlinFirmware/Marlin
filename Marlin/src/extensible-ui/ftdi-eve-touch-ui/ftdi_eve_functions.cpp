/************************
 * ftdi_eve_functions.h *
 ************************/

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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../../Marlin.h"
#include "../ui_config.h"

#if defined(FTDI_EVE_TOUCHSCREEN)

#include "ftdi_eve_constants.h"
#include "ftdi_eve_timings.h"
#include "ftdi_eve_functions.h"

void CLCD::enable (void) {
  mem_write_8(REG_PCLK, Pclk);
}

void CLCD::disable (void) {
  mem_write_8(REG_PCLK, 0x00);
}

void CLCD::set_backlight (uint16_t Freq, uint8_t Duty) {
  mem_write_16(REG_PWM_HZ, Freq);
  mem_write_8(REG_PWM_DUTY, Duty);
}

void CLCD::turn_on_backlight (void) {
  set_backlight(0x00FA, 128);
}

void CLCD::get_font_metrics(uint8_t font, struct FontMetrics &fm) {
  mem_read_bulk(ROM_FONT_ADDR + 148 * (font - 16), (uint8_t*) &fm, 148);
}

// HOST COMMAND FUNCTION

void CLCD::host_cmd (unsigned char host_command, unsigned char byte2) {  // Sends 24-Bit Host Command to LCD
  if(host_command != ACTIVE) {
    host_command |= 0x40;
  }
  spi_select();
  spi_transfer(host_command);
  spi_transfer(byte2);
  spi_transfer(0x00);
  spi_deselect();
}

void CLCD::flash_write_rgb332_bitmap(uint32_t mem_address, const unsigned char* p_rgb332_array, uint16_t num_bytes)
{
  for(unsigned int i = 0; i < num_bytes; i++) {
    unsigned char flash_byte = pgm_read_byte_near(p_rgb332_array + i);
    mem_write_8((mem_address + i), flash_byte);
  }
}

/******************* FT800/810 Graphic Commands *********************************/

#if defined(USE_FTDI_FT800)
uint32_t CLCD::CommandFifo::command_write_ptr = 0xFFFFFFFFul;
#endif

// DL Command - Set Clear Screen Color
void CLCD::CommandFifo::set_clear_color (uint32_t rgb) {
  cmd(CLEAR_COLOR_RGB | rgb);
}

// DL Command - Clear Color-Stencil-Tag Buffer(s)
void CLCD::CommandFifo::clear (bool clr, bool stl, bool tag) {
  cmd( CLEAR |
    (clr ? 0b00000001 : 0) |
    (stl ? 0b00000010 : 0) |
    (tag ? 0b00000100 : 0)
  );
}

// DL Command - Set Current Color
void CLCD::CommandFifo::set_color (uint32_t rgb) {
  cmd(COLOR_RGB | rgb);
}

// Co-Processor Command - Set Foreground Color for Widgets
void CLCD::CommandFifo::set_foreground_color (uint32_t rgb) {
  cmd(CMD_FGCOLOR);
  cmd(rgb);
}

// Co-Processor Command - Set Background Color for Widgets
void CLCD::CommandFifo::set_background_color (uint32_t rgb) {
  cmd(CMD_BGCOLOR);
  cmd(rgb);
}

void CLCD::CommandFifo::set_tag (uint8_t tag) {
  cmd(TAG | tag);
}

void CLCD::CommandFifo::bitmap_source (uint32_t ram_g_addr) {
  cmd(BITMAP_SOURCE | (ram_g_addr & 0x000FFFFF));
}

void CLCD::CommandFifo::bitmap_layout (uint8_t format, uint16_t linestride, uint16_t height) {
  cmd( BITMAP_LAYOUT |
    (uint32_t(height)     << 0)  |
    (uint32_t(linestride) << 9)  |
    (uint32_t(format)     << 19)
  );
}

void CLCD::CommandFifo::bitmap_size(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height) {
  cmd( BITMAP_SIZE |
    (uint32_t(height)  << 0)  |
    (uint32_t(width)   << 9)  |
    (uint32_t(wrapy)   << 18) |
    (uint32_t(wrapx)   << 19) |
    (uint32_t(filter)  << 20)
  );
}

void CLCD::CommandFifo::bitmap_handle (uint16_t Handle) {
  cmd( BITMAP_HANDLE | Handle);
}


void CLCD::CommandFifo::begin (uint32_t Primitive) {
  cmd(BEGIN + Primitive);
}

void CLCD::CommandFifo::vertex_2ii (uint16_t x_coord, uint16_t y_coord, uint8_t b_handle, uint8_t cell) {
  cmd( VERTEX2II |
    (uint32_t(cell)     << 0)  |
    (uint32_t(b_handle) << 7)  |
    (uint32_t(y_coord)  << 12) |
    (uint32_t(x_coord)  << 21)
  );
}

void CLCD::CommandFifo::vertex_2f (uint16_t x_Coord, uint16_t y_Coord) {
  cmd( VERTEX2F |
    (uint32_t(y_Coord)  << 0) |
    (uint32_t(x_Coord)  << 15)
  );
}

void CLCD::CommandFifo::draw_clock (int16_t x, int16_t y, int16_t r, uint16_t option, int16_t h, int16_t m, int16_t s, int16_t ms)
{
  struct {
    int32_t type = CMD_CLOCK;
    int16_t x;
    int16_t y;
    int16_t r;
    uint16_t option;
    int16_t h;
    int16_t m;
    int16_t s;
    int16_t ms;
  } cmd_data;

  cmd_data.x      = x;
  cmd_data.y      = y;
  cmd_data.r      = r;
  cmd_data.option = option;
  cmd_data.h      = h;
  cmd_data.m      = m;
  cmd_data.s      = s;
  cmd_data.ms     = ms;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::draw_progress_bar (int16_t x, int16_t y, int16_t w, int16_t h, int16_t val, int16_t range) {
  struct {
    int32_t  type = CMD_PROGRESS;
    int16_t  x;
    int16_t  y;
    int16_t  w;
    uint16_t h;
    uint16_t val;
    uint16_t range;
  } cmd_data;

  cmd_data.x      = x;
  cmd_data.y      = y;
  cmd_data.w      = w;
  cmd_data.h      = h;
  cmd_data.val    = val;
  cmd_data.range  = range;

  cmd( &cmd_data, sizeof(cmd_data) );
}

void CLCD::CommandFifo::draw_slider (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range) {
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

void CLCD::CommandFifo::mem_cpy (uint32_t dst, uint32_t src, uint32_t size) {
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

void CLCD::CommandFifo::append (uint32_t ptr, uint32_t size) {
  struct {
    uint32_t  type = CMD_APPEND;
    uint32_t  ptr;
    uint32_t  size;
  } cmd_data;

  cmd_data.ptr    = ptr;
  cmd_data.size   = size;

  cmd( &cmd_data, sizeof(cmd_data) );
}

/******************* DISPLAY LIST CACHE MANAGEMENT ************************/

// The init function ensures all cache locatios are marked as empty

void CLCD::DLCache::init() {
  mem_write_32(DL_FREE_ADDR, DL_FREE_ADDR + 4);
  for(uint8_t slot = 0; slot < DL_CACHE_SLOTS; slot++) {
    save_slot(slot, 0, 0);
  }
}

bool CLCD::DLCache::has_data() {
  #if !defined(DL_CACHE_DISABLED)
  return dl_size != 0;
  #else
  return false;
  #endif
}

/* This caches the current display list in RAMG so
 * that it can be appended later. The memory is
 * dynamically allocated following DL_FREE_ADDR.
 *
 * If num_bytes is provided, then that many bytes
 * will be reserved so that the cache may be re-written
 * later with potentially a bigger DL.
 */

bool CLCD::DLCache::store(uint32_t num_bytes /* = 0*/) {
  #if !defined(DL_CACHE_DISABLED)
  CLCD::CommandFifo cmd;

  // Execute any commands already in the FIFO
  cmd.execute();
  cmd.wait_until_idle();

  // Figure out how long the display list is
  uint32_t new_dl_size = mem_read_32(REG_CMD_DL) & 0x1FFF;
  uint32_t free_space  = 0;
  uint32_t dl_alloc    = 0;

  if(dl_addr == 0) {
    // If we are allocating new space...
    dl_addr     = mem_read_32(DL_FREE_ADDR);
    free_space  = RAM_G_SIZE - dl_addr;
    dl_alloc    = num_bytes ? num_bytes : new_dl_size;
    dl_size     = new_dl_size;
  } else {
    // Otherwise, we can only store as much space
    // as was previously allocated.
    free_space  = num_bytes ? num_bytes : dl_size;
    dl_alloc    = 0;
    dl_size     = new_dl_size;
  }

  if(dl_size > free_space) {
    // Not enough memory to cache the display list.
    #if defined(UI_FRAMEWORK_DEBUG)
      #if defined (SERIAL_PROTOCOLLNPAIR)
        SERIAL_PROTOCOLPAIR("Not enough space in GRAM to cache display list, free space: ", free_space);
        SERIAL_PROTOCOLLNPAIR(" Required: ", dl_size);
      #else
        Serial.print(F("Not enough space in GRAM to cache display list, free space:"));
        Serial.print(free_space);
        Serial.print(F(" Required: "));
        Serial.println(dl_size);
      #endif
    #endif
    return false;
  } else {
    #if defined(UI_FRAMEWORK_DEBUG)
      #if defined (SERIAL_PROTOCOLLNPAIR)
        SERIAL_PROTOCOLPAIR("Saving DL to RAMG cache, bytes: ", dl_size);
        SERIAL_PROTOCOLPAIR(" (Free space: ", free_space);
        SERIAL_PROTOCOLLNPGM(")");
      #else
        Serial.print(F("Saving DL to RAMG cache, bytes: "));
        Serial.println(dl_size);
        Serial.print(F(" (Free space: "));
        Serial.println(free_space);
        Serial.print(F(")"));
      #endif
    #endif
    cmd.mem_cpy(dl_addr, RAM_DL, dl_size);
    cmd.execute();
    save_slot(dl_slot, dl_addr, dl_size);
    if(dl_alloc > 0) {
      // If we allocated space dynamically, then adjust dl_free_addr.
      mem_write_32(DL_FREE_ADDR, dl_addr + dl_alloc);
    }
    return true;
  }
  #endif // DL_CACHE_DISABLED
}

void CLCD::DLCache::save_slot(uint8_t dl_slot, uint32_t dl_addr, uint32_t dl_size) {
  mem_write_32(DL_CACHE_START + dl_slot * 8 + 0, dl_addr);
  mem_write_32(DL_CACHE_START + dl_slot * 8 + 4, dl_size);
}

void CLCD::DLCache::load_slot() {
  dl_addr  = mem_read_32(DL_CACHE_START + dl_slot * 8 + 0);
  dl_size  = mem_read_32(DL_CACHE_START + dl_slot * 8 + 4);
}

void CLCD::DLCache::append() {
  CLCD::CommandFifo cmd;
  #if !defined(DL_CACHE_DISABLED)
  cmd.append(dl_addr, dl_size);
  #endif
  #if defined(UI_FRAMEWORK_DEBUG)
    cmd.execute();
    cmd.wait_until_idle();
    #if defined (SERIAL_PROTOCOLLNPAIR)
      SERIAL_PROTOCOLPAIR("Appending to DL from RAMG cache, bytes: ", dl_size);
      SERIAL_PROTOCOLPAIR(" (REG_CMD_DL: ", mem_read_32(REG_CMD_DL));
      SERIAL_PROTOCOLLNPGM(")");
    #else
      Serial.print(F("Appending to DL from RAMG cache, bytes: "));
      Serial.print(dl_size);
      Serial.print(F(" (REG_CMD_DL: "));
      Serial.print(mem_read_32(REG_CMD_DL));
      Serial.println(F(")"));
    #endif
  #endif // DL_CACHE_DISABLED
}

/******************* LCD INITIALIZATION ************************/

void CLCD::init (void) {
  spi_init();                                  // Set Up I/O Lines for SPI and FT800/810 Control
  delay(50);

  reset();                                    // Power Down the FT800/810 for 50 ms
  delay(50);

/*
 *  If driving the 4D Systems 4DLCD-FT843 Board, the following Init sequence is needed for its FT800 Driver
 */

#ifdef USE_FTDI_FT800                                    // Use External Crystal and 48 MHz System Clock
  host_cmd(CLKEXT, 0);

  delay(20);
  host_cmd(CLK48M, 0);
#else
  host_cmd(CLKINT, 0);
  delay(20);
  host_cmd(CLKSEL, Clksel);                     // Use Internal RC Oscillator and 48 MHz System Clock
#endif

  delay(20);

  host_cmd(ACTIVE, 0);                        // Activate the System Clock
  delay(50);

  delay(400);
  uint8_t device_id = mem_read_8(REG_ID);            // Read Device ID, Should Be 0x7C;
  #if defined(UI_FRAMEWORK_DEBUG)
  if(device_id != 0x7C) {
    #if defined (SERIAL_PROTOCOLLNPAIR)
      SERIAL_PROTOCOLLNPAIR("Incorrect device ID, should be 7C, got ", Device_ID);
    #else
      Serial.print(F("Incorrect device ID, should be 7C, got "));
      Serial.println(device_id, HEX);
    #endif
  } else {
    #if defined (SERIAL_PROTOCOLLNPGM)
      SERIAL_PROTOCOLLNPGM("Device is correct ");
    #else
      Serial.println(F("Device is correct "));
    #endif
  }
  #endif // UI_FRAMEWORK_DEBUG
  delay(400);

  mem_write_8(REG_GPIO, 0x00);                 // Turn OFF Display Enable (GPIO Bit 7);
  mem_write_8(REG_PCLK, 0x00);                 // Turn OFF LCD PCLK
  set_backlight(0x00FA, 0);

  /*
   *  Configure the FT800/810 Registers
   */

  mem_write_16(REG_HCYCLE,  Hcycle);
  mem_write_16(REG_HOFFSET, Hoffset);
  mem_write_16(REG_HSYNC0,  Hsync0);
  mem_write_16(REG_HSYNC1,  Hsync1);
  mem_write_16(REG_VCYCLE,  Vcycle);
  mem_write_16(REG_VOFFSET, Voffset);
  mem_write_16(REG_VSYNC0,  Vsync0);
  mem_write_16(REG_VSYNC1,  Vsync1);
  mem_write_16(REG_HSIZE,   Hsize);
  mem_write_16(REG_VSIZE,   Vsize);
  mem_write_8(REG_SWIZZLE,  Swizzle);
  mem_write_8(REG_PCLK_POL, Pclkpol);
  mem_write_8(REG_CSPREAD,  1);

  #if   defined(USE_PORTRAIT_ORIENTATION)  &&  defined(FLIP_UPSIDE_DOWN)
  mem_write_8(REG_ROTATE, 3);
  #elif defined(USE_PORTRAIT_ORIENTATION)  && !defined(FLIP_UPSIDE_DOWN)
  mem_write_8(REG_ROTATE, 2);
  #elif !defined(USE_PORTRAIT_ORIENTATION) &&  defined(FLIP_UPSIDE_DOWN)
  mem_write_8(REG_ROTATE, 1);
  #else !defined(USE_PORTRAIT_ORIENTATION) && !defined(FLIP_UPSIDE_DOWN)
  mem_write_8(REG_ROTATE, 0);
  #endif

  mem_write_8(REG_TOUCH_MODE, 0x03);           // Configure the Touch Screen
  mem_write_8(REG_TOUCH_ADC_MODE, 0x01);
  mem_write_8(REG_TOUCH_OVERSAMPLE, 0x0F);
  mem_write_16(REG_TOUCH_RZTHRESH, 5000);
  mem_write_8(REG_VOL_SOUND, 0x00);            // Turn Synthesizer Volume Off
  mem_write_8(REG_DLSWAP, 0x02);               // Swap on New Frame

  /*
   *  Turn on the Display         (set DISP high)
   *  Turn on the Audio Amplifier (set GP0 high; on the AO CLCD board, this is tied to the amplifier control)
   */
  #if defined(USE_FTDI_FT800)
    mem_write_8(REG_GPIO_DIR,   GPIO_DISP  | GPIO_GP0);
    mem_write_8(REG_GPIO,       GPIO_DISP  | GPIO_GP0);
  #else
    mem_write_16(REG_GPIOX_DIR, GPIOX_DISP | GPIOX_GP0);
    mem_write_16(REG_GPIOX,     GPIOX_DISP | GPIOX_GP0);
  #endif

  enable();                                   // Turns on Clock by setting PCLK Register to 5
  delay(50);

  CommandFifo::reset();
  delay(50);

  // Set Initial Values for Touch Transform Registers

  mem_write_32(REG_TOUCH_TRANSFORM_A, default_transform_a);
  mem_write_32(REG_TOUCH_TRANSFORM_B, default_transform_b);
  mem_write_32(REG_TOUCH_TRANSFORM_C, default_transform_c);
  mem_write_32(REG_TOUCH_TRANSFORM_D, default_transform_d);
  mem_write_32(REG_TOUCH_TRANSFORM_E, default_transform_e);
  mem_write_32(REG_TOUCH_TRANSFORM_F, default_transform_f);
}

/******************* SOUND HELPER CLASS ************************/

const PROGMEM CLCD::SoundPlayer::sound_t CLCD::SoundPlayer::silence[] = {
  {SILENCE, END_SONG, 0}
};

void CLCD::SoundPlayer::set_volume(uint8_t vol) {
  mem_write_8(REG_VOL_SOUND, vol);
}

void CLCD::SoundPlayer::play(effect_t effect, note_t note) {
  mem_write_16(REG_SOUND, (note << 8) | effect);
  mem_write_8( REG_PLAY,  1);

  #if defined(UI_FRAMEWORK_DEBUG)
    #if defined (SERIAL_PROTOCOLLNPAIR)
      SERIAL_PROTOCOLPAIR("Playing note ", note);
      SERIAL_PROTOCOLLNPAIR(", instrument ", effect);
    #endif
  #endif
}

note_t CLCD::SoundPlayer::frequency_to_midi_note(const uint16_t frequency_hz) {
  const float f0 = 440;
  return note_t(NOTE_A4 + (log(frequency_hz)-log(f0))*12/log(2) + 0.5);
}

// Plays a tone of a given frequency and duration. Since the FTDI FT810 only
// supports MIDI notes, we round down to the nearest note.

void CLCD::SoundPlayer::play_tone(const uint16_t frequency_hz, const uint16_t duration_ms) {
  play(ORGAN, frequency_to_midi_note(frequency_hz));

  // Schedule silence to squelch the note after the duration expires.
  sequence = silence;
  next = tiny_interval_t::tiny_interval(millis() + duration_ms);
}

void CLCD::SoundPlayer::play(const sound_t* seq) {
  sequence = seq;
  // Delaying the start of the sound seems to prevent glitches. Not sure why...
  next     = tiny_interval_t::tiny_interval(millis()+250);
}

bool CLCD::SoundPlayer::is_sound_playing() {
  return mem_read_8( REG_PLAY ) & 0x1;
}

void CLCD::SoundPlayer::onIdle() {
  if(!sequence) return;

  const uint8_t tiny_millis = tiny_interval_t::tiny_interval(millis());
  const bool ready_for_next_note = (next == WAIT) ? !is_sound_playing() : (tiny_millis > next);

  if(ready_for_next_note) {
    const effect_t fx = effect_t(pgm_read_byte_near(&sequence->effect));
    const note_t   nt =   note_t(pgm_read_byte_near(&sequence->note));
    const uint16_t ms = uint32_t(pgm_read_byte_near(&sequence->sixteenths)) * 1000 / 16;

    if(ms == 0 && fx == SILENCE && nt == 0) {
      sequence = 0;
      play(SILENCE, REST);
    } else {
      #if defined(UI_FRAMEWORK_DEBUG)
        #if defined (SERIAL_PROTOCOLLNPAIR)
          SERIAL_PROTOCOLLNPAIR("Scheduling note in ", ms);
        #endif
      #endif
      next =   (ms == WAIT) ? 0       : (tiny_interval_t::tiny_interval(millis() + ms));
      play(fx, (nt == 0)    ? NOTE_C4 : nt);
      sequence++;
    }
  }
}

#endif // FTDI_EVE_TOUCHSCREEN