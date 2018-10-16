#ifndef EXAMPLE_01_HPP_
#  define EXAMPLE_01_HPP_

#  include "OpenGL.hpp"

class GLExample01: public IGLWindow
{
public:

  GLExample01()
    : m_quad("quad")
  {
  }

  ~GLExample01()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual bool setup() override;
  virtual bool draw() override;

public:

  GLVertexShader   m_vs;
  GLFragmentShader m_fs;
  GLProgram        m_quad;
};

#  endif /* EXAMPLE_01_HPP_ */
