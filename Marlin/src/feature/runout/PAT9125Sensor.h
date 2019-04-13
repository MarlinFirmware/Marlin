/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../../core/swi2c.h"
#include "../../module/stepper.h"

// PAT9125 registers
#define PAT9125_PID1            0x00
#define PAT9125_PID2            0x01
#define PAT9125_MOTION          0x02
#define PAT9125_DELTA_YL        0x04
#define PAT9125_RES_X           0x0d
#define PAT9125_RES_Y           0x0e
#define PAT9125_DELTA_XYH       0x12

// PAT9125 configuration
#define PAT9125_I2C_ADDR  0x75  //ID=LO
#define PAT9125_XRES      0
#define PAT9125_YRES      240

class FilamentSensorPAT9125 {
  private:
    constexpr static float sensorResolution =
      #ifdef SENSOR_RESOLUTION
        SENSOR_RESOLUTION
      #else
        0.64
      #endif
    ;
    static bool operative, motion_detected;
    static int16_t y, resolutionSteps;

    static bool updateY(void);

    static uint8_t readRegister(const uint8_t addr);
    static void writeRegister(const uint8_t addr, const uint8_t data);

public:
    static inline bool setup() {
      operative = false;

      SERIAL_ECHOPGM("PAT9125::init ");
      swi2c_init();
      // Verify that the sensor responds with its correct product ID.
      uint8_t pat9125_PID1 = readRegister(PAT9125_PID1);
      uint8_t pat9125_PID2 = readRegister(PAT9125_PID2);
      if (pat9125_PID1 != 0x31 || pat9125_PID2 != 0x91) {
        pat9125_PID1 = readRegister(PAT9125_PID1);
        pat9125_PID2 = readRegister(PAT9125_PID2);
        if (pat9125_PID1 != 0x31 || pat9125_PID2 != 0x91) {
          SERIAL_ECHOPAIR("failed (PID1=", pat9125_PID1);
          SERIAL_ECHOPAIR(" PID2=", pat9125_PID2);
          SERIAL_ECHOLNPGM(")");

          return false;
        }
      }

      writeRegister(PAT9125_RES_X, PAT9125_XRES);
      writeRegister(PAT9125_RES_Y, PAT9125_YRES);
      operative = true;

      SERIAL_ECHOPAIR("RES_X=", readRegister(PAT9125_RES_X));
      SERIAL_ECHOLNPAIR(" RES_Y=", readRegister(PAT9125_RES_Y));

      initResolutionSteps();

      return true;
    }

    static inline void initResolutionSteps() {
      resolutionSteps = (int16_t)(sensorResolution * planner.settings.axis_steps_per_mm[E_AXIS] * 2 / 3);
    }

    static inline int16_t getResolutionSteps() { return resolutionSteps; }

    static inline void reset() { motion_detected = true; }

    static inline void checkMotion() {
      //if (updateY()) {
        //if (y != oldY) {
          motion_detected = true;
        //  oldY = y;
      //}
    }

    static inline bool getRunoutState() {
        return !motion_detected;
    }
};
