/**************
 * usb_host.h *
 **************/

/****************************************************************************
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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

/* This the following comes from "lib/usbhost.h", but has been rewritten
 * to use the SPI functions from Marlin's HAL */

class MAX3421e {
  private:
    static uint8_t vbusState;
    void cs();
    void ncs();

    uint8_t GpxHandler();
    uint8_t IntHandler();

  public:
    bool      start();

    void      regWr(uint8_t reg, uint8_t data);
    uint8_t*  bytesWr(uint8_t reg, uint8_t nbytes, uint8_t* data_p);
    void      gpioWr(uint8_t data);
    uint8_t   regRd(uint8_t reg);
    uint8_t*  bytesRd(uint8_t reg, uint8_t nbytes, uint8_t* data_p);
    uint8_t   gpioRd();
    bool      reset();

    uint8_t   getVbusState(void)   {return vbusState;};

    void busprobe();

    uint8_t Task();
};

#define USE_MARLIN_MAX3421E

#if defined(__SAM3X8E__) && !defined(ARDUINO_SAM_DUE)
  #define ARDUINO_SAM_DUE // Spoof the USB library that this is a DUE
#endif
