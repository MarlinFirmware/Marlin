/*
 Contributors:
    Copyright (c) 2014 Bob Cousins bobcousins42@googlemail.com
*/
/* **************************************************************************
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

// **************************************************************************
//
// Description: Fast IO functions for Arduino Due
//
// ARDUINO_ARCH_SAM
// **************************************************************************

#ifndef	_FASTIO_H
#define	_FASTIO_H



// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

/// Read a pin
#define READ(IO) digitalRead(IO)
/// Write to a pin
#define WRITE(IO, v) digitalWrite(IO,v)

/// toggle a pin
#define TOGGLE(IO)

/// set pin as input
#define SET_INPUT(IO) pinMode (IO,INPUT)
/// set pin as output
#define SET_OUTPUT(IO) pinMode (IO,OUTPUT)

/// check if pin is an input
#define GET_INPUT(IO)
/// check if pin is an output
#define GET_OUTPUT(IO)

/// check if pin is an timer
#define GET_TIMER(IO)

#endif	/* _FASTIO_H */
