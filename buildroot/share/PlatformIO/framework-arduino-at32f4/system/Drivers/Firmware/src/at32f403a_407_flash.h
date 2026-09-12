/**
  **************************************************************************
  * @file     at32f403a_407_flash.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 flash header file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32F403A_407_FLASH_H
#define __AT32F403A_407_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"


/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/** @defgroup FLASH_unlock_keys
  * @brief flash unlock keys
  * @{
  */

#define FLASH_UNLOCK_KEY1                ((uint32_t)0x45670123) /*!< flash operation unlock order key1 */
#define FLASH_UNLOCK_KEY2                ((uint32_t)0xCDEF89AB) /*!< flash operation unlock order key2 */
#define FAP_RELIEVE_KEY                  ((uint16_t)0x00A5)     /*!< flash fap relieve key val */
#define SLIB_UNLOCK_KEY                  ((uint32_t)0xA35F6D24) /*!< flash slib operation unlock order key */

/**
  * @}
  */

/** @defgroup FLASH_bank_address
  * @brief flash bank address
  * @{
  */

#define FLASH_BANK1_START_ADDR           ((uint32_t)0x08000000) /*!< flash start address of bank1 */
#define FLASH_BANK1_END_ADDR             ((uint32_t)0x0807FFFF) /*!< flash end address of bank1 */
#define FLASH_BANK2_START_ADDR           ((uint32_t)0x08080000) /*!< flash start address of bank2 */
#define FLASH_BANK2_END_ADDR             ((uint32_t)0x080FFFFF) /*!< flash end address of bank2 */
#define FLASH_SPIM_START_ADDR            ((uint32_t)0x08400000) /*!< flash start address of spim */

/**
  * @}
  */

/** @defgroup FLASH_flags
  * @brief flash flag
  * @{
  */

#define FLASH_OBF_FLAG                   FLASH_BANK1_OBF_FLAG     /*!< flash operate busy flag */
#define FLASH_ODF_FLAG                   FLASH_BANK1_ODF_FLAG     /*!< flash operate done flag */
#define FLASH_PRGMERR_FLAG               FLASH_BANK1_PRGMERR_FLAG /*!< flash program error flag */
#define FLASH_EPPERR_FLAG                FLASH_BANK1_EPPERR_FLAG  /*!< flash erase/program protection error flag */
#define FLASH_BANK1_OBF_FLAG             ((uint32_t)0x00000001)   /*!< flash bank1 operate busy flag */
#define FLASH_BANK1_ODF_FLAG             ((uint32_t)0x00000020)   /*!< flash bank1 operate done flag */
#define FLASH_BANK1_PRGMERR_FLAG         ((uint32_t)0x00000004)   /*!< flash bank1 program error flag */
#define FLASH_BANK1_EPPERR_FLAG          ((uint32_t)0x00000010)   /*!< flash bank1 erase/program protection error flag */
#define FLASH_BANK2_OBF_FLAG             ((uint32_t)0x10000001)   /*!< flash bank2 operate busy flag */
#define FLASH_BANK2_ODF_FLAG             ((uint32_t)0x10000020)   /*!< flash bank2 operate done flag */
#define FLASH_BANK2_PRGMERR_FLAG         ((uint32_t)0x10000004)   /*!< flash bank2 program error flag */
#define FLASH_BANK2_EPPERR_FLAG          ((uint32_t)0x10000010)   /*!< flash bank2 erase/program protection error flag */
#define FLASH_SPIM_OBF_FLAG              ((uint32_t)0x20000001)   /*!< flash spim operate busy flag */
#define FLASH_SPIM_ODF_FLAG              ((uint32_t)0x20000020)   /*!< flash spim operate done flag */
#define FLASH_SPIM_PRGMERR_FLAG          ((uint32_t)0x20000004)   /*!< flash spim program error flag */
#define FLASH_SPIM_EPPERR_FLAG           ((uint32_t)0x20000010)   /*!< flash spim erase/program protection error flag */
#define FLASH_USDERR_FLAG                ((uint32_t)0x40000001)   /*!< flash user system data error flag */

