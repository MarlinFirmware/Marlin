/**
  **************************************************************************
  * @file     at32f403a_407_emac.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 emac header file
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
#ifndef __AT32F403A_407_EMAC_H
#define __AT32F403A_407_EMAC_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f403a_407.h"

/** @addtogroup AT32F403A_407_periph_driver
  * @{
  */

/** @addtogroup EMAC
  * @{
  */

#define PHY_TIMEOUT                      (0x000FFFFF) /*!< timeout for phy response */

/** @defgroup EMAC_smi_clock_border_definition
  * @brief emac smi clock border
  * @{
  */

#define EMAC_HCLK_BORDER_20MHZ           (20000000)   /*!< hclk boarder of 20 mhz */
#define EMAC_HCLK_BORDER_35MHZ           (35000000)   /*!< hclk boarder of 35 mhz */
#define EMAC_HCLK_BORDER_60MHZ           (60000000)   /*!< hclk boarder of 60 mhz */
#define EMAC_HCLK_BORDER_100MHZ          (100000000)  /*!< hclk boarder of 100 mhz */
#define EMAC_HCLK_BORDER_150MHZ          (150000000)  /*!< hclk boarder of 150 mhz */
#define EMAC_HCLK_BORDER_240MHZ          (240000000)  /*!< hclk boarder of 240 mhz */

/**
  * @}
  */

/** @defgroup EMAC_interrupts_definition
  * @brief emac interrupts
  * @{
  */

#define EMAC_PMT_FLAG                    ((uint32_t)0x00000008) /*!< interrupt bit of pmt */
#define EMAC_MMC_FLAG                    ((uint32_t)0x00000010) /*!< interrupt bit of mmc */
#define EMAC_MMCR_FLAG                   ((uint32_t)0x00000020) /*!< interrupt bit of mmcr */
#define EMAC_MMCT_FLAG                   ((uint32_t)0x00000040) /*!< interrupt bit of mmct */
#define EMAC_TST_FLAG                    ((uint32_t)0x00000200) /*!< interrupt bit of tst */

/**
  * @}
  */

/** @defgroup EMAC_mmc_flags_definition
  * @brief emac mmc flags
  * @{
  */

#define MMC_RX_CRC_ERROR                 ((uint32_t)0x00000020) /*!< mmc error flag of rx crc */
#define MMC_RX_ALIGN_ERROR               ((uint32_t)0x00000040) /*!< mmc error flag of rx alignment */
#define MMC_RX_GOOD_UNICAST              ((uint32_t)0x00020000) /*!< mmc error flag of rx unicast good frames */
#define MMC_TX_SINGLE_COL                ((uint32_t)0x00004000) /*!< mmc error flag of tx single collision */
#define MMC_TX_MULTIPLE_COL              ((uint32_t)0x00008000) /*!< mmc error flag of tx multiple collision */
#define MMC_TX_GOOD_FRAMES               ((uint32_t)0x00200000) /*!< mmc error flag of tx good frames */

/**
  * @}
  */

/** @defgroup EMAC_packet_definition
  * @brief emac packet
  * @{
  */

#define EMAC_MAX_PACKET_LENGTH           1520 /*!< emac_header + emac_extra + emac_max_payload + emac_crc */
#define EMAC_HEADER                      14   /*!< 6 byte dest addr, 6 byte src addr, 2 byte length/ept_type */
#define EMAC_CRC                         4    /*!< ethernet crc */
#define EMAC_EXTRA                       2    /*!< extra bytes in some cases */
#define VLAN_TAG                         4    /*!< optional 802.1q vlan tag */
#define EMAC_MIN_PAYLOAD                 46   /*!< minimum ethernet payload size */
#define EMAC_MAX_PAYLOAD                 1500 /*!< maximum ethernet payload size */
#define JUMBO_FRAME_PAYLOAD              9000 /*!< jumbo frame payload size */
#define EMAC_DMARXDESC_FRAME_LENGTHSHIFT 16

/**
  * @}
  */

/** @defgroup EMAC_dma_descriptor_tdes0_definition
  * @brief tdes0 definition
  * @{
  */

#define EMAC_DMATXDESC_OWN               ((uint32_t)0x80000000) /*!< own bit: descriptor is owned by dma engine */
#define EMAC_DMATXDESC_IC                ((uint32_t)0x40000000) /*!< interrupt on completion */
#define EMAC_DMATXDESC_LS                ((uint32_t)0x20000000) /*!< last segment */
#define EMAC_DMATXDESC_FS                ((uint32_t)0x10000000) /*!< first segment */
#define EMAC_DMATXDESC_DC                ((uint32_t)0x08000000) /*!< disable crc */
#define EMAC_DMATXDESC_DP                ((uint32_t)0x04000000) /*!< disable padding */
#define EMAC_DMATXDESC_TTSE              ((uint32_t)0x02000000) /*!< transmit time stamp enable */
#define EMAC_DMATXDESC_CIC               ((uint32_t)0x00C00000) /*!< checksum insertion control: 4 cases */
#define EMAC_DMATXDESC_CIC_BYPASS        ((uint32_t)0x00000000) /*!< do nothing: checksum engine is bypassed */
#define EMAC_DMATXDESC_CIC_IPV4HEADER    ((uint32_t)0x00400000) /*!< ipv4 header checksum insertion */
#define EMAC_DMATXDESC_CIC_TUI_SEG       ((uint32_t)0x00800000) /*!< tcp/udp/icmp checksum insertion calculated over segment only */
#define EMAC_DMATXDESC_CIC_TUI_FULL      ((uint32_t)0x00C00000) /*!< tcp/udp/icmp checksum insertion fully calculated */
#define EMAC_DMATXDESC_TER               ((uint32_t)0x00200000) /*!< transmit end of ring */
#define EMAC_DMATXDESC_TCH               ((uint32_t)0x00100000) /*!< second address chained */
#define EMAC_DMATXDESC_TTSS              ((uint32_t)0x00020000) /*!< tx time stamp status */
#define EMAC_DMATXDESC_IHE               ((uint32_t)0x00010000) /*!< ip header error */
#define EMAC_DMATXDESC_ES                ((uint32_t)0x00008000) /*!< error summary: or of the following bits: ue || ED || EC || LCO || NC || LCA || FF || JT */
#define EMAC_DMATXDESC_JT                ((uint32_t)0x00004000) /*!< jabber timeout */
#define EMAC_DMATXDESC_FF                ((uint32_t)0x00002000) /*!< frame flushed: dma/mtl flushed the frame due to SW flush */
#define EMAC_DMATXDESC_PCE               ((uint32_t)0x00001000) /*!< payload checksum error */
#define EMAC_DMATXDESC_LCA               ((uint32_t)0x00000800) /*!< loss of carrier: carrier lost during tramsmission */
#define EMAC_DMATXDESC_NC                ((uint32_t)0x00000400) /*!< no carrier: no carrier signal from the tranceiver */
#define EMAC_DMATXDESC_LCO               ((uint32_t)0x00000200) /*!< late collision: transmission aborted due to collision */
#define EMAC_DMATXDESC_EC                ((uint32_t)0x00000100) /*!< excessive collision: transmission aborted after 16 collisions */
#define EMAC_DMATXDESC_VF                ((uint32_t)0x00000080) /*!< vlan frame */
#define EMAC_DMATXDESC_CC                ((uint32_t)0x00000078) /*!< collision count */
#define EMAC_DMATXDESC_ED                ((uint32_t)0x00000004) /*!< excessive deferral */
#define EMAC_DMATXDESC_UF                ((uint32_t)0x00000002) /*!< underflow error: late data arrival from the memory */
#define EMAC_DMATXDESC_DB                ((uint32_t)0x00000001) /*!< deferred bit */

/**
  * @}
  */

/** @defgroup EMAC_dma_descriptor_tdes1_definition
  * @brief  tdes1 descriptor
  * @{
  */

#define EMAC_DMATXDESC_TBS2              ((uint32_t)0x1FFF0000) /*!< transmit buffer2 size */
#define EMAC_DMATXDESC_TBS1              ((uint32_t)0x00001FFF) /*!< transmit buffer1 size */

/**
  * @}
  */

/** @defgroup EMAC_dma_descriptor_tdes2_definition
  * @brief  tdes2 descriptor
  * @{
  */

#define EMAC_DMATXDESC_B1AP              ((uint32_t)0xFFFFFFFF) /*!< buffer1 address pointer */

/**
  * @}
  */

/** @defgroup EMAC_dma_descriptor_tdes3_definition
  * @brief  tdes3 descriptor
  * @{
  */

#define EMAC_DMATxDesc_B2AP              ((uint32_t)0xFFFFFFFF) /*!< buffer2 address pointer */

/**
  * @}
  */

/** @defgroup EMAC_dma_descriptor_rdes0_definition
  * @brief  rdes0 descriptor
  * @{
  */

