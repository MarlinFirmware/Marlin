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

#include "buzzer.h"

class Speaker: public Buzzer {
  private:
    typedef Buzzer super;

    struct state_t {
      tone_t   tone;
      uint16_t period;
      uint16_t counter;
    } state;

  protected:
    /**
     * @brief Resets the state of the class
     * @details Brings the class state to a known one.
     */
    void reset() {
      super::reset();
      this->state.period = 0;
      this->state.counter = 0;
    }

  public:
    /**
     * @brief Class constructor
     */
    Speaker() {
      this->reset();
    }

    /**
     * @brief Loop function
     * @details This function should be called at loop, it will take care of
     * playing the tones in the queue.
     */
    virtual void tick() {
      if (!this->state.counter) {
        if (this->buffer.isEmpty()) return;

        this->reset();
        this->state.tone = this->buffer.dequeue();

        // Period is uint16, min frequency will be ~16Hz
        this->state.period = 1000000UL / this->state.tone.frequency;

        this->state.counter =
          (this->state.tone.counter * 1000L) / this->state.period;

        this->state.period   >>= 1;
        this->state.counter <<= 1;
      } else {
        const  uint32_t  now = micros();
        static uint32_t next = now + this->state.period;

        if (now >= next) {
          --this->state.counter;
          next = now + this->state.period;
          if (this->state.tone.frequency > 0) this->invert();
        }
      }
    }
};

#endif
