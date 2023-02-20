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
 * AVR HAL SPI implementation made by Martin Turski, company owner of EirDev
 * This implementation was specifically made for the Marlin FW.
 * For inquiries please contact turningtides@outlook.de
 * If there is any issue with this implementation, please open an inquiry on GitHub
 * and include the link in the mail. Otherwise your mail may get ignored.
 * Made to improve the reliability and future-proof Marlin.
 */

/**
 * HAL for AVR - SPI functions
 */

#ifdef __AVR__

#include "../../inc/MarlinConfig.h"

#if NONE(SOFTWARE_SPI, FORCE_SOFT_SPI, HALSPI_HW_GENERIC)

#include "registers.h"

#include <avr/boot.h>

#ifndef AVR_CHIPOSCILLATOR_FREQ
#error Missing AVR crystal oscillator frequency (AVR_CHIPOSCILLATOR_FREQ)! Consult manufacturer schematics for further details (XTAL1/XTAL2 pins as guidance)
#endif

  // ------------------------
  // Hardware SPI
  // ------------------------

  static void _spi_on_error(int code) {
    for (;;) {
#if ENABLED(HALSPI_DO_ERRORBEEPS) && PIN_EXISTS(BEEPER)
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(400);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(400);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(400);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(400);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(1000);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(1000);
      for (int n = 0; n < code; n++) {
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(500);
        OUT_WRITE(BEEPER_PIN, LOW);
        if (n < code-1)
          delay(500);
      }
      delay(1000);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(800);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(2000);
#endif
    }
  }

  #ifndef HALSPI_LOOPBEEP_TIMEOUT
    #define HALSPI_LOOPBEEP_TIMEOUT 3000
  #endif

  struct spi_monitored_loop
  {
  private:
  #if defined(HALSPI_DO_LOOPBEEPS) && PIN_EXISTS(BEEPER)
    uint32_t _start_millis;
  #endif
  public:
    inline spi_monitored_loop() {
  #if defined(HALSPI_DO_LOOPBEEPS) && PIN_EXISTS(BEEPER)
      _start_millis = millis();
  #endif
    }
    inline void update(unsigned int beep_code) {
  #if defined(HALSPI_DO_LOOPBEEPS) && PIN_EXISTS(BEEPER)
      if ((millis() - _start_millis) <= HALSPI_LOOPBEEP_TIMEOUT) return;
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(500);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(200);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(200);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(200);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(200);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(1000);
      for (unsigned int n = 0; n < beep_code; n++) {
        OUT_WRITE(BEEPER_PIN, HIGH);
        delay(200);
        OUT_WRITE(BEEPER_PIN, LOW);
        delay(200);
      }
      delay(800);
      OUT_WRITE(BEEPER_PIN, HIGH);
      delay(1000);
      OUT_WRITE(BEEPER_PIN, LOW);
      delay(2000);
  #endif
    }
  };

  static bool _spi_is_running = false;
  static int _spi_cs_pin;
  static bool _spi_transaction_is_active;
  static bool _spi_dirty_tx;

#if defined(GET_LOW_FUSE_BITS)
  static uint8_t _spi_lfuse;
