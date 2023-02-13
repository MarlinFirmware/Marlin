#ifndef _LIBMAPLE_USART_H_
#define _LIBMAPLE_USART_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../board/startup.h"
#include "libmaple_types.h"
#include "ring_buffer.h"

#define USART_RX_BUF_SIZE 64
#define USART_TX_BUF_SIZE 64

#define PRINT_UART M4_USART2
#define SCREEN_UART M4_USART1

#define PRINT_CLK PWC_FCG1_PERIPH_USART2
#define SCREEN_CLK PWC_FCG1_PERIPH_USART1

#define PRINT_RX_IRQ INT_USART2_RI
#define PRINT_RXERR_IRQ INT_USART2_EI
#define PRINT_TX_IRQ INT_USART2_TI
#define PRINT_TCI_IRQ INT_USART2_TCI

#define SCREEN_RX_IRQ INT_USART1_RI
#define SCREEN_RXERR_IRQ INT_USART1_EI
#define SCREEN_TX_IRQ INT_USART1_TI
#define SCREEN_TCI_IRQ INT_USART1_TCI

    typedef struct usart_dev
    {
        M4_USART_TypeDef *regs;          /**< Register map */
        ring_buffer *rb;                 /**< RX ring buffer */
        ring_buffer *wb;                 /**< TX ring buffer */
        uint32 max_baud;                 /**< @brief Deprecated.
                                          * Maximum baud rate. */
        uint8 rx_buf[USART_RX_BUF_SIZE]; /**< @brief Deprecated.
                                          * Actual RX buffer used by rb.
                                          * This field will be removed in
                                          * a future release. */
        uint8 tx_buf[USART_TX_BUF_SIZE]; /**< Actual TX buffer used by wb */
        uint32 clk_id;
        stc_usart_uart_init_t *pstcInitCfg;
        IRQn_Type RX_IRQ;
        IRQn_Type TX_IRQ;
        IRQn_Type RX_error_IRQ;
        IRQn_Type TX_complete_IRQ;
        uint32_t IRQ_priority;
    } usart_dev;

    struct usart_dev;
    extern usart_dev usart4;
    extern usart_dev usart3;
    extern usart_dev usart2;
    extern struct usart_dev *USART2;
    extern struct usart_dev *USART3;
    extern struct usart_dev *USART4;

    extern ring_buffer usart2_rb;
    extern ring_buffer usart2_wb;
    extern ring_buffer usart3_rb;
    extern ring_buffer usart3_wb;
    extern ring_buffer usart4_rb;
    extern ring_buffer usart4_wb;

    void usart_init(usart_dev *dev);
    void usart_set_baud_rate(usart_dev *dev, uint32 baud);
    void usart_enable(usart_dev *dev);
    void usart_disable(usart_dev *dev);
    void usart_foreach(void (*fn)(usart_dev *dev));
    uint32 usart_tx(usart_dev *dev, const uint8 *buf, uint32 len);
    uint32 usart_rx(usart_dev *dev, uint8 *buf, uint32 len);
    void usart_putudec(usart_dev *dev, uint32 val);

    /**
     * @brief Disable all serial ports.
     */
    static inline void usart_disable_all(void)
    {
        usart_foreach(usart_disable);
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
        return rb_remove(dev->rb);
    }

    /*
     * Roger Clark. 20141125,
     * added peek function.
     * @param dev Serial port to read from
     * @return byte read
     */
    static inline int usart_peek(usart_dev *dev)
    {
        return rb_peek(dev->rb);
    }

    /**
     * @brief Return the amount of data available in a serial port's RX buffer.
     * @param dev Serial port to check
     * @return Number of bytes in dev's RX buffer.
     */
    static inline uint32 usart_data_available(usart_dev *dev)
    {
        return rb_full_count(dev->rb);
    }

    /**
     * @brief Discard the contents of a serial port's RX buffer.
     * @param dev Serial port whose buffer to empty.
     */
    static inline void usart_reset_rx(usart_dev *dev)
    {
        rb_reset(dev->rb);
    }

    /**
     * @brief Discard the contents of a serial port's RX buffer.
     * @param dev Serial port whose buffer to empty.
     */
    static inline void usart_reset_tx(usart_dev *dev)
    {
        rb_reset(dev->wb);
    }

    static inline void usart_tx_irq(ring_buffer *wb, M4_USART_TypeDef *regs)
    {
        if (!rb_is_empty(wb))
        {
            USART_SendData(regs, rb_remove(wb));
        }
        else
        {
            USART_FuncCmd(regs, UsartTxEmptyInt, Disable);
            USART_FuncCmd(regs, UsartTxCmpltInt, Enable);
        }
    }

    static inline void usart_rx_irq(ring_buffer *rb, M4_USART_TypeDef *regs)
    {
        rb_push_insert(rb, (uint8)USART_RecData(regs));
    }

#ifdef __cplusplus
} // extern "C"
#endif

#endif
