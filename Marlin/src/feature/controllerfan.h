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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../inc/MarlinConfigPre.h"

typedef struct {
  uint8_t   active_speed,    // 0-255 (fullspeed); Speed with enabled stepper motors
            idle_speed;      // 0-255 (fullspeed); Speed after idle period with all motors are disabled
  uint16_t  duration;        // Duration in seconds for the fan to run after all motors are disabled
  bool      auto_mode;       // Default true
} controllerFan_settings_t;

#if ENABLED(USE_CONTROLLER_FAN)

class ControllerFan {
  private:
    static uint8_t speed;
    static void set_fan_speed(const uint8_t s);

  public:
    static controllerFan_settings_t settings;
    static inline bool state() { return speed > 0; }
    static inline void init() { reset() };
    static void reset();
    static void setup();
    static void update();
};

extern ControllerFan controllerFan;

#endif
