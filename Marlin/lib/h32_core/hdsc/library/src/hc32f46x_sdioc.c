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
/** \file hc32f46x_sdioc.c
 **
 ** A detailed description is available at
 ** @link SdiocGroup SDIOC description @endlink
 **
 **   - 2018-11-11  1.0 Hongjh First version for Device Driver Library of SDIOC.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_sdioc.h"
#include "hc32f46x_utility.h"

#if (DDL_SDIOC_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup SdiocGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief SDIOC internal data
 **
 ******************************************************************************/
typedef struct stc_sdioc_intern_data
{
    stc_sdioc_normal_irq_cb_t stcNormalIrqCb; ///< Normal irq callback function structure

    stc_sdioc_error_irq_cb_t stcErrorIrqCb; ///< Error irq callback function structure
} stc_sdioc_intern_data_t;

/**
 *******************************************************************************
 ** \brief SDIOC instance data
 **
 ******************************************************************************/
typedef struct stc_sdioc_instance_data
{
    const M4_SDIOC_TypeDef *SDIOCx; ///< pointer to registers of an instance

    stc_sdioc_intern_data_t stcInternData; ///< module internal data of instance
} stc_sdioc_instance_data_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Parameter valid check for SDIOC Instances. */
#define IS_VALID_SDIOC(__SDIOCx__)  \
    ((M4_SDIOC1 == (__SDIOCx__)) || \
     (M4_SDIOC2 == (__SDIOCx__)))

/*!< Parameter valid check for SDIOC Response Register. */
#define IS_VALID_SDIOC_RESP(x)  \
    ((SdiocRegResp01 == (x)) || \
     (SdiocRegResp23 == (x)) || \
     (SdiocRegResp45 == (x)) || \
     (SdiocRegResp67 == (x)))

/*!< Parameter valid check for SDIOC bus width. */
#define IS_VALID_SDIOC_BUS_WIDTH(x) \
    ((SdiocBusWidth1Bit == (x)) ||  \
     (SdiocBusWidth4Bit == (x)) ||  \
     (SdiocBusWidth8Bit == (x)))

/*!< Parameter valid check for SDIOC speed mode. */
#define IS_VALID_SDIOC_SPEED_MODE(x) \
    ((SdiocHighSpeedMode == (x)) ||  \
     (SdiocNormalSpeedMode == (x)))

/*!< Parameter valid check for SDIOC Clock division. */
#define IS_VALID_SDIOC_CLK_DIV(x) \
    ((SdiocClkDiv_1 == (x)) ||    \
     (SdiocClkDiv_2 == (x)) ||    \
     (SdiocClkDiv_4 == (x)) ||    \
     (SdiocClkDiv_8 == (x)) ||    \
     (SdiocClkDiv_16 == (x)) ||   \
     (SdiocClkDiv_32 == (x)) ||   \
     (SdiocClkDiv_64 == (x)) ||   \
     (SdiocClkDiv_128 == (x)) ||  \
     (SdiocClkDiv_256 == (x)))

/*!< Parameter valid check for SDIOC command type. */
#define IS_VALID_SDIOC_CMD_TYPE(x) \
    ((SdiocCmdAbort == (x)) ||     \
     (SdiocCmdResume == (x)) ||    \
     (SdiocCmdNormal == (x)) ||    \
     (SdiocCmdSuspend == (x)))

/*!< Parameter valid check for SDIOC data transfer direction. */
#define IS_VALID_SDIOC_TRANSFER_DIR(x) \
    ((SdiocTransferToCard == (x)) ||   \
     (SdiocTransferToHost == (x)))

/*!< Parameter valid check for SDIOC software reset type. */
#define IS_VALID_SDIOC_SWRESETTYPE(x) \
    ((SdiocSwResetAll == (x)) ||      \
     (SdiocSwResetCmdLine == (x)) ||  \
     (SdiocSwResetDatLine == (x)))

/*!< Parameter valid check for SDIOC data transfer mode. */
#define IS_VALID_SDIOC_TRANSFER_MODE(x) \
    ((SdiocTransferSingle == (x)) ||    \
     (SdiocTransferInfinite == (x)) ||  \
     (SdiocTransferMultiple == (x)) ||  \
     (SdiocTransferStopMultiple == (x)))

/*!< Parameter valid check for SDIOC data timeout. */
#define IS_VALID_SDIOC_DATA_TIMEOUT(x) \
    ((SdiocDtoSdclk_2_13 == (x)) ||    \
     (SdiocDtoSdclk_2_14 == (x)) ||    \
     (SdiocDtoSdclk_2_15 == (x)) ||    \
     (SdiocDtoSdclk_2_16 == (x)) ||    \
     (SdiocDtoSdclk_2_17 == (x)) ||    \
     (SdiocDtoSdclk_2_18 == (x)) ||    \
     (SdiocDtoSdclk_2_19 == (x)) ||    \
     (SdiocDtoSdclk_2_20 == (x)) ||    \
     (SdiocDtoSdclk_2_21 == (x)) ||    \
     (SdiocDtoSdclk_2_22 == (x)) ||    \
     (SdiocDtoSdclk_2_23 == (x)) ||    \
     (SdiocDtoSdclk_2_24 == (x)) ||    \
     (SdiocDtoSdclk_2_25 == (x)) ||    \
     (SdiocDtoSdclk_2_26 == (x)) ||    \
     (SdiocDtoSdclk_2_27 == (x)))

/*!< Parameter valid check for SDIOC Response type name. */
#define IS_VALID_SDIOC_RESP_TYPE_NAME(x) \
    ((SdiocCmdRspR1 == (x)) ||           \
     (SdiocCmdRspR1b == (x)) ||          \
     (SdiocCmdRspR2 == (x)) ||           \
     (SdiocCmdRspR3 == (x)) ||           \
     (SdiocCmdRspR4 == (x)) ||           \
     (SdiocCmdRspR5 == (x)) ||           \
     (SdiocCmdRspR5b == (x)) ||          \
     (SdiocCmdRspR6 == (x)) ||           \
     (SdiocCmdRspR7 == (x)) ||           \
     (SdiocCmdNoRsp == (x)))

/*!< Parameter valid check for SDIOC data timeout. */
#define IS_VALID_SDIOC_HOST_STATUS(x)    \
    ((SdiocCmdPinLvl == (x)) ||          \
     (SdiocData0PinLvl == (x)) ||        \
     (SdiocData1PinLvl == (x)) ||        \
     (SdiocData2PinLvl == (x)) ||        \
     (SdiocData3PinLvl == (x)) ||        \
     (SdiocCardInserted == (x)) ||       \
     (SdiocDataLineActive == (x)) ||     \
     (SdiocCardStateStable == (x)) ||    \
     (SdiocBufferReadEnble == (x)) ||    \
     (SdiocBufferWriteEnble == (x)) ||   \
     (SdiocCardDetectPinLvl == (x)) ||   \
     (SdiocCommandInhibitCmd == (x)) ||  \
     (SdiocWriteProtectPinLvl == (x)) || \
     (SdiocCommandInhibitData == (x)) || \
     (SdiocReadTransferActive == (x)) || \
     (SdiocWriteTransferActive == (x)))

/*!< Parameter valid check for SDIOC normal interrupt. */
#define IS_VALID_SDIOC_NOR_INT(x)      \
    ((SdiocCardInt == (x)) ||          \
     (SdiocErrorInt == (x)) ||         \
     (SdiocCardRemoval == (x)) ||      \
     (SdiocBlockGapEvent == (x)) ||    \
     (SdiocCardInsertedInt == (x)) ||  \
     (SdiocCommandComplete == (x)) ||  \
     (SdiocBufferReadReady == (x)) ||  \
     (SdiocBufferWriteReady == (x)) || \
     (SdiocTransferComplete == (x)))

/*!< Parameter valid check for SDIOC error interrupt. */
#define IS_VALID_SDIOC_ERR_INT(x)   \
    ((SdiocCmdCrcErr == (x)) ||     \
     (SdiocDataCrcErr == (x)) ||    \
     (SdiocCmdIndexErr == (x)) ||   \
     (SdiocCmdEndBitErr == (x)) ||  \
     (SdiocAutoCmd12Err == (x)) ||  \
     (SdiocCmdTimeoutErr == (x)) || \
     (SdiocDataEndBitErr == (x)) || \
     (SdiocDataTimeoutErr == (x)))

/*!< Parameter valid check for SDIOC auto CMD12 error status. */
#define IS_VALID_SDIOC_AUTOCMD_ERR(x)    \
    ((SdiocCmdNotIssuedErr == (x)) ||    \
     (SdiocAutoCmd12CrcErr == (x)) ||    \
     (SdiocAutoCmd12Timeout == (x)) ||   \
     (SdiocAutoCmd12IndexErr == (x)) ||  \
     (SdiocAutoCmd12EndBitErr == (x)) || \
     (SdiocAutoCmd12NotExecuted == (x)))

/*!< Parameter valid check for SDIOC detect card signal. */
#define IS_VALID_SDIOC_DETECT_SIG(x) \
    ((SdiocSdcdPinLevel == (x)) ||   \
     (SdiocCardDetectTestLevel == (x)))

/*!< Parameter valid check for SDIOC data block count value. */
#define IS_VALID_SDIOC_BLKCNT(x) ((x) != 0u)

