
#include "startup.h"
#include "sdio_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

	extern bool steup_sdio(void)
	{
		en_result_t rt = Ok;
		Sdioc_Class sdioc(M4_SDIOC1);
		rt = sdioc.init();
		return (rt == Ok ? true : false);
	}

	extern bool SDIO_ReadBlock_DMA(uint32_t blockAddress, uint8_t *data)
	{
		en_result_t result;
		result = SDIO.read(blockAddress, SD_COUNT, data);
		if (result != Ok)
		{
			printf("SDIO_ReadBlock_DMA err!,errcode=%u\n", result);
			return (false);
		}
		return (true);
	}

	extern bool SDIO_WriteBlockDMA(uint32_t blockAddress, const uint8_t *data)
	{
		en_result_t result;
		result = SDIO.write(blockAddress, SD_COUNT, data);
		if (result != Ok)
		{
			printf("SDIO_WriteBlockDMA err!,errcode=%u\n", result);
			return (false);
		}
		return (true);
	}

	extern en_result_t sdio_main(void)
	{
		uint32_t i = 0;
		en_result_t result;
		static uint32_t au32WriteBlocks[512];
		static uint32_t au32ReadBlocks[512];

		for (i = 0u; i < ARRAY_SZ(au32WriteBlocks); i++)
		{
			au32WriteBlocks[i] = i;
		}
		MEM_ZERO_STRUCT(au32ReadBlocks);
		result = SDIO.erase(SD_SECTOR_START, SD_SECTOR_COUNT);
		if (result != Ok)
		{
			printf("erase err!,errcode=%u\n", result);
			return (result);
		}
		result = SDIO.read(SD_SECTOR_START, SD_SECTOR_COUNT, (uint8_t *)&au32ReadBlocks[0]);
		if (result != Ok)
		{
			printf("read err!,errcode=%u\n", result);
			return (result);
		}
		for (i = 0u; i < ARRAY_SZ(au32WriteBlocks); i++)
		{
			if ((au32ReadBlocks[i] != 0xFFFFFFFFul) &&
				(au32ReadBlocks[i] != 0x00000000ul))
			{
				result = Error;
				printf("ARRAY_SZ err!\n");
				break;
			}
		}
		result = SDIO.write(SD_SECTOR_START, SD_SECTOR_COUNT, (uint8_t *)&au32WriteBlocks[0]);
		if (result != Ok)
		{
			printf("write err!,errcode=%u\n", result);
			return (result);
		}
		result = SDIO.read(SD_SECTOR_START, SD_SECTOR_COUNT, (uint8_t *)&au32ReadBlocks[0]);
		if (result != Ok)
		{
			printf("read data err!,errcode=%u\n", result);
			return (result);
		}
		if (0 != memcmp(au32WriteBlocks, au32ReadBlocks, sizeof(au32ReadBlocks)))
		{
			result = Error;
			printf("memcmp err!\n");
		}
		else
		{
			printf("sdio moution ok!\n");
		}
		return (result);
	}

#ifdef __cplusplus
};
#endif

/*****************end of file*******************/
