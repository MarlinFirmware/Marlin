/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef UNIFIED_BED_LEVELING_H
#define UNIFIED_BED_LEVELING_H

#include "../../../Marlin.h"
#include "../../../module/planner.h"
#include "../../../module/motion.h"

#define UBL_VERSION "1.01"
#define UBL_OK false
#define UBL_ERR true

#define USE_NOZZLE_AS_REFERENCE 0
#define USE_PROBE_AS_REFERENCE 1

typedef struct {
  int8_t x_index, y_index;
  float distance; // When populated, the distance from the search location
} mesh_index_pair;

// ubl.cpp

void bit_clear(uint16_t bits[16], const uint8_t x, const uint8_t y);
void bit_set(uint16_t bits[16], const uint8_t x, const uint8_t y);
bool is_bit_set(uint16_t bits[16], const uint8_t x, const uint8_t y);

// ubl_motion.cpp

void debug_current_and_destination(const char * const title);

// ubl_G29.cpp

enum MeshPointType { INVALID, REAL, SET_IN_BITMAP };

// External references

char *ftostr43sign(const float&, char);
bool ubl_lcd_clicked();

extern uint8_t ubl_cnt;

///////////////////////////////////////////////////////////////////////////////////////////////////////

#if ENABLED(ULTRA_LCD)
  extern char lcd_status_message[];
  void lcd_quick_feedback();
#endif

#define MESH_X_DIST (float(UBL_MESH_MAX_X - (UBL_MESH_MIN_X)) / float(GRID_MAX_POINTS_X - 1))
#define MESH_Y_DIST (float(UBL_MESH_MAX_Y - (UBL_MESH_MIN_Y)) / float(GRID_MAX_POINTS_Y - 1))

typedef struct {
  bool active = false;
  float z_offset = 0.0;
  int8_t storage_slot = -1;
} ubl_state;

class unified_bed_leveling {
  private:

    static float last_specified_z;

    static int    g29_verbose_level,
                  g29_phase_value,
                  g29_repetition_cnt,
                  g29_storage_slot,
                  g29_map_type;
    static bool   g29_c_flag, g29_x_flag, g29_y_flag;
    static float  g29_x_pos, g29_y_pos,
                  g29_card_thickness,
                  g29_constant;

    #if HAS_BED_PROBE
      static int  g29_grid_size;
    #endif

    #if ENABLED(UBL_G26_MESH_VALIDATION)
      static float   g26_extrusion_multiplier,
                     g26_retraction_multiplier,
                     g26_nozzle,
                     g26_filament_diameter,
                     g26_prime_length,
                     g26_x_pos, g26_y_pos,
                     g26_ooze_amount,
                     g26_layer_height;
      static int16_t g26_bed_temp,
                     g26_hotend_temp,
                     g26_repeats;
      static int8_t  g26_prime_flag;
      static bool    g26_continue_with_closest, g26_keep_heaters_on;
    #endif

    static float measure_point_with_encoder();
    static float measure_business_card_thickness(float);
    static bool g29_parameter_parsing();
    static void find_mean_mesh_height();
    static void shift_mesh_height();
    static void probe_entire_mesh(const float &lx, const float &ly, const bool do_ubl_mesh_map, const bool stow_probe, bool do_furthest);
    static void manually_probe_remaining_mesh(const float&, const float&, const float&, const float&, const bool);
    static void tilt_mesh_based_on_3pts(const float &z1, const float &z2, const float &z3);
    static void tilt_mesh_based_on_probed_grid(const bool do_ubl_mesh_map);
    static void g29_what_command();
    static void g29_eeprom_dump();
    static void g29_compare_current_mesh_to_stored_mesh();
    static void fine_tune_mesh(const float &lx, const float &ly, const bool do_ubl_mesh_map);
    static bool smart_fill_one(const uint8_t x, const uint8_t y, const int8_t xdir, const int8_t ydir);
    static void smart_fill_mesh();

    #if ENABLED(UBL_G26_MESH_VALIDATION)
      static bool exit_from_g26();
      static bool parse_G26_parameters();
      static void G26_line_to_destination(const float &feed_rate);
      static mesh_index_pair find_closest_circle_to_print(const float&, const float&);
      static bool look_for_lines_to_connect();
      static bool turn_on_heaters();
      static bool prime_nozzle();
      static void retract_filament(const float where[XYZE]);
      static void recover_filament(const float where[XYZE]);
      static void print_line_from_here_to_there(const float&, const float&, const float&, const float&, const float&, const float&);
      static void move_to(const float&, const float&, const float&, const float&);
      inline static void move_to(const float where[XYZE], const float &de) { move_to(where[X_AXIS], where[Y_AXIS], where[Z_AXIS], de); }
    #endif

  public:

