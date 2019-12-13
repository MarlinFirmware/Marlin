/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
 * Copyright (c) 2019 Lino Barreca
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
#include <stm32f4xx_ll_spi.h>

#define SPI_TRANSFER_TIMEOUT 1000
#define BUS_SPI_HANDLE(BUS_NUM) (&(spi[BUS_NUM] -> handle))

// ------------------------
// Public Variables
// ------------------------
static spi_t* spi[NUM_SPI_BUSES]  = { NULL };

// ------------------------
// Public functions
// ------------------------

bool spiInitialized(uint8_t bus_num)
{
  return spi[bus_num] != NULL;
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
  if (spiInitialized(bus_num)) spi_deinit(spi[bus_num]); //spi was already initialized maybe at a different clock. de-init & re-init

  spi[bus_num] = new spi_t();
  spi[bus_num] -> pin_sclk = digitalPinToPinName(SPI_BusConfig[bus_num][SPIBUS_CLCK]);
  
  uint32_t clock = spi_getClkFreq(spi[bus_num]);

  switch (spiRate) {
    case SPI_FULL_SPEED:    clock /= 2;   break; //MAX Speed
    case SPI_HALF_SPEED:    clock /= 4;   break;
    case SPI_QUARTER_SPEED: clock /= 8;   break;
    case SPI_EIGHTH_SPEED:  clock /= 16;  break;
    case SPI_SPEED_5:       clock /= 128; break;
    case SPI_SPEED_6:       clock /= 256; break; //MIN Speed
    default:
      clock = SPI_SPEED_CLOCK_DEFAULT; // Default from the SPI library
  }

  spi[bus_num] -> pin_miso = digitalPinToPinName(SPI_BusConfig[bus_num][SPIBUS_MISO]);
  spi[bus_num] -> pin_mosi = digitalPinToPinName(SPI_BusConfig[bus_num][SPIBUS_MOSI]);
  spi[bus_num] -> pin_ssel = NC; //this is choosen "manually" at each read/write to/from device

  spi_init(spi[bus_num], clock, (spi_mode_e)SPI_BusConfig[bus_num][SPIBUS_MODE], SPI_BusConfig[bus_num][SPIBUS_BITO]);

  char mess[50];
  sprintf(mess, PSTR("SPI %d Clock: %lu Hz"), bus_num, clock);
  SERIAL_ECHOLN(mess);
}

//#ifdef DUMP_SPI
void spiDumpRegisters(SPI_TypeDef* Instance) {
  uint32_t crcEnabled = LL_SPI_IsEnabledCRC(Instance);

  SERIAL_ECHO("CRC ");
  if (!crcEnabled)
    SERIAL_ECHO("OFF");
  else {
    SERIAL_ECHO("ON (POL=0x");
    SERIAL_PRINT(Instance->CRCPR, HEX);
    SERIAL_ECHO(")");
  }
  SERIAL_ECHO(", Phase:");
  SERIAL_ECHO((Instance->CR1 & 0b01000000000000) ? "CRCNEXT":"Data");
  SERIAL_ECHO(LL_SPI_GetDataWidth(Instance) == LL_SPI_DATAWIDTH_8BIT ? " (8":" (16");
  SERIAL_ECHOLN("bit)");

  SERIAL_ECHO("STATUS=");
  if (LL_SPI_IsActiveFlag_BSY(Instance)) SERIAL_ECHO(" BSY");
  if (LL_SPI_IsActiveFlag_OVR(Instance)) SERIAL_ECHO(" OVR");
  if (LL_SPI_IsActiveFlag_MODF(Instance)) SERIAL_ECHO(" MODF");
  if (LL_SPI_IsActiveFlag_CRCERR(Instance)) SERIAL_ECHO(" CRCERR");
  if (LL_SPI_IsActiveFlag_TXE(Instance)) SERIAL_ECHO(" TXE");
  if (LL_SPI_IsActiveFlag_RXNE(Instance)) SERIAL_ECHO(" RXNE");

  SERIAL_ECHO("\n Last DATA=0x");
  SERIAL_PRINTLN(Instance->DR, HEX);

  if (crcEnabled)
  {
    SERIAL_ECHO("CRCs: RX=0x");
    SERIAL_PRINT(Instance->RXCRCR, HEX);
    SERIAL_ECHO(" TX=0x");
    SERIAL_PRINTLN(Instance->TXCRCR, HEX);
  }
}
//#endif

uint8_t spiRec(uint8_t bus_num) {
#ifdef DUMP_SPI
  SERIAL_ECHO("R");
  SERIAL_PRINT(bus_num, DEC);
  SERIAL_ECHO(":");
#endif

  uint8_t b = 0xff;

  if (spiInitialized(bus_num)) {
    HAL_SPI_Receive(BUS_SPI_HANDLE(bus_num), &b, 1, SPI_TRANSFER_TIMEOUT);

#ifdef DUMP_SPI
    SERIAL_PRINTLN(b, HEX);
#endif
  }

  return b;
}

