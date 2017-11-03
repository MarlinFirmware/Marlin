/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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


// use this file to create the public interface for device drivers that are NOT in the U8G library

extern u8g_dev_t u8g_dev_st7565_64128n_HAL_2x_sw_spi;
extern u8g_dev_t u8g_dev_st7565_64128n_HAL_2x_hw_spi;
class U8GLIB_64128N_2X_HAL : public U8GLIB
{
  public:
    U8GLIB_64128N_2X_HAL(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_64128n_HAL_2x_sw_spi, sck, mosi, cs, a0, reset)
      { }
    U8GLIB_64128N_2X_HAL(uint8_t cs, uint8_t a0, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7565_64128n_HAL_2x_hw_spi, cs, a0, reset)
      { }
};


extern u8g_dev_t u8g_dev_st7920_128x64_HAL_4x_sw_spi;
extern u8g_dev_t u8g_dev_st7920_128x64_HAL_4x_hw_spi;
class U8GLIB_ST7920_128X64_4X_HAL : public U8GLIB
{
  public:
    U8GLIB_ST7920_128X64_4X_HAL(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_HAL_4x_sw_spi, sck, mosi, cs, U8G_PIN_NONE, reset)    // a0 = U8G_PIN_NONE
      { }
    U8GLIB_ST7920_128X64_4X_HAL(uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_HAL_4x_hw_spi, cs, U8G_PIN_NONE, reset)   // a0 = U8G_PIN_NONE
      { }
};



extern u8g_dev_t u8g_dev_st7920_128x64_rrd_sw_spi;
class U8GLIB_ST7920_128X64_RRD : public U8GLIB
{
  public:
    U8GLIB_ST7920_128X64_RRD(uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t reset = U8G_PIN_NONE)
      : U8GLIB(&u8g_dev_st7920_128x64_rrd_sw_spi, sck, mosi, cs, U8G_PIN_NONE, reset)   // a0 = U8G_PIN_NONE
      { }
};



extern u8g_dev_t u8g_dev_sh1106_128x64_2x_i2c_2_wire;
class U8GLIB_SH1106_128X64_2X_I2C_2_WIRE : public U8GLIB {
  public:
    U8GLIB_SH1106_128X64_2X_I2C_2_WIRE(uint8_t options = U8G_I2C_OPT_NONE)
    : U8GLIB(&u8g_dev_sh1106_128x64_2x_i2c_2_wire, options)
    {  }
};



extern u8g_dev_t u8g_dev_ssd1306_128x64_2x_i2c_2_wire;
class U8GLIB_SSD1306_128X64_2X_I2C_2_WIRE : public U8GLIB {
  public:
    U8GLIB_SSD1306_128X64_2X_I2C_2_WIRE(uint8_t options = U8G_I2C_OPT_NONE)
    : U8GLIB(&u8g_dev_ssd1306_128x64_2x_i2c_2_wire, options)
    {  }
};