    static void echo_name();
    static void report_state();
    static void save_ubl_active_state_and_disable();
    static void restore_ubl_active_state_and_leave();
    static void display_map(const int);
    static mesh_index_pair find_closest_mesh_point_of_type(const MeshPointType, const float&, const float&, const bool, uint16_t[16], bool);
    static void reset();
    static void invalidate();
    static void set_all_mesh_points_to_value(float);
    static bool sanity_check();

    static void G29() _O0;                          // O0 for no optimization
    static void smart_fill_wlsf(const float &) _O2; // O2 gives smaller code than Os on A2560

    #if ENABLED(UBL_G26_MESH_VALIDATION)
      static void G26();
    #endif

    static ubl_state state;

    static float z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

    // 15 is the maximum nubmer of grid points supported + 1 safety margin for now,
    // until determinism prevails
    constexpr static float _mesh_index_to_xpos[16] PROGMEM = {
                              UBL_MESH_MIN_X +  0 * (MESH_X_DIST), UBL_MESH_MIN_X +  1 * (MESH_X_DIST),
                              UBL_MESH_MIN_X +  2 * (MESH_X_DIST), UBL_MESH_MIN_X +  3 * (MESH_X_DIST),
                              UBL_MESH_MIN_X +  4 * (MESH_X_DIST), UBL_MESH_MIN_X +  5 * (MESH_X_DIST),
                              UBL_MESH_MIN_X +  6 * (MESH_X_DIST), UBL_MESH_MIN_X +  7 * (MESH_X_DIST),
                              UBL_MESH_MIN_X +  8 * (MESH_X_DIST), UBL_MESH_MIN_X +  9 * (MESH_X_DIST),
                              UBL_MESH_MIN_X + 10 * (MESH_X_DIST), UBL_MESH_MIN_X + 11 * (MESH_X_DIST),
                              UBL_MESH_MIN_X + 12 * (MESH_X_DIST), UBL_MESH_MIN_X + 13 * (MESH_X_DIST),
                              UBL_MESH_MIN_X + 14 * (MESH_X_DIST), UBL_MESH_MIN_X + 15 * (MESH_X_DIST)
                            };

    constexpr static float _mesh_index_to_ypos[16] PROGMEM = {
                              UBL_MESH_MIN_Y +  0 * (MESH_Y_DIST), UBL_MESH_MIN_Y +  1 * (MESH_Y_DIST),
                              UBL_MESH_MIN_Y +  2 * (MESH_Y_DIST), UBL_MESH_MIN_Y +  3 * (MESH_Y_DIST),
                              UBL_MESH_MIN_Y +  4 * (MESH_Y_DIST), UBL_MESH_MIN_Y +  5 * (MESH_Y_DIST),
                              UBL_MESH_MIN_Y +  6 * (MESH_Y_DIST), UBL_MESH_MIN_Y +  7 * (MESH_Y_DIST),
                              UBL_MESH_MIN_Y +  8 * (MESH_Y_DIST), UBL_MESH_MIN_Y +  9 * (MESH_Y_DIST),
                              UBL_MESH_MIN_Y + 10 * (MESH_Y_DIST), UBL_MESH_MIN_Y + 11 * (MESH_Y_DIST),
                              UBL_MESH_MIN_Y + 12 * (MESH_Y_DIST), UBL_MESH_MIN_Y + 13 * (MESH_Y_DIST),
                              UBL_MESH_MIN_Y + 14 * (MESH_Y_DIST), UBL_MESH_MIN_Y + 15 * (MESH_Y_DIST)
                            };

    static bool g26_debug_flag, has_control_of_lcd_panel;

    #if ENABLED(ULTRA_LCD)
      static bool lcd_map_control;
    #endif

    static volatile int encoder_diff; // Volatile because it's changed at interrupt time.

    unified_bed_leveling();

    FORCE_INLINE static void set_z(const int8_t px, const int8_t py, const float &z) { z_values[px][py] = z; }

    static int8_t get_cell_index_x(const float &x) {
      const int8_t cx = (x - (UBL_MESH_MIN_X)) * (1.0 / (MESH_X_DIST));
      return constrain(cx, 0, (GRID_MAX_POINTS_X) - 1);   // -1 is appropriate if we want all movement to the X_MAX
    }                                                     // position. But with this defined this way, it is possible
                                                          // to extrapolate off of this point even further out. Probably
                                                          // that is OK because something else should be keeping that from
                                                          // happening and should not be worried about at this level.
    static int8_t get_cell_index_y(const float &y) {
      const int8_t cy = (y - (UBL_MESH_MIN_Y)) * (1.0 / (MESH_Y_DIST));
      return constrain(cy, 0, (GRID_MAX_POINTS_Y) - 1);   // -1 is appropriate if we want all movement to the Y_MAX
    }                                                     // position. But with this defined this way, it is possible
                                                          // to extrapolate off of this point even further out. Probably
                                                          // that is OK because something else should be keeping that from
                                                          // happening and should not be worried about at this level.

