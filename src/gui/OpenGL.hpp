#ifndef OPENGL_HPP_
#  define OPENGL__HPP_

#  ifdef __APPLE__
#    include <GLUT/glut.h>
#    include <OpenGL/gl.h>
#    include <OpenGL/glu.h>
#  else
#    include <GL/glut.h>
#    include <GL/gl.h>
#    include <GL/glu.h>
#  endif

namespace simGL
{
#  ifdef CHECK_OPENGL
#    define glCheck(expr) expr; simGL::glCheckError(__FILE__, __LINE__, #expr);
#  else
#    define glCheck(expr) (expr)
#  endif

void glCheckError(const char* file, uint32_t line, const char* expression);

} // namespace simGL

#endif /* OPENGL_HPP_ */
