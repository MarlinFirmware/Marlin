/*------------------------------------------------------------------------*/
/* LPCXpresso176x: MMCv3/SDv1/SDv2 (SPI mode) control module              */
/*------------------------------------------------------------------------*/
/*
/  Copyright (C) 2015, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

#include "lpc17xx_ssp.h"
#include "lpc17xx_clkpwr.h"
#include "LPC176x.h"

#define SSP_CH	1	/* SSP channel to use (0:SSP0, 1:SSP1) */

#define SCLK_FAST	25000000UL	/* SCLK frequency under normal operation [Hz] */
#define	SCLK_SLOW	400000UL	/* SCLK frequency under initialization [Hz] */

//#define	MMC_CD		(!(FIO2PIN1 & _BV(1)))	/* Card detect (yes:true, no:false, default:true) */
#define	MMC_WP		0						/* Write protected (yes:true, no:false, default:false) */

#if SSP_CH == 0
#define	SSPxDR		SSP0DR
#define	SSPxSR		SSP0SR
#define	SSPxCR0		SSP0CR0
#define	SSPxCR1		SSP0CR1
#define	SSPxCPSR	SSP0CPSR
#define	CS_LOW()	{FIO0CLR2 = _BV(0);}	/* Set P0.16 low */
#define	CS_HIGH()	{FIO0SET2 = _BV(0);}	/* Set P0.16 high */
#define PCSSPx		PCSSP0
#define	PCLKSSPx	PCLK_SSP0
#define ATTACH_SSP() {\
		__set_PINSEL(0, 15, 2);	/* SCK0 */\
		__set_PINSEL(0, 17, 2);	/* MISO0 */\
		__set_PINSEL(0, 18, 2);	/* MOSI0 */\
		FIO0DIR |= _BV(16);		/* CS# (P0.16) */\
		}
#elif SSP_CH == 1
#define	SSPxDR		SSP1DR
#define	SSPxSR		SSP1SR
#define	SSPxCR0		SSP1CR0
#define	SSPxCR1		SSP1CR1
#define	SSPxCPSR	SSP1CPSR
#define	CS_LOW()	{FIO0CLR0 = _BV(6);}	/* Set P0.6 low */
#define	CS_HIGH()	{FIO0SET0 = _BV(6);}	/* Set P0.6 high */
#define PCSSPx		PCSSP1
#define	PCLKSSPx	PCLK_SSP1
#define ATTACH_SSP() {\
		__set_PINSEL(0, 7, 2);	/* SCK1 */\
		__set_PINSEL(0, 8, 2);	/* MISO1 */\
		__set_PINSEL(0, 9, 2);	/* MOSI1 */\
		FIO0DIR |= _BV(6);		/* CS# (P0.6) */\
		}
#endif

#define PCLKDIV_SSP	PCLKDIV_2

static void set_spi_clock(uint32_t target_clock)
{
  uint32_t prescale, cr0_div, cmp_clk, ssp_clk;

  /* The SSP clock is derived from the (main system oscillator / 2),
      so compute the best divider from that clock */
  #if SSP_CH == 0
    ssp_clk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_SSP0);
  #elif SSP_CH == 1
    ssp_clk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_SSP1);
  #endif

	/* Find closest divider to get at or under the target frequency.
	   Use smallest prescale possible and rely on the divider to get
	   the closest target frequency */
	cr0_div = 0;
	cmp_clk = 0xFFFFFFFF;
	prescale = 2;
	while (cmp_clk > target_clock)
	{
		cmp_clk = ssp_clk / ((cr0_div + 1) * prescale);
		if (cmp_clk > target_clock)
		{
			cr0_div++;
			if (cr0_div > 0xFF)
			{
				cr0_div = 0;
				prescale += 2;
			}
		}
	}

  /* Write computed prescaler and divider back to register */
  SSPxCR0 &= (~SSP_CR0_SCR(0xFF)) & SSP_CR0_BITMASK;
  SSPxCR0 |= (SSP_CR0_SCR(cr0_div)) & SSP_CR0_BITMASK;
  SSPxCPSR = prescale & SSP_CPSR_BITMASK;
}


