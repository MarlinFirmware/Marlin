#ifndef AnalogBinLogger_h
#define AnalogBinLogger_h
//------------------------------------------------------------------------------
// First block of file.
struct metadata_t {
  unsigned long  adcFrequency;     // ADC clock frequency
  unsigned long  cpuFrequency;     // CPU clock frequency
  unsigned long  sampleInterval;   // Sample interval in CPU cycles.
  unsigned long  recordEightBits;  // Size of ADC values, nonzero for 8-bits.
  unsigned long  pinCount;         // Number of analog pins in a sample.
  unsigned long  pinNumber[123];   // List of pin numbers in a sample.
};
//------------------------------------------------------------------------------
// Data block for 8-bit ADC mode.
const size_t DATA_DIM8 = 508;
struct block8_t {
  unsigned short count;    // count of data values
  unsigned short overrun;  // count of overruns since last block
  unsigned char  data[DATA_DIM8];
};
//------------------------------------------------------------------------------
// Data block for 10-bit ADC mode.
const size_t DATA_DIM16 = 254;
struct block16_t {
  unsigned short count;    // count of data values
  unsigned short overrun;  // count of overruns since last block
  unsigned short data[DATA_DIM16];
};
//------------------------------------------------------------------------------
// Data block for PC use
struct adcdata_t {
  unsigned short count;    // count of data values
  unsigned short overrun;  // count of overruns since last block
  union {
    unsigned char  u8[DATA_DIM8];
    unsigned short u16[DATA_DIM16];
  } data;
};
#endif  // AnalogBinLogger_h