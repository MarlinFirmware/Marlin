/**
Arduino library for MicroChip MCP4728 I2C D/A converter.
*/

#ifndef mcp4728_h
#define mcp4728_h

#include "Configuration.h"
#include "Configuration_adv.h"

#if ENABLED(DAC_STEPPER_CURRENT)
#include "WProgram.h"
#include "Wire.h"
//#include <Wire.h>

#define defaultVDD 5000
#define BASE_ADDR 0x60
#define RESET 0B00000110
#define WAKE 0B00001001
#define UPDATE 0B00001000
#define MULTIWRITE 0B01000000
#define SINGLEWRITE 0B01011000
#define SEQWRITE 0B01010000
#define VREFWRITE 0B10000000
#define GAINWRITE 0B11000000
#define POWERDOWNWRITE 0B10100000
#define GENERALCALL 0B0000000
#define GAINWRITE 0B11000000

// This is taken from the original lib, makes it easy to edit if needed
#define DAC_DEV_ADDRESS (BASE_ADDR | 0x00)

void mcp4728_init();
uint8_t mcp4728_analogWrite(uint8_t channel, uint16_t value);
uint8_t mcp4728_eepromWrite();
uint8_t mcp4728_setVref_all(uint8_t value);
uint8_t mcp4728_setGain_all(uint8_t value);
uint16_t mcp4728_getValue(uint8_t channel);
uint8_t mcp4728_fastWrite();
uint8_t mcp4728_simpleCommand(byte simpleCommand);

#endif
#endif

