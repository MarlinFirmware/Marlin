/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if HAS_TFT_XPT2046

#include "xpt2046.h"
#include "pinconfig.h"

uint16_t delta(uint16_t a, uint16_t b) { return a > b ? a - b : b - a; }

SPI_HandleTypeDef XPT2046::SPIx;
DMA_HandleTypeDef XPT2046::DMAtx;

void XPT2046::Init() {
  SPI_TypeDef *spiInstance;

  OUT_WRITE(TOUCH_CS_PIN, HIGH);

  #if PIN_EXISTS(TOUCH_INT)
    // Optional Pendrive interrupt pin
    SET_INPUT(TOUCH_INT_PIN);
  #endif

  spiInstance      = (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TOUCH_SCK_PIN),  PinMap_SPI_SCLK);
  if (spiInstance != (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TOUCH_MOSI_PIN), PinMap_SPI_MOSI)) spiInstance = NP;
  if (spiInstance != (SPI_TypeDef *)pinmap_peripheral(digitalPinToPinName(TOUCH_MISO_PIN), PinMap_SPI_MISO)) spiInstance = NP;

  SPIx.Instance                = spiInstance;

  if (SPIx.Instance) {
    SPIx.State                   = HAL_SPI_STATE_RESET;
    SPIx.Init.NSS                = SPI_NSS_SOFT;
    SPIx.Init.Mode               = SPI_MODE_MASTER;
    SPIx.Init.Direction          = SPI_DIRECTION_2LINES;
    SPIx.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_8;
    SPIx.Init.CLKPhase           = SPI_PHASE_2EDGE;
    SPIx.Init.CLKPolarity        = SPI_POLARITY_HIGH;
    SPIx.Init.DataSize           = SPI_DATASIZE_8BIT;
    SPIx.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    SPIx.Init.TIMode             = SPI_TIMODE_DISABLE;
    SPIx.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    SPIx.Init.CRCPolynomial      = 10;

    pinmap_pinout(digitalPinToPinName(TOUCH_SCK_PIN), PinMap_SPI_SCLK);
    pinmap_pinout(digitalPinToPinName(TOUCH_MOSI_PIN), PinMap_SPI_MOSI);
    pinmap_pinout(digitalPinToPinName(TOUCH_MISO_PIN), PinMap_SPI_MISO);

    #ifdef SPI1_BASE
      if (SPIx.Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        SPIx.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_16;
        #ifdef STM32F1xx
          DMAtx.Instance = DMA1_Channel3;
        #elif defined(STM32F4xx)
          DMAtx.Instance = DMA2_Stream3; // DMA2_Stream5
        #endif
        //SERIAL_ECHO_MSG(" Touch Screen on SPI1");
      }
    #endif
    #ifdef SPI2_BASE
      if (SPIx.Instance == SPI2) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        #ifdef STM32F1xx
          DMAtx.Instance = DMA1_Channel5;
        #elif defined(STM32F4xx)
          DMAtx.Instance = DMA1_Stream4;
        #endif
        //SERIAL_ECHO_MSG(" Touch Screen on SPI2");
      }
    #endif
    #ifdef SPI3_BASE
      if (SPIx.Instance == SPI3) {
        __HAL_RCC_SPI3_CLK_ENABLE();
        #ifdef STM32F1xx
          DMAtx.Instance = DMA2_Channel2;
        #elif defined(STM32F4xx)
          DMAtx.Instance = DMA1_Stream5;  // DMA1_Stream7
        #endif
        //SERIAL_ECHO_MSG(" Touch Screen on SPI3");
      }
    #endif
  }
  else {
    SPIx.Instance = NULL;
    SET_INPUT(TOUCH_MISO_PIN);
    SET_OUTPUT(TOUCH_MOSI_PIN);
    SET_OUTPUT(TOUCH_SCK_PIN);
    //SERIAL_ECHO_MSG(" Touch Screen on Software SPI");
  }

  getRawData(XPT2046_Z1);
}

bool XPT2046::isTouched() {
  return isBusy() ? false : (
    #if PIN_EXISTS(TOUCH_INT)
      READ(TOUCH_INT_PIN) != HIGH
    #else
      getRawData(XPT2046_Z1) >= XPT2046_Z1_THRESHOLD
    #endif
  );
}

bool XPT2046::getRawPoint(int16_t *x, int16_t *y) {
  if (isBusy()) return false;
  if (!isTouched()) return false;
  *x = getRawData(XPT2046_X);
  *y = getRawData(XPT2046_Y);
  return isTouched();
}

uint16_t XPT2046::getRawData(const XPTCoordinate coordinate) {
  uint16_t data[3];

  DataTransferBegin();

  for (uint16_t i = 0; i < 3 ; i++) {
    IO(coordinate);
    data[i] = (IO() << 4) | (IO() >> 4);
  }

  DataTransferEnd();

  uint16_t delta01 = delta(data[0], data[1]);
  uint16_t delta02 = delta(data[0], data[2]);
  uint16_t delta12 = delta(data[1], data[2]);

  if (delta01 > delta02 || delta01 > delta12) {
    if (delta02 > delta12)
      data[0] = data[2];
    else
      data[1] = data[2];
  }

  return (data[0] + data[1]) >> 1;
}

uint16_t XPT2046::HardwareIO(uint16_t data) {
  __HAL_SPI_ENABLE(&SPIx);
  while((SPIx.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE) {}
  SPIx.Instance->DR = data;
  while((SPIx.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE) {}
  __HAL_SPI_DISABLE(&SPIx);

  return SPIx.Instance->DR;
}

uint16_t XPT2046::SoftwareIO(uint16_t data) {
  uint16_t result = 0;

  for (uint8_t j = 0x80; j > 0; j >>= 1) {
    WRITE(TOUCH_SCK_PIN, LOW);
    __DSB();
    WRITE(TOUCH_MOSI_PIN, data & j ? HIGH : LOW);
    __DSB();
    if (READ(TOUCH_MISO_PIN)) result |= j;
    __DSB();
    WRITE(TOUCH_SCK_PIN, HIGH);
    __DSB();
  }
  WRITE(TOUCH_SCK_PIN, LOW);
  __DSB();

  return result;
}

#endif // HAS_TFT_XPT2046
