#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <map>

#include "../logger.h"
#include "../utility.h"

#include "../resources/resources.h"
#include "gl.h"

#include <debugbreak.h>

namespace renderer {

using mesh_id_t = size_t;
class Mesh;
extern mesh_id_t next_mesh_index;
extern std::map<mesh_id_t, std::shared_ptr<Mesh>> global_mesh_list;
extern std::mutex global_mesh_list_mutex;
inline constexpr size_t MAX_BUFFER_SIZE = 100000;

void gl_defer_call(std::function<void(void)> fn);
void render(glm::mat4 global_transform);
void destroy();

struct vertex_data_t {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;

  static constexpr std::size_t elements = 3;
  static constexpr std::array<data_descriptor_element_t, vertex_data_t::elements> descriptor {
      data_descriptor_element_t::build<decltype(position)>(),
      data_descriptor_element_t::build<decltype(normal)>(),
      data_descriptor_element_t::build<decltype(color)>(),
  };
};

struct shader_attr_t {
  uint32_t location;
  std::string name;
  uint32_t gl_type;
  int32_t size;
  bool active = true;
};

class ShaderProgram {
public:
  ShaderProgram(GLuint program_id) : m_program_id {program_id} { }

  ~ShaderProgram() {
    auto id = m_program_id;
    gl_defer_call([id]() { renderer::gl_assert_call(glDeleteShader, id); });
  }

  static GLuint load_program(char const* vertex_string, char const* fragment_string, char const* geometry_string = nullptr) {
    GLuint vertex_shader = 0, fragment_shader = 0, geometry_shader = 0;
    if (vertex_string != nullptr) {
      vertex_shader = load_shader(GL_VERTEX_SHADER, vertex_string);
    }
    if (fragment_string != nullptr) {
      fragment_shader = load_shader(GL_FRAGMENT_SHADER, fragment_string);
    }
    if (geometry_string != nullptr) {
      geometry_shader = load_shader(GL_GEOMETRY_SHADER, geometry_string);
    }

    GLuint shader_program = renderer::gl_assert_call(glCreateProgram);
    renderer::gl_assert_call(glAttachShader, shader_program, vertex_shader);
    renderer::gl_assert_call(glAttachShader, shader_program, fragment_shader);
    if (geometry_shader) {
      renderer::gl_assert_call(glAttachShader, shader_program, geometry_shader);
    }

    renderer::gl_assert_call(glLinkProgram, shader_program);

    GLint result = 0;
    renderer::gl_assert_call(glGetProgramiv, shader_program, GL_LINK_STATUS, &result);
    if (result == GL_TRUE) {
      renderer::gl_assert_call(glUseProgram, shader_program);
      logger::debug("ShaderProgram::load_program: shader program(%d) linked and activated", shader_program);

    } else {
      renderer::gl_assert_call(glDeleteProgram, shader_program);
      shader_program = 0;
      logger::error("ShaderProgram::load_program: unable to link shaders");
    }

    if (vertex_shader) renderer::gl_assert_call(glDeleteShader, vertex_shader);
    if (fragment_shader) renderer::gl_assert_call(glDeleteShader, fragment_shader);
    if (geometry_shader) renderer::gl_assert_call(glDeleteShader, geometry_shader);

    return shader_program;
  }

  static GLuint load_shader(GLuint shader_type, char const* shader_string) {
    GLuint shader_id = renderer::gl_assert_call(glCreateShader, shader_type);
    int length       = strlen(shader_string);
    renderer::gl_assert_call(glShaderSource, shader_id, 1, (GLchar const**)&shader_string, &length);
    renderer::gl_assert_call(glCompileShader, shader_id);

    GLint status;
    renderer::gl_assert_call(glGetShaderiv, shader_id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      GLint maxLength = 0;
      renderer::gl_assert_call(glGetShaderiv, shader_id, GL_INFO_LOG_LENGTH, &maxLength);
      std::vector<GLchar> errorLog(maxLength + 1);
      errorLog[maxLength] = 0;
      renderer::gl_assert_call(glGetShaderInfoLog, shader_id, maxLength, &maxLength, errorLog.data());
      logger::error("%s", errorLog.data());
      renderer::gl_assert_call(glDeleteShader, shader_id);
      return 0;
    }
    return shader_id;
  }

