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

#include "Marlin.h"
#include "math.h"
#include "vector_3.h"

#ifndef UNIFIED_BED_LEVELING_H
#define UNIFIED_BED_LEVELING_H

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    #define UBL_OK false
    #define UBL_ERR true

    typedef struct {
      int8_t x_index, y_index;
      float distance; // When populated, the distance from the search location
    } mesh_index_pair;

    enum MeshPointType { INVALID, REAL, SET_IN_BITMAP };

    bool axis_unhomed_error(bool, bool, bool);
    void dump(char * const str, const float &f);
    bool ubl_lcd_clicked();
    void probe_entire_mesh(const float&, const float&, const bool, const bool, const bool);
    void debug_current_and_destination(char *title);
    void ubl_line_to_destination(const float&, const float&, const float&, const float&, const float&, uint8_t);
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

    #define MESH_X_DIST ((float(UBL_MESH_MAX_X) - float(UBL_MESH_MIN_X)) / (float(UBL_MESH_NUM_X_POINTS) - 1.0))
    #define MESH_Y_DIST ((float(UBL_MESH_MAX_Y) - float(UBL_MESH_MIN_Y)) / (float(UBL_MESH_NUM_Y_POINTS) - 1.0))

    #if ENABLED(UBL_MESH_EDIT_ENABLED)
      extern bool g26_debug_flag;
    #else
      constexpr bool g26_debug_flag = false;
    #endif
    extern float last_specified_z;
    extern float fade_scaling_factor_for_current_height;
    extern float z_values[UBL_MESH_NUM_X_POINTS][UBL_MESH_NUM_Y_POINTS];
    extern float mesh_index_to_x_location[UBL_MESH_NUM_X_POINTS + 1]; // +1 just because of paranoia that we might end up on the
    extern float mesh_index_to_y_location[UBL_MESH_NUM_Y_POINTS + 1]; // the last Mesh Line and that is the start of a whole new cell

    class unified_bed_leveling {
      public:
      struct ubl_state {
        bool active = false;
        float z_offset = 0.0;
        int eeprom_storage_slot = -1,
            n_x = UBL_MESH_NUM_X_POINTS,
            n_y = UBL_MESH_NUM_Y_POINTS;
        float mesh_x_min = UBL_MESH_MIN_X,
              mesh_y_min = UBL_MESH_MIN_Y,
              mesh_x_max = UBL_MESH_MAX_X,
              mesh_y_max = UBL_MESH_MAX_Y,
              mesh_x_dist = MESH_X_DIST,
              mesh_y_dist = MESH_Y_DIST;

        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          float g29_correction_fade_height = 10.0,
                g29_fade_height_multiplier = 1.0 / 10.0; // It is cheaper to do a floating point multiply than a floating
                                                         // point divide. So, we keep this number in both forms. The first
                                                         // is for the user. The second one is the one that is actually used
                                                         // again and again and again during the correction calculations.
        #endif

        unsigned char padding[24];  // This is just to allow room to add state variables without
                                    // changing the location of data structures in the EEPROM.
                                    // This is for compatability with future versions to keep
                                    // people from having to regenerate thier mesh data.
                                    //
                                    // If you change the contents of this struct, please adjust
                                    // the padding[] to keep the size the same!
      } state, pre_initialized;

      unified_bed_leveling();
      //  ~unified_bed_leveling();  // No destructor because this object never goes away!

      void display_map(const int);

      void reset();
      void invalidate();

      void store_state();
      void load_state();
      void store_mesh(const int16_t);
      void load_mesh(const int16_t);

      bool sanity_check();

      FORCE_INLINE static float map_x_index_to_bed_location(const int8_t i) { return ((float) UBL_MESH_MIN_X) + (((float) MESH_X_DIST) * (float) i); };
      FORCE_INLINE static float map_y_index_to_bed_location(const int8_t i) { return ((float) UBL_MESH_MIN_Y) + (((float) MESH_Y_DIST) * (float) i); };

      FORCE_INLINE void set_z(const int8_t px, const int8_t py, const float &z) { z_values[px][py] = z; }

      static int8_t get_cell_index_x(const float &x) {
        const int8_t cx = (x - (UBL_MESH_MIN_X)) * (1.0 / (MESH_X_DIST));
        return constrain(cx, 0, (UBL_MESH_NUM_X_POINTS) - 1);   // -1 is appropriate if we want all movement to the X_MAX
      }                                                         // position. But with this defined this way, it is possible
                                                                // to extrapolate off of this point even further out. Probably
                                                                // that is OK because something else should be keeping that from
                                                                // happening and should not be worried about at this level.
      static int8_t get_cell_index_y(const float &y) {
        const int8_t cy = (y - (UBL_MESH_MIN_Y)) * (1.0 / (MESH_Y_DIST));
        return constrain(cy, 0, (UBL_MESH_NUM_Y_POINTS) - 1);   // -1 is appropriate if we want all movement to the Y_MAX
      }                                                         // position. But with this defined this way, it is possible
                                                                // to extrapolate off of this point even further out. Probably
                                                                // that is OK because something else should be keeping that from
                                                                // happening and should not be worried about at this level.

      static int8_t find_closest_x_index(const float &x) {
        const int8_t px = (x - (UBL_MESH_MIN_X) + (MESH_X_DIST) * 0.5) * (1.0 / (MESH_X_DIST));
        return (px >= 0 && px < (UBL_MESH_NUM_X_POINTS)) ? px : -1;
      }

      static int8_t find_closest_y_index(const float &y) {
        const int8_t py = (y - (UBL_MESH_MIN_Y) + (MESH_Y_DIST) * 0.5) * (1.0 / (MESH_Y_DIST));
        return (py >= 0 && py < (UBL_MESH_NUM_Y_POINTS)) ? py : -1;
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
        const float delta_z = (z2 - z1),
                    delta_a = (a0 - a1) / (a2 - a1);
        return z1 + delta_a * delta_z;
      }

      /**
       * get_z_correction_at_Y_intercept(float x0, int x1_i, int yi) only takes
       * three parameters. It assumes the x0 point is on a Mesh line denoted by yi. In theory
       * we could use get_cell_index_x(float x) to obtain the 2nd parameter x1_i but any code calling
       * the get_z_correction_along_vertical_mesh_line_at_specific_X routine  will already have
       * the X index of the x0 intersection available and we don't want to perform any extra floating
       * point operations.
       */
      inline float get_z_correction_along_horizontal_mesh_line_at_specific_X(const float &x0, const int x1_i, const int yi) {
        if (x1_i < 0 || yi < 0 || x1_i >= UBL_MESH_NUM_X_POINTS || yi >= UBL_MESH_NUM_Y_POINTS) {
          SERIAL_ECHOPAIR("? in get_z_correction_along_horizontal_mesh_line_at_specific_X(x0=", x0);
          SERIAL_ECHOPAIR(",x1_i=", x1_i);
          SERIAL_ECHOPAIR(",yi=", yi);
          SERIAL_CHAR(')');
          SERIAL_EOL;
          return NAN;
        }

        const float xratio = (RAW_X_POSITION(x0) - mesh_index_to_x_location[x1_i]) * (1.0 / (MESH_X_DIST)),
                    z1 = z_values[x1_i][yi],
                    z2 = z_values[x1_i + 1][yi],
                    dz = (z2 - z1);

        return z1 + xratio * dz;
      }

      //
      // See comments above for get_z_correction_along_horizontal_mesh_line_at_specific_X
      //
      inline float get_z_correction_along_vertical_mesh_line_at_specific_Y(const float &y0, const int xi, const int y1_i) {
        if (xi < 0 || y1_i < 0 || xi >= UBL_MESH_NUM_X_POINTS || y1_i >= UBL_MESH_NUM_Y_POINTS) {
          SERIAL_ECHOPAIR("? in get_z_correction_along_vertical_mesh_line_at_specific_X(y0=", y0);
          SERIAL_ECHOPAIR(", x1_i=", xi);
          SERIAL_ECHOPAIR(", yi=", y1_i);
          SERIAL_CHAR(')');
          SERIAL_EOL;
          return NAN;
        }

        const float yratio = (RAW_Y_POSITION(y0) - mesh_index_to_y_location[y1_i]) * (1.0 / (MESH_Y_DIST)),
                    z1 = z_values[xi][y1_i],
                    z2 = z_values[xi][y1_i + 1],
                    dz = (z2 - z1);

        return z1 + yratio * dz;
      }

      /**
       * This is the generic Z-Correction. It works anywhere within a Mesh Cell. It first
       * does a linear interpolation along both of the bounding X-Mesh-Lines to find the
       * Z-Height at both ends. Then it does a linear interpolation of these heights based
       * on the Y position within the cell.
       */
      float get_z_correction(const float &x0, const float &y0) const {
        const int8_t cx = get_cell_index_x(RAW_X_POSITION(x0)),
                     cy = get_cell_index_y(RAW_Y_POSITION(y0));

        if (cx < 0 || cy < 0 || cx >= UBL_MESH_NUM_X_POINTS || cy >= UBL_MESH_NUM_Y_POINTS) {

          SERIAL_ECHOPAIR("? in get_z_correction(x0=", x0);
          SERIAL_ECHOPAIR(", y0=", y0);
          SERIAL_CHAR(')');
          SERIAL_EOL;

          #if ENABLED(ULTRA_LCD)
            strcpy(lcd_status_message, "get_z_correction() indexes out of range.");
            lcd_quick_feedback();
          #endif
          return 0.0; // this used to return state.z_offset
        }

        const float z1 = calc_z0(RAW_X_POSITION(x0),
                      map_x_index_to_bed_location(cx), z_values[cx][cy],
                      map_x_index_to_bed_location(cx + 1), z_values[cx + 1][cy]),
                    z2 = calc_z0(RAW_X_POSITION(x0),
                      map_x_index_to_bed_location(cx), z_values[cx][cy + 1],
                      map_x_index_to_bed_location(cx + 1), z_values[cx + 1][cy + 1]);
              float z0 = calc_z0(RAW_Y_POSITION(y0),
                  map_y_index_to_bed_location(cy), z1,
                  map_y_index_to_bed_location(cy + 1), z2);

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(MESH_ADJUST)) {
            SERIAL_ECHOPAIR(" raw get_z_correction(", x0);
            SERIAL_ECHOPAIR(",", y0);
            SERIAL_ECHOPGM(")=");
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
              SERIAL_ECHOPGM("??? Yikes!  NAN in get_z_correction( ");
              SERIAL_ECHO(x0);
              SERIAL_ECHOPGM(", ");
              SERIAL_ECHO(y0);
              SERIAL_ECHOLNPGM(" )");
            }
          #endif
        }
        return z0; // there used to be a +state.z_offset on this line
      }

      /**
       * This routine is used to scale the Z correction depending upon the current nozzle height. It is
       * optimized for speed. It avoids floating point operations by checking if the requested scaling
       * factor is going to be the same as the last time the function calculated a value. If so, it just
       * returns it.
       *
       * It returns a scaling factor of 1.0 if UBL is inactive.
       * It returns a scaling factor of 0.0 if Z is past the specified 'Fade Height'
       */
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)

        FORCE_INLINE float fade_scaling_factor_for_z(const float &lz) const {
          const float rz = RAW_Z_POSITION(lz);
          if (last_specified_z != rz) {
            last_specified_z = rz;
            fade_scaling_factor_for_current_height =
              state.active && rz < state.g29_correction_fade_height
                ? 1.0 - (rz * state.g29_fade_height_multiplier)
                : 0.0;
          }
          return fade_scaling_factor_for_current_height;
        }

      #else

        static constexpr float fade_scaling_factor_for_z(const float &lz) { UNUSED(lz); return 1.0; }

      #endif

    }; // class unified_bed_leveling

    extern unified_bed_leveling ubl;
    extern int ubl_eeprom_start;

    #define UBL_LAST_EEPROM_INDEX (E2END - sizeof(unified_bed_leveling::state))

  #endif // AUTO_BED_LEVELING_UBL

#endif // UNIFIED_BED_LEVELING_H
