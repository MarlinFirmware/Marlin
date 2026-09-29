#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "HardwareSerial.h"

/*
USART1 (Default)
PA9: USART1_TX
PA10: USART1_RX

USART2 (Default)
PA2: USART2_TX
PA3: USART2_RX

USART3 (Default)
PB10: USART3_TX
PB11: USART3_RX

UART4 (Remap)
PA0: UART4_TX
PA1: UART4_RX

UART5 (Remap)
PB8 UART5_RX  DMA1_CHANNEL5
PB9 UART5_TX  DMA1_CHANNEL7

UART7 (Remap)
PB3  UART7_RX  DMA1_CHANNEL7
PB4  UART7_TX  DMA1_CHANNEL6

*/

// Buffer'lar (her biri için ayrı bellek ayrılacak)
static uint8_t rx_buffer1[512];
static uint8_t rx_buffer2[512];
static uint8_t rx_buffer3[512];
static uint8_t rx_buffer4[512];
static uint8_t rx_buffer5[512];
static uint8_t rx_buffer6[512];

// Global nesnelerin TANIMLAMASI (definition) → constructor burada çağrılır
HardwareSerial Serial1(USART1, rx_buffer1, 512);
HardwareSerial Serial2(USART2, rx_buffer2, 512);
HardwareSerial Serial3(USART3, rx_buffer3, 512);
HardwareSerial Serial4(UART4,  rx_buffer4, 512);
HardwareSerial Serial5(UART5,  rx_buffer5, 512);
HardwareSerial Serial7(UART7,  rx_buffer6, 512);

// Marlin and some framework sources (DShot, GPS) reference the bare `Serial`
// global. Alias it to the primary UART (Serial1) rather than instantiate a
// second HardwareSerial on the same peripheral.
HardwareSerial& Serial = Serial1;

typedef struct
{
    usart_data_bit_num_type data_bit;
    usart_parity_selection_type parity_selection;
    usart_stop_bit_num_type stop_bit;
} SERIAL_ConfigGrp_t;

static const SERIAL_ConfigGrp_t SERIAL_ConfigGrp[] =
{
    {USART_DATA_8BITS, USART_PARITY_NONE, USART_STOP_1_BIT},   // SERIAL_8N1
    {USART_DATA_8BITS, USART_PARITY_NONE, USART_STOP_2_BIT},   // SERIAL_8N2
    {USART_DATA_8BITS, USART_PARITY_EVEN, USART_STOP_1_BIT},   // SERIAL_8E1
    {USART_DATA_8BITS, USART_PARITY_EVEN, USART_STOP_2_BIT},   // SERIAL_8E2
    {USART_DATA_8BITS, USART_PARITY_ODD,  USART_STOP_1_BIT},   // SERIAL_8O1
    {USART_DATA_8BITS, USART_PARITY_ODD,  USART_STOP_2_BIT},   // SERIAL_8O2
    {USART_DATA_8BITS, USART_PARITY_NONE, USART_STOP_0_5_BIT}, // SERIAL_8N0_5
    {USART_DATA_8BITS, USART_PARITY_NONE, USART_STOP_1_5_BIT}, // SERIAL_8N1_5
    {USART_DATA_8BITS, USART_PARITY_EVEN, USART_STOP_0_5_BIT}, // SERIAL_8E0_5
    {USART_DATA_8BITS, USART_PARITY_EVEN, USART_STOP_1_5_BIT}, // SERIAL_8E1_5
    {USART_DATA_8BITS, USART_PARITY_ODD,  USART_STOP_0_5_BIT}, // SERIAL_8O0_5
    {USART_DATA_8BITS, USART_PARITY_ODD,  USART_STOP_1_5_BIT}, // SERIAL_8O1_5

    {USART_DATA_9BITS, USART_PARITY_NONE, USART_STOP_1_BIT},   // SERIAL_9N1
    {USART_DATA_9BITS, USART_PARITY_NONE, USART_STOP_2_BIT},   // SERIAL_9N2
    {USART_DATA_9BITS, USART_PARITY_EVEN, USART_STOP_1_BIT},   // SERIAL_9E1
    {USART_DATA_9BITS, USART_PARITY_EVEN, USART_STOP_2_BIT},   // SERIAL_9E2
    {USART_DATA_9BITS, USART_PARITY_ODD,  USART_STOP_1_BIT},   // SERIAL_9O1
    {USART_DATA_9BITS, USART_PARITY_ODD,  USART_STOP_2_BIT},   // SERIAL_9O2
    {USART_DATA_9BITS, USART_PARITY_NONE, USART_STOP_0_5_BIT}, // SERIAL_9N0_5
    {USART_DATA_9BITS, USART_PARITY_NONE, USART_STOP_1_5_BIT}, // SERIAL_9N1_5
    {USART_DATA_9BITS, USART_PARITY_EVEN, USART_STOP_0_5_BIT}, // SERIAL_9E0_5
    {USART_DATA_9BITS, USART_PARITY_EVEN, USART_STOP_1_5_BIT}, // SERIAL_9E1_5
    {USART_DATA_9BITS, USART_PARITY_ODD,  USART_STOP_0_5_BIT}, // SERIAL_9O0_5
    {USART_DATA_9BITS, USART_PARITY_ODD,  USART_STOP_1_5_BIT}, // SERIAL_9O1_5
};