/**
  * @}
  */

/** @defgroup FLASH_interrupts
  * @brief flash interrupts
  * @{
  */

#define FLASH_ERR_INT                    FLASH_BANK1_ERR_INT    /*!< flash error interrupt */
#define FLASH_ODF_INT                    FLASH_BANK1_ODF_INT    /*!< flash operate done interrupt */
#define FLASH_BANK1_ERR_INT              ((uint32_t)0x00000001) /*!< flash bank1 error interrupt */
#define FLASH_BANK1_ODF_INT              ((uint32_t)0x00000002) /*!< flash bank1 operate done interrupt */
#define FLASH_BANK2_ERR_INT              ((uint32_t)0x00000004) /*!< flash bank2 error interrupt */
#define FLASH_BANK2_ODF_INT              ((uint32_t)0x00000008) /*!< flash bank2 operate done interrupt */
#define FLASH_SPIM_ERR_INT               ((uint32_t)0x00000010) /*!< flash spim error interrupt */
#define FLASH_SPIM_ODF_INT               ((uint32_t)0x00000020) /*!< flash spim operate done interrupt */

/**
  * @}
  */

/** @defgroup FLASH_slib_mask
  * @brief flash slib mask
  * @{
  */

#define FLASH_SLIB_START_SECTOR          ((uint32_t)0x000007FF) /*!< flash slib start sector */
#define FLASH_SLIB_DATA_START_SECTOR     ((uint32_t)0x003FF800) /*!< flash slib d-bus area start sector */
#define FLASH_SLIB_END_SECTOR            ((uint32_t)0xFFC00000) /*!< flash slib end sector */

/**
  * @}
  */

/** @defgroup FLASH_user_system_data
  * @brief flash user system data
  * @{
  */

#define USD_WDT_ATO_DISABLE              ((uint16_t)0x0001) /*!< wdt auto start disabled  */
#define USD_WDT_ATO_ENABLE               ((uint16_t)0x0000) /*!< wdt auto start enabled */

#define USD_DEPSLP_NO_RST                ((uint16_t)0x0002) /*!< no reset generated when entering in deepsleep */
#define USD_DEPSLP_RST                   ((uint16_t)0x0000) /*!< reset generated when entering in deepsleep */

#define USD_STDBY_NO_RST                 ((uint16_t)0x0004) /*!< no reset generated when entering in standby */
#define USD_STDBY_RST                    ((uint16_t)0x0000) /*!< reset generated when entering in standby */

#define FLASH_BOOT_FROM_BANK1            ((uint16_t)0x0008) /*!< boot from bank1 */
#define FLASH_BOOT_FROM_BANK2            ((uint16_t)0x0000) /*!< boot from bank 2 or bank 1,depending on the activation of the bank */

/**
  * @}
  */

/** @defgroup FLASH_timeout_definition
  * @brief flash timeout definition
  * @{
  */

#define ERASE_TIMEOUT                    ((uint32_t)0x40000000) /*!< internal flash erase operation timeout */
#define PROGRAMMING_TIMEOUT              ((uint32_t)0x00100000) /*!< internal flash program operation timeout */
#define SPIM_ERASE_TIMEOUT               ((uint32_t)0xFFFFFFFF) /*!< spim erase operation timeout */
#define SPIM_PROGRAMMING_TIMEOUT         ((uint32_t)0x00100000) /*!< spim program operation timeout */
#define OPERATION_TIMEOUT                ((uint32_t)0x10000000) /*!< flash common operation timeout */

/**
  * @}
  */

/** @defgroup FLASH_exported_types
  * @{
  */

/**
  * @brief  flash status type
  */
typedef enum
{
  FLASH_OPERATE_BUSY                     = 0x00, /*!< flash status is operate busy */
  FLASH_PROGRAM_ERROR                    = 0x01, /*!< flash status is program error */
  FLASH_EPP_ERROR                        = 0x02, /*!< flash status is epp error */
  FLASH_OPERATE_DONE                     = 0x03, /*!< flash status is operate done */
  FLASH_OPERATE_TIMEOUT                  = 0x04  /*!< flash status is operate timeout */
} flash_status_type;

