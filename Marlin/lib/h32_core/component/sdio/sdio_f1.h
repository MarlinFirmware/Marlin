#ifndef SDIO_H
#define SDIO_H

#ifdef __cplusplus
extern "C"
{
#endif

/* SD sector && count */
#define SD_SECTOR_START (0u)
#define SD_SECTOR_COUNT (4u)
#define SD_COUNT (1u) // ENDER3

    extern en_result_t sdio_main(void);
    extern bool steup_sdio(void);
    extern bool SDIO_ReadBlock_DMA(uint32_t blockAddress, uint8_t *data);
    extern bool SDIO_WriteBlockDMA(uint32_t blockAddress, const uint8_t *data);

#ifdef __cplusplus
};
#endif

#endif
