/**
  **************************************************************************
  * @file     at32f403a_407_i2c.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 i2c header file
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32F403A_407_I2C_H
#define __AT32F403A_407_I2C_H

#ifdef __cplusplus
extern "C" {
#endif


/* includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */

/** @defgroup I2C_sts1_flags_definition
  * @brief  i2c sts1 flag
  * @{
  */

#define I2C_STARTF_FLAG                  ((uint32_t)0x00000001) /*!< i2c start condition generation complete flag */
#define I2C_ADDR7F_FLAG                  ((uint32_t)0x00000002) /*!< i2c 0~7 bit address match flag */
#define I2C_TDC_FLAG                     ((uint32_t)0x00000004) /*!< i2c transmit data complete flag */
#define I2C_ADDRHF_FLAG                  ((uint32_t)0x00000008) /*!< i2c master 9~8 bit address header match flag */
#define I2C_STOPF_FLAG                   ((uint32_t)0x00000010) /*!< i2c stop condition generation complete flag */
#define I2C_RDBF_FLAG                    ((uint32_t)0x00000040) /*!< i2c receive data buffer full flag */
#define I2C_TDBE_FLAG                    ((uint32_t)0x00000080) /*!< i2c transmit data buffer empty flag */
#define I2C_BUSERR_FLAG                  ((uint32_t)0x00000100) /*!< i2c bus error flag */
#define I2C_ARLOST_FLAG                  ((uint32_t)0x00000200) /*!< i2c arbitration lost flag */
#define I2C_ACKFAIL_FLAG                 ((uint32_t)0x00000400) /*!< i2c acknowledge failure flag */
#define I2C_OUF_FLAG                     ((uint32_t)0x00000800) /*!< i2c overflow or underflow flag */
#define I2C_PECERR_FLAG                  ((uint32_t)0x00001000) /*!< i2c pec receive error flag */
#define I2C_TMOUT_FLAG                   ((uint32_t)0x00004000) /*!< i2c smbus timeout flag */
#define I2C_ALERTF_FLAG                  ((uint32_t)0x00008000) /*!< i2c smbus alert flag */

/**
  * @}
  */

/** @defgroup I2C_sts2_flags_definition
  * @brief  i2c sts2 flag
  * @{
  */

#define I2C_TRMODE_FLAG                  ((uint32_t)0x10010000) /*!< i2c transmission mode */
#define I2C_BUSYF_FLAG                   ((uint32_t)0x10020000) /*!< i2c bus busy flag transmission mode */
#define I2C_DIRF_FLAG                    ((uint32_t)0x10040000) /*!< i2c transmission direction flag */
#define I2C_GCADDRF_FLAG                 ((uint32_t)0x10100000) /*!< i2c general call address received flag */
#define I2C_DEVADDRF_FLAG                ((uint32_t)0x10200000) /*!< i2c smbus device address received flag */
#define I2C_HOSTADDRF_FLAG               ((uint32_t)0x10400000) /*!< i2c smbus host address received flag */
#define I2C_ADDR2_FLAG                   ((uint32_t)0x10800000) /*!< i2c own address 2 received flag */

/**
  * @}
  */

/** @defgroup I2C_interrupts_definition
  * @brief i2c interrupt
  * @{
  */

#define I2C_DATA_INT                     ((uint16_t)0x0400) /*!< i2c data transmission interrupt */
#define I2C_EVT_INT                      ((uint16_t)0x0200) /*!< i2c event interrupt */
#define I2C_ERR_INT                      ((uint16_t)0x0100) /*!< i2c error interrupt */

/**
  * @}
  */

/** @defgroup I2C_exported_types
  * @{
  */

/**
  * @brief i2c master receiving mode acknowledge control
  */
typedef enum
{
  I2C_MASTER_ACK_CURRENT                 = 0x00, /*!< acken bit acts on the current byte */
  I2C_MASTER_ACK_NEXT                    = 0x01  /*!< acken bit acts on the next byte */
} i2c_master_ack_type;

/**
  * @brief i2c pec position set
  */
typedef enum
{
  I2C_PEC_POSITION_CURRENT               = 0x00, /*!< the current byte is pec */
  I2C_PEC_POSITION_NEXT                  = 0x01  /*!< the next byte is pec */
} i2c_pec_position_type;


/**
  * @brief i2c smbus alert pin set
  */
typedef enum
{
  I2C_SMBUS_ALERT_HIGH                   = 0x00, /*!< smbus alert pin set high */
  I2C_SMBUS_ALERT_LOW                    = 0x01  /*!< smbus alert pin set low */
} i2c_smbus_alert_set_type;

/**
  * @brief i2c smbus mode set
  */
