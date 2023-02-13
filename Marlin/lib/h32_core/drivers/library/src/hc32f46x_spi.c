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
/** \file hc32f46x_spi.c
 **
 ** A detailed description is available at
 ** @link SpiGroup Serial Peripheral Interface description @endlink
 **
 **   - 2018-10-29  1.0  Yangjp  First version for Device Driver Library of Spi.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_spi.h"
#include "hc32f46x_utility.h"

#if (DDL_SPI_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup SpiGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*!< Parameter valid check for SPI unit */
#define IS_VALID_SPI_UNIT(x) \
    ((M4_SPI1 == (x)) ||     \
     (M4_SPI2 == (x)) ||     \
     (M4_SPI3 == (x)) ||     \
     (M4_SPI4 == (x)))

/*!< Parameter valid check for SS setup delay option */
#define IS_VALID_SS_SETUP_DEALY_OPTION(x)   \
    ((SpiSsSetupDelayTypicalSck1 == (x)) || \
     (SpiSsSetupDelayCustomValue == (x)))

/*!< Parameter valid check for SS setup delay time */
#define IS_VALID_SS_SETUP_DELAY_TIME(x) \
    ((SpiSsSetupDelaySck1 == (x)) ||    \
     (SpiSsSetupDelaySck2 == (x)) ||    \
     (SpiSsSetupDelaySck3 == (x)) ||    \
     (SpiSsSetupDelaySck4 == (x)) ||    \
     (SpiSsSetupDelaySck5 == (x)) ||    \
     (SpiSsSetupDelaySck6 == (x)) ||    \
     (SpiSsSetupDelaySck7 == (x)) ||    \
     (SpiSsSetupDelaySck8 == (x)))

/*!< Parameter valid check for SS hold delay time */
#define IS_VALID_SS_HOLD_DELAY_TIME(x) \
    ((SpiSsHoldDelaySck1 == (x)) ||    \
     (SpiSsHoldDelaySck2 == (x)) ||    \
     (SpiSsHoldDelaySck3 == (x)) ||    \
     (SpiSsHoldDelaySck4 == (x)) ||    \
     (SpiSsHoldDelaySck5 == (x)) ||    \
     (SpiSsHoldDelaySck6 == (x)) ||    \
     (SpiSsHoldDelaySck7 == (x)) ||    \
     (SpiSsHoldDelaySck8 == (x)))

/*!< Parameter valid check for SS hold delay option */
#define IS_VALID_SS_HOLD_DELAY_OPTION(x)   \
    ((SpiSsHoldDelayTypicalSck1 == (x)) || \
     (SpiSsHoldDelayCustomValue == (x)))

/*!< Parameter valid check for SS interval time option */
#define IS_VALID_SS_INTERVAL_TIME_OPTION(x)       \
    ((SpiSsIntervalTypicalSck1PlusPck2 == (x)) || \
     (SpiSsIntervalCustomValue == (x)))

/*!< Parameter valid check for SS interval time */
#define IS_VALID_SS_INTERVAL_TIME(x)       \
    ((SpiSsIntervalSck1PlusPck2 == (x)) || \
     (SpiSsIntervalSck2PlusPck2 == (x)) || \
     (SpiSsIntervalSck3PlusPck2 == (x)) || \
     (SpiSsIntervalSck4PlusPck2 == (x)) || \
     (SpiSsIntervalSck5PlusPck2 == (x)) || \
     (SpiSsIntervalSck6PlusPck2 == (x)) || \
     (SpiSsIntervalSck7PlusPck2 == (x)) || \
     (SpiSsIntervalSck8PlusPck2 == (x)))

/*!< Parameter valid check for SS valid channel select */
#define IS_VALID_SS_VALID_CHANNEL(x) \
    ((SpiSsValidChannel0 == (x)) ||  \
     (SpiSsValidChannel1 == (x)) ||  \
     (SpiSsValidChannel2 == (x)) ||  \
     (SpiSsValidChannel3 == (x)))

/*!< Parameter valid check for SS polarity */
#define IS_VALID_SS_POLARITY(x) \
    ((SpiSsLowValid == (x)) ||  \
     (SpiSsHighValid == (x)))

/*!< Parameter valid check for read data register object */
#define IS_VALID_READ_DATA_REG_OBJECT(x) \
    ((SpiReadReceiverBuffer == (x)) ||   \
     (SpiReadSendBuffer == (x)))

