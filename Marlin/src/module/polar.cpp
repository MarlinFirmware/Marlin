/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * POLAR Kinematics was developed by Kadir ilkimen
 * 
 * A polar machine can have different configurations.
 * This kinematics only compatible with the following configuration:
 * - X : Independent linear.
 * - Y or B : Polar
 * - Z : Independent linear
 * 
 * babyBear and the PolarBear are open source Polar Machines by Kadir ilkimen.
 * 
 * babyBear 3D printer:
 * https://github.com/kadirilkimen/babyBear-3D-printer
 * 
 * babyBear is fully compatible with this POLAR Kinematics.
 * 
 * 
 * the PolarBear Cnc Machine:
 * https://github.com/kadirilkimen/Polar-Bear-Cnc-Machine
 * 
 * the PolarBear has the following configuration.
 * - X and Z as CoreXZ.
 * - Y or B : Polar
 * Tests required to confirm it works with POLAR and CoreXZ configuration.
 * 
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(POLAR)

#include "polar.h"
#include "motion.h"
#include "planner.h"

float segments_per_second = DEFAULT_SEGMENTS_PER_SECOND;

float polar_center_offset = POLAR_CENTER_OFFSET;

float current_polar_theta = 0;

float absoluteAngle(float a){
    if(a<0.0){
      while(a<0.0) {
          a+=360.0;
      }
    } else if(a>360.0) {
      while(a>360.0) {
          a-=360.0;
      }
    }
    return a;
  }

void forward_kinematics(const_float_t r, const_float_t theta) {
  const float absTheta = absoluteAngle(theta);
  float radius = r;
  if(polar_center_offset>0.0) radius = SQRT( ABS( sq(r) - sq(-polar_center_offset) ) );
  cartes.x = cos(RADIANS(absTheta))*radius;
  cartes.y = sin(RADIANS(absTheta))*radius;
}


void inverse_kinematics(const xyz_pos_t &raw) {

    const float x = raw.x,
                y = raw.y,
                rawRadius = HYPOT(x,y),
                posTheta = DEGREES(ATAN2(y, x));
    
    float r = rawRadius;
    float theta = absoluteAngle(posTheta);
    float currentAbsTheta = absoluteAngle(current_polar_theta);

    if(polar_center_offset>0.0) {
      const float offsetRadius = SQRT( ABS( sq(r) - sq(polar_center_offset) ) );
      float offsetTheta = absoluteAngle( DEGREES( ATAN2( polar_center_offset, offsetRadius ) ) );
      theta = absoluteAngle( offsetTheta+theta );
    }

    const float deltaTheta = theta - currentAbsTheta;
    if(ABS(deltaTheta)<=180) theta = current_polar_theta+deltaTheta;
    else {
      if(currentAbsTheta>180) theta = current_polar_theta+360+deltaTheta;
      else theta = current_polar_theta - (360-deltaTheta);
    }

    current_polar_theta = theta;

    delta.set(r, theta, raw.z);
}

void polar_report_positions() {
  SERIAL_ECHOLNPGM( "X: ", planner.get_axis_position_mm(X_AXIS), " POLAR Theta:",
    planner.get_axis_position_degrees(B_AXIS), " Z: ",
    planner.get_axis_position_mm(Z_AXIS) );

  SERIAL_EOL();}
#endif
