
#include "../board/startup.h"
#include "usart.h"

/******************************************************/
/******************************************************/
void Usart4RxIrqCallback(void)
{
	usart_rx_irq(&usart4_rb, SCREEN_UART);
}
void Usart4ErrIrqCallback(void)
{
	if (Set == USART_GetStatus(SCREEN_UART, UsartFrameErr))
	{
		USART_ClearStatus(SCREEN_UART, UsartFrameErr);
	}
	if (Set == USART_GetStatus(SCREEN_UART, UsartParityErr))
	{
		USART_ClearStatus(SCREEN_UART, UsartParityErr);
	}
	if (Set == USART_GetStatus(SCREEN_UART, UsartOverrunErr))
	{
		USART_ClearStatus(SCREEN_UART, UsartOverrunErr);
	}
}

void Usart4TxIrqCallback(void)
{
	usart_tx_irq(&usart4_wb, SCREEN_UART);
}

void Usart4TxCmpltIrqCallback(void)
{
	USART_FuncCmd(SCREEN_UART, UsartTxCmpltInt, Disable);
	USART_FuncCmd(SCREEN_UART, UsartTx, Disable);
}
/******************************************************/
/******************************************************/
void Usart2RxIrqCallback(void)
{
	usart_rx_irq(&usart2_rb, PRINT_UART);
}
void Usart2ErrIrqCallback(void)
{
	if (Set == USART_GetStatus(PRINT_UART, UsartFrameErr))
	{
		USART_ClearStatus(PRINT_UART, UsartFrameErr);
	}
	else
	{
	}

	if (Set == USART_GetStatus(PRINT_UART, UsartParityErr))
	{
		USART_ClearStatus(PRINT_UART, UsartParityErr);
	}
	else
	{
	}

	if (Set == USART_GetStatus(PRINT_UART, UsartOverrunErr))
	{
		USART_ClearStatus(PRINT_UART, UsartOverrunErr);
	}
	else
	{
	}
}

void Usart2TxIrqCallback(void)
{
	usart_tx_irq(&usart2_wb, PRINT_UART);
}

void Usart2TxCmpltIrqCallback(void)
{
	USART_FuncCmd(PRINT_UART, UsartTxCmpltInt, Disable);
	USART_FuncCmd(PRINT_UART, UsartTx, Disable);
}
/******************************************************/
/******************************************************/
void Usart3RxIrqCallback(void)
{
	usart_rx_irq(&usart3_rb, M4_USART3);
}
void Usart3ErrIrqCallback(void)
{
	if (Set == USART_GetStatus(M4_USART3, UsartFrameErr))
	{
		USART_ClearStatus(M4_USART3, UsartFrameErr);
	}
	else
	{
	}

	if (Set == USART_GetStatus(M4_USART3, UsartParityErr))
	{
		USART_ClearStatus(M4_USART3, UsartParityErr);
	}
	else
	{
	}

	if (Set == USART_GetStatus(M4_USART3, UsartOverrunErr))
	{
		USART_ClearStatus(M4_USART3, UsartOverrunErr);
	}
	else
	{
	}
}

void Usart3TxIrqCallback(void)
{
	usart_tx_irq(&usart3_wb, M4_USART3);
}

void Usart3TxCmpltIrqCallback(void)
{
	USART_FuncCmd(M4_USART3, UsartTxCmpltInt, Disable);
	USART_FuncCmd(M4_USART3, UsartTx, Disable);
}
/******************************************************/
/******************************************************/

void __O0 usart_init(usart_dev *dev)
{
	rb_init(dev->rb, USART_RX_BUF_SIZE, dev->rx_buf);
	rb_init(dev->wb, USART_TX_BUF_SIZE, dev->tx_buf);

	/* Enable peripheral clock */
	PWC_Fcg1PeriphClockCmd(dev->clk_id, Enable);
	/* Initialize UART */
	USART_UART_Init(dev->regs, dev->pstcInitCfg);
}

void usart_set_baud_rate(usart_dev *dev, uint32 baud)
{
	/* Set baudrate */
	USART_SetBaudrate(dev->regs, baud);
}