  static std::shared_ptr<ShaderProgram> create(
      const std::filesystem::path vertex_path, const std::filesystem::path fragment_path, const std::filesystem::path geometry_path = {}
  ) {
    auto program_id = load_program(
        resource::ResourceManager::get_as_cstr(vertex_path),
        resource::ResourceManager::get_as_cstr(fragment_path),
        resource::ResourceManager::get_as_cstr(geometry_path)
    );
    if (program_id == 0) return nullptr; // default shader?
    auto program = std::make_shared<ShaderProgram>(program_id);
    if (program) program->build_cache();
    program->m_vertex_path   = vertex_path;
    program->m_fragment_path = fragment_path;
    program->m_geometry_path = geometry_path;
    return program;
  }

  bool reload() {
    auto program_id = load_program(
        resource::ResourceManager::get_as_cstr(m_vertex_path),
        resource::ResourceManager::get_as_cstr(m_fragment_path),
        resource::ResourceManager::get_as_cstr(m_geometry_path)
    );
    if (program_id == 0) return false;
    m_program_id = program_id;
    build_cache();
    return true;
  }

  void build_cache() {
    logger::debug("ShaderProgram::build_cache");
    GLint size {};  // size of the variable
    GLenum type {}; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 64; // maximum name length //GL_ACTIVE_UNIFORM_MAX_LENGTH
    GLchar name[bufSize];       // variable name in GLSL
    GLsizei length;             // name length

    GLint attribute_count;
    for (auto& attr : m_attributes) {
      attr.second.active = false; // disable all current attributes for reload
    }
    renderer::gl_assert_call(glGetProgramiv, m_program_id, GL_ACTIVE_ATTRIBUTES, &attribute_count);
    logger::debug("attribute:");
    for (GLint i = 0; i < attribute_count; i++) {
      renderer::gl_assert_call(glGetActiveAttrib, m_program_id, (GLuint)i, bufSize, &length, &size, &type, (char*)name);

      auto attrib_loc = renderer::gl_assert_call(glGetAttribLocation, m_program_id, name);

      logger::debug("index: %d, location: %d, name: %s, type: %d, size: %d", i, attrib_loc, name, type, size);
      m_attributes[name] = shader_attr_t {(uint32_t)attrib_loc, name, type, size};
    }

    GLint uniform_count;
    for (auto& uniform : m_uniforms) {
      uniform.second.active = false; // disable all current uniforms for reload
    }
    renderer::gl_assert_call(glGetProgramiv, m_program_id, GL_ACTIVE_UNIFORMS, &uniform_count);
    logger::debug("uniform:");
    for (GLint i = 0; i < uniform_count; i++) {
      renderer::gl_assert_call(glGetActiveUniform, m_program_id, (GLuint)i, bufSize, &length, &size, &type, (char*)name);

      auto uniform_loc = renderer::gl_assert_call(glGetUniformLocation, m_program_id, name);

      logger::debug("index: %d, location: %d, name: %s, type: %d, size: %d", i, uniform_loc, name, type, size);
      m_uniforms[name] = shader_attr_t {(uint32_t)uniform_loc, name, type, size};
    }
  }

  GLuint m_program_id {};
  std::map<std::string, shader_attr_t> m_attributes {};
  std::map<std::string, shader_attr_t> m_uniforms {};
  std::filesystem::path m_vertex_path;
  std::filesystem::path m_fragment_path;
  std::filesystem::path m_geometry_path;
};

class ShaderProgramInstance {
public:
  ShaderProgramInstance(std::shared_ptr<ShaderProgram> program) : m_program {program} { }

  struct shader_uniform_t {
    shader_attr_t desc;
    gl_uniform_t uniform_data;
    std::function<void(shader_attr_t& uniform_desc, gl_uniform_t& uniform_data)> gl_uniform_call;
  };

