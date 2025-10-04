/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfig.h"

#if ENABLED(I2C_AMMETER)
#include "ammeter.h"

#ifndef I2C_AMMETER_SDA_PIN
#define I2C_AMMETER_SDA_PIN SDA
#endif
#ifndef I2C_AMMETER_SCL_PIN
#define I2C_AMMETER_SCL_PIN SCL
#endif
#ifndef I2C_AMMETER_ADDRESS
#define I2C_AMMETER_ADDRESS 0x40
#endif
#ifndef I2C_AMMETER_IMAX
#define I2C_AMMETER_IMAX 0.500 // Calibration range 500 Milliamps
#endif

#ifndef I2C_AMMETER_MONITOR_CHANNEL
#define I2C_AMMETER_MONITOR_CHANNEL 1
#endif

Ammeter ammeter;
float Ammeter::current;
uint32_t Ammeter::currentLSB;
int Ammeter::sensor_type = 0;

SlowSoftI2CMaster si = SlowSoftI2CMaster(I2C_AMMETER_SDA_PIN, I2C_AMMETER_SCL_PIN, true);
uint16_t _ammeter_read_register(uint8_t reg)
{
  if (!si.i2c_start((I2C_AMMETER_ADDRESS << 1) | I2C_WRITE))
    return -1; // No ACK from slave

  if (!si.i2c_write(reg))
    return -1; // failed write

  delayMicroseconds(600); // max 586us from datasheet

  if (!si.i2c_rep_start((I2C_AMMETER_ADDRESS << 1) | I2C_READ))
    return -2; // no ACK from slave on second transaction

  uint16_t ret = (si.i2c_read(false) << 8 | si.i2c_read(true));

  si.i2c_stop();

  #ifdef I2C_AMMETER_DEBUGGING
   SERIAL_ECHO("read: ");
   SERIAL_PRINT(I2C_AMMETER_ADDRESS, PrintBase::Hex);
   SERIAL_ECHO(":");
   SERIAL_PRINT(reg, PrintBase::Hex);
   SERIAL_ECHO(": ");
   SERIAL_PRINT(ret, PrintBase::Hex); 
    SERIAL_ECHO(" (");
   SERIAL_PRINT(ret, PrintBase::Dec);
   SERIAL_ECHOLN(")");
   #endif
  return ret;
}

void _ammeter_write_register(uint8_t reg, uint16_t value)
{
  if (!si.i2c_start((I2C_AMMETER_ADDRESS << 1) | I2C_WRITE))
    return; // No ACK from slave

  si.i2c_write(reg);
  si.i2c_write((value >> 8) & 0xFF);
  si.i2c_write(value & 0xFF);
  si.i2c_stop();
}
void Ammeter::init()
{
  if (si.i2c_init())
  {
    if (_ammeter_read_register(0xFE) == 0x5449)  // "TI"
    {

      uint16_t id = _ammeter_read_register(0xFF);
      sensor_type = id ;////TEMPTEST

      #ifdef I2C_AMMETER_DEBUGGING
        SERIAL_ECHO("TI I2C Device: 0x");
        SERIAL_PRINT(id, PrintBase::Hex);
        SERIAL_ECHO(" @ 0x");
        SERIAL_PRINT(I2C_AMMETER_ADDRESS, PrintBase::Hex);
        SERIAL_ECHOLNPGM(" OK ( SDA: ", I2C_AMMETER_SDA_PIN, " SCL: ", I2C_AMMETER_SCL_PIN, " )");
      #endif
      
      switch (sensor_type)
      {
      case 0x3220:
        //_ammeter_write_register(0x00, 0x927 | (0x8000 >> 7)); // [CH_x_EN, AVERAGES_256, BUS_CONV_TIME_1100US, SHUNT_CONV_TIME_1100US, MODE_SHUNT_BUS_CONT]
        TERN_(I2C_AMMETER_WARN_CURRENT_MA, _ammeter_write_register(0x6 + (I2C_AMMETER_MONITOR_CHANNEL * 2), I2C_AMMETER_WARN_CURRENT_MA / I2C_AMMETER_IMAX);)
        TERN_(I2C_AMMETER_ALARM_CURRENT_MA, _ammeter_write_register(0x5 + (I2C_AMMETER_MONITOR_CHANNEL * 2), I2C_AMMETER_ALARM_CURRENT_MA / I2C_AMMETER_IMAX);)
        break;
        
      case 0x2260:
      {
        currentLSB = ((I2C_AMMETER_IMAX / 32767) * 100000000);
        currentLSB /= 100000000;
        currentLSB /= 0.0001; // CHOP CHOP?
        currentLSB = ceil(currentLSB);
        currentLSB *= 0.0001;

        _ammeter_write_register(0x00, 0x927); // [AVERAGES_256, BUS_CONV_TIME_1100US, SHUNT_CONV_TIME_1100US, MODE_SHUNT_BUS_CONT]
        _ammeter_write_register(0x05, (uint16_t)((0.00512) / (currentLSB * I2C_AMMETER_SHUNT_RESISTOR)));
        break;
      }
      default: {
            #ifdef I2C_AMMETER_DEBUGGING
            SERIAL_ECHO("I2C Device: 0x");
            SERIAL_PRINT(sensor_type, PrintBase::Hex);
            SERIAL_ECHOLNPGM(" not supported");
            #endif
            
            break;
      }
      }
      return;
    }

    
  }
}

float Ammeter::read(uint8_t channel) { 
if (sensor_type != 0x3220) return -1;

  uint16_t i = _ammeter_read_register((channel * 2) - 1) ;
  
  if (i & 0x8000) i = ~i + 1;  // ABS(i) (Two's complement)
    #ifdef I2C_AMMETER_DEBUGGING
      SERIAL_ECHO("read(");
      SERIAL_PRINT(channel,PrintBase::Dec);
      SERIAL_ECHO("): ");
      SERIAL_ECHOLN((I2C_AMMETER_IMAX * I2C_AMMETER_SHUNT_RESISTOR) *  i);
    #endif

  return (I2C_AMMETER_IMAX * I2C_AMMETER_SHUNT_RESISTOR) * i;
}

int16_t Ammeter::readVoltage(uint8_t channel) {
  if (sensor_type != 0x3220) return -1;

  return _ammeter_read_register(channel * 2);  
}

float Ammeter::read() {
  switch (sensor_type) {
  case 0x3220: // INA3221
    current = _ammeter_read_register((I2C_AMMETER_MONITOR_CHANNEL * 2) - 1) * (I2C_AMMETER_IMAX * I2C_AMMETER_SHUNT_RESISTOR);
    break;

  case 0x2260: // INA226
    current = _ammeter_read_register(0x04) * currentLSB;
    break;

  default:
    //Stub for future implementations
    break;
  }
  return current;
}

#endif // I2C_AMMETER