#define EMAC_DMARXDESC_OWN               ((uint32_t)0x80000000) /*!< own bit: descriptor is owned by dma engine  */
#define EMAC_DMARXDESC_AFM               ((uint32_t)0x40000000) /*!< da filter fail for the rx frame  */
#define EMAC_DMARXDESC_FL                ((uint32_t)0x3FFF0000) /*!< receive descriptor frame length  */
#define EMAC_DMARXDESC_ES                ((uint32_t)0x00008000) /*!< error summary: or of the following bits: de || OE || IPC || LC || RWT || RE || CE */
#define EMAC_DMARXDESC_DE                ((uint32_t)0x00004000) /*!< desciptor error: no more descriptors for receive frame  */
#define EMAC_DMARXDESC_SAF               ((uint32_t)0x00002000) /*!< sa filter fail for the received frame */
#define EMAC_DMARXDESC_LE                ((uint32_t)0x00001000) /*!< frame size not matching with length field */
#define EMAC_DMARXDESC_OE                ((uint32_t)0x00000800) /*!< overflow error: frame was damaged due to buffer overflow */
#define EMAC_DMARXDESC_VLAN              ((uint32_t)0x00000400) /*!< vlan tag: received frame is a vlan frame */
#define EMAC_DMARXDESC_FS                ((uint32_t)0x00000200) /*!< first descriptor of the frame  */
#define EMAC_DMARXDESC_LS                ((uint32_t)0x00000100) /*!< last descriptor of the frame  */
#define EMAC_DMARXDESC_IPV4HCE           ((uint32_t)0x00000080) /*!< ipc checksum error: rx ipv4 header checksum error   */
#define EMAC_DMARXDESC_LC                ((uint32_t)0x00000040) /*!< late collision occurred during reception   */
#define EMAC_DMARXDESC_FT                ((uint32_t)0x00000020) /*!< frame ept_type - ethernet, otherwise 802.3    */
#define EMAC_DMARXDESC_RWT               ((uint32_t)0x00000010) /*!< receive watchdog timeout: watchdog timer expired during reception    */
#define EMAC_DMARXDESC_RE                ((uint32_t)0x00000008) /*!< receive error: error reported by mii interface  */
#define EMAC_DMARXDESC_DBE               ((uint32_t)0x00000004) /*!< dribble bit error: frame contains non int multiple of 8 bits  */
#define EMAC_DMARXDESC_CE                ((uint32_t)0x00000002) /*!< crc error */
#define EMAC_DMARXDESC_MAMPCE            ((uint32_t)0x00000001) /*!< rx mac address/payload checksum error: rx mac address matched/ Rx Payload Checksum Error */

/**
  * @}
  */

/** @defgroup EMAC_dma_descriptor_rdes1_definition
  * @brief  rdes1 descriptor
  * @{
  */

#define EMAC_DMARXDESC_DIC               ((uint32_t)0x80000000) /*!< disable interrupt on completion */
#define EMAC_DMARXDESC_RBS2              ((uint32_t)0x1FFF0000) /*!< receive buffer2 size */
#define EMAC_DMARXDESC_RER               ((uint32_t)0x00008000) /*!< receive end of ring */
#define EMAC_DMARXDESC_RCH               ((uint32_t)0x00004000) /*!< second address chained */
#define EMAC_DMARXDESC_RBS1              ((uint32_t)0x00001FFF) /*!< receive buffer1 size */

/**
  * @}
  */

/** @defgroup EMAC_dma_descriptor_rdes2_definition
  * @brief  rdes2 descriptor
  * @{
  */

#define EMAC_DMARXDESC_B1AP              ((uint32_t)0xFFFFFFFF) /*!< buffer1 address pointer */

/**
  * @}
  */

/** @defgroup EMAC_dma_descriptor_rdes3_definition
  * @brief  rdes3 descriptor
  * @{
  */

#define EMAC_DMARXDESC_B2AP              ((uint32_t)0xFFFFFFFF) /*!< buffer2 address pointer */

/**
  * @}
  */

/**
  * @brief  emac dma flag
  */
#define EMAC_DMA_TI_FLAG                 ((uint32_t)0x00000001) /*!< emac dma transmit interrupt */
#define EMAC_DMA_TPS_FLAG                ((uint32_t)0x00000002) /*!< emac dma transmit process stopped */
#define EMAC_DMA_TBU_FLAG                ((uint32_t)0x00000004) /*!< emac dma transmit buffer unavailable */
#define EMAC_DMA_TJT_FLAG                ((uint32_t)0x00000008) /*!< emac dma transmit jabber timeout */
#define EMAC_DMA_OVF_FLAG                ((uint32_t)0x00000010) /*!< emac dma receive overflow */
#define EMAC_DMA_UNF_FLAG                ((uint32_t)0x00000020) /*!< emac dma transmit underflow */
#define EMAC_DMA_RI_FLAG                 ((uint32_t)0x00000040) /*!< emac dma receive interrupt */
#define EMAC_DMA_RBU_FLAG                ((uint32_t)0x00000080) /*!< emac dma receive buffer unavailable */
#define EMAC_DMA_RPS_FLAG                ((uint32_t)0x00000100) /*!< emac dma receive process stopped */
#define EMAC_DMA_RWT_FLAG                ((uint32_t)0x00000200) /*!< emac dma receive watchdog timeout */
#define EMAC_DMA_ETI_FLAG                ((uint32_t)0x00000400) /*!< emac dma early transmit interrupt */
#define EMAC_DMA_FBEI_FLAG               ((uint32_t)0x00002000) /*!< emac dma fatal bus error interrupt */
#define EMAC_DMA_ERI_FLAG                ((uint32_t)0x00004000) /*!< emac dma early receive interrupt */
#define EMAC_DMA_AIS_FLAG                ((uint32_t)0x00008000) /*!< emac dma abnormal interrupt summary */
#define EMAC_DMA_NIS_FLAG                ((uint32_t)0x00010000) /*!< emac dma normal interrupt summary */

/** @defgroup EMAC_exported_types
  * @{
  */

/**
  * @brief  emac auto negotiation type
  */
typedef enum
{
  EMAC_AUTO_NEGOTIATION_OFF              = 0x00, /*!< disable auto negotiation */
  EMAC_AUTO_NEGOTIATION_ON               = 0x01  /*!< enable auto negotiation */
} emac_auto_negotiation_type;

/**
  * @brief  emac back_off limit type
  */
typedef enum
{
  EMAC_BACKOFF_LIMIT_0                   = 0x00, /*!< retransmission clock gap numbers betwenn n and 10 */
  EMAC_BACKOFF_LIMIT_1                   = 0x01, /*!< retransmission clock gap numbers betwenn n and 8 */
  EMAC_BACKOFF_LIMIT_2                   = 0x02, /*!< retransmission clock gap numbers betwenn n and 4 */
  EMAC_BACKOFF_LIMIT_3                   = 0x03  /*!< retransmission clock gap numbers betwenn n and 1 */
} emac_bol_type;

/**
  * @brief  emac duplex type
  */
typedef enum
{
  EMAC_HALF_DUPLEX                       = 0x00, /*!< half duplex */
  EMAC_FULL_DUPLEX                       = 0x01  /*!< full duplex */
} emac_duplex_type;

/**
  * @brief  emac speed type
  */
typedef enum
{
  EMAC_SPEED_10MBPS                      = 0x00, /*!< 10 mbps */
  EMAC_SPEED_100MBPS                     = 0x01  /*!< 100 mbps */
} emac_speed_type;

/**
  * @brief  emac interframe gap type
  */
typedef enum
{
  EMAC_INTERFRAME_GAP_96BIT              = 0x00, /*!< 96-bit numbers between two frames */
  EMAC_INTERFRAME_GAP_88BIT              = 0x01, /*!< 88-bit numbers between two frames */
  EMAC_INTERFRAME_GAP_80BIT              = 0x02, /*!< 80-bit numbers between two frames */
  EMAC_INTERFRAME_GAP_72BIT              = 0x03, /*!< 72-bit numbers between two frames */
  EMAC_INTERFRAME_GAP_64BIT              = 0x04, /*!< 64-bit numbers between two frames */
  EMAC_INTERFRAME_GAP_56BIT              = 0x05, /*!< 56-bit numbers between two frames */
  EMAC_INTERFRAME_GAP_48BIT              = 0x06, /*!< 48-bit numbers between two frames */
  EMAC_INTERFRAME_GAP_40BIT              = 0x07  /*!< 40-bit numbers between two frames */
} emac_intergrame_gap_type;

/**
  * @brief  mdc clock range type
  */
typedef enum
{
  EMAC_CLOCK_RANGE_60_TO_100             = 0x00, /*!< mdc is hclk/42 */
  EMAC_CLOCK_RANGE_100_TO_150            = 0x01, /*!< mdc is hclk/62 */
  EMAC_CLOCK_RANGE_20_TO_35              = 0x02, /*!< mdc is hclk/16 */
  EMAC_CLOCK_RANGE_35_TO_60              = 0x03, /*!< mdc is hclk/26 */
  EMAC_CLOCK_RANGE_150_TO_240            = 0x04  /*!< mdc is hclk/102 */
} emac_clock_range_type;

/**
  * @brief  emac control frames filter type
  */
