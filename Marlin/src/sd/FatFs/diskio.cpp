/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "../../inc/MarlinConfig.h"

// Create Sd2Card class def according to available sys libs
#if ENABLED(USB_FLASH_DRIVE_SUPPORT)
  #include "../usb_flashdrive/Sd2Card_FlashDrive.h"
#elif ENABLED(SDIO_SUPPORT)
  #include "../Sd2Card_sdio.h"
#else
  #include "../Sd2Card.h"
#endif

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


Sd2Card sd2card;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		result = false; // XXX RAM_disk_status();

		// translate the reslut code here
    stat = RES_NOTRDY;
		return stat;

	case DEV_MMC :
		result = true; // XXX MMC_disk_status();

		// translate the reslut code here

    stat = RES_OK; // XXX
		return stat;

	case DEV_USB :
		result = false; // XXX USB_disk_status();

		// translate the reslut code here

    stat = RES_NOTRDY;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		result = false; // XXX RAM_disk_initialize();

		// translate the reslut code here
		stat = RES_NOTRDY;
		return stat;

	case DEV_MMC :
		result = true;

		if (!sd2card.init(SPI_SPEED, SDSS)
			#if defined(LCD_SDSS) && (LCD_SDSS != SDSS)
				&& !sd2card.init(SPI_SPEED, LCD_SDSS)
			#endif
		) {
			SERIAL_ECHO_MSG(MSG_SD_INIT_FAIL);
			result = false;
		}

		return (result) ? RES_OK : RES_NOTRDY;

	case DEV_USB :
		result = false; // XXX USB_disk_initialize();

		// translate the reslut code here
		stat = RES_NOTRDY;
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		result = false; // RAM_disk_read(buff, sector, count);

    res = (result) ? RES_OK : RES_ERROR;
		return res;

	case DEV_MMC :
		result = true; // MMC_disk_read(buff, sector, count);
    sd2card.readStart(sector);
		UINT i = 0;
		while (i++ < count) {
			if (!(result = sd2card.readData(buff + i * FF_MIN_SS) )) break;
		}
    sd2card.readStop();

    // TODO: use int sd2card.errorCode()
    res = (result) ? RES_OK : RES_ERROR;
		return res;

	case DEV_USB :
		// translate the arguments here

		result = false; // USB_disk_read(buff, sector, count);

		// translate the reslut code here
    res = (result) ? RES_OK : RES_ERROR;
		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		result = false; // RAM_disk_write(buff, sector, count);

    // TODO: use int sd2card.errorCode()
    res = (result) ? RES_OK : RES_ERROR;
		return res;

	case DEV_MMC :
    result = true; // MMC_disk_read(buff, sector, count);

    sd2card.writeStart(sector, count);
		UINT i = 0;
		while (i++ < count) {
			if (!(result = sd2card.writeData(buff + i * FF_MIN_SS) )) break;
		}
    sd2card.writeStop();

    res = (result) ? RES_OK : RES_ERROR;
    return res;

	case DEV_USB :
		result = false; // USB_disk_write(buff, sector, count);

    // TODO: use int sd2card.errorCode()
    res = (result) ? RES_OK : RES_ERROR;
		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :

  if (pdrv) return RES_PARERR;

	res = RES_ERROR;

	switch (cmd) {
	case CTRL_SYNC :		/* Make sure that no pending write process */
    res = (sd2card.waitNotBusy(SD_WRITE_TIMEOUT)) ? RES_OK : RES_NOTRDY;
		break;

	case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
  		*(DWORD*)buff = sd2card.cardSize();
  		res = RES_OK;
  		break;

  	case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
  		*(WORD*)buff = FF_MIN_SS;
  		res = RES_OK;
  		break;

  	case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
  		*(DWORD*)buff = 1;
  		res = RES_OK;
  		break;

  	case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
  		*(BYTE*)buff = CardType;
  		res = RES_OK;
  		break;

  	case MMC_GET_CSD :		/* Receive CSD as a data block (16 bytes) */
  		for (n=0;n<16;n++)
  			*(ptr+n) = CardConfig.csd[n];
  		res = RES_OK;
  		break;

  	case MMC_GET_CID :		/* Receive CID as a data block (16 bytes) */
  		for (n=0;n<16;n++)
  			*(ptr+n) = CardConfig.cid[n];
  		res = RES_OK;
  		break;

  	case MMC_GET_OCR :		/* Receive OCR as an R3 resp (4 bytes) */
  		for (n=0;n<4;n++)
  			*(ptr+n) = CardConfig.ocr[n];
  		res = RES_OK;
  		break;

  	case MMC_GET_SDSTAT :	/* Receive SD status as a data block (64 bytes) */
  		for (n=0;n<64;n++)
              *(ptr+n) = CardConfig.status[n];
          res = RES_OK;
  		break;

  	default:
  		res = RES_PARERR;
  	}

    SD_DeSelect();
		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}
