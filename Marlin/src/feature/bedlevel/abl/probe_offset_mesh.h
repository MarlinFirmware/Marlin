#pragma once

#include "../../../inc/MarlinConfigPre.h"

typedef float probe_offset_mesh_t[PROBE_OFFSET_MESH_GRID_MAX_POINTS_X][PROBE_OFFSET_MESH_GRID_MAX_POINTS_Y];
extern probe_offset_mesh_t z_offset_mesh;
extern xy_pos_t z_offset_mesh_grid_spacing;
extern xy_float_t z_offset_mesh_grid_factor;

float z_offset_mesh_from_raw_position(const xy_pos_t &raw);
void refresh_probe_offset_mesh();
void print_z_offset_grid();