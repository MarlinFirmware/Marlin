/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if BOTH(DIGIPOT_I2C, DIGIPOT_MCP4018)

#include "Stream.h"
#include "utility/twi.h"
#include <SlowSoftI2CMaster.h>  //https://github.com/stawel/SlowSoftI2CMaster

// Settings for the I2C based DIGIPOT (MCP4018) based on WT150

#define DIGIPOT_A4988_Rsx               0.250
#define DIGIPOT_A4988_Vrefmax           1.666
#define DIGIPOT_A4988_MAX_VALUE         127

#define DIGIPOT_A4988_Itripmax(Vref)    ((Vref)/(8.0*DIGIPOT_A4988_Rsx))

#define DIGIPOT_A4988_FACTOR            ((DIGIPOT_A4988_MAX_VALUE)/DIGIPOT_A4988_Itripmax(DIGIPOT_A4988_Vrefmax))
#define DIGIPOT_A4988_MAX_CURRENT       2.0

static byte current_to_wiper(const float current) {
  const int16_t value = ceil(float(DIGIPOT_A4988_FACTOR) * current);
  return byte(constrain(value, 0, DIGIPOT_A4988_MAX_VALUE));
}

const uint8_t sda_pins[DIGIPOT_I2C_NUM_CHANNELS] = {
  DIGIPOTS_I2C_SDA_X
  #if DIGIPOT_I2C_NUM_CHANNELS > 1
    , DIGIPOTS_I2C_SDA_Y
    #if DIGIPOT_I2C_NUM_CHANNELS > 2
      , DIGIPOTS_I2C_SDA_Z
      #if DIGIPOT_I2C_NUM_CHANNELS > 3
        , DIGIPOTS_I2C_SDA_E0
        #if DIGIPOT_I2C_NUM_CHANNELS > 4
          , DIGIPOTS_I2C_SDA_E1
        #endif
      #endif
    #endif
  #endif
};

static SlowSoftI2CMaster pots[DIGIPOT_I2C_NUM_CHANNELS] = {
  SlowSoftI2CMaster { sda_pins[X_AXIS], DIGIPOTS_I2C_SCL }
  #if DIGIPOT_I2C_NUM_CHANNELS > 1
    , SlowSoftI2CMaster { sda_pins[Y_AXIS], DIGIPOTS_I2C_SCL }
    #if DIGIPOT_I2C_NUM_CHANNELS > 2
      , SlowSoftI2CMaster { sda_pins[Z_AXIS], DIGIPOTS_I2C_SCL }
      #if DIGIPOT_I2C_NUM_CHANNELS > 3
        , SlowSoftI2CMaster { sda_pins[E_AXIS], DIGIPOTS_I2C_SCL }
        #if DIGIPOT_I2C_NUM_CHANNELS > 4
          , SlowSoftI2CMaster { sda_pins[E_AXIS + 1], DIGIPOTS_I2C_SCL }
        #endif
      #endif
    #endif
  #endif
};

static void i2c_send(const uint8_t channel, const byte v) {
  if (WITHIN(channel, 0, DIGIPOT_I2C_NUM_CHANNELS - 1)) {
    pots[channel].i2c_start(((DIGIPOT_I2C_ADDRESS_A) << 1) | I2C_WRITE);
    pots[channel].i2c_write(v);
    pots[channel].i2c_stop();
  }
}

// This is for the MCP4018 I2C based digipot
void digipot_i2c_set_current(const uint8_t channel, const float current) {
  i2c_send(channel, current_to_wiper(_MIN(_MAX(current, 0), float(DIGIPOT_A4988_MAX_CURRENT))));
}

void digipot_i2c_init() {
  static const float digipot_motor_current[] PROGMEM = DIGIPOT_I2C_MOTOR_CURRENTS;

  for (uint8_t i = 0; i < DIGIPOT_I2C_NUM_CHANNELS; i++)
    pots[i].i2c_init();

  // setup initial currents as defined in Configuration_adv.h
  for (uint8_t i = 0; i < COUNT(digipot_motor_current); i++)
    digipot_i2c_set_current(i, pgm_read_float(&digipot_motor_current[i]));
}

#endif // DIGIPOT_I2C && DIGIPOT_MCP4018