/*!< Parameter valid check for SDIOC data block size value. */
#define IS_VALID_SDIOC_BLKSIZE(x) (!((x)&0xF000ul))

/*!< Parameter valid check for SDIOC command value. */
#define IS_VALID_SDIOC_CMD_VAL(x) (!(0xC0u & (x)))

/*!< Parameter valid check for buffer address. */
#define IS_VALID_TRANSFER_BUF_ALIGN(x) (!((SDIOC_BUF_ALIGN_SIZE - 1ul) & ((uint32_t)(x))))

/*!< Parameter valid check for SDIOC command value. */
#define IS_VALID_TRANSFER_BUF_LEN(x) (!((SDIOC_BUF_ALIGN_SIZE - 1ul) & ((uint32_t)(x))))

/*!< SDIOC unit max count value. */
#define SDIOC_UNIT_MAX_CNT (ARRAY_SZ(m_astcSdiocInstanceDataLut))

/*!< SDIOC default sdclk frequency. */
#define SDIOC_SDCLK_400K (400000ul)

/*!< Get the specified register address of the specified SDIOC unit */
#define SDIOC_ARG01(__SDIOCx__) ((uint32_t)(&((__SDIOCx__)->ARG0)))
#define SDIOC_BUF01(__SDIOCx__) ((uint32_t)(&((__SDIOCx__)->BUF0)))
#define SDIOC_RESPx(__SDIOCx__, RESP_REG) ((uint32_t)(&((__SDIOCx__)->RESP0)) + (uint32_t)(RESP_REG))

/* SDIOC buffer align size */
#define SDIOC_BUF_ALIGN_SIZE (4ul)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static en_sdioc_clk_div_t SdiocGetClkDiv(uint32_t u32Exclk,
                                         uint32_t u32SdiocClkFreq);
static stc_sdioc_intern_data_t *SdiocGetInternDataPtr(const M4_SDIOC_TypeDef *SDIOCx);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief Get SDIOC clock division.
 **
 ** \param [in] u32Exclk                Exclk frequency
 ** \param [in] u32ClkFreq              SDIOC clock frequency
 **
 ** \retval SdiocClkDiv_1               EXCLK/1
 ** \retval SdiocClkDiv_2               EXCLK/2
 ** \retval SdiocClkDiv_4               EXCLK/4
 ** \retval SdiocClkDiv_8               EXCLK/8
 ** \retval SdiocClkDiv_16              EXCLK/16
 ** \retval SdiocClkDiv_32              EXCLK/32
 ** \retval SdiocClkDiv_64              EXCLK/64
 ** \retval SdiocClkDiv_128             EXCLK/128
 ** \retval SdiocClkDiv_256             EXCLK/256
 **
 ******************************************************************************/
static en_sdioc_clk_div_t SdiocGetClkDiv(uint32_t u32Exclk,
                                         uint32_t u32ClkFreq)
{
    uint32_t u32SdClkDiv = 0ul;
    en_sdioc_clk_div_t enClockDiv = SdiocClkDiv_256;

    if (0ul != u32ClkFreq)
    {
        u32SdClkDiv = u32Exclk / u32ClkFreq;
        if (u32Exclk % u32ClkFreq)
        {
            u32SdClkDiv++;
        }

        if ((128ul < u32SdClkDiv) && (u32SdClkDiv <= 256ul))
        {
            enClockDiv = SdiocClkDiv_256;
        }
        else if ((64ul < u32SdClkDiv) && (u32SdClkDiv <= 128ul))
        {
            enClockDiv = SdiocClkDiv_128;
        }
        else if ((32ul < u32SdClkDiv) && (u32SdClkDiv <= 64ul))
        {
            enClockDiv = SdiocClkDiv_64;
        }
        else if ((16ul < u32SdClkDiv) && (u32SdClkDiv <= 32ul))
        {
            enClockDiv = SdiocClkDiv_32;
        }
        else if ((16ul < u32SdClkDiv) && (u32SdClkDiv <= 32ul))
        {
            enClockDiv = SdiocClkDiv_32;
        }
        else if ((8ul < u32SdClkDiv) && (u32SdClkDiv <= 16ul))
        {
            enClockDiv = SdiocClkDiv_16;
        }
        else if ((4ul < u32SdClkDiv) && (u32SdClkDiv <= 8ul))
        {
            enClockDiv = SdiocClkDiv_8;
        }
        else if ((2ul < u32SdClkDiv) && (u32SdClkDiv <= 4ul))
        {
            enClockDiv = SdiocClkDiv_4;
        }
        else if ((1ul < u32SdClkDiv) && (u32SdClkDiv <= 2ul))
        {
            enClockDiv = SdiocClkDiv_2;
        }
        else
        {
            enClockDiv = SdiocClkDiv_1;
        }
    }

    return enClockDiv;
}

/**
 *******************************************************************************
 ** \brief Return the internal data for a certain SDIOC instance.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_sdioc_intern_data_t *SdiocGetInternDataPtr(const M4_SDIOC_TypeDef *SDIOCx)
{
    uint8_t i;
    stc_sdioc_intern_data_t *pstcInternData = NULL;
    static stc_sdioc_instance_data_t m_astcSdiocInstanceDataLut[2];

    m_astcSdiocInstanceDataLut[0].SDIOCx = M4_SDIOC1;
    m_astcSdiocInstanceDataLut[1].SDIOCx = M4_SDIOC2;

    if (NULL != SDIOCx)
    {
        for (i = 0u; i < SDIOC_UNIT_MAX_CNT; i++)
        {
            if (SDIOCx == m_astcSdiocInstanceDataLut[i].SDIOCx)
            {
                pstcInternData = &m_astcSdiocInstanceDataLut[i].stcInternData;
                break;
            }
        }
    }

    return pstcInternData;
}

/**
 *******************************************************************************
 ** \brief SDIOC instance interrupt service routine
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval None
 **
 ******************************************************************************/
void SDIOC_IrqHandler(M4_SDIOC_TypeDef *SDIOCx)
{
    stc_sdioc_intern_data_t *pstcSdiocInternData = SdiocGetInternDataPtr(SDIOCx);

    /* Check for NULL pointer */
    if (NULL != pstcSdiocInternData)
    {
        /**************** Normal interrupt handler ****************/
        if (1u == SDIOCx->NORINTST_f.CC) /* Command complete */
        {
            SDIOCx->NORINTST_f.CC = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcNormalIrqCb.pfnCommandCompleteIrqCb)
            {
                pstcSdiocInternData->stcNormalIrqCb.pfnCommandCompleteIrqCb();
            }
        }

        if (1u == SDIOCx->NORINTST_f.TC) /* Transfer complete */
        {
            SDIOCx->NORINTST_f.TC = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcNormalIrqCb.pfnTransferCompleteIrqCb)
            {
                pstcSdiocInternData->stcNormalIrqCb.pfnTransferCompleteIrqCb();
            }
        }

        if (1u == SDIOCx->NORINTST_f.BGE) /* Block gap event */
        {
            SDIOCx->NORINTST_f.BGE = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcNormalIrqCb.pfnBlockGapIrqCb)
            {
                pstcSdiocInternData->stcNormalIrqCb.pfnBlockGapIrqCb();
            }
        }

        if (1u == SDIOCx->NORINTST_f.BWR) /* Buffer write ready */
        {
            SDIOCx->NORINTST_f.BWR = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcNormalIrqCb.pfnBufferWriteReadyIrqCb)
            {
                pstcSdiocInternData->stcNormalIrqCb.pfnBufferWriteReadyIrqCb();
            }
        }

        if (1u == SDIOCx->NORINTST_f.BRR) /* Buffer read ready */
        {
            SDIOCx->NORINTST_f.BRR = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcNormalIrqCb.pfnBufferReadReadyIrqCb)
            {
                pstcSdiocInternData->stcNormalIrqCb.pfnBufferReadReadyIrqCb();
            }
        }

        if (1u == SDIOCx->NORINTST_f.CIST) /* Card insertion */
        {
            SDIOCx->NORINTST_f.CIST = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcNormalIrqCb.pfnCardInsertIrqCb)
            {
                pstcSdiocInternData->stcNormalIrqCb.pfnCardInsertIrqCb();
            }
        }

        if (1u == SDIOCx->NORINTST_f.CRM) /* Card removal */
        {
            SDIOCx->NORINTST_f.CRM = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcNormalIrqCb.pfnCardRemovalIrqCb)
            {
                pstcSdiocInternData->stcNormalIrqCb.pfnCardRemovalIrqCb();
            }
        }

        if (1u == SDIOCx->NORINTST_f.CINT) /* Card interrupt */
        {
            SDIOCx->NORINTST_f.CINT = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcNormalIrqCb.pfnCardIrqCb)
            {
                pstcSdiocInternData->stcNormalIrqCb.pfnCardIrqCb();
            }
        }

        /**************** Error interrupt handler ****************/
        if (1u == SDIOCx->ERRINTST_f.CTOE) /* Command timeout error */
        {
            SDIOCx->ERRINTST_f.CTOE = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcErrorIrqCb.pfnCmdTimeoutErrIrqCb)
            {
                pstcSdiocInternData->stcErrorIrqCb.pfnCmdTimeoutErrIrqCb();
            }
        }

        if (1u == SDIOCx->ERRINTST_f.CCE) /* Command CRC error */
        {
            SDIOCx->ERRINTST_f.CCE = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcErrorIrqCb.pfnCmdCrcErrIrqCb)
            {
                pstcSdiocInternData->stcErrorIrqCb.pfnCmdCrcErrIrqCb();
            }
        }

        if (1u == SDIOCx->ERRINTST_f.CEBE) /* Command end bit error */
        {
            SDIOCx->ERRINTST_f.CEBE = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcErrorIrqCb.pfnCmdEndBitErrIrqCb)
            {
                pstcSdiocInternData->stcErrorIrqCb.pfnCmdEndBitErrIrqCb();
            }
        }

        if (1u == SDIOCx->ERRINTST_f.CIE) /* Command index error */
        {
            SDIOCx->ERRINTST_f.CIE = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcErrorIrqCb.pfnCmdIndexErrIrqCb)
            {
                pstcSdiocInternData->stcErrorIrqCb.pfnCmdIndexErrIrqCb();
            }
        }

        if (1u == SDIOCx->ERRINTST_f.DTOE) /* Data timeout error */
        {
            SDIOCx->ERRINTST_f.DTOE = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcErrorIrqCb.pfnDataTimeoutErrIrqCb)
            {
                pstcSdiocInternData->stcErrorIrqCb.pfnDataTimeoutErrIrqCb();
            }
        }

        if (1u == SDIOCx->ERRINTST_f.DEBE) /* Data end bit error */
        {
            SDIOCx->ERRINTST_f.DEBE = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcErrorIrqCb.pfnDataEndBitErrIrqCb)
            {
                pstcSdiocInternData->stcErrorIrqCb.pfnDataEndBitErrIrqCb();
            }
        }

        if (1u == SDIOCx->ERRINTST_f.DCE) /* Data CRC error */
        {
            SDIOCx->ERRINTST_f.DCE = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcErrorIrqCb.pfnDataCrcErrIrqCb)
            {
                pstcSdiocInternData->stcErrorIrqCb.pfnDataCrcErrIrqCb();
            }
        }

        if (1u == SDIOCx->ERRINTST_f.ACE) /* Auto CMD12 error */
        {
            SDIOCx->ERRINTST_f.ACE = 1u; /* Clear interrupt flag */
            if (NULL != pstcSdiocInternData->stcErrorIrqCb.pfnAutoCmdErrIrqCb)
            {
                pstcSdiocInternData->stcErrorIrqCb.pfnAutoCmdErrIrqCb();
            }
        }
    }
}

