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
      millis_t next;
    } state;

  protected:
    /**
     * @brief Resets the state of the class
     * @details Brings the class state to a known one.
     */
    void reset() {
      super::reset();
      this->state.next = 0;
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
      const uint32_t now = millis();

      if (now >= this->state.next) {
        if (this->buffer.isEmpty()) return;

        this->reset();
        this->state.tone = this->buffer.dequeue();
        this->state.next = now + this->state.tone.duration;
        ::tone(BEEPER_PIN, this->state.tone.frequency, this->state.tone.duration);
      }
    }
};

#endif