typedef enum
{
  EMAC_CONTROL_FRAME_PASSING_NO          = 0x00, /*!< don't pass any control frame to application */
  EMAC_CONTROL_FRAME_PASSING_ALL         = 0x02, /*!< pass all control frames to application */
  EMAC_CONTROL_FRAME_PASSING_MATCH       = 0x03  /*!< only pass filtered control frames to application */
} emac_control_frames_filter_type;

/**
  * @brief  pause threshold type
  */
typedef enum
{
  EMAC_PAUSE_4_SLOT_TIME                 = 0x00, /*!< pause time is 4 slot time */
  EMAC_PAUSE_28_SLOT_TIME                = 0x01, /*!< pause time is 28 slot time */
  EMAC_PAUSE_144_SLOT_TIME               = 0x02, /*!< pause time is 144 slot time */
  EMAC_PAUSE_256_SLOT_TIME               = 0x03  /*!< pause time is 256 slot time */
} emac_pause_slot_threshold_type;

/**
  * @brief  interrupt mask type
  */
typedef enum
{
  EMAC_INTERRUPT_PMT_MASK                = 0x00, /*!< mask pmt interrupt */
  EMAC_INTERRUPT_TST_MASK                = 0x01  /*!< mask tst interrupt */
} emac_interrupt_mask_type;

/**
  * @brief  mac address type
  */
typedef enum
{
  EMAC_ADDRESS_FILTER_1                  = 0x01, /*!< mac address 1 filter */
  EMAC_ADDRESS_FILTER_2                  = 0x02, /*!< mac address 2 filter */
  EMAC_ADDRESS_FILTER_3                  = 0x03  /*!< mac address 3 filter */
} emac_address_type;

/**
  * @brief  address filter type
  */
typedef enum
{
  EMAC_DESTINATION_FILTER                = 0x00, /*!< destination mac address filter */
  EMAC_SOURCE_FILTER                     = 0x01  /*!< source mac address filter */
} emac_address_filter_type;

/**
  * @brief  address mask type
  */
typedef enum
{
  EMAC_ADDRESS_MASK_8L0                  = 0x01, /*!< byte 0 of mac address low register */
  EMAC_ADDRESS_MASK_15L8                 = 0x02, /*!< byte 1 of mac address low register */
  EMAC_ADDRESS_MASK_23L16                = 0x04, /*!< byte 2 of mac address low register */
  EMAC_ADDRESS_MASK_31L24                = 0x08, /*!< byte 3 of mac address low register */
  EMAC_ADDRESS_MASK_7H0                  = 0x10, /*!< byte 0 of mac address high register */
  EMAC_ADDRESS_MASK_15H8                 = 0x20  /*!< byte 1 of mac address high register */
} emac_address_mask_type;

/**
  * @brief  rx tx priority ratio type
  */
typedef enum
{
  EMAC_DMA_1_RX_1_TX                     = 0x00, /*!< rx/tx ratio is 1:1 */
  EMAC_DMA_2_RX_1_TX                     = 0x01, /*!< rx/tx ratio is 2:1 */
  EMAC_DMA_3_RX_1_TX                     = 0x02, /*!< rx/tx ratio is 3:1 */
  EMAC_DMA_4_RX_1_TX                     = 0x03  /*!< rx/tx ratio is 4:1 */
} emac_dma_rx_tx_ratio_type;

/**
  * @brief  programmable burst length
  */
typedef enum
{
  EMAC_DMA_PBL_1                         = 0x01, /*!< maximum 1 time of beats to be transferred in one dma transaction */
  EMAC_DMA_PBL_2                         = 0x02, /*!< maximum 2 times of beats to be transferred in one dma transaction */
  EMAC_DMA_PBL_4                         = 0x04, /*!< maximum 4 times of beats to be transferred in one dma transaction */
  EMAC_DMA_PBL_8                         = 0x08, /*!< maximum 8 times of beats to be transferred in one dma transaction */
  EMAC_DMA_PBL_16                        = 0x10, /*!< maximum 16 times of beats to be transferred in one dma transaction */
  EMAC_DMA_PBL_32                        = 0x20  /*!< maximum 32 times of beats to be transferred in one dma transaction */
} emac_dma_pbl_type;

/**
  * @brief  dma tx rx type
  */
typedef enum
{
  EMAC_DMA_TRANSMIT                      = 0x00, /*!< transmit dma */
  EMAC_DMA_RECEIVE                       = 0x01  /*!< receive dma */
} emac_dma_tx_rx_type;

/**
  * @brief  dma receive process status type
  */
typedef enum
{
  EMAC_DMA_RX_RESET_STOP_COMMAND         = 0x00, /*!< receive reset or stop command */
  EMAC_DMA_RX_FETCH_DESCRIPTOR           = 0x01, /*!< rx dma is fetching descriptor */
  EMAC_DMA_RX_WAITING_PACKET             = 0x03, /*!< rx dma is waiting for packets */
  EMAC_DMA_RX_DESCRIPTOR_UNAVAILABLE     = 0x04, /*!< rx dma descriptor is unavailable */
  EMAC_DMA_RX_CLOSE_DESCRIPTOR           = 0x05, /*!< rx dma is closing descriptor */
  EMAC_DMA_RX_FIFO_TO_HOST               = 0x07  /*!< rx dma is transferring data from fifo to host */
} emac_dma_receive_process_status_type;

/**
  * @brief  dma transmit process status type
  */
typedef enum
{
  EMAC_DMA_TX_RESET_STOP_COMMAND         = 0x00, /*!< receive reset or stop command */
  EMAC_DMA_TX_FETCH_DESCRIPTOR           = 0x01, /*!< tx dma is fetching descriptor */
  EMAC_DMA_TX_WAITING_FOR_STATUS         = 0x02, /*!< tx dma is waiting for status message */
  EMAC_DMA_TX_HOST_TO_FIFO               = 0x03, /*!< tx dma is reading data from host and forward data to fifo */
  EMAC_DMA_TX_DESCRIPTOR_UNAVAILABLE     = 0x06, /*!< tx dma is unavailable or fifo underflow */
  EMAC_DMA_TX_CLOSE_DESCRIPTOR           = 0x07  /*!< tx dma is closing descriptor */
} emac_dma_transmit_process_status_type;

/**
  * @brief  dma operations type
  */
typedef enum
{
  EMAC_DMA_OPS_START_STOP_RECEIVE        = 0x00, /*!< start/stop receive */
  EMAC_DMA_OPS_SECOND_FRAME              = 0x01, /*!< operate on second frame */
  EMAC_DMA_OPS_FORWARD_UNDERSIZED        = 0x02, /*!< forward undersized good frames*/
  EMAC_DMA_OPS_FORWARD_ERROR             = 0x03, /*!< forward error frames */
  EMAC_DMA_OPS_START_STOP_TRANSMIT       = 0x04, /*!< start/stop transmission */
  EMAC_DMA_OPS_FLUSH_TRANSMIT_FIFO       = 0x05, /*!< flush transmit fifo */
  EMAC_DMA_OPS_TRANSMIT_STORE_FORWARD    = 0x06, /*!< transmit store and forward */
  EMAC_DMA_OPS_RECEIVE_FLUSH_DISABLE     = 0x07, /*!< disable flushing of received frames */
  EMAC_DMA_OPS_RECEIVE_STORE_FORWARD     = 0x08, /*!< receive store and forward */
  EMAC_DMA_OPS_DROP_ERROR_DISABLE        = 0x09  /*!< disbale dropping of tcp/ip checksum error frames */
} emac_dma_operations_type;

/**
  * @brief  receive threshold control type
  */
typedef enum
{
  EMAC_DMA_RX_THRESHOLD_64_BYTES         = 0x00, /*!< receive starts when the frame size within the receiv fifo is larger than 64 bytes */
  EMAC_DMA_RX_THRESHOLD_32_BYTES         = 0x01, /*!< receive starts when the frame size within the receiv fifo is larger than 32 bytes */
  EMAC_DMA_RX_THRESHOLD_96_BYTES         = 0x02, /*!< receive starts when the frame size within the receiv fifo is larger than 96 bytes */
  EMAC_DMA_RX_THRESHOLD_128_BYTES        = 0x03  /*!< receive starts when the frame size within the receiv fifo is larger than 128 bytes */
} emac_dma_receive_threshold_type;

/**
  * @brief  transmit threshold control type
  */
