

#include "HAL_uart.h"


#if USE_UART == 1
    MarlinUART marlinUART(&Serial1);
#endif


#if USE_UART == 2
    MarlinUART marlinUART(&Serial2);
#endif

#if USE_UART == 3
    MarlinUART marlinUART(&Serial3);
#endif

