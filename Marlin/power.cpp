//
// Created by christopher on 20.02.17.
//

#include "power.h"
#include "temperature.h"
#include "stepper_indirection.h"
#include "Marlin.h"
#include "MarlinConfig.h"
#include "ultralcd.h"

void Power::check() {
  bool powerNeeded = false;
  static millis_t lastPowerOn = 0;
  static millis_t nextPowerCheck = 0;
  millis_t ms = millis();
  if (ELAPSED(ms, nextPowerCheck)) {
    nextPowerCheck = ms + 2500UL;

#if ENABLED(FAN_NEEDS_POWER)
    for (uint8_t i = 0; i < FAN_COUNT; i++) {
      if (fanSpeeds[i] > 0) {
        powerNeeded = true;
      }
    }
#endif

#if ENABLED(AUTO_FAN_NEEDS_POWER)
    for (uint8_t i = 0; i < HOTENDS; i++) {
      if (autoFanSpeeds[i] > 0) {
        powerNeeded = true;
      }
    }
#endif

#if ENABLED(CONTROLLERFAN_NEEDS_POWER)
    if (controllerFanSpeed > 0) {
      powerNeeded = true;
    }
#endif

    if (X_ENABLE_READ == X_ENABLE_ON || Y_ENABLE_READ == Y_ENABLE_ON || Z_ENABLE_READ == Z_ENABLE_ON ||
        thermalManager.soft_pwm_bed > 0
        || E0_ENABLE_READ == E_ENABLE_ON // If any of the drivers are enabled...
#if E_STEPPERS > 1
      || E1_ENABLE_READ == E_ENABLE_ON
#if HAS_X2_ENABLE
              || X2_ENABLE_READ == X_ENABLE_ON
#endif
#if E_STEPPERS > 2
              || E2_ENABLE_READ == E_ENABLE_ON
#if E_STEPPERS > 3
                || E3_ENABLE_READ == E_ENABLE_ON
#endif
#endif
#endif
            ) {
      powerNeeded = true;
    }

    for (uint8_t i = 0; i < HOTENDS; i++) {
      if (thermalManager.target_temperature[i] > 0) {
        powerNeeded = true;
      }
    }

    if (thermalManager.target_temperature_bed > 0) {
      powerNeeded = true;
    }

    if (powerNeeded) {
      lastPowerOn = ms;
      this->power_on();
    } else {
      if (!lastPowerOn || ELAPSED(ms, lastPowerOn + (POWER_TIMEOUT) * 1000UL)) this->power_off();
    }
  }
}

void Power::power_on() {
  OUT_WRITE(PS_ON_PIN, PS_ON_AWAKE);
}

void Power::power_off() {
  OUT_WRITE(PS_ON_PIN, PS_ON_ASLEEP);
}


