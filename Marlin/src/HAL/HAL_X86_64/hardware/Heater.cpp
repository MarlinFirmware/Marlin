#ifdef __PLAT_X86_64__

#include <stdio.h>
#include "../../../inc/MarlinConfig.h"

#include "Heater.h"

Heater::Heater(pin_t heater, pin_t adc) {
  heater_state = 0;
  room_temp_raw = 150;
  last = std::chrono::high_resolution_clock::now();
  heater_pin = heater;
  adc_pin = adc;
  heat = 0.0;
}

Heater::~Heater() {
}

void Heater::update() {
  // crude pwm read and cruder heat simulation
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> delta = (now - last);
  if(delta.count() > 1 ) {
    heater_state = pwmcap.update(0xFFFF * Gpio::pin_map[heater_pin].value);
    last = now;
    heat += (heater_state - heat) * (delta.count() / 1000000.0);

    if(heat < room_temp_raw) heat = room_temp_raw;
    Gpio::pin_map[analogInputToDigitalPin(adc_pin)].value = 0xFFFF - (uint16_t)heat;
  }
}

void Heater::interrupt(GpioEvent ev) {
  // ununsed
}

#endif // __PLAT_X86_64__