void spiSend(uint8_t bus_num, uint8_t b) {
#ifdef DUMP_SPI
  SERIAL_ECHO("S");
  SERIAL_PRINT(bus_num, DEC);
  SERIAL_ECHO(":");
#endif

  if (!spiInitialized(bus_num)) return;

#ifdef DUMP_SPI
  SERIAL_PRINTLN(b, HEX);
#endif
  HAL_SPI_Transmit(BUS_SPI_HANDLE(bus_num), &b, sizeof(uint8_t), SPI_TRANSFER_TIMEOUT);
}

/**
 * @brief  Receives a number of bytes from the SPI port to a buffer
 * 
 * @param  dev_num Device number (identifies device and bus)
 * @param  buf     Pointer to starting address of buffer to write to.
 * @param  count   Number of bytes to receive.
 * 
 * @return Nothing
 *
 */
void spiRead(uint8_t bus_num, uint8_t* buf, uint16_t count) {
#ifdef DUMP_SPI
  SERIAL_ECHO("D");
  SERIAL_PRINT(bus_num, DEC);
  SERIAL_ECHO(":");
#endif

  if (count == 0 || !spiInitialized(bus_num)) return;
  memset(buf, 0xff, count);

  HAL_SPI_Receive(BUS_SPI_HANDLE(bus_num), buf, count, SPI_TRANSFER_TIMEOUT);

#ifdef DUMP_SPI
  for (uint16_t b=0; b<count && b<=DUMP_SPI; b++) {
    SERIAL_PRINT(buf[b], HEX);
    SERIAL_ECHO(b==DUMP_SPI ? "...":" ");
  }
  SERIAL_ECHOLN();
#endif
}
/**
 * @brief  Sends a number of bytes to the SPI port 
 * 
 * @param  bus_num Bus number
 * @param  buf     Pointer to starting address of buffer to send.
 * @param  count   Number of bytes to send.
 * 
 * @return Nothing
 *
 */
void spiWrite(uint8_t bus_num, const uint8_t* buf, uint16_t count) {
#ifdef DUMP_SPI
  SERIAL_ECHO("U");
  SERIAL_PRINT(bus_num, DEC);
  SERIAL_ECHO(":");
#endif

  if (count == 0 || !spiInitialized(bus_num)) return;

#ifdef DUMP_SPI
  for (uint16_t b=0; b<count && b<=DUMP_SPI; b++) {
    SERIAL_PRINT(buf[b], HEX);
    SERIAL_ECHO(b==DUMP_SPI ? "...":" ");
  }
  SERIAL_ECHOLN();
#endif

  HAL_SPI_Transmit(BUS_SPI_HANDLE(bus_num), (uint8_t*)buf, count, SPI_TRANSFER_TIMEOUT);
}

//Device functions

/**
 * @brief  Configures the parameters of the bus to the required ones for the device
 * @param  dev_num Device number (identifies device and bus)
 * @return LL handle to the bus for additional configuration and start
 */
SPI_TypeDef* spiSetBus(uint8_t dev_num) {
  SPI_TypeDef* hspi = BUS_SPI_HANDLE(BUS_OF_DEV(dev_num)) -> Instance;
  
  LL_SPI_Disable(hspi);
  LL_SPI_SetClockPolarity(hspi, CPOL_OF_DEV(dev_num) == SPI_PLO ? LL_SPI_POLARITY_LOW : LL_SPI_POLARITY_HIGH);
  LL_SPI_SetClockPhase(hspi, CPHA_OF_DEV(dev_num) == SPI_LTS ? LL_SPI_PHASE_1EDGE : LL_SPI_PHASE_2EDGE);
  LL_SPI_SetTransferBitOrder(hspi, BITO_OF_DEV(dev_num) == SPI_LSB ?  LL_SPI_LSB_FIRST : LL_SPI_MSB_FIRST);
  //TODO: configure speed per device

  return hspi;
}

