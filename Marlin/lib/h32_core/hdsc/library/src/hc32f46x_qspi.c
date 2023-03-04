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
/** \file hc32f46x_qspi.c
 **
 ** A detailed description is available at
 ** @link QspiGroup Queued SPI description @endlink
 **
 **   - 2018-11-20  1.0  Yangjp  First version for Device Driver Library of Qspi.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_qspi.h"
#include "hc32f46x_utility.h"

#if (DDL_QSPI_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup QspiGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*!< Parameter valid check for clock division */
#define IS_VALID_CLK_DIV(x)   \
    (((x) == QspiHclkDiv2) || \
     (((x) >= QspiHclkDiv3) && ((x) <= QspiHclkDiv64)))

/*!< Parameter valid check for spi mode */
#define IS_VALID_SPI_MODE(x)  \
    ((QspiSpiMode0 == (x)) || \
     (QspiSpiMode3 == (x)))

/*!< Parameter valid check for bus communication mode */
#define IS_VALID_BUS_COMM_MODE(x)     \
    ((QspiBusModeRomAccess == (x)) || \
     (QspiBusModeDirectAccess == (x)))

/*!< Parameter valid check for prefetch stop location */
#define IS_VALID_PREFETCH_STOP_LOCATION(x) \
    ((QspiPrefetchStopComplete == (x)) ||  \
     (QspiPrefetchStopImmediately == (x)))

/*!< Parameter valid check for receive data protocol */
#define IS_VALID_RECE_DATA_PROTOCOL(x)   \
    ((QspiProtocolExtendSpi == (x)) ||   \
     (QspiProtocolTwoWiresSpi == (x)) || \
     (QspiProtocolFourWiresSpi == (x)))

/*!< Parameter valid check for transmit address protocol */
#define IS_VALID_TRANS_ADDR_PROTOCOL(x)  \
    ((QspiProtocolExtendSpi == (x)) ||   \
     (QspiProtocolTwoWiresSpi == (x)) || \
     (QspiProtocolFourWiresSpi == (x)))

/*!< Parameter valid check for transmit instruction protocol */
#define IS_VALID_TRANS_INSTRUCT_PROTOCOL(x) \
    ((QspiProtocolExtendSpi == (x)) ||      \
     (QspiProtocolTwoWiresSpi == (x)) ||    \
     (QspiProtocolFourWiresSpi == (x)))

/*!< Parameter valid check for serial interface read mode */
#define IS_VALID_INTERFACE_READ_MODE(x)      \
    ((QspiReadModeStandard == (x)) ||        \
     (QspiReadModeFast == (x)) ||            \
     (QspiReadModeTwoWiresOutput == (x)) ||  \
     (QspiReadModeTwoWiresIO == (x)) ||      \
     (QspiReadModeFourWiresOutput == (x)) || \
     (QspiReadModeFourWiresIO == (x)) ||     \
     (QspiReadModeCustomStandard == (x)) ||  \
     (QspiReadModeCustomFast == (x)))

/*!< Parameter valid check for QSSN valid extend delay time */
#define IS_VALID_QSSN_VALID_EXTEND_TIME(x) \
    ((QspiQssnValidExtendNot == (x)) ||    \
     (QspiQssnValidExtendSck32 == (x)) ||  \
     (QspiQssnValidExtendSck128 == (x)) || \
     (QspiQssnValidExtendSckEver == (x)))

/*!< Parameter valid check for QSSN minimum interval time */
#define IS_VALID_QSSN_INTERVAL_TIME(x)  \
    ((QspiQssnIntervalQsck1 == (x)) ||  \
     (QspiQssnIntervalQsck2 == (x)) ||  \
     (QspiQssnIntervalQsck3 == (x)) ||  \
     (QspiQssnIntervalQsck4 == (x)) ||  \
     (QspiQssnIntervalQsck5 == (x)) ||  \
     (QspiQssnIntervalQsck6 == (x)) ||  \
     (QspiQssnIntervalQsck7 == (x)) ||  \
     (QspiQssnIntervalQsck8 == (x)) ||  \
     (QspiQssnIntervalQsck9 == (x)) ||  \
     (QspiQssnIntervalQsck10 == (x)) || \
     (QspiQssnIntervalQsck11 == (x)) || \
     (QspiQssnIntervalQsck12 == (x)) || \
     (QspiQssnIntervalQsck13 == (x)) || \
     (QspiQssnIntervalQsck14 == (x)) || \
     (QspiQssnIntervalQsck15 == (x)) || \
     (QspiQssnIntervalQsck16 <= (x)))

