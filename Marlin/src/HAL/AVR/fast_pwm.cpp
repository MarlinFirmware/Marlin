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
#ifdef __AVR__

#include "../../inc/MarlinConfig.h"

//#define DEBUG_AVR_FAST_PWM
#define DEBUG_OUT ENABLED(DEBUG_AVR_FAST_PWM)
#include "../../core/debug_out.h"

struct Timer {
  volatile uint8_t* TCCRnQ[3];  // max 3 TCCR registers per timer
  volatile uint16_t* OCRnQ[3];  // max 3 OCR registers per timer
  volatile uint16_t* ICRn;      // max 1 ICR register per timer
  uint8_t n;                    // the timer number [0->5]
  uint8_t q;                    // the timer output [0->2] (A->C)
  bool isPWM;                   // True if pin is a "hardware timer"
  bool isProtected;             // True if timer is protected
};

// Macros for the Timer structure
#define _SET_WGMnQ(T, V) do{ \
    *(T.TCCRnQ)[0] = (*(T.TCCRnQ)[0] & ~(0x3 << 0)) | (( int(V)       & 0x3) << 0); \
    *(T.TCCRnQ)[1] = (*(T.TCCRnQ)[1] & ~(0x3 << 3)) | (((int(V) >> 2) & 0x3) << 3); \
  }while(0)

// Set TCCR CS bits
#define _SET_CSn(T, V) (*(T.TCCRnQ)[1] = (*(T.TCCRnQ[1]) & ~(0x7 << 0)) | ((int(V) & 0x7) << 0))

// Set TCCR COM bits
#define _SET_COMnQ(T, Q, V) (*(T.TCCRnQ)[0] = (*(T.TCCRnQ)[0] & ~(0x3 << (6-2*(Q)))) | (int(V) << (6-2*(Q))))

// Set OCRnQ register
#define _SET_OCRnQ(T, Q, V) (*(T.OCRnQ)[Q] = int(V) & 0xFFFF)

// Set ICRn register (one per timer)
#define _SET_ICRn(T, V) (*(T.ICRn) = int(V) & 0xFFFF)

/**
 * Return a Timer struct describing a pin's timer.
 */
