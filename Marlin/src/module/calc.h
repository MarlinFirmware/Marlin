#ifndef __CALC_H
#define __CALC_H

#include "../../Configuration.h"

//extern float homing_feedrate[];
void get_coordinates();
bool code_seen(char code);
double code_value();
extern bool axis_relative_modes[];
extern int feedmultiply;
extern int extrudemultiply; // Sets extrude multiply factor (in percent) for all extruders
extern float add_homing[3];
extern bool junction_deviation;
extern bool linear_advance;
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
extern float zprobe_zoffset;
extern int fanSpeed;
#ifdef BARICUDA
extern int ValvePressure;
extern int EtoPPressure;
#endif

extern unsigned char active_extruder;

// From motion.h
void set_destination_from_current();
extern double feedrate_mm_s;
extern xyze_pos_t destination;
extern void prepare_move(const ExtraData& extra_data, const feedRate_t &fr_mm_s=0.0f);
#define prepare_internal_move_to_destination prepare_move

#endif
