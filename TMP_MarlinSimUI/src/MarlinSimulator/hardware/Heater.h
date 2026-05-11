#pragma once

#include <cmath>

#include "Gpio.h"

#include "../virtual_printer.h"

/**
 * data structures for future generalisation
 **/

struct enclosure_data {
  double temperature;
};

struct heater_data {
  double voltage, resistance;
};

struct hotend_data {
  double mass, surface_area, specific_heat;
};

struct adc_data {
  double pullup_resistance;
  uint32_t resolution;
};

class Heater: public VirtualPrinter::Component {
public:

  enum temperature_sensor_mode {
    Normal,
    ForceMin,
    ForceMax,
    ForcePause
  };

  Heater(pin_type heater_pin, pin_type adc_pin, heater_data heater, hotend_data hotend, adc_data adc);
  virtual ~Heater();
  void interrupt(GpioEvent& ev);
  void update();
  void ui_widget();

  pin_type heater_pin, adc_pin;

  //heater element
  double heater_volts = 12.0;
  double heater_resistance = 3.6; // 40Watts

  //pwm sim
  uint64_t pwm_period = 0;
  uint64_t pwm_duty = 0;
  uint64_t pwm_hightick = 0;
  uint64_t pwm_lowtick = 0;
  uint64_t pwm_last_update = 0;

  //hotend block
  double hotend_ambient_temperature = 25.0;
  double hotend_temperature = 0.0;
  double hotend_energy = 0.0;
  double hotend_mass = 13.0; // g [approxiamte of 2x2x1 heatblock + nozzle]

  // // intermediates for sphere approximation
  // double hotend_density = 2.7; // gm/cm^3 (Aluminium)
  // double hotend_volume = hotend_mass / hotend_density; // cm^3
  // double hotend_radius = std::cbrt((hotend_volume * 3.0) / (4.0 * M_PI)); // cm
  // double hotend_surface_area = 4.0 * M_PI * hotend_radius * hotend_radius; // cm^2
  // // ****

  double hotend_surface_area = (16.0 + 4.0);// cm^2 [approxiamte of 2x2x1 heatblock + nozzle]
  double hotend_specific_heat = 0.897; // j/g/C (Aluminum)
  double hotend_convection_transfer = 0.001; // 0.001 W/cm^2 . C is an approximate often used for convective heat transfer into slow moving air

  //adc
  double adc_pullup_resistance = 4700;
  uint32_t adc_resolution = 12;
  temperature_sensor_mode m_temperature_sensor_mode {};
};
