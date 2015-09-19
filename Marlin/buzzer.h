#ifndef BUZZER_H
  #define BUZZER_H

  #if HAS_BUZZER
    void buzz(long duration, uint16_t freq);
  #endif

#endif //BUZZER_H