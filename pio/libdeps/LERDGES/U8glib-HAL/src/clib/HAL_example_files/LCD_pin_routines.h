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

/**
 * Low level pin manipulation routines - used by all the drivers.
 *
 * These are based on the LPC1768 pinMode, digitalRead & digitalWrite routines.
 *
 * Couldn't just call exact copies because the overhead killed the LCD update speed
 * With an intermediate level the softspi was running in the 10-20kHz range which
 * resulted in using about about 25% of the CPU's time.
 */

void u8g_SetPinOutput(uint8_t internal_pin_number);
void u8g_SetPinInput(uint8_t internal_pin_number);
void u8g_SetPinLevel(uint8_t  pin, uint8_t  pin_status);
uint8_t u8g_GetPinLevel(uint8_t pin);
