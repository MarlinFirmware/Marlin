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

#ifndef __SPEAKER_H__
#define __SPEAKER_H__

#include "Marlin.h"
#include "buzzer.h"

class Speaker: public Buzzer {
  private:
    typedef Buzzer super;

    struct state_t {
      tone_t   tone;
      uint16_t period;
      uint32_t timestamp;
    } state;

  protected:
    void reset() {
      super::reset();
      this->state.period = 0;
      this->state.timestamp = 0;
    }

  public:
    Speaker() {
      this->reset();
    }

    void tick() {
      if (!this->state.timestamp) {
        if (this->buffer.isEmpty()) return;

        this->state.tone = this->buffer.dequeue();
        this->state.timestamp = millis() + this->state.tone.duration;
        // Speed up by pre-calculating the period
        this->state.period = (1000000L / this->state.tone.frequency) >>1;
      }

      else if (millis() >= this->state.timestamp) this->reset();

      else if (this->state.tone.frequency > 0) {
        uint32_t const us = micros();
        static uint32_t next = 0;

        if (us >= next) {
          next = us + this->state.period;
          this->invert();
        }
      }
    }
};

#endif