#endif

  void spiBegin() {
    #if PIN_EXISTS(SD_SS)
      // Do not init HIGH for boards with pin 4 used as Fans or Heaters or otherwise, not likely to have multiple SPI devices anyway.
      #if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega1284P__)
        // SS must be in output mode even if it is not chip select
        SET_OUTPUT(SD_SS_PIN);
      #else
        // set SS high - may be chip select for another SPI device
        OUT_WRITE(SD_SS_PIN, HIGH);
      #endif
    #endif
    // This could still be required because the specification says that the DDR of those pins is "User Defined".
    // (we only support master SPI)
    SET_OUTPUT(SD_SCK_PIN);
    SET_INPUT(SD_MISO_PIN);
    SET_OUTPUT(SD_MOSI_PIN);

  #if defined(GET_LOW_FUSE_BITS)
    cli();
    _spi_lfuse = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
    sei();
  #endif

    // By default we disable the SPI peripheral.
    _PRR0._PRSPI = false;
    _SPCR._SPE = false;
    _PRR0._PRSPI = true;
  }

  // Returns the clock frequency as output by the System Clock Prescaler.
  inline uint32_t _GetSystemClockFrequency() {
    // See which clock is selected.
    const ATmega_lfuse lfuse =
#ifndef GET_LOW_FUSE_BITS
      AVR_DEFAULT_LFUSE_VALUE;
#else
      _spi_lfuse;
#endif

    uint32_t baseclk;

    switch(lfuse._CKSEL) {
    case 15: case 14: case 13: case 12: case 11: case 10: case 9: case 8:
    case 7: case 6: case 5: case 4:
      baseclk = AVR_CHIPOSCILLATOR_FREQ;
      break;
    case 3:
      // Internal 128kHz RC Oscillator.
      baseclk = 128000;
      break;
    case 2:
      // Calibrated Internal RC Oscillator.
      baseclk = 8000000;
      break;
    case 1:
    case 0:
      _spi_on_error(3);
      break;
    }

    // Divide the system clock.
    uint8_t clkps_po2 = _CLKPR._CLKPS;

    return ( baseclk >> clkps_po2 );
  }

  // We could improve these definitions if we had C++20.
  template <typename regType> // SPCR_reg_t
  inline void _spiConfigBitOrder(regType& __SPCR, int mode) {
    if (mode == SPI_BITORDER_LSB) {
      __SPCR._DORD = 1;
    }
    else if (mode == SPI_BITORDER_MSB) {
      __SPCR._DORD = 0;
    }
  }

  template <typename regType> // SPCR_reg_t
  inline void _spiConfigClockMode(regType& __SPCR, int mode) {
    if (mode == SPI_CLKMODE_0) {
      __SPCR._CPOL = 0;
      __SPCR._CPHA = 0;
    }
    else if (mode == SPI_CLKMODE_1) {
      __SPCR._CPOL = 0;
      __SPCR._CPHA = 1;
    }
    else if (mode == SPI_CLKMODE_2) {
      __SPCR._CPOL = 1;
      __SPCR._CPHA = 0;
    }
    else if (mode == SPI_CLKMODE_3) {
      __SPCR._CPOL = 1;
      __SPCR._CPHA = 1;
    }
  }

#ifndef HALSPI_AVR_NO_SLEEP

  static volatile bool _spi_txcomplete = true;

  inline void _spi_enter_wait() {
    cli();

    SMCR_reg_t __SMCR;
    __SMCR._SE = true;
    __SMCR._SM = 0; // IDLE
    __SMCR.reserved1 = 0;
    AVRHelpers::dwrite(_SMCR, __SMCR);

    // Enable the SPI interrupt.
    _SPCR._SPIE = true;
    _spi_txcomplete = false;

    sei();
  }

  inline void _spi_leave_wait() {
    SMCR_reg_t __SMCR;
    __SMCR._SE = false;
    __SMCR._SM = 0;
    __SMCR.reserved1 = 0;
    AVRHelpers::dwrite(_SMCR, __SMCR);

    // Disable the SPI interrupt.
    _SPCR._SPIE = false;
    _spi_txcomplete = true;
  }

#include <avr/interrupt.h>

  ISR(SPI_STC_vect) {
    // Make sure that a SLEEP instruction right after this interrupt call does not halt the processor.
    // This is sort of like an event variable in Win32 or a futex in Linux.
    _spi_leave_wait();
  }
