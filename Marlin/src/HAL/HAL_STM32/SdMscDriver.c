//#ifdef USBD_USE_CDC_COMPOSITE

#ifdef SDIO_SUPPORT

#include <stdint.h>
#include <stdbool.h>

#include "SdMscDriver.h"

//#include "usbd_msc_bot.h"
//#include "usbd_msc_scsi.h"
//#include "usbd_msc_composite.h"
//#include "usbd_msc_cdc_composite.h"

//#include "usbd_msc_data.h"

#if defined(STM32F103xE) || defined(STM32F103xG)
#include <stm32f1xx_hal_rcc_ex.h>
#include <stm32f1xx_hal_sd.h>
#elif defined(STM32F4xx)
//  #include <stm32f4xx_hal_rcc_ex.h>
#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_sd.h>
#else
#error "ERROR - Only STM32F103xE, STM32F103xG or STM32F4xx CPUs supported"
#endif

#define TIMEOUT_SD_ACCESS   0xFF  // guess - don't know units

//HAL_StatusTypeDef SDIO_Init(void);


SD_HandleTypeDef hsd;  // create SDIO structure


//typedef enum
//{
//  HAL_OK       = 0x00U,
//  HAL_ERROR    = 0x01U,
//  HAL_BUSY     = 0x02U,
//  HAL_TIMEOUT  = 0x03U
//} HAL_StatusTypeDef;

