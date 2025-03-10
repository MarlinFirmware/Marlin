/*!
    \file  ff_gen_drv.c
    \brief FatFs generic low level driver

    \version 2018-10-08, V1.0.0, firmware for GD32 USBFS&USBHS
*/

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

#include "ff_gen_drv.h"


Disk_drvTypeDef disk = {{0},{0},{0},0};

/*!
    \brief      Links a compatible diskio driver/lun id and increments the number of active linked drivers
    \note       The number of linked drivers (volumes) is up to 10 due to FatFs limits
    \param[in]  drv: pointer to the disk IO Driver structure
    \param[in]  path: pointer to the logical drive path
    \param[in]  lun: only used for USB Key Disk to add multi-lun management else the parameter must be equal to 0
    \param[out] none
    \retval     Returns 0 in case of success, otherwise 1
*/
uint8_t FATFS_LinkDriverEx(const Diskio_drvTypeDef *drv, char *path, uint8_t lun)
{
    uint8_t ret = 1;
    uint8_t DiskNum = 0;

    if(disk.nbr < _VOLUMES) {
        disk.is_initialized[disk.nbr] = 0;
        disk.drv[disk.nbr] = drv;
        disk.lun[disk.nbr] = lun;
        DiskNum = disk.nbr++;
        path[0] = DiskNum + '0';
        path[1] = ':';
        path[2] = '/';
        path[3] = 0;
        ret = 0;
    }

    return ret;
}

/*!
    \brief      Links a compatible diskio driver/lun id and increments the number of active linked drivers
    \note       The number of linked drivers (volumes) is up to 10 due to FatFs limits
    \param[in]  drv: pointer to the disk IO Driver structure
    \param[in]  path: pointer to the logical drive path
    \param[out] none
    \retval     Returns 0 in case of success, otherwise 1
*/
uint8_t FATFS_LinkDriver(const Diskio_drvTypeDef *drv, char *path)
{
    return FATFS_LinkDriverEx(drv, path, 0);
}

/*!
    \brief      Unlinks a diskio driver and decrements the number of active linked drivers
    \param[in]  path: pointer to the logical drive path
    \param[in]  lun: only used for USB Key Disk to add multi-lun management else the parameter must be equal to 0
    \param[out] none
    \retval     Returns 0 in case of success, otherwise 1
*/
uint8_t FATFS_UnLinkDriverEx(char *path, uint8_t lun)
{
    uint8_t DiskNum = 0;
    uint8_t ret = 1;

    if (disk.nbr >= 1) {
        DiskNum = path[0] - '0';

        if (disk.drv[DiskNum] != 0) {
            disk.drv[DiskNum] = 0;
            disk.lun[DiskNum] = 0;
            disk.nbr--;
            ret = 0;
        }
    }

    return ret;
}

/*!
    \brief      Unlinks a diskio driver and decrements the number of active linked drivers
    \param[in]  path: pointer to the logical drive path
    \param[out] none
    \retval     Returns 0 in case of success, otherwise 1
*/
uint8_t FATFS_UnLinkDriver(char *path)
{
    return FATFS_UnLinkDriverEx(path, 0);
}

/*!
    \brief      Gets number of linked drivers to the FatFs module
    \param[in]  none
    \param[out] none
    \retval     Number of attached drivers
*/
uint8_t FATFS_GetAttachedDriversNbr(void)
{
    return disk.nbr;
}
