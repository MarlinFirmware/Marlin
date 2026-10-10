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
 * Onboard SD card for Atmel SAM3U (AT91SAM3U4E) - HSMCI controller
 *
 * Named sdio.cpp to match the interface it implements and the other HALs
 * (HAL/STM32/sdio.cpp, HAL/GD32_MFL/sd/sdio.cpp). The peripheral it actually
 * drives is HSMCI; see below.
 *
 * The 4pi's microSD socket is wired to the High Speed Multimedia Card
 * Interface in 4-bit mode - MCCK on PA3, MCCDA on PA4 and MCDA0..3 on PA5..PA8
 * - not to SPI. Marlin's stock Sd2Card driver speaks SPI and cannot reach it,
 * so this implements the SDIO interface (Marlin/src/sd/Sd2Card_sdio.h) that
 * DiskIODriver_SDIO calls instead.
 *
 * ONBOARD_SDIO is Marlin's cross-platform flag for "the onboard card is on a
 * native SD host controller rather than SPI" - it is not a reference to the
 * SDIO standard for WiFi/GPS cards. STM32, GD32 and HC32 each implement the
 * same SDIO_* interface over their own controller; HSMCI is the SAM3U's, and
 * at the protocol level they are all just SD hosts.
 *
 * Written from the SAM3U datasheet (section 34, HSMCI) and the SD Physical
 * Layer spec, cross-checked against the MCI driver in the original 4pi
 * firmware (referance material/4pi-firmware/at91lib/peripherals/mci/).
 *
 * Programmed I/O, not DMA. A 512-byte block is 128 word transfers; RDPROOF and
 * WRPROOF let the card clock stall while the CPU keeps up, which removes the
 * overrun/underrun risk that would otherwise make PIO marginal. DMA would only
 * matter if this had to run concurrently with something else, and Marlin reads
 * the card from the foreground.
 */

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"

#if ENABLED(ONBOARD_SDIO)

#include "../../sd/Sd2Card_sdio.h"
#include "clock.h"

// ---------------------------------------------------------------------------
// Hardware configuration
// ---------------------------------------------------------------------------

#define SD_BLOCK_SIZE   512

// The card must be clocked at 400kHz or less until it is out of identification
// mode, then may run at its full default-speed rate of 25MHz.
//   HSMCI clock = MCK / (2 * (CLKDIV + 1))
#define HSMCI_CLKDIV_FOR(hz) ((F_CPU) / (2UL * (hz)) - 1UL)
#define HSMCI_INIT_CLKDIV    HSMCI_CLKDIV_FOR(400000UL)    // 119 -> 400kHz
#define HSMCI_FULL_CLKDIV    1UL                           // 96MHz / 4 = 24MHz

// Pins, all on PIOA peripheral A
#define HSMCI_PIN_MASK (_BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7) | _BV(8))

// Every response/data error the driver treats as fatal for a transfer
#define HSMCI_SR_ERRORS ( HSMCI_SR_RINDE | HSMCI_SR_RDIRE | HSMCI_SR_RCRCE \
                        | HSMCI_SR_RENDE | HSMCI_SR_RTOE  | HSMCI_SR_DCRCE \
                        | HSMCI_SR_DTOE  | HSMCI_SR_OVRE  | HSMCI_SR_UNRE )

// ---------------------------------------------------------------------------
// SD command set
// ---------------------------------------------------------------------------

#define CMD_GO_IDLE_STATE        0
#define CMD_ALL_SEND_CID         2
#define CMD_SEND_RELATIVE_ADDR   3
#define CMD_SET_BUS_WIDTH        6   // ACMD
#define CMD_SELECT_CARD          7
#define CMD_SEND_IF_COND         8
#define CMD_SEND_CSD             9
#define CMD_STOP_TRANSMISSION   12
#define CMD_SET_BLOCKLEN        16
#define CMD_READ_SINGLE_BLOCK   17
#define CMD_WRITE_BLOCK         24
#define CMD_SD_SEND_OP_COND     41  // ACMD
#define CMD_APP_CMD             55

// Response types, as HSMCI_CMDR_RSPTYP values
#define RSP_NONE  0
#define RSP_48    1
#define RSP_136   2
#define RSP_R1B   3

// ---------------------------------------------------------------------------
// State
// ---------------------------------------------------------------------------

static bool     card_ready = false;
static bool     card_is_hc = false;   // SDHC/SDXC: addresses are blocks, not bytes
static uint16_t card_rca = 0;         // Relative card address, in the high 16 bits of the arg
static uint32_t card_blocks = 0;      // Capacity in 512-byte blocks

