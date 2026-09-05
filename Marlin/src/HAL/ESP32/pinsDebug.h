/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Pins Debugging for ESP32
 *
 *   - NUMBER_PINS_TOTAL
 *   - MULTI_NAME_PAD
 *   - getPinByIndex(index)
 *   - printPinNameByIndex(index)
 *   - getPinIsDigitalByIndex(index)
 *   - digitalPinToAnalogIndex(pin)
 *   - getValidPinMode(pin)
 *   - isValidPin(pin)
 *   - isAnalogPin(pin)
 *   - digitalRead_mod(pin)
 *   - pwm_status(pin)
 *   - printPinPWM(pin)
 *   - printPinPort(pin)
 *   - printPinNumber(pin)
 *   - printPinAnalog(pin)
 */

#include <driver/gpio.h>
#include <soc/gpio_struct.h>
#include <esp32-hal-ledc.h>

#define NUMBER_PINS_TOTAL NUM_DIGITAL_PINS
#define MULTI_NAME_PAD 16 // space needed to be pretty if not first name assigned to a pin

#define digitalRead_mod(P) extDigitalRead(P)
#define printPinNameByIndex(x) do{ sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); }while(0)
#define printPinNumber(P) do{ sprintf_P(buffer, PSTR("%02d"), P); SERIAL_ECHO(buffer); }while(0)
#define getPinByIndex(x) pin_array[x].pin
#define getPinIsDigitalByIndex(x) pin_array[x].is_digital

/**
 * GPIO numbering is not contiguous. On the ESP32, GPIO 20, 24, and 28-31 don't
 * exist, and 34-39 are input-only. SOC_GPIO_VALID_GPIO_MASK knows the gaps for
 * whichever ESP32 variant is being built.
 */
#define isValidPin(P) (WITHIN(P, 0, pin_t(NUMBER_PINS_TOTAL - 1)) && GPIO_IS_VALID_GPIO(P))

/**
 * ADC channels are not a contiguous range of GPIOs, so the channel and the pin
 * are different numbers. ESP32 analogRead() takes a GPIO, and the shared code
 * passes it whatever this returns, so return the pin. printPinAnalog reports
 * the actual channel.
 */
int8_t digitalPinToAnalogIndex(const pin_t pin) {
  return (isValidPin(pin) && digitalPinToAnalogChannel(pin) >= 0) ? int8_t(pin) : -1;
}

bool isAnalogPin(const pin_t pin) {
  return isValidPin(pin) && digitalPinToAnalogChannel(pin) >= 0;
}

#define printPinAnalog(P) do{ sprintf_P(buffer, PSTR(" (A%2d)  "), digitalPinToAnalogChannel(P)); SERIAL_ECHO(buffer); }while(0)

/**
 * True if the pin is currently an output. fastio's IS_OUTPUT is hardcoded to
 * 'true' for this HAL, so read the GPIO output-enable registers instead.
 */
static bool getValidPinMode(const pin_t pin) {
  if (!isValidPin(pin) || !GPIO_IS_VALID_OUTPUT_GPIO(pin)) return false; // 34-39 are input-only
  return pin < 32 ? TEST32(GPIO.enable, pin) : TEST32(GPIO.enable1.val, pin - 32);
}

// Defined by the HAL. Returns the LEDC channel driving the pin, or -1. Unlike
// get_pwm_channel it never allocates, so it's safe to call just to look.
int8_t channel_for_pin(const uint8_t pin);

bool pwm_status(const pin_t pin) {
  // chan_pin[] is zero-initialized, so channel_for_pin() would match GPIO 0
  // against an unallocated channel. get_pwm_channel skips pin 0 for the same
  // reason, so no PWM is ever assigned to it.
  if (!WITHIN(pin, 1, pin_t(MAX_PWM_IOPIN))) return false;
  return channel_for_pin(uint8_t(pin)) >= 0;
}

void printPinPWM(const pin_t pin) {
  if (!pwm_status(pin)) return;
  const uint8_t chan = uint8_t(channel_for_pin(uint8_t(pin)));
  SERIAL_ECHOPGM("  LEDC chan: ", chan, "  duty: ", uint32_t(ledcRead(chan)), "  freq: ", uint32_t(ledcReadFreq(chan)));
}

// ESP32 GPIOs are not grouped into named ports, so there is nothing to report.
void printPinPort(const pin_t) {}
