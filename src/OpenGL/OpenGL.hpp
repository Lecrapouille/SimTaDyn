#ifndef OPENGL_HPP_
#  define OPENGL_HPP_

#  include <GL/glew.h>
#  include <gtkmm.h>

namespace simGL
{
  GLuint createShaderProgram(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path);
}

#endif /* OPENGL_HPP_ */
