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
/** \file sdmmc_cmd.c
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

/**
 *******************************************************************************
 ** \addtogroup SdiocGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Parameter valid check for SDIOC Instances. */
#define IS_VALID_SDIOC(__SDIOCx__)  \
    ((M4_SDIOC1 == (__SDIOCx__)) || \
     (M4_SDIOC2 == (__SDIOCx__)))

/*!< Parameter valid check for SDIOC Instances. */
#define IS_VALID_SD_TYPE(x)        \
    ((SdmmcHighCapacity == (x)) || \
     (SdmmcStanderdCapacity == (x)))

/*!< CMDBSY bit loop count */
#define SDIOC_FLAG_CMDBUSY_LOOPS (200000ul)

/*!< Command send and response timeout */
#define SDMMC_CMD_TIMEOUT (10000ul)

/*!< OCR register bit */
#define SDMMC_SD_OCR_HCS (0x40000000ul)  /* High Capacity Support */
#define SDMMC_SD_OCR_BUSY (0x80000000ul) /* Card power up status bit (busy) */

#define SDMMC_BUS_WIDTH_1BIT (0x00000000ul)
#define SDMMC_BUS_WIDTH_4BIT (0x00000002ul)

#define SDMMC_CMD8_CHECK_PATTERN (0x000000AAul)
#define SDMMC_CMD8_VOLTAGE_SUPPLIED (0x00000100ul) /* 2.7 - 3.6v*/
#define SDMMC_CMD8_ARGUEMENT (SDMMC_CMD8_VOLTAGE_SUPPLIED | SDMMC_CMD8_CHECK_PATTERN)
#define SDMMC_ACMD41_VOLTAGE (SDMMC_SD_VOLT_3_3)
#define SDMMC_ACMD41_HCS (1ul << 30u)

/*!< Masks for R1 Response (SD Card Status Register) */
#define SDMMC_R1_AKE_SEQ_ERROR (0x1ul << 3u)
#define SDMMC_R1_ERASE_RESET (0x1ul << 13u)
#define SDMMC_R1_CARD_ECC_DISABLED (0x1ul << 14u)
#define SDMMC_R1_WP_ERASE_SKIP (0x1ul << 15u)
#define SDMMC_R1_CID_CSD_OVERWRITE (0x1ul << 16u)
#define SDMMC_R1_STREAM_WRITE_OVERRUN (0x1ul << 17u)
#define SDMMC_R1_STREAM_READ_UNDERRUN (0x1ul << 18u)
#define SDMMC_R1_GENERAL_UNKNOWN_ERROR (0x1ul << 19u)
#define SDMMC_R1_CC_ERROR (0x1ul << 20u)
#define SDMMC_R1_CARD_ECC_FAILED (0x1ul << 21u)
#define SDMMC_R1_ILLEGAL_COMMAND (0x1ul << 22u)
#define SDMMC_R1_COM_CRC_ERROR (0x1ul << 23u)
#define SDMMC_R1_LOCK_UNLOCK_FAILED (0x1ul << 24u)
#define SDMMC_R1_WP_VIOLATION (0x1ul << 26u)
#define SDMMC_R1_ERASE_PARAM_ERROR (0x1ul << 27u)
#define SDMMC_R1_ERASE_SEQ_ERROR (0x1ul << 28u)
#define SDMMC_R1_BLOCK_LEN_ERROR (0x1ul << 29u)
#define SDMMC_R1_ADDRESS_ERROR (0x1ul << 30u)
#define SDMMC_R1_OUT_OF_RANGE (0x1ul << 31u)
#define SDMMC_R1_ERRORS (0xFDFFE008ul)

/*!< Masks for R6 Response. */
#define SDMMC_R6_GENERAL_UNKNOWN_ERROR (0x1ul << 13u)
#define SDMMC_R6_ILLEGAL_CMD (0x1ul << 14u)
#define SDMMC_R6_COM_CRC_ERROR (0x1ul << 15u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static en_result_t SDMMC_GetCmdResp1(M4_SDIOC_TypeDef *SDIOCx,
                                     uint32_t *pu32CmdResp1,
                                     uint32_t u32Timeout);
