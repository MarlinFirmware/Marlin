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

#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "fastio.h"
#include "circularqueue.h"

#define TONE_QUEUE_LENGTH 4

struct tone_t {
  uint16_t duration;
  uint16_t frequency;
};

class Buzzer {
  private:
    struct state_t {
      tone_t   tone;
      uint32_t timestamp;
    } state;

  protected:
    CircularQueue<tone_t> buffer
      = CircularQueue<tone_t>(TONE_QUEUE_LENGTH);

    void invert() {
      WRITE(BEEPER_PIN, !READ(BEEPER_PIN));
    }

    void off() {
      WRITE(BEEPER_PIN, LOW);
    }

    void on() {
      WRITE(BEEPER_PIN, HIGH);
    }

    void reset() {
      this->off();
      this->state.timestamp = 0;
    }

  public:
    Buzzer() {
      SET_OUTPUT(BEEPER_PIN);
      this->reset();
    }

    void tone(uint16_t const &duration, uint16_t const &frequency = 0) {
      while (buffer.isFull()) this->tick();
      this->buffer.enqueue((tone_t) { duration, frequency });
    }

    virtual void tick() {
      if (!this->state.timestamp) {
        if (this->buffer.isEmpty()) return;

        this->state.tone = this->buffer.dequeue();
        this->state.timestamp = millis() + this->state.tone.duration;
        if (this->state.tone.frequency > 0) this->on();
      }
      else if (millis() >= this->state.timestamp) this->reset();
    }
};

#endif
