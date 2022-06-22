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
#pragma once

#include "../../../inc/MarlinConfig.h"

#define GT911_SLAVE_ADDRESS   0x28

#if !PIN_EXISTS(GT911_RST)
  #error "GT911_RST_PIN is not defined."
#elif !PIN_EXISTS(GT911_INT)
  #error "GT911_INT_PIN is not defined."
#elif !PIN_EXISTS(GT911_SW_I2C_SCL)
  #error "GT911_SW_I2C_SCL_PIN is not defined."
#elif !PIN_EXISTS(GT911_SW_I2C_SDA)
  #error "GT911_SW_I2C_SDA_PIN is not defined."
#endif

class SW_IIC {
  private:
    uint16_t scl_pin;
    uint16_t sda_pin;
    void write_scl(bool level)
    {
      WRITE(scl_pin, level);
    }
    void write_sda(bool level)
    {
      WRITE(sda_pin, level);
    }
    bool read_sda()
    {
      return READ(sda_pin);
    }
    void set_sda_out()
    {
      SET_OUTPUT(sda_pin);
    }
    void set_sda_in()
    {
      SET_INPUT_PULLUP(sda_pin);
    }
    static void iic_delay(uint8_t t)
    {
      delayMicroseconds(t);
    }

  public:
    SW_IIC(uint16_t sda, uint16_t scl);
    // setSCL/SDA have to be called before begin()
    void setSCL(uint16_t scl)
    {
      scl_pin = scl;
    };
    void setSDA(uint16_t sda)
    {
      sda_pin = sda;
    };
    void init();                // Initialize the IO port of IIC
    void start();               // Send IIC start signal
    void stop();                // Send IIC stop signal
    void send_byte(uint8_t txd); // IIC sends a byte
    uint8_t read_byte(bool ack); // IIC reads a byte
    void send_ack(bool ack);            // IIC sends ACK or NACK signal
    bool read_ack();
};

typedef struct __attribute__((__packed__)) {
  uint8_t xl;
  uint8_t xh;
  uint8_t yl;
  uint8_t yh;
  uint8_t sizel;
  uint8_t sizeh;
  uint8_t reserved;
  uint8_t track_id;
} GT911_POINT;

typedef union __attribute__((__packed__)) {
  uint8_t map[42];
  struct {
    uint8_t status;    // 0x814E
    uint8_t track_id;  // 0x814F

    GT911_POINT point[5]; // [0]:0x8150 - 0x8157 / [1]:0x8158 - 0x815F / [2]:0x8160 - 0x8167 / [3]:0x8168 - 0x816F / [4]:0x8170 - 0x8177
 } REG;
} GT911_REG_MAP;

class GT911 {
  private:
    static const uint8_t gt911_slave_address = GT911_SLAVE_ADDRESS;
    static GT911_REG_MAP reg;
    static SW_IIC sw_iic;
    static void write_reg(uint16_t reg, uint8_t reg_len, uint8_t* w_data, uint8_t w_len);
    static void read_reg(uint16_t reg, uint8_t reg_len, uint8_t* r_data, uint8_t r_len);

  public:
    static void Init();
    static bool getFirstTouchPoint(int16_t *x, int16_t *y);
    static bool getPoint(int16_t *x, int16_t *y);
};
