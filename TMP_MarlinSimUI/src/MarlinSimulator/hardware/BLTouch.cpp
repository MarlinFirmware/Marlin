#define BLTOUCH_DEPLOY          10
#define BLTOUCH_STOW            90
#define BLTOUCH_SW_MODE         60
#define BLTOUCH_SELFTEST       120
#define BLTOUCH_MODE_STORE     130
#define BLTOUCH_5V_MODE        140
#define BLTOUCH_OD_MODE        150
#define BLTOUCH_RESET          160

#include "print_bed.h"
#include "bed_probe.h"
#include "pwm_reader.h"

#include "BLTouch.h"

void BLTouchProbe::interrupt(GpioEvent& ev) {
  if (ev.pin_id == m_servo_pin) {
    auto angle = dutycycle_to_degrees(m_servo->pwm_duty);
    if (is_close_enough(angle, BLTOUCH_DEPLOY)) {
      m_probe->enabled = true;
      m_deployed = true;
    } else if (is_close_enough(angle, BLTOUCH_STOW)) {
      m_probe->enabled = false;
      m_deployed = false;
    }
  }
}