// ---------------------------------------------------------------------------
// Low level
// ---------------------------------------------------------------------------

/**
 * Issue a command and wait for it to complete.
 *
 * Error flags in HSMCI_SR are cleared by reading the register, so the status
 * is accumulated across polls rather than tested on the final read - otherwise
 * an error that latched early would be lost by the time CMDRDY appeared.
 *
 * `ignore` masks off errors that are expected for a given response: R3 (the
 * OCR in ACMD41) carries no CRC, so the controller always reports RCRCE for it.
 */
static bool hsmci_cmd(const uint32_t cmdr, const uint32_t arg, const uint32_t ignore=0) {
  // CMDR is write-protected while CMDRDY is low
  uint32_t guard = 1000000;
  while (!(HSMCI->HSMCI_SR & HSMCI_SR_CMDRDY) && --guard) { }
  if (!guard) return false;

  HSMCI->HSMCI_ARGR = arg;
  HSMCI->HSMCI_CMDR = cmdr;

  uint32_t status = 0;
  guard = 1000000;
  do {
    status |= HSMCI->HSMCI_SR;
    if (!--guard) return false;
  } while (!(status & HSMCI_SR_CMDRDY));

  return !(status & (HSMCI_SR_ERRORS & ~ignore));
}

// Build a CMDR value for a command that moves no data
static inline uint32_t cmdr(const uint8_t index, const uint8_t rsptyp) {
  return HSMCI_CMDR_CMDNB(index) | HSMCI_CMDR_RSPTYP(rsptyp) | HSMCI_CMDR_MAXLAT;
}

// Read a 48-bit response (the single 32-bit payload word)
static inline uint32_t rsp48() { return HSMCI->HSMCI_RSPR[0]; }

/**
 * Read a 136-bit response. HSMCI_RSPR[0] is a read-pointer that advances on
 * each access, so the four words come from reading the same index repeatedly.
 */
static void rsp136(uint32_t out[4]) {
  for (uint8_t i = 0; i < 4; i++) out[i] = HSMCI->HSMCI_RSPR[0];
}

// An application command is CMD55 addressed to the card, then the ACMD itself
static bool hsmci_acmd(const uint8_t index, const uint8_t rsptyp,
                       const uint32_t arg, const uint32_t ignore=0) {
  if (!hsmci_cmd(cmdr(CMD_APP_CMD, RSP_48), uint32_t(card_rca) << 16)) return false;
  return hsmci_cmd(cmdr(index, rsptyp), arg, ignore);
}

// Set the card clock without disturbing the rest of the mode register
static void hsmci_set_clkdiv(const uint32_t clkdiv) {
  HSMCI->HSMCI_MR = (HSMCI->HSMCI_MR & ~uint32_t(HSMCI_MR_CLKDIV_Msk)) | HSMCI_MR_CLKDIV(clkdiv);
}

// ---------------------------------------------------------------------------
// Card capacity
// ---------------------------------------------------------------------------

/**
 * Decode the capacity, in 512-byte blocks, from a CSD register.
 *
 * csd[0] holds CSD[127:96], csd[1] CSD[95:64], and so on - HSMCI strips the
 * response header, so the words line up with the spec's bit numbering.
 */
static uint32_t csd_capacity_blocks(const uint32_t csd[4]) {
  const uint8_t csd_structure = (csd[0] >> 30) & 0x3;

  if (csd_structure == 1) {
    // CSD version 2.0 (SDHC/SDXC): C_SIZE is CSD[69:48], capacity is
    // (C_SIZE + 1) * 512KB, i.e. (C_SIZE + 1) * 1024 blocks.
    const uint32_t c_size = ((csd[1] & 0x3F) << 16) | (csd[2] >> 16);
    return (c_size + 1) << 10;
  }

  // CSD version 1.0: capacity is (C_SIZE + 1) * 2^(C_SIZE_MULT + 2) * 2^READ_BL_LEN
  const uint32_t read_bl_len = (csd[1] >> 16) & 0x0F,          // CSD[83:80]
                 c_size      = ((csd[1] & 0x3FF) << 2) | (csd[2] >> 30),  // CSD[73:62]
                 c_size_mult = (csd[2] >> 15) & 0x07;          // CSD[49:47]

  const int32_t shift = int32_t(c_size_mult) + 2 + int32_t(read_bl_len) - 9;
  return shift >= 0 ? (c_size + 1) << shift : (c_size + 1) >> -shift;
}

