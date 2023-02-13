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
/** \file hc32f46x_utility.c
 **
 ** A detailed description is available at
 ** @link DdlUtilityGroup Ddl Utility description @endlink
 **
 **   - 2018-11-02  1.0  Zhangxl First version for Device Driver Library Utility.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "hc32f46x_utility.h"
#include "hc32f46x_usart.h"

#if (DDL_UTILITY_ENABLE == DDL_ON)

#define MYSERIAL_DEBUG M4_USART2
#define MYM4_MSTP M4_MSTP->FCG1_f.USART2
#define DBG_PORT PortA
#define DBG_PIN Pin09
#define DBG_FUNC Func_Usart2_Tx
#define DBG_PERIPH_CLK PWC_FCG1_PERIPH_USART2
/**
 *******************************************************************************
 ** \addtogroup DdlUtilityGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

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
#ifdef UART_DEBUG_PRINTF
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \brief Data printf via Uart Ch.3
     **
     ** \param [in]  u8Data                 Data to be sent
     **
     ******************************************************************************/
    void DebugOutput(uint8_t u8Data)
    {
        uint32_t timerout = 0;
        MYSERIAL_DEBUG->DR = u8Data;
        while (0ul == MYSERIAL_DEBUG->SR_f.TC)
        {
            if (++timerout > 5000)
                return;
        }
    }

/**
 *******************************************************************************
 ** \brief Re-target putchar function
 **
 ******************************************************************************/
#if defined(__GNUC__) && !defined(__CC_ARM)
    int _write(int fd, char *pBuffer, int size)
    {
        for (int i = 0; i < size; i++)
        {
            DebugOutput((uint8_t)pBuffer[i]);
        }
        return size;
    }
#else
int32_t fputc(int32_t ch, FILE *f)
{
    DebugOutput((uint8_t)ch);

    return (ch);
}
#endif
    /**
     *******************************************************************************
     ** \brief Set synchronous clock mode baudrate
     **
     ** \param [in] u32Baudrate             Baudrate
     **
     ** \retval Ok                          Configure successfully.
     ** \retval ErrorInvalidParameter       USARTx is invalid
     **
     ******************************************************************************/

    stc_usart_uart_init_t debug_stcInitCfg = {
        UsartIntClkCkNoOutput,
        UsartClkDiv_1,
        UsartDataBits8,
        UsartDataLsbFirst,
        UsartOneStopBit,
        UsartParityNone,
        UsartSamleBit8,
        UsartStartBitFallEdge,
        UsartRtsEnable,
    };
    static en_result_t SetUartBaudrate(uint32_t u32Baudrate)
    {
        en_result_t enRet = Ok;
        uint32_t B;
        uint32_t C;
        uint32_t OVER8;
        float32_t DIV = 0.0f;
        uint64_t u64Tmp = 0u;
        uint32_t DIV_Integer = 0u;
        uint32_t DIV_Fraction = 0xFFFFFFFFul;

        uint32_t u32PClk1 = 0u;
        uint32_t u32UartClk = 0u;

        /* Enable peripheral clock */
        PWC_Fcg1PeriphClockCmd(DBG_PERIPH_CLK, Enable);
        /* Initialize UART */
        USART_UART_Init(MYSERIAL_DEBUG, &debug_stcInitCfg);

        u32PClk1 = SystemCoreClock / (1ul << (M4_SYSREG->CMU_SCFGR_f.PCLK1S));
        u32UartClk = u32PClk1 / (1ul << (2ul * (MYSERIAL_DEBUG->PR_f.PSC)));

        B = u32Baudrate;
        C = u32UartClk;

        if (0ul == C)
        {
            enRet = ErrorInvalidParameter;
        }
        else
        {
            OVER8 = MYSERIAL_DEBUG->CR1_f.OVER8;

            /* FBME = 0 Calculation formula */
            /* B = C / (8 * (2 - OVER8) * (DIV_Integer + 1)) */
            /* DIV_Integer = (C / (B * 8 * (2 - OVER8))) - 1 */
            DIV = ((float)C / ((float)B * 8.0f * (2.0f - (float)OVER8))) - 1.0f;
            DIV_Integer = (uint32_t)(DIV);

            if ((DIV < 0.0f) || (DIV_Integer > 0xFFul))
            {
                enRet = ErrorInvalidParameter;
            }
            else
            {
                if ((DIV - (float32_t)DIV_Integer) > 0.00001f)
                {
                    /* FBME = 1 Calculation formula */
                    /* B = C * (128 + DIV_Fraction) / (8 * (2 - OVER8) * (DIV_Integer + 1) * 256) */
                    /* DIV_Fraction = ((8 * (2 - OVER8) * (DIV_Integer + 1) * 256 * B) / C) - 128 */
                    /* E = (C * (128 + DIV_Fraction) / (8 * (2 - OVER8) * (DIV_Integer + 1) * 256 * B)) - 1 */
                    /* DIV_Fraction = (((2 - OVER8) * (DIV_Integer + 1) * 2048 * B) / C) - 128 */
                    u64Tmp = (2u - (uint64_t)OVER8) * ((uint64_t)DIV_Integer + 1u) * (uint64_t)B;
                    DIV_Fraction = (uint32_t)(2048ul * u64Tmp / C - 128ul);
                }
                else
                {
                }

                MYSERIAL_DEBUG->CR1_f.FBME = (0xFFFFFFFFul == DIV_Fraction) ? 0ul : 1ul;
                MYSERIAL_DEBUG->BRR_f.DIV_FRACTION = DIV_Fraction;
                MYSERIAL_DEBUG->BRR_f.DIV_INTEGER = DIV_Integer;
                enRet = Ok;
            }
        }
        return enRet;
    }

    /**
     *******************************************************************************
     ** \brief Debug printf initialization function
     **
     ** \retval Ok                          Process successfully done
     **
     ******************************************************************************/
    en_result_t Ddl_UartInit(void)
    {
        en_result_t enRet = Ok;

        /* usart3_tx gpio  PE5 */
        // M4_PORT->PFSRE5_f.FSEL  = 32u;
        PORT_SetFunc(DBG_PORT, DBG_PIN, DBG_FUNC, Disable);
        /* enable usart3 */
        MYM4_MSTP = 0;
        /* usart3 init */

        MYSERIAL_DEBUG->CR1_f.ML = 0ul;    // LSB
        MYSERIAL_DEBUG->CR1_f.MS = 0ul;    // UART mode
        MYSERIAL_DEBUG->CR1_f.OVER8 = 1ul; // 8bit sampling mode
        MYSERIAL_DEBUG->CR1_f.M = 0ul;     // 8 bit data length
        MYSERIAL_DEBUG->CR1_f.PCE = 0ul;   // no parity bit

        /* baudrate set */
        if (Ok != SetUartBaudrate(115200ul))
        {
            enRet = Error;
        }
        else
        {
            /* 1 stop bit, single uart mode */
            MYSERIAL_DEBUG->CR2 = 0ul;

            /* CTS disable, Smart Card mode disable */
            MYSERIAL_DEBUG->CR3 = 0ul;

            MYSERIAL_DEBUG->CR1_f.TE = 1ul; // TX enable
        }

        return enRet;
    }
