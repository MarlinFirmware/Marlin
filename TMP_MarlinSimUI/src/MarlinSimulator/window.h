#pragma once

#include <string>
#include <vector>

#include <gl.h>

#include "renderer/renderer.h"


enum WindowReturnCode {
  WINDOW_OK,
  SDL_INIT_FAILED,
  SDL_CREATE_WINDOW_FAILED,
  SDL_GL_CREATECONTEXT_FAILED,
  GLEW_INIT_FAILED,
  IMGUI_NULL_WINDOW,
  IMGUI_SDLINIT_FAIL,
  IMGUI_GLINIT_FAIL
};

struct WindowConfig {
  enum GlProfile { CORE = 0x0001, COMPATIBILITY = 0x0002, ES = 0x0004 };

  std::string title       = "Window";
  GLuint gl_version_major = 3, gl_version_minor = 3, gl_profile = GlProfile::CORE, multisamples = 4, vsync = 1;
};

class Window {
public:
  Window() {}
  ~Window();

  void init(WindowConfig config = {});
  void* getHandle();
  void swap_buffers();
};

namespace opengl_util {

struct FrameBuffer {
  virtual bool update(GLuint w, GLuint h) = 0;
  virtual void release()                  = 0;
  virtual void bind()                     = 0;
  virtual void render()                   = 0;
  virtual void unbind()                   = 0;
  virtual GLuint texture_id()             = 0;

  virtual ~FrameBuffer() { }
};

struct MsaaFrameBuffer : public FrameBuffer {

  bool create(GLuint w, GLuint h, GLint msaa) {
    msaa_levels = msaa;
    return update(w, h);
  }

  bool update(GLuint w, GLuint h) {
    if ((w == width && h == height) || h == 0 || w == 0) {
      return true;
    }

    width  = w;
    height = h;

    if (color_attachment_id == 0) {
      renderer::gl_assert_call(glGenTextures, 1, &color_attachment_id);
    }
    renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, color_attachment_id);
    renderer::gl_assert_call(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (framebuffer_msaa_id == 0) {
      renderer::gl_assert_call(glGenFramebuffers, 1, &framebuffer_msaa_id);
    }
    renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, framebuffer_msaa_id);

    if (color_attachment_msaa_id == 0) {
      renderer::gl_assert_call(glGenRenderbuffers, 1, &color_attachment_msaa_id);
    }
    renderer::gl_assert_call(glBindRenderbuffer, GL_RENDERBUFFER, color_attachment_msaa_id);
    renderer::gl_assert_call(glRenderbufferStorageMultisample, GL_RENDERBUFFER, msaa_levels, GL_RGBA8, width, height);
    renderer::gl_assert_call(glBindRenderbuffer, GL_RENDERBUFFER, 0);

    if (depth_attachment_msaa_id == 0) {
      renderer::gl_assert_call(glGenRenderbuffers, 1, &depth_attachment_msaa_id);
    }
    renderer::gl_assert_call(glBindRenderbuffer, GL_RENDERBUFFER, depth_attachment_msaa_id);
    renderer::gl_assert_call(glRenderbufferStorageMultisample, GL_RENDERBUFFER, msaa_levels, GL_DEPTH_COMPONENT, width, height);
    renderer::gl_assert_call(glBindRenderbuffer, GL_RENDERBUFFER, 0);
    renderer::gl_assert_call(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_attachment_msaa_id);
    renderer::gl_assert_call(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_attachment_msaa_id);

    if (framebuffer_id == 0) {
      renderer::gl_assert_call(glGenFramebuffers, 1, &framebuffer_id);
    }
    renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, framebuffer_id);

    if (render_buffer_id == 0) {
      renderer::gl_assert_call(glGenRenderbuffers, 1, &render_buffer_id);
    }
    renderer::gl_assert_call(glBindRenderbuffer, GL_RENDERBUFFER, render_buffer_id);
    renderer::gl_assert_call(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    renderer::gl_assert_call(glBindRenderbuffer, GL_RENDERBUFFER, 0);
    renderer::gl_assert_call(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_id, 0);
    renderer::gl_assert_call(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer_id);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      release();
      return false;
    }

