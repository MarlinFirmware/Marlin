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
/** \file hc32f46x_usart.c
 **
 ** A detailed description is available at
 ** @link UsartGroup USART description @endlink
 **
 **   - 2018-11-27  1.0 Hongjh First version for Device Driver Library of USART.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_usart.h"
#include "hc32f46x_utility.h"

#if (DDL_USART_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup UsartGroup
 ******************************************************************************/

//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Parameter valid check for USART Instances. */
#define IS_VALID_USART(__USARTx__)  \
    ((M4_USART1 == (__USARTx__)) || \
     (M4_USART2 == (__USARTx__)) || \
     (M4_USART3 == (__USARTx__)) || \
     (M4_USART4 == (__USARTx__)))

/*!< Parameter valid check for USART clock prescale. */
#define IS_VALID_USART_CLK_DIV(x) \
    ((UsartClkDiv_1 == (x)) ||    \
     (UsartClkDiv_4 == (x)) ||    \
     (UsartClkDiv_16 == (x)) ||   \
     (UsartClkDiv_64 == (x)))

/*!< Parameter valid check for USART function type. */
#define IS_VALID_USART_FUNC(x)     \
    ((UsartRx == (x)) ||           \
     (UsartTx == (x)) ||           \
     (UsartCts == (x)) ||          \
     (UsartRxInt == (x)) ||        \
     (UsartTimeOut == (x)) ||      \
     (UsartSmartCard == (x)) ||    \
     (UsartSilentMode == (x)) ||   \
     (UsartTxEmptyInt == (x)) ||   \
     (UsartTimeOutInt == (x)) ||   \
     (UsartTxCmpltInt == (x)) ||   \
     (UsartParityCheck == (x)) ||  \
     (UsartNoiseFilter == (x)) ||  \
     (UsartFracBaudrate == (x)) || \
     (UsartMulProcessor == (x)) || \
     (UsartTxAndTxEmptyInt == (x)))

/*!< Parameter valid check for USART function type. */
#define IS_VALID_USART_STATUS(x) \
    ((UsartRxMpb == (x)) ||      \
     (UsartTxEmpty == (x)) ||    \
     (UsartFrameErr == (x)) ||   \
     (UsartRxNoEmpty == (x)) ||  \
     (UsartRxTimeOut == (x)) ||  \
     (UsartParityErr == (x)) ||  \
     (UsartOverrunErr == (x)) || \
     (UsartTxComplete == (x)))

/*!< Parameter valid check for USART clock mode. */
#define IS_VALID_USART_CLK_MODE(x)   \
    ((UsartExtClk == (x)) ||         \
     (UsartIntClkCkOutput == (x)) || \
     (UsartIntClkCkNoOutput == (x)))

/*!< Parameter valid check for USART stop bit. */
#define IS_VALID_USART_STOP_BIT(x) \
    ((UsartOneStopBit == (x)) ||   \
     (UsartTwoStopBit == (x)))

/*!< Parameter valid check for USART parity bit. */
#define IS_VALID_USART_PARITY_BIT(x) \
    ((UsartParityOdd == (x)) ||      \
     (UsartParityEven == (x)) ||     \
     (UsartParityNone == (x)))

/*!< Parameter valid check for USART data length. */
#define IS_VALID_USART_DATA_LEN(x) \
    ((UsartDataBits8 == (x)) ||    \
     (UsartDataBits9 == (x)))

/*!< Parameter valid check for USART data direction. */
#define IS_VALID_USART_DATA_DIR(x) \
    ((UsartDataLsbFirst == (x)) || \
     (UsartDataMsbFirst == (x)))

/*!< Parameter valid check for USART sample mode. */
#define IS_VALID_USART_SAMPLE_MODE(x) \
    ((UsartSamleBit8 == (x)) ||       \
     (UsartSamleBit16 == (x)))

/*!< Parameter valid check for USART sample mode. */
#define IS_VALID_USART_HW_FLOW_MODE(x) \
    ((UsartRtsEnable == (x)) ||        \
     (UsartCtsEnable == (x)))

/*!< Parameter valid check for USART detect mode. */
#define IS_VALID_USART_SB_DETECT_MODE(x) \
    ((UsartStartBitLowLvl == (x)) ||     \
     (UsartStartBitFallEdge == (x)))

/*!< Parameter valid check for USART mode. */
#define IS_VALID_USART_MODE(x)    \
    ((UsartUartMode == (x)) ||    \
     (UsartClkSyncMode == (x)) || \
     (UsartSmartCardMode == (x)))

/*!< Parameter valid check for USART ETU clocks number. */
#define IS_VALID_USART_ETU_CLK(x) \
    ((UsartScEtuClk32 == (x)) ||  \
     (UsartScEtuClk64 == (x)) ||  \
     (UsartScEtuClk128 == (x)) || \
     (UsartScEtuClk256 == (x)) || \
     (UsartScEtuClk372 == (x)))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static uint32_t UsartGetClk(const M4_USART_TypeDef *USARTx);
static en_result_t SetUartBaudrate(M4_USART_TypeDef *USARTx,
                                   uint32_t u32Baudrate);
static en_result_t SetClkSyncBaudrate(M4_USART_TypeDef *USARTx,
                                      uint32_t u32Baudrate);
