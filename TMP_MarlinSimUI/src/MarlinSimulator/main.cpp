#include <thread>
#include <atomic>

#include "application.h"
#include "execution_control.h"

#include "src/inc/MarlinConfig.h"

#include "RawSocketSerial.h"
#include "audio.h"

RawSocketSerial net_serial{};

std::atomic_bool main_finished = false;

void HAL_idletask() {
  Kernel::yield();
}

extern void setup();
extern void loop();
void marlin_loop() {
  static bool initialised = false;
  if (!initialised) {
    initialised = true;
    #ifdef MYSERIAL0
      MYSERIAL0.begin(BAUDRATE);
      SERIAL_FLUSHTX();
    #endif
    HAL_timer_init();
    setup();
  } else loop();
}

void simulation_main() {
  #ifdef __APPLE__
    pthread_setname_np("simulation_main");
  #else
    pthread_setname_np(pthread_self(), "simulation_main");
  #endif

  // Marlin Loop 500hz
  Kernel::Timers::timerInit(3, 1000000);
  Kernel::Timers::timerStart(3, 500);
  Kernel::Timers::timerEnable(3);
  Kernel::is_initialized(true);

  while(!main_finished) {
    try {
      Kernel::execute_loop();
    } catch (std::runtime_error& e) {
      // stack unrolled by exception in order to exit cleanly
      // todo: use a custom exception
      printf("Exception: %s\n", e.what());
      printf("Marlin thread terminated\n");
      main_finished = true;
    }
  }
}

// Main code
int main(int, char**) {
  bool audio_enabled = true; // TODO: get from config
  uint32_t sdl_flags = audio_enabled ? SDL_INIT_AUDIO : 0;
  SDL_Init(sdl_flags);
  SDLNet_Init();

  if (audio_enabled) audio_init();

  // Listen before starting simulator loop to avoid
  // thread synchronization issues if listen_on_port fails
  net_serial.listen_on_port(8099);

  Application app;
  std::thread simulation_loop(simulation_main);

  while (app.active) {
    app.update();
    app.render();
    std::this_thread::yield();
  }

  main_finished = true;
  Kernel::quit_requested = true;
  simulation_loop.join();
  net_serial.stop();

  SDLNet_Quit();
  SDL_Quit();

  return 0;
}
