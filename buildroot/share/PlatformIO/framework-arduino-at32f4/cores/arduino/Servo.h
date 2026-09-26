#ifndef SERVO_H
#define SERVO_H

#include "at32f403a_407.h"
#include <stdint.h>
#include <stdbool.h>

#define MIN_PULSE_WIDTH 544    // 0 derece için mikrosaniye
#define MAX_PULSE_WIDTH 2400   // 180 derece için mikrosaniye
#define DEFAULT_PULSE_WIDTH 1500 // 90 derece için
#define REFRESH_INTERVAL 20000 // 20ms PWM periyodu



class Servo {
private:
    bool attached;
    uint32_t pin;
    tmr_type *tmr;
    tmr_channel_select_type channel;
    // uint16_t min_pulse;
    // uint16_t max_pulse;
    uint16_t current_angle;
    
    void configure_timer(void);
    void configure_pin(void);
    uint16_t angle_to_pulse(uint16_t angle);
    void Get_Channel(uint8_t ch);
    
public:
    Servo();
    uint16_t min_pulse = MIN_PULSE_WIDTH;  // Constructor'da init et
    uint16_t max_pulse = MAX_PULSE_WIDTH;

    bool attach(uint32_t servo_pin, uint16_t min = MIN_PULSE_WIDTH, uint16_t max = MAX_PULSE_WIDTH);
    void detach();
    void write(uint16_t angle);
    void writeMicroseconds(uint16_t pulse_width);
    uint16_t read();
    bool attached_state();
};

#endif