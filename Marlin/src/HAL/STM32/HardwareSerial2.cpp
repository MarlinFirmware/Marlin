// IRON, HARDWARESERIAL2 CLASS, ADAPTATION FROM ARDUINO'S HARDWARESERIAL
#include <stdio.h>
#include "Arduino.h"
#include "../../inc/MarlinConfig.h"
#include "HardwareSerial2.h"
#include "uart.h"

#if defined(HAL_UART_MODULE_ENABLED) && !defined(HAL_UART_MODULE_ONLY)

#define HAVE_HWSERIAL1          // SELECT THE SERIAL PORT HERE!!!

#define PIN_SERIAL1_TX          PA9
#define PIN_SERIAL1_RX          PA10
#define PIN_SERIAL2_TX          2   // PIN_A2
#define PIN_SERIAL2_RX          3   // PIN_A3
#define PIN_SERIAL3_TX          26  // PB10
#define PIN_SERIAL3_RX          27  // PB11
#define PIN_SERIAL4_TX          42  // PC10
#define PIN_SERIAL4_RX          43  // PC11
#define PIN_SERIAL5_TX          44  // PC12
#define PIN_SERIAL5_RX          50  // PD2

// GET FROM INCLUDE FILE!!!
#define RCC_AHB1Periph_DMA1              ((uint32_t)0x00200000)
#define RCC_AHB1Periph_DMA2              ((uint32_t)0x00400000)

void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState)
{
  // Check the parameters
  assert_param(IS_RCC_AHB1_CLOCK_PERIPH(RCC_AHB1Periph));

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->AHB1ENR |= RCC_AHB1Periph;
  }
  else
  {
    RCC->AHB1ENR &= ~RCC_AHB1Periph;
  }
}
// ------------------------------------------------------------------------

  // SerialEvent functions are weak, so when the user doesn't define them,
  // the linker just sets their address to 0 (which is checked below).
  #if defined(HAVE_HWSERIAL1)
    HardwareSerial2 HSerial1(USART1);
    void serialEvent1() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL2)
    HardwareSerial2 HSerial2(USART2);
    void serialEvent2() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL3)
    HardwareSerial2 Serial3(USART3);
    void serialEvent3() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL4)
    #if defined(USART4)
      HardwareSerial2 HSerial4(USART4);
    #else
      HardwareSerial2 HSerial4(UART4);
    #endif
    void serialEvent4() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIAL5)
    #if defined(USART5)
      HardwareSerial2 HSerial5(USART5);
    #else
      HardwareSerial2 HSerial5(UART5);
    #endif
    void serialEvent5() __attribute__((weak));
  #endif

  #if defined(HAVE_HWSERIALLP1)
    HardwareSerial2 SerialLP1(LPUART1);
    void serialEventLP1() __attribute__((weak));
  #endif

