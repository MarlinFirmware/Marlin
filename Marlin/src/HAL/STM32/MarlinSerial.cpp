

#include "../../inc/MarlinConfig.h"
#include "MarlinSerial.h"

#if ENABLED(EMERGENCY_PARSER)
    #include "../../feature/e_parser.h"
#endif

#define DECLARE_SERIAL_PORT(ser_num) \
  void _rx_complete_irq_ ## ser_num (serial_t * obj); \
  MarlinSerial MSerial ## ser_num (USART ## ser_num, &_rx_complete_irq_ ## ser_num); \
  void _rx_complete_irq_ ## ser_num (serial_t * obj) { MSerial ## ser_num ._rx_complete_irq(obj); }

#define DECLARE_SERIAL_PORT_EXP(ser_num) DECLARE_SERIAL_PORT(ser_num)

#if (defined(SERIAL_PORT) && SERIAL_PORT >= 0)
  DECLARE_SERIAL_PORT_EXP(SERIAL_PORT)
#endif

#if (defined(SERIAL_PORT_2) && SERIAL_PORT_2 >= 0) 
  DECLARE_SERIAL_PORT_EXP(SERIAL_PORT_2)
#endif

#if (defined(DGUS_SERIAL_PORT) && DGUS_SERIAL_PORT >= 0) 
  DECLARE_SERIAL_PORT_EXP(DGUS_SERIAL_PORT)
#endif

void MarlinSerial::begin(unsigned long baud, uint8_t config) {
    HardwareSerial::begin(baud, config);
    // replace the IRQ callback with the one we have defined
    _serial.rx_callback = _rx_callback;
}

void MarlinSerial::_rx_complete_irq(serial_t *obj)
{
  // No Parity error, read byte and store it in the buffer if there is room
  unsigned char c;

  if (uart_getc(obj, &c) == 0) {

    rx_buffer_index_t i = (unsigned int)(obj->rx_head + 1) % SERIAL_RX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != obj->rx_tail) {
      obj->rx_buff[obj->rx_head] = c;
      obj->rx_head = i;
    }

    emergency_parser.update(emergency_state, c);
  }
}

