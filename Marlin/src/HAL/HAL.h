/* **************************************************************************

 Marlin 3D Printer Firmware
 Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com

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
// Description:          *** HAL wrapper ***
//
// Supports platforms : 
//     ARDUINO_ARCH_SAM
//     ARDUINO_ARCH_AVR
// **************************************************************************

#ifndef _HAL_H
#define _HAL_H

#ifdef ARDUINO_ARCH_SAM

#include "HAL_DUE/HAL_Due.h"

#elif defined(ARDUINO_ARCH_AVR)

#include "HAL_AVR/HAL_AVR.h"

#else

#error Unsupported Platform!

#endif

#endif /* HAL_H_ */
