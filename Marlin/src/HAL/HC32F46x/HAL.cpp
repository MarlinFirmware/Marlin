#include "HAL.h"

//
// Emergency Parser
//

#if ENABLED(EMERGENCY_PARSER)
void usart_rx_irq_hook(uint8_t ch, uint8_t usart)
{
    // only handle receive on USART channel 2 (PRINT/HOST serial)
    if (usart != 2)
        return;

    // submit character to emergency parser
    if (MYSERIAL1.emergency_parser_enabled())
        emergency_parser.update(MYSERIAL1.emergency_state, ch);
}
#endif
