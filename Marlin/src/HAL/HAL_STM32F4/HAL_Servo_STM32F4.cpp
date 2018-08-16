/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (C) 2017 Victor Perez
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

<<<<<<< HEAD:Marlin/src/HAL/HAL_STM32F4/HAL_Servo_STM32F4.cpp
#if defined(STM32F4) || defined(STM32F4xx)
=======
#include "MarlinConfig.h"
>>>>>>> 1.1.x:Marlin/mesh_bed_leveling.cpp

#include "../../inc/MarlinConfig.h"

<<<<<<< HEAD:Marlin/src/HAL/HAL_STM32F4/HAL_Servo_STM32F4.cpp
#if HAS_SERVOS

#include "HAL_Servo_STM32F4.h"
=======
  #include "mesh_bed_leveling.h"
  #include "Marlin.h"
  #include "serial.h"

  mesh_bed_leveling mbl;
>>>>>>> 1.1.x:Marlin/mesh_bed_leveling.cpp

int8_t libServo::attach(const int pin) {
  return Servo::attach(pin);
}

int8_t libServo::attach(const int pin, const int min, const int max) {
  return Servo::attach(pin, min, max);
}

<<<<<<< HEAD:Marlin/src/HAL/HAL_STM32F4/HAL_Servo_STM32F4.cpp
void libServo::move(const int value) {
  constexpr uint16_t servo_delay[] = SERVO_DELAY;
  static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");
  if (this->attach(0) >= 0) {
    this->write(value);
    safe_delay(servo_delay[this->servoIndex]);
    #if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE)
      this->detach();
    #endif
=======
  void mesh_bed_leveling::reset() {
    z_offset = 0;
    ZERO(z_values);
>>>>>>> 1.1.x:Marlin/mesh_bed_leveling.cpp
  }
}
#endif // HAS_SERVOS

<<<<<<< HEAD:Marlin/src/HAL/HAL_STM32F4/HAL_Servo_STM32F4.cpp
#endif // STM32F4 || STM32F4xx
=======
  void mesh_bed_leveling::report_mesh() {
    SERIAL_PROTOCOLLNPGM("Num X,Y: " STRINGIFY(GRID_MAX_POINTS_X) "," STRINGIFY(GRID_MAX_POINTS_Y));
    SERIAL_PROTOCOLPGM("Z offset: "); SERIAL_PROTOCOL_F(z_offset, 5);
    SERIAL_PROTOCOLLNPGM("\nMeasured points:");
    print_2d_array(GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y, 5,
      [](const uint8_t ix, const uint8_t iy) { return z_values[ix][iy]; }
    );
  }

#endif // MESH_BED_LEVELING
>>>>>>> 1.1.x:Marlin/mesh_bed_leveling.cpp
