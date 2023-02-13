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
/** \file hc32f46x_qspi.h
 **
 ** A detailed description is available at
 ** @link QspiGroup Queued SPI description @endlink
 **
 **   - 2018-11-20  1.0  Yangjp  First version for Device Driver Library of Qspi.
 **
 ******************************************************************************/
#ifndef __HC32F46x_QSPI_H__
#define __HC32F46x_QSPI_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_QSPI_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup QspiGroup Queued SPI(QSPI)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief QSPI spi protocol enumeration
     ******************************************************************************/
    typedef enum en_qspi_spi_protocol
    {
        QspiProtocolExtendSpi = 0u,    ///< Extend spi protocol
        QspiProtocolTwoWiresSpi = 1u,  ///< Two wires spi protocol
        QspiProtocolFourWiresSpi = 2u, ///< Four wires spi protocol
    } en_qspi_spi_protocol_t;

    /**
     *******************************************************************************
     ** \brief QSPI spi Mode enumeration
     ******************************************************************************/
    typedef enum en_qspi_spi_mode
    {
        QspiSpiMode0 = 0u, ///< Spi mode 0(QSCK normalcy is Low level)
        QspiSpiMode3 = 1u, ///< Spi mode 3(QSCK normalcy is High level)
    } en_qspi_spi_mode_t;

    /**
     *******************************************************************************
     ** \brief QSPI bus communication mode enumeration
     ******************************************************************************/
    typedef enum en_qspi_bus_mode
    {
        QspiBusModeRomAccess = 0u,    ///< Rom access mode
        QspiBusModeDirectAccess = 1u, ///< Direct communication mode
    } en_qspi_bus_mode_t;

    /**
     *******************************************************************************
     ** \brief QSPI prefetch data stop config enumeration
     ******************************************************************************/
    typedef enum en_qspi_prefetch_config
    {
        QspiPrefetchStopComplete = 0u,    ///< Stop after prefetch data complete
        QspiPrefetchStopImmediately = 1u, ///< Immediately stop prefetch
    } en_qspi_prefetch_config_t;

    /**
     *******************************************************************************
     ** \brief QSPI read mode enumeration
     ******************************************************************************/
    typedef enum en_qspi_read_mode
    {
        QspiReadModeStandard = 0u,        ///< Standard read
        QspiReadModeFast = 1u,            ///< Fast read
        QspiReadModeTwoWiresOutput = 2u,  ///< Two wires output fast read
        QspiReadModeTwoWiresIO = 3u,      ///< Two wires input/output fast read
        QspiReadModeFourWiresOutput = 4u, ///< Four wires output fast read
        QspiReadModeFourWiresIO = 5u,     ///< Four wires input/output fast read
        QspiReadModeCustomStandard = 6u,  ///< Custom standard read
        QspiReadModeCustomFast = 7u,      ///< Custom fast read
    } en_qspi_read_mode_t;

    /**
     *******************************************************************************
     ** \brief QSPI QSSN valid extend time enumeration
     ******************************************************************************/
    typedef enum en_qspi_qssn_valid_extend_time
    {
        QspiQssnValidExtendNot = 0u,     ///< Don't extend QSSN valid time
        QspiQssnValidExtendSck32 = 1u,   ///< QSSN valid time extend 32 QSCK cycles
        QspiQssnValidExtendSck128 = 2u,  ///< QSSN valid time extend 138 QSCK cycles
        QspiQssnValidExtendSckEver = 3u, ///< QSSN valid time extend to ever
    } en_qspi_qssn_valid_extend_time_t;

    /**
     *******************************************************************************
     ** \brief QSPI QSCK duty cycle correction enumeration
     ******************************************************************************/
    typedef enum en_qspi_qsck_duty_correction
    {
        QspiQsckDutyCorrNot = 0u,      ///< Don't correction duty cycle
        QspiQsckDutyCorrHalfHclk = 1u, ///< QSCK's rising edge delay 0.5 HCLK cycle when Qsck select HCLK is odd
    } en_qspi_qsck_duty_correction_t;

    /**
     *******************************************************************************
     ** \brief QSPI WP Pin output level enumeration
     ******************************************************************************/
    typedef enum en_qspi_wp_pin_level
    {
        QspiWpPinOutputLow = 0u,  ///< WP pin(QIO2) output low level
        QspiWpPinOutputHigh = 1u, ///< WP pin(QIO2) output high level
    } en_qspi_wp_pin_level_t;

    /**
     *******************************************************************************
     ** \brief QSPI QSSN setup delay time enumeration
     ******************************************************************************/
    typedef enum en_qspi_qssn_setup_delay
    {
        QspiQssnSetupDelayHalfQsck = 0u,  ///< QSSN setup delay 0.5 QSCK output than QSCK first rising edge
        QspiQssnSetupDelay1Dot5Qsck = 1u, ///< QSSN setup delay 1.5 QSCK output than QSCK first rising edge
    } en_qspi_qssn_setup_delay_t;

    /**
     *******************************************************************************
     ** \brief QSPI QSSN hold delay time enumeration
     ******************************************************************************/
    typedef enum en_qspi_qssn_hold_delay
    {
        QspiQssnHoldDelayHalfQsck = 0u,  ///< QSSN hold delay 0.5 QSCK release than QSCK last rising edge
        QspiQssnHoldDelay1Dot5Qsck = 1u, ///< QSSN hold delay 1.5 QSCK release than QSCK last rising edge
    } en_qspi_qssn_hold_delay_t;

    /**
     *******************************************************************************
     ** \brief QSPI address width enumeration
     ******************************************************************************/
    typedef enum en_qspi_addr_width
    {
        QspiAddressByteOne = 0u,   ///< One byte address
        QspiAddressByteTwo = 1u,   ///< Two byte address
        QspiAddressByteThree = 2u, ///< Three byte address
        QspiAddressByteFour = 3u,  ///< Four byte address
    } en_qspi_addr_width_t;

    /**
     *******************************************************************************
     ** \brief QSPI flag type enumeration
     ******************************************************************************/
    typedef enum en_qspi_flag_type
    {
        QspiFlagBusBusy = 0u,            ///< QSPI bus work status flag in direct communication mode
        QspiFlagXipMode = 1u,            ///< XIP mode status signal
        QspiFlagRomAccessError = 2u,     ///< Trigger rom access error flag in direct communication mode
        QspiFlagPrefetchBufferFull = 3u, ///< Prefetch buffer area status signal
        QspiFlagPrefetchStop = 4u,       ///< Prefetch  action status signal
    } en_qspi_flag_type_t;

    /**
     *******************************************************************************
     ** \brief QSPI clock division enumeration
     ******************************************************************************/
    typedef enum en_qspi_clk_div
    {
        QspiHclkDiv2 = 0u,   ///< Clock source: HCLK/2
        QspiHclkDiv3 = 2u,   ///< Clock source: HCLK/3
        QspiHclkDiv4 = 3u,   ///< Clock source: HCLK/4
        QspiHclkDiv5 = 4u,   ///< Clock source: HCLK/5
        QspiHclkDiv6 = 5u,   ///< Clock source: HCLK/6
        QspiHclkDiv7 = 6u,   ///< Clock source: HCLK/7
        QspiHclkDiv8 = 7u,   ///< Clock source: HCLK/8
        QspiHclkDiv9 = 8u,   ///< Clock source: HCLK/9
        QspiHclkDiv10 = 9u,  ///< Clock source: HCLK/10
        QspiHclkDiv11 = 10u, ///< Clock source: HCLK/11
        QspiHclkDiv12 = 11u, ///< Clock source: HCLK/12
        QspiHclkDiv13 = 12u, ///< Clock source: HCLK/13
        QspiHclkDiv14 = 13u, ///< Clock source: HCLK/14
        QspiHclkDiv15 = 14u, ///< Clock source: HCLK/15
        QspiHclkDiv16 = 15u, ///< Clock source: HCLK/16
        QspiHclkDiv17 = 16u, ///< Clock source: HCLK/17
        QspiHclkDiv18 = 17u, ///< Clock source: HCLK/18
        QspiHclkDiv19 = 18u, ///< Clock source: HCLK/19
        QspiHclkDiv20 = 19u, ///< Clock source: HCLK/20
        QspiHclkDiv21 = 20u, ///< Clock source: HCLK/21
        QspiHclkDiv22 = 21u, ///< Clock source: HCLK/22
        QspiHclkDiv23 = 22u, ///< Clock source: HCLK/23
        QspiHclkDiv24 = 23u, ///< Clock source: HCLK/24
        QspiHclkDiv25 = 24u, ///< Clock source: HCLK/25
        QspiHclkDiv26 = 25u, ///< Clock source: HCLK/26
        QspiHclkDiv27 = 26u, ///< Clock source: HCLK/27
        QspiHclkDiv28 = 27u, ///< Clock source: HCLK/28
        QspiHclkDiv29 = 28u, ///< Clock source: HCLK/29
        QspiHclkDiv30 = 29u, ///< Clock source: HCLK/30
        QspiHclkDiv31 = 30u, ///< Clock source: HCLK/31
        QspiHclkDiv32 = 31u, ///< Clock source: HCLK/32
        QspiHclkDiv33 = 32u, ///< Clock source: HCLK/33
        QspiHclkDiv34 = 33u, ///< Clock source: HCLK/34
        QspiHclkDiv35 = 34u, ///< Clock source: HCLK/35
        QspiHclkDiv36 = 35u, ///< Clock source: HCLK/36
        QspiHclkDiv37 = 36u, ///< Clock source: HCLK/37
        QspiHclkDiv38 = 37u, ///< Clock source: HCLK/38
        QspiHclkDiv39 = 38u, ///< Clock source: HCLK/39
        QspiHclkDiv40 = 39u, ///< Clock source: HCLK/40
        QspiHclkDiv41 = 40u, ///< Clock source: HCLK/41
        QspiHclkDiv42 = 41u, ///< Clock source: HCLK/42
        QspiHclkDiv43 = 42u, ///< Clock source: HCLK/43
        QspiHclkDiv44 = 43u, ///< Clock source: HCLK/44
        QspiHclkDiv45 = 44u, ///< Clock source: HCLK/45
        QspiHclkDiv46 = 45u, ///< Clock source: HCLK/46
        QspiHclkDiv47 = 46u, ///< Clock source: HCLK/47
        QspiHclkDiv48 = 47u, ///< Clock source: HCLK/48
        QspiHclkDiv49 = 48u, ///< Clock source: HCLK/49
        QspiHclkDiv50 = 49u, ///< Clock source: HCLK/50
        QspiHclkDiv51 = 50u, ///< Clock source: HCLK/51
        QspiHclkDiv52 = 51u, ///< Clock source: HCLK/52
        QspiHclkDiv53 = 52u, ///< Clock source: HCLK/53
        QspiHclkDiv54 = 53u, ///< Clock source: HCLK/54
        QspiHclkDiv55 = 54u, ///< Clock source: HCLK/55
        QspiHclkDiv56 = 55u, ///< Clock source: HCLK/56
        QspiHclkDiv57 = 56u, ///< Clock source: HCLK/57
        QspiHclkDiv58 = 57u, ///< Clock source: HCLK/58
        QspiHclkDiv59 = 58u, ///< Clock source: HCLK/59
        QspiHclkDiv60 = 59u, ///< Clock source: HCLK/60
        QspiHclkDiv61 = 60u, ///< Clock source: HCLK/61
        QspiHclkDiv62 = 61u, ///< Clock source: HCLK/62
        QspiHclkDiv63 = 62u, ///< Clock source: HCLK/63
        QspiHclkDiv64 = 63u, ///< Clock source: HCLK/64
    } en_qspi_clk_div_t;

    /**
     *******************************************************************************
     ** \brief QSPI QSSN minimum interval time enumeration
     ******************************************************************************/
    typedef enum en_qspi_qssn_interval_time
    {
        QspiQssnIntervalQsck1 = 0u,   ///< QSSN signal min interval time 1 QSCK
        QspiQssnIntervalQsck2 = 1u,   ///< QSSN signal min interval time 2 QSCK
        QspiQssnIntervalQsck3 = 2u,   ///< QSSN signal min interval time 3 QSCK
        QspiQssnIntervalQsck4 = 3u,   ///< QSSN signal min interval time 4 QSCK
        QspiQssnIntervalQsck5 = 4u,   ///< QSSN signal min interval time 5 QSCK
        QspiQssnIntervalQsck6 = 5u,   ///< QSSN signal min interval time 6 QSCK
        QspiQssnIntervalQsck7 = 6u,   ///< QSSN signal min interval time 7 QSCK
        QspiQssnIntervalQsck8 = 7u,   ///< QSSN signal min interval time 8 QSCK
        QspiQssnIntervalQsck9 = 8u,   ///< QSSN signal min interval time 9 QSCK
        QspiQssnIntervalQsck10 = 9u,  ///< QSSN signal min interval time 10 QSCK
        QspiQssnIntervalQsck11 = 10u, ///< QSSN signal min interval time 11 QSCK
        QspiQssnIntervalQsck12 = 11u, ///< QSSN signal min interval time 12 QSCK
        QspiQssnIntervalQsck13 = 12u, ///< QSSN signal min interval time 13 QSCK
        QspiQssnIntervalQsck14 = 13u, ///< QSSN signal min interval time 14 QSCK
        QspiQssnIntervalQsck15 = 14u, ///< QSSN signal min interval time 15 QSCK
        QspiQssnIntervalQsck16 = 15u, ///< QSSN signal min interval time 16 QSCK
    } en_qspi_qssn_interval_time_t;

    /**
     *******************************************************************************
     ** \brief QSPI virtual period enumeration
     ******************************************************************************/
    typedef enum en_qspi_virtual_period
    {
        QspiVirtualPeriodQsck3 = 0u,   ///< Virtual period select 3 QSCK
        QspiVirtualPeriodQsck4 = 1u,   ///< Virtual period select 4 QSCK
        QspiVirtualPeriodQsck5 = 2u,   ///< Virtual period select 5 QSCK
        QspiVirtualPeriodQsck6 = 3u,   ///< Virtual period select 6 QSCK
        QspiVirtualPeriodQsck7 = 4u,   ///< Virtual period select 7 QSCK
        QspiVirtualPeriodQsck8 = 5u,   ///< Virtual period select 8 QSCK
        QspiVirtualPeriodQsck9 = 6u,   ///< Virtual period select 9 QSCK
        QspiVirtualPeriodQsck10 = 7u,  ///< Virtual period select 10 QSCK
        QspiVirtualPeriodQsck11 = 8u,  ///< Virtual period select 11 QSCK
        QspiVirtualPeriodQsck12 = 9u,  ///< Virtual period select 12 QSCK
        QspiVirtualPeriodQsck13 = 10u, ///< Virtual period select 13 QSCK
        QspiVirtualPeriodQsck14 = 11u, ///< Virtual period select 14 QSCK
        QspiVirtualPeriodQsck15 = 12u, ///< Virtual period select 15 QSCK
        QspiVirtualPeriodQsck16 = 13u, ///< Virtual period select 16 QSCK
        QspiVirtualPeriodQsck17 = 14u, ///< Virtual period select 17 QSCK
        QspiVirtualPeriodQsck18 = 15u, ///< Virtual period select 18 QSCK
    } en_qspi_virtual_period_t;

    /**
     *******************************************************************************
     ** \brief QSPI communication protocol structure definition
     ******************************************************************************/
    typedef struct stc_qspi_comm_protocol
    {
        en_qspi_spi_protocol_t enReceProtocol;       ///< Receive data stage SPI protocol
        en_qspi_spi_protocol_t enTransAddrProtocol;  ///< Transmit address stage SPI protocol
        en_qspi_spi_protocol_t enTransInstrProtocol; ///< Transmit instruction stage SPI protocol
        en_qspi_read_mode_t enReadMode;              ///< Serial interface read mode
    } stc_qspi_comm_protocol_t;

    /**
     *******************************************************************************
     ** \brief QSPI init structure definition
     ******************************************************************************/
    typedef struct stc_qspi_init
    {
        en_qspi_clk_div_t enClkDiv;                             ///< Clock division
        en_qspi_spi_mode_t enSpiMode;                           ///< Specifies SPI mode
        en_qspi_bus_mode_t enBusCommMode;                       ///< Bus communication mode
        en_qspi_prefetch_config_t enPrefetchMode;               ///< Config prefetch stop location
        en_functional_state_t enPrefetchFuncEn;                 ///< Disable: disable prefetch function, Enable: enable prefetch function
        stc_qspi_comm_protocol_t stcCommProtocol;               ///< Qspi communication protocol config
        en_qspi_qssn_valid_extend_time_t enQssnValidExtendTime; ///< QSSN valid extend time function select after QSPI access bus
        en_qspi_qssn_interval_time_t enQssnIntervalTime;        ///< QSSN minimum interval time
        en_qspi_qsck_duty_correction_t enQsckDutyCorr;          ///< QSCK output duty cycles correction
        en_qspi_virtual_period_t enVirtualPeriod;               ///< Virtual period config
        en_qspi_wp_pin_level_t enWpPinLevel;                    ///< WP pin(QIO2) level select
        en_qspi_qssn_setup_delay_t enQssnSetupDelayTime;        ///< QSSN setup delay time choose
        en_qspi_qssn_hold_delay_t enQssnHoldDelayTime;          ///< QSSN hold delay time choose
        en_functional_state_t enFourByteAddrReadEn;             ///< Read instruction code select when set address width is four bytes
        en_qspi_addr_width_t enAddrWidth;                       ///< Serial interface address width choose
        uint8_t u8RomAccessInstr;                               ///< Rom access mode instruction
    } stc_qspi_init_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*!< 4-byte instruction mode using instruction set */
