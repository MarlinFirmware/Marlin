
#pragma once

#include "../../inc/MarlinConfigPre.h"
#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

typedef void (*usart_rx_callback_t)(serial_t * obj);

class MarlinSerial : public HardwareSerial {
public:
    MarlinSerial(void* peripheral, usart_rx_callback_t rx_callback) :
        HardwareSerial(peripheral), _rx_callback(rx_callback)
        #if ENABLED(EMERGENCY_PARSER)
          , emergency_state(EmergencyParser::State::EP_RESET)
        #endif
    { }

    void begin(unsigned long baud) {
        begin(baud, SERIAL_8N1);
    }
    void begin(unsigned long baud, uint8_t config);

    void _rx_complete_irq(serial_t* obj);

protected:
    usart_rx_callback_t _rx_callback;
#if ENABLED(EMERGENCY_PARSER)
    EmergencyParser::State emergency_state;
#endif

};

extern MarlinSerial MSerial1;
extern MarlinSerial MSerial2;
extern MarlinSerial MSerial3;
extern MarlinSerial MSerial4;
extern MarlinSerial MSerial5;