/*!< Parameter valid check for QSCK duty correction */
#define IS_VALID_QSCK_DUTY_CORR(x)   \
    ((QspiQsckDutyCorrNot == (x)) || \
     (QspiQsckDutyCorrHalfHclk == (x)))

/*!< Parameter valid check for virtual cycles */
#define IS_VALID_VIRTUAL_CYCLES(x)       \
    ((QspiVirtualPeriodQsck3 == (x)) ||  \
     (QspiVirtualPeriodQsck4 == (x)) ||  \
     (QspiVirtualPeriodQsck5 == (x)) ||  \
     (QspiVirtualPeriodQsck6 == (x)) ||  \
     (QspiVirtualPeriodQsck7 == (x)) ||  \
     (QspiVirtualPeriodQsck8 == (x)) ||  \
     (QspiVirtualPeriodQsck9 == (x)) ||  \
     (QspiVirtualPeriodQsck10 == (x)) || \
     (QspiVirtualPeriodQsck11 == (x)) || \
     (QspiVirtualPeriodQsck12 == (x)) || \
     (QspiVirtualPeriodQsck13 == (x)) || \
     (QspiVirtualPeriodQsck14 == (x)) || \
     (QspiVirtualPeriodQsck15 == (x)) || \
     (QspiVirtualPeriodQsck16 == (x)) || \
     (QspiVirtualPeriodQsck17 == (x)) || \
     (QspiVirtualPeriodQsck18 == (x)))

/*!< Parameter valid check for WP pin output level */
#define IS_VALID_WP_OUTPUT_LEVEL(x) \
    ((QspiWpPinOutputLow == (x)) || \
     (QspiWpPinOutputHigh == (x)))

/*!< Parameter valid check for QSSN setup delay time */
#define IS_VALID_QSSN_SETUP_DELAY(x)        \
    ((QspiQssnSetupDelayHalfQsck == (x)) || \
     (QspiQssnSetupDelay1Dot5Qsck == (x)))

/*!< Parameter valid check for QSSN hold delay time */
#define IS_VALID_QSSN_HOLD_TIME(x)         \
    ((QspiQssnHoldDelayHalfQsck == (x)) || \
     (QspiQssnHoldDelay1Dot5Qsck == (x)))

/*!< Parameter valid check for interface address width */
#define IS_VALID_INTERFACE_ADDR_WIDTH(x) \
    ((QspiAddressByteOne == (x)) ||      \
     (QspiAddressByteTwo == (x)) ||      \
     (QspiAddressByteThree == (x)) ||    \
     (QspiAddressByteFour == (x)))

/*!< Parameter valid check for extend address */
#define IS_VALID_SET_EXTEND_ADDR(x) ((x) <= 0x3Fu)

/*!< Parameter valid check for get flag type */
#define IS_VALID_GET_FLAG_TYPE(x)           \
    ((QspiFlagBusBusy == (x)) ||            \
     (QspiFlagXipMode == (x)) ||            \
     (QspiFlagRomAccessError == (x)) ||     \
     (QspiFlagPrefetchBufferFull == (x)) || \
     (QspiFlagPrefetchStop == (x)))

/*!< Parameter valid check for clear flag type */
#define IS_VALID_CLEAR_FLAG_TYPE(x) (QspiFlagRomAccessError == (x))

