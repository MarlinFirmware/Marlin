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

/**
 * Software SPI functions originally from Arduino Sd2Card Library
 * Copyright (c) 2009 by William Greiman
 */

/**
 * For TARGET_LPC1768
 */

/**
 * Hardware SPI and Software SPI implementations are included in this file.
 * The hardware SPI runs faster and has higher throughput but is not compatible
 * with some LCD interfaces/adapters.
 *
 * Control of the slave select pin(s) is handled by the calling routines.
 *
 * Some of the LCD interfaces/adapters result in the LCD SPI and the SD card
 * SPI sharing pins. The SCK, MOSI & MISO pins can NOT be set/cleared with
 * WRITE nor digitalWrite when the hardware SPI module within the LPC17xx is
 * active. If any of these pins are shared then the software SPI must be used.
 *
 * A more sophisticated hardware SPI can be found at the following link.
 * This implementation has not been fully debugged.
 * https://github.com/MarlinFirmware/Marlin/tree/071c7a78f27078fd4aee9a3ef365fcf5e143531e
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"
#include <SPI.h>

// Hardware SPI and SPIClass
#include <lpc17xx_pinsel.h>
#include <lpc17xx_clkpwr.h>

#include "../shared/HAL_SPI.h"

// ------------------------
// Public functions
// ------------------------
#if ENABLED(SOFTWARE_SPI)

  // Software SPI

  #include <SoftwareSPI.h>

  static uint8_t SPI_speed = SPI_FULL_SPEED;

  static uint8_t spiTransfer(uint8_t b) {
    return swSpiTransfer(b, SPI_speed, SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN);
  }

  void spiBegin() {
    swSpiBegin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN);
  }

  void spiInit(uint8_t spiRate) {
    SPI_speed = swSpiInit(spiRate, SD_SCK_PIN, SD_MOSI_PIN);
  }

  uint8_t spiRec() { return spiTransfer(0xFF); }

  void spiRead(uint8_t*buf, uint16_t nbyte) {
    for (int i = 0; i < nbyte; i++)
      buf[i] = spiTransfer(0xFF);
  }

  void spiSend(uint8_t b) { (void)spiTransfer(b); }

  void spiSend(const uint8_t *buf, size_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++)
      (void)spiTransfer(buf[i]);
  }

  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    (void)spiTransfer(token);
    for (uint16_t i = 0; i < 512; i++)
      (void)spiTransfer(buf[i]);
  }

#else

  #ifdef SD_SPI_SPEED
    #define INIT_SPI_SPEED SD_SPI_SPEED
  #else
    #define INIT_SPI_SPEED SPI_FULL_SPEED
  #endif

  void spiBegin() { spiInit(INIT_SPI_SPEED); } // Set up SCK, MOSI & MISO pins for SSP0

  void spiInit(uint8_t spiRate) {
    #if SD_MISO_PIN == BOARD_SPI1_MISO_PIN
      SPI.setModule(1);
    #elif SD_MISO_PIN == BOARD_SPI2_MISO_PIN
      SPI.setModule(2);
    #endif
    SPI.setDataSize(DATA_SIZE_8BIT);
    SPI.setDataMode(SPI_MODE0);

    SPI.setClock(SPISettings::spiRate2Clock(spiRate));
    SPI.begin();
  }

  static uint8_t doio(uint8_t b) {
    return SPI.transfer(b & 0x00FF) & 0x00FF;
  }

  void spiSend(uint8_t b) { doio(b); }

  void spiSend(const uint8_t *buf, size_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++) doio(buf[i]);
  }

  void spiSend(uint32_t chan, byte b) {}

  void spiSend(uint32_t chan, const uint8_t *buf, size_t nbyte) {}

  // Read single byte from SPI
  uint8_t spiRec() { return doio(0xFF); }

  uint8_t spiRec(uint32_t chan) { return 0; }

  // Read from SPI into buffer
  void spiRead(uint8_t *buf, uint16_t nbyte) {
    for (uint16_t i = 0; i < nbyte; i++) buf[i] = doio(0xFF);
  }

  uint8_t spiTransfer(uint8_t b) { return doio(b); }

  // Write from buffer to SPI
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
   (void)spiTransfer(token);
    for (uint16_t i = 0; i < 512; i++)
      (void)spiTransfer(buf[i]);
  }

  // Begin SPI transaction, set clock, bit order, data mode
  void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
    // TODO: Implement this method
  }

#endif // SOFTWARE_SPI

/**
 * @brief Wait until TXE (tx empty) flag is set and BSY (busy) flag unset.
 */
