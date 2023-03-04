#include "HAL.h"

//
// Emergency Parser
//
#if ENABLED(EMERGENCY_PARSER)
extern "C" void usart_rx_irq_hook(uint8_t ch, uint8_t usart)
{
    // only handle receive on host serial ports
    if (false
#ifdef SERIAL_PORT
        || usart != SERIAL_PORT
#endif
#ifdef SERIAL_PORT_2
        || usart != SERIAL_PORT_2
#endif
#ifdef SERIAL_PORT_3
        || usart != SERIAL_PORT_3
#endif
    )
    {
        return;
    }

    // submit character to emergency parser
    if (MYSERIAL1.emergency_parser_enabled())
        emergency_parser.update(MYSERIAL1.emergency_state, ch);
}
#endif
