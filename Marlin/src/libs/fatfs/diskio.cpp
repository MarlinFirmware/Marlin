/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/


#include "diskio.h"		/* FatFs lower layer API */
#ifdef MKS_WIFI

volatile uint8_t __attribute__ ((aligned (4))) buf_copy[512];

/* Definitions of physical drive number for each drive */
#define DEV_SD		0	/* Example: Map MMC/SD card to physical drive 1 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	if(pdrv == DEV_SD){
	return RES_OK;	
	};
	return STA_NODISK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	int result;
		
	if(pdrv == DEV_SD){
		result=SD_Init();
		if(result != 0) {
			return STA_NOINIT;
		};
		return(0);
	};
	return STA_NODISK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
uint8_t res=0;
	
	if(pdrv == DEV_SD){
	
		if(((uint32_t)buff % 4) != 0){
			DEBUG("Buffer not aligned");
			while (count--){

			res=SD_transfer((uint8_t *)buf_copy, (uint32_t) sector, 1, SD2UM);
			if(res != 0){
				res=SD_transfer((uint8_t *)buf_copy, (uint32_t) sector, 1, SD2UM);
				if(res != 0){
					return RES_ERROR;
				};
			};
			memcpy((uint8_t *)buff,(uint8_t *)buf_copy,512);
			buff+=512; //uint
			sector++;
			}

		}else{
		//1st read
		res=SD_transfer((uint8_t *)buff, (uint32_t) sector, count, SD2UM);
		if(res != 0){
			res=SD_transfer((uint8_t *)buff, (uint32_t) sector, count, SD2UM);
			if(res != 0){
				return RES_ERROR;
			};
		};
		};
		return RES_OK;
	};
	return RES_PARERR;	
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
uint8_t res;

	if(pdrv == DEV_SD){

		if(((uint32_t)buff % 4) != 0){
			DEBUG("Buffer not aligned");
			while (count--){
				memcpy((uint8_t *)buf_copy,(uint8_t *)buff,512);

				res=SD_transfer((uint8_t *)buf_copy, (uint32_t) sector, 1, UM2SD);
				if(res != 0){
					res=SD_transfer((uint8_t *)buf_copy, (uint32_t) sector, 1, UM2SD);
					if(res != 0){
						return RES_ERROR;
					};
				};
			
				buff+=512;
				sector++;
			}

		}else{
			res=SD_transfer((uint8_t *)buff, (uint32_t) sector, count, UM2SD);
			if(res != 0){
				res=SD_transfer((uint8_t *)buff, (uint32_t) sector, count, UM2SD);
				if(res != 0){
					return RES_ERROR;
				};
			};
		};
	
		return RES_OK;
	};


return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_ERROR; 

	if(pdrv == DEV_SD){

	switch (cmd)
  {

	case CTRL_SYNC:
      res = RES_OK;
      break;
 
    case GET_SECTOR_SIZE:
      *(DWORD*)buff = 512;
      res= RES_OK;
      break;
 
    case GET_BLOCK_SIZE:
      *(DWORD*)buff = 512;
      res= RES_OK;
      break;
  }
 
	return res;
};
	return(RES_OK);
};

DWORD get_fattime (void) {
	
	uint32_t  d,ret;
		
	d=2018-1980;
	ret=(d << 25);
	
	d=02;
	ret |= (d << 21);
	
	d=12;
	d=((d > 0) && (d < 32)) ? d : 1;
	ret |= (d << 16);
	
	d=12;
	d=((d > 0) && (d < 25)) ? d : 1;
	ret |= (d << 11);
	
	d=25;
	d=((d > 0) && (d < 60)) ? d : 1;
	ret |= (d << 5);
	
	d=30 / 2;
	d=((d > 0) && (d < 30)) ? d : 1;
	ret |= d ;
	
	return  ret;
};


#endif