HardwareSerial::HardwareSerial(usart_type* usart,
                                     uint8_t* buf,
                                     uint16_t size)
    : _usart(usart),
      _dma_rx(nullptr),
      _dma_tx(nullptr),
      _buf(buf),
      _size(size),
      _read(0)
{
  selectDmaChannel();
}

/* ---------------- DMA AUTO SELECT ---------------- */
void HardwareSerial::selectDmaChannel(void)
{
  if (_usart == USART1) {
    _dma_rx = DMA1_CHANNEL5;
    _dma_tx = DMA1_CHANNEL4;
  }
  else if (_usart == USART2) {
    _dma_rx = DMA1_CHANNEL6;
    _dma_tx = DMA1_CHANNEL7;
  }
  else if (_usart == USART3) {
    _dma_rx = DMA1_CHANNEL3;
    _dma_tx = DMA1_CHANNEL2;
  }
  else if (_usart == UART4) {
    _dma_rx = DMA2_CHANNEL3;
    _dma_tx = DMA2_CHANNEL5;
  }
  // === GÜNCELLEME: UART5 ===
  else if (_usart == UART5) {
    _dma_rx = DMA2_CHANNEL1;
    _dma_tx = DMA2_CHANNEL2;
  }
  // === GÜNCELLEME: UART7 ===
  else if (_usart == UART7) {
    _dma_rx = DMA2_CHANNEL4;
    _dma_tx = DMA2_CHANNEL5;
  }
}


/* ---------------- GPIO INIT ---------------- */
void HardwareSerial::initGpio(void)
{
  gpio_init_type gpio;
  gpio_default_para_init(&gpio);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  /* ---------- TX ---------- */
  gpio.gpio_mode = GPIO_MODE_MUX;
  gpio.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  if (_usart == USART1) { gpio.gpio_pins = GPIO_PINS_9; gpio_init(GPIOA, &gpio); }
  if (_usart == USART2) { gpio.gpio_pins = GPIO_PINS_2; gpio_init(GPIOA, &gpio); }
  if (_usart == USART3) { gpio.gpio_pins = GPIO_PINS_10; gpio_init(GPIOB, &gpio); }
  if (_usart == UART4) { gpio.gpio_pins = GPIO_PINS_0; gpio_init(GPIOA, &gpio); }
  if (_usart == UART5) { gpio.gpio_pins = GPIO_PINS_9; gpio_init(GPIOB, &gpio); }
  if (_usart == UART7) { gpio.gpio_pins = GPIO_PINS_4; gpio_init(GPIOB, &gpio); }
  /* ---------- RX ---------- */
  gpio.gpio_mode = GPIO_MODE_INPUT;
  if (_usart == USART1) { gpio.gpio_pins = GPIO_PINS_10; gpio_init(GPIOA, &gpio); }
  if (_usart == USART2) { gpio.gpio_pins = GPIO_PINS_3; gpio_init(GPIOA, &gpio); }
  if (_usart == USART3) { gpio.gpio_pins = GPIO_PINS_11; gpio_init(GPIOB, &gpio); }
  if (_usart == UART4) { gpio.gpio_pins = GPIO_PINS_1; gpio_init(GPIOA, &gpio); }
  if (_usart == UART5) { gpio.gpio_pins = GPIO_PINS_8; gpio_init(GPIOB, &gpio); }
  if (_usart == UART7) { gpio.gpio_pins = GPIO_PINS_3; gpio_init(GPIOB, &gpio); }
}

