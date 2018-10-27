/**
 * Interface from Atmel USB MSD to Marlin SD card
 */

#ifdef ARDUINO_ARCH_SAM

#include "../../../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "../../../sd/cardreader.h"
extern "C" {
#include "sd_mmc_spi_mem.h"
}

#define SD_MMC_BLOCK_SIZE 512

void sd_mmc_spi_mem_init(void) {
}

Ctrl_status sd_mmc_spi_test_unit_ready(void) {
  if (!IS_SD_INSERTED || IS_SD_PRINTING || IS_SD_FILE_OPEN || !card.cardOK)
    return CTRL_NO_PRESENT;
  return CTRL_GOOD;
}

Ctrl_status sd_mmc_spi_read_capacity(uint32_t *nb_sector) {
  if (!IS_SD_INSERTED || IS_SD_PRINTING || IS_SD_FILE_OPEN || !card.cardOK)
    return CTRL_NO_PRESENT;
  *nb_sector = card.getSd2Card().cardSize();
  return CTRL_GOOD;
}

bool sd_mmc_spi_unload(bool unload) {
  return true;
}

bool sd_mmc_spi_wr_protect(void) {
  return false;
}

bool sd_mmc_spi_removal(void) {
  if (!IS_SD_INSERTED || IS_SD_PRINTING || IS_SD_FILE_OPEN || !card.cardOK)
    return true;
  return false;
}

#if ACCESS_USB == true
/**
 * \name MEM <-> USB Interface
 * @{
 */

#include "udi_msc.h"

COMPILER_WORD_ALIGNED
uint8_t sector_buf[SD_MMC_BLOCK_SIZE];

// #define DEBUG_MMC

Ctrl_status sd_mmc_spi_usb_read_10(uint32_t addr, uint16_t nb_sector) {
  if (!IS_SD_INSERTED || IS_SD_PRINTING || IS_SD_FILE_OPEN || !card.cardOK)
    return CTRL_NO_PRESENT;

  #ifdef DEBUG_MMC
    char buffer[80];
    sprintf(buffer, "SDRD: %d @ 0x%08x\n", nb_sector, addr);
    SERIAL_PROTOCOL_P(0, buffer);
  #endif

  // Start reading
  if (!card.getSd2Card().readStart(addr))
    return CTRL_FAIL;

  // For each specified sector
  while (nb_sector--) {

    // Read a sector
    card.getSd2Card().readData(sector_buf);

    // RAM -> USB
    if (!udi_msc_trans_block(true, sector_buf, SD_MMC_BLOCK_SIZE, NULL)) {
      card.getSd2Card().readStop();
      return CTRL_FAIL;
    }
  }

  // Stop reading
  card.getSd2Card().readStop();

  // Done
  return CTRL_GOOD;
}

Ctrl_status sd_mmc_spi_usb_write_10(uint32_t addr, uint16_t nb_sector) {
  if (!IS_SD_INSERTED || IS_SD_PRINTING || IS_SD_FILE_OPEN || !card.cardOK)
    return CTRL_NO_PRESENT;

  #ifdef DEBUG_MMC
    char buffer[80];
    sprintf(buffer, "SDWR: %d @ 0x%08x\n", nb_sector, addr);
    SERIAL_PROTOCOL_P(0, buffer);
  #endif

  if (!card.getSd2Card().writeStart(addr, nb_sector))
    return CTRL_FAIL;

  // For each specified sector
  while (nb_sector--) {

    // USB -> RAM
    if (!udi_msc_trans_block(false, sector_buf, SD_MMC_BLOCK_SIZE, NULL)) {
      card.getSd2Card().writeStop();
      return CTRL_FAIL;
    }

    // Write a sector
    card.getSd2Card().writeData(sector_buf);
  }

  // Stop writing
  card.getSd2Card().writeStop();

  // Done
  return CTRL_GOOD;
}

#endif // ACCESS_USB == true

#endif // SDSUPPORT
#endif // ARDUINO_ARCH_SAM