static inline void waitSpiTxEnd(LPC_SSP_TypeDef *spi_d) {
  while (SSP_GetStatus(spi_d, SSP_STAT_TXFIFO_EMPTY) == RESET) { /* nada */ } // wait until TXE=1
  while (SSP_GetStatus(spi_d, SSP_STAT_BUSY) == SET) { /* nada */ }     // wait until BSY=0
}

// Retain the pin init state of the SPI, to avoid init more than once,
// even if more instances of SPIClass exist
static bool spiInitialised[BOARD_NR_SPI] = { false };

SPIClass::SPIClass(uint8_t device) {
  // Init things specific to each SPI device
  // clock divider setup is a bit of hack, and needs to be improved at a later date.

  #if BOARD_NR_SPI >= 1
    _settings[0].spi_d = LPC_SSP0;
    _settings[0].dataMode = SPI_MODE0;
    _settings[0].dataSize = DATA_SIZE_8BIT;
    _settings[0].clock = SPI_CLOCK_MAX;
    //_settings[0].clockDivider = determine_baud_rate(_settings[0].spi_d, _settings[0].clock);
  #endif

  #if BOARD_NR_SPI >= 2
    _settings[1].spi_d = LPC_SSP1;
    _settings[1].dataMode = SPI_MODE0;
    _settings[1].dataSize = DATA_SIZE_8BIT;
    _settings[1].clock = SPI_CLOCK_MAX;
    //_settings[1].clockDivider = determine_baud_rate(_settings[1].spi_d, _settings[1].clock);
  #endif

  setModule(device);

  // Init the GPDMA controller
  // TODO: call once in the constructor? or each time?
  GPDMA_Init();
}

SPIClass::SPIClass(pin_t mosi, pin_t miso, pin_t sclk, pin_t ssel) {
  #if BOARD_NR_SPI >= 1
    if (mosi == BOARD_SPI1_MOSI_PIN) SPIClass(1);
  #endif
  #if BOARD_NR_SPI >= 2
    if (mosi == BOARD_SPI2_MOSI_PIN) SPIClass(2);
  #endif
}

void SPIClass::begin() {
  // Init the SPI pins in the first begin call
  if ((_currentSetting->spi_d == LPC_SSP0 && spiInitialised[0] == false) ||
      (_currentSetting->spi_d == LPC_SSP1 && spiInitialised[1] == false)) {
    pin_t sck, miso, mosi;
    if (_currentSetting->spi_d == LPC_SSP0) {
      sck = BOARD_SPI1_SCK_PIN;
      miso = BOARD_SPI1_MISO_PIN;
      mosi = BOARD_SPI1_MOSI_PIN;
      spiInitialised[0] = true;
    }
    else if (_currentSetting->spi_d == LPC_SSP1) {
      sck = BOARD_SPI2_SCK_PIN;
      miso = BOARD_SPI2_MISO_PIN;
      mosi = BOARD_SPI2_MOSI_PIN;
      spiInitialised[1] = true;
    }
    PINSEL_CFG_Type PinCfg;  // data structure to hold init values
    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = LPC176x::pin_bit(sck);
    PinCfg.Portnum = LPC176x::pin_port(sck);
    PINSEL_ConfigPin(&PinCfg);
    SET_OUTPUT(sck);

    PinCfg.Pinnum = LPC176x::pin_bit(miso);
    PinCfg.Portnum = LPC176x::pin_port(miso);
    PINSEL_ConfigPin(&PinCfg);
    SET_INPUT(miso);

    PinCfg.Pinnum = LPC176x::pin_bit(mosi);
    PinCfg.Portnum = LPC176x::pin_port(mosi);
    PINSEL_ConfigPin(&PinCfg);
    SET_OUTPUT(mosi);
  }

  updateSettings();
  SSP_Cmd(_currentSetting->spi_d, ENABLE);  // start SSP running
}

