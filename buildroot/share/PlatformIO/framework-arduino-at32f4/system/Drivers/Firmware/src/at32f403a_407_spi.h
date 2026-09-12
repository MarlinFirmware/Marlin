/**
  **************************************************************************
  * @file     at32f403a_407_spi.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 spi header file
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
#ifndef __AT32F403A_407_SPI_H
#define __AT32F403A_407_SPI_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup SPI
  * @{
  */

/**
  * @defgroup SPI_I2S_flags_definition
  * @brief spi i2s flag
  * @{
  */

#define SPI_I2S_RDBF_FLAG                0x0001 /*!< spi or i2s receive data buffer full flag */
#define SPI_I2S_TDBE_FLAG                0x0002 /*!< spi or i2s transmit data buffer empty flag */
#define I2S_ACS_FLAG                     0x0004 /*!< i2s audio channel state flag */
#define I2S_TUERR_FLAG                   0x0008 /*!< i2s transmitter underload error flag */
#define SPI_CCERR_FLAG                   0x0010 /*!< spi crc calculation error flag */
#define SPI_MMERR_FLAG                   0x0020 /*!< spi master mode error flag */
#define SPI_I2S_ROERR_FLAG               0x0040 /*!< spi or i2s receiver overflow error flag */
#define SPI_I2S_BF_FLAG                  0x0080 /*!< spi or i2s busy flag */

/**
  * @}
  */

/**
  * @defgroup SPI_I2S_interrupts_definition
  * @brief spi i2s interrupt
  * @{
  */

#define SPI_I2S_ERROR_INT                0x0020 /*!< error interrupt */
#define SPI_I2S_RDBF_INT                 0x0040 /*!< receive data buffer full interrupt */
#define SPI_I2S_TDBE_INT                 0x0080 /*!< transmit data buffer empty interrupt */

/**
  * @}
  */

/** @defgroup SPI_exported_types
  * @{
  */

/**
  * @brief spi frame bit num type
  */
typedef enum
{
  SPI_FRAME_8BIT                         = 0x00, /*!< 8-bit data frame format */
  SPI_FRAME_16BIT                        = 0x01  /*!< 16-bit data frame format */
} spi_frame_bit_num_type;

/**
  * @brief spi master/slave mode type
  */
typedef enum
{
  SPI_MODE_SLAVE                         = 0x00, /*!< select as slave mode */
  SPI_MODE_MASTER                        = 0x01  /*!< select as master mode */
} spi_master_slave_mode_type;

/**
  * @brief spi clock polarity (clkpol) type
  */
typedef enum
{
  SPI_CLOCK_POLARITY_LOW                 = 0x00, /*!< sck keeps low at idle state */
  SPI_CLOCK_POLARITY_HIGH                = 0x01  /*!< sck keeps high at idle state */
} spi_clock_polarity_type;

/**
  * @brief spi clock phase (clkpha) type
  */
typedef enum
{
  SPI_CLOCK_PHASE_1EDGE                  = 0x00, /*!< data capture start from the first clock edge */
  SPI_CLOCK_PHASE_2EDGE                  = 0x01  /*!< data capture start from the second clock edge */
} spi_clock_phase_type;

/**
  * @brief spi cs mode type
  */
typedef enum
{
  SPI_CS_HARDWARE_MODE                   = 0x00, /*!< cs is hardware mode */
  SPI_CS_SOFTWARE_MODE                   = 0x01  /*!< cs is software mode */
} spi_cs_mode_type;

/**
  * @brief spi master clock frequency division type
  */
