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

void spiSetCRC(uint8_t bus_num, uint32_t CRCPol, bool word) {
  #ifdef DUMP_SPI
    SERIAL_ECHO("SPI ");
    SERIAL_PRINT(bus_num, DEC);
    SERIAL_ECHO(" before: ");
    spiDumpRegisters(BUS_SPI_HANDLE(bus_num) -> Instance);
  #endif

  if (!spiInitialized(bus_num)) return;

  (BUS_SPI_HANDLE(bus_num) -> Init).DataSize = word ? SPI_DATASIZE_16BIT : SPI_DATASIZE_8BIT;

  if (CRCPol) {
    (BUS_SPI_HANDLE(bus_num) -> Init).CRCCalculation = SPI_CRCCALCULATION_ENABLE;
    (BUS_SPI_HANDLE(bus_num) -> Init).CRCPolynomial = CRCPol;
  } else {
    (BUS_SPI_HANDLE(bus_num) -> Init).CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  }

  HAL_SPI_Init(BUS_SPI_HANDLE(bus_num));

  #ifdef DUMP_SPI
    SERIAL_ECHO("SPI ");
    SERIAL_PRINT(bus_num, DEC);
    SERIAL_ECHO(" after: ");
    spiDumpRegisters(BUS_SPI_HANDLE(bus_num) -> Instance);
  #endif
}

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

//CRC functions
bool spiCRCError(uint8_t bus_num) {
  if (!spiInitialized(bus_num)) return false;

#ifdef DUMP_SPI
  SERIAL_ECHO("SPI");
  SERIAL_PRINT(bus_num, DEC);
  SERIAL_ECHO(" RX:");
  SERIAL_PRINT(BUS_SPI_HANDLE(bus_num)->RxXferCount, DEC);
  SERIAL_ECHO(" (");
  SERIAL_PRINT(BUS_SPI_HANDLE(bus_num)->RxXferSize, DEC);
  SERIAL_ECHO("b) State=");
  SERIAL_PRINT(BUS_SPI_HANDLE(bus_num)->State, DEC);
  SERIAL_ECHO(" Error=");
  SERIAL_PRINTLN(BUS_SPI_HANDLE(bus_num)->ErrorCode, DEC);
#endif

  return (BUS_SPI_HANDLE(bus_num)->ErrorCode & HAL_SPI_ERROR_CRC) == HAL_SPI_ERROR_CRC;
}

//trace functions
void spiWrite8(uint8_t bus_num, const uint8_t* buf, uint16_t count) {
  if (count == 0 || !spiInitialized(bus_num)) return;

  SPI_TypeDef * hspi = BUS_SPI_HANDLE(bus_num) -> Instance;

  spiDumpRegisters(hspi);
  SERIAL_ECHO("Sending ");
  SERIAL_PRINT(count, DEC);
  SERIAL_ECHOLN(" bytes");

  SERIAL_ECHOLN("Clearing old CRC");
  LL_SPI_Disable(hspi);
  LL_SPI_DisableCRC(hspi);
  LL_SPI_EnableCRC(hspi);
  LL_SPI_Enable(hspi);
  spiDumpRegisters(hspi);

  SERIAL_ECHOLN("Loop:");
  while (count > 0)
    if (LL_SPI_IsActiveFlag_TXE(hspi))
    {
      spiDumpRegisters(hspi);
      SERIAL_ECHO("Sending ");
      SERIAL_PRINTLN(buf[0], HEX);
      LL_SPI_TransmitData8(hspi, buf[0]);
      buf += sizeof(uint8_t);
      count--;
    }
  
  SERIAL_ECHOLN("Send complete.");
  spiDumpRegisters(hspi);
  SERIAL_ECHOLN("Sending CRC...");
  LL_SPI_SetCRCNext(hspi);
  LL_SPI_ClearFlag_OVR(hspi);
  SERIAL_ECHOLN("CRC sent.");
  spiDumpRegisters(hspi);
}
void spiRead8(uint8_t bus_num, uint8_t* buf, const uint16_t count) {
  if (count == 0 || !spiInitialized(bus_num)) return;

  SPI_TypeDef * hspi = BUS_SPI_HANDLE(bus_num) -> Instance;

  bool send = true;
  uint16_t wcnt = count, remT = wcnt, remR = wcnt;

  SERIAL_ECHOLN("Clearing old CRC");
  LL_SPI_Disable(hspi);
  LL_SPI_DisableCRC(hspi);
  LL_SPI_EnableCRC(hspi);
  LL_SPI_Enable(hspi);
  spiDumpRegisters(hspi);

  while (remR > 0) {
    if (LL_SPI_IsActiveFlag_TXE(hspi) && send && remT > 0) { //if transmit buffer is empty and we need to send
      LL_SPI_TransmitData8(hspi, buf[wcnt - remT--]);

      send = false; //and wait
    }

    if (LL_SPI_IsActiveFlag_RXNE(hspi)) { //if receive buffer is not empty
      SERIAL_ECHO("Receiving idx");
      SERIAL_PRINT(wcnt - remR, DEC);
      buf[wcnt - remR] = LL_SPI_ReceiveData8(hspi);
      SERIAL_ECHO(", value=");
      SERIAL_PRINTLN(buf[wcnt - remR], HEX);
      spiDumpRegisters(hspi);

      remR--;
      send = true; //and send next
    }
  }

  SERIAL_ECHOLN("Receive complete.");
  spiDumpRegisters(hspi);

  if (LL_SPI_IsActiveFlag_OVR(hspi)) {
    SERIAL_ECHOLN("Clearing OVR");
    LL_SPI_ClearFlag_OVR(hspi);
    spiDumpRegisters(hspi);
  }
}

