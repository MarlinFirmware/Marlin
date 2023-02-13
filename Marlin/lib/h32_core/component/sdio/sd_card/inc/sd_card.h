/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file sd_card.h
 **
 ** A detailed description is available at
 ** @link SdiocGroup SDIOC description @endlink
 **
 **   - 2018-11-15  1.0 Hongjh First version for Device Driver Library of SDIOC.
 **
 ******************************************************************************/
#ifndef __SD_CARD_H__
#define __SD_CARD_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "sdmmc_cmd.h"
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup SdiocGroup Secure Digital Input and Output Controller(SDIOC)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/

    /**
     *******************************************************************************
     ** \brief SD Card version definition
     **
     ******************************************************************************/
    typedef enum en_sd_card_version
    {
        SdCardVer1x = 0u, ///< SD Card version: 1.x
        SdCardVer2x = 1u, ///< SD Card version: 2.x
    } en_sd_card_version_t;

    /**
     *******************************************************************************
     ** \brief SD Card device Read/Write mode
     **
     ******************************************************************************/
    typedef enum en_sd_card_device_mode
    {
        SdCardPollingMode = 0u, ///< Polling mode transfer
        SdCardDmaMode = 1u,     ///< DMA mode transfer
    } en_sd_card_device_mode_t;

    /**
     *******************************************************************************
     ** \brief SD Card Information Structure definition
     **
     ******************************************************************************/
    typedef struct stc_sd_card_info
    {
        uint32_t u32CardType; ///< Specifies the card Type

        uint32_t u32CardVersion; ///< Specifies the card version

        uint32_t u32Class; ///< Specifies the class of the card class

        uint32_t u32RelCardAddr; ///< Specifies the Relative Card Address

        uint32_t u32BlockNbr; ///< Specifies the Card Capacity in blocks

        uint32_t u32BlockSize; ///< Specifies one block size in bytes

        uint32_t u32LogBlockNbr; ///< Specifies the Card logical Capacity in blocks

        uint32_t u32LogBlockSize; ///< Specifies logical block size in bytes

    } stc_sd_card_info_t;

    /**
     *******************************************************************************
     ** \brief CSD Register ver1.0(for Standard Capacity Card)
     **
     ******************************************************************************/
    typedef struct stc_sdcard_csd_v1
    {
        /* Byte 1 */
        uint8_t RESERVE1 : 2;
        uint8_t FILE_FORMAT : 2;
        uint8_t TMP_WRITE_PROTECT : 1;
        uint8_t PERM_WRITE_PROTECT : 1;
        uint8_t COPY : 1;
        uint8_t FILE_FORMAT_GRP : 1;

        /* Byte 2 */
        uint8_t RESERVE2 : 5;
        uint8_t WRITE_BL_PARTIAL : 1;
        uint8_t WRITE_BL_LEN1 : 2;
        /* Byte 3 */
        uint8_t WRITE_BL_LEN2 : 2;
        uint8_t R2W_FACTOR : 3;
        uint8_t RESERVE3 : 2;
        uint8_t WP_GRP_ENABLE : 1;

        /* Byte 4 */
        uint8_t WP_GRP_SIZE : 7;
        uint8_t SECTOR_SIZE1 : 1;
        /* Byte 5 */
        uint8_t SECTOR_SIZE2 : 6;
        uint8_t ERASE_BLK_EN : 1;
        uint8_t C_SIZE_MULTI1 : 1;
        /* Byte 6 */
        uint8_t C_SIZE_MULTI2 : 2;
        uint8_t CDD_W_CURR_MAX : 3;
        uint8_t CDD_W_CURR_MIN : 3;

        /* Byte 7 */
        uint8_t VDD_R_CURR_MAX : 3;
        uint8_t VDD_R_CURR_MIN : 3;
        uint8_t C_SIZE1 : 2;
        /* Byte 8 */
        uint8_t C_SIZE2 : 8;
        /* Byte 9 */
        uint8_t C_SIZE3 : 2;
        uint8_t RESERVED4 : 2;
        uint8_t DSR_IMP : 1;
        uint8_t READ_BLK_MISALIGH : 1;
        uint8_t WRITE_BLK_MISALIGN : 1;
        uint8_t READ_BL_PARTIAL : 1;

        /* Byte 10 */
        uint8_t READ_BL_LEN : 4;
        uint8_t CCC1 : 4;
        /* Byte 11 */
        uint8_t CCC2 : 8;

        /* Byte 12 */
        uint8_t TRAN_SPEED : 8;

        /* Byte 13 */
        uint8_t NSAC : 8;

        /* Byte 14 */
        uint8_t TAAC : 8;

        /* Byte 15 */
        uint8_t RESERVE5 : 6;
        uint8_t CSD_STRUCTURE : 2;
    } stc_sdcard_csd_v1_t;

    /**
     *******************************************************************************
     ** \brief CSD Register ver2.0(for High Capacity Card)
     **
     ******************************************************************************/
    typedef struct stc_sdcard_csd_v2
    {
        /* Byte 1 */
        uint8_t RESERVED1 : 2;
        uint8_t FILE_FORMAT : 2;
        uint8_t TMP_WRITE_PROTECT : 1;
        uint8_t PERM_WRITE_PROTECT : 1;
        uint8_t COPY : 1;
        uint8_t FILE_FORMAT_GRP : 1;

        /* Byte 2 */
        uint8_t RESERVED2 : 5;
        uint8_t WRITE_BL_PARTIAL : 1;
        uint8_t WRITE_BL_LEN1 : 2;
        /* Byte 3 */
        uint8_t WRITE_BL_LEN2 : 2;
        uint8_t R2W_FACTOR : 3;
        uint8_t RESERVED3 : 2;
        uint8_t WP_GRP_ENABLE : 1;

        /* Byte 4 */
        uint8_t WP_GRP_SIZE : 7;
        uint8_t SECTOR_SIZE1 : 1;
        /* Byte 5 */
        uint8_t SECTOR_SIZE2 : 6;
        uint8_t ERASE_BLK_EN : 1;
        uint8_t RESERVED4 : 1;

        /* Byte 6 */
        uint8_t C_SIZE1 : 8;
        /* Byte 7 */
        uint8_t C_SIZE2 : 8;
        /* Byte 8 */
        uint8_t C_SIZE3 : 6;
        uint8_t RESERVED5 : 2;

        /* Byte 9 */
        uint8_t RESERVED6 : 4;
        uint8_t DSR_IMP : 1;
        uint8_t READ_BLK_MISALIGH : 1;
        uint8_t WRITE_BLK_MISALIGN : 1;
        uint8_t READ_BL_PARTIAL : 1;

        /* Byte 10 */
        uint8_t READ_BL_LEN : 4;
        uint8_t CCC1 : 4;

        /* Byte 11 */
        uint8_t CCC2 : 8;

        /* Byte 12 */
        uint8_t TRAN_SPEED : 8;

        /* Byte 13 */
        uint8_t NSAC : 8;

        /* Byte 14 */
        uint8_t TAAC : 8;

        /* Byte 15 */
        uint8_t RESERVED7 : 6;
        uint8_t CSD_STRUCTURE : 2;
    } stc_sdcard_csd_v2_t;

    /**
     *******************************************************************************
     ** \brief SD Card initilization parameters
     **
     ******************************************************************************/
    typedef struct stc_sdcard_init
    {
        en_sdioc_bus_width_t enBusWidth; ///< Specifies the SDIOC bus width.
                                         ///< This parameter can be a value of @ref en_sdioc_bus_width_t

        en_sdioc_clk_freq_t enClkFreq; ///< Specifies the SDIOC clock frequency.
                                       ///< This parameter can be a value of @ref en_sdioc_clk_freq_t

        en_sdioc_speed_mode_t enSpeedMode; ///< Specifies the SDIOC speed mode.
                                           ///< This parameter can be a value of @ref en_sdioc_speed_mode_t

        const stc_sdioc_init_t *pstcInitCfg; ///< SD required parameters
                                             ///< and this structure detail refer @ref stc_sdioc_init_t
    } stc_sdcard_init_t;

    /**
     *******************************************************************************
     ** \brief SD Card use DMA unit/channel parameters
     **
     ******************************************************************************/
    typedef struct stc_sdcard_dma_init
    {
        M4_DMA_TypeDef *DMAx; ///< Pointer to DMA registers base address
                              ///< and this structure detail refer @ref M4_DMA_TypeDef

        en_dma_channel_t enDmaCh; ///< Specifies the DMA channel.
                                  ///< This parameter can be a value of @ref en_dma_channel_t
    } stc_sdcard_dma_init_t;

    /**
     *******************************************************************************
     ** \brief SD handle Structure definition
     **
     ******************************************************************************/
    typedef struct stc_sd_handle
    {
        M4_SDIOC_TypeDef *SDIOCx; ///< Pointer to SD registers base address
                                  ///< and this structure detail refer @ref M4_SDIOC_TypeDef

        en_sd_card_device_mode_t enDevMode; ///< SD Card Write/Read mode and this parameter can be a value of @ref en_sd_card_device_mode_t

        const stc_sdcard_dma_init_t *pstcDmaInitCfg; ///< Pointer to SD Card initialization parameters
                                                     ///< and this structure detail refer @ref stc_sdcard_init_t

        const stc_sdcard_init_t *pstcCardInitCfg; ///< Pointer to SD Card initialization parameters
                                                  ///< and this structure detail refer @ref stc_sdcard_init_t

        __IO uint32_t Context; ///< SD transfer context

        __IO uint32_t u32ErrorCode; ///< SD Card Error codes

        stc_sd_card_info_t stcSdCardInfo; ///< SD Card information
                                          ///< and this structure detail refer @ref stc_sd_card_info_t

        stc_sdmmc_resp_card_status_t stcCardStatus; ///< SD Card status
                                                    ///< and this structure detail refer @ref stc_sdmmc_resp_card_status_t

        uint32_t OCR; ///< SD Card Operation Condition Register

        uint32_t CID[4]; ///< SD Card IDentification

        uint32_t CSD[4]; ///< SD Card Specific Data

        uint32_t DSR; ///< DSR value

        uint32_t RCA; ///< RCA value

        uint32_t SCR[2]; ///< SD Card Configuration Register

        uint32_t CSR; ///< SD Card Status Register
    } stc_sd_handle_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

