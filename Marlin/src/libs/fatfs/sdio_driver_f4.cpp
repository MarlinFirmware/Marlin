#include "sdio_driver_f4.h"
#ifdef MKS_WIFI
#ifdef STM32F4

volatile SDCard_TypeDef SDCard;
volatile SD_Status_TypeDef SDStatus;
volatile uint32_t response[4];   //Для хранения ответа от карты
volatile uint8_t transmit;       //Флаг запущенной передачи данных в SDIO
volatile uint8_t state=0;        //Для хранения состояния карты
volatile uint8_t multiblock=0;   //Используется в прерывании SDIO, чтоб слать команду STOP
volatile uint32_t error_flag=0;

void SD_check_status(SD_Status_TypeDef* SDStatus,uint32_t* reg){
	SDStatus->ake_seq_error     = (*reg & (1 << 3)) ? 1 : 0;
	SDStatus->app_cmd           = (*reg & (1 << 5)) ? 1 : 0;
	SDStatus->ready_for_data    = (*reg & (1 << 8)) ? 1 : 0;
	SDStatus->current_state     = (uint8_t)((*reg & (0x0F << 9)) >> 9);
	SDStatus->erase_reset       = (*reg & (1 << 13)) ? 1 : 0;
	SDStatus->card_ecc_disabled = (*reg & (1 << 14)) ? 1 : 0;
	SDStatus->wp_erase_skip     = (*reg & (1 << 15)) ? 1 : 0;
	SDStatus->csd_overwrite     = (*reg & (1 << 16)) ? 1 : 0;
	SDStatus->error             = (*reg & (1 << 19)) ? 1 : 0;
	SDStatus->cc_error          = (*reg & (1 << 20)) ? 1 : 0;
	SDStatus->card_ecc_failed   = (*reg & (1 << 21)) ? 1 : 0;
	SDStatus->illegal_command   = (*reg & (1 << 22)) ? 1 : 0;
	SDStatus->com_crc_error     = (*reg & (1 << 23)) ? 1 : 0;
	SDStatus->lock_unlock_failed= (*reg & (1 << 24)) ? 1 : 0;
	SDStatus->card_is_locked    = (*reg & (1 << 25)) ? 1 : 0;
	SDStatus->wp_violation      = (*reg & (1 << 26)) ? 1 : 0;
	SDStatus->erase_param       = (*reg & (1 << 27)) ? 1 : 0;
	SDStatus->erase_seq_error   = (*reg & (1 << 28)) ? 1 : 0;
	SDStatus->block_len_error   = (*reg & (1 << 29)) ? 1 : 0;
	SDStatus->address_error     = (*reg & (1 << 30)) ? 1 : 0;
	SDStatus->out_of_range      = (*reg & (1U << 31)) ? 1 : 0;
};

uint8_t SD_Cmd(uint8_t cmd, uint32_t arg, uint16_t response_type, uint32_t *response){
	SDIO->ICR = SDIO_ICR_CCRCFAILC | SDIO_ICR_CTIMEOUTC | SDIO_ICR_CMDRENDC | SDIO_ICR_CMDSENTC;
	SDIO->ARG = arg;
	SDIO->CMD = (uint32_t)(response_type | cmd | SDIO_CMD_CPSMEN);

	while(SDIO->STA & SDIO_STA_CMDACT){asm("nop");}; 

	if (response_type != SDIO_RESP_NONE) {
		response[0] =	SDIO->RESP1;
		response[1] =	SDIO->RESP2;
		response[2] =	SDIO->RESP3;
		response[3] =	SDIO->RESP4;
	}
	
	if (SDIO->STA & SDIO_STA_CTIMEOUT) return 2;
	if (SDIO->STA & SDIO_STA_CCRCFAIL) return 3;  
	return 0;
}

