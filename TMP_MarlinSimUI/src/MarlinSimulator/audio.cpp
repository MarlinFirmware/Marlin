#include <SDL2/SDL.h>
#include "audio.h"

struct SdlAudio{
  SDL_AudioSpec want;
  SDL_AudioSpec have;
  SDL_AudioDeviceID dev;
};

static bool running = false;
static SdlAudio audio;
static Config config = {.sampleFrequency = 44100, // Standard CD quality
                        .sampleSize = 512,       // 11ms
                        .audioFrequency = 440,    // A4 frequency
                        .audioAmplitude = 200};   // Volume

void audio_pause() {
    SDL_PauseAudioDevice(audio.dev, 1);
}

void audio_start() {
    SDL_PauseAudioDevice(audio.dev, 0);
}

void generate_square_wave(void *userdata, Uint8 *stream, int len) {
  Config *config = (Config *)userdata;
  if (!config->active && running) {
    audio_pause();
    running = false;
    return;
  }
  running = true;

  static uint32_t sample_index = 0;
  uint32_t sampleCount = len / sizeof(int16_t);
  uint32_t samples_per_half_wave = (config->sampleFrequency / config->audioFrequency) / 2;

  for (uint32_t i = 0; i < sampleCount; i++, sample_index++) {
    int16_t *sample_buffer = (int16_t *)stream + i;
    if (sample_index / samples_per_half_wave % 2) {
      *sample_buffer = config->audioAmplitude;
    } else {
      *sample_buffer = -config->audioAmplitude;
    }
  }
}

static std::function<void(void*, Uint8*, int)> custom_audio_cb {&generate_square_wave};

void audio_set_sample_cb(std::function<void(void*, Uint8*, int)> new_audio_cb) {
    audio_pause();
    custom_audio_cb = new_audio_cb;
    audio_start();
}

void audio_sample_cb(void *userdata, Uint8 *stream, int len) {
    if (custom_audio_cb != nullptr) {
        custom_audio_cb(userdata, stream, len);
        return;
    }
    audio_pause();
}

int audio_init() {
  SDL_zero(audio.want);

  audio.want.freq = config.sampleFrequency;   // Samples per second
  audio.want.format = AUDIO_S16;              // Signed 16-bit samples
  audio.want.channels = 1;                    // Mono
  audio.want.samples = config.sampleSize;     // Samples per callback
  audio.want.userdata = &config;              // Passed to callback
  audio.want.callback = audio_sample_cb;      // Callback function

  audio.dev = SDL_OpenAudioDevice(NULL, 0, &audio.want, &audio.have, 0);

  if (audio.dev == 0) {
    return 1;
  }

  if (audio.want.format != audio.have.format) {
    return 1;
  }

  return 0;
}

void tone_start(int frequency) {
    config.audioFrequency = frequency;
    config.active = true;
    audio_start();
}

void tone_start(int frequency, float volume) {
    config.audioFrequency = frequency;
    switch (audio.have.format) {
        case AUDIO_S16:
            config.audioAmplitude = ((float)INT16_MAX * volume);
            break;
        default: break;
    }
    audio_start();
}

void tone_stop() {
    config.active = false; // along with running flag, will make sure that 1 set of samples is always executed
}
