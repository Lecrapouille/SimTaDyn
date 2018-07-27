#ifndef OPENGL_HPP
#define OPENGL_HPP

#  include <GL/glew.h>

namespace SimTaDyn
{

#  ifdef CHECK_OPENGL
#    define glCheck(expr) expr; SimTaDyn::glCheckError(__FILE__, __LINE__, #expr);
#  else
#    define glCheck(expr) expr;
#  endif

  void glCheckError(const char* file, uint32_t line, const char* expression);
  void glStartContext();
  bool glIsFunctional();

} // namespace

#endif
