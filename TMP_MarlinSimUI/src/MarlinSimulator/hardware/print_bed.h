#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <imgui.h>

#include "Gpio.h"
#include "print_bed.h"

#include "../virtual_printer.h"

class PrintBed : public VirtualPrinter::Component {
public:
  PrintBed(glm::vec2 size) : VirtualPrinter::Component("PrintBed"), bed_size(size), bed_plane_center({size.x/2, size.y/2, 0}), bed_level_points{glm::vec3{bed_size.x / 2, bed_size.y, 0}, {0, 0, 0}, {bed_size.x, 0, 0}} {}

  enum bed_shape_t {
    flat,
    bowl,
    chip,
    ripple,
    x_ripple,
    y_ripple,
    xy_ripple
  };

  void ui_widget() {
    int8_t updated = 0;
    updated |= ImGui::SliderFloat("bed level(back centre)", &bed_level_points[0].z, -5.0f, 5.0f);
    updated |= ImGui::SliderFloat("bed level(front left)", &bed_level_points[1].z, -5.0f, 5.0f);
    updated |= ImGui::SliderFloat("bed level(front right)", &bed_level_points[2].z, -5.0f, 5.0f);
    // Add a checkbox to choose whether to show a bed gradient
    updated |= ImGui::Checkbox("Z height gradient", &gradient_enabled);

    // Add combo box with the options "flat", "bowl" and "dome"
    static const char* items[] = { "flat", "parabolic", "chip", "ripple", "x ripple", "y ripple", "xy ripple" };
    static int item_current = 0;
    updated |= ImGui::Combo("Bed shape", &item_current, items, IM_ARRAYSIZE(items));
    bed_shape = static_cast<bed_shape_t>(item_current);

    updated |= ImGui::SliderFloat("amplitude", &bed_shape_amplitude, -5.0f, 5.0f);

    if (updated) {
      dirty = true;
      build_3point(bed_level_points[0], bed_level_points[1], bed_level_points[2]);
    }
  }

  void build_3point(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    bed_level_points[0] = p1;
    bed_level_points[1] = p2;
    bed_level_points[2] = p3;
    auto v1 = p2 - p1;
    auto v2 = p3 - p1;
    bed_plane_normal = glm::normalize(glm::cross(v1, v2));
    bed_plane_center.z = ((bed_plane_normal.x * (bed_plane_center.x - p1.x) + bed_plane_normal.y * (bed_plane_center.y - p1.y)) / (- bed_plane_normal.z)) + p1.z;
  }

  float calculate_z(glm::vec2 xy) {
    float planar_z = ((bed_plane_normal.x * (xy.x - bed_plane_center.x) + bed_plane_normal.y * (xy.y - bed_plane_center.y)) / (- bed_plane_normal.z)) + bed_plane_center.z;

    // Optionally offset the calculated "flat" z value based on the bed shape
    // Overlays are geometric patterns starting from the center of the bed, so
    // we need to translate the xy coordinates to be relative to the center
    glm::vec2 center_origin_xy = glm::vec2{xy.x - bed_size.x / 2, xy.y - bed_size.y / 2};

    float shape_overlay_z = 0.0f;
    switch (bed_shape) {
      case flat:
        // No change
        break;
      case bowl:
        shape_overlay_z = bed_shape_amplitude * glm::sin(glm::length(center_origin_xy) / glm::length(bed_size) * glm::pi<float>());
        break;
      case chip:
        shape_overlay_z = bed_shape_amplitude * (center_origin_xy.x / bed_size.x) * (center_origin_xy.y / bed_size.y);
        break;
      case ripple:
        shape_overlay_z = bed_shape_amplitude * glm::sin(glm::length(center_origin_xy) / bed_size.x * 3 * glm::pi<float>());
        break;
      case x_ripple:
        shape_overlay_z = bed_shape_amplitude * glm::sin(center_origin_xy.x / bed_size.x * 3 * glm::pi<float>());
        break;
      case y_ripple:
        shape_overlay_z = bed_shape_amplitude * glm::sin(center_origin_xy.y / bed_size.y * 3 * glm::pi<float>());
        break;
      case xy_ripple:
        shape_overlay_z = bed_shape_amplitude * glm::sin(center_origin_xy.x / bed_size.x * 3 * glm::pi<float>()) * glm::sin(center_origin_xy.y / bed_size.y * 3 * glm::pi<float>());
        break;
    }

    return planar_z + shape_overlay_z;
  }

  glm::vec2 bed_size{200, 200};
  glm::vec3 bed_plane_center{100, 100, 0};
  glm::vec3 bed_plane_normal{0, 0, 1};
  std::array<glm::vec3, 3> bed_level_points;
  bool gradient_enabled = false;
  bed_shape_t bed_shape = flat;
  float bed_shape_amplitude = 1.0f;
  bool dirty = true;
};
