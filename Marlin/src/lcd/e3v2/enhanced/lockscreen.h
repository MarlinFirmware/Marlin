/**
 * DWIN UI Enhanced implementation
 * Author: Miguel A. Risco-Castillo
 * Version: 3.6.1
 * Date: 2021/08/29
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

#include "../../../core/types.h"

class LockScreenClass {
private:
  uint8_t Lock_Pos = 0;
  bool unlocked = false;
public:
  void Init();
  void onEncoderState(ENCODER_DiffState encoder_diffState);
  void Draw();
  bool isUnlocked();
};
extern LockScreenClass LockScreen;
