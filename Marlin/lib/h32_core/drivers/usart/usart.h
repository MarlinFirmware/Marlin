#pragma once
#include "../../hdsc/common/hc32_ddl.h"
#include "libmaple_types.h"
#include "../../RingBuffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef USART_RXTX_BUF_SIZE
#define USART_RXTX_BUF_SIZE 64
#endif

#ifndef USART_RX_BUF_SIZE
#define USART_RX_BUF_SIZE USART_RXTX_BUF_SIZE
#endif

#ifndef USART_TX_BUF_SIZE
#define USART_TX_BUF_SIZE USART_RXTX_BUF_SIZE
#endif

    typedef struct usart_dev
    {
        M4_USART_TypeDef *regs;
        RingBuffer *rb;
        RingBuffer *wb;
        uint32 clk_id;
        stc_usart_uart_init_t *pstcInitCfg;
        IRQn_Type RX_IRQ;
        IRQn_Type TX_IRQ;
        IRQn_Type RX_error_IRQ;
        IRQn_Type TX_complete_IRQ;
        uint32_t IRQ_priority;
    } usart_dev;

    // usart device variables
#define USART_DEV_VARS(nr)      \
    extern usart_dev usart##nr; \
    extern struct usart_dev *USART##nr;

    USART_DEV_VARS(1)
    USART_DEV_VARS(2)
    USART_DEV_VARS(3)

    // public api
    void usart_init(usart_dev *dev);
    void usart_set_baud_rate(usart_dev *dev, uint32 baud);
    void usart_enable(usart_dev *dev);
    void usart_disable(usart_dev *dev);
    uint32 usart_tx(usart_dev *dev, const uint8 *buf, uint32 len);
    uint32 usart_rx(usart_dev *dev, uint8 *buf, uint32 len);
    void usart_putudec(usart_dev *dev, uint32 val);

    /**
     * @brief Disable all serial ports.
     */
    static inline void usart_disable_all(void)
    {
        usart_disable(USART1);
        usart_disable(USART2);
        usart_disable(USART3);
    }

    /**
     * @brief Transmit one character on a serial port.
     *
     * This function blocks until the character has been queued
     * for transmission.
     *
     * @param dev Serial port to send on.
     * @param byte Byte to transmit.
     */
    static inline void usart_putc(usart_dev *dev, uint8 byte)
    {
        usart_tx(dev, &byte, 1);
    }

    /**
     * @brief Transmit a character string on a serial port.
     *
     * This function blocks until str is completely transmitted.
     *
     * @param dev Serial port to send on
     * @param str String to send
     */
    static inline void usart_putstr(usart_dev *dev, const char *str)
    {
        uint32 i = 0;
        while (str[i] != '\0')
        {
            usart_putc(dev, str[i++]);
        }
    }

    /**
     * @brief Read one character from a serial port.
     *
     * It's not safe to call this function if the serial port has no data
     * available.
     *
     * @param dev Serial port to read from
     * @return byte read
     * @see usart_data_available()
     */
    static inline uint8 usart_getc(usart_dev *dev)
    {
        return dev->rb->_pop();
    }

    /*
     * Roger Clark. 20141125,
     * added peek function.
     * @param dev Serial port to read from
     * @return byte read
     */
    static inline int usart_peek(usart_dev *dev)
    {
        return dev->rb->peek();
    }

    /**
     * @brief Return the amount of data available in a serial port's RX buffer.
     * @param dev Serial port to check
     * @return Number of bytes in dev's RX buffer.
     */
    static inline uint32 usart_data_available(usart_dev *dev)
    {
        return dev->rb->count();
    }

    /**
     * @brief Discard the contents of a serial port's RX buffer.
     * @param dev Serial port whose buffer to empty.
     */
    static inline void usart_reset_rx(usart_dev *dev)
    {
        dev->rb->clear();
    }

    /**
     * @brief Discard the contents of a serial port's RX buffer.
     * @param dev Serial port whose buffer to empty.
     */
    static inline void usart_reset_tx(usart_dev *dev)
    {
        dev->wb->clear();
    }

    /**
     * USART transmit hook
     *
     * @param ch the data byte to be transmitted
     * @param usart the usart channel. ([1,2,3,4]; 1 = DWIN, 2 = PRINT)
     */
    __weak extern void usart_tx_irq_hook(uint8_t ch, uint8_t usart);

    /**
     * USART receive hook
     *
     * @param ch the data byte that was received
     * @param usart the usart channel. ([1,2,3,4]; 1 = DWIN, 2 = PRINT)
     */
    __weak extern void usart_rx_irq_hook(uint8_t ch, uint8_t usart);

    /**
     * map usart device registers to usart channel number
     */
    static inline uint8_t usart_dev_to_channel(M4_USART_TypeDef *dev_regs)
    {
        if (dev_regs == M4_USART1)
            return 1;
        if (dev_regs == M4_USART2)
            return 2;
        if (dev_regs == M4_USART3)
            return 3;
        if (dev_regs == M4_USART4)
            return 4;

        return 0xff;
    }

    static inline void usart_tx_irq(usart_dev *dev)
    {
        uint8_t ch;
        if (dev->wb->pop(ch))
        {
            usart_tx_irq_hook(ch, usart_dev_to_channel(dev->regs));
            USART_SendData(dev->regs, ch);
        }
        else
        {
            USART_FuncCmd(dev->regs, UsartTxEmptyInt, Disable);
            USART_FuncCmd(dev->regs, UsartTxCmpltInt, Enable);
        }
    }

    static inline void usart_rx_irq(usart_dev *dev)
    {
        uint8_t ch = (uint8)USART_RecData(dev->regs);
        usart_rx_irq_hook(ch, usart_dev_to_channel(dev->regs));
        dev->rb->push(ch, true);
    }

#ifdef __cplusplus
} // extern "C"
#endif