#define QSPI_4BINSTR_STANDARD_READ 0x13u
#define QSPI_4BINSTR_FAST_READ 0x0Cu
#define QSPI_4BINSTR_TWO_WIRES_OUTPUT_READ 0x3Cu
#define QSPI_4BINSTR_TWO_WIRES_IO_READ 0xBCu
#define QSPI_4BINSTR_FOUR_WIRES_OUTPUT_READ 0x6Cu
#define QSPI_4BINSTR_FOUR_WIRES_IO_READ 0xECu
#define QSPI_4BINSTR_EXIT_4BINSTR_MODE 0xB7u

/*!< 3-byte instruction mode using instruction set */
#define QSPI_3BINSTR_STANDARD_READ 0x03u
#define QSPI_3BINSTR_FAST_READ 0x0Bu
#define QSPI_3BINSTR_TWO_WIRES_OUTPUT_READ 0x3Bu
#define QSPI_3BINSTR_TWO_WIRES_IO_READ 0xBBu
#define QSPI_3BINSTR_FOUR_WIRES_OUTPUT_READ 0x6Bu
#define QSPI_3BINSTR_FOUR_WIRES_IO_READ 0xEBu
#define QSPI_3BINSTR_ENTER_4BINSTR_MODE 0xE9u

/*!< General instruction set */
#define QSPI_WRITE_MODE_ENABLE 0x06u

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    /* Base functions */
    en_result_t QSPI_DeInit(void);
    en_result_t QSPI_Init(const stc_qspi_init_t *pstcQspiInitCfg);
    en_result_t QSPI_SetAddrWidth(en_qspi_addr_width_t enAddrWidth);
    en_result_t QSPI_SetExtendAddress(uint8_t u8Addr);
    en_result_t QSPI_CommProtocolConfig(const stc_qspi_comm_protocol_t *pstcCommProtocol);
    en_result_t QSPI_PrefetchCmd(en_functional_state_t enNewSta);
    en_result_t QSPI_SetClockDiv(en_qspi_clk_div_t enClkDiv);
    en_result_t QSPI_SetWPPinLevel(en_qspi_wp_pin_level_t enWpLevel);

    /* Rom access mode functions */
    en_result_t QSPI_SetRomAccessInstruct(uint8_t u8Instr);
    en_result_t QSPI_XipModeCmd(uint8_t u8Instr, en_functional_state_t enNewSta);

    /* Direct communication mode functions */
    en_result_t QSPI_WriteDirectCommValue(uint8_t u8Val);
    uint8_t QSPI_ReadDirectCommValue(void);
    en_result_t QSPI_EnterDirectCommMode(void);
    en_result_t QSPI_ExitDirectCommMode(void);

    /* Flags and get buffer functions */
    uint8_t QSPI_GetPrefetchBufferNum(void);
    en_flag_status_t QSPI_GetFlag(en_qspi_flag_type_t enFlag);
    en_result_t QSPI_ClearFlag(en_qspi_flag_type_t enFlag);

    //@} // QspiGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_QSPI_ENABLE */

#endif /* __HC32F46x_QSPI_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
