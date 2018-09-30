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

#include "types.h"
#include "fastio.h"
#include "circularqueue.h"
#include "temperature.h"

#include "MarlinConfig.h"

#define TONE_QUEUE_LENGTH 4

/**
 * @brief Tone structure
 * @details Simple abstraction of a tone based on a duration and a frequency.
 */
struct tone_t {
  uint16_t duration;
  uint16_t frequency;
};

/**
 * @brief Buzzer class
 */
class Buzzer {
  private:
    struct state_t {
      tone_t   tone;
      uint32_t endtime;
    } state;

  protected:
    CircularQueue<tone_t, TONE_QUEUE_LENGTH> buffer;

    /**
     * @brief Inverts the sate of a digital PIN
     * @details This will invert the current state of an digital IO pin.
     */
    void invert() {
      TOGGLE(BEEPER_PIN);
    }

    /**
     * @brief Turn off a digital PIN
     * @details Alias of digitalWrite(PIN, LOW) using FastIO
     */
    void off() {
      WRITE(BEEPER_PIN, LOW);
    }

    /**
     * @brief Turn on a digital PIN
     * @details Alias of digitalWrite(PIN, HIGH) using FastIO
     */
    void on() {
      WRITE(BEEPER_PIN, HIGH);
    }

    /**
     * @brief Resets the state of the class
     * @details Brings the class state to a known one.
     */
    void reset() {
      this->off();
      this->state.endtime = 0;
    }

  public:
    /**
     * @brief Class constructor
     */
    Buzzer() {
      SET_OUTPUT(BEEPER_PIN);
      this->reset();
    }

    /**
     * @brief Add a tone to the queue
     * @details Adds a tone_t structure to the ring buffer, will block IO if the
     *          queue is full waiting for one slot to get available.
     *
     * @param duration Duration of the tone in milliseconds
     * @param frequency Frequency of the tone in hertz
     */
    void tone(const uint16_t &duration, const uint16_t &frequency=0) {
      while (buffer.isFull()) {
        this->tick();
        thermalManager.manage_heater();
      }
      tone_t tone = { duration, frequency };
      this->buffer.enqueue(tone);
    }

    /**
     * @brief Loop function
     * @details This function should be called at loop, it will take care of
     *          playing the tones in the queue.
     */
    virtual void tick() {
      const millis_t now = millis();

      if (!this->state.endtime) {
        if (this->buffer.isEmpty()) return;

        this->state.tone = this->buffer.dequeue();
        this->state.endtime = now + this->state.tone.duration;

        if (this->state.tone.frequency > 0) {
          #if ENABLED(SPEAKER)
            CRITICAL_SECTION_START;
            ::tone(BEEPER_PIN, this->state.tone.frequency, this->state.tone.duration);
            CRITICAL_SECTION_END;
          #else
            this->on();
          #endif
        }
      }
      else if (ELAPSED(now, this->state.endtime)) this->reset();
    }
};

extern Buzzer buzzer;

#endif
