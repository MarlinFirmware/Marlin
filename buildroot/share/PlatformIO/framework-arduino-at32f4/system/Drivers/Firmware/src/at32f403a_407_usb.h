/**
  **************************************************************************
  * @file     at32f403a_407_usb.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 usb header file
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

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup USB
  * @{
  */

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32F403A_407_USB_H
#define __AT32F403A_407_USB_H

#ifdef __cplusplus
extern "C" {
#endif


/* includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"



/** @defgroup USB_interrupts_definition
  * @brief usb interrupt mask
  * @{
  */

#define USB_LSOF_INT                     ((uint32_t)0x00000100) /*!< usb lost sof interrupt */
#define USB_SOF_INT                      ((uint32_t)0x00000200) /*!< usb sof interrupt */
#define USB_RST_INT                      ((uint32_t)0x00000400) /*!< usb reset interrupt */
#define USB_SP_INT                       ((uint32_t)0x00000800) /*!< usb suspend interrupt */
#define USB_WK_INT                       ((uint32_t)0x00001000) /*!< usb wakeup interrupt */
#define USB_BE_INT                       ((uint32_t)0x00002000) /*!< usb bus error interrupt */
#define USB_UCFOR_INT                    ((uint32_t)0x00004000) /*!< usb core fifo overrun interrupt */
#define USB_TC_INT                       ((uint32_t)0x00008000) /*!< usb transmission completed interrupt */

/**
  * @}
  */

/** @defgroup USB_interrupt_flags_definition
  * @brief usb interrupt flag
  * @{
  */

#define USB_EPT_NUM_FLAG                 ((uint32_t)0x0000000F) /*!< usb endpoint number */
#define USB_INOUT_FLAG                   ((uint32_t)0x00000010) /*!< usb in/out transcation flag */
#define USB_LSOF_FLAG                    ((uint32_t)0x00000100) /*!< usb lost sof flag */
#define USB_SOF_FLAG                     ((uint32_t)0x00000200) /*!< usb sof flag */
#define USB_RST_FLAG                     ((uint32_t)0x00000400) /*!< usb reset flag */
#define USB_SP_FLAG                      ((uint32_t)0x00000800) /*!< usb suspend flag */
#define USB_WK_FLAG                      ((uint32_t)0x00001000) /*!< usb wakeup flag */
#define USB_BE_FLAG                      ((uint32_t)0x00002000) /*!< usb bus error flag */
#define USB_UCFOR_FLAG                   ((uint32_t)0x00004000) /*!< usb core fifo overrun flag */
#define USB_TC_FLAG                      ((uint32_t)0x00008000) /*!< usb transmission completed flag */

/**
  * @}
  */

/** @defgroup USB_endpoint_register_bit_definition
  * @brief usb endpoint register bit define
  * @{
  */

#define USB_EPTADDR                      ((uint32_t)0x0000000F) /*!< usb endpoint address */
#define USB_TXSTS                        ((uint32_t)0x00000030) /*!< usb tx status */
#define USB_TXDTS                        ((uint32_t)0x00000040) /*!< usb tx data toggle synchronization */
#define USB_TXTC                         ((uint32_t)0x00000080) /*!< usb tx transcation completed */
#define USB_EXF                          ((uint32_t)0x00000100) /*!< usb endpoint extend funtion */
#define USB_TRANS_TYPE                   ((uint32_t)0x00000600) /*!< usb transfer type */
#define USB_SETUPTC                      ((uint32_t)0x00000800) /*!< usb setup transcation completed */
#define USB_RXSTS                        ((uint32_t)0x00003000) /*!< usb rx status */
#define USB_RXDTS                        ((uint32_t)0x00004000) /*!< usb rx data toggle synchronization */
#define USB_RXTC                         ((uint32_t)0x00008000) /*!< usb rx transcation completed */

#define USB_EPT_BIT_MASK                 (uint32_t)(USB_TXTC | USB_SETUPTC | USB_EPTADDR | USB_EXF | USB_RXTC | USB_TRANS_TYPE) /*!< usb bit mask */
#define USB_TX_MASK                      (USB_TXSTS | USB_EPT_BIT_MASK) /*!< usb tx mask */
#define USB_RX_MASK                      (USB_RXSTS | USB_EPT_BIT_MASK) /*!< usb rx mask */

/**
  * @}
  */

/** @defgroup USB_endpoint_tx_and_rx_status_definition
  * @brief usb endpoint tx and rx status
  * @{
  */

#define USB_TX_DISABLE                   ((uint32_t)0x00000000) /*!< usb tx status disable */
#define USB_TX_STALL                     ((uint32_t)0x00000010) /*!< usb tx status stall */
#define USB_TX_NAK                       ((uint32_t)0x00000020) /*!< usb tx status nak */
#define USB_TX_VALID                     ((uint32_t)0x00000030) /*!< usb tx status valid */

#define USB_TXDTS0                       ((uint32_t)0x00000010) /*!< usb tx data toggle bit 0 */
#define USB_TXDTS1                       ((uint32_t)0x00000020) /*!< usb tx data toggle bit 1 */

#define USB_RX_DISABLE                   ((uint32_t)0x00000000) /*!< usb rx status disable */
#define USB_RX_STALL                     ((uint32_t)0x00001000) /*!< usb rx status stall */
#define USB_RX_NAK                       ((uint32_t)0x00002000) /*!< usb rx status nak */
#define USB_RX_VALID                     ((uint32_t)0x00003000) /*!< usb rx status valid */

#define USB_RXDTS0                       ((uint32_t)0x00001000) /*!< usb rx data toggle bit 0 */
#define USB_RXDTS1                       ((uint32_t)0x00002000) /*!< usb rx data toggle bit 1 */

/**
  * @}
  */

/** @defgroup USB_device_endpoint_register_type_definition
  * @brief usb device endpoint register type define
  * @{
  */

#define USB_EPT_CONTROL                  ((uint32_t)0x00000200) /*!< usb endpoint transfer type control */
#define USB_EPT_BULK                     ((uint32_t)0x00000000) /*!< usb endpoint transfer type bulk */
#define USB_EPT_INT                      ((uint32_t)0x00000600) /*!< usb endpoint transfer type interrupt */
#define USB_EPT_ISO                      ((uint32_t)0x00000400) /*!< usb endpoint transfer type iso */

/**
  * @}
  */

/** @defgroup USB_buffer_table_default_offset_address_definition
  * @brief usb buffer table default offset address
  * @{
  */

#define USB_BUFFER_TABLE_ADDRESS          0x0000 /*!< usb buffer table address */

/**
  * @}
  */

/** @defgroup USB_packet_buffer_start_address_definition
  * @brief usb packet buffer start address
  * @{
  */

#define USB_PACKET_BUFFER_ADDRESS         0x40006000 /*!< usb buffer address */
#define USB_PACKET_BUFFER_ADDRESS_EX      0x40007800 /*!< usb buffer extend address */

/**
  * @}
  */

/** @defgroup USB_exported_enum_types
  * @{
  */

/**
  * @brief usb endpoint number define
  */
typedef enum
{
  USB_EPT0                               = 0x00, /*!< usb endpoint 0 */
  USB_EPT1                               = 0x01, /*!< usb endpoint 1 */
  USB_EPT2                               = 0x02, /*!< usb endpoint 2 */
  USB_EPT3                               = 0x03, /*!< usb endpoint 3 */
  USB_EPT4                               = 0x04, /*!< usb endpoint 4 */
  USB_EPT5                               = 0x05, /*!< usb endpoint 5 */
  USB_EPT6                               = 0x06, /*!< usb endpoint 6 */
  USB_EPT7                               = 0x07  /*!< usb endpoint 7 */
} usb_ept_number_type;

/**
  * @brief usb endpoint max num define
  */
#ifndef USB_EPT_MAX_NUM
#define USB_EPT_MAX_NUM                   8  /*!< usb device support endpoint number */
#endif
/**
  * @brief endpoint transfer type define
  */
typedef enum
{
  EPT_CONTROL_TYPE                       = 0x00, /*!< usb transfer type control */
  EPT_BULK_TYPE                          = 0x01, /*!< usb transfer type bulk */
  EPT_INT_TYPE                           = 0x02, /*!< usb transfer type interrut */
  EPT_ISO_TYPE                           = 0x03  /*!< usb transfer type iso */
}ept_trans_type;

/**
  * @brief endpoint endpoint direction define
  */
typedef enum
{
  EPT_IN                                 = 0x00, /*!< usb endpoint direction in */
  EPT_OUT                                = 0x01  /*!< usb endpoint direction out */
}ept_inout_type;

/**
  * @brief data transfer direction
  */
typedef enum
{
  DATA_TRANS_OUT                         = 0x00, /*!< usb data transfer direction out */
  DATA_TRANS_IN                          = 0x01  /*!< usb data transfer direction in */
}data_trans_dir;

/**
  * @brief usb clock select
  */
typedef enum
{
  USB_CLK_HICK,
  USB_CLK_HEXT
}usb_clk48_s;
/**
  * @}
  */

/** @defgroup USB_macro_definition
  * @{
  */

/**
  * @brief  set usb endpoint tx status
  * @param  ept_num: endpoint number
  * @param  new_sts: the new tx status of this endpoint number
  * @retval none
  */
#define USB_SET_TXSTS(ept_num, new_sts) { \
  register uint16_t epsts = (USB->ept[ept_num]) & USB_TX_MASK; \
  if((new_sts & USB_TXDTS0) != 0)                    \
    epsts ^= USB_TXDTS0;                             \
  if((new_sts & USB_TXDTS1) != 0)                    \
    epsts ^= USB_TXDTS1;                             \
  USB->ept[ept_num] = epsts | USB_RXTC | USB_TXTC; \
}

