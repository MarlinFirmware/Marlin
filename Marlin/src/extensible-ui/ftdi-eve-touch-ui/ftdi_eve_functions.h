/**************************
 * ftdi_eve_functions.cpp *
 **************************/

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

  /****************************************************************************
  *                       FUNCTION MAP                                        *
  *                                                                           *
  * SPI and FT800/810 Commands                                                *
  *                                                                           *
  * CLCD::spi_select()                 Set CS line to 0                       *
  * CLCD::spi_deselect()               Set CS Line to 1                       *
  * CLCD::reset()                      Toggle FT800/810 Power Down Line 50 ms *
  * CLCD::spi_init()                   Configure I/O Lines for SPI            *
  * CLCD::spi_transfer()               Send/Receive 1 SPI Byte                *
  * CLCD::init()                       Set FT800/810 Registers                *
  * CLCD::enable()                     Turn On FT800/810 PCLK                 *
  * CLCD::disable()                    Turn Off FT8880/810 PCLK               *
  * CLCD::set_backlight()              Set LCD Backlight Level                *
  *                                                                           *
  * MEMORY READ FUNCTIONS                                                     *
  *                                                                           *
  * CLCD::mem_read_addr()              Send 32-Bit Address                    *
  * CLCD::mem_read_8()                 Read 1 Byte                            *
  * CLCD::mem_read_16()                Read 2 Bytes                           *
  * CLCD::mem_read_32()                Read 4 Bytes                           *
  *                                                                           *
  * MEMORY WRITE FUNCTIONS                                                    *
  *                                                                           *
  * CLCD::mem_write_addr()             Send 24-Bit Address                    *
  * CLCD::mem_write_8()                Write 1 Byte                           *
  * CLCD::mem_write_16()               Write 2 Bytes                          *
  * CLCD::mem_write_32()               Write 4 Bytes                          *
  *                                                                           *
  * HOST COMMAND FUNCTION                                                     *
  *                                                                           *
  * CLCD::host_cmd()                   Send 24-Bit Host Command               *
  *                                                                           *
  * COMMAND BUFFER FUNCTIONS                                                  *
  *                                                                           *
  * CLCD::cmd()                        Send 32-Bit Value(4 Bytes)CMD Buffer   *
  * CLCD::cmd()                        Send Data Structure with 32-Bit Cmd    *
  * CLCD::str()                        Send Text String in 32-Bit Multiples   *

  *                                                                           *
  * FT800/810 GRAPHIC COMMANDS                                                *
  *                                                                           *
  * class CLCD:CommandFifo {}          Class to control Cmd FIFO              *

  * CommandFifo::start()               Wait for CP finish - Set FIFO Ptr      *
  * CommandFifo::execute()             Set REG_CMD_WRITE and start CP         *
  * CommandFifo::reset()               Set Cmd Buffer Pointers to 0           *
  *
  * CommandFifo::clear_color()         Set Clear Screen Color                 *
  * CommandFifo::clear()               Clear Color-Stencil-Tag Buffer(s)      *
  * CommandFifo::color()               Set Current Color                      *
  * CommandFifo::set_foreground_color  Set Graphic Item Foreground Color      *
  * CommandFifo::set_background_color  Set Graphic Item Background Color      *
  * CommandFifo::bitmap_source()       Set RAM Address of Bitmap Data         *
  * CommandFifo::bitmap_layout()       Configure Bitmap Format and Layout     *
  * CommandFifo::bitmap_size()         Set Bitmap Dimensions                  *
  * CommandFifo::bitmap_handle()       Assign Handle ID to a Bitmap           *
  * CommandFifo::begin()               Begin Drawing a Primative              *
  * CommandFifo::mem_copy()            Copy a Block of Memory                 *
  * CommandFifo::append()              Append Commands to Current DL          *
  * CommandFifo::gradient_color()      Set 3D Button Highlight Color          *
  * CommandFifo::vertex_2ii()          Set Coordinates of Graphics Primative  *
  * CommandFifo::draw_button()         Draw Button with Bulk Write            *
  * CommandFifo::draw_text()           Draw Text with Bulk Write              *
  *
  *
  */

 /**************************************************
  * RAM_G Graphics RAM Allocation                  *
  *                                                *
  * Address    Use                                 *
  *                                                *
  *    8000    Extruder Bitmap                     *
  *    8100    Bed Heat Bitmap                     *
  *    8200    Fan Bitmap                          *
  *    8300    Thumb Drive Symbol Bitmap           *
  *   35000    Static DL Space (FT800)             *
  *   F5000    Static DL Space (FT810)             *
  **************************************************/

