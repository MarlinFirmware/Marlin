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

/**
 * Arduino API implementation for Atmel SAM3U (AT91SAM3U4E)
 *
 * See include/Arduino.h for the surface this provides and why it exists.
 */

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"
#include "clock.h"
#include "adc.h"

// ------------------------
// Time base
// ------------------------

/**
 * SysTick ticks once per millisecond. micros() interpolates within the current
 * millisecond from the SysTick reload counter, which counts *down* from
 * SYSTICK_RELOAD to zero.
 */
#define SYSTICK_RELOAD ((F_CPU) / 1000UL - 1UL)

static volatile uint32_t systick_millis = 0;

extern "C" void SysTick_Handler() { systick_millis++; }

void sam3u_systick_init() {
  SysTick->LOAD = SYSTICK_RELOAD;
  SysTick->VAL  = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
  // Lowest priority, so the stepper and temperature ISRs always win
  NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
}

extern "C" uint32_t millis() { return systick_millis; }

extern "C" uint32_t micros() {
  uint32_t ms, cycles;
  // Re-read if SysTick wrapped between the two reads, so ms and the counter
  // always come from the same millisecond.
  do {
    ms = systick_millis;
    cycles = SysTick->VAL;
    __DMB();
  } while (ms != systick_millis);
  return ms * 1000UL + (SYSTICK_RELOAD - cycles) / ((F_CPU) / 1000000UL);
}

extern "C" void delay(const uint32_t ms) {
  if (!ms) return;
  const uint32_t start = millis();
  while (millis() - start < ms) { /* wait */ }
}

extern "C" void delayMicroseconds(const uint32_t us) {
  if (!us) return;
  const uint32_t start = micros();
  while (micros() - start < us) { /* wait */ }
}

// ------------------------
// Digital I/O
// ------------------------

void pinMode(const pin_t pin, const uint8_t mode) {
  if (!VALID_PIN(pin)) return;

  Pio * const port = SAM3U_PIO(pin);
  const uint32_t mask = SAM3U_MASK(pin);

  pmc_enable_periph_clk(SAM3U_PIO_ID(pin));

  // Take the pin back from any peripheral that may own it
  port->PIO_PER = mask;

  switch (mode) {
    case OUTPUT:
      port->PIO_PUDR = mask;   // No pull-up on an output
      port->PIO_OER  = mask;
      break;
    case INPUT_PULLUP:
      port->PIO_PUER = mask;
      port->PIO_ODR  = mask;
      break;
    // The SAM3U PIO has pull-ups only - no pull-downs. INPUT_PULLDOWN falls
    // through to a plain floating input; see inc/SanityCheck.h.
    default:
      port->PIO_PUDR = mask;
      port->PIO_ODR  = mask;
      break;
  }
}

void digitalWrite(const pin_t pin, const uint8_t value) {
  if (!VALID_PIN(pin)) return;
  Pio * const port = SAM3U_PIO(pin);
  const uint32_t mask = SAM3U_MASK(pin);
  if (value) port->PIO_SODR = mask; else port->PIO_CODR = mask;
}

bool digitalRead(const pin_t pin) {
  if (!VALID_PIN(pin)) return false;
  return bool(SAM3U_PIO(pin)->PIO_PDSR & SAM3U_MASK(pin));
}

// ------------------------
// Analog
// ------------------------

uint16_t analogRead(const pin_t pin) { return sam3u_adc_read_pin(pin); }

/**
 * The SAM3U's PWM controller has only four channels, and none of the 4pi's
 * heater or FET outputs are routed to them in a usable combination, so this
 * HAL drives those loads with Marlin's software PWM (SOFT_PWM_SCALE /
 * FAN_SOFT_PWM, exactly as the original 4pi firmware did). analogWrite()
 * therefore degrades to a threshold: anything at or above half scale is on.
 */
void analogWrite(const pin_t pin, const int value) {
  digitalWrite(pin, value >= 128 ? HIGH : LOW);
}

// ------------------------
// External interrupts
// ------------------------

// One callback per pin, indexed the same way pins are numbered
static void (*pin_isr_callback[NUM_DIGITAL_PINS])() = { nullptr };

static void pio_isr_dispatch(const uint8_t port_index) {
  Pio * const port = port_index == 0 ? PIOA : port_index == 1 ? PIOB : PIOC;
  uint32_t status = port->PIO_ISR & port->PIO_IMR;
  while (status) {
    const uint8_t bit = 31 - __builtin_clz(status);
    status &= ~(1UL << bit);
    const uint8_t pin = port_index * 32 + bit;
    if (pin_isr_callback[pin]) pin_isr_callback[pin]();
  }
}

extern "C" void PIOA_Handler() { pio_isr_dispatch(0); }
extern "C" void PIOB_Handler() { pio_isr_dispatch(1); }
extern "C" void PIOC_Handler() { pio_isr_dispatch(2); }

void attachInterrupt(const pin_t pin, void (*callback)(), const uint8_t mode) {
  if (!VALID_PIN(pin) || !callback) return;

  Pio * const port = SAM3U_PIO(pin);
  const uint32_t mask = SAM3U_MASK(pin);
  const uint32_t pio_id = SAM3U_PIO_ID(pin);

  pin_isr_callback[pin] = callback;

  pmc_enable_periph_clk(pio_id);
  port->PIO_IDR = mask;                 // Quiesce while reconfiguring

  // "Additional interrupt modes" pick edge/level and polarity; without them
  // the PIO defaults to interrupting on both edges.
  switch (mode) {
    case CHANGE:
      port->PIO_AIMDR = mask;           // Back to the default both-edges mode
      break;
    case FALLING:
      port->PIO_AIMER = mask;
      port->PIO_ESR = mask;             // Edge, not level
      port->PIO_FELLSR = mask;          // Falling edge
      break;
    case RISING:
      port->PIO_AIMER = mask;
      port->PIO_ESR = mask;
      port->PIO_REHLSR = mask;          // Rising edge
      break;
    case LOW_EDGE:
      port->PIO_AIMER = mask;
      port->PIO_LSR = mask;             // Level, not edge
      port->PIO_FELLSR = mask;          // Low level
      break;
    case HIGH_EDGE:
      port->PIO_AIMER = mask;
      port->PIO_LSR = mask;
      port->PIO_REHLSR = mask;          // High level
      break;
    default:
      port->PIO_AIMDR = mask;
      break;
  }

  (void)port->PIO_ISR;                  // Clear any stale flag
  port->PIO_IER = mask;

  NVIC_EnableIRQ(IRQn_Type(pio_id));
}

void detachInterrupt(const pin_t pin) {
  if (!VALID_PIN(pin)) return;
  SAM3U_PIO(pin)->PIO_IDR = SAM3U_MASK(pin);
  pin_isr_callback[pin] = nullptr;
}

// ------------------------
// Misc
// ------------------------

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  const long divisor = in_max - in_min;
  if (!divisor) return out_min;
  return (x - in_min) * (out_max - out_min) / divisor + out_min;
}

char* dtostrf(double value, signed char width, unsigned char prec, char *out) {
  char fmt[24];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(out, fmt, value);
  return out;
}

#endif // __SAM3U4E__
