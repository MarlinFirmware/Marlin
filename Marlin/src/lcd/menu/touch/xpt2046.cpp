#include <Arduino.h>

#if defined(ARDUINO_ARCH_STM32)
#include "xpt2046.h"

#include "../../inc/MarlinConfig.h"
#ifdef LONGER3D
#include "../../longer/LGT_MACRO.h"
#endif
void initTouch_sw_SPI()
{
  pinMode(TP_INT, INPUT);  // Pendrive interrupt pin, used as pooling in IStouched
  pinMode(TP_CS2, OUTPUT);
  pinMode(TP_SCK, OUTPUT);
  pinMode(TP_MOSI, OUTPUT);
  pinMode(TP_MISO, INPUT);

  //spiConfig = SPISettings(XPT2046_SPI_CLOCK, MSBFIRST, SPI_MODE0);
  // Not needed, as these settings are hardcoded in the function

  digitalWrite(TP_SCK,0);
  digitalWrite(TP_CS2, 1);

  // Dummy read to initialize pd1 and pd0 bits from ADS7843
  getTouchCoordinate_sw_SPI(XPT2046_X);
}

uint16_t getTouchCoordinate_sw_SPI(uint8_t coordinate)
{
  uint16_t data[3], delta[3];

  coordinate |= XPT2046_CONTROL | XPT2046_DFR_MODE;

  for (uint16_t i = 0; i < 3 ; i++) {

    digitalWrite(TP_CS2, LOW);

    for (uint16_t j=0x80; j>0x00; j>>=1){
      digitalWrite(TP_SCK,0);
      digitalWrite(TP_MOSI,(bool)(coordinate & j));
      digitalWrite(TP_SCK, 1);
    }

    data[i] = 0;
    for (uint32_t j=0x8000; j!=0x0000; j>>=1){
      digitalWrite(TP_SCK,0);
      if (digitalRead(TP_MISO)) data[i]=(data[i] | j);
      digitalWrite(TP_SCK, 1);
    }

    digitalWrite(TP_CS2, HIGH);
    digitalWrite(TP_SCK,0);
    data[i]>>=4;
  }

  delta[0] = data[0] > data[1] ? data[0] - data[1] : data[1] - data[0];
  delta[1] = data[0] > data[2] ? data[0] - data[2] : data[2] - data[0];
  delta[2] = data[1] > data[2] ? data[1] - data[2] : data[2] - data[1];
#ifdef LONGER3D
///////////////////////////////////////////////////////
// Serial1.println("test:");
// for(int i=0;i<3;i++)
// {
//    Serial1.println(delta[i]);
// }
//////////////////////////////////////////////////
  for(int i=0;i<3;i++)
  {
    if(delta[i]<ERR_RANGE)
      continue;
    else
      return 0;
  }
#endif
  if (delta[0] <= delta[1] && delta[0] <= delta[2])
    return (data[0] + data[1]) >> 1;
  if (delta[1] <= delta[2])
    return (data[0] + data[2]) >> 1;

  return (data[1] + data[2]) >> 1;
}


bool getTouchPoint(uint16_t *x, uint16_t *y)
{
  if (!isTouched()) return false;
  *x = getTouchCoordinate_sw_SPI(XPT2046_X);
  *y = getTouchCoordinate_sw_SPI(XPT2046_Y);
  if (!isTouched()) return false;

  return true;
}

#else // !ARDUINO_ARCH_STM32
void initTouch_sw_SPI() {}
uint16_t getTouchCoordinate_sw_SPI(uint8_t coordinate) { return 0; }
bool getTouchPoint(uint16_t *x, uint16_t *y) { return false; }
#endif

