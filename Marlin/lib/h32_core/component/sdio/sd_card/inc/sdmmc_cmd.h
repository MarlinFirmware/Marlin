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
/** \file sdmmc_cmd.h
 **
 ** A detailed description is available at
 ** @link SdiocGroup SDIOC description @endlink
 **
 **   - 2018-11-15  1.0 Hongjh First version for Device Driver Library of SDIOC.
 **
 ******************************************************************************/
#ifndef __SDMMC_CMD_H__
#define __SDMMC_CMD_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "hc32f46x_utility.h"
#include "hc32f46x_dmac.h"

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
     ** \brief SD/eMMC/SDIO card status
     **
     ******************************************************************************/
    typedef enum en_sdmmc_card_state
    {
        SdmmcCardStateIdle = 0u,     ///< Card state: Idle State
        SdmmcCardStateReady = 1u,    ///< Card state: Ready State
        SdmmcCardStateIdent = 2u,    ///< Card state: Identification State
        SdmmcCardStateStby = 3u,     ///< Card state: Stand-by State
        SdmmcCardStateTran = 4u,     ///< Card state: Transfer State
        SdmmcCardStateSendData = 5u, ///< Card state: Sending-data State
        SdmmcCardStateRcvData = 6u,  ///< Card state: Receive-data State
        SdmmcCardStatePgm = 7u,      ///< Card state: Programming State
        SdmmcCardStateDis = 8u,      ///< Card state: Disconnect State
    } en_sdmmc_card_state_t;

    /**
     *******************************************************************************
     ** \brief SD/eMMC/SDIO card capacity.
     **
     ******************************************************************************/
    typedef enum sdmmc_capacity
    {
        SdmmcStanderdCapacity = 0x00000000u, ///< Card capacity: Standard Capacity
        SdmmcHighCapacity = 0x40000000u,     ///< Card capacity: High Capacity
    } sdmmc_capacity_t;

    /**
     *******************************************************************************
     ** \brief SD/eMMC/SDIO card response data structure.
     **
     ******************************************************************************/
    typedef struct stc_sdmmc_resp_card_status
    {
        uint32_t RESERVE1 : 3;       ///< RESERVE
        uint32_t AKE_SEQ_ERROR : 1;  ///< Error in the sequence of the authentication process
        uint32_t RESERVE2 : 1;       ///< RESERVE
        uint32_t APP_CMD : 1;        ///< The card will expect ACMD, or an indication that the command has been interpreted as ACMD
        uint32_t RESERVE3 : 2;       ///< RESERVE
        uint32_t READY_FOR_DATA : 1; ///< Corresponds to buffer empty signaling on the bus
        uint32_t CURRENT_STATE : 4;  ///< The state of the card when receiving the command.

        uint32_t ERASE_RESET : 1;      ///< An erase sequence was cleared before executing because an out of erase sequence command was received
        uint32_t CARD_ECC_DISABLE : 1; ///< The command has been executed without using the internal ECC.
        uint32_t WP_ERASE_SKIP : 1;    ///< Set when only partial address space was erased due to existing write protected blocks or the temporary or permanent write protected card was erased.
        uint32_t CSD_OVERWRITE : 1;    ///< - The read only section of the CSD does not match the card content.
                                       ///< - An attempt to reverse the copy (set as original) or permanent WP (unprotected) bits was made.
        uint32_t RESERVE4 : 2;         ///< RESERVE
        uint32_t ERRORR : 1;           ///< A general or an unknown error occurred during the operation.
        uint32_t CC_ERROR : 1;         ///< Internal card controller error
        uint32_t CARD_ECC_FAIL : 1;    ///< Card internal ECC was applied but failed to correct the data.
        uint32_t ILLEGAL_CMD : 1;      ///< Command not legal for the card state
        uint32_t COM_CRC_ERROR : 1;    ///< The CRC check of the previous command failed.
        uint32_t LOCK_UNLOCK_FAIL : 1; ///< Set when a sequence or password error has been detected in lock/unlock card command.
        uint32_t CARD_IS_LOCKED : 1;   ///< When set, signals that the card is locked by the host
        uint32_t WP_VIOLATION : 1;     ///< Set when the host attempts to write to a protected block or to the temporary or permanent write protected card.
        uint32_t ERASE_PARAM : 1;      ///< An invalid selection of write-blocks for erase occurred.
        uint32_t ERASE_SEQ_ERR : 1;    ///< An error in the sequence of erase commands occurred.
        uint32_t BLOCK_LEN_ERR : 1;    ///< The transferred block length is not allowed for this card, or the number of transferred bytes does not match the block length.
        uint32_t ADDRESS_ERROR : 1;    ///< A misaligned address which did not match the block length was used in the command.
        uint32_t OUT_OF_RANGE : 1;     ///< The command��s argument was out of the allowed range for this card.
    } stc_sdmmc_resp_card_status_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief SDMMC Commands Index
 **
 ******************************************************************************/
