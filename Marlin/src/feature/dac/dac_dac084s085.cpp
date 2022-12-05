/***************************************************************
 *
 * External DAC for Alligator Board
 *
 ****************************************************************/

#include "../../inc/MarlinConfig.h"

#if MB(ALLIGATOR)

#include "dac_dac084s085.h"

#include "../../MarlinCore.h"
#include "../../HAL/shared/Delay.h"

dac084s085::dac084s085() { }

void dac084s085::begin() {
  uint8_t externalDac_buf[] = { 0x20, 0x00 }; // all off

  // All SPI chip-select HIGH
  SET_OUTPUT(DAC0_SYNC_PIN);
  #if HAS_MULTI_EXTRUDER
    SET_OUTPUT(DAC1_SYNC_PIN);
  #endif
  cshigh();
  spiBegin();

  //init onboard DAC
  DELAY_US(2);
  WRITE(DAC0_SYNC_PIN, LOW);
  DELAY_US(2);
  WRITE(DAC0_SYNC_PIN, HIGH);
  DELAY_US(2);
  WRITE(DAC0_SYNC_PIN, LOW);

  spiSend(SPI_CHAN_DAC, externalDac_buf, COUNT(externalDac_buf));
  WRITE(DAC0_SYNC_PIN, HIGH);

  #if HAS_MULTI_EXTRUDER
    //init Piggy DAC
    DELAY_US(2);
    WRITE(DAC1_SYNC_PIN, LOW);
    DELAY_US(2);
    WRITE(DAC1_SYNC_PIN, HIGH);
    DELAY_US(2);
    WRITE(DAC1_SYNC_PIN, LOW);

    spiSend(SPI_CHAN_DAC, externalDac_buf, COUNT(externalDac_buf));
    WRITE(DAC1_SYNC_PIN, HIGH);
  #endif

  return;
}

void dac084s085::setValue(const uint8_t channel, const uint8_t value) {
  if (channel >= 7) return; // max channel (X,Y,Z,E0,E1,E2,E3)

  const uint8_t externalDac_buf[] = {
    0x10 | ((channel > 3 ? 7 : 3) - channel << 6) | (value >> 4),
    0x00 | (value << 4)
  };

  // All SPI chip-select HIGH
  cshigh();

  if (channel > 3) {        // DAC Piggy E1,E2,E3
    WRITE(DAC1_SYNC_PIN, LOW);
    DELAY_US(2);
    WRITE(DAC1_SYNC_PIN, HIGH);
    DELAY_US(2);
    WRITE(DAC1_SYNC_PIN, LOW);
  }
  else {                    // DAC onboard X,Y,Z,E0
    WRITE(DAC0_SYNC_PIN, LOW);
    DELAY_US(2);
    WRITE(DAC0_SYNC_PIN, HIGH);
    DELAY_US(2);
    WRITE(DAC0_SYNC_PIN, LOW);
  }

  DELAY_US(2);
  spiSend(SPI_CHAN_DAC, externalDac_buf, COUNT(externalDac_buf));
}

void dac084s085::cshigh() {
  WRITE(DAC0_SYNC_PIN, HIGH);
  #if HAS_MULTI_EXTRUDER
    WRITE(DAC1_SYNC_PIN, HIGH);
  #endif
  WRITE(SPI_EEPROM1_CS_PIN, HIGH);
  WRITE(SPI_EEPROM2_CS_PIN, HIGH);
  WRITE(SPI_FLASH_CS_PIN, HIGH);
  WRITE(SD_SS_PIN, HIGH);
}

#endif // MB(ALLIGATOR)
