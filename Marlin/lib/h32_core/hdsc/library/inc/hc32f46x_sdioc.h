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
/** \file hc32f46x_sdioc.h
 **
 ** A detailed description is available at
 ** @link SdiocGroup SDIOC description @endlink
 **
 **   - 2018-11-11  1.0 Hongjh First version for Device Driver Library of SDIOC.
 **
 ******************************************************************************/
#ifndef __HC32F46X_SDIOC_H__
#define __HC32F46X_SDIOC_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_SDIOC_ENABLE == DDL_ON)

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
     ** \brief SDIOC transfer bus width enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_bus_width
    {
        SdiocBusWidth4Bit = 0u, ///< The SDIOC bus width 4 bit
        SdiocBusWidth8Bit = 1u, ///< The SDIOC bus width 8 bit
        SdiocBusWidth1Bit = 2u, ///< The SDIOC bus width 1 bit
    } en_sdioc_bus_width_t;

    /**
     *******************************************************************************
     ** \brief SDIOC clock division enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_clk_div
    {
        SdiocClkDiv_1 = 0x00u,   ///< EXCLK/1
        SdiocClkDiv_2 = 0x01u,   ///< EXCLK/2
        SdiocClkDiv_4 = 0x02u,   ///< EXCLK/4
        SdiocClkDiv_8 = 0x04u,   ///< EXCLK/8
        SdiocClkDiv_16 = 0x08u,  ///< EXCLK/16
        SdiocClkDiv_32 = 0x10u,  ///< EXCLK/32
        SdiocClkDiv_64 = 0x20u,  ///< EXCLK/64
        SdiocClkDiv_128 = 0x40u, ///< EXCLK/128
        SdiocClkDiv_256 = 0x80u, ///< EXCLK/256
    } en_sdioc_clk_div_t;

    /**
     *******************************************************************************
     ** \brief SDIOC response type enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_response_type
    {
        SdiocResponseNoneBit = 0u,        ///< No Response
        SdiocResponse136Bit = 1u,         ///< Response Length 136
        SdiocResponse48Bit = 2u,          ///< Response Length 48
        SdiocResponse48BitCheckBusy = 3u, ///< Response Length 48 check Busy after response
    } en_sdioc_response_type_t;

    /**
     *******************************************************************************
     ** \brief SDIOC response index enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_response_index
    {
        SdiocCmdNoRsp = 0u,  ///< No Response
        SdiocCmdRspR1 = 1u,  ///< Command Response 1
        SdiocCmdRspR1b = 2u, ///< Command Response 1 with busy
        SdiocCmdRspR2 = 3u,  ///< Command Response 2
        SdiocCmdRspR3 = 4u,  ///< Command Response 3
        SdiocCmdRspR4 = 5u,  ///< Command Response 4
        SdiocCmdRspR5 = 6u,  ///< Command Response 5
        SdiocCmdRspR5b = 7u, ///< Command Response 5 with busy
        SdiocCmdRspR6 = 8u,  ///< Command Response 6
        SdiocCmdRspR7 = 9u,  ///< Command Response 7
    } en_sdioc_response_index_t;

    /**
     *******************************************************************************
     ** \brief SDIOC command type enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_cmd_type
    {
        SdiocCmdNormal = 0u,  ///< Other commands
        SdiocCmdSuspend = 1u, ///< CMD52 for writing "Bus Suspend" in CCCR
        SdiocCmdResume = 2u,  ///< CMD52 for writing "Function Select" in CCCR
        SdiocCmdAbort = 3u,   ///< CMD12, CMD52 for writing "I/O Abort" in CCCR
    } en_sdioc_cmd_type_t;

    /**
     *******************************************************************************
     ** \brief SDIOC data transfer direction enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_transfer_dir
    {
        SdiocTransferToCard = 0u, ///< Write (Host to Card)
        SdiocTransferToHost = 1u, ///< Read (Card to Host)
    } en_sdioc_transfer_dir_t;

    /**
     *******************************************************************************
     ** \brief SDIOC data transfer mode enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_transfer_mode
    {
        SdiocTransferSingle = 0u,       ///< Single Block transfer
        SdiocTransferInfinite = 1u,     ///< Infinite Block transfer
        SdiocTransferMultiple = 2u,     ///< Multiple Block transfer
        SdiocTransferStopMultiple = 3u, ///< Stop Multiple Block transfer
    } en_sdioc_transfer_mode_t;

    /**
     *******************************************************************************
     ** \brief SD data timeout time enumeration
     **
     ******************************************************************************/
    typedef enum en_sd_data_timeout
    {
        SdiocDtoSdclk_2_13 = 0u,  ///< Timeout time: SDCLK*2^13
        SdiocDtoSdclk_2_14 = 1u,  ///< Timeout time: SDCLK*2^14
        SdiocDtoSdclk_2_15 = 2u,  ///< Timeout time: SDCLK*2^15
        SdiocDtoSdclk_2_16 = 3u,  ///< Timeout time: SDCLK*2^16
        SdiocDtoSdclk_2_17 = 4u,  ///< Timeout time: SDCLK*2^17
        SdiocDtoSdclk_2_18 = 5u,  ///< Timeout time: SDCLK*2^18
        SdiocDtoSdclk_2_19 = 6u,  ///< Timeout time: SDCLK*2^19
        SdiocDtoSdclk_2_20 = 7u,  ///< Timeout time: SDCLK*2^20
        SdiocDtoSdclk_2_21 = 8u,  ///< Timeout time: SDCLK*2^21
        SdiocDtoSdclk_2_22 = 9u,  ///< Timeout time: SDCLK*2^22
        SdiocDtoSdclk_2_23 = 10u, ///< Timeout time: SDCLK*2^23
        SdiocDtoSdclk_2_24 = 11u, ///< Timeout time: SDCLK*2^24
        SdiocDtoSdclk_2_25 = 12u, ///< Timeout time: SDCLK*2^25
        SdiocDtoSdclk_2_26 = 13u, ///< Timeout time: SDCLK*2^26
        SdiocDtoSdclk_2_27 = 14u, ///< Timeout time: SDCLK*2^27
    } en_sdioc_data_timeout_t;

    /**
     *******************************************************************************
     ** \brief SDIOC dat line type enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_dat_line_type
    {
        SdiocDat0Line = 0u, ///< DAT0 Line
        SdiocDat1Line = 1u, ///< DAT1 Line
        SdiocDat2Line = 2u, ///< DAT2 Line
        SdiocDat3Line = 3u, ///< DAT3 Line
    } en_sdioc_dat_line_type_t;

    /**
     *******************************************************************************
     ** \brief SDIOC software reset type enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_sw_reset
    {
        SdiocSwResetDatLine = 0u, ///< Only part of data circuit is reset.
        SdiocSwResetCmdLine = 1u, ///< Only part of command circuit is reset.
        SdiocSwResetAll = 2u,     ///< Reset the entire Host Controller except for the card detection circuit.
    } en_sdioc_sw_reset_t;

    /**
     *******************************************************************************
     ** \brief SDIOC host status enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_host_status
    {
        SdiocCommandInhibitCmd = (1u << 0),   ///< Command Inhibit(CMD). 1: Cannot issue command; 0:Can issue command using only CMD line
        SdiocCommandInhibitData = (1u << 1),  ///< Command Inhibit(DAT). 1: Cannot issue command which uses the DAT line; 0:Can issue command which uses the DAT line
        SdiocDataLineActive = (1u << 2),      ///< 1: DAT Line Active; 0: DAT Line Inactive
        SdiocWriteTransferActive = (1u << 8), ///< Write Transfer Active.1: Transferring data; 0: No valid data
        SdiocReadTransferActive = (1u << 9),  ///< Read Transfer Active.1: Transferring data; 0: No valid data
        SdiocBufferWriteEnble = (1u << 10),   ///< 1: Write enable; 0: Write Disable
        SdiocBufferReadEnble = (1u << 11),    ///< 1: Read enable; 0: Read Disable
        SdiocCardInserted = (1u << 16),       ///< 1: Card Inserted; 0: Reset or Debouncing or No Card
        SdiocCardStateStable = (1u << 17),    ///< 1: No Card or Inserted; 0: Reset or Debouncing
        SdiocCardDetectPinLvl = (1u << 18),   ///< 1: Card present; 0: No card present
        SdiocWriteProtectPinLvl = (1u << 19), ///< 1: Write enabled; 0: Write protected
        SdiocData0PinLvl = (1u << 20),        ///< 1: DAT0 line signal level high; 0: DAT0 line signal level low
        SdiocData1PinLvl = (1u << 21),        ///< 1: DAT1 line signal level high; 0: DAT1 line signal level low
        SdiocData2PinLvl = (1u << 22),        ///< 1: DAT2 line signal level high; 0: DAT2 line signal level low
        SdiocData3PinLvl = (1u << 23),        ///< 1: DAT3 line signal level high; 0: DAT3 line signal level low
        SdiocCmdPinLvl = (1u << 24),          ///< 1: CMD line signal level high;  0: CMD line signal level low
    } en_sdioc_host_status_t;

    /**
     *******************************************************************************
     ** \brief SDIOC normal interrupt selection enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_nor_int_sel
    {
        SdiocCommandComplete = (1u << 0),  ///< Command Complete. 1: Command complete; 0:No command complete
        SdiocTransferComplete = (1u << 1), ///< Transfer Complete. 1: Data transfer complete; 0:No transfer complete
        SdiocBlockGapEvent = (1u << 2),    ///< Block Gap Event. 1: Transaction stopped at block gap; 0: No Block Gap Event
        SdiocBufferWriteReady = (1u << 4), ///< Buffer Write Ready. 1: Ready to Write buffer; 0: No ready to Write buffer
        SdiocBufferReadReady = (1u << 5),  ///< Buffer Read Ready. 1: Ready to read buffer; 0: No ready to read buffer
        SdiocCardInsertedInt = (1u << 6),  ///< Write Transfer Active.1: Transferring data; 0: No valid data
        SdiocCardRemoval = (1u << 7),      ///< Card Removal. 1: Card removed; 0: Card state stable or Debouncing
        SdiocCardInt = (1u << 8),          ///< Card Interrupt. 1: Generate Card Interrupt; 0: No Card Interrupt
        SdiocErrorInt = (1u << 15),        ///< Error Interrupt. 1: Error; 0: No Error
    } en_sdioc_nor_int_sel_t,
        en_sdioc_nor_int_flag_t;

    /**
     *******************************************************************************
     ** \brief SDIOC error interrupt selection enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_err_int_sel
    {
        SdiocCmdTimeoutErr = (1u << 0),  ///< Command Timeout Error. 1: Timer out; 0:No Error
        SdiocCmdCrcErr = (1u << 1),      ///< Command CRC Error. 1: Command CRC Error Generated; 0:No Error
        SdiocCmdEndBitErr = (1u << 2),   ///< Command End Bit Error. 1: End Bit Error Generated; 0:No Error
        SdiocCmdIndexErr = (1u << 3),    ///< Command Index Error. 1: Command Index Error Generatedr; 0:No Error
        SdiocDataTimeoutErr = (1u << 4), ///< Data Timeout Error. 1: Timer out; 0:No Error
        SdiocDataCrcErr = (1u << 5),     ///< Data CRC Error. 1: Data CRC Error Generated; 0:No Error
        SdiocDataEndBitErr = (1u << 6),  ///< Data End Bit Error. 1: End Bit Error Generated; 0:No Error
        SdiocAutoCmd12Err = (1u << 8),   ///< Auto CMD12 Error. 1: Error; 0:No Error
    } en_sdioc_err_int_sel_t,
        en_sdioc_err_int_flag_t;

    /**
     *******************************************************************************
     ** \brief SDIOC auto CMD12 error status enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_atuo_cmd_err_status
    {
        SdiocAutoCmd12NotExecuted = (1u << 0), ///< Auto CMD12 Not Executed. 1: Not executed; 0:Executed
        SdiocAutoCmd12Timeout = (1u << 1),     ///< Auto CMD12 Timeout Error. 1: Time out; 0:No error
        SdiocAutoCmd12CrcErr = (1u << 2),      ///< Auto CMD12 CRC Error. 1: CRC Error Generated; 0: No error
        SdiocAutoCmd12EndBitErr = (1u << 3),   ///< Auto CMD12 End Bit Error. 1: End Bit Error Generated; 0: No error to Write buffer
        SdiocAutoCmd12IndexErr = (1u << 4),    ///< Auto CMD12 Index Error. 1: Error; 0: No error
        SdiocCmdNotIssuedErr = (1u << 7),      ///< Command Not Issued By Auto CMD12 Error.1: Not Issued; 0: No error
    } en_sdioc_atuo_cmd_err_sel_t,
        en_sdioc_atuo_cmd_err_status_t;

    /**
     *******************************************************************************
     ** \brief SDIOC speed mode enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_speed_mode
    {
        SdiocNormalSpeedMode = 0u, ///< Normal speed mode
        SdiocHighSpeedMode = 1u,   ///< High speed mode
    } en_sdioc_speed_mode_t;

    /**
     *******************************************************************************
     ** \brief SDIOC response register enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_response_reg
    {
        SdiocRegResp01 = 0x00u, ///< Response 0/1 Register
        SdiocRegResp23 = 0x04u, ///< Response 2/3 Register
        SdiocRegResp45 = 0x08u, ///< Response 4/5 Register
        SdiocRegResp67 = 0x0Cu, ///< Response 5/6 Register
    } en_sdioc_response_reg_t;

    /**
     ******************************************************************************
     ** \brief SDIOC output clock frequency enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_clk_freq
    {
        SdiocClk400K = 400000u,  ///< SDIOC clock: 40KHz
        SdiocClk20M = 20000000u, ///< SDIOC clock: 20MHz
        SdiocClk25M = 25000000u, ///< SDIOC clock: 25MHz
        SdiocClk40M = 40000000u, ///< SDIOC clock: 40MHz
        SdiocClk50M = 50000000u, ///< SDIOC clock: 50MHz
    } en_sdioc_clk_freq_t;

    /**
     ******************************************************************************
     ** \brief SDIOC detect the source of card enumeration
     **
     ******************************************************************************/
    typedef enum en_sdioc_detect_signal
    {
        SdiocSdcdPinLevel = 0u,        ///< SDCD# is selected (for normal use)
        SdiocCardDetectTestLevel = 1u, ///< The Card Detect Test Level is selected(for test purpose)
    } en_sdioc_detect_signal_t;

    /**
     *******************************************************************************
     ** \brief SDIOC Command configure structure
     **
     ******************************************************************************/
    typedef struct stc_sdioc_cmd_cfg
    {
        uint8_t u8CmdIndex; ///< Command index

        uint32_t u32Argument; ///< The argument of command

        en_sdioc_cmd_type_t enCmdType; ///< Command type

        en_sdioc_response_index_t enRspIndex; ///< Response index, refer @ref en_sdioc_response_index_t for details

        en_functional_state_t enDataPresentEnable; ///< Enable: Data is present and shall be transferred using the DAT line, Disable:  Commands using only CMD line
    } stc_sdioc_cmd_cfg_t;

    /**
     *******************************************************************************
     ** \brief SDIOC Data configure structure
     **
     ******************************************************************************/
    typedef struct stc_sdioc_data_cfg
    {
        uint16_t u16BlkSize; ///< Block size

        uint16_t u16BlkCnt; ///< Block count

        en_functional_state_t enAutoCmd12Enable; ///< Enable: Auto CMD12 enable, Disable: Auto CMD12 disable

        en_sdioc_transfer_dir_t enTransferDir; ///< Specifies the data transfer direction of the SDIOC controller.
                                               ///< This parameter can be a value of @ref en_sdioc_transfer_dir_t.

        en_sdioc_data_timeout_t enDataTimeOut; ///< Specifies the data timeout period in card bus clock periods.
                                               ///< This parameter can be a value of @ref en_sdioc_data_timeout_t.

        en_sdioc_transfer_mode_t enTransferMode; ///< Specifies the data transfer mode of the SDIOC controller.
                                                 ///< This parameter can be a value of @ref en_sdioc_transfer_mode_t.
    } stc_sdioc_data_cfg_t;

    /**
     *******************************************************************************
     ** \brief SDIOC normal interrupt enable structure
     **
     ******************************************************************************/
    typedef struct stc_sdioc_normal_irq_en
    {
        union
        {
            uint16_t u16NormalIntsgEn;                     ///< SDIOC normal interrupt enable
            stc_sdioc_errintsgen_field_t stcNormalIntsgEn; ///< SDIOC normal interrupt enable bit-field structure
        };
    } stc_sdioc_normal_irq_en_t;

    /**
     *******************************************************************************
     ** \brief SDIOC normal interrupt enable structure
     **
     ******************************************************************************/
    typedef struct stc_sdioc_error_irq_en
    {
        union
        {
            uint16_t u16ErrorIntsgEn;                     ///< SDIOC error interrupt enable
            stc_sdioc_errintsgen_field_t stcErrorIntsgEn; ///< SDIOC error interrupt enable bit-field structure
        };
    } stc_sdioc_error_irq_en_t;

    /**
     *******************************************************************************
     ** \brief SDIOC error status callback functions
     **
     ******************************************************************************/
    typedef struct stc_sdioc_normal_irq_cb
    {
        func_ptr_t pfnCommandCompleteIrqCb; ///< Pointer to command complete callback function

        func_ptr_t pfnTransferCompleteIrqCb; ///< Pointer to transfer complete callback function

        func_ptr_t pfnBlockGapIrqCb; ///< Pointer to Block gap callback function

        func_ptr_t pfnBufferWriteReadyIrqCb; ///< Pointer to buffer write ready callback function

        func_ptr_t pfnBufferReadReadyIrqCb; ///< Pointer to buffer read ready callback function

        func_ptr_t pfnCardInsertIrqCb; ///< Pointer to card insertion callback function

        func_ptr_t pfnCardRemovalIrqCb; ///< Pointer to card removal callback function

        func_ptr_t pfnCardIrqCb; ///< Pointer to card interrupt callback function
    } stc_sdioc_normal_irq_cb_t;

    /**
     *******************************************************************************
     ** \brief SDIOC error status callback functions
     **
     ******************************************************************************/
    typedef struct stc_sdioc_error_irq_cb
    {
        func_ptr_t pfnCmdTimeoutErrIrqCb; ///< Pointer to command timeout error interrupt callback function

        func_ptr_t pfnCmdCrcErrIrqCb; ///< Pointer to command CRC error interrupt callback function

        func_ptr_t pfnCmdEndBitErrIrqCb; ///< Pointer to command end bit error interrupt callback function

        func_ptr_t pfnCmdIndexErrIrqCb; ///< Pointer to command index error interrupt callback function

        func_ptr_t pfnDataTimeoutErrIrqCb; ///< Pointer to data timeout error interrupt callback function

        func_ptr_t pfnDataCrcErrIrqCb; ///< Pointer to data CRC error interrupt callback function

        func_ptr_t pfnDataEndBitErrIrqCb; ///< Pointer to data end bit error interrupt callback function

        func_ptr_t pfnAutoCmdErrIrqCb; ///< Pointer to command error interrupt callback function
    } stc_sdioc_error_irq_cb_t;

    /**
     *******************************************************************************
     ** \brief SDIOC initialization configuration
     **
     ******************************************************************************/
    typedef struct stc_sdioc_init
    {
        stc_sdioc_normal_irq_en_t *pstcNormalIrqEn; ///< Pointer to normal interrupt enable structure

        stc_sdioc_normal_irq_cb_t *pstcNormalIrqCb; ///< Pointer to normal interrupt callback function structure

        stc_sdioc_error_irq_en_t *pstcErrorIrqEn; ///< Pointer to error interrupt enable structure

        stc_sdioc_error_irq_cb_t *pstcErrorIrqCb; ///< Pointer to error interrupt callback structure
    } stc_sdioc_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    void SDIOC_IrqHandler(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_Init(M4_SDIOC_TypeDef *SDIOCx,
                           const stc_sdioc_init_t *pstcInitCfg);
    en_result_t SDIOC_DeInit(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_SendCommand(M4_SDIOC_TypeDef *SDIOCx,
                                  const stc_sdioc_cmd_cfg_t *pstcCmdCfg);
    uint32_t SDIOC_GetResponse(const M4_SDIOC_TypeDef *SDIOCx,
                               en_sdioc_response_reg_t enRespReg);
    en_result_t SDIOC_ReadBuffer(M4_SDIOC_TypeDef *SDIOCx,
                                 uint8_t au8Data[],
                                 uint32_t u32Len);
    en_result_t SDIOC_WriteBuffer(M4_SDIOC_TypeDef *SDIOCx,
                                  uint8_t au8Data[],
                                  uint32_t u32Len);
    en_result_t SDIOC_ConfigData(M4_SDIOC_TypeDef *SDIOCx,
                                 const stc_sdioc_data_cfg_t *pstcDataCfg);
    en_result_t SDIOC_SdclkCmd(M4_SDIOC_TypeDef *SDIOCx,
                               en_functional_state_t enCmd);
    en_result_t SDIOC_SetClkDiv(M4_SDIOC_TypeDef *SDIOCx,
                                en_sdioc_clk_div_t enClkDiv);
    en_sdioc_clk_div_t SDIOC_GetClkDiv(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_SetClk(M4_SDIOC_TypeDef *SDIOCx, uint32_t u32ClkFreq);
    en_result_t SDIOC_SetBusWidth(M4_SDIOC_TypeDef *SDIOCx,
                                  en_sdioc_bus_width_t enBusWidth);
    en_sdioc_bus_width_t SDIOC_GetBusWidth(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_SetSpeedMode(M4_SDIOC_TypeDef *SDIOCx,
                                   en_sdioc_speed_mode_t enSpeedMode);
    en_sdioc_speed_mode_t SDIOC_GetSpeedMode(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_SetDataTimeout(M4_SDIOC_TypeDef *SDIOCx,
                                     en_sdioc_data_timeout_t enTimeout);
    en_sdioc_data_timeout_t SDIOC_GetDataTimeout(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_SetCardDetectSignal(M4_SDIOC_TypeDef *SDIOCx,
                                          en_sdioc_detect_signal_t enDetectSignal);
    en_flag_status_t SDIOC_GetCardDetectTestLevel(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_BusPowerOn(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_BusPowerOff(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_StopAtBlockGapCmd(M4_SDIOC_TypeDef *SDIOCx,
                                        en_functional_state_t enCmd);
    en_result_t SDIOC_RestartTransfer(M4_SDIOC_TypeDef *SDIOCx);
    en_result_t SDIOC_ReadWaitCmd(M4_SDIOC_TypeDef *SDIOCx,
                                  en_functional_state_t enCmd);
    en_result_t SDIOC_InterruptAtBlockGapCmd(M4_SDIOC_TypeDef *SDIOCx,
                                             en_functional_state_t enCmd);
    en_result_t SDIOC_SoftwareReset(M4_SDIOC_TypeDef *SDIOCx,
                                    en_sdioc_sw_reset_t enSwResetType);
    en_flag_status_t SDIOC_GetStatus(M4_SDIOC_TypeDef *SDIOCx,
                                     en_sdioc_host_status_t enHostStatus);
    en_result_t SDIOC_NormalIrqSignalCmd(M4_SDIOC_TypeDef *SDIOCx,
                                         en_sdioc_nor_int_sel_t enNorInt,
                                         en_functional_state_t enCmd);
    en_result_t SDIOC_NormalIrqStatusCmd(M4_SDIOC_TypeDef *SDIOCx,
                                         en_sdioc_nor_int_sel_t enNorInt,
                                         en_functional_state_t enCmd);
    en_flag_status_t SDIOC_GetNormalIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                            en_sdioc_nor_int_flag_t enNorInt);
    en_result_t SDIOC_ClearNormalIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                         en_sdioc_nor_int_flag_t enNorInt);
    en_result_t SDIOC_ErrIrqSignalCmd(M4_SDIOC_TypeDef *SDIOCx,
                                      en_sdioc_err_int_sel_t enErrInt,
                                      en_functional_state_t enCmd);
    en_result_t SDIOC_ErrIrqStatusCmd(M4_SDIOC_TypeDef *SDIOCx,
                                      en_sdioc_err_int_sel_t enErrInt,
                                      en_functional_state_t enCmd);
    en_flag_status_t SDIOC_GetErrIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                         en_sdioc_err_int_flag_t enErrInt);
    en_result_t SDIOC_ClearErrIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                      en_sdioc_err_int_flag_t enErrInt);
    en_result_t SDIOC_ForceErrIrqFlag(M4_SDIOC_TypeDef *SDIOCx,
                                      en_sdioc_err_int_sel_t enErrInt);
    en_flag_status_t SDIOC_GetAutoCmdErrStatus(M4_SDIOC_TypeDef *SDIOCx,
                                               en_sdioc_atuo_cmd_err_status_t enAutoCmdErr);
    en_result_t SDIOC_ForceAutoCmdErr(M4_SDIOC_TypeDef *SDIOCx,
                                      en_sdioc_atuo_cmd_err_sel_t enAutoCmdErr);

    //@} // SdiocGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_SDIOC_ENABLE */

#endif /* __HC32F46X_SDIOC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
