#include "Marlin.h"

#if ENABLED(MESH_BED_LEVELING)

  #define MESH_X_DIST ((MESH_MAX_X - MESH_MIN_X)/(MESH_NUM_X_POINTS - 1))
  #define MESH_Y_DIST ((MESH_MAX_Y - MESH_MIN_Y)/(MESH_NUM_Y_POINTS - 1))

  class mesh_bed_leveling {
  public:
    uint8_t active;
    float z_values[MESH_NUM_Y_POINTS][MESH_NUM_X_POINTS];

    mesh_bed_leveling();

    void reset();

    float get_x(int i) { return MESH_MIN_X + MESH_X_DIST * i; }
    float get_y(int i) { return MESH_MIN_Y + MESH_Y_DIST * i; }
    void set_z(int ix, int iy, float z) { z_values[iy][ix] = z; }

    int select_x_index(float x) {
      int i = 1;
      while (x > get_x(i) && i < MESH_NUM_X_POINTS - 1) i++;
      return i - 1;
    }

    int select_y_index(float y) {
      int i = 1;
      while (y > get_y(i) && i < MESH_NUM_Y_POINTS - 1) i++;
      return i - 1;
    }

    float calc_z0(float a0, float a1, float z1, float a2, float z2) {
      float delta_z = (z2 - z1) / (a2 - a1);
      float delta_a = a0 - a1;
      return z1 + delta_a * delta_z;
    }

    float get_z(float x0, float y0) {
      int x_index = select_x_index(x0);
      int y_index = select_y_index(y0);
      float z1 = calc_z0(x0,
                         get_x(x_index), z_values[y_index][x_index],
                         get_x(x_index + 1), z_values[y_index][x_index + 1]);
      float z2 = calc_z0(x0,
                         get_x(x_index), z_values[y_index + 1][x_index],
                         get_x(x_index + 1), z_values[y_index + 1][x_index + 1]);
      float z0 = calc_z0(y0,
                         get_y(y_index), z1,
                         get_y(y_index + 1), z2);
      return z0;
    }
  };

  extern mesh_bed_leveling mbl;

#endif  // MESH_BED_LEVELING