    static int8_t find_closest_x_index(const float &x) {
      const int8_t px = (x - (UBL_MESH_MIN_X) + (MESH_X_DIST) * 0.5) * (1.0 / (MESH_X_DIST));
      return WITHIN(px, 0, GRID_MAX_POINTS_X - 1) ? px : -1;
    }

    static int8_t find_closest_y_index(const float &y) {
      const int8_t py = (y - (UBL_MESH_MIN_Y) + (MESH_Y_DIST) * 0.5) * (1.0 / (MESH_Y_DIST));
      return WITHIN(py, 0, GRID_MAX_POINTS_Y - 1) ? py : -1;
    }

    /**
     *                           z2   --|
     *                 z0        |      |
     *                  |        |      + (z2-z1)
     *   z1             |        |      |
     * ---+-------------+--------+--  --|
     *   a1            a0        a2
     *    |<---delta_a---------->|
     *
     *  calc_z0 is the basis for all the Mesh Based correction. It is used to
     *  find the expected Z Height at a position between two known Z-Height locations.
     *
     *  It is fairly expensive with its 4 floating point additions and 2 floating point
     *  multiplications.
     */
    FORCE_INLINE static float calc_z0(const float &a0, const float &a1, const float &z1, const float &a2, const float &z2) {
      return z1 + (z2 - z1) * (a0 - a1) / (a2 - a1);
    }

    /**
     * z_correction_for_x_on_horizontal_mesh_line is an optimization for
     * the case where the printer is making a vertical line that only crosses horizontal mesh lines.
     */
    inline static float z_correction_for_x_on_horizontal_mesh_line(const float &lx0, const int x1_i, const int yi) {
      if (!WITHIN(x1_i, 0, GRID_MAX_POINTS_X - 2) || !WITHIN(yi, 0, GRID_MAX_POINTS_Y - 1)) {
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            serialprintPGM( !WITHIN(x1_i, 0, GRID_MAX_POINTS_X - 1) ? PSTR("x1l_i") : PSTR("yi") );
            SERIAL_ECHOPAIR(" out of bounds in z_correction_for_x_on_horizontal_mesh_line(lx0=", lx0);
            SERIAL_ECHOPAIR(",x1_i=", x1_i);
            SERIAL_ECHOPAIR(",yi=", yi);
            SERIAL_CHAR(')');
            SERIAL_EOL();
          }
        #endif
        return NAN;
      }

      const float xratio = (RAW_X_POSITION(lx0) - mesh_index_to_xpos(x1_i)) * (1.0 / (MESH_X_DIST)),
                  z1 = z_values[x1_i][yi];