#define CMD0_GO_IDLE_STATE (0u)          /*!< Resets the SD memory card.                                                               */
#define CMD1_SEND_OP_COND (1u)           /*!< Sends host capacity support information and activates the card's initialization process. */
#define CMD2_ALL_SEND_CID (2u)           /*!< Asks any card connected to the host to send the CID numbers on the CMD line.             */
#define CMD3_SEND_RELATIVE_ADDR (3u)     /*!< Asks the card to publish a new relative address (RCA).                                   */
#define CMD_SET_DSR (4u)                 /*!< Programs the DSR of all cards.                                                           */
#define CMD_SDMMC_SEN_OP_COND (5u)       /*!< Sends host capacity support information (HCS) and asks the accessed card to send its \
                                              operating condition register (OCR) content in the response on the CMD line.              */
#define CMD6_SWITCH_FUNC (6u)            /*!< Checks switchable function (mode 0) and switch card function (mode 1).                   */
#define CMD7_SEL_DESEL_CARD (7u)         /*!< Selects the card by its own relative address and gets deselected by any other address    */
#define CMD8_SEND_IF_COND (8u)           /*!< Sends SD Memory Card interface condition, which includes host supply voltage information \
                                              and asks the card whether card supports voltage.                                         */
#define CMD9_SEND_CSD (9u)               /*!< Addressed card sends its card specific data (CSD) on the CMD line.                       */
#define CMD10_SEND_CID (10u)             /*!< Addressed card sends its card identification (CID) on the CMD line.                      */
#define CMD11_READ_DAT_UNTIL_STOP (11u)  /*!< SD card doesn't support it.                                                              */
#define CMD12_STOP_TRANSMISSION (12u)    /*!< Forces the card to stop transmission.                                                    */
#define CMD13_SEND_STATUS (13u)          /*!< Addressed card sends its status register.                                                */
#define CMD14_HS_BUSTEST_READ (14u)      /*!< Reserved                                                                                 */
#define CMD15_GO_INACTIVE_STATE (15u)    /*!< Sends an addressed card into the inactive state.                                         */
#define CMD16_SET_BLOCKLEN (16u)         /*!< Sets the block length (in bytes for SDSC) for all following block commands     \
                                              (read, write, lock). Default block length is fixed to 512 Bytes. Not effective \
                                              for SDHS and SDXC.                                                                       */
#define CMD17_READ_SINGLE_BLOCK (17u)    /*!< Reads single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of \
                                              fixed 512 bytes in case of SDHC and SDXC.                                                */
#define CMD18_READ_MULTIPLE_BLOCK (18u)  /*!< Continuously transfers data blocks from card to host until interrupted by \
                                              STOP_TRANSMISSION command.                                                               */
#define CMD19_HS_BUSTEST_WRITE (19u)     /*!< 64 bytes tuning pattern is sent for SDR50 and SDR104.                                    */
#define CMD20_WRITE_DAT_UNTIL_STOP (20u) /*!< Speed class control command.                                                             */
#define CMD23_SET_BLOCK_COUNT (23u)      /*!< Specify block count for CMD18 and CMD25.                                                 */
#define CMD24_WRITE_SINGLE_BLOCK (24u)   /*!< Writes single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of \
                                              fixed 512 bytes in case of SDHC and SDXC.                                                */
