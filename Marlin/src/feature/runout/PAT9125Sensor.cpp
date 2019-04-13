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

#include "../../inc/MarlinConfigPre.h"

#if FRS(PAT9125)

#include "PAT9125Sensor.h"

bool FilamentSensorPAT9125::operative,
     FilamentSensorPAT9125::motion_detected;
int16_t FilamentSensorPAT9125::y,
        FilamentSensorPAT9125::resolutionSteps;

bool FilamentSensorPAT9125::updateY(void) {
  if (!operative) return false;

  uint8_t ucMotion = readRegister(PAT9125_MOTION);
  if (!operative) return false;

  if (ucMotion & 0x80) {
    uint8_t ucYL = readRegister(PAT9125_DELTA_YL);
    uint8_t ucXYH = readRegister(PAT9125_DELTA_XYH);
    if (!operative) return false;

    int16_t iDY = ucYL | ((ucXYH << 8) & 0xf00);
    if (iDY & 0x800) iDY -= 4096;
    y -= iDY; // negative number, because direction switching does not work
  }

  return true;
}

uint8_t FilamentSensorPAT9125::readRegister(const uint8_t addr) {
  uint8_t data;
  if (!swi2c_readByte_A8(PAT9125_I2C_ADDR, addr, &data)) {
    operative = false;
    return 0;
  }
  return data;
}

void FilamentSensorPAT9125::writeRegister(const uint8_t addr, const uint8_t data) {
  if (!swi2c_writeByte_A8(PAT9125_I2C_ADDR, addr, data))
    operative = false;
}

#endif // PAT9125
