#ifndef EXAMPLE_03_HPP_
#  define EXAMPLE_03_HPP_

#  include "GLWindow.hpp"
#  include "Color.hpp"
#  include "Vector3D.hpp"
#  include "GLShader.hpp"
#  include "GLVertexArray.hpp"
#  include "GLVertexBuffer.tpp"
#  include "GLAttrib.hpp"
#  include "GLTextures.hpp"

class GLExample03: public IGLWindow
{
public:

  GLExample03()
    : m_shader("Shader03"),
      m_vao("VAO"), 
      m_pos("vert"),
      m_texture("tex")
  {
  }

  ~GLExample03()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual bool setup() override;
  virtual bool draw() override;

  GLShader m_shader;
  GLVertexArray m_vao;
  GLVertexBuffer<float, 4U> m_pos;
  GLTexture2D m_texture;
};

#  endif /* EXAMPLE_03_HPP_ */