typedef enum
{
  I2C_SMBUS_MODE_DEVICE                  = 0x00, /*!< smbus device mode */
  I2C_SMBUS_MODE_HOST                    = 0x01  /*!< smbus host mode */
} i2c_smbus_mode_set_type;


/**
  * @brief i2c fast mode duty cycle
  */
typedef enum
{
  I2C_FSMODE_DUTY_2_1                    = 0x00, /*!< duty cycle is 2:1 in fast mode */
  I2C_FSMODE_DUTY_16_9                   = 0x01  /*!< duty cycle is 16:9 in fast mode */
} i2c_fsmode_duty_cycle_type;

/**
  * @brief i2c address mode
  */
typedef enum
{
  I2C_ADDRESS_MODE_7BIT                  = 0x00, /*!< 7bit address mode */
  I2C_ADDRESS_MODE_10BIT                 = 0x01  /*!< 10bit address mode */
} i2c_address_mode_type;

/**
  * @brief i2c address direction
  */
typedef enum
{
  I2C_DIRECTION_TRANSMIT                 = 0x00, /*!< transmit mode */
  I2C_DIRECTION_RECEIVE                  = 0x01  /*!< receive mode */
} i2c_direction_type;

/**
  * @brief type define i2c register all
  */
typedef struct
{
  /**
    * @brief i2c ctrl1 register, offset:0x00
    */
  union
  {
    __IO uint32_t ctrl1;
    struct
    {
      __IO uint32_t i2cen                : 1; /* [0] */
      __IO uint32_t permode              : 1; /* [1] */
      __IO uint32_t reserved1            : 1; /* [2] */
      __IO uint32_t smbmode              : 1; /* [3] */
      __IO uint32_t arpen                : 1; /* [4] */
      __IO uint32_t pecen                : 1; /* [5] */
      __IO uint32_t gcaen                : 1; /* [6] */
      __IO uint32_t stretch              : 1; /* [7] */
      __IO uint32_t genstart             : 1; /* [8] */
      __IO uint32_t genstop              : 1; /* [9] */
      __IO uint32_t acken                : 1; /* [10] */
      __IO uint32_t mackctrl             : 1; /* [11] */
      __IO uint32_t pecten               : 1; /* [12] */
      __IO uint32_t smbalert             : 1; /* [13] */
      __IO uint32_t reserved2            : 1; /* [14] */
      __IO uint32_t reset                : 1; /* [15] */
      __IO uint32_t reserved3            : 16;/* [31:16] */
    } ctrl1_bit;
  };

  /**
    * @brief i2c ctrl2 register, offset:0x04
    */
  union
  {
    __IO uint32_t ctrl2;
    struct
    {
      __IO uint32_t clkfreq              : 8; /* [7:0] */
      __IO uint32_t errien               : 1; /* [8] */
      __IO uint32_t evtien               : 1; /* [9] */
      __IO uint32_t dataien              : 1; /* [10] */
      __IO uint32_t dmaen                : 1; /* [11] */
      __IO uint32_t dmaend               : 1; /* [12] */
      __IO uint32_t reserved1            : 19;/* [31:13] */
    } ctrl2_bit;
  };

  /**
    * @brief i2c oaddr1 register, offset:0x08
    */
  union
  {
    __IO uint32_t oaddr1;
    struct
    {
      __IO uint32_t addr1                : 10;/* [9:0] */
      __IO uint32_t reserved1            : 5; /* [14:10] */
      __IO uint32_t addr1mode            : 1; /* [15] */
      __IO uint32_t reserved2            : 16;/* [31:16] */
    } oaddr1_bit;
  };

  /**
    * @brief i2c oaddr2 register, offset:0x0C
    */
  union
  {
    __IO uint32_t oaddr2;
    struct
    {
      __IO uint32_t addr2en              : 1; /* [0] */
      __IO uint32_t addr2                : 7; /* [7:1] */
      __IO uint32_t reserved1            : 24;/* [31:8] */
    } oaddr2_bit;
  };

  /**
    * @brief i2c dt register, offset:0x10
    */
  union
  {
    __IO uint32_t dt;
    struct
    {
      __IO uint32_t dt                   : 8; /* [7:0] */
      __IO uint32_t reserved1            : 24;/* [31:8] */
    } dt_bit;
  };

  /**
    * @brief i2c sts1 register, offset:0x14
    */
  union
  {
    __IO uint32_t sts1;
    struct
    {
      __IO uint32_t startf               : 1; /* [0] */
      __IO uint32_t addr7f               : 1; /* [1] */
      __IO uint32_t tdc                  : 1; /* [2] */
      __IO uint32_t addrhf               : 1; /* [3] */
      __IO uint32_t stopf                : 1; /* [4] */
      __IO uint32_t reserved1            : 1; /* [5] */
      __IO uint32_t rdbf                 : 1; /* [6] */
      __IO uint32_t tdbe                 : 1; /* [7] */
      __IO uint32_t buserr               : 1; /* [8] */
      __IO uint32_t arlost               : 1; /* [9] */
      __IO uint32_t ackfail              : 1; /* [10] */
      __IO uint32_t ouf                  : 1; /* [11] */
      __IO uint32_t pecerr               : 1; /* [12] */
      __IO uint32_t reserved2            : 1; /* [13] */
      __IO uint32_t tmout                : 1; /* [14] */
      __IO uint32_t alertf               : 1; /* [15] */
      __IO uint32_t reserved3            : 16; /* [31:16] */
    } sts1_bit;
  };

  /**
    * @brief i2c sts2 register, offset:0x18
    */
  union
  {
    __IO uint32_t sts2;
    struct
    {
      __IO uint32_t trmode               : 1; /* [0] */
      __IO uint32_t busyf                : 1; /* [1] */
      __IO uint32_t dirf                 : 1; /* [2] */
      __IO uint32_t reserved1            : 1; /* [3] */
      __IO uint32_t gcaddrf              : 1; /* [4] */
      __IO uint32_t devaddrf             : 1; /* [5] */
      __IO uint32_t hostaddrf            : 1; /* [6] */
      __IO uint32_t addr2                : 1; /* [7] */
      __IO uint32_t pecval               : 8; /* [15:8] */
      __IO uint32_t reserved2            : 16;/* [31:16] */
    } sts2_bit;
  };

  /**
    * @brief i2c clkctrl register, offset:0x1C
    */
  union
  {
    __IO uint32_t clkctrl;
    struct
    {
      __IO uint32_t speed                : 12;/* [11:0] */
      __IO uint32_t reserved1            : 2; /* [13:12] */
      __IO uint32_t dutymode             : 1; /* [14] */
      __IO uint32_t speedmode            : 1; /* [15] */
      __IO uint32_t reserved2            : 16;/* [31:16] */
    } clkctrl_bit;
  };

  /**
    * @brief i2c tmrise register, offset:0x20
    */
  union
  {
    __IO uint32_t tmrise;
    struct
    {
      __IO uint32_t risetime             : 6; /* [5:0] */
      __IO uint32_t reserved1            : 26;/* [31:6] */
    } tmrise_bit;
  };

} i2c_type;