#define CMD25_WRITE_MULTIPLE_BLOCK (25u) /*!< Continuously writes blocks of data until a STOP_TRANSMISSION follows.                    */
#define CMD26_PROG_CID (26u)             /*!< Reserved for manufacturers.                                                              */
#define CMD27_PROG_CSD (27u)             /*!< Programming of the programmable bits of the CSD.                                         */
#define CMD28_SET_WRITE_PROT (28u)       /*!< Sets the write protection bit of the addressed group.                                    */
#define CMD29_CLR_WRITE_PROT (29u)       /*!< Clears the write protection bit of the addressed group.                                  */
#define CMD30_SEND_WRITE_PROT (30u)      /*!< Asks the card to send the status of the write protection bits.                           */
#define CMD32_ERASE_WR_BLK_START (32u)   /*!< Sets the address of the first write block to be erased. (For SD card only).              */
#define CMD33_ERASE_WR_BLK_END (33u)     /*!< Sets the address of the last write block of the continuous range to be erased.           */
#define CMD35_ERASE_GRP_START (35u)      /*!< Sets the address of the first write block to be erased. Reserved for each command \
                                              system set by switch function command (CMD6).                                            */
#define CMD36_ERASE_GRP_END (36u)        /*!< Sets the address of the last write block of the continuous range to be erased. \
                                              Reserved for each command system set by switch function command (CMD6).                  */
#define CMD38_ERASE (38u)                /*!< Reserved for SD security applications.                                                   */
#define CMD39_FAST_IO (39u)              /*!< SD card doesn't support it (Reserved).                                                   */
#define CMD40_GO_IRQ_STATE (40u)         /*!< SD card doesn't support it (Reserved).                                                   */
#define CMD42_LOCK_UNLOCK (42u)          /*!< Sets/resets the password or lock/unlock the card. The size of the data block is set by \
                                              the SET_BLOCK_LEN command.                                                               */
#define CMD55_APP_CMD (55u)              /*!< Indicates to the card that the next command is an application specific command rather \
                                              than a standard command.                                                                 */
#define CMD56_GEN_CMD (56u)              /*!< Used either to transfer a data block to the card or to get a data block from the card \
                                              for general purpose/application specific commands.                                       */
#define CMD64_NO_CMD (64u)               /*!< No command                                                                               */

/**
 *******************************************************************************
 ** \brief SDMMC_APP_CMD Commands Index
 **
 ******************************************************************************/
#define ACMD6_APP_SD_SET_BUSWIDTH (6u)            /*!< (ACMD6) Defines the data bus width to be used for data transfer. The allowed data bus \
                                                       widths are given in SCR register.                                                        */
#define ACMD13_SD_APP_STATUS (13u)                /*!< (ACMD13) Sends the SD status.                                                            */
#define ACMD22_SD_APP_SEND_NUM_WRITE_BLOCKS (22u) /*!< (ACMD22) Sends the number of the written (without errors) write blocks. Responds with \
                                                       32bit+CRC data block.                                                                    */
#define ACMD41_SD_SEND_OP_COND (41u)              /*!< (ACMD41) Sends host capacity support information (HCS) and asks the accessed card to \
                                                       send its operating condition register (OCR) content in the response on the CMD line.     */
#define ACMD42_SET_CLR_CARD_DETECT (42u)          /*!< (ACMD42) Connect/Disconnect the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card  */
#define ACMD51_SD_APP_SEND_SCR (51u)              /*!< Reads the SD Configuration Register (SCR).                                               */
#define ACMD52_RW_DIRECT (52u)                    /*!< For SD I/O card only, reserved for security specification.                               */
#define ACMD53_RW_EXTENDED (53u)                  /*!< For SD I/O card only, reserved for security specification.                               */

/**
 *******************************************************************************
 ** \brief SD Card Specific security commands Commands Index
 **
 ******************************************************************************/
#define ACMD43_GET_MKB (43u)
#define ACMD44_GET_MID (44u)
#define ACMD45_SET_CER_RN1 (45u)
#define ACMD46_GET_CER_RN2 (46u)
#define ACMD47_SET_CER_RES2 (47u)
#define ACMD48_GET_CER_RES1 (48u)
#define ACMD18_SECURE_READ_MULTIPLE_BLOCK (18u)
#define ACMD25_SECURE_WRITE_MULTIPLE_BLOCK (25u)
#define ACMD38_SECURE_ERASE (38u)
#define ACMD49_CHANGE_SECURE_AREA (49u)
#define ACMD48_SECURE_WRITE_MKB (48u)