/*!< Parameter valid check for SCK polarity */
#define IS_VALID_SCK_POLARITY(x)    \
    ((SpiSckIdleLevelLow == (x)) || \
     (SpiSckIdelLevelHigh == (x)))

/*!< Parameter valid check for SCK phase */
#define IS_VALID_SCK_PHASE(x)              \
    ((SpiSckOddSampleEvenChange == (x)) || \
     (SpiSckOddChangeEvenSample == (x)))

/*!< Parameter valid check for clock division */
#define IS_VALID_CLK_DIV(x)   \
    ((SpiClkDiv2 == (x)) ||   \
     (SpiClkDiv4 == (x)) ||   \
     (SpiClkDiv8 == (x)) ||   \
     (SpiClkDiv16 == (x)) ||  \
     (SpiClkDiv32 == (x)) ||  \
     (SpiClkDiv64 == (x)) ||  \
     (SpiClkDiv128 == (x)) || \
     (SpiClkDiv256 == (x)))

/*!< Parameter valid check for data length */
#define IS_VALID_DATA_LENGTH(x)     \
    ((SpiDataLengthBit4 == (x)) ||  \
     (SpiDataLengthBit5 == (x)) ||  \
     (SpiDataLengthBit6 == (x)) ||  \
     (SpiDataLengthBit7 == (x)) ||  \
     (SpiDataLengthBit8 == (x)) ||  \
     (SpiDataLengthBit9 == (x)) ||  \
     (SpiDataLengthBit10 == (x)) || \
     (SpiDataLengthBit11 == (x)) || \
     (SpiDataLengthBit12 == (x)) || \
     (SpiDataLengthBit13 == (x)) || \
     (SpiDataLengthBit14 == (x)) || \
     (SpiDataLengthBit15 == (x)) || \
     (SpiDataLengthBit16 == (x)) || \
     (SpiDataLengthBit20 == (x)) || \
     (SpiDataLengthBit24 == (x)) || \
     (SpiDataLengthBit32 == (x)))

/*!< Parameter valid check for first bit position */
#define IS_VALID_FIRST_BIT_POSITION(x)  \
    ((SpiFirstBitPositionMSB == (x)) || \
     (SpiFirstBitPositionLSB == (x)))

/*!< Parameter valid check for frame number */
#define IS_VALID_FRAME_NUMBER(x) \
    ((SpiFrameNumber1 == (x)) || \
     (SpiFrameNumber2 == (x)) || \
     (SpiFrameNumber3 == (x)) || \
     (SpiFrameNumber4 == (x)))

/*!< Parameter valid check for work mode */
#define IS_VALID_WORK_MODE(x)     \
    ((SpiWorkMode4Line == (x)) || \
     (SpiWorkMode3Line == (x)))

/*!< Parameter valid check for transmission mode */
#define IS_VALID_COMM_MODE(x)       \
    ((SpiTransFullDuplex == (x)) || \
     (SpiTransOnlySend == (x)))

/*!< Parameter valid check for master slave mode */
#define IS_VALID_MASTER_SLAVE_MODE(x) \
    ((SpiModeSlave == (x)) ||         \
     (SpiModeMaster == (x)))

/*!< Parameter valid check for parity mode */
#define IS_VALID_PARITY_MODE(x) \
    ((SpiParityEven == (x)) ||  \
     (SpiParityOdd == (x)))

/*!< Parameter valid check for SS channel */
#define IS_VALID_SS_CHANNEL(x) \
    ((SpiSsChannel0 == (x)) || \
     (SpiSsChannel1 == (x)) || \
     (SpiSsChannel2 == (x)) || \
     (SpiSsChannel3 == (x)))

/*!< Parameter valid check for irq type */
#define IS_VALID_IRQ_TYPE(x)   \
    ((SpiIrqIdel == (x)) ||    \
     (SpiIrqReceive == (x)) || \
     (SpiIrqSend == (x)) ||    \
     (SpiIrqError == (x)))

/*!< Parameter valid check for flag type */
#define IS_VALID_FLAG_TYPE(x)             \
    ((SpiFlagReceiveBufferFull == (x)) || \
     (SpiFlagSendBufferEmpty == (x)) ||   \
     (SpiFlagUnderloadError == (x)) ||    \
     (SpiFlagParityError == (x)) ||       \
     (SpiFlagModeFaultError == (x)) ||    \
     (SpiFlagSpiIdle == (x)) ||           \
     (SpiFlagOverloadError == (x)))