/* ---------------- CLOCK INIT ---------------- */
void HardwareSerial::initClock(void)
{
  if (_usart == USART1)
    crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
  else if (_usart == USART2)
    crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);
  else if (_usart == USART3)
    crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, TRUE);
  else if (_usart == UART4) {
    crm_periph_clock_enable(CRM_UART4_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    gpio_pin_remap_config(UART4_GMUX_0010, TRUE);
  }
  else if (_usart == UART5) {
    crm_periph_clock_enable(CRM_UART5_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    gpio_pin_remap_config(UART5_GMUX_0001, TRUE);
  }
  else if (_usart == UART7) {
    crm_periph_clock_enable(CRM_UART7_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
    gpio_pin_remap_config(UART7_GMUX, TRUE);
  }

  // === GÜNCELLEME: DMA Saatlerini Garantiye Alın ===
  // Eğer UART4, UART5 veya UART7 ise mutlaka DMA2 saatini de açın
  if (_usart == UART4 || _usart == UART5 || _usart == UART7 || _dma_rx >= DMA2_CHANNEL3) {
    crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);
  }
  
  // Her halükarda DMA1 saatini açık tutalım veya koşula bağlayalım
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
}


void HardwareSerial::begin(uint32_t baud, SERIAL_Config_t config)
{
    initClock();
    initGpio();

    usart_init(_usart, baud, SERIAL_ConfigGrp[config].data_bit, SERIAL_ConfigGrp[config].stop_bit);
    usart_parity_selection_config(_usart, SERIAL_ConfigGrp[config].parity_selection);
    usart_transmitter_enable(_usart, TRUE);
    usart_receiver_enable(_usart, TRUE);
    usart_dma_receiver_enable(_usart, TRUE);
    usart_dma_transmitter_enable(_usart, TRUE);
    usart_enable(_usart, TRUE);

    // ==================== TX DMA ====================
    dma_init_type dma;
    dma_default_para_init(&dma);
    dma.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
    dma.memory_inc_enable = TRUE;
    dma.loop_mode_enable = FALSE;
    dma.peripheral_inc_enable = FALSE;
    dma.priority = DMA_PRIORITY_HIGH;
    dma.peripheral_base_addr = (uint32_t)&_usart->dt;
    dma_init(_dma_tx, &dma);

    dma_interrupt_enable(_dma_tx, DMA_FDT_INT, TRUE);

    // → NVIC KESME ETKİNLEŞTİRME (GÜNCELLENDİ)
    if (_usart == USART1)
        nvic_irq_enable(DMA1_Channel4_IRQn, 0, 0);
    else if (_usart == USART2)
        nvic_irq_enable(DMA1_Channel7_IRQn, 0, 0);
    else if (_usart == USART3)
        nvic_irq_enable(DMA1_Channel2_IRQn, 0, 0);
    else if (_usart == UART4)
        nvic_irq_enable(DMA2_Channel4_5_IRQn, 0, 0);
  
    // UART5 TX -> DMA2_CHANNEL2 kullandığımız için:
    else if (_usart == UART5)
        nvic_irq_enable(DMA2_Channel2_IRQn, 0, 0); 
  
    // UART7 TX -> DMA2_CHANNEL5 kullandığımız için (UART4 ile ortak IRQ):
    else if (_usart == UART7)
        nvic_irq_enable(DMA2_Channel4_5_IRQn, 0, 0);
  
    _tx_busy = false;

    // ==================== RX DMA (Circular) ====================
    dma_default_para_init(&dma);
    dma.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma.buffer_size = _size;
    dma.memory_base_addr = (uint32_t)_buf;
    dma.memory_inc_enable = TRUE;
    dma.peripheral_base_addr = (uint32_t)&_usart->dt;
    dma.loop_mode_enable = TRUE;          
    dma.priority = DMA_PRIORITY_HIGH;
    dma_init(_dma_rx, &dma);

    // === GÜNCELLEME: Flexible DMA Mapping dizilimi ===
    if (_usart == USART1) {
        dma_flexible_config(DMA1, FLEX_CHANNEL5, DMA_FLEXIBLE_UART1_RX);
        dma_flexible_config(DMA1, FLEX_CHANNEL4, DMA_FLEXIBLE_UART1_TX);
    }
    else if (_usart == USART2) {
        dma_flexible_config(DMA1, FLEX_CHANNEL6, DMA_FLEXIBLE_UART2_RX);
        dma_flexible_config(DMA1, FLEX_CHANNEL7, DMA_FLEXIBLE_UART2_TX);
    }
    else if (_usart == USART3) {
        dma_flexible_config(DMA1, FLEX_CHANNEL3, DMA_FLEXIBLE_UART3_RX);
        dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_UART3_TX);
    }
    else if (_usart == UART4) {
        dma_flexible_config(DMA2, FLEX_CHANNEL3, DMA_FLEXIBLE_UART4_RX);
        dma_flexible_config(DMA2, FLEX_CHANNEL5, DMA_FLEXIBLE_UART4_TX);
    }
    // UART5 için esnek kanal atamaları
    else if (_usart == UART5) {
        dma_flexible_config(DMA2, FLEX_CHANNEL1, DMA_FLEXIBLE_UART5_RX);
        dma_flexible_config(DMA2, FLEX_CHANNEL2, DMA_FLEXIBLE_UART5_TX);
    }
    // UART7 için esnek kanal atamaları
    else if (_usart == UART7) {
        dma_flexible_config(DMA2, FLEX_CHANNEL4, DMA_FLEXIBLE_UART7_RX);
        dma_flexible_config(DMA2, FLEX_CHANNEL5, DMA_FLEXIBLE_UART7_TX);
    }

    dma_channel_enable(_dma_rx, TRUE);   
}


