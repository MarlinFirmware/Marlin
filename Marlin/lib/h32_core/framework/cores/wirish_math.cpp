/*
 * Modified by LeafLabs, LLC.
 *
 * Part of the Wiring project - http://wiring.org.co Copyright (c)
 * 2004-06 Hernando Barragan Modified 13 August 2006, David A. Mellis
 * for Arduino - http://www.arduino.cc/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include "stdlib.h"
#include "wirish_math.h"

void randomSeed(unsigned int seed)
{
    if (seed != 0)
    {
        srand(seed);
    }
}

long random(long howbig)
{
    if (howbig == 0)
    {
        return 0;
    }

    return rand() % howbig;
}

long random(long howsmall, long howbig)
{
    if (howsmall >= howbig)
    {
        return howsmall;
    }

    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}

extern uint16_t makeWord(uint16_t w)
{
    return w;
}

extern uint16_t makeWord(uint8_t h, uint8_t l)
{
    return (h << 8) | l;
}
