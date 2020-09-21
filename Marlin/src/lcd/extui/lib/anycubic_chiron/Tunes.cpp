/********************
 * Tunes.cpp        *
 *******************/

/****************************************************************************
 *   Written By Nick Wells 2020 [https://github.com/SwiftNick]              * 
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
 *                                                                          *
 *   A Utility to play tunes using the buzzer in the printer controller.    *
 *   See tunes.h for note and tune definitions.                             *
 ***************************************************************************/

#include "Tunes.h"
#include "../../ui_api.h"
#if BOTH(ANYCUBIC_LCD_CHIRON, EXTENSIBLE_UI)
namespace Anycubic {
  void PlayTune(uint8_t beeperPin, const uint16_t * tune, uint8_t speed = 1 ) {
    uint8_t pos = 1;
    uint16_t wholenotelen = tune[0]/speed;
    do {
      uint16_t freq = tune[pos];
      uint16_t notelen = wholenotelen/tune[pos+1];  

      tone(beeperPin,freq,notelen);
      ExtUI::delay_ms(notelen);
      pos +=2;

      if(pos >= MAX_TUNE_LENGTH) break;
    } while (tune[pos] != n_END);
  }
}
#endif