typedef enum
{
  EMAC_DMA_TX_THRESHOLD_64_BYTES         = 0x00, /*!< transmission starts when the frame size within the transmit FIFO is larger than 64 bytes */
  EMAC_DMA_TX_THRESHOLD_128_BYTES        = 0x01, /*!< transmission starts when the frame size within the transmit FIFO is larger than 128 bytes */
  EMAC_DMA_TX_THRESHOLD_192_BYTES        = 0x02, /*!< transmission starts when the frame size within the transmit FIFO is larger than 192 bytes */
  EMAC_DMA_TX_THRESHOLD_256_BYTES        = 0x03, /*!< transmission starts when the frame size within the transmit FIFO is larger than 256 bytes */
  EMAC_DMA_TX_THRESHOLD_40_BYTES         = 0x04, /*!< transmission starts when the frame size within the transmit FIFO is larger than 40 bytes */
  EMAC_DMA_TX_THRESHOLD_32_BYTES         = 0x05, /*!< transmission starts when the frame size within the transmit FIFO is larger than 32 bytes */
  EMAC_DMA_TX_THRESHOLD_24_BYTES         = 0x06, /*!< transmission starts when the frame size within the transmit FIFO is larger than 24 bytes */
  EMAC_DMA_TX_THRESHOLD_16_BYTES         = 0x07  /*!< transmission starts when the frame size within the transmit FIFO is larger than 16 bytes */
} emac_dma_transmit_threshold_type;

/**
  * @brief  dma interrupt type
  */
typedef enum
{
  EMAC_DMA_INTERRUPT_TX                  = 0x00, /*!< transmit interrupt */
  EMAC_DMA_INTERRUPT_TX_STOP             = 0x01, /*!< transmit process stopped interrupt */
  EMAC_DMA_INTERRUPT_TX_UNAVAILABLE      = 0x02, /*!< transmit buffer unavailable interrupt */
  EMAC_DMA_INTERRUPT_TX_JABBER           = 0x03, /*!< transmit jabber timeout interrupt */
  EMAC_DMA_INTERRUPT_RX_OVERFLOW         = 0x04, /*!< receive overflow interrupt */
  EMAC_DMA_INTERRUPT_TX_UNDERFLOW        = 0x05, /*!< transmit underflow interrupt */
  EMAC_DMA_INTERRUPT_RX                  = 0x06, /*!< receive interrupt */
  EMAC_DMA_INTERRUPT_RX_UNAVAILABLE      = 0x07, /*!< receive buffer unavailable interrupt */
  EMAC_DMA_INTERRUPT_RX_STOP             = 0x08, /*!< receive process stopped interrupt */
  EMAC_DMA_INTERRUPT_RX_TIMEOUT          = 0x09, /*!< receive watchdog timeout interrupt */
  EMAC_DMA_INTERRUPT_TX_EARLY            = 0x0A, /*!< early transmit interrupt */
  EMAC_DMA_INTERRUPT_FATAL_BUS_ERROR     = 0x0B, /*!< fatal bus error interrupt */
  EMAC_DMA_INTERRUPT_RX_EARLY            = 0x0C, /*!< early receive interrupt */
  EMAC_DMA_INTERRUPT_ABNORMAL_SUMMARY    = 0x0D, /*!< abnormal interrupt summary */
  EMAC_DMA_INTERRUPT_NORMAL_SUMMARY      = 0x0E  /*!< normal interrupt summary */
} emac_dma_interrupt_type;

/**
  * @brief  dma tansfer address type
  */
typedef enum
{
  EMAC_DMA_TX_DESCRIPTOR                 = 0x00, /*!< transmit descriptor address */
  EMAC_DMA_RX_DESCRIPTOR                 = 0x01, /*!< receive descriptor address */
  EMAC_DMA_TX_BUFFER                     = 0x02, /*!< transmit buffer address */
  EMAC_DMA_RX_BUFFER                     = 0x03  /*!< receive buffer address */
} emac_dma_transfer_address_type;

/**
  * @brief  clock node type
  */
typedef enum
{
  EMAC_PTP_NORMAL_CLOCK                  = 0x00, /*!< normal clock node */
  EMAC_PTP_BOUNDARY_CLOCK                = 0x01, /*!< boundary clock node */
  EMAC_PTP_END_TO_END_CLOCK              = 0x02, /*!< end to end transparent clock node */
  EMAC_PTP_PEER_TO_PEER_CLOCK            = 0x03  /*!< peer to peer transparent clock node */
} emac_ptp_clock_node_type;

/**
  * @brief  time stamp status type
  */
typedef enum
{
  EMAC_PTP_SECOND_OVERFLOW               = 0x00, /*!< time stamp second overflow */
  EMAC_PTP_TARGET_TIME_REACH             = 0x01  /*!< time stamp target time reached */
} emac_ptp_timestamp_status_type;

/**
  * @brief  pps control type
  */
typedef enum
{
  EMAC_PTP_PPS_1HZ                       = 0x00, /*!< pps frequency is 1 hz */
  EMAC_PTP_PPS_2HZ                       = 0x01, /*!< pps frequency is 2 hz */
  EMAC_PTP_PPS_4HZ                       = 0x02, /*!< pps frequency is 4 hz */
  EMAC_PTP_PPS_8HZ                       = 0x03, /*!< pps frequency is 8 hz */
  EMAC_PTP_PPS_16HZ                      = 0x04, /*!< pps frequency is 16 hz */
  EMAC_PTP_PPS_32HZ                      = 0x05, /*!< pps frequency is 32 hz */
  EMAC_PTP_PPS_64HZ                      = 0x06, /*!< pps frequency is 64 hz */
  EMAC_PTP_PPS_128HZ                     = 0x07, /*!< pps frequency is 128 hz */
  EMAC_PTP_PPS_256HZ                     = 0x08, /*!< pps frequency is 256 hz */
  EMAC_PTP_PPS_512HZ                     = 0x09, /*!< pps frequency is 512 hz */
  EMAC_PTP_PPS_1024HZ                    = 0x0A, /*!< pps frequency is 1024 hz */
  EMAC_PTP_PPS_2048HZ                    = 0x0B, /*!< pps frequency is 2048 hz */
  EMAC_PTP_PPS_4096HZ                    = 0x0C, /*!< pps frequency is 4096 hz */
  EMAC_PTP_PPS_8192HZ                    = 0x0D, /*!< pps frequency is 8192 hz */
  EMAC_PTP_PPS_16384HZ                   = 0x0E, /*!< pps frequency is 16384 hz */
  EMAC_PTP_PPS_32768HZ                   = 0x0F  /*!< pps frequency is 32768 hz */
} emac_ptp_pps_control_type;

/**
  * @brief  ethernet mac control config type
  */
typedef struct
{
  emac_auto_negotiation_type             auto_nego;             /*!< auto negotiatin enable */
  confirm_state                          deferral_check;        /*!< deferral check enable */
  emac_bol_type                          back_off_limit;        /*!< back-off limit setting */
  confirm_state                          auto_pad_crc_strip;    /*!< automtic pad/crc stripping enable */
  confirm_state                          retry_disable;         /*!< retry disable*/
  confirm_state                          ipv4_checksum_offload; /*!< ipv4 checksum offload enable */
  emac_duplex_type                       duplex_mode;           /*!< duplex mode enable */
  confirm_state                          loopback_mode;         /*!< loopback mode enable */
  confirm_state                          receive_own_disable;   /*!< receive own disbale */
  emac_speed_type                        fast_ethernet_speed;   /*!< fast ethernet speed enable */
  confirm_state                          carrier_sense_disable; /*!< carrier sense disable*/
  emac_intergrame_gap_type               interframe_gap;        /*!< set interframe gap */
  confirm_state                          jabber_disable;        /*!< jabber disbale */
  confirm_state                          watchdog_disable;      /*!< watchdog disable */
} emac_control_config_type;

/**
  * @brief  ethernet mac dma config type
  */
typedef struct
{
  confirm_state                          aab_enable;        /*!< address-aligned beats enable */
  confirm_state                          usp_enable;        /*!< separate PBL enable */
  emac_dma_pbl_type                      rx_dma_pal;        /*!< rx dma pbl */
  confirm_state                          fb_enable;         /*!< separate PBL enable */
  emac_dma_pbl_type                      tx_dma_pal;        /*!< tx dma pbl */
  uint8_t                                desc_skip_length;  /*!< descriptor skip length */
  confirm_state                          da_enable;         /*!< dma arbitration enable */
  emac_dma_rx_tx_ratio_type              priority_ratio;    /*!< priority ratio */
  confirm_state                          dt_disable;        /*!< disable dropping of tcp/ip checksum error frames */
  confirm_state                          rsf_enable;        /*!< enable receiving store or forward */
  confirm_state                          flush_rx_disable;  /*!< disable flushing of received frames */
  confirm_state                          tsf_enable;        /*!< enable transmitting store or forward */
  emac_dma_transmit_threshold_type       tx_threshold;      /*!< transmit threshold control */
  confirm_state                          fef_enable;        /*!< enable forward error frames */
  confirm_state                          fugf_enable;       /*!< enable forward undersized good frames */
  emac_dma_receive_threshold_type        rx_threshold;      /*!< receive threshold control */
  confirm_state                          osf_enable;        /*!< enable operating on second frames */
} emac_dma_config_type;

/**
  * @brief  dma desciptors data structure definition
  */
typedef struct  {
  uint32_t   status;                /*!< status */
  uint32_t   controlsize;           /*!< control and buffer1, buffer2 lengths */
  uint32_t   buf1addr;              /*!< buffer1 address pointer */
  uint32_t   buf2nextdescaddr;      /*!< buffer2 or next descriptor address pointer */
} emac_dma_desc_type;