//  typedef struct
//  {
//    __IO uint8_t  CSDStruct;            /*!< CSD structure                         */
//    __IO uint8_t  SysSpecVersion;       /*!< System specification version          */
//    __IO uint8_t  Reserved1;            /*!< Reserved                              */
//    __IO uint8_t  TAAC;                 /*!< Data read access time 1               */
//    __IO uint8_t  NSAC;                 /*!< Data read access time 2 in CLK cycles */
//    __IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency              */
//    __IO uint16_t CardComdClasses;      /*!< Card command classes                  */
//    __IO uint8_t  RdBlockLen;           /*!< Max. read data block length           */
//    __IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed       */
//    __IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment              */
//    __IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment               */
//    __IO uint8_t  DSRImpl;              /*!< DSR implemented                       */
//    __IO uint8_t  Reserved2;            /*!< Reserved                              */
//    __IO uint32_t DeviceSize;           /*!< Device Size                           */
//    __IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min           */
//    __IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max           */
//    __IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min          */
//    __IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max          */
//    __IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier                */
//    __IO uint8_t  EraseGrSize;          /*!< Erase group size                      */
//    __IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier           */
//    __IO uint8_t  WrProtectGrSize;      /*!< Write protect group size              */
//    __IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable            */
//    __IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC              */
//    __IO uint8_t  WrSpeedFact;          /*!< Write speed factor                    */
//    __IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length          */
//    __IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed      */
//    __IO uint8_t  Reserved3;            /*!< Reserved                              */
//    __IO uint8_t  ContentProtectAppli;  /*!< Content protection application        */
//    __IO uint8_t  FileFormatGrouop;     /*!< File format group                     */
//    __IO uint8_t  CopyFlag;             /*!< Copy flag (OTP)                       */
//    __IO uint8_t  PermWrProtect;        /*!< Permanent write protection            */
//    __IO uint8_t  TempWrProtect;        /*!< Temporary write protection            */
//    __IO uint8_t  FileFormat;           /*!< File format                           */
//    __IO uint8_t  ECC;                  /*!< ECC code                              */
//    __IO uint8_t  CSD_CRC;              /*!< CSD CRC                               */
//    __IO uint8_t  Reserved4;            /*!< Always 1                              */
//
//  }HAL_SD_CardCSDTypeDef;
//  /**
//    * @}
//    */
//
//  /** @defgroup SD_Exported_Types_Group5 Card Identification Data: CID Register
//    * @{
//    */
//  typedef struct
//  {
//    __IO uint8_t  ManufacturerID;  /*!< Manufacturer ID       */
//    __IO uint16_t OEM_AppliID;     /*!< OEM/Application ID    */
//    __IO uint32_t ProdName1;       /*!< Product Name part1    */
//    __IO uint8_t  ProdName2;       /*!< Product Name part2    */
//    __IO uint8_t  ProdRev;         /*!< Product Revision      */
//    __IO uint32_t ProdSN;          /*!< Product Serial Number */
//    __IO uint8_t  Reserved1;       /*!< Reserved1             */
//    __IO uint16_t ManufactDate;    /*!< Manufacturing Date    */
//    __IO uint8_t  CID_CRC;         /*!< CID CRC               */
//    __IO uint8_t  Reserved2;       /*!< Always 1              */
//
//  }HAL_SD_CardCIDTypeDef;
//
//  typedef struct
//  {
//    __IO uint8_t  DataBusWidth;           /*!< Shows the currently defined data bus width                 */
//    __IO uint8_t  SecuredMode;            /*!< Card is in secured mode of operation                       */
//    __IO uint16_t CardType;               /*!< Carries information about card type                        */
//    __IO uint32_t ProtectedAreaSize;      /*!< Carries information about the capacity of protected area   */
//    __IO uint8_t  SpeedClass;             /*!< Carries information about the speed class of the card      */
//    __IO uint8_t  PerformanceMove;        /*!< Carries information about the card's performance move      */
//    __IO uint8_t  AllocationUnitSize;     /*!< Carries information about the card's allocation unit size  */
//    __IO uint16_t EraseSize;              /*!< Determines the number of AUs to be erased in one operation */
//    __IO uint8_t  EraseTimeout;           /*!< Determines the timeout for any number of AU erase          */
//    __IO uint8_t  EraseOffset;            /*!< Carries information about the erase offset                 */
//
//  }HAL_SD_CardStatusTypeDef;
//
//  typedef struct
//  {
//    uint32_t CardType;                     /*!< Specifies the card Type                         */
//
//    uint32_t CardVersion;                  /*!< Specifies the card version                      */
//
//    uint32_t Class;                        /*!< Specifies the class of the card class           */
//
//    uint32_t RelCardAdd;                   /*!< Specifies the Relative Card Address             */
//
//    uint32_t BlockNbr;                     /*!< Specifies the Card Capacity in blocks           */
//
//    uint32_t BlockSize;                    /*!< Specifies one block size in bytes               */
//
//    uint32_t LogBlockNbr;                  /*!< Specifies the Card logical Capacity in blocks   */
//
//    uint32_t LogBlockSize;                 /*!< Specifies logical block size in bytes           */
//
//  }HAL_SD_CardInfoTypeDef;


HAL_SD_CardCSDTypeDef pCSD;  // create structure to hold decoded CSD data
HAL_SD_CardCIDTypeDef pCID;  // create structure to hold decoded CID data
HAL_SD_CardStatusTypeDef pStatus;  // create structure to hold card status data
HAL_SD_CardInfoTypeDef pCardInfo;  // create structure to hold card info

HAL_StatusTypeDef HAL_SD_GetCardCSD(SD_HandleTypeDef *hsd, HAL_SD_CardCSDTypeDef *pCSD);
HAL_StatusTypeDef HAL_SD_GetCardCID(SD_HandleTypeDef *hsd, HAL_SD_CardCIDTypeDef *pCID);
HAL_StatusTypeDef HAL_SD_GetCardStatus(SD_HandleTypeDef *hsd, HAL_SD_CardStatusTypeDef *pStatus);
HAL_StatusTypeDef HAL_SD_ConfigWideBusOperation(SD_HandleTypeDef *hsd, uint32_t WideMode);

HAL_StatusTypeDef HAL_SD_Init(SD_HandleTypeDef *hsd);
HAL_StatusTypeDef HAL_SD_InitCard(SD_HandleTypeDef *hsd);
HAL_StatusTypeDef HAL_SD_GetCardInfo(SD_HandleTypeDef *hsd, HAL_SD_CardInfoTypeDef *pCardInfo);
HAL_StatusTypeDef HAL_SD_ReadBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout);
HAL_StatusTypeDef HAL_SD_WriteBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout);
HAL_StatusTypeDef SDIO_Init(SDIO_TypeDef *SDIOx, SDIO_InitTypeDef Init);

