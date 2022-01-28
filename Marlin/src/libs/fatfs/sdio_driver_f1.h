#ifndef SDIO_DRIVER_H
#define SDIO_DRIVER_H

#include "../../module/mks_wifi/mks_wifi.h"

#ifdef MKS_WIFI
#ifdef STM32F1

// SD card description
typedef struct {
	uint32_t    Capacity;        // Card capacity (MBytes for SDHC/SDXC, bytes otherwise)
	uint32_t    BlockCount;      // SD card blocks count
	uint32_t    BlockSize;       // SD card block size (bytes), determined in SD_ReadCSD()
	uint32_t    MaxBusClkFreq;   // Maximum card bus frequency (MHz)
	uint32_t    RCA;             // SD card RCA address (only for SDIO)
	uint32_t    PSN;             // SD card serial number
	uint32_t    CSD[4];          // SD card CSD register (card structure data)
	uint32_t    CID[4];          // SD card CID register (card identification number)
	uint16_t    OID;             // SD card OEM/Application ID
	uint16_t    MDT;             // SD card manufacturing date
	uint8_t     Type;            // Card type (detected by SD_Init())
	uint8_t     CSDVer;          // SD card CSD register version
	uint8_t     MID;             // SD card manufacturer ID
	uint8_t     PNM[5];          // SD card product name (5-character ASCII string)
	uint8_t     PRV;             // SD card product revision (two BCD digits: '6.2' will be 01100010b)
	uint8_t     SCR[8];          // SD card SCR register (SD card configuration)
} SDCard_TypeDef;

typedef struct {
	uint8_t			out_of_range;     //Аргумент команды вышел за пределы допустимого диапазона для этой карты.
	uint8_t			address_error;		//Ошибочно выровненный адрес, который не соответствует длине блока, который использовался в команде.
	uint8_t			block_len_error;	//Длина передаваемого блока не допустима для этой карты, или количество передаваемых байт не соответствует длине блока.
	uint8_t			erase_seq_error;	//Произошла ошибка в последовательности команд стирания.
	uint8_t			erase_param;			//Произошел недопустимый выбор записываемых блоков для стирания.
	uint8_t			wp_violation;			//Устанавливается, когда хост попытался выполнить запись в защищенный блок либо на временно защищенную от записи, либо на постоянно защищенную от записи карту.
	uint8_t			card_is_locked;		//Если установлен, то сигнализирует, что карта заблокирована хостом.
	uint8_t			lock_unlock_failed; //Устанавливается, когда была детектирована ошибка пароля в команде блокировки/разблокировки карты.
	uint8_t			com_crc_error;		//Ошибка CRC предыдущей команды.
	uint8_t			illegal_command;  //Команда недопустима для текущего состояния карты.
	uint8_t			card_ecc_failed;  //Была применена внутренняя ECC, но произошла ошибка для корректных данных.
	uint8_t			cc_error;					//Ошибка внутреннего контроллера карты.
	uint8_t			error;						//Во время выполнения операции произошла общая или неизвестная ошибка.
	uint8_t			csd_overwrite;    //Произошла одна из следующих ошибок: - Секция только для чтения CSD не соответствует содержимому карты.  - Попытка реверса копирования (копирование в место источника), или ошибка защиты от записи.
	uint8_t			wp_erase_skip;    //Устанавливается, когда была очищена только часть адресного пространства - из-за наличия защищенных от записи блоков, или очищалась карта, временно или постоянно защищенная от записи.
	uint8_t			card_ecc_disabled;//Была выполнена команда без внутреннего ECC.
	uint8_t			erase_reset;      //Была очищена последовательность стирания перед выполнением, потому что была принята команда выхода из последовательности стирания.
	uint8_t			current_state;    //Состояние карты, когда принимается команда. Если выполнение команды приводит к изменению состояния карты, это увидит хост в ответ на следующую команду. Эти 4 бита интерпретируются как двоичное число со значением в диапазоне от 0 до 15.
	/*
																	0: idle
																	1: ready
																	2: ident
																	3: stby
																	4: tran
																	5: data
																	6: rcv
																	7: prg
																	8: dis
																	9..14:зарезервировано
																	15: зарезервировано для режима I/O
	*/
	uint8_t			ready_for_data;  //Соответствует сигнализации по шине, что буфер пуст.
	uint8_t			app_cmd;         //Карта ожидает ACMD, или показывается, что команда была интерпретирована как ACMD.
	uint8_t			ake_seq_error;   //Ошибка в последовательности аутентификации.
} SD_Status_TypeDef;

// Card type
enum {
	SDCT_UNKNOWN = 0x00,
	SDCT_SDSC_V1 = 0x01,  // Standard capacity SD card v1.0
	SDCT_SDSC_V2 = 0x02,  // Standard capacity SD card v2.0
	SDCT_MMC     = 0x03,  // MMC
	SDCT_SDHC    = 0x04   // High capacity SD card (SDHC or SDXC)
};


#define SDIO_4BIT_Mode 1
//#define SDIO_HIGH_SPEED 1

#define SDIO_DATA_TIMEOUT     ((uint32_t)0x01000000)

// Mask for ACMD41
#define SD_STD_CAPACITY               ((uint32_t)0x00000000U)
#define SD_HIGH_CAPACITY              ((uint32_t)0x40000000U)

// SDIO CMD response type
#define SDIO_RESP_NONE         0x00                // No response
#define SDIO_RESP_SHORT        SDIO_CMD_WAITRESP_0 // Short response
#define SDIO_RESP_LONG         SDIO_CMD_WAITRESP   // Long response