/*!< SPI registers reset value */
#define SPI_REG_DR_RESET_VALUE 0x00000000ul
#define SPI_REG_CR1_RESET_VALUE 0x00000000ul
#define SPI_REG_CFG1_RESET_VALUE 0x00000010ul
#define SPI_REG_SR_RESET_VALUE 0x00000020ul
#define SPI_REG_CFG2_RESET_VALUE 0x0000031Dul

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief De-Initialize SPI unit
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_DeInit(M4_SPI_TypeDef *SPIx)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t regTemp = 0ul;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        regTemp = SPIx->SR;
        if (SPI_REG_SR_RESET_VALUE != regTemp)
        {
            SPIx->SR = SPI_REG_SR_RESET_VALUE;
        }
        SPIx->CR1 = SPI_REG_CR1_RESET_VALUE;
        SPIx->DR = SPI_REG_DR_RESET_VALUE;
        SPIx->CFG1 = SPI_REG_CFG1_RESET_VALUE;
        SPIx->CFG2 = SPI_REG_CFG2_RESET_VALUE;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize SPI unit
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] pstcSpiInitCfg          Pointer to SPI init configuration
 ** \arg See the struct #stc_spi_init_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **                                     - pstcSpiInitCfg == NULL
 **
 ******************************************************************************/
