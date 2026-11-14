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

/**
 * adc_ads1115.cpp - Read thermistors with a Texas Instruments ADS1115 16-bit I2C ADC
 *
 * One single-ended conversion is started at a time. Temperature::task polls for the
 * result, so the main loop never waits for a conversion. The ISR can't use this ADC
 * because I2C transfers need interrupts.
 *
 * Unlike an MCU ADC this chip has an internal reference, so a reading depends on the
 * pull-up voltage. Each reading is scaled by that voltage, measured on a spare input
 * with ADS1115_VREF_CHANNEL or else taken from ADS1115_VREF_MV.
 *
 * With ADS1115_RDY_PIN the ALERT/RDY output signals a finished conversion, so the
 * driver can check a pin instead of the I2C bus while the chip is converting.
 */

#include "../../inc/MarlinConfig.h"

#if HAS_ADS1115

#include "adc_ads1115.h"

#include <Wire.h>

#define ADS1115_REG_CONVERSION 0
#define ADS1115_REG_CONFIG     1
#define ADS1115_REG_LO_THRESH  2
#define ADS1115_REG_HI_THRESH  3

// Config register
#define ADS1115_OS         0x8000 // Write: Start a conversion. Read: Conversion finished.
#define ADS1115_MUX(N)     (0x4000 | (uint16_t(N) << 12)) // AINn vs. GND
#define ADS1115_PGA_6V144  0x0000
#define ADS1115_PGA_4V096  0x0200
#define ADS1115_SINGLE     0x0100 // Single-shot mode
#define ADS1115_128SPS     0x0080
#define ADS1115_COMP_OFF   0x0003
#define ADS1115_COMP_QUE_1 0x0000 // ALERT/RDY asserts after one conversion

#define ADS1115_CONV_MS    9      // 128 SPS takes 7.8ms, plus the 10% oscillator tolerance
#define ADS1115_RETRY_MS   10
#define ADS1115_RDY_MS     (2 * (ADS1115_CONV_MS)) // Check over I2C if ALERT/RDY hasn't asserted by now
#define ADS1115_FAULT_MS   1000   // Longest time allowed without a good scan of all inputs

// Use the smallest range that covers the pull-up voltage
#define ADS1115_USE_6V144  ((ADS1115_VREF_MV) > 4096)
#define ADS1115_PGA        (ADS1115_USE_6V144 ? ADS1115_PGA_6V144 : ADS1115_PGA_4V096)
#define ADS1115_NOMINAL    int16_t(int32_t(ADS1115_VREF_MV) * 32768L / (ADS1115_USE_6V144 ? 6144L : 4096L))

#if PIN_EXISTS(ADS1115_RDY)
  #define ADS1115_HAS_RDY 1
#endif

static constexpr uint8_t scan_channels[] = {
  #if TEMP_SENSOR_0_IS_ADS1115
    ADS1115_CHANNEL_0,
  #endif
  #if TEMP_SENSOR_1_IS_ADS1115
    ADS1115_CHANNEL_1,
  #endif
  #if TEMP_SENSOR_2_IS_ADS1115
    ADS1115_CHANNEL_2,
  #endif
  #if TEMP_SENSOR_BED_IS_ADS1115
    ADS1115_CHANNEL_BED,
  #endif
  #if TEMP_SENSOR_CHAMBER_IS_ADS1115
    ADS1115_CHANNEL_CHAMBER,
  #endif
  #ifdef ADS1115_VREF_CHANNEL
    ADS1115_VREF_CHANNEL,
  #endif
};

ADS1115 ads1115;

int16_t ADS1115::code[4];
uint8_t ADS1115::scan_index = 0;
bool ADS1115::converting = false, ADS1115::scanned = false, ADS1115::fault = false;
millis_t ADS1115::next_ms = 0, ADS1115::scan_ms = 0;

#if ADS1115_HAS_RDY
  static bool rdy_set = false, rdy_warned = false; // ALERT/RDY set up as conversion ready, its failure reported
  static millis_t start_ms = 0;                    // When the current conversion was started
#endif

static uint16_t config_for(const uint8_t channel) {
  return ADS1115_OS | ADS1115_MUX(channel) | ADS1115_PGA | ADS1115_SINGLE | ADS1115_128SPS | TERN(ADS1115_HAS_RDY, ADS1115_COMP_QUE_1, ADS1115_COMP_OFF);
}

static bool write_register(const uint8_t reg, const uint16_t value) {
  Wire.beginTransmission(I2C_ADDRESS(ADS1115_ADDRESS));
  Wire.write(reg);
  Wire.write(uint8_t(value >> 8));
  Wire.write(uint8_t(value & 0xFF));
  return Wire.endTransmission() == 0;
}

