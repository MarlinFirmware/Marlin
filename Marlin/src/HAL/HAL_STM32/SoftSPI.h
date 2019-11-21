//TODO: check this if you need it.

// ------------------------
// Software SPI
// ------------------------

#pragma once
#include "Arduino.h"

#define nop asm volatile ("\tnop\n")

/** Pullups disabled for MISO are disabled. */
#define MISO_LEVEL false

/**
 * @class SoftSPI
 * @brief Software SPI for when Hardware isn't available
 */
template<uint8_t MisoPin, uint8_t MosiPin, uint8_t SckPin, uint8_t Mode = 0>
class SoftSPI {
 public:
     SoftSPI(uint8_t mosi, uint8_t miso, uint8_t sclk, uint8_t mode = 0);

  //----------------------------------------------------------------------------
  /** Initialize SoftSPI pins. */
  void begin() {
    pinMode(MisoPin, INPUT);
    pinMode(MosiPin, OUTPUT);
    pinMode(SckPin, OUTPUT);

    digitalWrite(MisoPin, MISO_LEVEL);
    digitalWrite(MosiPin, !MODE_CPHA(Mode));
    digitalWrite(SckPin, MODE_CPOL(Mode));
  }
  //----------------------------------------------------------------------------
  /** Soft SPI receive byte.
   * @return Data byte received.
   */
  inline __attribute__((always_inline)) uint8_t receive() {
    uint8_t data = 0;
    receiveBit(7, &data);
    receiveBit(6, &data);
    receiveBit(5, &data);
    receiveBit(4, &data);
    receiveBit(3, &data);
    receiveBit(2, &data);
    receiveBit(1, &data);
    receiveBit(0, &data);
    return data;
  }
  //----------------------------------------------------------------------------
  /** Soft SPI send byte.
   * @param[in] data Data byte to send.
   */
  inline __attribute__((always_inline))
  void send(uint8_t data) {
    sendBit(7, data);
    sendBit(6, data);
    sendBit(5, data);
    sendBit(4, data);
    sendBit(3, data);
    sendBit(2, data);
    sendBit(1, data);
    sendBit(0, data);
  }
  //----------------------------------------------------------------------------
  /** Soft SPI transfer byte.
   * @param[in] txData Data byte to send.
   * @return Data byte received.
   */
  inline __attribute__((always_inline))
  uint8_t transfer(uint8_t txData) {
    uint8_t rxData = 0;
    transferBit(7, &rxData, txData);
    transferBit(6, &rxData, txData);
    transferBit(5, &rxData, txData);
    transferBit(4, &rxData, txData);
    transferBit(3, &rxData, txData);
    transferBit(2, &rxData, txData);
    transferBit(1, &rxData, txData);
    transferBit(0, &rxData, txData);
    return rxData;
  }

 private:
  inline __attribute__((always_inline)) bool MODE_CPHA(uint8_t mode) {return (mode & 1) != 0;}
  inline __attribute__((always_inline)) bool MODE_CPOL(uint8_t mode) {return (mode & 2) != 0;}

  inline __attribute__((always_inline)) void receiveBit(uint8_t bit, uint8_t* data) {
    if (MODE_CPHA(Mode)) digitalWrite(SckPin, !MODE_CPOL(Mode));
    nop; nop;
    digitalWrite(SckPin, MODE_CPHA(Mode) ? MODE_CPOL(Mode) : !MODE_CPOL(Mode));

    if (digitalRead(MisoPin)) *data |= 1 << bit;
    if (!MODE_CPHA(Mode)) digitalWrite(SckPin, MODE_CPOL(Mode));
  }

  inline __attribute__((always_inline)) void sendBit(uint8_t bit, uint8_t data) {
    if (MODE_CPHA(Mode)) digitalWrite(SckPin, !MODE_CPOL(Mode));

    digitalWrite(MosiPin, data & (1 << bit));
    digitalWrite(SckPin, MODE_CPHA(Mode) ? MODE_CPOL(Mode) : !MODE_CPOL(Mode));
    nop; nop;
    if (!MODE_CPHA(Mode)) digitalWrite(SckPin, MODE_CPOL(Mode));
  }

  inline __attribute__((always_inline)) void transferBit(uint8_t bit, uint8_t* rxData, uint8_t txData) {
    if (MODE_CPHA(Mode)) digitalWrite(SckPin, !MODE_CPOL(Mode));

    digitalWrite(MosiPin, txData & (1 << bit));
    digitalWrite(SckPin, MODE_CPHA(Mode) ? MODE_CPOL(Mode) : !MODE_CPOL(Mode));
    if (digitalRead(MisoPin)) *rxData |= 1 << bit;
    if (!MODE_CPHA(Mode)) digitalWrite(SckPin, MODE_CPOL(Mode));
  }
};