en_result_t SPI_Init(M4_SPI_TypeDef *SPIx, const stc_spi_init_t *pstcSpiInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if ((IS_VALID_SPI_UNIT(SPIx)) && (NULL != pstcSpiInitCfg))
    {
        DDL_ASSERT(IS_VALID_SS_SETUP_DEALY_OPTION(pstcSpiInitCfg->stcDelayConfig.enSsSetupDelayOption));
        DDL_ASSERT(IS_VALID_SS_SETUP_DELAY_TIME(pstcSpiInitCfg->stcDelayConfig.enSsSetupDelayTime));
        DDL_ASSERT(IS_VALID_SS_HOLD_DELAY_OPTION(pstcSpiInitCfg->stcDelayConfig.enSsHoldDelayOption));
        DDL_ASSERT(IS_VALID_SS_HOLD_DELAY_TIME(pstcSpiInitCfg->stcDelayConfig.enSsHoldDelayTime));
        DDL_ASSERT(IS_VALID_SS_INTERVAL_TIME_OPTION(pstcSpiInitCfg->stcDelayConfig.enSsIntervalTimeOption));
        DDL_ASSERT(IS_VALID_SS_INTERVAL_TIME(pstcSpiInitCfg->stcDelayConfig.enSsIntervalTime));
        DDL_ASSERT(IS_VALID_SS_VALID_CHANNEL(pstcSpiInitCfg->stcSsConfig.enSsValidBit));
        DDL_ASSERT(IS_VALID_SS_POLARITY(pstcSpiInitCfg->stcSsConfig.enSs0Polarity));
        DDL_ASSERT(IS_VALID_SS_POLARITY(pstcSpiInitCfg->stcSsConfig.enSs1Polarity));
        DDL_ASSERT(IS_VALID_SS_POLARITY(pstcSpiInitCfg->stcSsConfig.enSs2Polarity));
        DDL_ASSERT(IS_VALID_SS_POLARITY(pstcSpiInitCfg->stcSsConfig.enSs3Polarity));
        DDL_ASSERT(IS_VALID_READ_DATA_REG_OBJECT(pstcSpiInitCfg->enReadBufferObject));
        DDL_ASSERT(IS_VALID_SCK_POLARITY(pstcSpiInitCfg->enSckPolarity));
        DDL_ASSERT(IS_VALID_SCK_PHASE(pstcSpiInitCfg->enSckPhase));
        DDL_ASSERT(IS_VALID_CLK_DIV(pstcSpiInitCfg->enClkDiv));
        DDL_ASSERT(IS_VALID_DATA_LENGTH(pstcSpiInitCfg->enDataLength));
        DDL_ASSERT(IS_VALID_FIRST_BIT_POSITION(pstcSpiInitCfg->enFirstBitPosition));
        DDL_ASSERT(IS_VALID_FRAME_NUMBER(pstcSpiInitCfg->enFrameNumber));
        DDL_ASSERT(IS_VALID_WORK_MODE(pstcSpiInitCfg->enWorkMode));
        DDL_ASSERT(IS_VALID_COMM_MODE(pstcSpiInitCfg->enTransMode));
        DDL_ASSERT(IS_VALID_MASTER_SLAVE_MODE(pstcSpiInitCfg->enMasterSlaveMode));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcSpiInitCfg->enCommAutoSuspendEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcSpiInitCfg->enModeFaultErrorDetectEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcSpiInitCfg->enParitySelfDetectEn));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcSpiInitCfg->enParityEn));
        DDL_ASSERT(IS_VALID_PARITY_MODE(pstcSpiInitCfg->enParity));

        /* Master mode */
        if (SpiModeMaster == pstcSpiInitCfg->enMasterSlaveMode)
        {
            SPIx->CFG2_f.SCKDLE = pstcSpiInitCfg->stcDelayConfig.enSsSetupDelayOption;
            SPIx->CFG2_f.SSDLE = pstcSpiInitCfg->stcDelayConfig.enSsHoldDelayOption;
            SPIx->CFG2_f.NXTDLE = pstcSpiInitCfg->stcDelayConfig.enSsIntervalTimeOption;
            SPIx->CFG1_f.MSSI = pstcSpiInitCfg->stcDelayConfig.enSsSetupDelayTime;
            SPIx->CFG1_f.MSSDL = pstcSpiInitCfg->stcDelayConfig.enSsHoldDelayTime;
            SPIx->CFG1_f.MIDI = pstcSpiInitCfg->stcDelayConfig.enSsIntervalTime;
        }
        else
        {
            SPIx->CFG2_f.SCKDLE = SpiSsSetupDelayTypicalSck1;
            SPIx->CFG2_f.SSDLE = SpiSsHoldDelayTypicalSck1;
            SPIx->CFG2_f.NXTDLE = SpiSsIntervalTypicalSck1PlusPck2;
            SPIx->CFG1_f.MSSI = SpiSsSetupDelaySck1;
            SPIx->CFG1_f.MSSDL = SpiSsHoldDelaySck1;
            SPIx->CFG1_f.MIDI = SpiSsIntervalSck1PlusPck2;
        }

        /* 4 lines spi mode */
        if (SpiWorkMode4Line == pstcSpiInitCfg->enWorkMode)
        {
            SPIx->CFG2_f.SSA = pstcSpiInitCfg->stcSsConfig.enSsValidBit;
            SPIx->CFG1_f.SS0PV = pstcSpiInitCfg->stcSsConfig.enSs0Polarity;
            SPIx->CFG1_f.SS1PV = pstcSpiInitCfg->stcSsConfig.enSs1Polarity;
            SPIx->CFG1_f.SS2PV = pstcSpiInitCfg->stcSsConfig.enSs2Polarity;
            SPIx->CFG1_f.SS3PV = pstcSpiInitCfg->stcSsConfig.enSs3Polarity;
        }
        else
        {
            SPIx->CFG2_f.SSA = SpiSsValidChannel0;
            SPIx->CFG1_f.SS0PV = SpiSsLowValid;
            SPIx->CFG1_f.SS1PV = SpiSsLowValid;
            SPIx->CFG1_f.SS2PV = SpiSsLowValid;
            SPIx->CFG1_f.SS3PV = SpiSsLowValid;
        }

        /* Configure communication config register 1 */
        SPIx->CFG1_f.SPRDTD = pstcSpiInitCfg->enReadBufferObject;
        SPIx->CFG1_f.FTHLV = pstcSpiInitCfg->enFrameNumber;

        /* Configure communication config register 2 */
        SPIx->CFG2_f.LSBF = pstcSpiInitCfg->enFirstBitPosition;
        SPIx->CFG2_f.DSIZE = pstcSpiInitCfg->enDataLength;
        SPIx->CFG2_f.MBR = pstcSpiInitCfg->enClkDiv;
        SPIx->CFG2_f.CPOL = pstcSpiInitCfg->enSckPolarity;
        SPIx->CFG2_f.CPHA = pstcSpiInitCfg->enSckPhase;

        /* Configure control register */
        SPIx->CR1_f.SPIMDS = pstcSpiInitCfg->enWorkMode;
        SPIx->CR1_f.TXMDS = pstcSpiInitCfg->enTransMode;
        SPIx->CR1_f.MSTR = pstcSpiInitCfg->enMasterSlaveMode;
        SPIx->CR1_f.CSUSPE = pstcSpiInitCfg->enCommAutoSuspendEn;
        SPIx->CR1_f.MODFE = pstcSpiInitCfg->enModeFaultErrorDetectEn;
        SPIx->CR1_f.PATE = pstcSpiInitCfg->enParitySelfDetectEn;
        SPIx->CR1_f.PAE = pstcSpiInitCfg->enParityEn;
        SPIx->CR1_f.PAOE = pstcSpiInitCfg->enParity;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable SPI general loopback
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable general loopback
 ** \arg Enable                         Enable general loopback
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_GeneralLoopbackCmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        SPIx->CR1_f.SPLPBK2 = enNewSta;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable SPI reverse loopback
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable reverse loopback
 ** \arg Enable                         Enable reverse loopback
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_ReverseLoopbackCmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        SPIx->CR1_f.SPLPBK = enNewSta;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable SPI working
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable SPI working
 ** \arg Enable                         Enable SPI working
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_Cmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        SPIx->CR1_f.SPE = enNewSta;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI send 8bit data or 4/5/6/7 bit data
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] u8Data                  Send data value
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SendData8(M4_SPI_TypeDef *SPIx, uint8_t u8Data)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        SPIx->DR = u8Data;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI send 16bit data or 9/10/11/12/13/14/15 bit data
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] u16Data                 Send data value
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SendData16(M4_SPI_TypeDef *SPIx, uint16_t u16Data)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        SPIx->DR = u16Data;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI send 32bit data or 20/24 bit data
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] u32Data                 Send data value
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SendData32(M4_SPI_TypeDef *SPIx, uint32_t u32Data)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        SPIx->DR = u32Data;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI receive 8bit data or 4/5/6/7 bit data
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \retval uint8_t                     Receive data value
 **
 ******************************************************************************/
