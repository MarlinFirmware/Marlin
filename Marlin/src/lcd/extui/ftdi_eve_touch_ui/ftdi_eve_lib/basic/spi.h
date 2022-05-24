/*********
 * spi.h *
 *********/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#ifndef CLCD_USE_SOFT_SPI
  #include <SPI.h>
#endif

namespace FTDI {

  #if !defined(CLCD_SPI_BUS) || defined(CLCD_USE_SOFT_SPI)
    #define SPI_OBJ ::SPI
  #else
    extern SPIClass EVE_SPI;
    #define SPI_OBJ EVE_SPI
  #endif

  namespace SPI {
    #ifndef CLCD_USE_SOFT_SPI
      extern SPISettings spi_settings;
    #endif

    uint8_t  _soft_spi_xfer (uint8_t val);
    void     _soft_spi_send (uint8_t val);

    void     spi_init           ();

    void     spi_ftdi_select    ();
    void     spi_ftdi_deselect  ();

    void     spi_flash_select   ();
    void     spi_flash_deselect ();

    inline uint8_t spi_recv() {
      #ifdef CLCD_USE_SOFT_SPI
        return _soft_spi_xfer(0x00);
      #else
        return SPI_OBJ.transfer(0x00);
      #endif
    };

    inline void spi_send (uint8_t val) {
      #ifdef CLCD_USE_SOFT_SPI
        _soft_spi_send(val);
      #else
        SPI_OBJ.transfer(val);
      #endif
    };

    inline void       spi_write_8    (uint8_t val)    {spi_send(val);};
    inline uint8_t    spi_read_8     ()               {return spi_recv();};

    namespace least_significant_byte_first {
      inline void     spi_write_16   (uint16_t val)   {spi_send(val >> 0);
                                                       spi_send(val >> 8);};
      inline void     spi_write_32   (uint32_t val)   {spi_send(val >> 0);
                                                       spi_send(val >> 8);
                                                       spi_send(val >> 16);
                                                       spi_send(val >> 24);};

      inline uint8_t  spi_read_8     ()               {return spi_recv();};
      inline uint16_t spi_read_16    ()               {return (((uint16_t) spi_recv()) <<  0) |
                                                              (((uint16_t) spi_recv()) <<  8);};
      inline uint32_t spi_read_32    ()               {return (((uint32_t) spi_recv()) <<  0) |
                                                              (((uint32_t) spi_recv()) <<  8) |
                                                              (((uint32_t) spi_recv()) << 16) |
                                                              (((uint32_t) spi_recv()) << 24);};
    }

    namespace most_significant_byte_first {
      inline void     spi_write_16   (uint16_t val)   {spi_send(val >> 8);
                                                       spi_send(val >> 0);};
      inline void     spi_write_24   (uint32_t val)   {spi_send(val >> 16);
                                                       spi_send(val >> 8);
                                                       spi_send(val >> 0);};
      inline void     spi_write_32   (uint32_t val)   {spi_send(val >> 24);
                                                       spi_send(val >> 16);
                                                       spi_send(val >> 8);
                                                       spi_send(val >> 0);};

      inline uint16_t spi_read_16    ()               {return (((uint16_t) spi_recv()) <<  8) |
                                                              (((uint16_t) spi_recv()) <<  0);};
      inline uint32_t spi_read_32    ()               {return (((uint32_t) spi_recv()) << 24) |
                                                              (((uint32_t) spi_recv()) << 16) |
                                                              (((uint32_t) spi_recv()) <<  8) |
                                                              (((uint32_t) spi_recv()) <<  0);};
    }

    inline uint8_t ram_write(const uint8_t *p) {return *p;}
    inline uint8_t pgm_write(const uint8_t *p) {return pgm_read_byte(p);}

    typedef uint8_t (*bulk_write_op)(const uint8_t*);

    // Generic template for function for writing multiple bytes, plus padding bytes.
    // The template parameter op is an inlineable function which is applied to each byte.

    template<bulk_write_op byte_op>
    void spi_write_bulk(const void *data, uint16_t len, uint8_t padding) {
      const uint8_t *p = (const uint8_t *)data;
      while (len--)     spi_send(byte_op(p++));
      while (padding--) spi_send(0);
    }

    template<bulk_write_op byte_op>
    void spi_write_bulk(const void *data, uint16_t len) {
      const uint8_t *p = (const uint8_t *)data;
      while (len--) spi_send(byte_op(p++));
    }

    void spi_read_bulk(      void *data, uint16_t len);
    bool spi_verify_bulk(const void *data, uint16_t len);

    void ftdi_reset();
    void test_pulse();
  }
}
