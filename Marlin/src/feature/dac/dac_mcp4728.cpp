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

/**
 * mcp4728.cpp - Arduino library for MicroChip MCP4728 I2C D/A converter
 *
 * For implementation details, please take a look at the datasheet:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/22187a.pdf
 *
 * For discussion and feedback, please go to:
 * http://arduino.cc/forum/index.php/topic,51842.0.html
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(DAC_STEPPER_CURRENT)

#include "dac_mcp4728.h"

uint16_t mcp4728_values[XYZE];

/**
 * Begin I2C, get current values (input register and eeprom) of mcp4728
 */
void mcp4728_init() {
  Wire.begin();
  Wire.requestFrom(int(DAC_DEV_ADDRESS), 24);
  while (Wire.available()) {
    char deviceID = Wire.read(),
         hiByte = Wire.read(),
         loByte = Wire.read();

    if (!(deviceID & 0x08))
      mcp4728_values[(deviceID & 0x30) >> 4] = word((hiByte & 0x0F), loByte);
  }
}

/**
 * Write input resister value to specified channel using fastwrite method.
 * Channel : 0-3, Values : 0-4095
 */
uint8_t mcp4728_analogWrite(uint8_t channel, uint16_t value) {
  mcp4728_values[channel] = value;
  return mcp4728_fastWrite();
}

/**
 * Write all input resistor values to EEPROM using SequencialWrite method.
 * This will update both input register and EEPROM value
 * This will also write current Vref, PowerDown, Gain settings to EEPROM
 */
uint8_t mcp4728_eepromWrite() {
  Wire.beginTransmission(DAC_DEV_ADDRESS);
  Wire.write(SEQWRITE);
  LOOP_XYZE(i) {
    Wire.write(DAC_STEPPER_VREF << 7 | DAC_STEPPER_GAIN << 4 | highByte(mcp4728_values[i]));
    Wire.write(lowByte(mcp4728_values[i]));
  }
  return Wire.endTransmission();
}

/**
 * Write Voltage reference setting to all input regiters
 */
uint8_t mcp4728_setVref_all(uint8_t value) {
  Wire.beginTransmission(DAC_DEV_ADDRESS);
  Wire.write(VREFWRITE | (value ? 0x0F : 0x00));
  return Wire.endTransmission();
}
/**
 * Write Gain setting to all input regiters
 */
uint8_t mcp4728_setGain_all(uint8_t value) {
  Wire.beginTransmission(DAC_DEV_ADDRESS);
  Wire.write(GAINWRITE | (value ? 0x0F : 0x00));
  return Wire.endTransmission();
}

/**
 * Return Input Register value
 */
uint16_t mcp4728_getValue(uint8_t channel) { return mcp4728_values[channel]; }

/**
 * Steph: Might be useful in the future
 * Return Vout
 *
uint16_t mcp4728_getVout(uint8_t channel) {
  uint32_t vref = 2048,
           vOut = (vref * mcp4728_values[channel] * (_DAC_STEPPER_GAIN + 1)) / 4096;
  if (vOut > defaultVDD) vOut = defaultVDD;
  return vOut;
}
*/

/**
 * Returns DAC values as a 0-100 percentage of drive strength
 */
uint8_t mcp4728_getDrvPct(uint8_t channel) { return uint8_t(100.0 * mcp4728_values[channel] / (DAC_STEPPER_MAX) + 0.5); }

/**
 * Receives all Drive strengths as 0-100 percent values, updates
 * DAC Values array and calls fastwrite to update the DAC.
 */
void mcp4728_setDrvPct(uint8_t pct[XYZE]) {
  LOOP_XYZE(i) mcp4728_values[i] = 0.01 * pct[i] * (DAC_STEPPER_MAX);
  mcp4728_fastWrite();
}

/**
 * FastWrite input register values - All DAC ouput update. refer to DATASHEET 5.6.1
 * DAC Input and PowerDown bits update.
 * No EEPROM update
 */
uint8_t mcp4728_fastWrite() {
  Wire.beginTransmission(DAC_DEV_ADDRESS);
  LOOP_XYZE(i) {
    Wire.write(highByte(mcp4728_values[i]));
    Wire.write(lowByte(mcp4728_values[i]));
  }
  return Wire.endTransmission();
}

/**
 * Common function for simple general commands
 */
uint8_t mcp4728_simpleCommand(byte simpleCommand) {
  Wire.beginTransmission(GENERALCALL);
  Wire.write(simpleCommand);
  return Wire.endTransmission();
}

#endif // DAC_STEPPER_CURRENT
