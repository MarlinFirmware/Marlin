#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(FAST_PWM_FAN)

  #include <pwm.h>

  void set_pwm_frequency(const pin_t pin, int f_desired) {
    pwm_set_frequency(pin, f_desired);
  }

  void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
    pwm_write_ratio(pin, invert ? 1.0f - (float)v / v_size : (float)v / v_size);
  }

#endif // FAST_PWM_FAN
#endif // TARGET_LPC1768
