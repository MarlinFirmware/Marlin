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
/** \file hc32f46x_spi.h
 **
 ** A detailed description is available at
 ** @link SpiGroup Serial Peripheral Interface description @endlink
 **
 **   - 2018-10-29  1.0  Yangjp  First version for Device Driver Library of Spi.
 **
 ******************************************************************************/
#ifndef __HC32F46x_SPI_H__
#define __HC32F46x_SPI_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_SPI_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup SpiGroup Serial Peripheral Interface(SPI)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief SPI parity enumeration
     ******************************************************************************/
    typedef enum en_spi_parity
    {
        SpiParityEven = 0u, ///< Select even parity send and receive
        SpiParityOdd = 1u,  ///< Select odd parity send and receive
    } en_spi_parity_t;

    /**
     *******************************************************************************
     ** \brief SPI master/slave mode enumeration
     ******************************************************************************/
    typedef enum en_spi_master_slave_mode
    {
        SpiModeSlave = 0u,  ///< Spi slave mode
        SpiModeMaster = 1u, ///< Spi master mode
    } en_spi_master_slave_mode_t;

    /**
     *******************************************************************************
     ** \brief SPI transmission mode enumeration
     ******************************************************************************/
    typedef enum en_spi_trans_mode
    {
        SpiTransFullDuplex = 0u, ///< Full duplex sync serial communication
        SpiTransOnlySend = 1u,   ///< Only send serial communication
    } en_spi_trans_mode_t;

    /**
     *******************************************************************************
     ** \brief SPI work mode enumeration
     ******************************************************************************/
    typedef enum en_spi_work_mode
    {
        SpiWorkMode4Line = 0u, ///< 4 lines spi work mode
        SpiWorkMode3Line = 1u, ///< 3 lines spi work mode(clock sync running)
    } en_spi_work_mode_t;

    /**
     *******************************************************************************
     ** \brief SPI SS interval time enumeration
     ******************************************************************************/
    typedef enum en_spi_ss_interval_time
    {
        SpiSsIntervalSck1PlusPck2 = 0u, ///< Spi SS interval time 1 SCK plus 2 PCLK1
        SpiSsIntervalSck2PlusPck2 = 1u, ///< Spi SS interval time 2 SCK plus 2 PCLK1
        SpiSsIntervalSck3PlusPck2 = 2u, ///< Spi SS interval time 3 SCK plus 2 PCLK1
        SpiSsIntervalSck4PlusPck2 = 3u, ///< Spi SS interval time 4 SCK plus 2 PCLK1
        SpiSsIntervalSck5PlusPck2 = 4u, ///< Spi SS interval time 5 SCK plus 2 PCLK1
        SpiSsIntervalSck6PlusPck2 = 5u, ///< Spi SS interval time 6 SCK plus 2 PCLK1
        SpiSsIntervalSck7PlusPck2 = 6u, ///< Spi SS interval time 7 SCK plus 2 PCLK1
        SpiSsIntervalSck8PlusPck2 = 7u, ///< Spi SS interval time 8 SCK plus 2 PCLK1
    } en_spi_ss_interval_time_t;

    /**
     *******************************************************************************
     ** \brief SPI SS setup delay SCK enumeration
     ******************************************************************************/
    typedef enum en_spi_ss_setup_delay
    {
        SpiSsSetupDelaySck1 = 0u, ///< Spi SS setup delay 1 SCK
        SpiSsSetupDelaySck2 = 1u, ///< Spi SS setup delay 2 SCK
        SpiSsSetupDelaySck3 = 2u, ///< Spi SS setup delay 3 SCK
        SpiSsSetupDelaySck4 = 3u, ///< Spi SS setup delay 4 SCK
        SpiSsSetupDelaySck5 = 4u, ///< Spi SS setup delay 5 SCK
        SpiSsSetupDelaySck6 = 5u, ///< Spi SS setup delay 6 SCK
        SpiSsSetupDelaySck7 = 6u, ///< Spi SS setup delay 7 SCK
        SpiSsSetupDelaySck8 = 7u, ///< Spi SS setup delay 8 SCK
    } en_spi_ss_setup_delay_t;

    /**
     *******************************************************************************
     ** \brief SPI SS hold delay SCK enumeration
     ******************************************************************************/
    typedef enum en_spi_ss_hold_delay
    {
        SpiSsHoldDelaySck1 = 0u, ///< Spi SS hold delay 1 SCK
        SpiSsHoldDelaySck2 = 1u, ///< Spi SS hold delay 2 SCK
        SpiSsHoldDelaySck3 = 2u, ///< Spi SS hold delay 3 SCK
        SpiSsHoldDelaySck4 = 3u, ///< Spi SS hold delay 4 SCK
        SpiSsHoldDelaySck5 = 4u, ///< Spi SS hold delay 5 SCK
        SpiSsHoldDelaySck6 = 5u, ///< Spi SS hold delay 6 SCK
        SpiSsHoldDelaySck7 = 6u, ///< Spi SS hold delay 7 SCK
        SpiSsHoldDelaySck8 = 7u, ///< Spi SS hold delay 8 SCK
    } en_spi_ss_hold_delay_t;

    /**
     *******************************************************************************
     ** \brief SPI slave select polarity enumeration
     ******************************************************************************/
    typedef enum en_spi_ss_polarity
    {
        SpiSsLowValid = 0u,  ///< SS0~3 signal low level valid
        SpiSsHighValid = 1u, ///< SS0~3 signal high level valid
    } en_spi_ss_polarity_t;

    /**
     *******************************************************************************
     ** \brief SPI data register read object enumeration
     ******************************************************************************/
    typedef enum en_spi_read_object
    {
        SpiReadReceiverBuffer = 0u, ///< Read receive buffer
        SpiReadSendBuffer = 1u,     ///< Read send buffer(must be read when TDEF=1)
    } en_spi_read_object_t;

    /**
     *******************************************************************************
     ** \brief SPI frame number enumeration
     ******************************************************************************/
    typedef enum en_spi_frame_number
    {
        SpiFrameNumber1 = 0u, ///< 1 frame data
        SpiFrameNumber2 = 1u, ///< 2 frame data
        SpiFrameNumber3 = 2u, ///< 3 frame data
        SpiFrameNumber4 = 3u, ///< 4 frame data
    } en_spi_frame_number_t;

    /**
     *******************************************************************************
     ** \brief SPI SS setup delay SCK option enumeration
     ******************************************************************************/
    typedef enum en_spi_ss_setup_delay_option
    {
        SpiSsSetupDelayTypicalSck1 = 0u, ///< SS setup delay 1 SCK
        SpiSsSetupDelayCustomValue = 1u, ///< SS setup delay SCKDL register set value
    } en_spi_ss_setup_delay_option_t;

    /**
     *******************************************************************************
     ** \brief SPI SS hold delay SCK option enumeration
     ******************************************************************************/
    typedef enum en_spi_ss_hold_delay_option
    {
        SpiSsHoldDelayTypicalSck1 = 0u, ///< SS hold delay 1 SCK
        SpiSsHoldDelayCustomValue = 1u, ///< SS hold delay SSDL register set value
    } en_spi_ss_hold_delay_option_t;

    /**
     *******************************************************************************
     ** \brief SPI SS interval time option enumeration
     ******************************************************************************/
    typedef enum en_spi_ss_interval_time_option
    {
        SpiSsIntervalTypicalSck1PlusPck2 = 0u, ///< Spi SS interval time 1 SCK plus 2 PCLK1
        SpiSsIntervalCustomValue = 1u,         ///< Spi SS interval time NXTDL register set value
    } en_spi_ss_interval_time_option_t;

    /**
     *******************************************************************************
     ** \brief SPI first bit position enumeration
     ******************************************************************************/
    typedef enum en_spi_first_bit_position
    {
        SpiFirstBitPositionMSB = 0u, ///< Spi first bit to MSB
        SpiFirstBitPositionLSB = 1u, ///< Spi first bit to LSB
    } en_spi_first_bit_position_t;

    /**
     *******************************************************************************
     ** \brief SPI data length enumeration
     ******************************************************************************/
    typedef enum en_spi_data_length
    {
        SpiDataLengthBit4 = 0u,   ///< 4 bits
        SpiDataLengthBit5 = 1u,   ///< 5 bits
        SpiDataLengthBit6 = 2u,   ///< 6 bits
        SpiDataLengthBit7 = 3u,   ///< 7 bits
        SpiDataLengthBit8 = 4u,   ///< 8 bits
        SpiDataLengthBit9 = 5u,   ///< 9 bits
        SpiDataLengthBit10 = 6u,  ///< 10 bits
        SpiDataLengthBit11 = 7u,  ///< 11 bits
        SpiDataLengthBit12 = 8u,  ///< 12 bits
        SpiDataLengthBit13 = 9u,  ///< 13 bits
        SpiDataLengthBit14 = 10u, ///< 14 bits
        SpiDataLengthBit15 = 11u, ///< 15 bits
        SpiDataLengthBit16 = 12u, ///< 16 bits
        SpiDataLengthBit20 = 13u, ///< 20 bits
        SpiDataLengthBit24 = 14u, ///< 24 bits
        SpiDataLengthBit32 = 15u, ///< 32 bits
    } en_spi_data_length_t;

    /**
     *******************************************************************************
     ** \brief SPI SS valid channel select enumeration
     ******************************************************************************/
    typedef enum en_spi_ss_valid_channel
    {
        SpiSsValidChannel0 = 0u, ///< Select SS0 valid
        SpiSsValidChannel1 = 1u, ///< Select SS1 valid
        SpiSsValidChannel2 = 2u, ///< Select SS2 valid
        SpiSsValidChannel3 = 3u, ///< Select SS3 valid
    } en_spi_ss_valid_channel_t;

    /**
     *******************************************************************************
     ** \brief SPI clock division enumeration
     ******************************************************************************/
    typedef enum en_spi_clk_div
    {
        SpiClkDiv2 = 0u,   ///< Spi pckl1 division 2
        SpiClkDiv4 = 1u,   ///< Spi pckl1 division 4
        SpiClkDiv8 = 2u,   ///< Spi pckl1 division 8
        SpiClkDiv16 = 3u,  ///< Spi pckl1 division 16
        SpiClkDiv32 = 4u,  ///< Spi pckl1 division 32
        SpiClkDiv64 = 5u,  ///< Spi pckl1 division 64
        SpiClkDiv128 = 6u, ///< Spi pckl1 division 128
        SpiClkDiv256 = 7u, ///< Spi pckl1 division 256
    } en_spi_clk_div_t;

    /**
     *******************************************************************************
     ** \brief SPI SCK polarity enumeration
     ******************************************************************************/
    typedef enum en_spi_sck_polarity
    {
        SpiSckIdleLevelLow = 0u,  ///< SCK is low level when SCK idle
        SpiSckIdelLevelHigh = 1u, ///< SCK is high level when SCK idle
    } en_spi_sck_polarity_t;

    /**
     *******************************************************************************
     ** \brief SPI SCK phase enumeration
     ******************************************************************************/
    typedef enum en_spi_sck_phase
    {
        SpiSckOddSampleEvenChange = 0u, ///< SCK Odd edge data sample,even edge data change
        SpiSckOddChangeEvenSample = 1u, ///< SCK Odd edge data change,even edge data sample
    } en_spi_sck_phase_t;

    /**
     *******************************************************************************
     ** \brief SPI interrupt request type enumeration
     ******************************************************************************/
    typedef enum en_spi_irq_type
    {
        SpiIrqIdel = 0u,    ///< Spi idle interrupt request
        SpiIrqReceive = 1u, ///< Spi receive interrupt request
        SpiIrqSend = 2u,    ///< Spi send interrupt request
        SpiIrqError = 3u,   ///< Spi error interrupt request
    } en_spi_irq_type_t;

    /**
     *******************************************************************************
     ** \brief SPI flag type enumeration
     ******************************************************************************/
    typedef enum en_spi_flag_type
    {
        SpiFlagReceiveBufferFull = 0u, ///< Receive buffer full flag
        SpiFlagSendBufferEmpty = 1u,   ///< Send buffer empty flag
        SpiFlagUnderloadError = 2u,    ///< Underload error flag
        SpiFlagParityError = 3u,       ///< Parity error flag
        SpiFlagModeFaultError = 4u,    ///< Mode fault error flag
        SpiFlagSpiIdle = 5u,           ///< SPI idle flag
        SpiFlagOverloadError = 6u,     ///< Overload error flag
    } en_spi_flag_type_t;

    /**
     *******************************************************************************
     ** \brief SPI SS channel enumeration
     ******************************************************************************/
    typedef enum en_spi_ss_channel
    {
        SpiSsChannel0 = 0u, ///< SS0 channel
        SpiSsChannel1 = 1u, ///< SS1 channel
        SpiSsChannel2 = 2u, ///< SS2 channel
        SpiSsChannel3 = 3u, ///< SS3 channel
    } en_spi_ss_channel_t;

    /**
     *******************************************************************************
     ** \brief SPI bus delay structure definition
     **
     ** \note Slave mode stc_spi_delay_config_t is invalid
     ******************************************************************************/
    typedef struct stc_spi_delay_config
    {
        en_spi_ss_setup_delay_option_t enSsSetupDelayOption;     ///< SS setup delay time option
        en_spi_ss_setup_delay_t enSsSetupDelayTime;              ///< SS setup delay time(the value valid when enSsSetupDelayOption is custom)
        en_spi_ss_hold_delay_option_t enSsHoldDelayOption;       ///< SS hold delay time option
        en_spi_ss_hold_delay_t enSsHoldDelayTime;                ///< SS hold delay time(the value valid when enSsHoldDelayOption is custom)
        en_spi_ss_interval_time_option_t enSsIntervalTimeOption; ///< SS interval time option
        en_spi_ss_interval_time_t enSsIntervalTime;              ///< SS interval time(the value valid when enSsIntervalTimeOption is custom)
    } stc_spi_delay_config_t;

    /**
     *******************************************************************************
     ** \brief SPI SS config structure definition
     **
     ** \note 3 lines mode stc_spi_ss_config_t is invalid
     ******************************************************************************/
    typedef struct stc_spi_ss_config
    {
        en_spi_ss_valid_channel_t enSsValidBit; ///< SS valid channel select
        en_spi_ss_polarity_t enSs0Polarity;     ///< SS0 signal polarity
        en_spi_ss_polarity_t enSs1Polarity;     ///< SS1 signal polarity
        en_spi_ss_polarity_t enSs2Polarity;     ///< SS2 signal polarity
        en_spi_ss_polarity_t enSs3Polarity;     ///< SS3 signal polarity
    } stc_spi_ss_config_t;

    /**
     *******************************************************************************
     ** \brief SPI init structure definition
     ******************************************************************************/
    typedef struct stc_spi_init_t
    {
        stc_spi_delay_config_t stcDelayConfig;          ///< SPI delay structure(Slave mode is invalid)
        stc_spi_ss_config_t stcSsConfig;                ///< SS polarity and channel structure(3 lines mode invalid)
        en_spi_read_object_t enReadBufferObject;        ///< Data register read object select(must be read when TDEF=1)
        en_spi_sck_polarity_t enSckPolarity;            ///< Sck polarity
        en_spi_sck_phase_t enSckPhase;                  ///< Sck phase(This value must be SpiSckOddChangeEvenSample in 3-line mode)
        en_spi_clk_div_t enClkDiv;                      ///< SPI clock division
        en_spi_data_length_t enDataLength;              ///< Data length
        en_spi_first_bit_position_t enFirstBitPosition; ///< Data first bit position
        en_spi_frame_number_t enFrameNumber;            ///< Data frame number
        en_spi_work_mode_t enWorkMode;                  ///< Spi work mode
        en_spi_trans_mode_t enTransMode;                ///< transmission mode
        en_spi_master_slave_mode_t enMasterSlaveMode;   ///< Spi master/slave mode
        en_functional_state_t enCommAutoSuspendEn;      ///< Enable/disable Communication auto suspend
        en_functional_state_t enModeFaultErrorDetectEn; ///< Enable/disable Mode fault error detect
        en_functional_state_t enParitySelfDetectEn;     ///< Enable/disable Parity self detect
        en_functional_state_t enParityEn;               ///< Enable/disable Parity(if enable parity and SPI_CR1.TXMDS=1, receive data don't parity)
        en_spi_parity_t enParity;                       ///< Parity mode select
    } stc_spi_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    /* Base functions */
    en_result_t SPI_DeInit(M4_SPI_TypeDef *SPIx);
    en_result_t SPI_Init(M4_SPI_TypeDef *SPIx, const stc_spi_init_t *pstcSpiInitCfg);
    en_result_t SPI_GeneralLoopbackCmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewSta);
    en_result_t SPI_ReverseLoopbackCmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewSta);
    en_result_t SPI_Cmd(M4_SPI_TypeDef *SPIx, en_functional_state_t enNewSta);

    /* Send and receive data functions */
    en_result_t SPI_SendData8(M4_SPI_TypeDef *SPIx, uint8_t u8Data);
    en_result_t SPI_SendData16(M4_SPI_TypeDef *SPIx, uint16_t u16Data);
    en_result_t SPI_SendData32(M4_SPI_TypeDef *SPIx, uint32_t u32Data);
    uint8_t SPI_ReceiveData8(const M4_SPI_TypeDef *SPIx);
    uint16_t SPI_ReceiveData16(const M4_SPI_TypeDef *SPIx);
    uint32_t SPI_ReceiveData32(const M4_SPI_TypeDef *SPIx);

    /* Communication configure functions */
    en_result_t SPI_SetSsPolarity(M4_SPI_TypeDef *SPIx, en_spi_ss_channel_t enChannel,
                                  en_spi_ss_polarity_t enPolarity);
    en_result_t SPI_SetSsValidChannel(M4_SPI_TypeDef *SPIx, en_spi_ss_channel_t enChannel);
    en_result_t SPI_SetReadDataRegObject(M4_SPI_TypeDef *SPIx, en_spi_read_object_t enObject);
    en_result_t SPI_SetFrameNumber(M4_SPI_TypeDef *SPIx, en_spi_frame_number_t enFrameNum);
    en_result_t SPI_SetDataLength(M4_SPI_TypeDef *SPIx, en_spi_data_length_t enDataLength);
    en_result_t SPI_SetFirstBitPosition(M4_SPI_TypeDef *SPIx, en_spi_first_bit_position_t enPosition);
    en_result_t SPI_SetClockDiv(M4_SPI_TypeDef *SPIx, en_spi_clk_div_t enClkDiv);

    /* Interrupt and flags functions */
    en_result_t SPI_IrqCmd(M4_SPI_TypeDef *SPIx, en_spi_irq_type_t enIrq,
                           en_functional_state_t enNewSta);
    en_flag_status_t SPI_GetFlag(M4_SPI_TypeDef *SPIx, en_spi_flag_type_t enFlag);
    en_result_t SPI_ClearFlag(M4_SPI_TypeDef *SPIx, en_spi_flag_type_t enFlag);

    //@} // SpiGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_SPI_ENABLE */

#endif /* __HC32F46x_SPI_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
