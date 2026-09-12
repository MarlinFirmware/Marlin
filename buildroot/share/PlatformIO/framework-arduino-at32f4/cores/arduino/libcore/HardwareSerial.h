#ifndef HARDWARE_SERIAL_DMA_H
#define HARDWARE_SERIAL_DMA_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "at32f403a_407.h"

typedef enum
{
    SERIAL_8N1,
    SERIAL_8N2,
    SERIAL_8E1,
    SERIAL_8E2,
    SERIAL_8O1,
    SERIAL_8O2,
    SERIAL_8N0_5,
    SERIAL_8N1_5,
    SERIAL_8E0_5,
    SERIAL_8E1_5,
    SERIAL_8O0_5,
    SERIAL_8O1_5,

    SERIAL_9N1,
    SERIAL_9N2,
    SERIAL_9E1,
    SERIAL_9E2,
    SERIAL_9O1,
    SERIAL_9O2,
    SERIAL_9N0_5,
    SERIAL_9N1_5,
    SERIAL_9E0_5,
    SERIAL_9E1_5,
    SERIAL_9O0_5,
    SERIAL_9O1_5,
} SERIAL_Config_t;


class HardwareSerial
{
public:
  HardwareSerial(
    usart_type* usart,
    uint8_t* buffer,
    uint16_t size
  );

  void begin(uint32_t baud, SERIAL_Config_t config = SERIAL_8N1);
  int available();
  int read();
  void write(uint8_t c);
  void write(const uint8_t *buf, uint16_t len);
  void print(const char *s);
  void println(const char *s);
  void onTxComplete(void);
  
  

private:
    usart_type*        _usart;
    dma_channel_type *_dma_rx;
    dma_channel_type *_dma_tx;
    uint8_t*           _buf;
    uint16_t           _size;
    uint16_t           _read;
    volatile bool _tx_busy;

  void selectDmaChannel(void);
  void initGpio(void);
  void initClock(void);
};

extern HardwareSerial Serial1;   // global ya da extern olarak tanımlı olmalı
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;
extern HardwareSerial Serial4;
extern HardwareSerial Serial5;
extern HardwareSerial Serial7;
extern HardwareSerial& Serial;   // primary UART (aliased to Serial1 in HardwareSerial.cpp)
#endif
