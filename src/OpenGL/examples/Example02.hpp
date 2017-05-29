#ifndef EXAMPLE_02_HPP_
#  define EXAMPLE_02_HPP_

#  include "GLWindow.hpp"
#  include "Color.hpp"
#  include "Vector3D.hpp"
#  include "GLShader.hpp"
#  include "GLVertexArray.hpp"
#  include "GLVertexBuffer.tpp"
#  include "GLAttrib.hpp"
#  include "GLTextures.hpp"

class GLExample02: public IGLWindow
{
public:

  GLExample02()
    : m_shader("Shader02"),
      m_vao("VAO"), 
      m_pos("vert"),
      m_col("color"),
      m_tex("vertTexCoord")
  {
  }

  ~GLExample02()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual bool setup() override;
  virtual bool draw() override;

  GLShader m_shader;
  GLVertexArray m_vao;
  GLVertexBuffer<float, 4U> m_pos;
  GLVertexBuffer<float, 4U> m_col;
  GLVertexBuffer<float, 4U> m_tex;
  GLTexture2D m_texture;
};

#  endif /* EXAMPLE_02_HPP_ */
