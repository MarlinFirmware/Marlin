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

#ifdef HAL_STM32

#include "../../../inc/MarlinConfig.h"

#if HAS_TFT_XPT2046 || HAS_RES_TOUCH_BUTTONS

#include "xpt2046.h"
#include "pinconfig.h"

uint16_t delta(uint16_t a, uint16_t b) { return a > b ? a - b : b - a; }

SPI_HandleTypeDef XPT2046::SPIx;

uint8_t XPT2046::_GetClockDivider(uint32_t spibasefreq, uint32_t speed) {
  if (speed >= (spibasefreq /   2)) return SPI_BAUDRATEPRESCALER_2;
  if (speed >= (spibasefreq /   4)) return SPI_BAUDRATEPRESCALER_4;
  if (speed >= (spibasefreq /   8)) return SPI_BAUDRATEPRESCALER_8;
  if (speed >= (spibasefreq /  16)) return SPI_BAUDRATEPRESCALER_16;
  if (speed >= (spibasefreq /  32)) return SPI_BAUDRATEPRESCALER_32;
  if (speed >= (spibasefreq /  64)) return SPI_BAUDRATEPRESCALER_64;
  if (speed >= (spibasefreq / 128)) return SPI_BAUDRATEPRESCALER_128;
  return SPI_BAUDRATEPRESCALER_256;
}

extern "C" {
  #include <utility/spi_com.h>
}

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

  SPIx.Instance = spiInstance;

  if (spiInstance) {
    uint8_t clkdiv;
    #ifdef TOUCH_BAUDRATE
      spi_t tmp_spi;
      tmp_spi.pin_sclk = digitalPinToPinName(TOUCH_SCK_PIN);
      uint32_t spibasefreq = spi_getClkFreq(&tmp_spi);
      clkdiv = _GetClockDivider(spibasefreq, TOUCH_BAUDRATE);
    #else
      bool has_clkdiv = false;
      #ifdef SPI1_BASE
        if (spiInstance == SPI1) {
          clkdiv = SPI_BAUDRATEPRESCALER_16;
          has_clkdiv = true;
        }
      #endif
      if (!has_clkdiv) clkdiv = SPI_BAUDRATEPRESCALER_8;
    #endif

    SPIx.State                   = HAL_SPI_STATE_RESET;
    SPIx.Init.NSS                = SPI_NSS_SOFT;
    SPIx.Init.Mode               = SPI_MODE_MASTER;
    SPIx.Init.Direction          = SPI_DIRECTION_2LINES;
    SPIx.Init.BaudRatePrescaler  = clkdiv;
    SPIx.Init.CLKPhase           = SPI_PHASE_2EDGE;
    SPIx.Init.CLKPolarity        = SPI_POLARITY_HIGH;
    SPIx.Init.DataSize           = SPI_DATASIZE_8BIT;
    SPIx.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    SPIx.Init.TIMode             = SPI_TIMODE_DISABLE;
    SPIx.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    SPIx.Init.CRCPolynomial      = 10;
  }
  else {
    SET_INPUT(TOUCH_MISO_PIN);
    SET_OUTPUT(TOUCH_MOSI_PIN);
    SET_OUTPUT(TOUCH_SCK_PIN);
  }

  getRawData(XPT2046_Z1);
}

void XPT2046::HAL_SPI_Prepare() {
  pinmap_pinout(digitalPinToPinName(TOUCH_SCK_PIN), PinMap_SPI_SCLK);
  pinmap_pinout(digitalPinToPinName(TOUCH_MOSI_PIN), PinMap_SPI_MOSI);
  pinmap_pinout(digitalPinToPinName(TOUCH_MISO_PIN), PinMap_SPI_MISO);

  pin_PullConfig(get_GPIO_Port(STM_PORT(digitalPinToPinName(TOUCH_SCK_PIN))), STM_LL_GPIO_PIN(digitalPinToPinName(TOUCH_SCK_PIN)), GPIO_PULLDOWN);

  #define SPIX_PREPARE(I) \
    if (SPIx.Instance == SPI##I) { \
      __HAL_RCC_SPI##I##_CLK_ENABLE(); \
      __HAL_RCC_SPI##I##_FORCE_RESET(); \
      __HAL_RCC_SPI##I##_RELEASE_RESET(); \
    }
  #ifdef SPI1_BASE
    SPIX_PREPARE(1);
  #endif
  #ifdef SPI2_BASE
    SPIX_PREPARE(2);
  #endif
  #ifdef SPI3_BASE
    SPIX_PREPARE(3);
  #endif
  #ifdef SPI4_BASE
    SPIX_PREPARE(4);
  #endif
  #ifdef SPI5_BASE
    SPIX_PREPARE(5);
  #endif
  #ifdef SPI6_BASE
    SPIX_PREPARE(6);
  #endif
}

void XPT2046::HAL_SPI_Dismantle() {
  #define SPIX_DISMANTLE(I) \
    if (SPIx.Instance == SPI##I) { \
      __HAL_RCC_SPI##I##_FORCE_RESET(); \
      __HAL_RCC_SPI##I##_RELEASE_RESET(); \
      __HAL_RCC_SPI##I##_CLK_DISABLE(); \
    }
  #ifdef SPI1_BASE
    SPIX_DISMANTLE(1);
  #endif
  #ifdef SPI2_BASE
    SPIX_DISMANTLE(2);
  #endif
  #ifdef SPI3_BASE
    SPIX_DISMANTLE(3);
  #endif
  #ifdef SPI4_BASE
    SPIX_DISMANTLE(4);
  #endif
  #ifdef SPI5_BASE
    SPIX_DISMANTLE(5);
  #endif
  #ifdef SPI6_BASE
    SPIX_DISMANTLE(6);
  #endif
}

void XPT2046::DataTransferBegin() {
  if (SPIx.Instance) {
    HAL_SPI_Prepare();
    HAL_SPI_Init(&SPIx);
  }
  WRITE(TOUCH_CS_PIN, LOW);
}

void XPT2046::DataTransferEnd() {
  WRITE(TOUCH_CS_PIN, HIGH);
  if (SPIx.Instance) {
    HAL_SPI_DeInit(&SPIx);
    HAL_SPI_Dismantle();
  }
}

bool XPT2046::isTouched() {
  if (isBusy()) return false;
  return (
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
  while ((SPIx.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE) {}
  SPIx.Instance->DR = data;
  while ((SPIx.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE) {}
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
#endif // HAL_STM32
