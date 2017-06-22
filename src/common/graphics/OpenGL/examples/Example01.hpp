#ifndef EXAMPLE_01_HPP_
#  define EXAMPLE_01_HPP_

#  include "GLWindow.hpp"
#  include "GLVertexArray.hpp"
#  include "GLShader.hpp"
#  include "GLVertexBuffer.tpp"
#  include "GLAttrib.hpp"
#  include "GLTextures.hpp"

class GLExample01: public IGLWindow
{
public:

  GLExample01()
    : m_shader("Shader01"),
      m_vao("VAO"), 
      m_pos("vert"),
      m_col("color")
  {
  }

  ~GLExample01()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual bool setup() override;
  virtual bool draw() override;

private:

  void moveMe();

public:

  GLShader m_shader;
  GLVertexArray m_vao;
  GLVertexBuffer<float, 4U> m_pos;
  GLVertexBuffer<float, 4U> m_col;
};

#  endif /* EXAMPLE_01_HPP_ */