// ---------------------------------------------------------------------------
// Public interface (Marlin/src/sd/Sd2Card_sdio.h)
// ---------------------------------------------------------------------------

bool SDIO_Init() {
  card_ready = false;
  card_is_hc = false;
  card_rca = 0;
  card_blocks = 0;

  // Hand PA3..PA8 to the HSMCI (peripheral A) and enable their pull-ups: the
  // command and data lines idle high and the card only drives them low.
  pmc_enable_periph_clk(ID_PIOA);
  PIOA->PIO_IDR  = HSMCI_PIN_MASK;
  PIOA->PIO_PUER = HSMCI_PIN_MASK;
  PIOA->PIO_ABSR &= ~uint32_t(HSMCI_PIN_MASK);   // Peripheral A
  PIOA->PIO_PDR  = HSMCI_PIN_MASK;

  pmc_enable_periph_clk(ID_HSMCI);

  HSMCI->HSMCI_CR = HSMCI_CR_SWRST;
  HSMCI->HSMCI_CR = HSMCI_CR_MCIDIS | HSMCI_CR_PWSDIS;
  HSMCI->HSMCI_IDR = 0xFFFFFFFF;

  // Longest available data timeout - some cards are slow to finish a write
  HSMCI->HSMCI_DTOR = HSMCI_DTOR_DTOMUL(7) | HSMCI_DTOR_DTOCYC(15);

  /**
   * RDPROOF/WRPROOF stop the card clock whenever the FIFO would over- or
   * underrun, which is what makes programmed I/O safe here: the transfer waits
   * for the CPU instead of losing data.
   */
  HSMCI->HSMCI_MR = HSMCI_MR_CLKDIV(HSMCI_INIT_CLKDIV) | HSMCI_MR_PWSDIV(7)
                  | HSMCI_MR_RDPROOF | HSMCI_MR_WRPROOF;

  HSMCI->HSMCI_SDCR = HSMCI_SDCR_SDCSEL_SLOTA | HSMCI_SDCR_SDCBUS_1;
  HSMCI->HSMCI_CFG = HSMCI_CFG_FIFOMODE | HSMCI_CFG_FERRCTRL;
  HSMCI->HSMCI_DMA = 0;
  HSMCI->HSMCI_CR = HSMCI_CR_MCIEN;

  // 74 clocks with the command line high, per the SD spec's power-up sequence
  if (!hsmci_cmd(cmdr(CMD_GO_IDLE_STATE, RSP_NONE) | HSMCI_CMDR_SPCMD(1), 0)) return false;

  // CMD0: put the card in idle state
  if (!hsmci_cmd(cmdr(CMD_GO_IDLE_STATE, RSP_NONE), 0)) return false;

  /**
   * CMD8 tells a v2.00+ card the host's voltage range and asks it to echo the
   * check pattern back. A v1.x card simply does not respond, which is a
   * response timeout rather than a failure - so ignore RTOE and use the answer
   * only to decide whether to offer high capacity in ACMD41.
   */
  bool v2_card = false;
  if (hsmci_cmd(cmdr(CMD_SEND_IF_COND, RSP_48), 0x1AA, HSMCI_SR_RTOE))
    v2_card = ((rsp48() & 0xFFF) == 0x1AA);

  // ACMD41: wait for the card to finish its internal power-up. Offer HCS only
  // to a card that answered CMD8. R3 has no CRC, so RCRCE is expected.
  uint32_t ocr = 0;
  const uint32_t hcs = v2_card ? (1UL << 30) : 0;
  for (uint16_t tries = 2000; tries; tries--) {
    if (!hsmci_acmd(CMD_SD_SEND_OP_COND, RSP_48, 0x00100000 | hcs, HSMCI_SR_RCRCE)) return false;
    ocr = rsp48();
    if (ocr & (1UL << 31)) break;     // Power-up complete
    delay(1);
  }
  if (!(ocr & (1UL << 31))) return false;

  // CCS in the OCR distinguishes block-addressed high capacity cards
  card_is_hc = v2_card && (ocr & (1UL << 30));

  // CMD2 / CMD3: identify the card and get its relative address
  if (!hsmci_cmd(cmdr(CMD_ALL_SEND_CID, RSP_136), 0)) return false;
  if (!hsmci_cmd(cmdr(CMD_SEND_RELATIVE_ADDR, RSP_48), 0)) return false;
  card_rca = rsp48() >> 16;

  // CMD9: read the CSD for the capacity, which must happen before the card is
  // selected (SEND_CSD is only valid in stand-by state).
  if (!hsmci_cmd(cmdr(CMD_SEND_CSD, RSP_136), uint32_t(card_rca) << 16)) return false;
  uint32_t csd[4];
  rsp136(csd);
  card_blocks = csd_capacity_blocks(csd);

  // CMD7: select the card, moving it to transfer state
  if (!hsmci_cmd(cmdr(CMD_SELECT_CARD, RSP_R1B), uint32_t(card_rca) << 16)) return false;

  // ACMD6: switch the card to the 4-bit bus the board is wired for, then match
  // the controller to it. Order matters - the card must change first.
  if (!hsmci_acmd(CMD_SET_BUS_WIDTH, RSP_48, 2)) return false;
  HSMCI->HSMCI_SDCR = (HSMCI->HSMCI_SDCR & ~uint32_t(HSMCI_SDCR_SDCBUS_Msk)) | HSMCI_SDCR_SDCBUS_4;

  // Standard capacity cards need an explicit block length; SDHC ignores it
  if (!hsmci_cmd(cmdr(CMD_SET_BLOCKLEN, RSP_48), SD_BLOCK_SIZE)) return false;

  // Identification is over - run the bus at full speed
  hsmci_set_clkdiv(HSMCI_FULL_CLKDIV);

  card_ready = true;
  return true;
}