/*!< QSPI registers reset value */
#define QSPI_REG_CR_RESET_VALUE (0x003F0000ul)
#define QSPI_REG_CSCR_RESET_VALUE (0x0000000Ful)
#define QSPI_REG_FCR_RESET_VALUE (0x000080B3ul)
#define QSPI_REG_SR_RESET_VALUE (0x00008000ul)
#define QSPI_REG_CCMD_RESET_VALUE (0x00000000ul)
#define QSPI_REG_XCMD_RESET_VALUE (0x000000FFul)
#define QSPI_REG_EXAR_RESET_VALUE (0x00000000ul)
#define QSPI_REG_SR2_RESET_VALUE (0x00000000ul)
#define QSPI_REG_DCOM_RESET_VALUE (0x00000000ul)

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
 ** \brief De-Initialize QSPI unit
 **
 ** \param [in] None
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_DeInit(void)
{
    en_result_t enRet = Ok;

    M4_QSPI->CR = QSPI_REG_CR_RESET_VALUE;
    if (1u == M4_QSPI->SR_f.RAER)
    {
        M4_QSPI->SR2_f.RAERCLR = 1u;
    }
    M4_QSPI->CSCR = QSPI_REG_CSCR_RESET_VALUE;
    M4_QSPI->FCR = QSPI_REG_FCR_RESET_VALUE;
    M4_QSPI->EXAR = QSPI_REG_EXAR_RESET_VALUE;
    M4_QSPI->SR = QSPI_REG_SR_RESET_VALUE;
    M4_QSPI->CCMD = QSPI_REG_CCMD_RESET_VALUE;
    M4_QSPI->XCMD = QSPI_REG_XCMD_RESET_VALUE;
    M4_QSPI->DCOM = QSPI_REG_DCOM_RESET_VALUE;
    M4_QSPI->SR2 = QSPI_REG_SR2_RESET_VALUE;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize QSPI unit
 **
 ** \param [in] pstcQspiInitCfg         Pointer to qspi configuration
 ** \arg See the struct #stc_qspi_init_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval Error                       Parameter error
 **
 ******************************************************************************/
en_result_t QSPI_Init(const stc_qspi_init_t *pstcQspiInitCfg)
{
    en_result_t enRet = Ok;

    if (NULL == pstcQspiInitCfg)
    {
        enRet = Error;
    }
    else
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_CLK_DIV(pstcQspiInitCfg->enClkDiv));
        DDL_ASSERT(IS_VALID_SPI_MODE(pstcQspiInitCfg->enSpiMode));
        DDL_ASSERT(IS_VALID_BUS_COMM_MODE(pstcQspiInitCfg->enBusCommMode));
        DDL_ASSERT(IS_VALID_PREFETCH_STOP_LOCATION(pstcQspiInitCfg->enPrefetchMode));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcQspiInitCfg->enPrefetchFuncEn));
        DDL_ASSERT(IS_VALID_RECE_DATA_PROTOCOL(pstcQspiInitCfg->stcCommProtocol.enReceProtocol));
        DDL_ASSERT(IS_VALID_TRANS_ADDR_PROTOCOL(pstcQspiInitCfg->stcCommProtocol.enTransAddrProtocol));
        DDL_ASSERT(IS_VALID_TRANS_INSTRUCT_PROTOCOL(pstcQspiInitCfg->stcCommProtocol.enTransInstrProtocol));
        DDL_ASSERT(IS_VALID_INTERFACE_READ_MODE(pstcQspiInitCfg->stcCommProtocol.enReadMode));
        DDL_ASSERT(IS_VALID_QSSN_VALID_EXTEND_TIME(pstcQspiInitCfg->enQssnValidExtendTime));
        DDL_ASSERT(IS_VALID_QSSN_INTERVAL_TIME(pstcQspiInitCfg->enQssnIntervalTime));
        DDL_ASSERT(IS_VALID_QSCK_DUTY_CORR(pstcQspiInitCfg->enQsckDutyCorr));
        DDL_ASSERT(IS_VALID_VIRTUAL_CYCLES(pstcQspiInitCfg->enVirtualPeriod));
        DDL_ASSERT(IS_VALID_WP_OUTPUT_LEVEL(pstcQspiInitCfg->enWpPinLevel));
        DDL_ASSERT(IS_VALID_QSSN_SETUP_DELAY(pstcQspiInitCfg->enQssnSetupDelayTime));
        DDL_ASSERT(IS_VALID_QSSN_HOLD_TIME(pstcQspiInitCfg->enQssnHoldDelayTime));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcQspiInitCfg->enFourByteAddrReadEn));
        DDL_ASSERT(IS_VALID_INTERFACE_ADDR_WIDTH(pstcQspiInitCfg->enAddrWidth));

        /* Configure control register */
        M4_QSPI->CR_f.DIV = pstcQspiInitCfg->enClkDiv;
        M4_QSPI->CR_f.SPIMD3 = pstcQspiInitCfg->enSpiMode;
        M4_QSPI->CR_f.PFE = pstcQspiInitCfg->enPrefetchFuncEn;
        M4_QSPI->CR_f.PFSAE = pstcQspiInitCfg->enPrefetchMode;
        M4_QSPI->CR_f.MDSEL = pstcQspiInitCfg->stcCommProtocol.enReadMode;

        /* Custom read mode */
        if ((QspiReadModeCustomFast == pstcQspiInitCfg->stcCommProtocol.enReadMode) ||
            (QspiReadModeCustomStandard == pstcQspiInitCfg->stcCommProtocol.enReadMode))
        {
            M4_QSPI->CR_f.IPRSL = pstcQspiInitCfg->stcCommProtocol.enTransInstrProtocol;
            M4_QSPI->CR_f.APRSL = pstcQspiInitCfg->stcCommProtocol.enTransAddrProtocol;
            M4_QSPI->CR_f.DPRSL = pstcQspiInitCfg->stcCommProtocol.enReceProtocol;
        }
        else
        {
            M4_QSPI->CR_f.IPRSL = QspiProtocolExtendSpi;
            M4_QSPI->CR_f.APRSL = QspiProtocolExtendSpi;
            M4_QSPI->CR_f.DPRSL = QspiProtocolExtendSpi;
        }

        /* Configure chip select control register */
        M4_QSPI->CSCR_f.SSNW = pstcQspiInitCfg->enQssnValidExtendTime;
        M4_QSPI->CSCR_f.SSHW = pstcQspiInitCfg->enQssnIntervalTime;

        /* Configure format control register */
        if (((pstcQspiInitCfg->enClkDiv % 2) != 0) &&
            (pstcQspiInitCfg->enQsckDutyCorr != QspiQsckDutyCorrNot))
        {
            M4_QSPI->FCR_f.DUTY = QspiQsckDutyCorrNot;
        }
        else
        {
            M4_QSPI->FCR_f.DUTY = pstcQspiInitCfg->enQsckDutyCorr;
        }
        M4_QSPI->FCR_f.DMCYCN = pstcQspiInitCfg->enVirtualPeriod;
        M4_QSPI->FCR_f.WPOL = pstcQspiInitCfg->enWpPinLevel;
        M4_QSPI->FCR_f.SSNLD = pstcQspiInitCfg->enQssnSetupDelayTime;
        M4_QSPI->FCR_f.SSNHD = pstcQspiInitCfg->enQssnHoldDelayTime;
        M4_QSPI->FCR_f.FOUR_BIC = pstcQspiInitCfg->enFourByteAddrReadEn;
        M4_QSPI->FCR_f.AWSL = pstcQspiInitCfg->enAddrWidth;
        M4_QSPI->CR_f.DCOME = pstcQspiInitCfg->enBusCommMode;

        /* Configure ROM access instruction */
        M4_QSPI->CCMD = pstcQspiInitCfg->u8RomAccessInstr;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Config communication protocol structure
 **
 ** \param [in] pstcCommProtocol        Pointer to qspi communication protocol configuration
 ** \arg See the struct #stc_qspi_comm_protocol_t
 **
 ** \retval Ok                          Process successfully done
 ** \retval Error                       Parameter error
 **
 ******************************************************************************/