      return z1 + xratio * (z_values[x1_i + 1][yi] - z1);
    }

    //
    // See comments above for z_correction_for_x_on_horizontal_mesh_line
    //
    inline static float z_correction_for_y_on_vertical_mesh_line(const float &ly0, const int xi, const int y1_i) {
      if (!WITHIN(xi, 0, GRID_MAX_POINTS_X - 1) || !WITHIN(y1_i, 0, GRID_MAX_POINTS_Y - 2)) {
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            serialprintPGM( !WITHIN(xi, 0, GRID_MAX_POINTS_X - 1) ? PSTR("xi") : PSTR("yl_i") );
            SERIAL_ECHOPAIR(" out of bounds in z_correction_for_y_on_vertical_mesh_line(ly0=", ly0);
            SERIAL_ECHOPAIR(", xi=", xi);
            SERIAL_ECHOPAIR(", y1_i=", y1_i);
            SERIAL_CHAR(')');
            SERIAL_EOL();
          }
        #endif
        return NAN;
      }

      const float yratio = (RAW_Y_POSITION(ly0) - mesh_index_to_ypos(y1_i)) * (1.0 / (MESH_Y_DIST)),
                  z1 = z_values[xi][y1_i];

      return z1 + yratio * (z_values[xi][y1_i + 1] - z1);
    }

    /**
     * This is the generic Z-Correction. It works anywhere within a Mesh Cell. It first
     * does a linear interpolation along both of the bounding X-Mesh-Lines to find the
     * Z-Height at both ends. Then it does a linear interpolation of these heights based
     * on the Y position within the cell.
     */
    static float get_z_correction(const float &lx0, const float &ly0) {
      const int8_t cx = get_cell_index_x(RAW_X_POSITION(lx0)),
                   cy = get_cell_index_y(RAW_Y_POSITION(ly0));

      if (!WITHIN(cx, 0, GRID_MAX_POINTS_X - 2) || !WITHIN(cy, 0, GRID_MAX_POINTS_Y - 2)) {

        SERIAL_ECHOPAIR("? in get_z_correction(lx0=", lx0);
        SERIAL_ECHOPAIR(", ly0=", ly0);
        SERIAL_CHAR(')');
        SERIAL_EOL();

        #if ENABLED(ULTRA_LCD)
          strcpy(lcd_status_message, "get_z_correction() indexes out of range.");
          lcd_quick_feedback();
        #endif
        return NAN; // this used to return state.z_offset
      }

      const float z1 = calc_z0(RAW_X_POSITION(lx0),
                               mesh_index_to_xpos(cx), z_values[cx][cy],
                               mesh_index_to_xpos(cx + 1), z_values[cx + 1][cy]);

      const float z2 = calc_z0(RAW_X_POSITION(lx0),
                               mesh_index_to_xpos(cx), z_values[cx][cy + 1],
                               mesh_index_to_xpos(cx + 1), z_values[cx + 1][cy + 1]);

      float z0 = calc_z0(RAW_Y_POSITION(ly0),
                         mesh_index_to_ypos(cy), z1,
                         mesh_index_to_ypos(cy + 1), z2);

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(MESH_ADJUST)) {
          SERIAL_ECHOPAIR(" raw get_z_correction(", lx0);
          SERIAL_CHAR(',');
          SERIAL_ECHO(ly0);
          SERIAL_ECHOPGM(") = ");
          SERIAL_ECHO_F(z0, 6);
        }
      #endif

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(MESH_ADJUST)) {
          SERIAL_ECHOPGM(" >>>---> ");
          SERIAL_ECHO_F(z0, 6);
          SERIAL_EOL();
        }
      #endif

      if (isnan(z0)) { // if part of the Mesh is undefined, it will show up as NAN
        z0 = 0.0;      // in ubl.z_values[][] and propagate through the
                       // calculations. If our correction is NAN, we throw it out
                       // because part of the Mesh is undefined and we don't have the
                       // information we need to complete the height correction.

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(MESH_ADJUST)) {
            SERIAL_ECHOPAIR("??? Yikes!  NAN in get_z_correction(", lx0);
            SERIAL_CHAR(',');
            SERIAL_ECHO(ly0);
            SERIAL_CHAR(')');
            SERIAL_EOL();
          }
        #endif
      }
      return z0; // there used to be a +state.z_offset on this line
    }

    /**
     * This function sets the Z leveling fade factor based on the given Z height,
     * only re-calculating when necessary.
     *
     *  Returns 1.0 if planner.z_fade_height is 0.0.
     *  Returns 0.0 if Z is past the specified 'Fade Height'.
     */
    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      static inline float fade_scaling_factor_for_z(const float &lz) {
        if (planner.z_fade_height == 0.0) return 1.0;
        static float fade_scaling_factor = 1.0;
        const float rz = RAW_Z_POSITION(lz);
        if (last_specified_z != rz) {
          last_specified_z = rz;
          fade_scaling_factor =
            rz < planner.z_fade_height
              ? 1.0 - (rz * planner.inverse_z_fade_height)
              : 0.0;
        }
        return fade_scaling_factor;
      }
    #else
      FORCE_INLINE static float fade_scaling_factor_for_z(const float &lz) { return 1.0; }
    #endif

    FORCE_INLINE static float mesh_index_to_xpos(const uint8_t i) {
      return i < GRID_MAX_POINTS_X ? pgm_read_float(&_mesh_index_to_xpos[i]) : UBL_MESH_MIN_X + i * (MESH_X_DIST);
    }

    FORCE_INLINE static float mesh_index_to_ypos(const uint8_t i) {
      return i < GRID_MAX_POINTS_Y ? pgm_read_float(&_mesh_index_to_ypos[i]) : UBL_MESH_MIN_Y + i * (MESH_Y_DIST);
    }

    static bool prepare_segmented_line_to(const float ltarget[XYZE], const float &feedrate);
    static void line_to_destination_cartesian(const float &fr, uint8_t e);

    #define _CMPZ(a,b) (z_values[a][b] == z_values[a][b+1])
    #define CMPZ(a) (_CMPZ(a, 0) && _CMPZ(a, 1))
    #define ZZER(a) (z_values[a][0] == 0)

    FORCE_INLINE bool mesh_is_valid() {
      return !(
        (    CMPZ(0) && CMPZ(1) && CMPZ(2) // adjacent z values all equal?
          && ZZER(0) && ZZER(1) && ZZER(2) // all zero at the edge?
        )
        || isnan(z_values[0][0])
      );
    }

}; // class unified_bed_leveling

extern unified_bed_leveling ubl;

#endif // UNIFIED_BED_LEVELING_H