/**
 *******************************************************************************
 ** \brief Initializes a SDIOC.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pstcInitCfg             Pointer to SDIOC configure structure
 ** \arg This parameter detail refer @ref stc_sdioc_init_t
 **
 ** \retval Ok                          SDIOC initialized normally
 ** \retval ErrorTimeout                SDIOCx reset timeout
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SDIOCx is invalid
 **                                     - pstcInitCfg == NULL
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t SDIOC_Init(M4_SDIOC_TypeDef *SDIOCx,
                       const stc_sdioc_init_t *pstcInitCfg)
{
    __IO uint32_t i = 0ul;
    uint32_t u32Exclk = 0ul;
    uint32_t u32Cnt = SystemCoreClock / 100ul;
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_intern_data_t *pstcSdiocInternData = NULL;

    /* Get pointer to internal data structure. */
    pstcSdiocInternData = SdiocGetInternDataPtr(SDIOCx);
    if (NULL != pstcSdiocInternData) /* Check for instance available or not */
    {
        /* Reset all */
        SDIOCx->SFTRST_f.RSTA = 1u;
        while (0u != SDIOCx->SFTRST_f.RSTA) /* Wait until reset finish */
        {
            if (i++ > u32Cnt)
            {
                break;
            }
        }

        if (i < u32Cnt)
        {
            /* Get EXCLK frequency */
            u32Exclk = SystemCoreClock / (1ul << M4_SYSREG->CMU_SCFGR_f.EXCKS);

            SDIOCx->CLKCON_f.FS = SdiocGetClkDiv(u32Exclk, SdiocClk400K);
            SDIOCx->CLKCON_f.CE = (uint16_t)1u;
            SDIOCx->CLKCON_f.ICE = (uint16_t)1u;
            SDIOCx->PWRCON_f.PWON = (uint8_t)1u; /* Power on */

            /* Enable all status */
            SDIOCx->ERRINTST = (uint16_t)0x017Fu;   /* Clear Error interrupt status */
            SDIOCx->ERRINTSTEN = (uint16_t)0x017Fu; /* Enable Error interrupt status */
            SDIOCx->NORINTST = (uint16_t)0x00F7u;   /* Clear Normal interrupt status */
            SDIOCx->NORINTSTEN = (uint16_t)0x01F7u; /* Enable Normal interrupt status */

            /* Enable normal interrupt signal */
            if (NULL != pstcInitCfg)
            {
                if (NULL != pstcInitCfg->pstcNormalIrqEn)
                {
                    SDIOCx->NORINTSGEN = pstcInitCfg->pstcNormalIrqEn->u16NormalIntsgEn;
                }

                /* Set normal interrupt callback functions */
                if (NULL != pstcInitCfg->pstcNormalIrqCb)
                {
                    pstcSdiocInternData->stcNormalIrqCb.pfnCommandCompleteIrqCb = pstcInitCfg->pstcNormalIrqCb->pfnCommandCompleteIrqCb;
                    pstcSdiocInternData->stcNormalIrqCb.pfnTransferCompleteIrqCb = pstcInitCfg->pstcNormalIrqCb->pfnTransferCompleteIrqCb;
                    pstcSdiocInternData->stcNormalIrqCb.pfnBlockGapIrqCb = pstcInitCfg->pstcNormalIrqCb->pfnBlockGapIrqCb;
                    pstcSdiocInternData->stcNormalIrqCb.pfnBufferWriteReadyIrqCb = pstcInitCfg->pstcNormalIrqCb->pfnBufferWriteReadyIrqCb;
                    pstcSdiocInternData->stcNormalIrqCb.pfnBufferReadReadyIrqCb = pstcInitCfg->pstcNormalIrqCb->pfnBufferReadReadyIrqCb;
                    pstcSdiocInternData->stcNormalIrqCb.pfnCardInsertIrqCb = pstcInitCfg->pstcNormalIrqCb->pfnCardInsertIrqCb;
                    pstcSdiocInternData->stcNormalIrqCb.pfnCardRemovalIrqCb = pstcInitCfg->pstcNormalIrqCb->pfnCardRemovalIrqCb;
                    pstcSdiocInternData->stcNormalIrqCb.pfnCardIrqCb = pstcInitCfg->pstcNormalIrqCb->pfnCardIrqCb;
                }

                /* Enable error interrupt signal */
                if (NULL != pstcInitCfg->pstcErrorIrqEn)
                {
                    SDIOCx->ERRINTSGEN = pstcInitCfg->pstcErrorIrqEn->u16ErrorIntsgEn;
                }

                /* Set error interrupt callback functions */
                if (NULL != pstcInitCfg->pstcErrorIrqCb)
                {
                    pstcSdiocInternData->stcErrorIrqCb.pfnCmdTimeoutErrIrqCb = pstcInitCfg->pstcErrorIrqCb->pfnCmdTimeoutErrIrqCb;
                    pstcSdiocInternData->stcErrorIrqCb.pfnCmdCrcErrIrqCb = pstcInitCfg->pstcErrorIrqCb->pfnCmdCrcErrIrqCb;
                    pstcSdiocInternData->stcErrorIrqCb.pfnCmdEndBitErrIrqCb = pstcInitCfg->pstcErrorIrqCb->pfnCmdEndBitErrIrqCb;
                    pstcSdiocInternData->stcErrorIrqCb.pfnCmdIndexErrIrqCb = pstcInitCfg->pstcErrorIrqCb->pfnCmdIndexErrIrqCb;
                    pstcSdiocInternData->stcErrorIrqCb.pfnDataTimeoutErrIrqCb = pstcInitCfg->pstcErrorIrqCb->pfnDataTimeoutErrIrqCb;
                    pstcSdiocInternData->stcErrorIrqCb.pfnDataEndBitErrIrqCb = pstcInitCfg->pstcErrorIrqCb->pfnDataEndBitErrIrqCb;
                    pstcSdiocInternData->stcErrorIrqCb.pfnDataCrcErrIrqCb = pstcInitCfg->pstcErrorIrqCb->pfnDataCrcErrIrqCb;
                    pstcSdiocInternData->stcErrorIrqCb.pfnAutoCmdErrIrqCb = pstcInitCfg->pstcErrorIrqCb->pfnAutoCmdErrIrqCb;
                }
            }
            enRet = Ok;
        }
        else
        {
            enRet = ErrorTimeout;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Initializes the specified SDIOC unit.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval Ok                          De-Initialize successfully.
 ** \retval ErrorTimeout                SDIOCx reset timeout.
 ** \retval ErrorInvalidParameter       SDIOCx is invalid.
 **
 ******************************************************************************/
en_result_t SDIOC_DeInit(M4_SDIOC_TypeDef *SDIOCx)
{
    __IO uint32_t i = 0ul;
    uint32_t u32Cnt = SystemCoreClock / 100ul;
    en_result_t enRet = ErrorInvalidParameter;
    stc_sdioc_intern_data_t *pstcSdiocInternData = NULL;

    /* Get pointer to internal data structure. */
    pstcSdiocInternData = SdiocGetInternDataPtr(SDIOCx);
    if (NULL != pstcSdiocInternData) /* Check for instance available or not */
    {
        /* Reset all */
        SDIOCx->SFTRST_f.RSTA = 1u;
        while (0u != SDIOCx->SFTRST_f.RSTA) /* Wait until reset finish */
        {
            if (i++ > u32Cnt)
            {
                break;
            }
        }

        if (i < u32Cnt)
        {
            /* Set normal interrupt callback functions */
            pstcSdiocInternData->stcNormalIrqCb.pfnCommandCompleteIrqCb = NULL;
            pstcSdiocInternData->stcNormalIrqCb.pfnTransferCompleteIrqCb = NULL;
            pstcSdiocInternData->stcNormalIrqCb.pfnBlockGapIrqCb = NULL;
            pstcSdiocInternData->stcNormalIrqCb.pfnBufferWriteReadyIrqCb = NULL;
            pstcSdiocInternData->stcNormalIrqCb.pfnBufferReadReadyIrqCb = NULL;
            pstcSdiocInternData->stcNormalIrqCb.pfnCardInsertIrqCb = NULL;
            pstcSdiocInternData->stcNormalIrqCb.pfnCardRemovalIrqCb = NULL;
            pstcSdiocInternData->stcNormalIrqCb.pfnCardIrqCb = NULL;

            /* Set error interrupt callback functions */
            pstcSdiocInternData->stcErrorIrqCb.pfnCmdTimeoutErrIrqCb = NULL;
            pstcSdiocInternData->stcErrorIrqCb.pfnCmdCrcErrIrqCb = NULL;
            pstcSdiocInternData->stcErrorIrqCb.pfnCmdEndBitErrIrqCb = NULL;
            pstcSdiocInternData->stcErrorIrqCb.pfnCmdIndexErrIrqCb = NULL;
            pstcSdiocInternData->stcErrorIrqCb.pfnDataTimeoutErrIrqCb = NULL;
            pstcSdiocInternData->stcErrorIrqCb.pfnDataEndBitErrIrqCb = NULL;
            pstcSdiocInternData->stcErrorIrqCb.pfnDataCrcErrIrqCb = NULL;
            pstcSdiocInternData->stcErrorIrqCb.pfnAutoCmdErrIrqCb = NULL;
            enRet = Ok;
        }
        else
        {
            enRet = ErrorTimeout;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Send SD command
 **
 ** This function sends command on CMD line
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pstcCmdCfg              Pointer to command transfer configuration structure.
 ** \arg This parameter detail refer @ref stc_sdioc_cmd_cfg_t
 **
 ** \retval Ok                          Command sent normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SDIOCx is invalid
 **                                     - pstcCmdCfg == NULL
 **
 ******************************************************************************/
en_result_t SDIOC_SendCommand(M4_SDIOC_TypeDef *SDIOCx,
                              const stc_sdioc_cmd_cfg_t *pstcCmdCfg)
{
    uint32_t u32Addr;
    stc_sdioc_cmd_field_t stcCmdField;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for NULL pointer */
    if ((IS_VALID_SDIOC(SDIOCx)) && (NULL != pstcCmdCfg))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_CMD_VAL(pstcCmdCfg->u8CmdIndex));
        DDL_ASSERT(IS_VALID_SDIOC_CMD_TYPE(pstcCmdCfg->enCmdType));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcCmdCfg->enDataPresentEnable));
        DDL_ASSERT(IS_VALID_SDIOC_RESP_TYPE_NAME(pstcCmdCfg->enRspIndex));

        enRet = Ok;
        switch (pstcCmdCfg->enRspIndex)
        {
        case SdiocCmdNoRsp:
            stcCmdField.RESTYP = SdiocResponseNoneBit;
            stcCmdField.CCE = 0u;
            stcCmdField.ICE = 0u;
            break;
        case SdiocCmdRspR2:
            stcCmdField.RESTYP = SdiocResponse136Bit;
            stcCmdField.CCE = 1u;
            stcCmdField.ICE = 0u;
            break;
        case SdiocCmdRspR3:
        case SdiocCmdRspR4:
            stcCmdField.RESTYP = SdiocResponse48Bit;
            stcCmdField.CCE = 0u;
            stcCmdField.ICE = 0u;
            break;
        case SdiocCmdRspR1:
        case SdiocCmdRspR5:
        case SdiocCmdRspR6:
        case SdiocCmdRspR7:
            stcCmdField.RESTYP = SdiocResponse48Bit;
            stcCmdField.CCE = 1u;
            stcCmdField.ICE = 1u;
            break;
        case SdiocCmdRspR1b:
        case SdiocCmdRspR5b:
            stcCmdField.RESTYP = SdiocResponse48BitCheckBusy;
            stcCmdField.CCE = 1u;
            stcCmdField.ICE = 1u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }

        if (enRet == Ok)
        {
            stcCmdField.RESERVED2 = (uint16_t)0u;
            stcCmdField.TYP = (uint16_t)pstcCmdCfg->enCmdType;
            stcCmdField.IDX = (uint16_t)pstcCmdCfg->u8CmdIndex;
            stcCmdField.DAT = (uint16_t)(pstcCmdCfg->enDataPresentEnable);

            u32Addr = SDIOC_ARG01(SDIOCx);
            *(__IO uint32_t *)u32Addr = pstcCmdCfg->u32Argument;

            u32Addr = (uint32_t)&stcCmdField;
            SDIOCx->CMD = *(uint16_t *)u32Addr;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the response received from the card for the last command
 **
 ** This function sends command on CMD line
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enRespReg               Response Specifies the SDIOC response register.
 ** \arg SdiocRegResp01                 Response0 and Response1 Register
 ** \arg SdiocRegResp23                 Response2 and Response3 Register
 ** \arg SdiocRegResp45                 Response4 and Response5 Register
 ** \arg SdiocRegResp67                 Response6 and Response7 Register
 **
 ** \retval The Corresponding response register value
 **
 ******************************************************************************/
uint32_t SDIOC_GetResponse(const M4_SDIOC_TypeDef *SDIOCx,
                           en_sdioc_response_reg_t enRespReg)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));
    DDL_ASSERT(IS_VALID_SDIOC_RESP(enRespReg));

    return *(__IO uint32_t *)SDIOC_RESPx(SDIOCx, enRespReg);
}

/**
 *******************************************************************************
 ** \brief Read data from SDIOCx data buffer
 **
 ** This function reads 32-bit data from data buffer
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] au8Data                 Buffer which will store SDIOC_BUFFER data
 ** \param [in] u32Len                  Data length
 **
 ** \retval Ok                          Data is read normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SDIOCx is invalid
 **                                     - pu32Data == NULL
 **
 ******************************************************************************/
en_result_t SDIOC_ReadBuffer(M4_SDIOC_TypeDef *SDIOCx,
                             uint8_t au8Data[],
                             uint32_t u32Len)
{
    uint32_t i = 0ul;
    uint32_t u32Temp = 0ul;
    ;
    __IO uint32_t *SDIO_BUF_REG = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx && pu8Data pointer */
    if ((NULL != au8Data) &&
        (IS_VALID_SDIOC(SDIOCx)) &&
        (IS_VALID_TRANSFER_BUF_LEN(u32Len)))
    {
        SDIO_BUF_REG = (__IO uint32_t *)SDIOC_BUF01(SDIOCx);

        while (i < u32Len)
        {
            u32Temp = *SDIO_BUF_REG;
            au8Data[i++] = (uint8_t)((u32Temp >> 0ul) & 0x000000FF);
            au8Data[i++] = (uint8_t)((u32Temp >> 8ul) & 0x000000FF);
            au8Data[i++] = (uint8_t)((u32Temp >> 16ul) & 0x000000FF);
            au8Data[i++] = (uint8_t)((u32Temp >> 24ul) & 0x000000FF);
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Write data to SDIOCx data buffer
 **
 ** This function writes 32-bit data to data buffer
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] au8Data                 Buffer which will be wrote to SDIOC_BUFFER
 ** \param [in] u32Len                  Data length
 **
 ** \retval Ok                          Data is written normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SDIOCx is invalid
 **                                     - pu8Data == NULL
 **
 ******************************************************************************/
en_result_t SDIOC_WriteBuffer(M4_SDIOC_TypeDef *SDIOCx,
                              uint8_t au8Data[],
                              uint32_t u32Len)
{
    uint32_t i = 0ul;
    uint32_t u32Temp = 0ul;
    __IO uint32_t *SDIO_BUF_REG = NULL;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx && pu8Data pointer */
    if ((NULL != au8Data) &&
        (IS_VALID_SDIOC(SDIOCx)) &&
        (IS_VALID_TRANSFER_BUF_LEN(u32Len)))
    {
        SDIO_BUF_REG = (__IO uint32_t *)SDIOC_BUF01(SDIOCx);

        while (i < u32Len)
        {
            u32Temp = (((uint32_t)au8Data[i++]) << 0ul) & 0x000000FFul;
            u32Temp += (((uint32_t)au8Data[i++]) << 8ul) & 0x0000FF00ul;
            u32Temp += (((uint32_t)au8Data[i++]) << 16ul) & 0x00FF0000ul;
            u32Temp += (((uint32_t)au8Data[i++]) << 24ul) & 0xFF000000ul;

            *SDIO_BUF_REG = u32Temp;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Configure SDIOCx data parameters
 **
 ** This function writes 32-bit data to data buffer
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] pstcDataCfg             Pointer to SDIOC data transfer configuration structure
 ** \arg This parameter detail refer @ref stc_sdioc_data_cfg_t
 **
 ** \retval Ok                          configure normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SDIOCx is invalid
 **                                     - pstcDataCfg == NULL
 **
 ******************************************************************************/
en_result_t SDIOC_ConfigData(M4_SDIOC_TypeDef *SDIOCx,
                             const stc_sdioc_data_cfg_t *pstcDataCfg)
{
    uint16_t u16BlkCnt = (uint16_t)0;
    uint32_t u32Addr;
    stc_sdioc_transmode_field_t stcTransModeField = {0};
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx && pstcDataCfg pointer */
    if ((IS_VALID_SDIOC(SDIOCx)) && (NULL != pstcDataCfg))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_BLKCNT(pstcDataCfg->u16BlkCnt));
        DDL_ASSERT(IS_VALID_SDIOC_BLKSIZE(pstcDataCfg->u16BlkSize));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcDataCfg->enAutoCmd12Enable));
        DDL_ASSERT(IS_VALID_SDIOC_DATA_TIMEOUT(pstcDataCfg->enDataTimeOut));
        DDL_ASSERT(IS_VALID_SDIOC_TRANSFER_DIR(pstcDataCfg->enTransferDir));
        DDL_ASSERT(IS_VALID_SDIOC_TRANSFER_MODE(pstcDataCfg->enTransferMode));

        enRet = Ok;

        switch (pstcDataCfg->enTransferMode)
        {
        case SdiocTransferSingle:
            stcTransModeField.MULB = 0u;
            stcTransModeField.BCE = 0u;
            break;
        case SdiocTransferInfinite:
            stcTransModeField.MULB = 1u;
            stcTransModeField.BCE = 0u;
            break;
        case SdiocTransferMultiple:
            u16BlkCnt = pstcDataCfg->u16BlkCnt;
            stcTransModeField.MULB = 1u;
            stcTransModeField.BCE = 1u;
            break;
        case SdiocTransferStopMultiple:
            stcTransModeField.MULB = 1u;
            stcTransModeField.BCE = 1u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }

        if (enRet == Ok)
        {
            stcTransModeField.RESERVED0 = (uint16_t)0u;
            stcTransModeField.DDIR = (uint16_t)(pstcDataCfg->enTransferDir);
            stcTransModeField.ATCEN = (uint16_t)(pstcDataCfg->enAutoCmd12Enable);

            /* Set the SDIOC Data Transfer Timeout value */
            SDIOCx->TOUTCON = (uint8_t)(pstcDataCfg->enDataTimeOut);
            /* Set the SDIOC Block Count value */
            SDIOCx->BLKCNT = u16BlkCnt;
            /* Set the SDIOC Block Size value */
            SDIOCx->BLKSIZE = pstcDataCfg->u16BlkSize;
            /* Set the SDIOC Data Transfer Mode */
            u32Addr = (uint32_t)&stcTransModeField;
            SDIOCx->TRANSMODE = *(uint16_t *)u32Addr;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable/Disable SDCLK output
 **
 ** SD host drives SDCLK line.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enCmd                   The SDCLK functional state command
 ** \arg Enable                         Enable SDCLK function
 ** \arg Disable                        Disable SDCLK function
 **
 ** \retval Ok                          SDCLK output of SDIOCx enabled normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_SdclkCmd(M4_SDIOC_TypeDef *SDIOCx,
                           en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        SDIOCx->CLKCON_f.CE = (uint16_t)(enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set the clock division of SD clock
 **
 ** This function changes the SD clock division.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enClkDiv                SDIOC clock division value
 ** \arg SdiocClkDiv_1                  EXCLK/1
 ** \arg SdiocClkDiv_2                  EXCLK/2
 ** \arg SdiocClkDiv_4                  EXCLK/4
 ** \arg SdiocClkDiv_8                  EXCLK/8
 ** \arg SdiocClkDiv_16                 EXCLK/16
 ** \arg SdiocClkDiv_32                 EXCLK/32
 ** \arg SdiocClkDiv_64                 EXCLK/64
 ** \arg SdiocClkDiv_128                EXCLK/128
 ** \arg SdiocClkDiv_256                EXCLK/256
 **
 ** \retval Ok                          SDIOC clock division is changed normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_SetClkDiv(M4_SDIOC_TypeDef *SDIOCx,
                            en_sdioc_clk_div_t enClkDiv)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_CLK_DIV(enClkDiv));

        /* Set clock division */
        SDIOCx->CLKCON_f.FS = (uint16_t)enClkDiv;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the clock division of SD clock
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval SdiocClkDiv_1               EXCLK/1
 ** \retval SdiocClkDiv_2               EXCLK/2
 ** \retval SdiocClkDiv_4               EXCLK/4
 ** \retval SdiocClkDiv_8               EXCLK/8
 ** \retval SdiocClkDiv_16              EXCLK/16
 ** \retval SdiocClkDiv_32              EXCLK/32
 ** \retval SdiocClkDiv_64              EXCLK/64
 ** \retval SdiocClkDiv_128             EXCLK/128
 **
 ******************************************************************************/
en_sdioc_clk_div_t SDIOC_GetClkDiv(M4_SDIOC_TypeDef *SDIOCx)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));

    return ((en_sdioc_clk_div_t)SDIOCx->CLKCON_f.FS);
}

/**
 *******************************************************************************
 ** \brief Get the clock division of SD clock
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] u32ClkFreq              SDIOC clock frequency
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_SetClk(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32ClkFreq)
{
    uint32_t u32Exclk = 0ul;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Get EXCLK frequency */
        u32Exclk = SystemCoreClock / (1ul << M4_SYSREG->CMU_SCFGR_f.EXCKS);

        SDIOCx->CLKCON_f.CE = (uint16_t)0u;
        SDIOCx->CLKCON_f.FS = (uint16_t)SdiocGetClkDiv(u32Exclk, u32ClkFreq);
        SDIOCx->CLKCON_f.CE = (uint16_t)1u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set the bus width of SD Bus
 **
 ** This function changes the SD bus width.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enBusWidth              Bus width
 ** \arg SdiocBusWidth1Bit              The SDIOC bus width 1 bit
 ** \arg SdiocBusWidth4Bit              The SDIOC bus width 4 bit
 ** \arg SdiocBusWidth8Bit              The SDIOC bus width 8 bit
 **
 ** \retval Ok                          Bus width is set normally
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - SDIOCx is invalid
 **                                     - enBusWidth is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_SetBusWidth(M4_SDIOC_TypeDef *SDIOCx,
                              en_sdioc_bus_width_t enBusWidth)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_BUS_WIDTH(enBusWidth));

        enRet = Ok;

        switch (enBusWidth)
        {
        case SdiocBusWidth1Bit:
            SDIOCx->HOSTCON_f.EXDW = 0u;
            SDIOCx->HOSTCON_f.DW = 0u;
            break;
        case SdiocBusWidth4Bit:
            SDIOCx->HOSTCON_f.EXDW = 0u;
            SDIOCx->HOSTCON_f.DW = 1u;
            break;
        case SdiocBusWidth8Bit:
            SDIOCx->HOSTCON_f.EXDW = 1u;
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the bus width of SD Bus
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval SdiocBusWidth1Bit           The SDIOC bus width 1 bit
 ** \retval SdiocBusWidth4Bit           The SDIOC bus width 4 bit
 ** \retval SdiocBusWidth8Bit           The SDIOC bus width 8 bit
 **
 ******************************************************************************/
en_sdioc_bus_width_t SDIOC_GetBusWidth(M4_SDIOC_TypeDef *SDIOCx)
{
    en_sdioc_bus_width_t enBusWidth = SdiocBusWidth4Bit;

    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));

    if (0u == SDIOCx->HOSTCON_f.EXDW)
    {
        if (0u == SDIOCx->HOSTCON_f.DW)
        {
            enBusWidth = SdiocBusWidth1Bit;
        }
    }
    else
    {
        enBusWidth = SdiocBusWidth8Bit;
    }

    return enBusWidth;
}

