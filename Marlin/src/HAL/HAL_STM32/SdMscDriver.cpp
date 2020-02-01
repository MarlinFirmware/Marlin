
#ifdef SDIO_SUPPORT

extern "C" {
  #include "SdMscDriver.h"
}

//typedef enum
//{
//  HAL_OK       = 0x00U,
//  HAL_ERROR    = 0x01U,
//  HAL_BUSY     = 0x02U,
//  HAL_TIMEOUT  = 0x03U
//} HAL_StatusTypeDef;

// HAL pass is "0"
// Marlin SD card pass is "1"

bool SDIO_Init() {return !SDIO_Init_C();}
bool SDIO_ReadBlock(uint32_t block, uint8_t *dst) {return !SDIO_ReadBlock_C(block, dst);}
bool SDIO_WriteBlock(uint32_t block, const uint8_t *src) {return !SDIO_WriteBlock_C(block, src);}
#endif
