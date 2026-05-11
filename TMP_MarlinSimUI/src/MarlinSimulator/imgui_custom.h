#pragma once

#include <imgui.h>
#include <imgui_internal.h>

namespace imgui_custom {
  constexpr uint32_t hfeat = 1 + 7 + 7 + 1, vfeat = 1 + 18 + 1 + 7 + 7 + 1;
  typedef struct { uint32_t minw, minh; float ratio; } constraint_t;
  struct CustomConstraints {
    static void AspectRatio(ImGuiSizeCallbackData *data) {
      constraint_t c = *(constraint_t*)data->UserData;
      uint32_t x = std::max((uint32_t)data->DesiredSize.x, c.minw),
               y = std::max((uint32_t)data->DesiredSize.y, c.minh);
      // For Portrait, Y takes precedence, else X takes precedence
      if (c.ratio < 1.0f)
        x = std::floor((y - vfeat) * c.ratio) + hfeat;
      else
        y = std::floor((x - hfeat) / c.ratio) + vfeat;
      data->DesiredSize.x = x;
      data->DesiredSize.y = y;
    }
  };

  ImVec2 scale_proportionally(ImVec2 size, double width, double height, bool integer_scaling) {
    double scalex = size.x / width, scaley = size.y / height;
    if (scalex < scaley) {
      if (integer_scaling) {
        if (scalex > 1.0) scalex = std::floor(scalex);
        size.x = width * scalex;
      }
      size.y = height * scalex;
    } else {
      if (integer_scaling) {
        if (scaley > 1.0) scaley = std::floor(scaley);
        size.y = height * scaley;
      }
      size.x = width * scaley;
    }
    return size;
  }
}