/**
  * @brief  set usb endpoint rx status
  * @param  ept_num: endpoint number
  * @param  new_sts: the new rx status of this endpoint number
  * @retval none
  */
#define USB_SET_RXSTS(ept_num, new_sts) { \
  register uint16_t epsts = (USB->ept[ept_num]) & USB_RX_MASK; \
  if((new_sts & USB_RXDTS0) != 0) \
    epsts ^= USB_RXDTS0; \
  if((new_sts & USB_RXDTS1) != 0) \
    epsts ^= USB_RXDTS1; \
  USB->ept[ept_num] = epsts | USB_RXTC | USB_TXTC; \
}

/**
  * @brief  get usb endpoint tx/rx length address
  * @param  eptn: endpoint number
  * @retval the length address of tx/rx
  */
#define GET_TX_LEN_ADDR(eptn) (uint32_t *)((USB->buftbl + eptn * 8 + 2) * 2 + g_usb_packet_address)
#define GET_RX_LEN_ADDR(eptn) (uint32_t *)((USB->buftbl + eptn * 8 + 6) * 2 + g_usb_packet_address)

/**
  * @brief  get usb endpoint tx/rx data length
  * @param  eptn: endpoint number
  * @retval the length of tx/rx
  */
#define USB_GET_TX_LEN(eptn)  ((uint16_t)(*GET_TX_LEN_ADDR(eptn)) & 0x3ff)
#define USB_GET_RX_LEN(eptn)  ((uint16_t)(*GET_RX_LEN_ADDR(eptn)) & 0x3ff)

