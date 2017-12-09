/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Description: HAL for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 */

#ifdef ARDUINO_ARCH_SAM

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../../inc/MarlinConfig.h"

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

#if ENABLED(SOFTWARE_SPI)
  // --------------------------------------------------------------------------
  // software SPI
  // --------------------------------------------------------------------------

  /* ---------------- Delay Cycles routine -------------- */

  /* https://blueprints.launchpad.net/gcc-arm-embedded/+spec/delay-cycles */

  #define nop() __asm__ __volatile__("nop;\n\t":::)

  FORCE_INLINE static void __delay_4cycles(uint32_t cy) { // +1 cycle
    #if ARCH_PIPELINE_RELOAD_CYCLES<2
      #define EXTRA_NOP_CYCLES "nop"
    #else
      #define EXTRA_NOP_CYCLES ""
    #endif

    __asm__ __volatile__(
      ".syntax unified" "\n\t" // is to prevent CM0,CM1 non-unified syntax

      "loop%=:" "\n\t"
      " subs %[cnt],#1" "\n\t"
      EXTRA_NOP_CYCLES "\n\t"
      " bne loop%=" "\n\t"
      : [cnt]"+r"(cy) // output: +r means input+output
      : // input:
      : "cc" // clobbers:
    );
  }

  FORCE_INLINE static void DELAY_CYCLES(uint32_t x) {

    if (__builtin_constant_p(x)) {

      #define MAXNOPS 4

      if (x <= (MAXNOPS)) {
        switch(x) { case 4: nop(); case 3: nop(); case 2: nop(); case 1: nop(); }
      }
      else { // because of +1 cycle inside delay_4cycles
        const uint32_t rem = (x - 1) % (MAXNOPS);
        switch(rem) { case 3: nop(); case 2: nop(); case 1: nop(); }
        if ((x = (x - 1) / (MAXNOPS)))
          __delay_4cycles(x); // if need more then 4 nop loop is more optimal
      }
    }
    else
      __delay_4cycles(x / 4);
  }

  /* ---------------- Delay in nanoseconds and in microseconds */

  #define DELAY_NS(x) DELAY_CYCLES( (x) * (F_CPU/1000000) / 1000)

  typedef uint8_t (*pfnSpiTransfer) (uint8_t b);

  // bitbanging transfer
  #define SWSPI_BIT_XFER(n) \
      WRITE(MOSI_PIN, bout & (1 << n)); \
      WRITE(SCK_PIN, HIGH); /* Sampling point */\
      /* (implicit by overhead) DELAY_NS(63); 5.3 cycles @ 84mhz */ \
      bin |= (READ(MISO_PIN) != 0) << n; \
      WRITE(SCK_PIN, LOW); /* Toggling point*/ \
      /* (implicit by overhead) DELAY_NS(63); 5.3 cycles @ 84mhz */

  // run at ~8 .. ~10Mhz
  static uint8_t spiTransfer0(uint8_t bout) { // using Mode 0
    volatile uint8_t bin = 0; /* volatile to disable deferred processing */
    SWSPI_BIT_XFER(7);
    SWSPI_BIT_XFER(6);
    SWSPI_BIT_XFER(5);
    SWSPI_BIT_XFER(4);
    SWSPI_BIT_XFER(3);
    SWSPI_BIT_XFER(2);
    SWSPI_BIT_XFER(1);
    SWSPI_BIT_XFER(0);
    return bin;
  }

  // run at ~4Mhz
  static uint8_t spiTransfer1(uint8_t b) { // using Mode 0
    int bits = 8;
    do {
      WRITE(MOSI_PIN, b & 0x80);
      b <<= 1; // little setup time

      WRITE(SCK_PIN, HIGH);
      DELAY_NS(125); // 10 cycles @ 84mhz

      b |= (READ(MISO_PIN) != 0);

      WRITE(SCK_PIN, LOW);
      DELAY_NS(125); // 10 cycles @ 84mhz
    } while (--bits);
    return b;
  }

  // all the others
  static uint32_t spiDelayCyclesX4 = (F_CPU/1000000); // 4uS => 125khz

  static uint8_t spiTransferX(uint8_t b) { // using Mode 0
    int bits = 8;
    do {
      WRITE(MOSI_PIN, b & 0x80);
      b <<= 1; // little setup time

      WRITE(SCK_PIN, HIGH);
      __delay_4cycles(spiDelayCyclesX4);

      b |= (READ(MISO_PIN) != 0);

      WRITE(SCK_PIN, LOW);
      __delay_4cycles(spiDelayCyclesX4);
    } while (--bits);
    return b;
  }

  // Use the generic one
  static pfnSpiTransfer spiTransfer = spiTransferX;

  void spiBegin() {
    SET_OUTPUT(SS_PIN);
    WRITE(SS_PIN, HIGH);
    SET_OUTPUT(SCK_PIN);
    SET_INPUT(MISO_PIN);
    SET_OUTPUT(MOSI_PIN);
  }

  /**
   * spiRate should be
   *  0 :  8 - 10 MHz
   *  1 :  4 - 5 MHz
   *  2 :  2 - 2.5 MHz
   *  3 :  1 - 1.25 MHz
   *  4 :  500 - 625 kHz
   *  5 :  250 - 312 kHz
   *  6 :  125 - 156 kHz
   */
  void spiInit(uint8_t spiRate) {
    switch (spiRate) {
      case 0:
        spiTransfer = spiTransfer0;
        break;
      case 1:
        spiTransfer = spiTransfer1;
        break;
      default:
        spiDelayCyclesX4 = (F_CPU/1000000) >> (6 - spiRate);
        spiTransfer = spiTransferX;
        break;
    }

    WRITE(SS_PIN, HIGH);
    WRITE(MOSI_PIN, HIGH);
    WRITE(SCK_PIN, LOW);
  }

  uint8_t spiRec() {
    WRITE(SS_PIN, LOW);
    uint8_t b = spiTransfer(0xff);
    WRITE(SS_PIN, HIGH);
    return b;
  }

  void spiRead(uint8_t*buf, uint16_t nbyte) {
    if (nbyte == 0) return;
    WRITE(SS_PIN, LOW);
    for (int i = 0; i < nbyte; i++) {
      buf[i] = spiTransfer(0xff);
    }
    WRITE(SS_PIN, HIGH);
  }

  void spiSend(uint8_t b) {
    WRITE(SS_PIN, LOW);
    uint8_t response = spiTransfer(b);
    UNUSED(response);
    WRITE(SS_PIN, HIGH);
  }

  static void spiSend(const uint8_t* buf, size_t n) {
    uint8_t response;
    if (n == 0) return;
    WRITE(SS_PIN, LOW);
    for (uint16_t i = 0; i < n; i++) {
      response = spiTransfer(buf[i]);
    }
    UNUSED(response);
    WRITE(SS_PIN, HIGH);
  }

  void spiSendBlock(uint8_t token, const uint8_t* buf) {
    uint8_t response;

    WRITE(SS_PIN, LOW);
    response = spiTransfer(token);

    for (uint16_t i = 0; i < 512; i++) {
      response = spiTransfer(buf[i]);
    }
    UNUSED(response);
    WRITE(SS_PIN, HIGH);
  }

  #pragma GCC reset_options