  void bind_immediate() {
    if (m_program == nullptr) return;
    renderer::gl_assert_call(glUseProgram, m_program->m_program_id);
    for (auto& [location, uniform] : m_uniforms) {
      uniform.gl_uniform_call(uniform.desc, uniform.uniform_data);
    }
  }

  template<typename T> void set_uniform(std::string name, T* value) {
    using gl_uniform_type = gl_uniform<type_to_gl_enum<T>::value>;
    if (m_program == nullptr) return;
    if (!m_program->m_uniforms.count(name)) return;
    auto uniform                 = m_program->m_uniforms[name];
    m_uniforms[uniform.location] = shader_uniform_t {
        uniform,
        gl_uniform_type {value},
        [](shader_attr_t& uniform_desc, gl_uniform_t& uniform_data) {
          if constexpr (gl_uniform_type::is_matrix) {
            gl_uniform_type().invoke(uniform_desc.location, uniform_desc.size, GL_FALSE, glm::value_ptr(*std::get<gl_uniform_type>(uniform_data).value));
          } else if constexpr (std::is_same_v<T, unsigned int> || std::is_same_v<T, int> || std::is_same_v<T, bool> || std::is_same_v<T, float>) {
            gl_uniform_type().invoke(uniform_desc.location, uniform_desc.size, &(*std::get<gl_uniform_type>(uniform_data).value));
          } else {
            gl_uniform_type().invoke(uniform_desc.location, uniform_desc.size, glm::value_ptr(*std::get<gl_uniform_type>(uniform_data).value));
          }
        },
    };
    return;
  }

  std::shared_ptr<ShaderProgram> m_program {};
  std::map<uint32_t, shader_uniform_t> m_uniforms;
};

class BufferBase {
public:
  virtual ~BufferBase() { }

  virtual void destroy()  = 0;
  virtual void generate() = 0;
  virtual bool bind()     = 0;
  virtual void upload()   = 0;
  virtual void render()   = 0;

  GLuint m_vao                      = 0;
  GLuint m_vbo                      = 0;
  size_t m_geometry_offset          = 0;
  GLuint m_storage_hint             = GL_STATIC_DRAW;
  GeometryPrimitive m_geometry_type = GeometryPrimitive::TRIANGLES;
  bool m_dirty                      = true;
  bool m_generated                  = false;
};

template<typename ElementType> class Buffer : public BufferBase {
public:
  virtual void generate() override {
    renderer::gl_assert_call(glGenVertexArrays, 1, &m_vao);
    renderer::gl_assert_call(glGenBuffers, 1, &m_vbo);
    renderer::gl_assert_call(glBindVertexArray, m_vao);
    renderer::gl_assert_call(glBindBuffer, GL_ARRAY_BUFFER, m_vbo);

    size_t index = 0, offset = 0;
    for (auto& attrib : ElementType::descriptor) {
      renderer::gl_assert_call(glEnableVertexAttribArray, index);

      renderer::gl_assert_call(glVertexAttribPointer, index, attrib.elements, attrib.gl_enum, GL_FALSE, sizeof(ElementType), (void*)offset);

      ++index;
      offset += attrib.length;
    }
    m_generated = true;
  }

  virtual void destroy() override {
    GLuint vbo = m_vbo;
    GLuint vao = m_vao;
    if (m_vbo) gl_defer_call([vbo]() { renderer::gl_assert_call(glDeleteBuffers, 1, &vbo); });
    if (m_vao) gl_defer_call([vao]() { renderer::gl_assert_call(glDeleteBuffers, 1, &vao); });
    m_vbo = m_vao = 0;
  }

  ~Buffer() {
    destroy();
  }

  virtual bool bind() override {
    if (!m_generated) generate();
    if (m_vao == 0 || m_vbo == 0) return false;
    renderer::gl_assert_call(glBindVertexArray, m_vao);
    renderer::gl_assert_call(glBindBuffer, GL_ARRAY_BUFFER, m_vbo);
    return true;
  }

  virtual void upload() override {
    if (m_dirty && m_data.size() > 0) {
      renderer::gl_assert_call(glBufferData, GL_ARRAY_BUFFER, m_data.size() * sizeof(ElementType), &m_data[0], m_storage_hint);

      m_dirty = false;
    }
  }

  virtual void render() override {
    if (bind()) {
      upload();
      renderer::gl_assert_call(glDrawArrays, (GLenum)m_geometry_type, m_geometry_offset, m_data.size());
    }
  }

  static std::shared_ptr<Buffer<ElementType>> create() {
    return std::shared_ptr<Buffer<ElementType>>(new Buffer<ElementType>());
  }

  std::vector<ElementType>& data() {
    m_dirty = true;
    return m_data;
  }

  std::vector<ElementType> const& cdata() const {
    return m_data;
  }

  size_t size() {
    return m_data.size();
  }

  void add_vertex(ElementType elem) {
    m_dirty = true;
    m_data.push_back(elem);
  }

private:
  Buffer() { }

  std::vector<ElementType> m_data {};
};

class Mesh {
public:
  void render(glm::mat4 global_transform) {
    if ((!m_visible) || m_shader_instance == nullptr) return;
    if (m_transform_dirty) {
      build_transform();
      m_transform_dirty = false;
    }
    if (m_shader_dirty) {
      m_shader_instance->set_uniform("u_mvp", &m_view_projection_transform);
      m_shader_dirty = false;
    }
    m_view_projection_transform = global_transform * m_transform;
    m_shader_instance->bind_immediate();
    for (auto buffer : m_buffer) {
      buffer->render();
    }
  }

