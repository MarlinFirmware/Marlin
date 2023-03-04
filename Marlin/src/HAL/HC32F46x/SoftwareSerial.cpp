/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#if defined(TARGET_HC32F46x) && !defined(HAVE_SW_SERIAL)

/*
 * Stub class for SoftwareSerial
 *
 * TODO: if needed, may implement SoftwareSerial using https://github.com/FYSETC/SoftwareSerialM
 */
#include "SoftwareSerial.h"

SoftwareSerial::SoftwareSerial(int8_t RX_pin, int8_t TX_pin) {}

void SoftwareSerial::begin(const uint32_t baudrate) {}

bool SoftwareSerial::available()
{
  return false;
}

uint8_t SoftwareSerial::read()
{
  return 0;
}

uint16_t SoftwareSerial::write(uint8_t byte)
{
  return 0;
}

void SoftwareSerial::flush() {}

void SoftwareSerial::listen()
{
  listening = true;
}

void SoftwareSerial::stopListening()
{
  listening = false;
}

#endif // TARGET_HC32F46x