typedef enum
{
  SPI_MCLK_DIV_2                        = 0x00, /*!< master clock frequency division 2 */
  SPI_MCLK_DIV_4                        = 0x01, /*!< master clock frequency division 4 */
  SPI_MCLK_DIV_8                        = 0x02, /*!< master clock frequency division 8 */
  SPI_MCLK_DIV_16                       = 0x03, /*!< master clock frequency division 16 */
  SPI_MCLK_DIV_32                       = 0x04, /*!< master clock frequency division 32 */
  SPI_MCLK_DIV_64                       = 0x05, /*!< master clock frequency division 64 */
  SPI_MCLK_DIV_128                      = 0x06, /*!< master clock frequency division 128 */
  SPI_MCLK_DIV_256                      = 0x07, /*!< master clock frequency division 256 */
  SPI_MCLK_DIV_512                      = 0x08, /*!< master clock frequency division 512 */
  SPI_MCLK_DIV_1024                     = 0x09  /*!< master clock frequency division 1024 */
} spi_mclk_freq_div_type;

/**
  * @brief spi transmit first bit (lsb/msb) type
  */
typedef enum
{
  SPI_FIRST_BIT_MSB                      = 0x00, /*!< the frame format is msb first */
  SPI_FIRST_BIT_LSB                      = 0x01  /*!< the frame format is lsb first */
} spi_first_bit_type;

/**
  * @brief spi transmission mode type
  */
typedef enum
{
  SPI_TRANSMIT_FULL_DUPLEX               = 0x00, /*!< dual line unidirectional full-duplex mode(slben = 0 and ora = 0) */
  SPI_TRANSMIT_SIMPLEX_RX                = 0x01, /*!< dual line unidirectional simplex receive-only mode(slben = 0 and ora = 1) */
  SPI_TRANSMIT_HALF_DUPLEX_RX            = 0x02, /*!< single line bidirectional half duplex mode-receiving(slben = 1 and slbtd = 0) */
  SPI_TRANSMIT_HALF_DUPLEX_TX            = 0x03  /*!< single line bidirectional half duplex mode-transmitting(slben = 1 and slbtd = 1) */
} spi_transmission_mode_type;

/**
  * @brief spi crc direction type
  */
typedef enum
{
  SPI_CRC_RX                             = 0x0014, /*!< crc direction is rx */
  SPI_CRC_TX                             = 0x0018  /*!< crc direction is tx */
} spi_crc_direction_type;

/**
  * @brief spi single line bidirectional direction type
  */
typedef enum
{
  SPI_HALF_DUPLEX_DIRECTION_RX           = 0x00, /*!< single line bidirectional half duplex mode direction: receive(slbtd = 0) */
  SPI_HALF_DUPLEX_DIRECTION_TX           = 0x01  /*!< single line bidirectional half duplex mode direction: transmit(slbtd = 1) */
} spi_half_duplex_direction_type;

/**
  * @brief spi software cs internal level type
  */
typedef enum
{
  SPI_SWCS_INTERNAL_LEVEL_LOW            = 0x00, /*!< internal level low */
  SPI_SWCS_INTERNAL_LEVEL_HIGHT          = 0x01  /*!< internal level high */
} spi_software_cs_level_type;

/**
  * @brief i2s audio protocol type
  */
typedef enum
{
  I2S_AUDIO_PROTOCOL_PHILLIPS            = 0x00, /*!< i2s philip standard */
  I2S_AUDIO_PROTOCOL_MSB                 = 0x01, /*!< msb-justified standard */
  I2S_AUDIO_PROTOCOL_LSB                 = 0x02, /*!< lsb-justified standard */
  I2S_AUDIO_PROTOCOL_PCM_SHORT           = 0x03, /*!< pcm standard-short frame */
  I2S_AUDIO_PROTOCOL_PCM_LONG            = 0x04  /*!< pcm standard-long frame */
} i2s_audio_protocol_type;

/**
  * @brief i2s audio frequency type
  */
