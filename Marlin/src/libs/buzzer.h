/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../inc/MarlinConfig.h"

#if USE_BEEPER

  #include "circularqueue.h"

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
    public:

      typedef struct {
        tone_t   tone;
        uint32_t endtime;
      } state_t;

    private:
      static state_t state;

    protected:
      static CircularQueue<tone_t, TONE_QUEUE_LENGTH> buffer;

      /**
       * @brief Inverts the state of a digital PIN
       * @details This will invert the current state of an digital IO pin.
       */
      FORCE_INLINE static void invert() { TOGGLE(BEEPER_PIN); }

      /**
       * @brief Turn off a digital PIN
       * @details Alias of digitalWrite(PIN, LOW) using FastIO
       */
      FORCE_INLINE static void off() { WRITE(BEEPER_PIN, LOW); }

      /**
       * @brief Turn on a digital PIN
       * @details Alias of digitalWrite(PIN, HIGH) using FastIO
       */
      FORCE_INLINE static void on() { WRITE(BEEPER_PIN, HIGH); }

      /**
       * @brief Resets the state of the class
       * @details Brings the class state to a known one.
       */
      static void reset() {
        off();
        state.endtime = 0;
      }

    public:
      /**
       * @brief Init Buzzer
       */
      static void init() {
        SET_OUTPUT(BEEPER_PIN);
        reset();
      }

      /**
       * @brief Add a tone to the queue
       * @details Adds a tone_t structure to the ring buffer, will block IO if the
       *          queue is full waiting for one slot to get available.
       *
       * @param duration Duration of the tone in milliseconds
       * @param frequency Frequency of the tone in hertz
       */
      static void tone(const uint16_t duration, const uint16_t frequency=0);

      /**
       * @brief Tick function
       * @details This function should be called at loop, it will take care of
       *          playing the tones in the queue.
       */
      static void tick();
  };

  // Provide a buzzer instance
  extern Buzzer buzzer;

  // Buzz directly via the BEEPER pin tone queue
  #define BUZZ(d,f) buzzer.tone(d, f)

#elif HAS_BUZZER

  // Buzz indirectly via the MarlinUI instance
  #include "../lcd/marlinui.h"
  #define BUZZ(d,f) ui.buzz(d,f)

#else

  // No buzz capability
  #define BUZZ(d,f) NOOP

#endif
