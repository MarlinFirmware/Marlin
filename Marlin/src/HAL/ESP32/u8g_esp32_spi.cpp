#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfigPre.h"
#ifdef FYSETC_MINI_12864_2_1

#include <U8glib-HAL.h>
#include "Arduino.h"
#include "../shared/HAL_SPI.h"
#include "SPI.h"


static SPISettings spiConfig;
static uint8_t msgInitCount = 2; // Ignore all messages until 2nd U8G_COM_MSG_INIT

  #define MDOGLCD_MOSI                     23
  #define MDOGLCD_SCK                      18
  #define MLCD_RESET_PIN                   0
  #define MLCD_PINS_DC                     4
  #define MDOGLCD_CS                       21
  #define MDOGLCD_A0                       4

#ifndef LCD_SPI_SPEED
  #ifdef SD_SPI_SPEED
    #define LCD_SPI_SPEED SD_SPI_SPEED    // Assume SPI speed shared with SD
  #else
    #define LCD_SPI_SPEED SPI_FULL_SPEED  // Use full speed if SD speed is not supplied
  #endif
#endif

uint8_t u8g_eps_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  if (msgInitCount) {
    if (msg == U8G_COM_MSG_INIT) msgInitCount --;
    if (msgInitCount) return -1;
  }

  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;
    case U8G_COM_MSG_INIT:

      pinMode(MDOGLCD_CS, OUTPUT);
      pinMode(MDOGLCD_A0, OUTPUT);
      pinMode(MLCD_RESET_PIN, OUTPUT);

      digitalWrite(MDOGLCD_CS, HIGH);
      digitalWrite(MDOGLCD_A0, HIGH);
      digitalWrite(MLCD_RESET_PIN, HIGH);
      u8g_Delay(5);
      spiBegin();
      spiInit(LCD_SPI_SPEED);

      break;

    case U8G_COM_MSG_ADDRESS:           /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
        if(arg_val)
            digitalWrite(MDOGLCD_A0, HIGH);
        else
            digitalWrite(MDOGLCD_A0, LOW);
      break;

    case U8G_COM_MSG_CHIP_SELECT:       /* arg_val == 0 means HIGH level of U8G_PI_CS */
        digitalWrite(MDOGLCD_CS, arg_val ? LOW : HIGH);
      break;

    case U8G_COM_MSG_RESET:
        digitalWrite(MLCD_RESET_PIN, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      spiSend((uint8_t)arg_val);
    break;

    case U8G_COM_MSG_WRITE_SEQ:
      uint8_t *ptr = (uint8_t*) arg_ptr;
        while (arg_val > 0) {
          spiSend(*ptr++);
          arg_val--;
        }
      break;
  }
  return 1;
}

#endif // MKS_MINI_12864_V3

#endif // ARDUINO_ARCH_ESP32