uint32_t __attribute__((optimize("O0"))) SD_transfer(uint8_t *buf, uint32_t blk, uint32_t cnt, uint32_t dir){
	uint32_t trials;
	uint8_t cmd=0;

	if (SDCard.Type != SDCT_SDHC) {
		blk = blk * 512;
	}

	trials=SDIO_DATA_TIMEOUT;
	while (transmit && trials--) {};
	if(!trials) {
		//SEGGER_SYSVIEW_RecordEndCall(37);
		return 1;
		}

	state=0;
	while(state != 4){ //Дождаться когда карта будет в режиме tran (4)
		SD_Cmd(SD_CMD13, SDCard.RCA ,SDIO_RESP_SHORT,(uint32_t*)response); 
		SD_check_status((SD_Status_TypeDef*)&SDStatus,(uint32_t*)&response[0]);
		state=SDStatus.current_state;

		if((state == 5) || (state == 6)) SD_Cmd(SD_CMD12, 0, SDIO_RESP_SHORT,(uint32_t*)response);
	};

	//Выключить DMA (бит включения)
	DMA2->LIFCR=DMA_S3_CLEAR;
	DMA2_Stream3->CR=0;
	DMA2->LIFCR=DMA_S3_CLEAR;
	DMA2_Stream3->CR=DMA_SDIO_CR;

	multiblock = (cnt == 1) ? 0 : 1;
	if (dir==UM2SD){ //Запись
				DMA2_Stream3->CR|=(0x01 << DMA_SxCR_DIR_Pos);
				cmd=(cnt == 1)? SD_CMD24 : SD_CMD25;
			} 
	else if (dir==SD2UM){ //Чтение
				cmd=(cnt == 1)? SD_CMD17 : SD_CMD18;
			};

	DMA2_Stream3->M0AR=(uint32_t)buf;    //Memory address	
	DMA2_Stream3->PAR=(uint32_t)&(SDIO->FIFO);  //SDIO FIFO Address 
	DMA2_Stream3->NDTR=0;   //Peripheral controls, therefore we don't need to indicate a size
	DMA2_Stream3->FCR=DMA_SDIO_FCR;

	transmit=1;
	error_flag=0;
	//__disable_irq();
	SD_Cmd(cmd, blk, SDIO_RESP_SHORT, (uint32_t*)response);

	SDIO->DTIMER=(uint32_t)SDIO_DATA_R_TIMEOUT;
	SDIO->DLEN=cnt*512;    //Количество байт (блок 512 байт)
	SDIO->DCTRL= SDIO_DCTRL | (dir & SDIO_DCTRL_DTDIR);  //Direction. 0=Controller to card, 1=Card to Controller
	SDIO->MASK=0;
	SDIO->ICR=SDIO_ICR_STATIC;

	DMA2_Stream3->CR |= DMA_SxCR_EN;
	__DSB();
	SDIO->DCTRL|=1; //DPSM is enabled
	//__enable_irq();

	while((SDIO->STA & (SDIO_STA_DATAEND|SDIO_STA_ERRORS)) == 0){asm("nop");};

	if(SDIO->STA & SDIO_STA_ERRORS){
		error_flag=SDIO->STA;
		transmit=0;
		SDIO->ICR = SDIO_ICR_STATIC;
		DMA2_Stream3->CR = 0;
		DMA2->LIFCR = DMA_S3_CLEAR;
		//SEGGER_SYSVIEW_RecordEndCall(37);
		return error_flag;
	}
	
	if(dir==SD2UM) { //Read
		while (DMA2_Stream3->CR & DMA_SxCR_EN) {
			if(SDIO->STA & SDIO_STA_ERRORS)	{
				return 99;
			}
				DMA2_Stream3->CR = 0;
				DMA2->LIFCR = DMA_S3_CLEAR;
			};
	};

	if(multiblock > 0) SD_Cmd(SD_CMD12, 0, SDIO_RESP_SHORT, (uint32_t*)response);
	transmit=0;		
	DMA2->LIFCR = DMA_S3_CLEAR;
	return 0;	
};