// SD commands  index
#define SD_CMD0        ((uint8_t)0)
#define SD_CMD8        ((uint8_t)8)
#define SD_CMD55       ((uint8_t)55)
#define SD_ACMD41	   ((uint8_t)41)
#define SD_CMD2        ((uint8_t)2)
#define SD_CMD3        ((uint8_t)3)
#define SD_CMD6        ((uint8_t)6)
#define SD_CMD7        ((uint8_t)7)
#define SD_CMD9        ((uint8_t)9)

#define SD_CMD12        ((uint8_t)12)
#define SD_CMD13        ((uint8_t)13)

#define SD_CMD_SET_BLOCKLEN   ((uint8_t)16)
#define SD_CMD_SWITCH_FUNC    ((uint8_t)6U)

#define SD_CMD17        ((uint8_t)17)
#define SD_CMD18        ((uint8_t)18)

#define SD_CMD24        ((uint8_t)24)
#define SD_CMD25        ((uint8_t)25)

#define SDIO_ACMD41_CHECK            ((uint32_t)0x80000000)
// Pattern for R6 response
#define SD_CHECK_PATTERN              ((uint32_t)0x000001AA)

#define UM2SD         (0x00)  //Transfer Direction
#define SD2UM         (0x02)



#define DMA_S4_CLEAR        ((uint32_t) DMA_IFCR_CTCIF4 | DMA_IFCR_CTEIF4 | DMA_IFCR_CGIF4 | DMA_IFCR_CHTIF4)
#define DMA_SDIO_CR			((uint32_t)( (0x00 << DMA_CCR_PL_Pos)	  | \
										 (0x02 << DMA_CCR_MSIZE_Pos) | \
										 (0x02 << DMA_CCR_PSIZE_Pos) | \
										 (0x01 << DMA_CCR_MINC_Pos)  | \
										 (0x00 << DMA_CCR_PINC_Pos)  | \
										 (0x00 << DMA_CCR_CIRC_Pos)) )

#define SDIO_DATA_R_TIMEOUT   (uint32_t)9000000
#define SDIO_DCTRL				(uint32_t)((uint32_t) 9 << SDIO_DCTRL_DBLOCKSIZE_Pos ) | ((uint32_t) 1 << SDIO_DCTRL_DMAEN_Pos) | SDIO_DCTRL_DTEN
#define SDIO_ICR_STATIC     ((uint32_t)(SDIO_ICR_CCRCFAILC | SDIO_ICR_DCRCFAILC | SDIO_ICR_CTIMEOUTC | \
                                        SDIO_ICR_DTIMEOUTC | SDIO_ICR_TXUNDERRC | SDIO_ICR_RXOVERRC  | \
                                        SDIO_ICR_CMDRENDC  | SDIO_ICR_CMDSENTC  | SDIO_ICR_DATAENDC  | \
                                         SDIO_ICR_DBCKENDC | SDIO_ICR_STBITERRC ))

#define SDIO_ICR_DATA_FLAGS  (SDIO_ICR_DBCKENDC | SDIO_ICR_STBITERRC | SDIO_ICR_DATAENDC | SDIO_ICR_RXOVERRC | SDIO_ICR_TXUNDERRC | SDIO_ICR_DTIMEOUTC | SDIO_ICR_DCRCFAILC)
#define SDIO_ICR_CMD_FLAGS   (SDIO_ICR_CEATAENDC | SDIO_ICR_SDIOITC | SDIO_ICR_CMDSENTC | SDIO_ICR_CMDRENDC | SDIO_ICR_CTIMEOUTC | SDIO_ICR_CCRCFAILC)
#define SDIO_STA_ERRORS			(uint32_t)(SDIO_STA_STBITERR |  SDIO_STA_RXOVERR | SDIO_STA_TXUNDERR | SDIO_STA_DTIMEOUT | SDIO_STA_DCRCFAIL )

#define SDIO_STA_TRX_ERROR_FLAGS (SDIO_STA_STBITERR | SDIO_STA_RXOVERR | SDIO_STA_TXUNDERR | SDIO_STA_DTIMEOUT | SDIO_STA_DCRCFAIL)
#define SDIO_STA_CMD_ERROR_FLAGS (SDIO_STA_CTIMEOUT | SDIO_STA_CCRCFAIL)
#define SDIO_STA_TRX_ACT_FLAGS   (SDIO_STA_RXACT|SDIO_STA_TXACT)
#define SDIO_STA_CMD_FLAGS		(uint32_t)(SDIO_STA_CCRCFAIL|SDIO_STA_CTIMEOUT|SDIO_STA_CMDSENT|SDIO_STA_CMDREND)


#define DISABLE_IRQ 	{ __asm volatile ("cpsid i" : : : "memory");}
#define ENABLE_IRQ      { __asm volatile ("cpsie i" : : : "memory");}

//uint8_t SD_Init(void);
void SD_parse_CSD(uint32_t* reg);
void SD_check_status(SD_Status_TypeDef* SDStatus,uint32_t* reg);
uint32_t SD_get_block_count(void);
uint8_t SD_Cmd(uint8_t cmd, uint32_t arg, uint16_t response_type, uint32_t *response);
uint32_t SD_transfer(uint8_t *buf, uint32_t blk, uint32_t cnt, uint32_t dir);
uint8_t SD_Init(void);
//void SDIO_Config(void);

#endif

#endif
#endif