/**
 *******************************************************************************
 ** \brief Set the bus speed mode of SD Bus
 **
 ** This function changes the SD bus speed mode.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enSpeedMode             Speed mode
 ** \arg SdiocHighSpeedMode             High speed mode
 ** \arg SdiocNormalSpeedMode           Normal speed mode
 **
 ** \retval Ok                          Bus speed is set normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_SetSpeedMode(M4_SDIOC_TypeDef *SDIOCx,
                               en_sdioc_speed_mode_t enSpeedMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_SPEED_MODE(enSpeedMode));

        /* Set high speed mode */
        SDIOCx->HOSTCON_f.HSEN = ((SdiocHighSpeedMode == enSpeedMode) ? 1u : 0u);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the bus speed mode of SD Bus
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval SdiocHighSpeedMode          High speed mode
 ** \retval SdiocNormalSpeedMode        Normal speed mode
 **
 ******************************************************************************/
en_sdioc_speed_mode_t SDIOC_GetSpeedMode(M4_SDIOC_TypeDef *SDIOCx)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));

    return ((SDIOCx->HOSTCON_f.HSEN) ? SdiocHighSpeedMode : SdiocNormalSpeedMode);
}

/**
 *******************************************************************************
 ** \brief Set data timeout counter value
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enTimeout               Data timeout count value
 ** \arg SdiocDtoSdclk_2_13             Timeout time: SDCLK*2^13
 ** \arg SdiocDtoSdclk_2_14             Timeout time: SDCLK*2^14
 ** \arg SdiocDtoSdclk_2_15             Timeout time: SDCLK*2^15
 ** \arg SdiocDtoSdclk_2_16             Timeout time: SDCLK*2^16
 ** \arg SdiocDtoSdclk_2_17             Timeout time: SDCLK*2^17
 ** \arg SdiocDtoSdclk_2_18             Timeout time: SDCLK*2^18
 ** \arg SdiocDtoSdclk_2_19             Timeout time: SDCLK*2^19
 ** \arg SdiocDtoSdclk_2_20             Timeout time: SDCLK*2^20
 ** \arg SdiocDtoSdclk_2_21             Timeout time: SDCLK*2^21
 ** \arg SdiocDtoSdclk_2_22             Timeout time: SDCLK*2^22
 ** \arg SdiocDtoSdclk_2_23             Timeout time: SDCLK*2^23
 ** \arg SdiocDtoSdclk_2_24             Timeout time: SDCLK*2^24
 ** \arg SdiocDtoSdclk_2_25             Timeout time: SDCLK*2^25
 ** \arg SdiocDtoSdclk_2_26             Timeout time: SDCLK*2^26
 ** \arg SdiocDtoSdclk_2_27             Timeout time: SDCLK*2^27
 **
 ** \retval Ok                          Bus speed is set normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_SetDataTimeout(M4_SDIOC_TypeDef *SDIOCx,
                                 en_sdioc_data_timeout_t enTimeout)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_DATA_TIMEOUT(enTimeout));

        /* Set data timeout */
        SDIOCx->TOUTCON_f.DTO = (uint8_t)enTimeout;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get data timeout counter value
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval SdiocDtoSdclk_2_13          Timeout time: SDCLK*2^13
 ** \retval SdiocDtoSdclk_2_14          Timeout time: SDCLK*2^14
 ** \retval SdiocDtoSdclk_2_15          Timeout time: SDCLK*2^15
 ** \retval SdiocDtoSdclk_2_16          Timeout time: SDCLK*2^16
 ** \retval SdiocDtoSdclk_2_17          Timeout time: SDCLK*2^17
 ** \retval SdiocDtoSdclk_2_18          Timeout time: SDCLK*2^18
 ** \retval SdiocDtoSdclk_2_19          Timeout time: SDCLK*2^19
 ** \retval SdiocDtoSdclk_2_20          Timeout time: SDCLK*2^20
 ** \retval SdiocDtoSdclk_2_21          Timeout time: SDCLK*2^21
 ** \retval SdiocDtoSdclk_2_22          Timeout time: SDCLK*2^22
 ** \retval SdiocDtoSdclk_2_23          Timeout time: SDCLK*2^23
 ** \retval SdiocDtoSdclk_2_24          Timeout time: SDCLK*2^24
 ** \retval SdiocDtoSdclk_2_25          Timeout time: SDCLK*2^25
 ** \retval SdiocDtoSdclk_2_26          Timeout time: SDCLK*2^26
 ** \retval SdiocDtoSdclk_2_27          Timeout time: SDCLK*2^27
 **
 ******************************************************************************/
