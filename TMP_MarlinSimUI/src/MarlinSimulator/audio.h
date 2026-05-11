#pragma once

#include <functional>

struct Config {
  uint32_t sampleFrequency;
  uint32_t sampleSize;
  uint32_t audioFrequency;
  uint32_t audioAmplitude;
  bool active;
};

int audio_init();
void audio_set_sample_cb(std::function<void(void*, Uint8*, int)> new_audio_cb);
void audio_pause();
void audio_start();

void tone_start(int frequency);
void tone_start(int frequency, float volume);
void tone_stop();

