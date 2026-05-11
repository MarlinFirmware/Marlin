#pragma once

#include <fstream>

#include "window.h"
#include "user_interface.h"

#include "hardware/Heater.h"
#include "hardware/print_bed.h"

#include "visualisation.h"

#include <string>
#include <imgui.h>
#include <implot.h>
#include "user_interface.h"

#include "virtual_printer.h"

class Simulation {
public:

  Simulation() : vis(testPrinter) {}

  void init() {
    testPrinter.build();
    vis.create();
  }

  void process_event(SDL_Event& e) {}

  void update() {
    testPrinter.update();
  }

  void ui_callback(UiWindow*) {

  }

  void ui_info_callback(UiWindow *w) {
    vis.ui_info_callback(w);
  }

  VirtualPrinter testPrinter;
  Visualisation vis;
};

class Application {
public:
  Application();
  ~Application();

  void update();
  void render();

  bool active = true;
  Window window;
  UserInterface user_interface;
  Simulation sim;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  std::ifstream input_file;
};
