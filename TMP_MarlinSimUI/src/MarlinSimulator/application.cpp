#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>

#include "user_interface.h"
#include "application.h"
#include "logger.h"

#include "../HAL.h"
#include <src/MarlinCore.h>
#include <src/pins/pinsDebug.h>
#include <fstream>
#include <vcd_writer.h>
#include <regex>

Application::Application() {

  window.init({.title = "Marlin Simulator"});
  user_interface.init("imgui.ini");

  auto log_window = user_interface.addElement<LoggerWindow>("Log");
  logger::set_logger_callback([log_window](const std::string_view value){
    log_window->add_log(value);
  });

  sim.init();

  user_interface.m_main_menu = [this](){
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        // if (ImGui::MenuItem("Preferences")) {
        //   user_interface.ui_elements["Preferences"]->enable();
        // }
        if (ImGui::MenuItem("Quit")) {
          Kernel::quit_requested = true;
          active = false;
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View")) {
        if (ImGui::BeginMenu("Panel")) {
          for (auto& panel : user_interface.ui_elements) {
            if (panel.second->listed_on_main_menu) {
              bool panel_active = panel.second->active;
              if (ImGui::Checkbox(panel.first.c_str(), &panel_active)) {
                if (panel_active != panel.second->active) {
                  if (panel_active) {
                    panel.second->enable();
                  } else {
                    panel.second->disable();
                  }
                }
              }
            }
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  };

  user_interface.addElement<SerialMonitor>("Serial Monitor(0)");
  auto host_serial = user_interface.addElement<SerialMonitor>("Serial Monitor(1)");
  user_interface.addElement<SerialMonitor>("Serial Monitor(2)");
  user_interface.addElement<SerialMonitor>("Serial Monitor(3)");
  user_interface.addElement<SerialController>("SerialHost");

  user_interface.addElement<UiWindow>("Debug", [this](UiWindow* window){ this->sim.ui_info_callback(window); });

  user_interface.addElement<UiWindow>("Components", [this](UiWindow* window){ this->sim.testPrinter.ui_widgets(); });
  user_interface.addElement<Viewport>("Viewport", [this](UiWindow* window){ this->sim.vis.ui_viewport_callback(window); }, [this](UiWindow* window){ this->sim.vis.ui_viewport_menu_callback(window); });

  user_interface.addElement<UiWindow>("Simulation", [this](UiWindow* window){
    //Simulation Time
    uint64_t time_source = Kernel::SimulationRuntime::nanos();
    uint64_t hours = (time_source / (Kernel::TimeControl::ONE_BILLION * 60 * 60)) ;
    uint64_t remainder = (time_source % (Kernel::TimeControl::ONE_BILLION * 60 * 60));
    uint64_t mins = (remainder / (Kernel::TimeControl::ONE_BILLION * 60));
    remainder = (remainder % (Kernel::TimeControl::ONE_BILLION * 60));
    uint64_t seconds = remainder / (Kernel::TimeControl::ONE_BILLION);
    remainder = remainder % (Kernel::TimeControl::ONE_BILLION);
    ImGui::Text("%02ld:%02ld:%02ld.%09ld", hours, mins, seconds, remainder); //TODO: work around cross platform format string differences
    // Simulation Control
    auto ui_realtime_scale = Kernel::TimeControl::realtime_scale.load();
    ImGui::PushItemWidth(-1);
    ImGui::SliderFloat("##SimSpeed", &ui_realtime_scale, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
    ImGui::PopItemWidth();
    static float resume_scale = ui_realtime_scale;
    static bool paused = false;
    if (!paused) {
      if (ImGui::Button("Pause")) {
        resume_scale = ui_realtime_scale;
        ui_realtime_scale = 0.0f;
        paused = true;
      }
    } else if (ImGui::Button("Resume")) {
      ui_realtime_scale = resume_scale;
      paused = false;
    }

    ImGui::SameLine();
    if (ImGui::Button("Realtime")) { ui_realtime_scale = 1.0f; paused = false; }
    ImGui::SameLine();
    if (ImGui::Button("Max")) { ui_realtime_scale = 100.0f; paused = false; }
    ImGui::SameLine();
    if (ImGui::Button("Break")) Kernel::execution_break();
    Kernel::TimeControl::realtime_scale.store(ui_realtime_scale);
  });

  user_interface.addElement<UiPopup>("Preferences", true, [this](UiWindow* window){
    if (ImGui::Button("close")) { ImGui::CloseCurrentPopup(); }
  });

  user_interface.addElement<UiWindow>("Pin List", [this](UiWindow* window){
    for (auto p : pin_array) {
      bool value = Gpio::get_pin_value(p.pin);
      if(ImGui::Checkbox((std::string("##") + p.name).c_str(), &value)) {
        Gpio::set(p.pin, value);
      }
      ImGui::SameLine();
      ImGui::Text("[%04d]", Gpio::get_pin_value(p.pin));
      ImGui::SameLine();
      ImGui::Text("%s (%d)", p.name, p.pin);
    }
  });

  user_interface.addElement<UiWindow>("Signal Analyser", [this](UiWindow* window){
    if (!Gpio::isLoggingEnabled()) {
      if (ImGui::Button("Enable Pin Logging")) {
        Gpio::setLoggingEnabled(true);
      }
    }
    else {
      if (ImGui::Button("Disable Pin Logging")) {
        Gpio::setLoggingEnabled(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Reset logs")) {
        Gpio::resetLogs();
      }

      struct ScrollingData {
        int MaxSize;
        int Offset;
        ImVector<ImPlotPoint> Data;
        ScrollingData() {
            MaxSize = 100000;
            Offset  = 0;
            Data.reserve(MaxSize);
        }
        void AddPoint(double x, double y) {
            if (Data.size() < MaxSize)
                Data.push_back(ImPlotPoint(x,y));
            else {
                Data[Offset] = ImPlotPoint(x,y);
                Offset =  (Offset + 1) % MaxSize;
            }
        }
        void Erase() {
            if (Data.size() > 0) {
                Data.shrink(0);
                Offset  = 0;
            }
        }
      };

      static pin_type monitor_pin = X_STEP_PIN;
      static const char* label = "Select Pin";
      static char* active_label = (char *)label;
      if(ImGui::BeginCombo("##Select Pin", active_label)) {
        for (auto p : pin_array) {
          if (ImGui::Selectable(p.name, p.pin == monitor_pin)) {
            monitor_pin = p.pin;
            active_label = (char *)p.name;
          }
          if (p.pin == monitor_pin) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }

      if (Gpio::pin_map[monitor_pin].event_log.size()) {
        ScrollingData sdata;

        pin_log_data last{};
        for (auto v : Gpio::pin_map[monitor_pin].event_log) {
          if (last.timestamp) sdata.AddPoint(v.timestamp, last.value);
          sdata.AddPoint(v.timestamp, v.value);
          last = v;
        }
        sdata.AddPoint(Kernel::SimulationRuntime::nanos(),  last.value);

        static float window = 10000000000.0f;
        ImGui::SliderFloat("Window", &window, 10.f, 100000000000.f,"%.0f ns", ImGuiSliderFlags_Logarithmic);
        static float offset = 0.0f;
        ImGui::SliderFloat("X offset", &offset, 0.f, 10000000000.f,"%.0f ns");
        ImGui::SliderFloat("X offset", &offset, 0.f, 100000000000.f,"%.0f ns");
        if (ImPlot::BeginPlot("##SignalAnalyser", ImVec2(-1,150))) {
          ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_LockMin, ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_LockMin);
          ImPlot::SetupAxisLimits(ImAxis_X1, Kernel::SimulationRuntime::nanos() - window - offset, Kernel::SimulationRuntime::nanos() - offset, ImGuiCond_Always);
          ImPlot::SetupAxisLimits(ImAxis_Y1,0,1.4);
          ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
          ImPlot::PlotLine(active_label, &sdata.Data[0].x, &sdata.Data[0].y, sdata.Data.size(), 0, sdata.Offset, sizeof(ImPlotPoint));
          ImPlot::EndPlot();
        }
      }

      IGFD::FileDialogConfig config { "." };
      config.flags |= ImGuiFileDialogFlags_Modal;
      static bool export_single_pin = false;
      if (ImGui::Button("Export selected pin to file")) {
        export_single_pin = true;

        ImGuiFileDialog::Instance()->OpenDialog("PulseExportDlgKey", "Choose File", "Value Change Dump (*.vcd){.vcd},.*", config);
      }

      if (ImGui::Button("Export pins matching regex to file")) {
        export_single_pin = false;
        ImGuiFileDialog::Instance()->OpenDialog("PulseExportDlgKey", "Choose File", "Value Change Dump (*.vcd){.vcd},.*", config);
      }

      static char export_regex[128] = "";
      ImGui::SameLine();
      ImGui::InputText("Pin regex", export_regex, sizeof(export_regex));

      if (ImGuiFileDialog::Instance()->Display("PulseExportDlgKey", ImGuiWindowFlags_NoDocking))  {
        try {
          if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string image_filename = ImGuiFileDialog::Instance()->GetFilePathName();

            using namespace vcd;

            HeadPtr head = makeVCDHeader(static_cast<TimeScale>(50), TimeScaleUnit::ns, utils::now());
            VCDWriter writer {image_filename, head};

            if (export_single_pin) {
              std::string pin_name(active_label);
              auto scope = pin_name.substr(0, pin_name.find_first_of('_'));
              auto var   = writer.register_var(scope, pin_name, VariableType::wire, 1);

              if (Gpio::pin_map[monitor_pin].event_log.size() && pin_array[monitor_pin].is_digital) {
                for (const auto& value : Gpio::pin_map[monitor_pin].event_log) {
                  writer.change(var, value.timestamp / 50, utils::format("%u", value.value));
                }
              }
            } else {
              std::map<size_t, VarPtr> pin_to_var_map;
              std::regex expression(export_regex);

              for (auto pin : pin_array) {
                std::string pin_name(pin.name);
                bool regex_match = strlen(export_regex) == 0 || std::regex_search(pin_name, expression);
                auto scope       = pin_name.substr(0, pin_name.find_first_of('_'));
                if (pin.is_digital && regex_match) pin_to_var_map[pin.pin] = writer.register_var(scope, pin_name, VariableType::wire, 1);
              }

              std::multimap<uint64_t, std::pair<pin_t, uint16_t>> timestamp_pin_change_map;

              for (auto pin : pin_array) {
                if (pin.is_digital && pin_to_var_map.find(pin.pin) != pin_to_var_map.end())
                  for (const auto& data : Gpio::pin_map[pin.pin].event_log) {
                    timestamp_pin_change_map.emplace(std::make_pair(data.timestamp, std::make_pair(pin.pin, data.value)));
                  }
              }

              auto timestamp_offset = timestamp_pin_change_map.begin()->first;
              for (const auto& timestamp : timestamp_pin_change_map) {
                writer.change(pin_to_var_map[timestamp.second.first], (timestamp.first - timestamp_offset) / 50, utils::format("%u", timestamp.second.second));
              }
            }
          }
        } catch (const std::exception& e) {
          logger::error("Error exporting VCD file: %s", e.what());
        }
        ImGuiFileDialog::Instance()->Close();
      }
    }
  });

  user_interface.post_init = [&](){
    user_interface.ui_elements["Serial Monitor(0)"]->select();
    user_interface.ui_elements["Components"]->select();
  };

}

Application::~Application() {

}

void Application::update() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);

    switch (event.type) {
      case SDL_DROPFILE: {
        char *dropped_filedir = event.drop.file;
        //todo: stream file to the selected serial port?
        //input_file.open(dropped_filedir);
        SDL_free(dropped_filedir);    // Free dropped_filedir memory
      } break;

      case SDL_WINDOWEVENT:
        if (event.window.event != SDL_WINDOWEVENT_CLOSE || event.window.windowID != SDL_GetWindowID((SDL_Window*)window.getHandle()))
          break;

      case SDL_QUIT: active = false; break;
    }
  }

  sim.update();
  user_interface.show();
}

void Application::render() {
  sim.vis.framebuffer->bind();

  renderer::gl_assert_call(glClearColor, clear_color.x, clear_color.y, clear_color.z, 1.0);
  renderer::gl_assert_call(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  sim.vis.update();               // Update and render
  sim.vis.framebuffer->render();  // Render and unbind framebuffer

  user_interface.render();
  window.swap_buffers();
}