/**
  * @brief  double buffer mode get endpoint buf0/buf1 data length
  * @param  eptn: endpoint number
  * @retval the length of buf0/buf1
  */
#define USB_DBUF0_GET_LEN(eptn) USB_GET_TX_LEN(eptn)
#define USB_DBUF1_GET_LEN(eptn) USB_GET_RX_LEN(eptn)

/**
  * @brief  set usb length of rx buffer
  * @param  reg: usb rx length register
  * @param  len: rx max length
  * @param  blks: number of blocks
  */
#define BLK32(reg, len, blks) { \
  blks = (len) >> 5; \
  if(((len) & 0x1F) == 0) \
    blks --; \
  *reg = ((uint16_t)((blks) << 10) | 0x8000); \
}

#define BLK2(reg, len, blks) { \
  blks = (len) >> 1; \
  if(((len) & 0x1) == 0) \
    blks ++; \
  *reg = (uint16_t)((blks) << 10); \
}

#define USB_SET_RXLEN_REG(reg, len) { \
  uint16_t blks;                         \
  if(len > 62)                         \
  {                                      \
    BLK32(reg, len, blks);             \
  }                                      \
  else                                   \
  {                                      \
    BLK2(reg, len, blks);              \
  }                                      \
}

/**
  * @brief  set endpoint tx/rx transfer length
  * @param  eptn: endpoint number
  * @param  len: transfer length
  * @retval none
  */