en_result_t QSPI_CommProtocolConfig(const stc_qspi_comm_protocol_t *pstcCommProtocol)
{
    en_result_t enRet = Ok;

    if (NULL == pstcCommProtocol)
    {
        enRet = Error;
    }
    else
    {
        DDL_ASSERT(IS_VALID_RECE_DATA_PROTOCOL(pstcCommProtocol->enReceProtocol));
        DDL_ASSERT(IS_VALID_TRANS_ADDR_PROTOCOL(pstcCommProtocol->enTransAddrProtocol));
        DDL_ASSERT(IS_VALID_TRANS_INSTRUCT_PROTOCOL(pstcCommProtocol->enTransInstrProtocol));
        DDL_ASSERT(IS_VALID_INTERFACE_READ_MODE(pstcCommProtocol->enReadMode));

        M4_QSPI->CR_f.MDSEL = pstcCommProtocol->enReadMode;
        /* Custom read mode */
        if ((QspiReadModeCustomFast == pstcCommProtocol->enReadMode) ||
            (QspiReadModeCustomStandard == pstcCommProtocol->enReadMode))
        {
            M4_QSPI->CR_f.IPRSL = pstcCommProtocol->enTransInstrProtocol;
            M4_QSPI->CR_f.APRSL = pstcCommProtocol->enTransAddrProtocol;
            M4_QSPI->CR_f.DPRSL = pstcCommProtocol->enReceProtocol;
        }
        else
        {
            M4_QSPI->CR_f.IPRSL = QspiProtocolExtendSpi;
            M4_QSPI->CR_f.APRSL = QspiProtocolExtendSpi;
            M4_QSPI->CR_f.DPRSL = QspiProtocolExtendSpi;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enable or disable prefetch function
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable prefetch function
 ** \arg Enable                         Enable prefetch function
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_PrefetchCmd(en_functional_state_t enNewSta)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    M4_QSPI->CR_f.PFE = enNewSta;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set clock division
 **
 ** \param [in] enClkDiv                Clock division
 ** \arg QspiHclkDiv2                   Clock source: HCLK/2
 ** \arg QspiHclkDiv3                   Clock source: HCLK/3
 ** \arg QspiHclkDiv4                   Clock source: HCLK/4
 ** \arg QspiHclkDiv5                   Clock source: HCLK/5
 ** \arg QspiHclkDiv6                   Clock source: HCLK/6
 ** \arg QspiHclkDiv7                   Clock source: HCLK/7
 ** \arg QspiHclkDiv8                   Clock source: HCLK/8
 ** \arg QspiHclkDiv9                   Clock source: HCLK/9
 ** \arg QspiHclkDiv10                  Clock source: HCLK/10
 ** \arg QspiHclkDiv11                  Clock source: HCLK/11
 ** \arg QspiHclkDiv12                  Clock source: HCLK/12
 ** \arg QspiHclkDiv13                  Clock source: HCLK/13
 ** \arg QspiHclkDiv14                  Clock source: HCLK/14
 ** \arg QspiHclkDiv15                  Clock source: HCLK/15
 ** \arg QspiHclkDiv16                  Clock source: HCLK/16
 ** \arg QspiHclkDiv17                  Clock source: HCLK/17
 ** \arg QspiHclkDiv18                  Clock source: HCLK/18
 ** \arg QspiHclkDiv19                  Clock source: HCLK/19
 ** \arg QspiHclkDiv20                  Clock source: HCLK/20
 ** \arg QspiHclkDiv21                  Clock source: HCLK/21
 ** \arg QspiHclkDiv22                  Clock source: HCLK/22
 ** \arg QspiHclkDiv23                  Clock source: HCLK/23
 ** \arg QspiHclkDiv24                  Clock source: HCLK/24
 ** \arg QspiHclkDiv25                  Clock source: HCLK/25
 ** \arg QspiHclkDiv26                  Clock source: HCLK/26
 ** \arg QspiHclkDiv27                  Clock source: HCLK/27
 ** \arg QspiHclkDiv28                  Clock source: HCLK/28
 ** \arg QspiHclkDiv29                  Clock source: HCLK/29
 ** \arg QspiHclkDiv30                  Clock source: HCLK/30
 ** \arg QspiHclkDiv31                  Clock source: HCLK/31
 ** \arg QspiHclkDiv32                  Clock source: HCLK/32
 ** \arg QspiHclkDiv33                  Clock source: HCLK/33
 ** \arg QspiHclkDiv34                  Clock source: HCLK/34
 ** \arg QspiHclkDiv35                  Clock source: HCLK/35
 ** \arg QspiHclkDiv36                  Clock source: HCLK/36
 ** \arg QspiHclkDiv37                  Clock source: HCLK/37
 ** \arg QspiHclkDiv38                  Clock source: HCLK/38
 ** \arg QspiHclkDiv39                  Clock source: HCLK/39
 ** \arg QspiHclkDiv40                  Clock source: HCLK/40
 ** \arg QspiHclkDiv41                  Clock source: HCLK/41
 ** \arg QspiHclkDiv42                  Clock source: HCLK/42
 ** \arg QspiHclkDiv43                  Clock source: HCLK/43
 ** \arg QspiHclkDiv44                  Clock source: HCLK/44
 ** \arg QspiHclkDiv45                  Clock source: HCLK/45
 ** \arg QspiHclkDiv46                  Clock source: HCLK/46
 ** \arg QspiHclkDiv47                  Clock source: HCLK/47
 ** \arg QspiHclkDiv48                  Clock source: HCLK/48
 ** \arg QspiHclkDiv49                  Clock source: HCLK/49
 ** \arg QspiHclkDiv50                  Clock source: HCLK/50
 ** \arg QspiHclkDiv51                  Clock source: HCLK/51
 ** \arg QspiHclkDiv52                  Clock source: HCLK/52
 ** \arg QspiHclkDiv53                  Clock source: HCLK/53
 ** \arg QspiHclkDiv54                  Clock source: HCLK/54
 ** \arg QspiHclkDiv55                  Clock source: HCLK/55
 ** \arg QspiHclkDiv56                  Clock source: HCLK/56
 ** \arg QspiHclkDiv57                  Clock source: HCLK/57
 ** \arg QspiHclkDiv58                  Clock source: HCLK/58
 ** \arg QspiHclkDiv59                  Clock source: HCLK/59
 ** \arg QspiHclkDiv60                  Clock source: HCLK/60
 ** \arg QspiHclkDiv61                  Clock source: HCLK/61
 ** \arg QspiHclkDiv62                  Clock source: HCLK/62
 ** \arg QspiHclkDiv63                  Clock source: HCLK/63
 ** \arg QspiHclkDiv64                  Clock source: HCLK/64
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_SetClockDiv(en_qspi_clk_div_t enClkDiv)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_CLK_DIV(enClkDiv));

    M4_QSPI->CR_f.DIV = enClkDiv;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set WP Pin level
 **
 ** \param [in] enWpLevel               WP pin level
 ** \arg QspiWpPinOutputLow             WP pin(QIO2) output low level
 ** \arg QspiWpPinOutputHigh            WP pin(QIO2) output high level
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_SetWPPinLevel(en_qspi_wp_pin_level_t enWpLevel)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_WP_OUTPUT_LEVEL(enWpLevel));

    M4_QSPI->FCR_f.WPOL = enWpLevel;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set communication address width
 **
 ** \param [in] enAddrWidth             Communication address width
 ** \arg QspiAddressByteOne             One byte address
 ** \arg QspiAddressByteTwo             Two byte address
 ** \arg QspiAddressByteThree           Three byte address
 ** \arg QspiAddressByteFour            Four byte address
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_SetAddrWidth(en_qspi_addr_width_t enAddrWidth)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_INTERFACE_ADDR_WIDTH(enAddrWidth));

    M4_QSPI->FCR_f.AWSL = enAddrWidth;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set extend address value
 **
 ** \param [in] u8Addr                  Extend address value
 ** \arg 0~0x3F
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_SetExtendAddress(uint8_t u8Addr)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_SET_EXTEND_ADDR(u8Addr));

    M4_QSPI->EXAR_f.EXADR = u8Addr;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set rom access instruction
 **
 ** \param [in] u8Instr                 Rom access instruction
 ** \arg 0~0xFF
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_SetRomAccessInstruct(uint8_t u8Instr)
{
    en_result_t enRet = Ok;

    M4_QSPI->CCMD = u8Instr;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Write direct communication value
 **
 ** \param [in] u8Val                   Direct communication value
 ** \arg 0~0xFF
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_WriteDirectCommValue(uint8_t u8Val)
{
    en_result_t enRet = Ok;

    M4_QSPI->DCOM = u8Val;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Read direct communication value
 **
 ** \param [in] None
 **
 ** \retval uint8_t                     Direct communication read value
 **
 ******************************************************************************/
uint8_t QSPI_ReadDirectCommValue(void)
{
    return ((uint8_t)M4_QSPI->DCOM);
}

/**
 *******************************************************************************
 ** \brief Enable or disable xip mode
 **
 ** \param [in] u8Instr                 Enable or disable xip mode instruction
 ** \arg 0~0xFF
 **
 ** \param [in] enNewSta                The function new state
 ** \arg Disable                        Disable xip mode
 ** \arg Enable                         Enable xip mode
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_XipModeCmd(uint8_t u8Instr, en_functional_state_t enNewSta)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewSta));

    M4_QSPI->XCMD = u8Instr;
    if (Enable == enNewSta)
    {
        M4_QSPI->CR_f.XIPE = 1u;
    }
    else
    {
        M4_QSPI->CR_f.XIPE = 0u;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Enter direct communication mode
 **
 ** \param [in] None
 **
 ** \retval Ok                          Process successfully done
 **
 ** \note If you are in XIP mode, you need to exit XIP mode and then start direct communication mode.
 **
 ******************************************************************************/
en_result_t QSPI_EnterDirectCommMode(void)
{
    en_result_t enRet = Ok;

    M4_QSPI->CR_f.DCOME = 1u;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Exit direct communication mode
 **
 ** \param [in] None
 **
 ** \retval Ok                          Process successfully done
 **
 ******************************************************************************/
en_result_t QSPI_ExitDirectCommMode(void)
{
    en_result_t enRet = Ok;

    M4_QSPI->CR_f.DCOME = 0u;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get prefetch buffer current byte number
 **
 ** \param [in] None
 **
 ** \retval uint8_t                     Current buffer byte number
 **
 ******************************************************************************/
uint8_t QSPI_GetPrefetchBufferNum(void)
{
    return ((uint8_t)M4_QSPI->SR_f.PFNUM);
}

/**
 *******************************************************************************
 ** \brief Get flag status
 **
 ** \param [in] enFlag                  Choose need get status's flag
 ** \arg QspiFlagBusBusy                QSPI bus work status flag in direct communication mode
 ** \arg QspiFlagXipMode                XIP mode status signal
 ** \arg QspiFlagRomAccessError         Trigger rom access error flag in direct communication mode
 ** \arg QspiFlagPrefetchBufferFull     Prefetch buffer area status signal
 ** \arg QspiFlagPrefetchStop           Prefetch  action status signal
 **
 ** \retval Set                         Flag is set
 ** \retval Reset                       Flag is reset
 **
 ******************************************************************************/
en_flag_status_t QSPI_GetFlag(en_qspi_flag_type_t enFlag)
{
    en_flag_status_t enFlagSta = Reset;

    DDL_ASSERT(IS_VALID_GET_FLAG_TYPE(enFlag));

    switch (enFlag)
    {
    case QspiFlagBusBusy:
        enFlagSta = (en_flag_status_t)M4_QSPI->SR_f.BUSY;
        break;
    case QspiFlagXipMode:
        enFlagSta = (en_flag_status_t)M4_QSPI->SR_f.XIPF;
        break;
    case QspiFlagRomAccessError:
        enFlagSta = (en_flag_status_t)M4_QSPI->SR_f.RAER;
        break;
    case QspiFlagPrefetchBufferFull:
        enFlagSta = (en_flag_status_t)M4_QSPI->SR_f.PFFUL;
        break;
    case QspiFlagPrefetchStop:
        enFlagSta = (en_flag_status_t)M4_QSPI->SR_f.PFAN;
        break;
    default:
        break;
    }

    return enFlagSta;
}

/**
 *******************************************************************************
 ** \brief Clear flag status
 **
 ** \param [in] enFlag                  Choose need get status's flag
 ** \arg QspiFlagRomAccessError         Trigger rom access error flag in direct communication mode
 **
 ** \retval Ok                          Process successfully done
 ** \retval ErrorInvalidParameter       Parameter error
 **
 ******************************************************************************/
en_result_t QSPI_ClearFlag(en_qspi_flag_type_t enFlag)
{
    en_result_t enRet = Ok;

    if (QspiFlagRomAccessError == enFlag)
    {
        M4_QSPI->SR2_f.RAERCLR = 1u;
    }
    else
    {
        enRet = ErrorInvalidParameter;
    }

    return enRet;
}

//@} // QspiGroup

#endif /* DDL_QSPI_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
