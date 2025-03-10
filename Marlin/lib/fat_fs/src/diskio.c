/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2017        */
/*                                                                       */
/*   Portions COPYRIGHT 2017 STMicroelectronics                          */
/*   Portions Copyright (C) 2017, ChaN, all right reserved               */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various existing      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

/* Includes ------------------------------------------------------------------*/
#include "diskio.h"
#include "ff_gen_drv.h"

#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#endif

extern Disk_drvTypeDef  disk;

/*!
    \brief      Gets Disk Status
    \param[in]  pdrv: Physical drive number (0..)
    \param[out] none
    \retval     DSTATUS: Operation status
*/
DSTATUS disk_status (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    DSTATUS stat;

    stat = disk.drv[pdrv]->disk_status(disk.lun[pdrv]);

    return stat;
}

/*!
    \brief      Initializes a Drive
    \param[in]  pdrv: Physical drive number (0..)
    \param[out] none
    \retval     DSTATUS: Operation status
*/
DSTATUS disk_initialize (
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat = RES_OK;

    if (disk.is_initialized[pdrv] == 0) {
        disk.is_initialized[pdrv] = 1;
        stat = disk.drv[pdrv]->disk_initialize(disk.lun[pdrv]);
    }

    return stat;
}

/*!
    \brief      Reads Sector(s)
    \param[in]  pdrv: Physical drive number (0..)
    \param[in]  buff: Data buffer to store read data
    \param[in]  sector: Sector address (LBA)
    \param[in]  count: Number of sectors to write (1..128)
    \param[out] none
    \retval     DRESULT: Operation result
*/
DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res;

    res = disk.drv[pdrv]->disk_read(disk.lun[pdrv], buff, sector, count);

    return res;
}

/*!
    \brief      Writes Sector(s)
    \param[in]  pdrv: Physical drive number (0..)
    \param[in]  buff: Data to be written
    \param[in]  sector: Sector address (LBA)
    \param[in]  count: Number of sectors to write (1..128)
    \param[out] none
    \retval     DRESULT: Operation result
*/
#if _USE_WRITE == 1
DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DRESULT res;

    res = disk.drv[pdrv]->disk_write(disk.lun[pdrv], buff, sector, count);

    return res;
}
#endif /* _USE_WRITE == 1 */

/*!
    \brief      I/O control operation
    \param[in]  pdrv: Physical drive number (0..)
    \param[in]  cmd: Control code
    \param[in]  buff: Buffer to send/receive control data
    \param[out] none
    \retval     DRESULT: Operation result
*/
#if _USE_IOCTL == 1
DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;

    res = disk.drv[pdrv]->disk_ioctl(disk.lun[pdrv], cmd, buff);

    return res;
}
#endif /* _USE_IOCTL == 1 */

/*!
    \brief      Gets Time from RTC
    \param[in]  none
    \param[out] none
    \retval     Time in DWORD
*/
__weak DWORD get_fattime (void)
{
    return 0;
}
