#include "MarlinConfig.h"

#include "apa_102_led.h"

#if ENABLED(APA102_LED)

Adafruit_DotStar strip = Adafruit_DotStar(APA102_LED_PIXELS, APA102_LED_DPIN, APA102_LED_CPIN, APA102_LED_TYPE);

void set_indv_apa_color(uint16_t i, const uint32_t color)
{
  if(i < strip.numPixels()){
    strip.setPixelColor(i, color);
  }
}

void set_apa102_color(const uint32_t color)
{
    for (uint16_t i = 0; i < strip.numPixels(); i ++){
        strip.setPixelColor(i, color); // 'On' pixel at head
    }
    strip.show();
}

void setup_apa102()
{
  strip.begin();
  strip.setBrightness(APA102_BRIGHTNESS);
  strip.show();

  //set_apa102_color(strip.Color(255, 255, 255));
}


#endif
