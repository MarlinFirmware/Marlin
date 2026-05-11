#pragma once

#include <glad/glad.h>
#if defined(__APPLE__) && !defined(__MESA__)
  #include <OpenGL/gl.h>
#else
  #include <GL/gl.h>
#endif
