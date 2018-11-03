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
#pragma once

#include "../inc/MarlinConfig.h"

#ifdef __AVR__
  #define MIXER_ACCU_SIGNED
  typedef uint8_t mixer_color_t;
  typedef int8_t mixer_accu_t;
  #define COLOR_A_MASK 0x80
  #define COLOR_MASK 0x7F
#else
  typedef uint_fast16_t mixer_color_t;
  typedef uint_fast16_t mixer_accu_t;
  #define COLOR_A_MASK 0x8000
  #define COLOR_MASK 0x7FFF
#endif

#ifndef MIXING_VIRTUAL_TOOLS
  #define MIXING_VIRTUAL_TOOLS 1
#endif

#ifdef RETRACT_SYNC_MIXING
  #define NR_MIXING_VIRTUAL_TOOLS (MIXING_VIRTUAL_TOOLS + 1)
  #define MIXER_AUTORETRACT_TOOL MIXING_VIRTUAL_TOOLS
  #if NR_MIXING_VIRTUAL_TOOLS > 254
    #error "MIXING_VIRTUAL_TOOLS must be <= 254!"
  #endif
#else
  #define NR_MIXING_VIRTUAL_TOOLS (MIXING_VIRTUAL_TOOLS)
  #if NR_MIXING_VIRTUAL_TOOLS > 255
    #error "MIXING_VIRTUAL_TOOLS must be <= 255!"
  #endif
#endif

#define MIXER_STEPPER_LOOP(VAR) \
  for (uint_fast8_t VAR = 0; VAR < MIXING_STEPPERS; VAR++)

#define MIXER_BLOCK_DEFINITION mixer_color_t b_color[MIXING_STEPPERS]
#define MIXER_POPULATE_BLOCK() mixer.populate_block(block->b_color)
#define MIXER_STEPPER_SETUP() mixer.stepper_setup(current_block->b_color)

class Mixer {
  public:

  static void init(void); // Populate colors at boot time

  // Used up to Planner level
  static void normalize(const uint8_t tool_index);
  FORCE_INLINE static uint8_t get_current_v_tool(void) { return selected_v_tool; }
  FORCE_INLINE static void T(const uint_fast8_t c) { selected_v_tool = c; }
  FORCE_INLINE static void set_M163_collector(const uint8_t c, const float f) { M163_collector[c] = f; }

  // Used when dealing with blocks
  FORCE_INLINE static void populate_block(mixer_color_t b_color[]) {
    uint_fast8_t j = get_current_v_tool();
    MIXER_STEPPER_LOOP(i) b_color[i] = color[j][i];
  }
  FORCE_INLINE static void stepper_setup(mixer_color_t b_color[]) { MIXER_STEPPER_LOOP(i) s_color[i] = b_color[i]; }

  // Used in Stepper
  FORCE_INLINE static uint8_t get_stepper(void) { return runner; }
  FORCE_INLINE static uint8_t get_next_stepper(void) {
    do {
      if (--runner < 0) runner = MIXING_STEPPERS - 1;
      accu[runner] += s_color[runner];
      if (
        #ifdef MIXER_ACCU_SIGNED
          accu[runner] < 0
        #else
          accu[runner] & COLOR_A_MASK
        #endif
      ) {
        accu[runner] &= COLOR_MASK;
        return runner;
      }
    } while( true );
  }

  private:

  // Used up to Planner level
  static uint_fast8_t  selected_v_tool;
  static float         M163_collector[MIXING_STEPPERS];
  static mixer_color_t color[NR_MIXING_VIRTUAL_TOOLS][MIXING_STEPPERS];

  // Used in Stepper
  static int_fast8_t   runner;
  static mixer_color_t s_color[MIXING_STEPPERS];
  static mixer_accu_t  accu[MIXING_STEPPERS];
};

extern Mixer mixer;
