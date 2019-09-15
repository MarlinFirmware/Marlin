/*-----------------------------------------------------------------------
/ * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
/ * Copyright (c) 2019 BigTreeTech [https://github.com/bigtreetech]
/ * Low level disk interface module include file   (C)ChaN, 2015
/-----------------------------------------------------------------------*/

#pragma once

#define _DISKIO_WRITE   1   /* 1: Enable disk_write function */
#define _DISKIO_IOCTL   1   /* 1: Enable disk_ioctl fucntion */
#define _DISKIO_ISDIO   0   /* 1: Enable iSDIO control fucntion */

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned int UINT;

/* Status of Disk Functions */
typedef BYTE    DSTATUS;

/* Results of Disk Functions */
typedef enum {
  RES_OK = 0,     /* 0: Successful */
  RES_ERROR,      /* 1: R/W Error */
  RES_WRPRT,      /* 2: Write Protected */
  RES_NOTRDY,     /* 3: Not Ready */
  RES_PARERR      /* 4: Invalid Parameter */
} DRESULT;


#if _DISKIO_ISDIO
/* Command structure for iSDIO ioctl command */
typedef struct {
 BYTE    func;   /* Function number: 0..7 */
 WORD    ndata;  /* Number of bytes to transfer: 1..512, or mask + data */
 DWORD   addr;   /* Register address: 0..0x1FFFF */
 void*   data;   /* Pointer to the data (to be written | read buffer) */
} SDIO_CMD;
#endif

/*---------------------------------------*/
/* Prototypes for disk control functions */

DSTATUS disk_initialize(BYTE pdrv);
DSTATUS disk_status(BYTE pdrv);
DRESULT disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
#if _DISKIO_WRITE
  DRESULT disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
#endif
#if _DISKIO_IOCTL
  DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff);
#endif

/* Disk Status Bits (DSTATUS) */
#define STA_NOINIT      0x01    /* Drive not initialized */
#define STA_NODISK      0x02    /* No medium in the drive */
#define STA_PROTECT     0x04    /* Write protected */

/* Command code for disk_ioctrl fucntion */

/* Generic command (Used by FatFs) */
#define CTRL_SYNC        0  /* Complete pending write process (needed at _FS_READONLY == 0) */
#define GET_SECTOR_COUNT 1  /* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_SIZE  2  /* Get sector size (needed at _MAX_SS != _MIN_SS) */
#define GET_BLOCK_SIZE   3  /* Get erase block size (needed at _USE_MKFS == 1) */
#define CTRL_TRIM        4  /* Inform device that the data on the block of sectors is no longer used (needed at _USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_FORMAT      5  /* Create physical format on the media */
#define CTRL_POWER_IDLE  6  /* Put the device idle state */
#define CTRL_POWER_OFF   7  /* Put the device off state */
#define CTRL_LOCK        8  /* Lock media removal */
#define CTRL_UNLOCK      9  /* Unlock media removal */
#define CTRL_EJECT      10  /* Eject media */

/* MMC/SDC specific ioctl command (Not used by FatFs) */
#define MMC_GET_TYPE    50  /* Get card type */
#define MMC_GET_CSD     51  /* Get CSD */
#define MMC_GET_CID     52  /* Get CID */
#define MMC_GET_OCR     53  /* Get OCR */
#define MMC_GET_SDSTAT  54  /* Get SD status */
#define ISDIO_READ      55  /* Read data form SD iSDIO register */
#define ISDIO_WRITE     56  /* Write data to SD iSDIO register */
#define ISDIO_MRITE     57  /* Masked write data to SD iSDIO register */

/* ATA/CF specific ioctl command (Not used by FatFs) */
#define ATA_GET_REV     60  /* Get F/W revision */
#define ATA_GET_MODEL   61  /* Get model name */
#define ATA_GET_SN      62  /* Get serial number */

/* MMC card type flags (MMC_GET_TYPE) */
#define CT_MMC      0x01        /* MMC ver 3 */
#define CT_SD1      0x02        /* SD ver 1 */
#define CT_SD2      0x04        /* SD ver 2 */
#define CT_SDC      (CT_SD1|CT_SD2) /* SD */
#define CT_BLOCK    0x08        /* Block addressing */
