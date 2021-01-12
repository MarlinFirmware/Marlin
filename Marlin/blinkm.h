/*
  blinkm.h
  Library header file for BlinkM library
 */
#ifndef BLINKM_H
#define BLINKM_H

#include "Arduino.h"
#include "Wire.h"

extern bool cancel_heatup;

#define BLINK_LED(MS)  blink_time = MS
#define BLINKFEED_LED(MS)  blinkfeed_time = MS
#define BLINKRETRACT_LED(MS)  blinkretract_time = MS

extern uint8_t key_flag;
extern uint8_t cancel_print;
extern uint16_t blink_time;
extern uint8_t print_key_flag;
extern float z_height_stop;
extern uint8_t print_pause;
extern uint32_t sys_time;
extern volatile uint32_t protect_time;

enum LED_STATUD
{
  LED_ON=4000,
  LED_BLINK_0=2500,
  LED_BLINK_1=1500,
  LED_BLINK_2=1000,
  LED_BLINK_3=800,
  LED_BLINK_4=500,
  LED_BLINK_5=300,
  LED_BLINK_6=150,
  LED_BLINK_7=50,
  LED_OFF = 0,
};

//extern uint16_t blink_time;

void BlinkLed(void);
void BlinkFeedLed(void);
void BlinkRetractLed(void);

void SendColors(byte red, byte grn, byte blu);
void PrintOneKey(void);

void LoadFilament(void);

void heat_protect(void);
void home_key(void);

#endif
