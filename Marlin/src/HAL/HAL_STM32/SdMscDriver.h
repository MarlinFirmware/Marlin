//#ifdef USBD_USE_CDC_COMPOSITE


#include <stdint.h>
#include <stdbool.h>


bool SDIO_Init_C();
bool SDIO_ReadBlock_C(uint32_t block, uint8_t *dst);
bool SDIO_WriteBlock_C(uint32_t block, const uint8_t *src);
uint8_t _status_init;
uint8_t _status_W;


