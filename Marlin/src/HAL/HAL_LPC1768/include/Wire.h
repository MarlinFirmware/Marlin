/*
 * TwoWire.h - TWI/I2C library for Arduino & Wiring
 * Copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
 */

// Modified for use with the mcp4451 digipot routine

#ifdef TARGET_LPC1768

#ifndef TwoWire_h
#define TwoWire_h

#include <inttypes.h>

class TwoWire {
  public:
    //TwoWire();
    void begin();
    void beginTransmission(uint8_t);
    uint8_t endTransmission(void);
    size_t write(uint8_t);
};

//extern TwoWire Wire;

TwoWire Wire;

////////////////////////////////////////////////////////////////////////////////////////

extern "C" uint8_t digipot_mcp4451_start(uint8_t sla);
extern "C" void digipot_mcp4451_init(void);
extern "C" uint8_t digipot_mcp4451_send_byte(uint8_t data);


void TwoWire::beginTransmission(uint8_t sla) { digipot_mcp4451_start(sla);}
void TwoWire::begin(void) {digipot_mcp4451_init();}
size_t TwoWire::write(uint8_t data) {return digipot_mcp4451_send_byte(data);}
uint8_t TwoWire::endTransmission(void) {return 1;}

#endif
#endif  // TARGET_LPC1768
