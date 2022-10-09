#include "../../../inc/MarlinConfig.h"

#if ENABLED(RS485_ENABLED)
#include "../../../feature/rs485.h"
#include "../../gcode.h"

#define RS485_SEND_BUFFER_SIZE 4

void GcodeSuite::M485() {
  bus.setReadBackDelay(10);
  bus.setReadBackRetries(3);
  if(strlen(parser.string_arg) % 2 != 0) {
    SERIAL_ERROR_MSG("String parameter must be an even number of bytes");
    return;
  }

  // TODO Replace 32 with RS485BUS_BUFFER_SIZE
  if(strlen(parser.string_arg) > RS485_SEND_BUFFER_SIZE * 2) {
    SERIAL_ERROR_MSG("String parameter can not have more than ", RS485_SEND_BUFFER_SIZE, " bytes");
    return;
  }

  uint8_t buffer[RS485_SEND_BUFFER_SIZE];

  for(size_t i=0; i<strlen(parser.string_arg); i += 2) {
    // TODO Throw an error if it's not within the hex range
    uint8_t byte = HEXCHR(parser.string_arg[i]) << 4 | HEXCHR(parser.string_arg[i+1]);
    buffer[i] = byte;
  }

  PacketWriteResult writeResult = packetizer.writePacket(buffer, strlen(parser.string_arg) / 2);

  switch(writeResult) {
    case PacketWriteResult::OK:
      break;  // Nothing to do
    case PacketWriteResult::FAILED_INTERRUPTED:
      SERIAL_ERROR_MSG("RS485: Write failed interrupted");
      break;
    case PacketWriteResult::FAILED_BUFFER_FULL:
      SERIAL_ERROR_MSG("RS485: Write failed buffer full");
      break;
    case PacketWriteResult::FAILED_TIMEOUT:
      SERIAL_ERROR_MSG("RS485: Write failed timeout");
      break;
  }

  packetizer.setMaxReadTimeout(100);
  if(packetizer.hasPacket()) {
    SERIAL_ECHO("rs485-reply: ");
    for(size_t i=0; i<packetizer.packetLength(); i++) {
      uint8_t data = buffer[i];
      SERIAL_ECHO((data < 0x10) ? "0" : "");

      SERIAL_PRINT(data, PrintBase::Hex);
    }
    SERIAL_ECHOLN("");
  } else {
    SERIAL_ECHOLN("rs485-reply: TIMEOUT");
  }
}

#endif