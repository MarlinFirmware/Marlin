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

#ifndef G29_UNIFIED_BED_LEVELING_H
#define G29_UNIFIED_BED_LEVELING_H

#include "Marlin.h"

#if ENABLED(UNIFIED_BED_LEVELING_FEATURE)

struct mesh_index_pair {
	int x_index;
	int y_index;
	float distance;		// Not always used.  But when populated, it is the distance
				// from the search location
} ;

struct vector {
	double dx;
	double dy;
	double dz;
};

enum Mesh_Point_Type { INVALID=0, REAL=1, SET_IN_BITMAP=2 };

extern bed_leveling bed_leveling_mesh;
void axis_unhomed_error(bool);
void dump( char *str, float f );
bool G29_lcd_clicked(); 
void probe_entire_mesh( float, float, bool );
void manually_probe_remaining_mesh( float, float, float, float, bool );
struct vector tilt_mesh_based_on_3pts(float, float, float );
void new_set_bed_level_equation_3pts(float , float , float );
float measure_business_card_thickness(float );
struct mesh_index_pair find_closest_mesh_point_of_type( Mesh_Point_Type, float, float, bool, unsigned int [16] );
void  Adjust_Mesh_to_Mean();
bool G29_Parameter_Parsing();
void G29_What_Command();
void G29_EEPROM_Dump();
void G29_Kompare_Current_Mesh_to_Stored_Mesh();
void fine_tune_mesh( float, float, float, bool );
void bit_clear( unsigned int bits[16], int , int );
void bit_set( unsigned int bits[16], int , int );
bool is_bit_set( unsigned int bits[16], int , int );
char *ftostr43sign(const float& , char );
void lcd_implementation_drawedit(const char* , const char* value=NULL);
void gcode_G28();
extern char conv[8];

#endif  // UNIFIED_BED_LEVELING_FEATURE
#endif  // G29_UNIFIED_BED_LEVELING_H