uint8_t SPI_ReceiveData8(const M4_SPI_TypeDef *SPIx)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    return ((uint8_t)SPIx->DR);
}

/**
 *******************************************************************************
 ** \brief SPI receive 16bit data or 9/10/11/12/13/14/15 bit data
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \retval uint16_t                    Receive data value
 **
 ******************************************************************************/
uint16_t SPI_ReceiveData16(const M4_SPI_TypeDef *SPIx)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    return ((uint16_t)SPIx->DR);
}

/**
 *******************************************************************************
 ** \brief SPI receive 32bit data or 20/24 bit data
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \retval uint32_t                    Receive data value
 **
 ******************************************************************************/
uint32_t SPI_ReceiveData32(const M4_SPI_TypeDef *SPIx)
{
    /* Check parameters */
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    return ((uint32_t)SPIx->DR);
}

/**
 *******************************************************************************
 ** \brief SPI set SS channel valid level polarity
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enChannel               Select Slave channel
 ** \arg SpiSsChannel0                  SS0 channel
 ** \arg SpiSsChannel1                  SS1 channel
 ** \arg SpiSsChannel2                  SS2 channel
 ** \arg SpiSsChannel3                  SS3 channel
 **
 ** \param [in] enPolarity              SS channel valid level polarity
 ** \arg SpiSsLowValid                  SS0~3 signal low level valid
 ** \arg SpiSsHighValid                 SS0~3 signal high level valid
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SetSsPolarity(M4_SPI_TypeDef *SPIx, en_spi_ss_channel_t enChannel,
                              en_spi_ss_polarity_t enPolarity)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_SS_CHANNEL(enChannel));
        DDL_ASSERT(IS_VALID_SS_POLARITY(enPolarity));

        switch (enChannel)
        {
        case SpiSsChannel0:
            SPIx->CFG1_f.SS0PV = enPolarity;
            break;
        case SpiSsChannel1:
            SPIx->CFG1_f.SS1PV = enPolarity;
            break;
        case SpiSsChannel2:
            SPIx->CFG1_f.SS2PV = enPolarity;
            break;
        case SpiSsChannel3:
            SPIx->CFG1_f.SS3PV = enPolarity;
            break;
        default:
            break;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI set SS valid channel
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enChannel               Select Slave channel
 ** \arg SpiSsChannel0                  SS0 channel
 ** \arg SpiSsChannel1                  SS1 channel
 ** \arg SpiSsChannel2                  SS2 channel
 ** \arg SpiSsChannel3                  SS3 channel
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SetSsValidChannel(M4_SPI_TypeDef *SPIx, en_spi_ss_channel_t enChannel)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_SS_CHANNEL(enChannel));

        SPIx->CFG2_f.SSA = enChannel;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI set read data register object
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enObject                Read data register object
 ** \arg SpiReadReceiverBuffer          Read receive buffer
 ** \arg SpiReadSendBuffer              Read send buffer(must be read when TDEF=1)
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SetReadDataRegObject(M4_SPI_TypeDef *SPIx, en_spi_read_object_t enObject)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_READ_DATA_REG_OBJECT(enObject));

        SPIx->CFG1_f.SPRDTD = enObject;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI set frame number
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enFrameNum              Once read or write frame number
 ** \arg SpiFrameNumber1                1 frame data
 ** \arg SpiFrameNumber2                2 frame data
 ** \arg SpiFrameNumber3                3 frame data
 ** \arg SpiFrameNumber4                4 frame data
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SetFrameNumber(M4_SPI_TypeDef *SPIx, en_spi_frame_number_t enFrameNum)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_FRAME_NUMBER(enFrameNum));

        SPIx->CFG1_f.FTHLV = enFrameNum;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI set data length
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enDataLength            Read or write data length
 ** \arg SpiDataLengthBit4              4 bits
 ** \arg SpiDataLengthBit5              5 bits
 ** \arg SpiDataLengthBit6              6 bits
 ** \arg SpiDataLengthBit7              7 bits
 ** \arg SpiDataLengthBit8              8 bits
 ** \arg SpiDataLengthBit9              9 bits
 ** \arg SpiDataLengthBit10             10 bits
 ** \arg SpiDataLengthBit11             11 bits
 ** \arg SpiDataLengthBit12             12 bits
 ** \arg SpiDataLengthBit13             13 bits
 ** \arg SpiDataLengthBit14             14 bits
 ** \arg SpiDataLengthBit15             15 bits
 ** \arg SpiDataLengthBit16             16 bits
 ** \arg SpiDataLengthBit20             20 bits
 ** \arg SpiDataLengthBit24             24 bits
 ** \arg SpiDataLengthBit32             32 bits
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SetDataLength(M4_SPI_TypeDef *SPIx, en_spi_data_length_t enDataLength)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_DATA_LENGTH(enDataLength));

        SPIx->CFG2_f.DSIZE = enDataLength;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI set first bit position
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enPosition              First bit position
 ** \arg SpiFirstBitPositionMSB         Spi first bit to MSB
 ** \arg SpiFirstBitPositionLSB         Spi first bit to LSB
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SetFirstBitPosition(M4_SPI_TypeDef *SPIx, en_spi_first_bit_position_t enPosition)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_FIRST_BIT_POSITION(enPosition));

        SPIx->CFG2_f.LSBF = enPosition;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief SPI set clock division
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enClkDiv                Clock division
 ** \arg SpiClkDiv2                     Spi pckl1 division 2
 ** \arg SpiClkDiv4                     Spi pckl1 division 4
 ** \arg SpiClkDiv8                     Spi pckl1 division 8
 ** \arg SpiClkDiv16                    Spi pckl1 division 16
 ** \arg SpiClkDiv32                    Spi pckl1 division 32
 ** \arg SpiClkDiv64                    Spi pckl1 division 64
 ** \arg SpiClkDiv128                   Spi pckl1 division 128
 ** \arg SpiClkDiv256                   Spi pckl1 division 256
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_SetClockDiv(M4_SPI_TypeDef *SPIx, en_spi_clk_div_t enClkDiv)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_CLK_DIV(enClkDiv));

        SPIx->CFG2_f.MBR = enClkDiv;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable SPI interrupt request
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enIrq                   SPI interrupt request type
 ** \arg SpiIrqIdel                     Spi idle interrupt request
 ** \arg SpiIrqReceive                  Spi receive interrupt request
 ** \arg SpiIrqSend                     Spi send interrupt request
 ** \arg SpiIrqError                    Spi error interrupt request
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable interrupt request
 ** \arg Enable                         Enable interrupt request
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_IrqCmd(M4_SPI_TypeDef *SPIx, en_spi_irq_type_t enIrq,
                       en_functional_state_t enNewSta)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_IRQ_TYPE(enIrq));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

        switch (enIrq)
        {
        case SpiIrqIdel:
            SPIx->CR1_f.IDIE = enNewSta;
            break;
        case SpiIrqReceive:
            SPIx->CR1_f.RXIE = enNewSta;
            break;
        case SpiIrqSend:
            SPIx->CR1_f.TXIE = enNewSta;
            break;
        case SpiIrqError:
            SPIx->CR1_f.EIE = enNewSta;
            break;
        default:
            break;
        }
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get SPI flag status
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enFlag                  SPI flag type
 ** \arg SpiFlagReceiveBufferFull       Receive buffer full flag
 ** \arg SpiFlagSendBufferEmpty         Send buffer empty flag
 ** \arg SpiFlagUnderloadError          Underload error flag
 ** \arg SpiFlagParityError             Parity error flag
 ** \arg SpiFlagModeFaultError          Mode fault error flag
 ** \arg SpiFlagSpiIdle                 SPI idle flag
 ** \arg SpiFlagOverloadErro            Overload error flag
 **
 ** \retval Set                         Flag is set
 ** \retval Reset                       Flag is reset
 **
 ******************************************************************************/
