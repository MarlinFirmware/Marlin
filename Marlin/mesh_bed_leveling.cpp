#include "mesh_bed_leveling.h"

#if defined(MESH_BED_LEVELING)

mesh_bed_leveling mbl;

mesh_bed_leveling::mesh_bed_leveling() {
    reset();
}
    
void mesh_bed_leveling::reset() {
    for (int y=0; y<MESH_NUM_Y_POINTS; y++) {
        for (int x=0; x<MESH_NUM_X_POINTS; x++) {
            z_values[y][x] = 0;
        }
    }
    active = 0;
}

#endif  // MESH_BED_LEVELING
