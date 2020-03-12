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


#if ENABLED(USE_CONTROLLER_FAN)

  typedef struct {
    uint8_t controllerFan_Speed;        // 0-255 - 255 == fullspeed; Controller fan speed on motors enabled
    uint8_t controllerFan_Idle_Speed;   // 0-255 - 255 == fullspeed; Controller fan Idle speed if all motors are disabled
    uint16_t controllerFan_Duration;    // Duration in seconds for the fan to run after all motors are disabled
    bool controllerFan_AutoMode;        // Default true
  } controllerFan_settings_t;

  class ControllerFan {
    private:
      uint8_t iFanSpeed;

    public:
      void update();
      bool state();
      void reset();
      void init();
      void setup();
      controllerFan_settings_t settings_fan;
  };
  extern ControllerFan fanController;

#endif