/**
  * @brief type define emac mac register all
  */
typedef struct
{
  /**
    * @brief emac mac ctrl register, offset:0x00
    */
  union
  {
    __IO uint32_t ctrl;
    struct
    {
      __IO uint32_t reserved1            : 2; /* [0:1] */
      __IO uint32_t re                   : 1; /* [2] */
      __IO uint32_t te                   : 1; /* [3] */
      __IO uint32_t dc                   : 1; /* [4] */
      __IO uint32_t bl                   : 2; /* [5:6] */
      __IO uint32_t acs                  : 1; /* [7] */
      __IO uint32_t reserved2            : 1; /* [8] */
      __IO uint32_t dr                   : 1; /* [9] */
      __IO uint32_t ipc                  : 1; /* [10] */
      __IO uint32_t dm                   : 1; /* [11] */
      __IO uint32_t lm                   : 1; /* [12] */
      __IO uint32_t dro                  : 1; /* [13] */
      __IO uint32_t fes                  : 1; /* [14] */
      __IO uint32_t reserved3            : 1; /* [15] */
      __IO uint32_t dcs                  : 1; /* [16] */
      __IO uint32_t ifg                  : 3; /* [17:19] */
      __IO uint32_t reserved4            : 2; /* [20:21] */
      __IO uint32_t jd                   : 1; /* [22] */
      __IO uint32_t wd                   : 1; /* [23] */
      __IO uint32_t reserved5            : 8; /* [24:31] */
    } ctrl_bit;
  };

  /**
    * @brief emac mac frmf register, offset:0x04
    */
  union
  {
    __IO uint32_t frmf;
    struct
    {
      __IO uint32_t pr                   : 1; /* [0] */
      __IO uint32_t huc                  : 1; /* [1] */
      __IO uint32_t hmc                  : 1; /* [2] */
      __IO uint32_t daif                 : 1; /* [3] */
      __IO uint32_t pmc                  : 1; /* [4] */
      __IO uint32_t dbf                  : 1; /* [5] */
      __IO uint32_t pcf                  : 2; /* [6:7] */
      __IO uint32_t saif                 : 1; /* [8] */
      __IO uint32_t saf                  : 1; /* [9] */
      __IO uint32_t hpf                  : 1; /* [10] */
      __IO uint32_t reserved1            : 20;/* [11:30] */
      __IO uint32_t ra                   : 1; /* [31] */
    } frmf_bit;
  };

  /**
    * @brief emac mac hth register, offset:0x08
    */
  union
  {
    __IO uint32_t hth;
    struct
    {
      __IO uint32_t hth                  : 32; /* [0:31] */
    } hth_bit;
  };

  /**
    * @brief emac mac htl register, offset:0x0c
    */
  union
  {
    __IO uint32_t htl;
    struct
    {
      __IO uint32_t htl                  : 32; /* [0:31] */
    } htl_bit;
  };

  /**
    * @brief emac mac miiaddr register, offset:0x10
    */
  union
  {
    __IO uint32_t miiaddr;
    struct
    {
      __IO uint32_t mb                   : 1; /* [0] */
      __IO uint32_t mw                   : 1; /* [1] */
      __IO uint32_t cr                   : 4; /* [2:5] */
      __IO uint32_t mii                  : 5; /* [6:10] */
      __IO uint32_t pa                   : 5; /* [11:15] */
      __IO uint32_t reserved1            : 16;/* [16:31] */
    } miiaddr_bit;
  };

  /**
    * @brief emac mac miidt register, offset:0x14
    */
  union
  {
    __IO uint32_t miidt;
    struct
    {
      __IO uint32_t md                   : 16;/* [0:15] */
      __IO uint32_t reserved1            : 16;/* [16:31] */
    } miidt_bit;
  };

  /**
    * @brief emac mac fctrl register, offset:0x18
    */
  union
  {
    __IO uint32_t fctrl;
    struct
    {
      __IO uint32_t fcbbpa               : 1; /* [0] */
      __IO uint32_t etf                  : 1; /* [1] */
      __IO uint32_t erf                  : 1; /* [2] */
      __IO uint32_t dup                  : 1; /* [3] */
      __IO uint32_t plt                  : 2; /* [4:5] */
      __IO uint32_t reserved1            : 1; /* [6] */
      __IO uint32_t dzqp                 : 1; /* [7] */
      __IO uint32_t reserved2            : 8; /* [8:15] */
      __IO uint32_t pt                   : 16;/* [16:31] */
    } fctrl_bit;
  };

  /**
    * @brief emac mac vlt register, offset:0x1C
    */
  union
  {
    __IO uint32_t vlt;
    struct
    {
      __IO uint32_t vti                  : 16;/* [0:15] */
      __IO uint32_t etv                  : 1; /* [16] */
      __IO uint32_t reserved1            : 15;/* [17:31] */
    } vlt_bit;
  };

  /**
    * @brief emac mac reserved1 register, offset:0x20~0x24
    */
  __IO uint32_t reserved1[2];

  /**
    * @brief emac mac rwff register, offset:0x28
    */
  __IO uint32_t rwff;

  /**
    * @brief emac mac pmtctrlsts register, offset:0x2C
    */
  union
  {
    __IO uint32_t pmtctrlsts;
    struct
    {
      __IO uint32_t pd                   : 1; /* [0] */
      __IO uint32_t emp                  : 1; /* [1] */
      __IO uint32_t erwf                 : 1; /* [2] */
      __IO uint32_t reserved1            : 2; /* [3:4] */
      __IO uint32_t rmp                  : 1; /* [5] */
      __IO uint32_t rrwf                 : 1; /* [6] */
      __IO uint32_t reserved2            : 2; /* [7:8] */
      __IO uint32_t guc                  : 1; /* [9] */
      __IO uint32_t reserved3            : 21;/* [10:30] */
      __IO uint32_t rwffpr               : 1; /* [31] */
    } pmtctrlsts_bit;
  };

  /**
    * @brief emac mac reserved2 register, offset:0x30~0x34
    */
  __IO uint32_t reserved2[2];

  /**
    * @brief emac mac ists register, offset:0x38
    */
  union
  {
    __IO uint32_t ists;
    struct
    {
      __IO uint32_t reserved1            : 3; /* [0:2] */
      __IO uint32_t pis                  : 1; /* [3] */
      __IO uint32_t mis                  : 1; /* [4] */
      __IO uint32_t mris                 : 1; /* [5] */
      __IO uint32_t mtis                 : 1; /* [6] */
      __IO uint32_t reserved2            : 2; /* [7:8] */
      __IO uint32_t tis                  : 1; /* [9] */
      __IO uint32_t reserved3            : 22;/* [10:31] */
    } ists_bit;
  };

  /**
    * @brief emac mac imr register, offset:0x3C
    */
  union
  {
    __IO uint32_t imr;
    struct
    {
      __IO uint32_t reserved1            : 3; /* [0:2] */
      __IO uint32_t pim                  : 1; /* [3] */
      __IO uint32_t reserved2            : 5; /* [4:8] */
      __IO uint32_t tim                  : 1; /* [9] */
      __IO uint32_t reserved3            : 22;/* [10:31] */
    } imr_bit;
  };

  /**
    * @brief emac mac a0h register, offset:0x40
    */
  union
  {
    __IO uint32_t a0h;
    struct
    {
      __IO uint32_t ma0h                 : 16;/* [0:15] */
      __IO uint32_t reserved1            : 15;/* [16:30] */
      __IO uint32_t ae                   : 1; /* [31] */
    } a0h_bit;
  };

   /**
    * @brief emac mac a0l register, offset:0x44
    */
  union
  {
    __IO uint32_t a0l;
    struct
    {
      __IO uint32_t ma0l                 : 32;/* [0:31] */
    } a0l_bit;
  };

  /**
    * @brief emac mac a1h register, offset:0x48
    */
  union
  {
    __IO uint32_t a1h;
    struct
    {
      __IO uint32_t ma1h                 : 16;/* [0:15] */
      __IO uint32_t reserved1            : 8; /* [16:23] */
      __IO uint32_t mbc                  : 6; /* [24:29] */
      __IO uint32_t sa                   : 1; /* [30] */
      __IO uint32_t ae                  : 1; /* [31] */
    } a1h_bit;
  };

  /**
    * @brief emac mac a1l register, offset:0x4C
    */
  union
  {
    __IO uint32_t a1l;
    struct
    {
      __IO uint32_t ma1l                 : 32;/* [0:31] */
    } a1l_bit;
  };

  /**
    * @brief emac mac a2h register, offset:0x50
    */
  union
  {
    __IO uint32_t a2h;
    struct
    {
      __IO uint32_t ma2h                 : 16;/* [0:15] */
      __IO uint32_t reserved1            : 8; /* [16:23] */
      __IO uint32_t mbc                  : 6; /* [24:29] */
      __IO uint32_t sa                   : 1; /* [30] */
      __IO uint32_t ae                   : 1; /* [31] */
    } a2h_bit;
  };

  /**
    * @brief emac mac a2l register, offset:0x54
    */
  union
  {
    __IO uint32_t a2l;
    struct
    {
      __IO uint32_t ma2l                 : 32;/* [0:31] */
    } a2l_bit;
  };

  /**
    * @brief emac mac a3h register, offset:0x58
    */
  union
  {
    __IO uint32_t a3h;
    struct
    {
      __IO uint32_t ma3h                 : 16;/* [0:15] */
      __IO uint32_t reserved1            : 8; /* [16:23] */
      __IO uint32_t mbc                  : 6; /* [24:29] */
      __IO uint32_t sa                   : 1; /* [30] */
      __IO uint32_t ae                   : 1; /* [31] */
    } a3h_bit;
  };

  /**
    * @brief emac mac a3l register, offset:0x5C
    */
  union
  {
    __IO uint32_t a3l;
    struct
    {
      __IO uint32_t ma3l                 : 32;/* [0:31] */
    } a3l_bit;
  };
} emac_type;