#define FCLK_FAST() set_spi_clock(SCLK_FAST)
#define FCLK_SLOW() set_spi_clock(SCLK_SLOW)



/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

#include "diskio.h"


/* MMC/SD command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define	CMD48	(48)		/* READ_EXTR_SINGLE */
#define	CMD49	(49)		/* WRITE_EXTR_SINGLE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */


static volatile
DSTATUS Stat = STA_NOINIT;	/* Physical drive status */

static volatile
UINT Timer1, Timer2;	/* 1kHz decrement timer stopped at zero (disk_timerproc()) */

static
BYTE CardType;			/* Card type flags */



/*-----------------------------------------------------------------------*/
/* Send/Receive data to the MMC  (Platform dependent)                    */
/*-----------------------------------------------------------------------*/

/* Exchange a byte */
static
BYTE xchg_spi (
	BYTE dat	/* Data to send */
)
{
	SSPxDR = dat;
	while (SSPxSR & 0x10) ;
	return SSPxDR;
}


/* Receive multiple byte */
static
void rcvr_spi_multi (
	BYTE *buff,		/* Pointer to data buffer */
	UINT btr		/* Number of bytes to receive (16, 64 or 512) */
)
{
	UINT n;
	WORD d;


	SSPxCR0 |= 0x000F;	/* Select 16-bit mode */

	for (n = 0; n < 8; n++)			/* Push 8 frames into pipeline  */
		SSPxDR = 0xFFFF;
	btr -= 16;

	while (btr >= 2) {				/* Receive the data block into buffer */
		btr -= 2;
		while (!(SSPxSR & _BV(2))) ;	/* Wait for any data in receive FIFO */
		d = SSPxDR;
		SSPxDR = 0xFFFF;
		*buff++ = d >> 8;
		*buff++ = d;
	}

	for (n = 0; n < 8; n++) {		/* Pop remaining frames from pipeline */
		while (!(SSPxSR & _BV(2))) ;
		d = SSPxDR;
		*buff++ = d >> 8;
		*buff++ = d;
	}

	SSPxCR0 &= 0xFFF7;				/* Select 8-bit mode */
}


#if _DISKIO_WRITE
/* Send multiple byte */
static
void xmit_spi_multi (
	const BYTE *buff,	/* Pointer to the data */
	UINT btx			/* Number of bytes to send (multiple of 16) */
)
{
	UINT n;
	WORD d;


	SSPxCR0 |= 0x000F;			/* Select 16-bit mode */

	for (n = 0; n < 8; n++) {	/* Push 8 frames into pipeline  */
		d = *buff++;
		d = d << 8 | *buff++;
		SSPxDR = d;
	}
	btx -= 16;

	while (btx >= 2) {			/* Transmit data block */
		btx -= 2;
		d = *buff++;
		d = d << 8 | *buff++;
		while (!(SSPxSR & _BV(2))) ;	/* Wait for any data in receive FIFO */
		SSPxDR; SSPxDR = d;
	}

	for (n = 0; n < 8; n++) {	/* Flush pipeline */
		while (!(SSPxSR & _BV(2))) ;
		SSPxDR;
	}

	SSPxCR0 &= 0xFFF7;			/* Select 8-bit mode */
}
#endif


/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
int wait_ready (	/* 1:Ready, 0:Timeout */
	UINT wt			/* Timeout [ms] */
)
{
	BYTE d;


	Timer2 = wt;
	do {
		d = xchg_spi(0xFF);

		/* This loop takes a time. Insert rot_rdq() here for multitask envilonment. */

	} while (d != 0xFF && Timer2);	/* Wait for card goes ready or timeout */

	return (d == 0xFF) ? 1 : 0;
}



