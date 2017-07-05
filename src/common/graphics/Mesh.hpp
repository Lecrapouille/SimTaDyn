#ifndef MESH_HPP_
#  define MESH_HPP_

#  include "GLVertexArray.hpp"
#  include "GLVertexBuffer.hpp"
#  include "GLTextures.hpp"
#  include "Movable.tpp"
//#  include "Renderable.hpp"

//TODO ajouter visible

class Mesh//: public Movable<float, 3U> // , public Renderable
{
public:

  Mesh()
    : m_pos("vert"),
      //m_col("color"),
      m_tex("vertTexCoord"),
      m_shader(nullptr)
  {
  }

  ~Mesh()
  {
  }

  bool setup(GLShader &shader)
  {
    m_shader = &shader;

    m_texture.interpolation(GL_LINEAR);
    m_texture.wrapping(GL_CLAMP_TO_EDGE);
    if (false == m_texture.load("wooden-crate.jpg"))
      return false;

    m_pos.setup(*m_shader, 3, GL_FLOAT);
    //m_col.setup(*m_shader, 3, GL_FLOAT);
    m_tex.setup(*m_shader, 2, GL_FLOAT);

    m_vao.start();
    {
      m_pos.start();
      //m_col.start();
      m_tex.start();
    }
    m_vao.stop();

    return true;
  }

  void draw(GLuint const type)
  {
    m_vao.start();
    {
      m_pos.start();
      //m_col.start();

      // Set to 0 because the texture is bound to GL_TEXTURE0
      TextureID = glCheck(glGetUniformLocation(*m_shader, "tex"));
      glCheck(glUniform1i(TextureID, 0));
      glCheck(glActiveTexture(GL_TEXTURE0));
      m_texture.start();

      //std::cout << m_pos.size() << std::endl;
      glCheck(glDrawArrays(type, 0, m_pos.size() / 3));

      m_texture.stop();
      //m_col.stop();
      m_pos.stop();
    }
    m_vao.stop();
  }

  GLVertexBuffer<float> m_pos;
  //GLVertexBuffer<float> m_col;
  GLVertexBuffer<float> m_tex;
  GLTexture2D m_texture;
  GLShader *m_shader;

protected:

  GLint TextureID;
  GLVertexArray m_vao;
};

#endif