en_sdioc_data_timeout_t SDIOC_GetDataTimeout(M4_SDIOC_TypeDef *SDIOCx)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));

    return (en_sdioc_data_timeout_t)(SDIOCx->TOUTCON_f.DTO);
}

/**
 *******************************************************************************
 ** \brief Set the card detect signal
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enDetectSignal          Card detect signal
 ** \arg SdiocSdcdPinLevel              SDCD# is selected (for normal use)
 ** \arg SdiocCardDetectTestLevel       The Card Detect Test Level is selected(for test purpose)
 **
 ** \retval Ok                          Set normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_SetCardDetectSignal(M4_SDIOC_TypeDef *SDIOCx,
                                      en_sdioc_detect_signal_t enDetectSignal)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_DETECT_SIG(enDetectSignal));
        SDIOCx->HOSTCON_f.CDSS = (uint8_t)enDetectSignal;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get card inserted or not.
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval Set                         Card Inserted
 ** \retval Reset                       No Card
 **
 ** \note This bit is enabled while the Card Detect Signal Selection is set to 1
 **       and it indicates card inserted or not.
 **
 ******************************************************************************/
en_flag_status_t SDIOC_GetCardDetectTestLevel(M4_SDIOC_TypeDef *SDIOCx)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));

    return (en_flag_status_t)(SDIOCx->HOSTCON_f.CDTL);
}

