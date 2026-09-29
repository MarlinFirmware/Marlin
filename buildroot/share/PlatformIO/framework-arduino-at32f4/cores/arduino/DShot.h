#ifndef DSHOT_AT32F403A_H
#define DSHOT_AT32F403A_H

#include "Arduino.h"
#include "at32f403a_407.h"

#define DSHOT_FRAME_SIZE 16
#define DSHOT_BUFFER_SIZE 24   // reset pulse dahil

class DShot {
public:
    enum Mode {
        DSHOT150_HZ  = 150000,
        DSHOT300_HZ  = 300000,
        DSHOT600_HZ  = 600000,
        DSHOT1200_HZ = 1200000
    };

    DShot(Mode mode = DSHOT600_HZ);
    void begin(uint8_t pin);           // Arduino tarzı attach + init
    void write(uint16_t throttle);     // 0 ~ 2047

private:
    Mode     _mode;
    tmr_type* _timer;
    tmr_channel_select_type  _channel;   // TMR_SELECT_CHANNEL_x
    dma_channel_type* _dmaChannel;
    uint32_t          _dmaFdt;
    uint8_t _pin;

    uint16_t dshot_prepare_packet(uint16_t throttle);
};

#endif