/**
  * @brief type define emac mmc register all
  */
typedef struct
{
  /**
    * @brief emac mmc ctrl register, offset:0x0100
    */
  union
  {
    __IO uint32_t ctrl;
    struct
    {
      __IO uint32_t rc                   : 1; /* [0] */
      __IO uint32_t scr                  : 1; /* [1] */
      __IO uint32_t rr                   : 1; /* [2] */
      __IO uint32_t fmc                  : 1; /* [3] */
      __IO uint32_t reserved1            : 28;/* [4:31] */
    } ctrl_bit;
  };

  /**
    * @brief emac mmc ri register, offset:0x0104
    */
  union
  {
    __IO uint32_t ri;
    struct
    {
      __IO uint32_t reserved1            : 5; /* [0:4] */
      __IO uint32_t rfce                 : 1; /* [5] */
      __IO uint32_t rfae                 : 1; /* [6] */
      __IO uint32_t reserved2            : 10;/* [7:16] */
      __IO uint32_t rguf                 : 1; /* [17] */
      __IO uint32_t reserved3            : 14;/* [18:31] */
    } ri_bit;
  };

  /**
    * @brief emac mmc ti register, offset:0x0108
    */
  union
  {
    __IO uint32_t ti;
    struct
    {
      __IO uint32_t reserved1            : 14;/* [0:13] */
      __IO uint32_t tscgfci              : 1; /* [14] */
      __IO uint32_t tgfmsc               : 1; /* [15] */
      __IO uint32_t reserved2            : 5; /* [16:20] */
      __IO uint32_t tgf                  : 1; /* [21] */
      __IO uint32_t reserved3            : 10;/* [22:31] */
    } ti_bit;
  };

  /**
    * @brief emac mmc rim register, offset:0x010C
    */
  union
  {
    __IO uint32_t rim;
    struct
    {
      __IO uint32_t reserved1            : 5; /* [0:4] */
      __IO uint32_t rcefcim              : 1; /* [5] */
      __IO uint32_t raefacim             : 1; /* [6] */
      __IO uint32_t reserved2            : 10;/* [7:16] */
      __IO uint32_t rugfcim              : 1; /* [17] */
      __IO uint32_t reserved3            : 14;/* [18:31] */
    } rim_bit;
  };

  /**
    * @brief emac mmc tim register, offset:0x0110
    */
  union
  {
    __IO uint32_t tim;
    struct
    {
      __IO uint32_t reserved1            : 14;/* [0:13] */
      __IO uint32_t tscgfcim             : 1; /* [14] */
      __IO uint32_t tmcgfcim             : 1; /* [15] */
      __IO uint32_t reserved2            : 5; /* [16:20] */
      __IO uint32_t tgfcim               : 1; /* [21] */
      __IO uint32_t reserved3            : 10;/* [22:31] */
    } tim_bit;
  };

  /**
    * @brief emac mmc reserved1 register, offset:0x0114~0x0148
    */
  __IO uint32_t reserved1[14];

  /**
    * @brief emac mmc tfscc register, offset:0x014C
    */
  union
  {
    __IO uint32_t tfscc;
    struct
    {
      __IO uint32_t tgfscc               : 32;/* [0:31] */
    } tfscc_bit;
  };

  /**
    * @brief emac mmc tfmscc register, offset:0x0150
    */
  union
  {
    __IO uint32_t tfmscc;
    struct
    {
      __IO uint32_t tgfmscc              : 32;/* [0:31] */
    } tfmscc_bit;
  };

  /**
    * @brief emac mmc reserved2 register, offset:0x0154~0x0164
    */
  __IO uint32_t reserved2[5];

  /**
    * @brief emac mmc tfcnt register, offset:0x0168
    */
  union
  {
    __IO uint32_t tfcnt;
    struct
    {
      __IO uint32_t tgfc                 : 32;/* [0:31] */
    } tfcnt_bit;
  };

  /**
    * @brief emac mmc reserved3 register, offset:0x016C~0x0190
    */
  __IO uint32_t reserved3[10];

  /**
    * @brief emac mmc rfcecnt register, offset:0x0194
    */
  union
  {
    __IO uint32_t rfcecnt;
    struct
    {
      __IO uint32_t rfcec                : 32;/* [0:31] */
    } rfcecnt_bit;
  };

  /**
    * @brief emac mmc rfaecnt register, offset:0x0198
    */
  union
  {
    __IO uint32_t rfaecnt;
    struct
    {
      __IO uint32_t rfaec                : 32;/* [0:31] */
    } rfaecnt_bit;
  };

  /**
    * @brief emac mmc reserved4 register, offset:0x019C~0x01C0
    */
  __IO uint32_t reserved4[10];

  /**
    * @brief emac mmc rgufcnt register, offset:0x01C4
    */
  union
  {
    __IO uint32_t rgufcnt;
    struct
    {
      __IO uint32_t rgufc                : 32;/* [0:31] */
    } rgufcnt_bit;
  };
} emac_mmc_type;

/**
  * @brief type define emac ptp register all
  */
typedef struct
{
  /**
    * @brief emac ptp tsctrl register, offset:0x0700
    */
  union
  {
    __IO uint32_t tsctrl;
    struct
    {
      __IO uint32_t te                   : 1; /* [0] */
      __IO uint32_t tfcu                 : 1; /* [1] */
      __IO uint32_t ti                   : 1; /* [2] */
      __IO uint32_t tu                   : 1; /* [3] */
      __IO uint32_t tite                 : 1; /* [4] */
      __IO uint32_t aru                  : 1; /* [5] */
      __IO uint32_t reserved1            : 2; /* [6:7] */
      __IO uint32_t etaf                 : 1; /* [8] */
      __IO uint32_t tdbrc                : 1; /* [9] */
      __IO uint32_t eppv2f               : 1; /* [10] */
      __IO uint32_t eppef                : 1; /* [11] */
      __IO uint32_t eppfsip6u            : 1; /* [12] */
      __IO uint32_t eppfsip4u            : 1; /* [13] */
      __IO uint32_t etsfem               : 1; /* [14] */
      __IO uint32_t esfmrtm              : 1; /* [15] */
      __IO uint32_t sppfts               : 2; /* [16:17] */
      __IO uint32_t emafpff              : 1; /* [18] */
      __IO uint32_t reserved2            : 13;/* [19:31] */
    } tsctrl_bit;
  };

  /**
    * @brief emac ptp ssinc register, offset:0x0704
    */
  union
  {
    __IO uint32_t ssinc;
    struct
    {
      __IO uint32_t ssiv                 : 8; /* [0] */
      __IO uint32_t reserved1            : 24;/* [8:31] */
    } ssinc_bit;
  };

  /**
    * @brief emac ptp tsh register, offset:0x0708
    */
  union
  {
    __IO uint32_t tsh;
    struct
    {
      __IO uint32_t ts                   : 32;/* [0:31] */
    } tsh_bit;
  };

  /**
    * @brief emac ptp tsl register, offset:0x070C
    */
  union
  {
    __IO uint32_t tsl;
    struct
    {
      __IO uint32_t tss                  : 31;/* [0:30] */
      __IO uint32_t ast                  : 1; /* [31] */
    } tsl_bit;
  };

  /**
    * @brief emac ptp tshud register, offset:0x0710
    */
  union
  {
    __IO uint32_t tshud;
    struct
    {
      __IO uint32_t ts                   : 32;/* [0:31] */
    } tshud_bit;
  };

  /**
    * @brief emac ptp tslud register, offset:0x0714
    */
  union
  {
    __IO uint32_t tslud;
    struct
    {
      __IO uint32_t tss                  : 31;/* [0:30] */
      __IO uint32_t ast                  : 1; /* [31] */
    } tslud_bit;
  };

  /**
    * @brief emac ptp tsad register, offset:0x0718
    */
  union
  {
    __IO uint32_t tsad;
    struct
    {
      __IO uint32_t tar                  : 32;/* [0:31] */
    } tsad_bit;
  };

  /**
    * @brief emac ptp tth register, offset:0x071C
    */
  union
  {
    __IO uint32_t tth;
    struct
    {
      __IO uint32_t ttsr                 : 32;/* [0:31] */
    } tth_bit;
  };

  /**
    * @brief emac ptp ttl register, offset:0x0720
    */
  union
  {
    __IO uint32_t ttl;
    struct
    {
      __IO uint32_t ttlr                 : 32;/* [0:31] */
    } ttl_bit;
  };

  /**
    * @brief emac ptp reserved register, offset:0x0724
    */
  __IO uint32_t reserved1;

   /**
    * @brief emac ptp tssr register, offset:0x0728
    */
  union
  {
    __IO uint32_t tssr;
    struct
    {
      __IO uint32_t tso                  : 1; /* [0] */
      __IO uint32_t tttr                 : 1; /* [1] */
      __IO uint32_t reserved1            : 30;/* [2:31] */
    } tssr_bit;
  };

  /**
    * @brief emac ptp ppscr register, offset:0x072C
    */
  union
  {
    __IO uint32_t ppscr;
    struct
    {
      __IO uint32_t pofc                 : 4; /* [0:3] */
      __IO uint32_t reserved1            : 28;/* [4:31] */
    } ppscr_bit;
  };
} emac_ptp_type;