/* ---------------- RING BUFFER ---------------- */
int HardwareSerial::available() {
  uint16_t write_pos = _size - dma_data_number_get(_dma_rx);
  if (write_pos >= _read) return write_pos - _read;
  else return _size - _read + write_pos;
}

int HardwareSerial::read()
{
  if (!available()) return -1;
  uint8_t c = _buf[_read++];
  if (_read >= _size) _read = 0;
  return c;
}

void HardwareSerial::write(const uint8_t *data, uint16_t len)
{
  while (_tx_busy); // blocking (ilk sürüm için ideal)
  _tx_busy = true;
  dma_channel_enable(_dma_tx, FALSE);
  _dma_tx->maddr = (uint32_t)data;
  _dma_tx->dtcnt = len;
  dma_channel_enable(_dma_tx, TRUE);
}

void HardwareSerial::write(uint8_t c)
{
  write(&c, 1);
}

void HardwareSerial::print(const char *s)
{
  write((uint8_t*)s, strlen(s));
}

void HardwareSerial::println(const char *s)
{
  write((uint8_t*)s, strlen(s));
  write((uint8_t)0x0D);
  write((uint8_t)0x0A);
}

void HardwareSerial::onTxComplete(void)
{
  dma_channel_enable(_dma_tx, FALSE);
  _tx_busy = false;
}





// ISR'ler şöyle olur:

extern "C" void DMA1_Channel4_IRQHandler(void)  // USART1 TX - DMA1 CH4
{
    if (dma_flag_get(DMA1_FDT4_FLAG) != RESET) {
        dma_flag_clear(DMA1_FDT4_FLAG);
        Serial1.onTxComplete();
    }
}

extern "C" void DMA1_Channel7_IRQHandler(void)  // USART2 TX - DMA1 CH7
{
    if (dma_flag_get(DMA1_FDT7_FLAG) != RESET) {
        dma_flag_clear(DMA1_FDT7_FLAG);
        Serial2.onTxComplete();
    }
}

extern "C" void DMA1_Channel2_IRQHandler(void)  // USART3 TX - DMA1 CH2
{
    if (dma_flag_get(DMA1_FDT2_FLAG) != RESET) {
        dma_flag_clear(DMA1_FDT2_FLAG);
        Serial3.onTxComplete();
    }
}

// extern "C" void DMA2_Channel4_5_IRQHandler(void)  // UART4 TX - DMA2 CH5 (ortak IRQ)
// {
//     if (dma_flag_get(DMA2_FDT5_FLAG) != RESET) {
//         dma_flag_clear(DMA2_FDT5_FLAG);
//         Serial4.onTxComplete();
//     }
    
// }

// === GÜNCELLEME: UART5 TX Kesmesi (DMA2 Kanal 2) ===
extern "C" void DMA2_Channel2_IRQHandler(void)
{
    if (dma_flag_get(DMA2_FDT2_FLAG) != RESET) {
        dma_flag_clear(DMA2_FDT2_FLAG);
        Serial5.onTxComplete(); // UART5 nesne adınız farklıysa güncelleyin
    }
}

// === GÜNCELLEME: UART4 ve UART7 Ortak TX Kesmesi (DMA2 Kanal 4 ve 5) ===
extern "C" void DMA2_Channel4_5_IRQHandler(void)
{
    // UART4 TX kontrolü (Kanal 5)
    if (dma_flag_get(DMA2_FDT5_FLAG) != RESET) {
        dma_flag_clear(DMA2_FDT5_FLAG);
        Serial4.onTxComplete();
        
        // ÖNEMLİ: UART7 TX'i de DMA2_CHANNEL5'e atadığımız için, 
        // tetiklenen nesnenin Serial7 olup olmadığını da kontrol etmeliyiz.
        Serial7.onTxComplete(); 
    }
}
