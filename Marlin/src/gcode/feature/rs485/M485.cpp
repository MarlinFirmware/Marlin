/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if HAS_RS485_SERIAL

#include "../../../feature/rs485.h"
#include "../../gcode.h"

void write_packet_data() {
  for (size_t i = 0; i < rs485Packetizer.packetLength(); i++) {
    const uint8_t data = rs485Bus[i];
    if (data < 0x10) SERIAL_ECHOPGM_P(PSTR("0"));
    SERIAL_PRINT(data, PrintBase::Hex);
  }
  SERIAL_EOL();
}

static void rs485_write_failed(const PacketWriteResult writeResult) {
  SERIAL_ERROR_START();
  SERIAL_ECHOPGM("RS485: Write failed ");
  switch (writeResult) {
    case FAILED_INTERRUPTED: SERIAL_ECHOPGM("interrupted"); break;
    case FAILED_BUFFER_FULL: SERIAL_ECHOPGM("buffer full"); break;
    case FAILED_TIMEOUT: SERIAL_ECHOPGM("timeout"); break;
  }
  SERIAL_EOL();
}

void GcodeSuite::M485() {
  if (strlen(parser.string_arg) & 1) {
    SERIAL_ERROR_MSG("String must contain an even number of bytes.");
    return;
  }

  if (strlen(parser.string_arg) > RS485_SEND_BUFFER_SIZE * 2) {
    SERIAL_ERROR_MSG("String too long (" STRINGIFY(RS485_SEND_BUFFER_SIZE) " bytes max).");
    return;
  }

  // Convert the string to bytes in the buffer
  for (size_t i = 0; i < strlen(parser.string_arg); i += 2) {
    const uint8_t nybble1 = HEXCHR(parser.string_arg[i]),
                  nybble2 = HEXCHR(parser.string_arg[i + 1]);

    if (nybble1 == -1 || nybble2 == -1) {
      SERIAL_ERROR_START();
      SERIAL_ECHOPGM("Not a hex character: ");
      SERIAL_CHAR(nybble1 == -1 ? parser.string_arg[i] : parser.string_arg[i+1]);
      SERIAL_EOL();
      return;
    }

    rs485Buffer[i >> 1] = (nybble1 & 0x0F) << 4 | (nybble2 & 0x0F);
  }

  rs485Packetizer.setMaxReadTimeout(10);  // This can be super small since ideally any packets will already be in our buffer

  // Read and ignore any packets that may have come in, before we write.
  while (rs485Packetizer.hasPacket()) {
    SERIAL_ECHOPGM("rs485-unexpected-packet: ");
    write_packet_data();
    rs485Packetizer.clearPacket();
  }

  const PacketWriteResult writeResult = rs485Packetizer.writePacket(rs485Buffer, strlen(parser.string_arg) / 2);
  switch (writeResult) {
    default: rs485_write_failed(writeResult);
    case PacketWriteResult::OK:  // Nothing to do
  }

  rs485Packetizer.setMaxReadTimeout(50000); // 50 ms

  //millis_t startTime = millis();
  bool hasPacket = rs485Packetizer.hasPacket();
  //millis_t endTime = millis();
  //SERIAL_ECHOLNPGM("rs485-time: ", endTime - startTime);

  if (!hasPacket) { SERIAL_ECHOLNPGM("rs485-reply: TIMEOUT"); return; }

  SERIAL_ECHOPGM("rs485-reply: ");
  write_packet_data();
  rs485Packetizer.clearPacket();
}

#endif // HAS_RS485_SERIAL
