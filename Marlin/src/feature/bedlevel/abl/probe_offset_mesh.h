#pragma once

#include "../../../inc/MarlinConfigPre.h"

extern bed_mesh_t z_offset_mesh;

float z_offset_mesh_from_raw_position(const xy_pos_t &raw);
void print_z_offset_grid();