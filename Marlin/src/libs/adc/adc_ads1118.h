/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Based on Arduino Library for Texas Instruments ADS1118 - 16-Bit Analog-to-Digital Converter with internal Reference and Temperature Sensor
 * https://www.ti.com/product/ADS1118
 * https://github.com/ADS1xxx-Series-ADC-Libraries/ADS1118
 */

#include "../../inc/MarlinConfigPre.h"

// ADS config register bits and values:
// Bit 15: Single-shot conversion start
#define ADS_SS_NOP   0x0000
#define ADS_SS_START 0x8000
// Bits 14-12 Mux
#define INPUT_CHAN_0_1 0x0000 // *Default*
#define INPUT_CHAN_0_3 0x1000
#define INPUT_CHAN_1_3 0x2000
#define INPUT_CHAN_2_3 0x3000
#define INPUT_CHAN_0_G 0x4000
#define INPUT_CHAN_1_G 0x5000
#define INPUT_CHAN_2_G 0x6000
#define INPUT_CHAN_3_G 0x7000

/// Bits 11-9 ADC PGA gain select bits
/// the gain setting sets the voltage range for the ADC.  Full Scale Range
/// voltage is the read value at 0x7FFF  (the ADC returns a 16bit integer integer value)
/// we use the highest possible gain setting - k-Type thermocouples have a voltage
/// difference of ~12mV at 300C
#define PGA_0_6_14		0x0000  // Gain = 1, Full Scale Voltage is 6.14V
#define PGA_1_4_09		0x0200  // Gain = 1.5, Full Scale Voltage is 4.09V
#define PGA_2_2_04		0x0400  // Gain = 3, Full Scale Voltage is 2.04V *Default*
#define PGA_3_1_02		0x0600  // Gain = 6, Full Scale Voltage is 1.02V
#define PGA_4_0_512		0x0800  // Gain = 12, Full Scale Voltage is 0.512V
#define PGA_5_0_256		0x0A00  // Gain = 24, Full Scale Voltage is 0.256V

/// Bit 8: operating mode: single sample or continous conversion
#define CONTINUOUS_CONVERSION_MODE		0x0000 // continous conversion
#define SINGLE_SHOT_MODE		          0x0100 // single sample

/// Bit 7-5: Data Rate, Sample Frequency select bits (Hz)
#define SAMPLE_FREQ_860  0x00E0
#define SAMPLE_FREQ_475  0x00C0
#define SAMPLE_FREQ_250  0x00A0
#define SAMPLE_FREQ_128  0x0080  //* default
#define SAMPLE_FREQ_64	 0x0060
#define SAMPLE_FREQ_32   0x0040
#define SAMPLE_FREQ_16   0x0020
#define SAMPLE_FREQ_08   0x0000

/// Bit 4: ADC mode (thermocouples) vs temperature sensor (on-board cold_junction temp sensor)
#define ADC_MODE    0x0000
#define TEMP_MODE   0x0010

/// Bit 3: Pull up enable
#define PULL_UP_DISABLE  0x0000
#define PULL_UP_ENABLE   0x0008

/// write new data to the config register ( if bits <2:1> are not <01> the config bytes are ignored)
#define ADS_NOP     	0x0000
#define WRITE_CONFIG	0x0002

/// Number of read cycles between cold junction temperature reads
/// we don't need to read the cold junction temperature every cycle
/// because we don't expect it to change much
#define TEMP_CHECK_COUNT 120

#define THERM_CHANNEL_ONE	0
#define THERM_CHANNEL_TWO	1
#define THERM_CHANNEL_HBP	2
#define THERM_COLD_JUNCTION	3

typedef struct {
  int16_t adc;
  int16_t temp;
} ADC_Lookup;

