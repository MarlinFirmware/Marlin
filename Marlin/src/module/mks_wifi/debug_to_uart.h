#ifndef DEBUG_UART_H
#define DEBUG_UART_H

#include "../../MarlinCore.h"
#include "../../inc/MarlinConfig.h"

#ifdef DEBUG_TO_UART

void debug_to_uart(char *fmt,...);
inline void debug_to_uart_flush() { MYSERIAL2.flush(); }


#endif

#endif