#define USB_SET_TXLEN(eptn, len) (*(GET_TX_LEN_ADDR(eptn)) = (len))
#define USB_SET_RXLEN(eptn, len) { \
  uint32_t *rx_reg = GET_RX_LEN_ADDR(eptn); \
  USB_SET_RXLEN_REG(rx_reg, (len)); \
}

/**
  * @brief  double buffer mode set endpoint rx buf0 length
  * @param  eptn: endpoint number
  * @param  len: transfer length
  * @retval none
  */
#define USB_OUT_EPT_DOUBLE_BUF0(eptn, len) { \
  uint32_t *rx_reg = GET_TX_LEN_ADDR(eptn); \
  USB_SET_RXLEN_REG(rx_reg, (len)); \
}

/**
  * @brief  double buffer mode set endpoint buf0 length
  * @param  eptn: endpoint number
  * @param  len: transfer length
  * @param  dir: transfer direction(in/out)
  * @retval none
  */
#define USB_SET_EPT_DOUBLE_BUF0_LEN(eptn, len, dir) { \
  if(dir == DATA_TRANS_OUT) \
  { \
    USB_OUT_EPT_DOUBLE_BUF0(eptn, len); \
  } \
  else \
  { \
    *(GET_TX_LEN_ADDR(eptn)) = (len); \
  } \
}

/**
  * @brief  double buffer mode set endpoint buf1 length
  * @param  eptn: endpoint number
  * @param  len: transfer length
  * @param  dir: transfer direction(in/out)
  * @retval none
  */
#define USB_SET_EPT_DOUBLE_BUF1_LEN(eptn, len, dir) { \
  if(dir == DATA_TRANS_OUT) \
  { \
    USB_SET_RXLEN(eptn, len); \
  } \
  else \
  { \
    *(GET_RX_LEN_ADDR(eptn)) = (len); \
  } \
}

/**
  * @brief  set usb endpoint tx/rx fifo address
  * @param  eptn: endpoint number
  * @param  address: offset of the fifo address
  * @retval none
  */
#define USB_SET_TX_ADDRESS(eptn, address) (*(uint32_t *)((USB->buftbl + eptn * 8) * 2 + g_usb_packet_address) = address)
#define USB_SET_RX_ADDRESS(eptn, address) (*(uint32_t *)((USB->buftbl + eptn * 8 + 4) * 2 + g_usb_packet_address) = address)

/**
  * @brief  set double buffer mode usb endpoint buf0/buf1 fifo address
  * @param  eptn: endpoint number
  * @param  address: offset of the fifo address
  * @retval none
  */
#define USB_SET_DOUBLE_BUFF0_ADDRESS(eptn, address) (USB_SET_TX_ADDRESS(eptn, address))
#define USB_SET_DOUBLE_BUFF1_ADDRESS(eptn, address) (USB_SET_RX_ADDRESS(eptn, address))

