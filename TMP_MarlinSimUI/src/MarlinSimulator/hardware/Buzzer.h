#pragma once

#include <atomic>
#include <chrono>

#include <imgui.h>
#include "../audio.h"

#include "Gpio.h"
#include "../virtual_printer.h"

class Buzzer : public VirtualPrinter::Component {
public:
  Buzzer(pin_type pin, bool invert_logic) : VirtualPrinter::Component("Buzzer"), pin(pin), invert_logic(invert_logic) {
    Gpio::attach(pin, [this](GpioEvent& ev){ this->interrupt(ev); });

    audio_set_sample_cb([this](void* userdata, Uint8* sample_buffer, int samples_buffer_size){
      std::scoped_lock lock(events_mutex);
      Config *config = (Config *)userdata;
      int16_t *samples = (int16_t *)sample_buffer;
      size_t sample_count = samples_buffer_size / sizeof(int16_t);
      auto now = Kernel::SimulationRuntime::nanos();
      static uint64_t last_cb_sim_time = 0;

      if (m_frequency == 0 || m_volume == 0.0 || Kernel::TimeControl::realtime_scale == 0.0) {
        last_cb_sim_time = now;
        return;
      }

      static uint64_t sample_index = 0;
      uint32_t samples_per_half_wave = (config->sampleFrequency / m_frequency) / 2;
      double nanos_per_sample = (double)Kernel::TimeControl::ONE_BILLION / config->sampleFrequency;
      double time_scale = Kernel::TimeControl::realtime_scale;
      samples_per_half_wave = time_scale > 0 ? samples_per_half_wave / time_scale : 0;
      for (uint32_t i = 0; i < sample_count; i++, sample_index++) {
        if (samples_per_half_wave > 0 && buzzer_pin_was_high((last_cb_sim_time) + (i * nanos_per_sample))) {
          if (sample_index / samples_per_half_wave % 2) {
            samples[i] = INT16_MAX * m_volume;
          } else {
            samples[i] = -(INT16_MAX * m_volume);
          }
        } else {
          samples[i] = 0;
        }
      }
      last_cb_sim_time = now;
    });

    audio_start();
  }
  ~Buzzer() {}

  void ui_widget() {
    int freq = m_frequency;
    if(ImGui::InputInt("Hz", &freq)) {
      m_frequency = freq;
    }

    float vol = m_volume;
    if(ImGui::SliderFloat("Volume", &vol, 0.0f, 1.0f)) {
      m_volume = vol;
    }
  }

  void interrupt(GpioEvent& ev) {
    if (ev.event == ev.RISE || ev.event == ev.FALL) {
      std::scoped_lock lock(events_mutex);
      events.push_back(ev);
    }
  }

private:

  bool buzzer_pin_was_high(uint64_t nanos) {
    auto ticks = Kernel::TimeControl::nanosToTicks(nanos);
    static bool buzzer_state = false;
    while (events.size() > 0) {
      if (events.front().timestamp > ticks) return buzzer_state;
      if (events.front().timestamp <= ticks) {
        buzzer_state = events.front().event == GpioEvent::RISE;
        events.pop_front();
        continue;
      }
    }
    return buzzer_state;
  }

  const pin_type pin;
  const bool invert_logic;
  int m_frequency = 440;
  float m_volume = 0.10f;

  std::deque<GpioEvent> events;
  std::mutex events_mutex;
};
