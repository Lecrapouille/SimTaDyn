#ifndef EXAMPLE_03_HPP_
#  define EXAMPLE_03_HPP_

#  include "GLWindow.hpp"
#  include "GLShader.hpp"
#  include "GLVertexArray.hpp"
#  include "GLVertexBuffer.hpp"
#  include "GLAttrib.hpp"
#  include "GLTextures.hpp"
#  include "Movable.tpp"

class GLExample03: public IGLWindow
{
public:

  GLExample03()
    : m_shader("Shader03"),
      m_vao("VAO"),
      m_pos("vert"),
      m_tex("vertTexCoord")
  {
  }

  ~GLExample03()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual bool setup() override;
  virtual bool draw() override;
  void setUniform(const char *name, Matrix44f const &mat);
  void moveMe();

  GLShader m_shader;
  GLVertexArray m_vao;
  GLVertexBuffer<float> m_pos;
  GLVertexBuffer<float> m_tex;
  GLTexture2D m_texture;
};

#  endif /* EXAMPLE_03_HPP_ */
