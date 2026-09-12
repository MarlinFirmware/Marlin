/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef ARDUINO_ARCH_AT32

#include "../../inc/MarlinConfig.h"
#include "MarlinSerial.h"

// AT32 core provides Serial1–Serial5 and Serial7 as HardwareSerial globals.
// Each adapter wraps the global by pointer; each MSerialN wraps the adapter.
static AT32SerialAdapter at32_adapter1(&Serial1);
static AT32SerialAdapter at32_adapter2(&Serial2);
static AT32SerialAdapter at32_adapter3(&Serial3);
static AT32SerialAdapter at32_adapter4(&Serial4);
static AT32SerialAdapter at32_adapter5(&Serial5);

MSerialT MSerial1(true, at32_adapter1);
MSerialT MSerial2(true, at32_adapter2);
MSerialT MSerial3(true, at32_adapter3);
MSerialT MSerial4(true, at32_adapter4);
MSerialT MSerial5(true, at32_adapter5);

#endif // ARDUINO_ARCH_AT32