    return true;
  }

  void release() {
    renderer::gl_assert_call(glDeleteRenderbuffers, 1, &render_buffer_id);
    render_buffer_id = 0;
    renderer::gl_assert_call(glDeleteFramebuffers, 1, &framebuffer_id);
    framebuffer_id = 0;
    renderer::gl_assert_call(glDeleteRenderbuffers, 1, &depth_attachment_msaa_id);
    depth_attachment_msaa_id = 0;
    renderer::gl_assert_call(glDeleteRenderbuffers, 1, &color_attachment_msaa_id);
    color_attachment_msaa_id = 0;
    renderer::gl_assert_call(glDeleteFramebuffers, 1, &framebuffer_msaa_id);
    framebuffer_msaa_id = 0;
    renderer::gl_assert_call(glDeleteTextures, 1, &color_attachment_id);
    color_attachment_id = 0;
  }

  void bind() {
    if (framebuffer_msaa_id) {
      renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, framebuffer_msaa_id);

      renderer::gl_assert_call(glViewport, 0, 0, width, height);
    }
  }

  void render() {
    renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    renderer::gl_assert_call(glBindFramebuffer, GL_READ_FRAMEBUFFER, framebuffer_msaa_id);
    renderer::gl_assert_call(glBindFramebuffer, GL_DRAW_FRAMEBUFFER, framebuffer_id);
    renderer::gl_assert_call(glBlitFramebuffer, 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, 0);
  }

  void unbind() {
    renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, 0);
  }

  GLuint texture_id() {
    return color_attachment_id;
  }

  GLuint framebuffer_id = 0, framebuffer_msaa_id = 0, color_attachment_id = 0, color_attachment_msaa_id = 0, depth_attachment_msaa_id = 0, render_buffer_id = 0,
         width = 0, height = 0;
  GLint msaa_levels = 0;
};

struct TextureFrameBuffer : public FrameBuffer {
  bool create(GLuint w, GLuint h) {
    return update(w, h);
  }

  bool update(GLuint w, GLuint h) {
    if ((w == width && h == height) || h == 0 || w == 0) {
      return true;
    }

    width  = w;
    height = h;

    if (color_attachment_id == 0) {
      renderer::gl_assert_call(glGenTextures, 1, &color_attachment_id);
    }
    renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, color_attachment_id);
    renderer::gl_assert_call(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (depth_attachment_id == 0) {
      renderer::gl_assert_call(glGenTextures, 1, &depth_attachment_id);
    }
    renderer::gl_assert_call(glBindTexture, GL_TEXTURE_2D, depth_attachment_id);
    renderer::gl_assert_call(glTexImage2D, GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    renderer::gl_assert_call(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (framebuffer_id == 0) {
      renderer::gl_assert_call(glGenFramebuffers, 1, &framebuffer_id);
    }
    renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, framebuffer_id);
    renderer::gl_assert_call(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_id, 0);
    renderer::gl_assert_call(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_attachment_id, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      release();
      return false;
    }
    renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    return true;
  }

  void release() {
    renderer::gl_assert_call(glDeleteFramebuffers, 1, &framebuffer_id);
    framebuffer_id = 0;
    renderer::gl_assert_call(glDeleteTextures, 1, &depth_attachment_id);
    depth_attachment_id = 0;
    renderer::gl_assert_call(glDeleteTextures, 1, &color_attachment_id);
    color_attachment_id = 0;
  }

  void bind() {
    if (framebuffer_id) {
      renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, framebuffer_id);

      renderer::gl_assert_call(glViewport, 0, 0, width, height);
    }
  }

  void render() {
    unbind();
  }

  void unbind() {
    renderer::gl_assert_call(glBindFramebuffer, GL_FRAMEBUFFER, 0);
  }

  GLuint texture_id() {
    return color_attachment_id;
  }

  GLuint framebuffer_id = 0, color_attachment_id = 0, depth_attachment_id = 0, width = 0, height = 0;
};

} // namespace opengl_util
