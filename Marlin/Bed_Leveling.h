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

extern float last_specified_z;
extern float fade_scaling_factor_for_current_height;
extern float z_values[MESH_NUM_X_POINTS][MESH_NUM_Y_POINTS];
extern float mesh_index_to_X_location[MESH_NUM_X_POINTS+1];	// +1 just because of paranoia that we might end up on the
extern float mesh_index_to_Y_location[MESH_NUM_Y_POINTS+1];	// the last Mesh Line and that is the start of a whole new cell

class bed_leveling {
  public:
	struct ubl_state {
		bool active;
		float z_offset = 0.0;
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
		float G29_Fade_Height_Multiplier = 1.0/10.0; 	// It is cheaper to do a floating point multiply than a floating
								// point divide.  So, we keep this number in both forms.  The first
								// is for the user.  The second one is the one that is actually used
								// again and again and again during the correction calculations.

		unsigned char padding[24];  	// This is just to allow room to add state variables without
						// changing the location of data structures in the EEPROM.   
						// This is for compatability with future versions to keep 
						// people from having to regenerate thier mesh data.
						//
						// If you change the contents of this struct, please adjust
						// the padding[] to keep the size the same!
	} state, pre_initialized;


    bed_leveling();
//  ~bed_leveling();	// No destructor because this object never goes away!

    void display_map(int);

    void reset();
    void invalidate();

    void store_state();
    void load_state();
    void store_mesh(int);
    void load_mesh(int);

    int sanity_check();

    FORCE_INLINE float map_x_index_to_bed_location(int8_t i){ return ((float) MESH_MIN_X) + (((float) MESH_X_DIST) * (float) i); };
    FORCE_INLINE float map_y_index_to_bed_location(int8_t i){ return ((float) MESH_MIN_Y) + (((float) MESH_Y_DIST) * (float) i); };

    void set_z(const int8_t px, const int8_t py, const float z) { z_values[px][py] = z; }

    int8_t get_cell_index_x(float x) {
      int8_t cx = (x - (MESH_MIN_X)) * (1.0 / (MESH_X_DIST));
      return constrain(cx, 0, (MESH_NUM_X_POINTS) - 1);		// -1 is appropriate if we want to all movement to the X_MAX 
    }								// position.  But with this defined this way, it is possible
								// to extrapolate off of this point even further out.  Probably
								// that is OK because something else should be keeping that from
								// happening and should not be worried about at this level.
    int8_t get_cell_index_y(float y) {
      int8_t cy = cy = (y - (MESH_MIN_Y)) * (1.0 / (MESH_Y_DIST));
      return constrain(cy, 0, (MESH_NUM_Y_POINTS) - 1);		// -1 is appropriate if we want to all movement to the Y_MAX 
    }								// position.  But with this defined this way, it is possible
								// to extrapolate off of this point even further out.  Probably
								// that is OK because something else should be keeping that from
								// happening and should not be worried about at this level.

    int8_t find_closest_x_index(float x) {
      int8_t px = (x - (MESH_MIN_X) + (MESH_X_DIST) * 0.5) * (1.0 / (MESH_X_DIST));
      return (px >= 0 && px < (MESH_NUM_X_POINTS)) ? px : -1;
    }

    int8_t find_closest_y_index(float y) {
      int8_t py = (y - (MESH_MIN_Y) + (MESH_Y_DIST) * 0.5) * (1.0 / (MESH_Y_DIST));
      return (py >= 0 && py < (MESH_NUM_Y_POINTS)) ? py : -1;
    }



