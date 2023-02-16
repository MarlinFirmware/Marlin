

#include "../board/startup.h"
#include "usart.h"

stc_usart_uart_init_t stcInitCfg = {
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

ring_buffer usart4_rb;
ring_buffer usart4_wb;
usart_dev usart4 = {
    .regs = SCREEN_UART,
    .rb = &usart4_rb,
    .wb = &usart4_wb,
    .max_baud = 2250000UL,
    .rx_buf = {0},
    .tx_buf = {0},
    .clk_id = SCREEN_CLK,
    .pstcInitCfg = &stcInitCfg,
    .RX_IRQ = Int002_IRQn,
    .TX_IRQ = Int003_IRQn,
    .RX_error_IRQ = Int020_IRQn,
    .TX_complete_IRQ = Int021_IRQn,
    .IRQ_priority = DDL_IRQ_PRIORITY_02,
};
/** USART4 device */
usart_dev *USART4 = &usart4;

ring_buffer usart2_rb;
ring_buffer usart2_wb;
usart_dev usart2 = {
    .regs = PRINT_UART,
    .rb = &usart2_rb,
    .wb = &usart2_wb,
    .max_baud = 2250000UL,
    .rx_buf = {0},
    .tx_buf = {0},
    .clk_id = PRINT_CLK,
    .pstcInitCfg = &stcInitCfg,
    .RX_IRQ = Int004_IRQn,
    .TX_IRQ = Int005_IRQn,
    .RX_error_IRQ = Int022_IRQn,
    .TX_complete_IRQ = Int023_IRQn,
    .IRQ_priority = DDL_IRQ_PRIORITY_06,
};
/** USART2 device */
usart_dev *USART2 = &usart2;

ring_buffer usart3_rb;
ring_buffer usart3_wb;
usart_dev usart3 = {
    .regs = M4_USART3,
    .rb = &usart3_rb,
    .wb = &usart3_wb,
    .max_baud = 2250000UL,
    .rx_buf = {0},
    .tx_buf = {0},
    .clk_id = PWC_FCG1_PERIPH_USART3,
    .pstcInitCfg = &stcInitCfg,
    .RX_IRQ = Int008_IRQn,
    .TX_IRQ = Int009_IRQn,
    .RX_error_IRQ = Int024_IRQn,
    .TX_complete_IRQ = Int025_IRQn,
    .IRQ_priority = DDL_IRQ_PRIORITY_05,
};
/** USART3 device */
usart_dev *USART3 = &usart3;

void usart_foreach(void (*fn)(usart_dev *))
{
    fn(USART2);
    fn(USART3);
    fn(USART4);
}
