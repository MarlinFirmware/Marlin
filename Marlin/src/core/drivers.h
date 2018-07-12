/**
* Marlin 3D Printer Firmware
* Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef DRIVERS_H
#define DRIVERS_H

#define A4988            0x00
#define DRV8825          0x01
#define LV8729           0x02
#define L6470            0x03
#define TB6560           0x04
#define TB6600           0x05
#define TMC2100          0x06
#define TMC2130          0x07
#define TMC2208          0x08
#define TMC26X           0x09
#define TMC2660          0x0A
#define TMC5130          0x0B
#define DRIVER_TYPE_MASK 0xFF
#define SPI_STEPPER      0x100

#define IS_DRIVER_USED(a,t)     (defined(a##_DRIVER_TYPE) && (a##_DRIVER_TYPE & DRIVER_TYPE_MASK) == t)
#define HAVE_DRIVER(t)          (IS_DRIVER_USED(X,t)  || IS_DRIVER_USED(X2,t) || \
                                 IS_DRIVER_USED(Y,t)  || IS_DRIVER_USED(Y2,t) || \
                                 IS_DRIVER_USED(Z,t)  || IS_DRIVER_USED(Z2,t) || \
                                 IS_DRIVER_USED(E0,t) || IS_DRIVER_USED(E1,t) || \
                                 IS_DRIVER_USED(E2,t) || IS_DRIVER_USED(E3,t) || \
                                 IS_DRIVER_USED(E4,t))

#define IS_SPIDRIVER_USED(a,t)  (IS_DRIVER_USED(a,t) && (a##_DRIVER_TYPE & SPI_STEPPER))
#define HAVE_SPIDRIVER(t)       (IS_SPIDRIVER_USED(X,t)  || IS_SPIDRIVER_USED(X2,t) || \
                                 IS_SPIDRIVER_USED(Y,t)  || IS_SPIDRIVER_USED(Y2,t) || \
                                 IS_SPIDRIVER_USED(Z,t)  || IS_SPIDRIVER_USED(Z2,t) || \
                                 IS_SPIDRIVER_USED(E0,t) || IS_SPIDRIVER_USED(E1,t) || \
                                 IS_SPIDRIVER_USED(E2,t) || IS_SPIDRIVER_USED(E3,t) || \
                                 IS_SPIDRIVER_USED(E4,t))

#endif // DRIVERS_H
