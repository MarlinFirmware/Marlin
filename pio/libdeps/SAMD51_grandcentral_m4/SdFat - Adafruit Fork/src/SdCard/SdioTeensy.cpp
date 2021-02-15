/**
 * Copyright (c) 2011-2018 Bill Greiman
 * This file is part of the SdFat library for SD memory cards.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
#include "SdioCard.h"
//==============================================================================
#define SDHC_PROCTL_DTW_4BIT 0x01
const uint32_t FIFO_WML = 16;
const uint32_t CMD8_RETRIES = 10;
const uint32_t BUSY_TIMEOUT_MICROS = 500000;
//==============================================================================
const uint32_t SDHC_IRQSTATEN_MASK =
               SDHC_IRQSTATEN_DMAESEN | SDHC_IRQSTATEN_AC12ESEN |
               SDHC_IRQSTATEN_DEBESEN | SDHC_IRQSTATEN_DCESEN |
               SDHC_IRQSTATEN_DTOESEN | SDHC_IRQSTATEN_CIESEN |
               SDHC_IRQSTATEN_CEBESEN | SDHC_IRQSTATEN_CCESEN |
               SDHC_IRQSTATEN_CTOESEN | SDHC_IRQSTATEN_DINTSEN |
               SDHC_IRQSTATEN_TCSEN | SDHC_IRQSTATEN_CCSEN;

const uint32_t SDHC_IRQSTAT_CMD_ERROR =
               SDHC_IRQSTAT_CIE | SDHC_IRQSTAT_CEBE |
               SDHC_IRQSTAT_CCE | SDHC_IRQSTAT_CTOE;

const uint32_t SDHC_IRQSTAT_DATA_ERROR =
               SDHC_IRQSTAT_AC12E | SDHC_IRQSTAT_DEBE |
               SDHC_IRQSTAT_DCE | SDHC_IRQSTAT_DTOE;

const uint32_t SDHC_IRQSTAT_ERROR =
               SDHC_IRQSTAT_DMAE | SDHC_IRQSTAT_CMD_ERROR |
               SDHC_IRQSTAT_DATA_ERROR;

const uint32_t SDHC_IRQSIGEN_MASK =
               SDHC_IRQSIGEN_DMAEIEN | SDHC_IRQSIGEN_AC12EIEN |
               SDHC_IRQSIGEN_DEBEIEN | SDHC_IRQSIGEN_DCEIEN |
               SDHC_IRQSIGEN_DTOEIEN | SDHC_IRQSIGEN_CIEIEN |
               SDHC_IRQSIGEN_CEBEIEN | SDHC_IRQSIGEN_CCEIEN |
               SDHC_IRQSIGEN_CTOEIEN | SDHC_IRQSIGEN_TCIEN;
//=============================================================================
const uint32_t CMD_RESP_NONE = SDHC_XFERTYP_RSPTYP(0);

const uint32_t CMD_RESP_R1 = SDHC_XFERTYP_CICEN | SDHC_XFERTYP_CCCEN |
                             SDHC_XFERTYP_RSPTYP(2);

const uint32_t CMD_RESP_R1b = SDHC_XFERTYP_CICEN | SDHC_XFERTYP_CCCEN |
                              SDHC_XFERTYP_RSPTYP(3);

const uint32_t CMD_RESP_R2 = SDHC_XFERTYP_CCCEN | SDHC_XFERTYP_RSPTYP(1);

const uint32_t CMD_RESP_R3 = SDHC_XFERTYP_RSPTYP(2);

const uint32_t CMD_RESP_R6 = CMD_RESP_R1;

const uint32_t CMD_RESP_R7 = CMD_RESP_R1;

const uint32_t DATA_READ = SDHC_XFERTYP_DTDSEL | SDHC_XFERTYP_DPSEL;

const uint32_t DATA_READ_DMA = DATA_READ | SDHC_XFERTYP_DMAEN;

const uint32_t DATA_READ_MULTI_DMA = DATA_READ_DMA | SDHC_XFERTYP_MSBSEL |
                                     SDHC_XFERTYP_AC12EN | SDHC_XFERTYP_BCEN;

const uint32_t DATA_READ_MULTI_PGM = DATA_READ | SDHC_XFERTYP_MSBSEL |
                                     SDHC_XFERTYP_BCEN | SDHC_XFERTYP_AC12EN;

const uint32_t DATA_WRITE_DMA = SDHC_XFERTYP_DPSEL | SDHC_XFERTYP_DMAEN;

const uint32_t DATA_WRITE_MULTI_DMA = DATA_WRITE_DMA | SDHC_XFERTYP_MSBSEL |
                                      SDHC_XFERTYP_AC12EN | SDHC_XFERTYP_BCEN;

const uint32_t DATA_WRITE_MULTI_PGM = SDHC_XFERTYP_DPSEL |
                                      SDHC_XFERTYP_MSBSEL |
                                      SDHC_XFERTYP_BCEN | SDHC_XFERTYP_AC12EN;

const uint32_t ACMD6_XFERTYP = SDHC_XFERTYP_CMDINX(ACMD6) | CMD_RESP_R1;

const uint32_t ACMD41_XFERTYP = SDHC_XFERTYP_CMDINX(ACMD41) | CMD_RESP_R3;

const uint32_t CMD0_XFERTYP = SDHC_XFERTYP_CMDINX(CMD0) | CMD_RESP_NONE;

const uint32_t CMD2_XFERTYP = SDHC_XFERTYP_CMDINX(CMD2) | CMD_RESP_R2;

const uint32_t CMD3_XFERTYP = SDHC_XFERTYP_CMDINX(CMD3) | CMD_RESP_R6;

const uint32_t CMD6_XFERTYP = SDHC_XFERTYP_CMDINX(CMD6) | CMD_RESP_R1 |
                              DATA_READ_DMA;

const uint32_t CMD7_XFERTYP = SDHC_XFERTYP_CMDINX(CMD7) | CMD_RESP_R1b;

const uint32_t CMD8_XFERTYP = SDHC_XFERTYP_CMDINX(CMD8) | CMD_RESP_R7;

const uint32_t CMD9_XFERTYP = SDHC_XFERTYP_CMDINX(CMD9) | CMD_RESP_R2;

const uint32_t CMD10_XFERTYP = SDHC_XFERTYP_CMDINX(CMD10) | CMD_RESP_R2;

const uint32_t CMD12_XFERTYP = SDHC_XFERTYP_CMDINX(CMD12) | CMD_RESP_R1b |
                               SDHC_XFERTYP_CMDTYP(3);

const uint32_t CMD13_XFERTYP = SDHC_XFERTYP_CMDINX(CMD13) | CMD_RESP_R1;

const uint32_t CMD17_DMA_XFERTYP = SDHC_XFERTYP_CMDINX(CMD17) | CMD_RESP_R1 |
                                   DATA_READ_DMA;

const uint32_t CMD18_DMA_XFERTYP = SDHC_XFERTYP_CMDINX(CMD18) | CMD_RESP_R1 |
                                   DATA_READ_MULTI_DMA;

const uint32_t CMD18_PGM_XFERTYP = SDHC_XFERTYP_CMDINX(CMD18) | CMD_RESP_R1 |
                                   DATA_READ_MULTI_PGM;

const uint32_t CMD24_DMA_XFERTYP = SDHC_XFERTYP_CMDINX(CMD24) | CMD_RESP_R1 |
                                   DATA_WRITE_DMA;

const uint32_t CMD25_DMA_XFERTYP = SDHC_XFERTYP_CMDINX(CMD25) | CMD_RESP_R1 |
                                   DATA_WRITE_MULTI_DMA;

const uint32_t CMD25_PGM_XFERTYP = SDHC_XFERTYP_CMDINX(CMD25) | CMD_RESP_R1 |
                                   DATA_WRITE_MULTI_PGM;

const uint32_t CMD32_XFERTYP = SDHC_XFERTYP_CMDINX(CMD32) | CMD_RESP_R1;

const uint32_t CMD33_XFERTYP = SDHC_XFERTYP_CMDINX(CMD33) | CMD_RESP_R1;

const uint32_t CMD38_XFERTYP = SDHC_XFERTYP_CMDINX(CMD38) | CMD_RESP_R1b;

const uint32_t CMD55_XFERTYP = SDHC_XFERTYP_CMDINX(CMD55) | CMD_RESP_R1;

//=============================================================================
static bool cardCommand(uint32_t xfertyp, uint32_t arg);
static void enableGPIO(bool enable);
static void enableDmaIrs();
static void initSDHC();
static bool isBusyCMD13();
static bool isBusyCommandComplete();
static bool isBusyCommandInhibit();
static bool readReg16(uint32_t xfertyp, void* data);
static void setSdclk(uint32_t kHzMax);
static bool yieldTimeout(bool (*fcn)());
static bool waitDmaStatus();
static bool waitTimeout(bool (*fcn)());
//-----------------------------------------------------------------------------
static bool (*m_busyFcn)() = 0;
static bool m_initDone = false;
static bool m_version2;
static bool m_highCapacity;
static uint8_t m_errorCode = SD_CARD_ERROR_INIT_NOT_CALLED;
static uint32_t m_errorLine = 0;
static uint32_t m_rca;
static volatile bool m_dmaBusy = false;
static volatile uint32_t m_irqstat;
static uint32_t m_sdClkKhz = 0;
static uint32_t m_ocr;
static cid_t m_cid;
static csd_t m_csd;
//=============================================================================
#define USE_DEBUG_MODE 0
#if USE_DEBUG_MODE
#define DBG_IRQSTAT() if (SDHC_IRQSTAT) {Serial.print(__LINE__);\
        Serial.print(" IRQSTAT "); Serial.println(SDHC_IRQSTAT, HEX);}

static void printRegs(uint32_t line) {
  Serial.print(line);
  Serial.print(" PRSSTAT ");
  Serial.print(SDHC_PRSSTAT, HEX);
  Serial.print(" PROCTL ");
  Serial.print(SDHC_PROCTL, HEX);
  Serial.print(" IRQSTAT ");
  Serial.print(SDHC_IRQSTAT, HEX);
  Serial.print(" m_irqstat ");
  Serial.println(m_irqstat, HEX);
}
#else  // USE_DEBUG_MODE
#define DBG_IRQSTAT()
#endif  // USE_DEBUG_MODE
//=============================================================================
// Error function and macro.
#define sdError(code) setSdErrorCode(code, __LINE__)
inline bool setSdErrorCode(uint8_t code, uint32_t line) {
  m_errorCode = code;
  m_errorLine = line;
  return false;  // setSdErrorCode
}
//=============================================================================
// ISR
void sdhc_isr() {
  SDHC_IRQSIGEN = 0;
  m_irqstat = SDHC_IRQSTAT;
  SDHC_IRQSTAT = m_irqstat;
  m_dmaBusy = false;
}
//=============================================================================
// Static functions.
static bool cardAcmd(uint32_t rca, uint32_t xfertyp, uint32_t arg) {
  return cardCommand(CMD55_XFERTYP, rca) && cardCommand (xfertyp, arg);
}
//-----------------------------------------------------------------------------
static bool cardCommand(uint32_t xfertyp, uint32_t arg) {
  DBG_IRQSTAT();
  if (waitTimeout(isBusyCommandInhibit)) {
    return false;  // Caller will set errorCode.
  }
  SDHC_CMDARG = arg;
  SDHC_XFERTYP = xfertyp;
  if (waitTimeout(isBusyCommandComplete)) {
    return false;  // Caller will set errorCode.
  }
  m_irqstat = SDHC_IRQSTAT;
  SDHC_IRQSTAT = m_irqstat;

  return (m_irqstat & SDHC_IRQSTAT_CC) &&
         !(m_irqstat & SDHC_IRQSTAT_CMD_ERROR);
}
//-----------------------------------------------------------------------------
static bool cardCMD6(uint32_t arg, uint8_t* status) {
  // CMD6 returns 64 bytes.
  if (waitTimeout(isBusyCMD13)) {
    return sdError(SD_CARD_ERROR_CMD13);
  }
  enableDmaIrs();
  SDHC_DSADDR  = (uint32_t)status;
  SDHC_CMDARG = arg;
  SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(1) | SDHC_BLKATTR_BLKSIZE(64);
  SDHC_IRQSIGEN = SDHC_IRQSIGEN_MASK;
  SDHC_XFERTYP = CMD6_XFERTYP;

  if (!waitDmaStatus()) {
    return sdError(SD_CARD_ERROR_CMD6);
  }
  return true;
}
//-----------------------------------------------------------------------------
static void enableGPIO(bool enable) {
  const uint32_t PORT_CLK = PORT_PCR_MUX(4) | PORT_PCR_DSE;
  const uint32_t PORT_CMD_DATA = PORT_CLK | PORT_PCR_PS | PORT_PCR_PE;

  PORTE_PCR0 = enable ? PORT_CMD_DATA : 0;  // SDHC_D1
  PORTE_PCR1 = enable ? PORT_CMD_DATA : 0;  // SDHC_D0
  PORTE_PCR2 = enable ? PORT_CLK : 0;       // SDHC_CLK
  PORTE_PCR3 = enable ? PORT_CMD_DATA : 0;  // SDHC_CMD
  PORTE_PCR4 = enable ? PORT_CMD_DATA : 0;  // SDHC_D3
  PORTE_PCR5 = enable ? PORT_CMD_DATA : 0;  // SDHC_D2
}
//-----------------------------------------------------------------------------
static void enableDmaIrs() {
  m_dmaBusy = true;
  m_irqstat = 0;
}
//-----------------------------------------------------------------------------
static void initSDHC() {
#ifdef HAS_KINETIS_MPU
  // Allow SDHC Bus Master access.
  MPU_RGDAAC0 |= 0x0C000000;
#endif
  // Enable SDHC clock.
  SIM_SCGC3 |= SIM_SCGC3_SDHC;

  // Disable GPIO clock.
  enableGPIO(false);

  // Reset SDHC. Use default Water Mark Level of 16.
  SDHC_SYSCTL = SDHC_SYSCTL_RSTA;
  while (SDHC_SYSCTL & SDHC_SYSCTL_RSTA) {
  }
  // Set initial SCK rate.
  setSdclk(400);

  enableGPIO(true);

  // Enable desired IRQSTAT bits.
  SDHC_IRQSTATEN = SDHC_IRQSTATEN_MASK;

  NVIC_SET_PRIORITY(IRQ_SDHC, 6*16);
  NVIC_ENABLE_IRQ(IRQ_SDHC);

  // Send 80 clocks to card.
  SDHC_SYSCTL |= SDHC_SYSCTL_INITA;
  while (SDHC_SYSCTL & SDHC_SYSCTL_INITA) {
  }
}
//-----------------------------------------------------------------------------
static bool isBusyCMD13() {
  if (!cardCommand(CMD13_XFERTYP, m_rca)) {
    // Caller will timeout.
    return true;
  }
  return !(SDHC_CMDRSP0 & CARD_STATUS_READY_FOR_DATA);
}
//-----------------------------------------------------------------------------
static bool isBusyCommandComplete() {
  return !(SDHC_IRQSTAT &(SDHC_IRQSTAT_CC | SDHC_IRQSTAT_CMD_ERROR));
}
//-----------------------------------------------------------------------------
static bool isBusyCommandInhibit() {
  return SDHC_PRSSTAT & SDHC_PRSSTAT_CIHB;
}
//-----------------------------------------------------------------------------
static bool isBusyDMA() {
  return m_dmaBusy;
}
//-----------------------------------------------------------------------------
static bool isBusyFifoRead() {
  return !(SDHC_PRSSTAT & SDHC_PRSSTAT_BREN);
}
//-----------------------------------------------------------------------------
static bool isBusyFifoWrite() {
  return !(SDHC_PRSSTAT & SDHC_PRSSTAT_BWEN);
}
//-----------------------------------------------------------------------------
static bool isBusyTransferComplete() {
  return !(SDHC_IRQSTAT & (SDHC_IRQSTAT_TC | SDHC_IRQSTAT_ERROR));
}
//-----------------------------------------------------------------------------
static bool rdWrBlocks(uint32_t xfertyp,
                       uint32_t lba, uint8_t* buf, size_t n) {
  if ((3 & (uint32_t)buf) || n == 0) {
    return sdError(SD_CARD_ERROR_DMA);
  }
  if (yieldTimeout(isBusyCMD13)) {
    return sdError(SD_CARD_ERROR_CMD13);
  }
  enableDmaIrs();
  SDHC_DSADDR  = (uint32_t)buf;
  SDHC_CMDARG = m_highCapacity ? lba : 512*lba;
  SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(n) | SDHC_BLKATTR_BLKSIZE(512);
  SDHC_IRQSIGEN = SDHC_IRQSIGEN_MASK;
  SDHC_XFERTYP = xfertyp;

  return waitDmaStatus();
}
//-----------------------------------------------------------------------------
// Read 16 byte CID or CSD register.
static bool readReg16(uint32_t xfertyp, void* data) {
  uint8_t* d = reinterpret_cast<uint8_t*>(data);
  if (!cardCommand(xfertyp, m_rca)) {
    return false;  // Caller will set errorCode.
  }
  uint32_t sr[] = {SDHC_CMDRSP0, SDHC_CMDRSP1, SDHC_CMDRSP2, SDHC_CMDRSP3};
  for (int i = 0; i < 15; i++) {
    d[14 - i] = sr[i/4] >> 8*(i%4);
  }
  d[15] = 0;
  return true;
}
//-----------------------------------------------------------------------------
static void setSdclk(uint32_t kHzMax) {
  const uint32_t DVS_LIMIT = 0X10;
  const uint32_t SDCLKFS_LIMIT = 0X100;
  uint32_t dvs = 1;
  uint32_t sdclkfs = 1;
  uint32_t maxSdclk = 1000*kHzMax;

  while ((F_CPU/(sdclkfs*DVS_LIMIT) > maxSdclk) && (sdclkfs < SDCLKFS_LIMIT)) {
    sdclkfs <<= 1;
  }
  while ((F_CPU/(sdclkfs*dvs) > maxSdclk) && (dvs < DVS_LIMIT)) {
    dvs++;
  }
  m_sdClkKhz = F_CPU/(1000*sdclkfs*dvs);
  sdclkfs >>= 1;
  dvs--;

  // Disable SDHC clock.
  SDHC_SYSCTL &= ~SDHC_SYSCTL_SDCLKEN;

  // Change dividers.
  uint32_t sysctl = SDHC_SYSCTL & ~(SDHC_SYSCTL_DTOCV_MASK
                    | SDHC_SYSCTL_DVS_MASK | SDHC_SYSCTL_SDCLKFS_MASK);

  SDHC_SYSCTL = sysctl | SDHC_SYSCTL_DTOCV(0x0E) | SDHC_SYSCTL_DVS(dvs)
                | SDHC_SYSCTL_SDCLKFS(sdclkfs);

  // Wait until the SDHC clock is stable.
  while (!(SDHC_PRSSTAT & SDHC_PRSSTAT_SDSTB)) {
  }
  // Enable the SDHC clock.
  SDHC_SYSCTL |= SDHC_SYSCTL_SDCLKEN;
}
//-----------------------------------------------------------------------------
static bool transferStop() {
  DBG_IRQSTAT();

  if (!cardCommand(CMD12_XFERTYP, 0)) {
    return sdError(SD_CARD_ERROR_CMD12);
  }
  if (yieldTimeout(isBusyCMD13)) {
    return sdError(SD_CARD_ERROR_CMD13);
  }
  // Save registers before reset DAT lines.
  uint32_t irqsststen = SDHC_IRQSTATEN;
  uint32_t proctl = SDHC_PROCTL & ~SDHC_PROCTL_SABGREQ;

  // Do reset to clear CDIHB.  Should be a better way!
  SDHC_SYSCTL |= SDHC_SYSCTL_RSTD;

  // Restore registers.
  SDHC_IRQSTATEN = irqsststen;
  SDHC_PROCTL = proctl;

  return true;
}
//-----------------------------------------------------------------------------
// Return true if timeout occurs.
static bool yieldTimeout(bool (*fcn)()) {
  m_busyFcn = fcn;
  uint32_t m = micros();
  while (fcn()) {
    if ((micros() - m) > BUSY_TIMEOUT_MICROS) {
      m_busyFcn = 0;
      return true;
    }
    yield();
  }
  m_busyFcn = 0;
  return false;  // Caller will set errorCode.
}
//-----------------------------------------------------------------------------
static bool waitDmaStatus() {
  if (yieldTimeout(isBusyDMA)) {
    return false;  // Caller will set errorCode.
  }
  return (m_irqstat & SDHC_IRQSTAT_TC) && !(m_irqstat & SDHC_IRQSTAT_ERROR);
}
//-----------------------------------------------------------------------------
// Return true if timeout occurs.
static bool waitTimeout(bool (*fcn)()) {
  uint32_t m = micros();
  while (fcn()) {
    if ((micros() - m) > BUSY_TIMEOUT_MICROS) {
      return true;
    }
  }
  return false;  // Caller will set errorCode.
}
//=============================================================================
bool SdioCard::begin() {
  uint32_t kHzSdClk;
  uint32_t arg;
  m_initDone = false;
  m_errorCode = SD_CARD_ERROR_NONE;
  m_highCapacity = false;
  m_version2 = false;

  // initialize controller.
  initSDHC();

  if (!cardCommand(CMD0_XFERTYP, 0)) {
    return sdError(SD_CARD_ERROR_CMD0);
  }
  // Try several times for case of reset delay.
  for (uint32_t i = 0; i < CMD8_RETRIES; i++) {
    if (cardCommand(CMD8_XFERTYP, 0X1AA)) {
      if (SDHC_CMDRSP0 != 0X1AA) {
        return sdError(SD_CARD_ERROR_CMD8);
      }
      m_version2 = true;
      break;
    }
  }
  arg = m_version2 ? 0X40300000 : 0x00300000;
  uint32_t m = micros();
  do {
    if (!cardAcmd(0, ACMD41_XFERTYP, arg) ||
       ((micros() - m) > BUSY_TIMEOUT_MICROS)) {
      return sdError(SD_CARD_ERROR_ACMD41);
    }
  } while ((SDHC_CMDRSP0 & 0x80000000) == 0);

  m_ocr = SDHC_CMDRSP0;
  if (SDHC_CMDRSP0 & 0x40000000) {
    // Is high capacity.
    m_highCapacity = true;
  }
  if (!cardCommand(CMD2_XFERTYP, 0)) {
    return sdError(SD_CARD_ERROR_CMD2);
  }
  if (!cardCommand(CMD3_XFERTYP, 0)) {
    return sdError(SD_CARD_ERROR_CMD3);
  }
  m_rca = SDHC_CMDRSP0 & 0xFFFF0000;

  if (!readReg16(CMD9_XFERTYP, &m_csd)) {
    return sdError(SD_CARD_ERROR_CMD9);
  }
  if (!readReg16(CMD10_XFERTYP, &m_cid)) {
    return sdError(SD_CARD_ERROR_CMD10);
  }
  if (!cardCommand(CMD7_XFERTYP, m_rca)) {
    return sdError(SD_CARD_ERROR_CMD7);
  }
  // Set card to bus width four.
  if (!cardAcmd(m_rca, ACMD6_XFERTYP, 2)) {
    return sdError(SD_CARD_ERROR_ACMD6);
  }
  // Set SDHC to bus width four.
  SDHC_PROCTL &= ~SDHC_PROCTL_DTW_MASK;
  SDHC_PROCTL |= SDHC_PROCTL_DTW(SDHC_PROCTL_DTW_4BIT);

  SDHC_WML = SDHC_WML_RDWML(FIFO_WML) | SDHC_WML_WRWML(FIFO_WML);

  // Determine if High Speed mode is supported and set frequency.
  uint8_t status[64];
  if (cardCMD6(0X00FFFFFF, status) && (2 & status[13]) &&
      cardCMD6(0X80FFFFF1, status) && (status[16] & 0XF) == 1) {
    kHzSdClk = 50000;
  } else {
    kHzSdClk = 25000;
  }
  // disable GPIO
  enableGPIO(false);

  // Set the SDHC SCK frequency.
  setSdclk(kHzSdClk);

  // enable GPIO
  enableGPIO(true);
  m_initDone = true;
  return true;
}
//-----------------------------------------------------------------------------
uint32_t SdioCard::cardCapacity() {
  return sdCardCapacity(&m_csd);
}
//-----------------------------------------------------------------------------
bool SdioCard::erase(uint32_t firstBlock, uint32_t lastBlock) {
  // check for single block erase
  if (!m_csd.v1.erase_blk_en) {
    // erase size mask
    uint8_t m = (m_csd.v1.sector_size_high << 1) | m_csd.v1.sector_size_low;
    if ((firstBlock & m) != 0 || ((lastBlock + 1) & m) != 0) {
      // error card can't erase specified area
      return sdError(SD_CARD_ERROR_ERASE_SINGLE_BLOCK);
    }
  }
  if (!m_highCapacity) {
    firstBlock <<= 9;
    lastBlock <<= 9;
  }
  if (!cardCommand(CMD32_XFERTYP, firstBlock)) {
    return sdError(SD_CARD_ERROR_CMD32);
  }
  if (!cardCommand(CMD33_XFERTYP, lastBlock)) {
     return sdError(SD_CARD_ERROR_CMD33);
  }
  if (!cardCommand(CMD38_XFERTYP, 0)) {
    return sdError(SD_CARD_ERROR_CMD38);
  }
  if (waitTimeout(isBusyCMD13)) {
    return sdError(SD_CARD_ERROR_ERASE_TIMEOUT);
  }
  return true;
}
//-----------------------------------------------------------------------------
uint8_t SdioCard::errorCode() {
  return m_errorCode;
}
//-----------------------------------------------------------------------------
uint32_t SdioCard::errorData() {
  return m_irqstat;
}
//-----------------------------------------------------------------------------
uint32_t SdioCard::errorLine() {
  return m_errorLine;
}
//-----------------------------------------------------------------------------
bool SdioCard::isBusy() {
  return m_busyFcn ? m_busyFcn() : m_initDone && isBusyCMD13();
}
//-----------------------------------------------------------------------------
uint32_t SdioCard::kHzSdClk() {
  return m_sdClkKhz;
}
//-----------------------------------------------------------------------------
bool SdioCard::readBlock(uint32_t lba, uint8_t* buf) {
  uint8_t aligned[512];

  uint8_t* ptr = (uint32_t)buf & 3 ? aligned : buf;

  if (!rdWrBlocks(CMD17_DMA_XFERTYP, lba, ptr, 1)) {
    return sdError(SD_CARD_ERROR_CMD18);
  }
  if (ptr != buf) {
    memcpy(buf, aligned, 512);
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCard::readBlocks(uint32_t lba, uint8_t* buf, size_t n) {
  if ((uint32_t)buf & 3) {
    for (size_t i = 0; i < n; i++, lba++, buf += 512) {
      if (!readBlock(lba, buf)) {
        return false;  // readBlock will set errorCode.
      }
    }
    return true;
  }
  if (!rdWrBlocks(CMD18_DMA_XFERTYP, lba, buf, n)) {
    return sdError(SD_CARD_ERROR_CMD18);
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCard::readCID(void* cid) {
  memcpy(cid, &m_cid, 16);
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCard::readCSD(void* csd) {
  memcpy(csd, &m_csd, 16);
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCard::readData(uint8_t *dst) {
  DBG_IRQSTAT();
  uint32_t *p32 = reinterpret_cast<uint32_t*>(dst);

  if (!(SDHC_PRSSTAT & SDHC_PRSSTAT_RTA)) {
    SDHC_PROCTL &= ~SDHC_PROCTL_SABGREQ;
    if ((SDHC_BLKATTR & 0XFFFF0000) == 0X10000) {
    // Don't stop at block gap if last block.  Allows auto CMD12.
      SDHC_PROCTL |= SDHC_PROCTL_CREQ;
    } else {
      noInterrupts();
      SDHC_PROCTL |= SDHC_PROCTL_CREQ;
      SDHC_PROCTL |= SDHC_PROCTL_SABGREQ;
      interrupts();
    }
  }
  if (waitTimeout(isBusyFifoRead)) {
    return sdError(SD_CARD_ERROR_READ_FIFO);
  }
  for (uint32_t iw = 0 ; iw < 512/(4*FIFO_WML); iw++) {
    while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BREN)) {
    }
    for (uint32_t i = 0; i < FIFO_WML; i++) {
      p32[i] = SDHC_DATPORT;
    }
    p32 += FIFO_WML;
  }
  if (waitTimeout(isBusyTransferComplete)) {
    return sdError(SD_CARD_ERROR_READ_TIMEOUT);
  }
  m_irqstat = SDHC_IRQSTAT;
  SDHC_IRQSTAT = m_irqstat;
  return (m_irqstat & SDHC_IRQSTAT_TC) && !(m_irqstat & SDHC_IRQSTAT_ERROR);
}
//-----------------------------------------------------------------------------
bool SdioCard::readOCR(uint32_t* ocr) {
  *ocr = m_ocr;
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCard::readStart(uint32_t lba) {
  // K66/K65 Errata - SDHC: Does not support Infinite Block Transfer Mode.
  return sdError(SD_CARD_ERROR_FUNCTION_NOT_SUPPORTED);
}
//-----------------------------------------------------------------------------
// SDHC will do Auto CMD12 after count blocks.
bool SdioCard::readStart(uint32_t lba, uint32_t count) {
  DBG_IRQSTAT();
  if (count > 0XFFFF) {
    return sdError(SD_CARD_ERROR_READ_START);
  }
  if (yieldTimeout(isBusyCMD13)) {
    return sdError(SD_CARD_ERROR_CMD13);
  }
  if (count > 1) {
    SDHC_PROCTL |= SDHC_PROCTL_SABGREQ;
  }
  SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(count) | SDHC_BLKATTR_BLKSIZE(512);
  if (!cardCommand(CMD18_PGM_XFERTYP, m_highCapacity ? lba : 512*lba)) {
    return sdError(SD_CARD_ERROR_CMD18);
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCard::readStop() {
  return transferStop();
}
//-----------------------------------------------------------------------------
bool SdioCard::syncBlocks() {
  return true;
}
//-----------------------------------------------------------------------------
uint8_t SdioCard::type() {
  return  m_version2 ? m_highCapacity ?
          SD_CARD_TYPE_SDHC : SD_CARD_TYPE_SD2 : SD_CARD_TYPE_SD1;
}
//-----------------------------------------------------------------------------
bool SdioCard::writeBlock(uint32_t lba, const uint8_t* buf) {
  uint8_t *ptr;
  uint8_t aligned[512];
  if (3 & (uint32_t)buf) {
    ptr = aligned;
    memcpy(aligned, buf, 512);
  } else {
    ptr = const_cast<uint8_t*>(buf);
  }
  if (!rdWrBlocks(CMD24_DMA_XFERTYP, lba, ptr, 1)) {
    return sdError(SD_CARD_ERROR_CMD24);
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCard::writeBlocks(uint32_t lba, const uint8_t* buf, size_t n) {
  uint8_t* ptr = const_cast<uint8_t*>(buf);
  if (3 & (uint32_t)ptr) {
    for (size_t i = 0; i < n; i++, lba++, ptr += 512) {
      if (!writeBlock(lba, ptr)) {
        return false;  // writeBlock will set errorCode.
      }
    }
    return true;
  }
  if (!rdWrBlocks(CMD25_DMA_XFERTYP, lba, ptr, n)) {
    return sdError(SD_CARD_ERROR_CMD25);
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCard::writeData(const uint8_t* src) {
  DBG_IRQSTAT();
  const uint32_t* p32 = reinterpret_cast<const uint32_t*>(src);

  if (!(SDHC_PRSSTAT & SDHC_PRSSTAT_WTA)) {
    SDHC_PROCTL &= ~SDHC_PROCTL_SABGREQ;
    // Don't stop at block gap if last block.  Allows auto CMD12.
    if ((SDHC_BLKATTR & 0XFFFF0000) == 0X10000) {
      SDHC_PROCTL |= SDHC_PROCTL_CREQ;
    } else {
      SDHC_PROCTL |= SDHC_PROCTL_CREQ;
      SDHC_PROCTL |= SDHC_PROCTL_SABGREQ;
    }
  }
  if (waitTimeout(isBusyFifoWrite)) {
    return sdError(SD_CARD_ERROR_WRITE_FIFO);
  }
  for (uint32_t iw = 0 ; iw < 512/(4*FIFO_WML); iw++) {
    while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BWEN)) {
    }
    for (uint32_t i = 0; i < FIFO_WML; i++) {
      SDHC_DATPORT = p32[i];
    }
    p32 += FIFO_WML;
  }
  if (waitTimeout(isBusyTransferComplete)) {
    return sdError(SD_CARD_ERROR_WRITE_TIMEOUT);
  }
  m_irqstat = SDHC_IRQSTAT;
  SDHC_IRQSTAT = m_irqstat;
  return (m_irqstat & SDHC_IRQSTAT_TC) && !(m_irqstat & SDHC_IRQSTAT_ERROR);
}
//-----------------------------------------------------------------------------
bool SdioCard::writeStart(uint32_t lba) {
  // K66/K65 Errata - SDHC: Does not support Infinite Block Transfer Mode.
  return sdError(SD_CARD_ERROR_FUNCTION_NOT_SUPPORTED);
}
//-----------------------------------------------------------------------------
// SDHC will do Auto CMD12 after count blocks.
bool SdioCard::writeStart(uint32_t lba, uint32_t count) {
  if (count > 0XFFFF) {
    return sdError(SD_CARD_ERROR_WRITE_START);
  }
  DBG_IRQSTAT();
  if (yieldTimeout(isBusyCMD13)) {
    return sdError(SD_CARD_ERROR_CMD13);
  }
  if (count > 1) {
    SDHC_PROCTL |= SDHC_PROCTL_SABGREQ;
  }
  SDHC_BLKATTR = SDHC_BLKATTR_BLKCNT(count) | SDHC_BLKATTR_BLKSIZE(512);

  if (!cardCommand(CMD25_PGM_XFERTYP, m_highCapacity ? lba : 512*lba)) {
    return sdError(SD_CARD_ERROR_CMD25);
  }
  return true;
}
//-----------------------------------------------------------------------------
bool SdioCard::writeStop() {
  return transferStop();
}
#endif  // defined(__MK64FX512__) || defined(__MK66FX1M0__)