/**
  * @brief  set usb tx/rx toggle
  * @param  eptn: endpoint number
  * @retval none
  */
#define USB_TOGGLE_TXDTS(eptn) (USB->ept[eptn] = ((USB->ept[eptn] & USB_EPT_BIT_MASK) | USB_TXDTS | USB_RXTC | USB_TXTC))
#define USB_TOGGLE_RXDTS(eptn) (USB->ept[eptn] = ((USB->ept[eptn] & USB_EPT_BIT_MASK) | USB_RXDTS | USB_RXTC | USB_TXTC))

/**
  * @brief  clear usb tx/rx toggle
  * @param  eptn: endpoint number
  * @retval none
  */
#define USB_CLEAR_TXDTS(eptn) { \
  if(USB->ept_bit[eptn].txdts != 0) \
    USB_TOGGLE_TXDTS(eptn); \
}
#define USB_CLEAR_RXDTS(eptn) { \
  if(USB->ept_bit[eptn].rxdts != 0) \
    USB_TOGGLE_RXDTS(eptn); \
}

/**
  * @brief set usb endpoint type
  */

/**
  * @brief  set usb transfer type
  * @param  eptn: endpoint number
  * @param  type: transfer type
  * @retval none
  */
#define USB_SET_TRANS_TYPE(eptn, type) (USB->ept[eptn] = (USB->ept[eptn] & USB_EPT_BIT_MASK & (~USB_TRANS_TYPE)) | type)

/**
  * @brief  set/clear usb extend function
  * @param  eptn: endpoint number
  * @retval none
  */
#define USB_SET_EXF(eptn) (USB->ept[eptn] = USB_TXTC | USB_RXTC | ((USB->ept[eptn] | USB_EXF) & USB_EPT_BIT_MASK))
#define USB_CLEAR_EXF(eptn) (USB->ept[eptn] = USB_TXTC | USB_RXTC | (USB->ept[eptn] & ((~USB_EXF) & USB_EPT_BIT_MASK)))

/**
  * @brief  set usb device address
  * @param  eptn: endpoint number
  * @param  address: device address
  * @retval none
  */
#define USB_SET_EPT_ADDRESS(eptn, address) (USB->ept[eptn] = ((USB->ept[eptn] & USB_EPT_BIT_MASK & (~USB_EPTADDR)) | address))

/**
  * @brief  free buffer used by application
  * @param  eptn: endpoint number
  * @param  inout: transfer direction
  * @retval none
  */
#define USB_FREE_DB_USER_BUFFER(eptn, inout) { \
  if(inout == DATA_TRANS_IN) \
  { \
    USB_TOGGLE_RXDTS(eptn); \
  } \
  else \
  { \
    USB_TOGGLE_TXDTS(eptn); \
  } \
}

/**
  * @brief  clear tx/rx transfer completed flag
  * @param  eptn: endpoint number
  * @retval none
  */
#define USB_CLEAR_TXTC(eptn)    (USB->ept[eptn] &= 0xFF7F & USB_EPT_BIT_MASK)
#define USB_CLEAR_RXTC(eptn)    (USB->ept[eptn] &= 0x7FFF & USB_EPT_BIT_MASK)

/**
  * @brief  set/clear endpoint double buffer mode
  * @param  eptn: endpoint number
  * @retval none
  */
#define USB_SET_EPT_DOUBLE_BUFFER(eptn)   USB_SET_EXF(eptn)
#define USB_CLEAR_EPT_DOUBLE_BUFFER(eptn) USB_CLEAR_EXF(eptn)

/**
  * @}
  */

/** @defgroup USB_exported_types
  * @{
  */

/**
  * @brief  usb endpoint infomation structure definition
  */
