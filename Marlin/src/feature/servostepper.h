/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * servo stepper : allow the use of a servo in place of a stepper.
 */

class ServoStepper {
  public:
    ServoStepper(const uint8_t servoIndex);
    void enable(const uint8_t enabled);
    uint8_t isEnabled();
    void setDir(const uint8_t direction);
    uint8_t getDir();
    void doStep(const uint8_t step);
    uint8_t getStep();

  private:
    uint8_t enabled;
    uint8_t servoIndex;               // index into the channel data for this servo
    uint16_t currentPosition;
    uint16_t lastPositionSent;
    uint8_t currentDir;
    uint8_t previousStepWrite;
};