/**
  * @brief type define emac ptp register all
  */
typedef struct
{
  /**
    * @brief emac dma bm register, offset:0x1000
    */
  union
  {
    __IO uint32_t bm;
    struct
    {
      __IO uint32_t swr                  : 1; /* [0] */
      __IO uint32_t da                   : 1; /* [1] */
      __IO uint32_t dsl                  : 5; /* [2:6] */
      __IO uint32_t reserved1            : 1; /* [7] */
      __IO uint32_t pbl                  : 6; /* [8:13] */
      __IO uint32_t pr                   : 2; /* [14:15] */
      __IO uint32_t fb                   : 1; /* [16] */
      __IO uint32_t rdp                  : 6; /* [17:22] */
      __IO uint32_t usp                  : 1; /* [23] */
      __IO uint32_t pblx8                : 1; /* [24] */
      __IO uint32_t aab                  : 1; /* [25] */
      __IO uint32_t reserved2            : 6; /* [26:31] */
    } bm_bit;
  };

  /**
    * @brief emac dma tpd register, offset:0x1004
    */
  union
  {
    __IO uint32_t tpd;
    struct
    {
      __IO uint32_t tpd                  : 32; /* [0:31] */
    } tpd_bit;
  };

  /**
    * @brief emac dma rpd register, offset:0x1008
    */
  union
  {
    __IO uint32_t rpd;
    struct
    {
      __IO uint32_t rpd                  : 32; /* [0:31] */
    } rpd_bit;
  };

  /**
    * @brief emac dma rdladdr register, offset:0x100c
    */
  union
  {
    __IO uint32_t rdladdr;
    struct
    {
      __IO uint32_t srl                  : 32; /* [0:31] */
    } rdladdr_bit;
  };

  /**
    * @brief emac dma tdladdr register, offset:0x1010
    */
  union
  {
    __IO uint32_t tdladdr;
    struct
    {
      __IO uint32_t stl                  : 32; /* [0:31] */
    } tdladdr_bit;
  };

  /**
    * @brief emac dma sts register, offset:0x1014
    */
  union
  {
    __IO uint32_t sts;
    struct
    {
      __IO uint32_t ti                   : 1; /* [0] */
      __IO uint32_t tps                  : 1; /* [1] */
      __IO uint32_t tbu                  : 1; /* [2] */
      __IO uint32_t tjt                  : 1; /* [3] */
      __IO uint32_t ovf                  : 1; /* [4] */
      __IO uint32_t unf                  : 1; /* [5] */
      __IO uint32_t ri                   : 1; /* [6] */
      __IO uint32_t rbu                  : 1; /* [7] */
      __IO uint32_t rps                  : 1; /* [8] */
      __IO uint32_t rwt                  : 1; /* [9] */
      __IO uint32_t eti                  : 1; /* [10] */
      __IO uint32_t reserved1            : 2; /* [11:12] */
      __IO uint32_t fbei                 : 1; /* [13] */
      __IO uint32_t eri                  : 1; /* [14] */
      __IO uint32_t ais                  : 1; /* [15] */
      __IO uint32_t nis                  : 1; /* [16] */
      __IO uint32_t rs                   : 3; /* [17:19] */
      __IO uint32_t ts                   : 3; /* [20:22] */
      __IO uint32_t eb                   : 3; /* [23:25] */
      __IO uint32_t reserved2            : 1; /* [26] */
      __IO uint32_t mmi                  : 1; /* [27] */
      __IO uint32_t mpi                  : 1; /* [28] */
      __IO uint32_t tti                  : 1; /* [29] */
      __IO uint32_t reserved3            : 2; /* [30:31] */
    } sts_bit;
  };

  /**
    * @brief emac dma opm register, offset:0x1018
    */
  union
  {
    __IO uint32_t opm;
    struct
    {
      __IO uint32_t reserved1            : 1; /* [0] */
      __IO uint32_t ssr                  : 1; /* [1] */
      __IO uint32_t osf                  : 1; /* [2] */
      __IO uint32_t rtc                  : 2; /* [3:4] */
      __IO uint32_t reserved2            : 1; /* [5] */
      __IO uint32_t fugf                 : 1; /* [6] */
      __IO uint32_t fef                  : 1; /* [7] */
      __IO uint32_t reserved3            : 5; /* [8:12] */
      __IO uint32_t sstc                 : 1; /* [13] */
      __IO uint32_t ttc                  : 3; /* [14:16] */
      __IO uint32_t reserved4            : 3; /* [17:19] */
      __IO uint32_t ftf                  : 1; /* [20] */
      __IO uint32_t tsf                  : 1; /* [21] */
      __IO uint32_t reserved5            : 2; /* [22:23] */
      __IO uint32_t dfrf                 : 1; /* [24] */
      __IO uint32_t rsf                  : 1; /* [25] */
      __IO uint32_t dt                   : 1; /* [26] */
      __IO uint32_t reserved6            : 5; /* [27:31] */
    } opm_bit;
  };

  /**
    * @brief emac dma ie register, offset:0x101C
    */
  union
  {
    __IO uint32_t ie;
    struct
    {
      __IO uint32_t tie                  : 1; /* [0] */
      __IO uint32_t tse                  : 1; /* [1] */
      __IO uint32_t tue                  : 1; /* [2] */
      __IO uint32_t tje                  : 1; /* [3] */
      __IO uint32_t ove                  : 1; /* [4] */
      __IO uint32_t une                  : 1; /* [5] */
      __IO uint32_t rie                  : 1; /* [6] */
      __IO uint32_t rbue                 : 1; /* [7] */
      __IO uint32_t rse                  : 1; /* [8] */
      __IO uint32_t rwte                 : 1; /* [9] */
      __IO uint32_t eie                  : 1; /* [10] */
      __IO uint32_t reserved1            : 2; /* [11:12] */
      __IO uint32_t fbee                 : 1; /* [13] */
      __IO uint32_t ere                  : 1; /* [14] */
      __IO uint32_t aie                  : 1; /* [15] */
      __IO uint32_t nie                  : 1; /* [16] */
      __IO uint32_t reserved2            : 15;/* [17:31] */
    } ie_bit;
  };

  /**
    * @brief emac dma mfbocnt register, offset:0x1020
    */
  union
  {
    __IO uint32_t mfbocnt;
    struct
    {
      __IO uint32_t mfc                  : 16;/* [0:15] */
      __IO uint32_t obmfc                : 1; /* [16] */
      __IO uint32_t ofc                  : 11;/* [17:27] */
      __IO uint32_t obfoc                : 1; /* [28] */
      __IO uint32_t reserved1            : 3; /* [29:31] */
    } mfbocnt_bit;
  };

  /**
    * @brief emac dma reserved1 register, offset:0x1024~0x1044
    */
  __IO uint32_t reserved1[9];

  /**
    * @brief emac ctd register, offset:0x1048
    */
  union
  {
    __IO uint32_t ctd;
    struct
    {
      __IO uint32_t htdap                : 32;/* [0:31] */
    } ctd_bit;
  };

  /**
    * @brief emac crd register, offset:0x104C
    */
  union
  {
    __IO uint32_t crd;
    struct
    {
      __IO uint32_t hrdap                : 32;/* [0:31] */
    } crd_bit;
  };

  /**
    * @brief emac ctbaddr register, offset:0x1050
    */
  union
  {
    __IO uint32_t ctbaddr;
    struct
    {
      __IO uint32_t htbap                : 32;/* [0:31] */
    } ctbaddr_bit;
  };

  /**
    * @brief emac crbaddr register, offset:0x1054
    */
  union
  {
    __IO uint32_t crbaddr;
    struct
    {
      __IO uint32_t hrbap                : 32;/* [0:31] */
    } crbaddr_bit;
  };
} emac_dma_type;

/**
  * @}
  */

#define EMAC                             ((emac_type *) EMAC_BASE)
#define EMAC_MMC                         ((emac_mmc_type *) EMAC_MMC_BASE)
#define EMAC_PTP                         ((emac_ptp_type *) EMAC_PTP_BASE)
#define EMAC_DMA                         ((emac_dma_type *) EMAC_DMA_BASE)