#define TRANSFER_CLOCK_DIV ((uint8_t)SDIO_INIT_CLK_DIV/40)

#ifndef USBD_OK
#define USBD_OK 0
#endif

void go_to_transfer_speed()
{

  SD_InitTypeDef Init;

  /* Default SDIO peripheral configuration for SD card initialization */
  Init.ClockEdge           = hsd.Init.ClockEdge;
  Init.ClockBypass         = hsd.Init.ClockBypass;
  Init.ClockPowerSave      = hsd.Init.ClockPowerSave;
  Init.BusWide             = hsd.Init.BusWide;
  Init.HardwareFlowControl = hsd.Init.HardwareFlowControl;
  Init.ClockDiv            = TRANSFER_CLOCK_DIV;

  /* Initialize SDIO peripheral interface with default configuration */
  SDIO_Init(hsd.Instance, Init);
}

void SD_LowLevel_Init(void)
{

  uint32_t tempreg;

  GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOC_CLK_ENABLE(); //enable GPIO clocks
  __HAL_RCC_GPIOD_CLK_ENABLE(); //enable GPIO clocks


  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;  //D0-D3 & SCK
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = 1;  //GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


  // Configure PD.02 CMD line
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  RCC->APB2RSTR  &= ~RCC_APB2RSTR_SDIORST_Msk;  // take SDIO out of reset
  RCC->APB2ENR |= RCC_APB2RSTR_SDIORST_Msk;  // enable SDIO clock

  // Enable the DMA2 Clock

  //Initialize the SDIO (with initial <400Khz Clock)
  tempreg = 0;  //Reset value
  tempreg |= SDIO_CLKCR_CLKEN;  //Clock is enabled
  tempreg |= (uint32_t)0x76;  //Clock Divider. Clock = 48000/(118+2) = 400Khz
  //Keep the rest at 0 => HW_Flow Disabled, Rising Clock Edge, Disable CLK ByPass, Bus Width = 0, Power save Disable
  SDIO->CLKCR = tempreg;

  //Power up the SDIO
  SDIO->POWER = 0x03;

}


void HAL_SD_MspInit(SD_HandleTypeDef *hsd) // application specific init
{
  UNUSED(hsd);   /* Prevent unused argument(s) compilation warning */
  __HAL_RCC_SDIO_CLK_ENABLE();  // turn on SDIO clock
}

//uint8_t status_init;
//uint8_t status_W;

//  extern uint8_t _status_init;
//  extern uint8_t _status_W;

HAL_StatusTypeDef SD_SDIO_Init()
{
  //init SDIO and get SD card info



  _status_init = 0x0F;
  _status_W = 0x0F;
  uint8_t status;
  hsd.Instance = SDIO;
  hsd.State = 0;  // HAL_SD_STATE_RESET
  SD_LowLevel_Init();
  status = (uint8_t) HAL_SD_Init(&hsd);
  _status_init = status;
  if (!status) {  // low level init, enable SDIO & populate some HSD fields, init SD card and get CID & CSD registers)
//    go_to_transfer_speed();
//    status = (uint8_t) HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B);
    _status_W = 5;
  }
  return status;
}


