/*
  Ledstrip.h
  Library header file for Ledstrip library
 */

#define LEDSTRIP_OK         0
#define LEDSTRIP_NOLEDSPIN  1
#define LEDSTRIP_NONLEDS    2
#define LEDSTRIP_BADSEGMENT 3
#define LEDSTRIP_NOACTION   4

#define LED_POWEROFF    0
#define LED_POWERON     1
#define LED_POWERHALF   2
#define LED_POWERNOCHG  3

byte SendColorsOnLedstrip (int red, int grn, int blu, byte segment, byte power);

#if ENABLED(PRINTER_EVENT_LEDS)
  void handle_led_print_event(int code);
#endif