/** @defgroup EMAC_exported_functions
  * @{
  */

void emac_reset(void);
void emac_clock_range_set(void);
void emac_dma_software_reset_set(void);
flag_status emac_dma_software_reset_get(void);
void emac_start(void);
void emac_stop(void);
error_status emac_phy_register_write(uint8_t address, uint8_t reg, uint16_t data);
error_status emac_phy_register_read(uint8_t address, uint8_t reg, uint16_t *data);
void emac_control_para_init(emac_control_config_type *control_para);
void emac_control_config(emac_control_config_type *control_struct);
void emac_receiver_enable(confirm_state new_state);
void emac_trasmitter_enable(confirm_state new_state);
void emac_deferral_check_set(confirm_state new_state);
void emac_backoff_limit_set(emac_bol_type slot_time);
void emac_auto_pad_crc_stripping_set(confirm_state new_state);
void emac_retry_disable(confirm_state new_state);
void emac_ipv4_checksum_offload_set(confirm_state new_state);
void emac_loopback_mode_enable(confirm_state new_state);
void emac_receive_own_disable(confirm_state new_state);
void emac_carrier_sense_disable(confirm_state new_state);
void emac_interframe_gap_set(emac_intergrame_gap_type number);
void emac_jabber_disable(confirm_state new_state);
void emac_watchdog_disable(confirm_state new_state);
void emac_fast_speed_set(emac_speed_type speed);
void emac_duplex_mode_set(emac_duplex_type duplex_mode);
void emac_promiscuous_mode_set(confirm_state new_state);
void emac_hash_unicast_set(confirm_state new_state);
void emac_hash_multicast_set(confirm_state new_state);
void emac_dstaddr_inverse_filter_set(confirm_state new_state);
void emac_pass_all_multicasting_set(confirm_state new_state);
void emac_broadcast_frames_disable(confirm_state new_state);
void emac_pass_control_frames_set(emac_control_frames_filter_type condition);
void emac_srcaddr_inverse_filter_set(confirm_state new_state);
void emac_srcaddr_filter_set(confirm_state new_state);
void emac_hash_perfect_filter_set(confirm_state new_state);
void emac_receive_all_set(confirm_state new_state);
void emac_hash_table_high32bits_set(uint32_t high32bits);
void emac_hash_table_low32bits_set(uint32_t low32bits);
flag_status emac_mii_busy_get(void);
void emac_mii_write(confirm_state new_state);
void emac_fcb_bpa_set(confirm_state new_state);
void emac_transmit_flow_control_enable(confirm_state new_state);
void emac_receive_flow_control_enable(confirm_state new_state);
void emac_unicast_pause_frame_detect(confirm_state new_state);
void emac_pause_low_threshold_set(emac_pause_slot_threshold_type pasue_threshold);
void emac_zero_quanta_pause_disable(confirm_state new_state);
void emac_pause_time_set(uint16_t pause_time);
void emac_vlan_tag_identifier_set(uint16_t identifier);
void emac_vlan_tag_comparison_set(confirm_state new_state);
void emac_wakeup_frame_set(uint32_t value);
uint32_t emac_wakeup_frame_get(void);
void emac_power_down_set(confirm_state new_state);
void emac_magic_packet_enable(confirm_state new_state);
void emac_wakeup_frame_enable(confirm_state new_state);
flag_status emac_received_magic_packet_get(void);
flag_status emac_received_wakeup_frame_get(void);
void emac_global_unicast_set(confirm_state new_state);
void emac_wakeup_frame_filter_reset(confirm_state new_state);
flag_status emac_interrupt_status_read(uint32_t flag);
void emac_interrupt_mask_set(emac_interrupt_mask_type mask_type, confirm_state new_state);
void emac_local_address_set(uint8_t *address);
void emac_address_filter_set(emac_address_type mac, emac_address_filter_type filter, emac_address_mask_type mask_bit, confirm_state new_state);
uint32_t emac_received_packet_size_get(void);
uint32_t emac_dmarxdesc_frame_length_get(emac_dma_desc_type *dma_rx_desc);
void emac_dma_descriptor_list_address_set(emac_dma_tx_rx_type transfer_type, emac_dma_desc_type *dma_desc_tab, uint8_t *buff, uint32_t buffer_count);
uint32_t emac_dma_descriptor_list_address_get(emac_dma_tx_rx_type transfer_type);
void emac_dma_rx_desc_interrupt_config(emac_dma_desc_type *dma_rx_desc, confirm_state new_state);
void emac_dma_para_init(emac_dma_config_type *control_para);
void emac_dma_config(emac_dma_config_type *control_para);
void emac_dma_arbitation_set(emac_dma_rx_tx_ratio_type ratio, confirm_state new_state);
void emac_dma_descriptor_skip_length_set(uint8_t length);
void emac_dma_separate_pbl_set(emac_dma_pbl_type tx_length, emac_dma_pbl_type rx_length, confirm_state new_state);
void emac_dma_eight_pbl_mode_set(confirm_state new_state);
void emac_dma_address_aligned_beats_set(confirm_state new_state);
void emac_dma_poll_demand_set(emac_dma_tx_rx_type transfer_type, uint32_t value);
uint32_t emac_dma_poll_demand_get(emac_dma_tx_rx_type transfer_type);
emac_dma_receive_process_status_type emac_dma_receive_status_get(void);
emac_dma_transmit_process_status_type emac_dma_transmit_status_get(void);
void emac_dma_operations_set(emac_dma_operations_type ops, confirm_state new_state);
void emac_dma_receive_threshold_set(emac_dma_receive_threshold_type value);
void emac_dma_transmit_threshold_set(emac_dma_transmit_threshold_type value);
void emac_dma_interrupt_enable(emac_dma_interrupt_type it, confirm_state new_state);
uint16_t emac_dma_controller_missing_frame_get(void);
uint8_t emac_dma_missing_overflow_bit_get(void);
uint16_t emac_dma_application_missing_frame_get(void);
uint8_t emac_dma_fifo_overflow_bit_get(void);
uint32_t emac_dma_tansfer_address_get(emac_dma_transfer_address_type transfer_type);
void emac_mmc_counter_reset(void);
void emac_mmc_rollover_stop(confirm_state new_state);
void emac_mmc_reset_on_read_enable(confirm_state new_state);
void emac_mmc_counter_freeze(confirm_state new_state);
flag_status emac_mmc_received_status_get(uint32_t flag);
flag_status emac_mmc_transmit_status_get(uint32_t flag);
void emac_mmc_received_interrupt_mask_set(uint32_t flag, confirm_state new_state);
void emac_mmc_transmit_interrupt_mask_set(uint32_t flag, confirm_state new_state);
uint32_t emac_mmc_transmit_good_frames_get(uint32_t flag);
uint32_t emac_mmc_received_error_frames_get(uint32_t flag);
void emac_ptp_timestamp_enable(confirm_state new_state);
void emac_ptp_timestamp_fine_update_enable(confirm_state new_state);
void emac_ptp_timestamp_system_time_init(confirm_state new_state);
void emac_ptp_timestamp_system_time_update(confirm_state new_state);
void emac_ptp_interrupt_trigger_enable(confirm_state new_state);
void emac_ptp_addend_register_update(confirm_state new_state);
void emac_ptp_snapshot_received_frames_enable(confirm_state new_state);
void emac_ptp_subsecond_rollover_enable(confirm_state new_state);
void emac_ptp_psv2_enable(confirm_state new_state);
void emac_ptp_snapshot_emac_frames_enable(confirm_state new_state);
void emac_ptp_snapshot_ipv6_frames_enable(confirm_state new_state);
void emac_ptp_snapshot_ipv4_frames_enable(confirm_state new_state);
void emac_ptp_snapshot_event_message_enable(confirm_state new_state);
void emac_ptp_snapshot_master_event_enable(confirm_state new_state);
void emac_ptp_clock_node_set(emac_ptp_clock_node_type node);
void emac_ptp_mac_address_filter_enable(confirm_state new_state);
void emac_ptp_subsecond_increment_set(uint8_t value);
uint32_t emac_ptp_system_second_get(void);
uint32_t emac_ptp_system_subsecond_get(void);
confirm_state emac_ptp_system_time_sign_get(void);
void emac_ptp_system_second_set(uint32_t second);
void emac_ptp_system_subsecond_set(uint32_t subsecond);
void emac_ptp_system_time_sign_set(confirm_state sign);
void emac_ptp_timestamp_addend_set(uint32_t value);
void emac_ptp_target_second_set(uint32_t value);
void emac_ptp_target_nanosecond_set(uint32_t value);
confirm_state emac_ptp_timestamp_status_get(emac_ptp_timestamp_status_type status);
void emac_ptp_pps_frequency_set(emac_ptp_pps_control_type freq);
flag_status emac_dma_flag_get(uint32_t dma_flag);
void emac_dma_flag_clear(uint32_t dma_flag);

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
