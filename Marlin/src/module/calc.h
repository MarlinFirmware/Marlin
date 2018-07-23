#ifndef __CALC_H
#define __CALC_H

#include "../../Configuration.h"

//extern float homing_feedrate[];
extern bool axis_relative_modes[];
extern int feedmultiply;
extern int extrudemultiply; // Sets extrude multiply factor (in percent) for all extruders
extern float add_homing[3];
extern bool junction_deviation;
extern bool s_curve_acceleration;
extern bool volumetric_enabled;
#ifdef DELTA
extern float endstop_adj[3];
extern float delta_radius;
extern float delta_diagonal_rod;
extern float delta_segments_per_second;
void recalc_delta_settings(float radius, float diagonal_rod);
#endif
#ifdef SCARA
extern float axis_scaling[3];  // Build size scaling
#endif
extern float min_pos[3];
extern float max_pos[3];
extern bool axis_known_position[3];
extern float zprobe_zoffset;
extern int fanSpeed;
#ifdef BARICUDA
extern int ValvePressure;
extern int EtoPPressure;
#endif

extern unsigned char active_extruder;

#endif