#define SD_CARD_ERROR_NONE (SDMMC_ERROR_NONE)                                     /*!< No error                                                      */
#define SD_CARD_ERROR_CMD_CRC_FAIL (SDMMC_ERROR_CMD_CRC_FAIL)                     /*!< Command response received (but CRC check failed)              */
#define SD_CARD_ERROR_DATA_CRC_FAIL (SDMMC_ERROR_DATA_CRC_FAIL)                   /*!< Data block sent/received (CRC check failed)                   */
#define SD_CARD_ERROR_CMD_RSP_TIMEOUT (SDMMC_ERROR_CMD_RSP_TIMEOUT)               /*!< Command response timeout                                      */
#define SD_CARD_ERROR_DATA_TIMEOUT (SDMMC_ERROR_DATA_TIMEOUT)                     /*!< Data timeout                                                  */
#define SD_CARD_ERROR_TX_UNDERRUN (SDMMC_ERROR_TX_UNDERRUN)                       /*!< Transmit FIFO underrun                                        */
#define SD_CARD_ERROR_RX_OVERRUN (SDMMC_ERROR_RX_OVERRUN)                         /*!< Receive FIFO overrun                                          */
#define SD_CARD_ERROR_ADDR_MISALIGNED (SDMMC_ERROR_ADDR_MISALIGNED)               /*!< Misaligned address                                            */
#define SD_CARD_ERROR_BLOCK_LEN_ERR (SDMMC_ERROR_BLOCK_LEN_ERR)                   /*!< Transferred block length is not allowed for the card or the \
                                                                                       number of transferred bytes does not match the block length   */
