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


#ifndef UNIFIED_BED_LEVELING_H
#define UNIFIED_BED_LEVELING_H

#include "Marlin.h"
#include "math.h"

#if ENABLED(UNIFIED_BED_LEVELING_FEATURE)

  #if ENABLED(ULTRA_LCD)
    extern char lcd_status_message[];
    void lcd_quick_feedback();
  #endif

enum MBLStatus { MBL_STATUS_NONE = 0, MBL_STATUS_HAS_MESH_BIT = 0, MBL_STATUS_ACTIVE_BIT = 1 };

  #define MESH_X_DIST ((float) ((((float) MESH_MAX_X)-((float) MESH_MIN_X)) / (((float) MESH_NUM_X_POINTS)-1.0)))
  #define MESH_Y_DIST ((float) ((((float) MESH_MAX_Y)-((float) MESH_MIN_Y)) / (((float) MESH_NUM_Y_POINTS)-1.0)))

  class bed_leveling {
  public:
	struct ubl_state {
		bool active;
		float z_offset;
		int  EEPROM_storage_slot = -1;
		int  n_x = MESH_NUM_X_POINTS;
		int  n_y = MESH_NUM_Y_POINTS;
		float mesh_x_min = MESH_MIN_X;
		float mesh_y_min = MESH_MIN_Y;
		float mesh_x_max = MESH_MAX_X;
		float mesh_y_max = MESH_MAX_Y;
		float mesh_x_dist = MESH_X_DIST;
		float mesh_y_dist = MESH_Y_DIST;
		float G29_Correction_Fade_Height = 10.0; 
		unsigned char padding[28];  	// This is just to allow room to add state variables without
						// changing the location of data structures in the EEPROM.   
						// This is for compatability with future versions to keep 
						// people from having to regenerate thier mesh data.
						//
						// If you change the contents of this struct, please adjust
						// the padding[] to keep the size the same!!!
	} state, pre_initialized;

    float z_values[MESH_NUM_X_POINTS][MESH_NUM_Y_POINTS];

    bed_leveling();	// { reset(); }

    void display_map(int);

    void reset();
    void invalidate();

    void store_state();
    void load_state();
    void store_mesh(int);
    void load_mesh(int);

    int sanity_check();

    static FORCE_INLINE float map_x_index_to_bed_location(int8_t i){ return MESH_MIN_X + (MESH_X_DIST) * i; };
    static FORCE_INLINE float map_y_index_to_bed_location(int8_t i){ return MESH_MIN_Y + (MESH_Y_DIST) * i; };

    void set_z(const int8_t px, const int8_t py, const float z) { z_values[px][py] = z; }

    int8_t get_cell_index_x(float x) {
      int8_t cx = int(x - (MESH_MIN_X)) / (MESH_X_DIST);
      return constrain(cx, 0, (MESH_NUM_X_POINTS) - 2);
    }

    int8_t get_cell_index_y(float y) {
      int8_t cy = int(y - (MESH_MIN_Y)) / (MESH_Y_DIST);
      return constrain(cy, 0, (MESH_NUM_Y_POINTS) - 2);
    }

    int8_t find_closest_x_index(float x) {
      int8_t px = int(x - (MESH_MIN_X) + (MESH_X_DIST) / 2) / (MESH_X_DIST);
      return (px >= 0 && px < (MESH_NUM_X_POINTS)) ? px : -1;
    }

    int8_t find_closest_y_index(float y) {
      int8_t py = int(y - (MESH_MIN_Y) + (MESH_Y_DIST) / 2) / (MESH_Y_DIST);
      return (py >= 0 && py < (MESH_NUM_Y_POINTS)) ? py : -1;
    }

    float calc_z0(float a0, float a1, float z1, float a2, float z2) {
      float delta_z = (z2 - z1) / (a2 - a1);
      float delta_a = a0 - a1;
      return z1 + delta_a * delta_z;
    }

    float get_z_correction(float x0, float y0) {
      int8_t cx = get_cell_index_x(x0),
             cy = get_cell_index_y(y0);

      if (cx < 0 || cy < 0)  {

#if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(MESH_ADJUST)) {
          SERIAL_ECHOPAIR("huh???? get_z_correction(",cx);
          SERIAL_ECHOPAIR(",",cy);
          SERIAL_ECHO(")\n");
	}
#endif

#if ENABLED(ULTRA_LCD)
	      strcpy(lcd_status_message,"get_z_correction() indexes out of range.");
	      lcd_quick_feedback();
#endif
	      return state.z_offset;
      }

      float z1 = calc_z0(x0,
                         map_x_index_to_bed_location(cx), z_values[cx][cy],
                         map_x_index_to_bed_location(cx + 1), z_values[cx + 1][cy]);
      float z2 = calc_z0(x0,
                         map_x_index_to_bed_location(cx), z_values[cx][cy + 1],
                         map_x_index_to_bed_location(cx + 1), z_values[cx + 1][cy + 1]);
      float z0 = calc_z0(y0,
                         map_y_index_to_bed_location(cy), z1,
                         map_y_index_to_bed_location(cy + 1), z2);



#if ENABLED(DEBUG_LEVELING_FEATURE)
     if (DEBUGGING(MESH_ADJUST)) {
       SERIAL_ECHOPAIR(" raw get_z_correction(", x0);
       SERIAL_ECHOPAIR(",", y0);
       SERIAL_ECHO(")=");
       SERIAL_ECHO_F(z0,6);
     }
#endif

      z0 = z0 * fade_scaling_factor_for_current_height;

#if ENABLED(DEBUG_LEVELING_FEATURE)
     if (DEBUGGING(MESH_ADJUST)) {
       SERIAL_ECHO(" >>>---> ");
       SERIAL_ECHO_F(z0,6);
       SERIAL_ECHO("\n");
     }
#endif

      if ( isnan(z0) ) {	// if part of the Mesh is undefined, it will show up as NAN
	z0 = 0.0;		// in bed_leveling_mesh.z_values[][] and propagate through the calculations.
				// if our correction is NAN, we throw it out because part of the
				// Mesh is undefined and we don't have the information we need to
				// complete the height correction.

#if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(MESH_ADJUST)) {
          SERIAL_ECHO("??? Yikes!!!  NAN in get_z_correction( ");
          SERIAL_ECHO(x0);
          SERIAL_ECHO(", ");
          SERIAL_ECHO(y0);
          SERIAL_ECHO(" )\n");
        }
#endif
     }
     return z0 + state.z_offset;
  }
}; 

  extern bed_leveling bed_leveling_mesh;

#endif  // UNIFIED_BED_LEVELING_FEATURE
#endif

