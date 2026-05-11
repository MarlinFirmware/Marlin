#include "renderer.h"

namespace renderer {

mesh_id_t next_mesh_index = 1;
std::map<mesh_id_t, std::shared_ptr<Mesh>> global_mesh_list {};
std::vector<std::function<void(void)>> deferred_call_list {};

std::vector<std::shared_ptr<Mesh>> render_list {};
std::vector<std::shared_ptr<Mesh>> render_list_next {};
std::vector<std::shared_ptr<Mesh>>* active_render_list {&render_list};
std::vector<std::shared_ptr<Mesh>>* back_render_list {&render_list_next};
bool render_list_ready = false;

std::mutex deferred_call_mutex {};
std::mutex global_mesh_list_mutex {};

void gl_defer_call(std::function<void(void)> fn) {
  std::scoped_lock lock(deferred_call_mutex);
  deferred_call_list.push_back(fn);
}

void execute_deferred_calls() {
  std::scoped_lock lock(deferred_call_mutex);
  if (deferred_call_list.size()) {
    for (auto& fn : deferred_call_list) {
      fn();
    }
    deferred_call_list.clear();
  }
}

std::shared_ptr<Mesh> get_mesh_by_id(mesh_id_t resource_index) {
  std::scoped_lock global_lock(global_mesh_list_mutex);
  if (!global_mesh_list.count(resource_index)) return nullptr;
  return global_mesh_list[resource_index];
}

void render_mesh(mesh_id_t resource_index) {
  if (global_mesh_list.count(resource_index)) {
    back_render_list->push_back(global_mesh_list[resource_index]);
  }
}

void destroy_mesh(mesh_id_t resource_index) {
  std::scoped_lock global_lock(global_mesh_list_mutex);
  global_mesh_list.erase(resource_index);
}

void render_list_is_ready() {
  render_list_ready = true;
}

bool is_render_list_ready() {
  return render_list_ready;
}

void render_list_swap() {
  if (!is_render_list_ready()) return;
  if (active_render_list == &render_list_next) {
    active_render_list = &render_list;
    back_render_list   = &render_list_next;
  } else {
    active_render_list = &render_list_next;
    back_render_list   = &render_list;
  }
  back_render_list->clear();
  render_list_ready = false;
}

void destroy() {
  global_mesh_list.clear();
  execute_deferred_calls();
}

void render(glm::mat4 global_transform) {
  if (active_render_list != nullptr) {
    for (auto& mesh : *active_render_list) {
      mesh->render(global_transform);
    }
  }

  render_list_swap();
  execute_deferred_calls();
}

}
