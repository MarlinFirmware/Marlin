#include "HAL.h"
#include <core_hooks.h>
#include <drivers/panic/panic.h>

//
// Emergency Parser
//
#if ENABLED(EMERGENCY_PARSER)
extern "C" void core_hook_usart_rx_irq(uint8_t ch, uint8_t usart)
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

//
// panic hook
//
extern "C" void core_hook_panic_end()
{
    // print '!!' to signal error to host
    // do it 10x so it's not missed
    for (int i = 0; i < 10; i++)
    {
        panic_printf("\n!!\n");
    }
}
