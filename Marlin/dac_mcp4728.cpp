/*

  mcp4728.cpp - Arduino library for MicroChip MCP4728 I2C D/A converter
  For implementation details, please take a look at the datasheet http://ww1.microchip.com/downloads/en/DeviceDoc/22187a.pdf
  For discussion and feedback, please go to http://arduino.cc/forum/index.php/topic,51842.0.html
*/


/* _____PROJECT INCLUDES_____________________________________________________ */
#include "dac_mcp4728.h"

#if ENABLED(DAC_STEPPER_CURRENT)

// Used Global variables
uint16_t     mcp4728_values[4];

/*
Begin I2C, get current values (input register and eeprom) of mcp4728
*/
void mcp4728_init() {
  Wire.begin();
  Wire.requestFrom(int(DAC_DEV_ADDRESS), 24);
  while(Wire.available()) {
    int deviceID = Wire.receive();
    int hiByte = Wire.receive();
    int loByte = Wire.receive();

    int isEEPROM = (deviceID & 0B00001000) >> 3;
    int channel = (deviceID & 0B00110000) >> 4;
    if (isEEPROM != 1) {
      mcp4728_values[channel] = word((hiByte & 0B00001111), loByte);
    }
  }
}

/*
Write input resister value to specified channel using fastwrite method.
Channel : 0-3, Values : 0-4095
*/
uint8_t mcp4728_analogWrite(uint8_t channel, uint16_t value) {
  mcp4728_values[channel] = value;
  return mcp4728_fastWrite();
}
/*
Write all input resistor values to EEPROM using SequencialWrite method.
This will update both input register and EEPROM value
This will also write current Vref, PowerDown, Gain settings to EEPROM
*/
uint8_t mcp4728_eepromWrite() {
  Wire.beginTransmission(DAC_DEV_ADDRESS);
  Wire.send(SEQWRITE);
  for (uint8_t channel=0; channel <= 3; channel++) {
    Wire.send(DAC_STEPPER_VREF << 7 | 0 << 5 | DAC_STEPPER_GAIN << 4 | highByte(mcp4728_values[channel]));
    Wire.send(lowByte(mcp4728_values[channel]));
  }
  return Wire.endTransmission();
}

/*
  Write Voltage reference setting to all input regiters
*/
uint8_t mcp4728_setVref_all(uint8_t value) {
  Wire.beginTransmission(DAC_DEV_ADDRESS);
  Wire.send(VREFWRITE | value << 3 | value << 2 | value << 1 | value);
  return Wire.endTransmission();
}
/*
  Write Gain setting to all input regiters
*/
uint8_t mcp4728_setGain_all(uint8_t value) {
  Wire.beginTransmission(DAC_DEV_ADDRESS);
  Wire.send(GAINWRITE | value << 3 | value << 2 | value << 1 | value);
  return Wire.endTransmission();
}

/*
  Return Input Regiter value
*/
uint16_t mcp4728_getValue(uint8_t channel) { return mcp4728_values[channel]; }

/*
// Steph: Might be useful in the future
// Return Vout
uint16_t mcp4728_getVout(uint8_t channel) {
  uint32_t vref = 2048;
  uint32_t vOut = (vref * mcp4728_values[channel] * (_DAC_STEPPER_GAIN + 1)) / 4096;
  if (vOut > defaultVDD) vOut = defaultVDD;
  return vOut;
}
*/

/*
FastWrite input register values - All DAC ouput update. refer to DATASHEET 5.6.1
DAC Input and PowerDown bits update.
No EEPROM update
*/
uint8_t mcp4728_fastWrite() {
  Wire.beginTransmission(DAC_DEV_ADDRESS);
  for (uint8_t channel=0; channel <= 3; channel++) {
    Wire.send(highByte(mcp4728_values[channel]));
    Wire.send(lowByte(mcp4728_values[channel]));
  }
  return Wire.endTransmission();
}

/*
Common function for simple general commands
*/
uint8_t mcp4728_simpleCommand(byte simpleCommand) {
  Wire.beginTransmission(GENERALCALL);
  Wire.send(simpleCommand);
  return Wire.endTransmission();
}

#endif // DAC_STEPPER_CURRENT
