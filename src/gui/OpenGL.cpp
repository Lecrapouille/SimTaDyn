#include "OpenGL.hpp"
#include "simtypes.hpp"

namespace simGL
{

  void glCheckError(const char *file, uint32_t line, const char* expression)
  {
    GLenum err = glGetError();

    while (err != GL_NO_ERROR)
      {
        std::string error;
        std::string fileString = file;

        switch (err)
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
            error = "Unknown error (" + std::to_string(err) + ")";
            break;
          }

        std::cerr << "[ERROR] "
                  << fileString.substr(fileString.find_last_of("\\/") + 1)
                  << ":" << line
                  << " OpenGL failed executing " << expression
                  << std::endl;
      }
  }

} // namespace simGL