    //                           z2   --|
    //                 z0        |      |
    //                  |        |      + (z2-z1)
    //   z1             |        |      |
    // ---+-------------+--------+--  --|
    //   a1            a0        a2
    //    |<---delta_a---------->|
    //
    //  float calc_z0() is the basis for all the Mesh Based correction.  It is used to
    //  find the expected Z Height at a position between two known Z-Height locations
    //
    //  It is farly expensive with its 4 floating point additions and 2 floating point 
    //  multiplications.   
    
inline float calc_z0(float a0, float a1, float z1, float a2, float z2) {
      float delta_z = (z2 - z1);
      float delta_a = (a0 - a1) / (a2 - a1);
      return z1 + delta_a * delta_z;
    }


//	get_z_correction_at_Y_intercept(float x0, int x1_i, int yi) only takes 
//	three parameters.  It assumes the x0 point is on a Mesh line denoted by yi.   In theory
//	we could use get_cell_index_x(float x) to obtain the 2nd parameter x1_i but any code calling
//	the get_z_correction_along_vertical_mesh_line_at_specific_X routine  will already have 
//	the X index of the x0 intersection available and we don't want to perform any extra floating 
//	point operations.
//
		
inline float get_z_correction_along_horizontal_mesh_line_at_specific_X(float x0, int x1_i, int yi) {
float z1, z2, dz, a0ma1diva2ma1;

	a0ma1diva2ma1 = (x0-mesh_index_to_X_location[x1_i]) * (1.0/MESH_X_DIST);
	z1 = z_values[x1_i  ][yi];
	z2 = z_values[x1_i+1][yi];
	dz = (z2 - z1);

        return z1 + a0ma1diva2ma1 * dz;
}


//
//	See comments above for get_z_correction_along_horizontal_mesh_line_at_specific_X
//
//
inline float get_z_correction_along_vertical_mesh_line_at_specific_Y(float y0, int xi, int y1_i) {
float z1, z2, dx, dz, a0ma1diva2ma1;

	a0ma1diva2ma1 = (y0-mesh_index_to_Y_location[y1_i]) * (1.0/MESH_Y_DIST);
	z1 = z_values[xi][y1_i];
	z2 = z_values[xi][y1_i+1];
	dz = (z2 - z1);

        return z1 + a0ma1diva2ma1 * dz;
}

 
//
//	This is the generic Z-Correction.  It works anywhere within a Mesh Cell.  It first
//	does a linear interpolation along both of the bounding X-Mesh-Lines to find the
//	Z-Height at both ends.  Then it does a linear interpolation of these heights based
//	on the Y position within the cell.
//
    float get_z_correction(float x0, float y0) {
      int8_t cx = get_cell_index_x(x0),
             cy = get_cell_index_y(y0);

      if (cx < 0 || cy < 0)  {

           #if ENABLED(DEBUG_LEVELING_FEATURE)
              if (DEBUGGING(MESH_ADJUST)) {
                  SERIAL_ECHOPAIRPGM("huh???? get_z_correction(",cx);
                  SERIAL_ECHOPAIRPGM(",",cy);
                  SERIAL_ECHOPGM(")\n");
	      }
           #endif

           #if ENABLED(ULTRA_LCD)
	      strcpy(lcd_status_message,"get_z_correction() indexes out of range.");
	      lcd_quick_feedback();
           #endif
	      return 0.0;		// this used to return state.z_offset
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
               SERIAL_ECHOPAIRPGM(" raw get_z_correction(", x0);
               SERIAL_ECHOPAIRPGM(",", y0);
               SERIAL_ECHOPGM(")=");
               SERIAL_ECHO_F(z0,6);
           }
      #endif

#if ENABLED(DEBUG_LEVELING_FEATURE)
     if (DEBUGGING(MESH_ADJUST)) {
       SERIAL_ECHOPGM(" >>>---> ");
       SERIAL_ECHO_F(z0,6);
       SERIAL_ECHOPGM("\n");
     }
#endif

     if ( isnan(z0) ) {		// if part of the Mesh is undefined, it will show up as NAN
       z0 = 0.0;		// in blm.z_values[][] and propagate through the 
				// calculations. If our correction is NAN, we throw it out 
				// because part of the Mesh is undefined and we don't have the 
				// information we need to complete the height correction.

#if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(MESH_ADJUST)) {
          SERIAL_ECHOPGM("??? Yikes!  NAN in get_z_correction( ");
          SERIAL_ECHO(x0);
          SERIAL_ECHOPGM(", ");
          SERIAL_ECHO(y0);
          SERIAL_ECHOPGM(" )\n");
        }
#endif
     }
     return z0;			// there used to be a  +state.z_offset on this line
  }


//
// This routine is used to scale the Z correction depending upon the current nozzle height.  It is
// optimized for speed.  It avoids floating point operations by checking if the requested scaling
// factor is going to be the same as the last time the function calculated a value.  If so, it just
// returns it.
//
// If it must do a calcuation, it will return a scaling factor of 0.0 if the UBL System is not active
// or if the current Z Height is past the specified 'Fade Height'
//
  FORCE_INLINE float fade_scaling_factor_for_Z( float current_z ) {
    if ( last_specified_z == current_z ) 
      return fade_scaling_factor_for_current_height;

    last_specified_z = current_z;	
    if ( state.active && (current_z<state.G29_Correction_Fade_Height) ) 
      fade_scaling_factor_for_current_height = 1.0 - (current_z * state.G29_Fade_Height_Multiplier);
    else 
      fade_scaling_factor_for_current_height = 0.0;
    return fade_scaling_factor_for_current_height;
  } 
} ;

extern bed_leveling blm;

#endif  // UNIFIED_BED_LEVELING_FEATURE
#endif

