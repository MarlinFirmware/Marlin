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
 * feature/leds/printer_event_leds.cpp - LED color changing based on printer status
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(PRINTER_EVENT_LEDS)

#include "printer_event_leds.h"

PrinterEventLEDs printerEventLEDs;

#if HAS_LEDS_OFF_FLAG
  bool PrinterEventLEDs::leds_off_after_print; // = false
#endif

#if HAS_TEMP_HOTEND || HAS_HEATED_BED

  uint8_t PrinterEventLEDs::old_intensity = 0;

  inline uint8_t pel_intensity(const celsius_t start, const celsius_t current, const celsius_t target) {
    if (start == target) return 255;
    return (uint8_t)map(constrain(current, start, target), start, target, 0, 255);
  }

  inline void pel_set_rgb(const uint8_t r, const uint8_t g, const uint8_t b OPTARG(HAS_WHITE_LED, const uint8_t w=0)) {
    leds.set_color(
      LEDColor(r, g, b OPTARG(HAS_WHITE_LED, w) OPTARG(NEOPIXEL_LED, neo.brightness()))
      OPTARG(NEOPIXEL_IS_SEQUENTIAL, true)
    );
  }

#endif

#if HAS_TEMP_HOTEND

  void PrinterEventLEDs::onHotendHeating(const celsius_t start, const celsius_t current, const celsius_t target) {
    const uint8_t blue = pel_intensity(start, current, target);
    if (blue != old_intensity) {
      old_intensity = blue;
      pel_set_rgb(255, 0, 255 - blue);
    }
  }

#endif

#if HAS_HEATED_BED

  void PrinterEventLEDs::onBedHeating(const celsius_t start, const celsius_t current, const celsius_t target) {
    const uint8_t red = pel_intensity(start, current, target);
    if (red != old_intensity) {
      old_intensity = red;
      pel_set_rgb(red, 0, 255);
    }
  }

#endif

#if HAS_HEATED_CHAMBER

  void PrinterEventLEDs::onChamberHeating(const celsius_t start, const celsius_t current, const celsius_t target) {
    const uint8_t green = pel_intensity(start, current, target);
    if (green != old_intensity) {
      old_intensity = green;
      pel_set_rgb(255, green, 255);
    }
  }

#endif

#endif // PRINTER_EVENT_LEDS
