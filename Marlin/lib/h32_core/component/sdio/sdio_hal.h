#ifndef _CLASS_SDIOI_H_
#define _CLASS_SDIOI_H_

#include "hc32_ddl.h"
#include "sdmmc_cmd.h"
#include "sd_card.h"

#define sdio_timeout 100u

class Sdioc_Class
{
public:
	Sdioc_Class(M4_SDIOC_TypeDef *SDIOCx);
	~Sdioc_Class(void);

public:
	static en_result_t init(void);
	static void end(M4_SDIOC_TypeDef *SDIOCx);
	static en_result_t erase(uint32_t BlkStartAddr, uint32_t BlkEndAddr);
	static en_result_t read(uint32_t BlockAddr, uint16_t BlockCnt, uint8_t *dest);
	static en_result_t write(uint32_t BlockAddr, uint16_t BlockCnt, const uint8_t *dest);

private:
};

extern Sdioc_Class SDIO;

#endif

/*****************end of file*******************/
