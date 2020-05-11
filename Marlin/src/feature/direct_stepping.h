/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../inc/MarlinConfig.h"

namespace DirectStepping {

  enum State : char {
    MONITOR, NEWLINE, ADDRESS, SIZE, COLLECT, CHECKSUM, UNFAIL
  };

  enum PageState : uint8_t {
    FREE, WRITING, OK, FAIL
  };

  // Static state used for stepping through direct stepping pages
  struct page_step_state_t {
    // Current page
    uint8_t *page;
    // Current segment
    uint16_t segment_idx;
    // Current steps within segment
    uint8_t segment_steps;
    // Segment delta
    xyze_uint8_t sd;
    // Block delta
    xyze_int_t bd;
  };

  template<typename Cfg>
  class SerialPageManager {
  public:

    typedef typename Cfg::page_idx_t page_idx_t;

    static bool maybe_store_rxd_char(uint8_t c);
    static void write_responses();

    // common methods for page managers
    static void init();
    static uint8_t *get_page(const page_idx_t page_idx);
    static void free_page(const page_idx_t page_idx);

  protected:

    typedef typename Cfg::write_byte_idx_t write_byte_idx_t;

    static State state;
    static volatile bool fatal_error;

    static volatile PageState page_states[Cfg::NUM_PAGES];
    static volatile bool page_states_dirty;
    static millis_t next_response;

    static uint8_t pages[Cfg::NUM_PAGES][Cfg::PAGE_SIZE];
    static uint8_t checksum;
    static write_byte_idx_t write_byte_idx;
    static page_idx_t write_page_idx;
    static write_byte_idx_t write_page_size;

    static void set_page_state(const page_idx_t page_idx, const PageState page_state);
  };

  template<bool b, typename T, typename F> struct TypeSelector { typedef T type;} ;
  template<typename T, typename F> struct TypeSelector<false, T, F> { typedef F type; };

  template <int num_pages, int num_axes, int bits_segment, bool dir, int segments>
  struct config_t {
    static constexpr char CONTROL_CHAR  = '!';

    static constexpr int NUM_PAGES      = num_pages;
    static constexpr int NUM_AXES       = num_axes;
    static constexpr int BITS_SEGMENT   = bits_segment;
    static constexpr int DIRECTIONAL    = dir ? 1 : 0;
    static constexpr int SEGMENTS       = segments;

    static constexpr int RAW            = (BITS_SEGMENT == 1) ? 1 : 0;
    static constexpr int NUM_SEGMENTS   = 1 << BITS_SEGMENT;
    static constexpr int SEGMENT_STEPS  = 1 << (BITS_SEGMENT - DIRECTIONAL - RAW);
    static constexpr int TOTAL_STEPS    = SEGMENT_STEPS * SEGMENTS;
    static constexpr int PAGE_SIZE      = (NUM_AXES * BITS_SEGMENT * SEGMENTS) / 8;

    static constexpr millis_t RESPONSE_INTERVAL_MS = 50;

    typedef typename TypeSelector<(PAGE_SIZE>256), uint16_t, uint8_t>::type write_byte_idx_t;
    typedef typename TypeSelector<(NUM_PAGES>256), uint16_t, uint8_t>::type page_idx_t;
  };

  template <uint8_t num_pages>
  using SP_4x4D_128 = config_t<num_pages, 4, 4, true,  128>;

  template <uint8_t num_pages>
  using SP_4x2_256  = config_t<num_pages, 4, 2, false, 256>;

  template <uint8_t num_pages>
  using SP_4x1_512  = config_t<num_pages, 4, 1, false, 512>;

  // configured types
  typedef STEPPER_PAGE_FORMAT<STEPPER_PAGES> Config;

  template class PAGE_MANAGER<Config>;
  typedef PAGE_MANAGER<Config> PageManager;
};

#define SP_4x4D_128 1
//#define SP_4x4_128 2
//#define SP_4x2D_256 3
#define SP_4x2_256 4
#define SP_4x1_512 5

typedef typename DirectStepping::Config::page_idx_t page_idx_t;

// TODO: use config
typedef DirectStepping::page_step_state_t page_step_state_t;

extern const uint8_t segment_table[DirectStepping::Config::NUM_SEGMENTS][DirectStepping::Config::SEGMENT_STEPS];
extern DirectStepping::PageManager page_manager;
