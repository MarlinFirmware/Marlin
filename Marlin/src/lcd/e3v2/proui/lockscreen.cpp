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

#include "../../../inc/MarlinConfigPre.h"

#if ALL(DWIN_LCD_PROUI, HAS_LOCKSCREEN)

#include "dwin.h"
#include "dwinui.h"
#include "lockscreen.h"

LockScreen lockScreen;

uint8_t LockScreen::lock_pos = 0;
bool LockScreen::unlocked = false;
uint8_t LockScreen::rprocess = 0;

void LockScreen::init() {
  lock_pos = 0;
  unlocked = false;
  draw();
}

void LockScreen::draw() {
  title.draw(GET_TEXT_F(MSG_LOCKSCREEN));
  DWINUI::clearMainArea();
  DWINUI::drawIcon(ICON_LOGO, 71, 120);  // CREALITY logo
  DWINUI::drawCenteredString(COLOR_WHITE, 180, GET_TEXT_F(MSG_LOCKSCREEN_LOCKED));
  DWINUI::drawCenteredString(COLOR_WHITE, 200, GET_TEXT_F(MSG_LOCKSCREEN_UNLOCK));
  DWINUI::drawCenteredString(COLOR_WHITE, 240, F("-> | <-"));
  dwinDrawBox(1, hmiData.colorBarfill, 0, 260, DWIN_WIDTH, 20);
  dwinDrawVLine(COLOR_YELLOW, lock_pos * DWIN_WIDTH / 255, 260, 20);
  dwinUpdateLCD();
}

void LockScreen::onEncoder(EncoderState encoder_diffState) {
  switch (encoder_diffState) {
    case ENCODER_DIFF_CW:    lock_pos += 8; break;
    case ENCODER_DIFF_CCW:   lock_pos -= 8; break;
    case ENCODER_DIFF_ENTER: unlocked = (lock_pos == 128); break;
    default: break;
  }
  dwinDrawBox(1, hmiData.colorBarfill, 0, 260, DWIN_WIDTH, 20);
  dwinDrawVLine(COLOR_YELLOW, lock_pos * DWIN_WIDTH / 255, 260, 20);
  dwinUpdateLCD();
}

#endif // DWIN_LCD_PROUI && HAS_LOCKSCREEN
