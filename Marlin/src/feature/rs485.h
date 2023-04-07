#pragma once

#include "../inc/MarlinConfig.h"

#if ENABLED(RS485_ENABLED)

#include "../MarlinCore.h"

#include <rs485/rs485bus.hpp>
#include <rs485/bus_adapters/hardware_serial.h>

#include <rs485/protocols/photon.h>
#include <rs485/packetizer.h>

#define RS485_SEND_BUFFER_SIZE 32

extern HardwareSerialBusIO busIO;
extern RS485Bus<RS485_BUS_BUFFER_SIZE> bus;
extern PhotonProtocol protocol;
extern Packetizer packetizer;
extern uint8_t buffer[RS485_SEND_BUFFER_SIZE];

void rs485_init();

#endif