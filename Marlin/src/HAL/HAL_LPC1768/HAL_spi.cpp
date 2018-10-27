/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * Software SPI functions originally from Arduino Sd2Card Library
 * Copyright (C) 2009 by William Greiman
 */

/**
 * For TARGET_LPC1768
 */

/**
 * Hardware SPI and a software SPI implementations are included in this file.
 * The hardware SPI runs faster and has higher throughput but is not compatible
 * with some LCD interfaces/adapters.
 *
 * Control of the slave select pin(s) is handled by the calling routines.
 *
 * Some of the LCD interfaces/adapters result in the LCD SPI and the SD card
 * SPI sharing pins. The SCK, MOSI & MISO pins can NOT be set/cleared with
 * WRITE nor digitalWrite when the hardware SPI module within the LPC17xx is
 * active.  If any of these pins are shared then the software SPI must be used.
 *
 * A more sophisticated hardware SPI can be found at the following link.  This
 * implementation has not been fully debugged.
 * https://github.com/MarlinFirmware/Marlin/tree/071c7a78f27078fd4aee9a3ef365fcf5e143531e
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <SPI.h>

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

#if ENABLED(LPC_SOFTWARE_SPI)

  #include "SoftwareSPI.h"

  // --------------------------------------------------------------------------
  // Software SPI
  // --------------------------------------------------------------------------

  static uint8_t SPI_speed = 0;

  static uint8_t spiTransfer(uint8_t b) {
    return swSpiTransfer(b, SPI_speed, SCK_PIN, MISO_PIN, MOSI_PIN);
  }

  void spiBegin() {
    swSpiBegin(SCK_PIN, MISO_PIN, MOSI_PIN);
  }

  void spiInit(uint8_t spiRate) {
    SPI_speed = swSpiInit(spiRate, SCK_PIN, MOSI_PIN);
  }

  uint8_t spiRec() {
    uint8_t b = spiTransfer(0xFF);
    return b;
  }

  void spiRead(uint8_t*buf, uint16_t nbyte) {
    if (nbyte == 0) return;
    for (int i = 0; i < nbyte; i++) {
      buf[i] = spiTransfer(0xFF);
    }
  }

  void spiSend(uint8_t b) {
    uint8_t response = spiTransfer(b);
    UNUSED(response);
  }

  void spiSend(const uint8_t* buf, size_t n) {
    uint8_t response;
    if (n == 0) return;
    for (uint16_t i = 0; i < n; i++) {
      response = spiTransfer(buf[i]);
    }
    UNUSED(response);
  }

  void spiSendBlock(uint8_t token, const uint8_t* buf) {
    uint8_t response;
    response = spiTransfer(token);

    for (uint16_t i = 0; i < 512; i++) {
      response = spiTransfer(buf[i]);
    }
    UNUSED(response);
    WRITE(SS_PIN, HIGH);
  }

#else

  // hardware SPI

  #include <lpc17xx_pinsel.h>
  #include <lpc17xx_ssp.h>
  #include <lpc17xx_clkpwr.h>

  void spiBegin() {  // setup SCK, MOSI & MISO pins for SSP0

    PINSEL_CFG_Type PinCfg;  // data structure to hold init values
    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Pinnum = LPC1768_PIN_PIN(SCK_PIN);
    PinCfg.Portnum = LPC1768_PIN_PORT(SCK_PIN);
    PINSEL_ConfigPin(&PinCfg);
    SET_OUTPUT(SCK_PIN);

    PinCfg.Pinnum = LPC1768_PIN_PIN(MISO_PIN);
    PinCfg.Portnum = LPC1768_PIN_PORT(MISO_PIN);
    PINSEL_ConfigPin(&PinCfg);
    SET_INPUT(MISO_PIN);

    PinCfg.Pinnum = LPC1768_PIN_PIN(MOSI_PIN);
    PinCfg.Portnum = LPC1768_PIN_PORT(MOSI_PIN);
    PINSEL_ConfigPin(&PinCfg);
    SET_OUTPUT(MOSI_PIN);
  }

  void spiInit(uint8_t spiRate) {
    SSP_Cmd(LPC_SSP0, DISABLE); // Disable SSP0 before changing rate
    // table to convert Marlin spiRates (0-5 plus default) into bit rates
    uint32_t Marlin_speed[7]; // CPSR is always 2
    Marlin_speed[0] = 8333333; //(SCR:  2)  desired: 8,000,000  actual: 8,333,333  +4.2%  SPI_FULL_SPEED
    Marlin_speed[1] = 4166667; //(SCR:  5)  desired: 4,000,000  actual: 4,166,667  +4.2%  SPI_HALF_SPEED
    Marlin_speed[2] = 2083333; //(SCR: 11)  desired: 2,000,000  actual: 2,083,333  +4.2%  SPI_QUARTER_SPEED
    Marlin_speed[3] = 1000000; //(SCR: 24)  desired: 1,000,000  actual: 1,000,000         SPI_EIGHTH_SPEED
    Marlin_speed[4] =  500000; //(SCR: 49)  desired:   500,000  actual:   500,000         SPI_SPEED_5
    Marlin_speed[5] =  250000; //(SCR: 99)  desired:   250,000  actual:   250,000         SPI_SPEED_6
    Marlin_speed[6] =  125000; //(SCR:199)  desired:   125,000  actual:   125,000         Default from HAL.h

    // divide PCLK by 2 for SSP0
    CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_SSP0, CLKPWR_PCLKSEL_CCLK_DIV_2);

    // setup for SPI mode
    SSP_CFG_Type HW_SPI_init; // data structure to hold init values
    SSP_ConfigStructInit(&HW_SPI_init);  // set values for SPI mode
    HW_SPI_init.ClockRate = Marlin_speed[MIN(spiRate, 6)]; // put in the specified bit rate
    SSP_Init(LPC_SSP0, &HW_SPI_init);  // puts the values into the proper bits in the SSP0 registers

    SSP_Cmd(LPC_SSP0, ENABLE);  // start SSP0 running
  }

  void spiSend(uint8_t b) {
    while (!SSP_GetStatus(LPC_SSP0, SSP_STAT_TXFIFO_NOTFULL));   // wait for room in the buffer
    SSP_SendData(LPC_SSP0, b & 0x00FF);
    while (SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY));  // wait for it to finish
  }


  void spiSend(const uint8_t* buf, size_t n) {
    if (n == 0) return;
    for (uint16_t i = 0; i < n; i++) {
      while (!SSP_GetStatus(LPC_SSP0, SSP_STAT_TXFIFO_NOTFULL));   // wait for room in the buffer
      SSP_SendData(LPC_SSP0, buf[i] & 0x00FF);
    }
    while (SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY));  // wait for it to finish
  }

  void spiSend(uint32_t chan, byte b) {
  }

  void spiSend(uint32_t chan, const uint8_t* buf, size_t n) {
  }

  static uint8_t get_one_byte() {
    // send a dummy byte so can clock in receive data
    SSP_SendData(LPC_SSP0,0x00FF);
    while (SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY));  // wait for it to finish
    return SSP_ReceiveData(LPC_SSP0) & 0x00FF;
  }

  // Read single byte from SPI
  uint8_t spiRec() {
    while (SSP_GetStatus(LPC_SSP0, SSP_STAT_RXFIFO_NOTEMPTY) || SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY)) SSP_ReceiveData(LPC_SSP0);  //flush the receive buffer
    return get_one_byte();
  }

  uint8_t spiRec(uint32_t chan) {
    return 0;
  }

  // Read from SPI into buffer
  void spiRead(uint8_t*buf, uint16_t nbyte) {
    while (SSP_GetStatus(LPC_SSP0, SSP_STAT_RXFIFO_NOTEMPTY) || SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY)) SSP_ReceiveData(LPC_SSP0);  //flush the receive buffer
    if (nbyte == 0) return;
    for (int i = 0; i < nbyte; i++) {
      buf[i] = get_one_byte();
    }
  }

  static uint8_t spiTransfer(uint8_t b) {
    while (SSP_GetStatus(LPC_SSP0, SSP_STAT_RXFIFO_NOTEMPTY) || SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY)) SSP_ReceiveData(LPC_SSP0);  //flush the receive buffer
    SSP_SendData(LPC_SSP0, b);  // send the byte
    while (SSP_GetStatus(LPC_SSP0, SSP_STAT_BUSY));  // wait for it to finish
    return SSP_ReceiveData(LPC_SSP0) & 0x00FF;
  }

  // Write from buffer to SPI
  void spiSendBlock(uint8_t token, const uint8_t* buf) {
  }

  /** Begin SPI transaction, set clock, bit order, data mode */
  void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
    // TODO: to be implemented

  }

#endif // ENABLED(LPC_SOFTWARE_SPI)

void SPIClass::begin() { spiBegin(); }

void SPIClass::beginTransaction(SPISettings cfg) {
  uint8_t spiRate;
  switch(cfg.spiRate()) {
    case 8000000: spiRate=0 ;break;
    case 4000000: spiRate=1 ;break;
    case 2000000: spiRate=2 ;break;
    case 1000000: spiRate=3 ;break;
    case  500000: spiRate=4 ;break;
    case  250000: spiRate=5 ;break;
    case  125000: spiRate=6 ;break;
    default: spiRate=2; break;
  }
  spiInit(spiRate);
}

uint8_t SPIClass::transfer(uint8_t B) {
  return spiTransfer(B);
}
uint16_t SPIClass::transfer16(uint16_t data) {
  uint16_t buffer;
  buffer = transfer((data>>8) & 0xFF) << 8;
  buffer |= transfer(data & 0xFF) && 0xFF;
  return buffer;
}

SPIClass SPI;

#endif // TARGET_LPC1768