/**
 *******************************************************************************
 ** \brief Media voltage supported.
 **
 ******************************************************************************/
#define SDMMC_SD_VOLT_1_7 (0x00000010ul) /* Low voltage card minimum */
#define SDMMC_SD_VOLT_1_8 (0x00000020ul)
#define SDMMC_SD_VOLT_1_9 (0x00000040ul)
#define SDMMC_SD_VOLT_2_0 (0x00000080ul)
#define SDMMC_SD_VOLT_2_1 (0x00000100ul) /* Basic communication minimum */
#define SDMMC_SD_VOLT_2_2 (0x00000200ul)
#define SDMMC_SD_VOLT_2_3 (0x00000400ul)
#define SDMMC_SD_VOLT_2_4 (0x00000800ul)
#define SDMMC_SD_VOLT_2_5 (0x00001000ul)
#define SDMMC_SD_VOLT_2_6 (0x00002000ul)
#define SDMMC_SD_VOLT_2_7 (0x00004000ul)
#define SDMMC_SD_VOLT_2_8 (0x00008000ul) /* Memory access minimum */
#define SDMMC_SD_VOLT_2_9 (0x00010000ul)
#define SDMMC_SD_VOLT_3_0 (0x00020000ul)
#define SDMMC_SD_VOLT_3_1 (0x00040000ul)
#define SDMMC_SD_VOLT_3_2 (0x00080000ul)
#define SDMMC_SD_VOLT_3_3 (0x00100000ul)
#define SDMMC_SD_VOLT_3_4 (0x00200000ul)
#define SDMMC_SD_VOLT_3_5 (0x00400000ul)
#define SDMMC_SD_VOLT_3_6 (0x00800000ul)

#define SDMMC_ERROR_NONE (0x00000000ul)                   /*!< No error                                                      */
#define SDMMC_ERROR_CMD_CRC_FAIL (0x00000001ul)           /*!< Command response received (but CRC check failed)              */
#define SDMMC_ERROR_DATA_CRC_FAIL (0x00000002ul)          /*!< Data block sent/received (CRC check failed)                   */
#define SDMMC_ERROR_CMD_RSP_TIMEOUT (0x00000004ul)        /*!< Command response timeout                                      */
#define SDMMC_ERROR_DATA_TIMEOUT (0x00000008ul)           /*!< Data timeout                                                  */
#define SDMMC_ERROR_TX_UNDERRUN (0x00000010ul)            /*!< Transmit FIFO underrun                                        */
#define SDMMC_ERROR_RX_OVERRUN (0x00000020ul)             /*!< Receive FIFO overrun                                          */
#define SDMMC_ERROR_ADDR_MISALIGNED (0x00000040ul)        /*!< Misaligned address                                            */
#define SDMMC_ERROR_BLOCK_LEN_ERR (0x00000080ul)          /*!< Transferred block length is not allowed for the card or the \
                                                               number of transferred bytes does not match the block length   */
#define SDMMC_ERROR_ERASE_SEQ_ERR (0x00000100ul)          /*!< An error in the sequence of erase command occurs              */
#define SDMMC_ERROR_BAD_ERASE_PARAM (0x00000200ul)        /*!< An invalid selection for erase groups                         */
#define SDMMC_ERROR_WRITE_PROT_VIOLATION (0x00000400ul)   /*!< Attempt to program a write protect block                      */
#define SDMMC_ERROR_LOCK_UNLOCK_FAILED (0x00000800ul)     /*!< Sequence or password error has been detected in unlock \
                                                               command or if there was an attempt to access a locked card    */