#else
  // --------------------------------------------------------------------------
  // hardware SPI
  // --------------------------------------------------------------------------
  // 8.4 MHz, 4 MHz, 2 MHz, 1 MHz, 0.5 MHz, 0.329 MHz, 0.329 MHz
  int spiDueDividors[] = { 10, 21, 42, 84, 168, 255, 255 };
  bool spiInitMaded = false;

  void spiBegin() {
    if(spiInitMaded == false) {
      // Configure SPI pins
      PIO_Configure(
         g_APinDescription[SCK_PIN].pPort,
         g_APinDescription[SCK_PIN].ulPinType,
         g_APinDescription[SCK_PIN].ulPin,
         g_APinDescription[SCK_PIN].ulPinConfiguration);
      PIO_Configure(
         g_APinDescription[MOSI_PIN].pPort,
         g_APinDescription[MOSI_PIN].ulPinType,
         g_APinDescription[MOSI_PIN].ulPin,
         g_APinDescription[MOSI_PIN].ulPinConfiguration);
      PIO_Configure(
         g_APinDescription[MISO_PIN].pPort,
         g_APinDescription[MISO_PIN].ulPinType,
         g_APinDescription[MISO_PIN].ulPin,
         g_APinDescription[MISO_PIN].ulPinConfiguration);

      // set master mode, peripheral select, fault detection
      SPI_Configure(SPI0, ID_SPI0, SPI_MR_MSTR | SPI_MR_MODFDIS | SPI_MR_PS);
      SPI_Enable(SPI0);

      #if MB(ALLIGATOR)
        SET_OUTPUT(DAC0_SYNC);
        #if EXTRUDERS > 1
          SET_OUTPUT(DAC1_SYNC);
          WRITE(DAC1_SYNC, HIGH);
        #endif
        SET_OUTPUT(SPI_EEPROM1_CS);
        SET_OUTPUT(SPI_EEPROM2_CS);
        SET_OUTPUT(SPI_FLASH_CS);
        WRITE(DAC0_SYNC, HIGH);
        WRITE(SPI_EEPROM1_CS, HIGH );
        WRITE(SPI_EEPROM2_CS, HIGH );
        WRITE(SPI_FLASH_CS, HIGH );
        WRITE(SS_PIN, HIGH );
      #endif // MB(ALLIGATOR)

      PIO_Configure(
        g_APinDescription[SPI_PIN].pPort,
        g_APinDescription[SPI_PIN].ulPinType,
        g_APinDescription[SPI_PIN].ulPin,
        g_APinDescription[SPI_PIN].ulPinConfiguration);
      spiInit(1);
      spiInitMaded = true;
    }
  }

  void spiInit(uint8_t spiRate) {
    if(spiInitMaded == false) {
      if(spiRate > 6) spiRate = 1;

      #if MB(ALLIGATOR)
        // Set SPI mode 1, clock, select not active after transfer, with delay between transfers
        SPI_ConfigureNPCS(SPI0, SPI_CHAN_DAC,
                          SPI_CSR_CSAAT | SPI_CSR_SCBR(spiDueDividors[spiRate]) |
                          SPI_CSR_DLYBCT(1));
        // Set SPI mode 0, clock, select not active after transfer, with delay between transfers
        SPI_ConfigureNPCS(SPI0, SPI_CHAN_EEPROM1, SPI_CSR_NCPHA |
                          SPI_CSR_CSAAT | SPI_CSR_SCBR(spiDueDividors[spiRate]) |
                          SPI_CSR_DLYBCT(1));
      #endif//MB(ALLIGATOR)

      // Set SPI mode 0, clock, select not active after transfer, with delay between transfers
      SPI_ConfigureNPCS(SPI0, SPI_CHAN, SPI_CSR_NCPHA |
                        SPI_CSR_CSAAT | SPI_CSR_SCBR(spiDueDividors[spiRate]) |
                        SPI_CSR_DLYBCT(1));
      SPI_Enable(SPI0);
      spiInitMaded = true;
    }
  }

  // Write single byte to SPI
  void spiSend(byte b) {
    // write byte with address and end transmission flag
    SPI0->SPI_TDR = (uint32_t)b | SPI_PCS(SPI_CHAN) | SPI_TDR_LASTXFER;
    // wait for transmit register empty
    while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
    // wait for receive register
    while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
    // clear status
    SPI0->SPI_RDR;
    //delayMicroseconds(1U);
  }

  void spiSend(const uint8_t* buf, size_t n) {
    if (n == 0) return;
    for (size_t i = 0; i < n - 1; i++) {
      SPI0->SPI_TDR = (uint32_t)buf[i] | SPI_PCS(SPI_CHAN);
      while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      SPI0->SPI_RDR;
      //delayMicroseconds(1U);
    }
    spiSend(buf[n - 1]);
  }

  void spiSend(uint32_t chan, byte b) {
    uint8_t dummy_read = 0;
    // wait for transmit register empty
    while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
    // write byte with address and end transmission flag
    SPI0->SPI_TDR = (uint32_t)b | SPI_PCS(chan) | SPI_TDR_LASTXFER;
    // wait for receive register
    while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
    // clear status
    while ((SPI0->SPI_SR & SPI_SR_RDRF) == 1)
      dummy_read = SPI0->SPI_RDR;
    UNUSED(dummy_read);
  }

  void spiSend(uint32_t chan, const uint8_t* buf, size_t n) {
    uint8_t dummy_read = 0;
    if (n == 0) return;
    for (int i = 0; i < (int)n - 1; i++) {
      while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
      SPI0->SPI_TDR = (uint32_t)buf[i] | SPI_PCS(chan);
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 1)
        dummy_read = SPI0->SPI_RDR;
      UNUSED(dummy_read);
    }
    spiSend(chan, buf[n - 1]);
  }

  // Read single byte from SPI
  uint8_t spiRec() {
    // write dummy byte with address and end transmission flag
    SPI0->SPI_TDR = 0x000000FF | SPI_PCS(SPI_CHAN) | SPI_TDR_LASTXFER;
    // wait for transmit register empty
    while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);

    // wait for receive register
    while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
    // get byte from receive register
    //delayMicroseconds(1U);
    return SPI0->SPI_RDR;
  }

  uint8_t spiRec(uint32_t chan) {
    uint8_t spirec_tmp;
    // wait for transmit register empty
    while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
    while ((SPI0->SPI_SR & SPI_SR_RDRF) == 1)
      spirec_tmp =  SPI0->SPI_RDR;
      UNUSED(spirec_tmp);

    // write dummy byte with address and end transmission flag
    SPI0->SPI_TDR = 0x000000FF | SPI_PCS(chan) | SPI_TDR_LASTXFER;

    // wait for receive register
    while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
    // get byte from receive register
    return SPI0->SPI_RDR;
  }

  // Read from SPI into buffer
  void spiRead(uint8_t*buf, uint16_t nbyte) {
    if (nbyte-- == 0) return;

    for (int i = 0; i < nbyte; i++) {
      //while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
      SPI0->SPI_TDR = 0x000000FF | SPI_PCS(SPI_CHAN);
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      buf[i] = SPI0->SPI_RDR;
      //delayMicroseconds(1U);
    }
    buf[nbyte] = spiRec();
  }

  // Write from buffer to SPI
  void spiSendBlock(uint8_t token, const uint8_t* buf) {
    SPI0->SPI_TDR = (uint32_t)token | SPI_PCS(SPI_CHAN);
    while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
    //while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
    //SPI0->SPI_RDR;
    for (int i = 0; i < 511; i++) {
      SPI0->SPI_TDR = (uint32_t)buf[i] | SPI_PCS(SPI_CHAN);
      while ((SPI0->SPI_SR & SPI_SR_TDRE) == 0);
      while ((SPI0->SPI_SR & SPI_SR_RDRF) == 0);
      SPI0->SPI_RDR;
      //delayMicroseconds(1U);
    }
    spiSend(buf[511]);
  }
#endif // ENABLED(SOFTWARE_SPI)

#endif // ARDUINO_ARCH_SAM