typedef struct
{
  uint8_t                                eptn;                        /*!< endpoint register number (0~7) */
  uint8_t                                ept_address;                 /*!< endpoint address */
  uint8_t                                inout;                       /*!< endpoint dir DATA_TRANS_IN or DATA_TRANS_OUT */
  uint8_t                                trans_type;                  /*!< endpoint type:
                                                                           EPT_CONTROL_TYPE, EPT_BULK_TYPE, EPT_INT_TYPE, EPT_ISO_TYPE*/
  uint16_t                               tx_addr;                     /*!< endpoint tx buffer offset address */
  uint16_t                               rx_addr;                     /*!< endpoint rx buffer offset address */
  uint16_t                               maxpacket;                   /*!< endpoint max packet*/
  uint8_t                                is_double_buffer;            /*!< endpoint double buffer flag */
  uint8_t                                stall;                       /*!< endpoint is stall state */
  uint16_t                               status;                      /*!< endpoint status */

  /* transmission buffer and count */
  uint16_t                               total_len;                   /*!< endpoint transmission total length */
  uint16_t                               trans_len;                   /*!< endpoint transmission length*/
  uint8_t                                *trans_buf;                  /*!< endpoint transmission buffer */

  uint16_t                               last_len;                    /*!< last transfer length */
  uint16_t                               rem0_len;                    /*!< rem transfer length */
  uint16_t                               ept0_slen;                   /*!< endpoint 0 transfer sum length */
}usb_ept_info;

/**
 * @brief type define usb register all
 */
typedef struct
{
  /**
  * @brief usb endpoint register, offset:0x00
  */
  union
  {
    __IO uint32_t ept[8];
    struct
    {
      __IO uint32_t eptaddr                  : 4; /* [3:0] */
      __IO uint32_t txsts                    : 2; /* [5:4] */
      __IO uint32_t txdts                    : 1; /* [6] */
      __IO uint32_t txtc                     : 1; /* [7] */
      __IO uint32_t exf                      : 1; /* [8] */
      __IO uint32_t trans_type               : 2; /* [10:9] */
      __IO uint32_t setuptc                  : 1; /* [11] */
      __IO uint32_t rxsts                    : 2; /* [13:12] */
      __IO uint32_t rxdts                    : 1; /* [14] */
      __IO uint32_t rxtc                     : 1; /* [15] */
      __IO uint32_t reserved1                : 16; /* [31:16] */
    } ept_bit[8];
  };

  __IO uint32_t reserved1[8];

  /**
  * @brief usb control register, offset:0x40
  */
  union
  {
    __IO uint32_t ctrl;
    struct
    {
      __IO uint32_t csrst                    : 1; /* [0] */
      __IO uint32_t disusb                   : 1; /* [1] */
      __IO uint32_t lpm                      : 1; /* [2] */
      __IO uint32_t ssp                      : 1; /* [3] */
      __IO uint32_t gresume                  : 1; /* [4] */
      __IO uint32_t reserved1                : 3; /* [7:5] */
      __IO uint32_t lsofien                  : 1; /* [8] */
      __IO uint32_t sofien                   : 1; /* [9] */
      __IO uint32_t rstien                   : 1; /* [10] */
      __IO uint32_t spien                    : 1; /* [11] */
      __IO uint32_t wkien                    : 1; /* [12] */
      __IO uint32_t beien                    : 1; /* [13] */
      __IO uint32_t ucforien                 : 1; /* [14] */
      __IO uint32_t tcien                    : 1; /* [15] */
      __IO uint32_t reserved2                : 16; /* [31:16] */
    } ctrl_bit;
  };

  /**
  * @brief usb interrupt status register, offset:0x44
  */
  union
  {
    __IO uint32_t intsts;
    struct
    {
      __IO uint32_t ept_num                  : 4; /* [3:0] */
      __IO uint32_t inout                    : 1; /* [4] */
      __IO uint32_t reserved1                : 3; /* [7:5] */
      __IO uint32_t lsof                     : 1; /* [8] */
      __IO uint32_t sof                      : 1; /* [9] */
      __IO uint32_t rst                      : 1; /* [10] */
      __IO uint32_t sp                       : 1; /* [11] */
      __IO uint32_t wk                       : 1; /* [12] */
      __IO uint32_t be                       : 1; /* [13] */
      __IO uint32_t ucfor                    : 1; /* [14] */
      __IO uint32_t tc                       : 1; /* [15] */
      __IO uint32_t reserved2                : 16; /* [31:16] */
    } intsts_bit;
  };

  /**
  * @brief usb frame number register, offset:0x48
  */
  union
  {
    __IO uint32_t sofrnum;
    struct
    {
      __IO uint32_t sofnum                   : 11; /* [10:0] */
      __IO uint32_t lsofnum                  : 2; /* [12:11] */
      __IO uint32_t clck                     : 1; /* [13] */
      __IO uint32_t dmsts                    : 1; /* [14] */
      __IO uint32_t dpsts                    : 1; /* [15] */
      __IO uint32_t reserved1                : 16; /* [31:16] */
    } sofrnum_bit;
  };

  /**
  * @brief usb device address register, offset:0x4c
  */
  union
  {
    __IO uint32_t devaddr;
    struct
    {
      __IO uint32_t addr                     : 7; /* [6:0] */
      __IO uint32_t cen                      : 1; /* [7] */
      __IO uint32_t reserved1                : 24; /* [31:8] */
    } devaddr_bit;
  };

  /**
  * @brief usb buffer address register, offset:0x50
  */
  union
  {
    __IO uint32_t buftbl;
    struct
    {
      __IO uint32_t reserved1                : 3; /* [2:0] */
      __IO uint32_t btaddr                   : 13; /* [15:3] */
      __IO uint32_t reserved2                : 16; /* [31:16] */
    } buftbl_bit;
  };
  __IO uint32_t reserved2[3];
  /**
  * @brief usb cfg control register, offset:0x60
  */
  union
  {
    __IO uint32_t cfg;
    struct
    {
      __IO uint32_t sofouten                 : 1; /* [0] */
      __IO uint32_t puo                      : 1; /* [1] */
      __IO uint32_t reserved1                : 30; /* [31:2] */
    } cfg_bit;
  };

} usbd_type;