/*-----------------------------------------------------------------------*/
/* Deselect card and release SPI                                         */
/*-----------------------------------------------------------------------*/

static
void deselect (void)
{
	CS_HIGH();		/* CS = H */
	xchg_spi(0xFF);	/* Dummy clock (force DO hi-z for multiple slave SPI) */
}



/*-----------------------------------------------------------------------*/
/* Select card and wait for ready                                        */
/*-----------------------------------------------------------------------*/

static
int select (void)	/* 1:OK, 0:Timeout */
{
	CS_LOW();		/* CS = L */
	xchg_spi(0xFF);	/* Dummy clock (force DO enabled) */

	if (wait_ready(500)) return 1;	/* Leading busy check: Wait for card ready */

	deselect();		/* Timeout */
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Control SPI module (Platform dependent)                               */
/*-----------------------------------------------------------------------*/

static
void power_on (void)	/* Enable SSP module and attach it to I/O pads */
{
	__set_PCONP(PCSSPx, 1);	/* Enable SSP module */
	__set_PCLKSEL(PCLKSSPx, PCLKDIV_SSP);	/* Select PCLK frequency for SSP */
	SSPxCR0 = 0x0007;		/* Set mode: SPI mode 0, 8-bit */
	SSPxCR1 = 0x2;			/* Enable SSP with Master */
	ATTACH_SSP();			/* Attach SSP module to I/O pads */
	CS_HIGH();				/* Set CS# high */

	for (Timer1 = 10; Timer1; ) ;	/* 10ms */
}


static
void power_off (void)		/* Disable SPI function */
{
	select();				/* Wait for card ready */
	deselect();
}



/*-----------------------------------------------------------------------*/
/* Receive a data packet from the MMC                                    */
/*-----------------------------------------------------------------------*/

static
int rcvr_datablock (	/* 1:OK, 0:Error */
	BYTE *buff,			/* Data buffer */
	UINT btr			/* Data block length (byte) */
)
{
	BYTE token;


	Timer1 = 200;
	do {							/* Wait for DataStart token in timeout of 200ms */
		token = xchg_spi(0xFF);

		/* This loop will take a time. Insert rot_rdq() here for multitask envilonment. */

	} while ((token == 0xFF) && Timer1);
	if(token != 0xFE) return 0;		/* Function fails if invalid DataStart token or timeout */

	rcvr_spi_multi(buff, btr);		/* Store trailing data to the buffer */
	xchg_spi(0xFF); xchg_spi(0xFF);	/* Discard CRC */

	return 1;						/* Function succeeded */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to the MMC                                         */
/*-----------------------------------------------------------------------*/

#if _DISKIO_WRITE
static
int xmit_datablock (	/* 1:OK, 0:Failed */
	const BYTE *buff,	/* Ponter to 512 byte data to be sent */
	BYTE token			/* Token */
)
{
	BYTE resp;


	if (!wait_ready(500)) return 0;		/* Leading busy check: Wait for card ready to accept data block */

	xchg_spi(token);					/* Send token */
	if (token == 0xFD) return 1;		/* Do not send data if token is StopTran */

	xmit_spi_multi(buff, 512);			/* Data */
	xchg_spi(0xFF); xchg_spi(0xFF);		/* Dummy CRC */

	resp = xchg_spi(0xFF);				/* Receive data resp */

	return (resp & 0x1F) == 0x05 ? 1 : 0;	/* Data was accepted or not */

	/* Busy check is done at next transmission */
}
#endif


/*-----------------------------------------------------------------------*/
/* Send a command packet to the MMC                                      */
/*-----------------------------------------------------------------------*/

static
BYTE send_cmd (		/* Return value: R1 resp (bit7==1:Failed to send) */
	BYTE cmd,		/* Command index */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;


	if (cmd & 0x80) {	/* Send a CMD55 prior to ACMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready except to stop multiple block read */
	if (cmd != CMD12) {
		deselect();
		if (!select()) return 0xFF;
	}

	/* Send command packet */
	xchg_spi(0x40 | cmd);				/* Start + command index */
	xchg_spi((BYTE)(arg >> 24));		/* Argument[31..24] */
	xchg_spi((BYTE)(arg >> 16));		/* Argument[23..16] */
	xchg_spi((BYTE)(arg >> 8));			/* Argument[15..8] */
	xchg_spi((BYTE)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	xchg_spi(n);

	/* Receive command resp */
	if (cmd == CMD12) xchg_spi(0xFF);	/* Diacard following one byte when CMD12 */
	n = 10;								/* Wait for response (10 bytes max) */
	do
		res = xchg_spi(0xFF);
	while ((res & 0x80) && --n);

	return res;							/* Return received response */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize disk drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive number (0) */
)
{
	BYTE n, cmd, ty, ocr[4];


	if (drv) return STA_NOINIT;			/* Supports only drive 0 */
	power_on();							/* Initialize SPI */

	if (Stat & STA_NODISK) return Stat;	/* Is a card existing in the soket? */

	FCLK_SLOW();
	for (n = 10; n; n--) xchg_spi(0xFF);	/* Send 80 dummy clocks */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {			/* Put the card SPI state */
		Timer1 = 1000;						/* Initialization timeout = 1 sec */
		if (send_cmd(CMD8, 0x1AA) == 1) {	/* Is the catd SDv2? */
			for (n = 0; n < 4; n++) ocr[n] = xchg_spi(0xFF);	/* Get 32 bit return value of R7 resp */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {				/* Does the card support 2.7-3.6V? */
				while (Timer1 && send_cmd(ACMD41, 1UL << 30)) ;	/* Wait for end of initialization with ACMD41(HCS) */
				if (Timer1 && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = xchg_spi(0xFF);
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* Check if the card is SDv2 */
				}
			}
		} else {	/* Not an SDv2 card */
			if (send_cmd(ACMD41, 0) <= 1) 	{	/* SDv1 or MMCv3? */
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 (ACMD41(0)) */
			} else {
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 (CMD1(0)) */
			}
			while (Timer1 && send_cmd(cmd, 0)) ;		/* Wait for the card leaves idle state */
			if (!Timer1 || send_cmd(CMD16, 512) != 0)	/* Set block length: 512 */
				ty = 0;
		}
	}
	CardType = ty;	/* Card type */
	deselect();

	if (ty) {		/* OK */
		FCLK_FAST();			/* Set fast clock */
		Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
	} else {		/* Failed */
		power_off();
		Stat = STA_NOINIT;
	}

	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get disk status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive number (0) */
)
{
	if (drv) return STA_NOINIT;		/* Supports only drive 0 */

	return Stat;	/* Return disk status */
}



/*-----------------------------------------------------------------------*/
/* Read sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,		/* Physical drive number (0) */
	BYTE *buff,		/* Pointer to the data buffer to store read data */
	DWORD sector,	/* Start sector number (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	BYTE cmd;


	if (drv || !count) return RES_PARERR;		/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */
	if (!(CardType & CT_BLOCK)) sector *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	cmd = count > 1 ? CMD18 : CMD17;			/*  READ_MULTIPLE_BLOCK : READ_SINGLE_BLOCK */
	if (send_cmd(cmd, sector) == 0) {
		do {
			if (!rcvr_datablock(buff, 512)) break;
			buff += 512;
		} while (--count);
		if (cmd == CMD18) send_cmd(CMD12, 0);	/* STOP_TRANSMISSION */
	}
	deselect();

	return count ? RES_ERROR : RES_OK;	/* Return result */
}



/*-----------------------------------------------------------------------*/
/* Write sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _DISKIO_WRITE
DRESULT disk_write (
	BYTE drv,			/* Physical drive number (0) */
	const BYTE *buff,	/* Ponter to the data to write */
	DWORD sector,		/* Start sector number (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	if (drv || !count) return RES_PARERR;		/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check drive status */
	if (Stat & STA_PROTECT) return RES_WRPRT;	/* Check write protect */

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* LBA ==> BA conversion (byte addressing cards) */

	if (count == 1) {	/* Single sector write */
		if ((send_cmd(CMD24, sector) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(buff, 0xFE)) {
			count = 0;
		}
	}
	else {				/* Multiple sector write */
		if (CardType & CT_SDC) send_cmd(ACMD23, count);	/* Predefine number of sectors */
		if (send_cmd(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
			do {
				if (!xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD)) count = 1;	/* STOP_TRAN token */
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;	/* Return result */
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous drive controls other than data read/write               */
/*-----------------------------------------------------------------------*/

#if _DISKIO_IOCTL

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive number (0) */
	BYTE cmd,		/* Control command code */
	void *buff		/* Pointer to the conrtol data */
)
{
	DRESULT res;
	BYTE n, csd[16], *ptr = buff;
	DWORD *dp, st, ed, csize;
#if _DISKIO_ISDIO
	SDIO_CMD *sdio = buff;
	BYTE rc, *buf;
	UINT dc;
#endif

	if (drv) return RES_PARERR;					/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	res = RES_ERROR;

	switch (cmd) {
	case CTRL_SYNC:			/* Wait for end of internal write process of the drive */
		if (select()) res = RES_OK;
		break;

	case GET_SECTOR_COUNT:	/* Get drive capacity in unit of sector (DWORD) */
		if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
			if ((csd[0] >> 6) == 1) {	/* SDC ver 2.00 */
				csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
				*(DWORD*)buff = csize << 10;
			} else {					/* SDC ver 1.XX or MMC ver 3 */
				n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
				csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
				*(DWORD*)buff = csize << (n - 9);
			}
			res = RES_OK;
		}
		break;

	case GET_BLOCK_SIZE:	/* Get erase block size in unit of sector (DWORD) */
		if (CardType & CT_SD2) {	/* SDC ver 2.00 */
			if (send_cmd(ACMD13, 0) == 0) {	/* Read SD status */
				xchg_spi(0xFF);
				if (rcvr_datablock(csd, 16)) {				/* Read partial block */
					for (n = 64 - 16; n; n--) xchg_spi(0xFF);	/* Purge trailing data */
					*(DWORD*)buff = 16UL << (csd[10] >> 4);
					res = RES_OK;
				}
			}
		} else {					/* SDC ver 1.XX or MMC */
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {	/* Read CSD */
				if (CardType & CT_SD1) {	/* SDC ver 1.XX */
					*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
				} else {					/* MMC */
					*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
				}
				res = RES_OK;
			}
		}
		break;

	case CTRL_TRIM:		/* Erase a block of sectors (used when _USE_TRIM in ffconf.h is 1) */
		if (!(CardType & CT_SDC)) break;				/* Check if the card is SDC */
		if (disk_ioctl(drv, MMC_GET_CSD, csd)) break;	/* Get CSD */
		if (!(csd[0] >> 6) && !(csd[10] & 0x40)) break;	/* Check if sector erase can be applied to the card */
		dp = buff; st = dp[0]; ed = dp[1];				/* Load sector block */
		if (!(CardType & CT_BLOCK)) {
			st *= 512; ed *= 512;
		}
		if (send_cmd(CMD32, st) == 0 && send_cmd(CMD33, ed) == 0 && send_cmd(CMD38, 0) == 0 && wait_ready(30000)) {	/* Erase sector block */
			res = RES_OK;	/* FatFs does not check result of this command */
		}
		break;

	/* Following commands are never used by FatFs module */

	case MMC_GET_TYPE:		/* Get MMC/SDC type (BYTE) */
		*ptr = CardType;
		res = RES_OK;
		break;

	case MMC_GET_CSD:		/* Read CSD (16 bytes) */
		if (send_cmd(CMD9, 0) == 0 && rcvr_datablock(ptr, 16)) {	/* READ_CSD */
			res = RES_OK;
		}
		break;

	case MMC_GET_CID:		/* Read CID (16 bytes) */
		if (send_cmd(CMD10, 0) == 0 && rcvr_datablock(ptr, 16)) {	/* READ_CID */
			res = RES_OK;
		}
		break;

	case MMC_GET_OCR:		/* Read OCR (4 bytes) */
		if (send_cmd(CMD58, 0) == 0) {	/* READ_OCR */
			for (n = 4; n; n--) *ptr++ = xchg_spi(0xFF);
			res = RES_OK;
		}
		break;

	case MMC_GET_SDSTAT:	/* Read SD status (64 bytes) */
		if (send_cmd(ACMD13, 0) == 0) {	/* SD_STATUS */
			xchg_spi(0xFF);
			if (rcvr_datablock(ptr, 64)) res = RES_OK;
		}
		break;
#if _DISKIO_ISDIO
	case ISDIO_READ:
		sdio = buff;
		if (send_cmd(CMD48, 0x80000000 | sdio->func << 28 | sdio->addr << 9 | ((sdio->ndata - 1) & 0x1FF)) == 0) {
			for (Timer1 = 1000; (rc = xchg_spi(0xFF)) == 0xFF && Timer1; ) ;
			if (rc == 0xFE) {
				for (buf = sdio->data, dc = sdio->ndata; dc; dc--) *buf++ = xchg_spi(0xFF);
				for (dc = 514 - sdio->ndata; dc; dc--) xchg_spi(0xFF);
				res = RES_OK;
			}
		}
		break;
	case ISDIO_WRITE:
		sdio = buff;
		if (send_cmd(CMD49, 0x80000000 | sdio->func << 28 | sdio->addr << 9 | ((sdio->ndata - 1) & 0x1FF)) == 0) {
			xchg_spi(0xFF); xchg_spi(0xFE);
			for (buf = sdio->data, dc = sdio->ndata; dc; dc--) xchg_spi(*buf++);
			for (dc = 514 - sdio->ndata; dc; dc--) xchg_spi(0xFF);
			if ((xchg_spi(0xFF) & 0x1F) == 0x05) res = RES_OK;
		}
		break;
	case ISDIO_MRITE:
		sdio = buff;
		if (send_cmd(CMD49, 0x84000000 | sdio->func << 28 | sdio->addr << 9 | sdio->ndata >> 8) == 0) {
			xchg_spi(0xFF); xchg_spi(0xFE);
			xchg_spi(sdio->ndata);
			for (dc = 513; dc; dc--) xchg_spi(0xFF);
			if ((xchg_spi(0xFF) & 0x1F) == 0x05) res = RES_OK;
		}
		break;
#endif
	default:
		res = RES_PARERR;
	}

	deselect();
	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Device timer function                                                 */
/*-----------------------------------------------------------------------*/
/* This function must be called from timer interrupt routine in period
/  of 1 ms to generate card control timing.
*/

void disk_timerproc (void)
{
	WORD n;
	BYTE s;


	n = Timer1;						/* 1kHz decrement timer stopped at 0 */
	if (n) Timer1 = --n;
	n = Timer2;
	if (n) Timer2 = --n;

	s = Stat;
	if (MMC_WP) {	/* Write protected */
		s |= STA_PROTECT;
	} else {		/* Write enabled */
		s &= ~STA_PROTECT;
	}
	//if (MMC_CD) {	/* Card is in socket */
		s &= ~STA_NODISK;
	//} else {		/* Socket empty */
	//	s |= (STA_NODISK | STA_NOINIT);
	//}
	Stat = s;
}

