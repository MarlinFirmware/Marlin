#include "usart.h"

// initial usart config
stc_usart_uart_init_t usartConf = {
    .enClkMode = UsartIntClkCkNoOutput,
    .enClkDiv = UsartClkDiv_1,
    .enDataLength = UsartDataBits8,
    .enDirection = UsartDataLsbFirst,
    .enStopBit = UsartOneStopBit,
    .enParity = UsartParityNone,
    .enSampleMode = UsartSamleBit8,
    .enDetectMode = UsartStartBitFallEdge,
    .enHwFlow = UsartRtsEnable};

// rx / tx buffer shorthand
#define CREATE_RX_BUFFER(name) DEFINE_RING_BUFFER(, name, USART_RX_BUF_SIZE)
#define CREATE_TX_BUFFER(name) DEFINE_RING_BUFFER(, name, USART_TX_BUF_SIZE)

//
// USART1
//
CREATE_RX_BUFFER(usart1_rb)
CREATE_TX_BUFFER(usart1_wb)
usart_dev usart1 = {
    .regs = M4_USART1,
    .rb = &usart1_rb,
    .wb = &usart1_wb,
    .clk_id = PWC_FCG1_PERIPH_USART1,
    .pstcInitCfg = &usartConf,
    .RX_IRQ = Int002_IRQn,
    .TX_IRQ = Int003_IRQn,
    .RX_error_IRQ = Int020_IRQn,
    .TX_complete_IRQ = Int021_IRQn,
    .IRQ_priority = DDL_IRQ_PRIORITY_02,
};
usart_dev *USART1 = &usart1;

//
// USART2
//
CREATE_RX_BUFFER(usart2_rb)
CREATE_TX_BUFFER(usart2_wb)
usart_dev usart2 = {
    .regs = M4_USART2,
    .rb = &usart2_rb,
    .wb = &usart2_wb,
    .clk_id = PWC_FCG1_PERIPH_USART2,
    .pstcInitCfg = &usartConf,
    .RX_IRQ = Int004_IRQn,
    .TX_IRQ = Int005_IRQn,
    .RX_error_IRQ = Int022_IRQn,
    .TX_complete_IRQ = Int023_IRQn,
    .IRQ_priority = DDL_IRQ_PRIORITY_06,
};
usart_dev *USART2 = &usart2;

//
// USART3
//
CREATE_RX_BUFFER(usart3_rb)
CREATE_TX_BUFFER(usart3_wb)
usart_dev usart3 = {
    .regs = M4_USART3,
    .rb = &usart3_rb,
    .wb = &usart3_wb,
    .clk_id = PWC_FCG1_PERIPH_USART3,
    .pstcInitCfg = &usartConf,
    .RX_IRQ = Int008_IRQn,
    .TX_IRQ = Int009_IRQn,
    .RX_error_IRQ = Int024_IRQn,
    .TX_complete_IRQ = Int025_IRQn,
    .IRQ_priority = DDL_IRQ_PRIORITY_05,
};
usart_dev *USART3 = &usart3;
