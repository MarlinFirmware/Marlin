/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
/*
  Ledstrip.h
  Library header file for Ledstrip library
 */

#define LEDSTRIP_OK 0
#define LEDSTRIP_NOLEDSPIN 1
#define LEDSTRIP_NONLEDS 2
#define LEDSTRIP_BADSEGMENT 3
#define LEDSTRIP_NOACTION 4

#define LED_POWEROFF 0
#define LED_POWERON 1
#define LED_POWERHALF 2
#define LED_POWERNOCHG 3

void SendColorsOnLedstrip (byte red, byte grn, byte blu, byte segment, byte power);

#if ENABLED(PRINTER_EVENT_LEDS)
  void handle_led_print_event(int code);
#endif
