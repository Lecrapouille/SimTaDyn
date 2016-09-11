#ifndef OPGL_EXT_HPP_
#  define OPGL_EXT_HPP_

#  ifdef __APPLE__
#    include <GLUT/glut.h>
#    include <OpenGL/gl.h>
#    include <OpenGL/glu.h>
#  else
#    include <GL/glut.h>
#    include <GL/gl.h>
#    include <GL/glu.h>
#  endif

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)
void _check_gl_error(const char *file, int line);

#endif /* OPGL_EXT_HPP_ */
