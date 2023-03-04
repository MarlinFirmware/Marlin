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
/** \file sd_card.c
 **
 ** A detailed description is available at
 ** @link SdiocGroup SDIOC description @endlink
 **
 **   - 2018-11-15  1.0 Hongjh First version for Device Driver Library of SDIOC.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "sdmmc_cmd.h"
#include "sd_card.h"

/**
 *******************************************************************************
 ** \addtogroup SdiocGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef enum en_sd_card_type
{
    SdCardSdsc = 0u,
    SdCardSdhcSdxc = 1u,
    SdCardSecured = 3u,
} en_sd_card_type_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Card Command Class supported. */
#define SDMMC_CARD_CCCC_ERASE (0x00000020u)

/*!< Card locked */
#define SDMMC_CARD_LOCKED (0x02000000u)

/*!< Voltage trial times */
#define SD_CARD_MAX_VOLT_TRIAL (0x0000FFFFu)

/*!< High speed */
#define SD_CHECK_FUNCTION_HIGH_SPEED (0x00FFFF01u)
#define SD_SET_FUNCTION_HIGH_SPEED (0x80FFFF01u)

/*!< Block size is 512 bytes */
#define SD_CARD_BLOCK_SIZE (512u)

/*!< SDIOC buffer align size */
#define SDIOC_BUF_ALIGN_SIZE (4ul)

/*!< Log Block Number for 2G bytes Cards */
#define SD_CARD_CAPACITY (0x400000u)

/*!< Card power up status bit (busy) */
#define SD_CARD_OCR_BUSY (0x80000000ul)

#define SD_CARD_OP_NONE (0x00000000u)                 /*!< None                             */
#define SD_CARD_OP_READ_SINGLE_BLOCK (0x00000001u)    /*!< Read single block operation      */
#define SD_CARD_OP_READ_MULTIPLE_BLOCK (0x00000002u)  /*!< Read multiple blocks operation   */
#define SD_CARD_OP_WRITE_SINGLE_BLOCK (0x00000010u)   /*!< Write single block operation     */
#define SD_CARD_OP_WRITE_MULTIPLE_BLOCK (0x00000020u) /*!< Write multiple blocks operation  */
#define SD_CARD_OP_IT (0x00000008u)                   /*!< Process in Interrupt mode        */
#define SD_CARD_OP_DMA (0x00000080u)                  /*!< Process in DMA mode              */

/*!< the SD card relative card address. */
#define RelCardAddress(handle) ((handle)->stcSdCardInfo.u32RelCardAddr)
#define IsCardProgramming(handle) ((SdmmcCardStatePgm == (handle)->stcCardStatus.CURRENT_STATE) ? true : false)
#define IsCardReadyForData(handle) ((1u == (handle)->stcCardStatus.READY_FOR_DATA) ? true : false)

#define IS_DMA_CFG_VALID(handle)           \
    ((NULL != (handle)->pstcDmaInitCfg) && \
     (NULL != (handle)->pstcDmaInitCfg->DMAx))

/*!< the SD card use DMA unit && channel. */
#define DMA_Unit(handle) ((handle)->pstcDmaInitCfg->DMAx)
#define DMA_CH(handle) ((handle)->pstcDmaInitCfg->enDmaCh)

/*!< Parameter valid check for buffer address. */
#define IS_VALID_TRANSFER_BUF_ALIGN(x) (!((SDIOC_BUF_ALIGN_SIZE - 1ul) & ((uint32_t)(x))))

/*!< Parameter valid check for SDIOC command value. */
#define IS_VALID_TRANSFER_BUF_LEN(x) (!((SD_CARD_BLOCK_SIZE - 1u) & (x)))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static en_result_t SdCardInitSd(stc_sd_handle_t *handle);
static en_result_t SdCardPowerON(stc_sd_handle_t *handle);
static en_result_t SdCardInitHost(const stc_sd_handle_t *handle);
static en_result_t SdCardSetSpeed(stc_sd_handle_t *handle);
static en_result_t SdCardSetBusWidth(stc_sd_handle_t *handle);
static en_result_t SdCardCheckReayForData(stc_sd_handle_t *handle,
                                          uint32_t u32Timeout);
static en_result_t DmaSdiocTxConfig(M4_DMA_TypeDef *DMAx,
                                    en_dma_channel_t enCh,
                                    M4_SDIOC_TypeDef *SDIOCx,
                                    uint8_t *pu8TxBuf,
                                    uint16_t u16len);
static en_result_t DmaSdiocRxConfig(M4_DMA_TypeDef *DMAx,
                                    en_dma_channel_t enCh,
                                    M4_SDIOC_TypeDef *SDIOCx,
                                    uint8_t *pu8RxBuf,
                                    uint16_t u16len);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Initialize SD.
 **
 ** \param [in] handle                  Pointer to SD handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 ** \param [in] pstcInitCfg             The pointer of SD configure structure
 ** \arg This parameter detail refer @ref stc_sdcard_init_t
 **
 ** \retval Ok                          Initialize SD successfully.
 ** \retval Error                       Initialize SD unsuccessfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - handle == NULL
 **                                     - pstcInitCfg == NULL
 **
 ******************************************************************************/