uint16_t spiReadCRC16(uint8_t dev_num, uint16_t* buf, const uint16_t count) {
  if (count == 0 || !spiInitialized(BUS_OF_DEV(dev_num))) return 0;
  digitalWrite(CS_OF_DEV(dev_num), HIGH); //this is temporary until ALL SD card calls will be by device and not by bus. by then the CS will already be high when entering this

  SPI_TypeDef * hspi = spiSetBus(dev_num);
  LL_SPI_SetDataWidth(hspi, LL_SPI_DATAWIDTH_16BIT);
  LL_SPI_SetCRCPolynomial(hspi, 0x1021); //0x1021 is the normal polynomial for CRC16-CCITT
  LL_SPI_EnableCRC(hspi); //to clear CRC registers
  LL_SPI_Enable(hspi);
  digitalWrite(CS_OF_DEV(dev_num), LOW); //leave after LL_SPI_Enable

  bool send = true;
  uint16_t remR = count;

  while (remR > 0) {
    if (LL_SPI_IsActiveFlag_TXE(hspi) && send) {        //if transmit buffer is empty and we need to send
      LL_SPI_TransmitData16(hspi, 0xFFFF);              //send
      send = false;                                     //and wait
    }

    if (LL_SPI_IsActiveFlag_RXNE(hspi)) {               //if receive buffer is not empty
      buf[count - remR--] = __REV16(LL_SPI_ReceiveData16(hspi)); //receive
      send = true;                                      //and send next
    }
  }

  uint32_t calcCRC = LL_SPI_GetRxCRC(hspi);             //get running CRC

  digitalWrite(CS_OF_DEV(dev_num), HIGH);
  LL_SPI_Disable(hspi);
  LL_SPI_DisableCRC(hspi);
  LL_SPI_SetDataWidth(hspi, LL_SPI_DATAWIDTH_8BIT);
  LL_SPI_Enable(hspi);

  digitalWrite(CS_OF_DEV(dev_num), LOW); //this is temporary until all the SD card calls will be by device and not by bus. by then the CS will need to be left high

  return (uint16_t)calcCRC; //return HW-computed crc
}

void spiWriteCRC16(uint8_t dev_num, const uint16_t* buf, const uint16_t count) {
  if (count == 0 || !spiInitialized(BUS_OF_DEV(dev_num))) return;
  digitalWrite(CS_OF_DEV(dev_num), HIGH); //this is temporary until ALL SD card calls will be by device and not by bus. by then the CS will already be high when entering this

  SPI_TypeDef * hspi = spiSetBus(dev_num);
  LL_SPI_SetDataWidth(hspi, LL_SPI_DATAWIDTH_16BIT);
  LL_SPI_SetCRCPolynomial(hspi, 0x1021); //0x1021 is the normal polynomial for CRC16-CCITT
  LL_SPI_EnableCRC(hspi); //to clear CRC registers
  LL_SPI_Enable(hspi);
  digitalWrite(CS_OF_DEV(dev_num), LOW); //leave after LL_SPI_Enable

  uint16_t remT = count;

  while (remT > 0) 
    if (LL_SPI_IsActiveFlag_TXE(hspi))                  //if transmit buffer is empty
      LL_SPI_TransmitData16(hspi, __REV16(buf[count - remT--])); //send

  LL_SPI_SetCRCNext(hspi);

  digitalWrite(CS_OF_DEV(dev_num), HIGH);
  LL_SPI_Disable(hspi);
  LL_SPI_DisableCRC(hspi);
  LL_SPI_SetDataWidth(hspi, LL_SPI_DATAWIDTH_8BIT);
  LL_SPI_Enable(hspi);

  digitalWrite(CS_OF_DEV(dev_num), LOW); //this is temporary until all the SD card calls will be by device and not by bus. by then the CS will need to be left high
}

/**
 * @brief  Receives a single byte from the SPI device.
 * 
 * @param  dev_num Device number (identifies device and bus)
 * 
 * @return Byte received
 *
 */
uint8_t spiRecDevice(uint8_t dev_num) {
  LL_SPI_Enable(spiSetBus(dev_num));
  digitalWrite(CS_OF_DEV(dev_num), LOW);
  uint8_t b = spiRec(BUS_OF_DEV(dev_num));
  digitalWrite(CS_OF_DEV(dev_num), HIGH);
  return b;
}

/**
 * @brief  Receives a number of bytes from the SPI port to a buffer
 * 
 * @param  dev_num Device number (identifies device and bus)
 * @param  buf     Pointer to starting address of buffer to write to.
 * @param  count   Number of bytes to receive.
 * 
 * @return Nothing
 *
 */
void spiReadDevice(uint8_t dev_num, uint8_t* buf, uint16_t count) {
  LL_SPI_Enable(spiSetBus(dev_num));
  digitalWrite(CS_OF_DEV(dev_num), LOW);
  spiRead(BUS_OF_DEV(dev_num), buf, count);
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
void spiSendDevice(uint8_t dev_num, uint8_t b) {
  LL_SPI_Enable(spiSetBus(dev_num));
  digitalWrite(CS_OF_DEV(dev_num), LOW);
  spiSend(BUS_OF_DEV(dev_num), b);
  digitalWrite(CS_OF_DEV(dev_num), HIGH);
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
