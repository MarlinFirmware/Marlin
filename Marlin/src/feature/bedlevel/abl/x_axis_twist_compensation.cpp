#include "../../../inc/MarlinConfig.h"

#if ENABLED(X_AXIS_TWIST_COMPENSATION)

#include "../bedlevel.h"

xatc_points xatc_z_values;
float xatc_spacing;
float xatc_start;

void print_xatc_points(){
  SERIAL_ECHOLNPGM(" XATC correction values:");
  LOOP_L_N(x, XATC_MAX_POINTS) {
    SERIAL_CHAR(' ');
    if (!isnan(xatc_z_values[x])) {
      if (xatc_z_values[x] >= 0) SERIAL_CHAR('+');
      SERIAL_ECHO_F(xatc_z_values[x], 3);
    }
    else {
      LOOP_L_N(i, 6)
        SERIAL_CHAR(i ? '=' : ' ');
    }
  }
  SERIAL_EOL();
}

float lerp(float t, float a, float b){
  return  a + t * (b - a);
}

float x_axis_twist_compensation(const xy_pos_t &raw){
  float t = (raw.x - xatc_start) / xatc_spacing;
  int i = FLOOR(t);
  LIMIT(i, 0, XATC_MAX_POINTS - 2);
  t = t - i;
  return lerp(t, xatc_z_values[i], xatc_z_values[i + 1]);
}

#endif //PROBE_OFFSET_MESH