#ifndef OPENGL_HPP
#define OPENGL_HPP

#  include <GL/glew.h>
#  include <iostream>

namespace OpenGL
{

#  ifdef CHECK_OPENGL
#    define glCheck(expr) expr; OpenGL::glCheckError(__FILE__, __LINE__, #expr);
#  else
#    define glCheck(expr) expr;
#  endif

void glCheckError(const char* file, uint32_t line, const char* expression);

} // namespace simGL Configure OpenGL

#endif