static en_result_t SetScBaudrate(M4_USART_TypeDef *USARTx,
                                 uint32_t u32Baudrate);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Initialize UART mode of the specified USART.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] pstcInitCfg             Pointer to USART mode configure structure
 ** \arg This parameter detail refer @ref stc_usart_uart_init_t
 **
 ** \retval Ok                          USART is initialized normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - USARTx is invalid
 **                                     - pstcInitCfg == NULL
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t USART_UART_Init(M4_USART_TypeDef *USARTx,
                            const stc_usart_uart_init_t *pstcInitCfg)
{
    stc_usart_pr_field_t PR_f = {0};
    stc_usart_cr1_field_t CR1_f = {0};
    stc_usart_cr2_field_t CR2_f = {0};
    stc_usart_cr3_field_t CR3_f = {0};
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx && pstcInitCfg pointer */
    if ((IS_VALID_USART(USARTx)) && (NULL != pstcInitCfg))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_USART_CLK_DIV(pstcInitCfg->enClkDiv));
        DDL_ASSERT(IS_VALID_USART_CLK_MODE(pstcInitCfg->enClkMode));
        DDL_ASSERT(IS_VALID_USART_STOP_BIT(pstcInitCfg->enStopBit));
        DDL_ASSERT(IS_VALID_USART_PARITY_BIT(pstcInitCfg->enParity));
        DDL_ASSERT(IS_VALID_USART_DATA_DIR(pstcInitCfg->enDirection));
        DDL_ASSERT(IS_VALID_USART_DATA_LEN(pstcInitCfg->enDataLength));
        DDL_ASSERT(IS_VALID_USART_HW_FLOW_MODE(pstcInitCfg->enHwFlow));
        DDL_ASSERT(IS_VALID_USART_SAMPLE_MODE(pstcInitCfg->enSampleMode));
        DDL_ASSERT(IS_VALID_USART_SB_DETECT_MODE(pstcInitCfg->enDetectMode));

        /* Set default value */
        USARTx->CR1 = (uint32_t)0xFFFFFFF3ul;
        USARTx->CR1 = (uint32_t)0x80000000ul;
        USARTx->CR2 = (uint32_t)0x00000000ul;
        USARTx->CR3 = (uint32_t)0x00000000ul;
        USARTx->BRR = (uint32_t)0x0000FFFFul;
        USARTx->PR = (uint32_t)0x00000000ul;

        /* Set USART mode */
        CR3_f.SCEN = (uint32_t)0ul;
        CR1_f.MS = (uint32_t)0ul;

        PR_f.PSC = (uint32_t)(pstcInitCfg->enClkDiv);
        CR1_f.M = (uint32_t)(pstcInitCfg->enDataLength);
        CR1_f.ML = (uint32_t)(pstcInitCfg->enDirection);
        CR2_f.STOP = (uint32_t)(pstcInitCfg->enStopBit);
        CR2_f.CLKC = (uint32_t)(pstcInitCfg->enClkMode);

        switch (pstcInitCfg->enParity)
        {
        case UsartParityNone:
            CR1_f.PCE = (uint32_t)0ul;
            break;
        case UsartParityEven:
            CR1_f.PS = (uint32_t)0ul;
            CR1_f.PCE = (uint32_t)1ul;
            break;
        case UsartParityOdd:
            CR1_f.PS = (uint32_t)1ul;
            CR1_f.PCE = (uint32_t)1ul;
            break;
        default:
            break;
        }

        CR3_f.CTSE = (uint32_t)(pstcInitCfg->enHwFlow);
        CR1_f.SBS = (uint32_t)(pstcInitCfg->enDetectMode);
        CR1_f.OVER8 = (uint32_t)(pstcInitCfg->enSampleMode);

        USARTx->PR_f = PR_f;
        USARTx->CR1_f = CR1_f;
        USARTx->CR2_f = CR2_f;
        USARTx->CR3_f = CR3_f;
        enRet = Ok;
    }

    return enRet;
}
/**
 *******************************************************************************
 ** \brief Initialize clock sync mode of the specified USART.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] pstcInitCfg             Pointer to clock sync mode configure structure
 ** \arg This parameter detail refer @ref stc_usart_clksync_init_t
 **
 ** \retval Ok                          USART is initialized normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - USARTx is invalid
 **                                     - pstcInitCfg == NULL
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t USART_CLKSYNC_Init(M4_USART_TypeDef *USARTx,
                               const stc_usart_clksync_init_t *pstcInitCfg)
{
    stc_usart_pr_field_t PR_f = {0};
    stc_usart_cr1_field_t CR1_f = {0};
    stc_usart_cr2_field_t CR2_f = {0};
    stc_usart_cr3_field_t CR3_f = {0};
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx && pstcInitCfg pointer */
    if ((IS_VALID_USART(USARTx)) && (NULL != pstcInitCfg))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_USART_CLK_DIV(pstcInitCfg->enClkDiv));
        DDL_ASSERT(IS_VALID_USART_CLK_MODE(pstcInitCfg->enClkMode));
        DDL_ASSERT(IS_VALID_USART_DATA_DIR(pstcInitCfg->enDirection));
        DDL_ASSERT(IS_VALID_USART_HW_FLOW_MODE(pstcInitCfg->enHwFlow));

        /* Set default value */
        USARTx->CR1 = (uint32_t)0xFFFFFFF3ul;
        USARTx->CR1 = (uint32_t)0x80000000ul;
        USARTx->CR2 = (uint32_t)0x00000000ul;
        USARTx->CR3 = (uint32_t)0x00000000ul;
        USARTx->BRR = (uint32_t)0x0000FFFFul;
        USARTx->PR = (uint32_t)0x00000000ul;

        /* Set Clock Sync mode */
        CR3_f.SCEN = (uint32_t)0ul;
        CR1_f.MS = (uint32_t)1ul;
        CR1_f.ML = (uint32_t)(pstcInitCfg->enDirection);
        PR_f.PSC = (uint32_t)(pstcInitCfg->enClkDiv);
        CR2_f.CLKC = (uint32_t)(pstcInitCfg->enClkMode);
        CR3_f.CTSE = (uint32_t)(pstcInitCfg->enHwFlow);

        USARTx->PR_f = PR_f;
        USARTx->CR1_f = CR1_f;
        USARTx->CR2_f = CR2_f;
        USARTx->CR3_f = CR3_f;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Initialize smart card mode of the specified USART.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] pstcInitCfg             Pointer to smart card mode configure structure
 ** \arg This parameter detail refer @ref stc_usart_sc_init_t
 **
 ** \retval Ok                          USART is initialized normally
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - USARTx is invalid
 **                                     - pstcInitCfg == NULL
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t USART_SC_Init(M4_USART_TypeDef *USARTx,
                          const stc_usart_sc_init_t *pstcInitCfg)
{
    stc_usart_pr_field_t PR_f = {0};
    stc_usart_cr1_field_t CR1_f = {0};
    stc_usart_cr2_field_t CR2_f = {0};
    stc_usart_cr3_field_t CR3_f = {0};
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx && pstcInitCfg pointer */
    if ((IS_VALID_USART(USARTx)) && (NULL != pstcInitCfg))
    {
        /* Check parameters */
        DDL_ASSERT(IS_VALID_USART_CLK_DIV(pstcInitCfg->enClkDiv));
        DDL_ASSERT(IS_VALID_USART_CLK_MODE(pstcInitCfg->enClkMode));
        DDL_ASSERT(IS_VALID_USART_DATA_DIR(pstcInitCfg->enDirection));

        /* Set default value */
        USARTx->CR1 = (uint32_t)0xFFFFFFF3ul;
        USARTx->CR1 = (uint32_t)0x80000000ul;
        USARTx->CR2 = (uint32_t)0x00000000ul;
        USARTx->CR3 = (uint32_t)0x00000000ul;
        USARTx->BRR = (uint32_t)0x0000FFFFul;
        USARTx->PR = (uint32_t)0x00000000ul;

        CR1_f.PCE = (uint32_t)1ul;
        CR1_f.ML = (uint32_t)(pstcInitCfg->enDirection);
        CR2_f.CLKC = (uint32_t)(pstcInitCfg->enClkMode);
        CR3_f.SCEN = (uint32_t)1ul;             /* Set USART mode */
        CR3_f.BCN = (uint32_t)UsartScEtuClk372; /* ETU = 372 * CK */
        PR_f.PSC = (uint32_t)(pstcInitCfg->enClkDiv);

        USARTx->PR_f = PR_f;
        USARTx->CR1_f = CR1_f;
        USARTx->CR2_f = CR2_f;
        USARTx->CR3_f = CR3_f;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief De-Initializes the specified USART.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval Ok                          USART is de-initialized normally
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_DeInit(M4_USART_TypeDef *USARTx)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check for USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Set default value */
        USARTx->CR1 = (uint32_t)0xFFFFFFF3ul;
        USARTx->CR1 = (uint32_t)0x80000000ul;
        USARTx->CR2 = (uint32_t)0x00000000ul;
        USARTx->CR3 = (uint32_t)0x00000000ul;
        USARTx->BRR = (uint32_t)0x0000FFFFul;
        USARTx->PR = (uint32_t)0x00000000ul;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get flag status
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enStatus                Choose need get status's flag
 ** \arg UsartParityError               Parity check error
 ** \arg UsartFrameError                Frame error
 ** \arg UsartOverRunError              Overrun error
 ** \arg UsartRxRegNoEmpty              Rx register is no empty
 ** \arg UsartTxComplete                Transfer completely
 ** \arg UsartTxRegNoEmpty              Tx register is no empty
 ** \arg UsartRxTimeOut                 Data receive timeout
 ** \arg UsartRxDataType                Data is multiple processor id or normal data.
 **
 ** \retval Set                         Flag is set.
 ** \retval Reset                       Flag is reset or enStatus is invalid.
 **
 ******************************************************************************/
en_flag_status_t USART_GetStatus(M4_USART_TypeDef *USARTx,
                                 en_usart_status_t enStatus)
{
    /* Check parameter */
    DDL_ASSERT(IS_VALID_USART(USARTx));
    DDL_ASSERT(IS_VALID_USART_STATUS(enStatus));

    return ((USARTx->SR & enStatus) ? Set : Reset);
}

/**
 *******************************************************************************
 ** \brief Clear the specified USART status
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enStatus                The specified status
 ** \arg UsartParityErr                 Parity check error
 ** \arg UsartFrameErr                  Frame error
 ** \arg UsartOverRunErr                Overrun error
 ** \arg UsartRxTimeOut                 Data receive timeout
 **
 ** \retval Ok                          Clear flag successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - USARTx is invalid
 **                                     - enStatus is invalid
 **
 ******************************************************************************/
en_result_t USART_ClearStatus(M4_USART_TypeDef *USARTx,
                              en_usart_status_t enStatus)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        enRet = Ok;
        switch (enStatus)
        {
        case UsartParityErr:
            USARTx->CR1_f.CPE = 1ul;
            break;
        case UsartFrameErr:
            USARTx->CR1_f.CFE = 1ul;
            break;
        case UsartOverrunErr:
            USARTx->CR1_f.CORE = 1ul;
            break;
        case UsartRxTimeOut:
            USARTx->CR1_f.CRTOF = 1ul;
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
 ** \brief Configure USART function.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enFunc                  USART function selection
 ** \arg UsartTimeOut                   UART RX timeout function
 ** \arg UsartTimeOutInt                UART RX timeout interrupt function
 ** \arg UsartRx                        UART RX function
 ** \arg UsartTx                        UART TX function
 ** \arg UsartSilentMode                USART silent function
 ** \arg UsartRxInt                     USART RX interrupt function
 ** \arg UsartTxCmpltInt                USART TX complete interrupt function
 ** \arg UsartTxEmptyInt                USART TX empty interrupt function
 ** \arg UsartParityCheck               USART Parity check function
 ** \arg UsartFracBaudrate              USART fractional baudrate function
 ** \arg UsartNoiseFilter               USART noise filter function
 ** \param [in] enCmd                   USART functional state
 ** \arg Enable                         Enable the specified USART function
 ** \arg Disable                        Disable the specified USART function
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - USARTx == NULL
 **
 ******************************************************************************/
en_result_t USART_FuncCmd(M4_USART_TypeDef *USARTx,
                          en_usart_func_t enFunc,
                          en_functional_state_t enCmd)
{
    uint32_t u32Addr;
    __IO stc_usart_cr1_field_t CR1_f;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        enRet = Ok;
        switch (enFunc)
        {
        case UsartRx:
            USARTx->CR1_f.RE = (uint32_t)enCmd;
            break;
        case UsartRxInt:
            USARTx->CR1_f.RIE = (uint32_t)enCmd;
            break;
        case UsartTx:
            USARTx->CR1_f.TE = (uint32_t)enCmd;
            break;
        case UsartTxEmptyInt:
            USARTx->CR1_f.TXEIE = (uint32_t)enCmd;
            break;
        case UsartTimeOut:
            USARTx->CR1_f.RTOE = (uint32_t)enCmd;
            break;
        case UsartTimeOutInt:
            USARTx->CR1_f.RTOIE = (uint32_t)enCmd;
            break;
        case UsartSilentMode:
            USARTx->CR1_f.SLME = (uint32_t)enCmd;
            break;
        case UsartParityCheck:
            USARTx->CR1_f.PCE = (uint32_t)enCmd;
            break;
        case UsartNoiseFilter:
            USARTx->CR1_f.NFE = (uint32_t)enCmd;
            break;
        case UsartTxCmpltInt:
            USARTx->CR1_f.TCIE = (uint32_t)enCmd;
            break;
        case UsartTxAndTxEmptyInt:
            CR1_f = USARTx->CR1_f;
            CR1_f.TE = (uint32_t)enCmd;
            CR1_f.TXEIE = (uint32_t)enCmd;
            u32Addr = (uint32_t)&CR1_f;
            USARTx->CR1 = *(__IO uint32_t *)u32Addr;
            break;
        case UsartFracBaudrate:
            USARTx->CR1_f.FBME = (uint32_t)enCmd;
            break;
        case UsartMulProcessor:
            USARTx->CR2_f.MPE = (uint32_t)enCmd;
            break;
        case UsartSmartCard:
            USARTx->CR3_f.SCEN = (uint32_t)enCmd;
            break;
        case UsartCts:
            USARTx->CR3_f.CTSE = (uint32_t)enCmd;
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
 ** \brief Set USART parity bit.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enParity                USART parity selection
 ** \arg UsartParityNone                USART none parity
 ** \arg UsartParityEven                USART even parity
 ** \arg UsartParityOdd                 USART odd parity
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - USARTx is invalid
 **                                     - enParity is invalid
 **
 ******************************************************************************/
en_result_t USART_SetParity(M4_USART_TypeDef *USARTx,
                            en_usart_parity_t enParity)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        enRet = Ok;
        switch (enParity)
        {
        case UsartParityNone:
            USARTx->CR1_f.PCE = (uint32_t)0ul;
            break;
        case UsartParityEven:
            USARTx->CR1_f.PS = (uint32_t)0ul;
            USARTx->CR1_f.PCE = (uint32_t)1u;
            break;
        case UsartParityOdd:
            USARTx->CR1_f.PS = (uint32_t)1ul;
            USARTx->CR1_f.PCE = (uint32_t)1ul;
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
 ** \brief Get USART parity bit.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartParityNone             USART none parity
 ** \retval UsartParityEven             USART even parity
 ** \retval UsartParityOdd              USART odd parity
 **
 ******************************************************************************/
en_usart_parity_t USART_GetParity(M4_USART_TypeDef *USARTx)
{
    en_usart_parity_t enParity = UsartParityNone;

    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    if (0ul == USARTx->CR1_f.PCE)
    {
        enParity = UsartParityNone;
    }
    else if (0ul == USARTx->CR1_f.PS)
    {
        enParity = UsartParityEven;
    }
    else
    {
        enParity = UsartParityOdd;
    }

    return enParity;
}

/**
 *******************************************************************************
 ** \brief Set USART over sampling.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enSampleMode            USART parity selection
 ** \arg UsartSamleBit16                16 Bit
 ** \arg UsartSamleBit8                 8 Bit
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetOverSampling(M4_USART_TypeDef *USARTx,
                                  en_usart_sample_mode_t enSampleMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_SAMPLE_MODE(enSampleMode));

        USARTx->CR1_f.OVER8 = (uint32_t)enSampleMode;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get USART over sampling.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartSamleBit16             16 Bit
 ** \retval UsartSamleBit8              8 Bit
 **
 ******************************************************************************/
en_usart_sample_mode_t USART_GetOverSampling(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return (en_usart_sample_mode_t)USARTx->CR1_f.OVER8;
}

/**
 *******************************************************************************
 ** \brief Set USART data transfer direction.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enDir                   USART data direction selection
 ** \arg UsartDataLsbFirst              USART data LSB first
 ** \arg UsartDataMsbFirst              USART data MSB first
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetTransferDirection(M4_USART_TypeDef *USARTx,
                                       en_usart_data_dir_t enDir)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_DATA_DIR(enDir));

        USARTx->CR1_f.ML = (uint32_t)enDir;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get USART data transfer direction.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartDataLsbFirst              USART data LSB first
 ** \retval UsartDataMsbFirst              USART data MSB first
 **
 ******************************************************************************/
en_usart_data_dir_t USART_GetTransferDirection(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return (en_usart_data_dir_t)(USARTx->CR1_f.ML);
}

/**
 *******************************************************************************
 ** \brief Set USART data bit length.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enDataLen               USART data bit length
 ** \arg UsartDataBits8                 8 Bit
 ** \arg UsartDataBits8                 9 Bit
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetDataLength(M4_USART_TypeDef *USARTx,
                                en_usart_data_len_t enDataLen)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_DATA_LEN(enDataLen));

        USARTx->CR1_f.M = (uint32_t)enDataLen;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get USART data bit length.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartDataBits8              Data bit length:8 Bits
 ** \retval UsartDataBits8              Data bit length:9 Bits
 **
 ******************************************************************************/
en_usart_data_len_t USART_GetDataLength(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return (en_usart_data_len_t)(USARTx->CR1_f.M);
}

/**
 *******************************************************************************
 ** \brief Set USART clock mode.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enClkMode               USART clock mode selection
 ** \arg UsartExtClk                    Select external clock source
 ** \arg UsartIntClkCkOutput            Select internal clock source and output clock
 ** \arg UsartIntClkCkNoOutput          Select internal clock source and don't output clock
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetClkMode(M4_USART_TypeDef *USARTx,
                             en_usart_clk_mode_t enClkMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_CLK_MODE(enClkMode));

        USARTx->CR2_f.CLKC = (uint32_t)enClkMode;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get USART clock mode.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartExtClk                 Select external clock source
 ** \retval UsartIntClkCkOutput         Select internal clock source and output clock
 ** \retval UsartIntClkCkNoOutput       Select internal clock source and don't output clock
 **
 ******************************************************************************/
en_usart_clk_mode_t USART_GetClkMode(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return (en_usart_clk_mode_t)(USARTx->CR2_f.CLKC);
}

/**
 *******************************************************************************
 ** \brief Set USART mode.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enMode                  USART clock mode selection
 ** \arg UsartUartMode                  UART mode
 ** \arg UsartClkSyncMode               Clock sync mode
 ** \arg UsartSmartCardMode             Smart card mode
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - USARTx is invalid
 **                                     - enMode is invalid
 **
 ******************************************************************************/
en_result_t USART_SetMode(M4_USART_TypeDef *USARTx,
                          en_usart_mode_t enMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        enRet = Ok;
        switch (enMode)
        {
        case UsartUartMode:
            USARTx->CR3_f.SCEN = (uint32_t)0ul;
            USARTx->CR1_f.MS = (uint32_t)0ul;
            break;
        case UsartClkSyncMode:
            USARTx->CR3_f.SCEN = (uint32_t)0ul;
            USARTx->CR1_f.MS = (uint32_t)1ul;
            break;
        case UsartSmartCardMode:
            USARTx->CR3_f.SCEN = (uint32_t)1ul;
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
 ** \brief Get USART mode.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartUartMode               UART mode
 ** \retval UsartClkSyncMode            Clock sync mode
 ** \retval UsartSmartCardMode          Smart card mode
 **
 ******************************************************************************/
en_usart_mode_t USART_GetMode(M4_USART_TypeDef *USARTx)
{
    en_usart_mode_t enMode;

    /* Check parameter */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    if (1ul == USARTx->CR3_f.SCEN)
    {
        enMode = UsartSmartCardMode;
    }
    else if (1ul == USARTx->CR1_f.MS)
    {
        enMode = UsartClkSyncMode;
    }
    else
    {
        enMode = UsartUartMode;
    }

    return enMode;
}

/**
 *******************************************************************************
 ** \brief Set USART stop bit length.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enStopBit               USART stop bit selection
 ** \arg UsartOneStopBit                1 Stop Bit
 ** \arg UsartTwoStopBits               2 Stop Bit
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetStopBitsLength(M4_USART_TypeDef *USARTx,
                                    en_usart_stop_bit_t enStopBit)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_STOP_BIT(enStopBit));

        USARTx->CR2_f.STOP = (uint32_t)enStopBit;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get USART stop bit length.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartOneStopBit             1 Stop Bit
 ** \retval UsartTwoStopBits            2 Stop Bit
 **
 ******************************************************************************/
en_usart_stop_bit_t USART_GetStopBitsLength(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return (en_usart_stop_bit_t)(USARTx->CR2_f.STOP);
}

/**
 *******************************************************************************
 ** \brief Set USART detect mode.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enDetectMode            USART start bit detect mode
 ** \arg UsartStartBitLowLvl            Start bit: RD pin low level
 ** \arg UsartStartBitFallEdge          Start bit: RD pin falling edge
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetSbDetectMode(M4_USART_TypeDef *USARTx,
                                  en_usart_sb_detect_mode_t enDetectMode)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_SB_DETECT_MODE(enDetectMode));

        USARTx->CR1_f.SBS = (uint32_t)enDetectMode;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get USART detect mode.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartStartBitLowLvl         Start bit: RD pin low level
 ** \retval UsartStartBitFallEdge       Start bit: RD pin falling edge
 **
 ******************************************************************************/
en_usart_sb_detect_mode_t USART_GetSbDetectMode(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return (en_usart_sb_detect_mode_t)(USARTx->CR1_f.SBS);
}

/**
 *******************************************************************************
 ** \brief Set USART hardware flow control.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enHwFlowCtrl            Hardware flow control
 ** \arg UsartRtsEnable                 Enable RTS
 ** \arg UsartCtsEnable                 Enable CTS
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetHwFlowCtrl(M4_USART_TypeDef *USARTx,
                                en_usart_hw_flow_ctrl_t enHwFlowCtrl)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_HW_FLOW_MODE(enHwFlowCtrl));

        USARTx->CR3_f.CTSE = (uint32_t)enHwFlowCtrl;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get USART hardware flow control.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartRtsEnable              Enable RTS
 ** \retval UsartCtsEnable              Enable CTS
 **
 ******************************************************************************/
en_usart_hw_flow_ctrl_t USART_GetHwFlowCtrl(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return (en_usart_hw_flow_ctrl_t)(USARTx->CR3_f.CTSE);
}

/**
 *******************************************************************************
 ** \brief Set USART clock prescale.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enClkPrescale           USART clock prescale
 ** \arg UsartClkDiv_0                  PCLK/1
 ** \arg UsartClkDiv_4                  PCLK/4
 ** \arg UsartClkDiv_16                 PCLK/16
 ** \arg UsartClkDiv_64                 PCLK/64
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetClockDiv(M4_USART_TypeDef *USARTx,
                              en_usart_clk_div_t enClkPrescale)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_CLK_DIV(enClkPrescale));

        USARTx->PR_f.PSC = (uint32_t)enClkPrescale;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get USART clock division.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartClkDiv_0               PCLK/1
 ** \retval UsartClkDiv_4               PCLK/4
 ** \retval UsartClkDiv_16              PCLK/16
 ** \retval UsartClkDiv_64              PCLK/64
 **
 ******************************************************************************/
en_usart_clk_div_t USART_GetClockDiv(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return (en_usart_clk_div_t)(USARTx->PR_f.PSC);
}

/**
 *******************************************************************************
 ** \brief Set USART ETU clocks of smart card.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] enEtuClk                ETU clocks of smart card
 ** \arg UsartScEtuClk32                1 etu = 32/f
 ** \arg UsartScEtuClk64                1 etu = 64/f
 ** \arg UsartScEtuClk93                1 etu = 93/f
 ** \arg UsartScEtuClk128               1 etu = 128/f
 ** \arg UsartScEtuClk186               1 etu = 186/f
 ** \arg UsartScEtuClk256               1 etu = 256/f
 ** \arg UsartScEtuClk372               1 etu = 372/f
 ** \arg UsartScEtuClk512               1 etu = 512/f
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetScEtuClk(M4_USART_TypeDef *USARTx,
                              en_usart_sc_etu_clk_t enEtuClk)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        /* Check parameter */
        DDL_ASSERT(IS_VALID_USART_ETU_CLK(enEtuClk));

        USARTx->CR3_f.BCN = (uint32_t)enEtuClk;
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set USART ETU clocks of smart card.
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval UsartScEtuClk32             1 etu = 32/f
 ** \retval UsartScEtuClk64             1 etu = 64/f
 ** \retval UsartScEtuClk93             1 etu = 93/f
 ** \retval UsartScEtuClk128            1 etu = 128/f
 ** \retval UsartScEtuClk186            1 etu = 186/f
 ** \retval UsartScEtuClk256            1 etu = 256/f
 ** \retval UsartScEtuClk372            1 etu = 372/f
 ** \retval UsartScEtuClk512            1 etu = 512/f
 **
 ******************************************************************************/
en_usart_sc_etu_clk_t USART_GetScEtuClk(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return (en_usart_sc_etu_clk_t)(USARTx->CR3_f.BCN);
}

/**
 ******************************************************************************
 ** \brief Write UART data buffer
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] u16Data                 Send data
 **
 ** \retval Ok                          Data has been successfully sent
 **
 ******************************************************************************/
en_result_t USART_SendData(M4_USART_TypeDef *USARTx, uint16_t u16Data)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    USARTx->DR_f.TDR = (uint32_t)u16Data;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Read UART data buffer
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval Receive data
 **
 ******************************************************************************/
uint16_t USART_RecData(M4_USART_TypeDef *USARTx)
{
    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    return ((uint16_t)(USARTx->DR_f.RDR));
}

/**
 *******************************************************************************
 ** \brief Set USART baudrate
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] u32Baudrate             Baudrate
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
en_result_t USART_SetBaudrate(M4_USART_TypeDef *USARTx,
                              uint32_t u32Baudrate)
{
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        if (1ul == USARTx->CR3_f.SCEN)
        {
            enRet = SetScBaudrate(USARTx, u32Baudrate);
        }
        else if (1ul == USARTx->CR1_f.MS)
        {
            enRet = SetClkSyncBaudrate(USARTx, u32Baudrate);
        }
        else
        {
            enRet = SetUartBaudrate(USARTx, u32Baudrate);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set UART mode baudrate
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] u32Baudrate             Baudrate
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
static en_result_t SetUartBaudrate(M4_USART_TypeDef *USARTx,
                                   uint32_t u32Baudrate)
{
    uint32_t B = 0ul;
    uint32_t C = 0ul;
    uint32_t OVER8 = 0ul;
    float32_t DIV = 0.0f;
    uint64_t u64Tmp = 0u;
    uint32_t DIV_Integer = 0ul;
    uint32_t DIV_Fraction = 0xFFFFFFFFul;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        C = UsartGetClk(USARTx);

        if (C > 0ul)
        {
            B = u32Baudrate;
            OVER8 = USARTx->CR1_f.OVER8;
            /* FBME = 0 Calculation formula */
            /* B = C / (8 * (2 - OVER8) * (DIV_Integer + 1)) */
            /* DIV_Integer = (C / (B * 8 * (2 - OVER8))) - 1 */
            DIV = ((float)C / ((float)B * 8.0f * (2.0f - (float)OVER8))) - 1.0f;
            DIV_Integer = (uint32_t)(DIV);

            if (!((DIV < 0.0f) || (DIV_Integer > 0xFFul)))
            {
                enRet = Ok;
                if ((DIV - (float32_t)DIV_Integer) > 0.00001f)
                {
                    /* FBME = 1 Calculation formula */
                    /* B = C * (128 + DIV_Fraction) / (8 * (2 - OVER8) * (DIV_Integer + 1) * 256) */
                    /* DIV_Fraction = ((8 * (2 - OVER8) * (DIV_Integer + 1) * 256 * B) / C) - 128 */
                    /* E = (C * (128 + DIV_Fraction) / (8 * (2 - OVER8) * (DIV_Integer + 1) * 256 * B)) - 1 */
                    /* DIV_Fraction = (((2 - OVER8) * (DIV_Integer + 1) * 2048 * B) / C) - 128 */
                    u64Tmp = (uint64_t)(((uint64_t)2ul - (uint64_t)OVER8) * ((uint64_t)DIV_Integer + 1ul) * (uint64_t)B);
                    DIV_Fraction = (uint32_t)(2048ul * u64Tmp / C - 128ul);
                    if (DIV_Fraction > 0x7Ful)
                    {
                        enRet = ErrorInvalidParameter;
                    }
                }

                if (Ok == enRet)
                {
                    USARTx->CR1_f.FBME = (DIV_Fraction > 0x7Ful) ? 0ul : 1ul;
                    USARTx->BRR_f.DIV_FRACTION = DIV_Fraction;
                    USARTx->BRR_f.DIV_INTEGER = DIV_Integer;
                }
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set smart card mode baudrate
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] u32Baudrate             Baudrate
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
static en_result_t SetScBaudrate(M4_USART_TypeDef *USARTx,
                                 uint32_t u32Baudrate)
{
    uint32_t B = 0ul;
    uint32_t C = 0ul;
    uint32_t S = 0ul;
    float32_t DIV = 0.0f;
    uint64_t u64Tmp = 0u;
    uint32_t DIV_Integer = 0ul;
    uint32_t DIV_Fraction = 0xFFFFFFFFul;
    const uint16_t au16EtuClkCnts[] = {32u, 64u, 93u, 128u, 186u, 256u, 372u, 512u};
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        C = UsartGetClk(USARTx);

        if (C > 0ul)
        {
            B = u32Baudrate;
            S = au16EtuClkCnts[USARTx->CR3_f.BCN];

            /* FBME = 0 Calculation formula */
            /* B = C / (2 * S * (DIV_Integer + 1)) */
            /* DIV_Integer = (C / (B * 2 * S)) - 1 */
            DIV = ((float)C / ((float)B * (float)S * 2.0f)) - 1.0f;
            DIV_Integer = (uint32_t)DIV;

            if (!((DIV < 0.0f) || (DIV_Integer > 0xFFul)))
            {
                enRet = Ok;
                if ((DIV - (float32_t)DIV_Integer) > 0.00001f)
                {
                    /* FBME = 1 Calculation formula */
                    /* B = C * (128 + DIV_Fraction) / ((2 * S) * (DIV_Integer + 1) * 256) */
                    /* DIV_Fraction = ((2 * S) * (DIV_Integer + 1) * 256 * B / C) - 128 */
                    /* DIV_Fraction = ((DIV_Integer + 1) * B * S * 512 / C) - 128 */
                    u64Tmp = (uint64_t)(((uint64_t)DIV_Integer + 1ul) * B * S);
                    DIV_Fraction = (uint32_t)(512ul * u64Tmp / C - 128ul);
                    if (DIV_Fraction > 0x7Ful)
                    {
                        enRet = ErrorInvalidParameter;
                    }
                }

                if (Ok == enRet)
                {
                    USARTx->CR1_f.FBME = (DIV_Fraction > 0x7Ful) ? 0ul : 1ul;
                    USARTx->BRR_f.DIV_FRACTION = DIV_Fraction;
                    USARTx->BRR_f.DIV_INTEGER = DIV_Integer;
                }
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set synchronous clock mode baudrate
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 ** \param [in] u32Baudrate             Baudrate
 **
 ** \retval Ok                          Configure successfully.
 ** \retval ErrorInvalidParameter       USARTx is invalid
 **
 ******************************************************************************/
static en_result_t SetClkSyncBaudrate(M4_USART_TypeDef *USARTx,
                                      uint32_t u32Baudrate)
{
    uint32_t C = 0ul;
    uint32_t B = 0ul;
    uint64_t u64Tmp = 0u;
    float32_t DIV = 0.0f;
    uint32_t DIV_Integer = 0ul;
    uint32_t DIV_Fraction = 0xFFFFFFFFul;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check USARTx pointer */
    if (IS_VALID_USART(USARTx))
    {
        C = UsartGetClk(USARTx);
        if (C > 0ul)
        {
            B = u32Baudrate;

            /* FBME = 0 Calculation formula */
            /* B = C / (4 * (DIV_Integer + 1)) */
            /* DIV_Integer = (C / (B * 4)) - 1 */
            DIV = ((float)C / ((float)B * 4.0f)) - 1.0f;
            DIV_Integer = (uint32_t)DIV;

            if (!((DIV < 0.0f) || (DIV_Integer > 0xFFul)))
            {
                enRet = Ok;
                if ((DIV - (float32_t)DIV_Integer) > 0.00001f)
                {
                    /* FBME = 1 Calculation formula */
                    /* B = C * (128 + DIV_Fraction) / (4 * (DIV_Integer + 1) * 256) */
                    /* DIV_Fraction = (4 * (DIV_Integer + 1) * 256 * B / C) - 128 */
                    /* DIV_Fraction = ((DIV_Integer + 1) * B * 1024 / C) - 128 */
                    u64Tmp = (uint64_t)(((uint64_t)DIV_Integer + 1ul) * (uint64_t)B);
                    DIV_Fraction = (uint32_t)(1024ul * u64Tmp / C - 128ul);
                    if (DIV_Fraction > 0x7Ful)
                    {
                        enRet = ErrorInvalidParameter;
                    }
                }

                if (Ok == enRet)
                {
                    USARTx->CR1_f.FBME = (DIV_Fraction > 0x7Ful) ? 0ul : 1ul;
                    USARTx->BRR_f.DIV_FRACTION = DIV_Fraction;
                    USARTx->BRR_f.DIV_INTEGER = DIV_Integer;
                }
            }
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get USART clock
 **
 ** \param [in] USARTx                  Pointer to USART instance register base
 ** \arg M4_USART1                      USART unit 1 instance register base
 ** \arg M4_USART2                      USART unit 2 instance register base
 ** \arg M4_USART3                      USART unit 3 instance register base
 ** \arg M4_USART4                      USART unit 4 instance register base
 **
 ** \retval USART clock frequency
 **
 ******************************************************************************/
static uint32_t UsartGetClk(const M4_USART_TypeDef *USARTx)
{
    uint32_t u32PClk1 = 0ul;
    uint32_t u32UartClk = 0ul;

    /* Check USARTx pointer */
    DDL_ASSERT(IS_VALID_USART(USARTx));

    u32PClk1 = SystemCoreClock / (1ul << M4_SYSREG->CMU_SCFGR_f.PCLK1S);
    u32UartClk = u32PClk1 / (1ul << (2ul * USARTx->PR_f.PSC));

    return u32UartClk;
}

//@} // UsartGroup

#endif /* DDL_USART_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
