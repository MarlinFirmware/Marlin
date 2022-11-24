/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../platforms.h"

#include "../../../inc/MarlinConfig.h"

#if HAS_SPI_TFT

#include "tft_spi.h"

// SPI can have different baudrates depending on read or write functionality.
#ifndef TFT_BAUDRATE_READ
  // Default.
  #define TFT_BAUDRATE_READ 0
#endif

#ifndef TFT_BAUDRATE_WRITE
  #define TFT_BAUDRATE_WRITE TFT_BAUDRATE_READ
#endif

void TFT_SPI::Init() {

  OUT_WRITE(TFT_A0_PIN, HIGH);
  OUT_WRITE(TFT_CS_PIN, HIGH);
}

void TFT_SPI::DataTransferBegin(eSPIMode spiMode) {
  int pin_miso = -1;
#if PIN_EXISTS(TFT_MISO)
  pin_miso = TFT_MISO_PIN;
#endif
  spiInitEx((spiMode == eSPIMode::READ) ? TFT_BAUDRATE_READ : TFT_BAUDRATE_WRITE, TFT_SCK_PIN, pin_miso, TFT_MOSI_PIN, TFT_CS_PIN);
}

void TFT_SPI::DataTransferEnd(void) {
  spiClose();
}

#ifdef TFT_DEFAULT_DRIVER
  #include "../../../lcd/tft_io/tft_ids.h"
#endif

uint32_t TFT_SPI::GetID() {
  uint32_t id;
  id = ReadID(LCD_READ_ID);
  if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF) {
    id = ReadID(LCD_READ_ID4);
    #ifdef TFT_DEFAULT_DRIVER
      if ((id & 0xFFFF) == 0 || (id & 0xFFFF) == 0xFFFF)
        id = TFT_DEFAULT_DRIVER;
    #endif
   }
  return id;
}

uint32_t TFT_SPI::ReadID(uint16_t Reg) {
  #if PIN_EXISTS(TFT_MISO)
    uint32_t Data = 0;

    DataTransferBegin(eSPIMode::READ);
    WriteReg8(Reg);

    spiRead( &Data, 4 );
    DataTransferEnd();

    return Data >> 7;
  #else
    return 0;
  #endif
}

bool TFT_SPI::isBusy() {
#if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
  return spiAsyncIsRunning();
#else
  return false;
#endif
}

void TFT_SPI::Abort() {
#if ENABLED(HAL_SPI_SUPPORTS_ASYNC)
  spiAsyncAbort();
#endif
}

void TFT_SPI::Transmit(uint16_t Data) {
  spiSend16(Data);
}

void TFT_SPI::Transmit8(uint8_t Data) {
  spiSend(Data);
}

void TFT_SPI::TransmitDMA(const uint16_t *Data, uint16_t Count) {
  DataTransferBegin(eSPIMode::WRITE);
  spiWrite16(Data, Count);
  DataTransferEnd();
}

void TFT_SPI::TransmitRepeat(const uint16_t Data, uint16_t Count) {

  DataTransferBegin(eSPIMode::WRITE);
  spiWriteRepeat16(Data, Count);
  DataTransferEnd();
}

#if ENABLED(HAL_SPI_SUPPORTS_ASYNC)

static void (*_txcomplete_callback)(void*) = nullptr;
void *_txcomplete_ud = nullptr;

void TFT_SPI::_TXComplete( void *ud )
{
  auto cb = _txcomplete_callback;
  auto _ud = _txcomplete_ud;

  DataTransferEnd();

  if (cb) {
    cb( _ud );
  }
}

void TFT_SPI::TransmitDMA_Async(const uint16_t *Data, uint16_t Count, void (*completeCallback)(void*), void *ud) {
  DataTransferBegin(eSPIMode::WRITE);

  _txcomplete_callback = completeCallback;
  _txcomplete_ud = ud;

  spiWriteAsync16(Data, Count, TFT_SPI::_TXComplete, nullptr);
}

#endif

#endif // HAS_SPI_TFT