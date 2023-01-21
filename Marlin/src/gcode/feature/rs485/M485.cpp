#include "../../../inc/MarlinConfig.h"

#if ENABLED(RS485_ENABLED)
#include "../../../feature/rs485.h"
#include "../../gcode.h"

#define RS485_SEND_BUFFER_SIZE 32
//#define DEBUG_M485

bool m485_handle_packet_response() {
  if(! packetizer.hasPacket()) {
    return false;
  }

  SERIAL_ECHO("rs485-reply: ");
  for(size_t i=0; i<packetizer.packetLength(); i++) {
    uint8_t data = bus[i];
    SERIAL_ECHO((data < 0x10) ? "0" : "");

    SERIAL_PRINT(data, PrintBase::Hex);
  }
  SERIAL_ECHOLN("");
  packetizer.clearPacket();
  return true;
}

void GcodeSuite::M485() {
  bus.setReadBackDelay(10);
  bus.setReadBackRetries(3);
  if(strlen(parser.string_arg) % 2 != 0) {
    SERIAL_ERROR_MSG("String parameter must be an even number of bytes");
    return;
  }

  if(strlen(parser.string_arg) > RS485_SEND_BUFFER_SIZE * 2) {
    SERIAL_ERROR_MSG("String parameter can not have more than ", RS485_SEND_BUFFER_SIZE, " bytes");
    return;
  }

  uint8_t buffer[RS485_SEND_BUFFER_SIZE];

  for(size_t i=0; i<strlen(parser.string_arg); i += 2) {
    // TODO Throw an error if it's not within the hex range
    uint8_t byte = HEXCHR(parser.string_arg[i]) << 4 | HEXCHR(parser.string_arg[i+1]);
    buffer[i >> 1] = byte;

#ifdef DEBUG_M485
    SERIAL_ECHO((byte < 0x10) ? "0" : "");
    SERIAL_PRINT(byte, PrintBase::Hex);
#endif
  }
#ifdef DEBUG_M485
  SERIAL_ECHOLN("");
#endif

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

  packetizer.setMaxReadTimeout(1000);
  
  for(uint8_t attempt = 0; attempt < 5; attempt++) {
    delay(100);
#ifdef DEBUG_M485
    bus.fetch();
    SERIAL_ECHO("Attempt ");
    SERIAL_PRINT(attempt, PrintBase::Dec);
    SERIAL_ECHO(" (");
    SERIAL_PRINT(bus.available(), PrintBase::Dec);
    SERIAL_ECHO(") bytes: ");
    for(size_t i=0; i<bus.available(); i++) {
      uint8_t data = bus[i];
      SERIAL_ECHO((data < 0x10) ? "0" : "");

      SERIAL_PRINT(data, PrintBase::Hex);
    }
    SERIAL_ECHOLN("");

    for(size_t i=0; i<bus.available(); i++) {
      IsPacketResult isPacket = protocol.isPacket(bus, i, bus.available() - 1);
      SERIAL_ECHO(i);
      SERIAL_ECHO(": ");
      switch(isPacket.status) {
        case PacketStatus::YES:
          SERIAL_ECHOLN("Yes");
          m485_handle_packet_response();
          return;
        case PacketStatus::NO:
          SERIAL_ECHOLN("No");
          break;
        case PacketStatus::NOT_ENOUGH_BYTES:
          SERIAL_ECHOLN("Not Enough Bytes");
          break;
      }
    }
#else
  if(m485_handle_packet_response()) {
    return;
  }
#endif
  }
  SERIAL_ECHOLN("rs485-reply: TIMEOUT");
}

#endif