static en_result_t SDMMC_GetCmdResp1b(M4_SDIOC_TypeDef *SDIOCx,
                                      uint32_t *pu32CmdResp1,
                                      uint32_t u32Timeout);
static en_result_t SDMMC_GetCmdResp2(M4_SDIOC_TypeDef *SDIOCx,
                                     uint32_t u32CID_CSD[4]);
static en_result_t SDMMC_GetCmdResp3(M4_SDIOC_TypeDef *SDIOCx,
                                     uint32_t *pu32OCR);
static en_result_t SDMMC_GetCmdResp6(M4_SDIOC_TypeDef *SDIOx,
                                     uint32_t *pu32RCA);
static en_result_t SDMMC_GetCmdResp7(M4_SDIOC_TypeDef *SDIOCx,
                                     uint32_t *pu32IfCond);
static en_result_t SDMMC_WaitBusIdle(M4_SDIOC_TypeDef *SDIOCx);
static en_result_t SDMMC_WaitResponse(M4_SDIOC_TypeDef *SDIOCx,
                                      uint32_t u32Timeout);
static bool SDMMC_GetCardDetection(M4_SDIOC_TypeDef *SDIOCx);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Send the GO_IDLE_STATE command which will reset all cards to idle state
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd0_GoIdleState(M4_SDIOC_TypeDef *SDIOCx)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            stcCmdCfg.u8CmdIndex = CMD0_GO_IDLE_STATE;
            stcCmdCfg.u32Argument = 0u;
            stcCmdCfg.enRspIndex = SdiocCmdNoRsp;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            /* Set command and argument. */
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_WaitResponse(SDIOCx, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SEND_OP_COND command.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32Argument             Argument used for the command.
 ** \param [in] pu32OCR                 Pointer to buffer which will store OCR content.
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd1_SendOpCond(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32Argument, uint32_t *pu32OCR)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            stcCmdCfg.u8CmdIndex = CMD1_SEND_OP_COND;
            stcCmdCfg.u32Argument = u32Argument;
            stcCmdCfg.enRspIndex = SdiocCmdRspR3;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;

            /* Set command and argument. */
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp3(SDIOCx, pu32OCR);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the ALL_SEND_CID command for asking any card to send the CID numbers
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32CID                  Pointer to buffer which will store CID content.
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd2_AllSendCID(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32CID[4])
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            stcCmdCfg.u8CmdIndex = CMD2_ALL_SEND_CID;
            stcCmdCfg.u32Argument = 0u;
            stcCmdCfg.enRspIndex = SdiocCmdRspR2;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;

            /* Set command and argument. */
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp2(SDIOCx, u32CID);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SEND_RELATIVE_ADDR command for asking the card to publish a
 **        new relative address(RCA)
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pu32RCA                 Pointer to buffer which will store RCA.
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd3_SendRelativeAddr(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32RCA)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            stcCmdCfg.u8CmdIndex = CMD3_SEND_RELATIVE_ADDR;
            stcCmdCfg.u32Argument = 0u;
            stcCmdCfg.enRspIndex = SdiocCmdRspR6;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;

            /* Set command and argument. */
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp6(SDIOCx, pu32RCA);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SWITCH_FUNC command.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32Argument             Argument used for the command.
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd6_SwitchFunc(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32Argument, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            stcCmdCfg.u8CmdIndex = CMD6_SWITCH_FUNC;
            stcCmdCfg.u32Argument = u32Argument;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;

            /* Set command and argument. */
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SELECT/DESELECT_CARD command for toggling a card between the
 **        stand-by and transfer states or between the programming and disconnect states.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32RCA                  Relative Sdcard Address.
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd7_SelectDeselectCard(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32RCA, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD7_SEL_DESEL_CARD;
            stcCmdCfg.u32Argument = (u32RCA << 16u);
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SEND_IF_COND command. Sends SD Memory Card interface
 **        condition, which includes host supply voltage information and asks
 **        the card whether card supports voltage
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pu32IfCond              Pointer to buffers for storing interface condition.
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd8_SendIfCond(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32IfCond)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Send CMD8 to verify SD card interface operating condition */
            /* Argument: - [31:12]: Reserved (shall be set to '0')
            - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
            - [7:0]: Check Pattern (recommended 0xAA) */
            /* CMD Response: R7 */
            stcCmdCfg.u8CmdIndex = CMD8_SEND_IF_COND;
            stcCmdCfg.u32Argument = SDMMC_CMD8_ARGUEMENT;
            stcCmdCfg.enRspIndex = SdiocCmdRspR7;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg); /* Set command and argument. */
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp7(SDIOCx, pu32IfCond);
                if (enRet == Ok)
                {
                    if ((*pu32IfCond & 0xFFu) != SDMMC_CMD8_CHECK_PATTERN)
                    {
                        enRet = Error;
                    }
                }
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SEND_CSD command to addressed card which will sends its
 **        card-specificdata (CSD).
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32RCA                  Relative Card Address
 ** \param [in] u32CSD                  Pointer to buffers for storing CSD contents.
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd9_SendCSD(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32RCA, uint32_t u32CSD[4])
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD9_SEND_CSD;
            stcCmdCfg.u32Argument = (u32RCA << 16u);
            stcCmdCfg.enRspIndex = SdiocCmdRspR2;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp2(SDIOCx, u32CSD);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SEND_CID command to addressed card which will sends its card
 **        identification(CID).
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32RCA                  Relative Card Address
 ** \param [in] u32CID                  Pointer to buffers for storing CID contents.
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd10_SendCID(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32RCA, uint32_t u32CID[4])
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD10_SEND_CID;
            stcCmdCfg.u32Argument = (u32RCA << 16u);
            stcCmdCfg.enRspIndex = SdiocCmdRspR2;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp2(SDIOCx, u32CID);
            }
        }
    }

    return enRet;
}

