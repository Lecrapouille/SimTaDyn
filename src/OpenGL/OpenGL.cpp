#include "OpenGL.hpp"
#include <iostream>

namespace OpenGL
{

  void glCheckError(const char *file, uint32_t line, const char* expression)
  {
    GLenum id;

    while ((id = glGetError()) != GL_NO_ERROR)
      {
        const char* error;
        std::string fileString = file;

        switch (id)
          {
          case GL_INVALID_OPERATION:
            error = "GL_INVALID_OPERATION";
            break;
          case GL_INVALID_ENUM:
            error = "GL_INVALID_ENUM";
            break;
          case GL_INVALID_VALUE:
            error = "GL_INVALID_VALUE";
            break;
          case GL_OUT_OF_MEMORY:
            error = "GL_OUT_OF_MEMORY";
            break;
          case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
          default:
            error = "UNKNOWN";
            break;
          }

        std::cerr << "[ERROR][OPENGL][" << error << "] "
                  << fileString.substr(fileString.find_last_of("\\/") + 1)
                  << ":" << line
                  << " failed executing " << expression
                  << std::endl;
      }
  }

} // namespace OpenGL
