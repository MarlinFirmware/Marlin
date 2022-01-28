#ifndef MKS_WIFI_UI_H
#define MKS_WIFI_UI_H

#include "../../MarlinCore.h"

#if ENABLED(TFT_480x320) || ENABLED(TFT_480x320_SPI)

void mks_update_status(char *filename, uint32_t current_filesize, uint32_t file_size);
void mks_upload_screen(char *filename, uint32_t file_size);
void mks_end_transmit(void);
#endif

#endif