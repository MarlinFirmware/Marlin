#ifdef __PLAT_X86_64__

extern void setup();
extern void loop();

#include <thread>

#include "../../inc/MarlinConfig.h"
#include <stdio.h>
#include <stdarg.h>
#include "../Delay.h"
#include "hardware/Heater.h"
#include "hardware/LinearAxis.h"

// simple stdout / stdin implementation for fake serial port
void write_serial_thread() {
  for (;;) {
    for (std::size_t i = usb_serial.transmit_buffer.available(); i > 0; i--) {
      fputc(usb_serial.transmit_buffer.read(), stdout);
    }
    std::this_thread::yield();
  }
}

void read_serial_thread() {
  char buffer[255] = {};
  for (;;) {
    if (fgets(buffer , 255 , stdin)) {
      std::size_t len = MIN(strlen(buffer), usb_serial.receive_buffer.free());
      for (std::size_t i = 0; i < len; i++) {
        usb_serial.receive_buffer.write(buffer[i]);
      }
    }
    std::this_thread::yield();
  }
}

void simulation_loop() {
  Heater hotend(HEATER_0_PIN, TEMP_0_PIN);
  Heater bed(HEATER_BED_PIN, TEMP_BED_PIN);
  LinearAxis x_axis(X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN, X_MIN_PIN, X_MAX_PIN);
  LinearAxis y_axis(Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_MIN_PIN, Y_MAX_PIN);
  LinearAxis z_axis(Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_MIN_PIN, Z_MAX_PIN);
  LinearAxis extruder0(E0_ENABLE_PIN, E0_DIR_PIN, E0_STEP_PIN, P_NC, P_NC);

  for(;;) {

    hotend.update();
    bed.update();

    x_axis.update();
    y_axis.update();
    z_axis.update();
    extruder0.update();

    std::this_thread::yield();
  }
}

int main(void) {
  std::thread write_serial (write_serial_thread);
  std::thread read_serial (read_serial_thread);

  #if NUM_SERIAL > 0
    MYSERIAL0.begin(BAUDRATE);
    SERIAL_PRINTF("x86_64 Initialised\n", SystemCoreClock / 1000000);
    SERIAL_FLUSHTX();
  #endif

  HAL_timer_init();

  std::thread simulation (simulation_loop);

  DELAY_US(10000);

  setup();
  for (;;) loop();

  simulation.join();
  write_serial.join();
  read_serial.join();

}




#endif