bool SDIO_IsReady() { return card_ready; }

uint32_t SDIO_GetCardSize() { return card_blocks; }

bool SDIO_ReadBlock(uint32_t block, uint8_t *dst) {
  if (!card_ready) return false;

  // Standard capacity cards are addressed by byte, high capacity by block
  const uint32_t addr = card_is_hc ? block : block * SD_BLOCK_SIZE;

  HSMCI->HSMCI_BLKR = HSMCI_BLKR_BLKLEN(SD_BLOCK_SIZE) | HSMCI_BLKR_BCNT(1);

  if (!hsmci_cmd(cmdr(CMD_READ_SINGLE_BLOCK, RSP_48)
                 | HSMCI_CMDR_TRCMD(1)          // Start data transfer
                 | HSMCI_CMDR_TRDIR             // Card to host
                 | HSMCI_CMDR_TRTYP(0),         // Single block
                 addr)) return false;

  uint32_t *p = (uint32_t *)dst;                // Callers hand us aligned buffers
  for (uint16_t i = 0; i < SD_BLOCK_SIZE / 4; i++) {
    uint32_t status, guard = 1000000;
    do {
      status = HSMCI->HSMCI_SR;
      if (status & HSMCI_SR_ERRORS) return false;
      if (!--guard) return false;
    } while (!(status & HSMCI_SR_RXRDY));
    p[i] = HSMCI->HSMCI_RDR;
  }

  uint32_t guard = 1000000;
  while (!(HSMCI->HSMCI_SR & HSMCI_SR_XFRDONE) && --guard) { }
  return guard != 0;
}

bool SDIO_WriteBlock(uint32_t block, const uint8_t *src) {
  if (!card_ready) return false;

  const uint32_t addr = card_is_hc ? block : block * SD_BLOCK_SIZE;

  HSMCI->HSMCI_BLKR = HSMCI_BLKR_BLKLEN(SD_BLOCK_SIZE) | HSMCI_BLKR_BCNT(1);

  if (!hsmci_cmd(cmdr(CMD_WRITE_BLOCK, RSP_48)
                 | HSMCI_CMDR_TRCMD(1)          // Start data transfer
                 | HSMCI_CMDR_TRTYP(0),         // Single block, host to card
                 addr)) return false;

  const uint32_t *p = (const uint32_t *)src;
  for (uint16_t i = 0; i < SD_BLOCK_SIZE / 4; i++) {
    uint32_t status, guard = 1000000;
    do {
      status = HSMCI->HSMCI_SR;
      if (status & HSMCI_SR_ERRORS) return false;
      if (!--guard) return false;
    } while (!(status & HSMCI_SR_TXRDY));
    HSMCI->HSMCI_TDR = p[i];
  }

  // The card holds DAT0 low while it programs the block; NOTBUSY covers that
  uint32_t guard = 1000000;
  while (!(HSMCI->HSMCI_SR & HSMCI_SR_XFRDONE) && --guard) { }
  if (!guard) return false;

  guard = 10000000;
  while (!(HSMCI->HSMCI_SR & HSMCI_SR_NOTBUSY) && --guard) { }
  return guard != 0;
}

#endif // ONBOARD_SDIO
#endif // __SAM3U4E__