typedef enum
{
  I2S_AUDIO_FREQUENCY_DEFAULT            = 2,     /*!< i2s audio sampling frequency default */
  I2S_AUDIO_FREQUENCY_8K                 = 8000,  /*!< i2s audio sampling frequency 8k */
  I2S_AUDIO_FREQUENCY_11_025K            = 11025, /*!< i2s audio sampling frequency 11.025k */
  I2S_AUDIO_FREQUENCY_16K                = 16000, /*!< i2s audio sampling frequency 16k */
  I2S_AUDIO_FREQUENCY_22_05K             = 22050, /*!< i2s audio sampling frequency 22.05k */
  I2S_AUDIO_FREQUENCY_32K                = 32000, /*!< i2s audio sampling frequency 32k */
  I2S_AUDIO_FREQUENCY_44_1K              = 44100, /*!< i2s audio sampling frequency 44.1k */
  I2S_AUDIO_FREQUENCY_48K                = 48000, /*!< i2s audio sampling frequency 48k */
  I2S_AUDIO_FREQUENCY_96K                = 96000, /*!< i2s audio sampling frequency 96k */
  I2S_AUDIO_FREQUENCY_192K               = 192000 /*!< i2s audio sampling frequency 192k */
} i2s_audio_sampling_freq_type;

/**
  * @brief i2s data bit num and channel bit num type
  */
typedef enum
{
  I2S_DATA_16BIT_CHANNEL_16BIT           = 0x01, /*!< 16-bit data packed in 16-bit channel frame */
  I2S_DATA_16BIT_CHANNEL_32BIT           = 0x02, /*!< 16-bit data packed in 32-bit channel frame */
  I2S_DATA_24BIT_CHANNEL_32BIT           = 0x03, /*!< 24-bit data packed in 32-bit channel frame */
  I2S_DATA_32BIT_CHANNEL_32BIT           = 0x04  /*!< 32-bit data packed in 32-bit channel frame */
} i2s_data_channel_format_type;

/**
  * @brief i2s operation mode type
  */
typedef enum
{
  I2S_MODE_SLAVE_TX                      = 0x00, /*!< slave transmission mode */
  I2S_MODE_SLAVE_RX                      = 0x01, /*!< slave reception mode */
  I2S_MODE_MASTER_TX                     = 0x02, /*!< master transmission mode */
  I2S_MODE_MASTER_RX                     = 0x03  /*!< master reception mode */
} i2s_operation_mode_type;

/**
  * @brief i2s clock polarity type
  */
typedef enum
{
  I2S_CLOCK_POLARITY_LOW                 = 0x00, /*!< i2s clock steady state is low level */
  I2S_CLOCK_POLARITY_HIGH                = 0x01  /*!< i2s clock steady state is high level */
} i2s_clock_polarity_type;

/**
  * @brief spi init type
  */
typedef struct
{
  spi_transmission_mode_type             transmission_mode;     /*!< transmission mode selection */
  spi_master_slave_mode_type             master_slave_mode;     /*!< master or slave mode selection */
  spi_mclk_freq_div_type                 mclk_freq_division;    /*!< master clock frequency division selection */
  spi_first_bit_type                     first_bit_transmission;/*!< transmit lsb or msb selection */
  spi_frame_bit_num_type                 frame_bit_num;         /*!< frame bit num 8 or 16 bit selection */
  spi_clock_polarity_type                clock_polarity;        /*!< clock polarity selection */
  spi_clock_phase_type                   clock_phase;           /*!< clock phase selection */
  spi_cs_mode_type                       cs_mode_selection;     /*!< hardware or software cs mode selection */
} spi_init_type;

/**
  * @brief i2s init type
  */
typedef struct
{
  i2s_operation_mode_type                operation_mode;        /*!< operation mode selection */
  i2s_audio_protocol_type                audio_protocol;        /*!< audio protocol selection */
  i2s_audio_sampling_freq_type           audio_sampling_freq;   /*!< audio frequency selection */
  i2s_data_channel_format_type           data_channel_format;   /*!< data bit num and channel bit num selection */
  i2s_clock_polarity_type                clock_polarity;        /*!< clock polarity selection */
  confirm_state                          mclk_output_enable;    /*!< mclk_output selection */
} i2s_init_type;

