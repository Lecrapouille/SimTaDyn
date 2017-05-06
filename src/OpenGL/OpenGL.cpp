#include "OpenGL.hpp"
#include "SimTaDynLogger.hpp"
#include <iostream>
#include <gtkmm/glarea.h>

namespace SimTaDyn
{
  static bool _context_started = false;

  //! \brief GLArea only support Core profile.
  void glStartContext()
  {
    try
      {
        glewExperimental = true;
        GLenum err = glewInit();
        if (err != GLEW_OK)
          {
            const GLubyte* msg = glewGetErrorString(err);
            const char *m = reinterpret_cast<const char*>(msg);
            throw Gdk::GLError(Gdk::GLError::NOT_AVAILABLE, Glib::ustring(m));
          }
        _context_started = true;
      }
    catch (const Gdk::GLError& gle)
      {
        LOGES("An error occured during the creation of OpenGL context:");
        std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
      }
  }

  bool glIsFunctional()
  {
    return _context_started;
  }

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
} // namespace
