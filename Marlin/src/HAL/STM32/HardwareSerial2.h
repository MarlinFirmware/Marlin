#ifndef HardwareSerial2_h
#define HardwareSerial2_h

typedef struct
{
  USART_TypeDef * uart;
  uint32_t dma_rcc;
  uint32_t dma_channel;
  DMA_Stream_TypeDef * dma_streamRX;
} DMA_CFG;

class HardwareSerial2 : public Stream {
  protected:
    // Don't put any members after these buffers, since only the first
    // 32 bytes of this struct can be accessed quickly using the ldd instruction.
    unsigned char _rx_buffer[RX_BUFFER_SIZE];
    unsigned char _tx_buffer[TX_BUFFER_SIZE];

    serial_t _serial;

  public:
    HardwareSerial2(void *peripheral);

    void begin(unsigned long, uint8_t);
    void end();
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual size_t write(uint8_t);

    operator bool() { return true; }

    void setRx(uint32_t _rx);
    void setTx(uint32_t _tx);

    static int _tx_complete_irq(serial_t *obj); // Interrupt handler

  private:
    uint8_t _uart_index;
    bool    _rx_enabled;
    uint8_t _config;
    unsigned long _baud;
    void init(PinName _rx, PinName _tx);
    void update_rx_head(void);
    DMA_CFG RX_DMA;
    void Serial_DMA_Read_Enable(void);
};

#endif
