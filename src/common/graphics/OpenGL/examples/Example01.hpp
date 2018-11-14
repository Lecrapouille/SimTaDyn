#ifndef EXAMPLE_01_HPP_
#  define EXAMPLE_01_HPP_

#  include "OpenGL.hpp"
#  include "Movable.tpp"

class GLExample01: public IGLWindow
{
public:

  GLExample01()
    : m_vao_quad("VAO_quad"),
      m_vao_floor("VAO_floor"),
      m_prog("prog")
  {
  }

  ~GLExample01()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual bool setup() override;
  virtual bool draw() override;

public:

  GLVertexShader     vs;
  GLFragmentShader   fs;
  GLVAO              m_vao_quad;
  GLVAO              m_vao_floor;
  GLProgram          m_prog;
  Movable<float, 3U> m_movable1;
  Movable<float, 3U> m_movable2;
  Movable<float, 3U> m_movable3;
};

#  endif /* EXAMPLE_01_HPP_ */
