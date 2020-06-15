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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 // VFD controlling of spindle, Contributed by Stefan de Bruijn.

#include "VFDSpindle.h"

#include <HardwareSerial.h>

#if (VFD_RX_PIN == 19 && VFD_TX_PIN == 18)
#define VFDSerial Serial1
#elif (VFD_RX_PIN == 17 && VFD_TX_PIN == 16)
#define VFDSerial Serial2
#elif (VFD_RX_PIN == 15 && VFD_TX_PIN == 14)
#define VFDSerial Serial3
#else
#error "Incorrect serial port specified."
#endif

#define HUANYANG_MAX_MSG_SIZE   10 // Should be plenty

typedef struct {
  uint8_t tx_length;
  uint8_t rx_length;
  char msg[HUANYANG_MAX_MSG_SIZE];
} hy_command_t;

void VFDSpindle::init()
{
  pinMode(VFD_RTS_PIN, OUTPUT);
  digitalWrite(VFD_RTS_PIN, LOW);

  VFDSerial.begin(VFD_BAUD);
  power = 0;
}

/*
    ADDR    CMD     LEN     DATA        CRC
    0x01    0x03    0x01    0x01        0x31 0x88                   Start spindle clockwise
    0x01    0x03    0x01    0x08        0xF1 0x8E                   Stop spindle
    0x01    0x03    0x01    0x11        0x30 0x44                   Start spindle counter-clockwise
    0x01    0x05    0x02    0x09 0xC4   0xBF 0x0F                   Write Frequency (0x9C4 = 2500 = 25.00HZ)
*/

void VFDSpindle::send(const hy_command_t& cmd)
{
  // TODO: Receive first and parse the response -> update state!
  digitalWrite(VFD_RTS_PIN, LOW);
  while (VFDSerial.available)
  {
    while (VFDSerial.available) {
      VFDSerial.read();
    }
    delayMicroseconds(1000000 / VFD_BAUD); // see specs
  }

  // Write
  digitalWrite(VFD_RTS_PIN, HIGH);
  VFDSerial.write((const uint8_t*)(cmd.msg), cmd.tx_length);
}

void VFDSpindle::apply_power(const cutter_power_t inpow)
{
  if (inpow == 0) {
    mode = 0;
  }

  hy_command_t mode_cmd;

  mode_cmd.tx_length = 6;
  mode_cmd.rx_length = 6;

  mode_cmd.msg[0] = VFD_MODBUS_ADDR;
  mode_cmd.msg[1] = 0x03;
  mode_cmd.msg[2] = 0x01;

  if (mode == 1) // clockwise
  {
    mode_cmd.msg[3] = 0x01;
  }
  else if (mode == -1) // counterclockwise
  {
    mode_cmd.msg[3] = 0x11;
  }
  else    // off
  {
    mode_cmd.msg[3] = 0x08;
  }

  add_ModRTU_CRC(mode_cmd.msg, mode_cmd.rx_length);

  send(mode_cmd);
}

void VFDSpindle::add_ModRTU_CRC(char* buf, int full_msg_len) {
  uint16_t crc = 0xFFFF;
  for (int pos = 0; pos < full_msg_len - 2; pos++) {
    crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                      // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                          // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // add the calculated Crc to the message
  buf[full_msg_len - 1] = (crc & 0xFF00) >> 8;
  buf[full_msg_len - 2] = (crc & 0xFF);
}

bool VFDSpindle::active()
{
  // TODO
  return mode != 0;
}

void VFDSpindle::update_output()
{
  // TODO FIXME!
}

void VFDSpindle::power_delay()
{
  // TODO FIXME!
}

void VFDSpindle::set_direction(const bool reverse)
{
  mode = reverse ? -1 : 1;
}