/**
  * @}
  */

#define I2C1                             ((i2c_type *) I2C1_BASE)
#define I2C2                             ((i2c_type *) I2C2_BASE)
#define I2C3                             ((i2c_type *) I2C3_BASE)

/** @defgroup I2C_exported_functions
  * @{
  */

void i2c_reset(i2c_type *i2c_x);
void i2c_software_reset(i2c_type *i2c_x, confirm_state new_state);
void i2c_init(i2c_type *i2c_x, i2c_fsmode_duty_cycle_type duty, uint32_t speed);
void i2c_own_address1_set(i2c_type *i2c_x, i2c_address_mode_type mode, uint16_t address);
void i2c_own_address2_set(i2c_type *i2c_x, uint8_t address);
void i2c_own_address2_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_smbus_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_fast_mode_duty_set(i2c_type *i2c_x, i2c_fsmode_duty_cycle_type duty);
void i2c_clock_stretch_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_ack_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_master_receive_ack_set(i2c_type *i2c_x, i2c_master_ack_type pos);
void i2c_pec_position_set(i2c_type *i2c_x, i2c_pec_position_type pos);
void i2c_general_call_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_arp_mode_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_smbus_mode_set(i2c_type *i2c_x, i2c_smbus_mode_set_type mode);
void i2c_smbus_alert_set(i2c_type *i2c_x, i2c_smbus_alert_set_type level);
void i2c_pec_transmit_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_pec_calculate_enable(i2c_type *i2c_x, confirm_state new_state);
uint8_t i2c_pec_value_get(i2c_type *i2c_x);
void i2c_dma_end_transfer_set(i2c_type *i2c_x, confirm_state new_state);
void i2c_dma_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_interrupt_enable(i2c_type *i2c_x, uint16_t source, confirm_state new_state);
void i2c_start_generate(i2c_type *i2c_x);
void i2c_stop_generate(i2c_type *i2c_x);
void i2c_7bit_address_send(i2c_type *i2c_x, uint8_t address, i2c_direction_type direction);
void i2c_data_send(i2c_type *i2c_x, uint8_t data);
uint8_t i2c_data_receive(i2c_type *i2c_x);
flag_status i2c_flag_get(i2c_type *i2c_x, uint32_t flag);
void i2c_flag_clear(i2c_type *i2c_x, uint32_t flag);

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
