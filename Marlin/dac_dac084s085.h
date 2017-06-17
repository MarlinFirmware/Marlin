#ifndef dac084s085_h
#define dac084s085_h

class dac084s085 {
  public:
    dac084s085();
    static void begin(void);
    static void setValue(uint8_t channel, uint8_t value);
};

#endif //dac084s085_h