const static ADC_Lookup table_thermocouple_k[] PROGMEM = {
  { -304, -64},
  { -232, -48},
  { -157, -32},
  {  -79, -16},
  {    0,   0},
  {   82,  16},
  {  164,  32},
  {  248,  48},
  {  333,  64},
  {  418,  80},
  {  503,  96},
  {  588, 112},
  {  672, 128},
  {  755, 144},
  {  837, 160},
  {  919, 176},
  { 1001, 192},
  { 1083, 208},
  { 1165, 224},
  { 1248, 240},
  { 1331, 256},
  { 1415, 272},
  { 1499, 288},
  { 1584, 304},
  { 1754, 336},
  { 1840, 352},
  { 1926, 368},
  { 2012, 384},
  { 2099, 400}
};

#define TEMP_TABLE_SIZE (sizeof(table_thermocouple_k) / sizeof(table_thermocouple_k[0]))
#define TEMP_MIN_TEMP     0
#define TEMP_MAX_TEMP   300
#define TEMP_TABLE_OFFSET 0 // grados Celsius por índice

class ADS1118 {
  public:
    static void init(uint8_t cs, uint8_t mosi, uint8_t miso, uint8_t sck);
    static void startConversion(const uint8_t pair); // start conversion
    static bool ready();                             // indicates whether it is already ready to be read
    static int16_t read();                           // read converted value
    static bool busy();                              // conversion status
    static void loop();                              // non‑blocking cycle

    static bool checkDataReady();
    static void startContinuousConversion(const uint8_t channel_pair);
    static uint16_t readData();
    static int16_t readChannel(const uint8_t channel);
    static uint16_t readConfig ();
    static uint16_t readWriteData(const uint16_t config);

    float convertInternalTemp(const int16_t data);
    float readInternalTemp();

    uint16_t config_ADC_SS_CH0 = ADS_SS_START | INPUT_CHAN_0_1| PGA_5_0_256 | SINGLE_SHOT_MODE| SAMPLE_FREQ_128 | ADC_MODE | PULL_UP_ENABLE | WRITE_CONFIG;
    uint16_t config_ADC_SS_CH1 = ADS_SS_START | INPUT_CHAN_2_3 | PGA_5_0_256 | SINGLE_SHOT_MODE| SAMPLE_FREQ_128 | ADC_MODE | PULL_UP_ENABLE | WRITE_CONFIG;
    uint16_t config_ADC_SS_TEMP = ADS_SS_START  | PGA_5_0_256 | SINGLE_SHOT_MODE | SAMPLE_FREQ_128 | TEMP_MODE | PULL_UP_ENABLE | WRITE_CONFIG;

    static uint16_t current_config;
    static uint16_t previous_config;

  private:
    static void spiTransfer(uint8_t data, uint8_t &resp);
    static void writeWord(uint16_t word);
    static void readWord(uint16_t &word);

    static uint32_t transfer32 (uint16_t data);
    static uint16_t transfer16 (uint16_t data);
    static uint8_t transfer8 (uint8_t data);


    static void select();
    static void deselect();

    static void sckHigh();
    static void sckLow();

    static uint8_t cs_pin, mosi_pin, miso_pin, sck_pin;
    static unsigned long startTime;
    static int16_t lastValue;
    static int16_t config;

    static bool isBusy;
    static uint8_t currentchannel;

    //uint16_t configForChannel(const uint8_t channel);
    static uint16_t configChannel(const uint8_t channel);
    float readVoltage(const uint8_t channel, const float vref);
};

class ThermocoupleK {
  public:
    void init();
    float tempReadtoCelsius(int16_t rawADC);
    float calcTempCelsius();
    float getTempCelsius();
    float _Tcold,  _Thot;
    int16_t _raw_cold,  _raw_hot;

    void setRawCold(int16_t raw_cold);
    void setRawHot(int16_t raw_hot);

    void setTcold(float tcold);
    float getTcold();

    void setThot(float thot);
    float getThot();
};

extern ADS1118 ads1118;

extern ThermocoupleK thck_0;
extern ThermocoupleK thck_1;
