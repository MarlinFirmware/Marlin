/***************************************************************
 *
 * External DAC for Alligator Board
 *
 ****************************************************************/

#include "../../inc/MarlinConfig.h"

#if MB(ALLIGATOR)

#include "dac_dac084s085.h"

#include "../../Marlin.h"
#include "../../module/stepper.h"

dac084s085::dac084s085() {
  return ;
}

void dac084s085::begin() {
  uint8_t externalDac_buf[2] = {0x20,0x00};//all off

  // All SPI chip-select HIGH
  pinMode(DAC0_SYNC, OUTPUT);
  digitalWrite( DAC0_SYNC , HIGH );
  #if EXTRUDERS > 1
    pinMode(DAC1_SYNC, OUTPUT);
    digitalWrite( DAC1_SYNC , HIGH );
  #endif
  digitalWrite( SPI_EEPROM1_CS , HIGH );
  digitalWrite( SPI_EEPROM2_CS , HIGH );
  digitalWrite( SPI_FLASH_CS , HIGH );
  digitalWrite( SS_PIN , HIGH );
  spiBegin();

  //init onboard DAC
  delayMicroseconds(2U);
  digitalWrite( DAC0_SYNC , LOW );
  delayMicroseconds(2U);
  digitalWrite( DAC0_SYNC , HIGH );
  delayMicroseconds(2U);
  digitalWrite( DAC0_SYNC , LOW );

  spiSend(SPI_CHAN_DAC,externalDac_buf , 2);
  digitalWrite( DAC0_SYNC , HIGH );

  #if EXTRUDERS > 1
    //init Piggy DAC
    delayMicroseconds(2U);
    digitalWrite( DAC1_SYNC , LOW );
    delayMicroseconds(2U);
    digitalWrite( DAC1_SYNC , HIGH );
    delayMicroseconds(2U);
    digitalWrite( DAC1_SYNC , LOW );

    spiSend(SPI_CHAN_DAC,externalDac_buf , 2);
    digitalWrite( DAC1_SYNC , HIGH );
  #endif

  return;
}

void dac084s085::setValue(uint8_t channel, uint8_t value) {
  if(channel >= 7) // max channel (X,Y,Z,E0,E1,E2,E3)
    return;
  if(value > 255) value = 255;

  uint8_t externalDac_buf[2] = {0x10,0x00};

  if(channel > 3)
    externalDac_buf[0] |= (7 - channel << 6);
  else
    externalDac_buf[0] |= (3 - channel << 6);

  externalDac_buf[0] |= (value >> 4);
  externalDac_buf[1] |= (value << 4);

  // All SPI chip-select HIGH
  digitalWrite( DAC0_SYNC , HIGH );
  #if EXTRUDERS > 1
    digitalWrite( DAC1_SYNC , HIGH );
  #endif
  digitalWrite( SPI_EEPROM1_CS , HIGH );
  digitalWrite( SPI_EEPROM2_CS , HIGH );
  digitalWrite( SPI_FLASH_CS , HIGH );
  digitalWrite( SS_PIN , HIGH );

  if(channel > 3) { // DAC Piggy E1,E2,E3

    digitalWrite(DAC1_SYNC , LOW);
    delayMicroseconds(2U);
    digitalWrite(DAC1_SYNC , HIGH);
    delayMicroseconds(2U);
    digitalWrite(DAC1_SYNC , LOW);
  }

  else { // DAC onboard X,Y,Z,E0

    digitalWrite(DAC0_SYNC , LOW);
    delayMicroseconds(2U);
    digitalWrite(DAC0_SYNC , HIGH);
    delayMicroseconds(2U);
    digitalWrite(DAC0_SYNC , LOW);
  }

  delayMicroseconds(2U);
  spiSend(SPI_CHAN_DAC,externalDac_buf , 2);

  return;
}

#endif // MB(ALLIGATOR)