/**
 *******************************************************************************
 ** \brief Power on SD bus power
 **
 ** This function starts power supply on SD bus
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval Ok                          Power on normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_BusPowerOn(M4_SDIOC_TypeDef *SDIOCx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        SDIOCx->PWRCON_f.PWON = 1u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Power off SD bus power
 **
 ** This function stops power supply on SD bus
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval Ok                          Power off normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_BusPowerOff(M4_SDIOC_TypeDef *SDIOCx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        SDIOCx->PWRCON_f.PWON = 0u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable/Disable the function of Stop At Block Gap Request during block gap
 **
 ** This function is used to stop data trasnfer of multi-block transfer
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enCmd                   SDIOC Stop At Block Gap Request functional state
 ** \arg Enable                         Enable the function of Stop At Block Gap Request
 ** \arg Disable                        Disable the function of Stop At Block Gap Request
 **
 ** \retval Ok                          Set successfully
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_StopAtBlockGapCmd(M4_SDIOC_TypeDef *SDIOCx,
                                    en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));
        SDIOCx->BLKGPCON_f.SABGR = (uint8_t)(enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Restart data transfer
 **
 ** This function is used to restart data transfer when transfer is pending
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 **
 ** \retval Ok                          Set successfully
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_RestartTransfer(M4_SDIOC_TypeDef *SDIOCx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        SDIOCx->BLKGPCON_f.CR = 1u;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable/Disable the function of Read Wait Control
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enCmd                   SDIOC Read Wait Control functional state
 ** \arg Enable                         Enable the Read Wait Control function
 ** \arg Disable                        Disable the Read Wait Control function
 **
 ** \retval Ok                          Set successfully
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_ReadWaitCmd(M4_SDIOC_TypeDef *SDIOCx,
                              en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));
        SDIOCx->BLKGPCON_f.RWC = (uint8_t)(enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable/Disable the function of Interrupt At Block Gap
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enCmd                   SDIOC Interrupt At Block Gap functional state
 ** \arg Enable                         Enable the function of Interrupt At Block Gap
 ** \arg Disable                        Disable the function of Interrupt At Block Gap
 **
 ** \retval Ok                          Set successfully
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_InterruptAtBlockGapCmd(M4_SDIOC_TypeDef *SDIOCx,
                                         en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));
        SDIOCx->BLKGPCON_f.IABG = (uint8_t)(enCmd);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Generate software reset to SD card
 **
 ** This function generates software reset all command to SD card
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enSwResetType           Software reset type
 ** \arg SdiocSwResetAll                This reset affects the entire Host Controller except for the card detection circuit.
 ** \arg SdiocSwResetCmdLine            Only part of command circuit is reset.
 ** \arg SdiocSwResetDataLine           Only part of data circuit is reset.
 **
 ** \retval Ok                          Software reset is done normally
 ** \retval ErrorTimeout                SDIOCx reset timeout
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - SDIOCx is invalid
 **                                     - enSwResetType is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_SoftwareReset(M4_SDIOC_TypeDef *SDIOCx,
                                en_sdioc_sw_reset_t enSwResetType)
{
    __IO uint32_t i = 0ul;
    uint32_t u32Cnt = SystemCoreClock / 100ul;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_SWRESETTYPE(enSwResetType));

        enRet = Ok;
        switch (enSwResetType)
        {
        case SdiocSwResetAll:
            SDIOCx->SFTRST_f.RSTA = (uint8_t)1u;
            while (0u != SDIOCx->SFTRST_f.RSTA) /* Wait until reset finish */
            {
                if (i++ > u32Cnt)
                {
                    break;
                }
            }
            break;
        case SdiocSwResetCmdLine:
            SDIOCx->SFTRST_f.RSTC = (uint8_t)1u;
            while (0u != SDIOCx->SFTRST_f.RSTC) /* Wait until reset finish */
            {
                if (i++ > u32Cnt)
                {
                    break;
                }
            }
            break;
        case SdiocSwResetDatLine:
            SDIOCx->SFTRST_f.RSTD = (uint8_t)1u;
            while (0u != SDIOCx->SFTRST_f.RSTD) /* Wait until reset finish */
            {
                if (i++ > u32Cnt)
                {
                    break;
                }
            }
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }
    }

    if (i > u32Cnt)
    {
        enRet = ErrorTimeout;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the status of SDIOC host controller
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enHostStatus            SDIOC host status
 ** \arg SdiocCommandInhibitCmd         Command Inhibit(CMD). 1: Cannot issue command; 0:Can issue command using only CMD line
 ** \arg SdiocCommandInhibitData        Command Inhibit(DAT). 1: Cannot issue command which uses the DAT line; 0:Can issue command which uses the DAT line
 ** \arg SdiocDataLineActive            1: DAT Line Active; 0: DAT Line Inactive
 ** \arg SdiocWriteTransferActive       Write Transfer Active.1: Transferring data; 0: No valid data
 ** \arg SdiocReadTransferActive        Read Transfer Active.1: Transferring data; 0: No valid data
 ** \arg SdiocBufferWriteEnble          1: Write enable; 0: Write Disable
 ** \arg SdiocBufferReadEnble           1: Read enable; 0: Read Disable
 ** \arg SdiocCardInserted              1: Card Inserted; 0: Reset or Debouncing or No Card
 ** \arg SdiocCardStateStable           1: No Card or Inserted; 0: Reset or Debouncing
 ** \arg SdiocCardDetectPinLvl          1: Card present; 0: No card present
 ** \arg SdiocWriteProtectPinLvl        1: Write enabled; 0: Write protected
 ** \arg SdiocData0PinLvl               1: DAT0 line signal level high; 0: DAT0 line signal level low
 ** \arg SdiocData1PinLvl               1: DAT1 line signal level high; 0: DAT1 line signal level low
 ** \arg SdiocData2PinLvl               1: DAT2 line signal level high; 0: DAT2 line signal level low
 ** \arg SdiocData3PinLvl               1: DAT3 line signal level high; 0: DAT3 line signal level low
 ** \arg SdiocCmdPinLvl                 1: CMD line signal level high;  0: CMD line signal level low
 **
 ** \retval Set                         The specified status is set
 ** \retval Reset                       The specified status is zero
 **
 ******************************************************************************/
en_flag_status_t SDIOC_GetStatus(M4_SDIOC_TypeDef *SDIOCx,
                                 en_sdioc_host_status_t enHostStatus)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));
    DDL_ASSERT(IS_VALID_SDIOC_HOST_STATUS(enHostStatus));

    return ((SDIOCx->PSTAT & ((uint32_t)enHostStatus)) ? Set : Reset);
}