/**
  * @brief type define spi register all
  */
typedef struct
{

  /**
    * @brief spi ctrl1 register, offset:0x00
    */
  union
  {
    __IO uint32_t ctrl1;
    struct
    {
      __IO uint32_t clkpha               : 1; /* [0] */
      __IO uint32_t clkpol               : 1; /* [1] */
      __IO uint32_t msten                : 1; /* [2] */
      __IO uint32_t mdiv_l               : 3; /* [5:3] */
      __IO uint32_t spien                : 1; /* [6] */
      __IO uint32_t ltf                  : 1; /* [7] */
      __IO uint32_t swcsil               : 1; /* [8] */
      __IO uint32_t swcsen               : 1; /* [9] */
      __IO uint32_t ora                  : 1; /* [10] */
      __IO uint32_t fbn                  : 1; /* [11] */
      __IO uint32_t ntc                  : 1; /* [12] */
      __IO uint32_t ccen                 : 1; /* [13] */
      __IO uint32_t slbtd                : 1; /* [14] */
      __IO uint32_t slben                : 1; /* [15] */
      __IO uint32_t reserved1            : 16;/* [31:16] */
    } ctrl1_bit;
  };

  /**
    * @brief spi ctrl2 register, offset:0x04
    */
  union
  {
    __IO uint32_t ctrl2;
    struct
    {
      __IO uint32_t dmaren               : 1; /* [0] */
      __IO uint32_t dmaten               : 1; /* [1] */
      __IO uint32_t hwcsoe               : 1; /* [2] */
      __IO uint32_t reserved1            : 2; /* [4:3] */
      __IO uint32_t errie                : 1; /* [5] */
      __IO uint32_t rdbfie               : 1; /* [6] */
      __IO uint32_t tdbeie               : 1; /* [7] */
      __IO uint32_t mdiv_h               : 1; /* [8] */
      __IO uint32_t reserved2            : 23;/* [31:9] */
    } ctrl2_bit;
  };

  /**
    * @brief spi sts register, offset:0x08
    */
  union
  {
    __IO uint32_t sts;
    struct
    {
      __IO uint32_t rdbf                 : 1; /* [0] */
      __IO uint32_t tdbe                 : 1; /* [1] */
      __IO uint32_t acs                  : 1; /* [2] */
      __IO uint32_t tuerr                : 1; /* [3] */
      __IO uint32_t ccerr                : 1; /* [4] */
      __IO uint32_t mmerr                : 1; /* [5] */
      __IO uint32_t roerr                : 1; /* [6] */
      __IO uint32_t bf                   : 1; /* [7] */
      __IO uint32_t reserved1            : 24;/* [31:8] */
    } sts_bit;
  };

  /**
    * @brief spi dt register, offset:0x0C
    */
  union
  {
    __IO uint32_t dt;
    struct
    {
      __IO uint32_t dt                  : 16;/* [15:0] */
      __IO uint32_t reserved1           : 16;/* [31:16] */
    } dt_bit;
  };

  /**
    * @brief spi cpoly register, offset:0x10
    */
  union
  {
    __IO uint32_t cpoly;
    struct
    {
      __IO uint32_t cpoly               : 16;/* [15:0] */
      __IO uint32_t reserved1           : 16;/* [31:16] */
    } cpoly_bit;
  };

  /**
    * @brief spi rcrc register, offset:0x14
    */
  union
  {
    __IO uint32_t rcrc;
    struct
    {
      __IO uint32_t rcrc                : 16;/* [15:0] */
      __IO uint32_t reserved1           : 16;/* [31:16] */
    } rcrc_bit;
  };

  /**
    * @brief spi tcrc register, offset:0x18
    */
  union
  {
    __IO uint32_t tcrc;
    struct
    {
      __IO uint32_t tcrc                : 16;/* [15:0] */
      __IO uint32_t reserved1           : 16;/* [31:16] */
    } tcrc_bit;
  };

  /**
    * @brief spi i2sctrl register, offset:0x1C
    */
  union
  {
    __IO uint32_t i2sctrl;
    struct
    {
      __IO uint32_t i2scbn              : 1; /* [0] */
      __IO uint32_t i2sdbn              : 2; /* [2:1] */
      __IO uint32_t i2sclkpol           : 1; /* [3] */
      __IO uint32_t stdsel              : 2; /* [5:4] */
      __IO uint32_t reserved1           : 1; /* [6] */
      __IO uint32_t pcmfssel            : 1; /* [7] */
      __IO uint32_t opersel             : 2; /* [9:8] */
      __IO uint32_t i2sen               : 1; /* [10] */
      __IO uint32_t i2smsel             : 1; /* [11] */
      __IO uint32_t reserved2           : 20;/* [31:12] */
    } i2sctrl_bit;
  };

  /**
    * @brief spi i2sclk register, offset:0x20
    */
  union
  {
    __IO uint32_t i2sclk;
    struct
    {
      __IO uint32_t i2sdiv_l            : 8; /* [7:0] */
      __IO uint32_t i2sodd              : 1; /* [8] */
      __IO uint32_t i2smclkoe           : 1; /* [9] */
      __IO uint32_t i2sdiv_h            : 2; /* [11:10] */
      __IO uint32_t reserved1           : 20;/* [31:12] */
    } i2sclk_bit;
  };

} spi_type;