en_result_t SDCARD_Init(stc_sd_handle_t *handle,
                        const stc_sdcard_init_t *pstcInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != handle) && (NULL != pstcInitCfg))
    {
        handle->pstcCardInitCfg = pstcInitCfg;

        enRet = SdCardInitHost(handle);
        if (enRet != Ok)
        {
            printf("SdCardInitHost fail,%u !\n", (uint8_t)enRet);
            return enRet;
        }

        enRet = SdCardPowerON(handle);
        if (enRet != Ok)
        {
            printf("SdCardPowerON fail,%u,0x%lx !\n", (uint8_t)enRet, handle->u32ErrorCode);
            return enRet;
        }

        enRet = SdCardInitSd(handle);
        if (enRet != Ok)
        {
            printf("SdCardInitSd fail,%u !\n", (uint8_t)enRet);
            return enRet;
        }

        enRet = SdCardSetBusWidth(handle);
        if (enRet != Ok)
        {
            printf("SdCardSetBusWidth fail,%u !\n", (uint8_t)enRet);
            return enRet;
        }

        enRet = SdCardSetSpeed(handle);
        if (enRet != Ok)
        {
            printf("SdCardSetSpeed fail,%u !\n", (uint8_t)enRet);
            return enRet;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set SD Card device Read/Write mode.
 **
 ** \param [in] handle                  Pointer to SD handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 ** \param [in] enDevMode               Read/Write mode
 ** \arg SdCardDmaMode                  DMA mode transfer
 ** \arg SdCardPollingMode              Polling mode transfer
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       handle pointer is NULL.
 **
 ******************************************************************************/
en_result_t SDCARD_SetDeviceMode(stc_sd_handle_t *handle,
                                 en_sd_card_device_mode_t enDevMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != handle)
    {
        handle->enDevMode = enDevMode;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get SD Card device Read/Write mode.
 **
 ** \param [in] handle                  Pointer to SD handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 **
 ** \retval SdCardDmaMode               DMA mode transfer
 ** \retval SdCardPollingMode           Polling mode transfer
 **
 ******************************************************************************/
en_sd_card_device_mode_t SDCARD_GetDeviceMode(const stc_sd_handle_t *handle)
{
    DDL_ASSERT(NULL != handle);

    return handle->enDevMode;
}

/**
 *******************************************************************************
 ** \brief Get Card specified Data information.
 **
 ** \param [in] handle                  Pointer to SD Card handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 **
 ** \retval Ok                          Get successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - handle == NULL
 **
 ******************************************************************************/
en_result_t SDCARD_GetCardCSD(stc_sd_handle_t *handle)
{
    uint32_t u32Csize = 0ul;
    uint32_t u32NumSector = 0ul;
    uint32_t u32CsizeMulti = 0ul;
    stc_sdcard_csd_v1_t *pstcCsdSd = NULL;
    stc_sdcard_csd_v2_t *pstcCsdSdHc = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != handle)
    {
        /* High Capacity  CSD Version 2.0*/
        if ((handle->CSD[3] & 0x00FF0000ul) == 0x00400000ul)
        {
            pstcCsdSdHc = (stc_sdcard_csd_v2_t *)&handle->CSD[0];
            u32Csize = ((unsigned int)pstcCsdSdHc->C_SIZE3 << 16ul) + ((unsigned int)pstcCsdSdHc->C_SIZE2 << 8ul) + pstcCsdSdHc->C_SIZE1;

            u32NumSector = (u32Csize + 1ul) << 10ul;

            handle->stcSdCardInfo.u32Class = (((uint32_t)pstcCsdSdHc->CCC2) << 4u) | ((uint32_t)pstcCsdSdHc->CCC1);
            handle->stcSdCardInfo.u32BlockSize = 1ul << (pstcCsdSdHc->READ_BL_LEN);
        }
        else /* Standard Capacity     CSD Version 1.xx */
        {
            pstcCsdSd = (stc_sdcard_csd_v1_t *)&handle->CSD[0];

            u32Csize = ((unsigned int)pstcCsdSd->C_SIZE3 << 10) + ((unsigned int)pstcCsdSd->C_SIZE2 << 2) + pstcCsdSd->C_SIZE1;

            u32CsizeMulti = ((uint32_t)(pstcCsdSd->C_SIZE_MULTI2) << 1) + (uint32_t)pstcCsdSd->C_SIZE_MULTI1;
            u32NumSector = (u32Csize + 1ul) << (u32CsizeMulti + 2ul);

            if (pstcCsdSd->READ_BL_LEN == 0x0Au)
            {
                u32NumSector *= 2ul;
            }
            else if (pstcCsdSd->READ_BL_LEN == 0x0Bu)
            {
                u32NumSector *= 4ul;
            }
            else
            {
                /* Do nothing: only avoid MISRA warning */
            }

            handle->stcSdCardInfo.u32Class = ((uint32_t)pstcCsdSd->CCC2 << 4) | (uint32_t)pstcCsdSd->CCC1;
            handle->stcSdCardInfo.u32BlockSize = 1ul << (pstcCsdSd->READ_BL_LEN);
        }

        handle->stcSdCardInfo.u32BlockNbr = u32NumSector;
        handle->stcSdCardInfo.u32LogBlockNbr = (handle->stcSdCardInfo.u32BlockNbr) * ((handle->stcSdCardInfo.u32BlockSize) / SD_CARD_BLOCK_SIZE);
        handle->stcSdCardInfo.u32LogBlockSize = SD_CARD_BLOCK_SIZE;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Erase SD card blocks.
 **
 ** \param [in] handle                  Pointer to SD Card handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 ** \param [in] u32BlkStartAddr         Block start address
 ** \param [in] u32BlkEndAddr           Block end address
 ** \param [in] u32Timeout              Erase timeout value
 **
 ** \retval Ok                          Erase successfully.
 ** \retval Error                       Erase unsuccessfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - handle == NULL
 **                                     - u32BlkStartAddr out of range
 **                                     - u32BlkEndAddr out of range
 **
 ******************************************************************************/
en_result_t SDCARD_Erase(stc_sd_handle_t *handle,
                         uint32_t u32BlkStartAddr,
                         uint32_t u32BlkEndAddr,
                         uint32_t u32Timeout)
{
    en_result_t enCmdRet = Error;
    __IO uint32_t u32TimeCount = u32Timeout * (SystemCoreClock / 8u / 1000u);

    if ((NULL == handle) ||
        (u32BlkStartAddr > u32BlkEndAddr) ||
        (u32BlkEndAddr > handle->stcSdCardInfo.u32BlockNbr))
    {
        return ErrorInvalidParameter;
    }

    /* Check if the card command class supports erase command */
    if (!(handle->stcSdCardInfo.u32Class & SDMMC_CARD_CCCC_ERASE))
    {
        return ErrorAccessRights;
    }

    if ((SDIOC_GetResponse(handle->SDIOCx, SdiocRegResp01) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)
    {
        return ErrorAccessRights;
    }

    /* Check the Card capacity in term of Logical number of blocks */
    if (handle->stcSdCardInfo.u32CardType != SdCardSdhcSdxc)
    {
        u32BlkStartAddr *= SD_CARD_BLOCK_SIZE;
        u32BlkEndAddr *= SD_CARD_BLOCK_SIZE;
    }

    /* Send CMD35 ERASE_GRP_START with argument as address */
    enCmdRet = SDMMC_Cmd32_EraseWrBlkStart(handle->SDIOCx, u32BlkStartAddr, (uint32_t *)(&handle->stcCardStatus));
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    /* Send CMD36 ERASE_GRP_END with argument as address */
    enCmdRet = SDMMC_Cmd33_EraseWrBlkEnd(handle->SDIOCx, u32BlkEndAddr, (uint32_t *)(&handle->stcCardStatus));
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    enCmdRet = SDMMC_Cmd38_Erase(handle->SDIOCx, (uint32_t *)(&handle->stcCardStatus));
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    enCmdRet = SdCardCheckReayForData(handle, u32TimeCount);

    return enCmdRet;
}

/**
 *******************************************************************************
 ** \brief Read block data from SD card
 **
 ** \param [in] handle                  Pointer to SD Card handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 ** \param [in] u32BlockAddr            Block address
 ** \param [in] u16BlockCnt             Block Count
 ** \param [in] pu8Data                 Pointer to buffer which will store SD Card data.
 ** \param [in] u32Timeout              Transfer timeout
 **
 ** \retval Ok                          Read successfully.
 ** \retval Error                       Read unsuccessfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - handle == NULL
 **                                     - pu8Data == NULL
 **                                     - u16BlockCnt == 0
 **                                     - (u32BlockAddr + u16BlockCnt) out of range
 **
 ******************************************************************************/
en_result_t SDCARD_ReadBlocks(stc_sd_handle_t *handle,
                              uint32_t u32BlockAddr,
                              uint16_t u16BlockCnt,
                              uint8_t *pu8Data,
                              uint32_t u32Timeout)
{
    en_result_t enCmdRet;
    stc_sdioc_data_cfg_t stcDataCfg;
    uint8_t *pu8TempBuf = (uint8_t *)pu8Data;
    uint32_t u32TimeCount = u32Timeout * (SystemCoreClock / 8u / 1000u);
    __IO uint32_t u32Count = u32TimeCount;
    en_sd_card_device_mode_t enDeviceMode = SDCARD_GetDeviceMode(handle);
    en_flag_status_t enStatus = Reset;
    en_flag_status_t enIrqFlag = Reset;

    if (NULL == handle)
    {
        printf("SD ErrorInvalidParameter->(NULL)\n");
        return ErrorInvalidParameter;
    }

    if ((NULL == pu8Data) || (0u == u16BlockCnt))
    {
        handle->u32ErrorCode |= SD_CARD_ERROR_PARAM;
        printf("SD ErrorInvalidParameter->(SD_CARD_ERROR_PARAM)\n");
        return ErrorInvalidParameter;
    }

    handle->u32ErrorCode = SD_CARD_ERROR_NONE;

    if ((u32BlockAddr + u16BlockCnt) > (handle->stcSdCardInfo.u32LogBlockNbr))
    {
        handle->u32ErrorCode |= SD_CARD_ERROR_ADDR_OUT_OF_RANGE;
        printf("SD ErrorInvalidParameter->(SD_CARD_ERROR_ADDR_OUT_OF_RANGE)\n");
        return ErrorInvalidParameter;
    }

    if (handle->stcSdCardInfo.u32CardType != SdCardSdhcSdxc)
    {
        u32BlockAddr *= SD_CARD_BLOCK_SIZE;
    }

    /* Set Block Size for Card */
    enCmdRet = SDMMC_Cmd16_SetBlockLength(handle->SDIOCx, SD_CARD_BLOCK_SIZE, (uint32_t *)(&handle->stcCardStatus));
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    stcDataCfg.u16BlkCnt = u16BlockCnt;
    stcDataCfg.u16BlkSize = SD_CARD_BLOCK_SIZE;
    stcDataCfg.enDataTimeOut = SdiocDtoSdclk_2_27;
    stcDataCfg.enTransferDir = SdiocTransferToHost;
    stcDataCfg.enAutoCmd12Enable = (u16BlockCnt > 1u) ? Enable : Disable;
    stcDataCfg.enTransferMode = (u16BlockCnt > 1u) ? SdiocTransferMultiple : SdiocTransferSingle;
    enCmdRet = SDIOC_ConfigData(handle->SDIOCx, &stcDataCfg);
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    if (enDeviceMode == SdCardDmaMode)
    {
        if (IS_DMA_CFG_VALID(handle))
        {
            DmaSdiocRxConfig(DMA_Unit(handle), DMA_CH(handle), handle->SDIOCx, pu8TempBuf, u16BlockCnt * SD_CARD_BLOCK_SIZE);
        }
    }

    /* Read block(s) in polling mode */
    if (1u == u16BlockCnt)
    {
        handle->Context = SD_CARD_OP_READ_SINGLE_BLOCK;
        /* Read Single Block command */
        enCmdRet = SDMMC_Cmd17_ReadSingleBlock(handle->SDIOCx, u32BlockAddr, (uint32_t *)(&handle->stcCardStatus));
    }
    else
    {
        handle->Context = SD_CARD_OP_READ_MULTIPLE_BLOCK;
        /* Read Multi Block command */
        enCmdRet = SDMMC_Cmd18_ReadMultipleBlock(handle->SDIOCx, u32BlockAddr, (uint32_t *)(&handle->stcCardStatus));
    }

    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    if (SdCardPollingMode == enDeviceMode)
    {
        /* Poll on SDIO flags */
        while (u16BlockCnt)
        {
            enStatus = SDIOC_GetStatus(handle->SDIOCx, SdiocBufferReadEnble);
            enIrqFlag = SDIOC_GetNormalIrqFlag(handle->SDIOCx, SdiocBufferReadReady);
            if ((Set == enStatus) && (Set == enIrqFlag))
            {
                SDIOC_ReadBuffer(handle->SDIOCx, pu8TempBuf, SD_CARD_BLOCK_SIZE);

                u16BlockCnt--;
                u32Count = u32TimeCount;
                pu8TempBuf += SD_CARD_BLOCK_SIZE;
            }

            if (0ul == u32Count--)
            {
                return ErrorTimeout;
            }
        }
    }

    for (u32Count = u32TimeCount; u32Count > 0ul; u32Count--)
    {
        if (Set == SDIOC_GetNormalIrqFlag(handle->SDIOCx, SdiocTransferComplete))
        {
            break;
        }
    }

    if (0ul == u32Count)
    {
        return ErrorTimeout;
    }
    else
    {
        SDIOC_ClearNormalIrqFlag(handle->SDIOCx, SdiocTransferComplete);
    }

    /* check whether Data transfer stops */
    if (Set == SDIOC_GetNormalIrqFlag(handle->SDIOCx, SdiocErrorInt))
    {
        enCmdRet = Error;
    }

    return enCmdRet;
}

/**
 *******************************************************************************
 ** \brief Write block data to SD card
 **
 ** \param [in] handle                  Pointer to SD Card handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 ** \param [in] u32BlockAddr            Block address
 ** \param [in] u16BlockCnt             Block Count
 ** \param [in] pu8Data                 Pointer to buffer which contains data to be send to SD Card.
 ** \param [in] u32Timeout              Transfer timeout
 **
 ** \retval Ok                          Write successfully.
 ** \retval Error                       Write unsuccessfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - handle == NULL
 **                                     - pu8Data == NULL
 **                                     - u16BlockCnt == 0
 **                                     - (u32BlockAddr + u16BlockCnt) out of range
 **
 ******************************************************************************/
en_result_t SDCARD_WriteBlocks(stc_sd_handle_t *handle,
                               uint32_t u32BlockAddr,
                               uint16_t u16BlockCnt,
                               uint8_t *pu8Data,
                               uint32_t u32Timeout)
{
    en_result_t enCmdRet;
    stc_sdioc_data_cfg_t stcDataCfg;
    uint8_t *pu8TempBuf = (uint8_t *)pu8Data;
    uint32_t u32TimeCount = u32Timeout * (SystemCoreClock / 50ul / 1000ul);
    __IO uint32_t u32Count = u32TimeCount;
    en_sd_card_device_mode_t enDeviceMode = SDCARD_GetDeviceMode(handle);
    en_flag_status_t enStatus = Reset;
    en_flag_status_t enIrqFlag = Reset;

    if (NULL == handle)
    {
        return ErrorInvalidParameter;
    }

    if ((NULL == pu8Data) || (0u == u16BlockCnt))
    {
        handle->u32ErrorCode |= SD_CARD_ERROR_PARAM;
        return ErrorInvalidParameter;
    }

    handle->u32ErrorCode = SD_CARD_ERROR_NONE;

    if ((u32BlockAddr + u16BlockCnt) > (handle->stcSdCardInfo.u32LogBlockNbr))
    {
        handle->u32ErrorCode |= SD_CARD_ERROR_ADDR_OUT_OF_RANGE;
        return ErrorInvalidParameter;
    }

    /* Set Block Size for Card */
    enCmdRet = SDMMC_Cmd16_SetBlockLength(handle->SDIOCx, SD_CARD_BLOCK_SIZE, (uint32_t *)(&handle->stcCardStatus));
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    stcDataCfg.u16BlkCnt = u16BlockCnt;
    stcDataCfg.u16BlkSize = SD_CARD_BLOCK_SIZE;
    stcDataCfg.enDataTimeOut = SdiocDtoSdclk_2_27;
    stcDataCfg.enTransferDir = SdiocTransferToCard;
    stcDataCfg.enAutoCmd12Enable = (u16BlockCnt > 1u) ? Enable : Disable;
    stcDataCfg.enTransferMode = (u16BlockCnt > 1u) ? SdiocTransferMultiple : SdiocTransferSingle;
    enCmdRet = SDIOC_ConfigData(handle->SDIOCx, &stcDataCfg);
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    if (handle->stcSdCardInfo.u32CardType != SdCardSdhcSdxc)
    {
        u32BlockAddr *= SD_CARD_BLOCK_SIZE;
    }

    if (enDeviceMode == SdCardDmaMode)
    {
        if (IS_DMA_CFG_VALID(handle))
        {
            DmaSdiocTxConfig(DMA_Unit(handle), DMA_CH(handle), handle->SDIOCx, pu8TempBuf, u16BlockCnt * SD_CARD_BLOCK_SIZE);
        }
    }

    /* Write block(s) in polling mode */
    if (1u == u16BlockCnt)
    {
        /* Read Single Block command */
        enCmdRet = SDMMC_Cmd24_WriteSingleBlock(handle->SDIOCx, u32BlockAddr, (uint32_t *)(&handle->stcCardStatus));
    }
    else
    {
        /* Read Multi Block command */
        enCmdRet = SDMMC_Cmd25_WriteMultipleBlock(handle->SDIOCx, u32BlockAddr, (uint32_t *)(&handle->stcCardStatus));
    }

    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    if (SdCardPollingMode == enDeviceMode)
    {
        /* Poll on SDIO flags */
        while (u16BlockCnt)
        {
            enStatus = SDIOC_GetStatus(handle->SDIOCx, SdiocBufferWriteEnble);
            enIrqFlag = SDIOC_GetNormalIrqFlag(handle->SDIOCx, SdiocBufferWriteReady);
            if ((Set == enStatus) && (Set == enIrqFlag))
            {
                /* Write data to SDIO Tx Buffer */
                SDIOC_WriteBuffer(handle->SDIOCx, pu8TempBuf, SD_CARD_BLOCK_SIZE);

                u16BlockCnt--;
                u32Count = u32TimeCount;
                pu8TempBuf += SD_CARD_BLOCK_SIZE;
            }

            if (u32Count-- == 0ul)
            {
                return ErrorTimeout;
            }
        }
    }

    for (u32Count = u32TimeCount; u32Count > 0ul; u32Count--)
    {
        enStatus = SDIOC_GetStatus(handle->SDIOCx, SdiocData0PinLvl);
        enIrqFlag = SDIOC_GetNormalIrqFlag(handle->SDIOCx, SdiocTransferComplete);
        if ((Set == enStatus) && (Set == enIrqFlag))
        {
            break;
        }
    }

    if (0u == u32Count)
    {
        return ErrorTimeout;
    }
    else
    {
        SDIOC_ClearNormalIrqFlag(handle->SDIOCx, SdiocTransferComplete);
    }

    enCmdRet = SdCardCheckReayForData(handle, u32TimeCount);
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    /* check whether Data transfer stops */
    if (Set == SDIOC_GetNormalIrqFlag(handle->SDIOCx, SdiocErrorInt))
    {
        return Error;
    }

    SDIOC_ClearNormalIrqFlag(handle->SDIOCx, SdiocBufferWriteReady);

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set SD bus width.
 **
 ** \param [in] handle                  Pointer to SD handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 **
 ** \retval Ok                          Set SD bus width successfully.
 ** \retval Error                       Set SD bus width unsuccessfully.
 ** \retval ErrorInvalidParameter       If one of following conditions matches:
 **                                     - NULL == pstcCfg
 **                                     - enBusWidth is invalid.
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
static en_result_t SdCardSetBusWidth(stc_sd_handle_t *handle)
{
    uint32_t u32CmdArg = 0ul;
    en_result_t enCmdRet = Ok;

    if ((NULL == handle) || (NULL == handle->pstcCardInitCfg))
    {
        return ErrorInvalidParameter;
    }

    switch (handle->pstcCardInitCfg->enBusWidth)
    {
    case SdiocBusWidth1Bit:
        u32CmdArg = 0u;
        break;
    case SdiocBusWidth4Bit:
        u32CmdArg = 2u;
        break;
    default:
        enCmdRet = ErrorInvalidParameter;
        break;
    }

    if (Ok != enCmdRet)
    {
        return enCmdRet;
    }

    enCmdRet = SDMMC_Cmd55_AppCmd(handle->SDIOCx, (RelCardAddress(handle) << 16u), (uint32_t *)(&handle->stcCardStatus));
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    enCmdRet = SDMMC_Acmd6_SetBusWidth(handle->SDIOCx, u32CmdArg, (uint32_t *)(&handle->stcCardStatus));
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    enCmdRet = SDIOC_SetBusWidth(handle->SDIOCx, handle->pstcCardInitCfg->enBusWidth);
    if (enCmdRet != Ok)
    {
        return enCmdRet;
    }

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set SD high speed mode.
 **
 ** \param [in] handle                  Pointer to SD handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 **
 ** \retval Ok                          Set SD bus width successfully.
 ** \retval Ok                          Set SD bus width unsuccessfully.
 ** \retval ErrorInvalidParameter       If one of following conditions matches:
 **                                     - NULL == pstcCfg
 **                                     - u32Freq is invalid.
 **
 ******************************************************************************/
static en_result_t SdCardSetSpeed(stc_sd_handle_t *handle)
{
    uint8_t au8TempBuf[64] = {0};
    uint32_t u32TimeCount = 2000 * (SystemCoreClock / 8u / 1000u);
    __IO uint32_t u32Count = u32TimeCount;
    en_flag_status_t enStatus = Reset;
    en_flag_status_t enIrqFlag = Reset;

    en_result_t enCmdRet = ErrorInvalidParameter;
    stc_sdioc_data_cfg_t stcDataCfg;
    uint32_t u32Arg = SD_SET_FUNCTION_HIGH_SPEED;

    if ((NULL != handle) && (NULL != handle->pstcCardInitCfg))
    {
        enCmdRet = Ok;
        if (SdiocHighSpeedMode == handle->pstcCardInitCfg->enSpeedMode)
        {
            /* Set Block Size for Card */
            enCmdRet = SDMMC_Cmd16_SetBlockLength(handle->SDIOCx, 64u, (uint32_t *)(&handle->stcCardStatus));
            if (enCmdRet == Ok)
            {
                stcDataCfg.u16BlkCnt = 1u;
                stcDataCfg.u16BlkSize = 64u;
                stcDataCfg.enDataTimeOut = SdiocDtoSdclk_2_27;
                stcDataCfg.enTransferDir = SdiocTransferToHost;
                stcDataCfg.enAutoCmd12Enable = Disable;
                stcDataCfg.enTransferMode = SdiocTransferSingle;
                enCmdRet = SDIOC_ConfigData(handle->SDIOCx, &stcDataCfg);
                if (enCmdRet == Ok)
                {
                    enCmdRet = SDMMC_Cmd6_SwitchFunc(handle->SDIOCx, u32Arg, (uint32_t *)(&handle->stcCardStatus));
                    if (enCmdRet == Ok)
                    {
                        while (u32Count--)
                        {
                            enStatus = SDIOC_GetStatus(handle->SDIOCx, SdiocBufferReadEnble);
                            enIrqFlag = SDIOC_GetNormalIrqFlag(handle->SDIOCx, SdiocBufferReadReady);
                            if ((Set == enStatus) && (Set == enIrqFlag))
                            {
                                SDIOC_ReadBuffer(handle->SDIOCx, au8TempBuf, sizeof(au8TempBuf));
                                break;
                            }
                        }

                        if (0ul == u32Count)
                        {
                            return ErrorTimeout;
                        }

                        u32Count = u32TimeCount;
                        while (u32Count--)
                        {
                            if (Set == SDIOC_GetNormalIrqFlag(handle->SDIOCx, SdiocTransferComplete))
                            {
                                SDIOC_ClearNormalIrqFlag(handle->SDIOCx, SdiocTransferComplete);
                                break;
                            }
                        }

                        if (0ul == u32Count)
                        {
                            return ErrorTimeout;
                        }

                        /* Check whether switch function with error or transfer error occur */
                        if ((!(au8TempBuf[16] & 0x01)) ||
                            (Set == SDIOC_GetNormalIrqFlag(handle->SDIOCx, SdiocErrorInt)))
                        {
                            enCmdRet = Error;
                        }
                    }
                }
            }
        }

        if (enCmdRet == Ok)
        {
            SDIOC_SetSpeedMode(handle->SDIOCx, handle->pstcCardInitCfg->enSpeedMode);

            enCmdRet = SDIOC_SetClk(handle->SDIOCx, handle->pstcCardInitCfg->enClkFreq);
        }
    }

    return enCmdRet;
}

/**
 *******************************************************************************
 ** \brief Initialize SDIO Host
 **
 ** \param [in] handle                  Pointer to SD handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 **
 ** \retval Ok                          SDIO host initialized successfully
 ** \retval ErrorInvalidParameter       If one of following conditions matches:
 **                                     - NULL == pstcInitCfg
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
static en_result_t SdCardInitHost(const stc_sd_handle_t *handle)
{
    en_result_t enRet = Ok;

    if ((NULL == handle) || (NULL == handle->pstcCardInitCfg))
    {
        return ErrorInvalidParameter;
    }

    /* Enable SD clock supply */
    if (M4_SDIOC1 == handle->SDIOCx)
    {
        PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_SDIOC1, Enable);
    }
    else if (M4_SDIOC2 == handle->SDIOCx)
    {
        PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_SDIOC2, Enable);
    }
    else
    {
        return ErrorInvalidParameter;
    }

    enRet = SDIOC_Init(handle->SDIOCx, handle->pstcCardInitCfg->pstcInitCfg);

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize SD bus
 **
 ** \param [in] handle                  Pointer to SD handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 **
 ** \retval Ok                          SDIO bus cmd operate successfully.
 ** \retval Error                       SDIO bus cmd operate unsuccessfully.
 ** \retval ErrorInvalidParameter       If one of following conditions matches:
 **                                     - NULL == pstcCfg
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
static en_result_t SdCardInitSd(stc_sd_handle_t *handle)
{
    en_result_t enCmdRet = Ok;

    if (NULL == handle)
    {
        return ErrorInvalidParameter;
    }

    if (handle->stcSdCardInfo.u32CardType != SdCardSecured)
    {
        /* Send CMD2 ALL_SEND_CID */
        enCmdRet = SDMMC_Cmd2_AllSendCID(handle->SDIOCx, handle->CID);
        if (enCmdRet != Ok)
        {
            return enCmdRet;
        }

        /* Send CMD3 SET_REL_ADDR with argument 0 */
        /* SD Card publishes its RCA. */
        enCmdRet = SDMMC_Cmd3_SendRelativeAddr(handle->SDIOCx, &(handle->RCA));
        if (enCmdRet != Ok)
        {
            return enCmdRet;
        }

        /* Get the SD card RCA */
        RelCardAddress(handle) = handle->RCA;

        /* Send CMD9 SEND_CSD with argument as card's RCA */
        enCmdRet = SDMMC_Cmd9_SendCSD(handle->SDIOCx, RelCardAddress(handle), handle->CSD);
        if (enCmdRet != Ok)
        {
            return enCmdRet;
        }
    }

    /* Get CSD parameters */
    enCmdRet = SDCARD_GetCardCSD(handle);
    if (enCmdRet == Ok)
    {
        /* Select the Card */
        enCmdRet = SDMMC_Cmd7_SelectDeselectCard(handle->SDIOCx, RelCardAddress(handle), (uint32_t *)(&handle->stcCardStatus));
    }

    return enCmdRet;
}

/**
 *******************************************************************************
 ** \brief Activate SD in power-on stage
 **
 ** \param [in] handle                  Pointer to SD handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 **
 ** \retval Ok                          Activate successfully.
 ** \retval Error                       Activate unsuccessfully.
 ** \retval ErrorInvalidParameter       If one of following conditions matches:
 **                                     - NULL == handle
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
static en_result_t SdCardPowerON(stc_sd_handle_t *handle)
{
    uint32_t u32IfCond = 0u;
    __IO uint32_t u32Count = 0ul;
    en_result_t enCmdRet = Error;

    if (NULL == handle)
    {
        return ErrorInvalidParameter;
    }

    handle->u32ErrorCode = SD_CARD_ERROR_NONE;

    /* CMD0: GO_IDLE_STATE */
    enCmdRet = SDMMC_Cmd0_GoIdleState(handle->SDIOCx);
    if (enCmdRet != Ok)
    {
        handle->u32ErrorCode |= SD_CARD_ERROR_GENERAL_UNKNOWN_ERR;
        return enCmdRet;
    }

    /* Reset OCR valude */
    handle->OCR = 0ul;

    /* CMD8: SEND_IF_COND: Command available only on V2.0 cards */
    enCmdRet = SDMMC_Cmd8_SendIfCond(handle->SDIOCx, &u32IfCond);
    if (enCmdRet != Ok)
    {
        handle->stcSdCardInfo.u32CardVersion = SdCardVer1x;

        /* Send ACMD41 SD_APP_OP_COND with Argument 0x00100000 */
        while (SD_CARD_OCR_BUSY != (handle->OCR & SD_CARD_OCR_BUSY))
        {
            if (SD_CARD_MAX_VOLT_TRIAL == u32Count++)
            {
                handle->u32ErrorCode |= SD_CARD_ERROR_INVALID_VOLTRANGE;
                return enCmdRet;
            }

            /* SEND CMD55 APP_CMD with RCA as 0 */
            enCmdRet = SDMMC_Cmd55_AppCmd(handle->SDIOCx, 0u, (uint32_t *)(&handle->stcCardStatus));
            if (enCmdRet != Ok)
            {
                continue;
            }

            /* Send CMD41 */
            enCmdRet = SDMMC_Acmd41_SdSendOpCond(handle->SDIOCx, SdmmcStanderdCapacity, &handle->OCR);
            if (enCmdRet == Error)
            {
                handle->u32ErrorCode |= SD_CARD_ERROR_UNSUPPORTED_FEATURE;
                return enCmdRet;
            }
        }

        /* Card type is SDSC */
        handle->stcSdCardInfo.u32CardType = SdCardSdsc;
    }
    else
    {
        handle->stcSdCardInfo.u32CardVersion = SdCardVer2x;

        /* Send ACMD41 SD_APP_OP_COND with Argument 0x40100000 */
        while (SD_CARD_OCR_BUSY != (handle->OCR & SD_CARD_OCR_BUSY))
        {
            if (SD_CARD_MAX_VOLT_TRIAL == u32Count++)
            {
                handle->u32ErrorCode |= SD_CARD_ERROR_INVALID_VOLTRANGE;
                return Error;
            }

            /* SEND CMD55 APP_CMD with RCA as 0 */
            enCmdRet = SDMMC_Cmd55_AppCmd(handle->SDIOCx, 0u, (uint32_t *)(&handle->stcCardStatus));
            if (enCmdRet != Ok)
            {
                continue;
            }

            /* Send CMD41 */
            enCmdRet = SDMMC_Acmd41_SdSendOpCond(handle->SDIOCx, SdmmcHighCapacity, &handle->OCR);
            if (enCmdRet == Error)
            {
                handle->u32ErrorCode |= SD_CARD_ERROR_UNSUPPORTED_FEATURE;
                return enCmdRet;
            }
        }

        if ((handle->OCR & SdmmcHighCapacity) == SdmmcHighCapacity)
        {
            handle->stcSdCardInfo.u32CardType = SdCardSdhcSdxc;
        }
        else
        {
            handle->stcSdCardInfo.u32CardType = SdCardSdsc;
        }
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Checks if the SD card is ready for data.
 **
 ** \param [in] handle                  Pointer to SD Card handle
 ** \arg This parameter detail refer @ref stc_sd_handle_t
 ** \param [in] u32Timeout              Retry to get card status time
 **
 ** \retval Ok                          Card is ready for data
 ** \retval Error                       Card is not ready for data
 ** \retval ErrorInvalidParameter       NULL == handle
 **
 ******************************************************************************/
static en_result_t SdCardCheckReayForData(stc_sd_handle_t *handle, uint32_t u32Timeout)
{
    en_result_t enRet = ErrorInvalidParameter;

    if (NULL != handle)
    {
        do
        {
            enRet = SDMMC_Cmd13_SendStatus(handle->SDIOCx, RelCardAddress(handle), (uint32_t *)(&handle->stcCardStatus));
            if (Ok != enRet)
            {
                break;
            }
        } while ((--u32Timeout) && (false == IsCardReadyForData(handle)));

        if (0u == u32Timeout)
        {
            enRet = ErrorTimeout;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Configures the DMA Channel for SDIO Tx request.
 **
 ** \param [in] DMAx                    The pointer to DMAC register base
 ** \arg M4_DMA1                        DMAC unit 1 instance register base
 ** \arg M4_DMA2                        DMAC unit 2 instance register base
 ** \param [in] enCh                    The specified DMAC channel.
 ** \arg DmaCh0                         DMAC channel 0
 ** \arg DmaCh1                         DMAC channel 1
 ** \arg DmaCh2                         DMAC channel 2
 ** \arg DmaCh3                         DMAC channel 3
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pu8TxBuf                Pointer to the Tx buffer
 ** \param [in] u16len                  Tx buffer size
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       Set unsuccessfully.
 **
 ******************************************************************************/
static en_result_t DmaSdiocTxConfig(M4_DMA_TypeDef *DMAx,
                                    en_dma_channel_t enCh,
                                    M4_SDIOC_TypeDef *SDIOCx,
                                    uint8_t *pu8TxBuf,
                                    uint16_t u16len)
{
    stc_dma_config_t stcDmaInit;
    uint32_t u32Fcg0Periph = (M4_DMA1 == DMAx) ? PWC_FCG0_PERIPH_DMA1 : PWC_FCG0_PERIPH_DMA2;
    en_event_src_t enEvtSrc = (M4_SDIOC1 == SDIOCx) ? EVT_SDIOC1_DMAW : EVT_SDIOC2_DMAW;
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != pu8TxBuf) &&
        (IS_VALID_TRANSFER_BUF_LEN(u16len)) &&
        (IS_VALID_TRANSFER_BUF_ALIGN(pu8TxBuf)))
    {
        /* Enable peripheral clock */
        PWC_Fcg0PeriphClockCmd(u32Fcg0Periph, Enable);

        /* Enable DMA. */
        DMA_Cmd(DMAx, Enable);

        /* Initialize DMA. */
        MEM_ZERO_STRUCT(stcDmaInit);
        stcDmaInit.u16BlockSize = SD_CARD_BLOCK_SIZE / 4u;       /* Set data block size. */
        stcDmaInit.u16TransferCnt = u16len / SD_CARD_BLOCK_SIZE; /* Set transfer count. */
        stcDmaInit.u32SrcAddr = (uint32_t)(&pu8TxBuf[0]);        /* Set source address. */
        stcDmaInit.u32DesAddr = (uint32_t)(&SDIOCx->BUF0);       /* Set destination address. */
        stcDmaInit.stcDmaChCfg.enSrcInc = AddressIncrease;       /* Set source address mode. */
        stcDmaInit.stcDmaChCfg.enDesInc = AddressFix;            /* Set destination address mode. */
        stcDmaInit.stcDmaChCfg.enTrnWidth = Dma32Bit;            /* Set data width 8bit. */
        DMA_InitChannel(DMAx, enCh, &stcDmaInit);

        /* Enable the specified DMA channel. */
        DMA_ChannelCmd(DMAx, enCh, Enable);

        /* Clear DMA flag. */
        DMA_ClearIrqFlag(DMAx, enCh, TrnCpltIrq);
        DMA_ClearIrqFlag(DMAx, enCh, BlkTrnCpltIrq);

        /* Enable peripheral circuit trigger function. */
        PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_PTDIS, Enable);

        /* Set DMA trigger source. */
        DMA_SetTriggerSrc(DMAx, enCh, enEvtSrc);

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Configures the DMA Channel for SDIO Rx request.
 **
 ** \param [in] DMAx                    The pointer to DMAC register base
 ** \arg M4_DMA1                        DMAC unit 1 instance register base
 ** \arg M4_DMA2                        DMAC unit 2 instance register base
 ** \param [in] enCh                    The specified DMAC channel.
 ** \arg DmaCh0                         DMAC channel 0
 ** \arg DmaCh1                         DMAC channel 1
 ** \arg DmaCh2                         DMAC channel 2
 ** \arg DmaCh3                         DMAC channel 3
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pu8RxBuf                Pointer to the Rx buffer
 ** \param [in] u16len                  Rx buffer size
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       Set unsuccessfully.
 **
 ******************************************************************************/
static en_result_t DmaSdiocRxConfig(M4_DMA_TypeDef *DMAx,
                                    en_dma_channel_t enCh,
                                    M4_SDIOC_TypeDef *SDIOCx,
                                    uint8_t *pu8RxBuf,
                                    uint16_t u16len)
{
    stc_dma_config_t stcDmaInit;
    uint32_t u32Fcg0Periph = (M4_DMA1 == DMAx) ? PWC_FCG0_PERIPH_DMA1 : PWC_FCG0_PERIPH_DMA2;
    en_event_src_t enEvtSrc = (M4_SDIOC1 == SDIOCx) ? EVT_SDIOC1_DMAR : EVT_SDIOC2_DMAR;
    en_result_t enRet = ErrorInvalidParameter;

    if ((NULL != pu8RxBuf) &&
        (IS_VALID_TRANSFER_BUF_LEN(u16len)) &&
        (IS_VALID_TRANSFER_BUF_ALIGN(pu8RxBuf)))
    {
        /* Enable peripheral clock */
        PWC_Fcg0PeriphClockCmd(u32Fcg0Periph, Enable);

        /* Enable DMA. */
        DMA_Cmd(DMAx, Enable);

        /* Initialize DMA. */
        MEM_ZERO_STRUCT(stcDmaInit);
        stcDmaInit.u16BlockSize = SD_CARD_BLOCK_SIZE / 4u;       /* Set data block size. */
        stcDmaInit.u16TransferCnt = u16len / SD_CARD_BLOCK_SIZE; /* Set transfer count. */
        stcDmaInit.u32SrcAddr = (uint32_t)(&SDIOCx->BUF0);       /* Set source address. */
        stcDmaInit.u32DesAddr = (uint32_t)(&pu8RxBuf[0]);        /* Set destination address. */
        stcDmaInit.stcDmaChCfg.enSrcInc = AddressFix;            /* Set source address mode. */
        stcDmaInit.stcDmaChCfg.enDesInc = AddressIncrease;       /* Set destination address mode. */
        stcDmaInit.stcDmaChCfg.enTrnWidth = Dma32Bit;            /* Set data width 8bit. */
        DMA_InitChannel(DMAx, enCh, &stcDmaInit);

        /* Enable the specified DMA channel. */
        DMA_ChannelCmd(DMAx, enCh, Enable);

        /* Clear DMA flag. */
        DMA_ClearIrqFlag(DMAx, enCh, TrnCpltIrq);
        DMA_ClearIrqFlag(DMAx, enCh, BlkTrnCpltIrq);

        /* Enable peripheral circuit trigger function. */
        PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_PTDIS, Enable);

        /* Set DMA trigger source. */
        DMA_SetTriggerSrc(DMAx, enCh, enEvtSrc);

        enRet = Ok;
    }

    return enRet;
}

//@} // SdiocGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