uint8_t SD_Init(void) {
	volatile uint32_t trials = 0x0000FFFF;
	uint32_t tempreg;   //Для временного хранения регистров
	//char str[30];
	uint8_t result = 0;
	
	result = SD_Cmd(SD_CMD0,0x00,SDIO_RESP_NONE,(uint32_t*)response);  //NORESP
	if (result != 0){
		return 1;
	};
	
	
	result = SD_Cmd(SD_CMD8,SD_CHECK_PATTERN,SDIO_RESP_SHORT,(uint32_t*)response);  //R7
	if (result != 0) {
		return 8;
	};
	if (response[0] != SD_CHECK_PATTERN) {
		return 8;
	};

	trials = 0x0000FFFF;
	while (--trials) {
			SD_Cmd(SD_CMD55, 0 ,SDIO_RESP_SHORT,(uint32_t*)response); // CMD55 with RCA 0   R1
			SD_check_status((SD_Status_TypeDef*)&SDStatus,(uint32_t*)&response[0]);
			SD_Cmd(SD_ACMD41,(1<<20|1<<30),SDIO_RESP_SHORT,(uint32_t*)response);
			if (response[0] & SDIO_ACMD41_CHECK) break;
		}
	if (!trials) {
		return 41; 
	};

	SDCard.Type = (response[0] & SD_HIGH_CAPACITY) ? SDCT_SDHC : SDCT_SDSC_V2;
	DEBUG("Card type %d",SDCard.Type);

	result = SD_Cmd(SD_CMD2,0x00,SDIO_RESP_LONG,(uint32_t*)response); //CMD2 CID R2
	if (result != 0) {
		return 2;
	};

		
	SDCard.CID[0]=response[0];
	SDCard.CID[1]=response[1];
	SDCard.CID[2]=response[2];
	SDCard.CID[3]=response[3];

	
	result = SD_Cmd(SD_CMD3,0x00,SDIO_RESP_SHORT,(uint32_t*)response); //CMD3 RCA R6
	if (result != 0){
		return 3;		
	};
	SDCard.RCA=( response[0] & (0xFFFF0000) );

	result = SD_Cmd(SD_CMD9,SDCard.RCA,SDIO_RESP_LONG,(uint32_t*)response); //CMD9 СSD  R2
	if (result != 0) return 9;		
	
	SDCard.CSD[0]=response[0];
	SDCard.CSD[1]=response[1];
	SDCard.CSD[2]=response[2];
	SDCard.CSD[3]=response[3];
	
	SD_parse_CSD((uint32_t*)SDCard.CSD);	
		
	result = SD_Cmd(SD_CMD7,SDCard.RCA,SDIO_RESP_SHORT,(uint32_t*)response); //CMD7 tran   R1b
	SD_check_status((SD_Status_TypeDef*)&SDStatus,(uint32_t*)&response[0]);
	if (result != 0) return 7;		

	state=0;
	//Дождаться когда карта будет в режиме tran (4)
	while(state != 4){
		SD_Cmd(SD_CMD13, SDCard.RCA ,SDIO_RESP_SHORT,(uint32_t*)response); 
		SD_check_status((SD_Status_TypeDef*)&SDStatus,(uint32_t*)&response[0]);
		state=SDStatus.current_state;
	};
	


  #if(SDIO_4BIT_Mode == 1)
		result = SD_Cmd(SD_CMD55, SDCard.RCA ,SDIO_RESP_SHORT,(uint32_t*)response); //CMD55 with RCA
		SD_check_status((SD_Status_TypeDef*)&SDStatus,(uint32_t*)&response[0]);
		if (result != 0)return 55;
	
		result = SD_Cmd(6, 0x02, SDIO_RESP_SHORT,(uint32_t*)response);      //Шлем ACMD6 c аргументом 0x02, установив 4-битный режим
		if (result != 0) {return 6;};
		if (response[0] != 0x920) {return 5;};    //Убеждаемся, что карта находится в готовности работать с трансфером

		tempreg=((0x01)<<SDIO_CLKCR_WIDBUS_Pos)| SDIO_CLKCR_CLKEN; 
		SDIO->CLKCR=tempreg;	

		#if (SDIO_HIGH_SPEED != 0)
			SD_HighSpeed();
			tempreg=((0x01)<<SDIO_CLKCR_WIDBUS_Pos)| SDIO_CLKCR_BYPASS | SDIO_CLKCR_CLKEN; 
			SDIO->CLKCR=tempreg;	
		#endif
#else
		tempreg=0;  
		tempreg=SDIO_CLKCR_CLKEN; 
		SDIO->CLKCR=tempreg;	
#endif

	if ((SDCard.Type != SDCT_SDHC)) {
		result = SD_Cmd(SD_CMD_SET_BLOCKLEN, 512 ,SDIO_RESP_SHORT,(uint32_t*)response); //CMD16
		if (result != 0) {
			ERROR("Error set block size");
			return 16;
		}
	}


	return 0;
};

void SD_parse_CSD(uint32_t* reg){
	uint32_t tmp;
	//Версия CSD регистра
	if(reg[0] & (11U << 30)){
		SDCard.CSDVer=2;
	}else{
		SDCard.CSDVer=1;
	};
	//Размер карты и количество блоков	
	tmp= (reg[2] >> 16) & 0xFFFF;
	tmp |= (reg[1] & 0x3F) << 16;
	SDCard.BlockCount=tmp*1000;
	SDCard.Capacity=(tmp+1)*512;
};
#endif
#endif