#include "../../../inc/MarlinConfig.h"

#if ENABLED(RS485_ENABLED)
#include "../../../feature/rs485.h"
#include "../../gcode.h"

void write_packet_data() {
  for(size_t i=0; i<packetizer.packetLength(); i++) {
    uint8_t data = bus[i];
    SERIAL_ECHO((data < 0x10) ? "0" : "");

    SERIAL_PRINT(data, PrintBase::Hex);
  }
  serial_print(F("\n"));
}

void GcodeSuite::M485() {
  if(strlen(parser.string_arg) % 2 != 0) {
    SERIAL_ERROR_MSG("String parameter must be an even number of bytes");
    return;
  }

  if(strlen(parser.string_arg) > RS485_SEND_BUFFER_SIZE * 2) {
    SERIAL_ERROR_MSG("String parameter can not have more than ", RS485_SEND_BUFFER_SIZE, " bytes");
    return;
  }

  // Read in the hex data string provided into our internal buffer
  for(size_t i=0; i<strlen(parser.string_arg); i += 2) {
    int firstNibble = HEXCHR(parser.string_arg[i]);
    int secondNibble = HEXCHR(parser.string_arg[i+1]);

    if(firstNibble == -1 || secondNibble == -1) {
      SERIAL_ERROR_START();
      serial_print(F("Not a hex character: "));
      SERIAL_CHAR(firstNibble == -1 ? parser.string_arg[i] : parser.string_arg[i+1]);
      serial_print(F("\n"));
      
      return;
    }

    uint8_t byte = (firstNibble & 0xF) << 4 | (secondNibble & 0xF);
    buffer[i >> 1] = byte;
  }

  packetizer.setMaxReadTimeout(10);  // This can be super small since ideally any packets will already be in our buffer

  // Read and ignore any packets that may have come in, before we write.
  while(packetizer.hasPacket()) {
    SERIAL_ECHO("rs485-unexpected-packet: ");
    write_packet_data();
    packetizer.clearPacket();
  }

  PacketWriteResult writeResult = packetizer.writePacket(buffer, strlen(parser.string_arg) / 2);

  switch(writeResult) {
    case PacketWriteResult::OK:
      break;  // Nothing to do
    case PacketWriteResult::FAILED_INTERRUPTED:
      SERIAL_ERROR_MSG("RS485: Write failed interrupted");
      return;
    case PacketWriteResult::FAILED_BUFFER_FULL:
      SERIAL_ERROR_MSG("RS485: Write failed buffer full");
      return;
    case PacketWriteResult::FAILED_TIMEOUT:
      SERIAL_ERROR_MSG("RS485: Write failed timeout");
      return;
  }

  packetizer.setMaxReadTimeout(50000);  // 50 ms

  // unsigned long startTime = millis();
  bool hasPacket = packetizer.hasPacket();
  // unsigned long endTime = millis();
  // SERIAL_ECHO("rs485-time: ");
  // SERIAL_PRINTLN(endTime - startTime, PrintBase::Dec);

  if(! hasPacket) {
    SERIAL_ECHOLN("rs485-reply: TIMEOUT");
    return;
  }

  SERIAL_ECHO("rs485-reply: ");
  write_packet_data();
  packetizer.clearPacket();
}

#endif