static bool read_register(const uint8_t reg, uint16_t &value) {
  Wire.beginTransmission(I2C_ADDRESS(ADS1115_ADDRESS));
  Wire.write(reg);
  if (Wire.endTransmission() != 0) return false;
  if (Wire.requestFrom(uint8_t(I2C_ADDRESS(ADS1115_ADDRESS)), uint8_t(2)) != 2) return false;
  value = uint16_t(Wire.read()) << 8;
  value |= uint8_t(Wire.read());
  return true;
}

void ADS1115::init() {
  #if PINS_EXIST(I2C_SCL, I2C_SDA) && DISABLED(SOFT_I2C_EEPROM)
    I2C_SET_PINS(Wire, pin_t(I2C_SDA_PIN), pin_t(I2C_SCL_PIN));
  #endif
  Wire.begin();

  #ifdef __AVR__
    Wire.setWireTimeout(25000, true); // A stuck bus would otherwise block forever
  #endif

  #ifdef ADS1115_VREF_CHANNEL
    code[ADS1115_VREF_CHANNEL] = ADS1115_NOMINAL;
  #endif

  TERN_(ADS1115_HAS_RDY, SET_INPUT_PULLUP(ADS1115_RDY_PIN)); // ALERT/RDY is open-drain

  // Read all inputs now so that the first temperature update has real values
  scan_ms = millis();
  while (!scanned && !fault) {
    update();
    hal.watchdog_refresh();
  }
}

void ADS1115::update() {
  if (fault) return;

  const millis_t ms = millis();

  // Heaters must not run on stale readings
  if (ELAPSED(ms, scan_ms + (ADS1115_FAULT_MS))) {
    fault = true;
    SERIAL_ERROR_MSG("ADS1115 read failed");
    return;
  }

  if (PENDING(ms, next_ms)) return;

  const uint8_t channel = scan_channels[scan_index];
  bool ok = true;

  if (converting) {
    #if ADS1115_HAS_RDY
      // ALERT/RDY is high until the conversion is done. Only a late one needs the I2C check.
      const bool rdy_late = READ(ADS1115_RDY_PIN);
      if (rdy_late && PENDING(ms, start_ms + (ADS1115_RDY_MS))) return;
    #endif

    uint16_t config, value;
    ok = read_register(ADS1115_REG_CONFIG, config)
      && !((config ^ config_for(channel)) & ~uint16_t(ADS1115_OS)); // Is this the chip and input we set up?

    if (ok && !(config & ADS1115_OS)) {             // Still converting
      next_ms = ms + 1;
      return;
    }

    #if ADS1115_HAS_RDY
      // A reset chip has lost the thresholds. Set them up again with the next conversion.
      if (!ok || rdy_late) rdy_set = false;
      if (ok && rdy_late && !rdy_warned) {
        rdy_warned = true;
        SERIAL_WARN_MSG("ADS1115 ALERT/RDY didn't signal");
      }
    #endif

    ok = ok && read_register(ADS1115_REG_CONVERSION, value);

    #ifdef ADS1115_VREF_CHANNEL
      // A pull-up voltage far from nominal means a bad connection, not a usable reference
      if (ok && channel == ADS1115_VREF_CHANNEL)
        ok = WITHIN(int16_t(value), ADS1115_NOMINAL - ADS1115_NOMINAL / 5, ADS1115_NOMINAL + ADS1115_NOMINAL / 5);
    #endif

    if (ok) {
      code[channel] = int16_t(value);
      if (++scan_index >= COUNT(scan_channels)) {
        scan_index = 0;
        scan_ms = ms;
        scanned = true;
      }
    }
  }

  #if ADS1115_HAS_RDY
    // Thresholds with opposite sign bits make ALERT/RDY signal conversion ready
    if (ok && !rdy_set)
      rdy_set = write_register(ADS1115_REG_HI_THRESH, 0x8000) && write_register(ADS1115_REG_LO_THRESH, 0x0000);
    ok = ok && rdy_set;
    start_ms = ms;
  #endif

  converting = ok && write_register(ADS1115_REG_CONFIG, config_for(scan_channels[scan_index]));
  next_ms = ms + (converting ? TERN(ADS1115_HAS_RDY, 1, ADS1115_CONV_MS) : ADS1115_RETRY_MS);
}

raw_adc_t ADS1115::raw(const uint8_t channel, const uint32_t range) {
  const int16_t reading = code[channel];
  if (reading <= 0) return 0;
  const uint32_t scaled = uint32_t(reading) * range / uint32_t(TERN(ADS1115_HAS_VREF_CHANNEL, code[ADS1115_VREF_CHANNEL], ADS1115_NOMINAL));
  return raw_adc_t(_MIN(scaled, range - 1));
}

#endif // HAS_ADS1115
