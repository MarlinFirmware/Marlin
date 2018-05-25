#ifdef __PLAT_X86_64__

extern void setup();
extern void loop();

#include <thread>

#include <iostream>
#include <fstream>

#include "../../inc/MarlinConfig.h"
#include <stdio.h>
#include <stdarg.h>
#include "../Delay.h"
#include "hardware/IOLoggerCSV.h"
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
    std::size_t len = MIN(usb_serial.receive_buffer.free(), 254U);
    if (fgets(buffer, len, stdin)) {
      for (std::size_t i = 0; i < strlen(buffer); i++) {
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

/*
 *
 * Uncomment these lines to enable full GPIO and Positional Logging
 *
 *
 */

//  IOLoggerCSV logger("all_gpio_log.csv");
//  Gpio::attachLogger(&logger);
//
//  std::ofstream position_log;
//  position_log.open("axis_position_log.csv");
//
//  int32_t x,y,z;

  for(;;) {

    hotend.update();
    bed.update();

    x_axis.update();
    y_axis.update();
    z_axis.update();
    extruder0.update();

//    // log postion over time
//    if(x_axis.position != x || y_axis.position != y || z_axis.position != z) {
//      uint64_t update = MAX3(x_axis.last_update, y_axis.last_update, z_axis.last_update);
//      position_log << update << ", "<< x_axis.position << ", " << y_axis.position << ", " << z_axis.position << std::endl;
//      position_log.flush();
//      x = x_axis.position;
//      y = y_axis.position;
//      z = z_axis.position;
//    }
//    // flush the logger
//    logger.flush();

    std::this_thread::yield();
  }
}

int main(void) {
  std::thread write_serial (write_serial_thread);
  std::thread read_serial (read_serial_thread);

  #if NUM_SERIAL > 0
    MYSERIAL0.begin(BAUDRATE);
    SERIAL_PRINTF("x86_64 Initialised\n");
    SERIAL_FLUSHTX();
  #endif

  Clock::setFrequency(F_CPU);
  Clock::setTimeMultiplier(1.0); // some testing at 10x

  HAL_timer_init();

  std::thread simulation (simulation_loop);

  DELAY_US(10000);

  setup();
  for (;;) {
    loop();
    std::this_thread::yield();
  }

  simulation.join();
  write_serial.join();
  read_serial.join();

}




#endif