/*******************************************************************************
** \brief Send the STOP_TRANSMISSION command to force the card to stop transmission.
**
** \param [in] SDIOCx                  Pointer to SDIOC instance register base
** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
**
** \retval Ok                          Send successfully.
** \retval Error                       Send unsuccessfully.
**
*******************************************************************************/
en_result_t SDMMC_Cmd12_StopTransmission(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD12_STOP_TRANSMISSION;
            stcCmdCfg.u32Argument = 0u;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SEND_STATUS to addressed card which will send its status
 **        register contents
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32RCA                  Relative Card Address
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd13_SendStatus(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32RCA, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD13_SEND_STATUS;
            stcCmdCfg.u32Argument = (u32RCA << 16u);
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SET_BLOCKLEN command and check the response
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32BlockLen             Block length
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd16_SetBlockLength(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32BlockLen, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD16_SET_BLOCKLEN;
            stcCmdCfg.u32Argument = u32BlockLen;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the READ_SINGLE_BLOCK command and check the response
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32ReadAddr             Data address
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd17_ReadSingleBlock(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32ReadAddr, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD17_READ_SINGLE_BLOCK;
            stcCmdCfg.u32Argument = u32ReadAddr;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Enable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the READ_MULTIPLE_BLOCK command and check the response
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32ReadAddr             Data address
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd18_ReadMultipleBlock(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32ReadAddr, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD18_READ_MULTIPLE_BLOCK;
            stcCmdCfg.u32Argument = u32ReadAddr;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Enable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the WRITE_BLOCK command and check the response
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32WriteAddr            Data address
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd24_WriteSingleBlock(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32WriteAddr, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD24_WRITE_SINGLE_BLOCK;
            stcCmdCfg.u32Argument = u32WriteAddr;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Enable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the WRITE_MULTIPLE_BLOCK command and check the response
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32WriteAddr            Data address
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd25_WriteMultipleBlock(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32WriteAddr, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD25_WRITE_MULTIPLE_BLOCK;
            stcCmdCfg.u32Argument = u32WriteAddr;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Enable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the ERASE_WR_BLK_START command for setting the address of the
 **        first write block to be erased.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32StartAddr            The start address will be erased
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd32_EraseWrBlkStart(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32StartAddr, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD32_ERASE_WR_BLK_START;
            stcCmdCfg.u32Argument = u32StartAddr;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the ERASE_WR_BLK_END command for setting the address of the
 **        last write block of the continuous range to be erased.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32EndAddr              The last address will be erased
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd33_EraseWrBlkEnd(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32EndAddr, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD33_ERASE_WR_BLK_END;
            stcCmdCfg.u32Argument = u32EndAddr;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the ERASE_GRP_START command and check the response.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32StartAddr            The start address will be erased
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd35_SetEraseStartAddr(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32StartAddr, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD35_ERASE_GRP_START;
            stcCmdCfg.u32Argument = u32StartAddr;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the ERASE_GRP_END command and check the response.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32EndAddr              The end address will be erased
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd36_SetEraseEndAddr(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32EndAddr, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD36_ERASE_GRP_END;
            stcCmdCfg.u32Argument = u32EndAddr;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the ERASE command for erasing all previously selected write blocks.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd38_Erase(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            SDIOC_SetDataTimeout(SDIOCx, SdiocDtoSdclk_2_27);

            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD38_ERASE;
            stcCmdCfg.u32Argument = 0u;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1b;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1b(SDIOCx, pu32Resp1, 5u * SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the APP_CMD Command to interpret the following command as an
 **        application-specific command(ACMD).
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32Argument             Command argument(RCA)
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Cmd55_AppCmd(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32Argument, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = CMD55_APP_CMD;
            stcCmdCfg.u32Argument = u32Argument;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SET_BUS_WIDTH(ACMD6) Command to set bus width.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32BusWidth             The data bus width, '00'= 1bit or '10' = 4bits bus
 ** \arg SDMMC_BUS_WIDTH_1BIT           1 bits bus
 ** \arg SDMMC_BUS_WIDTH_4BIT           4 bits bus
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Acmd6_SetBusWidth(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32BusWidth, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = ACMD6_APP_SD_SET_BUSWIDTH;
            stcCmdCfg.u32Argument = u32BusWidth;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SD_STATUS(ACMD13) Command to ask the Status Register.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Acmd13_SdStatus(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = ACMD13_SD_APP_STATUS;
            stcCmdCfg.u32Argument = 0u;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SD_SEND_OP_COND(ACMD41) command to negotiate the operation
 **        voltage range and get OCR.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32SdType               Sdcard type
 ** \arg SdmmcHighCapacity              SD memory card: Standard capacity
 ** \arg SdmmcHighCapacity              SD memory card: High capacity
 ** \param [in] pu32OCR                 Pointer to the variable that will contain
 **                                     the OCR register contents.
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Acmd41_SdSendOpCond(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32SdType, uint32_t *pu32OCR)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if ((IS_VALID_SDIOC(SDIOCx)) && (NULL != pu32OCR))
    {
        DDL_ASSERT(IS_VALID_SD_TYPE(u32SdType));

        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = ACMD41_SD_SEND_OP_COND;
            stcCmdCfg.u32Argument = SDMMC_ACMD41_VOLTAGE | u32SdType;
            stcCmdCfg.enRspIndex = SdiocCmdRspR3;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp3(SDIOCx, pu32OCR);
                if (enRet == Ok)
                {
                    if (!(*pu32OCR & SDMMC_SD_OCR_BUSY))
                    {
                        enRet = ErrorOperationInProgress;
                    }
                }
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send the SEND_SCR(ACMD51) command to read the SD Configuration Register(SCR).
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [out] pu32Resp1              Pointer to response data buffer
 **
 ** \retval Ok                          Send successfully.
 ** \retval Error                       Send unsuccessfully.
 **
 ******************************************************************************/
en_result_t SDMMC_Acmd51_SendSCR(M4_SDIOC_TypeDef *SDIOCx, uint32_t *pu32Resp1)
{
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_cmd_cfg_t stcCmdCfg = {0};

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitBusIdle(SDIOCx);
        if (enRet == Ok)
        {
            /* Set command and argument. */
            stcCmdCfg.u8CmdIndex = ACMD51_SD_APP_SEND_SCR;
            stcCmdCfg.u32Argument = 0u;
            stcCmdCfg.enRspIndex = SdiocCmdRspR1;
            stcCmdCfg.enDataPresentEnable = Disable;
            stcCmdCfg.enCmdType = SdiocCmdNormal;
            enRet = SDIOC_SendCommand(SDIOCx, &stcCmdCfg);
            if (enRet == Ok)
            {
                /* Check for error conditions */
                enRet = SDMMC_GetCmdResp1(SDIOCx, pu32Resp1, SDMMC_CMD_TIMEOUT);
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Response R1(normal response command).
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32Timeout              timeout for waiting SDIOC idle
 **
 ** \retval Ok                          Get successfully.
 ** \retval Error                       Get unsuccessfully.
 ** \retval ErrorInvalidParameter       pu32CmdResp1 == NULL
 **
 ******************************************************************************/
static en_result_t SDMMC_GetCmdResp1(M4_SDIOC_TypeDef *SDIOCx,
                                     uint32_t *pu32CmdResp1,
                                     uint32_t u32Timeout)
{
    uint32_t u32R1 = 0u;
    en_result_t enRet = ErrorInvalidParameter;

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitResponse(SDIOCx, u32Timeout);
        if (enRet == Ok)
        {
            /* received response, retrieve it for analysis  */
            u32R1 = SDIOC_GetResponse(SDIOCx, SdiocRegResp01);
            if (u32R1 & SDMMC_R1_ERRORS)
            {
                enRet = Error;
            }

            if (pu32CmdResp1 != NULL)
            {
                *pu32CmdResp1 = u32R1;
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Response R1(normal response command) with busy signal.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32Timeout              timeout for waiting SDIOC idle
 **
 ** \retval Ok                          Get successfully.
 ** \retval Error                       Get unsuccessfully.
 ** \retval ErrorInvalidParameter       pu32CmdResp1 == NULL
 **
 ******************************************************************************/
static en_result_t SDMMC_GetCmdResp1b(M4_SDIOC_TypeDef *SDIOCx,
                                      uint32_t *pu32CmdResp1,
                                      uint32_t u32Timeout)
{
    uint32_t u32R1 = 0u;
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint32_t u32TmpCnt = u32Timeout;

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = SDMMC_WaitResponse(SDIOCx, u32Timeout);
        if (enRet == Ok)
        {
            do
            {
                if (u32TmpCnt-- == 0u)
                {
                    enRet = ErrorTimeout;
                }
            } while (Reset == SDIOC_GetStatus(SDIOCx, SdiocData0PinLvl));

            if (enRet == Ok)
            {
                /* received response, retrieve it for analysis  */
                u32R1 = SDIOC_GetResponse(SDIOCx, SdiocRegResp01);
                if (u32R1 & SDMMC_R1_ERRORS)
                {
                    enRet = Error;
                }

                if (pu32CmdResp1 != NULL)
                {
                    *pu32CmdResp1 = u32R1;
                }
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Response R2(CID, CSD register).
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32CID_CSD              Pointer to the variable that will contain the
 **                                     CID or CSD register contents
 **
 ** \retval Ok                          Get successfully.
 ** \retval Error                       Get unsuccessfully.
 ** \retval ErrorInvalidParameter       u32CID_CSD == NULL
 **
 ******************************************************************************/
static en_result_t SDMMC_GetCmdResp2(M4_SDIOC_TypeDef *SDIOCx,
                                     uint32_t u32CID_CSD[4])
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((IS_VALID_SDIOC(SDIOCx)) && (NULL != u32CID_CSD))
    {
        enRet = SDMMC_WaitResponse(SDIOCx, SDMMC_CMD_TIMEOUT);
        if (enRet == Ok)
        {
            /* received response */
            u32CID_CSD[0] = SDIOC_GetResponse(SDIOCx, SdiocRegResp01);
            u32CID_CSD[1] = SDIOC_GetResponse(SDIOCx, SdiocRegResp23);
            u32CID_CSD[2] = SDIOC_GetResponse(SDIOCx, SdiocRegResp45);
            u32CID_CSD[3] = SDIOC_GetResponse(SDIOCx, SdiocRegResp67);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Response R3(OCR register).
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pu32OCR                 Pointer to the variable that will contain
 **                                     the OCR register contents.
 **
 ** \retval Ok                          Get successfully.
 ** \retval Error                       Get unsuccessfully.
 ** \retval ErrorInvalidParameter       pu32OCR == NULL
 **
 ******************************************************************************/
static en_result_t SDMMC_GetCmdResp3(M4_SDIOC_TypeDef *SDIOCx,
                                     uint32_t *pu32OCR)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((IS_VALID_SDIOC(SDIOCx)) && (NULL != pu32OCR))
    {
        enRet = SDMMC_WaitResponse(SDIOCx, SDMMC_CMD_TIMEOUT);
        if (enRet == Ok)
        {
            /* received response, retrieve it for analysis  */
            *pu32OCR = SDIOC_GetResponse(SDIOCx, SdiocRegResp01);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Response R6(Published RCA response).
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pu32RCA                 Pointer to the variable that will contain
 **                                     published RCA number
 **
 ** \retval Ok                          Get successfully.
 ** \retval Error                       Get unsuccessfully.
 ** \retval ErrorInvalidParameter       pu32RCA == NULL
 **
 ******************************************************************************/
static en_result_t SDMMC_GetCmdResp6(M4_SDIOC_TypeDef *SDIOCx,
                                     uint32_t *pu32RCA)
{
    uint32_t u32R6 = 0ul;
    en_result_t enRet = ErrorInvalidParameter;

    if ((IS_VALID_SDIOC(SDIOCx)) && (NULL != pu32RCA))
    {
        enRet = SDMMC_WaitResponse(SDIOCx, SDMMC_CMD_TIMEOUT);
        if (enRet == Ok)
        {
            /* received response, retrieve it. */
            u32R6 = SDIOC_GetResponse(SDIOCx, SdiocRegResp01);
            if (!(u32R6 & (SDMMC_R6_GENERAL_UNKNOWN_ERROR | SDMMC_R6_ILLEGAL_CMD | SDMMC_R6_COM_CRC_ERROR)))
            {
                *pu32RCA = (u32R6 >> 16);
            }
            else
            {
                enRet = Error;
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get Response R7.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pu32IfCond              Pointer to the variable that will contain
 **                                     interface condition.
 **
 ** \retval Ok                          Get successfully.
 ** \retval Error                       Get unsuccessfully.
 ** \retval ErrorInvalidParameter       pu32IfCond == NULL
 **
 ******************************************************************************/
static en_result_t SDMMC_GetCmdResp7(M4_SDIOC_TypeDef *SDIOCx,
                                     uint32_t *pu32IfCond)
{
    en_result_t enRet = ErrorInvalidParameter;

    if ((IS_VALID_SDIOC(SDIOCx)) && (NULL != pu32IfCond))
    {
        enRet = SDMMC_WaitResponse(SDIOCx, SDMMC_CMD_TIMEOUT);
        if (enRet == Ok)
        {
            *pu32IfCond = SDIOC_GetResponse(SDIOCx, SdiocRegResp01);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Wait for bus line idle
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval Ok                          Command&Data line is free within a certain time
 ** \retval ErrorTimeout                Command&Data line is still busy after a certain time.
 ** \retval ErrorInvalidParameter       SDIOCx == NULL
 **
 ******************************************************************************/
static en_result_t SDMMC_WaitBusIdle(M4_SDIOC_TypeDef *SDIOCx)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t u32Retry = SDIOC_FLAG_CMDBUSY_LOOPS;
    en_flag_status_t enInhibitCmdStatus = Set;
    en_flag_status_t enInhibitDataStatus = Set;

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = ErrorTimeout;
        while (u32Retry-- > 0ul)
        {
            enInhibitCmdStatus = SDIOC_GetStatus(SDIOCx, SdiocCommandInhibitCmd);
            enInhibitDataStatus = SDIOC_GetStatus(SDIOCx, SdiocCommandInhibitData);
            if ((Reset == enInhibitCmdStatus) || (Reset == enInhibitDataStatus))
            {
                enRet = Ok;
                break;
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Wait response
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32Timeout              timeout times
 **
 ** \retval Ok                          Respond normal
 ** \retval ErrorTimeout                Respond timeout.
 ** \retval ErrorInvalidParameter       SDIOCx == NULL
 ** \retval ErrorAccessRights           Card is removed.
 **
 ******************************************************************************/
static en_result_t SDMMC_WaitResponse(M4_SDIOC_TypeDef *SDIOCx,
                                      uint32_t u32Timeout)
{
    /* 8 is the number of required instructions cycles for the below loop statement.
    The Timeout is expressed in ms */
    __IO uint32_t u32Count = u32Timeout * (SystemCoreClock / 8u / 1000u);
    en_flag_status_t enErrorInt = Set;
    en_flag_status_t enCommandComplete = Set;
    en_result_t enRet = ErrorInvalidParameter;

    if (IS_VALID_SDIOC(SDIOCx))
    {
        enRet = Ok;
        do
        {
            if (u32Count-- == 0u)
            {
                enRet = ErrorTimeout;
                break;
            }
            enErrorInt = SDIOC_GetNormalIrqFlag(SDIOCx, SdiocErrorInt);
            enCommandComplete = SDIOC_GetNormalIrqFlag(SDIOCx, SdiocCommandComplete);
        } while ((Reset == enErrorInt) && (Reset == enCommandComplete));

        if (enRet == Ok)
        {
            if (Set == SDIOC_GetNormalIrqFlag(SDIOCx, SdiocErrorInt))
            {
                enRet = Error;
            }
            else
            {
                if (false == SDMMC_GetCardDetection(SDIOCx))
                {
                    enRet = ErrorAccessRights;
                }
                else
                {
                    if (Set == SDIOC_GetNormalIrqFlag(SDIOCx, SdiocCommandComplete))
                    {
                        SDIOC_ClearNormalIrqFlag(SDIOCx, SdiocCommandComplete);
                    }

                    if (Set == SDIOC_GetNormalIrqFlag(SDIOCx, SdiocTransferComplete))
                    {
                        SDIOC_ClearNormalIrqFlag(SDIOCx, SdiocTransferComplete);
                    }
                }
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get card insertion status
 **
 ** This function checks card is inserted or not.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval true                        Card is inserted
 ** \retval false                       Card is removed
 **
 ******************************************************************************/
static bool SDMMC_GetCardDetection(M4_SDIOC_TypeDef *SDIOCx)
{
    bool bRet = false;
    __IO uint32_t u32Count = 5000ul * (SystemCoreClock / 8ul / 1000ul);

    /* Wait until card is stable */
    do
    {
        if (Set == SDIOC_GetStatus(SDIOCx, SdiocCardStateStable))
        {
            break;
        }
    } while (u32Count--);

    if (u32Count != 0ul)
    {
        bRet = (Set == SDIOC_GetStatus(SDIOCx, SdiocCardInserted)) ? true : false;
    }

    return bRet;
}

//@} // SdiocGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
