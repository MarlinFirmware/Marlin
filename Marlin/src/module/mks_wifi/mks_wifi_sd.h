#ifndef MKS_WIFI_SD_H
#define MKS_WIFI_SD_H
#include "mks_wifi.h"
#include "../../sd/cardreader.h"
#include "../shared_mem/shared_mem.h"

#ifdef MKS_WIFI


#define DMA_TIMEOUT 0x1ffffff
#define ESP_PACKET_SIZE     1024

#ifdef STM32F1
#define DMA_CONF    (uint32_t)(DMA_CCR_PL|DMA_CCR_MINC|DMA_CCR_TEIE|DMA_CCR_TCIE)
#define DMA_CLEAR   (uint32_t)(DMA_IFCR_CGIF5|DMA_IFCR_CTEIF5|DMA_IFCR_CHTIF5|DMA_IFCR_CTCIF5)
#endif

#ifdef STM32F4
#define DMA_CONF			((uint32_t)( (0x04 << DMA_SxCR_CHSEL_Pos) | \
										 (0x00 << DMA_SxCR_MBURST_Pos)| \
										 (0x00 << DMA_SxCR_PBURST_Pos)| \
										 (0x00 << DMA_SxCR_DBM_Pos)   | \
										 (0x00 << DMA_SxCR_PL_Pos)	  | \
										 (0x00 << DMA_SxCR_PINCOS_Pos)| \
										 (0x00 << DMA_SxCR_MSIZE_Pos) | \
										 (0x00 << DMA_SxCR_PSIZE_Pos) | \
										 (0x01 << DMA_SxCR_MINC_Pos)  | \
										 (0x00 << DMA_SxCR_PINC_Pos)  | \
										 (0x00 << DMA_SxCR_CIRC_Pos)  | \
										 (0x01 << DMA_SxCR_TCIE_Pos)  | \
										 (0x01 << DMA_SxCR_TEIE_Pos)  | \
										 (0x00 << DMA_SxCR_PFCTRL_Pos)))

#define DMA_S5_CLEAR            (uint32_t)(DMA_HIFCR_CTCIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CFEIF5 | DMA_HIFCR_CHTIF5)
#endif

//Под буфер для DMA два последних КБ из буфера
#define ESP_FILE_BUFF_COUNT (SHARED_MEM_1KB_COUNT-2)
//Под буфер для записи в файл все оставшееся с начала
#define FILE_BUFFER_SIZE  ESP_PACKET_SIZE*ESP_FILE_BUFF_COUNT

void mks_wifi_sd_ls(void);

uint8_t mks_wifi_sd_init(void);
void mks_wifi_sd_deinit(void);
void sd_delete_file(char *filename);

uint8_t get_dos_filename(char *filename, char* dosfilename);

void mks_wifi_start_file_upload(ESP_PROTOC_FRAME *packet);

#endif

#endif