#endif /* UART_DEBUG_PRINTF_ENABLE */
#ifdef __cplusplus
};
#endif

/**
 *******************************************************************************
 ** \brief Delay function, delay 1ms approximately
 **
 ** \param [in]  u32Cnt                 ms
 **
 ** \retval none
 **
 ******************************************************************************/
void Ddl_Delay1ms(uint32_t u32Cnt)
{
    volatile uint32_t i = 0ul;
    uint32_t u32Cyc = 0ul;

    u32Cyc = SystemCoreClock;
    u32Cyc = u32Cyc / 10000ul;
    while (u32Cnt-- > 0ul)
    {
        i = u32Cyc;
        while (i-- > 0ul)
        {
            ;
        }
    }
}

/**
 *******************************************************************************
 ** \brief Delay function, delay 1us approximately
 **
 ** \param [in]  u32Cnt                 us
 **
 ** \retval none
 **
 ******************************************************************************/
void Ddl_Delay1us(uint32_t u32Cnt)
{
    uint32_t u32Cyc = 1ul;
    volatile uint32_t i = 0ul;

    if (SystemCoreClock > 10000000ul)
    {
        u32Cyc = SystemCoreClock / 10000000ul;
        while (u32Cnt-- > 0ul)
        {
            i = u32Cyc;
            while (i-- > 0ul)
            {
                ;
            }
        }
    }
    else
    {
        while (u32Cnt-- > 0ul)
        {
            ;
        }
    }
}

/**
 *******************************************************************************
 ** \brief ddl assert error handle function
 **
 ** \param [in]  file                   Point to the current assert the wrong file
 ** \param [in]  line                   Point line assert the wrong file in the current
 **
 ******************************************************************************/
#ifdef __DEBUG
__WEAKDEF void Ddl_AssertHandler(uint8_t *file, int16_t line)
{
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    while (1)
    {
        ;
    }
}
#endif /* __DEBUG */

//@} // DdlUtilityGroup

#endif /* DDL_UTILITY_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
