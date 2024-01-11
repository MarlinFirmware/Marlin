/**
 * DWIN Single var plot
 * Author: Miguel A. Risco-Castillo
 * Version: 3.1.3
 * Date: 2023/07/12
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * For commercial applications additional licenses can be requested
 */
#pragma once

#include "dwinui.h"

class Plot {
public:
  static void draw(const frame_rect_t &frame, const_float_t max, const_float_t ref=0);
  static void update(const_float_t value);
};

extern Plot plot;