#define SDMMC_ERROR_COM_CRC_FAILED (0x00001000ul)         /*!< CRC check of the previous command failed                      */
#define SDMMC_ERROR_ILLEGAL_CMD (0x00002000ul)            /*!< Command is not legal for the card state                       */
#define SDMMC_ERROR_CARD_ECC_FAILED (0x00004000ul)        /*!< Card internal ECC was applied but failed to correct the data  */
#define SDMMC_ERROR_CC_ERR (0x00008000ul)                 /*!< Internal card controller error                                */
#define SDMMC_ERROR_GENERAL_UNKNOWN_ERR (0x00010000ul)    /*!< General or unknown error                                      */
#define SDMMC_ERROR_STREAM_READ_UNDERRUN (0x00020000ul)   /*!< The card could not sustain data reading in stream rmode       */
#define SDMMC_ERROR_STREAM_WRITE_OVERRUN (0x00040000ul)   /*!< The card could not sustain data programming in stream mode    */
#define SDMMC_ERROR_CID_CSD_OVERWRITE (0x00080000ul)      /*!< CID/CSD overwrite error                                       */
#define SDMMC_ERROR_WP_ERASE_SKIP (0x00100000ul)          /*!< Only partial address space was erased                         */
#define SDMMC_ERROR_CARD_ECC_DISABLED (0x00200000ul)      /*!< Command has been executed without using internal ECC          */
#define SDMMC_ERROR_ERASE_RESET (0x00400000ul)            /*!< Erase sequence was cleared before executing because an out \
                                                               of erase sequence command was received                        */
#define SDMMC_ERROR_AKE_SEQ_ERR (0x00800000ul)            /*!< Error in sequence of authentication                           */
#define SDMMC_ERROR_INVALID_VOLTRANGE (0x01000000ul)      /*!< Error in case of invalid voltage range                        */
#define SDMMC_ERROR_ADDR_OUT_OF_RANGE (0x02000000ul)      /*!< Error when addressed block is out of range                    */
#define SDMMC_ERROR_REQUEST_NOT_APPLICABLE (0x04000000ul) /*!< Error when command request is not applicable                  */
#define SDMMC_ERROR_INVALID_PARAMETER (0x08000000ul)      /*!< the used parameter is not valid                               */
#define SDMMC_ERROR_UNSUPPORTED_FEATURE (0x10000000ul)    /*!< Error when feature is not insupported                         */
#define SDMMC_ERROR_BUSY (0x20000000ul)                   /*!< Error when transfer process is busy                           */
#define SDMMC_ERROR_DMA (0x40000000ul)                    /*!< Error while DMA transfer                                      */
#define SDMMC_ERROR_TIMEOUT (0x80000000ul)                /*!< Timeout error                                                 */

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t SDMMC_Cmd0_GoIdleState(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDMMC_Cmd1_SendOpCond(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32Argument, uint32_t *pu32OCR);
    en_result_t SDMMC_Cmd2_AllSendCID(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32CID[4]);
    en_result_t SDMMC_Cmd3_SendRelativeAddr(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32RCA);
    en_result_t SDMMC_Cmd6_SwitchFunc(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32Argument, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd7_SelectDeselectCard(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32RCA, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd8_SendIfCond(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32IfCond);
    en_result_t SDMMC_Cmd9_SendCSD(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32RCA, uint32_t u32CSD[4]);
    en_result_t SDMMC_Cmd10_SendCID(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32RCA, uint32_t u32CID[4]);
    en_result_t SDMMC_Cmd12_StopTransmission(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd13_SendStatus(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32RCA, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd16_SetBlockLength(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32BlockLen, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd17_ReadSingleBlock(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32ReadAddr, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd18_ReadMultipleBlock(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32ReadAddr, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd24_WriteSingleBlock(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32WriteAddr, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd25_WriteMultipleBlock(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32WriteAddr, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd32_EraseWrBlkStart(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32StartAddr, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd33_EraseWrBlkEnd(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32EndAddr, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd35_SetEraseStartAddr(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32StartAddr, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd36_SetEraseEndAddr(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32EndAddr, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd38_Erase(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32Resp1);
    en_result_t SDMMC_Cmd55_AppCmd(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32Argument, uint32_t *pu32Resp1);
    en_result_t SDMMC_Acmd6_SetBusWidth(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32BusWidth, uint32_t *pu32Resp1);
    en_result_t SDMMC_Acmd13_SdStatus(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32Resp1);
    en_result_t SDMMC_Acmd41_SdSendOpCond(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32SdType, uint32_t *pu32OCR);
    en_result_t SDMMC_Acmd51_SendSCR(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32Resp1);

    //@} // SdiocGroup

#ifdef __cplusplus
}
#endif

#endif /* __SDMMC_CMD_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