  void build_transform() {
    m_transform = glm::translate(glm::mat4(1.0), m_position);
    m_transform = m_transform * glm::mat4_cast(m_rotation);
    m_transform = glm::scale(m_transform, m_scale);
    m_transform = glm::translate(m_transform, m_origin);
  }

  static std::shared_ptr<Mesh> create(std::shared_ptr<BufferBase> buffer) {
    auto mesh = std::shared_ptr<Mesh>(new Mesh());
    mesh->m_buffer.push_back(buffer);
    return mesh;
  }

  static std::shared_ptr<Mesh> create() {
    return std::shared_ptr<Mesh>(new Mesh());
  }

  template<typename VertexType> std::shared_ptr<Buffer<VertexType>> buffer() {
    return m_buffer.size() ? std::reinterpret_pointer_cast<Buffer<VertexType>>(m_buffer.back()) : nullptr;
  }

  template<typename VertexType> std::vector<std::shared_ptr<Buffer<VertexType>>>& buffer_vector() {
    return *reinterpret_cast<std::vector<std::shared_ptr<Buffer<VertexType>>>*>(&m_buffer);
  }

  void set_shader_program(std::shared_ptr<ShaderProgram> program) {
    m_shader_instance = std::make_shared<ShaderProgramInstance>(program);
  }

  void set_shader_program(std::shared_ptr<ShaderProgramInstance> program_instance) {
    m_shader_instance = program_instance;
  }

  glm::mat4 m_view_projection_transform {1.0};
  glm::mat4 m_transform {1.0};
  glm::vec3 m_origin {0.0, 0.0, 0.0};
  glm::vec3 m_position {0.0, 0.0, 0.0};
  glm::vec3 m_scale {1.0, 1.0, 1.0};
  glm::quat m_rotation {};
  bool m_visible         = true;
  bool m_transform_dirty = true;
  bool m_shader_dirty    = true;
  bool m_delete          = false;
  std::vector<std::shared_ptr<BufferBase>> m_buffer {};
  std::shared_ptr<ShaderProgramInstance> m_shader_instance {};

private:
  Mesh() { }
};

template<typename... Args> mesh_id_t create_mesh(Args... args) {
  std::scoped_lock global_lock(global_mesh_list_mutex);
  global_mesh_list.emplace(next_mesh_index, Mesh::create(std::forward(args)...));
  return next_mesh_index++;
}

std::shared_ptr<Mesh> get_mesh_by_id(mesh_id_t resource_index);
void destroy_mesh(mesh_id_t resource_index);
void render_mesh(mesh_id_t resource_index);
void render_list_is_ready();
}
