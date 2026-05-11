#include <algorithm>
#include <imgui.h>

#include "hardware/Button.h"
#include "hardware/StepperDriver.h"
#include "hardware/EndStop.h"
#include "hardware/Heater.h"
#include "hardware/print_bed.h"
#include "hardware/bed_probe.h"
#include "hardware/ST7796Device.h"
#include "hardware/HD44780Device.h"
#include "hardware/ST7920Device.h"
#include "hardware/SDCard.h"
#include "hardware/W25QxxDevice.h"
#include "hardware/FilamentRunoutSensor.h"
#include "hardware/NeoPixelDevice.h"
#include "hardware/KinematicSystem.h"
#include "hardware/pwm_reader.h"
#include "hardware/BLTouch.h"
#include "hardware/Buzzer.h"

#include "virtual_printer.h"

#include <src/inc/MarlinConfig.h>

#if HAS_TFT_XPT2046 || HAS_TOUCH_XPT2046
  #include "paths.h"
  #include MARLIN_HAL_PATH(tft/xpt2046.h)
#endif

#ifndef SD_DETECT_STATE
  #define SD_DETECT_STATE HIGH
#endif

std::function<void(kinematic_state&)> VirtualPrinter::on_kinematic_update;
std::map<std::string, std::shared_ptr<VirtualPrinter::Component>> VirtualPrinter::component_map;
std::vector<std::shared_ptr<VirtualPrinter::Component>> VirtualPrinter::components;
std::shared_ptr<VirtualPrinter::Component> VirtualPrinter::root;

void VirtualPrinter::Component::ui_widgets() {
  ui_widget();
  for(auto const& it : children) {
    if (ImGui::CollapsingHeader(it->name.c_str())) {
      ImGui::Indent(6.0f);
      ImGui::PushID(it->name.c_str());
      it->ui_widgets();
      ImGui::PopID();
      ImGui::Unindent(6.0f);
    }
  }
}

std::map<uint64_t, uint64_t> servo_pin_lookup { {0, SERVO0_PIN}, {1, SERVO1_PIN}, {2, SERVO2_PIN}, {3, SERVO3_PIN}};