void SPIClass::beginTransaction(const SPISettings &cfg) {
  setBitOrder(cfg.bitOrder);
  setDataMode(cfg.dataMode);
  setDataSize(cfg.dataSize);
  //setClockDivider(determine_baud_rate(_currentSetting->spi_d, settings.clock));
  begin();
}

uint8_t SPIClass::transfer(const uint16_t b) {
  // Send and receive a single byte
  SSP_ReceiveData(_currentSetting->spi_d); // read any previous data
  SSP_SendData(_currentSetting->spi_d, b);
  waitSpiTxEnd(_currentSetting->spi_d);  // wait for it to finish
  return SSP_ReceiveData(_currentSetting->spi_d);
}

uint16_t SPIClass::transfer16(const uint16_t data) {
  return (transfer((data >> 8) & 0xFF) << 8) | (transfer(data & 0xFF) & 0xFF);
}

void SPIClass::end() {
  // Neither is needed for Marlin
  //SSP_Cmd(_currentSetting->spi_d, DISABLE);
  //SSP_DeInit(_currentSetting->spi_d);
}

void SPIClass::send(uint8_t data) {
  SSP_SendData(_currentSetting->spi_d, data);
}

void SPIClass::dmaSend(void *buf, uint16_t length, bool minc) {
  //TODO: LPC dma can only write 0xFFF bytes at once.
  GPDMA_Channel_CFG_Type GPDMACfg;

  /* Configure GPDMA channel 0 -------------------------------------------------------------*/
  /* DMA Channel 0 */
  GPDMACfg.ChannelNum = 0;
  // Source memory
  GPDMACfg.SrcMemAddr = (uint32_t)buf;
  // Destination memory - Not used
  GPDMACfg.DstMemAddr = 0;
  // Transfer size
  GPDMACfg.TransferSize = length;
  // Transfer width
  GPDMACfg.TransferWidth = (_currentSetting->dataSize == DATA_SIZE_16BIT) ? GPDMA_WIDTH_HALFWORD : GPDMA_WIDTH_BYTE;
  // Transfer type
  GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2P;
  // Source connection - unused
  GPDMACfg.SrcConn = 0;
  // Destination connection
  GPDMACfg.DstConn = (_currentSetting->spi_d == LPC_SSP0) ? GPDMA_CONN_SSP0_Tx : GPDMA_CONN_SSP1_Tx;

  GPDMACfg.DMALLI = 0;

  // Enable dma on SPI
  SSP_DMACmd(_currentSetting->spi_d, SSP_DMA_TX, ENABLE);

  // Only increase memory if minc is true
  GPDMACfg.MemoryIncrease = (minc ? GPDMA_DMACCxControl_SI : 0);

  // Setup channel with given parameter
  GPDMA_Setup(&GPDMACfg);

  // Enable DMA
  GPDMA_ChannelCmd(0, ENABLE);

  /**
   * Observed behaviour on normal data transfer completion (SKR 1.3 board / LPC1768 MCU)
   *   GPDMA_STAT_INTTC flag is SET
   *   GPDMA_STAT_INTERR flag is NOT SET
   *   GPDMA_STAT_RAWINTTC flag is NOT SET
   *   GPDMA_STAT_RAWINTERR flag is SET
   */

  // Wait for data transfer
  while (!GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0) && !GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0)) {}

  // Clear err and int
  GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
  GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);

  // Disable DMA
  GPDMA_ChannelCmd(0, DISABLE);

  waitSpiTxEnd(_currentSetting->spi_d);

  SSP_DMACmd(_currentSetting->spi_d, SSP_DMA_TX, DISABLE);
}

