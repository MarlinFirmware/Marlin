/***********
 * rgb_t.h *
 ***********/

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

struct rgb_t {
    union {
      struct {
        uint8_t  b,g,r,a;
      };
      uint32_t packed;
    };

    rgb_t()                                : packed(0)              {}
    rgb_t(uint32_t rgb)                    : packed(rgb)            {}
    rgb_t(uint8_t r, uint8_t g, uint8_t b) : b(b), g(g), r(r), a(0) {}
    operator uint32_t() const              {return packed;};

    static void lerp(float t, const rgb_t a, const rgb_t b, rgb_t &c) {
      c.r = a.r + t * (b.r - a.r);
      c.g = a.g + t * (b.g - a.g);
      c.b = a.b + t * (b.b - a.b);
    }

    uint8_t luminance() const {return 0.299*r + 0.587*g + 0.114*b;}
};
