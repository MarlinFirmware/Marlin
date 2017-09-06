#ifndef DAC084S085_H
#define DAC084S085_H

class dac084s085 {
  public:
    dac084s085();
    static void begin(void);
    static void setValue(uint8_t channel, uint8_t value);
};

#endif // DAC084S085_H