en_flag_status_t SPI_GetFlag(M4_SPI_TypeDef *SPIx, en_spi_flag_type_t enFlag)
{
    en_flag_status_t enFlagSta = Reset;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_FLAG_TYPE(enFlag));

        switch (enFlag)
        {
        case SpiFlagReceiveBufferFull:
            enFlagSta = (en_flag_status_t)SPIx->SR_f.RDFF;
            break;
        case SpiFlagSendBufferEmpty:
            enFlagSta = (en_flag_status_t)SPIx->SR_f.TDEF;
            break;
        case SpiFlagUnderloadError:
            enFlagSta = (en_flag_status_t)SPIx->SR_f.UDRERF;
            break;
        case SpiFlagParityError:
            enFlagSta = (en_flag_status_t)SPIx->SR_f.PERF;
            break;
        case SpiFlagModeFaultError:
            enFlagSta = (en_flag_status_t)SPIx->SR_f.MODFERF;
            break;
        case SpiFlagSpiIdle:
            enFlagSta = (en_flag_status_t)(bool)(!SPIx->SR_f.IDLNF);
            break;
        case SpiFlagOverloadError:
            enFlagSta = (en_flag_status_t)SPIx->SR_f.OVRERF;
            break;
        default:
            break;
        }
    }

    return enFlagSta;
}

