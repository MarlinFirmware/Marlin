#ifndef DAC084S085_H
#define DAC084S085_H

class dac084s085 {
  public:
    dac084s085();
    static void begin(void);
    static void setValue(const uint8_t channel, const uint8_t value);
  private:
    static void cshigh();
};

#endif // DAC084S085_H