#ifndef _FTDI_EVE_FUNCTIONS_H_
#define _FTDI_EVE_FUNCTIONS_H_

#include "../ui_config.h"

#define DL_CACHE_START RAM_G_SIZE - 0xFFFF

// Uncomment the following to disable the DL caching mechanism
// (note, this will keep the LCD status message from working)
//#define DL_CACHE_DISABLED

#define FORCEDINLINE __attribute__((always_inline))

using namespace FTDI;

typedef const __FlashStringHelper *progmem_str;

class CLCD {
  private:
    static void     spi_init (void);
    static void     spi_select (void);
    static void     spi_deselect (void);
    static uint8_t  spi_transfer (uint8_t spiOutByte);

    static void     mem_read_addr  (uint32_t reg_address);
    static void     mem_write_addr (uint32_t reg_address);
    static void     mem_read_bulk  (uint32_t reg_address, uint8_t *data, uint16_t len);
    static void     mem_write_bulk (uint32_t reg_address, const void *data, uint16_t len, uint8_t padding = 0);
    static void     mem_write_bulk (uint32_t reg_address, progmem_str str, uint16_t len, uint8_t padding = 0);

  public:
    static uint8_t  mem_read_8   (uint32_t reg_address);
    static uint16_t mem_read_16  (uint32_t reg_address);
    static uint32_t mem_read_32  (uint32_t reg_address);
    static void     mem_write_8  (uint32_t reg_address, uint8_t w_data);
    static void     mem_write_16 (uint32_t reg_address, uint16_t w_data);
    static void     mem_write_32 (uint32_t reg_address, uint32_t w_data);

    static inline uint32_t pack_rgb(uint8_t r, uint8_t g, uint8_t b) {
      return (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
    }

  public:
    typedef struct {
      const uint8_t  format;
      const uint16_t linestride;
      const uint8_t  filter;
      const uint8_t  wrapx;
      const uint8_t  wrapy;
      const uint32_t RAMG_addr;
      const uint16_t width;
      const uint16_t height;
    } bitmap_info_t;

    class CommandFifo;
    class SoundPlayer;
    class DLCache;

  public:
    static void init (void);
    static void turn_on_backlight (void);
    static void reset (void);
    static void test_pulse(void);
    static void enable (void);
    static void disable (void);
    static void set_backlight (uint16_t freq, uint8_t duty);
    static void host_cmd (unsigned char host_command, unsigned char byte2);

    static void get_font_metrics (uint8_t font, struct FontMetrics &fm);
    static void flash_write_rgb332_bitmap (uint32_t mem_address, const unsigned char* p_rgb332_array, uint16_t num_bytes);

    static uint8_t get_tag () {return mem_read_8(REG_TOUCH_TAG);}
    static bool is_touching () {return (mem_read_32(REG_TOUCH_DIRECT_XY) & 0x80000000) == 0;}
};

/********************************* FT800/810 Commands *********************************/

struct FontMetrics {
  uint8_t   char_widths[128];
  uint32_t  font_format;
  uint32_t  font_stride;
  uint32_t  font_width;
  uint32_t  font_height;
  uint32_t  font_ptr;
};

/******************* FT800/810 Graphic Commands *********************************/

class CLCD::CommandFifo {
  protected:
    static uint32_t get_reg_cmd_write();
    static uint32_t get_reg_cmd_read();

    #if defined(USE_FTDI_FT800)
      static uint32_t command_write_ptr;
      template <class T> void _write_unaligned(T data, uint16_t len);
    #else
      uint32_t getRegCmdBSpace();
    #endif
    void start(void);