/**
  * @brief  flash spim model type
  */
typedef enum
{
  FLASH_SPIM_MODEL1                      = 0x01, /*!< spim model 1 */
  FLASH_SPIM_MODEL2                      = 0x02, /*!< spim model 2 */
} flash_spim_model_type;

/**
  * @brief type define flash register all
  */
typedef struct
{
  /**
    * @brief flash psr register, offset:0x00
    */
  union
  {
    __IO uint32_t psr;
    struct
    {
      __IO uint32_t reserved1            : 32; /* [31:0] */
    } psr_bit;
  };

  /**
    * @brief flash unlock register, offset:0x04
    */
  union
  {
    __IO uint32_t unlock;
    struct
    {
      __IO uint32_t ukval                : 32;/* [31:0] */
    } unlock_bit;
  };

  /**
    * @brief flash usd unlock register, offset:0x08
    */
  union
  {
    __IO uint32_t usd_unlock;
    struct
    {
      __IO uint32_t usd_ukval            : 32;/* [31:0] */
    } usd_unlock_bit;
  };

  /**
    * @brief flash sts register, offset:0x0C
    */
  union
  {
    __IO uint32_t sts;
    struct
    {
      __IO uint32_t obf                  : 1; /* [0] */
      __IO uint32_t reserved1            : 1; /* [1] */
      __IO uint32_t prgmerr              : 1; /* [2] */
      __IO uint32_t reserved2            : 1; /* [3] */
      __IO uint32_t epperr               : 1; /* [4] */
      __IO uint32_t odf                  : 1; /* [5] */
      __IO uint32_t reserved3            : 26;/* [31:6] */
    } sts_bit;
  };

  /**
    * @brief flash ctrl register, offset:0x10
    */
  union
  {
    __IO uint32_t ctrl;
    struct
    {
      __IO uint32_t fprgm                : 1; /* [0] */
      __IO uint32_t secers               : 1; /* [1] */
      __IO uint32_t bankers              : 1; /* [2] */
      __IO uint32_t reserved1            : 1; /* [3] */
      __IO uint32_t usdprgm              : 1; /* [4] */
      __IO uint32_t usders               : 1; /* [5] */
      __IO uint32_t erstr                : 1; /* [6] */
      __IO uint32_t oplk                 : 1; /* [7] */
      __IO uint32_t reserved2            : 1; /* [8] */
      __IO uint32_t usdulks              : 1; /* [9] */
      __IO uint32_t errie                : 1; /* [10] */
      __IO uint32_t reserved3            : 1; /* [11] */
      __IO uint32_t odfie                : 1; /* [12] */
      __IO uint32_t reserved4            : 19;/* [31:13] */
    } ctrl_bit;
  };

  /**
    * @brief flash addr register, offset:0x14
    */
  union
  {
    __IO uint32_t addr;
    struct
    {
      __IO uint32_t fa                   : 32;/* [31:0] */
    } addr_bit;
  };

  /**
    * @brief flash reserved1 register, offset:0x18
    */
  __IO uint32_t reserved1;

  /**
    * @brief flash usd register, offset:0x1C
    */
  union
  {
    __IO uint32_t usd;
    struct
    {
      __IO uint32_t usderr               : 1; /* [0] */
      __IO uint32_t fap                  : 1; /* [1] */
      __IO uint32_t wdt_ato_en           : 1; /* [2] */
      __IO uint32_t depslp_rst           : 1; /* [3] */
      __IO uint32_t stdby_rst            : 1; /* [4] */
      __IO uint32_t btopt                : 1; /* [5] */
      __IO uint32_t reserved1            : 4; /* [9:6] */
      __IO uint32_t user_d0              : 8; /* [17:10] */
      __IO uint32_t user_d1              : 8; /* [25:18] */
      __IO uint32_t reserved2            : 6; /* [31:26] */
    } usd_bit;
  };

  /**
    * @brief flash epps register, offset:0x20
    */
  union
  {
    __IO uint32_t epps;
    struct
    {
      __IO uint32_t epps                 : 32;/* [31:0] */
    } epps_bit;
  };

  /**
    * @brief flash reserved2 register, offset:0x40~0x24
    */
  __IO uint32_t reserved2[8];

  /**
    * @brief flash unlock2 register, offset:0x44
    */
  union
  {
    __IO uint32_t unlock2;
    struct
    {
      __IO uint32_t ukval                : 32;/* [31:0] */
    } unlock2_bit;
  };

  /**
    * @brief flash reserved3 register, offset:0x48
    */
  __IO uint32_t reserved3;

  /**
    * @brief flash sts2 register, offset:0x4C
    */
  union
  {
    __IO uint32_t sts2;
    struct
    {
      __IO uint32_t obf                  : 1; /* [0] */
      __IO uint32_t reserved1            : 1; /* [1] */
      __IO uint32_t prgmerr              : 1; /* [2] */
      __IO uint32_t reserved2            : 1; /* [3] */
      __IO uint32_t epperr               : 1; /* [4] */
      __IO uint32_t odf                  : 1; /* [5] */
      __IO uint32_t reserved3            : 26;/* [31:6] */
    } sts2_bit;
  };

  /**
    * @brief flash ctrl2 register, offset:0x50
    */
  union
  {
    __IO uint32_t ctrl2;
    struct
    {
      __IO uint32_t fprgm                : 1; /* [0] */
      __IO uint32_t secers               : 1; /* [1] */
      __IO uint32_t bankers              : 1; /* [2] */
      __IO uint32_t reserved1            : 3; /* [5:3] */
      __IO uint32_t erstr                : 1; /* [6] */
      __IO uint32_t oplk                 : 1; /* [7] */
      __IO uint32_t reserved2            : 2; /* [9:8] */
      __IO uint32_t errie                : 1; /* [10] */
      __IO uint32_t reserved3            : 1; /* [11] */
      __IO uint32_t odfie                : 1; /* [12] */
      __IO uint32_t reserved4            : 19;/* [31:13] */
    } ctrl2_bit;
  };

  /**
    * @brief flash addr2 register, offset:0x54
    */
  union
  {
    __IO uint32_t addr2;
    struct
    {
      __IO uint32_t fa                   : 32;/* [31:0] */
    } addr2_bit;
  };

  /**
    * @brief flash reserved4 register, offset:0x80~0x58
    */
  __IO uint32_t reserved4[11];

  /**
    * @brief flash unlock3 register, offset:0x84
    */
  union
  {
    __IO uint32_t unlock3;
    struct
    {
      __IO uint32_t ukval                : 32;/* [31:0] */
    } unlock3_bit;
  };

  /**
    * @brief flash select register, offset:0x88
    */
  union
  {
    __IO uint32_t select;
    struct
    {
      __IO uint32_t select               : 32;/* [31:0] */
    } select_bit;
  };

  /**
    * @brief flash sts3 register, offset:0x8C
    */
  union
  {
    __IO uint32_t sts3;
    struct
    {
      __IO uint32_t obf                  : 1; /* [0] */
      __IO uint32_t reserved1            : 1; /* [1] */
      __IO uint32_t prgmerr              : 1; /* [2] */
      __IO uint32_t reserved2            : 1; /* [3] */
      __IO uint32_t epperr               : 1; /* [4] */
      __IO uint32_t odf                  : 1; /* [5] */
      __IO uint32_t reserved3            : 26;/* [31:6] */
    } sts3_bit;
  };

  /**
    * @brief flash ctrl3 register, offset:0x90
    */
  union
  {
    __IO uint32_t ctrl3;
    struct
    {
      __IO uint32_t fprgm                : 1; /* [0] */
      __IO uint32_t secers               : 1; /* [1] */
      __IO uint32_t chpers               : 1; /* [2] */
      __IO uint32_t reserved1            : 3; /* [5:3] */
      __IO uint32_t erstr                : 1; /* [6] */
      __IO uint32_t oplk                 : 1; /* [7] */
      __IO uint32_t reserved2            : 2; /* [9:8] */
      __IO uint32_t errie                : 1; /* [10] */
      __IO uint32_t reserved3            : 1; /* [11] */
      __IO uint32_t odfie                : 1; /* [12] */
      __IO uint32_t reserved4            : 19;/* [31:13] */
    } ctrl3_bit;
  };

  /**
    * @brief flash addr3 register, offset:0x94
    */
  union
  {
    __IO uint32_t addr3;
    struct
    {
      __IO uint32_t fa                   : 32;/* [31:0] */
    } addr3_bit;
  };

  /**
    * @brief flash da register, offset:0x98
    */
  union
  {
    __IO uint32_t da;
    struct
    {
      __IO uint32_t fda                  : 32;/* [31:0] */
    } da_bit;
  };

  /**
    * @brief flash reserved5 register, offset:0xC8~0x9C
    */
  __IO uint32_t reserved5[12];

  /**
    * @brief flash slib_sts0 register, offset:0xCC
    */
  union
  {
    __IO uint32_t slib_sts0;
    struct
    {
      __IO uint32_t reserved1            : 3; /* [2:0] */
      __IO uint32_t slib_enf             : 1; /* [3] */
      __IO uint32_t reserved2            : 28;/* [31:4] */
    } slib_sts0_bit;
  };

  /**
    * @brief flash slib_sts1 register, offset:0xD0
    */
  union
  {
    __IO uint32_t slib_sts1;
    struct
    {
      __IO uint32_t slib_ss              : 11;/* [10:0] */
      __IO uint32_t slib_dat_ss          : 11;/* [21:11] */
      __IO uint32_t slib_es              : 10;/* [31:22] */
    } slib_sts1_bit;
  };

  /**
    * @brief flash slib_pwd_clr register, offset:0xD4
    */
  union
  {
    __IO uint32_t slib_pwd_clr;
    struct
    {
      __IO uint32_t slib_pclr_val        : 32;/* [31:0] */
    } slib_pwd_clr_bit;
  };

  /**
    * @brief flash slib_misc_sts register, offset:0xD8
    */
  union
  {
    __IO uint32_t slib_misc_sts;
    struct
    {
      __IO uint32_t slib_pwd_err         : 1; /* [0] */
      __IO uint32_t slib_pwd_ok          : 1; /* [1] */
      __IO uint32_t slib_ulkf            : 1; /* [2] */
      __IO uint32_t reserved1            : 13;/* [15:3] */
      __IO uint32_t slib_rcnt            : 9; /* [24:16] */
      __IO uint32_t reserved2            : 7; /* [31:25] */
    } slib_misc_sts_bit;
  };

  /**
    * @brief flash slib_set_pwd register, offset:0xDC
    */
  union
  {
    __IO uint32_t slib_set_pwd;
    struct
    {
      __IO uint32_t slib_pset_val        : 32;/* [31:0] */
    } slib_set_pwd_bit;
  };

  /**
    * @brief flash slib_set_range register, offset:0xE0
    */
  union
  {
    __IO uint32_t slib_set_range;
    struct
    {
      __IO uint32_t slib_ss_set          : 11;/* [10:0] */
      __IO uint32_t slib_dss_set         : 11;/* [21:11] */
      __IO uint32_t slib_es_set          : 10;/* [31:22] */
    } slib_set_range_bit;
  };

  /**
    * @brief flash reserved6 register, offset:0xEC~0xE4
    */
  __IO uint32_t reserved6[3];

  /**
    * @brief flash slib_unlock register, offset:0xF0
    */
  union
  {
    __IO uint32_t slib_unlock;
    struct
    {
      __IO uint32_t slib_ukval           : 32;/* [31:0] */
    } slib_unlock_bit;
  };

  /**
    * @brief flash crc_ctrl register, offset:0xF4
    */
  union
  {
    __IO uint32_t crc_ctrl;
    struct
    {
      __IO uint32_t crc_ss               : 12;/* [11:0] */
      __IO uint32_t crc_sn               : 12;/* [23:12] */
      __IO uint32_t reserved1            : 7; /* [30:24] */
      __IO uint32_t crc_strt             : 1; /* [31] */
    } crc_ctrl_bit;
  };

  /**
    * @brief flash crc_chkr register, offset:0xF8
    */
  union
  {
    __IO uint32_t crc_chkr;
    struct
    {
      __IO uint32_t crc_chkr             : 32;/* [31:0] */
    } crc_chkr_bit;
  };

} flash_type;