/**
 *******************************************************************************
 ** \brief Enable/Disable the specified signal of SDIOC normal interrupt
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enNorInt                SDIOC normal interrupt
 ** \arg SdiocCommandComplete           Command Complete. 1: Command complete; 0:No command complete
 ** \arg SdiocTransferComplete          Transfer Complete. 1: Data transfer complete; 0:No transfer complete
 ** \arg SdiocBlockGapEvent             Block Gap Event. 1: Transaction stopped at block gap; 0: No Block Gap Event
 ** \arg SdiocBufferWriteReady          Buffer Write Ready. 1: Ready to Write buffer; 0: No ready to Write buffer
 ** \arg SdiocBufferReadReady           Buffer Read Ready. 1: Ready to read buffer; 0: No ready to read buffer
 ** \arg SdiocCardInsertedInt           Write Transfer Active.1: Transferring data; 0: No valid data
 ** \arg SdiocCardRemoval               Card Removal. 1: Card removed; 0: Card state stable or Debouncing
 ** \arg SdiocCardInt                   Card Interrupt. 1: Generate Card Interrupt; 0: No Card Interrupt
 ** \arg SdiocErrorInt                  Error Interrupt. 1: Error; 0: No Error
 ** \param [in] enCmd                   SDIOC normal interrupt signal functional state
 ** \arg Enable                         Enable the specified signal of SD normal interrupt
 ** \arg Disable                        Disable the specified signal of SD normal interrupt
 **
 ** \retval Ok                          Set normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_NormalIrqSignalCmd(M4_SDIOC_TypeDef *SDIOCx,
                                     en_sdioc_nor_int_sel_t enNorInt,
                                     en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));
        DDL_ASSERT(IS_VALID_SDIOC_NOR_INT(enNorInt));

        if (Enable == enCmd)
        {
            SDIOCx->NORINTSGEN |= (uint16_t)enNorInt;
        }
        else
        {
            SDIOCx->NORINTSGEN &= (uint16_t)(~((uint16_t)enNorInt));
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable/Disable the status of SDIOC normal interrupt
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enNorInt                SDIOC normal interrupt
 ** \arg SdiocCommandComplete           Command Complete. 1: Command complete; 0:No command complete
 ** \arg SdiocTransferComplete          Transfer Complete. 1: Data transfer complete; 0:No transfer complete
 ** \arg SdiocBlockGapEvent             Block Gap Event. 1: Transaction stopped at block gap; 0: No Block Gap Event
 ** \arg SdiocBufferWriteReady          Buffer Write Ready. 1: Ready to Write buffer; 0: No ready to Write buffer
 ** \arg SdiocBufferReadReady           Buffer Read Ready. 1: Ready to read buffer; 0: No ready to read buffer
 ** \arg SdiocCardInsertedInt           Write Transfer Active.1: Transferring data; 0: No valid data
 ** \arg SdiocCardRemoval               Card Removal. 1: Card removed; 0: Card state stable or Debouncing
 ** \arg SdiocCardInt                   Card Interrupt. 1: Generate Card Interrupt; 0: No Card Interrupt
 ** \arg SdiocErrorInt                  Error Interrupt. 1: Error; 0: No Error
 ** \param [in] enCmd                   SDIOC normal interrupt status functional state
 ** \arg Enable                         Enable the specified status of SD normal interrupt
 ** \arg Disable                        Disable the specified status of SD normal interrupt
 **
 ** \retval Ok                          Set normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_NormalIrqStatusCmd(M4_SDIOC_TypeDef *SDIOCx,
                                     en_sdioc_nor_int_sel_t enNorInt,
                                     en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));
        DDL_ASSERT(IS_VALID_SDIOC_NOR_INT(enNorInt));

        if (Enable == enCmd)
        {
            SDIOCx->NORINTSTEN |= (uint16_t)enNorInt;
        }
        else
        {
            SDIOCx->NORINTSTEN &= (uint16_t)(~((uint16_t)enNorInt));
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the flag of SD normal interrupt
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enNorInt                SDIOC normal interrupt
 ** \arg SdiocCommandComplete           Command Complete. 1: Command complete; 0:No command complete
 ** \arg SdiocTransferComplete          Transfer Complete. 1: Data transfer complete; 0:No transfer complete
 ** \arg SdiocBlockGapEvent             Block Gap Event. 1: Transaction stopped at block gap; 0: No Block Gap Event
 ** \arg SdiocBufferWriteReady          Buffer Write Ready. 1: Ready to Write buffer; 0: No ready to Write buffer
 ** \arg SdiocBufferReadReady           Buffer Read Ready. 1: Ready to read buffer; 0: No ready to read buffer
 ** \arg SdiocCardInsertedInt           Write Transfer Active.1: Transferring data; 0: No valid data
 ** \arg SdiocCardRemoval               Card Removal. 1: Card removed; 0: Card state stable or Debouncing
 ** \arg SdiocCardInt                   Card Interrupt. 1: Generate Card Interrupt; 0: No Card Interrupt
 ** \arg SdiocErrorInt                  Error Interrupt. 1: Error; 0: No Error
 **
 ** \retval Set                         The specified interupt flag is set
 ** \retval Reset                       The specified interupt flag is zero
 **
 ******************************************************************************/
en_flag_status_t SDIOC_GetNormalIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                        en_sdioc_nor_int_flag_t enNorInt)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));
    DDL_ASSERT(IS_VALID_SDIOC_NOR_INT(enNorInt));

    return ((SDIOCx->NORINTST & ((uint16_t)enNorInt)) ? Set : Reset);
}

