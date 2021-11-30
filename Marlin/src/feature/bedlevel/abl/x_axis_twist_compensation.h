#pragma once

#include "../../../inc/MarlinConfigPre.h"

typedef float xatc_points[XATC_MAX_POINTS];
extern xatc_points xatc_z_values;
extern float xatc_spacing;
extern float xatc_start;

float x_axis_twist_compensation(const xy_pos_t &raw);
void print_xatc_points();