  public:
    template <class T> void write(T data, uint16_t len);

  public:
    CommandFifo() {start();}

    static void reset (void);
    static bool is_idle();
    static void wait_until_idle();

    void execute(void);

    void cmd (uint32_t cmd32);
    void cmd (void* data, uint16_t len);
    void str (const char * const data);
    void str (progmem_str data);
    void set_clear_color (uint32_t rgb);
    void clear (bool clr, bool stl, bool tag);
    void set_color (uint32_t rgb);
    void set_foreground_color (uint32_t rgb);
    void set_background_color (uint32_t rgb);
    void set_tag (uint8_t Tag);
    void bitmap_source (uint32_t ram_g_addr);

    // The following functions *must* be inlined since we are relying on the compiler to do
    // substitution of the constants from the data structure rather than actually storing
    // it in PROGMEM (which would fail, since we are not using pgm_read_near to read them).
    // Plus, by inlining, all the equations are evaluated at compile-time as everything
    // should be a constant.

    FORCEDINLINE void bitmap_source (const bitmap_info_t& info) {bitmap_source (info.RAMG_addr);};
    FORCEDINLINE void bitmap_layout (const bitmap_info_t& info) {bitmap_layout (info.format, info.linestride, info.height);};
    FORCEDINLINE void bitmap_size(const bitmap_info_t& info)    {bitmap_size (info.filter, info.wrapx, info.wrapy, info.width, info.height);}
    FORCEDINLINE void draw_button_icon(int16_t x, int16_t y, int16_t w, int16_t h, const bitmap_info_t& info, const float scale = 1) {
      begin(BEGIN_BITMAPS);
      if(scale != 1) {
        cmd(BITMAP_TRANSFORM_A | uint32_t(float(256)/scale) & 0xFFFF);
        cmd(BITMAP_TRANSFORM_E | uint32_t(float(256)/scale) & 0xFFFF);
      }
      bitmap_size(info.filter, info.wrapx, info.wrapy, info.width*scale, info.height*scale);
      vertex_2f((x + w/2 - info.width*scale/2)*16, (y + h/2 - info.height*scale/2)*16);
      if(scale != 1) {
        cmd(BITMAP_TRANSFORM_A | 256);
        cmd(BITMAP_TRANSFORM_E | 256);
      }
    }

    template<typename T> FORCEDINLINE
    void draw_button_text(int16_t x, int16_t y, int16_t w, int16_t h, T text, int16_t font, uint16_t options = OPT_CENTER) {
      draw_text(
        x + ((options & OPT_CENTERX) ? w/2 : ((options & OPT_RIGHTX) ? w : 0)),
        y + ((options & OPT_CENTERY) ? h/2 : h),
        text, font, options);
    }

    void bitmap_layout (uint8_t format, uint16_t linestride, uint16_t height);
    void bitmap_size(uint8_t filter, uint8_t wrapx, uint8_t wrapy, uint16_t width, uint16_t height);
    void bitmap_handle (uint16_t Handle);
    void begin (uint32_t Primitive);
    void vertex_2f  (uint16_t X_Coord, uint16_t Y_Coord);
    void vertex_2ii (uint16_t X_Coord, uint16_t Y_Coord, uint8_t B_Handle, uint8_t Cell);

    template<typename T>
    void draw_button(int16_t x, int16_t y, int16_t w, int16_t h, T text, int16_t font,  uint16_t option) {
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
      str(text);
    }

    template<typename T>
    void draw_text(int16_t x, int16_t y, T text, int16_t font,  uint16_t options) {
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
      str(text);
    }

