#ifndef EXAMPLE_04_HPP_
#  define EXAMPLE_04_HPP_

#  include "GLWindow.hpp"
#  include "Cube.hpp"

class GLExample04: public IGLWindow
{
public:

  GLExample04()
    : m_shader("Shader04")
  {
  }

  ~GLExample04()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual bool setup() override;
  virtual bool draw() override;
  void setUniform(const char *name, Matrix44f const &mat);

  GLShader m_shader;
  MeshCube m_cube;
  Movable<float, 3U> movable;
};

#  endif /* EXAMPLE_04_HPP_ */
