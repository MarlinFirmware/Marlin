#include <mutex>
#include <fstream>
#include <cmath>
#include <random>
#include "Gpio.h"

#include <gl.h>

#include "XPT2046Device.h"

#include "../paths.h"
#include MARLIN_HAL_PATH(tft/xpt2046.h)
#if ENABLED(TOUCH_SCREEN)
  #include <src/lcd/tft/touch.h>
#else
  #define MINIMUM_HOLD_TIME 15
#endif

void XPT2046Device::onByteReceived(uint8_t _byte) {
  SPISlavePeripheral::onByteReceived(_byte);
  switch (_byte) {
    //TODO: touch hold
    case XPT2046_Z1:
      if (dirty) {
        setResponse16(XPT2046_Z1_THRESHOLD); // respond that we have data to send
      }
      else {
        setResponse16(0);
      }
      break;

    case XPT2046_X:
      setResponse16(lastClickX);
      break;

    case XPT2046_Y:
      setResponse16(lastClickY);
      break;

    default:
      break;
  }
}

void XPT2046Device::onEndTransaction() {
  SPISlavePeripheral::onEndTransaction();
  if (!dirty) return;
  auto now = Kernel::SimulationRuntime::millis();
  // for at least 3 touches
  if ((now - touch_time) > (MINIMUM_HOLD_TIME * 3)) {
    dirty = false;
  }
};

void XPT2046Device::ui_widget() {
  ImGui::Text("X: %d", lastClickX);
  ImGui::Text("Y: %d", lastClickY);
}

void XPT2046Device::ui_callback() {
  if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    dirty = true;
    touch_time = Kernel::SimulationRuntime::millis();
    auto display_min = glm::vec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y);
    auto display_max = glm::vec2(ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y) - display_min;
    auto click_pixel = glm::vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y) - display_min;
    auto click_ratio = (click_pixel / display_max);
    lastClickX = 1024 * click_ratio.x;
    lastClickY = 1024 * click_ratio.y;
    //printf("click x: %d, y: %d\n",  lastClickX, lastClickY);
  }
}