/**
 *******************************************************************************
 ** \brief Clear the flag of SD normal interrupt
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enNorInt                SDIOC normal interrupt
 ** \arg SdiocCommandComplete           Command Complete. 1: Command complete; 0:No command complete
 ** \arg SdiocTransferComplete          Transfer Complete. 1: Data transfer complete; 0:No transfer complete
 ** \arg SdiocBlockGapEvent             Block Gap Event. 1: Transaction stopped at block gap; 0: No Block Gap Event
 ** \arg SdiocBufferWriteReady          Buffer Write Ready. 1: Ready to Write buffer; 0: No ready to Write buffer
 ** \arg SdiocBufferReadReady           Buffer Read Ready. 1: Ready to read buffer; 0: No ready to read buffer
 ** \arg SdiocCardInsertedInt           Write Transfer Active.1: Transferring data; 0: No valid data
 ** \arg SdiocCardRemoval               Card Removal. 1: Card removed; 0: Card state stable or Debouncing
 ** \arg SdiocCardInt                   Card Interrupt. 1: Generate Card Interrupt; 0: No Card Interrupt
 ** \arg SdiocErrorInt                  Error Interrupt. 1: Error; 0: No Error
 **
 ** \retval Ok                          Clear successfully.
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_ClearNormalIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                     en_sdioc_nor_int_flag_t enNorInt)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_NOR_INT(enNorInt));
        SDIOCx->NORINTST = (uint16_t)enNorInt;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable/Disable the signal of SD error interrupt
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enErrInt                SDIOC error interrupt
 ** \arg SdiocCmdTimeoutErr             Command Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocCmdCrcErr                 Command CRC Error. 1: Command CRC Error Generated; 0:No Error
 ** \arg SdiocCmdEndBitErr              Command End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocCmdIndexErr               Command Index Error. 1: Command Index Error Generatedr; 0:No Error
 ** \arg SdiocDataTimeoutErr            Data Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocDataCrcErr                Data CRC Error. 1: Data CRC Error Generated; 0:No Error
 ** \arg SdiocDataEndBitErr             Data End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocAutoCmd12Err              Auto CMD12 Error. 1: Error; 0:No Error
 ** \param [in] enCmd                   SDIOC error interrupt signal functional state
 ** \arg Enable                         Enable the specified signal of SD error interrupt
 ** \arg Disable                        Disable the specified signal of SD error interrupt
 **
 ** \retval Ok                          Set normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_ErrIrqSignalCmd(M4_SDIOC_TypeDef *SDIOCx,
                                  en_sdioc_err_int_sel_t enErrInt,
                                  en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_ERR_INT(enErrInt));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        if (Enable == enCmd)
        {
            SDIOCx->ERRINTSGEN |= (uint16_t)enErrInt;
        }
        else
        {
            SDIOCx->ERRINTSGEN &= (uint16_t)enErrInt;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable/Disable the status of SD error interrupt
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enErrInt                SDIOC error interrupt
 ** \arg SdiocCmdTimeoutErr             Command Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocCmdCrcErr                 Command CRC Error. 1: Command CRC Error Generated; 0:No Error
 ** \arg SdiocCmdEndBitErr              Command End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocCmdIndexErr               Command Index Error. 1: Command Index Error Generatedr; 0:No Error
 ** \arg SdiocDataTimeoutErr            Data Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocDataCrcErr                Data CRC Error. 1: Data CRC Error Generated; 0:No Error
 ** \arg SdiocDataEndBitErr             Data End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocAutoCmd12Err              Auto CMD12 Error. 1: Error; 0:No Error
 ** \param [in] enCmd                   SDIOC error interrupt status functional state
 ** \arg Enable                         Enable the specified status of SD error interrupt
 ** \arg Disable                        Disable the specified status of SD error interrupt
 **
 ** \retval Ok                          Set normally
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_ErrIrqStatusCmd(M4_SDIOC_TypeDef *SDIOCx,
                                  en_sdioc_err_int_sel_t enErrInt,
                                  en_functional_state_t enCmd)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_ERR_INT(enErrInt));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enCmd));

        if (Enable == enCmd)
        {
            SDIOCx->ERRINTSTEN |= (uint16_t)enErrInt;
        }
        else
        {
            SDIOCx->ERRINTSTEN &= (uint16_t)enErrInt;
        }

        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the flag of SD error interrupt
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enErrInt                SDIOC error interrupt
 ** \arg SdiocCmdTimeoutErr             Command Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocCmdCrcErr                 Command CRC Error. 1: Command CRC Error Generated; 0:No Error
 ** \arg SdiocCmdEndBitErr              Command End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocCmdIndexErr               Command Index Error. 1: Command Index Error Generatedr; 0:No Error
 ** \arg SdiocDataTimeoutErr            Data Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocDataCrcErr                Data CRC Error. 1: Data CRC Error Generated; 0:No Error
 ** \arg SdiocDataEndBitErr             Data End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocAutoCmd12Err              Auto CMD12 Error. 1: Error; 0:No Error
 **
 ** \retval Set                         The specified interupt flag is set
 ** \retval Reset                       The specified interupt flag is zero
 **
 ******************************************************************************/
en_flag_status_t SDIOC_GetErrIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                     en_sdioc_err_int_flag_t enErrInt)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));
    DDL_ASSERT(IS_VALID_SDIOC_ERR_INT(enErrInt));

    return ((SDIOCx->ERRINTST & ((uint16_t)enErrInt)) ? Set : Reset);
}

/**
 *******************************************************************************
 ** \brief Clear the flag of SD error interrupt
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enErrInt                SDIOC error interrupt
 ** \arg SdiocCmdTimeoutErr             Command Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocCmdCrcErr                 Command CRC Error. 1: Command CRC Error Generated; 0:No Error
 ** \arg SdiocCmdEndBitErr              Command End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocCmdIndexErr               Command Index Error. 1: Command Index Error Generatedr; 0:No Error
 ** \arg SdiocDataTimeoutErr            Data Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocDataCrcErr                Data CRC Error. 1: Data CRC Error Generated; 0:No Error
 ** \arg SdiocDataEndBitErr             Data End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocAutoCmd12Err              Auto CMD12 Error. 1: Error; 0:No Error
 **
 ** \retval Ok                          Clear successfully.
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_ClearErrIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                  en_sdioc_err_int_flag_t enErrInt)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_ERR_INT(enErrInt));
        SDIOCx->ERRINTST = (uint16_t)enErrInt;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Force the specified error interrupt flag
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enErrInt                SDIOC error interrupt
 ** \arg SdiocCmdTimeoutErr             Command Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocCmdCrcErr                 Command CRC Error. 1: Command CRC Error Generated; 0:No Error
 ** \arg SdiocCmdEndBitErr              Command End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocCmdIndexErr               Command Index Error. 1: Command Index Error Generatedr; 0:No Error
 ** \arg SdiocDataTimeoutErr            Data Timeout Error. 1: Timer out; 0:No Error
 ** \arg SdiocDataCrcErr                Data CRC Error. 1: Data CRC Error Generated; 0:No Error
 ** \arg SdiocDataEndBitErr             Data End Bit Error. 1: End Bit Error Generated; 0:No Error
 ** \arg SdiocAutoCmd12Err              Auto CMD12 Error. 1: Error; 0:No Error
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_ForceErrIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                  en_sdioc_err_int_sel_t enErrInt)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_ERR_INT(enErrInt));
        SDIOCx->FEE |= (uint16_t)enErrInt;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the status of auto CMD12 error
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enAutoCmdErr            SDIOC auto cmd error status selection
 ** \arg SdiocAutoCmd12NotExecuted      Auto CMD12 Not Executed. 1: Not executed; 0:Executed
 ** \arg SdiocAutoCmd12Timeout          Auto CMD12 Timeout Error. 1: Time out; 0:No error
 ** \arg SdiocAutoCmd12CrcErr           Auto CMD12 CRC Error. 1: CRC Error Generated; 0: No error
 ** \arg SdiocAutoCmd12EndBitErr        Auto CMD12 End Bit Error. 1: End Bit Error Generated; 0: No error to Write buffer
 ** \arg SdiocAutoCmd12IndexErr         Auto CMD12 Index Error. 1: Error; 0: No error
 ** \arg SdiocCmdNotIssuedErr           Command Not Issued By Auto CMD12 Error.1: Not Issued; 0: No error
 **
 ** \retval Set                         The specified status flag is set
 ** \retval Reset                       The specified status flag is zero
 **
 ******************************************************************************/
en_flag_status_t SDIOC_GetAutoCmdErrStatus(M4_SDIOC_TypeDef *SDIOCx,
                                           en_sdioc_atuo_cmd_err_status_t enAutoCmdErr)
{
    /* Check the parameters */
    DDL_ASSERT(IS_VALID_SDIOC(SDIOCx));
    DDL_ASSERT(IS_VALID_SDIOC_AUTOCMD_ERR(enAutoCmdErr));

    return ((SDIOCx->ATCERRST & ((uint16_t)enAutoCmdErr)) ? Set : Reset);
}

/**
 *******************************************************************************
 ** \brief Force the specified auto CMD12 error
 **
 ** \param [in] SDIOCx                  Pointer to SDIOC instance register base
 ** \arg M4_SDIOC1                      SDIOC unit 1 instance register base
 ** \arg M4_SDIOC2                      SDIOC unit 2 instance register base
 ** \param [in] enAutoCmdErr            SDIOC auto cmd error selection
 ** \arg SdiocAutoCmd12NotExecuted      Auto CMD12 Not Executed. 1: Not executed; 0:Executed
 ** \arg SdiocAutoCmd12Timeout          Auto CMD12 Timeout Error. 1: Time out; 0:No error
 ** \arg SdiocAutoCmd12CrcErr           Auto CMD12 CRC Error. 1: CRC Error Generated; 0: No error
 ** \arg SdiocAutoCmd12EndBitErr        Auto CMD12 End Bit Error. 1: End Bit Error Generated; 0: No error to Write buffer
 ** \arg SdiocAutoCmd12IndexErr         Auto CMD12 Index Error. 1: Error; 0: No error
 ** \arg SdiocCmdNotIssuedErr           Command Not Issued By Auto CMD12 Error.1: Not Issued; 0: No error
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       SDIOCx is invalid
 **
 ******************************************************************************/
en_result_t SDIOC_ForceAutoCmdErr(M4_SDIOC_TypeDef *SDIOCx,
                                  en_sdioc_atuo_cmd_err_sel_t enAutoCmdErr)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for SDIOCx pointer */
    if (IS_VALID_SDIOC(SDIOCx))
    {
        /* Check the parameters */
        DDL_ASSERT(IS_VALID_SDIOC_AUTOCMD_ERR(enAutoCmdErr));
        SDIOCx->FEA |= (uint16_t)enAutoCmdErr;
        enRet = Ok;
    }

    return enRet;
}

//@} // SdiocGroup

#endif /* DDL_SDIOC_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
