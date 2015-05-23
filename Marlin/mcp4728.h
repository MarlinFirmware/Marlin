/**
Arduino library for MicroChip MCP4728 I2C D/A converter.
*/

#ifndef mcp4728_h
#define mcp4728_h

#include "Configuration.h"
#include "Configuration_adv.h"
#include "WProgram.h"
#include <Wire.h>

#ifdef DAC_STEPPER_CURRENT

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


class mcp4728
{
  public:
    mcp4728(uint8_t);
    void     vdd(uint16_t);
    void     begin();
    uint8_t  reset();
    uint8_t  wake();
    uint8_t  update();
    uint8_t  analogWrite(uint16_t, uint16_t, uint16_t, uint16_t);
    uint8_t  analogWrite(uint8_t, uint16_t);
    uint8_t  eepromWrite(uint16_t, uint16_t, uint16_t, uint16_t);
    uint8_t  eepromWrite(uint8_t, uint16_t);
    uint8_t  eepromWrite();
    uint8_t  eepromReset();
    void     print();
    uint8_t  setVref(uint8_t, uint8_t, uint8_t, uint8_t);
    uint8_t  setVref(uint8_t, uint8_t);
    uint8_t  setGain(uint8_t, uint8_t, uint8_t, uint8_t);
    uint8_t  setGain(uint8_t, uint8_t);
    uint8_t  setPowerDown(uint8_t, uint8_t, uint8_t, uint8_t);
    uint8_t  setPowerDown(uint8_t, uint8_t);
    uint8_t  getId();
    uint8_t  getVref(uint8_t);
    uint8_t  getGain(uint8_t);
    uint8_t  getPowerDown(uint8_t);
    uint16_t getValue(uint8_t);
    uint8_t  getVrefEp(uint8_t);
    uint8_t  getGainEp(uint8_t);
    uint8_t  getPowerDownEp(uint8_t);
    uint16_t getValueEp(uint8_t);
    uint16_t    getVout(uint8_t);
    void     voutWrite(uint8_t, uint16_t);
    void     voutWrite(uint16_t, uint16_t, uint16_t, uint16_t);

  private:
    void         getStatus();
    uint8_t      fastWrite();
    uint8_t      multiWrite();
    uint8_t      singleWrite(uint8_t);
    uint8_t      seqWrite();
    uint8_t      writeVref();
    uint8_t      writeGain();
    uint8_t      writePowerDown();
    void         writeVout();
    uint8_t      _dev_address;
    uint8_t      _deviceID;
    uint8_t      _intVref[4];
    uint8_t      _gain[4];
    uint8_t      _powerDown[4];
    uint16_t     _values[4];
    uint16_t     _valuesEp[4];
    uint8_t      _intVrefEp[4];
    uint8_t      _gainEp[4];
    uint8_t      _powerDownEp[4];
    uint16_t     _vOut[4];
    uint16_t     _vdd;
    uint8_t      _simpleCommand(byte);
};
#endif
#endif

