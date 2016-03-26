/*
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

#ifndef TWIBUS_H
#define TWIBUS_H

/**
 * TWIBUS class
 *
 * This class implements a wrapper around the two wire (I2C) bus, it allows
 * Marlin to send and request data from any slave device on the bus. This is
 * an experimental feature and it's inner workings as well as public facing
 * interface are prune to change in the future.
 *
 * The two main consumers of this class are M155 and M156, where M155 allows
 * Marlin to send a I2C packet to a device (please be aware that no repeated
 * starts are possible), this can be done in caching method by calling multiple
 * times M155 B<byte-1 value in base 10> or a one liner M155, have a look at
 * the gcode_M155() function for more information. M156 allows Marlin to
 * request data from a device, the received data is then relayed into the serial
 * line for host interpretation.
 *
 */
class twibus
{
    private:
        // Timeout in ms for blocking operations
        const int timeout = 5;

        uint8_t addr = 0;
        uint8_t buffer_s = 0;
        char buffer[30];


    public:
        twibus();
        void reset();
        void send();
        void addbyte(char c);
        void address(uint8_t addr);
        void reqbytes(uint8_t bytes);
};

#endif //TWIBUS_H