    void draw_clock (int16_t x, int16_t y, int16_t r, uint16_t option, int16_t h, int16_t m, int16_t s, int16_t ms);
    void draw_progress_bar (int16_t x, int16_t y, int16_t w, int16_t h, int16_t val, int16_t range);
    void draw_slider (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t options, uint16_t val, uint16_t range);
    void mem_cpy (uint32_t dst, uint32_t src, uint32_t size);
    void append (uint32_t ptr, uint32_t size);
};

/******************* DISPLAY LIST CACHE MANAGEMENT ************************/

/* The Display List Cache mechanism stores the display list corresponding
 * to a menu into RAM_G so that on subsequent calls drawing the menu does
 * not require as much SPI traffic. Dynamic content, such as indicators,
 * should not be cached.
 *
 * The DLCache can be used like so:
 *
 *   void some_function() {
 *     CLCD::DLCache dlcache(UNIQUE_ID);
 *
 *     if(dlcache.hasData()) {
 *        dlcache.append();
 *     } else {
 *        // Add stuff to the DL
 *        dlcache.store();
 *     }
 *
 *
 * Layout of Cache memory:
 *
 * The cache memory begins with a table at
 * DL_CACHE_START: each table entry contains
 * an address and size for a cached DL slot.
 *
 * Immediately following the table is the
 * DL_FREE_ADDR, which points to free cache
 * space; following this is occupied DL space,
 * and after that free space that is yet to
 * be used.
 *
 *  location        data        sizeof
 *
 *  DL_CACHE_START  slot0_addr     4
 *                  slot0_size     4
 *                  slot1_addr     4
 *                  slot1_size     4
 *                      ...
 *                  slotN_addr     4
 *                  slotN_size     4
 *  DL_FREE_ADDR    dl_free_ptr    4
 *                  cached data
 *                      ...
 *  dl_free_ptr     empty space
 *                      ...
 */

class CLCD::DLCache {
  private:
    uint8_t  dl_slot;
    uint32_t dl_addr;
    uint16_t dl_size;

    void load_slot();
    static void save_slot(uint8_t dl_slot, uint32_t dl_addr, uint32_t dl_size);

  public:
    static void init();

    DLCache(uint8_t slot) {
      dl_slot = slot;
      load_slot();
    }

    bool has_data();
    bool store(uint32_t num_bytes = 0);
    void append();
};

#define DL_CACHE_SLOTS   250
#define DL_FREE_ADDR     DL_CACHE_START + DL_CACHE_SLOTS * 8

/******************* TINY INTERVAL CLASS ***********************/

/* tiny_interval() downsamples a 32-bit millis() value
   into a 8-bit value which can record periods of
   a few seconds with a rougly 1/16th of second
   resolution. This allows us to measure small
   intervals without needing to use four-byte counters.

   However, dues to wrap-arounds, this class may
   have a burst of misfires every 16 seconds or so and
   thus should only be used where this is harmless and
   memory savings outweigh accuracy.
 */
class tiny_interval_t {
  private:
    uint8_t end;
  public:
    static inline uint8_t tiny_interval(uint32_t ms) {
      return uint8_t(ms / 64);
    }

    inline void wait_for(uint32_t ms) {
      uint32_t now = millis();
      end = tiny_interval(now + ms);
      if(tiny_interval(now + ms*2) < end) {
        // Avoid special case where timer
        // might get wedged and stop firing.
        end = 0;
      }
    }

    inline bool elapsed() {
      uint8_t now = tiny_interval(millis());
      if(now > end) {
        return true;
      } else {
        return false;
      }
    }
};

/******************* SOUND HELPER CLASS ************************/

class CLCD::SoundPlayer {
  public:
    struct sound_t {
      effect_t  effect;      // The sound effect number
      note_t    note;        // The MIDI note value
      uint16_t  sixteenths;  // Duration of note, in sixteeths of a second, or zero to play to completion
    };

    const uint8_t WAIT = 0;

    static const PROGMEM sound_t silence[];

  private:
    const sound_t *sequence;
    uint8_t       next;

    note_t frequency_to_midi_note(const uint16_t frequency);

  public:
    static void set_volume(uint8_t volume);
    static void play(effect_t effect, note_t note = NOTE_C4);
    static bool is_sound_playing();

    void play(const sound_t* seq);
    void play_tone(const uint16_t frequency_hz, const uint16_t duration_ms);
    bool has_more_notes() {return sequence != 0;};

    void onIdle();
};

#endif // _FTDI_EVE_FUNCTIONS_H_