uint16_t spiRead16(uint8_t bus_num, uint16_t* buf, const uint16_t count) {
  if (count == 0 || !spiInitialized(bus_num)) return 0;

  SPI_TypeDef * hspi = BUS_SPI_HANDLE(bus_num) -> Instance;

  bool send = true;
  uint16_t remT = count, remR = count;

  /*SERIAL_ECHOLN("Setting up CRC");
  spiDumpRegisters(hspi);
  if (bus_num == 0) digitalWrite(CS_OF_DEV(0), HIGH);
  LL_SPI_Disable(hspi);
  LL_SPI_DisableCRC(hspi);
  LL_SPI_SetDataWidth(hspi, LL_SPI_DATAWIDTH_16BIT);
  LL_SPI_SetCRCPolynomial(hspi, 0x1021);
  LL_SPI_EnableCRC(hspi);
  LL_SPI_Enable(hspi);
  if (bus_num == 0) digitalWrite(CS_OF_DEV(0), LOW);*/
  spiDumpRegisters(hspi); SERIAL_FLUSH();

  while (remR > 0) {
    if (LL_SPI_IsActiveFlag_TXE(hspi) && send && remT > 0) { //if transmit buffer is empty and we need to send
      LL_SPI_TransmitData16(hspi, buf[count - remT]);

      /*if (remT == 0) { //transmitted everything
        SERIAL_ECHOLN("Sending CRC...");
        LL_SPI_SetCRCNext(hspi);
        spiDumpRegisters(hspi);
      }*/

      remT--;
      send = false; //and wait
    }

    if (LL_SPI_IsActiveFlag_RXNE(hspi)) { //if receive buffer is not empty
      SERIAL_ECHO("Receiving w");
      SERIAL_PRINT(count - remR, DEC); SERIAL_FLUSH();
      buf[count - remR] = LL_SPI_ReceiveData16(hspi);
      SERIAL_ECHO("=");
      SERIAL_PRINTLN(buf[count - remR], HEX); SERIAL_FLUSH();

      remR--;
      send = true; //and send next
    }
  }

  SERIAL_ECHOLN("Receive complete."); SERIAL_FLUSH();
  return (uint16_t) LL_SPI_GetRxCRC(hspi); //return HW-computed crc
}

//Device functions
/**
 * @brief  Receives a single byte from the SPI device.
 * 
 * @param  dev_num Device number (identifies device and bus)
 * 
 * @return Byte received
 *
 */
uint8_t spiRecDevice(uint8_t dev_num) {
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
  digitalWrite(CS_OF_DEV(dev_num), LOW);
  spiSend(BUS_OF_DEV(dev_num), b);
  digitalWrite(CS_OF_DEV(dev_num), HIGH);
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