void init_SDIO_pins(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /**SDIO GPIO Configuration
  PC8     ------> SDIO_D0
  PC12    ------> SDIO_CK
  PD2     ------> SDIO_CMD
  */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

//bool SDIO_init() { return (bool) (SD_SDIO_Init() ? 1 : 0);}
bool SDIO_Init_C() { return (bool) (SD_SDIO_Init() ? 1 : 0);}
bool SDIO_ReadBlock_C(uint32_t block, uint8_t *dst) {
  HAL_StatusTypeDef status_read;
  HAL_StatusTypeDef status_card;
if (block == 0) block = 16384;

  hsd.Instance = SDIO;

  status_read = HAL_SD_ReadBlocks(&hsd, (uint8_t*)dst, block, 1, 1000);  // read one 512 byte block with 500mS timeout
  status_card = HAL_SD_GetCardState(&hsd);

  return (bool) ((status_read | status_card) ? 1 : 0);
}

bool SDIO_WriteBlock_C(uint32_t block, const uint8_t *src) {
  HAL_StatusTypeDef status_write;
  HAL_StatusTypeDef status_card;

  hsd.Instance = SDIO;

  status_write = HAL_SD_WriteBlocks(&hsd, (uint8_t*)src, block, 1, 500);  // write one 512 byte block with 500mS timeout
  status_card = HAL_SD_GetCardState(&hsd);

  return (bool) ((status_write | status_card) ? 1 : 0);
}

//bool SDIO_init();
//bool SDIO_ReadBlock(uint32_t block, uint8_t *dst);
//bool SDIO_WriteBlock(uint32_t block, const uint8_t *src);
//SDIO_init(); }
//SDIO_ReadBlock(block, dst); }
//SDIO_WriteBlock

#if 0

int8_t SD_MSC_Init(uint8_t lun);

int8_t SD_MSC_GetCapacity(uint8_t lun,
uint32_t *block_num,
uint16_t *block_size);

int8_t SD_MSC_IsReady(uint8_t lun);

int8_t SD_MSC_IsWriteProtected(uint8_t lun);

int8_t SD_MSC_Read(uint8_t lun,
uint8_t *buf,
uint32_t blk_addr,
uint16_t blk_len);

int8_t SD_MSC_Write(uint8_t lun,
uint8_t *buf,
uint32_t blk_addr,
uint16_t blk_len);

int8_t SD_MSC_GetMaxLun(void);




#define STANDARD_INQUIRY_DATA_LEN 0x24U


uint8_t SD_MSC_Inquirydata[] = {/* 36 */
  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0', '1',                    /* Version      : 4 Bytes */
};


USBD_StorageTypeDef SdMscDriver = {
  SD_MSC_Init,
  SD_MSC_GetCapacity,
  SD_MSC_IsReady,
  SD_MSC_IsWriteProtected,
  SD_MSC_Read,
  SD_MSC_Write,
  SD_MSC_GetMaxLun,
  (int8_t *)SD_MSC_Inquirydata
};



int8_t SD_MSC_Init(uint8_t lun)
{
  (void)lun; // Not used

  SD_SDIO_Init();
  return USBD_OK;
}


int8_t SD_MSC_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  (void)lun; // Not used

  *block_num  = hsd.SdCard.BlockNbr;  // Card Capacity in blocks
  *block_size = hsd.SdCard.BlockSize;  // number of bytes in a block

  return USBD_OK;
}


int8_t  SD_MSC_IsReady(uint8_t lun)
{
  (void)lun; // Not used
  if (hsd.State == HAL_SD_STATE_READY) {
    return USBD_OK;
  }
  return USBD_FAIL;
}


int8_t SD_MSC_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  (void)lun; // Not used

  if (HAL_SD_ReadBlocks(&hsd, buf, blk_addr, blk_len, TIMEOUT_SD_ACCESS)) {
    return USBD_FAIL;
  }
  return USBD_OK;
}


int8_t SD_MSC_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  (void)lun; // Not used

  if (HAL_SD_WriteBlocks(&hsd, buf, blk_addr, blk_len, TIMEOUT_SD_ACCESS)) {
    return USBD_FAIL;
  }
  return USBD_OK;
}


int8_t  SD_MSC_IsWriteProtected(uint8_t lun)
{
  (void)lun; // Not used

  return USBD_OK; // Never write protected
}


int8_t SD_MSC_GetMaxLun(void)
{

  return 0; // We have just 1 Logic unit number (LUN) which is zero
}

#endif // 0

#endif /* USBD_USE_CDC_COMPOSITE */
