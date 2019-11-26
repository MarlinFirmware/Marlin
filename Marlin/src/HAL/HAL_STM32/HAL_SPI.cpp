/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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
#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../inc/MarlinConfig.h"

#include <spi_com.h> //use this as helper for SPI peripheral Init configuration

#define SPI_TRANSFER_TIMEOUT 1000
#define BUS_SPI_HANDLE(BUS_NUM) (&(spi[BUS_NUM] -> handle))

// ------------------------
// Public Variables
// ------------------------
static spi_t* spi[NUM_SPI_BUSES]  = { NULL };

// ------------------------
// Public functions
// ------------------------

#define spiBegin ; //not used on STM32

bool spiInitialized(uint8_t bus_num)
{
  return spi[bus_num] != NULL;
}

void spiDebug(const uint8_t bus_num) {
  char mess[150];

  HAL_SPI_StateTypeDef state = HAL_SPI_GetState(BUS_SPI_HANDLE(bus_num));
  uint32_t error = HAL_SPI_GetError(BUS_SPI_HANDLE(bus_num));

  sprintf(mess, PSTR("SPI %d State: %d, Error: %lu"), bus_num, state, error);
  SERIAL_ECHOLN(mess);
}

/**
 * Initialize and configure SPI BUS for specified SPI speed
 * 
 * @param bus_num Number of the spi bus
 * @param spiRate Maximum speed of the bus in Mhz
 * 
 * @return Nothing
 */
void spiInit(uint8_t bus_num, uint8_t spiRate) {
  if (spiInitialized(bus_num)) spi_deinit(spi[bus_num]); //spi was already initialized maybe at a different frequency. de-init & re-init

  uint32_t clock;

  switch (spiRate) {
    case SPI_FULL_SPEED:    clock = 20000000; break; // 13.9mhz=20000000  6.75mhz=10000000  3.38mhz=5000000  .833mhz=1000000
    case SPI_HALF_SPEED:    clock =  5000000; break;
    case SPI_QUARTER_SPEED: clock =  2500000; break;
    case SPI_EIGHTH_SPEED:  clock =  1250000; break;
    case SPI_SPEED_5:       clock =   625000; break;
    case SPI_SPEED_6:       clock =   300000; break;
    default:
      clock = SPI_SPEED_CLOCK_DEFAULT; // Default from the SPI library
  }

  spi[bus_num] = new spi_t();
  spi[bus_num] -> pin_miso = digitalPinToPinName(SPI_BusConfig[bus_num][SPIBUS_MISO]);
  spi[bus_num] -> pin_mosi = digitalPinToPinName(SPI_BusConfig[bus_num][SPIBUS_MOSI]);
  spi[bus_num] -> pin_sclk = digitalPinToPinName(SPI_BusConfig[bus_num][SPIBUS_CLCK]);
  spi[bus_num] -> pin_ssel = NC; //this is choosen "manually" at each read/write to/from device

  SERIAL_ECHO("Before init: frequency=");

  if (bus_num == 0) //SPI1
    SERIAL_PRINT(HAL_RCC_GetPCLK2Freq(), DEC);
  else //SPI2 & 3
    SERIAL_PRINT(HAL_RCC_GetPCLK1Freq(), DEC);

  spiDebug(bus_num);
  spi_init(spi[bus_num], clock, (spi_mode_e)SPI_BusConfig[bus_num][SPIBUS_MODE], 0);
  SERIAL_ECHO_MSG("After init");
  spiDebug(bus_num);

  if (clock < 700000) {
    BUS_SPI_HANDLE(bus_num)->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    HAL_SPI_Init(BUS_SPI_HANDLE(bus_num));
    SERIAL_ECHO_MSG("After force");
    spiDebug(bus_num);
  }
}

/**
 * @brief  Receives a single byte from the SPI device.
 * 
 * @param  dev_num Device number (identifies device and bus)
 * 
 * @return Byte received
 *
 */
uint8_t spiRec(uint8_t dev_num) {
  SERIAL_ECHO_MSG("Receive");
  uint8_t b = 0xff;
  if (!spiInitialized(BUS_OF_DEV(dev_num))) return b;

  digitalWrite(CS_OF_DEV(dev_num), LOW);
  HAL_SPI_Receive(BUS_SPI_HANDLE(BUS_OF_DEV(dev_num)), &b, 1, SPI_TRANSFER_TIMEOUT);
  spiDebug(BUS_OF_DEV(dev_num));
  digitalWrite(CS_OF_DEV(dev_num), HIGH);

  return b;
}

/**
 * @brief  Receives a number of bytes from the SPI port to a buffer
 * 
 * @param  dev_num Device number (identifies device and bus)
 * @param  buf     Pointer to starting address of buffer to write to.
 * @param  nbyte   Number of bytes to receive.
 * 
 * @return Nothing
 *
 */
void spiRead(uint8_t dev_num, uint8_t* buf, uint16_t nbyte) {
  SERIAL_ECHO_MSG("Read");
  if (!spiInitialized(BUS_OF_DEV(dev_num))) return;
  if (nbyte == 0) return;
  memset(buf, 0xff, nbyte);

  digitalWrite(CS_OF_DEV(dev_num), LOW);
  HAL_SPI_Receive(BUS_SPI_HANDLE(BUS_OF_DEV(dev_num)), buf, nbyte, SPI_TRANSFER_TIMEOUT);
  spiDebug(BUS_OF_DEV(dev_num));
  digitalWrite(CS_OF_DEV(dev_num), HIGH);
}

/**
 * @brief  Sends a single byte to a SPI device
 * 
 * @param  dev_num Device number (identifies device and bus)
 * @param  b Byte to send
 *
 * @details
 */
void spiSend(uint8_t dev_num, uint8_t b) {
  SERIAL_ECHO_MSG("Send");
  if (!spiInitialized(BUS_OF_DEV(dev_num))) return;

  digitalWrite(CS_OF_DEV(dev_num), LOW);
  spiWriteBus(BUS_OF_DEV(dev_num), b);
  digitalWrite(CS_OF_DEV(dev_num), HIGH);
}

void spiWriteBus(uint8_t bus_num, uint8_t b) {
  SERIAL_ECHO_MSG("Write");
  if (!spiInitialized(bus_num)) return;

  HAL_SPI_Transmit(BUS_SPI_HANDLE(bus_num), &b, sizeof(uint8_t), SPI_TRANSFER_TIMEOUT);
  spiDebug(bus_num);
}

/**
 * @brief  Write token and then write from 512 byte buffer to SPI (for SD card)
 *
 * @param  dev_num Device number (identifies device and bus)
 * @param  buf     Pointer with buffer start address
 * @return Nothing
 */
void spiSendBlock(uint8_t dev_num, uint8_t token, const uint8_t* buf) {
  SERIAL_ECHO_MSG("Block");
  if (!spiInitialized(BUS_OF_DEV(dev_num))) return;
  
  digitalWrite(CS_OF_DEV(dev_num), LOW);
  HAL_SPI_Transmit(BUS_SPI_HANDLE(BUS_OF_DEV(dev_num)), &token, sizeof(uint8_t), SPI_TRANSFER_TIMEOUT);
  spiDebug(BUS_OF_DEV(dev_num));
  HAL_SPI_Transmit(BUS_SPI_HANDLE(BUS_OF_DEV(dev_num)), (uint8_t*)buf, 512, SPI_TRANSFER_TIMEOUT);
  spiDebug(BUS_OF_DEV(dev_num));
  digitalWrite(CS_OF_DEV(dev_num), HIGH);
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
