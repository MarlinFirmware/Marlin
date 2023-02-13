

#ifdef ARDUINO_ARCH_STM32F1

#include "../../inc/MarlinConfig.h" // Allow pins/pins.h to set density

bool SDIO_Init()
{
	return (steup_sdio());
}

bool SDIO_ReadBlock(uint32_t blockAddress, uint8_t *data)
{
	uint32_t retries = 3;
	while (retries--)
		if (SDIO_ReadBlock_DMA(blockAddress, data))
			return true;
	return false;
}

bool SDIO_WriteBlock(uint32_t blockAddress, const uint8_t *data)
{
	return SDIO_WriteBlockDMA(blockAddress, data);
}

// No F1 board with SDIO + MSC using Maple, that I aware of...
bool SDIO_IsReady() { return true; }
uint32_t SDIO_GetCardSize() { return 0; }

#endif // ARDUINO_ARCH_STM32F1