void __O0 usart_enable(usart_dev *dev)
{
	stc_irq_regi_conf_t stcIrqRegiCfg;

	/* Set USART RX IRQ */
	stcIrqRegiCfg.enIRQn = dev->RX_IRQ;
	if (dev->regs == SCREEN_UART)
	{
		stcIrqRegiCfg.enIntSrc = SCREEN_RX_IRQ;
		stcIrqRegiCfg.pfnCallback = &Usart4RxIrqCallback;
	}
	else if (dev->regs == PRINT_UART)
	{
		stcIrqRegiCfg.enIntSrc = PRINT_RX_IRQ;
		stcIrqRegiCfg.pfnCallback = &Usart2RxIrqCallback;
	}
	else if (dev->regs == M4_USART3)
	{
		stcIrqRegiCfg.enIntSrc = INT_USART3_RI;
		stcIrqRegiCfg.pfnCallback = &Usart3RxIrqCallback;
	}
	enIrqRegistration(&stcIrqRegiCfg);
	NVIC_SetPriority(stcIrqRegiCfg.enIRQn, dev->IRQ_priority);
	NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
	NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

	/* Set USART RX error IRQ */
	stcIrqRegiCfg.enIRQn = dev->RX_error_IRQ;
	if (dev->regs == SCREEN_UART)
	{
		stcIrqRegiCfg.enIntSrc = SCREEN_RXERR_IRQ;
		stcIrqRegiCfg.pfnCallback = &Usart4ErrIrqCallback;
	}
	else if (dev->regs == PRINT_UART)
	{
		stcIrqRegiCfg.enIntSrc = PRINT_RXERR_IRQ;
		stcIrqRegiCfg.pfnCallback = &Usart2ErrIrqCallback;
	}
	else if (dev->regs == M4_USART3)
	{
		stcIrqRegiCfg.enIntSrc = INT_USART3_EI;
		stcIrqRegiCfg.pfnCallback = &Usart3ErrIrqCallback;
	}
	enIrqRegistration(&stcIrqRegiCfg);
	NVIC_SetPriority(stcIrqRegiCfg.enIRQn, dev->IRQ_priority);
	NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
	NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

	/* Set USART TX IRQ */
	stcIrqRegiCfg.enIRQn = dev->TX_IRQ;
	if (dev->regs == SCREEN_UART)
	{
		stcIrqRegiCfg.enIntSrc = SCREEN_TX_IRQ;
		stcIrqRegiCfg.pfnCallback = &Usart4TxIrqCallback;
	}
	else if (dev->regs == PRINT_UART)
	{
		stcIrqRegiCfg.enIntSrc = PRINT_TX_IRQ;
		stcIrqRegiCfg.pfnCallback = &Usart2TxIrqCallback;
	}
	else if (dev->regs == M4_USART3)
	{
		stcIrqRegiCfg.enIntSrc = INT_USART3_TI;
		stcIrqRegiCfg.pfnCallback = &Usart3TxIrqCallback;
	}
	enIrqRegistration(&stcIrqRegiCfg);
	NVIC_SetPriority(stcIrqRegiCfg.enIRQn, dev->IRQ_priority);
	NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
	NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

	/* Set USART TX complete IRQ */

	stcIrqRegiCfg.enIRQn = dev->TX_complete_IRQ;
	if (dev->regs == SCREEN_UART)
	{
		stcIrqRegiCfg.enIntSrc = SCREEN_TCI_IRQ;
		stcIrqRegiCfg.pfnCallback = &Usart4TxCmpltIrqCallback;
	}
	else if (dev->regs == PRINT_UART)
	{
		stcIrqRegiCfg.enIntSrc = PRINT_TCI_IRQ;
		stcIrqRegiCfg.pfnCallback = &Usart2TxCmpltIrqCallback;
	}
	else if (dev->regs == M4_USART3)
	{
		stcIrqRegiCfg.enIntSrc = INT_USART3_TCI;
		stcIrqRegiCfg.pfnCallback = &Usart3TxCmpltIrqCallback;
	}
	enIrqRegistration(&stcIrqRegiCfg);
	NVIC_SetPriority(stcIrqRegiCfg.enIRQn, dev->IRQ_priority);
	NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
	NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

	/*Enable RX && RX interupt function*/
	USART_FuncCmd(dev->regs, UsartRx, Enable);
	USART_FuncCmd(dev->regs, UsartRxInt, Enable);
}

void usart_disable(usart_dev *dev)
{
	while (!rb_is_empty(dev->wb))
		;
	USART_DeInit(dev->regs);
	/* Clean up buffer */
	usart_reset_rx(dev);
	usart_reset_tx(dev);
}

uint32 __O0 usart_tx(usart_dev *dev, const uint8 *buf, uint32 len)
{
	uint32 txed = 0;
	uint32 errcnt = 0;
	while (!rb_is_empty(dev->wb))
	{
		if (++errcnt > 500)
		{
			errcnt = 0;
			return txed;
		}
	}
	while (txed < len)
	{
		if (++errcnt > 500)
		{
			errcnt = 0;
			break;
		}
		if (rb_safe_insert(dev->wb, buf[txed]))
		{
			txed++;
		}
		else
			break;
	}
	if (!rb_is_empty(dev->wb))
	{
		USART_FuncCmd(dev->regs, UsartTxAndTxEmptyInt, Enable);
	}
	return txed;
}

uint32 __O0 usart_rx(usart_dev *dev, uint8 *buf, uint32 len)
{
	uint32 rxed = 0;
	uint32 errcnt = 0;
	while (usart_data_available(dev) && rxed < len)
	{
		*buf++ = usart_getc(dev);
		rxed++;
		if (++errcnt > 500)
		{
			errcnt = 0;
			break;
		}
	}
	return rxed;
}

void usart_putudec(usart_dev *dev, uint32 val)
{
	char digits[12];
	int i = 0;

	do
	{
		digits[i++] = val % 10 + '0';
		val /= 10;
	} while (val > 0);

	while (--i >= 0)
	{
		usart_putc(dev, digits[i]);
	}
}
