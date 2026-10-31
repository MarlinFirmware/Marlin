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
 * See PinsDebugBase in pins/pinsDebug.h for the hooks.
 */

#include <driver/gpio.h>
#include <soc/gpio_struct.h>
#include <esp32-hal-ledc.h>

#define PINS_DEBUG_CLASS

// Defined by the HAL. Returns the LEDC channel driving the pin, or -1. Unlike
// get_pwm_channel it never allocates, so it's safe to call just to look.
int8_t channel_for_pin(const uint8_t pin);

struct PinsDebug : PinsDebugBase<PinsDebug> {
  static constexpr uint16_t count = NUM_DIGITAL_PINS;
  static constexpr uint8_t name_pad = 16;   // space needed to be pretty if not first name assigned to a pin

  /**
   * GPIO numbering is not contiguous. On the ESP32, GPIO 20, 24, and 28-31 don't
   * exist, and 34-39 are input-only. SOC_GPIO_VALID_GPIO_MASK knows the gaps for
   * whichever ESP32 variant is being built.
   */
  static bool is_valid(const pin_t pin) { return WITHIN(pin, 0, pin_t(count - 1)) && GPIO_IS_VALID_GPIO(pin); }

  /**
   * M43 must never drive the SPI flash pins (GPIO 6-11), which crashes the chip,
   * or the UART0 pins (GPIO 1, 3) of the host serial port, which then stops working.
   */
  static bool never_touch(const uint16_t index) { return WITHIN(index, 6, 11) || index == 1 || index == 3; }

  /**
   * ADC channels are not a contiguous range of GPIOs, so the channel and the pin
   * are different numbers. ESP32 analogRead() takes a GPIO, and the shared code
   * passes it whatever this returns, so return the pin. print_analog reports
   * the actual channel.
   */
  static int16_t analog_index(const pin_t pin) {
    return (is_valid(pin) && digitalPinToAnalogChannel(pin) >= 0) ? int8_t(pin) : -1;
  }
  static bool is_analog(const pin_t pin) { return is_valid(pin) && digitalPinToAnalogChannel(pin) >= 0; }

  static void print_analog(const pin_t pin) {
    char buffer[12];
    sprintf_P(buffer, PSTR(" (A%2d)  "), digitalPinToAnalogChannel(pin));
    SERIAL_ECHO(buffer);
  }

  /**
   * True if the pin is currently an output. fastio's IS_OUTPUT is hardcoded to
   * 'true' for this HAL, so read the GPIO output-enable registers instead.
   */
  static bool is_output(const pin_t pin) {
    if (!is_valid(pin) || !GPIO_IS_VALID_OUTPUT_GPIO(pin)) return false; // 34-39 are input-only
    return pin < 32 ? TEST32(GPIO.enable, pin) : TEST32(GPIO.enable1.val, pin - 32);
  }

  static bool pwm_active(const pin_t pin) {
    // chan_pin[] is zero-initialized, so channel_for_pin() would match GPIO 0
    // against an unallocated channel. get_pwm_channel skips pin 0 for the same
    // reason, so no PWM is ever assigned to it.
    if (!WITHIN(pin, 1, pin_t(MAX_PWM_IOPIN))) return false;
    return channel_for_pin(uint8_t(pin)) >= 0;
  }

  static void print_pwm(const pin_t pin) {
    if (!pwm_active(pin)) return;
    const uint8_t chan = uint8_t(channel_for_pin(uint8_t(pin)));
    SERIAL_ECHO("  LEDC chan: ", chan, "  duty: ", uint32_t(ledcRead(chan)), "  freq: ", uint32_t(ledcReadFreq(chan)));
  }

  static void print_number(const pin_t pin) {
    char buffer[10];
    sprintf_P(buffer, PSTR("%02d"), pin);
    SERIAL_ECHO(buffer);
  }

  // ESP32 GPIOs are not grouped into named ports, so there is nothing to report (print_port).
};