/**
  * @}
  */

#define SPI1                            ((spi_type *) SPI1_BASE)
#define SPI2                            ((spi_type *) SPI2_BASE)
#define SPI3                            ((spi_type *) SPI3_BASE)
#define SPI4                            ((spi_type *) SPI4_BASE)
#define I2S2EXT                         ((spi_type *) I2S2EXT_BASE)
#define I2S3EXT                         ((spi_type *) I2S3EXT_BASE)

/** @defgroup SPI_exported_functions
  * @{
  */

void spi_i2s_reset(spi_type *spi_x);
void spi_default_para_init(spi_init_type* spi_init_struct);
void spi_init(spi_type* spi_x, spi_init_type* spi_init_struct);
void spi_crc_next_transmit(spi_type* spi_x);
void spi_crc_polynomial_set(spi_type* spi_x, uint16_t crc_poly);
uint16_t spi_crc_polynomial_get(spi_type* spi_x);
void spi_crc_enable(spi_type* spi_x, confirm_state new_state);
uint16_t spi_crc_value_get(spi_type* spi_x, spi_crc_direction_type crc_direction);
void spi_hardware_cs_output_enable(spi_type* spi_x, confirm_state new_state);
void spi_software_cs_internal_level_set(spi_type* spi_x, spi_software_cs_level_type level);
void spi_frame_bit_num_set(spi_type* spi_x, spi_frame_bit_num_type bit_num);
void spi_half_duplex_direction_set(spi_type* spi_x, spi_half_duplex_direction_type direction);
void spi_enable(spi_type* spi_x, confirm_state new_state);
void i2s_default_para_init(i2s_init_type* i2s_init_struct);
void i2s_init(spi_type* spi_x, i2s_init_type* i2s_init_struct);
void i2s_enable(spi_type* spi_x, confirm_state new_state);
void spi_i2s_interrupt_enable(spi_type* spi_x, uint32_t spi_i2s_int, confirm_state new_state);
void spi_i2s_dma_transmitter_enable(spi_type* spi_x, confirm_state new_state);
void spi_i2s_dma_receiver_enable(spi_type* spi_x, confirm_state new_state);
void spi_i2s_data_transmit(spi_type* spi_x, uint16_t tx_data);
uint16_t spi_i2s_data_receive(spi_type* spi_x);
flag_status spi_i2s_flag_get(spi_type* spi_x, uint32_t spi_i2s_flag);
void spi_i2s_flag_clear(spi_type* spi_x, uint32_t spi_i2s_flag);

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