#define SD_CARD_ERROR_ERASE_SEQ_ERR (SDMMC_ERROR_ERASE_SEQ_ERR)                   /*!< An error in the sequence of erase command occurs              */
#define SD_CARD_ERROR_BAD_ERASE_PARAM (SDMMC_ERROR_BAD_ERASE_PARAM)               /*!< An invalid selection for erase groups                         */
#define SD_CARD_ERROR_WRITE_PROT_VIOLATION (SDMMC_ERROR_WRITE_PROT_VIOLATION)     /*!< Attempt to program a write protect block                      */
#define SD_CARD_ERROR_LOCK_UNLOCK_FAILED (SDMMC_ERROR_LOCK_UNLOCK_FAILED)         /*!< Sequence or password error has been detected in unlock \
                                                                                       command or if there was an attempt to access a locked card    */
#define SD_CARD_ERROR_COM_CRC_FAILED (SDMMC_ERROR_COM_CRC_FAILED)                 /*!< CRC check of the previous command failed                      */
#define SD_CARD_ERROR_ILLEGAL_CMD (SDMMC_ERROR_ILLEGAL_CMD)                       /*!< Command is not legal for the card state                       */
#define SD_CARD_ERROR_CARD_ECC_FAILED (SDMMC_ERROR_CARD_ECC_FAILED)               /*!< Card internal ECC was applied but failed to correct the data  */
#define SD_CARD_ERROR_CC_ERR (SDMMC_ERROR_CC_ERR)                                 /*!< Internal card controller error                                */
#define SD_CARD_ERROR_GENERAL_UNKNOWN_ERR (SDMMC_ERROR_GENERAL_UNKNOWN_ERR)       /*!< General or unknown error                                      */
#define SD_CARD_ERROR_STREAM_READ_UNDERRUN (SDMMC_ERROR_STREAM_READ_UNDERRUN)     /*!< The card could not sustain data reading in stream rmode       */
#define SD_CARD_ERROR_STREAM_WRITE_OVERRUN (SDMMC_ERROR_STREAM_WRITE_OVERRUN)     /*!< The card could not sustain data programming in stream mode    */
#define SD_CARD_ERROR_CID_CSD_OVERWRITE (SDMMC_ERROR_CID_CSD_OVERWRITE)           /*!< CID/CSD overwrite error                                       */
#define SD_CARD_ERROR_WP_ERASE_SKIP (SDMMC_ERROR_WP_ERASE_SKIP)                   /*!< Only partial address space was erased                         */
#define SD_CARD_ERROR_CARD_ECC_DISABLED (SDMMC_ERROR_CARD_ECC_DISABLED)           /*!< Command has been executed without using internal ECC          */
#define SD_CARD_ERROR_ERASE_RESET (SDMMC_ERROR_ERASE_RESET)                       /*!< Erase sequence was cleared before executing because an out \
                                                                                       of erase sequence command was received                        */
