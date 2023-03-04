#ifdef TARGET_HC32F46x
#include "sdio.h"
#include "sd_card.h"

#define SDIO_INTERFACE M4_SDIOC1

#define SDIO_READ_RETRIES 3
#define SDIO_WRITE_RETRIES 1
#define SDIO_TIMEOUT 100u

#define WITH_RETRY(retries, fn)                     \
	for (int retry = 0; retry < (retries); retry++) \
	{                                               \
		MarlinHAL::watchdog_refresh();              \
		fn                                          \
	}

static stc_sd_handle_t cardHandle;

const stc_sdcard_dma_init_t dmaConf = {
	.DMAx = M4_DMA2,
	.enDmaCh = DmaCh0,
};

const stc_sdcard_init_t cardConf = {
	.enBusWidth = SdiocBusWidth4Bit,
	.enClkFreq = SdiocClk400K,
	.enSpeedMode = SdiocNormalSpeedMode,
	.pstcInitCfg = NULL,
};

bool SDIO_Init()
{
	// configure SDIO pins
	PORT_SetFuncGPIO(BOARD_SDIO_D0, Disable);
	PORT_SetFuncGPIO(BOARD_SDIO_D1, Disable);
	PORT_SetFuncGPIO(BOARD_SDIO_D2, Disable);
	PORT_SetFuncGPIO(BOARD_SDIO_D3, Disable);
	PORT_SetFuncGPIO(BOARD_SDIO_CLK, Disable);
	PORT_SetFuncGPIO(BOARD_SDIO_CMD, Disable);
	PORT_SetFuncGPIO(BOARD_SDIO_DET, Disable);

	// create sdio handle
	MEM_ZERO_STRUCT(cardHandle);
	cardHandle.SDIOCx = SDIO_INTERFACE;
	cardHandle.enDevMode = SdCardDmaMode;
	cardHandle.pstcDmaInitCfg = &dmaConf;

	// initialize sd card
	en_result_t rc = SDCARD_Init(&cardHandle, &cardConf);
	if (rc != Ok)
	{
		printf("SDIO_Init() error (rc=%u)\n", rc);
	}

	return rc == Ok;
}

bool SDIO_ReadBlock(uint32_t block, uint8_t *dst)
{
	WITH_RETRY(SDIO_READ_RETRIES, {
		en_result_t rc = SDCARD_ReadBlocks(&cardHandle, block, 1, dst, SDIO_TIMEOUT);
		if (rc == Ok)
		{
			return true;
		}
		else
		{
			printf("SDIO_ReadBlock error (rc=%u)\n", rc);
		}
	})

	return false;
}

bool SDIO_WriteBlock(uint32_t block, const uint8_t *src)
{
	WITH_RETRY(SDIO_WRITE_RETRIES, {
		en_result_t rc = SDCARD_WriteBlocks(&cardHandle, block, 1, (uint8_t *)src, SDIO_TIMEOUT);
		if (rc == Ok)
		{
			return true;
		}
		else
		{
			printf("SDIO_WriteBlock error (rc=%u)\n", rc);
		}
	})

	return false;
}

bool SDIO_IsReady()
{
	return bool(cardHandle.stcCardStatus.READY_FOR_DATA);
}

uint32_t SDIO_GetCardSize()
{
	// multiply number of blocks with block size to get size in bytes
	uint64_t cardSizeBytes = uint64_t(cardHandle.stcSdCardInfo.u32LogBlockNbr) * uint64_t(cardHandle.stcSdCardInfo.u32LogBlockSize);

	// if the card is bigger than ~4Gb (maximum a 32bit integer can hold), clamp to the maximum value of a 32 bit integer
	if(cardSizeBytes >= UINT32_MAX)
	{
		return UINT32_MAX;
	}

	return uint32_t(cardSizeBytes);
}

#endif // TARGET_HC32F46x
