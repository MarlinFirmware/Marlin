#include "../inc/MarlinConfig.h"

#ifdef RS485_ENABLED

#include "rs485.h"

HardwareSerialBusIO busIO(&RS485_SERIAL);
RS485Bus<RS485_BUS_BUFFER_SIZE> bus(busIO, RS485_RX_ENABLE_PIN, RS485_TX_ENABLE_PIN);

PhotonProtocol protocol;

Packetizer packetizer(bus, protocol);

void rs485_init() {
  RS485_SERIAL.begin(9600);
}

#endif