#define SD_CARD_ERROR_AKE_SEQ_ERR (SDMMC_ERROR_AKE_SEQ_ERR)                       /*!< Error in sequence of authentication                           */
#define SD_CARD_ERROR_INVALID_VOLTRANGE (SDMMC_ERROR_INVALID_VOLTRANGE)           /*!< Error in case of invalid voltage range                        */
#define SD_CARD_ERROR_ADDR_OUT_OF_RANGE (SDMMC_ERROR_ADDR_OUT_OF_RANGE)           /*!< Error when addressed block is out of range                    */
#define SD_CARD_ERROR_REQUEST_NOT_APPLICABLE (SDMMC_ERROR_REQUEST_NOT_APPLICABLE) /*!< Error when command request is not applicable                  */
#define SD_CARD_ERROR_PARAM (SDMMC_ERROR_INVALID_PARAMETER)                       /*!< the used parameter is not valid                               */
#define SD_CARD_ERROR_UNSUPPORTED_FEATURE (SDMMC_ERROR_UNSUPPORTED_FEATURE)       /*!< Error when feature is not insupported                         */
#define SD_CARD_ERROR_BUSY (SDMMC_ERROR_BUSY)                                     /*!< Error when transfer process is busy                           */
#define SD_CARD_ERROR_DMA (SDMMC_ERROR_DMA)                                       /*!< Error while DMA transfer                                      */
#define SD_CARD_ERROR_TIMEOUT (SDMMC_ERROR_TIMEOUT)                               /*!< Timeout error                                                 */

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/

    en_result_t SDCARD_Init(stc_sd_handle_t *handle,
                            const stc_sdcard_init_t *pstcInitCfg);
    en_result_t SDCARD_SetDeviceMode(stc_sd_handle_t *handle,
                                     en_sd_card_device_mode_t enDevMode);
    en_sd_card_device_mode_t SDCARD_GetDeviceMode(const stc_sd_handle_t *handle);
    en_result_t SDCARD_GetCardCSD(stc_sd_handle_t *handle);
    en_result_t SDCARD_Erase(stc_sd_handle_t *handle,
                             uint32_t u32BlkStartAddr,
                             uint32_t u32BlkEndAddr,
                             uint32_t u32Timeout);
    en_result_t SDCARD_ReadBlocks(stc_sd_handle_t *handle,
                                  uint32_t u32BlockAddr,
                                  uint16_t u16BlockCnt,
                                  uint8_t *pu8Data,
                                  uint32_t u32Timeout);
    en_result_t SDCARD_WriteBlocks(stc_sd_handle_t *handle,
                                   uint32_t u32BlockAddr,
                                   uint16_t u16BlockCnt,
                                   uint8_t *pu8Data,
                                   uint32_t u32Timeout);

    //@} // SdiocGroup

#ifdef __cplusplus
}
#endif

#endif /* __SD_CARD_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
