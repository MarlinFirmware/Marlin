/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "swi2c.h"
#include "../HAL/shared/Delay.h"


#if FRS(PAT9125)

#define SWI2C_RMSK   0x01               // read mask (bit0 = 1)
#define SWI2C_WMSK   0x00               // write mask (bit0 = 0)
#define SWI2C_ASHF   1                  // address shift (<< 1)
#define SWI2C_TMO    2048               // 2048 cycles timeout
#define I2CDELAY()   DELAY_US(1.25)     // delay to generate a 400KHz clock

void swi2c_init(void) {
  SET_OUTPUT(SDA_PIN);
  SET_OUTPUT(SCL_PIN);
  WRITE(SDA_PIN, HIGH);
  WRITE(SCL_PIN, HIGH);
  DELAY_US(125);
}

void swi2c_start(void) {
  WRITE(SDA_PIN, LOW);
  I2CDELAY();
  WRITE(SCL_PIN, LOW);
  I2CDELAY();
}

void swi2c_stop(void) {
  WRITE(SCL_PIN, HIGH);
  I2CDELAY();
  WRITE(SDA_PIN, HIGH);
  I2CDELAY();
}

void swi2c_ack(void) {
  WRITE(SDA_PIN, LOW);
  I2CDELAY();
  WRITE(SCL_PIN, HIGH);
  I2CDELAY();
  WRITE(SCL_PIN, LOW);
  I2CDELAY();
}

bool swi2c_wait_ack() {
  SET_INPUT(SDA_PIN);
  I2CDELAY();
  WRITE(SCL_PIN, HIGH);
  bool noAck;
  uint16_t ackto = SWI2C_TMO;
  while ((noAck = READ(SDA_PIN)) && ackto--) I2CDELAY();
  WRITE(SCL_PIN, LOW);
  I2CDELAY();
  SET_OUTPUT(SDA_PIN);
  I2CDELAY();
  WRITE(SDA_PIN, LOW);
  I2CDELAY();

  return !noAck;
}

uint8_t swi2c_read(void) {
  WRITE(SDA_PIN, HIGH);
  I2CDELAY();
  SET_INPUT(SDA_PIN);
  uint8_t data = 0;
  for (int8_t  bit = 7; bit >= 0; bit--) {
    WRITE(SCL_PIN, HIGH);
    I2CDELAY();
    data |= (READ(SDA_PIN) ? 1 : 0) << bit;
    WRITE(SCL_PIN, LOW);
    I2CDELAY();
  }
  SET_OUTPUT(SDA_PIN);
  return data;
}

void swi2c_write(uint8_t data) {
  for (int8_t bit = 7; bit >= 0; --bit) {
    WRITE(SDA_PIN, (data & 0x80) ? HIGH : LOW);
    data <<= 1;
    I2CDELAY();
    WRITE(SCL_PIN, HIGH);
    I2CDELAY();
    WRITE(SCL_PIN, LOW);
    I2CDELAY();
  }
}

bool swi2c_readByte_A8(const uint8_t dev_addr, const uint8_t addr, uint8_t* pbyte) {
  swi2c_start();
  swi2c_write(SWI2C_WMSK | (dev_addr << SWI2C_ASHF));
  const bool ack = swi2c_wait_ack();
  if (ack) {
    swi2c_write(addr);
    if (!swi2c_wait_ack()) return false;
    swi2c_stop();
    swi2c_start();
    swi2c_write(SWI2C_RMSK | (dev_addr << SWI2C_ASHF));
    if (!swi2c_wait_ack()) return false;
    const uint8_t byte = swi2c_read();
    if (pbyte) *pbyte = byte;
    }
  swi2c_stop();
  return ack;
}

bool swi2c_writeByte_A8(const uint8_t dev_addr, const uint8_t addr, const uint8_t data) {
  swi2c_start();
  swi2c_write(SWI2C_WMSK | (dev_addr << SWI2C_ASHF));
  const bool ack = swi2c_wait_ack();
  if (ack) {
    swi2c_write(addr);
    if (!swi2c_wait_ack()) return false;
    swi2c_write(data);
    if (!swi2c_wait_ack()) return false;
  }
  swi2c_stop();
  return ack;
}

#endif // FRS(PAT9125)
