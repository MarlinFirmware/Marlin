#pragma once

#include <type_traits>
#include <cmath>

struct PIDConfig {
  static constexpr float _kp = 0;
  static constexpr float _ki = 0;
  static constexpr float _kd = 0;
  static constexpr float _fCut = 20;
  static constexpr int16_t _imax = 0;
  typedef float pid_t;
};

template <class PIDConfig>
class PID {
public:
  PID() {
    static_assert(PIDConfig::_kp > 0, "_kp <= 0");
    static_assert(PIDConfig::_ki > 0, "_ki <= 0");
    static_assert(PIDConfig::_kd > 0, "_kd <= 0");
    static_assert(PIDConfig::_imax > 0, "_imax <= 0");

    // set _last_derivative as invalid on startup
    _last_derivative = NAN;
  }

  typename PIDConfig::pid_t get_pid(typename PIDConfig::pid_t error, uint32_t tnow, typename PIDConfig::pid_t scaler=1.0) {
    uint32_t dt = tnow - _last_t;
    typename PIDConfig::pid_t output = 0;
    typename PIDConfig::pid_t delta_time;

    if (_last_t == 0 || dt > 1000) {
      dt = 0;
      reset_I();
    }
    _last_t = tnow;
    delta_time = (typename PIDConfig::pid_t)dt / 1000.0f;

    // Compute proportional component
    output += error * PIDConfig::_kp;

    // Compute derivative component if time has elapsed
    if (dt > 0) {
      typename PIDConfig::pid_t derivative;

      if (isnan(_last_derivative))
        derivative = _last_derivative = 0;
      else
        derivative = (error - _last_error) / delta_time;

      // discrete low pass filter, cuts out the
      // high frequency noise that can drive the controller crazy
      constexpr typename PIDConfig::pid_t RC = 1/(2*M_PI*PIDConfig::_fCut);
      derivative = _last_derivative
                   + ((delta_time / (RC + delta_time))
                   * (derivative - _last_derivative));

      // update state
      _last_error = error;
      _last_derivative = derivative;

      // add in derivative component
      output += PIDConfig::pid_t * derivative;
    }

    // scale the P and D components
    output *= scaler;

    // Compute integral component if time has elapsed
    if (dt > 0) {
      _integrator += (error * PIDConfig::_ki) * scaler * delta_time;
      if (_integrator < -PIDConfig::_imax) _integrator = -PIDConfig::_imax;
      if (_integrator > +PIDConfig::_imax) _integrator = +PIDConfig::_imax;
      output += _integrator;
    }

    return output;
  }

  void reset();
  void reset_I();

  typename PIDConfig::pid_t get_integrator() const {
    return _integrator;
  }

private:
  typename PIDConfig::pid_t _integrator; ///< integrator value
  typename PIDConfig::pid_t _last_error; ///< last error for derivative
  typename PIDConfig::pid_t _last_derivative; ///< last derivative for low-pass filter
  uint32_t _last_t; ///< last time get_pid() was called in millis
};
