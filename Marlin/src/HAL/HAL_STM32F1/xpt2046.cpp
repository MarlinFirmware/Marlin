#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_BUTTONS)

#include "xpt2046.h"

#if ENABLED(TOUCH_CALIBRATION)
  #include "../../lcd/menu/touch/calibration.h"
#endif

static uint32_t timeout = 0;

extern int8_t encoderDiff;

void touch_swSPI_init(void)
{
  pinMode(TOUCH_INT, INPUT); // Pendrive interrupt pin, used as pooling in IStouched
  pinMode(TOUCH_CS, OUTPUT);
  pinMode(TOUCH_SCK, OUTPUT);
  pinMode(TOUCH_MOSI, OUTPUT);
  pinMode(TOUCH_MISO, INPUT);

  digitalWrite (TOUCH_SCK,0);
  digitalWrite (TOUCH_CS, 1);
  getInTouch(XPT2046_X); // this is a dummy read needed to enable pendrive status pin
}

uint8_t xpt2046_read_buttons()
{
  uint16_t x, y;
  int16_t tsoffsets[4] = { 0, 0, 0, 0 };

  if (timeout > millis())
    return 0;

  timeout = millis() + 250;

  #if ENABLED(TOUCH_CALIBRATION)
    tsoffsets[0] = calibration.results[0];
    tsoffsets[1] = calibration.results[1];
    tsoffsets[2] = calibration.results[2];
    tsoffsets[3] = calibration.results[3];
  #endif

  if (tsoffsets[0] + tsoffsets[1] == 0) {
    // Not yet set, so use defines as fallback...
    tsoffsets[0] = XPT2046_X_CALIBRATION;
    tsoffsets[1] = XPT2046_X_OFFSET;
    tsoffsets[2] = XPT2046_Y_CALIBRATION;
    tsoffsets[3] = XPT2046_Y_OFFSET;
  }

  // We rely on XPT2046 compatible mode to ADS7843, hence no Z1 and Z2 measurements possible.

  if (digitalRead(TOUCH_INT)) { return 0; } // if TOUCH_INT is high, no fingers are pressing on the touch screen > exit
  x = (uint16_t)((((uint32_t)(getInTouch(XPT2046_X))) * tsoffsets[0]) >> 16) + tsoffsets[1];
  y = (uint16_t)((((uint32_t)(getInTouch(XPT2046_Y))) * tsoffsets[2]) >> 16) + tsoffsets[3];
  if (digitalRead(TOUCH_INT)) { return 0; } // The fingers still need to be on the TS to have a valid read.

  if (y < 185 || y > 224) { return 0; }
  if (x >  20 && x <  99) encoderDiff = - ENCODER_STEPS_PER_MENU_ITEM * ENCODER_PULSES_PER_STEP;
  if (x > 120 && x < 199) encoderDiff = ENCODER_STEPS_PER_MENU_ITEM * ENCODER_PULSES_PER_STEP;
  if (x > 220 && x < 299) { return EN_C; }

  return 0;
}

uint16_t getInTouch(uint8_t coordinate)
{
  uint16_t data[3], delta[3];

  coordinate |= XPT2046_CONTROL | XPT2046_DFR_MODE;

  OUT_WRITE(TOUCH_CS, 0);

  for (uint16_t i = 0; i < 3 ; i++)
  {
    for (uint16_t j=0x80; j>0x00; j>>=1) {
      OUT_WRITE(TOUCH_SCK,0);
      OUT_WRITE(TOUCH_MOSI,(bool)(coordinate & j));
      OUT_WRITE(TOUCH_SCK, 1);
    }

    data[i] = 0;
    for (uint32_t j=0x8000; j!=0x0000; j>>=1) {
      OUT_WRITE(TOUCH_SCK,0);
      if (digitalRead(TOUCH_MISO)) data[i]=(data[i] | j);
      OUT_WRITE(TOUCH_SCK, 1);
    }

    OUT_WRITE(TOUCH_SCK,0);
    data[i]>>=4;
  }

  OUT_WRITE(TOUCH_CS, 1);

  delta[0] = data[0] > data[1] ? data[0] - data[1] : data[1] - data[0];
  delta[1] = data[0] > data[2] ? data[0] - data[2] : data[2] - data[0];
  delta[2] = data[1] > data[2] ? data[1] - data[2] : data[2] - data[1];

  if (delta[0] <= delta[1] && delta[0] <= delta[2])
    return (data[0] + data[1]) >> 1;
  if (delta[1] <= delta[2])
    return (data[0] + data[2]) >> 1;

  return (data[1] + data[2]) >> 1;
}

#endif // TOUCH_BUTTONS
#endif // __STM32F1__