#endif

  inline void _spi_waitForInterrupt() {
#ifndef HALSPI_AVR_NO_SLEEP
    // This function is meant to sleep until the AVR SPI peripheral triggers the SPIF interrupt.
    __asm__ __volatile__(
      A("SLEEP")
    );
#endif
  }

  inline bool _spi_hasTransferCompleted() {
#ifdef HALSPI_AVR_NO_SLEEP
    return _SPSR._SPIF;
#else
    return _spi_txcomplete;
#endif
  }

  inline void _spi_push(uint8_t b) {
#ifndef HALSPI_AVR_NO_SLEEP
    _spi_enter_wait();
#endif
    _SPDR = b;
  }

  /**
   * Initialize hardware SPI transaction
   */
  void spiInitEx(uint32_t maxClockFreq, const int hint_sck/*=-1*/, const int hint_miso/*=-1*/, const int hint_mosi/*=-1*/, const int hint_cs/*=-1*/) {
    if (_spi_is_running)
      _spi_on_error(1);

    // In hardware SPI mode we can only use the pre-determined SPI pins for MISO, MOSI and SCK, thus ignore the first three pin hints.
    // We only support master SPI for now.
    // For this we have to configure the chip-select (~SS) pin as output.
    // This way it cannot be driven low by external peripherals., thus keeping our master state.
    _spi_cs_pin = hint_cs;

    // Clear the power-reduction.
    _PRR0._PRSPI = false;

    // Calculate the required division to run the SPI clock below maxClockFreq.
    uint32_t sysclk = _GetSystemClockFrequency();

    SPCR_reg_t __SPCR;
    __SPCR._SPIE = false;
    __SPCR._SPE = false;
    _spiConfigBitOrder(__SPCR, SPI_BITORDER_DEFAULT);
    __SPCR._MSTR = true;
    _spiConfigClockMode(__SPCR, SPI_CLKMODE_DEFAULT);

    if ((sysclk / 2) <= maxClockFreq) {
      _SPSR._SPI2X = true;
      __SPCR._SPR = 0;
    }
    else if ((sysclk / 4) <= maxClockFreq) {
      _SPSR._SPI2X = false;
      __SPCR._SPR = 0;
    }
    else if ((sysclk / 8) <= maxClockFreq) {
      _SPSR._SPI2X = true;
      __SPCR._SPR = 1;
    }
    else if ((sysclk / 16) <= maxClockFreq) {
      _SPSR._SPI2X = false;
      __SPCR._SPR = 1;
    }
    else if ((sysclk / 32) <= maxClockFreq) {
      _SPSR._SPI2X = true;
      __SPCR._SPR = 2;
    }
    else if ((sysclk / 64) <= maxClockFreq) {
      _SPSR._SPI2X = false;
      __SPCR._SPR = 2;
    }
    else {
      // Cannot go below it.
      _SPSR._SPI2X = false;
      __SPCR._SPR = 3;
    }

    // Write initial configuration.
    AVRHelpers::dwrite(_SPCR, __SPCR);

    _spi_is_running = true;
    _spi_transaction_is_active = false;
    _spi_dirty_tx = false;
  }

  static void _maybe_start_transaction() {
    if (_spi_transaction_is_active) return;

    _SPCR._SPE = true;

    if (_spi_cs_pin >= 0) {
      _ATmega_pinMode(_spi_cs_pin, OUTPUT);
      _ATmega_digitalWrite(_spi_cs_pin, LOW);
    }

    _spi_transaction_is_active = true;
  }

  void spiInit(uint8_t spiRate, int hint_sck, int hint_miso, int hint_mosi, int hint_cs) {
    // Use datarates Marlin uses
    uint32_t clock;
    switch (spiRate) {
      case SPI_FULL_SPEED:    clock = 20000000; break; // 13.9mhz=20000000  6.75mhz=10000000  3.38mhz=5000000  .833mhz=1000000
      case SPI_HALF_SPEED:    clock =  5000000; break;
      case SPI_QUARTER_SPEED: clock =  2500000; break;
      case SPI_EIGHTH_SPEED:  clock =  1250000; break;
      case SPI_SPEED_5:       clock =   625000; break;
      case SPI_SPEED_6:       clock =   300000; break;
      default:
        clock = 4000000; // Default from the SPI library
    }
    spiInitEx(clock, hint_sck, hint_miso, hint_mosi, hint_cs);
  }

  inline void _spi_safety_delay() {
    //asm("nop"); // enable this if you encounter any delay troubles.
  }

  inline void _spi_finish_tx() {
    if (_spi_dirty_tx == false) return;

    _spi_safety_delay();
    spi_monitored_loop txew;
    while (_spi_hasTransferCompleted() == false) { txew.update(1); _spi_waitForInterrupt(); }

    _spi_dirty_tx = false;
  }

  void spiClose() {
    if (_spi_is_running == false)
      _spi_on_error(2);

    if (_spi_transaction_is_active) {
      _spi_finish_tx();

      if (_spi_cs_pin >= 0)
        _ATmega_digitalWrite(_spi_cs_pin, HIGH);

      _SPCR._SPE = false;

      _spi_transaction_is_active = false;
    }

    // Disable the peripheral again.
    _PRR0._PRSPI = true;

    _spi_is_running = false;
  }

  void spiSetBitOrder(int bitOrder) {
    if (_spi_is_running == false) _spi_on_error(4);
    _spiConfigBitOrder(_SPCR, bitOrder);
  }

  void spiSetClockMode(int clockMode) {
    if (_spi_is_running == false) _spi_on_error(4);
    _spiConfigClockMode(_SPCR, clockMode);
  }

  void spiEstablish() {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
  }

  /** SPI receive a byte */
  uint8_t spiRec(uint8_t txval) {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
    _spi_finish_tx();
    _spi_push(txval);
    _spi_safety_delay();
    spi_monitored_loop rxew;
    while (_spi_hasTransferCompleted() == false) { rxew.update(2); _spi_waitForInterrupt(); }
    return _SPDR;
  }

  inline void _split_txbytes(uint16_t txval, uint8_t& tx_first, uint8_t& tx_second, bool msb) {
    if (msb) {
      tx_first = ( txval >> 8 );
      tx_second = ( txval & 0xFF );
    }
    else {
      tx_first = ( txval & 0xFF );
      tx_second = ( txval >> 8 );
    }
  }

  inline uint16_t _fuse_txbytes(uint8_t rx_first, uint8_t rx_second, bool msb) {
    if (msb) {
      return ( (uint16_t)rx_first << 8 ) | ( (uint16_t)rx_second );
    }
    else {
      return ( (uint16_t)rx_first ) | ( (uint16_t)rx_second << 8 );
    }
  }

  uint16_t spiRec16(uint16_t txval) {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
    bool msb = ( _SPCR._DORD == 0 );
    uint8_t tx_first, tx_second;
    _split_txbytes(txval, tx_first, tx_second, msb);
    _spi_finish_tx();
    _spi_push(tx_first);
    _spi_safety_delay();
    spi_monitored_loop rxe1w;
    while (_spi_hasTransferCompleted() == false) { rxe1w.update(3); _spi_waitForInterrupt(); }
    uint8_t rx_first = _SPDR;
    _spi_push(tx_second);
    _spi_safety_delay();
    spi_monitored_loop rxe2w;
    while (_spi_hasTransferCompleted() == false) { rxe2w.update(4); _spi_waitForInterrupt(); }
    uint8_t rx_second = _SPDR;
    return _fuse_txbytes(rx_first, rx_second, msb);
  }

  /** SPI read data  */
  void spiRead(uint8_t *buf, uint16_t nbyte, uint8_t txval) {
    if (_spi_is_running == false) _spi_on_error(4);
    if (nbyte == 0) return;
    _maybe_start_transaction();
    _spi_finish_tx();
    for (uint16_t i = 0; i < nbyte; i++) {
      _spi_push(txval);
      _spi_safety_delay();
      spi_monitored_loop rxew;
      while (_spi_hasTransferCompleted() == false) { rxew.update(5); _spi_waitForInterrupt(); }
      buf[i] = _SPDR;
    }
  }

  inline void _spiSendByte(uint8_t byte) {
    _spi_finish_tx();
    _spi_push(byte);
    _spi_dirty_tx = true;
  }

  /** SPI send a byte */
  void spiSend(uint8_t b) {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
    _spiSendByte(b);
  }

  void spiSend16(uint16_t v) {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
    bool msb = ( _SPCR._DORD == 0 );
    uint8_t tx_first, tx_second;
    _split_txbytes(v, tx_first, tx_second, msb);
    _spiSendByte(tx_first);
    _spiSendByte(tx_second);
  }

  /** SPI send block  */
  void spiSendBlock(uint8_t token, const uint8_t *buf) {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
    _spiSendByte(token);
    for (uint16_t i = 0; i < 512; i++) {
      _spiSendByte(buf[i]);
    }
  }

  /** Begin SPI transaction */
  void spiWrite(const uint8_t *buf, uint16_t cnt) {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
    for (uint16_t n = 0; n < cnt; n++)
      _spiSendByte(buf[n]);
  }

  void spiWrite16(const uint16_t *buf, uint16_t cnt) {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
    bool msb = ( _SPCR._DORD == 0 );
    for (uint16_t n = 0; n < cnt; n++) {
      uint8_t tx_first, tx_second;
      _split_txbytes(buf[n], tx_first, tx_second, msb);
      _spiSendByte(tx_first);
      _spiSendByte(tx_second);
    }
  }

  void spiWriteRepeat(uint8_t val, uint16_t repcnt) {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
    for (uint16_t n = 0; n < repcnt; n++)
      _spiSendByte(val);
  }

  void spiWriteRepeat16(uint16_t val, uint16_t repcnt) {
    if (_spi_is_running == false) _spi_on_error(4);
    _maybe_start_transaction();
    bool msb = ( _SPCR._DORD == 0 );
    uint8_t tx_first, tx_second;
    _split_txbytes(val, tx_first, tx_second, msb);
    for (uint16_t n = 0; n < repcnt; n++) {
      _spiSendByte(tx_first);
      _spiSendByte(tx_second);
    }
  }

#endif

#endif
