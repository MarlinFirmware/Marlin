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
/** \file hc32f46x_usart.h
 **
 ** A detailed description is available at
 ** @link UsartGroup USART description @endlink
 **
 **   - 2018-11-27  1.0 Hongjh First version for Device Driver Library of USART.
 **
 ******************************************************************************/
#ifndef __HC32F46x_USART_H__
#define __HC32F46x_USART_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_USART_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup UsartGroup Universal Synchronous Asynchronous Receiver \
     **  Transmitter(USART)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/

    /**
     *******************************************************************************
     ** \brief USART tx mode in multiple processor mode enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_mp_tx_mode
    {
        UsartMpTxData = 0u, ///< USART Send data in multiple-processor mode
        UsartMpTxId = 1u,   ///< USART Send ID  in multiple-processor mode
    } en_usart_mp_tx_mode_t;

    /**
     *******************************************************************************
     ** \brief USART clock prescale enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_clk_div
    {
        UsartClkDiv_1 = 0u,  ///< PCLK/1
        UsartClkDiv_4 = 1u,  ///< PCLK/4
        UsartClkDiv_16 = 2u, ///< PCLK/16
        UsartClkDiv_64 = 3u, ///< PCLK/64
    } en_usart_clk_div_t;

    /**
     ******************************************************************************
     ** \brief USART mode
     **
     ******************************************************************************/
    typedef enum en_usart_mode
    {
        UsartUartMode = 0u,      ///< UART mode
        UsartClkSyncMode = 1u,   ///< Clock sync mode
        UsartSmartCardMode = 2u, ///< Smart card mode
    } en_usart_mode_t;

    /**
     ******************************************************************************
     ** \brief USART data direction
     **
     ******************************************************************************/
    typedef enum en_usart_data_dir
    {
        UsartDataLsbFirst = 0u, ///< LSB first
        UsartDataMsbFirst = 1u, ///< MSB first
    } en_usart_data_dir_t;

    /**
     ******************************************************************************
     ** \brief USART sample mode enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_sample_mode
    {
        UsartSamleBit16 = 0u, ///< 16 Bit
        UsartSamleBit8 = 1u,  ///< 8 Bit
    } en_usart_sample_mode_t;

    /**
     ******************************************************************************
     ** \brief USART data length enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_data_len
    {
        UsartDataBits8 = 0u, ///< 8 Bit
        UsartDataBits9 = 1u, ///< 9 Bit
    } en_usart_data_len_t;

    /**
     ******************************************************************************
     ** \brief USART parity format enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_parity
    {
        UsartParityNone = 0u, ///< No parity bit is used.
        UsartParityEven = 1u, ///< Even parity bit is used.
        UsartParityOdd = 2u,  ///< Odd parity bit is used.
    } en_usart_parity_t;

    /**
     ******************************************************************************
     ** \brief USART functions enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_func
    {
        UsartRx = 0u,              ///< UART RX function
        UsartRxInt = 1u,           ///< USART RX interrupt function
        UsartTx = 2u,              ///< UART TX function
        UsartTxEmptyInt = 3u,      ///< USART TX empty interrupt function
        UsartTimeOut = 4u,         ///< UART RX timeout function
        UsartTimeOutInt = 5u,      ///< UART RX timeout interrupt function
        UsartSilentMode = 6u,      ///< USART silent function
        UsartTxCmpltInt = 7u,      ///< USART TX complete interrupt function
        UsartTxAndTxEmptyInt = 8u, ///< USART TX function and USART TX empty interrupt function
        UsartParityCheck = 9u,     ///< USART Parity check function
        UsartNoiseFilter = 10u,    ///< USART noise filter function
        UsartFracBaudrate = 11u,   ///< USART fractional baudrate function
        UsartMulProcessor = 12u,   ///< USART multiple processor function
        UsartSmartCard = 13u,      ///< USART smart card mode function
        UsartCts = 14u,            ///< USART CTS function
    } en_usart_func_t;

    /**
     *******************************************************************************
     ** \brief USART status type enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_status
    {
        UsartParityErr = (1u << 0),  ///< USART parity error
        UsartFrameErr = (1u << 1),   ///< USART receive frame error
        UsartOverrunErr = (1u << 3), ///< USART receive over-run error
        UsartRxNoEmpty = (1u << 5),  ///< USART data receive register is not empty
        UsartTxComplete = (1u << 6), ///< USART transfer completely
        UsartTxEmpty = (1u << 7),    ///< USART data transfer register is empty
        UsartRxTimeOut = (1u << 8),  ///< USART data receive timeout
        UsartRxMpb = (1u << 16),     ///< USART multiple processor id or normal data, 0: receive date; 1: received ID
    } en_usart_status_t;

    /**
     *******************************************************************************
     ** \brief USART Stop bit length select enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_stop_bit
    {
        UsartOneStopBit = 0u, ///< 1 Stop Bit
        UsartTwoStopBit = 1u, ///< 2 Stop Bit
    } en_usart_stop_bit_t;

    /**
     *******************************************************************************
     ** \brief USART start bit detect mode enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_sb_detect_mode
    {
        UsartStartBitLowLvl = 0u,   ///< Start bit: RD pin low level
        UsartStartBitFallEdge = 1u, ///< Start bit: RD pin falling edge
    } en_usart_sb_detect_mode_t;

    /**
     *******************************************************************************
     ** \brief USART clock mode selection enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_clk_mode
    {
        UsartIntClkCkNoOutput = 0u, ///< Select internal clock source and don't output clock.
        UsartIntClkCkOutput = 1u,   ///< Select internal clock source and output clock.
        UsartExtClk = 2u,           ///< Select external clock source.
    } en_usart_clk_mode_t;

    /**
     *******************************************************************************
     ** \brief USART smart-card mode selection enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_hw_flow_ctrl
    {
        UsartRtsEnable = 0u, ///< Enable RTS function.
        UsartCtsEnable = 1u, ///< Enable CTS function.
    } en_usart_hw_flow_ctrl_t;

    /**
     ******************************************************************************
     ** \brief USART etu clocks of smart card enumeration
     **
     ******************************************************************************/
    typedef enum en_usart_sc_etu_clk
    {
        UsartScEtuClk32 = 0u,  ///< 1 etu = 32/f
        UsartScEtuClk64 = 1u,  ///< 1 etu = 64/f
        UsartScEtuClk128 = 3u, ///< 1 etu = 128/f
        UsartScEtuClk256 = 5u, ///< 1 etu = 256/f
        UsartScEtuClk372 = 6u, ///< 1 etu = 372/f
    } en_usart_sc_etu_clk_t;

    /**
     *******************************************************************************
     ** \brief Uart mode initialization configuration
     **
     ******************************************************************************/
    typedef struct stc_usart_uart_init
    {
        en_usart_clk_mode_t enClkMode; ///< Clock mode and this parameter can be a value of @ref en_usart_clk_mode_t

        en_usart_clk_div_t enClkDiv; ///< USART divide PCLK1, and this parameter can be a value of @ref en_usart_clk_div_t

        en_usart_data_len_t enDataLength; ///< 8/9 Bit character length and this parameter can be a value of @ref en_usart_data_len_t

        en_usart_data_dir_t enDirection; ///< UART data direction and this parameter can be a value of @ref en_usart_data_dir_t

        en_usart_stop_bit_t enStopBit; ///< Stop bit and this parameter can be a value of @ref en_usart_stop_bit_t

        en_usart_parity_t enParity; ///< Parity format and this parameter can be a value of @ref en_usart_parity_t

        en_usart_sample_mode_t enSampleMode; ///< USART sample mode, and this parameter can be a value of @ref en_usart_sample_mode_t

        en_usart_sb_detect_mode_t enDetectMode; ///< USART start bit detect mode and this parameter can be a value of @ref en_usart_sb_detect_mode_t

        en_usart_hw_flow_ctrl_t enHwFlow; ///< Hardware flow control and this parameter can be a value of @ref en_usart_hw_flow_ctrl_t
    } stc_usart_uart_init_t;

    /**
     *******************************************************************************
     ** \brief Clock sync mode initialization configuration
     **
     ******************************************************************************/
    typedef struct stc_usart_clksync_init
    {
        en_usart_clk_mode_t enClkMode; ///< Clock mode and this parameter can be a value of @ref en_usart_clk_mode_t

        en_usart_clk_div_t enClkDiv; ///< USART divide PCLK1, and this parameter can be a value of @ref en_usart_clk_div_t

        en_usart_data_dir_t enDirection; ///< UART data direction and this parameter can be a value of @ref en_usart_data_dir_t

        en_usart_hw_flow_ctrl_t enHwFlow; ///< Hardware flow control and this parameter can be a value of @ref en_usart_hw_flow_ctrl_t
    } stc_usart_clksync_init_t;

    /**
     *******************************************************************************
     ** \brief Smart card mode initialization configuration
     **
     ******************************************************************************/
    typedef struct stc_usart_sc_init
    {
        en_usart_clk_mode_t enClkMode; ///< Clock mode and this parameter can be a value of @ref en_usart_clk_mode_t

        en_usart_clk_div_t enClkDiv; ///< USART divide PCLK1, and this parameter can be a value of @ref en_usart_clk_div_t

        en_usart_data_dir_t enDirection; ///< UART data direction and this parameter can be a value of @ref en_usart_data_dir_t
    } stc_usart_sc_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t USART_UART_Init(M4_USART_TypeDef *USARTx,
                                const stc_usart_uart_init_t *pstcInitCfg);
    en_result_t USART_CLKSYNC_Init(M4_USART_TypeDef *USARTx,
                                   const stc_usart_clksync_init_t *pstcInitCfg);
    en_result_t USART_SC_Init(M4_USART_TypeDef *USARTx,
                              const stc_usart_sc_init_t *pstcInitCfg);
    en_result_t USART_DeInit(M4_USART_TypeDef *USARTx);
    en_flag_status_t USART_GetStatus(M4_USART_TypeDef *USARTx,
                                     en_usart_status_t enStatus);
    en_result_t USART_ClearStatus(M4_USART_TypeDef *USARTx,
                                  en_usart_status_t enStatus);
    en_result_t USART_FuncCmd(M4_USART_TypeDef *USARTx,
                              en_usart_func_t enFunc,
                              en_functional_state_t enCmd);
    en_result_t USART_SetParity(M4_USART_TypeDef *USARTx,
                                en_usart_parity_t enParity);
    en_usart_parity_t USART_GetParity(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetOverSampling(M4_USART_TypeDef *USARTx,
                                      en_usart_sample_mode_t enSampleMode);
    en_usart_sample_mode_t USART_GetOverSampling(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetDataDirection(M4_USART_TypeDef *USARTx,
                                       en_usart_data_dir_t enDir);
    en_usart_data_dir_t USART_GetTransferDirection(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetDataLength(M4_USART_TypeDef *USARTx,
                                    en_usart_data_len_t enDataLen);
    en_usart_data_len_t USART_GetDataLength(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetClkMode(M4_USART_TypeDef *USARTx,
                                 en_usart_clk_mode_t enClkMode);
    en_usart_clk_mode_t USART_GetClkMode(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetMode(M4_USART_TypeDef *USARTx,
                              en_usart_mode_t enMode);
    en_usart_mode_t USART_GetMode(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetStopBitsLength(M4_USART_TypeDef *USARTx,
                                        en_usart_stop_bit_t enStopBit);
    en_usart_stop_bit_t USART_GetStopBitsLength(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetSbDetectMode(M4_USART_TypeDef *USARTx,
                                      en_usart_sb_detect_mode_t enDetectMode);
    en_usart_sb_detect_mode_t USART_GetSbDetectMode(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetHwFlowCtrl(M4_USART_TypeDef *USARTx,
                                    en_usart_hw_flow_ctrl_t enHwFlowCtrl);
    en_usart_hw_flow_ctrl_t USART_GetHwFlowCtrl(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetClockDiv(M4_USART_TypeDef *USARTx,
                                  en_usart_clk_div_t enClkPrescale);
    en_usart_clk_div_t USART_GetClockDiv(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetScEtuClk(M4_USART_TypeDef *USARTx,
                                  en_usart_sc_etu_clk_t enEtuClk);
    en_usart_sc_etu_clk_t USART_GetScEtuClk(M4_USART_TypeDef *USARTx);
    en_result_t USART_SendData(M4_USART_TypeDef *USARTx, uint16_t u16Data);
    uint16_t USART_RecData(M4_USART_TypeDef *USARTx);
    en_result_t USART_SetBaudrate(M4_USART_TypeDef *USARTx,
                                  uint32_t u32Baudrate);

    //@} // UsartGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_USART_ENABLE */

#endif /* __HC32F46x_USART_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