/**
  * @brief user system data
  */
typedef struct
{
  __IO uint16_t fap;
  __IO uint16_t ssb;
  __IO uint16_t data0;
  __IO uint16_t data1;
  __IO uint16_t epp0;
  __IO uint16_t epp1;
  __IO uint16_t epp2;
  __IO uint16_t epp3;
  __IO uint16_t eopb0;
  __IO uint16_t reserved;
  __IO uint16_t data2;
  __IO uint16_t data3;
  __IO uint16_t data4;
  __IO uint16_t data5;
  __IO uint16_t data6;
  __IO uint16_t data7;
  __IO uint16_t ext_flash_key[8];
} usd_type;

/**
  * @}
  */

#define FLASH                            ((flash_type *) FLASH_REG_BASE)
#define USD                              ((usd_type *) USD_BASE)

/** @defgroup FLASH_exported_functions
  * @{
  */

flag_status flash_flag_get(uint32_t flash_flag);
void flash_flag_clear(uint32_t flash_flag);
flash_status_type flash_operation_status_get(void);
flash_status_type flash_bank1_operation_status_get(void);
flash_status_type flash_bank2_operation_status_get(void);
flash_status_type flash_spim_operation_status_get(void);
flash_status_type flash_operation_wait_for(uint32_t time_out);
flash_status_type flash_bank1_operation_wait_for(uint32_t time_out);
flash_status_type flash_bank2_operation_wait_for(uint32_t time_out);
flash_status_type flash_spim_operation_wait_for(uint32_t time_out);
void flash_unlock(void);
void flash_bank1_unlock(void);
void flash_bank2_unlock(void);
void flash_spim_unlock(void);
void flash_lock(void);
void flash_bank1_lock(void);
void flash_bank2_lock(void);
void flash_spim_lock(void);
flash_status_type flash_sector_erase(uint32_t sector_address);
flash_status_type flash_internal_all_erase(void);
flash_status_type flash_bank1_erase(void);
flash_status_type flash_bank2_erase(void);
flash_status_type flash_spim_all_erase(void);
flash_status_type flash_user_system_data_erase(void);
flash_status_type flash_word_program(uint32_t address, uint32_t data);
flash_status_type flash_halfword_program(uint32_t address, uint16_t data);
flash_status_type flash_byte_program(uint32_t address, uint8_t data);
flash_status_type flash_user_system_data_program(uint32_t address, uint8_t data);
flash_status_type flash_epp_set(uint32_t *sector_bits);
void flash_epp_status_get(uint32_t *sector_bits);
flash_status_type flash_fap_enable(confirm_state new_state);
flag_status flash_fap_status_get(void);
flash_status_type flash_ssb_set(uint8_t usd_ssb);
uint8_t flash_ssb_status_get(void);
void flash_interrupt_enable(uint32_t flash_int, confirm_state new_state);
void flash_spim_model_select(flash_spim_model_type mode);
void flash_spim_encryption_range_set(uint32_t decode_address);
flash_status_type flash_slib_enable(uint32_t pwd, uint16_t start_sector, uint16_t data_start_sector, uint16_t end_sector);
error_status flash_slib_disable(uint32_t pwd);
uint32_t flash_slib_remaining_count_get(void);
flag_status flash_slib_state_get(void);
uint16_t flash_slib_start_sector_get(void);
uint16_t flash_slib_datstart_sector_get(void);
uint16_t flash_slib_end_sector_get(void);
uint32_t flash_crc_calibrate(uint32_t start_sector, uint32_t sector_cnt);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
