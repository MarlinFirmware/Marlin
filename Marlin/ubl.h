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

#include "MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)

  #include "Marlin.h"
  #include "math.h"
  #include "vector_3.h"

  #define UBL_VERSION "1.00"
  #define UBL_OK false
  #define UBL_ERR true

  typedef struct {
    int8_t x_index, y_index;
    float distance; // When populated, the distance from the search location
  } mesh_index_pair;

  enum MeshPointType { INVALID, REAL, SET_IN_BITMAP };

  void dump(char * const str, const float &f);
  bool ubl_lcd_clicked();
  void probe_entire_mesh(const float&, const float&, const bool, const bool, const bool);
  void debug_current_and_destination(const char *title);
  void ubl_line_to_destination(const float&, uint8_t);
  void manually_probe_remaining_mesh(const float&, const float&, const float&, const float&, const bool);
  vector_3 tilt_mesh_based_on_3pts(const float&, const float&, const float&);
  float measure_business_card_thickness(const float&);
  mesh_index_pair find_closest_mesh_point_of_type(const MeshPointType, const float&, const float&, const bool, unsigned int[16], bool);
  void find_mean_mesh_height();
  void shift_mesh_height();
  bool g29_parameter_parsing();
  void g29_what_command();
  void g29_eeprom_dump();
  void g29_compare_current_mesh_to_stored_mesh();
  void fine_tune_mesh(const float&, const float&, const bool);
  void bit_clear(uint16_t bits[16], uint8_t x, uint8_t y);
  void bit_set(uint16_t bits[16], uint8_t x, uint8_t y);
  bool is_bit_set(uint16_t bits[16], uint8_t x, uint8_t y);
  char *ftostr43sign(const float&, char);

  void gcode_G26();
  void gcode_G28();
  void gcode_G29();
  extern char conv[9];

  void save_ubl_active_state_and_disable();
  void restore_ubl_active_state_and_leave();

  ///////////////////////////////////////////////////////////////////////////////////////////////////////

  #if ENABLED(ULTRA_LCD)
    extern char lcd_status_message[];
    void lcd_quick_feedback();
  #endif

  enum MBLStatus { MBL_STATUS_NONE = 0, MBL_STATUS_HAS_MESH_BIT = 0, MBL_STATUS_ACTIVE_BIT = 1 };

  #define MESH_X_DIST (float(UBL_MESH_MAX_X - (UBL_MESH_MIN_X)) / float(GRID_MAX_POINTS_X - 1))
  #define MESH_Y_DIST (float(UBL_MESH_MAX_Y - (UBL_MESH_MIN_Y)) / float(GRID_MAX_POINTS_Y - 1))

  typedef struct {
    bool active = false;
    float z_offset = 0.0;
    int8_t eeprom_storage_slot = -1,
           n_x = GRID_MAX_POINTS_X,
           n_y = GRID_MAX_POINTS_Y;

    float mesh_x_min = UBL_MESH_MIN_X,
          mesh_y_min = UBL_MESH_MIN_Y,
          mesh_x_max = UBL_MESH_MAX_X,
          mesh_y_max = UBL_MESH_MAX_Y,
          mesh_x_dist = MESH_X_DIST,
          mesh_y_dist = MESH_Y_DIST;

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      float g29_correction_fade_height = 10.0,
            g29_fade_height_multiplier = 1.0 / 10.0; // It's cheaper to do a floating point multiply than divide,
                                                     // so keep this value and its reciprocal.
    #endif

    // If you change this struct, adjust TOTAL_STRUCT_SIZE

    #define TOTAL_STRUCT_SIZE 40 // Total size of the above fields

    // padding provides space to add state variables without
    // changing the location of data structures in the EEPROM.
    // This is for compatibility with future versions to keep
    // users from having to regenerate their mesh data.
    unsigned char padding[64 - TOTAL_STRUCT_SIZE];

  } ubl_state;

  class unified_bed_leveling {
    private:

      static float last_specified_z;

    public:

      static ubl_state state, pre_initialized;

      static float z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y],
                   mesh_index_to_xpos[GRID_MAX_POINTS_X + 1], // +1 safety margin for now, until determinism prevails
                   mesh_index_to_ypos[GRID_MAX_POINTS_Y + 1];

      static bool g26_debug_flag,
                  has_control_of_lcd_panel;

      static int8_t eeprom_start;

      static volatile int encoder_diff; // Volatile because it's changed at interrupt time.

      unified_bed_leveling();

      static void display_map(const int);

      static void reset();
      static void invalidate();

      static void store_state();
      static void load_state();
      static void store_mesh(const int16_t);
      static void load_mesh(const int16_t);

      static bool sanity_check();

      static FORCE_INLINE void set_z(const int8_t px, const int8_t py, const float &z) { z_values[px][py] = z; }

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
      static FORCE_INLINE float calc_z0(const float &a0, const float &a1, const float &z1, const float &a2, const float &z2) {
        return z1 + (z2 - z1) * (a0 - a1) / (a2 - a1);
      }

      /**
       * z_correction_for_x_on_horizontal_mesh_line is an optimization for
       * the rare occasion when a point lies exactly on a Mesh line (denoted by index yi).
       */
      static inline float z_correction_for_x_on_horizontal_mesh_line(const float &lx0, const int x1_i, const int yi) {
        if (!WITHIN(x1_i, 0, GRID_MAX_POINTS_X - 1) || !WITHIN(yi, 0, GRID_MAX_POINTS_Y - 1)) {
          SERIAL_ECHOPAIR("? in z_correction_for_x_on_horizontal_mesh_line(lx0=", lx0);
          SERIAL_ECHOPAIR(",x1_i=", x1_i);
          SERIAL_ECHOPAIR(",yi=", yi);
          SERIAL_CHAR(')');
          SERIAL_EOL;
          return NAN;
        }

        const float xratio = (RAW_X_POSITION(lx0) - mesh_index_to_xpos[x1_i]) * (1.0 / (MESH_X_DIST)),
                    z1 = z_values[x1_i][yi];

        return z1 + xratio * (z_values[x1_i + 1][yi] - z1);
      }

      //
      // See comments above for z_correction_for_x_on_horizontal_mesh_line
      //
      static inline float z_correction_for_y_on_vertical_mesh_line(const float &ly0, const int xi, const int y1_i) {
        if (!WITHIN(xi, 0, GRID_MAX_POINTS_X - 1) || !WITHIN(y1_i, 0, GRID_MAX_POINTS_Y - 1)) {
          SERIAL_ECHOPAIR("? in get_z_correction_along_vertical_mesh_line_at_specific_x(ly0=", ly0);
          SERIAL_ECHOPAIR(", x1_i=", xi);
          SERIAL_ECHOPAIR(", yi=", y1_i);
          SERIAL_CHAR(')');
          SERIAL_EOL;
          return NAN;
        }

        const float yratio = (RAW_Y_POSITION(ly0) - mesh_index_to_ypos[y1_i]) * (1.0 / (MESH_Y_DIST)),
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

        if (!WITHIN(cx, 0, GRID_MAX_POINTS_X - 1) || !WITHIN(cy, 0, GRID_MAX_POINTS_Y - 1)) {

          SERIAL_ECHOPAIR("? in get_z_correction(lx0=", lx0);
          SERIAL_ECHOPAIR(", ly0=", ly0);
          SERIAL_CHAR(')');
          SERIAL_EOL;

          #if ENABLED(ULTRA_LCD)
            strcpy(lcd_status_message, "get_z_correction() indexes out of range.");
            lcd_quick_feedback();
          #endif
          return 0.0; // this used to return state.z_offset
        }

        const float z1 = calc_z0(RAW_X_POSITION(lx0),
                      mesh_index_to_xpos[cx], z_values[cx][cy],
                      mesh_index_to_xpos[cx + 1], z_values[cx + 1][cy]),
                    z2 = calc_z0(RAW_X_POSITION(lx0),
                      mesh_index_to_xpos[cx], z_values[cx][cy + 1],
                      mesh_index_to_xpos[cx + 1], z_values[cx + 1][cy + 1]);
              float z0 = calc_z0(RAW_Y_POSITION(ly0),
                  mesh_index_to_ypos[cy], z1,
                  mesh_index_to_ypos[cy + 1], z2);

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(MESH_ADJUST)) {
            SERIAL_ECHOPAIR(" raw get_z_correction(", lx0);
            SERIAL_CHAR(',')
            SERIAL_ECHO(ly0);
            SERIAL_ECHOPGM(") = ");
            SERIAL_ECHO_F(z0, 6);
          }
        #endif

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(MESH_ADJUST)) {
            SERIAL_ECHOPGM(" >>>---> ");
            SERIAL_ECHO_F(z0, 6);
            SERIAL_EOL;
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
              SERIAL_EOL;
            }
          #endif
        }
        return z0; // there used to be a +state.z_offset on this line
      }

      /**
       * This function sets the Z leveling fade factor based on the given Z height,
       * only re-calculating when necessary.
       *
       *  Returns 1.0 if g29_correction_fade_height is 0.0.
       *  Returns 0.0 if Z is past the specified 'Fade Height'.
       */
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)

        static FORCE_INLINE float fade_scaling_factor_for_z(const float &lz) {
          if (state.g29_correction_fade_height == 0.0) return 1.0;

          static float fade_scaling_factor = 1.0;
          const float rz = RAW_Z_POSITION(lz);
          if (last_specified_z != rz) {
            last_specified_z = rz;
            fade_scaling_factor =
              rz < state.g29_correction_fade_height
                ? 1.0 - (rz * state.g29_fade_height_multiplier)
                : 0.0;
          }
          return fade_scaling_factor;
        }

      #endif

  }; // class unified_bed_leveling

  extern unified_bed_leveling ubl;

  #define UBL_LAST_EEPROM_INDEX (E2END - sizeof(unified_bed_leveling::state))

#endif // AUTO_BED_LEVELING_UBL
#endif // UNIFIED_BED_LEVELING_H