/**
  * @}
  */

#define USB                              ((usbd_type *) USBFS_BASE)

typedef usbd_type usb_reg_type;
extern uint32_t g_usb_packet_address;

/** @defgroup USB_exported_functions
  * @{
  */

void usb_dev_init(usbd_type *usbx);
void usb_connect(usbd_type *usbx);
void usb_disconnect(usbd_type *usbx);
void usb_usbbufs_enable(usbd_type *usbx, confirm_state state);
void usb_ept_open(usbd_type *usbx, usb_ept_info *ept_info);
void usb_ept_close(usbd_type *usbx, usb_ept_info *ept_info);
void usb_write_packet(uint8_t *pusr_buf, uint16_t offset_addr, uint16_t nbytes);
void usb_read_packet(uint8_t *pusr_buf, uint16_t offset_addr, uint16_t nbytes);
void usb_interrupt_enable(usbd_type *usbx, uint16_t interrupt, confirm_state new_state);
void usb_set_address(usbd_type *usbx, uint8_t address);
void usb_ept_stall(usbd_type *usbx, usb_ept_info *ept_info);
void usb_enter_suspend(usbd_type *usbx);
void usb_exit_suspend(usbd_type *usbx);
void usb_remote_wkup_set(usbd_type *usbx);
void usb_remote_wkup_clear(usbd_type *usbx);
uint16_t usb_buffer_malloc(uint16_t maxpacket);
void usb_buffer_free(void);
flag_status usb_flag_get(usbd_type *usbx, uint16_t flag);
void usb_flag_clear(usbd_type *usbx, uint16_t flag);


#ifdef __cplusplus
}
#endif

#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
