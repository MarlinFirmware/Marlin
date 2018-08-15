#include "MarlinConfig.h"

#define APA102_RGB  DOTSTAR_RGB
#define APA102_RBG  DOTSTAR_RBG
#define APA102_GRB  DOTSTAR_GRB
#define APA102_GBR  DOTSTAR_GBR
#define APA102_BRG  DOTSTAR_BRG
#define APA102_BGR  DOTSTAR_BGR


#define APA102_IS_RGB  (APA102_LED_TYPE == APA102_RGB || APA102_LED_TYPE == APA102_RBG || APA102_LED_TYPE == APA102_GRB || APA102_LED_TYPE == APA102_GBR || APA102_LED_TYPE == APA102_BRG || APA102_LED_TYPE == APA102_BGR)
#define APA102_IS_RGBW !APA102_IS_RGB

#if APA102_IS_RGB
  #define APA102_WHITE 255, 255, 255, 0
#else
  #define APA102_WHITE 0, 0, 0, 255
#endif

#include <Adafruit_DotStar.h>
//#include <SPI.h>

void set_indv_apa_color(uint16_t i, const uint32_t color);
void set_apa102_color(const uint32_t color);
void setup_apa102();

extern Adafruit_DotStar strip;
