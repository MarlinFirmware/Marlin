#if defined(__MK64FX512__) || defined(__MK66FX1M0__)

#include "../HAL.h"
#include <SPI.h>
#include <pins_arduino.h>
#include "spi_pins.h"
#include "../../core/macros.h"

static SPISettings spiConfig;

// Standard SPI functions
/** Initialise SPI bus */
void spiBegin(void) {
  #if !PIN_EXISTS(SS)
    #error SS_PIN not defined!
  #endif
  SET_OUTPUT(SS_PIN);
  WRITE(SS_PIN, HIGH);
  SET_OUTPUT(SCK_PIN);
  SET_INPUT(MISO_PIN);
  SET_OUTPUT(MOSI_PIN);

  //#if DISABLED(SOFTWARE_SPI)
  #if 0
    // set SS high - may be chip select for another SPI device
    #if SET_SPI_SS_HIGH
      WRITE(SS_PIN, HIGH);
    #endif  // SET_SPI_SS_HIGH
    // set a default rate
    spiInit(SPI_HALF_SPEED); // 1
  #endif  // SOFTWARE_SPI
}

/** Configure SPI for specified SPI speed */
void spiInit(uint8_t spiRate) {
  // Use datarates Marlin uses
  uint32_t clock;
  switch (spiRate) {
  case SPI_FULL_SPEED:    clock = 10000000; break;
  case SPI_HALF_SPEED:    clock =  5000000; break;
  case SPI_QUARTER_SPEED: clock =  2500000; break;
  case SPI_EIGHTH_SPEED:  clock =  1250000; break;
  case SPI_SPEED_5:       clock =   625000; break;
  case SPI_SPEED_6:       clock =   312500; break;
  default:
    clock = 4000000; // Default from the SPI libarary
  }
  spiConfig = SPISettings(clock, MSBFIRST, SPI_MODE0);
  SPI.begin();
}

//------------------------------------------------------------------------------
/** SPI receive a byte */
uint8_t spiRec(void) {
  SPI.beginTransaction(spiConfig);
  uint8_t returnByte = SPI.transfer(0xFF);
  SPI.endTransaction();
  return returnByte;
//  SPDR = 0xFF;
//  while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
//  return SPDR;
}
//------------------------------------------------------------------------------
/** SPI read data  */
void spiRead(uint8_t* buf, uint16_t nbyte) {
  SPI.beginTransaction(spiConfig);
  SPI.transfer(buf, nbyte);
  SPI.endTransaction();
//if (nbyte-- == 0) return;
//  SPDR = 0xFF;
//for (uint16_t i = 0; i < nbyte; i++) {
//  while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
//  buf[i] = SPDR;
//  SPDR = 0xFF;
//}
//while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
//buf[nbyte] = SPDR;
}
//------------------------------------------------------------------------------
/** SPI send a byte */
void spiSend(uint8_t b) {
  SPI.beginTransaction(spiConfig);
  SPI.transfer(b);
  SPI.endTransaction();
//  SPDR = b;
//  while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
}
//------------------------------------------------------------------------------
/** SPI send block  */
void spiSendBlock(uint8_t token, const uint8_t* buf) {
  SPI.beginTransaction(spiConfig);
  SPDR = token;
  for (uint16_t i = 0; i < 512; i += 2) {
    while (!TEST(SPSR, SPIF)) { /* nada */ };
    SPDR = buf[i];
    while (!TEST(SPSR, SPIF)) { /* nada */ };
    SPDR = buf[i + 1];
  }
  while (!TEST(SPSR, SPIF)) { /* nada */ };
  SPI.endTransaction();
}


/** Begin SPI transaction, set clock, bit order, data mode */
void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
  spiConfig = SPISettings(spiClock, bitOrder, dataMode);

  SPI.beginTransaction(spiConfig);
}

#endif // __MK64FX512__ || __MK66FX1M0__
