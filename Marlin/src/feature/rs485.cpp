#include "../inc/MarlinConfig.h"

#ifdef RS485_ENABLED

#include "rs485.h"

HardwareSerialBusIO rs485BusIO(&RS485_SERIAL);
RS485Bus<RS485_BUS_BUFFER_SIZE> rs485Bus(rs485BusIO, RS485_RX_ENABLE_PIN, RS485_TX_ENABLE_PIN);

PhotonProtocol rs485Protocol;

Packetizer rs485Packetizer(rs485Bus, rs485Protocol);

uint8_t rs485Buffer[RS485_SEND_BUFFER_SIZE];

void rs485_init() {
  RS485_SERIAL.begin(57600);
}

#endif