#pragma once

#include <variant>

#include <gl.h>
#include <glm/glm.hpp>

#include <debugbreak.h>

#ifndef GL_LOG_LEVEL
  #define GL_LOG_LEVEL 0
#endif

namespace renderer {

template<uint32_t V> struct gl_enum_to_type;
template<> struct gl_enum_to_type<GL_FLOAT> { using value_type = float; };
template<> struct gl_enum_to_type<GL_FLOAT_VEC2> { using value_type = glm::vec2; };
template<> struct gl_enum_to_type<GL_FLOAT_VEC3> { using value_type = glm::vec3; };
template<> struct gl_enum_to_type<GL_FLOAT_VEC4> { using value_type = glm::vec4; };
template<> struct gl_enum_to_type<GL_DOUBLE> { using value_type = double; };
template<> struct gl_enum_to_type<GL_INT> { using value_type = int; };
template<> struct gl_enum_to_type<GL_INT_VEC2> { using value_type = glm::ivec2; };
template<> struct gl_enum_to_type<GL_INT_VEC3> { using value_type = glm::ivec3; };
template<> struct gl_enum_to_type<GL_INT_VEC4> { using value_type = glm::ivec4; };
template<> struct gl_enum_to_type<GL_UNSIGNED_INT> { using value_type = unsigned int; };
template<> struct gl_enum_to_type<GL_UNSIGNED_INT_VEC2> { using value_type = glm::uvec2; };
template<> struct gl_enum_to_type<GL_UNSIGNED_INT_VEC3> { using value_type = glm::uvec3; };
template<> struct gl_enum_to_type<GL_UNSIGNED_INT_VEC4> { using value_type = glm::uvec4; };
template<> struct gl_enum_to_type<GL_BOOL> { using value_type = bool; };
template<> struct gl_enum_to_type<GL_BOOL_VEC2> { using value_type = glm::bvec2; };
template<> struct gl_enum_to_type<GL_BOOL_VEC3> { using value_type = glm::bvec3; };
template<> struct gl_enum_to_type<GL_BOOL_VEC4> { using value_type = glm::bvec4; };
template<> struct gl_enum_to_type<GL_FLOAT_MAT2> { using value_type = glm::mat2; };
template<> struct gl_enum_to_type<GL_FLOAT_MAT3> { using value_type = glm::mat3; };
template<> struct gl_enum_to_type<GL_FLOAT_MAT4> { using value_type = glm::mat4; };
template<> struct gl_enum_to_type<GL_FLOAT_MAT2x3> { using value_type = glm::mat2x3; };
template<> struct gl_enum_to_type<GL_FLOAT_MAT2x4> { using value_type = glm::mat2x4; };
template<> struct gl_enum_to_type<GL_FLOAT_MAT3x2> { using value_type = glm::mat3x2; };
template<> struct gl_enum_to_type<GL_FLOAT_MAT3x4> { using value_type = glm::mat3x4; };
template<> struct gl_enum_to_type<GL_FLOAT_MAT4x2> { using value_type = glm::mat4x2; };
template<> struct gl_enum_to_type<GL_FLOAT_MAT4x3> { using value_type = glm::mat4x3; };

template<typename T> struct type_to_gl_enum;
template<> struct type_to_gl_enum<float> { static constexpr uint32_t value = GL_FLOAT; };
template<> struct type_to_gl_enum<glm::vec2> { static constexpr uint32_t value = GL_FLOAT_VEC2; };
template<> struct type_to_gl_enum<glm::vec3> { static constexpr uint32_t value = GL_FLOAT_VEC3; };
template<> struct type_to_gl_enum<glm::vec4> { static constexpr uint32_t value = GL_FLOAT_VEC4; };
template<> struct type_to_gl_enum<double> { static constexpr uint32_t value = GL_DOUBLE; };
template<> struct type_to_gl_enum<int32_t> { static constexpr uint32_t value = GL_INT; };
template<> struct type_to_gl_enum<glm::ivec2> { static constexpr uint32_t value = GL_INT_VEC2; };
template<> struct type_to_gl_enum<glm::ivec3> { static constexpr uint32_t value = GL_INT_VEC3; };
template<> struct type_to_gl_enum<glm::ivec4> { static constexpr uint32_t value = GL_INT_VEC4; };
template<> struct type_to_gl_enum<uint32_t> { static constexpr uint32_t value = GL_UNSIGNED_INT; };
template<> struct type_to_gl_enum<glm::uvec2> { static constexpr uint32_t value = GL_UNSIGNED_INT_VEC2; };
template<> struct type_to_gl_enum<glm::uvec3> { static constexpr uint32_t value = GL_UNSIGNED_INT_VEC3; };
template<> struct type_to_gl_enum<glm::uvec4> { static constexpr uint32_t value = GL_UNSIGNED_INT_VEC4; };
template<> struct type_to_gl_enum<bool> { static constexpr uint32_t value = GL_BOOL; };
template<> struct type_to_gl_enum<glm::bvec2> { static constexpr uint32_t value = GL_BOOL_VEC2; };
template<> struct type_to_gl_enum<glm::bvec3> { static constexpr uint32_t value = GL_BOOL_VEC3; };
template<> struct type_to_gl_enum<glm::bvec4> { static constexpr uint32_t value = GL_BOOL_VEC4; };
template<> struct type_to_gl_enum<glm::mat2> { static constexpr uint32_t value = GL_FLOAT_MAT2; };
template<> struct type_to_gl_enum<glm::mat3> { static constexpr uint32_t value = GL_FLOAT_MAT3; };
template<> struct type_to_gl_enum<glm::mat4> { static constexpr uint32_t value = GL_FLOAT_MAT4; };
template<> struct type_to_gl_enum<glm::mat2x3> { static constexpr uint32_t value = GL_FLOAT_MAT2x3; };
template<> struct type_to_gl_enum<glm::mat2x4> { static constexpr uint32_t value = GL_FLOAT_MAT2x4; };
template<> struct type_to_gl_enum<glm::mat3x2> { static constexpr uint32_t value = GL_FLOAT_MAT3x2; };
template<> struct type_to_gl_enum<glm::mat3x4> { static constexpr uint32_t value = GL_FLOAT_MAT3x4; };
template<> struct type_to_gl_enum<glm::mat4x2> { static constexpr uint32_t value = GL_FLOAT_MAT4x2; };
template<> struct type_to_gl_enum<glm::mat4x3> { static constexpr uint32_t value = GL_FLOAT_MAT4x3; };
template<> struct type_to_gl_enum<uint8_t>{ static constexpr uint32_t value = GL_UNSIGNED_BYTE; };
template<> struct type_to_gl_enum<int16_t> { static constexpr uint32_t value = GL_SHORT; };
template<> struct type_to_gl_enum<uint16_t>{ static constexpr uint32_t value = GL_UNSIGNED_SHORT; };
template<> struct type_to_gl_enum<int8_t>{ static constexpr uint32_t value = GL_BYTE; };

template<uint32_t V> struct gl_uniform;
template<> struct gl_uniform<GL_FLOAT> { using value_type = decltype(glUniform1fv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_FLOAT>::value_type *value; value_type invoke = glUniform1fv; };
template<> struct gl_uniform<GL_FLOAT_VEC2> { using value_type = decltype(glUniform2fv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_FLOAT_VEC2>::value_type *value; value_type invoke = glUniform2fv; };
template<> struct gl_uniform<GL_FLOAT_VEC3> { using value_type = decltype(glUniform3fv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_FLOAT_VEC3>::value_type *value; value_type invoke = glUniform3fv; };
template<> struct gl_uniform<GL_FLOAT_VEC4> { using value_type = decltype(glUniform4fv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_FLOAT_VEC4>::value_type *value; value_type invoke = glUniform4fv; };
template<> struct gl_uniform<GL_INT> { using value_type = decltype(glUniform1iv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_INT>::value_type *value; value_type invoke = glUniform1iv; };
template<> struct gl_uniform<GL_INT_VEC2> { using value_type = decltype(glUniform2iv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_INT_VEC2>::value_type *value; value_type invoke = glUniform2iv; };
template<> struct gl_uniform<GL_INT_VEC3> { using value_type = decltype(glUniform3iv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_INT_VEC3>::value_type *value; value_type invoke = glUniform3iv; };
template<> struct gl_uniform<GL_INT_VEC4> { using value_type = decltype(glUniform4iv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_INT_VEC4>::value_type *value; value_type invoke = glUniform4iv; };
template<> struct gl_uniform<GL_UNSIGNED_INT> { using value_type = decltype(glUniform1uiv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_UNSIGNED_INT>::value_type *value; value_type invoke = glUniform1uiv; };
template<> struct gl_uniform<GL_UNSIGNED_INT_VEC2> { using value_type = decltype(glUniform2uiv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_UNSIGNED_INT_VEC2>::value_type *value; value_type invoke = glUniform2uiv; };
template<> struct gl_uniform<GL_UNSIGNED_INT_VEC3> { using value_type = decltype(glUniform3uiv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_UNSIGNED_INT_VEC3>::value_type *value; value_type invoke = glUniform3uiv; };
template<> struct gl_uniform<GL_UNSIGNED_INT_VEC4> { using value_type = decltype(glUniform4uiv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_UNSIGNED_INT_VEC4>::value_type *value; value_type invoke = glUniform4uiv; };
template<> struct gl_uniform<GL_BOOL> { using value_type = decltype(glUniform1uiv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_BOOL>::value_type *value; value_type invoke = glUniform1uiv; };
template<> struct gl_uniform<GL_BOOL_VEC2> { using value_type = decltype(glUniform2uiv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_BOOL_VEC2>::value_type *value; value_type invoke = glUniform2uiv; };
template<> struct gl_uniform<GL_BOOL_VEC3> { using value_type = decltype(glUniform3uiv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_BOOL_VEC3>::value_type *value; value_type invoke = glUniform3uiv; };
template<> struct gl_uniform<GL_BOOL_VEC4> { using value_type = decltype(glUniform4uiv); static constexpr bool is_matrix = false; gl_enum_to_type<GL_BOOL_VEC4>::value_type *value; value_type invoke = glUniform4uiv; };
template<> struct gl_uniform<GL_FLOAT_MAT2> { using value_type = decltype(glUniformMatrix2fv); static constexpr bool is_matrix = true; gl_enum_to_type<GL_FLOAT_MAT2>::value_type *value; value_type invoke = glUniformMatrix2fv; };
template<> struct gl_uniform<GL_FLOAT_MAT3> { using value_type = decltype(glUniformMatrix3fv); static constexpr bool is_matrix = true; gl_enum_to_type<GL_FLOAT_MAT3>::value_type *value; value_type invoke = glUniformMatrix3fv; };
template<> struct gl_uniform<GL_FLOAT_MAT4> { using value_type = decltype(glUniformMatrix4fv); static constexpr bool is_matrix = true; gl_enum_to_type<GL_FLOAT_MAT4>::value_type *value; value_type invoke = glUniformMatrix4fv; };
template<> struct gl_uniform<GL_FLOAT_MAT2x3> { using value_type = decltype(glUniformMatrix2x3fv); static constexpr bool is_matrix = true; gl_enum_to_type<GL_FLOAT_MAT2x3>::value_type *value; value_type invoke = glUniformMatrix2x3fv; };
template<> struct gl_uniform<GL_FLOAT_MAT2x4> { using value_type = decltype(glUniformMatrix2x4fv); static constexpr bool is_matrix = true; gl_enum_to_type<GL_FLOAT_MAT2x4>::value_type *value; value_type invoke = glUniformMatrix2x4fv; };
template<> struct gl_uniform<GL_FLOAT_MAT3x2> { using value_type = decltype(glUniformMatrix3x2fv); static constexpr bool is_matrix = true; gl_enum_to_type<GL_FLOAT_MAT3x2>::value_type *value; value_type invoke = glUniformMatrix3x2fv; };
template<> struct gl_uniform<GL_FLOAT_MAT3x4> { using value_type = decltype(glUniformMatrix3x4fv); static constexpr bool is_matrix = true; gl_enum_to_type<GL_FLOAT_MAT3x4>::value_type *value; value_type invoke = glUniformMatrix3x4fv; };
template<> struct gl_uniform<GL_FLOAT_MAT4x2> { using value_type = decltype(glUniformMatrix4x2fv); static constexpr bool is_matrix = true; gl_enum_to_type<GL_FLOAT_MAT4x2>::value_type *value; value_type invoke = glUniformMatrix4x2fv; };
template<> struct gl_uniform<GL_FLOAT_MAT4x3> { using value_type = decltype(glUniformMatrix4x3fv); static constexpr bool is_matrix = true; gl_enum_to_type<GL_FLOAT_MAT4x3>::value_type *value; value_type invoke = glUniformMatrix4x3fv;
};

using gl_uniform_t = std::variant<
    gl_uniform<GL_FLOAT>,
    gl_uniform<GL_FLOAT_VEC2>,
    gl_uniform<GL_FLOAT_VEC3>,
    gl_uniform<GL_FLOAT_VEC4>,
    gl_uniform<GL_INT>,
    gl_uniform<GL_INT_VEC2>,
    gl_uniform<GL_INT_VEC3>,
    gl_uniform<GL_INT_VEC4>,
    gl_uniform<GL_UNSIGNED_INT>,
    gl_uniform<GL_UNSIGNED_INT_VEC2>,
    gl_uniform<GL_UNSIGNED_INT_VEC3>,
    gl_uniform<GL_UNSIGNED_INT_VEC4>,
    gl_uniform<GL_BOOL>,
    gl_uniform<GL_BOOL_VEC2>,
    gl_uniform<GL_BOOL_VEC3>,
    gl_uniform<GL_BOOL_VEC4>,
    gl_uniform<GL_FLOAT_MAT2>,
    gl_uniform<GL_FLOAT_MAT3>,
    gl_uniform<GL_FLOAT_MAT4>,
    gl_uniform<GL_FLOAT_MAT2x3>,
    gl_uniform<GL_FLOAT_MAT2x4>,
    gl_uniform<GL_FLOAT_MAT3x2>,
    gl_uniform<GL_FLOAT_MAT3x4>,
    gl_uniform<GL_FLOAT_MAT4x2>,
    gl_uniform<GL_FLOAT_MAT4x3>>;

template<uint32_t V> struct gl_texture_parameter;
template<> struct gl_texture_parameter<GL_FLOAT> { using value_type = decltype(glTexParameterfv); value_type invoke = glTexParameterfv; };
template<> struct gl_texture_parameter<GL_INT> { using value_type = decltype(glTexParameteriv); value_type invoke = glTexParameteriv; };
template<> struct gl_texture_parameter<GL_UNSIGNED_INT> { using value_type = decltype(glTexParameterIuiv); value_type invoke = glTexParameterIuiv; };

template<uint32_t V> struct gl_texture_image;
template<> struct gl_texture_image<1> { using value_type = decltype(glTexImage1D); value_type invoke = glTexImage1D; };
template<> struct gl_texture_image<2> { using value_type = decltype(glTexImage2D); value_type invoke = glTexImage2D; };
template<> struct gl_texture_image<3> { using value_type = decltype(glTexImage3D); value_type invoke = glTexImage3D; };

template<uint32_t V> struct gl_texture_subimage;
template<> struct gl_texture_subimage<1> { using value_type = decltype(glTexSubImage1D); value_type invoke = glTexSubImage1D; };
template<> struct gl_texture_subimage<2> { using value_type = decltype(glTexSubImage2D); value_type invoke = glTexSubImage2D; };
template<> struct gl_texture_subimage<3> { using value_type = decltype(glTexSubImage3D); value_type invoke = glTexSubImage3D; };

struct data_descriptor_element_t {
public:
  template <typename T> static constexpr data_descriptor_element_t build(){ return data_descriptor_element_t(T::length(), type_to_gl_enum<typename T::value_type>::value, sizeof(T)); };
  const uint32_t elements;
  const uint32_t gl_enum;
  const uint32_t length;
private:
  constexpr data_descriptor_element_t(const uint32_t elements, const uint32_t gl_enum, const uint32_t length) : elements{elements}, gl_enum{gl_enum}, length{length} {};
};

enum class GeometryPrimitive : uint32_t {
  POINTS = GL_POINTS,
  LINE_STRIP = GL_LINE_STRIP,
  LINE_LOOP = GL_LINE_LOOP,
  LINES = GL_LINES,
  LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
  LINES_ADJACENCY = GL_LINES_ADJACENCY,
  TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  TRIANGLE_FAN = GL_TRIANGLE_FAN,
  TRIANGLES = GL_TRIANGLES,
  TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
  TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY
};

inline constexpr int gl_log_level = GL_LOG_LEVEL;

const std::map<GLuint, std::string_view> gl_error_to_sv_lookup {
  {GL_NO_ERROR, "GL_NO_ERROR"},
  {GL_INVALID_ENUM, "GL_INVALID_ENUM"},
  {GL_INVALID_VALUE, "GL_INVALID_VALUE"},
  {GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
  {GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
  {GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"}
};

inline void gl_log_error() {
  if constexpr (gl_log_level > 0) {
    GLuint gl_error_value = glGetError();
    if (gl_error_value == GL_NO_ERROR) return;
    auto error_sv = gl_error_to_sv_lookup.at(gl_error_value);
    logger::error("GL Error: [%d]: %*s", gl_error_value, static_cast<int>(error_sv.size()), error_sv.data());
    if constexpr (gl_log_level > 1) debug_break();
  }
}

template<typename callback_t, typename... Args, typename R = typename return_type<callback_t>::type, bool return_is_void = std::is_same<R, void>::value>
[[gnu::always_inline]] inline auto gl_assert_call(callback_t fn, Args const&... args) -> R {
  if constexpr (!return_is_void) {
    auto value = fn(args...);
    gl_log_error();
    return value;
  } else {
    fn(args...);
    gl_log_error();
  }
}

}