// Constructors ////////////////////////////////////////////////////////////////
HardwareSerial2::HardwareSerial2(void *peripheral)
{
  if (peripheral == USART1)
  {
    setRx(PIN_SERIAL1_RX);
    setTx(PIN_SERIAL1_TX);
    RX_DMA = { USART1, RCC_AHB1Periph_DMA2, 4, DMA2_Stream2 };
    _uart_index = 0;
  }
  else if (peripheral == USART2)
  {
    setRx(PIN_SERIAL2_RX);
    setTx(PIN_SERIAL2_TX);
    RX_DMA = { USART2, RCC_AHB1Periph_DMA1, 4, DMA1_Stream5 };
    _uart_index = 1;
  }
  else if (peripheral == USART3)
  {
    setRx(PIN_SERIAL3_RX);
    setTx(PIN_SERIAL3_TX);
    RX_DMA = { USART3, RCC_AHB1Periph_DMA1, 4, DMA1_Stream1 };
    _uart_index = 2;
  }
  else
      
#ifdef USART4  
  if (peripheral == USART4)
  {
    RX_DMA = { USART4, RCC_AHB1Periph_DMA1, 4, DMA1_Stream2 };
#endif

#ifdef UART4  
  if (peripheral == UART4)
  {
    RX_DMA = { UART4, RCC_AHB1Periph_DMA1, 4, DMA1_Stream2 };
#endif

  setRx(PIN_SERIAL4_RX);
  setTx(PIN_SERIAL4_TX);
   _uart_index = 3;   
 }
 else
 { // else get the pins of the first peripheral occurence in PinMap
   _serial.pin_rx = pinmap_pin(peripheral, PinMap_UART_RX);
   _serial.pin_tx = pinmap_pin(peripheral, PinMap_UART_TX);
 }

  init(_serial.pin_rx, _serial.pin_tx);
}

void HardwareSerial2::setRx(uint32_t _rx)
{
  _serial.pin_rx = digitalPinToPinName(_rx);
}

void HardwareSerial2::setTx(uint32_t _tx)
{
  _serial.pin_tx = digitalPinToPinName(_tx);
}

void HardwareSerial2::init(PinName _rx, PinName _tx)
{
  _serial.pin_rx  = _rx;
  _serial.rx_buff = _rx_buffer;
  _serial.rx_head = _serial.rx_tail = 0;

  _serial.pin_tx  = _tx;
  _serial.tx_buff = _tx_buffer;
  _serial.tx_head = _serial.tx_tail = 0;
}

/**
  * @brief  Read receive byte from uart
  * @param  obj : pointer to serial_t structure
  * @retval last character received
  */

// Actual interrupt handlers //////////////////////////////////////////////////////////////

int HardwareSerial2::_tx_complete_irq(serial_t *obj)
{
  // If interrupts are enabled, there must be more data in the output buffer. Send the next byte
  obj->tx_tail = (obj->tx_tail + 1) % TX_BUFFER_SIZE;

  if (obj->tx_head == obj->tx_tail) { return -1; }

  return 0;
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial2::begin(unsigned long baud, uint8_t config)
{
  uint32_t databits = 0;
  uint32_t stopbits = 0;
  uint32_t parity = 0;

  _baud = baud;
  _config = config;

  // Manage databits
  switch (config & 0x07)
  {
    case 0x02:
      databits = 6;
      break;
    case 0x04:
      databits = 7;
      break;
    case 0x06:
      databits = 8;
      break;
    default:
      databits = 0;
      break;
  }

  if ((config & 0x30) == 0x30)
  {
    parity = UART_PARITY_ODD;
    databits++;
  }
  else if ((config & 0x20) == 0x20)
  {
    parity = UART_PARITY_EVEN;
    databits++;
  }
  else
  {
    parity = UART_PARITY_NONE;
  }

  if ((config & 0x08) == 0x08)
  {
    stopbits = UART_STOPBITS_2;
  }
  else
  {
    stopbits = UART_STOPBITS_1;
  }

  switch (databits) {
#ifdef UART_WORDLENGTH_7B
    case 7:
      databits = UART_WORDLENGTH_7B;
      break;
#endif
    case 8:
      databits = UART_WORDLENGTH_8B;
      break;
    case 9:
      databits = UART_WORDLENGTH_9B;
      break;
    default:
    case 0:
      Error_Handler();
      break;
  }

  uart_init(&_serial, (uint32_t)baud, databits, parity, stopbits);

// IRON, USING FREE RUNNING DMA READING, NO CALLBACK NEEDED
// uart_attach_rx_callback(&_serial, _rx_complete_irq); // IRON, start the interrupt reading, indicate where to callback

  Serial_DMA_Read_Enable(); // IRON, START THE DMA READING PROCESS
}

void HardwareSerial2::end(void)
{
  flush();  // wait for transmission of outgoing data

  uart_deinit(&_serial);

  _serial.rx_head = _serial.rx_tail;   // clear any received data
}

void HardwareSerial2::update_rx_head(void)  // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS
{

#if ENABLED(EMERGENCY_PARSER)
  static uint32_t flag = 0;
  while (flag != _serial.rx_head) // send all available data to emergency parser immediately
  {
    emergency_parser.update(static_cast<MSerialT*>(this)->emergency_state, _serial.rx_buff[flag++]);
    flag = flag % RX_BUFFER_SIZE;   
  }
#endif

  _serial.rx_head = RX_BUFFER_SIZE - RX_DMA.dma_streamRX->NDTR; // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS
}

int HardwareSerial2::available(void)
{
  update_rx_head();  // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS
  return ((unsigned int)(RX_BUFFER_SIZE + _serial.rx_head - _serial.rx_tail)) % RX_BUFFER_SIZE;
}

int HardwareSerial2::peek(void)
{
  update_rx_head();  // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS
  if (_serial.rx_head == _serial.rx_tail)
  {
    return -1;
  }
  else
  {
    return _serial.rx_buff[_serial.rx_tail];
  }
}

int HardwareSerial2::read(void)
{
  update_rx_head();  // IRON, ADDED, UPDATE HEAD FROM DMA PROGRESS
  if (_serial.rx_head == _serial.rx_tail) // if the head isn't ahead of the tail, we don't have any characters
  {
    return -1;
  } 
  else
  {
    unsigned char c = _serial.rx_buff[_serial.rx_tail];
    _serial.rx_tail = (rx_buffer_index_t)(_serial.rx_tail + 1) % RX_BUFFER_SIZE;
    return c;
  }
}

size_t HardwareSerial2::write(uint8_t c) // Interrupt based writing
{
  tx_buffer_index_t i = (_serial.tx_head + 1) % TX_BUFFER_SIZE;

  // If the output buffer is full, there's nothing for it other than to
  // wait for the interrupt handler to empty it a bit
  while (i == _serial.tx_tail) {  } // nop, the interrupt handler will free up space for us

  _serial.tx_buff[_serial.tx_head] = c;
  _serial.tx_head = i;

  if (!serial_tx_active(&_serial))
  {
    uart_attach_tx_callback(&_serial, _tx_complete_irq); // write next byte, launch interrupt
  }

  return 1;
}

void HardwareSerial2::Serial_DMA_Read_Enable(void)
{
  RCC_AHB1PeriphClockCmd(RX_DMA.dma_rcc, ENABLE);                   // enable DMA clock

  RX_DMA.dma_streamRX->PAR = (uint32_t)(&RX_DMA.uart->DR);          // RX peripheral address (usart)
  RX_DMA.dma_streamRX->M0AR = (uint32_t)_serial.rx_buff;            // RX destination address (memory)
  RX_DMA.dma_streamRX->NDTR = RX_BUFFER_SIZE;                       // RX buffer size

  RX_DMA.dma_streamRX->CR = (RX_DMA.dma_channel << 25);               // RX channel selection, set to 0 all the other CR bits

  // primary serial port priority at highest level (TX higher than RX)
  RX_DMA.dma_streamRX->CR |= (3<<16);                               // RX priority level: Very High

//RX_DMA.dma_streamRX->CR &= ~(3<<13);                              // RX memory data size: 8 bit
//RX_DMA.dma_streamRX->CR &= ~(3<<11);                              // RX peripheral data size: 8 bit
  RX_DMA.dma_streamRX->CR |= (1<<10);                               // RX memory increment mode
//RX_DMA.dma_streamRX->CR &= ~(1<<9);                               // RX peripheral no increment mode
  RX_DMA.dma_streamRX->CR |= (1<<8);                                // RX circular mode enabled
//RX_DMA.dma_streamRX->CR &= ~(1<<6);                               // RX data transfer direction: Peripheral-to-memory
  RX_DMA.uart->CR3 |= (1<<6);                                       // enable DMA receiver (DMAR)
  RX_DMA.dma_streamRX->CR |= (1<<0);                                // RX enable DMA
}

#endif // HAL_UART_MODULE_ENABLED && !HAL_UART_MODULE_ONLY