/**
 *******************************************************************************
 ** \brief Clear SPI flag status
 **
 ** \param [in] SPIx                    Pointer to SPI unit configuration address
 ** \arg M4_SPI1                        SPI unit 1 configuration Address
 ** \arg M4_SPI2                        SPI unit 2 configuration Address
 ** \arg M4_SPI3                        SPI unit 3 configuration Address
 ** \arg M4_SPI4                        SPI unit 4 configuration Address
 **
 ** \param [in] enFlag                  SPI flag type
 ** \arg SpiFlagReceiveBufferFull       Receive buffer full flag
 ** \arg SpiFlagSendBufferEmpty         Send buffer empty flag
 ** \arg SpiFlagUnderloadError          Underload error flag
 ** \arg SpiFlagParityError             Parity error flag
 ** \arg SpiFlagModeFaultError          Mode fault error flag
 ** \arg SpiFlagSpiIdle                 SPI empty flag
 ** \arg SpiFlagOverloadErro            Overload error flag
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - SPIx is invalid
 **
 ******************************************************************************/
en_result_t SPI_ClearFlag(M4_SPI_TypeDef *SPIx, en_spi_flag_type_t enFlag)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check parameters */
    if (IS_VALID_SPI_UNIT(SPIx))
    {
        DDL_ASSERT(IS_VALID_FLAG_TYPE(enFlag));

        switch (enFlag)
        {
        case SpiFlagReceiveBufferFull:
            SPIx->SR_f.RDFF = 0u;
            break;
        case SpiFlagSendBufferEmpty:
            SPIx->SR_f.TDEF = 0u;
            break;
        case SpiFlagUnderloadError:
            SPIx->SR_f.UDRERF = 0u;
            break;
        case SpiFlagParityError:
            SPIx->SR_f.PERF = 0u;
            break;
        case SpiFlagModeFaultError:
            SPIx->SR_f.MODFERF = 0u;
            break;
        case SpiFlagSpiIdle:
            SPIx->SR_f.IDLNF = 0u;
            break;
        case SpiFlagOverloadError:
            SPIx->SR_f.OVRERF = 0u;
            break;
        default:
            break;
        }
        enRet = Ok;
    }

    return enRet;
}

//@} // SpiGroup

#endif /* DDL_SPI_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
