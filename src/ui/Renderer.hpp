#ifndef RENDERER_HPP_
#  define RENDERER_HPP_

#  include "Color.hpp"
#  include "GLShader.hpp"
#  include "GLVertexArray.hpp"
#  include "GLVertexBuffer.hpp"
#  include "GLAttrib.hpp"
//#  include "Camera2D.hpp"
#  include "GLTextures.hpp"
#  include "Movable.tpp"

/*
 * OpenGL renderer.
 */
class GLRenderer
{
public:

  // FIXME: utiliser le pattern State pour empiler des GLRenderer
  enum RenderMode { Mode2D, Mode3D };

  //! \brief Empty constructor. It does not setup graphics because the
  //! OpenGL can be not yet initialized.
  GLRenderer()
    : //m_default_camera(),
      //m_current_camera(),
      m_background_color(Color::Black),
      m_shader("Shader01"),
      m_vao("VAO"),
      m_pos("vert"),
      m_tex("vertTexCoord"),
      m_mode(GLRenderer::Mode2D)
  {
  }

  //! \brief Delete buffers, textures and program.
  ~GLRenderer()
  {
  }

  //! \brief Set the background color. This color is used for clearing
  //! the screen.
  inline void backgroundColor(const Color& color)
  {
    m_background_color = color;
  }

  //! \brief Get the background color. This color is used for clearing
  //! the screen.
  inline Color backgroundColor() const
  {
    return m_background_color;
  }

  //! \brief Change the renderer mode
  inline void renderStyle(const RenderMode style)
  {
    m_mode = style;
  }

  //! \brief Get the current renderer mode
  inline RenderMode renderStyle() const
  {
    return m_mode;
  }

  //! Get the number of pixels of the viewport
  virtual uint32_t screenWidth() const = 0;

  //! Get the number of pixels of the viewport
  virtual uint32_t screenHeight() const = 0;

protected:

  //! \brief Same role than a constructor but here we are sure than
  //! and OpenGL context is created before.
  bool setupGraphics();

  //! \brief
  inline void clearScreen() const;

  //! \brief
  void draw();

  virtual void setContext() = 0;
  virtual void activateTransparency() = 0;
  virtual void activateDepthBuffer() = 0;

  //FIXME
  void setUniform(const char *name, Matrix44f const &mat);

protected:

  Color m_background_color;
  GLShader m_shader;
  GLVertexArray m_vao;
  GLVertexBuffer<float> m_pos;
  //GLVertexBuffer<float> m_col;
  GLVertexBuffer<float> m_tex;
  Movable<float, 3U> m_movable;
  GLTexture2D m_texture;
  RenderMode m_mode;
};

#endif /* RENDERER_HPP_ */