const Timer get_pwm_timer(const pin_t pin) {

  uint8_t q = 0;

  switch (digitalPinToTimer(pin)) {
    #ifdef TCCR0A
      IF_DISABLED(AVR_AT90USB1286_FAMILY, case TIMER0A:)
    #endif
    #ifdef TCCR1A
      case TIMER1A: case TIMER1B:
    #endif

    break;    // Protect reserved timers (TIMER0 & TIMER1)

    #ifdef TCCR0A
      case TIMER0B:   // Protected timer, but allow setting the duty cycle on OCR0B for pin D4 only
        return Timer({ { &TCCR0A, nullptr, nullptr }, { (uint16_t*)&OCR0A, (uint16_t*)&OCR0B, nullptr }, nullptr, 0, 1, true, true });
    #endif

    #if HAS_TCCR2
      case TIMER2:
        return Timer({ { &TCCR2, nullptr, nullptr }, { (uint16_t*)&OCR2, nullptr, nullptr }, nullptr, 2, 0, true, false });
    #elif ENABLED(USE_OCR2A_AS_TOP)
      case TIMER2A: break; // Protect TIMER2A since its OCR is used by TIMER2B
      case TIMER2B:
        return Timer({ { &TCCR2A, &TCCR2B, nullptr }, { (uint16_t*)&OCR2A, (uint16_t*)&OCR2B, nullptr }, nullptr, 2, 1, true, false });
    #elif defined(TCCR2A)
      case TIMER2B: ++q; case TIMER2A:
        return Timer({ { &TCCR2A, &TCCR2B, nullptr }, { (uint16_t*)&OCR2A, (uint16_t*)&OCR2B, nullptr }, nullptr, 2, q, true, false });
    #endif

    #ifdef OCR3C
      case TIMER3C: ++q; case TIMER3B: ++q; case TIMER3A:
        return Timer({ { &TCCR3A, &TCCR3B, &TCCR3C }, { &OCR3A, &OCR3B, &OCR3C  }, &ICR3, 3, q, true, false });
    #elif defined(OCR3B)
      case TIMER3B: ++q; case TIMER3A:
        return Timer({ { &TCCR3A, &TCCR3B, nullptr }, { &OCR3A, &OCR3B, nullptr }, &ICR3, 3, q, true, false });
    #endif

    #ifdef TCCR4A
      case TIMER4C: ++q; case TIMER4B: ++q; case TIMER4A:
        return Timer({ { &TCCR4A, &TCCR4B, &TCCR4C }, { &OCR4A, &OCR4B, &OCR4C }, &ICR4, 4, q, true, false });
    #endif

    #ifdef TCCR5A
      case TIMER5C: ++q; case TIMER5B: ++q; case TIMER5A:
        return Timer({ { &TCCR5A, &TCCR5B, &TCCR5C }, { &OCR5A, &OCR5B, &OCR5C }, &ICR5, 5, q, true, false });
    #endif
  }

  return Timer();
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired) {
  DEBUG_ECHOLNPGM("set_pwm_frequency(pin=", pin, ", freq=", f_desired, ")");
  const Timer timer = get_pwm_timer(pin);
  if (timer.isProtected || !timer.isPWM) return; // Don't proceed if protected timer or not recognized

  const bool is_timer2 = timer.n == 2;
  const uint16_t maxtop = is_timer2 ? 0xFF : 0xFFFF;

  DEBUG_ECHOLNPGM("maxtop=", maxtop);

  uint16_t res = 0xFF;        // resolution (TOP value)
  uint8_t j = CS_NONE;        // prescaler index
  uint8_t wgm = WGM_PWM_PC_8; // waveform generation mode

  // Calculating the prescaler and resolution to use to achieve closest frequency
  if (f_desired != 0) {
    constexpr uint16_t prescaler[] = { 1, 8, (32), 64, (128), 256, 1024 };  // (*) are Timer 2 only
    uint16_t f = (F_CPU) / (uint32_t(maxtop) << 11) + 1;      // Start with the lowest non-zero frequency achievable (for 16MHz, 1 or 31)

    DEBUG_ECHOLNPGM("f=", f);
    DEBUG_ECHOLNPGM("(prescaler loop)");
    for (uint8_t i = 0; i < COUNT(prescaler); ++i) {          // Loop through all prescaler values
      const uint32_t p = prescaler[i];                        // Extend to 32 bits for calculations
      DEBUG_ECHOLNPGM("prescaler[", i, "]=", p);
      uint16_t res_fast_temp, res_pc_temp;
      if (is_timer2) {
        #if ENABLED(USE_OCR2A_AS_TOP)                         // No resolution calculation for TIMER2 unless enabled USE_OCR2A_AS_TOP
          const uint16_t rft = (F_CPU) / (p * f_desired);
          res_fast_temp = rft - 1;
          res_pc_temp = rft / 2;
          DEBUG_ECHOLNPGM("(Timer2) res_fast_temp=", res_fast_temp, " res_pc_temp=", res_pc_temp);
        #else
          res_fast_temp = res_pc_temp = maxtop;
          DEBUG_ECHOLNPGM("(Timer2) res_fast_temp=", maxtop, " res_pc_temp=", maxtop);
        #endif
      }
      else {
        if (p == 32 || p == 128) continue;                    // Skip TIMER2 specific prescalers when not TIMER2
        const uint16_t rft = (F_CPU) / (p * f_desired);
        DEBUG_ECHOLNPGM("(Not Timer 2) F_CPU=", STRINGIFY(F_CPU), " prescaler=", p, " f_desired=", f_desired);
        res_fast_temp = rft - 1;
        res_pc_temp = rft / 2;
      }

      LIMIT(res_fast_temp, 1U, maxtop);
      LIMIT(res_pc_temp, 1U, maxtop);

      // Calculate frequencies of test prescaler and resolution values
      const uint16_t f_fast_temp = (F_CPU) / (p * (1 + res_fast_temp)),
                     f_pc_temp   = (F_CPU) / ((p * res_pc_temp) << 1),
                     f_diff      = _MAX(f, f_desired) - _MIN(f, f_desired),
                     f_fast_diff = _MAX(f_fast_temp, f_desired) - _MIN(f_fast_temp, f_desired),
                     f_pc_diff   = _MAX(f_pc_temp, f_desired) - _MIN(f_pc_temp, f_desired);

      DEBUG_ECHOLNPGM("f_fast_temp=", f_fast_temp, " f_pc_temp=", f_pc_temp, " f_diff=", f_diff, " f_fast_diff=", f_fast_diff, " f_pc_diff=", f_pc_diff);

      if (f_fast_diff < f_diff && f_fast_diff <= f_pc_diff) { // FAST values are closest to desired f
        // Set the Wave Generation Mode to FAST PWM
        wgm = is_timer2 ? uint8_t(TERN(USE_OCR2A_AS_TOP, WGM2_FAST_PWM_OCR2A, WGM2_FAST_PWM)) : uint8_t(WGM_FAST_PWM_ICRn);
        // Remember this combination
        f = f_fast_temp; res = res_fast_temp; j = i + 1;
        DEBUG_ECHOLNPGM("(FAST) updated f=", f);
      }
      else if (f_pc_diff < f_diff) {                          // PHASE CORRECT values are closes to desired f
        // Set the Wave Generation Mode to PWM PHASE CORRECT
        wgm = is_timer2 ? uint8_t(TERN(USE_OCR2A_AS_TOP, WGM2_PWM_PC_OCR2A, WGM2_PWM_PC)) : uint8_t(WGM_PWM_PC_ICRn);
        f = f_pc_temp; res = res_pc_temp; j = i + 1;
        DEBUG_ECHOLNPGM("(PHASE) updated f=", f);
      }
    } // prescaler loop
  }

  _SET_WGMnQ(timer, wgm);
  _SET_CSn(timer, j);

  if (is_timer2) {
    TERN_(USE_OCR2A_AS_TOP, _SET_OCRnQ(timer, 0, res)); // Set OCR2A value (TOP) = res
  }
  else
    _SET_ICRn(timer, res);                              // Set ICRn value (TOP) = res
}

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
  // If v is 0 or v_size (max), digitalWrite to LOW or HIGH.
  // Note that digitalWrite also disables PWM output for us (sets COM bit to 0)
  if (v == 0)
    digitalWrite(pin, invert);
  else if (v == v_size)
    digitalWrite(pin, !invert);
  else {
    const Timer timer = get_pwm_timer(pin);
    if (timer.isPWM) {
      if (timer.n == 0) {
        _SET_COMnQ(timer, timer.q, COM_CLEAR_SET);  // Only allow a TIMER0B select...
        _SET_OCRnQ(timer, timer.q, v);              // ...and OCR0B duty update. For output pin D4 no frequency changes are permitted.
      }
      else if (!timer.isProtected) {
        const uint16_t top = timer.n == 2 ? TERN(USE_OCR2A_AS_TOP, *timer.OCRnQ[0], 255) : *timer.ICRn;
        _SET_COMnQ(timer, SUM_TERN(HAS_TCCR2, timer.q, timer.q == 2), COM_CLEAR_SET + invert);   // COM20 is on bit 4 of TCCR2, so +1 for q==2
        _SET_OCRnQ(timer, timer.q, uint16_t(uint32_t(v) * top / v_size)); // Scale 8/16-bit v to top value
      }
    }
    else
      digitalWrite(pin, v < v_size / 2 ? LOW : HIGH);
  }
}

void MarlinHAL::init_pwm_timers() {
  // Init some timer frequencies to a default 1KHz
  const pin_t pwm_pin[] = {
    #ifdef __AVR_ATmega2560__
      10, 5, 6, 46
    #elif defined(__AVR_ATmega1280__)
      12, 31
    #elif defined(__AVR_ATmega644__) || defined(__AVR_ATmega1284__)
      15, 6
    #elif defined(__AVR_AT90USB1286__) || defined(__AVR_mega64) || defined(__AVR_mega128)
      16, 24
    #endif
  };

  for (uint8_t i = 0; i < COUNT(pwm_pin); ++i)
    set_pwm_frequency(pwm_pin[i], 1000);
}

#endif // __AVR__
