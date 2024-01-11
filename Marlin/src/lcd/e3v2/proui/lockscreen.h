/**
 * Lock screen implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 4.1.3
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
 */
#pragma once

#include "../common/encoder.h"
#include <stdint.h>

class LockScreen {
private:
  static bool unlocked;
  static uint8_t lock_pos;
public:
  static uint8_t rprocess;
  static void init();
  static void onEncoder(EncoderState encoder_diffState);
  static void draw();
  static bool isUnlocked() { return unlocked; }
};

extern LockScreen lockScreen;