void SPIClass::dmaSendAsync(void *buf, uint16_t length, bool minc) {
  //TODO: LPC dma can only write 0xFFF bytes at once.
  GPDMA_Channel_CFG_Type GPDMACfg;

  /* Configure GPDMA channel 0 -------------------------------------------------------------*/
  /* DMA Channel 0 */
  GPDMACfg.ChannelNum = 0;
  // Source memory
  GPDMACfg.SrcMemAddr = (uint32_t)buf;
  // Destination memory - Not used
  GPDMACfg.DstMemAddr = 0;
  // Transfer size
  GPDMACfg.TransferSize = length;
  // Transfer width
  GPDMACfg.TransferWidth = (_currentSetting->dataSize == DATA_SIZE_16BIT) ? GPDMA_WIDTH_HALFWORD : GPDMA_WIDTH_BYTE;
  // Transfer type
  GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2P;
  // Source connection - unused
  GPDMACfg.SrcConn = 0;
  // Destination connection
  GPDMACfg.DstConn = (_currentSetting->spi_d == LPC_SSP0) ? GPDMA_CONN_SSP0_Tx : GPDMA_CONN_SSP1_Tx;

  GPDMACfg.DMALLI = 0;

  // Enable dma on SPI
  SSP_DMACmd(_currentSetting->spi_d, SSP_DMA_TX, ENABLE);

  // Only increase memory if minc is true
  GPDMACfg.MemoryIncrease = (minc ? GPDMA_DMACCxControl_SI : 0);

  // Setup channel with given parameter
  GPDMA_Setup(&GPDMACfg);

  // Enable DMA
  GPDMA_ChannelCmd(0, ENABLE);
}

uint16_t SPIClass::read() {
  return SSP_ReceiveData(_currentSetting->spi_d);
}

void SPIClass::read(uint8_t *buf, uint32_t len) {
  for (uint16_t i = 0; i < len; i++) buf[i] = transfer(0xFF);
}

void SPIClass::setClock(uint32_t clock) { _currentSetting->clock = clock; }

void SPIClass::setModule(uint8_t device) { _currentSetting = &_settings[device - 1]; } // SPI channels are called 1, 2, and 3 but the array is zero-indexed

void SPIClass::setBitOrder(uint8_t bitOrder) { _currentSetting->bitOrder = bitOrder; }

void SPIClass::setDataMode(uint8_t dataMode) { _currentSetting->dataMode = dataMode; }

void SPIClass::setDataSize(uint32_t dataSize) { _currentSetting->dataSize = dataSize; }

/**
 * Set up/tear down
 */
void SPIClass::updateSettings() {
  //SSP_DeInit(_currentSetting->spi_d); //todo: need force de init?!

  // Divide PCLK by 2 for SSP0
  //CLKPWR_SetPCLKDiv(_currentSetting->spi_d == LPC_SSP0 ? CLKPWR_PCLKSEL_SSP0 : CLKPWR_PCLKSEL_SSP1, CLKPWR_PCLKSEL_CCLK_DIV_2);

  SSP_CFG_Type HW_SPI_init; // data structure to hold init values
  SSP_ConfigStructInit(&HW_SPI_init);  // set values for SPI mode
  HW_SPI_init.ClockRate = _currentSetting->clock;
  HW_SPI_init.Databit = _currentSetting->dataSize;

  /**
   * SPI Mode  CPOL  CPHA  Shift SCK-edge  Capture SCK-edge
   * 0       0     0     Falling     Rising
   * 1       0     1     Rising      Falling
   * 2       1     0     Rising      Falling
   * 3       1     1     Falling     Rising
   */
  switch (_currentSetting->dataMode) {
    case SPI_MODE0:
      HW_SPI_init.CPHA = SSP_CPHA_FIRST;
      HW_SPI_init.CPOL = SSP_CPOL_HI;
      break;
    case SPI_MODE1:
      HW_SPI_init.CPHA = SSP_CPHA_SECOND;
      HW_SPI_init.CPOL = SSP_CPOL_HI;
      break;
    case SPI_MODE2:
      HW_SPI_init.CPHA = SSP_CPHA_FIRST;
      HW_SPI_init.CPOL = SSP_CPOL_LO;
      break;
    case SPI_MODE3:
      HW_SPI_init.CPHA = SSP_CPHA_SECOND;
      HW_SPI_init.CPOL = SSP_CPOL_LO;
      break;
    default:
      break;
  }

  // TODO: handle bitOrder
  SSP_Init(_currentSetting->spi_d, &HW_SPI_init);  // puts the values into the proper bits in the SSP0 registers
}

#if SD_MISO_PIN == BOARD_SPI1_MISO_PIN
  SPIClass SPI(1);
#elif SD_MISO_PIN == BOARD_SPI2_MISO_PIN
  SPIClass SPI(2);
#endif

#endif // TARGET_LPC1768