void VirtualPrinter::build() {
  root = add_component<Component>("root");

  #if ENABLED(DELTA)
    auto kinematics = root->add_component<DeltaKinematicSystem>("Delta Kinematic System", on_kinematic_update);
    root->add_component<EndStop>("Endstop(Tower A Max)", X_MAX_PIN, !X_MAX_ENDSTOP_HIT_STATE, [kinematics](){ return kinematics->state.effector_position[0].stepper_position.x >= DELTA_HEIGHT; });
    root->add_component<EndStop>("Endstop(Tower B Max)", Y_MAX_PIN, !Y_MAX_ENDSTOP_HIT_STATE, [kinematics](){ return kinematics->state.effector_position[0].stepper_position.y >= DELTA_HEIGHT; });
    root->add_component<EndStop>("Endstop(Tower C Max)", Z_MAX_PIN, !Z_MAX_ENDSTOP_HIT_STATE, [kinematics](){ return kinematics->state.effector_position[0].stepper_position.z >= DELTA_HEIGHT; });
  #else
    auto kinematics = root->add_component<CartesianKinematicSystem>("Cartesian Kinematic System", on_kinematic_update);
    root->add_component<EndStop>("Endstop(X Min)", X_MIN_PIN, !X_MIN_ENDSTOP_HIT_STATE, [kinematics](){ return kinematics->state.effector_position[0].position.x <= X_MIN_POS; });
    #ifdef DUAL_X_CARRIAGE
      root->add_component<EndStop>("Endstop(X2 Max)", X_MAX_PIN, !X_MAX_ENDSTOP_HIT_STATE, [kinematics](){ return kinematics->state.effector_position[1].position.x >= X2_MAX_POS; });
    #endif
    root->add_component<EndStop>("Endstop(Y Min)", Y_MIN_PIN, !Y_MIN_ENDSTOP_HIT_STATE, [kinematics](){ return kinematics->state.effector_position[0].position.y <= Y_MIN_POS; });
    root->add_component<EndStop>("Endstop(Z Min)", Z_MIN_PIN, !Z_MIN_ENDSTOP_HIT_STATE, [kinematics](){ return kinematics->state.effector_position[0].position.z <= Z_MIN_POS; });
  #endif

  auto print_bed = root->add_component<PrintBed>("Print Bed", glm::vec2{X_BED_SIZE, Y_BED_SIZE});

  #if HAS_BED_PROBE
    #if ENABLED(BLTOUCH)
      root->add_component<BLTouchProbe>("BLTouch", servo_pin_lookup[Z_PROBE_SERVO_NR],
    #else
      root->add_component<BedProbe>("Probe",
    #endif
    #ifdef Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
      Z_MIN_PIN
    #else
      Z_MIN_PROBE_PIN
    #endif
    , glm::vec3 NOZZLE_TO_PROBE_OFFSET, kinematics->state.effector_position[0].position, *print_bed);
  #endif

  #if HOTENDS
    root->add_component<Heater>("Hotend0 Heater", HEATER_0_PIN, TEMP_0_PIN, heater_data{12, 3.6}, hotend_data{13, 20, 0.897}, adc_data{4700, 12});
    #if HOTENDS > 1
      root->add_component<Heater>("Hotend1 Heater", HEATER_1_PIN, TEMP_1_PIN, heater_data{12, 3.6}, hotend_data{13, 20, 0.897}, adc_data{4700, 12});
    #endif
    #if HOTENDS > 2
      root->add_component<Heater>("Hotend2 Heater", HEATER_2_PIN, TEMP_2_PIN, heater_data{12, 3.6}, hotend_data{13, 20, 0.897}, adc_data{4700, 12});
    #endif
    #if HOTENDS > 3
      root->add_component<Heater>("Hotend3 Heater", HEATER_3_PIN, TEMP_3_PIN, heater_data{12, 3.6}, hotend_data{13, 20, 0.897}, adc_data{4700, 12});
    #endif
    #if HOTENDS > 4
      root->add_component<Heater>("Hotend4 Heater", HEATER_4_PIN, TEMP_4_PIN, heater_data{12, 3.6}, hotend_data{13, 20, 0.897}, adc_data{4700, 12});
    #endif
    #if HOTENDS > 5
      root->add_component<Heater>("Hotend5 Heater", HEATER_5_PIN, TEMP_5_PIN, heater_data{12, 3.6}, hotend_data{13, 20, 0.897}, adc_data{4700, 12});
    #endif
    #if HOTENDS > 6
      root->add_component<Heater>("Hotend6 Heater", HEATER_6_PIN, TEMP_6_PIN, heater_data{12, 3.6}, hotend_data{13, 20, 0.897}, adc_data{4700, 12});
    #endif
    #if HOTENDS > 7
      root->add_component<Heater>("Hotend7 Heater", HEATER_7_PIN, TEMP_7_PIN, heater_data{12, 3.6}, hotend_data{13, 20, 0.897}, adc_data{4700, 12});
    #endif
  #endif

  #if TEMP_SENSOR_BED
    root->add_component<Heater>("Bed Heater", HEATER_BED_PIN, TEMP_BED_PIN, heater_data{12, 1.2}, hotend_data{325, 824, 0.897}, adc_data{4700, 12});
    #if HAS_BED_ZONES && BED_ZONES_COUNT > 1
    {
      constexpr pin_t zheater_pins[] = BED_ZONE_HEATER_PINS;
      constexpr pin_t zsensor_pins[] = BED_ZONE_SENSOR_PINS;
      for (int z = 1; z < BED_ZONES_COUNT; z++) {
        std::string zname = std::string("Bed Zone ") + std::to_string(z) + " Heater";
        root->add_component<Heater>(zname, zheater_pins[z], zsensor_pins[z],
          heater_data{12, 1.2}, hotend_data{325, 824, 0.897}, adc_data{4700, 12});
      }
    }
    #endif
  #endif

  #if TEMP_SENSOR_CHAMBER
    root->add_component<Heater>("Chamber Heater", HEATER_CHAMBER_PIN, TEMP_CHAMBER_PIN, heater_data{12, 1.2}, hotend_data{325, 824, 0.897}, adc_data{4700, 12});
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    root->add_component<FilamentRunoutSensor>("Filament Runout Sensor 1", FIL_RUNOUT1_PIN, FIL_RUNOUT1_STATE);
    #if NUM_RUNOUT_SENSORS > 1
      root->add_component<FilamentRunoutSensor>("Filament Runout Sensor 2", FIL_RUNOUT2_PIN, FIL_RUNOUT2_STATE);
    #endif
    #if NUM_RUNOUT_SENSORS > 2
      root->add_component<FilamentRunoutSensor>("Filament Runout Sensor 3", FIL_RUNOUT3_PIN, FIL_RUNOUT3_STATE);
    #endif
    #if NUM_RUNOUT_SENSORS > 3
      root->add_component<FilamentRunoutSensor>("Filament Runout Sensor 4", FIL_RUNOUT4_PIN, FIL_RUNOUT4_STATE);
    #endif
    #if NUM_RUNOUT_SENSORS > 4
      root->add_component<FilamentRunoutSensor>("Filament Runout Sensor 5", FIL_RUNOUT5_PIN, FIL_RUNOUT5_STATE);
    #endif
    #if NUM_RUNOUT_SENSORS > 5
      root->add_component<FilamentRunoutSensor>("Filament Runout Sensor 6", FIL_RUNOUT6_PIN, FIL_RUNOUT6_STATE);
    #endif
    #if NUM_RUNOUT_SENSORS > 6
      root->add_component<FilamentRunoutSensor>("Filament Runout Sensor 7", FIL_RUNOUT7_PIN, FIL_RUNOUT7_STATE);
    #endif
    #if NUM_RUNOUT_SENSORS > 7
      root->add_component<FilamentRunoutSensor>("Filament Runout Sensor 8", FIL_RUNOUT8_PIN, FIL_RUNOUT8_STATE);
    #endif
  #endif

  #ifdef FAN0_PIN
    root->add_component<PWMReader>("Fan0", FAN0_PIN);
  #endif

  #ifdef Z_PROBE_SERVO_NR
    #define SERVO_TEST(N) Z_PROBE_SERVO_NR != N
  #else
    #define SERVO_TEST(N) 1
  #endif
  #if NUM_SERVOS > 0 && SERVO_TEST(0)
    root->add_component<PWMReader>("Servo0", SERVO0_PIN);
  #endif
  #if NUM_SERVOS > 1 && SERVO_TEST(1)
    root->add_component<PWMReader>("Servo1", SERVO1_PIN);
  #endif
  #if NUM_SERVOS > 2 && SERVO_TEST(2)
    root->add_component<PWMReader>("Servo2", SERVO2_PIN);
  #endif
  #if NUM_SERVOS > 3 && SERVO_TEST(3)
    root->add_component<PWMReader>("Servo3", SERVO3_PIN);
  #endif

  #if ENABLED(SPI_FLASH)
    root->add_component<W25QxxDevice>("SPI Flash", spi_bus_by_pins<SPI_FLASH_SCK_PIN, SPI_FLASH_MOSI_PIN, SPI_FLASH_MISO_PIN>(), SPI_FLASH_CS_PIN, SPI_FLASH_SIZE);
  #endif

  #ifdef SDSUPPORT
    root->add_component<SDCard>("SD Card", spi_bus_by_pins<SD_SCK_PIN, SD_MOSI_PIN, SD_MISO_PIN>(), SD_SS_PIN, SD_DETECT_PIN, SD_DETECT_STATE);
  #endif

  #ifndef LCD_PINS_EN
    #define LCD_PINS_EN LCD_PINS_ENABLE
  #endif

  #if ENABLED(TFT_INTERFACE_SPI)
    root->add_component<ST7796Device>("ST7796Device Display", spi_bus_by_pins<TFT_SCK_PIN, TFT_MOSI_PIN, TFT_MISO_PIN>(), TFT_CS_PIN, spi_bus_by_pins<TOUCH_SCK_PIN, TOUCH_MOSI_PIN, TOUCH_MISO_PIN>(), TOUCH_CS_PIN, TFT_DC_PIN, BEEPER_PIN, BTN_EN1, BTN_EN2, BTN_ENC, BTN_BACK, KILL_PIN);
  #elif defined(HAS_MARLINUI_HD44780)
    root->add_component<HD44780Device>("HD44780Device Display", LCD_PINS_RS, LCD_PINS_EN, LCD_PINS_D4, LCD_PINS_D5, LCD_PINS_D6, LCD_PINS_D7, BEEPER_PIN, BTN_EN1, BTN_EN2, BTN_ENC, BTN_BACK, KILL_PIN);
  #elif defined(HAS_MARLINUI_U8GLIB)
    root->add_component<ST7920Device>("ST7920Device Display", LCD_PINS_D4, LCD_PINS_EN, LCD_PINS_RS, BEEPER_PIN, BTN_EN1, BTN_EN2, BTN_ENC, BTN_BACK, KILL_PIN);
  #endif

  #if HAS_KILL
    root->add_component<Button>("Kill Button", KILL_PIN, false);
  #endif

  #ifdef NEOPIXEL_LED
    root->add_component<NeoPixelDevice>("NeoPixelDevice", NEOPIXEL_PIN, NEOPIXEL_TYPE, NEOPIXEL_PIXELS);
  #endif

  #ifdef BEEPER_PIN
    #ifdef SPEAKER
    #else
      root->add_component<Buzzer>("Buzzer", BEEPER_PIN, false);
    #endif
  #endif

  for(auto const& component : components) component->ui_init();

  kinematics->kinematic_update();
}

void VirtualPrinter::ui_widgets() {
  if (root) root->ui_widgets();
}
